
/************************************************************************************************************/
/************************************************************************************************************/

// timer_appDlg.cpp : implementation file
//

/************************************************************************************************************/
/************************************************************************************************************/

#if ( 0 ) 

  ODBC Driver:

   changed page timeout from 5 on 9/11/01
  
  Get rid of user strings

#endif

/************************************************************************************************************/
/************************************************************************************************************/

#include "stdafx.h"

#include "xc_timer_dll.h"
#include "xc_timer_dll_functions.h"

#include "timer_app.h"

#include "timer_appDlg.h"
#include "configure_dlg.h"
#include "synch_dlg.h"
#include "event_and_heat_dlg.h"

#include "cdef.h"
#include "main.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/************************************************************************************************************/

#define UPDATE_VARIABLES_FROM_GUI   TRUE
#define UPDATE_GUI_FROM_VARIABLES   FALSE

/************************************************************************************************************/
/************************************************************************************************************/

static CWinApp * ptr_to_my_app ;

static BOOL  initialization_completed_flag   = 0 ; 

static long  application ;

static BOOL  interrupt_in_progress_flag      = 0 ;
static BOOL  linked_to_database_flag         = 0 ;
static BOOL  communicating_with_timers_flag  = 0 ;

static int exit_allowed_flag ;
static int module_character ;

XC_CONFIGURE_STRUCTURE_TYPE       * ptr_c ;
GLOBAL_VARIABLE_STRUCTURE_TYPE  * ptr_g ;


static BOOL db_opened_flag        = 0 ;
static BOOL reset_disabled_flag   = 0 ;

static char command_line_string   [ 200 ] ;

/************************************************************************************************************/
/************************************************************************************************************/

/////////////////////////////////////////////////////////////////////////////

// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
    CAboutDlg();

// Dialog Data
    //{{AFX_DATA(CAboutDlg)
    enum { IDD = IDD_ABOUTBOX };
    //}}AFX_DATA

    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CAboutDlg)
    protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

// Implementation
protected:
    //{{AFX_MSG(CAboutDlg)
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
    //{{AFX_DATA_INIT(CAboutDlg)
    //}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CAboutDlg)
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
    //{{AFX_MSG_MAP(CAboutDlg)
        // No message handlers
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTimer_appDlg dialog

CTimer_appDlg::CTimer_appDlg(CWnd* pParent /*=NULL*/)
    : CDialog(CTimer_appDlg::IDD, pParent)
{
    //{{AFX_DATA_INIT(CTimer_appDlg)
    m_time_of_day_string             = _T("");
    m_field_description_string       = _T("");
    m_terminal_text_box_string       = _T("");
    m_database_text_box_string       = _T("");
    m_version_string                 = _T("");
    m_heat_number                    = 0;
    m_event_number                   = 0;
    m_communicate_with_timers_flag   = FALSE;
    m_link_to_database_flag        = FALSE;
    m_link_to_database_flag          = FALSE;
    m_link_status_string = _T("");
    m_comm_status_string = _T("");
    //}}AFX_DATA_INIT

    // Note that LoadIcon does not require a subsequent DestroyIcon in Win32

    m_hIcon = AfxGetApp () -> LoadIcon ( IDR_MAINFRAME ) ;
}

void CTimer_appDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);

    //{{AFX_DATA_MAP(CTimer_appDlg)
    DDX_Text  ( pDX, IDC_EDIT_TIME_OF_DAY_STRING,        m_time_of_day_string);
    DDX_Text  ( pDX, IDC_EDIT_TEXT_FIELD_DESCRIPTION,    m_field_description_string);
    DDX_Text  ( pDX, IDC_EDIT_TERMINAL_TEXT_BOX,         m_terminal_text_box_string);
    DDX_Text  ( pDX, IDC_EDIT_DATABASE_TEXT_BOX,         m_database_text_box_string);
    DDX_Text  ( pDX, IDC_EDIT_VERSION,                   m_version_string);
    DDX_Text  ( pDX, IDC_EDIT_HEAT,                      m_heat_number);
    DDX_Text  ( pDX, IDC_EDIT_EVENT,                     m_event_number);
    DDX_Check ( pDX, IDC_CHECK_COMMUNICATE_WITH_TIMERS,  m_communicate_with_timers_flag);
    DDX_Check ( pDX, IDC_CHECK_LINK_TO_DATABASE,         m_link_to_database_flag);
    DDX_Text(pDX, IDC_EDIT_LINK_STATUS, m_link_status_string);
    DDX_Text(pDX, IDC_EDIT_COMM_STATUS, m_comm_status_string);
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CTimer_appDlg, CDialog)

    //{{AFX_MSG_MAP(CTimer_appDlg)
    ON_WM_DESTROY()
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    ON_WM_TIMER()
    ON_BN_CLICKED   ( IDC_BUTTON_RESET_TIMERS,                 OnButtonResetTimers)
    ON_BN_CLICKED   ( IDC_BUTTON_SYNCH_TIME,                   OnButtonSynchTime)
    ON_BN_CLICKED   ( IDC_BUTTON_CONFIGURE,                    OnButtonConfigure)
    ON_BN_CLICKED   ( IDC_BUTTON_RE_TRANSMIT,                  OnButtonReTransmit)
    ON_BN_CLICKED   ( IDC_BUTTON_EXIT,                         OnButtonExit)
    ON_BN_CLICKED   ( ID_HELP,                                 OnHelp)
    ON_BN_CLICKED   ( IDC_BUTTON_TEST,                         OnButtonTest)
    ON_BN_CLICKED   ( IDC_BUTTON_GENERATE_DUMMY_RECORD,        OnButtonGenerateDummyRecord)
    ON_BN_CLICKED   ( IDC_BUTTON_RESET_X,                      OnButtonResetX)
    ON_BN_CLICKED   ( IDC_BUTTON_SEND_EVENT_AND_HEAT_BUTTON,   OnButtonSendEventAndHeatButton)
    ON_BN_CLICKED   ( IDC_CHECK_COMMUNICATE_WITH_TIMERS,       OnCheckCommunicateWithTimers)
    ON_BN_CLICKED   ( IDC_CHECK_LINK_TO_DATABASE,              OnCheckLinkToDatabase)
    ON_BN_CLICKED   ( IDC_BUTTON_DELETE_RECORDS,               OnButtonDeleteRecords)
    ON_BN_CLICKED   ( IDC_BUTTON_DELETE_BACKUP_RECORDS,        OnButtonDeleteBackupRecords)
    //}}AFX_MSG_MAP

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTimer_appDlg message handlers

BOOL CTimer_appDlg::OnInitDialog()
{

    CDialog::OnInitDialog();

    // Add "About..." menu item to system menu.

    // IDM_ABOUTBOX must be in the system command range.
    ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
    ASSERT(IDM_ABOUTBOX < 0xF000);

    CMenu* pSysMenu = GetSystemMenu(FALSE);
    if (pSysMenu != NULL)
    {
        CString strAboutMenu;
        strAboutMenu.LoadString(IDS_ABOUTBOX);
        if (!strAboutMenu.IsEmpty())
        {
            pSysMenu->AppendMenu(MF_SEPARATOR);
            pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
        }
    }

    // Set the icon for this dialog.  The framework does this automatically
    //  when the application's main window is not a dialog
    SetIcon(m_hIcon, TRUE);         // Set big icon
    SetIcon(m_hIcon, FALSE);        // Set small icon
    
    // TODO: Add extra initialization here
 
    ptr_to_my_app = AfxGetApp () ;

    strcpy ( command_line_string, ptr_to_my_app -> m_lpCmdLine ) ;

    if ( strlen ( command_line_string ) > 0 ) {
      if ( SetCurrentDirectory ( command_line_string ) == 0 ) {  // new directory name
        MessageBox( "Could not Set Current Directory.", "Warning", ( MB_OK ) ) ;
      endif
    endif

    initialization_completed_flag = 0 ;
    
    m_TimerId = SetTimer ( 1, 50, NULL ) ;   // OnTimer Event 50 msec

    return TRUE;  // return TRUE  unless you set the focus to a control
}


void CTimer_appDlg::OnDestroy()
{
    WinHelp(0L, HELP_QUIT);
    CDialog::OnDestroy();
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CTimer_appDlg::OnPaint() 
{
    if (IsIconic())
    {
        CPaintDC dc(this); // device context for painting

        SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

        // Center icon in client rectangle
        int cxIcon = GetSystemMetrics(SM_CXICON);
        int cyIcon = GetSystemMetrics(SM_CYICON);
        CRect rect;
        GetClientRect(&rect);
        int x = (rect.Width() - cxIcon + 1) / 2;
        int y = (rect.Height() - cyIcon + 1) / 2;

        // Draw the icon
        dc.DrawIcon(x, y, m_hIcon);
    }
    else
    {
        CDialog::OnPaint();
    }
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CTimer_appDlg::OnQueryDragIcon()
{
    return (HCURSOR) m_hIcon;
}

/*********************************************************************/
/*********************************************************************/

// The following are Dialog Box crap

void CTimer_appDlg::OnCancel() 
{

  // TODO: Add your message handler code here
  exit_function () ;

}

/*********************************************************************/
/*********************************************************************/

// Button hits and keyboard hits are passed to state logic, to insure that critical operations are finished 
//        before a new operation is begun.  In other words, we want to make sure that a packet is totally transmitted
//        before a new packet is sent.

enum {
  ZERO_CHAR                      ,     // This is ignored
  SYNCH_CHAR                     ,     
  RESET_CHAR                     ,
  RESETX_CHAR                    ,
  EXIT_CHAR                      ,
  CONFIG_CHAR                    ,
  RETRANSMIT_CHAR                ,
  LINK_TO_DATABASE_CHAR          ,
  COMMUNICATE_WITH_TIMERS_CHAR   ,
  TEST_CHAR                      , 
  SEND_EVENT_CHAR                ,
  DELETE_RECORDS_CHAR            ,
  DELETE_BACKUP_RECORDS_CHAR     ,
  GENERATE_DUMMY_RECORD_CHAR     ,
  SEND_EVENT_AND_HEAT_CHAR       ,
} ;



void CTimer_appDlg::OnButtonResetTimers() 
{
    module_character = RESET_CHAR ;
}

void CTimer_appDlg::OnButtonSynchTime() 
{
    module_character = SYNCH_CHAR ;    
}

void CTimer_appDlg::OnButtonConfigure() 
{
    module_character = CONFIG_CHAR ;      
}

void CTimer_appDlg::OnButtonExit() 
{
    module_character = EXIT_CHAR ;
}

void CTimer_appDlg::OnButtonGenerateDummyRecord() 
{
     module_character = GENERATE_DUMMY_RECORD_CHAR ;
}

void CTimer_appDlg::OnButtonTest() 
{
    module_character = TEST_CHAR ;
}

void CTimer_appDlg::OnButtonDeleteRecords() 
{
    module_character = DELETE_RECORDS_CHAR ;
}

void CTimer_appDlg::OnButtonDeleteBackupRecords() 
{
    module_character = DELETE_BACKUP_RECORDS_CHAR ;
}

/*********************************************************************/

void CTimer_appDlg::OnHelp() 

{
    
}

/*********************************************************************/

void CTimer_appDlg::OnCheckCommunicateWithTimers() 

{
  module_character = COMMUNICATE_WITH_TIMERS_CHAR ;
    
}

/*********************************************************************/

void CTimer_appDlg::OnCheckLinkToDatabase() 
{
  module_character = LINK_TO_DATABASE_CHAR ;
}

/*********************************************************************/

void CTimer_appDlg::OnButtonSendEventAndHeatButton() 

{

  module_character = SEND_EVENT_AND_HEAT_CHAR ;
    
}

/*********************************************************************/

void CTimer_appDlg::OnButtonReTransmit() 
{
  module_character = RETRANSMIT_CHAR ;    
}

/*********************************************************************/

void CTimer_appDlg::OnButtonResetX() 

{
  module_character = RESETX_CHAR ;   
}

/*********************************************************************/
/*********************************************************************/
/*********************************************************************/
/*********************************************************************/

void CTimer_appDlg :: dlg_force_terminal_text_box_update ( char * terminal_string ) 

{
  
  int     read_index ;
  CString xscreen_string ;
  char *  ptr_string, * ptr_to_beginning_of_this_line_string ;
  BOOL    done_flag ;
  char    character ;

  static BOOL wrap_flag, partial_line_flag ;
  static char screen_string_array [ MAX_NUMBER_OF_TERMINAL_TEXT_BOX_LINES ][ 100 ] ;
  static int  write_index ;

  static char all_received_characters_string [ 5000 ] ;

  if ( terminal_string != NULL ) {

    strcat ( all_received_characters_string, terminal_string ) ;

// First, we add all lines to screen_string_array

    ptr_string                           = all_received_characters_string ;

    ptr_to_beginning_of_this_line_string = ptr_string ;

    done_flag = 0 ;

    while ( done_flag == 0 ) {

      character = * ptr_string ;            // Here we find first CR or LF (we treat \r like any other ASCII Character)

      if ( character == '\n' ) {    

        partial_line_flag = 0 ;

        * ptr_string = '\0' ;

        strcpy ( screen_string_array [ write_index ], ptr_to_beginning_of_this_line_string ) ;

        if ( ++ write_index >= MAX_NUMBER_OF_TERMINAL_TEXT_BOX_LINES ) {
          write_index = 0 ;
          wrap_flag   = 1 ;
        endif

        ptr_to_beginning_of_this_line_string = ++ ptr_string ;

      endif

//// If we have found the end of the string, we are done

      else if ( character == '\0' ) {   // Is it  '\0'
        if ( ptr_to_beginning_of_this_line_string != all_received_characters_string ) {
          strcpy ( all_received_characters_string, ptr_to_beginning_of_this_line_string ) ;
        endif
        done_flag = 1 ;
      endif

      else {
        partial_line_flag = 1 ;
        ptr_string ++ ;
      endif

    wend

  //**********/

//// Next, we display lines
    
    if ( wrap_flag == 0 ) {
      read_index = 0 ;
    endif
    else {
      read_index = write_index ;
    endif

    xscreen_string = "" ;

    if ( ( write_index != 0 ) || ( wrap_flag != 0 ) ) {
      do {
        xscreen_string += screen_string_array [ read_index ] ;    
        xscreen_string += "\n" ;    
        if ( ++ read_index >= MAX_NUMBER_OF_TERMINAL_TEXT_BOX_LINES ) read_index = 0 ;
       }  while ( read_index != write_index ) ;
    endif

    if ( partial_line_flag != 0 ) {
      xscreen_string += all_received_characters_string ;    
    endif

  endif

  else {

    wrap_flag = 0 ;
    partial_line_flag = 0 ;
    write_index = 0 ;

    all_received_characters_string [ 0 ] = '\0' ;

  endif

//UpdateData ( UPDATE_VARIABLES_FROM_GUI ) ;

  m_terminal_text_box_string = xscreen_string ;

  UpdateData ( UPDATE_GUI_FROM_VARIABLES) ;

}
    
/*********************************************************************/
/*********************************************************************/

void CTimer_appDlg :: dlg_force_database_text_box_update ( XC_TIMER_RECORD_STRUCTURE_TYPE * d ) 

{

  
  int     read_index ;
  CString xscreen_string ;

  static BOOL wrap_flag ;
  static char screen_string_array [ MAX_NUMBER_OF_DATABASE_TEXT_BOX_LINES ][ 100 ] ;
  static int  write_index ;

  if ( d == NULL ) {
    wrap_flag   = 0 ;
    write_index = 0 ;
    screen_string_array [ 0 ] [ 0 ] = '\0' ;
  endif

  else {


// Biggest time is -99:59:59.999 or 13 digits

//  sprintf ( scratch_string, "|%3s|%6s|%5s|%2s|%4s|%16s |%16s | %12s |", 
//                             "Dev", "Record", "Event", "Ch", "Type", "User String   ", "Bib String   ", "Time    " ) ;

    char time_string [ 30 ] ;
    char * ptr_to_colon ;    
    int number_of_spaces = 0 ;

    strcpy ( time_string, "       " ) ;

    ptr_to_colon = strchr ( d -> timer_time, ':' ) ;
    if ( ptr_to_colon != NULL ) {
      number_of_spaces = 3 - ( ptr_to_colon - d -> timer_time ) ;
    endif

    strcpy ( time_string + number_of_spaces, d -> timer_time ) ;  
 
    sprintf ( screen_string_array [ write_index ], "|%2d |%5d | %3d |%3d | %1d|  %1s |%16s |%16s |%-13s |", 
                 d -> device_num                ,
                 d -> record_num                ,
                 d -> event_num                 ,
                 d -> heat_num                  ,
                 d -> channel                   ,
                 d -> record_typ                ,
                 d -> userstring                ,
                 d -> bib_string                ,
                 time_string                        ) ;

  endif

  if ( ++ write_index >= MAX_NUMBER_OF_DATABASE_TEXT_BOX_LINES ) {
    write_index = 0 ;
    wrap_flag = 1 ;
  endif

  if ( wrap_flag == 0 ) {
    read_index = 0 ;
  endif
  else {
    read_index = write_index ;
  endif

  xscreen_string = "" ;

  do {
    xscreen_string += screen_string_array [ read_index ]  ;    
    xscreen_string += "\r\n" ;    
    if ( ++ read_index >= MAX_NUMBER_OF_DATABASE_TEXT_BOX_LINES ) read_index = 0 ;
  } while ( read_index != write_index ) ;

//UpdateData ( UPDATE_VARIABLES_FROM_GUI ) ;

  m_database_text_box_string = xscreen_string ;

  UpdateData ( UPDATE_GUI_FROM_VARIABLES) ;

}
    
/*********************************************************************/
/*********************************************************************/

// The following is the Dialog Box Timer Event.  It is called every 50 msec

/*********************************************************************/
/*********************************************************************/

void CTimer_appDlg::OnTimer ( UINT nIDEvent ) 

{
    // TODO: Add your message handler code here and/or call default
    
  BOOL                  done_flag ;
  long                  continue_flag ;
  char                  scratch_string  [  400 ] ;
  char                  terminal_string [ 5000 ] ;

  char *                terminal_string_ptr ;
  long                  character ;
  int                   terminal_string_length ;
  int                   number_of_records_acquired ;

  XC_TIMER_RECORD_STRUCTURE_TYPE  d ;

  static BOOL           initialization_in_progress_flag ;

  static unsigned short time_counts   ;
  static unsigned short timeout_counter  ;

/****************************************************************/

// Here we initialize the dialog box stuff during the first call
 
//if ( ( interrupt_in_progress_flag == 0 ) || ( ++ timeout_counter > 50 ) ) {

  if ( interrupt_in_progress_flag == 0 ) {

    timeout_counter = 0 ;

    interrupt_in_progress_flag = 1 ;

    if ( initialization_completed_flag == 0 ) {

      long initialize_flag ;

      initialization_in_progress_flag = 1 ;

      initialization_completed_flag = 1 ;

      ptr_c   = dll_get_pointer_to_configuration_structure  () ; 

      ptr_g   = dll_get_pointer_to_global_variable_structure () ;

      application     = 0 ;

      initialize_flag = 0x4000 | 0x100 | 0x40 | 0x10 | 2 ;       // 

      dll_initialize_dll_task ( & initialize_flag, command_line_string ) ;

      dll_set_string_delimiter  ( NULL ) ;
  
      sprintf ( scratch_string, "|%3s|%6s|%5s|%4s|%2s|%4s|%16s |%16s | %12s |", 
                                "Dev", "Record", "Event", "Heat", "Ch", "Type", "User String   ", "Bib String   ", "Time    " ) ;


//    UpdateData ( UPDATE_VARIABLES_FROM_GUI ) ;

      m_field_description_string. Format ( "%s",  scratch_string ) ;

      sprintf ( scratch_string, "Version %s at %s on %s", ptr_g -> version_number_string, ptr_g -> version_time_string, ptr_g -> version_date_string ) ;

      m_version_string = scratch_string ;

      UpdateData ( UPDATE_GUI_FROM_VARIABLES) ;

      initialization_in_progress_flag = 0 ;

    endif

/**************/

    if ( initialization_in_progress_flag == 0 ) {

// Here we update the Time of Day

      if ( ( ( time_counts ++ ) % 10 ) == 0 ) {

//      UpdateData ( UPDATE_VARIABLES_FROM_GUI ) ;
        m_time_of_day_string. Format ( "%s", _strtime ( scratch_string ) ) ;
        UpdateData ( UPDATE_GUI_FROM_VARIABLES) ;  

      endif

/**************/

      terminal_string_ptr = terminal_string  ;

      terminal_string_length = 0 ;

      done_flag = 0 ;

      while ( done_flag == 0 ) {

        character = dll_get_character_from_terminal_fifo () ;

        if ( character == -1 ) {
          done_flag = 1 ;
          * terminal_string_ptr = '\0' ;
        endif

        else {

          * terminal_string_ptr ++ = (char) character ;

		  if ( ++ terminal_string_length > 4000 ) {

            done_flag = 1;
			
	      endif

        endif

      wend

      if ( terminal_string [ 0 ] != '\0' ) {

        dlg_force_terminal_text_box_update ( terminal_string ) ;

      endif

/*********************************/

// Here, we append data records to database

      number_of_records_acquired = 0 ;

      continue_flag = 1 ;

      while ( continue_flag != 0 ) {

        continue_flag = dll_get_next_timer_structure ( & application, & d ) ;

        if ( continue_flag != 0 ) {

          dlg_force_database_text_box_update ( & d ) ;

// have delete records delete text file delete fifo and purge the text file

          if ( linked_to_database_flag == TRUE ) {

            if ( db_opened_flag != 0 ) {
              odbc_sql_append_database_record   ( & d ) ;
			endif

          endif 
          
          if ( ++ number_of_records_acquired > 25 ) {
            continue_flag = 0 ;
          endif
 
        endif

      wend

/*********************************/
/*********************************/

      if ( module_character != 0 ) {
      
        int tmp_module_character ;

        tmp_module_character = module_character ;  // This prevents reentrant calls
        module_character = 0 ;

        if ( tmp_module_character == CONFIG_CHAR ) {
          if ( ( linked_to_database_flag == 0 ) && ( communicating_with_timers_flag == 0 ) ) {
            configure_dlg configure_dialog_box ;
            configure_dialog_box . DoModal () ;
          endif
          else {
            MessageBox( "Must unlink database and stop communications before configuring.", "Warning", ( MB_OK ) ) ;
          endif
        endif

        else if ( tmp_module_character == SYNCH_CHAR ) {
          if ( MessageBox ( "Are You SURE that You Want to Re-Synch the Timers?", "Are You Sure", ( MB_YESNO | MB_ICONWARNING | MB_DEFBUTTON2  ) ) == IDYES ) {
            synch_dlg synch_dialog_box ;
            synch_dialog_box . DoModal () ;
          endif
        endif

        else if ( tmp_module_character == RESET_CHAR ) {
          if ( reset_disabled_flag == 0 ) {
            if ( MessageBox ( "Are You SURE that You Want to RESET the Timers?", "Are You Sure", ( MB_YESNO | MB_ICONWARNING | MB_DEFBUTTON2  ) ) == IDYES ) {
              dll_reset_timers () ;
              dlg_force_terminal_text_box_update ( NULL ) ;
              dlg_force_database_text_box_update ( NULL ) ;
            endif
          endif
          else {
            MessageBox( "Reset was Disabled", "Error", ( MB_OK ) ) ;
          endif
        endif

        else if ( tmp_module_character == RESETX_CHAR ) {
          if ( MessageBox ( "Are You SURE that You Want to Disable RESET ?", "Are You Sure", ( MB_YESNO | MB_ICONWARNING | MB_DEFBUTTON2  ) ) == IDYES ) {
            dll_disable_timer_reset () ;
            reset_disabled_flag   = 1 ;
          endif
        endif       

        else if ( tmp_module_character == RETRANSMIT_CHAR ) {
 
          if ( MessageBox ( "Are You SURE that You Want Timers to Re-Transmit All Data? (This may cause redundant database records)", "Are You Sure", ( MB_YESNO | MB_ICONWARNING | MB_DEFBUTTON2  ) ) == IDYES ) {

            long retransmit_type = 2 ;

            dll_retransmit_data ( NULL ) ;

            if ( MessageBox ( "Do you want to delete all data in the database file before retransmitting? Are You SURE?!!", "Are You Sure", ( MB_YESNO | MB_ICONWARNING | MB_DEFBUTTON2  ) ) == IDYES ) {
              delete_database_records ()  ;
            endif

            dll_retransmit_data ( & retransmit_type ) ;

          endif
        endif
 
        else if ( tmp_module_character == LINK_TO_DATABASE_CHAR ) {

          if ( linked_to_database_flag == 0 ) {

            linked_to_database_flag  = 1 ;

            if ( strcmpi ( ptr_c -> database_type, "DLL" ) == 0 ) {
              db_opened_flag          = 0 ;
              m_link_status_string = "DLL Link Open" ;
            endif

            else if ( odbc_open_database_connection ( ptr_c, ptr_g ) != 0 ) {
              db_opened_flag          = 1 ;
              m_link_status_string = ptr_g -> database_path_and_file_name ;
            endif
            else {
              db_opened_flag          = 0 ;
              linked_to_database_flag = 0 ;
              m_link_status_string = "Link Failed" ;
            endif
                
          endif

          else {
  
            linked_to_database_flag    = 0 ;

            if ( db_opened_flag != 0 ) {   
              odbc_close_database_connection () ;     
              m_link_status_string = "Unlinked" ;
            endif

            db_opened_flag             = 0 ;

          endif

//        UpdateData ( UPDATE_VARIABLES_FROM_GUI ) ;
          m_link_to_database_flag = linked_to_database_flag ;
          UpdateData ( UPDATE_GUI_FROM_VARIABLES) ;

        endif
  
        else if ( tmp_module_character == COMMUNICATE_WITH_TIMERS_CHAR ) {

          BOOL tmp_communicating_with_timers_flag ;

          UpdateData ( UPDATE_VARIABLES_FROM_GUI ) ;
          tmp_communicating_with_timers_flag = m_communicate_with_timers_flag ;
//        UpdateData ( UPDATE_GUI_FROM_VARIABLES) ;

          if ( tmp_communicating_with_timers_flag != 0 ) {
            if ( dll_start_communicating_with_timers () != 0 ) {
              #if ( RELEASE_DATE >= 0x090604 )
                m_comm_status_string.Format ( "Opened Communications on COMM%d at %5d baud", ptr_c -> serial_comm_port, ptr_c -> baud_rate ) ;
              #endif
              #if ( RELEASE_DATE <  0x090604 )
                m_comm_status_string.Format ( "Communicating on COMM%1d at %5d baud", ptr_c -> serial_comm_port, ptr_c -> baud_rate ) ;
              #endif
              communicating_with_timers_flag = 1 ;
            endif
            else {
              #if ( RELEASE_DATE >= 0x090604 )
                m_comm_status_string = "Failed to Open Communications" ;
              #endif
              #if ( RELEASE_DATE <  0x090604 )
                m_comm_status_string = "Failed to Start Communicating" ;
              #endif
            endif
          endif
          else {
            dll_stop_communicating_with_timers () ;
            communicating_with_timers_flag = 0 ;

            #if ( RELEASE_DATE >= 0x090604 )
              m_comm_status_string = "Closed Communications" ;
            #endif
            #if ( RELEASE_DATE <  0x090604 )
              m_comm_status_string = "Stopped Communicating" ;
            #endif

          endif

//        UpdateData ( UPDATE_VARIABLES_FROM_GUI ) ;
          m_communicate_with_timers_flag = communicating_with_timers_flag ;
          UpdateData ( UPDATE_GUI_FROM_VARIABLES) ;

        endif


        else if ( tmp_module_character == TEST_CHAR ) {

//        dll_test_time_routines () ;

        endif

        else if ( tmp_module_character == GENERATE_DUMMY_RECORD_CHAR ) {
          dll_generate_dummy_record () ;
        endif

        else if ( tmp_module_character == DELETE_RECORDS_CHAR ) {

          if ( MessageBox ( "Do you want to delete all data in the database file? Are You SURE?  This could be really embarassing!!", "Are You Sure", ( MB_YESNO | MB_ICONWARNING | MB_DEFBUTTON2  ) ) == IDYES ) {

            delete_database_records ()  ;

          endif

        endif

        else if ( tmp_module_character == DELETE_BACKUP_RECORDS_CHAR ) {

          if ( MessageBox ( "Do you want to delete all data in the backup text file?  This backup file is there to protect you!!", "Are You Sure", ( MB_YESNO | MB_ICONWARNING | MB_DEFBUTTON2  ) ) == IDYES ) {
            dll_delete_records_in_backup_text_file () ;
            dlg_force_terminal_text_box_update ( NULL ) ;
            dlg_force_database_text_box_update ( NULL ) ;
          endif

        endif
 
        else if ( tmp_module_character == SEND_EVENT_AND_HEAT_CHAR ) {
          c_event_and_heat_dlg event_and_heat_dialog_box ;
          event_and_heat_dialog_box . DoModal () ;

//        UpdateData ( UPDATE_VARIABLES_FROM_GUI ) ;
          m_event_number   =  ptr_c -> event_number  ;
          m_heat_number    =  ptr_c -> heat_number   ;
          UpdateData ( UPDATE_GUI_FROM_VARIABLES) ;

        endif

        else if ( tmp_module_character == EXIT_CHAR ) {
          exit_function () ;
        endif
  
      endif                                 // Key hit

 /*******************************/
/*******************************/

    endif                                   // Initialization in progress

    interrupt_in_progress_flag = 0 ;

  endif                                     // Interrupt in progress

  CDialog::OnTimer(nIDEvent);

}

/*********************************************************************/
/*********************************************************************/

void CTimer_appDlg::exit_function ( void )

{

  if ( ( linked_to_database_flag == 0 ) && ( communicating_with_timers_flag == 0 ) ) {

    if ( MessageBox ( "Are You SURE that You Want to Exit the Application?", "Are You Sure", ( MB_YESNO | MB_ICONWARNING | MB_DEFBUTTON2  ) ) == IDYES ) {

      exit_allowed_flag = 1 ;
      if ( db_opened_flag != 0 ) {   
        db_opened_flag = 0 ;
        odbc_close_database_connection () ;     
      endif
      if ( communicating_with_timers_flag != 0 ) {   
        communicating_with_timers_flag = 0 ;
        dll_stop_communicating_with_timers () ;
      endif

      dll_exit_routine (  "" ) ;

//    CDialog::OnOK();
      CDialog::OnCancel();
    endif
    interrupt_in_progress_flag = 0 ;

  endif

  else {
    MessageBox( "Must unlink database and stop communications before exiting.", "Warning", ( MB_OK ) ) ;
  endif

}

/****************************************************************************************************/
/****************************************************************************************************/

void CTimer_appDlg::delete_database_records ( void )

{

  if ( linked_to_database_flag != 0 ) {
  
    dll_delete_dll_fifo_records () ;

    if ( db_opened_flag != 0 ) {

      odbc_delete_all_database_records () ;

      if ( strcmpi ( ptr_c -> database_extension, ".XLS" ) == 0 ) {

        MessageBox ( "You must manually delete the records in the Excel Spreadsheet", "Message Box", ( MB_ICONERROR | MB_OK ) ) ;

      endif

    endif

    dlg_force_terminal_text_box_update ( NULL ) ;
    dlg_force_database_text_box_update ( NULL ) ;

  endif

  else {

    MessageBox( "Must Link to Database First", "Error", ( MB_ICONERROR | MB_OK ) ) ;

  endif

}

/****************************************************************************************************/
/****************************************************************************************************/
