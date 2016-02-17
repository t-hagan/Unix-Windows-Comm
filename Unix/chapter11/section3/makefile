INC_DIR = -I$(ROOT_DIR)/chapter1 -I$(ROOT_DIR)/chapter2/section3 -I$(ROOT_DIR)/chapter2/section4 -I$(ROOT_DIR)/chapter5/section1 -I$(ROOT_DIR)/chapter5/section3 -I$(ROOT_DIR)/chapter5/section4 -I$(ROOT_DIR)/chapter5/section5 -I$(ROOT_DIR)/chapter5/section6 -I$(ROOT_DIR)/chapter5/section7 -I.

all: SpawnServer

SpawnServer.o: SpawnServer.C
	CC $(INC_DIR) -DUNIX -c SpawnServer.C -g -o SpawnServer.o

SpawnServer: SpawnServer.o
	CC -o SpawnServer $(ROOT_DIR)/chapter2/section3/XPCSemaphore.o $(ROOT_DIR)/chapter2/section4/XPCPthread.o $(ROOT_DIR)/chapter5/section3/XPCGetProtocol.o $(ROOT_DIR)/chapter5/section4/XPCGetHostInfo.o $(ROOT_DIR)/chapter5/section5/XPCGetService.o $(ROOT_DIR)/chapter5/section6/XPCSocket.o $(ROOT_DIR)/chapter5/section7/XPCTcpSocket.o SpawnServer.o -lpthread -lposix4 -lsocket -lnsl -Bdynamic -lc
