
// configure_dlg.cpp : implementation file
//

#include "stdafx.h"
#include "direct.h"

#include "xc_timer_dll.h"
#include "xc_timer_dll_functions.h"

#include "timer_app.h"
#include "configure_dlg.h"
#include "DialogAdvancedSetup.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/**************************************************************************/

extern XC_CONFIGURE_STRUCTURE_TYPE    * ptr_c ;
extern GLOBAL_VARIABLE_STRUCTURE_TYPE * ptr_g ;

static int exit_allowed_flag ;

/**************************************************************************/

/////////////////////////////////////////////////////////////////////////////
// configure_dlg dialog


configure_dlg::configure_dlg(CWnd* pParent /*=NULL*/)
    : CDialog(configure_dlg::IDD, pParent)
{

  //{{AFX_DATA_INIT(configure_dlg)
    m_comm_port_string                = _T("");
    m_number_of_devices_used          =  0 ;
    m_current_working_directory       = _T("");
    m_database_type                   = _T("");
    m_database_table_name             = _T("");
    m_database_drive                  = _T("");
    m_database_directory              = _T("");
    m_database_name                   = _T("");
	m_database_extension              = _T("");
  	m_database_path_and_file_name     = _T("");
  	m_timer_type_used                 = _T("");
	//}}AFX_DATA_INIT

}


void configure_dlg::DoDataExchange(CDataExchange* pDX)
{

  CDialog::DoDataExchange(pDX);

  //{{AFX_DATA_MAP(configure_dlg)
    DDX_CBString ( pDX, IDC_COMBO_COMM_PORT,                  m_comm_port_string                 ) ;
    DDX_Text     ( pDX, IDC_EDIT_NUMBER_OF_DEVICES,           m_number_of_devices_used           ) ;
    DDX_Text     ( pDX, IDC_EDIT_CURRENT_WORKING_DIRECTORY,   m_current_working_directory        ) ;
    DDX_CBString ( pDX, IDC_COMBO_DATABASE_TYPE,              m_database_type                    ) ;
    DDX_Text     ( pDX, IDC_EDIT_DATABASE_TABLE_NAME,         m_database_table_name              ) ;
    DDX_Text     ( pDX, IDC_EDIT_DATABASE_DRIVE,              m_database_drive                   ) ;
    DDX_Text     ( pDX, IDC_EDIT_DATABASE_DIRECTORY,          m_database_directory               ) ;
    DDX_Text     ( pDX, IDC_EDIT_DATABASE_NAME,               m_database_name                    ) ;
	DDX_Text     ( pDX, IDC_EDIT_DATABASE_EXTENSION,          m_database_extension               ) ;
	DDX_Text     ( pDX, IDC_EDIT_DATABASE_PATH_AND_FILE_NAME, m_database_path_and_file_name      ) ;
	DDX_CBString ( pDX, IDC_COMBO_TIMER_TYPE_USED,            m_timer_type_used                  ) ;
	//}}AFX_DATA_MAP
                                                                                                   
                                                                                                   
}                                                                                                  


BEGIN_MESSAGE_MAP(configure_dlg, CDialog)
    //{{AFX_MSG_MAP(configure_dlg)
    ON_BN_CLICKED     ( IDC_BUTTON_RESTORE_DEFAULT_VALUES, OnButtonRestoreDefaultValues  )
	ON_EN_KILLFOCUS   ( IDC_EDIT_DATABASE_DRIVE,           OnKillfocusEditDatabaseDrive)
	ON_EN_KILLFOCUS   ( IDC_EDIT_DATABASE_DIRECTORY,       OnKillfocusEditDatabaseDirectory)
	ON_BN_CLICKED     ( IDC_BUTTON_ERNIES_CANCEL,          OnButtonErniesCancel)
	ON_BN_CLICKED(IDC_BUTTON_ADVANCED, OnButtonAdvanced)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// configure_dlg message handlers

/**************************************************************************/
/////////////////////////////////////////////////////////////////////////////
// configure_dlg message handlers

/**************************************************************************/
/**************************************************************************/

BOOL configure_dlg::OnInitDialog() 

{

    char scratch_string [ 500 ] ;

    CDialog::OnInitDialog();
    
    exit_allowed_flag = 0 ;

    // TODO: Add extra initialization here

    _getcwd ( scratch_string, 500 ) ;

    UpdateData ( TRUE ) ;

    m_current_working_directory = scratch_string ;

    UpdateData ( FALSE ) ;

    get_configuration_variables () ;
    
    return TRUE;  // return TRUE unless you set the focus to a control
                  // EXCEPTION: OCX Property Pages should return FALSE
}


/**************************************************************************/
/**************************************************************************/

void configure_dlg::OnOK() 

{
    // TODO: Add extra validation here
 
    UpdateData ( TRUE ) ;

    if ( strcmpi (  m_comm_port_string, "Comm1" ) == 0 )  ptr_c -> serial_comm_port = 1  ;
    if ( strcmpi (  m_comm_port_string, "Comm2" ) == 0 )  ptr_c -> serial_comm_port = 2  ;
    if ( strcmpi (  m_comm_port_string, "Comm3" ) == 0 )  ptr_c -> serial_comm_port = 3  ;
    if ( strcmpi (  m_comm_port_string, "Comm4" ) == 0 )  ptr_c -> serial_comm_port = 4  ;
                                                                                     
    ptr_c -> number_of_devices_used            =   m_number_of_devices_used ;


    strcpy ( ptr_c -> timer_type_used                     ,     m_timer_type_used                   ) ;
    strcpy ( ptr_c -> database_type                       ,     m_database_type                     ) ;
    strcpy ( ptr_c -> database_drive                      ,     m_database_drive                    ) ;
    strcpy ( ptr_c -> database_directory                  ,     m_database_directory                ) ;
    strcpy ( ptr_c -> database_name                       ,     m_database_name                     ) ;
    strcpy ( ptr_c -> database_extension                  ,     m_database_extension                ) ;
    strcpy ( ptr_c -> database_table_name                 ,     m_database_table_name               ) ;

    UpdateData ( FALSE ) ;

    dll_save_ini_file () ;
    
    exit_allowed_flag = 1 ;

    CDialog::OnOK();
}

/**************************************************************************/
/**************************************************************************/
/**************************************************************************/

void configure_dlg::OnButtonRestoreDefaultValues() 
{
    // TODO: Add your control notification handler code here

  dll_assign_default_values () ;
  get_configuration_variables () ;
    
}

/**************************************************************************/
/**************************************************************************/

// This function is called on init dialog box, and on restore default values

void configure_dlg :: get_configuration_variables ( void ) /* function declaration */

{

  char scratch_string [ 600 ] ;
  
  ptr_c   = dll_get_pointer_to_configuration_structure  () ; 
  ptr_g   = dll_get_pointer_to_global_variable_structure  () ; 

  _makepath  ( ptr_g -> database_path_and_file_name, ptr_c -> database_drive, ptr_c -> database_directory, ptr_c -> database_name, ptr_c -> database_extension ) ;

  sprintf ( scratch_string, "Comm%d", ptr_c -> serial_comm_port ) ;

  UpdateData ( TRUE ) ;

  m_comm_port_string                          =  scratch_string                        ;
                                                                                    
  m_number_of_devices_used                    =  ptr_c -> number_of_devices_used           ;

  m_timer_type_used                           =  ptr_c -> timer_type_used                  ;

  m_database_type                             =  ptr_c -> database_type                    ;
  m_database_drive                            =  ptr_c -> database_drive                   ;
  m_database_directory                        =  ptr_c -> database_directory               ;
  m_database_name                             =  ptr_c -> database_name                    ;
  m_database_extension                        =  ptr_c -> database_extension               ;
  m_database_table_name                       =  ptr_c -> database_table_name              ;

  m_database_path_and_file_name               =  ptr_g -> database_path_and_file_name           ;

  UpdateData ( FALSE ) ;

}

/**************************************************************************/
/**************************************************************************/
/**************************************************************************/

void configure_dlg::OnKillfocusEditDatabaseDrive() 
{
	// TODO: Add your control notification handler code here
    config_get_database_path_and_name () ;
}

void configure_dlg::OnKillfocusEditDatabaseDirectory() 
{

  // TODO: Add your control notification handler code here
  config_get_database_path_and_name () ;
 
}

/**************************************************************************/
/**************************************************************************/

void configure_dlg :: config_get_database_path_and_name ( void ) /* function declaration */

{

  UpdateData ( TRUE ) ;

  _makepath  ( ptr_g -> database_path_and_file_name, ptr_c -> database_drive, ptr_c -> database_directory, ptr_c -> database_name, ptr_c -> database_extension ) ;

  strcpy ( ptr_c -> database_drive                      ,     m_database_drive                    ) ;
  strcpy ( ptr_c -> database_directory                  ,     m_database_directory                ) ;
  strcpy ( ptr_c -> database_name                       ,     m_database_name                     ) ;
  strcpy ( ptr_c -> database_extension                  ,     m_database_extension                ) ;
 
  m_database_drive                            =  ptr_c -> database_drive                      ;
  m_database_directory                        =  ptr_c -> database_directory                  ;
  m_database_name                             =  ptr_c -> database_name                       ;
  m_database_extension                        =  ptr_c -> database_extension                  ;

  m_database_path_and_file_name               =  ptr_g -> database_path_and_file_name         ;

  UpdateData ( FALSE ) ;

}

/**************************************************************************/
/**************************************************************************/

// The following are Dialog Box crap

void configure_dlg::OnCancel() 
{
	// TODO: Add extra cleanup here


  if ( exit_allowed_flag == 1 ) {
	CDialog::OnCancel();
  }
}

void configure_dlg::OnButtonErniesCancel() 
{
	// TODO: Add your control notification handler code here
	exit_allowed_flag = 1 ;
	CDialog::OnCancel() ;

}


/**************************************************************************/
/**************************************************************************/

void configure_dlg::OnButtonAdvanced() 
{
	// TODO: Add your control notification handler code here
	
    CDialogAdvancedSetup advanced_configure_dialog_box ;
    advanced_configure_dialog_box . DoModal () ;

}

/**************************************************************************/
/**************************************************************************/
