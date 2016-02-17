#include <rpc/rpc.h>
#include <powerrpc.h>
#include "loan_system3.h"

/* Default timeout can be changed using clnt_control() */
static struct timeval PWRPC_TIMEOUT_DEF = { 25, 0 };

spaymentinfo *
dgetpayment_1(argp, clnt)
    sloaninfo *argp;
    CLIENT *clnt;
{

#ifdef PWRPC_MT
    _declspec(thread)
#endif
    static spaymentinfo res;

    bzero((char *)&res, sizeof(res));
    if (clnt_call(clnt, dGetPayment, xdr_sloaninfo, argp, xdr_spaymentinfo, &res, PWRPC_TIMEOUT) != RPC_SUCCESS) {
        return (NULL);
    }
    return (&res);
}


sschedual *
sgetlistofpayments_1(argp, clnt)
    sloaninfo *argp;
    CLIENT *clnt;
{

#ifdef PWRPC_MT
    _declspec(thread)
#endif
    static sschedual res;

    bzero((char *)&res, sizeof(res));
    if (clnt_call(clnt, sGetListOfPayments, xdr_sloaninfo, argp, xdr_sschedual, &res, PWRPC_TIMEOUT) != RPC_SUCCESS) {
        return (NULL);
    }
    return (&res);
}

