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
// Basic 'C' callable function interface
int EXP_LVL1 CS_dtSelector (char *dtKeyName)
{
	TCHAR tcKeyName [cs_KEYNM_DEF];
	CdtSelect dtSelect;

	CSt_strNCpy (tcKeyName,dtKeyName,cs_KEYNM_DEF);
	dtSelect.SetInitialDatum (tcKeyName);
	INT_PTR st = dtSelect.DoModal ();
	if (st == IDOK)
	{
		CSt_strNCpy (dtKeyName,dtSelect.GetSelectedDatum (),cs_KEYNM_DEF);
	}
	return static_cast<int>(st);
}
int EXP_LVL1 CS_dtSelectorA (char *dtKeyName,char *description,char *source,char *ellipsoid,char *technique)
{
	TCHAR tcKeyName [cs_KEYNM_DEF];
	CdtSelect dtSelect;

	CSt_strNCpy (tcKeyName,dtKeyName,cs_KEYNM_DEF);
	dtSelect.SetInitialDatum (tcKeyName);
	INT_PTR st = dtSelect.DoModal ();
	if (st == IDOK)
	{
		CSt_strNCpy (dtKeyName,dtSelect.GetSelectedDatum (),cs_KEYNM_DEF);
		CSt_strNCpy (description,dtSelect.GetSelectedDescription (),64);
		CSt_strNCpy (source,dtSelect.GetSelectedSource (),64);
		CSt_strNCpy (ellipsoid,dtSelect.GetSelectedEllipsoid (),cs_KEYNM_DEF);
		CSt_strNCpy (technique,dtSelect.GetSelectedTechnique (),32);
	}
	return (int)st;
}
/**********************************************************************
**	CdtSelect dtSelect;
**			dtSelect.SetInitialDatum (initialName);
**			finalName = GetSelectedDatum ();
**
**	char* initialName;			key name of the datum which is to be displayed
**								upon dialog activation.
**	char* finalName;			pointer to static memory containing the key name
**								of the datum definition which was displayed
**								when the user terminated the dialog box.
**********************************************************************/

/////////////////////////////////////////////////////////////////////////////
// CdtSelect dialog
CdtSelect::CdtSelect(CWnd* pParent /*=NULL*/)
		: CDialog(CdtSelect::IDD, pParent)
{
	//{{AFX_DATA_INIT(CdtSelect)
	m_Description = _T("");
	m_Ellipsoid = _T("");
	m_Source = _T("");
	m_Technique = _T("");
	//}}AFX_DATA_INIT
	m_KeyName = _T("WGS84");
}
void CdtSelect::SetInitialDatum (const TCHAR* dtKeyName)
{
	m_KeyName = dtKeyName;
}
BOOL CdtSelect::OnInitDialog (void)
{
	int ii, idx;
	CWnd *wp;
	TCHAR keyName [cs_KEYNM_DEF];

	CDialog::OnInitDialog ();

	// Populate the datum key name combo box.

	m_KeyNameCombo.ResetContent ();
	for (ii = 0;
		 CSt_dtEnum (ii,keyName,tchrCount (keyName)) > 0;
		 ii += 1)
	{
		idx = m_KeyNameCombo.AddString (keyName);
		m_KeyNameCombo.SetItemData (idx,ii);
	}
	if (!CSt_dtIsValid (m_KeyName))
	{
		m_KeyName = _T("WGS84");
	}
	m_KeyNameCombo.SelectString (-1,m_KeyName);
	ShowDetails ();

	wp = GetDlgItem (ID_DTSL_HELP);
	if (wp != NULL) wp->EnableWindow (CS_isHlpAvailable ());
	return (TRUE);
}

void CdtSelect::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CdtSelect)
	DDX_Control(pDX, IDC_DTSL_KEYNAME, m_KeyNameCombo);
	DDX_Text(pDX, IDC_DTSL_DESCR, m_Description);
	DDV_MaxChars(pDX, m_Description, 63);
	DDX_Text(pDX, IDC_DTSL_ELLIP, m_Ellipsoid);
	DDV_MaxChars(pDX, m_Ellipsoid, 63);
	DDX_Text(pDX, IDC_DTSL_SOURCE, m_Source);
	DDV_MaxChars(pDX, m_Source, 63);
	DDX_Text(pDX, IDC_DTSL_TECHNIQUE, m_Technique);
	DDV_MaxChars(pDX, m_Technique, 63);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CdtSelect, CDialog)
	//{{AFX_MSG_MAP(CdtSelect)
	ON_CBN_SELCHANGE(IDC_DTSL_KEYNAME, OnSelchangeDtslKeyname)
	ON_BN_CLICKED(ID_DTSL_HELP, OnDtslHelp)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CdtSelect message handlers

void CdtSelect::OnOK() 
{
	CDialog::OnOK();
}
void CdtSelect::OnSelchangeDtslKeyname() 
{
	int idx;
	TCHAR keyName [cs_KEYNM_DEF];

	idx = m_KeyNameCombo.GetCurSel ();
	idx = static_cast<int>(m_KeyNameCombo.GetItemData (idx));
	CSt_dtEnum (idx,keyName,tchrCount (keyName));
	m_KeyName = keyName;
	ShowDetails ();
	return;
}
void CdtSelect::OnDtslHelp() 
{
	CSwinhlp (m_hWnd,csHLPID_DTSELECTOR);
}
void CdtSelect::ShowDetails ()
{
	int ii, ident;
	struct cs_Dtdef_ *dtPtr;
	struct cs_Eldef_ *elPtr;
	TCHAR description [64];

	dtPtr = CSt_dtdef (m_KeyName);
	if (dtPtr == NULL)
	{
		TCHAR errMesg [256];
		CSt_errmsg (errMesg,tchrCount (errMesg));
		AfxMessageBox (errMesg);
		return;
	}
	CSt_cstringAssign (m_Description,dtPtr->name);
	CSt_cstringAssign (m_Source,dtPtr->source);
	elPtr = CS_eldef (dtPtr->ell_knm);
	if (elPtr == NULL) m_Ellipsoid = _T("???");
	else
	{
		CSt_cstringAssign (m_Ellipsoid,elPtr->name);
		CS_free (elPtr);
	}
	m_Technique = _T("???");
	for (ii = 0;
		 CSt_viaEnum (ii,description,tchrCount (description),&ident) > 0;
		 ii += 1)
	{
		if (dtPtr->to84_via == ident)
		{
			m_Technique = description;
			break;
		}
	}
	CS_free (dtPtr);
	UpdateData (FALSE);
	return;
}

#endif
