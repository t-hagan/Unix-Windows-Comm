#include <rpc/rpc.h>
#include <powerrpc.h>
#include "SharedMemServer.h"

/* Default timeout can be changed using clnt_control() */
static struct timeval PWRPC_TIMEOUT_DEF = { 25, 0 };

sharedmemretval *
createmem_1(argp, clnt)
	createsharedmem *argp;
	CLIENT *clnt;
{

#ifdef PWRPC_MT
	_declspec(thread)
#endif
	static sharedmemretval res;

	bzero((char *)&res, sizeof(res));
	if (clnt_call(clnt, createmem, xdr_createsharedmem, argp, xdr_sharedmemretval, &res, PWRPC_TIMEOUT) != RPC_SUCCESS) {
		return (NULL);
	}
	return (&res);
}


sharedmemretval *
write_1(argp, clnt)
	writesharedmem *argp;
	CLIENT *clnt;
{

#ifdef PWRPC_MT
	_declspec(thread)
#endif
	static sharedmemretval res;

	bzero((char *)&res, sizeof(res));
	if (clnt_call(clnt, write, xdr_writesharedmem, argp, xdr_sharedmemretval, &res, PWRPC_TIMEOUT) != RPC_SUCCESS) {
		return (NULL);
	}
	return (&res);
}


sharedmem *
read_1(argp, clnt)
	createsharedmem *argp;
	CLIENT *clnt;
{

#ifdef PWRPC_MT
	_declspec(thread)
#endif
	static sharedmem res;

	bzero((char *)&res, sizeof(res));
	if (clnt_call(clnt, read, xdr_createsharedmem, argp, xdr_sharedmem, &res, PWRPC_TIMEOUT) != RPC_SUCCESS) {
		return (NULL);
	}
	return (&res);
}


sharedmemretval *
removemem_1(argp, clnt)
	int *argp;
	CLIENT *clnt;
{

#ifdef PWRPC_MT
	_declspec(thread)
#endif
	static sharedmemretval res;

	bzero((char *)&res, sizeof(res));
	if (clnt_call(clnt, removemem, xdr_int, argp, xdr_sharedmemretval, &res, PWRPC_TIMEOUT) != RPC_SUCCESS) {
		return (NULL);
	}
	return (&res);
}

