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
#include "OdOrdinateDimRecomputor.h"
#include "RxObjectImpl.h"
#include "DbObjectContextData.h"

#include "Ge/GeMatrix3d.h"
#include "Ge/GeExtents3d.h"

#include "DbOrdinateDimension.h"
#include "OdRecomputorEngine.h"
#include "DbLine.h"
#include "DbPoint.h"


OdDimRecomputorPtr OdDbOrdinateDimRecomputePEImpl::createRecomputor()
{
  return OdRxObjectImpl<OdOrdinateDimRecomputor>::createObject();
}

OdOrdinateDimRecomputor::OdOrdinateDimRecomputor()
: m_bXOrd(false)
{
}

OdOrdinateDimRecomputor::~OdOrdinateDimRecomputor()
{
}

OdUInt8 OdOrdinateDimRecomputor::getDimTypeFlag()
{
  return DimOrdinate;
}

void OdOrdinateDimRecomputor::getDimParams(const OdDbDimension* pDim, OdDbDimensionObjectContextData* ctx)
{
  OdDimRecomputor::getDimParams(pDim, ctx);
  OdDbOrdinateDimensionPtr pDimPtr(pDim);
  m_DefPoint1 = pDimPtr->definingPoint();
  m_DefPoint2 = ctx ? ((OdDbOrdinateDimensionObjectContextData*)ctx)->leaderEndPoint() : pDimPtr->leaderEndPoint();
  m_bXOrd = pDimPtr->isUsingXAxis();
  m_DimLineDefPt = ctx ? ((OdDbOrdinateDimensionObjectContextData*)ctx)->origin() : pDimPtr->origin();
}

void OdOrdinateDimRecomputor::setDimParams(OdDbDimension* pDim, OdDbDimensionObjectContextData* ctx) const
{
  OdDimRecomputor::setDimParams(pDim, ctx);
  OdDbOrdinateDimensionPtr pDimPtr(pDim);
  pDimPtr->setDefiningPoint(m_DefPoint1);
  if (!ctx || ctx->isDefaultContextData())
  {
    pDimPtr->setLeaderEndPoint(m_DefPoint2);
    pDimPtr->setOrigin(m_DimLineDefPt);
  }
  if (ctx)
  {
    ((OdDbOrdinateDimensionObjectContextData*)ctx)->setLeaderEndPoint(m_DefPoint2);
    ((OdDbOrdinateDimensionObjectContextData*)ctx)->setOrigin(m_DimLineDefPt);
  }
  if(m_bXOrd)
    pDimPtr->useXAxis();
  else
    pDimPtr->useYAxis();
}


///

void OdOrdinateDimRecomputor::preprocessDimPoints(OdDbDimStyleTableRecord*)
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
  pLine->transformBy(toHorBack); \
  pLine->setTransparency(transparency); \
  ents.push_back(pLine);

#define ADD_POINT(a) \
  point = OdDbPoint::createObject(); \
  point->setPosition(a); \
  point->setColor(OdCmEntityColor::kByBlock); \
  point->setLayer(defPointsId); \
  point->setTransparency(transparency); \
  ents.push_back(point);


void OdOrdinateDimRecomputor::buildDimension(OdArray<OdDbEntityPtr>& ents,
                                             OdDbDimStyleTableRecord* pDimVars)
{
  double gap = getDimgap(pDimVars);

  //OdGePoint3d textPoint   = m_DimTextMidPt;
  OdGePoint3d firstPoint  = m_DefPoint2;
  OdGePoint3d secondPoint = m_DefPoint1;
  OdGePoint3d originPoint = m_DimLineDefPt;
  OdCmTransparency transparency = m_transparency;

  // transforming to OCS
  OdGeMatrix3d mWorld2Plane(OdGeMatrix3d::worldToPlane(m_vNormal));
  OdGeMatrix3d mPlane2World(OdGeMatrix3d::planeToWorld(m_vNormal));
  //textPoint.transformBy(mWorld2Plane);
  firstPoint.transformBy(mWorld2Plane);
  secondPoint.transformBy(mWorld2Plane);
  originPoint.transformBy(mWorld2Plane);
  double savedZCoord = firstPoint.z;

  firstPoint.z = secondPoint.z = originPoint.z = 0.0; // cr4288

  // angle between line segment (textPoint, firstPoint) and X Axis
  OdGeVector3d dir, xAxis, yAxis;
  getUCS(xAxis, yAxis);
  if(m_bXOrd)
  {
    // Vertical dimension(indicates X coordinate)
    // dir = yAxis * (secondPoint.y - firstPoint.y) / fabs(secondPoint.y - firstPoint.y);
    dir = yAxis;
  }
  else
  {
    // Horizontal dimension(indicates Y coordinate)
    // dir = xAxis * (secondPoint.x - firstPoint.x) / fabs(secondPoint.x - firstPoint.x);
    dir = xAxis;
  }
  // SZ In case UserDefinedTextPosition ACAD uses text position point to calculate direction of dimention
  OdGePoint3d point1 = firstPoint;
  if ( m_bUserDefinedTextPosition && ( getDimtmove(pDimVars) != 2 ) )
  {
    point1 = m_ocsDimTextMidPt - OdGeVector3d(0.0, 0.0, m_ocsDimTextMidPt.z);
  }
  double d = dir.dotProduct(point1-secondPoint);
  if ( d > 0 || ( m_bUserDefinedTextPosition && OdZero(d) ) )
  {
    dir *= -1.0;
  }
  double angle = OdGeVector3d::kXAxis.angleTo(dir, OdGeVector3d::kZAxis);
  OdGeMatrix3d toHor(OdGeMatrix3d::rotation(-angle, OdGeVector3d::kZAxis, firstPoint));
  OdGeMatrix3d toHorBack(toHor.inverse());
  OdGePoint3d firstPointHor = firstPoint;
  firstPointHor.transformBy(toHor);
  OdGePoint3d secondPointHor = secondPoint;
  secondPointHor.transformBy(toHor);

  // Lines
  OdInt16 tad = getDimtad(pDimVars);
  double asz = getDimasz(pDimVars);
  OdCmColor color = getDimclre(pDimVars);

  bool bTextAboveLine = ( tad > 0 ) && OdZero(m_dTextRot);
  OdDb::LineWeight lineweight = OdDb::LineWeight(getDimlwe(pDimVars));
  OdGePoint3d pointA = firstPointHor + OdGeVector3d(2.0 * asz, 0.0, 0.0);
  OdGePoint3d pointB = pointA + OdGeVector3d(/*2.0 * asz*/0.0, secondPointHor.y - firstPointHor.y, 0.0);

  OdRecomputorEngine engine;
  OdUInt16 just = getDimjust(pDimVars);

  engine.setTextParams(just, tad, gap);

  gap = ::fabs(gap);

  // Text
  OdDbMTextPtr pText;
  createTextEntityForDimGraphic(pText, pDimVars);

  OdDbMTextPtr pAddedText;
  if (!pText.isNull())
  {
    engine.m_dTextRot = m_dTextRot;
    pAddedText = engine.createXText(pText, getDimtmove(pDimVars), m_bUserDefinedTextPosition, m_dTextRot);
  }

  engine.m_textPosition = m_ocsDimTextMidPt;
  engine.m_textPosition.z = 0.0;
  engine.calcTextExtents(pText);
  OdDbMTextPtr pLabel, pRate;
  if ( m_bInspection )
  {
    engine.setInspection( (m_nInspectionFrame & OdDbDimension::kShapeLabel) != 0, 
      (m_nInspectionFrame & OdDbDimension::kShapeRate) != 0, 
      (m_nInspectionFrame & (OdDbDimension::kShapeRound | OdDbDimension::kShapeAngular ) ) != 0 );
    if ( pText.get() )
    {
      createInspectionEntitisForDimGraphic(pLabel, pRate, pDimVars);
      engine.calcInspectionExtents(pLabel, pRate, true);
    }
  }

  OdGeVector3d textDirection;
  if(m_bXOrd)
  {
    textDirection = OdZero(m_dTextRot) ? yAxis : xAxis;
  }
  else
  {
    textDirection = xAxis;
  }
  textDirection.transformBy(OdGeMatrix3d::rotation(m_dTextRot, OdGeVector3d::kZAxis));
  double xExtText = 0.0;
  if(pText.get()) // 18.07.2003 GU - text can be suppressed
  {
    // Calculating text position(default case)
    double height = engine.textHeight();
    if(!m_bUserDefinedTextPosition)
    {
      OdGeMatrix3d toTextHor(OdGeMatrix3d::rotation(-OdGeVector3d::kXAxis.angleTo(textDirection, OdGeVector3d::kZAxis),
        OdGeVector3d::kZAxis, firstPoint));
      OdGeMatrix3d toTextHorBack(toTextHor.inverse());
      OdGeVector3d dirTG = dir;
      dirTG.transformBy(toTextHor); // dirTG is normalized

      if (!pAddedText.isNull())
      {
        OdGeExtents3d exts;
        pAddedText->setDirection(OdGeVector3d::kXAxis);
        pAddedText->getGeomExtents(exts);
        engine.m_dXTextHeight = exts.maxPoint().y - exts.minPoint().y;
        engine.m_textHeight = (engine.m_textExtents.maxPoint().y - engine.m_textExtents.minPoint().y);
        engine.m_textWidth = odmax((engine.m_textExtents.maxPoint().x - engine.m_textExtents.minPoint().x),
          (exts.maxPoint().x - exts.minPoint().x));
      }

      // Calculating in units where text box is horizontal
      pText->setLocation(firstPoint);

      // Query gapped extents
      OdGeExtents3d extsTG;
      OdGePoint3d minPoint = firstPoint;
      OdGePoint3d maxPoint = firstPoint;
      xExtText = engine.textWidth(false);
      minPoint.x -= ( gap + xExtText/2.0);
      minPoint.y -= height/2.0;
      maxPoint.x += (gap + xExtText/2.0);
      maxPoint.y += height/2.0;

      extsTG.set(minPoint, maxPoint);

      OdGePoint2dArray itersecTG;
      intersectLineAndExts(firstPoint, firstPoint + dirTG * minPoint.distanceTo(maxPoint) * 1.1, extsTG, itersecTG);
      if ( itersecTG.size() == 1 )
      {
        OdGePoint3d interPoint(itersecTG[0].x, itersecTG[0].y, 0.0);
        interPoint.transformBy(toTextHorBack);
        m_ocsDimTextMidPt = firstPoint + (firstPoint - interPoint) + OdGeVector3d(0.0, 0.0, m_ocsDimTextMidPt.z);

        OdGeVector3d perp = textDirection.perpVector();
        if ( bTextAboveLine )
        {
          if ( tad == 4 )
            perp *= -1.0;
          // double height = engine.textHeight();
          m_ocsDimTextMidPt += perp*(height/2.0);
          if ( engine.isBox() )
          {
            m_ocsDimTextMidPt += perp*gap;
          }
        }
      } else
      {
        ODA_ASSERT(0);
      }
    }
    pText->setLocation(m_ocsDimTextMidPt - OdGeVector3d(0.0, 0.0, m_ocsDimTextMidPt.z));
    pText->setDirection(textDirection);
    engine.m_textPosition = m_ocsDimTextMidPt;
    engine.m_textDirection = textDirection;
  }

  // if text position is set up by user, we should correct firstPoint
  if(m_bUserDefinedTextPosition )
  {
    bool bTextAboveLineSaved = bTextAboveLine;
    if ( getDimtmove(pDimVars) != 2 )
    {
      if(pText.get())
      {
        double height = engine.textHeight();
        OdGePoint3d txtLocation = pText->location();
        txtLocation.z = 0; // projection

        if (!pAddedText.isNull())
        {
          OdGeExtents3d exts;
          pAddedText->setDirection(OdGeVector3d::kXAxis);
          pAddedText->getGeomExtents(exts);
          engine.m_dXTextHeight = exts.maxPoint().y - exts.minPoint().y;
          engine.m_textHeight = (engine.m_textExtents.maxPoint().y - engine.m_textExtents.minPoint().y);
          engine.m_textWidth = odmax((engine.m_textExtents.maxPoint().x - engine.m_textExtents.minPoint().x),
            (exts.maxPoint().x - exts.minPoint().x));
        }
        else
        {
          if ( engine.m_bUseXText )
            bTextAboveLine = false;
          engine.m_bUseXText = false;
        }

        OdGeVector3d saveDirection = pText->direction();
        pText->setDirection(OdGeVector3d::kXAxis);

        OdGeMatrix3d toTextHor(OdGeMatrix3d::rotation(-OdGeVector3d::kXAxis.angleTo(saveDirection, 
          OdGeVector3d::kZAxis), OdGeVector3d::kZAxis, txtLocation));
        OdGeMatrix3d toTextHorBack(toTextHor.inverse());
        OdGeVector3d dirTG = dir;
        dirTG.transformBy(toTextHor); // dirTG is normalized

        OdGeExtents3d extsTG;

        OdGePoint3d minPoint = txtLocation; 
        OdGePoint3d maxPoint = txtLocation;  
        xExtText = engine.textWidth(false);
        minPoint.x -= ( gap + xExtText/2.0);
        minPoint.y -= height/2.0;
        maxPoint.x += (gap + xExtText/2.0);
        maxPoint.y += height/2.0;

        extsTG.set(minPoint, maxPoint);

        OdGePoint2dArray itersecTG;
        double diagLength = minPoint.distanceTo(maxPoint) * 1.1; // koef 1.1 is only for sure
        intersectLineAndExts(txtLocation - dirTG * diagLength, 
          txtLocation + dirTG * diagLength, extsTG, itersecTG);
        ODA_ASSERT(itersecTG.size() == 2);
        OdGePoint3dArray candPts;
        OdUInt32 i, numCndPts = itersecTG.size();
        candPts.resize(numCndPts);
        for(i = 0; i < numCndPts; i ++)
        {
          candPts[i].set(itersecTG[i].x, itersecTG[i].y, 0.0);
          candPts[i].transformBy(toTextHorBack);
        }
        firstPoint  = candPts[0]; // will exception in case empty array
        double dist = candPts[0].distanceTo(secondPoint);
        for(i = 1; i < numCndPts; i ++)
        {
          double distCnd = candPts[i].distanceTo(secondPoint);
          if(distCnd <= dist) // "=" - SZ Possible it need other criterion
          {
            firstPoint = candPts[i];
            dist = distCnd;
          }
        }
        if ( bTextAboveLine )
        {
          firstPoint -= textDirection.perpVector() * height/2.0;
          if ( engine.isBox() )
          {
            firstPoint -= textDirection.perpVector()*gap;
          }
        }
        pText->setDirection(saveDirection);
        engine.m_textDirection = saveDirection;
      }
      else
      {
        firstPoint = OdGePoint3d();
      }
    }
    bTextAboveLine = bTextAboveLineSaved;
    m_DefPoint2 = firstPoint; // acad corrects m_DefPoint2 too
    m_DefPoint2.z = savedZCoord;
    m_DefPoint2.transformBy(mPlane2World);
    firstPointHor = firstPoint;
    firstPointHor.transformBy(toHor);
    pointA = firstPointHor + OdGeVector3d(2.0 * asz, 0.0, 0.0);
    pointB = pointA + OdGeVector3d(/*2.0 * asz*/0.0, secondPointHor.y - firstPointHor.y, 0.0);
  }

  pointB.x += 2.0 * asz;
  if ( secondPointHor.x - pointB.x < 2.0*asz )
  {
    pointB.x = secondPointHor.x - 2.0*asz;
  } 

  if ( bTextAboveLine )
  {
    firstPointHor.x -= (xExtText+gap);
  }

  // putting lines
  OdDbLinePtr pLine;
  double len = ::fabs(pointB.x - secondPointHor.x);
  OdDbObjectId dimLinetype = m_DimExt2LinetypeId;
  double linetypeScale = m_dLinetypeScale;

  if ( OdZero(pointA.y - pointB.y) && OdZero(pointA.y - firstPointHor.y)  )
  {
    // Points on one line
    if (m_ExtLineFixLenEnable && m_ExtLineFixLen + getDimexo(pDimVars) < len)
    {
      OdGePoint3d scndPt = firstPoint + OdGeVector3d(m_ExtLineFixLen, 0.0, 0.0);
      ADD_LINE(scndPt, firstPoint);
    } else
    {
      ADD_LINE(secondPointHor - OdGeVector3d(getDimexo(pDimVars), 0.0, 0.0), firstPointHor);
    }
    setDimLineIndex(ents.last(), DIM_LINE_ORD_INDEX);
  } else
  {
    if ( m_ExtLineFixLenEnable && m_ExtLineFixLen + getDimexo(pDimVars) < len )
    {
      OdGePoint3d scndPt = pointB + OdGeVector3d(m_ExtLineFixLen, 0.0, 0.0);
      ADD_LINE(scndPt, pointB);
    }  else
    {
      ADD_LINE(secondPointHor - OdGeVector3d(getDimexo(pDimVars), 0.0, 0.0), pointB);
    }
    setDimLineIndex(ents.last(), EXT_LINE_1_INDEX);
    ADD_LINE(pointB, pointA);
    setDimLineIndex(ents.last(), EXT_LINE_2_INDEX);
    ADD_LINE(pointA, firstPointHor);
    setDimLineIndex(ents.last(), DIM_LINE_ORD_INDEX);
  }

  // Make Inspection
  if ( m_bInspection )
  {
    engine.m_dTextRot = pText->rotation();
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
    engine.m_textPosition = pos;
  } else
  {
    // putting text
    if(pText.get())
    {
      ents.push_back(pText);
      if (!pAddedText.isNull())
      {
        OdArray<OdGePoint3d> points;
        bool bGap = false;
        if ( tad == 0 )
          bGap = true;
        OdGeVector3d perp = textDirection.perpVector()*(engine.textHeight(bGap) / 2.0);
        engine.m_textPosition += perp;
        pText->setLocation(engine.m_textPosition);

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
  // Points
  OdDbPointPtr point;
  OdDbObjectId defPointsId = getLayerDefpointsId();
  ADD_POINT(secondPoint);
  ADD_POINT(firstPoint);
  ADD_POINT(originPoint);
}

// Updates measurement value
void OdOrdinateDimRecomputor::updateMeasurement(OdDbDimStyleTableRecord* /*pDimVars*/)
{
  OdGeMatrix3d mWorld2Plane(OdGeMatrix3d::worldToPlane(m_vNormal));
  OdGePoint3d secondPoint = m_DefPoint1;
  OdGePoint3d originPoint = m_DimLineDefPt;
  secondPoint.transformBy(mWorld2Plane);
  originPoint.transformBy(mWorld2Plane);
  secondPoint.transformBy(OdGeMatrix3d::rotation(m_dUcsXAngle, OdGeVector3d::kZAxis));
  originPoint.transformBy(OdGeMatrix3d::rotation(m_dUcsXAngle, OdGeVector3d::kZAxis));

  // 19.12.2003 GU
  // ACAD seems to be always using fabs... (feature?)
  if(m_bXOrd)
  {
    m_dMeasurement = fabs(secondPoint.x-originPoint.x);
  }
  else
  {
    m_dMeasurement = fabs(secondPoint.y-originPoint.y);
  }
  m_dMeasurement *= m_dimlfac;
}

void OdOrdinateDimRecomputor::formatMeasurement(OdString& measurement, OdDbDimStyleTableRecord* pDimVars)
{
  formatDimStrWithAltUnits(measurement, pDimVars);
}

