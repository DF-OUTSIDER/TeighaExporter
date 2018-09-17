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

#include "OdaCommon.h"

#include "DbGeomRef.h"
#include "RxObjectImpl.h"
#include "Ge/GePoint3d.h"

//////////////////////////////////////////////////////////////////////////

ODRX_NO_CONS_DEFINE_MEMBERS(OdDbGeomRef, OdRxObject)

OdDbGeomRef::~OdDbGeomRef()
{
}

void OdDbGeomRef::reset()
{
  ODA_FAIL_ONCE(); // should be overloaded
}

bool OdDbGeomRef::isValid() const
{
  ODA_FAIL_ONCE(); // should be overloaded
  return false;
}

bool OdDbGeomRef::isEmpty() const
{
  ODA_FAIL_ONCE(); // should be overloaded
  return true;
}

OdDbEntity* OdDbGeomRef::createEntity() const
{
  ODA_FAIL_ONCE(); // should be overloaded
  return NULL;
}

OdResult OdDbGeomRef::evaluateAndCacheGeometry()
{
  ODA_FAIL_ONCE(); // should be overloaded
  return eNotImplementedYet;
}

//////////////////////////////////////////////////////////////////////////
ODRX_CONS_DEFINE_MEMBERS_ALTNAME(OdDbSubentRef, OdDbGeomRef, L"AcDbSubentRef", RXIMPL_CONSTR)
//ODRX_NO_CONS_DEFINE_MEMBERS(OdDbSubentRef, OdDbGeomRef)

OdDbSubentRef::OdDbSubentRef()
  : m_pEntityId(OdDbCompoundObjectId::createObject())
{
}

OdDbSubentRef::OdDbSubentRef(const OdDbCompoundObjectId& entityId, 
                             const OdDbSubentId& subentId) // = OdDbSubentId()
  : m_pEntityId(OdDbCompoundObjectId::createObject())
  , m_SubentId(subentId)
{
  *m_pEntityId.get() = entityId;
}


OdSmartPtr<OdDbSubentRef> OdDbSubentRef::createObject( const OdDbCompoundObjectId* entityId, const OdDbSubentId& subentId/*= OdDbSubentId() */ )
{
  OdSmartPtr<OdDbSubentRef> res = OdDbSubentRef::createObject();
  res->setEntity( *entityId );
  res->setSubent( subentId );
  return res;
}

OdDbSubentRef& OdDbSubentRef::operator =(const OdDbSubentRef& from)
{
  if (this == &from)
    return *this;
  ODA_ASSERT_ONCE(m_pEntityId.get() && from.m_pEntityId.get());

  *m_pEntityId.get() = *from.m_pEntityId.get();
  m_SubentId = from.m_SubentId;
  return *this;
}

void OdDbSubentRef::copyFrom(const OdRxObject* pFrom)
{
  OdDbSubentRefPtr pSrc = OdDbSubentRef::cast(pFrom);
  if (pSrc.get())
    OdDbSubentRef::operator =(*pSrc.get());
}

void OdDbSubentRef::reset()
{
  ODA_ASSERT_ONCE(m_pEntityId.get());
  *m_pEntityId.get() = *OdDbCompoundObjectId::createObject().get();
  m_SubentId = OdDbSubentId();
}

bool OdDbSubentRef::isValid() const
{
  ODA_FAIL_ONCE(); // TODO
  return !isEmpty();
}

bool OdDbSubentRef::isEmpty() const
{
  ODA_ASSERT_ONCE(m_pEntityId.get());
  return m_pEntityId->isEmpty(); 
}

const OdDbCompoundObjectId& OdDbSubentRef::entity() const
{
  ODA_ASSERT_ONCE(m_pEntityId.get());
  return *m_pEntityId.get();
}

OdDbSubentId OdDbSubentRef::subentId() const
{
  return m_SubentId;
}

OdDbEntity* OdDbSubentRef::createEntity() const
{
  return NULL;
}

OdResult OdDbSubentRef::evaluateAndCacheGeometry() 
{ 
  return eOk; 
}

void OdDbSubentRef::setEntity(const OdDbCompoundObjectId& entityId) 
{
  ODA_ASSERT_ONCE(m_pEntityId.get());
  *m_pEntityId.get() = entityId; 
}

void OdDbSubentRef::setSubent(const OdDbSubentId& subentId) 
{ 
  m_SubentId = subentId; 
}

void OdDbSubentRef::setFromSubentPath(const OdDbFullSubentPath&)
{
  ODA_FAIL_ONCE(); // TODO
}

//////////////////////////////////////////////////////////////////////////

//ODRX_CONS_DEFINE_MEMBERS_ALTNAME(OdDbFaceRef, OdDbSubentRef, L"AcDbFaceRef", RXIMPL_CONSTR)
ODRX_NO_CONS_DEFINE_MEMBERS_ALTNAME(OdDbFaceRef, OdDbSubentRef, L"AcDbFaceRef")

OdDbFaceRef::OdDbFaceRef()
  : m_pAsmBody(NULL)
{
}

OdDbFaceRef::OdDbFaceRef(const OdDbCompoundObjectId& entityId, 
                         const OdDbSubentId& subentId, // = OdDbSubentId()
                         class BODY* pAsmBody) // = NULL
  : m_pAsmBody(pAsmBody)
{
  setEntity(entityId);
  setSubent(subentId);
}

OdDbFaceRef::~OdDbFaceRef()
{
}

OdDbFaceRef::OdDbFaceRef(const OdDbFaceRef& from)
{
  OdDbFaceRef::operator =(from);
}

OdDbFaceRef& OdDbFaceRef::operator =(const OdDbFaceRef& from)
{
  if (this == &from)
    return *this;
  OdDbSubentRef::operator =(from);

  m_pAsmBody = from.m_pAsmBody;
  return *this;
}

void OdDbFaceRef::copyFrom(const OdRxObject* pFrom)
{
  OdDbFaceRefPtr pSrc = OdDbFaceRef::cast(pFrom);
  if (pSrc.get())
    OdDbFaceRef::operator =(*pSrc.get());
}

void OdDbFaceRef::reset()
{
  OdDbSubentRef::reset();
  m_pAsmBody = NULL;
}

bool OdDbFaceRef::isValid() const
{
  ODA_FAIL_ONCE(); // TODO
  return false;
}

bool OdDbFaceRef::isEmpty() const 
{ 
  return m_pAsmBody == NULL; 
}

class BODY* OdDbFaceRef::asmBody() const 
{ 
  return m_pAsmBody; 
}    

OdDbEntity* OdDbFaceRef::createEntity() const
{
  ODA_FAIL_ONCE(); // TODO
  return NULL;
}

OdResult OdDbFaceRef::evaluateAndCacheGeometry() 
{ 
  ODA_FAIL_ONCE(); // TODO
  return eNotImplementedYet;
}

//////////////////////////////////////////////////////////////////////////

ODRX_CONS_DEFINE_MEMBERS_ALTNAME(OdDbEdgeRef, OdDbSubentRef, L"AcDbEdgeRef", RXIMPL_CONSTR)

OdDbEdgeRef::OdDbEdgeRef()
{
}

OdDbEdgeRef::OdDbEdgeRef(const OdDbFullSubentPath& path)
{
  setFromSubentPath(path);
}

OdDbEdgeRef::OdDbEdgeRef(const OdDbCompoundObjectId& entityId, 
                         const OdDbSubentId& edgeSubentId, // = OdDbSubentId()
                         const OdDbSubentId& faceSubentId, // = OdDbSubentId()
                         const OdGeCurve3d* pCurve) // = NULL
  : m_FaceSubentId(faceSubentId)
{
  setEntity(entityId);
  setSubent(edgeSubentId);
  setCurve(pCurve);
}

OdDbEdgeRef::OdDbEdgeRef(const OdDbEntity* pEntity)
{
  ODA_FAIL_ONCE(); // TODO
}

OdDbEdgeRef::OdDbEdgeRef(const OdGeCurve3d* pCurve)
{
  setCurve(pCurve);
}

OdSmartPtr<OdDbEdgeRef> OdDbEdgeRef::createObject( const OdDbCompoundObjectIdPtr& entityId, const OdGeCurve3d* pCurve) 
{
  OdDbEdgeRefPtr res = OdDbEdgeRef::createObject();
  res->setEntity( *entityId.get() );
  res->setCurve( pCurve );
  return res;
}

OdSmartPtr<OdDbEdgeRef> OdDbEdgeRef::createObject( const OdGeCurve3d* pCurve )
{
  OdDbEdgeRefPtr res = OdDbEdgeRef::createObject();
  res->setCurve( pCurve );
  return res;
}

OdDbEdgeRef::~OdDbEdgeRef()
{
}

OdDbEdgeRef::OdDbEdgeRef(const OdDbEdgeRef& from)
{
  OdDbEdgeRef::operator =(from);
}

OdDbEdgeRef& OdDbEdgeRef::operator =(const OdDbEdgeRef& from)
{
  if (this == &from)
    return *this;
  OdDbSubentRef::operator =(from);

  m_FaceSubentId = from.m_FaceSubentId;
  setCurve(from.m_pCurve.get());
  return *this;
}

void OdDbEdgeRef::copyFrom(const OdRxObject* pFrom)
{
  OdDbEdgeRefPtr pSrc = OdDbEdgeRef::cast(pFrom);
  if (pSrc.get())
    OdDbEdgeRef::operator =(*pSrc.get());
}

void OdDbEdgeRef::reset()
{
  OdDbSubentRef::reset();
  m_FaceSubentId = OdDbSubentId();
  m_pCurve = NULL;
}

bool OdDbEdgeRef::isValid() const
{
  ODA_FAIL_ONCE(); // TODO
  return false;
}

bool OdDbEdgeRef::isEmpty() const
{ 
  return !m_pCurve.get(); 
}

void OdDbEdgeRef::setFaceSubentity(const OdDbSubentId& faceSubentId)
{
  m_FaceSubentId = faceSubentId;
}

OdDbSubentId OdDbEdgeRef::faceSubentId() const 
{ 
  return m_FaceSubentId; 
}

const OdGeCurve3d* OdDbEdgeRef::curve() const
{ 
  return m_pCurve.get(); 
} 

OdResult OdDbEdgeRef::evaluateCurve(OdGeCurve3d*&) const
{
  ODA_FAIL_ONCE(); // TODO
  return eNotImplementedYet;
}

OdDbEntity* OdDbEdgeRef::createEntity() const
{
  ODA_FAIL_ONCE(); // TODO
  return NULL;
}

OdResult OdDbEdgeRef::evaluateAndCacheGeometry()
{
  ODA_FAIL_ONCE(); // TODO
  return eNotImplementedYet;
}

void OdDbEdgeRef::setCurve(const OdGeCurve3d* pCurve)
{
  m_pCurve = NULL;
  if (pCurve)
    m_pCurve = static_cast<OdGeCurve3d*>(pCurve->copy());
}



//////////////////////////////////////////////////////////////////////////

ODRX_CONS_DEFINE_MEMBERS_ALTNAME(OdDbVertexRef, OdDbSubentRef, L"AcDbVertexRef", RXIMPL_CONSTR)

OdDbVertexRef::OdDbVertexRef()
  : m_ImpliedType(kExplicitVertex)
  , m_Index(0)
{
}

OdDbVertexRef::~OdDbVertexRef()
{
}

OdDbVertexRef::OdDbVertexRef(const OdDbCompoundObjectId& entityId,
                             const OdDbSubentId& subentId, // = OdDbSubentId()
                             const OdGePoint3d& point) // = OdGePoint3d::kOrigin
  : m_ImpliedType(kExplicitVertex)
  , m_Index(0)
  , m_Point(point)
{
  setEntity(entityId); 
  setSubent(subentId);
}

OdDbVertexRef::OdDbVertexRef(const OdDbFullSubentPath& path)
  : m_ImpliedType(kExplicitVertex)
  , m_Index(0)
{
  setFromSubentPath(path);
}

OdDbVertexRef::OdDbVertexRef(const OdDbEntity* pEntity)
  : m_ImpliedType(kExplicitVertex)
  , m_Index(0)
{
  ODA_ASSERT_ONCE(!pEntity); // TODO
}

OdDbVertexRef::OdDbVertexRef(const OdGePoint3d& point)
  : m_ImpliedType(kExplicitVertex)
  , m_Index(0)
  , m_Point(point)
{
}

OdDbVertexRef::OdDbVertexRef(OdDbVertexRef::ImpliedType impliedType, 
                             const OdDbEdgeRef& edgeRef, 
                             int controlOrFitPointIndex, // = 0
                             const OdGePoint3d & point) // = OdGePoint3d::kOrigin
  : m_ImpliedType(impliedType)
  , m_pReferencedRef(&edgeRef)
  , m_Index(controlOrFitPointIndex)
  , m_Point(point)
{
}

OdSmartPtr<OdDbVertexRef> OdDbVertexRef::createObject( const OdGePoint3d& point )
{
  OdDbVertexRefPtr pVertexRef = OdDbVertexRef::createObject();
  pVertexRef->m_Point = point;
  return pVertexRef;
}

OdSmartPtr<OdDbVertexRef> OdDbVertexRef::createObject( const OdDbObjectId entId, const OdDbSubentId subId, const OdGePoint3d& point )
{
  OdDbVertexRefPtr pVertexRef = OdDbVertexRef::createObject();
  pVertexRef->m_Point = point;
  OdDbCompoundObjectIdPtr pCompoundObjectId = OdDbCompoundObjectId::createObject();
  pCompoundObjectId->setEmpty();
  pCompoundObjectId->set( entId );
  pVertexRef->setEntity( *pCompoundObjectId );

  OdDbSubentRefPtr pSubEntRef = OdDbSubentRef::createObject( pCompoundObjectId, subId);
  pVertexRef->setReferencedRef( pSubEntRef );

  return pVertexRef;
}

OdDbVertexRef::OdDbVertexRef(const OdDbVertexRef& from)
{
  OdDbVertexRef::operator =(from);
}

OdDbVertexRef& OdDbVertexRef::operator =(const OdDbVertexRef& from)
{
  if (this == &from)
    return *this;
  OdDbSubentRef::operator =(from);

  m_ImpliedType = from.m_ImpliedType;
  m_pReferencedRef = from.m_pReferencedRef;
  m_Index = from.m_Index;
  m_Point = from.m_Point; 
  return *this;
}

void OdDbVertexRef::copyFrom(const OdRxObject* pFrom)
{
  OdDbVertexRefPtr pSrc = OdDbVertexRef::cast(pFrom);
  if (pSrc.get())
    OdDbVertexRef::operator =(*pSrc.get());
}

void OdDbVertexRef::reset()
{
  OdDbSubentRef::reset();
  m_ImpliedType = kExplicitVertex;
  m_pReferencedRef = NULL;
  m_Index = 0;
  m_Point = OdGePoint3d(); 
}

bool OdDbVertexRef::isValid() const
{
  ODA_FAIL_ONCE(); // TODO
  return false;
}

const OdDbCompoundObjectId& OdDbVertexRef::entity() const
{
  return OdDbSubentRef::entity();
}

OdDbSubentId OdDbVertexRef::subentId() const
{
  return OdDbSubentRef::subentId();
}

OdDbVertexRef::ImpliedType OdDbVertexRef::impliedType() const 
{ 
  return m_ImpliedType;
}

const OdDbGeomRef* OdDbVertexRef::referencedRef() const
{ 
  return m_pReferencedRef;
}

int OdDbVertexRef::index() const 
{ 
  return m_Index;
}

OdGePoint3d OdDbVertexRef::point() const
{
  return m_Point; 
}

OdResult OdDbVertexRef::evaluatePoint(OdGePoint3d&) const
{
  ODA_FAIL_ONCE(); // TODO
  return eNotImplementedYet;
}

class OdDbEntity* OdDbVertexRef::createEntity() const
{
  ODA_FAIL_ONCE(); // TODO
  return NULL;
}

OdResult OdDbVertexRef::evaluateAndCacheGeometry()
{
  ODA_FAIL_ONCE(); // TODO
  return eNotImplementedYet;
}

void OdDbVertexRef::setReferencedRef(OdDbGeomRefPtr pReferencedRef)
{
  m_pReferencedRef = pReferencedRef;
}
//////////////////////////////////////////////////////////////////////////

ODRX_CONS_DEFINE_MEMBERS_ALTNAME(OdDbPathRef, OdDbGeomRef, L"AcDbPathRef", RXIMPL_CONSTR)

OdDbPathRef::OdDbPathRef()
{
}

OdDbPathRef::OdDbPathRef(const OdArray<OdDbEdgeRef>& edgeRefs)
{
  setEdgeRefs(edgeRefs);
}

OdDbPathRef::OdDbPathRef(const OdDbFullSubentPathArray& edgeSubentPathArr,
                         const OdDbFullSubentPathArray& faceSubentPathArr)
{
  ODA_FAIL_ONCE(); // TODO
  throw OdError(eNotImplementedYet);
}

OdDbPathRef::~OdDbPathRef()
{
}

OdDbPathRef::OdDbPathRef(const OdDbPathRef& from)
{
  OdDbPathRef::operator =(from);
}

OdDbPathRef& OdDbPathRef::operator =(const OdDbPathRef& from)
{
  if (this == &from)
    return *this;
  //OdDbGeomRef::operator =(from);

  m_EdgeRefs = from.m_EdgeRefs;
  return *this;
}

void OdDbPathRef::copyFrom(const OdRxObject* pFrom)
{
  OdDbPathRefPtr pSrc = OdDbPathRef::cast(pFrom);
  if (pSrc.get())
    OdDbPathRef::operator =(*pSrc.get());
}

void OdDbPathRef::reset()
{
  m_EdgeRefs.clear();
}

bool OdDbPathRef::isValid() const
{
  ODA_FAIL_ONCE(); // TODO
  return false;
}

bool OdDbPathRef::isEmpty() const
{
  ODA_FAIL_ONCE(); // TODO
  return false;
}

void OdDbPathRef::setEdgeRefs(const OdArray<OdDbEdgeRef>& edges)
{
  m_EdgeRefs = edges;
}

OdResult OdDbPathRef::getEntityArray(OdArray<OdDbEntity*>& entities, 
                                     bool concatenate) const
{
  ODA_FAIL_ONCE(); // TODO
  return eNotImplementedYet;
}

const OdArray<OdDbEdgeRef>& OdDbPathRef::edgeRefs() const 
{ 
  return m_EdgeRefs; 
}

bool OdDbPathRef::isEqualTo(const OdGeCurve3d*)
{
  ODA_FAIL_ONCE(); // TODO
  return false;
}

bool OdDbPathRef::isReferencePath()
{
  ODA_FAIL_ONCE(); // TODO
  return false;
}

OdDbEntity* OdDbPathRef::createEntity() const
{
  ODA_FAIL_ONCE(); // TODO
  return NULL;
}

OdResult OdDbPathRef::evaluateAndCacheGeometry()
{
  ODA_FAIL_ONCE(); // TODO
  return eNotImplementedYet;
}

//////////////////////////////////////////////////////////////////////////

ODRX_CONS_DEFINE_MEMBERS_ALTNAME(OdDbVectorRef, OdDbGeomRef, L"AcDbVectorRef", RXIMPL_CONSTR)

OdDbVectorRef::OdDbVectorRef()
{
}

OdDbVectorRef::OdDbVectorRef(const OdGeVector3d& vector)
  : m_Vector(vector)
{
}

OdDbVectorRef::~OdDbVectorRef()
{
}

OdDbVectorRef::OdDbVectorRef(const OdDbVectorRef& from)
{
  OdDbVectorRef::operator =(from);
}

OdDbVectorRef& OdDbVectorRef::operator =(const OdDbVectorRef& from)
{
  if (this == &from)
    return *this;
  //OdDbGeomRef::operator =(from);

  m_Vector = from.m_Vector;
  return *this;
}

void OdDbVectorRef::copyFrom(const OdRxObject* pFrom)
{
  OdDbVectorRefPtr pSrc = OdDbVectorRef::cast(pFrom);
  if (pSrc.get())
    OdDbVectorRef::operator =(*pSrc.get());
}

void OdDbVectorRef::reset()
{
}

bool OdDbVectorRef::isValid() const
{
  ODA_FAIL_ONCE(); // TODO
  return false;
}

bool OdDbVectorRef::isEmpty() const
{
  ODA_FAIL_ONCE(); // TODO
  return false;
}

const OdGeVector3d OdDbVectorRef::vector() const
{ 
  return m_Vector; 
}

void OdDbVectorRef::set(const OdGeVector3d& vect)
{
  m_Vector = vect;
}

OdDbEntity* OdDbVectorRef::createEntity() const
{
  ODA_FAIL_ONCE(); // TODO
  return NULL;
}

OdResult OdDbVectorRef::evaluateAndCacheGeometry()
{
  ODA_FAIL_ONCE(); // TODO
  return eNotImplementedYet;
}

//////////////////////////////////////////////////////////////////////////

ODRX_CONS_DEFINE_MEMBERS_ALTNAME(OdDbCoordSystemRef, OdDbGeomRef, L"AcDbCoordSystemRef", RXIMPL_CONSTR)

OdDbCoordSystemRef::OdDbCoordSystemRef()
  : m_pEntityId(OdDbCompoundObjectId::createObject())
{
}

OdDbCoordSystemRef::OdDbCoordSystemRef(const OdGeMatrix3d& matrix)
  : m_CoordSystem(matrix)
  , m_pEntityId(OdDbCompoundObjectId::createObject())
{
}

OdDbCoordSystemRef::OdDbCoordSystemRef(const OdDbCompoundObjectId& entityId, const OdGeMatrix3d& matrix)
  : m_CoordSystem(matrix)
  , m_pEntityId(OdDbCompoundObjectId::createObject())
{
  *m_pEntityId.get() = entityId;
}

OdDbCoordSystemRef::OdDbCoordSystemRef(const OdArray<OdDbSubentRef*>& SubentRefs)
  : m_pEntityId(OdDbCompoundObjectId::createObject())
  , m_SubentRefs(SubentRefs)
{
}

OdDbCoordSystemRef::~OdDbCoordSystemRef()
{
}

OdDbCoordSystemRef::OdDbCoordSystemRef(const OdDbCoordSystemRef& from)
{
  OdDbCoordSystemRef::operator =(from);
}

OdDbCoordSystemRef& OdDbCoordSystemRef::operator =(const OdDbCoordSystemRef& from)
{
  if (this == &from)
    return *this;
  //OdDbGeomRef::operator =(from);

  m_CoordSystem = from.m_CoordSystem;
  ODA_ASSERT_ONCE(m_pEntityId.get() && from.m_pEntityId.get());
  *m_pEntityId.get() = *from.m_pEntityId.get();
  m_SubentRefs = from.m_SubentRefs;
  return *this;
}

void OdDbCoordSystemRef::copyFrom(const OdRxObject* pFrom)
{
  OdDbCoordSystemRefPtr pSrc = OdDbCoordSystemRef::cast(pFrom);
  if (pSrc.get())
    OdDbCoordSystemRef::operator =(*pSrc.get());
}

void OdDbCoordSystemRef::reset()
{
  m_CoordSystem = OdGeMatrix3d::kIdentity;
  ODA_ASSERT_ONCE(m_pEntityId.get());
  *m_pEntityId.get() = *OdDbCompoundObjectId::createObject().get();
  m_SubentRefs.clear();
}
  
bool OdDbCoordSystemRef::isValid() const
{
  ODA_FAIL_ONCE(); // TODO
  return false;
}

bool OdDbCoordSystemRef::isEmpty() const
{
  ODA_FAIL_ONCE(); // TODO
  return false;
}

const OdGeMatrix3d& OdDbCoordSystemRef::coordSystem() const
{ 
  return m_CoordSystem; 
}

const OdDbCompoundObjectId& OdDbCoordSystemRef::entityId() const
{
  ODA_ASSERT_ONCE(m_pEntityId.get());
  return *m_pEntityId.get();    
}

const OdArray<OdDbSubentRef*>& OdDbCoordSystemRef::subentRefs() const 
{ 
  return m_SubentRefs;  
}

void OdDbCoordSystemRef::set(const OdGeMatrix3d& matrix)
{
  m_CoordSystem = matrix; 
}

void OdDbCoordSystemRef::set(const OdDbCompoundObjectId& entityId)
{
  *m_pEntityId.get() = entityId;
}
  
void OdDbCoordSystemRef::set(const OdArray<OdDbSubentRef*>& subentRefs)
{
  m_SubentRefs = subentRefs;  
}

OdDbEntity* OdDbCoordSystemRef::createEntity() const
{ 
  ODA_FAIL_ONCE(); // TODO
  return NULL;
}

OdResult OdDbCoordSystemRef::evaluateAndCacheGeometry()
{
  ODA_FAIL_ONCE(); // TODO
  return eNotImplementedYet;
}
