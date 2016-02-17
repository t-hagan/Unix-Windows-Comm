#include <XPCSemaphore.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <iostream.h>

XPCSemaphore::XPCSemaphore(int _iShareSem, unsigned int _iInitialValue,  int _iMaxValue)
{
    // Verify that the share flag is valid
    if ((_iShareSem != PUBLIC_SEM) &&
        (_iShareSem != PRIVATE_SEM))
    {
        XPCException semExcept("Invalid Share Flag For XPCSemaphore Construction");
        throw semExcept;
        return;
    }

    cCanInherit = _iShareSem;
    
    // Allocate memory for semValue
    semValue = new sem_t;

    // Allocate memory for the semaphore name so the destructor does not fail
    sSemName = new char[1];

    // Initalize the unnamed semaphore using the initial value specified
    if (sem_init(semValue, _iShareSem, _iInitialValue) == -1)
    {
        char sErrMsg[512];
    
        sprintf(sErrMsg, "Error Constructing XPCSemaphore: %s", strerror(errno));
        XPCException semExcept(sErrMsg);
        throw semExcept;
        return;
    }
}

XPCSemaphore::XPCSemaphore(const char *_sSemName, int _iAccessMode, int _iInitialValue, int _iMaxValue)
{
    // The validity of the semphore name is checked
    if (!_sSemName)
    {
        XPCException semExcept("A name must be supplied for a named semaphore");
        throw semExcept;
        return;
    }
    else if (_sSemName[0] != '/')
    {
        XPCException semExcept("A named semaphore must start with a '/'");
        throw semExcept;
        return;
    }
    else if (strchr((char *)_sSemName+1, '/') != 0)
    {
        XPCException semExcept("Only the first letter of a semaphore name can contain a '/'");
        throw semExcept;
        return;
    }

    // The semaphore name is allocated and stored    
    sSemName = new char[strlen(_sSemName)+1];
    if (!sSemName)
    {
        XPCException semExcept("Cannot allocate memory for semaphore name");
        throw semExcept;
        return;
    }

    semValue = new sem_t;
    strcpy(sSemName, _sSemName);

    if (_iAccessMode == OPEN_SEM)
    {
        // Semaphore is opened for access only
        semValue = sem_open(sSemName, _iAccessMode);        
        if ((int)semValue == -1)
        {
            char sErrMsg[512];

            sprintf(sErrMsg, "Error Constructing XPCSemaphore: %s", strerror(errno));
            XPCException semExcept(sErrMsg);
            throw semExcept;
            return;
        }
    }
    else
    {
        // Semaphore is opened or created using the initial value specified
        semValue = sem_open(sSemName, _iAccessMode, SEMAPHORE_PERMISSION, _iInitialValue);
        if ((int)semValue == -1)
        {
            char sErrMsg[512];

            sprintf(sErrMsg, "Error constructing XPCSemaphore: %s", strerror(errno));
            XPCException semExcept(sErrMsg);
            throw semExcept;
            return;
        }
    }
}

void XPCSemaphore::vCloseSemaphore()
{
    if (strlen(sSemName) > 0)
    {
        // If the semaphore is named it is closed using sem_close
        if (sem_close(semValue) == -1)
        {
            char sErrMsg[512];

            sprintf(sErrMsg, "Error closing XPCSemaphore: %s", strerror(errno));
            XPCException semExcept(sErrMsg);
            throw semExcept;
            return;
        }
    }
    else
    {
        // If the semaphore is unnamed it is closed using sem_destroy
        if (sem_destroy(semValue) == -1)
        {
            char sErrMsg[512];

            sprintf(sErrMsg, "Error closing XPCSemaphore: %s", strerror(errno));
            XPCException semExcept(sErrMsg);
            throw semExcept;
            return;
        }
    }
}

void XPCSemaphore::vRemoveSemaphore()
{
    if (strlen(sSemName) > 0)
    {       
        // If the semaphore is named it is removed using sem_unlink
        if (sem_unlink((const char *)sSemName) == -1)
        {
            char sErrMsg[512];

            sprintf(sErrMsg, "Error removing XPCSemaphore: %s", strerror(errno));
            XPCException semExcept(sErrMsg);
            throw semExcept;
            return;
        }
    }
    else
    {
        // If the semaphore is unnamed it is removed using sem_destroy
        if (sem_destroy(semValue) == -1)
        {
            char sErrMsg[512];

            sprintf(sErrMsg, "Error removing XPCSemaphore: %s", strerror(errno));
            XPCException semExcept(sErrMsg);
            throw semExcept;
            return;
        }
    }
}

void XPCSemaphore::vLockWait()
{
    int retval;

    // If the semaphore lock fails due to an interrupt continue trying to lock it 
    while (((retval = sem_wait(semValue)) == -1) && (errno == EINTR))
    { };
        
    if (retval == -1) 
    {
        char sErrMsg[512];

        sprintf(sErrMsg, "Error locking XPCSemaphore: %s", strerror(errno));
        XPCException semExcept(sErrMsg);
        throw semExcept;
        return;
    }
}

int XPCSemaphore::vLockNoWait()
{
    int retval;

    // If the semaphore lock fails due to an interrupt, return a 0
    if (((retval = sem_trywait(semValue)) == -1) && (errno == EAGAIN))
        return 0;
    else if (retval == -1)
    {
        char sErrMsg[512];

        sprintf(sErrMsg, "Error locking XPCSemaphore: %s", strerror(errno));
        XPCException semExcept(sErrMsg);
        throw semExcept;
        return 0;
    }
    else    
        return 1;
}

void XPCSemaphore::vUnlock()
{
    if (sem_post(semValue) == -1)
    {
        char sErrMsg[512];

        sprintf(sErrMsg, "Error removing XPCSemaphore: %s", strerror(errno));
        XPCException semExcept(sErrMsg);
        throw semExcept;
        return;
    }
}

int XPCSemaphore::iGetValue()
{
    int iCurrentSemValue;

    // Retrieve the current value of the semaphore
    if (sem_getvalue(semValue, &iCurrentSemValue) == -1)
    {
        char sErrMsg[512];

        sprintf(sErrMsg, "Error getting the value of semaphore: %s", strerror(errno));
        XPCException semExcept(sErrMsg);
        throw semExcept;
        return -1; 
    }
    else
        return iCurrentSemValue;
}    
