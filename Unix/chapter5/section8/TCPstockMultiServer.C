#include <iostream.h>
#include <string.h>
#include <XPCTcpSocket.h>        // The XPCTcpSocket definition 
#include <XPCTransaction.h>    // The XPCTransaction definition
#include <XPCPosition.h>        // The XPCPosition definition
#include <XPCSemaphore.h>        // The XPCSemaphore definition
#include <XPCPthread.h>        // The XPCPthread definition
#include <stdlib.h>

#define PORT_NUM1 6100
#define PORT_NUM2 6200

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

// Structure stores data passed to the thread
struct STcpThreadStruct
{
    XPCTcpSocket *clientConnection;
    XPCSemaphore *positionSem;
};

// Forward declaration of threads
void *vStockPositionTracker(void *);
void *vGlobalPositionTracker(void *);

int iNumUsers;                // Number of portfolios being tracked
XPCUserPosition userPositions[50];    // User portfolios

int main()
{
    XPCTcpSocket *clientConnection;        // Pointer to new XPCTcpSocket instance 
                            // derived from the vAccept method.

    STcpThreadStruct *newTCPThreadStruct;        // Pointer to an instance of STcpThreadStruct

    // Create an unnamed semaphore
    XPCSemaphore positionSem((int)PRIVATE_SEM, (unsigned int)1);

    int iMaxSocketFd = 0;    // Maximum file descriptor size
    fd_set fdset;        // Set of "watched" file descriptors
    
    try
    {
        // Instance of XPCTcpSocket creates and opens a new TCP socket.  This instance 
        // of XPCTcpSocket is dedicated to accepting client connections.
        XPCTcpSocket portfolioSocket((long int)PORT_NUM1);
        portfolioSocket.vBindSocket();    
        portfolioSocket.vListen();

        // TCP socket that communicates the firms' positions
        XPCTcpSocket positionSocket((long int)PORT_NUM2);
        positionSocket.vBindSocket();
        positionSocket.vListen();

        // The maximum socket file descriptor is determined
        if (portfolioSocket.iGetSocketFd() < positionSocket.iGetSocketFd())
            iMaxSocketFd = positionSocket.iGetSocketFd() + 1;
        else
            iMaxSocketFd = portfolioSocket.iGetSocketFd() + 1;    
    
        iNumUsers = 0;    // Set the number of portfolios to 0
    
        // The server loops forever accepting client connections and spawning additional 
        // server processes.  The additional server processes are dedicated for 
        // communicating position adjustments with its connected client.
        while(1)
        {
            FD_ZERO(&fdset);                // Zero out the file descriptor set
            FD_SET(portfolioSocket.iGetSocketFd(), &fdset);    // Add the portfolio socket
            FD_SET(positionSocket.iGetSocketFd(), &fdset);    // Add the position socket

            // Setup the asynchronous socket file descriptor selection
            int iSelectRet = select(iMaxSocketFd, &fdset, NULL, NULL, (struct timeval *)NULL);
            if (iSelectRet == -1)
            {
                cerr << "Select failed: " << strerror(errno) << endl;
                break;
            }

            // Locate the socket file descriptor the client is connecting on
            for (int iSelectCount = 0; iSelectCount < iSelectRet; iSelectCount++)
            {
                if (FD_ISSET(portfolioSocket.iGetSocketFd(), &fdset) != 0)
                {        
                    // A stock trading client has connected
                    clientConnection = portfolioSocket.Accept();
                    
                    // Create an instance of STcpThreadStruct and store the thread related
                    // information
                    newTCPThreadStruct = new STcpThreadStruct;
                    newTCPThreadStruct->clientConnection = clientConnection;
                    newTCPThreadStruct->positionSem = &positionSem;

                    // Create a thread to process the client's request
                    XPCPthread<int> newPositionKeeper(vStockPositionTracker, (void *)newTCPThreadStruct);
                }
                else if (FD_ISSET(positionSocket.iGetSocketFd(), &fdset) != 0)
                {
                    // A position requesting client has connected
                    clientConnection = positionSocket.Accept();

                    // Create an instance of STcpThreadStruct and store the thread related 
                    // information 
                    newTCPThreadStruct = new STcpThreadStruct;     
                    newTCPThreadStruct->clientConnection = clientConnection;       
                    newTCPThreadStruct->positionSem = &positionSem;

                    // Create a thread to process the client's request     
                    XPCPthread<int> newGlobalPosition(vGlobalPositionTracker, (void *)newTCPThreadStruct);       
                }
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

// The firms position communicating thread
void *vGlobalPositionTracker(void *vArg)
{
    // The argument passed to the thread is casted back to its original
    // STcpThreadStruct version.
    STcpThreadStruct *newConnect = (STcpThreadStruct *)vArg;

    XPCStockPosition *individualPosition;     // A pointer to an individual ticker
                                            // position

    // Stores the ticker and global position information
    XPCPosition newPosition;

    // Temporary storage for the firms position given a ticker symbol.
    int iTotalPosition;

    // The server thread loops forever until the client disconnects.
    while(1)
    {
        try
        {
                 // The thread blocks until a message position requesting message
                 // is received from the client.
                 int iNumBytes = newConnect->clientConnection->iRecieveMessage((void *)&newPosition, sizeof(newPosition));

                 // If the client disconnect exit the thread.
                 if ((iNumBytes == 0) || (errno == ECONNRESET))
                 {
                        // The XPCTcpSocket instance created for client
                        // communication is deleted as well as the structure
                        // holding thread specific data.
                        delete newConnect->clientConnection;
                        delete newConnect;
                        return 0;
                 }

                 // Initialize the total position for the given ticker to 0.
                 iTotalPosition = 0;

                 // Lock the portfolio protecting semaphore to avoid parallel
                 // access by more than one thread.
                 newConnect->positionSem->vLockWait();

                 // Display the ticker received on the console.
                 cout << "Got New Position Request" << endl;
                 cout << "Ticker: " << newPosition.sGetTicker() << endl;

                 // Loop through all user positions and locate individual positions
                 // matching the ticker requested in the XPCPosition record.
                 for (int iUserCount = 0; iUserCount < iNumUsers; iUserCount++)
                 {
                        // If a ticker position exists add the position to the
                        // firm's total position
                        if ((individualPosition = userPositions[iUserCount].FindPosition(newPosition.sGetTicker())) != NULL)
                                iTotalPosition += individualPosition->iGetPosition();
                 }

                 // Unlock the portfolio protecting semaphore.
                 newConnect->positionSem->vUnlock();

                 // Store the firms total position as a string in the instance
                 // of XPCPosition.
                 newPosition.vSetPosition(iTotalPosition);
         
                 // Send the XPCPosition instance to the client.
                 newConnect->clientConnection->iSendMessage((void *)&newPosition, sizeof(newPosition));
         }

         // Catch all socket related exceptions.
         catch(XPCException &exceptObject)
         {
              cout << exceptObject.sGetException() << endl;
              return 0;
         }
    }
    return 0;
}

// The stock transaction communicationg thread
void *vStockPositionTracker(void *vArg)
{
    // The argument is cast back to a STcpThreadStruct instance
    STcpThreadStruct *newConnect = (STcpThreadStruct *)vArg;

    XPCTransaction newTransaction;  // Stores the transaction received from the client.
    XPCStockPosition *individualPosition;// A pointer to an individual ticker
                                         // position
    int iFound;

    // Loop forever processing client position updates until the client disconnects.
    while(1)
    {    
        try
        {
            // Receive the client's transaction message
            int iNumBytes = newConnect->clientConnection->iRecieveMessage((void *)&newTransaction, sizeof(newTransaction));

            if ((iNumBytes == 0) || (errno == ECONNRESET))
            {
                // Client has disconnected.  Do cleanup and exit the thread
                delete newConnect->clientConnection;
                delete newConnect;    
                return 1;    
            }

            // Lock the semaphore to prevent multiple access problems
            newConnect->positionSem->vLockWait();

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

                // Unlock the semaphore
                newConnect->positionSem->vUnlock();

                // Use the iSendMessage method to send the updated position to the 
                // connected client
                newConnect->clientConnection->iSendMessage((void *)&newTransaction, sizeof(newTransaction));    
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
