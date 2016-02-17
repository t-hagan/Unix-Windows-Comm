#ifndef _XPCGetHostInfo
#define _XPCGetHostInfo

#include <XPCException.h>
#ifdef UNIX
    #include <arpa/inet.h>
    #include <netdb.h>
    #include <netinet/in.h>
    #include <sys/socket.h>
#else
    #include <winsock2.h>
    #define in_addr_t unsigned long
#endif
    
enum hostType {NAME, ADDRESS};

class XPCGetHostInfo
{
#ifdef UNIX
    char cIteratorFlag;        // Host database iteration flag
#endif
    struct hostent *hostPtr;    // Entry within the host address database
public:
#ifdef UNIX
    // Default constructor.  Opens the host entry database
    XPCGetHostInfo()
    {
        vOpenHostDb();
    }
#endif
    // Retrieves the host entry based on the host name or address
    XPCGetHostInfo(const char *_sHostName, hostType _type);

    XPCGetHostInfo(in_addr_t *_netAddr);

    // Destructor.  Closes the host entry database.
    ~XPCGetHostInfo()
    {
#ifdef UNIX
        endhostent();
#endif
    }
#ifdef UNIX
    // Retrieves the next host entry in the database
    char cGetNextHost();

    // Opens the host entry database
    void vOpenHostDb()
    {
        endhostent();
        cIteratorFlag = 1;
        sethostent(1);
    }
#endif
    // Retrieves the hosts IP address
    char *sGetHostAddress() 
    {
        struct in_addr *addr_ptr;
        addr_ptr = (struct in_addr *)*hostPtr->h_addr_list;
        return inet_ntoa(*addr_ptr);
    }    
    
    // Retrieves the hosts name
    char *sGetHostName()
    {
        return hostPtr->h_name;
    }
};

#endif
