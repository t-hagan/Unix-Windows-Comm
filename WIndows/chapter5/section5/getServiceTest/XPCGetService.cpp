#include <XPCGetService.h>
#include <stdio.h>

XPCGetService::XPCGetService(const char *_sServiceName, const char *_sProto)
{
    // Retrieve the service database record based on the service name and protocol
    if (_sProto != NULL)
    {
        try
        {
            XPCGetProtocol verifyProtocol(_sProto);
        }
        catch (XPCException &exceptObject)
        {
            char sMsg[512];
            
            sprintf(sMsg, "Error getting service using Protocol %s: %s\0", _sProto, exceptObject.sGetException());
            XPCException getServiceException(sMsg);
            throw getServiceException;
            return;
        }
    }

    if ((servicePtr = getservbyname(_sServiceName, _sProto)) == NULL)
    {
        XPCException getServiceException("Cannot Find Service By Name");
        throw getServiceException;
        return;
    }
}

XPCGetService::XPCGetService(int _iPort, const char *_sProto)
{
    // Retrieve the services database record based on the service port and protocol
        if (_sProto != NULL)
        {
                try
                {
                        XPCGetProtocol verifyProtocol(_sProto);
                }
                catch (XPCException &exceptObject)
                {               
                        char sMsg[512]; 
                         
                        sprintf(sMsg, "Error getting service using Protocol %s: %s\0", _sProto, exceptObject.sGetException
());
                        XPCException getServiceException(sMsg);  
                        throw getServiceException;
                        return;
                }
        }
 
        if ((servicePtr = getservbyport(_iPort, _sProto)) == NULL) 
        {
                XPCException getServiceException("Cannot Find Service By Port");
                throw getServiceException;
                return;
        }
}

#ifdef UNIX
char XPCGetService::cGetNextService()
{
    // Get the next services record from the database
    if (cIteratorFlag == 1)
    {
        if ((servicePtr = getservent()) == NULL)
            return 0;
        else
            return 1;
    }
    else
        return 0;
}
#endif
