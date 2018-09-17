/*
 * Copyright (c) 2008, Autodesk, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the Autodesk, Inc. nor the names of its
 *       contributors may be used to endorse or promote products derived
 *       from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY Autodesk, Inc. ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL Autodesk, Inc. OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

/**********************************************************************
**	The Windows debuggin stuff can be royal pain.  We disable it here.
**********************************************************************/
#if defined (_DEBUG)
#	define _DEBUG_WAS_DEFINED
#	undef _DEBUG
#	define NDEBUG
#endif

/**********************************************************************
**	Defines, etc. required for the MFC dialog box functions.
**********************************************************************/

/*	Have we been here before? */

#include <afxwin.h>		// MFC core and standard components
#include <afxext.h>		// MFC extensions
#include <afxcmn.h>		// MFC support for Windows Common Controls
#include "cs_Wchar.hpp"	// Narrow/Wide character mapping
#include "cs_res.h"		// Resource File define's

// Functions to deal with TCHAR
#define tchrCount(array) (sizeof (array) / sizeof (array[0]))

struct csT_Csgrplst_
{
	TCHAR key_nm [24];
	TCHAR descr [64];
	TCHAR source [64];
	TCHAR ref_typ [10];
	TCHAR ref_to [24];
	TCHAR unit [16];
};

wchar_t* CSt_strNCpy (wchar_t* trg,const wchar_t* src,unsigned chCnt);
wchar_t* CSt_strNCpy (wchar_t* trg,const char* src,unsigned chCnt);
char* CSt_strNCpy (char* trg,const wchar_t* src,unsigned chCnt);
char* CSt_strNCpy (char* trg,const char* src,unsigned chCnt);
int	CSt_stricmp (const TCHAR* cp1,const TCHAR* cp2);
int	CSt_strnicmp (const TCHAR* cp1,const TCHAR* cp2,unsigned count);
void CSt_cstringAssign (CString& cString,const char* ccPtr);

void CSt_errmsg (TCHAR* bufr,unsigned bufrSize);
int CSt_csIsValid (const TCHAR* tcKeyName);
int CSt_dtIsValid (const TCHAR* tcKeyName);
int CSt_elIsValid (const TCHAR* tcKeyName);
long32_t CSt_atof (double *result,const TCHAR* value);
long32_t CSt_ftoa (TCHAR* bufr,int size,double value,long32_t frmt);
int CSt_elSelector  (TCHAR* elKeyName);
struct cs_Eldef_* CSt_eldef (const TCHAR* el_nam);
int CSt_getEllipsoidOf (const TCHAR* csKeyName,TCHAR* ellipsoidName,int size);
double CSt_unitlu (short type,const TCHAR* name);
int CSt_isgeo (const TCHAR* csys);
int CSt_getUnitsOf (const TCHAR* csKeyName,TCHAR* unitName,int size);
struct cs_Dtdef_* CSt_dtdef (const TCHAR* dat_nam);
int CSt_getDatumOf (const TCHAR* csKeyName,TCHAR* datumName,int size);
struct cs_Datum_* CSt_dtloc (const TCHAR* dat_nam);
struct cs_Csdef_* CSt_csdef (const TCHAR* keyName);
struct cs_Csprm_* CSbcclu_T (const TCHAR* csKeyName);

int CSt_csEnum (int index,TCHAR* keyName,unsigned size);
int	CSt_prjEnum (int index,ulong32_t *prj_flags,TCHAR* prj_keynm,
													int keynm_sz,
													TCHAR* prj_descr,
													int descr_sz);
int	CSt_quadEnum (int index,TCHAR* descr,int size,int *ident);
int CSt_unEnum (int index,int type,TCHAR* un_name,int un_size);
int	CSt_viaEnum (int index,TCHAR* descr,int size,int *ident);
int	CSt_dtEnum (int index,TCHAR* key_name,int size);
int	CSt_elEnum (int index,TCHAR* key_name,int size);

int	CSt_csGrpEnum (int index,TCHAR* grp_name,int name_sz,TCHAR* grp_dscr,int dscr_sz);
int CSt_csEnumByGroup (int index,const TCHAR* grp_name,struct csT_Csgrplst_ *cs_descr);

int CSt_dtSelector (TCHAR* dtKeyName);
int CSt_csBrowser (TCHAR* csKeyName);
int CSt_dtSelectorA (TCHAR* dtKeyName,TCHAR* description,TCHAR* source,TCHAR* ellipsoid,TCHAR* technique);
int CSt_elSelectorA (TCHAR* elKeyName,TCHAR* description,TCHAR* source);
int CSt_newKeyName (TCHAR* keyName,int type,int preserve);

void CSt_csfnm (const TCHAR* new_name);
void CSt_dtfnm (const TCHAR* new_name);
void CSt_elfnm (const TCHAR* new_name);
int	CSt_altdr (const TCHAR* alt_dir);
int	CSt_altdr (TCHAR* alt_dir);
int	CSt_access (const TCHAR* path,int mode);

//	Some function prototypes; used in the DoDataExchange sequence.
struct cs_Prjtab_ *PrjLookUp (const char *prjKeyName);
void AFXAPI DDX_CharArray    (CDataExchange* pDX,int nIDC,TCHAR* data,int count);
void AFXAPI DDX_StaticLabel  (CDataExchange* pDX,int nIDC,const char* data);
void AFXAPI DDX_StaticLabelT (CDataExchange* pDX,int nIDC,const TCHAR* data);
void AFXAPI DDX_PrjParm      (CDataExchange* pDX,int nIDC,double& value,struct cs_Prjprm_* prmType);
void AFXAPI DDX_ZoneNbr      (CDataExchange* pDX,int nIDC,double& value);
void AFXAPI DDX_HemiNS       (CDataExchange* pDX,int nIDC,double& value);
long32_t AFXAPI DDX_Number       (CDataExchange* pDX,int nIDC,double& value,struct cs_Prjprm_ *prmType);
void AFXAPI DDX_Scale        (CDataExchange* pDX,int nIDC,double& value,bool reduction);
long32_t AFXAPI DDX_Coord        (CDataExchange* pDX,int nIDC,double& value,long32_t* format);

////////////////////////////////////////////////////////////////////////////
// CcsBrowse dialog
class CcsBrowse : public CDialog
{
// Construction
public:
	CcsBrowse(CWnd* pParent = NULL);   // standard constructor
	BOOL OnInitDialog (void);
	void SetInitialKeyName (const TCHAR* keyName);
	const TCHAR* GetSelectedKeyName (void);
	void RebuildKeynameCombo (const TCHAR* grpName);

// Dialog Data
	//{{AFX_DATA(CcsBrowse)
	enum { IDD = IDD_BRWS_CS };
	CComboBox	m_KeyNameCombo;
	CComboBox	m_GroupCombo;
	CString	m_Description;
	CString	m_RefTo;
	CString	m_Source;
	CString	m_Unit;
	//}}AFX_DATA
	CString m_KeyName;
	CString m_Group;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CcsBrowse)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	void OnOK (void);

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CcsBrowse)
	afx_msg void OnHelp();
	afx_msg void OnSelendokBrcsGroup();
	afx_msg void OnSelchangeBrcsKeynm();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
/////////////////////////////////////////////////////////////////////////////
// CcsDualBrowse dialog
class CcsDualBrowse : public CDialog
{
// Construction
public:
	CcsDualBrowse(CWnd* pParent = NULL);
	BOOL OnInitDialog (void);
	void SetSrcKeyName (const TCHAR* srcKeyName);
	void SetTrgKeyName (const TCHAR* trgKeyName);
	const TCHAR* GetSrcKeyName (void) const {return (LPCTSTR)m_Source; }
	const TCHAR* GetTrgKeyName (void) const {return (LPCTSTR)m_Target; }

// Dialog Data
	//{{AFX_DATA(CcsDualBrowse)
	enum { IDD = IDD_BRWS_CS2 };
	CComboBox	m_TrgKeyName;
	CComboBox	m_TrgGroup;
	CComboBox	m_SrcKeyName;
	CComboBox	m_SrcGroup;
	CString	m_SrcDescription;
	CString	m_SrcReference;
	CString	m_SrcRefLabel;
	CString	m_SrcSource;
	CString	m_SrcUnit;
	CString	m_TrgDescription;
	CString	m_TrgReference;
	CString	m_TrgRefLabel;
	CString	m_TrgSource;
	CString	m_TrgUnit;
	//}}AFX_DATA
	CString m_Source;
	CString m_Target;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CcsDualBrowse)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CcsDualBrowse)
	afx_msg void OnCsbr2Help();
	virtual void OnCancel();
	virtual void OnOK();
	afx_msg void OnChangeSrcGroup();
	afx_msg void OnChangeTrgGroup();
	afx_msg void OnChangeTrgKeyNm();
	afx_msg void OnChangeSrcKeyNm();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void RebuildKeyNameCombo (CComboBox& comboBox,const TCHAR* grpName);
};
/////////////////////////////////////////////////////////////////////////////
// CcsDataDir dialog
class CcsDataDir : public CDialog
{
// Construction
public:
	CcsDataDir(CWnd* pParent = NULL);   // standard constructor
	BOOL OnInitDialog (void);
	bool GetIniSave (void) {return (m_SaveInINI != 0); };
	bool GetRegSave (void) {return (m_SaveInRegistry != 0); };
	void EnableINIBox (void) {m_EnableINI = true;}
	void EnableRegBox (void) {m_EnableReg = true;}

// Dialog Data
	//{{AFX_DATA(CcsDataDir)
	enum { IDD = IDD_MPDR_DIALOG };
	CString	m_CSName;
	CString	m_DataDirectory;
	CString	m_ElName;
	CString	m_DtName;
	BOOL	m_SaveInINI;
	BOOL	m_SaveInRegistry;
	//}}AFX_DATA
	bool m_EnableINI;
	bool m_EnableReg;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CcsDataDir)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	virtual void OnOK (void);

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CcsDataDir)
	afx_msg void OnMpdrHelp();
	afx_msg void OnBtnDirbrwse();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

class CcsEdit;
///////////////////////////////////////////////////////////////////////////////
//	The following structure is used to build a table, which in turn is used
//	to make a lot of the code dealing with Projection Parameters table driven;
//	thus eliminating alot of code which can go awry.
struct TcsEdParmMap
{
	int	prmNbr;
	UINT lblID;
	UINT valID;
};

/////////////////////////////////////////////////////////////////////////////
// CcsEdIdent dialog
class CcsEdIdent : public CPropertyPage
{
// Construction
public:
	CcsEdIdent();
	~CcsEdIdent();
	BOOL OnInitDialog (void);
	void RebuildComboBox (void);

// Dialog Data
	//{{AFX_DATA(CcsEdIdent)
	enum { IDD = IDD_EDCS_IDNT };
	CComboBox	m_GroupCombo;
	CButton	m_BrowseBtn;
	CComboBox	m_KeyNameCombo;
	CString	m_CountrySt;
	CString	m_Description;
	CString	m_Source;
	CString	m_Location;
	//}}AFX_DATA
	CString m_Group;
	CcsEdit *m_PropSheet;

// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CcsEdIdent)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CcsEdIdent)
	afx_msg void OnCsidBrowse();
	afx_msg void OnHelpButton ();
	afx_msg void OnChange();
	afx_msg void OnSelendokKeyName();
	afx_msg void OnKillfocusCsidDescr();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

/////////////////////////////////////////////////////////////////////////////
// CcsEdGnrl dialog
class CcsEdGnrl : public CPropertyPage
{
// Construction
public:
	CcsEdGnrl();
	~CcsEdGnrl();
	BOOL OnInitDialog (void);
	BOOL OnSetActive (void);
	void RebuildUnits (int unitType);
	void SetScaleRatio (CString& result,int nIDC,double scale,bool reduction);

// Dialog Data
	//{{AFX_DATA(CcsEdGnrl)
	enum { IDD = IDD_EDCS_GENRL };
	CComboBox	m_UnitCombo;
	CComboBox	m_QuadCombo;
	CComboBox	m_Projection;
	CString	m_MapRatio;
	CString	m_RedRatio;
	CString	m_RefKeyName;
	CString	m_RefType;
	BOOL	m_RefToDflt;
	BOOL	m_UnitDflt;
	//}}AFX_DATA
	CString m_PrjKeyName;
	CString m_Unit;
	double m_SclRed;
	double m_MapScale;
	short m_Quad;

// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CcsEdGnrl)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	BOOL OnKillActive (void);

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CcsEdGnrl)
	afx_msg void OnCsgnDatumBtn();
	afx_msg void OnChange();
	afx_msg void OnHelpButton ();
	afx_msg void OnSelendokCsgnPrjctn();
	afx_msg void OnSelendokCsgnQuad();
	afx_msg void OnSelendokCsgnRefto();
	afx_msg void OnSelendokCsgnUnit();
	afx_msg void OnCsgnElpbtn();
	afx_msg void OnCsgnDatbtn();
	afx_msg void OnKillfocusCsgnSclred();
	afx_msg void OnKillfocusCsgnMapscl();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	CcsEdit *m_PropSheet;

};

/////////////////////////////////////////////////////////////////////////////
// CcsEdOrigins dialog
class CcsEdOrigins : public CPropertyPage
{
// Construction
public:
	CcsEdOrigins();
	~CcsEdOrigins();

// Dialog Data
	//{{AFX_DATA(CcsEdOrigins)
	enum { IDD = IDD_EDCS_ORGS };
	//}}AFX_DATA
	double	m_NZeroX;
	double	m_NZeroY;
	double	m_OrgLat;
	double	m_OrgLng;
	double	m_XOff;
	double	m_YOff;

// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CcsEdOrigins)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	BOOL OnKillActive (void);

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CcsEdOrigins)
	afx_msg void OnChange();
	afx_msg void OnHelpButton ();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	CcsEdit *m_PropSheet;
};

/////////////////////////////////////////////////////////////////////////////
// CcsEdExtents dialog
class CcsEdExtents : public CPropertyPage
{
// Construction
public:
	CcsEdExtents();
	~CcsEdExtents();

// Dialog Data
	//{{AFX_DATA(CcsEdExtents)
	enum { IDD = IDD_EDCS_EXTENTS };
	//}}AFX_DATA
	double m_MinLng;
	double m_MinLat;
	double m_MaxLng;
	double m_MaxLat;
	double m_MinX;
	double m_MinY;
	double m_MaxX;
	double m_MaxY;

// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CcsEdExtents)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	BOOL OnKillActive (void);

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CcsEdExtents)
	afx_msg void OnChange();
	afx_msg void OnHelpButton ();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()


public:
	CcsEdit *m_PropSheet;
};

/////////////////////////////////////////////////////////////////////////////
// CcsEdParms1 dialog
class CcsEdParms1 : public CPropertyPage
{
// Construction
public:
	CcsEdParms1();
	~CcsEdParms1();
	BOOL OnSetActive (void);

// Dialog Data
	//{{AFX_DATA(CcsEdParms1)
	enum { IDD = IDD_EDCS_PARMS };
	//}}AFX_DATA
	double	m_PrjParm1;
	double	m_PrjParm2;
	double	m_PrjParm3;
	double	m_PrjParm4;
	double	m_PrjParm5;
	double	m_PrjParm6;

// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CcsEdParms1)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	BOOL OnKillActive (void);

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CcsEdParms1)
	afx_msg void OnChange();
	afx_msg void OnHelpButton ();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()


public:
	CcsEdit *m_PropSheet;
};

/////////////////////////////////////////////////////////////////////////////
// CcsEdParms2 dialog
class CcsEdParms2 : public CPropertyPage
{
// Construction
public:
	CcsEdParms2();
	~CcsEdParms2();
	BOOL OnSetActive (void);

// Dialog Data
	//{{AFX_DATA(CcsEdParms2)
	enum { IDD = IDD_EDCS_PARMS2 };
	//}}AFX_DATA
	double	m_PrjParm10;
	double	m_PrjParm11;
	double	m_PrjParm12;
	double	m_PrjParm7;
	double	m_PrjParm9;
	double	m_PrjParm8;

// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CcsEdParms2)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	BOOL OnKillActive (void);

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CcsEdParms2)
	afx_msg void OnChange();
	afx_msg void OnHelpButton ();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()


public:
	CcsEdit *m_PropSheet;
};

/////////////////////////////////////////////////////////////////////////////
// CcsEdParms3 dialog

class CcsEdParms3 : public CPropertyPage
{
// Construction
public:
	CcsEdParms3();
	~CcsEdParms3();
	BOOL OnSetActive (void);

// Dialog Data
	//{{AFX_DATA(CcsEdParms3)
	enum { IDD = IDD_EDCS_PARMS3 };
	//}}AFX_DATA
	double	m_PrjParm13;
	double	m_PrjParm14;
	double	m_PrjParm15;
	double	m_PrjParm16;
	double	m_PrjParm17;
	double	m_PrjParm18;

// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CcsEdParms3)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	BOOL OnKillActive (void);

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CcsEdParms3)
	afx_msg void OnChange();
	afx_msg void OnHelpButton ();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	CcsEdit *m_PropSheet;
};

/////////////////////////////////////////////////////////////////////////////
// CcsParms4 dialog
class CcsEdParms4 : public CPropertyPage
{
// Construction
public:
	CcsEdParms4();
	~CcsEdParms4();
	BOOL OnSetActive (void);

// Dialog Data
	//{{AFX_DATA(CcsEdParms4)
	enum { IDD = IDD_EDCS_PARMS4 };
	//}}AFX_DATA
	double	m_PrjParm19;
	double	m_PrjParm20;
	double	m_PrjParm21;
	double	m_PrjParm22;
	double	m_PrjParm23;
	double	m_PrjParm24;

// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CcsEdParms4)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	BOOL OnKillActive (void);

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CcsEdParms4)
	afx_msg void OnChange();
	afx_msg void OnHelpButton ();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	CcsEdit *m_PropSheet;
};

/////////////////////////////////////////////////////////////////////////////
// CcsEdit
class CcsEdit : public CPropertySheet
{
// Construction
public:
	CcsEdit(CWnd* pParentWnd = NULL,UINT iSelectPage = 0);
	void SetKeyName (const TCHAR* csKeyName);
	const TCHAR* GetKeyName (void);
	void SetLongitudeFormat (const long32_t frmt);
	void SetLatitudeFormat (const long32_t frmt);
	void SetLatLongFormat (const long32_t frmt);
	void SetXFormat (const long32_t frmt);
	void SetYFormat (const long32_t frmt);
	void SetXYFormat (const long32_t frmt);
	BOOL OnInitDialog (void);
	bool GetCoordSys (const TCHAR* keyName);
	bool NewProjection (const char* prjKeyName);
	void SetButtons (void);
	bool ChangeCoordSys (const TCHAR* keyName);
	void NewCoordSys (const CString& csKeyName);
	int PutCoordSys (void);
	bool DeleteCoordSys (const CString& dtKeyName);
	void FillInData (void);
	void DisperseData (struct cs_Csdef_ *csPtr);
	void CollectData (struct cs_Csdef_ *cs_Ptr);
	void OnChange (void);
	void OnCsEdDelSav (void); 
	void OnCsEdNewCncl (void); 
	void OnCsEdClose (void); 
	BOOL OnCommand(WPARAM wParam,LPARAM lParam);
private:
	void AdjustParmArray (void);
// Attributes
public:
// Operations
public:
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CcsEdit)
	//}}AFX_VIRTUAL
// Implementation
public:
	virtual ~CcsEdit();
	// Generated message map functions
protected:
	//{{AFX_MSG(CcsEdit)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	CcsEdIdent   m_PgIdent;
	CcsEdGnrl    m_PgGnrl;
	CcsEdOrigins m_PgOrigins;
	CcsEdExtents m_PgExtents;
	CcsEdParms1  m_PgParms1;
	CcsEdParms2  m_PgParms2;
	CcsEdParms3  m_PgParms3;
	CcsEdParms4  m_PgParms4;

	CString m_KeyName;
	CString m_PreviousKeyName;
	CString m_Description;
	int m_NewIdx;
	unsigned short m_PrjCode;
	int m_UnitType;
	int m_PrjFlags;
	int m_ParmCount;
	long32_t m_LngFrmt;
	long32_t m_LatFrmt;
	long32_t m_XFrmt;
	long32_t m_YFrmt;
	long32_t m_SclFrmt;
	long32_t m_AnglFrmt;
	bool m_Modified;
	bool m_Protected;

	// The following array is used to label, validate, format, and otherwise
	// process the projection parameters whose type, function, and nature
	// change with each projection.  There is one element in the array for
	// each of the 24 (currently) possible parameters.  The contents of this
	// array is updated whenever a change in projection is sensed.  Note,
	// projection parameters beyond number 2 are rarely used.

	struct cs_Prjprm_ m_LngPrm;
	struct cs_Prjprm_ m_ExLngPrm;
	struct cs_Prjprm_ m_LatPrm;
	struct cs_Prjprm_ m_XPrm;
	struct cs_Prjprm_ m_YPrm;
	struct cs_Prjprm_ m_ZroPrm;

	struct cs_Prjprm_ m_PrjParms [24];
};

/////////////////////////////////////////////////////////////////////////////
// CcsTest dialog
class CcsTest : public CDialog
{
// Construction
public:
	CcsTest(CWnd* pParent = NULL);
	void SetSourceSystem (const TCHAR* srcSystem);
	void SetTargetSystem (const TCHAR* trgSystem);
	void SetSourceCoordinate (const double xy [2]);
	const TCHAR* GetSourceSystem (void);
	const TCHAR* GetTargetSystem (void);
	void GetSourceCoordinate (double xy [2]);
	bool SetLabels (bool target);
	CWnd* Calculate (void);
	void UpdateSource (void);
	void UpdateTarget (void);
	CWnd* FetchSource (void);

// Dialog Data
	//{{AFX_DATA(CcsTest)
	enum { IDD = IDD_CSTST_DIALOG };
	CString	m_SrcConvergence;
	CString	m_SrcDatumKeyName;
	CString	m_SrcDatumLabel;
	CString	m_SrcDescription;
	CString	m_SrcKeyName;
	CString	m_SrcSource;
	CString	m_SrcStatus;
	CString	m_SrcUnit;
	CString	m_SrcXLatitude;
	CString	m_SrcXLatLabel;
	CString	m_SrcYLongitude;
	CString m_SrcZHeight;
	CString	m_TrgConvergence;
	CString	m_TrgDatLabel;
	CString	m_TrgDescription;
	CString	m_TrgDatumKeyName;
	CString	m_TrgGridScale;
	CString	m_TrgKeyName;
	CString	m_TrgSource;
	CString	m_TrgStatus;
	CString	m_TrgUnit;
	CString	m_TrgXLatLabel;
	CString	m_SrcGridScale;
	CString	m_TrgXLatitude;
	CString	m_TrgYLongitude;
	CString m_TrgZHeight;
	BOOL	m_DecimalDegrees;
	BOOL	m_HighPrecision;
	CString	m_SrcYLngLabel;
	CString	m_TrgYLngLabel;
	CString m_SrcZHgtLabel;
	CString m_TrgZHgtLabel;
	BOOL	m_SrcSPZone83;
	BOOL	m_TrgSPZone83;
	BOOL    m_ThreeDCalc;
	//}}AFX_DATA

	double m_SourceXY [3];
	double m_SourceScale;
	double m_SourceConvergence;
	double m_TargetXY [3];
	double m_TargetScale;
	double m_TargetConvergence;

	// Current source values

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CcsTest)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	BOOL OnInitDialog (void);

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CcsTest)
	afx_msg void OnCststCalculate();
	afx_msg void OnCststReverse();
	afx_msg void OnHelp();
	afx_msg void OnKillfocusCststSrckeynm();
	afx_msg void OnKillfocusCststTrgkeynm();
	afx_msg void OnChange();
	afx_msg void OnCststSrccsbtn();
	afx_msg void OnCststTrgcsbtn();
	afx_msg void OnCstst3DCheck();
	afx_msg void OnCststHelp();
	//}}AFX_MSG-
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// CdtEdit dialog
class CdtEdit : public CDialog
{
// Construction
public:
	CdtEdit(CWnd* pParent = NULL);   // standard constructor
	BOOL CdtEdit::OnInitDialog ();

// Dialog Data
	//{{AFX_DATA(CdtEdit)
	enum { IDD = IDD_DTEDITOR };
	CComboBox	m_KeyNameCombo;
	CComboBox	m_284Via;
	double	m_DeltaX;
	double	m_DeltaY;
	double	m_DeltaZ;
	CString	m_Description;
	double	m_RotX;
	double	m_RotY;
	double	m_RotZ;
	double	m_Scale;
	CString	m_Source;
	CString	m_EllipsoidDescr;
	//}}AFX_DATA

	bool m_Protected;
	bool m_Modified;
	int m_NewIdx;
	CString m_KeyName;
	CString m_PreviousKeyName;
	CString m_Ellipsoid;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CdtEdit)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CdtEdit)
	afx_msg void OnDtedDelsav();
	afx_msg void OnChange();
	afx_msg void OnDtedHelp();
	afx_msg void OnDtedNewcncl();
	afx_msg void OnDtedClose();
	afx_msg void OnSelendokDted284via();
	afx_msg void OnSelchangeDtedKeyname();
	afx_msg void OnDtedNewEllipsoid();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	afx_msg void OnCancel ();
	void NewDatum (const CString& dtKeyName);
	bool ChangeDatum (const CString& dtKeyName);
	bool GetDatum (const CString& dtKeyName);
	int  PutDatum (void);	
	bool DeleteDatum (const CString& dtKeyName);
	void CalculateDisplay (void);
	void SetButtons (void);
	void RebuildComboBox (void);
	void NewEllipsoid(void);

public:
	void SetKeyName (const TCHAR* dtKeyName);
	const TCHAR* GetKeyName (void);
};

/////////////////////////////////////////////////////////////////////////////
// CdtSelect dialog
class CdtSelect : public CDialog
{
// Construction
public:
	CdtSelect(CWnd* pParent = NULL);   // standard constructor
	BOOL OnInitDialog (void);
	void SetInitialDatum (const TCHAR* dtKeyName);
	const TCHAR* GetSelectedDatum (void) { return (LPCTSTR)m_KeyName; };
	const TCHAR* GetSelectedDescription (void) {return (LPCTSTR)m_Description; };
	const TCHAR* GetSelectedSource (void) {return (LPCTSTR)m_Source; };
	const TCHAR* GetSelectedEllipsoid (void) {return (LPCTSTR)m_Ellipsoid; };
	const TCHAR* GetSelectedTechnique (void) {return (LPCTSTR)m_Technique; };
	void ShowDetails (void);

// Dialog Data
	//{{AFX_DATA(CdtSelect)
	enum { IDD = IDD_DTSL_DIALOG };
	CComboBox	m_KeyNameCombo;
	CString	m_Description;
	CString	m_Ellipsoid;
	CString	m_Source;
	CString	m_Technique;
	//}}AFX_DATA
	CString m_KeyName;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CdtSelect)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CdtSelect)
	virtual void OnOK();
	afx_msg void OnSelchangeDtslKeyname();
	afx_msg void OnDtslHelp();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// CelEdit dialog
class CelEdit : public CDialog
{
// Construction
public:
	CelEdit(CWnd* pParent = NULL);   // standard constructor
	BOOL OnInitDialog ();

// Dialog Data
	//{{AFX_DATA(CelEdit)
	enum { IDD = IDD_ELEDITOR };
	CComboBox	m_KeyNameCombo;
	CString	m_EsqString;
	CString	m_FlatString;
	CString	m_Source;
	CString	m_Description;
	double	m_EqRadius;
	double	m_PlrRadius;
	//}}AFX_DATA

	bool m_Protected;
	bool m_Modified;
	int m_NewIdx;
	CString m_KeyName;
	CString m_PreviousKeyName;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CelEdit)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CelEdit)
	afx_msg void OnEledClose();
	afx_msg void OnEledHelp();
	afx_msg void OnChange();
	afx_msg void OnEledNewcncl();
	afx_msg void OnChangeRadii();
	afx_msg void OnEledDelsav();
	afx_msg void OnNewSelection();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	afx_msg void OnCancel ();
	void NewEllipsoid (const CString& elKeyName);
	bool ChangeEllipsoid (const CString& elKeyName);
	bool GetEllipsoid (const CString& elKeyName);
	int  PutEllipsoid (void);	
	bool DeleteEllipsoid (const CString& elKeyName);
	void CalculateDisplay (void);
	void SetButtons (void);
	void RebuildComboBox (void);

public:
	void SetKeyName (const TCHAR* elKeyName);
	const TCHAR* GetKeyName (void);
};

/////////////////////////////////////////////////////////////////////////////
// CelSelect dialog
class CelSelect : public CDialog
{
// Construction
public:
	CelSelect(CWnd* pParent = NULL);   // standard constructor
	void SetInitialEllipsoid (const TCHAR* elKeyName);
	const TCHAR* GetSelectedEllipsoid (void) { return (LPCTSTR)m_KeyName; };
	const TCHAR* GetSelectedDescription (void) { return (LPCTSTR)m_Description; };
	const TCHAR* GetSelectedSource (void) { return (LPCTSTR)m_Source; };
	void ShowDetails (void);

// Dialog Data
	//{{AFX_DATA(CelSelect)
	enum { IDD = IDD_ELSL_DIALOG };
	CComboBox	m_KeyNameCombo;
	CString	m_Description;
	CString	m_Source;
	CString	m_Numbers;
	//}}AFX_DATA
	CString m_KeyName;

// Overrides
	BOOL OnInitDialog (void);
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CelSelect)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CelSelect)
	afx_msg void OnElslHelp();
	afx_msg void OnSelchangeElslKeyname();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// CnewKeyName dialog
class CnewKeyName : public CDialog
{
// Construction
public:
	enum TnewKeyNameTypes {NonDict = 0,CoordSys = 1,Datum = 2,Ellipsoid = 3};
	CnewKeyName(TnewKeyNameTypes type,CWnd* pParent = NULL);   // only constructor
	void SetTitle (const TCHAR* title) {m_Title = title; };
	void SetLegend (const TCHAR* legend) {m_Legend = legend; };
	CString GetNewName (void) const {return m_NewKeyName; };
	void SetPreserveEnable (bool flag) {m_PreserveEnabled = flag; };
	bool GetPreserve (void) const {return (m_Preserve != 0); };

// Dialog Data
	//{{AFX_DATA(CnewKeyName)
	enum { IDD = IDD_NEWKEYNM };
	CString	m_NewKeyName;		// New name acquired from user.
	BOOL	m_Preserve;			// True says "Preserve existsing Data" box is checked.
	//}}AFX_DATA
	bool m_PreserveEnabled;
	TnewKeyNameTypes m_Type;	// Indicates the type of key name being acquired.
	CString m_Title;
	CString m_Legend;			// The text which goes in the box above the edit control

// Overrides
	BOOL OnInitDialog (void);
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CnewKeyName)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CnewKeyName)
	virtual void OnOK();
	afx_msg void OnChelp();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// CmgTest dialog
class CmgTest : public CDialog
{
// Construction
public:
	CmgTest(CWnd* pParent = NULL);   // standard constructor
	BOOL OnInitDialog (void);
	void SetEllipsoid (const TCHAR* elKeyName);
	const TCHAR* GetEllipsoid (void);
	void SetMgrsString (const TCHAR* mgrsString);
	const TCHAR* GetMgrsString (void);
	int GetPrecision (void) const {return m_Precision; }
	void SetPrecision (int prec) {m_Precision = prec; }
	long32_t GetLngFrmt (void) const {return m_LngFrmt; }
	long32_t GetLatFrmt (void) const {return m_LatFrmt; }
	long32_t GetXFrmt (void) const {return m_XFrmt; }
	long32_t GetYFrmt (void) const {return m_YFrmt; }
	void SetLngFrmt (long32_t frmt) {m_LngFrmt = frmt;}
	void SetLatFrmt (long32_t frmt) {m_LatFrmt = frmt;}
	void SetXFrmt (long32_t frmt) {m_XFrmt = frmt;}
	void SetYFrmt (long32_t frmt) {m_YFrmt = frmt;}

// Dialog Data
	//{{AFX_DATA(CmgTest)
	enum { IDD = IDD_MGTST_DIALOG };
	CComboBox	m_ZoneCombo;
	CSpinButtonCtrl m_PrecSpin;
	BOOL	m_Bessel;
	CString	m_MgrsString;
	int		m_Precision;
	CString	m_Ellipsoid;		// Description of
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

private:
	struct cs_Mgrs_ *m_Mgrs;
	CString m_ElKeyName;
	double m_ERad;
	double m_ESqr;
	long32_t m_LngFrmt;
	long32_t m_LatFrmt;
	long32_t m_XFrmt;
	long32_t m_YFrmt;
	int m_Zone;
};

/////////////////////////////////////////////////////////////////////////////
// CgdcEdit dialog
class CgdcEdit : public CDialog
{
// Construction
public:
	CgdcEdit(CWnd* pParent = NULL);
	const char* GetGdcName (void);
	void SetInitialCatalog (enum cs_GdcCatalogs initialGdc);
	enum cs_GdcCatalogs GetCurrentCatalog (void);
// Dialog Data
	//{{AFX_DATA(CgdcEdit)
	enum { IDD = IDD_GDC_EDIT };
	CListBox	m_EntryList;
	CComboBox	m_CatalogCombo;
	double	m_Density;
	CString	m_EntryPath;
	long32_t	m_BufferSize;
	CString	m_Fallback;
	//}}AFX_DATA
	bool m_CatChanged;
	ulong32_t m_Options;
	struct csDatumCatalog_ *m_Catalog;
	const struct cs_GdcCatalogTable_ *m_CatalogTable;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CgdcEdit)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CgdcEdit)
	afx_msg void OnGdcAdd();
	afx_msg void OnGdcDelete();
	afx_msg void OnGdcUpdate();
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeGdcCatalogs();
	afx_msg void OnGdcDtbrwse();
	afx_msg void OnGdcFilebrwse();
	afx_msg void OnSelchangeGdcEntrylist();
	afx_msg void OnChangeGdcFallback();
	virtual void OnOK();
	afx_msg void OnGdcBtndown();
	afx_msg void OnGdcBtnfirst();
	afx_msg void OnGdcBtnlast();
	afx_msg void OnGdcBtnup();
	afx_msg void OnDblclkGdcEntrylist();
	afx_msg void OnHelp();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	void UpdateCatalog (void);
	void UpdateEntryList (int entIndex);
	void ShowEntry (int entIndex);
	int CheckFallback (const char *dtKeyName);
};

/////////////////////////////////////////////////////////////////////////////
// CgdeEdit dialog
class CgdeEdit : public CDialog
{
// Construction
public:
	CgdeEdit(CWnd* pParent = NULL);
	void SetBaseDir (const char* baseDir);
	void SetCatalogEntry (const struct csDatumCatalogEntry_ *entPtr,
						  const struct cs_GdcCatalogTable_ *table);
	const TCHAR* GetFileName (void);
	const TCHAR* GetPath (void);
	const char* GetRelPath (void);
	long32_t GetBufferSize (void);
	double GetDensity (void);
	ulong32_t GetOptions (void);
	short GetRelative (void);
// Dialog Data
	//{{AFX_DATA(CgdeEdit)
	enum { IDD = IDD_GDC_ENTRY };
	BOOL	m_Absolute;
	long32_t	m_BufferSize;
	double	m_Density;
	CString	m_FilePath;
	CString	m_FileName;
	//}}AFX_DATA
	ulong32_t m_Options;
	const struct cs_GdcCatalogTable_ *m_CatalogTable;
	char m_BaseDir [MAXPATH];
	char m_RelPath [MAXPATH];
/// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CgdeEdit)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CgdeEdit)
	afx_msg void OnGdeBrwse();
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnHelp();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
/////////////////////////////////////////////////////////////////////////////
// CdlgGeoCentric dialog
class CdlgGeoCentric : public CDialog
{
// Construction
public:
	CdlgGeoCentric (CWnd* pParent = NULL);   // standard constructor
	bool SetEllipsoid (const char *elKeyName);
	bool SetLinearUnit (const char *unKeyName);
	void SetGeographic (const double geographic [3]);
	void SetGeoCentric (const double geoCentric [3]);
	void SetGeographicFormats (const long32_t frmts [3]);
	void SetGeoCentricFormats (const long32_t frmts [3]);
	const TCHAR* GetEllipsoid (void) const {return (LPCTSTR)Ellipsoid; };
	const TCHAR* GetLinearUnit (void) const {return (LPCTSTR)LinearUnit; };
	void GetGeographic (double geographic [3]) const;
	void GetGeoCentric (double geoCentric [3]) const;
	void GetGeographicFormats (long32_t frmts [3]) const;
	void GetGeoCentricFormats (long32_t frmts [3]) const;

// Dialog Data
	//{{AFX_DATA(CdlgGeoCentric)
	enum { IDD = IDD_GEOCENTRIC };
	CString	GeoCtrX;
	CString	GeoCtrY;
	CString	GeoCtrZ;
	CString	Ellipsoid;
	CString	Height;
	CString	Latitude;
	CString	Longitude;
	CString	LinearUnit;
	//}}AFX_DATA
	double EquatorialRadius;
	double EccentricitySquared;
	double UnitScale;
	mutable long32_t LngFrmt, LatFrmt, HgtFrmt;
	mutable long32_t CtrXFrmt, CtrYFrmt, CtrZFrmt;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CdlgGeoCentric)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CdlgGeoCentric)
	afx_msg void OnBtnEllipsoid();
	afx_msg void OnBtnTocentric();
	afx_msg void OnBtnTogeographic();
	afx_msg void OnBtnUnit();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	void EraseResults (void);
	void OnEditFormat (CWnd *wp,const char *Title,CString& value,long32_t *frmt,bool lock);
};
/////////////////////////////////////////////////////////////////////////////
// CdlgAtofFormat dialog
class CdlgAtofFormat : public CDialog
{
// Construction
public:
	CdlgAtofFormat(CWnd* pParent = NULL);   // standard constructor
	void SetInitialValue (long32_t initValue,bool lock = false);
	long32_t GetFormatValue (void) const {return FormatValue; };
// Dialog Data
	//{{AFX_DATA(CdlgAtofFormat)
	enum { IDD = IDD_FTOA_FORMAT };
	CSpinButtonCtrl	PrecisionSpinner;
	BOOL	ThreeDigits;
	int		FormatRadioButton;
	BOOL	UseDirectionCharacter;
	int		DirectionRadioButton;
	BOOL	Use360Mode;
	BOOL	AutomaticPrecision;
	UINT	ManualPrecision;
	BOOL	LeadingZeroDegrees;
	BOOL	LeadingZeroMinSec;
	//}}AFX_DATA
	bool LockDegrees;
	bool LockDirection;
	bool LockType;
	long32_t FormatValue;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CdlgAtofFormat)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CdlgAtofFormat)
	afx_msg void OnFtoaDirchr();
	afx_msg void OnFtoaPrecauto();
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	void ControlActivate (void);
};
/**********************************************************************
**	Re-enable debugging for anything that may follow this header.
**********************************************************************/
#if defined (_DEBUG_WAS_DEFINED)
#	undef _DEBUG_WAS_DEFINED
#	undef NDEBUG
#	define _DEBUG
#endif

//{{AFX_INSERT_LOCATION}}


