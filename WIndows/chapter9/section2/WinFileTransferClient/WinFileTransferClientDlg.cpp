// WinFileTransferClientDlg.cpp : implementation file
//

#include "stdafx.h"
#include "WinFileTransferClient.h"
#include "WinFileTransferClientDlg.h"
#include <XPCFileTransfer.h>
#include <afxsock.h>

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
// CWinFileTransferClientDlg dialog

CWinFileTransferClientDlg::CWinFileTransferClientDlg(CWnd* pParent /*=NULL*/)
    : CDialog(CWinFileTransferClientDlg::IDD, pParent)
{
    //{{AFX_DATA_INIT(CWinFileTransferClientDlg)
    m_strServer = _T("");
    m_strFile = _T("");
    m_strContents = _T("");
    //}}AFX_DATA_INIT
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CWinFileTransferClientDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CWinFileTransferClientDlg)
    DDX_Text(pDX, IDC_SERVER, m_strServer);
    DDX_Text(pDX, IDC_FILE, m_strFile);
    DDX_Text(pDX, IDC_CONTENTS, m_strContents);
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CWinFileTransferClientDlg, CDialog)
    //{{AFX_MSG_MAP(CWinFileTransferClientDlg)
    ON_WM_SYSCOMMAND()
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWinFileTransferClientDlg message handlers

BOOL CWinFileTransferClientDlg::OnInitDialog()
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

    SetIcon(m_hIcon, TRUE);            // Set big icon
    SetIcon(m_hIcon, FALSE);        // Set small icon

    // The socket library is initialized.
    AfxSocketInit();
    return TRUE;  // return TRUE  unless you set the focus to a control
}

void CWinFileTransferClientDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CWinFileTransferClientDlg::OnPaint() 
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

HCURSOR CWinFileTransferClientDlg::OnQueryDragIcon()
{
    return (HCURSOR) m_hIcon;
}

void CWinFileTransferClientDlg::OnOK() 
{
    char sBuf[1024];
    DWORD dwNumRead;

    // get data from the window controls
    UpdateData (TRUE);

    // temporary file and pathname
    CString strTempFile ("C:\\temp\\temp.txt");

    try
    {
        // connect to the file transfer server and get the file
        XPCFileTransfer fileTrans (
            m_strServer.GetBuffer (m_strServer.GetLength()));
        fileTrans.vTransferFile (
            m_strFile.GetBuffer (m_strFile.GetLength()),
            strTempFile.GetBuffer (strTempFile.GetLength()),
            TRANSFER_GET, 
            TRANSFER_TEXT);
    }
    catch (XPCException& ex)
    {
        MessageBox (ex.sGetException());
    }

    // clear any text in the contents text box's data member
    m_strContents.Empty();

    // open the temporary file for reading
    HANDLE hFile= CreateFile (strTempFile,
                        GENERIC_READ,
                        FILE_SHARE_READ,
                        NULL,
                        OPEN_EXISTING,
                        FILE_ATTRIBUTE_NORMAL,
                        NULL);
    if (hFile == NULL)
    {
        CString strErr = ErrorString (
                (CString)"Error opening file: ");
        MessageBox (strErr);
    }

    while (1)
    {
        // read the a buffer from the file
        if (!ReadFile (hFile,
                    sBuf,
                    sizeof (sBuf)-1,
                    &dwNumRead,
                    NULL))
        {
            CString strErr = ErrorString (
                    (CString)"Error reading file: ");
            MessageBox (strErr);
            return;
        }
        // handle end of file
        if (dwNumRead == 0)
            break;

        // null terminate the string
        sBuf[dwNumRead-1]= '\0';

        // append the data to contents text box's data member
        m_strContents+= sBuf;
    }

    CloseHandle (hFile);

    // push the data to the window controls
    UpdateData (FALSE);
}


CString& CWinFileTransferClientDlg::ErrorString (CString& sLeader)
{
    CString strBuf;
    LPVOID lpMsgBuf;
    UINT ErrNo;

    FormatMessage (
            FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
            NULL,
            ErrNo=GetLastError (),
            MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
            (LPTSTR) &lpMsgBuf,
            0,
            NULL);

    strBuf.Format ("%s: %s", sLeader, (LPTSTR)lpMsgBuf);

    LocalFree (lpMsgBuf);
    return strBuf;
}

void CWinFileTransferClientDlg::OnCancel() 
{
    WSACleanup();
    
    CDialog::OnCancel();
}
