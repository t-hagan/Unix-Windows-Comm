#include <XPCEvent.h>
#include <XPCSemaphore.h>
class XPCThreadInfo
{
private:
    char* sProdFilename;
    char* sSemName;
    XPCSemaphore semFileSem;
    XPCSemaphore* semThreadSem;
    XPCSemaphore* semMainSem;
    XPCEvent evExit;

public:

    XPCThreadInfo (char* fileName,
                char* semName,
                XPCSemaphore* threadSem,
                XPCSemaphore* mainSem) :

                sProdFilename (fileName),
                semFileSem (semName),
                sSemName (semName),
                semThreadSem (threadSem),
                semMainSem (mainSem) { }

    ~XPCThreadInfo () {}
    char* sProducerFilename() {return sProdFilename;}
    void ProducerFilename (char* fName) {sProdFilename= fName;}

    char* SemName() {return sSemName;}

    XPCSemaphore FileSem() {return semFileSem;}

    XPCSemaphore* ThreadSem() {return semThreadSem;}
    void ThreadSem (XPCSemaphore* threadSem) {semThreadSem= threadSem;}

    XPCSemaphore* MainSem() {return semMainSem;}
    void MainSem (XPCSemaphore* mainSem) {semMainSem= mainSem;}

    XPCEvent& ExitEvent() {return evExit;}
};


class XPCMessage
{
private:
    char* sMsg;
    char* sFname;

public:
    char* sMessage () {return sMsg;}
    void Message (char* msg) {sMsg= msg;}

    char* sFilename() {return sFname;}
    void Filename (char* fn) {sFname= fn;}

    XPCMessage() :
        sMsg(0),
        sFname(0) {}
        ~XPCMessage() { }
};
