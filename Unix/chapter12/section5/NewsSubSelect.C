#include <XPCMsg.h>
#include <iostream.h>
#include <Priority.h>

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

main(int argc, char *argv[])
{
    char sSubscription[50];        // Stores the user-chosen subscription
    XPCMessage *Subscribe;        // Defines the XPCMessage subscription object
    XPCHeader Header;            // Stores the published XPCHeader object
    int iMaxSocketFd = 0;        // Stores the maximum file descriptor value
    fd_set fdset;            // Stores the set of socket file descriptors

    try
    {
        // TCP socket is setup to subscribe to business messages
        XPCTcpSocket BusSubscriptionSocket((long int)6800);
        BusSubscriptionSocket.vConnect(argv[1]);

        // TCP socket is setup to subscribe to tech messages
        XPCTcpSocket TechSubscriptionSocket((long int)6800);
        TechSubscriptionSocket.vConnect(argv[1]);

        // The largest file descriptor is chosen and stored
        if (BusSubscriptionSocket.iGetSocketFd() > TechSubscriptionSocket.iGetSocketFd())
            iMaxSocketFd = BusSubscriptionSocket.iGetSocketFd() + 1;
        else
            iMaxSocketFd = TechSubscriptionSocket.iGetSocketFd() + 1;    

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
            
        while(1)
        {
            FD_ZERO(&fdset);    // The set of file descriptors is cleared

            // The business and tech socket file descriptors are added to the set
            FD_SET(BusSubscriptionSocket.iGetSocketFd(), &fdset);
            FD_SET(TechSubscriptionSocket.iGetSocketFd(), &fdset);

            // Asynchronous selection of sockets that have incoming message is established
            int iSelectRetValue = select(iMaxSocketFd, &fdset, NULL, NULL, 0);
            if (iSelectRetValue == -1)
            {
                cerr << "Select failed" << endl;
                return 1;
            }

            // If an incoming message is detected, the correct XPCTcpSocketObject is chosen and
            // is used to receive the message
            for (int iCount = 0; iCount < iSelectRetValue; iCount++)
            {
                if (FD_ISSET(BusSubscriptionSocket.iGetSocketFd(), &fdset) != 0)
                {    
                    // The message received is on the business related socket
                    XPCMessage::vGetMessages(&BusSubscriptionSocket, Header, vProcessNews);
                }
                else if (FD_ISSET(TechSubscriptionSocket.iGetSocketFd(), &fdset) != 0)
                {
                    // The message received is on the tech related socket
                    XPCMessage::vGetMessages(&TechSubscriptionSocket, Header, vProcessNews);
                }
            }
        }    
    }
    catch(XPCException &exceptOb)
    {
        cout << "Communication Error: " << exceptOb.sGetException() << endl;
        return 0;
    }

    return 1;
}
