#ifndef _XPCPubSub
#define _XPCPubSub

#include <XPCTcpSocket.h>
#include <string.h>
#include <XPCEndian.h>
#include <stdlib.h>

// Message type definitions
#define PUBLISH 0
#define SUBSCRIBE 1

#ifdef UNIX
    #define UNIXOP 0
    #define OPSYSTEM UNIXOP
#else
    #define WINDOWS_NTOP 1
    #define OPSYSTEM WINDOWS_NTOP
#endif

class XPCHeader
{
    char cType;                // Defines the message type (PUBLISH or SUBSCRIBE)
    char cOpSystem;            // Defines the operating system
    char sSubscription[256];   // The name of the message
    XPCEndian<int> iNumComponents;    // The number of message components
public:
    // Default constructor.  Sets  the number of components to 0
    XPCHeader() { iNumComponents = (int)0; }

    // Constructor.  Sets the message type, subscription name, and number of components
    XPCHeader(char _cType, char *_sSubscription, int _iNumComponents, char _cOpSystem = OPSYSTEM)
    {
        cOpSystem = _cOpSystem;
        cType = _cType;
        strcpy(sSubscription, _sSubscription);
        iNumComponents = _iNumComponents;
    }

    // Metod to set the message type, subscription name, and number of components
    void vSetValues(char _cType, char *_sSubscription, int _iNumComponents, char _cOpSystem = OPSYSTEM)
    {
        cOpSystem = _cOpSystem;
        cType = _cType;
        strcpy(sSubscription, _sSubscription);
        iNumComponents = _iNumComponents;
    }

    // Returns the number of components
    int iGetNumComponents(char _cOpSystem = OPSYSTEM)
    { 
        if (_cOpSystem != cOpSystem)
            return iNumComponents.getEndianValue();
        else
            return iNumComponents.getOriginalValue();
    }

    // Returns the subscription name
    char *sGetSubscription() { return sSubscription; }

    // Returns the message type
    char cGetType() { return cType; }
};

#endif
