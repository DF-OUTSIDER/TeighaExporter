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


#ifndef _OD_HEAPOPERATORS_INCLUDED_
#define _OD_HEAPOPERATORS_INCLUDED_

#include "OdAlloc.h"
#include <new>

#ifdef __BCPLUSPLUS__ // borland compiler does not support "placement delete"

/** \details
    Defines the new and delete operators for a class derived from the OdRxObject class.
*/
#define ODRX_HEAP_OPERATORS() \
void* operator new(size_t s) { return ::odrxAlloc(s); }\
void operator delete(void* p) { ::odrxFree(p); }\
void* operator new[](size_t s) { return ::odrxAlloc(s); }\
void operator delete[](void* p) { ::odrxFree(p); }\
void *operator new(size_t, void* p) { return p; }\
void *operator new[](size_t, void* p) { return p; }

#elif defined(_MSC_VER) || (defined(__cplusplus) && (__cplusplus > 199711L))
// MSVC ignores exception specs (and gives warning)
// Dynamic exception specifications are deprecated in C++11

/** \details
    Defines the new and delete operators for a class derived from the OdRxObject class.
*/
#define ODRX_HEAP_OPERATORS() \
void* operator new(size_t s)\
{\
  void* p = ::odrxAlloc(s);\
  if ( !p ) throw std::bad_alloc();\
  return p;\
}\
void operator delete(void* p) { ::odrxFree(p); }\
void* operator new[](size_t s)\
{\
  void* p = ::odrxAlloc(s);\
  if ( !p ) throw std::bad_alloc();\
  return p;\
}\
void operator delete[](void* p) { ::odrxFree(p); }\
void *operator new(size_t, void* p) { return p; }\
void operator delete( void*, void* ) {}\
void *operator new[](size_t, void* p) { return p; }\
void operator delete[]( void*, void* ) {}

#else

/** \details
    Defines the new and delete operators for a class derived from the OdRxObject class.
*/
#define ODRX_HEAP_OPERATORS() \
void* operator new(size_t s) throw(std::bad_alloc)\
{\
  void* p = ::odrxAlloc(s);\
  if ( !p ) throw std::bad_alloc();\
  return p;\
}\
void operator delete(void* p) { ::odrxFree(p); }\
void* operator new[](size_t s) throw(std::bad_alloc)\
{\
  void* p = ::odrxAlloc(s);\
  if ( !p ) throw std::bad_alloc();\
  return p;\
}\
void operator delete[](void* p) { ::odrxFree(p); }\
void *operator new(size_t, void* p) throw() { return p; }\
void operator delete( void*, void* ) {}\
void *operator new[](size_t, void* p) throw() { return p; }\
void operator delete[]( void*, void* ) {}

#endif

#if !defined(_MSC_VER) || (_MSC_VER < 1900)

/** \details
    Defines the new and delete operators that generate the exception.
*/
#define ODRX_NO_HEAP_OPERATORS() \
void* operator new(size_t ) throw() { ODA_FAIL(); return 0; }\
void operator delete(void* ) { ODA_FAIL(); throw OdError(eNotApplicable); }\
void* operator new[](size_t ) throw() { ODA_FAIL(); return 0; }\
void operator delete[](void* ) { ODA_FAIL(); throw OdError(eNotApplicable); }

#else // defined(_MSC_VER) && (_MSC_VER >= 1900)

/** \details
    Defines the new and delete operators that generate the exception.
    Prevents warning C4297: destructor or deallocator has a (possibly implicit) non-throwing exception specification.
*/
#define ODRX_NO_HEAP_OPERATORS() \
static void _throwOdError_eNotApplicable() { ODA_FAIL(); throw OdError(eNotApplicable); }\
void* operator new(size_t ) throw() { ODA_FAIL(); return 0; }\
void operator delete(void* ) { _throwOdError_eNotApplicable(); }\
void* operator new[](size_t ) throw() { ODA_FAIL(); return 0; }\
void operator delete[](void* ) { _throwOdError_eNotApplicable(); }

#endif // defined(_MSC_VER) && (_MSC_VER >= 1900)

#ifdef OD_REDEFINE_HEAP_OPERATORS

/** \details
    Defines the new and delete operators that use the new and delete operators of a base class.
*/
#define ODRX_USING_HEAP_OPERATORS(T) \
void* operator new(size_t s) throw() { return T::operator new(s); }\
void operator delete(void* p) { T::operator delete(p); }\
void* operator new[](size_t s) throw() { return T::operator new(s); }\
void operator delete[](void* p) { T::operator delete(p); }

#else

/** \details
    Defines the new and delete operators that use the new and delete operators of a base class.
*/
#define ODRX_USING_HEAP_OPERATORS(T) \
using T::operator new;\
using T::operator delete;\
using T::operator new[];\
using T::operator delete[]

#endif


#endif // _OD_HEAPOPERATORS_INCLUDED_
