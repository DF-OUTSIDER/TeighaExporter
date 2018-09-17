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
#include "Od3PointAngularDimRecomputor.h"
#include "RxObjectImpl.h"
#include "DbObjectContextData.h"

#include "Ge/GeMatrix3d.h"
/*
#include "Ge/GeExtents3d.h"
#include "Ge/GeCircArc2d.h"
#include "Ge/GeExtents3d.h"
#include "Ge/GeLine2d.h"
#include "Ge/GeLineSeg2d.h"
*/
#include "Db3PointAngularDimension.h"
#include "DbLine.h"
#include "DbPoint.h"
#include "DbArc.h"
#include "DbTextStyleTableRecord.h"
#include "DbDatabase.h"
#include "DbDimStyleTableRecord.h"

#include "OdAngularRecomputorEngine.h"
#include "OdRound.h"

#define STL_USING_ALGORITHM
#include "OdaSTL.h"

#include "DbUnitsFormatter.h"

OdDimRecomputorPtr OdDb3PointAngularDimRecomputePEImpl::createRecomputor()
{
  return OdRxObjectImpl<Od3PointAngularDimRecomputor>::createObject();
}

Od3PointAngularDimRecomputor::Od3PointAngularDimRecomputor()
{
}

Od3PointAngularDimRecomputor::~Od3PointAngularDimRecomputor()
{
}

OdUInt8 Od3PointAngularDimRecomputor::getDimTypeFlag()
{
  return Dim3PointAngular;
}

void Od3PointAngularDimRecomputor::getDimParams(const OdDbDimension* pDim, OdDbDimensionObjectContextData* ctx)
{
  OdDimRecomputor::getDimParams(pDim, ctx);
  OdDb3PointAngularDimensionPtr pDimPtr(pDim);
  m_DefPoint1 = pDimPtr->xLine1Point();
  m_DefPoint2 = pDimPtr->xLine2Point();
  m_DefPoint3 = pDimPtr->centerPoint();
  m_DimLineDefPt = ctx ? ((OdDbAngularDimensionObjectContextData*)ctx)->arcPoint() : pDimPtr->arcPoint();
  m_bExtArcOn = pDimPtr->extArcOn();
}

void Od3PointAngularDimRecomputor::setDimParams(OdDbDimension* pDim, OdDbDimensionObjectContextData* ctx) const
{
  OdDimRecomputor::setDimParams(pDim, ctx);
  OdDb3PointAngularDimensionPtr pDimPtr(pDim);
  if (ctx)
    ((OdDbAngularDimensionObjectContextData*)ctx)->setArcPoint(m_DimLineDefPt);
  if (!ctx || ctx->isDefaultContextData())
    pDimPtr->setArcPoint(m_DimLineDefPt);
  /*
  pDimPtr->setXLine1Point(m_DefPoint1);
  pDimPtr->setXLine2Point(m_DefPoint2);
  pDimPtr->setCenterPoint(m_DefPoint3);
  */
}


///

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

void Od3PointAngularDimRecomputor::preprocessDimPoints(OdDbDimStyleTableRecord* )
{
  // empty
}

void Od3PointAngularDimRecomputor::updateMeasurement(OdDbDimStyleTableRecord* )
{
  Od3PointAngularRecomputorEngine engine;

  engine.m_centerPtArc = m_DefPoint3;
  engine.m_arcPt = m_DimLineDefPt;
  engine.m_xLine1Pt = m_DefPoint1;
  engine.m_xLine2Pt = m_DefPoint2;

  OdGeMatrix3d mWorld2Plane(OdGeMatrix3d::worldToPlane(m_vNormal));
  engine.tranformBy(mWorld2Plane);
  m_dMeasurement = engine.measurement();
}

void Od3PointAngularDimRecomputor::formatDimStr(OdString& mainVal, double value, OdInt16 units,
                                                OdInt16 precision, double /*roundOff*/,
                                                OdInt16 azin, OdChar dsep, OdInt16 /*fracFormat*/,
                                                bool /*forLimits*/, bool /*forTolerances*/, double /*dimTfac*/)
{
  double mesNum;
  switch(units)
  {
  case 0: // Decimal degrees
  case 4: // Surveyor's units
    mesNum = OdaToDegree(value);
    formatDimDecStr(mainVal, (azin & 1) != 0, (azin & 2) != 0, precision, mesNum, 0.0, dsep);
    mainVal += OD_T("%%d");
    break;
  case 1: // Degrees, minutes, seconds
    {
      //6.03.2008 Bricsys: decomposition value from radians to degrees, minutes, seconds is not exactly.
      // Sometimes function returned string like this 7d15'60" instead of 7d16'0" 
      // OdDbUnitsFormatter::formatA() function returns more correctly string.
      mainVal = OdDbUnitsFormatter::formatA(value, OdDbUnitsFormatter::AUnits(units), precision, 0, 0);
      mainVal.replace(OD_T("d"), OD_T("%%d"));
#if 0
      mesNum = OdaToDegree(value);
      double rndScnd = pow(0.1, precision);
      mesNum = OdRound((mesNum + 1.0e-10) / rndScnd) * rndScnd;

      int degrees = (int)floor(mesNum);
      double remainder = mesNum - (double)degrees;
      remainder *= 60;
      int minutes = (int)floor(remainder);
      remainder -= (double)minutes;
      remainder *= 60; // now seconds are here
      OdString degreesStr, minsStr, secsStr;
      degreesStr.format(OD_T("%i"), degrees);
      minsStr.format(OD_T("%i"), minutes);

      mainVal = degreesStr + OD_T("%%d"); // 20.12.2004 GU, cr2437 -- depends on precision
      if(precision > 0)
        mainVal += minsStr + OD_T("'");
      if(precision > 2)
      {
        switch (precision)
        {
          case 3:
          case 4:
            secsStr.format(OD_T("%.0lf"), remainder);
            break;
          case 5:
            secsStr.format(OD_T("%.1lf"), remainder);
            break;
          case 6:
            secsStr.format(OD_T("%.2lf"), remainder);
            break;
          case 7:
            secsStr.format(OD_T("%.3lf"), remainder);
            break;
          case 8:
            secsStr.format(OD_T("%.4lf"), remainder);
            break;
        }
        mainVal += secsStr + OD_T("\"");
      }
#endif
    }
    break;
  case 2: // Gradians
    mesNum = value * 200 / OdaPI;
    formatDimDecStr(mainVal, (azin & 1) != 0, (azin & 2) != 0, precision, mesNum, 0.0, dsep);
    mainVal += OD_T("g");
    break;
  case 3: // Radians
    mesNum = value;
    formatDimDecStr(mainVal, (azin & 1) != 0, (azin & 2) != 0, precision, mesNum, 0.0, dsep);
    mainVal += OD_T("r");
    break;
  default:
    ODA_FAIL();
  }
}

#if 0
static bool isVectorInOrder(const OdGeVector3d& lowAngle,
                            const OdGeVector3d& itVector,
                            const OdGeVector3d& highAngle)
{
  return (OdZero(fabs(lowAngle.angleTo(itVector, OdGeVector3d::kZAxis))+
    fabs(itVector.angleTo(highAngle, OdGeVector3d::kZAxis))-
    fabs(lowAngle.angleTo(highAngle, OdGeVector3d::kZAxis)) /* tol here */ ));
}

// Not very good function. Must be Ge functionality.
static void checkIntersectionsOfArcAndHorLineSeg(OdGePoint3dArray& intersections, const OdGePoint3d& centerPoint,
                                                 double radius, const OdGeVector3d& lowAngle,
                                                 const OdGeVector3d& highAngle,
                                                 double y, double minX, double maxX)
{
  OdGePoint3dArray cand;
  double deltaSq = radius*radius - (centerPoint.y - y)*(centerPoint.y - y);
  if (deltaSq < 0)
    return;
  double deltaX = sqrt(deltaSq);
  if(((centerPoint.x - deltaX) > minX) && ((centerPoint.x - deltaX) < maxX))
  {
    cand.push_back(OdGePoint3d((centerPoint.x - deltaX), y, 0.0));
  }
  if(((centerPoint.x + deltaX) > minX) && ((centerPoint.x + deltaX) < maxX))
  {
    cand.push_back(OdGePoint3d((centerPoint.x + deltaX), y, 0.0));
  }
  // Verifying angles
  for(OdGePoint3dArray::iterator it = cand.begin(); it != cand.end(); ++ it)
  {
    OdGeVector3d itVector((*it) - centerPoint);
    // G.Udov, 28.07.2003
    // this is unsuitable condition - file D:\OWF\dimensions\angular\obr\2.dwg
    // if((lowAngle.crossProduct(itVector).z > 0) && (itVector.crossProduct(highAngle).z > 0))

    //if(OdZero(fabs(lowAngle.angleTo(itVector, OdGeVector3d::kZAxis))+
    //   fabs(itVector.angleTo(highAngle, OdGeVector3d::kZAxis))-
    //   fabs(lowAngle.angleTo(highAngle, OdGeVector3d::kZAxis)) /* tol here */ ))
    if(isVectorInOrder(lowAngle, itVector, highAngle))
    {
      intersections.push_back(*it);
    }
  }
}

// Not very good function. Must be Ge functionality.
static void checkIntersectionsOfArcAndVertLineSeg(OdGePoint3dArray& intersections, const OdGePoint3d& centerPoint,
                                                  double radius, const OdGeVector3d& lowAngle,
                                                  const OdGeVector3d& highAngle,
                                                  double x, double minY, double maxY)
{
  OdGePoint3dArray intersectionsInv;
  OdGePoint3d centerPointInv(centerPoint.y, centerPoint.x, 0.0);
  OdGeVector3d lowAngleInv(lowAngle.y, lowAngle.x, 0.0);
  OdGeVector3d highAngleInv(highAngle.y, highAngle.x, 0.0);
  std::swap(lowAngleInv, highAngleInv); // ?
  checkIntersectionsOfArcAndHorLineSeg(intersectionsInv, centerPointInv, radius, lowAngleInv, highAngleInv,
    x, minY, maxY);
  for(OdGePoint3dArray::iterator it = intersectionsInv.begin(); it != intersectionsInv.end(); ++ it)
  {
    intersections.push_back(OdGePoint3d(it->y, it->x, 0.0));
  }
}
#endif

void Od3PointAngularDimRecomputor::buildDimension(OdArray<OdDbEntityPtr>& ents, OdDbDimStyleTableRecord* pDimVars)
{
  OdDb::LineWeight lineweight = OdDb::LineWeight(getDimlwd(pDimVars));
  OdCmColor color = getDimclrd(pDimVars);
  OdDbObjectId dimLinetype = m_DimLinetypeId;
  double linetypeScale = m_dLinetypeScale;
  OdCmTransparency transparency = m_transparency;

  Od3PointAngularRecomputorEngine engine;

  engine.m_centerPtArc = m_DefPoint3;
  engine.m_arcPt = m_DimLineDefPt;
  engine.m_xLine1Pt = m_DefPoint1;
  engine.m_xLine2Pt = m_DefPoint2;

  OdGeMatrix3d mWorld2Plane(OdGeMatrix3d::worldToPlane(m_vNormal));
  OdGeMatrix3d mPlane2World(OdGeMatrix3d::planeToWorld(m_vNormal));

  engine.tranformBy(mWorld2Plane);
  getUCS(engine.m_xAxis, engine.m_yAxis);

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
    engine.m_textPosition.z = 0.0;
    engine.calcDimArcPoint();
    m_DimLineDefPt = engine.m_arcPt;
    m_DimLineDefPt.z = m_ocsDimTextMidPt.z;
    m_DimLineDefPt.transformBy(mPlane2World);
  } 

  engine.calcExtLinePoints();
  engine.calcExtLineAngles();

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

  // engine.calcArrowAngles(asz1, asz2);
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
    engine.fitTextAndArrows(false, true, false);

    if ( engine.m_isFitArrowsWithoutText )
    {
      engine.m_bArrowsInside = true;
    } else
    {
      engine.m_bArrowsInside = false;
      engine.m_bNeedCalcTextIntersection = false;
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

  if ( buildExtLine1(engine.m_xLine1Pt, engine.m_xLine1PtEnd, ents, pDimVars, 0.0) )
    setDimLineIndex(ents.last(), EXT_LINE_1_INDEX);

  if ( buildExtLine2(engine.m_xLine2Pt, engine.m_xLine2PtEnd, ents, pDimVars, 0.0) )
    setDimLineIndex(ents.last(), EXT_LINE_2_INDEX);

  OdGePoint3d centerPoint;
  double radius;
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
          setDimLineIndex(ents.last(), DIM_LINE_INDEX);
        }
        else
        {
          ADD_ARC(engine.m_extLine1Angle, engine.m_extLine2Angle );
          setDimLineIndex(ents.last(), DIM_LINE_INDEX);
        }
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
    if ( engine.makeExtensionArc(getDimtmove(pDimVars)) )
    {
      ADD_ARC(engine.m_dStartExtAng, engine.m_dEndExtAng);
    }
  }

  engine.correctDimArcPoint();
  m_DimLineDefPt = engine.m_arcPt;
  m_DimLineDefPt.z = m_ocsDimTextMidPt.z;
  m_DimLineDefPt.transformBy(mPlane2World);

  if ( !pText.isNull() )
  {
    pText->setAttachmentMovingLocation((OdDbMText::AttachmentPoint)m_AttachPt);
  } 
  OdDbPointPtr point;
  OdDbObjectId defPointsId = getLayerDefpointsId();
  ADD_POINT(engine.m_centerPtArc);
  ADD_POINT(engine.m_xLine1Pt);
  ADD_POINT(engine.m_xLine2Pt);
  ADD_POINT(engine.m_arcPt);

}

void Od3PointAngularDimRecomputor::formatMeasurement(OdString& mainVal, OdDbDimStyleTableRecord* pDimVars)
{
  // text
  OdInt16 azin = getDimazin(pDimVars);
  OdInt16 precision = getDimadec(pDimVars);
  OdChar dsep = (OdChar)getDimdsep(pDimVars);
  OdInt16 units = getDimaunit(pDimVars);
  if(precision == -1)
  {
    precision = getDimdec(pDimVars);
  }
  double  dimTm = getDimtm(pDimVars);
  double  dimTp = getDimtp(pDimVars);
  // They are in according units, so we must to conver them to radians
  switch(units)
  {
  case 0: // Decimal degrees
  case 1: // Degrees\mins\secs
    dimTm = OdaToRadian(dimTm);
    dimTp = OdaToRadian(dimTp);
    break;
  case 2: // Gradians
    dimTm = dimTm * OdaPI / 200.0;
    dimTp = dimTp * OdaPI / 200.0;
    break;
  }
  // now filling m_strDimText
  int quotesNum = m_strDimText.find(OD_T("<>"));
  if(m_strDimText.isEmpty() || (quotesNum != -1))
  {
    // dimlunit --> dimaunit
    formatDimStrWithLimsAndTols(mainVal, getDimlim(pDimVars), getDimtol(pDimVars),
      m_dMeasurement, units, dimTm, dimTp, azin, getDimtfac(pDimVars), precision, 0.0, dsep, 0,
      getDimtxt(pDimVars), azin, precision, getDimpost(pDimVars), true);
    OdUInt8 tolj = getDimtolj(pDimVars);
    if(tolj != 0)
    {
      OdString toljAdd;
      toljAdd.format(OD_T("\\A%i;"), (int)tolj);
      mainVal = toljAdd + mainVal;
    }
    if(quotesNum != -1)
    {
      OdString oldMes = mainVal;
      mainVal = m_strDimText;
      mainVal.replace(OD_T("<>"), oldMes.c_str());
    }
    int xPos = mainVal.find(OD_T("\\X"));
    if(xPos != -1)
    {
      mainVal.replace(OD_T("\\X"), OD_T("\\P"));
    }
  }
  else
  {
    mainVal = m_strDimText;
    mainVal.replace(OD_T("\\X"), OD_T("\\P"));
  }
  ////////////
}

