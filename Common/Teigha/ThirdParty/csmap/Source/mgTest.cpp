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

#include "cs_map.h"

#if defined (__MFC__)

#include "cs_mfc.h"
#include "cs_hlp.h"

///////////////////////////////////////////////////////////////////////////////
// Basic API access function.
extern "C" void EXP_LVL1 CS_mgTest (char *elKeyName)
{
	TCHAR tcKeyName [cs_KEYNM_DEF];
	CmgTest mgTest;

	CSt_strNCpy (tcKeyName,elKeyName,cs_KEYNM_DEF);
	mgTest.SetEllipsoid (tcKeyName);
	INT_PTR st = mgTest.DoModal ();
	if (st == IDOK)
	{
		CSt_strNCpy (elKeyName,(LPCTSTR)mgTest.GetEllipsoid (),cs_KEYNM_DEF);
	}
	return;	
}
extern "C" void EXP_LVL1 CS_mgTestA (char *elKeyName,int* prec,long32_t* latFrmt,long32_t* lngFrmt)
{
	TCHAR tcKeyName [cs_KEYNM_DEF];
	CmgTest mgTest;

	CSt_strNCpy (tcKeyName,elKeyName,cs_KEYNM_DEF);
	mgTest.SetEllipsoid (tcKeyName);
	mgTest.SetPrecision (*prec);
	mgTest.SetLatFrmt (*latFrmt);
	mgTest.SetLngFrmt (*lngFrmt);
	mgTest.DoModal ();
	CSt_strNCpy (elKeyName,(LPCTSTR)mgTest.GetEllipsoid (),cs_KEYNM_DEF);
	*prec = mgTest.GetPrecision ();
	*latFrmt = mgTest.GetLatFrmt ();
	*lngFrmt = mgTest.GetLngFrmt ();
	return;
}
/////////////////////////////////////////////////////////////////////////////
// CmgTest dialog
CmgTest::CmgTest(CWnd* pParent) : CDialog(CmgTest::IDD, pParent)
{
	//{{AFX_DATA_INIT(CmgTest)
	m_Bessel = FALSE;
	m_MgrsString = _T("");
	m_Precision = 5;
	m_Ellipsoid = _T("");
	m_Latitude = 0.0;
	m_Longitude = 0.0;
	m_Easting = 0.0;
	m_Northing = 0.0;
	//}}AFX_DATA_INIT

	m_ElKeyName = _T("");
	m_Zone = 0;
	m_ERad = 6378137.0;
	m_ESqr = 0.00669437999013;
	m_LngFrmt = cs_ATOF_LNGDFLT;
	m_LatFrmt = cs_ATOF_LATDFLT;
	m_XFrmt   = cs_ATOF_XXXDFLT;
	m_YFrmt   = cs_ATOF_YYYDFLT;
}
void CmgTest::SetEllipsoid (const TCHAR* elKeyName)
{
	m_ElKeyName = elKeyName;
}
const TCHAR* CmgTest::GetEllipsoid ()
{
	return (LPCTSTR)m_ElKeyName;
}
void CmgTest::SetMgrsString (const TCHAR* mgrsString)
{
	m_MgrsString = mgrsString;
}
const TCHAR* CmgTest::GetMgrsString ()
{
	return (LPCTSTR)m_MgrsString;
}
BOOL CmgTest::OnInitDialog ()
{
	BOOL rtnValue;
	struct cs_Eldef_ *elDef;
	TCHAR errmsg [256];

	rtnValue = CDialog::OnInitDialog ();

	// Set the ellipsoid name.
	if (!m_ElKeyName.IsEmpty ())
	{
		elDef = CSt_eldef ((LPCTSTR)m_ElKeyName);
		if (elDef == NULL)
		{
			m_ElKeyName = _T("WGS84");
			m_ERad = 6378137.0;
			m_ESqr = 0.00669437999013;
			m_Ellipsoid = _T("World Geodetic System of 1984");
		}
		else
		{
			m_ERad = elDef->e_rad;
			m_ESqr = elDef->ecent * elDef->ecent;
			CSt_cstringAssign (m_Ellipsoid,elDef->name);
		}
	}
	else
	{
		m_ElKeyName = _T("WGS84");
		m_ERad = 6378137.0;
		m_ESqr = 0.00669437999013;
		m_Ellipsoid = _T("World Geodetic System of 1984");
	}

	m_Mgrs = CSnewMgrs (m_ERad,m_ESqr,m_Bessel ? 1 : 0);
	if (m_Mgrs == NULL)
	{
		CSt_errmsg (errmsg,tchrCount (errmsg));
		AfxMessageBox (errmsg);
	}
	// Set up the zone combo box.  We use indicies of 0 through 122.
	m_ZoneCombo.ResetContent ();
	for (int idx = 0;idx <= 122;idx += 1)
	{
		TCHAR bufr [64];

		if (idx == 0) m_ZoneCombo.AddString (_T("Error"));
		else if (idx <= 60)
		{
			_stprintf (bufr,_T("%dN"),idx);
			m_ZoneCombo.AddString (bufr);
		}
		else if (idx <= 120)
		{
			_stprintf (bufr,_T("%dS"),(idx - 60));
			m_ZoneCombo.AddString (bufr);
		}
		else if (idx == 121) m_ZoneCombo.AddString (_T("North Polar"));
		else if (idx == 122) m_ZoneCombo.AddString (_T("South Polar"));
	}

	// Set up the spinner for precision.
	CWnd* wPtr = GetDlgItem (IDC_MGTST_PREC);
	if (wPtr != 0) m_PrecSpin.SetBuddy (wPtr);
	m_PrecSpin.SetBase (10);
	m_PrecSpin.SetRange (0,5);
	m_PrecSpin.SetPos (m_Precision);

	// Disable the Help button if help isn't available.
	CWnd* wp = GetDlgItem (ID_MGTST_HELP);
	if (wp != NULL) wp->EnableWindow (CS_isHlpAvailable ());

	// Update the display
	UpdateData (FALSE);
	
	// If the MGRS String is not empty, call the MGRS Calculate function.
	if (!m_MgrsString.IsEmpty ())
	{
		OnMgtstMgrsbtn();
	}
	return rtnValue;
}
void CmgTest::DoDataExchange(CDataExchange* pDX)
{
	int curSel;

	CDialog::DoDataExchange(pDX);

	//{{AFX_DATA_MAP(CmgTest)
	DDX_Control(pDX, IDC_MGTST_ZONECOMBO, m_ZoneCombo);
	DDX_Control(pDX, IDC_MGTST_PRECSPIN, m_PrecSpin);
	DDX_Check (pDX, IDC_MGTST_BESSEL, m_Bessel);
	DDX_Text (pDX,IDC_MGTST_MGRS,m_MgrsString);
	DDX_Text (pDX,IDC_MGTST_PREC,m_Precision);
	DDV_MinMaxInt (pDX, m_Precision,0,5);
	DDX_Text (pDX,IDC_MGTST_ELLIPSOID, m_Ellipsoid);
	//}}AFX_DATA_MAP

	if (pDX->m_bSaveAndValidate)
	{
		curSel = m_ZoneCombo.GetCurSel ();
		if      (curSel ==   0) m_Zone = 0;
		else if (curSel <=  60) m_Zone = curSel;
		else if (curSel <= 120) m_Zone = 60 - curSel;
		else if (curSel == 121) m_Zone = 61;
		else if (curSel == 122) m_Zone = -61;
		else                    m_Zone = 0;
	}
	else
	{
		if      (m_Zone ==   0) curSel = 0;
		else if (m_Zone == -61) curSel = 122;
		else if (m_Zone ==  61) curSel = 121;
		else if (m_Zone <    0) curSel = 60 - m_Zone;
		else if (m_Zone >    0) curSel = m_Zone;
		m_ZoneCombo.SetCurSel (curSel);
	}

	DDX_Coord (pDX,IDC_MGTST_LAT,m_Latitude,&m_LatFrmt);
	DDX_Coord (pDX,IDC_MGTST_LONG,m_Longitude,&m_LngFrmt);

	long32_t xxx, yyy;
	xxx = m_XFrmt;	yyy = m_YFrmt;
	DDX_Coord (pDX,IDC_MGTST_UTMX, m_Easting,&xxx);
	DDX_Coord (pDX,IDC_MGTST_UTMY,m_Northing,&yyy);
}

BEGIN_MESSAGE_MAP(CmgTest, CDialog)
	//{{AFX_MSG_MAP(CmgTest)
	ON_BN_CLICKED(ID_MGTST_HELP, OnMgtstHelp)
	ON_BN_CLICKED(IDC_MGTST_LATLNGBTN, OnMgtstLatlngbtn)
	ON_BN_CLICKED(IDC_MGTST_BESSEL, OnMgtstBessel)
	ON_BN_CLICKED(IDC_MGTST_MGRSBTN, OnMgtstMgrsbtn)
	ON_BN_CLICKED(IDC_MGTST_UTMBTN, OnMgtstUtmbtn)
	ON_BN_CLICKED(IDC_MGTST_ELSELECT, OnMgtstElselect)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CmgTest message handlers
void CmgTest::OnMgtstHelp() 
{
	CSwinhlp (m_hWnd,csHLPID_MGRS_TEST);
}
void CmgTest::OnMgtstLatlngbtn() 
{
	int st;
	char ccBufr [256];
	TCHAR tcBufr [256];
	TCHAR errmsg [256];
	double latLng [2];
	double utmUps [2];

	if (!UpdateData (TRUE)) return;
	if (m_Mgrs == NULL)
	{
		AfxMessageBox (_T("MGRS system initialization failed."));
	}

	m_MgrsString.Empty ();
	m_Easting = m_Northing = 0.0;

	latLng [0] = m_Longitude;
	latLng [1] = m_Latitude;
	m_Zone = CScalcUtmUps (m_Mgrs,utmUps,latLng);
	if (m_Zone == 0)
	{
		CSt_errmsg (errmsg,tchrCount (errmsg));
		AfxMessageBox (errmsg);
	}
	else
	{
		m_Easting = utmUps [0];
		m_Northing = utmUps [1];
		st = CScalcMgrsFromLlUtm (m_Mgrs,ccBufr,sizeof (ccBufr),latLng,utmUps,m_Precision);
		if (st != 0)
		{
			CSt_errmsg (errmsg,tchrCount (errmsg));
			AfxMessageBox (errmsg);
		}
		else
		{
			CSt_strNCpy (tcBufr,ccBufr,tchrCount (tcBufr));
			m_MgrsString = tcBufr;
		}
	}
	UpdateData (FALSE);
}
void CmgTest::OnMgtstBessel() 
{
	if (!UpdateData (TRUE)) return;

	CSdeleteMgrs (m_Mgrs);
	m_Mgrs = NULL;	
	m_Mgrs = CSnewMgrs (m_ERad,m_ESqr,m_Bessel ? 1 : 0);
	return;
}
void CmgTest::OnMgtstMgrsbtn() 
{
	int st;
	char ccBufr [128];
	TCHAR errmsg [256];
	double latLng [2];
	double utmUps [2];

	if (!UpdateData (TRUE)) return;
	if (m_Mgrs == NULL)
	{
		AfxMessageBox (_T("MGRS system initialization failed."));
	}

	m_Latitude = m_Longitude = m_Easting = m_Northing = 0.0;

	CSt_strNCpy (ccBufr,(LPCTSTR)m_MgrsString,sizeof (ccBufr));
	st = CScalcLlFromMgrs (m_Mgrs,latLng,ccBufr);
	if (st != 0)
	{
		CSt_errmsg (errmsg,tchrCount (errmsg));
		AfxMessageBox (errmsg);
	}
	if (st >= 0)
	{
		m_Latitude = latLng [1];
		m_Longitude = latLng [0];
		m_Zone = CScalcUtmUps (m_Mgrs,utmUps,latLng);
		if (m_Zone == 0)
		{
			CSt_errmsg (errmsg,tchrCount (errmsg));
			AfxMessageBox (errmsg);
		}
		else
		{
			m_Easting = utmUps [0];
			m_Northing = utmUps [1];
		}
	}
	UpdateData (FALSE);
}
void CmgTest::OnCancel() 
{
	CDialog::OnCancel();
}
void CmgTest::OnMgtstUtmbtn() 
{
	int st;
	char ccBufr [128];
	TCHAR tcBufr [128];
	TCHAR errmsg [256];
	double latLng [2];
	double utmUps [2];

	if (!UpdateData (TRUE)) return;
	if (m_Mgrs == NULL)
	{
		AfxMessageBox (_T("MGRS system initialization failed."));
		return;
	}

	m_MgrsString.Empty ();
	m_Latitude = m_Longitude = 0.0;

	utmUps [0] = m_Easting;
	utmUps [1] = m_Northing;
	st = CScalcLatLng (m_Mgrs,latLng,utmUps,m_Zone);
	if (st != 0)
	{
		CSt_errmsg (errmsg,tchrCount (errmsg));
		AfxMessageBox (errmsg);
	}
	else
	{
		m_Longitude = latLng [0];
		m_Latitude = latLng [1];
		st = CScalcMgrsFromLlUtm (m_Mgrs,ccBufr,sizeof (ccBufr),latLng,utmUps,m_Precision);
		if (st != 0)
		{
			CSt_errmsg (errmsg,tchrCount (errmsg));
			AfxMessageBox (errmsg);
		}
		else
		{
			CSt_strNCpy (tcBufr,ccBufr,tchrCount (tcBufr));
			m_MgrsString = tcBufr;
		}
	}
	UpdateData (FALSE);
}
void CmgTest::OnMgtstElselect() 
{
	INT_PTR st;
	struct cs_Eldef_ *elDef;
	TCHAR errmsg [256];
	CelSelect elSelect;

	elSelect.SetInitialEllipsoid (m_ElKeyName);
	st = elSelect.DoModal ();
	if (st == IDOK)
	{
		m_ElKeyName = elSelect.GetSelectedEllipsoid ();

		CSdeleteMgrs (m_Mgrs);
		m_Mgrs = NULL;	
		m_Longitude = m_Latitude = m_Easting = m_Northing = 0.0;
		elDef = CSt_eldef ((LPCTSTR)m_ElKeyName);
		if (elDef == NULL)
		{
			CSt_errmsg (errmsg,tchrCount (errmsg));
			AfxMessageBox (errmsg);
		}
		else
		{
			m_ERad = elDef->e_rad;
			m_ESqr = elDef->ecent * elDef->ecent;
			CSt_cstringAssign (m_Ellipsoid,elDef->name);
			m_Mgrs = CSnewMgrs (m_ERad,m_ESqr,static_cast<short>(m_Bessel));
			if (m_Mgrs == NULL)
			{
				CSt_errmsg (errmsg,tchrCount (errmsg));
				AfxMessageBox (errmsg);
			}
		}
		UpdateData (FALSE);
	}
}
long32_t AFXAPI DDX_Coord (CDataExchange* pDX,int nIDC,double& value,long32_t* frmt)
{
	int err_flg;

	long32_t st;

	double dtmp;

	TCHAR buffer [64];

	err_flg = FALSE;
	HWND hWndCtrl = pDX->PrepareEditCtrl(nIDC);

	if (pDX->m_bSaveAndValidate)
	{
		::GetWindowText(hWndCtrl,buffer,sizeof (buffer));
		st = CSt_atof (&dtmp,buffer);
		value = dtmp;
		if (st < 0L)
		{
			err_flg = TRUE;

			// A problem was detected in the conversion to
			// double.  Tell the user what the problem is
			// and throw an exception.
			if ((st & cs_ATOF_SECS60) != 0)
			{
				AfxMessageBox (_T("Value expected to be seconds was 60 or larger."));
			}
			else if ((st & cs_ATOF_MINS60) != 0)
			{
				AfxMessageBox (_T("Value expected to be minutes was 60 or larger."));
			}
			else if ((st & cs_ATOF_MLTPNT) != 0)
			{
				AfxMessageBox (_T("Multiple decimal points encountered, value intended is ambiguous."));
			}
			else if ((st & cs_ATOF_MLTSGN) != 0)
			{
				AfxMessageBox (_T("Multiple sign indications were encountered, value intended is ambiguous."));
			}
			else if ((st & cs_ATOF_ERRCMA) != 0)
			{
				AfxMessageBox (_T("Thousands separator was used inconsistently, value intended is ambiguous."));
			}
			else
			{
				AfxMessageBox (_T("Numeric input formatting error."));
			}
		}
		if ((*frmt & cs_ATOF_VALLNG) != 0 && fabs (dtmp) > 200.0)
		{
			err_flg = TRUE;
			AfxMessageBox (_T("Longitude value provided is suspiciously large."));
		}
		else if ((*frmt & cs_ATOF_VALLAT) != 0 && fabs (dtmp) > 90.0)
		{
			err_flg = TRUE;
			AfxMessageBox (_T("Latitude value provided is suspiciously large."));
		}
		if (err_flg)
		{
			pDX->Fail ();
		}

		// Return the format to the calling function.
		*frmt = st;
	}
	else
	{
		dtmp = value;
		st = CSt_ftoa (buffer,tchrCount (buffer),dtmp,*frmt);
		::SetWindowText (hWndCtrl,buffer);
	}
	return (st);
}
#endif
