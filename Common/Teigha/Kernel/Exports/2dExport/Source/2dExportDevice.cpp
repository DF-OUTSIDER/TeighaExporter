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

//////////////////////////////////////////////////////////////////////
// 2dExportDevice.cpp: implementation of the Od2dExportDevice, Od2dExportDevice classes.
//                    These are base classes for 2d export.
//////////////////////////////////////////////////////////////////////

#include "OdaCommon.h"
#include "2dExportDevice.h"
#include "2dSupport.h"

#include "Ge/GeCircArc3d.h"
#include "ColorMapping.h"
#include "Ge/GeEllipArc3d.h"
#include "Ge/GeEllipArc.h"
#include "Ge/GeLineSeg3d.h"
#include "OdFont.h"
#include "Ge/GeScale3d.h"
#include "OdRound.h"

#include "RxDictionary.h"
#include "RxObjectImpl.h"
#include "DbBaseHostAppServices.h"
#include "RxRasterServices.h"
#include "DynamicLinker.h"

#define STL_USING_ALGORITHM
#include "OdaSTL.h"

#if defined(ODA_WINDOWS) && !defined(_WINRT) && !defined(_WIN32_WCE)
#include "../../../Extensions/ExRender/win/GdiBmp.h"
#else 
#define drawWithGdiBmp(pDrawable, pVect, origin, u, v, bitmapWidth, bitmapHeight, rect, bMonochrome)
#endif

namespace TD_2D_EXPORT
{

//////////////////////////////////////////////////////////////////////
//  Od2dExportDevice
//////////////////////////////////////////////////////////////////////
//====================================================================
//
Od2dExportDevice::Od2dExportDevice(OdUInt32 t) 
  : m_bUseHLR(true)
  , m_type(t)
{
  ::odgsInitialize();
}

Od2dExportDevice::~Od2dExportDevice()
{
  ::odgsUninitialize();
}

double Od2dExportDevice::renderAreaMultiplier(OdUInt32 width, OdUInt32 height) const
{
  double dMax(width);
  if(height > dMax)
    dMax = height;
  double koef(1.0);
  if(dMax > 1000.0) // needn't create very DPI rasters
    koef = 1000.0 / dMax;
  return koef;
}

OdGsDevicePtr Od2dExportDevice::createRenderDevice(OdGsView* pView)
{
  OdDbBaseDatabasePEPtr pDbPE = OdDbBaseDatabasePE::cast(userGiContext()->database());
  if (pDbPE.isNull())
    return OdGsDevicePtr();
  OdDbBaseHostAppServices *pSvcs = pDbPE->appServices(userGiContext()->database());
  if (!pSvcs)
    return OdGsDevicePtr();
  return pSvcs->gsBitmapDevice(pView, userGiContext()->database(),
                               OdDbBaseHostAppServices::kFor2dExportRender |
                               ((pView->mode() == OdGsView::kHiddenLine && isUseHLR()) ? OdDbBaseHostAppServices::kFor2dExportRenderHLR : 0) |
                               (isUseHLR() ? OdDbBaseHostAppServices::kUseSoftwareHLR : 0));
}

void Od2dExportDevice::setRenderDevice(OdGsDevice* pRenderDev, bool bEnable)
{
  m_renderDevices.clear();
  m_renderDeviceIdx.clear();
  if (bEnable)
  {
    m_renderDeviceIdx.resize(numViews(), -1);
    for (int nView = 0; nView < numViews(); nView++)
    {
      OdGsView* pView = viewAt(nView);
      if (pView->mode() >= OdGsView::kHiddenLine && pView->isVisible())
      {
        RenderDeviceInfo rDev;
        if (pRenderDev)
        {
          if (m_renderDevices.isEmpty())
          {
            rDev.m_pDevice = pRenderDev;
            m_renderDevices.push_back(rDev);
          }
          m_renderDeviceIdx[nView] = 0;
        }
        else
        {
          rDev.m_pDevice = createRenderDevice(pView);
          if (!rDev.m_pDevice.isNull())
          {
            const RenderDeviceInfo *pInfo = m_renderDevices.getPtr();
            for (OdUInt32 nDev = 0; nDev < m_renderDevices.size(); nDev++)
            {
              if (pInfo->m_pDevice.get() == rDev.m_pDevice.get())
              {
                m_renderDeviceIdx[nView] = (int)nDev;
                pInfo = NULL;
                break;
              }
            }
            if (pInfo)
            {
              m_renderDevices.push_back(rDev);
              m_renderDeviceIdx[nView] = m_renderDevices.size() - 1;
            }
          }
        }
      }
    }
  }
}

inline bool isViewportInsideOutputArea(const OdGsView *pView)
{
  return static_cast<const OdGsViewImpl*>(pView)->isViewportOnScreen();
}

//====================================================================
//
void Od2dExportDevice::update(OdGsDCRect* pUpdatedRect)
{
  for (int nDevice = 0; nDevice < numRenderDevices(); nDevice++)
  {
    OdGsDevice *pRenderDevice = renderLayoutHelperAt(nDevice);
    if (!pRenderDevice) pRenderDevice = renderDeviceAt(nDevice);
    // Render device available - should process shaded viewports
    // this device could have (m_pRenderDevice->numViews() + 1) views
    // In this case additional view is paper space view, that must be drawn last
    for(int i = 0; i < pRenderDevice->numViews(); i ++)
    {
      OdGsView* pView = viewAt(i);
      OdGsView* pRenderView = pRenderDevice->viewAt(i);
      if ((i == (int)m_renderDeviceIdx.size())|| (m_renderDeviceIdx[i] == nDevice && pView->isVisible() && isViewportInsideOutputArea(pView)))
      {
        pView->hide();
        pRenderView->show();
      }
      else
      {
        pRenderView->hide();
      }
    }
  }

  for (int nDevice = 0; nDevice < numRenderDevices(); nDevice++)
  {
    OdGsDevice *pRenderDevice = renderLayoutHelperAt(nDevice);
    if (!pRenderDevice) pRenderDevice = renderDeviceAt(nDevice);

    OdUInt32 width  = Od_abs((int)(m_outputRect.m_max.x-m_outputRect.m_min.x));
    OdUInt32 height = Od_abs((int)(m_outputRect.m_max.y-m_outputRect.m_min.y));
    double koef = renderAreaMultiplier(width, height);
    OdUInt32 wholeWidth  = OdRoundToLong(width*koef);
    OdUInt32 wholeHeight = OdRoundToLong(height*koef);
    pRenderDevice->onSize(OdGsDCRect(0, wholeWidth, wholeHeight, 0));

    pRenderDevice->update(0);
    OdGiRasterImagePtr pWholeImg = pRenderDevice->properties()->getAt(OD_T("RasterImage"));

    // for debug only
    //OdRxRasterServicesPtr pRasSvcs = odrxDynamicLinker()->loadApp(RX_RASTER_SERVICES_APPNAME);
    //if (pRasSvcs.get())
    //  pRasSvcs->saveRasterImage(pWholeImg, L"c:\\owf\\whole.bmp");

    for(int i = 0; i < pRenderDevice->numViews(); i ++)
    {
      OdGsView* pRenderView = pRenderDevice->viewAt(i);
      OdGsView* pView = viewAt(i);
      if(pRenderView->isVisible())
      {
        // get client screen rect (take into account DC subdividing)
        OdGePoint3d ll, ur;
        pRenderView->getViewport((OdGePoint2d&)ll, (OdGePoint2d&)ur);
        OdGeMatrix3d xfm = pRenderView->screenMatrix();
        ll.transformBy(xfm);
        ur.transformBy(xfm);
        if(ll.x > ur.x)
          std::swap(ll.x, ur.x);
        if(ll.y > ur.y)
          std::swap(ll.y, ur.y);
        if(ll.x < 0.0)
          ll.x = 0.0;
        if(ll.y < 0.0)
          ll.y = 0.0;
        if(ur.x < 0.0)
          ur.x = 0.0;
        if(ur.y < 0.0)
          ur.y = 0.0;
        if(ll.x > wholeWidth)
          ll.x = wholeWidth;
        if(ll.y > wholeHeight)
          ll.y = wholeHeight;
        if(ur.x > wholeWidth)
          ur.x = wholeWidth;
        if(ur.y > wholeHeight)
          ur.y = wholeHeight;

        OdUInt32 pieceHeight = OdRoundToLong(ur.y-ll.y);
        OdUInt32 pieceWidth = OdRoundToLong(ur.x-ll.x);
        if(pieceHeight > 2 && pieceWidth > 2)
        {
          OdGiRasterImagePtr pSweetPiece = pWholeImg->crop(OdRoundToLong(ll.x)+1, OdRoundToLong(wholeHeight-ll.y)-pieceHeight, 
            pieceWidth-2, pieceHeight-2);
          
          // for debug only
          /* OdString name;
          name.format("c:\\owf\\piece%i.bmp", i);
          OdRxRasterServicesPtr pRasSvcs = odrxDynamicLinker()->loadApp(RX_RASTER_SERVICES_APPNAME);
          if (pRasSvcs.get())
            pRasSvcs->saveRasterImage(pSweetPiece, name);
          */
          
          OdGeExtents2d imgExts; // extents of image in DWF device coords
          pView->getViewport((OdGePoint2d&)ll, (OdGePoint2d&)ur);
          xfm = pView->screenMatrix();
          ll.transformBy(xfm);
          ur.transformBy(xfm);

          if(ll.x > ur.x)
            std::swap(ll.x, ur.x);
          if(ll.y > ur.y)
            std::swap(ll.y, ur.y);

          if(ll.x < m_outputRect.m_min.x)
            ll.x = m_outputRect.m_min.x;
          if(ll.y < m_outputRect.m_min.y)
            ll.y = m_outputRect.m_min.y;
          if(ur.x < m_outputRect.m_min.x)
            ur.x = m_outputRect.m_min.x;
          if(ur.y < m_outputRect.m_min.y)
            ur.y = m_outputRect.m_min.y;
          if(ll.x > m_outputRect.m_max.x)
            ll.x = m_outputRect.m_max.x;
          if(ll.y > m_outputRect.m_max.y)
            ll.y = m_outputRect.m_max.y;
          if(ur.x > m_outputRect.m_max.x)
            ur.x = m_outputRect.m_max.x;
          if(ur.y > m_outputRect.m_max.y)
            ur.y = m_outputRect.m_max.y;

          imgExts.addPoint((OdGePoint2d&)ll);
          imgExts.addPoint((OdGePoint2d&)ur);
          
          Od2dExportView* p2dExpView = static_cast<Od2dExportView*>(pView);
          OdIntArray nrcCounts;
          OdGsDCPointArray nrcPoints;
          p2dExpView->viewportClipRegion(nrcCounts, nrcPoints);
          bool bNrcClipped(false);
          if(nrcCounts.size())
          {
            bNrcClipped = true;
            dc_pushClip(nrcCounts.size(), nrcCounts.asArrayPtr(), nrcPoints);
          }
          bool bOutputClip(false);
          {
            // Clip by output rect
            OdGsDCPoint outMinPt(0, wholeHeight);
            OdGsDCPoint outMaxPt(wholeWidth, 0);
            OdGsDCPoint inMinPt(OdRoundToLong(ll.x), OdRoundToLong(ur.y));
            OdGsDCPoint inMaxPt(OdRoundToLong(ur.x), OdRoundToLong(ll.y));
            if (inMinPt.x < outMinPt.x || inMaxPt.x > outMaxPt.x ||
                inMaxPt.y < outMaxPt.y || inMinPt.y > outMinPt.y)
            {
              bOutputClip = true;
              const int nrcCountsOut = 4;
              OdGsDCPointArray nrcPointsOut;
              nrcPointsOut.resize(4);
              nrcPointsOut[0] = m_outputRect.m_min;
              nrcPointsOut[1].x = m_outputRect.m_max.x;
              nrcPointsOut[1].y = m_outputRect.m_min.y;
              nrcPointsOut[2] = m_outputRect.m_max;
              nrcPointsOut[3].x = m_outputRect.m_min.x;
              nrcPointsOut[3].y = m_outputRect.m_max.y;
              dc_pushClip(1, &nrcCountsOut, nrcPointsOut);
            }
          }

          dc_raster_image(pSweetPiece, imgExts);

          if(bOutputClip)
          {
            dc_popClip();
          }
          if(bNrcClipped)
          {
            dc_popClip();
          }
        }
      }
    }
  }

  OdGsBaseVectorizeDevice::update(pUpdatedRect);
}


//====================================================================
//
//
// getSupportedColorBits()
//
//  Returns the supported number of bits per pixel
//
int Od2dExportDevice::getSupportedColorBits(int nColorBits, bool ) const
{
  int nBitsPerPixel = orgBitonal;

  switch (nColorBits)
  {
  case 1:
    nBitsPerPixel = orgBitonal;
    break;

  case 4:
  case 8:
    nBitsPerPixel = orgMapped;
    break;

  case 16:
  case 24:
    nBitsPerPixel = orgRGB;
    break;

  case 32:
    nBitsPerPixel = orgBGRA;
    break;

  default:
    ODA_ASSERT(false);
    break;
  }
  return nBitsPerPixel;
}



//////////////////////////////////////////////////////////////////////
//  Od2dExportView
//////////////////////////////////////////////////////////////////////

Od2dExportView::Od2dExportView()
{
  m_pViewportClip = OdGiOrthoClipperEx::createObject();
  m_pViewportClip->input().addSourceNode(output());

  m_pHLRemover = OdGiHLRemover::createObject(); // by default it is disabled
  m_pHLRemover->input().addSourceNode(m_pViewportClip->output());

  m_pXYProjector = OdGiXYProjector::createObject();
  m_pXYProjector->input().addSourceNode(m_pHLRemover->output());

  // here we don't know if it is perspective view, so
  // xform is created in any case
  m_pPerspPrepClip = OdGiOrthoClipperEx::createObject();
  m_pPerspPrepClip->setDeviation(&m_pModelToEyeProc->eyeDeviation());
  m_pPerspXform = OdGiXform::createObject();
  m_pPerspPrepr = OdGiPerspectivePreprocessor::createObject();
  m_pPerspPrepr->setDeviation(&m_pModelToEyeProc->eyeDeviation());
}

void Od2dExportView::setView( const OdGePoint3d & position,
                              const OdGePoint3d& target,
                              const OdGeVector3d& upVector,
                              double fieldWidth,
                              double fieldHeight,
                              Projection projection )
{
  Projection prevProj = view().isPerspective() ? kPerspective : kParallel;
  OdGsBaseVectorizeView::setView(position, target, upVector, fieldWidth, fieldHeight, projection);
  if(projection == kPerspective)
  {
    m_pPerspPrepClip->input().removeSourceNode(output());
    m_pViewportClip->input().removeSourceNode(output());
    m_pHLRemover->input().removeSourceNode(m_pViewportClip->output());
    m_pXYProjector->input().removeSourceNode(m_pHLRemover->output());

    m_pPerspPrepClip->input().addSourceNode(output());
    m_pPerspPrepr->input().removeSourceNode(m_pPerspPrepClip->output());
    m_pPerspPrepr->input().addSourceNode(m_pPerspPrepClip->output());
    m_pPerspXform->input().removeSourceNode(m_pPerspPrepr->output());
    m_pPerspXform->input().addSourceNode(m_pPerspPrepr->output());
    m_pViewportClip->input().removeSourceNode(m_pPerspXform->output());
    m_pViewportClip->input().addSourceNode(m_pPerspXform->output());
    m_pHLRemover->input().addSourceNode(m_pViewportClip->output());
    m_pXYProjector->input().addSourceNode(m_pHLRemover->output());

    m_pViewportClip->setDeviation(&m_pModelToEyeProc->eyeDeviation());
    m_pPerspPrepClip->setDeviation(&m_pModelToEyeProc->eyeDeviation());
  }
  else if (prevProj != projection)
  {
    // #8806 : Previous projection is perspective, so restore conveyor for orthogonal projection
    // Detach perspective conveyor:
    // output()->perspPrepClip->perspPrepr->perspXform->viewportClip->HLRemover->XYproj
    m_pPerspPrepClip->input().removeSourceNode(output());
    m_pPerspPrepr->input().removeSourceNode(m_pPerspPrepClip->output());
    m_pPerspXform->input().removeSourceNode(m_pPerspPrepr->output());
    m_pViewportClip->input().removeSourceNode(m_pPerspXform->output());
    m_pHLRemover->input().removeSourceNode(m_pViewportClip->output());
    m_pXYProjector->input().removeSourceNode(m_pHLRemover->output());

    // Attach orthogonal conveyor:
    // output()->viewportClip->HLRemover->XYproj
    m_pViewportClip->input().addSourceNode(output());
    m_pHLRemover->input().addSourceNode(m_pViewportClip->output());
    m_pXYProjector->input().addSourceNode(m_pHLRemover->output());
  }
}

void Od2dExportDevice::circleProc(const OdGePoint3d& center,
                                  double radius,
                                  const OdGeVector3d& normal,
                                  const OdGeVector3d* pExtrusion)
{
  if ((pExtrusion && !pExtrusion->isZeroLength()) || circleArcFillMode())
  {
    OdGiGeometrySimplifier::circleProc( center, radius, normal, pExtrusion );
    return;
  }

  if (OdZero(radius))
  {
    // to transform to point at this case
    OdGePoint3d points[2];
    points[1] = points[0] = center;

    OdGeVector3d normals[2];
    normals[1] = normals[0] = normal;

    polylineProc(2, points, normals, pExtrusion, -1);
    return;
  }
  // To transform the entity firstly and then to project onto XYPlane when there is a need.

  OdGeEllipArc3d ellipse;

  OdGeVector3d tmp(OdGeVector3d::kXAxis);    
  OdGeVector3d majorAxis = tmp.transformBy( OdGeMatrix3d::planeToWorld(normal) ).normalize() * radius;
  double radiusRatio = 1.0;
  double startAngle = 0.0;
  double endAngle = Oda2PI;

  double dMajorRadius = majorAxis.length();

  ellipse.set(center, 
              majorAxis / dMajorRadius, 
              normal.crossProduct(majorAxis).normalize(),
              dMajorRadius, dMajorRadius * radiusRatio,
              OdGeEllipArc::paramFromAngle(startAngle, radiusRatio),
              OdGeEllipArc::paramFromAngle(endAngle, radiusRatio)
             );

  if(GETBIT(getDeviceType(), Od2dExportDevice::kSupport2dCircle))
  {
    dc_circle(ellipse);
  }
  else if(GETBIT(getDeviceType(), Od2dExportDevice::kSupport2dEllipse))
  {
    dc_ellipse(ellipse);
  }
  else if(GETBIT(getDeviceType(), Od2dExportDevice::kSupport2dPolyline))
  {
    OdGiGeometrySimplifier::circleProc(center, radius, normal, pExtrusion);
  }
}


//====================================================================
//
void Od2dExportDevice::circleProc(const OdGePoint3d& ptStart,
                              const OdGePoint3d& pt2,
                              const OdGePoint3d& ptEnd,
                              const OdGeVector3d* pExtrusion)
{
  OdGeCircArc3d circArc;
  OdGeError error;
  circArc.set(ptStart, pt2, ptEnd, error);

  if (error != OdGe::kOk)
  {
    OdGiGeometrySimplifier::circleProc(ptStart, pt2, ptEnd, pExtrusion);
  }
  else
  {
    circleProc(circArc.center(), circArc.radius(), circArc.normal(), pExtrusion);
  }
}

void Od2dExportDevice::circularArcProc(const OdGePoint3d& center,
                                       double radius,
                                       const OdGeVector3d& normal,
                                       const OdGeVector3d& startVector,
                                       double sweepAngle,
                                       OdGiArcType arcType,
                                       const OdGeVector3d* pExtrusion)
{
  if ((pExtrusion && !pExtrusion->isZeroLength()) || (arcType != kOdGiArcSimple && circleArcFillMode()))
  {
    OdGiGeometrySimplifier::circularArcProc(center, radius, normal, startVector, sweepAngle, arcType, pExtrusion);
    return;
  }

  if (OdZero(radius))
  {
    OdGePoint3d points[2];
    points[1] = points[0] = center;
    OdGeVector3d normals[2];
    normals[1] = normals[0] = normal;
    polylineProc(2, points, normals, pExtrusion, -1);
  }
  else if (OdZero(sweepAngle))
  {
    OdGePoint3d points[2];
    points[1] = points[0] = (center + startVector * radius);
    OdGeVector3d normals[2];
    normals[1] = normals[0] = normal;
    polylineProc(2, points, normals, pExtrusion, -1);
  }
  else
  {
    OdGeVector3d majorAxis = startVector.normal() * radius;
    double radiusRatio = 1.0;
    double dMajorRadius = majorAxis.length();
    OdGeEllipArc3d ellipse(center, 
                           majorAxis / dMajorRadius, 
                           normal.crossProduct(majorAxis).normalize(),
                           dMajorRadius, dMajorRadius * radiusRatio,
                           0,
                           OdGeEllipArc::paramFromAngle(sweepAngle, radiusRatio));
    putEllipse(ellipse);
  }
}

//====================================================================
//
void Od2dExportDevice::circularArcProc(const OdGePoint3d& start,
                                       const OdGePoint3d& point,
                                       const OdGePoint3d& end,
                                       OdGiArcType arcType,
                                       const OdGeVector3d* pExtrusion)
{
  OdGeCircArc3d circArc;
  OdGeError error;
  circArc.set(start, point, end, error);

  if (error != OdGe::kOk)
  {
    OdGiGeometrySimplifier::circularArcProc(start, point, end, arcType, pExtrusion);
  }
  else
  {
    OdGeVector3d startVec(circArc.refVec().rotateBy(circArc.startAng(), circArc.normal()));
    circularArcProc( circArc.center(), circArc.radius(), circArc.normal(), startVec,
                     circArc.endAng() - circArc.startAng(), arcType, pExtrusion );
  }
}

// AlexR : 3194,3195
bool testCorrectAxis(const OdGeEllipArc3d &ellipArc, OdGeTol dCorrectAxisTol)
{
  OdGeEllipArc3d tmpArc(ellipArc);
  OdGeExtents3d ext1, ext2;
  tmpArc.getGeomExtents(ext1);
  OdGe::ErrorCondition ret;
  tmpArc.orthogonolizeAxes( dCorrectAxisTol, ret );
  if (ret == OdGe::kOk)
  {
    tmpArc.getGeomExtents(ext2);

    double d1, d2;
    double dDiff = ext1.maxPoint().distanceTo(ext1.minPoint()) * 0.1; // 10% differences

    bool b1, b2, b3, b4;
  
    d1 = ext1.minPoint().x;
    d2 = ext2.minPoint().x;
    b1 = OdEqual(d1, d2, dDiff);

    d1 = ext1.minPoint().y;
    d2 = ext2.minPoint().y;
    b2 = OdEqual(d1, d2, dDiff);

    d1 = ext1.maxPoint().x;
    d2 = ext2.maxPoint().x;
    b3 = OdEqual(d1, d2, dDiff);

    d1 = ext1.maxPoint().y;
    d2 = ext2.maxPoint().y;
    b4 = OdEqual(d1, d2, dDiff);

    return (b1 && b2 && b3 && b4);
  }
  return true; // cannot test this way
}

//====================================================================
//
// MKU 28.10.2004   Regular dwf export recovery after after changes in Ge.
//                   It's necessary to call normalize() after getting vector now
//
void Od2dExportDevice::ellipArcProc(const OdGeEllipArc3d& arc,
                                    const OdGePoint3d* pEndPointsOverrides,
                                    OdGiArcType arcType,
                                    const OdGeVector3d* pExtrusion)
{
  if ((pExtrusion && !pExtrusion->isZeroLength()) || (arcType != kOdGiArcSimple && circleArcFillMode()) /*||
      (OdZero(arc.majorRadius()) || OdZero(arc.minorRadius()))*/)
  {
    OdGiGeometrySimplifier::ellipArcProc(arc, pEndPointsOverrides, arcType, pExtrusion);
    return;
  }

  OdGeEllipArc3d ellipArc(arc);

  OdGe::ErrorCondition ret;
  ret = OdGe::kOk;

  OdGeVector3d normal;
  try
  {
    normal = ellipArc.normal();
    normal.normalize(OdGeContext::gTol, ret);
  }
  catch (...)
  {
    OdGiGeometrySimplifier::ellipArcProc(arc, pEndPointsOverrides, arcType, pExtrusion);
    return;
  }

  double dRadius = (ellipArc.majorRadius() > ellipArc.minorRadius()) ? ellipArc.majorRadius() : ellipArc.minorRadius();

  if (OdZero(ellipArc.majorRadius()) || OdZero(ellipArc.minorRadius()))
  {
    OdGePoint3d points[2];
    if (OdZero(dRadius)) //degenerated to point
    {
      points[1] = points[0] = ellipArc.center();
    }
    else //degenerated to line
    {
      //we can't just use the extents so we have to do it the other way
      //find sample points (see OdGiGeometrySimplifier::ellipArcProc)
      OdGePoint3dArray arcPoints;
      OdGeInterval interval;
      ellipArc.getInterval(interval);
      ellipArc.appendSamplePoints(interval.lowerBound(), interval.upperBound(), deviation(kOdGiMaxDevForCurve, ellipArc.center()), arcPoints);
      if (pEndPointsOverrides)
      {
        arcPoints.first() = pEndPointsOverrides[0];
        arcPoints.last() = pEndPointsOverrides[1];
      }
      //find min and max points
      OdUInt32 size = arcPoints.size();
      OdGeExtents3d ext;
      for (OdUInt32 i = 0; i < size; i++)
        ext.addPoint(arcPoints[i]);
      if (arcPoints.contains(ext.minPoint()) && arcPoints.contains(ext.maxPoint())) //all right - just put them to device
      {
        points[0] = ext.minPoint();
        points[1] = ext.maxPoint();
      } 
      else //arc has other direction
      {
        points[0] = OdGePoint3d(ext.minPoint().x, ext.maxPoint().y, 0.); //z-coord is always 0 for 2d device
        points[1] = OdGePoint3d(ext.maxPoint().x, ext.minPoint().y, 0.);
        ODA_ASSERT(arcPoints.contains(points[0]) && arcPoints.contains(arcPoints[1])); //if there are no these points, then something wrong (but I'm sure them have to be there)
      }
    }
    
    OdGeVector3d normals[2];
    if (ret == OdGe::kOk)
      normals[1] = normals[0] = normal; //anyway polylineProc doesn't use them

    polylineProc(2, points, (ret == OdGe::kOk) ? normals : NULL, pExtrusion, -1);
    return;
  }

  if(ret == OdGe::kOk)
  {
    if(ellipArc.isCircular(/*OdGeTol(1.0e-6)*/))
    {
      OdGeCircArc3d circle( ellipArc.center(),
                            normal,
                            ellipArc.majorAxis(),
                            dRadius,
                            ellipArc.startAng(), 
                            ellipArc.endAng()
                          );

      if (circle.isClosed())
      {
        circleProc( circle.center(), circle.radius(), normal/*circle.normal()*/, pExtrusion );
      }
      else
      {
        circularArcProc( ellipArc.center(), 
                         dRadius, 
                         normal,
                         ellipArc.startPoint() - ellipArc.center(),
                                         ellipArc.endAng() - ellipArc.startAng(), 
                                         arcType, pExtrusion ); 
      }
    }
    else
    {
      OdGeTol dCorrectAxisTol(OdGeContext::gTol);

      if (!testCorrectAxis(ellipArc, dCorrectAxisTol)) // AlexR : test for 3195, 3194, 5123
      {
        OdGiGeometrySimplifier::ellipArcProc( arc, pEndPointsOverrides, arcType, pExtrusion );
        return;
      }

      OdGe::ErrorCondition ret;
      ellipArc.orthogonolizeAxes( dCorrectAxisTol, ret );

      OdGeError flag1, flag2;
      //OdGeVector3d vMajor = ellipArc.majorAxis( OdGeContext::gTol, flag1 );
      //OdGeVector3d vMinor = ellipArc.minorAxis( OdGeContext::gTol, flag2 );
      OdGeVector3d vMajor = ellipArc.majorAxis();
      OdGeVector3d vMinor = ellipArc.minorAxis();
      vMajor.normalize( OdGeContext::gTol, flag1 );
      vMinor.normalize( OdGeContext::gTol, flag2 );

      if ( ret != OdGe::kOk || flag1 != OdGe::kOk || flag2 != OdGe::kOk)
      {
        OdGiGeometrySimplifier::ellipArcProc( arc, pEndPointsOverrides, arcType, pExtrusion );
      }
      else
      {

        OdGe::ErrorCondition ret;
        //OdGeVector3d normalArc = ellipArc.normal(OdGeContext::gTol, ret);     // 1703 // MKU 05.03.2004
        OdGeVector3d normalArc = ellipArc.normal();     // 1703 // MKU 05.03.2004
        normalArc.normalize( OdGeContext::gTol, ret );
        if (ret != OdGe::kOk)
        {
          OdGiGeometrySimplifier::ellipArcProc( arc, pEndPointsOverrides, arcType, pExtrusion );
          return;
        }
        else
        {
          // Everything is correct. Finally outputting the ellipse.
          putEllipse(ellipArc);
        }
      }
    }
  }
  else
  {
    OdGiGeometrySimplifier::ellipArcProc( ellipArc,
                                          pEndPointsOverrides,
                                          arcType,
                                          pExtrusion );
  }
}

//
// putEllipse
//
void Od2dExportDevice::putEllipse(OdGeEllipArc3d& ellipse)
{
  // To transform the entity firstly and then to project onto XYPlane when there is a need.

  // Check Coo system and reverse it if there is a need
  if(ellipse.normal().z < 0)
  {
    ellipse.reverseParam();
  }

  if(GETBIT(getDeviceType(), Od2dExportDevice::kSupport2dEllipse))
  {
    dc_ellipse(ellipse);
  }
  else if(GETBIT(getDeviceType(), Od2dExportDevice::kSupport2dPolyline))
  {
    OdGiGeometrySimplifier::ellipArcProc(ellipse);
  }
}

void Od2dExportDevice::triangleOut(const OdInt32* vertices, const OdGeVector3d* )
{
  const OdGiVertexData* vd = vertexData();
  if (vd && vd->trueColors())
  {
    OdInt32 face[4];
    face[0] = 3;
    for(OdInt32 j = 1; j < 4; j++)
      face[j] = vertices[j-1];
    dc_gouraud_polytriangle(vertexDataList(), 4, face, vd->trueColors());
  }
  else
  {
    OdGePoint2d points2d[3];
    for(OdInt32 j = 0; j < 3; j++)
      points2d[j] = OdGePoint2d(*(OdGePoint2d*)(vertexDataList() + vertices[j]));

    dc_polygon(3, points2d);
  }
}

void Od2dExportDevice::polylineOut(OdInt32 nPoints, const OdGePoint3d* pVertexList)
{
  OdGePoint2dArray points2d;
  points2d.resize(nPoints);

  for(OdInt32 j = 0; j < nPoints; j++)
    points2d[j] = OdGePoint2d(pVertexList[j].x, pVertexList[j].y);
 
  if(nPoints)
    dc_polyline(nPoints, points2d.asArrayPtr());
}

//====================================================================
//
void Od2dExportDevice::polygonProc(OdInt32 nPoints,
                                   const OdGePoint3d* pVertexList,
                                   const OdGeVector3d* pNormal,
                                   const OdGeVector3d* pExtrusion)
{
  if(pExtrusion && !pExtrusion->isZeroLength())
  {
    OdGiGeometrySimplifier::polygonProc(nPoints, pVertexList, NULL, pExtrusion);
    return;
  }

  if(fillMode())
  {
    if (GETBIT(m_effTraits.drawFlags(), OdGiSubEntityTraits::kDrawContourFill) &&
        !GETBIT(getDeviceType(), Od2dExportDevice::kSupportContourFill) && drawContext())
    {
      OdGePoint2dArray points;
      points.resize(nPoints + 1);
      for(OdInt32 j = 0; j < nPoints; j++)
        points[j] = OdGePoint2d(pVertexList[j].x, pVertexList[j].y);
      points[nPoints] = points.first();
      OdGiSubEntityTraits &traits = drawContext()->subEntityTraits();
      OdCmEntityColor prevColor = traits.trueColor();
      traits.setTrueColor(traits.secondaryTrueColor());
      drawContext()->onTraitsModified();
      dc_polygon(nPoints, points.getPtr());
      traits.setTrueColor(prevColor);
      drawContext()->onTraitsModified();
      dc_polyline(nPoints + 1, points.getPtr());
    }
    else
    {
      OdGePoint2dArray points2d;
      points2d.resize(nPoints);

      for(OdInt32 j = 0; j < nPoints; j++)
        points2d[j] = OdGePoint2d(pVertexList[j].x, pVertexList[j].y);

      dc_polygon(nPoints, points2d.asArrayPtr());
    }
  }
  else
  {
    // draw closed polyline
    OdGePoint3dArray points(nPoints + 1);
    points.insert(points.end(), pVertexList, pVertexList + nPoints);
    points.push_back(pVertexList[0]);
    polylineProc(++nPoints, points.getPtr(), pNormal);
  }
}


void Od2dExportDevice::textProc(const OdGePoint3d& position,
                                const OdGeVector3d& u, // normal
                                const OdGeVector3d& v,  // direction
                                const OdChar* msg,
                                OdInt32 length,
                                bool raw,                       // informing whether to interpret escape codes
                                const OdGiTextStyle* pStyle,
                                const OdGeVector3d* pExtrusion)
{
  if(length == 0)  
    return;               // Text must be not empty

  if(!pStyle->isTtfFont())
  {
    // for SHX font
    //
    ODA_ASSERT(pStyle->isShxFont());

    if ( pExtrusion && !pExtrusion->isZeroLength() )
    {
      OdGiGeometrySimplifier::textProc( position, u, v, msg, length, raw, pStyle, pExtrusion );
      return;
    }
    else
    {
      shxText(position, u, v, msg, length, raw, pStyle, pExtrusion);
    }
  }
  else
  {
    if (u.isZeroLength() || v.isZeroLength()) // check via default tolerance // #6094
      return;

    // for True Type Font only

    // Prepare Font
    //
    bool bBold;
    bool bItalic;
    int  nCharset;
    int  nPitchAndFamily;
    pStyle->ttfdescriptor().getTtfFlags(bBold, bItalic, nCharset, nPitchAndFamily);


    OdGeVector2d yDir;
    yDir = (const OdGeVector2d&)v;

    // To set font

    OdTtfDescriptor desc;
    pStyle->getFont()->getDescriptor(desc);
    Od2dFont odFont;
    odFont.m_font_name = desc.typeface();
    if ( odFont.m_font_name.isEmpty() )
      odFont.m_font_name = desc.fileName();
    odFont.m_charset = nCharset;
    odFont.m_pitch   = desc.getPitch();
    odFont.m_family  = desc.getFamily();

    odFont.m_bold = bBold;
    odFont.m_italic = bItalic;
    odFont.m_underline = pStyle->isUnderlined();

                                        // describes how the entire string is rotated from horizontal
    double dRotation = OdGeVector3d::kXAxis.angleTo( u, OdGeVector3d::kZAxis);
    odFont.m_rotation = OdaToDegree(dRotation);
    odFont.m_width_scale = u.length()/v.length(); //pStyle->xScale();
                                        // The desired rotation of the individual characters within the font
    odFont.m_oblique = OdaPI2 - u.angleTo(v, OdGeVector3d::kZAxis);

    odFont.m_height = v.length();

    if (pStyle->isVertical())
    {
      odFont.m_flags |= Od2dFont::VERTICAL;
    }
    if (pStyle->isUpsideDown())
    {
      odFont.m_flags |= Od2dFont::MIRROR_Y;
    }
    if (pStyle->isBackward())
    {
      odFont.m_flags |= Od2dFont::MIRROR_X;
    }
    if (pStyle->isUnderlined())
    {
      odFont.m_flags |= Od2dFont::UNDERSCORE;
    }
    if (pStyle->isOverlined())
    {
      odFont.m_flags |= Od2dFont::OVERSCORE;
    }

    {
      bool bASCII(true);

      bool bAlwaysUnicode = GETBIT(getDeviceType(), Od2dExportDevice::kUnicodeOnly);
      if ( bAlwaysUnicode )
        bASCII = false;

      OdString sProcessedText;
      OdArray<OdUInt16>    arrayUnicode;
      if (!bASCII || !textProcessing( position, u, v, msg, length, raw, pStyle, sProcessedText,
                           /*giEntryPoint, pFontXform,*/ arrayUnicode, /*arrayOverlines, arrayUnderlines,*/
                           true)
                         )
      {
        bASCII = false;
        arrayUnicode.clear();

        textProcessing( position, u, v, msg, length, raw, pStyle, sProcessedText,
                        /*giEntryPoint, pFontXform,*/ arrayUnicode, /*arrayOverlines, arrayUnderlines,*/
                        false);
      }

      Od2dExportView* p2dExpView = static_cast<Od2dExportView*>(viewAt(0));
      p2dExpView->setText( position, u, v, 
               (bASCII) ? sProcessedText.c_str() : msg, 
               (bASCII) ? sProcessedText.getLength() : length, 
               true, pStyle, true, &odFont, 
               (bASCII) ? NULL : &arrayUnicode
             );
    } 
  }
}

bool Od2dExportDevice::textProcessing(const OdGePoint3d& position,
                                      const OdGeVector3d& u,
                                      const OdGeVector3d& v,
                                      const OdChar* msg,
                                      OdInt32 length,
                                      bool raw,
                                      const OdGiTextStyle* pStyle,
                                      OdString& sProcessedText,
                                      OdArray<OdUInt16>& arrayUnicode,
                                      bool bASCII,
                                      OdBoolArray *isInBigFont,
                                      OdGePoint2dArray *pAdvances
                                      )
{
  OdFont* pDrawFont = pStyle->getFont();

  OdGiConveyorEntryPoint  giEntryPoint;
  OdGiXformPtr            pFontXform;
  pFontXform = OdGiXform::createObject();

  pFontXform->input().addSourceNode( giEntryPoint );
  pFontXform->output().setDestGeometry( *this );

  OdGeMatrix3d charXForm;
  OdGePoint2d advance;
  charXForm.setCoordSystem(position, u, v, u.crossProduct( v ));

  OdGeScale3d scale(1. / pDrawFont->fontAbove());
  charXForm.setToProduct(OdGeMatrix3d::scaling( scale, position), charXForm );

  OdDbBaseDatabase* pDb = drawContext()->giContext().database();
  OdBaseTextIteratorPtr itText = OdDbBaseDatabasePEPtr(pDb)->createTextIterator( pDb, msg, length, raw, pStyle );
  OdCharacterProperties chProperties;
  OdTextProperties textFlags;
  //bool bOverlined(false);
  //bool bUnderlined(false);

  textFlags.setVerticalText(pStyle->isVertical());
  textFlags.setTrackingPercent(pStyle->trackingPercent());
  textFlags.setIncludePenups(false);

  OdGePoint3d pointsUnder[2];
  OdGePoint3d pointsOver[2];

  bool bUnicode(false);
  for( bool bLast = false; !bLast && !bUnicode; bLast = itText->currProperties().bLastChar )
  {
    OdUInt16 ch = itText->nextChar();
    if ( ch == 0 ) break;

    if (isInBigFont)
    {
      isInBigFont->push_back( itText->currProperties().bInBigFont );
    }

    if (bASCII)
    { 
      // ASCII case
      if ( ch < 128 ) 
      {
        sProcessedText += (char)ch;
      }
      else 
      {
        bUnicode = true;
        break;
      }
    } 
    else 
    { 
      // Unicode case
      arrayUnicode.push_back( ch );
    }

    if (!raw)
    {
      chProperties = itText->currProperties();

      textFlags.setUnderlined( chProperties.bUnderlined );
      textFlags.setOverlined( chProperties.bOverlined );

      pFontXform->setTransform(charXForm);

      pDrawFont->getScore( ch, advance, pointsOver, pointsUnder, textFlags );

      if (textFlags.isOverlined()) 
      {
        (&giEntryPoint.geometry())->polylineProc(2, pointsOver);
      }
      if (textFlags.isUnderlined())
      {
        (&giEntryPoint.geometry())->polylineProc(2, pointsUnder);
      }
    }
    else if (pAdvances)
    {
      pFontXform->setTransform(charXForm);
      pDrawFont->getScore( ch, advance, pointsOver, pointsUnder, textFlags );
    }

    /*
    if (textFlags.isOverlined()) 
    {
      if (!bOverlined)
      {
        // Starts the overline; sets the start and the end points
        arrayOverlines.push_back(pointsOver[0]);
        arrayOverlines.push_back(pointsOver[1]);
      }
      else
      {
        // Coninues the overline; changes the end overline point
        arrayOverlines.removeLast();
        arrayOverlines.push_back(pointsOver[1]);
      }
      bOverlined = true;
    }
    else
    {
      bOverlined = false;
    }
    if (textFlags.isUnderlined())
    {
      if (!bUnderlined)
      {
        // Starts the Underline; sets the start and the end points
        arrayUnderlines.push_back(pointsUnder[0]);
        arrayUnderlines.push_back(pointsUnder[1]);
      }
      else
      {
        // Coninues the Underline; changes the end Underline point
        arrayUnderlines.removeLast();
        arrayUnderlines.push_back(pointsUnder[1]);
      }
      bUnderlined = true;
    }
    else
    {
      bUnderlined = false;
    }
    */

    if (pAdvances)
    {
      pAdvances->append()->set(charXForm.translation().x, charXForm.translation().y);
    }

    charXForm.setToProduct(charXForm, OdGeMatrix3d::translation(OdGeVector3d(advance.x, advance.y, 0.)));
  }

  return !bUnicode;
}

//====================================================================
//
void Od2dExportDevice::xlineProc(const OdGePoint3d& pt1, const OdGePoint3d& pt2)
{
  OdGePoint3d ptsLine[2];

  ptsLine[0] = pt1;
  ptsLine[1] = pt2;
  polylineProc( 2, ptsLine );
}

//====================================================================
//
void Od2dExportDevice::rayProc(const OdGePoint3d& pt1, const OdGePoint3d& pt2)
{
  OdGePoint3d ptsLine[2];

  ptsLine[0] = pt1;
  ptsLine[1] = pt2;
  polylineProc( 2, ptsLine );
}


//====================================================================
// OLE support

static void exOwnerDrawDc(const OdGePoint3d& _origin,
                          const OdGeVector3d& _u,
                          const OdGeVector3d& _v,
                          const OdGiSelfGdiDrawable* pDrawable,
                          bool /*bDcAligned*/,
                          bool /*bAllowClipping*/,
                          OdGsBaseVectorizer& view)
{
  OdGeMatrix3d x = view.getModelToEyeTransform();

  OdGePoint3d origin(x * _origin);
  OdGeVector3d u(x * _u), v(x * _v);

  OdGeExtents3d ext(origin, origin);
  ext.addPoint(origin + u);
  ext.addPoint(origin + v);
  ext.addPoint(origin + u + v);

  OdGePoint3d imgMin = ext.minPoint();
  OdGePoint3d imgMax = ext.maxPoint();

  OdGePoint3d origin2;
  OdGeVector3d u2;
  OdGeVector3d v2;
  origin2 = imgMin;
  v2 = u2 = imgMax - imgMin;
  u2.z = u2.y =0.;
  v2.z = v2.x =0.;

  x = view.eyeToOutputTransform();
  origin2.transformBy(x);
  u2.transformBy(x);
  v2.transformBy(x);

  // to increase DPI of output
  //double dCoef = 1.;
  //u2 *= dCoef;
  //v2 *= dCoef;

  OdUInt64 rasterWidth, rasterHeight;
  try
  {
    rasterWidth = OdRoundToLong(fabs(u2.x));
    rasterHeight = OdRoundToLong(fabs(v2.y));
    if (rasterWidth * rasterHeight > 48*1024*1024) // CreateDibBitmap limitation (huge rasters are scaled anyway)
    {
      double scale = sqrt(( ((double)(rasterWidth) / 1024.) * ((double)(rasterHeight) / 1024.) ) / 48.);
      if (OdLessOrEqual(scale, 1.))
        scale = 1.1;
      while (rasterWidth * rasterHeight > 48 * 1024 * 1024)
      {
        rasterWidth = OdRoundToLong((double)rasterWidth / scale);
        rasterHeight = OdRoundToLong((double)rasterHeight / scale);
      }
    }
  }
  catch (const OdError&)
  {
    // #10225 : Can't render if arithmetic overflow thrown
    return;
  }

  //u2 /= dCoef;
  //v2 /= dCoef;

  OdGsDCRect rect; // RECT 
  rect.m_min.x = 0; // left
  rect.m_min.y = rasterHeight; // top
  rect.m_max.x = rasterWidth; // right
  rect.m_max.y = 0; // bottom 

  OdSmartPtr<OdGiSelfGiDrawablePE> pOdGiSelfGiDrawablePE = OdGiSelfGiDrawablePE::cast(pDrawable);
  if (   pOdGiSelfGiDrawablePE.get() 
      && pOdGiSelfGiDrawablePE->draw(const_cast<OdGiSelfGdiDrawable*>(pDrawable), 
                                     &view.output().destGeometry(), origin2, u2, v2, rect, true))
    return;
  drawWithGdiBmp(pDrawable, &view, origin2, u2, v2, rasterWidth, rasterHeight, rect, false);
}

void Od2dExportView::ownerDrawDc(
    const OdGePoint3d& origin,
    const OdGeVector3d& u,
    const OdGeVector3d& v,
    const OdGiSelfGdiDrawable* pDrawable,
    bool bDcAligned,
    bool bAllowClipping)
{
  if(mode() != OdGsView::k2DOptimized)
    return;
  exOwnerDrawDc(origin, u, v, pDrawable, bDcAligned, bAllowClipping, *this);
}

//====================================================================
//
void Od2dExportView::onTraitsModified()
{
  OdGsBaseVectorizer::onTraitsModified();
  device()->onTraitsModified(effectiveTraits());
}

void Od2dExportDevice::onTraitsModified(const OdGiSubEntityTraitsData& traits)
{
  m_effTraits = traits;
}


//====================================================================
//
bool Od2dExportView::regenAbort() const
{
  // return true here to abort the rendering process
  return false;  
}

//====================================================================
//
void Od2dExportView::beginViewVectorization()
{
  OdGsBaseVectorizer::beginViewVectorization();
  device()->setDrawContext(drawContext());
  device()->setupSimplifier( &m_pModelToEyeProc->eyeDeviation() );
  setDrawContextFlags(kSpatialFilterSimplText | kEmbranchmentSimplText, false);
}

//====================================================================
//
void Od2dExportView::endViewVectorization()
{
  OdGsBaseVectorizer::endViewVectorization();
#if !defined(_WIN32_WCE) || (_WIN32_WCE >= 0x500)
  if (!m_pHLRemover.isNull() && isVisible() && !std::uncaught_exception())
#else
  if (!m_pHLRemover.isNull() && isVisible())
#endif
    m_pHLRemover->process(); // compute hidden lines in case HLR enabled
  device()->setDrawContext(0);
}

void Od2dExportView::loadViewport()
{
  OdGsDCPoint min, max;
  screenRect(min, max);
  OdGeMatrix3d xToDc = eyeToScreenMatrix();

  m_pHLRemover->setDrawContext(OdGsBaseVectorizer::drawContext());

  if(mode() == kHiddenLine && device()->isUseHLR())
  {
    m_pHLRemover->enable(true);
    m_pHLRemover->invertZ(true);
  }
  else
  {
    // only k2DOptimized mode supports plot style tables
    m_renderMode = k2DOptimized/*kWireframe*/;
    m_regenerationType = kOdGiStandardDisplay;
    m_pHLRemover->enable(false);
  }

  const OdGePoint2d pts[4] = { OdGePoint2d(min.x, min.y), 
    OdGePoint2d((double)max.x, (double)min.y),
    OdGePoint2d(max.x, max.y),
    OdGePoint2d((double)min.x, (double)max.y) };
  const int ptsCounts = 4;

  if(!view().isPerspective())
  {
    OdGeMatrix3d xToDc = eyeToScreenMatrix();

    setEyeToOutputTransform(xToDc);

    OdGeVector3d zAxis = OdGeVector3d::kZAxis;
    OdGeVector3d dcFront(0.,0.,frontClip());
    OdGeVector3d dcBack(0.,0.,backClip());
    dcFront.transformBy(xToDc);
    dcBack.transformBy(xToDc);
    zAxis.transformBy(xToDc);

    if(zAxis.z > 0.)
    {
      m_pViewportClip->set(ptsCounts, pts, isBackClipped(), dcBack.z, isFrontClipped(), dcFront.z);
    }
    else
    {
      m_pViewportClip->set(ptsCounts, pts, isFrontClipped(), dcFront.z, isBackClipped(), dcBack.z);
    }
    m_pXYProjector->enable();
  }
  else
  {
    OdGeMatrix3d xToDc = screenMatrix() * projectionMatrix();
    setEyeToOutputTransform(OdGeMatrix3d::kIdentity);

    bool isFrontClp(isFrontClipped());
    bool isBackClp(isBackClipped());

    double frClp(frontClip());
    double bkClp(backClip());

    if(isFrontClp)
      frClp = odmin(frClp, focalLength() * 0.9);
    else
      frClp = focalLength() * 0.9;

    m_pPerspPrepClip->set(0, 0, isBackClp, bkClp, true, frClp);
    m_pViewportClip->pushClipStage(1, &ptsCounts, 4, pts);

    m_pXYProjector->enable();
    m_pPerspPrepClip->setDrawContext(OdGiBaseVectorizer::drawContext());

    m_pPerspPrepr->setDrawContext(OdGiBaseVectorizer::drawContext());
    m_pXYProjector->output().setDestGeometry(*device());
    m_pPerspXform->setTransform(xToDc);  
  }
  m_pPerspPrepClip->setDeviation(&m_pModelToEyeProc->eyeDeviation());

  m_pViewportClip->setDrawContext(OdGiBaseVectorizer::drawContext());
  m_pViewportClip->setDeviation(&m_pModelToEyeProc->eyeDeviation());
}

void Od2dExportView::update()
{
  if(abs((int)(dcScreenMax().x-dcScreenMin().x)) > 0 && abs((int)(dcScreenMax().y-dcScreenMin().y)) > 0)
  {
    OdGsBaseVectorizeView::update();
  }
}

//====================================================================
//
bool Od2dExportView::isTransparency(OdGeVector3d screenU, OdGeVector3d screenV)
{
  return ((screenU.isParallelTo(OdGeVector3d::kXAxis) 
           && screenV.isParallelTo(OdGeVector3d::kYAxis))
                        ? false
                        : true
         );
}

// Default implementations
void Od2dExportDevice::dc_polyline(OdUInt32, const OdGePoint2d*)
{
  throw OdError(eNotImplemented); // If called, then must be implemented in child class
}

void Od2dExportDevice::dc_polygon(OdUInt32, const OdGePoint2d*)
{
  throw OdError(eNotImplemented); // If called, then must be implemented in child class
}

void Od2dExportDevice::dc_circle(const OdGeEllipArc3d&)
{
  throw OdError(eNotImplemented); // If called, then must be implemented in child class
}

void Od2dExportDevice::dc_ellipse(const OdGeEllipArc3d&)
{
  throw OdError(eNotImplemented); // If called, then must be implemented in child class
}

void Od2dExportDevice::dc_gouraud_polytriangle(const OdGePoint3d*,
                                               OdInt32,
                                               const OdInt32*,
                                               const OdCmEntityColor*)
{
  throw OdError(eNotImplemented); // If called, then must be implemented in child class
}

void Od2dExportDevice::dc_raster_image(OdGiRasterImagePtr, const OdGeExtents2d&)
{
  throw OdError(eNotImplemented); // If called, then must be implemented in child class
}

void Od2dExportDevice::dc_color(ODCOLORREF)
{
  throw OdError(eNotImplemented); // If called, then must be implemented in child class
}

void Od2dExportDevice::dc_layer(int, const OdString&)
{
  throw OdError(eNotImplemented); // If called, then must be implemented in child class
}

void Od2dExportDevice::dc_pushClip(int, const int*, const OdGsDCPointArray&)
{
  throw OdError(eNotImplemented); // If called, then must be implemented in child class
}

void Od2dExportDevice::dc_popClip()
{
  throw OdError(eNotImplemented); // If called, then must be implemented in child class
}

} // TD_2D_EXPORT namespace
