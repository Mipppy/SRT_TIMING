#if !defined(AFX_FLASH_DLG_H__B84D9F20_33A3_11D5_BD14_204C4F4F5020__INCLUDED_)
#define AFX_FLASH_DLG_H__B84D9F20_33A3_11D5_BD14_204C4F4F5020__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// flash_dlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// flash_dlg dialog

class flash_dlg : public CDialog
{
// Construction
public:
	flash_dlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(flash_dlg)
	enum { IDD = IDD_FLASH_DIALOG };
	CString	m_time_of_day_string;
	CString	m_terminal_text_box_string;
	CString	m_status_string;
	UINT	m_percent_done;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(flash_dlg)
    public:
    virtual void dlg_force_terminal_text_box_update ( char * terminal_string ) ;

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(flash_dlg)
	afx_msg void OnTimer(UINT nIDEvent);
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

    unsigned int m_TimerId;      // OnTimer

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FLASH_DLG_H__B84D9F20_33A3_11D5_BD14_204C4F4F5020__INCLUDED_)
