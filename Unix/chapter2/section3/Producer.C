#include <iostream.h>
#include <XPCSemaphore.h>
#include <fstream.h>
#include <string.h>
#include <unistd.h>

main(int argc, char *argv[])
{
    char sMessage[512];    // User-inputted message
    char cExistFlag;    // Semaphore existance indicator
    char cFirstLoopFlag;    // First time through producer's loop indicator
    XPCSemaphore *tempProducerSemaphore;    // Temporary semaphore

    if (argc < 3)
    {
        cerr << "USAGE: Producer <semaphore name> <pathname>" << endl;
        return 0;
    }

    cExistFlag = 1;

    try
    {
        // Attempt to open the named semaphore
        tempProducerSemaphore = new XPCSemaphore(argv[1], OPEN_SEM);
    }
    catch(XPCException &exceptOb)
    {
        // Set the cExistFlag to FALSE if the semaphore does not exist
        cExistFlag = 0;
    }

    // If the semaphore already exists attempt to destroy it
    if (cExistFlag == 1)
    {
        cerr << "Named Semaphore Already Exists. Attempting to Remove Semaphore" << endl;
        try
        {
            tempProducerSemaphore->vRemoveSemaphore();
        }
        catch(XPCException &exceptOb)
        {
            cerr << exceptOb.sGetException() << endl;
            return 0;
        }
        delete tempProducerSemaphore;    
    }

    // Set the first loop flag to TRUE the first time through the loop
    cFirstLoopFlag = 1;

    try
    {
        // Create the semaphore and initialize it to 0 or locked
        XPCSemaphore producerSemaphore(argv[1], O_CREAT, 0);
    
        // Loop asking the user for data    
        while(1)
        {
            // Don't lock the first time through the loop

            if (cFirstLoopFlag == 0)
                producerSemaphore.vLockWait();
        
            cout << "Enter a message to send to the server" << endl;
            cin >> sMessage;

            if (strcmp(sMessage, "QUIT") == 0)
            {
                producerSemaphore.vUnlock();
                break;    
            }

            ofstream producerConsumerFile(argv[2], ios::app | ios::ate);
            if (!producerConsumerFile)
            {
                cerr << "Error opening the producer / consumer file" << endl;
                break;
            }

            // Write the user's message to the file    
            producerConsumerFile << sMessage << endl;
            producerConsumerFile.close();

            // Unlock the semaphore
            producerSemaphore.vUnlock();

            if (cFirstLoopFlag)
                cFirstLoopFlag = 0;
    
            sleep(1);    
        }

        // Close and remove the named semaphore
        producerSemaphore.vCloseSemaphore();
        producerSemaphore.vRemoveSemaphore();
    }
    catch(XPCException &myExcept)
    {
        cout << myExcept.sGetException() << endl;
        return 0;
    }
}    
