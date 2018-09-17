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


#ifndef ODDBLONGTRANSWORKSETITERATOR_INCLUDED
#define ODDBLONGTRANSWORKSETITERATOR_INCLUDED

#include "DbObject.h"
#include "DwgDeclareMembers.h"

/** \details
    This class implements Iterator objects that traverse OdDbLongTransaction worksets.

    <group OdDb_Classes> 
*/
class ODRX_ABSTRACT TOOLKIT_EXPORT OdDbLongTransWorkSetIterator : public OdRxObject
{
public:
  ODRX_DECLARE_MEMBERS(OdDbLongTransWorkSetIterator);
  
  /** \details
    Sets this Iterator object to reference the entity that it would normally return first.
  
    \param incRemovedObjs [in]  True to include removed objects.
    \param incSecondaryObjs [in]  True to include secondary objects.
       
    \remarks
    Allows multiple traversals of the iterator list.
  */
    virtual void start(
    bool incRemovedObjs = false, 
    bool incSecondaryObjs = false) = 0; 
  
  /** \details
    Returns true if and only if the traversal by this Iterator object is complete.
  */
    virtual bool done() = 0;

  /** \details
    Sets this Iterator object to reference the entity following the current entity.
  */
    virtual void step() = 0;
  
  /** \details
    Returns the object ID of the entity currently referenced by this Iterator object.
  */
    virtual OdDbObjectId objectId() const = 0;
  
  /** \details
    Returns true if and only if the entity currently referenced by this Iterator object has been erased.
    
    \remarks
    The default iterators return only non-erased objects. 
  */
    virtual bool curObjectIsErased() const = 0;

  /** \details
    Returns true if and only if the entity currently referenced by this Iterator object has been 
    removed from the work set.

    \remarks
    The default iterators return only non-removed objects.
  */
    virtual bool curObjectIsRemoved() const = 0;

  /** \details
    Returns true if and only if the entity currently referenced by this Iterator 
    object is a Primary object.

    \remarks
    Primary objects are in the work set because they have been explicitly added to it.
    
    Non-Primary objects are in the work set only because they are referenced by Primary objects.
    
    \remarks
    The default iterators return only Primary objects.
  */
    virtual bool curObjectIsPrimary() const = 0;
};

/** \details
  This template class is a specialization of the OdSmartPtr class for OdDbLongTransWorkSetIterator object pointers.
*/
typedef OdSmartPtr<OdDbLongTransWorkSetIterator> OdDbLongTransWorkSetIteratorPtr;

/** \details
    This class implements the tracking of long transactions.
    
    \remarks
    This class creates and appends OdcDbLongTransaction objects to the database and 
    and returns the object ID of the OdDbLongTransaction object. Destruction of the
    OdDbLongTransaction object is handled by the database.

    <group OdDb_Classes> 
*/
class TOOLKIT_EXPORT OdDbLongTransaction : public OdDbObject
{
public:
  DWGMAP_DECLARE_MEMBERS(OdDbLongTransaction);

  OdDbLongTransaction();
  
  enum
  {
    kSameDb        = 0, // Entities are checked out from a normal block in the current *database*.
    kXrefDb        = 1, // Entities are checked out from ModelSpace or an Xref dependent block in an Xref *database*.
    kUnrelatedDb   = 2  // Entities are checked out from ModelSpace or a block in an unrelated *database*.
  };
  
  /** \details
    Returns the type of the pending long transaction:
    
    \remarks
    type will return one of the following
    
    <table>
    Name           Value   Description
    kSameDb        0       Entities are checked out from a normal block or layout in the current database.
    kXrefDb        1       Entities are checked out from ModelSpace or an Xref dependent block in an Xref database.
    kUnrelatedDb   2       Entities are checked out from ModelSpace or a block in an unrelated database.
    </table>
    
  */
  int type() const;
  
  /** \details
    Returns the object ID of the block from which the entities are cloned.
  */
  OdDbObjectId originBlock() const;

  /** \details
    Returns the object ID of the block to which the entities are cloned.
  */
  OdDbObjectId destinationBlock() const;
  
  
  /** \details
    Returns the long transaction name.
    
    \returns
    Returns the name of the origin block or layout.
  */
  OdString getLongTransactionName() const;
  
  /** \details
    Adds the specified object to the current work set.

    \param objectId [in]  Object ID to be added.
    
    \returns
    Returns eOk if successful, or an appropriate error code if not.   
    
    objectId must be in the same database as the the transaction. 
    
    objectId can specify objects from either the origin block or destination block. 
    Objects from the destination block will be cloned; objects from the origin block will not.
  */
  OdResult addToWorkSet(OdDbObjectId ObjectId);
  /** \details
    Removes the specified object from the current work set.

    \param objectId [in]  Object ID to be removed.
    
    \returns
    Returns eOk if successful, or an appropriate error code if not.   
  */
  OdResult removeFromWorkSet(OdDbObjectId id);
  
  /** \details
    Synchronizes work set.
    \remarks
    removeFromWorkSet() does not remove from the work set objects referenced by removed objects. 
    syncWorkSet() removes them.
  */
  OdResult syncWorkSet();
  
  /** \details
    Creates an Iterator object that provides access to the entities in the work set.
    \param incRemovedObjs [in]  True to include removed and erased objects from the work set.
    \param incSecondaryObj [in]  True to include objects referenced by objects in the work set.
  */
  OdDbLongTransWorkSetIteratorPtr newWorkSetIterator(
    bool incRemovedObjs = false, 
    bool incSecondaryObjs = false) const;
  
  /** \details
    Returns the object ID from which the workSetId was cloned.
    \param workSetId [in]  Work set object ID..
  */
  OdDbObjectId originObject(
    OdDbObjectId workSetId) const;

  /** \details
    Returns true if and only if the specified object ID is in the work set.
    
    \param objectId [in]  Object ID being tested.
    \param getErased [in]  True to return true for erased and removed objects in the work set.
  */
  bool workSetHas(OdDbObjectId 
    ObjectId, 
    bool getErased = false) const;
  
  /** \details
    Returns a pointer to the OdDbMapping object used for OdLongTransationReactor callbacks used for checkin/checkout.
  */
  OdDbIdMapping* activeIdMap();

  void applyPartialUndo(OdDbDwgFiler* pUndoFiler, OdRxClass* pClassObj);
  virtual OdResult dwgInFields(OdDbDwgFiler* pFiler);
  virtual void dwgOutFields(OdDbDwgFiler* pFiler) const;
  virtual OdResult subErase(bool erasing);
};

typedef OdSmartPtr<OdDbLongTransaction> OdDbLongTransactionPtr;

#endif // ODDBLONGTRANSWORKSETITERATOR_INCLUDED
