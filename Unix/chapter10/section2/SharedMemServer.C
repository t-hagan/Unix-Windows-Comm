#include <iostream.h>
#include <fstream.h>
#include <XPCSemaphore.h>
#include <XPCSharedMem.h>
#include <XPCSignal.h>
#include <XPCException.h>
#include <Personnel.h>
#include <stdlib.h>

XPCSemaphore *mysem;        // Pointer to the named semaphore
XPCSharedMem *mymem;        // Pointer to shared memory

// Control-C handler. Remove the shared memory segment and semaphore
void vExitHandler(int _iSig)
{
    mysem->vRemoveSemaphore();
    mymem->vRemove();
    exit(0);
}

main()
{
    SPersonnelList *MessageListPtr;    // List of personnel records
    void *vMem;            // Pointer to the unformatted contents of the shared memory segment
    
    try
    {
        // Setup a Ctrl-C handler
        XPCSignal SigExitHandler(SIGINT, vExitHandler);

        // Create a named semaphore. Initialize it in the "locked" state
        mysem = new XPCSemaphore("/personnelsem", CREATE_SEM, 0);

        // Create a shared memory segment with an ID of 8888 and a size of SPersonnelList
        mymem = new XPCSharedMem(8888, CREATE, sizeof(SPersonnelList));

        // Attach the shared memory
        vMem = mymem->vAttach();

        // The shared memory is cast to its original structure
        MessageListPtr = new SPersonnelList;
        MessageListPtr = (SPersonnelList *)vMem;

        while(1)
        {
            // Access to the shared memory segment is requested.  If
            // no clients have unlocked the semaphore, the server
            // remains in a suspended state.
            mysem->vLockWait();

            // Open the personnel database
            ofstream personnelDb("personnel.db", ios::app | ios::ate);
            if (!personnelDb)
            {
                cerr << "Error opening personnel.db" << endl;
                return 0;
            }

            // Store each personnel record within the personnel
            // database.
            for (int count = 0; count < MessageListPtr->iMessageNumber; count++)
            {
                personnelDb << MessageListPtr->Message[count].sName << "," << flush;
                personnelDb << MessageListPtr->Message[count].sSSNO << "," << flush;
                personnelDb << MessageListPtr->Message[count].iAge << "," << endl;
            }
            personnelDb.close(); // Closes the personnel db

            // The shared memory segment is cleared once processed.
            memset((void *)MessageListPtr, 0, sizeof(SPersonnelList));

            // Unlock the named semaphore so that the client can
            // write additional information within the shared memory
            // segment.
            mysem->vUnlock();

            sleep(1);       // A sleep of 1 second is done so the client
                            // has a chance to lock the semaphore before
                            // the server can do so again.
        }
    }

    // All semaphore or shared memory related exceptions are caught and
    // displayed.
    catch(XPCException &exceptObject)
    {
        cerr << "Server Error: " << exceptObject.sGetException() << endl;

        // Remove the semaphore and shared memory segment
        mysem->vRemoveSemaphore();
        mymem->vRemove();
        return 0;
    }
    return 1;
}
