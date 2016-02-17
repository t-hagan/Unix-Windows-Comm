#include <XPCTcpSocket.h>
#include <XPCFileTransfer.h>
#include <XPCThread.h>
#include <XPCFileRequest.h>
#include <fstream.h>

#ifdef UNIX
void*  vFileTransferThread(void* _vArg);
#else
DWORD WINAPI  vFileTransferThread(LPVOID _vArg);
#endif

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
        XPCTcpSocket fileTransSocket((long int)FILE_TRANSFER_PORT);

        fileTransSocket.vBindSocket();
        fileTransSocket.vListen(5);

        while(1)
        {
            XPCTcpSocket *newConnect = fileTransSocket.Accept();

            XPCThread fileTransfer(vFileTransferThread, (void *)newConnect);
        }
    }
    catch(XPCException &exceptOb)
    {
        cout << "Error in main(): " << exceptOb.sGetException() << endl;
        return 0;
    }

#ifdef WINDOWS_NT
    WSACleanup();
#endif
    return 1;
}
    
#ifdef UNIX
void*  vFileTransferThread(void* _vArg)
#else
DWORD WINAPI  vFileTransferThread(LPVOID _vArg)
#endif
{
    // the socket is passed to us
    XPCTcpSocket *clientConnect = (XPCTcpSocket *)_vArg;
    // the file request object is sent from the client
    XPCFileRequest fileRequest;

    try
    {
        while(1)
        {
            // get the file request object
            int iNumBytes = clientConnect->iRecieveMessage (
                                    (void *)&fileRequest, 
                                    sizeof(XPCFileRequest));

#ifdef UNIX
            if ((iNumBytes == 0) || (errno == ECONNRESET))
#else
            if ((iNumBytes == 0) || 
                    (WSAGetLastError() == WSAECONNRESET))
#endif
            {
                return 0;
            }

            // instantiate a XPCFileTransfer object to handle the 
            // request
            XPCFileTransfer fileTrans (clientConnect, &fileRequest);
        }
    }
    catch(XPCException &exceptOb)
    {
        cerr << exceptOb.sGetException() << endl;
    }

    return 0;
}
