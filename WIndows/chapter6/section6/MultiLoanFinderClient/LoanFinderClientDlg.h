// LoanFinderClientDlg.cpp : implementation file
//

#include "stdafx.h"
#include "LoanFinderClient.h"
#include "LoanFinderClientDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

UINT CallRpcThread(void *param);

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
    CAboutDlg();

// Dialog Data
    //{{AFX_DATA(CAboutDlg)
    enum { IDD = IDD_ABOUTBOX };
    //}}AFX_DATA

    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CAboutDlg)
    protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

// Implementation
protected:
    //{{AFX_MSG(CAboutDlg)
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
    //{{AFX_DATA_INIT(CAboutDlg)
    //}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CAboutDlg)
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
    //{{AFX_MSG_MAP(CAboutDlg)
        // No message handlers
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLoanFinderClientDlg dialog

CLoanFinderClientDlg::CLoanFinderClientDlg(CWnd* pParent /*=NULL*/)
    : CDialog(CLoanFinderClientDlg::IDD, pParent)
{
    //{{AFX_DATA_INIT(CLoanFinderClientDlg)
        // NOTE: the ClassWizard will add member initialization here
    //}}AFX_DATA_INIT
    // Note that LoadIcon does not require a subsequent DestroyIcon in Win32
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CLoanFinderClientDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CLoanFinderClientDlg)
    DDX_Control(pDX, IDC_AMORTIZATION_LISTBOX, m_AmortizationListBox);
    DDX_Control(pDX, IDC_YEARS_EDIT, m_YearsEdit);
    DDX_Control(pDX, IDC_PERIOD_EDIT, m_PeriodEdit);
    DDX_Control(pDX, IDC_PAYMENT_EDIT, m_PaymentEdit);
    DDX_Control(pDX, IDC_BANK_EDIT, m_BankEdit);
    DDX_Control(pDX, IDC_AMOUNT_EDIT, m_AmountEdit);
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CLoanFinderClientDlg, CDialog)
    //{{AFX_MSG_MAP(CLoanFinderClientDlg)
    ON_WM_SYSCOMMAND()
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    ON_BN_CLICKED(IDC_EXIT_BUTTON, OnExitButton)
    ON_BN_CLICKED(IDC_CALC_BUTTON, OnCalcButton)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLoanFinderClientDlg message handlers

BOOL CLoanFinderClientDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    // Add "About..." menu item to system menu.

    // IDM_ABOUTBOX must be in the system command range.
    ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
    ASSERT(IDM_ABOUTBOX < 0xF000);

    CMenu* pSysMenu = GetSystemMenu(FALSE);
    if (pSysMenu != NULL)
    {
        CString strAboutMenu;
        strAboutMenu.LoadString(IDS_ABOUTBOX);
        if (!strAboutMenu.IsEmpty())
        {
            pSysMenu->AppendMenu(MF_SEPARATOR);
            pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
        }
    }

    // Set the icon for this dialog.  The framework does this automatically
    //  when the application's main window is not a dialog
    SetIcon(m_hIcon, TRUE);            // Set big icon
    SetIcon(m_hIcon, FALSE);        // Set small icon
    
    // The pointer to the CLIENT structure is allocated memory.  If memory is not 
    // allocated for cl, scope is lost once the OnInitDialog() method exits.
    cl = new CLIENT;

    // A connection is made to the RPC server.
    cl = clnt_create(SERVER_HOST_NAME, payments, paymentsver, "tcp");
    if (cl == NULL)
    {
        // If the connection fails an error message is displayed containing the
        // RPC error.

        char *sErrorMsg = clnt_spcreateerror("Error Connecting to the RPC Server: ");
        MessageBox(sErrorMsg, "RPC ERROR", MB_OK);
        return 0;
    }

    
    return TRUE;  // return TRUE  unless you set the focus to a control
}

void CLoanFinderClientDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
    if ((nID & 0xFFF0) == IDM_ABOUTBOX)
    {
        CAboutDlg dlgAbout;
        dlgAbout.DoModal();
    }
    else
    {
        CDialog::OnSysCommand(nID, lParam);
    }
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CLoanFinderClientDlg::OnPaint() 
{
    if (IsIconic())
    {
        CPaintDC dc(this); // device context for painting

        SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

        // Center icon in client rectangle
        int cxIcon = GetSystemMetrics(SM_CXICON);
        int cyIcon = GetSystemMetrics(SM_CYICON);
        CRect rect;
        GetClientRect(&rect);
        int x = (rect.Width() - cxIcon + 1) / 2;
        int y = (rect.Height() - cyIcon + 1) / 2;

        // Draw the icon
        dc.DrawIcon(x, y, m_hIcon);
    }
    else
    {
        CDialog::OnPaint();
    }
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CLoanFinderClientDlg::OnQueryDragIcon()
{
    return (HCURSOR) m_hIcon;
}

void CLoanFinderClientDlg::OnExitButton() 
{
    // The CLIENT structure used for connection and data transfer is deallocated.
    clnt_destroy(cl);

    CDialog::OnOK();    
}

void CLoanFinderClientDlg::OnCalcButton() 
{
    CString tempString;                // Holds the text extracted from the edit
                                    // box controls.
    
    // Clears the contents of the amortization list box.
    m_AmortizationListBox.ResetContent();     

    // Retrieve the user-inputted loan amount.  If it does not exist an error message
    // is displayed.   If it does exist the value is stored in the dAmount data member
    // of aloan.
    m_AmountEdit.GetWindowText(tempString);
    if (!tempString.IsEmpty())
        aloan.dAmount = atof((const char *)tempString);
    else
    {
        MessageBox("Entry of a Loan Amount is Required", "Error", MB_OK);
        return;
    }

    // Retrieve the user-inputted number of years for the loan.  If it does not exist 
    // an error message is displayed.  If it does exist the value is stored in the 
    // iYear data member of aloan.
    m_YearsEdit.GetWindowText(tempString);
    if (!tempString.IsEmpty())
        aloan.iYear = atoi((const char *)tempString);
    else
    {
        MessageBox("Entry of Number of Loan Years is Required", "Error", MB_OK);
        return;
    }

    // The user-inputted number of payments throughout the year is retrieved.  If the 
    // number of payments does not exist an error message is displayed.  If it does
    // exist the value is stored in the iPerod data member of aloan.
    m_PeriodEdit.GetWindowText(tempString);
    if (!tempString.IsEmpty())
        aloan.iPeriod = atoi((const char *)tempString);
    else
    {
        MessageBox("Entry of Number of Payments During the Year is Required", "Error", MB_OK);
        return;
    }

    // A thread is created to call the remote procedures.  A pointer to the current
    // dialog class is sent to the thread to enable the thread to update the 
    // appropriate controls.  Threading of the RPC calls allows the user to
    // still be able to have access to the dialog.
    AfxBeginThread (CallRpcThread, (void *)this);
}

// The thread that calls the remote procedures.
UINT CallRpcThread(void *param)
{
    // The thread's parameter is casted back to its original CLoanClientDlg class 
    // format.
    CLoanFinderClientDlg *loanDlg = (CLoanFinderClientDlg *)param;

    char sPaymentString[512];            // Formatted amortization payment line
    struct spaymentinfo *retpayment;    // Holds the return value from the
                                        // dgetpayment_1() remote procedure call.
    struct sschedual *retschedual;        // Holds the return value from the
                                        // dsgetlistofpayments_1() remote procedure 
                                        // call.
    struct sschedual *currentpayment;    // Points to the current sschedule structure
                                        // within the list of sschedule structures.

    // The dgetpayment_1() remote procedure is called
    retpayment = dgetpayment_1(&loanDlg->aloan, loanDlg->cl);

    // The bank name is played in the m_BankEdit control and the payment amount is 
    // placed in the m_PaymentEdit control.

    char sBuffer[512];    // Temporary buffer to hold the bank name and payment 
                        // amount.

    retpayment->sBankName.sBankName_val[retpayment->sBankName.sBankName_len] = 0;
    sprintf(sBuffer, "%s", retpayment->sBankName.sBankName_val);
    loanDlg->m_BankEdit.SetWindowText(sBuffer);

    sprintf(sBuffer, "%.02f", retpayment->dPayment);
    loanDlg->m_PaymentEdit.SetWindowText(sBuffer);
    
    // The memory used to store the bank name is freed.
    xdr_free((int (*)(XDR *, char *))xdr_spaymentinfo, (char *)&retpayment->sBankName);

    // The sgetlistofpayments_1() remote procedure is called.
    retschedual = sgetlistofpayments_1(&loanDlg->aloan, loanDlg->cl);

    // If retrieval of the loan amortization schedule fails, an error message is
    // displayed.
    if (retschedual == NULL)
    {
        loanDlg->MessageBox("Error Retrieving the Loan Amortization Schedule", "Error", MB_OK);
        return 0;
    }

    // The loan amortization schedule is displayed within the m_AmortizationListBox.
    currentpayment = retschedual;
    while(currentpayment != NULL)
    {
        sprintf(sPaymentString, "%d     %.02f     %.02f     %.02f     %.02f", 
            currentpayment->aPayment.iPeriod,
            currentpayment->aPayment.dPayment,
            currentpayment->aPayment.dInterest,
            currentpayment->aPayment.dPrincipal,
            currentpayment->aPayment.dAmountRemaining);
        loanDlg->m_AmortizationListBox.InsertString(-1, sPaymentString);
        currentpayment = currentpayment->next;
    }

    // The memory used for the loan amortization schedule linked list is freed.
    xdr_free((int (*)(XDR *, char *))xdr_sschedual, (char *)retschedual);

    return 0;    
}
