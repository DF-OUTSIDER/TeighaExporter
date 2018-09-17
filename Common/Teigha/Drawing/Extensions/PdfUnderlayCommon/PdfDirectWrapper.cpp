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

// PDFDirect module wrapper (used for isolate same typenames).
#include "OdaCommon.h"
#include "PdfDirectWrapper.h"

#include "MemoryStream.h"
#include "OdPlatformStreamer.h"
#include "RxRasterServices.h"

#include "Gi/GiViewportDraw.h"
#include "Ge/avgpc.h"
#include "Ge/GeExtents2d.h"

#include "DbHostAppServices.h"
#include "PdfUnderlayItemImpl.h"

#define PDF_UNDERLAY_GEOMETRY_CACHE
#ifdef PDF_UNDERLAY_GEOMETRY_CACHE
bool cacheUnderlayGeometrySplinesAsPolylines = true;
#else
bool cacheUnderlayGeometrySplinesAsPolylines = false;
#endif

ODRX_NO_CONS_DEFINE_MEMBERS(PdfDirectWrapper, OdRxObject);
ODRX_NO_CONS_DEFINE_MEMBERS(OdDbPdfUnderlayItemPE, OdRxObject);

OdStaticRxObject<OdDbPdfUnderlayItemPE> PdfDirectWrapper::m_PdfItemPE;

OdGiRasterImagePtr PdfDirectWrapper::convertBmpToRaster(const BITMAPINFOHEADER &bmi, const unsigned char *imagedata)  const
{
  OdStreamBufPtr pFileBuf = OdMemoryStream::createNew();

  OdPlatformStreamer::wrInt16(*pFileBuf, 0x4d42);
  OdUInt32 scanLinesPos = 14 + 40;  // Below version is correct only on Windows
  OdUInt32 size = scanLinesPos + bmi.biWidth*32*Od_abs(bmi.biHeight);

  OdPlatformStreamer::wrInt32(*pFileBuf, size);
  OdPlatformStreamer::wrInt32(*pFileBuf, 0); // reserved
  OdPlatformStreamer::wrInt32(*pFileBuf, scanLinesPos); // offBits
  OdPlatformStreamer::wrInt32(*pFileBuf, bmi.biSize);  // not portable: sizeof(BITMAPINFOHEADER));
  // save BITMAPINFOHEADER
  OdPlatformStreamer::wrInt32(*pFileBuf, bmi.biWidth);
  OdPlatformStreamer::wrInt32(*pFileBuf, bmi.biHeight);
  OdPlatformStreamer::wrInt16(*pFileBuf, bmi.biPlanes);
  OdPlatformStreamer::wrInt16(*pFileBuf, bmi.biBitCount);
  OdPlatformStreamer::wrInt32(*pFileBuf, bmi.biCompression);
  OdPlatformStreamer::wrInt32(*pFileBuf, bmi.biSizeImage);
  OdPlatformStreamer::wrInt32(*pFileBuf, bmi.biXPelsPerMeter);
  OdPlatformStreamer::wrInt32(*pFileBuf, bmi.biYPelsPerMeter);
  OdPlatformStreamer::wrInt32(*pFileBuf, bmi.biClrUsed);
  OdPlatformStreamer::wrInt32(*pFileBuf, bmi.biClrImportant);

  pFileBuf->putBytes(imagedata, bmi.biSizeImage);

  pFileBuf->rewind();

  OdGiRasterImagePtr pTmp;
  OdRxRasterServicesPtr pRasSvcs = odrxDynamicLinker()->loadApp(RX_RASTER_SERVICES_APPNAME);
  if ( !pRasSvcs.isNull() )
  {
    pTmp = pRasSvcs->loadRasterImage(pFileBuf);
  }
  return pTmp;
}

void PdfDirectWrapper::method1(double uScale, double vScale, OdGiViewportDraw* pVd, OdGeExtents2d &resUnCS, bool &bFlag) const
{
  OdGeMatrix3d x = pVd->viewport().getModelToEyeTransform();

  OdGePoint2d vps[2];
  pVd->viewport().getViewportDcCorners(vps[0], vps[1]);  // in EYE

                                                         //#define PDF_DEBUG
#ifdef PDF_DEBUG
  OdGeMatrix3d xx = x.inverse();

  {
    double d = (vps[1].x - vps[0].x) / 10. * 2;
    vps[0].x += d;
    vps[1].x -= d;
    d = (vps[1].y - vps[0].y) / 10. * 2;
    vps[0].y += d;
    vps[1].y -= d;

    // draw new viewport
    OdGePoint3d pnts[5];
    pnts[0].set(vps[0].x, vps[0].y, 0);
    pnts[1].set(vps[1].x, vps[0].y, 0);
    pnts[2].set(vps[1].x, vps[1].y, 0);
    pnts[3].set(vps[0].x, vps[1].y, 0);
    pnts[4] = pnts[0];

    pnts[0].transformBy(xx); // now in WCS
    pnts[1].transformBy(xx);
    pnts[2].transformBy(xx); // now in WCS
    pnts[3].transformBy(xx);
    pnts[4].transformBy(xx);

    pVd->subEntityTraits().setColor(0);
    pVd->geometry().polyline(5, pnts);
  }
#endif

  OdGePoint3d origin(OdGePoint3d::kOrigin); // in WCS
  OdGeVector3d u(OdGeVector3d::kXAxis * uScale), v(OdGeVector3d::kYAxis * vScale);

  origin.transformBy(x); // now in EYE
  u.transformBy(x); // now in EYE
  v.transformBy(x); // now in EYE

  origin.z = u.z = v.z = 0;

  /// to underlay CS
  OdGeMatrix2d mx, mmx;
  mmx.setCoordSystem((const OdGePoint2d&)origin, (const OdGeVector2d&)u, (const OdGeVector2d&)v);
  mx = mmx.inverse();

  int holeflag = /*FALSE*/0;
  gpc_vertex_list gpcVL, gpcVL2;
  gpc_vertex gpcVXs[4], gpcVXs2[4];
  gpc_polygon gpVP, gpVP2, gpVP3;

  gpVP.num_contours = 1;
  gpVP.hole = &holeflag;
  gpVP.contour = &gpcVL;
  gpVP.contour->num_vertices = 4;
  gpVP.contour->vertex = &gpcVXs[0];
  gpcVXs[0].x = vps[0].x;
  gpcVXs[0].y = vps[0].y;
  gpcVXs[1].x = vps[0].x;
  gpcVXs[1].y = vps[1].y;
  gpcVXs[2].x = vps[1].x;
  gpcVXs[2].y = vps[1].y;
  gpcVXs[3].x = vps[1].x;
  gpcVXs[3].y = vps[0].y;

  gpVP2.num_contours = 1;
  gpVP2.hole = &holeflag;
  gpVP2.contour = &gpcVL2;
  gpVP2.contour->num_vertices = 4;
  gpVP2.contour->vertex = &gpcVXs2[0];
  gpcVXs2[0].x = origin.x;
  gpcVXs2[0].y = origin.y;
  gpcVXs2[1].x = origin.x + u.x;
  gpcVXs2[1].y = origin.y + u.y;
  gpcVXs2[2].x = origin.x + u.x + v.x;
  gpcVXs2[2].y = origin.y + u.y + v.y;
  gpcVXs2[3].x = origin.x + v.x;
  gpcVXs2[3].y = origin.y + v.y;

  gpc_polygon_clip(GPC_INT, &gpVP, &gpVP2, &gpVP3);

  if (gpVP3.num_contours == 0)
  {
    bFlag = /*FALSE*/0;
    return;
  }

  resUnCS = OdGeExtents2d();
  for (int f = 0; f < gpVP3.contour[0].num_vertices; ++f)
  {
    OdGePoint2d p(gpVP3.contour[0].vertex[f].x, gpVP3.contour[0].vertex[f].y);
    p.transformBy(mx);
    resUnCS.addPoint(p); // in underlayCS
  }

  OdGeExtents2d uB;
  uB.addPoint(OdGePoint2d(0, 0));
  uB.addPoint(OdGePoint2d(1, 1));
  resUnCS.intersectWith(uB, &resUnCS); // in underlayCS

#ifdef PDF_DEBUG
  {
    OdGePoint3d p[5];
    (OdGePoint2d&)p[0] = resUnCS.minPoint();
    (OdGePoint2d&)p[2] = resUnCS.maxPoint();
    p[1] = p[0]; p[1].y = p[2].y;
    p[3] = p[0]; p[3].x = p[2].x;
    p[4] = p[0];

    ((OdGePoint2d&)p[0]).transformBy(mmx); // in EYE
    ((OdGePoint2d&)p[1]).transformBy(mmx);
    ((OdGePoint2d&)p[2]).transformBy(mmx);
    ((OdGePoint2d&)p[3]).transformBy(mmx);
    ((OdGePoint2d&)p[4]).transformBy(mmx);

    p[0].transformBy(xx); // in WCS
    p[1].transformBy(xx);
    p[2].transformBy(xx);
    p[3].transformBy(xx);
    p[4].transformBy(xx);

    pVd->subEntityTraits().setColor(3);
    pVd->geometry().polyline(5, p);
  }
#endif

  gpc_free_polygon(&gpVP3);

  bFlag = /*TRUE*/1;
}

OdGeExtents2d PdfDirectWrapper::extendVisiblePart(const OdGeExtents2d& visiblePartUnCS) const
{
  OdGeVector2d visibleSize = visiblePartUnCS.maxPoint() - visiblePartUnCS.minPoint();

  // we keep a larger image than what is actually displayed.
  // extended area will be about 4 times larger than visible area
  OdGeVector2d extendBy = visibleSize * 0.5;
  OdGeExtents2d extUnCS = visiblePartUnCS;
  extUnCS.expandBy(extendBy);
  extUnCS.expandBy(-extendBy);
  OdGePoint2d minPt(extUnCS.minPoint()), maxPt(extUnCS.maxPoint());

  {
    minPt.x -= 1;
    minPt.y -= 1;
    maxPt.x = (OdUInt32)(maxPt.x + 1.5);
    maxPt.y = (OdUInt32)(maxPt.y + 1.5);
  }

  if (minPt.x < 0.)
    minPt.x = 0.;
  if (minPt.y < 0.)
    minPt.y = 0.;
  extUnCS.set(minPt, maxPt);
  return extUnCS;
}

bool PdfDirectWrapper::ImageCache::isUContextEqual(const OdDbUnderlayDrawContext& drawCtx1, const OdDbUnderlayDrawContext& drawCtx2)
{
  if (drawCtx1.contrast == drawCtx2.contrast &&
    drawCtx1.fade == drawCtx2.fade &&
    drawCtx1.monochrome == drawCtx2.monochrome &&
    drawCtx1.adjustColorForBackground == drawCtx2.adjustColorForBackground)
  {
    OdArray<OdUnderlayLayer>::const_iterator it1 = drawCtx1.layers.begin();
    for (; it1 != drawCtx1.layers.end(); it1++)
    {
      bool isExist = false;
      OdArray<OdUnderlayLayer>::const_iterator it2 = drawCtx2.layers.begin();
      for (; it2 != drawCtx2.layers.end(); it2++)
      {
        if (it1->name() == it2->name() && it1->state() == it2->state())
        {
          isExist = true;
          break;
        }
      }
      if (!isExist)
        return false;
    }
    return true;
  }
  return false;
}

PdfDirectWrapper::ImageCache& PdfDirectWrapper::ImageCacheHolder::getPdfInfo(OdUInt32 vpId, const OdDbObjectId& objId) //const
{
  TD_AUTOLOCK(m_lock);
  if (m_cachedPdfInfoArray.size() <= vpId)
    m_cachedPdfInfoArray.resize(vpId + 1);

  OdImageCacheMap::iterator iter = m_cachedPdfInfoArray[vpId].find(objId);
  if (iter == m_cachedPdfInfoArray[vpId].end())
  {
    ImageCache cache;
    m_cachedPdfInfoArray[vpId][objId] = cache;
    OdDbObjectPtr pObjSrc = objId.safeOpenObject();
    pObjSrc->addReactor(this);
  }
  return m_cachedPdfInfoArray[vpId][objId];
}

void PdfDirectWrapper::ImageCacheHolder::erased(const OdDbObject* pObj, bool bErasing)
{
  TD_AUTOLOCK(m_lock);
  OdDbObjectId objID = pObj->id();
  pObj->removeReactor(this);
  for (OdUInt32 i = 0; i < m_cachedPdfInfoArray.size(); i++)
  {
    OdImageCacheMap::iterator iter = m_cachedPdfInfoArray[i].find(objID);
    if (iter != m_cachedPdfInfoArray[i].end())
      m_cachedPdfInfoArray[i].erase(iter);
  }
}

void PdfDirectWrapper::ImageCacheHolder::goodbye(const OdDbObject* pObject)
{
  erased(pObject);
}

void PdfDirectWrapper::ImageCacheHolder::clear()
{
  TD_AUTOLOCK(m_lock);
  for (OdUInt32 i = 0; i < m_cachedPdfInfoArray.size(); i++)
  {
    for (OdImageCacheMap::iterator iter = m_cachedPdfInfoArray[i].begin(); iter != m_cachedPdfInfoArray[i].end(); iter++)
    {
      try
      {
        iter->first.safeOpenObject()->removeReactor(this);
      }
      catch (...)
      {
      }
    }
    m_cachedPdfInfoArray[i].clear();
  }
  m_cachedPdfInfoArray.clear();
};

OdResult PdfDirectWrapper::getSnapGeometry(OdDbDatabase *pDb, int nPage, OdGiGeometry *pGrabber, const OdArray<OdUnderlayLayer> &layers, const OdGePoint3d* pickPtMc) const
{
  //if (!security(getKey()))
    //return eKeyNotFound;

  if (m_path.isEmpty() || nPage < 0 || nPage > getItemsCount() || !pGrabber)
    return eInvalidInput;

  if (m_geometryCache.m_nPage != nPage)
    createGeometryCache(nPage, layers);

  OdGeExtents2d selectionBoxMc;
  {
    ODA_ASSERT_ONCE(pDb);
    if (pDb && pickPtMc)
    {
      double apertureMc = pDb->appServices()->getAPERTURE() * getMeasureDictInfo(nPage);
      OdGeVector2d ap(apertureMc, apertureMc);
      OdGePoint2d minPtMc = pickPtMc->convert2d() - ap;
      OdGePoint2d maxPtMc = pickPtMc->convert2d() + ap;
      selectionBoxMc.set(minPtMc, maxPtMc);
    }
  }

  bool bFullTrace = pickPtMc == 0;

  for (GeometryCache::Polylines::iterator i = m_geometryCache.m_polylines.begin();
    i != m_geometryCache.m_polylines.end(); ++i)
  {
    if (bFullTrace || !selectionBoxMc.isDisjoint(i->second))
      pGrabber->polyline(i->first.size(), i->first.asArrayPtr());
  }

  for (GeometryCache::Splines::iterator i = m_geometryCache.m_splines.begin();
    i != m_geometryCache.m_splines.end(); ++i)
  {
    if (bFullTrace || !selectionBoxMc.isDisjoint(i->second))
      pGrabber->nurbs(i->first);
  }

  for (GeometryCache::Ellipses::iterator i = m_geometryCache.m_ellipses.begin();
    i != m_geometryCache.m_ellipses.end(); ++i)
  {
    if (bFullTrace || !selectionBoxMc.isDisjoint(i->second))
      pGrabber->ellipArc(i->first);
  }

  // check boundary

  return eOk;
}

OdResult PdfDirectWrapper::optimizeSplines(OdGiViewportDraw* pVd) const
{
  m_geometryCache.m_splines2polylines.clear();
  for (GeometryCache::Splines::iterator i = m_geometryCache.m_splines.begin();
    i != m_geometryCache.m_splines.end(); ++i)
  {
    OdGeNurbCurve3d nurb = i->first;
    OdGeExtents2d ext = i->second;

    double dev = pVd->deviation(kOdGiMaxDevForCurve, nurb.controlPointAt(0));
    if (dev != 0.0) // appendSamplePoints uses strong compare
    {
      OdGeExtents3d ext;
      int nCp = nurb.numControlPoints();
      // OdGeCurve3d::boundBlock should be used here then it will be implemented
      for (int i = 0; i < nCp; i++)
        ext.addPoint(nurb.controlPointAt(i));
      const OdGePoint3d &ptMin = ext.minPoint(), &ptMax = ext.maxPoint();
      OdGeVector3d nrbExt = ptMax - ptMin;
      // This is correct to get deviation here with /ptMin + nrbExt * 0.5/ instead of 0 control point, but
      // sometimes deviations is null, so we could avoid extents calculation for optimization purposes.
      double dMax = OdGreater(nrbExt.x, nrbExt.y) ? nrbExt.x : nrbExt.y;
      if (OdGreater(nrbExt.z, dMax)) dMax = nrbExt.z;
      // Some magic value for keep kOdGiMaxDevForCurve near Acad range
      dev *= 0.007;
      dMax *= 1.0e-5;
      if (OdGreater(dMax, dev))
        dev = dMax;
    }

    OdGePoint3dArray points;
    nurb.appendSamplePoints(nurb.startParam(), nurb.endParam(), dev, points);
    m_geometryCache.m_splines2polylines.push_back(GeometryCache::Polyline(points, ext));
  }
  return eOk;
}

OdResult PdfDirectWrapper::drawGeometryForSelection(int nPage, OdGiViewportDraw* pVd, const OdDbUnderlayDrawContext& context) const
{
  if (m_path.isEmpty() || nPage < 0 || nPage > getItemsCount() || !pVd)
    return eInvalidInput;
  OdGiGeometry *pGrabber = &pVd->geometry();

  if (m_geometryCache.m_nPage != nPage)
    createGeometryCache(nPage, context.layers);

  if (m_geometryCache.m_splines.size() > 0)
  {
    double dev = pVd->deviation(kOdGiMaxDevForCurve, m_geometryCache.m_splines[0].first.controlPointAt(0));
    if ((m_Deviation == 0 || dev < m_Deviation) && cacheUnderlayGeometrySplinesAsPolylines)
    {
      m_Deviation = dev;
      optimizeSplines(pVd);
    }
  }

  bool selectionGeom = pVd->subEntityTraits().selectionGeom();
  pVd->subEntityTraits().setSelectionGeom(true);

  for (GeometryCache::Polylines::iterator i = m_geometryCache.m_polylines.begin();
    i != m_geometryCache.m_polylines.end(); ++i)
  {
    pGrabber->polyline(i->first.size(), i->first.asArrayPtr());
  }

  for (GeometryCache::Polylines::iterator i = m_geometryCache.m_splines2polylines.begin();
    i != m_geometryCache.m_splines2polylines.end(); ++i)
  {
    pGrabber->polyline(i->first.size(), i->first.asArrayPtr());
  }

  if (!cacheUnderlayGeometrySplinesAsPolylines)
  {
    for (GeometryCache::Splines::iterator i = m_geometryCache.m_splines.begin();
      i != m_geometryCache.m_splines.end(); ++i)
    {
      pGrabber->nurbs(i->first);
    }
  }

  for (GeometryCache::Ellipses::iterator i = m_geometryCache.m_ellipses.begin();
    i != m_geometryCache.m_ellipses.end(); ++i)
  {
    pGrabber->ellipArc(i->first);
  }
  pVd->subEntityTraits().setSelectionGeom(selectionGeom);
  return eOk;
}
