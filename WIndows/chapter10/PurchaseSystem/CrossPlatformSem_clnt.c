#include <rpc/rpc.h>
#include <powerrpc.h>
#include "CrossPlatformSem.h"

/* Default timeout can be changed using clnt_control() */
static struct timeval PWRPC_TIMEOUT_DEF = { 25, 0 };

retvalstruct *
icreatesem_1(argp, clnt)
	semstruct *argp;
	CLIENT *clnt;
{

#ifdef PWRPC_MT
	_declspec(thread)
#endif
	static retvalstruct res;

	bzero((char *)&res, sizeof(res));
	if (clnt_call(clnt, icreatesem, xdr_semstruct, argp, xdr_retvalstruct, &res, PWRPC_TIMEOUT_DEF) != RPC_SUCCESS) {
		return (NULL);
	}
	return (&res);
}


retvalstruct *
iclosesem_1(argp, clnt)
	semname *argp;
	CLIENT *clnt;
{

#ifdef PWRPC_MT
	_declspec(thread)
#endif
	static retvalstruct res;

	bzero((char *)&res, sizeof(res));
	if (clnt_call(clnt, iclosesem, xdr_semname, argp, xdr_retvalstruct, &res, PWRPC_TIMEOUT_DEF) != RPC_SUCCESS) {
		return (NULL);
	}
	return (&res);
}


retvalstruct *
iremovesemaphore_1(argp, clnt)
	semname *argp;
	CLIENT *clnt;
{

#ifdef PWRPC_MT
	_declspec(thread)
#endif
	static retvalstruct res;

	bzero((char *)&res, sizeof(res));
	if (clnt_call(clnt, iremovesemaphore, xdr_semname, argp, xdr_retvalstruct, &res, PWRPC_TIMEOUT_DEF) != RPC_SUCCESS) {
		return (NULL);
	}
	return (&res);
}


retvalstruct *
ilockwait_1(argp, clnt)
	semname *argp;
	CLIENT *clnt;
{

#ifdef PWRPC_MT
	_declspec(thread)
#endif
	static retvalstruct res;

	bzero((char *)&res, sizeof(res));
	if (clnt_call(clnt, ilockwait, xdr_semname, argp, xdr_retvalstruct, &res, PWRPC_TIMEOUT_DEF) != RPC_SUCCESS) {
		return (NULL);
	}
	return (&res);
}


retvalstruct *
ilocknowait_1(argp, clnt)
	semname *argp;
	CLIENT *clnt;
{

#ifdef PWRPC_MT
	_declspec(thread)
#endif
	static retvalstruct res;

	bzero((char *)&res, sizeof(res));
	if (clnt_call(clnt, ilocknowait, xdr_semname, argp, xdr_retvalstruct, &res, PWRPC_TIMEOUT_DEF) != RPC_SUCCESS) {
		return (NULL);
	}
	return (&res);
}


retvalstruct *
iunlock_1(argp, clnt)
	semname *argp;
	CLIENT *clnt;
{

#ifdef PWRPC_MT
	_declspec(thread)
#endif
	static retvalstruct res;

	bzero((char *)&res, sizeof(res));
	if (clnt_call(clnt, iunlock, xdr_semname, argp, xdr_retvalstruct, &res, PWRPC_TIMEOUT_DEF) != RPC_SUCCESS) {
		return (NULL);
	}
	return (&res);
}

