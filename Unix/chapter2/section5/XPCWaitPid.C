#include <XPCWaitPid.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>

XPCWaitPid::XPCWaitPid()
{
    // Set the return status of an unmonitored child to unknown
    iStatus = UNKNOWN_STATUS;

    // Ignore all SIGCLD signals.  This prevents a zombie from being created
    XPCSignal ignoreChildStatus(SIGCLD, IGNORE);
}

XPCWaitPid::XPCWaitPid(int _iChildpid, int _Options)
{
    iChildpid = _iChildpid;    // Child process being monitored
    int iReturnStatus;    // Child process return status

    // Get the return status of the child.  Ignore signal interrupts.
    while((iReturnStatus = waitpid(iChildpid, &iStatus, _Options)) == EINTR);

    // Throw an exception if an error waiting for the child process occurred    
    if ((iReturnStatus == -1) && ((errno == ECHILD) || (errno == EINVAL)))
    {
        char sErrMsg[512];

        sprintf(sErrMsg, "Error Constructing XPCWaitPid: %s", strerror(errno));
        XPCException semExcept(sErrMsg);
        throw semExcept;
        return;
    }
}

waitClassReturn XPCWaitPid::getStatusDefinition()
{
    // Return the appropriate status value
    if (WIFEXITED(iStatus) != 0)
        return NORMAL_TERMINATION;
    else if (WIFSTOPPED(iStatus) != 0)
        return STOPPED;
    else if (WIFCONTINUED(iStatus) != 0)
        return CONTINUED;
    else if ((WIFSIGNALED(iStatus) != 0) && (WCOREDUMP(iStatus) != 0))
        return CORED;
    else if (WIFSIGNALED(iStatus) != 0)
        return SIGNAL_TERMINATION;
    else
        return UNKNOWN;
}

waitClassReturn XPCWaitPid::getReturnValue(int &_iReturnValue)
{
    // Return the appropriate return status and value of the child
    if (getStatusDefinition() ==  NORMAL_TERMINATION)
        _iReturnValue = WEXITSTATUS(iStatus);
    else if (getStatusDefinition() == SIGNAL_TERMINATION)
        _iReturnValue = WTERMSIG(iStatus);
    else if (getStatusDefinition() == STOPPED)
        _iReturnValue = WSTOPSIG(iStatus);
    else 
        _iReturnValue = -1;

    return getStatusDefinition();
}
