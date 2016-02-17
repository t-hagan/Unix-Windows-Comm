#include <XPCPthread.h>
#include <stdio.h>
#include <string.h>
#include <XPCException.h>

template <class StatusClass> XPCPthread<StatusClass>::XPCPthread(void *func(void *), void *_vArg, XPCPthreadAttr *_threadAttr)
{
    if (_threadAttr == NULL)
    {
        // Create a new thread using the user defined function and default thread parameters 
        if (pthread_create(&thread_id, NULL, func, _vArg) != 0)
        {
                    char sErrMsg[512];

                    sprintf(sErrMsg, "Error Constructing XPCPthread: %s", strerror(errno));
                    XPCException semExcept(sErrMsg);
                    throw semExcept;
                    return;
        }
    }

    // Create a new thread using the user defined function and the attributes defined within
    // the XPCPthreadAttr object
    else if (pthread_create(&thread_id, _threadAttr->getAttr(), func, _vArg) != 0)
    {
        char sErrMsg[512];

        sprintf(sErrMsg, "Error Constructing XPCPthread: %s", strerror(errno));
        XPCException semExcept(sErrMsg);
        throw semExcept;
        return;
    }
}

template <class StatusClass> void XPCPthread<StatusClass>::vExitThread(StatusClass *Status)
{
    // Exit the current thread using a given status
    pthread_exit((void *)Status);
}

template <class StatusClass> void XPCPthread<StatusClass>::vSignalThread(int _iSig)
{
    // Send a signal to the specified thread
    if (pthread_kill(thread_id, _iSig) != 0)
    {
        char sErrMsg[512];

        sprintf(sErrMsg, "Error Constructing XPCPthread: %s", strerror(errno));
        XPCException semExcept(sErrMsg);
        throw semExcept;
        return;
    }
}

template <class StatusClass> void XPCPthread<StatusClass>::vWaitForThread()
{
    void *vStatus;

    // Wait for a thread to complete and capture its status
    if (pthread_join(thread_id, &vStatus) != 0)
    {
        char sErrMsg[512];

        sprintf(sErrMsg, "Error Constructing XPCPthread: %s", strerror(errno));
        XPCException semExcept(sErrMsg);
        throw semExcept;
        return;
    }
    
    status = (StatusClass *)vStatus;
}
