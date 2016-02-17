#ifndef _XPCSpawnInfo
#define _XPCSpawnInfo

#include <string.h>

// Action definitions
#define REGISTER 0
#define SPAWN 1

class XPCSpawnInfo
{
        char cAction;        // Stores the action definition
        char sAppName[150];// Stores the spawned process name and any required
                            // parameters
public:
    // Constructor
    XPCSpawnInfo(char _cAction, char *_sAppName)
    {
        cAction = _cAction;
        strcpy(sAppName, _sAppName);
    }

    // Returns the values of the private data members
    const char cGetAction() const { return cAction; }
    const char *sGetAppName() const { return sAppName; }
};

class XPCServerSpawn: public XPCSpawnInfo
{
    int iPid;    // Stores the process's ID
public:
    // Constructor
    XPCServerSpawn(char *_sAppName, int _iPid) : XPCSpawnInfo(REGISTER, _sAppName) { iPid = _iPid; }
    
    // Returns the process's ID
    const int iGetPid() const { return iPid; }
};

#endif
