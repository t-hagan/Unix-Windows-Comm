// WinRexecProcessControlClientDlg.cpp : implementation file
//

#include "stdafx.h"
#include "WinRexecProcessControlClient.h"
#include "WinRexecProcessControlClientDlg.h"
#include <fstream.h>

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
// CWinRexecProcessControlClientDlg dialog

CWinRexecProcessControlClientDlg::CWinRexecProcessControlClientDlg(CWnd* pParent /*=NULL*/)
    : CDialog(CWinRexecProcessControlClientDlg::IDD, pParent)
{
    //{{AFX_DATA_INIT(CWinRexecProcessControlClientDlg)
        // NOTE: the ClassWizard will add member initialization here
    //}}AFX_DATA_INIT
    // Note that LoadIcon does not require a subsequent DestroyIcon in Win32
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CWinRexecProcessControlClientDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CWinRexecProcessControlClientDlg)
    DDX_Control(pDX, IDC_RESULTS_LIST, m_ResultsList);
    DDX_Control(pDX, IDC_HOST_LIST, m_HostList);
    DDX_Control(pDX, IDC_DATA_EDIT, m_DataEdit);
    DDX_Control(pDX, IDC_COMMAND_COMBO, m_CommandCombo);
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CWinRexecProcessControlClientDlg, CDialog)
    //{{AFX_MSG_MAP(CWinRexecProcessControlClientDlg)
    ON_WM_SYSCOMMAND()
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    ON_BN_CLICKED(IDC_EXECUTE_BUTTON, OnExecuteButton)
    ON_BN_CLICKED(IDC_EXIT_BUTTON, OnExitButton)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWinRexecProcessControlClientDlg message handlers

BOOL CWinRexecProcessControlClientDlg::OnInitDialog()
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
    
    // Get the list of hosts
    char sHost[512];
    ifstream fin("c:\\temp\\lookup_list.txt");
    if (!fin)
    {
        MessageBox("Could not open lookup_list.txt", "Error", MB_OK);
        CDialog::OnOK();
    }

    // Add the hosts until the eof
    while(!fin.eof())
    {
        fin >> sHost;

        m_HostList.AddString(sHost);
    }
    
    try
    {
        // Create an array of CRexecPipe objects
        rexecPipeArray = new SRexecArray[m_HostList.GetCount()];

        // Initialize the winSock library
        AfxSocketInit();

        // Each element of the CRexecPipe array is initialized. CRexecPipe 
        // initialization causes a connection to be made to the specified remote 
        // host

        for (int iCount = 0; iCount < m_HostList.GetCount(); iCount++)
        {
            m_HostList.GetText(iCount, sHost);
            rexecPipeArray[iCount].rexecPipe = new XPCRexecPipe("username", "password", "/chapter11/section2/ProcessLookupServer", sHost, "** DONE **", REXEC_PORT);
        }

        // Add the command to the combo box
        m_CommandCombo.AddString("LOOKUP");
        m_CommandCombo.AddString("KILL");
        m_CommandCombo.SetCurSel(0);
    }
    catch(XPCException &exceptOb)
    {
        MessageBox(exceptOb.sGetException(), "REXEC PIPE ERROR", MB_OK);
        CDialog::OnOK();
    }

    return TRUE;  // return TRUE  unless you set the focus to a control
}

void CWinRexecProcessControlClientDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

UINT CWinRexecProcessControlClientDlg::rexecThread(void *_vArg)
{
    // The thread's argument is cast to the CRexecThread class
    XPCRexecThread *threadData = (XPCRexecThread *)_vArg;

    char sResult[255];    // Stores the data read from the pipe
    
    try
    {
        // Data is read from the pipe and placed within the results list box until 
        // no more data is available.
        while (*threadData->getRexecPipe() >> sResult != NO_MORE_DATA)
        {
            threadData->getPsDlg()->m_ResultsList.AddString(sResult);
        }
    }
    catch(XPCException &exceptOb)
    {
        // All XPCRexecPipe related exceptions are caught and displayed to 
        // the user
        threadData->getPsDlg()->MessageBox(exceptOb.sGetException(), "REXEC PIPE ERROR", MB_OK);
    }

    delete threadData;

    return 1;
}


// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CWinRexecProcessControlClientDlg::OnPaint() 
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
HCURSOR CWinRexecProcessControlClientDlg::OnQueryDragIcon()
{
    return (HCURSOR) m_hIcon;
}

void CWinRexecProcessControlClientDlg::OnExecuteButton() 
{
    char sData[80];            // Stores the data associated with the chosen 
                            // command
    char sCommand[80];        // Stores the command sent to the Process Control 
                            // Server
    char sCommandCombo[80];    // Stores the user chosen command

    m_ResultsList.ResetContent();        // The results are cleared

    // The user's chosen command and associated data are extracted from the dialog and
    // combined to form one command
    m_DataEdit.GetWindowText(sData, 80);
    m_CommandCombo.GetWindowText(sCommandCombo, 80);
        
    sprintf(sCommand, "%s %s", sCommandCombo, sData);
    
    try
    {
        // The command is sent to all connected Process Control Servers using 
        // XPCRexecPipe objects contained within the REXEC pipe array
        for (int iCount = 0; iCount < m_HostList.GetCount(); iCount++)
        {
            *rexecPipeArray[iCount].rexecPipe << sCommand;
        
            // A XPCRexecThread object is created that contains the XPCRexecPipe 
            // object used to send the command and a pointer to the process 
            // control client dialog    
            XPCRexecThread *threadData = new XPCRexecThread(rexecPipeArray[iCount].rexecPipe, this);
        

            // A thread is spawned to retrieve the server's reply
            AfxBeginThread(CWinRexecProcessControlClientDlg::rexecThread, (void *)threadData);
        }
    }
    catch(XPCException &exceptOb)
    {
        // All REXEC pipe related errors are trapped and displayed to the user 
        MessageBox(exceptOb.sGetException(), "REXEC PIPE ERROR", MB_OK);
    }    
    
}

void CWinRexecProcessControlClientDlg::OnExitButton() 
{
        // When the "Exit" button is clicked, a "QUIT" message is sent to all Process 
        // Control Servers connected to XPCRexecPipe objects within the REXEC pipe array
        for (int iCount = 0; iCount < m_HostList.GetCount(); iCount++)
        {
            *rexecPipeArray[iCount].rexecPipe << "QUIT";
        }

        // The Process Control Client exits
        CDialog::OnOK();
    
}
