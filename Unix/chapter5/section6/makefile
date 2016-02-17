INC_DIR = -I$(ROOT_DIR)/chapter1 -I../section1 -I../section3 -I../section4 -I../section5 -I.

all: UDPstockServer

XPCSocket.o: XPCSocket.C
	CC $(INC_DIR) -DUNIX -c XPCSocket.C -g -o XPCSocket.o

XPCUdpSocket.o: XPCUdpSocket.C
	CC $(INC_DIR) -DUNIX -c XPCUdpSocket.C -g -o XPCUdpSocket.o

UDPstockServer.o: UDPstockServer.C 
	CC $(INC_DIR) -DUNIX -c UDPstockServer.C -g -o UDPstockServer.o

UDPstockServer: XPCSocket.o XPCUdpSocket.o UDPstockServer.o
	CC -o UDPstockServer ../section3/XPCGetProtocol.o ../section4/XPCGetHostInfo.o ../section5/XPCGetService.o XPCSocket.o XPCUdpSocket.o UDPstockServer.o -lsocket -lnsl
