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

#ifndef DbImpAssocAction_INCLUDED_
#define DbImpAssocAction_INCLUDED_ /*!DOM*/

#include "DbConstraintsExports.h"
#include "DbAssocGlobal.h"
#include "DbEntity.h"     // Because of DragStat enum
#include "DbImpAssocActionBody.h"

#include "TD_PackPush.h"


/** \details
  This class represents the custom implementation of an associated actions.

  Library: Source code provided.
  <group Contraint_Classes> 
*/
class DBCONSTRAINTS_EXPORT OdDbImpAssocAction
{
public:     
    /** \details
      Constructor creates an instance of this class.
    */
    OdDbImpAssocAction();

    /** \details
      Destructor destroys an instance of this class.
    */
    virtual ~OdDbImpAssocAction();

    /** \details
      Returns a pointer to the implementation of associated action object.
    */
    static OdDbImpAssocAction* getImpObject(OdDbAssocAction *pAction);

    /** \details
      Returns an object ID of the owned action instance. 
    */
    virtual OdDbObjectId actionBody() const;
    
    /** \details
      Sets the owned action instance using an object ID.
    */
    virtual OdResult setActionBody(OdDbObjectId& actionId, const OdDbObjectId& actionBodyId);

    /** \details
      Returns the current status of this action. 
    */
    virtual OdDbAssocStatus status() const;

    /** \details
      Sets the status for this action. 
    */
    virtual OdResult setStatus(OdDbAssocStatus newStatus, 
                               bool notifyOwningNetwork = true,
                               bool setInOwnedActions   = false);

    /** \details
      Returns the logically owning association network, or kNull if it is the topmost. 
    */
    virtual OdDbObjectId owningNetwork() const;

    /** \details
      Sets the logically owning association network. 
    */
    virtual OdResult setOwningNetwork(OdDbAssocAction *pAction, const OdDbObjectId& networkId, bool alsoSetAsDatabaseOwner);

    /** \details
      Gets the dependencies of this action. 
    */
    virtual OdResult getDependencies(bool readDependenciesWanted,
                                     bool writeDependenciesWanted,
                                     OdDbObjectIdArray& dependencyIds) const;

    /** \details
      Adds a dependency for this action. 
    */
    virtual OdResult addDependency(const OdDbObjectId& dependencyId, 
                                   bool setThisActionAsOwningAction,
                                   OdDbAssocAction *pAction);

    /** \details
      Removes a dependency from this action. 
    */
    virtual OdResult removeDependency(const OdDbObjectId& dependencyId, 
                                      bool alsoEraseIt);

    /** \details
      Clears all dependencies from this action. 
    */
    virtual OdResult removeAllDependencies(bool alsoEraseThem);

    /** \details
      Gets an array of objects which this action depends on or modifies. 
    */
    virtual OdResult getDependentObjects(bool readDependenciesWanted,
                                 bool writeDependenciesWanted,
                                 OdDbObjectIdArray& objectIds) const;
#if 0
    /** \details
      Checks whether the given dependency is owned by this action, either directly, 
      or if this action is a network. 
    */
    virtual bool isOwnedDependency(const OdDbAssocDependency* pDependency) const;

    /** \details
      Checks whether the given dependency owned by this action, is external to this action. 
    */
    virtual bool isExternalDependency(const OdDbAssocDependency* pDependency) const;
#endif
    /** \details
      Checks whether the object change is relevant to which particular dependency. 
    */
    virtual bool isRelevantDependencyChange(const OdDbAssocDependency* pDependency) const;
#if 0
    /** \details
      Checks whether a dependency has a cached value. It is delegated from the owned 
      dependency's predicate. 
    */
    virtual bool hasDependencyCachedValue(const OdDbAssocDependency* pDependency) const;

    /** \details
      Checks whether two depedencies depend on the same thing and allows the owning action, 
      instead of the dependency itself.
    */
    virtual bool areDependenciesOnTheSameThing(const OdDbAssocDependency* pDependency1, 
                                               const OdDbAssocDependency* pDependency2) const;

    /** \details
      Checks whether two dependencies are equal and allows the owning action, instead of 
      the dependency itself.
    */
    virtual bool areDependenciesEqual(const OdDbAssocDependency* pDependency1, 
                                      const OdDbAssocDependency* pDependency2) const;

#endif
    /** \details
      Notifies the owning action when its objects have a dependency on have been cloned.
    */
    virtual void dependentObjectCloned(const OdDbAssocDependency* pDependency, 
                               const OdDbObject*          pDbObj, 
                               const OdDbObject*          pNewObj);

    /** \details
      Notifies the owning action when its objects already have cloned ,but their ids haven't 
      been translated yet. It is called just before the end of the main deep-cloning loop. 
    */
    virtual OdResult addMoreObjectsToDeepClone(OdDbIdMapping& idMap, 
                                       OdDbObjectIdArray& additionalObjectsToClone) const;


    /** \details
      Informs the source actions of the cloning object so that they can update the clones or do other 
      action if it is necessary. This method is called from reactor notification.
    */
    virtual OdResult postProcessAfterDeepClone(OdDbAssocAction *pAction, OdDbIdMapping& idMap); 

    /** \details
      Informs the source actions so that it can obtain its clone. This method is called from reactor 
      notification.
    */
    virtual OdResult postProcessAfterDeepCloneCancel(OdDbAssocAction *pAction, OdDbIdMapping& idMap);

    /** \details
      Checks whether the system is evaluating an action or a network of actions.
    */
    virtual bool isActionEvaluationInProgress() const;

    /** \details
      Returns the current association evaluation callback set instance, or NULL if no action evaluation 
      is in progress.
    */
    virtual OdDbAssocEvaluationCallback* currentEvaluationCallback() const;

    /** \details
      Aggregates all dependencies of the action evaluates the ones the need to be evaluated.
    */
    virtual OdResult evaluateDependencies();

    /** \details
      Allows the owning action, instead of the dependency itself, to be performed.
    */
    virtual void evaluateDependency(OdDbAssocDependency* pDependency);

    /** \details
      Notifies the action owning the dependency that its status has been changed. The dependency 
      is still opened for write and already has the new status.
    */
    virtual OdResult ownedDependencyStatusChanged(OdDbAssocDependency* pOwnedDependency, 
                                          OdDbAssocStatus      previousStatus);
#if 0
    /** \details
      Notifes the action that its owner is transformed. For example, all entities in the block owning 
      the network that owns the action are transformed.
    */
    virtual OdResult transformActionBy(const OdGeMatrix3d& transform);

    /** \details
      Compares this action with the given action. Returns True if and only if they are equal.
    */
    virtual bool isEqualTo(const OdDbAssocAction* pOtherAction) const;

    /** \details
      Returns the evaluation priority of this action.
    */
    virtual OdDbAssocEvaluationPriority evaluationPriority() const;
#endif
    /** \details
      Allows the action report on all other actions that need to be evaluated.
    */
    virtual void getDependentActionsToEvaluate(OdDbActionsToEvaluateCallback* pActionsToEvaluateCallback) const;

    /** \details
      Evaluates this action. This method is called from client code to evaluate an action or 
      an associative network.
    */
    virtual void evaluate(OdDbAssocAction *pAction, OdDbAssocEvaluationCallback* pEvaluationCallback);

    /** \details
      Returns an ID of the object that owns the network that owns this action.
    */
    OdDbObjectId objectThatOwnsNetworkInstance(const OdDbObject *pObj) const;
#if 0
    /** \details
      Notifies the action when the action depends on objects, either directly or indirectly.
    */
    virtual void dragStatus(const OdDb::DragStat status);
#endif

  /** \details
    Reads the .dwg file data of this object. 

    \param pFiler [in]  Filer object from which data are read.
    
    \remarks
    Returns the filer status.
  */
  virtual OdResult dwgInFields(OdDbDwgFiler* pFiler, OdDbObjectId objectId);

  /** \details
    Writes the .dwg file data of this object. 

    \param pFiler [in]  Pointer to the filer to which data are written.
  */
  virtual void dwgOutFields(OdDbDwgFiler* pFiler, OdDbObjectId objectId) const;

  /** \details
    Reads the .dxf file data of this object. 

    \param pFiler [in]  Filer object from which data are read.
    
    \remarks
    Returns the filer status.
  */
  virtual OdResult dxfInFields(OdDbDxfFiler* pFiler, OdDbObjectId objectId);

  /** \details
    Writes the .dxf file data of this object. 

    \param pFiler [in]  Pointer to the filer to which data are written.
  */
  virtual void dxfOutFields(OdDbDxfFiler* pFiler, OdDbObjectId objectId) const;

  /** \details
    After loading from file, performs necessary actions which require accessing other Database 
    objects. For example, processing round-trip data.
  */
  virtual void composeForLoad(OdDbObject* pObj, OdDb::SaveType format, OdDb::DwgVersion version, OdDbAuditInfo* pAuditInfo );

  /** \details
    Determines the behavior for custom objects when saving to an earlier version of .dwg or .dxf file.
  */
  virtual void decomposeForSave(OdDbObject* pObj, OdDb::SaveType format, OdDb::DwgVersion version);

  virtual void audit(OdDbObject* pObj, OdDbAuditInfo* pAuditInfo);

  /** \details
    Sets the index for this action.
  */
  OdResult setActionIndex(int newIndex);

  /** \details
    Gets the maximal index of the associated dependency.
  */
  int getMaximumAssocDepIndex();

  /** \details
    Sets the maximal index of the associated dependency for this action.
  */
  void setMaximumAssocDepIndex(int index);

  /** \details
    Performs the post evaluating.
  */
  virtual void doPostEvaluateJob(OdDbAssocAction *pAction);

// AC2013
// OdDbAssocActionParams stuff

  /** \details
    Removes all owned action parameters and optionally erases them.
  */
  OdResult removeAllParams(bool bEraseThem);

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
  OdResult addParam(const OdString& paramName, OdRxClass* pParamClass, 
                    OdDbObjectId& paramId, int& paramIndex, OdDbAssocAction *AssocAction);

//#if defined(DB_ASSOC_ACTION_PARAM_METHOGS_DEFINED)

//#endif

  /** \details
    Removes an existing action parameter and optionally erases it.
  */
  OdResult removeParam(const OdDbObjectId& paramId, bool alsoEraseIt);

//#if defined(DB_ASSOC_ACTION_PARAM_METHOGS_DEFINED)

  /** \details
    Returns an array of object IDs that refer to all owned action parameters with the given name.
  */
  OdDbObjectIdArray paramsAtName(const OdString& paramName) const;

  /** \details
    Returns an owned action parameter specified by its name and optional index for parameters with the same name.
  */
  OdDbObjectId paramAtName(const OdString& paramName, int index = 0) const;

//#endif

  /** \details
    Returns an owned action parameter specified by its index.
  */
  OdDbObjectId paramAtIndex(int paramIndex) const;

//#if defined(DB_ASSOC_ACTION_PARAM_METHOGS_DEFINED)

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
  OdResult getValueParam(const OdString& paramName,
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

//#endif
  virtual OdStringArray compareWith(const OdDbImpAssocAction *bodyIDToCompare, OdDb::DwgVersion filerVersion = OdDb::kDHL_CURRENT) const;

protected:
  /*!DOM*/
  int findDependenciesArrayIndex(OdDbObjectId objectId);

  /*!DOM*/
  OdDbObjectIdArray m_arrDependencies;
  /*!DOM*/
  OdDbObjectId m_idActionBody;
  /*!DOM*/
  OdDbObjectId m_idOwningNetwork;
  /*!DOM*/
  OdDbAssocStatus m_status; //??
  /*!DOM*/
  OdInt32 m_evalPriority; //??
  /*!DOM*/
  OdUInt32 m_Unk_UInt32_1;
  /*!DOM*/
  OdUInt32 m_actionIndex;
  /*!DOM*/
  OdUInt32 m_maxAssocDepIdx;
  /*!DOM*/
  bool m_isBase;
  // see also OdDbImpAssocParamBasedActionBody (R24)
  // R27:
  /*!DOM*/
  OdDbObjectIdArray m_arrParams;
  /*!DOM*/
  OdArray<OdDbImpValueParamPtr> m_arrValueParams;

  bool m_EvaluationInProgress;

};

#include "TD_PackPop.h"

#endif

