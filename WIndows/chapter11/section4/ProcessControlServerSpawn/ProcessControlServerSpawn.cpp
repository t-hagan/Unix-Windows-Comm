#include <iostream.h>
#include <CProcessRequest.h>
#include <CPipe.h>
#include <CSpawnClass.h>
#include <winuser.h>

#define SPAWN_SERVER "aHost"

// Defines linked list of CProcessRequest objects
class CProcessRequestList : public CProcessRequest
{
public:
    CProcessRequestList *next;
    CProcessRequestList(char *_sAppName, long int _iProcessID) : CProcessRequest(_sAppName, _iProcessID)
    { next = NULL; }
};

// Structure to hold the linked-list of CProcessRequestList objects
struct SProcessList
{
    char sAppName[256];                        // Name of the application being searched
    CProcessRequestList *firstProcess;        // First element in the list
    CProcessRequestList *currentProcess;    // Current element in the list

    // Constructor
    SProcessList() { firstProcess = NULL; }

    // Removes all nodes in the linked-list
    void vClear()
    {
        while(firstProcess != NULL)
        {
            currentProcess = firstProcess;
            firstProcess = firstProcess->next;
            delete currentProcess;
        }
    }
};

BOOL CALLBACK MyEnumFunction(HWND h, LPARAM lpData)
{
    // Callback function that enumerates window processes

    // The structure containing the linked-list is cast to EnumerateProcess's incoming 
    // parameter
    SProcessList *processList = (SProcessList *)lpData;

    LPDWORD lpdw = new DWORD;
    DWORD result;
    char message[256];

    // Get the list of windows processes
    result = GetWindowThreadProcessId(h, lpdw);

    if (GetWindowText(h, message, 256))
    {
        if (strcmp(processList->sAppName, "ALL") != 0)
            if (strcmp(processList->sAppName, message) != 0)
                return TRUE;

        if (processList->firstProcess == NULL)
        {
            processList->firstProcess = new CProcessRequestList(message, *lpdw);
            processList->currentProcess = processList->firstProcess;
        }
        else
        {
            CProcessRequestList *newProcess = new CProcessRequestList(message, *lpdw);
            processList->currentProcess->next = newProcess;
            processList->currentProcess = processList->currentProcess->next;
        }
    }

    return TRUE;
};

main()
{
    SProcessList theList;                    // The linked-list of CProcessRequest objects
    CProcessRequest clientRequest;            // CProcessRequest message received from the client
    CProcessRequest failRequest(-1);        // Fail message sent to the client
    HANDLE ps;                                // A process handle
    int iPid;                                // Process ID fothe process the client requested to kill
    int iPort;                                // The port number returned by the Spawn Server

    // Initialize the WinSock library
    WSADATA wsaData;

    if (WSAStartup(0x101, &wsaData) != 0)
    {
        cout << "Error initializing WinSock library: " << WSAGetLastError() << endl;
        return 0;
    }

    try
    {
        // The ProcessControlServerSpawn is registered with the Spawn Server
        CSpawnClass regApp((long int)6572, SPAWN_SERVER, "d:\\ProcessControlServerSpawn.exe", REGISTER);
        iPort = regApp;
    }
    catch(IPCExceptionClass &exceptOb)
    {
        cout << "Spawn Error: " << exceptOb.sGetException() << endl;
        return 0;
    }
    
    if (iPort == -1)
    {
        cerr << "Error registering the ProcessControlServerSpawn with the Spawn Server" << endl;
        return 1;
    }

    try
    {
        // A CPipe object is created using the socket port returned by the 
        // Spawn Server
        CPipe<CProcessRequest> myPipe((long int)iPort);

        // Accept the client's connection
        CPipe<CProcessRequest> *newPipe = myPipe.vProcessConnection();

        while(1)
        {
            // Read the client's request
            *newPipe >> clientRequest;

            switch(clientRequest.cGetRequest())
            {
                case PROCESS_LIST:
                    // Retrieve the list of processes
                    strcpy(theList.sAppName, clientRequest.sGetAppName());

                    if (!EnumWindows((WNDENUMPROC)MyEnumFunction, (LPARAM)&theList))
                    {
                        // If the enumeration fails return a fail message to the client
                        *newPipe << failRequest;
                        break;
                    }

                    // The linked-list is traversed and each node is sent to the client
                    theList.currentProcess = theList.firstProcess;
                    while(theList.currentProcess != NULL)
                    {
                        CProcessRequest *aRequest = (CProcessRequest *)theList.currentProcess;
                        *newPipe << *aRequest;
                        Sleep(50);
                        theList.currentProcess = theList.currentProcess->next;
                    }
                    // The failRequest object is sent to the client to indicated the
                    // list of processes returned is complete
                    *newPipe << failRequest;

                    // All elements of the linked-list are removed
                    theList.vClear();

                    break;
                case PROCESS_KILL:
                    // The ID of the requested process to kill is stored
                    iPid = clientRequest.iGetProcessID();

                    // A handle to the process is opened
                    ps = OpenProcess(PROCESS_TERMINATE, FALSE, iPid);
                    if (ps)
                    {
                        // The requested process is terminated
                        if (!TerminateProcess(ps, -1))
                        {
                            // If termination fails, a failRequest object is returned to the client
                            *newPipe << failRequest;
                        }
                        else
                        {
                            // If termination succeeds the object the client sent is returned
                            *newPipe << clientRequest;
                        }
                    }
                    else
                        // If a handle to the client failed, a failRequest message is returned
                        *newPipe << failRequest;
                    break;
                case QUIT:
                    // The client sent a QUIT message.  The CPipe object is deleted and the server exits
                    delete newPipe;
                    return 1;
                    break;
            }
        }
    }
    catch(IPCExceptionClass &exceptObject)
    {
                cout << "ProcessControlServerSpawn error: " << exceptObject.sGetException() << endl;
                return 0;
    }
    return 1;
}
