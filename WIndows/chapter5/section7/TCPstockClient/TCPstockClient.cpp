#include <iostream.h>
#include <XPCTcpSocket.h>        // The XPCTcpSocket definition
#include <XPCTransaction.h>    // The XPCTransaction definition
#include <XPCException.h>    // The Inter-platform communication exception class

// Address and port number definitions 
#define UNIX_SERVER "aHost"
#define UNIX_PORT_NUM 6100
#define WINDOWS_NT_PORT_NUM 6100

main()
{
    // Local variables used to hold user-inputted components of a XPCTransaction
    // instance.
    char sUser[50];        // User name
    char sTicker[10];    // Ticker symbol
    char cBS;            // Buy/Sell indicator
    long int iAmount;    // Number of shares bought or sold

    // Before using sockets on the Windows-NT operating system, the Winsock library
    // must be initialized.
    WSADATA wsaData;

    if (WSAStartup(0x101, &wsaData) != 0)
    {
        cout << "Error initializing WinSock library: " << WSAGetLastError() << endl;
        return 0;
    }

    // The name of the user is retrieved from the console and is stored in the local 
    // sUser variable.
    cout << "Enter your User Name: " << flush;
    cin >> sUser;

    try
    {
        // An instance of the XPCTcpSocket is created.  At this point a TCP 
        // socket has been created and a port has been defined.
        XPCTcpSocket tcpSocket((long int)WINDOWS_NT_PORT_NUM);

        // Connect to the server.
        tcpSocket.vConnect(UNIX_SERVER);

        // Loop forever prompting the user for transaction record data.
        while(1)
        {
            cout << "Enter Ticker Symbol: " << flush;
            cin >> sTicker;

            // If the user enters "QUIT", exit the loop.
            if (strcmp(sTicker, "QUIT") == 0)
                break;

            cout << "Enter Buy(B) or Sell(S): " << flush;
            cin >> cBS;
            cout << "Enter Quantity: " << flush;
            cin >> iAmount;

            // Create a transaction record class instance
            XPCTransaction newTransaction(sUser, sTicker, cBS, iAmount);

            // Send the transaction record to the UNIX server using the client
            // version of iSendMessage. 
            tcpSocket.iSendMessage((void *)&newTransaction, sizeof(newTransaction));

            // Suspend process execution until the UNIX server replies with the
            // updated position. The client version of the iReceiveMessage 
            // method is used.
            tcpSocket.iRecieveMessage((void *)&newTransaction, sizeof(newTransaction), MSG_WAITALL);

            
            // Display the newly updated position
            cout << "Ticker: " << newTransaction.sGetTicker() << " New Position: " << newTransaction.iGetAmount() << endl;
        }
    }
    catch(XPCException &exceptObject)
    {
        cout << exceptObject.sGetException() << endl;
    }

    // Disconnect from the Windows NT socket library.  The socket is already closed
    // since the instance of XPCUdpSocket got destructed after it left the 
    // try-catch block.
    WSACleanup();
}
