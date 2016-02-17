INC_DIR = -I$(ROOT_DIR)/chapter1 -I$(ROOT_DIR)/chapter9/section2/FileTransferClient -I$(ROOT_DIR)/chapter2/section1 -I$(ROOT_DIR)/chapter2/section4 -I$(ROOT_DIR)/chapter5/section1 -I$(ROOT_DIR)/chapter5/section3 -I$(ROOT_DIR)/chapter5/section4 -I$(ROOT_DIR)/chapter5/section5 -I$(ROOT_DIR)/chapter5/section6 -I$(ROOT_DIR)/chapter5/section7 -I.

all: FileTransferServer

FileTransferServer.o: FileTransferServer.C
	CC $(INC_DIR) -DUNIX -c FileTransferServer.C -g -o FileTransferServer.o

FileTransferServer: FileTransferServer.o
	CC -o FileTransferServer $(ROOT_DIR)/chapter9/section2/FileTransferClient/XPCFileTransfer.o $(ROOT_DIR)/chapter2/section1/XPCFileStat.o $(ROOT_DIR)/chapter2/section4/XPCPthread.o $(ROOT_DIR)/chapter5/section3/XPCGetProtocol.o $(ROOT_DIR)/chapter5/section4/XPCGetHostInfo.o $(ROOT_DIR)/chapter5/section5/XPCGetService.o $(ROOT_DIR)/chapter5/section6/XPCSocket.o $(ROOT_DIR)/chapter5/section7/XPCTcpSocket.o FileTransferServer.o -lpthread -lsocket -lnsl -Bdynamic -lc
