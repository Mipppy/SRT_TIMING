
// DialogAdvancedSetup.cpp : implementation file
//

#include "stdafx.h"

#include "xc_timer_dll.h"
#include "xc_timer_dll_functions.h"

#include "timer_app.h"
#include "DialogAdvancedSetup.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/**************************************************************************/

extern XC_CONFIGURE_STRUCTURE_TYPE * ptr_c ;

static int exit_allowed_flag ;

/**************************************************************************/

/////////////////////////////////////////////////////////////////////////////
// CDialogAdvancedSetup dialog


CDialogAdvancedSetup::CDialogAdvancedSetup(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogAdvancedSetup::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialogAdvancedSetup)
	m_initial_device1_record = 0;
	m_initial_device2_record = 0;
	m_initial_device3_record = 0;
	m_initial_device4_record = 0;
	m_initial_device5_record = 0;
	m_initial_device6_record = 0;
	m_initial_device7_record = 0;
	m_initial_device8_record = 0;
	m_initial_device9_record = 0;
	m_baud_rate = 0;
	m_output_diagnostic_files_flag = FALSE;
	m_database_highest_record_number = 0;
	m_odbc_user_data_sourse_name = _T("");
	m_odbc_user_dsn_flag = FALSE;
	m_talk_time = 0;
	//}}AFX_DATA_INIT
}


void CDialogAdvancedSetup::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogAdvancedSetup)
	DDX_Text(pDX, IDC_EDIT_DEVICE_INITIAL_RECORD1,            m_initial_device1_record);
	DDX_Text(pDX, IDC_EDIT_DEVICE_INITIAL_RECORD2,            m_initial_device2_record);
	DDX_Text(pDX, IDC_EDIT_DEVICE_INITIAL_RECORD3,            m_initial_device3_record);
	DDX_Text(pDX, IDC_EDIT_DEVICE_INITIAL_RECORD4,            m_initial_device4_record);
	DDX_Text(pDX, IDC_EDIT_DEVICE_INITIAL_RECORD5,            m_initial_device5_record);
	DDX_Text(pDX, IDC_EDIT_DEVICE_INITIAL_RECORD6,            m_initial_device6_record);
	DDX_Text(pDX, IDC_EDIT_DEVICE_INITIAL_RECORD7,            m_initial_device7_record);
	DDX_Text(pDX, IDC_EDIT_DEVICE_INITIAL_RECORD8,            m_initial_device8_record);
	DDX_Text(pDX, IDC_EDIT_DEVICE_INITIAL_RECORD9,            m_initial_device9_record);
	DDX_Text(pDX, IDC_EDIT_BAUD_RATE,                         m_baud_rate);
	DDX_Check(pDX, IDC_CHECK_OUTPUT_DIAGNOSTIC_FILES,         m_output_diagnostic_files_flag);
	DDX_Text(pDX, IDC_EDIT_DATABASE_HIGHEST_RECORD_NUMBER,    m_database_highest_record_number);
	DDX_Text(pDX, IDC_EDIT_ODBC_USER_DATA_SOURCE_NAME,        m_odbc_user_data_sourse_name);
	DDX_Check(pDX, IDC_CHECK_USER_DSN_FLAG,                   m_odbc_user_dsn_flag);
	DDX_Text(pDX, IDC_EDIT_TALK_TIME, m_talk_time);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogAdvancedSetup, CDialog)
	//{{AFX_MSG_MAP(CDialogAdvancedSetup)
	ON_BN_CLICKED(IDC_BUTTON_ERNIES_CANCEL, OnButtonErniesCancel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogAdvancedSetup message handlers


/**************************************************************************/
/**************************************************************************/

// WM_INITDIALOG

BOOL CDialogAdvancedSetup::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

    exit_allowed_flag = 0 ;

    // TODO: Add extra initialization here

    get_configuration_variables () ;
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

/**************************************************************************/

// This routine is called after OK is hit. 

void CDialogAdvancedSetup :: OnOK() 
{
    // TODO: Add extra validation here
 
    UpdateData ( TRUE ) ;

    ptr_c -> baud_rate                           =   m_baud_rate                           ;
                                                                                     
    ptr_c -> diagnostic_flags                    =   m_output_diagnostic_files_flag        ;
    ptr_c -> talk_time                           =   m_talk_time                           ;

    strcpy ( ptr_c -> database_odbc_user_data_source_name      ,     m_odbc_user_data_sourse_name             ) ;

    ptr_c -> odbc_user_dsn_flag                  =   m_odbc_user_dsn_flag     ;

    ptr_c -> highest_record_received_array [ 1 ] =   m_initial_device1_record ;
    ptr_c -> highest_record_received_array [ 2 ] =   m_initial_device2_record ;
    ptr_c -> highest_record_received_array [ 3 ] =   m_initial_device3_record ;
    ptr_c -> highest_record_received_array [ 4 ] =   m_initial_device4_record ;
    ptr_c -> highest_record_received_array [ 5 ] =   m_initial_device5_record ;
    ptr_c -> highest_record_received_array [ 6 ] =   m_initial_device6_record ;
    ptr_c -> highest_record_received_array [ 7 ] =   m_initial_device7_record ;
    ptr_c -> highest_record_received_array [ 8 ] =   m_initial_device8_record ;
    ptr_c -> highest_record_received_array [ 9 ] =   m_initial_device9_record ;

    ptr_c -> database_highest_record_number      =   m_database_highest_record_number ;

    UpdateData ( FALSE ) ;

    dll_save_ini_file () ;
    
    exit_allowed_flag = 1 ;

    CDialog::OnOK();
}

/**************************************************************************/
/**************************************************************************/
/**************************************************************************/
/**************************************************************************/

void CDialogAdvancedSetup :: get_configuration_variables ( void ) /* function declaration */

{
 
  ptr_c   = dll_get_pointer_to_configuration_structure  () ; 

  UpdateData ( TRUE ) ;

  m_baud_rate                                 =  ptr_c -> baud_rate                        ;
                                                                                    
  m_output_diagnostic_files_flag              =  ptr_c -> diagnostic_flags     ;
  m_talk_time                                 =  ptr_c -> talk_time                       ;
 
  m_odbc_user_data_sourse_name                =  ptr_c -> database_odbc_user_data_source_name   ;
  m_odbc_user_dsn_flag                        =  ptr_c -> odbc_user_dsn_flag               ;   

  m_initial_device1_record                    =  ptr_c -> highest_record_received_array [ 1 ]    ;
  m_initial_device2_record                    =  ptr_c -> highest_record_received_array [ 2 ]    ;
  m_initial_device3_record                    =  ptr_c -> highest_record_received_array [ 3 ]    ;
  m_initial_device4_record                    =  ptr_c -> highest_record_received_array [ 4 ]    ;
  m_initial_device5_record                    =  ptr_c -> highest_record_received_array [ 5 ]    ;
  m_initial_device6_record                    =  ptr_c -> highest_record_received_array [ 6 ]    ;
  m_initial_device7_record                    =  ptr_c -> highest_record_received_array [ 7 ]    ;
  m_initial_device8_record                    =  ptr_c -> highest_record_received_array [ 8 ]    ;
  m_initial_device9_record                    =  ptr_c -> highest_record_received_array [ 9 ]    ;

  m_database_highest_record_number            =  ptr_c -> database_highest_record_number   ;

  UpdateData ( FALSE ) ;

}

/**************************************************************************/
/**************************************************************************/

void CDialogAdvancedSetup::OnCancel() 
{
	// TODO: Add extra cleanup here


  if ( exit_allowed_flag == 1 ) {
	CDialog::OnCancel();
  }
}
/**************************************************************************/

void CDialogAdvancedSetup::OnButtonErniesCancel() 
{
	// TODO: Add your control notification handler code here
	
	// TODO: Add your control notification handler code here
	exit_allowed_flag = 1 ;
	CDialog::OnCancel() ;

}

/**************************************************************************/
/**************************************************************************/
/**************************************************************************/
