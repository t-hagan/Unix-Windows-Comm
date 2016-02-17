#include <XPCFileStat.h>
#include <iostream.h>

XPCFileStat::XPCFileStat()
{
    long lMaxpath;

    // Determine the maximum size of a pathname
    if ((lMaxpath = pathconf("/", _PC_PATH_MAX)) == -1)
    {
        XPCException newExcept("Could not determine maximum pathname length");
        throw newExcept;
        return;
    }

    // Allocate memory for the pathname
    cFileName = new char[lMaxpath + 1];
    if (!cFileName)
    {
        XPCException newExcept("Could not allocate memory for cFileName");
        throw newExcept;
        return;
    }

    // Store the current working directory
    if (getcwd(cFileName, lMaxpath) == NULL)
    {
        XPCException newExcept("Could not get current working directory");
        throw newExcept;
        return;
    }

    // Retrieve the file's statistics
    if (lstat(cFileName, &sStatBuf) == -1)
    {
        XPCException newExcept("Could not obtain statics on directory.");
        throw newExcept;
        return;
    }
}

XPCFileStat::XPCFileStat(char *_psFileName)
{
    // Allocate memory to store the pathname
    cFileName = new char[strlen(_psFileName)+1];
    if (!cFileName)
    {
        XPCException newExcept("Could not allocate memory for cFileName");
        throw newExcept;
        return;
    }

    // Copy the pathname to the private data member
    strcpy(cFileName, _psFileName);

    // Retrieve the file's statistics
    if (lstat(cFileName, &sStatBuf) == -1)
    {
        XPCException newExcept("Could not obtain statics on directory.");
        throw newExcept;
        return;
    }    
}

XPCFileStat::XPCFileStat(const XPCFileStat &_oldClass)
{
    cFileName = new char[sizeof(_oldClass.cFileName)+1];
    if (!cFileName)
    {
        XPCException newExcept("Could not allocate memory for cFileName");
        throw newExcept;
        return;
    }

    strcpy(cFileName, _oldClass.sGetFileName());

    memcpy((void *)&sStatBuf, (void *)&_oldClass.getStatBuf(), sizeof(struct stat));
}

enum eDirectoryTypes XPCFileStat::iGetFileType()
{
    // Extract the file type bits from st_mode and match them up with
    // the file type constants

    switch(sStatBuf.st_mode & S_IFMT)
    {
        case S_IFIFO:
            return FIFO;
            break;
        case S_IFCHR:
            return CHARACTER;
            break;
        case S_IFDIR:
            return DIRECTORY;
            break;
        case S_IFBLK:        
            return BLOCK;
            break;
        case S_IFREG:
            return ORDINARY_FILE;
            break;
        case S_IFLNK:
            return LINK_DIRECTORY;
            break;
        default:
            return UNKNOWN_TYPE;
            break;
    }
}

enum ePermissions XPCFileStat::iGetOwnerPermissions()
{
    // Extract the owner permission bits and return the appropriate
    // permission value

    switch(sStatBuf.st_mode & S_IRWXU)
    {
        case 0700:
            return READ_WRITE_EXECUTE;
            break;
        case 0600:
            return READ_WRITE;
            break;
        case 0500:
            return READ_EXECUTE;
            break;
        case 0400:
            return READ;
            break;
        case 0300:
            return WRITE_EXECUTE;
            break;
        case 0200:
            return WRITE;
            break;
        case 0100:
            return EXECUTE;
            break;
        default:
            return UNKNOWN_PERMISSION;
    }
}

enum ePermissions XPCFileStat::iGetGroupPermissions()
{
    // Extract the group permission bits and return the appropriate
        // permission value

    switch(sStatBuf.st_mode & S_IRWXG)
    {
                case 0070:
                        return READ_WRITE_EXECUTE;
                        break;
                case 0060:
                        return READ_WRITE;
                        break;
                case 0050:
                        return READ_EXECUTE;
                        break;
                case 0040:
                        return READ;
                        break;
                case 0030:
                        return WRITE_EXECUTE;
                        break;
                case 0020:
                        return WRITE;
                        break;
                case 0010:
                        return EXECUTE;
                        break;
                default:
                        return UNKNOWN_PERMISSION;
    }
}

enum ePermissions XPCFileStat::iGetOtherPermissions()
{
    // Extract the "other users" permission bits and return the appropriate
    // permission value

    switch(sStatBuf.st_mode & S_IRWXO)
    {
                case 0007:
                        return READ_WRITE_EXECUTE;
                        break;
                case 0006:
                        return READ_WRITE;
                        break;
                case 0005:
                        return READ_EXECUTE;
                        break;
                case 0004:
                        return READ;
                        break;
                case 0003:
                        return WRITE_EXECUTE;
                        break;
                case 0002:
                        return WRITE;
                        break;
                case 0001:
                        return EXECUTE;
                        break;
                default:
                        return UNKNOWN_PERMISSION;
    }
}

XPCFileStat &XPCFileStat::operator=(const XPCFileStat &_oldClass)
{
    if (this == &_oldClass)
        return *this;

    if (sizeof(cFileName) < sizeof(_oldClass.sGetFileName()))
    {
        delete [] cFileName;
        cFileName = new char[sizeof(cFileName)];
        if (!cFileName)
        {
                    XPCException newExcept("Could not allocate memory for cFileName");
                    throw newExcept;
                    return *this;
        }    
    }
    
    memcpy((void *)cFileName, (void *)_oldClass.sGetFileName(), sizeof(_oldClass.sGetFileName()));
    memcpy((void *)&sStatBuf, (void *)&_oldClass.getStatBuf(), sizeof(struct stat));
}

char XPCFileStat::cCheckReadAccess()
{
    // Check for ownership or effective ownership
    if ((sStatBuf.st_uid == getuid()) || (sStatBuf.st_uid == geteuid()))
    {

        // We are the owner.  Check the owner's permissions for read access.
        // If read access is available TRUE is returned FALSE otherwise
        enum ePermissions eOwnerPermissions = iGetOwnerPermissions();
        if ((eOwnerPermissions == READ) || (eOwnerPermissions == READ_EXECUTE) ||
            (eOwnerPermissions == READ_WRITE) || (eOwnerPermissions == READ_WRITE_EXECUTE))
            return 1;
        else
            return 0;
    }

    // Check for group ownership or effective group ownership
    else if ((sStatBuf.st_gid == getgid()) || (sStatBuf.st_gid == getegid()))
    {

        // Group is the owner.  Check the group's permissions for read access.
        // If read access is available TRUE is returned FALSE otherwise
        enum ePermissions eGroupPermissions = iGetGroupPermissions();
        if ((eGroupPermissions == READ) || (eGroupPermissions == READ_EXECUTE) ||
            (eGroupPermissions == READ_WRITE) || (eGroupPermissions == READ_WRITE_EXECUTE))
            return 1;
        else 
            return 0;
    }

    // Check for all other user's owner ship
    else
    {
        // Other users are the owner.  Check the other user's permissions for read access
        // If read access is available TRUE is returned FALSE otherwise
        enum ePermissions eOtherPermissions = iGetOtherPermissions();
        if ((eOtherPermissions == READ) || (eOtherPermissions == READ_EXECUTE) ||
            (eOtherPermissions == READ_WRITE) || (eOtherPermissions == READ_WRITE_EXECUTE))
            return 1;
        else
            return 0;
    }
}

char XPCFileStat::cCheckWriteAccess()
{
    // Check for ownership or effective ownership
    if ((sStatBuf.st_uid == getuid()) || (sStatBuf.st_uid == geteuid()))
    {
        // We are the owner.  Check the owner's permissions for write access.
        // If write access is available TRUE is returned FALSE otherwise    
        enum ePermissions eOwnerPermissions = iGetOwnerPermissions();
        if ((eOwnerPermissions == WRITE) || (eOwnerPermissions == WRITE_EXECUTE) ||
            (eOwnerPermissions == READ_WRITE) || (eOwnerPermissions == READ_WRITE_EXECUTE))
            return 1;
        else
            return 0;
    }

    // Check for group ownership or effective group ownership    
    else if ((sStatBuf.st_gid == getgid()) || (sStatBuf.st_gid == getegid()))
    {
        // Group is the owner.  Check the group's permissions for write access.
     	// If write access is available TRUE is returned FALSE otherwise    
        enum ePermissions eGroupPermissions = iGetGroupPermissions();
        if ((eGroupPermissions == WRITE) || (eGroupPermissions == WRITE_EXECUTE) ||
            (eGroupPermissions == READ_WRITE) || (eGroupPermissions == READ_WRITE_EXECUTE))
            return 1;
        else    
            return 0;
    }

    // Check for all other user's owner ship    
    else
    {
        // Other users are the owner.  Check the other user's permissions for write access
        // If write access is available TRUE is returned FALSE otherwise
        enum ePermissions eOtherPermissions = iGetOtherPermissions();
        if ((eOtherPermissions == WRITE) || (eOtherPermissions == WRITE_EXECUTE) ||
            (eOtherPermissions == READ_WRITE) || (eOtherPermissions == READ_WRITE_EXECUTE))
            return 1;
        else
            return 0;
    }
}

char XPCFileStat::cCheckExecuteAccess()
{
    // Check for ownership or effective ownership    
    if ((sStatBuf.st_uid == getuid()) || (sStatBuf.st_uid == geteuid()))
    {
        // We are the owner.  Check the owner's permissions for execute access.
        // If execute access is available TRUE is returned FALSE otherwise    
        enum ePermissions eOwnerPermissions = iGetOwnerPermissions();
        if ((eOwnerPermissions == WRITE_EXECUTE) || (eOwnerPermissions == READ_EXECUTE) ||
            (eOwnerPermissions == READ_WRITE_EXECUTE))
            return 1;
        else
            return 0;
    } 

    // Check for group ownership or effective group ownership    
    else if ((sStatBuf.st_gid == getgid()) || (sStatBuf.st_gid == getegid()))
    {
        // Group is the owner.  Check the group's permissions for execute access.
        // If execute access is available TRUE is returned FALSE otherwise    
        enum ePermissions eGroupPermissions = iGetGroupPermissions();
        if ((eGroupPermissions == WRITE_EXECUTE) || (eGroupPermissions == READ_EXECUTE) ||
            (eGroupPermissions == READ_WRITE_EXECUTE))
            return 1;
        else    
            return 0;
    }

    // Check for all other user's owner ship
    else
    {
        // Other users are the owner.  Check the other user's permissions for execute access
        // If execute access is available TRUE is returned FALSE otherwise
        enum ePermissions eOtherPermissions = iGetOtherPermissions();
        if ((eOtherPermissions == WRITE_EXECUTE) || (eOtherPermissions == READ_EXECUTE) ||
            (eOtherPermissions == READ_WRITE_EXECUTE))
            return 1;
        else
            return 0;
    }
}
