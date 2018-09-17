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

extern "C" char cs_Unique;
///////////////////////////////////////////////////////////////////////////////
// Basic 'C' callable function interface
extern "C" int EXP_LVL1 CS_newKeyName (char *keyName,int type,int preserve)
{
	int st = 0;

	CnewKeyName dlgNewKeyName (static_cast<CnewKeyName::TnewKeyNameTypes>(type));

	dlgNewKeyName.SetPreserveEnable (preserve != 0);
	INT_PTR stDlg = dlgNewKeyName.DoModal ();
	if (stDlg == IDOK)
	{
		CString tempStr = dlgNewKeyName.GetNewName ();
		CSt_strNCpy (keyName,(LPCTSTR)tempStr,cs_KEYNM_DEF);
		st = dlgNewKeyName.GetPreserve () ? 1 : 0;
	}
	else
	{
		st = -1;
	}
	return st;
}
extern "C" int EXP_LVL1 CS_newKeyNameEx (char *keyName,int type,int preserve,const char *title,const char *legend)
{
	int st = 0;
	TCHAR tcTitle [128];
	TCHAR tcLegend [128];

	CSt_strNCpy (tcTitle,title,tchrCount (tcTitle));
	CSt_strNCpy (tcLegend,title,tchrCount (tcLegend));
	
	CnewKeyName dlgNewKeyName (static_cast<CnewKeyName::TnewKeyNameTypes>(type));

	dlgNewKeyName.SetPreserveEnable (preserve != 0);
	dlgNewKeyName.SetTitle (tcTitle);
	dlgNewKeyName.SetLegend (tcLegend);
	INT_PTR stDlg = dlgNewKeyName.DoModal ();
	if (stDlg == IDOK)
	{
		CString tempStr = dlgNewKeyName.GetNewName ();
		CSt_strNCpy (keyName,(LPCTSTR)tempStr,cs_KEYNM_DEF);
		st = dlgNewKeyName.GetPreserve () ? 1 : 0;
	}
	else
	{
		st = -1;
	}
	return st;
}
/**********************************************************************
**	CnewKeyName (type,pParent);
**	CString GetNewKeyName (void);
**	bool GetPreserve (void);
**
**	Dialog used to acquire a new key name.  The type of key name
**	is specified at construction time.  Type is necessary so that
**	an appropriate check for an existing name can be made.
**
**	Preserve is the status of the "Preserve Existing Data" check
**	box.  Of course, you use GetNewKeyName to obtain the key
**	name the user entered if DoDialog () returns IDOK.
**********************************************************************/

/////////////////////////////////////////////////////////////////////////////
// CnewKeyName dialog

CnewKeyName::CnewKeyName(TnewKeyNameTypes type,CWnd* pParent)
	: CDialog(CnewKeyName::IDD, pParent)
{
	//{{AFX_DATA_INIT(CnewKeyName)
	m_NewKeyName = _T("");
	m_Preserve = TRUE;
	//}}AFX_DATA_INIT
	m_PreserveEnabled = true;
	m_Type = type;
	m_Title = _T("New Dictionary Entry Key Name");
	m_Legend = _T("User defined key names usually require a colon character in the name.  ")
			   _T("While case is preserved for display purposes, it is NOT used to distinguish one name from another.");
}
BOOL CnewKeyName::OnInitDialog ()
{
	CWnd *wp;

	CDialog::OnInitDialog ();

	// Disable the Help button if Help is not available
	wp = GetDlgItem (IDCHELP);
	if (wp != NULL) wp->EnableWindow (CS_isHlpAvailable ());
	wp = GetDlgItem (IDC_PRESERVE);
	if (wp != NULL) wp->EnableWindow (m_PreserveEnabled);
	wp = GetDlgItem (IDC_KYNM_LEGEND);
	if (wp != NULL) wp->SetWindowText ((LPCTSTR)m_Legend);
	SetWindowText (m_Title);
	return (TRUE);
}
void CnewKeyName::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CnewKeyName)
	DDX_Text(pDX, IDC_NEWKEYNAME, m_NewKeyName);
	DDV_MaxChars(pDX, m_NewKeyName, 23);
	DDX_Check(pDX, IDC_PRESERVE, m_Preserve);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CnewKeyName, CDialog)
	//{{AFX_MSG_MAP(CnewKeyName)
	ON_BN_CLICKED(IDCHELP, OnChelp)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CnewKeyName message handlers
void CnewKeyName::OnOK() 
{
	char keyName [32];
	
	if (!UpdateData (TRUE)) return;
	// Verify that the new meets other requirements.
	CSt_strNCpy (keyName,(LPCTSTR)m_NewKeyName,sizeof (keyName));
	keyName [sizeof (keyName) - 1] = '\0';
	if (CS_nampp (keyName) != 0)
	{
		// Show dialog box error and return.
		// By not calling CDialog::OnOK, the dialog box remains active.
		TCHAR errMesg [256];
		CSt_errmsg (errMesg,tchrCount (errMesg));
		AfxMessageBox (errMesg);
		return;
	}

	// Note, CS_nampp doesn't know that this is a new name, but we do.  So we
	// enforce the cs_Unique restriction if active.
	if (cs_Unique != '\0' && strchr (keyName,cs_Unique) == NULL && m_Type != NonDict)
	{
		char errMsg [256];
		TCHAR tcTemp [256];
		sprintf (errMsg,"Keynames of user definitions must contain the '%c' character to avoid name collisions with\nupdate distributions.  Please adjust name appropriately.",cs_Unique);
		CSt_strNCpy (tcTemp,errMsg,tchrCount (tcTemp));
		AfxMessageBox (tcTemp,MB_OK | MB_ICONEXCLAMATION);
		return;
	}

	// Verify that the new name does not already exist.
	switch (m_Type) {
	case CoordSys:
		if (CS_csIsValid (keyName))
		{
			AfxMessageBox (_T("Coordinate system with this name already exists.\nPlease choose another name."),MB_OK | MB_ICONEXCLAMATION);
			return;
		}
		break;
	case Datum:
		if (CS_dtIsValid (keyName))
		{
			AfxMessageBox (_T("Datum with this key name already exists.\nPlease choose another name."),MB_OK | MB_ICONEXCLAMATION);
			return;
		}
		break;
	case Ellipsoid:
		if (CS_elIsValid (keyName))
		{
			AfxMessageBox (_T("Ellipsoid with this key name already exists.\nPlease choose another name."),MB_OK | MB_ICONEXCLAMATION);
			return;
		}
		break;
	case NonDict:
	default:
		break;
	}
	// If we're still here, we have a valid name.
	CDialog::OnOK();
}
void CnewKeyName::OnChelp()
{
	CSwinhlp (m_hWnd,csHLPID_NEWKEYNM);
}
#endif
