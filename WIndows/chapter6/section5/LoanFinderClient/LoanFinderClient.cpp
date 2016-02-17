#include <iostream.h>
#include <rpc/rpc.h>

extern "C"
{
#include "loan_system2.h"
}

// RPC server name definition.
#define SERVER_HOST_NAME "aHost"

main()
{
    double dLoanAmount;                    // Holds the amount of the loan.
    struct spaymentinfo *retpayment;    // A pointer to the server's return value.
    struct sschedual *retschedual;        // Holds the return from the 
                                        // sgetlistofpayments_1 remote procedure.
    
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
        cout << "(1)Locate Bank with Lowest Loan Payment" << endl;
        cout << "(2) Display an Amortization Schedule" << endl;
        cout << "(3) Exit" << endl;
        cout << "Enter Your Choice: " << flush;
        cin >> iChoice;


        // If the user chooses 1, the user is prompted for the amount of the loan.
        // A pointer to the loan amount along with the client connection 
        // information is passed to the dgetpayment_1() client stub function.  If 
        // the return value is NULL an error occurred. Otherwise the bank name and 
        // loan payment amount is displayed for the user.  The last step frees 
        // the memory allocated for holding the bank name.
        if (iChoice == 1)
        {

            cout << "Enter Loan Amount: " << flush;
            cin >> dLoanAmount;

            retpayment = dgetpayment_1(&dLoanAmount, cl);
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

        // If the user chooses the second menu item, they are prompted for the
        // loan amount.  The loan amount is used as a parameter for the 
        // sgetlistofpayments_1() remote procedure call.  The
        //  sgetlistofpayments_1() remote procedure call returns an instance of 
        // sschedual.  If the return  value is NULL, an error occurred computing
        // the loan amortization  schedule.  Otherwise, the schedule is displayed 
        // for the user and the memory used for the sschedual instance is freed.
        else if (iChoice == 2)
        {
            cout << "Enter Loan Amount: " << flush;
            cin >> dLoanAmount;

            retschedual = sgetlistofpayments_1(&dLoanAmount, cl);
            if (retschedual == NULL)
            {
                cerr << clnt_spcreateerror("Error calling sgetlistofpayments_1: ") << endl;
                continue;
            }

            for (int count = 0; count < MAX_PAYMENTS; count++)
            {
                cout << "Period: " << retschedual->listofpayments[count].iPeriod << endl;
                cout << "Payment Amount: " << retschedual->listofpayments[count].dPayment << endl;
                cout << "Interest: " << retschedual->listofpayments[count].dInterest << endl;
                cout << "Principle: " << retschedual->listofpayments[count].dPrincipal << endl;
                cout << "Amount of Loan Remaining: " << retschedual->listofpayments[count].dAmountRemaining << endl << endl;
            }
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
