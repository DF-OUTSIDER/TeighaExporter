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

#ifndef _OD_DWFTUBEXT_INCLUDED_
#define _OD_DWFTUBEXT_INCLUDED_

#include "OdDbStub.h"
#include "DwfObject.h" // "DwfObjectImpl.h"

class OdDwfStubExt : public OdDbStub
{
public:
  OdDwfStubExt(OdDbBaseDatabase* pDatabase, const OdDbHandle& h)
    : OdDbStub(pDatabase, h)
  {
  }

  //void bindObject(OdDwfObject* pObj)
  //{
  //  setObject(pObj);
  //  setFlags(0, kOdDbIdErased|kErasedPermanently);
  //  pObj->objectId() = this;
  //  //OdDwfObjectImpl::cast(pObj)->setDatabase(static_cast<DwfDatabaseImpl*>(static_cast<OdDwfObject*>(database())));
  //}

  //void erasePermanently()
  //{
  //  setFlags(kErasedPermanently, kErasedPermanently);
  //  detachObject();
  //}

  void detachObject()
  {
    if (getObject().get() && getObject()->numRefs() > 1)
    {
      OdDwfObject* pObj = OdDwfObject::cast(getObject()).get();
      if (pObj)
      {
        if (pObj->objectId() == OdDwfObjectId(this))
        {
          pObj->objectId() = OdDwfObjectId::kNull;
          //OdDwfObjectImpl::cast(pObj)->setDatabase(0);
        }
        getObject().release();
      }
    }
  }

  OdDwfObjectId ownerId() const 
  { 
    return m_pOwner; 
  }
  void setOwnerId(OdDwfObjectId id) 
  { 
    m_pOwner = id; 
  }
};

class OdDwfStubExtFactory : public OdDbStubFactory
{
public:
  virtual OdDbStub* createStub(OdDbBaseDatabase* pDb, const OdDbHandle& h)
  {
    return new OdDwfStubExt(pDb, h);
  }
  virtual void freeStub(OdDbStub* pStub)
  {
    if (pStub)
    {
      OdDwfStubExt* pStubExt = static_cast<OdDwfStubExt*>(pStub);
      pStubExt->detachObject();
      delete pStubExt;
    }
  }
};

#endif // _OD_DWFTUBEXT_INCLUDED_
