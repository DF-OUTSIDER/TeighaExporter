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
#ifndef DbImpAssocDependency_INCLUDED_
#define DbImpAssocDependency_INCLUDED_ /*!DOM*/

#include "DbConstraintsExports.h"
#include "DbAssocGlobal.h"
#include "DbAssocAction.h"
#include "DbObject.h"

#include "DbCompoundObjectId.h"
#include "StaticRxObject.h"


class OdDbAssocEvaluationCallback;

#include "TD_PackPush.h"

/** \details
  This class represents the custom implementation of the dependency which stores information 
  about which action objects depend on or modify an object of a database.

  Library: Source code provided.
  <group Contraint_Classes> 
*/
class DBCONSTRAINTS_EXPORT OdDbImpAssocDependency
{
public: 
  /** \details
    Constructor creates an instance of this class.
  */
  OdDbImpAssocDependency();

  /** \details
    Destructor destroys an instance of this class.
  */
  virtual ~OdDbImpAssocDependency();

  /** \details
    Returns an object ID of the associated dependency body object derived object owned 
    by this dependency or kNull if dependencies are not set for this object.
  */
  OdDbObjectId dependencyBody() const;//
    
  /** \details
    Sets the associated dependency body object owned by this dependency using the object ID. 
  */
  OdResult setDependencyBody(OdDbObjectId dependencyBodyId, OdDbObjectId dependencyId);//

  /** \details
    Returns the current status of this dependency.
  */
  OdDbAssocStatus status() const;//

  /** \details
    Sets the status for this dependency. If the status equals one of codes indicating that 
    the action owning the dependency will be evaluated.
  */
  OdResult setStatus(OdDbAssocStatus newStatus, bool notifyOwningAction, OdDbAssocDependency *pDep);//

  /** \details
    Checks whether the action owning this dependency uses the value of a depended-on object
    only for reading, that is, the dependency has the read-only status.
  */
  bool isReadDependency() const;//

  /** \details
    Checks whether the action owning this dependency modifies the value of a depended-on object, 
    that is, the dependency has the read-write status.
  */
  bool isWriteDependency() const;//

  /** \details
    Switches the read status for this dependency.
  */
  void setIsReadDependency(bool bRead);//

  /** \details
    Switches the write status for this dependency.
  */
  void setIsWriteDependency(bool bWrite);//

  /** \details
    Checks whether the action owning this dependency requires the depended-on object to be expected
    the object state relative to the list of dependencies on the object before the action to be evaluated.
  */
  bool isObjectStateDependent() const;//

  /** \details
    Switches the evaluation of the owning action depend on the state of the depended-on object.
  */
  void setIsObjectStateDependent(bool bDependent);//

  /** \details
    Returns the order of this dependency. It is the position in the list of dependencies.
  */
  int order() const;//

  /** \details
    Sets the order for this dependency.
  */
  void setOrder(int newOrder);//

  /** \details
    Sets the owning action for this dependency using the given object ID.
  */
  virtual OdResult setOwningAction(OdDbObjectId actionId, OdDbAssocDependency *pDep);//

  /** \details
    Returns an object ID of the object is attached to this dependency as a persistent reactor.
  */
  OdDbObjectId dependentOnObject() const;//

  /** \details
    Gets an object ID of the compound object which is attached to as a persistent reactor.
  */
  OdResult getDependentOnCompoundObject(OdDbCompoundObjectIdPtr&) const;

  /** \details
    Checks whether this dependency is used a compound object to refer to the depended-on object.
  */
  bool isDependentOnCompoundObject() const;//

  /** \details
    Returns an object ID of the previous dependency in the ordered list of attached dependencies.
  */
  OdDbObjectId prevDependencyOnObject() const;//

  /** \details
    Returns an object ID of the next dependency in the ordered list of attached dependencies.
  */
  OdDbObjectId nextDependencyOnObject() const;//

  /** \details
    Checks whether this dependency is attached to an object.
  */
  bool isAttachedToObject() const { return !dependentOnObject().isNull(); }

  /** \details
    Attaches this dependency to the given compound object as a previous reactor and inserts it at the 
    position in the list of dependencies, based on the value returned by the order() method. 
  */
  OdResult attachToObject(const OdDbCompoundObjectId *pCompoundId, OdDbAssocDependency *pDep);

  /** \details
    Checks whether the attachment is read-only.
  */
  virtual bool isReadyToAttach() { return true; } //This method used in OdDbImpAssocGeomDependency

  /** \details
    Checks whether the attachment is read-write.
  */
  virtual bool isNeedWriteDependency() { return false; }

  /** \details
    Returns the current status of the object which is dependent upon this dependency.
  */
  OdResult dependentOnObjectStatus() const;

  /** \details
    Detaches this dependency from an object to which it is currently attached. 
    This dependency must be currently attached to an object.
  */
  OdResult detachFromObject(OdDbAssocDependency *pDep);

  /** \details
    Updates the object depends on the dependency, based on the information obtained from the owning 
    action. If the dependency owns an dependency body object, its updateDependentOnObject() method 
    is called, otherwise the default implementation of this method does nothing.
  */
  virtual OdResult updateDependentOnObject(OdDbAssocDependency *pDep);

  /** \details
    Sets the dependent-on object for this dependency using the given compound object.
  */
  void setDependentOnObject(OdDbCompoundObjectId* pCompoundId, OdDbAssocDependency *pDep);

  /** \details
    Checks whether a dependency method delegates to the action which owns the dependency 
    if the dependency does not have implementation of this method.
  */
  bool isDelegatingToOwningAction() const;

  /** \details
    Switches the option which indicates whether some dependency methods delegate to the action that 
    owns the dependency if the dependency does not have implementation of this method.
  */
  void setIsDelegatingToOwningAction(bool bDelegating);//

  /** \details
    Checks whether the dependency has cached the value of the depended-on object.
  */
  bool hasCachedValue() const;

  /** \details
    Checks whether the changes are relevant to the depended-on object for this dependency type.
  */
  virtual bool isRelevantChange(const OdDbAssocDependency *pDep) const;//

  /** \details
    Checks whether this dependency and given dependency depend on the same object.
  */
  bool isDependentOnTheSameThingAs(const OdDbAssocDependency* pOtherDependency) const;

  /** \details
    Checks whether the depended-on object is read-only.
  */
  bool isDependentOnObjectReadOnly() const;

  /** \details
    Compares this dependency with given dependency whether they are equal. Both dependencies 
    need be opened at least for reading.
  */
  bool isEqualTo(const OdDbAssocDependency* pOtherDependency) const;

  /** \details
    Checks whether the system is currently evaluating an action or a network of actions.
  */
  bool isActionEvaluationInProgress( const OdDbAssocDependency *pDependency ) const;

  /** \details
    Returns the current evaluation callback set, or NULL if no action evaluation is in progress.
  */
  OdDbAssocEvaluationCallback* currentEvaluationCallback() const;

  /** \details
    Updates "dirty" dependencies.
  */
  void evaluate();

  /** \details
    Notification callback for OdDbObject::erased(). This method is called when the dependency is erased.
  */
  virtual void erased(const OdDbObject* dbObj, OdDbAssocDependency *pDep, bool isErasing = true);

  /** \details
    Notification callback for OdDbObject::modified(). This method is called when the dependency is modified.
  */
  virtual void modified(const OdDbObject* dbObj, OdDbAssocDependency *pDep);

  /** \details
    Notification callback for OdDbObject::copied(). This method is called when the dependency is copied.
  */
  virtual void copied(const OdDbObject* pDbObj, const OdDbObject* pNewObj, OdDbAssocDependency *pDep);

  /** \details
    Reads the .dwg file data of this object. 

    \param pFiler [in]  Filer object from which data are read.
    
    \remarks
    Returns the filer status.
  */
  virtual OdResult dwgInFields(OdDbDwgFiler* pFiler);

  /** \details
    Writes the .dwg file data of this object. 

    \param pFiler [in]  Pointer to the filer to which data are written.
  */
  virtual void dwgOutFields(OdDbDwgFiler* pFiler) const;

  /** \details
    Reads the .dxf file data of this object. 

    \param pFiler [in]  Filer object from which data are read.
    
    \remarks
    Returns the filer status.
  */
  virtual OdResult dxfInFields(OdDbDxfFiler* pFiler);

  /** \details
    Writes the .dxf file data of this object. 

    \param pFiler [in]  Pointer to the filer to which data are written.
  */
  virtual void dxfOutFields(OdDbDxfFiler* pFiler) const;

  /** Description:
  Repair OdDbAssocDependency which attached to pObj as persistent reactors
  */
  static void repairDependencies(OdDbObject* pObj);

  virtual OdStringArray compareWith(const OdDbImpAssocDependency *depIDToCompare, OdDb::DwgVersion filerVersion = OdDb::kDHL_CURRENT) const;
protected:
  /*!DOM*/
  OdDbObjectId  m_dependencyBodyId;
  /*!DOM*/
  OdDbObjectId  m_dependentOnObjectId;
  /*!DOM*/
  OdDbObjectId  m_prevDependencyOnObjectID;
  /*!DOM*/
  OdDbObjectId  m_nextDependencyOnObjectID;
  /*!DOM*/
  OdDbAssocStatus m_nStatus;
  /*!DOM*/
  OdInt32       m_nOrder;
  /*!DOM*/
  OdUInt32      m_nIndex;
  /*!DOM*/
  bool          m_bReadDep;
  /*!DOM*/
  bool          m_bWriteDep;
  /*!DOM*/
  bool          m_bObjectStateDependent;
  /*!DOM*/
  bool          m_bDelegatingToOwningAction;
  /*!DOM*/
  OdStaticRxObject<OdDbCompoundObjectId> m_refPath;

};

#include "TD_PackPop.h"

#endif
