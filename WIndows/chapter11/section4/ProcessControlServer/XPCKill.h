#ifndef _XPCKill
#define _XPCKill

#include <XPCEndian.h>    // Defines the XPCEndian object (See Section 5.?)

class XPCKill
{
    XPCEndian<long int> iProcessID;        // The ID of the process to Kill
public:
    // Constructor.  Stores the ID of the process to kill as well as the confirmation 
    // of the kill
    XPCKill(long int _iProcessID) { iProcessID = _iProcessID; }

    // Returns the endian value of the iProcessID data member
    long int iGetProcessID() { return iProcessID.getEndianValue(); }
};

#endif
