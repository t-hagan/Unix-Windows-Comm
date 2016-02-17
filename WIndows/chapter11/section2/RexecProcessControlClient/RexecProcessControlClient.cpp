#include <iostream.h>
#include <XPCRexecPipe.h>

int main(int argc, char *argv[])
{
    char sCommand[80];                // The user-entered command
    char sResult[255];                // The results sent by the remote server
    char sLookup[255];                // The process to lookup
    char sServerCommand[255];        // The command sent to the server
    XPCRexecPipe *rexecPipeArray[10];    // An array of ten REXEC pipe clients

    // Check to make sure the correct number of arguments are entered
    // argv[1] - Username
    // argv[2] - Password
    // argv[3] - argv[12] - Remote hosts

    if ((argc < 4) || (argc > 13))
    {
        cout << "Incorrect Number of arguments" << endl;
        return 0;
    }

    // Initialize the WinSock library
    WSADATA    wsaData;
    if (WSAStartup(0x101, &wsaData))
    {
        cout << "Unable to intialize the WinSock library" << endl;
        return 1;
    }

    try
    {
        // Spawn and connect to the ProcessLookupServer on the specified hosts
        for (int iCount = 3; iCount < argc; iCount++)
            rexecPipeArray[iCount-3] = new XPCRexecPipe(argv[1], argv[2], "ProcessLookupServer", argv[iCount], "** DONE **", REXEC_PORT);

        while(1)
        {
            // Accept a user-entered command
            cout << "Enter A Command (LOOKUP, KILL, QUIT): " << flush;
            cin >> sCommand;

            if (!strcmp(sCommand, "LOOKUP"))
            {
                // Prompt the user for the name of the process to lookup
                cout << "Enter The Process Name To Lookup: " << flush;
                cin >> sLookup;

                sprintf(sServerCommand, "%s %s", sCommand, sLookup);
                
                // Send the process name to all remote hosts specified
                for (iCount = 3; iCount < argc; iCount++)
                    *rexecPipeArray[iCount-3] << sServerCommand;
            }
            else if (!strcmp(sCommand, "KILL"))
            {
                // If the user-entered command is "KILL", the user is 
                // prompted for the name of the process to kill.  
                cout << "Enter name of the process to kill: " << flush;
                cin >> sLookup;

                sprintf(sServerCommand, "%s %s", sCommand, sLookup);

                // The combined commands are piped to all connected servers
                for (int iCount = 3; iCount < argc; iCount++)
                    *rexecPipeArray[iCount-3] << sServerCommand;
            }
            else if (!strcmp(sCommand, "QUIT"))
            {
                // If the command is QUIT, send the command to remote hosts specified
                for (int iCount = 3; iCount < argc; iCount++)
                    *rexecPipeArray[iCount-3] << "QUIT";
                break;
            }
            else
            {
                cout << "Unknown command entered" << endl;
                continue;
            }

            // Display the results to the user
            for (int iCount = 3; iCount < argc; iCount++)
                while (*rexecPipeArray[iCount-3] >> sResult)
                {
                    cout << sResult << endl;
                }
        }
    }
    catch(XPCException &myExcept)
    {
        cout << myExcept.sGetException() << endl;
    }
}
