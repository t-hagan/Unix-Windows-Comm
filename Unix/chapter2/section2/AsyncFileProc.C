#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <signal.h>
#include <iostream.h>
#include <XPCSignal.h>
#include <XPCFileStat.h>
#include <setjmp.h>

// Macros defining iToggleFlag values
#define OFF 0
#define ON 1

// Flag used to indicate whether file processing is ON or OFF.
// Defaulted to ON
static int iToggleFlag = ON;

// Value of the file's previous image
XPCFileStat *oldFileStat;

// Stores the environment to which siglongjmp jumps to
sigjmp_buf prevEnv;

void vAlarmHandler(int _iSig)
{
    // Retrieve the current file stats
    XPCFileStat newFileStat((char *)oldFileStat->sGetFileName());

    // If the file has changed the location specified by prevEnv is jumped to otherwise
    // the alarm is reset
    if (newFileStat.lGetLastModTime() != oldFileStat->lGetLastModTime())
    {
        siglongjmp(prevEnv, _iSig);
    }
    else
        alarm(1);
}
    
static void vToggleNotification(int _iSig)
{
    // The appropriate action is performed based on the signal recieved
    switch(_iSig)
    {
        case SIGUSR1:
            // If the signal is SIGUSR1, processing is turned on
            if (iToggleFlag == OFF)
            {
                cout << "Turning processing ON" << endl;
                alarm(1);
                iToggleFlag = ON;
            }
            else
                cout << "Processing is already ON" << endl;
            break;
        case SIGUSR2:
            // If the signal is SIGUSR2, processing is turned off
            cout << "Turning processing OFF" << endl;
            alarm(0);
            iToggleFlag = OFF;
            break;
    }
}

void vProcessFile(XPCSignal &signalON)
{
    int iSavemask = 1;
    long lTime;

    // The location to jump is set when the file's modification date has changed
    if (sigsetjmp(prevEnv, iSavemask) != 0)
    {
        // The siglongjmp has jumped to this location since the file has changed
        *oldFileStat = XPCFileStat((char *)oldFileStat->sGetFileName());
        lTime = oldFileStat->lGetLastModTime();
        cout << "Current File Modify Time: " << ctime(&lTime) << endl; 
    }

    // Set the alarm to go off in 1 secotnd
    alarm(1);

    cout << "Starting file Processing" << endl;
    
    for (;;)
    {
        try
        {
            // If the toggle flag is OFF wait for the ON signal
            if (iToggleFlag == OFF)
                signalON.vWaitForSignal();
        }
        catch(XPCException &exceptObject)
        {
            cout << exceptObject.sGetException() << endl;
            return;
        } 
        sleep(1);
    }
}

main (int argc, char *argv[])
{
    if (argc != 2)
    {
        cerr << "Usage: AsyncFileProc <file name>" << endl;
        return 0;
    }
    
    try
    {
        // Create signal handlers for turning file processing on, off, and
        // alarms
        XPCSignal signalON(SIGUSR1, vToggleNotification);
        XPCSignal signalOFF(SIGUSR2, vToggleNotification);
        XPCSignal signalAlarm(SIGALRM, vAlarmHandler);
    
        // Get access to the user-defined file
                oldFileStat = new XPCFileStat(argv[1]);

        // Process the given file
        vProcessFile(signalON);
    }
    catch(XPCException &exceptObject)
    {
        cout << "Application Error: " << exceptObject.sGetException() << endl;
        delete oldFileStat;    
                return 0;
    }
    
    return 1;
}
