INC_DIR = -I$(ROOT_DIR)/chapter1 -I.

all: getHostTest

XPCGetHostInfo.o: XPCGetHostInfo.C
	CC $(INC_DIR) -DUNIX -c XPCGetHostInfo.C -g -o XPCGetHostInfo.o

getHostTest.o: getHostTest.C 
	CC $(INC_DIR) -DUNIX -c getHostTest.C -g -o getHostTest.o

getHostTest: XPCGetHostInfo.o getHostTest.o
	CC -o getHostTest XPCGetHostInfo.o getHostTest.o -lsocket -lnsl
