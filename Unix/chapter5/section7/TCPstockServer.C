#include <iostream.h>
#include <string.h>
#include <XPCTcpSocket.h>        // The XPCTcpSocket definition 
#include <XPCTransaction.h>    // The XPCTransaction definition

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

int main()
{
    XPCUserPosition userPositions[50];    // Initialize an array of 50 user positions.
    XPCStockPosition *individualPosition;    // A pointer to an individual ticker
                                             // position.
    int iNumUsers = 0;                // The number of user positions.
    XPCTransaction newTransaction;    // An instance of a stock transaction
                                      // record.
    int iFound;                       // A flag indicating whether or not a
                                      // user position is found.
    XPCTcpSocket    *clientConnection;    // Connection dedicated for client communication
    char sHost[80];                       // Connected host

    try
    {
        // An instance of XPCTcpSocket is created.  A TCP socket is created.
        XPCTcpSocket tcpSocket((long int)UNIX_PORT_NUM);

        tcpSocket.vBindSocket();
        tcpSocket.vListen();

        // Accepts a client connection.  Processing is suspended until the
        // client connects
        clientConnection = tcpSocket.Accept(sHost);
        
        cout << "A client from " << sHost << " connected" << endl;

        // Loop forever receiving transaction messages from Windows NT clients.
        while(1)
        {
            // The client's message is recieved
            int iNumBytes = clientConnection->iRecieveMessage((void *)&newTransaction, sizeof(newTransaction), MSG_WAITALL);
#ifdef WINDOWS_NT
            if (WSAGetLastError() == WSAECONNRESET)
#else
            if ((iNumBytes == 0) || (errno == ECONNRESET))
#endif
            {
                // The client has disconnected.  Delete the connection and
                // suspend processing until a new client connects
                delete clientConnection;
                clientConnection = tcpSocket.Accept(sHost);

                cout << "A client from " << sHost << " connected" << endl;

                // Wait for a message
                clientConnection->iRecieveMessage((void *)&newTransaction, sizeof(newTransaction), MSG_WAITALL);
            }
                        
            // Display the components of the transaction received.
            cout << "Got New Transaction:" << endl;
            cout << "User: " << newTransaction.sGetUser() << endl;
            cout << "Ticker: " << newTransaction.sGetTicker() << endl;
            cout << "BS: " << newTransaction.cGetBS() << endl;
            cout << "Amount: " << newTransaction.iGetAmount() << endl;

            // Set the user-position found flag to false.
            iFound = 0;

            // Loop through all user positions and locate a position matching
            // the user contained within the transaction record received.
            for (int iUserCount = 0; iUserCount < iNumUsers; iUserCount++)
            {
                // If a position exists for the user, locate the position
                // for the ticker contained within the transaction record. 
                if (strcmp(userPositions[iUserCount].sGetUser(), newTransaction.sGetUser()) == 0)
                {
                    // If a ticker position exists add the transaction 
                    // amount to the current position.
                    if ((individualPosition = userPositions[iUserCount].FindPosition(newTransaction.sGetTicker())) != NULL)
                    {
                        individualPosition->vAddPosition(newTransaction.iGetAmount(), newTransaction.cGetBS());            
                    }
                    // If a position does not exists for the given
                    // ticker, add a new ticker position to the user's 
                    // portfolio.
                    else
                    {
                        individualPosition = userPositions[iUserCount].AddNewPosition(newTransaction.sGetTicker(), newTransaction.iGetAmount(), newTransaction.cGetBS());
                    }
                    // Set the user position found flag to true.
                    iFound = 1;
                    break;
                }
            }

            // If the user position found flag is false add a new user 
            // position.
            if (iFound == 0)
            {
                // If the number of user positions is currently 50, no more 
                // user positions can be added.  Set the individual user 
                // position to NULL.
                if (iNumUsers == 50)
                individualPosition = NULL;
                else
                {
                    // Add a new user position.
                    userPositions[iNumUsers].vSetUser(newTransaction.sGetUser());
                    individualPosition = userPositions[iNumUsers].AddNewPosition(newTransaction.sGetTicker(), newTransaction.iGetAmount(), newTransaction.cGetBS());
                    // Increment the number of user positions by 1.
                    iNumUsers++;
                }
            }
        
            // Display the new position, set the transaction record to reflect 
            // the new position, and send the transaction record to the
            // appropriate client.
            cout << "New Position: " << individualPosition->iGetPosition() << endl;    
            newTransaction.vSetAmount(individualPosition->iGetPosition());

            // Uses iSendMessage to send the updated position to the connected
            // client
            clientConnection->iSendMessage((void *)&newTransaction, sizeof(newTransaction));    
        }
    }
    // Catch any exceptions that are caused by the TCP socket related classes.
    catch(XPCException &exceptObject)
    {
        cout << exceptObject.sGetException() << endl;
        return 0;
    }
}

