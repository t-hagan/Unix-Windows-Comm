INC_DIR = -I$(ROOT_DIR)/chapter1 -I$(ROOT_DIR)/chapter2/section3 -I$(ROOT_DIR)/chapter2/section4 -I$(ROOT_DIR)/chapter5/section1 -I$(ROOT_DIR)/chapter5/section3 -I$(ROOT_DIR)/chapter5/section4 -I$(ROOT_DIR)/chapter5/section5 -I$(ROOT_DIR)/chapter5/section6 -I$(ROOT_DIR)/chapter5/section7 -I.

all: PubSubServer

PubSubServer.o: PubSubServer.C
	CC $(INC_DIR) -DUNIX -c PubSubServer.C -g -o PubSubServer.o

PubSubServer: PubSubServer.o
	CC -o PubSubServer $(ROOT_DIR)/chapter2/section3/XPCSemaphore.o $(ROOT_DIR)/chapter2/section4/XPCPthread.o $(ROOT_DIR)/chapter5/section3/XPCGetProtocol.o $(ROOT_DIR)/chapter5/section4/XPCGetHostInfo.o $(ROOT_DIR)/chapter5/section5/XPCGetService.o $(ROOT_DIR)/chapter5/section6/XPCSocket.o $(ROOT_DIR)/chapter5/section7/XPCTcpSocket.o PubSubServer.o -lpthread -lposix4 -lsocket -lnsl -Bdynamic -lc
