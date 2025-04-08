// flash_dlg.cpp : implementation file
//

#include "stdafx.h"

#include "xc_dll.h"

#include "timer_app.h"
#include "flash_dlg.h"

#include "cdef.h"
#include "main.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static BOOL  initialization_completed_flag = 0 ; 

/////////////////////////////////////////////////////////////////////////////
// flash_dlg dialog


flash_dlg::flash_dlg(CWnd* pParent /*=NULL*/)
    : CDialog(flash_dlg::IDD, pParent)
{
    //{{AFX_DATA_INIT(flash_dlg)
	m_time_of_day_string = _T("");
	m_terminal_text_box_string = _T("");
	m_status_string = _T("");
	m_percent_done = 0;
	//}}AFX_DATA_INIT
}


void flash_dlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(flash_dlg)
	DDX_Text(pDX, IDC_EDIT_TIME_OF_DAY_STRING, m_time_of_day_string);
	DDX_Text(pDX, IDC_EDIT_TERMINAL_TEXT_BOX, m_terminal_text_box_string);
	DDX_Text(pDX, IDC_EDIT_STATUS, m_status_string);
	DDX_Text(pDX, IDC_EDIT_PERCENT_DONE, m_percent_done);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(flash_dlg, CDialog)
    //{{AFX_MSG_MAP(flash_dlg)
    ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////

// flash_dlg message handlers

/*****************************************************************************************/


BOOL flash_dlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
  initialization_completed_flag = 0 ; 

  m_TimerId = SetTimer ( 1, 50, NULL ) ;   // OnTimer Event 50 msec

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

/*****************************************************************************************/
/*****************************************************************************************/

void flash_dlg :: dlg_force_terminal_text_box_update ( char * terminal_string ) 

{
  
  int     read_index ;
  CString xscreen_string ;
  char *  ptr_string, * ptr_to_beginning_of_this_line_string ;
  BOOL    done_flag ;
  char    character ;

  static BOOL wrap_flag, partial_line_flag ;
  static char screen_string_array [ MAX_NUMBER_OF_TERMINAL_TEXT_BOX_LINES ][ 100 ] ;
  static int  write_index ;

  static char all_received_characters_string [ 400 ] ;

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

// If we have found the end of the string, we are done

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

 /**********/

// Next, we display lines
  
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

  UpdateData ( TRUE ) ;

  m_terminal_text_box_string = xscreen_string ;

  UpdateData ( FALSE ) ;

}
    
/*********************************************************************/


void flash_dlg::OnTimer(UINT nIDEvent) 
{
    // TODO: Add your message handler code here and/or call default

  char                  terminal_string [ 400 ] ;
  char                  status_string   [ 400 ] ;
  char                  scratch_string  [ 400 ] ;
  int                   percent_done ;
  int                   return_value ;

  static BOOL           done_flag ;

  static unsigned short time_counts ;

  terminal_string [ 0 ] = '\0' ;

  return_value = 0 ;

  if ( initialization_completed_flag == 0 ) {
    done_flag    = 0 ;
    percent_done = 0 ;
    transmit_generic_packet ( "FL l" ) ;
    flash_program_routine ( 1, terminal_string, status_string, & percent_done ) ;
    initialization_completed_flag = 1 ; 
  endif

  else if ( done_flag == 0 ) {

    return_value = flash_program_routine ( 0, terminal_string, status_string, & percent_done ) ;

    if ( return_value == 1 ) {
      done_flag = 1 ;
      Beep ( 1000, 1000 ) ;    // sound frequency,  sound duration
      MessageBox( "Flash Finished Programming", "Information", ( MB_ICONERROR | MB_OK ) ) ;
      initialization_completed_flag = 0 ; 
      CDialog::OnOK();
    endif

    if ( strlen ( terminal_string ) > 0 ) {
      dlg_force_terminal_text_box_update ( terminal_string ) ;
    endif

  endif

   if ( done_flag != 0 ) {
   endif

  if ( strlen ( status_string ) > 0 ) {
    UpdateData ( TRUE ) ;
    m_percent_done  = percent_done  ;
    m_status_string = status_string ;
    UpdateData ( FALSE ) ;  
  endif

/**************/

// Here we update the Time of Day

  if ( ( ( time_counts ++ ) % 10 ) == 0 ) {

    UpdateData ( TRUE ) ;
    m_percent_done = percent_done ;
    m_time_of_day_string. Format ( "%s", _strtime ( scratch_string ) ) ;
    UpdateData ( FALSE ) ;  

  endif
    
  CDialog::OnTimer(nIDEvent);

}

/*****************************************************************************************/
