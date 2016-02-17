/* License: @%$Netbula LLC, Demo software, commercial distrbution prohibited. */
#include <rpc/types.h>

/*redefine below to use different timeout*/
#ifndef PWRPC_TIMEOUT
#define PWRPC_TIMEOUT PWRPC_TIMEOUT_DEF
#endif

#ifndef _RPCAPI
#define _RPCAPI  /* wanna change this? */
#endif


typedef struct {
	u_int sharedmem_len;
	char *sharedmem_val;
} sharedmem;
bool_t xdr_sharedmem(XDR *, sharedmem *);


struct createsharedmem {
	int ikey;
	int isize;
};
typedef struct createsharedmem createsharedmem;
bool_t xdr_createsharedmem(XDR *, createsharedmem *);


struct writesharedmem {
	int ikey;
	sharedmem data;
};
typedef struct writesharedmem writesharedmem;
bool_t xdr_writesharedmem(XDR *, writesharedmem *);


struct sharedmemretval {
	int iretval;
	char *serrormsg;
};
typedef struct sharedmemretval sharedmemretval;
bool_t xdr_sharedmemretval(XDR *, sharedmemretval *);


#define crossplatformmem ((u_long)0x22200000)
#define crossplatformmemver ((u_long)1)
#define createmem ((u_long)1)
_RPCAPI extern sharedmemretval *createmem_1(createsharedmem *, CLIENT *);
#define write ((u_long)2)
_RPCAPI extern sharedmemretval *write_1(writesharedmem *, CLIENT *);
#define read ((u_long)3)
_RPCAPI extern sharedmem *read_1(createsharedmem *, CLIENT *);
#define removemem ((u_long)4)
_RPCAPI extern sharedmemretval *removemem_1(int *, CLIENT *);

