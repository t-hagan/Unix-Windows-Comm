#ifndef _XPCProcessList
#define _XPCProcessList

#include <XPCEndian.h>    // Defines the XPCEndian class (See Section 5.?)

class XPCProcessList
{
    char sProcessName[256];            // The name of the process
    XPCEndian<long int> iProcessID;    // The process ID
public:
    // Constructor. Stores the name of the process the client wishes to monitor.  If
    // no process name is specified, all processes are retrieved by specifying 'ALL'
    XPCProcessList(char *_sProcessName = NULL)
    {
        if (_sProcessName == NULL)
            strcpy(sProcessName, "ALL");
        else
            strcpy(sProcessName, _sProcessName);
    }

    // Constructor.  Stores the name of the process and its ID
    XPCProcessList(char *_sProcessName, long int _iProcessID)
    {
        strcpy(sProcessName, _sProcessName);
        iProcessID = _iProcessID;
    }

    // Retrieves private data member values.
    char *sGetProcessName()  { return sProcessName; }
    long int iGetProcessID() { return iProcessID.getEndianValue(); }
};

#endif
