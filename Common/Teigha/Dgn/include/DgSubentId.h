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


#ifndef ODDG_SUBENT_H
#define ODDG_SUBENT_H /*!DOM*/

#include "DbBaseSubentId.h"
#include "DgElementId.h"

/** \details
    This class uniquely defines subentities within in an OdDbDatabase instance.  

    \remarks
    Each OdDgFullSubentPath object consists of a OdDbSubentId object and
    an ordered array of Object IDs. The SubentId consists the index and subentType
    of the object. The array of Object IDs defines the path to the 
    subentity from the outermost entity (in PaperSpace or ModelSpace) 
    to the entity containing the subentity.
    
    Corresponding C++ library: TG_Db

    <group OdDg_Classes>
*/
class TG_EXPORT OdDgFullSubentPath : public OdDbBaseFullSubentPath
{
public:
  OdDgFullSubentPath() 
  {}
  OdDgFullSubentPath(OdDb::SubentType type, OdGsMarker index)
    : OdDbBaseFullSubentPath(type, index)
  {}
  OdDgFullSubentPath(OdDgElementId entId, OdDbSubentId subId)
    : OdDbBaseFullSubentPath(entId, subId)
  {}

  OdDgFullSubentPath(OdDgElementId entId, OdDb::SubentType type, OdGsMarker index)
    : OdDbBaseFullSubentPath(entId, type, index)
  {}

  OdDgFullSubentPath(OdDgElementIdArray objectIds, OdDbSubentId subId)
    : OdDbBaseFullSubentPath(*reinterpret_cast<OdDbStubPtrArray*>(&objectIds), subId)
  {
  }
  OdDgFullSubentPath(OdDbStubPtrArray objectIds, OdDbSubentId subId)
    : OdDbBaseFullSubentPath(objectIds, subId)
  {
  }

  void objectIds(OdDgElementIdArray& objectIdsArg) const
  {
    objectIdsArg = *reinterpret_cast<const OdDgElementIdArray*>(&m_ObjectIds);
  }

  OdDgFullSubentPath& operator =(const OdDgFullSubentPath& fullSubentPath)
  {
    OdDbBaseFullSubentPath::operator=(fullSubentPath);
    return *this;
  }

  bool operator==(const OdDgFullSubentPath& fullSubentPath) const
  {
    return OdDbBaseFullSubentPath::operator==(fullSubentPath);
  }

  /** \details
    Returns a reference to the embedded OdDgElementIdArray object in this FullSubentPath object.
  */
  const OdDgElementIdArray& objectIds() const
  {
    return *reinterpret_cast<const OdDgElementIdArray*>(&m_ObjectIds);
  }
  
  /** \details
    Returns a reference to the embedded OdDgElementIdArray object in this FullSubentPath object.
  */
  OdDgElementIdArray& objectIds()
  {
    return *reinterpret_cast<OdDgElementIdArray*>(&m_ObjectIds);
  }
  
  /** \details
    Returns a copy of the embedded OdDbSubentId object in this FullSubentPath object.
  */
  const OdDbSubentId subentId() const
  {
    return m_SubentId;
  }

  /** \details
    Returns a reference to the embedded OdDbSubentId object in this FullSubentPath object.
  */
  OdDbSubentId& subentId()
  {
    return m_SubentId;
  }
};

/** \details
    This template class is a specialization of the OdArray class for OdDgFullSubentPath object pointers.
*/
typedef OdArray<OdDgFullSubentPath> OdDgFullSubentPathArray;

#endif  // ODDG_SUBENT_H


