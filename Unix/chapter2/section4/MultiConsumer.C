#include <iostream.h>
#include <fstream.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <XPCPthread.h>
#include <XPCSignal.h>
#include <XPCMultiConsumer.h>

sigset_t newSigSet;        // The new thread signal mask
int iConsumerThreadCount;    // Number of consumer threads running
XPCMessage globalMsg;    // Consumer thread message holder

void vKillThreadSignalHandler(int _iSig)
{
    // Called when the SIGUSR1 signal is caught.

    // Sets the exit status object for the thread
    XPCExitStatus *newStatus = new XPCExitStatus(1, "Received Signal and Exiting Consumer Thread");

    // Exits the thread with the appropriate status
    pthread_exit((void *)newStatus);
}

void *vConsumerThread(void *_vArg)
{
    // Consumer thread
    XPCThread *threadInfo = (XPCThread *)_vArg;

    // Holds the producer's message
    char sProducerMessage[512];

    cout << "Thread started to monitor: " << threadInfo->sGetFilename() << endl;

    // Signal handler to process SIGUSR1 signals
    try
    {
         XPCSignal killThreadSigHandler(SIGUSR1, vKillThreadSignalHandler);
    }
    catch(XPCException &exceptOb)
    {
        cout << exceptOb.sGetException() << endl;
        XPCExitStatus *newStatus = new XPCExitStatus(2, "Error creating thread signal handler");
        return (void *)newStatus;
    }

    // Unblock blocked signals 
    pthread_sigmask(SIG_UNBLOCK, &newSigSet, NULL);

    try
    {
        // Open the named semaphore
        XPCSemaphore namedSem(threadInfo->sGetSemname(), OPEN_SEM);

        while(1)
        {
            // Lock the named semaphore
            cout << threadInfo->sGetSemname() << endl; 
            namedSem.vLockWait();
        
            // Open the shared file    
            ifstream fin(threadInfo->sGetFilename());
            fin >> sProducerMessage;
            fin.close();

            // Remove the contents of the shared file
            ofstream fout(threadInfo->sGetFilename());
            fout.close();

            // Unlock the named semaphore
            namedSem.vUnlock();

            // Lock the mesage buffer semaphore
            threadInfo->getSem()->vLockWait();

            globalMsg.sPutMessage(sProducerMessage);
            globalMsg.sPutFilename(threadInfo->sGetFilename());    
        
            // Unlock the message buffer semaphore    
            threadInfo->getSem()->vUnlock();

            // Unlock the thread's main unamed sempahore indicating new data
            // has been placed in global memory
            threadInfo->getMainSem()->vUnlock();

            sleep(1);
        }
    }
    catch(XPCException &myExcept)
    {
        cout << myExcept.sGetException() << endl;

        // Return the threads status message with the exception text included
        XPCExitStatus *newStatus = new XPCExitStatus(2, myExcept.sGetException());
        return (void *)newStatus;
    }
}

void *vWaitForConsumerThread(void *_vArg)
{
    XPCPthread<XPCExitStatus> *threadPtr = (XPCPthread<XPCExitStatus> *)_vArg;

    // Wait for the consumer thread to complete
    threadPtr->vWaitForThread();

    // Retrieve and display the status for the completed thread
    XPCExitStatus *Status = threadPtr->getStatus();
    cout << "Status: " << Status->sGetMsg() << endl;
    delete Status;

    // Reduce the number of consumer threads running    
    iConsumerThreadCount--;

    delete threadPtr;

    // If no consumer threads are running the consumer process exits
    if(iConsumerThreadCount == 0)
        exit(0);

    return 0;
}
    
main(int argc, char *argv[])
{
    // An array of upto 100 consumer threads is created
    XPCPthread<XPCExitStatus> *newThread[100];

    // The number of consumer threads is initialized to 0
    iConsumerThreadCount = 0;

    try
    {
        // Semaphore to protect the shared global message object
        XPCSemaphore threadSem((int)PRIVATE_SEM, (unsigned int)1);

        // Semaphore to notify the main thread that a message has been received
        XPCSemaphore mainSem((int)PRIVATE_SEM, (unsigned int)0);

        // A signal mask is created that only holds SIGUSR1
        sigemptyset(&newSigSet);
        sigaddset(&newSigSet, SIGUSR1);

        // Block the signals in the signal mask
        pthread_sigmask(SIG_BLOCK, &newSigSet, NULL);

        for (int count = 1; count < argc; count += 2)
        {
            // Create a consumer thread and a thread to monitor the consumer thread
            XPCThread *newThreadClass = new XPCThread(argv[count], argv[count+1], &threadSem, &mainSem);
            newThread[iConsumerThreadCount] = new XPCPthread<XPCExitStatus>(vConsumerThread, (void *)newThreadClass);
            XPCPthread<int> threadWait(vWaitForConsumerThread, (void *)newThread[iConsumerThreadCount]);
            iConsumerThreadCount++;
        }

        // Process the received messages
        while(1)    // Loop forever
        {
            // Lock the semaphore indicating a message is received
            mainSem.vLockWait();

            cout << "Message Recieved: " << globalMsg.sGetMessage() << endl;
            cout << "Message Queue: " << globalMsg.sGetFilename() << endl;

            // If the message is QUIT signals are sent to all threads to terminate
            // themselves
            if (strcmp(globalMsg.sGetMessage(), "QUIT") == 0)
            {
                int iThreadCounter = 0;
                for (int count = 2; count < argc; count += 2)
                {
                    if (strcmp(globalMsg.sGetFilename(), argv[count]) == 0)
                    {
                        cout << "Kill Thread: " << iThreadCounter << endl;
                        newThread[iThreadCounter]->vSignalThread(SIGUSR1);
                        XPCExitStatus *Status = newThread[iThreadCounter]->getStatus();
                        break;
                    }
                    iThreadCounter++;
                }                
            }
        }
    }
    catch(XPCException &exceptOb)
    {
        cout << exceptOb.sGetException() << endl;
    }
}    
