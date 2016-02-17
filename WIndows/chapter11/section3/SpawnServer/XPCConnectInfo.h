#ifndef _XPCConnectInfo
#define _XPCConnectInfo

#include <XPCTcpSocket.h>
#include <XPCSemaphore.h>

class XPCConnectInfo
{
        XPCTcpSocket *aSocket;    // Pointer to the connected socket
        XPCSemaphore *aSem;        // A pointer to the unnamed semaphore
public:
        // Constructor
        XPCConnectInfo(XPCTcpSocket *_aSocket, XPCSemaphore *_aSem) { aSocket = _aSocket; aSem = _aSem; }

        // Member functions that return private data members
        XPCTcpSocket *getSocket() { return aSocket; }
        XPCSemaphore *getSem() { return aSem; }
};

#endif
