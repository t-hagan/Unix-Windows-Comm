// CTCPAsyncDlg.h : header file
//

#if !defined(AFX_CTCPASYNCDLG_H__13136A27_D0A7_11D2_B23B_00A0C90C4DA6__INCLUDED_)
#define AFX_CTCPASYNCDLG_H__13136A27_D0A7_11D2_B23B_00A0C90C4DA6__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

/////////////////////////////////////////////////////////////////////////////
// CCTCPAsyncDlg dialog

#include <XPCAsyncTcpSocket.h> // The XPCAsyncTcpSocket definition
#include <XPCException.h>    // The IPCException definition
#include "XPCTransaction.h"    // The XPCTransaction definition
#include "XPCPrice.h"        // The XPCPrice definition

// Address and port number definitions 
#define UNIX_SERVER "aHost"
#define PORTFOLIO_PORTNUM 6100
#define PRICE_PORTNUM 6101

class CTCPAsyncDlg : public CDialog
{
    // A pointer to an instance of the XPCAsyncTcpSocket. The XPCStockPositionDlg
    // class is used as XPCAsynchTcpSocket's template. This instances acts as a TCP
    // client.
    XPCAsyncTcpSocket<CTCPAsyncDlg> *portfolioSocket;

    // A pointer to an instance of the CAsyncTcpSocket.  The CStockPositionDlg
    // class is used as CAsyncTcpSocket's template.  This instance acts as a TCP
    // server.
    XPCAsyncTcpSocket<CTCPAsyncDlg> *priceSocket;

    // Array of up to 50 ticker positions
    XPCTransaction portfolio[50];

    // Array of up to 50 ticker prices
    XPCPrice prices[50];

    // Number of positions currently tracked.
    int iTotalPositions;

    // Number of prices tracked
    int iTotalPrices;

    // Gets a price given a ticker
    double dGetPrice(char *_sTicker);
public:
    CTCPAsyncDlg(CWnd* pParent = NULL);    // standard constructor

    // Destructor
    ~CTCPAsyncDlg() { delete priceSocket; delete portfolioSocket; }

    // The vProcessMessage method is called when the OnReceive receives data
    // method of XPCAsyncTcpSocket.
    void vProcessMessage(char *, int);

    // Process socket error messages
    void vProcessError(char *);

    // The vProcessConnect method is called when the portfolioSocket connects to the 
    // TCP server.
    void vProcessConnection(sockaddr_in *clientAddress);

    // Returns a message to be sent over the TCP socket. This method is not used for
    // this application 
    char *sGetMessageBuffer(int &iMessageSize) { iMessageSize = 0; return NULL; }

    // The vProcessClose method is called when a client disconnects from the
    // priceSocket.  This method is called by the OnClose method of 
    // XPCAsynchTcpSocket. A message box appears if the price provider client
    // disconnects. 
    void vProcessClose(sockaddr_in *clientAddress)
    { 
        MessageBox("The Price Provider Has Disconnected", "Error", MB_OK);
    }

    void vUpdatePositionList(XPCTransaction *);        // Updates the ticker 
                                                        // position list

    void vUpdatePricesList(XPCPrice *);                    // Updates price list
// Dialog Data
    //{{AFX_DATA(CTCPAsyncDlg)
    enum { IDD = IDD_CTCPASYNC_DIALOG };
    CEdit    m_UserEdit;
    CEdit    m_TickerEdit;
    CEdit    m_QuantEdit;
    CListBox    m_PositionList;
    CEdit    m_BsEdit;
    //}}AFX_DATA

    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CTCPAsyncDlg)
    protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

// Implementation
protected:
    HICON m_hIcon;

    // Generated message map functions
    //{{AFX_MSG(CCTCPAsyncDlg)
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

#endif // !defined(AFX_CTCPASYNCDLG_H__13136A27_D0A7_11D2_B23B_00A0C90C4DA6__INCLUDED_)
