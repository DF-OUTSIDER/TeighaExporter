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

#ifndef _ODSAVESTATE_INCLUDED_
#define _ODSAVESTATE_INCLUDED_

#include "TD_PackPush.h"

/** \details
    This template class implements SaveState objects which save and restore variables.
    
    \remarks
    Variables are saved locally on construction of OdSaveState objects.
    Variables are restored when OdSaveState objects go out of scope.
  
    <group Other_Classes>
*/
template <class T>
class OdSaveState
{
  T&  m_val;        // Variable
  T   m_oldValue;   // Old value
public:
  /** \param variable [in]  Variable to save.
  */
  OdSaveState( T& variable )
    : m_val( variable )
  {
    m_oldValue = m_val;
  }
  /** \param newValue [in]  New value for variable.
  */
  OdSaveState( T& variable, const T& newValue )
    : m_val( variable )
  {
    m_oldValue = m_val;
    m_val = newValue;
  }
  ~OdSaveState()
  {
    m_val = m_oldValue;
  }
  /** \details
    Returns the saved value of the variable.
  */
  operator const T&() const
  {
    return m_oldValue;
  }
};

#ifdef __hpux
#define OdSaveStateFlagDef(type, flag, name, flags) \
  const type _##name##Flag = flag; \
  OdSaveStateFlagImpl<type, _##name##Flag> name(flags)
#define OdSaveStateFlag(type, flag, name, flags, newVal) \
  const type _##name##Flag = flag; \
  OdSaveStateFlagImpl<type, _##name##Flag> name(flags, newVal)
#else
#define OdSaveStateFlagDef(type, flag, name, flags) \
  OdSaveStateFlagImpl<type, flag> name(flags)
#define OdSaveStateFlag(type, flag, name, flags, newVal) \
  OdSaveStateFlagImpl<type, flag> name(flags, newVal)
#endif

/** \details
            
    <group Other_Classes> 
*/
template <class T, T flag>
class OdSaveStateFlagImpl
{
  T&   m_val;       // Variable
  bool m_oldValue;  // Old flag value
public:
  /** \param variable [in]  Variable to save.
  */
  OdSaveStateFlagImpl(T& variable): m_val( variable )
  {
    m_oldValue = GETBIT(m_val, flag);
  }
  OdSaveStateFlagImpl(T& variable, bool bNewValue): m_val( variable )
  {
    m_oldValue = GETBIT(m_val, flag);
    SETBIT(m_val, flag, bNewValue);
  }
  ~OdSaveStateFlagImpl()
  {
    SETBIT(m_val, flag, m_oldValue);
  }
  /** \details
    Returns the saved value of the flag.
  */
  operator bool() const
  {
    return m_oldValue;
  }
};

template <typename ObjType, typename ValType>
class OdSaveStateFunc
{
public:
  OdSaveStateFunc(ObjType* pObj, ValType(ObjType::*getFunc)() const,
    void(ObjType::*setFunc)(ValType))
    :m_pObj(pObj), m_getFunc(getFunc), m_setFunc(setFunc)
  {
    m_oldValue = (*m_pObj.*m_getFunc)();
  }
  OdSaveStateFunc(ObjType* pObj, ValType(ObjType::*getFunc)() const,
    void(ObjType::*setFunc)(ValType), ValType newValue)
    :m_pObj(pObj), m_getFunc(getFunc), m_setFunc(setFunc)
  {
    m_oldValue = (*m_pObj.*m_getFunc)();
    (*m_pObj.*m_setFunc)(newValue);
  }
  ~OdSaveStateFunc()
  {
    (*m_pObj.*m_setFunc)(m_oldValue);
  }
protected:
  ObjType* m_pObj;
  ValType(ObjType::*m_getFunc)() const;
  void(ObjType::*m_setFunc)(ValType);
  ValType m_oldValue;
};

#include "TD_PackPop.h"

#endif //#ifndef _ODSAVESTATE_INCLUDED_
