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


#ifndef _ODDB_DBSTUB_INCLUDED_
#define _ODDB_DBSTUB_INCLUDED_

//
// Internal Id Flags
// NOTE: Define flags before Includes because of porting issue
//
#define kOdDbIdModified        0x00000001  // OdDbIndexUpdateData::kModified
#define kOdDbIdErased          0x00000002  // OdDbIndexUpdateData::kDeleted
#define kOdDbIdProcessed       0x00000004
#define kOdDbIdLoading         0x00000008
#define kOdDbIdTruncXref       0x00000010  // used during decomposeDatabase
#define kOdDbIdOwned           0x00000020  // used during id filer output to avoid cycled ownership
#define kOdDbIdReferenced      0x00000040  // used during id filer output for purging
#define kOdDbIdJustAppended    0x00000080  // Bricsys change. The flag is set when the object is appended to db and cleared when the active ps layout is switched; used in oddbEntNext.
#define kOdDbIdUserMask        0x000000FF

#define kErasedPermanently     0x10000000
#define kObjectLeftOnDisk      0x20000000
#define kObjectSameOnDisk      0x40000000  // used for unloading
#define kObjectPageOnDisk      0x80000000  // used for paging

#define kOdDbIdMapping         0x01000000
#define kOdDbIdMapFlag         0x02000000
#define kOdDbIdMapOwnerXlated  0x04000000
#define kOdDbIdMapCloned       0x08000000
#define kOdDbIdMapPrimary      0x00001000
#define kOdDbIdMapMask         0x0E001000
#define kOdDbIdMappingMask     0x0F001000

#define kOdDbIdRedirected      0x00000100
// ODA_MT_DB_BEGIN
#define kOdDbIdNoAutoLock      0x00000200  // support no auto locking mode
#define kOdDbObjectLocked      0x00000400  // support exclusive reading mode
#define kComposeForLoadCalled  0x00000800  // if compose for load was called for this object
// ODA_MT_DB_END

#define kOdDatabaseFlag        0x00100000  // unused
#define kOdDbIdBlkChgIterFlag  0x00200000
#define kOdDbIdNdxUpdtDataFlag 0x00400000
#define kOdDbIdSingleAuxData   0x00800000  // flag controls the keeping of OdDbStub's auxiliary data

// Flags for different OdDbStub's auxiliary data
#define kOdDbIdIndexData       0x00010000
#define kOdDbIdMappingData     0x00020000 
#define kOdDbIdPaging          0x00040000

#define kOdDbNullTransResident 0x00080000

#include "DbRootExport.h"
#include "BitFlags.h"
#include "OdAuxDataBitList.h"
#include "DbHandle.h"
#include "RxObject.h"
#include "DbBaseDatabase.h"
#include "SharedPtr.h"


class OdDbStub;
class OdHandleTree;

typedef OdAuxDataBitList
<
  OdUInt32, 
  OdBitSet<OdUInt32>,
  kOdDbIdSingleAuxData,
  kOdDbIdIndexData,
  kOdDbIdPaging
>
OdDbStubAuxData;


class DBROOT_EXPORT OdDbStub : public OdDbStubAuxData
{
protected:
  OdDbStub(OdDbBaseDatabase* pDatabase, const OdDbHandle& h)
    : m_handle(h)
    , m_pOwner(0)
    , m_pDb(pDatabase)
  {}

public:
  ODRX_HEAP_OPERATORS();
  ~OdDbStub() {}

  OdDbBaseDatabase* database() const { return m_pDb; }

  const OdDbHandle& getHandle() const { return m_handle; }

  OdRxObjectPtr& getObject() { return m_pObject; }
  void setObject(OdRxObject* pObj) { m_pObject = pObj; }

  void lockObject() { m_pObject->addRef(); setFlags(kOdDbObjectLocked, kOdDbObjectLocked); }
  void unlockObject() { m_pObject->release(); setFlags(0, kOdDbObjectLocked); }
  bool isObjectLocked() { return flags(kOdDbObjectLocked) != 0; }

  OdDbStub* owner() { return m_pOwner; }
  const OdDbStub* owner() const { return m_pOwner; }
  void setOwner(OdDbStub* pOwner) { m_pOwner = pOwner; }
  
protected:
  OdDbHandle            m_handle;
  mutable OdRxObjectPtr m_pObject;
  OdDbStub*             m_pOwner;
  OdDbBaseDatabase*     m_pDb;  
  friend DBROOT_EXPORT void oddbSwapHandles(OdHandleTree* ht, const OdDbHandle& h1, const OdDbHandle& h2);
};


class DBROOT_EXPORT OdDbStubFactory
{
public:
  virtual ~OdDbStubFactory() {}
  virtual OdDbStub* createStub(OdDbBaseDatabase* pDb, const OdDbHandle& h) = 0;
  virtual void freeStub(OdDbStub* pStub) = 0;
};

typedef OdSharedPtr<OdDbStubFactory> OdDbStubFactoryPtr;

typedef OdDbStubFactory* (*OdStubFactoryFunc) ();


class OdDbStubAutoLock
{
public:
  OdDbStubAutoLock(OdDbStub* id, bool isMTMode = true)
    : m_id(0)
  {
    if (isMTMode)
    {
      m_id = id;
      m_id->lockObject();
    }
  }
  ~OdDbStubAutoLock()
  { 
    if (m_id)
      m_id->unlockObject();
  }
private:
  OdDbStub* m_id;
};


#endif // _ODDB_DBSTUB_INCLUDED_
