// UDPAsyncDlg.cpp : implementation file
//

#include "stdafx.h"
#include "UDPAsync.h"
#include "UDPAsyncDlg.h"

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
// CUDPAsyncDlg dialog

CUDPAsyncDlg::CUDPAsyncDlg(CWnd* pParent /*=NULL*/)
    : CDialog(CUDPAsyncDlg::IDD, pParent)
{
    //{{AFX_DATA_INIT(CUDPAsyncDlg)
        // NOTE: the ClassWizard will add member initialization here
    //}}AFX_DATA_INIT
    // Note that LoadIcon does not require a subsequent DestroyIcon in Win32
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CUDPAsyncDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CUDPAsyncDlg)
    DDX_Control(pDX, IDC_USER_EDIT, m_UserEdit);
    DDX_Control(pDX, IDC_TICKER_EDIT, m_TickerEdit);
    DDX_Control(pDX, IDC_QUANT_EDIT, m_QuantEdit);
    DDX_Control(pDX, IDC_POSITION_LIST, m_PositionList);
    DDX_Control(pDX, IDC_BS_EDIT, m_BsEdit);
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CUDPAsyncDlg, CDialog)
    //{{AFX_MSG_MAP(CUDPAsyncDlg)
    ON_WM_SYSCOMMAND()
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    ON_BN_CLICKED(IDC_EXIT_BUTTON, OnExitButton)
    ON_BN_CLICKED(IDC_EXECUTE_BUTTON, OnExecuteButton)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CUDPAsyncDlg message handlers

BOOL CUDPAsyncDlg::OnInitDialog()
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
    
    // The windows socket library is initialized.
    AfxSocketInit();

    // The instance of the CAsyncUdpSocket is created.  
    try
    {
        portfolioSocket = new XPCAsyncUdpSocket<CUDPAsyncDlg>(this, UNIX_PORT_NUM);
        portfolioSocket->vBindAsyncSocket();
    }
    catch(XPCException &exceptObject)
    {
        // An exception is thrown if a failure occurs creating and binding the
        // socket and the application exits.
        MessageBox(exceptObject.sGetException(), "Socket Error", MB_OK);
        CDialog::OnOK();
    }

    // The number of positions within the portfolio is initialized to 0.
    iTotalPositions = 0;

    return TRUE;  // return TRUE  unless you set the focus to a control
}

void CUDPAsyncDlg::vProcessError(char *_sMsg)
{
    MessageBox(_sMsg, "Socket Error", MB_OK);
}

void CUDPAsyncDlg::vProcessMessage(char *sMsg, int _iNumBytes)
{
    // The received buffer is casted to a CTransaction instance.
    XPCTransaction *incomingTransaction = (XPCTransaction *)sMsg;

    // The vUpdatePositionList method is called with the value of the position update.
    vUpdatePositionList(incomingTransaction);
}

void CUDPAsyncDlg::vUpdatePositionList(XPCTransaction *incomingTransaction)
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
            portfolio[iCount].vSetAmount(incomingTransaction->iGetAmount());
            m_PositionList.DeleteString(iCount);
            sprintf(sTickermsg, "%s %d\0", portfolio[iCount].sGetTicker(), incomingTransaction->iGetAmount());
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
        portfolio[iTotalPositions].vSetAmount(incomingTransaction->iGetAmount());
        sprintf(sTickermsg, "%s %d", portfolio[iCount].sGetTicker(), incomingTransaction->iGetAmount());
        m_PositionList.InsertString(-1, sTickermsg);
        iTotalPositions++;
    }
}

void CUDPAsyncDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CUDPAsyncDlg::OnPaint() 
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
HCURSOR CUDPAsyncDlg::OnQueryDragIcon()
{
    return (HCURSOR) m_hIcon;
}

void CUDPAsyncDlg::OnExitButton() 
{
    CDialog::OnOK();    
}

void CUDPAsyncDlg::OnExecuteButton() 
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
    
    // The transaction record is sent to the position tracking server.  There is no
    // need to thread this call since sending messages using UDP sockets is 
    // non-blocking.
    portfolioSocket->iSendMessage((void *)&newTransaction, sizeof(newTransaction), UNIX_SERVER);    
}
