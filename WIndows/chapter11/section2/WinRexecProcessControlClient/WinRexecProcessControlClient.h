// WinRexecProcessControlClient.h : main header file for the WINREXECPROCESSCONTROLCLIENT application
//

#if !defined(AFX_WINREXECPROCESSCONTROLCLIENT_H__5B279155_DB26_11D2_B23B_00A0C90C4DA6__INCLUDED_)
#define AFX_WINREXECPROCESSCONTROLCLIENT_H__5B279155_DB26_11D2_B23B_00A0C90C4DA6__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#ifndef __AFXWIN_H__
    #error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"        // main symbols

/////////////////////////////////////////////////////////////////////////////
// CWinRexecProcessControlClientApp:
// See WinRexecProcessControlClient.cpp for the implementation of this class
//

class CWinRexecProcessControlClientApp : public CWinApp
{
public:
    CWinRexecProcessControlClientApp();

// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CWinRexecProcessControlClientApp)
    public:
    virtual BOOL InitInstance();
    //}}AFX_VIRTUAL

// Implementation

    //{{AFX_MSG(CWinRexecProcessControlClientApp)
        // NOTE - the ClassWizard will add and remove member functions here.
        //    DO NOT EDIT what you see in these blocks of generated code !
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WINREXECPROCESSCONTROLCLIENT_H__5B279155_DB26_11D2_B23B_00A0C90C4DA6__INCLUDED_)
