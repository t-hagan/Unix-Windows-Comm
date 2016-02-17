#ifndef _XPCSocket
#define _XPCSocket

#include <XPCGetProtocol.h>
#include <XPCGetHostInfo.h>
#include <stdio.h>
#include <string.h>

#ifdef UNIX
    #include <sys/socket.h>
    #include <unistd.h>
    #include <fcntl.h>
    #include <errno.h>
    #include <iostream.h>
    #include <sys/types.h>
    #include <stropts.h>
    #include <sys/filio.h>
#else
    #include <winsock2.h>
#endif

class XPCSocket
{
protected:
    int iPort;        // Socket port number    
    int iSocket;        // Socket file descriptor
    int iBlocking;        // Blocking flag
    char cBind;        // Binding flag
        struct sockaddr_in clientAddress;    // Address of the client that sent data
public:
    // Constructor.  Creates a socket given a protocol (UDP / TCP) and a port number
    XPCSocket(char *_sProtocol, int _iPort);

    // Constructor.  Stores a socket file descriptor
    XPCSocket(int _iSocket) : iSocket(_iSocket) { };

    // Destructor.  Closes the socket
    virtual ~XPCSocket()
    {
        vCloseSocket();
    }

    // Closes the socket
    void vCloseSocket()
    {
        #ifdef WINDOWS_NT
            closesocket(iSocket);
        #else
            close(iSocket);
        #endif
    }

    // The following member functions sets socket options on and off
    void vSetDebug(int _iToggle);
    void vSetBroadcast(int _iToggle);
    void vSetReuseAddr(int _iToggle);
    void vSetKeepAlive(int _iToggle);
    void vSetLinger(struct linger _lingerOption);
    void vSetSocketBlocking(int _iToggle);

    // Sets the size of the send and receive buffer
    void vSetSendBuf(int _iSendBufSize);
    void vSetRecieveBuf(int _iRecieveBufSize);

    // The following member functions retrieve socket option settings
    int iGetDebug();
    int  iGetBroadcast();
    int  iGetReuseAddr();
    int  iGetKeepAlive();
    void vGetLinger(struct linger &_lingerOption);
    int  iGetSendBuf();
    int  iGetRecieveBuf();
    int  iGetSocketBlocking() { return iBlocking; }

    // Returns the socket file descriptor
    int  iGetSocketFd() { return iSocket; }

    // Gets the system error
    char *sGetError()
    {
        static char buf[10];
        #ifdef UNIX
            return strerror(errno);
        #else
            sprintf(buf, "%d", WSAGetLastError());    
            return buf;
        #endif
    }
};

#endif
