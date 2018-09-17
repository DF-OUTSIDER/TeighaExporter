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




#ifndef ODDB_DBBASESUBENT_H
#define ODDB_DBBASESUBENT_H /*!DOM*/

#include "TD_PackPush.h"

#include "DbRootExport.h"
#include "DbStubPtrArray.h"

namespace OdDb
{
  enum SubentType
  {
    kNullSubentType = 0,
    kFaceSubentType,
    kEdgeSubentType,
    kVertexSubentType,
    kMlineSubentCache,
	  kClassSubentType
  };
}

/** \details
    This class implements Subentity Id objects for OdDbEntity objects in an OdDbDatabase instance.  

    \remarks
    A given OdDbEntity object may consist of any number of graphical subentities.
    
    Subentity type must be one of the following:

    <table>
    Name                        Value
    OdDb::kNullSubentType       0
    OdDb::kFaceSubentType       1
    OdDb::kEdgeSubentType       2
    OdDb::kVertexSubentType     3   
    OdDb::kMlineSubentCache     4  
    </table>

    Each SubentityId object is created from a Subentity type and an index.
    
    Corresponding C++ library: TD_DbRoot

    <group OdDbRoot_Classes>
*/
class DBROOT_EXPORT OdDbSubentId
{
public:
  OdDbSubentId()
    : m_Type(OdDb::kNullSubentType)
    , m_Index(0) 
  {}

  OdDbSubentId(OdDb::SubentType type, OdGsMarker index)
    : m_Type(type)
    , m_Index(index) 
  {}

  bool operator ==(const OdDbSubentId& subentId) const
  {
    return ((m_Index == subentId.m_Index) && (m_Type == subentId.m_Type));
  }
      
  bool operator !=(const OdDbSubentId& subentId) const
  {
    return ((m_Index != subentId.m_Index) || (m_Type != subentId.m_Type));
  }
  
  /** \details
    Returns the type of this SubentId object.

    \remarks
    
    type returns one of the following:

    <table>
    Name                        Value
    OdDb::kNullSubentType       0
    OdDb::kFaceSubentType       1
    OdDb::kEdgeSubentType       2
    OdDb::kVertexSubentType     3   
    OdDb::kMlineSubentCache     4  
    </table>
  */
  OdDb::SubentType type () const 
  { 
    return m_Type; 
  }
  
  /** \details
    Sets the type of this SubentId object.

    \param type [in]  Type.
    
    \remarks
    type must be one of the following:

    <table>
    Name                        Value
    OdDb::kNullSubentType       0
    OdDb::kFaceSubentType       1
    OdDb::kEdgeSubentType       2
    OdDb::kVertexSubentType     3   
    OdDb::kMlineSubentCache     4  
    </table>
  */
  void setType(OdDb::SubentType type) 
  { 
    m_Type = type; 
  }
  /** \details
    Returns the index of this SubentId object.
  */
  OdGsMarker index() const 
  { 
    return m_Index; 
  }
  
  /** \details
    Sets the index of this SubentId object.
    \param index [in]  Index.
  */
  void setIndex(OdGsMarker index) 
  { 
    m_Index = index; 
  }

private:
  OdDb::SubentType m_Type;
  OdGsMarker       m_Index;
};

/** \details
    This base class uniquely defines subentities within in a database instance.

    \remarks
    Each OdDbBaseFullSubentPath object consists of a OdDbSubentId object and
    an ordered array of Object IDs. The SubentId consists the index and subentType
    of the object. The array of Object IDs defines the path to the 
    subentity from the outermost entity to the entity containing the subentity.
    
    Corresponding C++ library: TD_DbRoot

    <group OdDbRoot_Classes>
*/
class DBROOT_EXPORT OdDbBaseFullSubentPath
{
public:
  OdDbBaseFullSubentPath() 
  {}
  OdDbBaseFullSubentPath(OdDb::SubentType type, OdGsMarker index)
    : m_SubentId(type, index) 
  {}
  OdDbBaseFullSubentPath(OdDbStub* entId, OdDbSubentId subId)
    : m_SubentId(subId) 
  {
    m_ObjectIds.clear();
    m_ObjectIds.append(entId) ;
  }

  OdDbBaseFullSubentPath(OdDbStub* entId, OdDb::SubentType type, OdGsMarker index)
    : m_SubentId(type, index)
  {
    m_ObjectIds.clear();
    m_ObjectIds.append(entId) ;
  }

  OdDbBaseFullSubentPath(OdDbStubPtrArray objectIds, OdDbSubentId subId)
    : m_ObjectIds(objectIds) 
    , m_SubentId(subId) 
  {
  }

  void objectIds(OdDbStubPtrArray& objectIdsArg) const
  {
    objectIdsArg = m_ObjectIds;
  }

  OdDbBaseFullSubentPath& operator =(const OdDbBaseFullSubentPath& fullSubentPath)
  {
    m_ObjectIds = fullSubentPath.objectIds();
    m_SubentId  = fullSubentPath.subentId();
    return *this;
  }

  bool operator==(const OdDbBaseFullSubentPath& fullSubentPath) const
  {
    return m_ObjectIds == fullSubentPath.objectIds() && m_SubentId == fullSubentPath.subentId();
  }

  /** \details
    Returns a reference to the embedded OdDbStubPtrArray object in this BaseFullSubentPath object.
  */
  const OdDbStubPtrArray& objectIds() const
  {
    return m_ObjectIds;
  }
  
  /** \details
    Returns a reference to the embedded OdDbStubPtrArray object in this BaseFullSubentPath object.
  */
  OdDbStubPtrArray& objectIds()
  {
    return m_ObjectIds;
  }
  
  /** \details
    Returns a copy of the embedded OdDbSubentId object in this BaseFullSubentPath object.
  */
  const OdDbSubentId subentId() const
  {
    return m_SubentId;
  }

  /** \details
    Returns a reference to the embedded OdDbSubentId object in this BaseFullSubentPath object.
  */
  OdDbSubentId& subentId()
  {
    return m_SubentId;
  }
  
protected:              
  OdDbStubPtrArray  m_ObjectIds;
  OdDbSubentId      m_SubentId;
};

/** \details
    This template class is a specialization of the OdArray class for OdDbBaseFullSubentPath object pointers.
*/
typedef OdArray<OdDbBaseFullSubentPath> OdDbBaseFullSubentPathArray;

DBROOT_EXPORT OdRxObject* baseDatabaseBy(OdDbStub* id);

#include "TD_PackPop.h"

#endif
