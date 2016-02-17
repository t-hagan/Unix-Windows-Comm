#include <XPCGetService.h>
#include <iostream.h>

main()
{
#ifdef WINDOWS_NT
        WSADATA wsaData;

        // Initialize the winsock library
        if (WSAStartup(0x101, &wsaData) != 0)
        {
                cout << "Error intializing Winsock library: " << WSAGetLastError() << endl;
                return 0;
        }
#endif

    // Get the service information based on the service name and protocol
    try
    {
        XPCGetService serviceByName("exec", "tcp");
        cout << "Name: " << serviceByName.sGetServiceName() << endl;
        cout << "Protocol: " << serviceByName.sGetServiceProtocol() << endl;
        cout << "Port: " << serviceByName.iGetServicePort() << endl << endl;
    }
    catch(XPCException &exceptObject)
    {
        cout << exceptObject.sGetException() << endl;
    }

    // Get the service information based on the service port and protocol
    try
    {
        XPCGetService serviceByPort(512, "udp");
        cout << "Name: " << serviceByPort.sGetServiceName() << endl;
        cout << "Protocol: " << serviceByPort.sGetServiceProtocol() << endl;
        cout << "Port: " << serviceByPort.iGetServicePort() << endl << endl;
    }
    catch(XPCException &exceptObject)
    {
            cout << exceptObject.sGetException() << endl;
    }

#ifdef UNIX
    // Get all the services from within the services database
    XPCGetService allServices;
    while (allServices.cGetNextService())
    {
        cout << "Name: " << allServices.sGetServiceName() << endl;
        cout << "Protocol: " << allServices.sGetServiceProtocol() << endl;
        cout << "Port: " << allServices.iGetServicePort() << endl << endl;
    }
#endif

#ifdef WINDOWS_NT
        // Close the winsock library
        if (WSACleanup() != 0)
        {
                cerr << "Error cleaning up the Winsock library: " << WSAGetLastError() << endl;
        }
#endif    
    return 1;
}    
