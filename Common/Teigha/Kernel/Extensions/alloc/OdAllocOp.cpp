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

#include "OdAllocOp.h"

ODRX_ALLOC_OPERATORS();
//ODRX_CRTDBG_MAP_ALLOC_OPERATORS();















//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//#include <stdlib.h>
//#include "OdAllocExport.h"
//#include "OdAlloc.h"
//#include <new>
//
//// Duplicate symbol link errors result using PocketPC if we define our own new & delete
//#if !defined(_WIN32_WCE) 
//
//#ifdef _MSC_VER
//#pragma warning(disable: 4290)  // C++ exception spec. ignored
////#define ALLOCDLL_OPERATOR_EXPORT
//#else
////#define ALLOCDLL_OPERATOR_EXPORT ALLOCDLL_EXPORT 
//#endif
//
//void* operator new (size_t size) throw(std::bad_alloc)
//{
//  void* p = ::odrxAlloc(size);
//#ifndef __GNUC__
//  // Under GCC std::allocator allocates zero sizes via operator new (by default)
//  if ( !p && size ) throw std::bad_alloc();
//#endif
//  return p;
//}
//
//void operator delete (void *p) throw()
//{
//  if (!p) return;
//  ::odrxFree(p);
//}
//
//void* operator new[] (size_t size) throw(std::bad_alloc)
//{
//  void* p = ::odrxAlloc(size);
//#ifndef __GNUC__
//  // Under GCC std::allocator allocates zero sizes via operator new (by default)
//  if ( !p && size) throw std::bad_alloc();
//#endif
//  return p;
//}
//
//void operator delete[] (void *p) throw()
//{
//  if (!p) return;
//  ::odrxFree(p);
//}
//
//#if _MSC_VER > 1800 || __GNUC__ >= 5//VS2015 and newer or GCC 5 and newer (support sized deallocation)
//void operator delete(void* p, std::size_t)
//{
//  if (!p) return;
//  ::odrxFree(p);
//}
//
//void operator delete[](void* p, std::size_t)
//{
//  if (!p) return;
//  ::odrxFree(p);
//}
//#endif
//
//
//void *operator new(size_t size, const std::nothrow_t&) throw()
//{
//  return operator new(size);
//}
//void * operator new[](::size_t count, const std::nothrow_t&) throw()
//{
//  return operator new[](count);
//}
//void operator delete[](void *ptr, const std::nothrow_t&) throw()
//{
//  operator delete[](ptr);
//}
//void operator delete(void *ptr, const std::nothrow_t&) throw()
//{
//  operator delete(ptr);
//}
//#if _MSC_VER > 1800 || __GNUC__ >= 5//VS2015 and newer or GCC 5 and newer (support sized deallocation)
//void operator delete(void* ptr, std::size_t s, const std::nothrow_t&)
//{
//  operator delete(ptr, s);
//}
//void operator delete[](void* ptr, std::size_t s, const std::nothrow_t&)
//{
//  operator delete[](ptr, s);
//}
//#endif
//
//#endif
//
//// Support _CRTDBG_MAP_ALLOC memory allocation
////
//#if (_MSC_VER >= 1200) && defined(_DEBUG) && defined(_CRTDBG_MAP_ALLOC)
//
//extern "C"
//{
//  extern _CRTIMP void * __cdecl _malloc_dbg(size_t, int, const char *, int);
//}
//
//void* __cdecl operator new(size_t size, int type, const char* fname, int line)
//{
//  return _malloc_dbg(size, type, fname, line);
//}
//
//void* __cdecl operator new[](size_t size, int type, const char* fname, int line)
//{
//  return operator new(size, type, fname, line);
//}
//
//#else //_CRTDBG_MAP_ALLOC
//
//#ifdef _MSC_VER
//#define TD_CDECL __cdecl
//#else
//#define TD_CDECL
//#endif
//
//void* TD_CDECL operator new(size_t size, int, const char*,int)
//{
//  return ::operator new(size);
//}
//
//void* TD_CDECL operator new[](size_t size, int, const char*, int)
//{
//  return ::operator new[](size);
//}
//
//#endif //_CRTDBG_MAP_ALLOC
