#include <XPCMsg.h>            // Defines the XPCMsg object
#include <iostream.h>

int main(int argc, char *argv[])
{
    char sBuf[256];        // Stores the user-entered values

    // Initialize the WinSock library
    WSADATA wsaData;
    if (WSAStartup(0x101, &wsaData) != 0)
    {
        cout << "Error initializing WinSock library: " << WSAGetLastError() << endl;
        return 0;
    }

    try
    {
        // A XPCTcpSocket object is created using socket port #6800 and 
        // connected to the host specified on the command-line.  The host 
        // specified must be executing the PubSubServer application
        XPCTcpSocket techSocket((long int)6800);
        techSocket.vConnect(argv[1]);

        while(1)    // Loop forever
        {
            // The user is prompted for the news message.  If the user enters 
            // "QUIT", the client exits.
            cout << "Enter News Item: " << flush;
            cin.getline(sBuf, 256);

            if (strcmp(sBuf, "QUIT") == 0)
                    break;

            // A new XPCMessage object is created.  Three CBodyComponent 
            // objects are specified and the message is given the name of 
            // "TECH".
            XPCMessage newMessage(3, "TECH", PUBLISH, &techSocket);

            // The user-entered news item is inserted into the XPCMessage 
            // object and is given a label of "News"
            newMessage.vInsert("News", sBuf);
            
            // The source of the news item is prompted for.
            cout << "Enter Source: " << flush;
            cin.getline(sBuf, 256);

            // The user-entered source is inserted into the XPCMessage
            // object and is given a label of "Source"
            newMessage.vInsert("Source", sBuf);
    
            // The priority level of the news item is prompted for.
            cout << "Enter Priority Level: " << flush;
            cin.getline(sBuf, 256);

            // The user-entered priority is converted to an integer and
            // inserted into the XPCMessage object.  The priority is given a
            // label of "Priority"
            newMessage.vInsert("Priority", atoi(sBuf));     
            
            // The XPCMessage object is published
            newMessage.vPublish();
        }
    }
    catch(XPCException &exceptOb)
    {
        // All socket communication related exceptions are caught and displayed to
        // the user.
        cout << "Communication Error: " << exceptOb.sGetException() << endl;
        return 0;
    }

    return 1;
}
