#include <fstream.h>
#include <XPCDaemon.h>

int main()
{
    // Create an instance of the XPCDaemon.  Since this class has no data members it 
    // does not need to be created on the heap.  
    try
    {
        XPCDaemon createANewDaemon;
    }
    catch(XPCException &exceptObject)
    {
            // If creating the daemon class fails, return an error code of 0
            cout << exceptObject.sGetException() << endl;
            return 0;
    }
    
    // Open a file in the current working directory to log messages the daemon produces.
    ofstream daemonMsgs("~/book/chapter2/section5/daemonMsg.log");

    // If file creation fails, exit the process returning an error code of 0
    if (!daemonMsgs)
    {
        return 0;
    }

    daemonMsgs << "Successfully creating daemon!!!" << endl;
    
    // Insert remainder of daemon code here.
    while(1) { continue; }

    // This code should never be executed since the daemon runs continuously.
    return 1;
}

