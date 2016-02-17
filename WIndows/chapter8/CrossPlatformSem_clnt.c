#include <rpc/rpc.h>
#include <powerrpc.h>
#include "CrossPlatformSem.h"

/* Default timeout can be changed using clnt_control() */
static struct timeval PWRPC_TIMEOUT_DEF = { 25, 0 };

retvalstruct *
createsem_1(argp, clnt)
	semstruct *argp;
	CLIENT *clnt;
{

#ifdef PWRPC_MT
	_declspec(thread)
#endif
	static retvalstruct res;

	bzero((char *)&res, sizeof(res));
	if (clnt_call(clnt, createsem, xdr_semstruct, argp, xdr_retvalstruct, &res, PWRPC_TIMEOUT) != RPC_SUCCESS) {
		return (NULL);
	}
	return (&res);
}


retvalstruct *
closesem_1(argp, clnt)
	semname *argp;
	CLIENT *clnt;
{

#ifdef PWRPC_MT
	_declspec(thread)
#endif
	static retvalstruct res;

	bzero((char *)&res, sizeof(res));
	if (clnt_call(clnt, closesem, xdr_semname, argp, xdr_retvalstruct, &res, PWRPC_TIMEOUT) != RPC_SUCCESS) {
		return (NULL);
	}
	return (&res);
}


retvalstruct *
removesem_1(argp, clnt)
	semname *argp;
	CLIENT *clnt;
{

#ifdef PWRPC_MT
	_declspec(thread)
#endif
	static retvalstruct res;

	bzero((char *)&res, sizeof(res));
	if (clnt_call(clnt, removesem, xdr_semname, argp, xdr_retvalstruct, &res, PWRPC_TIMEOUT) != RPC_SUCCESS) {
		return (NULL);
	}
	return (&res);
}


retvalstruct *
lockwait_1(argp, clnt)
	semname *argp;
	CLIENT *clnt;
{

#ifdef PWRPC_MT
	_declspec(thread)
#endif
	static retvalstruct res;

	bzero((char *)&res, sizeof(res));
	if (clnt_call(clnt, lockwait, xdr_semname, argp, xdr_retvalstruct, &res, PWRPC_TIMEOUT) != RPC_SUCCESS) {
		return (NULL);
	}
	return (&res);
}


retvalstruct *
locknowait_1(argp, clnt)
	semname *argp;
	CLIENT *clnt;
{

#ifdef PWRPC_MT
	_declspec(thread)
#endif
	static retvalstruct res;

	bzero((char *)&res, sizeof(res));
	if (clnt_call(clnt, locknowait, xdr_semname, argp, xdr_retvalstruct, &res, PWRPC_TIMEOUT) != RPC_SUCCESS) {
		return (NULL);
	}
	return (&res);
}


retvalstruct *
unlock_1(argp, clnt)
	semname *argp;
	CLIENT *clnt;
{

#ifdef PWRPC_MT
	_declspec(thread)
#endif
	static retvalstruct res;

	bzero((char *)&res, sizeof(res));
	if (clnt_call(clnt, unlock, xdr_semname, argp, xdr_retvalstruct, &res, PWRPC_TIMEOUT) != RPC_SUCCESS) {
		return (NULL);
	}
	return (&res);
}


retvalstruct *
getvalue_1(argp, clnt)
	semname *argp;
	CLIENT *clnt;
{

#ifdef PWRPC_MT
	_declspec(thread)
#endif
	static retvalstruct res;

	bzero((char *)&res, sizeof(res));
	if (clnt_call(clnt, getvalue, xdr_semname, argp, xdr_retvalstruct, &res, PWRPC_TIMEOUT) != RPC_SUCCESS) {
		return (NULL);
	}
	return (&res);
}

