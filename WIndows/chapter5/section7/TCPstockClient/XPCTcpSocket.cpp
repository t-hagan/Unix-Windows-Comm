#include <XPCTcpSocket.h>

void XPCTcpSocket::vConnect(char *_sHost)
{
        struct sockaddr_in serverAddress;

        // Resolve the IP address of the given host name
        XPCGetHostInfo getHostInfo(_sHost, NAME);

        // Store the IP address and socket port number
        serverAddress.sin_family = AF_INET;
        serverAddress.sin_addr.s_addr =
inet_addr(getHostInfo.sGetHostAddress());
        serverAddress.sin_port = htons(iPort);

        // Connect to the given address
        if (connect(iSocket, (struct sockaddr *)&serverAddress,
sizeof(serverAddress)) == -1)
        {
                char sMsg[512];

        sprintf(sMsg, "Error Connecting To Socket. %s\0", sGetError());
        XPCException socketExcept(sMsg);
        throw socketExcept;
        return;
    }
}

XPCTcpSocket *XPCTcpSocket::Accept(char *_sHost)
{
        short int iNewSocket;   // Stores the new socket file descriptor

        struct sockaddr_in clientAddress;       // Stores the connected clients
info

        // Gets the length of the client's address
        int iClientAddressLen = sizeof(clientAddress);

        // Accepts a new client connection and stores its socket file descriptor
        if ((iNewSocket = accept(iSocket, (struct sockaddr *)&clientAddress,
&iClientAddressLen)) == -1)
        {
                char sMsg[512];

        sprintf(sMsg, "Error Accepting Socket. %s\0", sGetError());
        XPCException socketExcept(sMsg);
        throw socketExcept;
        return NULL;
    } 

        // If the host name is requested
        if (_sHost != NULL)
        {
                // Get the ascii representation of the address
                char *sAddress = inet_ntoa((struct
in_addr)clientAddress.sin_addr);

                // Get the host name given the address
                XPCGetHostInfo getHostInfo(sAddress, ADDRESS);

                // Store the host name
                strcpy(_sHost, getHostInfo.sGetHostName());
        }

        // Create and return the new XPCTcpSocket object
        XPCTcpSocket *newSocket = new XPCTcpSocket(iNewSocket);
        return newSocket;
}

void XPCTcpSocket::vListen(int _iNumPorts)
{
        // Incoming connections are listened for
        if (listen(iSocket, _iNumPorts) == -1)
        {
                char sMsg[512];

        sprintf(sMsg, "Error Listening To Socket. %s\0", sGetError());
        XPCException socketExcept(sMsg);
        throw socketExcept;
        return;
    }
}       

int XPCTcpSocket::iSendMessage(void *_vMessage, int _iMessageSize)
{
        int iNumBytes;  // Stores the number of bytes sent

        // Sends the message to the connected host
        if ((iNumBytes = send(iSocket, (char *)_vMessage, _iMessageSize, 0)) ==
-1)
        {
                char sMsg[512];

        sprintf(sMsg, "Error sending socket message: %s", sGetError());
        XPCException socketExcept(sMsg);
        throw socketExcept;
        return 0;
    }

        return iNumBytes;
}

#ifdef WINDOWS_NT
int XPCTcpSocket::iRecieveMessageAll(void *_vMessage, int _iMessageSize)
{
        int iNumBytes = 0;      // The number of bytes received

        int iCurrentSize = _iMessageSize;       // The number of bytes wanted to
receive
        int iOffsetSize = 0;                    // The number of bytes currently
recieved

        // While the number of bytes received is less than the number requested
continue to
        // retrieve more data
        while (iNumBytes < iCurrentSize)
        {
                // The socket message is recieved and stored within the mesage
offset by the 
                // offset number of bytes
                iNumBytes = recv(iSocket, (char *)_vMessage + iOffsetSize,
iCurrentSize, 0);
        if (iNumBytes == -1)
        {
            // If the reason for failure is a client disconnect, an exception is
not thrown.
            // The number of bytes returned is 0
            if (WSAGetLastError() == WSAECONNRESET)
                    return 0;
                
                        char sMsg[512];

            sprintf(sMsg, "Error receiving on socket: %s", sGetError());
            XPCException socketExcept(sMsg);
            throw socketExcept;
            return iNumBytes;
        }
                else if (iNumBytes == 0)
                        return iNumBytes;

                // If the total number of bytes requested are not returned, the
offset is adjusted
                // and the number of bytes left to receive is also adjusted
                else if (iNumBytes < iCurrentSize)
                {
                        iOffsetSize += iNumBytes;
                        iCurrentSize = iCurrentSize - iNumBytes;
                        iNumBytes = 0;
                }
        }

        return _iMessageSize;   
}
#endif

int XPCTcpSocket::iRecieveMessage(void *_vMessage, int _iMessageSize, int
_iOption)
{
        int iNumBytes;  // The number of bytes recieved

#ifdef WINDOWS_NT
        if (_iOption == MSG_WAITALL)
                // If the option is MSG_WAITALL and this is a WINDOW_NT machine
call iReceiveMessageAll
                // to process it
                return iRecieveMessageAll(_vMessage, _iMessageSize);
#endif

        // Recieves a TCP socket message.  The number of bytes received is
returned
        iNumBytes = recv(iSocket, (char *)_vMessage, _iMessageSize, _iOption);
        if (iNumBytes == -1)
        {
                // If the reason for failure is a client disconnect, an
exception is not thrown.
                // The number of bytes returned is 0    
#ifdef UNIX
                if (errno == ECONNRESET)
                        return 0;
#else
                if (WSAGetLastError() == WSAECONNRESET)
                        return 0;
#endif
                char sMsg[512];

        sprintf(sMsg, "Error receiving on socket: %s", sGetError());
        XPCException socketExcept(sMsg);
        throw socketExcept;
        return iNumBytes;
    }

        return iNumBytes;
}

void XPCTcpSocket::vBindSocket()
{
        // Bind the socket to the given address and port number
        if (bind(iSocket, (struct sockaddr *)&clientAddress,
sizeof(clientAddress)) == -1)
        {
                char sMsg[512];

        sprintf(sMsg, "Error binding to socket: %s", sGetError());
        XPCException socketExcept(sMsg);
        throw socketExcept;
        return;
    }
}

