
// timer_appDlg.h : header file
//

#if !defined(AFX_TIMER_APPDLG_H__CBD31219_2993_11D5_BD09_204C4F4F5020__INCLUDED_)
#define AFX_TIMER_APPDLG_H__CBD31219_2993_11D5_BD09_204C4F4F5020__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CTimer_appDlg dialog

class CTimer_appDlg : public CDialog
{
// Construction
public:
	CTimer_appDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CTimer_appDlg)
	enum { IDD = IDD_TIMER_APP_DIALOG };
	long	m_time_counts;
	CString	m_time_of_day_string;
	CString	m_field_description_string;
	CString	m_terminal_text_box_string;
	CString	m_database_text_box_string;
	BOOL	m_append_to_database_flag;
	CString	m_version_string;
	UINT	m_heat_number;
	int		m_event_number;
	BOOL	m_communicate_with_timers_flag;
	BOOL	m_link_to_database_flag;
	CString	m_link_status_string;
	CString	m_comm_status_string;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTimer_appDlg)
	public:
    virtual void dlg_force_database_text_box_update ( XC_TIMER_RECORD_STRUCTURE_TYPE * d ) ;
    virtual void dlg_force_terminal_text_box_update ( char * terminal_string ) ;
    virtual void exit_function                      ( void ) ;
    virtual void delete_database_records            ( void ) ;

	protected:
    virtual void OnCancel() ;
 	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CTimer_appDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnButtonResetTimers();
	afx_msg void OnButtonSynchTime();
	afx_msg void OnButtonConfigure();
	afx_msg void OnButtonReTransmit();
	afx_msg void OnButtonExit();
	afx_msg void OnHelp();
	afx_msg void OnCheckAppendToDatabaseFlag();
	afx_msg void OnCheckAppendToDatabase();
	afx_msg void OnButtonDeleteRecords();
	afx_msg void OnButtonOdbcTest();
	afx_msg void OnButtonTest();
	afx_msg void OnButtonGenerateDummyRecord();
	afx_msg void OnChangeEditEvent();
	afx_msg void OnKillfocusEditEvent();
	afx_msg void OnChangeEditEventAndHeat();
	afx_msg void OnButtonResetX();
	afx_msg void OnButtonSendEventAndHeatButton();
	afx_msg void OnCheckCommunicateWithTimers();
	afx_msg void OnCheckLinkToDatabase();
	afx_msg void OnButtonDeleteRecordsFromBackupFile();
	afx_msg void OnButtonDeleteBackupRecords();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

    unsigned int m_TimerId;      // OnTimer

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TIMER_APPDLG_H__CBD31219_2993_11D5_BD09_204C4F4F5020__INCLUDED_)
