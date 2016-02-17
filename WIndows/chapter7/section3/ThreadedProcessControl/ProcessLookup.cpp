#include <XPCRexecSetup.h>
#include <XPCThread.h>
#include <iostream.h>
#include <XPCRexecThread.h>

DWORD WINAPI vRexecThreadFunc(void *_vParam);

int main(int argc, char *argv[])
{
    char sCommand[80];    // Command read from user
    int iProcessCount;        // A counter for the number of remote hosts the user has requested on the command line. 
    int iRetCode;        // Return code
    WSADATA wsaData;    // Holds winsock initialization information.
    char sProcess_name[80];    // Name of the process being looked up

    // Three arguments are expected:
    // argv[1] - Username
    // argv[2] - Password
    // argv[3] and greater - Remote hosts

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

    // Create a named semaphore which as a maximum value of number of hosts and is 
    // initialized to the number of hosts.
    XPCSemaphore mySem("THREAD_SEM", CREATE_SEM, argc - 3, argc - 3);

    while(1)
    {
    // Catch all exceptions
    try
    {
        // Prompt for user input.
        cout << "Enter a command (LOOKUP, QUIT): " << flush;
        cin >> sCommand;

        // If the user command is LOOKUP prompt for the process name to look up.
        if (!strcmp(sCommand, "LOOKUP"))
        {
            strcat(sCommand, "\n");
            cout << "Enter Process Name: " << flush;
            cin >> sProcess_name;
            strcat(sProcess_name, "\n");

            // Spawn the correct number of threads, one for each remote host, and lock 
            // the semaphore for each one spawned.
            for (iProcessCount = 3; iProcessCount < argc; iProcessCount++)
            {
                mySem.vLockWait();

                // Create a XPCRexecThread instance with the username, password, 
                // process name to be search, and remote host.  Pass this class to 
                // the rexec thread. 
                XPCRexecThread *newThread = new XPCRexecThread(argv[1], argv[2], argv[iProcessCount], sCommand, &mySem, sProcess_name);

                XPCThread rexecThread(vRexecThreadFunc, (void *)newThread);
            }

            for (iProcessCount = 3; iProcessCount < argc; iProcessCount++)
                mySem.vLockWait();
            for (iProcessCount = 3; iProcessCount < argc; iProcessCount++)
                mySem.vUnlock();
        }

        // If the user command is QUIT exit the client program
        else if (!strcmp(sCommand, "QUIT"))
            break;

        // If the user command is unknown issue the error message
        else
            cout << "Command: " << sCommand << " is Unknown" << endl;
    }
    catch(XPCException &exceptOb)
    {
        cerr << exceptOb.sGetException() << endl;
        return;
    }
    }
}


DWORD WINAPI vRexecThreadFunc(void *_vParam)
{
    XPCRexecThread *newThread = (XPCRexecThread *)_vParam;    // The XPCRexecThread 
                                                                // passed to vRexecThreadFunc
    char sResult[255];                        // Results from remote server 
                                            // output.
    int iRetCode;                            // A return code.
    
    // Catch all REXEC related exceptions.
    try
    {
        // Create a XPCRexecSetup instance passing the username,password, target 
        // command, remote host, and socket port number.
        XPCRexecSetup rexecSession(newThread->sGetUsername(), newThread->sGetPassword(), "/home/nadelsm/book/chapter7/section3/ProcessLookupServer", newThread->sGetHost(), "*DONE*", REXEC_PORT);

        // Send the username, password, and target command to the remote host for 
        // validation.
        rexecSession.vREXEC();

        // Send the PS command to the server
        if (!rexecSession.iSendMessage(newThread->sGetCommand(), strlen(newThread->sGetCommand())))
        {
            cerr << "Error sending: "<< newThread->sGetCommand() << endl;
            newThread->getSem()->vUnlock();
            delete newThread;
            return 0;
        }    

        // Send the process name to search to the server
        if (!rexecSession. iSendMessage(newThread->sGetProcessName(), strlen(newThread->sGetProcessName())))
        {
            cout << "Error sending: "<< newThread->sGetProcessName() << endl;
            newThread->getSem()->vUnlock();
            delete newThread;
            return 0;
        }
    
        // Read return results from the server and display it for the user.
        while ((iRetCode = rexecSession.iProcessResults(sResult)) == SUCCESS)
            cout << sResult << endl;

        if (iRetCode == FAIL)
        {
            cout << "Error processing results" << endl;
        }
    }
    catch(XPCException &exceptObject)
    {
        cerr << exceptObject.sGetException() << endl;
    }

    // Unlock a semaphore value
    newThread->getSem()->vUnlock();

    // Remove the instance of XPCRexecThread
    delete newThread;
    return 0;
}

