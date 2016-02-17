#include <XPCPipe.h>        // The XPCPipe class definition
#include <XPCKill.h>        // The XPCKill class definition
#include <XPCProcessList.h>    // The XPCProcessList class definition
#include <stdlib.h>

#define PROCESS_LIST_PORT 6700
#define KILL_PORT 6701

main()
{
    int iMenuChoice;            // Holds the menu choice
    char sProcessName[256];            // Holds the user specified process name
    long int iProcessID;            // Holds the user specified process ID

    try
    {
        while(1)    // Loop forever
        {
            cout << endl << "** Process Control Menu **" << endl;
            cout << "(1) Retrieve Process Information" << endl;
            cout << "(2) Kill Remote Process" << endl;
            cout << "(3) Exit" << endl;
            cout << "Choose Item: " << flush;
            cin >> iMenuChoice;

            if (iMenuChoice == 1)
            {
                    // If the user chooses to retrieve process 
                    // information a XPCProcessList object is created 
                    // using the process name entered.  A XPCPipe object 
                    // is created connecting to the server, and the 
                    // XPCProcessList object is sent through the pipe.

                    cout << "Enter Process Name or ALL: " << flush;
                    gets(sProcessName);

                    XPCProcessList remoteProcessInfo(sProcessName);
                    XPCPipe<XPCProcessList> processListPipe(PROCESS_LIST_PORT, "aHost");
                    processListPipe << remoteProcessInfo;

                    // The list of processes are retrieved and displayed 
                    // until no more are sent by the server
                    cout << "Process List: " << endl << endl;
                    while(processListPipe >> remoteProcessInfo)
                    {
                        cout << remoteProcessInfo.sGetProcessName() << "\t" << remoteProcessInfo.iGetProcessID() << endl;
                    }
            }    
            else if (iMenuChoice == 2)
            {    
                    // If the user chooses to terminate a process, the 
                    // process ID is prompted for.  The process ID is 
                    // used to construct a XPCKill object.  A XPCPipe object
                    // is constructed, connecting to the server's 
                    // KILL_PORT.  The XPCKill object is sent to the 
                    // server via the pipe.
                    cout << "Enter Process ID: " << flush;
                    cin >> iProcessID;
            
                    XPCKill remoteProcessKill(iProcessID);
                    XPCPipe<XPCKill> killPipe(KILL_PORT, "aHost");
                    killPipe << remoteProcessKill;

                    // The server's kill confirmation is retrieved, it 
                    // the results are displayed
                    killPipe >> remoteProcessKill;

                    if (remoteProcessKill.iGetProcessID() == -1)
                        cout << "Error killing process" << endl;
                    else
                        cout << "Successfully killed process" << endl;
            }    
            else if (iMenuChoice == 3)
            {    
                    // If the user chooses to exit the application, the 
                    // application returns
                    return 1;
            }
            else
                    cout << "Error: Invalid menu item chosen" << endl;
        }
    }
    catch(XPCException &exceptOb)
    {
        // All XPCPipe related exceptions are caught and displayed for the user.
        cerr << "Communication Error: " << exceptOb.sGetException() << endl;
        return 0;
    }
}            
