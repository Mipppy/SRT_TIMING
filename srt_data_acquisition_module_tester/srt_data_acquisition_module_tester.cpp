// srt_data_acquisition_module_tester.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "srt_data_acquisition_module_tester.h"
#include "srt_data_acquisition_module_tester_dialog_box.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// class_data_acquisition_module_tester_app

BEGIN_MESSAGE_MAP(class_data_acquisition_module_tester_app, CWinApp)
	//{{AFX_MSG_MAP(class_data_acquisition_module_tester_app)
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// class_data_acquisition_module_tester_app construction

class_data_acquisition_module_tester_app::class_data_acquisition_module_tester_app()
{
}

/////////////////////////////////////////////////////////////////////////////
// The one and only class_data_acquisition_module_tester_app object

class_data_acquisition_module_tester_app theApp;

/////////////////////////////////////////////////////////////////////////////
// class_data_acquisition_module_tester_app initialization

BOOL class_data_acquisition_module_tester_app::InitInstance()
{
	// Standard initialization

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	class_data_acquisition_module_tester_dialog_box dlg;
	m_pMainWnd = &dlg;
	int nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
	}
	else if (nResponse == IDCANCEL)
	{
	}

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}
