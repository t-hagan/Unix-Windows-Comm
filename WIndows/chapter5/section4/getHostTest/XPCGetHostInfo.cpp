#include <XPCGetHostInfo.h>

XPCGetHostInfo::XPCGetHostInfo(const char *_sHost, hostType _type)
{
#ifdef UNIX
    cIteratorFlag = 0;
#endif
    if (_type == NAME)
    {
        // Retrieve host by name
        hostPtr = gethostbyname(_sHost);
        if (hostPtr == NULL)
        {
            XPCException exceptObject("Error Getting Host By Name");
            throw exceptObject;
            return;
        }
    }    
    else if (_type == ADDRESS)
    {
        // Retrieve host by address
        unsigned long netAddr = inet_addr(_sHost);
        if (netAddr == -1)
        {
            XPCException exceptObject("Error Getting Host By Address");
            throw exceptObject;
            return;
        }

        hostPtr = gethostbyaddr((char *)&netAddr, sizeof(netAddr), AF_INET);
        if (hostPtr == NULL)
        {
            XPCException exceptObject("Error Getting Host By Address");
            throw exceptObject;
            return;
        }
    }    
    else
    {
        XPCException exceptObject("Parameter Error Constructing XPCGetHostInfo");
        throw exceptObject;    
        return;
    }
}

XPCGetHostInfo::XPCGetHostInfo(in_addr_t *_netAddr)
{
    if (*_netAddr == -1)
    {
        XPCException exceptObject("Error Getting Host By Address");
        throw exceptObject;
        return;
    }

    hostPtr = gethostbyaddr((char *)_netAddr, sizeof(*_netAddr), AF_INET);
    if (hostPtr == NULL)
    {
        XPCException exceptObject("Error Getting Host By Address");
        throw exceptObject;
        return;
    }
}

#ifdef UNIX
char XPCGetHostInfo::cGetNextHost()
{
     // Get the next host from the database
     if (cIteratorFlag == 1)
     {
         if ((hostPtr = gethostent()) == NULL)
              return 0;
         else
              return 1;
     }
     return 0;
}
#endif
