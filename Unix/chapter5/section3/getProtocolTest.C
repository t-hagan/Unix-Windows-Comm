#include <XPCGetProtocol.h>
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

    // Get the protocol name and number given a name
    try
    {
        XPCGetProtocol getProtocolName("udp");
        cout << "Name: " << getProtocolName.sGetProtocolName() << endl;
        cout << "Number: " << getProtocolName.iGetProtocolNumber() << endl << endl;
    }
    catch(XPCException &exceptObject)
    {
        cout << exceptObject.sGetException() << endl;
    }

    // Get the protocol name and number given a number
    try
    {
        XPCGetProtocol getProtocolNumber(6);
        cout << "Name: " << getProtocolNumber.sGetProtocolName() << endl;
        cout << "Number: " << getProtocolNumber.iGetProtocolNumber() << endl << endl;
    }
    catch(XPCException &exceptObject)  
    {
            cout << exceptObject.sGetException() << endl;  
    }

#ifdef UNIX
    // Loop through all entries within the protocol database    
    XPCGetProtocol getAllProtocols;
    while (getAllProtocols.cGetNextProtocol())
    {
        cout << "Protocol: " << getAllProtocols.sGetProtocolName() << " " << getAllProtocols.iGetProtocolNumber() << endl;
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
