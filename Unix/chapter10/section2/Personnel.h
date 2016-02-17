#ifndef _Personnel
#define _Personnel

// A single record containing personnel information
struct SPersonnelStruct
{
        char sName[50];	// Name	
        char sSSNO[12]; // Social security numberj
        int iAge;	// Age
};

// A list of up to 10 personnel records
struct SPersonnelList
{
        SPersonnelStruct Message[10];	// Record list
        int iMessageNumber;		// Number of records
};

#endif
