const MAX_BANK_NAME = 512;
const MAX_PAYMENTS = 360;

struct ssinglepayment {
    int iPeriod;
    double dPayment;
    double dInterest;
    double dPrincipal;
    double dAmountRemaining;
};

struct spaymentinfo {
    char sBankName<MAX_BANK_NAME>;
    double dPayment;
};

struct sschedual {
    ssinglepayment listofpayments[MAX_PAYMENTS];
};

program payments {
    version paymentsver {
        spaymentinfo dGetPayment(double)=1;
        sschedual sGetListOfPayments(double)=2;
    } = 1;
} = 0x22000000;
