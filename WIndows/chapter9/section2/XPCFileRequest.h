#ifndef _XPCFileRequest
#define _XPCFileRequest

#include <string.h>

class XPCFileRequest
{
    char sFileName[256];
    char sDirection[2];
    char sType[2];
public:
    // default constructor used by server
    XPCFileRequest() { };

    // constructor used by client, includes parameters for all
    // private member variables
    XPCFileRequest(char *_sFileName, char *_sDirection, char *_sType)
    {
        strcpy (sFileName, _sFileName);
        strcpy (sDirection, _sDirection);
        strcpy (sType, _sType);
    }

    // get functions for private members
    char *sGetFileName() { return sFileName; }
    char *sGetDirection() { return sDirection; }
    char *sGetType() { return sType; }
};


#endif
        
