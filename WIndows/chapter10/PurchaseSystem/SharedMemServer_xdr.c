#include <rpc/rpc.h>
#include "SharedMemServer.h"


bool_t
xdr_sharedmem(xdrs, objp)
	XDR *xdrs;
	sharedmem *objp;
{
	if (!xdr_array(xdrs, (char **)&objp->sharedmem_val, (u_int *)&objp->sharedmem_len, ~0, sizeof(char), xdr_char)) {
		return (FALSE);
	}
	return (TRUE);
}




bool_t
xdr_createsharedmem(xdrs, objp)
	XDR *xdrs;
	createsharedmem *objp;
{
	if (!xdr_int(xdrs, &objp->ikey)) {
		return (FALSE);
	}
	if (!xdr_int(xdrs, &objp->isize)) {
		return (FALSE);
	}
	return (TRUE);
}




bool_t
xdr_writesharedmem(xdrs, objp)
	XDR *xdrs;
	writesharedmem *objp;
{
	if (!xdr_int(xdrs, &objp->ikey)) {
		return (FALSE);
	}
	if (!xdr_sharedmem(xdrs, &objp->data)) {
		return (FALSE);
	}
	return (TRUE);
}




bool_t
xdr_sharedmemretval(xdrs, objp)
	XDR *xdrs;
	sharedmemretval *objp;
{
	if (!xdr_int(xdrs, &objp->iretval)) {
		return (FALSE);
	}
	if (!xdr_string(xdrs, &objp->serrormsg, ~0)) {
		return (FALSE);
	}
	return (TRUE);
}


