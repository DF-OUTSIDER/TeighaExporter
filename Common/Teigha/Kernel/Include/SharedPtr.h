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


#ifndef _SharedPtr_h_Included
#define _SharedPtr_h_Included

#include "TD_PackPush.h"

#include "OdaCommon.h"
#include "OdAlloc.h"

/** \details
    This template class implements SharedPointers for instances not derived from OdRxObject.

    \remarks
    SharedPointers relieve the programmer
    of having to determine when objects are no longer needed, or having to delete them
    them at that time.

    Each object referenced by a SharedPointer (henceforth 'referenced object') 
    has associated with it a reference count; i.e., how many SharedPointers are referencing it.
    When the reference count reaches zero, the referenced object is deleted.

    Corresponding C++ library: TD_Root
    <group Other_Classes> 
*/
template <class T> class OdSharedPtr
{
public:

  OdSharedPtr() : _reference(0), _refCount(0) {}
  
  OdSharedPtr(T* pObject) : _refCount(0)
  { 
    if ((_reference = pObject) != 0)
    {
      _refCount = (int*)::odrxAlloc(sizeof(int*));
      *_refCount = 1;
    }
  }
  
  /** \remarks
    The another OdSharedPtr object is supplied, its reference count, if present, is incremented.
  */
  OdSharedPtr(const OdSharedPtr& other) 
  {
    _reference = other._reference;
    _refCount = other._refCount;
    if (_refCount)
    {
      ++*_refCount;
    }
  }

  /** \remarks
    Decrements the reference count of the object referenced by this
    SharedPointer object.

    When the reference count reaches zero, the referenced object is deleted.
  */
  ~OdSharedPtr()
  {
    if (_refCount && !--*_refCount)
    {
      delete _reference;
      odrxFree(_refCount);
    }
  }

  OdSharedPtr& operator=(const OdSharedPtr &other)
  {
    if (_reference != other._reference)
    {
      if (_refCount && !--*_refCount)
      {
        odrxFree(_refCount);
        delete _reference;
      }

      _reference = other._reference;
      _refCount = other._refCount;
      if (_refCount)
      {
        ++*_refCount;
      }
    }
#ifdef _DEBUG
    else if (_reference != 0)
    {
      // there can't be independent shared pointers on the same object
      ODA_ASSERT_ONCE(_refCount == other._refCount);
      // if the counters are the same - do nothing (the assignment is no-op)
    }
#endif // _DEBUG
    return *this;
  }

  /** \details
    Returns the object referenced by this SharedPointer.
      
    \remarks
    The reference count of the referenced object is unchanged.
  */
  T* operator->() { return _reference; }

  /** \details
      Returns the object referenced by this SharedPointer.  
      
      \remarks
      This SharedPointer object maintains its reference to the object, and the object's
      reference count is not modified.
  */
  T* get() { return _reference; }
  
  /** \details
      Returns the object referenced by this SharedPointer.  
      
      \remarks
      This SharedPointer object maintains its reference to the object, and the object's
      reference count is not modified.
  */
  const T* get() const { return _reference; }
  
  operator T*() { return _reference; }
  operator const T*() const { return _reference; }
  
  /** \details
    Returns the object referenced by this SharedPointer.
      
    \remarks
    The reference count of the referenced object is unchanged.
  */
  const T* operator->() const { return _reference; }
  
  T& operator*(){ return *_reference; }
  
  const T& operator*() const { return *_reference; }

  /** \details
    Returns true if and only if this SharedPointer contains a null reference.
  */
  bool isNull() const {return _reference == 0;}

  /** \details
    Detaches referenced object from the pointer.
    Warning: this method may be used only if reference count is 1 
      (i.e. when this pointer is the single owner of the referenced object)
  */
  T* detach()
  {
    if (!_refCount)
      return 0;
    if (*_refCount > 1)
    {
      ODA_FAIL();
    }
    odrxFree(_refCount);
    _refCount = 0;
    T* ret = _reference;
    _reference = 0;
    return ret;
  }
  /** \details
    Returns the number of references to the object.
  */
  int refCount() const
  {
    return _refCount ? *_refCount : 0;
  }
private: 
  T* _reference;
  int* _refCount;
};

#include "TD_PackPop.h"

#endif // _SharedPtr_h_Included
