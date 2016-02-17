// PurchaseSystem.h : main header file for the PURCHASESYSTEM application
//

#if !defined(AFX_PURCHASESYSTEM_H__0E57C9C5_9F2F_11D2_B237_00A0C90C4DA6__INCLUDED_)
#define AFX_PURCHASESYSTEM_H__0E57C9C5_9F2F_11D2_B237_00A0C90C4DA6__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#ifndef __AFXWIN_H__
    #error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"        // main symbols

/////////////////////////////////////////////////////////////////////////////
// CPurchaseSystemApp:
// See PurchaseSystem.cpp for the implementation of this class
//

class CPurchaseSystemApp : public CWinApp
{
public:
    CPurchaseSystemApp();

// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CPurchaseSystemApp)
    public:
    virtual BOOL InitInstance();
    //}}AFX_VIRTUAL

// Implementation

    //{{AFX_MSG(CPurchaseSystemApp)
        // NOTE - the ClassWizard will add and remove member functions here.
        //    DO NOT EDIT what you see in these blocks of generated code !
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PURCHASESYSTEM_H__0E57C9C5_9F2F_11D2_B237_00A0C90C4DA6__INCLUDED_)
