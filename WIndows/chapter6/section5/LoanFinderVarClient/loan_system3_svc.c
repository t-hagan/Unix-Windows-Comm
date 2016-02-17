#include <stdio.h>
#include <rpc/rpc.h>
#include <rpc/pmap_cln.h>
#include "loan_system3.h"
#include <powerrpc.h>
static SVCXPRT *tcp_master =0;
static SVCXPRT *udp_master =0;

static void payments_1();

main()
{
    SVCXPRT *transp;

    pw_serv_init();
    (void)pmap_unset(payments, paymentsver);

    transp = svcudp_create(RPC_ANYSOCK);
    if (transp == NULL) {
        (void)fprintf(stderr, "cannot create udp service.\n");
        exit(1);
    }
        (void)fprintf(stdout, "Created udp service.\n");
    udp_master = transp;
    if (!svc_register(transp, payments, paymentsver, payments_1, IPPROTO_UDP)) {
        (void)fprintf(stderr, "unable to register (payments, paymentsver, udp).\n");
        exit(1);
    }

    transp = svctcp_create(RPC_ANYSOCK, 0, 0);
    if (transp == NULL) {
        (void)fprintf(stderr, "cannot create tcp service.\n");
        exit(1);
    }
        (void)fprintf(stdout, "Created tcp service.\n");
    tcp_master = transp;
    if (!svc_register(transp, payments, paymentsver, payments_1, IPPROTO_TCP)) {
        (void)fprintf(stderr, "unable to register (payments, paymentsver, tcp).\n");
        exit(1);
    }
    pw_serv_mainloop(tcp_master, udp_master, 0, 1);
    (void)fprintf(stderr, "svc_run returned\n");
    exit(1);
}

static void
payments_1(rqstp, transp)
    struct svc_req *rqstp;
    SVCXPRT *transp;
{
    union {
        sloaninfo dgetpayment_1_OVER;
        sloaninfo sgetlistofpayments_1_OVER;
    } RPC_over;
    union {
        spaymentinfo dgetpayment_1_BACK;
        sschedual sgetlistofpayments_1_BACK;
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

    case dGetPayment:
        callInfo.xdr_over = (xdrproc_t) xdr_sloaninfo;
        callInfo.over_size = sizeof(RPC_over);
        callInfo.xdr_back = (xdrproc_t)xdr_spaymentinfo;
        callInfo.back_size = sizeof(RPC_back);
        callInfo.local = (local_call_t) dgetpayment_1;
        break;

    case sGetListOfPayments:
        callInfo.xdr_over = (xdrproc_t) xdr_sloaninfo;
        callInfo.over_size = sizeof(RPC_over);
        callInfo.xdr_back = (xdrproc_t)xdr_sschedual;
        callInfo.back_size = sizeof(RPC_back);
        callInfo.local = (local_call_t) sgetlistofpayments_1;
        break;

    default:
        svcerr_noproc(transp);
        return;
    }
    pw_serv_call(&callInfo);

}

