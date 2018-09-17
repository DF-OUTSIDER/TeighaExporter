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
#include "cs_Legacy.h"

#if defined (__MFC__)

#include "cs_mfc.h"
#include "cs_hlp.h"


/**********************************************************************
**	CdtEdit dtEdit;
**			dtEdit.SetKeyName (initialName);
**			ok = dtEdit.DoModal ();
**			finalName = dtEdit.GeKeyName ();
**			extern "C" CS_dtEditor (keyName);
**
**	char* initialName;			key name of the datum which is to be displayed
**								upon dialog activation.
**	int ok;						returns IDOK if user exited with OK button, else
**								returns IDCANCEL.
**	char* finalName;			pointer to static memory containing the key name
**								of the datum definition which was displayed
**								when the user terminated the dialog box.
**	char* keyName;				character array which holds the initial name
**								when the function is called, and in which the
**								function returns the name of the datum
**								which was displayed when the dialog terminated.
**
**	Note, that the finalName pointer becomes invalid when the
**	CdtEdit object is detroyed (i.e. goes out of scope).
**********************************************************************/

///////////////////////////////////////////////////////////////////////////////
// Basic API access function.
extern "C" void EXP_LVL1 CS_dtEditor (char *dtKeyName)
{
	TCHAR tcKeyName [cs_KEYNM_DEF];
	CdtEdit dtEditor;

	CSt_strNCpy (tcKeyName,dtKeyName,cs_KEYNM_DEF);
	CS_recvr ();
	dtEditor.SetKeyName (tcKeyName);
	dtEditor.DoModal ();
	CSt_strNCpy (dtKeyName,dtEditor.GetKeyName (),cs_KEYNM_DEF);
	return;	
}
/////////////////////////////////////////////////////////////////////////////
// CdtEdit dialog
CdtEdit::CdtEdit(CWnd* pParent) : CDialog(CdtEdit::IDD, pParent)
{
	//{{AFX_DATA_INIT(CdtEdit)
	m_DeltaX = 0.0;
	m_DeltaY = 0.0;
	m_DeltaZ = 0.0;
	m_Description = _T("");
	m_RotX = 0.0;
	m_RotY = 0.0;
	m_RotZ = 0.0;
	m_Scale = 0.0;
	m_Source = _T("");
	m_EllipsoidDescr = _T("");
	//}}AFX_DATA_INIT

	m_Protected = false;
	m_Modified = false;
	m_NewIdx = -1;
	m_KeyName = _T("WGS84");
	m_PreviousKeyName = m_KeyName;
}

void CdtEdit::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CdtEdit)
	DDX_Control(pDX, IDC_DTED_KEYNAME, m_KeyNameCombo);
	DDX_Control(pDX, IDC_DTED_284VIA, m_284Via);
	DDX_Text(pDX, IDC_DTED_DELTAX, m_DeltaX);
	DDV_MinMaxDouble(pDX, m_DeltaX, -50000., 50000.);
	DDX_Text(pDX, IDC_DTED_DELTAY, m_DeltaY);
	DDV_MinMaxDouble(pDX, m_DeltaY, -50000., 50000.);
	DDX_Text(pDX, IDC_DTED_DELTAZ, m_DeltaZ);
	DDV_MinMaxDouble(pDX, m_DeltaZ, -50000., 50000.);
	DDX_Text(pDX, IDC_DTED_DESCR, m_Description);
	DDV_MaxChars(pDX, m_Description, 63);
	DDX_Text(pDX, IDC_DTED_ROTX, m_RotX);
	DDV_MinMaxDouble(pDX, m_RotX, -50000., 50000.);
	DDX_Text(pDX, IDC_DTED_ROTY, m_RotY);
	DDV_MinMaxDouble(pDX, m_RotY, -50000., 50000.);
	DDX_Text(pDX, IDC_DTED_ROTZ, m_RotZ);
	DDV_MinMaxDouble(pDX, m_RotZ, -50000., 50000.);
	DDX_Text(pDX, IDC_DTED_SCALE, m_Scale);
	DDV_MinMaxDouble(pDX, m_Scale, -2000., 2000.);
	DDX_Text(pDX, IDC_DTED_SOURCE, m_Source);
	DDV_MaxChars(pDX, m_Source, 63);
	DDX_Text(pDX, IDC_DTED_ELIPDSCR, m_EllipsoidDescr);
	DDV_MaxChars(pDX, m_EllipsoidDescr, 63);
	//}}AFX_DATA_MAP
}

void CdtEdit::SetKeyName (const TCHAR* dtKeyName)
{
	char kyTemp [cs_KEYNM_DEF + 2];

	CSt_strNCpy (kyTemp,dtKeyName,sizeof (kyTemp));
	CS_nampp (kyTemp);
	if (CS_dtIsValid (kyTemp))
	{
		CSt_cstringAssign (m_KeyName,kyTemp);
	}
}

const TCHAR* CdtEdit::GetKeyName ()
{
	return (LPCTSTR)m_KeyName;
}

BOOL CdtEdit::OnInitDialog ()
{
	int ii, idx, ident;
	CWnd *wp;
	TCHAR description [64];

	CDialog::OnInitDialog ();

	// Enumerate the current ellisoid key names and populate the
	// Key Name combo box.  We record the CS_MAP index with the
	// entry in case MFC doesn't sort the same as CS-MAP.  Then,
	// we select the initial ellipsoid as desired, populate the
	// the necessary fields, and display the whole mess.

	CS_recvr ();
	RebuildComboBox ();
	// Populate the "to84_bia" combo box.
	m_284Via.ResetContent ();
	for (ii = 0;
		 CSt_viaEnum (ii,description,tchrCount (description),&ident) > 0;
		 ii += 1)
	{
		idx = m_284Via.AddString (description);
		m_284Via.SetItemData (idx,ident);
	}
	ChangeDatum (m_KeyName);

	// Disable the Help button if help isn't available.
	wp = GetDlgItem (IDC_DTED_HELP);
	if (wp != NULL) wp->EnableWindow (CS_isHlpAvailable ());
	return (TRUE);
}

void CdtEdit::RebuildComboBox ()
{
	int ii;
	TCHAR dtName [32];

	m_KeyNameCombo.ResetContent ();
	for (ii = 0;CSt_dtEnum (ii,dtName,tchrCount (dtName)) > 0;ii += 1)
	{
		m_KeyNameCombo.AddString (dtName);
	}
	if (ii < 0)
	{
		TCHAR errMesg [256];
		CSt_errmsg (errMesg,tchrCount (errMesg));
		AfxMessageBox (errMesg);
	}
}

void CdtEdit::NewDatum (const CString& dtKeyName)
{
	m_KeyName = dtKeyName;
	m_Description.Empty ();
	m_Source.Empty ();
	m_DeltaX = 0.0;
	m_DeltaY = 0.0;
	m_DeltaZ = 0.0;
	m_RotX = 0.0;
	m_RotY = 0.0;
	m_RotZ = 0.0;
	m_Scale = 0.0;
	m_284Via.SetCurSel (0);
	m_Ellipsoid = "WGS84";
	NewEllipsoid ();
	return;
}

bool CdtEdit::ChangeDatum (const CString& dtKeyName)
{
	if (!GetDatum (dtKeyName)) return false;
	if (m_KeyNameCombo.SelectString (-1,m_KeyName) == CB_ERR)
	{	// Force WGS84 if the provided datum name was bogus
		m_KeyName = _T("WGS84");
		m_KeyNameCombo.SelectString (-1,(LPCTSTR)m_KeyName);
		GetDatum (m_KeyName);
	}
	m_Modified = false;
	m_NewIdx = -1;
	SetButtons ();
	UpdateData (FALSE);
	return true;
}

bool CdtEdit::GetDatum (const CString& dtKeyName)
{
	struct cs_Dtdef_ *dtPtr;

	dtPtr = CSt_dtdef (dtKeyName);
	if (dtPtr == NULL)
	{
		TCHAR errMesg [256];
		CSt_errmsg (errMesg,tchrCount (errMesg));
		AfxMessageBox (errMesg);
		return false;
	}
	m_Protected = (CS_prchk (dtPtr->protect) != 0);
	CSt_cstringAssign (m_Description,dtPtr->name);
	CSt_cstringAssign (m_Source,dtPtr->source);
	CSt_cstringAssign (m_Ellipsoid,dtPtr->ell_knm);
	NewEllipsoid ();
	m_DeltaX = dtPtr->delta_X;
	m_DeltaY = dtPtr->delta_Y;
	m_DeltaZ = dtPtr->delta_Z;
	m_RotX = dtPtr->rot_X;
	m_RotY = dtPtr->rot_Y;
	m_RotZ = dtPtr->rot_Z;
	m_Scale = dtPtr->bwscale;
	int selIdx = 0;
	for (int ii = 0;ii < m_284Via.GetCount ();ii += 1)
	{
		if (m_284Via.GetItemData (ii) == (DWORD)dtPtr->to84_via)
		{
			selIdx = ii;
			break;
		}
	}
	m_284Via.SetCurSel (selIdx);
	m_KeyName = dtKeyName;
	CS_free (dtPtr);
	return true;
}

int CdtEdit::PutDatum ()
{
	CString stemp;
	struct cs_Dtdef_ dtDef;

	memset (&dtDef,'\0',sizeof (dtDef));
	CSt_strNCpy (dtDef.key_nm,(LPCTSTR)m_KeyName,sizeof (dtDef.key_nm));
	CSt_strNCpy (dtDef.name,(LPCTSTR)m_Description,sizeof (dtDef.name));
	CSt_strNCpy (dtDef.source,(LPCTSTR)m_Source,sizeof (dtDef.source));

	dtDef.delta_X = m_DeltaX;
	dtDef.delta_Y = m_DeltaY;
	dtDef.delta_Z = m_DeltaZ;
	dtDef.rot_X = m_RotX;
	dtDef.rot_Y = m_RotY;
	dtDef.rot_Z = m_RotZ;
	dtDef.bwscale = m_Scale;
	CSt_strNCpy (dtDef.ell_knm,(LPCTSTR)m_Ellipsoid,sizeof (dtDef.ell_knm));
	dtDef.to84_via = cs_DTCTYP_NONE;
	int idx = m_284Via.GetCurSel ();
	if (idx != CB_ERR)
	{
	    m_284Via.GetLBText (idx,stemp);
	    if (stemp [0] != _T('*'))
	    {
    	    dtDef.to84_via = (short)m_284Via.GetItemData (idx);
    	    dtDef.protect = 0;
    	}
	}

	int st = CS_dtupd (&dtDef,0);
	if (st < 0)
	{
		TCHAR errMesg [256];
		CSt_errmsg (errMesg,tchrCount (errMesg));
		AfxMessageBox (errMesg);
	}
	return (st);
}

bool CdtEdit::DeleteDatum (const CString& dtKeyName)
{
	bool rtnValue;
	int st = IDOK;
	struct cs_Dtdef_ dtDef;

	if (m_Protected)
	{
		AfxMessageBox (_T("Displayed datum definition is protected and may not be deleted."));
		st = IDCANCEL;
		rtnValue = false;
	}
	else
	{
		st = AfxMessageBox (_T("Click 'OK' to confirm your request to delete this datum definition."),MB_OKCANCEL | MB_ICONQUESTION | MB_DEFBUTTON2);
		rtnValue = false;
	}
	if (st == IDOK)
	{
		memset (&dtDef,'\0',sizeof (dtDef));
		CSt_strNCpy (dtDef.key_nm,(LPCTSTR)dtKeyName,sizeof (dtDef.key_nm));
		int st = CS_dtdel (&dtDef);
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

BEGIN_MESSAGE_MAP(CdtEdit, CDialog)
	//{{AFX_MSG_MAP(CdtEdit)
	ON_BN_CLICKED(IDC_DTED_DELSAV, OnDtedDelsav)
	ON_EN_CHANGE(IDC_DTED_DELTAX, OnChange)
	ON_BN_CLICKED(IDC_DTED_HELP, OnDtedHelp)
	ON_BN_CLICKED(IDC_DTED_NEWCNCL, OnDtedNewcncl)
	ON_BN_CLICKED(ID_DTED_CLOSE, OnDtedClose)
	ON_CBN_SELENDOK(IDC_DTED_284VIA, OnSelendokDted284via)
	ON_CBN_SELCHANGE(IDC_DTED_KEYNAME, OnSelchangeDtedKeyname)
	ON_EN_CHANGE(IDC_DTED_DELTAY, OnChange)
	ON_EN_CHANGE(IDC_DTED_DELTAZ, OnChange)
	ON_EN_CHANGE(IDC_DTED_DESCR, OnChange)
	ON_EN_CHANGE(IDC_DTED_ROTX, OnChange)
	ON_EN_CHANGE(IDC_DTED_ROTY, OnChange)
	ON_EN_CHANGE(IDC_DTED_ROTZ, OnChange)
	ON_EN_CHANGE(IDC_DTED_SCALE, OnChange)
	ON_EN_CHANGE(IDC_DTED_SOURCE, OnChange)
	ON_BN_CLICKED(IDC_DTED_ELIPBTN, OnDtedNewEllipsoid)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CdtEdit message handlers

void CdtEdit::OnDtedDelsav() 
{
	int st;

	if (m_Modified)
	{	// Here to perform the save operation.
		if (!UpdateData (TRUE)) return;
		st = PutDatum ();
		if (st == 0)
		{
			CS_recvr ();
			RebuildComboBox ();
		}
		if (st >= 0)
		{
			m_NewIdx = -1;
			ChangeDatum (m_KeyName);
		}
	}
	else
	{	// Here to perform the delete operation.  Must determine
		// which datum to display after the delete operation.
		char dtName [32];
		char c_KeyName [32];
		
		int newIdx = 0;
		CSt_strNCpy (c_KeyName,(LPCTSTR)m_KeyName,sizeof (c_KeyName));
		for (int ii = 0;CS_dtEnum (ii,dtName,sizeof (dtName)) > 0;ii += 1)
		{
			if (!CS_stricmp (c_KeyName,dtName)) break;
			newIdx = ii;
		}
		if (CS_dtEnum (newIdx,dtName,sizeof (dtName)) <= 0)
		{	// Display WGS84 if all else fails.
			strcpy (dtName,"WGS84");
		}
		if (DeleteDatum (m_KeyName))
		{
			CSt_cstringAssign (m_KeyName,dtName);
			// Rebuild the combo box with new names and indices.
			CS_recvr ();
			RebuildComboBox ();
			// Select the new ellipsoid
			ChangeDatum (m_KeyName);
		}
	}
}

void CdtEdit::OnChange() 
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
void CdtEdit::OnDtedHelp() 
{
	CSwinhlp (m_hWnd,csHLPID_DTEDIT);
}
void CdtEdit::OnDtedNewcncl() 
{
	if (m_Modified)
	{	// Here to perform a cancel.
		if (m_NewIdx >= 0)
		{
			m_KeyNameCombo.DeleteString (m_NewIdx);
			m_NewIdx = -1;
			m_KeyName = m_PreviousKeyName;
		}
		ChangeDatum (m_KeyName);
	}
	else
	{	// Here to create a new datum.  Save the current name for
		// use in the event of a subsequent cancel operation.  Then,
		// have operator provide the name of the new datum.

		m_PreviousKeyName = m_KeyName;
		CnewKeyName dtNewName (CnewKeyName::Datum,this);
		INT_PTR st = dtNewName.DoModal ();
		if (st == IDOK)
		{
			m_KeyName = dtNewName.GetNewName ();
			// See if we are to reset all values.
			if (!dtNewName.GetPreserve ())
			{
				NewDatum (m_KeyName);
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

void CdtEdit::OnDtedClose() 
{
	int response;
	if (m_Modified && !m_Protected)
	{
		response = AfxMessageBox (_T("Changes have been made, but not saved.  Do you\n")
								  _T("want to exit and discard these changes?"),
								  MB_YESNO | MB_ICONQUESTION);
		if (response == IDNO) return;
	}
	CDialog::OnCancel ();
	return;
}
void CdtEdit::SetButtons ()
{
	bool deltas, rots, scale, grid;
	int idx;
	CWnd *wp;

	idx = m_284Via.GetCurSel ();
	idx = static_cast<int>(m_284Via.GetItemData (idx));

	deltas = rots = scale = grid = false;
	switch (idx) {
	case cs_DTCTYP_NAD83:
	case cs_DTCTYP_MREG:
	case cs_DTCTYP_WGS84:
	case cs_DTCTYP_WGS72:
	case cs_DTCTYP_HPGN:
	case cs_DTCTYP_GDA94:
	case cs_DTCTYP_NZGD2K:
		break;
	case cs_DTCTYP_AGD66:
	case cs_DTCTYP_NAD27:
	case cs_DTCTYP_AGD84:
	case cs_DTCTYP_NZGD49:
	case cs_DTCTYP_ATS77:
	case cs_DTCTYP_CSRS:
		deltas = rots = scale = false;
		break;
	case cs_DTCTYP_MOLO:
		deltas = true;
		break;
	case cs_DTCTYP_7PARM:
	case cs_DTCTYP_BURS:
		deltas = rots = scale = true;
		break;
	case cs_DTCTYP_3PARM:
	case cs_DTCTYP_GEOCTR:
		deltas = true;
		break;
	case cs_DTCTYP_6PARM:
		deltas = rots = true;
		break;
	case cs_DTCTYP_4PARM:
		deltas = scale = true;
		break;
	default:
		deltas = rots = scale = false;
		break;
	}

	wp = GetDlgItem (IDC_DTED_NEWCNCL);
	if (wp != NULL) wp->SetWindowText (m_Modified ? _T("Cancel") : _T("New"));
	wp = GetDlgItem (IDC_DTED_DELSAV);
	if (wp != NULL)
	{
		wp->SetWindowText (m_Modified ? _T("Save") : _T("Delete"));
		wp->EnableWindow (!m_Protected);
	}
	wp = GetDlgItem (IDC_DTED_KEYNAME);
	if (wp != NULL) wp->EnableWindow (!m_Modified);

	wp = GetDlgItem (IDC_DTED_DELTAX);
	if (wp != NULL) wp->EnableWindow (deltas);
	wp = GetDlgItem (IDC_DTED_DELTAY);
	if (wp != NULL) wp->EnableWindow (deltas);
	wp = GetDlgItem (IDC_DTED_DELTAZ);
	if (wp != NULL) wp->EnableWindow (deltas);

	wp = GetDlgItem (IDC_DTED_ROTX);
	if (wp != NULL) wp->EnableWindow (rots);
	wp = GetDlgItem (IDC_DTED_ROTY);
	if (wp != NULL) wp->EnableWindow (rots);
	wp = GetDlgItem (IDC_DTED_ROTZ);
	if (wp != NULL) wp->EnableWindow (rots);

	wp = GetDlgItem (IDC_DTED_SCALE);
	if (wp != NULL) wp->EnableWindow (scale);

	return;
}

void CdtEdit::OnCancel() 
{
	int response;
	if (m_Modified && !m_Protected)
	{
		response = AfxMessageBox (_T("Changes have been made, but not saved.  Do you\n")
								  _T("to exit and discard these changes?"),
								  MB_YESNO | MB_ICONQUESTION);
		if (response == IDNO) return;
	}
	CDialog::OnCancel ();
	return;
}
void CdtEdit::OnSelendokDted284via() 
{
	OnChange ();
	SetButtons ();
}
void CdtEdit::OnSelchangeDtedKeyname() 
{
	int idx = m_KeyNameCombo.GetCurSel ();
	m_KeyNameCombo.GetLBText (idx,m_KeyName);
	ChangeDatum (m_KeyName);
	return;
}
void CdtEdit::OnDtedNewEllipsoid() 
{
	TCHAR newKeyName [32];
	CelSelect elSelect;
	
	UpdateData (TRUE);
	elSelect.SetInitialEllipsoid (m_Ellipsoid);
	INT_PTR st = elSelect.DoModal ();
	if (st == IDOK)
	{
		CSt_strNCpy (newKeyName,elSelect.GetSelectedEllipsoid (),tchrCount (newKeyName));		
		if (CSt_stricmp (m_Ellipsoid,newKeyName))
		{
			OnChange ();
			m_Ellipsoid = newKeyName;
			OnChange ();
			NewEllipsoid ();
			UpdateData (FALSE);
		}
	}
}
void CdtEdit::NewEllipsoid()
{
	struct cs_Eldef_ *elPtr;
	m_EllipsoidDescr = _T("???");
	elPtr = CSt_eldef (m_Ellipsoid);
	if (elPtr != NULL)
	{
		CSt_cstringAssign (m_EllipsoidDescr,elPtr->name);
		CS_free (elPtr);
	}
	return;
}
#endif
