
// xc_timing_db_set.cpp : implementation of the class_xc_db_odbc_set class
//

#include "stdafx.h"
#include "xc_dll.h"
#include "timer_app.h"
#include "xc_db_odbc_set.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/**********************************************************************/

extern XC_CONFIGURE_STRUCTURE_TYPE * c ;

/**********************************************************************/

// class_xc_db_odbc_set implementation

IMPLEMENT_DYNAMIC(class_xc_db_odbc_set, CRecordset)

class_xc_db_odbc_set::class_xc_db_odbc_set(CDatabase* pdb)
    : CRecordset(pdb)
{
    //{{AFX_FIELD_INIT(class_xc_db_odbc_set)

    m_timer_table_id_field                  =  0     ;
    m_timing_device_field                   =  0     ; 
    m_record_number_for_device_field        =  0     ;
    m_event_number_field                    =  0     ;
    m_heat_number_field                     =  0     ;
    m_timer_channel_for_device_field        =  0     ;

    m_record_type_string_field              = _T(" ") ;
    m_total_user_string_field               = _T(" ") ;
    m_user1_string_field                    = _T(" ") ;
    m_user2_string_field                    = _T(" ") ;

    m_user3_string_field                    = _T(" ") ;
    m_user4_string_field                    = _T(" ") ;
    m_bib_string_field                      = _T(" ") ;
    m_timer_time_string_field               = _T(" ") ;

    m_pc_time_string_field                  = _T(" ") ;
    m_note_string_field                     = _T(" ") ;

    m_nFields                               = 14     ;

    //}}AFX_FIELD_INIT


//  m_nDefaultType = dbOpenTable;  // From DAO example
//  m_nDefaultType = snapshot ;    // Was snapshot till 4/18/01 
    m_nDefaultType = dynaset  ;    // Was snapshot till 4/18/01

}

/**********************************************************************/

CString class_xc_db_odbc_set::GetDefaultConnect()
{

  char tmp_odbc_data_source_name [ 400 ] ;

  strcpy ( tmp_odbc_data_source_name,  "ODBC;DSN="                          ) ;
  strcat ( tmp_odbc_data_source_name,  c -> database_odbc_data_source_name  ) ;

  return _T ( tmp_odbc_data_source_name ) ;

}

/**********************************************************************/

CString class_xc_db_odbc_set::GetDefaultSQL()

{

  char     tmp_database_table_name [ 500 ] ;

  strcpy ( tmp_database_table_name,    "["                           ) ;
  strcat ( tmp_database_table_name,    c -> database_table_name      ) ;
  strcat ( tmp_database_table_name,    "]"                           ) ;
 
  return _T ( tmp_database_table_name ) ;

}

/**********************************************************************/

void class_xc_db_odbc_set::DoFieldExchange(CFieldExchange* pFX)

{                                                                                   

  //{{AFX_FIELD_MAP(class_xc_db_odbc_set)

    pFX->SetFieldType(CFieldExchange::outputColumn);

    RFX_Long   ( pFX, _T ( "[TABLE_ID]"      ), m_timer_table_id_field                    ) ;
    RFX_Int    ( pFX, _T ( "[DEVICE_NUM]"    ), m_timing_device_field                     ) ;
    RFX_Long   ( pFX, _T ( "[RECORD_NUM]"    ), m_record_number_for_device_field          ) ;
    RFX_Int    ( pFX, _T ( "[EVENT_NUM]"     ), m_event_number_field                      ) ;
    RFX_Int    ( pFX, _T ( "[EVENT_NUM]"     ), m_heat_number_field                       ) ;
    RFX_Int    ( pFX, _T ( "[CHANNEL]"       ), m_timer_channel_for_device_field          ) ;

    RFX_Text   ( pFX, _T ( "[RECORD_TYP]"    ), m_record_type_string_field                ) ;
    RFX_Text   ( pFX, _T ( "[USERSTRING]"    ), m_total_user_string_field                 ) ;
    RFX_Text   ( pFX, _T ( "[USER1FIELD]"    ), m_user1_string_field                      ) ;
    RFX_Text   ( pFX, _T ( "[USER2FIELD]"    ), m_user2_string_field                      ) ;
    RFX_Text   ( pFX, _T ( "[USER3FIELD]"    ), m_user3_string_field                      ) ;
    RFX_Text   ( pFX, _T ( "[USER4FIELD]"    ), m_user4_string_field                      ) ;
    RFX_Text   ( pFX, _T ( "[BIB_STRING]"    ), m_bib_string_field                        ) ;
    RFX_Text   ( pFX, _T ( "[TIMER_TIME]"    ), m_timer_time_string_field                 ) ;
    RFX_Text   ( pFX, _T ( "[PC_TIME]"       ), m_pc_time_string_field                    ) ;
    RFX_Text   ( pFX, _T ( "[NOTE]"          ), m_note_string_field                       ) ;

  //}}AFX_FIELD_MAP

}

/*****************************************************************************************************************************/
/*****************************************************************************************************************************/

#if ( 0 )

int DLL_EXPORT odbc_add_new_record_to_recordset  ( XC_TIMER_RECORD_STRUCTURE_TYPE * d ) /* function declaration */

{
 
  CString xscratch ;
  int return_value ;

  return_value = 1 ;

 // LRY added the arguments which should not be necessary

  m_odbc_data_set . Open( CRecordset::dynaset, NULL, CRecordset::appendOnly );

  if ( m_odbc_data_set . IsOpen() == 0 ) {
    return_value = -1 ;
  }

  else {         

    if ( m_odbc_data_set . CanAppend () == 0 ) MessageBox ( GetFocus(), "Cannot Append to Recordset", "DB Error Message", MB_OK);
    if ( m_odbc_data_set . CanUpdate () == 0 ) MessageBox ( GetFocus(), "Cannot Update Recordset",    "DB Error Message", MB_OK);

    m_odbc_data_set . AddNew();

    m_odbc_data_set . m_timer_table_id_field                     = (long)  d -> timer_table_id_field            ;
    m_odbc_data_set . m_timing_device_field                      = (int)   d -> timing_device_field             ;
    m_odbc_data_set . m_record_number_for_device_field           = (long)  d -> record_number_for_device_field  ; 
    m_odbc_data_set . m_event_number_field                       = (long)  d -> event_number_field  ; 
    m_odbc_data_set . m_heat_number_field                        = (long)  d -> heat_number_field   ; 
    m_odbc_data_set . m_timer_channel_for_device_field           = (long)  d -> timer_channel_for_device_field  ; 

    m_odbc_data_set . m_record_type_string_field        . Format ( "%s",  d -> record_type_string_field      )  ; 
    m_odbc_data_set . m_total_user_string_field         . Format ( "%s",  d -> total_user_string_field       )  ;
    m_odbc_data_set . m_user1_string_field              . Format ( "%s",  d -> user_string_array_field [ 0 ] )  ;
    m_odbc_data_set . m_user2_string_field              . Format ( "%s",  d -> user_string_array_field [ 1 ] )  ;
    m_odbc_data_set . m_user3_string_field              . Format ( "%s",  d -> user_string_array_field [ 2 ] )  ;

    m_odbc_data_set . m_user4_string_field              . Format ( "%s",  d -> user_string_array_field [ 3 ] )  ;
    m_odbc_data_set . m_bib_string_field                . Format ( "%s",  d -> bib_string_field              )  ;
    m_odbc_data_set . m_timer_time_string_field         . Format ( "%s",  d -> timer_time_string_field       )  ;
    m_odbc_data_set . m_pc_time_string_field            . Format ( "%s",  d -> pc_time_string_field          )  ;
    m_odbc_data_set . m_note_string_field               . Format ( "%s",  d -> note_string_field             )  ;


   if ( m_odbc_data_set . Update() == 0 ) MessageBox ( GetFocus(), "ODBC Update Failed", "DB Diagnostics", MB_OK);

   m_odbc_data_set . Close();

  }

  return return_value ;

}

#endif

/*****************************************************************************************************************************/
/*****************************************************************************************************************************/

#if ( 0 ) 

class_xc_db_odbc_set   m_odbc_data_set ( & cdatabase_class ) ;

int DLL_EXPORT odbc_test_recordset_connection  ( void ) /* function declaration */

{


  int return_value = 1 ;

  char tmp_odbc_data_source_name [ 400 ] ;

  sprintf ( tmp_odbc_data_source_name,  "DSN=%s", cfg -> database_odbc_data_source_name  ) ;

  if ( return_value >= 0 ) {

    m_odbc_data_set . Open() ;

    if ( m_odbc_data_set. IsOpen() == 0 ) {
      return_value = -2 ;
    }

    else {
      m_odbc_data_set . Close();
    }

  }

  return return_value ;

}

#endif

/*****************************************************************************************************************************/
/*****************************************************************************************************************************/

// class_xc_db_odbc_set diagnostics

#ifdef _DEBUG
void class_xc_db_odbc_set::AssertValid() const
{
    CRecordset::AssertValid();
}
#endif //_DEBUG

/**********************************************************************/

#ifdef _DEBUG
void class_xc_db_odbc_set::Dump(CDumpContext& dc) const
{
    CRecordset::Dump(dc);
}
#endif //_DEBUG

/**********************************************************************/
/**********************************************************************/
