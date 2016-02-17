typedef char sharedmem<>;

struct createsharedmem
{
    int ikey;
    int isize;
};

struct writesharedmem
{
    int ikey;
    sharedmem data;
};

struct sharedmemretval
{
        int iretval;
        string serrormsg<>;
};
    
program crossplatformmem {
    version crossplatformmemver {
        sharedmemretval createmem(createsharedmem)=1;
        sharedmemretval write(writesharedmem)=2;
        sharedmem read(createsharedmem)=3;
        sharedmemretval removemem(int)=4;    
    } = 1;
} = 0x22200000;
