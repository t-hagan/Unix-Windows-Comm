// PurchaseSystemDlg.cpp : implementation file
//

#include "stdafx.h"
#include "PurchaseSystem.h"
#include "PurchaseSystemDlg.h"

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
// CPurchaseSystemDlg dialog

CPurchaseSystemDlg::CPurchaseSystemDlg(CWnd* pParent /*=NULL*/)
    : CDialog(CPurchaseSystemDlg::IDD, pParent)
{
    //{{AFX_DATA_INIT(CPurchaseSystemDlg)
        // NOTE: the ClassWizard will add member initialization here
    //}}AFX_DATA_INIT
    // Note that LoadIcon does not require a subsequent DestroyIcon in Win32
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CPurchaseSystemDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CPurchaseSystemDlg)
    DDX_Control(pDX, IDC_WIDGET_EDIT, m_WidgetEdit);
    DDX_Control(pDX, IDC_SPROCKET_EDIT, m_SprocketEdit);
    DDX_Control(pDX, IDC_CONFIRM_EDIT, m_ConfirmEdit);
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CPurchaseSystemDlg, CDialog)
    //{{AFX_MSG_MAP(CPurchaseSystemDlg)
    ON_WM_SYSCOMMAND()
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    ON_BN_CLICKED(IDC_EXIT_BUTTON, OnExitButton)
    ON_BN_CLICKED(IDC_ORDER_BUTTON, OnOrderButton)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPurchaseSystemDlg message handlers

BOOL CPurchaseSystemDlg::OnInitDialog()
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
    
    // Connect to the shared memory server
    memcl = new CLIENT;
    memcl = clnt_create(SHARED_MEM_SERVER, crossplatformmem, crossplatformmemver, "tcp");
    if (memcl == NULL)
    {
        char *sErrorMsg = clnt_spcreateerror("Error connecting to the shared memory server");
        MessageBox(sErrorMsg, "Connection Error", MB_OK);
        CDialog::OnOK();

    }

    // Connect to the semaphore server
    semcl = new CLIENT;
    semcl = clnt_create(SEMAPHORE_SERVER, crossplatformsem, crossplatformsemver, "tcp");
    if (semcl == NULL)
    {
        char *sErrorMsg = clnt_spcreateerror("Error connecting to the semaphore server");
        MessageBox(sErrorMsg, "Connection Error", MB_OK);
        CDialog::OnOK();
    }

    // Setup the transaction shared memory write buffer
    writeTransactMem.ikey = 7000;
    writeTransactMem.data.sharedmem_len = sizeof(XPCTransaction);
    writeTransactMem.data.sharedmem_val = new char[sizeof(XPCTransaction)];

    // Setup the confirmation shared memory segment access information
    confirmMem.ikey = 7001;
    confirmMem.isize = sizeof(XPCConfirmation);
    
    // Setup the cross-platform semaphore names.  Since the Semaphore Server is 
    // Windows NT based there is no need to preceed the name with a '/'
    transactSem = "TRANSACTION";
    confirmSem = "CONFIRMATION";

    return TRUE;  // return TRUE  unless you set the focus to a control
}

void CPurchaseSystemDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CPurchaseSystemDlg::OnPaint() 
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
HCURSOR CPurchaseSystemDlg::OnQueryDragIcon()
{
    return (HCURSOR) m_hIcon;
}

void CPurchaseSystemDlg::OnExitButton() 
{
    CDialog::OnOK();    
}

void CPurchaseSystemDlg::OnOrderButton() 
{
        retvalstruct *sem_ret;        // Holds semaphore RPC return value
        sharedmemretval *mem_ret;    // Holds shared memory RPC return value
        CString windowText;            // Holds order quantity text
        XPCTransaction transactRec;    // Stores the order information
        sharedmem *mem;                // Points to the raw shared memory bytes 
                                    // returned from the confirmation shared 
                                    // memory segment
        char sConfirmBuf[512];        // Stores the confirmation message
        

        // Retrieve the order information for Widgets.  Store the item ID and 
        // quantity within the transactRec object
        m_WidgetEdit.GetWindowText(windowText);
        transactRec.items[0].vSetItemID(WIDGET);
        windowText.TrimLeft();
        if (windowText.GetLength() == 0)
        {
            transactRec.items[0].vSetNum(0);
        }
        else
        {
            transactRec.items[0].vSetNum(atoi((const char *)windowText));
        }

        // Retrieve the order information for Sprockets.  Store the item ID and
        // quantity within the transactRect object
        m_SprocketEdit.GetWindowText(windowText);
        transactRec.items[1].vSetItemID(SPROCKET);
        windowText.TrimLeft();
        if (windowText.GetLength() == 0)
        {
            transactRec.items[1].vSetNum(0);
        }
        else
        {
            transactRec.items[1].vSetNum(atoi((const char *)windowText));
        }

        // Store the raw order information bytes into the transaction shared 
        // memory buffer
        memcpy((void *)writeTransactMem.data.sharedmem_val, (void *)&transactRec, sizeof(XPCTransaction));

        // Write the transaction shared memory buffer to the transaction shared 
        // memory segment
        mem_ret = write_1(&writeTransactMem, memcl);
        if (!mem_ret->iretval)
        {
            MessageBox(mem_ret->serrormsg, "Transaction Write Error", MB_OK);
            return;
        }

        // Unlock the transaction semaphore so that the confirmation server can
        // lock it and process the order
        sem_ret = unlock_1(&transactSem, semcl);
        if (!sem_ret->iretval)
        {
            MessageBox(sem_ret->serrormsg, "Semaphore Unlock Error", MB_OK);
            return;
        }

        // Attempt to lock the confirmation semaphore
        sem_ret = lockwait_1(&confirmSem, semcl);
        if (!sem_ret->iretval)
        {
            MessageBox(sem_ret->serrormsg, "Semaphore Lock Error", MB_OK);
            return;
        }

        // Since the confirmation semaphore was locked, information is available 
        // on the confirmation shared memory segment.  The raw bytes are 
        // extracted.
        mem = read_1(&confirmMem, memcl);

        // If the confirmation shared memory segment is empty, a problem has 
        // occurred
        if (*mem->sharedmem_val == 0)
        {
            MessageBox("Error Processing Order Request", "Confirmation Error", MB_OK);
            return;
        }

        // The raw bytes from the confirmation shared memory segment are cast to a 
        // XPCConfirmation object
        XPCConfirmation *confirm = (XPCConfirmation *)mem->sharedmem_val;

        // If the confirmation number is 0, an order processing problem has 
        // occurred.  The message is displayed within the confirmation edit window
        if (confirm->iGetConfirmNum() == 0)
        {
            m_ConfirmEdit.SetWindowText("Error Processing Order Request");
            return;
        }

        // The confirmation message is stored within a character buffer and 
        // displayed within the confirmation edit window
        sprintf(sConfirmBuf, "Order Confirmed: Confirmation #: %d Total Price: %.02f", confirm->iGetConfirmNum(), confirm->dGetPurchasePrice());
        m_ConfirmEdit.SetWindowText(sConfirmBuf);
}
