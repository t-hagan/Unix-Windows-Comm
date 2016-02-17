#include <XPCMsg.h>
#include <iostream.h>

main(int argc, char *argv[])
{
    char sBuf[256];        // Stores the user-entered values
    
    try
    {
        // A XPCTcpSocket is created which communicates with the Publish
        // Subscribe server
        XPCTcpSocket businessSocket((long int)6800);

        // The socket is connected to the Publish/Subscribe host
        businessSocket.vConnect(argv[1]);

        while(1)
        {
            // The user is prompted for the news messages.  If the user enters "QUIT"
            // the client exits.
            cout << "Enter News Item: " << flush;
            cin.getline(sBuf, 256);
            cout << sBuf << endl;
    
            if (strcmp(sBuf, "QUIT") == 0)
                break;

            // A new XPCMessage object is created
            XPCMessage newMessage(3, "BUSINESS", PUBLISH, &businessSocket);

            // The news item is inserted into the message with a label of News
            newMessage.vInsert("News", sBuf);
        
            // The user is prompted for the source of the news item
            cout << "Enter Source: " << flush;
            cin.getline(sBuf, 256);
            cout << sBuf << endl;

            // The source is inserted into the message with a label of Source
            newMessage.vInsert("Source", sBuf);
    
            // The user is prompted for the priority level of the news item    
            cout << "Enter Priority Level: " << flush;
            cin.getline(sBuf, 256);
            cout << sBuf << endl;

            // The priority level is inserted into the message with a label of Priority
            newMessage.vInsert("Priority", atoi(sBuf));

            // The messages is published
            newMessage.vPublish();

        }
    }
    catch(XPCException &exceptOb)
    {
        cout << exceptOb.sGetException() << endl;
        return 0;
    }

    return 1;
}
