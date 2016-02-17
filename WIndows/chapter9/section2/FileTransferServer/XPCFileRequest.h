#ifndef _XPCFileRequest
#define _XPCFileRequest

#include <string.h>

class XPCFileRequest
{
    char sFileName[256];
    char sDirection[2];
    char sType[2];
public:
    XPCFileRequest() { };
    XPCFileRequest(char *_sFileName, char *_sDirection, char *_sType)
    {
        strcpy (sFileName, _sFileName);
        strcpy (sDirection, _sDirection);
        strcpy (sType, _sType);
    }

    char *sGetFileName() { return sFileName; }
    char *sGetDirection() { return sDirection; }
    char *sGetType() { return sType; }
};

#endif
        
