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

#include "DbAssocGeomDependency.h"
#include "DbAssoc2dConstraintGroup.h"
#include "OdConstrainedRigidSetImpl.h"
#include "DbBlockReference.h"
#include "DbFiler.h"

OdConstrainedRigidSetImpl::OdConstrainedRigidSetImpl():OdConstrainedGeometryImpl(),
                                               m_matrix(),
                                               m_connections(),m_unk_Bool(0)
{
}

int OdConstrainedRigidSetImpl::numOfConstrainedGeoms() const
{
  return m_connections.length();
}

OdConstrainedGeometry* OdConstrainedRigidSetImpl::getConstrainedGeomAt(int index) const
{
  OdDbAssoc2dConstraintGroupPtr pGr = OdDbAssoc2dConstraintGroup::cast(owningConstraintGroupId().openObject());
  OdConstraintGroupNode *pGrNode = pGr->getGroupNodePtr(m_connections[index]);
  
  OdSmartPtr<OdConstrainedGeometry> pRigidSet = OdConstrainedGeometry::cast(pGrNode);
  return pRigidSet;
}

OdGeMatrix3d OdConstrainedRigidSetImpl::transform() const
{
  return m_matrix;
}

void OdConstrainedRigidSetImpl::dwgOutFields(OdDbDwgFiler* pFiler) const
{
  OdConstrainedGeometryImpl::dwgOutFields(pFiler);

  pFiler->wrBool(m_unk_Bool);

  int i, j;
  for ( i = 0; i < 4; i++ )
  {
    for ( j = 0; j < 4; j++ )
    {
      pFiler->wrDouble(m_matrix.entry[i][j]);
    }
  }

  int logicalLen;
  pFiler->wrInt32((logicalLen = m_connections.length()));

  const OdConstraintGroupNodeId* pConnections = m_connections.asArrayPtr();
  while (logicalLen--)
  {
    pFiler->wrInt32(*pConnections++);
  }
}

OdResult OdConstrainedRigidSetImpl::dwgInFields(OdDbDwgFiler* pFiler)
{
  OdResult res = OdConstrainedGeometryImpl::dwgInFields(pFiler);
  if ( res != eOk )
    return res;

  m_unk_Bool = pFiler->rdBool();
  ODA_ASSERT(m_unk_Bool == false);

  int i, j;
  for ( i = 0; i < 4; i++ )
  {
    for ( j = 0; j < 4; j++ )
    {
      m_matrix.entry[i][j] = pFiler->rdDouble();
    }
  }

  int logicalLen = pFiler->rdInt32();
  m_connections.resize(logicalLen);

  OdConstraintGroupNodeId* pConnections = m_connections.asArrayPtr();
  while (logicalLen--)
  {
    *pConnections++ = pFiler->rdInt32();
  }

  return res;
}

void OdConstrainedRigidSetImpl::dxfOutFields(OdDbDxfFiler* pFiler) const
{
  OdConstrainedGeometryImpl::dxfOutFields(pFiler);
  pFiler->wrBool(290, m_unk_Bool);

  int i, j;
  for ( i = 0; i < 4; i++ )
  {
    for ( j = 0; j < 4; j++ )
    {
      pFiler->wrDouble(40, m_matrix.entry[i][j]);
    }
  }

  int logicalLen;
  pFiler->wrInt32(90, (logicalLen = m_connections.length()));

  const OdConstraintGroupNodeId* pConnections = m_connections.asArrayPtr();
  while (logicalLen--)
  {
    pFiler->wrInt32(90, *pConnections++);
  }
}

#define NEXT_CODE(code) \
if (pFiler->nextItem() != code) \
{                               \
  ODA_FAIL();                   \
  return eBadDxfSequence;       \
}

OdResult OdConstrainedRigidSetImpl::dxfInFields(OdDbDxfFiler* pFiler)
{
  OdResult res = OdConstrainedGeometryImpl::dxfInFields(pFiler);
  if ( res != eOk )
    return res;

  NEXT_CODE(290)
  m_unk_Bool = pFiler->rdBool();
  ODA_ASSERT(m_unk_Bool == false);

  int i, j;
  for ( i = 0; i < 4; i++ )
  {
    for ( j = 0; j < 4; j++ )
    {
      NEXT_CODE(40)
      m_matrix.entry[i][j] = pFiler->rdDouble();
    }
  }

  NEXT_CODE(90)
  int logicalLen = pFiler->rdInt32();
  m_connections.resize(logicalLen);

  OdConstraintGroupNodeId* pConnections = m_connections.asArrayPtr();
  while (logicalLen--)
  {
    NEXT_CODE(90)
    *pConnections++ = pFiler->rdInt32();
  }

  return res;
}

#undef NEXT_CODE

void OdConstrainedRigidSetImpl::setTransform(const OdGeMatrix3d &val)
{
  for ( int i = 0; i < 4; i++ )
    for ( int j = 0; j < 4; j++ )
      m_matrix.entry[i][j] = val.entry[i][j];
}

OdResult OdConstrainedRigidSetImpl::setDataFromGeomDependency(const OdDbObjectId geomDependencyId,
                                             const OdGeMatrix3d& matrFromWrd)
{
  OdDbAssocGeomDependencyPtr pGeom = geomDependency(geomDependencyId);
  OdDbObjectPtr pObj = pGeom->dependentOnObject().openObject();
  if ( pObj->isKindOf(OdDbBlockReference::desc()) )
  {
    OdDbBlockReferencePtr pBlockRef = OdDbBlockReference::cast(pObj);
    setTransform(pBlockRef->blockTransform());
    return eOk;
  }
  else
  {
    ODA_FAIL();
  }

  return eNotImplementedYet;
}

OdResult OdConstrainedRigidSetImpl::updateDbObject(OdDbObject *pObj, OdDbAssocGeomDependency* pDep, const OdGeMatrix3d& matrToWrd)
{
  if ( pObj->isKindOf(OdDbBlockReference::desc()) )
  {
    OdDbBlockReferencePtr pBlockRef = OdDbBlockReference::cast(pObj);
    pBlockRef->setBlockTransform(transform());
    return eOk;
  }
  else
  {
    ODA_FAIL();
  }

  return eNotImplementedYet;
}
