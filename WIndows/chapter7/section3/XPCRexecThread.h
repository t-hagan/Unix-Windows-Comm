#ifndef _XPCRexecThread
#define _XPCRexecThread

#include <XPCSemaphore.h>

class XPCRexecThread
{
private:
    char sCommand[80];            // The command to send the server.
    char sProcessName[80];        // The process name for the server to search.
    char sUsername[17];        
    char sPassword[17];
    char sHost[25];                 // Host to connect.
    XPCSemaphore *mySem;             // Pointer to the semaphore instance  
public:
    // Constructor.  Defaults sProcessName to NULL
    XPCRexecThread(char *_sUser, char *_sPass, char *_sHost, char *_sCommand, XPCSemaphore *_mySem, char *_sProcessName)
    {
        strcpy(sUsername, _sUser);
        strcpy(sPassword, _sPass);
        strcpy(sHost, _sHost);
        strcpy(sCommand, _sCommand);
        strcpy(sProcessName, _sProcessName);
        mySem = _mySem;
    }
    // Functions to return private member data.
    char *sGetCommand() { return sCommand; };
    char *sGetProcessName() { return sProcessName; };
    char *sGetUsername() { return sUsername; };
    char *sGetPassword() { return sPassword; };
    char *sGetHost() { return sHost; };
    XPCSemaphore *getSem() { return mySem; }
};

#endif
