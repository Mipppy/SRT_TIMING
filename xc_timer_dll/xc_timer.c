
/************************************************************************/

#if ( 0 ) 

Tasks:

  reset record counters to last value

  ask user whether to delete results.txt file

#endif

/************************************************************************/

#define MAIN_MODULE                         1
#define REGISTER_MODULE                     1
#define INTERNAL_XRAM_VARIABLES_MODULE      1
#define EXTERNAL_XRAM_VARIABLES_MODULE      1 

#define TERMINAL_FIFO_MAX_LENGTH         1000
                   
/************************************************************************/

#define XC_COMM_MODULE 1

#include <x_header.h>
#include <win_header.h>

#include <commport.h>
#include <errors.h>

/*********************************************************************/
/*********************************************************************/

void CALLBACK dll_timer_events ( unsigned int id, unsigned int msg, DWORD dwUser, DWORD dw1, DWORD dw2 ) ;/* function declaration */

XC_CONFIGURE_STRUCTURE_TYPE     c ;

GLOBAL_VARIABLE_STRUCTURE_TYPE  g ;

long time_of_last_reception_array [ MAX_NUMBER_OF_DEVICES ] ;

/***********************************************************************/
/***********************************************************************/

static   char          time_string                   [  100 ] ;
static   char          open_file_name                [ 1000 ] ;

static   char          terminal_fifo               [ TERMINAL_FIFO_MAX_LENGTH ] ;
static   int           terminal_fifo_write_index ;
static   int           terminal_fifo_read_index ;

unsigned char          comm_port_initialized_flag ;
static unsigned int    timer_events_enabled_flag ;
static unsigned int    timer_id ;

static int             sleep_counter ;
static int             max_sleep_counter ;
static int             token_cycle_counter ;
static int             device_number_with_token ;

/************************************************************************/
/************************************************************************/

// Here we read .INI variables, open LOG file, allocate buffers in memory, etc
//      and open COMM ports

long DLL_EXPORT __stdcall dll_initialize_dll_task ( long * read_ini_file_flag, char * ptr_to_race_directory ) /* function declaration */

{

  char scratch_string [ 200 ] ;
  int  tmp_read_ini_file_flag ;

/******************************************************************/

  if ( read_ini_file_flag == NULL ) {
    tmp_read_ini_file_flag = 0 ;
  endif
  else {
    tmp_read_ini_file_flag = (int) ( * read_ini_file_flag ) ;
  endif

/******************************************************************/
/******************************************************************/

  dll_assign_default_values () ;

/******************************************************************/

  strcat ( g.version_number_string,   DAQ_SOFTWARE_VERSION_NUMBER_STRING  ) ;
  strcat ( g.version_time_string,     DAQ_SOFTWARE_VERSION_TIME_STRING    ) ;
  strcat ( g.version_date_string,     DAQ_SOFTWARE_VERSION_DATE_STRING    ) ;

/******************************************************************/

  _getcwd ( g . current_working_directory, 500 ) ;

  if ( ( ptr_to_race_directory == NULL ) || ( ptr_to_race_directory [ 0 ] == '\0' ) ) {
    strcpy  ( g.race_directory, g.current_working_directory ) ;
  endif
  else {
    strcpy  ( g.race_directory, ptr_to_race_directory ) ;
  endif

/******************************************************************/

  get_file_path_and_name_from_parts ( g . log_file_path_and_name, g . race_directory, "srt_log", ".txt" ) ;

  if ( ( tmp_read_ini_file_flag & 0x10 ) == 0 ) {
    strcpy (  g . log_file_path_and_name, "" ) ;
  endif

/******************************************************************/

  output_formatted_file_string ( g . log_file_path_and_name     , NULL ) ;

  output_formatted_file_string ( g . log_file_path_and_name,  "SRT Data Acquisition Module Runtime Log Here\n" ) ;
  output_formatted_file_string ( g . log_file_path_and_name,  "  at %s",    _strtime ( time_string ) ) ;
  output_formatted_file_string ( g . log_file_path_and_name,  "  on %s:\n", _strdate ( scratch_string ) ) ;
  output_formatted_file_string ( g . log_file_path_and_name,  "  %s at %s on %s\n\n\n", DAQ_SOFTWARE_VERSION_NUMBER_STRING, DAQ_SOFTWARE_VERSION_TIME_STRING, DAQ_SOFTWARE_VERSION_DATE_STRING ) ;

/******************************************************************/

  get_file_path_and_name_from_parts ( open_file_name, g . race_directory, "open", ".txt" ) ;

  if ( ( tmp_read_ini_file_flag & 0x100 ) != 0 ) {

    if ( access ( open_file_name, 0 ) != 0 ) {    

      output_formatted_file_string ( open_file_name     , NULL ) ;
      output_formatted_file_string ( open_file_name     , "Data Acquisition Module Opened at %s on %s \n\n",  _strtime ( time_string ), _strdate ( scratch_string ) ) ;

    endif

    else {

      MessageBox ( GetFocus (), "It appears that the Data Acquisition Module is already Open.\n\nIf you have two instances of the Data Acquisition Module open at the same time, they should use different COMM Ports,", NULL, MB_OK ) ;

    endif

  endif

/******************************************************************/

  get_file_path_and_name_from_parts ( g.ini_file_path_and_name,    g . race_directory, "srt_data_acquisition",  ".ini" ) ;

  read_all_ini_parameters ( tmp_read_ini_file_flag, g.ini_file_path_and_name ) ;

/******************************************************************/
/******************************************************************/
 
  get_file_path_and_name_from_parts ( g.backup_file_path_and_name, g . race_directory, "srt_backup_timer_data",  ".txt"  ) ;

  #if ( RELEASE_DATE >= 0x090604 )
    if ( access ( g.backup_file_path_and_name, 0 ) == 0 ) {    // If backup file does exist, then it is used.
    endif

    else if ( ( tmp_read_ini_file_flag & 0x20 ) == 0 ) {
      strcpy (  g.backup_file_path_and_name, "" ) ;
    endif
  #endif

  #if ( RELEASE_DATE <  0x090604 )

    if ( ( tmp_read_ini_file_flag & 0x20 ) == 0 ) {
      strcpy (  g.backup_file_path_and_name, "" ) ;
    endif
  #endif

/********************************************************/

  comm_port_initialized_flag = 0 ;

  terminal_fifo_write_index  = 0 ;
  terminal_fifo_read_index   = 0 ;

  timer_events_enabled_flag  = 0 ;
  timer_id                   = 0 ;

  dll_delete_dll_fifo_records () ;

/*******************************************************/

  g.timer_event_interval            = 50 ;    // timer interval in millisecs
  g.timer_event_resolution          =  2 ;    // timer interval in millisecs

/*******************************************************/

  output_formatted_file_string ( g . log_file_path_and_name,  "SRT Data Acquisition Module Has Been Initialized\n\n" ) ;

  return 1 ;

}

/**************************************************************************/
/**************************************************************************/
/**************************************************************************/
/**************************************************************************/

long DLL_EXPORT __stdcall dll_exit_routine ( char * exit_string ) /* function declaration */

{

  dll_stop_communicating_with_timers () ;

  unlink ( "open.txt" ) ;

  if ( ( exit_string == NULL ) || ( exit_string [ 0 ] == '\0' ) ) {
    output_formatted_file_string ( g . log_file_path_and_name, "Normal Exit at %s \n", _strtime ( time_string ) ) ;
  endif

  else {
    output_formatted_file_string ( g . log_file_path_and_name, "Abnormal Termination from DLL\n" ) ;
    output_formatted_file_string ( g . log_file_path_and_name, "Exit String: %s at %s \n", exit_string, _strtime ( time_string ) ) ;
  endif

  output_formatted_file_string ( g . log_file_path_and_name, "Closing Runtime Log \n" ) ;
  exit ( 0 ) ;
  
}

/****************************************************************/
/****************************************************************/
/****************************************************************/

void debug ( void ) /* function declaration */

{               
}

/**************************************************************************/
/**************************************************************************/

void output_formatted_file_string ( char * file_path_and_name, char * format_string,... ) /* function declaration */

{

  char debug_string [ 512 ] ;

  FILE * output_file ;

  va_list marker ;                   // Declaration of typedef??

  if ( file_path_and_name [ 0 ] != '\0' ) {

    if ( format_string == NULL ) {

      if ( ( output_file = fopen ( file_path_and_name,  "w" ) ) == NULL ) {
        sprintf ( debug_string, "Can't Open file %s", file_path_and_name ) ;
        MessageBox ( GetFocus(), debug_string, "Timer Error Message", MB_OK);
      endif

      fclose  ( output_file ) ;

    endif

    else {

      va_start ( marker, format_string ) ;

      vsprintf ( debug_string, format_string, marker ) ;

      if ( ( output_file = fopen ( file_path_and_name,  "a" ) ) == NULL ) {
        sprintf ( debug_string, "Can't Open file %s", file_path_and_name ) ;
        MessageBox ( GetFocus(), debug_string, "Timer Error Message", MB_OK);
      endif

      fprintf ( output_file, "%s", debug_string ) ;

      fclose  ( output_file ) ;

    endif

  endif

}

/*****************************************************************************************************************************/
/*****************************************************************************************************************************/

void xserial_putc ( char character ) /* function declaration */

{
  comm_write_string ( & character, 1 ) ;
}

/*****************************************************************************************************************************/
/*****************************************************************************************************************************/

// This routine gets characters, appends them to a file, processes the packets, and appends data packets to fifo.

void get_characters_from_timers ( void ) /* function declaration */

{

  int character ;
  FILE * rx_file ;

  while ( ( character = ernies_comm_read_byte () ) != -1 ) {
  
    if ( ( character & 0x80 ) == 0 ) {
      stuff_character_into_terminal_fifo ( (char) character ) ;
    endif
    else {
      stuff_character_into_terminal_fifo ( '*' ) ;
    endif

    if ( ( c . diagnostic_flags & 0x02 ) != 0 ) {
      if ( ( rx_file = fopen ( g.rx_file_path_and_name,  "a" ) ) != NULL ) {
        fputc   ( character, rx_file ) ;
        fclose  ( rx_file ) ;
      endif
    endif

    if ( g . srt_1000_used_flag != 0 ) {

      if ( timer_process_rx_byte ( (char) character ) == 2 ) {      // yes, we have a completed rx packet
        process_rx_packet () ;
      endiff

    endif

    else if ( g . tag_heuer_505_used_flag != 0 ) {
  
      if ( process_tag_rx_data ( (char) character ) == 2 ) {      // yes, we have a completed rx packet
        handle_tag_505_rx_packet () ;
        stuff_character_into_terminal_fifo ( '\n' ) ;      
      endif

    endif

    else if ( g . tag_heuer_605_used_flag != 0 ) {
  
      if ( process_tag_rx_data ( (char) character ) == 2 ) {      // yes, we have a completed rx packet
        handle_tag_605_rx_packet () ;
        stuff_character_into_terminal_fifo ( '\n' ) ;      
      endif

    endif

  wend

}

/**********************************************************************/
/**********************************************************************/

// This is same as lry's comm_read_byte, except that it puts error stuff into log file

int ernies_comm_read_byte ( void ) /* function declaration */

{

  int character = -1 ;

  read_t          read_info_structure ;

  if ( comm_read_byte ( & read_info_structure ) != 0 ) {
    character =  read_info_structure . character ;
    if ( read_info_structure.error != 0 ) {
      output_formatted_file_string ( g . log_file_path_and_name, "Comm Error %8lx \n", read_info_structure.error ) ;
    endif
    if ( read_info_structure.event != 0 ) {
      output_formatted_file_string ( g . log_file_path_and_name, "Comm Event %8lx \n", read_info_structure.event ) ;
    endif
  endif

  return character ;

}

/*****************************************************************************************************************************/
/*****************************************************************************************************************************/

long DLL_EXPORT __stdcall dll_generate_dummy_record  ( void )  /* function declaration */

{

  static unsigned long dummy_record_number ;

  XC_TIMER_RECORD_STRUCTURE_TYPE  x ;

  output_formatted_file_string ( g . log_file_path_and_name, "DLL Call: Generating a Dummy Record at %s\n", _strtime ( time_string ) ) ;

  dummy_record_number ++ ;

  _strtime ( time_string ) ;

  x . table_id    =  0  ;
  x . device_num  = 99  ;
  x . record_num  = dummy_record_number  ; 
  x . event_num   =  0  ; 
  x . heat_num    =  0  ; 
  x . channel     =  0  ; 

  strcpy ( x . record_typ       , "b"            ) ;
  strcpy ( x . userstring       , "9999 dummy"   ) ;
  strcpy ( x . userxfield [ 0 ] , ""             ) ;
  strcpy ( x . userxfield [ 1 ] , ""             ) ;
  strcpy ( x . userxfield [ 2 ] , ""             ) ;
  strcpy ( x . userxfield [ 3 ] , ""             ) ;
  strcpy ( x . bib_string       , "9999"         ) ;
  strcpy ( x . timer_time       ,  time_string   ) ;
  strcpy ( x . pc_time          ,  time_string   ) ;
  strcpy ( x . notes            , "notes"        ) ;

  dll_put_timer_structure_into_fifo ( & x ) ;

  return 1 ;
}

/**********************************************************/

// Here we are passed timer data, and write it to a text file

void write_timer_structure_to_backup_file ( XC_TIMER_RECORD_STRUCTURE_TYPE * s ) /* function declaration */

{

 output_formatted_file_string ( g.backup_file_path_and_name,  

                "%d\t%d\t%d\t%d\t%d\t%d\t%s\t%s\t%s\t%s\t%s\t%s\n", 

                    s -> table_id, 
                    s -> device_num, 
                    s -> record_num,
                    s -> event_num,
                    s -> heat_num,
                    s -> channel,
 
                    s -> record_typ,
                    s -> userstring, 
                    s -> bib_string,                         
                    s -> timer_time,
                    s -> pc_time,
                    s -> notes ) ;

}

/**********************************************************/

void stuff_character_into_terminal_fifo ( char character ) /* function declaration */

{

  int tmp_read_index  ;
  int tmp_write_index ;

  tmp_write_index = terminal_fifo_write_index ;
  tmp_read_index  = terminal_fifo_read_index ;

  terminal_fifo [ tmp_write_index ] = character ;

  if   ( ++ tmp_write_index  >= TERMINAL_FIFO_MAX_LENGTH ) tmp_write_index = 0 ;

  if   (    tmp_write_index == tmp_read_index ) {

    tmp_read_index += 20 ;

    if ( tmp_read_index >= TERMINAL_FIFO_MAX_LENGTH ) tmp_read_index -= TERMINAL_FIFO_MAX_LENGTH ;

  endif

  terminal_fifo_read_index  = tmp_read_index  ;
  terminal_fifo_write_index = tmp_write_index ;

}

/***************************************/

// Here we read from terminal fifo

long DLL_EXPORT __stdcall dll_get_character_from_terminal_fifo ( void ) /* function declaration */

{

  long character ;
  int  tmp_read_index  ;

  tmp_read_index  = terminal_fifo_read_index ;

  if ( tmp_read_index != terminal_fifo_write_index ) {
    character = (long) terminal_fifo [ tmp_read_index ] ; 
    if ( ++ tmp_read_index >= TERMINAL_FIFO_MAX_LENGTH ) tmp_read_index = 0 ;
  endif
  else {
    character = -1 ;
  endif

  terminal_fifo_read_index  = tmp_read_index  ;

  return ( character ) ;

}

/*****************************************************************************************************************************/
/*****************************************************************************************************************************/

// This is the same as comm_open, except that first, we close the comm port and delay a wee bit
//      Also, diagnostics only have to appear once

int local_comm_open ( int local_comm_port, long local_baud_rate ) /* function declaration */

{

  read_t  read_info_structure ;

  output_formatted_file_string ( g . log_file_path_and_name, "Initializing COMM %4d Port with Baud Rate of %6ld  at %s \n", local_comm_port, local_baud_rate, _strtime ( time_string ) ) ;

  #if ( RELEASE_DATE >= 0x061203 )
    delay ( 1000 ) ;
  #endif

  #if ( RELEASE_DATE <  0x061203 )
    comm_port_initialized_flag = 1 ;  // To force a close 
  #endif

  if ( comm_port_initialized_flag != 0 ) {
    comm_close () ;
    comm_port_initialized_flag = 0 ;
    delay ( 500 ) ;
  endif

  set_error_reporter ( comm_error_handler ) ;

  if ( comm_open ( local_comm_port - 1, local_baud_rate, 0, NULL ) == 1 ) {

    comm_port_initialized_flag = 1 ;                          

    while ( comm_read_byte ( & read_info_structure ) != 0 ) ;   // Here we purge the RX buffer

    output_formatted_file_string ( g . log_file_path_and_name, "COM Port Initialized\n\n" ) ;

  endif

  else {

    comm_port_initialized_flag = 0 ;

    MessageBox ( GetFocus(), "Cannot Open Comm Port. Perhaps the Data Acquisition Module is already open, or COMM Port has been opened by another application\n\n" , "Timer Error Message", MB_OK);

    output_formatted_file_string ( g . log_file_path_and_name, "Cannot Open Comm Port. Perhaps opened by another Application\n\n" ) ;

  endif


  return comm_port_initialized_flag ;

}

/*****************************************************************************************************************************/
/*****************************************************************************************************************************/

long DLL_EXPORT __stdcall dll_delete_records_in_backup_text_file ( void ) /* function declaration */

{
  create_backup_text_file ( 1 ) ;
  return 1 ;
}

/*****************************************************************************************************************************/

void create_backup_text_file ( long delete_flag ) /* function declaration */

{

  char scratch_string [ 500 ] ;

  output_formatted_file_string ( g . log_file_path_and_name, "DLL Call: Creating Backup Text File at %s \n", _strtime ( time_string ) ) ;

  #if ( RELEASE_DATE >= 0x090604 )
    get_file_path_and_name_from_parts ( g.backup_file_path_and_name, g . race_directory, "srt_backup_timer_data",  ".txt"  ) ;
  #endif

  if ( delete_flag != 0 ) {
    output_formatted_file_string ( g.backup_file_path_and_name,  NULL ) ;
  endif

  else if ( access ( g.backup_file_path_and_name, 0 ) != 0 ) {    // If backup file does not exist, it is created

    output_formatted_file_string ( g.backup_file_path_and_name,  NULL ) ;

  endif

  output_formatted_file_string ( g.backup_file_path_and_name,  "\nSummit Systems Timer Backup Text File Initialized" ) ;
  output_formatted_file_string ( g.backup_file_path_and_name,  "  at %s",      _strtime ( time_string ) ) ;
  output_formatted_file_string ( g.backup_file_path_and_name,  "  on %s:\n\n", _strdate ( scratch_string ) ) ;

  output_formatted_file_string ( g.backup_file_path_and_name,  "%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\n", 
        "table_id",
        "device_num",
        "record_num",
        "event_num",
        "heat_num",
        "channel",
        "record_typ",
        "total_user_string",
        "bib_string",
        "timer_time",
        "pc_time",
        "notes"
  ) ;

}                                            

/*****************************************************************************************************************************/

void delay ( unsigned int delay_time ) /* function declaration */

{
  unsigned long current_time ;
  current_time = GetTickCount () ;
  while ( ( GetTickCount () - current_time ) < (unsigned long) delay_time ) ;
}

/*****************************************************************************************************************************/
/*****************************************************************************************************************************/

XC_CONFIGURE_STRUCTURE_TYPE DLL_EXPORT * __stdcall dll_get_pointer_to_configuration_structure ( void ) /* function declaration */

{

  return ( & c );

}

/*****************************************************************************************************************************/
/*****************************************************************************************************************************/

GLOBAL_VARIABLE_STRUCTURE_TYPE DLL_EXPORT * __stdcall dll_get_pointer_to_global_variable_structure ( void ) /* function declaration */

{

  return ( & g );

}

/*****************************************************************************************************************************/
/*****************************************************************************************************************************/
 
long DLL_EXPORT __stdcall dll_reset_all_timer_record_counters_to_zero ( void ) /* function declaration */

{

  int local_device_number ;

  output_formatted_file_string ( g . log_file_path_and_name, "DLL Call: Reseting all Timer Record Counters to Zero at %s\n", _strtime ( time_string ) ) ;

  for ( local_device_number = 0 ; local_device_number < MAX_NUMBER_OF_DEVICES ; local_device_number ++ ) {
    c . highest_record_received_array [ local_device_number ] = 0 ;
  next

  return 1 ;
}

/*****************************************************************************************************************************/
/*****************************************************************************************************************************/
/*****************************************************************************************************************************/

void read_all_ini_parameters ( int read_ini_file_flag, char * tmp_ini_file_path_and_name ) /* function declaration */

{

  char ini_file_string [ 600 ] ;
  FILE * ini_file ;

  if ( ( read_ini_file_flag & 0x0F ) != 0 ) {

    if   ( ( access ( tmp_ini_file_path_and_name, 0 ) == 0 ) &&                // If file exists
         ( ( ini_file = fopen ( tmp_ini_file_path_and_name, "r" ) ) != NULL ) ) {   //    and we can open it
      while ( fgets ( ini_file_string, 500, ini_file ) != NULL ) {
        parse_ini_file ( ini_file_string ) ;
      wend
      fclose ( ini_file ) ;
    endif

    else {
      if ( read_ini_file_flag > 1 ) {
        dll_save_ini_file () ;
      endif
    endif

  endif

}

/*****************************************************************/

long DLL_EXPORT __stdcall dll_save_ini_file ( void ) /* function declaration */

{

  char tmp_ini_file_path_and_name [ 300 ] ;

  FILE * ini_file ;

  strcpy ( tmp_ini_file_path_and_name, g.ini_file_path_and_name ) ;

  if ( ( ini_file = fopen ( tmp_ini_file_path_and_name, "w" ) ) == NULL ) {
    char error_string [ 300 ] ;
    sprintf ( error_string, "Can't Open file %s", tmp_ini_file_path_and_name ) ;
    MessageBox ( GetFocus(), error_string, "Timer Error Message", MB_OK);
  endif

  else {

    fprintf ( ini_file, "serial_comm_port                          = %5d ; \n",     c.serial_comm_port                     ) ;
    fprintf ( ini_file, "baud_rate                                 = %5d ; \n",     c.baud_rate                            ) ;

    fprintf ( ini_file, "number_of_devices                         = %5d ; \n",     c.number_of_devices_used               ) ;
    fprintf ( ini_file, "diagnostic_flags                          = %5ld ; \n",    c.diagnostic_flags                     ) ;
    fprintf ( ini_file, "talk_time                                 = %5d ; \n",     c.talk_time                            ) ;
//  fprintf ( ini_file, "srt_1000_used_flag                        = %5d ; \n",     c.srt_1000_used_flag                   ) ;
//  fprintf ( ini_file, "tag_heuer_505_used_flag                   = %5d ; \n",     c.tag_heuer_505_used_flag              ) ;
//  fprintf ( ini_file, "tag_heuer_605_used_flag                   = %5d ; \n",     c.tag_heuer_605_used_flag              ) ;
//  fprintf ( ini_file, "hora_targets_used_flag                    = %5d ; \n",     c.hora_targets_used_flag               ) ;
    fprintf ( ini_file, "left_justify_time_strings_flag            = %5d ; \n",     c.left_justify_time_strings_flag       ) ;
    fprintf ( ini_file, "string_delimiter                          = %5d ; \n",     c.string_delimiter                     ) ;

    fprintf ( ini_file, "timer_type_used                           = \"%s\" ; \n",  c.timer_type_used                      ) ;
    fprintf ( ini_file, "database_type                             = \"%s\" ; \n",  c.database_type                        ) ;
    fprintf ( ini_file, "database_drive                            = \"%s\" ; \n",  c.database_drive                       ) ;
    fprintf ( ini_file, "database_directory                        = \"%s\" ; \n",  c.database_directory                   ) ;
    fprintf ( ini_file, "database_name                             = \"%s\" ; \n",  c.database_name                        ) ;
    fprintf ( ini_file, "database_extension                        = \"%s\" ; \n",  c.database_extension                   ) ;
    fprintf ( ini_file, "database_table_name                       = \"%s\" ; \n",  c.database_table_name                  ) ;

    fprintf ( ini_file, "odbc_user_dsn_flag                        = %5d ; \n",     c.odbc_user_dsn_flag                   ) ;
    fprintf ( ini_file, "database_odbc_user_data_source_name       = \"%s\" ; \n",  c. database_odbc_user_data_source_name ) ;

    fprintf ( ini_file, "database_highest_record_number            = %5d ; \n",     c.database_highest_record_number       ) ;

    fprintf ( ini_file, "timer1_highest_record_received            = %5d ; \n",     c.highest_record_received_array [ 1 ]  ) ;
    fprintf ( ini_file, "timer2_highest_record_received            = %5d ; \n",     c.highest_record_received_array [ 2 ]  ) ;
    fprintf ( ini_file, "timer3_highest_record_received            = %5d ; \n",     c.highest_record_received_array [ 3 ]  ) ;
    fprintf ( ini_file, "timer4_highest_record_received            = %5d ; \n",     c.highest_record_received_array [ 4 ]  ) ;
    fprintf ( ini_file, "timer5_highest_record_received            = %5d ; \n",     c.highest_record_received_array [ 5 ]  ) ;
    fprintf ( ini_file, "timer6_highest_record_received            = %5d ; \n",     c.highest_record_received_array [ 6 ]  ) ;
    fprintf ( ini_file, "timer7_highest_record_received            = %5d ; \n",     c.highest_record_received_array [ 7 ]  ) ;
    fprintf ( ini_file, "timer8_highest_record_received            = %5d ; \n",     c.highest_record_received_array [ 8 ]  ) ;
    fprintf ( ini_file, "timer9_highest_record_received            = %5d ; \n",     c.highest_record_received_array [ 9 ]  ) ;

    fclose ( ini_file ) ;

  endif

  return 1 ;
}

/*****************************************************************/

void parse_ini_file ( char * command_string ) /* function declaration */

{

   signed   char   * ptr_to_char   ;
 unsigned   char   * ptr_to_uchar  ;
 unsigned   short  * ptr_to_hex    ;
   signed   short  * ptr_to_short  ;
 unsigned   short  * ptr_to_ushort ;
   signed   int    * ptr_to_int    ;
 unsigned   int    * ptr_to_uint   ;
   signed   long   * ptr_to_long   ;
 unsigned   long   * ptr_to_ulong  ;
            double * ptr_to_double ;
   signed   char   * ptr_to_string ;

  char   * equal_ptr, * comment_ptr, * command_ptr, * quote_ptr ;

// First, we get ptr to equal sign

  if   ( ( equal_ptr = strchr ( command_string, '=' ) ) != NULL ) {
    * equal_ptr ++ = '\0' ;
  endif

  command_ptr = get_ptr_to_first_non_white ( command_string ) ;

  remove_trailing_white ( command_ptr ) ;  

  ptr_to_char   = NULL ;
  ptr_to_uchar  = NULL ;
  ptr_to_hex    = NULL ;
  ptr_to_short  = NULL ;
  ptr_to_ushort = NULL ;
  ptr_to_int    = NULL ;
  ptr_to_uint   = NULL ;
  ptr_to_long   = NULL ;
  ptr_to_ulong  = NULL ;
  ptr_to_double = NULL ;
  ptr_to_string = NULL ;

  if ( equal_ptr != NULL ) {

  #if ( RELEASE_DATE >= 0x090604 )
    if      ( strcmpi ( command_ptr, "serial_comm_port"                            ) == 0 ) ptr_to_uint  = & c.serial_comm_port                 ;
  #endif

  #if ( RELEASE_DATE <  0x090604 )
    if      ( strcmpi ( command_ptr, "serial_comm_port"                            ) == 0 ) ptr_to_uchar  = & c.serial_comm_port                 ;
  #endif

    else if ( strcmpi ( command_ptr, "baud_rate"                                   ) == 0 ) ptr_to_ulong  = & c.baud_rate                        ;

    else if ( strcmpi ( command_ptr, "number_of_devices"                           ) == 0 ) ptr_to_ushort = & c.number_of_devices_used           ;
    else if ( strcmpi ( command_ptr, "diagnostic_flags"                            ) == 0 ) ptr_to_long   = & c.diagnostic_flags                 ;
    else if ( strcmpi ( command_ptr, "talk_time"                                   ) == 0 ) ptr_to_ushort = & c.talk_time                        ;

//  else if ( strcmpi ( command_ptr, "srt_1000_used_flag"                          ) == 0 ) ptr_to_uchar  = & c.srt_1000_used_flag               ;
//  else if ( strcmpi ( command_ptr, "tag_heuer_505_used_flag"                     ) == 0 ) ptr_to_uchar  = & c.tag_heuer_505_used_flag          ;
//  else if ( strcmpi ( command_ptr, "tag_heuer_605_used_flag"                     ) == 0 ) ptr_to_uchar  = & c.tag_heuer_605_used_flag          ;
//  else if ( strcmpi ( command_ptr, "hora_targets_used_flag"                      ) == 0 ) ptr_to_uchar  = & c.hora_targets_used_flag           ;
    else if ( strcmpi ( command_ptr, "left_justify_time_strings_flag"              ) == 0 ) ptr_to_uchar  = & c.left_justify_time_strings_flag   ;
    else if ( strcmpi ( command_ptr, "odbc_user_dsn_flag"                          ) == 0 ) ptr_to_uchar  = & c.odbc_user_dsn_flag               ;
    else if ( strcmpi ( command_ptr, "string_delimiter"                            ) == 0 ) ptr_to_char   = & c.string_delimiter                 ;

    else if ( strcmpi ( command_ptr, "timer_type_used"                             ) == 0 ) ptr_to_string = & ( c.timer_type_used                [ 0 ] ) ;
    else if ( strcmpi ( command_ptr, "database_type"                               ) == 0 ) ptr_to_string = & ( c.database_type                  [ 0 ] ) ;
    else if ( strcmpi ( command_ptr, "database_drive"                              ) == 0 ) ptr_to_string = & ( c.database_drive                 [ 0 ] ) ;
    else if ( strcmpi ( command_ptr, "database_directory"                          ) == 0 ) ptr_to_string = & ( c.database_directory             [ 0 ] ) ;
    else if ( strcmpi ( command_ptr, "database_name"                               ) == 0 ) ptr_to_string = & ( c.database_name                  [ 0 ] ) ;
    else if ( strcmpi ( command_ptr, "database_extension"                          ) == 0 ) ptr_to_string = & ( c.database_extension             [ 0 ] ) ;
    else if ( strcmpi ( command_ptr, "database_table_name"                         ) == 0 ) ptr_to_string = & ( c.database_table_name            [ 0 ] ) ;
    else if ( strcmpi ( command_ptr, "database_odbc_user_data_source_name"         ) == 0 ) ptr_to_string = & ( c.database_odbc_user_data_source_name [ 0 ] ) ;

    else if ( strcmpi ( command_ptr, "database_highest_record_number"              ) == 0 ) ptr_to_long   = & c.database_highest_record_number  ;

    else if ( strcmpi ( command_ptr, "timer1_highest_record_received"              ) == 0 ) ptr_to_ushort = & ( c.highest_record_received_array [ 1 ] )  ;
    else if ( strcmpi ( command_ptr, "timer2_highest_record_received"              ) == 0 ) ptr_to_ushort = & ( c.highest_record_received_array [ 2 ] )  ;
    else if ( strcmpi ( command_ptr, "timer3_highest_record_received"              ) == 0 ) ptr_to_ushort = & ( c.highest_record_received_array [ 3 ] )  ;
    else if ( strcmpi ( command_ptr, "timer4_highest_record_received"              ) == 0 ) ptr_to_ushort = & ( c.highest_record_received_array [ 4 ] )  ;
    else if ( strcmpi ( command_ptr, "timer5_highest_record_received"              ) == 0 ) ptr_to_ushort = & ( c.highest_record_received_array [ 5 ] )  ;
    else if ( strcmpi ( command_ptr, "timer6_highest_record_received"              ) == 0 ) ptr_to_ushort = & ( c.highest_record_received_array [ 6 ] )  ;
    else if ( strcmpi ( command_ptr, "timer7_highest_record_received"              ) == 0 ) ptr_to_ushort = & ( c.highest_record_received_array [ 7 ] )  ;
    else if ( strcmpi ( command_ptr, "timer8_highest_record_received"              ) == 0 ) ptr_to_ushort = & ( c.highest_record_received_array [ 8 ] )  ;
    else if ( strcmpi ( command_ptr, "timer9_highest_record_received"              ) == 0 ) ptr_to_ushort = & ( c.highest_record_received_array [ 9 ] )  ;

 
    if      ( ptr_to_string != NULL ) {     

      remove_leading_and_trailing_white ( equal_ptr ) ;

      if ( ( * equal_ptr ++ == '\"' ) && ( ( ( quote_ptr = strchr ( equal_ptr, '\"' ) ) != NULL ) ) ) {

        * quote_ptr = '\0' ;

        strcpy ( ptr_to_string, equal_ptr ) ;

      endif

    endif

    else {
      if ( ( comment_ptr = strchr ( equal_ptr, ';' ) ) != NULL ) {
        * comment_ptr ++ = '\0' ;
      endif
    endif

    if      ( ptr_to_char  != NULL ) {     
      * ptr_to_char = (char) atoi ( equal_ptr ) ;
    endif
    if      ( ptr_to_uchar  != NULL ) {     
      * ptr_to_uchar = (unsigned char) atoi ( equal_ptr ) ;
    endif
    if      ( ptr_to_hex  != NULL ) {     
      remove_leading_and_trailing_white ( equal_ptr ) ;
      * ptr_to_hex = (unsigned short) hex_atoi ( equal_ptr ) ;
    endif
    if      ( ptr_to_short  != NULL ) {     
      * ptr_to_short = (short) atoi ( equal_ptr ) ;
    endif
    if      ( ptr_to_ushort  != NULL ) {     
      * ptr_to_ushort = (unsigned short) atoi ( equal_ptr ) ;
    endif
    if      ( ptr_to_int  != NULL ) {     
      * ptr_to_int = (int) atoi ( equal_ptr ) ;
    endif
    if      ( ptr_to_uint  != NULL ) {     
      * ptr_to_uint = (unsigned int) atoi ( equal_ptr ) ;
    endif
    if      ( ptr_to_long  != NULL ) {     
      * ptr_to_long = (long) atol ( equal_ptr ) ;
    endif
    if      ( ptr_to_long  != NULL ) {     
      * ptr_to_long = (long) atol ( equal_ptr ) ;
    endif
    if      ( ptr_to_ulong  != NULL ) {     
      * ptr_to_ulong = (unsigned long) atol ( equal_ptr ) ;
    endif

    if      ( ptr_to_double != NULL ) {     
      * ptr_to_double = atof ( equal_ptr ) ;
    endif

  endif

}

/*****************************************************************************************************************************/
/*****************************************************************************************************************************/
 
long DLL_EXPORT __stdcall dll_assign_default_values ( void ) /* function declaration */

{

  int index ;

  c.diagnostic_flags                         =      0 ;

  c.serial_comm_port                         =      1 ;  // 1 for COMM1, etc
  c.baud_rate                                =   9600 ;
                                                         
  c.number_of_devices_used                   =      4 ;
  c.talk_time                                =    400 ;

  g.srt_1000_used_flag                       =      1 ;
  g.tag_heuer_505_used_flag                  =      0 ;
  g.tag_heuer_605_used_flag                  =      0 ;
  g.hora_targets_used_flag                   =      0 ;

  c.string_delimiter                         =    '!' ;

  c.left_justify_time_strings_flag           =      1 ;

  strcpy ( c.timer_type_used,                     "SRT 1000"                             ) ;                               

  c.database_highest_record_number           =      0 ;

  for ( index = 0 ; index < MAX_NUMBER_OF_DEVICES ; index ++ ) {
    c.highest_record_received_array [ index ] = 0 ;
  next

  strcpy ( c.database_type,                       "Access"                               ) ;
  strcpy ( c.database_drive,                      "c:"                                   ) ;
  strcpy ( c.database_directory,                  "\\srt_timing\\srt_data_acquisition_module\\" ) ;
  strcpy ( c.database_name,                       "srt_timer_data"                       ) ;
  strcpy ( c.database_extension,                  ".mdb"                                 ) ;
  strcpy ( c.database_table_name,                 "timer_data"                           ) ;


  c.odbc_user_dsn_flag                       = 0 ;
  strcpy ( c.database_odbc_user_data_source_name, ""                                     ) ;  // This takes mdb name from Control Panel

  c.event_number = 0 ;
  c.heat_number  = 0 ;
           

  return 1 ;
}

/*****************************************************************************************************************************/
/*****************************************************************************************************************************/

// This is required, because stdio.c calls this function several times

void handle_serial_interrupt ( void ) /* function declaration */
{}

/*****************************************************************************************************************************/
/*****************************************************************************************************************************/

/***********************************************************************************/

void comm_error_handler ( char * message ) /* function declaration */

{
  output_formatted_file_string ( g . log_file_path_and_name, "%s\n", message ) ;
}

/*****************************************************************************************************************************/
/*****************************************************************************************************************************/
/*****************************************************************************************************************************/

int get_file_path_and_name_from_parts ( char * file_path_and_name, char * path, char * file_name, char * file_extension  ) /* function declaration */

{

  strcpy ( file_path_and_name, path ) ;

  if ( file_path_and_name [ strlen ( file_path_and_name ) - 1 ] != '\\' ) {
    strcat ( file_path_and_name, "\\" ) ;
  endif

  strcat ( file_path_and_name, file_name ) ;

  if ( file_extension [ 0 ] == '.' ) {
  endif
  else if ( file_name [ strlen ( file_name ) - 1 ] == '.' ) {
  endif
  else if ( file_extension [ 0 ] != '\0' ) {
    strcat ( file_path_and_name, "." ) ;
  endif

  strcat ( file_path_and_name, file_extension ) ;

  return 1 ;

}

/*****************************************************************************************************************************/
/*****************************************************************************************************************************/

long DLL_EXPORT __stdcall dll_synch_timers ( long * synch_timers_flag, char * synch_time_string )  /* function declaration */

{

  char packet_string [ 500 ] ;    

  if ( ( synch_timers_flag == NULL ) || ( synch_time_string == NULL ) || ( synch_time_string [ 0 ] == '\0' ) || ( (int) (* synch_timers_flag) == SYNCH_TIMERS_TO_SYSTEM_CLOCK ) ) {
    sprintf ( packet_string, "SY %s",  _strtime ( time_string ) ) ;
  endif

  else if ( (int) (* synch_timers_flag) == SYNCH_TIMERS_TO_USER_CLOCK ) {
    sprintf ( packet_string, "SY %s", synch_time_string ) ;
  endif
  
  else {
    sprintf ( packet_string, "SYO %s", synch_time_string ) ;
  endif

  output_formatted_file_string ( g . log_file_path_and_name,  "Synching Timers: at %s  Synch Packet: %s \n\n", _strtime ( time_string ), packet_string ) ;

  pc_transmit_generic_packet ( packet_string ) ;

  return 1 ;
}

/*****************************************************************************************************************************/
/*****************************************************************************************************************************/

long DLL_EXPORT __stdcall dll_reset_timers ( void ) /* function declaration */

{

  output_formatted_file_string ( g . log_file_path_and_name,  "Reseting Timers: at %s  \n\n", _strtime ( time_string ) ) ;


  pc_transmit_generic_packet ( "RS " ) ;

  dll_reset_all_timer_record_counters_to_zero () ;

  return 1 ;
}

/*****************************************************************************************************************************/
/*****************************************************************************************************************************/

long DLL_EXPORT __stdcall dll_disable_timer_reset ( void ) /* function declaration */

{

  output_formatted_file_string ( g . log_file_path_and_name,  "Disabling Reset: at %s  \n\n", _strtime ( time_string ) ) ;

  pc_transmit_generic_packet ( "RS x" ) ;
  dll_reset_all_timer_record_counters_to_zero () ;

  return 1 ;
}

/*****************************************************************************************************************************/
/*****************************************************************************************************************************/

long DLL_EXPORT __stdcall dll_start_communicating_with_timers ( void ) /* function declaration */

{

  g.srt_1000_used_flag      = 0 ;
  g.tag_heuer_505_used_flag = 0 ;
  g.tag_heuer_605_used_flag = 0 ;
  g.hora_targets_used_flag  = 0 ;

  if ( strcmpi ( c . timer_type_used, "SRT 1000" ) == 0 ) g.srt_1000_used_flag      = 1 ;
  if ( strcmpi ( c . timer_type_used, "Tag 505"  ) == 0 ) g.tag_heuer_505_used_flag = 1 ;
  if ( strcmpi ( c . timer_type_used, "Tag 605"  ) == 0 ) g.tag_heuer_605_used_flag = 1 ;
  if ( strcmpi ( c . timer_type_used, "Hora"     ) == 0 ) g.hora_targets_used_flag  = 1 ;

  max_sleep_counter          = c . talk_time / 200 ;

  sleep_counter              = 0 ;
  token_cycle_counter        = 0 ;
  device_number_with_token   = 0 ;

  create_backup_text_file ( 0 ) ;


  if ( ( c . diagnostic_flags & 0x01 ) != 0 ) {

    get_file_path_and_name_from_parts ( g.tx_file_path_and_name, g . race_directory         , "tx", ".txt"       ) ;
    output_formatted_file_string ( g.tx_file_path_and_name,       NULL ) ;

  endif

  if ( ( c . diagnostic_flags & 0x02 ) != 0 ) {

    get_file_path_and_name_from_parts ( g.rx_file_path_and_name, g . race_directory         , "rx", ".txt"       ) ;
    output_formatted_file_string ( g.rx_file_path_and_name,       NULL ) ;

  endif



  local_comm_open ( c.serial_comm_port, c.baud_rate ) ;
  start_timer_events () ;

  return timer_events_enabled_flag ;

}

/*****************************************************************************************************************************/
/*****************************************************************************************************************************/

long DLL_EXPORT __stdcall dll_stop_communicating_with_timers ( void ) /* function declaration */

{

  stop_timer_events () ;
  comm_close () ;

  output_formatted_file_string ( g . log_file_path_and_name,  "Stopped Communicating with Timers at %s\n\n", _strtime ( time_string ) ) ;

  return 1 ;
}

/*****************************************************************************************************************************/
/*****************************************************************************************************************************/

void static stop_timer_events ( void ) /* function declaration */

{

  if ( timer_id != 0 ) {

    timeKillEvent ( timer_id ) ;

    timeEndPeriod ( (WORD) g.timer_event_resolution ) ;

    timer_events_enabled_flag = 0 ;

    timer_id = 0 ;

  endif

}

/*****************************************************************************************************************************/
/*****************************************************************************************************************************/

void static start_timer_events ( void ) /* function declaration */

{

  timer_events_enabled_flag = 0 ;

  if ( timer_id == 0 ) {

    if ( comm_port_initialized_flag != 0 ) {

      if ( ( timer_id = timeSetEvent ( (WORD) g.timer_event_interval, (WORD) g.timer_event_resolution, dll_timer_events, (DWORD) NULL, TIME_PERIODIC ) ) != 0 ) {

        timer_events_enabled_flag = 1 ;
 
        timeBeginPeriod ( (WORD) g.timer_event_resolution ) ;

        output_formatted_file_string ( g . log_file_path_and_name,  "Started Successfully Communicating with Timers at %s\n\n", _strtime ( time_string ) ) ;

      endif

    endif

  endif


}


/*****************************************************************************************************************************/
/*****************************************************************************************************************************/

// This routine is called every 50 msec
// Here we address the next device

void CALLBACK dll_timer_events ( unsigned int id, unsigned int msg, DWORD dwUser, DWORD dw1, DWORD dw2 ) /* function declaration */

{

  char packet_string [ 200 ] ;

  if ( comm_port_initialized_flag != 0 ) {

    if ( timer_events_enabled_flag != 0 ) {

      if ( sleep_counter ++ >= max_sleep_counter ) {

//      output_formatted_file_string ( g . log_file_path_and_name,  "DLL Timer Events Here \n" ) ;

        sleep_counter = 0 ;

        if ( ++ token_cycle_counter == 1 ) {

//        output_formatted_file_string ( g . log_file_path_and_name,  "Token Counter = 1 \n" ) ;

          sprintf ( packet_string, "TK 0" ) ;
          pc_transmit_generic_packet ( packet_string ) ;
        endif

        else if ( token_cycle_counter == 2 ) {

          sprintf ( packet_string, "TK %d %d", device_number_with_token,  1  +  c . highest_record_received_array [ device_number_with_token ] ) ;
          pc_transmit_generic_packet ( packet_string ) ;

//        output_formatted_file_string ( g . log_file_path_and_name,  "Token Packet: %s \n", packet_string ) ;

          if ( ++ device_number_with_token > c . number_of_devices_used ) {
            device_number_with_token = 1 ;
          endif

        endif

        else if ( token_cycle_counter >= 4 ) {
          token_cycle_counter = 0 ;
        endif

      endiff

      get_characters_from_timers () ;

    endif

  endif

}

/*****************************************************************************************************************************/
/*****************************************************************************************************************************/

long DLL_EXPORT __stdcall dll_set_event_and_heat ( long * q_device, long * q_event, long * q_heat ) /* function declaration */

{
 
  int device ;

  if ( q_device == NULL ) {
    device = 0 ;
  endif
  else {
    device               = (int) ( * q_device ) ;
  endif

  if ( q_event == NULL ) {
    c.event_number       = 0 ;
  endif
  else {
    c.event_number       = (int) ( * q_event    ) ;
  endif

  if ( q_heat == NULL ) {
    c.heat_number        = 0 ;
  endif
  else {
    c.heat_number        = (int) ( * q_heat     ) ;
  endif
 
  sprintf ( scratch_string, "EV %2ld  %2ld  %2ld", device, c.event_number, c.heat_number ) ;

  pc_transmit_generic_packet ( scratch_string ) ;

  output_formatted_file_string ( g . log_file_path_and_name,  "Sent Event and Heat: at %s  Packet: %s \n\n", _strtime ( time_string ), scratch_string ) ;

  return 1 ;
}

/*****************************************************************************************************************************/
/*****************************************************************************************************************************/

long DLL_EXPORT __stdcall dll_set_comm_port ( long * comm_port ) /* function declaration */

{

  if ( comm_port == NULL ) {
    c.serial_comm_port = 1 ;
  endif
  else {
    c.serial_comm_port = (int) ( * comm_port ) ;
  endif

  output_formatted_file_string ( g . log_file_path_and_name,  "Set COMM Port to %ld\n\n", c.serial_comm_port ) ;


  return 1 ;
}

/*****************************************************************************************************************************/
/*****************************************************************************************************************************/

long DLL_EXPORT __stdcall dll_set_baud_rate ( long * baud_rate ) /* function declaration */

{

  if ( baud_rate == NULL ) {
    c.baud_rate = 9600 ;
  endif
  else {
    c.baud_rate = (int) ( * baud_rate ) ;
  endif

  output_formatted_file_string ( g . log_file_path_and_name,  "Set Baud Rate to %ld\n\n", c.baud_rate ) ;


  return 1 ;
}

/*****************************************************************************************************************************/
/*****************************************************************************************************************************/

long DLL_EXPORT __stdcall dll_set_number_of_timers ( long * number_of_timers ) /* function declaration */
{

  if ( number_of_timers == NULL ) {
    c.number_of_devices_used = 2 ;
  endif
  else {
    c.number_of_devices_used = (int) ( * number_of_timers ) ;
  endif

  output_formatted_file_string ( g . log_file_path_and_name,  "Set Number of Timers to %ld\n\n", c.number_of_devices_used ) ;

  return 1 ;
}

/*****************************************************************************************************************************/
/*****************************************************************************************************************************/

long DLL_EXPORT __stdcall dll_set_talk_time ( long * talk_time ) /* function declaration */

{

  if ( talk_time == NULL ) {
    c.talk_time = 400 ;
  endif
  else {
    c.talk_time = (int) ( * talk_time ) ;
  endif

  output_formatted_file_string ( g . log_file_path_and_name,  "Set Talk Time to %ld\n\n", c.talk_time ) ;

  return 1 ;
}

/*****************************************************************************************************************************/
/*****************************************************************************************************************************/

long DLL_EXPORT __stdcall dll_set_string_delimiter  ( long * string_delimiter ) /* function declaration */
{

  if ( string_delimiter == NULL ) {
    c.string_delimiter = '\0' ;
  endif
  else {
    c.string_delimiter = (char) ( * string_delimiter ) ;
  endif

  return 1 ;

}

/*****************************************************************************************************************************/

long DLL_EXPORT __stdcall dll_enable_left_justification_of_time_strings ( void ) /* function declaration */
{
  c.left_justify_time_strings_flag  = 1 ;
  return 1 ;
}

/*****************************************************************************************************************************/
/*****************************************************************************************************************************/

long DLL_EXPORT __stdcall dll_disable_left_justification_of_time_strings  ( void ) /* function declaration */
{
  c.left_justify_time_strings_flag  = 0 ;
  return 1 ;
}

/*****************************************************************************************************************************/
/*****************************************************************************************************************************/

long DLL_EXPORT __stdcall dll_set_highest_record_received_for_this_device ( long * q_device, long * highest_record_already_received ) /* function declaration */
 
{

  int device ;
  int highest_record ;

  if ( q_device == NULL ) {
    device = 0 ;
  endif
  else {
    device = (int) ( * q_device ) ;
  endif

  if ( highest_record_already_received == NULL ) {
    highest_record = 0 ;
  endif
  else {
    highest_record = (int) ( * highest_record_already_received ) ;
  endif

  if ( device < MAX_NUMBER_OF_DEVICES ) {

    c.highest_record_received_array [ device ] = highest_record ;

  endif

  return 1 ;
}

/*****************************************************************************************************************************/
/*****************************************************************************************************************************/

long DLL_EXPORT __stdcall dll_set_highest_record_number_in_database ( long * highest_record_number ) /* function declaration */

{

  if ( highest_record_number == NULL ) {
    c.database_highest_record_number = 0 ;
  endif
  else {
    c.database_highest_record_number = (int) ( * highest_record_number ) ;
  endif

  return 1 ;
}

/*****************************************************************************************************************************/
/*****************************************************************************************************************************/

long DLL_EXPORT __stdcall dll_set_timer_type_used ( char * timer_type ) /* function declaration */

{

  if ( ( timer_type == NULL ) || ( timer_type [ 0 ] == '\0' ) || ( c.password != 2718281828 ) ) {
    strcpy ( c.timer_type_used, "SRT 1000" ) ;
  endif
  else {
    strcpy ( c.timer_type_used, timer_type ) ;
  endif

  output_formatted_file_string ( g . log_file_path_and_name,  "Set Timer Type to %s\n\n", c.timer_type_used ) ;

  return 1 ;

}

/*****************************************************************************************************************************/
/*****************************************************************************************************************************/

// Added password 11/24/03

long DLL_EXPORT __stdcall dll_set_password ( long * password ) /* function declaration */

{

  if ( password == NULL ) {
    c.password = 0 ;
  endif
  else {
    c.password = * password ;
  endif

  return 1 ;

}

/*****************************************************************************************************************************/
/*****************************************************************************************************************************/

long DLL_EXPORT __stdcall dll_set_database_parameters ( char * database_type, char * database_drive, char * database_directory, char * database_name, char * database_extension, char * database_table_name ) /* function declaration */

{

  strcpy ( c.database_type        ,     database_type          ) ;
  strcpy ( c.database_drive       ,     database_drive         ) ;
  strcpy ( c.database_directory   ,     database_directory     ) ;
  strcpy ( c.database_name        ,     database_name          ) ;
  strcpy ( c.database_extension   ,     database_extension     ) ;
  strcpy ( c.database_table_name  ,     database_table_name    ) ;

  output_formatted_file_string ( g . log_file_path_and_name, "Set Database Parameters at %s \n\n", _strtime ( time_string ) ) ;

  return 1 ;
}

/*****************************************************************************************************************************/
/*****************************************************************************************************************************/

long DLL_EXPORT __stdcall dll_set_diagnostic_flags ( long * diagnostic_flags ) /* function declaration */

{

  if ( diagnostic_flags == NULL ) {
    c.diagnostic_flags = -1 ;
  endif
  else {
    c.diagnostic_flags = * diagnostic_flags ;
  endif

  output_formatted_file_string ( g . log_file_path_and_name, "Set Diagnostic Flags to %8lx \n\n", c.diagnostic_flags ) ;


  return 1 ;
}

/****************************************************************************************************/
/****************************************************************************************************/

long DLL_EXPORT __stdcall dll_test_function_call_passing_string ( char * string1, char * string2 ) /* function declaration */

{

  strcpy ( string1, "Hello world from Timer DLL!" ) ;

  strcpy ( string2, "Have Fun!" ) ;

  return 314159L ;

}

/****************************************************************************************************/
/****************************************************************************************************/

// Added 11/24/03

// Note:  We do not want to stop communicating with timers, because we need to transmit "TK 0" packets

// What really needs to happen is the following.
//           Tell all devices to shut up.
//           delay a second
//           delete records from the fifo
//           

long DLL_EXPORT __stdcall dll_retransmit_data ( long * retransmit_flag )  /* function declaration */

{

  if ( ( retransmit_flag == NULL ) || ( ( * retransmit_flag ) == 0 ) ) {

    stop_timer_events () ;

    delay ( 500 ) ;

    pc_transmit_generic_packet ( "TK 0" ) ;                  // Tell all devices to shut up

    delay ( 500 ) ;

    pc_transmit_generic_packet ( "TK 0" ) ;                  // Tell all devices to shut up

    delay ( 500 ) ;

    dll_delete_dll_fifo_records () ;                      
    dll_set_highest_record_number_in_database ( NULL ) ;

  endif

  else if ( ( * retransmit_flag ) == 1 ) {

    if ( strcmpi ( c.database_extension, ".XLS" ) != 0 ) {

      MessageBox ( GetFocus (), "You need to manually delete data from the raw timer table.",      NULL, MB_OK ) ;

    endif

    else {

      MessageBox ( GetFocus (), "You need to manually delete data from the raw data spreadsheet.", NULL, MB_OK ) ;

    endif

  endif

  else {

    pc_transmit_generic_packet ( "TK 0" ) ;                  // Tell all devices to shut up

    delay ( 500 ) ;

    dll_reset_all_timer_record_counters_to_zero () ;

    start_timer_events () ;

  endif

  return 1 ;

}

/*****************************************************************************************************************************/
/*****************************************************************************************************************************/

long DLL_EXPORT __stdcall dll_get_msec_since_last_communication ( long * device ) /* function declaration */ 

{

  long return_value ;
  long xtime ;

  xtime = time_of_last_reception_array [ (int) ( * device ) ] ;

  if ( xtime == 0 ) {
    return_value = -1 ;
  endif

  else {
    return_value = GetTickCount () - xtime ;
  endif

  return return_value ;

}

/*****************************************************************************************************************************/
/*****************************************************************************************************************************/
/*****************************************************************************************************************************/

void pc_transmit_generic_packet ( char * ptr_to_string_buffer ) /* function declaration */

{

  if ( comm_port_initialized_flag != 0 ) {
    transmit_generic_packet ( ptr_to_string_buffer ) ;
  endif

}

/*****************************************************************************************************************************/
/*****************************************************************************************************************************/
