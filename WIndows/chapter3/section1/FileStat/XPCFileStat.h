// XPCFileStat.h: interface for the CFileStat class.
//
//////////////////////////////////////////////////////////////////////

#if !defined (_XPCFILESTAT_H_)
#define _XPCFILESTAT_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <windows.h>
#include <aclapi.h>
#include <XPCException.h>


// Win 32 attribute constants redefined for simplicity
#define FILE_TYPES    DWORD
#define ARCHIVE        FILE_ATTRIBUTE_ARCHIVE
#define COMPRESSED    FILE_ATTRIBUTE_COMPRESSED
#define DIRECTORY    FILE_ATTRIBUTE_DIRECTORY
#define HIDDEN        FILE_ATTRIBUTE_HIDDEN
#define NORMAL        FILE_ATTRIBUTE_NORMAL
#define OFFLINE        FILE_ATTRIBUTE_OFFLINE
#define READONLY    FILE_ATTRIBUTE_READONLY
#define SYSTEM        FILE_ATTRIBUTE_SYSTEM
#define TEMPORARY    FILE_ATTRIBUTE_TEMPORARY

class XPCFileStat  
{
private:
    char * sFileName;    // stores the filename or directory

    // file attributes
    WIN32_FILE_ATTRIBUTE_DATA fileAttr;

    // security descriptor
    PSECURITY_DESCRIPTOR pSD;
    // owner and group trustees
    PTRUSTEE pOwner, pGroup;
    // keeps the number of entries in the access lists (ACLs) below
    ULONG lNumAccessEntries, pNumAuditEntries;
    PEXPLICIT_ACCESS pListOfAccessEntries, pListOfAuditEntries;
    // handle to the current process
    HANDLE hProcess;
    // handle to the current process token
    HANDLE hProcessToken;
    // group info of the current process/
    PTOKEN_GROUPS pGroupInfo;
    // user info of the current process/
    PTOKEN_USER pUserInfo;

    // worker functions...
    // get the error string for the last error
    char *sErrorString (char *_sErr);
    // get all status information for sFname
    void vGetStat(char *_sFName);
    // check if User or Group has access to our file
    int iCheckCurrentUserAndGroups(TRUSTEE _tGroupOrUser);
public:
    // default constructor
    XPCFileStat();
    // constructor used for a specific path name
    XPCFileStat (char *);
    // coppy constructor
    XPCFileStat (const XPCFileStat &);
    //destructor
    ~XPCFileStat();
    // return the low order file size
    int iGetSize() {return (fileAttr.nFileSizeLow);}
    // return the file attributes
    FILE_TYPES dwGetFileType() {return (fileAttr.dwFileAttributes);}
    // return the path name
    char *sGetFileName() {return (sFileName);}
    // creation time
    FILETIME ftGetCreationTime() {return (fileAttr.ftCreationTime);}
    // last access time
    FILETIME ftGetLastAccessTime() {return (fileAttr.ftLastAccessTime);}
    // last write time
    FILETIME ftGetLastModTime() {return (fileAttr.ftLastWriteTime);}
    // check if process has execute privileges
    int iCheckExecuteAccess();
    // check if process has write privileges
    int iCheckWriteAccess();
    // check if process has read access
    int iCheckReadAccess();
};

#endif // !defined(_XPCFILESTAT_H_)
