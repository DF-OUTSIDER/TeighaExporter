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




#ifndef __DEBUG_STUFF__INCLUDED
#define __DEBUG_STUFF__INCLUDED


#if defined(_DEBUG) || !defined(NDEBUG)
#define ODA_DIAGNOSTICS
#endif // _DEBUG

#ifdef ODA_DIAGNOSTICS

#include "OdaDefs.h"
#include "RootExport.h"

void FIRSTDLL_EXPORT OdAssert(const char* expresssion, const char* filename, int nLineNo);
void FIRSTDLL_EXPORT OdAssert(const char* group, 
                              const char* expresssion, const char* fileName, int nLineNo);
void FIRSTDLL_EXPORT OdTrace(const OdChar* szFormat, ...);
#if defined(_MSC_VER) && _MSC_VER >= 1300
void FIRSTDLL_EXPORT OdTrace(const __wchar_t* lpszFormat, ...);
#endif

inline void oda_noop(void) {}
#define ODA_ASSERT(exp) ((!(exp)) ? OdAssert(#exp, __FILE__, __LINE__) : oda_noop())
#define ODA_VERIFY(exp) ODA_ASSERT(exp)
#define ODA_ASSERT_ONCE(exp)\
{ static bool was_here = false;\
  if (!was_here && !(exp))\
  { was_here = true;\
    OdAssert(#exp, __FILE__, __LINE__);\
  }\
}
#define ODA_VERIFY_ONCE(exp)\
{ static bool was_here = false;\
  if (!(exp) && !was_here)\
  { was_here = true;\
    OdAssert(#exp, __FILE__, __LINE__);\
  }\
}
#define ODA_FAIL() OdAssert("Invalid Execution.", __FILE__, __LINE__)
#define ODA_FAIL_ONCE()\
{ static bool was_here = false;\
  if (!was_here)\
  { was_here = true;\
    OdAssert("Invalid Execution.", __FILE__, __LINE__);\
  }\
}

#define ODA_FAIL_M(message) (void)( (OdAssert(message, __FILE__, __LINE__), 0) )
#define ODA_FAIL_M_ONCE(message)\
{ static bool was_here = false;\
  if (!was_here)\
  { was_here = true;\
    OdAssert(message, __FILE__, __LINE__);\
  }\
}


#define ODA_ASSERT_X(grp, exp) (void)( (exp) || (OdAssert((const char*)#grp, (const char*)#exp, __FILE__, __LINE__), 0) )
#define ODA_VERIFY_X(grp, exp) ODA_ASSERT_X(grp, exp)
#define ODA_ASSERT_ONCE_X(grp, exp)\
{ static bool was_here = false;\
  if (!was_here && !(exp))\
  { was_here = true;\
    OdAssert(#grp, #exp, __FILE__, __LINE__);\
  }\
}
#define ODA_FAIL_X(grp) OdAssert(#grp, "Invalid Execution.", __FILE__, __LINE__)
#define ODA_FAIL_ONCE_X(grp)\
{ static bool was_here = false;\
  if (!was_here)\
  { was_here = true;\
    OdAssert(#grp, "Invalid Execution.", __FILE__, __LINE__);\
  }\
}
#define ODA_ASSERT_VAR(code) code // to declare variables for assert conditions only
#define ToOdInt8  (val) (ODA_ASSERT((  (OdInt8)val) == val), (OdInt8)  val)
#define ToOdUInt8 (val) (ODA_ASSERT(( (OdUInt8)val) == val), (OdUInt8) val)
#define ToOdInt16 (val) (ODA_ASSERT((( OdInt16)val) == val), (OdInt16) val)
#define ToOdUInt16(val) (ODA_ASSERT(((OdUInt16)val) == val), (OdUInt16)val)
#define ToOdInt32 (val) (ODA_ASSERT(( (OdInt32)val) == val), (OdInt32) val)
#define ToOdUInt32(val) (ODA_ASSERT(((OdUInt32)val) == val), (OdUInt32)val)

#else // ODA_DIAGNOSTICS

#define ODA_ASSERT(condition) 
#define ODA_VERIFY(condition)         (void)(condition)
#define ODA_ASSERT_ONCE(condition)
#define ODA_VERIFY_ONCE(condition)    (void)(condition)
#define ODA_FAIL() 
#define ODA_FAIL_ONCE() 
#define ODA_FAIL_M(message) 
#define ODA_FAIL_M_ONCE(message) 

#define ODA_ASSERT_X(grp, exp)
#define ODA_VERIFY_X(grp, exp)         (void)(exp)
#define ODA_ASSERT_ONCE_X(grp, exp)
#define ODA_FAIL_X(grp)
#define ODA_FAIL_ONCE_X(grp)
#define ODA_ASSERT_VAR(code)

#define ToOdInt8  (val) ((OdInt8)  val)
#define ToOdUInt8 (val) ((OdUInt8) val)
#define ToOdInt16 (val) ((OdInt16) val)
#define ToOdUInt16(val) ((OdUInt16)val)
#define ToOdInt32 (val) ((OdInt32) val)
#define ToOdUInt32(val) ((OdUInt32)val)

#define ODA_NON_TRACING

#endif // ODA_DIAGNOSTICS

#ifndef ODA_NON_TRACING
  #define ODA_TRACE OdTrace
  #define ODA_TRACE0(szFormat) OdTrace(OD_T(szFormat))
  #define ODA_TRACE1(szFormat, param1) OdTrace(OD_T(szFormat), param1)
  #define ODA_TRACE2(szFormat, param1, param2) OdTrace(OD_T(szFormat), param1, param2)
  #define ODA_TRACE3(szFormat, param1, param2, param3) OdTrace(OD_T(szFormat), param1, param2, param3)
#else // ODA_NON_TRACING
  #define ODA_TRACE
  #define ODA_TRACE0(szFormat) 
  #define ODA_TRACE1(szFormat, param1) 
  #define ODA_TRACE2(szFormat, param1, param2) 
  #define ODA_TRACE3(szFormat, param1, param2, param3) 
#endif // ODA_NON_TRACING

// Use this macro to perform compilation time check.
// For example:   ODA_ASSUME(sizeof(double) == 8)
#ifdef __GNUC__
  #define ODA_ASSUME(expr) extern char __attribute((unused)) OdaAssumeArray[expr];
#else
  #define ODA_ASSUME(expr) extern char OdaAssumeArray[expr];
#endif    

#if defined(_MSC_VER)

#pragma warning (push)
#pragma warning ( disable : 4100 )  // Unreferenced formal parameter
#pragma warning ( disable : 4512 )  //assignment operator could not be generated

#include <memory>

// Memory allocation
#if defined(_DEBUG) && defined(_CRTDBG_MAP_ALLOC) && (_MSC_VER >= 1200) && !defined(_ARM_)
#include <malloc.h>
#include <crtdbg.h>
#include "OdError.h"

inline void* operator new(size_t nSize, const char* /*LPCSTR*/ lpszFileName, int nLine)
{
  void* pRes = _malloc_dbg(nSize, _NORMAL_BLOCK, lpszFileName, nLine);
  if(!pRes) throw OdError(eOutOfMemory);
  return pRes;
}

inline void  operator delete(void * pMem, const char* /*LPCSTR lpszFileName*/, int /*nLine*/)
{
  _free_dbg(pMem, _NORMAL_BLOCK);
}

#ifndef DEBUG_NEW
#define DEBUG_NEW new(__FILE__, __LINE__)
#endif

#else //#ifdef _CRTDBG_MAP_ALLOC

//inline void* operator new(size_t nSize) { return Oda::mem_alloc(nSize); }
//inline void  operator delete(void* pMem) { Oda::mem_free(pMem); }

#endif  //_CRTDBG_MAP_ALLOC
#pragma warning (pop)

#include <new.h>

#endif  // _MSC_VER

// Cause Compiler to print a message to output console with File and Line# for 
// Double-Click response
// Ex:
// #pragma MARKMESSAGE("Warning! Implementation is incorrect!")
#ifndef MARKMESSAGE
  #if defined(_MSC_VER)
    #pragma warning (disable:4081)
  #endif
  #if defined(_MSC_VER) && defined(_DEBUG)
    #define _schSTR(x)  #x
    #define _schSTR2(x) _schSTR(x)
    #define MARKMESSAGE(desc) message(__FILE__ "(" _schSTR2(__LINE__) "): " #desc)
  #else
    #if defined(__sun) && !defined(__GNUC__)
      #define MARKMESSAGE(desc) dummy_pragma
    #else
      #define MARKMESSAGE(desc) 
    #endif
  #endif
#endif  // MARKMESSAGE

#if defined(OD_DIAGNOSTICS) && defined(_MSC_VER) && defined(_DEBUG) &&  (_MSC_VER < 1400)
#pragma function(memcpy)
inline void * memcpy(void * dest, const void * src, size_t size)
{ // Memory blocks must not overlap
  ODA_ASSERT(((char*)dest > (char*)src && (char*)dest >= ((char*)src + size)) ||
             ((char*)dest < (char*)src && ((char*)dest + size) <= (char*)src) ||
                     dest == src );
  return memmove(dest, src, size);
}
#endif // _MSC_VER

// _MSC_VER default behaviour is 'If both parameters of atan2 are 0, the function returns 0'
// Other compilers (e.g. BB6) can throw exception.
#if defined(__BORLANDC__) || defined(__BCPLUSPLUS__)
#include <math.h>
  inline double OD_ATAN2(double y, double x)
  {
    if (x == 0. && y == 0.)
      return 0.;
    return atan2(y,x);
  }

#elif defined(_DEBUG) && defined(_MSC_VER) && defined(ODA_LINT)
  FIRSTDLL_EXPORT double OD_ATAN2(double y, double x);
  #define atan2(x, y) Do_not_use_atan2_directly__Use__OD_ATAN2_instead

#else
  #define OD_ATAN2(y,x)  (atan2(y,x)) // Safe for MSVC and hope for others. CoreRegression test should show.
#endif


#if defined(ODA_LINT) && defined(_DEBUG)
  FIRSTDLL_EXPORT double OD_ASIN(double a);
  #define asin(a)   Do_not_use_asin_directly__Use__OD_ASIN_instead

  FIRSTDLL_EXPORT double OD_ACOS(double a);
  #define acos(a)   Do_not_use_acos_directly__Use__OD_ACOS_instead
#else
#define OD_ASIN(a) asin(a)
#define OD_ACOS(a) acos(a)
#endif

#ifdef _MSC_VER
#define OD_WARNING_PUSH warning(push)
#define OD_WARNING_DISABLE(number) warning(disable : number)
#define OD_WARNING_POP  warning(pop)
#define OD_WARNING_SUPPRESS(number)  warning(suppress : number)
#else
  #if defined(__sun) && !defined(__GNUC__)
    #define OD_WARNING_PUSH dummy_pragma
    #define OD_WARNING_DISABLE(number)  dummy_pragma
    #define OD_WARNING_POP  dummy_pragma
    #define OD_WARNING_SUPPRESS(number) dummy_pragma
  #else
    #define OD_WARNING_PUSH
    #define OD_WARNING_DISABLE(number)
    #define OD_WARNING_POP
    #define OD_WARNING_SUPPRESS(number)
  #endif
#endif

#endif // __DEBUG_STUFF__INCLUDED


