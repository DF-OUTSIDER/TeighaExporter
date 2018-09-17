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




#ifndef __ODDBIDMAPPING_INCLUDED__
#define __ODDBIDMAPPING_INCLUDED__

#include "TD_PackPush.h"

#include "OdaDefs.h"
#include "DbObjectId.h"
#include "DbObject.h"
#include "RxObject.h"

class OdDbDatabase;
class OdDbIdMapping;

/** \details
This template class is a specialization of the OdSmartPtr class for OdDbIdMapping object pointers.
*/
typedef OdSmartPtr<OdDbIdMapping> OdDbIdMappingPtr;

/** \details
    This class is the element class for OdDbMapping, 
    which is used in deepclone operations to map
    Object IDs from the original objects to their clones.
  
    \sa
    TD_Db
    
    \remarks
    Key is the Object ID of the original object.  
    Value is the Object ID of the cloned object.  
    
    <group OdDb_Classes>
*/
class OdDbIdPair
{
public:
  /** \param source [in]  OdDbIdPair to be copied.
    \param key [in]  Object ID to use as key.
    \param value [in]  Object ID to use as Value.
    \param cloned [in]  Key object has been cloned.
    \param ownerXlated [in]  Owner of key object has been translated.
    \param primary [in]  Key object is primary.
  */
  OdDbIdPair()
    : m_bCloned(false), m_bOwnerXlated(false), m_bPrimary(false) { }
    
  OdDbIdPair(
    const OdDbIdPair& source)
    : m_Key(source.key())
    , m_Value(source.value())
    , m_bCloned(source.isCloned())
    , m_bOwnerXlated(source.isOwnerXlated())
    , m_bPrimary(source.isPrimary()){ }

  OdDbIdPair(
    const OdDbObjectId& key)
    : m_Key(key)
    , m_bCloned(false)
    , m_bOwnerXlated(false)
    , m_bPrimary(false){ }
    
  OdDbIdPair(
    const OdDbObjectId& key, 
    const OdDbObjectId& value, 
    bool cloned = false,
    bool ownerXlated = true,
    bool primary = false)
    : m_Key(key)
    , m_Value(value)
    , m_bCloned(cloned)
    , m_bOwnerXlated(ownerXlated)
    , m_bPrimary(primary) { }

  
  /** \details
    Returns the key for this IdPair object.
  */
  OdDbObjectId key() const { return m_Key; } 

  /** \details
    Returns the value for this IdPair object.
  */
  OdDbObjectId value() const { return m_Value; }

  /** \details
    Returns true if and only if the key object for this IdPair object has been cloned.
  */
  inline bool isCloned() const { return m_bCloned; }
  
  /** \details
    Returns true if the key object for this IdPair is primary.
  */
  inline bool isPrimary() const { return m_bPrimary; }
  
  /** \details
    Returns true if and only if the owner of the key object for this IdPair object has been translated.
  */
  inline bool isOwnerXlated() const { return m_bOwnerXlated; }
  
  /** \details
    Sets the parameters for this IdPair object according to the arguments.

    \param key [in]  Object ID to use as key.
    \param value [in]  Object ID to use as Value.
    \param cloned [in]  Key object has been cloned.
    \param ownerXlated [in]  Owner of key object has been translated.    
  */
  OdDbIdPair& set(
    const OdDbObjectId& key, 
    const OdDbObjectId& value,
    bool cloned = false,
    bool ownerXlated = true,
    bool primary = false)
  {
    setKey(key);
    setValue(value);
    setCloned(cloned);
    setOwnerXlated(ownerXlated);
    return *this;
  }

    
  /** \details
    Sets the key for this IdPair object.

    \param key [in]  Object ID to use as key.
  */
  void setKey(
    const OdDbObjectId& key) { m_Key = key; }
    
  /** \details
    Sets the value for this IdPair object.

    \param value [in]  Object ID to use as Value.
  */
  void setValue(
    const OdDbObjectId& value) { m_Value = value; }
    
  /** \details
    Controls the cloned setting for this IdPair object.

    \param cloned [in]  Key object has been cloned.
  */
  void setCloned(
    bool cloned) { m_bCloned = cloned; }

  /** \details
    Controls the primary setting for this IdPair object.

    \param cloned [in]  Key object is primary.
  */
    void setPrimary(bool primary) { m_bPrimary = primary; }

  /** \details
    Controls the owner translated setting for this IdPair object.

    \param ownerXlated [in]  Owner of key object has been translated.    
  */
  void setOwnerXlated(
    bool ownerXlated) { m_bOwnerXlated = ownerXlated; }
  
private:
  OdDbObjectId m_Key;
  OdDbObjectId m_Value;
  bool m_bCloned;
  bool m_bOwnerXlated;
  bool m_bPrimary;
};

class OdDbIdMappingIter;

/** \details
    This template class is a specialization of the OdSmartPtr class for OdDbIdMappingIter object pointers.
*/
typedef OdSmartPtr<OdDbIdMappingIter> OdDbIdMappingIterPtr;

/** \details
  
    <group TD_Namespaces>
*/
namespace OdDb
{
  enum DeepCloneType
  {
    kDcCopy           = 0,    // Copy, Array, Mirror
    kDcExplode        = 1,    // Explode
    kDcBlock          = 2,    // Block definition
    kDcXrefBind       = 3,    // Xref Bind
    kDcSymTableMerge  = 4,    // Xref Attach, DxfIn, IgesIn
    kDcInsert         = 6,    // Insert of a .dwg file
    kDcWblock         = 7,    // Wblock
    kDcObjects        = 8,    // OdDbDatabase::deepCloneObjects()
    kDcXrefInsert     = 9,    // Xref Insert, Xref BInd
    kDcInsertCopy     = 10,   // Insert() 
    kDcWblkObjects    = 11    // Wblock objects
  };
}

/** \details
    This class is used in deepclone operations to map, using OdDbIdPair objects,
    Object IDs from the original objects to their clones.
    
    \sa
    TD_Db
    
    \remarks
    There is no mechanism to clear all the mappings in an OdDbIdMapping instance; a new
    instance must be created for each deepclone operation.

    <group OdDb_Classes>
*/
class TOOLKIT_EXPORT OdDbIdMapping : public OdRxObject
{
public:
  ODRX_DECLARE_MEMBERS(OdDbIdMapping);

  OdDbIdMapping() {}
  
  static OdDbIdMappingPtr createObject(OdDb::DeepCloneType);
  
  /** \details
    Adds the specified IdPair object to this IdMapping object.
    
    \param idPair [in]  IdPair to add.
  */
  virtual void assign(
    const OdDbIdPair& idPair) = 0;

  /** \details
    Returns the value of the IdPair in this IdMapping object
    that matches the key in the specified IdPair object.
    
    \param idPair [in/out] Supplies the key and receives the value.
    
    \remarks
    Returns true if and only if the key is found. 
  */
  virtual bool compute(
    OdDbIdPair& idPair) const = 0;
  /** \details
    Deletes the IdPair with the specified key from this IdMapping object.
    
    \param key [in]  Object ID key to delete.
  */
  virtual bool del(
    const OdDbObjectId& key) = 0;

  /** \details
    Creates an Iterator object that provides access to the IdPair objects in this IdMapping object.
  */
   virtual OdDbIdMappingIterPtr newIterator() = 0;
  
  /** \details
    Returns the destination database for the deepclone operation using this IdMapping object. 
  */
  virtual OdDbDatabase* destDb() const = 0;
  
  /** \details
    Sets the destination database for the deepclone operation using this IdMapping object.
    
    \param pDb [in]  Pointer to the destination database. 
  */
  virtual void setDestDb(
    OdDbDatabase* pDb) = 0;

  /** \details
    Returns the source database for the deepclone operation using this IdMapping object. 
  */
  virtual OdDbDatabase* origDb() const = 0;
  
  /** \details
    Notification function called whenever the Xref Block ID is about to be inserted
    into the destination database.
    
    Returns the ObjectID of the Xref Block.
  */
  virtual OdDbObjectId insertingXrefBlockId() const = 0;
  
  /** \details
    Returns the type of deepclone operation using this IdMapping object.
    
    \remarks
    deepCloneContext() returns one of the of the following:
    
    <table>
    Name                Value   Description
    kDcCopy             0       Copy Array Mirror
    kDcExplode          1       Explode
    kDcBlock            2       Block definition
    kDcXrefBind         3       Xref Bind
    kDcSymTableMerge    4       Xref Attach DxfIn IgesIn
    kDcInsert           6       Insert of a .dwg file
    kDcWblock           7       Wblock
    kDcObjects          8       OdDbDatabase::deepCloneObjects()
    kDcXrefInsert       9       Xref Insert Xref BInd
    kDcInsertCopy       10      Insert() 
    kDcWblkObjects      11      Wblock objects
    </table>
    
  */
  virtual OdDb::DeepCloneType deepCloneContext() const = 0;
  
  /** \details
    Returns the type of duplicate record cloning for IdMapping object.
    
    \remarks
    duplicateRecordCloning() returns one of the of the following:
    
    <table>
    Name                  Value   Description
    kDrcNotApplicable     0       Not applicable to the object.
    kDrcIgnore            1       If a duplicate record exists, use the existing record in the database, and ignore the clone.
    kDrcReplace           2       If a duplicate record exists, replace it with the cloned record.
    kDrcXrefMangleName    3       Incoming record names are mangled with <Xref>$0$<name>
    kDrcMangleName        4       Incoming record names are mangled with $0$<name>
    kDrcUnmangleName      5       Unmangle the names mangled by kDrcMangleName, then default to kDrcIgnore. Typically used by RefEdit when checking records into the original database.  
    </table>

  */
  virtual OdDb::DuplicateRecordCloning duplicateRecordCloning() const = 0;
};

/** \details
    This class defines Iterator objects that 
    traverse OdDbIdPair records in OdDbIdMapping instances.

    \sa
    TD_Db
  
    \remarks
    Instances are returned by OdDbIdMapping::newIterator().

    <group OdDb_Classes>
*/
class TOOLKIT_EXPORT OdDbIdMappingIter : public OdRxObject
{
public:
  ODRX_DECLARE_MEMBERS(OdDbIdMappingIter);

  OdDbIdMappingIter() {}
  
  /** \details
    Sets this Iterator object to reference the OdIdPair that it would normally return first.
     
    \remarks
    Allows multiple traversals of the Iterator list.
  */
    virtual void start() = 0;
  
  /** \details
    Returns the IdPair pointed to by this Iterator Object.
    
    \param idPair [out]  IdPair.
  */
   virtual void getMap(
    OdDbIdPair& idPair) = 0;

  /** \details
    Increments this Iterator object.
  */
  virtual void next() = 0;
  
  /** \details
    Returns true if and only if the traversal by this Iterator object is complete.
  */
    virtual bool done() = 0;
};


#include "TD_PackPop.h"

#endif // __ODDBIDMAPPING_INCLUDED__


