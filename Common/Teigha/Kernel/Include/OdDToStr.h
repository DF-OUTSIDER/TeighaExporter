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


#ifndef _OD_DTOSTR_
#define _OD_DTOSTR_

//DD:EXPORT_ON

#include "OdString.h"
#include "OdAnsiString.h"

FIRSTDLL_EXPORT void odDToStr(char* dst, double val, char fmt, int prec, int cropzeros = 0);
FIRSTDLL_EXPORT OdString odDToStr(double val, char fmt, int prec, int cropzeros = 0);
FIRSTDLL_EXPORT double odStrToD(const OdChar* p, OdChar** ppend = 0);
#if defined(_MSC_VER) && _MSC_VER >= 1300
FIRSTDLL_EXPORT inline double odStrToD(const __wchar_t* p, __wchar_t** ppend = 0)
{
  return odStrToD(reinterpret_cast<const OdChar*>(p), reinterpret_cast<OdChar**>(ppend));
}
#endif
FIRSTDLL_EXPORT double odStrToD(const char* p, char** ppend = 0);
FIRSTDLL_EXPORT double odStrToD(const OdString&);
FIRSTDLL_EXPORT double odStrToD(const OdAnsiString&);

//DD:EXPORT_OFF

#endif // _OD_DTOSTR_
