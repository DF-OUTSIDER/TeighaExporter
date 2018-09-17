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

/************************************************************************/
/* This file implements user-defined memory management.                 */
/************************************************************************/

#include <stdlib.h>
#include "OdAlloc.h"
#include <new>

#if (_MSC_VER >= 1200) && defined(_DEBUG) && defined(_CRTDBG_MAP_ALLOC)
#else
// comment this out to trace memory leaks by standard CRT means
#define _ODA_USE_QUICK_HEAP_
#endif

#ifdef _ODA_USE_QUICK_HEAP_
#include "QuickHeap.h"

static QuickHeap* g_QuickHeap = 0;
static int g_HeapUseCounter = 0;
extern "C" 
{
  ALLOCDLL_EXPORT void* odrxAlloc(size_t nBytes)
  {
    if ( !g_QuickHeap ) g_QuickHeap = new QuickHeap();
    ++g_HeapUseCounter;
    return g_QuickHeap->Alloc(nBytes);
  }

  ALLOCDLL_EXPORT void* odrxRealloc(void* pMemBlock, size_t newSize, size_t /*oldSize*/)
  {
    return g_QuickHeap->ReAlloc( pMemBlock, newSize );
  }

  ALLOCDLL_EXPORT void odrxFree(void* pMemBlock) 
  {
    g_QuickHeap->Free( pMemBlock ); 
    if ( !--g_HeapUseCounter )
    {
      delete g_QuickHeap;
      g_QuickHeap = 0;
    }
  }
} // extern "C"
#else // !_ODA_USE_QUICK_HEAP_
extern "C" 
{
  ALLOCDLL_EXPORT void* odrxAlloc(size_t nBytes)
  {
    return ::malloc( nBytes );
  }

  ALLOCDLL_EXPORT void* odrxRealloc(void* pMemBlock, size_t newSize, size_t /*oldSize*/)
  {
    return ::realloc( pMemBlock, newSize );
  }

  ALLOCDLL_EXPORT void odrxFree(void* pMemBlock) 
  {
    ::free( pMemBlock ); 
  }
} // extern "C"
#endif // !_ODA_USE_QUICK_HEAP_

ALLOCDLL_EXPORT OdUInt64 GetMemoryUsageInternalInfo(OdUInt32 target)
{
  return 1;
}
