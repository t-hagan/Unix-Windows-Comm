#include <XPCPipe.h>            // The XPCPipe class definition
#include <XPCKill.h>            // The XPCKill class definition
#include <XPCProcessList.h>    // The XPCProcessList class definition
#include <XPCThread.h>        // The XPCThread class definition
#include <winuser.h> 


#define PROCESS_LIST_PORT 6700
#define KILL_PORT 6701

// Linked list definition of XPCProcessList objects
class XPCProcessListLinked : public XPCProcessList
{
public:
    XPCProcessListLinked *next;
    XPCProcessListLinked(char *_sAppName, int _iProcessID) : XPCProcessList(_sAppName, _iProcessID) { next = NULL; }
};

// Contains a linked-list of XPCProcessListLinked objects
struct SProcessList
{
    char sAppName[256];        // Name of the user-requested application.  Can be
                            // "ALL" or a specific application name
    XPCProcessListLinked *firstProcess;
    XPCProcessListLinked *currentProcess;
    SProcessList() { firstProcess = NULL; }
};

DWORD WINAPI iRetrieveProcessList(void *_vArg);
DWORD WINAPI iKillProcess(void *_vArg);

main()
{
    int iMaxSocketId = 0;        // Holds the maximum socket file descriptor value
    fd_set fdset;                // The set of file descriptors used by select
    WSADATA wsaData;

    // Initialize the Winsock library
    if (WSAStartup(0x101, &wsaData) != 0)
    {
        cout << "Error initializing Winsock library: " << WSAGetLastError() << endl;
        return 0;
    }

    try
    {
        // Two server pipes are constructed.  One for processing process list
        // messages and the other for processing process kill messages.
        XPCPipe<XPCProcessList> processListPipe((long int)PROCESS_LIST_PORT);
        XPCPipe<XPCKill> killPipe((long int)KILL_PORT);

        // The larger socket file descriptor is chosen
        if (processListPipe.GetSocket()->iGetSocketFd() > killPipe.GetSocket()->iGetSocketFd())
            iMaxSocketId = processListPipe.GetSocket()->iGetSocketFd() + 1;
        else
            iMaxSocketId = killPipe.GetSocket()->iGetSocketFd() + 1;

        while(1)    // Loop forever 
        {
            // The set of file descriptors need to be reassigned during each 
            // interation of the loop since select replaces the original fdset 
            // with return results.
            FD_ZERO(&fdset);    // Clears all file descriptors out of fdset

            // Inserts the two socket file descriptors into fdset
            FD_SET(processListPipe.GetSocket()->iGetSocketFd(), &fdset);
            FD_SET(killPipe.GetSocket()->iGetSocketFd(), &fdset);
    
            // Asynchronously select socket file descriptors available for
            // receiving messages.  Since clients connecting are reading for 
            // receiving messages, the current fdset is placed in the readfds
            // parameter.  The select is set to never timeout
            int iSelectRetValue = select(iMaxSocketId, &fdset, NULL, NULL, 0);

            // If select returns a -1, select failed.
            if (iSelectRetValue == -1)
            {
                cerr << "Select failed" << endl;
                return 1;
            } 

            // A loop is executed for the number of file descriptors returned 
            // in the file descriptor set.
            for (int iCount = 0; iCount < iSelectRetValue; iCount++)
            {
                // If the client connects on the 'processListPipe', the 
                // connection is accepted and a thread is spawned to 
                // process the client request
                if (FD_ISSET(processListPipe.GetSocket()->iGetSocketFd(), &fdset) != 0)
                {
                    XPCPipe<XPCProcessList> *newConnection = processListPipe.vProcessConnection();
                    XPCThread processListPipeThread(iRetrieveProcessList, (void *)newConnection);
                    continue;
                }

                // If the client connects on the 'killPipe', the connection 
                // is accepted and a thread is spawned to process the 
                // client request
                else if (FD_ISSET(killPipe.GetSocket()->iGetSocketFd(), &fdset) != 0)
                {
                    XPCPipe<XPCKill> *newConnection = killPipe.vProcessConnection();
                    XPCThread killPipe(iKillProcess, (void *)newConnection);
                    continue;
                }
            }
        }
    }
    catch(XPCException &exceptOb)
    {
        cerr << "Server Error Occurred: " << exceptOb.sGetException();
        return 1;
    }
}

BOOL CALLBACK EnumerateProcesses(HWND handle, LPARAM lpData)
{
    // Callback function that enumerates through all processes that contain a window 
    // and stores the requested process or processes within a linked-list

    // The structure containing the linked-list is cast to EnumerateProcess's incoming 
    // parameter
    SProcessList *processList = (SProcessList *)lpData;
    
    LPDWORD ProcessID = new DWORD;    // Stores the application's process ID
    DWORD result;                    // Stores the result of 
                                    // GetWindowThreadProcessID()
    char sProcessName[256];            // Stores the name of the enumerated window

    // Retrieves the next process
    result = GetWindowThreadProcessId(handle, ProcessID);

    // Retrieves the name of the process's window
    if (GetWindowText(handle, sProcessName, 256))
    {
        // If the requested process name is not ALL, only store the process 
        // matching the name
        if (strcmp(processList->sAppName, "ALL") != 0)
            if (strcmp(processList->sAppName, sProcessName) != 0)
                return TRUE;

        // Store the process information within the linked-list
        if (processList->firstProcess == NULL)
        {
            processList->firstProcess = new XPCProcessListLinked(sProcessName, *ProcessID);
            processList->currentProcess = processList->firstProcess;
        }
        else
        {
            XPCProcessListLinked *newProcess = new XPCProcessListLinked(sProcessName, *ProcessID);
            processList->currentProcess->next = newProcess;
            processList->currentProcess = processList->currentProcess->next;
        }
    }
    return TRUE;
}
        
DWORD WINAPI iRetrieveProcessList(void *_vArg)
{
    XPCPipe<XPCProcessList> *processListPipe = (XPCPipe<XPCProcessList> *)_vArg;    // XPCPipe object passed in
    XPCProcessList newRequest;                    // Stores the client's message
    SProcessList theList;                        // Stores the list of process 
                                                // information

    try
    {
        // Retrieve the client's request
        *processListPipe >> newRequest;
        
        // Store the user's request
        strcpy(theList.sAppName, newRequest.sGetProcessName());

        // Get requested process information
        if (!EnumWindows((WNDENUMPROC)EnumerateProcesses, (LPARAM)&theList))
        {
            cerr << "Error getting process information" << endl;
            return 0;
        }

        // Transmit the list of processes back to the client
        if (theList.firstProcess == NULL)
        {
            // If there are no matching processes, a failure message is
            //  returned to the client
            XPCProcessList failProcess("NONE", -1);
            *processListPipe << failProcess;
        }
        else
        {
            theList.currentProcess = theList.firstProcess;
            while(theList.currentProcess != NULL)
            {
                // The XPCProcessListLinked object is cast to its base class 
                // XPCProcessList and transmitted to the client
                *processListPipe << *(XPCProcessList *)theList.currentProcess;
                theList.currentProcess = theList.currentProcess->next;
            }
        }
        // Close the pipe connection
        delete processListPipe;
    }
    catch(XPCException &exceptOb)
    {
        cerr << "iRetrieveProcessList Error: " << exceptOb.sGetException() << endl;
        return 0;
    }
    return 1;
}

DWORD WINAPI iKillProcess(void *_vArg)
{
    XPCPipe<XPCKill> *killPipe = (XPCPipe<XPCKill> *)_vArg;    // XPCPipe object passed in
    XPCKill newRequest(-1);
    XPCKill failKill(-1);

    try
    {
        // Retrieve the client's request
        *killPipe >> newRequest;

        // Open a handle to the process ID contained within the client's message
        HANDLE ps = OpenProcess(PROCESS_TERMINATE, FALSE, newRequest.iGetProcessID());
        if (ps)
        {
            // Terminate the process
            if (TerminateProcess(ps, -1))
                *killPipe << newRequest;    // Confirm successful
                                            // termination by returning the 
                                            // original XPCKill object
            else
                *killPipe << failKill;        // Send XPCKill object with a -1 
                                            // process ID to indicate failure 
                                            // of process termination
        }
        else
        {
            *killPipe << failKill;        // Send XPCKill object with a -1 
                                        // process ID to indicate failure 
                                        // of process termination
        }

        // Close the pipe connection
        delete killPipe;
    }
    catch(XPCException &exceptOb)
    {
        cerr << "iKillProcess Error: " << exceptOb.sGetException() << endl;
        return 1;
    }
} 
