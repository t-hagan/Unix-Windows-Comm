#include <XPCSignal.h>
#include <iostream.h>
#include <errno.h>

XPCSignal::XPCSignal(int _iSignal, void (*vSigHandler)(int))
{
    iSignal = _iSignal;

    // Sets the user-defined handler as the signal handler for the signal defined
    // in iSignal
    if (sigset(iSignal, vSigHandler) == SIG_ERR)
    {
        XPCException myExcept("Unable to set the signal");
        throw myExcept;
        return;
    }    
}

XPCSignal::XPCSignal(int _iSignal, int _iSigAction)
{
    iSignal = _iSignal;

    if (_iSigAction == IGNORE)
    {
        // If the action defined is SIG_IGN the signal handler is set to ignore the
        // specified signal
        if (sigset(iSignal, SIG_IGN) == SIG_ERR)
        {
                    XPCException myExcept("Unable to set the signal to ignore");
                    throw myExcept;
                    return;
        }
    }
    else if (_iSigAction == DEFAULT)
    {
        // If the action defined is SIG_DFL the signal handler is set to peform the 
        // default action associated with the signal
        if (sigset(iSignal, SIG_DFL) == SIG_ERR)
        {
            XPCException myExcept("Unable to set the signal to default action");
            throw myExcept;
            return;
        }
    }
    else
    {
        XPCException myExcept("Unknown signal action");
        throw myExcept;
        return;
    }
}

XPCSignal::XPCSignal(int _iSignal)
{
    iSignal = _iSignal;
}

void XPCSignal::vBlockSignal()
{
    // Blocks the specified signal
    if (sighold(iSignal) == -1)
    {
        XPCException myExcept("Failed to block signal");
        throw myExcept;
        return;
    }    
}

void XPCSignal::vWaitForAllSignals()
{
    // Suspend execution and wait until a signal is received
    if ((sigpause(0) == -1) && (errno != EINTR))
    {
        XPCException myExcept("Failed to wait for signal");        
        throw myExcept;
        return;
    }
}

void XPCSignal::vWaitForSignal()
{
    // Suspend execution and wait for the signal defined in iSignal
    if ((sigpause(iSignal) == -1) && (errno != EINTR))
    {
        XPCException myExcept("Failed to wait for signal");
        throw myExcept;
        return;
    }    
}    
    
void XPCSignal::vReleaseSignal()
{
    // Releases the specified signal
    if (sigrelse(iSignal) == -1)
    {
        XPCException myExcept("Failed to release signal");
        throw myExcept;
        return;
    }
}

void XPCSignal::vSendSignal(int _iPid)
{
    // Send the signal defined in iSignal to the process or group of processes
    // defined in _iPid
    if (kill(_iPid, iSignal) == -1)
    {
        XPCException myExcept("Failed to send the signal");
        throw myExcept;
        return;
    }
}
