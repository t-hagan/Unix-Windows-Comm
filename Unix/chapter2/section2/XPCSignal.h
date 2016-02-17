#ifndef _XPCSignal
#define _XPCSignal

#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <XPCException.h>

// Macros defining the single handling for a given signal
#define IGNORE 0
#define DEFAULT 1

class XPCSignal
{
    int iSignal;    // Value of the signal used
public:
    // Constructor.  Takes a signal value and a pointer to a user-defined signal handler
    XPCSignal(int _iSignal, void (*vSigHandler)(int));

    // Constructor.  Takes a signal value and a signal handler of IGNORE or DEFAULT
    XPCSignal(int _iSignal, int _iSigAction);

    // Constructor.  Takes a signal but no handler
    XPCSignal(int _iSignal);

    // Blocks the signal defined by iSignal
    void vBlockSignal();

    // Releases the signal defined by iSignal
    void vReleaseSignal();

    // Wait for all an occurance of any signal
    static void vWaitForAllSignals();

    // Waits for the signal defined by iSignal
    void vWaitForSignal();

    // Sends a signal defined by iSignal to a specific process or group of processes
    void vSendSignal(int _iPid = 0);
};    

#endif
