#include <stdio.h>
#include <rpc/rpc.h>
#include <rpc/pmap_cln.h>
#include "SharedMemServer.h"
#include <powerrpc.h>
static SVCXPRT *tcp_master =0;
static SVCXPRT *udp_master =0;

static void crossplatformmem_1();

main()
{
	SVCXPRT *transp;

	pw_serv_init();
	(void)pmap_unset(crossplatformmem, crossplatformmemver);

	transp = svcudp_create(RPC_ANYSOCK);
	if (transp == NULL) {
		(void)fprintf(stderr, "cannot create udp service.\n");
		exit(1);
	}
		(void)fprintf(stdout, "Created udp service.\n");
	udp_master = transp;
	if (!svc_register(transp, crossplatformmem, crossplatformmemver, crossplatformmem_1, IPPROTO_UDP)) {
		(void)fprintf(stderr, "unable to register (crossplatformmem, crossplatformmemver, udp).\n");
		exit(1);
	}

	transp = svctcp_create(RPC_ANYSOCK, 0, 0);
	if (transp == NULL) {
		(void)fprintf(stderr, "cannot create tcp service.\n");
		exit(1);
	}
		(void)fprintf(stdout, "Created tcp service.\n");
	tcp_master = transp;
	if (!svc_register(transp, crossplatformmem, crossplatformmemver, crossplatformmem_1, IPPROTO_TCP)) {
		(void)fprintf(stderr, "unable to register (crossplatformmem, crossplatformmemver, tcp).\n");
		exit(1);
	}
	pw_serv_mainloop(tcp_master, udp_master, 0, 1);
	(void)fprintf(stderr, "svc_run returned\n");
	exit(1);
}

static void
crossplatformmem_1(rqstp, transp)
	struct svc_req *rqstp;
	SVCXPRT *transp;
{
	union {
		createsharedmem createmem_1_OVER;
		writesharedmem write_1_OVER;
		createsharedmem read_1_OVER;
		int removemem_1_OVER;
	} RPC_over;
	union {
		sharedmemretval createmem_1_BACK;
		sharedmemretval write_1_BACK;
		sharedmem read_1_BACK;
		sharedmemretval removemem_1_BACK;
	} RPC_back;
	pwrpc_call_struct callInfo;
	callInfo.ptransport = transp;
	callInfo.pover = (caddr_t)&RPC_over;
	callInfo.pback = (caddr_t)&RPC_back;
	callInfo.reply_now = 0;
#ifndef PWRPC_MT_ON_CALL
#define PWRPC_MT_ON_CALL 5
#endif
	callInfo.mt_on_call = PWRPC_MT_ON_CALL;
	callInfo.free_after = 0;
	callInfo.proc = 0;
	callInfo.psvc_req = rqstp;

	switch (rqstp->rq_proc) {
	case NULLPROC:
		(void)svc_sendreply(transp, xdr_void, (char *)NULL);
		return;

	case createmem:
		callInfo.xdr_over = (xdrproc_t) xdr_createsharedmem;
		callInfo.over_size = sizeof(RPC_over);
		callInfo.xdr_back = (xdrproc_t)xdr_sharedmemretval;
		callInfo.back_size = sizeof(RPC_back);
		callInfo.local = (local_call_t) createmem_1;
		break;

	case write:
		callInfo.xdr_over = (xdrproc_t) xdr_writesharedmem;
		callInfo.over_size = sizeof(RPC_over);
		callInfo.xdr_back = (xdrproc_t)xdr_sharedmemretval;
		callInfo.back_size = sizeof(RPC_back);
		callInfo.local = (local_call_t) write_1;
		break;

	case read:
		callInfo.xdr_over = (xdrproc_t) xdr_createsharedmem;
		callInfo.over_size = sizeof(RPC_over);
		callInfo.xdr_back = (xdrproc_t)xdr_sharedmem;
		callInfo.back_size = sizeof(RPC_back);
		callInfo.local = (local_call_t) read_1;
		break;

	case removemem:
		callInfo.xdr_over = (xdrproc_t) xdr_int;
		callInfo.over_size = sizeof(RPC_over);
		callInfo.xdr_back = (xdrproc_t)xdr_sharedmemretval;
		callInfo.back_size = sizeof(RPC_back);
		callInfo.local = (local_call_t) removemem_1;
		break;

	default:
		svcerr_noproc(transp);
		return;
	}
	pw_serv_call(&callInfo);

}

