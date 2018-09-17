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

// COMErrorCtx.cpp: implementation of the ErrorCtx class.
//
//////////////////////////////////////////////////////////////////////

#include "OdaCommon.h"
#include "COMErrorCtx.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

OdResult ErrorCtx::code() const
{
  return eExtendedError;
}

OdString WinErrorDesc(DWORD err)//, ...)
{
  //va_list argList;
  //va_start(argList, err);

  LPTSTR lpMsgBuf;
  if(::FormatMessage( 
    FORMAT_MESSAGE_ALLOCATE_BUFFER|FORMAT_MESSAGE_FROM_SYSTEM|FORMAT_MESSAGE_IGNORE_INSERTS,
    NULL,
    err,
    MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
    (LPTSTR)&lpMsgBuf,
    0,
    0//&argList
    ))
  {
    
    OdString msg(lpMsgBuf);
    
    // Free the buffer.
    ::LocalFree(lpMsgBuf);
    
    //va_end(argList);
    return msg;
  }
  return OdString::kEmpty;
}

OdString ErrorCtx::description() const
{
  OdString msg = ::WinErrorDesc(m_hr);
  if(msg.isEmpty())
    msg.format(OD_T("OLE error: 0x%08X"), m_hr);
  return msg;
}
