INC_DIR = -I. -I$(ROOT_DIR)/chapter1 -I../section2

all:SharedMemServer

SharedMemServer_xdr.o: SharedMemServer_xdr.c
	gcc -c SharedMemServer_xdr.c -o SharedMemServer_xdr.o

SharedMemServer_svc.o: SharedMemServer_svc.c
	gcc -c SharedMemServer_svc.c -o SharedMemServer_svc.o

SharedMemServer.o: SharedMemServer.C
	CC $(INC_DIR) -c SharedMemServer.C -o SharedMemServer.o

SharedMemServer: SharedMemServer_xdr.o SharedMemServer_svc.o SharedMemServer.o
	CC -o SharedMemServer SharedMemServer_xdr.o SharedMemServer_svc.o ../section2/XPCSharedMem.o SharedMemServer.o -lthread -lposix4 -lnsl
