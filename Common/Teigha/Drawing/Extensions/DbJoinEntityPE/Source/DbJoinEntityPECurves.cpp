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
#include "DbJoinEntityPECurves.h"
#include "OdError.h"
#include "Ge/GeGbl.h"
#include "Ge/GeNurbCurve3d.h"
#include "Ge/GeCompositeCurve3d.h"
#include "Ge/GeEllipArc3d.h"
#include "Ge/GeCircArc3d.h"
#include "Ge/GeLineSeg3d.h"
#include "DbCurve.h"
#include "DbLine.h"
#include "DbArc.h"
#include "DbEllipse.h"
#include "Db2dPolyline.h"
#include "Db3dPolyline.h"
#include "DbPolyline.h"
#include "DbSpline.h"
#include "Db3dPolylineVertex.h"
#include "Ge/GePoint2dArray.h"
#include "Int32Array.h"
#include "Ge/GeKnotVector.h"
#define STL_USING_LIST
#define STL_USING_ALGORITHM
#define STL_USING_FUNCTIONAL
#define STL_USING_SET
#include "OdaSTL.h"

namespace joinEntityPE
{
struct CurveEndPoint
{
  CurveEndPoint() : m_iCrv(-1), m_bStart(false) {}
  OdGePoint3d m_pt;
  int m_iCrv;
  bool m_bStart;
};

struct CurveIndex
{
  CurveIndex() : m_index(-1), m_bReversed(false) {}
  int m_index;
  bool m_bReversed;
};

class ComparerPositionAndIndex : public std::binary_function<CurveEndPoint, CurveEndPoint, bool>
{
public:
  bool operator()(const CurveEndPoint& a1, const CurveEndPoint& a2)
  {
    ODA_ASSERT(a1.m_iCrv >= 0 && a2.m_iCrv >= 0);
    if(!OdEqual(a1.m_pt.x, a2.m_pt.x))
    {
      return a1.m_pt.x < a2.m_pt.x;
    }
    if(!OdEqual(a1.m_pt.y, a2.m_pt.y))
    {
      return a1.m_pt.y < a2.m_pt.y;
    }
    if(!OdEqual(a1.m_pt.z, a2.m_pt.z))
    {
      return a1.m_pt.z < a2.m_pt.z;
    }
    return a1.m_iCrv < a2.m_iCrv;
  }
};

class ComparerPosition : public std::binary_function<CurveEndPoint, CurveEndPoint, bool>
{
public:
  bool operator()(const CurveEndPoint& a1, const CurveEndPoint& a2)
  {
    if(!OdEqual(a1.m_pt.x, a2.m_pt.x))
    {
      return a1.m_pt.x < a2.m_pt.x;
    }
    if(!OdEqual(a1.m_pt.y, a2.m_pt.y))
    {
      return a1.m_pt.y < a2.m_pt.y;
    }
    return OdLess(a1.m_pt.z, a2.m_pt.z);
  }
};

bool findCurveSequence(std::list<CurveIndex>& listIndex, const OdDbCurvePtr primaryEntity, const OdDbEntityPtrArray& otherEntities, const OdRxClass** aObjDesc, int nObjDesc, bool bCheckPlanarity)
{
  if(primaryEntity->isClosed())
  {
    return false;
  }
  CurveEndPoint aPtEndPrimary[2];
  if(primaryEntity->getStartPoint(aPtEndPrimary[0].m_pt) != eOk || primaryEntity->getEndPoint(aPtEndPrimary[1].m_pt) != eOk)
  {
    return false;
  }
  OdGeVector3d normalPrimary;
  {
    OdGePlane plane;
    OdDb::Planarity planarity;
    primaryEntity->getPlane(plane, planarity);
    normalPrimary = plane.normal();
  }

  // find all ends candidates
  OdArray<CurveEndPoint> aPtCurve;
  unsigned int i, nEnt = otherEntities.logicalLength();
  for(i = 0; i < nEnt; ++i)
  {
    OdDbCurvePtr curve = OdDbCurve::cast(otherEntities[i]);
    if(curve.isNull())
    {
      continue;
    }

    // apply type filter
    int k;
    for(k = 0; k < nObjDesc; ++k)
    {
      if(curve->isKindOf(aObjDesc[k]))
      {
        break;
      }
    }
    if(nObjDesc > 0 && k == nObjDesc)
    {
      continue;
    }

    OdGePoint3d ptStart, ptEnd;
    if(curve->getStartPoint(ptStart) != eOk || curve->getEndPoint(ptEnd) != eOk)
    {
      continue;
    }
    if(ptStart.isEqualTo(ptEnd))
    {
      continue;
    }

    if(bCheckPlanarity)
    {
      OdGePlane plane;
      OdDb::Planarity planarity;
      curve->getPlane(plane, planarity);
      if(!plane.isOn(ptStart) || !plane.isOn(ptEnd))
      {
        continue;
      }
      if(planarity == OdDb::kNonPlanar || (planarity == OdDb::kPlanar && !normalPrimary.isParallelTo(plane.normal())))
      {
        continue;
      }
    }

    CurveEndPoint pt1;
    pt1.m_pt = ptStart;
    pt1.m_iCrv = i;
    pt1.m_bStart = true;
    aPtCurve.push_back(pt1);
    CurveEndPoint pt2;
    pt2.m_pt = ptEnd;
    pt2.m_iCrv = i;
    pt2.m_bStart = false;
    aPtCurve.push_back(pt2);
  }

  // sort to put equal points together
  ComparerPositionAndIndex comparerPositionIndex;
  std::sort(aPtCurve.begin(), aPtCurve.end(), comparerPositionIndex);

  // form the curves sequence to join, find end2end pairs
  ComparerPosition comparerPosition;
  CurveIndex delimeter;
  delimeter.m_index = -1;
  listIndex.push_back(delimeter);
  std::set<int> setUsedIndices;
  for(i = 0; i < 2; ++i)
  {
    CurveEndPoint ptNext = aPtEndPrimary[i];
    int indexCurrent = -1;

    for(;;)
    {
      std::pair<OdArray<CurveEndPoint>::iterator, OdArray<CurveEndPoint>::iterator> bounds;
      bounds = std::equal_range(aPtCurve.begin(), aPtCurve.end(), ptNext, comparerPosition);
      if(bounds.first == bounds.second) // not found
      {
        ODA_ASSERT(indexCurrent < 0);
        break;
      }
      if(indexCurrent >= 0 && bounds.second - bounds.first < 2) // no pair found
      {
        break;
      }
      CurveEndPoint& pt = indexCurrent < 0 || bounds.first->m_iCrv != indexCurrent ? *(bounds.first) : *(bounds.first + 1);
      indexCurrent = pt.m_iCrv;
      CurveIndex curveIndex;
      curveIndex.m_index = indexCurrent;

      // check for loop
      if(std::find(setUsedIndices.begin(), setUsedIndices.end(), indexCurrent) != setUsedIndices.end())
      {
        break;
      }

      if(i == 0)
      {
        curveIndex.m_bReversed = pt.m_bStart == true;
        listIndex.push_front(curveIndex);
      }
      else
      {
        curveIndex.m_bReversed = pt.m_bStart == false;
        listIndex.push_back(curveIndex);
      }
      setUsedIndices.insert(indexCurrent);

      // find next point
      if(pt.m_bStart)
      {
        OdDbCurve::cast(otherEntities[indexCurrent])->getEndPoint(ptNext.m_pt);
      }
      else
      {
        OdDbCurve::cast(otherEntities[indexCurrent])->getStartPoint(ptNext.m_pt);
      }

      // check for loop
      if(ptNext.m_pt.isEqualTo(aPtEndPrimary[i].m_pt)) // cycle from one end
      {
        break;
      }
      if(ptNext.m_pt.isEqualTo(aPtEndPrimary[1 - i].m_pt)) // contour closes the poly
      {
        return true;
      }
    }
  }

  return listIndex.size() > 1;
}

bool getNurbs(const OdDbCurvePtr curve, OdGeNurbCurve3d*& nurbs)
{
  nurbs = NULL;

  if(curve->isA() == OdDbSpline::desc())
  {
    OdDbSplinePtr spline = curve;
    int degree;
    bool rational, closed, periodic;
    OdGePoint3dArray controlPoints;
    OdGeKnotVector knots;
    OdGeDoubleArray weights;
    double controlPtTol;
    spline->getNurbsData(degree, rational, closed, periodic, controlPoints, knots, weights, controlPtTol);
    nurbs = new OdGeNurbCurve3d(degree, knots, controlPoints, weights, periodic);
    return true;
  }

  OdGeCurve3d* pGeCurve = NULL;
  OdResult res = curve->getOdGeCurve(pGeCurve);
  if(res != eOk)
  {
    return false;
  }

  switch(pGeCurve->type())
  {
  case OdGe::kCircArc3d:
    nurbs = new OdGeNurbCurve3d(OdGeEllipArc3d(*static_cast<OdGeCircArc3d*>(pGeCurve)));
    break;
  case OdGe::kEllipArc3d:
    nurbs = new OdGeNurbCurve3d(*static_cast<const OdGeEllipArc3d*>(pGeCurve));
    break;
  case OdGe::kLineSeg3d:
    nurbs = new OdGeNurbCurve3d(*static_cast<const OdGeLineSeg3d*>(pGeCurve));
    break;
  case OdGe::kNurbCurve3d:
    nurbs = new OdGeNurbCurve3d(*static_cast<const OdGeNurbCurve3d*>(pGeCurve));
    break;
  case OdGe::kCompositeCrv3d:
    {
      OdGeCompositeCurve3d* pComposite = (OdGeCompositeCurve3d*)pGeCurve;
      OdGeCurve3dPtrArray aCrv;
      pComposite->getCurveList(aCrv);
      unsigned int i;
      for(i = 0; i < aCrv.length(); ++i)
      {
        const OdGeCurve3d* pCrv = aCrv[i].get();
        OdGeNurbCurve3d* tmp = NULL;
        if(pCrv->type() == OdGe::kLineSeg3d)
        {
          const OdGeLineSeg3d* seg = (const OdGeLineSeg3d*)pCrv;
          tmp = new OdGeNurbCurve3d(*seg);
        }
        else if(pCrv->type() == OdGe::kCircArc3d)
        {
          const OdGeCircArc3d* arc = (const OdGeCircArc3d*)pCrv;
          tmp = new OdGeNurbCurve3d(*arc);
        }
        else
        {
          ODA_ASSERT(0);
        }
        if(i == 0)
        {
          nurbs = tmp;
        }
        else
        {
          try
          {
            nurbs->joinWith(*tmp);
          }
          catch (...)
          {
            delete tmp;
            return false;
          }
          delete tmp;
        }
      }
    }
    break;
  default:
    {
      ODA_ASSERT(0);
    }
  }
  delete pGeCurve;

  return true;
}

// returns true is circle becomes closed
bool calcArcAngles(double angBeg, double angEnd, double paramMin, double& paramMax)
{
  while(angEnd > paramMax)
  {
    angBeg -= Oda2PI;
    angEnd -= Oda2PI;
  }
  while(angEnd < paramMin)
  {
    angBeg += Oda2PI;
    angEnd += Oda2PI;
  }
  if(OdEqual(angEnd, paramMin) 
    || (angEnd > paramMin && angEnd < paramMax && angBeg < paramMin)) // full circle
  {
    paramMax = paramMin + Oda2PI;
    return true;
  }
  if(angEnd > paramMax)
  {
    paramMax = angEnd;
  }
  return false;
}

OdResult OdDbLineJoinEntityPE::joinEntities(OdDbEntityPtr primaryEntity, const OdDbEntityPtrArray& otherEntities, OdGeIntArray& joinedEntityIndices) const
{
  joinedEntityIndices.clear();
  OdDbLinePtr pPrimary = OdDbLine::cast(primaryEntity);
  if(pPrimary.isNull())
  {
    return eInvalidInput;
  }
  const OdGePoint3d ptStartPrimary = pPrimary->startPoint();
  const OdGePoint3d ptEndPrimary = pPrimary->endPoint();
  OdGeLine3d geLine(ptStartPrimary, ptEndPrimary);

  unsigned int i, j, nEnt = otherEntities.logicalLength();
  double paramMin = 0.;
  double paramMax = 1.;
  for(i = 0; i < nEnt; ++i)
  {
    OdDbLinePtr line = OdDbLine::cast(otherEntities[i]);
    if(line.isNull())
    {
      continue;
    }

    OdGePoint3d aPtEnd[2];
    aPtEnd[0] = line->startPoint();
    aPtEnd[1] = line->endPoint();

    if(!geLine.isOn(aPtEnd[0]) || !geLine.isOn(aPtEnd[1]))
    {
      continue;
    }

    for(j = 0; j < 2; ++j)
    {
      const double param = geLine.paramOf(aPtEnd[j]);
      if(param < paramMin)
      {
        paramMin = param;
      }
      else if(param > paramMax)
      {
        paramMax = param;
      }
    }

    joinedEntityIndices.push_back(i);
  }

  if(joinedEntityIndices.isEmpty())
  {
    return eInvalidInput;
  }

  pPrimary->setStartPoint(geLine.evalPoint(paramMin));
  pPrimary->setEndPoint(geLine.evalPoint(paramMax));
  return eOk;
}

OdResult OdDbArcJoinEntityPE::joinEntities(OdDbEntityPtr primaryEntity, const OdDbEntityPtrArray& otherEntities, OdGeIntArray& joinedEntityIndices) const
{
  joinedEntityIndices.clear();
  OdDbArcPtr pPrimary = OdDbArc::cast(primaryEntity);
  if(pPrimary.isNull())
  {
    return eInvalidInput;
  }
  const OdGePoint3d center = pPrimary->center();
  const OdGeVector3d normal = pPrimary->normal();
  const double radius = pPrimary->radius();

  unsigned int i, nEnt = otherEntities.logicalLength();
  const double paramMin = pPrimary->startAngle();
  double paramMax = pPrimary->endAngle();
  if(OdEqual(paramMax - paramMin, Oda2PI)) // closed
  {
    return eInvalidInput;
  }
  bool bFullCircle = false;
  for(i = 0; i < nEnt; ++i)
  {
    OdDbArcPtr arc = OdDbArc::cast(otherEntities[i]);
    if(arc.isNull())
    {
      continue;
    }

    if(!arc->center().isEqualTo(center) 
      || !OdEqual(arc->radius(), radius) 
      || !arc->normal().isEqualTo(normal))
    {
      continue;
    }

    if(!bFullCircle)
    {
      double angBeg = arc->startAngle();
      double angEnd = arc->endAngle();
      bFullCircle = joinEntityPE::calcArcAngles(angBeg, angEnd, paramMin, paramMax);
    }

    joinedEntityIndices.push_back(i);
  }

  if(joinedEntityIndices.isEmpty())
  {
    return eInvalidInput;
  }

  if(paramMax > paramMin + Oda2PI)
  {
    paramMax = paramMin + Oda2PI;
  }
  pPrimary->setEndAngle(paramMax);
  return eOk;
}

OdResult OdDbEllipseJoinEntityPE::joinEntities(OdDbEntityPtr primaryEntity, const OdDbEntityPtrArray& otherEntities, OdGeIntArray& joinedEntityIndices) const
{
  joinedEntityIndices.clear();
  OdDbEllipsePtr pPrimary = OdDbEllipse::cast(primaryEntity);
  if(pPrimary.isNull() || pPrimary->isClosed())
  {
    return eInvalidInput;
  }

  unsigned int i, nEnt = otherEntities.logicalLength();
  const OdGePoint3d center = pPrimary->center();
  const OdGeVector3d majorAxis = pPrimary->majorAxis();
  const OdGeVector3d minorAxis = pPrimary->minorAxis();
  const double radiusRatio = pPrimary->radiusRatio();
  const double paramMin = pPrimary->startAngle();
  double paramMax = pPrimary->endAngle();
  bool bFullEllipse = false;
  for(i = 0; i < nEnt; ++i)
  {
    OdDbEllipsePtr arc = OdDbEllipse::cast(otherEntities[i]);
    if(arc.isNull())
    {
      continue;
    }

    if(!arc->center().isEqualTo(center) 
      || !OdEqual(arc->radiusRatio(), radiusRatio) 
      || !arc->majorAxis().isEqualTo(majorAxis)
      || !arc->minorAxis().isEqualTo(minorAxis))
    {
      continue;
    }

    if(!bFullEllipse)
    {
      double angBeg = arc->startAngle();
      double angEnd = arc->endAngle();
      bFullEllipse = joinEntityPE::calcArcAngles(angBeg, angEnd, paramMin, paramMax);
    }

    joinedEntityIndices.push_back(i);
  }

  if(joinedEntityIndices.isEmpty())
  {
    return eInvalidInput;
  }

  if(paramMax > paramMin + Oda2PI)
  {
    paramMax = paramMin + Oda2PI;
  }
  pPrimary->setEndAngle(paramMax);
  return eOk;
}

OdResult OdDb3dPolylineJoinEntityPE::joinEntities(OdDbEntityPtr primaryEntity, const OdDbEntityPtrArray& otherEntities, OdGeIntArray& joinedEntityIndices) const
{
  joinedEntityIndices.clear();
  OdDb3dPolylinePtr pPrimary = OdDb3dPolyline::cast(primaryEntity);
  if(pPrimary.isNull())
  {
    return eInvalidInput;
  }
  
  std::list<joinEntityPE::CurveIndex> listIndex;
  const OdRxClass* aObjDesc[] = {OdDbLine::desc(), OdDbArc::desc(), OdDbEllipse::desc(), OdDbPolyline::desc(), OdDb2dPolyline::desc(), OdDb3dPolyline::desc(), OdDbSpline::desc()};
  if(!joinEntityPE::findCurveSequence(listIndex, pPrimary, otherEntities, aObjDesc, 7, false))
  {
    return eInvalidInput;
  }

  // form the extended poly
  OdGePoint3dArray aPt;
  std::list<joinEntityPE::CurveIndex>::iterator index;
  for(index = listIndex.begin(); index != listIndex.end(); ++index)
  {
    if(index->m_index < 0) // place source curve
    {
      for(OdDbObjectIteratorPtr it = pPrimary->vertexIterator(); !it->done(); it->step())
      {
        OdDb3dPolylineVertexPtr pVert = it->entity();
        if(pVert->vertexType() != OdDb::k3dSimpleVertex)
        {
          continue;
        }
        const OdGePoint3d pt = pVert->position();
        if(!aPt.isEmpty() && aPt.last().isEqualTo(pt))
        {
          continue;
        }
        aPt.push_back(pt);
      }
      continue;
    }

    OdDbCurvePtr curve = otherEntities[index->m_index];

    // avoid pt doubling
    if(!aPt.isEmpty())
    {
      aPt.setLogicalLength(aPt.logicalLength() - 1); 
    }

    // if curve is poly, collect vertices. bulges are ignored
    OdGePoint3dArray aPtPoly;
    ODA_ASSERT(!curve->isClosed());
    if(curve->isA() == OdDb2dPolyline::desc())
    {
      OdDb2dPolylinePtr pPoly2d = curve;
      for(OdDbObjectIteratorPtr it = pPoly2d->vertexIterator(); !it->done(); it->step())
      {
        OdDb2dVertexPtr pVert = it->entity();
        if(pVert->vertexType() != OdDb::k2dVertex)
        {
          continue;
        }
        const OdGePoint3d pt = pVert->position();
        if(!aPtPoly.isEmpty() && aPtPoly.last().isEqualTo(pt))
        {
          continue;
        }
        aPtPoly.push_back(pt);
      }
    }
    else if(curve->isA() == OdDbPolyline::desc())
    {
      OdDbPolylinePtr pPoly = curve;
      for(unsigned int i = 0; i < pPoly->numVerts(); ++i)
      {
        OdGePoint3d pt;
        pPoly->getPointAt(i, pt);
        if(!aPtPoly.isEmpty() && aPtPoly.last().isEqualTo(pt))
        {
          continue;
        }
        aPtPoly.push_back(pt);
      }
    }
    else if(curve->isA() == OdDb3dPolyline::desc())
    {
      OdDb3dPolylinePtr pPoly3d = curve;
      for(OdDbObjectIteratorPtr it = pPoly3d->vertexIterator(); !it->done(); it->step())
      {
        OdDb3dPolylineVertexPtr pVert = it->entity();
        if(pVert->vertexType() != OdDb::k3dSimpleVertex)
        {
          continue;
        }
        const OdGePoint3d pt = pVert->position();
        if(!aPtPoly.isEmpty() && aPtPoly.last().isEqualTo(pt))
        {
          continue;
        }
        aPtPoly.push_back(pt);
      }
    }
    if(!aPtPoly.isEmpty())
    {
      if(index->m_bReversed)
      {
        aPtPoly.reverse();
      }
      aPt.append(aPtPoly);
    }
    else
    {
      OdGeNurbCurve3d* nurbs = NULL;
      bool res = getNurbs(curve, nurbs);
      ODA_ASSERT(res);
      if(index->m_bReversed)
      {
        nurbs->reverseParam();
      }
      nurbs->appendSamplePoints(NULL, 0., aPt);
      delete nurbs;
    }

    joinedEntityIndices.push_back(index->m_index);
  }

  // nothing found to join
  if(joinedEntityIndices.isEmpty())
  {
    return eInvalidInput;
  }

  // set new data
  /*for(OdDbObjectIteratorPtr it = pPrimary->vertexIterator(); !it->done(); it->step())
  {
    OdDb3dPolylineVertexPtr pVert = it->entity();
    pVert->erase();
  }
  pPrimary->setPolyType(OdDb::k3dSimplePoly);*/
  OdGeCompositeCurve3d composite;
  composite.setCurveList(NULL, 0);
  pPrimary->setFromOdGeCurve(composite);
  OdDbObjectId id;
  for(unsigned int i = 0; i < aPt.logicalLength(); ++i)
  {
    OdDb3dPolylineVertexPtr pAddedVertex = OdDb3dPolylineVertex::createObject();
    pAddedVertex->setPosition(aPt[i]);
    pAddedVertex->setVertexType(OdDb::k3dSimpleVertex);
    id = pPrimary->insertVertexAt(id, pAddedVertex);
  }

  return eOk;
}

OdResult OdDb2dPolylineJoinEntityPE::joinEntities(OdDbEntityPtr primaryEntity, const OdDbEntityPtrArray& otherEntities, OdGeIntArray& joinedEntityIndices) const
{
  joinedEntityIndices.clear();
  OdDb2dPolylinePtr pPrimary = OdDb2dPolyline::cast(primaryEntity);
  if(pPrimary.isNull())
  {
    return eInvalidInput;
  }

  OdDbPolylinePtr pPoly = OdDbPolyline::createObject();
  OdResult res = pPoly->convertFrom(pPrimary, false);
  ODA_ASSERT(res == eOk);
  OdDbJoinEntityPEPtr pJoinPE = OdDbJoinEntityPE::cast(pPoly);
  res = pJoinPE->joinEntities(pPoly, otherEntities, joinedEntityIndices);
  if(res != eOk)
  {
    return res;
  }
  res = pPoly->convertTo(pPrimary.get(), false);
  ODA_ASSERT(res == eOk);

  return eOk;
}

struct polyData
{
  polyData() : createIdentifiers(false), createBulges(false), createWidths(false) {}
  OdGePoint2dArray     m_Points;
  OdGeDoubleArray      m_Bulges;
  OdInt32Array         m_Identifiers;
  OdGePoint2dArray     m_Widths;
  OdGeVector3d normalPrimary;
  OdGePoint2d constWidth;
  bool createIdentifiers;
  bool createBulges;
  bool createWidths;
};

void joinDbPolyline(const OdDbPolylinePtr pImpl, polyData& dataThis, const CurveIndex& index)
{
  const unsigned int lenOther = pImpl->numVerts();
  OdGePoint2d ptBeg, ptEnd;
  pImpl->getPointAt(0, ptBeg);
  pImpl->getPointAt(lenOther - 1, ptEnd);
  ODA_ASSERT(dataThis.m_Points.isEmpty() || dataThis.m_Points.last().isEqualTo(index.m_bReversed ? ptEnd : ptBeg));
  const unsigned int lenOld = dataThis.m_Points.logicalLength();
  const unsigned int lenNew = lenOld == 0 ? lenOther : lenOld + lenOther - 1;
  dataThis.m_Points.setLogicalLength(lenNew);
  dataThis.m_Bulges.setLogicalLength(lenNew);
  dataThis.m_Identifiers.setLogicalLength(lenNew);
  dataThis.m_Widths.setLogicalLength(lenNew);
  if(pImpl->hasBulges())
  {
    dataThis.createBulges = true;
  }
  if(pImpl->hasVertexIdentifiers())
  {
    dataThis.createIdentifiers = true;
  }
  if(pImpl->hasWidth())
  {
    dataThis.createWidths = true;
  }

  double bulgeCoef = 1.;
  if((index.m_bReversed && pImpl->normal().isCodirectionalTo(dataThis.normalPrimary)) || (!index.m_bReversed && !pImpl->normal().isCodirectionalTo(dataThis.normalPrimary)))
  {
    bulgeCoef = -1.;
  }
  for(unsigned int i = 0; i < lenOther; ++i)
  {
    const int iTo = lenOld == 0 ? i : lenOld - 1 + i;
    const int iFrom = index.m_bReversed ? lenOther - 1 - i : i;
    OdGePoint2d ptFrom;
    pImpl->getPointAt(iFrom, ptFrom);
    dataThis.m_Points[iTo] = ptFrom;

    const double bulge = pImpl->getBulgeAt(iFrom)*bulgeCoef;
    if(!index.m_bReversed)
    {
      dataThis.m_Bulges[iTo] = bulge;
    }
    else
    {
      dataThis.m_Bulges[iTo] = 0.;
      if(i > 0)
      {
        dataThis.m_Bulges[iTo - 1] = bulge;
      }
    }

    dataThis.m_Identifiers[iTo] = pImpl->getVertexIdentifierAt(iFrom);

    OdGePoint2d width;
    pImpl->getWidthsAt(iFrom, width.x, width.y);
    if(!index.m_bReversed)
    {
      dataThis.m_Widths[iTo] = width;
    }
    else
    {
      std::swap(width.x, width.y);
      dataThis.m_Widths[iTo] = OdGePoint2d(pImpl->getConstantWidth(), pImpl->getConstantWidth());
      if(i > 0)
      {
        dataThis.m_Widths[iTo - 1] = width;
      }
    }   
  }
  dataThis.m_Bulges.last() = 0.;
}

void joinLine(const OdDbLinePtr pLine, polyData& dataThis, const CurveIndex& index)
{
  OdGePoint3d ptEnd[2];
  pLine->getStartPoint(ptEnd[index.m_bReversed ? 1 : 0]);
  pLine->getEndPoint(ptEnd[index.m_bReversed ? 0 : 1]);
  const OdGeMatrix3d w2o(OdGeMatrix3d::worldToPlane(dataThis.normalPrimary));
  ODA_ASSERT(dataThis.m_Points.isEmpty() || dataThis.m_Points.last().isEqualTo(ptEnd[0].transformBy(w2o).convert2d()));

  for(unsigned int i = dataThis.m_Points.isEmpty() ? 0 : 1; i < 2; ++i)
  {
    const OdGePoint3d ptProj = ptEnd[i].transformBy(w2o);
    dataThis.m_Points.push_back(ptProj.convert2d());
    dataThis.m_Bulges.push_back(0.);
    dataThis.m_Identifiers.push_back(0);
    dataThis.m_Widths.push_back(dataThis.constWidth);
  }
}

void joinArc(const OdDbArcPtr pArc, polyData& dataThis, const CurveIndex& index)
{
  OdGePoint3d ptEnd[2];
  pArc->getStartPoint(ptEnd[index.m_bReversed ? 1 : 0]);
  pArc->getEndPoint(ptEnd[index.m_bReversed ? 0 : 1]);
  const OdGeMatrix3d w2o(OdGeMatrix3d::worldToPlane(dataThis.normalPrimary));
  ODA_ASSERT(dataThis.m_Points.isEmpty() || dataThis.m_Points.last().isEqualTo(ptEnd[0].transformBy(w2o).convert2d()));

  double bulge = tan((pArc->endAngle() - pArc->startAngle())*0.25);
  if((index.m_bReversed && pArc->normal().isCodirectionalTo(dataThis.normalPrimary)) || (!index.m_bReversed && !pArc->normal().isCodirectionalTo(dataThis.normalPrimary)))
  {
    bulge *= -1.;
  }
  if(!dataThis.m_Points.isEmpty())
  {
    dataThis.m_Bulges.last() = bulge;
  }

  for(unsigned int i = dataThis.m_Points.isEmpty() ? 0 : 1; i < 2; ++i)
  {
    const OdGePoint3d ptProj = ptEnd[i].transformBy(w2o);
    dataThis.m_Points.push_back(ptProj.convert2d());
    dataThis.m_Bulges.push_back(i == 0 ? bulge : 0.);
    dataThis.m_Identifiers.push_back(0);
    dataThis.m_Widths.push_back(dataThis.constWidth);
  }
  dataThis.createBulges = true;
}

void joinPrimary(const OdDbPolylinePtr pImplPrimary, polyData& dataThis)
{
  OdGePoint2d ptBeg;
  pImplPrimary->getPointAt(0, ptBeg);
  ODA_ASSERT(dataThis.m_Points.isEmpty() || dataThis.m_Points.last().isEqualTo(ptBeg));
  int nPt = dataThis.m_Points.logicalLength();
  if(nPt > 0)
  {
    dataThis.m_Points.setLogicalLength(--nPt); // replace last point
  }
  const unsigned int nAppend = pImplPrimary->numVerts();
  dataThis.m_Points.setLogicalLength(nPt + nAppend);
  dataThis.m_Bulges.setLogicalLength(nPt + nAppend);
  dataThis.m_Identifiers.setLogicalLength(nPt + nAppend);
  dataThis.m_Widths.setLogicalLength(nPt + nAppend);
  for(unsigned int i = 0; i < nAppend; ++i)
  {
    OdGePoint2d pt;
    pImplPrimary->getPointAt(i, pt);
    dataThis.m_Points[nPt + i] = pt;
    dataThis.m_Bulges[nPt + i] = pImplPrimary->getBulgeAt(i);
    OdGePoint2d wt;
    pImplPrimary->getWidthsAt(i, wt.x, wt.y);
    dataThis.m_Widths[nPt + i] = wt;
    dataThis.m_Identifiers[nPt + i] = pImplPrimary->getVertexIdentifierAt(i);
  }
  dataThis.m_Bulges.last() = 0.;
}

OdResult OdDbPolylineJoinEntityPE::joinEntities(OdDbEntityPtr primaryEntity, const OdDbEntityPtrArray& otherEntities, OdGeIntArray& joinedEntityIndices) const
{
  joinedEntityIndices.clear();
  OdDbPolylinePtr pPrimary = OdDbPolyline::cast(primaryEntity);
  if(pPrimary.isNull() || pPrimary->numVerts() < 2)
  {
    return eInvalidInput;
  }

  std::list<joinEntityPE::CurveIndex> listIndex;
  const OdRxClass* aObjDesc[] = {OdDbLine::desc(), OdDbArc::desc(), OdDbPolyline::desc(), OdDb2dPolyline::desc()};
  if(!joinEntityPE::findCurveSequence(listIndex, pPrimary, otherEntities, aObjDesc, 4, true))
  {
    return eInvalidInput;
  }

  joinEntityPE::polyData dataThis;
  dataThis.normalPrimary = pPrimary->normal();
  dataThis.constWidth.set(pPrimary->getConstantWidth(), pPrimary->getConstantWidth());

  // form the extended poly
  std::list<joinEntityPE::CurveIndex>::iterator index;
  for(index = listIndex.begin(); index != listIndex.end(); ++index)
  {
    if(index->m_index < 0) // place source curve
    {
      joinEntityPE::joinPrimary(pPrimary, dataThis);
      continue;
    }

    if(otherEntities[index->m_index]->isA() == OdDbLine::desc())
    {
      OdDbLinePtr pLine = otherEntities[index->m_index];
      joinEntityPE::joinLine(pLine, dataThis, *index);
    }
    else if(otherEntities[index->m_index]->isA() == OdDbArc::desc())
    {
      OdDbArcPtr pArc = otherEntities[index->m_index];
      joinEntityPE::joinArc(pArc, dataThis, *index);
    }
    else if(otherEntities[index->m_index]->isA() == OdDbPolyline::desc())
    {
      OdDbPolylinePtr pPoly = otherEntities[index->m_index];
      joinEntityPE::joinDbPolyline(pPoly, dataThis, *index);
    }
    else if(otherEntities[index->m_index]->isA() == OdDb2dPolyline::desc())
    {
      OdDb2dPolylinePtr pPoly2d = otherEntities[index->m_index];
      OdDbPolylinePtr pPoly = OdDbPolyline::createObject();
      OdResult res = pPoly->convertFrom(pPoly2d, false);
      if(res != eOk)
      {
        return eInvalidInput;
      }
      joinEntityPE::joinDbPolyline(pPoly, dataThis, *index);
    }
    joinedEntityIndices.push_back(index->m_index);
  }

  // nothing found to join
  if(joinedEntityIndices.isEmpty())
  {
    return eInvalidInput;
  }

  // set new data
  const bool hasIdentifiers = pPrimary->hasVertexIdentifiers();
  const bool hasBulges = pPrimary->hasBulges();
  const bool hasWidths = pPrimary->hasWidth();
  const int nPtNew = dataThis.m_Points.logicalLength();
  pPrimary->reset(true, nPtNew);
  for(int i = 0; i < nPtNew; ++i)
  {
    pPrimary->setPointAt(i, dataThis.m_Points[i]);
    if(hasBulges || dataThis.createBulges)
    {
      pPrimary->setBulgeAt(i, dataThis.m_Bulges[i]);
    }
    if(hasIdentifiers || dataThis.createIdentifiers)
    {
      pPrimary->setVertexIdentifierAt(i, dataThis.m_Identifiers[i]);
    }
    if(hasWidths || dataThis.createWidths)
    {
      pPrimary->setWidthsAt(i, dataThis.m_Widths[i].x, dataThis.m_Widths[i].y);
    }
  }

  return eOk;
}

OdResult OdDbSplineJoinEntityPE::joinEntities(OdDbEntityPtr primaryEntity, const OdDbEntityPtrArray& otherEntities, OdGeIntArray& joinedEntityIndices) const
{
  joinedEntityIndices.clear();
  OdDbSplinePtr pPrimary = OdDbSpline::cast(primaryEntity);
  if(pPrimary.isNull() || pPrimary->numControlPoints() == 0)
  {
    return eInvalidInput;
  }

  std::list<joinEntityPE::CurveIndex> listIndex;
  const OdRxClass* aObjDesc[] = {OdDbLine::desc(), OdDbArc::desc(), OdDbEllipse::desc(), OdDbPolyline::desc(), OdDb2dPolyline::desc(), OdDb3dPolyline::desc(), OdDbSpline::desc()};
  if(!joinEntityPE::findCurveSequence(listIndex, pPrimary, otherEntities, aObjDesc, 7, false))
  {
    return eInvalidInput;
  }

  OdGeNurbCurve3d* pGePrimary = NULL;
  bool res = getNurbs(pPrimary, pGePrimary);
  ODA_ASSERT(res);
  // form the extended spline
  OdGeNurbCurve3d result;
  std::list<joinEntityPE::CurveIndex>::iterator index;
  for(index = listIndex.begin(); index != listIndex.end(); ++index)
  {
    if(index->m_index < 0) // place source curve
    {
      if(index == listIndex.begin())
      {
        result = *pGePrimary;
      }
      else
      {
        try
        {
          result.joinWith(*pGePrimary);
        }
        catch (...)
        {
          delete pGePrimary;
          continue;
        }
      }
      delete pGePrimary;
      continue;
    }

    OdDbCurvePtr curve = otherEntities[index->m_index];
    OdGeNurbCurve3d* nurbs = NULL;
    res = getNurbs(curve, nurbs);
    ODA_ASSERT(res);
    if(index->m_bReversed)
    {
      nurbs->reverseParam();
    }
    if(index == listIndex.begin())
    {
      result = *nurbs;
    }
    else
    {
      try
      {
        result.joinWith(*nurbs);
      }
      catch (...)
      {
        delete nurbs;
        continue;
      }
    }
    delete nurbs;

    joinedEntityIndices.push_back(index->m_index);
  }

  // nothing found to join
  if(joinedEntityIndices.isEmpty())
  {
    return eInvalidInput;
  }

  // set new data
  pPrimary->setFromOdGeCurve(result);

  return eOk;
}
} // namespace
