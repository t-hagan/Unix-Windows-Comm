#include <iostream.h>
#include <XPCPipe.h>            // Defines the XPCPipe class
#include <XPCProcessRequest.h>        // Defines the XPCProcessRequest class
#include <XPCSpawn.h>            // Defines the XPCSpawn class
#include <stdlib.h>

#define REMOTE_HOST "aHost"

main()
{
    int iChoice;            // Stores the user's menu choice
    XPCProcessRequest *newRequest;    // The XPCProcessRequest message sent to the 
                                      // ProcessControlServer which contains the user's 
                                      // request
    XPCProcessRequest response;    // Stores the ProcessControlServer's response
    char sAppName[256];        // Stores the name of the application for which the 
                               // user wishes to receive information
    int iProcessID;            // Stores the ID of the process the user wishes to 
                               // terminate
    int iPort;                     // The port number returned by the Spawn Server

    try
    {
        // A request is made of the Spawn Server to spawn the ProcessControlServer
        // on the 'spawn_server_host'
        XPCSpawn newApp((long int)6572, REMOTE_HOST, "d:\\ProcessControlServerSpawn.exe", SPAWN);

        // The socket port number returned by the Spawn Server is stored
        iPort = newApp;
    }
    catch(XPCException &exceptOb)
    {
        // XPCSpawn exceptions are caught and displayed
        cout << "Spawn Error: " << exceptOb.sGetException() << endl;
        return 0;
    }

    if (iPort == -1)
    {
            cerr << "Error spawning: Exiting" << endl;
            return 0;
    }
  
    try
    {
        // A pipe connection is made to the ProcessControlServer using the socket 
        // port
        XPCPipe<XPCProcessRequest> myPipe((long int)iPort, REMOTE_HOST);

       while(1)    // Loop forever
       {
            cout << "Menu:" << endl;
            cout << "(1) Process Info" << endl;
            cout << "(2) Process Kill" << endl;
            cout << "(3) Quit" << endl;
            cout << "Choice: " << flush;
            cin >> iChoice;

            switch(iChoice)
            {
                case 1:
                    // If the user chose to view process information, 
                    // the user is prompted for the name of the process 
                    // or 'ALL' for all processes
                    cout << "Enter process name or ALL: " << flush;
                    gets(sAppName);

                    // A XPCProcessRequest object is constructed given the 
                    // user's process name
                    newRequest = new XPCProcessRequest(sAppName);
                                        
                    // The XPCProcessRequest is piped to the 
                    // ProcessControlServer
                    myPipe << *newRequest;
                                
                    // Messages from the ProcessControlServer are 
                    // received in a loop.  The loop is complete when 
                    // the server sends a XPCProcessRequest message with a 
                    // process ID of -1
                    while(1)
                    {
                          myPipe >> response;
                          int iPid = response.iGetProcessID();
                          if (iPid == -1)
                               break;
                
                        // The process name and process ID are 
                        // displayed to the user
                        cout << response.sGetAppName() << "\t" << iPid << endl;
                    }
                                
                    delete newRequest;      
                    break;  
                case 2:
                    // If the user chose to terminate a process, the 
                    // user is prompted from the ID of the process
                    cout << "Enter process ID: " << flush;
                    cin >> iProcessID;

                    // A XPCProcessRequest object is created with the 
                    // user-inputted process ID
                    newRequest = new XPCProcessRequest(iProcessID);

                    // The XPCProcessRequest object is piped to the 
                    // ProcessControlServer
                    myPipe << *newRequest;

                    delete newRequest;

                    // The ProcessControlServer's response is received
                    myPipe >> response;
                        
                    // If the ProcessControlServer sent a 
                    // XPCProcessControl object with a process ID of -1, 
                    // termination failed otherwise it succeeded.
                    if (response.iGetProcessID() == -1)
                        cout << "Process Kill Failed" << endl;
                    else
                        cout << "Process Kill Successful" << endl;
                    break;
                                case 3:
                    // If the user chose to quit, a XPCProcessRequest
                    // object is created with a request of QUIT.
                    newRequest = new XPCProcessRequest();

                    // The XPCProcessRequest object is piped to the 
                    // ProcessControlServer
                    myPipe << *newRequest;
                                       
                    exit(0);
                    break;
                }
            }
        }
        catch(XPCException &exceptOb)
        {
                // All XPCPipe related exceptions are caught and displayed to the user
                cout << "ProcessControlClient Error: " << exceptOb.sGetException() << endl;
                return 0;
        }
        return 1;
}
