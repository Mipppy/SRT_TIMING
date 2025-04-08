
/************************************************************************************************************/
/************************************************************************************************************/

#if   defined ( _USRDLLx )
  #define xxDLL_EXPORT _declspec ( dllexport )
#endif
#if ! defined ( _USRDLLx )
  #define xxDLL_EXPORT
#endif

#if   defined ( _USRDLL )
#endif

#define DLL_EXPORT _declspec ( dllexport )

/************************************************************************************************************/
/************************************************************************************************************/

#define RELEASE_DATE 0x090604

/************************************************************************************************************/
/************************************************************************************************************/

#if ( RELEASE_DATE == 0x090604 )

  #define DAQ_SOFTWARE_VERSION_DATE_STRING          "6/04/09"
  #define DAQ_SOFTWARE_VERSION_TIME_STRING          "9:54"
  #define DAQ_SOFTWARE_VERSION_NUMBER_STRING        "1.6.2"

#endif

#if ( RELEASE_DATE == 0x061203 )

  #define DAQ_SOFTWARE_VERSION_DATE_STRING          "12/03/06"
  #define DAQ_SOFTWARE_VERSION_TIME_STRING          "13:28"
  #define DAQ_SOFTWARE_VERSION_NUMBER_STRING        "1.6.0"

#endif

#if ( RELEASE_DATE  < 0x061203 )

  #define DAQ_SOFTWARE_VERSION_DATE_STRING          "12/30/03"
  #define DAQ_SOFTWARE_VERSION_TIME_STRING          "14:38"
  #define DAQ_SOFTWARE_VERSION_NUMBER_STRING        "1.5.57"

#endif

/************************************************************************************************************/

#define MAX_NUMBER_OF_DATABASE_TEXT_BOX_LINES   10
#define MAX_NUMBER_OF_TERMINAL_TEXT_BOX_LINES   10

#define MAX_NUMBER_OF_TIMER_FIFO_ENTRIES      2000  

#define MAX_NUMBER_OF_APPS                      10

#define MAX_NUMBER_OF_DEVICES                   32

#define MAX_NUMBER_OF_USER_STRING_FIELDS        10

/***************************************************************************/
/***************************************************************************/

// Configuration Structure 10 members

typedef struct {

  #if ( RELEASE_DATE >= 0x090604 )
    unsigned int   serial_comm_port                          ;  // 1 for COMM1, etc
  #endif
  #if ( RELEASE_DATE <  0x090604 )
    unsigned char  serial_comm_port                          ;  // 1 for COMM1, etc
  #endif

  unsigned long  baud_rate                                   ;

  unsigned short number_of_devices_used                      ;

  unsigned short talk_time                                   ;

  char           string_delimiter                            ;
  unsigned char  left_justify_time_strings_flag              ;

  long           password                                    ;

  unsigned       diagnostic_flags                            ;

  unsigned long  database_highest_record_number              ;     // Auto Number in Access

  unsigned short highest_record_received_array [ MAX_NUMBER_OF_DEVICES ] ;

  char           timer_type_used                     [ 100 ] ;

  char           database_type                       [ 100 ] ;
  char           database_drive                      [ 100 ] ;
  char           database_directory                  [ 400 ] ;
  char           database_name                       [ 400 ] ;
  char           database_extension                  [ 400 ] ;
  char           database_table_name                 [ 400 ] ;

  unsigned char  odbc_user_dsn_flag                          ;
  char           database_odbc_user_data_source_name [ 400 ] ;

  unsigned int   event_number                                ;
  unsigned int   heat_number                                 ;

} XC_CONFIGURE_STRUCTURE_TYPE ;

/***************************************************************************/
/***************************************************************************/

// Global Variables Structure 10 members

typedef struct {
          
  char           version_number_string          [ 200 ] ;
  char           version_time_string            [ 200 ] ;
  char           version_date_string            [ 200 ] ;

  char           race_directory                 [ 500 ] ;        // Directory with Race Files
                 
  char           database_path_and_file_name    [ 500 ] ;
                               
  char           current_working_directory      [ 500 ] ;        // Directory where Application Starts
  char           log_file_path_and_name         [ 500 ] ;
  char           tx_file_path_and_name          [ 500 ] ;
  char           rx_file_path_and_name          [ 100 ] ;
  char           ini_file_path_and_name         [ 100 ] ;
  char           backup_file_path_and_name      [ 100 ] ;

  unsigned char  srt_1000_used_flag                         ;
  unsigned char  tag_heuer_505_used_flag                    ;
  unsigned char  tag_heuer_605_used_flag                    ;
  unsigned char  hora_targets_used_flag                     ;

  int            timer_event_interval   ;
  int            timer_event_resolution ;


} GLOBAL_VARIABLE_STRUCTURE_TYPE ;

/***************************************************************************/

// The following is from the database table definition

typedef struct {

  unsigned long  table_id                                                 ;
  unsigned short device_num                                               ;
  unsigned long  record_num                                               ;
  unsigned short event_num                                                ;
  unsigned short heat_num                                                 ;
  unsigned short channel                                                  ;

  char           record_typ                                        [ 20 ] ;
  char           userstring                                        [ 20 ] ;
  char           userxfield   [ MAX_NUMBER_OF_USER_STRING_FIELDS ] [ 20 ] ;
  char           bib_string                                        [ 20 ] ;
  char           timer_time                                        [ 20 ] ;
  char           pc_time                                           [ 20 ] ;
  char           notes                                             [ 50 ] ;
 
} XC_TIMER_RECORD_STRUCTURE_TYPE ;

/************************/

enum {

SYNCH_TIMERS_TO_SYSTEM_CLOCK  = 0 , 
SYNCH_TIMERS_TO_USER_CLOCK        ,
SYNCH_TIMERS_ADJUSTMENT

} ;


// The following structure is an array of structures

typedef struct {

  short timer_fifo_read_index   ;
  short timer_fifo_write_index  ;
  short total_number_of_entries ;
  short overrun_flag ;

  XC_TIMER_RECORD_STRUCTURE_TYPE timer_sub_structure [ MAX_NUMBER_OF_TIMER_FIFO_ENTRIES ] ;

} TIMER_ARRAY_OF_STRUCTURES_TYPE ;

/***************************************************************************/

#ifdef __cplusplus
extern "C" {  // only need to export C interface if
              // used by C++ source code
#endif

/*****************************************************************************************************************************/
/*****************************************************************************************************************************/

// The following function gets the pointer to the configuration structure.
// It is not necessary to call this function
// The programmer can either directly update members of this structure, or call functions which update the structure

XC_CONFIGURE_STRUCTURE_TYPE DLL_EXPORT * __stdcall dll_get_pointer_to_configuration_structure ( void ) ; 

// The following function gets the pointer to the global variable structure which can be used to pass parameters between dll and calling application.

GLOBAL_VARIABLE_STRUCTURE_TYPE DLL_EXPORT * __stdcall dll_get_pointer_to_global_variable_structure ( void ) ; 

                          
// The following function returns a 0 when there is no data to retrieve from the dll. 
//                     It returns a 1 when there is    data to retrieve from the dll. 
//     Data is put into the structure

long DLL_EXPORT __stdcall dll_get_next_timer_structure     ( long * application, XC_TIMER_RECORD_STRUCTURE_TYPE * d ) ; 
long DLL_EXPORT __stdcall dll_get_next_timer_structure_old ( long * application, XC_TIMER_RECORD_STRUCTURE_TYPE * d ) ; 

void DLL_EXPORT __stdcall dll_put_timer_structure_into_fifo ( XC_TIMER_RECORD_STRUCTURE_TYPE * s ) ; /* function declaration */

// The following sets up the database parameters

long DLL_EXPORT __stdcall dll_set_database_parameters ( char * database_type, char * database_drive, char * database_directory, char * database_name, char * database_extension, char * database_table_name ) ; 

long DLL_EXPORT __stdcall dll_set_timer_type_used ( char * timer_type ) ; /* function declaration */
long DLL_EXPORT __stdcall dll_set_password        ( long * password   ) ; /* function declaration */

long DLL_EXPORT __stdcall dll_retransmit_data ( long * flag )  ; /* function declaration */

/*****************************************************************************************************************************/
/*****************************************************************************************************************************/

// The following functions are in the odbc dll

long DLL_EXPORT __stdcall odbc_write_file_dsn              ( void ) ; 
long DLL_EXPORT __stdcall odbc_open_database_connection    ( XC_CONFIGURE_STRUCTURE_TYPE * tmp_c, GLOBAL_VARIABLE_STRUCTURE_TYPE * tmp_g ) ;
long DLL_EXPORT __stdcall odbc_delete_all_database_records ( void ) ; 
long DLL_EXPORT __stdcall odbc_sql_append_database_record  ( XC_TIMER_RECORD_STRUCTURE_TYPE * d ) ;
long DLL_EXPORT __stdcall odbc_close_database_connection   ( void ) ;

/*****************************************************************************************************************************/

long comm_write_string( unsigned char *ptr_to_tx_info, long length );

void get_characters_from_timers ( void ) ; 
void transmit_generic_packet ( char * ptr_to_string_buffer ) ; 
void output_formatted_file_string ( char * file_path_and_name, char * format_string,... ) ; 
void comm_close ( void ) ; 
void write_timer_structure_to_backup_file ( XC_TIMER_RECORD_STRUCTURE_TYPE * s ) ; 
int get_file_path_and_name_from_parts ( char * file_path_and_name, char * path, char * file_name, char * file_extension  ) ; 
void stuff_character_into_terminal_fifo ( char character ) ; 

void static stop_timer_events ( void ) ;
void static start_timer_events ( void ) ;


void copy_generic_structure ( unsigned char * d, unsigned char * s, int number_of_bytes ) ; 
void read_all_ini_parameters ( int read_ini_file_flag, char * tmp_ini_file_path_and_name ) ; 
void parse_ini_file ( char * command_string ) ; 
int local_comm_open ( int local_comm_port, long local_baud_rate ) ; 
void delay ( unsigned int delay_time ) ; 
void comm_error_handler ( char * message ) ; 

void put_timer_structure_into_tmp_fifo     ( XC_TIMER_RECORD_STRUCTURE_TYPE * s ) ; 

void create_backup_text_file ( long delete_flag ) ; 

void pc_transmit_generic_packet ( char * ptr_to_string_buffer ) ; 

int ernies_comm_read_byte ( void ) ; /* function declaration */

#ifdef __cplusplus
}
#endif



/***************************************************************************/





   