#include <iostream.h>
#include <string.h>
#include <fstream.h>
#include <iostream.h>
#include <stdio.h>
#include "XPCThreadInfo.h"
#include <XPCFileStat.h>
#include <XPCSemaphore.h>
#include <XPCThread.h>
#include "XPCProcess.h"

int iConsumerThreadCount;
XPCMessage globalMsg;

DWORD WINAPI ConsumerThreadProc (LPVOID lpData)
{

    XPCThreadInfo* threadInfo= (XPCThreadInfo*) lpData;

    char sProducerMessage [512];

    cout << "Thread started to monitor: " << threadInfo->sProducerFilename() << endl;

    try
    {
        XPCSemaphore namedSem (threadInfo->SemName(), OPEN_SEM);
        while (1)
        {
            namedSem.vLockWait();

            ifstream fin (threadInfo->sProducerFilename());
            fin >> sProducerMessage;
            fin.close ();

            ofstream fout (threadInfo->sProducerFilename(), ios::app | ios::trunc);
            fout.close();

            namedSem.vUnlock();

            threadInfo->ThreadSem()->vLockWait();
            globalMsg.Message (sProducerMessage);
            globalMsg.Filename (threadInfo->sProducerFilename());

            threadInfo->ThreadSem()->vUnlock();
            threadInfo->MainSem()->vUnlock();

            Sleep (1);

            if (threadInfo->ExitEvent().bWaitForEvent(0))
            {
                ExitThread (0);
            }
        }
    }
    catch (XPCException& ex)
    {
        cout << ex.sGetException() << endl;
        return -1;
    }
    return 0;
}

DWORD WINAPI WaitForConsumerThreadProc (LPVOID lpData)
{

    XPCThread* threadPtr = (XPCThread*) lpData;

    try
    {
        threadPtr->vWaitForThread();

        DWORD dwRet;

        threadPtr->vGetExitCode (&dwRet);

        cout << "return status from thread: " << dwRet << endl;


        delete threadPtr;

        if ((--iConsumerThreadCount) == 0)
            exit (0);

    }
    catch (XPCException& ex)
    {
        cout << ex.sGetException() << endl;
    }
    return 0;
}

DWORD WINAPI SpawnProcessThread (LPVOID lpData)
{
    char cmdLineBuf[512];

    XPCProducerInfo *producerInfo= (XPCProducerInfo*) lpData;

    sprintf (cmdLineBuf, "%s %s %s", 
        "c:\\projects\\producer\\debug\\producer.exe",
        producerInfo->sGetSemname(), 
        producerInfo->sGetFilename());

    try
    {
        XPCProcessAttrib procAttrib (NULL, cmdLineBuf);

        procAttrib.vSetCreationFlag (CREATE_SUSPENDED | CREATE_NEW_CONSOLE);

        XPCProcess proc (procAttrib);

        proc.vResume();

        proc.vWaitForTerminate();

        cout << "Producer exited, return status: " << proc.dwGetExitCode() << endl;

    }
    catch (XPCException &ex)
    {
        cerr << ex.sGetException() << endl;
        return 0;
    }

}


main (int argc, char* argv[])
{
    XPCThread* consumerThread[100];
    XPCThreadInfo* consumerThreadInfo[100];

    iConsumerThreadCount= 0;

    XPCSemaphore threadSem (NULL, 1);

    XPCSemaphore mainSem (NULL);

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

        XPCProducerInfo *newProducerInfo= new XPCProducerInfo (argv[count],
                                                argv[count+1],
                                                consumerThread[iConsumerThreadCount]);

        XPCThread spawnProcessThread (SpawnProcessThread, 
                    (LPVOID) newProducerInfo);

        XPCThread waitThread (WaitForConsumerThreadProc,
                        consumerThread[iConsumerThreadCount]);

        iConsumerThreadCount++;
    }

    while (1)
    {
        mainSem.vLockWait();

        cout << "Message Received: " << globalMsg.sMessage() << endl;
        cout << "Message Queue: " << globalMsg.sFilename() << endl;


        if (strcmp (globalMsg.sMessage(), "QUIT") == 0)
        {
            int iThreadCount= 0;
            for (int count= 1; count < argc; count += 2)
            {
                if (strcmp (globalMsg.sFilename(), argv[count]) == 0)
                {
                    cout << "Kill thread: " << iThreadCount << endl;

                    consumerThreadInfo[iThreadCount]->ExitEvent().vSendEvent();
                    break;
                }
                iThreadCount++;
            }
        }
    }
    return 0;
}





