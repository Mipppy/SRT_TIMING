

/************************************************************************************************************/
/************************************************************************************************************/

#define DLL_EXPORT _declspec ( dllexport )

/************************************************************************************************************/
/************************************************************************************************************/

#if ( 0 ) 

// The simplest code in 'C' would be the following:

// First, declare variables

  long   initialize_method      ;
  long   synch_method           ;
  long   comm_port              ;
  long   number_of_timers       ;

  char   race_directory [ 200 ] ; 

  long   app                    ;
  long   table_id               ;
  long   device_num             ;
  long   record_num             ;
  long   event_num              ;
  long   heat_num               ;
  long   channel                ;
  char   record_typ             ;   
  char   userstring      [ 20 ] ;
  char   user1_string    [ 20 ] ;
  char   user2_string    [ 20 ] ;
  char   user3_string    [ 20 ] ;
  char   user4_string    [ 20 ] ;
  char   bib_string      [ 20 ] ;
  char   timer_time      [ 20 ] ;
  char   pc_time         [ 20 ] ;
  char   notes           [ 20 ] ;

/*********/

// Then make function calls to the dll

  strcpy ( race_directory, "" ) ;                                       // Setting race_directory = "" puts log file in the current working directory
  initialize_method = 0     ;                                           // Setting initialize_method = 0 causes .INI file to be ignored.

  dll_initialize_dll_task ( & initialize_method, race_directory ) ;                    

  number_of_timers  = 4 ;
  dll_set_number_of_timers ( & number_of_timers ) ; 

  comm_port         = 1 ;
  dll_set_comm_port ( & comm_port ) ;                                   // Set COMM port. Default is 1.

  dll_start_communicating_with_timers () ;

  dll_reset_timers () ;

  synch_method      = 0 ;                                               // Synch  timers to PC Time
  dll_synch_timers ( & synch_method, "" ) ;                             

  dll_disable_timer_reset () ;                                          // Disables any further software resets from PC


  app       = 0 ;

// Then call the following function until the race is over.

  dll_get_next_timer_record (                                          
          & app, 
          & table_id, 
          & device_num,
          & record_num,
          & event_num,
          & heat_num,
          & channel,
          & record_typ,               
          & userstring,
          & user1_string,
          & user2_string,
          & user3_string,
          & user4_string,
          & bib_string,
          & timer_time,
          & pc_time,
          & notes                ) ; 
 
  dll_stop_communicating_withc_timers () ;

  dll_exit_routine ( "" ) ;

#endif

/************************************************************************************************************/
/************************************************************************************************************/

#ifdef __cplusplus
extern "C" {  // only need to export C interface if
              // used by C++ source code
#endif

/************************************************************************************************************/
/************************************************************************************************************/

// Here, all function prototypes are given.

// The first group of functions are called before communications begin

// The following function initializes the timer dll.
// It is necessary to call this function first
//     If read_ini_file_flag = 0, then the dll uses default configuration values
//     If read_ini_file_flag = 1, then the dll overwrites  the default configuration parameters with values from the .INI file if the .INI file exists
//     If read_ini_file_flag = 2, then the dll generates a .INI file with default values if the .INI file does not already exist.

//     ptr_to_race_directory is a string with the path to the race directory where all files are written


long DLL_EXPORT __stdcall dll_initialize_dll_task ( long * read_ini_file_flag,  char * ptr_to_race_directory ) ; 


// The following function does the obvious

long DLL_EXPORT __stdcall dll_set_number_of_timers ( long * number_of_timers ) ; 


// The following function does the obvious thing (1 for COMM1, 2 for COMM2, etc).  Default is COMM 1

long DLL_EXPORT __stdcall dll_set_comm_port ( long * comm_port ) ; 
        

// The following function sets talk time (msec). Talk time is the time duration, where a particular device has the token and can talk on the RS485 line.  Default is 400 msec.

long DLL_EXPORT __stdcall dll_set_talk_time ( long * talk_time ) ; 

// The following functions do the obvious thing

long DLL_EXPORT __stdcall dll_set_string_delimiter ( long * string_delimiter ) ;   // Default is terminating strings with '!!'

long DLL_EXPORT __stdcall dll_disable_left_justification_of_time_strings () ;    // Default is right justificaiton, so 9:00:00 and 10:00:00 line up
long DLL_EXPORT __stdcall dll_enable_left_justification_of_time_strings  () ;    // Gets rid of leading spaces (which confuse Excel)

long DLL_EXPORT __stdcall dll_set_diagnostic_flags ( long * diagnostic_flags ) ; 

long DLL_EXPORT __stdcall dll_set_highest_record_received_for_this_device ( long * q_device, long * highest_record_already_received ) ; 

long DLL_EXPORT __stdcall dll_set_highest_record_number_in_database ( long * highest_record_number ) ; 

long DLL_EXPORT __stdcall dll_save_ini_file ( void ) ; 

/************************************************************************************************************/

// The following functions are called after communications begin


// The following function starts communicating with the timers

long DLL_EXPORT __stdcall dll_start_communicating_with_timers ( void ) ; 


// The following function resets all of the timers from the PC

long DLL_EXPORT __stdcall dll_reset_timers ( void ) ;


// The following function synchs all of the timers.
//     Set synch_timers_flag = 0 for synching to the PC clock
//     Set synch_timers_flag = 1 for synching to a wristwatch.                      The synch_time_string contains the time to synch to
//     Set synch_timers_flag = 2 for adjusting the synch time by a certain amount.  The synch_time_string contains the amount of adjustment

long DLL_EXPORT __stdcall dll_synch_timers ( long * synch_timers_flag, char * synch_time_string ) ; 


// The following function disables the ability to reset the timers from the PC

long DLL_EXPORT __stdcall dll_disable_timer_reset ( void ) ; 


// The following function sends event and heat information to the timers.
//     If device = 0, then the event and heat information is sent to all timers

long DLL_EXPORT __stdcall dll_set_event_and_heat ( long * device, long * event, long * heat ) ; 
 

//     It is not necessary to call this function, unless you want the timers to retransmit all of their data

long DLL_EXPORT __stdcall dll_reset_all_timer_record_counters_to_zero ( void ) ; 


// The following function empties out the backup text file

long DLL_EXPORT __stdcall dll_delete_records_in_backup_text_file ( void ) ; 

 
// The following function gets timing data
// The following function returns a 0 when there is no data to retrieve from the dll. 
//                     It returns a 1 when there is    data to retrieve from the dll. 


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
        char  * notes                ) ; 


// The following function deletes all dll records from the fifo

long DLL_EXPORT __stdcall dll_delete_dll_fifo_records ( void ) ; 
                
// The following function be called before exiting.
// It can also be called if an error condition occurs.

long DLL_EXPORT __stdcall dll_exit_routine ( char * exit_string ) ; 


// The following function gets ALL ascii characters that have been sent back by ALL timers.  This supports a "dumb" terminal for viewing
// This function returns -1 when no data is available, otherwise, it returns the ASCII character

long DLL_EXPORT __stdcall dll_get_character_from_terminal_fifo ( void ) ; 


// The following function stops communicating with the timers

long DLL_EXPORT __stdcall dll_stop_communicating_with_timers ( void ) ; 

/*****************************************************************************************************************************/

// The following functions provide useful diagnostics


// The following function can be used to test the software without having any timers connected

long DLL_EXPORT __stdcall dll_generate_dummy_record  ( void )  ; 


// The following function returns x1 + x2.   x1 is set to 2 * x1,  x2 is set to 4 * x1

long DLL_EXPORT __stdcall dll_test_function_call_passing_long ( long * x1, long* x2 ) ; 


// The following function returns 314159, and 

long DLL_EXPORT __stdcall dll_test_function_call_passing_string ( char * string1, char * string2 ) ; 

/*****************************************************************************************************************************/
/*****************************************************************************************************************************/

// The following function tests the dll call

long DLL_EXPORT __stdcall dll_print_timing_structure ( char * tmp_file_path_and_name, char * heading_string, XC_TIMER_RECORD_STRUCTURE_TYPE * x ) ; 


// The following function sets variables back to their default values
// It is not necessary to call this function

long DLL_EXPORT __stdcall dll_assign_default_values ( void ) ; 


// The following function does the obvious thing.  Default is 9600.  That is the only baud rate currently supported

long DLL_EXPORT __stdcall dll_set_baud_rate ( long * baud_rate ) ; 


// This function returns the number of msec since last communicaiton from this device

long DLL_EXPORT __stdcall dll_get_msec_since_last_communication ( long * device ) ; 

/************************************************************************************************************/
/************************************************************************************************************/
















#ifdef __cplusplus
}
#endif

/************************************************************************************************************/
/************************************************************************************************************/
