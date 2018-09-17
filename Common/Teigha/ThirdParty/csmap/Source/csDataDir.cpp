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

extern "C" char cs_DirsepC;
extern "C" char cs_Dir [];
extern "C" char *cs_DirP;
extern "C" char cs_Csname [];
extern "C" char cs_Dtname [];
extern "C" char cs_Elname [];


/**********************************************************************
**	CcsDataDir csDataDir;
**		extern "C" CS_csDataDir ();
**********************************************************************/

///////////////////////////////////////////////////////////////////////////////
// 'C' callable DataDir function.  Name is not mangled for ease of access from
//	Visual Basic, et. al.
extern "C" int EXP_LVL1 CS_csDataDir (int flags)
{
	int rtnVal;

	CS_recvr ();
	CcsDataDir csDataDir;
	if (flags & 1) csDataDir.EnableINIBox ();
	if (flags & 2) csDataDir.EnableRegBox ();
	csDataDir.DoModal ();
	rtnVal = 0;
	if (csDataDir.GetIniSave ()) rtnVal |= 1;
	if (csDataDir.GetRegSave ()) rtnVal |= 2;
	return rtnVal;
}
/////////////////////////////////////////////////////////////////////////////
// CcsDataDir dialog
CcsDataDir::CcsDataDir(CWnd* pParent) : CDialog(CcsDataDir::IDD, pParent)
{
	TCHAR tcTemp [MAXPATH];

	//{{AFX_DATA_INIT(CcsDataDir)
	m_CSName = _T("");
	m_DataDirectory = _T("");
	m_ElName = _T("");
	m_DtName = _T("");
	m_SaveInINI = FALSE;
	m_SaveInRegistry = FALSE;
	//}}AFX_DATA_INIT
	m_EnableINI = false;
	m_EnableReg = false;

	CSt_strNCpy (tcTemp,cs_Csname,tchrCount (tcTemp));
	m_CSName = cs_Csname;
	CSt_strNCpy (tcTemp,cs_Dtname,tchrCount (tcTemp));
	m_DtName = tcTemp;
	CSt_strNCpy (tcTemp,cs_Elname,tchrCount (tcTemp));
	m_ElName = tcTemp;
	*cs_DirP = '\0';
	CSt_strNCpy (tcTemp,cs_Dir,tchrCount (tcTemp));
	m_DataDirectory = tcTemp;
}
void CcsDataDir::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CcsDataDir)
	DDX_Text(pDX, IDC_MPDR_CSNAME, m_CSName);
	DDX_Text(pDX, IDC_MPDR_DIRECTORY, m_DataDirectory);
	DDX_Text(pDX, IDC_MPDR_ELNAME, m_ElName);
	DDX_Text(pDX, IDC_MPDR_DTNAME, m_DtName);
	DDX_Check(pDX, IDC_MPDR_SVINI, m_SaveInINI);
	DDX_Check(pDX, IDC_MPDR_SVREGISTRY, m_SaveInRegistry);
	//}}AFX_DATA_MAP
}
BOOL CcsDataDir::OnInitDialog ()
{
	CWnd *wp;

	BOOL rtnValue = CDialog::OnInitDialog ();

	// Hide the check boxes as is appropriate.
	wp = GetDlgItem (IDC_MPDR_SVINI);
	if (wp != NULL) wp->EnableWindow (m_EnableINI);
	wp = GetDlgItem (IDC_MPDR_SVREGISTRY);
	if (wp != NULL) wp->EnableWindow (m_EnableReg);

	// Hide the Help button if Help isn't available.
	wp = GetDlgItem (IDC_MPDR_HELP);
	if (wp != NULL) wp->EnableWindow (CS_isHlpAvailable ());

	return rtnValue;
}
BEGIN_MESSAGE_MAP(CcsDataDir, CDialog)
	//{{AFX_MSG_MAP(CcsDataDir)
	ON_BN_CLICKED(IDC_MPDR_HELP, OnMpdrHelp)
	ON_BN_CLICKED(IDC_BTN_DIRBRWSE, OnBtnDirbrwse)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
/////////////////////////////////////////////////////////////////////////////
// CcsDataDir message handlers
void CcsDataDir::OnBtnDirbrwse() 
{
	int ii;
	TCHAR tcTemp [MAXPATH + 16];
	CString newPath;
	DWORD options = OFN_HIDEREADONLY | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR | OFN_SHAREAWARE;

	CS_stcpy (cs_DirP,cs_Csname);
	CSt_strNCpy (tcTemp,cs_Dir,tchrCount (tcTemp));
	CFileDialog dataDir (TRUE,NULL,tcTemp,options,_T("All Files (*.*)|*.*||"),NULL);

	AfxMessageBox (_T("Windows does not provide a 'directory browser' as a 'common dialog'!\n")
				   _T("A 'file browser' dialog will next appear.  Use it to specify a directory\n")
				   _T("by choosing a file (any file) in the desired directory."));
	INT_PTR st = dataDir.DoModal ();
	if (st == IDOK)
	{
		newPath = dataDir.GetPathName ();
		ii = newPath.ReverseFind (cs_DirsepC);
		if (ii >= 0)
		{
			m_DataDirectory = newPath.Left (ii);
			UpdateData (FALSE);
		}
	}
}
void CcsDataDir::OnOK ()
{
	TCHAR* cp;
	CWnd *wp;
	TCHAR tcTemp [MAXPATH];

	if (!UpdateData (TRUE)) return;

	// Check the provided data.

	cp = CSt_strNCpy (tcTemp,(LPCTSTR)m_DataDirectory,tchrCount (tcTemp));
	if (_tcslen (tcTemp) < 3 || tcTemp [1] != _T(':'))
	{
		AfxMessageBox (_T("Full path to a directory must be provided."));
		wp = GetDlgItem (IDC_MPDR_DIRECTORY);
		if (wp != NULL) GotoDlgCtrl (wp);
		return;
	}
	if (*(cp - 1) == _T('\\'))
	{
		cp -= 1;
		*cp = _T('\0');
	}
	if (CSt_access (tcTemp,0) != 0)
	{
		AfxMessageBox (_T("Directory provided does not exist."));
		wp = GetDlgItem (IDC_MPDR_DIRECTORY);
		if (wp != NULL) GotoDlgCtrl (wp);
		return;
	}
	*cp++ = _T('\\');
	CSt_strNCpy (cp,(LPCTSTR)m_CSName,cs_KEYNM_DEF);
	if (CSt_access (tcTemp,4) != 0)
	{
		AfxMessageBox (_T("Coordinate System dictionary file with name provided\n")
					   _T("does not exist in directory provided."));
		wp = GetDlgItem (IDC_MPDR_CSNAME);
		if (wp != NULL) GotoDlgCtrl (wp);
		return;
	}
	CSt_strNCpy (cp,(LPCTSTR)m_DtName,cs_KEYNM_DEF);
	if (CSt_access (tcTemp,4) != 0)
	{
		AfxMessageBox (_T("Datum dictionary file with name provided\n")
					   _T("does not exist in directory provided."));
		wp = GetDlgItem (IDC_MPDR_DTNAME);
		if (wp != NULL) GotoDlgCtrl (wp);
		return;
	}
	CSt_strNCpy (cp,(LPCTSTR)m_ElName,cs_KEYNM_DEF);
	if (CSt_access (tcTemp,4) != 0)
	{
		AfxMessageBox (_T("Ellipsoid dictionary file with name provided\n")
					   _T("does not exist in directory provided."));
		wp = GetDlgItem (IDC_MPDR_ELNAME);
		if (wp != NULL) GotoDlgCtrl (wp);
		return;
	}

	// If we are still here, it all checks out.  We save the stuff in the proper
	// locations.
	CSt_csfnm (m_CSName);
	CSt_dtfnm (m_DtName);
	CSt_elfnm (m_ElName);
	CSt_altdr (m_DataDirectory);

#ifdef __SKIP__
	if (m_SaveInRegistry)
	{
		long32_t reg_st;
		HKEY strtKey = HKEY_LOCAL_MACHINE;
		HKEY softKey = 0;
		HKEY mntrKey = 0;
		HKEY trlnKey = 0;
		HKEY vrsnKey = 0;
	
		reg_st = RegOpenKeyEx (strtKey,cs_RKeySoftware,0,KEY_SET_VALUE,&softKey);
		if (reg_st == ERROR_SUCCESS)
		{
			reg_st = RegOpenKeyEx (softKey,cs_RKeyCompany,0,KEY_SET_VALUE,&mntrKey);
			RegCloseKey (softKey);
		}
		if (reg_st == ERROR_SUCCESS)
		{
			reg_st = RegOpenKeyEx (mntrKey,cs_RKeyProduct,0,KEY_SET_VALUE,&trlnKey);
			RegCloseKey (mntrKey);
		}
		if (reg_st == ERROR_SUCCESS)
		{
			reg_st = RegOpenKeyEx (trlnKey,cs_RKeyVersion,0,KEY_SET_VALUE,&vrsnKey);
			RegCloseKey (trlnKey);
		}
		if (reg_st == ERROR_SUCCESS)
		{
			char uctemp [MAXPATH];
			strcpy (uctemp,(LPCTSTR)m_DataDirectory);
			reg_st = RegSetValueEx (vrsnKey,"DataDir",0,REG_SZ,(unsigned char*)uctemp,strlen (uctemp));
			if (reg_st == ERROR_SUCCESS)
			{
				strcpy (uctemp,(LPCTSTR)m_CSName);
				reg_st = RegSetValueEx (vrsnKey,"CSName",0,REG_SZ,(unsigned char*)uctemp,strlen (uctemp));
			}
			if (reg_st == ERROR_SUCCESS)
			{
				strcpy (uctemp,(LPCTSTR)m_DtName);
				reg_st = RegSetValueEx (vrsnKey,"DtName",0,REG_SZ,(unsigned char *)uctemp,strlen (uctemp));
			}
			if (reg_st == ERROR_SUCCESS)
			{
				strcpy (uctemp,(LPCTSTR)m_ElName);
				reg_st = RegSetValueEx (vrsnKey,"ElName",0,REG_SZ,(unsigned char *)uctemp,strlen (uctemp));
			}
			RegCloseKey (vrsnKey);
		}
	}
	if (m_SaveInINI)
	{
		BOOL ok;
		char ctemp [MAXPATH];
		CS_stncp (ctemp,theApp.m_FullPath,sizeof (ctemp));
		cp = strrchr (ctemp,'.');
		if (cp == NULL) return;
		strcpy (cp,".INI");
		if (CS_access (ctemp,0) != 0)
		{
			FILE* fstr = fopen (ctemp,"w");
			fclose (fstr);
		}
		if (CS_access (ctemp,2) != 0)
		{
			AfxMessageBox ("Write permission to .INI file has been denied.");
			return;
		}
		ok = WritePrivateProfileString ("Directories","DataDir",(LPCTSTR)m_DataDirectory,ctemp);
		if (ok)	ok = WritePrivateProfileString ("Directories","CSName",(LPCTSTR)m_CSName,ctemp);
		if (ok)	ok = WritePrivateProfileString ("Directories","DtName",(LPCTSTR)m_DtName,ctemp);
		if (ok)	ok = WritePrivateProfileString ("Directories","ElName",(LPCTSTR)m_ElName,ctemp);
	}
#endif
	CS_recvr ();
	CDialog::OnOK ();
	return;
}
void CcsDataDir::OnMpdrHelp() 
{
	CSwinhlp (m_hWnd,csHLPID_DATADIR_DLG);
}
#endif
