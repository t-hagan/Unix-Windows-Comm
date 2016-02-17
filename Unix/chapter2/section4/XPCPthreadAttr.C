#include <XPCPthreadAttr.h>
#include <stdio.h>
#include <string.h>

XPCPthreadAttr::XPCPthreadAttr()
{
    if (pthread_attr_init(&attr) != 0)
    {
        char sErrMsg[512];

        sprintf(sErrMsg, "Error Constructing pthreadAttr: %s", strerror(errno));
        XPCException semExcept(sErrMsg);
        throw semExcept;
        return;
    }
}

void XPCPthreadAttr::vDestroyAttr()
{
    if (pthread_attr_destroy(&attr) != 0)
    {
                char sErrMsg[512];

                sprintf(sErrMsg, "Error Destroying the Attributes: %s", strerror(errno));
                XPCException semExcept(sErrMsg);
                throw semExcept;
                return;
    }
}

int XPCPthreadAttr::iGetStackSize()
{
    size_t stackSize;

    if (pthread_attr_getstacksize(&attr, &stackSize) != 0)
    {
                char sErrMsg[512];

                sprintf(sErrMsg, "Error Returning Attribute Stack Size: %s", strerror(errno));
                XPCException semExcept(sErrMsg);
                throw semExcept;
                return -1;
    }

    return stackSize;
} 

void XPCPthreadAttr::vSetStackSize(int _iStackSize)
{
    if (pthread_attr_setstacksize(&attr, (size_t)_iStackSize) != 0)
    { 
                char sErrMsg[512]; 
 
                sprintf(sErrMsg, "Error Setting Attribute Stack Size: %s", strerror(errno)); 
                XPCException semExcept(sErrMsg); 
                throw semExcept; 
                return;
    }
}

void *XPCPthreadAttr::vGetStackAddr()
{
    void *vStackAddr;

    if (pthread_attr_getstackaddr(&attr, &vStackAddr) != 0)
    { 
                char sErrMsg[512]; 
 
                sprintf(sErrMsg, "Error Getting Attribute Stack Addr: %s", strerror(errno)); 
                XPCException semExcept(sErrMsg); 
                throw semExcept; 
                return NULL;
    }

    return vStackAddr;
}

void XPCPthreadAttr::vSetStackAddr(void *_vStackAddr)
{
    if (pthread_attr_setstackaddr(&attr, _vStackAddr) != 0)
    { 
                char sErrMsg[512]; 
 
                sprintf(sErrMsg, "Error Setting Attribute Stack Addr: %s", strerror(errno)); 
                XPCException semExcept(sErrMsg); 
                throw semExcept; 
                return;
    }
}    

int XPCPthreadAttr::iGetDetachedState()
{
    int iDetachedState;

    if (pthread_attr_getdetachstate(&attr, &iDetachedState) != 0)
    { 
                char sErrMsg[512]; 
 
                sprintf(sErrMsg, "Error Setting Attribute Stack Size: %s", strerror(errno)); 
                XPCException semExcept(sErrMsg); 
                throw semExcept; 
                return -1;
    }

    return iDetachedState;
}    

void XPCPthreadAttr::vSetDetachedState(int _iDetachedState)
{
    if (pthread_attr_setdetachstate(&attr, _iDetachedState) != 0)
    { 
                char sErrMsg[512]; 
 
                sprintf(sErrMsg, "Error Setting Attribute Detached State: %s", strerror(errno)); 
                XPCException semExcept(sErrMsg); 
                throw semExcept; 
                return;
    }
}

int XPCPthreadAttr::iGetScope()
{
    int iScope;

    if (pthread_attr_getscope(&attr, &iScope) != 0)
    { 
                char sErrMsg[512]; 
 
                sprintf(sErrMsg, "Error Getting Attribute Scope: %s", strerror(errno)); 
                XPCException semExcept(sErrMsg); 
                throw semExcept; 
                return -1;
    }

    return iScope;
}

void XPCPthreadAttr::vSetScope(int _iScope)
{
    if (pthread_attr_setscope(&attr, _iScope) != 0)
    {
                char sErrMsg[512]; 
 
                sprintf(sErrMsg, "Error Getting Attribute Scope: %s", strerror(errno)); 
                XPCException semExcept(sErrMsg); 
                throw semExcept; 
                return;
    }
}

int XPCPthreadAttr::iGetInheritSched()
{
    int iInheritSched;

    if (pthread_attr_getinheritsched(&attr, &iInheritSched) != 0)
    { 
                char sErrMsg[512]; 
 
                sprintf(sErrMsg, "Error Getting Inherit Sched: %s", strerror(errno)); 
                XPCException semExcept(sErrMsg); 
                throw semExcept; 
                return -1;
    }

    return iInheritSched;
}    

void XPCPthreadAttr::vSetInheritSched(int _iInheritSched)
{
    if (pthread_attr_setinheritsched(&attr, _iInheritSched) != 0)
    {       
                char sErrMsg[512];     
 
                sprintf(sErrMsg, "Error Setting Inherit Sched: %s", strerror(errno));        
                XPCException semExcept(sErrMsg);  
                throw semExcept;       
                return;
    }
}

int XPCPthreadAttr::iGetSchedPolicy()
{
    int iPolicy;

    if (pthread_attr_getschedpolicy(&attr, &iPolicy) != 0)
    {       
                char sErrMsg[512];     
 
                sprintf(sErrMsg, "Error Getting Attribute Sched Policy: %s", strerror(errno));        
                XPCException semExcept(sErrMsg);  
                throw semExcept;       
                return -1;
    }

    return iPolicy;
}

void XPCPthreadAttr::vSetSchedPolicy(int _iPolicy)
{
    if (pthread_attr_setschedpolicy(&attr, _iPolicy) != 0)
    {       
                char sErrMsg[512];     
 
                sprintf(sErrMsg, "Error Setting Attribute Sched Policy: %s", strerror(errno));        
                XPCException semExcept(sErrMsg);  
                throw semExcept;       
                return;
    }
}

struct sched_param XPCPthreadAttr::getSchedParam()
{
    struct sched_param sched;

    if (pthread_attr_getschedparam(&attr, &sched) != 0)
    {
        char sErrMsg[512];     
 
        sprintf(sErrMsg, "Error Setting Attribute Sched: %s", strerror(errno));        
        XPCException semExcept(sErrMsg);  
        throw semExcept;       
        return sched;
    }   
    
    return sched;   
}    

void XPCPthreadAttr::vSetSchedParam(const struct sched_param *schedParam)
{
    if (pthread_attr_setschedparam(&attr, schedParam) != 0)
    {
        char sErrMsg[512];     
 
        sprintf(sErrMsg, "Error Setting Attribute Sched Policy: %s", strerror(errno));        
        XPCException semExcept(sErrMsg);  
        throw semExcept;       
        return;
    }      
}    
