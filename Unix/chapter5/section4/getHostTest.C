#include <XPCGetHostInfo.h>
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
        
    // Retrieve the host name and host address given the host name
    try
    {
        XPCGetHostInfo HostName("aHost", NAME);
        cout << "Name: " << HostName.sGetHostName() << endl;
        cout << "Address: " << HostName.sGetHostAddress() << endl;
    }
    catch (XPCException &exceptObject)
    {
        cout << exceptObject.sGetException() << endl;
    }

    // Retrieve the host name and host address given the host address
    try
    {
            XPCGetHostInfo HostAddress("146.121.32.139", ADDRESS);
            cout << "Name: " << HostAddress.sGetHostName() << endl;
            cout << "Address: " << HostAddress.sGetHostAddress() << endl;
    }
    catch (XPCException &exceptObject)
    {
            cout << exceptObject.sGetException() << endl;
    }

#ifdef UNIX
    // Retrieve all entries from the host name database
    XPCGetHostInfo AllHosts;
    while(AllHosts.cGetNextHost())
    {
        cout << "Name: " << AllHosts.sGetHostName() << endl;
        cout << "Address: " << AllHosts.sGetHostAddress() << endl;
    }
#endif

#ifdef WINDOWS_NT
        // Close the winsock library
        if (WSACleanup() != 0)
        {
                cerr << "Error cleaning up the Winsock library: " << WSAGetLastError() << endl;
        }
#endif
}
