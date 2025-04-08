#if !defined(AFX_DIALOGADVANCEDSETUP_H__893A5532_F7C9_11D5_BE4E_204C4F4F5020__INCLUDED_)
#define AFX_DIALOGADVANCEDSETUP_H__893A5532_F7C9_11D5_BE4E_204C4F4F5020__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DialogAdvancedSetup.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDialogAdvancedSetup dialog

class CDialogAdvancedSetup : public CDialog
{
// Construction
public:
	CDialogAdvancedSetup(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDialogAdvancedSetup)
	enum { IDD = IDD_DIALOG_ADVANCED_SETUP };
	UINT	m_initial_device1_record;
	UINT	m_initial_device2_record;
	UINT	m_initial_device3_record;
	UINT	m_initial_device4_record;
	UINT	m_initial_device5_record;
	UINT	m_initial_device6_record;
	UINT	m_initial_device7_record;
	UINT	m_initial_device8_record;
	UINT	m_initial_device9_record;
	long	m_baud_rate;
	BOOL	m_output_diagnostic_files_flag;
	UINT	m_number_of_bytes_to_program_hex;
	long	m_database_highest_record_number;
	CString	m_odbc_user_data_sourse_name;
	BOOL	m_odbc_user_dsn_flag;
	int		m_talk_time;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialogAdvancedSetup)
    public:
    virtual void get_configuration_variables ( void ) ; /* function declaration */
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDialogAdvancedSetup)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnButtonErniesCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIALOGADVANCEDSETUP_H__893A5532_F7C9_11D5_BE4E_204C4F4F5020__INCLUDED_)
