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

#ifndef DbAssocValueProviderPE_INCLUDED_
#define DbAssocValueProviderPE_INCLUDED_ /*!DOM*/


//#include "dbeval.h"
#include "DbConstraintsExports.h"
#include "RxObject.h"

#include "TD_PackPush.h"

class OdDbEvalVariant;

/** \details
  This class provides the protocol extension pure virtual base class that declares the protocol 
  for getting and setting arbitrary simple scalar values.

  Library: Source code provided.
  <group Contraint_Classes> 
*/
class DBCONSTRAINTS_EXPORT OdDbAssocValueProviderPE : public OdRxObject
{
public:
  ODRX_DECLARE_MEMBERS(OdDbAssocValueProviderPE);
#if 0

  /** \details
    Checks whether the value identified by own name can be obtained.
  */
  virtual bool canGetValue(const OdDbObject* pObject, const OdString& valueName) { return true; }

  /** \details
    Checks whether the value identified by own name can be specified.
  */
  virtual bool canSetValue(const OdDbObject* pObject, const OdString& valueName) { return false; }

  /** \details
    Gets the value identified by own name.
  */
  virtual OdResult getValue(const OdDbObject* pObject, 
                                     const OdString&   valueName, 
                                     OdDbEvalVariant&  value) = 0;

  /** \details
    Sets the value identified by own name.
  */
  virtual OdResult setValue(OdDbObject* pObject, 
                            const OdString& valueName,
                            const OdDbEvalVariant& newValue) 
  {
    return eNotHandled;
  }
#endif
};


/** \details
  The typified smart pointer. This template class is specialization of the OdSmartPtr class 
  for the OdDbAssocValueProviderPE class.

  \sa
  <link smart_pointers.html, Working with Smart Pointers>
*/
typedef OdSmartPtr<OdDbAssocValueProviderPE> OdDbAssocValueProviderPEPtr;

#include "TD_PackPop.h"

#endif

