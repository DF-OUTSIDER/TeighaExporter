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

#ifndef DbAssocParamBasedActionBody_INCLUDED_
#define DbAssocParamBasedActionBody_INCLUDED_ /*!DOM*/

#include "DbAssocActionBody.h"

#include "TD_PackPush.h"

/** \details
  This class represents the associated parameter based on the action body.

  Library: Source code provided.
  <group Contraint_Classes> 
*/
class DBCONSTRAINTS_EXPORT ODRX_ABSTRACT OdDbAssocParamBasedActionBody : public OdDbAssocActionBody
{
public: 
  ODRX_DECLARE_MEMBERS(OdDbAssocParamBasedActionBody);

  /** \details
    Constructor creates an instance of this class.
  */
  OdDbAssocParamBasedActionBody();

  // is abstract via evaluateOverride()
};

/** \details
  The typified smart pointer. This template class is specialization of the OdSmartPtr class 
  for the OdDbAssocParamBasedActionBody class.

  \sa
  <link smart_pointers.html, Working with Smart Pointers>
*/
typedef OdSmartPtr<OdDbAssocParamBasedActionBody> OdDbAssocParamBasedActionBodyPtr;

#include "TD_PackPop.h"

#endif
