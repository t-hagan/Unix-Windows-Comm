#ifdef UNIX
    #include <unistd.h>
#endif

#include <XPCSemaphore.h>

#include <iostream.h>

extern "C"
{
#include "CrossPlatformSem.h"
}

retvalstruct *createsem_1(semstruct *_newSemInfo)
{
    static retvalstruct ret;    // Return value
    
    ret.iretval = 1;        // Initialize the return to SUCCESS

    try
    {
        // Create the named semaphore
        XPCSemaphore newSem(_newSemInfo->ssemname, CREATE_SEM, _newSemInfo->iinitvalue, _newSemInfo->imaxvalue);
    }
    catch(XPCException &exceptObject)
    {
        // Store the exception in the return value
        if (ret.serrormsg)
            delete [] ret.serrormsg;
        ret.serrormsg = new char[strlen(exceptObject.sGetException())];
        strcpy(ret.serrormsg, exceptObject.sGetException());
        ret.iretval = 0; // Set teh return code to FAILURE
        return &ret;
    }

    // Clear any memory used for serrormsg and minimally reallocate for XDR encoding and decoding reasons
    if (ret.serrormsg)
        delete [] ret.serrormsg;
    ret.serrormsg = new char[2];

    return &ret;
}

retvalstruct *closesem_1(semname *_name)
{
    static retvalstruct ret;        // Return value

        ret.iretval = 1;            // Initialize the return to SUCCESS

    try
    {
        // Open the named semaphore
        XPCSemaphore newSem(*_name, OPEN_SEM);
    
        // Close the semaphore
        newSem.vCloseSemaphore();
    }
    catch(XPCException &exceptObject)
    {
        // Store the exception in the return value
        if (ret.serrormsg)
            delete [] ret.serrormsg;

        ret.serrormsg = new char[strlen(exceptObject.sGetException())+1];
        strcpy(ret.serrormsg, exceptObject.sGetException());
        ret.iretval = 0;
    }
    
    // Clear any memory used for serrormsg and minimally reallocate for XDR encoding and decoding reasons
    if (ret.serrormsg)
        delete [] ret.serrormsg;

    ret.serrormsg = new char[2];
    
    return &ret;
}

retvalstruct *removesem_1(semname *_name)
{
    static retvalstruct ret;      // Return value

        ret.iretval = 1;          // Initialize the return to SUCCESS

    try
    {
        // Open the named semaphore    
        XPCSemaphore newSem(*_name, OPEN_SEM);

        // Remove the named semaphore
#ifdef UNIX
        newSem.vRemoveSemaphore();
#else
        newSem.vCloseSemaphore();
#endif
    }
    catch(XPCException &exceptObject)
    {
       // Store the exception in the return value
        if (ret.serrormsg)
            delete [] ret.serrormsg;

        ret.serrormsg = new char[strlen(exceptObject.sGetException())+1];
        strcpy(ret.serrormsg, exceptObject.sGetException());
        ret.iretval = 0;    // Return code is set to FAILURE
        return &ret;
    }
    
    // Clear any memory used for serrormsg and minimally reallocate for XDR encoding and decoding reasons
    if (ret.serrormsg)
           delete [] ret.serrormsg;

    ret.serrormsg = new char[2];
    
    return &ret;
}

retvalstruct *lockwait_1(semname *_name)
{
    static retvalstruct ret;    // Return value

    ret.iretval = 1;        // Initialize the return value to SUCCESS

    try
    {
    // Open the named semaphore
    XPCSemaphore newSem(*_name, OPEN_SEM);

    // Lock the named semaphore
            newSem.vLockWait();
    }
    catch(XPCException &exceptObject)
    {
        // Store the exception in the return value
        if (ret.serrormsg)
            delete [] ret.serrormsg;

        ret.serrormsg = new char[strlen(exceptObject.sGetException())+1];
        strcpy(ret.serrormsg, exceptObject.sGetException());
        ret.iretval = 0;
        return &ret;
    }
           
    // Clear any memory used for serrormsg and minimally reallocate for XDR encoding and decoding reasons
    if (ret.serrormsg)
        delete [] ret.serrormsg;

    ret.serrormsg = new char[2];

    return &ret;
}

retvalstruct *locknowait_1(semname *_name)
{
    static retvalstruct ret;        // Return value

    ret.iretval = 1;                // Initialize the return to SUCCESS

    try
    {
    // Open the named semaphore
    XPCSemaphore newSem(*_name, OPEN_SEM);
    
    // Attempt to lock the semaphore
            newSem.vLockNoWait();
    }
    catch(XPCException &exceptObject)
    {
        // Store the exception in the return value
        if (ret.serrormsg)
            delete [] ret.serrormsg;

        ret.serrormsg = new char[strlen(exceptObject.sGetException())+1];
        strcpy(ret.serrormsg, exceptObject.sGetException());
        ret.iretval = 0;
        return &ret;
    }
    
    // Clear any memory used for serrormsg and minimally reallocate for XDR encoding and decoding reasons
    if (ret.serrormsg)
        delete [] ret.serrormsg;

    ret.serrormsg = new char[2];
    
    return &ret;
}

retvalstruct *unlock_1(semname *_name)
{
    static retvalstruct ret;        // Return value

    ret.iretval = 1;                // Initialize the return to SUCCESS

    try
    {
        // Open the named semaphore
        XPCSemaphore newSem(*_name, OPEN_SEM);

        // Unlock the semaphore
        newSem.vUnlock();
    }
    catch(XPCException &exceptObject)
    {
        // Store the exception in the return value
        if (ret.serrormsg)
                delete [] ret.serrormsg;
        ret.serrormsg = new char[strlen(exceptObject.sGetException())+1];
        strcpy(ret.serrormsg, exceptObject.sGetException());
        ret.iretval = 0;
        return &ret;
    }
    
// Clear any memory used for serrormsg and minimally reallocate for XDR encoding and decoding reasons
    if (ret.serrormsg)
            delete [] ret.serrormsg;
    ret.serrormsg = new char[2];
    
    return &ret;
}

retvalstruct *getvalue_1(semname *_name)
{
    static retvalstruct ret;     // Return value

    ret.iretval = -1;            // Initialize the return to FAILURE

    try
    {
        // Open the named semaphore
        XPCSemaphore newSem(*_name, OPEN_SEM);
        // Get the value of the named semaphore
        ret.iretval = newSem.iGetValue();
    }
    catch(XPCException &exceptObject)
    {
        // Store the exception in the return value
        if (ret.serrormsg)
            delete [] ret.serrormsg;

        ret.serrormsg = new char[strlen(exceptObject.sGetException())+1];
        strcpy(ret.serrormsg, exceptObject.sGetException());
        return &ret;
    }

    // Clear any memory used for serrormsg and minimally reallocate for XDR encoding and decoding reasons
    if (ret.serrormsg)
            delete [] ret.serrormsg;
    ret.serrormsg = new char[2];

    return &ret;
}    
