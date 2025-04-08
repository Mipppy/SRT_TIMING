
/***************************************************************************/
/***************************************************************************/


// xc_timer_db.cpp :  This module appends records to the database
//


#include "stdafx.h"
#include "xc_db_dll.h"

#include "xc_dll.h"

#include "xc_db_odbc_set.h"
#include "xc_db_dao_set.h"

/***************************************************************************/
/***************************************************************************/

XC_CONFIGURE_STRUCTURE_TYPE * c ;

int debug_flag = 0 ;

/***************************************************************************/
/***************************************************************************/

/***************************************************************************/
/***************************************************************************/

#if ( 1 ) 
// LRY - CRecordset class constructor needs pointer to CDatabase object
  CDatabase              cdatabase_class   ;
  class_xc_db_odbc_set   m_odbc_data_set( &cdatabase_class )   ;

  class_xc_db_odbc_set_access odbc_data_set_access( &cdatabase_class );
  class_xc_db_odbc_set_dbase  odbc_data_set_dbase ( &cdatabase_class );
  class_xc_db_odbc_set_excel  odbc_data_set_excel ( &cdatabase_class );
  class_xc_db_odbc_set_foxpro odbc_data_set_foxpro( &cdatabase_class );

  class_xc_db_dao_set    m_dao_data_set  ;
#endif

/***************************************************************************/
/***************************************************************************/

#if ( 0 ) 

BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
    switch (ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:
		case DLL_THREAD_ATTACH:
		case DLL_THREAD_DETACH:
		case DLL_PROCESS_DETACH:
			break;
    }
    return TRUE;
}


// This is an example of an exported variable
XC_DB_DLL_API int nXc_db_dll=0;

// This is an example of an exported function.
XC_DB_DLL_API int fnXc_db_dll(void)
{
	return 42;
}

// This is the constructor of a class that has been exported.
// see xc_db_dll.h for the class definition
CXc_db_dll::CXc_db_dll()
{ 
	return; 
}

#endif

/***************************************************************************/
/***************************************************************************/

// LRY - modify temporarily for open/close/both
int DLL_EXPORT odbc_test_database_connection  ( XC_CONFIGURE_STRUCTURE_TYPE * tmp_c, BOOL open_it, BOOL close_it ) /* function declaration */
{
  int return_value = 1 ;

  char tmp_odbc_data_source_name [ 400 ] ;

  c = tmp_c ;

#if ( 0 ) 
  class_xc_db_odbc_set   m_odbc_data_set        ;
#endif

  strcpy ( tmp_odbc_data_source_name,  "DSN="                        ) ;
  strcat ( tmp_odbc_data_source_name,  c -> database_odbc_data_source_name  ) ;

#if (1)

  // LRY
  if ( open_it )
  cdatabase_class . OpenEx ( tmp_odbc_data_source_name, 0 ) ;

  if ( cdatabase_class . IsOpen() == 0 ) {
    return_value = -1 ;
  }

  else {
     if ( close_it )
    cdatabase_class . Close () ;
     
     close_it = FALSE;
  }

  if ( close_it )
     cdatabase_class.Close();

#endif

/****/

#if ( 0 ) 

  if ( return_value >= 0 ) {

    m_odbc_data_set . Open() ;

    if ( m_odbc_data_set. IsOpen() == 0 ) {
      return_value = -2 ;
    }

    else {
      m_odbc_data_set . Close();
    }

  }

#endif

  return return_value ;

}

/***************************************************************************/
/***************************************************************************/

int DLL_EXPORT odbc_append_record_to_database  ( XC_TIMER_EVENT_STRUCTURE_TYPE * d  ) /* function declaration */

{
 
  CString xscratch ;
  char    scratch [ 500 ] ;
  int return_value ;

  #if ( 0 ) 
    class_xc_db_odbc_set   m_odbc_data_set        ;
  #endif

  return_value = 1 ;

  // LRY
  m_odbc_data_set . Open( CRecordset::dynaset, NULL, CRecordset::appendOnly );

  if ( m_odbc_data_set . IsOpen() == 0 ) {
    return_value = -1 ;
  }

  else {         

    if ( debug_flag != 0 ) sprintf  ( scratch, "GetRowsetSize is %8ld", m_odbc_data_set . GetRowsetSize () ) ;                                           

    if ( debug_flag != 0 ) MessageBox ( GetFocus(), scratch, "DB Diagnostics", MB_OK);

    if ( debug_flag != 0 ) xscratch = m_odbc_data_set . GetSQL () ;

    if ( debug_flag != 0 ) MessageBox ( GetFocus(), xscratch, "DB Diagnostics", MB_OK);

    if ( m_odbc_data_set . CanAppend () == 0 ) MessageBox ( GetFocus(), "Cannot Append to Recordset", "DB Error Message", MB_OK);
    if ( m_odbc_data_set . CanUpdate () == 0 ) MessageBox ( GetFocus(), "Cannot Update Recordset",    "DB Error Message", MB_OK);

    m_odbc_data_set . AddNew();

    if ( debug_flag != 0 ) MessageBox ( GetFocus(), "Just Called AddNew", "DB Diagnostics", MB_OK);

    m_odbc_data_set . m_raw_table_event_counter_field            = 0  ;
    m_odbc_data_set . m_timing_device_field                      = d -> timing_device_field             ;
    m_odbc_data_set . m_record_number_for_device_field           = d -> record_number_for_device_field  ; 
    m_odbc_data_set . m_timer_channel_for_device_field           = d -> timer_channel_for_device_field  ; 

    m_odbc_data_set . m_total_user_string_field         . Format ( "%s",  d -> total_user_string_field       )      ;
    m_odbc_data_set . m_user1_string_field              . Format ( "%s",  d -> user_string_array_field [ 0 ] )      ;
    m_odbc_data_set . m_user2_string_field              . Format ( "%s",  d -> user_string_array_field [ 1 ] )      ;
    m_odbc_data_set . m_user3_string_field              . Format ( "%s",  d -> user_string_array_field [ 2 ] )      ;

    m_odbc_data_set . m_user4_string_field              . Format ( "%s",  d -> user_string_array_field [ 3 ] )      ;
    m_odbc_data_set . m_bib_string_field                . Format ( "%s",  d -> bib_string_field              )      ;
    m_odbc_data_set . m_timer_time_string_field         . Format ( "%s",  d -> timer_time_string_field       )      ;
    m_odbc_data_set . m_pc_time_string_field            . Format ( "%s",  d -> pc_time_string_field          )      ;


   if ( debug_flag != 0 ) MessageBox ( GetFocus(), "Ready to Call Update", "DB Diagnostics", MB_OK);

   if ( m_odbc_data_set . Update() == 0 ) MessageBox ( GetFocus(), "ODBC Update Failed", "DB Diagnostics", MB_OK);

   if ( debug_flag != 0 ) MessageBox ( GetFocus(), "Just Called Update", "DB Diagnostics", MB_OK);

   m_odbc_data_set . Close();

   if ( debug_flag != 0 ) MessageBox ( GetFocus(), "Just Called Close", "DB Diagnostics", MB_OK);

  }

  return return_value ;

}

/*********************************************************************/
/*********************************************************************/
//
// LRY - add functions for each type of database
//

int DLL_EXPORT odbc_append_record_to_access( XC_TIMER_EVENT_STRUCTURE_TYPE *d )
{
  int return_value = -1;

  cdatabase_class.OpenEx( "DSN=MS Access Database", 0 ) ;

  if ( odbc_data_set_access.Open( CRecordset::dynaset, NULL, CRecordset::appendOnly ) )
  {
    return_value = 1;

    odbc_data_set_access.AddNew();

    odbc_data_set_access.m_raw_table_event_counter_field   = 0  ;
    odbc_data_set_access.m_timing_device_field             = d -> timing_device_field             ;
    odbc_data_set_access.m_record_number_for_device_field  = d -> record_number_for_device_field  ; 
    odbc_data_set_access.m_timer_channel_for_device_field  = d -> timer_channel_for_device_field  ; 

    odbc_data_set_access.m_total_user_string_field         . Format ( "%s",  d -> total_user_string_field       )      ;
    odbc_data_set_access.m_user1_string_field              . Format ( "%s",  d -> user_string_array_field [ 0 ] )      ;
    odbc_data_set_access.m_user2_string_field              . Format ( "%s",  d -> user_string_array_field [ 1 ] )      ;
    odbc_data_set_access.m_user3_string_field              . Format ( "%s",  d -> user_string_array_field [ 2 ] )      ;

    odbc_data_set_access.m_user4_string_field              . Format ( "%s",  d -> user_string_array_field [ 3 ] )      ;
    odbc_data_set_access.m_bib_string_field                . Format ( "%s",  d -> bib_string_field              )      ;
    odbc_data_set_access.m_timer_time_string_field         . Format ( "%s",  d -> timer_time_string_field       )      ;
    odbc_data_set_access.m_pc_time_string_field            . Format ( "%s",  d -> pc_time_string_field          )      ;


    if ( odbc_data_set_access.Update() == 0 )
       return_value = -1;

    odbc_data_set_access.Close();
  }
  
  cdatabase_class.Close();

  return return_value ;
}

int DLL_EXPORT odbc_append_record_to_dbase( XC_TIMER_EVENT_STRUCTURE_TYPE *d )
{
  int return_value = -1;

  cdatabase_class.OpenEx( "DSN=dBASE Files", 0 ) ;

  if ( odbc_data_set_dbase.Open( CRecordset::snapshot, NULL, CRecordset::appendOnly ) ) // VVV
  {
    return_value = 1;

    odbc_data_set_dbase.AddNew();

    odbc_data_set_dbase.m_raw_table_event_counter_field   = 0  ;
    odbc_data_set_dbase.m_timing_device_field             = d -> timing_device_field             ;
    odbc_data_set_dbase.m_record_number_for_device_field  = d -> record_number_for_device_field  ; 
    odbc_data_set_dbase.m_timer_channel_for_device_field  = d -> timer_channel_for_device_field  ; 

    odbc_data_set_dbase.m_total_user_string_field         . Format ( "%s",  d -> total_user_string_field       )      ;
    odbc_data_set_dbase.m_user1_string_field              . Format ( "%s",  d -> user_string_array_field [ 0 ] )      ;
    odbc_data_set_dbase.m_user2_string_field              . Format ( "%s",  d -> user_string_array_field [ 1 ] )      ;
    odbc_data_set_dbase.m_user3_string_field              . Format ( "%s",  d -> user_string_array_field [ 2 ] )      ;

    odbc_data_set_dbase.m_user4_string_field              . Format ( "%s",  d -> user_string_array_field [ 3 ] )      ;
    odbc_data_set_dbase.m_bib_string_field                . Format ( "%s",  d -> bib_string_field              )      ;
    odbc_data_set_dbase.m_timer_time_string_field         . Format ( "%s",  d -> timer_time_string_field       )      ;
    odbc_data_set_dbase.m_pc_time_string_field            . Format ( "%s",  d -> pc_time_string_field          )      ;


    if ( odbc_data_set_dbase.Update() == 0 )
       return_value = -1;

    odbc_data_set_dbase.Close();
  }

  cdatabase_class.Close();

  return return_value ;
}

int DLL_EXPORT odbc_append_record_to_excel( XC_TIMER_EVENT_STRUCTURE_TYPE *d )
{
  int return_value = -1;

  cdatabase_class.OpenEx( "DSN=Excel Files", 0 ) ;

  if ( odbc_data_set_excel.Open( CRecordset::snapshot, NULL, CRecordset::appendOnly ) ) // VVV
  {
    return_value = 1;

    odbc_data_set_excel.AddNew();

    odbc_data_set_excel.m_raw_table_event_counter_field   = 0  ;
    odbc_data_set_excel.m_timing_device_field             = d -> timing_device_field             ;
    odbc_data_set_excel.m_record_number_for_device_field  = d -> record_number_for_device_field  ; 
    odbc_data_set_excel.m_timer_channel_for_device_field  = d -> timer_channel_for_device_field  ; 

    odbc_data_set_excel.m_total_user_string_field         . Format ( "%s",  d -> total_user_string_field       )      ;
    odbc_data_set_excel.m_user1_string_field              . Format ( "%s",  d -> user_string_array_field [ 0 ] )      ;
    odbc_data_set_excel.m_user2_string_field              . Format ( "%s",  d -> user_string_array_field [ 1 ] )      ;
    odbc_data_set_excel.m_user3_string_field              . Format ( "%s",  d -> user_string_array_field [ 2 ] )      ;

    odbc_data_set_excel.m_user4_string_field              . Format ( "%s",  d -> user_string_array_field [ 3 ] )      ;
    odbc_data_set_excel.m_bib_string_field                . Format ( "%s",  d -> bib_string_field              )      ;
    odbc_data_set_excel.m_timer_time_string_field         . Format ( "%s",  d -> timer_time_string_field       )      ;
    odbc_data_set_excel.m_pc_time_string_field            . Format ( "%s",  d -> pc_time_string_field          )      ;


    if ( odbc_data_set_excel.Update() == 0 )
       return_value = -1;

    odbc_data_set_excel.Close();
  }

  cdatabase_class.Close();

  return return_value ;
}

int DLL_EXPORT odbc_append_record_to_foxpro( XC_TIMER_EVENT_STRUCTURE_TYPE *d )
{
  int return_value = -1;

  cdatabase_class.OpenEx( "DSN=Visual Foxpro Database", 0 ) ;

  if ( odbc_data_set_foxpro.Open( CRecordset::snapshot, NULL, CRecordset::appendOnly ) ) // VVV
  {
    return_value = 1;

    odbc_data_set_foxpro.AddNew();

    odbc_data_set_foxpro.m_raw_table_event_counter_field   = 0  ;
    odbc_data_set_foxpro.m_timing_device_field             = d -> timing_device_field             ;
    odbc_data_set_foxpro.m_record_number_for_device_field  = d -> record_number_for_device_field  ; 
    odbc_data_set_foxpro.m_timer_channel_for_device_field  = d -> timer_channel_for_device_field  ; 

    odbc_data_set_foxpro.m_total_user_string_field         . Format ( "%s",  d -> total_user_string_field       )      ;
    odbc_data_set_foxpro.m_user1_string_field              . Format ( "%s",  d -> user_string_array_field [ 0 ] )      ;
    odbc_data_set_foxpro.m_user2_string_field              . Format ( "%s",  d -> user_string_array_field [ 1 ] )      ;
    odbc_data_set_foxpro.m_user3_string_field              . Format ( "%s",  d -> user_string_array_field [ 2 ] )      ;

    odbc_data_set_foxpro.m_user4_string_field              . Format ( "%s",  d -> user_string_array_field [ 3 ] )      ;
    odbc_data_set_foxpro.m_bib_string_field                . Format ( "%s",  d -> bib_string_field              )      ;
    odbc_data_set_foxpro.m_timer_time_string_field         . Format ( "%s",  d -> timer_time_string_field       )      ;
    odbc_data_set_foxpro.m_pc_time_string_field            . Format ( "%s",  d -> pc_time_string_field          )      ;


    if ( odbc_data_set_foxpro.Update() == 0 )
       return_value = -1;

    odbc_data_set_foxpro.Close();
  }

  cdatabase_class.Close();

  return return_value ;
}

/*********************************************************************/
/*********************************************************************/

int DLL_EXPORT odbc_delete_all_database_records ( void ) /* function declaration */

{
  int return_value = 1 ;

  char sql_string [ 400 ] ;
  char tmp_odbc_data_source_name [ 400 ] ;
  CDatabase                cdatabase_class   ;
 
  sprintf ( sql_string, "DELETE * FROM %s ; ", c -> database_table_name ) ;

  strcpy ( tmp_odbc_data_source_name,  "DSN="                        ) ;
  strcat ( tmp_odbc_data_source_name,  c -> database_odbc_data_source_name ) ;
   
//m_dsn_name = tmp_odbc_data_source_name ;

  cdatabase_class . OpenEx ( tmp_odbc_data_source_name, 0 ) ;

  if ( cdatabase_class . IsOpen() == 0 ) {
    return_value = -1 ;
  }

  else {
    cdatabase_class . ExecuteSQL ( sql_string );
    cdatabase_class . Close () ;
  }

  return return_value ;
}

/***************************************************************************/
/*********************************************************************/
/*********************************************************************/
/*********************************************************************/
/*********************************************************************/

// LRY

int DLL_EXPORT append_record_to_database( XC_CONFIGURE_STRUCTURE_TYPE *cfg, XC_TIMER_EVENT_STRUCTURE_TYPE *d )
{
  int return_value = 1;

  CString open_database_string;
  
  open_database_string.Format( "DSN=%s", (LPCTSTR)cfg->database_odbc_data_source_name );

  cdatabase_class.OpenEx( (LPCTSTR)open_database_string, 0 ) ;

  CString cmd( "INSERT INTO [tablename] VALUES ( event_counter, timing_device, record_number, timer_channel, 'user0', 'user1', 'user2', 'user3', 'user4', 'bib', 'timer_time', 'pc_time' );" );
  cmd.Replace( "tablename", (LPCTSTR)cfg->database_table_name );

  CString int_as_string;
  
  static int an_event_counter = 0;

  int_as_string.Format( "%d", an_event_counter++ );
  cmd.Replace( "event_counter", (LPCTSTR)int_as_string );

  int_as_string.Format( "%d", d->timing_device_field );
  cmd.Replace( "timing_device", int_as_string );

  int_as_string.Format( "%d", d->record_number_for_device_field );
  cmd.Replace( "record_number", int_as_string );

  int_as_string.Format( "%d", d->timer_channel_for_device_field );
  cmd.Replace( "timer_channel", int_as_string );

  cmd.Replace( "user0", d->total_user_string_field );
  cmd.Replace( "user1", d->user_string_array_field[1] );
  cmd.Replace( "user2", d->user_string_array_field[2] );
  cmd.Replace( "user3", d->user_string_array_field[3] );
  cmd.Replace( "user4", d->user_string_array_field[4] );
  cmd.Replace( "bib", d->bib_string_field );
  cmd.Replace( "timer_time", d->timer_time_string_field );
  cmd.Replace( "pc_time", d->pc_time_string_field );

  cdatabase_class.ExecuteSQL( cmd );

  cdatabase_class.Close();

  return return_value ;
}
