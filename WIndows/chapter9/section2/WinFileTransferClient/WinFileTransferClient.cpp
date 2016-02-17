// WinFileTransferClient.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "WinFileTransferClient.h"
#include "WinFileTransferClientDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWinFileTransferClientApp

BEGIN_MESSAGE_MAP(CWinFileTransferClientApp, CWinApp)
    //{{AFX_MSG_MAP(CWinFileTransferClientApp)
    //}}AFX_MSG
    ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWinFileTransferClientApp construction

CWinFileTransferClientApp::CWinFileTransferClientApp()
{
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CWinFileTransferClientApp object

CWinFileTransferClientApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CWinFileTransferClientApp initialization

BOOL CWinFileTransferClientApp::InitInstance()
{
//    if (!AfxSocketInit())
//    {
//        AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
//        return FALSE;
//    }

    AfxEnableControlContainer();

    // Standard initialization

#ifdef _AFXDLL
    Enable3dControls();            // Call this when using MFC in a shared DLL
#else
    Enable3dControlsStatic();    // Call this when linking to MFC statically
#endif

    CWinFileTransferClientDlg dlg;
    m_pMainWnd = &dlg;
    int nResponse = dlg.DoModal();

    // Since the dialog has been closed, return FALSE so that we exit the
    //  application, rather than start the application's message pump.
    return FALSE;
}
