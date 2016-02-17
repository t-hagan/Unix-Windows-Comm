// TCPThread.h : main header file for the TCPTHREAD application
//

#if !defined(AFX_TCPTHREAD_H__B874DF24_D1B8_11D2_B23B_00A0C90C4DA6__INCLUDED_)
#define AFX_TCPTHREAD_H__B874DF24_D1B8_11D2_B23B_00A0C90C4DA6__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#ifndef __AFXWIN_H__
    #error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"        // main symbols

/////////////////////////////////////////////////////////////////////////////
// CTCPThreadApp:
// See TCPThread.cpp for the implementation of this class
//

class CTCPThreadApp : public CWinApp
{
public:
    CTCPThreadApp();

// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CTCPThreadApp)
    public:
    virtual BOOL InitInstance();
    //}}AFX_VIRTUAL

// Implementation

    //{{AFX_MSG(CTCPThreadApp)
        // NOTE - the ClassWizard will add and remove member functions here.
        //    DO NOT EDIT what you see in these blocks of generated code !
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TCPTHREAD_H__B874DF24_D1B8_11D2_B23B_00A0C90C4DA6__INCLUDED_)
