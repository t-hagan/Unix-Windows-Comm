#ifndef _XPCPthread
#define _XPCPthread

#include <pthread.h>
#include <XPCPthreadAttr.h>

template <class StatusClass> class XPCPthread
{
private:
    pthread_t thread_id;    // Id of the thread
    StatusClass *status;    // The completion status of the thread
public:
    // Default constructor used when the thread creation is unnecessary and the thread
    // ID is unknown
    XPCPthread() { thread_id = pthread_self(); }

    // Constructor.  Takes the thread function, and any thread attributes
    XPCPthread(void *func(void *), void *arg, XPCPthreadAttr *_threadAttr = NULL);

    // Exit the current thread if the thread's ID is equal to thread_id
    void vExitThread(StatusClass *_Status);

    // Sends a signal to the thread defined by thread_id
    void vSignalThread(int _iSig);

    // Waits for the thread's completion
    void vWaitForThread();

    // Returns the completion status of the thread
    StatusClass *getStatus() { return status; }
};

#endif
