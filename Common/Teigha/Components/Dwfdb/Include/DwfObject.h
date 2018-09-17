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

#ifndef _DWFOBJECT_H_
#define _DWFOBJECT_H_

#include "DbHandle.h"
#include "Gi/GiDrawable.h"
#include "DwfdbExport.h"

class OdDwfObject;
class OdDwfDatabase;
class OdDwfObjectReactor;

class DWFDBEXPORT OdDwfObjectId
{
public: 
  ODRX_HEAP_OPERATORS();
  OdDwfObjectId() : m_Id(NULL) {}
  OdDwfObjectId(OdDbStub* id) : m_Id(id) {}
  ~OdDwfObjectId();

  DWFDBEXPORT_STATIC static const OdDwfObjectId kNull;

  OdDwfObjectId& operator=(OdDbStub* id) { m_Id = id; return *this; }
  bool operator!() const { return isNull(); }
  operator OdDbStub*() const { return m_Id; }
  OdDbStub* operator->() const { return reinterpret_cast<OdDbStub*>(m_Id); }

  bool isNull() const { return m_Id == NULL; }
  void setNull() { m_Id = 0; }
  bool isValid() const { return m_Id != NULL; }

  OdDwfDatabase* database() const;
  OdDbHandle getHandle() const;

  OdRxObjectPtr getObject();
  void setObject(OdRxObject* pObj);

  OdDbStub* owner();
  const OdDbStub* owner() const;
  void setOwner(OdDbStub* pOwner);

  OdSmartPtr<OdDwfObject> openObject() const;
  OdSmartPtr<OdDwfObject> safeOpenObject() const;

  bool isErased() const;

protected:
  OdDbStub* id() const { return m_Id; }
  class OdDbStub* m_Id;
};

typedef OdArray<OdDwfObjectId, OdMemoryAllocator<OdDwfObjectId> > OdDwfObjectIdArray;

class DWFDBEXPORT OdDwfObject : public OdGiDrawable // OdGiDrawableImpl
{
public:
  ODRX_DECLARE_MEMBERS(OdDwfObject);

  virtual OdDwfObjectId objectId() const = 0;
  virtual OdDbStub* ownerId() const = 0;

  virtual void addReactor(OdDwfObjectReactor* pReactor) = 0;
  virtual void removeReactor(OdDwfObjectReactor* pReactor) = 0;
};

typedef OdSmartPtr<OdDwfObject> OdDwfObjectPtr;

#endif // _DWFOBJECT_H_
