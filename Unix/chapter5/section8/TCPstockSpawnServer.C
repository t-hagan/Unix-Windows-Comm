#include <iostream.h>
#include <string.h>
#include <XPCTcpSocket.h>        // The XPCTcpSocket definition 
#include <XPCWaitPid.h>        // The XPCWaitPid definition    
#include <XPCTransaction.h>    // The XPCTransaction definition
#include <stdlib.h>

#define UNIX_PORT_NUM 6100

// XPCStockPosition is contains the position for a given ticker symbol.  
class XPCStockPosition
{
private:
    char sTicker[10];    // Holds the stock ticker symbol
    long int iPosition;    // Holds the stock position
public:
    // The constructor.  Sets the initial position to 0.
    XPCStockPosition() { iPosition = 0; }

    // The vAddPosition method adds or subtracts a given amount from the 
    // position depending on the _cBS parameter.
    void vAddPosition(long int _iAmount, char _cBS) 
    {
        // If the _cBS parameter indicates a "buy", _iAmount is added to the 
        // position, otherwise _iAmount is subtracted.
        if (_cBS == 'B')
            iPosition += _iAmount;
        else 
            iPosition += -1 * _iAmount; 
    } 
    // Returns the current position
    long int iGetPosition() { return iPosition; }

    // Returns the ticker symbol
    char *sGetTicker() { return sTicker; }

    // Sets the ticker symbol
    void vStoreTicker(char *_sTicker) { strcpy(sTicker, _sTicker); }
};

// The XPCUserPosition class stores the up to 50 different stock ticker symbols for a given
// user.
class XPCUserPosition
{
    int iNumTickers;            // The current number of tickers stored
    char sUser[50];            // The user for which ticker positions are
                    // stored.
    XPCStockPosition stockPortfolio[50];    // An array of 50 ticker positions
public:
    // Default constructor. Sets the number of tickers to 0
    XPCUserPosition() { iNumTickers == 0; }

    // Sets the user for the stock position portfolio 
    void vSetUser(char *_sUser) { strcpy(sUser, _sUser); }

    // Returns the user
    char *sGetUser() { return sUser; }

    // Finds the stock position for a given ticker
    XPCStockPosition *FindPosition(char *_sTicker)
    {
        for (int count = 0; count < iNumTickers; count++)
        {
            // If the stock position exists, return its address.
            if (strcmp(stockPortfolio[count].sGetTicker(), _sTicker) == 0)
                return &stockPortfolio[count];
        }
        // If the ticker does not exist return a NULL pointer.
        return (XPCStockPosition *)NULL;
    }

    // Adds a new ticker position to the user's portfolio and returns the new ticker
    // position.
    XPCStockPosition *AddNewPosition(char *_sTicker, int _iAmount, char _cBS)
    {
        // If there are already 50 ticker positions, return NULL
        if (iNumTickers == 50)
            return (XPCStockPosition *)NULL;

        stockPortfolio[iNumTickers].vStoreTicker(_sTicker);
        stockPortfolio[iNumTickers].vAddPosition(_iAmount, _cBS);

        // Increment the number of tickers by 1
        iNumTickers++;
            return &stockPortfolio[iNumTickers-1];
    }            
};

void vStockPositionTracker(XPCTcpSocket *);

int main()
{
    XPCTcpSocket *clientConnection;        // Pointer to new XPCTcpSocket instance 
                                           // derived from the vAccept method.

    int iChildpid;                    // Stores the process ID of the child
                                      // spawned.
    
    try
    {
        // Instance of XPCTcpSocket creates and opens a new TCP socket.  This instance 
        // of XPCTcpSocket is dedicated to accepting client connections.
        XPCTcpSocket portfolioSocket((long int)UNIX_PORT_NUM);

        // Socket is bound to the port number specified by the UNIX_PORT_NUM definition 
        // and client connections are listened for.
        portfolioSocket.vBindSocket();    
        portfolioSocket.vListen();
        
        // The server loops forever accepting client connections and spawning additional 
        // server processes.  The additional server processes are dedicated for 
        // communicating position adjustments with its connected client.
        while(1)
        {
            // Client is accepted and a new socket file descriptor is created for 
            // communication
            clientConnection = portfolioSocket.Accept();

            // Fork a child process
            if ((iChildpid = fork()) == -1)
            {
                cerr << "Error Forking: " << strerror(errno) << endl;
                return 0;
            }

            // If this is the child process, the vStockPositionTracker procedure is 
            // executed.  This procedure communicates with the client.
            if (iChildpid == 0)
            {
                vStockPositionTracker(clientConnection);
            }

            // Prevent a zombie process from being created
            else
            {
                XPCWaitPid waitForChild();
            }
        }
    }

    // All XPC related exceptions are caught.
    catch(XPCException &exceptObject)
    {    
        cout << exceptObject.sGetException() << endl;
        return 0;
    }
}

// The vStockPositionTracker procedure gets called when the child process is spawned.
// This procedure is dedicated for communication with a connected client.  A pointer to 
// the new instance of XPCTcpSocket created by vAccept is sent as a parameter.  If the 
// spawned application was a new application, the socket file descriptor (converted to a 
// character array) would need to be sent instead.
void vStockPositionTracker(XPCTcpSocket *_clientConnection)
{
    int iNewPosition;             // Holds the user's updated ticker position
    XPCUserPosition userPositions[1];          // Since the spawned process only communicates with 
                                               // one user, only one portfolio needs to be stored 
                                               // in memory

    XPCTransaction newTransaction;  // Stores the transaction received from the client.
    XPCStockPosition *individualPosition;// A pointer to an individual ticker
                                         // position

    // Loop forever processing client position updates until the client disconnects.
    while(1)
    {    
        try
        {
            // A new transaction message is received into a XPCTransaction
            // instance. The server suspends processing until a transaction message is
            // received.  Processing is only suspended within the child process.  The 
            // parent process is unaffected and continues accepting client 
            // connections.
            int iNumBytes = _clientConnection->iRecieveMessage((void *)&newTransaction, sizeof(newTransaction));

#ifdef WINDOWS_NT
            if ((iNumBytes == 0) || (WSAGetLastError() == WSAECONNRESET))
#else
            if ((iNumBytes == 0) || (errno == ECONNRESET))
#endif
            {
                // Client has disconnected.  Exit the child process with a status
                // of 1
                exit(1);
            }

            // Display the components of the transaction received.
            cout << "Got New Transaction:" << endl;
            cout << "User: " << newTransaction.sGetUser() << endl;
            cout << "Ticker: " << newTransaction.sGetTicker() << endl;
            cout << "BS: " << newTransaction.cGetBS() << endl;
            cout << "Amount: " << newTransaction.iGetAmount() << endl;

            // If a ticker position exists add the transaction 
            // amount to the current position.
            if ((individualPosition = userPositions[0].FindPosition(newTransaction.sGetTicker())) != NULL)
            {                                                    individualPosition->vAddPosition(newTransaction.iGetAmount(), newTransaction.cGetBS());            
            }
            // If a position does not exists for the given ticker, add a new ticker 
// position to the user's portfolio.
            else
            {
                individualPosition = userPositions[0].AddNewPosition(newTransaction.sGetTicker(), newTransaction.iGetAmount(), newTransaction.cGetBS());
            }
            
            // Display the new position, set the transaction record to reflect 
            // the new position, and send the transaction record to the appropriate 
            // client.
            cout << "New Position: " << individualPosition->iGetPosition() << endl;    
            newTransaction.vSetAmount(individualPosition->iGetPosition());

            // Use the iSendMessage method to send the updated position to the 
            // connected client
            _clientConnection->iSendMessage((void *)&newTransaction, sizeof(newTransaction));    
        }

        // Catch any exceptions that are caused by the TCP socket related classes.
        // Exit the child process with a status of 0
        catch(XPCException &exceptObject)
        {
            cout << exceptObject.sGetException() << endl;
            exit(0);
        }
    }
}
