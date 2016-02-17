#include <XPCDaemon.h>
#include <stdio.h>
#include <stdlib.h>

XPCDaemon::XPCDaemon()
{
    try
    {
        // Ignore stop and terminal signals
        vIgnoreStopSignal();
        vIgnoreTerminalSignals();

        // Spawn the daemon process in the background
        vSpawnDaemon();

        // Make the process a group leader
        vMakeProcessGroupLeader();

        // Close all open file descriptors
        vCloseFileDescriptors();

        // Change the current working directory
        vChangeWorkingDirectory();

        // Set the file permission bits to 0
        vSetFileCreationMask();

        // Ignore the return status from child processes
        vIgnoreChildProcesses();
    }
    catch(XPCException &exceptOb)
    {
        char sDaemonClassMsg[512];

        sprintf(sDaemonClassMsg, "Error Instantiating CDaemon: %s", exceptOb.sGetException());
        XPCException daemonExcept(sDaemonClassMsg);
        throw daemonExcept;
        return;
    }
}

void XPCDaemon::vIgnoreStopSignal()
{
    try
    {
        // Ignore stop signal
        XPCSignal ignoreStopSignal(SIGTSTP, IGNORE);
    }
    catch(XPCException &exceptOb)
    {
        char sDaemonClassMsg[512];

        sprintf(sDaemonClassMsg, "Error ignoring SIGTSTP for CDaemon: %s", exceptOb.sGetException());
        XPCException daemonExcept(sDaemonClassMsg);
        throw daemonExcept;
        return;
    }
}

void XPCDaemon::vIgnoreTerminalSignals()
{
    try
    {
        // Ignore terminal signals
        XPCSignal ignoreBackgroundWrite(SIGTTOU, IGNORE);
        XPCSignal ignoreBackgroundRead(SIGTTIN, IGNORE);
    }
    catch(XPCException &exceptOb)
    {
        char sDaemonClassMsg[512];

        sprintf(sDaemonClassMsg, "Error ignoring terminal I/O signals for XPCDaemon: %s", exceptOb.sGetException());
        XPCException daemonExcept(sDaemonClassMsg);
        throw daemonExcept;
        return;
    }
}

void XPCDaemon::vSpawnDaemon()
{
    int iChildPid;

    // Fork process in the background
    if ((iChildPid = fork()) == -1)
    {
        char sMsg[512];
        sprintf(sMsg, "Error constructing XPCDaemon.  Cannot fork: %s", strerror(errno));
        XPCException daemonExcept(sMsg);
        throw daemonExcept;
        return;
    }
    else if (iChildPid > 0)
        exit(0);
}

void XPCDaemon::vMakeProcessGroupLeader()
{
    // Make the daemon process a group leader and dissaciate it from the
    // controlling terminal
    if (setpgrp() == -1)
    {
        char sMsg[512];
        sprintf(sMsg, "Error making daemon process a group leader: %s", strerror(errno));
        XPCException daemonExcept(sMsg);
        throw daemonExcept;
        return;
    }        
}

void XPCDaemon::vCloseFileDescriptors()
{
    int iFileDescriptor;

    // Close all open file descriptors
    for (iFileDescriptor = 0; iFileDescriptor < NOFILE; iFileDescriptor++)
    {
        if ((close(iFileDescriptor) == -1) && (errno != EBADF))
        {
            char sMsg[512];
            sprintf(sMsg, "Error closing daemon's inherited file descriptors: %s", strerror(errno));
            XPCException daemonExcept(sMsg);
            throw daemonExcept;
            return;
        }
    }
}

void XPCDaemon::vChangeWorkingDirectory()
{
    // Change the working directory to the root directory
    if (chdir("/") == -1)
    {
        char sMsg[512];
        sprintf(sMsg, "Error constructing XPCDaemon.  Cannot Change Director: %s", strerror(errno));
        XPCException daemonExcept(sMsg);
        throw daemonExcept;
        return;
    }
}

void XPCDaemon::vSetFileCreationMask()
{
    // Set the file permissions bits to 0
    umask(0);
}

void XPCDaemon::vIgnoreChildProcesses()
{
    // Ignore signals sent by children processes
    try
    {
        XPCSignal ignoreChildren(SIGCLD, IGNORE);
    }
    catch(XPCException &exceptObject)
    {
        char sMsg[512];
        sprintf(sMsg, "Error constructing XPCDaemon. %s", strerror(errno));
        XPCException daemonExcept(sMsg);
        throw daemonExcept;
        return;
    }
}    
