
// xc_timing_db_set.h : interface of the class_xc_db_odbc_set class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_XC_TIMING_DB_SET_H__F148F42F_2A0B_11D5_BD09_204C4F4F5020__INCLUDED_)
#define      AFX_XC_TIMING_DB_SET_H__F148F42F_2A0B_11D5_BD09_204C4F4F5020__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class class_xc_db_odbc_set : public CRecordset

{

public:
    class_xc_db_odbc_set(CDatabase* pDatabase = NULL);
    DECLARE_DYNAMIC(class_xc_db_odbc_set)

// Field/Param Data

    //{{AFX_FIELD(class_xc_db_odbc_set, CRecordset)

    long      m_timer_table_id_field             ;
    int       m_timing_device_field              ;
    long      m_record_number_for_device_field   ;
    int       m_event_number_field   ;
    int       m_heat_number_field    ;
    int       m_timer_channel_for_device_field   ;

    CString   m_record_type_string_field         ;
    CString   m_total_user_string_field          ;
    CString   m_user1_string_field               ;
    CString   m_user2_string_field               ;
    CString   m_user3_string_field               ;
    CString   m_user4_string_field               ;
    CString   m_bib_string_field                 ;
    CString   m_timer_time_string_field          ;
    CString   m_pc_time_string_field             ;
    CString   m_note_string_field                ;

    //}}AFX_FIELD

// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(class_xc_db_odbc_set)
    public:
    virtual CString GetDefaultConnect();    // Default connection string
    virtual CString GetDefaultSQL();          // default SQL for Recordset
    virtual void DoFieldExchange(CFieldExchange* pFX);  // RFX support
    //}}AFX_VIRTUAL

// Implementation
#ifdef _DEBUG
    virtual void AssertValid() const;
    virtual void Dump(CDumpContext& dc) const;
#endif

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_XC_TIMING_DB_SET_H__F148F42F_2A0B_11D5_BD09_204C4F4F5020__INCLUDED_)

