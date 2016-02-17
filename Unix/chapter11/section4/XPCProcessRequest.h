#ifndef _XPCProcessRequest
#define _XPCProcessRequest

#include <XPCEndian.h>

#define PROCESS_LIST 0
#define PROCESS_KILL 1
#define QUIT 2

class XPCProcessRequest
{
    char cRequest;            // Stores the type of request
    char sAppName[256];        // Stores the name of the application 
    XPCEndian<long int> iProcessID;     // Stores the ID of the process
public:
    // Default constructor.  Assigns cRequest to QUIT
    XPCProcessRequest() { cRequest = QUIT; }

    // Constructor.  Stores name of the application and optionally the
    // process ID
    XPCProcessRequest(char *_sAppName, long int _iProcessID = 0)
    {
        cRequest = PROCESS_LIST;    // The request is set to PROCESS_LIST for 
                                    // this constructor
        strcpy(sAppName, _sAppName);
        iProcessID = _iProcessID;
    }

    // Constructor.  Stores the request and the ID of the process to kill
    XPCProcessRequest(long int _iProcessID)
    {
        cRequest = PROCESS_KILL;    // The request is set to PROCESS_KILL for 
                        // this constructor
        iProcessID = _iProcessID;
    }

    // Member functions that return the value of the private member variables
    char cGetRequest() { return cRequest; }
    char *sGetAppName() { return sAppName; }

    // Returns the "endian" value of iProcessID
    long int iGetProcessID() { return iProcessID.getEndianValue(); }
};        

#endif

