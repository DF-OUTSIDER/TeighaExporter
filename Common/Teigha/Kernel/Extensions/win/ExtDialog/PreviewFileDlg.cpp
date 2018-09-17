/////////////////////////////////////////////////////////////////////////////// 
// Copyright (C) 2002-2018, Open Design Alliance (the "Alliance"). 
// All rights reserved. 
// 
// This software and its documentation and related materials are owned by 
// the Alliance. The software may only be incorporated into application 
// programs owned by members of the Alliance, subject to a signed 
// Membership Agreement and Supplemental Software License Agreement with the
// Alliance. The structure and organization of this software are the valuable  
// trade secrets of the Alliance and its suppliers. The software is also 
// protected by copyright law and international treaty provisions. Application  
// programs incorporating this software must include the following statement 
// with their copyright notices:
//   
//   This application incorporates Teigha(R) software pursuant to a license 
//   agreement with Open Design Alliance.
//   Teigha(R) Copyright (C) 2002-2018 by Open Design Alliance. 
//   All rights reserved.
//
// By use of this software, its documentation or related materials, you 
// acknowledge and accept the above terms.
///////////////////////////////////////////////////////////////////////////////

// PreviewFileDlg.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "FileDlgExt.h"
#include "PreviewFileDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#define  IDC_CHECK_PREVIEW  6002
extern HINSTANCE  _savedhInstance;

//////////////////////////////////////////////////////////////////////////////
//   interface functions
void CPreviewFileDlg::SetRootPath(LPCTSTR pRoot)
{
  m_ofn.lpstrInitialDir = pRoot;
  return;
}

long CPreviewFileDlg::GetFullFileName(LPTSTR pPathBuffer, long &nBuffSize)
{
  long     nInSize = nBuffSize;
  CString  sOutPath;

  sOutPath = GetPathName();
  nBuffSize = sOutPath.GetLength();
  if (NULL != pPathBuffer)
  {
    if (nInSize >= nBuffSize + 1)
    {
      _tcscpy(pPathBuffer, (LPCTSTR)sOutPath);
      return nBuffSize;
    }
  }
  return 0;
}

int  CPreviewFileDlg::ShowModal()
{
  CPreviewFileDlg  *pDlg = static_cast<CPreviewFileDlg *>(this);

  return pDlg->DoModal();
}

void CPreviewFileDlg::ReleaseDlg()
{
  delete this;
  return;
}

long CPreviewFileDlg::GetAllSelectedFileNames(LPTSTR fileName, long &buffSize)
{
  long ctr = 0;
  if (fileName == NULL)
  {
    buffSize = 0;
    POSITION posFileName = GetStartPosition();
    while (posFileName != NULL)
    {
      CStringW fileName = GetNextPathName(posFileName);
      long len = fileName.GetLength();
      if (len > 0)
      {
        buffSize += fileName.GetLength() + 1;
        ++ctr;
      }
    }
    buffSize += 1;
  }
  else
  {
    TCHAR *res = fileName;
    *res = 0;
    long fullLen = 0;

    long ctr = 0;
    POSITION posFileName = GetStartPosition();
    while (posFileName != NULL)
    {
      CStringW fileName = GetNextPathName(posFileName);
      long len = fileName.GetLength();
      if (len > 0 && (fullLen + len + 1) < buffSize)
      {
        if (fullLen > 0)
        {
          wcscat(res, L"|");
          ++fullLen;
        }

        wcscat(res, fileName);
        fullLen += len;
      }
      else
        return ctr;
      ++ctr;
    }
  }
  return ctr;
}

/////////////////////////////////////////////////////////////////////////////
// CPreviewFileDlg

IMPLEMENT_DYNAMIC(CPreviewFileDlg, CFileDialog)

CPreviewFileDlg::CPreviewFileDlg(CAbstractPreview *pPreview, BOOL bOpenFileDialog, LPCTSTR lpszDefExt,
                                 LPCTSTR lpszFileName, DWORD dwFlags, LPCTSTR lpszFilter, CWnd* pParentWnd) :
#if !defined(_MFC_VER) || (_MFC_VER < 0x0900)
		             CFileDialog(bOpenFileDialog, lpszDefExt, lpszFileName, dwFlags, lpszFilter, pParentWnd)
#else
  // In MFC 9.0 new boolean argument added - bVistaStyle. If it is TRUE (by default) dialog templates will doesn't work (MSDN).
		             CFileDialog(bOpenFileDialog, lpszDefExt, lpszFileName, dwFlags, lpszFilter, pParentWnd, 0, FALSE)
#endif
{
  m_pPreview = pPreview;
  m_bPreview = true;

	m_ofn.Flags |= OFN_EXPLORER | OFN_ENABLETEMPLATE;
	m_ofn.hInstance = _savedhInstance;
	m_ofn.lpTemplateName = _T("FILEOPEN_TEMPLATE");
}

BEGIN_MESSAGE_MAP(CPreviewFileDlg, CFileDialog)
	//{{AFX_MSG_MAP(CPreviewFileDlg)
	ON_BN_CLICKED(IDC_CHECK_PREVIEW,OnClickedPreview)
	ON_WM_PAINT()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


BOOL CPreviewFileDlg::OnInitDialog()
{
	CFileDialog::OnInitDialog();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CPreviewFileDlg::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	ASSERT(pResult != NULL);

	// allow message map to override

	OFNOTIFY* pNotify = (OFNOTIFY*)lParam;
	switch(pNotify->hdr.code)
	{
	case CDN_SELCHANGE:
		OnFileNameChange();
		return TRUE;
	case CDN_FOLDERCHANGE:
		OnFolderChange();
		return TRUE;
	case CDN_INITDONE:
		OnInitDone();
		return TRUE;
	}
	return CFileDialog::OnNotify(wParam, lParam, pResult);
}

void CPreviewFileDlg::OnInitDone()
{
  CWnd *pWnd = GetDlgItem(IDC_STATIC_RECT);
	if (pWnd)
	{
		CRect     cr;

		pWnd->GetWindowRect(&cr);
		ScreenToClient(&cr);
		cr.top = cr.bottom+15;
		cr.bottom = cr.top+20;
		cr.left += 40;

		m_checkBox.Create(_T("&Preview"), WS_CHILD|WS_VISIBLE|WS_TABSTOP|BS_AUTOCHECKBOX, cr, this, IDC_CHECK_PREVIEW);
		m_checkBox.SetFont(GetFont());
		m_checkBox.SetCheck(m_bPreview);

		pWnd->GetWindowRect(&cr);
		ScreenToClient(&cr);
		cr.top = cr.bottom + 45;
		cr.bottom = cr.top + 90; //102;
		// cr.left += 5; //15;
		cr.right = cr.left + 120;
		m_rPreviewRect = cr;
		m_Region.CreateRectRgn(m_rPreviewRect.left+1, m_rPreviewRect.top+1, m_rPreviewRect.right-1, m_rPreviewRect.bottom-1);
	}
}


void CPreviewFileDlg::OnFileNameChange()
{
  if (m_bPreview) // && !m_bChanged
  {
   	// to avoid bug in MFC in GetPathName()
    TCHAR name[65536];
    _tcscpy(name, GetPathName());

     m_pPreview->SetPreviewFile(name);
     InvalidateRect(&m_rPreviewRect);
  }
}

void CPreviewFileDlg::OnClickedPreview()
{
  CButton  *pBtn;

  pBtn = (CButton *)GetDlgItem(IDC_CHECK_PREVIEW);

  m_bPreview = pBtn->GetCheck() == 1 ? TRUE : FALSE;
  if (m_bPreview)
		m_pPreview->SetPreviewFile(GetPathName());
  InvalidateRect(&m_rPreviewRect);
}

void CPreviewFileDlg::OnFolderChange()
{
	m_pPreview->SetPreviewFile(CString(_T("")));
	InvalidateRect(&m_rPreviewRect);
}

void CPreviewFileDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	dc.Draw3dRect(&m_rPreviewRect, ::GetSysColor(COLOR_BTNSHADOW), ::GetSysColor(COLOR_BTNHILIGHT));
	if (m_bPreview)
	{
		dc.SelectClipRgn(&m_Region);
		m_pPreview->DrawPreview((HDC)dc, m_rPreviewRect.left + 1, m_rPreviewRect.top + 1,
                                     m_rPreviewRect.Width() - 2, m_rPreviewRect.Height() - 2);
		dc.SelectClipRgn(NULL);
	}
  return;
}

void CPreviewFileDlg::OnSize(UINT nType, int cx, int cy)
{
	CFileDialog::OnSize(nType, cx, cy);

  if ( m_Region.m_hObject )
  {
    CRect   cr;
    CWnd    *pWnd = GetDlgItem(IDC_STATIC_RECT);

    pWnd->GetWindowRect(&cr);
    ScreenToClient(&cr);
    cr.top = cr.bottom + 45;
    cr.bottom = cr.top + 90; //102;
    // cr.left += 5; //15;
    cr.right = cr.left + 120;
    m_rPreviewRect = cr;
    m_Region.SetRectRgn(m_rPreviewRect.left+1, m_rPreviewRect.top+1, m_rPreviewRect.right-1, m_rPreviewRect.bottom-1);

    Invalidate();
  }
  return;
}


