// NewsPublisher.h : main header file for the NEWSPUBLISHER application
//

#if !defined(AFX_NEWSPUBLISHER_H__03E035E5_C02A_11D2_B23B_00A0C90C4DA6__INCLUDED_)
#define AFX_NEWSPUBLISHER_H__03E035E5_C02A_11D2_B23B_00A0C90C4DA6__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#ifndef __AFXWIN_H__
    #error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"        // main symbols

/////////////////////////////////////////////////////////////////////////////
// CNewsPublisherApp:
// See NewsPublisher.cpp for the implementation of this class
//

class CNewsPublisherApp : public CWinApp
{
public:
    CNewsPublisherApp();

// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CNewsPublisherApp)
    public:
    virtual BOOL InitInstance();
    //}}AFX_VIRTUAL

// Implementation

    //{{AFX_MSG(CNewsPublisherApp)
        // NOTE - the ClassWizard will add and remove member functions here.
        //    DO NOT EDIT what you see in these blocks of generated code !
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NEWSPUBLISHER_H__03E035E5_C02A_11D2_B23B_00A0C90C4DA6__INCLUDED_)
