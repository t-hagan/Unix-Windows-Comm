#include <XPCFileTransfer.h>



main()
{
#ifdef WINDOWS_NT
    // Before using sockets on the Windows-NT operating system, the Winsock library
    // must be initialized.
    WSADATA wsaData;

    if (WSAStartup(0x101, &wsaData) != 0)
    {
        cout << "Error initializing WinSock library: " << WSAGetLastError() << endl;
        return 0;
    }
#endif

    try
    {
        XPCFileTransfer fileTrans ("gsxsv024");
        fileTrans.vTransferFile ("/home/hagant/melville.txt",
            "C:\\temp\\melville.txt",
             TRANSFER_PUT, TRANSFER_TEXT );
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
