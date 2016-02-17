// UDPAsyncDlg.h : header file
//

#if !defined(AFX_UDPASYNCDLG_H__13136A17_D0A7_11D2_B23B_00A0C90C4DA6__INCLUDED_)
#define AFX_UDPASYNCDLG_H__13136A17_D0A7_11D2_B23B_00A0C90C4DA6__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

/////////////////////////////////////////////////////////////////////////////
// CUDPAsyncDlg dialog

#include <XPCAsyncUdpSocket.h> // The CAsyncUdp definition
#include <XPCException.h>    // The XPCException definition
#include <XPCTransaction.h>    // The XPCTransaction definition

// Address and port number definitions 
#define UNIX_SERVER "aHost"
#define UNIX_PORT_NUM 6100
#define WINDOWS_NT_PORT_NUM 6100

class CUDPAsyncDlg : public CDialog
{
    // A pointer to an instance of the XPCAsyncUdpSocket class. The CStockPositionDlg
    // class is used as XPCAsyncUdpSocket's template.
    XPCAsyncUdpSocket<CUDPAsyncDlg> *portfolioSocket;

    // Array of up to 50 ticker positions
    XPCTransaction portfolio[50];

    // Number of positions currently tracked.
    int iTotalPositions;
public:
    CUDPAsyncDlg(CWnd* pParent = NULL);    // standard constructor

    // The method called when data is received by the OnReceive method of 
    // CAsyncUdpSocket.
    void vProcessMessage(char *, int);

    // Process socket related error messages
    void vProcessError(char *);

    void vUpdatePositionList(XPCTransaction *);        // Updates the ticker 
                                                        // position list
// Dialog Data
    //{{AFX_DATA(CUDPAsyncDlg)
    enum { IDD = IDD_UDPASYNC_DIALOG };
    CEdit    m_UserEdit;
    CEdit    m_TickerEdit;
    CEdit    m_QuantEdit;
    CListBox    m_PositionList;
    CEdit    m_BsEdit;
    //}}AFX_DATA

    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CUDPAsyncDlg)
    protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

// Implementation
protected:
    HICON m_hIcon;

    // Generated message map functions
    //{{AFX_MSG(CUDPAsyncDlg)
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

#endif // !defined(AFX_UDPASYNCDLG_H__13136A17_D0A7_11D2_B23B_00A0C90C4DA6__INCLUDED_)
