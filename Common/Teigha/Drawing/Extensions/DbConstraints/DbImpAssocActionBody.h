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
#ifndef DbImpAssocActionBody_INCLUDED_
#define DbImpAssocActionBody_INCLUDED_ /*!DOM*/

#include "DbConstraintsExports.h"
#include "DbAssocGlobal.h"
#include "DbAssocArrayParameters.h"
#include "DbObject.h"
#include "SharedPtr.h"
#include "DbEvalVariant.h"
#include "DbAssocAction.h"
#include "TD_PackPush.h"
#include "DbViewport.h"

// attention : there is no accordant OdDbValueParam

/** \details
  This class represents the custom implementation of the value parameter.

  Library: Source code provided.
  <group Contraint_Classes> 
*/
class DBCONSTRAINTS_EXPORT OdDbImpValueParam 
{
public:
  /** \details
    Constructor creates an instance of this class.
  */
  OdDbImpValueParam();

  /** \details
    Destructor destroys an instance of this class.
  */
  virtual ~OdDbImpValueParam();

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

  const OdString& name() const {return m_sName;}
  void setName(const OdString& sName) {m_sName = sName;}

  OdValue::UnitType unitType() const {return m_UnitType;}
  void setUnitType(OdValue::UnitType val) {m_UnitType = val;}

  OdDbEvalVariantPtr value() const {return m_value;}
  void setValue(const OdDbEvalVariant& Val);

  OdDbObjectId controlledObjectDepId() const {return m_ControlledObjectDepId;}
  void setControlledObjectDepId(OdDbObjectId id) {m_ControlledObjectDepId = id;}

  OdDbObjectIdArray inputVariablesIds() const {return m_InputVariablesIds;}
  void setInputVariablesIds(OdDbObjectIdArray vars) {m_InputVariablesIds = vars;}

  virtual OdStringArray compareWith(const OdDbImpValueParam *paramIDToCompare, OdDb::DwgVersion filerVersion = OdDb::kDHL_CURRENT) const;
protected:
  /*!DOM*/
  OdString m_sName;
  /*!DOM*/
  OdValue::UnitType m_UnitType;
  /*!DOM*/
  OdDbEvalVariantPtr m_value;
  /*!DOM*/
  OdDbObjectId      m_ControlledObjectDepId;
  OdDbObjectIdArray m_InputVariablesIds;
};

/** \details
  The typified smart pointer. This template class is specialization of the OdSmartPtr class 
  for the OdDbImpValueParam class.

  \sa
  <link smart_pointers.html, Working with Smart Pointers>
*/
typedef OdSharedPtr<OdDbImpValueParam> OdDbImpValueParamPtr;


/** \details
  This class represents the custom implementation of the action body.

  Library: Source code provided.
  <group Contraint_Classes> 
*/
class DBCONSTRAINTS_EXPORT OdDbImpAssocActionBody
{
public: 
  /** \details
    Constructor creates an instance of this class.
  */
  OdDbImpAssocActionBody();

  /** \details
    Destructor destroys an instance of this class.
  */
  virtual ~OdDbImpAssocActionBody();

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
  Creates action and action body, add it to db.
  */
  static OdResult createActionAndActionBodyAndPostToDatabase( OdRxClass* pActionBodyClass, const OdDbObjectId& objectId, 
    OdDbObjectId& createdActionId, OdDbObjectId& createdActionBodyId);

  virtual OdStringArray compareWith(OdDbImpAssocActionBody *bodyIDToCompare, OdDb::DwgVersion filerVersion = OdDb::kDHL_CURRENT) const;

  OdDbObjectIdArray m_arrParams;
  OdArray<OdDbImpValueParamPtr> m_arrValueParams;
protected:
  OdUInt32 m_dwgVersion;

  OdDbEntityPtr checkAccompanying(OdDbObjectId parentActionId, OdDbAssocActionPtr pAction);
  /** \details
  Gets the object ID of the associated array which this action controlls.
  */
  virtual OdDbObjectId getArrayEntity(OdDbObjectId parentActionId) const;
  OdDbObjectId getIdDepArrayEntity(OdDbObjectIdArray depIDs, bool isWriteDependencyOnly = false) const;
  //static void transformPositionByViewPort(OdDbViewportPtr& pViewport, OdGePoint3d& blPosition);

  friend class OdDbImpAssocAction;
  friend class OdDbImpSmartCenterActionBody;
  
  //OdDbObjectIdArray m_arrParams;
};


/** \details
  This class represents the custom implementation of the associated parameter of the action body.

  Library: Source code provided.
  <group Contraint_Classes> 
*/
class DBCONSTRAINTS_EXPORT OdDbImpAssocParamBasedActionBody : public OdDbImpAssocActionBody
{
public: 
  /** \details
    Constructor creates an instance of this class.
  */
  OdDbImpAssocParamBasedActionBody();
  OdDbImpAssocParamBasedActionBody(bool notparent);
  /** \details
    Destructor destroys an instance of this class.
  */
  virtual ~OdDbImpAssocParamBasedActionBody();

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
  OdDbObjectId& assocDependencyID() { return m_assocDependencyID; }
  virtual OdStringArray compareWith(OdDbImpAssocActionBody *bodyIDToCompare, OdDb::DwgVersion filerVersion = OdDb::kDHL_CURRENT) const;

protected:
  /*!DOM*/
  friend class OdDbImpAssocAction;
  /*!DOM*/
  //OdDbObjectIdArray m_arrParams;
  /*!DOM*/
  //OdArray<OdDbImpValueParamPtr> m_arrValueParams;
private:
  OdUInt32 m_undefined1;
  OdUInt32 m_undefined2;
  OdUInt32 m_undefined3;
  OdUInt32 m_undefined4;
  OdDbObjectId m_assocDependencyID;
  bool m_notParent;
};

/** \details
  This class represents the custom implementation of the array of the associated parameter of the action body.

  Library: Source code provided.
  <group Contraint_Classes> 
*/
class DBCONSTRAINTS_EXPORT OdDbImpAssocArrayActionBody : public OdDbImpAssocParamBasedActionBody
{
public: 
  /** \details
    Constructor creates an instance of this class.
  */
  OdDbImpAssocArrayActionBody();

  /** \details
    Destructor destroys an instance of this class.
  */
  virtual ~OdDbImpAssocArrayActionBody();

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

  void composeForLoad( OdDbObject *pObject, OdDb::SaveType format, OdDb::DwgVersion version, OdDbAuditInfo* pAuditInfo);

  /** \details
    Provides the standard audit protocol.
  */
  virtual void audit( OdDbObject* pObj, OdDbAuditInfo* pAuditInfo );

  /** \details
    This is the notification call of the corresponding evaluate() method 
    of the parent class that owns this action body.
  */
  virtual void evaluateOverride( OdDbObjectId parentActionId );

  /** \details
    Returns a pointer to the array of parameters for writing.
  */
  const OdDbAssocArrayParametersPtr parameters() const;
  
  /** \details
    Returns a pointer to the array of parameters for writing.
  */
  OdDbAssocArrayParametersPtr& parameters();
  /** \details
    Switches the erase/unerase status for an item at given index.
  */
  OdResult deleteItem(const OdDbItemLocator& index, bool bErase);  
  /** \details
    Gets the item at the given subent path.
  */
  const OdDbAssocArrayItem* getItemAt( const OdDbItemLocator& index,
                                       OdDbFullSubentPath& path,
                                       OdDbObjectId arrayId ) const;
  /** \details
    Performs a relative transformation to the item at the given index.
  */
  OdResult transformItemBy(const OdDbItemLocator& index, const OdGeMatrix3d& xform);
  /** \details
    Gets the indices of all items which are owned by this array.
  */
  void getItems(OdArray<OdDbItemLocator>& indices, bool skipErased) const;
  /** \details
    Gets the item at the given subent path.
  */
  const OdDbAssocArrayItem* getItemAt(const OdDbFullSubentPath& path) const;
  /** \details
    Creates associative array: action, action body, dependencies etc.
  */
  static OdResult createInstance( const OdDbObjectIdArray& sourceEntites, 
                                  OdDbVertexRef& basePoint, 
                                  const OdDbAssocArrayParametersPtr &pParameters, 
                                  OdDbObjectId& arrayId, 
                                  OdDbObjectId& actionBodyId);
  /** \details
    Get object ID of array action body manage this item.
  */
  static OdDbObjectId getControllingActionBody( const OdDbEntity* pEntity, 
                                                const OdDbItemLocator* pItemIndex = NULL );
  /** \details
    Checks that the entity is an associative array.
  */
  static bool isAssociativeArray( const OdDbEntity* pEntity );
  /** \details
    Removes item transformation.
  */
  static OdResult resetArrayItems( OdDbObjectId arrayEntityId,
                                   const OdArray<OdDbItemLocator>& indices, 
                                   bool resetAll = false);
  /** \details
    Removes associativity. Each array item is extracted as individual items. 
  */
  static OdResult explode( OdDbEntity* pEntity, OdDbObjectIdArray& newIds );

  /** \details
    Applies the transformation matrix to the array.
  */
  OdResult transformBy(const OdGeMatrix3d& xform);
  /** \details
    Returns the transformation matrix.
  */
  OdGeMatrix3d getTransform() const;
  /** \details
    Returns an object ID of the owning block table record for source items.
  */
  OdDbObjectId getArraySourceBTR( OdDbObjectId parentActionId ) const;

  /** \details
    Gets the object ID of the associated array which this action controlls.
  */
  virtual OdDbObjectId getArrayEntity( OdDbObjectId parentActionId ) const;

  /** \details
    Checks whether an item of an associated array can be controlled either by created action 
    or by another action which modifies this item.
  */
  virtual bool controlsItem(const OdDbItemLocator& index) const;
  /** \details
    Returns an array of object IDs of source entities for this action.
  */
  OdDbObjectIdArray getSourceEntities( OdDbObjectId parentActionId ) const;
  /** \details
    Adds an entity specified by a given object ID to the list of source items and 
    updates each item in the array which this action controlls.
  */
  OdResult addSourceEntity( OdDbObjectId entity, const OdGePoint3d& basePoint, OdDbObjectId parentActionId );
  /** \details
    Removes an entity specified by a given object ID from the list of source items and 
    updates each item in the array which this action controlls.
  */
  OdResult removeSourceEntity( OdDbObjectId entity, OdDbObjectId parentActionId );
  /** \details
    Sets the base point for source objects.
  */
  OdResult setSourceBasePoint( const OdDbVertexRef& basePoint, const OdDbObjectId &parentActionId );

  /** \details
    Gets the base point of source objects.
  */
  OdResult getSourceBasePoint( OdDbVertexRef& vertexRef, OdGePoint3d& position ) const;

  static OdResult getArrayItemLocators( const OdDbFullSubentPathArray& subents, OdArray<OdDbItemLocator>& indices );
  


  OdResult addMoreObjectsToDeepCloneOverride( const OdDbAssocArrayActionBody *pBody,
                                              OdDbIdMapping& idMap, 
                                              OdDbObjectIdArray& additionalObjectsToClone) const;

  OdResult postProcessAfterDeepCloneOverride( const OdDbAssocArrayActionBody *pBody, OdDbIdMapping& idMap );

  OdDbBlockTableRecordPtr getSourceBTR( const OdDbObjectId &parentActionId ) const;
  OdDbBlockTableRecordPtr getSourceBTR( const OdDbObjectId &parentActionId );

protected:
  /*!DOM*/
  OdDbAssocArrayParametersPtr m_pParams; 
  /* Transformation matrix of the array.*/
  OdGeMatrix3d m_matrix;

  OdDbObjectId getIdDepArrayEntity(OdDbObjectIdArray depIDs) const;

  inline OdResult getItem( OdDbAssocArrayItemPtr &pItem, const OdDbItemLocator& index ) const;
};

class DBCONSTRAINTS_EXPORT OdDbImpAssocArrayModifyActionBody : public OdDbImpAssocArrayActionBody
{
public:
  
  /** \details
    Constructor creates an instance of this class.
  */
  OdDbImpAssocArrayModifyActionBody();

  /** \details
    Destructor destroys an instance of this class.
  */
  virtual ~OdDbImpAssocArrayModifyActionBody();

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

  void composeForLoad( OdDbObject *pObject, OdDb::SaveType format, OdDb::DwgVersion version, OdDbAuditInfo* pAuditInfo);

  /** \details
    Get indices of overridden items.
  */
  OdResult getOverridenItems( OdArray<OdDbItemLocator> &indices ) const;
  
  /** \details
    Set indices of overridden items.
  */
  OdResult setOverridenItems( const OdArray<OdDbItemLocator> &indices);

  /** \details
    Checks whether an item of an associated array can be controlled either by created action 
    or by another action which modifies this item.
  */
  virtual bool controlsItem(const OdDbItemLocator& index) const;
  
  /** \details
    Modify array: add new action, action body, dependencies etc.
  */
  static OdResult createInstance( OdDbObjectId arrayId, 
                                  const OdArray<OdDbItemLocator> &indices,
                                  const OdDbObjectIdArray &subEnt,
                                  OdDbVertexRef &basePoint,
                                  OdDbObjectId& actionBodyId);

  /** \details
    This is the notification call of the corresponding evaluate() method 
    of the parent class that owns this action body.
  */
  virtual void evaluateOverride( OdDbObjectId parentActionId );

  /** \details
    Gets the object ID of the associated array which this action controlls.
  */
  virtual OdDbObjectId getArrayEntity( OdDbObjectId parentActionId ) const;

private:

  OdArray<OdDbItemLocator> m_indices;

  void restoreSrcPosition( OdDbAssocAction* pParentAction );
 
};


#include "TD_PackPop.h"

#endif
