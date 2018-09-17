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

#if defined(ODA_WINDOWS) && !defined(_WINRT) && !defined ( _WIN32_WCE )
#include "win/GdiBmp.h"
#else
#define drawWithGdiBmp(pDrawable, pVect, origin, u, v, bitmapWidth, bitmapHeight, rect, bMonochrome, \
                       isExport, viewportOriginX, viewportOriginY)
#endif

inline bool isDcAligned(const OdGeVector3d& u, const OdGeVector3d& v)
{
  return (
    u.isParallelTo(OdGeVector3d::kXAxis) && v.isParallelTo(OdGeVector3d::kYAxis) ||
    u.isParallelTo(OdGeVector3d::kYAxis) && v.isParallelTo(OdGeVector3d::kXAxis)
         );
}

class SimpleQuadrilateral
{
  OdGePoint2d points[4];
  public:
    SimpleQuadrilateral() { }

    void setPoint(int num, double x, double y) { points[num].set(x, y); }

    bool isPointInside(double x, double y)
    {
      if (OdGreaterOrEqual(x, points[0].x) && OdGreaterOrEqual(y, points[0].y) &&
          OdLessOrEqual(x, points[2].x) && OdLessOrEqual(y, points[2].y))
        return true;
      return false;
    }
    bool intersectWith(const OdGePoint2d &a, const OdGePoint2d &b, int nSeg, double &t)
    {
      const double tol = OdGeContext::gTol.equalPoint();
      const OdGePoint2d &c = points[nSeg];
      const OdGePoint2d &d = points[(nSeg + 1) % 4];
      double d0 = (b.y - a.y) * (c.x - d.x) - (c.y - d.y) * (b.x - a.x);
      double d1 = (c.y - a.y) * (c.x - d.x) - (c.y - d.y) * (c.x - a.x);
      //double d2 = (b.y - a.y) * (c.x - a.x) - (c.y - a.y) * (b.x - a.x);
      if (OdNonZero(d0, tol))
      {
        double t1 = d1 / d0;
        //double t2 = d2 / d0;
        if ((t1 > -tol) && (t1 < 1. + tol))
        {
          t = t1;
          return true;	
        }
      }
      return false;
    }
    void addTx(OdGePoint2d &tMin, OdGePoint2d &tMax, double tx, double ty, bool bTx = true, bool bTy = true)
    {
      if (bTx)
      {
        if (tx < tMin.x) tMin.x = tx;
        if (tx > tMax.x) tMax.x = tx;
      }
      if (bTy)
      {
        if (ty < tMin.y) tMin.y = ty;
        if (ty > tMax.y) tMax.y = ty;
      }
    }
    void addPointSec(OdGePoint2d &tMin, OdGePoint2d &tMax, double x, double y, double tx, double ty)
    {
      if (isPointInside(x, y))
        addTx(tMin, tMax, tx, ty);
    }
    void addIsectSec(OdGePoint2d &tMin, OdGePoint2d &tMax, double ax, double ay, double atx, double aty,
                                                           double bx, double by, double btx, double bty)
    {
      for (int i = 0; i < 4; i++)
      {
        OdGePoint2d a(ax, ay), b(bx, by);
        double t;
        if (intersectWith(a, b, i, t))
          addTx(tMin, tMax, atx + (btx - atx) * t, aty + (bty - aty) * t, (i & 1) == 1, (i & 1) == 0);
      }
    }
};

static void exOwnerDrawDc(const OdGePoint3d& _origin,
                          const OdGeVector3d& _u,
                          const OdGeVector3d& _v,
                          const OdGiSelfGdiDrawable* pDrawable,
                          bool bDcAligned,
                          bool /*bAllowClipping*/,
                          OdGsBaseVectorizer& vect,
                          bool bNoClipVP = false, // Disable viewport clipping
                          bool bNoClipDC = false) // Disable output rect clipping
{
  OdGsViewImpl &view = vect.view();

  OdGeMatrix3d x = vect.getModelToEyeTransform();

  OdGeVector3d u(x * _u), v(x * _v);

  if(bDcAligned && !isDcAligned(u, v))
    return;

  OdGePoint3d origin(x * _origin);
  OdGePoint3d origin2;
  OdGeVector3d u2;
  OdGeVector3d v2;
  ODRECT rect;

  OdGeExtents3d ext(origin, origin);
  ext.addPoint(origin + u);
  ext.addPoint(origin + v);
  ext.addPoint(origin + u + v);

  OdGePoint3d imgMin = ext.minPoint();
  OdGePoint3d imgMax = ext.maxPoint();

  OdGePoint3d vpMax(view.fieldWidth() / 2., view.fieldHeight() / 2., 0.);
  OdGePoint3d vpMin(-vpMax.x, -vpMax.y, -vpMax.z);

  const OdGsDCRect& dcRect = static_cast<OdGsBaseVectorizeDevice*>(view.device())->outputRect();

  x = view.eyeToScreenMatrix().invert();
  OdGePoint3d dcMin(dcRect.m_min.x, dcRect.m_min.y, 0.);
  dcMin.transformBy(x);
  OdGePoint3d dcMax(dcRect.m_max.x, dcRect.m_max.y, 0.);
  dcMax.transformBy(x);

  if (!bNoClipVP)
  {
    if(imgMax.x < vpMin.x || imgMax.y < vpMin.y ||
       imgMin.x > vpMax.x || imgMin.y > vpMax.y)
      return;
  }

  if (!bNoClipDC)
  {
    if(imgMax.x < dcMin.x || imgMax.y < dcMin.y ||
       imgMin.x > dcMax.x || imgMin.y > dcMax.y)
      return;
  }

  if (!bNoClipVP)
  {
    if(imgMin.x < vpMin.x)
      imgMin.x = vpMin.x;
    if(imgMin.y < vpMin.y)
      imgMin.y = vpMin.y;
    if(imgMax.x > vpMax.x)
      imgMax.x = vpMax.x;
    if(imgMax.y > vpMax.y)
      imgMax.y = vpMax.y;
  }

  if (!bNoClipDC)
  {
    if(imgMin.x < dcMin.x)
      imgMin.x = dcMin.x;
    if(imgMin.y < dcMin.y)
      imgMin.y = dcMin.y;
    if(imgMax.x > dcMax.x)
      imgMax.x = dcMax.x;
    if(imgMax.y > dcMax.y)
      imgMax.y = dcMax.y;
  }

  if (bDcAligned && isDcAligned(u, v))
  {
    origin2 = imgMin;
    v2 = u2 = imgMax - imgMin;
    u2.z = u2.y =0.;
    v2.z = v2.x =0.;
  }
  else
  {
    if (bNoClipVP && bNoClipDC)
    {
      origin2 = origin;
      u2 = u; v2 = v;

      ext.set(imgMin, OdGePoint3d(imgMin.x + u.convert2d().length(), imgMin.y + v.convert2d().length(), imgMax.z));
    }
    else
    {
      OdGePoint2d tMin(1.0, 1.0), tMax(0.0, 0.0);
      SimpleQuadrilateral secRec;
      secRec.setPoint(0, imgMin.x, imgMin.y);
      secRec.setPoint(1, imgMax.x, imgMin.y);
      secRec.setPoint(2, imgMax.x, imgMax.y);
      secRec.setPoint(3, imgMin.x, imgMax.y);
      secRec.addPointSec(tMin, tMax, origin.x, origin.y, 0.0, 0.0);
      secRec.addIsectSec(tMin, tMax, origin.x, origin.y, 0.0, 0.0, origin.x + u.x, origin.y + u.y, 1.0, 0.0);
      secRec.addPointSec(tMin, tMax, origin.x + u.x, origin.y + u.y, 1.0, 0.0);
      secRec.addIsectSec(tMin, tMax, origin.x + u.x, origin.y + u.y, 1.0, 0.0, origin.x + u.x + v.x, origin.y + u.y + v.y, 1.0, 1.0);
      secRec.addPointSec(tMin, tMax, origin.x + u.x + v.x, origin.y + u.y + v.y, 1.0, 1.0);
      secRec.addIsectSec(tMin, tMax, origin.x + u.x + v.x, origin.y + u.y + v.y, 1.0, 1.0, origin.x + v.x, origin.y + v.y, 0.0, 1.0);
      secRec.addPointSec(tMin, tMax, origin.x + v.x, origin.y + v.y, 0.0, 1.0);
      secRec.addIsectSec(tMin, tMax, origin.x + v.x, origin.y + v.y, 0.0, 1.0, origin.x, origin.y, 0.0, 0.0);
      if (OdLessOrEqual(tMax.x, tMin.x) || OdLessOrEqual(tMax.y, tMin.y))
        return;
      origin2 = origin + u * tMin.x + v * tMin.y;
      u2 = u * (tMax.x - tMin.x);
      v2 = v * (tMax.y - tMin.y);

      ext.set(OdGePoint3d(imgMin.x - (u * tMin.x).convert2d().length(), imgMin.y - (v * tMin.y).convert2d().length(), ext.minPoint().z),
              OdGePoint3d(imgMin.x + (u * (1.0 - tMin.x)).convert2d().length(), imgMin.y + (v * (1.0 - tMin.y)).convert2d().length(), ext.maxPoint().z));
    }
    imgMax.x = imgMin.x + u2.convert2d().length();
    imgMax.y = imgMin.y + v2.convert2d().length();
  }

  x = vect.eyeToOutputTransform();
  origin2.transformBy(x);
  u2.transformBy(x);
  v2.transformBy(x);

  x = view.eyeToScreenMatrix();
  ext.transformBy(x);
  imgMax.transformBy(x);
  imgMin.transformBy(x);
  dcMin.set(dcRect.m_min.x, dcRect.m_min.y, 0.);
  dcMax.set(dcRect.m_max.x, dcRect.m_max.y, 0.);

  try
  {
    rect.left   = OdRoundToLong(ext.minPoint().x);
    rect.right  = OdRoundToLong(ext.maxPoint().x);
    rect.top    = OdRoundToLong(ext.maxPoint().y);
    rect.bottom = OdRoundToLong(ext.minPoint().y);
  }
  catch (const OdError&)
  {
    // #10225 : Can't render if arithmetic overflow thrown
    return;
  }

  OdSmartPtr<OdGiSelfGiDrawablePE> pOdGiSelfGiDrawablePE = OdGiSelfGiDrawablePE::cast(pDrawable);
  if (   pOdGiSelfGiDrawablePE.get() 
      && pOdGiSelfGiDrawablePE->draw(const_cast<OdGiSelfGdiDrawable*>(pDrawable), 
                                     &vect.output().destGeometry(), origin2, u2, v2, (OdGsDCRect&)rect))
    return;

  drawWithGdiBmp(pDrawable, &vect, origin2, u2, v2, 
                 OdRoundToLong(fabs(imgMax.x - imgMin.x)), OdRoundToLong(fabs(imgMax.y - imgMin.y)),
                 (OdGsDCRect&)rect, false, false,
                 OdRoundToLong(imgMin.x - dcMin.x), OdRoundToLong(imgMax.y - dcMax.y));
}
