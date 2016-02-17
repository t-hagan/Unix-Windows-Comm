#include <iostream.h>
#include <string.h>
#include <fstream.h>
#include <iostream.h>
#include <XPCSemaphore.h>

int main (int argc, char* argv[])
{
    // buffer to hold the message from the user
    char sMessage[512];

    BOOL bFirstLoop = TRUE;

    if (argc < 3)
    {
        cerr << "USAGE: producer <semaphore name> <pathname>" << endl;
        return 1;
    }

    //create the named semaphore
    try
    {
        XPCSemaphore producerSemaphore(argv[1]);

        // loop until the user enters "QUIT"
        while (1)
        {
            // only lock after the first time through
            if (!bFirstLoop)
                producerSemaphore.vLockWait();

            // prompt for a message from the user
            cout << "Enter a message to send to the consumer: " <<
                flush;

            cin >> sMessage;

            // open the file given in the command line and
            // append the message to it

            ofstream producerConsumerFile (argv[2], ios::app | ios::trunc);
            if (!producerConsumerFile)
            {
                cerr << "Error opening producer/consumer file" << endl;
                break;
            }

            // write the message and close the file
            producerConsumerFile << sMessage << endl;
            producerConsumerFile.close();

            // unlock the semaphore to a allow the consumer
            // to read the file
            producerSemaphore.vUnlock();

            // set flag that we've gone through once
            bFirstLoop= FALSE;
            // sleep to give the consumer a chance to get 
            // the semaphore
            Sleep(1);

            // exit the loop if quit was entered
            if (strcmp (sMessage, "QUIT") == 0)
            {
                producerSemaphore.vUnlock();
                break;
            }
        }    //while (1)

        // close the semaphore because we're done
        producerSemaphore.vCloseSemaphore();
    }
    catch (XPCException &ex)
    {
        cerr << ex.sGetException() << endl;
        return 2;
    }
    return 0;
}
