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

#ifndef DbAssocActionBody_INCLUDED_
#define DbAssocActionBody_INCLUDED_ /*!DOM*/

#include "DbConstraintsExports.h"
#include "DbObject.h"
#include "DbAssocAction.h"
#include "DbEntity.h"

#include "TD_PackPush.h"

/** \details
  This class is abstract base class for deriving custom action body.

  Library: Source code provided.
  <group Contraint_Classes> 
*/
class DBCONSTRAINTS_EXPORT ODRX_ABSTRACT OdDbAssocActionBody : public OdDbObject
{
public: 
  ODRX_DECLARE_MEMBERS(OdDbAssocActionBody);

  /** \details
    Constructor creates an instance of this class.
  */
  OdDbAssocActionBody();

  //explicit AcDbAssocActionBody(AcDbAssocCreateImpObject createImpObject = kAcDbAssocCreateImpObject);

  /** \details
    Destructor destroys an instance of this class.
  */
  virtual ~OdDbAssocActionBody();

  /** \details
    Returns an object ID of the parent action. 
  */
  OdDbObjectId parentAction() const { return ownerId(); }

  /** \details
    Returns the current status of this action. 
  */
  OdDbAssocStatus status() const;

  /** \details
    Sets the status for this action. 
  */
  OdResult setStatus(OdDbAssocStatus newStatus, 
                      bool           notifyOwningNetwork = true,
                      bool           setInOwnedActions   = false);

  /** \details
    Returns the logically owning association network, or kNull if it is the topmost. 
  */
  OdDbObjectId owningNetwork() const;

  /** \details
    Gets the dependencies of this action. The default implementation obtains all dependencies 
    owned by the base action class.
  */
  OdResult getDependencies(bool readDependenciesWanted, 
                           bool writeDependenciesWanted,
                           OdDbObjectIdArray& dependencyIds) const; 

  /** \details
    Adds a dependency for this action. The default implementation calls the corresponding method 
    of the parent class that owns this action.
  */
  OdResult addDependency(const OdDbObjectId& dependencyId, 
                         bool  setThisActionAsOwningAction = true); 

  /** \details
    Removes a dependency from this action. 
  */
  OdResult removeDependency(const OdDbObjectId& dependencyId, 
                            bool          alsoEraseIt);

  /** \details
    Clears all dependencies from this action. 
  */
  OdResult removeAllDependencies(bool alsoEraseThem);

  /** \details
    Aggregates all dependencies of the action evaluates the ones the need to be evaluated.
  */
  OdResult evaluateDependencies();

  /** \details
    Checks whether the system is evaluating an action or a network of actions.
  */
  bool isActionEvaluationInProgress() const;

  /** \details
    Returns the current association evaluation callback set instance, or NULL if no action evaluation 
    is in progress.
  */
  OdDbAssocEvaluationCallback* currentEvaluationCallback() const;
  
  /** \details
    Creates ODDbAssocAction and ODDbAssocActionBody
    of the required class, makes the action own the action body and posts 
    both to the database of the given objectId.     
  */
  static OdResult createActionAndActionBodyAndPostToDatabase( OdRxClass* pActionBodyClass, const OdDbObjectId& objectId, OdDbObjectId& createdActionId, OdDbObjectId& createdActionBodyId);
public: 
  /** \details
    Method is called by the corresponding method of the parent action which owns this action
    (see OdDbAssocAction::evaluate() method).
  */
  virtual void evaluateOverride() = 0;

  /** \details
    Method is called by the corresponding method of the parent action which owns this action
    (see OdDbAssocAction::getDependencies() method).

    \remarks Not implemented
  */
  OdResult getDependenciesOverride(bool readDependenciesWanted, 
                                   bool writeDependenciesWanted,
                                   OdDbObjectIdArray& dependencyIds) const
  { return eNotImplemented; }

  /** \details
    Method is called by the corresponding method of the parent action which owns this action
    (see OdDbAssocAction::getDependentObjects() method).

    \remarks Not implemented
  */
  OdResult getDependentObjectsOverride(bool readDependenciesWanted,
                                       bool writeDependenciesWanted,
                                       OdDbObjectIdArray& objectIds) const
  { return eNotImplemented; }

  /** \details
    Method is called by the corresponding method of the parent action which owns this action
    (see OdDbAssocAction::removeAllDependencies() method).

    \remarks Not implemented
  */
  virtual OdResult removeAllDependenciesOverride(bool alsoEraseThem)
  { return eNotImplemented; }

  /** \details
    Method is called by the corresponding method of the parent action which owns this action
    (see OdDbAssocAction::isOwnedDependency() method).

    \remarks Not implemented
  */
  virtual OdResult isOwnedDependencyOverride(const OdDbAssocDependency* pDependency, 
                                             bool& isOwnedDependency) const
  { return eNotImplemented; }

  /** \details
    Method is called by the corresponding method of the parent action which owns this action
    (see OdDbAssocAction::isExternalDependency() method).

    \remarks Not implemented
  */
  virtual OdResult isExternalDependencyOverride(const OdDbAssocDependency* pDependency, 
                                                      bool& isExternalDependency) const
  { return eNotImplemented; }

  /** \details
    Method is called by the corresponding method of the parent action which owns this action
    (see OdDbAssocAction::hasDependencyCachedValue() method).

    \remarks Not implemented
  */
  virtual OdResult hasDependencyCachedValueOverride(const OdDbAssocDependency* pDependency,
                                                             bool& hasDepCachedValue) const
  { return eNotImplemented; }

  /** \details
    Method is called by the corresponding method of the parent action which owns this action
    (see OdDbAssocAction::isRelevantDependencyChange() method).

    \remarks Not implemented
  */
  virtual OdResult isRelevantDependencyChangeOverride(const OdDbAssocDependency* pDependency,
                                                      bool& isRelevantDepChange) const
  { return eNotImplemented; }

  /** \details
    Method is called by the corresponding method of the parent action which owns this action
    (see OdDbAssocAction::areDependenciesOnTheSameThing() method).

    \remarks Not implemented
  */
  virtual OdResult areDependenciesOnTheSameThingOverride(const OdDbAssocDependency* pDependency1,
                                                         const OdDbAssocDependency* pDependency2,
                                                         bool& areDependentOnSameThing) const
  { return eNotImplemented; }

  /** \details
    Method is called by the corresponding method of the parent action which owns this action
    (see OdDbAssocAction::areDependenciesEqual() method).

    \remarks Not implemented
  */
  virtual OdResult areDependenciesEqualOverride(const OdDbAssocDependency* pDependency1,
                                                const OdDbAssocDependency* pDependency2,
                                                bool& areEqual) const
  { return eNotImplemented; }

  /** \details
    Method is called by the corresponding method of the parent action which owns this action
    (see OdDbAssocAction::evaluateDependency() method).

    \remarks Not implemented
  */
  virtual OdResult evaluateDependencyOverride(OdDbAssocDependency* pDependency)
  { return eNotImplemented; }

  /** \details
    Method is called by the corresponding method of the parent action which owns this action
    (see OdDbAssocAction::dependentObjectCloned() method).

    \remarks Not implemented
  */
  virtual OdResult dependentObjectClonedOverride(const OdDbAssocDependency* pDependency, 
                                                 const OdDbObject*          pDbObj, 
                                                 const OdDbObject*          pNewObj)
  { return eNotImplemented; }

  /** \details
    Method is called by the corresponding method of the parent action which owns this action
    (see OdDbAssocAction::addMoreObjectsToDeepClone() method).

    \remarks Not implemented
  */
  virtual OdResult addMoreObjectsToDeepCloneOverride(OdDbIdMapping& idMap, 
                                                     OdDbObjectIdArray& additionalObjectsToClone) const
  { return eNotImplemented; }

  /** \details
    Method is called by the corresponding method of the parent action which owns this action
    (see OdDbAssocAction::postProcessAfterDeepClone() method).

    \remarks Not implemented
  */
  virtual OdResult postProcessAfterDeepCloneOverride(OdDbIdMapping& idMap) 
  { return eNotImplemented; }

  /** \details
    Method is called by the corresponding method of the parent action which owns this action
    (see OdDbAssocAction::postProcessAfterDeepCloneCancel() method).

    \remarks Not implemented
  */
  virtual OdResult postProcessAfterDeepCloneCancelOverride(OdDbIdMapping& idMap)
  { return eNotImplemented; }

  /** \details
    Method is called by the corresponding method of the parent action which owns this action
    (see OdDbAssocAction::ownedDependencyStatusChanged() method).

    \remarks Not implemented
  */
  virtual OdResult ownedDependencyStatusChangedOverride(OdDbAssocDependency* pOwnedDependency, 
                                                        OdDbAssocStatus      previousStatus)
  { return eNotImplemented; }

  /** \details
    Method is called by the corresponding method of the parent action which owns this action
    (see OdDbAssocAction::isEqualTo() method).

    \remarks Not implemented
  */
  virtual OdResult isEqualToOverride(const OdDbAssocAction* pOtherAction, bool& isEqual) const
  { return eNotImplemented; }

  /** \details
    Method is called by the corresponding method of the parent action which owns this action
    (see OdDbAssocAction::evaluationPriority() method).

    \remarks Not implemented
  */
  virtual OdResult evaluationPriorityOverride(OdDbAssocEvaluationPriority& priority) const
  { return eNotImplemented; }

  /** \details
    Method is called by the corresponding method of the parent action which owns this action
    (see OdDbAssocAction::getDependentActionsToEvaluate() method).

    \remarks Not implemented
  */
  virtual OdResult getDependentActionsToEvaluateOverride(OdDbActionsToEvaluateCallback* pActionsToEvaluateCallback) const
  { return eNotImplemented; }

  /** \details
    Method is called by the corresponding method of the parent action which owns this action
    (see OdDbAssocAction::transformActionBy() method).

    \remarks Not implemented
  */
  virtual OdResult transformActionByOverride(const OdGeMatrix3d&)
  { return eNotImplemented; }

  /** \details
    Method is called by the corresponding method of the parent action which owns this action
    (see OdDbAssocAction::dragStatus() method).

    \remarks Not implemented
  */
  virtual OdResult dragStatusOverride(const OdDb::DragStat status)
  { return eNotImplemented; }

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
  
  /** \details
    Just a shortcut to OdDbAssocAction method.
  */
  OdResult getValueParam( const OdString& paramName,
                          OdDbEvalVariant& value,
                          OdString& expression,
                          OdString& evaluatorId,
                          int valueIndex = 0) const; 


  /** \details
    Just a shortcut to OdDbAssocAction method.
  */
  OdResult setValueParam( const OdString& paramName,
                          const OdDbEvalVariant& value,
                          const OdString& expression,
                          const OdString& evaluatorId,
                          OdString& errorMessage,
                          bool silentMode,
                          int valueIndex = 0);

  /** \details
    Just a shortcut to OdDbAssocAction method.
  */
  OdDbObjectId paramAtName(const OdString& paramName, int index = 0) const;

  /** \details
    Just a shortcut to OdDbAssocAction method.
  */
  OdValue::UnitType valueParamUnitType(const OdString& paramName) const;

  /** \details
    Just a shortcut to OdDbAssocAction method.
  */
  OdResult setValueParamUnitType(const OdString& paramName, OdValue::UnitType unitType);
  /** \details
    Just a shortcut to OdDbAssocAction method.
  */
  OdResult addParam( const OdString& paramName, 
                     OdRxClass* pParamClass, 
                     OdDbObjectId& paramId, 
                     int& paramIndex);
  /** \details
    Just a shortcut to OdDbAssocAction method.
  */
  void ownedValueParamNames(OdStringArray& paramNames) const;

  virtual OdStringArray compareWith(const OdDbObjectId bodyIDToCompare, OdDb::DwgVersion filerVersion = OdDb::kDHL_CURRENT) const;
protected:
  /*!DOM*/
  friend class OdDbImpAssocAction;
  /*!DOM*/
  class OdDbImpAssocActionBody* m_pImpObj;
};
/** \details
  The typified smart pointer. This template class is specialization of the OdSmartPtr class 
  for the OdDbAssocActionBody class.

  \sa
  <link smart_pointers.html, Working with Smart Pointers>
*/
typedef OdSmartPtr<OdDbAssocActionBody> OdDbAssocActionBodyPtr;

#include "TD_PackPop.h"
#endif

