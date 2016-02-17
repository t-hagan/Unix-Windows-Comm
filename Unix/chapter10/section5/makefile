INC_DIR = -I. -I$(ROOT_DIR)/chapter1 -I$(ROOT_DIR)/chapter5/section1 -I$(ROOT_DIR)/chapter8/client

all:InventoryMgr

SharedMemServer_clnt.o: SharedMemServer_clnt.c
	gcc -c SharedMemServer_clnt.c -o SharedMemServer_clnt.o

InventoryMgr.o: InventoryMgr.C
	CC $(INC_DIR) -c InventoryMgr.C -o InventoryMgr.o

InventoryMgr: InventoryMgr.o SharedMemServer_clnt.o
	CC -o InventoryMgr ../section4/SharedMemServer_xdr.o SharedMemServer_clnt.o $(ROOT_DIR)/chapter8/client/CrossPlatformSem_xdr.o $(ROOT_DIR)/chapter8/client/CrossPlatformSem_clnt.o InventoryMgr.o -lnsl
