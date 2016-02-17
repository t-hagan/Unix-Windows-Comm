#include <CSpawnClass.h>
#ifdef WINDOWS_NT
    #include <Process.h>
#endif

CSpawnClass::CSpawnClass(long int _iPort, char *_sServer, char *_sAppInfo, char _cAction)
{
    CPort commPort;                // Stores the returned port value for the Spawn Server

    iPort = -1;                    // Inialize the value of iPort to -1 or failure
    CServerSpawn *newRegister;    // Stores registration information

    // Construct a cSpawn object
    CSpawn newSpawn(_cAction, _sAppInfo);
    
    // Construct a socket with the Spawn Server's port number
    Socket = new CTcpSocketClass((long int) _iPort);
       
    // Connect to the remote workstation
    Socket->vConnect(_sServer);

    switch(_cAction)
    {
        case SPAWN:
            // If the action is spawn, a CSpawn object is sent to the
            // Spawn Server
            Socket->iSendMessage((void *)&newSpawn, sizeof(CSpawn));
            break;
        case REGISTER:
            // If the action is register, a CServerSpawn object is created and
            // sent to the Spawn Server
#ifdef UNIX
            newRegister = new CServerSpawn(_sAppInfo, getpid());
#else
            newRegister = new CServerSpawn(_sAppInfo, _getpid());
#endif
            Socket->iSendMessage((void *)newRegister, sizeof(CServerSpawn));
            delete [] newRegister;
            break;
        default:
            IPCExceptionClass exceptOb("Error Constructing CServerSpawn:  Invalid Action Code");
            throw exceptOb;
            return;
    }

    // Wait for the Spawn Server to reply
    Socket->iRecieveMessage((void *)&commPort, sizeof(CPort), MSG_WAITALL);

    if (_cAction == REGISTER)
        // If the action is REGISTER the original port number format is retrieved
        iPort = commPort.iGetOriginalPort();
    else
        // If the action is SPAWN, the endian value of the port number is retrieved
        iPort = commPort.iGetEndianPort();
    
    // Close the socket
    delete Socket;
}
