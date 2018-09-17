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



#ifndef _ODDBCLASS_INCLUDED_
#define _ODDBCLASS_INCLUDED_

#include "TD_PackPush.h"

#include "RxObject.h"

////1) Override
//OdRxClass* ClassName::saveAsClass(OdRxClass*) const
//{
//  return PseudoBaseClass::saveAsClass(PseudoBaseClass::desc());
//}
//
////2) rxInit()
//// - verify pseudo base class (Save_As_Class)
//// - remember old constructor 
//// - set new constructor
//// 


/** \details
    Declares the functions required by pseudo database objects.
*/
#define ODDB_PSEUDO_DECLARE_MEMBERS(ClassName)    \
public:                                           \
  ODRX_DECLARE_MEMBERS(ClassName);                \
  static OdPseudoConstructorType g_pMainConstr;   \
  OdRxClass* saveAsClass(OdRxClass* pClass) const

/** \details
  Defines initialization functions for pseudo *database* objects.

*/
#define ODDB_PSEUDO_DEFINE_INIT_MEMBERS(ClassName,ParentClass,pseudoConsFn,PseudoBaseClass) \
                                                                            \
/* Registers this class with Teigha. */                                     \
void ClassName::rxInit()                                                    \
{                                                                           \
  if (!ClassName::g_pDesc)                                                  \
  {                                                                         \
    OdRxClass* pParent = ParentClass::desc();                               \
    OdRxClass* pPseudoBase = PseudoBaseClass::desc();                       \
    if (!pParent->isDerivedFrom(pPseudoBase))                               \
    {                                                                       \
      throw OdError(eNotThatKindOfClass);                                   \
    }                                                                       \
    ClassName::g_pDesc = PseudoBaseClass::desc();                           \
    g_pMainConstr = pPseudoBase->constructor();                             \
    pPseudoBase->setConstructor(ClassName::pseudoConstructor);              \
  }                                                                         \
  else                                                                      \
  {                                                                         \
    ODA_ASSERT(("Class ["#ClassName"] is already initialized.",0));         \
    throw OdError(eExtendedError);                                          \
  }                                                                         \
}                                                                           \
                                                                            \
/* Unregisters this class with Teigha. */                                   \
void ClassName::rxUninit()                                                  \
{                                                                           \
  if (ClassName::g_pDesc)                                                   \
  {                                                                         \
    OdRxClass* pPseudoBase = PseudoBaseClass::desc();                       \
    pPseudoBase->setConstructor(g_pMainConstr);                             \
    ClassName::g_pMainConstr = 0;                                           \
    ClassName::g_pDesc = 0;                                                 \
  }                                                                         \
  else                                                                      \
  {                                                                         \
    ODA_ASSERT(("Class ["#ClassName"] is not initialized yet.",0));         \
    throw OdError(eNotInitializedYet);                                      \
  }                                                                         \
}

/** \details
    Defines RTTI, initialization functions and constructor for pseudo database objects.
*/
#define ODDB_PSEUDO_DEFINE_MEMBERS(ClassName,ParentClass,PseudoBaseClass,DOCREATE)  \
                                                                                    \
ODRX_DEFINE_RTTI_MEMBERS(ClassName,ParentClass)                                     \
                                                                                    \
ODRX_DEFINE_PSEUDOCONSTRUCTOR(ClassName,DOCREATE)                                   \
                                                                                    \
ODDB_PSEUDO_DEFINE_INIT_MEMBERS(ClassName,ParentClass,pseudoConsFn,PseudoBaseClass) \
                                                                                    \
OdPseudoConstructorType ClassName::g_pMainConstr = 0;                               \
                                                                                    \
OdRxClass* ClassName::saveAsClass(OdRxClass*) const                                 \
{                                                                                   \
  return PseudoBaseClass::saveAsClass(PseudoBaseClass::desc());                     \
}

#include "TD_PackPop.h"

#endif //_ODDBCLASS_INCLUDED_
