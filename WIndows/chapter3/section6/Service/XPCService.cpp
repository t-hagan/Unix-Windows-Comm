// Service.cpp: implementation of the Service class.
//
//////////////////////////////////////////////////////////////////////

#include "XPCService.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

XPCService::XPCService(const char* _sServiceName,
            const char* _sAppPath,
            LPSERVICE_MAIN_FUNCTION _ServiceMain,
            LPHANDLER_FUNCTION _Handler)
{
    // save the service name and app path parameters
    sServiceName= new char[strlen(_sServiceName)+1];
    strcpy (sServiceName, _sServiceName);

    sAppPath= new char[strlen(_sAppPath)+1];
    strcpy (sAppPath, _sAppPath);

    // init the SeviceStatus structure
    ServiceStatus.dwServiceType= SERVICE_WIN32_OWN_PROCESS;
    ServiceStatus.dwCurrentState= SERVICE_STOPPED;
    ServiceStatus.dwControlsAccepted= SERVICE_ACCEPT_STOP;
    ServiceStatus.dwWin32ExitCode= 0;
    ServiceStatus.dwServiceSpecificExitCode= 0;
    ServiceStatus.dwCheckPoint= 0;
    ServiceStatus.dwWaitHint= 0;

    // save the addresses of the user's call-backs
    Handler= _Handler;
    ServiceMain= _ServiceMain;
}

XPCService::~XPCService()
{
    delete sServiceName;
    delete sAppPath;
}

void XPCService::vInstallService ()
{
    // get the handle to the service manager
    SC_HANDLE hServiceControlManager= OpenSCManager (NULL, // local PC
                        NULL,    // active services database
                        SC_MANAGER_ALL_ACCESS);

    // throw an exception if we can't get it
    if (!hServiceControlManager)
    {
        XPCException ex (ErrorString("OpenSCManager failed"));
        throw ex;
    }

    // don't bother installing it if it's already installed
    if (!iIsServiceInstalled())
    {
        // create the service
        SC_HANDLE hService= CreateService (hServiceControlManager,
                    sServiceName,
                    sServiceName,
                    SERVICE_ALL_ACCESS,
                    SERVICE_WIN32_OWN_PROCESS,
                    SERVICE_DEMAND_START,
                    SERVICE_ERROR_NORMAL,
                    sAppPath,
                    NULL,
                    NULL,
                    NULL,
                    NULL,
                    NULL);

        if (!hService)
        {
            CloseServiceHandle (hServiceControlManager);
            XPCException ex (ErrorString("CreateService failed"));
            throw ex;
        }
        // close all open handles before exiting
        CloseServiceHandle (hService);
    }
    CloseServiceHandle (hServiceControlManager);
}

int XPCService::iIsServiceInstalled()
{
    int iRet= FALSE;

    // get a handle to the service manager first
    SC_HANDLE hServiceControlManager= OpenSCManager (NULL, // local PC
                        NULL,    // active services database
                        SC_MANAGER_ALL_ACCESS);

    // throw an exeception if we cannot get it
    if (!hServiceControlManager)
    {
        XPCException ex (ErrorString("OpenSCManager failed"));
        throw ex;
        return 0;
    }

    // open the service with query access only
    SC_HANDLE hService= OpenService (hServiceControlManager,
                        sServiceName,
                        SERVICE_QUERY_CONFIG);

    // if we don't get a handle back, the service is not installed
    if (hService)
    {
        iRet= TRUE;
        CloseServiceHandle (hService);
    }

    // close all handles and return result
    CloseServiceHandle (hServiceControlManager);
    return iRet;
}

void XPCService::vSetControl (DWORD dwCtrl)
{
    // get a handle to the service control manager
    SC_HANDLE hServiceControlManager= OpenSCManager (NULL, // local PC
                        NULL,    // active services database
                        SC_MANAGER_ALL_ACCESS);

    // if null then throw an exception
    if (!hServiceControlManager)
    {
        XPCException ex (ErrorString("OpenSCManager failed"));
        throw ex;
    }

    // open the service
    SC_HANDLE hService= OpenService (hServiceControlManager,
                        sServiceName,
                        SERVICE_ALL_ACCESS);

    if (hService)
    {
        // send the control word to the handler
        if (ControlService (hService, dwCtrl, &ServiceStatus))
        {
            XPCException ex (ErrorString("ControlService failed"));
            throw ex;
        }
        CloseServiceHandle (hService);
    }
    else
    {
        // if we couldn't open the service, tidy-up and throw
        // an exception
        XPCException ex (ErrorString("OpenService failed"));
        CloseServiceHandle (hServiceControlManager);
        throw ex;
    }

    CloseServiceHandle (hServiceControlManager);
}

void XPCService::vRemoveService()
{
    // get the handle to the service manager
    SC_HANDLE hServiceControlManager= OpenSCManager (NULL, // local PC
                        NULL,    // active services database
                        SC_MANAGER_ALL_ACCESS);

    // if we can't get it, throw an exception
    if (!hServiceControlManager)
    {
        XPCException ex (ErrorString("OpenSCManager failed"));
        throw ex;
    }

    // open the service, all access
    SC_HANDLE hService= OpenService (hServiceControlManager,
                        sServiceName,
                        SERVICE_ALL_ACCESS);

    // if we got a handle to the service, 
    if (hService)
    {
        // then send a message to ServiceMain to stop
        if (ControlService (hService,
                        SERVICE_CONTROL_STOP, 
                        &ServiceStatus))
        {
            XPCException ex (ErrorString("ControlService failed"));
            throw ex;
        }
        // and delete the service
        if (!DeleteService (hService))
        {
            XPCException ex (ErrorString("DeleteService failed"));
            throw ex;
        }
        CloseServiceHandle (hService);
    }

    CloseServiceHandle (hServiceControlManager);
}


void XPCService::vStartService ()
{
    SERVICE_TABLE_ENTRY serviceTableEntry[]= 
    {
        {sServiceName, ServiceMain},
        {NULL, NULL}
    };

    if (!StartServiceCtrlDispatcher (serviceTableEntry))
    {
        XPCException ex (ErrorString("StartServiceCtrlDispatcher failed"));
        throw ex;
    }
}

void XPCService::vReportState()
{
    if (!SetServiceStatus (hServiceStatus, &ServiceStatus))    
    {
        XPCException ex (ErrorString("SetServiceStatus failed"));
        throw ex;
    }
}

void XPCService::vServiceMainInit()
{
    // register the handler
    hServiceStatus= RegisterServiceCtrlHandler (sServiceName, 
                                            Handler);
    // throw an exception if failure
    if (!hServiceStatus)
    {
        XPCException ex (ErrorString("SetServiceStatus failed"));
        throw ex;
    }

    // notify the SC manager that our start is pending
    vSetState (SERVICE_START_PENDING);
    vReportState();
}

char * XPCService::ErrorString(char * sLeader)
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

