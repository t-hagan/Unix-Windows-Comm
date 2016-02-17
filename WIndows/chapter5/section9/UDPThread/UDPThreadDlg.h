// UDPThreadDlg.h : header file
//

#if !defined(AFX_UDPTHREADDLG_H__8D40A537_49A5_11D2_B229_00A0C90C4DA6__INCLUDED_)
#define AFX_UDPTHREADDLG_H__8D40A537_49A5_11D2_B229_00A0C90C4DA6__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

/////////////////////////////////////////////////////////////////////////////
// CUDPThreadDlg dialog

#include <XPCUdpSocket.h>
#include <XPCException.h>
#include <XPCTransaction.h>

// Address and port number definitions 
#define UNIX_SERVER "aHost"
#define UNIX_PORT_NUM 6100
#define WINDOWS_NT_PORT_NUM 6100

class CUDPThreadDlg : public CDialog
{
    XPCTransaction portfolio[50];
    int iTotalPositions;
public:
    CUDPThreadDlg(CWnd* pParent = NULL);    // standard constructor
    ~CUDPThreadDlg() { if (positionSocket) delete positionSocket; } 

    void vUpdatePositionList(XPCTransaction *);        // Updates the ticker 
                                                    // position list

    XPCUdpSocket *positionSocket;            // Pointer to an instance of
                                                // XPCUdpSocket

// Dialog Data
    //{{AFX_DATA(CUDPThreadDlg)
    enum { IDD = IDD_UDPTHREAD_DIALOG };
    CListBox    m_PositionList;
    CEdit    m_TickerEdit;
    CEdit    m_UserEdit;
    CEdit    m_QuantEdit;
    CEdit    m_BsEdit;
    //}}AFX_DATA

    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CUDPThreadDlg)
    protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

// Implementation
protected:
    HICON m_hIcon;

    // Generated message map functions
    //{{AFX_MSG(CUDPThreadDlg)
    virtual BOOL OnInitDialog();
    afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
    afx_msg void OnPaint();
    afx_msg HCURSOR OnQueryDragIcon();
    afx_msg void OnExecuteButton();
    afx_msg void OnExitButton();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_UDPTHREADDLG_H__8D40A537_49A5_11D2_B229_00A0C90C4DA6__INCLUDED_)
