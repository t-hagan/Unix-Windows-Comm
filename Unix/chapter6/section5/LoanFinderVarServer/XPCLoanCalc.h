#ifndef _XPCLoanCalc
#define _XPCLoanCalc

#include <string.h>

extern "C"
{
#include <loan_system3.h>
}

class XPCBankDB
{
        double dRate;           // The rate of the loan
        int iLoanLength;        // The length of the loan
        char sBankName[512];    // The name of the bank
public:
        // The constructor.  Initializes private member variables.
        XPCBankDB(double _dRate, int _iLoanLength, char *_sBankName) :
        dRate(_dRate), iLoanLength(_iLoanLength) { strcpy(sBankName, _sBankName); }

        // Public methods which retrieve private member variable values.
        double dGetRate() { return dRate; }
        int iGetLoanLength() { return iLoanLength; }
        char *sGetBankName() { return sBankName; }
};

class XPCLoanCalc
{
        double dCurrentRate;    // The loan rate
        int iNumYears;          // The number of years of the loan
        int iNumPeriods;        // The number of payment periods within the year.
        double dAmount; // The amount of the loan
        double dPayment;        // The amount of the payment
        char sBankName[512];    // The name of the bank providing the loan
        sschedual *currentSched;// The amortization schedual
    
        // The local bank database
        XPCBankDB *BankDatabase[5];

        // Retrieves the bank name with the lowest rate for the given period.  Returns
        // TRUE if the bank name and rate were successfully retrieved, FALSE if they were
        // not.
        int iRetrieveRate();
public:
        // database.
        XPCLoanCalc(int _iNumYears, int _iNumPeriods, double _dAmount);

        // Calls iRetrieveRate to get the bank name and rate and calculates the payment
        // based the amount of the loan given.
        void vCalcPayment();

        // Calculates the amortization schedule
        void vCalcSchedual();

        // Return the payment amount.
        double dRetPayment() { return dPayment; }

        // Return the name of the bank.
        char *sGetBankName() { return sBankName; }

        // Returns the pointer to the amortization schedual
        sschedual *getCurrentSched() { return currentSched; }
};

#endif
