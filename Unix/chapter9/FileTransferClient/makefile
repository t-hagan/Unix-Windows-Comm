INC_DIR = -I$(ROOT_DIR)/chapter1 -I$(ROOT_DIR)/chapter2/section1 -I$(ROOT_DIR)/chapter2/section4 -I$(ROOT_DIR)/chapter5/section1 -I$(ROOT_DIR)/chapter5/section3 -I$(ROOT_DIR)/chapter5/section4 -I$(ROOT_DIR)/chapter5/section5 -I$(ROOT_DIR)/chapter5/section6 -I$(ROOT_DIR)/chapter5/section7 -I.

all: FileTransferClient

FileTransferClient.o: FileTransferClient.C
	CC $(INC_DIR) -DUNIX -c FileTransferClient.C -g -o FileTransferClient.o

XPCFileTransfer.o: XPCFileTransfer.C
	CC $(INC_DIR) -DUNIX -c XPCFileTransfer.C -g -o XPCFileTransfer.o

FileTransferClient: FileTransferClient.o XPCFileTransfer.o
	CC -o FileTransferClient $(ROOT_DIR)/chapter2/section1/XPCFileStat.o $(ROOT_DIR)/chapter2/section4/XPCPthread.o $(ROOT_DIR)/chapter5/section3/XPCGetProtocol.o $(ROOT_DIR)/chapter5/section4/XPCGetHostInfo.o $(ROOT_DIR)/chapter5/section5/XPCGetService.o $(ROOT_DIR)/chapter5/section6/XPCSocket.o $(ROOT_DIR)/chapter5/section7/XPCTcpSocket.o FileTransferClient.o XPCFileTransfer.o -lpthread -lsocket -lnsl -Bdynamic -lc
