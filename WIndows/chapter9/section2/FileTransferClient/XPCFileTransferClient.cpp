#include <XPCFileTransferClient.h>


XPCFileTransferClient::XPCFileTransferClient (char *_sHost, 
                                          char *_sSourceFileName, 
                                          char *_sDestFileName,
                                          char *_sDirection,
                                          char *_sType)
{
    char *sBuf;
    char sType[10];
    int iBufSize;

    fileTransferSocket = new XPCTcpSocket((long int)FILE_TRANSFER_PORT);
    fileTransferSocket->vConnect(_sHost);

    if (strcmp (_sDirection, TRANSFER_GET) == 0)
    {
        XPCFileRequest fileRequest(_sSourceFileName, _sDirection, _sType);
        fileTransferSocket->iSendMessage((void *)&fileRequest, sizeof(XPCFileRequest));

        if (strcmp (_sType, TRANSFER_BINARY) == 0)
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

        if (strcmp (_sType, TRANSFER_BINARY) == 0)
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

        fclose (fileIn);
        delete [] sBuf;
    }
}
