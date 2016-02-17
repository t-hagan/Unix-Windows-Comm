#ifndef _XPCConnectInfo
#define _XPCConnectInfo

#include <XPCSemaphore.h>
#include <XPCTcpSocket.h>

class XPCConnectInfo
{
        XPCTcpSocket *aSocket;    // Stores a TCP socket
        XPCSemaphore *aSem;        // Stores a private semaphore
public:
    // Constructor.  Stores the socket and semaphore
        XPCConnectInfo(XPCTcpSocket *_aSocket, XPCSemaphore *_aSem) { aSocket = _aSocket; aSem = _aSem; }

    // Methods used to retrieve private data member values
        XPCTcpSocket *getSocket() { return aSocket; }
        XPCSemaphore *getSem() { return aSem; }
};

#endif
