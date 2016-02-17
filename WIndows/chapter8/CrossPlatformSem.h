/* License: @%$Netbula LLC, Demo software, commercial distrbution prohibited. */
#include <rpc/rpc.h>
#include <rpc/types.h>

/*redefine below to use different timeout*/
#ifndef PWRPC_TIMEOUT
#define PWRPC_TIMEOUT PWRPC_TIMEOUT_DEF
#endif

#ifndef _RPCAPI
#define _RPCAPI  /* wanna change this? */
#endif


typedef char *semname;
bool_t xdr_semname(XDR *, semname *);


struct semstruct {
	semname ssemname;
	int iinitvalue;
	int imaxvalue;
};
typedef struct semstruct semstruct;
bool_t xdr_semstruct(XDR *, semstruct *);


struct retvalstruct {
	int iretval;
	char *serrormsg;
};
typedef struct retvalstruct retvalstruct;
bool_t xdr_retvalstruct(XDR *, retvalstruct *);


#define crossplatformsem ((u_long)0x22100000)
#define crossplatformsemver ((u_long)1)
#define createsem ((u_long)1)
_RPCAPI extern retvalstruct *createsem_1(semstruct *, CLIENT *);
#define closesem ((u_long)2)
_RPCAPI extern retvalstruct *closesem_1(semname *, CLIENT *);
#define removesem ((u_long)3)
_RPCAPI extern retvalstruct *removesem_1(semname *, CLIENT *);
#define lockwait ((u_long)4)
_RPCAPI extern retvalstruct *lockwait_1(semname *, CLIENT *);
#define locknowait ((u_long)5)
_RPCAPI extern retvalstruct *locknowait_1(semname *, CLIENT *);
#define unlock ((u_long)6)
_RPCAPI extern retvalstruct *unlock_1(semname *, CLIENT *);
#define getvalue ((u_long)7)
_RPCAPI extern retvalstruct *getvalue_1(semname *, CLIENT *);

