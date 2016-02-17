#include <XPCSharedMem.h>
#include <errno.h>
#include <stdio.h>
#include <iostream.h>

XPCSharedMem::XPCSharedMem(int _iKey, int _iMethod, int _iSize, int _iPerms)
{
    iShmId = 0;    // Initialize the shared memory ID to 0

    if (_iMethod == CREATE)
    {
        // Create a new shared memory segment
        if ((iShmId = shmget(_iKey, _iSize, _iPerms | IPC_CREAT | IPC_EXCL)) == -1)
        {
            char sMsg[512];
            
            sprintf(sMsg, "Error creating Shared Memory: %s", strerror(errno));
            XPCException myExcept(sMsg);
            throw myExcept;
            return;
        }
    }
    else
    {
        // Open an existing shared memory segment
        if ((iShmId = shmget(_iKey, _iSize, IPC_CREAT)) == -1)
        {
            char sMsg[512];
                                
            sprintf(sMsg, "Error opening Shared Memory: %s", strerror(errno));
            XPCException myExcept(sMsg);
            throw myExcept;
            return;
        }
    }
}

XPCSharedMem::~XPCSharedMem()
{
    return;
}

void *XPCSharedMem::vAttach(int _Access)
{
    void *mem;    // Holds the address of the attached shared memory segment

    // Attaches to the shared memory segment
    if ((mem = shmat(iShmId, (void *) 0, _Access)) == (void *)-1)
    {
        char sMsg[512];
                               
        sprintf(sMsg, "Error attaching Shared Memory: %s", strerror(errno));
        XPCException myExcept(sMsg);
        throw myExcept;
        return (void *)NULL;
    }

    return mem;    // The address of the shared memory segment is returned
}

void XPCSharedMem::vDetach(void *_vMem)
{
    // Detaches from the shared memory segment
    if (shmdt((char *) _vMem) == -1)
    {
        char sMsg[512];

        sprintf(sMsg, "Error Detaching From Shared Memory: %s", strerror(errno));
        XPCException myExcept(sMsg);
        throw myExcept;
        return;
    }
}

void XPCSharedMem::vRemove()
{
    // Removes the shared memory segment
    if ((shmctl(iShmId, IPC_RMID, NULL)) == -1)
    {
        char sMsg[512];

        sprintf(sMsg, "Error removing Shared Memory: %s", strerror(errno));
        XPCException myExcept(sMsg);
        throw myExcept;
        return;
    }
}
