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

#ifndef DbImpAssocVariable_INCLUDED_
#define DbImpAssocVariable_INCLUDED_ /*!DOM*/

#include"DbImpAssocAction.h"
#include "DbAssocVariable.h"
#include "DbEvalVariant.h"  
#include "BoolArray.h"
#include "Int16Array.h"

#include "TD_PackPush.h"

/** \details
  This class represents the custom implementation of the variable which stores 
  a scalar value of following types: double, int, or string. 

  Library: Source code provided.
  <group Contraint_Classes> 
*/
class DBCONSTRAINTS_EXPORT OdDbImpAssocVariable : public OdDbImpAssocAction
{
public:     
  /** \details
    Constructor creates an instance of this class.
  */
  OdDbImpAssocVariable();

  /** \details
    Destructor destroys an instance of this class.
  */
  virtual ~OdDbImpAssocVariable();

  /** \details
    Returns the variable name as an arbitrary string.
  */
  const OdString name() const;

  /** \details
    Returns the expression as an arbitrary string or an empty string if the variable is a constant.
  */
  const OdString expression() const;

  /** \details
    Returns the evaluated value of the variable.
  */
  const OdDbEvalVariantPtr value() const;

  /** \details
    Returns the variable description as an arbitrary string.
  */
  const OdString description() const;

  /** \details
    Returns the name of the variable using a given string.
  */
  OdResult setName(const OdString newName, bool updateReferencingExpressions);

  /** \details
    Evaluates this variable for an action.
  */
  virtual void evaluate(OdDbAssocAction* pAction, OdDbAssocEvaluationCallback* pEvaluationCallback);

  /** \details
    Sets the name for this variable using a given string.
  */
  OdResult setName(OdDbAssocVariable* pVariable, const OdString newName, bool updateReferencingExpressions);

  /** \details
    Sets the constant value for this variable.
  */
  OdResult fixVariable(const OdString& name, OdDbEvalVariantPtr value);

  /** \details
    Renames the variable.
  */
  OdResult renameVariable(const OdString& oldName, const OdString& newName);

  /** \details
    Checks the cyclic dependencies for specified names and reactors.
  */
  static OdResult checkCyclicDependencies(OdArray<OdString>& names, OdDbObjectIdArray& reactors);

  /** \details
    Searches the object using its name and returns an object ID if the object is found or 
    kNull if the object is not found.
  */
  OdDbObjectId findObjectByName(const OdString  objectName, 
                                const OdRxClass* pObjectClass) const;

  /** \details
    Validates the given name, expression or both, to check whether they can be used for this variable.
  */
  OdResult validateNameAndExpression(const OdString nameToValidate, 
                                              const OdString expressionToValidate,
                                              OdString       errorMessage) const;

  /** \details
    Sets the expression as a string and syntactically checks it.
  */
  OdResult setExpression(OdDbAssocVariable* pVariable,
                        const OdString newExpression,
                        const OdString evaluatorId, 
                        bool checkForCyclicalDependencies,
                        bool updateDependenciesOnReferencedSymbols,
                        OdString errorMessage = OdString::kEmpty,
                        bool silentMode = false);

  /** \details
    Returns the ID of the expression evaluator for parsing the expression.
  */
  const OdString evaluatorId() const;

  /** \details
    Sets the given ID of the expression evaluator to be used for parsing the expression. 
    An empty string sets the default evaluator.
  */
  OdResult setEvaluatorId(const OdString evalId);

  /** \details
    Sets the evaluated value. If the expression is not empty, this value will be overwritten 
    and the expression will be evaluated.
  */
  OdResult setValue(const OdDbEvalVariant& newValue);

  /** \details
    Sets the description for this variable as a string value.
  */
  OdResult setDescription(const OdString newDescription);

  /** \details
    For internal use.
  */
  bool     isMergeable          () const;

  /** \details
    For internal use.
  */
  bool     mustMerge            () const;

  /** \details
    For internal use.
  */
  OdString mergeableVariableName() const;

  /** \details
    For internal use.
  */
  void setIsMergeable(bool isMerg, bool mustMerg = false, const OdString mergeableVariableName = OdString());

  /** \details
    Evaluates the expression.
  */
  OdResult evaluateExpression(OdDbEvalVariant& evaluatedExpressionValue,
                                       OdString        errorMessage = OdString()) const;

  OdResult evaluateExpression(OdDbObjectIdArray& objectIds,    // in/out argument
                              // TODO OdArray<OdDbEvalVariantPtr>&  
                              OdArray<OdDbEvalVariant>& objectValues, // in/out argument
                              OdDbEvalVariant& evaluatedExpressionValue,
                              OdString errorMessage = OdString()) const;

  /** \details
    Adds the callback to this variable.
 
    \remarks
    The client code can register callback(s) derived from associated callback that are 
    then called when some events are triggered for variables.
  */
  static void addGlobalCallback(class OdDbAssocVariableCallback* pCallback);

  /** \details
    Removes the callback from this variable.
  */
  static void removeGlobalCallback(class OdDbAssocVariableCallback* pCallback);

  /** \details
    Returns the callback that is the collection of all associated callbacks registered by client code.
  */
  static class OdDbAssocVariableCallback* globalCallback();

  /** \details
    Reads the .dwg file data of this object. 

    \param pFiler [in]  Filer object from which data are read.
    
    \remarks
    Returns the filer status.
  */
  OdResult dwgInFields(OdDbDwgFiler* pFiler, OdDbObjectId objectId);

  /** \details
    Writes the .dwg file data of this object. 

    \param pFiler [in]  Pointer to the filer to which data are written.
  */
  void dwgOutFields(OdDbDwgFiler* pFiler, OdDbObjectId objectId) const;

  /** \details
    Reads the .dxf file data of this object. 

    \param pFiler [in]  Filer object from which data are read.
    
    \remarks
    Returns the filer status.
  */
  OdResult dxfInFields(OdDbDxfFiler* pFiler, OdDbObjectId objectId);

  /** \details
    Writes the .dxf file data of this object. 

    \param pFiler [in]  Pointer to the filer to which data are written.
  */
  void dxfOutFields(OdDbDxfFiler* pFiler, OdDbObjectId objectId) const;

  /*!DOM*/
  static OdDbAssocVariableCallback* m_assocVariableCallback;
protected:
  /*!DOM*/
  OdString m_varName;
  /*!DOM*/
  OdString m_expression;
  /*!DOM*/
  OdString m_description;
  /*!DOM*/
  OdString m_evaluatorId;
  /*!DOM*/
  OdBool m_isMergeable;
  /*!DOM*/
  OdBool m_mustMerge;
  /*!DOM*/
  OdString m_mergeableVariableName;
  /*!DOM*/
  OdDbEvalVariantPtr m_value;
  /*!DOM*/
  OdInt16Array  m_arrValueDependenciesIdx;
};

#include "TD_PackPop.h"

#endif
