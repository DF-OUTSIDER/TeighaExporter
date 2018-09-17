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

// COMErrorCtx.h: interface for the OdCOMErrorCtx class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_COMERRORCTX_H__2F5F2D57_75C1_4783_BD69_2112F3BB028F__INCLUDED_)
#define AFX_COMERRORCTX_H__2F5F2D57_75C1_4783_BD69_2112F3BB028F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TD_PackPush.h"

#include "OdString.h"
#include "OdErrorContext.h"
#include "OleItemHandler.h"

/** <group Win_Classes> 
*/

/** \details
    This class implements a node in an ErrorContext stack.
    
    \remarks
    Each ErrorContext object points to the previous ErrorContext object in the stack.
    
    Library: Source code provided.
    <group Win_Classes> 
*/
class ErrorCtx : public OdErrorContext  
{
  HRESULT m_hr;
  OdSmartPtr<OdErrorContext> init(HRESULT hr)
  {
    m_hr=hr;
    return this;
  }
public:
  /** \details
    Throws an error if and only if hResult specifies a failure.
    
    \remarks
    Calls fire(hResult) if and only if FAILED(hResult).
  */
  static void check(HRESULT hResult)
  {
    if(FAILED(hResult))
      fire(hResult);
  }
  /** \details
    Throws an error for hResult.
    \param hResult [in]  Error code.
    Throws:
    OdOleError(OdRxObjectImpl<ErrorCtx>::createObject()->init(hResult))
  */
  static void fire(HRESULT hResult)
  {
    throw OdError(OdRxObjectImpl<ErrorCtx>::createObject()->init(hResult));
  }
  /** \details
    Throws an error with the Win32LastError code.

    \remarks
    Calls fire(HRESULT_FROM_WIN32(::GetLastError()))
  */
  static void fireWin32LastError()
  {
    fire(HRESULT_FROM_WIN32(::GetLastError()));
  }
  /** \details
    Throws an error with the Win32LastError code if and only if success is false.
    
    \remarks
    Calls fire(HRESULT_FROM_WIN32(::GetLastError())) if and only if success is false.
  */
  static void checkWin32(BOOL success)
  {
    if(!success)
      fire(HRESULT_FROM_WIN32(::GetLastError()));
  }
  OdString description() const;
  OdResult code() const;
  /** \details
    Returns the HRESULT with which this Error Context was initialized. 
  */
    HRESULT hresult() const { return m_hr; }
};

#include "TD_PackPop.h"

#endif // !defined(AFX_COMERRORCTX_H__2F5F2D57_75C1_4783_BD69_2112F3BB028F__INCLUDED_)
