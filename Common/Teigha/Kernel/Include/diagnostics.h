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

#ifndef __DIAGNOSTICS__INCLUDED
#define __DIAGNOSTICS__INCLUDED

//DD:EXPORT_ON

typedef void (*OdAssertFunc) (
    const char* expresssion, 
    const char* filename, 
    int nLineNo);
typedef bool (*OdCheckAssertGroupFunc) (
    const char* group);

typedef void (*OdTraceFunc)(
  const OdChar* debugString);

OdTraceFunc FIRSTDLL_EXPORT odSetTraceFunc(OdTraceFunc traceFunc);
OdAssertFunc FIRSTDLL_EXPORT odSetAssertFunc(OdAssertFunc assertFunc);
OdCheckAssertGroupFunc FIRSTDLL_EXPORT odSetCheckAssertGroupFunc(
                                             OdCheckAssertGroupFunc checkFunc);

void FIRSTDLL_EXPORT OdTrace(
    const OdChar* lpszFormat, ...);
#if defined(_MSC_VER) && _MSC_VER >= 1300
void FIRSTDLL_EXPORT OdTrace(const __wchar_t* lpszFormat, ...);
#endif
void FIRSTDLL_EXPORT OdAssert(
    const char* expresssion, 
    const char* filename, 
    int nLineNo);
void FIRSTDLL_EXPORT OdAssert(
    const char* group,
    const char* expresssion, 
    const char* fileName, 
    int nLineNo);

//DD:EXPORT_OFF

#endif //__DIAGNOSTICS__INCLUDED
