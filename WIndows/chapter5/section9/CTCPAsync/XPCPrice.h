#ifndef _XPCPrice
#define _XPCPrice

#define PRICE_MESSAGE 1    // Price message definition

#include <XPCEndian.h>

class XPCPrice
{
    char cType;                // Message Type
    char sTicker[10];        // Stock ticker symbol
    XPCEndian<double> dPrice;    // Stock price
public:
    // Constructor
    XPCPrice() { cType = PRICE_MESSAGE; }
    
    // Sets private data values
    void vSetTicker(char *_sTicker) { strcpy(sTicker, _sTicker); }
    void vSetPrice(double _dPrice) { dPrice = _dPrice; }

    // Retrieves private data values
    char cGetType() { return cType; }
    char *sGetTicker() { return sTicker; }
    double dGetPrice() { return dPrice.getEndianValue(); }

    double dGetOrigPrice() { return dPrice.getOriginalValue(); }
};

#endif
