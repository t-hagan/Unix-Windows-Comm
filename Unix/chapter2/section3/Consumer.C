#include <iostream.h>
#include <XPCSemaphore.h>
#include <XPCFileStat.h>
#include <fstream.h>
#include <string.h>

main(int argc, char *argv[])
{
    // User-inputted message
    char sMessage[512];

    // Object used to check accessibility to shared file
    XPCFileStat *checkProducerConsumerFile;

    if (argc < 3)
    {
        cerr << "USAGE: Consumer <semaphore name> <pathname>" << endl;
        return 0;
    }
    
    try
    {
        // Open the named semaphore
        XPCSemaphore consumerSemaphore(argv[1], OPEN_SEM);
        
        while(1)    // Loop forever
        {
            // Wait for and lock the semaphore
            consumerSemaphore.vLockWait();

            // Check the accessibility to the given pathname
            try
            {
                checkProducerConsumerFile = new XPCFileStat(argv[2]);
            }
            catch(XPCException &exceptOb)
            {
                cerr << exceptOb.sGetException() << endl;
                return 0;
            }

            // If the consumer is not able to access the file the application
            // exits
            if ((!checkProducerConsumerFile->cCheckReadAccess()) ||
                (!checkProducerConsumerFile->cCheckWriteAccess()))
            {
                cerr << "Access to the producer / consumer file is not allowed" << endl;
                cerr << "Exiting the consumer" << endl;
                consumerSemaphore.vUnlock();
                return 0;
            }

            // If the size of the file is 0, the semaphore is unlocked and the loop
            // is restarted
            if (checkProducerConsumerFile->iGetSize() == 0)
            {
                cerr << "The file does not contain data.  Restarting loop" << endl;
                delete checkProducerConsumerFile;
                consumerSemaphore.vUnlock();
                sleep(1);
                continue;
            }

            delete checkProducerConsumerFile;

            // Open the file and read its contents
            ifstream producerConsumerFile(argv[2]);
            producerConsumerFile.getline(sMessage, 512);
            producerConsumerFile.close();

            // Remove the message from the file
            ofstream resetProducerConsumerFile(argv[2]);
            resetProducerConsumerFile.close();

            // Unlock the semaphore
            consumerSemaphore.vUnlock();
    
            // Display the message
            cout << "RECEIVED: " << sMessage << endl;
            
            sleep(1);    
        }

    }
    catch(XPCException &myExcept)
    {
        cout << myExcept.sGetException() << endl;
        return 0;
    }
}    
