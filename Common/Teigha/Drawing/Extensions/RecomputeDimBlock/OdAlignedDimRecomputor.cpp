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
#include "RxObjectImpl.h"

#include "Ge/GeMatrix3d.h"
#include "Ge/GeExtents3d.h"

#include "DbTextStyleTableRecord.h"
#include "DbAlignedDimension.h"
#include "OdAlignedRecomputorEngine.h"
#include "DbDimStyleTableRecord.h"
#include "DbObjectContextData.h"

#include "DbLine.h"
#include "DbPoint.h"

#define STL_USING_ALGORITHM
#include "OdaSTL.h"

OdDimRecomputorPtr OdDbAlignedDimRecomputePEImpl::createRecomputor()
{
  return OdRxObjectImpl<OdAlignedDimRecomputor>::createObject();
}

OdAlignedDimRecomputor::OdAlignedDimRecomputor()
: m_dOblique(0.0)
, m_bJogSymbolOn(false)
, m_JogSymbolHeight(5.0)
, m_JogSymbolUserDefPos(true)
{
}

OdAlignedDimRecomputor::~OdAlignedDimRecomputor()
{
}

OdUInt8 OdAlignedDimRecomputor::getDimTypeFlag()
{
  return DimAligned;
}

void OdAlignedDimRecomputor::getDimParams(const OdDbDimension* pDim, OdDbDimensionObjectContextData* ctx)
{
  OdDimRecomputor::getDimParams(pDim, ctx);
  OdDbAlignedDimensionPtr pDimPtr(pDim);

  m_DefPoint1 = pDimPtr->xLine1Point();
  m_DefPoint2 = pDimPtr->xLine2Point();
  m_DimLineDefPt = ctx ? ((OdDbAlignedDimensionObjectContextData*)ctx)->dimLinePoint() : pDimPtr->dimLinePoint();
  m_dOblique = pDimPtr->oblique();
  if ( ::fabs(m_dOblique) > 1.0E+17 )
  {
    m_dOblique = 0.0;
  }
  m_bJogSymbolOn = pDimPtr->jogSymbolOn();
  m_pJogPosition = pDimPtr->jogSymbolPosition(); 
  m_JogSymbolHeight = pDimPtr->jogSymbolHeight();
}

void OdAlignedDimRecomputor::setDimParams(OdDbDimension* pDim, OdDbDimensionObjectContextData* ctx) const
{
  OdDimRecomputor::setDimParams(pDim, ctx);
  OdDbAlignedDimensionPtr pDimPtr(pDim);
  if (!ctx || ctx->isDefaultContextData())
    pDimPtr->setDimLinePoint(m_DimLineDefPt);
  if (ctx)
    ((OdDbAlignedDimensionObjectContextData*)ctx)->setDimLinePoint(m_DimLineDefPt);

  if ( m_bJogSymbolOn )
    pDimPtr->setJogSymbolPosition(m_pJogPosition); // correct jog position

  /*
  pDimPtr->setXLine1Point(m_DefPoint1);
  pDimPtr->setXLine2Point(m_DefPoint2);
  // pDimPtr->setOblique(m_dOblique);
  */
}

void OdAlignedDimRecomputor::calculateExtLinePoints(OdGePoint3d& extLine1Start, 
                                                    OdGePoint3d& extLine1End,
                                                    OdGePoint3d& extLine2Start, 
                                                    OdGePoint3d& extLine2End,
                                                    const OdGePoint3d& dimLineDefPt, 
                                                    const OdGePoint3d& defPoint1, 
                                                    const OdGePoint3d& defPoint2)
{
  extLine1Start = defPoint1;
  extLine2Start = defPoint2;
  extLine2End   = dimLineDefPt;
  extLine1End   = extLine1Start + (extLine2End.asVector() - extLine2Start.asVector());
}

inline bool isInsideExts(OdGePoint3d point, OdDbMText* pText)
{
  OdGeVector3d textDir = pText->direction();
  pText->setDirection(OdGeVector3d::kXAxis);
  OdGeExtents3d exts;
  pText->getGeomExtents(exts);
  pText->setDirection(textDir);
  point.transformBy(OdGeMatrix3d::rotation(-OdGeVector3d::kXAxis.angleTo(textDir, OdGeVector3d::kZAxis),
    OdGeVector3d::kZAxis, pText->location()));
  if(point.x > exts.minPoint().x && point.x < exts.maxPoint().x && point.y > exts.minPoint().y &&
    point.y < exts.maxPoint().y)
  {
    return true;
  }
  else
  {
    return false;
  }
}
void OdAlignedDimRecomputor::formatMeasurement(OdString& measurement, /*OdString& addedText, */OdDbDimStyleTableRecord* pDimVars)
{
  formatDimStrWithAltUnits(measurement, pDimVars);

  /*
  // working with "\X" symbol -- G. Udov, 29.07.2003
  int xPos = measurement.find("\\X");
  if(xPos != -1)
  {
    measurement.setAt(xPos+1,'P');
  }
  */
}

#define ADD_POINT(a) \
  point = OdDbPoint::createObject(); \
  point->setPosition(a); \
  point->setColorIndex(0); \
  point->setLayer(defPointsId); \
  point->setTransparency(transparency); \
  ents.push_back(point);

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


void OdAlignedDimRecomputor::buildDimension(OdArray<OdDbEntityPtr>& ents,
                                            OdDbDimStyleTableRecord* pDimVars)
{
  OdDb::LineWeight lineweight = OdDb::LineWeight(getDimlwd(pDimVars));
  OdCmColor color = getDimclrd(pDimVars);
  OdDbObjectId dimLinetype = m_DimLinetypeId;
  double linetypeScale = m_dLinetypeScale;
  OdCmTransparency transparency = m_transparency;

  OdAlignedRecomputorEngine engine;

  engine.m_dimLineDefPt = m_DimLineDefPt;
  engine.m_xLine1Pt = m_DefPoint1;
  engine.m_xLine2Pt = m_DefPoint2;

  OdGeMatrix3d mWorld2Plane(OdGeMatrix3d::worldToPlane(m_vNormal));
  OdGeMatrix3d mPlane2World(OdGeMatrix3d::planeToWorld(m_vNormal));

  engine.tranformBy(mWorld2Plane);
  getUCS(engine.m_xAxis, engine.m_yAxis);

  OdUInt16 just = getDimjust(pDimVars);
  OdUInt16 tad = getDimtad(pDimVars);

  OdDbMTextPtr pText;
  createTextEntityForDimGraphic(pText, pDimVars);

  OdDbMTextPtr pAddedText;

  if ( !pText.isNull() )
  {
     engine.m_dTextRot = m_dTextRot;
     pAddedText = engine.createXText(pText, getDimtmove(pDimVars), m_bUserDefinedTextPosition, m_dTextRot);
  }

  double gap = getDimgap(pDimVars);
  if ( !pAddedText.isNull() )
    gap = ::fabs(gap);
  engine.setTextParams(just, tad, gap);
  if ( m_bInspection )
  {
    engine.setInspection( (m_nInspectionFrame & OdDbDimension::kShapeLabel) != 0, 
      (m_nInspectionFrame & OdDbDimension::kShapeRate) != 0, 
      (m_nInspectionFrame & (OdDbDimension::kShapeRound | OdDbDimension::kShapeAngular ) ) != 0 );
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

  double tszVar = getDimtsz(pDimVars);
  double sizeStroke = tszVar; // *getDimscale(pDimVars); 

  OdDbObjectId arrowID = pDimVars->arrowId(OdDb::kFirstArrow);

  if ( OdNonZero(tszVar) || toExtend(dimBlk1) )
  {
    asz1 = 0.0;
  }

  if ( OdNonZero(tszVar) || toExtend(dimBlk2) )
  {
    asz2 = 0.0;
  }
  if ( OdNonZero(tszVar) )
  {
    asz = sizeStroke*2.0;
  }
  sizeStroke /= ::cos(OdaPI4);

  engine.setAszParams(asz1, asz2, asz, m_bArrowFirstIsFlipped, m_bArrowSecondIsFlipped);

  bool sd1 = getDimsd1(pDimVars);
  bool sd2 = getDimsd2(pDimVars);
  if ( sd1 && sd2 )
    engine.m_bSuppressDimLine = false;
  else if ( sd1 || sd2 )
    engine.m_bSuppressDimLine = true;

  if ( getDimTypeFlag() == DimRotated )
    engine.preprocessData(m_dOblique,  true, ((OdRotatedDimRecomputor*)this)->m_dRotAngle);
  else
    engine.preprocessData(m_dOblique);

  engine.m_dimTih = getDimtih(pDimVars);
  engine.m_dimToh = getDimtoh(pDimVars);
  engine.m_dimTvp = getDimtvp(pDimVars);

  OdDbMTextPtr pLabel, pRate;
  if ( m_bInspection )
  {
    createInspectionEntitisForDimGraphic(pLabel, pRate, pDimVars);
  }

  if( !m_bUserDefinedTextPosition )
  {
    if ( !pText.isNull() )
    {
      engine.m_textPosition = engine.m_middleDimLinePt;
      engine.m_dTextRot = pText->rotation();
      pText->setLocation(engine.m_middleDimLinePt);

      engine.calcTextExtents(pText);
      if (!pAddedText.isNull())
      {
        OdGeExtents3d exts;
        pAddedText->setDirection(OdGeVector3d::kXAxis);
        pAddedText->getGeomExtents(exts);
        engine.m_dXTextHeight = exts.maxPoint().y - exts.minPoint().y;
        engine.m_textHeight = (engine.m_textExtents.maxPoint().y - engine.m_textExtents.minPoint().y);
        engine.m_textWidth = odmax((engine.m_textExtents.maxPoint().x - engine.m_textExtents.minPoint().x),
            (exts.maxPoint().x - exts.minPoint().x) );
      }

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

      if (!pAddedText.isNull() && tad == 0)
         engine.setTextParams(just, 1, gap);

      if ( sd2 && just == 0 && !engine.m_bTextInside )
      {
        engine.m_bSwapTextPos = true;
      }
      engine.adjustTextLocation(getDimtmove(pDimVars), getDimexe(pDimVars));

      // if ( bUseAddedText && tad == 0 ) 
      //  engine.setTextParams(just, tad, gap);
      if ( engine.m_bSwapTextPos )
        engine.m_bTextHasLine = false;

      if (OdNonZero(m_dTextRot) )
      {
        engine.m_textDirection = engine.m_xAxis;
        engine.m_textDirection.rotateBy(m_dTextRot, OdGeVector3d::kZAxis);
      }
      pText->setDirection(engine.m_textDirection);
      
      pText->setLocation(engine.m_textPosition);
      engine.m_dimOcsTextPosition.z = m_ocsDimTextMidPt.z;
      m_ocsDimTextMidPt = engine.m_dimOcsTextPosition;
    } else // no text
    {
      engine.fitTextAndArrows(false, true, false);
      if ( engine.m_isFitArrowsWithoutText )
        engine.m_bArrowsInside = true;
      else
        engine.m_bArrowsInside = false;

      engine.m_bTextInside = true;
      engine.m_bNeedCalcTextIntersection = false;
      engine.m_dimOcsTextPosition.z = m_ocsDimTextMidPt.z;
    }
  } 
  // User defined
  else
  { 
    engine.m_textPosition = m_ocsDimTextMidPt;
    engine.m_textPosition.z = 0;
    engine.m_bUserDefPos = true;

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

    if ( !pText.isNull() )
    {
      engine.calcTextExtents(pText);
      if ( m_bInspection )
      {
        engine.calcInspectionExtents(pLabel, pRate, true);
      }
    }

    if ( engine.m_bUseXText )
    {
      engine.m_textHeight = engine.m_textExtents.maxPoint().y - engine.m_textExtents.minPoint().y;
      engine.m_textWidth = engine.m_textExtents.maxPoint().x - engine.m_textExtents.minPoint().x;
    }

    OdInt16 dm = getDimtmove(pDimVars);
    engine.m_dimatfit = getDimatfit(pDimVars);
    engine.adjustUserDefText(dm, asz, getDimexe(pDimVars));
    engine.m_dimOcsTextPosition.z = m_ocsDimTextMidPt.z;
    if ( dm == 0 )
    {
      m_ocsDimTextMidPt = engine.m_dimOcsTextPosition;
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

  m_DimLineDefPt = engine.m_arrow2Pt;
  m_DimLineDefPt.z = engine.m_dimOcsTextPosition.z;
  m_DimLineDefPt.transformBy(mPlane2World);

  if ( engine.m_bLeader )
  {
    OdDbLinePtr pLine;
    ADD_LINE(engine.m_ldrPt1, engine.m_ldrPt2);
    ADD_LINE(engine.m_ldrPt2, engine.m_ldrPt3);
  }

  if(!getDimse1(pDimVars))
  {
    buildExtLine(engine.m_xLine1Pt, engine.m_xLine1PtEnd, ents, pDimVars, 0.0, engine.m_dirExtLine, m_dOblique);
    ents.last()->setLinetype(m_DimExt1LinetypeId);
    setDimLineIndex(ents.last(), EXT_LINE_1_INDEX);
  }

  if(!getDimse2(pDimVars))
  {
    buildExtLine(engine.m_xLine2Pt, engine.m_xLine2PtEnd, ents, pDimVars, 0.0, engine.m_dirExtLine, m_dOblique);
    ents.last()->setLinetype(m_DimExt2LinetypeId);
    setDimLineIndex(ents.last(), EXT_LINE_2_INDEX);
  }

  if ( !pText.isNull() )
  {
    if (!pAddedText.isNull())
    {
      // calc text position and extents for box
      // engine.applyAddedText(pAddedText);
      if ( tad == 0 && engine.m_bTextInside )
        engine.m_bNeedCalcTextIntersection = true;
      else
        engine.m_bNeedCalcTextIntersection = false;
    }

    engine.calcTextExtents(pText, true);
    engine.m_dTextRot = pText->rotation();
  } else
  {
    engine.m_bNeedCalcTextIntersection = false;
  }

  if ( engine.m_dimLines != 0 )
    engine.intersectDimLineWithText();

  bool bSuppressDimLinesAtAll = false;

  if ( (sd1 && sd2 && !engine.m_bTextInside && just < 3 ) ||
    (!engine.m_bArrowsInside && ( !getDimtofl(pDimVars) || sd1 ) ) )
  {
    // TODO: text is inside in this case It need check above
    bSuppressDimLinesAtAll = true;
    engine.m_dimLines = 0;
  } 
  if ( getDimtmove(pDimVars) == 1 && ( engine.m_bLeader || engine.m_isTxtPosFarFromArrowPoint) ) // /* m_bUserDefinedTextPosition && */ 
  {
    bSuppressDimLinesAtAll = false;
    engine.m_dimLines = 1;
  }
  if ( engine.m_bTextHasLine && !sd1 && !engine.m_bSwapTextPos)
  {
    OdDbLinePtr pLine;
    ADD_LINE(engine.m_textLinePt1, engine.m_textLinePt2);
  }

  if (  engine.m_bLeader && getDimtmove(pDimVars) == 1 )
  {
    bSuppressDimLinesAtAll = false;
  }
  bool bSuppressArrows = false;
  if (!sd1 && !sd2 && m_bUserDefinedTextPosition)
  {
    if ( !engine.m_isFitArrowsWithoutText && getDimtmove(pDimVars) == 2 && getDimsoxd(pDimVars))
    {
      engine.m_bTail1 = engine.m_bTail2 = false;
      engine.setAszParams(0.0, 0.0, 0.0, false, false);
      bSuppressArrows = true;
      if ( getDimtofl(pDimVars) )
        engine.m_dimLines = 1;
      else
        engine.m_dimLines = 0;
    } else if ( !engine.m_bArrowsInside && engine.m_dimLines == 0 && getDimtofl(pDimVars))
    {
      engine.m_dimLines = 1;
    }
  }

  engine.calcDirectionArrows();
  engine.calcDimLinesPoints();
  engine.addDimLineTails(getDimsoxd(pDimVars), getDimdle(pDimVars));

  OdDbLinePtr pLine;
  if ( !bSuppressDimLinesAtAll )
  {
    bool bJog = false;
    OdGePoint3dArray points;

    if ( ( sd1 || sd2 ) && engine.m_bTextInside )
    {
      engine.correctDimLinesPoints();
      if ( m_bJogSymbolOn )
      {
        engine.m_textHeight = getDimtxt(pDimVars);
        bJog = engine.makeJogSymbol(m_pJogPosition, m_JogSymbolHeight, points, pAddedText, sd1 != 0, sd2 != 0);
      }
      if ( !sd1 )
      {
        if ( engine.m_dimLines == 2 )
        {
          ADD_LINE(engine.m_dimLineEnd1Pt, engine.m_dimLineStart1Pt);
        } else
        {
          ADD_LINE(engine.m_dimLineEnd1Pt, engine.m_dimLineStart2Pt);
        }
        setDimLineIndex(ents.last(), DIM_LINE_INDEX);
      } else if ( !sd2 )
      {
        if ( engine.m_dimLines == 2 )
        {
          ADD_LINE(engine.m_dimLineEnd2Pt, engine.m_dimLineStart2Pt);
        }
        else
        {
          ADD_LINE(engine.m_dimLineEnd2Pt, engine.m_dimLineStart1Pt);
        }
        setDimLineIndex(ents.last(), DIM_LINE_INDEX);
      } // else
      // ODA_ASSERT(0);
      if ( bJog )
      {
        if (engine.m_bAddedDimLine)
        {
          ADD_LINE(engine.m_AddedDimLineEnd, engine.m_AddedDimLineStart);
          setDimLineIndex(ents.last(), DIM_LINE_INDEX);
        }
        ADD_LINE(points[0], points[1]);
        ADD_LINE(points[1], points[2]);
        ADD_LINE(points[2], points[3]);
      }
    } else if ( m_bUserDefinedTextPosition && !engine.m_bTextInside  && ( sd1 || sd2 ) )
    {
      OdInt16 dm = getDimtmove(pDimVars);
      if ( dm == 0 )
      {
        if ( engine.m_textPosition.distanceTo(engine.m_arrow1Pt) < engine.m_textPosition.distanceTo(engine.m_arrow2Pt) )
        {
          if ( !sd2 )
          {
            ADD_LINE(engine.m_arrow1Pt, engine.m_arrow2Pt);
          } else
          {
            OdGePoint3d point;
            point = engine.m_arrow1Pt + engine.m_dirDimLine*2.0*asz;
            ADD_LINE(engine.m_arrow1Pt, point);
          }
          setDimLineIndex(ents.last(), DIM_LINE_INDEX);
        } else
        {
          if ( !sd1 )
          {
            ADD_LINE(engine.m_arrow1Pt, engine.m_arrow2Pt);
          } else
          {
            OdGePoint3d point;
            point = engine.m_arrow2Pt - engine.m_dirDimLine*2.0*asz;
            ADD_LINE(engine.m_arrow2Pt, point);
          }
          setDimLineIndex(ents.last(), DIM_LINE_INDEX);
        }
      } else if ( dm == 1 )
      {
        ADD_LINE(engine.m_arrow1Pt, engine.m_arrow2Pt);
        setDimLineIndex(ents.last(), DIM_LINE_INDEX);
      } else
      {
        if ( sd1 )
        {
          OdGePoint3d point;
          point = engine.m_arrow2Pt - engine.m_dirDimLine*2.0*asz;
          ADD_LINE(engine.m_arrow2Pt, point);
          setDimLineIndex(ents.last(), DIM_LINE_INDEX);
        } else
        {
          OdGePoint3d point;
          point = engine.m_arrow1Pt + engine.m_dirDimLine*2.0*asz;
          ADD_LINE(engine.m_arrow1Pt, point);
          setDimLineIndex(ents.last(), DIM_LINE_INDEX);
        }
      }
      engine.m_dimLines = 0;
    } else
    {
      if ( m_bJogSymbolOn )
      {
        engine.m_textHeight = getDimtxt(pDimVars);
        bJog = engine.makeJogSymbol(m_pJogPosition, m_JogSymbolHeight, points, pAddedText);
      }

      if ( bJog )
      {
        if ( engine.m_dimLines == 1 )
        {
          ADD_LINE(engine.m_dimLineEnd1Pt, engine.m_dimLineStart1Pt);
          setDimLineIndex(ents.last(), DIM_LINE_INDEX);
          ADD_LINE(engine.m_dimLineEnd2Pt, engine.m_dimLineStart2Pt);
          setDimLineIndex(ents.last(), DIM_LINE_INDEX);
        } else if ( engine.m_dimLines == 2 )
        {
          ADD_LINE(engine.m_dimLineEnd1Pt, engine.m_dimLineStart1Pt);
          setDimLineIndex(ents.last(), DIM_LINE_INDEX);
          ADD_LINE(engine.m_dimLineEnd2Pt, engine.m_dimLineStart2Pt);
          setDimLineIndex(ents.last(), DIM_LINE_INDEX);
          if (engine.m_bAddedDimLine)
          {
            ADD_LINE(engine.m_AddedDimLineEnd, engine.m_AddedDimLineStart);
            setDimLineIndex(ents.last(), DIM_LINE_INDEX);
          }
        }
        ADD_LINE(points[0], points[1]);
        ADD_LINE(points[1], points[2]);
        ADD_LINE(points[2], points[3]);
      } else
      {
        if ( engine.m_dimLines == 1 )
        {
          ADD_LINE(engine.m_dimLineEnd1Pt, engine.m_dimLineEnd2Pt);
          setDimLineIndex(ents.last(), DIM_LINE_INDEX);
        } else if ( engine.m_dimLines == 2 )
        {
          ADD_LINE(engine.m_dimLineEnd1Pt, engine.m_dimLineStart1Pt);
          setDimLineIndex(ents.last(), DIM_LINE_INDEX);
          ADD_LINE(engine.m_dimLineEnd2Pt, engine.m_dimLineStart2Pt);
          setDimLineIndex(ents.last(), DIM_LINE_INDEX);
        }
      }
    }
  }

  // if ( !bSuppressDimLinesAtAll )
  {
    if ( engine.m_bTail1 && !sd1 )
    {
      ADD_LINE(engine.m_tail1StartPt , engine.m_tail1EndPt);
      setDimLineIndex(ents.last(), DIM_LINE_INDEX);
    }
    if ( engine.m_bTail2 && !sd2 )
    {
      ADD_LINE(engine.m_tail2StartPt , engine.m_tail2EndPt);
      setDimLineIndex(ents.last(), DIM_LINE_INDEX);
    }
  }

  if ( !engine.m_bArrowsInside )
  {
    if ( getDimtofl(pDimVars) )
      engine.m_dimLines = 1;
    else
      bSuppressDimLinesAtAll = true;
  }

  if ( !bSuppressDimLinesAtAll && !engine.m_bTextInside && engine.m_dimLines == 1 )
  {
    pDimVars->setDimsd1(false);
    pDimVars->setDimsd2(false);
  }
 
  if ( !(!m_bUserDefinedTextPosition && engine.m_bTextInside && !engine.m_bArrowsInside && getDimsoxd(pDimVars)) )
  {
    if ( OdNonZero(tszVar) )
    {
      OdGeVector3d v = engine.m_dirArrow1;
      v.rotateBy(OdaPI4, OdGeVector3d::kZAxis);
      OdGePoint3d p1, p2;
      p1 = engine.m_arrow1Pt + v*sizeStroke;
      p2 = engine.m_arrow1Pt - v*sizeStroke;
      ADD_LINE(p1, p2);
    } else if (!bSuppressArrows && ( !sd1 || engine.m_dimLines ))
      buildArrow1(engine.m_arrow1Pt, engine.m_dirArrow1, ents, pDimVars);

    if ( OdNonZero(tszVar) )
    {
      OdGeVector3d v = engine.m_dirArrow2;
      v.rotateBy(OdaPI4, OdGeVector3d::kZAxis);
      OdGePoint3d p1, p2;
      p1 = engine.m_arrow2Pt + v*sizeStroke;
      p2 = engine.m_arrow2Pt - v*sizeStroke;
      ADD_LINE(p1, p2);
    } else if (!bSuppressArrows && (!sd2 || engine.m_dimLines ))
      buildArrow2(engine.m_arrow2Pt, engine.m_dirArrow2, ents, pDimVars);
  }

  // Make inspection
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
      if (!pAddedText.isNull())
      {
        OdArray<OdGePoint3d> points;
        gap = getDimgap(pDimVars);
        engine.setTextParams(just, tad, gap);

        engine.applyXText(pAddedText, points);

        OdUInt8 tolj = getDimtolj(pDimVars);
        if(tolj != 0)
        {
          OdString toljAdd;
          toljAdd.format(OD_T("\\A%i;"), (int)tolj);
          OdString contStr = toljAdd + pAddedText->contents();
          pAddedText->setContents(contStr);
        }

        ents.push_back(pAddedText);

        if ( getDimgap(pDimVars) < 0.0 )
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
  if ( !pText.isNull() )
  {
    pText->setAttachmentMovingLocation((OdDbMText::AttachmentPoint)m_AttachPt);
  } 

  OdDbPointPtr point;
  OdDbObjectId defPointsId = getLayerDefpointsId();
  // already projected
  ADD_POINT(/*mPlane2World */ engine.m_xLine1Pt);
  ADD_POINT(/*mPlane2World */ engine.m_xLine2Pt);
  ADD_POINT(/*mPlane2World */ engine.m_dimLineDefPt);
}

void OdAlignedDimRecomputor::buildDimLine1(const OdGePoint3d& start,
                                           const OdGePoint3d& end,
                                           OdArray<OdDbEntityPtr>& ents,
                                           OdDbDimStyleTableRecord* pDimVars)
{
  if(getDimsd1(pDimVars))
    return;
  OdGeVector3d end2start = (start - end);
  if(end2start.isZeroLength()) // cr 1932
    return;
  end2start.normalize();
  OdGePoint3d newEnd = end;
  if(getDimsah(pDimVars))
  {
    if(toExtend(getDimblk1(pDimVars)))
    {
      newEnd -= end2start * (getDimasz(pDimVars) + getDimdle(pDimVars));
    }
  }
  buildDimLine(start, newEnd, ents, pDimVars);
}

void OdAlignedDimRecomputor::buildDimLine2(const OdGePoint3d& start,
                                           const OdGePoint3d& end,
                                           OdArray<OdDbEntityPtr>& ents,
                                           OdDbDimStyleTableRecord* pDimVars)
{
  if(getDimsd2(pDimVars))
    return;
  OdGeVector3d end2start = start - end;
  if(end2start.isZeroLength()) // cr1932
    return;  
  end2start.normalize();
  OdGePoint3d newEnd = end;
  if(getDimsah(pDimVars))
  {
    if(toExtend(getDimblk2(pDimVars)))
    {
      newEnd -= end2start * (getDimasz(pDimVars) + getDimdle(pDimVars));
    }
  }
  buildDimLine(start, newEnd, ents, pDimVars);
}

void OdAlignedDimRecomputor::buildDimLine(const OdGePoint3d& start, 
                                          const OdGePoint3d& end,
                                          OdArray<OdDbEntityPtr>& ents, 
                                          OdDbDimStyleTableRecord* pDimVars)
{
  OdGeVector3d end2start = (start.asVector() - end.asVector()).normalize();
  OdGePoint3d newEnd = end;
  if(!getDimsah(pDimVars))
  {
    if(toExtend(getDimblk(pDimVars)))
    {
      newEnd -= end2start * (getDimasz(pDimVars) + getDimdle(pDimVars));
    }
  }
  newEnd += end2start * getDimasz(pDimVars);

  OdDbLinePtr pLine = OdDbLine::createObject();
  pLine->setLineWeight(OdDb::LineWeight(getDimlwd(pDimVars)));
  pLine->setColor(getDimclrd(pDimVars));
  pLine->setStartPoint(newEnd);
  pLine->setEndPoint(start);
  pLine->setLinetype(m_DimLinetypeId);
  ents.push_back(pLine);
}

// This function is called if both dimension lines are not supressed
void OdAlignedDimRecomputor::buildSingleDimLine(const OdGePoint3d& start,
                                                const OdGePoint3d& end,
                                                OdArray<OdDbEntityPtr>& ents,
                                                OdDbDimStyleTableRecord* pDimVars)
{
  OdGeVector3d end2start = (start.asVector() - end.asVector()).normalize();
  OdGePoint3d newEnd = end, newStart = start;
  if(!getDimsah(pDimVars))
  {
    if(toExtend(getDimblk(pDimVars)))
    {
      newEnd -= end2start * (getDimasz(pDimVars) + getDimdle(pDimVars));
      newStart += end2start * (getDimasz(pDimVars) + getDimdle(pDimVars));
    }
  }
  else
  {
    if(toExtend(getDimblk1(pDimVars)))
    {
      newStart += end2start * (getDimasz(pDimVars) + getDimdle(pDimVars));
    }
    if(toExtend(getDimblk2(pDimVars)))
    {
      newEnd -= end2start * (getDimasz(pDimVars) + getDimdle(pDimVars));
    }
  }
  newEnd   += end2start * getDimasz(pDimVars);
  newStart -= end2start * getDimasz(pDimVars);

  OdDbLinePtr pLine = OdDbLine::createObject();
  pLine->setLineWeight(OdDb::LineWeight(getDimlwd(pDimVars)));
  pLine->setColor(getDimclrd(pDimVars));
  pLine->setStartPoint(newStart);
  pLine->setEndPoint(newEnd);
  ents.push_back(pLine);
}

void OdAlignedDimRecomputor::preprocessDimPoints(OdDbDimStyleTableRecord* pDimVars)
{
  // Point m_DimLineDefPt must lie on perpendicular to m_DefPoint1 - m_DefPoint2
  OdGeMatrix3d world2Plane(OdGeMatrix3d::worldToPlane(normal()));
  OdGePoint3d ocsDefPt1(m_DefPoint1), ocsDefPt2(m_DefPoint2), ocsDimLDPt(m_DimLineDefPt);
  ocsDefPt1.transformBy(world2Plane);
  ocsDefPt2.transformBy(world2Plane);
  ocsDimLDPt.transformBy(world2Plane);

  // GU: same code duplicated because it may become different in future
  if(m_bUserDefinedTextPosition && (getDimtmove(pDimVars) == 0) )
  {
    return; // TODO: ...
            // SZ: 
            //    in case dimtoh() ( text ouside is horizontal) it need to add in account text extention
            //    and direction is depended of text site  relative ext lines

    /*
    // 20.12.2004 GU, cr2408. Should move m_DimLineDefPt only when Dimtmove is zero
    if(!ocsDefPt1.isEqualTo(ocsDefPt2) && (getDimtmove(pDimVars) == 0))
    {
      OdGeVector3d perp = OdGeVector3d::kZAxis.crossProduct(ocsDefPt1 - ocsDefPt2);
      if(OdNonZero(m_dOblique))
        perp.rotateBy(m_dOblique-OdaPI2, OdGeVector3d::kZAxis);
      OdGePoint3d newDimLDPt;
      intersectLines(newDimLDPt, ocsDefPt2, m_ocsDimTextMidPt, perp, ocsDefPt1 - ocsDefPt2);
      m_DimLineDefPt = newDimLDPt.transformBy(OdGeMatrix3d::planeToWorld(normal()));
    }
    */
  }
  else
  {
    if(!ocsDefPt1.isEqualTo(ocsDefPt2))
    {
      OdGeVector3d perp = OdGeVector3d::kZAxis.crossProduct(ocsDefPt1 - ocsDefPt2);
      if ( !perp.isZeroLength() )
      {
        if(OdNonZero(m_dOblique))
        {
          // 29.07.2003, G. Udov, to take oblique into account
          perp.rotateBy(m_dOblique-OdaPI2, OdGeVector3d::kZAxis);
        }
        OdGePoint3d newDimLDPt;
        intersectLines(newDimLDPt, ocsDefPt2, ocsDimLDPt, perp, ocsDefPt1 - ocsDefPt2);
        m_DimLineDefPt = newDimLDPt.transformBy(OdGeMatrix3d::planeToWorld(normal()));
      }
    }
  }
}

void OdAlignedDimRecomputor::updateMeasurement(OdDbDimStyleTableRecord* /*pDimVars*/)
{
  OdGeMatrix3d mWorld2Plane(OdGeMatrix3d::worldToPlane(m_vNormal));
  OdGePoint3d defPt1(m_DefPoint1), defPt2(m_DefPoint2);
  defPt1.transformBy(mWorld2Plane);
  defPt2.transformBy(mWorld2Plane);
  defPt1.z = defPt2.z = 0.0;
  m_dMeasurement = defPt1.distanceTo(defPt2);
  m_dMeasurement *= m_dimlfac;
}
