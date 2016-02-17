// NewsSubClientDlg.h : header file
//

#if !defined(AFX_NEWSSUBCLIENTDLG_H__03E035F6_C02A_11D2_B23B_00A0C90C4DA6__INCLUDED_)
#define AFX_NEWSSUBCLIENTDLG_H__03E035F6_C02A_11D2_B23B_00A0C90C4DA6__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

/////////////////////////////////////////////////////////////////////////////
// CNewsSubClientDlg dialog

#include <XPCAsyncTcpSocket.h>
#include <XPCMsg.h>

#define UNKNOWN -1
#define CRITICAL 1
#define IMPORTANT 2
#define INFORMATIVE 3

#define PUBSUBSERVER_HOST "aHost"

class CNewsSubClientDlg : public CDialog
{
    XPCTcpSocket *BusSocket;            // Socket to receive business messages
    XPCTcpSocket *TechSocket;        // Socket to receive tech messages

    // Windows Asynchronous socket definitions of subscription sockets
    XPCAsyncTcpSocket<CNewsSubClientDlg> *BusAsyncSocket;
    XPCAsyncTcpSocket<CNewsSubClientDlg> *TechAsyncSocket;

public:
    // Processes asynchronous messages received from the socket
    void vProcessMessage(char *, int);

    // Processes asynchronous socket error messages
    void vProcessError(char *);

    CNewsSubClientDlg(CWnd* pParent = NULL);    // standard constructor

    // Methods required by XPCAsyncTcpSocket but not used
    void vProcessConnection(struct sockaddr_in *addr) { return; }
    char *sGetMessageBuffer(int iNumBytes) { return NULL; }
    void vProcessClose(struct sockaddr_in *addr) { return; }

// Dialog Data
    //{{AFX_DATA(CNewsSubClientDlg)
    enum { IDD = IDD_NEWSSUBCLIENT_DIALOG };
    CListBox    m_SubscribedList;
    CListBox    m_SubscribeList;
    CListBox    m_MessageList;
    //}}AFX_DATA

    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CNewsSubClientDlg)
    protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

// Implementation
protected:
    HICON m_hIcon;

    // Generated message map functions
    //{{AFX_MSG(CNewsSubClientDlg)
    virtual BOOL OnInitDialog();
    afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
    afx_msg void OnPaint();
    afx_msg HCURSOR OnQueryDragIcon();
    afx_msg void OnAddButton();
    afx_msg void OnRemoveButton();
    afx_msg void OnExitButton();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NEWSSUBCLIENTDLG_H__03E035F6_C02A_11D2_B23B_00A0C90C4DA6__INCLUDED_)
