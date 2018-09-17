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

/**********************************************************************
**	CcsDualBrowser csDualBrowser;
**		csDualBrowser.SetSrcKeyName (srcName)
**		csDualBrowser.SetTrgKeyName (trgName);
**		ok = elEdit.DoModal ();
**		newSrcName = elEdit.GetSrcKeyName ();
**		newTrgName = elEdit.GetTrgKeyName ();
**
**	extern "C" int CS_csDualBrowser (srcKeyName,trgKeyName);
**
**	char* srcName;				key name of the source coordinate system
**								which is to be displayed upon dialog
**								activation.
**	char* trgName;				key name of the target coordinate system
**								which is to be displayed upon dialog
**								activation.
**	int ok;						returns IDOK if user exited with OK button, else
**								returns IDCANCEL.
**	const char* newSrcName;		pointer to static memory containing the
**								key name of the selected source coordinate
**								system definition.
**	const char* newTrgName;		pointer to static memory containing the
**								key name of the selected target coordinate
**								system definition.
**	char srcKeyName [cs_KEYNM_DEF];
**								array in which the initial source key name
**								is obtained and the newly selected source
**								key name is returned.
**	char trgKeyName [cs_KEYNM_DEF];
**								array in which the initial target key name
**								is obtained and the newly selected target
**								key name is returned.
**
**	Note, values obtained from GetSrcKeyName and GetTrgKeyName become
**	invalid when the CcsDualBrowser object is destroyed.
**********************************************************************/

///////////////////////////////////////////////////////////////////////////////
// Basic API access function.
int EXP_LVL1 CS_csDualBrowser (char *srcKeyName,char *trgKeyName)
{
	INT_PTR st;
	TCHAR tcTemp [cs_KEYNM_DEF];
	CcsDualBrowse csDualBrowse;

	// First we verify that the CS-MAP stuff is up and running in a
	// normal fashion by checking the validity of the the 'LL84'
	// coordinate system.
	if (CS_csIsValid ("LL84") < 0) return -1; 

	// OK, we proceed assuming that all normal CS-MAP has been initialized
	// and has found all of its various pieces.
	CSt_strNCpy (tcTemp,srcKeyName,cs_KEYNM_DEF);
	csDualBrowse.SetSrcKeyName (tcTemp);
	CSt_strNCpy (tcTemp,trgKeyName,cs_KEYNM_DEF);
	csDualBrowse.SetTrgKeyName (tcTemp);
	st = csDualBrowse.DoModal ();

	// A return value of 1 indicates that the user canceled, no
	// changes were made.
	if (st != IDOK) return 1;

	// Return value of zero indicates that the dialog exited with
	// OK, and selections MAY have changed.
	CSt_strNCpy (srcKeyName,csDualBrowse.GetSrcKeyName (),cs_KEYNM_DEF);
	CSt_strNCpy (trgKeyName,csDualBrowse.GetTrgKeyName (),cs_KEYNM_DEF);
	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// CcsDualBrowse dialog

CcsDualBrowse::CcsDualBrowse(CWnd* pParent /*=NULL*/)
			: CDialog(CcsDualBrowse::IDD, pParent)
{
	//{{AFX_DATA_INIT(CcsDualBrowse)
	m_SrcDescription = _T("");
	m_SrcReference = _T("");
	m_SrcRefLabel = _T("");
	m_SrcSource = _T("");
	m_SrcUnit = _T("");
	m_TrgDescription = _T("");
	m_TrgReference = _T("");
	m_TrgRefLabel = _T("");
	m_TrgSource = _T("");
	m_TrgUnit = _T("");
	//}}AFX_DATA_INIT

	m_Source = _T("US48");
	m_Target = _T("LL27");
}
void CcsDualBrowse::SetSrcKeyName (const TCHAR* srcKeyName)
{
	// We don't change to an invalid name.
	if (CSt_csIsValid (srcKeyName)) m_Source = srcKeyName;
}
void CcsDualBrowse::SetTrgKeyName (const TCHAR* trgKeyName)
{
	// We don't change to an invalid name.
	if (CSt_csIsValid (trgKeyName)) m_Target = trgKeyName;
}
BOOL CcsDualBrowse::OnInitDialog ()
{
	int st;
	int ii, idx;
	struct cs_Csdef_ *csPtr;
	TCHAR grpName [24], grpDescr [64];
	TCHAR srcGrpName [24], srcGrpDescr [64];
	TCHAR trgGrpName [24], trgGrpDescr [64];

	CDialog::OnInitDialog ();

	// We need to extract the group names from the coordinate system
	// definitions.
	csPtr = CSt_csdef ((LPCTSTR)m_Source);
	if (csPtr == NULL)
	{
		// Should never happen.
		TCHAR errmsg [256];
		CSt_errmsg (errmsg,tchrCount (errmsg));
		AfxMessageBox (errmsg);
		return TRUE;
	}
	CSt_strNCpy (srcGrpName,csPtr->group,tchrCount (srcGrpName));
	CS_free (csPtr);
	
	csPtr = CSt_csdef ((LPCTSTR)m_Target);
	if (csPtr == NULL)
	{
		// Should never happen.
		TCHAR errmsg [256];
		CSt_errmsg (errmsg,tchrCount (errmsg));
		AfxMessageBox (errmsg);
		return TRUE;
	}
	CSt_strNCpy (trgGrpName,csPtr->group,tchrCount (trgGrpName));
	CS_free (csPtr);

	// Set up the group combo boxes, making the proper current selections.
	srcGrpDescr [0] = trgGrpDescr [0] = _T('\0');
	m_SrcGroup.ResetContent ();
	m_TrgGroup.ResetContent ();
	for (ii = 0;CSt_csGrpEnum (ii,grpName,tchrCount (grpName),grpDescr,tchrCount (grpDescr)) > 0;ii += 1)
	{
		idx = m_SrcGroup.AddString (grpDescr);
		m_SrcGroup.SetItemData (idx,ii);
		if (!CSt_stricmp (grpName,srcGrpName)) CSt_strNCpy (srcGrpDescr,grpDescr,tchrCount (srcGrpDescr));

		idx = m_TrgGroup.AddString (grpDescr);
		m_TrgGroup.SetItemData (idx,ii);
		if (!CSt_stricmp (grpName,trgGrpName)) CSt_strNCpy (trgGrpDescr,grpDescr,tchrCount (trgGrpDescr));
	}
	if (ii > 0)
	{
		m_SrcGroup.SelectString (-1,srcGrpDescr);
		m_TrgGroup.SelectString (-1,trgGrpDescr);
	}

	// Now, the Source Key Name combo box
	RebuildKeyNameCombo (m_SrcKeyName,srcGrpName);
	st = m_SrcKeyName.SelectString (-1,m_Source);
	if (st == CB_ERR) m_SrcKeyName.SetCurSel (0);
	OnChangeSrcKeyNm();

	// Now, the Source Key Name combo box
	RebuildKeyNameCombo (m_TrgKeyName,trgGrpName);
	st = m_TrgKeyName.SelectString (-1,m_Target);
	if (st == CB_ERR) m_TrgKeyName.SetCurSel (0);
	OnChangeTrgKeyNm();

	// Disable the Help button if help is not available.
	CWnd* wp = GetDlgItem (ID_BRWS_HELP);
	if (wp != NULL) wp->EnableWindow (CS_isHlpAvailable ());
	return TRUE;
}
void CcsDualBrowse::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CcsDualBrowse)
	DDX_Control(pDX, IDC_CSBR2_TRGKEYNM, m_TrgKeyName);
	DDX_Control(pDX, IDC_CSBR2_TRGGRP, m_TrgGroup);
	DDX_Control(pDX, IDC_CSBR2_SRCKEYNM, m_SrcKeyName);
	DDX_Control(pDX, IDC_CSBR2_SRCGRP, m_SrcGroup);
	DDX_Text(pDX, IDC_CSBR2_SRCDESCR, m_SrcDescription);
	DDX_Text(pDX, IDC_CSBR2_SRCREF, m_SrcReference);
	DDX_Text(pDX, IDC_CSBR2_SRCREFLBL, m_SrcRefLabel);
	DDX_Text(pDX, IDC_CSBR2_SRCSRC, m_SrcSource);
	DDX_Text(pDX, IDC_CSBR2_SRCUNIT, m_SrcUnit);
	DDX_Text(pDX, IDC_CSBR2_TRGDESCR, m_TrgDescription);
	DDX_Text(pDX, IDC_CSBR2_TRGREF, m_TrgReference);
	DDX_Text(pDX, IDC_CSBR2_TRGREFLBL, m_TrgRefLabel);
	DDX_Text(pDX, IDC_CSBR2_TRGSRC, m_TrgSource);
	DDX_Text(pDX, IDC_CSBR2_TRGUNIT, m_TrgUnit);
	//}}AFX_DATA_MAP
}
void CcsDualBrowse::RebuildKeyNameCombo (CComboBox& comboBox,const TCHAR* grpName)
{
	int ii, idx;
	struct csT_Csgrplst_ csDescr;

	comboBox.ResetContent ();
	for (ii = 0;CSt_csEnumByGroup (ii,grpName,&csDescr) > 0;ii += 1)
	{
		idx = comboBox.AddString (csDescr.key_nm);
		comboBox.SetItemData (idx,ii);
	}
	if (ii == 0)
	{
		idx = comboBox.AddString (_T("<none>"));
		comboBox.SetItemData (idx,9999999);
	}
}

BEGIN_MESSAGE_MAP(CcsDualBrowse, CDialog)
	//{{AFX_MSG_MAP(CcsDualBrowse)
	ON_BN_CLICKED(ID_CSBR2_HELP, OnCsbr2Help)
	ON_CBN_SELCHANGE(IDC_CSBR2_SRCGRP, OnChangeSrcGroup)
	ON_CBN_SELCHANGE(IDC_CSBR2_TRGGRP, OnChangeTrgGroup)
	ON_CBN_SELCHANGE(IDC_CSBR2_TRGKEYNM, OnChangeTrgKeyNm)
	ON_CBN_SELCHANGE(IDC_CSBR2_SRCKEYNM, OnChangeSrcKeyNm)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CcsDualBrowse message handlers
void CcsDualBrowse::OnCsbr2Help() 
{
}
void CcsDualBrowse::OnCancel() 
{
	CDialog::OnCancel();
}
void CcsDualBrowse::OnOK() 
{
	
	CDialog::OnOK();
}
void CcsDualBrowse::OnChangeSrcGroup() 
{
	int ii, idx, st;
	TCHAR grpName [24], grpDescr [64];

	ii = m_SrcGroup.GetCurSel ();
	idx = static_cast<int>(m_SrcGroup.GetItemData (ii));
	st = CSt_csGrpEnum (idx,grpName,tchrCount (grpName),grpDescr,tchrCount (grpDescr));
	RebuildKeyNameCombo (m_SrcKeyName,grpName);
	m_SrcKeyName.SetCurSel (0);
	OnChangeSrcKeyNm ();
	return;
}
void CcsDualBrowse::OnChangeTrgGroup() 
{
	int ii, idx, st;
	TCHAR grpName [24], grpDescr [64];

	ii = m_TrgGroup.GetCurSel ();
	idx = static_cast<int>(m_TrgGroup.GetItemData (ii));
	st = CSt_csGrpEnum (idx,grpName,tchrCount (grpName),grpDescr,tchrCount (grpDescr));
	RebuildKeyNameCombo (m_TrgKeyName,grpName);
	m_TrgKeyName.SetCurSel (0);
	OnChangeTrgKeyNm ();
	return;
}
void CcsDualBrowse::OnChangeSrcKeyNm() 
{
	int ii, idx, st;
	TCHAR grpName [cs_KEYNM_DEF];
	struct csT_Csgrplst_ csDescr;

	// First, we get ther current group name.
	ii = m_SrcGroup.GetCurSel ();
	idx = static_cast<int>(m_SrcGroup.GetItemData (ii));
	st = CSt_csGrpEnum (idx,grpName,tchrCount (grpName),NULL,0);

	// Here when the selected source key name changes.
	ii = m_SrcKeyName.GetCurSel ();
	idx = static_cast<int>(m_SrcKeyName.GetItemData (ii));
	st = -1;
	if (idx < 9999999)
	{
		st = CSt_csEnumByGroup (idx,grpName,&csDescr);
		if (st > 0)
		{
			m_Source = csDescr.key_nm;
			m_SrcDescription = csDescr.descr;
			m_SrcSource = csDescr.source;
			m_SrcRefLabel = csDescr.ref_typ;
			m_SrcRefLabel += _T(":");
			m_SrcReference = csDescr.ref_to;
			m_SrcUnit = csDescr.unit;
		}
	}
	if (st <= 0)
	{
		m_Source = _T("");
		m_SrcDescription = _T("");
		m_SrcSource = _T("");
		m_SrcRefLabel = _T("");
		m_SrcReference = _T("");
		m_SrcUnit = _T("");
	}
	UpdateData (FALSE);
}
void CcsDualBrowse::OnChangeTrgKeyNm() 
{
	// Here when the selected target key name changes.
	int ii, idx, st;
	TCHAR grpName [cs_KEYNM_DEF];
	struct csT_Csgrplst_ csDescr;

	// First, we get ther current group name.
	ii = m_TrgGroup.GetCurSel ();
	idx = static_cast<int>(m_TrgGroup.GetItemData (ii));
	st = CSt_csGrpEnum (idx,grpName,tchrCount (grpName),NULL,0);

	// Here when the selected source key name changes.
	ii = m_TrgKeyName.GetCurSel ();
	idx = static_cast<int>(m_TrgKeyName.GetItemData (ii));
	st = -1;
	if (idx < 9999999)
	{
		st = CSt_csEnumByGroup (idx,grpName,&csDescr);
		if (st > 0)
		{
			m_Target = csDescr.key_nm;
			m_TrgDescription = csDescr.descr;
			m_TrgSource = csDescr.source;
			m_TrgRefLabel = csDescr.ref_typ;
			m_TrgRefLabel += _T(":");
			m_TrgReference = csDescr.ref_to;
			m_TrgUnit = csDescr.unit;
		}
	}
	if (st <= 0)
	{
		m_Target = _T("");
		m_TrgDescription = _T("");
		m_TrgSource = _T("");
		m_TrgRefLabel = _T("");
		m_TrgReference = _T("");
		m_TrgUnit = _T("");
	}
	UpdateData (FALSE);
}
#endif
