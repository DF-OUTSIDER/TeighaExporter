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

#ifndef __DWF_OBJECTCONTAINER_H__
#define __DWF_OBJECTCONTAINER_H__

#include "OdArray.h"
#include "RxObjectImpl.h"
#include "DwfObjectIterator.h" // #include "DwfObject.h"
#define STL_USING_ALGORITHM
#include "OdaSTL.h"

//----------------------------------------------------------
//
// Internal data structure for storing IDs
// (at present is a simple array)
//

typedef OdDwfObjectIdArray DwfIdContainer;
class DwfIdContainerType
{
public:
  typedef OdDwfObjectId item;
  typedef DwfIdContainer container;
  typedef DwfIdContainer::iterator iterator;
  typedef DwfIdContainer::const_iterator const_iterator;

  static bool isDone(const container* pContainer, const const_iterator& iter)
  { 
    return (iter == pContainer->end() || iter < pContainer->begin());
  }
  static const_iterator find(const container* pContainer, const item& id)
  {
    return std::find(pContainer->begin(), pContainer->end(), id);
  }
  static bool itemIsErased(const const_iterator& iter)
  {
    return (*iter).isErased();
  }
};

#define ITER_IS_DONE() TContainerType::isDone(m_pContainer, m_iter)
#define ITER_FIND(id) TContainerType::find(m_pContainer, id)

//----------------------------------------------------------
//
// OdBaseIterator
// Implementation of iterator's base methods.
//

class DwfBaseIteratorDummyInterface
{};

template < class TItem,
           class TContainerType,
           class TInterface = DwfBaseIteratorDummyInterface >
class DwfBaseIterator : public TInterface
{
public:
  void start(bool bForward = true, bool bSkipDeleted = true)
  {
    ODA_ASSERT(m_pContainer);
    if (bForward)
    {
      m_iter = m_pContainer->begin();
    }
    else
    {
      m_iter = m_pContainer->end();
      if (m_pContainer->size())
      {
        --m_iter;
      }
    }
    if (bSkipDeleted)
    {
      skipDeleted(bForward);
    }
  }

  bool done() const
  {
    return ITER_IS_DONE();
  }

  bool seek(const TItem& id)
  {
    m_iter = ITER_FIND(id);
    return !ITER_IS_DONE();
  }

  void step(bool bForward = true, bool bSkipDeleted = true)
  {
    if (!ITER_IS_DONE())
    {
      if (bForward)
        ++m_iter;
      else
        --m_iter;

      if (bSkipDeleted)
      {
        skipDeleted(bForward);
      }
    }
  }

  TItem item() const
  {
    return ITER_IS_DONE() ? TItem(0) : *m_iter;
  }

protected:
  // Constructors
  DwfBaseIterator()
    : m_pContainer((OD_TYPENAME TContainerType::container*)0)
  {}

  DwfBaseIterator(const typename TContainerType::container* pContainer)
    : m_pContainer(pContainer)
  {}

  // Support method
  virtual void skipDeleted(bool bForward)
  {
    if (bForward)
    {
      while (!ITER_IS_DONE() && TContainerType::itemIsErased(m_iter))
      {
        ++m_iter;
      }
    }
    else
    {
      while (!ITER_IS_DONE() && TContainerType::itemIsErased(m_iter))
      {
        --m_iter;
      }
    }
  }

  const OD_TYPENAME TContainerType::container* m_pContainer;
  OD_TYPENAME TContainerType::const_iterator m_iter;
};

//----------------------------------------------------------
//
// OdDwfObjectContainer
// Internal inline pared-down interface to IDs container
//

class DWFDBEXPORT OdDwfObjectContainer : public DwfIdContainer
{
public:
  class iterator : public DwfBaseIterator<OdDwfObjectId, DwfIdContainerType>
  {
  public:
    iterator(const OdDwfObjectContainer* pContainer)
      : DwfBaseIterator<OdDwfObjectId, DwfIdContainerType>(pContainer)
    {}
  };

  OdDwfObjectContainer()
  {}
  void append(const OdDwfObjectId& id)
  {
    push_back(id);
  }
  void remove(const OdDwfObjectId& id)
  {
    DwfIdContainer::remove(id);
  }
  OdUInt32 size() const
  {
    return DwfIdContainer::size();
  }
  OdDwfObjectId at(OdUInt32 i) const
  {
    return DwfIdContainer::at(i);
  }
  void clear()
  {
    DwfIdContainer::clear();
  }
  OdDwfObjectIteratorPtr createIterator(bool atBeginning = true, bool skipDeleted = true) const;
};

#endif //__DWF_OBJECTCONTAINER_H__
