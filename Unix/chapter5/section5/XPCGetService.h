#ifndef _XPCGetService
#define _XPCGetService

#include <XPCGetProtocol.h>
#include <XPCException.h>
#ifdef UNIX
    #include <netdb.h>
#else
    #include <winsock2.h>
#endif

class XPCGetService
{
#ifdef UNIX
    char cIteratorFlag;        // Iterator indicator
#endif
    struct servent *servicePtr;    // Pointer to a service structure
public:
#ifdef UNIX
    // Default constructor.  Opens the services database
    XPCGetService()
    {
        vOpenServiceDb();
    }
#endif
    // Constructor.  Retrieves the services information based on a service name
    XPCGetService(const char *_sServiceName, const char *_sProto = NULL);

    // Constructor.  Retrieves the services information based on the port number
    XPCGetService(int _iPort, const char *_sProto = NULL);

    // Destructor.  Closes the services database
    ~XPCGetService()
    {
#ifdef UNIX
        endservent();
#endif
    }

#ifdef UNIX
    // Gets the next service record within the database
    char cGetNextService();

    // Opens the services database
    void vOpenServiceDb()
    {
        endservent();
        cIteratorFlag = 1;
        setservent(1);
    }
#endif
    // Retrieve the service name
    char *sGetServiceName() { return servicePtr->s_name; }

    // Retrieve the service protocol (UDP or TCP)
    char *sGetServiceProtocol() { return servicePtr->s_proto; }

    // Retrieve the service port
    int iGetServicePort() { return servicePtr->s_port; }
};

#endif
