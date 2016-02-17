#ifndef _XPCFileTransfer
#define _XPCFileTransfer

#include <XPCTcpSocket.h>
#include <XPCFileRequest.h>
#include <XPCEndian.h>
#include <fstream.h>

#define FILE_TRANSFER_PORT 6201
#define TRANSFER_TEXT "T"
#define TRANSFER_BINARY "B"
#define TRANSFER_GET "G"
#define TRANSFER_PUT "P"

class XPCFileTransfer
{
    XPCTcpSocket *fileTransferSocket;
public:
    XPCFileTransfer (char *_sHost);

    void vTransferFile (char *_sSourceFileName, 
            char *_sDestFileName, 
            char *_sDirection = TRANSFER_GET,
            char *_sType = TRANSFER_TEXT);

    XPCFileTransfer (XPCTcpSocket *_fileTransferSocket,
        XPCFileRequest *_fileRequest);

    virtual ~XPCFileTransfer() { }
};

#endif
