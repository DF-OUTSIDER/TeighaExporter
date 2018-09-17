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

#ifndef DbAssocSurfacesParameters_INCLUDED_
#define DbAssocSurfacesParameters_INCLUDED_ /*!DOM*/

#include "DbAssocArrayParameters.h"
#include "DbAssocArrayCommonParameters.h"
#include "DbImpAssocSurfacesParameters.h"

#include "TD_PackPush.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/** \details
This class represents an array of assoc loft parameters.

Library: Source code provided.
<group Contraint_Classes>
*/
class DBCONSTRAINTS_EXPORT OdDbAssocLoftParameters : public OdDbAssocArrayParameters
{
public:
  /** Casts the specified pointer to an ClassName SmartPointer. **/
  static OdSmartPtr<OdDbAssocLoftParameters> cast(const OdRxObject* pObj)
  {
    if (pObj)
      return OdSmartPtr<OdDbAssocLoftParameters>(((OdDbAssocLoftParameters*)pObj->queryX(OdDbAssocLoftParameters::desc())), kOdRxObjAttach);

    return (OdDbAssocLoftParameters*)0;
  }

  static OdRxClass* g_pDesc;

  /** Returns the static ClassType description object associated with this object. **/
  /** This function is for use only when the class type of this object is known.          **/
  /** If the class type of this object is unknown, use instead isA().                           **/
  static OdRxClass* desc();
  /** Returns the ClassType description instance associated with this object.      **/
  /** This function is for use only when the class type of this object is unknown.        **/
  /** If the class type of this object is known, use instead desc().                            **/
  virtual OdRxClass* isA() const;
  /** Returns the Protocol Extension object for this object.        **/
  /** Return null if there is no Protocol Extension object is found.    **/
  virtual OdRxObject* queryX(const OdRxClass* protocolClass) const;
  /** Creates a new instance of this object type.                   **/
  /** Returns a SmartPointer to the new instance.                       **/
  static OdRxObjectPtr pseudoConstructor();
  /** Creates a new instance of this object type.                   **/
  /** Returns a SmartPointer to the new instance.                       **/
  static OdSmartPtr<OdDbAssocLoftParameters> createObject()
  {
    if (!desc())
      throw OdError(eNotInitializedYet);
    return desc()->create();
  }
  /** Registers ClassName with Teigha.                           **/
  static void rxInit();
  static void rxInit(AppNameChangeFuncPtr appNameChangeFunc);
  /* Unregisters ClassName with Teigha.                          **/
  static void rxUninit();

  /** \details
  Constructor creates an instance of this class.
  */
  OdDbAssocLoftParameters
    (
      int continuity = 1,
      double bulge = 0.5
      );

  /** \details
  Destructor destroys an instance of this class.
  */
  virtual ~OdDbAssocLoftParameters();

  virtual OdDbItemLocator getCanonicalForm(const OdDbItemLocator& locator) const;
  virtual OdResult getItems(OdArray<OdDbAssocArrayItemPtr>& items) const;
  virtual OdResult getItemPosition(const OdDbItemLocator &, OdGePoint3d &, OdGeMatrix3d &) const;

  virtual OdResult setOwner(class OdDbAssocActionBody* pOwner);
  void setInputVariables(const OdDbObjectIdArray& sourceEntites, short continuity, double bulge);

protected:
  OdDbAssocLoftParameters(class OdDbImpAssocLoftParameters* pImpl);
};

typedef OdSmartPtr<OdDbAssocLoftParameters> OdDbAssocLoftParametersPtr;

#include "TD_PackPop.h"

#endif

