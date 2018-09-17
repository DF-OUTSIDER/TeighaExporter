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

#ifndef DbAssocDependencyBody_INCLUDED_
#define DbAssocDependencyBody_INCLUDED_ /*!DOM*/

#include "DbConstraintsExports.h"
#include "DbAssocDependency.h"

#include "DbFiler.h"

#include "TD_PackPush.h"


/** \details
  This class is abstract base class for deriving the custom dependency bodies.

  Library: Source code provided.
  <group Contraint_Classes> 
*/
class DBCONSTRAINTS_EXPORT OdDbAssocDependencyBody : public OdDbObject
{
public: 
  ODRX_DECLARE_MEMBERS(OdDbAssocDependencyBody);

  /** \details
    Constructor creates an instance of this class.
  */
  OdDbAssocDependencyBody();

  /** \details
    Destructor destroys an instance of this class.
  */
  virtual ~OdDbAssocDependencyBody();

  /** \details
    Returns an object ID of the owner of this dependency.
  */
  OdDbObjectId parentDependency() const { return ownerId(); }

  /** \details
    Returns the current status of this custom dependency body.
  */
  OdDbAssocStatus status() const;

  /** \details
    Sets the status for this custom dependency body.

    \remarks
    The method can notify the owing action when the second argument is True.
  */
  OdResult setStatus(OdDbAssocStatus newStatus, 
                     bool notifyOwningAction = true);

  /** \details
    Returns an object ID of the owing action associated with this custom dependency body.
  */
  OdDbObjectId owningAction() const;

  /** \details
    Calls the method of the parent dependency that owns this custom dependency body
    to get an object ID of the object is attached to this dependency.
  */
  OdDbObjectId dependentOnObject() const;

  /** \details
    Calls the method of the parent dependency that owns this custom dependency body
    to check whether this dependency is attached to an object.
  */
  bool isAttachedToObject() const { return !dependentOnObject().isNull(); }

  /** \details
    Calls the method of the parent dependency that owns this custom dependency body
    to check whether the system is currently evaluating an action or a network of actions.
  */
  bool isActionEvaluationInProgress() const;

  /** \details
    Calls the method of the parent dependency that owns this custom dependency body
    to get the current evaluation callback set.
  */
  OdDbAssocEvaluationCallback* currentEvaluationCallback() const;

  /** \details
    Reads the .dwg file data of this object. 

    \param pFiler [in]  Filer object from which data are read.
    
    \remarks
    Returns the filer status.
  */
  OdResult dwgInFields(OdDbDwgFiler* pFiler);

  /** \details
    Writes the .dwg file data of this object. 

    \param pFiler [in]  Pointer to the filer to which data are written.
  */
  void dwgOutFields(OdDbDwgFiler* pFiler) const;

  /** \details
    Reads the .dxf file data of this object. 

    \param pFiler [in]  Filer object from which data are read.
    
    \remarks
    Returns the filer status.
  */
  OdResult dxfInFields(OdDbDxfFiler* pFiler);

  /** \details
    Writes the .dxf file data of this object. 

    \param pFiler [in]  Pointer to the filer to which data are written.
  */
  void dxfOutFields(OdDbDxfFiler* pFiler) const;

public: 
    // Virtual methods that can be overridden by the derived classes

  /** \details
    This method is called from the corresponding method of the parent class when the dependency is 
    evaluated. It is pure method and must be overridden.
  */
  virtual void evaluateOverride() = 0;

  /** \details
    This method is called from the corresponding method of the parent class when the dependency is updated. 
    Method is not implemented.
  */
  virtual OdResult updateDependentOnObjectOverride()
  { return eNotImplemented; }

  /** \details
    This method is called from the corresponding method of the parent class when the dependency is cached 
    a value. Method is not implemented.
  */
  virtual OdResult hasCachedValueOverride(bool& /*hasCachedVal*/) const
  { return eNotImplemented; }

  /** \details
    This method is called from the corresponding method of the parent class. Method is not implemented.
  */
  virtual OdResult isRelevantChangeOverride(bool& /*isRelevChange*/) const
  { return eNotImplemented; }

  /** \details
    This method is called from the corresponding method of the parent class. Method is not implemented.
  */
  virtual OdResult isDependentOnTheSameThingAsOverride(const OdDbAssocDependency* /*pOtherDependency*/, 
                                                         bool& /*isDependentOnSameThing*/) const
  { return eNotImplemented; }

  /** \details
    This method is called from the corresponding method of the parent class when the dependency is compared. 
    Method is not implemented.
  */
  virtual OdResult isEqualToOverride(const OdDbAssocDependency* /*pOtherDependency*/, bool& /*isEqual*/) const
  { return eNotImplemented; }

  /** \details
    This method is called from the corresponding method of the parent class when the dependency is erased.
  */
  virtual void erasedOverride(const OdDbObject* /*pDbObj*/, bool /*isErasing*/)
  {}

  /** \details
    This method is called from the corresponding method of the parent class when the dependency is modified.
  */
  virtual void modifiedOverride(const OdDbObject* /*pDbObj*/) 
  {}

  /** \details
    This method is called from the corresponding method of the parent class when the dependency is copied.
  */
  virtual void clonedOverride(const OdDbObject* /*pDbObj*/, const OdDbObject* /*pNewObj*/)
  {}

};


/** \details
  The typified smart pointer. This template class is specialization of the OdSmartPtr class 
  for the OdDbAssocDependencyBody class.

  \sa
  <link smart_pointers.html, Working with Smart Pointers>
*/
typedef OdSmartPtr<OdDbAssocDependencyBody> OdDbAssocDependencyBodyPtr;

#include "TD_PackPop.h"

#endif
