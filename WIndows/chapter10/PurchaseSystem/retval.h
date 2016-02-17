#ifndef _retval
#define _retval

struct retvalstruct {
	int iretval;
	char *serrormsg;
};
typedef struct retvalstruct retvalstruct;
bool_t xdr_retvalstruct(XDR *, retvalstruct *);

#endif