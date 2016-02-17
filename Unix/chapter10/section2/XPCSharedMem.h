#ifndef _XPCSharedMem
#define _XPCSharedMem

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <XPCException.h>

// Macros defining shared memory segment creation or access
#define CREATE 0
#define OPEN 1

class XPCSharedMem
{
    int iShmId;    // ID of the shared memory segment
public:
    // Constructor.  Opens the shared memory segment
    XPCSharedMem(int _iKey, int _iMethod, int _iSize = 0, int _iPerms = 0666);

    // Destructor
    virtual ~XPCSharedMem();

    // Attaches tot he shared memory segment specified by iShmId
    void *vAttach(int _Access = 0);

    // Removes the shared memory segment
    void vRemove();

    // Detaches from the shared memory segment
    void vDetach(void *_vMem);
};

#endif
