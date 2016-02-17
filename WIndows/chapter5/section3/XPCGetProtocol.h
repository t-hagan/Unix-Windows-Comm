#ifndef _XPCGetProtocol
#define _XPCGetprotocol

#ifdef UNIX
    #include <netdb.h>
#else
    #include <winsock2.h>
#endif

#include <XPCException.h>

class XPCGetProtocol
{
#ifdef UNIX
    char cIteratorFlag;        // Protocol database iteration flag
#endif
    struct protoent *protocolPtr;    // Pointer to protocol database entry
public:
#ifdef UNIX
    // Default constructor.  Opens the protocol database
    XPCGetProtocol()
    {
        vOpenProtocolDb();
    }
#endif

    // Constructor.  Returns the protoent structure given the protocol name.
    XPCGetProtocol(const char *_sName);

    // Constructor.  Returns the protoent structure given the protocol number
    XPCGetProtocol(int _iProtocol);

    // Desstructor closes the database connection  
        ~XPCGetProtocol()
        {
#ifdef UNIX
                endprotoent();
#endif
        }

    // Opens the protocol database and sets the cIteratorFlag to true
#ifdef UNIX
    void vOpenProtocolDb()
    {
        endprotoent();
        cIteratorFlag = 1;
        setprotoent(1);
    }    

    // Iterates through the list of protocols
    char cGetNextProtocol()
    {
        if (cIteratorFlag == 1)
        {
            if ((protocolPtr = getprotoent()) == NULL)
                return 0;
            else
                return 1;
        }
        return 0;
    } 
#endif

    // Returns the protocol name
    char *sGetProtocolName() { return protocolPtr->p_name; }

    // Returns the protcol number
    int iGetProtocolNumber() { return protocolPtr->p_proto; }
};

#endif
