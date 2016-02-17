#ifndef _XPCMultiConsumer
#define _XPCMultiConsumer

#include <XPCSemaphore.h>

class XPCThread
{
        char sFilename[50];
        char sSemname[50];
        XPCSemaphore *sem;
        XPCSemaphore *mainSem;
public:
        XPCThread(char *_sSemname, char *_sFilename, XPCSemaphore *_sem,
                        XPCSemaphore *_mainsem)
        {
                strcpy(sFilename, _sFilename);
                strcpy(sSemname, _sSemname);
                sem = _sem;
                mainSem = _mainsem;
        }
    char *sGetFilename() { return sFilename; }
    char *sGetSemname() { return sSemname; }
    XPCSemaphore *getSem() { return sem; }
    XPCSemaphore *getMainSem() { return mainSem; }
};

class XPCMessage
{
        char sFilename[50];
        char sMessage[50];
public:
    XPCMessage()
    {
        sFilename[0] = 0;
        sMessage[0] = 0;
    }
    void sPutMessage(char *_sMessage) { strcpy(sMessage, _sMessage); }
    void sPutFilename(char *_sFilename) { strcpy(sFilename, _sFilename); }
    char *sGetFilename() { return sFilename; }
    char *sGetMessage() { return sMessage; }

};

class XPCExitStatus
{
        int iStatus;
        char sMsg[50];
public:
    XPCExitStatus(int _iStatus, char *_sMsg)
    {
        iStatus = _iStatus;
        strcpy(sMsg, _sMsg);
    }
    int iGetStatus() { return iStatus; }
    char *sGetMsg() { return sMsg; }
};

#endif
