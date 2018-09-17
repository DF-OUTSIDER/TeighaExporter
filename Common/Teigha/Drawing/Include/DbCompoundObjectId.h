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

#ifndef OdDbCompoundObjectId_INCLUDED
#define OdDbCompoundObjectId_INCLUDED

#include "RxObject.h"
#include "DbObjectId.h"
#include "IdArrays.h"

class OdDbDwgFiler;
class OdDbDxfFiler;

class OdGeMatrix3d;
class OdDbIdMapping;
class OdDbCompoundObjectIdImpl;

#include "TD_PackPush.h"

/** \details

    <group OdDb_Classes>
*/
class DBENT_EXPORT OdDbCompoundObjectId : public OdRxObject
{
public:
    ODRX_DECLARE_MEMBERS(OdDbCompoundObjectId);

    virtual ~OdDbCompoundObjectId();

    OdDbCompoundObjectId& operator =(const OdDbObjectId&);
    OdDbCompoundObjectId& operator =(const OdDbCompoundObjectId&);

    bool operator ==(const OdDbCompoundObjectId&) const;
    bool operator !=(const OdDbCompoundObjectId& other) const;

    OdDbObjectId topId() const;

    OdDbObjectId leafId() const;

    OdResult getFullPath(OdDbObjectIdArray& fullPath) const;

    OdResult getPath(OdDbObjectIdArray& path) const;

    void setEmpty();

    OdResult set(const OdDbObjectId&, OdDbDatabase* pHostDatabase = NULL);

    OdResult set(const OdDbCompoundObjectId&, OdDbDatabase* pHostDatabase = NULL);

    OdResult set(const OdDbObjectId& id, const OdDbObjectIdArray& path, OdDbDatabase* pHostDatabase = NULL);

    OdResult setFullPath(const OdDbObjectIdArray& fullPath, OdDbDatabase* pHostDatabase = NULL);

    bool isEmpty() const;

    bool isValid(int validityCheckingLevel = 1) const;

    bool isExternal() const;

    bool isSimpleObjectId() const;

    OdResult getTransform(OdGeMatrix3d& trans) const;

    bool remap(const OdDbIdMapping& idMap);

    OdResult dwgOutFields(OdDbDwgFiler* pFiler, OdDbDatabase* pHostDatabase) const;

    OdResult dwgInFields(OdDbDwgFiler* pFiler, int ownerVersion);

    OdResult dxfOutFields(OdDbDxfFiler* pFiler, OdDbDatabase* pHostDatabase) const;

    OdResult dxfInFields(OdDbDxfFiler* pFiler, OdDbDatabase* pHostDatabase, int ownerVersion);

    static const OdDbCompoundObjectId& nullId();

protected:
    OdDbCompoundObjectId();
    OdDbCompoundObjectId(OdDbCompoundObjectIdImpl*);

    OdDbCompoundObjectIdImpl* m_pImpl;
};

typedef OdSmartPtr<OdDbCompoundObjectId> OdDbCompoundObjectIdPtr;

/** \details
  This PE provides transform for child objects. Implemented for Block Reference, Viewport. Can be implemented for custom objects.
  <group OdDb_Classes> 
*/
class DBENT_EXPORT OdDbParentTransformOfChildPE : public OdRxObject
{
public:
        ODRX_DECLARE_MEMBERS(OdDbParentTransformOfChildPE);
        virtual OdResult getParentTransformOfChild(const OdDbObject* pThisParent, const OdDbObjectId& childId, OdGeMatrix3d&) = 0;
};
typedef OdSmartPtr<OdDbParentTransformOfChildPE> OdDbParentTransformOfChildPEPtr;

#include "TD_PackPop.h"

#endif
