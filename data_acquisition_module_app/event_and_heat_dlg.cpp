
// _Event_and_Heat_Dlg.cpp : implementation file
//

#include "stdafx.h"

#include "xc_timer_dll.h"
#include "xc_timer_dll_functions.h"

#include "timer_app.h"

#include "event_and_heat_dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////

extern XC_CONFIGURE_STRUCTURE_TYPE * ptr_c ;
unsigned int device_number ;

/////////////////////////////////////////////////////////////////////////////

// c_event_and_heat_dlg dialog

c_event_and_heat_dlg::c_event_and_heat_dlg(CWnd* pParent /*=NULL*/)
	: CDialog(c_event_and_heat_dlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(c_event_and_heat_dlg)
	m_device_number = 0;
	m_event_number  = 0;
	m_heat_number   = 0;
	//}}AFX_DATA_INIT
}


void c_event_and_heat_dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(c_event_and_heat_dlg)
	DDX_Text(pDX, IDC_EDIT_DEVICE, m_device_number);
	DDX_Text(pDX, IDC_EDIT_EVENT,  m_event_number);
	DDX_Text(pDX, IDC_EDIT_HEAT,   m_heat_number);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(c_event_and_heat_dlg, CDialog)
	//{{AFX_MSG_MAP(c_event_and_heat_dlg)
	ON_BN_CLICKED(IDC_BUTTON_SEND_TO_EVERY_DEVICE, OnButtonSendToEveryDevice)
	ON_BN_CLICKED(IDC_BUTTON_SEND_TO_ONE_DEVICE, OnButtonSendToOneDevice)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////

// c_event_and_heat_dlg message handlers

void c_event_and_heat_dlg::OnButtonSendToEveryDevice() 
{
	// TODO: Add your control notification handler code here

     UpdateData ( TRUE ) ;

    device_number        = 0               ;
    ptr_c -> event_number    = m_event_number  ;
    ptr_c -> heat_number     = m_heat_number   ;
  
    UpdateData ( FALSE ) ;

    dll_set_event_and_heat ( (long *)( & device_number ), (long *) ( & ( ptr_c -> event_number ) ), (long *) ( & ( ptr_c -> heat_number ) ) ) ;

    CDialog::OnOK();

}

/////////////////////////////////////////////////////////////////////////////

void c_event_and_heat_dlg::OnButtonSendToOneDevice() 
{

	// TODO: Add your control notification handler code here

      UpdateData ( TRUE ) ;

    device_number            = m_device_number ;
    ptr_c -> event_number    = m_event_number  ;
    ptr_c -> heat_number     = m_heat_number   ;
  
    UpdateData ( FALSE ) ;

    dll_set_event_and_heat ( (long *)( & device_number ), (long *) ( & ( ptr_c -> event_number ) ), (long *) ( & ( ptr_c -> heat_number ) ) ) ;

    CDialog::OnOK();
	
}

/////////////////////////////////////////////////////////////////////////////

BOOL c_event_and_heat_dlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE

    ptr_c   = dll_get_pointer_to_configuration_structure  () ; 

    UpdateData ( TRUE ) ;

    m_event_number   =  ptr_c -> event_number  ;
    m_heat_number    =  ptr_c -> heat_number   ;
  
    UpdateData ( FALSE ) ;

}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
