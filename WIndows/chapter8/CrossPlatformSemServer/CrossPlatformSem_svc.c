#include <stdio.h>
#include <rpc/rpc.h>
#include <rpc/pmap_cln.h>
#include "CrossPlatformSem.h"
#include <powerrpc.h>
static SVCXPRT *tcp_master =0;
static SVCXPRT *udp_master =0;

static void crossplatformsem_1();

main()
{
    SVCXPRT *transp;

    pw_serv_init();
    (void)pmap_unset(crossplatformsem, crossplatformsemver);

    transp = svcudp_create(RPC_ANYSOCK);
    if (transp == NULL) {
        (void)fprintf(stderr, "cannot create udp service.\n");
        exit(1);
    }
        (void)fprintf(stdout, "Created udp service.\n");
    udp_master = transp;
    if (!svc_register(transp, crossplatformsem, crossplatformsemver, crossplatformsem_1, IPPROTO_UDP)) {
        (void)fprintf(stderr, "unable to register (crossplatformsem, crossplatformsemver, udp).\n");
        exit(1);
    }

    transp = svctcp_create(RPC_ANYSOCK, 0, 0);
    if (transp == NULL) {
        (void)fprintf(stderr, "cannot create tcp service.\n");
        exit(1);
    }
        (void)fprintf(stdout, "Created tcp service.\n");
    tcp_master = transp;
    if (!svc_register(transp, crossplatformsem, crossplatformsemver, crossplatformsem_1, IPPROTO_TCP)) {
        (void)fprintf(stderr, "unable to register (crossplatformsem, crossplatformsemver, tcp).\n");
        exit(1);
    }
    pw_serv_mainloop(tcp_master, udp_master, 0, 1);
    (void)fprintf(stderr, "svc_run returned\n");
    exit(1);
}

static void
crossplatformsem_1(rqstp, transp)
    struct svc_req *rqstp;
    SVCXPRT *transp;
{
    union {
        semstruct createsem_1_OVER;
        semname closesem_1_OVER;
        semname removesem_1_OVER;
        semname lockwait_1_OVER;
        semname locknowait_1_OVER;
        semname unlock_1_OVER;
        semname getvalue_1_OVER;
    } RPC_over;
    union {
        retvalstruct createsem_1_BACK;
        retvalstruct closesem_1_BACK;
        retvalstruct removesem_1_BACK;
        retvalstruct lockwait_1_BACK;
        retvalstruct locknowait_1_BACK;
        retvalstruct unlock_1_BACK;
        retvalstruct getvalue_1_BACK;
    } RPC_back;
    pwrpc_call_struct callInfo;
    callInfo.ptransport = transp;
    callInfo.pover = (caddr_t)&RPC_over;
    callInfo.pback = (caddr_t)&RPC_back;
    callInfo.reply_now = 0;
#ifndef PWRPC_MT_ON_CALL
#define PWRPC_MT_ON_CALL 6
#endif
    callInfo.mt_on_call = PWRPC_MT_ON_CALL;
    callInfo.free_after = 0;
    callInfo.proc = 0;
    callInfo.psvc_req = rqstp;

    switch (rqstp->rq_proc) {
    case NULLPROC:
        (void)svc_sendreply(transp, xdr_void, (char *)NULL);
        return;

    case createsem:
        callInfo.xdr_over = (xdrproc_t) xdr_semstruct;
        callInfo.over_size = sizeof(RPC_over);
        callInfo.xdr_back = (xdrproc_t)xdr_retvalstruct;
        callInfo.back_size = sizeof(RPC_back);
        callInfo.local = (local_call_t) createsem_1;
        break;

    case closesem:
        callInfo.xdr_over = (xdrproc_t) xdr_semname;
        callInfo.over_size = sizeof(RPC_over);
        callInfo.xdr_back = (xdrproc_t)xdr_retvalstruct;
        callInfo.back_size = sizeof(RPC_back);
        callInfo.local = (local_call_t) closesem_1;
        break;

    case removesem:
        callInfo.xdr_over = (xdrproc_t) xdr_semname;
        callInfo.over_size = sizeof(RPC_over);
        callInfo.xdr_back = (xdrproc_t)xdr_retvalstruct;
        callInfo.back_size = sizeof(RPC_back);
        callInfo.local = (local_call_t) removesem_1;
        break;

    case lockwait:
        callInfo.xdr_over = (xdrproc_t) xdr_semname;
        callInfo.over_size = sizeof(RPC_over);
        callInfo.xdr_back = (xdrproc_t)xdr_retvalstruct;
        callInfo.back_size = sizeof(RPC_back);
        callInfo.local = (local_call_t) lockwait_1;
        break;

    case locknowait:
        callInfo.xdr_over = (xdrproc_t) xdr_semname;
        callInfo.over_size = sizeof(RPC_over);
        callInfo.xdr_back = (xdrproc_t)xdr_retvalstruct;
        callInfo.back_size = sizeof(RPC_back);
        callInfo.local = (local_call_t) locknowait_1;
        break;

    case unlock:
        callInfo.xdr_over = (xdrproc_t) xdr_semname;
        callInfo.over_size = sizeof(RPC_over);
        callInfo.xdr_back = (xdrproc_t)xdr_retvalstruct;
        callInfo.back_size = sizeof(RPC_back);
        callInfo.local = (local_call_t) unlock_1;
        break;

    case getvalue:
        callInfo.xdr_over = (xdrproc_t) xdr_semname;
        callInfo.over_size = sizeof(RPC_over);
        callInfo.xdr_back = (xdrproc_t)xdr_retvalstruct;
        callInfo.back_size = sizeof(RPC_back);
        callInfo.local = (local_call_t) getvalue_1;
        break;

    default:
        svcerr_noproc(transp);
        return;
    }
    pw_serv_call(&callInfo);

}

