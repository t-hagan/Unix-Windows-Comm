#include "XPCFTPClient.h"

#define  ToUpper(b)  (((int)b)&0xff)

XPCFTPClient::XPCFTPClient(char *_sHost, int iPort)
{
    char sMessage[256];
    int nRet;

    Command = new XPCTcpSocket ((long int)iPort);
    Command->vConnect (_sHost);

    if (!(nRet= Command->iRecieveMessage (sMessage, sizeof(sMessage))))
    {
        XPCException ex ("Error receiving connection confirmation");
        throw ex;
        return;
    }
    iType= FTP_ASCII;
}

void XPCFTPClient::vLogin(char *_sUsername, char *_sPassword)
{
    char sMessage[256];
    int nRet;

    sprintf(sMessage, "USER %s\r\n", _sUsername); 
    if (!Command->iSendMessage(sMessage, strlen(sMessage)))
    {
        XPCException ex("Error sending user name");
        throw ex;
    }

    if (!(nRet= Command->iRecieveMessage (sMessage, sizeof(sMessage))))
    {
        XPCException ex("Error receiving username confirmation");
        throw ex;
    }

    sprintf(sMessage, "PASS %s\r\n", _sPassword); 
    if (!Command->iSendMessage(sMessage, strlen(sMessage)))
    {
        XPCException ex("Error sending password");
        throw ex;
    }

    if (!(nRet= Command->iRecieveMessage (sMessage, sizeof(sMessage))))
    {
        XPCException ex("Error receiving password confirmation");
        throw ex;
    }
}

void XPCFTPClient::vPutFile (char *_sSourceFile, char *_sTargetFile)
{
    char sMessage[2048];
    char sType[10];
    int nRet;
    sprintf(sMessage, "STOR %s\r\n", _sTargetFile);

    // get the listen socket before the command is sent
    XPCTcpSocket* ListenSock= ListenSocket();

    if (!Command->iSendMessage(sMessage, strlen(sMessage)))
    {
        XPCException ex("Error sending STOR command");
        throw ex;
    }

    if (!(nRet= Command->iRecieveMessage (sMessage, sizeof(sMessage))))
    {
        XPCException ex("Error receiving STOR command");
        throw ex;
    }

    XPCTcpSocket *DataSock= ListenSock->Accept();

    if (iType == FTP_BINARY)
        strcpy (sType, "rb");
    else
        strcpy (sType, "rt");

    FILE* hFile= fopen (_sSourceFile, sType);

    if (!hFile)
    {
        XPCException ex (ErrorString ("Error opening source file"));
        throw ex;
    }
    

    while (!feof (hFile))
    {
        int iNumRead= fread (sMessage, sizeof (char), 2048, hFile);//sizeof (sMessage), hFile);
        if (!DataSock->iSendMessage ((void *)sMessage, iNumRead))
        {
            XPCException ex("Error sending file data");
            throw ex;
        }
    }

    fclose (hFile);
    delete DataSock;
    delete ListenSock;
}

void XPCFTPClient::vGetFile (char *_sSourceFile, char *_sTargetFile)
{
    char sMessage[2048];
    char sType[10];
    int nRet;
    if (iType == FTP_BINARY)
        strcpy (sType, "wb");
    else
        strcpy (sType, "wt");

    FILE *hFile= fopen (_sTargetFile, sType);
    if (!hFile)
    {
        XPCException ex (ErrorString ("Error creating hTarget file"));
        throw ex;
    }


    sprintf(sMessage, "RETR %s\r\n", _sSourceFile);

    // get the listen socket before the command is sent
    XPCTcpSocket* ListenSock= ListenSocket();

    if (!Command->iSendMessage(sMessage, strlen(sMessage)))
    {
        XPCException ex("Error sending RETR command");
        throw ex;
    }

    if (!(nRet= Command->iRecieveMessage (sMessage, sizeof(sMessage))))
    {
        XPCException ex("Error receiving RETR command");
        throw ex;
    }
    XPCTcpSocket *DataSock= ListenSock->Accept();

    while (1)
    {
        int iNumBytes = DataSock->iRecieveMessage((void *)sMessage, 
                                                    sizeof (sMessage));
#ifdef UNIX
        if ((iNumBytes == 0) || (errno == ECONNRESET))
#else
        if ((iNumBytes == 0) || (WSAGetLastError() == WSAECONNRESET))
#endif
        {
            break;
        }
        fwrite ((unsigned char *)sMessage, sizeof (char), iNumBytes, hFile);
    }

    fclose (hFile);
    delete DataSock;
    delete ListenSock;
}

void XPCFTPClient::vList (char *_sBuffer, int iSize)
{
    char sMessage[256];
    int nRet;

    // get the listen sicket before the command is sent
    XPCTcpSocket* ListenSock= ListenSocket();

    strcpy(sMessage, "NLST\r\n");

    if (!Command->iSendMessage(sMessage, strlen(sMessage)))
    {
        XPCException ex("Error sending LIST command");
        throw ex;
    }

    if (!(nRet= Command->iRecieveMessage (sMessage, sizeof(sMessage))))
    {
        XPCException ex("Error receiving LIST command");
        throw ex;
    }
    XPCTcpSocket *DataSock= ListenSock->Accept();

    int nNumRead;

    for (nRet= 0; nRet < iSize; nRet+= nNumRead)
    {
        nNumRead= DataSock->iRecieveMessage (&_sBuffer[nRet], iSize - nRet);
        if (nNumRead == 0)
            break;
    }

    _sBuffer[nRet-1]= '\0';

    delete ListenSock;
    delete DataSock;
}


void XPCFTPClient::vPWD (char *_sBuffer, int iSize)
{
    char sMessage[256];
    int nRet;

    strcpy(sMessage, "PWD\r\n");

    if (!Command->iSendMessage(sMessage, strlen(sMessage)))
    {
        XPCException ex("Error sending PWD command");
        throw ex;
    }

    if (!(nRet= Command->iRecieveMessage (_sBuffer, iSize)))
    {
        XPCException ex("Error receiving PWD command");
        throw ex;
    }
    _sBuffer[nRet-1]= '\0';
}

void XPCFTPClient::vCWD (char *_sDir)
{
    char sMessage[256];
    int nRet;

    sprintf(sMessage, "CWD %s\r\n", _sDir);

    if (!Command->iSendMessage(sMessage, strlen(sMessage)))
    {
        XPCException ex("Error sending CWD command");
        throw ex;
    }

    if (!(nRet= Command->iRecieveMessage (sMessage, sizeof(sMessage))))
    {
        XPCException ex("Error receiving CWD command");
        throw ex;
    }
}

void XPCFTPClient::vAscii ()
{
    char sMessage[256];
    int nRet;

    sprintf(sMessage, "TYPE A\r\n");

    if (!Command->iSendMessage(sMessage, strlen(sMessage)))
    {
        XPCException ex("Error sending Ascii command");
        throw ex;
    }

    if (!(nRet= Command->iRecieveMessage (sMessage, sizeof(sMessage))))
    {
        XPCException ex("Error receiving Ascii command");
        throw ex;
    }
    iType= FTP_ASCII;
}



void XPCFTPClient::vBinary ()
{
    char sMessage[256];
    int nRet;

    sprintf(sMessage, "TYPE I\r\n");

    if (!Command->iSendMessage(sMessage, strlen(sMessage)))
    {
        XPCException ex("Error sending Binary command");
        throw ex;
    }

    if (!(nRet= Command->iRecieveMessage (sMessage, sizeof(sMessage))))
    {
        XPCException ex("Error receiving Binary command");
        throw ex;
    }
    iType= FTP_BINARY;
}



XPCTcpSocket* XPCFTPClient::ListenSocket()
{
    char sMessage[256]; 
    int nRet;
    struct sockaddr_in  ListenAddr, CommandAddr;
    char *sDataPort,*sIPAddr;
    XPCTcpSocket* ListenSock;

    ListenSock= new XPCTcpSocket((long int)0);
    ListenSock->vBindSocket();

    int nLen= sizeof (ListenAddr);

    getsockname (ListenSock->iGetSocketFd(), (struct sockaddr *)&ListenAddr, &nLen);
    
    sDataPort= (char *)&ListenAddr.sin_port;

    nLen= sizeof(CommandAddr);

    getsockname (Command->iGetSocketFd(), (struct sockaddr *)&CommandAddr, &nLen);

    sIPAddr= (char *)&CommandAddr.sin_addr;


    sprintf (sMessage, "PORT %d,%d,%d,%d,%d,%d\r\n",
        ToUpper(sIPAddr[0]), ToUpper(sIPAddr[1]), ToUpper(sIPAddr[2]), ToUpper(sIPAddr[3]),
        ToUpper(sDataPort[0]), ToUpper(sDataPort[1]));

    if (!Command->iSendMessage (sMessage, strlen(sMessage)))
    {
        XPCException ex("Error PORT command");
        throw ex;
    }

    if (!(nRet= Command->iRecieveMessage (sMessage, sizeof(sMessage))))
    {
        XPCException ex("Error receiving PORT command");
        throw ex;
    }

    ListenSock->vListen(1);
    return ListenSock;
}


char * XPCFTPClient::ErrorString(char * sLeader)
{
    static char buf[255];
    LPVOID lpMsgBuf;
    UINT ErrNo;

    FormatMessage (
            FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
            NULL,
            ErrNo=GetLastError (),
            MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
            (LPTSTR) &lpMsgBuf,
            0,
            NULL);

    wsprintf (buf, "%s: %s", sLeader, (LPTSTR)lpMsgBuf);

    LocalFree (lpMsgBuf);
    return buf;
}

