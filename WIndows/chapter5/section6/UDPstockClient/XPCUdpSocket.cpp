#include <XPCUdpSocket.h>

int XPCUdpSocket::iSendMessage(void *_vMessage, int _iMsgLen, char *_sHostName, int _iPort)
{
    sockaddr_in serverAddress;
    int iNumBytes;

    // Bind the socket if it has not already been bound
    if (cBind == 0)
    {
        try
        {
            vBindSocket();
        }
        catch(XPCException &exceptObject)
        {
            char sMsg[512];

            sprintf(sMsg, "Error Binding Socket. Could Not Send Message: %s\0", exceptObject.sGetException());
            XPCException socketExcept(sMsg);
            throw socketExcept;
            return 0;
        }
        cBind = 1;
    }

    try
    {
        // Resolve the host adress
        XPCGetHostInfo sendHost(_sHostName, NAME);
        serverAddress.sin_family = AF_INET;
        serverAddress.sin_addr.s_addr = inet_addr(sendHost.sGetHostAddress());

        if (_iPort == 0)
            _iPort = iPort;
        serverAddress.sin_port = htons(_iPort);    
    }
    catch(XPCException &exceptObject)
    {
        char sMsg[512];

        sprintf(sMsg, "Error Resolving Host Name.  Could Not Send Message: %s\0", exceptObject.sGetException());
        XPCException socketExcept(sMsg);
        throw socketExcept;
        return 0;
    }

    // Send the message to the target process
    if ((iNumBytes = sendto(iSocket, (char *)_vMessage, _iMsgLen, 0, (struct sockaddr *)&serverAddress,
        sizeof(serverAddress))) == -1)
    {
        char sMsg[512];

        sprintf(sMsg, "Error sending socket message: %s", sGetError());
        XPCException socketExcept(sMsg);
        throw socketExcept;
        return 0;
    }    
    return iNumBytes;
}

int XPCUdpSocket::iSendReply(void *_vMessage, int _iMsgLen)
{
    int iNumBytes;

    // Send the reply message to the same client that just sent a message
    if ((iNumBytes = sendto(iSocket, (char *)_vMessage, _iMsgLen, 0, (struct sockaddr *)&clientAddress,
                sizeof(clientAddress))) == -1)
        {
                char sMsg[512];

                sprintf(sMsg, "Error sending socket message: %s", sGetError());
                XPCException socketExcept(sMsg);
                throw socketExcept;
                return 0;
        }
        return iNumBytes;
}
    
int XPCUdpSocket::iReceiveMessage(void *_vMessage, int _iMsgLen, int _iOption)
{
    int iClientAddrLen;
    int iNumBytes;

    // Bind the socket if it has not already been bound
    if (cBind == 0)
    {
        try
        {
            vBindSocket();
        }
        catch(XPCException &exceptObject)
        {
            char sMsg[512];

            sprintf(sMsg, "Error Recieving Message. Could Not Bind Socket: %s\0", exceptObject.sGetException());
            XPCException socketExcept(sMsg);
            throw socketExcept;
            return 0;
        }
        cBind = 1;
    }

    iClientAddrLen = sizeof(clientAddress);

    // Receive a message
    if ((iNumBytes = recvfrom(iSocket, (char *)_vMessage, _iMsgLen, _iOption, (struct sockaddr *)&clientAddress,
        &iClientAddrLen)) ==  -1)
    {
        char sMsg[512];

                sprintf(sMsg, "Error recieving socket message: %s", sGetError()); 
                XPCException socketExcept(sMsg);  
                throw socketExcept;    
                return 0;
        }
    return iNumBytes;      
}

void XPCUdpSocket::vBindSocket()
{
    // Bind the socket to a given port
    if (bind(iSocket, (struct sockaddr *)&clientAddress, sizeof(clientAddress)) == -1)
    {
        char sMsg[512];

                sprintf(sMsg, "Error binding to socket: %s", sGetError());
                XPCException socketExcept(sMsg);
                throw socketExcept;
                return;
        }
    cBind = 1;
}
