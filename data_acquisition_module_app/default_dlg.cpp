// default_dlg.cpp : implementation file
//

#include "stdafx.h"
#include "timer_app.h"
#include "default_dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// default_dlg dialog


default_dlg::default_dlg(CWnd* pParent /*=NULL*/)
	: CDialog(default_dlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(default_dlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void default_dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(default_dlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(default_dlg, CDialog)
	//{{AFX_MSG_MAP(default_dlg)
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// default_dlg message handlers

void default_dlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}

void default_dlg::OnOK() 
{
	// TODO: Add extra validation here
	
	CDialog::OnOK();
}

void default_dlg::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	
	CDialog::OnClose();
}
