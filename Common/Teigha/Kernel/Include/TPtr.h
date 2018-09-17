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

#ifndef __TPTR_H__
#define __TPTR_H__

/** \details
  <group Other_Classes> 
*/
template <typename TObj>
class TObjRelease
{
  public:
    static void addRef(TObj* pObj)  { if (pObj) pObj->addRef(); }
    static void release(TObj* pObj) { if (pObj) pObj->release(); }
};

/** \details
  <group Other_Classes> 
*/
template <typename TObj>
class TObjDelete
{
  public:
    static void addRef(TObj* pObj) {}
    static void release(TObj* pObj) { delete pObj; }
};

/** \details
  <group Other_Classes> 
*/
template <typename TObj>
class TObjDeleteArray
{
  public:
    static void addRef(TObj* pObj) {}
    static void release(TObj* pObj) { delete []pObj; }
};

/** \details
  <group Other_Classes> 
*/
//template <typename TObj>
//class TObjFree
//{
//  public:
//    static void addRef(TObj* pObj) {}
//    static void release(TObj* pObj) { ::odrxFree(pObj); }
//};

/** \details
  <group Other_Classes> 
*/
template <typename TObj, typename THelper = TObjRelease<TObj> >
class TPtr
{
  protected:
    TObj *m_obj;

    TPtr &replace(TObj* pObj)
    {
      THelper::addRef(pObj);
      THelper::release(m_obj);
      m_obj = pObj;
      return *this;
    }

  public:
    TPtr() : m_obj (NULL) {}
    TPtr(const TPtr& ref) : m_obj (ref.m_obj)
    {
      THelper::addRef(m_obj);
    }
    TPtr(const TObj* pObj, bool bAttach = false) : m_obj(const_cast<TObj*>(pObj))
    {
      if (!bAttach)
        THelper::addRef(m_obj);
    }
    ~TPtr()
    {
      THelper::release(m_obj);
    }
    TPtr &operator =(TObj *pObj)
    {
      return replace (pObj);
    }
    TPtr &operator =(const TPtr& ref)
    {
      return replace (ref.m_obj);
    }
    const TObj* get() const
    {
      return m_obj;
    }
    TObj* get()
    {
      return m_obj;
    }
    TObj* operator ->()
    {
      ODA_ASSERT(m_obj);
      return m_obj;
    }
    const TObj* operator ->() const
    {
      ODA_ASSERT(m_obj);
      return m_obj;
    }
    operator TObj *()
    {
      return m_obj;
    }
    operator const TObj *() const
    {
      return m_obj;
    }
    TObj** getPtr()
    {
      THelper::release(m_obj);
      m_obj = NULL;
      return &m_obj; // return reference to NULL pointer
    }
    bool isNull() const
    {
      return !m_obj;
    }
    TObj *detach ()
    {
      TObj *pObj = m_obj;
      m_obj = NULL;
      return pObj;
    }
    TObj* attach(TObj* obj)
    {
      THelper::release(m_obj);
      m_obj = obj;
      return m_obj;
    }
};

#endif // __TPTR_H__
