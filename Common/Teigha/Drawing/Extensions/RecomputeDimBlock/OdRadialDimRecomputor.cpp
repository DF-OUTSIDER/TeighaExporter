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
#include "OdRadialDimRecomputor.h"
#include "RxObjectImpl.h"
#include "DbObjectContextData.h"


#include "Ge/GeMatrix3d.h"
#include "Ge/GeExtents3d.h"
#include "Ge/GeLine2d.h"

#include "DbRadialDimension.h"
#include "DbLine.h"
#include "DbPoint.h"
#include "DbArc.h"

#include "OdRadialRecomputorEngine.h"

OdRadialDimRecomputor::OdRadialDimRecomputor()
: m_dLeaderLen(0.0)
, m_bExtArcOn(false)
, m_dExtArcStartAngle(0.0)
, m_dExtArcEndAngle(0.0)
, m_bNeedCorrectTextPos(false)
{
}

OdRadialDimRecomputor::~OdRadialDimRecomputor()
{
}

OdUInt8 OdRadialDimRecomputor::getDimTypeFlag()
{
  return DimRadial;
}
  
OdDimRecomputorPtr OdDbRadialDimRecomputePEImpl::createRecomputor()
{
  return OdRxObjectImpl<OdRadialDimRecomputor>::createObject();
}

void OdRadialDimRecomputor::getDimParams(const OdDbDimension* pDim, OdDbDimensionObjectContextData* ctx)
{
  OdDimRecomputor::getDimParams(pDim, ctx);
  OdDbRadialDimensionPtr pDimPtr(pDim);
  m_DefPointCenter = ctx ? ((OdDbRadialDimensionObjectContextData*)ctx)->chordPoint(): pDimPtr->chordPoint();
  m_dLeaderLen = pDimPtr->leaderLength();
  m_DimLineDefPt = pDimPtr->center();
  m_bExtArcOn = pDimPtr->extArcOn();
  m_dExtArcStartAngle = pDimPtr->extArcStartAngle();
  m_dExtArcEndAngle = pDimPtr->extArcEndAngle();
}

void OdRadialDimRecomputor::setDimParams(OdDbDimension* pDim, OdDbDimensionObjectContextData* ctx) const
{
  OdDimRecomputor::setDimParams(pDim, ctx);
  OdDbRadialDimensionPtr pDimPtr(pDim);
  if ( ctx )
    ((OdDbRadialDimensionObjectContextData*)ctx)->setChordPoint(m_DefPointCenter);
  else
    pDimPtr->setChordPoint(m_DefPointCenter);
  // pDimPtr->setCenter(m_DimLineDefPt);
  if ( m_bNeedCorrectTextPos )
  {
    pDimPtr->useSetTextPosition();
    pDimPtr->setLeaderLength(m_dLeaderLen);
  }
}

///
void OdRadialDimRecomputor::formatDimStrWithLimsAndTols(OdString& measurement, bool isLims, bool isTols,
                                                        double dMeasurement, OdInt16 dimLunit,
                                                        double dimTm, double dimTp, OdInt16 dimTzin,
                                                        double dimTfac, OdInt16 dimTdec, double dimRnd,
                                                        OdChar dimDsep, OdInt16 dimFrac, double dimTxt,
                                                        OdInt16 dimZin, OdInt16 dimDec, OdString dimPost,
                                                        bool bPrimaryUnits)
{
  OdDimRecomputor::formatDimStrWithLimsAndTols(measurement, isLims, isTols, dMeasurement, dimLunit,
    dimTm, dimTp, dimTzin, dimTfac, dimTdec, dimRnd, dimDsep, dimFrac, dimTxt, dimZin, dimDec, dimPost, bPrimaryUnits);

  // GU 18.11.2004 cr 2382
  // if dimpost contains quotes we shouldn't
  // append any default prefixes
  int numQuotesPost = dimPost.find(OD_T("<>"));  // dimpost - primary units 
  int numQuotesApost = dimPost.find(OD_T("[]")); // dimapost - alternate units
  if ( ( bPrimaryUnits && numQuotesPost == -1) || ( !bPrimaryUnits && numQuotesApost == -1 ) )
  {
    if(getDimTypeFlag() == DimDiametric)
      measurement = OD_T("%%c") + measurement;
    else
      measurement = OD_T("R") + measurement;
  }
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

// Updates measurement value
void OdRadialDimRecomputor::updateMeasurement(OdDbDimStyleTableRecord* /*pDimVars*/)
{
  m_dMeasurement = m_DimLineDefPt.distanceTo(m_DefPointCenter);
  m_dMeasurement *= m_dimlfac;
}

void OdRadialDimRecomputor::preprocessDimPoints(OdDbDimStyleTableRecord* /*pDimVars*/)
{
  return;
#if 0
  // 12.09.03 GU
  // in case m_bUserDefinedTextPosition
  // m_dLeaderLen isn't used and AutoCAD doesn't change it.
  double leaderLenLen = fabs(m_dLeaderLen);
  double asz = getDimasz(pDimVars);
  OdGeMatrix3d mWorld2Plane(OdGeMatrix3d::worldToPlane(m_vNormal));
  if((leaderLenLen < 2.0 * asz) && !m_bUserDefinedTextPosition)
  {
    double newLeadLn = 2.0 * getDimasz(pDimVars);
    if(OdNonZero(leaderLenLen))
      newLeadLn *= m_dLeaderLen / leaderLenLen;
    m_dLeaderLen = newLeadLn;
  }
  if(m_bUserDefinedTextPosition)
  {
    // text position although should be preprocessed (GU, cr2398)
    OdGePoint3d centerPoint(m_DimLineDefPt);
    OdGePoint3d textPoint(m_ocsDimTextMidPt);
    OdGePoint3d endArrowPoint(m_DefPointCenter);

    centerPoint.transformBy(mWorld2Plane);
    centerPoint.z = 0.0;
    // textPoint.transformBy(mWorld2Plane);
    textPoint.z = 0.0;
    endArrowPoint.transformBy(mWorld2Plane);
    endArrowPoint.z = 0;

    double radius(centerPoint.distanceTo(endArrowPoint));
    if(getDimTypeFlag() == DimDiametric)
      radius /= 2.0;

    bool bTextInside = (textPoint.distanceTo(centerPoint) < radius);
    if(getDimTypeFlag() == DimDiametric)
    {
      // NB: for diametric dimensions centerPoint isn't really a center
      // issues on \owf\dimensions\radial\1.dwg
      bTextInside = (textPoint.distanceTo(OdGePoint3d()+(endArrowPoint.asVector()+centerPoint.asVector())/2.0) < radius);
    }

    if((bTextInside && !getDimtih(pDimVars)) || (!bTextInside && !getDimtoh(pDimVars)))
    {
      OdGeLine2d radialLine(centerPoint.convert2d(), endArrowPoint.convert2d());
      OdGeLine2d perpLine(textPoint.convert2d(), radialLine.direction().perpVector());
      OdGePoint2d intPt;

      if(perpLine.intersectWith(radialLine, intPt))
      {
        m_ocsDimTextMidPt.x = intPt.x;
        m_ocsDimTextMidPt.y = intPt.y;
      }
    }
    else // if ( getDimtmove(pDimVars) != 2 ) // cr2452
    {
      OdGeVector3d e_c(endArrowPoint-centerPoint);
      if(OdNonZero(e_c.y))
      {
        e_c.normalize();
        OdGePoint3d dogLegPoint = endArrowPoint + e_c*(bTextInside ? -1.0 : 1.0) * 2.0 * asz;
        bool b2Up(false);
        bool eapDownerCenter = (endArrowPoint.y < centerPoint.y);
        b2Up = bTextInside ? eapDownerCenter : !eapDownerCenter;
        if((dogLegPoint.y > textPoint.y) && b2Up)
        {
          m_ocsDimTextMidPt.y = (dogLegPoint + asz*(bTextInside ? -1.0 : 1.0)*((e_c.x > 0) ? 1.0 : -1.0)*OdGeVector3d::kXAxis).y;
        }
        if((dogLegPoint.y < textPoint.y) && !b2Up)
        {
          m_ocsDimTextMidPt.y = (dogLegPoint - asz*(bTextInside ? -1.0 : 1.0)*((e_c.x > 0) ? 1.0 : -1.0)*OdGeVector3d::kXAxis).y;
        }
        // m_ocsDimTextMidPt.z holds elevation
      }
    }
  }
#endif
}

void OdRadialDimRecomputor::buildDimension(OdArray<OdDbEntityPtr>& ents,
                                           OdDbDimStyleTableRecord* pDimVars)
{
  OdGeMatrix3d mWorld2Plane(OdGeMatrix3d::worldToPlane(m_vNormal));
  
  OdCmTransparency transparency = m_transparency;
  OdRadialRecomputorEngine engine;

  engine.m_xLine1Pt = m_DimLineDefPt;
  engine.m_xLine2Pt = m_DefPointCenter;
  engine.m_dimOcsTextPosition = m_ocsDimTextMidPt;
  engine.m_dimOcsTextPosition.z = 0;
  engine.m_dLeaderLen = m_dLeaderLen;

  engine.tranformBy(mWorld2Plane);
  getUCS(engine.m_xAxis, engine.m_yAxis);

  OdDbMTextPtr pText;
  createTextEntityForDimGraphic(pText, pDimVars);

  OdUInt16 just = getDimjust(pDimVars);
  OdUInt16 tad = getDimtad(pDimVars);
  double gap = getDimgap(pDimVars);

  OdDbMTextPtr pLabel, pRate;
  if ( m_bInspection )
  {
    engine.setInspection( (m_nInspectionFrame & OdDbDimension::kShapeLabel) != 0, 
      (m_nInspectionFrame & OdDbDimension::kShapeRate) != 0, 
      (m_nInspectionFrame & (OdDbDimension::kShapeRound | OdDbDimension::kShapeAngular ) ) != 0 );

    createInspectionEntitisForDimGraphic(pLabel, pRate, pDimVars);
  }

  engine.m_textPosition = m_ocsDimTextMidPt;
  engine.m_textPosition.z = 0;
  engine.m_bRadial = (getDimTypeFlag() == DimRadial);

  // Dimension arrow blocks
  OdDbObjectId dimBlk1, dimBlk2;
  if(getDimsah(pDimVars))
  {
    dimBlk1 = getDimblk1(pDimVars);
    dimBlk2 = getDimblk2(pDimVars);
  }
  else
  {
    dimBlk2 = dimBlk1 = getDimblk(pDimVars);
  }
  double asz1, asz2, asz;
  asz1 = asz2 = asz = getDimasz(pDimVars);

  double tszVar = getDimtsz(pDimVars);
  double sizeStroke = tszVar; // *getDimscale(pDimVars); 

  if ( (dimBlk1.isNull() && OdNonZero(tszVar)) || toExtend(dimBlk1) )
  {
    asz1 = 0.0;
  }

  if ( (dimBlk2.isNull() && OdNonZero(tszVar)) || toExtend(dimBlk2) )
  {
    asz2 = 0.0;
  }
  if ( OdNonZero(tszVar) && (dimBlk1.isNull() || dimBlk2.isNull()) )
    asz = sizeStroke*2.0;
  sizeStroke /= ::cos(OdaPI4);

  engine.setAszParams(asz1, asz2, asz, m_bArrowSecondIsFlipped, m_bArrowFirstIsFlipped);

  engine.m_dimTih = getDimtih(pDimVars);
  engine.m_dimToh = getDimtoh(pDimVars);
  engine.m_dimTvp = getDimtvp(pDimVars);
  engine.m_dimLineDir = engine.m_xLine2Pt - engine.m_xLine1Pt;
  if ( engine.m_dimLineDir.isZeroLength() )
    engine.m_dimLineDir = engine.m_xAxis;
  else
    engine.m_dimLineDir.normalize();
  engine.m_atfit = getDimatfit(pDimVars);

  if (OdNonZero(m_dTextRot))
  {
    engine.m_textDirection = engine.m_xAxis;
    engine.m_textDirection.rotateBy(m_dTextRot, OdGeVector3d::kZAxis);
    if ( !pText.isNull() )
    {
      pText->setDirection(engine.m_textDirection);
    }
    engine.m_dTextRot = m_dTextRot;
    engine.m_bUseRotate = true;
  } else
    engine.m_bUseRotate = false;

  OdDbMTextPtr pAddedText;

  if ( !pText.isNull() )
  {
    pAddedText = engine.createXText(pText, getDimtmove(pDimVars), m_bUserDefinedTextPosition, m_dTextRot);
    engine.calcTextExtents(pText);
    engine.m_textWidth = (engine.m_textExtents.maxPoint().x - engine.m_textExtents.minPoint().x);
    if ( m_bInspection )
    {
      engine.calcInspectionExtents(pLabel, pRate, true);
    }
    if (!pAddedText.isNull())
    {
      OdGeExtents3d exts;
      pAddedText->setDirection(OdGeVector3d::kXAxis);
      pAddedText->getGeomExtents(exts);
      engine.m_dXTextHeight = exts.maxPoint().y - exts.minPoint().y;
      engine.m_textHeight = (engine.m_textExtents.maxPoint().y - engine.m_textExtents.minPoint().y);
      engine.m_textWidth = odmax((engine.m_textWidth), (exts.maxPoint().x - exts.minPoint().x));
      gap = ::fabs(gap);
    }
  }
  engine.setTextParams(just, tad, gap);

  engine.calcCenterPoint();
  engine.calcRadius();
  OdInt16 dm = getDimtmove(pDimVars);
  engine.m_dimMove = dm;
  engine.m_tix = getDimtix(pDimVars);
  bool bNeedPreprocess = true;
  if ( OdNonZero(m_dLeaderLen) && !m_bUserDefinedTextPosition )
  {
    engine.applyLeaderLen();

    m_bNeedCorrectTextPos = true;
    engine.m_dimOcsTextPosition.z = m_ocsDimTextMidPt.z;
    m_ocsDimTextMidPt = engine.m_dimOcsTextPosition;

    m_bUserDefinedTextPosition = true;
    m_dLeaderLen = 0.0;
    bNeedPreprocess = false;
  }

  if ( !m_bUserDefinedTextPosition )
  {
    engine.adjustTextAndArrowsPlace(dm, getDimatfit(pDimVars), getDimtix(pDimVars), getDimtofl(pDimVars) );
    engine.adjustTextLocation(dm, 0.0);
    engine.makeLines(getDimtofl(pDimVars));
    engine.m_dimOcsTextPosition.z = m_ocsDimTextMidPt.z;
    m_ocsDimTextMidPt = engine.m_dimOcsTextPosition;
  } else
  {
    engine.m_bUserDefPos = true;
    engine.textLocation(dm);
    if ( engine.m_bRadial && dm == 0 && engine.m_atfit != 3 && !engine.m_bTextHor && !engine.m_bUseRotate )
    {
      OdGeVector3d v = engine.m_dimOcsTextPosition - engine.m_xLine1Pt;
      v.normalize();

      OdGeVector3d yUnit = OdGeVector3d::kZAxis.crossProduct(v).normalize();
      OdGeMatrix3d cordSys;
      cordSys.setCoordSystem(engine.m_xLine1Pt, v, yUnit, OdGeVector3d::kZAxis);
      cordSys.invert();
      OdGePoint3d p1 = engine.m_dimOcsTextPosition;
      OdGePoint3d p2 = engine.m_xLine2Pt;
      p1.transformBy(cordSys);
      p2.transformBy(cordSys);

      if ( p1.x > 0.0 && p2.x < 0.0 )
        v *= -1.0;

      engine.m_xLine2Pt = engine.m_xLine1Pt + v*engine.m_radius;

      engine.m_dimLineDir = engine.m_xLine2Pt - engine.m_xLine1Pt;
      if (engine.m_dimLineDir.isZeroLength())
        engine.m_dimLineDir = engine.m_xAxis;
      else
        engine.m_dimLineDir.normalize();

      OdGeMatrix3d mPlane2World(OdGeMatrix3d::planeToWorld(m_vNormal));

      m_DefPointCenter = engine.m_xLine2Pt;
      m_DefPointCenter.transformBy(mPlane2World);
    }
    engine.preprocessDimPoints(dm);
    if ( dm != 2 )
    {
      engine.adjustUserDefText(dm, asz, getDimexe(pDimVars));

      bool bExtArc = m_bExtArcOn && getDimse1(pDimVars);
      engine.adjustArrowAndLines(getDimtofl(pDimVars), bExtArc, m_dExtArcStartAngle, m_dExtArcEndAngle);

      m_bNeedCorrectTextPos = true;
      engine.m_dimOcsTextPosition.z = m_ocsDimTextMidPt.z;
      m_ocsDimTextMidPt = engine.m_dimOcsTextPosition;
    }
  }

  engine.adjustFlipArrows();
  // Dimension lines
  OdCmColor color = getDimclrd(pDimVars);
  OdDb::LineWeight lineweight = OdDb::LineWeight(getDimlwd(pDimVars));
  OdDbObjectId dimLinetype = m_DimLinetypeId;
  double linetypeScale = m_dLinetypeScale;
  OdDbLinePtr pLine;

  bool bDrawDimLine = true;
  if ( engine.m_bRadial && getDimsd2(pDimVars) )
  {
    if ( getDimsd2(pDimVars) )
      bDrawDimLine = false;
  } else // diametric
  {
    if ( getDimsd1(pDimVars) && getDimsd2(pDimVars) )
      bDrawDimLine = false;
  }

  if ( bDrawDimLine) // !getDimsd1(pDimVars) || ( engine.m_bRadial && !getDimsd2(pDimVars)))
  {
    if ( engine.m_bTextHasLine )
    {
      ADD_LINE(engine.m_textLinePt2, engine.m_textLinePt1);
      setDimLineIndex(ents.last(), DIM_LINE_INDEX);
    }
    if ( engine.m_bTail1 )
    {
      ADD_LINE(engine.m_tail1StartPt, engine.m_tail1EndPt);
      setDimLineIndex(ents.last(), DIM_LINE_INDEX);
    }
    if ( engine.m_bTail2 )
    {
      ADD_LINE(engine.m_tail2StartPt, engine.m_tail2EndPt);
      setDimLineIndex(ents.last(), DIM_LINE_INDEX);
    }
    if ( engine.m_bflipedTail )
    {
      ADD_LINE(engine.m_flipedTailStartPt, engine.m_flipedTailEndPt);
      setDimLineIndex(ents.last(), DIM_LINE_INDEX);
    }

    if ( engine.m_dimLines > 0 )
    {
      ADD_LINE(engine.m_xLine1PtEnd, engine.m_xLine1Pt);
      setDimLineIndex(ents.last(), DIM_LINE_INDEX);
    }
    if ( engine.m_dimLines == 2 )
    {
      ADD_LINE(engine.m_xLine2PtEnd, engine.m_xLine2Pt);
      setDimLineIndex(ents.last(), DIM_LINE_INDEX);
    }
  }

  // Make Inspection
  if ( m_bInspection )
  {
    OdGePoint3d pos = engine.m_textPosition;
    makeInspectionFrame(pDimVars, &engine, ents);
    if ( !pText.isNull() )
    {
      pText->setLocation(engine.m_textPosition);
      pText->setDirection(engine.m_textDirection);
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
    if ( !pText.isNull() )
    {
      if (OdNonZero(m_dTextRot) )
      {
        engine.m_textDirection = engine.m_xAxis;
        engine.m_textDirection.rotateBy(m_dTextRot, OdGeVector3d::kZAxis);
      }
      pText->setDirection(engine.m_textDirection);
      pText->setLocation(engine.m_textPosition);
      ents.push_back(pText);

      if (!pAddedText.isNull())
      {
        OdArray<OdGePoint3d> points;
        gap = getDimgap(pDimVars);
        engine.setTextParams(just, tad, gap);

        engine.applyXText(pAddedText, points);

        OdUInt8 tolj = getDimtolj(pDimVars);
        if (tolj != 0)
        {
          OdString toljAdd;
          toljAdd.format(OD_T("\\A%i;"), (int)tolj);
          OdString contStr = toljAdd + pAddedText->contents();
          pAddedText->setContents(contStr);
        }

        ents.push_back(pAddedText);

        if (getDimgap(pDimVars) < 0.0)
        {
          ADD_LINE(points[0], points[1]);
          ADD_LINE(points[1], points[2]);
          ADD_LINE(points[2], points[3]);
          ADD_LINE(points[3], points[0]);
        }
      } else
        buildBoundaryBox(pDimVars, pText, ents);
    }
  }

  if( ( !engine.m_bTextInside && !engine.m_bFarOutsideText && !getDimsd1(pDimVars) && getDimtofl(pDimVars) && !engine.m_bDogLegInside ) 
    || (!engine.m_bRadial && bDrawDimLine && engine.m_dimLines == 0 && getDimtofl(pDimVars) && !engine.m_bTextInside) )
  {
    ADD_LINE(engine.m_arrow2Pt, engine.m_arrow1Pt);
    setDimLineIndex(ents.last(), DIM_LINE_INDEX);
  }

  if ( engine.m_bArrow2Present )
  {
    if ( dimBlk1.isNull() && OdNonZero(tszVar) )
    {
      OdGeVector3d v = engine.m_dirArrow2;
      v.rotateBy(OdaPI4, OdGeVector3d::kZAxis);
      OdGePoint3d p1, p2;
      p1 = engine.m_arrow2Pt + v*sizeStroke;
      p2 = engine.m_arrow2Pt - v*sizeStroke;
      ADD_LINE(p1, p2);
      setDimLineIndex(ents.last(), DIM_LINE_INDEX);
    } else
    {
      if ( !getDimsd1(pDimVars) || !getDimsd2(pDimVars) )
      {
        buildArrow(engine.m_arrow2Pt,  engine.m_dirArrow2 , ents, pDimVars, dimBlk1);
      }
    }
  }

  if ( dimBlk2.isNull() && OdNonZero(tszVar) )
  {
    OdGeVector3d v = engine.m_dirArrow1;
    v.rotateBy(OdaPI4, OdGeVector3d::kZAxis);
    OdGePoint3d p1, p2;
    p1 = engine.m_arrow1Pt + v*sizeStroke;
    p2 = engine.m_arrow1Pt - v*sizeStroke;
    ADD_LINE(p1, p2);
    setDimLineIndex(ents.last(), DIM_LINE_INDEX);
  } else
  {
    if ( engine.m_bRadial )
    {
      if ( !getDimsd2(pDimVars) )
      {
        buildArrow(engine.m_arrow1Pt,  engine.m_dirArrow1 , ents, pDimVars, dimBlk2);
      }
    } else if ( !getDimsd1(pDimVars) || !getDimsd2(pDimVars) )
    {
      buildArrow(engine.m_arrow1Pt,  engine.m_dirArrow1 , ents, pDimVars, dimBlk2);
    }

  }
  if ( engine.isCenterMark() )
  {
    OdDb::LineWeight lineweight = (OdDb::LineWeight)m_nLineweight;
    // Need to check 
    if(!getDimtofl(pDimVars))
    {
      color = getDimclre(pDimVars);
      double cen = getDimcen(pDimVars);
      OdGePoint3d center = engine.m_centerPt;
      OdGeVector3d XDir = engine.m_xAxis, YDir = engine.m_yAxis;
      if(cen > 0)
      {
        OdGeVector3d XDir1 = XDir, YDir1 = YDir;
        XDir1 *= -1.0;
        YDir1 *= -1.0;
        // Drawing center marks
        ADD_LINE(center + XDir1*cen, center + XDir*cen);
        ADD_LINE(center + YDir1*cen, center + YDir*cen);
      }
      if(cen < 0)
      {
        // Drawing center lines
        cen = -cen;
        ADD_LINE(center - XDir*cen, center + XDir*cen);
        ADD_LINE(center - YDir*cen, center + YDir*cen);
        double rad = center.distanceTo(engine.m_xLine2Pt) + cen;
        ADD_LINE(center - 2.0*cen*XDir, center - rad*XDir);
        ADD_LINE(center + 2.0*cen*XDir, center + rad*XDir);
        ADD_LINE(center - 2.0*cen*YDir, center - rad*YDir);
        ADD_LINE(center + 2.0*cen*YDir, center + rad*YDir);
      }
    } else
    {
      /*
      // Dimension line inside circle

      // 24.07.2003 G.Udov
      // seems that sometimes we should supress this line for diametric dimension
      if(!((getDimTypeFlag() == DimDiametric) && (
        textPoint.distanceTo(centerPoint) < textPoint.distanceTo(endArrowPoint))))
      {
        ADD_LINE(centerPoint, endArrowPoint);
      }
      */
    }
  }

  if ( m_bExtArcOn  && !getDimse1(pDimVars) )
  {
    if ( engine.calcExtensionArc(m_dExtArcStartAngle, m_dExtArcEndAngle, getDimexe(pDimVars), getDimexo(pDimVars)) )
    {
      OdGePoint3d centerPoint = engine.m_centerPt;
      double radius = engine.m_radius;
      OdDbArcPtr pDimArc;

      color = getDimclre(pDimVars);
      lineweight = OdDb::LineWeight(getDimlwd(pDimVars));
      dimLinetype = getDimltex1(pDimVars);
      ADD_ARC(engine.m_dStartExtAng, engine.m_dEndExtAng);
    }
  }
  if ( !pText.isNull() )
  {
    pText->setAttachmentMovingLocation((OdDbMText::AttachmentPoint)m_AttachPt);
  } 
  // Points
  color.setColorMethod(OdCmEntityColor::kByBlock);
  OdDbPointPtr point;
  OdDbObjectId defPointsId = getLayerDefpointsId();
  ADD_POINT(engine.m_arrow1Pt);
  ADD_POINT(engine.m_arrow2Pt);
}

void OdRadialDimRecomputor::formatMeasurement(OdString& measurement, OdDbDimStyleTableRecord* pDimVars)
{
  formatDimStrWithAltUnits(measurement, pDimVars);
}
