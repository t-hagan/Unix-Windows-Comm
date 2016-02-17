// NewsPublisherDlg.h : header file
//

#if !defined(AFX_NEWSPUBLISHERDLG_H__03E035E7_C02A_11D2_B23B_00A0C90C4DA6__INCLUDED_)
#define AFX_NEWSPUBLISHERDLG_H__03E035E7_C02A_11D2_B23B_00A0C90C4DA6__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

/////////////////////////////////////////////////////////////////////////////
// CNewsPublisherDlg dialog

#include <XPCMsg.h>

#define PUBSUBSERVER "aHost"

class CNewsPublisherDlg : public CDialog
{
    XPCTcpSocket *clientSocket;    // Communication TCP socket
public:
    CNewsPublisherDlg(CWnd* pParent = NULL);    // standard constructor

// Dialog Data
    //{{AFX_DATA(CNewsPublisherDlg)
    enum { IDD = IDD_NEWSPUBLISHER_DIALOG };
    CEdit    m_NewsMessage;
    CEdit    m_NewsSource;
    CComboBox    m_NewsPriority;
    CComboBox    m_NewsService;
    //}}AFX_DATA

    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CNewsPublisherDlg)
    protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

// Implementation
protected:
    HICON m_hIcon;

    // Generated message map functions
    //{{AFX_MSG(CNewsPublisherDlg)
    virtual BOOL OnInitDialog();
    afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
    afx_msg void OnPaint();
    afx_msg HCURSOR OnQueryDragIcon();
    afx_msg void OnSendButton();
    afx_msg void OnExitButton();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NEWSPUBLISHERDLG_H__03E035E7_C02A_11D2_B23B_00A0C90C4DA6__INCLUDED_)
