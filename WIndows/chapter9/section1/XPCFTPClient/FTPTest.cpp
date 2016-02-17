#include "XPCFTPClient.h"
#include <iostream.h>
#include <XPCException.h>

main(int argc, char *argv[])
{
    WSADATA wsaData;
    XPCFTPClient *FileTrans;
    char sBuf[4086];


    if (WSAStartup(0x101, &wsaData) != 0)
    {
        cout << "Error initializing WinSock library: " << WSAGetLastError() << endl;
        return 0;
    }

    try
    {
        FileTrans= new XPCFTPClient ("servername");

        FileTrans->vLogin ("username", "password");

        FileTrans->vCWD ("/home/user");

        FileTrans->vPWD (sBuf, sizeof sBuf);

        cout << sBuf << endl;

        FileTrans->vList (sBuf, sizeof sBuf);

        FileTrans->vBinary();

        FileTrans->vPutFile ("H:\\data\\memos\\bobs.doc", "bobs.doc");
        FileTrans->vBinary();

        FileTrans->vGetFile ("bobs.doc", "c:\\temp\\bobs.doc");

        FileTrans->vList (sBuf, sizeof sBuf);

        cout << sBuf << endl;

        FileTrans->vPWD (sBuf, sizeof sBuf);
        FileTrans->vCWD ("/home/user/docs");
        FileTrans->vPWD (sBuf, sizeof sBuf);
        delete FileTrans;
    }
    catch(XPCException &exceptOb)
    {
        cout << "Ftp Error: " << exceptOb.sGetException() << endl;
        return 0;
    }

    return 1;
}
