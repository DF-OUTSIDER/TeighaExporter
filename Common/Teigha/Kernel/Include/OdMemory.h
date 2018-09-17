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




#ifndef _OD_MEMORY_H_
#define _OD_MEMORY_H_

#define STL_USING_MEMORY
#include "OdaSTL.h"

#include "OdPlatform.h"
#include "OdAlloc.h"

// TEMPLATE CLASS allocator
// Some compilers don't have support for construct & destroy functions.

#ifndef OD_STD_ALLOCATOR
/** \details
    This template class extends the functionality of the standard STL class std::allocator
    for those compilers that not support for construct and destroy.
    
    \remarks
    If construct and destroy are supported, as indicated by #define OD_STD_ALLOCATOR,
    this class is does nothing to std::allocator.
    
    <group Other_Classes>
*/
template<class T>
class OdAllocator : public std::allocator<T>
{
public:
  typedef typename std::allocator<T>::size_type       size_type;
  typedef typename std::allocator<T>::difference_type difference_type;
  typedef typename std::allocator<T>::pointer         pointer;
  typedef typename std::allocator<T>::const_pointer   const_pointer;
  typedef typename std::allocator<T>::reference       reference;
  typedef typename std::allocator<T>::const_reference const_reference;
  typedef typename std::allocator<T>::value_type      value_type;

private:
  inline void constructn(size_type N, pointer ptr, const T& _V = T())
  {
    while(N--) construct(ptr + N, _V);
  }
  inline void destroyn(size_type N, pointer ptr)
  {
    while(N--) destroy(ptr + N);
  }
public:
  /** \details
    Allocates a block of memory sufficient to store the specified number of elements.
    \param N [in]  Number of elements. 
    \returns
    Returns a pointer to the allocated memory.
  */
  pointer allocate(size_type N, const void * = 0)
  {
    pointer res = NULL;
    if (N)
    {
      res = (pointer)::odrxAlloc(N * sizeof(T));
      constructn(N, res);
    }
    return res;
  }
  
  //!DOM For CodeWarrior

  pointer allocate(size_type N, const_pointer = 0)
  {
    pointer res = NULL;
    if (N)
    {
      res = (pointer)::odrxAlloc(N * sizeof(T));
      constructn(N, res);
    }
    return res;
  }

#ifdef _MSC_VER

#if _MSC_VER > 1600
#ifndef _FARQ /* specify standard memory model */
 #define _FARQ
 #define _PDFT ptrdiff_t
 #define _SIZT size_t
#endif
#endif //!DOM _MSC_VER > 1600
  /** \details
    Allocates a block of memory sufficient to store the specified number of characters.
    \param N [in]  Number of characters. 
    \returns
    Returns a pointer to the allocated memory.
    
    \remarks
    Prevents "Freeing Mismatched Memory" errors in Purify.
  */
  char _FARQ *_Charalloc(size_type N)
  {
    return (char _FARQ*)::odrxAlloc(N);
  }
#endif

  /** \details
    Frees a block of memory allocated with allocate.
    \param P [in]  Pointer to the memory.
  */
  void deallocate(void* P, size_type = 0 ) { ::odrxFree(P); }
};

#else
/** \details

    <group Other_Classes>
*/
template<class T>
class OdAllocator : public std::allocator<T>
{};


#endif


#endif //_OD_MEMORY_H_

