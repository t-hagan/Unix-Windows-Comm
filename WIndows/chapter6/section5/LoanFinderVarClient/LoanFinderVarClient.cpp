#include <iostream.h>
#include <rpc/rpc.h>

extern "C"
{
#include "loan_system3.h"
}

// RPC server name definition.
#define SERVER_HOST_NAME "aHost"

main()
{
    double dLoanAmount;                    // Holds the amount of the loan.
    int iYears;                            // The user-inputted number of years for the 
                                        // loan.
    int iPeriods;                        // The user-inputted number of loan payments 
                                        // during the year.

    struct spaymentinfo *retpayment;    // A pointer to the server's return value.
    struct sschedual *retschedual;        // Holds the return from the 
                                        // sgetlistofpayments_1 remote procedure.
    struct sschedual *currentpayment;    // Pointer to the current element in the 
                                        // sschedule linked list.
    CLIENT *cl;                            // A pointer to server location information.
    int iChoice;                        // Client menu choice.

    // Create a connection to the server.  If the connection fails an error
    // message is displayed and the program exits.
    cl = clnt_create(SERVER_HOST_NAME, payments, paymentsver, "tcp");
    if (cl == NULL)
    {
        cerr << "Error connecting to server" << endl;
        return 0;
    }

    // Loop forever
    while(1)
    {
        // Display a menu to the user
        cout << "(1) Locate Bank with Lowest Loan Payment" << endl;
        cout << "(2) Display an Amortization Schedule" << endl;
        cout << "(3) Exit" << endl;
        cout << "Enter Your Choice: " << flush;
        cin >> iChoice;


        // If the user chooses 1, the user is prompted for loan information.
        // A pointer to the loan information along with the client connection 
        // information is passed to the dgetpayment_1() client stub function.  If 
        // the return value is NULL an error occurred. Otherwise the bank name and 
        // loan payment amount is displayed for the user.  The last step frees 
        // the memory allocated for holding the bank name.
        if (iChoice == 1)
        {

            // Prompt the user for the loan amount, the number of years of the 
            // loan, and the number of loan payments within the year.
            cout << "Enter Loan Amount: " << flush;
            cin >> dLoanAmount;

            cout << "Enter Number of Years: " << flush;
            cin >> iYears;

            cout << "Enter Number of Periods: " << flush;
            cin >> iPeriods;

            // Pack the user-supplied loan information into a sloaninfo 
            // structure.
            sloaninfo aloan;
            aloan.dAmount = dLoanAmount;
            aloan.iYear = iYears;
            aloan.iPeriod = iPeriods;

            retpayment = dgetpayment_1(&aloan, cl);
            if (retpayment == NULL)
            {
                cerr << clnt_spcreateerror("Error calling dgetpayment_1: ") << endl;
                continue;
            }

            retpayment->sBankName.sBankName_val[retpayment->sBankName.sBankName_len] = 0;
            cout << "Bank Name: " << retpayment->sBankName.sBankName_val << endl;
            cout << "Payment  : " << retpayment->dPayment << endl;

            // Free memory used for storing the bank name.
            xdr_free((int (*)(XDR *, char *))xdr_spaymentinfo, (char *)&retpayment->sBankName);
        }

        else if (iChoice == 2)
        {
            // Prompt the user for the loan amount, the number of years of the 
            // loan, and the number of loan payments within the year.
            cout << "Enter Loan Amount: " << flush;
            cin >> dLoanAmount;

            cout << "Enter Number of Years: " << flush;
            cin >> iYears;

            cout << "Enter Number of Periods: " << flush;
            cin >> iPeriods;

            // Pack the user-supplied loan information into a sloaninfo 
            // structure.
            sloaninfo aloan;
            aloan.dAmount = dLoanAmount;
            aloan.iYear = iYears;
            aloan.iPeriod = iPeriods;

            // Call the sgetlistofpayment_1() remote procedure passing it the 
            // sloaninfo structure.  The sgetlistofpayments_1() returns a 
            // linked-list containing the consecutive loan payments for the 
            // information provided.
            retschedual = sgetlistofpayments_1(&aloan, cl);
            
            // If the linked-list returned is NULL an error must have occurred 
            // computing the amortization schedule.
            if (retschedual == NULL)
            {
                cerr << "Error computing loan amortization schedule." << endl;
                continue;
            }

            // If the loan amortization schedule is successfully returned, each
            // payment is displayed for the user.
            currentpayment = retschedual;        // A pointer is set to the 
                                                // first element in the
                                                // linked-list.

            // The linked-list is traversed until no more elements exist.
            while(currentpayment != NULL)
            {
                cout << "Period: " << currentpayment->aPayment.iPeriod << endl;
                cout << "Payment Amount: " << currentpayment->aPayment.dPayment << endl;
                cout << "Interest: " << currentpayment->aPayment.dInterest << endl;
                cout << "Principle: " << currentpayment->aPayment.dPrincipal << endl;
                cout << "Amount of Loan Remaining: " << currentpayment->aPayment.dAmountRemaining << endl << endl;


                // The next element within the linked-list is retrieved.
                currentpayment = currentpayment->next;
            }

            // Free the memory allocated for the linked list.
            xdr_free((int (*)(XDR *, char *))xdr_sschedual, (char *)retschedual);

        }

        // If the user chooses 3, the client loop exits.
        else if (iChoice == 3)
        {
            break;
        }

        // If neither 1 nor 2 is chosen, an error message is displayed. 
        else
        {
            cerr << "Invalid Menu Choice" << endl << endl;
        }
    }

    // The client's resources are freed before the client exits.
    clnt_destroy(cl);

    return 1;
}
