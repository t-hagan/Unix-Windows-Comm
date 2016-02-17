#ifndef _XPCClientConnection
#define _XPCClientConnection

#include <XPCSpawnInfo.h>
#include <XPCTcpSocket.h>

class XPCClientConnection
{
        char sAppName[1024];     // Name of the server application
        int iPid;                 // The pricess ID of the spawned server
        XPCTcpSocket *aSocket;// A pointer to the XPCTcpSocket instance that
                                   // contains the connected client's socket info
public:
        XPCClientConnection *next; // Pointer to the next XPCClientConnection object in the list

        // Constructor.
        XPCClientConnection(char *_sAppName, int _iPid, XPCTcpSocket *_aSocket)
        {
                strcpy(sAppName, _sAppName);
                aSocket = _aSocket;
                iPid = _iPid;
                next = NULL;
        }

    // Overloaded equal operator.  Determines if a XPCClientConnection object contains
    // the same info as a XPCServerSpawn object
    int operator==(const XPCServerSpawn &serverOb)
    {
        if ((strcmp(sAppName, serverOb.sGetAppName()) == 0) &&
            (iPid == serverOb.iGetPid()))
            return 1;
        else
            return 0;
    }

    // Member functions that return private member variables
    const char *sGetAppName() const { return sAppName; }
    XPCTcpSocket *getSocket() { return aSocket; }
    const int iGetPid() const { return iPid; }
};

#endif
