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

#ifndef _ExTrackers_h_Included_
#define _ExTrackers_h_Included_

#include "TD_PackPush.h"

#include "Ed/EdFunctionIO.h"
#include "SSet.h"
#include "StaticRxObject.h"
#include "ExEdBaseIO.h"
#include "Ge/GePlane.h"
#include "Ge/GeLineSeg2d.h"
#include "Gi/GiUtils.h"
#include "CmColorBase.h"
#include "RxObjectImpl.h"

/** <group ExServices_Classes>
*/
class GraphTrackerBase : public OdEdPointDefTracker, public OdGiDrawableImpl<>
{
protected:
  ODRX_USING_HEAP_OPERATORS(OdEdPointDefTracker);
protected:
  OdGsModelPtr m_pModel;
protected:
  GraphTrackerBase() { }

  bool hasGsModel() const { return !m_pModel.isNull(); }
  OdGsModel *gsModel() const { return const_cast<OdGsModel*>(m_pModel.get()); }
  void setGsModel(OdGsModel *pModel) { m_pModel = pModel; }
  void resetGsModel() { m_pModel.release(); }

  void initializeGsModel(OdGsView* pView, bool bForce = false)
  {
    if (pView && (bForce || !hasGsModel()) && pView->device())
    {
      OdGsModelPtr pModel = pView->device()->createModel();
      setGsModel(pModel.get());
      if (hasGsModel())
        gsModel()->setRenderType(OdGsModel::kDirect);
    }
  }

public:
  void invalidateGsModel()
  {
    OdGiDrawable* pParent = NULL;
    if (hasGsModel())
      gsModel()->onModified(this, pParent);
  }

  int addDrawables(OdGsView* pView)
  {
    initializeGsModel(pView);
    pView->add(this, gsModel());
    return 1;
  }
  void removeDrawables(OdGsView* pView)
  {
    pView->erase(this);
  }
};

/** <group ExServices_Classes>
*/
class PolylineSelectionTracker : public GraphTrackerBase
{
protected:
  ODRX_USING_HEAP_OPERATORS(GraphTrackerBase);
  const OdGePoint3dArray *m_pts;
  OdGePoint3d m_pt;
  //PolylineSelectionTracker() : m_pts(NULL) { }
  void setValue(const OdGePoint3d &pt)
  { 
    m_pt = pt;
    invalidateGsModel();
  }
  void setValue(const OdGePoint3dArray *pts)
  {
    m_pts = pts;
  }
  virtual OdUInt32 subSetAttributes(OdGiDrawableTraits* ) const ODRX_OVERRIDE
  {
    return kDrawableIsAnEntity;
  }
  virtual bool subWorldDraw(OdGiWorldDraw* pWd) const ODRX_OVERRIDE
  {
    OdGiDrawFlagsHelper _dfh(pWd->subEntityTraits(), OdGiSubEntityTraits::kDrawNoPlotstyle);
    if (m_pts && m_pts->size() > 1)
    {
      pWd->geometry().polyline((OdInt32)m_pts->size(), m_pts->asArrayPtr());
    }
    if (m_pts && m_pts->size() > 0)
    {
      OdGePoint3d tailPts[2] =
      {
        m_pts->last(),
        m_pt
      };
      pWd->geometry().polyline(2, tailPts);
    }
    return true;
  }
  OdGePoint3d basePoint() const
  {
    if (m_pts && !m_pts->empty())
      return m_pts->first();
    return OdGePoint3d::kOrigin;
  }
public:
  static OdEdPointTrackerPtr create(const OdGePoint3dArray *pts = NULL)
  {
    OdEdPointTrackerPtr pRes = OdRxObjectImpl<PolylineSelectionTracker, OdEdPointTracker>::createObject();
    static_cast<PolylineSelectionTracker*>(pRes.get())->setValue(pts);
    return pRes;
  }
};

/** <group ExServices_Classes>
*/
class PolygonSelectionTracker : public PolylineSelectionTracker
{
protected:
  bool subWorldDraw(OdGiWorldDraw* pWd) const
  {
    OdGiDrawFlagsHelper _dfh(pWd->subEntityTraits(), OdGiSubEntityTraits::kDrawNoPlotstyle);
    PolylineSelectionTracker::subWorldDraw(pWd);
    // Polygon segments must not intersect. This check must be implemented in selection command.
    if (m_pts && m_pts->size() > 1)
    {
      OdGePoint3d closePts[2] =
      {
        m_pt,
        m_pts->first()
      };
      pWd->geometry().polyline(2, closePts);
    }
    return true;
  }
public:
  static OdEdPointTrackerPtr create(const OdGePoint3dArray *pts = NULL)
  {
    OdEdPointTrackerPtr pRes = OdRxObjectImpl<PolygonSelectionTracker, OdEdPointTracker>::createObject();
    static_cast<PolygonSelectionTracker*>(pRes.get())->setValue(pts);
    return pRes;
  }
  static bool doesPolySegmentsNotIntersects(const OdGePoint3dArray &segments, const OdGePoint3d &newPoint)
  {
    if (segments.size() < 3)
      return true;
    OdGePoint3dArray poly = segments;
    //poly.push_back(newPoint);
    OdGeVector3d zAxis = ::odgiFaceNormal(poly.size(), poly.asArrayPtr());
    if (OdZero(zAxis.length()))
      return true; // All segments are parallel
    poly.push_back(newPoint);
    OdUInt32 nSeg;
    OdGeVector3d xAxis;
    for (nSeg = 0; nSeg < poly.size() - 1; nSeg++)
    {
      xAxis = segments[nSeg + 1] - segments[nSeg];
      OdGe::ErrorCondition error;
      xAxis.normalize(OdGeContext::gZeroTol, error);
      if (error == OdGe::kOk)
        break;
    }
    OdGeVector3d yAxis = zAxis.crossProduct(xAxis).normal();
    OdGePoint2dArray poly2d;
    poly2d.resize(poly.size());
    for (nSeg = 0; nSeg < poly.size(); nSeg++)
      poly2d[nSeg].set(poly[nSeg].asVector().dotProduct(xAxis), poly[nSeg].asVector().dotProduct(yAxis));
    // 1-st segment with all without last
    for (nSeg = 0; nSeg < poly2d.size() - 3; nSeg++)
    {
      OdGePoint2d rVal;
      if (OdGeLineSeg2d(poly2d[nSeg], poly2d[nSeg + 1]).intersectWith(OdGeLineSeg2d(poly2d[poly2d.size() - 2], poly2d.last()), rVal))
        return false;
    }
    // 2-nd segment with all without first
    for (nSeg = 1; nSeg < poly2d.size() - 2; nSeg++)
    {
      OdGePoint2d rVal;
      if (OdGeLineSeg2d(poly2d[nSeg], poly2d[nSeg + 1]).intersectWith(OdGeLineSeg2d(poly2d.first(), poly2d.last()), rVal))
        return false;
    }
    return true;
  }
};

/** <group ExServices_Classes>
*/
class PlaneSelectionTracker : public PolylineSelectionTracker
{
protected:
  bool subWorldDraw(OdGiWorldDraw* pWd) const
  {
    OdInt32 nDots = 75;
    OdGiDrawFlagsHelper _dfh(pWd->subEntityTraits(), OdGiSubEntityTraits::kDrawNoPlotstyle);
    if (m_pts && m_pts->size() > 1)
    {
      if ( m_pts->size() == 3)
        pWd->geometry().polyline((OdInt32)m_pts->size()-1, m_pts->asArrayPtr());
      else
        pWd->geometry().polyline((OdInt32)m_pts->size(), m_pts->asArrayPtr());
    }
    
    if (m_pts && m_pts->size() > 0 && m_pts->size() < 3)
    {
      OdGePoint3d tailPts[2] =
      {
        m_pts->last(),
        m_pt
      };

      if ( m_pts->size() > 1 ) //means draw normal
      {
        OdGiSubEntityTraits& traits = pWd->subEntityTraits();
        traits.setColor(OdCmEntityColor::kACIRed);
        pWd->geometry().rowOfDots(nDots, tailPts[0], (tailPts[1] - tailPts[0])/double(nDots));
      }
      else
        pWd->geometry().polyline(2, tailPts);
    }

    if (m_pts && m_pts->size() == 3) // not in drag - only after finising and adding the whole tracker to the storage
    {
      OdGePoint3d tailPts[2] =
      {
        (*m_pts)[1],
        (*m_pts)[2],
      };

      OdGiSubEntityTraits& traits = pWd->subEntityTraits();
      traits.setColor(OdCmEntityColor::kACIRed);
      pWd->geometry().rowOfDots(nDots, tailPts[0], (tailPts[1] - tailPts[0])/double(nDots));
    }

    return true;
  }
public:
  static OdEdPointTrackerPtr create(const OdGePoint3dArray *pts = NULL)
  {
    OdEdPointTrackerPtr pRes = OdRxObjectImpl<PlaneSelectionTracker, OdEdPointTracker>::createObject();
    static_cast<PlaneSelectionTracker*>(pRes.get())->setValue(pts);
    return pRes;
  }
};

/** <group ExServices_Classes>
*/
class PointToOrientTracker : public OdEdPointTracker
{
  OdGePoint3d       m_basePoint;
  OdEdRealTracker*  m_pTracker;
  OdGeMatrix3d      m_xUcsToWorld;
public:
  static OdEdPointTrackerPtr create(const OdGePoint3d& basePoint, OdEdRealTracker* pTracker)
  {
    OdSmartPtr<PointToOrientTracker> pRes;
    /*
    = OdRxObjectImpl<PointToOrientTracker>::createObject();
    pRes->m_basePoint = basePoint;
    pRes->m_pTracker = pTracker;
    */
#pragma MARKMESSAGE(TODO : implement PointToOrientTracker)
    return pRes;
  }
  void setValue(const OdGePoint3d& pt)
  {
    m_pTracker->setValue((pt-m_basePoint).length());
  }

  int addDrawables(OdGsView* pView) 
  { 
    return m_pTracker->addDrawables(pView); 
  }
  void removeDrawables(OdGsView* pView) 
  { 
    m_pTracker->removeDrawables(pView); 
  }
};

/** <group ExServices_Classes>
*/
class ANGBASE_Tracker : public OdEdRealTracker
{
  OdEdRealTracker*  m_pAngleTracker;
  double            m_ANGBASE;
protected:
  ANGBASE_Tracker() {}
public:
  static OdEdRealTrackerPtr create(OdEdRealTracker* pAngleTracker, double ANGBASE)
  {
    OdSmartPtr<ANGBASE_Tracker> pRes = OdRxObjectImpl<ANGBASE_Tracker>::createObject();
    pRes->m_pAngleTracker = pAngleTracker;
    pRes->m_ANGBASE = ANGBASE;
    return pRes;
  }
  void setValue(double orient)
  {
    m_pAngleTracker->setValue(orient - m_ANGBASE);
  }

  int addDrawables(OdGsView* pView) 
  { 
    return m_pAngleTracker->addDrawables(pView); 
  }
  void removeDrawables(OdGsView* pView) 
  { 
    m_pAngleTracker->removeDrawables(pView); 
  }
};

/** <group ExServices_Classes>
*/
class RubberBand : public GraphTrackerBase
{
  ODRX_USING_HEAP_OPERATORS(GraphTrackerBase);
  OdGePoint3d m_pts[2];

  virtual OdUInt32 subSetAttributes(OdGiDrawableTraits* ) const ODRX_OVERRIDE
  {
    return kDrawableIsAnEntity;
  }
  virtual bool subWorldDraw(OdGiWorldDraw* pWd) const ODRX_OVERRIDE
  {
    OdGiDrawFlagsHelper _dfh(pWd->subEntityTraits(), OdGiSubEntityTraits::kDrawNoPlotstyle);
    pWd->geometry().polyline(2, m_pts);
    return true;
  }
  OdGePoint3d basePoint() const
  {
    return m_pts[0];
  }
public:
  static OdEdPointTrackerPtr create(const OdGePoint3d& base, OdGsModel* pModel = NULL)
  {
    OdEdPointTrackerPtr pRes = OdRxObjectImpl<RubberBand, OdEdPointTracker>::createObject();
    static_cast<RubberBand*>(pRes.get())->m_pts[0] = base;
    static_cast<RubberBand*>(pRes.get())->setGsModel(pModel);
    return pRes;
  }

  void setValue(const OdGePoint3d& p)
  {
    m_pts[1] = p;
    invalidateGsModel();
  }
};

/** <group ExServices_Classes>
*/
class RectFrame : public GraphTrackerBase
{
protected:
  ODRX_USING_HEAP_OPERATORS(GraphTrackerBase);
  mutable OdGePoint3d m_pts[4];

  virtual OdUInt32 subSetAttributes(OdGiDrawableTraits* pDrawTraits) const ODRX_OVERRIDE
  {
    OdGiSubEntityTraitsPtr pTraits = OdGiSubEntityTraits::cast(pDrawTraits);
    if (!pTraits.isNull())
      pTraits->setFillType(kOdGiFillNever);
    return kDrawableIsAnEntity;
  }
  virtual bool subWorldDraw(OdGiWorldDraw* pWd) const ODRX_OVERRIDE
  {
    return false;
  }
  virtual void subViewportDraw(OdGiViewportDraw* pVd) const ODRX_OVERRIDE
  {
    const OdGiViewport& vp = pVd->viewport();
    OdGeMatrix3d x = vp.getWorldToEyeTransform();
    OdGePoint3d p0 = x * m_pts[0];
    OdGePoint3d p2 = x * m_pts[2];
    
    m_pts[1].x = p0.x;
    m_pts[3].x = p2.x;
    m_pts[1].y = p2.y;
    m_pts[3].y = p0.y;
    m_pts[1].z =
    m_pts[3].z = p2.z;
    
    x = vp.getEyeToWorldTransform();
    m_pts[1].transformBy(x);
    m_pts[3].transformBy(x);

    if (hasGsModel())
    {
      // fix invalid plane clipping of selection rectangle for some DGN drawing (like samp4.dgn)
      bool clipFront = false, 
           clipBack = false;
      double front, 
             back;
      if (   vp.getFrontAndBackClipValues(clipFront, clipBack, front, back)
          && clipFront && clipBack)
      {
        //OdGePoint3d ptBase = m_pts[0];
        OdGePoint3d ptBase = vp.getCameraLocation();
        ptBase += vp.viewDir().normalize() * ((front + back) / 2.0);
        OdGePlane plane(ptBase, vp.viewDir());
        for (int index = 0; index < 4; index++)
        {
          if (!plane.isOn(m_pts[index]))
          {
            OdGePoint3d ptProj;
            if (plane.project(m_pts[index], ptProj))
              m_pts[index] = ptProj;
          }
        }
      }
    }

    OdGiDrawFlagsHelper _dfh(pVd->subEntityTraits(), OdGiSubEntityTraits::kDrawNoPlotstyle);
    pVd->geometry().polygon(4, m_pts);
  }
  OdGePoint3d basePoint() const
  {
    return m_pts[0];
  }
public:
  static OdEdPointTrackerPtr create(const OdGePoint3d& base, OdGsModel* pModel)
  {
    OdEdPointTrackerPtr pRes = OdRxObjectImpl<RectFrame, OdEdPointTracker>::createObject();
    static_cast<RectFrame*>(pRes.get())->m_pts[0] = base;
    static_cast<RectFrame*>(pRes.get())->setGsModel(pModel);
    return pRes;
  }
  void setValue(const OdGePoint3d& p)
  {
    m_pts[2] = p;
    invalidateGsModel();
  }
};

#include "TD_PackPop.h"

#endif // _ExTrackers_h_Included_
