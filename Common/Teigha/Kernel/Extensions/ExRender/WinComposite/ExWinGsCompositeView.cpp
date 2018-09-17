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
// Composite device

#include "OdaCommon.h"
#include "ExWinGsCompositeDevice.h"
#include "ExWinGsCompositeView.h"

#if (_MSC_VER >= 1400)
#define ODGSVIEW_SUPER __super
#else
#define ODGSVIEW_SUPER ExGsCompositeView
#endif

ExWinGsCompositeView::ExWinGsCompositeView()
  : ExGsCompositeView()
{
}

ExWinGsCompositeView::~ExWinGsCompositeView()
{
}

void ExWinGsCompositeView::getViewportClipRect(OdGsDCRect &pRect)
{
  OdGePoint2d lowerLeft, upperRight;
  OdGsDCRect vRect2;
  pRect = device()->outputRect();
  getViewport(lowerLeft, upperRight);
  getViewport(vRect2);
  long nWidth = Od_abs(pRect.m_max.x - pRect.m_min.x);
  long nHeight = Od_abs(pRect.m_min.y - pRect.m_max.y);
  pRect.m_min.x = OdRoundToLong(lowerLeft.x * nWidth);
  pRect.m_min.y = nHeight - OdRoundToLong(lowerLeft.y * nHeight);
  pRect.m_max.x = OdRoundToLong(upperRight.x * nWidth);
  pRect.m_max.y = nHeight - OdRoundToLong(upperRight.y * nHeight);
  if (pRect.m_min.x > pRect.m_max.x)
    std::swap(pRect.m_min.x, pRect.m_max.x);
  if (pRect.m_min.y > pRect.m_max.y)
    std::swap(pRect.m_min.y, pRect.m_max.y);
  if (vRect2.m_min.x > vRect2.m_max.x)
    std::swap(vRect2.m_min.x, vRect2.m_max.x);
  if (vRect2.m_min.y > vRect2.m_max.y)
    std::swap(vRect2.m_min.y, vRect2.m_max.y);
  pRect.intersectWith(vRect2, false);
  std::swap(pRect.m_min.y, pRect.m_max.y);
}

void ExWinGsCompositeView::drawViewportFrame()
{
  if (isViewportBorderVisible())
  {
    bool bNrcClip = isNonRectClipped();
    ODCOLORREF color; int width;
    getViewportBorderProperties(color, width);
    if (width < 1) width = 1;
    {
      HPEN hPen = ::CreatePen(PS_SOLID, width, color), hPrevPen;
      HDC hDc = static_cast<ExWinGsCompositeDevice*>(device())->drawDc();
      hPrevPen = (HPEN)::SelectObject(hDc, (HGDIOBJ)hPen);
      if (bNrcClip)
      {
        OdIntArray nrcCounts; OdGsDCPointArray nrcVerts;
        viewportClipRegion(nrcCounts, nrcVerts);
        OdUInt32 nCnt, nCnts = nrcCounts.size();
        const int *pCnt = nrcCounts.getPtr();
        const OdGsDCPoint *pVert = nrcVerts.getPtr();
        for (nCnt = 0; nCnt < nCnts; nCnt++)
        {
          OdUInt32 nVerts = (OdUInt32)pCnt[nCnt] - 1;
          ::MoveToEx(hDc, pVert->x, pVert->y, NULL);
          do
          {
            pVert++;
            nVerts--;
            ::LineTo(hDc, pVert->x, pVert->y);
          } while (nVerts);
          pVert++;
        }
      }
      else
      {
        OdGsDCRect dcRect;
        getViewportClipRect(dcRect);
        ::MoveToEx(hDc, dcRect.m_min.x, dcRect.m_min.y, NULL);
        ::LineTo(hDc, dcRect.m_max.x, dcRect.m_min.y);
        ::LineTo(hDc, dcRect.m_max.x, dcRect.m_max.y);
        ::LineTo(hDc, dcRect.m_min.x, dcRect.m_max.y);
        ::LineTo(hDc, dcRect.m_min.x, dcRect.m_min.y);
      }
      ::SelectObject(hDc, (HGDIOBJ)hPrevPen);
      ::DeleteObject((HGDIOBJ)hPen);
    }
  }
}

// Snapshot support

void ExWinGsCompositeView::getSnapShot(OdGiRasterImagePtr &pImage, const OdGsDCRect &region)
{
  static_cast<ExWinGsCompositeDevice*>(device())->getSnapShot(pImage, region, this);
}

//
