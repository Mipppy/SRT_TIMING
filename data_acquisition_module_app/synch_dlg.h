
#if !defined(AFX_SYNCH_DLG_H__8F43D471_3737_11D5_BD1D_204C4F4F5020__INCLUDED_)
#define AFX_SYNCH_DLG_H__8F43D471_3737_11D5_BD1D_204C4F4F5020__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// synch_dlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// synch_dlg dialog

class synch_dlg : public CDialog
{
// Construction
public:
	synch_dlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(synch_dlg)
	enum { IDD = IDD_DIALOG_SYNCH };
	CString	m_user_synch_time;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(synch_dlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(synch_dlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonSynchToSystem();
	virtual void OnCancel();
	afx_msg void OnButtonSynchToUserTime();
	afx_msg void OnButtonAdjustSynchTime();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SYNCH_DLG_H__8F43D471_3737_11D5_BD1D_204C4F4F5020__INCLUDED_)
