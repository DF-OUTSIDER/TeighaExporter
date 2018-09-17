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
#include "OdArcDimRecomputor.h"
#include "RxObjectImpl.h"

#include "OdAngularRecomputorEngine.h"
#include "DbObjectContextData.h"
/*
#include "Ge/GeCircArc2d.h"
#include "Ge/GeExtents3d.h"
#include "Ge/GeLine2d.h"
*/
#include "Ge/GeLineSeg2d.h"

#include "DbArcDimension.h"
#include "DbLine.h"
#include "DbArc.h"
#include "DbDimStyleTableRecord.h"
#include "DbPoint.h"
#include "DbDatabase.h"

#define STL_USING_ALGORITHM
#include "OdaSTL.h"


OdDimRecomputorPtr OdDbArcDimRecomputePEImpl::createRecomputor()
{
  return OdRxObjectImpl<OdArcDimRecomputor>::createObject();
}

OdArcDimRecomputor::OdArcDimRecomputor()
: m_bIsPartial(false)
, m_dStartParam(0.0)
, m_dEndParam(0.0)
, m_bHasLeader(false)
{
}

OdArcDimRecomputor::~OdArcDimRecomputor()
{
}

void OdArcDimRecomputor::getDimParams(const OdDbDimension* pDim, OdDbDimensionObjectContextData* ctx)
{
  OdDimRecomputor::getDimParams(pDim, ctx);
  OdDbArcDimensionPtr pDimPtr(pDim);
  m_ArcPt = ctx ? ((OdDbAngularDimensionObjectContextData*)ctx)->arcPoint() : pDimPtr->arcPoint();
  m_xLine1Pt = pDimPtr->xLine1Point();
  m_xLine2Pt = pDimPtr->xLine2Point();
  m_CenterPt = pDimPtr->centerPoint();
  m_bIsPartial = pDimPtr->isPartial();
  m_dStartParam = pDimPtr->arcStartParam();
  m_dEndParam = pDimPtr->arcEndParam();
  m_bHasLeader = pDimPtr->hasLeader();
  m_arcSymbType = pDimPtr->arcSymbolType();
}

void OdArcDimRecomputor::setDimParams(OdDbDimension* pDim, OdDbDimensionObjectContextData* ctx) const
{
  OdDimRecomputor::setDimParams(pDim, ctx);
  OdDbArcDimensionPtr pDimPtr(pDim);
  if (ctx)
    ((OdDbAngularDimensionObjectContextData*)ctx)->setArcPoint(m_ArcPt);
  if (!ctx || ctx->isDefaultContextData())
    pDimPtr->setArcPoint(m_ArcPt);
}

OdUInt8 OdArcDimRecomputor::getDimTypeFlag()
{
  return DimArcLen;
}

///

void OdArcDimRecomputor::preprocessDimPoints(OdDbDimStyleTableRecord*)
{
  // empty
}

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

// pLine->transformBy(toHorBack); 

#define ADD_POINT(a) \
  point = OdDbPoint::createObject(); \
  point->setPosition(a); \
  point->setColor(OdCmEntityColor::kByBlock); \
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

#ifdef _DEBUG
#define DEBUG_BUILD_ARC_DIM
#endif


void OdArcDimRecomputor::updateMeasurement(OdDbDimStyleTableRecord* /*pDimVars*/)
{
  OdArcLenRecomputorEngine engine;

  engine.m_centerPtArc = m_CenterPt;
  engine.m_arcPt = m_ArcPt;
  engine.m_xLine1Pt = m_xLine1Pt;
  engine.m_xLine2Pt = m_xLine2Pt;

  OdGeMatrix3d mWorld2Plane(OdGeMatrix3d::worldToPlane(m_vNormal));
  engine.tranformBy(mWorld2Plane);
  m_dMeasurement = engine.measurement();
  m_dMeasurement *= m_dimlfac;
}

void OdArcDimRecomputor::buildDimension(OdArray<OdDbEntityPtr>& ents, OdDbDimStyleTableRecord* pDimVars)
{
  OdDb::LineWeight lineweight = OdDb::LineWeight(getDimlwd(pDimVars));
  OdCmColor color = getDimclrd(pDimVars);
  OdDbObjectId dimLinetype = m_DimLinetypeId;
  double linetypeScale = m_dLinetypeScale;
  OdCmTransparency transparency = m_transparency;

  OdArcLenRecomputorEngine engine;

  engine.m_centerPtArc = m_CenterPt;
  engine.m_arcPt = m_ArcPt;
  engine.m_xLine1Pt = m_xLine1Pt;
  engine.m_xLine2Pt = m_xLine2Pt;
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
    engine.m_bUserDefPos = true;
    engine.m_textPosition = m_ocsDimTextMidPt;
    engine.m_textPosition.z = 0;
    engine.calcDimArcPoint();
    m_ArcPt = engine.m_arcPt;
    m_ArcPt.z = m_ocsDimTextMidPt.z;
    m_ArcPt.transformBy(mPlane2World);
  }

  engine.m_arcSym = m_arcSymbType;

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
  // engine.calcArrowAngles(asz1, asz2);

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
  } else // User defined text position
  {
    engine.m_textPosition = m_ocsDimTextMidPt;
    engine.m_textPosition.z = 0.0;
    engine.m_bUserDefPos = true;
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
    engine.checkArrowsPlace();
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

  OdGeVector3d dirExtLn1 = engine.m_xLine1Dir;
  OdGeVector3d dirExtLn2 = engine.m_xLine2Dir;
  if ( engine.m_angle < OdaPI2 )
  {
    dirExtLn1 = engine.m_middleArcPt - engine.m_centerDimPtArc;
    dirExtLn1.normalize();
    dirExtLn2 = dirExtLn1;
  } 

  if ( buildExtLine1(engine.m_xLine1Pt, engine.m_xLine1PtEnd, ents, pDimVars, 0.0, dirExtLn1) )
    setDimLineIndex(ents.last(), EXT_LINE_1_INDEX);
  if ( buildExtLine2(engine.m_xLine2Pt, engine.m_xLine2PtEnd, ents, pDimVars, 0.0, dirExtLn2) )
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

  if ( engine.m_bAddedArrow1Arc)
  {
    ADD_ARC(engine.m_addedArrow1Arc.startAngFromXAxis(), engine.m_addedArrow1Arc.endAngFromXAxis());
    setDimLineIndex(ents.last(), DIM_LINE_INDEX);
  }
  if ( engine.m_bAddedArrow2Arc)
  {
    ADD_ARC(engine.m_addedArrow2Arc.startAngFromXAxis(), engine.m_addedArrow2Arc.endAngFromXAxis());
    setDimLineIndex(ents.last(), DIM_LINE_INDEX);
  }

  if ( !engine.m_bArrowsInside )
  {
    if ( getDimtofl(pDimVars) )
      engine.m_dimLines = 1;
    else
      bSuppressDimLinesAtAll = true;
  }

  if ( getDimtmove(pDimVars) == 1 && ( engine.m_bTextHasLine || engine.m_bLeader ) )
  {
    bSuppressDimLinesAtAll = false;
    engine.m_dimLines = 1;
  }
  
  engine.prepareDimArc();
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
        ADD_ARC(engine.m_dimArc2StartAngle, engine.m_dimArc2EndAngle );
        setDimLineIndex(ents.last(), DIM_LINE_INDEX);
      }
    }
  }

  if ( !bSuppressDimLinesAtAll && !engine.m_bTextInside && engine.m_dimLines == 1 )
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
    OdGePoint3d pos = engine.m_textPosition;
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
    engine.m_textPosition = pos; // restore for calculate arcsym posision
  } else
  {
    if(pText.get())
    {
      if ( m_BgrndTxtFlag && engine.m_arcSym < 2 )
      {
        OdDbDatabase* pDb = database();
        if ( engine.m_arcSym == 0 )
        {
          if ( pDb && pDb->getMEASUREMENT() == OdDb::kMetric )
            pText->setBackgroundScaleFactor(-1.1);
          else
            pText->setBackgroundScaleFactor(-1.25);
        }
        else 
        {
          if ( pDb && pDb->getMEASUREMENT() == OdDb::kMetric )
            pText->setBackgroundScaleFactor(-2.2);
          else
            pText->setBackgroundScaleFactor(-2.5);
        }
      }

      ents.push_back(pText);
      buildBoundaryBox(pDimVars, pText, ents);
    }
  }

  if ( engine.m_arcSym < 2 && !pText.isNull() )
  {
    engine.fitArcParam(pText);
    OdGePoint2d Pt2d = engine.m_arcInText.center();
    centerPoint.x = Pt2d.x;
    centerPoint.y = Pt2d.y;
    double startAngl = engine.m_arcInText.startAngFromXAxis();
    double endAngl = engine.m_arcInText.endAngFromXAxis();

    OdDbArcPtr pArc = OdDbArc::createObject(); 
    pArc->setLineWeight(lineweight); 
    pArc->setColor(pText->color()); 
    pArc->setTransparency(m_transparency);

    pArc->setCenter(centerPoint); 
    pArc->setRadius(engine.m_arcInText.radius());
    pArc->setStartAngle((startAngl > Oda2PI) ? startAngl - Oda2PI : startAngl );
    pArc->setEndAngle((endAngl > Oda2PI) ? endAngl - Oda2PI : endAngl );
    ents.push_back(pArc);
  }

  if ( m_bHasLeader && engine.m_angle > OdaPI2 && engine.m_radiusDimArc > engine.m_sourceArcRadius )
  {
    // Create leader here
    engine.addGapToTextExtents(false);
    OdGePoint3d minPt = engine.m_textExtentsWithGap.minPoint();
    OdGePoint3d maxPt = engine.m_textExtentsWithGap.maxPoint();
    OdGeMatrix2d matr = OdGeMatrix2d::rotation(pText->rotation(), OdGePoint2d(engine.m_textPosition.x, engine.m_textPosition.y));

    OdGeLineSeg2d line;
    OdArray<OdGePoint2d> points;
    points.resize(4);

    line.set(OdGePoint2d(minPt.x, minPt.y), OdGePoint2d(minPt.x, maxPt.y));
    points[0] = line.midPoint();
    line.set(OdGePoint2d(minPt.x, maxPt.y), OdGePoint2d(maxPt.x, maxPt.y));
    points[1] = line.midPoint();
    line.set(OdGePoint2d(maxPt.x, maxPt.y), OdGePoint2d(maxPt.x, minPt.y));
    points[2] = line.midPoint();
    line.set(OdGePoint2d(maxPt.x, minPt.y), OdGePoint2d(minPt.x, minPt.y));
    points[3] = line.midPoint();

    OdGePoint2d cent2d(engine.m_centerDimPtArc.x, engine.m_centerDimPtArc.y);
    points[0].transformBy(matr);
    double minDist = cent2d.distanceTo(points[0]);
    OdGePoint2d point = points[0];
    for ( OdInt32 i = 1; i < 4; i++ )
    {
      points[i].transformBy(matr);
      double dist = cent2d.distanceTo(points[i]);
      if ( minDist > dist  )
      {
        minDist = dist;
        point = points[i];
      }
    }

    OdDbLinePtr pLine;
    OdGeVector3d v2arc = engine.m_centerDimPtArc - engine.m_textPosition;
    v2arc.normalize();
    OdGePoint3d textPoint(point.x, point.y, 0.0);
    OdGePoint3d pointOnArc = engine.m_textPosition + v2arc*(engine.m_radiusDimArc - engine.m_sourceArcRadius);
    double gap = getDimgap(pDimVars);

    if ( engine.m_dimLines == 1 && engine.m_bTextInside )
    {
      if ( OdNegative(gap) )
      {
        textPoint -= v2arc*gap;
        pointOnArc = textPoint + v2arc*(engine.m_radiusDimArc - engine.m_sourceArcRadius);
      } else
      {
        pointOnArc = textPoint + v2arc*(engine.m_radiusDimArc - engine.m_sourceArcRadius);
      }
    }

    ADD_LINE(textPoint, pointOnArc);

    OdGeVector3d dirArrow = textPoint - pointOnArc;
    dirArrow.normalize();

    buildArrow1(pointOnArc, dirArrow, ents, pDimVars);
  }
  // Correct
  engine.correctDimArcPoint();
  m_ArcPt = engine.m_arcPt;
  m_ArcPt.z = m_ocsDimTextMidPt.z;
  m_ArcPt.transformBy(mPlane2World);

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
