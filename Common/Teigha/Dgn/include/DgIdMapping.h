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


#ifndef __OD_DG_IDMAPPING_INCLUDED__
#define __OD_DG_IDMAPPING_INCLUDED__

#include "TD_PackPush.h"

#include "OdaDefs.h"
#include "DgElementId.h"
#include "DgElement.h"
//#include "RxObject.h"

class OdDgDatabase;
class OdDgIdMapping;

/** \details
This template class is a specialization of the OdSmartPtr class for OdDgIdMapping element pointers.
*/
typedef OdSmartPtr<OdDgIdMapping> OdDgIdMappingPtr;

/** \details
    This class is the element class for OdDgMapping, 
    which is used in deepclone operations to map
    Element IDs from the original elements to their clones.
  
    \sa
    TG_Db
    
    \remarks
    Key is the Element ID of the original element.  
    Value is the Element ID of the cloned element.  
    
    <group OdDg_Classes>
*/
class OdDgIdPair
{
public:
  /** \param source [in]  OdDgIdPair to be copied.
    \param key [in]  Element ID to use as key.
    \param value [in]  Element ID to use as Value.
    \param cloned [in]  Key element has been cloned.
    \param ownerXlated [in]  Owner of key element has been translated.    
  */
  OdDgIdPair()
    : m_bCloned(false), m_bOwnerXlated(false) { }
    
  OdDgIdPair(
    const OdDgIdPair& source)
    : m_Key(source.key())
    , m_Value(source.value())
    , m_bCloned(source.isCloned())
    , m_bOwnerXlated(source.isOwnerXlated()) { }

  OdDgIdPair(
    const OdDgElementId& key)
    : m_Key(key)
    , m_bCloned(false)
    , m_bOwnerXlated(false) { }
    
  OdDgIdPair(
    const OdDgElementId& key, 
    const OdDgElementId& value, 
    bool cloned = false, 
    bool ownerXlated = true)
    : m_Key(key)
    , m_Value(value)
    , m_bCloned(cloned)
    , m_bOwnerXlated(ownerXlated) { }

  
  /** \details
    Returns the key for this IdPair element.
  */
  OdDgElementId key() const { return m_Key; } 

  /** \details
    Returns the value for this IdPair element.
  */
  OdDgElementId value() const { return m_Value; }

  /** \details
    Returns true if and only if the key element for this IdPair element has been cloned.
  */
  inline bool isCloned() const { return m_bCloned; }
  
  /** \details
    Returns true if and only if the owner of the key element for this IdPair element has been translated.
  */
  inline bool isOwnerXlated() const { return m_bOwnerXlated; }
  
  /** \details
    Sets the parameters for this IdPair element according to the arguments.

    \param key [in]  Element ID to use as key.
    \param value [in]  Element ID to use as Value.
    \param cloned [in]  Key element has been cloned.
    \param ownerXlated [in]  Owner of key element has been translated.    
  */
  OdDgIdPair& set(
    const OdDgElementId& key, 
    const OdDgElementId& value,
    bool cloned = false, 
    bool ownerXlated = true)
  {
    setKey(key);
    setValue(value);
    setCloned(cloned);
    setOwnerXlated(ownerXlated);
    return *this;
  }

    
  /** \details
    Sets the key for this IdPair element.

    \param key [in]  Element ID to use as key.
  */
  void setKey(
    const OdDgElementId& key) { m_Key = key; }
    
  /** \details
    Sets the value for this IdPair element.

    \param value [in]  Element ID to use as Value.
  */
  void setValue(
    const OdDgElementId& value) { m_Value = value; }
    
  /** \details
    Controls the cloned setting for this IdPair element.

    \param cloned [in]  Key element has been cloned.
  */
  void setCloned(
    bool cloned) { m_bCloned = cloned; }

  /** \details
    Controls the owner translated setting for this IdPair element.

    \param ownerXlated [in]  Owner of key element has been translated.    
  */
  void setOwnerXlated(
    bool ownerXlated) { m_bOwnerXlated = ownerXlated; }
  
private:
  OdDgElementId m_Key;
  OdDgElementId m_Value;
  bool m_bCloned;
  bool m_bOwnerXlated;
};

class OdDgIdMappingIter;

/** \details
    This template class is a specialization of the OdSmartPtr class for OdDgIdMappingIter element pointers.
*/
typedef OdSmartPtr<OdDgIdMappingIter> OdDgIdMappingIterPtr;

/** \details
  
    <group TG_Namespaces>
*/
namespace OdDg
{
  /** \details
   
    Corresponding C++ library: TG_Db

    <group OdDg_Classes>
  */
  enum DeepCloneType
  {
    kDcCopy           = 0,    // Copy, Array, Mirror
    kDcExplode        = 1,    // Explode
    kDcBlock          = 2,    // Block definition
    kDcXrefBind       = 3,    // Xref Bind
    kDcSymTableMerge  = 4,    // Xref Attach, DxfIn, IgesIn
    kDcInsert         = 6,    // Insert of a .dwg file
    kDcWblock         = 7,    // Wblock
    kDcObjects        = 8,    // OdDgDatabase::deepCloneObjects()
    kDcXrefInsert     = 9,    // Xref Insert, Xref BInd
    kDcInsertCopy     = 10,   // Insert() 
    kDcWblkObjects    = 11    // Wblock objects
  };
}

/** \details
    This class is used in deepclone operations to map, using OdDgIdPair elements,
    Element IDs from the original elements to their clones.
    
    \sa
    TG_Db
    
    \remarks
    There is no mechanism to clear all the mappings in an OdDgIdMapping instance; a new
    instance must be created for each deepclone operation.

    <group OdDg_Classes>
*/
class TG_EXPORT OdDgIdMapping : public OdRxObject
{
public:
  ODRX_DECLARE_MEMBERS(OdDgIdMapping);

  OdDgIdMapping() {}
  
  static OdDgIdMappingPtr createObject(OdDg::DeepCloneType);
  
  /** \details
    Adds the specified IdPair element to this IdMapping element.
    
    \param idPair [in]  IdPair to add.
  */
  virtual void assign(
    const OdDgIdPair& idPair) = 0;

  /** \details
    Returns the value of the IdPair in this IdMapping element
    that matches the key in the specified IdPair element.
    
    \param idPair [in/out] Supplies the key and receives the value.
    
    \returns
    Returns true if and only if the key is found. 
  */
  virtual bool compute(
    OdDgIdPair& idPair) const = 0;
  /** \details
    Deletes the IdPair with the specified key from this IdMapping element.
    
    \param key [in]  Element ID key to delete.
  */
  virtual bool del(
    const OdDgElementId& key) = 0;

  /** \details
    Creates an Iterator element that provides access to the IdPair elements in this IdMapping element.
  */
   virtual OdDgIdMappingIterPtr newIterator() = 0;
  
  /** \details
    Returns the destination database for the deepclone operation using this IdMapping element. 
  */
  virtual OdDgDatabase* destDb() const = 0;
  
  /** \details
    Sets the destination database for the deepclone operation using this IdMapping element.
    
    \param pDb [in]  Pointer to the destination database. 
  */
  virtual void setDestDb(
    OdDgDatabase* pDb) = 0;

  /** \details
    Returns the source database for the deepclone operation using this IdMapping element. 
  */
  virtual OdDgDatabase* origDb() const = 0;
  
  /** \details
    Notification function called whenever the Xref Block ID is about to be inserted
    into the destination database.
    
    Returns the ElementID of the Xref Block.
  */
  virtual OdDgElementId insertingXrefBlockId() const = 0;
  
  /** \details
    Returns the type of deepclone operation using this IdMapping element.
    
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
    kDcObjects          8       OdDgDatabase::deepCloneObjects()
    kDcXrefInsert       9       Xref Insert Xref BInd
    kDcInsertCopy       10      Insert() 
    kDcWblkObjects      11      Wblock objects
    </table>
    
  */
  virtual OdDg::DeepCloneType deepCloneContext() const = 0;
  
  /** \details
    Returns the type of duplicate record cloning for IdMapping element.
    
    \remarks
    duplicateRecordCloning() returns one of the of the following:
    
    <table>
    Name                  Value   Description
    kDrcNotApplicable     0       Not applicable to the element.
    kDrcIgnore            1       If a duplicate record exists, use the existing record in the database, and ignore the clone.
    kDrcReplace           2       If a duplicate record exists, replace it with the cloned record.
    kDrcXrefMangleName    3       Incoming record names are mangled with <Xref>$0$<name>
    kDrcMangleName        4       Incoming record names are mangled with $0$<name>
    kDrcUnmangleName      5       Unmangle the names mangled by kDrcMangleName, then default to kDrcIgnore. Typically used by RefEdit when checking records into the original database.  
    </table>

  */
  virtual OdDg::DuplicateRecordCloning duplicateRecordCloning() const = 0;
};

/** \details
    This class defines Iterator elements that 
    traverse OdDgIdPair records in OdDgIdMapping instances.

    \sa
    TG_Db
  
    \remarks
    Instances are returned by OdDgIdMapping::newIterator().

    <group OdDg_Classes>
*/
class TG_EXPORT OdDgIdMappingIter : public OdRxObject
{
public:
  ODRX_DECLARE_MEMBERS(OdDgIdMappingIter);

  OdDgIdMappingIter() {}
  
  /** \details
    Sets this Iterator element to reference the OdIdPair that it would normally return first.
     
    \remarks
    Allows multiple traversals of the Iterator list.
  */
    virtual void start() = 0;
  
  /** \details
    Returns the IdPair pointed to by this Iterator Object.
    
    \param idPair [out]  IdPair.
  */
   virtual void getMap(
    OdDgIdPair& idPair) = 0;

  /** \details
    Increments this Iterator element.
  */
  virtual void next() = 0;
  
  /** \details
    Returns true if and only if the traversal by this Iterator element is complete.
  */
    virtual bool done() = 0;
};


#include "TD_PackPop.h"

#endif // __OD_DG_IDMAPPING_INCLUDED__


