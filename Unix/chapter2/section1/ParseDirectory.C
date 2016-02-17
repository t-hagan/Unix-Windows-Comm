#include <iostream.h>
#include <dirent.h>
#include <string.h>
#include <stdio.h>
#include <XPCFileStat.h>

// Directory type string constants
char *sDirTypes[] = {"FIFO", "CHARACTER", "DIRECTORY", "LINK_DIRECTORY", "BLOCK", "FILE", "UNKNOWN_TYPE"};

// Permission string constants
char *sPermissions[] = {"READ", "WRITE", "EXECUTE", "READ_EXECUTE", "WRITE_EXECUTE", "READ_WRITE",
                        "READ_WRITE_EXECUTE", "UNKNOWN_PERMISSION"};

// True / False constants
char *sYN[] = {"NO", "YES"};

void vTraverseDirectory(char *_sDir)
{
    // Retrieve information about all files and subdirectories given a directory
    DIR *pDirp;
    struct dirent *pDp;
    long lTime;
    char *sFullDir;

    // Open the given directory
    if ((pDirp = opendir(_sDir)) == NULL)
    {
         cerr << "Error: Cannot open " << _sDir << endl;
         return;
    }

    // loop through all directory entries
    while ((pDp = readdir(pDirp)) != NULL)
    {
        if ((!strcmp(pDp->d_name, ".")) || (!strcmp(pDp->d_name, "..")))
            continue;

        try
        {
            // Create the full pathname
            sFullDir = new char[strlen(_sDir) + strlen(pDp->d_name) + 1];    
            sprintf(sFullDir, "%s/%s\0", _sDir, pDp->d_name);
                
            // Get the pathname statistics    
            XPCFileStat fileObject(sFullDir);

            // Display the file's statistics
            cout << "File Name        : " << fileObject.sGetFileName() << endl;
            cout << "File Type        : " << sDirTypes[fileObject.iGetFileType()] << endl;
            cout << "Owner Permissions: " << sPermissions[fileObject.iGetOwnerPermissions()] << endl;
            cout << "Group Permissions: " << sPermissions[fileObject.iGetGroupPermissions()] << endl;
            cout << "Other Permissions: " << sPermissions[fileObject.iGetOtherPermissions()] << endl;
            cout << "File Size        : " << fileObject.iGetSize() << endl;
            cout << "IO Block Size    : " << fileObject.lGetIOBlkSize() << endl;

            lTime = fileObject.lGetLastAccessTime();
            cout << "Last Access Time : " << ctime(&lTime);

            lTime = fileObject.lGetLastModTime();
            cout << "Last Mod Time    : " << ctime(&lTime);

            lTime = fileObject.lGetStatusChange();
            cout << "Last Status Time : " << ctime(&lTime);

            cout << "Read Access      : " << sYN[fileObject.cCheckReadAccess()] << endl;
            cout << "Write Access     : " << sYN[fileObject.cCheckWriteAccess()] << endl;
            cout << "Execute Access   : " << sYN[fileObject.cCheckExecuteAccess()] << endl;
            cout << endl;

            // If the file is a directory or a link, the directory or linked is traversed
            if ((fileObject.iGetFileType() == DIRECTORY) || (fileObject.iGetFileType() == LINK_DIRECTORY))
            {
                sprintf(sFullDir, "%s/%s\0", _sDir, pDp->d_name);

                if (!fileObject.cCheckExecuteAccess())
                    cout << "** Cannot Open Directory: " << sFullDir << endl;
                else
                    vTraverseDirectory(sFullDir);
            }
        }
        catch(XPCException &exceptObject)
        {
             cout << exceptObject.sGetException() << endl;
             return;
        }
    }

    closedir(pDirp);
}

main(int argc, char *argv[])
{
    if (argc != 2)
    {
        cerr << "Usage: ParseDirectory < Directory pathname > " << endl;
        return 0;
    }
    
    vTraverseDirectory(argv[1]);
    return 1;
}
