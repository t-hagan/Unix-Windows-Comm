
#include "XPCProcessAttrib.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

XPCProcessAttrib::XPCProcessAttrib(LPTSTR appName,
                    LPSTR commandLine,
                    BOOL processInheritable,
                    BOOL threadInheritable,
                    BOOL inheritHandles,
                    DWORD creationFlags,
                    LPVOID environment,
                    LPTSTR currentDirectory)
{
    pSI= (LPSTARTUPINFO)HeapAlloc (GetProcessHeap(), 
                        HEAP_ZERO_MEMORY, 
                        sizeof (STARTUPINFO));

    pProcessSA= (LPSECURITY_ATTRIBUTES) HeapAlloc (GetProcessHeap(), 
                            HEAP_ZERO_MEMORY, 
                            sizeof (SECURITY_ATTRIBUTES));

    pThreadSA= (LPSECURITY_ATTRIBUTES) HeapAlloc (GetProcessHeap(), 
                                HEAP_ZERO_MEMORY, 
                                sizeof (SECURITY_ATTRIBUTES));

    pProcessInfo= (LPPROCESS_INFORMATION) HeapAlloc (GetProcessHeap(),
                            HEAP_ZERO_MEMORY,
                            sizeof (PROCESS_INFORMATION));

    if (!pSI || !pProcessSA || !pThreadSA || !pProcessInfo)
    {
        XPCException ex (ErrorString("HeapAlloc failed"));
        throw ex;
        return;
    }

    pCurrentDirectory= pApplicationName= pCommandLine= NULL;
    pEnvironment= NULL;

    GetStartupInfo (pSI);

    pProcessSA->nLength= sizeof(SECURITY_ATTRIBUTES);
    pProcessSA->lpSecurityDescriptor= NULL; //no restrictions
    pProcessSA->bInheritHandle= processInheritable;

    pThreadSA->nLength= sizeof(SECURITY_ATTRIBUTES);
    pThreadSA->lpSecurityDescriptor= NULL; //no restrictions
    pThreadSA->bInheritHandle= threadInheritable;

    bInheritHandles= inheritHandles;
    dwCreationFlags= creationFlags;

    vSetCurrentDirectory (currentDirectory);
    vSetCommandLine (commandLine);
    vSetApplicationName (appName);
    vSetEnvironment (environment);
}

XPCProcessAttrib::~XPCProcessAttrib()
{
    HeapFree (GetProcessHeap(), 0, pSI);
    HeapFree (GetProcessHeap(), 0, pProcessSA);
    HeapFree (GetProcessHeap(), 0, pThreadSA);
    HeapFree (GetProcessHeap(), 0, pProcessInfo);
    HeapFree (GetProcessHeap(), 0, pEnvironment);

    if (pApplicationName)
        HeapFree (GetProcessHeap(), 0, pApplicationName);

    if (pCommandLine)
        HeapFree (GetProcessHeap(), 0, pCommandLine);

    if (pCurrentDirectory)
        HeapFree (GetProcessHeap(), 0, pCurrentDirectory);

    if (pEnvironment)
        HeapFree (GetProcessHeap(), 0, pEnvironment);
}


void XPCProcessAttrib::AddEnvironmentVariable(LPCTSTR param, LPCTSTR val)
{
    DWORD dwI= 0;

    if (dwEnvSize > 0)
        dwI= dwEnvSize-1;

    if (pEnvironment)
    {
        pEnvironment= HeapReAlloc (GetProcessHeap(), 
                            HEAP_ZERO_MEMORY,
                            pEnvironment,
                            dwEnvSize+= strlen (param) + strlen(val) + 2);
    }
    else
    {
        pEnvironment= HeapAlloc (GetProcessHeap(),
                            HEAP_ZERO_MEMORY,
                            dwEnvSize= strlen (param) + strlen(val) + 3);
    }

    if (!pEnvironment)
    {
        XPCException ex (ErrorString("HeapReAlloc failed"));
        throw ex;
        return;
    }

    char* pBuf= (char*) pEnvironment;
    strcpy (&pBuf[dwI], param);
    dwI+= strlen (param);
    strcpy (&pBuf[dwI++], "=");
    strcpy (&pBuf[dwI], val);
    pBuf[dwEnvSize-1]= '\0';
}

void XPCProcessAttrib::vSetEnvironment (LPVOID _pEnv)
{
    char *pEnv= (char *) _pEnv;
    
    if (pEnvironment)
        HeapFree (GetProcessHeap(), 0, pEnvironment);

    if (pEnv)
    {
        for (dwEnvSize= 1; ; dwEnvSize++, pEnv++)
        {
            if (!*pEnv  && !*(pEnv+1))
                break;
        }
        pEnvironment= HeapAlloc (GetProcessHeap(), HEAP_ZERO_MEMORY, dwEnvSize+1);
        if (!pEnvironment)
        {
            XPCException ex (ErrorString("HeapAlloc failed"));
            throw ex;
            return;
        }
        memmove (pEnvironment, _pEnv, dwEnvSize+1);
    }
    else
    {
        pEnvironment= NULL;
        dwEnvSize= 0;
    }
}


void XPCProcessAttrib::vSetCommandLine (LPTSTR commandLine)
{
    if (pCommandLine)
    {
        HeapFree (GetProcessHeap(), 0, pEnvironment);
        pCommandLine= NULL;
    }

    if (commandLine)
    {
        pCommandLine= (LPTSTR) HeapAlloc (GetProcessHeap(), 0, strlen (commandLine)+1);
        if (!pCommandLine)
        {
            XPCException ex (ErrorString("HeapAlloc failed"));
            throw ex;
            return;
        }
        strcpy (pCommandLine, commandLine);
    }
}


void XPCProcessAttrib::vSetApplicationName (LPTSTR appName)
{
    if (pApplicationName)
    {
        HeapFree (GetProcessHeap(), 0, pApplicationName);
        pApplicationName= NULL;
    }

    if (appName)
    {
        pApplicationName= (LPTSTR) HeapAlloc (GetProcessHeap(), 0, strlen (appName)+1);
        if (!pApplicationName)
        {
            XPCException ex (ErrorString("HeapAlloc failed"));
            throw ex;
            return;
        }
        strcpy (pApplicationName, appName);
    }
}


void XPCProcessAttrib::vSetCurrentDirectory (LPTSTR currentDirectory)
{
    if (pCurrentDirectory)
    {
        HeapFree (GetProcessHeap(), 0, pCurrentDirectory);
        pCurrentDirectory= NULL;
    }

    if (currentDirectory)
    {
        pCurrentDirectory= (LPTSTR) HeapAlloc (GetProcessHeap(), 0, 
                                strlen (currentDirectory)+1);
        if (!pCurrentDirectory)
        {
            XPCException ex (ErrorString("HeapAlloc failed"));
            throw ex;
            return;
        }
        strcpy (pCurrentDirectory, currentDirectory);
    }
}

char * XPCProcessAttrib::ErrorString(char * sLeader)
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
