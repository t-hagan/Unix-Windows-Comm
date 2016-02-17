#ifndef _XPCRexecAsyncSetup
#define _XPCRexecAsyncSetup

#include <XPCAsyncTcpSocket.h>    // Defines the Asynchronous TCP socket class

#define REXEC_PORT 512

// Abstract base class which defines rexec callback function.
class CRexecDlg
{
public:
    virtual void vResultCallback(CString &result) = 0;    // Callback function to process REXEC
                                                        // messages
};

// Asynchronous XPCRexecSetup class
class XPCRexecAsyncSetup : public XPCAsyncTcpSocket<XPCRexecAsyncSetup>
{
private:
    CString sUsername;    
    CString sPassword;
    CString sCommand;            // Target command spawned by rexecd which becomes the 
                                // server.
    CString sHost;                // Host on which the server is executed.
    CString sFinishIndicator;    // User-defined string to signify the end of an REXEC
                                // session.
    char cFirstReceive;            // A flag indicating the first message sent by the server
    CRexecDlg *targetDialog;    // Base class containing the callback function
    
public:
    // Constructor.  Takes the rexecd socket port number and an optional user-defined code
    // signaling the end of the REXEC session.  The XPCAsyncTcpSocket is constructed with a 
    // pointer to the current instance of CRexecSetup and the REXEC socket port number.
    XPCRexecAsyncSetup(CRexecDlg *_targetDialog, long int _iPort, char *_sFinishIndicator = NULL)  : XPCAsyncTcpSocket<XPCRexecAsyncSetup>(this, _iPort)
    {
        cFirstReceive = 1;
        targetDialog = _targetDialog;
        sFinishIndicator = CString(_sFinishIndicator);
    }
    // Sets up the asynchronous select call and connects to the server.
    void vREXEC()
    {
        // Set the asynchronous select to process read, close, and connect messages
        vAttachSocket();

        // Connect to the rexec host.
		try
		{
			vConnect((char *)(const char *)sHost);
		}
		catch(XPCException &exceptObject)
		{
			if (strcmp(exceptObject.sGetException(), "Error Connecting To Socket. 10035") != 0)
				throw exceptObject;
		}
    }

    // Process socket related error messages
    void vProcessError(char *_sErrMsg)
    {
        targetDialog->vResultCallback(CString(_sErrMsg));
    }

    // Processes a receive message sent by the OnReceive method of XPCAsyncTcpSocket
    void vProcessMessage(char *_sMsgBuf, int _iNumBytes)
    {
        char sBuf[256];        // A return buffer from the socket
        char sAddString[3];    // A character array to hold one byte from buf
        static CString result;    // A line of results from the socket. A line of result is 
                            // defined as a character stream ending in a carriage
                            // return.

        // If an error occurred receiving send the error message to the windows application
        if (_sMsgBuf == NULL)
        {
            XPCException exceptObject("An error occurred receiving data");
            throw exceptObject;
            return;
        }

        // If this is the first time receiving data from the server, check for error messages.
        if (cFirstReceive)
        {
            cFirstReceive = 0;        // Set the firstReceive flag to FALSE.

            // If the first byte of the server message is 1.  An error has occurred.  Send the
            // error message to the windows application.

            if (_sMsgBuf[0] == 1)
            {
                char sMessageBuf[1024];

                sprintf(sMessageBuf, "ERROR: An REXEC error occurred: %s\0", (char *)_sMsgBuf+1);
                targetDialog->vResultCallback(CString(sMessageBuf));
                return;
            }

            // If the first byte was a 0, send a message to the windows application informing 
            // it of a successfulconnect.
            else
            {
                targetDialog->vResultCallback(CString("MSG|Successful Connect"));
                return;
            }
        }

        // Process each line of data contained in _sMsgBuf.  A line is a character array ending in 
        // a carriage return. Send each line to vResultCallback for processing by the windows 
        // application.
        for (int iBufCount = 0; iBufCount < _iNumBytes; iBufCount++)
        {
            if (_sMsgBuf[iBufCount] == 10)
            {
                targetDialog->vResultCallback(result);
                result = "";
            }
            else
            {
                sAddString[0] = _sMsgBuf[iBufCount];
                sAddString[1] = 0;
                result += sAddString;
            }
        }
        //targetDialog->vResultCallback(result);
        //result = "";
    }

    // Processes a socket connect message sent by the OnConnect method of XPCAsyncTcpSocket
    void vProcessConnection(struct sockaddr_in *_sAddress)
    {
        char sBuf[255];                // A temporary buffer
        unsigned int retCode;        // Return code from the CAsyncSocket Send method

        // If an error occurred connecting to the host send an error message to the windows 
        // application.
        if (_sAddress == NULL)
        {
            XPCException exceptObject("Error occurred connecting");
            throw exceptObject;
            return;
        }

        strcpy(sBuf, "");
        iSendMessage((void *)sBuf, 1);
        
        // Send the username.
        if (!iSendMessage((void *)(const char *)sUsername, strlen(sUsername)+1))
        {
            XPCException exceptObject("Error sending username");
            throw exceptObject;
            return;
        }
    
        // Send the password.
        if (!iSendMessage((void *)(const char *)sPassword, strlen(sPassword)+1))
        {
            XPCException exceptObject("Error sending password");
            throw exceptObject;
            return;
        }

        // Send the command to be spawned and used as the server to rexecd.
        if (!iSendMessage((void *)(const char *)sCommand, strlen(sCommand)+1))
        {
            XPCException exceptObject("Error sending command");
            throw exceptObject;
            return;
        }
    }

    // Processes a socket close message sent by the OnClose method of CAsyncTcpSocket
    void vProcessClose(struct sockaddr_in *_sAddress)
    {
        int iRetCode;        // Return code from iReceiveMessage from XPCTcpSocket
        char sBuf[256];        // A buffer of data from the socket
        char sAddString[3];    // A string with one character from buf and an NULL character
        CString result;        // One line from the socket.  A line is a series of characters
                            // terminated with a carriage return

        // While there is data on the socket, read it into sBuf and send each line of data to 
        // vResultCallback.
        while ((iRetCode = iRecieveMessage((void *)sBuf, 255)) > 0)
        {
            for (int iBufCount = 0; iBufCount < iRetCode; iBufCount++)
            {
                if (sBuf[iBufCount] == 10)
                {
                    targetDialog->vResultCallback(result);
                    result = "";
                }
                else
                {
                    sAddString[0] = sBuf[iBufCount];
                    sAddString[1] = 0;
                    result += sAddString;
                }
            }
        }

        // Send the last line of data to result callback.
        targetDialog->vResultCallback(result);

        // Send the send of server session indicator to ResultCallback for end of session
        // processing
        targetDialog->vResultCallback(CString("%%%DONE%%%"));
    }

    char *sGetMessageBuffer(int _iSize) { return NULL; }

    // Functions to set the private members
    void vSetUsername(CString _sUsername) { sUsername = _sUsername; }
    void vSetPassword(CString _sPassword) { sPassword = _sPassword; }
    void vSetHost(CString _sHost) { sHost = _sHost; }
    void vSetCommand(CString _sCommand) { sCommand = _sCommand; }

    // Returns the finish indicator
    CString sGetFinishIndicator() { return sFinishIndicator; }
};

#endif
