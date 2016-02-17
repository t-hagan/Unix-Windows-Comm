const MAX_BANK_NAME = 512;
const MAX_PAYMENTS = 360;

struct ssinglepayment {
    int iPeriod;
    double dPayment;
    double dInterest;
    double dPrincipal;
    double dAmountRemaining;
};

struct sloaninfo {
    double dAmount;
    int iPeriod;
    int iYear;
};

struct spaymentinfo {
    char sBankName<MAX_BANK_NAME>;
    double dPayment;
};

struct sschedual {
    ssinglepayment aPayment;
    sschedual *next;
};

program payments {
    version paymentsver {
        spaymentinfo dGetPayment(sloaninfo)=1;
        sschedual sGetListOfPayments(sloaninfo)=2;
    } = 1;
} = 0x22000000;
