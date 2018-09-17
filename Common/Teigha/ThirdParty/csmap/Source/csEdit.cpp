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

extern "C" char csErrnam [];
extern "C" struct cs_Prjtab_ cs_Prjtab [];

/**********************************************************************
**	CelEdit elEdit;
**			elEdit.SetKeyName (initialName);
**			ok = elEdit.DoModal ();
**			finalName = elEdit.GeKeyName ();
**			extern "C" CS_elEditor (keyName);
**
**	char* initialName;			key name of the ellipsoid which is to be displayed
**								upon dialog activation.
**	int ok;						returns IDOK if user exited with OK button, else
**								returns IDCANCEL.
**	char* finalName;			pointer to static memory containing the key name
**								of the ellipsoid definition which was displayed
**								when the user terminated the dialog box.
**	char* keyName;				character array which holds the initial name
**								when the function is called, and in which the
**								function returns the name of the ellipsoid
**								which was displayed when the dialog terminated.
**
**	Note, that the finalName pointer becomes invalid when the
**	CelEdit object is detroyed (i.e. goes out of scope).
**********************************************************************/

///////////////////////////////////////////////////////////////////////////////
// The following table drives much of the Projection Parameter stuff.
// There must be precisely one element in this array per possible parameter.
const static struct TcsEdParmMap csEdParmMap [24] =
{
	{  0,  IDC_EDCS_PP_LBL1,   IDC_EDCS_PRJ_PRM1},
	{  1,  IDC_EDCS_PP_LBL2,   IDC_EDCS_PRJ_PRM2},
	{  2,  IDC_EDCS_PP_LBL3,   IDC_EDCS_PRJ_PRM3},
	{  3,  IDC_EDCS_PP_LBL4,   IDC_EDCS_PRJ_PRM4},
	{  4,  IDC_EDCS_PP_LBL5,   IDC_EDCS_PRJ_PRM5},
	{  5,  IDC_EDCS_PP_LBL6,   IDC_EDCS_PRJ_PRM6},
	{  6,  IDC_EDCS_PP_LBL7,   IDC_EDCS_PRJ_PRM7},
	{  7,  IDC_EDCS_PP_LBL8,   IDC_EDCS_PRJ_PRM8},
	{  8,  IDC_EDCS_PP_LBL9,   IDC_EDCS_PRJ_PRM9},
	{  9,  IDC_EDCS_PP_LBL10,  IDC_EDCS_PRJ_PRM10},
	{ 10,  IDC_EDCS_PP_LBL11,  IDC_EDCS_PRJ_PRM11},
	{ 11,  IDC_EDCS_PP_LBL12,  IDC_EDCS_PRJ_PRM12},
	{ 12,  IDC_EDCS_PP_LBL13,  IDC_EDCS_PRJ_PRM13},
	{ 13,  IDC_EDCS_PP_LBL14,  IDC_EDCS_PRJ_PRM14},
	{ 14,  IDC_EDCS_PP_LBL15,  IDC_EDCS_PRJ_PRM15},
	{ 15,  IDC_EDCS_PP_LBL16,  IDC_EDCS_PRJ_PRM16},
	{ 16,  IDC_EDCS_PP_LBL17,  IDC_EDCS_PRJ_PRM17},
	{ 17,  IDC_EDCS_PP_LBL18,  IDC_EDCS_PRJ_PRM18},
	{ 18,  IDC_EDCS_PP_LBL19,  IDC_EDCS_PRJ_PRM19},
	{ 19,  IDC_EDCS_PP_LBL20,  IDC_EDCS_PRJ_PRM20},
	{ 20,  IDC_EDCS_PP_LBL21,  IDC_EDCS_PRJ_PRM21},
	{ 21,  IDC_EDCS_PP_LBL22,  IDC_EDCS_PRJ_PRM22},
	{ 22,  IDC_EDCS_PP_LBL23,  IDC_EDCS_PRJ_PRM23},
	{ 23,  IDC_EDCS_PP_LBL24,  IDC_EDCS_PRJ_PRM24}
};
///////////////////////////////////////////////////////////////////////////////
// Basic 'C' callable function interface
void EXP_LVL1 CS_csEditor (char *csKeyName)
{
	TCHAR tcKeyName [cs_KEYNM_DEF];
	CcsEdit csEditor;

	CS_recvr ();
	CSt_strNCpy (tcKeyName,csKeyName,tchrCount (tcKeyName));
	csEditor.SetKeyName (tcKeyName);
	csEditor.DoModal ();
	CSt_strNCpy (csKeyName,csEditor.GetKeyName (),cs_KEYNM_DEF);
	return;
}
/////////////////////////////////////////////////////////////////////////////
// CcsEdit
CcsEdit::CcsEdit(CWnd* pParentWnd, UINT iSelectPage) : CPropertySheet(_T("Coordinate System Editor"), pParentWnd, iSelectPage)
{
	// Show Help button only if Help is indeed available.
	if (CS_isHlpAvailable ()) m_psh.dwFlags |=  PSH_HASHELP;

	AddPage (&m_PgIdent);
	AddPage (&m_PgGnrl);
	AddPage (&m_PgOrigins);
	AddPage (&m_PgExtents);
	AddPage (&m_PgParms1);
	AddPage (&m_PgParms2);
	AddPage (&m_PgParms3);
	AddPage (&m_PgParms4);

	m_PgIdent.m_PropSheet   = this;
	m_PgGnrl.m_PropSheet    = this;
	m_PgOrigins.m_PropSheet = this;
	m_PgExtents.m_PropSheet = this;
	m_PgParms1.m_PropSheet  = this;
	m_PgParms2.m_PropSheet  = this;
	m_PgParms3.m_PropSheet  = this;
	m_PgParms4.m_PropSheet  = this;

	m_Modified = false;
	m_Protected = false;
	m_NewIdx = -1;
	m_ParmCount = 0;
	m_KeyName = "LL";
	m_PreviousKeyName = "LL";
	m_LngFrmt = cs_ATOF_LNGDFLT + 2;
	m_LatFrmt = cs_ATOF_LATDFLT + 2;
	m_XFrmt = cs_ATOF_COMMA;
	m_YFrmt = cs_ATOF_COMMA;
	m_SclFrmt = 10L;
	m_AnglFrmt = (cs_ATOF_MINSEC | cs_ATOF_MINSEC0 | cs_ATOF_DIRCHR | cs_ATOF_XEAST | 5L);

	m_LngPrm.log_type = cs_PRMLTYP_LNG;
	m_LngPrm.format  = m_LngFrmt;
	m_LngPrm.min_val = -180.0;
	m_LngPrm.max_val = 360.0;
	m_LngPrm.deflt   = 0.0;

	m_ExLngPrm.log_type = cs_PRMLTYP_LNG;
	m_ExLngPrm.format  = m_LngFrmt;
	m_ExLngPrm.min_val = -270.0;
	m_ExLngPrm.max_val = 360.0;
	m_ExLngPrm.deflt   = 0.0;

	m_LatPrm.log_type = cs_PRMLTYP_LAT;
	m_LatPrm.format  = m_LatFrmt;
	m_LatPrm.min_val = -90.0;
	m_LatPrm.max_val = 90.0;
	m_LatPrm.deflt   = 0.0;

	m_XPrm.log_type = cs_PRMLTYP_AZM;
	m_XPrm.format    = m_XFrmt;
	m_XPrm.min_val   = -9.99999E+12;
	m_XPrm.max_val   = 9.99999E+12;
	m_XPrm.deflt     = 0.0;

	m_YPrm.log_type = cs_PRMLTYP_AZM;
	m_YPrm.format    = m_YFrmt;
	m_YPrm.min_val   = -9.99999E+12;
	m_YPrm.max_val   = 9.999999E+12;
	m_YPrm.deflt     = 0.0;

	m_ZroPrm.log_type = cs_PRMLTYP_AZM;
	m_ZroPrm.format  = 5L;
	m_ZroPrm.min_val = 0.0;
	m_ZroPrm.max_val = 1.0E+03;
	m_ZroPrm.deflt   = 0.0001; 
}
void CcsEdit::SetKeyName (const TCHAR* csKeyName)
{
	char kyTemp [cs_KEYNM_DEF + 2];
	TCHAR tcKeyName [cs_KEYNM_DEF + 1];

	CSt_strNCpy (kyTemp,csKeyName,sizeof (kyTemp));
	CS_nampp (kyTemp);
	if (CS_csIsValid (kyTemp))
	{
		CSt_strNCpy (tcKeyName,csKeyName,tchrCount (tcKeyName));
		m_KeyName = tcKeyName;
	}
}
const TCHAR* CcsEdit::GetKeyName ()
{
	return (LPCTSTR)m_KeyName;
}
void CcsEdit::SetLongitudeFormat (const long32_t format)
{
	m_LngFrmt = format;
	AdjustParmArray ();
}
void CcsEdit::SetLatitudeFormat (const long32_t format)
{
	m_LatFrmt = format;
	AdjustParmArray ();
}
void CcsEdit::SetLatLongFormat (const long32_t format)
{
	m_LngFrmt = format | cs_ATOF_XEAST;
	m_LatFrmt = format & (~((long32_t)cs_ATOF_XEAST));
	AdjustParmArray ();
}
void CcsEdit::SetXFormat (const long32_t format)
{
	m_XFrmt = format;
	AdjustParmArray ();
}
void CcsEdit::SetYFormat (const long32_t format)
{
	m_YFrmt = format;
	AdjustParmArray ();
}
void CcsEdit::SetXYFormat (const long32_t format)
{
	m_XFrmt = format | cs_ATOF_XEAST;
	m_YFrmt = format & (~((long32_t)cs_ATOF_XEAST));
	AdjustParmArray ();
}
BOOL CcsEdit::OnInitDialog (void)
{
	CWnd *wp;

	CPropertySheet::OnInitDialog ();

	GetCoordSys (m_KeyName);

	wp = GetDlgItem (IDOK);
	if (wp != NULL) wp->SetWindowText (_T("Close"));
	wp = GetDlgItem (IDCANCEL);
	if (wp != NULL) wp->SetWindowText (_T("New"));
	wp = GetDlgItem (ID_APPLY_NOW);
	if (wp != NULL)
	{
	    wp->SetWindowText (_T("Delete"));
	    wp->EnableWindow (!m_Protected);
	}
	GetActivePage()->OnSetActive ();
	return TRUE;
}
CcsEdit::~CcsEdit()
{
}
void CcsEdit::FillInData ()
{
	struct cs_Csdef_ cs_def;

	CollectData (&cs_def);
	CS_fillIn (&cs_def);
	DisperseData (&cs_def);
	return;
}
void CcsEdit::DisperseData (struct cs_Csdef_ *csPtr)
{
	int len;
	char *cp, *cpl;

	CSt_cstringAssign (m_KeyName,csPtr->key_nm);
	CSt_cstringAssign (m_PgIdent.m_Description,csPtr->desc_nm);
	CSt_cstringAssign (m_Description,csPtr->desc_nm);
	CSt_cstringAssign (m_PgIdent.m_Source,csPtr->source);
	CSt_cstringAssign (m_PgIdent.m_Group,csPtr->group);
	CSt_cstringAssign (m_PgIdent.m_CountrySt,csPtr->cntry_st);
	CSt_cstringAssign (m_PgIdent.m_Location,csPtr->locatn);

	CSt_cstringAssign (m_PgGnrl.m_PrjKeyName,csPtr->prj_knm);
	if (csPtr->dat_knm [0] != '\0')
	{
		CSt_cstringAssign (m_PgGnrl.m_RefType,"Datum");
		CSt_cstringAssign (m_PgGnrl.m_RefKeyName,csPtr->dat_knm);
		cp = csPtr->dat_knm;
		len = static_cast<int>(strlen (cp));
		cpl = cp + len - 1;
		m_PgGnrl.m_RefToDflt = (*cp == cs_DFLT_IDNTBEG && *cpl == cs_DFLT_IDNTEND);
		if (m_PgGnrl.m_RefToDflt)
		{
			m_PgGnrl.m_RefKeyName = m_PgGnrl.m_RefKeyName.Mid (1,len - 2);
		}
	}
	else
	{
		CSt_cstringAssign (m_PgGnrl.m_RefType,"Ellipsoid");
		CSt_cstringAssign (m_PgGnrl.m_RefKeyName,csPtr->elp_knm);
		cp = csPtr->elp_knm;
		len = static_cast<int>(strlen (cp));
		cpl = cp + len - 1;
		m_PgGnrl.m_RefToDflt = (*cp == cs_DFLT_IDNTBEG && *cpl == cs_DFLT_IDNTEND);
		if (m_PgGnrl.m_RefToDflt)
		{
			m_PgGnrl.m_RefKeyName = m_PgGnrl.m_RefKeyName.Mid (1,len - 2);
		}
	}
	CSt_cstringAssign (m_PgGnrl.m_Unit,csPtr->unit);
	cp = csPtr->unit;
	len = static_cast<int>(strlen (cp));
	cpl = cp + len - 1;
	m_PgGnrl.m_UnitDflt = (*cp == cs_DFLT_IDNTBEG && *cpl == cs_DFLT_IDNTEND);
	if (m_PgGnrl.m_UnitDflt)
	{
		m_PgGnrl.m_Unit = m_PgGnrl.m_Unit.Mid (1,len - 2);
	}
	m_PgGnrl.m_MapScale = csPtr->map_scl;
	m_PgGnrl.m_SclRed = csPtr->scl_red;
	m_PgGnrl.m_Quad = csPtr->quad;

	m_PgOrigins.m_OrgLng = csPtr->org_lng;
	m_PgOrigins.m_OrgLat = csPtr->org_lat;
	m_PgOrigins.m_XOff = csPtr->x_off;
	m_PgOrigins.m_YOff = csPtr->y_off;
	m_PgOrigins.m_NZeroX = csPtr->zero [0];
	m_PgOrigins.m_NZeroY = csPtr->zero [1];

	m_PgExtents.m_MinLat = csPtr->ll_min [1];
	m_PgExtents.m_MinLng = csPtr->ll_min [0];
	m_PgExtents.m_MaxLat = csPtr->ll_max [1];
	m_PgExtents.m_MaxLng = csPtr->ll_max [0];
	m_PgExtents.m_MinX = csPtr->xy_min [0];
	m_PgExtents.m_MinY = csPtr->xy_min [1];
	m_PgExtents.m_MaxX = csPtr->xy_max [0];
	m_PgExtents.m_MaxY = csPtr->xy_max [1];

	m_PgParms1.m_PrjParm1 = csPtr->prj_prm1;
	m_PgParms1.m_PrjParm2 = csPtr->prj_prm2;
	m_PgParms1.m_PrjParm3 = csPtr->prj_prm3;
	m_PgParms1.m_PrjParm4 = csPtr->prj_prm4;
	m_PgParms1.m_PrjParm5 = csPtr->prj_prm5;
	m_PgParms1.m_PrjParm6 = csPtr->prj_prm6;

	m_PgParms2.m_PrjParm7 = csPtr->prj_prm7;
	m_PgParms2.m_PrjParm8 = csPtr->prj_prm8;
	m_PgParms2.m_PrjParm9 = csPtr->prj_prm9;
	m_PgParms2.m_PrjParm10 = csPtr->prj_prm10;
	m_PgParms2.m_PrjParm11 = csPtr->prj_prm11;
	m_PgParms2.m_PrjParm12 = csPtr->prj_prm12;

	m_PgParms3.m_PrjParm13 = csPtr->prj_prm13;
	m_PgParms3.m_PrjParm14 = csPtr->prj_prm14;
	m_PgParms3.m_PrjParm15 = csPtr->prj_prm15;
	m_PgParms3.m_PrjParm16 = csPtr->prj_prm16;
	m_PgParms3.m_PrjParm17 = csPtr->prj_prm17;
	m_PgParms3.m_PrjParm18 = csPtr->prj_prm18;

	m_PgParms4.m_PrjParm19 = csPtr->prj_prm19;
	m_PgParms4.m_PrjParm20 = csPtr->prj_prm20;
	m_PgParms4.m_PrjParm21 = csPtr->prj_prm21;
	m_PgParms4.m_PrjParm22 = csPtr->prj_prm22;
	m_PgParms4.m_PrjParm23 = csPtr->prj_prm23;
	m_PgParms4.m_PrjParm24 = csPtr->prj_prm24;
}
void CcsEdit::CollectData (struct cs_Csdef_ *csPtr)
{
	char *cp;
	char ctemp [96];

	CSt_strNCpy (csPtr->key_nm,(LPCTSTR)m_KeyName,sizeof (csPtr->key_nm));
	CSt_strNCpy (csPtr->desc_nm,(LPCTSTR)m_PgIdent.m_Description,sizeof (csPtr->desc_nm));
	CSt_strNCpy (csPtr->source,(LPCTSTR)m_PgIdent.m_Source,sizeof (csPtr->source));
	CSt_strNCpy (csPtr->group,(LPCTSTR)m_PgIdent.m_Group,sizeof (csPtr->group));
	CSt_strNCpy (csPtr->cntry_st,(LPCTSTR)m_PgIdent.m_CountrySt,sizeof (csPtr->cntry_st));
	CSt_strNCpy (csPtr->locatn,(LPCTSTR)m_PgIdent.m_Location,sizeof (csPtr->locatn));

	CSt_strNCpy (csPtr->prj_knm,(LPCTSTR)m_PgGnrl.m_PrjKeyName,sizeof (csPtr->prj_knm));
	cp = ctemp;
	if (m_PgGnrl.m_RefToDflt) *cp++ = cs_DFLT_IDNTBEG;
	cp = CSt_strNCpy (cp,(LPCTSTR)m_PgGnrl.m_RefKeyName,sizeof (ctemp) - 2);
	if (m_PgGnrl.m_RefToDflt) *cp++ = cs_DFLT_IDNTEND;
	*cp = '\0';
	if (m_PgGnrl.m_RefType [0] == _T('D'))
	{
		csPtr->elp_knm [0] = '\0';
		CS_stncp (csPtr->dat_knm,ctemp,sizeof (csPtr->dat_knm));
	}
	else
	{
		csPtr->dat_knm [0] = '\0';
		CS_stncp (csPtr->elp_knm,ctemp,sizeof (csPtr->elp_knm));
	}
	cp = ctemp;
	if (m_PgGnrl.m_UnitDflt) *cp++ = cs_DFLT_IDNTBEG;
	cp = CSt_strNCpy (cp,(LPCTSTR)m_PgGnrl.m_Unit,sizeof (ctemp) - 2);
	if (m_PgGnrl.m_UnitDflt) *cp++ = cs_DFLT_IDNTEND;
	*cp = '\0';
	CS_stncp (csPtr->unit,ctemp,sizeof (csPtr->unit));
	csPtr->map_scl = m_PgGnrl.m_MapScale;
	csPtr->scl_red = m_PgGnrl.m_SclRed;
	csPtr->quad = m_PgGnrl.m_Quad;

	csPtr->org_lng = m_PgOrigins.m_OrgLng;
	csPtr->org_lat = m_PgOrigins.m_OrgLat;
	csPtr->x_off = m_PgOrigins.m_XOff;
	csPtr->y_off = m_PgOrigins.m_YOff;
	csPtr->zero [0] = m_PgOrigins.m_NZeroX;
	csPtr->zero [1] = m_PgOrigins.m_NZeroY;

	csPtr->ll_min [1] = m_PgExtents.m_MinLat;
	csPtr->ll_min [0] = m_PgExtents.m_MinLng;
	csPtr->ll_max [1] = m_PgExtents.m_MaxLat;
	csPtr->ll_max [0] = m_PgExtents.m_MaxLng;
	csPtr->xy_min [0] = m_PgExtents.m_MinX;
	csPtr->xy_min [1] = m_PgExtents.m_MinY;
	csPtr->xy_max [0] = m_PgExtents.m_MaxX;
	csPtr->xy_max [1] = m_PgExtents.m_MaxY;

	csPtr->prj_prm1  = m_PgParms1.m_PrjParm1;
	csPtr->prj_prm2  = m_PgParms1.m_PrjParm2;
	csPtr->prj_prm3  = m_PgParms1.m_PrjParm3;
	csPtr->prj_prm4  = m_PgParms1.m_PrjParm4;
	csPtr->prj_prm5  = m_PgParms1.m_PrjParm5;
	csPtr->prj_prm6  = m_PgParms1.m_PrjParm6;

	csPtr->prj_prm7  = m_PgParms2.m_PrjParm7;
	csPtr->prj_prm8  = m_PgParms2.m_PrjParm8;
	csPtr->prj_prm9  = m_PgParms2.m_PrjParm9;
	csPtr->prj_prm10 = m_PgParms2.m_PrjParm10;
	csPtr->prj_prm11 = m_PgParms2.m_PrjParm11;
	csPtr->prj_prm12 = m_PgParms2.m_PrjParm12;

	csPtr->prj_prm13 = m_PgParms3.m_PrjParm13;
	csPtr->prj_prm14 = m_PgParms3.m_PrjParm14;
	csPtr->prj_prm15 = m_PgParms3.m_PrjParm15;
	csPtr->prj_prm16 = m_PgParms3.m_PrjParm16;
	csPtr->prj_prm17 = m_PgParms3.m_PrjParm17;
	csPtr->prj_prm18 = m_PgParms3.m_PrjParm18;

	csPtr->prj_prm19 = m_PgParms4.m_PrjParm19;
	csPtr->prj_prm20 = m_PgParms4.m_PrjParm20;
	csPtr->prj_prm21 = m_PgParms4.m_PrjParm21;
	csPtr->prj_prm22 = m_PgParms4.m_PrjParm22;
	csPtr->prj_prm23 = m_PgParms4.m_PrjParm23;
	csPtr->prj_prm24 = m_PgParms4.m_PrjParm24;
}
///////////////////////////////////////////////////////////////////////////////
// ChangeCSys updates all data items in the CcsEdit class to reflect a new and
//	different coordinate system.  The state of all windows, etc. remains
//	unchanged.
bool CcsEdit::GetCoordSys (const TCHAR* keyName)
{
	struct cs_Csdef_ *cs_def;

	// Get the definition to be displayed.
	cs_def = CSt_csdef (keyName);
	if (cs_def == NULL)
	{
		TCHAR errMesg [256];
		CSt_errmsg (errMesg,tchrCount (errMesg));
		AfxMessageBox (errMesg);
		return false;
	}
	CS_fillIn (cs_def);
	// Determine the projection code number.
	if (!NewProjection (cs_def->prj_knm))
	{
		CS_free (cs_def);
		TCHAR errMesg [256];
		CSt_errmsg (errMesg,tchrCount (errMesg));
		AfxMessageBox (errMesg);
		return (false);
	}
	// We've pretty much verified that we will succeed.  Disperse
	// the data to the individual pages.
	DisperseData (cs_def);
	m_Protected = (CS_prchk (cs_def->protect) != 0);
	m_Modified = FALSE;
	CS_free (cs_def);
	return true;
}
///////////////////////////////////////////////////////////////////////////////
//	Given a projection code, update the m_PrjParms member of the class.
//	State of all display pages remains unchanged.
bool CcsEdit::NewProjection (const char* prjKeyName)
{
	int ii, st, newPgCnt, prvPgCnt;
	struct cs_Prjtab_ *prjPtr;

	prjPtr = PrjLookUp (prjKeyName);
	if (prjPtr == NULL)
	{
		CS_stncp (csErrnam,prjKeyName,MAXPATH);
		CS_erpt (cs_UNKWN_PROJ);
		return false;
	}
	m_PrjCode = prjPtr->code;
	m_PrjFlags = prjPtr->flags;
	m_UnitType = (m_PrjCode == cs_PRJCOD_UNITY) ? cs_UTYP_ANG : cs_UTYP_LEN;
	m_ParmCount = 0;
	for (ii = 0;ii < 24;ii++)
	{
		st = CS_prjprm (&m_PrjParms [ii],m_PrjCode,ii);
		if (st < 0)
		{
			TCHAR errMesg [256];
			CSt_errmsg (errMesg,tchrCount (errMesg));
			AfxMessageBox (errMesg);
			return false;
		}
		else if (st == 0)
		{	// This parameter is not used.
			memset (&m_PrjParms [ii],'\0',sizeof (struct cs_Prjprm_));
			m_PrjParms [ii].phys_type = cs_PRMPTYP_NONE;
			m_PrjParms [ii].log_type = cs_PRMCOD_NOTUSED;
			m_PrjParms [ii].prj_code = m_PrjCode;
			m_PrjParms [ii].parm_nbr = static_cast<short>(ii);
		}
		else
		{
			m_ParmCount += 1;
		}
	}
	// Remove any unnecessary pages.
	newPgCnt = (m_ParmCount + 5) / 6;
	prvPgCnt = GetPageCount () - 4;
	for (ii = prvPgCnt;ii > newPgCnt;ii -= 1)
	{
		switch (ii) {
		case 4: RemovePage (&m_PgParms4); break;
		case 3: RemovePage (&m_PgParms3); break;
		case 2: RemovePage (&m_PgParms2); break;
		case 1: RemovePage (&m_PgParms1); break;
		}
	}
	for (ii = prvPgCnt + 1;ii <= newPgCnt;ii += 1)
	{
		switch (ii) {
		case 1: AddPage (&m_PgParms1); break;
		case 2: AddPage (&m_PgParms2); break;
		case 3: AddPage (&m_PgParms3); break;
		case 4: AddPage (&m_PgParms4); break;
		}
	}
	// Finally, adjust the parameter array we built above to
	// include any of the format speciciations which were set
	// above.
	AdjustParmArray ();
	return true;
}
void CcsEdit::AdjustParmArray (void)
{
	// This function adjusts the default format specifications which
	// are returned by CS_prjprm with those maintained in this class.

	int ii;
	struct cs_Prjprm_ *prmPtr;

	for (ii = 0;ii < 24;ii += 1)
	{
		prmPtr = &m_PrjParms [ii];
		switch (prmPtr->log_type) {

		case cs_PRMLTYP_LNG: prmPtr->format = m_LngFrmt; break;
		case cs_PRMLTYP_LAT: prmPtr->format = m_LatFrmt; break;
		case cs_PRMLTYP_AZM: prmPtr->format = m_AnglFrmt; break;
		case cs_PRMLTYP_ANGD:   break;
		case cs_PRMLTYP_CMPLXC: break;
		case cs_PRMLTYP_ZNBR:   break;
		case cs_PRMLTYP_HSNS:   break;
		case cs_PRMLTYP_GHGT:   break;
		case cs_PRMLTYP_ELEV:   break;
		case cs_PRMLTYP_AFCOEF: prmPtr->format = 12L; break;
		case cs_PRMLTYP_XYCRD:  prmPtr->format =  4L; break;
		case cs_PRMLTYP_SCALE:  prmPtr->format = 11L; break;
		default:			    break;
		}
	}
	m_LngPrm.format = m_LngFrmt;
	m_LatPrm.format = m_LatFrmt;
	m_XPrm.format = m_XFrmt;
	m_YPrm.format = m_YFrmt;
}
///////////////////////////////////////////////////////////////////////////////
//	Update the state of all buttons based on the m_Modified and m_Protected
//	member variables.
void CcsEdit::SetButtons ()
{
	CWnd *wp;

	wp = GetDlgItem (IDCANCEL);
	if (wp != NULL)	wp->SetWindowText (m_Modified ? _T("Cancel") : _T("New"));
	wp = GetDlgItem (ID_APPLY_NOW);
	if (wp != NULL)
	{
		wp->EnableWindow (!m_Protected);	// Can't Delete/Save protected definition
		wp->SetWindowText (m_Modified ? _T("Save") : _T("Delete"));
	}
	m_PgIdent.m_KeyNameCombo.EnableWindow (!m_Modified);
	m_PgIdent.m_BrowseBtn.EnableWindow (!m_Modified);
}
///////////////////////////////////////////////////////////////////////////////
// ChangeCoordSys -- Brings in a, possibly, new and differect coordinate
//	system for editing.  The display of the currently active page is updated.
//	Previously existing coordinate system is overwritten, even if modified,
//	without notification.
bool CcsEdit::ChangeCoordSys (const TCHAR* keyName)
{
	if (!GetCoordSys (keyName)) return false;
	m_KeyName = keyName;
	m_Modified = false;
	m_NewIdx = -1;
	SetButtons ();
	UpdateData (FALSE);
	CPropertyPage* ppp = GetActivePage ();	// should never fail.
	if (ppp != NULL) ppp->OnSetActive ();
	return true;
}
///////////////////////////////////////////////////////////////////////////////
//	Initialize m_CSDef and m_PrjParms for a new coordinate system.
void CcsEdit::NewCoordSys (const CString& csKeyName)
{
	struct cs_Prjtab_ *prjPtr;
	struct cs_Csdef_ csDef;

	m_KeyName = csKeyName;
	memset (&csDef,'\0',sizeof (csDef));
	CSt_strNCpy (csDef.key_nm,(LPCTSTR)csKeyName,sizeof (csDef.key_nm));
	CS_stncp (csDef.dat_knm,"WGS84",sizeof (csDef.dat_knm));
	csDef.elp_knm [0] = '\0';
	prjPtr = PrjLookUp ("TM");
	CS_stncp (csDef.prj_knm,prjPtr->key_nm,sizeof (csDef.prj_knm));
	m_PrjCode = prjPtr->code;
	m_PrjFlags = prjPtr->flags;
	if ((m_PrjFlags & cs_PRJFLG_GEOGR) != 0)
	{
		m_UnitType = cs_UTYP_ANG;
		CS_stncp (csDef.unit,"Degree",sizeof (csDef.unit));
		csDef.zero [0] = 1.0E-10;
		csDef.zero [1] = 1.0E-10;
	}
	else
	{
		m_UnitType = cs_UTYP_LEN;
		CS_stncp (csDef.unit,"Meter",sizeof (csDef.unit));
		csDef.zero [0] = 0.001;
		csDef.zero [1] = 0.001;
	}
	CS_stncp (csDef.group,"USER",sizeof (csDef.group));
	csDef.scl_red = 1.0;
	csDef.map_scl = 1.0;
	csDef.quad = 1;
	NewProjection (prjPtr->key_nm);
	DisperseData (&csDef);
	return;
}
///////////////////////////////////////////////////////////////////////////////
// Write the current definition back out to the dictionary.  Returns the
//	status.  This is used to determine if the combo box needs to be
//	rebuilt.
int CcsEdit::PutCoordSys ()
{
	int st, errCount;
	int errList [4];
	struct cs_Csdef_ csDef;

	CollectData (&csDef);
	csDef.protect = 0;
	// Check the coordinate System.
	errCount = CS_cschk (&csDef,cs_CSCHK_DATUM | cs_CSCHK_ELLIPS,errList,4);
	if (errCount < 0)
	{
		TCHAR errMesg [256];
		CSt_errmsg (errMesg,tchrCount (errMesg));
		AfxMessageBox (errMesg);
		return (errCount);
	}
	else if (errCount > 0)
	{
		TCHAR errMesg [256];
		CS_stncp (csErrnam,csDef.key_nm,MAXPATH);
		CS_erpt (errList [0]);
		CSt_errmsg (errMesg,tchrCount (errMesg));
		AfxMessageBox (errMesg);
		if (errCount > 1)
		{
			CS_erpt (errList [1]);
			CSt_errmsg (errMesg,tchrCount (errMesg));
			AfxMessageBox (errMesg);
		}
		if (errCount > 2)
		{
			CS_erpt (errList [2]);
			CSt_errmsg (errMesg,tchrCount (errMesg));
			AfxMessageBox (errMesg);
		}
		if (errCount > 3)
		{
			CS_erpt (errList [3]);
			CSt_errmsg (errMesg,tchrCount (errMesg));
			AfxMessageBox (errMesg);
		}
		return (-errList [0]);
	}
	st = CS_csupd (&csDef,0);
	if (st < 0)
	{
		TCHAR errMesg [256];
		CSt_errmsg (errMesg,tchrCount (errMesg));
		AfxMessageBox (errMesg);
	}
	return (st);
}
///////////////////////////////////////////////////////////////////////////////
// Delete the current coordinate system definition.  This implies we display
//	another one.	
bool CcsEdit::DeleteCoordSys (const CString& keyName)
{
	bool rtnValue;
	int st = IDOK;
	struct cs_Csdef_ csDef;

	if (m_Protected)
	{
		AfxMessageBox (_T("Displayed coordinate system definition is protected and may not be deleted."));
		st = IDCANCEL;
		rtnValue = false;
	}
	else
	{
		st = AfxMessageBox (_T("Click 'OK' to confirm your request to delete this coordinate system definition."),MB_OKCANCEL | MB_ICONQUESTION | MB_DEFBUTTON2);
		rtnValue = false;
	}
	if (st == IDOK)
	{
		memset (&csDef,'\0',sizeof (csDef));
		CSt_strNCpy (csDef.key_nm,m_KeyName,sizeof (csDef.key_nm));
		int st = CS_csdel (&csDef);
		if (st != 0)
		{
			TCHAR errMesg [256];
			CSt_errmsg (errMesg,tchrCount (errMesg));
			AfxMessageBox (errMesg);
			rtnValue = false;
		}
	}
	return rtnValue;
}
///////////////////////////////////////////////////////////////////////////////
//
BEGIN_MESSAGE_MAP(CcsEdit, CPropertySheet)
	//{{AFX_MSG_MAP(CcsEdit)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
/////////////////////////////////////////////////////////////////////////////
// CcsEdit message handlers
void CcsEdit::OnChange() 
{
	if (!m_Modified)
	{
		if (m_Protected)
		{
			AfxMessageBox (_T("Current definition is protected.  You will\n")
						   _T("not be able to save your changes."));
		}
		m_Modified = true;
		SetButtons ();
	}
}
void CcsEdit::OnCsEdDelSav()
{	// Here when the APPLY_NOW button is pressed.  This is multiplexed
	// as Delete and Save.
	int st;
	if (m_Modified)
	{	// Here to perform the save operation.  Update the data from the
		// currently active window.
		if (!GetActivePage()->UpdateData (TRUE)) return;
		st = PutCoordSys ();
		if (st >= 0)
		{
			m_NewIdx = -1;
			CS_recvr ();
			m_PgIdent.RebuildComboBox ();
			ChangeCoordSys (m_KeyName);
		}
	}
	else
	{	// Here to perform the delete operation.  Must determine
		// which coordinate system to display after the delete operation.
		char csName [32];
		int newIdx = 0;
		for (int ii = 0;CS_csEnum (ii,csName,sizeof (csName)) > 0;ii += 1)
		{
			if (m_KeyName == csName) break;
			newIdx = ii;		
		}
		if (CS_csEnum (newIdx,csName,sizeof (csName)) <= 0)
		{	// Display LL if all else fails.
			strcpy (csName,"LL");
		}
		if (DeleteCoordSys (m_KeyName))
		{
			CSt_cstringAssign (m_KeyName,csName);
			// Rebuild the combo box with new names and indices.
			CS_recvr ();
			m_PgIdent.RebuildComboBox ();
			// Select the new coordinate system
			ChangeCoordSys (m_KeyName);
		}
	}
}
///////////////////////////////////////////////////////////////////////////////
//	Function handles the multiplexing of the IDCANCEL button.
void CcsEdit::OnCsEdNewCncl() 
{
	if (m_Modified)
	{	// Here to perform a cancel.
		if (m_NewIdx >= 0)
		{	// The current coordinate system is a new one.
			m_PgIdent.m_KeyNameCombo.DeleteString (m_NewIdx);
			m_NewIdx = -1;
			m_KeyName = m_PreviousKeyName;
		}
		ChangeCoordSys (m_KeyName);
	}
	else
	{	// Here to create a new coordinate system.  Save the current name for
		// use in the event of a subsequent cancel operation.  Then, have
		// operator provide the name of the new coordinate system.
		m_PreviousKeyName = m_KeyName;
		CnewKeyName csNewName (CnewKeyName::CoordSys,this);
		INT_PTR st = csNewName.DoModal ();
		if (st == IDOK)
		{
			m_KeyName = csNewName.GetNewName ();
			// See if we are to reset all values.
			if (!csNewName.GetPreserve ())
			{
				NewCoordSys (m_KeyName);
			}
			m_Modified = true;
			m_Protected = false;
			m_NewIdx = m_PgIdent.m_KeyNameCombo.AddString (m_KeyName);
			m_PgIdent.m_KeyNameCombo.SetItemData (m_NewIdx,99999);
			m_PgIdent.m_KeyNameCombo.SelectString (-1,m_KeyName);
			SetButtons ();
		}
	}
	// Make sure the current display is correct.
	GetActivePage()->OnSetActive ();
}
void CcsEdit::OnCsEdClose()
{
	int response;
	if (m_Modified && !m_Protected)
	{
		response = AfxMessageBox (_T("Changes have been made, but not saved.\n")
								  _T("Do you to exit and discard these changes?"),
								  MB_YESNO | MB_ICONQUESTION);
		if (response == IDNO) return;
	}
	CPropertySheet::EndDialog (0);
	return;
}
///////////////////////////////////////////////////////////////////////////////
// The following intercepts the messages for the specific buttons of
// the property page.  This disables some of the Microsoft software
// default processing.
BOOL CcsEdit::OnCommand(WPARAM wParam,LPARAM lParam) 
{
	BOOL rtnValue;
	UINT nID;
	UINT nCode;

	rtnValue = TRUE;	
	nCode = HIWORD(wParam);
	nID = LOWORD(wParam);
	if (nCode != BN_CLICKED || (nID != IDOK && nID != IDCANCEL && nID != ID_APPLY_NOW))
	{
		// Message is something other than one of the standard four buttons.
		rtnValue = CPropertySheet::OnCommand(wParam,lParam);
	}
	else
	{
		switch (nID) {

		case IDOK:
			// This is our "Close" button.
			OnCsEdClose();
			break;

		case IDCANCEL:
			// Button is multiplexed between Cancel and New.
			OnCsEdNewCncl ();
			break;

		case ID_APPLY_NOW:
			// Button is multiplexed between Save and Delete.
			OnCsEdDelSav (); 
			break;

		default:
			rtnValue = CPropertySheet::OnCommand(wParam, lParam);
			break;
		// Help button is handled individually on each page.
		}
	}
	return rtnValue;	
}
/////////////////////////////////////////////////////////////////////////////
// CcsEdIdent property page

CcsEdIdent::CcsEdIdent() : CPropertyPage(CcsEdIdent::IDD)
{
	//{{AFX_DATA_INIT(CcsEdIdent)
	m_CountrySt = _T("");
	m_Description = _T("");
	m_Source = _T("");
	m_Location = _T("");
	//}}AFX_DATA_INIT
	if (CS_isHlpAvailable ()) m_psp.dwFlags |=  PSP_HASHELP;
}
BOOL CcsEdIdent::OnInitDialog ()
{
	int ii, idx, st;
	TCHAR keyName [32];
	TCHAR description [64];

	CDialog::OnInitDialog ();
	RebuildComboBox ();		// Key name combo box.
	// Do the group combo box.  List does not change.  Note, we code
	// to allow Windows to sort the combo box.
	for (ii = 0;true;ii += 1)
	{
		st = CSt_csGrpEnum (ii,keyName,tchrCount (keyName),description,tchrCount (description));
		if (st <= 0) break;
		idx = m_GroupCombo.AddString (description);
		m_GroupCombo.SetItemData (idx,ii);
	}
	// st will be ngative here in the event of an error.  What would you do???
	ASSERT (st == 0);
	UpdateData (FALSE);
	return TRUE;
}
CcsEdIdent::~CcsEdIdent()
{
}
//	Rebuild the key name combo box.
void CcsEdIdent::RebuildComboBox ()
{
	int ii, idx;
	int st = 0;
	TCHAR csName [32];

	ASSERT (m_PropSheet->m_NewIdx == -1);
	m_KeyNameCombo.ResetContent ();
	for (ii = 0;(st = CSt_csEnum (ii,csName,tchrCount (csName))) > 0;ii += 1)
	{
		idx = m_KeyNameCombo.AddString (csName);
		m_KeyNameCombo.SetItemData (idx,(DWORD)ii);
	}
	if (st < 0)
	{
		TCHAR errMesg [256];
		CSt_errmsg (errMesg,tchrCount (errMesg));
		AfxMessageBox (errMesg);
	}
	st = m_KeyNameCombo.SelectString (-1,m_PropSheet->m_KeyName);
	ASSERT (st != CB_ERR);
}
void CcsEdIdent::DoDataExchange(CDataExchange* pDX)
{
	int st, ii;
	DWORD idx;
	TCHAR keyName [32], description [128];

	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CcsEdIdent)
	DDX_Control(pDX, IDC_CSID_GROUP, m_GroupCombo);
	DDX_Control(pDX, IDC_CSID_BRWSE, m_BrowseBtn);
	DDX_Control(pDX, IDC_CSID_KEYNAME, m_KeyNameCombo);
	DDX_Text(pDX, IDC_CSID_CNTRYST, m_CountrySt);
	DDV_MaxChars(pDX, m_CountrySt, 47);
	DDX_Text(pDX, IDC_CSID_DESCR, m_Description);
	DDV_MaxChars(pDX, m_Description, 63);
	DDX_Text(pDX, IDC_CSID_SOURCE, m_Source);
	DDV_MaxChars(pDX, m_Source, 63);
	DDX_Text(pDX, IDC_CSID_LOCATN, m_Location);
	DDV_MaxChars(pDX, m_Location, 23);
	//}}AFX_DATA_MAP
	// Do the combo boxes.
	if (pDX->m_bSaveAndValidate)
	{
		ii = m_GroupCombo.GetCurSel ();
		idx = static_cast<int>(m_GroupCombo.GetItemData (ii));
		st = CSt_csGrpEnum (idx,keyName,tchrCount (keyName),NULL,0);
		ASSERT (st == 1);
		m_Group = keyName;
	}
	else
	{
		st = 0;
		for (ii = 0;true;ii += 1)
		{
			st = CSt_csGrpEnum (ii,keyName,tchrCount (keyName),description,tchrCount (description));
			if (st <= 0 || !CSt_stricmp (keyName,m_Group)) break;
		}
		if (st > 0) m_GroupCombo.SelectString (-1,description);
	}
}
BEGIN_MESSAGE_MAP(CcsEdIdent, CPropertyPage)
	//{{AFX_MSG_MAP(CcsEdIdent)
	ON_BN_CLICKED(IDC_CSID_BRWSE, OnCsidBrowse)
	ON_BN_CLICKED(ID_HELP,OnHelpButton)
	ON_EN_CHANGE(IDC_CSID_CNTRYST, OnChange)
	ON_CBN_SELENDOK(IDC_CSID_KEYNAME, OnSelendokKeyName)
	ON_CBN_SELCHANGE(IDC_CSID_GROUP, OnChange)
	ON_EN_CHANGE(IDC_CSID_SOURCE, OnChange)
	ON_CBN_CLOSEUP(IDC_CSID_KEYNAME, OnSelendokKeyName)
	ON_EN_CHANGE(IDC_CSID_DESCR, OnChange)
	ON_EN_KILLFOCUS(IDC_CSID_DESCR, OnKillfocusCsidDescr)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CcsEdIdent message handlers
void CcsEdIdent::OnSelendokKeyName() 
{
	int ii;
	DWORD idx;
	TCHAR keyName [32];

	ii = m_KeyNameCombo.GetCurSel ();
	idx = static_cast<int>(m_KeyNameCombo.GetItemData (ii));
	CSt_csEnum (idx,keyName,tchrCount (keyName));
	if (m_PropSheet->m_KeyName != keyName)
	{
		m_PropSheet->ChangeCoordSys (keyName);
	}
}
void CcsEdIdent::OnCsidBrowse() 
{
	int st;
	CcsBrowse csBrowse;

	csBrowse.SetInitialKeyName (m_PropSheet->m_KeyName);
	INT_PTR stDlg = csBrowse.DoModal ();
	if (stDlg == IDOK)
	{
		CString newKeyName;
		newKeyName = csBrowse.GetSelectedKeyName ();
		st = m_KeyNameCombo.SelectString (-1,newKeyName);
		ASSERT (st != CB_ERR);
		m_PropSheet->ChangeCoordSys (csBrowse.GetSelectedKeyName ());
	}
}
void CcsEdIdent::OnHelpButton() 
{
	CSwinhlp (m_hWnd,csHLPID_CSED_IDENT);
}
void CcsEdIdent::OnChange() 
{
	m_PropSheet->OnChange ();	
}
void CcsEdIdent::OnKillfocusCsidDescr() 
{
	CWnd *wp;
	TCHAR tcTemp [96];

	wp = GetDlgItem (IDC_CSID_DESCR);
	if (wp != NULL)
	{
		wp->GetWindowText (tcTemp,tchrCount (tcTemp));
		m_PropSheet->m_Description = tcTemp;
	}
}
/////////////////////////////////////////////////////////////////////////////
// CcsEdGnrl property page
CcsEdGnrl::CcsEdGnrl() : CPropertyPage(CcsEdGnrl::IDD)
{
	//{{AFX_DATA_INIT(CcsEdGnrl)
	m_MapRatio = _T("");
	m_RedRatio = _T("");
	m_RefKeyName = _T("");
	m_RefType = _T("");
	m_RefToDflt = FALSE;
	m_UnitDflt = FALSE;
	//}}AFX_DATA_INIT

	if (CS_isHlpAvailable ()) m_psp.dwFlags |=  PSP_HASHELP;
	m_PrjKeyName = _T("");
	m_MapScale = 1.0;
	m_SclRed = 1.0;
}
CcsEdGnrl::~CcsEdGnrl()
{
}
/*static char quadLabels [8][60] =
{
	"X/Lng increases to the east, Y/Lat increases to the north",
	"X/Lng increases to the west, Y/Lat increases to the north",
	"X/Lng increases to the west, Y/Lat increases to the south",
	"X/Lng increases to the east, Y/Lat increases to the south",
	"X increases to the north, Y increases to the east",
	"X increases to the north, Y increases to the west",
	"X increases to the south, Y increases to the west",
	"X increases to the south, Y increases to the east"
}; */
BOOL CcsEdGnrl::OnInitDialog ()
{
	int ii, idx, ident;
	ulong32_t prjFlags;
	TCHAR keyName [32];
	TCHAR description [64];

	CDialog::OnInitDialog ();

	// Set up the combo boxes.
	// Projection
	m_Projection.ResetContent ();
	for (ii = 0;
	     CSt_prjEnum (ii,&prjFlags,keyName,tchrCount (keyName),description,tchrCount (description)) > 0;
		 ii += 1)
	{
		idx = m_Projection.AddString (description);
		m_Projection.SetItemData (idx,ii);
	}
	// Units
	RebuildUnits (m_PropSheet->m_UnitType);
	// Quad
	m_QuadCombo.ResetContent ();
	for (ii = 0;
	     CSt_quadEnum (ii,description,tchrCount (description),&ident) > 0;
	     ii += 1)
	{
		idx = m_QuadCombo.AddString (description);
		m_QuadCombo.SetItemData (idx,ident + 100);	// DWORDS are unsigned.
	}
	UpdateData (FALSE);
	return TRUE;
}

BOOL CcsEdGnrl::OnSetActive ()
{
	SetScaleRatio (m_RedRatio,0,m_SclRed,true);
	SetScaleRatio (m_MapRatio,0,m_MapScale,false);
	return CPropertyPage::OnSetActive ();
}

void CcsEdGnrl::DoDataExchange(CDataExchange* pDX)
{
	int st;
	int ii, idx;
	ulong32_t prjFlags;
	CWnd *wp;
	TCHAR keyName [32];
	TCHAR description [64];

	wp = GetDlgItem (IDC_CSGN_SCLRED);
	if (wp != NULL) wp->EnableWindow ((m_PropSheet->m_PrjFlags & cs_PRJFLG_SCLRED) != 0);

	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CcsEdGnrl)
	DDX_Control(pDX, IDC_CSGN_UNIT, m_UnitCombo);
	DDX_Control(pDX, IDC_CSGN_QUAD, m_QuadCombo);
	DDX_Control(pDX, IDC_CSGN_PRJCTN, m_Projection);
	DDX_Text(pDX, IDC_CSGN_MAPRATIO, m_MapRatio);
	DDV_MaxChars(pDX, m_MapRatio, 18);
	DDX_Text(pDX, IDC_CSGN_REDRATIO, m_RedRatio);
	DDV_MaxChars(pDX, m_RedRatio, 18);
	DDX_Text(pDX, IDC_CSGN_REFKYNM, m_RefKeyName);
	DDV_MaxChars(pDX, m_RefKeyName, 24);
	DDX_Text(pDX, IDC_CSGN_REFTYP, m_RefType);
	DDV_MaxChars(pDX, m_RefType, 9);
	DDX_Check(pDX, IDC_CSGN_DEFLT_CHK, m_RefToDflt);
	DDX_Check(pDX, IDC_UNIT_DEFLT_CHK, m_UnitDflt);
	//}}AFX_DATA_MAP
	DDX_StaticLabelT (pDX,IDC_CSGN_SELDSCR,m_PropSheet->m_Description);
	DDX_Scale (pDX,IDC_CSGN_SCLRED,m_SclRed,true);
	DDX_Scale (pDX,IDC_CSGN_MAPSCL,m_MapScale,false);
	// Do the combo boxes.
	if (pDX->m_bSaveAndValidate)
	{	// Capture possibly modified data.
		// Map projection. We only deal with the name here.
		ii = m_Projection.GetCurSel ();
		idx = static_cast<int>(m_Projection.GetItemData (ii));
		st = CSt_prjEnum (idx,&prjFlags,keyName,tchrCount (keyName),description,tchrCount (description));
		ASSERT (st > 0);
		m_PrjKeyName = keyName;
		// Units
		ii = m_UnitCombo.GetCurSel ();
		idx = static_cast<int>(m_UnitCombo.GetItemData (ii));
		st = CSt_unEnum (idx,m_PropSheet->m_UnitType,keyName,tchrCount (keyName));
		ASSERT (st == 1);
		m_Unit = keyName;
		// Quad
		ii = m_QuadCombo.GetCurSel ();
		m_Quad = (short)m_QuadCombo.GetItemData (ii) - 100;
	}
	else
	{	// Display appropriate representation.
		// Map Projection
		for (ii = 0;true;ii += 1)
		{
			st = CSt_prjEnum (ii,&prjFlags,keyName,tchrCount (keyName),description,tchrCount (description));
			if (st < 1 || !CSt_stricmp (keyName,m_PrjKeyName)) break;
		}
		ASSERT (st > 0);
		m_Projection.SelectString (-1,description);
		// Units
		RebuildUnits (m_PropSheet->m_UnitType);
		for (ii = 0;true;ii += 1)
		{
			st = CSt_unEnum (ii,m_PropSheet->m_UnitType,keyName,tchrCount (keyName));
			if (st < 1 || !CSt_stricmp (keyName,m_Unit)) break;
		}
		ASSERT (st == 1);
		m_UnitCombo.SelectString (-1,keyName);
		// Quad
		for (ii = 0;ii < m_QuadCombo.GetCount ();ii += 1)
		{
			if (m_Quad == ((short)m_QuadCombo.GetItemData (ii) - 100))
			{
				m_QuadCombo.SetCurSel (ii);
				break;
			}
		}
		ASSERT (m_QuadCombo.GetCount() == 0 || ii < m_QuadCombo.GetCount ());
	}
	SetScaleRatio (m_RedRatio,0,m_SclRed,true);
	SetScaleRatio (m_MapRatio,0,m_MapScale,false);
}
BEGIN_MESSAGE_MAP(CcsEdGnrl, CPropertyPage)
	//{{AFX_MSG_MAP(CcsEdGnrl)
	ON_BN_CLICKED(IDC_CSGN_DEFLT_CHK, OnChange)
	ON_CBN_SELENDOK(IDC_CSGN_PRJCTN, OnSelendokCsgnPrjctn)
	ON_CBN_SELENDOK(IDC_CSGN_QUAD, OnSelendokCsgnQuad)
	ON_CBN_SELENDOK(IDC_CSGN_UNIT, OnSelendokCsgnUnit)
	ON_BN_CLICKED(IDC_CSGN_ELPBTN, OnCsgnElpbtn)
	ON_BN_CLICKED(IDC_CSGN_DATBTN, OnCsgnDatbtn)
	ON_EN_CHANGE(IDC_CSGN_MAPSCL, OnChange)
	ON_EN_CHANGE(IDC_CSGN_SCLRED, OnChange)
	ON_BN_CLICKED(IDC_UNIT_DEFLT_CHK, OnChange)
	ON_EN_KILLFOCUS(IDC_CSGN_SCLRED, OnKillfocusCsgnSclred)
	ON_EN_KILLFOCUS(IDC_CSGN_MAPSCL, OnKillfocusCsgnMapscl)
	ON_BN_CLICKED(ID_HELP,OnHelpButton)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
/////////////////////////////////////////////////////////////////////////////
// CcsEdGnrl message handlers
void CcsEdGnrl::OnChange() 
{
	m_PropSheet->OnChange ();	
}
void CcsEdGnrl::OnHelpButton() 
{
	CSwinhlp (m_hWnd,csHLPID_CSED_GENRL);
}
void CcsEdGnrl::OnSelendokCsgnPrjctn() 
{
	int ii, idx, st;
	int oldUnits;
	ulong32_t prjFlags;
	char prjKeyName [32];
	TCHAR keyName [32];
	TCHAR description [64];

	// User selected a new projection.
	oldUnits = m_PropSheet->m_UnitType;
	m_PropSheet->OnChange ();
	ii = m_Projection.GetCurSel ();
	idx = static_cast<int>(m_Projection.GetItemData (ii));
	st = CSt_prjEnum (idx,&prjFlags,keyName,tchrCount (keyName),description,tchrCount (description));
	ASSERT (st > 0);
	CSt_strNCpy (prjKeyName,keyName,sizeof (prjKeyName));
	if (!m_PropSheet->NewProjection (prjKeyName))
	{
		TCHAR errMesg [256];
		CSt_errmsg (errMesg,tchrCount (errMesg));
		AfxMessageBox (errMesg);
		return;
	}
	m_PrjKeyName = keyName;
	// If the unit type has changed, we need to deal with the new units.
	if (oldUnits != m_PropSheet->m_UnitType)
	{
		RebuildUnits (m_PropSheet->m_UnitType);
		if (m_PropSheet->m_UnitType == cs_UTYP_LEN)
		{
			AfxMessageBox (_T("Projection change required change in type of system unit;\n")
						   _T("system unit is being set to Meter."));
			m_Unit = _T("Meter");
		}
		else
		{
			AfxMessageBox (_T("Projection change required change in type of system unit;\n")
						   _T("system unit is being set to Degree."));
			m_Unit = _T("Degree");
		}
	}
	UpdateData (FALSE);
}
void CcsEdGnrl::RebuildUnits (int unitType)
{
	TCHAR keyName [32];
	int ii, idx;

	m_UnitCombo.ResetContent ();
	for (ii = 0;
		 CSt_unEnum (ii,unitType,keyName,tchrCount (keyName)) > 0;
		 ii += 1)
	{
		idx = m_UnitCombo.AddString (keyName);
		m_UnitCombo.SetItemData (idx,ii);
	}
}
void CcsEdGnrl::OnSelendokCsgnQuad() 
{
	// TODO: Add your control notification handler code here
	m_PropSheet->OnChange ();
}
void CcsEdGnrl::OnSelendokCsgnRefto() 
{
	// TODO: Add your control notification handler code here
	// Get the current selection.  Deal with the default issue.	
}
void CcsEdGnrl::OnSelendokCsgnUnit() 
{
	m_PropSheet->OnChange ();	
}
//	Compute/Display Scale Ratio
void CcsEdGnrl::SetScaleRatio (CString& result,int nIDC,double scale,bool reduction)
{
	TCHAR tcTemp [12];

	if (scale <= 0.0)
	{
		result.Empty ();
	}
	else
	{
		if (fabs (scale - 1.0) < 1.0E-14)
		{
			result = _T("1:1");
		}
		else
		{
			if (reduction)
			{
				scale = 1.0 / (1.0 - scale);
			}
			CSt_ftoa (tcTemp,tchrCount (tcTemp),scale,cs_ATOF_COMMA | 1L);
			result.Format (_T("1:%s"),tcTemp);
		}
	}
	if (nIDC != 0)
	{
		CWnd *wp = GetDlgItem (nIDC);
		if (wp != NULL) wp->SetWindowText (result);
	}
}
void CcsEdGnrl::OnCsgnElpbtn() 
{
	CString initialKeyName;
	CelSelect elSelect;

	initialKeyName = m_RefKeyName;
	if (m_RefType [0] == _T('D'))
	{
		struct cs_Dtdef_ *dtPtr;

		initialKeyName = _T("WGS84");
		dtPtr = CSt_dtdef (m_RefKeyName);
		if (dtPtr != NULL)
		{
			CSt_cstringAssign (initialKeyName,dtPtr->ell_knm);
			CS_free (dtPtr);
		}
	}
	elSelect.SetInitialEllipsoid (initialKeyName);
	INT_PTR st = elSelect.DoModal ();
	if (st == IDOK)
	{
		if (m_RefType [0] != _T('E') ||
		    CSt_stricmp (m_RefKeyName,elSelect.GetSelectedEllipsoid ()))
		{
			m_PropSheet->OnChange ();
			m_RefKeyName = elSelect.GetSelectedEllipsoid ();
			m_RefType = _T("Ellipsoid");
			UpdateData (FALSE);
		}
	}
}
void CcsEdGnrl::OnCsgnDatbtn() 
{
	CdtSelect dtSelect;
	dtSelect.SetInitialDatum (m_RefKeyName);
	INT_PTR st = dtSelect.DoModal ();
	if (st == IDOK)
	{
		if (m_RefType [0] != _T('D') ||
		    CSt_stricmp (m_RefKeyName,dtSelect.GetSelectedDatum ()))
		{
			m_PropSheet->OnChange ();
			m_RefKeyName = dtSelect.GetSelectedDatum ();
			m_RefType = _T("Datum");
			UpdateData (FALSE);
		}
	}
}
void CcsEdGnrl::OnKillfocusCsgnSclred() 
{
	CWnd* wp;
	TCHAR *cp;
	double nn, dd, dblTmp;
	CString dummy;
	TCHAR tcTemp [32];

	wp = GetDlgItem (IDC_CSGN_SCLRED);
	if (wp == NULL) return;
	wp->GetWindowText (tcTemp,tchrCount (tcTemp));
	cp = _tcschr (tcTemp,_T(':'));
	if (cp != NULL)
	{
		*cp++ = _T('\0');
		CSt_atof (&nn,tcTemp);
		CSt_atof (&dd,cp);
		nn = fabs (nn);
		dd = fabs (dd);
		if (dd <= 1.0E-14 || nn < 1.0E-14 || fabs (nn - dd) < 1.0E-10)
		{
			dblTmp = 1.0;
		}
		else if (dd > nn) dblTmp = 1.0 - (nn / dd);
		else			  dblTmp = 1.0 + (dd / nn);
		_stprintf (tcTemp,_T("%.8f"),dblTmp);
		wp->SetWindowText (tcTemp);
	}
	else
	{
		TCHAR* dummyPtr;
		dblTmp = _tcstod (tcTemp,&dummyPtr);
	}
	SetScaleRatio (dummy,IDC_CSGN_REDRATIO,dblTmp,true);
}
void CcsEdGnrl::OnKillfocusCsgnMapscl() 
{
	CWnd* wp;
	TCHAR *cp;
	double nn, dd, dblTmp;
	CString dummy;
	TCHAR tcTemp [32];

	wp = GetDlgItem (IDC_CSGN_MAPSCL);
	if (wp == NULL) return;
	wp->GetWindowText (tcTemp,tchrCount (tcTemp));
	cp = _tcschr (tcTemp,_T(':'));
	if (cp != NULL)
	{
		*cp++ = _T('\0');
		CSt_atof (&dd,tcTemp);
		CSt_atof (&nn,cp);
		if (fabs (dd) > 1.0E-14) dblTmp = fabs (nn / dd);
		else dblTmp = 1.0;
		_stprintf (tcTemp,_T("%10.0f"),dblTmp);
		wp->SetWindowText (tcTemp);
	}
	else
	{
		CSt_atof (&dblTmp,tcTemp);
	}
	SetScaleRatio (dummy,IDC_CSGN_MAPRATIO,dblTmp,false);
}
BOOL CcsEdGnrl::OnKillActive ()
{
	if (!UpdateData (TRUE)) return FALSE;
	m_PropSheet->FillInData ();
	return TRUE;
}
/////////////////////////////////////////////////////////////////////////////
// CcsEdOrigins property page
CcsEdOrigins::CcsEdOrigins() : CPropertyPage(CcsEdOrigins::IDD)
{
	//{{AFX_DATA_INIT(CcsEdOrigins)
	//}}AFX_DATA_INIT
	if (CS_isHlpAvailable ()) m_psp.dwFlags |=  PSP_HASHELP;
	m_NZeroX = 0.0;
	m_NZeroY = 0.0;
	m_OrgLat = 0.0;
	m_OrgLng = 0.0;
	m_XOff = 0.0;
	m_YOff = 0.0;
}
CcsEdOrigins::~CcsEdOrigins()
{
}
void CcsEdOrigins::DoDataExchange(CDataExchange* pDX)
{
	CWnd *wp;

	// The following are used to convey format information to the
	// DDX function which handles the appropriate item.

	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CcsEdOrigins)
	//}}AFX_DATA_MAP

	wp = GetDlgItem (IDC_EDCS_ORGLAT);
	if (wp != NULL) wp->EnableWindow ((m_PropSheet->m_PrjFlags & cs_PRJFLG_ORGLAT) == 0);
	wp = GetDlgItem (IDC_EDCS_ORGLNG);
	if (wp != NULL) wp->EnableWindow ((m_PropSheet->m_PrjFlags & cs_PRJFLG_ORGLNG) == 0);
	wp = GetDlgItem (IDC_EDCS_XOFF);
	if (wp != NULL) wp->EnableWindow ((m_PropSheet->m_PrjFlags & cs_PRJFLG_ORGFLS) == 0);
	wp = GetDlgItem (IDC_EDCS_YOFF);
	if (wp != NULL) wp->EnableWindow ((m_PropSheet->m_PrjFlags & cs_PRJFLG_ORGFLS) == 0);

	// Again, we do most of our own I/O.  The DDX_PrjParm function comes
	// in pretty handy.

	DDX_StaticLabelT (pDX, IDC_CSOR_SELDSCR,m_PropSheet->m_Description);
	DDX_PrjParm (pDX, IDC_EDCS_MINNZX,m_NZeroX,&m_PropSheet->m_ZroPrm);
	DDX_PrjParm (pDX, IDC_EDCS_MINNZY,m_NZeroY,&m_PropSheet->m_ZroPrm);
	DDX_PrjParm (pDX, IDC_EDCS_ORGLAT,m_OrgLat,&m_PropSheet->m_LatPrm);
	DDX_PrjParm (pDX, IDC_EDCS_ORGLNG,m_OrgLng,&m_PropSheet->m_LngPrm);
	DDX_PrjParm (pDX, IDC_EDCS_XOFF,m_XOff,&m_PropSheet->m_XPrm);
	DDX_PrjParm (pDX, IDC_EDCS_YOFF,m_YOff,&m_PropSheet->m_YPrm);
}

BEGIN_MESSAGE_MAP(CcsEdOrigins, CPropertyPage)
	//{{AFX_MSG_MAP(CcsEdOrigins)
	ON_EN_CHANGE(IDC_EDCS_MINNZX, OnChange)
	ON_EN_CHANGE(IDC_EDCS_MINNZY, OnChange)
	ON_EN_CHANGE(IDC_EDCS_ORGLAT, OnChange)
	ON_EN_CHANGE(IDC_EDCS_ORGLNG, OnChange)
	ON_EN_CHANGE(IDC_EDCS_XOFF, OnChange)
	ON_EN_CHANGE(IDC_EDCS_YOFF, OnChange)
	ON_BN_CLICKED(ID_HELP,OnHelpButton)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CcsEdOrigins message handlers
void CcsEdOrigins::OnChange() 
{
	m_PropSheet->OnChange ();
}
void CcsEdOrigins::OnHelpButton() 
{
	CSwinhlp (m_hWnd,csHLPID_CSED_ORGNS);
}
BOOL CcsEdOrigins::OnKillActive ()
{
	if (!UpdateData (TRUE)) return FALSE;
	m_PropSheet->FillInData ();
	return TRUE;
}
/////////////////////////////////////////////////////////////////////////////
// CcsEdExtents property page
CcsEdExtents::CcsEdExtents() : CPropertyPage(CcsEdExtents::IDD)
{
	//{{AFX_DATA_INIT(CcsEdExtents)
	//}}AFX_DATA_INIT
	if (CS_isHlpAvailable ()) m_psp.dwFlags |=  PSP_HASHELP;
	m_MinLng = 0.0;
	m_MinLat = 0.0;
	m_MaxLng = 0.0;
	m_MaxLat = 0.0;
	m_MinX = 0.0;
	m_MinY = 0.0;
	m_MaxX = 0.0;
	m_MaxY = 0.0;
}
CcsEdExtents::~CcsEdExtents()
{
}
void CcsEdExtents::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CcsEdExtents)
	//}}AFX_DATA_MAP
	DDX_StaticLabelT (pDX, IDC_EDCS_SELDSCR,m_PropSheet->m_Description);
	DDX_PrjParm (pDX, IDC_EDCS_MAXLAT,m_MaxLat,&m_PropSheet->m_LatPrm);
	DDX_PrjParm (pDX, IDC_EDCS_MAXLNG,m_MaxLng,&m_PropSheet->m_ExLngPrm);
	DDX_PrjParm (pDX, IDC_EDCS_MINLAT,m_MinLat,&m_PropSheet->m_LatPrm);
	DDX_PrjParm (pDX, IDC_EDCS_MINLNG,m_MinLng,&m_PropSheet->m_ExLngPrm);
	DDX_PrjParm (pDX, IDC_EDCS_MAXX,m_MaxX,&m_PropSheet->m_XPrm);
	DDX_PrjParm (pDX, IDC_EDCS_MAXY,m_MaxY,&m_PropSheet->m_YPrm);
	DDX_PrjParm (pDX, IDC_EDCS_MINX,m_MinX,&m_PropSheet->m_XPrm);
	DDX_PrjParm (pDX, IDC_EDCS_MINY,m_MinY,&m_PropSheet->m_YPrm);
}
BEGIN_MESSAGE_MAP(CcsEdExtents, CPropertyPage)
	//{{AFX_MSG_MAP(CcsEdExtents)
	ON_EN_CHANGE(IDC_EDCS_MAXLAT, OnChange)
	ON_EN_CHANGE(IDC_EDCS_MAXLNG, OnChange)
	ON_EN_CHANGE(IDC_EDCS_MAXX, OnChange)
	ON_EN_CHANGE(IDC_EDCS_MAXY, OnChange)
	ON_EN_CHANGE(IDC_EDCS_MINLAT, OnChange)
	ON_EN_CHANGE(IDC_EDCS_MINLNG, OnChange)
	ON_EN_CHANGE(IDC_EDCS_MINX, OnChange)
	ON_EN_CHANGE(IDC_EDCS_MINY, OnChange)
	ON_BN_CLICKED(ID_HELP,OnHelpButton)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
/////////////////////////////////////////////////////////////////////////////
// CcsEdExtents message handlers
void CcsEdExtents::OnChange() 
{
	m_PropSheet->OnChange ();	
}
void CcsEdExtents::OnHelpButton() 
{
	CSwinhlp (m_hWnd,csHLPID_CSED_EXTNT);
}
BOOL CcsEdExtents::OnKillActive ()
{
	if (!UpdateData (TRUE)) return FALSE;
	m_PropSheet->FillInData ();
	return TRUE;
}
/////////////////////////////////////////////////////////////////////////////
// CcsEdParms1 property page
CcsEdParms1::CcsEdParms1() : CPropertyPage(CcsEdParms1::IDD)
{
	//{{AFX_DATA_INIT(CcsEdParms1)
	//}}AFX_DATA_INIT
	if (CS_isHlpAvailable ()) m_psp.dwFlags |=  PSP_HASHELP;
	m_PrjParm1 = 0.0;
	m_PrjParm2 = 0.0;
	m_PrjParm3 = 0.0;
	m_PrjParm4 = 0.0;
	m_PrjParm5 = 0.0;
	m_PrjParm6 = 0.0;
}
BOOL CcsEdParms1::OnSetActive ()
{
	// Here to enable/disable parameters as appropriate.  Note,
	// the projection change change while thie screen is active.

	int ii, shwCmd;
	CWnd *wp;

	for (ii = 1;ii <= 6;ii += 1)
	{
		shwCmd = (ii <= m_PropSheet->m_ParmCount) ? SW_SHOW : SW_HIDE;
		wp = GetDlgItem (csEdParmMap [ii - 1].lblID);
		if (wp != NULL) wp->ShowWindow (shwCmd);
		wp = GetDlgItem (csEdParmMap [ii - 1].valID);
		if (wp != NULL) wp->ShowWindow (shwCmd);
	}

	return CPropertyPage::OnSetActive ();
}

CcsEdParms1::~CcsEdParms1()
{
}

void CcsEdParms1::DoDataExchange(CDataExchange* pDX)
{
	struct cs_Prjprm_ *prmPtr;

	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CcsEdParms1)
	//}}AFX_DATA_MAP

	DDX_StaticLabelT (pDX,IDC_CSPR_SELDSCR,m_PropSheet->m_Description);

	prmPtr = &m_PropSheet->m_PrjParms [0];
	DDX_StaticLabel (pDX,IDC_EDCS_PP_LBL1,prmPtr->label);
	DDX_PrjParm (pDX,IDC_EDCS_PRJ_PRM1,m_PrjParm1,prmPtr);

	prmPtr = &m_PropSheet->m_PrjParms [1];
	DDX_StaticLabel (pDX,IDC_EDCS_PP_LBL2,prmPtr->label);
	DDX_PrjParm (pDX,IDC_EDCS_PRJ_PRM2,m_PrjParm2,prmPtr);

	prmPtr = &m_PropSheet->m_PrjParms [2];
	DDX_StaticLabel (pDX,IDC_EDCS_PP_LBL3,prmPtr->label);
	DDX_PrjParm (pDX,IDC_EDCS_PRJ_PRM3,m_PrjParm3,prmPtr);

	prmPtr = &m_PropSheet->m_PrjParms [3];
	DDX_StaticLabel (pDX,IDC_EDCS_PP_LBL4,prmPtr->label);
	DDX_PrjParm (pDX,IDC_EDCS_PRJ_PRM4,m_PrjParm4,prmPtr);

	prmPtr = &m_PropSheet->m_PrjParms [4];
	DDX_StaticLabel (pDX,IDC_EDCS_PP_LBL5,prmPtr->label);
	DDX_PrjParm (pDX,IDC_EDCS_PRJ_PRM5,m_PrjParm5,prmPtr);

	prmPtr = &m_PropSheet->m_PrjParms [5];
	DDX_StaticLabel (pDX,IDC_EDCS_PP_LBL6,prmPtr->label);
	DDX_PrjParm (pDX,IDC_EDCS_PRJ_PRM6,m_PrjParm6,prmPtr);
}

BEGIN_MESSAGE_MAP(CcsEdParms1, CPropertyPage)
	//{{AFX_MSG_MAP(CcsEdParms1)
	ON_EN_CHANGE(IDC_EDCS_PRJ_PRM1, OnChange)
	ON_EN_CHANGE(IDC_EDCS_PRJ_PRM2, OnChange)
	ON_EN_CHANGE(IDC_EDCS_PRJ_PRM3, OnChange)
	ON_EN_CHANGE(IDC_EDCS_PRJ_PRM4, OnChange)
	ON_EN_CHANGE(IDC_EDCS_PRJ_PRM5, OnChange)
	ON_EN_CHANGE(IDC_EDCS_PRJ_PRM6, OnChange)
	ON_BN_CLICKED(ID_HELP,OnHelpButton)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CcsEdParms1 message handlers
void CcsEdParms1::OnChange() 
{
	m_PropSheet->OnChange ();
}
void CcsEdParms1::OnHelpButton() 
{
	CSwinhlp (m_hWnd,csHLPID_CSED_PARMS);
}
BOOL CcsEdParms1::OnKillActive ()
{
	if (!UpdateData (TRUE)) return FALSE;
	m_PropSheet->FillInData ();
	return TRUE;
}
/////////////////////////////////////////////////////////////////////////////
// CcsParms2 property page
CcsEdParms2::CcsEdParms2() : CPropertyPage(CcsEdParms2::IDD)
{
	//{{AFX_DATA_INIT(CcsEdParms2)
	//}}AFX_DATA_INIT
	if (CS_isHlpAvailable ()) m_psp.dwFlags |=  PSP_HASHELP;
	m_PrjParm10 = 0.0;
	m_PrjParm11 = 0.0;
	m_PrjParm12 = 0.0;
	m_PrjParm7 = 0.0;
	m_PrjParm9 = 0.0;
	m_PrjParm8 = 0.0;
}
BOOL CcsEdParms2::OnSetActive ()
{
	// Here to enable/disable parameters as appropriate.  Note,
	// the projection change change while thie screen is active.

	int ii, shwCmd;
	CWnd *wp;

	for (ii = 7;ii <= 12;ii += 1)
	{
		shwCmd = (ii <= m_PropSheet->m_ParmCount) ? SW_SHOW : SW_HIDE;
		wp = GetDlgItem (csEdParmMap [ii - 1].lblID);
		if (wp != NULL) wp->ShowWindow (shwCmd);
		wp = GetDlgItem (csEdParmMap [ii - 1].valID);
		if (wp != NULL) wp->ShowWindow (shwCmd);
	}

	return CPropertyPage::OnSetActive ();
}

CcsEdParms2::~CcsEdParms2()
{
}

void CcsEdParms2::DoDataExchange(CDataExchange* pDX)
{
	struct cs_Prjprm_ *prmPtr;

	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CcsEdParms2)
	//}}AFX_DATA_MAP

	DDX_StaticLabelT (pDX,IDC_CSPR_SELDSCR,m_PropSheet->m_Description);

	prmPtr = &m_PropSheet->m_PrjParms [6];
	DDX_StaticLabel (pDX,IDC_EDCS_PP_LBL7,prmPtr->label);
	DDX_PrjParm (pDX,IDC_EDCS_PRJ_PRM7,m_PrjParm7,prmPtr);

	prmPtr = &m_PropSheet->m_PrjParms [7];
	DDX_StaticLabel (pDX,IDC_EDCS_PP_LBL8,prmPtr->label);
	DDX_PrjParm (pDX,IDC_EDCS_PRJ_PRM8,m_PrjParm8,prmPtr);

	prmPtr = &m_PropSheet->m_PrjParms [8];
	DDX_StaticLabel (pDX,IDC_EDCS_PP_LBL9,prmPtr->label);
	DDX_PrjParm (pDX,IDC_EDCS_PRJ_PRM9,m_PrjParm9,prmPtr);

	prmPtr = &m_PropSheet->m_PrjParms [9];
	DDX_StaticLabel (pDX,IDC_EDCS_PP_LBL10,prmPtr->label);
	DDX_PrjParm (pDX,IDC_EDCS_PRJ_PRM10,m_PrjParm10,prmPtr);

	prmPtr = &m_PropSheet->m_PrjParms [10];
	DDX_StaticLabel (pDX,IDC_EDCS_PP_LBL11,prmPtr->label);
	DDX_PrjParm (pDX,IDC_EDCS_PRJ_PRM11,m_PrjParm11,prmPtr);

	prmPtr = &m_PropSheet->m_PrjParms [11];
	DDX_StaticLabel (pDX,IDC_EDCS_PP_LBL12,prmPtr->label);
	DDX_PrjParm (pDX,IDC_EDCS_PRJ_PRM12,m_PrjParm12,prmPtr);
}

BEGIN_MESSAGE_MAP(CcsEdParms2, CPropertyPage)
	//{{AFX_MSG_MAP(CcsEdParms2)
	ON_EN_CHANGE(IDC_EDCS_PRJ_PRM10, OnChange)
	ON_EN_CHANGE(IDC_EDCS_PRJ_PRM11, OnChange)
	ON_EN_CHANGE(IDC_EDCS_PRJ_PRM12, OnChange)
	ON_EN_CHANGE(IDC_EDCS_PRJ_PRM7, OnChange)
	ON_EN_CHANGE(IDC_EDCS_PRJ_PRM8, OnChange)
	ON_EN_CHANGE(IDC_EDCS_PRJ_PRM9, OnChange)
	ON_BN_CLICKED(ID_HELP,OnHelpButton)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CcsParms2 message handlers
void CcsEdParms2::OnChange() 
{
	m_PropSheet->OnChange ();
}
void CcsEdParms2::OnHelpButton() 
{
	CSwinhlp (m_hWnd,csHLPID_CSED_PARMS);
}
BOOL CcsEdParms2::OnKillActive ()
{
	if (!UpdateData (TRUE)) return FALSE;
	m_PropSheet->FillInData ();
	return TRUE;
}
/////////////////////////////////////////////////////////////////////////////
// CcsParms3 property page
CcsEdParms3::CcsEdParms3() : CPropertyPage(CcsEdParms3::IDD)
{
	//{{AFX_DATA_INIT(CcsEdParms3)
	//}}AFX_DATA_INIT
	if (CS_isHlpAvailable ()) m_psp.dwFlags |=  PSP_HASHELP;
	m_PrjParm13 = 0.0;
	m_PrjParm14 = 0.0;
	m_PrjParm15 = 0.0;
	m_PrjParm16 = 0.0;
	m_PrjParm17 = 0.0;
	m_PrjParm18 = 0.0;
}
BOOL CcsEdParms3::OnSetActive ()
{
	// Here to enable/disable parameters as appropriate.  Note,
	// the projection change change while thie screen is active.

	int ii, shwCmd;
	CWnd *wp;

	for (ii = 13;ii <= 18;ii += 1)
	{
		shwCmd = (ii <= m_PropSheet->m_ParmCount) ? SW_SHOW : SW_HIDE;
		wp = GetDlgItem (csEdParmMap [ii - 1].lblID);
		if (wp != NULL) wp->ShowWindow (shwCmd);
		wp = GetDlgItem (csEdParmMap [ii - 1].valID);
		if (wp != NULL) wp->ShowWindow (shwCmd);
	}
	return CPropertyPage::OnSetActive ();
}

CcsEdParms3::~CcsEdParms3()
{
}

void CcsEdParms3::DoDataExchange(CDataExchange* pDX)
{
	struct cs_Prjprm_ *prmPtr;

	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CcsEdParms3)
	//}}AFX_DATA_MAP

	DDX_StaticLabelT (pDX,IDC_CSPR_SELDSCR,m_PropSheet->m_Description);

	prmPtr = &m_PropSheet->m_PrjParms [12];
	DDX_StaticLabel (pDX,IDC_EDCS_PP_LBL13,prmPtr->label);
	DDX_PrjParm (pDX,IDC_EDCS_PRJ_PRM13,m_PrjParm13,prmPtr);

	prmPtr = &m_PropSheet->m_PrjParms [13];
	DDX_StaticLabel (pDX,IDC_EDCS_PP_LBL14,prmPtr->label);
	DDX_PrjParm (pDX,IDC_EDCS_PRJ_PRM14,m_PrjParm14,prmPtr);

	prmPtr = &m_PropSheet->m_PrjParms [14];
	DDX_StaticLabel (pDX,IDC_EDCS_PP_LBL15,prmPtr->label);
	DDX_PrjParm (pDX,IDC_EDCS_PRJ_PRM15,m_PrjParm15,prmPtr);

	prmPtr = &m_PropSheet->m_PrjParms [15];
	DDX_StaticLabel (pDX,IDC_EDCS_PP_LBL16,prmPtr->label);
	DDX_PrjParm (pDX,IDC_EDCS_PRJ_PRM16,m_PrjParm16,prmPtr);

	prmPtr = &m_PropSheet->m_PrjParms [16];
	DDX_StaticLabel (pDX,IDC_EDCS_PP_LBL17,prmPtr->label);
	DDX_PrjParm (pDX,IDC_EDCS_PRJ_PRM17,m_PrjParm17,prmPtr);

	prmPtr = &m_PropSheet->m_PrjParms [17];
	DDX_StaticLabel (pDX,IDC_EDCS_PP_LBL18,prmPtr->label);
	DDX_PrjParm (pDX,IDC_EDCS_PRJ_PRM18,m_PrjParm18,prmPtr);
}

BEGIN_MESSAGE_MAP(CcsEdParms3, CPropertyPage)
	//{{AFX_MSG_MAP(CcsEdParms3)
	ON_EN_CHANGE(IDC_EDCS_PRJ_PRM13, OnChange)
	ON_EN_CHANGE(IDC_EDCS_PRJ_PRM14, OnChange)
	ON_EN_CHANGE(IDC_EDCS_PRJ_PRM15, OnChange)
	ON_EN_CHANGE(IDC_EDCS_PRJ_PRM16, OnChange)
	ON_EN_CHANGE(IDC_EDCS_PRJ_PRM17, OnChange)
	ON_EN_CHANGE(IDC_EDCS_PRJ_PRM18, OnChange)
	ON_BN_CLICKED(ID_HELP,OnHelpButton)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CcsParms3 message handlers
void CcsEdParms3::OnChange() 
{
	m_PropSheet->OnChange ();
}
void CcsEdParms3::OnHelpButton() 
{
	CSwinhlp (m_hWnd,csHLPID_CSED_PARMS);
}
BOOL CcsEdParms3::OnKillActive ()
{
	if (!UpdateData (TRUE)) return FALSE;
	m_PropSheet->FillInData ();
	return TRUE;
}
/////////////////////////////////////////////////////////////////////////////
// CcsParms4 property page
CcsEdParms4::CcsEdParms4() : CPropertyPage(CcsEdParms4::IDD)
{
	// No data memebers, nothing to initialize.
	//{{AFX_DATA_INIT(CcsEdParms4)
	//}}AFX_DATA_INIT
	if (CS_isHlpAvailable ()) m_psp.dwFlags |=  PSP_HASHELP;
	m_PrjParm19 = 0.0;
	m_PrjParm20 = 0.0;
	m_PrjParm21 = 0.0;
	m_PrjParm22 = 0.0;
	m_PrjParm23 = 0.0;
	m_PrjParm24 = 0.0;
}
CcsEdParms4::~CcsEdParms4()
{
}
BOOL CcsEdParms4::OnSetActive ()
{
	// Here to enable/disable parameters as appropriate.  Note,
	// the projection change change while thie screen is active.

	int ii, shwCmd;
	CWnd *wp;

	for (ii = 19;ii <= 24;ii += 1)
	{
		shwCmd = (ii <= m_PropSheet->m_ParmCount) ? SW_SHOW : SW_HIDE;
		wp = GetDlgItem (csEdParmMap [ii - 1].lblID);
		if (wp != NULL) wp->ShowWindow (shwCmd);
		wp = GetDlgItem (csEdParmMap [ii - 1].valID);
		if (wp != NULL) wp->ShowWindow (shwCmd);
	}

	return CPropertyPage::OnSetActive ();
}
	
void CcsEdParms4::DoDataExchange(CDataExchange* pDX)
{
	struct cs_Prjprm_ *prmPtr;

	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CcsEdParms4)
	//}}AFX_DATA_MAP

	DDX_StaticLabelT (pDX,IDC_CSPR_SELDSCR,m_PropSheet->m_Description);

	prmPtr = &m_PropSheet->m_PrjParms [18];
	DDX_StaticLabel (pDX,IDC_EDCS_PP_LBL19,prmPtr->label);
	DDX_PrjParm (pDX,IDC_EDCS_PRJ_PRM19,m_PrjParm19,prmPtr);

	prmPtr = &m_PropSheet->m_PrjParms [19];
	DDX_StaticLabel (pDX,IDC_EDCS_PP_LBL20,prmPtr->label);
	DDX_PrjParm (pDX,IDC_EDCS_PRJ_PRM20,m_PrjParm20,prmPtr);

	prmPtr = &m_PropSheet->m_PrjParms [20];
	DDX_StaticLabel (pDX,IDC_EDCS_PP_LBL21,prmPtr->label);
	DDX_PrjParm (pDX,IDC_EDCS_PRJ_PRM21,m_PrjParm21,prmPtr);

	prmPtr = &m_PropSheet->m_PrjParms [21];
	DDX_StaticLabel (pDX,IDC_EDCS_PP_LBL22,prmPtr->label);
	DDX_PrjParm (pDX,IDC_EDCS_PRJ_PRM22,m_PrjParm22,prmPtr);

	prmPtr = &m_PropSheet->m_PrjParms [22];
	DDX_StaticLabel (pDX,IDC_EDCS_PP_LBL23,prmPtr->label);
	DDX_PrjParm (pDX,IDC_EDCS_PRJ_PRM23,m_PrjParm23,prmPtr);

	prmPtr = &m_PropSheet->m_PrjParms [23];
	DDX_StaticLabel (pDX,IDC_EDCS_PP_LBL24,prmPtr->label);
	DDX_PrjParm (pDX,IDC_EDCS_PRJ_PRM24,m_PrjParm24,prmPtr);
}
BEGIN_MESSAGE_MAP(CcsEdParms4, CPropertyPage)
	//{{AFX_MSG_MAP(CcsEdParms4)
	ON_EN_CHANGE(IDC_EDCS_PRJ_PRM19, OnChange)
	ON_EN_CHANGE(IDC_EDCS_PRJ_PRM20, OnChange)
	ON_EN_CHANGE(IDC_EDCS_PRJ_PRM21, OnChange)
	ON_EN_CHANGE(IDC_EDCS_PRJ_PRM22, OnChange)
	ON_EN_CHANGE(IDC_EDCS_PRJ_PRM23, OnChange)
	ON_EN_CHANGE(IDC_EDCS_PRJ_PRM24, OnChange)
	ON_BN_CLICKED(ID_HELP,OnHelpButton)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
/////////////////////////////////////////////////////////////////////////////
// CcsEdParms4 message handlers
void CcsEdParms4::OnChange() 
{
	// User changed, or is attempting to change, one of the parameters.
	m_PropSheet->OnChange ();
}
void CcsEdParms4::OnHelpButton() 
{
	CSwinhlp (m_hWnd,csHLPID_CSED_PARMS);
}
BOOL CcsEdParms4::OnKillActive ()
{
	if (!UpdateData (TRUE)) return FALSE;
	m_PropSheet->FillInData ();
	return TRUE;
}
/////////////////////////////////////////////////////////////////////////////
//	Custom DDX and DDV routines used in this property sheet.  These cannot be
//	incorporated into the Class Wizard scheme of things as they take special
//	parameters; special parameters are not supported by the Class Wizard.
//
//	DDX_PrjParm is a general DDX function which performs the same function
//	as all DDX functions.  However, it takes an additional parameter, a
//	pointer to a cs_Prjprm_ structure which defines the type and formatting
//	for this value.
void AFXAPI DDX_PrjParm (CDataExchange* pDX,int nIDC,double& value,struct cs_Prjprm_* prmType)
{
	TCHAR nullStr [2];

	nullStr [0] = _T('\0');
	switch (prmType->log_type) {

	default:
	case cs_PRMLTYP_NONE:
		if (pDX->m_bSaveAndValidate) value = 0.0;
		else ::SetWindowText(pDX->PrepareEditCtrl(nIDC),nullStr);
		break;
	case cs_PRMLTYP_LNG:
	case cs_PRMLTYP_LAT:
	case cs_PRMLTYP_AZM:
	case cs_PRMLTYP_ANGD:
	case cs_PRMLTYP_CMPLXC:
	case cs_PRMLTYP_GHGT:
	case cs_PRMLTYP_ELEV:
	case cs_PRMLTYP_AFCOEF:
	case cs_PRMLTYP_XYCRD:
	case cs_PRMLTYP_SCALE:
		// These are all normal numeric values.
		DDX_Number (pDX,nIDC,value,prmType);
		break;
	case cs_PRMLTYP_ZNBR:
		// This is the special UTM zone number, must be 1 - 60.
		DDX_ZoneNbr (pDX,nIDC,value);
		break;
	case cs_PRMLTYP_HSNS:
		// This is the hemisphere guy, translates -1.0 to South and
		// +1.0 to North, and vice versa.
		DDX_HemiNS (pDX,nIDC,value);
		break;
	}
	return;
}
//	This function handles the UTM Zone number.
void AFXAPI DDX_ZoneNbr (CDataExchange* pDX,int nIDC,double& value)
{
	int zone;
	TCHAR buffer [34];		// _ltoa below assumes at least 34

	HWND hWndCtrl = pDX->PrepareEditCtrl(nIDC);
	if (pDX->m_bSaveAndValidate)
	{
		// Fetch from Dialog Box.
		::GetWindowText(hWndCtrl,buffer,tchrCount (buffer));
		zone = _tstoi (buffer);
		if (zone < 1 || zone > 60)
		{
			AfxMessageBox (_T("UTM Zone must be an integer between 1 and 60."));
			pDX->Fail ();
		}
		value = (double)zone;
	}
	else
	{
		// Display to Dialog Box
		_ltot ((long32_t)(value),buffer,10);
		::SetWindowText (hWndCtrl,buffer);
	}
}
void AFXAPI DDX_HemiNS (CDataExchange* pDX,int nIDC,double& value)
{
	unsigned len;
	int hemisphere;
	TCHAR buffer [34];

	HWND hWndCtrl = pDX->PrepareEditCtrl(nIDC);
	if (pDX->m_bSaveAndValidate)
	{
		// Fetch from dialog box.
		::GetWindowText(hWndCtrl,buffer,tchrCount (buffer));
		hemisphere = 0;
		// Try to do a smart compare.
		len = static_cast<unsigned>(_tcslen (buffer));
		if (!CSt_strnicmp (buffer,_T("NORTH"),len)) hemisphere = 1;
		if (!CSt_strnicmp (buffer,_T("SOUTH"),len)) hemisphere = -1;
		if (hemisphere == 0)
		{
			AfxMessageBox (_T("Hemisphere must be \"North\" or \"South\" (N or S will also work)."));
			pDX->Fail ();
		}
		value = (double)hemisphere;
	}
	else
	{
		CSt_strNCpy (buffer,((int)(value)) >= 0 ? "North" : "South",tchrCount (buffer));
		::SetWindowText(hWndCtrl,buffer);
	}
}
//	This function handles parameters which are numbers.  The format
//	of a display is indicated by the format parameter.  DDX functions
//	normally return "void".  In this case, this function returns the
//	formatting encountered when the user input is interpreted.  This
//	just makes this function a bit more useful; although at this
//	writting, the return value is not used. Can be removed if the
//	compiler starts to complain.
long32_t AFXAPI DDX_Number (CDataExchange* pDX,int nIDC,double& value,struct cs_Prjprm_ *prmType)
{
	bool errorFlag;
	long32_t st;
	double tmpDbl;
	TCHAR buffer [48];

	errorFlag = false;
	HWND hWndCtrl = pDX->PrepareEditCtrl(nIDC);
	if (pDX->m_bSaveAndValidate)
	{
		// Fetch a value from the dialog box.  CS_atof will eat just
		// about anything.
		::GetWindowText(hWndCtrl,buffer,tchrCount (buffer));
		st = CSt_atof (&tmpDbl,buffer);
		if (st < 0L)
		{
			TCHAR errMesg [256];
			// There was a formatting problem.  Decipher the cause and
			// issue the best error message we can.
			errorFlag = TRUE;
			CSt_errmsg (errMesg,tchrCount (errMesg));
			AfxMessageBox (errMesg);
		}
		else
		{
			if (tmpDbl > prmType->max_val)  
			{
				errorFlag = true;
				AfxMessageBox (_T("Value supplied exceeds the maximum value expected for\n")
							   _T("this parameter."));
			}
			else if (tmpDbl < prmType->min_val)
			{
				errorFlag = true;
				AfxMessageBox (_T("Value supplied is less than the minimum value expected for\n")
							   _T("this parameter."));
			}
		}
		if (errorFlag)
		{
			pDX->Fail ();
		}
		// If we're still here, we have a valid value.
		value = tmpDbl;
	}
	else
	{
		// Display the value in the dialog box.  Format to be used is in
		// the prmType structure.  Note, formatting errors will show up
		// in the edit control, usually as a "***.**".
		st = CSt_ftoa (buffer,tchrCount (buffer),value,prmType->format);
		::SetWindowText(hWndCtrl,buffer);
	}
	return (st);
}
//	This function handles scale values.  Supports entering ratios.
void AFXAPI DDX_Scale (CDataExchange* pDX,int nIDC,double& value,bool reduction)
{
	bool errorFlag;
	long32_t st;
	TCHAR *cp;
	TCHAR *errMsg;
	double dblTmp;
	TCHAR buffer [48];

	errorFlag = false;
	errMsg = NULL;
	HWND hWndCtrl = pDX->PrepareEditCtrl(nIDC);
	if (pDX->m_bSaveAndValidate)
	{
		// Fetch a value from the dialog box.
		::GetWindowText(hWndCtrl,buffer,tchrCount (buffer));
		// See if it has a colon in it.  If so, we treat as a ratio.  If
		// not, we handle it as an ordinary number.  We use CS_atof as
		// it handles commas.
		cp = _tcsrchr (buffer,_T(':'));
		if (cp == NULL)
		{
			st = CSt_atof (&dblTmp,buffer);
		}
		else
		{
			double nn, dd;
			*cp++ = _T('\0');
			st = CSt_atof (&nn,buffer);
			if (st >= 0)
			{
				st = CSt_atof (&dd,cp);
				if (st >= 0)
				{
					if (dd < 1.0 || nn < 1.0 || nn > dd)
					{
						errMsg = _T("Invalid ratio input format. Correct, or use pure decimal form.");
						st = -1;
					}
					else
					{
						if (fabs (nn - dd) < 1.0E-10) dblTmp = 1.0;
						else dblTmp = nn / dd;
						if (reduction && dblTmp != 1.0) dblTmp = 1.0 - dblTmp;
					}
				}
			}
		}
		if (st < 0L)
		{
			// There was a formatting problem.  Decipher the cause and
			// issue the best error message we can.
			errorFlag = TRUE;
			if (errMsg == NULL)
			{
				TCHAR errMesg [256];
				CSt_errmsg (errMesg,tchrCount (errMesg));
				AfxMessageBox (errMesg);
			}
			else AfxMessageBox (errMsg);
		}
		if (errorFlag)
		{
			pDX->Fail ();
		}
		// If we're still here, we have a valid value.
		value = dblTmp;
	}
	else
	{
		_stprintf (buffer,_T("%12.9f"),value);
		::SetWindowText(hWndCtrl,buffer);
	}
	return;
}
//	Very efficiently write static screen labels.  We have a lot of
//	dynamic static labels in this dialog.
void AFXAPI DDX_StaticLabelT (CDataExchange* pDX,int nIDC,const TCHAR* data)
{
	HWND hWndCtrl = pDX->PrepareEditCtrl(nIDC);
	if (!pDX->m_bSaveAndValidate)
	{
		::SetWindowText (hWndCtrl,data);
	}
}
void AFXAPI DDX_StaticLabel (CDataExchange* pDX,int nIDC,const char* data)
{
	TCHAR tcTemp [128];
	CSt_strNCpy (tcTemp,data,tchrCount (tcTemp));
	HWND hWndCtrl = pDX->PrepareEditCtrl(nIDC);
	if (!pDX->m_bSaveAndValidate)
	{
		::SetWindowText (hWndCtrl,tcTemp);
	}
}
//	We use the following for character arrays.  Using this function, we can
//	go to and from the source of the data directly, thus eliminating much
//	code.
void AFXAPI DDX_CharArray (CDataExchange* pDX,int nIDC,TCHAR* data,int count)
{
	TCHAR buffer [256];

	HWND hWndCtrl = pDX->PrepareEditCtrl(nIDC);
	if (pDX->m_bSaveAndValidate)
	{
		::GetWindowText(hWndCtrl,buffer,tchrCount (buffer));
		if (_tcslen (buffer) >= (unsigned)count)
		{
			pDX->Fail ();
		}
		// If we're still here, the data is OK.
		_tcscpy (data,buffer);
	}
	else
	{
		// This is easy.
		::SetWindowText(hWndCtrl,data);
	}
}
// Projection Table Look Up
struct cs_Prjtab_* PrjLookUp (const char* prjKeyName)
{
	struct cs_Prjtab_ *prjPtr;
	for (prjPtr = cs_Prjtab;prjPtr->code != cs_PRJCOD_END;prjPtr += 1)
	{
		if (!CS_stricmp (prjKeyName,prjPtr->key_nm)) break;
	}
	if (prjPtr->code == cs_PRJCOD_END) prjPtr = NULL;
	return (prjPtr);
}
#endif
