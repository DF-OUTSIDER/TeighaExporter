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
#include "BrepBuilderFillerModule.h"
#include "BrepBuilderInitialData.h"
#include "IMaterialAndColorHelper.h"

#include "Br/BrBrep.h"
#include "Br/BrComplex.h"
#include "Br/BrShell.h"
#include "Br/BrFace.h"
#include "Br/BrLoop.h"
#include "Br/BrEdge.h"
#include "Br/BrBrepComplexTraverser.h"
#include "Br/BrComplexShellTraverser.h"
#include "Br/BrShellFaceTraverser.h"
#include "Br/BrFaceLoopTraverser.h"
#include "Br/BrLoopEdgeTraverser.h"
#include "Br/BrLoopVertexTraverser.h"
#include "Br/BrBrepFaceTraverser.h"

#include "BrepBuilder/BrepBuilder.h"

#include "Ge/GePoint3d.h"
#include "Ge/GePoint2d.h"
#include "Ge/GeExternalCurve3d.h"
#include "Ge/GeExternalSurface.h"
#include "Ge/GeExternalBoundedSurface.h"
#include "Ge/GeLineSeg3d.h"
#include "Ge/GeEllipArc3d.h"
#include "Ge/GeCurve2d.h"
#include "Ge/GeGbl.h"

#define STL_USING_SET
#include "OdaSTL.h"

//
#include "TPtr.h"
typedef TPtr< OdGeCurve3d, TObjDelete<OdGeCurve3d> > OdGeTempCurve3dPtr;
typedef TPtr< OdGeCurve2d, TObjDelete<OdGeCurve2d> > OdGeTempCurve2dPtr;
typedef TPtr< OdGeNurbCurve3d, TObjDelete<OdGeNurbCurve3d> > OdGeTempNurbCurve3dPtr;
typedef TPtr< OdGeNurbCurve2d, TObjDelete<OdGeNurbCurve2d> > OdGeTempNurbCurve2dPtr;
typedef TPtr< OdGeSurface, TObjDelete<OdGeSurface> > OdGeTempSurfacePtr;


//
ODRX_DEFINE_DYNAMIC_MODULE(OdBrepBuilderFillerModule);

void OdBrepBuilderFillerModule::initApp()
{
}

void OdBrepBuilderFillerModule::uninitApp()
{
}

// Helper functions
class OdBrepBuilderFillerHelper
{
  BrepBuilderInitialData& m_initialData;

  OdIMaterialAndColorHelper* m_pMaterialHelper;
  OdBrepBuilderFillerParams m_params;

  OdArray<OdBrEdge> m_edges;
  const BrepBuilderInitialCoedge::EdgeIndex m_zeroBase;

  double m_toleranceInterval;
  double m_toleranceDiffPoints;
  double m_toleranceRestore2dCurve;
  double m_toleranceCoincide;

  mutable std::set<const OdGeCurve3d*> m_edgeCurveChanged;// true if edge curve was fixed => required to update coedge curve

public:
  OdBrepBuilderFillerHelper(
    BrepBuilderInitialData& initalData,
    OdIMaterialAndColorHelper* pMaterialHelper = NULL,
    const OdBrepBuilderFillerParams& params = OdBrepBuilderFillerParams()
  )
    : m_pMaterialHelper(pMaterialHelper)
    , m_params(params)
    , m_initialData(initalData)
    , m_zeroBase(initalData.edges.size())
    , m_toleranceInterval(1e-9)// double d2dParamTol = 1e-9;//TODO: possible investigation needed
    , m_toleranceDiffPoints(1e-3)
    , m_toleranceRestore2dCurve(1e-6)
    , m_toleranceCoincide(1e-6)
  {
  }

  // Interval
  /** \details
  Test that scndInt interval is part of frstInt.
  */
  bool isIntervalContain(const OdGeInterval& frstInt, const OdGeInterval& scndInt) const;
  /** \details
  Test that scndInt interval is part of reversed frstInt.
  */
  bool isIntervalContainReversed(const OdGeInterval& frstInt, const OdGeInterval& scndInt) const;

  // Curve
  template <class NURBS>
  static void resetNurbInterval(NURBS& curve);

  // Edge
  OdGeCurve3dPtr getEdgeCurve(const OdBrEdge& edge) const;
  bool fixEllipse(OdGeCurve3dPtr& pCurve, const OdBrEdge& edge) const;
  bool fixNurb(OdGeCurve3dPtr& pCurve, const OdBrEdge& edge) const;
  bool getEdgeCurveFixed(const OdBrEdge& edge, OdGeCurve3dPtr& pCurve) const;

  // Coedge
  OdGeCurve2dPtr getParamCurve(const OdBrLoopEdgeTraverser& loEdTrav) const;
  bool checkNurb2dFor3d(const OdGeSurfacePtr& pSurf, const OdGeCurve3dPtr& pCurve3d, OdGeCurve2dPtr& pCurve2d) const;
  OdResult checkCurve2dFor3d(const OdGeSurfacePtr& pSurf, const OdGeCurve3dPtr& pCurve3d, OdGeCurve2dPtr& pCurve2d) const;
  void moveParamCurveNurbInterval(const OdGeSurfacePtr& pSurf, const OdGeCurve3dPtr& pCurve3d, OdGeCurve2dPtr& pCurve2d) const;
  OdResult moveParamCurveInterval(const OdGeSurfacePtr& pSurf, const OdGeCurve3dPtr& pCurve3d, OdGeCurve2dPtr& pCurve2d) const;
  //this routine compare a 2d curve direction with 3d curve direction
  bool isDir2dCoincide3d(const OdGeSurfacePtr& pSurf, const OdGeCurve3dPtr& pCurve, const OdGeCurve2dPtr& pParcur) const;
  OdResult fixParamCurve(const OdGeSurfacePtr& pSurf, const OdGeCurve3dPtr& pCurve, OdGeCurve2dPtr& pParamCurve) const;

  // Face
  OdGeSurfacePtr checkExtSurface(const OdGeSurface* resSurf, const OdBrFace& face) const;
  OdGeSurfacePtr getFaceSurface(const OdBrFace& face) const;

  // Loop
  OdResult performLoopWithApex(const OdBrLoop& loop, OdGeCurve3dPtr& curve, OdGeCurve2dPtr& paramCurve) const;

  // Toplogy
  OdResult run(const OdBrBrep& brep);
  OdResult performComplex(const OdBrComplex& complex);
  OdResult performShell(const OdBrShell& shell, BrepBuilderShellsArray& arrShells);
  OdResult performFace(const OdBrFace& face, BrepBuilderInitialSurfaceArray &arrSurfaces);
  OdResult performLoop(const OdBrLoop& loop,
    BrepBuilderInitialSurface& surfData);
};

// Interval
bool OdBrepBuilderFillerHelper::isIntervalContain(const OdGeInterval& frstInt, const OdGeInterval& scndInt) const
{
  return OdLessOrEqual(frstInt.lowerBound(), scndInt.lowerBound(), m_toleranceInterval)
    && OdGreaterOrEqual(frstInt.upperBound(), scndInt.upperBound(), m_toleranceInterval);
}
bool OdBrepBuilderFillerHelper::isIntervalContainReversed(const OdGeInterval& frstInt, const OdGeInterval& scndInt) const
{
  return OdGreaterOrEqual(frstInt.lowerBound()*-1, scndInt.upperBound(), m_toleranceInterval)
    && OdLessOrEqual(frstInt.upperBound()*-1, scndInt.lowerBound(), m_toleranceInterval);
}

// Curve
template <class NURBS>
void OdBrepBuilderFillerHelper::resetNurbInterval(NURBS& curve)
{
  OdGeInterval originInt;
  curve.getInterval(originInt);
  double knot0 = curve.knotAt(0);
  double knot1 = curve.knotAt(curve.numKnots() - 1);
  if (!OdEqual(originInt.lowerBound(), knot0) || !OdEqual(originInt.upperBound(), knot1))
  {
    curve.setInterval(OdGeInterval(knot0, knot1));
  }
}

// Edge
OdGeCurve3dPtr OdBrepBuilderFillerHelper::getEdgeCurve(const OdBrEdge& edge) const
{
  OdGeCurve3dPtr curve3d(edge.getCurve());

  if (!curve3d)
  {
    OdGeNurbCurve3d nurbCurve3d;
    if (edge.getCurveAsNurb(nurbCurve3d))
    {
      return static_cast<OdGeCurve3d*>(nurbCurve3d.copy());
    }
    return NULL;
  }

  OdGe::EntityId entType = curve3d->type();
  if (OdGe::kExternalCurve3d == entType)
  {
    OdGeCurve3d* resCurve3d = NULL;
    const OdGeExternalCurve3d* extCurve = static_cast<OdGeExternalCurve3d*>(curve3d.get());

    if (extCurve->isNativeCurve(resCurve3d))
    {
      OdGeCurve3dPtr pCurveStore(resCurve3d);

      OdGeInterval curvIntOrig, curvIntNew;
      curve3d->getInterval(curvIntOrig);
      pCurveStore->getInterval(curvIntNew);

      if (isIntervalContain(curvIntOrig, curvIntNew)
        || isIntervalContainReversed(curvIntOrig, curvIntNew)
        || m_params.isSkipCoedge2dCurve())
      {
        curve3d = pCurveStore;
      }
    }
  }

  return curve3d;
}

bool OdBrepBuilderFillerHelper::fixEllipse(OdGeCurve3dPtr& pCurve, const OdBrEdge& edge) const
{
  OdGeEllipArc3d& geEllipse = *(OdGeEllipArc3d*)pCurve.get();
  OdBrVertex startVertex;
  OdBrVertex endVertex;
  if (!edge.getVertex1(startVertex) || !edge.getVertex2(endVertex))
  {
    // parasolid variant
    return true;
  }

  OdGePoint3d startPoint;
  OdGePoint3d endPoint;
  if (edge.getOrientToCurve())
  {
    startPoint = startVertex.getPoint();
    endPoint = endVertex.getPoint();
  }
  else
  {
    endPoint = startVertex.getPoint();
    startPoint = endVertex.getPoint();
  }

  OdGePoint3d curveStart;
  OdGePoint3d curveEnd;
  if (!geEllipse.hasStartPoint(curveStart)
    || !geEllipse.hasEndPoint(curveEnd))
  {
    return false;
  }

  if (startPoint.isEqualTo(curveStart, m_toleranceDiffPoints))
  {
    return true;
  }

  OdGeInterval interval;
  geEllipse.getInterval(interval);

  if (!geEllipse.isCircular() || !startPoint.isEqualTo(endPoint, m_toleranceDiffPoints))
  {
    double newStartParam = geEllipse.paramOf(startPoint);
    geEllipse.setInterval(OdGeInterval(newStartParam, newStartParam + interval.length()));
    pCurve = new OdGeNurbCurve3d(geEllipse);

    m_edgeCurveChanged.insert(pCurve);

    ODA_ASSERT_ONCE(((OdGeNurbCurve3d*)pCurve.get())->startPoint().isEqualTo(startPoint, 1e-04));

    return true;
  }

  // closed circle
  OdGePoint3d center = geEllipse.center();
  OdGeVector3d major = geEllipse.majorAxis();
  OdGeVector3d normal = geEllipse.normal();

  ODA_ASSERT_ONCE(OdEqual(startPoint.distanceTo(center), geEllipse.majorRadius(), 1e-05));

  OdGeVector3d centerToStart = startPoint - center;
  if (major.isCodirectionalTo(centerToStart))
  {
    if (OdNegative(interval.lowerBound()))
    {
      geEllipse.setInterval(OdGeInterval(0., interval.length()));

      m_edgeCurveChanged.insert(pCurve);
    }
  }
  else
  {
    double angle = major.angleTo(centerToStart, normal);

    if (!OdZero(angle))
    {
      geEllipse.rotateBy(angle, normal, center);
      if (OdNegative(interval.lowerBound()))
      {
        geEllipse.setInterval(OdGeInterval(0., interval.length()));

        m_edgeCurveChanged.insert(pCurve);
      }
    }
  }

  ODA_ASSERT_ONCE(geEllipse.hasStartPoint(curveStart));
  ODA_ASSERT_ONCE(startPoint.isEqualTo(curveStart, 1e-05));

  return true;
}

bool OdBrepBuilderFillerHelper::fixNurb(OdGeCurve3dPtr& pCurve, const OdBrEdge& edge) const
{
  OdGeNurbCurve3d& geNurb = *(OdGeNurbCurve3d*)pCurve.get();
  OdBrVertex startVertex;
  OdBrVertex endVertex;
  if (!edge.getVertex1(startVertex) || !edge.getVertex2(endVertex))
  {
    // parasolid variant
    return true;
  }

  OdGePoint3d startPoint;
  OdGePoint3d endPoint;
  if (edge.getOrientToCurve())
  {
    startPoint = startVertex.getPoint();
    endPoint = endVertex.getPoint();
  }
  else
  {
    endPoint = startVertex.getPoint();
    startPoint = endVertex.getPoint();
  }

  OdGePoint3d curveStart = geNurb.startPoint();
  OdGePoint3d curveEnd = geNurb.endPoint();
  if (startPoint.isEqualTo(curveStart, m_toleranceDiffPoints)
    && endPoint.isEqualTo(curveEnd, m_toleranceDiffPoints))
  {
    return true;
  }

  if (startPoint.isEqualTo(endPoint, m_toleranceDiffPoints))
  {
    // Closed curve
    // isOn not implemented
    double splitParam = geNurb.paramOf(startPoint);
    OdGeInterval curve3dInt;
    geNurb.getInterval(curve3dInt);
    if (OdEqual(splitParam, curve3dInt.lowerBound(), m_toleranceDiffPoints)
      || OdEqual(splitParam, curve3dInt.upperBound(), m_toleranceDiffPoints))
    {
      // it seems that point is not on curve
      return true;
    }

    OdGeCurve3d* piece1 = NULL;
    OdGeCurve3d* piece2 = NULL;
    geNurb.getSplitCurves(splitParam, piece1, piece2);
    OdGeTempCurve3dPtr pPiece1(piece1);
    OdGeTempCurve3dPtr pPiece2(piece2);
    if (!pPiece1.isNull() && !pPiece2.isNull())
    {
      ODA_ASSERT_ONCE((OdGe::kNurbCurve3d == pPiece1->type()));
      ODA_ASSERT_ONCE((OdGe::kNurbCurve3d == pPiece2->type()));
      OdGeNurbCurve3d& geNurb1 = (OdGeNurbCurve3d&)*pPiece1;
      OdGeNurbCurve3d& geNurb2 = (OdGeNurbCurve3d&)*pPiece2;
      geNurb = geNurb2.joinWith(geNurb1);
      resetNurbInterval<OdGeNurbCurve3d>(geNurb);

      m_edgeCurveChanged.insert(pCurve);

      ODA_ASSERT_ONCE(geNurb.startPoint().isEqualTo(startPoint, 1e-02) && geNurb.endPoint().isEqualTo(endPoint, 1e-02));
    }
  }
  else
  {
    double startParamNew = geNurb.paramOf(startPoint);
    double endParamNew = geNurb.paramOf(endPoint);
    OdGeInterval curve3dInt;
    geNurb.getInterval(curve3dInt);
    if ((OdEqual(startParamNew, curve3dInt.lowerBound(), m_toleranceDiffPoints)
      && OdEqual(endParamNew, curve3dInt.upperBound(), m_toleranceDiffPoints)))
    {
      // it seems that point is not on curve
      return true;
    }

    double startParam = geNurb.startParam();
    double endParam = geNurb.endParam();
    if ((startParam < startParamNew && startParamNew < endParam) && (startParam < endParamNew && endParamNew < endParam))
    {
      if ((startParamNew > endParamNew) && geNurb.isClosed())
      {
        // remove middle of curve and merge second part with first
        OdGeTempNurbCurve3dPtr pNurb1((OdGeNurbCurve3d*)geNurb.copy());
        OdGeTempNurbCurve3dPtr pNurb2((OdGeNurbCurve3d*)geNurb.copy());
        if (!pNurb1.isNull() && !pNurb2.isNull())
        {
          pNurb1->hardTrimByParams(startParamNew, endParam);
          pNurb2->hardTrimByParams(startParam, endParamNew);
          geNurb = pNurb1->joinWith(*pNurb2);
          resetNurbInterval<OdGeNurbCurve3d>(geNurb);

          m_edgeCurveChanged.insert(pCurve);

          ODA_ASSERT_ONCE(geNurb.startPoint().isEqualTo(startPoint, 1e-02) && geNurb.endPoint().isEqualTo(endPoint, 1e-02));
        }
      }
      else
      {
        // trim curve
        ODA_ASSERT_ONCE(startParamNew < endParamNew);
        geNurb.hardTrimByParams(startParamNew, endParamNew);
        resetNurbInterval<OdGeNurbCurve3d>(geNurb);

        m_edgeCurveChanged.insert(pCurve);

        ODA_ASSERT_ONCE(geNurb.startPoint().isEqualTo(startPoint, 1e-02) && geNurb.endPoint().isEqualTo(endPoint, 1e-02));
      }
    }
  }

  return true;
}

bool OdBrepBuilderFillerHelper::getEdgeCurveFixed(const OdBrEdge& edge, OdGeCurve3dPtr& pCurve) const
{
  pCurve = getEdgeCurve(edge);

  if (pCurve.isNull())
  {
    return false;
  }

  OdGe::EntityId curveType = pCurve->type();
  if (OdGe::kEllipArc3d == curveType)
  {
    return fixEllipse(pCurve, edge);
  }
  else if (OdGe::kNurbCurve3d == curveType)
  {
    return fixNurb(pCurve, edge);
  }

  return true;
}

// Coedge
OdGeCurve2dPtr OdBrepBuilderFillerHelper::getParamCurve(const OdBrLoopEdgeTraverser& loEdTrav) const
{
  OdGeNurbCurve2d nurbCurve2d;
  if (odbrOK == loEdTrav.getParamCurveAsNurb(nurbCurve2d))
  {
    return static_cast<OdGeCurve2d*>(nurbCurve2d.copy());
  }

  OdGeTempCurve2dPtr paramCurve(loEdTrav.getParamCurve());
  if (paramCurve.isNull()) return NULL;

  // Acis bb required nurb 2d curve. Another (bim,prc) bb doesn't use 2d curve.
  if (OdGe::kNurbCurve2d == paramCurve->type()) return paramCurve.detach();
  return OdGeNurbCurve2d::convertFrom(paramCurve.get(), m_toleranceRestore2dCurve, true);
}

bool OdBrepBuilderFillerHelper::checkNurb2dFor3d(const OdGeSurfacePtr& pSurf, const OdGeCurve3dPtr& pCurve3d, OdGeCurve2dPtr& pCurve2d) const
{
  OdGeNurbCurve2d& geNurb2d = *(OdGeNurbCurve2d*)pCurve2d.get();

  OdGePoint3d curve3dStart;
  OdGePoint3d curve3dEnd;
  OdGePoint2d curve2dStart = geNurb2d.startPoint();
  OdGePoint2d curve2dEnd = geNurb2d.endPoint();

  if (!pCurve3d->hasStartPoint(curve3dStart)
    || !pCurve3d->hasEndPoint(curve3dEnd))
  {
    return false;
  }

  OdGePoint3d surf3dStart = pSurf->evalPoint(curve2dStart);
  OdGePoint3d surf3dEnd = pSurf->evalPoint(curve2dEnd);

  if ((surf3dStart.isEqualTo(curve3dStart, m_toleranceDiffPoints)
    && surf3dEnd.isEqualTo(curve3dEnd, m_toleranceDiffPoints))
    || (surf3dStart.isEqualTo(curve3dEnd, m_toleranceDiffPoints)
    && surf3dEnd.isEqualTo(curve3dStart, m_toleranceDiffPoints)))
  {
    return true;
  }

  // required to fix curve
  OdGePoint2d start2dPoint;
  OdGePoint2d end2dPoint;
  OdGe::EntityId surfType = pSurf->type();
  if (OdGe::kNurbSurface == surfType)
  {
    OdGeNurbSurface* pNurbSurf = (OdGeNurbSurface*)pSurf.get();
    start2dPoint = pNurbSurf->paramOfPrec(curve3dStart);
    end2dPoint = pNurbSurf->paramOfPrec(curve3dEnd);
  }
  else
  {
    start2dPoint = pSurf->paramOf(curve3dStart);
    end2dPoint = pSurf->paramOf(curve3dEnd);
  }

  if (!curve3dStart.isEqualTo(pSurf->evalPoint(start2dPoint), m_toleranceDiffPoints)
    || !curve3dEnd.isEqualTo(pSurf->evalPoint(end2dPoint), m_toleranceDiffPoints))
  {
    return false;
  }

  if (start2dPoint.isEqualTo(end2dPoint, m_toleranceDiffPoints))
  {
    if (start2dPoint.isEqualTo(curve2dStart, m_toleranceDiffPoints)
      || start2dPoint.isEqualTo(curve2dEnd, m_toleranceDiffPoints))
    {
      return false;
    }

    double splitParam;
    if (!geNurb2d.isOn(start2dPoint, splitParam, m_toleranceDiffPoints))
    {
      return false;
    }

    OdGeCurve2d* piece1 = NULL;
    OdGeCurve2d* piece2 = NULL;
    geNurb2d.getSplitCurves(splitParam, piece1, piece2);
    OdGeTempCurve2dPtr pPiece1(piece1);
    OdGeTempCurve2dPtr pPiece2(piece2);
    if (!pPiece1.isNull() && !pPiece2.isNull())
    {
      ODA_ASSERT_ONCE((OdGe::kNurbCurve2d == pPiece1->type()));
      ODA_ASSERT_ONCE((OdGe::kNurbCurve2d == pPiece2->type()));
      OdGeNurbCurve2d& geNurb1 = (OdGeNurbCurve2d&)*pPiece1;
      OdGeNurbCurve2d& geNurb2 = (OdGeNurbCurve2d&)*pPiece2;
      geNurb2.transformBy(OdGeMatrix2d::translation(geNurb1.startPoint() - geNurb2.endPoint()));
      geNurb2d = geNurb2.joinWith(geNurb1);
      resetNurbInterval<OdGeNurbCurve2d>(geNurb2d);

      return (pSurf->evalPoint(geNurb2d.startPoint()).isEqualTo(curve3dStart, m_toleranceDiffPoints)
        && pSurf->evalPoint(geNurb2d.endPoint()).isEqualTo(curve3dEnd, m_toleranceDiffPoints));
    }
  }
  else
  {
    double startParamNew;
    double endParamNew;
    if (!geNurb2d.isOn(start2dPoint, startParamNew, m_toleranceDiffPoints)
      || !geNurb2d.isOn(end2dPoint, endParamNew, m_toleranceDiffPoints))
    {
      return false;
    }

    double startParam = geNurb2d.startParam();
    double endParam = geNurb2d.endParam();
    if ((startParam < startParamNew && startParamNew < endParam) && (startParam < endParamNew && endParamNew < endParam))
    {
      if ((startParamNew > endParamNew) && geNurb2d.isClosed())
      {
        OdGeTempNurbCurve2dPtr pNurb1((OdGeNurbCurve2d*)geNurb2d.copy());
        OdGeTempNurbCurve2dPtr pNurb2((OdGeNurbCurve2d*)geNurb2d.copy());
        if (!pNurb1.isNull() && !pNurb2.isNull())
        {
          pNurb1->hardTrimByParams(startParamNew, endParam);
          pNurb2->hardTrimByParams(startParam, endParamNew);
          geNurb2d = pNurb1->joinWith(*pNurb2);
          resetNurbInterval<OdGeNurbCurve2d>(geNurb2d);

          return (pSurf->evalPoint(geNurb2d.startPoint()).isEqualTo(curve3dStart, m_toleranceDiffPoints)
            && pSurf->evalPoint(geNurb2d.endPoint()).isEqualTo(curve3dEnd, m_toleranceDiffPoints));
        }
      }
      else
      {
        ODA_ASSERT_ONCE(startParamNew < endParamNew);
        geNurb2d.hardTrimByParams(startParamNew, endParamNew);
        resetNurbInterval<OdGeNurbCurve2d>(geNurb2d);

        return (pSurf->evalPoint(geNurb2d.startPoint()).isEqualTo(curve3dStart, m_toleranceDiffPoints)
          && pSurf->evalPoint(geNurb2d.endPoint()).isEqualTo(curve3dEnd, m_toleranceDiffPoints));
      }
    }
  }

  return false;
}

OdResult OdBrepBuilderFillerHelper::checkCurve2dFor3d(const OdGeSurfacePtr& pSurf, const OdGeCurve3dPtr& pCurve3d, OdGeCurve2dPtr& pCurve2d) const
{
  if (m_edgeCurveChanged.end() == m_edgeCurveChanged.find(pCurve3d))
  {
    return eOk;
  }

  OdGe::EntityId curve2dType = pCurve2d->type();
  if (OdGe::kNurbCurve2d == curve2dType)
  {
    return checkNurb2dFor3d(pSurf, pCurve3d, pCurve2d) ? eOk : eInvalidInput;
  }

  // TODO not nurb curve
  return eNotImplemented;
}

void OdBrepBuilderFillerHelper::moveParamCurveNurbInterval(const OdGeSurfacePtr& pSurf, const OdGeCurve3dPtr& pCurve3d, OdGeCurve2dPtr& pCurve2d) const
{
  OdGeNurbCurve2d& geNurb2d = *(OdGeNurbCurve2d*)pCurve2d.get();

  OdGeInterval curv3dInt;
  OdGeInterval curv2dInt;
  pCurve3d->getInterval(curv3dInt);
  geNurb2d.getInterval(curv2dInt);

  ODA_ASSERT_ONCE(OdEqual(curv2dInt.lowerBound(), geNurb2d.knotAt(0)));
  ODA_ASSERT_ONCE(OdEqual(curv2dInt.upperBound(), geNurb2d.knotAt(geNurb2d.numKnots() - 1)));

  OdGeKnotVector aKt = geNurb2d.knots();
  aKt.setRange(curv3dInt.lowerBound(), curv3dInt.upperBound());
  ODA_ASSERT_ONCE(aKt.length() == geNurb2d.numKnots());

  for (int i = 0; i < aKt.length(); ++i)
  {
    geNurb2d.setKnotAt(i, aKt[i]);
  }

  resetNurbInterval<OdGeNurbCurve2d>(geNurb2d);
}

OdResult OdBrepBuilderFillerHelper::moveParamCurveInterval(const OdGeSurfacePtr& pSurf, const OdGeCurve3dPtr& pCurve3d, OdGeCurve2dPtr& pCurve2d) const
{
  OdGe::EntityId curve2dType = pCurve2d->type();
  if (OdGe::kNurbCurve2d == curve2dType)
  {
    moveParamCurveNurbInterval(pSurf, pCurve3d, pCurve2d);
    return eOk;
  }

  // TODO not nurb curve
  return eNotImplemented;
}


bool OdBrepBuilderFillerHelper::isDir2dCoincide3d(const OdGeSurfacePtr &pSurf, const OdGeCurve3dPtr &pCurve, const OdGeCurve2dPtr &pParcur) const
{
  ODA_ASSERT_ONCE(!pSurf.isNull());
  ODA_ASSERT_ONCE(!pCurve.isNull());
  ODA_ASSERT_ONCE(!pParcur.isNull());

  bool result = true;
  OdResult status = geIsDir2dOnSurfCoincide3d(pSurf, pCurve, pParcur, result, m_toleranceCoincide);
  if (eOk == status)
  {
    return result;
  }

  return true;
}

template <class NURBS, class POINT>
void nurbCurveReverseParam(NURBS* pThis)
{
  ODA_ASSERT(!pThis->hasFitData());
  if (pThis->numControlPoints() == 0)
  {
    return;
  }

  // swapping...
  int i, sz;
  sz = pThis->numWeights();
  for (i = 0; i < sz / 2; i++)
  {
    double tmp = pThis->weightAt(i);
    pThis->setWeightAt(i, pThis->weightAt(sz - i - 1));
    pThis->setWeightAt(sz - i - 1, tmp);
  }
  sz = pThis->numControlPoints();
  for (i = 0; i < sz / 2; i++)
  {
    POINT tmp = pThis->controlPointAt(i);
    pThis->setControlPointAt(i, pThis->controlPointAt(sz - i - 1));
    pThis->setControlPointAt(sz - i - 1, tmp);
  }

  sz = pThis->numKnots();
  for (i = 0; i < sz / 2; i++)
  {
    double tmp = pThis->knotAt(i);
    pThis->setKnotAt(i, pThis->knotAt(sz - i - 1));
    pThis->setKnotAt(sz - i - 1, tmp);
  }
  for (i = 0; i < sz; i++)
  {
    pThis->setKnotAt(i, pThis->knotAt(i)*-1.);
  }

  OdGeInterval interval;
  pThis->getInterval(interval);
  const bool bIntervalBounded = !OdEqual(interval.lowerBound(), pThis->knotAt(0)) || !OdEqual(interval.upperBound(), pThis->knotAt(sz - 1));
  if (bIntervalBounded)
  {
    pThis->setInterval(OdGeInterval(-interval.upperBound(), -interval.lowerBound()));
  }
}
OdResult OdBrepBuilderFillerHelper::fixParamCurve(const OdGeSurfacePtr &pSurf, const OdGeCurve3dPtr &pCurve, OdGeCurve2dPtr& pParamCurve) const
{
  if (!pParamCurve)
  {
    return eOk;
  }

  // First update 2d curve if 3d was fixed
  OdResult status = checkCurve2dFor3d(pSurf, pCurve, pParamCurve);
  if (eOk != status)
  {
    return status;
  }

  OdGeInterval curv3dInt, curv2dInt;
  pCurve->getInterval(curv3dInt);
  pParamCurve->getInterval(curv2dInt);

  if (!isIntervalContain(curv2dInt, curv3dInt)
    && !isIntervalContainReversed(curv2dInt, curv3dInt))
  {
    status = moveParamCurveInterval(pSurf, pCurve, pParamCurve);
    if (eOk != status)
    {
      return status;
    }
    pParamCurve->getInterval(curv2dInt);
  }

  if (!isDir2dCoincide3d(pSurf, pCurve, pParamCurve))
  {
    if (OdGe::kNurbCurve2d == pParamCurve->type())
    {
      nurbCurveReverseParam<OdGeNurbCurve2d, OdGePoint2d>(static_cast<OdGeNurbCurve2d*>(pParamCurve.get()));
    }
    else
    {
      return eNotImplemented;
    }
    pParamCurve->getInterval(curv2dInt);
  }

  if (isIntervalContain(curv2dInt, curv3dInt))
  {
    return eOk;
  }

  if (isIntervalContainReversed(curv2dInt, curv3dInt))
  {
    if (OdGe::kNurbCurve2d == pParamCurve->type())
    {
      nurbCurveReverseParam<OdGeNurbCurve2d, OdGePoint2d>(static_cast<OdGeNurbCurve2d*>(pParamCurve.get()));
    }
    else
    {
      return eNotImplemented;
    }
    pParamCurve->getInterval(curv2dInt);
  }
  else
  {
    return eInvalidIntervals;
  }

  if (!isDir2dCoincide3d(pSurf, pCurve, pParamCurve))
  {
    if (OdGe::kNurbCurve2d == pParamCurve->type())
    {
      nurbCurveReverseParam<OdGeNurbCurve2d, OdGePoint2d>(static_cast<OdGeNurbCurve2d*>(pParamCurve.get()));
    }
    else
    {
      return eNotImplemented;
    }
    pParamCurve->getInterval(curv2dInt);
  }

  if (isIntervalContain(curv2dInt, curv3dInt))
  {
    return eOk;
  }

  return eInvalidIntervals;
}

// Face
OdGeSurfacePtr OdBrepBuilderFillerHelper::checkExtSurface(const OdGeSurface* resSurf, const OdBrFace &face) const
{
  const OdGeExternalSurface* extSurf = static_cast<const OdGeExternalSurface*>(resSurf);
  OdGeSurface* pSurf = NULL;
  if (extSurf->isNativeSurface(pSurf))
    return pSurf;

  OdGeNurbSurface nurbFace;
  face.getSurfaceAsNurb(nurbFace);
  return new OdGeNurbSurface(nurbFace);
}

OdGeSurfacePtr OdBrepBuilderFillerHelper::getFaceSurface(const OdBrFace& face) const
{
  OdGeSurfacePtr surf(face.getSurface());
  if (!surf)
  {
    OdGeNurbSurface nurbSurf;
    if (odbrOK == face.getSurfaceAsNurb(nurbSurf))
    {
      return static_cast<OdGeSurface*>(nurbSurf.copy());
    }
    return NULL;
  }

  OdGe::EntityId entType = surf->type();
  if (OdGe::kExternalBoundedSurface == entType)
  {
    OdGeSurface* tResSurf = NULL;
    const OdGeExternalBoundedSurface* extSurf = static_cast<OdGeExternalBoundedSurface*>(surf.get());
    extSurf->getBaseSurface(tResSurf);
    OdGeTempSurfacePtr resSurf(tResSurf);
    if (!resSurf.isNull() && resSurf->type() != OdGe::kExternalSurface)
      surf = resSurf.detach();
    else if (!resSurf.isNull() && resSurf->type() == OdGe::kExternalSurface)
      surf = checkExtSurface(resSurf, face);
  }
  else if (OdGe::kExternalSurface == entType)
    surf = checkExtSurface(surf, face);

  return surf;
}

// Loop
OdResult OdBrepBuilderFillerHelper::performLoopWithApex(const OdBrLoop& loop,
  OdGeCurve3dPtr& curve, OdGeCurve2dPtr& paramCurve) const
{
  OdBrLoopVertexTraverser loopVertTrav;
  if (odbrOK != loopVertTrav.setLoop(loop))
  {
    return eInvalidInput;
  }

  OdGePoint3d point = loopVertTrav.getVertex().getPoint();

  if ((odbrOK != loopVertTrav.next()) || !loopVertTrav.done())
  {
    // unexpected: more then one point
    return eInvalidInput;
  }

  curve = new OdGeLineSeg3d(point, point);
  paramCurve = NULL;

  return eOk;
}

// Topology
OdResult OdBrepBuilderFillerHelper::run(const OdBrBrep& brep)
{
  // Checks
  if (!brep.isValid())
  {
    return eNotInitializedYet;
  }

  OdBrBrepComplexTraverser complxTrav;
  OdBrErrorStatus errStatus = complxTrav.setBrep(brep);
  if (odbrOK != errStatus)
  {
    return eInvalidInput;
  }

  while (!complxTrav.done())
  {
    OdBrComplex complex = complxTrav.getComplex();

    OdResult res = performComplex(complex);
    if (eOk != res)
    {
      return res;
    }

    if (odbrOK != complxTrav.next())
    {
      return eInvalidInput;
    }
  }

  return eOk;
}

OdResult OdBrepBuilderFillerHelper::performComplex(const OdBrComplex& complex)
{
  OdBrComplexShellTraverser complxShellTrav;
  OdBrErrorStatus errStatus = complxShellTrav.setComplex(complex);
  if (odbrUnsuitableTopology == errStatus)
  {
    return eOk;
  }
  if (odbrOK != errStatus)
  {
    return eInvalidInput;
  }

  BrepBuilderShellsArray shells;
  while (!complxShellTrav.done())
  {
    OdBrShell shell = complxShellTrav.getShell();

    OdResult res = performShell(shell, shells);
    if (eOk != res)
    {
      return res;
    }

    if (odbrOK != complxShellTrav.next())
    {
      return eInvalidInput;
    }
  }
  m_initialData.complexes.append(shells);

  return eOk;
}

OdResult OdBrepBuilderFillerHelper::performShell(const OdBrShell& shell, BrepBuilderShellsArray& arrShells)
{
  OdBrShellFaceTraverser shellFaceTrav;
  OdBrErrorStatus errStatus = shellFaceTrav.setShell(shell);
  if (odbrUnsuitableTopology == errStatus)
  {
    return eOk;
  }
  if (odbrOK != errStatus)
  {
    return eInvalidInput;
  }

  BrepBuilderInitialSurfaceArray arrSurfaces;
  while (!shellFaceTrav.done())
  {
    OdBrFace face = shellFaceTrav.getFace();
    OdResult res = performFace(face, arrSurfaces);
    if (eOk != res)
    {
      return res;
    }
    if (odbrOK != shellFaceTrav.next())
    {
      return eInvalidInput;
    }
  }
  arrShells.append(arrSurfaces);

  return eOk;
}

OdResult OdBrepBuilderFillerHelper::performFace(const OdBrFace& face, BrepBuilderInitialSurfaceArray &arrSurfaces)
{
  OdResult eStatus;
  BrepBuilderInitialSurface surfData;
  surfData.pSurf = getFaceSurface(face);
  if (surfData.pSurf.isNull())
  {
    // skip face without surface
    if (m_params.isSkipNullSurface())
    {
      return eOk;
    }
    return eInvalidInput;
  }

  // face direction
  surfData.direction = face.getOrientToSurface() ? OdBrepBuilder::kForward : OdBrepBuilder::kReversed;

  // face visual
  eStatus = surfData.setupVisualInfo(face, m_pMaterialHelper);
  if (eOk != eStatus)
  {
    return eStatus;
  }

  OdBrFaceLoopTraverser faceLoopTrav;
  OdBrErrorStatus err = faceLoopTrav.setFace(face);
  if (odbrUnsuitableTopology == err)
  {
    // Face without loops (sphere, torus)
    arrSurfaces.append(surfData);
    return eOk;
  }
  if (odbrOK != err)
  {
    return eInvalidInput;
  }

  while (!faceLoopTrav.done())
  {
    OdBrLoop loop = faceLoopTrav.getLoop();
    eStatus = performLoop(loop, surfData);
    if (eOk != eStatus)
    {
      return eStatus;
    }

    if (odbrOK != faceLoopTrav.next())
    {
      return eInvalidInput;
    }
  }
  arrSurfaces.append(surfData);
  return eOk;
}

OdResult OdBrepBuilderFillerHelper::performLoop(const OdBrLoop& loop,
  BrepBuilderInitialSurface& surfData)
{
  OdBrErrorStatus err;
  OdBrLoopEdgeTraverser loopEdgeTrav;
  err = loopEdgeTrav.setLoop(loop);

  // Loop with apex:
  if (odbrDegenerateTopology == err) // maybe there should be odbrUnsuitableTopology (see arx)
  {
    OdGeCurve3dPtr curve;
    OdGeCurve2dPtr paramCurve;
    OdResult resStatus = performLoopWithApex(loop, curve, paramCurve);
    if (eOk == resStatus)
    {
      m_initialData.edges.append(curve);
      surfData.loops.append(BrepBuilderInitialLoop(
        paramCurve,
        m_zeroBase + m_edges.append(OdBrEdge()), // stub edge for correct index
        OdBrepBuilder::kForward
      ));

      return eOk;
    }
    return resStatus;
  }
  if (odbrOK != err)
  {
    return eInvalidInput;
  }

  // Regular loop:
  BrepBuilderInitialLoop loopData;

  while (!loopEdgeTrav.done())
  {
    OdBrEdge edge = loopEdgeTrav.getEdge();

    BrepBuilderInitialCoedge& coedgeData = *loopData.coedges.append();

    OdGeCurve3dPtr curve(NULL);
    bool isNewEdge = true;
    for (OdArray<OdBrEdge>::size_type i = 0; i < m_edges.size(); ++i)
    {
      if (m_edges[i].isEqualTo(&edge))
      {
        isNewEdge = false;
        coedgeData.edgeIndex = m_zeroBase + i;
        curve = m_initialData.edges[coedgeData.edgeIndex].curve;
        break;
      }
    }
    if (isNewEdge)
    {
      if (!getEdgeCurveFixed(edge, curve))
      {
        return eInvalidInput;
      }
      BrepBuilderInitialEdge& edgeData = *m_initialData.edges.append();
      edgeData.curve = curve;

      // edge visual
      OdResult eStatus = edgeData.setupVisualInfo(edge, m_pMaterialHelper);
      if (eOk != eStatus)
      {
        return eStatus;
      }

      coedgeData.edgeIndex = m_zeroBase + m_edges.append(edge);
    }

    if (!m_params.isSkipCoedge2dCurve())
    {
      coedgeData.curve = getParamCurve(loopEdgeTrav);

      // acis bb required 2d coedge curve for nurb surface
      ODA_ASSERT_ONCE(!curve.isNull());
      ODA_ASSERT_ONCE(!surfData.pSurf.isNull());
      if (coedgeData.curve.isNull() && (surfData.pSurf->type() == OdGe::kNurbSurface))
      {
        coedgeData.curve = OdGeCurve2d::restoreUvCurve(curve, surfData.pSurf, m_toleranceRestore2dCurve);
        if (coedgeData.curve.isNull())
        {
          return eInvalidInput;
        }
      }

      OdResult eStatus = fixParamCurve(surfData.pSurf, curve, coedgeData.curve);
      //if surface is analytic-defined (cone, plane etc.) parametric curves may not required
      //so if we have such surface (any but not a spline) and 2d curve is "wrong" (wrong interval or direction)
      //this 2d curve may be detached and will not given to brep builder
      if (eOk != eStatus)
      {
        if (surfData.pSurf->type() != OdGe::kNurbSurface)
        {
          coedgeData.curve = OdGeCurve2dPtr();
        }
        else
        {
          return eStatus;
        }
      }
    }

    coedgeData.direction = (edge.getOrientToCurve() == loopEdgeTrav.getEdgeOrientToLoop())
      ? OdBrepBuilder::kForward : OdBrepBuilder::kReversed;

    if (odbrOK != loopEdgeTrav.next())
    {
      return eInvalidInput;
    }
  }

  // skip loop without coedge
  if (!loopData.coedges.empty())
  {
    surfData.loops.append(loopData);
  }

  return eOk;
}
//

OdResult OdBrepBuilderFillerModule::getDataFrom(const OdBrBrep& brep,
  OdIMaterialAndColorHelper* materialHelper, const OdBrepBuilderFillerParams& params,
  BrepBuilderInitialData& data)
{
  OdBrepBuilderFillerHelper brepBuilderFillerHelper(data, materialHelper, params);
  return brepBuilderFillerHelper.run(brep);
}

OdResult OdBrepBuilderFillerModule::initFrom(OdBrepBuilder& builder, const BrepBuilderInitialData& data,
  OdGeCurve3dPtrArray& arrEdges, OdGeCurve2dPtrArray& arrCoedges, OdGeSurfacePtrArray& arrSurfaces)
{
  if (!builder.isValid())
  {
    return eNotInitializedYet;
  }
  // TODO check can add geom (not implemented now)
  //if (!builder.canAddGeometry())
  //{
  //  return eInvalidInput;
  //}

  OdResult err = eOk;

  arrEdges.reserve(arrEdges.size() + data.edges.size());
  OdArray<BRepBuilderGeometryId> edgeIds(data.edges.size());
  for (BrepBuilderInitialCoedge::EdgeIndex i = 0; i < data.edges.size(); ++i)
  {
    const BrepBuilderInitialEdge& edgeData = data.edges[i];
    arrEdges.append(edgeData.curve);
    edgeIds.append(builder.addEdge(edgeData.curve));
    if (edgeData.hasColor)
    {
      err = builder.setEdgeColor(edgeIds.last(), edgeData.color);
      if (eOk != err)
      {
        return err;
      }
    }
  }

  for (BrepBuilderComplexArray::const_iterator complexIt = data.complexes.begin(); complexIt != data.complexes.end(); ++complexIt)
  {
    BRepBuilderGeometryId currentComplexId;
    //this try-catch is needed because BIM brep builder isn't work with complexes and shells
    //and throw exception eNotApplicable. This is normal state and we can continue 
    try
    {
      currentComplexId = builder.addComplex();
    }
    catch (const OdError& err)
    {
      if (err.code() != eNotApplicable)
      {
        throw err;
      }
    }
    for (BrepBuilderShellsArray::const_iterator shellIt = complexIt->begin(); shellIt != complexIt->end(); ++shellIt)
    {
      BRepBuilderGeometryId currentShellId(OdBrepBuilder::kDefaultShellId);
      try
      {
        currentShellId = builder.addShell(currentComplexId);
        ODA_ASSERT_ONCE(currentShellId != OdBrepBuilder::kDefaultShellId);
      }
      catch (const OdError& err)
      {
        if (err.code() != eNotApplicable)
        {
          throw err;
        }
      }
      for (BrepBuilderInitialSurfaceArray::const_iterator surfIt = shellIt->begin(); surfIt != shellIt->end(); ++surfIt)
      {
        arrSurfaces.append(surfIt->pSurf);
        BRepBuilderGeometryId currentFaceId = builder.addFace(surfIt->pSurf, surfIt->direction, currentShellId);

        if (surfIt->material)
        {
          builder.setFacesMaterial(currentFaceId, *surfIt->material);
        }
        if (surfIt->hasMaterialMapping)
        {
          err = builder.setFaceMaterialMapping(currentFaceId, surfIt->materialMapper);
          if (eOk != err)
          {
            return err;
          }
        }
        if (surfIt->hasColor)
        {
          err = builder.setFaceColor(currentFaceId, surfIt->color);
          if (eOk != err)
          {
            return err;
          }
        }

        const BrepBuilderInitialLoopArray& loops = surfIt->loops;
        for (BrepBuilderInitialLoopArray::const_iterator loopIt = loops.begin(); loopIt != loops.end(); ++loopIt)
        {
          BRepBuilderGeometryId currentLoopId = builder.addLoop(currentFaceId);

          const BrepBuilderInitialCoedgeArray& coedges = loopIt->coedges;
          for (BrepBuilderInitialCoedgeArray::const_iterator coedgeIt = coedges.begin(); coedgeIt != coedges.end(); ++coedgeIt)
          {
            arrCoedges.append(coedgeIt->curve);
            builder.addCoedge(currentLoopId, edgeIds[coedgeIt->edgeIndex], coedgeIt->direction, coedgeIt->curve);
          }

          builder.finishLoop(currentLoopId);
        }

        builder.finishFace(currentFaceId);
      }
      //this try-catch is needed because BIM brep builder isn't work with complexes and shells
      //and throw exception eNotApplicable. This is normal state and we can continue 
      try
      {
        builder.finishShell(currentShellId);
      }
      catch (const OdError& err)
      {
        if (err.code() != eNotApplicable)
        {
          throw err;
        }
      }
    }
    try
    {
      builder.finishComplex(currentComplexId);
    }
    catch(const OdError& err)
    { 
      if (err.code() != eNotApplicable)
      {
        throw err;
      }
    }
  }
  return eOk;
}

OdResult OdBrepBuilderFillerModule::initFrom(OdBrepBuilder& builder, const OdBrBrep& brep,
  OdGeCurve3dPtrArray& arrEdges, OdGeCurve2dPtrArray& arrCoedges, OdGeSurfacePtrArray& arrSurfaces,
  OdIMaterialAndColorHelper* materialHelper, const OdBrepBuilderFillerParams& params)
{
  // Checks
  if (!brep.isValid())
  {
    return eNotInitializedYet;
  }
  if (!builder.isValid())
  {
    return eNotInitializedYet;
  }
  // TODO check can add geom (not implemented now)
  //if (!builder.canAddGeometry())
  //{
  //  return eInvalidInput;
  //}

  BrepBuilderInitialData initData;

  OdResult res = getDataFrom(brep, materialHelper, params, initData);
  if (eOk != res)
  {
    return res;
  }
  return initFrom(builder, initData, arrEdges, arrCoedges, arrSurfaces);
}


OdResult OdBrepBuilderFillerModule::initFromNURBSingleFace(OdBrepBuilder& builder, const OdBrBrep& brep, OdGeCurve3dPtrArray& arrEdges, OdGeCurve2dPtrArray& arrCoedges, OdGeSurfacePtr &surface)
{
  try
  {
    OdBrErrorStatus err = odbrOK;
    OdBrBrepFaceTraverser bft;
    BrepBuilderInitialData ignore;
    OdBrepBuilderFillerHelper fillerHelper(ignore);
    if (bft.setBrep(brep) != odbrOK)
    {
      return eInvalidInput;
    }
    while (!bft.done() && (err == odbrOK))
    {
      OdBrFaceLoopTraverser faLoTrav;
      OdBrFace face = bft.getFace();
      OdGeNurbSurface *pNurbSurf = new OdGeNurbSurface;
      face.getSurfaceAsNurb(*pNurbSurf);
      surface = pNurbSurf;

      //add nurbs surface to BB
      bool bOrientToSurface = face.getOrientToSurface();
      BRepBuilderGeometryId complexId = builder.addComplex();
      BRepBuilderGeometryId shellId = builder.addShell(complexId);
      BRepBuilderGeometryId faceId = builder.addFace(pNurbSurf, bOrientToSurface ? OdBrepBuilder::kForward : OdBrepBuilder::kReversed, shellId);
      //iterate over trimming-loop and create data for BB
      for (faLoTrav.setFace(face); !faLoTrav.done(); faLoTrav.next())
      {
        BRepBuilderGeometryId LoopId = builder.addLoop(faceId);
        OdBrLoopEdgeTraverser loEdTrav;
        OdBrLoop loop = faLoTrav.getLoop();
        err = loEdTrav.setLoop(loop);
        if (odbrDegenerateTopology == err)
        {
          OdGeCurve3dPtr pCurve3d;
          OdGeCurve2dPtr pCurve2d;
          OdResult resStatus = fillerHelper.performLoopWithApex(loop, pCurve3d, pCurve2d);
          if (eOk == resStatus)
          {
            arrEdges.append(pCurve3d);
            arrCoedges.append(pCurve2d);
            BRepBuilderGeometryId edgeId = builder.addEdge(pCurve3d);
            builder.addCoedge(LoopId, edgeId, OdBrepBuilder::kForward, pCurve2d);
            err = odbrOK;
            continue;
          }
          return resStatus;
        }

        OdArray<OdBrEdge> arrBrepEdges;
        OdArray<BRepBuilderGeometryId> arrBrepEdgesID;
        for (; !loEdTrav.done(); loEdTrav.next())
        {
          OdBrEdge edge = loEdTrav.getEdge();
          unsigned int iFindIndex = 0;
          bool bFindEdge = false;
          for (unsigned int k = 0; k < arrBrepEdges.size(); k++)
          {
            if (edge.isEqualTo(&arrBrepEdges[k]))
            {
              //In case of nurbs cone we have one edge and 2 coedges, but brep returns 2 edges
              iFindIndex = k;
              bFindEdge = true;
              break;
            }
          }

          bool bEdgeForward = edge.getOrientToCurve();
          bool bCoedgeForward = loEdTrav.getEdgeOrientToLoop();

          OdGeCurve3dPtr pCurve3d;
          if (!fillerHelper.getEdgeCurveFixed(edge, pCurve3d))
          {
            return eInvalidInput;
          }
          arrEdges.append(pCurve3d);
          OdGeCurve2dPtr pCurve2d = fillerHelper.getParamCurve(loEdTrav);

          OdResult resStatus = fillerHelper.fixParamCurve(surface, pCurve3d, pCurve2d);
          //if surface is analytic-defined (cone, plane etc.) parametric curves may not required
          //so if we have such surface (any but not a spline) and 2d curve is "wrong" (wrong interval or direction)
          //this 2d curve may be detached and will not given to brep builder
          if (eOk != resStatus)
          {
            if (surface->type() == OdGe::kNurbSurface)
            {
              return resStatus; 
            }
            else
            {
              pCurve2d = OdGeCurve2dPtr();
            }
          }
          arrCoedges.append(pCurve2d);
          BRepBuilderGeometryId edgeId;
          if (!bFindEdge)
          {
            edgeId = builder.addEdge(pCurve3d);
            builder.addCoedge(LoopId, edgeId, bCoedgeForward ? OdBrepBuilder::kForward : OdBrepBuilder::kReversed, pCurve2d);
            arrBrepEdgesID.append(edgeId);
            arrBrepEdges.append(edge);
          }
          else
          {
            builder.addCoedge(LoopId, arrBrepEdgesID.at(iFindIndex), bCoedgeForward ? OdBrepBuilder::kForward : OdBrepBuilder::kReversed, pCurve2d);
          }
        }
      }
      builder.finishFace(faceId);
      builder.finishShell(shellId);
      builder.finishComplex(complexId);
      break;//Only one face
    }
  }
  catch (const OdError& err)
  {
    throw err;
  }
  catch (...)
  {
    return eInvalidInput;
  }
  return eOk;
}


// Member methods of BrepBuilderInitialData.h classes
OdResult BrepBuilderInitialSurface::setupVisualInfo(const OdBrFace& face, OdIMaterialAndColorHelper* pMaterialHelper)
{
  if (!pMaterialHelper) return eOk;
  return pMaterialHelper->getFaceVisualInfo(face, material, materialMapper, hasMaterialMapping, color, hasColor);
}

OdResult BrepBuilderInitialEdge::setupVisualInfo(const OdBrEdge& edge, OdIMaterialAndColorHelper* pMaterialHelper)
{
  if (!pMaterialHelper) return eOk;
  return pMaterialHelper->getEdgeVisualInfo(edge, color, hasColor);
}
