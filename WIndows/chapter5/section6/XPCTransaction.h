#ifndef _XPCTransaction
#define _XPCCTransaction

#include <stdio.h>
#include <string.h>
#include <XPCEndian.h>	// Defines the CEndian class

class XPCTransaction
{
private:
	char sUser[50];	    // User name
	char sTicker[10];	    // Stock ticker symbol
	char cBS;		    // Indicator for buy (B) or sell (s)
	XPCEndian<long int> iAmount; // The amount of stock bought or sold
public:
	// Default constructor
	XPCTransaction() { };

	// Overloaded constructor.  Takes a user name, ticker symbol, buy/sell indicator,
	// and an integer number of shares to buy or sell.
	XPCTransaction(char *_sUser, char *_sTicker, char _cBS, long int _iAmount)
	{
		strcpy(sUser, _sUser);
		strcpy(sTicker, _sTicker);
		cBS = _cBS;
		iAmount = _iAmount;
	}

	// Sets the position amount
	void vSetAmount(long int _iAmount) { iAmount = _iAmount; }

    // sets the ticker
    void vSetTicker (char *_sTicker) { strcpy(sTicker, _sTicker); }
		
	// The following methods are used to retrieve the private data members.
	char *sGetUser() { return sUser; }
	char *sGetTicker() { return sTicker; }
	char cGetBS() { return cBS; }

	// The private data member iAmount is returned in its native format
	long int iGetAmount() { return iAmount.getEndianValue(); }
};

#endif
