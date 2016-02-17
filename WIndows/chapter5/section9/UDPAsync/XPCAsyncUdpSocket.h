#ifndef _XPCAsyncUdpSocket
#define _XPCAsyncUdpSocket

#include <XPCUdpSocket.h>    // XPCUdpSocket Definition
#include <afxsock.h>            // CAsyncSocket Definition 
#include <XPCException.h>    // XPCException Definition

template <class CAppClass> class XPCAsyncUdpSocket : public XPCUdpSocket, public CAsyncSocket
{
    CAppClass *CAppClassPtr;    // A pointer to the Application class used to define 
                                // the instance of XPCAsyncUdpSocket
    char *sMsgBuf;                // A buffer that stores a received socket message.
public:
    // The Constructor.  Takes a pointer to the application class and the socket port 
    // that is communicated upon.  A UDP socket is created and the receive message 
    // buffer is allocated with the maximum number of bytes the UDP socket can 
    // receive.
    XPCAsyncUdpSocket(CAppClass *_CAppClassPtr, long int _iPort, int _iNumBytes = 0) : XPCUdpSocket(_iPort),
    CAppClassPtr(_CAppClassPtr) 
    {
        if (_iNumBytes == 0)
            // Allocate the maximum number of bytes the UDP socket can receive.
            sMsgBuf = new char[iGetRecieveBuf()+1];
        else
            // Allocate the user give nubmer of bytes the UDP socket can receive
            sMsgBuf = new char[_iNumBytes+1];
    }

    // The Destructor.  Deletes the memory used by the receive message 
    // buffer.
    ~XPCAsyncUdpSocket() { delete [] sMsgBuf; }

    // Returns the pointer to the main application class
    CAppClass *getCAppClassPtr();

    // The vBindAsyncSocket method binds the socket file descriptor to the selected
    // port and address.
    void vBindAsyncSocket()
    {
        vBindSocket();        // Bind the UDP socket port and address

        // Attach the socket to the CAsyncSocket class and setup incoming data 
        // notification.
        if (Attach(iGetSocketFd(), FD_READ) == 0)
        {
            // If the attach fails, an exception is thrown.
            char sMsg[512];

            sprintf(sMsg, "Error occurred while trying to attach file descriptor: %d\0", GetLastError());
            XPCException socketExcept(sMsg);
            throw socketExcept;
            return;
        }
    }

    // The iSendUdpMessage method calls the inherited iSendMessage method of
    // XPCUdpSocket to send a message to a specific host and socket port number.
    void iSendUdpMessage(void *_vMessage, int _iMsgLen, char *_sHostAddress, int _iPort = 0)
    {
        iSendMessage(_vMessage, _iMsgLen, _sHostAddress, _iPort);
    }

    // The iSendUdpReply method calls the inherited iSendRepy method of
    // XPCUdpSocket to send a message to the host from which a message was 
    // received.
    void iSendUdpReply(void *_vMessage, int _iMsgLen)
    {
        iSendReply(_vMessage, _iMsgLen);
    }

    // The virtual OnReceive method of CAsyncSocket is overridden.  This method is
    // called when data is received on the UDP socket.  
    void OnReceive(int iErr)
    {
        int iNumBytes = 0;    // The total number of bytes received from the socket

        // If an error occurs receiving the message, an exception is thrown.
        if (iErr != 0)
        {
            char sMsg[512];

            sprintf(sMsg, "Error receiving data from socket: %d\0", GetLastError());
            CAppClassPtr->vProcessError(sMsg);
            return;
        }

        // The message buffer is initialized.
        memset(sMsgBuf, 0, sizeof(sMsgBuf));

        // The message is received in the sMsgBuf private data member.
        try
        {
            iNumBytes = iReceiveMessage((void *)sMsgBuf, iGetRecieveBuf());
        }
        catch(XPCException &socketExcept)
        {
            return;
        }

        // If the message buffer is successfully received, the vProcessMessage
        // method of the application class is called and passed sMsgBuf for 
        // application specific processing.
        CAppClassPtr->vProcessMessage(sMsgBuf, iNumBytes);
    }
};

#endif
