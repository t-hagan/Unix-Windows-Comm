// ProcessLookupDlg.h : header file
//

#if !defined(AFX_PROCESSLOOKUPDLG_H__90DC9216_D62C_11D2_B23B_00A0C90C4DA6__INCLUDED_)
#define AFX_PROCESSLOOKUPDLG_H__90DC9216_D62C_11D2_B23B_00A0C90C4DA6__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include <XPCRexecAsyncSetup.h>

/////////////////////////////////////////////////////////////////////////////
// CProcessLookupDlg dialog

class CProcessLookupDlg : public CDialog, public CRexecDlg
{
	XPCRexecAsyncSetup *rexecSession;				// Pointer to instance of CRexecSetup 
public:
	CProcessLookupDlg(CWnd* pParent = NULL);	// standard constructor

	// The result callback function
	void vResultCallback(CString &rslt);
// Dialog Data
	//{{AFX_DATA(CProcessLookupDlg)
	enum { IDD = IDD_PROCESSLOOKUP_DIALOG };
	CEdit	m_StatusEdit;
	CButton	m_ExecuteButton;
	CEdit	m_UsernameEdit;
	CListBox	m_ResultsList;
	CEdit	m_PasswordEdit;
	CEdit	m_HostEdit;
	CEdit	m_CmdEdit;
	CStatic	m_CmdPrmptStatic;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CProcessLookupDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CProcessLookupDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnExitButton();
	afx_msg void OnExecuteButton();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROCESSLOOKUPDLG_H__90DC9216_D62C_11D2_B23B_00A0C90C4DA6__INCLUDED_)
