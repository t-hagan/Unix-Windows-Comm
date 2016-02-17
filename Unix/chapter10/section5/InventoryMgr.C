#include <iostream.h>
#include <string.h>
#include <unistd.h>
#include <XPCTransaction.h>
#include <XPCConfirmation.h>
#include <rpc/rpc.h>

extern "C"
{
#include <SharedMemServer.h>
#include <CrossPlatformSem.h>
}

// Purchase ID definitions
#define WIDGET 0
#define SPROCKET 1

// Purchase price definitions
#define WIDGET_PRICE 10.0
#define SPROCKET_PRICE 20.0

// Server definitions
#define SHARED_MEM_SERVER "aHost"
#define SEMAPHORE_SERVER "aHost"

double dGetPurchasePrice(XPCItem *_item)
{
        switch(_item->cGetID())
        {
                // The purchaes price is calculated for the given item and amount
                case WIDGET:
                        return _item->iGetNum() * WIDGET_PRICE;
                case SPROCKET:
                        return _item->iGetNum() * SPROCKET_PRICE;
                default:
                        return 0.0;
        }
}

main()
{
    XPCTransaction *transactRec;    // Ponter to a XPCTransaction record
    XPCConfirmation confirmRec;    // XPCConfirmation record instance
    createsharedmem transactMem;    // Transaction shared memory creation information        
    createsharedmem confirmMem;    // XPConfirmation shared memory creation information
    writesharedmem writeConfirmMem;    // Data to write to the confirmation shared memory segment
    CLIENT *memcl;            // Connection information for the shared memory server
    CLIENT *semcl;            // Connection information for the semaphore server
    retvalstruct *sem_ret = new retvalstruct;        // Return value from semaphore server
    sharedmemretval *mem_ret = new sharedmemretval; // Return value from shared memory server
    semname transactSem;        // The name of the transaction semaphore
    semname confirmSem;        // The name of the confirmation semaphore
    semstruct newTransactSem;    // Stores creation information for the transaction semaphore
    semstruct newConfirmSem;    // Stores creation information for the confirmation semaphore
    sharedmem *mem;            // Pointer to the raw shared memory bytes
    double dTotalPurchase;        // The total purchase price for the user's order
    int iConfirmNum = 1;        // Stores the confirmation number

    // Connection is made to the shared memory server    
    memcl = clnt_create(SHARED_MEM_SERVER, crossplatformmem, crossplatformmemver, "tcp");
    if (memcl == NULL)
    {
        cerr << clnt_spcreateerror("Error connecting to shared memory server") << endl;
        return 0;
    }

    // Connection is made to the shared memory server
    semcl = clnt_create(SEMAPHORE_SERVER, crossplatformsem, crossplatformsemver, "tcp");
    if (semcl == NULL)
    {
        cerr << clnt_spcreateerror("Error connecting to sem server: ") << endl;
        return 0;
    }

    // The transaction shared memory segment is defined
    transactMem.ikey = 7000;
    transactMem.isize = sizeof(XPCTransaction);

    // The confirmation shared memory segment is defined
    confirmMem.ikey = 7001;
    confirmMem.isize = sizeof(XPCConfirmation);

    // Remove the existing shared memory segment
    removemem_1(&transactMem.ikey, memcl);
    removemem_1(&confirmMem.ikey, memcl);

    // Create the transaction shared memory segment
    mem_ret = createmem_1(&transactMem, memcl);
    if (!mem_ret->iretval)
    {
        cerr << "Error creating transaction shared memory: " << mem_ret->serrormsg << endl;
        return 0;
    }

    // Creat the confirmation shared memory segment
    mem_ret = createmem_1(&confirmMem, memcl);
    if (!mem_ret->iretval)
    {
        cerr << "Error creating confirmation shared memory: " << mem_ret->serrormsg << endl;
        return 0;
    }

    // Store the semaphore names.  Since the Semaphore Server is running on Windows NT
    // There is no need to preceed the name with a '/'
    transactSem = new char[20];
    strcpy(transactSem, "TRANSACTION");    
    
    confirmSem = new char[20];
    strcpy(confirmSem, "CONFIRMATION");

    // Store the semaphore initialization information
    newTransactSem.ssemname = new char[20];
    strcpy(newTransactSem.ssemname, transactSem);
    newTransactSem.iinitvalue = 0;
    newTransactSem.imaxvalue = 1;

    newConfirmSem.ssemname = new char[20];
    strcpy(newConfirmSem.ssemname, confirmSem);
    newConfirmSem.iinitvalue = 0;
    newConfirmSem.imaxvalue = 1;

    // Remove previous instances of the semaphore    
    sem_ret = removesem_1(&transactSem, semcl);
    sem_ret = removesem_1(&confirmSem, semcl);

    // Create the transaction semaphore
    sem_ret = createsem_1(&newTransactSem, semcl);
    if (!sem_ret->iretval)
    {
        cerr << "Error creating transaction semaphore: " << sem_ret->serrormsg << endl;
        return 0;
    }

    // Create the confirmation semaphore
    sem_ret = createsem_1(&newConfirmSem, semcl);
    if (!sem_ret->iretval)
    {
        cerr << "Error creating confirmation semaphore: " << sem_ret->serrormsg << endl;
        return 0;
    }

    // Set the writeConfirmMem values
    writeConfirmMem.ikey = 7001;
    writeConfirmMem.data.sharedmem_len = sizeof(XPCConfirmation);
    writeConfirmMem.data.sharedmem_val = new char[sizeof(XPCConfirmation)];

    while(1)
    {
        dTotalPurchase = 0.0;

        // Lock the transaction semaphore
        sem_ret = lockwait_1(&transactSem, semcl);
        if (!sem_ret->iretval)
        {
            cerr << "Error locking semaphore: " << sem_ret->serrormsg << endl;
            return 0;
        }

        // Read the data from the shared memory segment
        mem = read_1(&transactMem, memcl);

        // If no data is available for reading an error has occured
        if (mem->sharedmem_len == 0)
        {
            cerr << "Error reading shared memory " << endl;

            // The confirmation order number is set to 0
            confirmRec.vSetConfirmNum(0);
        }
        else
        {    
            // If data is available the data is cast to a XPCTransaction object
            transactRec = (XPCTransaction *)mem->sharedmem_val;

            // The purchase price for the ordered items is totalled
            for (int count = 0; count < 2; count++)
            {
                dTotalPurchase += dGetPurchasePrice(&transactRec->items[count]);
            }

            // The XPCConfirmation object is set to the purchase price and current
            // confirmation number
            confirmRec.vSetPurchasePrice(dTotalPurchase);
            confirmRec.vSetConfirmNum(iConfirmNum);

            // The confirmation number is incremented by 1
            iConfirmNum++;
        }

        // The confirmation object is copied into the confirmation shared memory holding buffer    
        memcpy((void *)writeConfirmMem.data.sharedmem_val, (void *)&confirmRec, sizeof(XPCConfirmation));

        // The confirmation shared memory segment is written to
        mem_ret = write_1(&writeConfirmMem, memcl);
        if (!mem_ret->iretval)
        {
            cerr << "Error writing to shared memory: " << mem_ret->serrormsg << endl;
        }

        // The confirmation semaphore lock is released    
        sem_ret = unlock_1(&confirmSem, semcl);  
        if (!sem_ret->iretval)     
        {
            cerr << "Error unlocking semaphore: " << sem_ret->serrormsg << endl; 
            return 0;      
        }
    }
}        
