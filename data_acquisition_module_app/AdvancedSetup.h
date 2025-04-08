#if !defined(AFX_ADVANCEDSETUP_H__893A5531_F7C9_11D5_BE4E_204C4F4F5020__INCLUDED_)
#define AFX_ADVANCEDSETUP_H__893A5531_F7C9_11D5_BE4E_204C4F4F5020__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AdvancedSetup.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAdvancedSetup dialog

class CAdvancedSetup : public CDialog
{
// Construction
public:
	CAdvancedSetup(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CAdvancedSetup)
	enum { IDD = IDD_DIALOG1 };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAdvancedSetup)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAdvancedSetup)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ADVANCEDSETUP_H__893A5531_F7C9_11D5_BE4E_204C4F4F5020__INCLUDED_)
