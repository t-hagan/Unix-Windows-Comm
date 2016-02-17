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

    fileTransferSocket= _fileTransferSocket;

    if (strcmp (_fileRequest->sGetDirection(), TRANSFER_GET) == 0) 
    {
        iBufSize= fileTransferSocket->iGetSendBuf();
        sBuf = new char[iBufSize];

        if (strcmp (_fileRequest->sGetType(), TRANSFER_BINARY) == 0)
            strcpy (sType, "rb");
        else
            strcpy (sType, "rt");

        FILE* fileIn;
        fileIn= fopen (_fileRequest->sGetFileName(), sType);

        while (!feof (fileIn))
        {
            int iNumRead= fread (sBuf, sizeof (char), iBufSize, fileIn);
            fileTransferSocket->iSendMessage ((void *)sBuf, iNumRead);
        }

        fclose (fileIn);
        delete [] sBuf;
        delete fileTransferSocket;
    }
    else
    {
        fileTransferSocket->vSetLinger (lingerLimit);

        if (strcmp (_fileRequest->sGetType(), TRANSFER_BINARY) == 0)
            strcpy (sType, "wb");
        else
            strcpy (sType, "wt");

        FILE *fileOut= fopen (_fileRequest->sGetFileName(), sType);
        if (!fileOut)
        {
            return;
        }
        int iBufSize = fileTransferSocket->iGetRecieveBuf();
        sBuf = new char[iBufSize];

        while (1)
        {
            int iNumBytes = fileTransferSocket->iRecieveMessage((void *)sBuf, iBufSize);
#ifdef UNIX
            if ((iNumBytes == 0) || (errno == ECONNRESET))
#else
            if ((iNumBytes == 0) || (WSAGetLastError() == WSAECONNRESET))
#endif
            {
                break;
            }
            fwrite ((unsigned char *)sBuf, sizeof (char), iNumBytes, fileOut);
        }
        fclose (fileOut);
        delete [] sBuf;
    }
}


XPCFileTransfer::XPCFileTransfer (char *_sHost)
{ 
    fileTransferSocket = new XPCTcpSocket((long int)FILE_TRANSFER_PORT);
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

    if (strcmp (_sDirection, TRANSFER_GET) == 0)
    {
        fileTransferSocket->vSetLinger (lingerLimit);

        XPCFileRequest fileRequest(_sSourceFileName, _sDirection, _sType);
        fileTransferSocket->iSendMessage((void *)&fileRequest, sizeof(XPCFileRequest));

        if (strcmp (fileRequest.sGetType(), TRANSFER_BINARY) == 0)
            strcpy (sType, "wb");
        else
            strcpy (sType, "wt");
    
        FILE *localFile= fopen (_sDestFileName, sType);
        if (!localFile)
        {
            fileTransferSocket->vCloseSocket();
            XPCException ex("Error creating local file");
            throw ex;
        }

        iBufSize= fileTransferSocket->iGetRecieveBuf();
        sBuf = new char[iBufSize];
        
        while (1)
        {
            int iNumBytes = fileTransferSocket->iRecieveMessage((void *)sBuf, iBufSize);
#ifdef UNIX
            if ((iNumBytes == 0) || (errno == ECONNRESET))
#else
            if ((iNumBytes == 0) || (WSAGetLastError() == WSAECONNRESET))
#endif
            {
                break;
            }

            fwrite ((unsigned char *)sBuf, sizeof (char), iNumBytes, localFile);
        }
        delete [] sBuf;
        fclose (localFile);
    }
    else
    {

        XPCFileRequest fileRequest(_sDestFileName, _sDirection, _sType);
        fileTransferSocket->iSendMessage((void *)&fileRequest, sizeof(XPCFileRequest));

        iBufSize = fileTransferSocket->iGetSendBuf();
        
        sBuf = new char[iBufSize];

        if (strcmp (fileRequest.sGetType(), TRANSFER_BINARY) == 0)
            strcpy (sType, "rb");
        else
            strcpy (sType, "rt");
    
        FILE* fileIn;
        fileIn= fopen (_sSourceFileName, sType);

        while (!feof (fileIn))
        {
            int iNumRead= fread (sBuf, sizeof (char), iBufSize, fileIn);
            fileTransferSocket->iSendMessage ((void *)sBuf, iNumRead);
        }

        delete [] sBuf;
        delete fileTransferSocket;
        fclose (fileIn);
    }
}
