#include <iostream.h>
#include <rpc/rpc.h>

extern "C"
{
#include "loan_system.h"
}

// RPC server name definition.
#define SERVER_HOST_NAME "aHost"

main()
{
    double dLoanAmount;                    // Holds the amount of the loan.
    struct spaymentinfo *retpayment;    // A pointer to the server's return value.
    CLIENT *cl;                            // A pointer to server location information.
    int iChoice;                        // Client menu choice.

    // Create a connection to the server.  If the connection fails an error
    // message is displayed and the program exits.
    cl = clnt_create(SERVER_HOST_NAME, payments, paymentsver, "udp");
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
        cout << "(2)Exit" << endl;
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
        // If the user chooses 2, the client loop exits.
        else if (iChoice == 2)
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
