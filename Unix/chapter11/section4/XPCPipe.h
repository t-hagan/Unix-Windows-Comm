#ifndef _XPCPipe
#define _XPCPipe

#include <iostream.h>
#include <XPCException.h>
#include <XPCTcpSocket.h>
#include <string.h>

template <class XPCData> class XPCPipe
{
    XPCTcpSocket *socket;    // Communication socket
public:
    // Constructor.  Stores the connected socket
    XPCPipe(XPCTcpSocket *_socket) { socket = _socket; }

    // Constructor.  Listens for connections on a given port
    XPCPipe(long int _iPort);

    // Constructor.  Connects to a given server on a given port
    XPCPipe(long int _iPort, char *_sServer);

    // Descructor.  Removes the socket
    ~XPCPipe() { delete socket; }

    // Accepts new connections
    XPCPipe<XPCData> *vProcessConnection();

    // Returns the socket
    XPCTcpSocket *GetSocket() { return socket; }

    // Recieves piped data
    void operator<<(XPCData &_ob);

    // Sends piped data
    int operator>>(XPCData &_ob);
};

#endif
