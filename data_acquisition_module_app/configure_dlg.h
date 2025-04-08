
#if !defined(AFX_CONFIGURE_DLG_H__83B6AF93_3286_11D5_BD14_204C4F4F5020__INCLUDED_)
#define AFX_CONFIGURE_DLG_H__83B6AF93_3286_11D5_BD14_204C4F4F5020__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// configure_dlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// configure_dlg dialog

class configure_dlg : public CDialog
{
// Construction
public:
	configure_dlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(configure_dlg)
	enum { IDD = IDD_CONFIGURE_DIALOG };
	CString	m_comm_port_string;
    UINT    m_number_of_devices_used                       ;
	CString	m_current_working_directory;
	CString	m_database_type;
	CString	m_database_table_name;
	CString	m_database_name;
	CString	m_database_directory;
	CString	m_database_drive;
	CString	m_database_path_and_file_name;
	CString	m_timer_type_used;
	CString	m_database_extension;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(configure_dlg)
	public:
    virtual void get_configuration_variables ( void ) ; /* function declaration */
    virtual void config_get_database_path_and_name ( void ) ;/* function declaration */
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(configure_dlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnButtonRestoreDefaultValues();
	afx_msg void OnKillfocusEditDatabaseDrive();
	afx_msg void OnKillfocusEditDatabaseDirectory();
	afx_msg void OnButtonErniesCancel();
	afx_msg void OnButtonAdvanced();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CONFIGURE_DLG_H__83B6AF93_3286_11D5_BD14_204C4F4F5020__INCLUDED_)
