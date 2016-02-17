/*
 * Please do not edit this file.
 * It was generated using rpcgen.
 */

#include "CrossPlatformSem.h"
#include <stdio.h>
#include <stdlib.h> /* getenv, exit */

/* Default timeout can be changed using clnt_control() */
static struct timeval TIMEOUT = { 25, 0 };

retvalstruct *
createsem_1(argp, clnt)
    semstruct *argp;
    CLIENT *clnt;
{
    static retvalstruct clnt_res;

    memset((char *)&clnt_res, 0, sizeof (clnt_res));
    if (clnt_call(clnt, createsem,
        (xdrproc_t) xdr_semstruct, (caddr_t) argp,
        (xdrproc_t) xdr_retvalstruct, (caddr_t) &clnt_res,
        TIMEOUT) != RPC_SUCCESS) {
        return (NULL);
    }
    return (&clnt_res);
}

retvalstruct *
closesem_1(argp, clnt)
    semname *argp;
    CLIENT *clnt;
{
    static retvalstruct clnt_res;

    memset((char *)&clnt_res, 0, sizeof (clnt_res));
    if (clnt_call(clnt, closesem,
        (xdrproc_t) xdr_semname, (caddr_t) argp,
        (xdrproc_t) xdr_retvalstruct, (caddr_t) &clnt_res,
        TIMEOUT) != RPC_SUCCESS) {
        return (NULL);
    }
    return (&clnt_res);
}

retvalstruct *
removesem_1(argp, clnt)
    semname *argp;
    CLIENT *clnt;
{
    static retvalstruct clnt_res;

    memset((char *)&clnt_res, 0, sizeof (clnt_res));
    if (clnt_call(clnt, removesem,
        (xdrproc_t) xdr_semname, (caddr_t) argp,
        (xdrproc_t) xdr_retvalstruct, (caddr_t) &clnt_res,
        TIMEOUT) != RPC_SUCCESS) {
        return (NULL);
    }
    return (&clnt_res);
}

retvalstruct *
lockwait_1(argp, clnt)
    semname *argp;
    CLIENT *clnt;
{
    static retvalstruct clnt_res;
    struct timeval semlocktime;    // Holds the clnt_call() timeout value

    // Increase the timeout value
    semlocktime.tv_sec = 300000000;
    semlocktime.tv_usec = 0;
    
    memset((char *)&clnt_res, 0, sizeof (clnt_res));
    if (clnt_call(clnt, lockwait,
        (xdrproc_t) xdr_semname, (caddr_t) argp,
        (xdrproc_t) xdr_retvalstruct, (caddr_t) &clnt_res,
        semlocktime) != RPC_SUCCESS) {
        return (NULL);
    }
    return (&clnt_res);
}

retvalstruct *
locknowait_1(argp, clnt)
    semname *argp;
    CLIENT *clnt;
{
    static retvalstruct clnt_res;

    memset((char *)&clnt_res, 0, sizeof (clnt_res));
    if (clnt_call(clnt, locknowait,
        (xdrproc_t) xdr_semname, (caddr_t) argp,
        (xdrproc_t) xdr_retvalstruct, (caddr_t) &clnt_res,
        TIMEOUT) != RPC_SUCCESS) {
        return (NULL);
    }
    return (&clnt_res);
}

retvalstruct *
unlock_1(argp, clnt)
    semname *argp;
    CLIENT *clnt;
{
    static retvalstruct clnt_res;

    memset((char *)&clnt_res, 0, sizeof (clnt_res));
    if (clnt_call(clnt, unlock,
        (xdrproc_t) xdr_semname, (caddr_t) argp,
        (xdrproc_t) xdr_retvalstruct, (caddr_t) &clnt_res,
        TIMEOUT) != RPC_SUCCESS) {
        return (NULL);
    }
    return (&clnt_res);
}

retvalstruct *
getvalue_1(argp, clnt)
    semname *argp;
    CLIENT *clnt;
{
    static retvalstruct clnt_res;

    memset((char *)&clnt_res, 0, sizeof (clnt_res));
    if (clnt_call(clnt, getvalue,
        (xdrproc_t) xdr_semname, (caddr_t) argp,
        (xdrproc_t) xdr_retvalstruct, (caddr_t) &clnt_res,
        TIMEOUT) != RPC_SUCCESS) {
        return (NULL);
    }
    return (&clnt_res);
}