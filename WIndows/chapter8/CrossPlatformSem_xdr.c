#include <rpc/rpc.h>
#include "CrossPlatformSem.h"


bool_t
xdr_semname(xdrs, objp)
	XDR *xdrs;
	semname *objp;
{
	if (!xdr_string(xdrs, objp, ~0)) {
		return (FALSE);
	}
	return (TRUE);
}




bool_t
xdr_semstruct(xdrs, objp)
	XDR *xdrs;
	semstruct *objp;
{
	if (!xdr_semname(xdrs, &objp->ssemname)) {
		return (FALSE);
	}
	if (!xdr_int(xdrs, &objp->iinitvalue)) {
		return (FALSE);
	}
	if (!xdr_int(xdrs, &objp->imaxvalue)) {
		return (FALSE);
	}
	return (TRUE);
}




bool_t
xdr_retvalstruct(xdrs, objp)
	XDR *xdrs;
	retvalstruct *objp;
{
	if (!xdr_int(xdrs, &objp->iretval)) {
		return (FALSE);
	}
	if (!xdr_string(xdrs, &objp->serrormsg, ~0)) {
		return (FALSE);
	}
	return (TRUE);
}


