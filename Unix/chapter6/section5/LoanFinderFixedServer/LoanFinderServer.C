#include <XPCLoanCalc.h>
#include <iostream.h>

extern "C"
{
#include <loan_system2.h>
}

sschedual *sgetlistofpayments_1(double *_dAmount)
{
    // An instance of XPCLoanCalc is created.  The constructor is passed a value of 30
    // years, 12 payments per year, and the amount of the loan.
    XPCLoanCalc aLoan(30, 12, *_dAmount);

    // The loan amortization schedule is computed.
    aLoan.vCalcSchedual();

    // The structure returned holding the requested amortization schedule.
    static sschedual aschedual;

    // Copies the amortization schedule from aLoan to aschedual.
    for (int payment = 0; payment < MAX_PAYMENTS; payment++)
    {
        memcpy((void *)&aschedual.listofpayments[payment], (void *)&aLoan.getListofPayments(payment), sizeof(struct ssinglepayment));
    }

    // Returns the amortization schedule.
    return &aschedual;
}

spaymentinfo *dgetpayment_1(double *_dAmount)
{
    static spaymentinfo apayment;

    delete [] apayment.sBankName.sBankName_val;

    // An instance of XPCLoanCalc is constructed using a time period of 30 years and 12
    // payments per year
    XPCLoanCalc aLoan(30, 12, *_dAmount);

    // Retrieve the loan payment
    aLoan.vCalcPayment();

    // The bank name structure is allocated memory and the bank name is stored within it
    apayment.sBankName.sBankName_val = new char[strlen(aLoan.sGetBankName())];
    apayment.sBankName.sBankName_len = strlen(aLoan.sGetBankName());
    strcpy(apayment.sBankName.sBankName_val, aLoan.sGetBankName());

    cout << apayment.sBankName.sBankName_val << endl;

    // Thank loan payment is stored
    apayment.dPayment = aLoan.dRetPayment();

    // The spaymentinfo instance is returned to the client
    return &apayment;
}
