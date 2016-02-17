// CTCPAsyncDlg.cpp : implementation file
//

#include "stdafx.h"
#include "CTCPAsync.h"
#include "CTCPAsyncDlg.h"

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
// CCTCPAsyncDlg dialog

CTCPAsyncDlg::CTCPAsyncDlg(CWnd* pParent /*=NULL*/)
    : CDialog(CTCPAsyncDlg::IDD, pParent)
{
    //{{AFX_DATA_INIT(CTCPAsyncDlg)
        // NOTE: the ClassWizard will add member initialization here
    //}}AFX_DATA_INIT
    // Note that LoadIcon does not require a subsequent DestroyIcon in Win32
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTCPAsyncDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CTCPAsyncDlg)
    DDX_Control(pDX, IDC_USER_EDIT, m_UserEdit);
    DDX_Control(pDX, IDC_TICKER_EDIT, m_TickerEdit);
    DDX_Control(pDX, IDC_QUANT_EDIT, m_QuantEdit);
    DDX_Control(pDX, IDC_POSITION_LIST, m_PositionList);
    DDX_Control(pDX, IDC_BS_EDIT, m_BsEdit);
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CTCPAsyncDlg, CDialog)
    //{{AFX_MSG_MAP(CTCPAsyncDlg)
    ON_WM_SYSCOMMAND()
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    ON_BN_CLICKED(IDC_EXIT_BUTTON, OnExitButton)
    ON_BN_CLICKED(IDC_EXECUTE_BUTTON, OnExecuteButton)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCTCPAsyncDlg message handlers

BOOL CTCPAsyncDlg::OnInitDialog()
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

    // Disable all CStockPositionDlg data entry Windows until the socket is connected 
    // to the server.  This prevents data being sent on an unconnected TCP socket.
    m_TickerEdit.EnableWindow(FALSE);
    m_UserEdit.EnableWindow(FALSE);
    m_QuantEdit.EnableWindow(FALSE);
    m_BsEdit.EnableWindow(FALSE);

    // The socket library is initialized.
    AfxSocketInit();

    iTotalPositions = 0;
    iTotalPrices = 0;

    try
    {
        priceSocket = new XPCAsyncTcpSocket<CTCPAsyncDlg>(this, (long int)PRICE_PORTNUM);
        priceSocket->vBindSocket();
        priceSocket->vAttachSocket();
        priceSocket->vListen(5);

        portfolioSocket = new XPCAsyncTcpSocket<CTCPAsyncDlg>(this, (long int)PORTFOLIO_PORTNUM);
        portfolioSocket->vAttachSocket();
        portfolioSocket->vConnect(UNIX_SERVER);
    }
    catch(XPCException &exceptObject)
    {
        if (strcmp(exceptObject.sGetException(), "Error Connecting To Socket. 10035"))
        {
            MessageBox(exceptObject.sGetException(), "Error", MB_OK);
            return 0;
        }
    }

    return TRUE;  // return TRUE  unless you set the focus to a control
}

void CTCPAsyncDlg::vProcessMessage(char *_sMsg, int _iNumbytes)
{
    if (*_sMsg == POSITION_MESSAGE)
    {
        XPCTransaction *incomingTransaction = (XPCTransaction *)_sMsg;
        vUpdatePositionList(incomingTransaction);
    }
    else if (*_sMsg == PRICE_MESSAGE)
    {
        XPCPrice *incomingPrice = (XPCPrice *)_sMsg;
        vUpdatePricesList(incomingPrice);
    }
    else
        return;
}

void CTCPAsyncDlg::vProcessError(char *_sErrMsg)
{
    if (strcmp(_sErrMsg, "Error Connecting To Socket: 10035"))
        MessageBox(_sErrMsg, "Socket Error", MB_OK);
}

void CTCPAsyncDlg::vProcessConnection(sockaddr_in *socketAddress)
{
    // If the connection is successful, enable all stock trade entry windows.
    if (socketAddress != NULL)
    {
        m_TickerEdit.EnableWindow(TRUE);
        m_UserEdit.EnableWindow(TRUE);
        m_QuantEdit.EnableWindow(TRUE);
        m_BsEdit.EnableWindow(TRUE);
    }
}

void CTCPAsyncDlg::vUpdatePositionList(XPCTransaction *incomingTransaction)
{
    char sTickermsg[512];        // A string containing the ticker and position 
    int iFound = 0;                // A flag indicating whether or not the ticker 
                                // position already exists in the list box.

    // Search the list of tickers to determine whether or not the ticker position
    // exists in the listbox.
    for (int iCount = 0; iCount < iTotalPositions; iCount++)
    {
        // If the ticker position exists, the position is updated with the 
        // position returned from the server and is displayed in the listbox.
        if (strcmp(incomingTransaction->sGetTicker(), portfolio[iCount].sGetTicker()) == 0)
        {
            portfolio[iCount].vSetAmount(incomingTransaction->iGetOrigAmount());
            m_PositionList.DeleteString(iCount);
            sprintf(sTickermsg, "%s %d %.02f\0", portfolio[iCount].sGetTicker(), incomingTransaction->iGetAmount(), incomingTransaction->iGetAmount() * dGetPrice(incomingTransaction->sGetTicker()));
            m_PositionList.InsertString(iCount, sTickermsg);

            // The iFound flag is set to 1 indicating the position is found in
            // the portfolio list.
            iFound = 1;
        }
    }

    // If the ticker position does not exist in the portfolio list, it is added and
    // the ticker and position is displayed within the listbox.
    if (!iFound)
    {
        portfolio[iTotalPositions].vSetTicker(incomingTransaction->sGetTicker());
        portfolio[iTotalPositions].vSetAmount(incomingTransaction->iGetOrigAmount());
        sprintf(sTickermsg, "%s %d %.02f", portfolio[iCount].sGetTicker(), incomingTransaction->iGetAmount(), incomingTransaction->iGetAmount() * dGetPrice(incomingTransaction->sGetTicker()));
        m_PositionList.InsertString(-1, sTickermsg);
        iTotalPositions++;
    }
}

void CTCPAsyncDlg::vUpdatePricesList(XPCPrice *incomingPrice)
{
    int iFound = 0;

    for (int iCount = 0; iCount < iTotalPrices; iCount++)
    {
        if (strcmp(incomingPrice->sGetTicker(), prices[iCount].sGetTicker()) == 0)
        {
            prices[iCount].vSetPrice(incomingPrice->dGetOrigPrice());
            iFound = 1;
            break;
        }
    }

    if (!iFound)
    {
        prices[iCount].vSetTicker(incomingPrice->sGetTicker());
        prices[iCount].vSetPrice(incomingPrice->dGetOrigPrice());
        iTotalPrices++;
    }

    for (iCount = 0; iCount < iTotalPositions; iCount++)
    {
        if (strcmp(incomingPrice->sGetTicker(), portfolio[iCount].sGetTicker()) == 0)
        {
            vUpdatePositionList(&portfolio[iCount]);
            break;
        }
    }
}

double CTCPAsyncDlg::dGetPrice(char *_sTicker)
{
    for (int iCount = 0; iCount < iTotalPrices; iCount++)
    {
        if (strcmp(_sTicker, prices[iCount].sGetTicker()) == 0)
        {
            return prices[iCount].dGetPrice();
        }
    }

    return 0.0;
}

void CTCPAsyncDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CTCPAsyncDlg::OnPaint() 
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
HCURSOR CTCPAsyncDlg::OnQueryDragIcon()
{
    return (HCURSOR) m_hIcon;
}

void CTCPAsyncDlg::OnExitButton() 
{
    CDialog::OnOK();    
}

void CTCPAsyncDlg::OnExecuteButton() 
{
    CString username;    // Stores the portfolio username extracted from the 
                        // "UserName" Edit Control.
    CString ticker;        // Stores the ticker symbol extracted from the "Ticker" 
                        // Edit Control.
    CString bscode;        // Stores the Buy/Sell code extracted from the "Buy/Sell" 
                        // Edit Control.
    CString quantity;    // Stores the trade quantity extracted from the "Number of 
                        // Shares" Edit Control.

    // The stock trade related data is extracted from the edit controls and copied 
    // into the transaction record instance.
    m_UserEdit.GetWindowText(username);
    m_TickerEdit.GetWindowText(ticker);
    m_BsEdit.GetWindowText(bscode);
    m_QuantEdit.GetWindowText(quantity);

    // The user-supplied data is used to create an instance of the stock transaction 
    // record.
    XPCTransaction newTransaction((char *)(const char *)username, (char *)(const char *)ticker, bscode[0], atoi(quantity));
    
    try
    {
        // The transaction record is sent to the position-tracking server.  
        int iNumBytes = portfolioSocket->iSendMessage((void *)&newTransaction, sizeof(newTransaction));

        // If the call to the iSendMessage method resulted in a socket 
        // error indicating a broken client / server connection, "Stock 
        // Trade Entry" Window components are disabled and the thread 
        // returns.
        if ((iNumBytes == 0) || (WSAGetLastError() == WSAECONNRESET))
        {
            m_TickerEdit.EnableWindow(FALSE);
            m_UserEdit.EnableWindow(FALSE);
            m_QuantEdit.EnableWindow(FALSE);
            m_BsEdit.EnableWindow(FALSE);
            return;
        }    
    }
    catch(XPCException &exceptOb)
    {
        MessageBox(exceptOb.sGetException(), "Send Error", MB_OK);
    }    
}
