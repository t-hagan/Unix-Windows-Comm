INC_DIR = -I$(ROOT_DIR)/chapter1 -I$(ROOT_DIR)/chapter2/section1 -I.

all: ParseDirectory

XPCFileStat.o: XPCFileStat.C
	CC $(INC_DIR) -c XPCFileStat.C -g -o XPCFileStat.o

ParseDirectory.o: ParseDirectory.C 
	CC $(INC_DIR) -c ParseDirectory.C -g -o ParseDirectory.o

ParseDirectory: XPCFileStat.o ParseDirectory.o
	CC -o ParseDirectory XPCFileStat.o ParseDirectory.o
