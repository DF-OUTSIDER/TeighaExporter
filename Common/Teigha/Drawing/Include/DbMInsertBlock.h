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




#ifndef _OD_DB_MINSERT_BLOCK_
#define _OD_DB_MINSERT_BLOCK_

#include "TD_PackPush.h"

#include "DbBlockReference.h"
#include "DwgDeclareMembers.h"

class OdDbMInsertBlockImpl;

/** \details
    This class represents arrayed instances of block references (MInserts) in an OdDbDatabase instance.

    \sa
    TD_Db

    \remarks
    Creating an OdDbMInsertBlock instance with exactly one row and column creates an OdDbBlockReference instance. 
    <group OdDb_Classes> 
*/
class DBENT_EXPORT OdDbMInsertBlock : public OdDbBlockReference
{
public:
  DWGMAP_DECLARE_MEMBERS(OdDbMInsertBlock);

  OdDbMInsertBlock();
  
  /** \details
    Returns the number of columns for this MInsert (DXF 70).
  */
  OdUInt16 columns() const;

  /** \details
    Sets the number of columns for this MInsert (DXF 70).
    \param numColumns [in]  Number of columns.
  */
  void setColumns(OdUInt16 numColumns);
  
  /** \details
    Returns the number of rows for this MInsert (DXF 71).
  */
  OdUInt16 rows() const;

  /** \details
    Sets the number of rows for this MInsert (DXF 71).
    \param numRows [in]  Number of rows.
  */
  void  setRows(OdUInt16 numRows);
  
  /** \details
    Returns the column spacing for this MInsert (DXF 44).
  */
  double columnSpacing() const;

  /** \details
    Sets the column spacing for this MInsert (DXF 44).
    \param colSpacing [in]  Column spacing.
  */
  void  setColumnSpacing(double colSpacing);
  
  /** \details
    Returns the row spacing for this MInsert (DXF 45).
  */
  double rowSpacing() const;

  /** \details
    Sets the row spacing for this MInsert (DXF 45).
    \param rowSpacing [in]  Row spacing.
  */
  void  setRowSpacing(
    double rowSpacing);

  /** \details
    Reads the .dwg file data of this object.
    
    \param pFiler [in]  Filer object from which data are read.
    
    \remarks 
    Returns the filer status. 
    This function is called by dwgIn() to allow the object to read its data. 
    When overriding this function: 
    1) Call assertWriteEnabled(). 
    2) Call the parent class's dwgInFields(pFiler). 
    3) If it returns eOK, continue; otherwise return whatever the parent's dwgInFields(pFiler) returned. 
    4) Call the OdDbDwgFiler(pFiler) methods to read each of the object's data items in the order 
    they were written. 
    5) Return pFiler->filerStatus().
  */   
  virtual OdResult dwgInFields(
    OdDbDwgFiler* pFiler);

  /** \details
    Writes the .dwg file data of this object.
    
    \param pFiler [in]  Pointer to the filer to which data are written.
    
    \remarks 
    This function is called by dwgIn() to allow the object to write its data. 
    When overriding this function: 
    1) Call assertReadEnabled(). 2) Call the parent class's dwgOutFields(pFiler). 
    3) Call the OdDbDwgFiler (pFiler) methods to write each of the object's data items in the 
    order they were written.
  */    
  virtual void dwgOutFields(
    OdDbDwgFiler* pFiler) const;

  /** \details
    Reads the DXF data of this object. 
    
    \param pFiler [in]  Pointer to the filer from which data are read. 
    
    \remarks 
    Returns the filer status. 
    This function is called by dxfIn() to allow the object to read its data. 
    When overriding this function: 
    1) Call assertWriteEnabled(). 2) Call the parent class's dwgInFields(pFiler).
    3) If it returns eOK, continue; otherwise return whatever the parent's dxfInFields(pFiler) returned. 
    4) Call the OdDbDxfFiler(pFiler) methods to read each of the object's data items in the order they were written. 
    5) Return pFiler->filerStatus().
  */    
  virtual OdResult dxfInFields(
    OdDbDxfFiler* pFiler);

  /** \details
    Reads the DXF data of this object. 
    
    \param pFiler [in]  Pointer to the filer from which data are read. 
    
    \remarks 
    This function is called by dxfOut() to allow the object to write its data. 
    When overriding this function: 
    1) Call assertReadEnabled(). 2) Call the parent class's dxfOutFields (pFiler). 
    4) Use pFiler to call the OdDbDxfFiler methods to write each of the object's data items 
    in the order they were written.
  */
  virtual void dxfOutFields(
    OdDbDxfFiler* pFiler) const;

  /** \details
    Overridables
  */    
  virtual OdResult subGetClassID(
    void* pClsid) const;

  /** \details
    Applies the specified 3D transformation matrix to this entity.
    
    \param xfm [in] 3D transformation matrix.
    
    \returns
    Returns eOk if successful, or an appropriate error code if not.
  */
  virtual OdResult subTransformBy(
    const OdGeMatrix3d& xfm);

  /** \details
    Explodes this entity into a set of simpler entities, and adds them to the specified block 
    table record.

    \param pTargetBlkRec [out]  Pointer to the Block Table Record.
    \param ids [in/out]  Receives an array of Object IDs of the new Block References.

    \remarks
    Entities resulting from the explosion are appended to the specified array.
    The newly created entities are database resident or not depending on the block table 
    record they are appended to. If block table record is database resident the entities 
    are database resident as well. If block table record is not database resident newly 
    created entities are not database resident.
    The default implementation of this function returns eNotApplicable. This function can 
    be overridden in custom classes.
  */  
  OdResult explodeToBlock(
    OdDbBlockTableRecord *pTargetBlkRec, 
    OdDbObjectIdArray *pResIdArray);

  /** \details
    This is subGetGeomExtents, a member of class OdDbBlockReference.
    
    \param extents [out] An output extents.
    
    \returns
    Returns eOk if successful, or an appropriate error code if not.
  */ 
  OdResult subGetGeomExtents(
    OdGeExtents3d& extents) const;

  /** \details
    Returns true if the entity has no view-dependent graphics. In another case
    returns false.
    
    \param pWd [in] Pointer to the OdGiWorldDraw object.
  */
  virtual bool subWorldDraw(
    OdGiWorldDraw* pWd) const;

  /** \details
    Explodes this entity into a set of simpler entities. 
    
    \param entitySet [in] Receives an array of pointers to the new entities.
    
    \returns
    Returns eOk if successful, or an appropriate error code if not.
  */
  virtual OdResult subExplode(OdRxObjectPtrArray& entitySet) const;
};
/** \details
  This template class is a specialization of the OdSmartPtr class for OdDbMInsertBlock object pointers.
*/
typedef OdSmartPtr<OdDbMInsertBlock> OdDbMInsertBlockPtr;

#include "TD_PackPop.h"

#endif
