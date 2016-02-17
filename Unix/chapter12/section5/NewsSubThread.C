#include <XPCMsg.h>
#include <iostream.h>
#include <Priority.h>
#ifdef UNIX
    #include <XPCPthread.h>
#else
    #include <XPCThread.h>
#endif

void vProcessNews(XPCTcpSocket *_Socket, int _iNumComponents, char *_sMsgName)
{
        try
        {
                // A XPCMessage object is constructed using message components sent
                // over the network
                XPCMessage newMessage(_iNumComponents, _Socket, _sMsgName);

                char sNewsItem[1024];   // Holds the formatted news message
                int iPriority;          // Holds the priority value of the news message
                char sPriority[256];    // Holds the news priority string representation
                char sNews[256];        // Holds the news message
                char sSource[256];      // Holds the news source

                // The priority of the news is extracted
                if (!newMessage.iGetValueByName("Priority", &iPriority))
                        iPriority = UNKNOWN;

                // The news message is extracted
                if (!newMessage.iGetValueByName("News", sNews))
                        strcpy(sNews, "UNKNOWN");

                // The news source is extracted
                if (!newMessage.iGetValueByName("Source", sSource))
                        strcpy(sSource, "UNKNOWN");

                // A string representation of the priority is stored based on the
                // priority value extracted from the XPCMessage object
                switch(iPriority)
                {
                        case UNKNOWN:
                                strcpy(sPriority, "UNKNOWN");
                                break;
                        case CRITICAL:
                                strcpy(sPriority, "Critical");
                                break;
                        case IMPORTANT:
                                strcpy(sPriority, "Important");
                                break;
                        case INFORMATIVE:
                                strcpy(sPriority, "Informative");
                                break;
                }
                // The entire news message is formatted and displayed to the user
                sprintf(sNewsItem, "%s : From %s Issued by %s - %s", sPriority, _sMsgName, sSource, sNews);
                cout << sNewsItem << endl;
        }
        catch(XPCException &exceptOb)
        {
                cerr << "Error receiving news message from " << _sMsgName << ": " << exceptOb.sGetException() << endl;
        }
}

#ifdef UNIX
void *vSubscribeThread(void *vArg)
#else
DWORD WINAPI vSubscribeThread(void *vArg);
#endif
{    
    // Cast the thread's argument back to a XPCTcpSocket object
    XPCTcpSocket *Socket = (XPCTcpSocket *)vArg;

    // A blank header is created
    XPCHeader Header;

    try
    {
        while(1)
        {
            // Process the incoming message
            XPCMessage::vGetMessages(Socket, Header, vProcessNews);
        }
    }
    catch(XPCException &exceptOb)
    {
        cout << "Subscribe Error: " << exceptOb.sGetException() << endl;
        return 0;
    }

    return 1;
}

main(int argc, char *argv[])
{
    char sSubscription[50];        // Stores the user-chosen subscription
    XPCMessage *Subscribe;        // Defines the XPCMessage subscription object

    try
    {
        // TCP socket is setup to subscribe to business messages
        XPCTcpSocket BusSubscriptionSocket((long int)6800);
        BusSubscriptionSocket.vConnect(argv[1]);

        // TCP socket is setup to subscribe to tech messages
        XPCTcpSocket TechSubscriptionSocket((long int)6800);
        TechSubscriptionSocket.vConnect(argv[1]);

        // The user is prompted for the name of the message to which to subscribe
        cout << "Enter The Message You Wish to Subscribe (BUSINESS, TECH, ALL): " << flush;
        cin >> sSubscription;

        // If the name chosen is ALL or BUSINESS, the BUSINESS message is subscribed
        if ((strcmp(sSubscription, "ALL") == 0) || ((strcmp(sSubscription, "BUSINESS") == 0)))
        {
            Subscribe = new XPCMessage(0, "BUSINESS", SUBSCRIBE, &BusSubscriptionSocket);
            Subscribe->vSubscribe();
            delete Subscribe;
        }

        // If the name chosen is ALL or TECH, the TECH message is subscribed
        if ((strcmp(sSubscription, "ALL") == 0) || ((strcmp(sSubscription, "TECH") == 0)))
        {
            Subscribe = new XPCMessage(0, "TECH", SUBSCRIBE, &TechSubscriptionSocket); 
            Subscribe->vSubscribe();
            delete Subscribe;
        }

        // Threads are spawned to process the two messages
#ifdef UNIX    
        XPCPthread<int> BusinessThread(vSubscribeThread, (void *)&BusSubscriptionSocket);
        XPCPthread<int> TechThread(vSubscribeThread, (void *)&TechSubscriptionSocket);
#else
        XPCThread BusinessThread(vSubscribeThread, (void *)&BusSubscriptionSocket);
        XPCThread TechThread(vSubscribeThread, (void *)&TechSubscriptionSocket);
#endif
        BusinessThread.vWaitForThread();
        TechThread.vWaitForThread();            
    }
    catch(XPCException &exceptOb)
    {
        cout << exceptOb.sGetException() << endl;
        return 0;
    }

    return 1;
}
