// PurchaseSystemDlg.h : header file
//

#if !defined(AFX_PURCHASESYSTEMDLG_H__0E57C9C7_9F2F_11D2_B237_00A0C90C4DA6__INCLUDED_)
#define AFX_PURCHASESYSTEMDLG_H__0E57C9C7_9F2F_11D2_B237_00A0C90C4DA6__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include <XPCTransaction.h>
#include <XPCConfirmation.h>
#include <rpc/rpc.h>

extern "C"
{
#include <SharedMemServer.h>
#include <CrossPlatformSem.h>
}

/////////////////////////////////////////////////////////////////////////////
// CPurchaseSystemDlg dialog

// Product ID definitions
#define WIDGET 0
#define SPROCKET 1

// Semaphore and Shared Memory Server definitions
#define SHARED_MEM_SERVER "aHost"
#define SEMAPHORE_SERVER "aHost"

class CPurchaseSystemDlg : public CDialog
{
// Construction
public:
    CPurchaseSystemDlg(CWnd* pParent = NULL);    // standard constructor
    CLIENT *memcl;                    // Holds shared memory server connection information
    CLIENT *semcl;                    // Holds semaphore server connection information
    writesharedmem writeTransactMem;// Stores data to write to the transaction shared memory segment
    createsharedmem confirmMem;        // Defines the shared memory segment when extracting confirmation information
    semname transactSem;            // Holds the transaction semaphore name
    semname confirmSem;                // Holds the confirmation semaphore name

// Dialog Data
    //{{AFX_DATA(CPurchaseSystemDlg)
    enum { IDD = IDD_PURCHASESYSTEM_DIALOG };
    CEdit    m_WidgetEdit;
    CEdit    m_SprocketEdit;
    CEdit    m_ConfirmEdit;
    //}}AFX_DATA

    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CPurchaseSystemDlg)
    protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

// Implementation
protected:
    HICON m_hIcon;

    // Generated message map functions
    //{{AFX_MSG(CPurchaseSystemDlg)
    virtual BOOL OnInitDialog();
    afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
    afx_msg void OnPaint();
    afx_msg HCURSOR OnQueryDragIcon();
    afx_msg void OnExitButton();
    afx_msg void OnOrderButton();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PURCHASESYSTEMDLG_H__0E57C9C7_9F2F_11D2_B237_00A0C90C4DA6__INCLUDED_)
