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

#include "OdConstrainedGeometry.h"
#include "OdDistanceConstraintImpl.h"
#include "DbFiler.h"

#include "DbDimension.h"
#include "DbAlignedDimension.h"
#include "DbRotatedDimension.h"

OdDistanceConstraintImpl::OdDistanceConstraintImpl():OdExplicitConstraintImpl(),
                                                    m_dirType(OdDistanceConstraint::kNotDirected),
                                                    m_lineId(OdConstraintGroupNode::kNullGroupNodeId)
{
  m_dirVect.set(0, 0, 0);
}

OdDistanceConstraintImpl::OdDistanceConstraintImpl(const OdGeVector3d& direction):OdExplicitConstraintImpl(),
                                                    m_dirType(OdDistanceConstraint::kFixedDirection),
                                                    m_lineId(OdConstraintGroupNode::kNullGroupNodeId)
{
  m_dirVect.set(direction.x, direction.y, direction.z);
}

OdDistanceConstraintImpl::OdDistanceConstraintImpl(const OdConstraintGroupNodeId consLineId,
                                                   OdDistanceConstraint::DirectionType type):OdExplicitConstraintImpl(),
                                                    m_dirType(type),
                                                    m_lineId(consLineId)
{
  m_dirVect.set(0, 0, 0);
}

OdResult OdDistanceConstraintImpl::setDirection(const OdGeVector3d& direction)
{
  m_dirVect.set(direction.x, direction.y, direction.z);
  m_dirType = OdDistanceConstraint::kFixedDirection;
  return eOk;
}

OdResult OdDistanceConstraintImpl::setDirection(const OdConstraintGroupNodeId consLineId,
                                                OdDistanceConstraint::DirectionType type)
{
  m_lineId = consLineId;
  m_dirType = type;
  return eOk;
}

OdDistanceConstraintImpl::~OdDistanceConstraintImpl()
{
}

OdDistanceConstraint::DirectionType OdDistanceConstraintImpl::directionType() const
{
  return m_dirType;
}

OdGeVector3d OdDistanceConstraintImpl::direction() const
{
  return m_dirVect;
}

OdConstraintGroupNodeId OdDistanceConstraintImpl::constrainedLineId() const
{
  return m_lineId;
}

OdResult OdDistanceConstraintImpl::dwgInFields(OdDbDwgFiler* pFiler)
{
  OdResult res = OdExplicitConstraintImpl::dwgInFields(pFiler);
  if ( res != eOk )
    return res;
   
  m_dirType = static_cast<OdDistanceConstraint::DirectionType>(pFiler->rdUInt8());
  switch ( m_dirType )
  {
    case OdDistanceConstraint::kNotDirected:

    break;
    case OdDistanceConstraint::kFixedDirection:
      m_dirVect = pFiler->rdVector3d();
    break;
    case OdDistanceConstraint::kPerpendicularToLine:
      ODA_FAIL();
    break;
    case OdDistanceConstraint::kParallelToLine:
      ODA_FAIL();
    break;
  }
  return res;
}

void OdDistanceConstraintImpl::dwgOutFields(OdDbDwgFiler* pFiler) const
{
  OdExplicitConstraintImpl::dwgOutFields(pFiler);
  pFiler->wrUInt8((OdUInt8)m_dirType);

  switch ( m_dirType )
  {
    case OdDistanceConstraint::kNotDirected:

    break;
    case OdDistanceConstraint::kFixedDirection:
      pFiler->wrVector3d(m_dirVect);
    break;
    case OdDistanceConstraint::kPerpendicularToLine:
      ODA_FAIL();
    break;
    case OdDistanceConstraint::kParallelToLine:
      ODA_FAIL();
    break;
  }
}

#define NEXT_CODE(code) \
if (pFiler->nextItem() != code) \
{                               \
  ODA_FAIL();                   \
  return eBadDxfSequence;       \
}

OdResult OdDistanceConstraintImpl::dxfInFields(OdDbDxfFiler* pFiler)
{
  OdResult res = OdExplicitConstraintImpl::dxfInFields(pFiler);
  if ( res != eOk )
    return res;

  NEXT_CODE(280)
  m_dirType = static_cast<OdDistanceConstraint::DirectionType>(pFiler->rdUInt8());
  switch ( m_dirType )
  {
    case OdDistanceConstraint::kNotDirected:

    break;
    case OdDistanceConstraint::kFixedDirection:
      NEXT_CODE(10)
      pFiler->rdVector3d(m_dirVect);
    break;
    case OdDistanceConstraint::kPerpendicularToLine:
      ODA_FAIL();
    break;
    case OdDistanceConstraint::kParallelToLine:
      ODA_FAIL();
    break;
  }
  return res;
}

#undef NEXT_CODE

void OdDistanceConstraintImpl::dxfOutFields(OdDbDxfFiler* pFiler) const
{
  OdExplicitConstraintImpl::dxfOutFields(pFiler);
  pFiler->wrUInt8(280, (OdUInt8)m_dirType);

  switch ( m_dirType )
  {
    case OdDistanceConstraint::kNotDirected:

    break;
    case OdDistanceConstraint::kFixedDirection:
      pFiler->wrVector3d(10, m_dirVect);
    break;
    case OdDistanceConstraint::kPerpendicularToLine:
      ODA_FAIL();
    break;
    case OdDistanceConstraint::kParallelToLine:
      ODA_FAIL();
    break;
  }
}

OdGePoint3d getPoint(OdConstrainedGeometry * pGeom)
{
  if ( pGeom->isKindOf(OdConstrainedPoint::desc()) )
    return (OdConstrainedPoint::cast(pGeom))->point();
  else if ( pGeom->isKindOf(OdConstrainedBoundedLine::desc()) )
  {
    OdConstrainedBoundedLine *pLine = OdConstrainedBoundedLine::cast(pGeom);

    return pLine->midPoint();
  }
  else
    return OdGePoint3d::kOrigin;
}

void OdDistanceConstraintImpl::updateDimDependency(const OdGeMatrix3d& matrToWrd)
{
  OdDbObjectId id = dimDependencyId();
  if ( id.isNull() )
    return;

  OdArray<OdConstrainedGeometry*> arrGeom;
  getConnectedGeometries(arrGeom);
  
  ODA_ASSERT(arrGeom.length() == 2);

  OdDbAssocDependencyPtr assocDepPtr = id.openObject();
  id = assocDepPtr->dependentOnObject();
  if ( id.isNull() )
    return;

  OdDbDimensionPtr dimPtr = id.openObject(OdDb::kForWrite);
  //OdSmartPtr<OdConstrainedPoint> implPt;

  if ( dimPtr->isKindOf(OdDbAlignedDimension::desc()) )
  {
    OdDbAlignedDimensionPtr pRotDim = OdDbAlignedDimension::cast(dimPtr);
   
    OdGePoint3d dimLnPt = pRotDim->dimLinePoint();
    OdGePoint3d dimXLn1Pt = pRotDim->xLine1Point();
    OdGePoint3d dimXLn2Pt = pRotDim->xLine2Point();

    OdGeVector3d xLn1LnVec = dimLnPt - dimXLn1Pt;

    OdGeVector3d txtPosLnVec = pRotDim->textPosition() - dimLnPt;

    //implPt = OdConstrainedPoint::cast(arrGeom[0]);
    dimXLn1Pt = getPoint(arrGeom[0]);//implPt->point();

    //implPt = OdConstrainedPoint::cast(arrGeom[1]);
    dimXLn2Pt = getPoint(arrGeom[1]);//implPt->point();

    dimXLn1Pt.transformBy(matrToWrd);
    dimXLn2Pt.transformBy(matrToWrd);

    dimLnPt = dimXLn1Pt + xLn1LnVec;

    OdGePoint3d txtPosPt = dimLnPt + txtPosLnVec;

    pRotDim->setDimLinePoint(dimLnPt);
    pRotDim->setXLine1Point(dimXLn1Pt);
    pRotDim->setXLine2Point(dimXLn2Pt);
    pRotDim->setTextPosition(txtPosPt);
  }
  else if ( dimPtr->isKindOf(OdDbRotatedDimension::desc()) )
  {
    OdDbRotatedDimensionPtr pRotDim = OdDbRotatedDimension::cast(dimPtr);

    OdGePoint3d dimLnPt = pRotDim->dimLinePoint();
    OdGePoint3d dimXLn1Pt = pRotDim->xLine1Point();
    OdGePoint3d dimXLn2Pt = pRotDim->xLine2Point();

    OdGePoint3d txtPosPt = pRotDim->textPosition();

    OdGeVector3d xLn1LnVec = dimLnPt - dimXLn1Pt;
    OdGeVector3d txtPosLnVec = txtPosPt - dimLnPt;


    //find dir
    OdGeVector3d dir;
    switch ( m_dirType )
    {
      case OdDistanceConstraint::kNotDirected:
        if ( arrGeom[0]->isKindOf(OdConstrainedPoint::desc()) )
        {
          if ( arrGeom[1]->isKindOf(OdConstrainedPoint::desc()) )
          {
            dir = (OdConstrainedPoint::cast(arrGeom[1]))->point() -
                      (OdConstrainedPoint::cast(arrGeom[0]))->point();
          }
          else
          {
            OdConstrainedBoundedLine *pL1 = OdConstrainedBoundedLine::cast(arrGeom[1]);

            OdGeVector3d dir1 = (OdConstrainedPoint::cast(arrGeom[0]))->point() - pL1->startPoint();
            OdGeVector3d dir2 = pL1->endPoint() - pL1->startPoint();

            double val = (dir1.crossProduct(dir2)).z;
		        if ( fabs(val) <= 1.0e-4 )
			        val = 0.0;

            if ( val < 0 )
            {
              dir.x = -pL1->direction().y;
              dir.y = pL1->direction().x;
            }
            else
            {
              dir.x = pL1->direction().y;
              dir.y = -pL1->direction().x;
            }
          }
        }
        else if ( arrGeom[0]->isKindOf(OdConstrainedBoundedLine::desc()) )
        {
          OdConstrainedBoundedLine *pL0 = OdConstrainedBoundedLine::cast(arrGeom[0]);
          OdGeVector3d dir1;
          if ( arrGeom[1]->isKindOf(OdConstrainedBoundedLine::desc()) )
            dir1 = (OdConstrainedBoundedLine::cast(arrGeom[1]))->startPoint() - pL0->startPoint();
          else
            dir1 = (OdConstrainedPoint::cast(arrGeom[1]))->point() - pL0->startPoint();
          OdGeVector3d dir2 = pL0->endPoint() - pL0->startPoint();
          
          double val = (dir1.crossProduct(dir2)).z;
		      if ( fabs(val) <= 1.0e-4 )
			      val = 0.0;

          if ( val >= 0 )
          {
            dir.x = -pL0->direction().y;
            dir.y = pL0->direction().x;
          }
          else
          {
            dir.x = pL0->direction().y;
            dir.y = -pL0->direction().x;
          }
        }
      break;
      case OdDistanceConstraint::kFixedDirection:
        dir = m_dirVect;
      break;
      case OdDistanceConstraint::kPerpendicularToLine:
        ODA_FAIL();
      break;
      case OdDistanceConstraint::kParallelToLine:
        ODA_FAIL();
      break;
    }

    //implPt = OdConstrainedPoint::cast(arrGeom[0]);
    dimXLn1Pt = getPoint(arrGeom[0]);//implPt->point();

    //implPt = OdConstrainedPoint::cast(arrGeom[1]);
    dimXLn2Pt = getPoint(arrGeom[1]);//implPt->point();

    dimXLn1Pt.transformBy(matrToWrd);
    dimXLn2Pt.transformBy(matrToWrd);

    dimLnPt = dimXLn1Pt + xLn1LnVec;
    txtPosPt = dimLnPt + txtPosLnVec;

    pRotDim->setDimLinePoint(dimLnPt);
    pRotDim->setXLine1Point(dimXLn1Pt);
    pRotDim->setXLine2Point(dimXLn2Pt);
    pRotDim->setTextPosition(txtPosPt);
    
    dir.transformBy(matrToWrd);
    OdGePlane ocsPlane;
    OdDb::Planarity flag;
    pRotDim->getPlane(ocsPlane, flag);
    double ang = dir.angleOnPlane(ocsPlane);
    pRotDim->setRotation(ang);
  }
  else
    ODA_FAIL_ONCE();

  fillCurrentStateObj();
}

void OdDistanceConstraintImpl::fillCurrentStateObj()
{
  OdDbObjectId id = dimDependencyId();
  if ( id.isNull() )
    return;

  OdArray<OdConstrainedGeometry*> arrGeom;
  getConnectedGeometries(arrGeom);
  
  ODA_ASSERT(arrGeom.length() == 2);
  if (arrGeom.length() != 2)
    return;

  if ( arrGeom[0]->isKindOf(OdConstrainedPoint::desc()) )
    m_oldPointF = (OdConstrainedPoint::cast(arrGeom[0]))->point();
  else if ( arrGeom[0]->isKindOf(OdConstrainedBoundedLine::desc()) )
  {
    OdConstrainedBoundedLine *pLine = OdConstrainedBoundedLine::cast(arrGeom[0]);

    m_oldLineF.set(pLine->startPoint(), pLine->endPoint());
  }

  if ( arrGeom[1]->isKindOf(OdConstrainedPoint::desc()) )
    m_oldPointF = (OdConstrainedPoint::cast(arrGeom[1]))->point();
  else if ( arrGeom[1]->isKindOf(OdConstrainedBoundedLine::desc()) )
  {
    OdConstrainedBoundedLine *pLine = OdConstrainedBoundedLine::cast(arrGeom[1]);

    m_oldLineF.set(pLine->startPoint(), pLine->endPoint());
  }
}
