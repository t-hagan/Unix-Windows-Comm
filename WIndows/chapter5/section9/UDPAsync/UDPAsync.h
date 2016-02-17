// UDPAsync.h : main header file for the UDPASYNC application
//

#if !defined(AFX_UDPASYNC_H__13136A15_D0A7_11D2_B23B_00A0C90C4DA6__INCLUDED_)
#define AFX_UDPASYNC_H__13136A15_D0A7_11D2_B23B_00A0C90C4DA6__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#ifndef __AFXWIN_H__
    #error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"        // main symbols

/////////////////////////////////////////////////////////////////////////////
// CUDPAsyncApp:
// See UDPAsync.cpp for the implementation of this class
//

class CUDPAsyncApp : public CWinApp
{
public:
    CUDPAsyncApp();

// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CUDPAsyncApp)
    public:
    virtual BOOL InitInstance();
    //}}AFX_VIRTUAL

// Implementation

    //{{AFX_MSG(CUDPAsyncApp)
        // NOTE - the ClassWizard will add and remove member functions here.
        //    DO NOT EDIT what you see in these blocks of generated code !
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_UDPASYNC_H__13136A15_D0A7_11D2_B23B_00A0C90C4DA6__INCLUDED_)
