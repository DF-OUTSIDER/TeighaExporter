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

#ifndef _ODA_UNIDIRECTIONALLISTT_H_
#define _ODA_UNIDIRECTIONALLISTT_H_

#include "OdArrayMemAlloc.h"

#include "TD_PackPush.h"

template<class T, class A = OdObjectsAllocator<T>, class Mm = OdrxMemoryManager> class OdUnidirectionalList;

/** \details
    This class declares the unidirectional list.
    Corresponding C++ library:  TD_Db
    <group Other_Classes> 
*/
template<class T, class A, class Mm>
class OdUnidirectionalList
{
  struct SNode
  {
    SNode()
    : m_pNext(NULL)
    {
    }

  public:
    SNode* m_pNext;
    T m_value;
  };

  class CmpValues
  {
  public:
    CmpValues(const T& value)
    : m_value(value) 
    {
    }

    bool operator()(const T& value) const
    {
      return (m_value == value);
    }

  protected:
    const T& m_value;
  };

public:
  class ConstIterator
  {
  public:
    ConstIterator(SNode* pNode = NULL)
    : m_pNode(pNode)
    {
    }

    const T& operator*() const
    {
      ODA_ASSERT(m_pNode != NULL);

      return m_pNode->m_value;
    }

    const T* operator->() const
    {
      ODA_ASSERT(m_pNode != NULL);

      return &(m_pNode->m_value);
    }

    ConstIterator& operator++()
    {
      ODA_ASSERT(m_pNode != NULL);

      m_pNode = m_pNode->m_pNext;

      return *this;
    }

    ConstIterator operator++(int)
    {
      ODA_ASSERT(m_pNode != NULL);

      return ConstIterator(m_pNode->m_pNext);
    }

    bool operator==(const ConstIterator& it) const
    {
      return (m_pNode == it.m_pNode);
    }

    bool operator!=(const ConstIterator& it) const
    {
      return (m_pNode != it.m_pNode);
    }

    SNode* node()
    {
      return m_pNode;
    }

    void setNode(SNode* pNode)
    {
      m_pNode = pNode;
    }

  private:
    ConstIterator& operator--()
    {
      return *this;
    }

    ConstIterator operator--(int)
    {
      return ConstIterator();
    }

  protected:
    SNode* m_pNode;
  };

  class Iterator : public ConstIterator
  {
  public:
    Iterator(SNode* pNode = NULL)
    : ConstIterator(pNode)
    {
    }

    T& operator*()
    {
      ODA_ASSERT(ConstIterator::m_pNode != NULL);

      return ConstIterator::m_pNode->m_value;
    }

    T* operator->()
    {
      ODA_ASSERT(ConstIterator::m_pNode != NULL);

      return &(ConstIterator::m_pNode->m_value);
    }

    Iterator& operator++()
    {
      ODA_ASSERT(ConstIterator::m_pNode != NULL);

      ConstIterator::m_pNode = ConstIterator::m_pNode->m_pNext;

      return *this;
    }

    Iterator operator++(int)
    {
      ODA_ASSERT(ConstIterator::m_pNode != NULL);

      return Iterator(ConstIterator::m_pNode->m_pNext);
    }

  private:
    Iterator& operator--()
    {
      return *this;
    }

    Iterator operator--(int)
    {
      return Iterator();
    }
  };

  typedef typename A::size_type size_type;
  typedef Iterator iterator;
  typedef ConstIterator const_iterator;

public:
  OdUnidirectionalList();
  OdUnidirectionalList(const OdUnidirectionalList& list);

  ~OdUnidirectionalList();

  OdUnidirectionalList& operator=(const OdUnidirectionalList& list);

  iterator begin();
  const_iterator begin() const;
  iterator end();
  const_iterator end() const;

  size_type size() const;

  bool isEmpty() const;

  iterator insertAfter(const_iterator it, const T& value = T());
  iterator insertAtBegin(const T& value = T());

  iterator removeAfter(const_iterator it);

  void clear();

  bool find(iterator& it, iterator& prevIt, const T& value
    , const_iterator prevStartIt = const_iterator()) const;
  bool find(iterator& it, const T& value) const;
  
  template<class P>
  bool findIf(iterator& it, iterator& prevIt, P pred
    , const_iterator prevStartIt = const_iterator()) const;
  template<class P>
  bool findIf(iterator& it, P pred) const;

  bool operator==(const OdUnidirectionalList& list) const;

protected:
  static SNode* createNode();
  static SNode* createNode(const T& value);

  static void destroyNode(SNode* pNode);

  static SNode* copyList(SNode* pNode);

private:
  SNode* m_pHead;
  size_type m_size;
};


template<class T, class A, class Mm>
inline OdUnidirectionalList<T, A, Mm>::OdUnidirectionalList()
: m_pHead(NULL), m_size(0)
{
}

template<class T, class A, class Mm>
inline OdUnidirectionalList<T, A, Mm>::OdUnidirectionalList(const OdUnidirectionalList<T, A, Mm>& list)
: m_pHead(copyList(list.m_pHead)), m_size(list.m_size)
{
}

template<class T, class A, class Mm>
inline OdUnidirectionalList<T, A, Mm>::~OdUnidirectionalList()
{
  clear();
}

template<class T, class A, class Mm>
inline OdUnidirectionalList<T, A, Mm>& OdUnidirectionalList<T, A, Mm>::operator=(const OdUnidirectionalList<T, A, Mm>& list)
{
  if(this != &list)
  {
    clear();

    m_pHead = copyList(list.m_pHead);
    m_size = list.m_size;
  }

  return *this;
}

template<class T, class A, class Mm>
inline typename OdUnidirectionalList<T, A, Mm>::SNode* OdUnidirectionalList<T, A, Mm>::createNode()
{
  SNode* pNewNode = reinterpret_cast<SNode*>(Mm::Alloc(sizeof(SNode)));

  if(pNewNode == NULL)
    throw OdError(eOutOfMemory);

  A::construct(&(pNewNode->m_value));

  return pNewNode;
}

template<class T, class A, class Mm>
inline typename OdUnidirectionalList<T, A, Mm>::SNode* OdUnidirectionalList<T, A, Mm>::createNode(const T& value)
{
  SNode* pNewNode = reinterpret_cast<SNode*>(Mm::Alloc(sizeof(SNode)));

  if(pNewNode == NULL)
    throw OdError(eOutOfMemory);

  A::construct(&(pNewNode->m_value), value);

  return pNewNode;
}

template<class T, class A, class Mm>
inline void OdUnidirectionalList<T, A, Mm>::destroyNode(SNode* pNode)
{
  ODA_ASSERT(pNode != NULL);

  A::destroy(&(pNode->m_value));

  Mm::Free(pNode);
}

template<class T, class A, class Mm>
inline typename OdUnidirectionalList<T, A, Mm>::SNode* OdUnidirectionalList<T, A, Mm>::copyList(SNode* pNode)
{
  SNode* pHead = NULL;

  if(pNode != NULL)
  {
    SNode* pNewNode = pHead = createNode(pNode->m_value);

    pNewNode->m_pNext = NULL;

    for(pNode = pNode->m_pNext; pNode != NULL; pNode = pNode->m_pNext)
    {
      pNewNode->m_pNext = createNode(pNode->m_value);

      pNewNode->m_pNext->m_pNext = NULL;

      pNewNode = pNewNode->m_pNext;
    }
  }

  return pHead;
}

template<class T, class A, class Mm>
inline typename OdUnidirectionalList<T, A, Mm>::iterator OdUnidirectionalList<T, A, Mm>::begin()
{
  return iterator(m_pHead);
}

template<class T, class A, class Mm>
inline typename OdUnidirectionalList<T, A, Mm>::const_iterator OdUnidirectionalList<T, A, Mm>::begin() const
{
  return const_iterator(m_pHead);
}

template<class T, class A, class Mm>
inline typename OdUnidirectionalList<T, A, Mm>::iterator OdUnidirectionalList<T, A, Mm>::end()
{
  return iterator();
}

template<class T, class A, class Mm>
inline typename OdUnidirectionalList<T, A, Mm>::const_iterator OdUnidirectionalList<T, A, Mm>::end() const
{
  return const_iterator();
}

template<class T, class A, class Mm>
inline typename OdUnidirectionalList<T, A, Mm>::size_type OdUnidirectionalList<T, A, Mm>::size() const
{
  return m_size;
}

template<class T, class A, class Mm>
inline bool OdUnidirectionalList<T, A, Mm>::isEmpty() const
{
  return (m_size == 0);
}

template<class T, class A, class Mm>
inline typename OdUnidirectionalList<T, A, Mm>::iterator OdUnidirectionalList<T, A, Mm>::insertAfter(const_iterator it
  , const T& value)
{
  SNode* pNode = createNode(value);

  if(it.node() != NULL)
  {
    pNode->m_pNext = it.node()->m_pNext;
    it.node()->m_pNext = pNode;

  }
  else
  {
    pNode->m_pNext = m_pHead;
    m_pHead = pNode;
  }
  
  ++m_size;

  return iterator(pNode);
}

template<class T, class A, class Mm>
inline typename OdUnidirectionalList<T, A, Mm>::iterator OdUnidirectionalList<T, A, Mm>::insertAtBegin(const T& value)
{
  SNode* pNode = createNode(value);

  pNode->m_pNext = m_pHead;
  m_pHead = pNode;
  
  ++m_size;

  return iterator(pNode);
}

template<class T, class A, class Mm>
inline typename OdUnidirectionalList<T, A, Mm>::iterator OdUnidirectionalList<T, A, Mm>::removeAfter(const_iterator it)
{
  if(it.node() != NULL)
  {
    SNode* pNode = it.node()->m_pNext;

    if(pNode != NULL)
    {
      it.node()->m_pNext = pNode->m_pNext;

      destroyNode(pNode);

      --m_size;
    }

    return iterator(it.node()->m_pNext);
  }

  SNode* pNode = m_pHead;

  m_pHead = m_pHead->m_pNext;

  destroyNode(pNode);
  
  --m_size;

  return iterator(m_pHead);
}

template<class T, class A, class Mm>
inline void OdUnidirectionalList<T, A, Mm>::clear()
{
  SNode* pNode;
  SNode* pNextNode;

  for(pNode = m_pHead; pNode != NULL; pNode = pNextNode)
  {
    pNextNode = pNode->m_pNext;

    destroyNode(pNode);
  }

  m_pHead = NULL;
  m_size = 0;
}

template<class T, class A, class Mm>
inline bool OdUnidirectionalList<T, A, Mm>::find(iterator& it, iterator& prevIt
  , const T& value, const_iterator prevStartIt) const
{
  return find(it, prevIt, CmpValues(value), prevStartIt);
}

template<class T, class A, class Mm>
inline bool OdUnidirectionalList<T, A, Mm>::find(iterator& it, const T& value) const
{
  return find(it, CmpValues(value));
}

template<class T, class A, class Mm>
template<class P>
inline bool OdUnidirectionalList<T, A, Mm>::findIf(iterator& it, iterator& prevIt
  , P pred, const_iterator prevStartIt) const
{
  if(!isEmpty())
  {
    ODA_ASSERT(m_pHead != NULL);

    SNode* pPrevNode = prevStartIt.node();
    SNode* pNode = ((pPrevNode != NULL) ? pPrevNode->m_pNext : m_pHead);

    for(; pNode != NULL; pPrevNode = pNode, pNode = pNode->m_pNext)
    {
      if(pred(pNode->m_value))
      {
        it.setNode(pNode);
        prevIt.setNode(pPrevNode);

        return true;
      }
    }
  }

  return false;
}

template<class T, class A, class Mm>
template<class P>
inline bool OdUnidirectionalList<T, A, Mm>::findIf(iterator& it, P pred) const
{
  if(!isEmpty())
  {
    ODA_ASSERT(m_pHead != NULL);

    for(SNode* pNode = m_pHead; pNode != NULL; pNode = pNode->m_pNext)
    {
      if(pred(pNode->m_value))
      {
        it.setNode(pNode);

        return true;
      }
    }
  }

  return false;
}

template<class T, class A, class Mm>
inline bool OdUnidirectionalList<T, A, Mm>::operator==(const OdUnidirectionalList<T, A, Mm>& list) const
{
  if(m_size == list.m_size)
  {
    SNode* pNode1 = m_pHead;
    SNode* pNode2 = list.m_pHead;

    for(; pNode1 != NULL; pNode1 = pNode1->m_pNext, pNode2 = pNode2->m_pNext)
    {
      ODA_ASSERT(pNode2 != NULL);

      if(pNode1->m_value != pNode2->m_value)
        return false;
    }

    return true;
  }

  return false;
}

#include "TD_PackPop.h"

#endif // _ODA_UNIDIRECTIONALLISTT_H_
