#include <XPCLoanCalc.h>
#include <iostream.h>

extern "C"
{
#include <loan_system3.h>
}

sschedual *sgetlistofpayments_1(sloaninfo *_sInfo)
{
        // The XPCLoanCalc object is made static to avoid it from being lost when the
        // function goes out of scope.
        static XPCLoanCalc *aLoan = NULL;

        // If the XPCLoanCalc object has been previously used, the object is deleted.
        if (aLoan != NULL)
                delete aLoan;

        // A new instance of the XPCLoanCalc object is created on the heap.  The user-
         // defined loan values are used to create the XPCLoanCalc object instance.
        aLoan = new XPCLoanCalc(_sInfo->iYear, _sInfo->iPeriod, _sInfo->dAmount);

        // The loan amount is calculated.
        aLoan->vCalcPayment();

        // The loan amortization schedule is created.
        aLoan->vCalcSchedual();

        // The loan amortization schedule is returned to the client.
        return aLoan->getCurrentSched();
}

spaymentinfo *dgetpayment_1(sloaninfo *_sInfo)
{
    static spaymentinfo apayment;

    delete [] apayment.sBankName.sBankName_val;

    // An instance of XPCLoanCalc is constructed using a time period of 30 years and 12
    // payments per year
    XPCLoanCalc aLoan(_sInfo->iYear, _sInfo->iPeriod, _sInfo->dAmount);

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
