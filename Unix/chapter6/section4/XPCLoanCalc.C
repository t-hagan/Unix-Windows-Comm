#include <XPCLoanCalc.h>
#include <iostream.h>
#include <string.h>
#include <math.h>

// Constructor
XPCLoanCalc::XPCLoanCalc(int _iNumYears,
             int _iNumPeriods,
             double _dAmount) :
             iNumYears(_iNumYears),
             iNumPeriods(_iNumPeriods),
             dAmount(_dAmount)    
{
    // Create the bank database
    BankDatabase[0] = new XPCBankDB(.08, 30 * 12, "BankA");
    BankDatabase[1] = new XPCBankDB(.07, 30 * 12, "BankB");
    BankDatabase[2] = new XPCBankDB(.06, 15 * 12, "BankC");
    BankDatabase[3] = new XPCBankDB(.075, 15 * 6, "BankD");
    BankDatabase[4] = new XPCBankDB(.0725, 30 * 10, "BankE");
}

int XPCLoanCalc::iRetrieveRate()
{
    // Initlize the lowest rate to 9999
    dCurrentRate = 9999;

    // Loop through all entries within the bank database
    for (int count = 0; count < 5; count++)
    {
        // If the loan length for the bank entry matches the requested loan
        // length, the bank's rate is compared to the current rate.  If the bank's
        // rate is lower than the current rate, the current rate is replaced and
        // the bank's name is stored.
        if (BankDatabase[count]->iGetLoanLength() == (iNumYears * iNumPeriods))
            if (BankDatabase[count]->dGetRate() < dCurrentRate)
            {
                dCurrentRate = BankDatabase[count]->dGetRate();
                strcpy(sBankName, BankDatabase[count]->sGetBankName());
            }
    }

    // If after looping though all database entries there where no banks that had a
    // loan for the period requested, return FALSE.
    if (dCurrentRate == 9999)
            return 0;

    return 1;
}

void XPCLoanCalc::vCalcPayment()
{

    // Retireve the bank name and loan rate
    if (!iRetrieveRate())
    {
        // If there is an error retrieving the loan informaiton, store an error message
        // and return;
        strcpy(sBankName, "Could Not Find A Bank That Meets the Requested Criteria");
        dPayment = 0;
        return;
    }

    // Calculate the number of payments
    int iNumPayments = iNumYears * iNumPeriods;
    double dFactor = 0;

    // Calculate the loan payment amount
    for (int iPayment = 1; iPayment <= iNumPayments; iPayment++)
    {    
        dFactor = dFactor + (1.0/pow(1.0 + (dCurrentRate)/(double)iNumPeriods, (double)iPayment));
    }

    dPayment = dAmount / dFactor;
}
