#if !defined(XPCEVENT_H)
#define XPCEVENT_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#define _WIN32_WINNT 0x0400
#include <windows.h>
#include "XPCException.h"

class XPCEvent  
{
public:
    void vResetEvent();
    HANDLE hGetEventHandle();
    PAPCFUNC GetAPCFunc();
    void vSendEvent();
    BOOL bWaitForEvent(DWORD dwMilliseconds=INFINITE);
    XPCEvent (char *cEventName=NULL, PAPCFUNC _pfnAPC=NULL);
    virtual ~XPCEvent();
    static char * ErrorString(char * sLeader);

private:
static DWORD WINAPI ThreadProc(LPVOID lpData);
    PAPCFUNC pfnAPC;
    DWORD dwThreadID;
    HANDLE hThread;
    LPSECURITY_ATTRIBUTES pSA;
    HANDLE hEvent;
};

#endif // !defined(XPCEVENT_H)
