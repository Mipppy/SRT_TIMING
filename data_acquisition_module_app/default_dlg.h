#if !defined(AFX_DEFAULT_DLG_H__8F43D472_3737_11D5_BD1D_204C4F4F5020__INCLUDED_)
#define AFX_DEFAULT_DLG_H__8F43D472_3737_11D5_BD1D_204C4F4F5020__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// default_dlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// default_dlg dialog

class default_dlg : public CDialog
{
// Construction
public:
	default_dlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(default_dlg)
	enum { IDD = IDD_DIALOG_DEFAULT };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(default_dlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(default_dlg)
	virtual void OnCancel();
	virtual void OnOK();
	afx_msg void OnClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DEFAULT_DLG_H__8F43D472_3737_11D5_BD1D_204C4F4F5020__INCLUDED_)
