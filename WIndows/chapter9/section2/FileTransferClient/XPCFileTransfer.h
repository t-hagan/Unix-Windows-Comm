#ifndef _XPCFileTransfer
#define _XPCFileTransfer

#include <XPCTcpSocket.h>
#include <XPCFileRequest.h>
#include <XPCEndian.h>
#include <fstream.h>

// default port if not specified
#define FILE_TRANSFER_PORT 6201

// transfer types
#define TRANSFER_TEXT "T"
#define TRANSFER_BINARY "B"

// transfer directions
#define TRANSFER_GET "G"
#define TRANSFER_PUT "P"

class XPCFileTransfer
{
    XPCTcpSocket *fileTransferSocket;

public:
    // client constructor
    XPCFileTransfer (char *_sHost);
    
    // client file transfer function
    void vTransferFile (char *_sDestFileName, 
            char *_sSourceFileName, 
            char *_sDirection = TRANSFER_GET,
            char *_sType = TRANSFER_TEXT);

    // server constructor
    XPCFileTransfer (XPCTcpSocket *_fileTransferSocket,
        XPCFileRequest *_fileRequest);

    // destructor
    virtual ~XPCFileTransfer() { }
};

#endif
        
