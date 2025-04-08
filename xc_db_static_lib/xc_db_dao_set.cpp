// daoSet.cpp : implementation of the class_xc_db_dao_set class
//

#include "stdafx.h"
#include "xc_dll.h"
#include "timer_app.h"
#include "xc_db_dao_set.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/**********************************************************************/

extern XC_CONFIGURE_STRUCTURE_TYPE * c ;

/**********************************************************************/

/////////////////////////////////////////////////////////////////////////////

// class_xc_db_dao_set implementation

IMPLEMENT_DYNAMIC(class_xc_db_dao_set, CDaoRecordset)

class_xc_db_dao_set::class_xc_db_dao_set(CDaoDatabase* pdb)
	: CDaoRecordset(pdb)
{

  //{{AFX_FIELD_INIT(class_xc_db_dao_set)

    m_raw_table_event_counter_field         =  0     ;
    m_timing_device_field                   =  0     ; 
    m_record_number_for_device_field        =  0     ;
    m_timer_channel_for_device_field        =  0     ;

    m_total_user_string_field               = _T(" ") ;
    m_user1_string_field                    = _T(" ") ;
    m_user2_string_field                    = _T(" ") ;
    m_user3_string_field                    = _T(" ") ;
    m_user4_string_field                    = _T(" ") ;
    m_bib_string_field                      = _T(" ") ;
    m_timer_time_string_field               = _T(" ") ;
    m_pc_time_string_field                  = _T(" ") ;

    m_nFields                               = 12     ;

  //}}AFX_FIELD_INIT

  m_nDefaultType = dbOpenTable;

}

/********************************************************************************/

CString class_xc_db_dao_set::GetDefaultDBName ()

{

   return _T ( c -> database_path_and_file_name ) ;

}

/********************************************************************************/

CString class_xc_db_dao_set::GetDefaultSQL()
{

  char     tmp_database_table_name [ 500 ] ;

  strcpy ( tmp_database_table_name,    "["                           ) ;
  strcat ( tmp_database_table_name,    c -> database_table_name       ) ;
  strcat ( tmp_database_table_name,    "]"                           ) ;
 
  return _T ( tmp_database_table_name ) ;

}

/********************************************************************************/

void class_xc_db_dao_set::DoFieldExchange(CDaoFieldExchange* pFX)
{

  //{{AFX_FIELD_MAP(class_xc_db_dao_set)

	pFX->SetFieldType(CDaoFieldExchange::outputColumn);

    DFX_Double ( pFX, _T ( "[TABLE_ID]"      ), m_raw_table_event_counter_field           ) ;
    DFX_Double ( pFX, _T ( "[DEVICE_NUM]"    ), m_timing_device_field                     ) ;
    DFX_Double ( pFX, _T ( "[RECORD_NUM]"    ), m_record_number_for_device_field          ) ;
    DFX_Double ( pFX, _T ( "[CHANNEL]"       ), m_timer_channel_for_device_field          ) ;

    DFX_Text   ( pFX, _T ( "[USERSTRING]"    ), m_total_user_string_field                 ) ;
    DFX_Text   ( pFX, _T ( "[USER1FIELD]"    ), m_user1_string_field                      ) ;
    DFX_Text   ( pFX, _T ( "[USER2FIELD]"    ), m_user2_string_field                      ) ;
    DFX_Text   ( pFX, _T ( "[USER3FIELD]"    ), m_user3_string_field                      ) ;
    DFX_Text   ( pFX, _T ( "[USER4FIELD]"    ), m_user4_string_field                      ) ;
    DFX_Text   ( pFX, _T ( "[BIB_STRING]"    ), m_bib_string_field                        ) ;
    DFX_Text   ( pFX, _T ( "[TIMER_TIME]"    ), m_timer_time_string_field                 ) ;
    DFX_Text   ( pFX, _T ( "[PC_TIME]"       ), m_pc_time_string_field                    ) ;

#if ( 0 )

    DFX_Long  ( pFX, _T ( "[TABLE_ID]"      ), m_raw_table_event_counter_field           ) ;
    DFX_Short ( pFX, _T ( "[DEVICE_NUM]"    ), m_timing_device_field                     ) ;
    DFX_Long  ( pFX, _T ( "[RECORD_NUM]"    ), m_record_number_for_device_field          ) ;
    DFX_Short ( pFX, _T ( "[CHANNEL]"       ), m_timer_channel_for_device_field          ) ;
 
   DFX_Text  ( pFX, _T ( "[USERSTRING]"    ), m_total_user_string_field                 ) ;
    DFX_Text  ( pFX, _T ( "[USER1FIELD]"    ), m_user1_string_field                      ) ;
    DFX_Text  ( pFX, _T ( "[USER2FIELD]"    ), m_user2_string_field                      ) ;
    DFX_Text  ( pFX, _T ( "[USER3FIELD]"    ), m_user3_string_field                      ) ;
    DFX_Text  ( pFX, _T ( "[USER4FIELD]"    ), m_user4_string_field                      ) ;
    DFX_Text  ( pFX, _T ( "[BIB_STRING]"    ), m_bib_string_field                        ) ;
    DFX_Text  ( pFX, _T ( "[TIMER_TIME]"    ), m_timer_time_string_field                 ) ;
    DFX_Text  ( pFX, _T ( "[PC_TIME]"       ), m_pc_time_string_field                    ) ;

#endif

  //}}AFX_FIELD_MAP

}

/////////////////////////////////////////////////////////////////////////////

// class_xc_db_dao_set diagnostics

#ifdef _DEBUG

void class_xc_db_dao_set::AssertValid() const
{
	CDaoRecordset::AssertValid();
}

void class_xc_db_dao_set::Dump(CDumpContext& dc) const
{
	CDaoRecordset::Dump(dc);
}

#endif //_DEBUG

/********************************************************************************/
/*********************************************************************/

int DLL_EXPORT dao_test_database_connection  ( XC_CONFIGURE_STRUCTURE_TYPE * tmp_c ) /* function declaration */

{

  int return_value = 1 ;

  c = tmp_c ;

  m_dao_data_set . Open();

  if ( m_dao_data_set . IsOpen() == 0 ) {
    return_value = -2 ;
  }

  else {
    m_dao_data_set . Close();
  }

  return return_value ;

}

/***************************************************************************/
/***************************************************************************/


int DLL_EXPORT dao_append_record_to_database  ( XC_TIMER_EVENT_STRUCTURE_TYPE * d  ) /* function declaration */

{
 
  CString xscratch ;
 
  int return_value = 1 ;

//    m_dao_data_set . SetLockingMode ( pessimistic ) ;
    
  m_dao_data_set . Open();

  if ( m_dao_data_set . IsOpen() == 0 ) {
    return_value = -1 ;
  }

  else {         

    if ( m_dao_data_set . CanAppend () == 0 ) MessageBox ( GetFocus(), "Cannot Append to Recordset", "DB Error Message", MB_OK) ;
    if ( m_dao_data_set . CanUpdate () == 0 ) MessageBox ( GetFocus(), "Cannot Update Recordset",    "DB Error Message", MB_OK) ;

    if ( debug_flag != 0 ) xscratch = m_dao_data_set . GetSQL () ;

    if ( debug_flag != 0 ) MessageBox ( GetFocus(), xscratch, "DB Diagnostics", MB_OK) ;

    m_dao_data_set . AddNew () ;

    m_dao_data_set . m_raw_table_event_counter_field            = (double) 0  ;
    m_dao_data_set . m_timing_device_field                      = (double)   d -> timing_device_field             ;
    m_dao_data_set . m_record_number_for_device_field           = (double) d -> record_number_for_device_field  ; 
    m_dao_data_set . m_timer_channel_for_device_field           = (double)   d -> timer_channel_for_device_field  ; 

    m_dao_data_set . m_total_user_string_field         . Format ( "%s",  d -> total_user_string_field       )      ;
    m_dao_data_set . m_user1_string_field              . Format ( "%s",  d -> user_string_array_field [ 0 ] )      ;
    m_dao_data_set . m_user2_string_field              . Format ( "%s",  d -> user_string_array_field [ 1 ] )      ;
    m_dao_data_set . m_user3_string_field              . Format ( "%s",  d -> user_string_array_field [ 2 ] )      ;

    m_dao_data_set . m_user4_string_field              . Format ( "%s",  d -> user_string_array_field [ 3 ] )      ;
    m_dao_data_set . m_bib_string_field                . Format ( "%s",  d -> bib_string_field              )      ;
    m_dao_data_set . m_timer_time_string_field         . Format ( "%s",  d -> timer_time_string_field       )      ;
    m_dao_data_set . m_pc_time_string_field            . Format ( "%s",  d -> pc_time_string_field          )      ;

    if ( debug_flag != 0 ) MessageBox ( GetFocus(), "Ready to Call Update", "DB Diagnostics", MB_OK ) ;

    m_dao_data_set . Update() ;

    if ( debug_flag != 0 ) MessageBox ( GetFocus(), "Just Called Update",   "DB Diagnostics", MB_OK ) ;

    m_dao_data_set . Close();

    if ( debug_flag != 0 ) MessageBox ( GetFocus(), "Just Called Close",    "DB Diagnostics", MB_OK ) ;

  }

  return return_value ;

}

/***************************************************************************/

int DLL_EXPORT dao_delete_all_database_records ( void ) /* function declaration */

{

  int return_value = 1 ;

  char sql_string [ 400 ] ;

  CDaoDatabase  c_dao_database_class ;

  c_dao_database_class . Open ( c -> database_path_and_file_name, 0 ) ;

  if ( c_dao_database_class . IsOpen() == 0 ) {
    MessageBox ( GetFocus(), "Could not Open Datapase", "DB Diagnostics", MB_OK );
    return_value = -1 ;
  }

  else {
    sprintf ( sql_string, "DELETE * FROM %s ; ", c -> database_table_name ) ;
    c_dao_database_class . Execute ( sql_string, 0 );
    c_dao_database_class . Close () ;
  }

  return return_value ;

}

/***************************************************************************/
/***************************************************************************/
/********************************************************************************/
