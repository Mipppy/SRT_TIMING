// timer_app.h : main header file for the TIMER_APP application
//

#if !defined(AFX_TIMER_APP_H__CBD31217_2993_11D5_BD09_204C4F4F5020__INCLUDED_)
#define AFX_TIMER_APP_H__CBD31217_2993_11D5_BD09_204C4F4F5020__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CTimer_appApp:
// See timer_app.cpp for the implementation of this class
//



class CTimer_appApp : public CWinApp
{
public:
	CTimer_appApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTimer_appApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CTimer_appApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TIMER_APP_H__CBD31217_2993_11D5_BD09_204C4F4F5020__INCLUDED_)
