// NewsSubClientDlg.cpp : implementation file
//

#include "stdafx.h"
#include "NewsSubClient.h"
#include "NewsSubClientDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
    CAboutDlg();

// Dialog Data
    //{{AFX_DATA(CAboutDlg)
    enum { IDD = IDD_ABOUTBOX };
    //}}AFX_DATA

    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CAboutDlg)
    protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

// Implementation
protected:
    //{{AFX_MSG(CAboutDlg)
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
    //{{AFX_DATA_INIT(CAboutDlg)
    //}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CAboutDlg)
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
    //{{AFX_MSG_MAP(CAboutDlg)
        // No message handlers
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNewsSubClientDlg dialog

CNewsSubClientDlg::CNewsSubClientDlg(CWnd* pParent /*=NULL*/)
    : CDialog(CNewsSubClientDlg::IDD, pParent)
{
    //{{AFX_DATA_INIT(CNewsSubClientDlg)
        // NOTE: the ClassWizard will add member initialization here
    //}}AFX_DATA_INIT
    // Note that LoadIcon does not require a subsequent DestroyIcon in Win32
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CNewsSubClientDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CNewsSubClientDlg)
    DDX_Control(pDX, IDC_SUBSCRIBED_LIST, m_SubscribedList);
    DDX_Control(pDX, IDC_SUBSCRIBE_LIST, m_SubscribeList);
    DDX_Control(pDX, IDC_MESSAGE_LIST, m_MessageList);
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CNewsSubClientDlg, CDialog)
    //{{AFX_MSG_MAP(CNewsSubClientDlg)
    ON_WM_SYSCOMMAND()
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    ON_BN_CLICKED(IDC_ADD_BUTTON, OnAddButton)
    ON_BN_CLICKED(IDC_REMOVE_BUTTON, OnRemoveButton)
    ON_BN_CLICKED(IDC_EXIT_BUTTON, OnExitButton)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNewsSubClientDlg message handlers

BOOL CNewsSubClientDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    // Add "About..." menu item to system menu.

    // IDM_ABOUTBOX must be in the system command range.
    ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
    ASSERT(IDM_ABOUTBOX < 0xF000);

    CMenu* pSysMenu = GetSystemMenu(FALSE);
    if (pSysMenu != NULL)
    {
        CString strAboutMenu;
        strAboutMenu.LoadString(IDS_ABOUTBOX);
        if (!strAboutMenu.IsEmpty())
        {
            pSysMenu->AppendMenu(MF_SEPARATOR);
            pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
        }
    }

    // Set the icon for this dialog.  The framework does this automatically
    //  when the application's main window is not a dialog
    SetIcon(m_hIcon, TRUE);            // Set big icon
    SetIcon(m_hIcon, FALSE);        // Set small icon
    
    // Subscriptions are added to the list of available subscriptions
    m_SubscribeList.AddString("BUSINESS");
    m_SubscribeList.AddString("TECH");

    // Initialize the WinSock library
    AfxSocketInit();

    return TRUE;  // return TRUE  unless you set the focus to a control
}

void CNewsSubClientDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
    if ((nID & 0xFFF0) == IDM_ABOUTBOX)
    {
        CAboutDlg dlgAbout;
        dlgAbout.DoModal();
    }
    else
    {
        CDialog::OnSysCommand(nID, lParam);
    }
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CNewsSubClientDlg::OnPaint() 
{
    if (IsIconic())
    {
        CPaintDC dc(this); // device context for painting

        SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

        // Center icon in client rectangle
        int cxIcon = GetSystemMetrics(SM_CXICON);
        int cyIcon = GetSystemMetrics(SM_CYICON);
        CRect rect;
        GetClientRect(&rect);
        int x = (rect.Width() - cxIcon + 1) / 2;
        int y = (rect.Height() - cyIcon + 1) / 2;

        // Draw the icon
        dc.DrawIcon(x, y, m_hIcon);
    }
    else
    {
        CDialog::OnPaint();
    }
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CNewsSubClientDlg::OnQueryDragIcon()
{
    return (HCURSOR) m_hIcon;
}

void CNewsSubClientDlg::OnAddButton() 
{
    CString sSubscription;        // Stores the name of the subscribed message chosen 
                                // from m_SubscribeList
    try
    {
        // The user-chosen message is extracted from the m_SubscribeList
        int iCurSel = m_SubscribeList.GetCurSel();
        if (iCurSel == LB_ERR)
        {
            MessageBox("A Subscription Must Be Chosen", "Subscribe Error", MB_OK);
            return;
        }

        m_SubscribeList.GetText(iCurSel, sSubscription);
    
        if (sSubscription == "BUSINESS")
        {
            // If the user subscribed to BUSINESS, BusSocket is constructed
            // using socket port #6800 and connected to the PubSubServer
            BusSocket = new XPCTcpSocket((long int)6800);
            BusSocket->vConnect(PUBSUBSERVER_HOST);

            // The BusAsyncSocket object is constructed using BusSocket as its 
            // underlying means of communication.  BusAsyncSocket is defined to 
            // retrieve XPCHeader objects with a specification of MSG_WAITALL.  
            // MSG_WAITALL prevents the vProcessMessage() method from being 
            // called until the entire XPCHeader object is received.
            BusAsyncSocket = new XPCAsyncTcpSocket<CNewsSubClientDlg>((CNewsSubClientDlg *)this, (short int)BusSocket->iGetSocketFd(), sizeof(XPCHeader), MSG_WAITALL);

            // BusSocket is attached to the Windows queue
            BusAsyncSocket->vAttachSocket();

            // A XPCMessage object is constructed using the name of the user-chosen 
            // message and a type of SUBSCRIBE.
            XPCMessage Subscribe(0, (char *)(const char *)sSubscription, SUBSCRIBE, BusSocket);
            
            // The user-chosen message is subscribed
            Subscribe.vSubscribe();
        }
        else
        {
            // If the user subscribed to TECH, TechSocket is constructed
            // using socket port #6800 and connected to the PubSubServer
            TechSocket = new XPCTcpSocket((long int)6800);
            TechSocket->vConnect(PUBSUBSERVER_HOST);

            // The TechAsyncSocket object is constructed using TechSocket as
            // its underlying means of communication.  TechAsyncSocket is 
            // defined to retrieve XPCHeader objects with a specification of 
            // MSG_WAITALL. MSG_WAITALL prevents the vProcessMessage() method
            //  from being called until the entire XPCHeader object is received.
            TechAsyncSocket = new XPCAsyncTcpSocket<CNewsSubClientDlg>((CNewsSubClientDlg *)this, (short int)TechSocket->iGetSocketFd(), sizeof(XPCHeader), MSG_WAITALL);
            TechAsyncSocket->vAttachSocket();

            // A XPCMessage object is constructed using the name of the user-chosen 
            // message and a type of SUBSCRIBE.
            XPCMessage Subscribe(0, (char *)(const char *)sSubscription, SUBSCRIBE, TechSocket);
            
            // The user-chosen message is subscribed
            Subscribe.vSubscribe();
        }

        // If the message is successfully subscribed, it is removed from 
        // m_SubscribeList and added to m_SubscribedList
        m_SubscribeList.DeleteString(iCurSel);
        m_SubscribedList.AddString(sSubscription);
    }
    catch(XPCException &exceptObject)
    {
        // All communication errors are caught and displayed to the user
        MessageBox(exceptObject.sGetException(), "Communication Error", MB_OK);
        return;
    }
}

void CNewsSubClientDlg::OnRemoveButton() 
{
    CString sSubscription;        // Stores the name of the message that is being 
                                // unsubscribed

    try
    {
        // The user-chosen message is extracted from m_SubscribedList
        int iCurSel = m_SubscribedList.GetCurSel();
        if (iCurSel == LB_ERR)
        {
            MessageBox("A Subscription Must Be Chosen", "UnSubscribe Error", MB_OK);
            return;
        }
        m_SubscribedList.GetText(iCurSel, sSubscription);

        // If the subscription to unsubscribe is BUSINESS the BusAsyncSocket and 
        // BusSocket objects are deleted.  Otherwise the TechAsyncSocket and 
        // TechSocket objects are deleted.  Deletion causes the specific socket 
        // connection with PubSubServer to terminate and therefore the News 
        // Subscription client receives no more messages.
        if (sSubscription == "BUSINESS")
        {
            delete BusAsyncSocket;
            delete BusSocket;
        }
        else
        {
            delete TechAsyncSocket;
            delete TechSocket;
        }

        // The message unsubscribed from is removed from m_SubscribeList and added 
        // to m_SubscribedList
        m_SubscribeList.AddString(sSubscription);
        m_SubscribedList.DeleteString(iCurSel);
    }
    catch(XPCException &exceptObject)
    {
        // All communication errors are caught and displayed to the user
        MessageBox(exceptObject.sGetException(), "Communication Error", MB_OK);
        return;
    }
}

void vProcessNews(XPCTcpSocket *_Socket, int _iNumComponents, char *_sMsgName)
{
    // A pointer to the main dialog window is retrieved
    CNewsSubClientDlg *dlgPtr = (CNewsSubClientDlg *)AfxGetMainWnd();
    try
    {
        // The remainder of the message is retrieved
        XPCMessage newMessage(_iNumComponents, _Socket, _sMsgName);

        char sNewsItem[1024];        // Holds the formatted news message
        int iPriority;                // Holds the priority value of the news 
                                    // message
        char sPriority[256];        // Holds the news priority string 
                                    // representation
        char sNews[256];            // Holds the news message
        char sSource[256];            // Holds the news source

         // The priority of the news item is extracted.
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

         // The entire news message is formatted and displayed to the user.
        sprintf(sNewsItem, "%s : From %s Issued by %s - %s", sPriority, _sMsgName, sSource, sNews);

         // The formatted news message is inserted into the m_MessageList list box
        dlgPtr->m_MessageList.InsertString(0, sNewsItem);
    }
    catch(XPCException &exceptOb)
    {
        // All message processing errors are caught and displayed to the user
        char sMsg[512];

        sprintf(sMsg, "Error receiving news message from %s: %s", _sMsgName, exceptOb.sGetException());
        dlgPtr->MessageBox(sMsg, "News Error", MB_OK); 
    }
}

void CNewsSubClientDlg::vProcessMessage(char *sMsg, int iNumBytes)
{
    // The character buffer sent from XPCAsyncTcpSocket is cast to a XPCHeader object
    XPCHeader *Header = (XPCHeader *)sMsg;
    
    if (strcmp(Header->sGetSubscription(), "BUSINESS") == 0)
    {
        // If the message name contained within Header is BUSINESS, the 
        // vProcessNews() function is called to process the remainder of the 
        // message. vProcessNews() is passed a pointer to BusSocket.
        vProcessNews(BusSocket, Header->iGetNumComponents(), Header->sGetSubscription());
    }
    else
    {
        // If the message name contained within Header is TECH, the 
        // vProcessNews() function is called to process the remainder of the 
        // message. vProcessNews() is passed a pointer to TechSocket.
        vProcessNews(TechSocket, Header->iGetNumComponents(), Header->sGetSubscription());
    }
}

void CNewsSubClientDlg::vProcessError(char *sErrMsg)
{
    MessageBox(sErrMsg, "Communication Error", MB_OK);
}

void CNewsSubClientDlg::OnExitButton() 
{
    CDialog::OnOK();
}
