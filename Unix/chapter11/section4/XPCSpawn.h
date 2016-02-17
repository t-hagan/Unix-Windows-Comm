#ifndef _XPCSpawn
#define _XPCSpawn

#include <XPCException.h>
#include <XPCTcpSocket.h>
#include <XPCSpawnInfo.h>
#include <XPCPort.h>
#include <string.h>

class XPCSpawn
{
    XPCTcpSocket *Socket;        // The TCP socket used to communicate with the Spawn Server
    int iPort;            // The returned socket port number
public:
    // Constructor.  Connects to the Spawn Server and retrieves the socket port
    // number
    XPCSpawn(long int _iPort, char *_sServer, char *_AppInfo, char _cAction);

    // Overloaded int operator.  Returns the value of the socket port
    operator int() { return iPort; }
};

#endif;
