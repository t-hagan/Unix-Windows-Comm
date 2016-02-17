#include <iostream.h>
#include "XPCFileStat.h"


// Append the name of the attribute to the output string
// if the attribute type is set.
void ListFileAttr (char * outStr, DWORD dwAttr)
{
    strcpy (outStr, "");

    if (dwAttr & FILE_ATTRIBUTE_ARCHIVE)
        strcat (outStr, "archive, ");
    if (dwAttr & FILE_ATTRIBUTE_COMPRESSED)
        strcat (outStr, "compressed, ");
    if (dwAttr & FILE_ATTRIBUTE_DIRECTORY)
        strcat (outStr, "directory, ");
    if (dwAttr & FILE_ATTRIBUTE_HIDDEN)
        strcat (outStr, "hidden, ");
    if (dwAttr & FILE_ATTRIBUTE_NORMAL)
        strcat (outStr, "normal, ");
    if (dwAttr & FILE_ATTRIBUTE_OFFLINE)
        strcat (outStr, "offline, ");
    if (dwAttr & FILE_ATTRIBUTE_READONLY)
        strcat (outStr, "read-only, ");
    if (dwAttr & FILE_ATTRIBUTE_SYSTEM)
        strcat (outStr, "system, ");
    if (dwAttr & FILE_ATTRIBUTE_TEMPORARY)
        strcat (outStr, "temporary, ");

    if (*outStr) // eliminate the trailing ","
        (*strrchr(outStr, ','))= '\0';
}

// output all relevant infomation about the file from
// the XPCFileStat class
void PrintStat (XPCFileStat *stat)
{
    char buf1[255], buf2[255];
    FILETIME tmFile, tmLocal;
    SYSTEMTIME tm;

    // name of the file...
    wsprintf (buf1, "File: %s", stat->sGetFileName());

    cout << buf1 << endl;

    // list its attributes
    ListFileAttr (buf2, stat->dwGetFileType());

    wsprintf (buf1,"Attributes: %s", buf2);

    cout << buf1 << endl;

    // file creation time
    tmFile= stat->ftGetCreationTime();

    // convert from from file time to local time
    FileTimeToLocalFileTime (&tmFile, &tmLocal);
    // and them to system time to get a readable format
    FileTimeToSystemTime (&tmLocal, &tm);

    wsprintf (buf1, "Created: %02d/%02d/%d %02d:%02d:%02d\n",
        tm.wMonth, tm.wDay, tm.wYear,
        tm.wHour, tm.wMinute, tm.wSecond);

    cout << buf1 << endl;

    // do the same for last access time...
    tmFile= stat->ftGetLastAccessTime();

    FileTimeToLocalFileTime (&tmFile, &tmLocal);
    FileTimeToSystemTime (&tmLocal, &tm);

    wsprintf (buf1, "Last Accessed: %02d/%02d/%d %02d:%02d:%02d\n",
        tm.wMonth, tm.wDay, tm.wYear,
        tm.wHour, tm.wMinute, tm.wSecond);

    cout << buf1 << endl;

    // and last modified time...
    tmFile= stat->ftGetLastModTime();

    FileTimeToLocalFileTime (&tmFile, &tmLocal);
    FileTimeToSystemTime (&tmLocal, &tm);

    wsprintf (buf1, "Last Modified: %02d/%02d/%d %02d:%02d:%02d\n",
        tm.wMonth, tm.wDay, tm.wYear,
        tm.wHour, tm.wMinute, tm.wSecond);

    cout << buf1 << endl;

    // now output the access rights

    // read first
    wsprintf (buf1, "Read Access: %s", (stat->iCheckReadAccess()) ?
                        "allowed" : "denied");

    cout << buf1 << endl;

    // write
    wsprintf (buf1, "Write Access: %s", (stat->iCheckWriteAccess()) ?
                        "allowed" : "denied");

    cout << buf1 << endl;

    // and execute
    wsprintf (buf1, "Execute Access: %s", (stat->iCheckExecuteAccess()) ?
                        "allowed" : "denied");

    cout << buf1 << endl;
    cout << endl;
}
    
int nReadDirectory (char * sDirName)
{
    char buf1[255];
    WIN32_FIND_DATA FindFileData;
    int nRet= 0; // normal return value (non-zero on error)
    XPCFileStat *pstat;

    if (!*sDirName)
        return nRet;

    if (!strrchr (sDirName, '*'))
        wsprintf (buf1, "%s%s", sDirName, "\\*.*");
    else
        wsprintf (buf1, "%s", sDirName);

    HANDLE hFind= FindFirstFile (buf1, &FindFileData);

    if (!hFind)
        return 1;

    // skip the . and .. entries in the directory
    if (strcmp (FindFileData.cFileName, ".") == 0)
        FindNextFile (hFind, &FindFileData);

    // list all files in the dirctory
    while (FindNextFile (hFind, &FindFileData))
    {
        wsprintf (buf1, "%s\\%s", sDirName, FindFileData.cFileName);

        try // instantiate a XPCFileStat on this file...
        {
            pstat= new XPCFileStat(buf1);
        }
        catch (XPCException ex)
        {
            cout << ex.sGetException() << endl;
            return 1;
        }
        // print the relevant data...
        PrintStat (pstat);

        // if this is a directory, then output its contents
        if (pstat->dwGetFileType() & FILE_ATTRIBUTE_DIRECTORY)
        {
            nRet= nReadDirectory (pstat->sGetFileName());
        }
        // clean-up
        delete pstat;
    }

    // close the find handle
    FindClose (hFind);
    return nRet;
}


int main (int argc, char* argv[])
{
    char buf[255];

    wsprintf (buf, "Processing directory: %s", argv[1]); 
    cout << buf << endl;
    return (nReadDirectory (argv[1]));
}
