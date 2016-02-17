#ifndef _XPCSemaphore
#define _XPCSemaphore

#include <semaphore.h>
#include <XPCException.h>
#include <sys/stat.h>

// Semaphore inheritance macros
#define PUBLIC_SEM 1
#define PRIVATE_SEM 0

// Semaphore creation macros
#define OPEN_SEM 0
#define CREATE_OR_OPEN_SEM O_CREAT
#define CREATE_SEM O_CREAT | O_EXCL

// Semaphore permission macros
#define OWNER_PERMISSION S_IRUSR | S_IWUSR
#define GROUP_PERMISSION S_IRGRP | S_IWGRP
#define OTHER_PERMISSION S_IROTH | S_IWOTH
#define SEMAPHORE_PERMISSION OWNER_PERMISSION | GROUP_PERMISSION | OTHER_PERMISSION

class XPCSemaphore
{
private:
    sem_t *semValue;    // The address of the semaphore
    char *sSemName;        // The name of the named semaphore
    char cCanInherit;    // The inheritance flag
public:
    // Constructor for the unamed semaphore
    XPCSemaphore(int _iShareSem, unsigned int _iInitialValue, int _iMaxValue = 0);

    // Constructor for the named semaphore
    XPCSemaphore(const char *_sSemName, int i_AccessMode = OPEN_SEM, int _iInitialValue = 0, int _iMaxValue = 0);
    
    // Copy constructor
    XPCSemaphore(const XPCSemaphore &ob)
    {
        if (strlen(ob.sGetSemName()) > 0)
        {
            sSemName = new char[strlen(ob.sGetSemName()) + 1];
            strcpy(sSemName, ob.sGetSemName());
        }
        semValue = ob.getSemValue();
        cCanInherit = ob.cInherit();
    }

    // Destructor
    ~XPCSemaphore() { delete [] sSemName; }

    // Closes the semaphore.  Named semaphore only
    void vCloseSemaphore();

    // Removes the semaphore
    void vRemoveSemaphore();

    // Locks the semaphore and blocks processing if semaphore cannot be locked
    void vLockWait();

    // Locks the semaphore and does not block processing if semaphore cannot be locked
    int vLockNoWait();

    // Unlocks the semaphore
    void vUnlock();

    // Retrieves the value of the semphore
    int iGetValue();

    // Methods to retrieve private data member values
    char cInherit() const { return cCanInherit; }
    sem_t *getSemValue() const { return semValue; }
    char *sGetSemName() const { return sSemName; }
};

#endif
