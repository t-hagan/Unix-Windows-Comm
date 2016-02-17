#include <XPCFileTransfer.h>


main()
{
    // Before using sockets on the Windows-NT operating system, the Winsock library
    // must be initialized.
#ifdef WINDOWS_NT
    WSADATA wsaData;

    if (WSAStartup(0x101, &wsaData) != 0)
    {
        cout << "Error initializing WinSock library: " << WSAGetLastError() << endl;
        return 0;
    }
#endif

    try
    {
        XPCFileTransfer fileTrans ("gsdws124");
        fileTrans.vTransferFile (
                    "C:\\temp\\fileDir.C",
                    "/home/nadelsm/book/mark.C",
                    TRANSFER_PUT, 
                    TRANSFER_TEXT);
    }
    catch (XPCException& exceptObj)
    {
        cout << exceptObj.sGetException() << endl;
    }

#ifdef WINDOWS_NT
    WSACleanup();
#endif

    return 1;
}
