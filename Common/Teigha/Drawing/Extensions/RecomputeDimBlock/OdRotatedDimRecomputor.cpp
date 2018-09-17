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
#include "OdRotatedDimRecomputor.h"
#include "DbRotatedDimension.h"
#include "DbDimStyleTableRecord.h"
#include "RxObjectImpl.h"
#include "DbObjectContextData.h"

#include "Ge/GeMatrix3d.h"
#include "Ge/GeLine2d.h"
#include "Ge/GeVector2d.h"

OdDimRecomputorPtr OdDbRotatedDimRecomputePEImpl::createRecomputor()
{
  return OdRxObjectImpl<OdRotatedDimRecomputor>::createObject();
}

OdRotatedDimRecomputor::OdRotatedDimRecomputor() : OdAlignedDimRecomputor()
, m_dRotAngle(0.0)
{
}

OdRotatedDimRecomputor::~OdRotatedDimRecomputor()
{
}

OdUInt8 OdRotatedDimRecomputor::getDimTypeFlag()
{
  return DimRotated;
}

void OdRotatedDimRecomputor::getDimParams(const OdDbDimension* pDim, OdDbDimensionObjectContextData* ctx)
{
  OdDimRecomputor::getDimParams(pDim, ctx);
  OdDbRotatedDimensionPtr pDimPtr(pDim);
  m_dRotAngle = pDimPtr->rotation();
  m_DefPoint1 = pDimPtr->xLine1Point();
  m_DefPoint2 = pDimPtr->xLine2Point();
  m_DimLineDefPt = ctx ? ((OdDbAlignedDimensionObjectContextData*)ctx)->dimLinePoint() : pDimPtr->dimLinePoint();
  m_dOblique = pDimPtr->oblique();
  if ( ::fabs(m_dOblique) > 1.0E+10 )
  {
    m_dOblique = 0.0;
  }
  m_bJogSymbolOn = pDimPtr->jogSymbolOn();
  m_pJogPosition = pDimPtr->jogSymbolPosition(); 
  m_JogSymbolHeight = pDimPtr->jogSymbolHeight();
}

void OdRotatedDimRecomputor::setDimParams(OdDbDimension* pDim, OdDbDimensionObjectContextData* ctx) const
{
  OdDimRecomputor::setDimParams(pDim, ctx);

  OdDbRotatedDimensionPtr pDimPtr(pDim);
  if (!ctx || ctx->isDefaultContextData())
    pDimPtr->setDimLinePoint(m_DimLineDefPt);
  if (ctx)
    ((OdDbAlignedDimensionObjectContextData*)ctx)->setDimLinePoint(m_DimLineDefPt);

  if ( m_bJogSymbolOn )
    pDimPtr->setJogSymbolPosition(m_pJogPosition); 
  /*
  // pDimPtr->setRotation(m_dRotAngle);
  pDimPtr->setXLine1Point(m_DefPoint1);
  pDimPtr->setXLine2Point(m_DefPoint2);
  // pDimPtr->setOblique(m_dOblique);
  */
}

void OdRotatedDimRecomputor::preprocessDimPoints(OdDbDimStyleTableRecord* pDimVars)
{
  OdGeMatrix3d world2Plane(OdGeMatrix3d::worldToPlane(normal()));
  OdGePoint3d ocsDefPt1(m_DefPoint1), ocsDefPt2(m_DefPoint2), ocsDimLDPt(m_DimLineDefPt);
  OdGePoint3d textPoint(m_ocsDimTextMidPt);
  textPoint.z = 0.0;
  ocsDefPt1.transformBy(world2Plane);
  ocsDefPt2.transformBy(world2Plane);
  ocsDimLDPt.transformBy(world2Plane);
  double savedZCoord = ocsDimLDPt.z;

  ocsDefPt1.z = ocsDefPt2.z = ocsDimLDPt.z = 0.0; // cr2793

  OdGeVector3d perp(OdGeVector3d::kYAxis);
  if(OdZero(m_dOblique))
  {
    perp.rotateBy(m_dRotAngle, OdGeVector3d::kZAxis);
  }
  else
  {
    perp.rotateBy(m_dRotAngle-(OdaPI2-m_dOblique), OdGeVector3d::kZAxis);
  }
  OdGeVector3d parall(OdGeVector3d::kXAxis);
  parall.rotateBy(m_dRotAngle, OdGeVector3d::kZAxis);
  OdGeLine3d perpLine(ocsDefPt2, perp);
  // m_bUserDefinedTextPosition && (pDimVars->dimtmove() == 0)) // CR2369
  OdGeLine3d dimLine(ocsDimLDPt, parall); 
  dimLine.intersectWith(perpLine, m_DimLineDefPt);
  m_DimLineDefPt.z = savedZCoord;
  m_DimLineDefPt.transformBy(OdGeMatrix3d::planeToWorld(normal()));
}

void OdRotatedDimRecomputor::updateMeasurement(OdDbDimStyleTableRecord* /*pDimVars*/)
{
  OdGeMatrix3d mWorld2Plane(OdGeMatrix3d::worldToPlane(m_vNormal));

  OdGePoint3d defPoint1 = m_DefPoint1;
  OdGePoint3d defPoint2 = m_DefPoint2;

  defPoint1.transformBy(mWorld2Plane);
  defPoint2.transformBy(mWorld2Plane);

  defPoint1.z = defPoint2.z = 0.0;

  OdGeVector3d vDef(-sin(m_dRotAngle), cos(m_dRotAngle), 0.0);
  OdGeVector3d vDef1(defPoint1 - defPoint2);
#if defined(_MSC_VER) && (_MSC_VER < 1400)
  // #14654
  m_dMeasurement = sqrt(vDef.crossProduct(vDef1).lengthSqrd());
#else
  m_dMeasurement = vDef.crossProduct(vDef1).length();
#endif
  m_dMeasurement *= m_dimlfac;
}

void OdRotatedDimRecomputor::calculateExtLinePoints(OdGePoint3d& extLine1Start, OdGePoint3d& extLine1End,
                                                    OdGePoint3d& extLine2Start, OdGePoint3d& extLine2End,
                                                    const OdGePoint3d& dimLineDefPt, const OdGePoint3d& defPoint1,
                                                    const OdGePoint3d& defPoint2)
{
  extLine1Start = defPoint1;
  extLine2Start = defPoint2;
  extLine2End   = dimLineDefPt;
  OdGeVector3d first  = defPoint2 - dimLineDefPt;

  if(OdNonZero(first.length()))
  {
    if(OdZero(m_dOblique))
    {
      first.rotateBy(OdaPI2, OdGeVector3d::kZAxis);
    }
    else
    {
      first.rotateBy(OdaPI2-(OdaPI2+m_dOblique), OdGeVector3d::kZAxis);
    }
    OdGeVector3d second = defPoint2 - dimLineDefPt;
    // extLine1End is intersection of line(first, dimLineDefPt) and line(second, defPoint1)
    double t = (second.x * (defPoint1.y - dimLineDefPt.y) - second.y * (defPoint1.x - dimLineDefPt.x)) /
      (second.x * first.y - first.x * second.y);
    extLine1End = dimLineDefPt + first * t;
  }
  else
  {
    OdGeVector3d v1 = OdGeVector3d::kXAxis;
    v1.rotateBy(m_dRotAngle, OdGeVector3d::kZAxis);
    OdGeVector3d v2 = OdGeVector3d::kYAxis;
    double dOblique = 0.0;
    if(OdNonZero(m_dOblique))
    {
      dOblique = m_dOblique - OdaPI2;;
    }
    v2.rotateBy(m_dRotAngle+dOblique, OdGeVector3d::kZAxis);
    OdGeLine2d line1, line2;
    line1.set(OdGePoint2d(dimLineDefPt.x,dimLineDefPt.y), OdGeVector2d(v1.x, v1.y));
    line2.set(OdGePoint2d(extLine1Start.x, extLine1Start.y), OdGeVector2d(v2.x, v2.y));
    OdGePoint2d p;
    line1.intersectWith(line2, p);
    extLine1End.x = p.x;
    extLine1End.y = p.y;
    // extLine1End = extLine1Start + asz * OdGeVector3d(-sin(rotAng), cos(rotAng), 0.0);
  }
}
