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

#ifndef _ODDBXREFOBJECTID_H_INCLUDED_
#define _ODDBXREFOBJECTID_H_INCLUDED_

#include "TD_PackPush.h"

#include "DbObjectId.h"

/** \details
    This class representing an Xref database object Id.

    Corresponding C++ library: TD_Db
    <group OdDb_Classes>
*/
class DBENT_EXPORT OdDbXrefObjectId
{
  protected:
    OdDbObjectId m_objectId;
    OdDbHandle   m_objectHandle;
  public:
    OdDbXrefObjectId() { }

    bool operator ==(const OdDbXrefObjectId& xrefId) const
    { return (xrefId.m_objectId == m_objectId) && (xrefId.m_objectHandle == m_objectHandle); }
    bool operator !=(const OdDbXrefObjectId& xrefId) const
    { return (xrefId.m_objectId != m_objectId) || (xrefId.m_objectHandle != m_objectHandle); }

    bool isXref() const { return !m_objectHandle.isNull(); }
    bool isValid() const { return m_objectId.isValid(); }

    bool isNull() const { return m_objectId.isNull(); }
    void setNull()
    {
      m_objectId = OdDbObjectId::kNull;
      m_objectHandle = OdDbHandle();
    }

    OdDbObjectId getLocalId() const
    {
      if (isXref())
        throw OdError(eInvalidXrefObjectId);
      return m_objectId;
    }
    void setLocalId(const OdDbObjectId &objectId)
    {
      m_objectId = objectId;
      m_objectHandle = OdDbHandle();
    }

    OdDbObjectId getXrefId(OdDbHandle &objectHandle)
    {
      if (!isXref())
        throw OdError(eInvalidXrefObjectId);
      objectHandle = m_objectHandle;
      return m_objectId;
    }
    void setXrefId(const OdDbObjectId &objectId, const OdDbHandle &objectHandle)
    {
      if (objectId.isNull())
        throw OdError(eNullObjectId);
      if (objectHandle.isNull())
        throw OdError(eNullHandle);
      m_objectId = objectId;
      m_objectHandle = objectHandle;
    }

    OdDbObjectId resolveObjectId() const;

    void serializeToResbuf(OdResBufPtr &pResBuf, OdResBufPtr &pLastResBuf) const;
    void serializeFromResbuf(const OdResBufPtr pResBuf, OdResBufPtr &pNextResBuf, const OdDbDatabase* pDb);

    // Helpers
    static void seekToResbuf(OdResBufPtr &pResBuf, OdResBufPtr &pLastResBuf);
};

#include "TD_PackPop.h"

#endif // _ODDBXREFOBJECTID_H_INCLUDED_
