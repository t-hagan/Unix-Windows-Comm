#ifndef _XPCException
#define _XPCException

#include <string.h>

class XPCException
{
    char sExceptMsg[255];    // Stores the exception message
public:
    // Constructor.  Stores the application defined exception message
    XPCException(char *sMsg) { strncpy(sExceptMsg, sMsg, 254); }

    // Returns the exception message
    const char *sGetException() { return sExceptMsg; }
};

#endif
