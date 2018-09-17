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




#ifndef OD_DBSORTENTSTABLE_H
#define OD_DBSORTENTSTABLE_H

#include "TD_PackPush.h"

#include "DbObject.h"

#define STL_USING_UTILITY
#include "OdaSTL.h"

/** \details
    This template class is a specialization of the std::pair class for OdDbHandle-OdDbSoftPointerId pairs.
*/
typedef std::pair<OdDbHandle, OdDbSoftPointerId> HandlePair;

/** \details
    This template class is a specialization of the OdArray class for OdDbHandle-OdDbSoftPointerId pairs.
*/
typedef OdArray<HandlePair> HandlePairsArray;


/** \details
    This class implements the SortentsTable, which specifies the DrawOrder 
    of entities in an OdDbDatabase instance.

    \remarks
    Each instance of this class contains the DrawOrder for a single OdDbBlockRecord. 
    
    When drawing entities in a Block Table Record, an iterator traverses the Block Table Record 
    in the order of ascending handles. If there is a HandlePair in the SortentsTable corresponding
    to the handle of the object about to be drawn, the entity specified by the entity ID in the HandlePair is
    drawn in its stead. If there is no such HandlePair, the entity referenced by the iterator is drawn.
    
    <group OdDb_Classes>
*/
class TOOLKIT_EXPORT OdDbSortentsTable : public OdDbObject
{
public:
  ODDB_DECLARE_MEMBERS(OdDbSortentsTable);

  OdDbSortentsTable();

  /** \details
    Function returns the handle that indicates the draw order.
  */
  void getSortHandle(const OdDbObjectId& id, OdDbHandle& h) const;

  /** \details
    Moves the entities with specified entity IDs to the bottom of this SortentsTable.

    \param entityIds [in]  Entity IDs.
  */
  void moveToBottom(
    OdDbObjectIdArray& entityIds);

  /** \details
    Moves the entities with specified entity IDs to the top of this SortentsTable.

    \param entityIds [in]  Entity IDs.
  */
  void moveToTop(
    OdDbObjectIdArray& entityIds); 

  /** \details
    Moves the entities with specified entity IDs below the target entity in this SortentsTable.

    \param entityIds [in]  Entity IDs.
    \param targetId [in]  Target entity ID.
  */
  void moveBelow(
    OdDbObjectIdArray& entityIds, 
    OdDbObjectId targetId);

  /** \details
    Moves the entities with specified entity IDs above  the target entity in this SortentsTable.

    \param entityIds [in]  Entity IDs.
    \param targetId [in]  Target entity ID.
  */
  void moveAbove(
    OdDbObjectIdArray& entityIds, 
    OdDbObjectId targetId);

  /** \details
    Swaps the DrawOrder of the specified entities in this SortentsTable.
    
    \param firstId [in]  First entity ID.
    \param secondId [in]  Second entity ID.
  */
  void swapOrder(
    OdDbObjectId firstId, 
    OdDbObjectId secondId);

  /** \details
    Returns the Object ID of the Block Table Record to which this SortentsTable belongs.
  */
  OdDbObjectId  blockId() const; 
        
  ///  2005 functions
  ///

  /** \details
    Returns true if and only if the first entity is drawn before the second with this SortentsTable.

    \param firstID [in]  First entity ID.
    \param secondID [in]  Second entity ID.
    
    \returns
    Returns eOk if successful, or an appropriate error code if not.
  */
  bool firstEntityIsDrawnBeforeSecond(
    OdDbObjectId firstID, 
    OdDbObjectId secondID) const;

  /** \details
    Returns an array of the entity IDs of the entities in the associated Block Table Record,
    in the DrawOrder for this SortentsTable.

    \param entityIds [out]  Entity IDs.
    \param honorSortentsMask [in]  SORTENTS mask. 

    \remarks
    If (honorSortentsMask ^ SORTENTS) != honorSortentsMask, the entities are returned unsorted.
    \returns
    Returns eOk if successful, or an appropriate error code if not.
    
    \remarks
    As implemented, honorSortentsMask is ignored.
    It will be fully implemented in a future release.
  */
  void getFullDrawOrder(
    OdDbObjectIdArray& entityIds, 
    OdUInt8 honorSortentsMask = 0) const;

  /** \details
    Rearranges the specified entity IDs into their current relative DrawOrder for this SortentsTable.
    
    \param entityIds [in/out] Entity IDs.
    \param honorSortentsMask [in] SORTENTS mask. 

    \remarks
    If (honorSortentsMask ^ SORTENTS) != honorSortentsMask, the entities are returned unsorted.
    \returns
    Returns eOk if successful, or an appropriate error code if not.

    \remarks
    As implemented, honorSortentsMask is ignored.
    It will be fully implemented in a future release.
  */
  void getRelativeDrawOrder(
    OdDbObjectIdArray& entityIds, 
    OdUInt8 honorSortentsMask = 0) const;


  /** \details
    Sets the relative DrawOrder for the specified entities in this SortentsTable.

    \param entityIds [in]  Entity IDs in DrawOrder.

    \remarks
    The DrawOrder of other entities are unaffected.
    
    Returns eOk if successful, or an appropriate error code if not.
  */
  void setRelativeDrawOrder(
    const OdDbObjectIdArray& entityIds);

  /** \details
    Sets this SortentsTable from an array of HandlePair pairs.
    
    \param handlePairs [in]  HandlePairs.
    
    \remarks
    Use of this function is not recommended.
  */
  void setAbsoluteDrawOrder(
    const HandlePairsArray& handlePairs);

  /** \details
    Returns this SortentsTable as an array of HandlePair pairs.
    
    \param handlePairs [out]  Receives an array of HandlePairs.
    
    \remarks
    Use of this function is not recommended.
  */
  void getAbsoluteDrawOrder(
    HandlePairsArray& handlePairs);

  virtual OdResult dwgInFields(
    OdDbDwgFiler* pFiler);

  virtual void dwgOutFields(
    OdDbDwgFiler* pFiler) const;

  virtual OdResult dxfInFields(
    OdDbDxfFiler* pFiler);

  virtual void dxfOutFields(
    OdDbDxfFiler* pFiler) const;

  virtual OdResult subGetClassID(
    void* pClsid) const;
};


/** \details
    This template class is a specialization of the OdSmartPtr class for OdDbSortentsTable object pointers.
*/
typedef OdSmartPtr<OdDbSortentsTable> OdDbSortentsTablePtr;

#include "TD_PackPop.h"

#endif //OD_DBSORTENTSTABLE_H

