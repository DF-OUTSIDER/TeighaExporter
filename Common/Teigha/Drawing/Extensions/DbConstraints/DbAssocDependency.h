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

#ifndef DbAssocDependency_INCLUDED_
#define DbAssocDependency_INCLUDED_ /*!DOM*/

#include "DbConstraintsExports.h"
#include "DbAssocGlobal.h"
#include "DbAssocAction.h"
#include "DbObject.h"
#include "DbCompoundObjectId.h"

class OdDbImpAssocDependency;
class OdDbAssocEvaluationCallback;

#include "TD_PackPush.h"

/** \details
  This is the base class that represents the dependency which stores information 
  about which action objects depend on or modify an object of a database.
  It is how associativity is represented in the drawing. Dependencies are attached 
  to the objects of a database as persistent reactors and are owned by actions.
  When the depended-on object changes, it notifies the dependency using the persistent 
  reactor mechanism. 

  Library: Source code provided.
  <group Contraint_Classes> 
*/
class DBCONSTRAINTS_EXPORT OdDbAssocDependency : public OdDbObject
{
public: 
  ODRX_DECLARE_MEMBERS(OdDbAssocDependency);

  /** \details
    Constructor creates an instance of this class.
  */
  OdDbAssocDependency(bool createImp = true);

  /** \details
    Destructor destroys an instance of this class.
  */
  ~OdDbAssocDependency();

  /** \details
    Returns an object ID of the associated dependency body object derived object owned 
    by this dependency or kNull if dependencies are not set for this object.
  */
  OdDbObjectId dependencyBody() const;
    
  /** \details
    Sets the object ID for this dependency body.
    Sets the associated dependency body object object owned by this dependency. 
    This object must implement the body of the custom dependency.
  */
  OdResult setDependencyBody(OdDbObjectId dependencyBodyId);

  /** \details
    Returns the current status of this dependency.
  */
  OdDbAssocStatus status() const;

  /** \details
    Sets the status for this dependency. If the status equals one of codes indicating that 
    the action owning the dependency will be evaluated.

    \remarks
    The method can notify the owing action when the second argument is True.
  */
  OdResult setStatus(OdDbAssocStatus newStatus, bool notifyOwningAction = true);

  /** \details
    Checks whether the action owning this dependency uses the value of a depended-on object
    only for reading, that is, the dependency has the read-only status.
  */
  bool isReadDependency() const;

  /** \details
    Switches the read status for this dependency.
  */
  void setIsReadDependency(bool bRead);

  /** \details
    Checks whether the action owning this dependency modifies the value of a depended-on object, 
    that is, the dependency has the read-write status.
  */
  bool isWriteDependency() const;

  /** \details
    Switches the write status for this dependency.
  */
  void setIsWriteDependency(bool bWrite);

  /** \details
    Checks whether the action owning this dependency requires the depended-on object to be expected
    the object state relative to the list of dependencies on the object before the action to be evaluated.
  */
  bool isObjectStateDependent() const;

  /** \details
    Switches the evaluation of the owning action depend on the state of the depended-on object.
  */
  void setIsObjectStateDependent(bool bDependent);

  /** \details
    Returns the order of this dependency. It is the position in the list of dependencies.
  */
  int order() const;

  /** \details
    Sets the order for this dependency.
  */
  void setOrder(int newOrder);

  /** \details
    Returns an object ID of the owning action associated with this dependency.
  */
  OdDbObjectId owningAction() const { return ownerId(); }

  /** \details
    Sets the owning action for this dependency using the given object ID.
  */
  OdResult setOwningAction(OdDbObjectId actionId);

  /** \details
    Returns an object ID of the object is attached to this dependency as a persistent reactor.
  */
  OdDbObjectId dependentOnObject() const;

  /** \details
    Gets an object ID of the compound object which is attached to as a persistent reactor.
  */
  OdResult getDependentOnCompoundObject(OdDbCompoundObjectIdPtr&) const;

  /** \details
    Checks whether this dependency is used a compound object to refer to the depended-on object.
  */
  bool isDependentOnCompoundObject() const;

  /** \details
    Returns an object ID of the previous dependency in the ordered list of attached dependencies.
  */
  OdDbObjectId prevDependencyOnObject() const;

  /** \details
    Returns an object ID of the next dependency in the ordered list of attached dependencies.
  */
  OdDbObjectId nextDependencyOnObject() const;

  /** \details
    Checks whether this dependency is attached to an object.
  */
  bool isAttachedToObject() const { return !dependentOnObject().isNull(); }

  /** \details
    Attaches this dependency to the given compound object as a previous reactor and inserts it at the 
    position in the list of dependencies, based on the value returned by the order() method. 
  */
  OdResult attachToObject(const OdDbCompoundObjectId* compoundId);

  /** \details
    Returns the current status of the object which is dependent upon this dependency.
  */
  OdResult dependentOnObjectStatus() const;

  /** \details
    Detaches this dependency from an object to which it is currently attached. 
    This dependency must be currently attached to an object.
  */
  OdResult detachFromObject();

  /** \details
    Updates the object depends on the dependency, based on the information obtained from the owning 
    action. If the dependency owns an dependency body object, its updateDependentOnObject() method 
    is called, otherwise the default implementation of this method does nothing.
  */
  OdResult updateDependentOnObject();

  /** \details
    Sets the dependent-on object for this dependency using the given compound object.
  */
  void setDependentOnObject(OdDbCompoundObjectId* compoundId);

  /** \details
    Gets an object ID of the dependency object that is first in the list of dependencies attached 
    to the given object.
  */
  static OdDbObjectId getFirstDependencyOnObject(OdDbObjectPtr pObject);

  /** \details
    Gets all dependency objects that are dependent on the given object.
  */
  static void getDependenciesOnObject(OdDbObjectPtr pObject, 
                                      bool readDependenciesWanted, 
                                      bool writeDependenciesWanted,
                                      OdDbObjectIdArray& dependencyIds);

  /** \details
    Notifies all dependencies on the given object which has changed by setting the dependency status.
  */
  static OdResult notifyDependenciesOnObject(OdDbObjectPtr pObject,OdDbAssocStatus newStatus);

  /** \details
    Checks whether a dependency method delegates to the action which owns the dependency 
    if the dependency does not have implementation of this method.
  */
  bool isDelegatingToOwningAction() const;

  /** \details
    Switches the option which indicates whether some dependency methods delegate to the action that 
    owns the dependency if the dependency does not have implementation of this method.
  */
  void setIsDelegatingToOwningAction(bool bDelegating);

  /** \details
    Checks whether the dependency has cached the value of the depended-on object.
  */
  bool hasCachedValue() const;

  /** \details
    Checks whether the changes are relevant to the depended-on object for this dependency type.
  */
  bool isRelevantChange() const;

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
  bool isActionEvaluationInProgress() const;

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

    \remarks
    This method should not be overridden; it should not be called directly from client code.
  */
  void erased(const OdDbObject* dbObj, bool isErasing = true);

  /** \details
    Notification callback for OdDbObject::modified(). This method is called when the dependency is modified.
  */
  void modified(const OdDbObject* dbObj);

  /** \details
    Notification callback for OdDbObject::copied(). This method is called when the dependency is copied.
  */
  void copied(const OdDbObject* pDbObj, const OdDbObject* pNewObj);

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

  virtual OdStringArray compareWith(const OdDbObjectId paramIDToCompare, OdDb::DwgVersion filerVersion = OdDb::kDHL_CURRENT) const;
protected:
  /*!DOM*/
  friend class OdDbImpAssocDependency;
  /*!DOM*/
  OdDbImpAssocDependency *m_pImpObj;

};


/** \details
  The typified smart pointer for the dependency object. This template class 
  is specialization of the OdSmartPtr class for the OdDbAssocDependency class.

  \sa
  <link smart_pointers.html, Working with Smart Pointers>
*/
typedef OdSmartPtr<OdDbAssocDependency> OdDbAssocDependencyPtr;

#include "TD_PackPop.h"

#endif
