#include <iostream.h>
#include <XPCTcpSocket.h>        // The XPCTcpSocket definition
#include <XPCException.h>    // The XPCException definition
#include <XPCPosition.h>        // The XPCPosition definition 

// Address and port number definitions 
#define UNIX_SERVER "aHost"
#define PORT_NUM2 6200

main()
{
    WSADATA wsaData;        // Stores Winsock library information
    struct timeval timeout;    // The timeout value for the select system call
    char sTicker[10];        // A local variable to hold the user-inputted ticker.
    fd_set fdset;            // Set of "watched" file descriptors

    // The select system call is set to timeout after 10 seconds with no data existing
    // on the socket.
    timeout.tv_sec = 10;
    timeout.tv_usec = 0;

    // The Winsock library in initialized.
    if (WSAStartup(0x101, &wsaData) != 0)
    {
        cout << "Error initializing WinSock library: " << WSAGetLastError() << endl;
        return 0;
    }

    try
    {
        // A TCP socket is created using the port number defined in PORT_NUM2 as
        // the position communication port.
        XPCTcpSocket tcpSocket((long int)PORT_NUM2);

        // The client connects to the specified UNIX server using the socket port 
        // specified in the constructor.
        tcpSocket.vConnect(UNIX_SERVER);

        // Loop forever until the user requests to exit.
        while(1)
        {
            XPCPosition newPosition;    // A new instance of the 
                                        // XPCPosition is created.

            // Request the ticker from the user.
            cout << "Enter Ticker Symbol: " << flush;
            cin >> sTicker;


            // If the user enters the "QUIT" command, the loop breaks, and the
            // client exits.
            if (strcmp(sTicker, "QUIT") == 0)
                break;

            // The ticker is stored in the XPCPosition instance.
            newPosition.vSetTicker(sTicker);

            // The instance of XPCPosition is sent to the server 
            tcpSocket.iSendMessage((void *)&newPosition, sizeof(newPosition));

            // The socket file descriptor set is cleared and the socket file 
            // descriptor contained within tcpSocket is added to the file
            // descriptor set.
            FD_ZERO(&fdset);
            FD_SET(tcpSocket.iGetSocketFd(), &fdset);

            // A select is setup to return when data is available on the socket
            // for reading.  If data is not available after 10 seconds, select
            // returns with a value of 0.  If data is available on the socket,
            // the select returns and data can be retrieved off the socket.
            int iSelectRet = select(tcpSocket.iGetSocketFd() + 1, &fdset, NULL, NULL, &timeout);
            // If select returns a -1, then it failed and the client exits.
            if (iSelectRet == -1)
            {
                cout << "Select failed: " << WSAGetLastError() << endl;
                return 0;
            }

            // If the select returns a 0, then the select timeout.
            else if (iSelectRet == 0)
            {
                cout << "Select timed out: Server Not Responding" << endl;
                continue;
            }

            // If data is available on the TCP socket, the data is retrieved
            // and placed in the instance of XPCPosition.  The new position
            // is displayed for the user. 
            if (FD_ISSET(tcpSocket.iGetSocketFd(), &fdset) != 0)
            {
                tcpSocket.iRecieveMessage((void *)&newPosition, sizeof(newPosition));
                cout << "Ticker: " << newPosition.sGetTicker() << " Position: " << newPosition.iGetPosition() << endl;
            }
        }
    }
    // Socket related exceptions are caught and displayed.
    catch(XPCException &exceptObject)
    {
        cout << exceptObject.sGetException() << endl;
    }

    // The client releases Winsock resources when exiting.
    if (WSACleanup() != 0)
    {
        cout << "Error cleaning up WinSock library: " << WSAGetLastError() << endl;
    }
}
