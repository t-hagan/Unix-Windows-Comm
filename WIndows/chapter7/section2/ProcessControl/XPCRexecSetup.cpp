#include <XPCRexecSetup.h>

XPCRexecSetup::XPCRexecSetup(char *_sUser, char *_sPass, char *_sCommand, char *_sHost, char *_sIndicator, int _iPort) : XPCTcpSocket((long int)_iPort)
{    
    // Store the user name, password, command, and indicator if it exists
    strcpy(sUsername, _sUser);
    strcpy(sPassword, _sPass);
    strcpy(sCommand, _sCommand);
    if (_sIndicator == NULL)
        sFinishIndicator[0] = 0;
    else
        strcpy(sFinishIndicator, _sIndicator);
    
    // Initialize the receive buffer
    sBuffer[0] = 0;

    // Connect to the rexec daemon.
    vInitConnection(_sHost);
}

void XPCRexecSetup::vInitConnection(char *_sHost)
{
    // An instance of XPCGetHostInfo is created passing its constructor the name of the host and a parameter indicating the name 
    // being passed is the actual name of the host and not its address.
    XPCGetHostInfo rexecHost(_sHost, NAME);

    // Connect to the UNIX server.  If unsuccessful an exception is thrown.
    vConnect(rexecHost.sGetHostAddress());
}

void XPCRexecSetup::vCheckConnectionSuccess()
{
    char          cRexecdRetValue;        // Return value of rexecd.
    char         cProcessResultsRet;    // Return value of iProcessResults.
    char          sResult[255];        // An output stream from rexecd.

    // Get one byte from rexecd. 
    int iNumBytes = iRecieveMessage((void *)&cRexecdRetValue, 1);

    // If there was an error receiving information from rexecd, throw an exception.
    if (iNumBytes < 0)
    {
        XPCException exceptObject("Error receiving from socket");
        throw exceptObject;
        return;
    }

    // If the number of bytes return is 0 a problem has occurred.  An exception is thrown.
    if (iNumBytes == 0)
    {
        XPCException exceptObject("Bytes returned is 0");
        throw exceptObject;
        return;
    }

    // If cRexecdRetValue is 1 then rexecd had problems validating the user name, password, or command.  
    // The exact error message is extracted from rexecd and an exception is thrown.

    if (cRexecdRetValue == 1)
    {
        char sMsg[512];
        strcpy(sMsg, "Connection Error");

        // Add rexecd's error message to sMsg
        while ((cProcessResultsRet = iProcessResults(sResult)) == 1)
            strcat(sMsg, sResult);

        if (cProcessResultsRet == 0)
        strcat(sMsg, "Error retrieving rexecd error message");    
        
        XPCException exceptObject(sMsg);
        throw exceptObject;
        return;
    }
} 

int XPCRexecSetup::iGetBufferFromServer(int &_iNumBytes)
{
    memset(sBuffer, '\0', 255);
    // Retrieve up to 255 bytes from the server using the inherited iReceiveMessage() method.
    _iNumBytes = iRecieveMessage((void *)sBuffer, 255);
    
    // If an error occurred return FAIL
    if (_iNumBytes < 0)
    {
        return FAIL;
    }

    // If no more data is available from the stream return NO_MORE_DATA
    if (_iNumBytes == 0)
        return NO_MORE_DATA;

    return SUCCESS;
}

void XPCRexecSetup::vREXEC()
{
    char sBuf[2];        // Holds a blank string

    // The first component rexecd expects to see is a secondary socket port for communicating signals and
    // standard error messages.  This design is not using a secondary socket port and therefore sends a blank 
    // string instead.

    strcpy(sBuf, "");
    iSendMessage((void *)sBuf, 1);

    // Send the username.
    if (!iSendMessage((void *)sUsername, strlen(sUsername)+1))
    {
        XPCException exceptObject("Error sending username");
        throw exceptObject;
        return;
    }
    
    // Send the password.
    if (!iSendMessage((void *)sPassword, strlen(sPassword)+1))
    {
        XPCException exceptObject("Error sending password");
        throw exceptObject;
        return;
    }

    // Send the command to be spawned and used as the server to rexecd.
    if (!iSendMessage((void *)sCommand, strlen(sCommand)+1))
    {
        XPCException exceptObject("Error sending command");
        throw exceptObject;
        return;
    }

    // Check rexecd's validation of the connection.  If the connection fails, vCheckConnectionSuccess() throws an exception.
    vCheckConnectionSuccess();
}

int XPCRexecSetup::iProcessResults(char *_sResult)
{
    static int iBufferCount = 0;        // Create a static integer to keep track of the position parsed within buffer.
    int iResultCount = 0;                // Counter for the result character array
    int iRetCode;                        // Return code from iGetBufferFromServer()
    static int iBufLen = 0;                // The length of the TCP buffer

    _sResult[0] = 0;            // Initalize the result parameter

    while(1)
    {
        // If the buffer from iGetBufferFromServer() is empty call iGetBufferFromServer() to fill it.
        if (*sBuffer == 0)            
        {
            // If no more data is returned from the server return NO_MORE_DATA.
            if ((iRetCode = iGetBufferFromServer(iBufLen)) == NO_MORE_DATA)
            {
                iBufferCount = 0;
                sBuffer[0] = 0;
                return NO_MORE_DATA;
            }
            // If iGetBufferFromServer() failed return FAIL.
            else if (iRetCode == FAIL)
            {
                iBufferCount = 0;
                sBuffer[0] = 0;
                return FAIL;
            }
        }

        // Fill the _sResults parameter with each line contained within sBuffer.  If the _sResults parameter equals the finish 
        // indicator return NO_MORE_DATA.  Otherwise SUCCESS is returned.

        while (iBufferCount < iBufLen)
        {
            // If buffer equals carriage return, return NO_MORE_DATA if result equals sFinishIndicator.
            // Otherwise SUCCESS is returned.

            if (sBuffer[iBufferCount] == 10)
            {
                _sResult[iResultCount] = 0;
                if  ((sFinishIndicator != NULL) && (!strcmp(_sResult, sFinishIndicator)))
                {
                    iBufferCount = 0;
                    sBuffer[0] = 0;
                    return NO_MORE_DATA;
                }
                else
                {
                    iBufferCount++;
                    return SUCCESS;
                }
            }
            else
            {
                _sResult[iResultCount] = sBuffer[iBufferCount];
                iResultCount++;
                iBufferCount++;
            }
        }
        iBufferCount = 0;
        sBuffer[0] = 0;
    }
}
