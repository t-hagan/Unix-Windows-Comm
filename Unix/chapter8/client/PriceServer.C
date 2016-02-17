#include <iostream.h>
#include <rpc/rpc.h>
#include <string.h>
#include <unistd.h>
#include <XPCPthread.h>
#include <XPCTcpSocket.h>
#include <XPCPriceTrans.h>
#include <errno.h>
#include <stdlib.h>

extern "C"
{
#include <CrossPlatformSem.h>
}

#define SEMAPHORE_SERVER "aHost"

class XPCPriceThread
{
private:
    CLIENT *cl;            // Holds Semaphore Server communication information
    XPCTcpSocket *socket;    // Holds price client TCP socket communication information
public:
    // Constructor
    XPCPriceThread(XPCTcpSocket *_socket, CLIENT *_cl) :
        socket(_socket), cl(_cl) { };

    // Returns private data member values
    CLIENT *getClient() { return cl; }
    XPCTcpSocket *getSocket() { return socket; }
};

// Internal price database structure
struct sTickerPrice
{
    char sTicker[4];
    double dPrice;
    char sSource[10];
    struct sTickerPrice *next;
};


// Forward declaration of the HTML publisher routine
void vPublishPrices();

// Forward declaration of client communication thread
void *vUpdatePriceThread(void *_vArg);

// Pointer to the first and last element in the ticker symbol database
struct sTickerPrice *firstElement, *lastElement;

// Total number of elements in the ticker symbol database
int iMaxPriceListCount;

main(int argc, char *argv[])
{
    CLIENT *cl;        // Holds the Semaphore Server connection information
    retvalstruct *ret;    // The return value from the remote semaphore procedure calls
    semstruct newSem;    // Holds the ifnromation sent to the Semaphore Server for creation of a new
                         // semaphore
    semname mySem;        // Hlds the name of the cross-platform semaphore

    iMaxPriceListCount = 0;

    // Initialize the stock price database
    firstElement = NULL;

    // Connection is created with the semaphore server
    cl = clnt_create(SEMAPHORE_SERVER, crossplatformsem, crossplatformsemver, "tcp");
    if (cl == NULL)
    {
        cerr << "Error connecting to server" << endl;
        return 0;
    }

    // The name of the semaphore is stored. 
    mySem = "/price_server_sem";
    newSem.ssemname = mySem;

    // Set the values of the semaphore
    newSem.iinitvalue = 1;
    newSem.imaxvalue = 1;

    // Any previous instances of the semaphore are destroyed
    closesem_1(&mySem, cl);
    removesem_1(&mySem, cl);

    // A new semaphore instance is created
    ret = createsem_1(&newSem, cl);
    if (!ret->iretval)
    {
        cerr << "Error Creating Semaphore: " << ret->serrormsg << endl;
        return 0;
    }

    try
    {
        // The TCP socket is created and opened
        XPCTcpSocket priceSocket((long int)6548);

        // The socket is bound and the server starts listening for connections
        priceSocket.vBindSocket();
        priceSocket.vListen(5);

        while(1)
        {
            // A new connection is accepted
            XPCTcpSocket *newSocket = priceSocket.Accept();

            // A XPCPriceThread object is created to hold the socket and semaphore  info
            XPCPriceThread *newPriceInfo = new XPCPriceThread(newSocket, cl);

            // The price update thread is spawned
            XPCPthread<int> newPriceProvider(vUpdatePriceThread, (void *)newPriceInfo);
        }
    }
    catch(XPCException &exceptObject)
    {
        cout << exceptObject.sGetException() << endl;
        cout << errno << endl;
    }
}
    
void *vUpdatePriceThread(void *_vArg)
{
    // The XPCPriceThread object is cast to its original form
    XPCPriceThread *priceInfo = (XPCPriceThread *)_vArg;

    // Holds the current element in the traversal of the stock price database
    struct sTickerPrice *currentElement;

    // The name of the semaphore is stored
    semname mySem;
    mySem = "/price_server_sem";
    
    retvalstruct *ret;    // The return value from the remote semaphore procedure calls
    int iRet;        // The number of bytes recieved from the client

    while(1)
    {
        // A new instance of the XPCPriceTrans object is created
        XPCPriceTrans *newPriceTrans = new XPCPriceTrans;

        try
        {
            // Retrieves the XPCPriceTrans object from the client
            iRet = priceInfo->getSocket()->iRecieveMessage((void *)newPriceTrans, sizeof(XPCPriceTrans));
            // If the client disconnects, the thread exits
            if ((iRet == 0) || (errno == ECONNRESET))
            {
                delete priceInfo;
                delete newPriceTrans;
                return 1;
            }

            cout << "Got New Price: " << endl;
            cout << "Ticker: " << newPriceTrans->sGetTicker() << endl;
            cout << "Price : " << newPriceTrans->dGetPrice() << endl << endl;

            // The ticker database is updated with the  new XPCPriceTrans object
        
            // First check to see if the stock ticker already exists within the database
            int iFound = 0;
            currentElement = firstElement;
            while (currentElement != NULL) 
            {
                // If it exists update its price
                if (strcmp(newPriceTrans->sGetTicker(), currentElement->sTicker) == 0)
                {
                    iFound = 1;
                    cout << "Found Ticker. Old Price " << currentElement->dPrice << endl;
                    currentElement->dPrice = newPriceTrans->dGetPrice();
                    strcpy(currentElement->sSource, newPriceTrans->sGetSource());
                }
                currentElement = currentElement->next;
            }

            // If the ticker does not exists insert it at the end of the linked-list    
            if (!iFound)
            {
                sTickerPrice *newTicker = new sTickerPrice;
                strcpy(newTicker->sTicker, newPriceTrans->sGetTicker());    
                strcpy(newTicker->sSource, newPriceTrans->sGetSource());
                newTicker->dPrice = newPriceTrans->dGetPrice();
                newTicker->next = NULL;
            
                if (firstElement == NULL)
                {
                    firstElement = newTicker;
                    lastElement = firstElement;
                } 
                else
                {
                    lastElement->next = newTicker;
                    lastElement = lastElement->next;
                }
            }

            // Create the HTML file
            vPublishPrices();

            // Unlock the cross-platform semaphore
            ret = unlock_1(&mySem, priceInfo->getClient());
            if (!ret->iretval)     
            {
                cerr << "Error unlocking semaphore: " << ret->serrormsg << endl;       
            }   

            xdr_free((int(*)(XDR*, void*))xdr_retvalstruct, (char *)ret);
            delete newPriceTrans;
        }
        catch(XPCException &myExcept)
        {
            cerr << "Socket Error: " << myExcept.sGetException() << endl;
            delete priceInfo;
            delete newPriceTrans;
            return 1;
        }    
    }
    return 1;
}

void vPublishPrices()
{
    sTickerPrice *currentElement;

    cout << "<HTML>" << endl;
    cout<< "<HEAD>" << endl;
    cout << "<TITLE>Stock Prices</TITLE>" << endl;
    cout << "</HEAD>" << endl;
    cout<< "<BODY>" << endl;

    cout << "<TABLE BORDER CELLSPACING=1 CELLPADDING=0 BORDERCOLOR=\"#000000\" ALIGN=\"LEFT\">" << endl;
    cout << "<TR>" << endl;
    cout << "<TD><B>Ticker</B></TD>" << endl;
    cout << "<TD><B>Price</B></FONT></TD>" << endl;
    cout << "<TD><B>Source</B></FONT></TD>" << endl;
    cout << "</TR>" << endl;

    // Traverse the price database and create the HTML page
    currentElement = firstElement;
    while(currentElement != NULL)
    {
        cout << "<TR>" << endl;
        cout << "<TD>" << currentElement->sTicker << "</TD>" << endl;
        cout << "<TD>" << currentElement->dPrice << "</TD>" << endl;
        cout << "<TD>" << currentElement->sSource << "</TD>" << endl;
        cout << "</TR>" << endl;
        currentElement = currentElement->next;
    }
    cout << "</TABLE>" << endl;
}
