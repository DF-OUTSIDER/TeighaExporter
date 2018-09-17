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




#ifndef _ODSTACK_H_
#define _ODSTACK_H_

#include "TD_PackPush.h"

template <class T> class OdStackItem;
template <class T> class OdStack;

/** \details

    <group OdGi_Classes> 
*/
template <class T>
class OdStackItem : public T
{
  friend class OdStack<T>;
protected:
  OdStackItem* m_pUnder;
  inline OdStackItem(OdStackItem* pUnder, const T& val)
    : T(val), m_pUnder(pUnder) { }
  inline OdStackItem(OdStackItem* pUnder) : m_pUnder(pUnder) { }
};

/** \details

    <group OdGi_Classes> 
*/
template <class T>
class OdStack
{
  typedef OdStackItem<T> TItem;
public:
  TItem* m_pTop;
  inline OdStack() : m_pTop(0) { }
  inline void push(const T& inVal)
  {
    m_pTop = new TItem(m_pTop, inVal);
  }
  inline T* push()
  {
    m_pTop = new TItem(m_pTop);
    return top();
  }
  inline void pop(T& outVal)
  {
    ODA_ASSERT(m_pTop); // pop from empty stack
    outVal = *m_pTop;
    pop();
  }

  inline const T* top() const { return m_pTop; }
  inline T* top() { return m_pTop; }

  inline void pop()
  {
    TItem* pTop = m_pTop;
    ODA_ASSERT(pTop); // pop from empty stack
    m_pTop = pTop->m_pUnder;
    delete pTop;
  }

  inline ~OdStack()
  {
    while(m_pTop)
    {
      pop();
    }
  }

  inline T* beforeTop() const 
  { 
    ODA_ASSERT(m_pTop);
    return m_pTop->m_pUnder;
  }

  bool empty()const { return m_pTop == 0; }
  
  size_t size() const
  {
    size_t n = 0;
    for ( TItem* p = m_pTop; p; p = p->m_pUnder)
      ++n;
    return n;
  }
};

#include "TD_PackPop.h"

#endif //#ifndef _ODSTACK_H_
