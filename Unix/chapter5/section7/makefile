INC_DIR = -I$(ROOT_DIR)/chapter1 -I../section1 -I../section3 -I../section4 -I../section5 -I../section6 -I.

all: TCPstockServer

XPCTcpSocket.o: XPCTcpSocket.C
	CC $(INC_DIR) -DUNIX -c XPCTcpSocket.C -g -o XPCTcpSocket.o

TCPstockServer.o: TCPstockServer.C 
	CC $(INC_DIR) -DUNIX -c TCPstockServer.C -g -o TCPstockServer.o

TCPstockServer: XPCTcpSocket.o TCPstockServer.o
	CC -o TCPstockServer ../section3/XPCGetProtocol.o ../section4/XPCGetHostInfo.o ../section5/XPCGetService.o ../section6/XPCSocket.o XPCTcpSocket.o TCPstockServer.o -lsocket -lnsl
