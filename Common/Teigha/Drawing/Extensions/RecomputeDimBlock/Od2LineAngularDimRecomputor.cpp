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
#include "Od2LineAngularDimRecomputor.h"
#include "RxObjectImpl.h"

#include "OdAngularRecomputorEngine.h"

#include "Ge/GeMatrix3d.h"

#include "Db2LineAngularDimension.h"
#include "DbPoint.h"
#include "DbLine.h"
#include "DbArc.h"
#include "DbDimStyleTableRecord.h"
#include "DbObjectContextData.h"

#define STL_USING_ALGORITHM
#include "OdaSTL.h"


OdDimRecomputorPtr OdDb2LineAngularDimRecomputePEImpl::createRecomputor()
{
  return OdRxObjectImpl<Od2LineAngularDimRecomputor>::createObject();
}

Od2LineAngularDimRecomputor::Od2LineAngularDimRecomputor()
{
}

Od2LineAngularDimRecomputor::~Od2LineAngularDimRecomputor()
{
}

OdUInt8 Od2LineAngularDimRecomputor::getDimTypeFlag()
{
  return Dim2LineAngular;
}

void Od2LineAngularDimRecomputor::getDimParams(const OdDbDimension* pDim, OdDbDimensionObjectContextData* ctx)
{
  OdDimRecomputor::getDimParams(pDim, ctx);
  OdDb2LineAngularDimensionPtr pDimPtr(pDim);
  
  m_ArcDefPt = ctx ? ((OdDbAngularDimensionObjectContextData*)ctx)->arcPoint() : pDimPtr->arcPoint();
  m_DefPoint1 = pDimPtr->xLine1Start();
  m_DefPoint2 = pDimPtr->xLine1End();
  m_DefPoint3 = pDimPtr->xLine2Start();
  m_DimLineDefPt = pDimPtr->xLine2End();
  m_bExtArcOn = pDimPtr->extArcOn();
}

void Od2LineAngularDimRecomputor::setDimParams(OdDbDimension* pDim, OdDbDimensionObjectContextData* ctx) const
{
  OdDimRecomputor::setDimParams(pDim, ctx);
  OdDb2LineAngularDimensionPtr pDimPtr(pDim);
  if (ctx)
    ((OdDbAngularDimensionObjectContextData*)ctx)->setArcPoint(m_ArcDefPt);
  if (!ctx || ctx->isDefaultContextData())
    pDimPtr->setArcPoint(m_ArcDefPt);
  /*
  pDimPtr->setXLine1Start(m_DefPoint1);
  pDimPtr->setXLine1End(m_DefPoint2);
  pDimPtr->setXLine2Start(m_DefPoint3);
  pDimPtr->setXLine2End(m_DimLineDefPt);
  */
}

///

inline bool isInside(const OdGeVector3d& first, const OdGeVector3d& middle, const OdGeVector3d& second)
{
  return (first.crossProduct(middle).z >= 0.0) && (middle.crossProduct(second).z >= 0.0);
}

inline bool isBetween(const OdGePoint3d& Point1, const OdGePoint3d& middlePoint, const OdGePoint3d& Point2)
{
  if (fabs(Point1.distanceTo(middlePoint) + Point2.distanceTo(middlePoint) - Point1.distanceTo(Point2)) < 1.0e-6)
  {
    return true;
  }
  else
  {
    return false;
  }
}

inline OdGePoint3d getNearestPoint(const OdGePoint3d& middlePoint, const OdGePoint3d& Point1,
                                   const OdGePoint3d& Point2, const OdGePoint3d& Point3)
{
  if (middlePoint.distanceTo(Point1) < middlePoint.distanceTo(Point2))
  {
    // Point1 is closer then Point2
    if (middlePoint.distanceTo(Point1) < middlePoint.distanceTo(Point3))
    {
      return Point1;
    }
    else
    {
      return Point3;
    }
  }
  else
  {
    // Point2 is closer then Point1
    if (middlePoint.distanceTo(Point2) < middlePoint.distanceTo(Point3))
    {
      return Point2;
    }
    else
    {

      return Point3;
    }
  }
}

#define ADD_POINT(a) \
  point = OdDbPoint::createObject(); \
  point->setPosition(a); \
  point->setColorIndex(0); \
  point->setLayer(defPointsId); \
  point->setTransparency(transparency); \
  ents.push_back(point);

#define ADD_ARC(sAngle, eAngle) \
  pDimArc = OdDbArc::createObject(); \
  pDimArc->setLineWeight(lineweight); \
  pDimArc->setColor(color); \
  pDimArc->setCenter(centerPoint); \
  pDimArc->setRadius(radius); \
  pDimArc->setStartAngle((sAngle > Oda2PI) ? sAngle - Oda2PI : sAngle ); \
  pDimArc->setEndAngle((eAngle > Oda2PI) ? eAngle - Oda2PI : eAngle ); \
  pDimArc->setLinetype(dimLinetype); \
  pDimArc->setLinetypeScale(linetypeScale); \
  pDimArc->setTransparency(transparency); \
  ents.push_back(pDimArc);

#define ADD_LINE(a, b) \
  pLine = OdDbLine::createObject(); \
  pLine->setColor(color); \
  pLine->setLineWeight(lineweight); \
  pLine->setStartPoint(a); \
  pLine->setEndPoint(b); \
  pLine->setLinetype(dimLinetype); \
  pLine->setLinetypeScale(linetypeScale); \
  pLine->setTransparency(transparency); \
  ents.push_back(pLine);

void Od2LineAngularDimRecomputor::calculateBoundVecsRad(OdGePoint3d&  centerPoint,
                                                        OdGeVector3d& firstLineVector,
                                                        OdGeVector3d& secondLineVector,
                                                        double& radius,
                                                        int& numswaps)
{
  OdGeMatrix3d mWorld2Plane(OdGeMatrix3d::worldToPlane(m_vNormal));

  OdGePoint3d firstLinePoint1  = m_DimLineDefPt;
  OdGePoint3d firstLinePoint2  = m_DefPoint3;
  OdGePoint3d secondLinePoint1 = m_DefPoint1;
  OdGePoint3d secondLinePoint2 = m_DefPoint2;
  OdGePoint3d pointOnArc       = m_ArcDefPt;

  firstLinePoint1.transformBy(mWorld2Plane);
  firstLinePoint2.transformBy(mWorld2Plane);
  secondLinePoint1.transformBy(mWorld2Plane);
  secondLinePoint2.transformBy(mWorld2Plane);
  pointOnArc.transformBy(mWorld2Plane);

  // cr2789
  firstLinePoint1.z = firstLinePoint2.z = secondLinePoint1.z = secondLinePoint2.z = pointOnArc.z = 0.0;

  firstLineVector  = firstLinePoint2 - firstLinePoint1;
  secondLineVector = secondLinePoint2 - secondLinePoint1;

  // CR 3547
  OdGeLine2d line1(firstLinePoint1.convert2d(), firstLinePoint2.convert2d());
  OdGeLine2d line2(secondLinePoint1.convert2d(), secondLinePoint2.convert2d());
  OdGePoint2d pnt;
  if ( line1.intersectWith(line2, pnt) )
  {
    centerPoint.set(pnt.x, pnt.y, 0.0);
    // intersectLines(centerPoint, firstLinePoint1, secondLinePoint1, firstLineVector, secondLineVector);
  } else
  {
    if ( firstLinePoint1 == secondLinePoint2 || firstLinePoint2 == secondLinePoint2  )
      centerPoint = secondLinePoint2;
    else 
      centerPoint = secondLinePoint1;
  }
  OdGeVector3d arcDefVector = pointOnArc - centerPoint;
  radius = centerPoint.distanceTo(pointOnArc);

  // setting up firstLineVector and secondLineVector to direct from center parallel according extline
  numswaps = 0;
  if (firstLineVector.angleTo(secondLineVector, OdGeVector3d::kZAxis) > OdaPI)
  {
    std::swap(firstLineVector, secondLineVector);
    numswaps ++;
  }
  if (firstLineVector.angleTo(secondLineVector, OdGeVector3d::kZAxis) > OdaPI2)
  {
    firstLineVector *= -1.0;
    std::swap(firstLineVector, secondLineVector);
    numswaps ++;
  }
  // #14709 Arc point is equal to one of dimension line points
  if ( firstLineVector.isCodirectionalTo(arcDefVector) || firstLineVector.isCodirectionalTo(-arcDefVector) )
  {
    arcDefVector.rotateBy(-0.01, OdGeVector3d::kZAxis);
  } else if ( secondLineVector.isCodirectionalTo(arcDefVector) || secondLineVector.isCodirectionalTo(-arcDefVector) )
  {
    arcDefVector.rotateBy(0.01, OdGeVector3d::kZAxis);
  }

  ODA_ASSERT(firstLineVector.angleTo(secondLineVector, OdGeVector3d::kZAxis) <= OdaPI2);
  if (!isInside(firstLineVector, arcDefVector, secondLineVector))
  {
    firstLineVector  *= -1.0;
    secondLineVector *= -1.0;
    if (!isInside(firstLineVector, arcDefVector, secondLineVector))
    {
      firstLineVector *= -1.0;
      std::swap(firstLineVector, secondLineVector);
      numswaps ++;
      if (!isInside(firstLineVector, arcDefVector, secondLineVector))
      {
        firstLineVector  *= -1.0;
        secondLineVector *= -1.0;
      }
    }
  }
  ODA_ASSERT(isInside(firstLineVector, arcDefVector, secondLineVector));
}

void Od2LineAngularDimRecomputor::buildDimension(OdArray<OdDbEntityPtr>& ents, OdDbDimStyleTableRecord* pDimVars)
{
  OdDbPointPtr point;
  OdDbObjectId defPointsId = getLayerDefpointsId();
  OdCmTransparency transparency = m_transparency;

  OdGeMatrix3d mWorld2Plane(OdGeMatrix3d::worldToPlane(m_vNormal));
  OdGeMatrix3d mPlane2World(OdGeMatrix3d::planeToWorld(m_vNormal));

  OdGePoint3d firstLinePoint1  = m_DimLineDefPt;
  OdGePoint3d firstLinePoint2  = m_DefPoint3;
  OdGePoint3d secondLinePoint1 = m_DefPoint1;
  OdGePoint3d secondLinePoint2 = m_DefPoint2;
  OdGePoint3d pointOnArc       = m_ArcDefPt;


  firstLinePoint1.transformBy(mWorld2Plane);
  firstLinePoint2.transformBy(mWorld2Plane);
  secondLinePoint1.transformBy(mWorld2Plane);
  secondLinePoint2.transformBy(mWorld2Plane);
  pointOnArc.transformBy(mWorld2Plane);
  firstLinePoint1.z = firstLinePoint2.z = secondLinePoint1.z = secondLinePoint2.z = pointOnArc.z = 0.0;
  Od2LineAngularRecomputorEngine engine;

  OdGeVector3d firstLineVector;
  OdGeVector3d secondLineVector;
  double radius;
  OdGePoint3d centerPoint;
  int numswaps;

  calculateBoundVecsRad(centerPoint, firstLineVector, secondLineVector, radius, numswaps);

  if ( firstLineVector.isZeroLength() || secondLineVector.isZeroLength() )
  {
    // ACAD remains only this five points
    ADD_POINT(pointOnArc);
    ADD_POINT(firstLinePoint2);
    ADD_POINT(secondLinePoint1);
    ADD_POINT(secondLinePoint2);
    ADD_POINT(firstLinePoint1);
    return;
  }
  if ( m_bUserDefinedTextPosition && getDimtmove(pDimVars) == 0 )
  {
    radius = centerPoint.distanceTo(OdGePoint3d(m_ocsDimTextMidPt.x, m_ocsDimTextMidPt.y, 0.0));
  }

  // Extension lines
  if (numswaps % 2)
  {
    // Changing vectors
    std::swap(firstLineVector, secondLineVector);
  }
  double firstAngle  = OdGeVector3d::kXAxis.angleTo(firstLineVector, OdGeVector3d::kZAxis);
  double secondAngle = OdGeVector3d::kXAxis.angleTo(secondLineVector, OdGeVector3d::kZAxis);

  OdGePoint3d firstLineArcPoint = centerPoint + radius * firstLineVector.normalize();
  OdGePoint3d secondLineArcPoint = centerPoint + radius * secondLineVector.normalize();

  engine.m_xLine1PtEnd = firstLineArcPoint;
  engine.m_xLine2PtEnd = secondLineArcPoint;
  engine.m_extLine1Angle = firstAngle;
  engine.m_extLine2Angle = secondAngle;
  engine.m_radiusDimArc = engine.m_sourceArcRadius = radius;

  getUCS(engine.m_xAxis, engine.m_yAxis);

  if (!isBetween(firstLinePoint1, firstLineArcPoint, firstLinePoint2))
  {
    OdGePoint3d startPt = getNearestPoint(firstLineArcPoint, firstLinePoint1, firstLinePoint2, centerPoint);
    engine.m_xLine1Pt = startPt;
    engine.m_bExtLine1Enabled = true;
  } else
  {
    OdGeVector3d v1 = firstLineArcPoint - centerPoint;
    OdGeVector3d v2 = firstLinePoint1 - centerPoint;
    if ( v1.isCodirectionalTo(v2) )
      engine.m_xLine1Pt = firstLinePoint1;
    else
      engine.m_xLine1Pt = firstLinePoint2;
      
    engine.m_bExtLine1Enabled = false;
  }
  if (!isBetween(secondLinePoint1, secondLineArcPoint, secondLinePoint2))
  {
    OdGePoint3d startPt = getNearestPoint(secondLineArcPoint, secondLinePoint1, secondLinePoint2, centerPoint);
    engine.m_xLine2Pt = startPt;
    engine.m_bExtLine2Enabled = true;
  } else
  {
    OdGeVector3d v1 = secondLineArcPoint - centerPoint;
    OdGeVector3d v2 = secondLinePoint1 - centerPoint;
    if ( v1.isCodirectionalTo(v2) )
      engine.m_xLine2Pt = secondLinePoint1;
    else
      engine.m_xLine2Pt = secondLinePoint2;
    engine.m_bExtLine2Enabled = false;
  }

  OdDb::LineWeight lineweight = OdDb::LineWeight(getDimlwd(pDimVars));
  OdCmColor color = getDimclrd(pDimVars);
  OdDbObjectId dimLinetype = m_DimLinetypeId;
  double linetypeScale = m_dLinetypeScale;

  engine.m_arcPt = pointOnArc;
  engine.m_centerPtArc = centerPoint;

  OdUInt16 just = getDimjust(pDimVars);
  OdUInt16 tad = getDimtad(pDimVars);
  engine.setTextParams(just, tad, getDimgap(pDimVars));

  OdDbMTextPtr pText;
  createTextEntityForDimGraphic(pText, pDimVars);

  OdDbMTextPtr pLabel, pRate;
  if ( m_bInspection )
  {
    engine.setInspection( (m_nInspectionFrame & OdDbDimension::kShapeLabel) != 0, 
      (m_nInspectionFrame & OdDbDimension::kShapeRate) != 0, 
      (m_nInspectionFrame & (OdDbDimension::kShapeRound | OdDbDimension::kShapeAngular ) ) != 0 );
    createInspectionEntitisForDimGraphic(pLabel, pRate, pDimVars);
  }

  if ( m_bUserDefinedTextPosition && getDimtmove(pDimVars) == 0 )
  {
    // calc m_arcPt...
    engine.m_textPosition = m_ocsDimTextMidPt;
    engine.m_textPosition.z = 0;
    engine.calcDimArcPoint();
    m_ArcDefPt = engine.m_arcPt;
    m_ArcDefPt.z = m_ocsDimTextMidPt.z;
    m_ArcDefPt.transformBy(mPlane2World);
  } else
  {
    engine.calcDimPoints();
  }
  // Arrow blocks
  OdDbObjectId dimBlk1, dimBlk2;
  if(getDimsah(pDimVars))
  {
    dimBlk1 = getDimblk1(pDimVars);
    dimBlk2 = getDimblk2(pDimVars);
  }
  else
  {
    dimBlk1 = dimBlk2 = getDimblk(pDimVars);
  }
  double asz1, asz2, asz;
  asz = asz1 = asz2 = getDimasz(pDimVars);
  if(toExtend(dimBlk1))
  {
    asz1 = 0.0;
  }
  if(toExtend(dimBlk2))
  {
    asz2 = 0.0;
  }
  engine.setAszParams(asz1, asz2, asz, m_bArrowFirstIsFlipped, m_bArrowSecondIsFlipped);

  engine.m_dimTih = getDimtih(pDimVars);
  engine.m_dimToh = getDimtoh(pDimVars);
  engine.m_dimTvp = getDimtvp(pDimVars);

  if( !m_bUserDefinedTextPosition )
  {
    if ( !pText.isNull() )
    {
      engine.m_textPosition = engine.m_middleArcPt;
      pText->setDirection(engine.m_dirToMiddleArcPt.perpVector().normalize());
      engine.m_dTextRot = pText->rotation();
      pText->setLocation(engine.m_middleArcPt);
      engine.calcTextExtents(pText);

      if ( m_bInspection )
      {
        engine.calcInspectionExtents(pLabel, pRate, true);
      }

      if ( OdNonZero(m_dTextRot) )
      {
        OdGeVector3d textDir = engine.m_xAxis;
        textDir.rotateBy(m_dTextRot, OdGeVector3d::kZAxis);
        pText->setDirection(textDir);
        engine.m_dTextRot = pText->rotation();
        engine.m_bUseRotate = true;
      }

      engine.adjustTextAndArrowsPlace(getDimatfit(pDimVars), getDimtix(pDimVars));
      engine.adjustTextLocation(getDimtmove(pDimVars), getDimexe(pDimVars));

      if (OdNonZero(m_dTextRot) )
      {
        engine.m_textDirection = engine.m_xAxis;
        engine.m_textDirection.rotateBy(m_dTextRot, OdGeVector3d::kZAxis);
      }
      pText->setDirection(engine.m_textDirection);

      pText->setLocation(engine.m_textPosition);
      double z = m_ocsDimTextMidPt.z;
      m_ocsDimTextMidPt = engine.m_textPosition;
      m_ocsDimTextMidPt.z = z;
    } else
    {
      engine.fitTextAndArrows(false, true, false);
      if ( engine.m_isFitArrowsWithoutText )
        engine.m_bArrowsInside = true;
      else
        engine.m_bArrowsInside = false;
      engine.m_bTextInside = true;
      engine.m_bNeedCalcTextIntersection = false;
    }
  }
  else // User defined text position
  {
    engine.m_textPosition = m_ocsDimTextMidPt;
    engine.m_textPosition.z = 0.0;
    engine.m_bUserDefPos = true;
    engine.m_bTextInside = false;
    if ( OdNonZero(m_dTextRot) )
    {
      OdGeVector3d textDir = engine.m_xAxis;
      textDir.rotateBy(m_dTextRot, OdGeVector3d::kZAxis);
      pText->setDirection(textDir);
      engine.m_dTextRot = pText->rotation();
      engine.m_bUseRotate = true;
    } else
      engine.m_bUseRotate = false;

    if ( !pText.isNull() )
      engine.calcTextExtents(pText);
    if ( m_bInspection )
    {
      engine.calcInspectionExtents(pLabel, pRate, true);
    }

    OdInt16 dm = getDimtmove(pDimVars);
    engine.adjustUserDefText(dm, asz, getDimexe(pDimVars));
    if ( dm == 0 ) 
    {
      OdGePoint3d pp = engine.m_textPosition;
      engine.adjustTextAndArrowsPlace(getDimatfit(pDimVars), getDimtix(pDimVars));
      engine.m_textPosition = pp;
    } else
    {
      engine.fitTextAndArrows(false, true, false);
      if ( engine.m_isFitArrowsWithoutText )
      {
        engine.m_bArrowsInside = true;
      } else
      {
        engine.m_bArrowsInside = false;
        engine.m_bNeedCalcTextIntersection = false;
      }
    }
    if ( !pText.isNull() )
    {
      if (OdNonZero(m_dTextRot) )
      {
        engine.m_textDirection = engine.m_xAxis;
        engine.m_textDirection.rotateBy(m_dTextRot, OdGeVector3d::kZAxis);
      }
      pText->setDirection(engine.m_textDirection);

      pText->setLocation(engine.m_textPosition);

    }

  }

  if ( engine.m_bLeader )
  {
    OdDbLinePtr pLine;
    ADD_LINE(engine.m_ldrPt1, engine.m_ldrPt2);
    setDimLineIndex(ents.last(), DIM_LINE_INDEX);
    ADD_LINE(engine.m_ldrPt2, engine.m_middleArcPt);
    setDimLineIndex(ents.last(), DIM_LINE_INDEX);
  }

  if ( engine.m_bTextHasLine )
  {
    OdDbLinePtr pLine;
    ADD_LINE(engine.m_textLinePt1, engine.m_textLinePt2);
    setDimLineIndex(ents.last(), DIM_LINE_INDEX);
  }

  if ( !pText.isNull() )
  {
    engine.calcTextExtents(pText, true);
    engine.m_dTextRot = pText->rotation();
  } else
  {
    engine.m_bNeedCalcTextIntersection = false;
  }
  engine.intersectTextBoxWithDimArc();

  if ( engine.m_bSwaped )
  {
    if ( engine.m_bExtLine1Enabled )
    {
      if ( buildExtLine2(engine.m_xLine1Pt, engine.m_xLine1PtEnd, ents, pDimVars, 0.0) )
        setDimLineIndex(ents.last(), EXT_LINE_2_INDEX);
    }
    if ( engine.m_bExtLine2Enabled )
    {
      if (buildExtLine1(engine.m_xLine2Pt, engine.m_xLine2PtEnd, ents, pDimVars, 0.0) )
        setDimLineIndex(ents.last(), EXT_LINE_1_INDEX);
    }

  } else
  {
    if ( engine.m_bExtLine1Enabled )
    {
      if ( buildExtLine1(engine.m_xLine1Pt, engine.m_xLine1PtEnd, ents, pDimVars, 0.0) )
        setDimLineIndex(ents.last(), EXT_LINE_1_INDEX);
    }
    if ( engine.m_bExtLine2Enabled )
    {
      if ( buildExtLine2(engine.m_xLine2Pt, engine.m_xLine2PtEnd, ents, pDimVars, 0.0) )
        setDimLineIndex(ents.last(), EXT_LINE_2_INDEX);
    }
  }

  // OdGePoint3d centerPoint;
  // double radius;
  OdDbArcPtr pDimArc;

  OdUInt16 sd1 = getDimsd1(pDimVars);
  OdUInt16 sd2 = getDimsd2(pDimVars);

  bool bSuppressDimLinesAtAll = false;

  if ( sd1 && sd2 && !engine.m_bTextInside )
  {
    // TODO: text is inside in this case It need check above
    bSuppressDimLinesAtAll = true;
  } 

  centerPoint = engine.m_centerDimPtArc;
  radius = engine.m_radiusDimArc;

  engine.applyFlipArrows();
  engine.calcDirectionArrows();
  engine.addTailDimLineArc(asz, getDimsoxd(pDimVars));

  if ( !engine.m_bArrowsInside )
  {
    if ( !( m_bUserDefinedTextPosition && getDimtmove(pDimVars) == 2 ) )
    {
      if ( getDimtofl(pDimVars) )
        engine.m_dimLines = 1;
      else
        bSuppressDimLinesAtAll = true;
    }
  }
  if ( getDimtmove(pDimVars) == 1 && ( engine.m_bTextHasLine || engine.m_bLeader ) )
  {
    bSuppressDimLinesAtAll = false;
    engine.m_dimLines = 1;
  }

  engine.prepareDimArc();

  if ( engine.m_bAddedArrow1Arc && !sd1)
  {
    ADD_ARC(engine.m_addedArrow1Arc.startAngFromXAxis(), engine.m_addedArrow1Arc.endAngFromXAxis());
    setDimLineIndex(ents.last(), DIM_LINE_INDEX);
  }
  if ( engine.m_bAddedArrow2Arc && !sd2)
  {
    ADD_ARC(engine.m_addedArrow2Arc.startAngFromXAxis(), engine.m_addedArrow2Arc.endAngFromXAxis());
    setDimLineIndex(ents.last(), DIM_LINE_INDEX);
  }

  if ( !bSuppressDimLinesAtAll )
  {
    if ( ( sd1 || sd2 ) && engine.m_bTextInside )
    {
      engine.calcTextAngles();
      if ( !sd1 )
      {
        if ( engine.m_dimLines == 2 )
        {
          ADD_ARC(engine.m_dimArc1EndAngle, engine.m_EndTextAngle );
        } else
        {
          ADD_ARC(engine.m_dimArc1EndAngle, engine.m_StartTextAngle );
        }
        setDimLineIndex(ents.last(), DIM_LINE_INDEX);
      } else if ( !sd2 )
      {
        if ( engine.m_dimLines == 2 )
        {
          ADD_ARC(engine.m_StartTextAngle, engine.m_dimArc2EndAngle );
        }
        else
        {
          ADD_ARC(engine.m_EndTextAngle, engine.m_dimArc2EndAngle );
        }
        setDimLineIndex(ents.last(), DIM_LINE_INDEX);
      } // else
      // ODA_ASSERT(0);
    } else
    {
      if ( engine.m_dimLines == 1  )
      {
        if ( engine.m_bArrowsInside )
        {
          ADD_ARC(engine.m_dimArc1EndAngle, engine.m_dimArc2EndAngle );
        }
        else
        {
          ADD_ARC(engine.m_extLine1Angle, engine.m_extLine2Angle );
        }
        setDimLineIndex(ents.last(), DIM_LINE_INDEX);
      }
      else
      {
        ADD_ARC(engine.m_dimArc1EndAngle, engine.m_dimArc1StartAngle );
        setDimLineIndex(ents.last(), DIM_LINE_INDEX);
        ADD_ARC(engine.m_dimArc2StartAngle, engine.m_dimArc2EndAngle);
        setDimLineIndex(ents.last(), DIM_LINE_INDEX);
      }
    }
  }

  if ( !bSuppressDimLinesAtAll && !engine.m_bTextInside && 
    engine.m_dimLines == 1 && !m_bUserDefinedTextPosition)
  {
    pDimVars->setDimsd1(false);
    pDimVars->setDimsd2(false);
  }

  if ( !( !engine.m_bArrowsInside && getDimsoxd(pDimVars) ) )
  {
    buildArrow1(engine.m_arrow1Pt, engine.m_dirArrow1, ents, pDimVars);
    buildArrow2(engine.m_arrow2Pt, engine.m_dirArrow2, ents, pDimVars);
  }

  // Make Inspection
  if ( m_bInspection )
  {
    makeInspectionFrame(pDimVars, &engine, ents);
    if ( !pText.isNull() )
    {
      pText->setLocation(engine.m_textPosition);
      ents.push_back(pText);
    }

    if ( !pLabel.isNull() )
    {
      pLabel->setLocation(engine.m_labelPosition);
      pLabel->setDirection(engine.m_textDirection);
      ents.push_back(pLabel);
    }

    if ( !pRate.isNull() )
    {
      pRate->setLocation(engine.m_ratePosition);
      pRate->setDirection(engine.m_textDirection);
      ents.push_back(pRate);
    }
  } else
  {
    if(pText.get())
    {
      ents.push_back(pText);
      buildBoundaryBox(pDimVars, pText, ents);
    }
  }

  if ( m_bUserDefinedTextPosition && m_bExtArcOn )
  {
    engine.m_dimOcsTextPosition = m_ocsDimTextMidPt;
    engine.m_dimOcsTextPosition.z = 0.0;
    if ( engine.makeExtensionArc(getDimtmove(pDimVars)) )
    {
      ADD_ARC(engine.m_dStartExtAng, engine.m_dEndExtAng);
    }
  }

  engine.correctDimArcPoint();
  m_ArcDefPt = engine.m_arcPt;
  m_ArcDefPt.z = m_ocsDimTextMidPt.z;
  m_ArcDefPt.transformBy(mPlane2World);

  if ( !pText.isNull() )
  {
    pText->setAttachmentMovingLocation((OdDbMText::AttachmentPoint)m_AttachPt);
  } 
  // Points
  ADD_POINT(engine.m_arcPt);
  ADD_POINT(firstLinePoint2);
  ADD_POINT(secondLinePoint1);
  ADD_POINT(secondLinePoint2);
  ADD_POINT(firstLinePoint1);
}

void Od2LineAngularDimRecomputor::updateMeasurement(OdDbDimStyleTableRecord* )
{
  OdGeVector3d firstLineVector, secondLineVector;
  double radius;
  int numswaps;
  OdGePoint3d center;
  calculateBoundVecsRad(center, firstLineVector, secondLineVector, radius, numswaps);
  m_dMeasurement = fabs(firstLineVector.angleTo(secondLineVector));
}
