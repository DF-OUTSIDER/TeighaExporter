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

#ifndef DbAssocActionParam_INCLUDED_
#define DbAssocActionParam_INCLUDED_ /*!DOM*/

#include "DbAssocAction.h"
#include "DbCompoundObjectId.h"
#include "DbGeomRef.h"

#include "TD_PackPush.h"

/** \details
  This class implements the action parameter.

  Library: Source code provided.
  <group Contraint_Classes> 
*/
class DBCONSTRAINTS_EXPORT OdDbAssocActionParam : public OdDbObject
{
public:     
  ODRX_DECLARE_MEMBERS(OdDbAssocActionParam);

  /** \details
    Constructor creates an instance of this class.
  */
  OdDbAssocActionParam(bool createImpObj = true);

  /** \details
    Destructor destroys an instance of this class.
  */
  ~OdDbAssocActionParam();

  /** \details
    Returns the parameter name.
  */
  const OdString& name() const;

  /** \details
    Sets the parameter name.
  */
  virtual OdResult setName(const OdString& newName);

  /** \details
    Makes empty the parameter.
  */
  virtual OdResult makeParamEmpty(bool alsoEraseOwnedObjects);

  /** \details
    Makes constant the parameter.
  */
  virtual OdResult makeParamConstant();

  /** \details
    Applies the transformation matrix to the constraint geometry.
  */
  virtual OdResult transformConstantGeometry(const OdGeMatrix3d& transform);

  /** \details
    Returns an object ID of the parent action. 
  */
  OdDbObjectId parentAction() const;

  /** \details
    Detaches the dependencies of this action. 
  */
  virtual OdResult detachDependencies();

  /** \details
    Gets the dependencies of this action. 
  */
  virtual OdResult getDependencies(bool readDependenciesWanted, 
                                   bool writeDependenciesWanted,
                                   OdDbObjectIdArray& dependencyIds) const;

  /** \details
    Gets an array of IDs of the compound object for this action. 
  */
  OdResult getCompoundObjectIds(bool readDependenciesWanted, 
                                bool writeDependenciesWanted,
                                OdArray<OdDbCompoundObjectId>& compoundObjectIds) const;

  /** \details
    Returns the current status of this action. 
  */
  OdDbAssocStatus status(bool alsoCheckDependencies) const;

  /** \details
    Sets the status for this action. 
  */
  OdResult setStatus(OdDbAssocStatus newStatus, 
                     bool notifyParentAction = true, 
                     bool setInOwnedParams = false);

  /** \details
    Audits the associative data sets. 
  */
  virtual void auditAssociativeData(OdDbAssocStatus& parentActionHandling);

  //virtual void collectPersSubentNamingDataOverride(OdDbPersStepIdArray& stepIds, 
  //                                                 OdDbPersSubentIdArray& persSubentIds) const;
  //virtual void clonePersSubentNamingDataOverride(OdDbAssocPersSubentManagerCloner* pCloner);

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
  Gets the compounded object.
  */
  OdResult getObject(OdDbCompoundObjectId* object) const;

  /** \details
  Sets the compounded object.
  */
  OdResult setObject( //OdDbObjectId actionId,
    const OdDbCompoundObjectId* object,
    bool isReadDependency = true,
    bool isWriteDependency = false);

  OdResult setObjectOnly(const OdDbCompoundObjectId* object,
    bool isReadDependency, // = true
    bool isWriteDependency // = false
    );

  virtual OdStringArray compareWith(const OdDbObjectId paramIDToCompare, OdDb::DwgVersion filerVersion = OdDb::kDHL_CURRENT) const;
protected:
  /*!DOM*/
  class OdDbImpAssocActionParam* m_pImpObj;
};

/** \details
  The typified smart pointer. This template class is specialization of the OdSmartPtr class 
  for the OdDbAssocActionParam class.

  \sa
  <link smart_pointers.html, Working with Smart Pointers>
*/
typedef OdSmartPtr<OdDbAssocActionParam> OdDbAssocActionParamPtr;


/** \details
  This class implements the body action parameter.

  Library: Source code provided.
  <group Contraint_Classes> 
*/
class DBCONSTRAINTS_EXPORT OdDbAssocAsmBodyActionParam : public OdDbAssocActionParam 
{
public:
  ODRX_DECLARE_MEMBERS(OdDbAssocAsmBodyActionParam);

  /** \details
    Constructor creates an instance of this class.
  */
  OdDbAssocAsmBodyActionParam(bool createImpObj = true);

  /** \details
    Sets the body for this action.
  */
  virtual OdResult setBody(const OdDbEntity* pAsmEntity, bool isReadDependency = true, bool isWriteDependency = false, int dependencyOrder = 0);
  
  /** \details
    Sets the body for this action.
  */
  virtual OdResult setBody(OdDbObjectId asmEntityId, bool isReadDependency = true, bool isWriteDependency = false, int dependencyOrder = 0);
  
  /** \details
    Sets the body for this action.
  */
  virtual OdResult setBody(void* pAsmBody, bool makeCopy); 

  /** \details
    Returns the body or its duplicate from this action.
  */
  virtual void* body(bool makeCopy) const;

  /** \details
    Gets an ID of the compounded object this action.
  */
  OdResult getDependentOnCompoundObject(OdDbCompoundObjectId& compoundId) const;
};

/** \details
  The typified smart pointer. This template class is specialization of the OdSmartPtr class 
  for the OdDbAssocAsmBodyActionParam class.

  \sa
  <link smart_pointers.html, Working with Smart Pointers>
*/
typedef OdSmartPtr<OdDbAssocAsmBodyActionParam> OdDbAssocAsmBodyActionParamPtr;


/** \details
  This class implements the compounded action parameter.

  Library: Source code provided.
  <group Contraint_Classes> 
*/
class DBCONSTRAINTS_EXPORT OdDbAssocCompoundActionParam : public OdDbAssocActionParam
{
public:
  ODRX_DECLARE_MEMBERS(OdDbAssocCompoundActionParam);

  /** \details
    Constructor creates an instance of this class.
  */
  OdDbAssocCompoundActionParam(bool createImpObj = true);

  /** \details
    Removes all parameters and optional erases them.
  */
  virtual OdResult removeAllParams(bool alsoEraseThem);

  /** \details
    Returns the number of parameters.
  */
  int paramCount() const;

  /** \details
    Returns the object ID of owned parameters.
  */
  const OdDbObjectIdArray& ownedParams() const;

  /** \details
    Adds a parameter using an object ID and index.
  */
  OdResult addParam(const OdDbObjectId& paramId, int& paramIndex);

  /** \details
    Removes the parameter using its object ID and optional erases it.
  */
  OdResult removeParam(const OdDbObjectId& paramId, bool alsoEraseIt); 

  /** \details
    Returns an array of object ID of parameters using the parameter name.
  */
  OdDbObjectIdArray paramsAtName(const OdString& paramName) const;

  /** \details
    Returns an object ID of the parameter using its name and index.
  */
  OdDbObjectId paramAtName(const OdString& paramName, int index = 0) const;

  /** \details
    Returns an object ID of the parameter using its index.
  */
  OdDbObjectId paramAtIndex(int paramIndex) const;

  /** \details
  Creates and adds a new parameter.
  */
  OdResult addChildParam(const OdDbObjectId actionId, const OdString& paramName, OdRxClass* pParamClass, const OdDbObjectId anchorId, OdDbObjectId& paramId, int& paramIndex);
};

/** \details
  The typified smart pointer. This template class is specialization of the OdSmartPtr class 
  for the OdDbAssocCompoundActionParam class.

  \sa
  <link smart_pointers.html, Working with Smart Pointers>
*/
typedef OdSmartPtr<OdDbAssocCompoundActionParam> OdDbAssocCompoundActionParamPtr;


/** \details
  This class implements the edge action parameter.

  Library: Source code provided.
  <group Contraint_Classes> 
*/
class DBCONSTRAINTS_EXPORT OdDbAssocEdgeActionParam : public OdDbAssocActionParam
{
public:
  ODRX_DECLARE_MEMBERS(OdDbAssocEdgeActionParam);

  /** \details
    Constructor creates an instance of this class.
  */
  OdDbAssocEdgeActionParam(bool createImpObj = true);

  /** \details
    Sets the edge reference.
  */
  virtual OdResult setEdgeRef(const OdDbEdgeRef& edgeRef, bool isReadDependency = true, bool isWriteDependency = false, int dependencyOrder = 0);

  /** \details
    Gets the edge reference.
  */
  virtual OdResult getEdgeRef(OdArray<OdDbEdgeRef>& edgeRefs) const;

  /** \details
    Sets the edge subentity geometry.
  */
  virtual OdResult setEdgeSubentityGeometry(const OdGeCurve3d* pNewEdgeCurve);

  /** \details
    Gets an ID of the compounded object this action.
  */
  OdResult getDependentOnCompoundObject(OdDbCompoundObjectId& compoundId) const;
};

/** \details
  The typified smart pointer. This template class is specialization of the OdSmartPtr class 
  for the OdDbAssocEdgeActionParam class.

  \sa
  <link smart_pointers.html, Working with Smart Pointers>
*/
typedef OdSmartPtr<OdDbAssocEdgeActionParam> OdDbAssocEdgeActionParamPtr;


/** \details
  This class implements the face action parameter.

  Library: Source code provided.
  <group Contraint_Classes> 
*/
class DBCONSTRAINTS_EXPORT OdDbAssocFaceActionParam : public OdDbAssocActionParam
{
public:
  ODRX_DECLARE_MEMBERS(OdDbAssocFaceActionParam);

  /** \details
    Constructor creates an instance of this class.
  */
  OdDbAssocFaceActionParam(bool createImpObj = true);

  /** \details
    Sets the face reference.
  */
  virtual OdResult setFaceRef(const OdDbFaceRef& faceRef, bool isReadDependency = true, bool isWriteDependency = false, int dependencyOrder = 0);

  /** \details
    Gets the face reference.
  */
  virtual OdResult getFaceRef(OdArray<OdDbFaceRef>& faceRefs) const;

  /** \details
    Sets the face subentity geometry.
  */
  virtual OdResult setFaceSubentityGeometry(const OdGeSurface* pNewFaceSurface);

  /** \details
    Gets an ID of the compounded object this action.
  */
  OdResult getDependentOnCompoundObject(OdDbCompoundObjectId& compoundId) const;
};

/** \details
  The typified smart pointer. This template class is specialization of the OdSmartPtr class 
  for the OdDbAssocFaceActionParam class.

  \sa
  <link smart_pointers.html, Working with Smart Pointers>
*/
typedef OdSmartPtr<OdDbAssocFaceActionParam> OdDbAssocFaceActionParamPtr;


/** \details
This class implements the face action parameter.

Library: Source code provided.
<group Contraint_Classes>
*/
class DBCONSTRAINTS_EXPORT OdDbAssocTrimmingBodyActionParam  : public OdDbAssocFaceActionParam
{
public:
	ODRX_DECLARE_MEMBERS(OdDbAssocTrimmingBodyActionParam);

	/** \details
	Constructor creates an instance of this class.
	*/
	OdDbAssocTrimmingBodyActionParam(bool createImpObj = true);

};

/** \details
The typified smart pointer. This template class is specialization of the OdSmartPtr class
for the OdDbAssocFaceActionParam class.

\sa
<link smart_pointers.html, Working with Smart Pointers>
*/
typedef OdSmartPtr<OdDbAssocTrimmingBodyActionParam> OdDbAssocTrimmingBodyActionParamPtr;
/** \details
  This class implements the object action parameter.

  Library: Source code provided.
  <group Contraint_Classes> 
*/
class DBCONSTRAINTS_EXPORT OdDbAssocObjectActionParam : public OdDbAssocActionParam
{
public:
  ODRX_DECLARE_MEMBERS(OdDbAssocObjectActionParam);

  /** \details
    Constructor creates an instance of this class.
  */
  OdDbAssocObjectActionParam(bool createImpObj = true);

  /** \details
    Gets the compounded object.
  */
  OdResult getObject(OdDbCompoundObjectId* object) const;
};

/** \details
  The typified smart pointer. This template class is specialization of the OdSmartPtr class 
  for the OdDbAssocObjectActionParam class.

  \sa
  <link smart_pointers.html, Working with Smart Pointers>
*/
typedef OdSmartPtr<OdDbAssocObjectActionParam> OdDbAssocObjectActionParamPtr;


/** \details
  This class implements the vertex action parameter.

  Library: Source code provided.
  <group Contraint_Classes> 
*/
class DBCONSTRAINTS_EXPORT OdDbAssocVertexActionParam : public OdDbAssocActionParam
{
public:
  ODRX_DECLARE_MEMBERS(OdDbAssocVertexActionParam);

  /** \details
    Constructor creates an instance of this class.
  */
  OdDbAssocVertexActionParam(bool createImpObj = true);

  /** \details
    Sets the vertex reference.
  */
  virtual OdResult setVertexRef(const OdDbVertexRef& vertexRef, bool isReadDependency = true, bool isWriteDependency = false, int dependencyOrder = 0);

  /** \details
    Gets the vertex reference.
  */
  OdResult getVertexRef(OdDbVertexRef& vertexRef) const;

  /** \details
    Gets an array of vertex references.
  */
  virtual OdResult getVertexRef(OdArray<OdDbVertexRef>& vertexRefs) const;

  /** \details
    Sets the vertex subentity geometry.
  */
  virtual OdResult setVertexSubentityGeometry(const OdGePoint3d& newPosition);

  /** \details
    Gets an ID of the compounded object this action.
  */
  OdResult getDependentOnCompoundObject(OdDbCompoundObjectId& compoundId) const;
};

/** \details
  The typified smart pointer. This template class is specialization of the OdSmartPtr class 
  for the OdDbAssocVertexActionParam class.

  \sa
  <link smart_pointers.html, Working with Smart Pointers>
*/
typedef OdSmartPtr<OdDbAssocVertexActionParam> OdDbAssocVertexActionParamPtr;

/////////////////////////////////////////////////////////////////////////////////////////////////////

/** \details
<group Contraint_Classes>
*/
class DBCONSTRAINTS_EXPORT OdDbAssocPointRefActionParam : public OdDbAssocCompoundActionParam
{
public:
  ODRX_DECLARE_MEMBERS(OdDbAssocPointRefActionParam);

  /** \details
  Constructor creates an instance of this class.
  */
  OdDbAssocPointRefActionParam(bool createImpObj = true);
};

typedef OdSmartPtr<OdDbAssocPointRefActionParam> OdDbAssocPointRefActionParamPtr;

/** \details
<group Contraint_Classes>
*/
class DBCONSTRAINTS_EXPORT OdDbAssocOsnapPointRefActionParam : public OdDbAssocPointRefActionParam
{
public:
  ODRX_DECLARE_MEMBERS(OdDbAssocOsnapPointRefActionParam);

  /** \details
  Constructor creates an instance of this class.
  */
  OdDbAssocOsnapPointRefActionParam(bool createImpObj = true);

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
  Gets the compounded object.
  */

  void setOsnapMode(OdDb::OsnapMode mode);
  void setParam(double param);
};

typedef OdSmartPtr<OdDbAssocOsnapPointRefActionParam> OdDbAssocOsnapPointRefActionParamPtr;
/////////////////////////////////////////////////////////////////////////////////////////////////////

/** \details
<group Contraint_Classes>
*/
class DBCONSTRAINTS_EXPORT OdDbAssocPathActionParam : public OdDbAssocCompoundActionParam
{
public:
	ODRX_DECLARE_MEMBERS(OdDbAssocPathActionParam);

	/** \details
	Constructor creates an instance of this class.
	*/
	OdDbAssocPathActionParam(bool createImpObj = true);
};

typedef OdSmartPtr<OdDbAssocPathActionParam> OdDbAssocPathActionParamPtr;
#include "TD_PackPop.h"

#endif // DbAssocActionParam_INCLUDED_
