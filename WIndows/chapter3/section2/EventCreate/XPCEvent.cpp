
#include "XPCEvent.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
XPCEvent::XPCEvent (char * cEventName, PAPCFUNC _pfnAPC)
{
    pSA= (LPSECURITY_ATTRIBUTES) HeapAlloc (GetProcessHeap(), 0, 
                                    sizeof(SECURITY_ATTRIBUTES));

    pSA->nLength= sizeof(SECURITY_ATTRIBUTES);
    pSA->lpSecurityDescriptor= NULL;
    pSA->bInheritHandle= TRUE;

    if ((hEvent= CreateEvent (pSA, TRUE, FALSE, cEventName)) == NULL)
    {
        XPCException ex (ErrorString("CreateEvent failed"));
        throw ex;
        return;
    }

    if ((pfnAPC= _pfnAPC) != NULL)
    {

        if ((hThread= CreateThread (pSA, 0, ThreadProc, this, 0, &dwThreadID)) == NULL)
        {
            XPCException ex (ErrorString("CreateThread failed"));
            throw ex;
            return;
        }

    }
    else
        hThread= NULL;
}


XPCEvent::~XPCEvent()
{
    HeapFree (GetProcessHeap(), 0, pSA); 
    if (hThread)
        CloseHandle (hThread);
    CloseHandle (hEvent);
}


BOOL XPCEvent::bWaitForEvent(DWORD dwMilliseconds)
{
    DWORD dwRet;
    if ((dwRet= WaitForSingleObjectEx (hEvent, dwMilliseconds, TRUE)) == 0xFFFFFFFF)
    {
        XPCException ex (ErrorString("WaitForSingleObjectEx failed"));
        throw ex;
        return FALSE;
    }
    else if (dwRet == WAIT_TIMEOUT)
        return FALSE;
    else
        return TRUE;
}


void XPCEvent::vSendEvent()
{
    if (!SetEvent (hEvent))
    {
        XPCException ex (ErrorString("SetEvent failed"));
        throw ex;
        return;
    }
}

void XPCEvent::vResetEvent()
{
    ::ResetEvent (hEvent);
}


PAPCFUNC XPCEvent::GetAPCFunc()
{
    return (pfnAPC);
}


HANDLE XPCEvent::hGetEventHandle()
{
    return (hEvent);
}


char * XPCEvent::ErrorString(char * sLeader)
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


DWORD WINAPI XPCEvent::ThreadProc(LPVOID lpData)
{
    XPCEvent *pEvent= (XPCEvent*)lpData;

    while (1)
    {

        if (WaitForSingleObjectEx (pEvent->hGetEventHandle(), INFINITE, TRUE) == 0xFFFFFFFF)
        {
            XPCException ex (XPCEvent::ErrorString("WaitForSingleObjectEx failed"));
            throw ex;
            return 0;
        }
        
        if (!QueueUserAPC (pEvent->GetAPCFunc(), GetCurrentThread(), (DWORD) lpData))
        {
            XPCException ex (XPCEvent::ErrorString("QueueUserAPC failed"));
            throw ex;
            return 0;
        }

        SleepEx (100, TRUE);
    }
    return 0;
}




