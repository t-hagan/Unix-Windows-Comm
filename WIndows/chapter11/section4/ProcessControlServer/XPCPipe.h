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
    XPCPipe(long int _iPort)
    {
        // Create a new socket
        socket = new XPCTcpSocket((long int) _iPort);

        // Bind the socket
        socket->vBindSocket();

        // Listen for connections on the socket
        socket->vListen(5);
    }

    // Constructor.  Connects to a given server on a given port
    XPCPipe(long int _iPort, char *_sServer)
    {
        // Create a new socket
        socket = new XPCTcpSocket((long int) _iPort);

        // Connect to the given server     
        socket->vConnect(_sServer);
    }

    // Destructor.  Deletes socket
    ~XPCPipe() { delete socket; }

    // Accepts new connections
    XPCPipe<XPCData> *vProcessConnection()
    {
        // Accept socket connections
        XPCTcpSocket *newsocket = socket->Accept();
    
        // Create a new XPCPipe object given the connected socket
        XPCPipe<XPCData> *newPipe = new XPCPipe<XPCData>(newsocket);
        return newPipe;
    }

    // Returns the socket
    XPCTcpSocket *GetSocket() { return socket; }

    // Recieves piped data
    void operator<<(XPCData &_ob)
    {
        // Create a data stream to the size of the given XPCData object
        char *sDataStream = new char[sizeof(_ob)];

        // Copy the contents of the XPCData object to the data stream
        memcpy((void *)sDataStream, (void *)&_ob, sizeof(_ob));
        
        // Send the data stream
        socket->iSendMessage(sDataStream, sizeof(_ob));
        delete [] sDataStream;
    }

    // Sends piped data
    int operator>>(XPCData &_ob)
    {
        int iNumBytes;    // Number of bytes recieved

        // Allocate memory for the raw bytes received
        char *sDataStream = new char[sizeof(_ob)];

        // Wait for the entire message to be received
        iNumBytes = socket->iRecieveMessage(sDataStream, sizeof(_ob), MSG_WAITALL);

        // If no data is received a 0 is returned
        if (iNumBytes == 0)
            return 0;

        // The byte stream is copied into the template defining object
        memcpy((void *)&_ob, (void *)sDataStream, sizeof(_ob));
        delete [] sDataStream;

        return 1;    
    }
};

#endif
