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
int EXP_LVL1 CS_elSelector (char *elKeyName)
{
	TCHAR tcKeyName [cs_KEYNM_DEF];
	CelSelect elSelect;

	CSt_strNCpy (tcKeyName,elKeyName,cs_KEYNM_DEF);
	elSelect.SetInitialEllipsoid (tcKeyName);
	INT_PTR st = elSelect.DoModal ();
	if (st == IDOK)
	{
		CSt_strNCpy (tcKeyName,elSelect.GetSelectedEllipsoid (),cs_KEYNM_DEF);
	}
	return (int)st;
}
int EXP_LVL1 CS_elSelectorA (char *elKeyName,char *description,char *source)
{
	TCHAR tcKeyName [cs_KEYNM_DEF];
	CelSelect elSelect;

	CSt_strNCpy (tcKeyName,elKeyName,cs_KEYNM_DEF);
	elSelect.SetInitialEllipsoid (tcKeyName);
	INT_PTR st = elSelect.DoModal ();
	if (st == IDOK)
	{
		CSt_strNCpy (elKeyName,elSelect.GetSelectedEllipsoid (),cs_KEYNM_DEF);
		CSt_strNCpy (description,elSelect.GetSelectedDescription (),64);
		CSt_strNCpy (source,elSelect.GetSelectedSource (),64);
	}
	return (int)st;
}
/**********************************************************************
**	CelSelect elSelect;
**			elSelect.SetInitialEllipsoid (initialName);
**			finalName = GetSelectedEllipsoid ();
**
**	char* initialName;			key name of the ellipsoid which is to be displayed
**								upon dialog activation.
**	char* finalName;			pointer to static memory containing the key name
**								of the ellipsoid definition which was displayed
**								when the user terminated the dialog box.
**********************************************************************/

/////////////////////////////////////////////////////////////////////////////
// CelSelect dialog
CelSelect::CelSelect(CWnd* pParent) : CDialog(CelSelect::IDD, pParent)
{
	//{{AFX_DATA_INIT(CelSelect)
	m_Description = _T("");
	m_Source = _T("");
	m_Numbers = _T("");
	//}}AFX_DATA_INIT
	m_KeyName = _T("WGS84");
}
void CelSelect::SetInitialEllipsoid (const TCHAR* elKeyName)
{
	m_KeyName = elKeyName;
}
BOOL CelSelect::OnInitDialog ()
{
	int ii, idx;
	CWnd *wp;
	TCHAR keyName [24];

	CDialog::OnInitDialog ();

	m_KeyNameCombo.ResetContent ();
	for (ii = 0;
		 CSt_elEnum (ii,keyName,tchrCount (keyName)) > 0;
		 ii += 1)
	{
		idx = m_KeyNameCombo.AddString (keyName);
		m_KeyNameCombo.SetItemData (idx,ii);
	}
	if (!CSt_elIsValid (m_KeyName))
	{
		m_KeyName = _T("WGS84");
	}
	m_KeyNameCombo.SelectString (-1,m_KeyName);
	ShowDetails ();

	// Disable the Help button if Help is not available
	wp = GetDlgItem (ID_ELSL_HELP);
	if (wp != NULL) wp->EnableWindow (CS_isHlpAvailable ());

	return (TRUE);
}
void CelSelect::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CelSelect)
	DDX_Control(pDX, IDC_ELSL_KEYNAME, m_KeyNameCombo);
	DDX_Text(pDX, IDC_ELSL_DESCR, m_Description);
	DDV_MaxChars(pDX, m_Description, 64);
	DDX_Text(pDX, IDC_ELSL_SOURCE, m_Source);
	DDV_MaxChars(pDX, m_Source, 64);
	DDX_Text(pDX, IDC_ELSL_NUMBERS, m_Numbers);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CelSelect, CDialog)
	//{{AFX_MSG_MAP(CelSelect)
	ON_BN_CLICKED(ID_ELSL_HELP, OnElslHelp)
	ON_CBN_SELCHANGE(IDC_ELSL_KEYNAME, OnSelchangeElslKeyname)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CelSelect message handlers
void CelSelect::OnElslHelp() 
{
	CSwinhlp (m_hWnd,csHLPID_ELSELECTOR);
}
void CelSelect::OnSelchangeElslKeyname() 
{
	int idx;
	TCHAR keyName [24];

	idx = m_KeyNameCombo.GetCurSel ();
	idx = static_cast<int>(m_KeyNameCombo.GetItemData (idx));
	CSt_elEnum (idx,keyName,tchrCount (keyName));
	m_KeyName = keyName;
	ShowDetails ();
	return;
}
void CelSelect::OnOK() 
{
	if (!UpdateData (TRUE)) return;
	CDialog::OnOK();
}
void CelSelect::ShowDetails ()
{
	double flat;
	struct cs_Eldef_ *elPtr;

	elPtr = CSt_eldef (m_KeyName);
	if (elPtr == NULL)
	{
		TCHAR errMesg [256];
		CSt_errmsg (errMesg,tchrCount (errMesg));
		AfxMessageBox (errMesg);
		return;
	}
	flat = 0.0;
	if (fabs (elPtr->flat) > 1.0E-10) flat = 1.0 / elPtr->flat;
	CSt_cstringAssign (m_Description,elPtr->name);
	CSt_cstringAssign (m_Source,elPtr->source);
	m_Numbers.Format (_T("%12.3f, %12.3f, %10.5f, %10.8f"),
					  elPtr->e_rad,
					  elPtr->p_rad,
					  flat,
					  elPtr->ecent * elPtr->ecent);
	CS_free (elPtr);
	UpdateData (FALSE);
	return;
}
#endif
