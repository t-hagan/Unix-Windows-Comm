typedef string semname<>;

struct semstruct
{
	semname ssemname;
	int iinitvalue;
	int imaxvalue;
};

struct retvalstruct
{
	int iretval;
	string serrormsg<>;
};

program crossplatformsem {
	version crossplatformsemver {
		retvalstruct createsem(semstruct)=1;
		retvalstruct closesem(semname)=2;
		retvalstruct removesem(semname)=3;
		retvalstruct lockwait(semname)=4;
		retvalstruct locknowait(semname)=5;
		retvalstruct unlock(semname)=6;
		retvalstruct getvalue(semname)=7;
	} = 1;
} = 0x22100000;
