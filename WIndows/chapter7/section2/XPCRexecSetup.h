#ifndef _XPCRexecSetup
#define _XPCRexecSetup

#include <XPCTcpSocket.h>    // TCP socket class
#include <XPCGetHostInfo.h>        // Host address translation class
 
#define REXEC_PORT 512            // 512 is the port defined by /etc/services on the UNIX workstation

// Function return definitions
#define FAIL 0
#define SUCCESS 1
#define NO_MORE_DATA 2

class XPCRexecSetup : public XPCTcpSocket
{
private:
    char sUsername[17];        // User name cannot be more that 16 characters.
    char sPassword[17];        // Password cannot be more than 16 characters.
    char sCommand[80];        // The command to execute on the UNIX server.
                            // Make larger if needed.
    char sErr_msg[80];        // Holds error messages. Make larger if needed.
    char sBuffer[255];        // Holds up to 255 bytes returned from the server.
    char sFinishIndicator[80];    // Holds an indicator for comparison against the server's output stream.

    // Connect the TCP socket to the server's rexecd port.
    void  vInitConnection(char *_sHost);

    // Make sure that connection is successful and that the username, password, and
    // command pass verification
    void vCheckConnectionSuccess();

    // Get a return buffer from the server
    int iGetBufferFromServer(int &_iNumBytes);
public:
    // Constructor
    XPCRexecSetup(char *_sUser, char *_sPass, char *_sCommand, char *_sHost, char *_sIndicator, int _iPort);

    // Send the username, password, and command to rexecd and check for success
    void vREXEC();

    // Process a buffer returned from the server and return a result set
    int iProcessResults(char *_sResult);
};

#endif
