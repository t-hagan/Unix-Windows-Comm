#include <XPCFileTransfer.h>


XPCFileTransfer::XPCFileTransfer (XPCTcpSocket *_fileTransferSocket,
                    XPCFileRequest *_fileRequest)
{
    char *sBuf;
    char sType[10];
    int iBufSize;
    struct linger lingerLimit;

    lingerLimit.l_onoff= 1;
    lingerLimit.l_linger= 5;

    fileTransferSocket = _fileTransferSocket;

    // determine the direction of the file request
    // handle get requests here
    if (strcmp (_fileRequest->sGetDirection(), TRANSFER_GET) == 0) 
    {
        fileTransferSocket->vSetLinger (lingerLimit);
        // get the size of the i/o buffer
        iBufSize= fileTransferSocket->iGetSendBuf();
        sBuf = new char[iBufSize];

        // map the file type to the fopen type
        if (strcmp (_fileRequest->sGetType(), TRANSFER_BINARY) == 0)
            strcpy (sType, "rb");
        else
            strcpy (sType, "rt");

        // open the file
        FILE* fileIn= fopen (_fileRequest->sGetFileName(), sType);

        // read the file until eof
        while (!feof (fileIn))
        {
            int iNumRead= fread (sBuf, 
                            sizeof (char), 
                            iBufSize, 
                            fileIn);
            // send the file data to the client
            fileTransferSocket->iSendMessage ((void *)sBuf, 
                                            iNumRead);
        }

        // clean up
        fclose (fileIn);
        delete fileTransferSocket;
        delete [] sBuf;
    }
    else
    // handle put requests here
    {
        // map the file type to the fopen type
        if (strcmp (_fileRequest->sGetType(), TRANSFER_BINARY) == 0)
            strcpy (sType, "wb");
        else
            strcpy (sType, "wt");

        // open the file
        FILE *fileOut= fopen (_fileRequest->sGetFileName(), sType);
        if (!fileOut)
        {
            return;
        }
        // create a buffer the size of the socket's i/o buffer
        int iBufSize = _fileTransferSocket->iGetRecieveBuf();
        sBuf = new char[iBufSize];

        // loop until all data is received
        while (1)
        {
            // receive a buffer from the client
            int iNumBytes = 
                        _fileTransferSocket->iRecieveMessage(
                                                (void *)sBuf, 
                                                iBufSize);
#ifdef UNIX
            if ((iNumBytes == 0) || (errno == ECONNRESET))
#else
            if ((iNumBytes == 0) || 
                        (WSAGetLastError() == WSAECONNRESET))
#endif
            {
                break; // if no data then done
            }
            // write data to the target file
            fwrite ((unsigned char *)sBuf, 
                        sizeof (char), 
                        iNumBytes, 
                        fileOut);
        }

        // clean up
        delete [] sBuf;
        fclose (fileOut);
    }
}

XPCFileTransfer::XPCFileTransfer (char *_sHost)
{
    // create the socket
    fileTransferSocket = 
            new XPCTcpSocket((long int)FILE_TRANSFER_PORT);

    // connect to the host
    fileTransferSocket->vConnect(_sHost);

}


void XPCFileTransfer::vTransferFile (char *_sSourceFileName, 
                    char *_sDestFileName,
                    char *_sDirection,
                    char *_sType)
{
    char *sBuf;
    char sType[10];
    int iBufSize;
    struct linger lingerLimit;

    lingerLimit.l_onoff= 1;
    lingerLimit.l_linger= 5;


    // handle get file here
    if (strcmp (_sDirection, TRANSFER_GET) == 0)
    {

        // create the file request object for the get operation
        XPCFileRequest fileRequest(_sSourceFileName, 
                        _sDirection, 
                        _sType);

        // send the object to the server
        fileTransferSocket->iSendMessage((void *)&fileRequest,
                            sizeof(XPCFileRequest));

        // translate the file type to type code for fopen
        if (strcmp (fileRequest.sGetType(), TRANSFER_BINARY) == 0)
            strcpy (sType, "wb");
        else
            strcpy (sType, "wt");
    
        // open the file
        FILE *localFile= fopen (_sDestFileName, sType);
        if (!localFile)
        {
            fileTransferSocket->vCloseSocket();
            XPCException ex("Error creating local file");
            throw ex;
        }

        // get the i/o buffer size 
        iBufSize= fileTransferSocket->iGetRecieveBuf();
        // and create a buffer to match
        sBuf = new char[iBufSize];
        
        while (1)
        {
            // get the data from the server
            int iNumBytes = 
                    fileTransferSocket->iRecieveMessage((void *)sBuf, 
                                                    iBufSize);
            // is there data?
#ifdef UNIX
            if ((iNumBytes == 0) || (errno == ECONNRESET))
#else
            if ((iNumBytes == 0) || 
                            (WSAGetLastError() == WSAECONNRESET))
#endif
            {
                break; // no, get out
            }

            // yes, save it to file
            fwrite ((unsigned char *)sBuf, 
                        sizeof (char), 
                        iNumBytes, 
                        localFile);
        }

        // clean up
        delete [] sBuf;
        fclose (localFile);
    }
    // handle put file here
    else
    {
        fileTransferSocket->vSetLinger (lingerLimit);
        // create the file request object for the put operation
        XPCFileRequest fileRequest(_sDestFileName, 
                            _sDirection, 
                            _sType);

        // send the object to the server
        fileTransferSocket->iSendMessage((void *)&fileRequest, 
                                    sizeof(XPCFileRequest));

        // get the i/o buffer size
        iBufSize = fileTransferSocket->iGetSendBuf();

        // and create a buffer to match
        sBuf = new char[iBufSize];

        // map the file type to the fopen code
        if (strcmp (fileRequest.sGetType(), TRANSFER_BINARY) == 0)
            strcpy (sType, "rb");
        else
            strcpy (sType, "rt");
    
        // open the file
        FILE* fileIn= fopen (_sSourceFileName, sType);

        // read the file till eof
        while (!feof (fileIn))
        {
            // read a buffer
            int iNumRead= fread (sBuf, 
                            sizeof (char), 
                            iBufSize, 
                            fileIn);

            // send the buffer to the server
            fileTransferSocket->iSendMessage ((void *)sBuf,
                                                iNumRead);
        }

        // clean up
        delete [] sBuf;
        delete fileTransferSocket;
        fclose (fileIn);
    }
}
