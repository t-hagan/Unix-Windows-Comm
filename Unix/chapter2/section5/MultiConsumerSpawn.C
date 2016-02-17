#include <iostream.h>
#include <fstream.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <XPCPthread.h>
#include <XPCSignal.h>
#include <XPCMultiConsumer.h>
#include <XPCWaitPid.h>

sigset_t newSigSet;        // The new thread signal mask
int iConsumerThreadCount;    // Number of consumer threads running
XPCMessage globalMsg;    // Consumer thread message holder

class XPCProducerInfo
{
        char *sFilename;        // The shared filename
        char *sSemname; // The name of the named semaphore

        // The associated consumer thread object.
        XPCPthread<XPCExitStatus> *consumerThread;
public:
        // The constructor.
        XPCProducerInfo(char *_sSemname, char *_sFilename, XPCPthread<XPCExitStatus> *_consumerThread)
        {
                sFilename = new char[strlen(_sFilename)+1];
                sSemname = new char[strlen(_sSemname)+1];

                strcpy(sFilename, _sFilename);
                strcpy(sSemname, _sSemname);
                consumerThread = _consumerThread;
        }

        // The copy constructor.
        XPCProducerInfo(const XPCProducerInfo &_origProducerInfoClass)
        {
                sFilename = new char[strlen(_origProducerInfoClass.sGetFilename())+1];
                sSemname = new char[strlen(_origProducerInfoClass.sGetSemname())+1];

                strcpy(sFilename, _origProducerInfoClass.sGetFilename());
                strcpy(sSemname, _origProducerInfoClass.sGetSemname());
                consumerThread = _origProducerInfoClass.getConsumerThread();
        }

        // Private data member retrieval methods.
        char *sGetFilename() const { return sFilename; }
        char *sGetSemname() const { return sSemname; }
        XPCPthread<XPCExitStatus> *getConsumerThread() const { return consumerThread; }

        // The equal operator.
        XPCProducerInfo &operator=(const XPCProducerInfo &_origProducerInfoClass)
        {
                if (this == &_origProducerInfoClass)
                        return *this;

                if (sFilename)
                        delete [] sFilename;
                if (sSemname)
                        delete [] sSemname;

        sFilename = new char[strlen(_origProducerInfoClass.sGetFilename())+1];
                sSemname = new char[strlen(_origProducerInfoClass.sGetSemname())+1];

                strcpy(sFilename, _origProducerInfoClass.sGetFilename());
                strcpy(sSemname, _origProducerInfoClass.sGetSemname());
                consumerThread = _origProducerInfoClass.getConsumerThread();

                return *this;
        }
};

void *vSpawnProcess(void *_vArg)
{
        // Cast the argument sent to the vSpawnProcess thread back to its original class,
        // XPCProducerInfo.
        XPCProducerInfo *producerInfo = (XPCProducerInfo *)_vArg;

        int iProducerProcessID; // Contains the process ID of the spawned producer.

        cout << "Spawning producer:" << endl;
        cout << "Semaphore        :" << producerInfo->sGetSemname() << endl;
        cout << "Filename         :" << producerInfo->sGetFilename() << endl;

        // Spawn the producer by execing the producer process. If the process cannot be
        // forked, the thread exits.
        if ((iProducerProcessID = fork()) == -1)
        {
                cout << "Error spawning producer" << endl;
                return 0;
        }

        // The child process execs the producer.  The producer is passed the semaphore
        // name and shared file name as its arguments.  Because the producer takes input
        // from the command line, it is spawned within a terminal.  Therefore each
        // producer gets its own terminal.
        if (iProducerProcessID == 0)
        {
                execlp("xterm", "xterm", "-e", "../section3/Producer",
                        producerInfo->sGetSemname(), producerInfo->sGetFilename(), NULL);
        }
        // The subsequent parent process waits for the child process to exit.  If the
        // child process exits, the kill signal, SIGUSR1, is sent to the consumer thread.
else
        {
                try
                {
                        int iReturnValue;       // Holds the producer process's return value

                        // Wait for the producer process to complete.
                        XPCWaitPid waitForProducer(iProducerProcessID);

                        // When the producer completes, retrieve the return status and
                        // return value and display them.
                        waitClassReturn returnStatus = waitForProducer.getReturnValue(iReturnValue);

                        cout << "Producer Process Exited:" << endl;
                        cout << "Return Status          :" << returnStatus << endl;
            cout << "Return Value           :" << iReturnValue << endl;

                        // Send a signal to kill the consumer thread.
                        producerInfo->getConsumerThread()->vSignalThread(SIGUSR1);
                        return 1;
                }

                // Catch errors waiting for the producer process or sending the consumer
                // thread a signal.
                catch(XPCException &exceptObject)
                {
                        cout << "Error Occurred Waiting for Producer: " << exceptObject.sGetException() << endl;
                        return 0;
                }
        }
}

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

    sleep(5);

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

            XPCProducerInfo *newProducerInfo = new XPCProducerInfo(argv[count], argv[count+1], newThread[iConsumerThreadCount]);

            // Spawn the producer-spawning thread
            XPCPthread<int> spawnProcessThread(vSpawnProcess, (void *)newProducerInfo);

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
        }
    }
    catch(XPCException &exceptOb)
    {
        cout << exceptOb.sGetException() << endl;
    }
}    
