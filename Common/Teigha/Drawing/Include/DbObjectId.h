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


#ifndef _ODDBOBJECTID_INCLUDED_
#define _ODDBOBJECTID_INCLUDED_ /* {Secret } */

#include "DbExport.h"
#include "DbHandle.h"
#include "OdError.h"
#include "TD_PackPush.h"

class OdDbDatabase;
class OdDbObject;
class OdRxClass;

template <class T> class OdSmartPtr;

/** \details
  This template class is a specialization of the OdSmartPtr class for OdDbObject object pointers.
*/
typedef OdSmartPtr<OdDbObject> OdDbObjectPtr;

class OdDbStub;
class OdDbStubExt;

/** \details
  
    <group TD_Namespaces>
*/
namespace OdDb
{
  enum OpenMode
  {
    kNotOpen           = -1,  // Object is not open.
    kForRead           = 0,   // Object is open for reading.
    kForWrite          = 1,   // Object is open for reading and/or writing.
    kForNotify         = 2    // Object is open for notify.
  };

  enum ReferenceType
  {
    kSoftPointerRef     = 0,  // Soft Pointer Reference
    kHardPointerRef     = 1,  // Hard Pointer Reference
    kSoftOwnershipRef   = 2,  // Soft Ownership Reference
    kHardOwnershipRef   = 3   // Hard Ownership Reference
  };
}

/** \details
    This class implements memory-resident ObjectId objects for OdDbDatabase objects.  

    \remarks
    Database objects reference
    other database objects using ObjectId objects, and a database object pointer
    can always be obtained from a valid ObjectId objects. The effect of this mechanism is
    that database objects do not have to reside in memory unless they are explicitly
    being examined or modified by the user.  
    
    The user must explicitly open an object
    before reading or writing to it, and should release it when the operation
    is completed.  This functionality allows Teigha to support partial loading of 
    a database, where ObjectId objects exist for all objects in the database, but 
    the actual database objects need not be loaded until they are accessed.  
    It also allows database objects that are not in use to be swapped out of memory, 
    and loaded back in when they are accessed.  ObjectId objects are not written out to a 
    DWG/DXF file.  If a reference must be preserved to a database object that has been 
    serialized, the object's database handle (OdDbHandle) should be used.

    \sa
    * OdDbHandle
    * OdDbObject

    Corresponding C++ library: TD_Db

    <group OdDb_Classes>
*/
class TOOLKIT_EXPORT OdDbObjectId
{
public:
  /** \remarks
    This function guarantees that isNull() will 
    return true if it is the first operation applied to this instance.
  */
  OdDbObjectId () : m_Id (0) { }

  /** \details
    For Teigha internal use only. 
  */

  /*!DOM*/
  OdDbObjectId (
    OdDbStub* objectId) : m_Id (objectId) { }

  /** \details
    The null ObjectId object.
  */
  TOOLKIT_EXPORT_STATIC static const OdDbObjectId kNull;

  /** \details
    Returns true and only if this ObjectId object is null.
  */
  bool isNull () const { return m_Id == 0; }

  /** \details
    Sets this Object ID to null.
  */
  void setNull () { m_Id = 0; }

  /** \details
    Returns true and only if this ObjectId object references a valid object.
  */
  bool isValid () const;

  // VS2015 requires to create copy constructor explicity
#if defined(_MSC_VER) && (_MSC_VER > 1800)
  OdDbObjectId(const OdDbObjectId& objectId) = default;
  OdDbObjectId& operator = (const OdDbObjectId& objectId) = default;
#endif

  /*!DOM*/

  OdDbObjectId& operator = (
    OdDbStub* objectId) { m_Id = objectId; return *this; }

  bool operator < (
    const OdDbObjectId& objectId) const;

  bool operator > (
    const OdDbObjectId& objectId) const;

  bool operator >= (
    const OdDbObjectId& objectId) const;

  bool operator <= (
    const OdDbObjectId& objectId) const;

  bool operator == (
    const OdDbObjectId& objectId) const;

  bool operator != (
    const OdDbObjectId& objectId) const;

  bool operator ! () const { return isNull(); }

  /** \details
    For Teigha internal use only.
  */
  
  /*!DOM*/
  inline operator OdDbStub* () const { return (OdDbStub*)m_Id; }

  /** \details
    For Teigha internal use only. 
  */
  
  /*!DOM*/
  inline OdDbStubExt* operator -> () const { return (OdDbStubExt*)m_Id; }

  /** \details
    Returns the database with which this ObjectId object is associated.
    
    \remarks
    Returns NULL if this ObjectId object is not associated with any database.
  */
  OdDbDatabase* database () const;

  /** \details
    Returns the original database with which this ObjectId object is associated.
    
    \remarks
    If the object associated with this ObjectId object has been redirected to
    a host database from an Xref database, this function returns
    a pointer to the Xref database. 
    
    Otherwise, it returns a pointer to the database with which this ObjectId object is associated.
  */
  OdDbDatabase* originalDatabase () const;

  /** \details
    If this ObjectId object has been redirected from another database (possibly an Xref), this function
    returns the actual ObjectId object for that database.
  */
  void convertToRedirectedId ();

  /** \details
    Returns true if and only if the object associated with this ObjectId object is erased.
  */
  bool isErased () const;

  /** \details
    Returns true if and only if this object associated with this ObjectId object is erased, or any of its ownership hierarchy
    have been erased.
  */
  bool isEffectivelyErased () const;

  /** \details
    For Teigha internal use only. 
  */
  
  /*!DOM*/
  bool objectLeftOnDisk () const;

  /** \details
    Returns the database handle of the object referenced by this ObjectId object.
    
    \remarks
    Returns NULL if no database object is referenced by this ObjectId object.

    If this ObjectId object has been redirected from another database (possibly an Xref), this function
    returns the handle for this database.
  */
  const OdDbHandle& getHandle () const;

  /** \details
    Returns the database handle of the object referenced by this ObjectId object.
    
    \remarks
    Returns NULL if no database object is referenced by this ObjectgId object.

    If this ObjectId object has been redirected from another database (possibly an Xref), this function
    returns the handle for the original database.
  */
  const OdDbHandle& getNonForwardedHandle () const;

  /** \details
    Opens the database object associated with this ObjectId object, in the specified mode.

    \param pObj [out]  Smart pointer to opened object
    \param openMode [in]  Mode in which to open the object.
    \param openErasedOne [in]  If and only if true, erased objects will be opened.

    \returns
    Returns eOk if successful, otherwise an Error Code.
    
    openMode must be one of the following:
    
    <table>
    Name                Value     Description
    OdDb::kForRead      0         Object is open for reading.
    OdDb::kForWrite     1         Object is open for reading and/or writing.
    OdDb::kForNotify    2         Object is open for notify.
    </table>
    
  */
  OdResult openObject (
    OdDbObjectPtr& pObj,
    OdDb::OpenMode openMode = OdDb::kForRead, 
    bool openErasedOne = false) const;
  /** \details
    Opens the database object associated with this ObjectId object, in the specified mode.

    \param openMode [in]  Mode in which to open the object.
    \param openErasedOne [in]  If and only if true, erased objects will be opened.

    \returns
    Returns a SmartPointer to the opened object if successful, otherwise a null SmartPointer.
    
    \remarks
    openMode must be one of the following:
    
    <table>
    Name                Value     Description
    OdDb::kForRead      0         Object is open for reading.
    OdDb::kForWrite     1         Object is open for reading and/or writing.
    OdDb::kForNotify    2         Object is open for notify.
    </table>
    
  */
  OdDbObjectPtr openObject (
    OdDb::OpenMode openMode = OdDb::kForRead, 
    bool openErasedOne = false) const;
  /** \details
    Opens the database object associated with this ObjectId object, in the specified mode, or throws and exception if unsucessful.

    \param openMode [in]  Mode in which to open the object.
    \param openErasedOne [in]  If and only if true, erased objects will be opened.

    \returns
    Returns a SmartPointer to the opened object.
    
    \remarks
    openMode must be one of the following:
    
    <table>
    Name                Value     Description
    OdDb::kForRead      0         Object is open for reading.
    OdDb::kForWrite     1         Object is open for reading and/or writing.
    OdDb::kForNotify    2         Object is open for notify.
    </table>

    Throws:
    
    <table>
    Exception           Cause
    eNullObjectId       This ObjectId object is null.
    ePermanentlyErased  Not opened and openErasedOne == 1
    eWasErased          Not opened and openErasedOne == 0
    </table>
    
  */
    OdDbObjectPtr safeOpenObject (
    OdDb::OpenMode openMode = OdDb::kForRead, 
    bool openErasedOne = false) const;

  /** \details
    Binds the specified object.
    
    \param pObj [in]  Pointer to the object to bind.
  */
  void bindObject (
    OdDbObject* pObj);

  OdRxClass* objectClass() const;

protected:
  friend class OdDbStub;
  OdDbStub* m_Id;
};

/** \details
    This class is a specialization of OdDbObjectId indicating a hard owner relationship.

    \remarks
    An OdDbHardOwnershipId reference to another object is used when the owner requires the owned
    object, and the owned object cannot exist on its own.
    
    An OdDbHardOwnershipId reference to an object dictates that the owned object is written to 
    .dwg and .dxf files whenever the owner object is written.
    
    Corresponding C++ library: TD_Db

    <group OdDb_Classes>
*/
class TOOLKIT_EXPORT OdDbHardOwnershipId : public OdDbObjectId
{
public:

  OdDbHardOwnershipId () {}

  OdDbHardOwnershipId (
    const OdDbObjectId& objectId) : OdDbObjectId (objectId) {}

  /*!DOM*/
  OdDbHardOwnershipId (
    OdDbStub* objectId) : OdDbObjectId (objectId) {}

  OdDbHardOwnershipId& operator = (
    const OdDbObjectId& objectId) { OdDbObjectId::operator= (objectId); return *this; }


  /*!DOM*/
  OdDbHardOwnershipId& operator = (
    OdDbStub* objectId) { m_Id = objectId; return *this; }

  bool operator != (
    const OdDbObjectId& objectId) const { return OdDbObjectId::operator!= (objectId); }

  /*!DOM*/
  bool operator != (
    OdDbStub* objectId) const { return OdDbObjectId::operator!= (objectId); }

  bool operator == (
    const OdDbObjectId& objectId) const { return OdDbObjectId::operator== (objectId); }

  /*!DOM*/
  bool operator == (
    OdDbStub* objectId) const { return OdDbObjectId::operator== (objectId); }
};

/** \details
    This class is a specialization of OdDbObjectId indicating a soft owner relationship.

    \remarks
    An OdDbSoftOwnershipId reference to another object is used when the owner does not requires the owned
    object, and the owned object cannot exist on its own.

    An OdDbSoftOwnershipId reference to an object dictates that the owned object is written to 
    .dwg and .dxf files whenever the owner object is written.
    
    Corresponding C++ library: TD_Db

    <group OdDb_Classes>
*/
class TOOLKIT_EXPORT OdDbSoftOwnershipId : public OdDbObjectId
{
public:

  OdDbSoftOwnershipId () {}

  OdDbSoftOwnershipId (
    const OdDbObjectId& objectId) : OdDbObjectId (objectId) {}

  /*!DOM*/
  OdDbSoftOwnershipId (
    OdDbStub* objectId) : OdDbObjectId (objectId) {}

  OdDbSoftOwnershipId& operator = (
    const OdDbObjectId& objectId) { OdDbObjectId::operator= (objectId); return *this; }

  /*!DOM*/
  OdDbSoftOwnershipId& operator = (
    OdDbStub* objectId) { m_Id = objectId; return *this; }
    
  bool operator != (
    const OdDbObjectId& objectId) const { return OdDbObjectId::operator!= (objectId); }

  /*!DOM*/
  bool operator != (
    OdDbStub* objectId) const { return OdDbObjectId::operator!= (objectId); }

  bool operator == (
    const OdDbObjectId& objectId) const { return OdDbObjectId::operator== (objectId); }

  /*!DOM*/
  bool operator == (
    OdDbStub* objectId) const { return OdDbObjectId::operator== (objectId); }

};

/** \details
    This class is a specialization of OdDbObjectId indicating a hard pointer relationship.

    \remarks
    An OdDbHardPointerId reference to another object is used when one object depends on the existance of another, but
    both are owned by other objects.

    Corresponding C++ library: TD_Db

    <group OdDb_Classes>
*/
class TOOLKIT_EXPORT OdDbHardPointerId : public OdDbObjectId
{
public:

  OdDbHardPointerId () {}

  OdDbHardPointerId (
    const OdDbObjectId& objectId) : OdDbObjectId (objectId) {}

  /*!DOM*/
  OdDbHardPointerId (
    OdDbStub* objectId) : OdDbObjectId (objectId) {}

  OdDbHardPointerId& operator = (
    const OdDbHardPointerId& objectId) { OdDbObjectId::operator= (objectId); return *this; }

  OdDbHardPointerId& operator = (
    const OdDbObjectId& objectId) { OdDbObjectId::operator= (objectId); return *this; }

  /*!DOM*/
  OdDbHardPointerId& operator = (
    OdDbStub* objectId) { m_Id = objectId; return *this; }

  bool operator != (
    const OdDbObjectId& objectId) const { return OdDbObjectId::operator!= (objectId); }

  /*!DOM*/
  bool operator != (
    OdDbStub* objectId) const { return OdDbObjectId::operator!= (objectId); }

  bool operator == (
    const OdDbObjectId& objectId) const { return OdDbObjectId::operator== (objectId); }

  /*!DOM*/
  bool operator == (
    OdDbStub* objectId) const { return OdDbObjectId::operator== (objectId); }
};

/** \details
    This class is a specialization of OdDbObjectId indicating a soft pointer relationship.

    \remarks
    An OdDbSoftPointerId reference another object is used when neither object depends on the existance of the other.

    Corresponding C++ library: TD_Db

    <group OdDb_Classes>
*/
class TOOLKIT_EXPORT OdDbSoftPointerId : public OdDbObjectId
{
public:
  OdDbSoftPointerId () {}

  OdDbSoftPointerId (
    const OdDbObjectId& objectId) : OdDbObjectId (objectId) {}

  /*!DOM*/
  OdDbSoftPointerId (
    OdDbStub* objectId) : OdDbObjectId (objectId) {}

  OdDbSoftPointerId& operator = (
    const OdDbSoftPointerId& objectId) { OdDbObjectId::operator= (objectId); return *this; }

  OdDbSoftPointerId& operator = (
    const OdDbObjectId& objectId) { OdDbObjectId::operator= (objectId); return *this; }

  /*!DOM*/
  OdDbSoftPointerId& operator = (
    OdDbStub* objectId) { m_Id = (OdDbStub*)objectId; return *this; }

  bool operator != (
    const OdDbObjectId& objectId) const { return OdDbObjectId::operator!= (objectId); }

  /*!DOM*/
  bool operator != (
    OdDbStub* objectId) const { return OdDbObjectId::operator!= (objectId); }

  bool operator == (
    const OdDbObjectId& objectId) const { return OdDbObjectId::operator== (objectId); }
  /*!DOM*/
  bool operator == (
    OdDbStub* objectId) const { return OdDbObjectId::operator== (objectId); }

};

#include "TD_PackPop.h"

#endif //_ODDBOBJECTID_INCLUDED_


