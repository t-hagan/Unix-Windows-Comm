// LoanFinderClientDlg.h : header file
//

#if !defined(AFX_LOANFINDERCLIENTDLG_H__B246FBEB_D27C_11D2_B23B_00A0C90C4DA6__INCLUDED_)
#define AFX_LOANFINDERCLIENTDLG_H__B246FBEB_D27C_11D2_B23B_00A0C90C4DA6__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

/////////////////////////////////////////////////////////////////////////////
// CLoanFinderClientDlg dialog

#include <rpc/rpc.h>

// RPC server name definition.
#define SERVER_HOST_NAME "aHost"

extern "C"
{
#include <loan_system3.h>
}

class CLoanFinderClientDlg : public CDialog
{
// Construction
public:
    CLoanFinderClientDlg(CWnd* pParent = NULL);    // standard constructor
    CLIENT *cl;            // Pointer to client connection information
    sloaninfo aloan;    // Stores user-loan information in an sloaninfo structure 
                        // instance.  sloaninfo is defined within loan_system3.h.

// Dialog Data
    //{{AFX_DATA(CLoanFinderClientDlg)
    enum { IDD = IDD_LOANFINDERCLIENT_DIALOG };
    CListBox    m_AmortizationListBox;
    CEdit    m_YearsEdit;
    CEdit    m_PeriodEdit;
    CEdit    m_PaymentEdit;
    CEdit    m_BankEdit;
    CEdit    m_AmountEdit;
    //}}AFX_DATA

    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CLoanFinderClientDlg)
    protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

// Implementation
protected:
    HICON m_hIcon;

    // Generated message map functions
    //{{AFX_MSG(CLoanFinderClientDlg)
    virtual BOOL OnInitDialog();
    afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
    afx_msg void OnPaint();
    afx_msg HCURSOR OnQueryDragIcon();
    afx_msg void OnExitButton();
    afx_msg void OnCalcButton();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LOANFINDERCLIENTDLG_H__B246FBEB_D27C_11D2_B23B_00A0C90C4DA6__INCLUDED_)
