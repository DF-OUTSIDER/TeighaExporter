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

#if !defined(AFX_PREVIEWFILEDLG_H__2A317218_4BFD_490C_89B7_9E5190FFC5D5__INCLUDED_)
#define AFX_PREVIEWFILEDLG_H__2A317218_4BFD_490C_89B7_9E5190FFC5D5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PreviewFileDlg.h : header file
//

#include "TD_PackPush.h"

/////////////////////////////////////////////////////////////////////////////
// CPreviewFileDlg dialog

/** \details
  This class adds drawing preview to the CFileDialog class.
  library: Source code provided. 
  <group Win_Classes> 
*/
class CPreviewFileDlg : public CFileDialog, public OpenWithPreviewDlg
{
	DECLARE_DYNAMIC(CPreviewFileDlg)

public:
  /** \param pPreview [in]  Pointer to Preview object.
    \param fileOpenDialog [in]  True for File Open dialog, false for File SaveAs dialog.
    \param defaultExtension [in]  Default filename extension.
    \param defaultFileName [in]  Default filename.
    \param flags [in]  File Dialog flags.
    \param filters [in]  File filters.
    \param pParentWindow [in]  Pointer to the Parent Window.   
  */
	CPreviewFileDlg(CAbstractPreview *pPreview,
                  BOOL fileOpenDialog,
		              LPCTSTR defaultExtension = NULL,
		              LPCTSTR defaultFileName = NULL,
		              DWORD flags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		              LPCTSTR filters = NULL,
		              CWnd* pParentWindow = NULL);

  long GetFullFileName(LPTSTR fileName, long &buffSize);
  void SetRootPath(LPCTSTR root);
  int  ShowModal();
  void ReleaseDlg();
  long GetAllSelectedFileNames(LPTSTR fileName, long &buffSize);

protected:
	//{{AFX_MSG(CPreviewFileDlg)
	
	/** \details
	  Notification function when the dialog box is initialized.
	*/
	virtual BOOL OnInitDialog();
	/** \details
	  Notification function called when the preview control is clicked.
	*/
 	afx_msg void OnClickedPreview();
	/** \details
	  Notification function called when the previews is to be refreshed.
	*/
	afx_msg void OnPaint();
  /** \details
    Notification function called when the window is sized.
    \param nType [in]  Size type.
    \param cx [in]  Width in pixels.
    \param cy [in]  Height in pixels. 
  */
	afx_msg void OnSize(UINT nType, int cx, int cy);
/*
    Remarks:
    nType will be one of the following:
    
    @table
    Name              Description
    SIZE_MAXIMIZED    Window maximized. 
    SIZE_MINIMIZED    Window minimized. 
    SIZE_RESTORED     Window resized, but neither SIZE_MINIMIZED nor SIZE_MAXIMIZED. 
    SIZE_MAXHIDE      Some other window maximized. 
    SIZE_MAXSHOW      Some other window restored to its former size.    
*/
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

  /** \details
    Notification function called by CFileDialog on receipt of a WM_NOTIFY message.
    \note
    This function always returns TRUE.
  */
  virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
  /** \details
    Notification function called when the filename changes.
    \remarks
    Called by OnNotify().
  */
  virtual void OnFileNameChange();
  /** \details
    Notification function called when the folder changes.
    \remarks
    Called by OnNotify().
  */
  virtual void OnFolderChange();
  /** \details
    Notification function called when the dialog box initialization is done.
    \remarks
    Called by OnNotify().
  */
  virtual void OnInitDone();

protected:

  CButton           m_checkBox;
  CAbstractPreview  *m_pPreview;
  CRect             m_rPreviewRect;
  bool              m_bPreview;
  CRgn              m_Region;
  CString           m_csFileName;

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
#include "TD_PackPop.h"

#endif // !defined(AFX_PREVIEWFILEDLG_H__2A317218_4BFD_490C_89B7_9E5190FFC5D5__INCLUDED_)
