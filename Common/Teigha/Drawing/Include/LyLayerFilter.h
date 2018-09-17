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



#ifndef _OD_LY_H
#define _OD_LY_H

#include "TD_PackPush.h"

#include "RxObject.h"
#include "OdArray.h"
#include "DbObjectId.h"
#include "IdArrays.h"


class OdLyLayerFilter;
/** \details
  This template class is a specialization of the OdSmartPtr class for OdLyLayerFilter object pointers.
*/
typedef OdSmartPtr<OdLyLayerFilter> OdLyLayerFilterPtr;
/** \details
  This template class is a specialization of the OdArray class for OdLyLayerFilter object pointers.
*/
typedef OdArray<OdLyLayerFilterPtr> OdLyLayerFilterArray;

class OdDbLayerTableRecord;
class OdDbDxfFiler;
class OdLySystemInternals;

/** \details
            
    Corresponding C++ library: TD_Db

    <group OdLy_Classes> 
*/
class OdLyRelExpr
{
public:
    virtual ~OdLyRelExpr() {}
    virtual const OdString& getConstant() const = 0;
    virtual const OdString& getVariable() const = 0;
protected:
    OdLyRelExpr() {}
};

/** \details
            
    Corresponding C++ library: TD_Db

    <group OdLy_Classes> 
*/
class OdLyAndExpr
{
public:
    virtual ~OdLyAndExpr() {}
    virtual const OdArray<OdLyRelExpr*>& getRelExprs() const = 0;
protected:
    OdLyAndExpr() {}
};

/** \details
            
    Corresponding C++ library: TD_Db

    <group OdLy_Classes> 
*/
class OdLyBoolExpr
{
public:
    virtual ~OdLyBoolExpr() {}
    virtual const OdArray<OdLyAndExpr*>& getAndExprs() const = 0;
protected:
    OdLyBoolExpr() {}
};

/** \details
    This class implements LayerFilter objects. 

    \remarks
    This class allows the client to specify, retrieve, and nest and unnest LayerFilters,
    and to determine if a layer is visible with for LayerFilter object.
    
    All Layer Filters implement this interface.
    \sa
    TD_Db
    <group OdLy_Classes> 
*/
class TOOLKIT_EXPORT OdLyLayerFilter : public OdRxObject
{
public:
  ODRX_DECLARE_MEMBERS(OdLyLayerFilter);
  OdLyLayerFilter();
  virtual ~OdLyLayerFilter();
  
  /** \details
    Returns the name of this LayerFilter object.
  */
  virtual const OdString& name() const;
  
  /** \details
    Sets the name of this LayerFilter object.
    \param name [in]  Name.
    
    \returns
    Returns true if and only if successful.
    \remarks
    Some LayerFilter objects do not allow renamimg; e.g., "All", "Xref".

  */
  virtual bool setName(
    const OdString& name);
    
  /** \details
    Returns true if and only if this LayerFilter object can be renamed.

    \remarks
    Some LayerFilter objects do not allow renamimg; e.g., "All", "Xref".
  */
  virtual bool allowRename() const;

  /** \details
    Returns the parent LayerFilter of this LayerFilter object.

    \remarks
    All LayerFilter objects, except the top level, "ALL", have parents.
  */
  virtual OdLyLayerFilter* parent() const;
  
  /** \details
    Returns the nested LayerFilters of this LayerFilter object.
  */
  virtual const OdLyLayerFilterArray& getNestedFilters() const;

  /** \details
    Adds the specified LayerFilter as a nested LayerFilter to this LayerFilter object.
    \param pLayerFilter [in]  Pointer to the LayerFilter to nest.
  */
  virtual void addNested(
    OdLyLayerFilter* pLayerFilter);
    
  /** \details
    Removes the specified LayerFilter as a nested LayerFilter of this LayerFilter object.
    \param pLayerFilter [in]  Pointer to the LayerFilter to remove.
  */
  virtual void removeNested(
    OdLyLayerFilter* pLayerFilter);
  
  /** \details
    Notification function called after a LayerFilter is loaded.
    
    \remarks
    This is an opportunity for a LayerFilter to automatically generate its nested filters.
  */
  virtual OdResult generateNested();
  
  
  /** \details
    Returns true if and only if this LayerFilter was dynamically generated.
    
    \remarks
    True indicates that this LayerFilter should not be persisted.
  */
  virtual bool dynamicallyGenerated() const;
  
  /** \details
    Returns true if and only if this LayerFilter allows nested LayerFilters.
  */
  virtual bool allowNested() const;
  
  /** \details
    Returns true if and only if this LayerFilter can be deleted.
  */
  virtual bool allowDelete() const;
  
  /** \details
    Returns true if and only if this LayerFilter is a proxy LayerFilter.
  */
  virtual bool isProxy() const;
  
  /** \details
    Returns true if and only if this LayerFilter filters for the Object ID of a LayerTableRecord.
    
    \remarks
    These filters are known as LayerGroups.
  */
  virtual bool isIdFilter() const;
  
  /** \details
    Returns true if and only if the specified layer is visible with this LayerFilter object.
    \param layer [in]  Layer.
  */
  virtual bool filter(
    OdDbLayerTableRecord* layer) const;
  
  /** \details
    Returns the filter expression for this LayerFilter object.
    
    BNF for the valid boolean expression:
    <bool_expr>::= <bool_expr> or <and_expr> | <and_expr>
    <and_expr>::=<and_expr> and <rel_expr> | <rel_expr>
    <rel_expr>::=<variable><rel_op><constant> 
    <rel_op>::= == | !=
    <constant>::=A-Z0-9*?~@.,-
    <variable>::=#A-Z0-9

    Supported variables are:

    NAME
    LOCKED
    PLOTTABLE
    LINETYPE
    LINEWEIGHT
    COLOR
    OFF
    FROZEN
    PLOTSTYLENAME
    NEWVPFROZEN
    VP
    VPFROZEN
    VPCOLOR
    VPLINETYPE
    VPLINEWEIGHT
  */
  virtual const OdString& filterExpression() const;

  /** \details
    Returns the filter expression for this LayerFilter object.
  */
  const OdLyBoolExpr* filterExpressionTree() const;
  
  /** \details
    Sets the filter expression for this LayerFilter object.
    \param filterExpression [in]  Filter expression.
  */
  virtual OdResult setFilterExpression(
     const OdString& filterExpression );
  
  
  /** \details
    Returns true if and only if this LayerFilter object and the specified LayerFilter
    object allow the same layers.
    \param pOther [in]  Pointer to the other LayerFilter.
  */
  virtual bool compareTo(
    const OdLyLayerFilter* pOther) const;
  
  /** \details
    Reads the DXF format data of this object from the specified filter.
    \param pFiler [in]  Pointer to the filer from which data are to be read.
  */
  virtual OdResult readFrom(
    OdDbDxfFiler* pFiler);
    
  /** \details
    Writes the DXF format data of this object to the specified filter.
    \param pFiler [in]  Pointer to the filer to which data are to be written.
  */
  virtual void writeTo(
    OdDbDxfFiler* pFiler) const;
  
private:
  void* m_pImpl;
  friend class OdLySystemInternals;
  
protected:
  OdLyLayerFilter( OdLySystemInternals* );
};


/** \details
    This class implements LayerGroup objects. 

    \remarks
    This class allows the client to specify, retrieve, and group and ungroup layers,
    and to determine if a layer is grouped within a LayerGroup object.

    \sa
    TD_Db
    <group OdLy_Classes> 
*/
class TOOLKIT_EXPORT OdLyLayerGroup : public OdLyLayerFilter
{
public:
  ODRX_DECLARE_MEMBERS(OdLyLayerGroup);
  OdLyLayerGroup();
  
  /** \details
    Adds the specified layer to this LayerGroup object.
    \param layerId [in]  Object ID of the OdDbLayerTableRecord.
  */
  virtual void addLayerId(
    const OdDbObjectId& layerId);
    
  /** \details
    Removes the specified layer to this LayerGroup object.
    \param layerId [in]  Object ID of the OdDbLayerTableRecord.
  */
  virtual void removeLayerId (
    const OdDbObjectId& layerId);
  
  /** \details
    Returns the LayerTableRecord Object IDs in this LayerGroup object.
  */
  virtual const OdDbObjectIdArray& layerIds() const;
  
protected:
  OdLyLayerGroup( OdLySystemInternals* );
};

typedef OdSmartPtr<OdLyLayerGroup> OdLyLayerGroupPtr;

/** \details
    This class simplifies accessing LayerFilter objects in the LayerTable extension dictionary.
    \sa
    TD_Db
    <group OdLy_Classes> 
*/
class OdLyLayerFilterManager : public OdRxObject
{
public:
  ODRX_DECLARE_MEMBERS(OdLyLayerFilterManager);
  /** \details
    Returns the root and current filters from the database containing this LayerFilterManager object.
    \param pRoot [out]  Receives a SmartPointer to the root filter.
    \param pCurrent [out]  Receives a SmartPointer to the filter current when setFilters() was last called.  
  */
  virtual OdResult getFilters(
    OdLyLayerFilterPtr& pRoot, 
    OdLyLayerFilterPtr& pCurrent ) = 0;
  
  /** \details
    Writes the filter tree and current filters to the database containing this LayerFilterManager object.
    \param pRoot [in]  Pointer to the root filter.
    \param pCurrent [in]  Pointer to the current filter to be persisted.  
  */
  virtual void setFilters(
    const OdLyLayerFilterPtr pRoot, 
    const OdLyLayerFilterPtr pCurrent ) = 0;
};

/** \details
  This template class is a specialization of the OdSmartPtr class for OdLyLayerFilterManager object pointers.
*/
typedef OdSmartPtr<OdLyLayerFilterManager> OdLyLayerFilterManagerPtr;

/** \details
  Returns a LayerFilterManager object for the specified database.
  \param pDb [in]  Pointer to the database.
*/
TOOLKIT_EXPORT OdLyLayerFilterManagerPtr odlyGetLayerFilterManager(
  OdDbDatabase* pDb );

#include "TD_PackPop.h"

#endif  // _OD_LY_H
