#ifndef _XPCPthreadAttr
#define _XPCPthreadAttr

#include <pthread.h>
#include <errno.h>
#include <XPCException.h>

class XPCPthreadAttr
{
private:
    pthread_attr_t attr;    // Stores thread attribute
public:
    // Default constuctor
    XPCPthreadAttr();
    
    // Destructor    
    ~XPCPthreadAttr();

    // Destroys the attribute stored in the private data member
    void vDestroyAttr();

    // Gets the thread's stack size attribute
    int iGetStackSize();

    // Sets the thread's stack size attribute
    void vSetStackSize(int _iStackSize);

    // Gets the address of the thread stack
    void *vGetStackAddr();

    // Sets the address of the thread stack
    void vSetStackAddr(void *_vStackAddr);

    // Gets the detached state of the thread
    int iGetDetachedState();

    // Sets the detached state of the thread
    void vSetDetachedState(int _iDetachedState);

    // Gets the processing scope of the thread
    int iGetScope();

    // Sets the processing scope of the thread
    void vSetScope(int _iScope);

    // Gets the inherited scheduling of a thread
    int iGetInheritSched();

    // Sets the inhertied scheduling property of a thread
    void vSetInheritSched(int _iInheritSched);

    // Gets the scheduling policy of the thread
    int iGetSchedPolicy();

    // Sets the scheduling policy of the thread
    void vSetSchedPolicy(int _iPolicy);

    // Gets the scheduling structure of the thread
    struct sched_param getSchedParam();

    // Sets the scheduling structure of the thread    
    void vSetSchedParam(const struct sched_param *schedParam);

    // Returns the attr private data member
    pthread_attr_t *getAttr() { return &attr; }    
};

#endif
