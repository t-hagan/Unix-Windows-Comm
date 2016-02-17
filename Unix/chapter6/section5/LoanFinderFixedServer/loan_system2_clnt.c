/*
 * Please do not edit this file.
 * It was generated using rpcgen.
 */

#include "loan_system2.h"
#include <stdio.h>
#include <stdlib.h> /* getenv, exit */

/* Default timeout can be changed using clnt_control() */
static struct timeval TIMEOUT = { 25, 0 };

spaymentinfo *
dgetpayment_1(argp, clnt)
    double *argp;
    CLIENT *clnt;
{
    static spaymentinfo clnt_res;

    memset((char *)&clnt_res, 0, sizeof (clnt_res));
    if (clnt_call(clnt, dGetPayment,
        (xdrproc_t) xdr_double, (caddr_t) argp,
        (xdrproc_t) xdr_spaymentinfo, (caddr_t) &clnt_res,
        TIMEOUT) != RPC_SUCCESS) {
        return (NULL);
    }
    return (&clnt_res);
}

sschedual *
sgetlistofpayments_1(argp, clnt)
    double *argp;
    CLIENT *clnt;
{
    static sschedual clnt_res;

    memset((char *)&clnt_res, 0, sizeof (clnt_res));
    if (clnt_call(clnt, sGetListOfPayments,
        (xdrproc_t) xdr_double, (caddr_t) argp,
        (xdrproc_t) xdr_sschedual, (caddr_t) &clnt_res,
        TIMEOUT) != RPC_SUCCESS) {
        return (NULL);
    }
    return (&clnt_res);
}
