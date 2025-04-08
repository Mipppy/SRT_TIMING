#if !defined(AFX__EVENT_AND_HEAT_DLG_H__95E50201_758E_11D6_BF2E_204C4F4F5020__INCLUDED_)
#define AFX__EVENT_AND_HEAT_DLG_H__95E50201_758E_11D6_BF2E_204C4F4F5020__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// _Event_and_Heat_Dlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// c_event_and_heat_dlg dialog

class c_event_and_heat_dlg : public CDialog
{
// Construction
public:
	c_event_and_heat_dlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(c_event_and_heat_dlg)
	enum { IDD = IDD_DIALOG_SET_EVENT_AND_HEAT };
	UINT	m_device_number;
	UINT	m_event_number;
	UINT	m_heat_number;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(c_event_and_heat_dlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(c_event_and_heat_dlg)
	afx_msg void OnButtonSendToEveryDevice();
	afx_msg void OnButtonSendToOneDevice();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX__EVENT_AND_HEAT_DLG_H__95E50201_758E_11D6_BF2E_204C4F4F5020__INCLUDED_)
