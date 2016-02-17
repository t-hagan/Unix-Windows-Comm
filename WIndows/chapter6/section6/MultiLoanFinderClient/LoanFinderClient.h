// LoanFinderClient.h : main header file for the LOANFINDERCLIENT application
//

#if !defined(AFX_LOANFINDERCLIENT_H__B246FBE9_D27C_11D2_B23B_00A0C90C4DA6__INCLUDED_)
#define AFX_LOANFINDERCLIENT_H__B246FBE9_D27C_11D2_B23B_00A0C90C4DA6__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#ifndef __AFXWIN_H__
    #error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"        // main symbols

/////////////////////////////////////////////////////////////////////////////
// CLoanFinderClientApp:
// See LoanFinderClient.cpp for the implementation of this class
//

class CLoanFinderClientApp : public CWinApp
{
public:
    CLoanFinderClientApp();

// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CLoanFinderClientApp)
    public:
    virtual BOOL InitInstance();
    //}}AFX_VIRTUAL

// Implementation

    //{{AFX_MSG(CLoanFinderClientApp)
        // NOTE - the ClassWizard will add and remove member functions here.
        //    DO NOT EDIT what you see in these blocks of generated code !
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LOANFINDERCLIENT_H__B246FBE9_D27C_11D2_B23B_00A0C90C4DA6__INCLUDED_)
