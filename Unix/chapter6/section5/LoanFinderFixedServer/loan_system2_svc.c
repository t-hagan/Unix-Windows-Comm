/*
 * Please do not edit this file.
 * It was generated using rpcgen.
 */

#include "loan_system2.h"
#include <stdio.h>
#include <stdlib.h> /* getenv, exit */
#include <signal.h>
#include <sys/types.h>
#include <memory.h>
#include <stropts.h>
#include <netconfig.h>
#include <sys/resource.h> /* rlimit */
#include <syslog.h>

#ifdef DEBUG
#define    RPC_SVC_FG
#endif

#define    _RPCSVC_CLOSEDOWN 120
static int _rpcpmstart;        /* Started by a port monitor ? */

/* States a server can be in wrt request */

#define    _IDLE 0
#define    _SERVED 1

static int _rpcsvcstate = _IDLE;    /* Set when a request is serviced */
static int _rpcsvccount = 0;        /* Number of requests being serviced */

static
void _msgout(msg)
    char *msg;
{
#ifdef RPC_SVC_FG
    if (_rpcpmstart)
        syslog(LOG_ERR, msg);
    else
        (void) fprintf(stderr, "%s\n", msg);
#else
    syslog(LOG_ERR, msg);
#endif
}

static void
closedown(sig)
    int sig;
{
    if (_rpcsvcstate == _IDLE && _rpcsvccount == 0) {
        extern fd_set svc_fdset;
        static int size;
        int i, openfd;
        struct t_info tinfo;

        if (!t_getinfo(0, &tinfo) && (tinfo.servtype == T_CLTS))
            exit(0);
        if (size == 0) {
            struct rlimit rl;

            rl.rlim_max = 0;
            getrlimit(RLIMIT_NOFILE, &rl);
            if ((size = rl.rlim_max) == 0) {
                return;
            }
        }
        for (i = 0, openfd = 0; i < size && openfd < 2; i++)
            if (FD_ISSET(i, &svc_fdset))
                openfd++;
        if (openfd <= 1)
            exit(0);
    } else
        _rpcsvcstate = _IDLE;

    (void) signal(SIGALRM, (void(*)()) closedown);
    (void) alarm(_RPCSVC_CLOSEDOWN/2);
}

static void
payments_1(rqstp, transp)
    struct svc_req *rqstp;
    register SVCXPRT *transp;
{
    union {
        double dgetpayment_1_arg;
        double sgetlistofpayments_1_arg;
    } argument;
    char *result;
    bool_t (*_xdr_argument)(), (*_xdr_result)();
    char *(*local)();

    _rpcsvccount++;
    switch (rqstp->rq_proc) {
    case NULLPROC:
        (void) svc_sendreply(transp, xdr_void,
            (char *)NULL);
        _rpcsvccount--;
        _rpcsvcstate = _SERVED;
        return;

    case dGetPayment:
        _xdr_argument = xdr_double;
        _xdr_result = xdr_spaymentinfo;
        local = (char *(*)()) dgetpayment_1;
        break;

    case sGetListOfPayments:
        _xdr_argument = xdr_double;
        _xdr_result = xdr_sschedual;
        local = (char *(*)()) sgetlistofpayments_1;
        break;

    default:
        svcerr_noproc(transp);
        _rpcsvccount--;
        _rpcsvcstate = _SERVED;
        return;
    }
    (void) memset((char *)&argument, 0, sizeof (argument));
    if (!svc_getargs(transp, _xdr_argument, (caddr_t) &argument)) {
        svcerr_decode(transp);
        _rpcsvccount--;
        _rpcsvcstate = _SERVED;
        return;
    }
    result = (*local)(&argument, rqstp);
    if (result != NULL && !svc_sendreply(transp, _xdr_result, result)) {
        svcerr_systemerr(transp);
    }
    if (!svc_freeargs(transp, _xdr_argument, (caddr_t) &argument)) {
        _msgout("unable to free arguments");
        exit(1);
    }
    _rpcsvccount--;
    _rpcsvcstate = _SERVED;
    return;
}

main()
{
    pid_t pid;
    int i;

    (void) sigset(SIGPIPE, SIG_IGN);

    /*
     * If stdin looks like a TLI endpoint, we assume
     * that we were started by a port monitor. If
     * t_getstate fails with TBADF, this is not a
     * TLI endpoint.
     */
    if (t_getstate(0) != -1 || t_errno != TBADF) {
        char *netid;
        struct netconfig *nconf = NULL;
        SVCXPRT *transp;
        int pmclose;

        _rpcpmstart = 1;
        openlog("loan_system2", LOG_PID, LOG_DAEMON);

        if ((netid = getenv("NLSPROVIDER")) == NULL) {
        /* started from inetd */
            pmclose = 1;
        } else {
            if ((nconf = getnetconfigent(netid)) == NULL)
                _msgout("cannot get transport info");

            pmclose = (t_getstate(0) != T_DATAXFER);
        }
        if ((transp = svc_tli_create(0, nconf, NULL, 0, 0)) == NULL) {
            _msgout("cannot create server handle");
            exit(1);
        }
        if (nconf)
            freenetconfigent(nconf);
        if (!svc_reg(transp, payments, paymentsver, payments_1, 0)) {
            _msgout("unable to register (payments, paymentsver).");
            exit(1);
        }
        if (pmclose) {
            (void) signal(SIGALRM, (void(*)()) closedown);
            (void) alarm(_RPCSVC_CLOSEDOWN/2);
        }
        svc_run();
        exit(1);
        /* NOTREACHED */
    }    else {
#ifndef RPC_SVC_FG
        int size;
        struct rlimit rl;
        pid = fork();
        if (pid < 0) {
            perror("cannot fork");
            exit(1);
        }
        if (pid)
            exit(0);
        rl.rlim_max = 0;
        getrlimit(RLIMIT_NOFILE, &rl);
        if ((size = rl.rlim_max) == 0)
            exit(1);
        for (i = 0; i < size; i++)
            (void) close(i);
        i = open("/dev/null", 2);
        (void) dup2(i, 1);
        (void) dup2(i, 2);
        setsid();
        openlog("loan_system2", LOG_PID, LOG_DAEMON);
#endif
    }
    if (!svc_create(payments_1, payments, paymentsver, "netpath")) {
        _msgout("unable to create (payments, paymentsver) for netpath.");
        exit(1);
    }

    svc_run();
    _msgout("svc_run returned");
    exit(1);
    /* NOTREACHED */
}
