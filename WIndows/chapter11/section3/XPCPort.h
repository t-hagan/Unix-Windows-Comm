#ifndef _XPCPort
#define _XPCPort

#include <XPCEndian.h>    // Defines the XPCEndian class

class XPCPort
{
        XPCEndian<long int> iPort;        // The big-endian / little-endian representation of
                                        // the socket port number
public:
        // The constructor.  Stores the port value within the private iPort data member
        XPCPort(long int _iPort = 0) { iPort  = _iPort; }

        // Stores the port value within the private iPort data member
        void vSetPort(long int _iPort) { iPort = _iPort; }

        // Returns the port number in either its original or endian format
        long int iGetOriginalPort()
        {
                return iPort.getOriginalValue();  // Return the port number in its original
                         // format if compiled with the spawned
                         // server.
        }
        long int iGetEndianPort()
        {
                return iPort.getEndianValue(); // Return the port number in its endian
                        // format if compiled with the remote
                        // client.
        }
};

#endif
