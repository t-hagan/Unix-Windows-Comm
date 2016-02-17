#ifndef _XPCFileStat
#define _XPCFileStat

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>

// The inter-platform exception class defined in Chapter 1
#include <XPCException.h>

// The UNIX file directory types
enum eDirectoryTypes {FIFO, CHARACTER, DIRECTORY, LINK_DIRECTORY, BLOCK, ORDINARY_FILE, UNKNOWN_TYPE};

// The access permissions to a pathname
enum ePermissions {READ, WRITE, EXECUTE, READ_EXECUTE, WRITE_EXECUTE, READ_WRITE,
           READ_WRITE_EXECUTE, UNKNOWN_PERMISSION};

class XPCFileStat
{
    char *cFileName;    // Private data member pointing to the specified pathname
    struct stat sStatBuf;    // Private data member containing an instance of stat
public:
    // The default constructor.  Stores the current working directory in cFileName
    XPCFileStat();

    // Constructor which takes the specified pathname and stores it in XPCFileName
    XPCFileStat(char *);

    // Copy constructor
    XPCFileStat(const XPCFileStat &);

    // Destructor
    ~XPCFileStat() { delete [] cFileName; }

    // Returns the size of data stored at cFileName
    int iGetSize() { return sStatBuf.st_size; }

    // Returns the name of the pathname stored in cFileName    
    const char *sGetFileName() const { return cFileName; }

    // Returns the file statistics buffer
    const struct stat getStatBuf() const { return sStatBuf; }

    // Returns the file type of cFileName
    enum eDirectoryTypes iGetFileType();

    // Returns the access rights the pathname owner has to cFileName    
    enum ePermissions iGetOwnerPermissions();

    // Returns the access rights with which a member in the owner's group has access
    // to cFileName
    enum ePermissions iGetGroupPermissions();

    // Returns the access rights all users, other than the owner and group members
        // have to cFileName    
    enum ePermissions iGetOtherPermissions();

    // The last time cFileName was accessed.  The time is returned in seconds
    long lGetLastAccessTime() { return sStatBuf.st_atime; }

    // The last time cFileName was modified.  The time is returned in seconds
    long lGetLastModTime() { return sStatBuf.st_mtime; }

    // The last time cFileName was accessed, modified, or had its status (permissions)
        // changed.
    long lGetStatusChange() { return sStatBuf.st_ctime; }

    // Retrieves the size of an IO block
    long lGetIOBlkSize() { return sStatBuf.st_blksize; }

    // Indicates whether or not the application has read access to cFileName
    char cCheckReadAccess();

    // Indicates whehter or not the application has write access to cFileName
    char cCheckWriteAccess();

    // Indicates whether or not the applicaiton has execute access to cFileName
    char cCheckExecuteAccess();

    // Overloaded equal operator to insure cFileName is correctly copied
    XPCFileStat &operator=(const XPCFileStat &);
};

#endif
