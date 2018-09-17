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
#ifndef __DG_ELEMENTID_H__
#define __DG_ELEMENTID_H__


#include "DbHandle.h"
#include "SmartPtr.h"
#include "DgExport.h"

#include "TD_PackPush.h"

class OdDgDatabase;
class OdRxObjectPtr;
class OdDbStub;
class OdDgStub;

/** \details
  
    <group DD_Namespaces>
*/
namespace OdDg
{
  /** \details
   
    Corresponding C++ library: TG_Db

    <group OdDg_Classes>
  */
  enum OpenMode
  {
    kNotOpen           = -1,  // Object is not open.
    kForRead           = 0,   // Object is open for reading.
    kForWrite          = 1,   // Object is open for reading and/or writing.
    kForNotify         = 2    // Object is open for notify.
  };
}

/** \details
    This class implements memory-resident ElementId objects for OdDgDatabase objects.  

    \remarks
    Database objects reference
    other database objects using ElementId objects, and a database object pointer
    can always be obtained from a valid ObjectId objects. The effect of this mechanism is
    that database objects do not have to reside in memory unless they are explicitly
    being examined or modified by the user.  
    
    \sa
    *  OdDbHandle
    *  OdDgElement

    Corresponding C++ library: TG_Db

    <group OdDg_Classes>
*/
class TG_EXPORT OdDgElementId
{
public:
  /** \remarks
    This function guarantees that isNull() will 
    return true if it is the first operation applied to this instance.
  */
  OdDgElementId () : m_Id (0) { }

  /** \details
    For Teigha for .dwg files internal use only. 
  */

  /*!DOM*/
  OdDgElementId (
    OdDbStub* objectId) : m_Id (objectId) { }

  /** \details
    The null ObjectId object.
  */
  TG_EXPORT_STATIC static const OdDgElementId kNull;

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

  OdDgElementId& operator = (
    const OdDgElementId& objectId) { m_Id = objectId.m_Id; return *this; }

  /* { Secret } */
  OdDgElementId& operator = (
    OdDbStub* objectId) { m_Id = objectId; return *this; }

  bool operator < (
    const OdDgElementId& objectId) const;

  bool operator > (
    const OdDgElementId& objectId) const;

  bool operator >= (
    const OdDgElementId& objectId) const;

  bool operator <= (
    const OdDgElementId& objectId) const;

  bool operator == (
    const OdDgElementId& objectId) const;

  bool operator != (
    const OdDgElementId& objectId) const;

  bool operator ! () const;

  /** \details
    For Teigha™ for .dgn files internal use only.
  */
  
  /*!DOM*/
  inline operator OdDbStub* () const { return m_Id; }

  /** \details
    For Teigha™ for .dgn files internal use only. 
  */
  
  /*!DOM*/
//????  inline OdDbStub* operator -> () const { return (OdDbStub*)m_Id; }

  /** \details
    For Teigha internal use only. 
  */
  
  /*!DOM*/
  inline OdDgStub* operator -> () const { return (OdDgStub*)m_Id; }

  /** \details
    Returns the database with which this ObjectId object is associated.
    
    \remarks
    Returns NULL if this ObjectId object is not associated with any database.
  */
  OdDgDatabase* database () const;

  /** \details
    Returns the original database with which this ObjectId object is associated.
    
    \remarks
    If the object associated with this ObjectId object has been redirected to
    a host database from an Xref database, this function returns
    a pointer to the Xref database. 
    
    Otherwise, it returns a pointer to the database with which this ObjectId object is associated.
  */
  OdDgDatabase* originalDatabase () const;

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

    \param openMode [in]  Mode in which to open the object.
    \param openErasedOne [in]  If and only if true, erased objects will be opened.

    \returns
    Returns a SmartPointer to the opened object if successful, otherwise a null SmartPointer.
    \remarks
    openMode must be one of the following:
    
    <table>
    Name                Value     Description
    OdDg::kForRead      0         Object is open for reading.
    OdDg::kForWrite     1         Object is open for reading and/or writing.
    OdDg::kForNotify    2         Object is open for notify.
    </table>
    
  */
  OdRxObjectPtr openObject (
    OdDg::OpenMode openMode = OdDg::kForRead, 
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
    OdDg::kForRead      0         Object is open for reading.
    OdDg::kForWrite     1         Object is open for reading and/or writing.
    OdDg::kForNotify    2         Object is open for notify.
    </table>

    Throws:
    
    <table>
    Exception           Cause
    eNullObjectId       This ObjectId object is null.
    ePermanentlyErased  Not opened and openErasedOne == 1
    eWasErased          Not opened and openErasedOne == 0
    </table>
    
  */
    OdRxObjectPtr safeOpenObject (
    OdDg::OpenMode openMode = OdDg::kForRead, 
    bool openErasedOne = false) const;

protected:
  friend class OdDbStub;
  OdDbStub* m_Id;
};

#include "TD_PackPop.h"

#endif //__DG_ELEMENTID_H__


