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

// FileDlgExt.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "FileDlgExt.h"
#include "PreviewFileDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CFileDlgExtApp
// See FileDlgExt.cpp for the implementation of this class
//

class CFileDlgExtApp : public CWinApp
{
public:
	CFileDlgExtApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFileDlgExtApp)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CFileDlgExtApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////
// CFileDlgExtApp

BEGIN_MESSAGE_MAP(CFileDlgExtApp, CWinApp)
	//{{AFX_MSG_MAP(CFileDlgExtApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFileDlgExtApp construction

CFileDlgExtApp::CFileDlgExtApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CFileDlgExtApp object

CFileDlgExtApp theApp;
HINSTANCE  _savedhInstance;

bool DLGEXT_EXPORT  CreateOpenWithPreviewDlg(CAbstractPreview *pView, HWND /*hParent*/, LPCTSTR lpszFileName, 
                                             LPCTSTR lpszFilter, DWORD dwFlags, OpenWithPreviewDlg **pDlg)
{
  AFX_MANAGE_STATE(AfxGetStaticModuleState());

  CWnd    wParent;
  *pDlg = NULL;
  
  _savedhInstance = AfxGetResourceHandle();
  //wParent.Attach(hParent);
  try
  {
    //AfxSetResourceHandle(_savedhInstance);
    CPreviewFileDlg   *pNew = new CPreviewFileDlg(pView, TRUE, NULL, lpszFileName, dwFlags, lpszFilter);
    *pDlg = static_cast<OpenWithPreviewDlg *>(pNew);
  }
  catch (...)
  {
    return false;
  }

  return true;
}
