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

/*
*/

#ifndef  _FILE_DLG_EXT_H_
#define  _FILE_DLG_EXT_H_

#if !defined(FILEEXT_EXPORT)
#define DLGEXT_EXPORT

#else
#define DLGEXT_EXPORT __declspec(dllexport)

#endif  FILEEXT_EXPORT

#include "TD_PackPush.h"

/** \details
  This class is the platform-independent base class for classes that add drawing preview to dialog boxes.
  library: Source code provided. 
  <group Win_Classes> 
*/
class OpenWithPreviewDlg
{
public:
  /** \details
    Returns the full path of the selected file from this dialog box object.
    \param fileName [out]  Receives the filename.
    \param buffSize [in/out] Specifies/receives the length of the full filename.
  */
  virtual long GetFullFileName(LPTSTR fileName, long& buffSize) = 0;
  /** \details
    Sets the initial folder for this dialog box object.
    \param root [in]  Initial folder.
  */
  virtual void SetRootPath(LPCTSTR root) = 0;
  /** \details
    Displays this modal dialog box object.
    \remarks
    Returns the status of the dialog box.
  */
  virtual int  ShowModal() = 0;
  /** \details
    Releases this dialog box object.
  */
  virtual void ReleaseDlg() = 0;
  /** \details
    Returns multiple selected file names through "|" symbol. Call with fileNames = NULL
    returns needed length of the buffer.
  */
  virtual long GetAllSelectedFileNames(LPTSTR fileNames, long& buffSize) = 0;
};


/** \details
  This class is the platform-independent base class for classes that drawing preview. 
  library: Source code provided. 
  <group Win_Classes> 
*/
class CAbstractPreview
{
public:
  /** \details
    Sets the filename of the preview file for this Abstract Preview object.
    \param fileName [in]  Filename.
  */
  virtual void  SetPreviewFile(LPCTSTR fileName) = 0;

  /** \details
    Draws the preview for this Abstract Preview object..
    \param DC [in]  Handle of the device context.
    \param x [in]  Horizontal position of the upper-left corner.
    \param y [in]  Vertical position of the upper-left corner.
    \param width [in]  Width in pixels.
    \param height [in]  Height in pixels.
    \remarks
    x and y increase left to right, top to bottom.
  */
	virtual void  DrawPreview(HDC DC,int x,int y,int width, int height) = 0;
};

typedef bool (*ODA_OPEN_DLGPROC)(CAbstractPreview *pView, HWND parent, LPCTSTR fileName,
                            LPCTSTR filter, DWORD flags, OpenWithPreviewDlg **pDlg);


#ifdef __cplusplus
extern "C" {
#endif 

bool DLGEXT_EXPORT  CreateOpenWithPreviewDlg(CAbstractPreview *pView, HWND parent, LPCTSTR fileName,
                                             LPCTSTR filter, DWORD flags, OpenWithPreviewDlg **pDlg);

#ifdef __cplusplus
}
#endif 

#include "TD_PackPop.h"

#endif _FILE_DLG_EXT_H_
