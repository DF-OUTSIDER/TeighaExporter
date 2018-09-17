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

#ifndef OdDbAssocRotatedDimActionBody_INCLUDED_
#define OdDbAssocRotatedDimActionBody_INCLUDED_ /*!DOM*/

#include "DbAssocActionBody.h"
#include "DbGeomRef.h"

#include "TD_PackPush.h"

/** \details
This class represents the base class OdDbAssocAnnotationActionBody for the Assoc Dim action bodies.

Library: Source code provided.
<group Contraint_Classes>
*/
class DBCONSTRAINTS_EXPORT ODRX_ABSTRACT OdDbAssocAnnotationActionBody : public OdDbAssocActionBody
{
public:
  ODRX_DECLARE_MEMBERS(OdDbAssocAnnotationActionBody);

  /** \details
  Constructor creates an instance of this class.
  */
  OdDbAssocAnnotationActionBody();

  /** \details
  This is the notification call of the corresponding evaluate() method
  of the parent class that owns this action body.
  */
  virtual void evaluateOverride(void);
  virtual void modified(const OdDbObject* pObject);

  void setAssocDependencyID(OdDbObjectId objID);
  void setActionParamID1(OdDbObjectId objID);
  void setActionParamID2(OdDbObjectId objID);
};

/** \details
The typified smart pointer. This template class is specialization of the OdSmartPtr class
for the OdDbAssocAnnotationActionBody class.

\sa
<link smart_pointers.html, Working with Smart Pointers>
*/
typedef OdSmartPtr<OdDbAssocAnnotationActionBody> OdDbAssocAnnotationActionBodyPtr;
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

/** \details
<group Contraint_Classes>
*/
class DBCONSTRAINTS_EXPORT OdDbAssocRotatedDimActionBody : public OdDbAssocAnnotationActionBody
{
public:
  ODRX_DECLARE_MEMBERS(OdDbAssocRotatedDimActionBody);

  /** \details
  Constructor creates an instance of this class.
  */
  OdDbAssocRotatedDimActionBody(bool createImpObj = true);

  /** \details
  Destructor destroys an instance of this class.
  */
  virtual ~OdDbAssocRotatedDimActionBody();

  //const OdDbSmartCenterMarkParametersPtr parameters() const;
  //OdDbSmartCenterMarkParametersPtr& parameters();
    virtual void composeForLoad(OdDb::SaveType format, OdDb::DwgVersion version, OdDbAuditInfo* pAuditInfo);
  /** \details
  Creates associative AssocRotatedDim: action, action body, dependencies etc.
  */
  static OdResult createInstance(
	  const OdDbFullSubentPathArray entPathArray,
	  OdGePoint3dArray ptsSelected,
	  OdDbObjectId& actionBodyId);
};

typedef OdSmartPtr<OdDbAssocRotatedDimActionBody> OdDbAssocRotatedDimActionBodyPtr;
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "TD_PackPop.h"

#endif

