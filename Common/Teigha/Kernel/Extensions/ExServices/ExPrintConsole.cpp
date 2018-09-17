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

//#include "StdAfx.h"
//
//#if defined(_MSC_VER) && defined(_WIN32)
//#include <windows.h>
//#endif
//
//#define STL_USING_IOSTREAM
//#define STL_USING_VECTOR
//#include "OdaSTL.h"
//
#include "OdaCommon.h"
//#include "OdToolKit.h"
//#include "ExHostAppServices.h"
//#include "Gs/Gs.h"
//#include "DbAudit.h"
//#include "Gi/TtfDescriptor.h"
//#include "OdStreamBuf.h"
//#include "RxDictionary.h"
//#include "OdFontServices.h"
//#include "OdCharMapper.h"
#include "ExPrintConsole.h"
#include "OdString.h"
#include "OdCharMapper.h"

//
//#define  STD(a)  std:: a

#if defined(ANDROID)
#include <android/log.h>

#define  LOG_TAG    "TeighaJni"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,  LOG_TAG, __VA_ARGS__)
#endif

#ifdef _WINRT
static PrintConsoleInsideFunc g_printConsoleInsideFunc = wprintf;
#else
static PrintConsoleInsideFunc g_printConsoleInsideFunc = NULL;
#endif //_WINRT

/************************************************************************/
/* Sets printConsoleString function                                     */
/************************************************************************/
void odExSetPrintConsoleInsideFunc(PrintConsoleInsideFunc printConsoleFunc)
{
    g_printConsoleInsideFunc = printConsoleFunc;
}

void odPrintConsoleString(const OdChar* fmt, ...) // to stdout
{
  va_list argList;
  va_start(argList, fmt);
  OdString s;
  s.formatV(fmt, argList);
  va_end(argList);
  if (g_printConsoleInsideFunc)
  {
    g_printConsoleInsideFunc(OD_T("%ls"), s.c_str());
    return;
  }
#ifdef ODA_WINDOWS
#ifdef OD_WINDOWS_DESKTOP
  DWORD dw;
  HANDLE o = ::GetStdHandle(STD_OUTPUT_HANDLE);
  if (o != NULL)
  {
    if (::GetFileType(o) == FILE_TYPE_CHAR)
      ::WriteConsoleW(o, s.c_str(), s.getLength(), &dw, 0);
    else
      ::WriteFile(o, (const char*)s, s.getLengthA(), &dw, 0);
  }
#else
//TODO
#endif
#elif defined(ANDROID)
  printf("%s", (const char*)s);
  LOGI("%s", (const char*)s);
#else
  OdAnsiCharArray bf;
  int lenStr = s.getLength();
  bf.reserve(lenStr * sizeof(OdChar) + 1);
  OdCharMapper::unicodeToUtf8(s.c_str(), lenStr, bf);
  printf("%s", bf.asArrayPtr());
#endif
}

void odPrintErrorString(const OdChar* fmt, ...) // to stderr
{
  va_list argList;
  va_start(argList, fmt);
  OdString s;
  s.formatV(fmt, argList);
  va_end(argList);
  if (g_printConsoleInsideFunc)
  {
    g_printConsoleInsideFunc(OD_T("%ls"), s.c_str());
    return;
  }
#ifdef ODA_WINDOWS
#ifdef OD_WINDOWS_DESKTOP
  DWORD dw;
  HANDLE o = ::GetStdHandle(STD_ERROR_HANDLE);
  if (o != NULL)
  {
    if (::GetFileType(o) == FILE_TYPE_CHAR)
      ::WriteConsoleW(o, s.c_str(), s.getLength(), &dw, 0);
    else
      ::WriteFile(o, (const char*)s, s.getLengthA(), &dw, 0);
  }
#else
//TODO
#endif
#elif defined(ANDROID)
  printf("%s", (const char*)s);
  LOGI("%s", (const char*)s);
#else
  OdAnsiCharArray bf;
  int lenStr = s.getLength();
  bf.reserve(lenStr * sizeof(OdChar)  + 1);
  OdCharMapper::unicodeToUtf8(s.c_str(), lenStr, bf);
  fprintf(stderr, "%s" ,bf.asArrayPtr());
#endif
}
