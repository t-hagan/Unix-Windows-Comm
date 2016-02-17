#ifndef _XPCTcpSocket
#define _XPCTcpSocket

#include <XPCSocket.h>

class XPCTcpSocket : public XPCSocket
{
private:
#ifdef WINDOWS_NT
    // Windows NT version of the MSG_WAITALL option
    iRecieveMessageAll(void *_vMessage, int _iMessageSize);
#endif
public:
    // Constructor.  Used to create a new TCP socket given a port
    XPCTcpSocket(long int _iPort) : XPCSocket("tcp", _iPort) { };

    // Constructor.  Called when a client connects and a new file descriptor has
    // be created as a result.    
    XPCTcpSocket(short int _iSocketFd) : XPCSocket(_iSocketFd) { };

    // Sends a message to a connected host. The number of bytes sent is returned
    int iSendMessage(void *_vMessage, int _iMessageSize);

    // Receives a TCP message 
    int iRecieveMessage(void *_vMessage, int _iMessageSize, int _iOption = 0);

    // Binds the socket to an address and port number
    void vBindSocket();

    // Accepts a connecting client.  The address of the connected client is stored in the
    // parameter
    XPCTcpSocket *Accept(char *_sHost = NULL);

    // Listens to connecting clients
    void vListen(int _iNumPorts = 5);

    // Connects to a client specified by a supplied host name
    virtual void vConnect(char *_sHost);
};

#endif
