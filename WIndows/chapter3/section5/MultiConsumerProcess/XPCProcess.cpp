
#include "XPCProcess.h"
#include "XPCProcessAttrib.h"
#include <XPCException.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

XPCProcess::XPCProcess(XPCProcessAttrib& attrib):
                    procAttrib (attrib)
{

if (!CreateProcess (attrib.pApplicationName, 
                attrib.pCommandLine, 
                attrib.pProcessSA, 
                attrib.pThreadSA, 
                attrib.bInheritHandles, 
                attrib.dwCreationFlags, 
                attrib.pEnvironment, 
                attrib.pCurrentDirectory, 
                attrib.pSI, 
                attrib.pProcessInfo))
    {
        XPCException ex (ErrorString("CreateProcess failed"));
        throw ex;
        return;
    }
}

XPCProcess::~XPCProcess()
{

}

char * XPCProcess::ErrorString(char * sLeader)
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

DWORD XPCProcess::dwGetExitCode()
{
    DWORD dwRet=0;

    if (!GetExitCodeProcess (procAttrib.pGetProcessInformation()->hProcess, &dwRet))
    {
        XPCException ex (ErrorString("GetExitCodeProcess failed"));
        throw ex;
    }
    return (dwRet);
}


void XPCProcess::vTerminate()
{
    if (!TerminateProcess (procAttrib.pGetProcessInformation()->hProcess, 1))
    {
        XPCException ex (ErrorString("TerminateProcess failed"));
        throw ex;
        return;
    }
}

void XPCProcess::vWaitForTerminate(DWORD dwWaitTime)
{
    if (WaitForSingleObject (procAttrib.pGetProcessInformation()->hProcess,
            dwWaitTime) == WAIT_FAILED)
    {
        XPCException ex (ErrorString("WaitForSingleObject failed"));
        throw ex;
        return;
    }
}

void XPCProcess::vResume()
{
    if (ResumeThread (procAttrib.pGetProcessInformation()->hThread) == 0xFFFFFFFF)
    {
        XPCException ex (ErrorString("ResumeThread failed"));
        throw ex;
        return;
    }
}
