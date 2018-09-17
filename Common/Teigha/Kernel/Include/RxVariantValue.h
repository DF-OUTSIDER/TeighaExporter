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

#ifndef _VARIANTVALUE_H_INCLUDED_
#define _VARIANTVALUE_H_INCLUDED_

#include "TD_PackPush.h"

#include "RxObject.h"
#include "RxVariant.h"
#include "RxObjectImpl.h"

class OdRxVariant;
typedef OdSmartPtr<OdRxVariant> OdRxVariantPtr;

/** \details
    This class implements Variant objects in an OdRx context. 
    
    \remarks
    These objects are used to store data for OdRxVariantValue objects.
    Corresponding C++ library: TD_Db
    <group OdRx_Classes> 
*/
class OdRxVariant : public OdRxObject, public OdVariant { };

/** \details
    This class implements Variant Value objects in an OdRx context. 
   
    Corresponding C++ library: TD_Db
    <group OdRx_Classes> 
*/
class OdRxVariantValue : public OdSmartPtr<OdRxVariant>
{
public:
  /** \details
    Assigns the specified OdRxVariant object to this RxVariantValue object.
    \param pVariant [in]  Pointer to the Variant object.
  */
  void assign(const OdRxVariant* pVariant) { OdSmartPtr<OdRxVariant>::operator=(pVariant); }

  /** \remarks
    Constructors with pointer arguments assign the referenced objects to the new RxVariantValue object. 
    
    Constructors with value arguments create a new RxVariant object with the specified value,
    and assign them the new RxVariantValue object.
  */
  OdRxVariantValue(const OdRxObject* pObject) : OdSmartPtr<OdRxVariant>(pObject) {}

  OdRxVariantValue(const OdRxVariant* pVariant) : OdSmartPtr<OdRxVariant>(pVariant) {}

#define ODRXVARVAL_CONSTR(type, setter) \
  OdRxVariantValue(type value) \
  { \
    assign(OdRxObjectImpl<OdRxVariant>::createObject()); \
    get()->setter(value); \
  }
  ODRXVARVAL_CONSTR(bool           , setBool)
  ODRXVARVAL_CONSTR(OdUInt8        , setUInt8)
  ODRXVARVAL_CONSTR(OdInt8         , setInt8)
  ODRXVARVAL_CONSTR(OdUInt16       , setUInt16)
  ODRXVARVAL_CONSTR(OdInt16        , setInt16)
  ODRXVARVAL_CONSTR(OdUInt32       , setUInt32)
  ODRXVARVAL_CONSTR(OdInt32        , setInt32)
  ODRXVARVAL_CONSTR(OdUInt64       , setUInt64)
  ODRXVARVAL_CONSTR(OdInt64        , setInt64)
#ifdef OD_INTPTR_UNIQUE
  ODRXVARVAL_CONSTR(OdIntPtr       , setIntPtr)
#endif
  ODRXVARVAL_CONSTR(double         , setDouble)
  ODRXVARVAL_CONSTR(const OdString&, setString)
  ODRXVARVAL_CONSTR(const OdAnsiString&, setAnsiString)
  ODRXVARVAL_CONSTR(const OdStringArray&, setStringArray)
  ODRXVARVAL_CONSTR(const OdRxObjectPtrArray&, setRxObjectPtrArray)
  ODRXVARVAL_CONSTR(const OdDoubleArray&, setDoubleArray)
#undef ODRXVARVAL_CONSTR

  /** \remarks
    Cast operators return the value of the OdRxVariant object referenced 
    by this OdRxVariantValue object.
  */
#define ODRXVARVAL_IMPLICIT(type, getter) \
  operator type() const \
  { \
    return get()->getter(); \
  } \
  operator type() \
  { \
    return get()->getter(); \
  }
  // #7196 : override OdSmartPtr<T>::operator bool() private member.
  // #7196 : some compilers (MSVC) require non-const for all types if bool() member available.
  ODRXVARVAL_IMPLICIT(bool    , getBool)
  ODRXVARVAL_IMPLICIT(OdUInt8 , getUInt8)
  ODRXVARVAL_IMPLICIT(OdUInt16, getUInt16)
  ODRXVARVAL_IMPLICIT(OdUInt32, getUInt32)
  ODRXVARVAL_IMPLICIT(OdUInt64, getUInt64)
  ODRXVARVAL_IMPLICIT(OdInt8  , getInt8)
  ODRXVARVAL_IMPLICIT(OdInt16 , getInt16)
  ODRXVARVAL_IMPLICIT(OdInt32 , getInt32)
  ODRXVARVAL_IMPLICIT(OdInt64 , getInt64)
#ifdef OD_INTPTR_UNIQUE
  ODRXVARVAL_IMPLICIT(OdIntPtr, getIntPtr)
#endif
  ODRXVARVAL_IMPLICIT(double  , getDouble)
  ODRXVARVAL_IMPLICIT(OdString, getString)
  ODRXVARVAL_IMPLICIT(OdAnsiString, getAnsiString)
  ODRXVARVAL_IMPLICIT(OdStringArray, getStringArray)
  ODRXVARVAL_IMPLICIT(OdRxObjectPtrArray, getRxObjectPtrArray)
  ODRXVARVAL_IMPLICIT(OdDoubleArray, getDoubleArray)
#undef ODRXVARVAL_IMPLICIT
};

#include "TD_PackPop.h"

#endif //_VARIANTVALUE_H_INCLUDED_
