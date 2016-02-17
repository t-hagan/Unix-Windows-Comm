INC_DIR = -I.

all: LoanFinderServer

loan_system_xdr.o: loan_system_xdr.c
	gcc -c loan_system_xdr.c -o loan_system_xdr.o

loan_system_svc.o: loan_system_svc.c
	gcc -c loan_system_svc.c -o loan_system_svc.o

XPCLoanCalc.o: XPCLoanCalc.C
	CC -I. -c XPCLoanCalc.C -o XPCLoanCalc.o

LoanFinderServer.o: LoanFinderServer.C
	CC -I. -c LoanFinderServer.C -o LoanFinderServer.o

LoanFinderServer: loan_system_xdr.o loan_system_svc.o XPCLoanCalc.o LoanFinderServer.o
	CC -o LoanFinderServer loan_system_xdr.o loan_system_svc.o XPCLoanCalc.o LoanFinderServer.o -lnsl
