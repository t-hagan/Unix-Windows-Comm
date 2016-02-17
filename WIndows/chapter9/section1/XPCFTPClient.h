#ifndef _XPCFTPClient
#define _XPCFTPClient

#include <XPCTCPsocket.h>
#include <XPCException.h>
#include <fstream.h>

#define FTP_COMMAND_PORT 21
#define FTP_ASCII 0
#define FTP_BINARY 1

class XPCFTPClient 
{
    XPCTcpSocket *Command;
    XPCTcpSocket* ListenSocket();
    char * ErrorString(char * sLeader);
    int iType;

public:
    XPCFTPClient (char *_sHost, int iPort=FTP_COMMAND_PORT);

    void vLogin (char *_sUsername, char *_sPassword);

    void vList (char *_sBuffer, int iSize);

    void vCWD (char *_sDir);

    void vPWD (char *_sBuffer, int iSize);

    void vPutFile (char *_sSourceFile, char *_sTargetFile);

    void vGetFile (char *_sSourceFile, char *_sTargetFile);

    void vAscii();

    void vBinary();
};

#endif
