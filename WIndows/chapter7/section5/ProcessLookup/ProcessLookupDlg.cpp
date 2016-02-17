// ProcessLookupDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ProcessLookup.h"
#include "ProcessLookupDlg.h"

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
// CProcessLookupDlg dialog

CProcessLookupDlg::CProcessLookupDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CProcessLookupDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CProcessLookupDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CProcessLookupDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CProcessLookupDlg)
	DDX_Control(pDX, IDC_STATUS_EDIT, m_StatusEdit);
	DDX_Control(pDX, IDC_EXECUTE_BUTTON, m_ExecuteButton);
	DDX_Control(pDX, IDC_USERNAME_EDIT, m_UsernameEdit);
	DDX_Control(pDX, IDC_RESULTS_LIST, m_ResultsList);
	DDX_Control(pDX, IDC_PASSWORD_EDIT, m_PasswordEdit);
	DDX_Control(pDX, IDC_HOST_EDIT, m_HostEdit);
	DDX_Control(pDX, IDC_CMD_EDIT, m_CmdEdit);
	DDX_Control(pDX, IDC_CMDPRMNT_STATIC, m_CmdPrmptStatic);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CProcessLookupDlg, CDialog)
	//{{AFX_MSG_MAP(CProcessLookupDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_EXIT_BUTTON, OnExitButton)
	ON_BN_CLICKED(IDC_EXECUTE_BUTTON, OnExecuteButton)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CProcessLookupDlg message handlers

BOOL CProcessLookupDlg::OnInitDialog()
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
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	AfxSocketInit();
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CProcessLookupDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CProcessLookupDlg::OnPaint() 
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
HCURSOR CProcessLookupDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CProcessLookupDlg::OnExitButton() 
{
	CDialog::OnOK();	
}

void CProcessLookupDlg::OnExecuteButton() 
{
	CString ButtonCaption;		// Holds the button caption
	CString	windowText;			// Holds the text from the windows controls

	m_ExecuteButton.GetWindowText(ButtonCaption);	// Retrieve the button caption

	// Reset the results window and status window
	m_ResultsList.ResetContent();
	m_StatusEdit.SetWindowText("");

	// If the button caption is "Execute", create a new REXEC session.
	if (ButtonCaption == CString("&Execute"))
	{
		try
		{
			// Construct a new rexecSession with a pointer to the current object and a finish indicator.
			rexecSession = new XPCRexecAsyncSetup(this, REXEC_PORT, "*DONE*");

			// Disable the execute button while connecting to the server
			m_ExecuteButton.EnableWindow(FALSE);

			// Get the connection information from the dialog
			m_UsernameEdit.GetWindowText(windowText);
			rexecSession->vSetUsername(windowText);

			m_PasswordEdit.GetWindowText(windowText);
			rexecSession->vSetPassword(windowText);

			m_HostEdit.GetWindowText(windowText);
			rexecSession->vSetHost(windowText);

			m_CmdEdit.GetWindowText(windowText);
			rexecSession->vSetCommand(windowText);
		
			rexecSession->vREXEC();
		}
		catch(XPCException &exceptObject)
		{
			char sMsg[512];
			sprintf(sMsg, "ERROR|%s", exceptObject.sGetException());
			vResultCallback(CString(sMsg));
			return;
		}
	}

	// If the button caption is not "Execute", send the command to the current UNIX server.
	else
	{
		// Disable the execute button while the server is processing the command.
		m_ExecuteButton.EnableWindow(FALSE);
		char sCmd[80];

		m_CmdEdit.GetWindowText(sCmd, 80);
		rexecSession->iSendMessage(sCmd, strlen(sCmd));
	}	
}

void CProcessLookupDlg::vResultCallback(CString &rslt)
{
	// If rslt is equal to the end of session value, change the button caption to "Execute",
	// change the command prompt to "REXEC Command", and enable the execute button.
	if (rslt == "%%%DONE%%%")
	{
		m_ExecuteButton.SetWindowText("&Execute");
		m_CmdPrmptStatic.SetWindowText("REXEC Command");
		m_ExecuteButton.EnableWindow(TRUE);
		return;
	}
	
	// If rslt is equal to the sFinishIndicator, clear the command prompt, set the execute 
	// button caption to "Command" and enable the execute button.
	if (rslt == rexecSession->sGetFinishIndicator())
	{
		m_ExecuteButton.SetWindowText("&Command");
		m_CmdEdit.SetWindowText("");
		m_ExecuteButton.EnableWindow(TRUE);
		return;
	}

	// If rslt contains the command "MSG", display the message in the status window.
	else if (!strncmp(rslt, "MSG|", 4))
	{
		char sTemprslt[255];
		strcpy(sTemprslt, rslt);
		m_StatusEdit.SetWindowText(CString((char *)sTemprslt+4));
	}

	// If rslt contains the command "ERROR", display the error message in the status window.
	else if (!strncmp(rslt, "ERROR|", 6))
	{
		char sTemprslt[255];
		strcpy(sTemprslt, rslt);
		m_StatusEdit.SetWindowText(CString("ERROR: ") + CString((char *)sTemprslt+6));
	}

	// If rslt contains the command "CMDPMPT", change the command prompt.
	else if (!strncmp(rslt, "CMDPMPT|", 8))
	{
		char sTemprslt[255];
		strcpy(sTemprslt, rslt);
		m_CmdPrmptStatic.SetWindowText(CString((char *)sTemprslt+8));
	}

	// Display all other messages in the results window.  Concattinate a new line / carriage
	// return to the end of the message for display purposes in the results window.
	else
	{
		if (rslt.GetLength() > 0)
			m_ResultsList.InsertString(-1, rslt);
	}
}
