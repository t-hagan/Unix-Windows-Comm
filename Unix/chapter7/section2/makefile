all: ProcessControlServer

ProcessControlServer.o: ProcessControlServer.C
	CC -c ProcessControlServer.C -g -o ProcessControlServer.o

ProcessControlServer: ProcessControlServer.o
	CC -o ProcessControlServer ProcessControlServer.o
