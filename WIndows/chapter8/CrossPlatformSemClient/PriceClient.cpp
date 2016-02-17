#include <XPCTcpSocket.h>
#include <iostream.h>
#include <rpc/rpc.h>
#include <string.h>
#include <winbase.h>
#include <XPCException.h>
#include <XPCPriceTrans.h>

#define PRICE_SERVER "aHost"
#define SEMAPHORE_SERVER "aHost"

extern "C"
{
#include "CrossPlatformSem.h"
}

main(int argc, char *argv[])
{
    CLIENT *cl;            // RPC connection information
    retvalstruct *ret;    // Return value from cross-platform semaphore RPC calls
    semname mySem;        // The name of the cross-platform semaphore
    char sTicker[4];    // Holds the user-inputted stock ticker symbol
    double dPrice;        // Holds the user-inputted stock price
    char sSource[10];    // Holds the user-inputted client name to be used as the 
                        // pricing source

    // Create a connection to the semaphore server
    cl = clnt_create(SEMAPHORE_SERVER, crossplatformsem, crossplatformsemver, "tcp");
    if (cl == NULL)
    {
        cerr << "Error connecting to server" << endl;
        return 0;
    }

    // Store the name of the semaphore
    mySem = "/price_server_sem";

#ifdef WINDOWS_NT
    WSADATA wsaData;

    // Initialize the winsock library
    if (WSAStartup(0x101, &wsaData) != 0)
    {
        cout << "Error initializing WinSock library: " << WSAGetLastError() << endl;
        return 0;
    }

#endif

    // The TCP socket is created and opened
    XPCTcpSocket aSocket((long int)6548);
    try
    {
        // A connection to the price server is opened
        aSocket.vConnect(PRICE_SERVER);
    }
    catch(XPCException &myExcept)
    {
        cerr << "Error connecting to price server: " << myExcept.sGetException() << endl;
        return 0;
    }

    // The user is prompted for the name of the price client.  This name is used to
    // define the stock price source.
    cout << "Enter Source Name: " << flush;
    cin >> sSource; 

    while(1)
    {
        try
        {
            // Prompt the user for the stock ticker symbol
            cout << "Enter Ticker: " << flush;
            cin >> sTicker;

            // If the user enters "QUT", the client exits.  Upon exiting, the
            // destructor of XPCTcpSocket is called which closes the 
            // client's socket connection.
            if (strcmp(sTicker, "QUIT") == 0)
            {
                return 0;
            }

            // Prompt the user for the stock price
            cout << "Enter Price: " << flush;
            cin >> dPrice;

            // A XPCPriceTrans instance is created
            XPCPriceTrans aPrice(sTicker, dPrice, sSource);

            // The cross-platform semaphore is locked
            ret = lockwait_1(&mySem, cl);
            if (!ret->iretval)
            {
                cerr << "Error locking semaphore: " << ret->serrormsg << endl;
                return 0;
            }

#ifdef WINDOWS_NT
            xdr_free((int(*)(XDR *, char *))xdr_retvalstruct, (char *)ret);
#else
            xdr_free((int(*)(XDR *, void *))xdr_retvalstruct, (char *)ret);
#endif

            // The price information message is sent to the price server
            aSocket.iSendMessage((void *)&aPrice, sizeof(XPCPriceTrans));
        }
        catch(XPCException &myExcept)
        {
            cerr << myExcept.sGetException() << endl;
            return 0;
        }
    }
}
