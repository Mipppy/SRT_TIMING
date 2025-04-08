// AdvancedSetup.cpp : implementation file
//

#include "stdafx.h"
#include "timer_app.h"
#include "AdvancedSetup.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAdvancedSetup dialog


CAdvancedSetup::CAdvancedSetup(CWnd* pParent /*=NULL*/)
	: CDialog(CAdvancedSetup::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAdvancedSetup)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CAdvancedSetup::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAdvancedSetup)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAdvancedSetup, CDialog)
	//{{AFX_MSG_MAP(CAdvancedSetup)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAdvancedSetup message handlers
