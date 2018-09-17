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
// Basic API access function.
extern "C" void EXP_LVL1 CS_elEditor (char *elKeyName)
{
	TCHAR tcKeyName [cs_KEYNM_DEF];
	CelEdit elEditor;

	CS_recvr ();
	CSt_strNCpy (tcKeyName,elKeyName,cs_KEYNM_DEF);
	elEditor.SetKeyName (tcKeyName);
	INT_PTR st = elEditor.DoModal ();
	if (st == IDOK)
	{
		CSt_strNCpy (elKeyName,elEditor.GetKeyName (),cs_KEYNM_DEF);
	}
	return;	
}
/////////////////////////////////////////////////////////////////////////////
// CelEdit dialog
CelEdit::CelEdit(CWnd* pParent) : CDialog(CelEdit::IDD, pParent)
{
	//{{AFX_DATA_INIT(CelEdit)
	m_EsqString = _T("");
	m_FlatString = _T("");
	m_Source = _T("");
	m_Description = _T("");
	m_EqRadius = 0.0;
	m_PlrRadius = 0.0;
	//}}AFX_DATA_INIT

	m_Protected = false;
	m_Modified = false;
	m_NewIdx = -1;
	m_KeyName = _T("WGS84");
	m_PreviousKeyName = m_KeyName;
}

void CelEdit::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CelEdit)
	DDX_Control(pDX, IDC_ELED_KEYNAME, m_KeyNameCombo);
	DDX_Text(pDX, IDC_ELED_ESQDISP, m_EsqString);
	DDX_Text(pDX, IDC_ELED_FLTDISP, m_FlatString);
	DDX_Text(pDX, IDC_ELED_SOURCE, m_Source);
	DDV_MaxChars(pDX, m_Source, 63);
	DDX_Text(pDX, IDC_ELED_DESCR, m_Description);
	DDX_Text(pDX, IDC_ELED_EQRAD, m_EqRadius);
	DDV_MinMaxDouble(pDX, m_EqRadius, 1.e-004, 99999999.);
	DDX_Text(pDX, IDC_ELED_PLRAD, m_PlrRadius);
	DDV_MinMaxDouble(pDX, m_PlrRadius, 1.e-003, 99999999.);
	//}}AFX_DATA_MAP
}

void CelEdit::SetKeyName (const TCHAR* elKeyName)
{
	char kyTemp [cs_KEYNM_DEF + 2];

	CSt_strNCpy (kyTemp,elKeyName,sizeof (kyTemp));
	CS_nampp (kyTemp);
	if (CS_elIsValid (kyTemp))
	{
		CSt_cstringAssign (m_KeyName,kyTemp);
	}
}

const TCHAR* CelEdit::GetKeyName ()
{
	return (LPCTSTR)m_KeyName;
}

BOOL CelEdit::OnInitDialog ()
{
	CWnd *wp;

	CDialog::OnInitDialog ();

	// Enumerate the current ellisoid key names and populate the
	// Key Name combo box.  We record the CS_MAP index with the
	// entry in case MFC doesn't sort the same as CS-MAP.  Then,
	// we select the initial ellipsoid as desired, populate the
	// the necessary fields, and display the whole mess.

	CS_recvr ();
	RebuildComboBox ();
	ChangeEllipsoid (m_KeyName);

	// Disable the Help button if help isn't available.
	wp = GetDlgItem (ID_ELED_HELP);
	if (wp != NULL) wp->EnableWindow (CS_isHlpAvailable ());
	return (TRUE);
}

void CelEdit::RebuildComboBox ()
{
	int ii;
	TCHAR elName [32];

	m_KeyNameCombo.ResetContent ();
	for (ii = 0;CSt_elEnum (ii,elName,tchrCount (elName)) > 0;ii += 1)
	{
		m_KeyNameCombo.AddString (elName);
	}
	if (ii < 0)
	{
		TCHAR errMesg [256];
		CSt_errmsg (errMesg,tchrCount (errMesg));
		AfxMessageBox (errMesg);
	}
}

void CelEdit::NewEllipsoid (const CString& elKeyName)
{
	m_KeyName = elKeyName;
	m_Description.Empty ();
	m_Source.Empty ();
	m_EqRadius = 0.0;
	m_PlrRadius = 0.0;
	m_EsqString.Empty ();
	m_FlatString.Empty ();
	return;
}

bool CelEdit::ChangeEllipsoid (const CString& elKeyName)
{
	if (!GetEllipsoid (elKeyName)) return false;
	if (m_KeyNameCombo.SelectString (-1,m_KeyName) == CB_ERR)
	{	// Force WGS84 if the provided ellipsoid neame was bogus
		m_KeyName = _T("WGS84");
		m_KeyNameCombo.SelectString (-1,(LPCTSTR)m_KeyName);
		GetEllipsoid (m_KeyName);
	}
	CalculateDisplay ();
	m_Modified = false;
	m_NewIdx = -1;
	SetButtons ();
	UpdateData (FALSE);
	return true;
}

bool CelEdit::GetEllipsoid (const CString& elKeyName)
{
	struct cs_Eldef_ *elPtr;

	elPtr = CSt_eldef (elKeyName);
	if (elPtr == NULL)
	{
		TCHAR errMesg [256];
		CSt_errmsg (errMesg,tchrCount (errMesg));
		AfxMessageBox (errMesg);
		return false;
	}
	m_Protected = (CS_prchk (elPtr->protect) != 0);
	CSt_cstringAssign (m_Description,elPtr->name);
	CSt_cstringAssign (m_Source,elPtr->source);
	m_EqRadius = elPtr->e_rad;
	m_PlrRadius = elPtr->p_rad;
	m_KeyName = elKeyName;
	CS_free (elPtr);
	return true;
}

int CelEdit::PutEllipsoid ()
{
	struct cs_Eldef_ elDef;

	memset (&elDef,'\0',sizeof (elDef));
	CSt_strNCpy (elDef.key_nm,(LPCTSTR)m_KeyName,sizeof (elDef.key_nm));
	CSt_strNCpy (elDef.name,(LPCTSTR)m_Description,sizeof (elDef.name));
	CSt_strNCpy (elDef.source,(LPCTSTR)m_Source,sizeof (elDef.source));
	elDef.e_rad = m_EqRadius;
	elDef.p_rad = m_PlrRadius;
	elDef.flat = 1.0 - m_EqRadius / m_PlrRadius;
	double aSq = m_EqRadius * m_EqRadius;
	double bSq = m_PlrRadius * m_PlrRadius;
	elDef.ecent = sqrt ((aSq - bSq) / aSq);
	elDef.protect = 0;

	int st = CS_elupd (&elDef,0);
	if (st < 0)
	{
		TCHAR errMesg [256];
		CSt_errmsg (errMesg,tchrCount (errMesg));
		AfxMessageBox (errMesg);
	}
	return (st);
}

bool CelEdit::DeleteEllipsoid (const CString& elKeyName)
{
	bool rtnValue = true;
	int st = IDOK;
	struct cs_Eldef_ elDef;

	if (m_Protected)
	{
		AfxMessageBox (_T("Displayed ellipsoid definition is protected and may not be deleted."));
		st = IDCANCEL;
		rtnValue = false;
	}
	else
	{
		st = AfxMessageBox (_T("Click 'OK' to confirm your request to delete this ellipsoid definition."),MB_OKCANCEL | MB_ICONQUESTION | MB_DEFBUTTON2);
		rtnValue = false;
	}
	if (st == IDOK)
	{
		memset (&elDef,'\0',sizeof (elDef));
		CSt_strNCpy (elDef.key_nm,(LPCTSTR)elKeyName,sizeof (elDef.key_nm));
		int st = CS_eldel (&elDef);
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

void CelEdit::CalculateDisplay ()
{
	double eSq;
	double flat;

	if (m_PlrRadius <= 0.0 || m_EqRadius <= 0.0 || fabs (m_PlrRadius - m_EqRadius) < 0.001)
	{	// Don't want to divide by zero.
		m_PlrRadius = m_EqRadius;
		m_EsqString.Empty ();
		m_FlatString.Empty ();
	}
	else
	{
		flat = 1.0 - m_PlrRadius / m_EqRadius;
		eSq = (flat + flat) - (flat * flat);
		m_EsqString.Format (_T("%.10f"),eSq);
		m_FlatString.Format (_T("%.6f"),(1.0 / flat));
	}
	return;
}

BEGIN_MESSAGE_MAP(CelEdit, CDialog)
	//{{AFX_MSG_MAP(CelEdit)
	ON_BN_CLICKED(ID_ELED_CLOSE, OnEledClose)
	ON_BN_CLICKED(ID_ELED_HELP, OnEledHelp)
	ON_EN_CHANGE(IDC_ELED_DESCR, OnChange)
	ON_BN_CLICKED(ID_ELED_NEWCNCL, OnEledNewcncl)
	ON_EN_KILLFOCUS(IDC_ELED_PLRAD, OnChangeRadii)
	ON_BN_CLICKED(ID_ELED_DELSAV, OnEledDelsav)
	ON_EN_CHANGE(IDC_ELED_SOURCE, OnChange)
	ON_EN_KILLFOCUS(IDC_ELED_EQRAD, OnChangeRadii)
	ON_EN_CHANGE(IDC_ELED_EQRAD, OnChange)
	ON_EN_CHANGE(IDC_ELED_PLRAD, OnChange)
	ON_CBN_SELCHANGE(IDC_ELED_KEYNAME, OnNewSelection)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CelEdit message handlers

void CelEdit::OnEledClose() 
{
	int response;

	if (m_Modified && !m_Protected)
	{
		response = AfxMessageBox (_T("Changes have been made, but not saved.\nDo you to exit and discard these changes?"),MB_YESNO | MB_ICONQUESTION);
		if (response == IDNO) return;
	}
	CDialog::OnCancel ();
	return;
}

void CelEdit::OnCancel ()
{
	int response;

	if (m_Modified && !m_Protected)
	{
		response = AfxMessageBox (_T("Changes have been made, but not saved.\nDo you to exit and discard these changes?"),MB_YESNO | MB_ICONQUESTION);
		if (response == IDNO) return;
	}
	CDialog::OnCancel ();
	return;
}

void CelEdit::OnEledDelsav() 
{
	int st;

	if (m_Modified)
	{	// Here to perform the save operation.
		if (!UpdateData (TRUE)) return;
		st = PutEllipsoid ();
		if (st == 0)
		{
			CS_recvr ();
			RebuildComboBox ();
		}
		if (st >= 0)
		{
			m_NewIdx = -1;
			ChangeEllipsoid (m_KeyName);
		}
	}
	else
	{	// Here to perform the delete operation.  Must determine
		// which ellipsoid to display after the delete operation.
		char elName [32];
		char c_KeyName [32];
		int newIdx = 0;
		CSt_strNCpy (c_KeyName,(LPCTSTR)m_KeyName,sizeof (c_KeyName));
		for (int ii = 0;CS_elEnum (ii,elName,sizeof (elName)) > 0;ii += 1)
		{
			if (!CS_stricmp (elName,c_KeyName)) break;
			newIdx = ii;		
		}
		if (CS_elEnum (newIdx,elName,sizeof (elName)) <= 0)
		{	// Display WGS84 if all else fails.
			strcpy (elName,"WGS84");
		}
		if (DeleteEllipsoid (m_KeyName))
		{
			CSt_cstringAssign (m_KeyName,elName);
			// Rebuild the combo box with new names and indices.
			CS_recvr ();
			RebuildComboBox ();
			// Select the new ellipsoid
			ChangeEllipsoid (m_KeyName);
		}
	}
}
void CelEdit::OnEledHelp() 
{
	CSwinhlp (m_hWnd,csHLPID_ELEDIT);
}
void CelEdit::OnEledNewcncl()
{
	if (m_Modified)
	{	// Here to perform a cancel.
		if (m_NewIdx >= 0)
		{
			m_KeyNameCombo.DeleteString (m_NewIdx);
			m_NewIdx = -1;
			m_KeyName = m_PreviousKeyName;
		}
		ChangeEllipsoid ((LPCTSTR)m_KeyName);
	}
	else
	{
		// Here to create a new ellipsoid.  Save the current name for
		// use in the event of a subsequent cancel operation.  Then,
		// have operator provide the name of the new ellipsoid.

		m_PreviousKeyName = m_KeyName;
		CnewKeyName elNewName (CnewKeyName::Ellipsoid,this);
		INT_PTR st = elNewName.DoModal ();
		if (st == IDOK)
		{
			m_KeyName = elNewName.GetNewName ();
			// See if we are to reset all values.
			if (!elNewName.GetPreserve ())
			{
				NewEllipsoid (m_KeyName);
			}
			m_Modified = true;
			m_Protected = false;
			m_NewIdx = m_KeyNameCombo.AddString (m_KeyName);
			m_KeyNameCombo.SelectString (-1,m_KeyName);
			SetButtons ();
			UpdateData (FALSE);
		}
	}
}

void CelEdit::OnNewSelection() 
{
	int idx = m_KeyNameCombo.GetCurSel ();
	m_KeyNameCombo.GetLBText (idx,m_KeyName);
	ChangeEllipsoid (m_KeyName);
	return;
}
void CelEdit::OnChange() 
{
	if (!m_Modified)
	{
		if (m_Protected)
		{
			AfxMessageBox (_T("Current definition is protected.\nYou will not be able to save your changes."));
		}
		m_Modified = true;
		SetButtons ();
	}
}
void CelEdit::OnChangeRadii() 
{
	if (!UpdateData (TRUE)) return;
	CalculateDisplay ();
	UpdateData (FALSE);
	return;
}

void CelEdit::SetButtons ()
{
	CWnd *wp;

	wp = GetDlgItem (ID_ELED_NEWCNCL);
	if (wp != NULL) wp->SetWindowText (m_Modified ? _T("Cancel") : _T("New"));
	wp = GetDlgItem (ID_ELED_DELSAV);
	if (wp != NULL)
	{
		wp->SetWindowText (m_Modified ? _T("Save") : _T("Delete"));
		wp->EnableWindow (!m_Protected);
	}
	wp = GetDlgItem (IDC_ELED_KEYNAME);
	if (wp != NULL) wp->EnableWindow (!m_Modified);
}
#endif
