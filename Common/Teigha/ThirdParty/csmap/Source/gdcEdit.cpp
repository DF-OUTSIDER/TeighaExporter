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

#if defined (__MFC__)

#include "cs_map.h"
#include "cs_Legacy.h"
#include "cs_mfc.h"
#include "cs_hlp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern "C" char cs_Dir [];
extern "C" char *cs_DirP;
extern "C" char cs_DirsepC;
extern "C" char cs_ExtsepC;

///////////////////////////////////////////////////////////////////////////////
// Basic API access function.
extern "C" void EXP_LVL1 CS_gdcEditor (char *gdcName)
{
	enum cs_GdcCatalogs ident;
	CgdcEdit gdcEditor;

	CS_recvr ();
	ident = CS_gdcGetIdent (gdcName);
	if (ident != gdcFileNone)
	{
		gdcEditor.SetInitialCatalog (ident);
		gdcEditor.DoModal ();
		ident = gdcEditor.GetCurrentCatalog ();
		if (ident != gdcFileNone)
		{
			CS_stncp (gdcName,CS_gdcGetName (ident),cs_FNM_MAXLEN);
		}
	}
	return;
}
/////////////////////////////////////////////////////////////////////////////
// CgdcEdit dialog
CgdcEdit::CgdcEdit(CWnd* pParent /*=NULL*/)
	: CDialog(CgdcEdit::IDD, pParent)
{
	//{{AFX_DATA_INIT(CgdcEdit)
	m_Density = 0.0;
	m_EntryPath = _T("");
	m_BufferSize = 0;
	m_Fallback = _T("");
	//}}AFX_DATA_INIT
	m_CatChanged = false;
	m_Options = 0UL;
	m_Catalog = 0;
	m_CatalogTable = 0;
}
const char *CgdcEdit::GetGdcName (void)
{
	const char *rtnValue = 0;

	if (m_CatalogTable != 0) rtnValue = m_CatalogTable->gdcName;
	return rtnValue;
}
void CgdcEdit::SetInitialCatalog (enum cs_GdcCatalogs initialGdc)
{
	m_CatalogTable = CS_gdcTable (initialGdc);
}
enum cs_GdcCatalogs CgdcEdit::GetCurrentCatalog ()
{
	enum cs_GdcCatalogs rtnValue;

	rtnValue = gdcFileNone;
	if (m_CatalogTable != 0)
	{
		rtnValue = m_CatalogTable->ident;
	}
	return rtnValue;
}
void CgdcEdit::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CgdcEdit)
	DDX_Control(pDX, IDC_GDC_ENTRYLIST, m_EntryList);
	DDX_Control(pDX, IDC_GDC_CATALOGS, m_CatalogCombo);
	DDX_Text(pDX, IDC_GDC_GRIDCELL, m_Density);
	DDX_Text(pDX, IDC_GDC_PATH, m_EntryPath);
	DDX_Text(pDX, IDC_GDC_BUFRSIZE, m_BufferSize);
	DDX_Text(pDX, IDC_GDC_FALLBACK, m_Fallback);
	DDV_MaxChars(pDX, m_Fallback, 24);
	//}}AFX_DATA_MAP
}
BEGIN_MESSAGE_MAP(CgdcEdit, CDialog)
	//{{AFX_MSG_MAP(CgdcEdit)
	ON_BN_CLICKED(IDC_GDC_ADD, OnGdcAdd)
	ON_BN_CLICKED(IDC_GDC_DELETE, OnGdcDelete)
	ON_BN_CLICKED(IDC_GDC_UPDATE, OnGdcUpdate)
	ON_CBN_SELCHANGE(IDC_GDC_CATALOGS, OnSelchangeGdcCatalogs)
	ON_BN_CLICKED(IDC_GDC_DTBRWSE, OnGdcDtbrwse)
	ON_BN_CLICKED(IDC_GDC_FILEBRWSE, OnGdcFilebrwse)
	ON_LBN_SELCHANGE(IDC_GDC_ENTRYLIST, OnSelchangeGdcEntrylist)
	ON_EN_CHANGE(IDC_GDC_FALLBACK, OnChangeGdcFallback)
	ON_BN_CLICKED(IDC_GDC_BTNDOWN, OnGdcBtndown)
	ON_BN_CLICKED(IDC_GDC_BTNFIRST, OnGdcBtnfirst)
	ON_BN_CLICKED(IDC_GDC_BTNLAST, OnGdcBtnlast)
	ON_BN_CLICKED(IDC_GDC_BTNUP, OnGdcBtnup)
	ON_LBN_DBLCLK(IDC_GDC_ENTRYLIST, OnDblclkGdcEntrylist)
	ON_BN_CLICKED(IDHELP, OnHelp)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
/////////////////////////////////////////////////////////////////////////////
// CgdcEdit message handlers
BOOL CgdcEdit::OnInitDialog () 
{
	BOOL rtnValue;
	int idx;
	int cbSel;
	const char *gdcPtr;
	CString strTemp;

	rtnValue = CDialog::OnInitDialog ();

	// Disable the Help button if help isn't available.
	CWnd *wp = GetDlgItem (IDHELP);
	if (wp != NULL) wp->EnableWindow (CS_isHlpAvailable ());

	m_CatalogCombo.ResetContent ();
	idx = 0;
	cbSel = 0;
	while ((gdcPtr = CS_gdcEnum (idx,0)) != 0)
	{
		CS_stcpy (cs_DirP,gdcPtr);
		if (CS_access (cs_Dir,4) == 0)
		{
			m_CatalogCombo.AddString (gdcPtr);
		}
		idx += 1;
	}
	cbSel = 0;
	if (m_CatalogTable != 0)
	{
		cbSel = m_CatalogCombo.FindStringExact (-1,m_CatalogTable->gdcName);
		if (cbSel == CB_ERR) cbSel = 0;
	}
	m_CatalogCombo.SetCurSel (cbSel);

	OnSelchangeGdcCatalogs ();
	return rtnValue;
}
void CgdcEdit::OnGdcAdd() 
{
	struct csDatumCatalogEntry_ *entPtr;
	struct csDatumCatalogEntry_ entry;
	CgdeEdit gdeEdit;

	gdeEdit.SetBaseDir (cs_Dir);
	entry.pathName [0] = '\0';
	entry.bufferSize = 0;
	entry.density = 0.0;
	entry.flags = 0UL;
	gdeEdit.SetCatalogEntry (&entry,m_CatalogTable);
	INT_PTR st = gdeEdit.DoModal ();
	if (st == IDOK)
	{
		entPtr = CSnewDatumCatalogEntry (gdeEdit.GetRelPath (),gdeEdit.GetRelative (),
															   gdeEdit.GetBufferSize (),
															   gdeEdit.GetOptions (),
															   gdeEdit.GetDensity ());
		if (m_Catalog != 0)
		{
			CSaddEntryDataumCatalog (m_Catalog,entPtr);
			m_CatChanged = true;
			UpdateEntryList (m_EntryList.GetCount ());
		}
	}
}
void CgdcEdit::OnGdcDelete() 
{
	int st;
	int lbSel;

	lbSel = m_EntryList.GetCurSel ();
	if (lbSel != LB_ERR && m_Catalog != 0)
	{
		st = CSdeleteEntryDatumCatalog (m_Catalog,lbSel);
		if (st > 0) m_CatChanged = true;
		if (lbSel > 0) lbSel -= 1;
		UpdateEntryList (lbSel);
	}
}
void CgdcEdit::OnGdcUpdate() 
{
	int lbSel;
	struct csDatumCatalogEntry_ *entPtr;
	CgdeEdit gdeEdit;

	lbSel = m_EntryList.GetCurSel ();
	if (lbSel != LB_ERR && m_Catalog != 0)
	{
		entPtr = CSgetDatumCatalogEntry (m_Catalog,lbSel);
		if (entPtr != 0)
		{
			gdeEdit.SetBaseDir (cs_Dir);
			gdeEdit.SetCatalogEntry (entPtr,m_CatalogTable);
			INT_PTR st = gdeEdit.DoModal ();
			if (st == IDOK)
			{
				m_CatChanged = true;
				CS_stncp (entPtr->pathName,gdeEdit.GetRelPath (),sizeof (entPtr->pathName));
				entPtr->bufferSize = gdeEdit.GetBufferSize ();
				entPtr->density = gdeEdit.GetDensity ();
				entPtr->flags = gdeEdit.GetOptions ();
				entPtr->relative = gdeEdit.GetRelative ();
				OnSelchangeGdcEntrylist(); 
			}
		}
	}
}
void CgdcEdit::OnSelchangeGdcCatalogs ()
{
	int cbIdx;
	CWnd *wp;
	CString strTemp;
	enum cs_GdcCatalogs ident;
	char errMsg [1024];

	// Before we dump this catalog in favor of the newly selected one,
	// update the 'on disk' copy if it was changed.
	UpdateCatalog ();

	// Set up the dialog for the new GDC file.  We
	// write somewhat generically, so that we can use
	// this for initialization as well.
	cbIdx = m_CatalogCombo.GetCurSel ();
	m_CatalogCombo.GetLBText (cbIdx,strTemp);
	ident = CS_gdcGetIdent ((LPCTSTR)strTemp);
	CS_stcpy (cs_DirP,(LPCTSTR)strTemp);
	if (CS_access (cs_Dir,4) == 0)
	{
		if (m_Catalog != NULL)
		{
			CSdeleteDatumCatalog (m_Catalog);
			m_Catalog = 0;
		}
		m_Catalog = CSnewDatumCatalog (cs_Dir);
		if (m_Catalog != 0)
		{
			m_Fallback = CSgetFallbackName (m_Catalog);
			UpdateEntryList (0);
			m_CatalogTable = CS_gdcTable (ident);
		}
		else
		{
			m_CatalogTable = 0;
			m_Fallback.Empty ();
			m_EntryList.ResetContent ();
			CS_errmsg (errMsg,sizeof (errMsg));
			AfxMessageBox (errMsg);
		}
		wp = GetDlgItem (IDC_GDC_DTBRWSE);
		if (wp != NULL) wp->EnableWindow (m_CatalogTable != 0);
		wp = GetDlgItem (IDC_GDC_UPDATE);
		if (wp != NULL) wp->EnableWindow (m_CatalogTable != 0);
		wp = GetDlgItem (IDC_GDC_DELETE);
		if (wp != NULL) wp->EnableWindow (m_CatalogTable != 0);
	}
	UpdateData (FALSE);
	return;
}
void CgdcEdit::UpdateEntryList (int index)
{
	int ii;
	struct csDatumCatalogEntry_ *entPtr;
	
	m_EntryList.ResetContent ();

	// Add the datum catalog entries to the list.
	ii = -1;
	while ((entPtr = CSgetDatumCatalogEntry (m_Catalog,++ii)) != NULL)
	{
		char *cp = strrchr (entPtr->pathName,cs_DirsepC);
		if (cp == NULL) cp = entPtr->pathName;
		else cp += 1;
		m_EntryList.AddString (cp);
	}
	m_EntryList.SetCurSel (index);
	ShowEntry (index);
}
void CgdcEdit::ShowEntry (int entryIdx)
{
	struct csDatumCatalogEntry_*entPtr;
	
	entPtr = 0;
	if (m_Catalog != 0)
	{
		entPtr = CSgetDatumCatalogEntry (m_Catalog,entryIdx);
	}
	if (entPtr != 0)
	{
		m_EntryPath = entPtr->pathName;
		m_BufferSize = entPtr->bufferSize;
		m_Density = entPtr->density * 60;
		m_Options = entPtr->flags;
	}
	else
	{
		m_EntryPath.Empty ();
		m_BufferSize = 0L;
		m_Density = 0.0;
		m_Options = 0UL;
	}
	UpdateData (FALSE);
}
void CgdcEdit::OnGdcDtbrwse() 
{
	int st;
	struct cs_Dtdef_ *dtDef;
	char dtTemp [cs_KEYNM_DEF];

	dtDef = 0;
	CS_stncp (dtTemp,(LPCTSTR)m_Fallback,sizeof (dtTemp));
	do
	{
		st = CS_dtSelector (dtTemp);
		if (st == IDOK)
		{
			st = CheckFallback (dtTemp);
		}
		if (st == IDOK)
		{
			m_Fallback = dtTemp;
			CS_stncp (m_Catalog->fallback,dtTemp,sizeof (m_Catalog->fallback));
			m_CatChanged = true;
			UpdateData (FALSE);
		}
	}
	while (st == IDRETRY);
	return;
}
void CgdcEdit::OnGdcFilebrwse() 
{
}
void CgdcEdit::OnSelchangeGdcEntrylist() 
{
	int lbSel;

	lbSel = m_EntryList.GetCurSel ();
	ShowEntry (lbSel);
}
void CgdcEdit::OnChangeGdcFallback () 
{
	m_CatChanged = true;
}

void CgdcEdit::OnOK() 
{
	int st;

	if (!UpdateData (TRUE)) return;

	// Check the fallback, in case the user changed it via the
	// edit control.
	st = CheckFallback ((LPCTSTR)m_Fallback);
	if (st != IDOK)
	{
		CWnd *wp = GetDlgItem (IDC_GDC_FALLBACK);
		if (wp != 0) GotoDlgCtrl (wp);
		return;
	}	

	// Update the current catalog if it has changed.
	UpdateCatalog ();

	// OK, I guess we can exit.
	CDialog::OnOK();
}
int CgdcEdit::CheckFallback (const char *dtKeyName)
{
	int st;
	short to84Via;
	struct cs_Dtdef_ *dtDef;

	/* It the key name is empty, it's OK.  There is no fallback. */
    	if (*dtKeyName == '\0' || !CS_stricmp (dtKeyName,"<none>"))
	{
		return IDOK;
	}

	/* Something is there, we must validate it. */
	st = IDRETRY;
	dtDef = CS_dtdef (dtKeyName);
	if (dtDef != 0)
	{
		to84Via = dtDef->to84_via;
		CS_free (dtDef);
		dtDef = 0;
		if (to84Via == cs_DTCTYP_MOLO   ||
			to84Via == cs_DTCTYP_MREG   ||
			to84Via == cs_DTCTYP_BURS   ||
			to84Via == cs_DTCTYP_WGS72  ||
			to84Via == cs_DTCTYP_7PARM  ||
			to84Via == cs_DTCTYP_3PARM  ||
			to84Via == cs_DTCTYP_GEOCTR ||
			to84Via == cs_DTCTYP_4PARM  ||
			to84Via == cs_DTCTYP_6PARM  ||
			to84Via == cs_DTCTYP_WGS84)
		{
			st = IDOK;
		}
		else
		{
			AfxMessageBox ("Fallback datum must not be one which relies on a grid data file of any sort.");
			st = IDRETRY;
		}
	}
	else
	{
		AfxMessageBox ("Couldn't locate selected datum in the dictionary.");
		st = IDCANCEL;
	}
	return st;
}
void CgdcEdit::OnGdcBtndown() 
{
	int st;
	int cbSel;

	cbSel = m_EntryList.GetCurSel ();
	if (cbSel != CB_ERR)
	{
		st = CSmoveDownDatumCatalog (m_Catalog,cbSel);
		if (st > 0) m_CatChanged = true;
		cbSel += 1;
		if (cbSel >= m_EntryList.GetCount ())
		{
			cbSel = m_EntryList.GetCount () - 1;
		}
		UpdateEntryList (cbSel);
	}
}
void CgdcEdit::OnGdcBtnfirst() 
{
	int st;
	int cbSel;

	cbSel = m_EntryList.GetCurSel ();
	if (cbSel != CB_ERR)
	{
		st = CSmakeFirstDatumCatalog (m_Catalog,cbSel);
		if (st > 0) m_CatChanged = true;
		UpdateEntryList (0);
	}
}
void CgdcEdit::OnGdcBtnlast() 
{
	int st;
	int cbSel;

	cbSel = m_EntryList.GetCurSel ();
	if (cbSel != CB_ERR)
	{
		st = CSmakeLastDatumCatalog (m_Catalog,cbSel);
		if (st > 0) m_CatChanged = true;
		cbSel = m_EntryList.GetCount () - 1;
		UpdateEntryList (cbSel);
	}
}
void CgdcEdit::OnGdcBtnup()
{
	int st;
	int cbSel;

	cbSel = m_EntryList.GetCurSel ();
	if (cbSel != CB_ERR)
	{
		st = CSmoveUpDatumCatalog (m_Catalog,cbSel);
		if (st > 0) m_CatChanged = true;
		cbSel -= 1;
		if (cbSel < 0) cbSel = 0;
		UpdateEntryList (cbSel);
	}
}
void CgdcEdit::UpdateCatalog ()
{
	if (m_CatChanged && m_CatalogTable != 0)
	{
		int st = AfxMessageBox ("Current catalog has been changed.  Shall we save the changes?",MB_YESNO);
		if (st == IDYES)
		{
			CS_stncp (m_Catalog->fallback,(LPCSTR)m_Fallback,cs_KEYNM_DEF);
			CS_stcpy (cs_DirP,m_CatalogTable->gdcName);
			CSwriteDatumCatalog (m_Catalog,cs_Dir);
		}
	}
	m_CatChanged = false;
	return;
}
void CgdcEdit::OnDblclkGdcEntrylist() 
{
	OnGdcUpdate ();
}
void CgdcEdit::OnHelp() 
{
	CSwinhlp (m_hWnd,csHLPID_GDC);
}
/////////////////////////////////////////////////////////////////////////////
// CgdeEdit dialog
CgdeEdit::CgdeEdit(CWnd* pParent /*=NULL*/)
	: CDialog(CgdeEdit::IDD, pParent)
{
	//{{AFX_DATA_INIT(CgdeEdit)
	m_Absolute = FALSE;
	m_BufferSize = 0;
	m_Density = 0.0;
	m_FilePath = _T("");
	m_FileName = _T("");
	//}}AFX_DATA_INIT
	m_Options = 0UL;
	m_BaseDir [0] = '\0';
	m_RelPath [0] = '\0';
}
void CgdeEdit::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CgdeEdit)
	DDX_Check(pDX, IDC_GDE_ABSOLUTE, m_Absolute);
	DDX_Text(pDX, IDC_GDE_BUFRSIZE, m_BufferSize);
	DDX_Text(pDX, IDC_GDE_DENSITY, m_Density);
	DDX_Text(pDX, IDC_GDE_PATH, m_FilePath);
	DDV_MaxChars(pDX, m_FilePath, 260);
	DDX_Text(pDX, IDC_GDE_FILENAME, m_FileName);
	//}}AFX_DATA_MAP
}
void CgdeEdit::SetBaseDir (const char *baseDir)
{
	char *cp;

	CS_stncp (m_BaseDir,baseDir,sizeof (m_BaseDir));
	cp = m_BaseDir + strlen (m_BaseDir) - 1;
	if (*cp == cs_DirsepC) *cp = '\0';
}
void CgdeEdit::SetCatalogEntry (const struct csDatumCatalogEntry_ *entPtr,const struct cs_GdcCatalogTable_ *table)
{
	char *cp;
	char cTemp [260];

	CS_stncp (cTemp,entPtr->pathName,sizeof (cTemp));
	cp = strrchr (cTemp,cs_DirsepC);
	if (cp == 0)
	{
		m_FileName = cTemp;
		m_FilePath.Empty ();
	}
	else
	{
		*cp++ = '\0';
		m_FileName = cp;
		m_FilePath = cTemp;
	}
	m_BufferSize = entPtr->bufferSize;
	m_Density = entPtr->density * 60.0;
	m_Options = entPtr->flags;
	m_CatalogTable = table;
	m_Absolute = (entPtr->relative == 0);
	return;
}
const char *CgdeEdit::GetFileName (void)
{
	return (LPCTSTR)m_FileName;
}
const char *CgdeEdit::GetPath (void)
{
	return (LPCTSTR)m_FilePath;
}
const char *CgdeEdit::GetRelPath (void)
{
	char *cp;

	cp = m_RelPath;
	if (m_FilePath != m_BaseDir || m_Absolute)
	{
		cp = CS_stcpy (cp,(LPCTSTR)m_FilePath);
		*cp++ = cs_DirsepC;
		CS_stcpy (cp,(LPCTSTR)m_FileName);
	}
	else
	{
		*cp++ = '.';
		*cp++ = cs_DirsepC;
		CS_stcpy (cp,(LPCTSTR)m_FileName);
	}
	return m_RelPath;
}
long CgdeEdit::GetBufferSize (void)
{
	return m_BufferSize;
}
double CgdeEdit::GetDensity (void)
{
	return m_Density / 60.0;
}
unsigned long CgdeEdit::GetOptions (void)
{
	return m_Options;
}
short CgdeEdit::GetRelative (void)
{
	return m_Absolute ? 0 : 1;
}
BEGIN_MESSAGE_MAP(CgdeEdit, CDialog)
	//{{AFX_MSG_MAP(CgdeEdit)
	ON_BN_CLICKED(IDC_GDE_BRWSE, OnGdeBrwse)
	ON_BN_CLICKED(IDHELP, OnHelp)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
/////////////////////////////////////////////////////////////////////////////
// CgdeEdit message handlers
void CgdeEdit::OnGdeBrwse() 
{
	DWORD ofnFlags;
	CString extension;

	ofnFlags = OFN_FILEMUSTEXIST | OFN_HIDEREADONLY | OFN_NOCHANGEDIR;
	CFileDialog fileFind (TRUE,0,(LPCTSTR)m_FileName,ofnFlags,m_CatalogTable->filterW,NULL);
	fileFind.m_ofn.lpstrTitle = "Select Geodetic Data File";

	if (fileFind.DoModal () == IDOK)
	{
		m_FileName = fileFind.GetFileName ();
		m_FilePath = fileFind.GetPathName ();
		int pathLen = m_FilePath.ReverseFind (L'\\');
		if (pathLen > 0) m_FilePath = m_FilePath.Left (pathLen);
		else m_FilePath.Empty ();
		extension = m_FileName.Right (4);
		if (extension.CollateNoCase (".LAS") == 0 || extension.CollateNoCase (".LOS") == 0)
		{
			int length = m_FileName.GetLength ();
			m_FileName.SetAt ((length - 2),'?');
		}
		UpdateData (FALSE);
	}
	return;
}
void CgdeEdit::OnOK() 
{
	int st;
	int length;
	char *cp;
	char lclPath [MAX_PATH];

	if (!UpdateData (TRUE)) return;

	// Verify that the path to the file exists.  We need to play a special
	// game when the file is oneof the .l?s variety.
	cp = CS_stncp (lclPath,(LPCTSTR)m_FilePath,sizeof (lclPath));
	length = static_cast<int>(sizeof (lclPath) - strlen (lclPath) - 1);
	*cp++ = cs_DirsepC;
	length -= 1;
	CS_stncp (cp,m_FileName,length);
	cp = strrchr (lclPath,cs_ExtsepC);
	if (cp != NULL && !CS_stricmp ((cp + 1),"l?s"))
	{
		*(cp + 2) = 'a';
		st = CS_access (lclPath,4);
		*(cp + 2) = 'o';
		st |= CS_access (lclPath,4);
	}
	else
	{
		st = CS_access (lclPath,4);
	}
	if (st != 0)
	{
		AfxMessageBox (_T("Indicated grid file(s) does(do) not exist(s) or cannot be read."));
		CWnd *wp = GetDlgItem (IDC_GDE_FILENAME);
		if (wp != 0) GotoDlgCtrl (wp);
		return;
	}

	// Verify a reasonable value for other parameters.
	if (m_BufferSize < 0 || m_BufferSize > (1024 * 1204 * 4))
	{
		AfxMessageBox (_T("Buffer size value is unreasonable.  Please adjust"));
		CWnd *wp = GetDlgItem (IDC_GDE_BUFRSIZE);
		if (wp != 0) GotoDlgCtrl (wp);
		return;
	}
	if (m_Density < 0.0 || m_Density > 60.0)
	{
		AfxMessageBox (_T("Grid cell size value is unreasonable.  Please adjust"));
		CWnd *wp = GetDlgItem (IDC_GDE_DENSITY);
		if (wp != 0) GotoDlgCtrl (wp);
		return;
	}
	CDialog::OnOK();
}
BOOL CgdeEdit::OnInitDialog() 
{
	char *cp;
	size_t length;
	BOOL rtnValue;

	rtnValue = CDialog::OnInitDialog();

	// Disable the Help button if help isn't available.
	CWnd *wp = GetDlgItem (IDHELP);
	if (wp != NULL) wp->EnableWindow (CS_isHlpAvailable ());

	length = strlen (m_BaseDir);
	
	m_Absolute = !CS_strnicmp (m_BaseDir,(LPCTSTR)m_FilePath,length);
	if (m_Absolute)
	{
		cp = strrchr (m_BaseDir,cs_DirsepC);
		if (cp != NULL)
		{
			*cp = '\0';
			length = strlen (m_BaseDir);
			*cp = cs_DirsepC;
			m_Absolute = !CS_strnicmp (m_BaseDir,(LPCTSTR)m_FilePath,length);
		}
	}
	UpdateData (FALSE);
	return rtnValue;
}

void CgdeEdit::OnHelp() 
{
	CSwinhlp (m_hWnd,csHLPID_GDE);
}

#endif
