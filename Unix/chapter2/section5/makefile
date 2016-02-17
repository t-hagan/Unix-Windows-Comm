INC_DIR = -I$(ROOT_DIR)/chapter1 -I$(ROOT_DIR)/chapter2/section1 -I$(ROOT_DIR)/chapter2/section2 -I$(ROOT_DIR)/chapter2/section3 -I$(ROOT_DIR)/chapter2/section4 -I.

all: MultiConsumerSpawn

XPCWaitPid.o: XPCWaitPid.C
	CC $(INC_DIR) -c XPCWaitPid.C -g -o XPCWaitPid.o

MultiConsumerSpawn.o: MultiConsumerSpawn.C
	CC $(INC_DIR) -c MultiConsumerSpawn.C -g -o MultiConsumerSpawn.o

MultiConsumerSpawn: XPCWaitPid.o MultiConsumerSpawn.o
	CC -o MultiConsumerSpawn ../section2/XPCSignal.o ../section3/XPCSemaphore.o ../section4/XPCPthreadAttr.o ../section4/XPCPthread.o XPCWaitPid.o MultiConsumerSpawn.o -lpthread -lposix4
