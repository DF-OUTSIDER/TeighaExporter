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




#if !defined(_ODREFCOUNTIMPL_INCLUDED_)
#define _ODREFCOUNTIMPL_INCLUDED_

#include "TD_PackPush.h"

#include "RxObject.h"

/** \details
    Template class that provides the reference counting functionality needed
    to instantiate objects that are descended from OdRxObject.

    <group OdRx_Classes>
*/
template<class T, class TInterface = T>
class OdRefCountImpl : public T
{
  /** \details
      Allows to create OdRefCountImpl<T> instances inside this class only.
  */
  OdUInt32 m_nRefCounter;

  /** \details
      Assignment operator prohibited.
  */
  OdRefCountImpl& operator = (const OdRefCountImpl&);

  /** \details
      Allows to create OdRefCountImpl<T> instances inside this class only.
  */
  ODRX_HEAP_OPERATORS();

  /** \details
      Constructor.  Sets the reference count to one.
  */
  OdRefCountImpl() : m_nRefCounter(1) {}
public:

  /** \details
      Increments the reference count.
  */
  void addRef()
  {
    ++m_nRefCounter;
  }

  /** \details
      Decrements the reference count, and deletes this object if the reference count
      reaches zero.
  */
  void release()
  {
		ODA_ASSERT((m_nRefCounter > 0));
		if (!(--m_nRefCounter))
		{
      T::onFinalRelease();
      delete this;
		}
  }

  long numRefs() const { return m_nRefCounter; }

  /** \details
      Creates an instance of OdRefCountImpl<T, TInterface>
      and returns SmartPointer to it.
  */
  static OdSmartPtr<TInterface> createObject()
  {
    return OdSmartPtr<TInterface>(static_cast<TInterface*>(new OdRefCountImpl<T, TInterface>), kOdRxObjAttach);
  }
};

#define ODREFCOUNTIMPL_CONSTR(CLASS, pRes) pRes = OdRefCountImpl<CLASS>::createObject()

#include "TD_PackPop.h"

#endif // !defined(_ODREFCOUNTIMPL_INCLUDED_)

