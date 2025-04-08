// srt_data_acquisition_module_tester_dialog_box.h : header file
//

#if !defined(AFX_SRT_DATA_ACQUISITION_MODULE_TESTER_DIALOG_BOX_H__B5E61DB8_5FD8_4876_930B_33637F2CEDDA__INCLUDED_)
#define AFX_SRT_DATA_ACQUISITION_MODULE_TESTER_DIALOG_BOX_H__B5E61DB8_5FD8_4876_930B_33637F2CEDDA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// class_data_acquisition_module_tester_dialog_box dialog

class class_data_acquisition_module_tester_dialog_box : public CDialog
{
// Construction
public:
	class_data_acquisition_module_tester_dialog_box(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(class_data_acquisition_module_tester_dialog_box)
	enum { IDD = IDD_SRT_DATA_ACQUISITION_MODULE_TESTER_DIALOG };
	CString	m_valid_records_field_string;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(class_data_acquisition_module_tester_dialog_box)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(class_data_acquisition_module_tester_dialog_box)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

    unsigned int m_TimerId;      // OnTimer


};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SRT_DATA_ACQUISITION_MODULE_TESTER_DIALOG_BOX_H__B5E61DB8_5FD8_4876_930B_33637F2CEDDA__INCLUDED_)
