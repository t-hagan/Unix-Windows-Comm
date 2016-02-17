#ifndef _XPCFileTransferClient
#define _XPCFileTransferClient

#include <XPCTcpSocket.h>
#include <XPCFileRequest.h>
#include <XPCEndian.h>
#include <fstream.h>

#define FILE_TRANSFER_PORT 6201
#define TRANSFER_TEXT "T"
#define TRANSFER_BINARY "B"
#define TRANSFER_GET "G"
#define TRANSFER_PUT "P"

class XPCFileTransferClient
{
    XPCTcpSocket *fileTransferSocket;

public:
    XPCFileTransferClient(char *_sHost, 
            char *_sSourceFileName, 
            char *_sDestFileName, 
            char *_sDirection = TRANSFER_GET,
            char *_sType = TRANSFER_TEXT);
    virtual ~XPCFileTransferClient () 
    {
        delete fileTransferSocket;
    }
};

#endif
        
