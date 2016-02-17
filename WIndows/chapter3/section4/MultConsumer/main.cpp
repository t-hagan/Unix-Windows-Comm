#include <iostream.h>
#include <string.h>
#include <fstream.h>
#include <iostream.h>
#include "MultiProducer.h"
#include <XPCFileStat.h>
#include <XPCSemaphore.h>
#include "XPCThread.h"


// global variables

// count of all active threads
int iConsumerThreadCount;

// message class used by the threads to pass data
// back to main thread
XPCMessage globalMsg;


// consumer thread procedure
DWORD WINAPI ConsumerThreadProc (LPVOID lpData)
{
    // the thread info object is passed in to us
    XPCThreadInfo* threadInfo= (XPCThreadInfo*) lpData;

    char sProducerMessage [512];

    cout << "Thread started to monitor: " << 
            threadInfo->sProducerFilename() << endl;

    try
    {
        // open the semaphore from the producer
        XPCSemaphore namedSem (threadInfo->SemName(), 0, 10, 
                            OPEN_SEM);
        while (1)
        {
            // wait on the producer's semaphore
            namedSem.vLockWait();

            // when we have a lock on the producers
            // semaphore, we can read the data from the file
            ifstream fin (threadInfo->sProducerFilename());
            fin >> sProducerMessage;
            fin.close ();

            // delete the data in the file
            ofstream fout (threadInfo->sProducerFilename(), ios::app | ios::trunc);
            fout.close();

            // let the producer have the semaphore back
            namedSem.vUnlock();

            // now get the lock to semaphore that protects
            // our global data variable
            threadInfo->ThreadSem()->vLockWait();

            // write the producer's data to the 
            // global data variable
            globalMsg.Message (sProducerMessage);
            globalMsg.Filename (threadInfo->sProducerFilename());

            // unlock the main semaphore to tell the main thread
            // that there is data in the global variable
            threadInfo->MainSem()->vUnlock();

            Sleep (1);

            // if the exit event has occurred then exit
            // the thread
            if (threadInfo->ExitEvent().bWaitForEvent(0))
            {
                ExitThread (0);
            }
        }
    }
    catch (XPCException & ex)
    {
        cout << ex.sGetException() << endl;
        return -1;
    }
    return 0;
}
// thread proc that waits for each thread to terminate.
DWORD WINAPI WaitForConsumerThreadProc (LPVOID lpData)
{

    // the thread pointer class is passed to us
    XPCThread* threadPtr = (XPCThread*) lpData;

    try
    {
        // wait for our thread to terminate
        threadPtr->vWaitForThread();

        DWORD dwRet;

        // get the exit code and display it
        threadPtr->vGetExitCode (&dwRet);

        cout << "return status from thread: " << dwRet << endl;

        // clean up the object
        delete threadPtr;

        // if this is the last thread then exit the program
        if ((--iConsumerThreadCount) == 0)
            exit (0);

    }
    catch (XPCException & ex)
    {
        cout << ex.sGetException() << endl;
    }
    return 0;
}

main (int argc, char* argv[])
{
    // a thread object and a thread info 
    // object for each consumer thread
    XPCThread* consumerThread[100];
    XPCThreadInfo* consumerThreadInfo[100];

    iConsumerThreadCount= 0;

    // semaphore that protects the global message variable
    XPCSemaphore threadSem (NULL, 1);

    // semaphore that indicates to the main processing loop
    // (below) that a thread has data to be processed
    XPCSemaphore mainSem (NULL);


    // create a thread info object and a thread for each
    // producer listed on the command line
    for (int count= 1; count < argc; count+= 2)
    {
        consumerThreadInfo[iConsumerThreadCount]=
                new XPCThreadInfo (argv[count],
                            argv[count+1],
                            &threadSem,
                            &mainSem);
                            
        consumerThread[iConsumerThreadCount]= 
            new XPCThread (ConsumerThreadProc,
            (void*)consumerThreadInfo[iConsumerThreadCount]);

        XPCThread waitThread (WaitForConsumerThreadProc,
                consumerThread[iConsumerThreadCount]);

        // keep track of the number of active threads
        iConsumerThreadCount++;
    }

    while (1)
    {
        // wait till a thread has read data from a producer
        mainSem.vLockWait();

        // process the data from that thread
        cout << "Message Received: " << 
                globalMsg.sMessage() << endl;
        cout << "Message Queue: " << 
                globalMsg.sFilename() << endl;

        // if a producer sends the quit message
        // terminate the thread that is servicing
        // that producer
        if (strcmp (globalMsg.sMessage(), "QUIT") == 0)
        {
            int iThreadCount= 0;
            // first find the correct thread
            for (int count= 1; count < argc; count += 2)
            {
                // if the file names match, the we have
                // the right thread
                if (strcmp (globalMsg.sFilename(), 
                        argv[count]) == 0)
                {
                    cout << "Kill thread: " << 
                            iThreadCount << endl;

                    // send the event to kill the thread
                    consumerThreadInfo[iThreadCount]->
                            ExitEvent().vSendEvent();
                    break;
                }
                iThreadCount++;
            }
        }
        // unlock the semaphore that protects the global data 
        // variable (locked by the thread)
        threadSem.vUnlock();
    }
    return 0;
}




