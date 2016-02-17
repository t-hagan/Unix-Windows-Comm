#include <rpc/rpc.h>
#include "loan_system.h"


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


