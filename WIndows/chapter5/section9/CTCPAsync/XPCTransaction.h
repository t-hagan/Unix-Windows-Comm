#ifndef _XPCTransaction
#define _XPCTransaction

#define POSITION_MESSAGE 0    // Position message definition

#include <XPCEndian.h>

class XPCTransaction
{
private:
    char cType;            // Stores the message type indicator
    char sUser[50];        // Stores the user name
    char sTicker[10];    // Stores the ticker symbol
    char cBS;            // Stores the buy/sell code
    XPCEndian<long int> iAmount;    // Stores the stock transaction amount
public:
    // The constructor initializes message type indicator.
    XPCTransaction() { cType = POSITION_MESSAGE; }
    
    // Retrieves the message type.
    char cGetType() { return cType; }

    // Overloaded constructor.  Takes a user name, ticker symbol, buy/sell indicator,
    // and an integer number of shares to buy or sell.
    XPCTransaction(char *_sUser, char *_sTicker, char _cBS, long int _iAmount)
    {
        cType = POSITION_MESSAGE;
        strcpy(sUser, _sUser);
        strcpy(sTicker, _sTicker);
        cBS = _cBS;
        iAmount = _iAmount;
    }

    // Sets the position amount
    void vSetAmount(long int _iAmount) { iAmount = _iAmount; }
    
    // Sets the ticker symbol
    void vSetTicker(char *_sTicker) { strcpy(sTicker, _sTicker); }

    // The following methods are used to retrieve the private data members.
    char *sGetUser() { return sUser; }
    char *sGetTicker() { return sTicker; }
    char cGetBS() { return cBS; }

    // The private data member iAmount is returned in its native format
    long int iGetAmount() { return iAmount.getEndianValue(); }

    long int iGetOrigAmount() { return iAmount.getOriginalValue(); }
};

#endif
