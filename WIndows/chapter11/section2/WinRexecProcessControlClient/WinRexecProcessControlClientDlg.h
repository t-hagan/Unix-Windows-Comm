// WinRexecProcessControlClientDlg.h : header file
//

#if !defined(AFX_WINREXECPROCESSCONTROLCLIENTDLG_H__5B279157_DB26_11D2_B23B_00A0C90C4DA6__INCLUDED_)
#define AFX_WINREXECPROCESSCONTROLCLIENTDLG_H__5B279157_DB26_11D2_B23B_00A0C90C4DA6__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

/////////////////////////////////////////////////////////////////////////////
// CWinRexecProcessControlClientDlg dialog
#include <XPCRexecPipe.h>
#include <afxsock.h>
#include <afxcoll.h>

struct SRexecArray
{
    XPCRexecPipe *rexecPipe;
};

class CWinRexecProcessControlClientDlg : public CDialog
{
    void vProcessResult(char *);            
    static UINT rexecThread(void *_vArg);    // Thread callback function
    struct SRexecArray *rexecPipeArray;        // Defines an array of XPCRexecPipe 
                                            // objects

public:
    CWinRexecProcessControlClientDlg(CWnd* pParent = NULL);    // standard constructor

// Dialog Data
    //{{AFX_DATA(CWinRexecProcessControlClientDlg)
    enum { IDD = IDD_WINREXECPROCESSCONTROLCLIENT_DIALOG };
    CListBox    m_ResultsList;
    CListBox    m_HostList;
    CEdit    m_DataEdit;
    CComboBox    m_CommandCombo;
    //}}AFX_DATA

    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CWinRexecProcessControlClientDlg)
    protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

// Implementation
protected:
    HICON m_hIcon;

    // Generated message map functions
    //{{AFX_MSG(CWinRexecProcessControlClientDlg)
    virtual BOOL OnInitDialog();
    afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
    afx_msg void OnPaint();
    afx_msg HCURSOR OnQueryDragIcon();
    afx_msg void OnExecuteButton();
    afx_msg void OnExitButton();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

class XPCRexecThread
{
    XPCRexecPipe *rexecPipe;            // Pointer to a XPCRexecPipe object
    CWinRexecProcessControlClientDlg *psDlg;    // Pointer to the main dialog class
public:
    // Constructor.  Stores the private data member elements
    XPCRexecThread(XPCRexecPipe *_rexecPipe, CWinRexecProcessControlClientDlg *_psDlg)
    {
        rexecPipe = _rexecPipe;
        psDlg = _psDlg;
    }    

    // Methods to access the private data members
    XPCRexecPipe *getRexecPipe() { return rexecPipe; }
    CWinRexecProcessControlClientDlg *getPsDlg() { return psDlg; }
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WINREXECPROCESSCONTROLCLIENTDLG_H__5B279157_DB26_11D2_B23B_00A0C90C4DA6__INCLUDED_)
