#ifndef _CPubSub
#define _CPubSub

#define PUBLISH 0
#define SUBSCRIBE 1

#define UNIX 0
#define WINDOWS_NT 1

#include <TCPsocketClass.h>
#include <string.h>
#include <CEndian.h>
#include <stdlib.h>

#ifdef UNIXOP
        #define OPSYSTEM UNIX
#else
        #define OPSYSTEM WINDOWS_NT
#endif

class CPubSubMsg
{
    char cType;
    char cOpSystem;
    char sSubscription[256];
    convert<int> iCount;
public:
    CPubSubMsg(int _iCount = 0) { iCount = _iCount; }
    CPubSubMsg(char _cType, char *_sSubscription, int _iCount, char _cOpSystem = OPSYSTEM)
    {
        cOpSystem = _cOpSystem;
        cType = _cType;
        strcpy(sSubscription, _sSubscription);
        iCount = _iCount;
    }
    void vSetValues(char _cType, char *_sSubscription, int _iCount, char _cOpSystem = OPSYSTEM)
        {
                cOpSystem = _cOpSystem;
                cType = _cType;
                strcpy(sSubscription, _sSubscription);
                iCount = _iCount;
        }
    int iGetCount(char _cOpSystem = OPSYSTEM)
    { 
        if (_cOpSystem != cOpSystem)
            return iCount.getSwapValue();
        else
            return iCount.getValue();
    }
    char *sGetSubscription() { return sSubscription; }
    char cGetType() { return cType; }
};

#endif
        
