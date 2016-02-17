#include <XPCSpawn.h>

XPCSpawn::XPCSpawn(long int _iPort, char *_sServer, char *_sAppInfo, char _cAction)
{
    XPCPort commPort;        // Stores the returned port value for the Spawn Server

    iPort = -1;            // Inialize the value of iPort to -1 or failure
    XPCServerSpawn *newRegister;    // Stores registration information

    // Construct a XPCSpawn object
    XPCSpawnInfo newSpawn(_cAction, _sAppInfo);
    
    // Construct a socket with the Spawn Server's port number
    Socket = new XPCTcpSocket((long int) _iPort);
       
    // Connect to the remote workstation
    Socket->vConnect(_sServer);

    switch(_cAction)
    {
        case SPAWN:
            // If the action is spawn, a XPCSpawnInfo object is sent to the
            // Spawn Server
            Socket->iSendMessage((void *)&newSpawn, sizeof(XPCSpawnInfo));
            break;
        case REGISTER:
            // If the action is register, a XPCServerSpawn object is created and
            // sent to the Spawn Server
#ifdef UNIX
            newRegister = new XPCServerSpawn(_sAppInfo, getpid());
#else
            newRegister = new XPCServerSpawn(_sAppInfo, _getpid());
#endif
            Socket->iSendMessage((void *)newRegister, sizeof(XPCServerSpawn));
            delete [] newRegister;
            break;
        default:
            XPCException exceptOb("Error Constructing XPCServerSpawn:  Invalid Action Code");
            throw exceptOb;
            return;
    }

    // Wait for the Spawn Server to reply
    Socket->iRecieveMessage((void *)&commPort, sizeof(XPCPort), MSG_WAITALL);

    if (_cAction == REGISTER)
        // If the action is REGISTER the original port number format is retrieved
        iPort = commPort.iGetOriginalPort();
    else
        // If the action is SPAWN, the endian value of the port number is retrieved
        iPort = commPort.iGetEndianPort();
    
    // Close the socket
    delete Socket;
}
