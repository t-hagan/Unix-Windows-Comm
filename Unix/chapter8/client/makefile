INC_DIR = -I$(ROOT_DIR)/chapter1 -I$(ROOT_DIR)/chapter5/section1 -I$(ROOT_DIR)/chapter5/section3 -I$(ROOT_DIR)/chapter5/section4 -I$(ROOT_DIR)/chapter5/section5 -I$(ROOT_DIR)/chapter5/section6 -I$(ROOT_DIR)/chapter5/section7 -I$(ROOT_DIR)/chapter2/section4 -I.

all: PriceServer

CrossPlatformSem_xdr.o: CrossPlatformSem_xdr.c
	gcc -c CrossPlatformSem_xdr.c -g -o CrossPlatformSem_xdr.o

CrossPlatformSem_clnt.o: CrossPlatformSem_clnt.c
	gcc -c CrossPlatformSem_clnt.c -g -o CrossPlatformSem_clnt.o

PriceServer.o: PriceServer.C
	CC $(INC_DIR) -DUNIX -c PriceServer.C -g -o PriceServer.o

PriceServer: CrossPlatformSem_xdr.o CrossPlatformSem_clnt.o PriceServer.o
	CC -o PriceServer CrossPlatformSem_xdr.o CrossPlatformSem_clnt.o $(ROOT_DIR)/chapter2/section4/XPCPthread.o $(ROOT_DIR)/chapter5/section6/XPCSocket.o $(ROOT_DIR)/chapter5/section7/XPCTcpSocket.o $(ROOT_DIR)/chapter5/section4/XPCGetHostInfo.o $(ROOT_DIR)/chapter5/section5/XPCGetService.o $(ROOT_DIR)/chapter5/section3/XPCGetProtocol.o PriceServer.o -lpthread -lsocket -lnsl -Bdynamic -lc
