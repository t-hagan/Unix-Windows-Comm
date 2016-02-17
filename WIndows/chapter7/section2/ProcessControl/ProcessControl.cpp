#include <iostream.h>
#include <XPCRexecSetup.h>        // XPCRexecSetup class definition

int main(int argc, char *argv[])
{
    char sCommand[80];    // Command read from user
    char sProcessId[10];// Process ID to kill
    char sResult[255];    // Result from iProcessResults
    int iRetCode;        // Return code
    WSADATA wsaData;    // Holds winsock initialization information.

    // Three arguments are expected:
    // argv[1] - user name
    // argv[2] - password
    // argv[3] - UNIX host
    if (argc < 4)
    {
        cerr << "Incorrect number of arguments" << endl;
        return 1;
    }

    // Initialize winsock
    if (WSAStartup(0x101, &wsaData))
    {
        cerr <<  "Unable to initalize the winsock library" << endl;
        return 1;
    }

    // Catch all exceptions
    try
    {
        // Create an instance of XPCRexecSetup passing the user name, password, host, and the UNIX server command
        // ps_control. If the XPCRexecSetup instance fails exit the client
        
        XPCRexecSetup rexecSession(argv[1], argv[2], "/chapter7/section2/ProcessControlServer", argv[3], "*DONE*", REXEC_PORT);

        // Send the host the user name, password, and command and check validation
        rexecSession.vREXEC();

        // Continue in a loop until the user enters "QUIT"
        while(1)
        {

            // Accept a command
            cout << "Enter A Command (PS, KILL, QUIT): " << flush;
            cin >> sCommand;
            strcat(sCommand, "\n");
            // Send the command to the server
            if (!rexecSession.iSendMessage((void *)sCommand, strlen(sCommand)))
            {
                cout << "Error sending " << sCommand << endl;
                continue;
            }

            // If the command sent is KILL, read the process id from the user and send it to the client.
            if (!strncmp(sCommand, "KILL", 4))
            {
                //strcat(sCommand, " ");

                cout << "Enter the Process Id: " << flush;
                cin >> sProcessId;
                strcat(sProcessId, "\n");
                
                if (!rexecSession.iSendMessage((void *)sProcessId, strlen(sProcessId)))
                {
                    cout << "Error sending " << sProcessId << endl;
                    continue;
                }
                strcat(sCommand, sProcessId);

                if (!rexecSession.iSendMessage((void *)sCommand, strlen(sCommand)))
                {
                    cerr << "Error sending process id: " << sCommand << endl;
                    continue;
                }
            }

            // If the command is QUIT exit the client
            if (!strncmp(sCommand, "QUIT", 4))
                break;

            // Process output from the server and display it on the console
            while ((iRetCode = rexecSession.iProcessResults(sResult)) == SUCCESS)
                cout << sResult << endl;

            if (iRetCode == FAIL)
            {
                cout << "Error processing results" << endl;
            }
            continue;
        }
    }
    catch(XPCException &exceptObject)
    {
        cerr << exceptObject.sGetException() << endl;
        return 1;
    }
    return 0;
}
