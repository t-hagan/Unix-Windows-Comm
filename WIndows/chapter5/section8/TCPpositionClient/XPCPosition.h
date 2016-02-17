#include <XPCEndian.h>    // Defines the XPCEndian class

class XPCPosition
{
private:
    char sTicker[10];            // Holds the ticker symbol.
    XPCEndian<long int> iTotalPosition;    // Holds the total position of a given 
                        // ticker
public:
    // The constructor.  Initializes the ticker and total position string to NULL.
    XPCPosition() { sTicker[0] = 0; long int iZero = 0; iTotalPosition = iZero; }

    // Stores the ticker symbol
    void vSetTicker(char *_sTicker) { strcpy(sTicker, _sTicker); }

    // Stores the integer position as a string
    void vSetPosition(long int _iPosition) { iTotalPosition = _iPosition; }

    // Returns the ticker symbol
    char *sGetTicker() { return sTicker; }

    // Returns the endian value of the position
    int iGetPosition() { return iTotalPosition.getEndianValue(); }
};

