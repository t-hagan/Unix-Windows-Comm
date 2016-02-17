#ifndef _XPCDaemon
#define _XPCDaemon

#include <sys/file.h>
#include <sys/param.h>
#include <errno.h>
#include <XPCException.h>
#include <XPCWaitPid.h>

class XPCDaemon
{
private:
    // Ignores the SIGTSTP signal
    void vIgnoreStopSignal();

    // Ignores the terminal I/O signal
    void vIgnoreTerminalSignals();

    // Spawns the current process in the background
    void vSpawnDaemon();

    // Make the process the group leader
    void vMakeProcessGroupLeader();

    // Close all open file descriptors
    void vCloseFileDescriptors();

    // Change the working directory
    void vChangeWorkingDirectory();

    // Set the file creation mask to 0
    void vSetFileCreationMask();

    // Igore child return status
    void vIgnoreChildProcesses();
public:
    // The constructor
    XPCDaemon();
};

#endif
