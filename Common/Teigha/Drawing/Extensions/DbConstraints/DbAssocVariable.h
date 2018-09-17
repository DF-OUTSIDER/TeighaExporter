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

#ifndef DbAssocVariable_INCLUDED_
#define DbAssocVariable_INCLUDED_ /*!DOM*/

#include "DbConstraintsExports.h"

#include "DbAssocValueProviderPE.h"
#include "DbAssocAction.h"

#include "OdString.h"
#include "BoolArray.h"
#include "DoubleArray.h"
#include "Int16Array.h"

#include "DbEvalVariant.h"

#include "TD_PackPush.h"


/** \details
  This class represents the variable which stores a scalar value of following types: 
  double, int, or string. The value may be either a constant or be defined by an expression. 
  The expression can refer to other symbols which the evaluate() method of the variable 
  can transform to the evaluated value using the current values of the referenced symbols. 

  Library: Source code provided.
  <group Contraint_Classes> 
*/
class DBCONSTRAINTS_EXPORT OdDbAssocVariable : public OdDbAssocAction
{
public:     
  ODRX_DECLARE_MEMBERS(OdDbAssocVariable);

  /** \details
    Constructor creates an instance of this class.
  */
  OdDbAssocVariable();

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
    Sets the name for this variable using a given string.
  */
  OdResult setName(const OdString newName, bool updateReferencingExpressions);

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
  OdResult setExpression(const OdString newExpression, 
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

  /*!DOM*/
  bool     isMergeable          () const;

  /*!DOM*/
  bool     mustMerge            () const;

  /*!DOM*/
  OdString mergeableVariableName() const;

  /*!DOM*/
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
};

/** \details
  The typified smart pointer. This template class is specialization of the OdSmartPtr class 
  for the OdDbAssocVariable class.

  \sa
  <link smart_pointers.html, Working with Smart Pointers>
*/
typedef OdSmartPtr<OdDbAssocVariable> OdDbAssocVariablePtr;


/** \details
  This class represents the associated callback. The client code can register callback(s) derived 
  from associated callback that are then called when some events are triggered for variables.

  Library: Source code provided.
  <group Contraint_Classes> 
*/
class OdDbAssocVariableCallback
{
public:
  /** \details
    Constructor creates an instance of this class.
  */
  OdDbAssocVariableCallback() {}

  /** \details
    Destructor destroys an instance of this class.
  */
  virtual ~OdDbAssocVariableCallback() {}

  /** \details
    Checks whether the client code permits or forbids the erasing of callbacks.
  */
  virtual bool canBeErased(const OdDbAssocVariable* pVariable) = 0;

  /** \details
    This method is called from the validateNameAndExpression() method of the associated variable 
    object. This method assumes the client code perform additional testing to decide if a given 
    name or expression are allowed to be used for the given variable.
  */
  virtual OdResult validateNameAndExpression(const OdDbAssocVariable* pVariable,
                                             const OdString nameToValidate, 
                                             const OdString expressionToValidate,
                                             OdString errorMessage) = 0;
};


/** \details
  This class represents the implementation of the associated value provider protocol extension 
  to obtain the value of an associated variable.

  Library: Source code provided.
  <group Contraint_Classes> 
*/
class OdDbAssocVariableValueProviderPE : public OdDbAssocValueProviderPE
{
public:
  ODRX_DECLARE_MEMBERS(OdDbAssocVariableValueProviderPE);
#if 0

  /** \details
    Gets the value of the given variable using its name and pointer to the object.
  */
  virtual OdResult getValue(const OdDbObject* pObject, 
                           const OdString   valueName, 
                           OdDbEvalVariant&  value);
#endif
};  

/** \details
  The typified smart pointer. This template class is specialization of the OdSmartPtr class 
  for the OdDbAssocVariableValueProviderPE class.

  \sa
  <link smart_pointers.html, Working with Smart Pointers>
*/
typedef OdSmartPtr<OdDbAssocVariableValueProviderPE> OdDbAssocVariableValueProviderPEPtr;

#include "TD_PackPop.h"

#endif
