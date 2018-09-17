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

#ifndef DbAssocAction_INCLUDED_
#define DbAssocAction_INCLUDED_ /*!DOM*/

#include "DbConstraintsExports.h"
#include "DbAssocGlobal.h"
#include "DbEntity.h"     // Because of DragStat enum

#include "StringArray.h"
#include "DbEvalVariant.h"
#include "OdValue.h"

class OdDbImpAssocAction;

#include "TD_PackPush.h"

// CS - means result string after comparizon

// Common
#define CS_UNDEFINED_PROPERTY L"undefinedProperty"
#define CS_NOT_CRITICAL L"notError"
#define CS_OBJECT_ERROR L"objectError"
#define CS_DIFFERENT_OBJECTS L"Objects are different"
#define CS_DIFFERENT_PARAM_CRITICAL L"Parameters are different! Comparizon is bad!"

// AssocDependency
#define CS_UNDEFINED_ANNOTATION_ACTIONBODY L"undefinedAnnotationActionBody"
#define CS_READ_DEP L"bReadDep"
#define CS_WRITE_DEP L"bWriteDep"
#define CS_OBJECT_STATE_DEPENDENT L"bObjectStateDependent"
#define CS_DELEGATING_TO_OWNING_ACTION L"bDelegatingToOwningAction"
#define CS_ORDER L"nOrder"
#define CS_PARAM_NAME L"nOrder"

#define CS_INDEX L"nIndex"

// AssocRotatedDimActionBody
#define CS_UNDEFINED_ANNOTATION_ACTION_BODY L"undefinedAnnotationActionBody"
#define CS_PARAM_BASED_ACTION_BODY L"paramBasedActionBody"

/** \details
  This class is the base class for actions which provide the associative behavior. 
  This class itself does not have virtual methods that could be overridden, therefore it is not 
  possible for external developers to derive directly from it and override its methods. 

  Library: Source code provided.
  <group Contraint_Classes> 
*/
class DBCONSTRAINTS_EXPORT OdDbAssocAction : public OdDbObject
{
public:     
    ODRX_DECLARE_MEMBERS(OdDbAssocAction);

    /** \details
      Constructor creates an instance of this class.
    */
    OdDbAssocAction(bool createImpObj = true);

    /** \details
      Destructor destroys an instance of this class.
    */
    ~OdDbAssocAction();

    /** \details
      Returns an object ID of the owned action instance. 
    */
    OdDbObjectId actionBody() const;
    
    /** \details
      Sets the owned action instance using an object ID.
    */
    OdResult setActionBody(const OdDbObjectId& actionBodyId);

    /** \details
      Returns the current status of this action. 
    */
    OdDbAssocStatus status() const;

    /** \details
      Sets the status for this action. 
    */
    OdResult setStatus(OdDbAssocStatus newStatus, 
                       bool notifyOwningNetwork = true,
                       bool setInOwnedActions   = false);

    /** \details
      Returns the logically owning association network, or kNull if it is the topmost. 
    */
    OdDbObjectId owningNetwork() const;

    /** \details
      Sets the logically owning association network. 
    */
    OdResult setOwningNetwork(const OdDbObjectId& networkId, bool alsoSetAsDatabaseOwner);


    /** \details
      Gets the dependencies of this action. 
    */
    OdResult getDependencies(bool readDependenciesWanted,
                             bool writeDependenciesWanted,
                             OdDbObjectIdArray& dependencyIds) const;

    /** \details
      Adds a dependency for this action. 
    */
    OdResult addDependency(const OdDbObjectId& dependencyId, 
                           bool setThisActionAsOwningAction = true);

    /** \details
      Removes a dependency from this action. 
    */
    OdResult removeDependency(const OdDbObjectId& dependencyId, 
                              bool alsoEraseIt);

    /** \details
      Clears all dependencies from this action. 
    */
    OdResult removeAllDependencies(bool alsoEraseThem);

    /** \details
      Gets an array of objects which this action depends on or modifies. 
    */
    OdResult getDependentObjects(bool readDependenciesWanted,
                                 bool writeDependenciesWanted,
                                 OdDbObjectIdArray& objectIds) const;
#if 0
    /** \details
      Checks whether the given dependency is owned by this action, either directly, 
      or if this action is a network. 
    */
    bool isOwnedDependency(const OdDbAssocDependency* pDependency) const;

    /** \details
      Checks whether the given dependency owned by this action, is external to this action. 
    */
    bool isExternalDependency(const OdDbAssocDependency* pDependency) const;
#endif
    /** \details
      Checks whether the object change is relevant to which particular dependency. 
    */
    bool isRelevantDependencyChange(const OdDbAssocDependency* pDependency) const;
#if 0
    /** \details
      Checks whether a dependency has a cached value. It is delegated from the owned 
      dependency's predicate. 
    */
    bool hasDependencyCachedValue(const OdDbAssocDependency* pDependency) const;

    /** \details
      Checks whether two depedencies depend on the same thing and allows the owning action, 
      instead of the dependency itself.
    */
    bool areDependenciesOnTheSameThing(const OdDbAssocDependency* pDependency1, 
                                       const OdDbAssocDependency* pDependency2) const;

    /** \details
      Checks whether two dependencies are equal and allows the owning action, instead of 
      the dependency itself.
    */
    bool areDependenciesEqual(const OdDbAssocDependency* pDependency1, 
                              const OdDbAssocDependency* pDependency2) const;

    /** \details
      Gets all actions depending on the given object.
    */
    static OdResult getActionsDependentOnObject(const OdDbObject* pObject,
                                                bool readDependenciesWanted,
                                                bool writeDependenciesWanted,                                   
                                                OdDbObjectIdArray& actionIds);
#endif
    /** \details
      Notifies the owning action when its objects have a dependency on have been cloned.
    */
    void dependentObjectCloned(const OdDbAssocDependency* pDependency, 
                               const OdDbObject*          pDbObj, 
                               const OdDbObject*          pNewObj);

    /** \details
      Notifies the owning action when its objects already have cloned ,but their ids haven't 
      been translated yet. It is called just before the end of the main deep-cloning loop. 
    */
    OdResult addMoreObjectsToDeepClone(OdDbIdMapping& idMap, 
                                       OdDbObjectIdArray& additionalObjectsToClone) const;

    /** \details
      Informs the source actions of the cloning object so that they can update the clones or do other 
      action if it is necessary. This method is called from reactor notification.
    */
    OdResult postProcessAfterDeepClone(OdDbIdMapping& idMap); 

    /** \details
      Informs the source actions so that it can obtain its clone. This method is called from reactor 
      notification.
    */
    OdResult postProcessAfterDeepCloneCancel(OdDbIdMapping& idMap);

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
      Aggregates all dependencies of the action evaluates the ones the need to be evaluated.
    */
    OdResult evaluateDependencies();

    /** \details
      Allows the owning action, instead of the dependency itself, to be performed.
    */
    void evaluateDependency(OdDbAssocDependency* pDependency);

    /** \details
      Notifies the action owning the dependency that its status has been changed. The dependency 
      is still opened for write and already has the new status.
    */
    OdResult ownedDependencyStatusChanged(OdDbAssocDependency* pOwnedDependency, 
                                          OdDbAssocStatus      previousStatus);
#if 0
    /** \details
      Notifes the action that its owner is transformed. For example, all entities in the block owning 
      the network that owns the action are transformed.
    */
    OdResult transformActionBy(const OdGeMatrix3d& transform);

    /** \details
      Compares this action with the given action. Returns True if and only if they are equal.
    */
    bool isEqualTo(const OdDbAssocAction* pOtherAction) const;

    /** \details
      Returns the evaluation priority of this action.
    */
    OdDbAssocEvaluationPriority evaluationPriority() const;
#endif
    /** \details
      Allows the action report on all other actions that need to be evaluated.
    */
    void getDependentActionsToEvaluate(OdDbActionsToEvaluateCallback* pActionsToEvaluateCallback) const;

    /** \details
      Evaluates this action. This method is called from client code to evaluate an action or 
      an associative network.
    */
    void evaluate(OdDbAssocEvaluationCallback* pEvaluationCallback);

    /** \details
      Returns an ID of the object that owns the network that owns this action.
    */
    OdDbObjectId objectThatOwnsNetworkInstance() const;
#if 0
    /** \details
      Notifies the action when the action depends on objects, either directly or indirectly.
    */
    void dragStatus(const OdDb::DragStat status);
#endif

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
    After loading from file, performs necessary actions which require accessing other Database 
    objects. For example, processing round-trip data.
  */
  virtual void composeForLoad(OdDb::SaveType format, OdDb::DwgVersion version, OdDbAuditInfo* pAuditInfo);

  /** \details
    Determines the behavior for custom objects when saving to an earlier version of .dwg or .dxf file.
  */
  virtual OdDbObjectPtr decomposeForSave(OdDb::DwgVersion ver, OdDbObjectId& replaceId, bool& exchangeXData);

  virtual void audit(OdDbAuditInfo* pAuditInfo);


  /** \details
    Sets the index for this action.
  */
  OdResult setActionIndex(int newIndex);

// AC2013
// OdDbAssocActionParams stuff

  /** \details
    Returns the number of the owned action parameters.
  */
  int paramCount() const;

  /** \details
    Returns an array of the owned action parameters.
  */
  const OdDbObjectIdArray& ownedParams() const;

  /** \details
    Creates and adds a new action parameter.
  */
  OdResult addParam(const OdDbObjectId& paramId, int& paramIndex);
  
  /** \details
    Creates and adds a new action parameter.
  */
  OdResult addParam(const OdString& paramName, OdRxClass* pParamClass, OdDbObjectId& paramId, int& paramIndex);

  /** \details
    Removes an existing action parameter and optionally erases it.
  */
  OdResult removeParam(const OdDbObjectId& paramId, bool alsoEraseIt);

  /** \details
    Removes all owned action parameters and optionally erases them.
  */
  OdResult removeAllParams(bool bEraseThem);

  /** \details
    Returns an array of object IDs that refer to all owned action parameters with the given name.
  */
  OdDbObjectIdArray paramsAtName(const OdString& paramName) const;

  /** \details
    Returns an owned action parameter specified by its name and optional index for parameters with the same name.
  */
  OdDbObjectId paramAtName(const OdString& paramName, int index = 0) const;

  /** \details
    Returns an owned action parameter specified by its index.
  */
  OdDbObjectId paramAtIndex(int paramIndex) const;

  /** \details
    Returns the names of all value action parameters.
  */
  void ownedValueParamNames(OdStringArray& paramNames) const;

  /** \details
    Gets an array of values, array of expressions and array of evaluatorIds of a given value action parameter.
  */
  OdResult getValueParamArray(const OdString& paramName,
                              OdArray<OdDbEvalVariantPtr>& values,
                              OdStringArray& expressions,
                              OdStringArray& evaluatorIds) const;

  /** \details
    Gets a value, expression and evaluatorIds of a given value action parameter.
  */
  OdResult getValueParam(const OdString&  paramName,
                         OdDbEvalVariant& value,
                         OdString& expression,
                         OdString& evaluatorId,
                         int valueIndex = 0) const;

  /** \details
    Sets an array of values, array of expressions and array of evaluatorIds for a given value action parameter.
  */
  OdResult setValueParamArray(const OdString& paramName,
                              const OdArray<OdDbEvalVariantPtr>& values,
                              const OdStringArray& expressions,
                              const OdStringArray& evaluatorIds,
                              OdStringArray& errorMessages,
                              bool silentMode = false);
  /** \details
    Sets a value, expression and evaluatorIds for a given value action parameter.
  */
  OdResult setValueParam(const OdString& paramName,
                         const OdDbEvalVariant& value,
                         const OdString& expression,
                         const OdString& evaluatorId,
                         OdString& errorMessage,
                         bool silentMode = false,
                         int valueIndex = 0);

  /** \details
    Returns the unit type of a given value action parameter.
  */
  OdValue::UnitType valueParamUnitType(const OdString& paramName) const;

  /** \details
    Sets the unit type for a given value action parameter.
  */
  OdResult setValueParamUnitType(const OdString& paramName, OdValue::UnitType unitType);

  /** \details
    Removes the value action parameter using its name.
  */
  OdResult removeValueParam(const OdString& paramName);

  /** \details
    An each value action parameter may either be defined by a constant, or an expression. 
    When it is an expression, the parameter stores an object ID of a read-only associated
    value dependency on an associated variable that stores the expression and provides the 
    input value. 
  */
  OdResult valueParamInputVariables(const OdString& paramName, OdDbObjectIdArray& variableIds) const;

  /** \details
    The value action parameter can store an object ID of an "output" dependency on an associated 
    object. The parameter then becomes "linked" with the named "property" of the associated object.
  */
  OdResult setValueParamControlledObjectDep(const OdString& paramName, const OdDbObjectId& controlledObjectDepId);

  /** \details
    Updates the "property" of the associated object from the current value of a value action parameter.
  */
  OdResult updateValueParamControlledObject(const OdString& paramName) const;

  /** \details
    Updates the value action parameter from the current value of a named "property" of the associated object.
  */
  OdResult updateValueParamFromControlledObject(const OdString& paramName);

  /** \details
    Updates the named properties of all objects linked with this value action parameters.
  */
  OdResult updateAllObjectsControlledByValueParams() const;

  /** \details
    Applies the transformation matrix to all constant geometries in all owned associated parameters.
  */
  OdResult transformAllConstantGeometryParams(const OdGeMatrix3d& transform);

  /** \details
    Scales the values of all value action parameters whose units indicate represent distances.
  */
  OdResult scaleAllDistanceValueParams(double scaleFactor);

  virtual OdStringArray compareWith(const OdDbObjectId bodyIDToCompare, OdDb::DwgVersion filerVersion = OdDb::kDHL_CURRENT) const;

protected:
  /*!DOM*/
  OdDbImpAssocAction *m_pImpObj;

  /*!DOM*/
  friend class OdDbImpAssocAction;
};

/** \details
  The typified smart pointer. This template class is specialization of the OdSmartPtr class 
  for the OdDbAssocAction class.

  \sa
  <link smart_pointers.html, Working with Smart Pointers>
*/
typedef OdSmartPtr<OdDbAssocAction> OdDbAssocActionPtr;

#include "TD_PackPop.h"

#endif

