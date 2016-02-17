// WinFileTransferClientDlg.h : header file
//

#if !defined(AFX_WINFILETRANSFERCLIENTDLG_H__45130016_D634_11D2_B1E1_0008C745C863__INCLUDED_)
#define AFX_WINFILETRANSFERCLIENTDLG_H__45130016_D634_11D2_B1E1_0008C745C863__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

/////////////////////////////////////////////////////////////////////////////
// CWinFileTransferClientDlg dialog

class CWinFileTransferClientDlg : public CDialog
{
// Construction
public:
    CWinFileTransferClientDlg(CWnd* pParent = NULL);    // standard constructor
    CString& ErrorString (CString& sLeader);

// Dialog Data
    //{{AFX_DATA(CWinFileTransferClientDlg)
    enum { IDD = IDD_WINFILETRANSFERCLIENT_DIALOG };
    CString    m_strServer;
    CString    m_strFile;
    CString    m_strContents;
    //}}AFX_DATA

    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CWinFileTransferClientDlg)
    protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

// Implementation
protected:
    HICON m_hIcon;

    // Generated message map functions
    //{{AFX_MSG(CWinFileTransferClientDlg)
    virtual BOOL OnInitDialog();
    afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
    afx_msg void OnPaint();
    afx_msg HCURSOR OnQueryDragIcon();
    virtual void OnOK();
    virtual void OnCancel();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WINFILETRANSFERCLIENTDLG_H__45130016_D634_11D2_B1E1_0008C745C863__INCLUDED_)
