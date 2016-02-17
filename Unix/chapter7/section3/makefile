all: ProcessLookupServer

ProcessLookupServer.o: ProcessLookupServer.C
	CC -c ProcessLookupServer.C -g -o ProcessLookupServer.o

ProcessLookupServer: ProcessLookupServer.o
	CC -o ProcessLookupServer ProcessLookupServer.o
