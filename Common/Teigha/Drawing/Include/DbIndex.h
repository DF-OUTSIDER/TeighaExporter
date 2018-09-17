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




#ifndef OD_DBINDEX_H
#define OD_DBINDEX_H

#include "TD_PackPush.h"

class OdDbDate;
class OdDbIndexIterator; 
class OdDbFilter;
class OdDbIndexUpdateData;
class OdDbBlockChangeIterator;
class OdDbFilter;
class OdDbBlockTableRecord;
class OdDbBlockChangeIterator;
class OdDbIndexUpdateData;
class OdDbBlockChangeIteratorImpl;
class OdDbIndexUpdateDataImpl;
class OdDbIndexUpdateDataIteratorImpl;
class OdDbFilteredBlockIterator;

#include "DbFilter.h"

/** \details
  This template class is a specialization of the OdSmartPtr class for OdDbFilteredBlockIterator object pointers.
*/
typedef OdSmartPtr<OdDbFilteredBlockIterator> OdDbFilteredBlockIteratorPtr;

extern void processBTRIndexObjects(
  OdDbBlockTableRecord* pBlock, 
  int indexCtlVal,
  OdDbBlockChangeIterator* pBlkChgIter,
  OdDbIndexUpdateData* pIdxUpdData);


/** \details
    This class iterates through changed entities in an OdDbBlockTableRecord instance.
    
    \sa
    TD_Db

    <group OdDb_Classes>
*/
class TOOLKIT_EXPORT OdDbBlockChangeIterator
{
private:
  friend class OdDbBlockChangeIteratorImpl;
  OdDbBlockChangeIteratorImpl* m_pImpl;
  OdDbBlockChangeIterator() : m_pImpl(0) {}
  OdDbBlockChangeIterator(
    const OdDbBlockChangeIterator&);
public:

  /** \details
    Sets this Iterator object to reference the entity that it would normally return first.
     
    \remarks
    Allows multiple traversals of the iterator list.
  */
    void start();

  /** \details
    Returns the Object ID of the entity currently referenced by this Iterator object.
  */
    OdDbObjectId id() const;
  
  /** \details
    Sets this Iterator object to reference the entity following the current entity.
  */
    void next();

  /** \details
    Returns true if and only if the traversal by this Iterator object is complete.
  */
    bool done(); 
  
  /** \details
    Returns the Object ID of the entity currently referenced by this Iterator object, 
    and the flags and data associated with it.
    
    \param currentId [out]  Receives the current Object ID.
    \param flags [out]  Receives the 8-bit flags.
    \param data [out]  Receives the 32-bit data
  */
    void curIdInfo(
    OdDbObjectId& currentId, 
    OdUInt8& flags, 
    OdUInt32& data) const;
    
  /** \details
    Sets the flags and data associated with the object currently referenced by this Iterator object.
    
    \param flags [in]  Current object 8-bit flags.
    \param data [in]  Current object 32-bit data
  */
    void setCurIdInfo(
    OdUInt8 flags, 
    OdUInt32 data);
  
  /** \details
    Returns the OdDbIndexUpdateData object associating Object IDs to data and flags. 
  */
  OdDbIndexUpdateData* updateData() const;
  
  /** \details
    Clears the processed bit (0x04) of the flags of entities being traversed.
  */
  void clearProcessedFlags();
};

/** \details
    This class associates Index specific flags and data with an OdDbObjectId. 
    
    \sa
    TD_Db
    
    <group OdDb_Classes>
*/
class TOOLKIT_EXPORT OdDbIndexUpdateData 
{
  friend class OdDbIndexUpdateDataImpl;
  OdDbIndexUpdateDataImpl* m_pImpl;
  OdDbIndexUpdateData() : m_pImpl(0) {}
  OdDbIndexUpdateData(
    const OdDbIndexUpdateData&);
public:
  enum UpdateFlags 
  { 
    kModified   = 1, // Modified RO
    kDeleted    = 2, // Deleted RO
    kProcessed  = 4, // Processed RW
    kUnknownKey = 8  // Unknown key
  };
  /** \details
    Returns the Object ID of the OdDbBlockTableRecord instance that owns this OdDbIndexUpdateData object.
  */
  OdDbObjectId objectBeingIndexedId() const;

  
  /** \details
    Adds the specified Object ID to this OdDbIndexUpdateData object.

    \param Object ID [in] Object ID to be added.
  */
  void addId(
    OdDbObjectId objectId);
  
  /** \details
    Sets the flags associated with the specified Object ID in this OdDbIndexUpdateData object.
    
    \param objectID [in]  Object ID.
    \param flags [in]  8-bit flags.
    
    \remarks
    Returns true if and only if successful.
  */
  bool setIdFlags(
    OdDbObjectId objectId, 
    OdUInt8 flags);

  /** \details
    Sets the data associated with the specified Object ID in this OdDbIndexUpdateData object.
    
    \param objectID [in]  Object ID.
    \param data [in]  32-bit data.
    
    \remarks
    Returns true if and only if successful.
  */
  bool setIdData(
    OdDbObjectId objectId, 
    OdUInt32 data);

  /** \details
    Returns the data associated with the specified Object ID in this OdDbIndexUpdateData object.
    
    \param objectID [in]  Object ID.
    \param data [out]  32-bit data.
    
    \remarks
    Returns true if and only if successful.
  */
  bool getIdData(
    OdDbObjectId objectId, 
    OdUInt32& data) const;

  /** \details
    Returns the flags associated with the specified Object ID in this OdDbIndexUpdateData object.
    
    \param objectID [in]  Object ID.
    \param flags [out]  Receives the 8-bit flags.
    
    \remarks
    Returns true if and only if successful.
  */
  bool getIdFlags(
    OdDbObjectId objectId, 
    OdUInt8& flags) const; 

  /** \details
    Returns the flags and data associated with the specified Object ID in this OdDbIndexUpdateData object.
    
    \param objectID [in]  Object ID.
    \param flags [out]  Receives the 8-bit flags.
    \param data [out]  Receives the 32-bit data.
    
    \remarks
    Returns true if and only if successful.
  */
  bool getFlagsAndData(
    OdDbObjectId objectId, 
    OdUInt8& flags, 
    OdUInt32& data) const;
};

/** \details
    This class iterates through OdDbIndexUpdateData instances.    

    \sa
    TD_Db

    <group OdDb_Classes>
*/
class TOOLKIT_EXPORT OdDbIndexUpdateDataIterator
{
  friend class OdDbIndexUpdateDataIteratorImpl;
  OdDbIndexUpdateDataIteratorImpl* m_pImpl;
public:
  OdDbIndexUpdateDataIterator(
    const OdDbIndexUpdateData* pIndexUpdateData);
  
  /** \details
    Sets this Iterator object to reference the entity that it would normally return first.
     
    \remarks
    Allows multiple traversals of the iterator list.
  */
    void start();

  /** \details
    Returns the Object ID of the entity currently referenced by this Iterator object.
  */
    OdDbObjectId id() const;
  
 /** \details
    Sets this Iterator object to reference the entity following the current entity.
 */
    void next();

  /** \details
    Returns true if and only if the traversal by this Iterator object is complete.
  */
    bool done(); 
  
  /** \details
    Returns the Object ID of the entity currently referenced by this Iterator object, 
    and the flags and data associated with it.
    
    \param currentId [out]  Receives the current Object ID.
    \param flags [out]  Receives the 8-bit flags.
    \param data [out]  Receives the 32-bit data
  */
    void currentData(
    OdDbObjectId& currentId, 
    OdUInt8& flags, 
    OdUInt32& data) const;
};


/** \details
    The class is the base class for all OdDb Index objects.

    \sa
    TD_Db
    
    <group OdDb_Classes>
*/
class TOOLKIT_EXPORT OdDbIndex : public OdDbObject
{
public:
  ODDB_DECLARE_MEMBERS(OdDbIndex);

  OdDbIndex();
  
  
  /** \details
    Creates an Iterator object that provides access to the entities in this Index object.
    
    \param pFilter [in]  Pointer to the filter to be applied to this Index object.
  */
   virtual OdDbFilteredBlockIteratorPtr newIterator(
    const OdDbFilter* pFilter) const;
 
  /** \details
    Fully rebuilds this Index object from the entities in the block table record object.
    \param pIdxData [in]  Pointer to the OdDbIndexUpdateData object to be used in the rebuild. 
  */
   virtual void rebuildFull(
    OdDbIndexUpdateData* pIdxData);
  
  /** \details
    Sets the Julian lastUpdatedAt timestamp for this Index object.
    
    \param time [in]  Julian date.
  */
  void setLastUpdatedAt(
    const OdDbDate& time);
    
  /** \details
    Returns the Julian lastUpdatedAt timestamp for this Index object.
  */
  OdDbDate lastUpdatedAt() const;
  
  /** \details
    Sets the UT lastUpdatedAt timestamp for this Index object.
    
    \param time [in]  UT date.
  */
  void setLastUpdatedAtU(
    const OdDbDate& time);
    
  /** \details
    Returns the UT lastUpdatedAt timestamp for this Index object.
  */
  OdDbDate lastUpdatedAtU() const;
  
  /** \details
    Returns true if and only if this Index object is up to date.
  */
  bool isUptoDate() const; 
  
  virtual OdResult dwgInFields(
    OdDbDwgFiler* pFiler);

  virtual void dwgOutFields(
    OdDbDwgFiler* pFiler) const;

  virtual OdResult dxfInFields(
    OdDbDxfFiler* pFiler);

  virtual void dxfOutFields(
    OdDbDxfFiler* pFiler) const;

protected:
 
  /** \details
    Called by OdDbIndexFilterManager::updateIndexes() when only modifications are being registered. 

    \param iterator [in]  Iterator of modified entities.
    
    \remarks
    Modified entities includes added, deleted, and changed entities.
    
    \remarks
    This class must be implemented in custom classes derived from OdDbIndex. A full rebuild may be performed if desired.
  */
 virtual void rebuildModified(
    OdDbBlockChangeIterator* iterator);
  
  friend class OdDbIndexImpl;
  friend void processBTRIndexObjects(
    OdDbBlockTableRecord* pBTR, 
    int indexCtlVal, 
    OdDbBlockChangeIterator* pBlkChgIter, 
    OdDbIndexUpdateData* pIdxUpdData );
};
/** \details
  This template class is a specialization of the OdSmartPtr class for OdDbIndex object pointers.
*/
typedef OdSmartPtr<OdDbIndex> OdDbIndexPtr;

class OdDbBlockTableRecord;
class OdDbBlockReference;

/** \details
    This namespace provides functions pertaining to indices and filters.
    
    \sa
    TD_Db
    
    <group TD_Namespaces>
*/
namespace OdDbIndexFilterManager
{
  /** \details
    Updates all Index objects associated with all block table record objects in the specified
    OdDbDatabase object.
    
    \param pDb [in]  Pointer to the database object.
  */
  TOOLKIT_EXPORT void updateIndexes(
    OdDbDatabase* pDb);
  
  /** \details
    Adds the specified Index object to the specified block table record object.
    
    \param pBTR [in]  Pointer to the block table record object.
    \param pIndex [in]  Pointer to the Index object.
    
    \remarks
    An index of the same OdRxClass as the specified index will be deleted.
  */
  TOOLKIT_EXPORT void addIndex(
    OdDbBlockTableRecord* pBTR, 
    OdDbIndex* pIndex);
  
  /** \details
    Removes the specified Index object from the specified block table record object.
    
    \param pBTR [in]  Pointer to the Block Table Record.
    \param key [in]  Class descriptor to specify the index.
  */
  TOOLKIT_EXPORT void removeIndex(
    OdDbBlockTableRecord* pBTR, 
    const OdRxClass* key);

  
  /** \details
    Returns the specified Index object.
    
    \param pBTR [in]  Pointer to the block table record object.
    \param key [in]  Class descriptor to specify the index object.
    \param readOrWrite [in]  Mode in which to open the index object.
  */
  TOOLKIT_EXPORT OdDbIndexPtr getIndex(
    const OdDbBlockTableRecord* pBTR, 
    const OdRxClass* key, 
    OdDb::OpenMode readOrWrite = OdDb::kForRead);

  /** \param btrIndex [in]  Position of the Index object within the block table record object.
  */
    TOOLKIT_EXPORT OdDbIndexPtr getIndex(
    const OdDbBlockTableRecord* pBTR, 
    int btrIndex, 
    OdDb::OpenMode readOrWrite = OdDb::kForRead);
  
  /** \details
    Returns the number of indices associated with the block table record object.
    
    \param pBTR [in]  Pointer to the block table record object.
  */
  TOOLKIT_EXPORT int numIndexes(
    const OdDbBlockTableRecord* pBTR);
  
  /** \details
    Adds the specified Filter object to the specified block reference entity.
    
    \param pBlkRef [in]  Pointer to the block reference entity.
    \param pFilter [in]  Pointer to the Filter object.
    
    \remarks
    An filter of the same OdRxClass as the specified filter will be deleted.
  */
  TOOLKIT_EXPORT void addFilter(
    OdDbBlockReference* pBlkRef, 
    OdDbFilter* pFilter);
  
  /** \details
    Removes the specified Filter object from the specified block reference entity.
    
    \param pBlkRef [in]  Pointer to the block reference entity.
    \param key [in]  Class descriptor to specify the filter.
  */
  TOOLKIT_EXPORT void removeFilter(
    OdDbBlockReference* pBlkRef, 
    const OdRxClass* key);
  
  /** \details
    Returns the specified Filter object.
    
    \param pBlkRef [in]  Pointer to the block reference entity.
    \param key [in]  Class descriptor to specify the filter object.
    \param readOrWrite [in]  Mode in which to open the filter object.
  */
  TOOLKIT_EXPORT OdDbFilterPtr getFilter(
    const OdDbBlockReference* pBlkRef, 
    const OdRxClass* key, 
    OdDb::OpenMode readOrWrite);
  
  /** \param btrIndex [in]  Position of the Index object within the block table record object.
  */
    TOOLKIT_EXPORT OdDbFilterPtr getFilter(
    const OdDbBlockReference* pBlkRef, 
    int btrIndex, 
    OdDb::OpenMode readOrWrite);
  
  /** \details
    Returns the number of indices associated with the block reference entity.
    
    \param pBlkRef [in]  Pointer to the block reference entity.
  */
  TOOLKIT_EXPORT int numFilters(
    const OdDbBlockReference* pBlkRef);
}

#include "TD_PackPop.h"

#endif // OD_DBINDEX_H

