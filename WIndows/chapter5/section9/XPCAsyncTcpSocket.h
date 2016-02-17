#ifndef _XPCAsyncTcpSocket
#define _XPCAsyncTcpSocket

#include <XPCTcpSocket.h>		// XPCTcpSocket Definition
#include <afxsock.h>			// CAsyncSocket Definition 
#include <XPCException.h>	// XPCException Definition

template <class CAppClass> class XPCAsyncTcpSocket : public XPCTcpSocket, public CAsyncSocket
{
	CAppClass *CAppClassPtr;	// A pointer to the Application class used to define 
								// the instance of XPCAsynchTcpSocket
	char *sMsgBuf;				// A buffer that stores a received socket message.
	int iOption;				// Message receive option
	int iMaxMsgSize;			// Maximum size of message
public:
	// A constructor.  Takes a pointer to the application class and the socket port 
// that is communicated on.  A TCP socket is created and the receive message 
// buffer is allocated the maximum number of bytes the TCP socket can receive.
	XPCAsyncTcpSocket(CAppClass *_CAppClassPtr, long int _iPort, int _iNumBytes = 0, int _iOption = 0) : XPCTcpSocket(_iPort),
	CAppClassPtr(_CAppClassPtr) 
	{
		iOption = _iOption;
		if (_iNumBytes == 0)
		{
			// Allocate the maximum number of bytes the TCP socket can receive to the private
			// message buffer.
			iMaxMsgSize = iGetRecieveBuf();
			sMsgBuf = new char[iMaxMsgSize+1];
		}
		else
		{
			// Allocate the user-specified number of bytes the TCP socket can receive to the private
			// message buffer.
			iMaxMsgSize = _iNumBytes;
			sMsgBuf = new char[_iNumBytes + 1];
		}
	}

	// A constructor. Takes a pointer to the application class and a new socket file 
	// descriptor.  This constructor is called when a client connects to the 
	// XPCAsynchTcpSocket acting as a server.  
	XPCAsyncTcpSocket(CAppClass *_CAppClassPtr, short int _iSocket, int _iNumBytes = 0, int _iOption = 0) : XPCTcpSocket(_iSocket), CAppClassPtr(_CAppClassPtr) 
	{
		iOption = _iOption;
		if (_iNumBytes == 0)
		{
			// Allocate the maximum number of bytes the TCP socket can receive to the private
			// message buffer.
			iMaxMsgSize = iGetRecieveBuf();
			sMsgBuf = new char[iMaxMsgSize+1];
		}
		else
		{
			// Allocate the user-specified number of bytes the TCP socket can receive to the private
			// message buffer.
			iMaxMsgSize = _iNumBytes;
			sMsgBuf = new char[_iNumBytes + 1];
		}
	}


	// The Destructor.  Deletes the memory used by the temporary receive message 
	// buffer.
	~XPCAsyncTcpSocket() { delete [] sMsgBuf; }


	// The vAttachSocket method attaches the socket file descriptor to the
	// CAsyncSocket class.  The socket file descriptor is setup to receive 
	// asynchronous notification if data is received (TCP Client or Server), the
	// socket is closed (TCP Server), the server accepts a client connection (TCP 
	// Server), and a client attempts to connect (TCP Client).
	void vAttachSocket()
	{
		if (Attach(iGetSocketFd(), FD_READ | FD_CLOSE | FD_ACCEPT | FD_CONNECT) == 0)
		{
			// An exception is thrown if the socket fails to attach.
			char sMsg[512];

			sprintf(sMsg, "Error occurred while trying to attach file descriptor: %d\0", GetLastError());
			XPCException socketExcept(sMsg);
			throw socketExcept;
			return;
		}
	}

	// The vConnect() method of XPCTcpSocket is overridden.  The new version of 
     // vConnect() check for WSAWOULDBLOCK before throwing an exception
     void vConnect(char *_sServerAddress)
     {
          sockaddr_in serverAddress;    // Stores the address and port number of the TCP 
                                        // socket server

          // An instance of XPCGetHostInfo is created with the value stored in the 
          // sServerAddress parameter.  The IP address retrieved from XPCGetHostInfo
          // is used along with the socket port number (stored in the inherited iPort
          // member variable) to create an instance of the sockaddr_in structure.  If 
          // XPCGetHostInfo fails to resolve the host, an exception is thrown.
          try
          {
               XPCGetHostInfo getHostInfo(_sServerAddress, NAME);

               serverAddress.sin_family = AF_INET;
               serverAddress.sin_addr.s_addr = inet_addr(getHostInfo.sGetHostAddress());
               serverAddress.sin_port = htons(iPort);
          }
          catch(XPCException &exceptObject)
          {
               char sMsg[512];

               sprintf(sMsg, "Error Connecting.  Error resolving hostname %s: %s\0", _sSendToHost, exceptObject.sGetException());
               XPCException socketExcept(sMsg);
               throw socketExcept;
               return;
          }

          // The socket is connected to the server using the connect system call.  If the 
          // connection fails an exception is thrown.
          if (connect(iSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) == -1)
          {
               // If connect returns a warning that blocking occurs return without error.
               if  (WSAGetLastError() == WSAEWOULDBLOCK)
                   return;
              char sMsg[512];

              sprintf(sMsg, "Error Connecting To Socket. %d\0", sGetError());
              XPCException socketExcept(sMsg);
              throw socketExcept;
              return;
          }
     }

	// The OnConnect method is called when the TCP socket either connects to a server 
	// or the socket fails connection.  If the connection fails, the 
	// vProcessConnection method of the application class is sent a NULL pointer to 
	// the server's address.  Otherwise the vProcessConnection method is sent the 
	// address of the connected host packed in a sockaddr_in structure.
	void OnConnect(int iErr)
	{
		sockaddr_in socketAddress;	// Stores the address of the host connected 
		int socketAddressLen;		// The size of the sockaddr_in structure.

		// If the connection fails a NULL address pointer is sent to 
		// vProcessConnection.
		if (iErr != 0)
		{
			
			CAppClassPtr->vProcessConnection((sockaddr_in *)NULL);
			return;
		}
		else
		{
			// The length of the socket address structure is stored.
			socketAddressLen = sizeof(socketAddress);

			// The address of the connected host is retrieved and sent to the 
			// vProcessConnection method of the application class.
			GetPeerName((SOCKADDR *)&socketAddress, &socketAddressLen);
			CAppClassPtr->vProcessConnection(&socketAddress);
		}
	}
	
	// The OnAccept method is called when a TCP client connects.  If no error occurs 
	// connecting, the vAccept method of CTcpSocket is called. The vAccept method 
	// returns a new socket file descriptor.  This socket file descriptor is used to 
	// create a new instance of CAsynchTcpSocket that communicates with the 
	// client.  The new CAsynchTcpSocket is setup to asynchronously handle data 
	// retrieval, data sending, and TCP connection shutdowns.
	void OnAccept(int iErr)
	{
		// If an error occurs accepting a client connection a message box is 
		// displayed an the method exits.
		if (iErr != 0)
		{
			char sMsg[512];

			sprintf(sMsg, "Error accepting data from socket: %d\0", GetLastError());
			CAppClassPtr->vProcessError(sMsg);
			return;
		}

		// The client connection is accepted and a new TCP socket file descriptor 
		// is returned.
		XPCTcpSocket *newSocket = XPCTcpSocket::Accept();

		// A new instance of the XPCAsyncTcpSocket class is created using the new 
		// TCP socket file descriptor.
		XPCAsyncTcpSocket<CAppClass> *newSocketConnection = new XPCAsyncTcpSocket<CAppClass>(CAppClassPtr, (short int)newSocket->iGetSocketFd(), iMaxMsgSize, iOption);

		// The new socket file descriptor is setup for asynchronous notification.
		newSocketConnection->Attach(newSocket->iGetSocketFd(), FD_READ | FD_WRITE | FD_CLOSE);
	}

	// The OnReceive method is called when data is available for retrieval on the 
	// socket.  The iReceiveMessage method stores the message in the sMsgBuf private 
	// member variable.  The vProcessMessage method of the main application class is 
	// called and passed the message buffer.
	void OnReceive(int iErr)
	{
		int iNumBytes = 0;	// The number of bytes recieved from the socket

		// If data retrieval fails, a message box is displayed detailing the error 
		// and the method exits.
		if (iErr != 0)
		{
			char sMsg[512];

			CAppClassPtr->vProcessError(sMsg);
			return;
		}

		try
		{
			// The message is received into the message buffer.
			iNumBytes = iRecieveMessage((void *)sMsgBuf, iMaxMsgSize, iOption);
		}
		catch(XPCException &exceptOb)
		{
			CAppClassPtr->vProcessError(exceptOb.sGetException());
			return;
		}

		// The vProcessMessage method of the main application class is called and 
		// passed the message buffer for application specific processing.
		CAppClassPtr->vProcessMessage(sMsgBuf, iNumBytes);
	}

	// The OnSend method is called when a the iSendMessage method of XPCAsyncSocketClass 
	// has been executed and message is too large to send at once.  This method calls 
	// the main application class sGetMessageBuffer method to retrieve the message 
	// buffer to send.  The method loops sending the maximum number of bytes allowed 
	// by the TCP socket protocol until all data has been sent. 
	void OnSend(int iErr)
	{
		// If an error occurs sending a message, a message box is displayed and
		// the method exits.
		if (iErr != 0)
		{
			char sMsg[512];

			sprintf(sMsg, "Error sending data: %d\0", GetLastError());
			CAppClassPtr->vProcessError(sMsg);
			return;
		}

		// The sGetMessageBuffer method of the main application class retrieves 
		// the buffer to send and its size.
		int iSendBufSize;
		char *sSendBuf = CAppClassPtr->sGetMessageBuffer(iSendBufSize);

		// The iSendMessage method is used to send the message buffer.
		iSendMessage(sSendBuf, iSendBufSize);
	}

	// The OnClose method is called when the TCP client disconnects from the server. 
	// If no errors occur, the vProcessClose method of the main application class is 
	// called for application specific processing.
	void OnClose(int iErr)
	{
		sockaddr_in socketAddress;	// Stores the address of the host connected 
									// too.
		int socketAddressLen;		// The size of the sockaddr_in structure.

		// If an error occurred with the client disconnecting a message box 
		// displays the error and the method returns.
		if (iErr != 0)
		{
			char sMsg[512];

			sprintf(sMsg, "Error closing socket: %d\0", GetLastError());
			CAppClassPtr->vProcessError(sMsg);
			return;
		}

		// The length of the socket address structure is stored.
		socketAddressLen = sizeof(socketAddress);

		// The address of the connected host is retrieved and sent to the 
		// vProcessConnection method of the application class.
		GetPeerName((SOCKADDR *)&socketAddress, &socketAddressLen);

		// The vProcessClose method of the main application class is called with 
		// the disconnected client's address.
		CAppClassPtr->vProcessClose(&socketAddress);
		delete this;
	}
};

#endif