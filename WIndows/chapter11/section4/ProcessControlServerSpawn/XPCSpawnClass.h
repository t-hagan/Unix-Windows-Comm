#ifndef _CSpawnClass
#define _CSpawnClass

#include <IPCExceptionClass.h>
#include <CTcpSocketClass.h>
#include <CSpawn.h>
#include <CPort.h>
#include <string.h>

class CSpawnClass
{
    CTcpSocketClass *Socket;    // The TCP socket used to communicate with the Spawn Server
    int iPort;                    // The returned socket port number
public:
    // Constructor.  Connects to the Spawn Server and retrieves the socket port
    // number
    CSpawnClass(long int _iPort, char *_sServer, char *_AppInfo, char _cAction);

    // Overloaded int operator.  Returns the value of the socket port
    operator int() { return iPort; }
};

#endif;
