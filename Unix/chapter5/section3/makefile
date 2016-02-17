INC_DIR = -I$(ROOT_DIR)/chapter1 -I.

all: getProtocolTest

XPCGetProtocol.o: XPCGetProtocol.C
	CC $(INC_DIR) -DUNIX -c XPCGetProtocol.C -g -o XPCGetProtocol.o

getProtocolTest.o: getProtocolTest.C 
	CC $(INC_DIR) -DUNIX -c getProtocolTest.C -g -o getProtocolTest.o

getProtocolTest: XPCGetProtocol.o getProtocolTest.o
	CC -o getProtocolTest XPCGetProtocol.o getProtocolTest.o -lsocket -lnsl
