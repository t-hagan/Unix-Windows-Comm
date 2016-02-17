#include <iostream.h>
#include <fstream.h>
#include "XPCService.h"

// globals that can be accessed by the 
// handler and service main functions
XPCService *myService;
BOOL bRunning;
ofstream *debugFile;


void WINAPI ServiceMain (DWORD dwArgc, LPTSTR* psArgv)
{
    *debugFile << "In ServiceMain." << endl;

    // register the control handler
    myService->vServiceMainInit();
    // init our running flag
    bRunning= TRUE;
    *debugFile << "Setting state to running." << endl;
    // tell the SC manager we're running
    myService->vSetState (SERVICE_RUNNING);
    myService->vReportState();
    // loop until we're told to stop
    while (bRunning)
    {
        *debugFile << "runnning..." << endl;
        // do something useful here
        Sleep (500);
    }
}


void WINAPI Handler (DWORD dwOpcode)
{
    *debugFile << "In Handler." << endl;

    // switch on dwOpcode
    switch (dwOpcode)
    {
    case SERVICE_CONTROL_STOP:
        *debugFile << "In Handler. SERVICE_CONTROL_STOP" << endl;
        myService->vSetState (SERVICE_STOP_PENDING);
        bRunning= FALSE;
        break;

    case SERVICE_CONTROL_PAUSE:
        *debugFile << "In Handler. SERVICE_CONTROL_PAUSE" << endl;
        break;

    case SERVICE_CONTROL_CONTINUE:
        *debugFile << "In Handler. SERVICE_CONTROL_CONTINUE" << endl;
        break;

    case SERVICE_CONTROL_SHUTDOWN:
        *debugFile << "In Handler. SERVICE_CONTROL_SHUTDOWN" << endl;
        break;

    case SERVICE_CONTROL_INTERROGATE:
        *debugFile << "In Handler. SERVICE_CONTROL_INTERROGATE" << endl;
        break;

    default:
        *debugFile << "In Handler. default..." << endl;
        ;
    }
    myService->vReportState();
}


// the only parameter passed to the program is the 
// pathname to the debug file

int main (int agrc, char* argv[])
{
    // open the debug file
    debugFile= new ofstream  (argv[1], ios::app);

    try
    {
        // create the service object
        myService= new XPCService ("MyService",
                                "C:\\projects\\service\\debug\\service.exe",
                                ServiceMain, 
                                Handler);

        // install the service and start it
        myService->vInstallService();
        *debugFile << "Service created. Now starting service." << endl;
        myService->vStartService();
        *debugFile << "Service started." << endl;
    }
    catch (XPCException & ex)
    {
        *debugFile << ex.sGetException() << endl;
    }

    // remove the service and clean up
    myService->vRemoveService();
    delete myService;
    debugFile->close();
    return 0;
}
