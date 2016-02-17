// CService.h: interface for the Service class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(_XPCSERVICE_H__)
#define _XPCSERVICE_H_

#include <windows.h>
#include <winsvc.h>

#include <XPCException.h>

class XPCService  
{
    char* sServiceName; // name of the service
    char* sAppPath;  // path to executable file
    // address of user provided ServiceMain function
    LPSERVICE_MAIN_FUNCTION ServiceMain;
    // address of user provided Handler function
    LPHANDLER_FUNCTION Handler;
    // service status structure
    SERVICE_STATUS ServiceStatus;
    // handle to control status function
    SERVICE_STATUS_HANDLE hServiceStatus;
    // send a control word to the control status function
    void vSetControl (DWORD dwCtrl);
    // error string function
    static char * ErrorString(char * sLeader);

public:
    // consructor-must provide a service name
    // path and call-backs can be null if the service
    // is only opened, not created.
    XPCService(const char* _sServiceName,
            const char* _sAppPath= NULL,
            LPSERVICE_MAIN_FUNCTION _ServiceMain = NULL,
            LPHANDLER_FUNCTION _Handler= NULL);

    virtual ~XPCService();    // destructor

    void vInstallService ();  // install the service
    int iIsServiceInstalled(); // test if service is installed
    void vRemoveService(); // uninstall the service
    void vStartService ();  //start the service

        // get the current service state
    DWORD dwGetState () {return (ServiceStatus.dwCurrentState);}

        // set the service state
    void vSetState (DWORD dwState)
            {ServiceStatus.dwCurrentState= dwState;}

        // report current state to the service control manager
    void vReportState(); 

        // return the handle to the service
    SERVICE_STATUS_HANDLE hGetStatusHandle() {return hServiceStatus;}

        // called by the user's service main to register
        // the control and status handler
    void vServiceMainInit ();
};

#endif // !defined(_XPCSERVICE_H_)
