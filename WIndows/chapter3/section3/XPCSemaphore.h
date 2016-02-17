//////////////////////////////////////////////////////////////////////

#if !defined _XPCSEMAPHORE_H_
#define _XPCSEMAPHORE_H_

#include <windows.h>
#include "XPCException.h"

#define OPEN_SEM 0
#define CREATE_OR_OPEN_SEM 1
#define CREATE_SEM 2

class XPCSemaphore  
{
public:
    XPCSemaphore(const char* _sSemName,
            long lInitialValue=0,
            long lMaxValue=10,
            int nAccessMode=CREATE_SEM,
            BOOL bCanInherit=TRUE);
    virtual ~XPCSemaphore();
    void vCloseSemaphore();
    char * ErrorString (char* sleader);
    void vLockWait();
    BOOL vLockNoWait();
    void vUnlock();
    long lGetValue();
private:
    BOOL bCanInherit;
    char * sSemName;
    HANDLE hSemaphore;
    LPSECURITY_ATTRIBUTES pSA;
};

#endif // !defined(XPCSEMAPHORE_H_)
