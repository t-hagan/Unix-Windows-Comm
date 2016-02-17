
#if !defined _XPCTHREAD_H_
#define _XPCTHREAD_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
#define _WIN32_WINNT 0x0400
#include <windows.h>

class XPCThread  
{
private:
    HANDLE hThread;        // thread's handle
    DWORD dwThreadID;    // ID of thread
    LPSECURITY_ATTRIBUTES pSA;
    DWORD dwExitCode;

    static char * ErrorString(char * sLeader);
public:
    // default constuctor, used if the current thread is unknown...and needed
    XPCThread() {hThread= GetCurrentThread();
                dwThreadID= GetCurrentThreadId();
                dwExitCode= 0xFFFFFFFF;}

    // normal constructor
    XPCThread (LPTHREAD_START_ROUTINE pFunc,    // address of thread proc

            LPVOID lpParameter=NULL,        // address of proc's parameter,
                                            // default to none.
            DWORD dwFlags=0,                // creation flag, default to
                                            // execute on creation.
            DWORD dwStackSize=0,            // stack size, default to
                                            // calling threads stack size.
            BOOL bInheritable=FALSE);        // is the thread inheritable
                                            // to threads create by this one?

    ~XPCThread();

    void vWaitForThread();
    void vGetExitCode (LPDWORD pdwExitCode);
    void vSetThreadPriority(int nPriority);
    int nGetThreadPriority();
    void vDisablePriorityBoost (BOOL bDisable);
    BOOL bIsPriorityBoostDisabled();
    void vSetProcessorMask(DWORD dwMask);
    void vSetIdealProcessor (DWORD dwProc);
    void vSuspend();
    void vResume();
    DWORD dwGetExitCode ();
    void vGetThreadTimes (LPFILETIME lpCreationTime,
                    LPFILETIME lpExitTime,
                    LPFILETIME lpKernelTime,
                    LPFILETIME lpUserTime);
    DWORD dwGetThreadID () {return dwThreadID;}
};

#endif // !defined(XPCTHREAD_H_)
