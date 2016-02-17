INC_DIR = -I$(ROOT_DIR)/chapter1 -I$(ROOT_DIR)/chapter2/section3 -I.

all: CrossPlatformSemServer

CrossPlatformSem_xdr.o: CrossPlatformSem_xdr.c
	gcc -c CrossPlatformSem_xdr.c -g -o CrossPlatformSem_xdr.o

CrossPlatformSem_svc.o: CrossPlatformSem_svc.c
	gcc -c CrossPlatformSem_svc.c -g -o CrossPlatformSem_svc.o

CrossPlatformSem_server.o: CrossPlatformSem_server.C
	CC $(INC_DIR) -DUNIX -c CrossPlatformSem_server.C -g -o CrossPlatformSem_server.o

CrossPlatformSemServer: CrossPlatformSem_xdr.o CrossPlatformSem_svc.o CrossPlatformSem_server.o
	CC -o CrossPlatformSemServer CrossPlatformSem_xdr.o CrossPlatformSem_svc.o $(ROOT_DIR)/chapter2/section3/XPCSemaphore.o CrossPlatformSem_server.o -lthread -lposix4 -lnsl
