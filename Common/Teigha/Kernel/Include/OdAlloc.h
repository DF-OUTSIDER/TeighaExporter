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


#ifndef _OD_ALLOC_INCLUDED_
#define _OD_ALLOC_INCLUDED_

#include "OdAllocExport.h"

/** \details
    Defines the interface for Teigha memory management.
*/

//DD:EXPORT_ON

#ifdef __cplusplus
  extern "C" {
#endif

  /** \details
    Allocates a block of memory.
    \param nBytes [in]  Number of bytes to allocate.
    \returns
    Returns a pointer to the newly allocated memory block.
    \sa
    Memory Management
  */
  extern ALLOCDLL_EXPORT void* odrxAlloc  (size_t nBytes);
  /** \details
    Reallocates a block of memory.
    \param pMemBlock [in]  Pointer to the previously allocated memory block.
    \param newSize [in]  New size in bytes.
    \param oldSize [in]  Old size in bytes.
    \returns
    Returns a pointer to the newly allocated memory block.
    \sa
    Memory Management
  */
  extern ALLOCDLL_EXPORT void* odrxRealloc(void* pMemBlock, size_t newSize, size_t oldSize);
  /** \details
    Frees a block of memory.
    \param pMemBlock [in]  Pointer to the previously allocated memory block.
    \sa
    Memory Management
  */
  extern ALLOCDLL_EXPORT void  odrxFree   (void* pMemBlock);
  /** \details
      For internal use only.  
  */
  extern ALLOCDLL_EXPORT OdUInt64 GetMemoryUsageInternalInfo(OdUInt32 target);

#ifdef __cplusplus
  }   // extern "C"
#endif

//DD:EXPORT_OFF

#endif 
