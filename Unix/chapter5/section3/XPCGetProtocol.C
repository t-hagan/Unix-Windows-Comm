#include <XPCGetProtocol.h>

XPCGetProtocol::XPCGetProtocol(const char *_sName)
{
#ifdef UNIX
       cIteratorFlag = 0;
#endif
       // Retrieves the protocol structure by name
       protocolPtr = getprotobyname(_sName);
       if (protocolPtr == NULL)
       {
             XPCException exceptObject("Could Not Get Protocol By Name");
             throw exceptObject;
             return;
        }
}

XPCGetProtocol::XPCGetProtocol(int _iProtocol)
{
#ifdef UNIX
        cIteratorFlag = 0;
#endif
        // Retrieves the protocol structure by number
        protocolPtr = getprotobynumber(_iProtocol);
        if (protocolPtr == NULL)
        {
              XPCException exceptObject("Could Not Get Protocol By Number");
              throw exceptObject;
              return;
        }
}
