#include <XPCPthread.h>            // Defines the XPCPthread class (Section 2.4)
#include <XPCSemaphore.h>            // Defines the XPCSemaphore class (Section 2.3)
#include <XPCClientConnection.h>        // Defines the XPCClientConnection class
#include <XPCConnectInfo.h>        // Defines the XPCClientInfo class
#include <XPCPort.h>
#include <iostream.h>
#include <stdlib.h>
#include <unistd.h>

static XPCClientConnection *firstClient;    // Pointer to the first element within the 
                                            // XPCClientConnection linked-list

void *vProcessConnect(void *_vArg);    // Declaration for the remote process 
                                       // communicating thread

void vCClientConnectionInsert(char *_sAppName, int _iPid, XPCTcpSocket *_socket)
{
    XPCClientConnection *currentClient;    // Pointer to an element within the 
                                           // XPCClientConnection linked-list

    if (firstClient == NULL)
    {
        // Insert into the beginning of the list
        firstClient = new XPCClientConnection(_sAppName, _iPid, _socket);
        currentClient = firstClient;
    }
    else
    {
        // Get to the end of the list
        currentClient = firstClient;
        while (currentClient->next != NULL)
        {
            currentClient = currentClient->next;
        }

        // Insert at the end of the list
        XPCClientConnection *newClient = new XPCClientConnection(_sAppName, _iPid, _socket);
        currentClient->next = newClient;
    }
}

int iGetPort()
{
    long int iMaxPortNum = 6500;    // Stores the maximum value of chosen port 
                                    // number

    int iNumTries = 0;            // Number of socket port attempts

    // A range of socket ports are tested for availability.  The loop 
    // continues until an available port is found or until the entire 
    // range of ports has been attempted
    do
    {       
        try
        {
            // Create a socket using the next port in the range
            XPCTcpSocket testSocket(iMaxPortNum);

            // Bind the socket
            testSocket.vBindSocket();

            // The socket is closed
            testSocket.vCloseSocket();    

            // If the port number is available, the socket can be bound and an
            // exception is NOT thrown.  The port number is returned
            return iMaxPortNum;
        }
        catch(XPCException &exceptOb)
        {
            // If a socket cannot be bound to the given port,
            // an exception is thrown.  The port number is 
            // decremented for another try
            cout << "Failed Port #:" << iMaxPortNum << endl;
            iMaxPortNum--;
            iNumTries++;
            if (iMaxPortNum < 5000)
                iMaxPortNum = 6500;
        }
    } while (iNumTries == iMaxPortNum - 5000);

    // Return -1 indicating an available port cannot be found
    return -1;
}

void vRemoveNode(XPCClientConnection *currentNode)
{
    XPCClientConnection *previousClient;

    // The client information node is removed from the linked-list
    if (currentNode == firstClient)
    {
        if (firstClient->next == NULL)
        {
            firstClient->next = NULL;
            firstClient = NULL;
        }
        else
        {
            firstClient = firstClient->next;
            delete previousClient;
        }
    }
    else
    {
        previousClient->next = currentNode->next; 
        delete currentNode;
    }
}

main()
{
        firstClient = NULL;            // Initialize the first element of the 
                                       // linked-list to NULL
        
     try
     {
        // Domain semaphore with a starting value of 1 (Unlock) is created.  The     
        // semaphore is used within the spawned threads to protect the 
        // XPCClientConnection linked-list from multiple access
        XPCSemaphore listSem((int)PRIVATE_SEM, (unsigned int)1);

        // New socket is created and bound to port #6572
        XPCTcpSocket SpawnServerSocket((long int)6572);
        SpawnServerSocket.vBindSocket();

        // Connections are listened for
        SpawnServerSocket.vListen(5);

        while(1)    // Loop forever
        {
            // Accept remote process connections.  When a remote process 
            // connects an new XPCTcpSocket object is created.
            XPCTcpSocket *RemoteProcessSocket = SpawnServerSocket.Accept();
                
            // The newly created socket and the semaphore are placed within a 
            // XPCConnectInfo object
            XPCConnectInfo *connectInfo = new XPCConnectInfo(RemoteProcessSocket, &listSem);

            // A thread is created dedicated to communicating with the 
            //connected socket.  The thread is passed the connectInfo object.  
            XPCPthread<int> newConnectThread(vProcessConnect, (void *)connectInfo);              
        }
    }
    catch(XPCException &exceptObject)
    {
        // All semaphore, socket, and thread related exceptions are captured and
        // displayed
        cout << "SpawnServer Error: " << exceptObject.sGetException() << endl;
        return 1;
    }
}

void *vProcessConnect(void *_vArg)
{
    // Thread's argument is cast to a XPCConnectInfo object
    XPCConnectInfo *connectInfo = (XPCConnectInfo *)_vArg;
    int iRet;                // Number of bytes returned from receiving 
                             // or sending data on the socket
    int iPid;                // Process ID of the spawn application
    char sSocketBuf[1024];            // Stores the raw bytes received from the 
                                      // socket
        
    XPCPort aPort;                // Stores the chosen socket port number

    // XPCClientConnection linked-list pointers
    XPCClientConnection *currentClient;
    XPCClientConnection *previousClient;

    try
    {
        // Receive socket data
        iRet = connectInfo->getSocket()->iRecieveMessage((void *)sSocketBuf, sizeof(sSocketBuf));
        if (iRet == 0)
        {
            cout << "Receive Error: No Data Received of Socket" << endl;
            delete connectInfo;
            return 1;
        }       

        // Cast raw data to a XPCSpawnInfo object
        XPCSpawnInfo *spawnData = (XPCSpawnInfo *)sSocketBuf;

        if (spawnData->cGetAction() == SPAWN)
        {
            // If the action from the connected process is SPAWN, an attempt 
            // is made to spawn the requested application                        
            iPid = fork();
            if (iPid == 0)    // This is the child process
            {
                // The requested application is spawned
                if (execlp(spawnData->sGetAppName(), spawnData->sGetAppName(), NULL) == -1)
                {
                    // If the requested application cannot be 
                    // spawned, a XPCPort object is returned to the 
                    // client with a port value of -1 indicating an 
                    // error occured
                    XPCPort failurePort((long int)-1);
                    iRet = connectInfo->getSocket()->iSendMessage((void *)&failurePort, sizeof(XPCPort));
                    if (iRet == 0)
                    {
                        cerr << "Socket Send Error: Error sending failure to client" << endl;
                    }
                    delete connectInfo;
                    return 1;
                }
            }
        
            // Lock the domain semaphore and insert the socket message into 
            // the XPCClientConnection linked-list
            connectInfo->getSem()->vLockWait();    
            vCClientConnectionInsert(spawnData->sGetAppName(), iPid, connectInfo->getSocket());

            // The semaphore is unlocked once the socket message is inserted 
            // into the linked-list
            connectInfo->getSem()->vUnlock();
        }
        else 
        {
            // Since the connected process's action is REGISTER, the socket 
            // data is cast to a XPCServerSpawn object
            XPCServerSpawn *serverSpawn = (XPCServerSpawn *)sSocketBuf;

            // Retrieve an Unused Port
            int iPort = iGetPort();
            aPort.vSetPort(iPort);

            // The domain sempaphore is locked before the XPCClientConnection 
            // linked-list is traversed
            connectInfo->getSem()->vLockWait();

            // The linked-list is traversed until the registered application 
            // name and process ID can be located
            previousClient = currentClient = firstClient;
            while (currentClient != NULL)
            {
                  if (*currentClient == *serverSpawn) 
                  {    
                          // If the client information is found, the XPCPort 
                          // object is sent to both the client and the 
                          // server
                          iRet = connectInfo->getSocket()->iSendMessage((void *)&aPort, sizeof(XPCPort));
                          if (iRet == 0)
                          {
                              cerr << "Error sending XPCPort to client" << endl;
                          }

                          // A sleep is done to insure the server receives 
                          // the socket port and binds to it before the 
                          // client has the change to connect
                          sleep(2);

                          iRet = currentClient->getSocket()->iSendMessage((void *)&aPort, sizeof(XPCPort));
                          if (iRet == 0)
                          {
                              cerr << "Error sending XPCPort to server" << endl;
                          }

                          // Remove the found client node from the linked-list 
                          vRemoveNode(currentClient);
                          break;
                    }
                    previousClient = currentClient;
                    currentClient = currentClient->next;
                }

                // If the client information could not be located, a XPCPort object 
                // containing a -1 is sent to the server indicating failure
                if (currentClient == NULL)
                {
                    XPCPort failurePort((long int)-1);
                    iRet = connectInfo->getSocket()->iSendMessage((void *)&failurePort, sizeof(XPCPort));
                    if (iRet == 0)
                    {
                        cerr << "Error sending socket data" << endl;
                    }
                 }

                 // The domain semaphore protecting the linked-list is unlocked
                 connectInfo->getSem()->vUnlock();
            }       
        }
        catch(XPCException &exceptObject)
        {
            // All socket related errors are caught and displayed to the user
            cout << "Spawn Server Error: " << exceptObject.sGetException() << endl;
        }
        
        delete connectInfo;
}
