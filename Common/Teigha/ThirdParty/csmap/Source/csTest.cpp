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

/*lint -e766 */			/* bogus lint message, says cs_map.h is not referenced */

#include "cs_map.h"

#if defined (__MFC__)

#include "cs_mfc.h"
#include "cs_hlp.h"

/**********************************************************************
**	CcsTest csTest;
**	Lots and lots opf member functions, see the header file.
**********************************************************************/

///////////////////////////////////////////////////////////////////////////////
// C callable function.  Note, name is not mangled for easy access from Visual
//	Basic, et al.
extern "C" void EXP_LVL1 CS_csTest (char *srcSystem,char *trgSystem,double srcXYZ [3])
{
	TCHAR srcKeyName [cs_KEYNM_DEF];
	TCHAR trgKeyName [cs_KEYNM_DEF];
	
	CSt_strNCpy (srcKeyName,srcSystem,tchrCount (srcKeyName));
	CSt_strNCpy (trgKeyName,trgSystem,tchrCount (trgKeyName));
	
	CcsTest csTest;
	CS_recvr ();

	csTest.SetSourceSystem (srcKeyName);
	csTest.SetTargetSystem (trgKeyName);
	csTest.SetSourceCoordinate (srcXYZ);
	csTest.DoModal ();
	// Must capture the final settings, for persistence, before the
	// class goes out of scope.
	CSt_strNCpy (srcSystem,csTest.GetSourceSystem (),cs_KEYNM_DEF);
	CSt_strNCpy (trgSystem,csTest.GetTargetSystem (),cs_KEYNM_DEF);
	csTest.GetSourceCoordinate (srcXYZ);
	
	// The Test object uses the high level caches.  The following
	// releases the memory consumed by such, and eliminates what
	// appears to be a memory leak.
	CS_recvr ();
	return;
}
/////////////////////////////////////////////////////////////////////////////
// CcsTest dialog
CcsTest::CcsTest(CWnd* pParent /*=NULL*/)
	: CDialog(CcsTest::IDD, pParent)
{
	//{{AFX_DATA_INIT(CcsTest)
	m_SrcConvergence = _T("");
	m_SrcDatumKeyName = _T("");
	m_SrcDatumLabel = _T("");
	m_SrcDescription = _T("");
	m_SrcKeyName = _T("");
	m_SrcSource = _T("");
	m_SrcStatus = _T("");
	m_SrcUnit = _T("");
	m_SrcXLatitude = _T("");
	m_SrcXLatLabel = _T("");
	m_SrcYLongitude = _T("");
	m_SrcZHeight = _T("");
	m_TrgConvergence = _T("");
	m_TrgDatLabel = _T("");
	m_TrgDescription = _T("");
	m_TrgDatumKeyName = _T("");
	m_TrgGridScale = _T("");
	m_TrgKeyName = _T("");
	m_TrgSource = _T("");
	m_TrgStatus = _T("");
	m_TrgUnit = _T("");
	m_TrgXLatLabel = _T("");
	m_SrcGridScale = _T("");
	m_TrgXLatitude = _T("");
	m_TrgYLongitude = _T("");
	m_TrgZHeight = _T("");
	m_DecimalDegrees = FALSE;
	m_HighPrecision = TRUE;
	m_SrcYLngLabel = _T("");
	m_TrgYLngLabel = _T("");
	m_SrcZHgtLabel = _T("");
	m_SrcZHgtLabel = _T("");
	m_SrcSPZone83 = FALSE;
	m_TrgSPZone83 = FALSE;
	m_ThreeDCalc = FALSE;
	//}}AFX_DATA_INIT
	m_SourceXY [0] = 0.0;
	m_SourceXY [1] = 0.0;
	m_SourceXY [3] = 0.0;
	m_SourceScale = 1.0;
	m_SourceConvergence = 0.0;
	m_TargetXY [0] = 0.0;
	m_TargetXY [1] = 0.0;
	m_TargetXY [2] = 0.0;
	m_TargetScale = 1.0;
	m_TargetConvergence = 0.0;
}

BOOL CcsTest::OnInitDialog ()
{
	CWnd *wp;

	CDialog::OnInitDialog ();

	UpdateSource ();
	if (m_SrcKeyName.IsEmpty ()) m_SrcKeyName = _T("US48");
	if (m_TrgKeyName.IsEmpty ()) m_TrgKeyName = _T("LL27");
	SetLabels (true);
	SetLabels (false);
	FetchSource ();
	Calculate ();
	UpdateSource ();
	UpdateTarget ();
	UpdateData (FALSE);

	// Disable the Help button if help is not available.
	wp = GetDlgItem (ID_CSTST_HELP);
	if (wp != NULL) wp->EnableWindow (CS_isHlpAvailable ());

	return (TRUE);
};

void CcsTest::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	//{{AFX_DATA_MAP(CcsTest)
	DDX_Text(pDX, IDC_CSTST_SRCCNVRG, m_SrcConvergence);
	DDX_Text(pDX, IDC_CSTST_SRCDATKYNM, m_SrcDatumKeyName);
	DDX_Text(pDX, IDC_CSTST_SRCDATLBL, m_SrcDatumLabel);
	DDX_Text(pDX, IDC_CSTST_SRCDESCR, m_SrcDescription);
	DDV_MaxChars(pDX, m_SrcDescription, 64);
	DDX_Text(pDX, IDC_CSTST_SRCKEYNM, m_SrcKeyName);
	DDX_Text(pDX, IDC_CSTST_SRCSRC, m_SrcSource);
	DDX_Text(pDX, IDC_CSTST_SRCSTAT, m_SrcStatus);
	DDX_Text(pDX, IDC_CSTST_SRCUNIT, m_SrcUnit);
	DDX_Text(pDX, IDC_CSTST_SRCXLAT, m_SrcXLatitude);
	DDX_Text(pDX, IDC_CSTST_SRCXLATLBL, m_SrcXLatLabel);
	DDX_Text(pDX, IDC_CSTST_SRCYLNG, m_SrcYLongitude);
	DDX_Text(pDX, IDC_CSTST_SRCZHGT, m_SrcZHeight);
	DDX_Text(pDX, IDC_CSTST_TRGCNVRG, m_TrgConvergence);
	DDX_Text(pDX, IDC_CSTST_TRGDATLBL, m_TrgDatLabel);
	DDX_Text(pDX, IDC_CSTST_TRGDESCR, m_TrgDescription);
	DDX_Text(pDX, IDC_CSTST_TRGDTKY, m_TrgDatumKeyName);
	DDX_Text(pDX, IDC_CSTST_TRGGRIDSCL, m_TrgGridScale);
	DDX_Text(pDX, IDC_CSTST_TRGKEYNM, m_TrgKeyName);
	DDX_Text(pDX, IDC_CSTST_TRGSRC, m_TrgSource);
	DDX_Text(pDX, IDC_CSTST_TRGSTAT, m_TrgStatus);
	DDX_Text(pDX, IDC_CSTST_TRGUNIT, m_TrgUnit);
	DDX_Text(pDX, IDC_CSTST_TRGXLATLBL, m_TrgXLatLabel);
	DDX_Text(pDX, IDC_CSTST_SRCGRIDSCL, m_SrcGridScale);
	DDX_Text(pDX, IDC_CSTST_TRGXLAT, m_TrgXLatitude);
	DDX_Text(pDX, IDC_CSTST_TRGYLNG, m_TrgYLongitude);
	DDX_Text(pDX, IDC_CSTST_TRGZHGT, m_TrgZHeight);
	DDX_Check(pDX, IDC_CSTST_DECIMAL, m_DecimalDegrees);
	DDX_Check(pDX, IDC_CSTST_HIGHPREC, m_HighPrecision);
	DDX_Check(pDX, IDC_CSTST_3DCHECK, m_ThreeDCalc);
	DDX_Text(pDX, IDC_CSTST_SRCYLNGLBL, m_SrcYLngLabel);
	DDX_Text(pDX, IDC_CSTST_TRGYLNGLBL, m_TrgYLngLabel);
	DDX_Check(pDX, IDC_CSTST_SRCSP83, m_SrcSPZone83);
	DDX_Check(pDX, IDC_CSTST_TRGSP83, m_TrgSPZone83);
	//}}AFX_DATA_MAP

	if (pDX->m_bSaveAndValidate)
	{
		char csKeyName [cs_KEYNM_DEF];
		CSt_strNCpy (csKeyName,m_SrcKeyName,sizeof (csKeyName));
		CS_spZoneNbrMap (csKeyName,m_SrcSPZone83);
		CSt_cstringAssign (m_SrcKeyName,csKeyName);
		CSt_strNCpy (csKeyName,m_TrgKeyName,sizeof (csKeyName));
		CS_spZoneNbrMap (csKeyName,m_TrgSPZone83);
		CSt_cstringAssign (m_TrgKeyName,csKeyName);
	}
}

BEGIN_MESSAGE_MAP(CcsTest, CDialog)
	//{{AFX_MSG_MAP(CcsTest)
	ON_BN_CLICKED(ID_CSTST_CALCULATE, OnCststCalculate)
	ON_BN_CLICKED(ID_CSTST_REVERSE, OnCststReverse)
	ON_BN_CLICKED(ID_HELP, OnHelp)
	ON_EN_KILLFOCUS(IDC_CSTST_SRCKEYNM, OnKillfocusCststSrckeynm)
	ON_EN_KILLFOCUS(IDC_CSTST_TRGKEYNM, OnKillfocusCststTrgkeynm)
	ON_EN_CHANGE(IDC_CSTST_SRCKEYNM, OnChange)
	ON_BN_CLICKED(IDC_CSTST_SRCCSBTN, OnCststSrccsbtn)
	ON_BN_CLICKED(IDC_CSTST_TRGCSBTN, OnCststTrgcsbtn)
	ON_BN_CLICKED(IDC_CSTST_3DCHECK, OnCstst3DCheck)
	ON_EN_CHANGE(IDC_CSTST_SRCXLAT, OnChange)
	ON_EN_CHANGE(IDC_CSTST_SRCYLNG, OnChange)
	ON_EN_CHANGE(IDC_CSTST_TRGKEYNM, OnChange)
	ON_BN_CLICKED(ID_CSTST_HELP, OnCststHelp)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
//	Support Functions

void CcsTest::SetSourceSystem (const TCHAR* srcSystem)
{
	m_SrcKeyName = srcSystem;
}
void CcsTest::SetTargetSystem (const TCHAR* trgSystem)
{
	m_TrgKeyName = trgSystem;
}
void CcsTest::SetSourceCoordinate (const double srcXY [3])
{
	m_SourceXY [0] = srcXY [0];
	m_SourceXY [1] = srcXY [1];
	m_SourceXY [2] = srcXY [2];
}
const TCHAR* CcsTest::GetSourceSystem ()
{
	return m_SrcKeyName;
}
const TCHAR* CcsTest::GetTargetSystem ()
{
	return m_TrgKeyName;
}
void CcsTest::GetSourceCoordinate (double* srcXY)
{
	srcXY [0] = m_SourceXY [0];
	srcXY [1] = m_SourceXY [1];
	srcXY [2] = m_SourceXY [2];
}
void CcsTest::UpdateSource ()
{
	int prec;
	long32_t frmtXLat, frmtYLng, frmtZHgt;
	TCHAR tcTemp [24];

	// Is this a geographic coordinate???
	if (CSt_isgeo (m_SrcKeyName))
	{
		if (m_DecimalDegrees)
		{
			prec = m_HighPrecision ? 13 : 9;
			frmtXLat = prec;
			frmtYLng = prec;
		}
		else
		{
			prec = m_HighPrecision ? 6 : 2;
			frmtXLat = cs_ATOF_MINSEC | cs_ATOF_DIRCHR | (prec + 1);
			frmtYLng = cs_ATOF_MINSEC | cs_ATOF_DIRCHR | cs_ATOF_XEAST | (prec + 1);
		}
		prec = m_HighPrecision ? 4 : 1;
		frmtZHgt = cs_ATOF_COMMA | (prec + 1);

		CSt_ftoa (tcTemp,tchrCount (tcTemp),m_SourceXY [1],frmtXLat);
		m_SrcXLatitude = tcTemp;
		CSt_ftoa (tcTemp,tchrCount (tcTemp),m_SourceXY [0],frmtYLng);
		m_SrcYLongitude = tcTemp;
		CSt_ftoa (tcTemp,tchrCount (tcTemp),m_SourceXY [2],frmtZHgt);
		m_SrcZHeight = tcTemp;

		m_SrcGridScale = _T("Not Applicable");
		m_SrcConvergence = _T("Not Applicable");
	}
	else
	{
		prec = m_HighPrecision ? 5 : 3;
		frmtXLat = cs_ATOF_COMMA | (prec + 1);
		frmtYLng = cs_ATOF_COMMA | (prec + 1);
		frmtZHgt = cs_ATOF_COMMA | (prec + 1);
		CSt_ftoa (tcTemp,tchrCount (tcTemp),m_SourceXY [0],frmtXLat);
		m_SrcXLatitude = tcTemp;
		CSt_ftoa (tcTemp,tchrCount (tcTemp),m_SourceXY [1],frmtYLng);
		m_SrcYLongitude = tcTemp;
		CSt_ftoa (tcTemp,tchrCount (tcTemp),m_SourceXY [2],frmtZHgt);
		m_SrcZHeight = tcTemp;

		// Grid Scale for the source system.
		if (m_SourceScale >= 9999.00)
		{
			m_SrcGridScale = _T("Infinite");
		}
		else if (m_SourceScale > 0.0)
		{
			prec = m_HighPrecision ? 9 : 5;
			CSt_ftoa (tcTemp,tchrCount (tcTemp),m_SourceScale,(prec + 1));
			m_SrcGridScale = tcTemp;
		}
		else if (m_SourceScale < 0.0)
		{
			m_SrcGridScale = _T("Domain Err");
		}
		else
		{
			m_SrcGridScale = _T("");
		}

		// Convergence for the source system.
		if (m_SourceConvergence > -360.0)
		{
			if (m_DecimalDegrees)
			{
				prec = m_HighPrecision ? 9 : 5;
				CSt_ftoa (tcTemp,tchrCount (tcTemp),m_SourceConvergence,(prec + 1));
				m_SrcConvergence = tcTemp;
			}
			else
			{
				prec = m_HighPrecision ? 4 : 0;
				frmtYLng = cs_ATOF_MINSEC | cs_ATOF_DIRCHR | cs_ATOF_XEAST | (prec + 1);
				CSt_ftoa (tcTemp,tchrCount (tcTemp),m_SourceConvergence,frmtYLng);
				m_SrcConvergence = tcTemp;
			}
		}
		else if (m_SourceConvergence == -360.0)
		{
			m_SrcConvergence = _T("Domain Err");
		}
		else
		{
			m_SrcConvergence = _T("");
		}
	}
	return;
}
void CcsTest::UpdateTarget ()
{
	int prec;
	long32_t frmtXLat, frmtYLng, frmtZHgt;
	TCHAR tcTemp [24];

	// Is this a geographic coordinate???
	if (CSt_isgeo (m_TrgKeyName))
	{
		if (m_DecimalDegrees)
		{
			prec = m_HighPrecision ? 13 : 9;
			frmtXLat = prec;
			frmtYLng = prec;
		}
		else
		{
			prec = m_HighPrecision ? 6 : 2;
			frmtXLat = cs_ATOF_MINSEC | cs_ATOF_DIRCHR | (prec + 1);
			frmtYLng = cs_ATOF_MINSEC | cs_ATOF_DIRCHR | cs_ATOF_XEAST | (prec + 1);
		}
		prec = m_HighPrecision ? 4 : 2;
		frmtZHgt = cs_ATOF_COMMA | (prec + 1);

		CSt_ftoa (tcTemp,tchrCount (tcTemp),m_TargetXY [1],frmtXLat);
		m_TrgXLatitude = tcTemp;
		CSt_ftoa (tcTemp,tchrCount (tcTemp),m_TargetXY [0],frmtYLng);
		m_TrgYLongitude = tcTemp;
		CSt_ftoa (tcTemp,tchrCount (tcTemp),m_TargetXY [2],frmtZHgt);
		m_TrgZHeight = tcTemp;
		m_TrgGridScale = _T("Not Applicable");
		m_TrgConvergence = _T("Not Applicable");
	}
	else
	{
		prec = m_HighPrecision ? 5 : 3;
		frmtXLat = cs_ATOF_COMMA | (prec + 1);
		frmtYLng = cs_ATOF_COMMA | (prec + 1);
		frmtZHgt = cs_ATOF_COMMA | (prec + 1);
		CSt_ftoa (tcTemp,tchrCount (tcTemp),m_TargetXY [0],frmtXLat);
		m_TrgXLatitude = tcTemp;
		CSt_ftoa (tcTemp,tchrCount (tcTemp),m_TargetXY [1],frmtYLng);
		m_TrgYLongitude = tcTemp;
		CSt_ftoa (tcTemp,tchrCount (tcTemp),m_TargetXY [2],frmtZHgt);
		m_TrgZHeight = tcTemp;
		// Grid Scale for the target system.
		if (m_TargetScale >= 9999.00)
		{
			m_TrgGridScale = _T("Infinite");
		}
		else if (m_TargetScale > 0.0)
		{
			prec = m_HighPrecision ? 9 : 5;
			CSt_ftoa (tcTemp,tchrCount (tcTemp),m_TargetScale,(prec + 1));
			m_TrgGridScale = tcTemp;
		}
		else if (m_TargetScale < 0.0)
		{
			m_TrgGridScale = _T("Domain Err");
		}
		else
		{
			m_TrgGridScale = _T("");
		}
		// Convergence for the source system.
		if (m_TargetConvergence > -360.0)
		{
			if (m_DecimalDegrees)
			{
				prec = m_HighPrecision ? 9 : 5;
				CSt_ftoa (tcTemp,tchrCount (tcTemp),m_TargetConvergence,(prec + 1));
				m_TrgConvergence = tcTemp;
			}
			else
			{
				prec = m_HighPrecision ? 4 : 0;
				frmtYLng = cs_ATOF_MINSEC | cs_ATOF_DIRCHR | cs_ATOF_XEAST | (prec + 1);
				CSt_ftoa (tcTemp,tchrCount (tcTemp),m_TargetConvergence,frmtYLng);
				m_TrgConvergence = tcTemp;
			}
		}
		else if (m_TargetConvergence == -360.0)
		{
			m_TrgConvergence = _T("Domain Err");
		}
		else
		{
			m_TrgConvergence = _T("");
		}
	}
	return;
}
CWnd* CcsTest::FetchSource ()
{
	CWnd* errorPtr;
	long32_t format;

	errorPtr = NULL;
	if (CSt_isgeo (m_SrcKeyName))
	{
		format = CSt_atof (&m_SourceXY [1],(LPCTSTR)m_SrcXLatitude);
		if (format < 0L) errorPtr = GetDlgItem (IDC_CSTST_SRCXLAT);
		else
		{
			format = CSt_atof (&m_SourceXY [0],(LPCTSTR)m_SrcYLongitude);
			if (format < 0L) errorPtr = GetDlgItem (IDC_CSTST_SRCYLNG);
			else
			{
				format = CSt_atof (&m_SourceXY [2],(LPCTSTR)m_SrcZHeight);
				if (format < 0L) errorPtr = GetDlgItem (IDC_CSTST_SRCZHGT);
			}
		}
	}
	else
	{
		format = CSt_atof (&m_SourceXY [0],(LPCTSTR)m_SrcXLatitude);
		if (format < 0L) errorPtr = GetDlgItem (IDC_CSTST_SRCXLAT);
		else
		{
			format = CSt_atof (&m_SourceXY [1],(LPCTSTR)m_SrcYLongitude);
			if (format < 0L) errorPtr = GetDlgItem (IDC_CSTST_SRCYLNG);
			else
			{
				format = CSt_atof (&m_SourceXY [2],(LPCTSTR)m_SrcZHeight);
				if (format < 0L) errorPtr = GetDlgItem (IDC_CSTST_SRCZHGT);
			}
		}
	}
	return errorPtr;
}
bool CcsTest::SetLabels (bool target)
{
	struct cs_Csprm_ *csPtr;

	if (!target)
	{
		csPtr = CSbcclu_T (m_SrcKeyName);
		if (csPtr == NULL) return false;
		CSt_cstringAssign (m_SrcDescription,csPtr->csdef.desc_nm);
		CSt_cstringAssign (m_SrcSource,csPtr->csdef.source);
		if (csPtr->csdef.dat_knm [0] != _T('\0'))
		{
			m_SrcDatumLabel = _T("Datum:");
			CSt_cstringAssign (m_SrcDatumKeyName,csPtr->csdef.dat_knm);
		}
		else
		{
			m_SrcDatumLabel = _T("Ellipsoid:");
			CSt_cstringAssign (m_SrcDatumKeyName,csPtr->csdef.elp_knm);
		}
		CSt_cstringAssign (m_SrcUnit,csPtr->csdef.unit);
		if ((csPtr->prj_flags & cs_PRJFLG_GEOGR) == 0)
		{
			m_SrcXLatLabel = _T("X/Easting");
			m_SrcYLngLabel = _T("Y/Northing");
			if (m_ThreeDCalc) m_SrcZHgtLabel = _T("Z/Elevation");
			else              m_SrcZHgtLabel = _T("");
		}
		else
		{
			m_SrcXLatLabel = _T("Latitude");
			m_SrcYLngLabel = _T("Longitude");
			if (m_ThreeDCalc) m_SrcZHgtLabel = _T("Height");
			else              m_SrcZHgtLabel = _T("");
		}
	}
	else
	{
		csPtr = CSbcclu_T (m_TrgKeyName);
		if (csPtr == NULL) return false;
		CSt_cstringAssign (m_TrgDescription,csPtr->csdef.desc_nm);
		CSt_cstringAssign (m_TrgSource,csPtr->csdef.source);
		if (csPtr->csdef.dat_knm [0] != _T('\0'))
		{
			m_TrgDatLabel = _T("Datum:");
			CSt_cstringAssign (m_TrgDatumKeyName,csPtr->csdef.dat_knm);
		}
		else
		{
			m_TrgDatLabel = _T("Ellipsoid:");
			CSt_cstringAssign (m_TrgDatumKeyName,csPtr->csdef.elp_knm);
		}
		CSt_cstringAssign (m_TrgUnit,csPtr->csdef.unit);
		if ((csPtr->prj_flags & cs_PRJFLG_GEOGR) == 0)
		{
			m_TrgXLatLabel = _T("X/Easting");
			m_TrgYLngLabel = _T("Y/Northing");
			if (m_ThreeDCalc) m_TrgZHgtLabel = _T("Z/Elevation");
			else              m_TrgZHgtLabel = _T("");
		}
		else
		{
			m_TrgXLatLabel = _T("Latitude");
			m_TrgYLngLabel = _T("Longitude");
			if (m_ThreeDCalc) m_TrgZHgtLabel = _T("Height");
			else              m_TrgZHgtLabel = _T("");
		}
	}
	return true;
}
CWnd* CcsTest::Calculate ()
{
	int status;
	CWnd* errorPtr;
	struct cs_Csprm_ *srcPtr;
	struct cs_Csprm_ *trgPtr;
	struct cs_Dtcprm_ *dtcPtr;
	double xy [3], ll [3];
	TCHAR tcTemp [512];

	errorPtr = FetchSource ();
	if (errorPtr != NULL) goto error;

	xy [0] = m_SourceXY [0];
	xy [1] = m_SourceXY [1];
	if (m_ThreeDCalc) xy [2] = m_SourceXY [2];
	else              xy [2] = 0.0;

	// Set up the conversion.
	srcPtr = CSbcclu_T (m_SrcKeyName);
	if (srcPtr == NULL)
	{
		errorPtr = GetDlgItem (IDC_CSTST_SRCKEYNM);
		goto error;
	}
	trgPtr = CSbcclu_T (m_TrgKeyName);
	if (trgPtr == NULL)
	{
		errorPtr = GetDlgItem (IDC_CSTST_TRGKEYNM);
		goto error;
	}
	dtcPtr = CSbdclu (srcPtr,trgPtr,cs_DTCFLG_DAT_W,cs_DTCFLG_BLK_10);
	if (dtcPtr == NULL)
	{
		errorPtr = GetDlgItem (IDC_CSTST_SRCKEYNM);
		goto error;
	}

	// Do the conversion
	if (m_ThreeDCalc)
	{
		status = CS_cs3ll (srcPtr,ll,xy);
	}
	else
	{
		status = CS_cs2ll (srcPtr,ll,xy);
	}

	if (status == cs_CNVRT_OK)
	{
		m_SrcStatus = _T("OK");
	}
	else if (status == cs_CNVRT_USFL)
	{
		m_SrcStatus = _T("Range");
	}
	else if (status == cs_CNVRT_DOMN)
	{
		m_SrcStatus = _T("Domain");
	}
	else if (status == cs_CNVRT_DEMO)
	{
		m_SrcStatus = _T("Demo");
	}
	else
	{
		m_SrcStatus = _T("???");
	}
	// Source Grid Scale and Convergence.
	m_SourceScale = CS_csscl (srcPtr,ll);
	m_SourceConvergence = CS_cscnv (srcPtr,ll);
	UpdateSource ();

	// Do the datum shift.
	if (m_ThreeDCalc)
	{
		status = CS_dtcvt3D (dtcPtr,ll,ll);
	}
	else
	{
		status = CS_dtcvt (dtcPtr,ll,ll);
	}
	if (status == 0)
	{
		m_TrgStatus.Empty ();
	}
	else if (status < 0)
	{
		m_TrgStatus = _T("DtErr");
		errorPtr = GetDlgItem (IDC_CSTST_SRCKEYNM);
		goto error;
	}
	else
	{
		m_TrgStatus = _T("DtRng");
		CSt_errmsg (tcTemp,tchrCount (tcTemp));
		AfxMessageBox (tcTemp);
	}

	// Target conversion
	if (m_ThreeDCalc)
	{
		status = CS_ll3cs (trgPtr,xy,ll);
	}
	else
	{
		status = CS_ll2cs (trgPtr,xy,ll);
	}
	if (m_TrgStatus.IsEmpty ())
	{
		if (status == cs_CNVRT_OK)
		{
			m_TrgStatus = _T("OK");
		}
		else if (status == cs_CNVRT_USFL)
		{
			m_TrgStatus = _T("Range");
		}
		else if (status == cs_CNVRT_DOMN)
		{
			m_TrgStatus = _T("Domain");
		}
		else if (status == cs_CNVRT_DEMO)
		{
			m_TrgStatus = _T("Demo");
		}
		else
		{
			m_TrgStatus = _T("???");
		}
	}
	m_TargetXY [0] = xy [0];
	m_TargetXY [1] = xy [1];
	if (m_ThreeDCalc) m_TargetXY [2] = xy [2];
	else              m_TargetXY [2] = 0.0;
	// Target Grid Scale and Convergence.
	m_TargetScale = CS_csscl (trgPtr,ll);
	m_TargetConvergence = CS_cscnv (trgPtr,ll);
	// Update the display
	UpdateTarget ();
	UpdateData (FALSE);
	return NULL;
error:
	m_TrgStatus = _T("Error");
	m_TargetXY [0] = 0.0;
	m_TargetXY [1] = 0.0;
	m_TargetXY [2] = 0.0;
	UpdateTarget ();
	UpdateData (FALSE);
	return errorPtr;
}
/////////////////////////////////////////////////////////////////////////////
// CcsTest message handlers
void CcsTest::OnCststCalculate() 
{
	CWnd* errorPtr;
	CWnd* rvPtr;
	TCHAR tcTemp [256];

	if (!UpdateData (TRUE)) return;
	errorPtr = Calculate ();
	if (errorPtr != NULL)
	{
		CSt_errmsg (tcTemp,tchrCount (tcTemp));
		AfxMessageBox (tcTemp);
	}
	else
	{
		errorPtr = GetDlgItem (IDC_CSTST_SRCXLAT);
	}
	GotoDlgCtrl (errorPtr);
	rvPtr = GetDlgItem (ID_CSTST_REVERSE);
	if (rvPtr != NULL) rvPtr->EnableWindow (TRUE);
	UpdateData (FALSE);
}
void CcsTest::OnCststReverse() 
{
	CWnd* errorPtr;
	CString tmpSystem;
	double tmpXY [3];

	// Capture status of the check boxes before we proceed.
	if (!UpdateData (TRUE)) return;
	tmpSystem = m_SrcKeyName;
	m_SrcKeyName = m_TrgKeyName;
	m_TrgKeyName = tmpSystem;
	SetLabels (false);
	SetLabels (true);
	tmpXY [0] = m_SourceXY [0];
	tmpXY [1] = m_SourceXY [1];
	tmpXY [2] = m_SourceXY [2];
	m_SourceXY [0] = m_TargetXY [0];
	m_SourceXY [1] = m_TargetXY [1];
	m_SourceXY [2] = m_TargetXY [2];
	m_TargetXY [0] = tmpXY [0];
	m_TargetXY [1] = tmpXY [1];
	m_TargetXY [2] = tmpXY [2];
	UpdateSource ();
	FetchSource ();
	errorPtr = Calculate ();
	if (errorPtr == NULL)
	{
		UpdateTarget ();
		errorPtr = GetDlgItem (IDC_CSTST_SRCXLAT);
	}
	else
	{
		TCHAR tcTemp [256];
		CSt_errmsg (tcTemp,tchrCount (tcTemp));
		AfxMessageBox (tcTemp);
	}
	GotoDlgCtrl (errorPtr);
	UpdateData (FALSE);
	return;
}
void CcsTest::OnHelp() 
{
	CSwinhlp (m_hWnd,csHLPID_CSTEST);
}
void CcsTest::OnKillfocusCststSrckeynm() 
{
	if (!UpdateData (TRUE)) return;
	SetLabels (false);
	UpdateData (FALSE);	
}
void CcsTest::OnKillfocusCststTrgkeynm() 
{
	if (!UpdateData (TRUE)) return;
	SetLabels (true);
	UpdateData (FALSE);	
}

void CcsTest::OnChange() 
{
	CWnd* rvPtr;
	
	rvPtr = GetDlgItem (ID_CSTST_REVERSE);
	if (rvPtr != NULL) rvPtr->EnableWindow (FALSE);
}
void CcsTest::OnCststSrccsbtn() 
{
	CcsBrowse csBrowse;

	if (!UpdateData (TRUE)) return;
	csBrowse.SetInitialKeyName (m_SrcKeyName);
	INT_PTR st = csBrowse.DoModal ();
	if (st == IDOK)
	{
		m_SrcKeyName = csBrowse.GetSelectedKeyName ();
		SetLabels (false);
		OnChange ();
		UpdateData (FALSE);
	}
}
void CcsTest::OnCststTrgcsbtn() 
{
	CcsBrowse csBrowse;

	if (!UpdateData (TRUE)) return;
	csBrowse.SetInitialKeyName (m_TrgKeyName);
	INT_PTR st = csBrowse.DoModal ();
	if (st == IDOK)
	{
		m_TrgKeyName = csBrowse.GetSelectedKeyName ();
		SetLabels (true);
		OnChange ();
		UpdateData (FALSE);
	}
}
void CcsTest::OnCstst3DCheck () 
{
	int checked;
	CWnd *wp;

	checked = IsDlgButtonChecked (IDC_CSTST_3DCHECK);
	wp = GetDlgItem (IDC_CSTST_SRCZHGT);
	if (wp != NULL) wp->EnableWindow (checked != 0);
	wp = GetDlgItem (IDC_CSTST_SRCZHGTLBL);
	if (wp != NULL) wp->EnableWindow (checked != 0);
	wp = GetDlgItem (IDC_CSTST_TRGZHGT);
	if (wp != NULL) wp->EnableWindow (checked != 0);
	wp = GetDlgItem (IDC_CSTST_TRGZHGTLBL);
	if (wp != NULL) wp->EnableWindow (checked != 0);
}
void CcsTest::OnCststHelp() 
{
	CSwinhlp (m_hWnd,csHLPID_CSTEST);
}

#endif
