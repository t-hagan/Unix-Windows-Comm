#ifndef _XPCWaitPid
#define _XPCWaitPid

#include <sys/wait.h>
#include <sys/types.h>
#include <XPCException.h>
#include <XPCSignal.h>

// Status types returned by child processes
enum waitClassReturn { NORMAL_TERMINATION, SIGNAL_TERMINATION, STOPPED, CONTINUED, CORED, UNKNOWN };

#define UNKNOWN_STATUS 9999    // Status of a child return is unknown

class XPCWaitPid
{
    int iChildpid;    // Process ID being waited upon
    int iStatus;    // Status value of a returned child
public:
    // Default constructor.  Ignores status of child process return and prevents
    // zombie processes from being created
    XPCWaitPid();

    // Constructor.  Waits for a child process to complete
    XPCWaitPid(int _iChildpid, int _Options = 0);

    // The class returns the completion status of the child
    operator waitClassReturn() 
    {
        return getStatusDefinition();
    }

    // Returns the enumerated status value of the child process's return
    waitClassReturn getStatusDefinition();

    // Returns the enumerated status value of the child process's return as well as
    // the return value associated with the return status
    waitClassReturn getReturnValue(int &_iReturnValue);
};

#endif
