#include "XPCEvent.h"
#include "XPCSemaphore.h"
#include "XPCThread.h"

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

class XPCProducerInfo
{
    char* sFileName;
    char* sSemName;

    XPCThread* consumerThread;

public:
    XPCProducerInfo (char* _sFileName, char* _sSemName,
                    XPCThread* _consumerThread)
    {
        sFileName= new char[strlen (_sFileName)+1];
        sSemName= new char[strlen (_sSemName)+1];
        strcpy (sFileName, _sFileName);
        strcpy (sSemName, _sSemName);

        consumerThread= _consumerThread;
    }

    // copy constructor
    XPCProducerInfo (const XPCProducerInfo& rhs)
    {
        sFileName= new char[strlen (rhs.sFileName)+1];
        sSemName= new char[strlen (rhs.sSemName)+1];

        strcpy (sFileName, rhs.sFileName);
        strcpy (sSemName, rhs.sSemName);

        consumerThread= rhs.consumerThread;
    }

    char* sGetFilename() {return sFileName;}
    char* sGetSemname() {return sSemName;}
    XPCThread* getConsumerThread() {return consumerThread;}

    // assignment operator
    XPCProducerInfo& operator = (const XPCProducerInfo& rhs)
    {
        if (this == &rhs)
            return *this;

        if (sFileName)
            delete [] sFileName;

        if (sSemName)
            delete [] sSemName;

        sFileName= new char[strlen(rhs.sFileName)+1];
        sSemName= new char[strlen(rhs.sSemName)+1];

        strcpy (sFileName, rhs.sFileName);
        strcpy (sSemName, rhs.sSemName);
        consumerThread= rhs.consumerThread;
        return *this;
    }
};
