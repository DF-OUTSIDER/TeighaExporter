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
#include "DbLine.h"
#include "DbArc.h"
#include "DbCircle.h"
#include "DbText.h"
#include "DbMText.h"
#include "DbRay.h"
#include "DbXline.h"
#include "DbSpline.h"
#include "DbPolyline.h"
#include "Db2dPolyline.h"
#include "DbEllipse.h"
#include "DbLeader.h"
#include "DbMLeader.h"
#include "DbDimension.h"
#include "DbBlockTableRecord.h"
#include "DbBlockReference.h"
#include "OdBreakDimEngine.h"
#include "OdDimRecomputor.h"
#include "Ge/GeCurve3d.h"
#include "Ge/GeRay3d.h"
#include "Ge/GeEllipArc3d.h"
#include "Ge/GeNurbCurve3d.h"

/*
The following objects can be used as cutting edges when adding a dimension break:
  Dimension
  Leader

  Line  (Ray and XLine)
  Circle
  Arc
  Spline
  Ellipse
  Polyline
  Text
  Multiline text

  Blocks but limited to the previously mentioned objects in this list
  Xrefs but limited to the previously mentioned objects in this list
*/

#define BREAK_TOLERANCE_MIN  1e-8
#define BREAK_TOLERANCE_MAX  1e-5

void OdBreakDimEngine::setDimensionEnts(OdArray<OdDbEntityPtr>& ents, OdGeMatrix3d matr)
{
  m_DimEnts = ents;
  m_Matrix = matr;
  m_tol = BREAK_TOLERANCE_MIN;
}

void OdBreakDimEngine::breakDimension(OdDbBreakDataPtr brData)
{
  OdDbBreakPointRefPtrArray pntRefArray;
  brData->getBreakPointRef(pntRefArray);

  OdArray<OdDbEntityPtr>::iterator pSrcEnt = m_DimEnts.begin();

  OdGeExtents3d extents;
  for (; pSrcEnt != m_DimEnts.end(); pSrcEnt++)
  {
    if ((*pSrcEnt)->isKindOf(OdDbLine::desc()))
    {
      OdDbLinePtr ln = OdDbLine::cast(*pSrcEnt);
      extents.addPoint(ln->startPoint());
      extents.addPoint(ln->endPoint());
    }
    else if ((*pSrcEnt)->isKindOf(OdDbArc::desc()))
    {
      OdDbArcPtr ar = OdDbArc::cast(*pSrcEnt);
      extents.addPoint(ar->center());
      OdGePoint3d pt;
      if ( ar->getEndPoint(pt) == eOk )
        extents.addPoint(pt);
      if ( ar->getStartPoint(pt) == eOk )
        extents.addPoint(pt);
    }
  }
  if (extents.center().asVector().length() > 1e6)
    m_tol = BREAK_TOLERANCE_MAX;

  pSrcEnt = m_DimEnts.begin();
  for(; pSrcEnt != m_DimEnts.end(); pSrcEnt++)
  {
    if((*pSrcEnt)->isKindOf(OdDbLine::desc()) || 
      ((*pSrcEnt)->isKindOf(OdDbArc::desc())) )
    {
      m_BreakPoints.clear();
      m_curEntity = *pSrcEnt;
      collectBreakPoints(*pSrcEnt, pntRefArray);
      if ( !breakDimensionLine() )
      {
        m_newDimEnts.push_back(*pSrcEnt);
      }
    } else
    {
      m_newDimEnts.push_back(*pSrcEnt);
    }
  }
}

double OdBreakDimEngine::breakAngle(double radius)
{
  if ( OdZero(radius) ) // prevent division by zero
    return OdaPI;

  double chord = m_BreakSize;
  double acosArg  = (1.0 - chord*chord / (2.0 * radius * radius) );
  // these are incorrect cases - break size is larger then radius
  if(acosArg > 1.0)
  {
    acosArg = 1.0 - 1.0e-10;
  }
  if(acosArg < -1.0)
  {
    acosArg = -1.0 + 1.0e-10;
  }
  return OD_ACOS(acosArg);
}

int OdBreakDimEngine::getIntersectionPoints(const OdGeLine3d* dimLine, const OdGePoint3dArray& boundingPoints, 
                                            OdGePoint3d& point1, OdGePoint3d& point2)
{
  OdGePoint3dArray intersections;
  OdGePoint3d intersecPt;

  OdGeLineSeg3d line(boundingPoints[0], boundingPoints[1]);
  if ( dimLine->intersectWith(line, intersecPt, m_tol) )
  {
    intersections.push_back(intersecPt);
  }

  line.set(boundingPoints[0], boundingPoints[2]);
  if ( dimLine->intersectWith(line, intersecPt, m_tol) )
  {
    intersections.push_back(intersecPt);
  }

  line.set(boundingPoints[2], boundingPoints[3]);
  if ( dimLine->intersectWith(line, intersecPt, m_tol) )
  {
    intersections.push_back(intersecPt);
  }

  line.set(boundingPoints[3], boundingPoints[1]);
  if ( dimLine->intersectWith(line, intersecPt, m_tol) )
  {
    intersections.push_back(intersecPt);
  }

  if ( intersections.size() == 2 )
  {
    point1 = intersections[0];
    point2 = intersections[1];
    return 2;
  } else if ( intersections.size() == 1 )
  {
    point1 = intersections[0];
    return 1;
  } else if ( intersections.size() == 4 )
  {
    // exclude same points
    if ( intersections[0] == intersections[1] )
    {
      point1 = intersections[0];
      point2 = intersections[2];
    } else
    {
      point1 = intersections[0];
      point2 = intersections[1];
    }
    return 2;
  }
  return 0;
}

int OdBreakDimEngine::getIntersectionPoints(const OdGeCircArc3d* Circle, const OdGePoint3dArray& boundingPoints,
                                            OdGePoint3d& point1, OdGePoint3d& point2)
{
  int num = 0;

  OdGePoint3d p1, p2;
  OdGeLineSeg3d line;

  OdGePoint3dArray intersections;
  line.set(boundingPoints[0], boundingPoints[1]);

  if ( Circle->intersectWith(line, num, p1, p2, m_tol) )
  {
    intersections.push_back(p1);
    if ( num == 2 )
      intersections.push_back(p2);
  }
  line.set(boundingPoints[0], boundingPoints[2]);

  if ( Circle->intersectWith(line, num, p1, p2, m_tol) )
  {
    intersections.push_back(p1);
    if ( num == 2 )
      intersections.push_back(p2);
  }
  line.set(boundingPoints[2], boundingPoints[3]);

  if ( Circle->intersectWith(line, num, p1, p2, m_tol) )
  {
    intersections.push_back(p1);
    if ( num == 2 )
      intersections.push_back(p2);
  }

  line.set(boundingPoints[3], boundingPoints[1]);
  if ( Circle->intersectWith(line, num, p1, p2, m_tol) )
  {
    intersections.push_back(p1);
    if ( num == 2 )
      intersections.push_back(p2);
  }

  if ( intersections.size() == 0 )
    return 0;

  if ( intersections.size() == 1 )
  {
    point1 = intersections[0];
    return 1;
  } else if ( intersections.size() == 2 )
  {
    point1 = intersections[0];
    point2 = intersections[1];
    return 2;
  } else 
  {
    double p0 = Circle->paramOf(intersections[0], m_tol);
    double p1 = Circle->paramOf(intersections[1], m_tol);
    double p2 = Circle->paramOf(intersections[2], m_tol);
    double minP = p0, maxP = p0;
    point1 = intersections[0]; point2 = intersections[0];
    if ( p1 < minP )
    {
      point1 = intersections[1];
      minP = p1;
    }
    if ( p1 > maxP )
    {
      point1 = intersections[1];
      maxP = p1;
    }

    if ( p2 < minP )
    {
      point1 = intersections[2];
      minP = p2;
    }
    if ( p2 > maxP )
    {
      point2 = intersections[2];
      maxP = p2;
    }
    if ( intersections.size() == 3 )
    {
      return 2;
    }

    double p3 = Circle->paramOf(intersections[3], m_tol);
    if ( p3 < minP )
    {
      point1 = intersections[3];
    }
    if ( p3 > maxP )
    {
      point2 = intersections[3];
    }

    return 2;
    // ODA_ASSERT(intersections.isEmpty());
  }
  // return intersections.size();
}

void OdBreakDimEngine::collectBreakPoints(OdDbEntityPtr pLine, const OdDbBreakPointRefPtrArray& pntRefArray)
{
  OdDbXrefFullSubentPath subEntPath;
  OdDbObjectId idEnt;
  OdDbObjectIdArray ids;

  OdDbEntityPtr entPtr;

  OdGeCurve3d* geCurveBrkDim = NULL;
  OdDbCurvePtr curvBrkDimPtr = OdDbCurve::cast(pLine);
  OdResult res = curvBrkDimPtr->getOdGeCurve(geCurveBrkDim);
  
  if ( geCurveBrkDim )
    geCurveBrkDim->transformBy(m_Matrix);
  else
    return;

  OdInt32 amPoints = pntRefArray.size();
  for ( OdInt32 i = 0; i < amPoints; i++ )
  {
    OdDbBreakPointRefPtr pntRef = pntRefArray[i];
    OdDbBreakPointRef::BreakPointType pntType = pntRef->pointType();
    if ( pntType == OdDbBreakPointRef::kStatic2Point )
    {
      if ( dimLineIndex(pLine) == pntRef->lineIndex() )
      {
        OdGePoint3d pt1 = pntRef->breakPoint();
        OdGePoint3d pt2 = pntRef->breakPoint2();
        breakBy2StaticPoint(pLine, pt1, pt2, pntRef->lineIndex());
      }
      continue;
    }
    if ( pntType == OdDbBreakPointRef::kStatic )
    {
      ODA_ASSERT(0);
      continue;
    }
    pntRef->getBreakPointId(subEntPath);
    subEntPath.objectIds(ids);
    if ( ids.isEmpty() )
      continue;
    idEnt = ids[0];
    if ( idEnt.isNull() )
      continue;

    entPtr = idEnt.safeOpenObject();
    if ( !entPtr.isNull() )
    {
      if ( entPtr->isKindOf(OdDbArc::desc()) || entPtr->isKindOf(OdDbCircle::desc()) )
      {
        intersectWithArc(entPtr, geCurveBrkDim);
      } else if ( entPtr->isKindOf(OdDbLine::desc()) || entPtr->isKindOf(OdDbXline::desc()) 
        || entPtr->isKindOf(OdDbRay::desc()) )
      {
        intersectWithLine(entPtr, geCurveBrkDim);
      } else if ( entPtr->isKindOf(OdDbMText::desc()) || entPtr->isKindOf(OdDbText::desc()) )
      {
        intersectWithText(entPtr, geCurveBrkDim);
      } 
      else if ( entPtr->isKindOf(OdDbPolyline::desc()) ) 
      {
        intersectWithPolyline(entPtr, geCurveBrkDim);
      } else if ( entPtr->isKindOf(OdDb2dPolyline::desc()) )
      {
        intersectWith2DPolyline(entPtr, geCurveBrkDim);
      } else if ( entPtr->isKindOf(OdDbEllipse::desc()) )
      {
        intersectWithEllipse(entPtr, geCurveBrkDim);
      } else if ( entPtr->isKindOf(OdDbSpline::desc()) ) 
      {
        intersectWithSpline(entPtr, geCurveBrkDim);
      } else if ( entPtr->isKindOf(OdDbLeader::desc()) ) 
      {
        intersectWithLeader(entPtr, geCurveBrkDim);
      } else if ( entPtr->isKindOf(OdDbDimension::desc()) )
      {
        intersectWithDimension(entPtr, geCurveBrkDim);
      } else if ( entPtr->isKindOf(OdDbBlockReference::desc()) )
      {
        intersectWithBlock(entPtr, geCurveBrkDim);
      } else if (entPtr->isKindOf(OdDbMLeader::desc()))
      {
        intersectWithMLeader(entPtr, geCurveBrkDim);
      }

    }
  }
  delete (geCurveBrkDim);
}

void OdBreakDimEngine::addBreakPoint(BreakPointData brData)
{
  OdGeMatrix3d matr = m_Matrix;
  matr.invert();
  brData.m_Point.transformBy(matr);
  brData.m_Point2.transformBy(matr);

  if ( m_curEntity->isKindOf(OdDbLine::desc()) )
  {
    OdDbLinePtr pLine = OdDbLine::cast(m_curEntity);

    OdGePoint3d startPt = pLine->startPoint();
    OdGePoint3d endPt = pLine->endPoint();

    OdGeVector3d v = endPt - startPt;
    if ( !v.isZeroLength() )
      v.normalize();
    v *= m_BreakSize;

    if ( brData.m_Type == BreakPointData::BREAK_TEXT )
    {
      brData.m_Point  = brData.m_Point  - v;
      brData.m_Point2 = brData.m_Point2 + v;
    } else if ( brData.m_Type == BreakPointData::BREAK_POINT )
    {
      OdGePoint3d point = brData.m_Point;
      brData.m_Point = point - v;
      brData.m_Point2 = point + v;
    }

    if ( brData.m_Type == BreakPointData::BREAK_TEXT 
      || brData.m_Type == BreakPointData::BREAK_POINT )
    {
      OdGeLineSeg3d geLine;
      geLine.set(startPt, endPt);
      if ( !geLine.isOn(brData.m_Point, m_tol) )
        brData.m_Point = startPt;
      if ( !geLine.isOn(brData.m_Point2, m_tol) )
        brData.m_Point2 = endPt;
    }
    brData.m_Distance = startPt.distanceTo(brData.m_Point);

    int sz = m_BreakPoints.size();
    if ( sz == 0 )
    {
      m_BreakPoints.push_back(brData);
      return;
    }

    int i = 0;
    for(i = 0; i < sz; i++)
    {
      if ( brData.m_Distance < m_BreakPoints[i].m_Distance )
      {
        m_BreakPoints.insertAt(i, brData);
        break;
      }
    }
    if ( i == sz )
      m_BreakPoints.push_back(brData);
  } else
  {
    OdDbArcPtr pArc = OdDbArc::cast(m_curEntity);

    double startAng = pArc->startAngle();
    double endAng = pArc->endAngle();
    if ( endAng < startAng )
      endAng += Oda2PI;

    double param = 0.0, param2 = 0.0;

    double brkAng = breakAngle(pArc->radius());

    pArc->getParamAtPoint(brData.m_Point, param);
    if ( brData.m_Type != BreakPointData::BREAK_POINT )
      pArc->getParamAtPoint(brData.m_Point2, param2);

    if ( brData.m_Type == BreakPointData::BREAK_TEXT 
      || brData.m_Type == BreakPointData::BREAK_STATIC )
    {
      if ( param < param2 )
      {
         brData.m_Angle = param;
         brData.m_Angle2 = param2;
      } else
      {
        brData.m_Angle = param2;
        brData.m_Angle2 = param;
      }
    } else if ( brData.m_Type == BreakPointData::BREAK_POINT )
    {
      brData.m_Angle = param - brkAng;
      brData.m_Angle2 = param + brkAng;
      if ( brData.m_Angle < startAng )
        brData.m_Angle = startAng;
      if ( brData.m_Angle2 > endAng )
        brData.m_Angle2 = endAng;
    }

    int sz = m_BreakPoints.size();
    if ( sz == 0 )
    {
      m_BreakPoints.push_back(brData);
      return;
    }

    int i = 0;
    for(i = 0; i < sz; i++)
    {
      if ( brData.m_Angle < m_BreakPoints[i].m_Angle )
      {
        m_BreakPoints.insertAt(i, brData);
        break;
      }
    }
    if ( i == sz )
      m_BreakPoints.push_back(brData);
  }
}

void OdBreakDimEngine::intersectWithGeLine(OdGeLineSeg3d* lineSeg, OdGeCurve3d* geCurveBrkDim)
{
  bool bRes = false;
  OdGePoint3d pt1, pt2;
  int numInt = 0;

  if ( geCurveBrkDim->type() == OdGe::kLineSeg3d )
    bRes = lineSeg->intersectWith(*((OdGeLineSeg3d*)geCurveBrkDim), pt1, m_tol);
  else
    bRes = ((OdGeCircArc3d*)geCurveBrkDim)->intersectWith(*lineSeg, numInt, pt1, pt2, m_tol);

  if ( bRes )
  {
    BreakPointData bkPnt;
    bkPnt.m_Type = BreakPointData::BREAK_POINT;
    bkPnt.m_Point = pt1;
    addBreakPoint(bkPnt);
    if ( numInt > 1 )
    {
      bkPnt.m_Point2 = pt2;
      addBreakPoint(bkPnt);
    }
  }
}

void OdBreakDimEngine::breakBy2StaticPoint(OdDbEntityPtr pEnt, OdGePoint3d point1, OdGePoint3d point2, OdInt32 /* index*/)
{
  bool bRes = false, bRes1 = false, bRes2 = false;
  OdGePoint3d pt1, pt2;

  // ODA_ASSERT(index >= 0 && index <= 3);

  OdDbCurvePtr curvPtr;

  BreakPointData bkPnt;
  bkPnt.m_Type = BreakPointData::BREAK_STATIC;

  if(pEnt->isKindOf(OdDbLine::desc()))
  {
    OdDbLinePtr dimLine = OdDbLine::cast(pEnt);

    OdGePoint3d startPt = dimLine->startPoint();
    OdGePoint3d endPt = dimLine->endPoint();
    OdGeVector3d  v = endPt - startPt;

    OdGeVector3d  breakLineDir = v.perpVector();
    OdGeLine3d line1, line2;
    line1.set(point1, breakLineDir);
    line2.set(point2, breakLineDir);


    OdGeLine3d geDimLine; 
    geDimLine.set(startPt, v);

    bRes1 = geDimLine.intersectWith(line1, pt1, m_tol);
    bRes2 = geDimLine.intersectWith(line2, pt2, m_tol);

    double p1 = geDimLine.paramOf(point1, m_tol);
    double p2 = geDimLine.paramOf(point2, m_tol);

    bRes1 = ( p1 >= 0.0 ) && ( p1 <= 1.0 );
    bRes2 = ( p2 >= 0.0 ) && ( p2 <= 1.0 );

    if ( bRes1 && bRes2 )
    {
      if ( p1 < p2 )
      {
        bkPnt.m_Point  = pt1;
        bkPnt.m_Point2 = pt2;
      } else
      {
        bkPnt.m_Point  = pt2;
        bkPnt.m_Point2 = pt1;
      }
      addBreakPoint(bkPnt);
    } else if ( bRes1 )
    {
      if ( p2 < 0.0 )
      {
        bkPnt.m_Point = startPt;
        bkPnt.m_Point2 = pt1;
      } else
      {
        bkPnt.m_Point = pt1;
        bkPnt.m_Point2 = endPt;
      }
      addBreakPoint(bkPnt);
    } else if ( bRes2 )
    {
      if ( p1 < 0.0 )
      {
        bkPnt.m_Point = startPt;
        bkPnt.m_Point2 = pt2;
      } else
      {
        bkPnt.m_Point = pt2;
        bkPnt.m_Point2 = endPt;
      }
      addBreakPoint(bkPnt);
    }
    return;
  } 
  if(pEnt->isKindOf(OdDbArc::desc()))
  {
    OdGePoint3d pt3, pt4;    
    int numInt = 0;

    OdGeCurve3d* geCurve = NULL;
    curvPtr = OdDbCurve::cast(pEnt);

    curvPtr->getOdGeCurve(geCurve);
    if (geCurve == NULL )
      return;
    OdGeCircArc3d* dimArc = (OdGeCircArc3d*)geCurve;

    OdGeRay3d line1, line2;
    line1.set(dimArc->center(), point1);
    line2.set(dimArc->center(), point2);

    bRes1 = dimArc->intersectWith(line1, numInt, pt1, pt3, m_tol);
    bRes2 = dimArc->intersectWith(line2, numInt, pt2, pt4, m_tol);

    if ( bRes1 && bRes2 )
    {
      bkPnt.m_Point  = pt1;
      bkPnt.m_Point2 = pt2;
      addBreakPoint(bkPnt);
    } // else do nothing
    delete(geCurve);
    return;
  }
}

void OdBreakDimEngine::intersectWithArc(OdDbEntityPtr pArc, OdGeCurve3d* geCurveBrkDim)
{
  OdGeCurve3d* geCurveBrk = NULL;
  OdDbCurvePtr curvBrkPtr = OdDbCurve::cast(pArc);
  OdResult res = curvBrkPtr->getOdGeCurve(geCurveBrk);

  if ( geCurveBrk == NULL )
    return;
  if ( geCurveBrk->type() != OdGe::kCircArc3d )
  {
    ODA_ASSERT(0);
    if ( geCurveBrk )
      delete(geCurveBrk);
    return;
  }

  bool bRes = false;
  OdGePoint3d pt1, pt2;
  int numInt = 0;

  // OdGe::EntityId entId = geCurveBrk->type();
  if ( geCurveBrkDim->type() == OdGe::kLineSeg3d )
    bRes = ((OdGeCircArc3d*)geCurveBrk)->intersectWith(*((OdGeLineSeg3d*)geCurveBrkDim), numInt, pt1, pt2, m_tol);
  else
    bRes = ((OdGeCircArc3d*)geCurveBrk)->intersectWith(*((OdGeCircArc3d*)geCurveBrkDim), numInt, pt1, pt2, m_tol);

  if ( bRes )
  {
    BreakPointData bkPnt;
    bkPnt.m_Type = BreakPointData::BREAK_POINT;
    bkPnt.m_Point = pt1;
    addBreakPoint(bkPnt);
    if ( numInt > 1 )
    {
      bkPnt.m_Point = pt2;
      addBreakPoint(bkPnt);
    }
  }
  delete(geCurveBrk);
}

void OdBreakDimEngine::intersectWithLine(OdDbEntityPtr pLine, OdGeCurve3d* geCurveBrkDim)
{
  OdGeCurve3d* geCurveBrk = NULL;
  OdDbCurvePtr curvBrkPtr = OdDbCurve::cast(pLine);
  OdResult res = curvBrkPtr->getOdGeCurve(geCurveBrk);

  if (geCurveBrk == NULL)
    return;

  bool bRes = false;
  OdGePoint3d pt1, pt2;
  int numInt = 0;

  OdGe::EntityId entId = geCurveBrk->type();
  if ( geCurveBrkDim->type() == OdGe::kLineSeg3d )
    bRes = ((OdGeLineSeg3d*)geCurveBrk)->intersectWith(*((OdGeLineSeg3d*)geCurveBrkDim), pt1, m_tol);
  else
    bRes = ((OdGeCircArc3d*)geCurveBrkDim)->intersectWith(*((OdGeLineSeg3d*)geCurveBrk), numInt, pt1, pt2, m_tol);

  if ( bRes )
  {
    BreakPointData bkPnt;
    bkPnt.m_Type = BreakPointData::BREAK_POINT;
    bkPnt.m_Point = pt1;
    addBreakPoint(bkPnt);
    if ( numInt > 1 )
    {
      bkPnt.m_Point = pt2;
      addBreakPoint(bkPnt);
    }
  }
  delete(geCurveBrk);
}

void OdBreakDimEngine::intersectWithText(OdDbEntityPtr pText, OdGeCurve3d* geCurveBrkDim)
{
  OdGePoint3dArray boundingPoints;
  OdString content;
  if ( pText->isKindOf(OdDbMText::desc()) ) 
  {
    OdDbMTextPtr mText = pText;
    content = mText->contents();
    mText->getActualBoundingPoints(boundingPoints);
  } else  // pText->isKindOf(OdDbText::desc())
  {
    OdDbTextPtr mText = pText;
    content = mText->textString();
    mText->getBoundingPoints(boundingPoints);  
  }
  ODA_TRACE1("\nText{%s}\n", content.c_str());

  OdGePoint3d pt1, pt2, pt3;
  int numInt = 0;
  bool bRes = false;
  OdDbCurvePtr curvPtr;

  if ( geCurveBrkDim->type() == OdGe::kLineSeg3d )
  {
    numInt = getIntersectionPoints((OdGeLine3d*)geCurveBrkDim, boundingPoints, pt1, pt2 );
    if ( numInt == 2 )
    {
      BreakPointData bkPnt;
      bkPnt.m_Type = BreakPointData::BREAK_TEXT;
      if ( geCurveBrkDim->paramOf(pt1) < geCurveBrkDim->paramOf(pt2) )
      {
        bkPnt.m_Point  = pt1;
        bkPnt.m_Point2 = pt2;
      } else
      {
        bkPnt.m_Point  = pt2;
        bkPnt.m_Point2 = pt1;
      }
      addBreakPoint(bkPnt);
    }
  } else
  {
    numInt = getIntersectionPoints((OdGeCircArc3d*)geCurveBrkDim, boundingPoints, pt1, pt2 );
    if ( numInt > 1 )
    {
      BreakPointData bkPnt;
      bkPnt.m_Type = BreakPointData::BREAK_TEXT;
      bkPnt.m_Point  = pt1;
      bkPnt.m_Point2 = pt2;
      addBreakPoint(bkPnt);
    }
  }
}

void OdBreakDimEngine::intersectWithPolyline(OdDbEntityPtr pPolyline, OdGeCurve3d* geCurveBrkDim)
{
  OdDbPolylinePtr curvBrkPtr = OdDbPolyline::cast(pPolyline);

  OdGeLineSeg3d  geLine;
  OdGeCircArc3d  geArc;

  OdGePoint3d pt1, pt2;

  int verts = curvBrkPtr->numVerts();
  for ( int i = 0; i < verts; i++ )
  {
    OdDbPolyline::SegType type = curvBrkPtr->segType(i);
    int numInt = 0;
    bool bRes = false;

    if ( type == OdDbPolyline::kLine )
    {
      curvBrkPtr->getLineSegAt(i, geLine);
      if ( geCurveBrkDim->type() == OdGe::kLineSeg3d )
        bRes = ((OdGeLineSeg3d*)geCurveBrkDim)->intersectWith(geLine, pt1, m_tol);
      else
        bRes = ((OdGeCircArc3d*)geCurveBrkDim)->intersectWith(geLine, numInt, pt1, pt2, m_tol);

    } else if ( type == OdDbPolyline::kArc )
    {
      curvBrkPtr->getArcSegAt(i, geArc);
      if ( geCurveBrkDim->type() == OdGe::kLineSeg3d )
        bRes = geArc.intersectWith(*((OdGeLineSeg3d*)geCurveBrkDim), numInt, pt1, pt2, m_tol);
      else
        bRes = ((OdGeCircArc3d*)geCurveBrkDim)->intersectWith(geArc, numInt, pt1, pt2, m_tol);
    }

    if ( bRes )
    {
      BreakPointData bkPnt;
      bkPnt.m_Type = BreakPointData::BREAK_POINT;
      bkPnt.m_Point = pt1;
      addBreakPoint(bkPnt);
      if ( numInt > 1 )
      {
        bkPnt.m_Point = pt1;
        addBreakPoint(bkPnt);
      }
    }
  }
}

void OdBreakDimEngine::intersectWith2DPolyline (OdDbEntityPtr p2DPLine, OdGeCurve3d* geCurveBrkDim)
{
  OdDb2dPolylinePtr curvBrkPtr = OdDb2dPolyline::cast(p2DPLine);
  OdDb::Poly2dType type = curvBrkPtr->polyType();

  if ( type == OdDb::k2dSimplePoly || type == OdDb::k2dFitCurvePoly )
  {
    OdDbPolylinePtr pLine = OdDbPolyline::createObject();
    OdResult res = pLine->convertFrom(curvBrkPtr, false);
    if ( res == eOk )
      return intersectWithPolyline(pLine, geCurveBrkDim);
  } 
}

void OdBreakDimEngine::intersectWithEllipse(OdDbEntityPtr pEllipse, OdGeCurve3d* geCurveBrkDim)
{
  OdGeCurve3d* geCurveBrk = NULL;
  OdDbCurvePtr curvBrkPtr = OdDbCurve::cast(pEllipse);
  OdResult res = curvBrkPtr->getOdGeCurve(geCurveBrk);
  OdGe::EntityId idTp = geCurveBrk->type();

  if (geCurveBrk == NULL)
    return;
  if ( geCurveBrk->type() != OdGe::kEllipArc3d )
  {
    ODA_ASSERT(0);
    if ( geCurveBrk )
      delete(geCurveBrk);
    return;
  }

  OdGePoint3d pt1, pt2;
  int numInt = 0;
  bool bRes = false;
  if ( geCurveBrkDim->type() == OdGe::kLineSeg3d )
  {
    bRes = ((OdGeEllipArc3d*)geCurveBrk)->intersectWith(*((OdGeLineSeg3d*)geCurveBrkDim), numInt, pt1, pt2, m_tol);
    if ( bRes )
    {
      BreakPointData bkPnt;
      bkPnt.m_Type = BreakPointData::BREAK_POINT;
      bkPnt.m_Point = pt1;
      addBreakPoint(bkPnt);
      if ( numInt > 1 )
      {
        bkPnt.m_Point = pt2;
        addBreakPoint(bkPnt);
      }
    }
  } else  // ellips with arc
  {
    double minor = ((OdGeEllipArc3d*)geCurveBrk)->minorRadius();
    double approxEps = minor*0.01;

    OdGePoint3dArray pnts;
    geCurveBrk->getSamplePoints(NULL, approxEps, pnts);

    OdGeLineSeg3d lineSeg;
    int sz = pnts.size();
    for (int i = 0; i < sz-1; i++ )
    {
      lineSeg.set(pnts[i], pnts[i+1]);
      intersectWithGeLine(&lineSeg, geCurveBrkDim);
    }
  }
  delete(geCurveBrk);
}

void OdBreakDimEngine::intersectWithSpline(OdDbEntityPtr pSpline, OdGeCurve3d* geCurveBrkDim)
{
  OdDbSplinePtr pSpl = OdDbSpline::cast(pSpline);

  OdGeCurve3d* geCurve = NULL;
  OdDbCurvePtr curvPtr = OdDbCurve::cast(pSpl);

  OdResult res = curvPtr->getOdGeCurve(geCurve);
  if ( res != eOk )
    return;

  OdGeNurbCurve3d* nurbCurv = (OdGeNurbCurve3d*)geCurve;

  OdGeExtents3d extents;
  pSpl->getGeomExtents(extents);
  double approxEps = (extents.minPoint().distanceTo(extents.maxPoint()))*0.001;

  OdGePoint3dArray pnts;
  nurbCurv->getSamplePoints(NULL, approxEps, pnts);

  OdGeLineSeg3d lineSeg;
  int sz = pnts.size();
  for (int i = 0; i < sz-1; i++ )
  {
    lineSeg.set(pnts[i], pnts[i+1]);
    intersectWithGeLine(&lineSeg, geCurveBrkDim);
  }
  delete(geCurve);
}

static bool getHookPoint(OdDbLeaderPtr pLdr, OdGePoint3d& p1, OdGePoint3d& p2)
{
  if (pLdr->annoType() == OdDbLeader::kMText)
  {
    p1 = pLdr->lastVertex();
    p2 = p1;

    OdDbMTextPtr mText = OdDbMText::cast(pLdr->annotationObjId().safeOpenObject());
    double annoWidth = mText->actualWidth();
    double dHookLen = 0.0;

    double dSize = pLdr->dimscale();
    if (OdZero(dSize))
      dSize = 1.;
    else
      dSize *= pLdr->dimasz();

    if (!pLdr->isHookLineOnXDir())
    {
      dSize = -dSize;
    }
    if (pLdr->hasHookLine())
    {
      p1 += pLdr->annotationXDir() * dSize;
    }


    if (pLdr->dimtad() != 0)
    {
      dHookLen = annoWidth + ::fabs(pLdr->dimgap());
      if (!pLdr->isHookLineOnXDir())
      {
        dHookLen = -dHookLen;
      }
    }
    p2 -= pLdr->annotationXDir() * dHookLen;
    return true;
  }
  return false;
}

void OdBreakDimEngine::intersectWithLeader(OdDbEntityPtr pLeader, OdGeCurve3d* geCurveBrkDim)
{
  bool bRet = false;
  OdDbLeaderPtr pLdr = OdDbLeader::cast(pLeader);

  OdInt32 numVert = pLdr->numVertices();
  bool bSpine = pLdr->isSplined();

  OdGePoint3dArray Points; 
  Points.resize(numVert);
  for (int i = 0; i < numVert; i++ )
  {
    Points[i] = pLdr->vertexAt(i);
  }

  if ( !bSpine )
  {
    OdGePoint3d p1, p2;
    if (getHookPoint(pLdr, p1, p2))
    {
      unsigned sz = Points.size() + 1;
      Points.resize(sz);
      Points[sz - 2] = p1;
      Points[sz - 1] = p2;
      numVert++;
    }
    OdGeLineSeg3d lineSeg;
    for (int i = 0; i < numVert-1; i++ )
    {
      lineSeg.set(Points[i], Points[i+1]);
      intersectWithGeLine(&lineSeg, geCurveBrkDim);
    }
  } else
  {
    OdGeNurbCurve3d NurbCurve;
    OdGeVector3d endTangent;
    OdGeError status; 
    if ( !pLdr->annotationObjId().isNull() )
    {
      if ( pLdr->isHookLineOnXDir() )
        endTangent = -pLdr->annotationXDir();
      else
        endTangent = pLdr->annotationXDir();
    }
    OdGeVector3d startTangent(Points[1] - Points[0]);

    NurbCurve.setFitData(Points, startTangent.normalize(OdGeContext::gTol, status),
      endTangent.normalize(OdGeContext::gTol, status));

    OdGePoint3dArray pnts;
    NurbCurve.getSamplePoints(NurbCurve.numControlPoints()*3, pnts);

    OdGeLineSeg3d lineSeg;
    int sz = pnts.size();
    for (int i = 0; i < sz-1; i++ )
    {
      lineSeg.set(pnts[i], pnts[i+1]);
      intersectWithGeLine(&lineSeg, geCurveBrkDim);
    }
  }
}

void OdBreakDimEngine::intersectWithMLeader(OdDbEntityPtr pMLeader, OdGeCurve3d* geCurveBrkDim)
{
  OdDbMLeaderPtr leader = OdDbMLeader::cast(pMLeader);
  
  OdRxObjectPtrArray ents;
  leader->explodeGeometry(ents);
  OdRxObjectPtrArray::iterator pEntIter;
  for (pEntIter = ents.begin(); pEntIter != ents.end(); pEntIter++)
  {
    OdDbEntityPtr entPtr = *pEntIter;
    if (entPtr->isKindOf(OdDbLine::desc()))
    {
      intersectWithLine(entPtr, geCurveBrkDim);
    }
    else if (entPtr->isKindOf(OdDbMText::desc()))
    {
      intersectWithText(entPtr, geCurveBrkDim);
    }
    else if (entPtr->isKindOf(OdDb2dPolyline::desc()))
    {
      intersectWith2DPolyline(entPtr, geCurveBrkDim);
    }
  }
}

void OdBreakDimEngine::intersectWithDimension(OdDbEntityPtr pDim, OdGeCurve3d* geCurveBrkDim)
{
  OdDbDimensionPtr dimension = OdDbDimension::cast(pDim);
  OdDbBlockTableRecordPtr dimBlock = OdDbBlockTableRecord::cast(dimension->dimBlock());
  
  if (dimBlock.isNull())
  { 
    // MT loading. Possible situation - there is no dimension block yet.
    // It seems safe to call recomputeDimBlock() here because decomposeForLoad() is called after reading drawing.
    dimension->upgradeOpen();
    dimension->recomputeDimBlock();
    dimBlock = OdDbBlockTableRecord::cast(dimension->dimBlock());
    dimension->downgradeOpen();
  }

  OdDbObjectIteratorPtr pEntIter;
  for(pEntIter = dimBlock->newIterator(); !pEntIter->done(); pEntIter->step())
  {
    OdDbEntityPtr entPtr = pEntIter->entity();
    if ( entPtr->isKindOf(OdDbLine::desc()) )
    {
      intersectWithLine(entPtr, geCurveBrkDim);
    } else if ( entPtr->isKindOf(OdDbArc::desc()) )
    {
      intersectWithArc(entPtr, geCurveBrkDim);
    } else if ( entPtr->isKindOf(OdDbMText::desc()) )
    {
      intersectWithText(entPtr, geCurveBrkDim);
    } 
  }
}

void OdBreakDimEngine::intersectWithBlock(OdDbEntityPtr pBlock, OdGeCurve3d* geCurveBrkDim)
{
  OdDbBlockReferencePtr blkRef = OdDbBlockReference::cast(pBlock);

  OdDbBlockTableRecordPtr block = blkRef->blockTableRecord().safeOpenObject();
  OdGeMatrix3d trans = blkRef->blockTransform();

  OdDbObjectIteratorPtr pEntIter;
  OdDbEntityPtr entPtr;

  for(pEntIter = block->newIterator(); !pEntIter->done(); pEntIter->step())
  {
    pEntIter->entity()->getTransformedCopy(trans, entPtr);
    if ( entPtr->isKindOf(OdDbLine::desc()) || entPtr->isKindOf(OdDbXline::desc()) 
      || entPtr->isKindOf(OdDbRay::desc()) )
    {
      intersectWithLine(entPtr, geCurveBrkDim);
    } else if ( entPtr->isKindOf(OdDbArc::desc()) || entPtr->isKindOf(OdDbCircle::desc()) )
    {
      intersectWithArc(entPtr, geCurveBrkDim);
    } else if ( entPtr->isKindOf(OdDbMText::desc()) || entPtr->isKindOf(OdDbText::desc()) )
    {
      intersectWithText(entPtr, geCurveBrkDim);
    } else if ( entPtr->isKindOf(OdDbPolyline::desc()) )
    {
      intersectWithPolyline(entPtr, geCurveBrkDim);
    } else if ( entPtr->isKindOf(OdDbEllipse::desc()) )
    {
      intersectWithEllipse(entPtr, geCurveBrkDim);
    } else if ( entPtr->isKindOf(OdDbSpline::desc()) ) 
    {
      intersectWithSpline(entPtr, geCurveBrkDim);
    } else if ( entPtr->isKindOf(OdDbLeader::desc()) ) 
    {
      intersectWithLeader(entPtr, geCurveBrkDim);
    } else if ( entPtr->isKindOf(OdDbDimension::desc()) )
    {
      intersectWithDimension(entPtr, geCurveBrkDim);
    } else if ( entPtr->isKindOf(OdDbBlockReference::desc()) )
    {
      intersectWithBlock(entPtr, geCurveBrkDim);
    }
  }
  OdDbObjectIteratorPtr attrIter =  blkRef->attributeIterator();
  while (!attrIter->done())
  {
    OdDbEntityPtr pTextAttr = attrIter->entity();
    intersectWithText(pTextAttr, geCurveBrkDim);
    attrIter->step();
  }
}

bool OdBreakDimEngine::breakDimensionLine()
{
  int sz = m_BreakPoints.size();
  if ( sz == 0 )
    return false;

  if ( m_curEntity->isKindOf(OdDbLine::desc()) )
  {
    OdDbLinePtr pLine = OdDbLine::cast(m_curEntity);
    // OdGePoint3d p = pLine->startPoint();

    OdDbLinePtr newLine = pLine->clone();
    OdDbLinePtr nextNewLine;

    for(int i = 0; i < sz; i++)
    {
      OdGeLineSeg3d geLine;
      geLine.set(newLine->startPoint(), newLine->endPoint());

      bool b1 = geLine.isOn(m_BreakPoints[i].m_Point, m_tol);
      bool b2 = geLine.isOn(m_BreakPoints[i].m_Point2, m_tol);

      if ( !b1 && !b2 )
      {
        continue;
        // ODA_ASSERT(false);
      }

      if ( b1 && b2 )
      {
        nextNewLine = newLine->clone();

        newLine->setEndPoint(m_BreakPoints[i].m_Point);
        nextNewLine->setStartPoint(m_BreakPoints[i].m_Point2);

        if ( !newLine->startPoint().isEqualTo(newLine->endPoint(), m_tol))
          m_newDimEnts.push_back(newLine);
        newLine = nextNewLine;

      } else if ( b1 )
      {
        newLine->setEndPoint(m_BreakPoints[i].m_Point);
      } else if ( b2 )
      {
        newLine->setStartPoint(m_BreakPoints[i].m_Point2);
      }
    }
    if ( !newLine->startPoint().isEqualTo(newLine->endPoint(), m_tol))
      m_newDimEnts.push_back(newLine);
  } else
  {
    OdDbArcPtr pArc = OdDbArc::cast(m_curEntity);

    OdDbArcPtr newArc = pArc->clone();
    OdDbArcPtr nextNewArc;

    for(int i = 0; i < sz; i++)
    {
      double stAng = newArc->startAngle();
      double endAng = newArc->endAngle();
      if ( endAng < stAng )
      {
        endAng += Oda2PI;
      } else
      {
         if ( m_BreakPoints[i].m_Angle > Oda2PI )
         {
           m_BreakPoints[i].m_Angle -= Oda2PI;
           m_BreakPoints[i].m_Angle2 -= Oda2PI;
         }
      }
      bool b1 = m_BreakPoints[i].m_Angle > stAng;
      bool b2 = m_BreakPoints[i].m_Angle2 < endAng;

      if ( !b1 && !b2 )
      {
        continue;
        // ODA_ASSERT(false);
      }

      if ( b1 && b2 )
      {
        nextNewArc = newArc->clone();
        newArc->setEndAngle(m_BreakPoints[i].m_Angle);
        nextNewArc->setStartAngle(m_BreakPoints[i].m_Angle2);
        m_newDimEnts.push_back(newArc);
        newArc = nextNewArc;
      } else if ( b1 )
      {
        newArc->setEndAngle(m_BreakPoints[i].m_Angle);
      } else if ( b2 )
      {
        newArc->setStartAngle(m_BreakPoints[i].m_Angle2);
      }
    }
    m_newDimEnts.push_back(newArc);
  }
  return true;
}

