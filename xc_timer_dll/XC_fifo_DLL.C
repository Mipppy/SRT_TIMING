
/************************************************************************/
/************************************************************************/

#define XC_FIFO_DLL_MODULE 1

#include <x_header.h>
#include <win_header.h>

/*********************************************************************/
/*********************************************************************/

// Note, can only share initialized data segment !!!!!!!!!!!!!!!!

#if   defined ( MAIN_MODULE )
  #define XX_PREDEFINE_ZERO = { 0 } ;
#endif

#if ! defined ( MAIN_MODULE )
  #define XX_PREDEFINE_ZERO /* */
#endif

#ifdef XC_FIFO_DLL_MODULE
  #pragma comment ( linker, "/SECTION:.shared,RWS" )
  #pragma data_seg ( ".shared" )
#endif

/*********************************************************************/
/*********************************************************************/

extern XC_CONFIGURE_STRUCTURE_TYPE        c ;
extern GLOBAL_VARIABLE_STRUCTURE_TYPE     g ;

BOOL dll_initialized_flag = 0 ;

BOOL write_thread_wants_to_preempt_flag ; 

BOOL read_thread_in_progress_flag ;

char xxlog_file_path_and_name [ 400 ] = { 0 } ;

int timer_fifo_write_index  = 0 ;

int timer_fifo_read_index [ MAX_NUMBER_OF_APPS ] = { 0 } ;
int app_used_flag_array   [ MAX_NUMBER_OF_APPS ] = { 0 } ;
int overrun_flag_array    [ MAX_NUMBER_OF_APPS ] = { 0 } ;

TIMER_ARRAY_OF_STRUCTURES_TYPE   timer_array_of_structures     = { 0 } ;


// The following is the actual pointer that I use

XC_TIMER_RECORD_STRUCTURE_TYPE *  ptr_to_timer_data_array_of_structures = NULL ;

/***********************************************************************/

#ifdef XC_FIFO_DLL_MODULE
  #pragma data_seg ()
#endif

/*********************************************************************/
/*********************************************************************/
/*********************************************************************/

#if ( 0 )

// Defines the entry point for the DLL application.

BOOL APIENTRY DllMain ( HANDLE hModule, 
                        DWORD  ul_reason_for_call, 
                        LPVOID lpReserved  )

{

  char scratch_string [ 400 ] ;

  switch (ul_reason_for_call) {

    case DLL_PROCESS_ATTACH:

      if ( dll_initialized_flag == 0 ) {
 
        dll_initialized_flag = 1 ;

        write_thread_wants_to_preempt_flag = 0 ;
        read_thread_in_progress_flag       = 0 ;

        get_file_path_and_name_from_parts ( log_file_path_and_name, g . race_directory, "srt_dll_log",  ".txt" ) ;

        output_formatted_file_string ( log_file_path_and_name, NULL ) ;
        output_formatted_file_string ( log_file_path_and_name, "\nSRT DLL Log Here at %s\n",  _strtime ( scratch_string ) ) ;
        output_formatted_file_string ( log_file_path_and_name, "\nCurrent Directory is %s\n", _getcwd ( scratch_string, 1000 ) ) ;

      endif

    break;
    case DLL_THREAD_ATTACH:
      output_formatted_file_string ( log_file_path_and_name, "DLL Thread Attach\n" ) ;
    break;
    case DLL_THREAD_DETACH:
      output_formatted_file_string ( log_file_path_and_name, "DLL Thread Detach\n" ) ;
    break;
    case DLL_PROCESS_DETACH:
    break;

  end_switch 

  return TRUE;

}

#endif

/**********************************************************/

long DLL_EXPORT __stdcall dll_delete_dll_fifo_records ( void ) /* function declaration */

{

  int index ;

  write_thread_wants_to_preempt_flag = 1 ; 

  while ( read_thread_in_progress_flag != 0 ) ;

  timer_fifo_write_index  = 0 ;

  for ( index = 0 ; index < MAX_NUMBER_OF_APPS ; index ++ ) {

    timer_fifo_read_index [ index ] = 0 ;
    overrun_flag_array    [ index ] = 0 ;

  next

  write_thread_wants_to_preempt_flag = 0 ; 
  return 1 ;

}

/**********************************************************/

// This function is called from packets.c, and when we want to stuff a dummy record.
// Here we are passed timer data, and write it to a file, and to fifos

// This was modified 11/20/03 by adding DLL_EXPORT

void DLL_EXPORT __stdcall dll_put_timer_structure_into_fifo ( XC_TIMER_RECORD_STRUCTURE_TYPE * s ) /* function declaration */

{

  unsigned short app ;
  XC_TIMER_RECORD_STRUCTURE_TYPE * d ;

// Here we write to fifo for win_app

  if ( c.left_justify_time_strings_flag != 0 ) {
    remove_leading_and_trailing_white ( s-> pc_time    ) ;
    remove_leading_and_trailing_white ( s-> timer_time ) ;
  endif

  ptr_to_timer_data_array_of_structures = ( XC_TIMER_RECORD_STRUCTURE_TYPE * ) ( & ( timer_array_of_structures . timer_sub_structure [ 0 ] ) ) ;

  d = & ( ptr_to_timer_data_array_of_structures [ timer_fifo_write_index ] ) ;

  copy_generic_structure ( (unsigned char *) d, (unsigned char *) s, sizeof ( XC_TIMER_RECORD_STRUCTURE_TYPE ) ) ;

  if ( ++ timer_fifo_write_index >= MAX_NUMBER_OF_TIMER_FIFO_ENTRIES ) {
    timer_fifo_write_index = 0 ;
  endif

  for ( app = 0 ; app < MAX_NUMBER_OF_APPS ; app ++ ) {

    if ( app_used_flag_array [ app ] != 0 ) {

      if ( timer_fifo_write_index == timer_fifo_read_index [ app ] ) {
        overrun_flag_array [ app ] = 1 ;
        if ( ++ timer_fifo_read_index [ app ] >= MAX_NUMBER_OF_TIMER_FIFO_ENTRIES ) {
          timer_fifo_read_index [ app ] = 0 ;
        endif

      endif

    endif

  next

  write_timer_structure_to_backup_file ( s ) ;

}

/*****************************************************************************************************************************/
/*****************************************************************************************************************************/

long DLL_EXPORT __stdcall dll_get_next_timer_record ( 

        long  * app, 
        long  * table_id,
        long  * device_num,
        long  * record_num,
        long  * event_num,
        long  * heat_num,
        long  * channel,
        char  * record_typ,
        char  * userstring,
        char  * user1_string,
        char  * user2_string,
        char  * user3_string,
        char  * user4_string,
        char  * bib_string,
        char  * timer_time,
        char  * pc_time,
        char  * notes                   ) /* function declaration */

{
 
  int   local_app ;
  long  return_flag ;
  char  delimiter_string [ 10 ] ;

  XC_TIMER_RECORD_STRUCTURE_TYPE * s ;

  return_flag = 0 ;
 
  if ( write_thread_wants_to_preempt_flag == 0 ) {

    read_thread_in_progress_flag = 1 ;

    if ( write_thread_wants_to_preempt_flag == 0 ) {

      if ( app != NULL ) {
        local_app = (int) ( * app ) ;
      endif
          else {
        local_app = 0 ;
      endif

      
	  app_used_flag_array [ local_app ] = 1 ;               

      if ( timer_fifo_write_index != timer_fifo_read_index [ local_app ] ) {

        return_flag = 1 ; 

        ptr_to_timer_data_array_of_structures = ( XC_TIMER_RECORD_STRUCTURE_TYPE * ) ( & ( timer_array_of_structures . timer_sub_structure [ 0 ] ) ) ;

        s = & ( ptr_to_timer_data_array_of_structures [ timer_fifo_read_index [ local_app ] ] ) ;

        * table_id                     = (long)  s -> table_id                   ;
        * device_num                   = (long)  s -> device_num                 ;
        * record_num                   = (long)  s -> record_num                 ;
        * event_num                    = (long)  s -> event_num                  ;
        * heat_num                     = (long)  s -> heat_num                   ;
        * channel                      = (long)  s -> channel                    ;

        strcpy ( record_typ                   ,  s -> record_typ           ) ;
        strcpy ( userstring                   ,  s -> userstring           ) ;
        strcpy ( user1_string                 ,  s -> userxfield [ 0 ]     ) ;
        strcpy ( user2_string                 ,  s -> userxfield [ 1 ]     ) ;
        strcpy ( user3_string                 ,  s -> userxfield [ 2 ]     ) ;
        strcpy ( user4_string                 ,  s -> userxfield [ 3 ]     ) ;
        strcpy ( bib_string                   ,  s -> bib_string           ) ;
        strcpy ( timer_time                   ,  s -> timer_time           ) ;
        strcpy ( pc_time                      ,  s -> pc_time              ) ;
        strcpy ( notes                        ,  s -> notes                ) ;

        if ( c.string_delimiter != 0 ) {

          delimiter_string [ 0 ] = c.string_delimiter ;
          delimiter_string [ 1 ] = '\0' ;

          strcat ( record_typ                 , delimiter_string      ) ;
          strcat ( userstring                 , delimiter_string      ) ;
          strcat ( user1_string               , delimiter_string      ) ;
          strcat ( user2_string               , delimiter_string      ) ;
          strcat ( user3_string               , delimiter_string      ) ;
          strcat ( user4_string               , delimiter_string      ) ;
          strcat ( bib_string                 , delimiter_string      ) ;
          strcat ( timer_time                 , delimiter_string      ) ;
          strcat ( pc_time                    , delimiter_string      ) ;
          strcat ( notes                      , delimiter_string      ) ;

        endif

        if ( ++ ( timer_fifo_read_index [ local_app ] ) >= MAX_NUMBER_OF_TIMER_FIFO_ENTRIES ) timer_fifo_read_index [ local_app ] = 0 ;

      endif

    endif

  endif

  read_thread_in_progress_flag = 0 ;

  return return_flag ;

}

/*****************************************************************************************************************************/
/*****************************************************************************************************************************/
/*****************************************************************************************************************************/

// Heading string is to tell exactly why we are printing the structure

long DLL_EXPORT __stdcall dll_print_timing_structure ( char * tmp_file_path_and_name, char * heading_string, XC_TIMER_RECORD_STRUCTURE_TYPE * x ) /* function declaration */

{

  output_formatted_file_string ( tmp_file_path_and_name, "Heading: %s  structure_address=%8ld\n", heading_string, (long) x              ) ;

  output_formatted_file_string ( tmp_file_path_and_name, "   table_id           =%8ld\n", x -> table_id        ) ;
  output_formatted_file_string ( tmp_file_path_and_name, "   device_num         =%8d\n",  x -> device_num      ) ;
  output_formatted_file_string ( tmp_file_path_and_name, "   record_num         =%8ld\n", x -> record_num      ) ;
  output_formatted_file_string ( tmp_file_path_and_name, "   event_num          =%8ld\n", x -> event_num       ) ;
  output_formatted_file_string ( tmp_file_path_and_name, "   heat_num           =%8ld\n", x -> heat_num        ) ;
  output_formatted_file_string ( tmp_file_path_and_name, "   channel            =%8d\n",  x -> channel         ) ;

  output_formatted_file_string ( tmp_file_path_and_name, "   record_typ         = %s\n", x -> record_typ       ) ;
  output_formatted_file_string ( tmp_file_path_and_name, "   total_user_string  = %s\n", x -> userstring       ) ;
  output_formatted_file_string ( tmp_file_path_and_name, "   user1_string       = %s\n", x -> userxfield [ 0 ] ) ;
  output_formatted_file_string ( tmp_file_path_and_name, "   user2_string       = %s\n", x -> userxfield [ 1 ] ) ;
  output_formatted_file_string ( tmp_file_path_and_name, "   user3_string       = %s\n", x -> userxfield [ 2 ] ) ;
  output_formatted_file_string ( tmp_file_path_and_name, "   user4_string       = %s\n", x -> userxfield [ 3 ] ) ;

  output_formatted_file_string ( tmp_file_path_and_name, "   bib_string         = %s\n", x -> bib_string  ) ;
  output_formatted_file_string ( tmp_file_path_and_name, "   timer_time         = %s\n", x -> timer_time  ) ;
  output_formatted_file_string ( tmp_file_path_and_name, "   pc_time            = %s\n", x -> pc_time     ) ;
  output_formatted_file_string ( tmp_file_path_and_name, "   notes              = %s\n\n\n", x -> notes   ) ;
 
  return 1 ;

}                                                      

/*****************************************************************************************************************************/

void copy_generic_structure ( unsigned char * d, unsigned char * s, int number_of_bytes ) /* function declaration */

{

  int index ;
  unsigned char * destination_pointer ;
  unsigned char * source_pointer ;

  destination_pointer = (unsigned char *) d ;
  source_pointer      = (unsigned char *) s ;

  for ( index = 0 ; index < number_of_bytes ; index ++ ) {
    * destination_pointer ++ = * source_pointer ++ ;
  next

}

/*****************************************************************************************************************************/
/*****************************************************************************************************************************/
/*****************************************************************************************************************************/

long DLL_EXPORT __stdcall dll_get_next_timer_structure_old ( long * app, XC_TIMER_RECORD_STRUCTURE_TYPE * d ) /* function declaration */

{

  int   local_app ;
  long  return_flag ;

  XC_TIMER_RECORD_STRUCTURE_TYPE * s ;
 

  return_flag = 0 ;
 
  if ( write_thread_wants_to_preempt_flag == 0 ) {

    read_thread_in_progress_flag = 1 ;

    if ( write_thread_wants_to_preempt_flag == 0 ) {

      if ( app == NULL ) {
        local_app = 0 ;
      endif
      else {
        local_app = (int) ( * app ) ;
      endif

      app_used_flag_array [ local_app ] = 1 ;               

      if ( timer_fifo_write_index != timer_fifo_read_index [ local_app ] ) {

        return_flag = 1 ; 

        ptr_to_timer_data_array_of_structures = ( XC_TIMER_RECORD_STRUCTURE_TYPE * ) ( & ( timer_array_of_structures . timer_sub_structure [ 0 ] ) ) ;

        s = & ( ptr_to_timer_data_array_of_structures [ timer_fifo_read_index [ local_app ] ] ) ;

        copy_generic_structure ( (unsigned char *) d, (unsigned char *) s, sizeof ( XC_TIMER_RECORD_STRUCTURE_TYPE ) ) ;

        if ( ++ ( timer_fifo_read_index [ local_app ] ) >= MAX_NUMBER_OF_TIMER_FIFO_ENTRIES ) timer_fifo_read_index [ local_app ] = 0 ;

      endif

    endif

  endif

  read_thread_in_progress_flag = 0 ;

  return return_flag ;

}

/*****************************************************************************************************************************/
/*****************************************************************************************************************************/

// This uses dll_get_next_timer_record routine, same as as Geoff and Tom

long DLL_EXPORT __stdcall dll_get_next_timer_structure ( long * app, XC_TIMER_RECORD_STRUCTURE_TYPE * d ) /* function declaration */

{

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

  if ( dll_get_next_timer_record ( 

           app, 
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
           notes                   
        
      ) != 0 ) {

    d -> table_id      = (long)   table_id     ;
    d -> device_num    = (int)    device_num   ;
    d -> record_num    = (int)    record_num   ;
    d -> event_num     = (int)    event_num    ;
    d -> heat_num      = (int)    heat_num     ;
    d -> channel       = (int)    channel      ;

    strcpy ( d -> record_typ        ,  record_typ      ) ;
    strcpy ( d -> userstring        ,  userstring      ) ;
    strcpy ( d -> userxfield [ 0 ]  ,  user1_string    ) ;
    strcpy ( d -> userxfield [ 1 ]  ,  user2_string    ) ;
    strcpy ( d -> userxfield [ 2 ]  ,  user3_string    ) ;
    strcpy ( d -> userxfield [ 3 ]  ,  user4_string    ) ;
    strcpy ( d -> bib_string        ,  bib_string      ) ;
    strcpy ( d -> timer_time        ,  timer_time      ) ;
    strcpy ( d -> pc_time           ,  pc_time         ) ;
    strcpy ( d -> notes             ,  notes           ) ;

 
    return 1 ;

  endif

  else {
    return 0 ;
  endif


}

/****************************************************************************************************/
/****************************************************************************************************/

// This function returns x1 + x2.
//      x1 is set to 2 * x1,  x2 is set to 4 * x1

long DLL_EXPORT __stdcall dll_test_function_call_passing_long ( long * x1, long * x2 ) /* function declaration */

{

  long return_flag ;

  return_flag = ( * x1 ) + ( * x2 ) ;

  * x1 = ( * x1 ) * 2 ;
  * x2 = ( * x2 ) * 4 ;

  return return_flag ;

}

/****************************************************************************************************/
/****************************************************************************************************/

#if ( 0 ) 

TIMER_ARRAY_OF_STRUCTURES_TYPE   timer_data_tmp_structure ;

void DLL_EXPORT __stdcall dll_put_timer_structure_into_tmp_fifo ( XC_TIMER_RECORD_STRUCTURE_TYPE * s ) /* function declaration */

{

  XC_TIMER_RECORD_STRUCTURE_TYPE * d ;
 
   s -> table_id = timer_data_tmp_structure . timer_fifo_write_index + 1 ;

   d = & ( timer_data_tmp_structure . timer_sub_structure [ ( timer_data_tmp_structure . timer_fifo_write_index ) ] ) ;

   copy_generic_structure ( (unsigned char *) d, (unsigned char *) s, sizeof ( XC_TIMER_RECORD_STRUCTURE_TYPE ) ) ;

   timer_data_tmp_structure . total_number_of_entries ++ ;

   if ( ++ ( timer_data_tmp_structure . timer_fifo_write_index ) >= MAX_NUMBER_OF_TIMER_FIFO_ENTRIES ) {
     ( timer_data_tmp_structure . timer_fifo_write_index ) = 0 ;
   endif

   if ( ( timer_data_tmp_structure . timer_fifo_write_index ) == ( timer_data_tmp_structure . timer_fifo_read_index ) ) {
     timer_data_tmp_structure . overrun_flag = 1 ;
     if ( ++ ( timer_data_tmp_structure . timer_fifo_read_index ) >= MAX_NUMBER_OF_TIMER_FIFO_ENTRIES ) {
       timer_data_tmp_structure . timer_fifo_read_index = 0 ;
     endif
   endif

  write_timer_structure_to_backup_file ( s ) ;

}

/*****************************************************************************************************************************/

long DLL_EXPORT __stdcall dll_get_next_timer_structure_from_tmp_fifo ( XC_TIMER_RECORD_STRUCTURE_TYPE * d ) /* function declaration */

{

  long return_flag ;
  XC_TIMER_RECORD_STRUCTURE_TYPE * s ;

  return_flag = 0 ;

  if ( timer_data_tmp_structure . timer_fifo_write_index != timer_data_tmp_structure . timer_fifo_read_index ) {

    s = & ( timer_data_tmp_structure . timer_sub_structure [ ( timer_data_tmp_structure . timer_fifo_read_index ) ] ) ;

    copy_generic_structure ( (unsigned char *) d, (unsigned char *) s, sizeof ( XC_TIMER_RECORD_STRUCTURE_TYPE ) ) ;

    if ( ++ ( timer_data_tmp_structure . timer_fifo_read_index ) >= MAX_NUMBER_OF_TIMER_FIFO_ENTRIES ) {
      ( timer_data_tmp_structure . timer_fifo_read_index ) = 0 ;
    endif

    return_flag = 1 ;

  endif

  return return_flag ;

}

#endif

/*****************************************************************************************************************************/
/*****************************************************************************************************************************/

