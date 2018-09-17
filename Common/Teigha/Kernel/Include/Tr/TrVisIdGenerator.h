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
// GLES2 static resource ID generator

#ifndef ODTRVISIDGENERATOR
#define ODTRVISIDGENERATOR

#include "TD_PackPush.h"

#include "../TrVisDefs.h"
#define STL_USING_SET
#include "OdaSTL.h"

/** \details
  <group ExRender_Windows_Classes>
*/
template<class T>
class OdTrVisIdGeneratorImpl
{
  typedef std::set<T> IdMap;
  mutable IdMap m_idMap;
  mutable T m_idMapCounter, m_idMapSearchEx;
  protected:
    T inc(T &counter, T &searchEx) const
    {
      const T NegativeId = (T)(-1);
      if (counter == NegativeId)
      {
        T id = searchEx;
        do
        {
          if (!findId(id))
            return searchEx = id;
          id++;
        } while (id < NegativeId);
        id = 100;
        while (id < searchEx)
        {
          if (!findId(id))
            return searchEx = id;
          id++;
        }
        counter = 100;
      }
      return counter++;
    }
  public:
    OdTrVisIdGeneratorImpl()
    {
      clean();
    }

    void clean()
    {
      m_idMap.clear();
      m_idMapCounter = m_idMapSearchEx = 100;
    }

    T generateId() const
    {
      const T freeId = inc(m_idMapCounter, m_idMapSearchEx);
      m_idMap.insert(freeId);
      return freeId;
    }

    void killId(T id) const
    {
      m_idMap.erase(id);
    }

    bool findId(T id) const
    {
      return m_idMap.find(id) != m_idMap.end();
    }
};

/** \details
  <group ExRender_Windows_Classes>
*/
typedef OdTrVisIdGeneratorImpl<OdTrVisId> OdTrVisGenericIdGenerator;

#include "TD_PackPop.h"

#endif // ODTRVISIDGENERATOR
