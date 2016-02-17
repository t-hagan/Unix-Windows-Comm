#include <rpc/rpc.h>
#include "loan_system3.h"


bool_t
xdr_ssinglepayment(xdrs, objp)
    XDR *xdrs;
    ssinglepayment *objp;
{
    if (!xdr_int(xdrs, &objp->iPeriod)) {
        return (FALSE);
    }
    if (!xdr_double(xdrs, &objp->dPayment)) {
        return (FALSE);
    }
    if (!xdr_double(xdrs, &objp->dInterest)) {
        return (FALSE);
    }
    if (!xdr_double(xdrs, &objp->dPrincipal)) {
        return (FALSE);
    }
    if (!xdr_double(xdrs, &objp->dAmountRemaining)) {
        return (FALSE);
    }
    return (TRUE);
}




bool_t
xdr_sloaninfo(xdrs, objp)
    XDR *xdrs;
    sloaninfo *objp;
{
    if (!xdr_double(xdrs, &objp->dAmount)) {
        return (FALSE);
    }
    if (!xdr_int(xdrs, &objp->iPeriod)) {
        return (FALSE);
    }
    if (!xdr_int(xdrs, &objp->iYear)) {
        return (FALSE);
    }
    return (TRUE);
}




bool_t
xdr_spaymentinfo(xdrs, objp)
    XDR *xdrs;
    spaymentinfo *objp;
{
    if (!xdr_array(xdrs, (char **)&objp->sBankName.sBankName_val, (u_int *)&objp->sBankName.sBankName_len, MAX_BANK_NAME, sizeof(char), xdr_char)) {
        return (FALSE);
    }
    if (!xdr_double(xdrs, &objp->dPayment)) {
        return (FALSE);
    }
    return (TRUE);
}




bool_t
xdr_sschedual(xdrs, objp)
    XDR *xdrs;
    sschedual *objp;
{
    if (!xdr_ssinglepayment(xdrs, &objp->aPayment)) {
        return (FALSE);
    }
    if (!xdr_pointer(xdrs, (char **)&objp->next, sizeof(sschedual), xdr_sschedual)) {
        return (FALSE);
    }
    return (TRUE);
}


