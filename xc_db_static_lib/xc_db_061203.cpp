
/***************************************************************************/
/***************************************************************************/

// xc_timer_db.cpp :  This module appends records to the database

#include "stdafx.h"
#include "direct.h"  
#include "io.h"
#include "stdlib.h"

#include "xc_db.h"
#include "xc_timer_dll.h"

#include "cdef.h"

/***************************************************************************/

void get_sql_append_string ( char * sql_string, XC_TIMER_RECORD_STRUCTURE_TYPE * d ) ; /* function declaration */

/***************************************************************************/

static XC_CONFIGURE_STRUCTURE_TYPE       * ptr_c ;

static GLOBAL_VARIABLE_STRUCTURE_TYPE  * ptr_g ;

static int           debug_flag = 0 ;

static char          file_dsn_path_and_name [ 500 ] ;

static char          local_table_name [ 200 ] ;

/***************************************************************************/
/***************************************************************************/

/***************************************************************************/
/***************************************************************************/

CDatabase              cdatabase_class   ;

/***************************************************************************/
/***************************************************************************/

// Returns 1 when successful

long DLL_EXPORT __stdcall odbc_open_database_connection    ( XC_CONFIGURE_STRUCTURE_TYPE * tmp_c, GLOBAL_VARIABLE_STRUCTURE_TYPE * tmp_g ) /* function declaration */

{

  #if ( RELEASE_DATE >= 0x061203 )

    char tmp_odbc_data_source_name [ 400 ] ;
    char scratch_string            [ 400 ] ;
    int success_flag ;

    ptr_c = tmp_c ;
    ptr_g = tmp_g ;

    if ( ptr_c -> odbc_user_dsn_flag == 0 ) {
      _makepath  ( ptr_g -> database_path_and_file_name, ptr_c -> database_drive, ptr_c -> database_directory, ptr_c -> database_name, ptr_c -> database_extension ) ;

      if ( _access ( ptr_g -> database_path_and_file_name, 0 ) == 0 ) { 
        odbc_write_file_dsn () ;
        output_formatted_file_string ( ptr_g -> log_file_path_and_name, "Attempting to Open Database with File DSN from file %s\n", file_dsn_path_and_name ) ;
        sprintf ( tmp_odbc_data_source_name,  "FILEDSN=%s", file_dsn_path_and_name                 ) ;

      endif

      else {
        success_flag = 0 ;
        sprintf ( scratch_string, "Could not find database file %s",  ptr_g -> database_path_and_file_name ) ;
        MessageBox( NULL, scratch_string, "Error", ( MB_ICONERROR | MB_OK ) ) ;
      endif

    endif

    else {

      strcpy ( local_table_name, ptr_c-> database_table_name ) ;

      if ( ptr_c -> odbc_user_dsn_flag == 1 ) {
        output_formatted_file_string ( ptr_g -> log_file_path_and_name, "Attempting to Open Database with User DSN %s\n", ptr_c -> database_odbc_user_data_source_name ) ;
        sprintf ( tmp_odbc_data_source_name,  "DSN=%s",     ptr_c -> database_odbc_user_data_source_name  ) ;
      endif

      else if ( ptr_c -> odbc_user_dsn_flag == 2 ) {

        strcpy ( local_table_name, ptr_c-> database_table_name ) ;

        if ( _access ( ptr_c -> database_odbc_user_data_source_name, 0 ) == 0 ) { 
          output_formatted_file_string ( ptr_g -> log_file_path_and_name, "Attempting to Open Database with Custom File DSN from file %s\n", ptr_c -> database_odbc_user_data_source_name ) ;
          sprintf ( tmp_odbc_data_source_name,  "FILEDSN=%s", ptr_c -> database_odbc_user_data_source_name                 ) ;
        endif

        else {
          success_flag = 0 ;
          sprintf ( scratch_string, "Could not find dsn file %s",  ptr_c -> database_odbc_user_data_source_name  ) ;
          MessageBox( NULL, scratch_string, "Error", ( MB_ICONERROR | MB_OK ) ) ;
        endif

      endif


      else {
        success_flag = 0 ;
        sprintf ( scratch_string, "ODBC USER DSN Flag is set to invalid value (only 0-2 are valid)." ) ;
        MessageBox( NULL, scratch_string, "Error", ( MB_ICONERROR | MB_OK ) ) ;
      endif

    endif

    if ( success_flag != 0 ) {

      cdatabase_class . OpenEx ( tmp_odbc_data_source_name, 0 ) ;

      if ( cdatabase_class . IsOpen() == 0 ) {
        success_flag = 0 ;
        if ( ptr_c -> odbc_user_dsn_flag == 1 ) {
          MessageBox( NULL, "Could not Connect to Database.  Check ODBC Parameters in the Control Panel", "Error", ( MB_ICONERROR | MB_OK ) ) ;
        endif
        else {
          MessageBox( NULL, "Could not Connect to Database.  Check ODBC Parameters in the File DSN", "Error", ( MB_ICONERROR | MB_OK ) ) ;
        endif
      endif
      else {
        output_formatted_file_string ( ptr_g -> log_file_path_and_name, "Success Opening ODBC Database!\n" ) ;
        success_flag = 1 ;
      endif

    endif

    return success_flag ;

  #endif

  #if ( RELEASE_DATE <  0x061203 )

    char tmp_odbc_data_source_name [ 400 ] ;
    char scratch_string            [ 400 ] ;
    int return_value ;

    ptr_c = tmp_c ;
    ptr_g = tmp_g ;

    _makepath  ( ptr_g -> database_path_and_file_name, ptr_c -> database_drive, ptr_c -> database_directory, ptr_c -> database_name, ptr_c -> database_extension ) ;

    if ( _access ( ptr_g -> database_path_and_file_name, 0 ) == 0 ) { 

      if ( ptr_c -> odbc_user_dsn_flag == 0 ) {
        odbc_write_file_dsn () ;
        output_formatted_file_string ( ptr_g -> log_file_path_and_name, "Opening Database with File DSN from file %s\n", file_dsn_path_and_name ) ;
        sprintf ( tmp_odbc_data_source_name,  "FILEDSN=%s", file_dsn_path_and_name                 ) ;
      endif
      else {
        output_formatted_file_string ( ptr_g -> log_file_path_and_name, "Opening Database with User DSN %s\n", ptr_c -> database_odbc_user_data_source_name ) ;
        sprintf ( tmp_odbc_data_source_name,  "DSN=%s",     ptr_c -> database_odbc_user_data_source_name  ) ;
      endif

      cdatabase_class . OpenEx ( tmp_odbc_data_source_name, 0 ) ;

      if ( cdatabase_class . IsOpen() == 0 ) {
        return_value = 0 ;
        MessageBox( NULL, "Could not Connect to Database.  Check ODBC Parameters in the Control Panel", "Error", ( MB_ICONERROR | MB_OK ) ) ;
      endif
      else {
        return_value = 1 ;
      endif
    endif

    else {
      return_value = 0 ;
      sprintf ( scratch_string, "Could not find database file %s",  ptr_g -> database_path_and_file_name ) ;
      MessageBox( NULL, scratch_string, "Error", ( MB_ICONERROR | MB_OK ) ) ;
    endif

    return return_value ;

  #endif

}

/***************************************************************************/
/***************************************************************************/

long DLL_EXPORT __stdcall odbc_close_database_connection  ( void ) /* function declaration */

{
  cdatabase_class . Close () ;
  return 1 ;
}

/***************************************************************************/
/***************************************************************************/
/***************************************************************************/
/***************************************************************************/
/***************************************************************************/

long DLL_EXPORT __stdcall odbc_delete_all_database_records ( void ) /* function declaration */

{

  int return_value ;

  char sql_string [ 400 ] ;

  return_value = 1 ;

  sprintf ( sql_string, "DELETE * FROM %s ; ", ptr_c -> database_table_name ) ;

  if ( cdatabase_class . IsOpen () == 0 ) {
    return_value = -1 ;
    MessageBox( NULL, "Failed to Delete Database Records!", "Error", ( MB_ICONERROR | MB_OK ) ) ;
  endif

  else {

    if ( strcmpi ( ptr_c -> database_extension, ".XLS" ) != 0 ) {
      cdatabase_class . ExecuteSQL ( sql_string );
    endif

    ptr_c -> database_highest_record_number = 0 ;

  endif

  return return_value ;

}

/*********************************************************************/
/*********************************************************************/
/*********************************************************************/
/*********************************************************************/
/*********************************************************************/
/*********************************************************************/

// LRY's routine follows

long DLL_EXPORT __stdcall odbc_sql_append_database_record ( XC_TIMER_RECORD_STRUCTURE_TYPE *d )

{

  char sql_string   [ 1000 ] ;
  CString cmd ;

  int return_value = 1;

// Access has auto-number feature, so we don't update the record_number field

  if ( strcmpi ( ptr_c -> database_type, "Access" ) == 0 ) {
    cmd = ( "INSERT INTO [tablename] ( DEVICE_NUM, RECORD_NUM, EVENT_NUM, HEAT_NUM, CHANNEL, RECORD_TYP, USERSTRING, USER1FIELD, USER2FIELD, USER3FIELD, USER4FIELD, BIB_STRING, TIMER_TIME, PC_TIME, NOTES ) VALUES ( timing_device, device_record_number, event_number, heat_number, timer_channel, 'record_type', 'total_user_string', 'user1', 'user2', 'user3', 'user4', 'bib', 'timer_time', 'pc_time', 'notes' ) ;" );
  endif

// For other databases, we do update the record_number_field

  else {
    cmd = ( "INSERT INTO [tablename] VALUES ( database_record_number, timing_device, device_record_number, event_number, heat_number, timer_channel, 'record_type', 'total_user_string', 'user1', 'user2', 'user3', 'user4', 'bib', 'timer_time', 'pc_time', 'notes' );" );
  endif
  
  cmd.Replace ( "tablename", (LPCTSTR) local_table_name ) ;

  CString int_as_string;

  ptr_c -> database_highest_record_number ++ ;

  int_as_string.Format( "%ld",           ptr_c -> database_highest_record_number );
  cmd.Replace( "database_record_number", int_as_string );

  int_as_string.Format( "%d",            d->device_num );
  cmd.Replace( "timing_device",          int_as_string );

  int_as_string.Format( "%d",            d->record_num );
  cmd.Replace( "device_record_number",   int_as_string );

  int_as_string.Format( "%d",            d->event_num );
  cmd.Replace( "event_number",           int_as_string );

  int_as_string.Format( "%d",            d->heat_num );
  cmd.Replace( "heat_number",            int_as_string );

  int_as_string.Format( "%d",            d->channel );
  cmd.Replace( "timer_channel",          int_as_string );

  cmd.Replace( "record_type",            d->record_typ );
  cmd.Replace( "total_user_string",      d->userstring );
  cmd.Replace( "user1",                  d->userxfield[0] );
  cmd.Replace( "user2",                  d->userxfield[1] );
  cmd.Replace( "user3",                  d->userxfield[2] );
  cmd.Replace( "user4",                  d->userxfield[3] );
  cmd.Replace( "bib",                    d->bib_string );
  cmd.Replace( "timer_time",             d->timer_time );
  cmd.Replace( "pc_time",                d->pc_time );
  cmd.Replace( "notes",                  d->notes );

  strcpy ( sql_string, cmd ) ;

  if ( ( ptr_c -> diagnostic_flags & 0x04 ) != 0 ) {
    output_formatted_file_string ( ptr_g -> log_file_path_and_name, "**%s**\n", sql_string  ) ;
  endif

  cdatabase_class.ExecuteSQL ( cmd ) ;

  return return_value ;

}

/*****************************************************************************************************************************/
/*****************************************************************************************************************************/

long DLL_EXPORT __stdcall odbc_write_file_dsn ( void ) /* function declaration */

{

  char path_name [ 300 ] ;

  get_file_path_and_name_from_parts ( file_dsn_path_and_name, ptr_g -> race_directory, "srt_file_dsn",  ".dsn" ) ;

  strcpy ( local_table_name, ptr_c-> database_table_name ) ;

  output_formatted_file_string ( file_dsn_path_and_name, NULL ) ;
                                                                                   
  if ( strcmpi ( ptr_c -> database_type, "Access" ) == 0 ) {

    output_formatted_file_string ( file_dsn_path_and_name, "[ODBC]\n"                                            ) ;
    output_formatted_file_string ( file_dsn_path_and_name, "DRIVER=Microsoft Access Driver (*.mdb)\n"            ) ;
    output_formatted_file_string ( file_dsn_path_and_name, "UID=admin\n"                                         ) ;
    output_formatted_file_string ( file_dsn_path_and_name, "UserCommitSync=Yes\n"                                ) ;
    output_formatted_file_string ( file_dsn_path_and_name, "Threads=3\n"                                         ) ;
    output_formatted_file_string ( file_dsn_path_and_name, "SafeTransactions=0\n"                                ) ;
    output_formatted_file_string ( file_dsn_path_and_name, "PageTimeout=5\n"                                     ) ;
    output_formatted_file_string ( file_dsn_path_and_name, "MaxScanRows=8\n"                                     ) ;
    output_formatted_file_string ( file_dsn_path_and_name, "MaxBufferSize=2048\n"                                ) ;
    output_formatted_file_string ( file_dsn_path_and_name, "FIL=MS Access\n"                                     ) ;
    output_formatted_file_string ( file_dsn_path_and_name, "DriverId=25\n"                                       ) ;
    output_formatted_file_string ( file_dsn_path_and_name, "DefaultDir=%s\n",                          path_name ) ;
    output_formatted_file_string ( file_dsn_path_and_name, "DBQ=%s\n",      ptr_g -> database_path_and_file_name ) ;

  endif


  if ( strcmpi ( ptr_c -> database_type, "Excel 3.0" ) == 0 ) {

    strcat ( local_table_name, "$" ) ;

    output_formatted_file_string ( file_dsn_path_and_name, "[ODBC]\n"                                            ) ;
    output_formatted_file_string ( file_dsn_path_and_name, "DRIVER=Microsoft Excel Driver (*.xls)\n"             ) ;
    output_formatted_file_string ( file_dsn_path_and_name, "UID=admin\n"                                         ) ;
    output_formatted_file_string ( file_dsn_path_and_name, "UserCommitSync=Yes\n"                                ) ;
    output_formatted_file_string ( file_dsn_path_and_name, "Threads=3\n"                                         ) ;
    output_formatted_file_string ( file_dsn_path_and_name, "SafeTransactions=0\n"                                ) ;
    output_formatted_file_string ( file_dsn_path_and_name, "ReadOnly=0\n"                                        ) ;
    output_formatted_file_string ( file_dsn_path_and_name, "PageTimeout=5\n"                                     ) ;
    output_formatted_file_string ( file_dsn_path_and_name, "MaxScanRows=8\n"                                     ) ;
    output_formatted_file_string ( file_dsn_path_and_name, "MaxBufferSize=2048\n"                                ) ;
    output_formatted_file_string ( file_dsn_path_and_name, "FIL=excel 3.0\n"                                     ) ;
    output_formatted_file_string ( file_dsn_path_and_name, "DriverId=534\n"                                      ) ;
    output_formatted_file_string ( file_dsn_path_and_name, "DefaultDir=%s\n",                          path_name ) ;
    output_formatted_file_string ( file_dsn_path_and_name, "DBQ=%s\n",      ptr_g -> database_path_and_file_name ) ;

  endif

 if ( strcmpi ( ptr_c -> database_type, "Excel 4.0" ) == 0 ) {

    strcat ( local_table_name, "$" ) ;

    output_formatted_file_string ( file_dsn_path_and_name, "[ODBC]\n"                                            ) ;
    output_formatted_file_string ( file_dsn_path_and_name, "DRIVER=Microsoft Excel Driver (*.xls)\n"             ) ;
    output_formatted_file_string ( file_dsn_path_and_name, "UID=admin\n"                                         ) ;
    output_formatted_file_string ( file_dsn_path_and_name, "UserCommitSync=Yes\n"                                ) ;
    output_formatted_file_string ( file_dsn_path_and_name, "Threads=3\n"                                         ) ;
    output_formatted_file_string ( file_dsn_path_and_name, "SafeTransactions=0\n"                                ) ;
    output_formatted_file_string ( file_dsn_path_and_name, "ReadOnly=0\n"                                        ) ;
    output_formatted_file_string ( file_dsn_path_and_name, "PageTimeout=5\n"                                     ) ;
    output_formatted_file_string ( file_dsn_path_and_name, "MaxScanRows=8\n"                                     ) ;
    output_formatted_file_string ( file_dsn_path_and_name, "MaxBufferSize=2048\n"                                ) ;
    output_formatted_file_string ( file_dsn_path_and_name, "FIL=excel 4.0\n"                                     ) ;
    output_formatted_file_string ( file_dsn_path_and_name, "DriverId=278\n"                                      ) ;
    output_formatted_file_string ( file_dsn_path_and_name, "DefaultDir=%s\n",                          path_name ) ;
    output_formatted_file_string ( file_dsn_path_and_name, "DBQ=%s\n",      ptr_g -> database_path_and_file_name ) ;

  endif

  if ( strcmpi ( ptr_c -> database_type, "Excel 5.0/7.0" ) == 0 ) {

    strcat ( local_table_name, "$" ) ;

    output_formatted_file_string ( file_dsn_path_and_name, "[ODBC]\n"                                            ) ;
    output_formatted_file_string ( file_dsn_path_and_name, "DRIVER=Microsoft Excel Driver (*.xls)\n"             ) ;
    output_formatted_file_string ( file_dsn_path_and_name, "UID=admin\n"                                         ) ;
    output_formatted_file_string ( file_dsn_path_and_name, "UserCommitSync=Yes\n"                                ) ;
    output_formatted_file_string ( file_dsn_path_and_name, "Threads=3\n"                                         ) ;
    output_formatted_file_string ( file_dsn_path_and_name, "SafeTransactions=0\n"                                ) ;
    output_formatted_file_string ( file_dsn_path_and_name, "ReadOnly=0\n"                                        ) ;
    output_formatted_file_string ( file_dsn_path_and_name, "PageTimeout=5\n"                                     ) ;
    output_formatted_file_string ( file_dsn_path_and_name, "MaxScanRows=8\n"                                     ) ;
    output_formatted_file_string ( file_dsn_path_and_name, "MaxBufferSize=2048\n"                                ) ;
    output_formatted_file_string ( file_dsn_path_and_name, "FIL=excel 5.0\n"                                     ) ;
    output_formatted_file_string ( file_dsn_path_and_name, "DriverId=22\n"                                       ) ;
    output_formatted_file_string ( file_dsn_path_and_name, "DefaultDir=%s\n",                          path_name ) ;
    output_formatted_file_string ( file_dsn_path_and_name, "DBQ=%s\n",      ptr_g -> database_path_and_file_name ) ;

  endif

  if ( strcmpi ( ptr_c -> database_type, "Excel 97-2000" ) == 0 ) {

    strcat ( local_table_name, "$" ) ;

    output_formatted_file_string ( file_dsn_path_and_name, "[ODBC]\n"                                            ) ;
    output_formatted_file_string ( file_dsn_path_and_name, "DRIVER=Microsoft Excel Driver (*.xls)\n"             ) ;
    output_formatted_file_string ( file_dsn_path_and_name, "UID=admin\n"                                         ) ;
    output_formatted_file_string ( file_dsn_path_and_name, "UserCommitSync=Yes\n"                                ) ;
    output_formatted_file_string ( file_dsn_path_and_name, "Threads=3\n"                                         ) ;
    output_formatted_file_string ( file_dsn_path_and_name, "SafeTransactions=0\n"                                ) ;
    output_formatted_file_string ( file_dsn_path_and_name, "ReadOnly=0\n"                                        ) ;
    output_formatted_file_string ( file_dsn_path_and_name, "PageTimeout=5\n"                                     ) ;
    output_formatted_file_string ( file_dsn_path_and_name, "MaxScanRows=8\n"                                     ) ;
    output_formatted_file_string ( file_dsn_path_and_name, "MaxBufferSize=2048\n"                                ) ;
    output_formatted_file_string ( file_dsn_path_and_name, "FIL=excel 8.0\n"                                     ) ;
    output_formatted_file_string ( file_dsn_path_and_name, "DriverId=790\n"                                      ) ;
    output_formatted_file_string ( file_dsn_path_and_name, "DefaultDir=%s\n",                          path_name ) ;
    output_formatted_file_string ( file_dsn_path_and_name, "DBQ=%s\n",      ptr_g -> database_path_and_file_name ) ;

  endif

  if ( strcmpi ( ptr_c -> database_type, "dBase III" ) == 0 ) {
                                                                                                            
    output_formatted_file_string ( file_dsn_path_and_name, "[ODBC]\n"                                            ) ;
    output_formatted_file_string ( file_dsn_path_and_name, "DRIVER=Microsoft dBase Driver (*.dbf)\n"             ) ;
    output_formatted_file_string ( file_dsn_path_and_name, "UID=admin\n"                                         ) ;
    output_formatted_file_string ( file_dsn_path_and_name, "UserCommitSync=Yes\n"                                ) ;
    output_formatted_file_string ( file_dsn_path_and_name, "Threads=3\n"                                         ) ;
    output_formatted_file_string ( file_dsn_path_and_name, "Statistics=0\n"                                      ) ;
    output_formatted_file_string ( file_dsn_path_and_name, "SafeTransactions=0\n"                                ) ;
    output_formatted_file_string ( file_dsn_path_and_name, "PageTimeout=600\n"                                   ) ;
    output_formatted_file_string ( file_dsn_path_and_name, "MaxScanRows=8\n"                                     ) ;
    output_formatted_file_string ( file_dsn_path_and_name, "MaxBufferSize=2048\n"                                ) ;
    output_formatted_file_string ( file_dsn_path_and_name, "FIL=dBase III\n"                                     ) ;
    output_formatted_file_string ( file_dsn_path_and_name, "DriverId=21\n"                                       ) ;
    output_formatted_file_string ( file_dsn_path_and_name, "Deleted=1\n"                                         ) ;
    output_formatted_file_string ( file_dsn_path_and_name, "DefaultDir=%s\n",                          path_name ) ;
    output_formatted_file_string ( file_dsn_path_and_name, "DBQ=%s\n",      ptr_g -> database_path_and_file_name ) ;
    output_formatted_file_string ( file_dsn_path_and_name, "CollatingSequence=ASCII\n"                           ) ;

  endif

  if ( strcmpi ( ptr_c -> database_type, "dBase IV" ) == 0 ) {
                                                                                                            
    output_formatted_file_string ( file_dsn_path_and_name, "[ODBC]\n"                                            ) ;
    output_formatted_file_string ( file_dsn_path_and_name, "DRIVER=Microsoft dBase Driver (*.dbf)\n"             ) ;
    output_formatted_file_string ( file_dsn_path_and_name, "UID=admin\n"                                         ) ;
    output_formatted_file_string ( file_dsn_path_and_name, "UserCommitSync=Yes\n"                                ) ;
    output_formatted_file_string ( file_dsn_path_and_name, "Threads=3\n"                                         ) ;
    output_formatted_file_string ( file_dsn_path_and_name, "Statistics=0\n"                                      ) ;
    output_formatted_file_string ( file_dsn_path_and_name, "SafeTransactions=0\n"                                ) ;
    output_formatted_file_string ( file_dsn_path_and_name, "PageTimeout=600\n"                                   ) ;
    output_formatted_file_string ( file_dsn_path_and_name, "MaxScanRows=8\n"                                     ) ;
    output_formatted_file_string ( file_dsn_path_and_name, "MaxBufferSize=2048\n"                                ) ;
    output_formatted_file_string ( file_dsn_path_and_name, "FIL=dBase IV\n"                                      ) ;
    output_formatted_file_string ( file_dsn_path_and_name, "DriverId=277\n"                                      ) ;
    output_formatted_file_string ( file_dsn_path_and_name, "Deleted=1\n"                                         ) ;
    output_formatted_file_string ( file_dsn_path_and_name, "DefaultDir=%s\n",                          path_name ) ;
    output_formatted_file_string ( file_dsn_path_and_name, "DBQ=%s\n",      ptr_g -> database_path_and_file_name ) ;
    output_formatted_file_string ( file_dsn_path_and_name, "CollatingSequence=ASCII\n"                           ) ;

  endif

  if ( strcmpi ( ptr_c -> database_type, "dBase 5" ) == 0 ) {
                                                                                                            
    output_formatted_file_string ( file_dsn_path_and_name, "[ODBC]\n"                                            ) ;
    output_formatted_file_string ( file_dsn_path_and_name, "DRIVER=Microsoft dBase Driver (*.dbf)\n"             ) ;
    output_formatted_file_string ( file_dsn_path_and_name, "UID=admin\n"                                         ) ;
    output_formatted_file_string ( file_dsn_path_and_name, "UserCommitSync=Yes\n"                                ) ;
    output_formatted_file_string ( file_dsn_path_and_name, "Threads=3\n"                                         ) ;
    output_formatted_file_string ( file_dsn_path_and_name, "Statistics=0\n"                                      ) ;
    output_formatted_file_string ( file_dsn_path_and_name, "SafeTransactions=0\n"                                ) ;
    output_formatted_file_string ( file_dsn_path_and_name, "PageTimeout=600\n"                                   ) ;
    output_formatted_file_string ( file_dsn_path_and_name, "MaxScanRows=8\n"                                     ) ;
    output_formatted_file_string ( file_dsn_path_and_name, "MaxBufferSize=2048\n"                                ) ;
    output_formatted_file_string ( file_dsn_path_and_name, "FIL=dBase 5.0\n"                                     ) ;
    output_formatted_file_string ( file_dsn_path_and_name, "DriverId=533\n"                                      ) ;
    output_formatted_file_string ( file_dsn_path_and_name, "Deleted=1\n"                                         ) ;
    output_formatted_file_string ( file_dsn_path_and_name, "DefaultDir=%s\n",                          path_name ) ;
    output_formatted_file_string ( file_dsn_path_and_name, "DBQ=%s\n",      ptr_g -> database_path_and_file_name ) ;
    output_formatted_file_string ( file_dsn_path_and_name, "CollatingSequence=ASCII\n"                           ) ;

  endif

  return 1 ;

}

/*****************************************************************************************************************************/
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

int get_file_path_and_name_from_parts ( char * file_path_and_name, char * path, char * file_name, char * file_extension  ) /* function declaration */

{

//_getcwd ( file_path_and_name, 500 ) ;

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
