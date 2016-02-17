INC_DIR = -I$(ROOT_DIR)/chapter1 -I../section3 -I.

all: getServiceTest

XPCGetService.o: XPCGetService.C
	CC $(INC_DIR) -DUNIX -c XPCGetService.C -g -o XPCGetService.o

getServiceTest.o: getServiceTest.C 
	CC $(INC_DIR) -DUNIX -c getServiceTest.C -g -o getServiceTest.o

getServiceTest: XPCGetService.o getServiceTest.o
	CC -o getServiceTest ../section3/XPCGetProtocol.o XPCGetService.o getServiceTest.o -lsocket -lnsl
