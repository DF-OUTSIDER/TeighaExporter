#if !defined(AFX_MGTEST_H__EA64A3E9_F6BC_11D3_A846_00500472CA36__INCLUDED_)
#define AFX_MGTEST_H__EA64A3E9_F6BC_11D3_A846_00500472CA36__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// mgTest.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CmgTest dialog
class CmgTest : public CDialog
{
// Construction
public:
	CmgTest(CWnd* pParent = NULL);   // standard constructor
	BOOL OnInitDialog (void);
	void SetEllipsoid (const char *elKeyName);
	const char *GetEllipsoid (void);
	void SetMgrsString (const char *mgrsString);
	const char* GetMgrsString (void);

// Dialog Data
	//{{AFX_DATA(CmgTest)
	enum { IDD = IDD_MGTST_DIALOG };
	CComboBox	m_ZoneCombo;
	BOOL	m_Bessel;
	CString	m_MgrsString;
	int		m_Precision;
	CString	m_Ellipsoid;
	double	m_Latitude;
	double	m_Longitude;
	double	m_Easting;
	double	m_Northing;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CmgTest)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CmgTest)
	afx_msg void OnMgtstHelp();
	afx_msg void OnMgtstLatlngbtn();
	afx_msg void OnMgtstBessel();
	afx_msg void OnMgtstMgrsbtn();
	virtual void OnCancel();
	afx_msg void OnMgtstUtmbtn();
	afx_msg void OnMgtstElselect();
	afx_msg void OnZonespin(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	void ResetDisplayItems ();

private:
	struct cs_Mgrs_ *m_MgrsPtr;
	char m_ElKeyName [32];
	int m_Zone;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MGTEST_H__EA64A3E9_F6BC_11D3_A846_00500472CA36__INCLUDED_)
