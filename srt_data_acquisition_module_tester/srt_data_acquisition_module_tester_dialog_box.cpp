// srt_data_acquisition_module_tester_dialog_box.cpp : implementation file
//

#include "stdafx.h"
#include "srt_data_acquisition_module_tester.h"
#include "srt_data_acquisition_module_tester_dialog_box.h"
#include "tab_delimited_text_file_class.h"
#include "srt_data_acquisition_module_api.h"
#include "srt_data_acquisition_module_public_functions.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/********************************************************************************************/

#define endif }

#define UPDATE_VARIABLES_FROM_GUI   TRUE
#define UPDATE_GUI_FROM_VARIABLES   FALSE

#define NUMBER_OF_LINES_IN_TEXT_BOX  10

/********************************************************************************************/

/////////////////////////////////////////////////////////////////////////////
// class_data_acquisition_module_tester_dialog_box dialog

class_data_acquisition_module_tester_dialog_box::class_data_acquisition_module_tester_dialog_box(CWnd* pParent /*=NULL*/)
    : CDialog(class_data_acquisition_module_tester_dialog_box::IDD, pParent)
{
    //{{AFX_DATA_INIT(class_data_acquisition_module_tester_dialog_box)
	m_valid_records_field_string = _T("");
	//}}AFX_DATA_INIT
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void class_data_acquisition_module_tester_dialog_box::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(class_data_acquisition_module_tester_dialog_box)
	DDX_Text(pDX, IDC_EDIT_VALID_RECORDS_TEXT_BOX, m_valid_records_field_string);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(class_data_acquisition_module_tester_dialog_box, CDialog)
    //{{AFX_MSG_MAP(class_data_acquisition_module_tester_dialog_box)
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    ON_WM_TIMER()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// class_data_acquisition_module_tester_dialog_box message handlers

BOOL class_data_acquisition_module_tester_dialog_box::OnInitDialog()
{
    CDialog::OnInitDialog();

    SetIcon(m_hIcon, TRUE);         // Set big icon
    SetIcon(m_hIcon, FALSE);        // Set small icon
    
    // TODO: Add extra initialization here
    
    m_TimerId = SetTimer ( 1, 200, NULL ) ;   // OnTimer Event

    return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void class_data_acquisition_module_tester_dialog_box::OnPaint() 
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

HCURSOR class_data_acquisition_module_tester_dialog_box::OnQueryDragIcon()
{
    return (HCURSOR) m_hIcon;
}

/****************************************************************************************/
/****************************************************************************************/

void class_data_acquisition_module_tester_dialog_box::OnTimer ( UINT nIDEvent ) 

{

  static BOOL dialog_box_initialized_flag ;


  char  scratch_string [ 500 ] ;
  char  big_ass_string [ 5000 ] ;
  static char  string_array [ NUMBER_OF_LINES_IN_TEXT_BOX ][ 500 ] ;
  static char  header_string [ 500 ] ;
  static int write_index ;
  int   read_index ;
  static BOOL  wrap_flag  ; 
  char * ptr ;

  long   app               ;
  long   table_id          ;
  long   device_num        ;
  long   record_num        ;
  long   event_num         ;
  long   heat_num          ;
  long   channel           ;
  char   record_typ        [ 200 ] ;
  char   userstring        [ 200 ] ;
  char   user1_string      [ 200 ] ;
  char   user2_string      [ 200 ] ;
  char   user3_string      [ 200 ] ;
  char   user4_string      [ 200 ] ;
  char   bib_string        [ 200 ] ;
  char   timer_time        [ 200 ] ;
  char   pc_time           [ 200 ] ;
  char   notes             [ 200 ] ;



  if ( dialog_box_initialized_flag == 0 ) {

    dialog_box_initialized_flag = 1 ;

    sprintf ( header_string, "|%3s|%6s|%5s|%4s|%16s | %12s |\r\n", 
                             "Dev", "Record", "Ch", "Type", "User String   ", "Time" ) ;


    m_valid_records_field_string = header_string ;

    UpdateData ( UPDATE_GUI_FROM_VARIABLES ) ;  

  endif


  else {

    if ( dll_get_initialization_flag () != 0 ) {

      app = 3 ;

//    data_available = dll_get_next_timer_record (

             & app,
             & table_id,
             & device_num,
             & record_num,
             & event_num,
             & heat_num,
             & channel,
               record_typ,
               userstring,
               user1_string,
               user2_string,
               user3_string,
               user4_string,
               bib_string,
               timer_time,
               pc_time,
               notes   ) ;


      data_available = dll_get_next_timer_record_from_database (

             & app,
             & table_id,
             & device_num,
             & record_num,
             & event_num,
             & heat_num,
             & channel,
               record_typ,
               userstring,
               user1_string,
               user2_string,
               user3_string,
               user4_string,
               bib_string,
               timer_time,
               pc_time,
               notes   ) ;



        if ( data_available ) != 0 ) {


        if ( ( ptr = strchr ( record_typ, '!' ) ) != NULL ) * ptr = '\0' ;
        if ( ( ptr = strchr ( userstring, '!' ) ) != NULL ) * ptr = '\0' ;
        if ( ( ptr = strchr ( timer_time, '!' ) ) != NULL ) * ptr = '\0' ;


        sprintf ( scratch_string, "|%2d |%5d | %3d |  %1s |%-16s |%13s |\r\n", 
             (int) device_num                ,
             (int) record_num                ,
             (int) channel                   ,
                   record_typ                ,
                   userstring                ,
                   timer_time                      ) ;


        strcpy ( string_array [ write_index ], scratch_string ) ;

        if ( ++ write_index >= NUMBER_OF_LINES_IN_TEXT_BOX ) {
          write_index = 0 ;
          wrap_flag   = 1 ;
        endif

        if ( wrap_flag == 0 ) {
          read_index = 0 ;
        endif

        else {
          read_index = write_index - NUMBER_OF_LINES_IN_TEXT_BOX ;
          if ( read_index < 0 ) read_index += NUMBER_OF_LINES_IN_TEXT_BOX ;
        endif

        strcpy ( big_ass_string, header_string ) ;

        do {
          strcat ( big_ass_string, string_array [ read_index ] ) ;
          if ( ++ read_index >= NUMBER_OF_LINES_IN_TEXT_BOX ) read_index = 0 ;
        } while ( read_index != write_index ) ; 


        m_valid_records_field_string = big_ass_string ;

        UpdateData ( UPDATE_GUI_FROM_VARIABLES ) ;  
    
      endif

    endif

  endif

  CDialog::OnTimer(nIDEvent);

}

/****************************************************************************************/
/****************************************************************************************/
