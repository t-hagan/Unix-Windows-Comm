#ifndef _XPCClientConnection
#define _XPCClientConnection

#include <XPCTcpSocket.h>
#include <XPCSemaphore.h>

class XPCClientConnection
{
        char sSubscribe[1024];        // The name of the message subscribed 
        XPCTcpSocket *aSocket;        // A TCP socket
public:
        XPCClientConnection *next;    // Points to the next element within the linked-list

        // Constructor.  Constructs a node in the linked list of connections
        XPCClientConnection(char *_sSubscribe, XPCTcpSocket *_aSocket)
        {
                strcpy(sSubscribe, _sSubscribe);
                aSocket = _aSocket;
                next = NULL;
        }

        // Returns the values of the private data members
        char *sGetSubscribe() { return sSubscribe; }
        XPCTcpSocket *getSocket() { return aSocket; }
};

#endif
