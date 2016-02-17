#include <iostream.h>
#include <XPCSemaphore.h>        // Defines XPCSemaphore
#include <XPCSharedMem.h>        // Defines XPCSharedMem
#include <XPCException.h>    // The Inter-platform exception class
#include <Personnel.h>
#include <unistd.h>

main()
{
        char sCommand[50];        // Stores a name
        SPersonnelList *MessageListPtr;    // Pointer to the personnel list
        void *vMem;            // Pointer to the shared memory segment
        XPCSemaphore *mysem;        // Named semaphore
        XPCSharedMem *mymem;        // Shared memory
 
        try
        {
            // Opens the named semaphore.
            mysem = new XPCSemaphore("/personnelsem", OPEN);

            // Opens the shared memory segment.
            mymem = new XPCSharedMem(8888, OPEN);

            // The shared memory segment is attached to and is cast to 
            // SPersonnelList
            vMem = mymem->vAttach();

            MessageListPtr = new SPersonnelList;
            MessageListPtr = (SPersonnelList *)vMem;

	    // Lock the semaphore
            mysem->vLockWait();
 
            while(1)
            {      
                // Clear the contents of the shared memory segment
                memset((void *)MessageListPtr, 0, sizeof(SPersonnelList));

                // Read up to 10 personnel records.  
                for (int iPersonnelCount = 0; iPersonnelCount < 10; iPersonnelCount++)
                {
                    // Read a name
                    cout << "Enter a Name, DONE to send the message, or EXIT to exit the client : " << flush;
                    cin >> sCommand;

                    // If the user enters "EXIT", the semaphore is
                    // unlocked, the shared memory is detached, and the 
                    // client exits.
                    if (strcmp(sCommand, "EXIT") == 0)
                    {
                        mymem->vDetach(vMem);
                        mysem->vUnlock();
                        return 1;
                    }
                    // If the user enters "DONE", the loop is broken.
                    else if (strcmp(sCommand, "DONE") == 0)
                    {
                        break;
                    }
                    // The name is stored in shared memory
                    else                            
                        strcpy(MessageListPtr ->Message[iPersonnelCount].sName, sCommand);

                    // The user's social security number and age are 
                    // read and stored in shared memory
                    cout << "Enter the social security number: " << flush;
                    cin >> MessageListPtr->Message[iPersonnelCount].sSSNO;
                        
                    cout << "Enter the age: " << flush;
                    cin >> MessageListPtr->Message[iPersonnelCount].iAge;
                    MessageListPtr->iMessageNumber = iPersonnelCount+1;
                }

                cout << "Unlocking sem" << endl;               
                // The semaphore is unlocked.
                mysem->vUnlock();
                sleep(1);
            }
        }       
        catch(XPCException &exceptObject)
        {
            // If an exception is caught the error is displayed and the shared 
            // memory segment is detached and the semaphore is unlocked.
            cout << "Client Error: " << exceptObject.sGetException() << endl;
            mymem->vDetach(vMem);
            mysem->vUnlock();
            return 0;
        }                       
}
