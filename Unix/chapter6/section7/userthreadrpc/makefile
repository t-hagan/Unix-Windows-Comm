INC_DIR = -I../../section5/LoanFinderVarServer -I.

all: LoanFinderVarServer

loan_system3_svc.o: loan_system3_svc.c
	gcc $(INC_DIR) -c loan_system3_svc.c -o loan_system3_svc.o

LoanFinderVarServer: loan_system3_svc.o
	CC -o LoanFinderVarServer ../../section5/LoanFinderVarServer/loan_system3_xdr.o ../../section5/LoanFinderVarServer/loan_system3_svc.o ../../section5/LoanFinderVarServer/XPCLoanCalc.o ../../section5/LoanFinderVarServer/LoanFinderVarServer.o -lthread -lnsl
