// srt_data_acquisition_module_tester.h : main header file for the SRT_DATA_ACQUISITION_MODULE_TESTER application
//

#if !defined(AFX_SRT_DATA_ACQUISITION_MODULE_TESTER_H__8611FA5C_FA71_4073_AF85_275C9E092450__INCLUDED_)
#define AFX_SRT_DATA_ACQUISITION_MODULE_TESTER_H__8611FA5C_FA71_4073_AF85_275C9E092450__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// class_data_acquisition_module_tester_app:
// See srt_data_acquisition_module_tester.cpp for the implementation of this class
//

class class_data_acquisition_module_tester_app : public CWinApp
{
public:
	class_data_acquisition_module_tester_app();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(class_data_acquisition_module_tester_app)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(class_data_acquisition_module_tester_app)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SRT_DATA_ACQUISITION_MODULE_TESTER_H__8611FA5C_FA71_4073_AF85_275C9E092450__INCLUDED_)
