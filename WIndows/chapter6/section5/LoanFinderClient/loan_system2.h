/* License: @%$Netbula LLC, Demo software, commercial distrbution prohibited. */
#include <rpc/types.h>

/*redefine below to use different timeout*/
#ifndef PWRPC_TIMEOUT
#define PWRPC_TIMEOUT PWRPC_TIMEOUT_DEF
#endif

#ifndef _RPCAPI
#define _RPCAPI  /* wanna change this? */
#endif

#define MAX_BANK_NAME 512
#define MAX_PAYMENTS 360

struct ssinglepayment {
    int iPeriod;
    double dPayment;
    double dInterest;
    double dPrincipal;
    double dAmountRemaining;
};
typedef struct ssinglepayment ssinglepayment;
bool_t xdr_ssinglepayment(XDR *, ssinglepayment *);


struct spaymentinfo {
    struct {
        u_int sBankName_len;
        char *sBankName_val;
    } sBankName;
    double dPayment;
};
typedef struct spaymentinfo spaymentinfo;
bool_t xdr_spaymentinfo(XDR *, spaymentinfo *);


struct sschedual {
    ssinglepayment listofpayments[MAX_PAYMENTS];
};
typedef struct sschedual sschedual;
bool_t xdr_sschedual(XDR *, sschedual *);


#define payments ((u_long)0x22000000)
#define paymentsver ((u_long)1)
#define dGetPayment ((u_long)1)
_RPCAPI extern spaymentinfo *dgetpayment_1(double *, CLIENT *);
#define sGetListOfPayments ((u_long)2)
_RPCAPI extern sschedual *sgetlistofpayments_1(double *, CLIENT *);

