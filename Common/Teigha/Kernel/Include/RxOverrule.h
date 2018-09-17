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

#ifndef _RXOVERRULE_H
#define _RXOVERRULE_H

#include "RxObject.h"

#include "TD_PackPush.h"
class OdRxOverruleIterator;

/** \details
  This class is an abstract base class for implementation of overrules. An overrule can be registered or unregistered,
  and overrules can be globally activated or deactivated. Overrules are internally managed by pairs of classes 
  named as "target" and "overrule." The "target" is the class to which the overrule applies. Classes can have multiple 
  overrules registered. 

  Corresponding C++ library: TD_Db
  <group OdRx_Classes> 
*/
class FIRSTDLL_EXPORT OdRxOverrule : public OdRxObject
{
public:
  ODRX_DECLARE_MEMBERS(OdRxOverrule);

  /** \details
    Destructor deletes an instance of this class.
  */
  virtual ~OdRxOverrule();

  /** \details
    Determines whether the overrule is applicable and returns True if the overrule can be applied,
    or False, otherwise. It is a pure virtual method that must be implemented in the derived class.

    \param pOverruledSubject [in]  A raw pointer to an object to be overruled.
  */
  virtual bool isApplicable(const OdRxObject* pOverruledSubject) const = 0;

  /** \details
    Registers an object for a given class to be overruled.
    This method returns eOk if successful.

    \param pClass [in]  A raw pointer to the class description object to which the overrule applies.
    \param pOverrule [in]  A raw pointer to an object to be overruled.
    \param bAddAtLast [in]  Determines whether the pair should be added in the end of the overrule collection.
  */
  static OdResult addOverrule(OdRxClass* pClass, OdRxOverrule* pOverrule, bool bAddAtLast = false);

  /** \details
    Unregisters an object for the given class.
    This method returns eOk if successful.

    \param pClass [in]  A raw pointer to the class description object to which the overrule applies.
    \param pOverrule [in]  A raw pointer to an object to be overruled.
  */
  static OdResult removeOverrule(OdRxClass* pClass, OdRxOverrule* pOverrule);

  /** \details
    Enables or disables the overruling mechanism.

    \param bIsOverruling [in]  Determines whether the overruling mechanism is enabled (true) or disabled (false).
  */
  static void setIsOverruling(bool bIsOverruling);

  /** \details
    Checks whether the overruling mechanism is enabled (true) or disabled (false).
  */
  static bool isOverruling()
  {
    return s_bIsOverruling;
  }

  /** \details
    Checks whether the specified overrule handles a given object and returns True if the object 
    is to be handled by overrule, or False, otherwise.

    \param pOverruleClass [in]  A raw pointer to the class description object to which the overrule applies.
    \param pSubject [in]  A raw pointer to an object to be overruled.
  */
  static bool hasOverrule(const OdRxObject* pSubject, OdRxClass* pOverruleClass);
private:
  /*!DOM*/
  OdRxOverruleIterator* m_pIter;
  /*!DOM*/
  FIRSTDLL_EXPORT_STATIC static bool s_bIsOverruling;
  /*!DOM*/
  friend class OdRxOverruleInternals;
};

#include "TD_PackPop.h"
#endif  // _RXOVERRULE_H
