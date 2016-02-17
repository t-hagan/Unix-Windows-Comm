INC_DIR = -I$(ROOT_DIR)/chapter1 -I$(ROOT_DIR)/chapter2/section1 -I$(ROOT_DIR)/chapter2/section2 -I$(ROOT_DIR)/chapter2/section3 -I.

all: MultiConsumer

XPCPthreadAttr.o: XPCPthreadAttr.C
	CC $(INC_DIR) -c XPCPthreadAttr.C -g -o XPCPthreadAttr.o

XPCPthread.o: XPCPthread.C
	CC $(INC_DIR) -c XPCPthread.C -g -o XPCPthread.o

MultiConsumer.o: MultiConsumer.C
	CC $(INC_DIR) -c MultiConsumer.C -g -o MultiConsumer.o

MultiConsumer: XPCPthreadAttr.o XPCPthread.o MultiConsumer.o
	CC -o MultiConsumer ../section2/XPCSignal.o ../section3/XPCSemaphore.o XPCPthreadAttr.o XPCPthread.o MultiConsumer.o -lpthread -lposix4
