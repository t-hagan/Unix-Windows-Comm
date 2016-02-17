
#if !defined(XPCPROCESSATTRIB_H_)
#define XPCPROCESSATTRIB_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
#include <windows.h>
#include "XPCException.h"

class XPCProcessAttrib  
{
    friend class XPCProcess;

public:
    XPCProcessAttrib(LPTSTR appName= NULL,
                    LPSTR commandLine= NULL,
                    BOOL processInheritable= FALSE,
                    BOOL threadInheritable= FALSE,
                    BOOL inheritHandles= FALSE,
                    DWORD creationFlags= 0,
                    LPVOID environment= NULL,
                    LPTSTR currentDirectory= NULL);

    virtual ~XPCProcessAttrib();

    void AddEnvironmentVariable (LPCTSTR param, LPCTSTR val);

    LPVOID GetEnvironmentBlock()
    {return pEnvironment;}

    void vSetProcessInheritable (BOOL bInherit) 
    {pProcessSA->bInheritHandle= bInherit;}

    void vSetThreadInheritable (BOOL bInherit)
    {pThreadSA->bInheritHandle= bInherit;}

    void vSetInheritHandles (BOOL bInherit)
    {bInheritHandles= bInherit;}

    void vSetCreationFlag (DWORD dwBitmap)
    {dwCreationFlags |= dwBitmap;}

    void vSetEnvironment (LPVOID pEnv);
    void vSetCommandLine (LPTSTR pCmd);
    void vSetApplicationName (LPTSTR pApp);
    void vSetCurrentDirectory (LPTSTR pDir);

    void vSetStartupInfo (LPSTARTUPINFO pStrt)
    {memmove (pSI, pStrt, sizeof (STARTUPINFO));}

    void vGetProcessInformation (LPPROCESS_INFORMATION pPI)
    {memmove (pPI, pProcessInfo, sizeof (PROCESS_INFORMATION));}

    LPPROCESS_INFORMATION pGetProcessInformation()
    {return pProcessInfo;}
    
private:
    DWORD dwEnvSize;
    LPTSTR pCurrentDirectory;
    LPVOID pEnvironment;
    DWORD dwCreationFlags;
    BOOL bInheritHandles;
    LPTSTR pCommandLine;
    LPTSTR pApplicationName;
    LPPROCESS_INFORMATION pProcessInfo;
    LPSECURITY_ATTRIBUTES pThreadSA;
    LPSECURITY_ATTRIBUTES pProcessSA;
    LPSTARTUPINFO pSI;

    char * ErrorString(char * sLeader);
};

#endif // !defined(XPCPROCESSATTRIB_H_)
