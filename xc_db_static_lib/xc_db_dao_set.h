// daoSet.h : interface of the class_xc_db_dao_set class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_DAOSET_H__380DEEA1_5380_11D5_BD40_204C4F4F5020__INCLUDED_)
#define AFX_DAOSET_H__380DEEA1_5380_11D5_BD40_204C4F4F5020__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class class_xc_db_dao_set : public CDaoRecordset
{
public:
	class_xc_db_dao_set(CDaoDatabase* pDatabase = NULL);
	DECLARE_DYNAMIC(class_xc_db_dao_set)

// Field/Param Data
	//{{AFX_FIELD(class_xc_db_dao_set, CDaoRecordset)

    double    m_raw_table_event_counter_field    ;
    double    m_timing_device_field              ;
    double    m_record_number_for_device_field   ;
    double    m_timer_channel_for_device_field   ;

    CString   m_total_user_string_field          ;
    CString   m_user1_string_field               ;
    CString   m_user2_string_field               ;
    CString   m_user3_string_field               ;
    CString   m_user4_string_field               ;
    CString   m_bib_string_field                 ;
    CString   m_timer_time_string_field          ;
    CString   m_pc_time_string_field             ;

#if ( 0 ) 

    long      m_raw_table_event_counter_field  ;
    short     m_timing_device_field              ;
    long      m_record_number_for_device_field   ;
    short     m_timer_channel_for_device_field   ;

    CString   m_total_user_string_field          ;
    CString   m_user1_string_field               ;
    CString   m_user2_string_field               ;
    CString   m_user3_string_field               ;
    CString   m_user4_string_field               ;
    CString   m_bib_string_field                 ;
    CString   m_timer_time_string_field          ;
    CString   m_pc_time_string_field             ;

#endif

	//}}AFX_FIELD

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(class_xc_db_dao_set)
	public:
	virtual CString GetDefaultDBName();		// REVIEW:  Get a comment here
	virtual CString GetDefaultSQL(); 	// default SQL for Recordset
	virtual void DoFieldExchange(CDaoFieldExchange* pFX);	// RFX support
	//}}AFX_VIRTUAL

// Implementation
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DAOSET_H__380DEEA1_5380_11D5_BD40_204C4F4F5020__INCLUDED_)

