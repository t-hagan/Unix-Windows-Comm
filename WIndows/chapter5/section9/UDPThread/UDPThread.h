// UDPThread.h : main header file for the UDPTHREAD application
//

#if !defined(AFX_UDPTHREAD_H__ED53B394_D304_11D2_B23B_00A0C90C4DA6__INCLUDED_)
#define AFX_UDPTHREAD_H__ED53B394_D304_11D2_B23B_00A0C90C4DA6__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#ifndef __AFXWIN_H__
    #error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"        // main symbols

/////////////////////////////////////////////////////////////////////////////
// CUDPThreadApp:
// See UDPThread.cpp for the implementation of this class
//

class CUDPThreadApp : public CWinApp
{
public:
    CUDPThreadApp();

// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CUDPThreadApp)
    public:
    virtual BOOL InitInstance();
    //}}AFX_VIRTUAL

// Implementation

    //{{AFX_MSG(CUDPThreadApp)
        // NOTE - the ClassWizard will add and remove member functions here.
        //    DO NOT EDIT what you see in these blocks of generated code !
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_UDPTHREAD_H__ED53B394_D304_11D2_B23B_00A0C90C4DA6__INCLUDED_)
