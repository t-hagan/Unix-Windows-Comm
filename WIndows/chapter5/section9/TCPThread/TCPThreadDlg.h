// TCPThreadDlg.h : header file
//

#if !defined(AFX_TCPTHREADDLG_H__B874DF26_D1B8_11D2_B23B_00A0C90C4DA6__INCLUDED_)
#define AFX_TCPTHREADDLG_H__B874DF26_D1B8_11D2_B23B_00A0C90C4DA6__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

/////////////////////////////////////////////////////////////////////////////
// CTCPThreadDlg dialog

#include <XPCTcpSocket.h>
#include <XPCException.h>
#include <XPCTransaction.h>

// Address and port number definitions 
#define UNIX_SERVER "aHost"
#define UNIX_PORT_NUM 6100
#define WINDOWS_NT_PORT_NUM 6100

class CTCPThreadDlg : public CDialog
{
    XPCTransaction portfolio[50];
    int iTotalPositions;
public:
    CTCPThreadDlg(CWnd* pParent = NULL);    // standard constructor
    ~CTCPThreadDlg() { if (positionSocket) delete positionSocket; } 

    void vUpdatePositionList(XPCTransaction *);        // Updates the ticker 
                                                        // position list

    XPCTcpSocket *positionSocket;            // Pointer to an instance of
                                                // XPCTcpSocket
// Dialog Data
    //{{AFX_DATA(CTCPThreadDlg)
    enum { IDD = IDD_TCPTHREAD_DIALOG };
    CEdit    m_UserEdit;
    CEdit    m_TickerEdit;
    CEdit    m_QuantEdit;
    CListBox    m_PositionList;
    CEdit    m_BsEdit;
    //}}AFX_DATA

    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CTCPThreadDlg)
    protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

// Implementation
protected:
    HICON m_hIcon;

    // Generated message map functions
    //{{AFX_MSG(CTCPThreadDlg)
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

#endif // !defined(AFX_TCPTHREADDLG_H__B874DF26_D1B8_11D2_B23B_00A0C90C4DA6__INCLUDED_)
