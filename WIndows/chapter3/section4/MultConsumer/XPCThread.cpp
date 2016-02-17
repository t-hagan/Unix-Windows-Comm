
#include "XPCThread.h"
#include <XPCException.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

XPCThread::XPCThread(LPTHREAD_START_ROUTINE pFunc,
            LPVOID lpParameter,    
            DWORD dwFlags,            
            DWORD dwStackSize,        
            BOOL bInheritable)
{
    pSA= (LPSECURITY_ATTRIBUTES) HeapAlloc (GetProcessHeap(), 0, 
                                    sizeof(SECURITY_ATTRIBUTES));

    pSA->nLength= sizeof(SECURITY_ATTRIBUTES);
    pSA->lpSecurityDescriptor= NULL; // default to all access
    pSA->bInheritHandle= bInheritable;

    if ((hThread= CreateThread (pSA, 
                            dwStackSize, 
                            pFunc, 
                            lpParameter,
                            dwFlags,
                            &dwThreadID)) == NULL)
    {
        XPCException ex (ErrorString("CreateThread failed"));
        throw ex;
        return;
    }
    dwExitCode= 0xFFFFFFFF;
}

XPCThread::~XPCThread()
{
    HeapFree (GetProcessHeap(), 0, pSA); 
}

void XPCThread::vWaitForThread()
{
    if (WaitForSingleObject (hThread, INFINITE) == WAIT_FAILED)
    {
        XPCException ex (ErrorString("WaitForSingleObjectEx failed"));
        throw ex;
        return;
    }
}

void XPCThread::vGetExitCode(LPDWORD pdwExitCode)
{
    if (GetExitCodeThread (hThread, &dwExitCode))
    {
        *pdwExitCode= dwExitCode;
    }
    else
    {
        XPCException ex (ErrorString("GetExitCodeThread failed"));
        throw ex;
    }
}

DWORD XPCThread::dwGetExitCode()
{
    vGetExitCode (&dwExitCode);
    return dwExitCode;
}


void XPCThread::vResume()
{
    if (ResumeThread (hThread) == 0xFFFFFFFF)
    {
        XPCException ex (ErrorString("ResumeThread failed"));
        throw ex;
        return;
    }
}

void XPCThread::vSuspend()
{
    if (SuspendThread (hThread) == 0xFFFFFFFF)
    {
        XPCException ex (ErrorString("SuspendThread failed"));
        throw ex;
        return;
    }

}

int XPCThread::nGetThreadPriority()
{
    int nRet;
    if ((nRet= GetThreadPriority (hThread)) == THREAD_PRIORITY_ERROR_RETURN)
    {
        XPCException ex (XPCThread::ErrorString("GetThreadPriority failed"));
        throw ex;
        return 0;
    }
    return nRet;
}

void XPCThread::vSetThreadPriority(int nPriority)
{
    if (!SetThreadPriority (hThread, nPriority))
    {
        XPCException ex (XPCThread::ErrorString("SetThreadPriority failed"));
        throw ex;
    }
}

void XPCThread::vDisablePriorityBoost (BOOL bDisable)
{
    if (!SetThreadPriorityBoost (hThread, bDisable))
    if (0)
    {
        XPCException ex (XPCThread::ErrorString("SetThreadPriorityBoost failed"));
        throw ex;
    }
}


BOOL XPCThread::bIsPriorityBoostDisabled()
{
    BOOL bBoosted;

    if (!GetThreadPriorityBoost (hThread, &bBoosted))
    if (0)
    {
        XPCException ex (XPCThread::ErrorString("GetThreadPriorityBoost failed"));
        throw ex;
    }
    return bBoosted;
}

void XPCThread::vSetProcessorMask(DWORD dwMask)
{
    if (!SetThreadAffinityMask (hThread, dwMask))
    if (0)
    {
        XPCException ex (XPCThread::ErrorString("SetThreadAffinityMask failed"));
        throw ex;
    }
}


void XPCThread::vSetIdealProcessor(DWORD dwProc)
{
    if (SetThreadIdealProcessor (hThread, dwProc) == -1)
    if (0)
    {
        XPCException ex (XPCThread::ErrorString("SetThreadIdealProcessor failed"));
        throw ex;
    }
}


void XPCThread::vGetThreadTimes (LPFILETIME lpCreationTime,
                    LPFILETIME lpExitTime,
                    LPFILETIME lpKernelTime,
                    LPFILETIME lpUserTime)
{
    if (!GetThreadTimes (hThread,
                    lpCreationTime,
                    lpExitTime,
                    lpKernelTime,
                    lpUserTime))
    {
        XPCException ex (XPCThread::ErrorString("GetThreadTimes failed"));
        throw ex;
    }
}

char * XPCThread::ErrorString(char * sLeader)
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





