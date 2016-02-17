// Semaphore.cpp: implementation of the CSemaphore class.
//
//////////////////////////////////////////////////////////////////////

#include "XPCSemaphore.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

XPCSemaphore::XPCSemaphore(const char* _sSemName,
                       long lInitialValue,
                       long lMaxValue,
                       int nAccessMode,
                       BOOL _bCanInherit)
{
    if (nAccessMode == CREATE_OR_OPEN_SEM || nAccessMode == CREATE_SEM)
    {
        pSA= (LPSECURITY_ATTRIBUTES) HeapAlloc (GetProcessHeap(), 0, 
                                        sizeof(SECURITY_ATTRIBUTES));

        pSA->nLength= sizeof(SECURITY_ATTRIBUTES);
        pSA->lpSecurityDescriptor= NULL; //no restrictions
        pSA->bInheritHandle= _bCanInherit;
        if ((hSemaphore= CreateSemaphore (pSA, 
                    lInitialValue, 
                    lMaxValue, 
                    _sSemName)) == NULL)
        {
            XPCException ex (ErrorString("CreateSemaphore failed"));
            throw ex;
            return;
        }
    }
    else
    {
        if ((hSemaphore= OpenSemaphore (SEMAPHORE_ALL_ACCESS,
                                    _bCanInherit,
                                    _sSemName)) == NULL)
        {
            XPCException ex (ErrorString("OpenSemaphore failed"));
            throw ex;
            return;
        }
    }

    if (_sSemName)
    {
        sSemName = new char[strlen (_sSemName) + 1];
        if (!sSemName)
        {
            XPCException ex ("Cannot allocate memory for semaphore name.");
            throw ex;
        }
        strcpy (sSemName, _sSemName);
    }
    else
        sSemName= NULL;

    bCanInherit= _bCanInherit;
}

XPCSemaphore::~XPCSemaphore()
{
    if (sSemName)
        delete sSemName;
}

char * XPCSemaphore::ErrorString(char * sLeader)
{
    static char buf[255];
    LPVOID lpMsgBuf;
    UINT ErrNo;

    FormatMessage (
            FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
            NULL,
            ErrNo=GetLastError (),
            MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
            (LPTSTR) &lpMsgBuf,
            0,
            NULL);

    wsprintf (buf, "%s: %s", sLeader, (LPTSTR)lpMsgBuf);

    LocalFree (lpMsgBuf);
    return buf;
}

void XPCSemaphore::vCloseSemaphore()
{
    if (!CloseHandle (hSemaphore))
    {
        XPCException ex (ErrorString("CloseHandle failed"));
        throw ex;
    }
}

void XPCSemaphore::vLockWait()
{
    if (WaitForSingleObject (hSemaphore, INFINITE) == WAIT_FAILED)
    {
        XPCException ex (ErrorString("WaitForSingleObject failed"));
        throw ex;
    }
}

BOOL XPCSemaphore::vLockNoWait()
{
    DWORD dwRet = WaitForSingleObject (hSemaphore, 0);

    if (dwRet == WAIT_TIMEOUT)
        return FALSE;

    if (dwRet == WAIT_OBJECT_0)
        return TRUE;

    if (dwRet == WAIT_FAILED)
    {
        XPCException ex (ErrorString("WaitForSingleObject failed"));
        throw ex;
    }
    return FALSE;
}


void XPCSemaphore::vUnlock()
{
    if (!ReleaseSemaphore (hSemaphore, 1, NULL))
    {
        XPCException ex (ErrorString("ReleaseSemaphore failed"));
        throw ex;
    }
}

long XPCSemaphore::lGetValue()
{
    long lRet=0;

    if (!ReleaseSemaphore (hSemaphore, 0, &lRet))
    {
        XPCException ex (ErrorString("ReleaseSemaphore failed"));
        throw ex;
    }
    return lRet;
}
