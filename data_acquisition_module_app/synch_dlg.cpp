
// synch_dlg.cpp : implementation file
//

#include "stdafx.h"

#include "xc_timer_dll.h"
#include "xc_timer_dll_functions.h"

#include "timer_app.h"
#include "synch_dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// synch_dlg dialog


synch_dlg::synch_dlg(CWnd* pParent /*=NULL*/)
	: CDialog(synch_dlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(synch_dlg)
	m_user_synch_time = _T("");
	//}}AFX_DATA_INIT
}


void synch_dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(synch_dlg)
	DDX_Text(pDX, IDC_EDIT_USER_SYNCH_TIME, m_user_synch_time);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(synch_dlg, CDialog)
	//{{AFX_MSG_MAP(synch_dlg)
	ON_BN_CLICKED(IDC_BUTTON_SYNCH_TO_SYSTEM, OnButtonSynchToSystem)
	ON_BN_CLICKED(IDC_BUTTON_SYNCH_TO_USER_TIME, OnButtonSynchToUserTime)
	ON_BN_CLICKED(IDC_BUTTON_ADJUST_SYNCH_TIME, OnButtonAdjustSynchTime)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/*************************************************************************************/
/*************************************************************************************/

/////////////////////////////////////////////////////////////////////////////
// synch_dlg message handlers

BOOL synch_dlg::OnInitDialog() 

{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
   m_user_synch_time = "0:0:0.0" ;

   UpdateData ( FALSE ) ;

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

/*******************/

void synch_dlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}

/*******************/

void synch_dlg::OnButtonSynchToSystem() 
{
	// TODO: Add your control notification handler code here

    long synch_flag = (long) SYNCH_TIMERS_TO_SYSTEM_CLOCK ;

    dll_synch_timers ( & synch_flag, NULL ) ;
	
	CDialog::OnOK();

}

/*******************/

void synch_dlg::OnButtonSynchToUserTime() 
{
	// TODO: Add your control notification handler code here

    char synch_time_string   [ 300 ] ;

    long synch_flag = (long) SYNCH_TIMERS_TO_USER_CLOCK ;


    UpdateData ( TRUE ) ;

    strcpy ( synch_time_string, m_user_synch_time ) ;

    dll_synch_timers ( & synch_flag, synch_time_string ) ;

	CDialog::OnOK();
	
}


/*************************************************************************************/
/*************************************************************************************/

void synch_dlg::OnButtonAdjustSynchTime() 
{
	// TODO: Add your control notification handler code here

    char synch_time_string   [ 300 ] ;

    long synch_flag = (long) SYNCH_TIMERS_ADJUSTMENT ;


    UpdateData ( TRUE ) ;

    strcpy ( synch_time_string, m_user_synch_time ) ;
    dll_synch_timers ( & synch_flag, synch_time_string ) ;
	
	CDialog::OnOK();
	
}

/*************************************************************************************/
/*************************************************************************************/
