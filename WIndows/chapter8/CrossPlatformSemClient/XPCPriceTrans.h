#ifndef _XPCPriceTrans
#define _XPCPriceTrans

#include <XPCEndian.h>

class XPCPriceTrans
{
private:
        char sTicker[4];    // 3 character stock ticker symbol
        XPCEndian<double> dPrice;    // Stock price
        char sSource[10];    // Price source
public:
        // Constructor stores stock information
        XPCPriceTrans(char *_sTicker, double _dPrice, char *_sSource)
        {
            strcpy(sTicker, _sTicker);
            dPrice = _dPrice;
            strcpy(sSource, _sSource);
        }

        // Default constructor
        XPCPriceTrans()
        {
            double dZero = 0.0;

            sTicker[0] = 0;
            dPrice = dZero;
            sSource[0] = 0;
        }
        // Returns private data member values
        char *sGetTicker() { return sTicker; }
        char *sGetSource() { return sSource; }
        double dGetPrice() { return dPrice.getEndianValue(); }
};

#endif
