#include <XPCPipe.h>
#include <iostream.h>

template <class XPCData> XPCPipe<XPCData>::XPCPipe(long int _iPort)
{
    // Create a new socket
    socket = new XPCTcpSocket((long int) _iPort);

    // Bind the socket
    socket->vBindSocket();

    // Listen for connections on the socket
    socket->vListen(5);
}

template <class XPCData> XPCPipe<XPCData>::XPCPipe(long int _iPort, char *_sServer)
{
    // Create a new socket
    socket = new XPCTcpSocket((long int) _iPort);

    // Connect to the given server     
    socket->vConnect(_sServer);
}

template <class XPCData> XPCPipe<XPCData> *XPCPipe<XPCData>::vProcessConnection()
{
    // Accept socket connections
    XPCTcpSocket *newsocket = socket->Accept();
    
    // Create a new XPCPipe object given the connected socket
    XPCPipe<CData> *newPipe = new XPCPipe<XPCData>(newsocket);
    return newPipe;
}
    
template <class XPCData> void XPCPipe<XPCData>::operator<<(XPCData &_ob)
{
    // Create a data stream to the size of the given CData object
    char *sDataStream = new char[sizeof(_ob)];

    // Copy the contents of the CData object to the data stream
    memcpy((void *)sDataStream, (void *)&_ob, sizeof(_ob));
    
    // Send the data stream
    socket->iSendMessage(sDataStream, sizeof(_ob));
    delete [] sDataStream;
}

template<class XPCData> int XPCPipe<XPCData>::operator>>(XPCData &_ob)
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
