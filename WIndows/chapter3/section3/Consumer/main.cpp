#include <iostream.h>
#include <string.h>
#include <fstream.h>
#include <iostream.h>
#include <XPCFileStat.h>
#include "XPCSemaphore.h"

main (int argc, char* argv[])
{
    // buffer to hold the message from the file
    char sMessage[512];

    // pointer to a XPCFileStat to use to check for the existence,
    // accessibility, and size of the file
    XPCFileStat *checkProducerConsumerFile;

    // check for the right number of args
    if (argc < 3)
    {
        cerr << "USAGE: producer <semaphore name> <pathname>" << endl;
        return 0;
    }

    // open the named semaphore
    try
    {
        XPCSemaphore consumerSemaphore(argv[1], OPEN_SEM);

        // loop forever
        while (1)
        {
            // wait for the lock
            consumerSemaphore.vLockWait();

            // we've got the lock so create an instance 
            // of XPCFileStat to see if the file exists
            try
            {
                checkProducerConsumerFile = new XPCFileStat(argv[2]);
            }
            catch (XPCException &ex)
            {
                cerr << ex.sGetException() << endl;
                return 0;
            }

            if (!checkProducerConsumerFile->iCheckReadAccess() ||
                !checkProducerConsumerFile->iCheckWriteAccess())
            {
                cerr << "Access to file is not allowed" << endl;
                consumerSemaphore.vUnlock();
                return 0;
            }
    
            // check the file size. If 0 then loop again
            if (checkProducerConsumerFile->iGetSize() == 0)
            {
                cerr << "The producer/consumer file contains no data." << endl; 
                cerr << "Restarting loop." << endl;
                delete checkProducerConsumerFile;

                // unlock the semaphore and give the producer 
                // a chance to regain the lock
                consumerSemaphore.vUnlock();
                Sleep(1);
                continue;
            }
            delete checkProducerConsumerFile;
            //open the file and read the message
            ifstream producerConsumerFile (argv[2]);
            producerConsumerFile >> sMessage;
            producerConsumerFile.close();

            // unlock the semaphore, print the message
            consumerSemaphore.vUnlock();
            cout << "Received: " << sMessage << endl;
            if (strcmp (sMessage, "QUIT") == 0)
            {
                consumerSemaphore.vUnlock();
                return 0;
            }

            // allow the producer to grab the lock
            Sleep(1);
        }
    }
    catch (XPCException &ex)
    {
        cout << ex.sGetException() << endl;
        return 0;
    }
}
