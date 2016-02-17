#ifndef _XPCUdpSocket
#define _XPCUdpSocket

#include <XPCSocket.h>
#include <XPCException.h>

class XPCUdpSocket : public XPCSocket
{
private:
    char cBind;
public:
    // Constructor.  Creates a UDP socket
    XPCUdpSocket(int _iPort) : XPCSocket("udp", _iPort) { cBind = 0; }

    // Sends a message to a given host listening on a specific port number
    int iSendMessage(void *_vMessage, int _iMsgLen, char *_sHostName, int _iPort = 0);

    // Sends a message to the process that most recently sent a message
    int iSendReply(void *_vMessage, int _iMsgLen);

    // Receives a UDP socket message
    int iReceiveMessage(void *_vMessage, int _iMsgLen, int _iOption = 0);

    // Binds to UDP socket for listening
    void vBindSocket();
};

#endif
