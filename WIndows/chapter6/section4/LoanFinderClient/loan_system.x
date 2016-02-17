const MAX_BANK_NAME = 512;

struct spaymentinfo {
	char sBankName<MAX_BANK_NAME>;
	double dPayment;
};

program payments {
	version paymentsver {
		spaymentinfo dGetPayment(double)=1;
	} = 1;
} = 0x21000000;
