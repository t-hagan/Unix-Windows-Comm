#ifndef _XPCTransaction
#define _XPCTransaction

#include <stdio.h>
#include <stdlib.h>
#include <XPCEndian.h>

class XPCItem
{
    XPCEndian<long int> iQuantity;    // The amount purchased
    char cItemID;            // Order ID
public:
    // Constructor.  Initialize private members
    XPCItem() { long int iZero = 0; iQuantity = iZero; cItemID = -1; }

    // Methods for setting and retrieving private data members
    void vSetNum(long int _iQuantity)
    {
        iQuantity = _iQuantity;
    }
    void vSetItemID(char _cItemID)
    {
        cItemID = _cItemID;
    }
    char cGetID() { return cItemID; }
    int iGetNum() { return iQuantity.getEndianValue(); }
};

class XPCTransaction
{
public:
    XPCItem items[2];        // Holds the ordr information for widgets and sprockets    
    XPCTransaction() { };    // Default constructor
};

#endif
