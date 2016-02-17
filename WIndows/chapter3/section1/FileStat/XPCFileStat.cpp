// FileStat.cpp: implementation of the CFileStat class.
//
//////////////////////////////////////////////////////////////////////

#include "XPCFileStat.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

XPCFileStat::XPCFileStat()
{
    // get the size of the buffer needed first
    int iBufSize= GetCurrentDirectory (0, sFileName);

    // allocate memory to store the path name
    sFileName= (char *) HeapAlloc (GetProcessHeap(), 0, iBufSize+1);

    // now get the current working directory
    if (!sFileName || !GetCurrentDirectory (iBufSize, sFileName))
    {
        // throw an exception if we failed
        XPCException ex 
                (sErrorString("GetCurrentDirectory failed."));
        throw ex;
        return;
    }
    // call the worker function to get the stat information
    vGetStat (sFileName);
}


XPCFileStat::XPCFileStat(char * _sFName)
{
    // allocate space  for the path name
    sFileName= (char *) HeapAlloc (GetProcessHeap(), 0, strlen(_sFName)+1);
    if (!sFileName)
    {
        // throw and exception if we couldn't get the memory
        XPCException ex 
                (sErrorString("Could not allocate memory for sFileName"));
        throw ex;
        return;
    }
    // copy the path name in
    strcpy (sFileName, _sFName);
    // call the worker function to get the stat info
    vGetStat (sFileName);
}

XPCFileStat::~XPCFileStat()
{

    HeapFree (GetProcessHeap(), 0, sFileName); 
    HeapFree (GetProcessHeap(), 0, pSD);
    HeapFree (GetProcessHeap(), 0, pGroupInfo);
    HeapFree (GetProcessHeap(), 0, pUserInfo);

    LocalFree (pOwner);
    LocalFree (pGroup);
    LocalFree (pListOfAccessEntries);
    LocalFree (pListOfAuditEntries);

    CloseHandle (hProcessToken);
}

XPCFileStat::XPCFileStat(const XPCFileStat& rhs)
{
    sFileName= (char *) HeapAlloc (GetProcessHeap(), 0, strlen(rhs.sFileName)+1);
    strcpy (sFileName, rhs.sFileName);

    fileAttr.dwFileAttributes= rhs.fileAttr.dwFileAttributes;
    fileAttr.ftCreationTime= rhs.fileAttr.ftCreationTime;
    fileAttr.ftLastAccessTime= rhs.fileAttr.ftLastAccessTime;
    fileAttr.ftLastWriteTime= rhs.fileAttr.ftLastWriteTime;
    fileAttr.nFileSizeHigh= rhs.fileAttr.nFileSizeHigh;
    fileAttr.nFileSizeLow= rhs.fileAttr.nFileSizeLow;
}


int XPCFileStat::iCheckReadAccess()
{

    for (ULONG i= 0; i < lNumAccessEntries; i++)
    {
        if (iCheckCurrentUserAndGroups (pListOfAccessEntries[i].Trustee))
        {
            if (pListOfAccessEntries[i].grfAccessPermissions & FILE_READ_DATA)
            {
                if (pListOfAccessEntries[i].grfAccessMode == SET_ACCESS)
                    return TRUE;
                else if (pListOfAccessEntries[i].grfAccessMode == DENY_ACCESS)
                    return FALSE;
            }
        }
    }
    return TRUE;
}

int XPCFileStat::iCheckWriteAccess()
{
    // read only attribute has been set on the file so no one
    // can write to it!
    if ((fileAttr.dwFileAttributes & FILE_ATTRIBUTE_READONLY))
        return FALSE;

    for (ULONG i= 0; i < lNumAccessEntries; i++)
    {
        if (iCheckCurrentUserAndGroups (pListOfAccessEntries[i].Trustee))
        {
            if (pListOfAccessEntries[i].grfAccessPermissions & FILE_WRITE_DATA)
            {
                if (pListOfAccessEntries[i].grfAccessMode == SET_ACCESS)
                    return TRUE;
                else if (pListOfAccessEntries[i].grfAccessMode == DENY_ACCESS)
                    return FALSE;
            }
        }
    }
    return TRUE;
}

int XPCFileStat::iCheckExecuteAccess()
{
    int nRet= TRUE;


    for (ULONG i= 0; i < lNumAccessEntries; i++)
    {
        if (iCheckCurrentUserAndGroups (pListOfAccessEntries[i].Trustee))
        {
            if (pListOfAccessEntries[i].grfAccessPermissions & FILE_EXECUTE)
            {
                if (pListOfAccessEntries[i].grfAccessMode == SET_ACCESS)
                    return TRUE; 
                else if (pListOfAccessEntries[i].grfAccessMode == DENY_ACCESS)
                    nRet= FALSE;
            }
        }
    }
    return nRet;
}

int XPCFileStat::iCheckCurrentUserAndGroups(TRUSTEE _tGroupOrUser)
{
    char sAccountName[255];
    char sReferencedDomainName[255];
    DWORD dwAccountSize = sizeof sAccountName;
    DWORD dwDomainSize = sizeof sReferencedDomainName;
    SID_NAME_USE eUse;

    // check through the access list of the current user
    // to see if any group he or she is a member of is 
    // the group passed in by tGroupOrUser

    // see if the trustee is a user
    if (_tGroupOrUser.TrusteeType == TRUSTEE_IS_USER)
    {
        // handle the case where the trustee info is a SID
        if (_tGroupOrUser.TrusteeForm == TRUSTEE_IS_SID)
        {
            if (EqualSid ((PSID) _tGroupOrUser.ptstrName, 
                            pUserInfo->User.Sid))
                return TRUE;
        }
        // or a user name
        else if (_tGroupOrUser.TrusteeForm == TRUSTEE_IS_NAME)
        {
            // look up the acct info for the app's user
            if (!LookupAccountSid (NULL,
                    pUserInfo->User.Sid,
                    sAccountName,
                    &dwAccountSize,
                    sReferencedDomainName,
                    &dwDomainSize,
                    &eUse))
            {
                    return FALSE;
            }
            // and compare it to the trustee's name
            if (strcmp (sAccountName, 
                        _tGroupOrUser.ptstrName) == 0)
                return TRUE;
        }
    }
    // here the trustee is a group
    else if (_tGroupOrUser.TrusteeType == TRUSTEE_IS_GROUP)
    {
        // loop through the list of groups the app
        // is member of 
        for (UINT i= 0; i < pGroupInfo->GroupCount; i++)
        {
            if (!IsValidSid (pGroupInfo->Groups[i].Sid))
                continue;

            // the trustee is a SID
            if (_tGroupOrUser.TrusteeForm == TRUSTEE_IS_SID)
            {
                if (EqualSid ((PSID) _tGroupOrUser.ptstrName,
                            &pGroupInfo->Groups[i].Sid))
                    return TRUE;
            }
            // the trustee is a name
            else if (_tGroupOrUser.TrusteeForm == TRUSTEE_IS_NAME)
            {
                // lookup the acct info for this groups SID
                if (!LookupAccountSid (NULL,
                        pGroupInfo->Groups[i].Sid,
                        sAccountName,
                        &dwAccountSize,
                        sReferencedDomainName,
                        &dwDomainSize,
                        &eUse))
                {
                        continue;
                }
                // compare the name of the group to
                // the trustee's name
                if (strcmp (sAccountName,
                            _tGroupOrUser.ptstrName) == 0)
                    return TRUE;
            }
        }
    }
    return FALSE; // not found!
}

void XPCFileStat::vGetStat(char *_sFName)
{
    if (!GetFileAttributesEx (_sFName,
                GetFileExInfoStandard,
                (LPVOID) &fileAttr))
    {
        XPCException ex (sErrorString("GetFileAttributesEx failed"));
        throw ex;
        return;
    }

    DWORD dwSDSize= 0;
    // first call gets the size of the security descriptor
    GetFileSecurity (_sFName,
            OWNER_SECURITY_INFORMATION | GROUP_SECURITY_INFORMATION |
            DACL_SECURITY_INFORMATION,
            NULL,
            dwSDSize,
            &dwSDSize);

    // allocate memory for it
    pSD= (PSECURITY_DESCRIPTOR) HeapAlloc (GetProcessHeap(), 0, dwSDSize);

    // now get the descriptor
    if (!pSD || !GetFileSecurity (_sFName,
                OWNER_SECURITY_INFORMATION | GROUP_SECURITY_INFORMATION |
                DACL_SECURITY_INFORMATION,
                pSD,
                dwSDSize,
                &dwSDSize))
    {
        XPCException ex (sErrorString("GetFileSecurity failed"));
        throw ex;
        return;
    }

    // and look-up the components of that descriptor
    if (LookupSecurityDescriptorParts (&pOwner,
                &pGroup,
                &lNumAccessEntries,
                &pListOfAccessEntries,
                &pNumAuditEntries,
                &pListOfAuditEntries,
                pSD) != ERROR_SUCCESS)
    {
        XPCException ex (sErrorString("LookupSecurityDescriptorParts failed"));
        throw ex;
        return;
    }

    // get a handle to the current process
    hProcess= GetCurrentProcess();

    // open its process token
    if (!OpenProcessToken (hProcess,
                TOKEN_QUERY | TOKEN_DUPLICATE,
                &hProcessToken))
    {
        XPCException ex 
                (sErrorString("OpenProcessToken failed."));
        throw ex;
        return;
    }

    DWORD dwTokenSize= 0;

    // now get the groups this process token is a member of…
    // first get the size required for the group information
    GetTokenInformation (hProcessToken,
                    TokenGroups,
                    NULL,
                    dwTokenSize,
                    &dwTokenSize);

    pGroupInfo= (PTOKEN_GROUPS) HeapAlloc (GetProcessHeap(), 0, 
                            dwTokenSize);

    // now get the group information
    if (!pGroupInfo || !GetTokenInformation (hProcessToken,
                TokenGroups,
                pGroupInfo,
                dwTokenSize,
                &dwTokenSize))
    {
        XPCException ex 
            (sErrorString("GetTokenInformation failed."));
        throw ex;
        return;
    }

    // get the size required for the user information
    GetTokenInformation (hProcessToken,
                    TokenUser,
                    NULL,
                    dwTokenSize,
                    &dwTokenSize);

    pUserInfo= (PTOKEN_USER) HeapAlloc (GetProcessHeap(), 0, 
                    dwTokenSize);

    // get the user information
    if (!pGroupInfo || !GetTokenInformation (hProcessToken,
                            TokenUser,
                            pUserInfo,
                            dwTokenSize,
                            &dwTokenSize))
    {
        XPCException ex 
            (sErrorString("GetTokenInformation failed."));
        throw ex;
        return;
    }
}

char * XPCFileStat::sErrorString(char *sLeader)
{
    static char buf[255];
    LPVOID lpMsgBuf;
    UINT ErrNo;

    FormatMessage (
            FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
            NULL,
            ErrNo=GetLastError (),
            MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
            (LPTSTR) &lpMsgBuf,
            0,
            NULL);

    wsprintf (buf, "%s: %s", sLeader, (LPTSTR)lpMsgBuf);

    LocalFree (lpMsgBuf);
    return buf;
}
