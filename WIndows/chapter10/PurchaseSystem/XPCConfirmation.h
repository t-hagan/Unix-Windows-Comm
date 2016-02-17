#ifndef _XPCConfirmation
#define _XPCConfirmation

#include <XPCEndian.h>

class XPCConfirmation
{
    XPCEndian<double> dPrice;            // The purchase price
    XPCEndian<long int> iConfirmNum;    // The confirmation number
public:
    // Default constructor.  Initialize private data members to 0
    XPCConfirmation() { long int iZero = 0; double dZero = 0.0; dPrice = dZero; iConfirmNum = iZero; }

    // Methods for setting and retrieving private data members
    void vSetPurchasePrice(double _dPrice) 
    {
        dPrice = _dPrice;
    }
    void vSetConfirmNum(long int _iConfirmNum)
    {
        iConfirmNum = _iConfirmNum;
    }
    int iGetConfirmNum() { return iConfirmNum.getEndianValue(); }
    double dGetPurchasePrice() { return dPrice.getEndianValue(); }
};

#endif
