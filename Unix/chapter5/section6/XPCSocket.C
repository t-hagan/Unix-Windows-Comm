#include <XPCSocket.h>

XPCSocket::XPCSocket(char *_sProtocol, int _iPort)
{
    iPort = _iPort;
    iBlocking = 0;

    try
    {
        // Retrieve the socket protocol
        XPCGetProtocol socketProtocol(_sProtocol);

        // If the protocol is UDP a UDP socket is created
        if (strcmp(socketProtocol.sGetProtocolName(), "udp") == 0)
        {
            if ((iSocket = socket(AF_INET, SOCK_DGRAM, socketProtocol.iGetProtocolNumber())) == -1)
            {
                char sMsg[512];

                sprintf(sMsg, "Error opening socket: %s", sGetError());
                XPCException socketExcept(sMsg);
                throw socketExcept;
                return;
            }
        }

        // If the protocol is TCP a TCP socket is created
        else if (strcmp(socketProtocol.sGetProtocolName(), "tcp") == 0)
        {
            if ((iSocket = socket(AF_INET, SOCK_STREAM, socketProtocol.iGetProtocolNumber())) == -1)
            {
                char sMsg[512];

                sprintf(sMsg, "Error opening socket: %s", sGetError());
                XPCException socketExcept(sMsg);
                throw socketExcept;
                return;
            }
        }
    }
    catch(XPCException &exceptObject)
    {
        char sMsg[512];

        sprintf(sMsg, "Protocol Error Definition: %s", exceptObject.sGetException());
        XPCException socketExcept(sMsg);
        throw socketExcept;
        return;
    }

    // The client address is initialized to all addresses at the specified port
    clientAddress.sin_family = AF_INET;
    clientAddress.sin_addr.s_addr = htonl(INADDR_ANY);
    clientAddress.sin_port = htons(iPort);
}
    
void XPCSocket::vSetDebug(int _iToggle)
{
    if (setsockopt(iSocket, SOL_SOCKET, SO_DEBUG, (char *)&_iToggle, sizeof(_iToggle)) == -1)
    {
        char sMsg[512];

        sprintf(sMsg, "Error Setting Debug Option: %s", sGetError());
        XPCException sockOptExcept(sMsg);
        throw sockOptExcept;
        return;
    }
}

void XPCSocket::vSetBroadcast(int _iToggle)
{
        if (setsockopt(iSocket, SOL_SOCKET, SO_BROADCAST, (char *)&_iToggle, sizeof(_iToggle)) == -1) 
        { 
                char sMsg[512];          
 
                sprintf(sMsg, "Error Setting Broadcast Option: %s", sGetError()); 
                XPCException sockOptExcept(sMsg); 
                throw sockOptExcept; 
                return; 
        } 
} 

void XPCSocket::vSetReuseAddr(int _iToggle)
{
        if (setsockopt(iSocket, SOL_SOCKET, SO_REUSEADDR, (char *)&_iToggle, sizeof(_iToggle)) == -1) 
        { 
                char sMsg[512];          
 
                sprintf(sMsg, "Error Setting Reuseaddr Option: %s", sGetError()); 
                XPCException sockOptExcept(sMsg); 
                throw sockOptExcept; 
                return; 
        } 
} 

void XPCSocket::vSetKeepAlive(int _iToggle)
{
        if (setsockopt(iSocket, SOL_SOCKET, SO_KEEPALIVE, (char *)&_iToggle, sizeof(_iToggle)) == -1) 
        { 
                char sMsg[512];          
 
                sprintf(sMsg, "Error Setting Keepalive Option: %s", sGetError()); 
                XPCException sockOptExcept(sMsg); 
                throw sockOptExcept; 
                return; 
        } 
} 

void XPCSocket::vSetLinger(struct linger _lingerOption)
{
        if (setsockopt(iSocket, SOL_SOCKET, SO_LINGER, (char *)&_lingerOption, sizeof(struct linger)) == -1) 
        { 
                char sMsg[512];          
 
                sprintf(sMsg, "Error Setting Linger Option: %s", sGetError()); 
                XPCException sockOptExcept(sMsg); 
                throw sockOptExcept; 
                return; 
        } 
} 

void XPCSocket::vSetSendBuf(int _iSendBufSize)
{
        if (setsockopt(iSocket, SOL_SOCKET, SO_SNDBUF, (char *)&_iSendBufSize, sizeof(_iSendBufSize)) == -1) 
        { 
                char sMsg[512];          
 
                sprintf(sMsg, "Error Setting SendBufSize Option: %s", sGetError()); 
                XPCException sockOptExcept(sMsg); 
                throw sockOptExcept; 
                return; 
        } 
} 

void XPCSocket::vSetRecieveBuf(int _iRecieveBufSize)
{
        if (setsockopt(iSocket, SOL_SOCKET, SO_SNDBUF, (char *)&_iRecieveBufSize, sizeof(_iRecieveBufSize)) == -1)
        { 
                char sMsg[512];          
 
                sprintf(sMsg, "Error Setting RecieveBufSize Option: %s", sGetError()); 
                XPCException sockOptExcept(sMsg); 
                throw sockOptExcept; 
                return; 
        } 
}

void XPCSocket::vSetSocketBlocking(int _iToggle)
{
    int iSocketFlags;
    char sMsg[512];

    if (_iToggle)
    {
        if (iGetSocketBlocking())
            return;
        else
        {
            iBlocking = 1;
            // Socket blocking is turned ON
#ifdef WINDOWS_NT
            if (ioctlsocket(iSocket, FIONBIO, (unsigned int *)&iBlocking) == -1)
#else
            if (ioctl(iSocket, FIONBIO, (char *)&iBlocking) == -1)
#endif
            {
                    sprintf(sMsg, "Error Turning ON Socket Blocking Status: %s", sGetError());
                    XPCException sockOptExcept(sMsg);
                    throw sockOptExcept;
                    return;
            }                        

        }
    }
    else
    {
        if (!iGetSocketBlocking())
                return;
        else
        {
            iBlocking = 0;
            // Socket blocking is turned off
#ifdef WINDOWS_NT
            if (ioctlsocket(iSocket, FIONBIO, (unsigned int *)&iBlocking) == -1)
#else
            if (ioctl(iSocket, FIONBIO, (char *)&iBlocking) == -1)
#endif
            {
                sprintf(sMsg, "Error Turning OFF Socket Blocking Status: %s", sGetError());
                XPCException sockOptExcept(sMsg);
                throw sockOptExcept;
                return;
            }

        }
    }
}

int XPCSocket::iGetDebug()
{
    int iGetOption;

    int iOptionLen = sizeof(iGetOption);
    
    if (getsockopt(iSocket, SOL_SOCKET, SO_DEBUG, (char *)&iGetOption, &iOptionLen) == -1)
    {
        char sMsg[512];

        sprintf(sMsg, "Error Recieving Debug Option: %s", sGetError());
        XPCException sockOptExcept(sMsg);
        throw sockOptExcept;
        return -1;
    }
    return iGetOption;
}

int XPCSocket::iGetBroadcast()
{
    int iGetOption;        
    
    int iOptionLen = sizeof(iGetOption);

    if (getsockopt(iSocket, SOL_SOCKET, SO_BROADCAST, (char *)&iGetOption, &iOptionLen) == -1)
    {
            char sMsg[512];

            sprintf(sMsg, "Error Extracting Broadcast Option: %s", sGetError());
            XPCException sockOptExcept(sMsg);
            throw sockOptExcept;
            return -1;
    }
    return iGetOption;    
}

int XPCSocket::iGetReuseAddr()
{
        int iGetOption;        

        int iOptionLen = sizeof(iGetOption);

        if (getsockopt(iSocket, SOL_SOCKET, SO_REUSEADDR, (char *)&iGetOption, &iOptionLen) == -1)
        {
            char sMsg[512];

            sprintf(sMsg, "Error Extracting Resuseaddr Option: %s", sGetError());
            XPCException sockOptExcept(sMsg);
            throw sockOptExcept;
            return -1;
        } 
        return iGetOption;    
}

int XPCSocket::iGetKeepAlive()
{
        int iGetOption;        

        int iOptionLen = sizeof(iGetOption);

        if (getsockopt(iSocket, SOL_SOCKET, SO_KEEPALIVE, (char *)&iGetOption, &iOptionLen) == -1)
        {
                char sMsg[512];

                sprintf(sMsg, "Error Extracting Keepalive Option: %s", sGetError());
                XPCException sockOptExcept(sMsg);
                throw sockOptExcept;
                return -1;
        }
        return iGetOption;    
}

void XPCSocket::vGetLinger(struct linger &_lingerOption)
{
    int iOptionLen = sizeof(struct linger);

    if (getsockopt(iSocket, SOL_SOCKET, SO_LINGER, (char *)&_lingerOption, &iOptionLen) == -1)
    {
            char sMsg[512];

            sprintf(sMsg, "Error Extracting Linger Option: %s", sGetError());
            XPCException sockOptExcept(sMsg);
            throw sockOptExcept;
            return;
    }
    return;
}

int XPCSocket::iGetSendBuf()
{
    int iSendBuf;

    int iOptionLen = sizeof(iSendBuf);

    if (getsockopt(iSocket, SOL_SOCKET, SO_SNDBUF, (char *)&iSendBuf, &iOptionLen) == -1)
    {
            char sMsg[512];

            sprintf(sMsg, "Error Extracting SendBuf Option: %s", sGetError());
            XPCException sockOptExcept(sMsg);
            throw sockOptExcept;
            return -1;
    }
    return iSendBuf;
}    

int XPCSocket::iGetRecieveBuf()
{
        int iRcvBuf;

        int iOptionLen = sizeof(iRcvBuf);

        if (getsockopt(iSocket, SOL_SOCKET, SO_RCVBUF, (char *)&iRcvBuf, &iOptionLen) == -1)
        {
                char sMsg[512];

                sprintf(sMsg, "Error Extracting RcvBuf Option: %s", sGetError());
                XPCException sockOptExcept(sMsg);
                throw sockOptExcept;
                return -1;
        }
        return iRcvBuf;
}
