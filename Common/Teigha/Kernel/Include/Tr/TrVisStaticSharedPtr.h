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
// GLES2 device static shared pointer

#ifndef ODTRVISSTATICSHAREDPTR
#define ODTRVISSTATICSHAREDPTR

#include "TD_PackPush.h"

#include "SharedPtr.h"

/** \details
    <group ExRender_Windows_Classes>
*/
template <class T>
class OdStaticSharedPtr : public OdSharedPtr<T>
{
  protected:
    int m_realRefs;
  protected:
    T **accessObjPtr() { return reinterpret_cast<T**>(this); }
    int **accessRefPtr() { return reinterpret_cast<int**>(this) + 1; }
  public:
    OdStaticSharedPtr(T *pRef)
      : OdSharedPtr<T>(), m_realRefs(2)
    {
      *accessObjPtr() = pRef;
      *accessRefPtr() = &m_realRefs;
    }
    OdStaticSharedPtr& operator=(const OdStaticSharedPtr &other)
    {
      **this = *other;
      return *this;
    }
  private:
    T* detach() { return NULL; }
    OdStaticSharedPtr(const OdStaticSharedPtr &other) {}
};

/** \details
    <group ExRender_Windows_Classes>
*/
template <class T>
class OdStaticDefSharedPtr : public OdStaticSharedPtr<T>
{
  protected:
    T m_realObj;
  public:
    OdStaticDefSharedPtr()
      : OdStaticSharedPtr<T>(&m_realObj)
    {
    }
    OdStaticDefSharedPtr(const OdStaticDefSharedPtr &other)
      : OdStaticSharedPtr<T>(&m_realObj)
    {
      m_realObj = *other;
    }
};

#include "TD_PackPop.h"

#endif // ODTRVISSTATICSHAREDPTR
