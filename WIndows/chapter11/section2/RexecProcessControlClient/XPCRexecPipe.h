#ifndef _XPCRexecPipe
#define _XPCRexecPipe

#include <XPCRexecSetup.h>

class XPCRexecPipe : private XPCRexecSetup
{
public:
    // Constructor.  Connects to the specified remote host using REXEC protocols and spawns the requested application
    XPCRexecPipe(char *_sUserName, char *_sPassword, char *_sAppName, char *_sHost, char *_sFinishIndicator, int _iPort) : XPCRexecSetup(_sUserName, _sPassword, _sAppName, _sHost, _sFinishIndicator, _iPort)
    {
        // Send the username, password, and command to rexecd and check for success
        vREXEC();
    }

    // Send a message to the remote server
    void operator<<(char *_sCommand)
    {
        iSendMessage((void *)_sCommand, strlen(_sCommand));
    }

    // Receive a message from the remote server
    int operator>>(char *_sResult)
    {
        int iRet;

        iRet = iProcessResults(_sResult);
        if (iRet == FAIL)
        {
            XPCException receiveExcept("Error Receiving Data from REXEC Pipe");
            throw receiveExcept;
            return 0;
        }
        else if (iRet == NO_MORE_DATA)
        {
            _sResult[0] = 0;
            return 0;
        }
        else
            return iRet;
    }
};

#endif
