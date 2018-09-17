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
#include <tchar.h>
#include "ExGiGDIGeometry.h"
#include "../ExGsHelpers.h"

#include "Int32Array.h"
#include "UInt8Array.h"
#include "OdRound.h"
#include "RxObjectImpl.h"

#include "Ge/GeExtents2d.h"
#include "Ge/GeMatrix2d.h"
#include "Ge/GeExtents3d.h"

#include "Gi/GiTextStyle.h"
#include "Gi/GiShellFaceIterator.h"
#include "Gi/GiMetafile.h"
#include "Gi/GiViewportGeometry.h"
#include "Gi/GiRasterWrappers.h"
#include "Gi/GiSubEntityTraitsData.h"

#include "RxDictionary.h"
#include "OdFontServices.h"

#define ROUND(n) OdRoundToLong(n)

#include "ExGsGDIVectorizeDevice.h"

#include "../ExGsGradientRender.h"

#ifdef _WIN32_WCE
#define DD_IGNORE_WCEALT_NEW
#include "afxwin.h"

#if (_WIN32_WCE < 0x500 && _MSC_VER < 1300)
#include "wcealt.h"
#define CreateBrushIndirect wce_CreateBrushIndirect
#endif

void ExGiGDIGeometry::triangleOut(const OdInt32* pTriangleVertices, const OdGeVector3d* pNormal)
{
  m_trianglePts[0] = vertexDataList()[pTriangleVertices[0]];
  m_trianglePts[1] = vertexDataList()[pTriangleVertices[1]];
  m_trianglePts[2] = vertexDataList()[pTriangleVertices[2]];
  polygonOut(3, m_trianglePts, pNormal);
}

#else //#ifdef _WIN32_WCE

#define OD_ENABLE_POSTSCRIPT_PRINTING
#define OD_ENABLE_POSTSCRIPT_LEVEL3

typedef BOOL (CALLBACK*GRADFILLPROC)(HDC, PTRIVERTEX, ULONG, PVOID, ULONG, ULONG);
typedef BOOL (CALLBACK*ALPHABLENDPROC)(HDC, int, int, int, int, HDC, int, int, int, int, BLENDFUNCTION);
typedef COLORREF (CALLBACK*DCBRUSHCLRPROC)(HDC, COLORREF);

#ifndef DC_BRUSH
#define DC_BRUSH 18
#endif

bool ExGiGDIGeometry::beginGradientFill(OdInt32 nbVertex,
                                        const OdGePoint3d* pVertexList,
                                        const OdGiVertexData* pVertexData)
{
  if(m_grFillPrAddr)
  {
    m_triangleFn = &ExGiGDIGeometry::gradientTriangleOut;
    m_triangles.clear();
    m_triangles.reserve(nbVertex);

    const OdCmEntityColor* pVertColors = pVertexData->trueColors();
    m_triVertexes.resize(nbVertex);
    for(OdInt32 i = 0; i < nbVertex; i ++)
    {
      m_triVertexes[i].x = OdTruncateToLong(pVertexList[i].x);
      m_triVertexes[i].y = OdTruncateToLong(pVertexList[i].y);

      if (!m_bGlobalAlpha)
      {
        m_triVertexes[i].Red   = (unsigned short)(pVertColors[i].red() << 8);
        m_triVertexes[i].Green = (unsigned short)(pVertColors[i].green() << 8);
        m_triVertexes[i].Blue  = (unsigned short)(pVertColors[i].blue() << 8);
      }
      else
      {
        ODCOLORREF merged = OdMergeRGBAlpha(m_clrFadeColor, ODTOCOLORREF(pVertColors[i]), (OdUInt8)m_nGlobalAlpha);
        m_triVertexes[i].Red   = (unsigned short)(ODGETRED(merged) << 8);
        m_triVertexes[i].Green = (unsigned short)(ODGETGREEN(merged) << 8);
        m_triVertexes[i].Blue  = (unsigned short)(ODGETBLUE(merged) << 8);
      }
      m_triVertexes[i].Alpha = 0;
    }
    return true;
  }
  return false;
}

void ExGiGDIGeometry::triangleOut(const OdInt32* pTriangleVertices, const OdGeVector3d* pNormal)
{
  (this->*m_triangleFn)(pTriangleVertices, pNormal);
}

void ExGiGDIGeometry::regularTriangleOut(const OdInt32* pTriangleVertices, const OdGeVector3d* pNormal)
{
  m_trianglePts[0] = vertexDataList()[pTriangleVertices[0]];
  m_trianglePts[1] = vertexDataList()[pTriangleVertices[1]];
  m_trianglePts[2] = vertexDataList()[pTriangleVertices[2]];
  polygonOut(3, m_trianglePts, pNormal);
}

struct GradientSubdivider
{ // #8151
  enum ClipSide
  {
    CS_Left   = 1,
    CS_Top    = 2,
    CS_Right  = 4,
    CS_Bottom = 8
  };

  static bool RequireSubdivision(const TRIVERTEX *pVts, const OdInt32* pTriangleVertices,
                                 const OdGsDCRect &rcRect, OdUInt32 &bSubdiv, LONG limit)
  {
    LONG minX = pVts[pTriangleVertices[0]].x,
         minY = pVts[pTriangleVertices[0]].y,
         maxX = pVts[pTriangleVertices[0]].x,
         maxY = pVts[pTriangleVertices[0]].y;

    if (pVts[pTriangleVertices[1]].x < minX)
      minX = pVts[pTriangleVertices[1]].x;
    else if (pVts[pTriangleVertices[1]].x > maxX)
      maxX = pVts[pTriangleVertices[1]].x;
    if (pVts[pTriangleVertices[1]].y < minY)
      minY = pVts[pTriangleVertices[1]].y;
    else if (pVts[pTriangleVertices[1]].y > maxY)
      maxY = pVts[pTriangleVertices[1]].y;

    if (pVts[pTriangleVertices[2]].x < minX)
      minX = pVts[pTriangleVertices[2]].x;
    else if (pVts[pTriangleVertices[2]].x > maxX)
      maxX = pVts[pTriangleVertices[2]].x;
    if (pVts[pTriangleVertices[2]].y < minY)
      minY = pVts[pTriangleVertices[2]].y;
    else if (pVts[pTriangleVertices[2]].y > maxY)
      maxY = pVts[pTriangleVertices[2]].y;

    if (maxX < rcRect.m_min.x || minX > rcRect.m_max.x ||
        maxY < rcRect.m_max.y || minY > rcRect.m_min.y)
      return false;

    if (minX < rcRect.m_min.x - limit && maxX > rcRect.m_min.x - limit)
      bSubdiv |= CS_Left;
    if (minY < rcRect.m_max.y - limit && maxY > rcRect.m_max.y - limit)
      bSubdiv |= CS_Top;
    if (maxX > rcRect.m_max.x + limit && minX < rcRect.m_max.x + limit)
      bSubdiv |= CS_Right;
    if (maxY > rcRect.m_min.y + limit && minY < rcRect.m_min.y + limit)
      bSubdiv |= CS_Bottom;
    return true;
  }

  static bool LineIsect(LONG x1, LONG y1, LONG x2, LONG y2,
                        LONG x3, LONG y3, LONG x4, LONG y4, LONG &x, LONG &y)
  {
    // Uses INT64-s and doubles to improve stability
    if (x3 == x4)
    {
      // Horizontal xline
      OdInt64 ymin, ymax, xmin, xmax;
      if (x1 < x2)
      {
        xmin = x1; xmax = x2; ymin = y1; ymax = y2;
      }
      else
      {
        xmin = x2; xmax = x1; ymin = y2; ymax = y1;
      }
      if (x3 >= xmin && x4 <= xmax)
      {
        // Inside segment
        if (x1 == x3 && x2 == x3)
          return false; // Collinear
        x = x3;
        if (xmax != xmin)
        {
          y = LONG(OdInt64(double(OdInt64(x3) - xmin) / (xmax - xmin) * (ymax - ymin)) + ymin);
        }
        else
        {
          y = LONG(ymin);
        }
        return true;
      }
    }
    else
    {
      // Vertical xline
      OdInt64 ymin, ymax, xmin, xmax;
      if (y1 < y2)
      {
        xmin = x1; xmax = x2; ymin = y1; ymax = y2;
      }
      else
      {
        xmin = x2; xmax = x1; ymin = y2; ymax = y1;
      }
      if (y3 >= ymin && y4 <= ymax)
      {
        // Inside segment
        if (y1 == y3 && y2 == y3)
          return false; // Collinear
        y = y3;
        if (ymax != ymin)
        {
          x = LONG(OdInt64(double(OdInt64(y3) - ymin) / (ymax - ymin) * (xmax - xmin)) + xmin);
        }
        else
        {
          x = LONG(xmin);
        }
        return true;
      }
    }
    return false;
  }

  static void IplColor(const TRIVERTEX *v1, const TRIVERTEX *v2, LONG x, LONG y, TRIVERTEX &fill)
  {
    fill.x = x; fill.y = y;
    LONG dx = v2->x - v1->x, dy = v2->y - v1->y;
    double l = sqrt(double(dx) * double(dx) + double(dy) * double(dy));
    if (OdZero(l))
    {
      fill.Red = v1->Red;
      fill.Green = v1->Green;
      fill.Blue = v1->Blue;
      fill.Alpha = v1->Alpha;
      return;
    }
    dx = x - v1->x, dy = y - v1->y;
    double t = sqrt(double(dx) * double(dx) + double(dy) * double(dy)) / l;
    fill.Red = v1->Red + COLOR16((v2->Red - v1->Red) * t);
    fill.Green = v1->Green + COLOR16((v2->Green - v1->Green) * t);
    fill.Blue = v1->Blue + COLOR16((v2->Blue - v1->Blue) * t);
    fill.Alpha = 0;
  }

  static OdUInt32 Subdivide(TRIVERTEXArray &verts, const OdInt32* pTriangleVertices, OdInt32 *outTris,
                            const OdGsDCRect &rcRect, OdUInt32 bSubdiv, LONG limit)
  {
    LONG xs1, ys1, xs2, ys2, xo1 = 0, yo1 = 0, xo2 = 0, yo2 = 0, ns1 = 0, ns2 = 0, ptn = 0;
    OdInt32 ins = (OdInt32)verts.size();
    if (GETBIT(bSubdiv, CS_Left))
    {
      xs1 = xs2 = rcRect.m_min.x - limit;
      ys1 = rcRect.m_max.y - limit;
      ys2 = rcRect.m_min.y + limit;
    }
    else if (GETBIT(bSubdiv, CS_Top))
    {
      ys1 = ys2 = rcRect.m_max.y - limit;
      xs1 = rcRect.m_min.x - limit;
      xs2 = rcRect.m_max.x + limit;
    }
    else if (GETBIT(bSubdiv, CS_Right))
    {
      xs1 = xs2 = rcRect.m_max.x + limit;
      ys1 = rcRect.m_max.y - limit;
      ys2 = rcRect.m_min.y + limit;
    }
    else
    {
      ys1 = ys2 = rcRect.m_max.y + limit;
      xs1 = rcRect.m_min.x - limit;
      xs2 = rcRect.m_max.x + limit;
    }
    TRIVERTEX *pVerts[3] = { &verts[pTriangleVertices[0]], &verts[pTriangleVertices[1]], &verts[pTriangleVertices[2]] };
    LONG opt_x, opt_y;
    if (LineIsect(pVerts[0]->x, pVerts[0]->y, pVerts[1]->x, pVerts[1]->y, xs1, ys1, xs2, ys2, opt_x, opt_y))
    {
      ptn = 1;
      xo1 = opt_x;
      yo1 = opt_y;
      ns1 = 0;
    }
    if (LineIsect(pVerts[1]->x, pVerts[1]->y, pVerts[2]->x, pVerts[2]->y, xs1, ys1, xs2, ys2, opt_x, opt_y))
    {
      if (ptn == 1)
      {
        xo2 = opt_x;
        yo2 = opt_y;
        ns2 = 1;
        ptn = 2;
      }
      else
      {
        ptn = 1;
        xo1 = opt_x;
        yo1 = opt_y;
        ns1 = 1;
      }
    }
    if (ptn != 2)
    {
      if (LineIsect(pVerts[2]->x, pVerts[2]->y, pVerts[0]->x, pVerts[0]->y, xs1, ys1, xs2, ys2, opt_x, opt_y))
      {
        if (ptn == 1)
        {
          xo2 = opt_x;
          yo2 = opt_y;
          ns2 = 2;
          ptn = 2;
        }
      }
    }
    if (ptn != 2)
      return 0;
    LONG eLock = 0;
    if (ns1 != 1 && ns2 != 1)
      eLock = 1;
    else if (ns1 != 2 && ns2 != 2)
      eLock = 2;
    LONG vertCut = (eLock == 0) ? 2 : ((eLock == 1) ? 0 : 1);
    TRIVERTEX sd1, sd2;
    switch (vertCut)
    {
      case 0:
        IplColor(pVerts[vertCut], pVerts[1], xo1, yo1, sd1);
        IplColor(pVerts[vertCut], pVerts[2], xo2, yo2, sd2);
      break;
      case 1:
        IplColor(pVerts[vertCut], pVerts[0], xo1, yo1, sd1);
        IplColor(pVerts[vertCut], pVerts[2], xo2, yo2, sd2);
      break;
      case 2:
        IplColor(pVerts[vertCut], pVerts[1], xo1, yo1, sd1);
        IplColor(pVerts[vertCut], pVerts[0], xo2, yo2, sd2);
    };
    // Cutted triangle
    outTris[0] = ins; outTris[1] = pTriangleVertices[vertCut]; outTris[2] = ins + 1;
    // Cutted quadrilateral
    switch (eLock)
    {
      case 0:
        outTris[3] = pTriangleVertices[0]; outTris[4] = pTriangleVertices[1]; outTris[5] = ins;
        outTris[6] = pTriangleVertices[0]; outTris[7] = ins; outTris[8] = ins + 1;
      break;
      case 1:
        outTris[3] = pTriangleVertices[1]; outTris[4] = pTriangleVertices[2]; outTris[5] = ins + 1;
        outTris[6] = pTriangleVertices[1]; outTris[7] = ins + 1; outTris[8] = ins;
      break;
      case 2:
        outTris[3] = pTriangleVertices[2]; outTris[4] = pTriangleVertices[0]; outTris[5] = ins;
        outTris[6] = pTriangleVertices[2]; outTris[7] = ins; outTris[8] = ins + 1;
    }
    verts.push_back(sd1);
    verts.push_back(sd2);
    return 3;
  }
};

void ExGiGDIGeometry::gradientTriangleOut(const OdInt32* pTriangleVertices, const OdGeVector3d* )
{
  const LONG limit = 4000;
  OdUInt32 bSubdiv = 0;
  if (GradientSubdivider::RequireSubdivision(m_triVertexes.getPtr(), pTriangleVertices, m_rcTarget, bSubdiv, limit))
  {
    if (bSubdiv)
    {
      OdInt32 outTris[12];
      OdUInt32 nTris = GradientSubdivider::Subdivide(m_triVertexes, pTriangleVertices, outTris, m_rcTarget, bSubdiv, limit);
      OdInt32 *pData = outTris;
      while (nTris--) {
        gradientTriangleOut(pData, NULL);
        pData += 3;
      }
      return;
    }

    GRADIENT_TRIANGLE* pTriangle = m_triangles.append();

    pTriangle->Vertex1 = *(pTriangleVertices++);
    pTriangle->Vertex2 = *(pTriangleVertices++);
    pTriangle->Vertex3 = *(pTriangleVertices++);

    //OdString fmt;
    //fmt.format(L"Tri: %d, %d - %d, %d - %d, %d\n", m_triVertexes[pTriangle->Vertex1].x, m_triVertexes[pTriangle->Vertex1].y,
    //           m_triVertexes[pTriangle->Vertex2].x, m_triVertexes[pTriangle->Vertex2].y,
    //           m_triVertexes[pTriangle->Vertex3].x, m_triVertexes[pTriangle->Vertex3].y);
    //ODA_TRACE(fmt.c_str());
    //printf(OdAnsiString(fmt).c_str());
  }
}

void ExGiGDIGeometry::endGradientFill(HDC hdc)
{
  //SEA 3/21/2017 Now "Gradient As Bitmap" should use new way in all cases
  if( !m_bGradientAsPolys && m_bGradientAsBitmap )
  {
    //Instead on GradientFill we will use own gradient render
    renderGradientTriangles( hdc );
    //SEA 4/13/2017
    //CORE-11507 fix
    m_triVertexes.clear();
    m_triangleFn = &ExGiGDIGeometry::regularTriangleOut;
    return;
  }
 
  if (!m_bGradientAsBitmap && !m_bGradientAsPolys)
  {
#if defined(OD_ENABLE_POSTSCRIPT_PRINTING) && defined(OD_ENABLE_POSTSCRIPT_LEVEL3)
    // Final limit: 3633, truncated: 3600. #7542 truncated: 2500
    const OdUInt32 PSLimit = 2500; // PostScript Level 3 error workaround
#endif
#ifdef OD_ENABLE_POSTSCRIPT_PRINTING
    int devType = ::GetDeviceCaps(hdc, TECHNOLOGY);
    //DWORD objType = ::GetObjectType(hdc);
    //bool gradSp = (::GetDeviceCaps(hdc, SHADEBLENDCAPS) & SB_GRAD_TRI) == SB_GRAD_TRI;
#ifdef OD_ENABLE_POSTSCRIPT_LEVEL3
    if (devType != DT_RASPRINTER || m_triangles.size() < PSLimit)
#else
    if (devType != DT_RASPRINTER)
#endif
#endif
    {
      ((GRADFILLPROC)m_grFillPrAddr)(hdc, m_triVertexes.asArrayPtr(), m_triVertexes.size(),
        m_triangles.asArrayPtr(), m_triangles.size(), GRADIENT_FILL_TRIANGLE);
    }
#ifdef OD_ENABLE_POSTSCRIPT_PRINTING
    else
    {
#ifdef OD_ENABLE_POSTSCRIPT_LEVEL3
#define OD_MULT_X_3(x) (((x) << 1) + (x)) // Optimized multiplication by 3 ((x * 3) == ((x << 1) + x))
      // Workaround for printing via PostScript. PostScript Level 3 has limits on gradient triangles output which causes stackoverflow error.
      TRIVERTEX *triVerts = new TRIVERTEX[OD_MULT_X_3(PSLimit)];
      GRADIENT_TRIANGLE *pTriIn = m_triangles.asArrayPtr(), *pTriOut;
      pTriOut = new GRADIENT_TRIANGLE[PSLimit];
      OdUInt32 i, j, trisSize = m_triangles.size(), nTris;
      for (i = 0, j = 0; i < PSLimit; i++, j += 3)
      {
        pTriOut[i].Vertex1 = j;
        pTriOut[i].Vertex2 = j + 1;
        pTriOut[i].Vertex3 = j + 2;
      }
      for (i = 0; i < trisSize; i += PSLimit)
      {
        nTris = odmin(trisSize - i, PSLimit);
        TRIVERTEX *pTriProc = triVerts;
        for (j = 0; j < nTris; j++, pTriProc += 3)
        {
          pTriProc[0] = m_triVertexes.at(pTriIn[i + j].Vertex1);
          pTriProc[1] = m_triVertexes.at(pTriIn[i + j].Vertex2);
          pTriProc[2] = m_triVertexes.at(pTriIn[i + j].Vertex3);
        }
        ((GRADFILLPROC)m_grFillPrAddr)(hdc, triVerts, OD_MULT_X_3(nTris),
          pTriOut, nTris, GRADIENT_FILL_TRIANGLE);
      }
      delete []pTriOut;
      delete []triVerts;
#undef OD_MULT_X_3
#else
      // PostScript Level 1 and 2 workaround. Will work on Level 3 also.
#error @@@TODO: PostScript doesn't support transparent image background. Need find other solution. Clipping?
      SIZE minLim; minLim.cx = m_triVertexes.first().x; minLim.cy = m_triVertexes.first().y;
      SIZE maxLim; maxLim = minLim;
      TRIVERTEX *pTriVerts = m_triVertexes.asArrayPtr();
      OdUInt32 nVerts = m_triVertexes.size(), i;
      for (i = 1; i < nVerts; i++) {
        if (pTriVerts[i].x < minLim.cx) minLim.cx = pTriVerts[i].x;
        else if (pTriVerts[i].x > maxLim.cx) maxLim.cx = pTriVerts[i].x;
        if (pTriVerts[i].y < minLim.cy) minLim.cy = pTriVerts[i].y;
        else if (pTriVerts[i].y > maxLim.cy) maxLim.cy = pTriVerts[i].y;
      }
      SIZE bmpSize; bmpSize.cx = maxLim.cx - minLim.cx + 1; bmpSize.cy = maxLim.cy - minLim.cy + 1;
      RECT bmpRect; bmpRect.left = bmpRect.top = 0; bmpRect.right = bmpSize.cx; bmpRect.bottom = bmpSize.cy;
      HDC hMemDC = ::CreateCompatibleDC(hdc);
      HBITMAP hMemBitmap = ::CreateCompatibleBitmap(hdc, bmpSize.cx, bmpSize.cy);
      HBITMAP hPrevMemBitmap = (HBITMAP)::SelectObject(hMemDC, (HGDIOBJ)hMemBitmap);
      HDC hMaskDC = ::CreateCompatibleDC(NULL/*hdc*/);
      HBITMAP hMaskBitmap = ::CreateBitmap(bmpSize.cx, bmpSize.cy, 1, 1, NULL);//::CreateCompatibleBitmap(hdc, bmpSize.cx, bmpSize.cy);
      HBITMAP hPrevMaskBitmap = (HBITMAP)::SelectObject(hMaskDC, (HGDIOBJ)hMaskBitmap);
      ::FillRect(hMemDC, &bmpRect, (HBRUSH)::GetStockObject(BLACK_BRUSH));
      //::FillRect(hMaskDC, &bmpRect, (HBRUSH)::GetStockObject(WHITE_BRUSH));
      ::SetWindowOrgEx(hMemDC, minLim.cx, minLim.cy, NULL);
      ::SetWindowOrgEx(hMaskDC, minLim.cx, minLim.cy, NULL);
      ((GRADFILLPROC)m_grFillPrAddr)(hMemDC, pTriVerts, nVerts,
        m_triangles.asArrayPtr(), m_triangles.size(), GRADIENT_FILL_TRIANGLE);
      ////::SetROP2(hMaskDC, R2_WHITE);
      //for (i = 0; i < nVerts; i++) {
      //  pTriVerts[i].Red = pTriVerts[i].Green = pTriVerts[i].Blue = 0xFF << 8;
      //}
      //((GRADFILLPROC)m_grFillPrAddr)(hMaskDC, pTriVerts, nVerts,
      //  m_triangles.asArrayPtr(), m_triangles.size(), GRADIENT_FILL_TRIANGLE);
      ::SetWindowOrgEx(hMemDC, 0, 0, NULL);
      ::SetWindowOrgEx(hMaskDC, 0, 0, NULL);
      ::SetBkColor(hMemDC, RGB(0, 0, 0));
      ::BitBlt(hMaskDC, 0, 0, bmpSize.cx, bmpSize.cy, hMemDC, 0, 0, SRCCOPY);
      //::BitBlt(hdc, minLim.cx, minLim.cy, bmpSize.cx, bmpSize.cy, hMaskDC, 0, 0, SRCAND);
      //::BitBlt(hdc, minLim.cx, minLim.cy, bmpSize.cx, bmpSize.cy, hMemDC, 0, 0, SRCPAINT);
      ::MaskBlt(hdc, minLim.cx, minLim.cy, bmpSize.cx, bmpSize.cy, hMemDC, 0, 0, hMaskBitmap, 0, 0, 0xCCAA0020);
      ::SelectObject(hMaskDC, (HGDIOBJ)hPrevMaskBitmap);
      ::SelectObject(hMemDC, (HGDIOBJ)hPrevMemBitmap);
      ::DeleteObject((HGDIOBJ)hMaskBitmap);
      ::DeleteObject((HGDIOBJ)hMemBitmap);
      ::DeleteDC(hMaskDC);
      ::DeleteDC(hMemDC);
#endif
    }
#endif
  }
  else
  { // Draw gradients as bitmap
    // Compute size of gradient
    SIZE minLim; minLim.cx = m_triVertexes.first().x; minLim.cy = m_triVertexes.first().y;
    SIZE maxLim; maxLim = minLim;
    TRIVERTEX *pTriVerts = m_triVertexes.asArrayPtr();
    OdUInt32 nVerts = m_triVertexes.size(), i;
    for (i = 1; i < nVerts; i++) {
      if (pTriVerts[i].x < minLim.cx) minLim.cx = pTriVerts[i].x;
      else if (pTriVerts[i].x > maxLim.cx) maxLim.cx = pTriVerts[i].x;
      if (pTriVerts[i].y < minLim.cy) minLim.cy = pTriVerts[i].y;
      else if (pTriVerts[i].y > maxLim.cy) maxLim.cy = pTriVerts[i].y;
    }
    // Clip gradient by output rect
    if (minLim.cx < m_rcTarget.m_min.x)
      minLim.cx = m_rcTarget.m_min.x;
    if (maxLim.cx > m_rcTarget.m_max.x)
      maxLim.cx = m_rcTarget.m_max.x;
    if (minLim.cy < m_rcTarget.m_max.y)
      minLim.cy = m_rcTarget.m_max.y;
    if (maxLim.cy > m_rcTarget.m_min.y)
      maxLim.cy = m_rcTarget.m_min.y;
    if ((minLim.cx < maxLim.cx) && (minLim.cy < maxLim.cy))
    {
      SIZE bmpSize; bmpSize.cx = maxLim.cx - minLim.cx + 1; bmpSize.cy = maxLim.cy - minLim.cy + 1;
      RECT bmpRect; bmpRect.left = bmpRect.top = 0; bmpRect.right = bmpSize.cx; bmpRect.bottom = bmpSize.cy;
      //int devType = ::GetDeviceCaps(hdc, TECHNOLOGY);
      //int blendCaps = ::GetDeviceCaps(hdc, SHADEBLENDCAPS); // drivers never returns them correct
      if( m_bGradientAsPolys ) //|| /*(((devType != DT_RASPRINTER) || (blendCaps & SB_PIXEL_ALPHA)) &&*/ m_alphaBlendPrAddr/*)*/)
      {

        OdUInt8* pPixels = new OdUInt8[ bmpSize.cx * bmpSize.cy * 4 ];//32bpp buffer
        if( pPixels )   
        {
          OdUInt8 *pPixelsBuf = pPixels;
          OdUInt32 nPixels = bmpSize.cx * bmpSize.cy, nPixel;
          for (nPixel = 0; nPixel < nPixels; nPixel++, pPixelsBuf += 4)
          {
            pPixelsBuf[0] = pPixelsBuf[1] = pPixelsBuf[2] = 0;
            pPixelsBuf[3] = 255;
          }
          ExGsGradientRender render;
          render.setupContext32BPP( bmpSize.cx, bmpSize.cy, pPixels, 0 );
          render.setOrigin( OdGePoint2d( minLim.cx, minLim.cy ) );
          render.setClipRect( OdGsDCRect( minLim.cx, maxLim.cx, minLim.cy, maxLim.cy ) );
          render.renderTriangles( pTriVerts, nVerts,  m_triangles.asArrayPtr(), m_triangles.size() );
          pPixelsBuf = pPixels;
//#define OD_CLUSTERIZER_DEBUG
#ifdef OD_CLUSTERIZER_DEBUG
          // background image for test
          for (nPixel = 0; nPixel < nPixels; nPixel++, pPixelsBuf += 4)
            pPixelsBuf[3] = ~pPixelsBuf[3]; // Invert alpha (gradiends drawn with zero alpha)
          BLENDFUNCTION bf;
          bf.AlphaFormat = AC_SRC_ALPHA;
          bf.BlendFlags = 0;
          bf.BlendOp = AC_SRC_OVER;
          bf.SourceConstantAlpha = 255;
          ((ALPHABLENDPROC)m_alphaBlendPrAddr)(hdc, minLim.cx, minLim.cy, bmpSize.cx, bmpSize.cy, hMemDC, 0, 0, bmpSize.cx, bmpSize.cy, bf);
          pPixelsBuf = pPixels;
          for (nPixel = 0; nPixel < nPixels; nPixel++, pPixelsBuf += 4)
            pPixelsBuf[3] = ~pPixelsBuf[3]; // Invert alpha (gradiends drawn with zero alpha)
          pPixelsBuf = pPixels;
#else
          HGDIOBJ hPrevPen = ::SelectObject(hdc, ::GetStockObject(NULL_PEN));
          HGDIOBJ hPrevBrush;
          if (m_dcBrushClrPrAddr)
            hPrevBrush = ::SelectObject(hdc, ::GetStockObject(DC_BRUSH));
#endif
          // Subdivide image into set of optimal clusters and output as polygones
          struct Clusterizer {
            HDC m_hDc;
            SIZE m_minLim;
            OdUInt8 m_threshold;
            OdUInt8 *m_pPixels;
            SIZE m_sizeIm;
            DCBRUSHCLRPROC m_dcProc;

            Clusterizer(HDC hDc, const SIZE &minLim, OdUInt8 threshold, OdUInt8 *pPixels, const SIZE &sizeIm, DCBRUSHCLRPROC dcProc)
              : m_hDc(hDc), m_minLim(minLim), m_threshold(threshold), m_pPixels(pPixels), m_sizeIm(sizeIm), m_dcProc(dcProc) { }

            inline bool isPxFit(const OdUInt8 *pOrig, const OdUInt8 *pPx)
            {
              if (pPx[3] == 255) return false;
              return ((Od_abs((long)pOrig[0] - (long)pPx[0]) <= m_threshold) &&
                      (Od_abs((long)pOrig[1] - (long)pPx[1]) <= m_threshold) &&
                      (Od_abs((long)pOrig[2] - (long)pPx[2]) <= m_threshold));
            }

            void clusterize()
            {
              const bool bBMPHeightReversed = false; // #CORE-11879 : in GradientRender scanlines isn't reversed in difference with DIB.
              OdUInt32 scanlineLength = m_sizeIm.cx * 4;
              OdUInt8 *pScan = (bBMPHeightReversed) ? m_pPixels : (m_pPixels + scanlineLength * (m_sizeIm.cy - 1));
#ifdef OD_CLUSTERIZER_DEBUG
              POINT apt[5]; // for test
#else
              POINT apt[4];
#endif
              for (OdUInt32 nY = 0; nY < (OdUInt32)m_sizeIm.cy; nY++)
              {
                for (OdUInt32 nX = 0; nX < (OdUInt32)m_sizeIm.cx; nX++, pScan += 4)
                {
                  if (pScan[3] == 0) // First pixel for square
                  {
                    // Compute maximum fit pixels horizontally
                    OdUInt32 nXMatch = 1; OdUInt8 *pScanIt = pScan + 4;
                    for (OdUInt32 nXMatchIt = nX + 1; nXMatchIt < (OdUInt32)m_sizeIm.cx; nXMatchIt++, pScanIt += 4)
                    {
                      if (isPxFit(pScan, pScanIt))
                        nXMatch++;
                      else
                        break;
                    }
                    // Assume that our horizontal row is a maximum rectangle
                    SIZE nCurMax; OdUInt32 curMaxArea = nXMatch;
                    nCurMax.cx = nXMatch; nCurMax.cy = 1;
                    // Run through each row and detect maximum area
                    OdUInt32 nYMatch = 1; pScanIt = (bBMPHeightReversed) ? (pScan + scanlineLength) : (pScan - scanlineLength);
                    for (OdUInt32 nYMatchIt = nY + 1; nYMatchIt < (OdUInt32)m_sizeIm.cy; nYMatchIt++, pScanIt = (bBMPHeightReversed) ? (pScanIt + scanlineLength) : (pScanIt - scanlineLength))
                    {
                      OdUInt8 *pScanline = pScanIt;
                      OdUInt32 nRowMax = 0;
                      for (OdUInt32 nXMatchIt = 0; nXMatchIt < nXMatch; nXMatchIt++, pScanline += 4)
                      {
                        if (isPxFit(pScan, pScanline))
                          nRowMax++;
                        else
                          break;
                      }
                      if (!nRowMax)
                        break; // Nothing to search more
                      nYMatch++;
                      OdUInt32 maxArea = nYMatch * nRowMax;
                      if (maxArea > curMaxArea)
                      {
                        nCurMax.cx = nRowMax; nCurMax.cy = nYMatch;
                        curMaxArea = maxArea;
                      }
                      nXMatch = nRowMax;
                    }
                    // Now we have maximum possible cluster to output
                    // Compute middle color for set of pixels and mark pixels as used
                    pScanIt = pScan;
                    OdUInt8 minColor[3], maxColor[3];
                    minColor[0] = maxColor[0] = pScan[0];
                    minColor[1] = maxColor[1] = pScan[1];
                    minColor[2] = maxColor[2] = pScan[2];
                    for (OdUInt32 nYBlock = 0; nYBlock < (OdUInt32)nCurMax.cy; nYBlock++, pScanIt = (bBMPHeightReversed) ? (pScanIt + scanlineLength) : (pScanIt - scanlineLength))
                    {
                      OdUInt8 *pScanline = pScanIt;
                      for (OdUInt32 nXBlock = 0; nXBlock < (OdUInt32)nCurMax.cx; nXBlock++, pScanline += 4)
                      {
                        if (pScanline[0] < minColor[0]) minColor[0] = pScanline[0];
                        else if (pScanline[0] > maxColor[0]) maxColor[0] = pScanline[0];
                        if (pScanline[1] < minColor[1]) minColor[1] = pScanline[1];
                        else if (pScanline[1] > maxColor[1]) maxColor[1] = pScanline[1];
                        if (pScanline[2] < minColor[2]) minColor[2] = pScanline[2];
                        else if (pScanline[2] > maxColor[2]) maxColor[2] = pScanline[2];
                        pScanline[3] = 255;
                      }
                    }
                    // Now draw cluster
                    apt[0].x = m_minLim.cx + nX; apt[0].y = m_minLim.cy + m_sizeIm.cy - nY;
                    apt[1].x = apt[0].x + nCurMax.cx; apt[1].y = apt[0].y;
                    apt[2].x = apt[1].x; apt[2].y = apt[0].y - nCurMax.cy;
                    apt[3].x = apt[0].x; apt[3].y = apt[2].y;
#ifdef OD_CLUSTERIZER_DEBUG
                    apt[4] = apt[0]; // for test
                    ::Polyline(m_hDc, apt, 5); // for test
#else
                    if (m_dcProc)
                    {
                      m_dcProc(m_hDc, RGB(minColor[2] + ((maxColor[2] - minColor[2]) >> 1),
                                          minColor[1] + ((maxColor[1] - minColor[1]) >> 1),
                                          minColor[0] + ((maxColor[0] - minColor[0]) >> 1)));
                      ::Polygon(m_hDc, apt, 4);
                    }
                    else
                    {
                      HBRUSH hBrush = ::CreateSolidBrush(RGB(minColor[2] + ((maxColor[2] - minColor[2]) >> 1),
                                                             minColor[1] + ((maxColor[1] - minColor[1]) >> 1),
                                                             minColor[0] + ((maxColor[0] - minColor[0]) >> 1)));
                      HGDIOBJ hPrevBrush = ::SelectObject(m_hDc, (HGDIOBJ)hBrush);
                      ::Polygon(m_hDc, apt, 4);
                      ::SelectObject(m_hDc, hPrevBrush);
                      ::DeleteObject((HGDIOBJ)hBrush);
                    }
#endif
                  }
                }
                if (!bBMPHeightReversed)
                  pScan -= scanlineLength * 2;
              }
            }
            // Since we operate with absolute valuer threshold must be divided by two
          } _clusterizer(hdc, minLim, m_nGradientAsPolysSubdivideTreshold >> 1, pPixelsBuf, bmpSize, (DCBRUSHCLRPROC)m_dcBrushClrPrAddr);
          _clusterizer.clusterize();
#ifndef OD_CLUSTERIZER_DEBUG
          ::SelectObject(hdc, hPrevPen);
          if (m_dcBrushClrPrAddr)
            ::SelectObject(hdc, hPrevBrush);
#endif
          delete[] pPixelsBuf;
        }
      }
      else
      { // Typically doesn't supported by printers. But works with XPS.
        HDC hMemDC = ::CreateCompatibleDC(hdc);
        if (!hMemDC) hMemDC = ::CreateCompatibleDC(NULL); // Possible for EMF
        HBITMAP hMemBitmap = ::CreateCompatibleBitmap(hdc, bmpSize.cx, bmpSize.cy);
        HBITMAP hPrevMemBitmap = (HBITMAP)::SelectObject(hMemDC, (HGDIOBJ)hMemBitmap);
        HDC hMaskDC = ::CreateCompatibleDC(NULL);
        HBITMAP hMaskBitmap = ::CreateBitmap(bmpSize.cx, bmpSize.cy, 1, 1, NULL);
        HBITMAP hPrevMaskBitmap = (HBITMAP)::SelectObject(hMaskDC, (HGDIOBJ)hMaskBitmap);
        ::FillRect(hMemDC, &bmpRect, (HBRUSH)::GetStockObject(BLACK_BRUSH));
        ::SetWindowOrgEx(hMemDC, minLim.cx, minLim.cy, NULL);
        ((GRADFILLPROC)m_grFillPrAddr)(hMemDC, pTriVerts, nVerts,
          m_triangles.asArrayPtr(), m_triangles.size(), GRADIENT_FILL_TRIANGLE);
        ::SetWindowOrgEx(hMemDC, 0, 0, NULL);
        ::SetBkColor(hMemDC, RGB(0, 0, 0));
        ::BitBlt(hMaskDC, 0, 0, bmpSize.cx, bmpSize.cy, hMemDC, 0, 0, SRCCOPY);
        ::MaskBlt(hdc, minLim.cx, minLim.cy, bmpSize.cx, bmpSize.cy, hMemDC, 0, 0, hMaskBitmap, 0, 0, 0xCCAA0020);
        ::SelectObject(hMaskDC, (HGDIOBJ)hPrevMaskBitmap);
        ::SelectObject(hMemDC, (HGDIOBJ)hPrevMemBitmap);
        ::DeleteObject((HGDIOBJ)hMaskBitmap);
        ::DeleteObject((HGDIOBJ)hMemBitmap);
        ::DeleteDC(hMaskDC);
        ::DeleteDC(hMemDC);
      }
    }
  }
  m_triVertexes.clear();
  m_triangleFn = &ExGiGDIGeometry::regularTriangleOut;
}

#endif //#ifdef _WIN32_WCE


OdGeTol g_drawTol(.5);

//Implementation of class ExGsOpenGLBaseDevice
//*******************************************************************************/
// Constructor
//*******************************************************************************/
ExGiGDIGeometry::ExGiGDIGeometry()
  : m_hOldBrush(0)
  , m_hOldPen(0)
  , m_hNullPen(0)
  , m_hThinPen(0)
  , m_hExPen(0)
  , m_hNullBrush(0)
  , m_bPenValid(false)
  , m_bBrushValid(false)
  , m_bThinPenValid(false)
  , m_thinPenOverride(0)
  , m_hTargetDC(0)
  , m_paletteColors(0)
  , m_numPaletteColors(0)
  , m_lpStyle(NULL)
  , m_oldEndStyle(OdPs::kLesUseObject)
  , m_oldJoinStyle(OdPs::kLjsUseObject)
  , m_oldLinetype(kLtpSolid)
  , m_oldFillStyle(kFsSolid)
  , m_oldBkMode(TRANSPARENT)
#ifndef _WIN32_WCE
  , m_hinstMsImgDll(NULL)
  , m_grFillPrAddr(NULL)
  , m_alphaBlendPrAddr(NULL)
  , m_dcBrushClrPrAddr(NULL)
  , m_triangleFn(0)
#endif
  , m_psLinetypeDefs(::odgiGetPsLinetypesManager(OdGiPsLinetypes::kGDIDefs).getGDIDefinitions())
  , m_psFillstyleDefs(::odgiGetPsFillstylesManager().getGDIDefinitions())
  , m_bProcessingTTF(false)
  , m_bDrawDragging(false)
  , m_bDrawSelection(false)
  , m_bGlobalAlpha(false)
  , m_nGlobalAlpha(0)
  , m_clrFadeColor(0)
  , m_bPointsLwd(true)
  , m_lwdCapStyle(OdPs::kLesRound)
  , m_lwdJoinStyle(OdPs::kLjsRound)
  , m_bExtLwdStyle(false)
  , m_bLwdOverride(false)
  //SEA 21/3/2017 GradientAsBitmap is true by default now
  , m_bGradientAsBitmap(true)
  , m_bGradientAsPolys(false)
  , m_nGradientAsPolysSubdivideTreshold(10)
  , m_pColorConverter(NULL)
{
  /*m_logPen.lopnStyle = PS_SOLID;
  m_logPen.lopnColor = 0;
  m_logPen.lopnWidth.x =
  m_logPen.lopnWidth.y = 0;*/
  m_dwPenStyle   = PS_SOLID;
#ifndef _WIN32_WCE
  m_dwPenStyle |= PS_GEOMETRIC;
#endif

  m_dwWidth      = 1;
  m_dwStyleCount = 0;
  m_penBrush.lbStyle = BS_SOLID;
  m_penBrush.lbHatch = 0;
  m_penBrush.lbColor = 0;

  m_logBrush.lbStyle = BS_SOLID;
  m_logBrush.lbColor = 0;
  m_logBrush.lbHatch = 0;

  OdGeDoubleArray values(5);
  values.insert(values.end(), 5, .5);

  OdGiGeometrySimplifier::setDeviation(values);

#ifndef _WIN32_WCE
  m_triangleFn = &ExGiGDIGeometry::regularTriangleOut;
  m_hinstMsImgDll = LoadLibrary(_T("msimg32"));
  if(m_hinstMsImgDll)
  {
    m_grFillPrAddr = ::GetProcAddress(m_hinstMsImgDll, "GradientFill");
    m_alphaBlendPrAddr = ::GetProcAddress(m_hinstMsImgDll, "AlphaBlend");
  }
  {
    HMODULE gdiDll = ::LoadLibrary(_T("Gdi32"));
    if (gdiDll)
    {
      m_dcBrushClrPrAddr = ::GetProcAddress(gdiDll, "SetDCBrushColor");
      // Gdi32.dll in any case linked with WinGDI, so just unlink library here.
      ::FreeLibrary(gdiDll);
    }
  }
#endif //#ifndef _WIN32_WCE
}

ExGiGDIGeometry::~ExGiGDIGeometry()
{
#ifndef _WIN32_WCE
  if(m_hinstMsImgDll)
  {
    ::FreeLibrary(m_hinstMsImgDll);
  }
#endif //#ifndef _WIN32_WCE
}

void ExGiGDIGeometry::setLogicalPalette(const ODCOLORREF* palette, OdUInt16 nCount)
{
  m_numPaletteColors = nCount;
  m_paletteColors = palette;
}

void ExGiGDIGeometry::beginPaint(HDC hTargetDC)
{
  m_hTargetDC = hTargetDC;

  // needs to be re-initialised. Because beginPaint can be called several times during device life-time
  m_logBrush.lbStyle = BS_SOLID;
  // m_logBrush.lbColor = 0; this is bg color which can be set outside
  m_logBrush.lbHatch = 0;
  m_dwPenStyle       = PS_SOLID;
#ifndef _WIN32_WCE
  m_dwPenStyle |= PS_GEOMETRIC;
#endif

  m_dwWidth          = 1;
  m_penBrush.lbStyle = BS_SOLID;
  m_penBrush.lbHatch = 0;
  // m_penBrush.lbColor = 0; this is bg color which can be set outside
  m_dwStyleCount     = 0;

  m_hOldBrush = ::SelectObject(hTargetDC, ::CreateBrushIndirect(&m_logBrush));
#ifndef _WIN32_WCE
  m_hOldPen = ::SelectObject(hTargetDC, createCurrentPen());
#else
  m_hOldPen = ::SelectObject(hTargetDC, ::CreatePen(m_dwPenStyle, m_dwWidth, m_penBrush.lbColor));
#endif
  m_oldLinetype  = kLtpSolid; // since we changed pen
  m_oldFillStyle = kFsSolid;  // since we changed brush
  m_oldEndStyle = OdPs::kLesRound; // since we reseted m_dwPenStyle
  m_oldJoinStyle = OdPs::kLjsRound; // since we reseted m_dwPenStyle

  m_bPenValid = m_bBrushValid = false;
  m_bThinPenValid = false; m_thinPenOverride = 0;

  LOGPEN lp = {PS_NULL, {0,0}, 0};
  m_hNullPen = ::CreatePenIndirect(&lp);
  lp.lopnStyle = PS_SOLID;
  m_hThinPen = ::CreatePenIndirect(&lp);
  LOGBRUSH lb = {BS_NULL, 0, 0};
  m_hNullBrush = ::CreateBrushIndirect(&lb);

  // required for plotstyles
  ::SetBkColor(hTargetDC, m_paletteColors[0]);
  
  m_oldBkMode = TRANSPARENT;
  ::SetBkMode(hTargetDC, m_oldBkMode);
}

void ExGiGDIGeometry::endPaint()
{
  if (m_hTargetDC) 
    ::DeleteObject(::SelectObject(m_hTargetDC, m_hOldPen));
  if(m_logBrush.lbStyle == BS_PATTERN)
    ::DeleteObject((HBITMAP)m_logBrush.lbHatch); // delete old bitmap
  if (m_hTargetDC) 
    ::DeleteObject(::SelectObject(m_hTargetDC, m_hOldBrush));
  ::DeleteObject(m_hNullPen);
  ::DeleteObject(m_hThinPen);
  ::DeleteObject(m_hNullBrush);
  if (m_hExPen) { ::DeleteObject(m_hExPen); m_hExPen = NULL; }
  m_hOldPen = m_hNullPen = m_hThinPen = 0;
  m_hOldBrush = m_hNullBrush = 0;
  m_hTargetDC = 0;
}

static void applyLwdPolylineHighlighting(HDC hdc, OdInt32 nPts, const OdGePoint3d* points, ODCOLORREF color)
{
  HGDIOBJ hOldPen = ::SelectObject(hdc, ::CreatePen(PS_DOT, 1, color));
  int x = OdTruncateToLong(points->x), y = OdTruncateToLong(points->y);
  ::MoveToEx(hdc, x, y, 0);
  ++points;
  while(--nPts)
  {
    x = OdTruncateToLong(points->x);
    y = OdTruncateToLong(points->y);
    ::LineTo(hdc, x, y);
    ++points;
  }
  ::DeleteObject(::SelectObject(hdc, hOldPen));
}

class GDIObjStack
{
  HDC     m_hdc;
  HGDIOBJ m_hOldObj;
  inline GDIObjStack(const GDIObjStack& /*data*/) {}
public:
  inline GDIObjStack()
    : m_hdc(0)
  { }
  inline GDIObjStack(HDC hdc, HGDIOBJ hObj)
    : m_hdc(0)
  {
    pushObject(hdc, hObj);
  }
  inline void pushObject(HDC hdc, HGDIOBJ hObj)
  {
    ODA_ASSERT(m_hdc==0);
    m_hdc = hdc;
    m_hOldObj = ::SelectObject(hdc, hObj);
  }
  inline void popObject()
  {
    if(m_hdc)
    {
      ::SelectObject(m_hdc, m_hOldObj);
      m_hdc = 0;
    }
  }
  inline ~GDIObjStack()
  {
    popObject();
  }
};

void ExGiGDIGeometry::polylineOut(OdInt32 nPts, const OdGePoint3d* points )
{
  if(nPts > 0)
  {
    HDC hdc = m_hTargetDC;
    if (!m_bExtLwdStyle || m_dwWidth <= 1 || nPts == 1)
    {
      int x = OdTruncateToLong(points->x), y = OdTruncateToLong(points->y);
      if(nPts > 1)
      {
        const OdInt32 nStore = nPts;

        ::MoveToEx(hdc, x, y, 0);
        ++points;
        while(--nPts)
        {
          x = OdTruncateToLong(points->x);
          y = OdTruncateToLong(points->y);
          ::LineTo(hdc, x, y);
          ++points;
        }
        if(!isExtPen())
        {
          // The last pixel of a line is not drawn with some GDI pens
          // so force draw last pixel of the last segment
          ::LineTo(hdc, ++x, y);
        }
        if (m_dwWidth > 1 && m_oldLinetype == kLtpForSelection)
          applyLwdPolylineHighlighting(hdc, nStore, points - nStore, m_paletteColors[0]);
      }
      else
      {
        if (!m_bPointsLwd || m_dwWidth <= 1)
        {
          ::SetPixel(hdc, x, y, m_penBrush.lbColor);
        }
        else
        {
          // Draw as filled circle
          ::MoveToEx(hdc, x, y, NULL); ::LineTo(hdc, x, y);
        }
      }
    }
    else
    {
      { // #9277 : extended lineweight styles support
        // Since we are output plot styles via plotstyle generator currently we could process only for extended lineweight styles.
        // Unfortunately WinGDI can't process all styles which we need. Plotstyle generator also cannot be reused, so make all
        // calculation ourselves.
#ifndef _WIN32_WCE
        if (m_lwdCapStyle != OdPs::kLesDiamond && m_lwdJoinStyle != OdPs::kLjsDiamond)
        {
          plotGeneratorPolylineOut(points, (OdUInt32)nPts, m_lwdCapStyle, m_lwdJoinStyle, (double)m_dwWidth);
        }
        else
        {
#endif
        HGDIOBJ itlThinPen = NULL;
        if (m_logBrush.lbColor != m_penBrush.lbColor)
          itlThinPen = (HGDIOBJ)::CreatePen(PS_SOLID, 1, m_logBrush.lbColor);
        HGDIOBJ prevPen = ::SelectObject(hdc, (itlThinPen) ? itlThinPen : thinPen());
        HGDIOBJ itlBrush = NULL, prevBrush = NULL;
        if ((m_logBrush.lbStyle != BS_SOLID) || m_logBrush.lbHatch)
        {
          itlBrush = (HGDIOBJ)::CreateSolidBrush(m_logBrush.lbColor);
          prevBrush = ::SelectObject(hdc, itlBrush);
        }
        class OdExGDIExtendedLineWeightStyleClient : public OdExExtendedLineWeightStyleClient
        {
          HDC hdc;
          HGDIOBJ capPen;
          public:
            OdExGDIExtendedLineWeightStyleClient(double m_lwd, OdPs::LineEndStyle capStyle, OdPs::LineJoinStyle joinStyle, HDC _hdc, HGDIOBJ hPen)
              : OdExExtendedLineWeightStyleClient(OdGeVector3d::kZAxis, m_lwd, capStyle, joinStyle), hdc(_hdc), capPen(hPen)
            {
            }
            virtual bool elsPolygonOut(const OdGePoint3d *pPoints, OdUInt32 nPoints)
            {
              POINT dcPoints[4];
              for (OdUInt32 n = 0; n < nPoints; n++)
              {
                dcPoints[n].x = OdTruncateToLong(pPoints[n].x);
                dcPoints[n].y = OdTruncateToLong(pPoints[n].y);
              }
              ::Polygon(hdc, dcPoints, (int)nPoints);
              return true;
            }
            virtual bool elsCircleOut(const OdGePoint3d &pPoint, const OdGeVector3d & /*normal*/, double /*radius*/)
            {
              GDIObjStack _PenStack(hdc, capPen);
              long x = OdTruncateToLong(pPoint.x), y = OdTruncateToLong(pPoint.y);
              ::MoveToEx(hdc, x, y, NULL); ::LineTo(hdc, x, y);
              return true;
            }
        } _exlwd_renderer((double)m_dwWidth, m_lwdCapStyle, m_lwdJoinStyle, hdc, prevPen);
        _exlwd_renderer.process(points, nPts);
        if (itlBrush)
          ::DeleteObject(::SelectObject(hdc, prevBrush));
        ::SelectObject(hdc, prevPen);
        if (itlThinPen)
          ::DeleteObject(itlThinPen);
      }
#ifndef _WIN32_WCE
      }
#endif
      if (m_oldLinetype == kLtpForSelection)
        applyLwdPolylineHighlighting(hdc, nPts, points, m_paletteColors[0]);
    }
  }
}

void ExGiGDIGeometry::polylineProc(OdInt32 nPts, const OdGePoint3d* points,
                                   const OdGeVector3d* ,
                                   const OdGeVector3d* pExtrusion,
                                   OdGsMarker /*lBaseSubEntMarker*/)
{
  if (nPts == 0)
    return;

  if(pExtrusion && ((OdGeVector2d*)pExtrusion)->lengthSqrd() > .25)
  {
    // setCurrentTraits(); -- must be on conveyor input

    if (m_bExtLwdStyle && m_dwWidth > 1)
    { // #9277 : extended lineweight styles support
      OdGiGeometrySimplifier::polylineProc(nPts, points, NULL, pExtrusion);
      return;
    }

    HDC hdc = m_hTargetDC;

    int X = OdTruncateToLong(points->x), Y = OdTruncateToLong(points->y);

    // Draw with extrusion
    int Xe = OdTruncateToLong(points->x + pExtrusion->x), Ye = OdTruncateToLong(points->y + pExtrusion->y);
    ::MoveToEx(hdc, X, Y, 0);
    ::LineTo(hdc, Xe, Ye);
    ++points;
    int X2, Y2, X2e, Y2e;
    for(OdInt32 i=1; i<nPts; ++i)
    {
      X2 = OdTruncateToLong(points->x);
      Y2 = OdTruncateToLong(points->y);
      X2e = OdTruncateToLong(points->x + pExtrusion->x);
      Y2e = OdTruncateToLong(points->y + pExtrusion->y);

      ::MoveToEx(hdc, X, Y, 0);
      ::LineTo(hdc, X2, Y2);
      ::LineTo(hdc, X2e, Y2e);
      ::LineTo(hdc, Xe, Ye);

      X  = X2;
      Y  = Y2;
      Xe = X2e;
      Ye = Y2e;

      ++points;
    }
  }
  else
  {
    ExGiGDIGeometry::polylineOut( nPts, points );
  }
}

#ifndef _WIN32_WCE
bool ExGiGDIGeometry::plotGeneratorPolylineOut(const OdGePoint3d *pPoints, OdUInt32 nPoints,
  OdPs::LineEndStyle capStyle, OdPs::LineJoinStyle joinStyle, double fLwd)
{
  DWORD dwPenStyle = PS_GEOMETRIC | PS_SOLID;
  DWORD dwWidth = (DWORD)OdRoundToLong(fLwd);
  switch (capStyle)
  {
    case OdPs::kLesButt: dwPenStyle |= PS_ENDCAP_FLAT; break;
    case OdPs::kLesSquare: dwPenStyle |= PS_ENDCAP_SQUARE; break;
    default: dwPenStyle |= PS_ENDCAP_ROUND;
  }
  switch (joinStyle)
  {
    case OdPs::kLjsMiter: dwPenStyle |= PS_JOIN_MITER; break;
    case OdPs::kLjsBevel: dwPenStyle |= PS_JOIN_BEVEL; break;
    default: dwPenStyle |= PS_JOIN_ROUND;
  }
  if (m_hExPen)
  {
    EXTLOGPEN prevSets;
    if (::GetObject((HANDLE)m_hExPen, sizeof(EXTLOGPEN), (LPVOID)&prevSets) > 0)
    {
      if (prevSets.elpColor != m_penBrush.lbColor ||
          prevSets.elpPenStyle != dwPenStyle ||
          prevSets.elpWidth != dwWidth)
      {
        ::DeleteObject((HGDIOBJ)m_hExPen);
        m_hExPen = NULL;
      }
    }
    else
    {
      ::DeleteObject((HGDIOBJ)m_hExPen);
      m_hExPen = NULL;
    }
  }
  if (!m_hExPen)
  {
    LOGBRUSH lbStyle = { BS_SOLID, m_penBrush.lbColor, NULL };
    m_hExPen = ::ExtCreatePen(dwPenStyle, dwWidth, &lbStyle, 0, NULL);
  }
  if (nPoints > 0)
  {
    HDC hdc = m_hTargetDC;
    GDIObjStack _pen(hdc, (HGDIOBJ)m_hExPen);
    if (nPoints > 1)
    {
      m_dcPoints.resize(nPoints);
      POINT *pOut = m_dcPoints.asArrayPtr();
      for (OdUInt32 nPoint = 0; nPoint < nPoints; nPoint++)
      {
        pOut[nPoint].x = OdTruncateToLong(pPoints[nPoint].x);
        pOut[nPoint].y = OdTruncateToLong(pPoints[nPoint].y);
      }
      ::Polyline(hdc, pOut, (int)nPoints);
    }
    else
    {
      // Draw as filled circle
      int x = OdTruncateToLong(pPoints->x), y = OdTruncateToLong(pPoints->y);
      ::MoveToEx(hdc, x, y, NULL); ::LineTo(hdc, x, y);
    }
  }
  return true;
}
#else
bool ExGiGDIGeometry::plotGeneratorPolylineOut(const OdGePoint3d * /*pPoints*/, OdUInt32 /*nPoints*/,
  OdPs::LineEndStyle /*capStyle*/, OdPs::LineJoinStyle /*joinStyle*/, double /*fLwd*/) { return false; }
#endif

/* some experiments...
#include "Ge/GeEllipArc3d.h"
#include "Ge/GePlane.h"

void ExGiGDIGeometry::ellipArcProc(const OdGeEllipArc3d& ellipArc,
                                   const OdGePoint3d* pEndPointsOverrides,
                                   OdGiArcType arcType,
                                   const OdGeVector3d* pExtrusion)
{
  OdGiGeometrySimplifier::ellipArcProc(ellipArc, pEndPointsOverrides, arcType, pExtrusion);

  OdGeEllipArc3d* pEllipse = (OdGeEllipArc3d*)ellipArc.project(OdGePlane(OdGePoint3d::kOrigin, OdGeVector3d::kZAxis), OdGeVector3d::kZAxis);
  OdGePoint3d min = pEllipse->center(), max = min;
  min.x -= pEllipse->majorRadius();
  min.y -= pEllipse->minorRadius();
  max.x += pEllipse->majorRadius();
  max.y += pEllipse->minorRadius();
  delete pEllipse;
  ::Ellipse(m_hTargetDC, OdRoundToLong(min.x), OdRoundToLong(min.y), OdRoundToLong(max.x), OdRoundToLong(max.y));
}
*/

// Fast solution for polygons with 3-4 vertices without holes
static bool odCheckPolygonVisibility(const OdInt32* pFaceList, OdInt32 nPts, const OdGePoint3d *pPoints)
{
  class FaceListAccessor {
    const OdInt32* m_pFaceList;
    public:
      explicit FaceListAccessor(const OdInt32* pFaceList) : m_pFaceList(pFaceList) { }
      void operator ++(int) { if (m_pFaceList) m_pFaceList++; }
      OdInt32 operator [](OdInt32 n) { if (m_pFaceList) return m_pFaceList[n]; else return n; }
  } faceList(pFaceList);
  faceList++; // Need only outer contour for check
  for (OdInt32 nPt = 1; nPt <= nPts; nPt++)
  {
    // Compute square length for 1st long axis (doesn't need sqrt since 1 is 1 in square length too)
    OdGeVector3d axis = pPoints[faceList[nPt % nPts]] - pPoints[faceList[nPt - 1]];
    const double len1 = axis.x * axis.x + axis.y * axis.y;
    if (len1 >= 1.0)
    { // 1st long axis found, compute perpendicular axis
      axis.z = 0.0; // normalize
      const OdGeVector3d axis2 = (axis / sqrt(len1)).perpVector(); // perp is Ok here since works in 2d
      for (OdInt32 nPt2 = nPt + 1; nPt2 <= nPts; nPt2++)
      {
        // Check length by secondary axis
        const double len2 = (pPoints[faceList[nPt2 % nPts]] - pPoints[faceList[nPt2 - 1]]).dotProduct(axis2);
        // Polygon is visible if secondary axis have >= 1px length too
        if ((len2 >= 1.0) || (len2 <= -1.0))
          return true;
      }
      // Polygon is invisible if have only one long axis.
      return false;
    }
  }
  // Polygon which is < 1px for both axes. This is haven't sense to draw lines for it since it is will cause 1 pixel
  // on screen.
  return true;
}

// Skeleton-based polygon topology analysis (any type of polygons, even if it contain holes)
class OdPolygonVisibilityChecker : public OdRxObject
{
  double m_fPolygonOffset;
  struct PolyStats
  {
    OdUInt32 m_nContours;
    OdUInt32 m_nHoles;
    OdUInt32 m_nIslands;
    OdUInt32 m_nFirstPolyVerts;
    void reset()
    {
      m_nContours = m_nHoles = m_nIslands = m_nFirstPolyVerts = 0;
    }
  } m_stats;
  struct DataProc
  {
    OdGePoint2dArray m_skeleton;
    OdGePoint2dArray m_holeSkeleton;
  } m_data;
  protected:
    bool isCoincident(const OdGePoint2d &a, const OdGePoint2d &b) const
    {
      return a.isEqualTo(b);
    }
    // nFrom should be set to segment b point, so can't be set to 0
    OdInt32 nextSegment(OdInt32 nFrom, const OdInt32* pFaceList, OdInt32 nPts, const OdGePoint3d *pPoints) const
    {
      while (nFrom < nPts)
      {
        if (!isCoincident(pPoints[pFaceList[nFrom - 1]].convert2d(), pPoints[pFaceList[nFrom]].convert2d()))
          return nFrom;
        nFrom++;
      }
      return -1;
    }
    OdInt32 nextSegment(OdInt32 nFrom, OdInt32 nPts, const OdGePoint3d *pPoints) const
    {
      while (nFrom < nPts)
      {
        if (!isCoincident(pPoints[nFrom - 1].convert2d(), pPoints[nFrom].convert2d()))
          return nFrom;
        nFrom++;
      }
      return -1;
    }
    bool intersect(const OdGePoint2d &a, const OdGePoint2d &b, const OdGePoint2d &c, const OdGePoint2d &d, OdGePoint2d *pOut = NULL) const
    {
      double Ax, Bx, Cx, Ay, By, Cy, df, ef, ff, x1lo, x1hi, y1lo, y1hi;
      Ax = b.x - a.x; Bx = c.x - d.x;
      if (Ax < 0.0) { x1lo = b.x; x1hi = a.x; }
      else { x1hi = b.x; x1lo = a.x; }
      if (Bx > 0.0) { if (x1hi < d.x || c.x < x1lo) return false; }
      else { if (x1hi < c.x || d.x < x1lo) return false; }
      Ay = b.y - a.y; By = c.y - d.y;
      if (Ay < 0.0) { y1lo = b.y; y1hi = a.y; }
      else { y1hi = b.y; y1lo = a.y; }
      if (By > 0.0) { if (y1hi < d.y || c.y < y1lo) return false; }
      else { if (y1hi < c.y || d.y < y1lo) return false; }
      Cx = a.x - c.x; Cy = a.y - c.y;
      df = By * Cx - Bx * Cy; ff = Ay * Bx - Ax * By;
      if (ff > 0.0) { if (df < 0.0 || df > ff) return false; }
      else { if (df > 0.0 || df < ff) return false; }
      ef = Ax * Cy - Ay * Cx;
      if (ff > 0.0) { if (ef < 0.0 || ef > ff) return false; }
      else { if (ef > 0.0 || ef < ff) return false; }
      if (pOut)
      {
        if (OdZero(ff, 1.e-300)) return false; // parallel line segments. can't calculate intersection point
        pOut->x = a.x + (df * Ax) / ff;
        pOut->y = a.y + (df * Ay) / ff;
      }
      return true;
    }
    template <class PtClass>
    double getPolygonArea2d_itl(const PtClass* pPoints, OdUInt32 nbPoints, const OdInt32 *pList) const
    {
      double area = 0.0;
      if (nbPoints == 0) return area;
      for (OdUInt32 i = 0; i < nbPoints; i++) {
        OdUInt32 j = i + 1;
        if (j == nbPoints)
          j = 0;
        const PtClass *p1 = (!pList) ? (pPoints + i) : (pPoints + pList[i]);
        const PtClass *p2 = (!pList) ? (pPoints + j) : (pPoints + pList[j]);
        area += p1->x * p2->y;
        area -= p1->y * p2->x;
      }
      return area * 0.5;
    }
    double getPolygonArea2d(const OdGePoint2d* pPoints, OdUInt32 nbPoints) const
    {
      return getPolygonArea2d_itl<OdGePoint2d>(pPoints, nbPoints, NULL);
    }
    double getPolygonArea2d(const OdGePoint3d* pPoints, OdUInt32 nbPoints, const OdInt32 *pList) const
    {
      return getPolygonArea2d_itl<OdGePoint3d>(pPoints, nbPoints, pList);
    }
    bool isInside(const OdGePoint2d &pt) const
    {
      const OdUInt32 nPts = m_data.m_skeleton.size();
      const OdGePoint2d *pPts = m_data.m_skeleton.getPtr();
      OdUInt32 tickTack = 0;
      for (OdUInt32 i = 0; i < nPts; i++)
      {
        OdUInt32 j = i + 1;
        if (j == nPts) j = 0;
        const OdGePoint2d *pti = pPts + i,
                          *ptj = pPts + j;
        if ((((pti->x <= pt.x) && (pt.x < ptj->x)) ||
            ((ptj->x <= pt.x) && (pt.x < pti->x))) &&
             (pt.y < (ptj->y - pti->y) * (pt.x - pti->x) /
             (ptj->x - pti->x) + pti->y))
          tickTack = !tickTack;
      }
      return !!tickTack;
    }
    bool appendSkeletonPoint(const OdGePoint2d &seg1A, const OdGePoint2d &seg1B,
                             const OdGePoint2d &seg2A, const OdGePoint2d &seg2B,
                             bool bPolygonCW, bool bHole, bool bLastSeg = false)
    {
      const OdGeVector2d seg1 = (seg1B - seg1A).normal();
      OdGeVector2d perVec1 = seg1.perpVector(),
                   perVec2 = (seg2B - seg2A).perpVector().normal(); 
      perVec1 += perVec2;
      if (perVec1.isZeroLength())
        perVec1 = -perVec2; // Opposite directions case
      perVec1.normalize();
      // Calculate miter
      double ang = seg1.crossProduct(perVec1);
      if (OdNonZero(ang)) perVec1 *= 1.0 / ang;
      if (!bPolygonCW) perVec1.negate();
      const OdGePoint2d skelPt = seg2A + perVec1 * m_fPolygonOffset;
      if (!bHole)
      { // check intersections
        const OdUInt32 nPts = m_data.m_skeleton.size();
        const OdGePoint2d *pPts = m_data.m_skeleton.getPtr();
        for (OdUInt32 nPt = 1; nPt < nPts; nPt++)
        {
          // check that new segment doesn't intersect with exist segments (self-intersection check)
          // don't check with last segment since them connecting
          if ((nPt != (nPts - 1)) && intersect(pPts[nPt - 1], pPts[nPt], pPts[nPts - 1], skelPt))
            return false;
          // check that skeleton offset doesn't intersect with exist segments (quad case)
          if (intersect(pPts[nPt - 1], pPts[nPt], seg2A, skelPt))
            return false;
        }
        m_data.m_skeleton.append(skelPt);
        if (bLastSeg)
        {
          // Last segment must be checked too
          const OdUInt32 nPts = m_data.m_skeleton.size();
          const OdGePoint2d *pPts = m_data.m_skeleton.getPtr();
          // don't check with first and last segments since them connecting
          for (OdUInt32 nPt = 2; nPt < nPts - 1; nPt++)
          {
            // check that new segment doesn't intersect with exist segments (self-intersection check)
            if (intersect(pPts[nPt - 1], pPts[nPt], skelPt, pPts[0]))
              return false;
          }
        }
      }
      else
      {
        if (!isInside(skelPt))
          return false;
        m_data.m_holeSkeleton.push_back(skelPt);
        if (m_data.m_holeSkeleton.size() > 1)
        {
          const OdUInt32 nPts = m_data.m_skeleton.size();
          const OdGePoint2d *pPts = m_data.m_skeleton.getPtr();
          const OdUInt32 nPtsH = m_data.m_holeSkeleton.size();
          const OdGePoint2d *pPtsH = m_data.m_holeSkeleton.getPtr();
          // check that island skeleton doesn't intersects with hole skeleton
          for (OdUInt32 nPt = 1; nPt <= nPts; nPt++)
          {
            const OdUInt32 nPt0 = nPt - 1;
            const OdUInt32 nPt1 = (nPt != nPts) ? nPt : 0;
            if (intersect(pPts[nPt0], pPts[nPt1], pPtsH[nPtsH - 2], pPtsH[nPtsH - 1]))
              return false;
            if (bLastSeg)
            {
              if (intersect(pPts[nPt0], pPts[nPt1], pPtsH[nPtsH - 1], pPtsH[0]))
                return false;
            }
          }
        }
      }
      return true;
    }
    bool generateSkeleton(const OdInt32* pFaceList, OdInt32 nPts, const OdGePoint3d *pPoints, bool bPolygonCW, bool bHole)
    {
      if (!bHole)
      {
        m_data.m_skeleton.clear();
        m_data.m_skeleton.reserve((OdUInt32)nPts);
      }
      else
        m_data.m_holeSkeleton.clear();
      if (pFaceList)
      {
        // Find first curve winding
        OdInt32 nSeg = 1;
        OdGePoint2d segFA = pPoints[pFaceList[nPts - 1]].convert2d(), seg1A,
                    segFB = pPoints[pFaceList[0]].convert2d(), seg1B;
        if (isCoincident(segFA, segFB))
        {
          nSeg = nextSegment(nSeg, pFaceList, nPts, pPoints);
          if (nSeg == -1) // Degenerate polygon, need be outlined
            return false;
          segFA = pPoints[pFaceList[nSeg - 1]].convert2d();
          segFB = pPoints[pFaceList[nSeg]].convert2d();
        }
        nSeg = nextSegment(nSeg + 1, pFaceList, nPts, pPoints);
        if (nSeg == -1) // Degenerate polygon, need be outlined
          return false;
        OdGePoint2d seg2A = pPoints[pFaceList[nSeg - 1]].convert2d(),
                    seg2B = pPoints[pFaceList[nSeg]].convert2d();
        appendSkeletonPoint(segFA, segFB, seg2A, seg2B, bPolygonCW, bHole);
        while ((nSeg = nextSegment(nSeg + 1, pFaceList, nPts, pPoints)) != -1)
        {
          seg1A = seg2A; seg1B = seg2B;
          seg2A = pPoints[pFaceList[nSeg - 1]].convert2d();
          seg2B = pPoints[pFaceList[nSeg]].convert2d();
          if (!appendSkeletonPoint(seg1A, seg1B, seg2A, seg2B, bPolygonCW, bHole))
            return false; // Self-intersections exist
        }
        if (!appendSkeletonPoint(seg2A, seg2B, segFA, segFB, bPolygonCW, bHole, true))
          return false; // Self-intersections exist
      }
      else
      {
        // Find first curve winding
        OdInt32 nSeg = 1;
        OdGePoint2d segFA = pPoints[nPts - 1].convert2d(), seg1A,
                    segFB = pPoints[0].convert2d(), seg1B;
        if (isCoincident(segFA, segFB))
        {
          nSeg = nextSegment(nSeg, nPts, pPoints);
          if (nSeg == -1) // Degenerate polygon, need be outlined
            return false;
          segFA = pPoints[nSeg - 1].convert2d();
          segFB = pPoints[nSeg].convert2d();
        }
        nSeg = nextSegment(nSeg + 1, nPts, pPoints);
        if (nSeg == -1) // Degenerate polygon, need be outlined
          return false;
        OdGePoint2d seg2A = pPoints[nSeg - 1].convert2d(),
                    seg2B = pPoints[nSeg].convert2d();
        appendSkeletonPoint(segFA, segFB, seg2A, seg2B, bPolygonCW, bHole);
        while ((nSeg = nextSegment(nSeg + 1, nPts, pPoints)) != -1)
        {
          seg1A = seg2A; seg1B = seg2B;
          seg2A = pPoints[nSeg - 1].convert2d();
          seg2B = pPoints[nSeg].convert2d();
          if (!appendSkeletonPoint(seg1A, seg1B, seg2A, seg2B, bPolygonCW, bHole))
            return false; // Self-intersections exist
        }
        if (!appendSkeletonPoint(seg2A, seg2B, segFA, segFB, bPolygonCW, bHole, true))
          return false; // Self-intersections exist
      }
      return true;
    }
  public:
    OdPolygonVisibilityChecker(double fPolygonOffset = 0.55)
      : m_fPolygonOffset(fPolygonOffset) { }
    bool check(OdInt32 nFaceList, const OdInt32* pFaceList, OdInt32 nPts, const OdGePoint3d *pPoints, bool bForceSkeleton = false)
    {
      // compute statistics
      m_stats.reset();
      if (pFaceList)
      {
        const OdInt32 *pFace = pFaceList, *pFaceLast = pFaceList + nFaceList;
        while (pFace < pFaceLast)
        {
          m_stats.m_nContours++;
          if (*pFace < 0)
          {
            m_stats.m_nHoles++;
            pFace += -*pFace;
          }
          else
          {
            m_stats.m_nIslands++;
            pFace += *pFace;
          }
          pFace++;
        }
        m_stats.m_nFirstPolyVerts = *pFaceList;
      }
      else
      {
        m_stats.m_nContours = m_stats.m_nIslands = 1;
        m_stats.m_nFirstPolyVerts = (OdUInt32)nPts;
      }
      // Probably move through simplified solution
      if (!bForceSkeleton && (m_stats.m_nContours == 1) && (m_stats.m_nFirstPolyVerts < 5))
        return odCheckPolygonVisibility(pFaceList, m_stats.m_nFirstPolyVerts, pPoints);
      // Run analysis
      OdUInt32 nContour = 0;
      do
      {
        double polygonArea = getPolygonArea2d(pPoints, (pFaceList) ? *pFaceList : nPts, (pFaceList) ? (pFaceList + 1) : NULL);
        bool bPolygonCW = !(polygonArea < 0.0);
        if (!bPolygonCW)
          polygonArea = -polygonArea;
        { // Build skeleton for island
          if (pFaceList)
          {
            nPts = *pFaceList;
            pFaceList++;
          }
          if (!generateSkeleton(pFaceList, nPts, pPoints, bPolygonCW, false))
            return false;
        }
        if (m_data.m_skeleton.size() < 3)
          return false; // Degenerate polygon, need be outlined
        // check skeleton correctness
        double skeletonArea = getPolygonArea2d(m_data.m_skeleton.getPtr(), m_data.m_skeleton.size());
        if (!bPolygonCW)
          skeletonArea = -skeletonArea;
        if ((skeletonArea <= 0.0) || (skeletonArea > polygonArea))
          return false;
        // check holes if present
        nContour++;
        if (pFaceList)
        {
          pFaceList += nPts;
          if (m_stats.m_nHoles)
          {
            while ((nContour < m_stats.m_nContours) && (*pFaceList < 0))
            {
              nPts = -*pFaceList;
              pFaceList++;
              if (!generateSkeleton(pFaceList, nPts, pPoints, bPolygonCW, true))
                return false;
              pFaceList += nPts;
              ++nContour;
            }
          }
        }
      } while (nContour < m_stats.m_nContours);
      return true;
    }
#ifdef _DEBUG
    void drawSkeleton(HDC hDc, COLORREF clr)
    {
      HPEN tmpPen = ::CreatePen(PS_SOLID, 1, clr);
      HGDIOBJ prevPen = ::SelectObject(hDc, (HGDIOBJ)tmpPen);
      if (m_data.m_skeleton.size())
        ::MoveToEx(hDc, (int)m_data.m_skeleton.first().x, (int)m_data.m_skeleton.first().y, NULL);
      for (OdUInt32 n = 1; n < m_data.m_skeleton.size(); n++)
        ::LineTo(hDc, (int)m_data.m_skeleton[n].x, (int)m_data.m_skeleton[n].y);
      if (m_data.m_skeleton.size())
        ::LineTo(hDc, (int)m_data.m_skeleton.first().x, (int)m_data.m_skeleton.first().y);
      ::SelectObject(hDc, prevPen);
      ::DeleteObject((HGDIOBJ)tmpPen);
    }
#endif
    static OdRxObjectPtr createMe()
    {
      return OdRxObjectPtr(OdRxObjectImpl<OdPolygonVisibilityChecker>::createObject());
    }
    static OdPolygonVisibilityChecker *getAt(OdRxObjectPtr &pPVC)
    {
      if (pPVC.isNull())
        pPVC = createMe();
      return static_cast<OdPolygonVisibilityChecker*>(pPVC.get());
    }
    void resetOffset(double fOffset) { m_fPolygonOffset = fOffset; }
};

inline void toDevice(const OdGePoint3d& point, POINT& dcPoint)
{
  dcPoint.x = OdTruncateToLong(point.x);
  dcPoint.y = OdTruncateToLong(point.y);
}

template <class Iterator>
inline void toDevice(OdUInt32 nPoints, Iterator p3dPoints, POINTArray& dcPoints)
{
  dcPoints.resize(nPoints);
  POINT* pPoint = dcPoints.asArrayPtr();
  for(OdUInt32 i=0; i<nPoints; ++i, ++pPoint, ++p3dPoints)
  {
    pPoint->x = OdTruncateToLong(p3dPoints->x);
    pPoint->y = OdTruncateToLong(p3dPoints->y);
  }
}

void ExGiGDIGeometry::polygonOut(OdInt32 nPts, const OdGePoint3d* points, const OdGeVector3d*)
{
  GDIObjStack _GDIObjStack;
  if (giCtx().isPlotGeneration() && (m_bProcessingTTF || m_dwWidth <= 1))
  {
    // #8743: we doesn't need any contours during printing
    OdPolygonVisibilityChecker *pPVC =  OdPolygonVisibilityChecker::getAt(m_pPVC);
    if (pPVC->check(0, NULL, nPts, points))
      _GDIObjStack.pushObject(m_hTargetDC, nullPen());
    else if (ttfProcessingPenOverride())
      _GDIObjStack.pushObject(m_hTargetDC, thinPen());
  }
  else if (m_bProcessingTTF)
  {
    if (ttfProcessingPenOverride())
      _GDIObjStack.pushObject(m_hTargetDC, thinPen());
  }
  else if (m_oldFillStyle != kFsSolid && m_oldFillStyle != kFsHollow)
  {
    OdPolygonVisibilityChecker *pPVC = OdPolygonVisibilityChecker::getAt(m_pPVC);
    if (pPVC->check(0, NULL, nPts, points))
      _GDIObjStack.pushObject(m_hTargetDC, nullPen());
  }
  // #9277 : extended lineweight styles support
  else if (m_bExtLwdStyle && m_dwWidth > 1)
  {
    OdGePoint3dArray tmpArray(nPts + 1, 1);
    tmpArray.insert(tmpArray.end(), points, points + nPts);
    tmpArray.push_back(*points);
    polylineOut(nPts + 1, tmpArray.getPtr());
    _GDIObjStack.pushObject(m_hTargetDC, nullPen());
  }

  ::toDevice(nPts, points, m_dcPoints);
  ::Polygon(m_hTargetDC, m_dcPoints.getPtr(), nPts);
}

void ExGiGDIGeometry::startThinPenOverride()
{
  if (m_thinPenOverride != m_dwWidth)
  {
    m_thinPenOverride = m_dwWidth;
    m_dwWidth = 1;
    m_bPenValid = false;
    updatePenNBrush(m_bDrawSelection);
  }
}

void ExGiGDIGeometry::endThinPenOverride()
{
  if (m_thinPenOverride)
  {
    m_dwWidth = m_thinPenOverride;
    m_thinPenOverride = 0;
    m_bPenValid = false;
    updatePenNBrush(m_bDrawSelection);
  }
}

class ZeroLineweightSaver
{
  protected:
    ExGiGDIGeometry *m_pGeom;
  public:
    ZeroLineweightSaver(bool bExpression, ExGiGDIGeometry *pThis)
      : m_pGeom(NULL)
    {
      if (bExpression)
      {
        pThis->startThinPenOverride();
        m_pGeom = pThis;
      }
    }
    ~ZeroLineweightSaver()
    {
      if (m_pGeom)
        m_pGeom->endThinPenOverride();
    }
};

class PolygonFillModeOverrider : public ZeroLineweightSaver
{ // #CORE-12928
  protected:
    OdGiSubEntityTraits *m_pTraits;
  public:
    PolygonFillModeOverrider(bool bExpression, bool bSimplFlagCheck, ExGiGDIGeometry *pThis, OdGiSubEntityTraits *pTraits)
      : ZeroLineweightSaver(bExpression, pThis)
      , m_pTraits(NULL)
    {
      if (bSimplFlagCheck && pTraits)
      {
        const bool bFillCheck = !GETBIT(pTraits->drawFlags(), OdGiSubEntityTraits::kDisableFillModeCheck);
        if (bFillCheck && (pTraits->fillType() == kOdGiFillAlways))
        {
          pTraits->setFillType(kOdGiFillNever);
          m_pTraits = pTraits;
        }
        else if (!bFillCheck)
          pThis->drawContext()->onTraitsModified();
        if (m_pTraits) // Update brush style
          pThis->drawContext()->onTraitsModified();
      }
    }
    ~PolygonFillModeOverrider()
    { if (m_pTraits) m_pTraits->setFillType(kOdGiFillAlways); }
};

void ExGiGDIGeometry::polygonProc(OdInt32 nbPoints, const OdGePoint3d* pVertexList,
                                  const OdGeVector3d* pNormal, const OdGeVector3d* pExtrusion)
{
  bool bEdges = false;
  PolygonFillModeOverrider _ls(m_dwWidth > 1 && (fillMode(bEdges) && !bEdges), GETBIT(m_simplFlags, kSimplFillModeDisabled), this, m_pTraits);
  OdGiGeometrySimplifier::polygonProc(nbPoints, pVertexList, pNormal, pExtrusion);
}

void ExGiGDIGeometry::generateShellFaces(OdInt32 faceListSize, const OdInt32* pFaceList,
                                         const OdGiEdgeData* pEdgeData, const OdGiFaceData* pFaceData)
{
  ZeroLineweightSaver _ls(m_dwWidth > 1 && !m_bProcessingTTF, this);
  OdGiGeometrySimplifier::generateShellFaces(faceListSize, pFaceList, pEdgeData, pFaceData);
}

void ExGiGDIGeometry::shellFaceOut(OdInt32 faceListSize, const OdInt32* pFaceList, const OdGeVector3d* pNormal)
{
  ODA_ASSERT(*pFaceList > 0);

#ifndef _WIN32_WCE
  if(m_triVertexes.size()) // gradient
  {
    OdGiGeometrySimplifier::shellFaceOut(faceListSize, pFaceList, pNormal);
    return;
  }
#endif

  GDIObjStack _GDIObjStack;
  if (giCtx().isPlotGeneration() && (m_bProcessingTTF || m_dwWidth <= 1))
  {
    // #8743: we doesn't need any contours during printing
    OdPolygonVisibilityChecker *pPVC = OdPolygonVisibilityChecker::getAt(m_pPVC);
    if (pPVC->check(faceListSize, pFaceList, vertexDataCount(), vertexDataList()))
      _GDIObjStack.pushObject(m_hTargetDC, nullPen());
    else if (ttfProcessingPenOverride())
      _GDIObjStack.pushObject(m_hTargetDC, thinPen());
  }
  else if (m_bProcessingTTF)
  {
    if (ttfProcessingPenOverride())
      _GDIObjStack.pushObject(m_hTargetDC, thinPen());
  }
  else
  if(m_oldFillStyle != kFsSolid && m_oldFillStyle != kFsHollow)
  {
    OdPolygonVisibilityChecker *pPVC = OdPolygonVisibilityChecker::getAt(m_pPVC);
    if (pPVC->check(faceListSize, pFaceList, vertexDataCount(), vertexDataList()))
      _GDIObjStack.pushObject(m_hTargetDC, nullPen());
  }

  m_dcPoints.reserve(faceListSize - 1);
  OdGiShellFaceIterator pVertex(vertexDataList(), pFaceList+1);
  const OdInt32* pFaceListEnd = pFaceList+faceListSize;

  ::toDevice(*pFaceList, pVertex, m_dcPoints);
  pVertex += *pFaceList;
  if( pVertex < pFaceListEnd ) // has holes
  {
    ODA_ASSUME(sizeof(OdInt32)==sizeof(int));
    OdInt32Array counts;
    OdInt32 count;
    counts.append(count = *pFaceList);
    do
    {
      counts.append(count = -*pVertex.faceListPos());
      ODA_ASSERT(count > 0);
      ++pVertex; // skip loop counter
      while(count--)
      {
        ::toDevice(*pVertex, *m_dcPoints.append());
        ++pVertex;
      }
    }
    while( pVertex < pFaceListEnd );

#ifndef _WIN32_WCE
    ::PolyPolygon(m_hTargetDC, m_dcPoints.getPtr(), (int*)counts.getPtr(), counts.size());
#else
    CDC *pDc = CDC::FromHandle(m_hTargetDC);
    pDc->PolyPolygon( (struct tagPOINT *)m_dcPoints.getPtr(), (int*)counts.getPtr(), counts.size());
#endif
  }
  else // no holes
  {
    ::Polygon(m_hTargetDC, m_dcPoints.getPtr(), *pFaceList);
  }
#ifdef SKELETON_DEBUG
  if (!m_pPVC.isNull())
  {
    OdPolygonVisibilityChecker *pPVC = OdPolygonVisibilityChecker::getAt(m_pPVC);
    if (pPVC->check(faceListSize, pFaceList, vertexDataCount(), vertexDataList(), true))
      pPVC->drawSkeleton(m_hTargetDC, RGB(255, 0, 0));
    else
      pPVC->drawSkeleton(m_hTargetDC, RGB(0, 0, 255));
  }
#endif
}

void ExGiGDIGeometry::ttfPolyDrawProc(OdInt32 nbVertex, const OdGePoint3d* pVertexList,
                                      OdInt32 faceListSize, const OdInt32* pFaceList,
                                      const OdUInt8* pBezierTypes, const OdGiFaceData* pFaceData)
{
#ifdef _WIN32_WCE
  OdGiConveyorGeometry::ttfPolyDrawProc(nbVertex, pVertexList, faceListSize, pFaceList, pBezierTypes, pFaceData);
#else
  if (::GetDeviceCaps(m_hTargetDC, TECHNOLOGY) == DT_METAFILE)
  {
    // Old Windows metafiles don't support GDI paths, so goto shell branch
    OdGiConveyorGeometry::ttfPolyDrawProc(nbVertex, pVertexList, faceListSize, pFaceList, pBezierTypes, pFaceData);
  }
  else
  {
    GDIObjStack _GDIObjStack;
    if (ttfProcessingPenOverride())
      _GDIObjStack.pushObject(m_hTargetDC, thinPen());

    ::toDevice(nbVertex, pVertexList, m_dcPoints);

    int prevFillMode = ::GetPolyFillMode(m_hTargetDC);
    if (prevFillMode != WINDING)
      ::SetPolyFillMode(m_hTargetDC, WINDING);
    ::BeginPath(m_hTargetDC);
    ::PolyDraw(m_hTargetDC, m_dcPoints.getPtr(), (const BYTE*)pBezierTypes, nbVertex);
    ::EndPath(m_hTargetDC);
    ::StrokeAndFillPath(m_hTargetDC);
    if (prevFillMode != WINDING)
      ::SetPolyFillMode(m_hTargetDC, prevFillMode);
  }
#endif
}

// MKU 16.03.2004   I guess these are outdated calls
/*
inline void dcLine(HDC hdc, const OdGePoint3d* points)
{
  ::MoveToEx(hdc, OdRoundToLong(points->x), OdRoundToLong(points->y), 0);
  ++points;
  ::LineTo(hdc, OdRoundToLong(points->x), OdRoundToLong(points->y));
}

inline void dcLine(HDC hdc, const OdGePoint3d& point1, const OdGePoint3d& point2)
{
  ::MoveToEx(hdc, OdRoundToLong(point1.x), OdRoundToLong(point1.y), 0);
  ::LineTo(hdc, OdRoundToLong(point2.x), OdRoundToLong(point2.y));
}
*/

void ExGiGDIGeometry::meshProc(OdInt32 rows, OdInt32 columns,
                               const OdGePoint3d* pVertexList,
                               const OdGiEdgeData* pEdgeData,
                               const OdGiFaceData* pFaceData,
                               const OdGiVertexData* pVertexData)
{
#ifndef _WIN32_WCE
  // Gradient filling
  if( OdGiGeometrySimplifier::fillMode() )
  {
    if( pVertexData && pVertexData->trueColors() )
    {
      if(beginGradientFill(rows * columns, pVertexList, pVertexData))
      {
        // setCurrentTraits(); -- must be on conveyor input
        OdGiGeometrySimplifier::meshProc(rows, columns, pVertexList, pEdgeData, pFaceData, pVertexData);
        endGradientFill(m_hTargetDC);
        return ;
      }
    }
  }
#endif
  OdGiGeometrySimplifier::meshProc(rows, columns, pVertexList, pEdgeData, pFaceData, pVertexData);
}

void ExGiGDIGeometry::shellProc(OdInt32 nbVertex,
                                const OdGePoint3d* pVertexList,
                                OdInt32 faceListSize,
                                const OdInt32* pFaceList,
                                const OdGiEdgeData* pEdgeData,
                                const OdGiFaceData* pFaceData,
                                const OdGiVertexData* pVertexData)
{
#ifndef _WIN32_WCE
  // Gradient filling
  if( OdGiGeometrySimplifier::fillMode() && !isSelection() && !isDragging())
  {
    if( pVertexData && pVertexData->trueColors() )
    {
      if(beginGradientFill(nbVertex, pVertexList, pVertexData))
      {
        // setCurrentTraits(); -- must be on conveyor input
        OdGiGeometrySimplifier::shellProc(nbVertex, pVertexList, faceListSize,
          pFaceList, pEdgeData, pFaceData, pVertexData);

        endGradientFill(m_hTargetDC);
        return ;
      }
    }
  }
#endif
  OdGiGeometrySimplifier::shellProc(nbVertex, pVertexList, faceListSize, pFaceList,
                                    pEdgeData, pFaceData, pVertexData);
}

#if !defined(_WIN32_WCE)
#define MIN_SIZE_TTF_TEXT 5

bool getLogFont(const OdGeVector2d& xDir, const OdGeVector2d& yDir, const OdGiTextStyle& style, bool twisted, 
                LOGFONT* lFont, HDC hDC, const OdGiContext *pCtx)
{
  int ang = -OdRoundToLong(xDir.angle() / OdaPI * 180. * 10.);
  LONG lfHeight = OdRoundToLong(style.getFont()->getHeight() * yDir.length() / style.getFont()->fontAbove() * fabs(sin(xDir.angleTo(yDir))));
  LONG lfWidth  = OdRoundToLong(style.getFont()->getAverageWidth() * xDir.length() / style.getFont()->fontAbove() );
  if ( lfHeight < MIN_SIZE_TTF_TEXT )
    return false;

  OdFont* font = style.getFont();
  if ( font && font->getLogFont(NULL) == sizeof(LOGFONT) )
  {
    font->getLogFont(lFont);
  } else
  {
    lFont->lfWeight = style.ttfdescriptor().isBold() ? FW_BOLD : FW_DONTCARE;
    lFont->lfItalic = (unsigned char)(style.ttfdescriptor().isItalic() ? 1 : 0);
    lFont->lfUnderline = (unsigned char)(style.isUnderlined() ? 1 : 0);
    lFont->lfStrikeOut = 0;
    lFont->lfOutPrecision = OUT_DEVICE_PRECIS;
    lFont->lfClipPrecision = OUT_DEFAULT_PRECIS;
    lFont->lfQuality = PROOF_QUALITY;
    lFont->lfPitchAndFamily = (unsigned char)style.ttfdescriptor().pitchAndFamily();
    lFont->lfCharSet = (unsigned char)style.ttfdescriptor().charSet();
    if ( style.ttfdescriptor().typeface().isEmpty() )
    {
      // #13191 Empty typeface in style
      OdString fileName = style.getFontFilePath(pCtx->database());
      if ( !fileName.isEmpty() )
      {
        OdFontServicesPtr fontSvc = odrxSysRegistry()->getAt(ODDB_FONT_SERVICES);
        if ( !fontSvc.isNull() )
        {
          OdTtfDescriptor descr;
          if ( fontSvc->getTTFParamFromFile(fileName, descr) )
          {
            ::_tcscpy(lFont->lfFaceName, descr.typeface());
          }
        }
      }
    } else
    {
      ::_tcscpy(lFont->lfFaceName, style.ttfdescriptor().typeface());
    }

    // #12624 : switch to ANSI charset if font doesn't support specified charset (like OdTrueTypeFontWin does)
    struct CheckCharset
    {
      static int CALLBACK EnumFontFamExProc(const LOGFONT *lpelfe, const TEXTMETRIC * /*lpntme*/,
                                            DWORD /*FontType*/, LPARAM lParam)
      {
        return (lpelfe->lfCharSet == (BYTE)lParam) ? 0 : 1;
      }
      static bool Check(HDC hDc, LOGFONT *pLogFont)
      {
        //if ((pLogFont->lfCharSet == ANSI_CHARSET) || (pLogFont->lfCharSet == DEFAULT_CHARSET))
        // #13008 : Ansi charset could be also unavailable in font, for example in AIGDT only symbol charset available
        if (pLogFont->lfCharSet == DEFAULT_CHARSET)
          return true;
        return ::EnumFontFamiliesEx(hDc, pLogFont, EnumFontFamExProc, (LPARAM)pLogFont->lfCharSet, 0) == 0;
      }
    };
    if (!CheckCharset::Check(hDC, lFont))
      //lFont->lfCharSet = ANSI_CHARSET;
      // #13008 : Default charset useful for both cases #12624 and #13008, in both cases Windows use charset available in font
      lFont->lfCharSet = DEFAULT_CHARSET;
  }
  lFont->lfHeight = lfHeight;
  lFont->lfWidth  = lfWidth ? lfWidth : 1;
  if (twisted)
  {
    lFont->lfEscapement = 0;
    lFont->lfOrientation = 0;
  } else
  {
    lFont->lfEscapement = ang;
    lFont->lfOrientation = ang;
  }  
  return true;
}

void distributeExtra(int* pDx, int size, int extra)
{
  int extraForAll = extra / size;
  int res = extra - extraForAll*size;
  if (res < 0)
    res = -res;
  for (int i = 0; i < size; ++i)
  {
    pDx[i] += extraForAll;
    if (i < res)
    {
      if (extra < 0)
        --pDx[i];
      else
        ++pDx[i];
    }
  }
}

class TextCharacterExtraSaver
{
  HDC m_hDc;
  int m_nExtra;
  bool m_bSet;
  public:
    TextCharacterExtraSaver(HDC hDc, int nExtra)
      : m_hDc(hDc)
      , m_nExtra(0)
      , m_bSet(false)
    {
      if (nExtra != 0)
      {
        m_nExtra = ::GetTextCharacterExtra(hDc);
        ::SetTextCharacterExtra(hDc, nExtra);
        m_bSet = true;
      }
    }
    ~TextCharacterExtraSaver()
    {
      if (m_bSet)
        ::SetTextCharacterExtra(m_hDc, m_nExtra);
    }
};

class TextAlignSaver
{
  HDC m_hDc;
  UINT m_nAlign;
  bool m_bSet;
  public:
    TextAlignSaver(HDC hDc, UINT nAlign)
      : m_hDc(hDc)
      , m_nAlign(0)
      , m_bSet(false)
    {
      m_nAlign = ::GetTextAlign(hDc);
      if (m_bSet = (nAlign != m_nAlign))
        ::SetTextAlign(hDc, nAlign);
    }
    ~TextAlignSaver()
    {
      if (m_bSet)
        ::SetTextAlign(m_hDc, m_nAlign);
    }
};

OdString ExGiGDIGeometry::textProcExEmbedRawData(const OdString &msg, const OdIntArray &ulFlags, int *length, bool *raw)
{
  if (ulFlags.isEmpty())
    return msg;
  OdString embedRaw;
  const OdUInt32 nFlags = ulFlags.size() >> 1;
  const int *pFlags = ulFlags.getPtr();
  int nPrevPos = 0, nPrevFlags = 0;
  for (OdUInt32 nFlag = 0; nFlag < nFlags; nFlag++, pFlags += 2)
  {
    if (pFlags[0] > nPrevPos)
    {
      embedRaw += msg.mid(nPrevPos, pFlags[0] - nPrevPos);
      nPrevPos = pFlags[0];
    }
    if (GETBIT(nPrevFlags, 4) != GETBIT(pFlags[1], 4))
      embedRaw += OD_T("%%k");
    if (GETBIT(nPrevFlags, 2) != GETBIT(pFlags[1], 2))
      embedRaw += OD_T("%%o");
    if (GETBIT(nPrevFlags, 1) != GETBIT(pFlags[1], 1))
      embedRaw += OD_T("%%u");
    nPrevFlags = pFlags[1];
  }
  if (length) *length = embedRaw.getLength();
  if (raw) *raw = false;
  return embedRaw;
}

inline void drawTextThroughVectorizer(ExGiGDIGeometry *pGeom, const OdGePoint3d& position, const OdGeVector3d& u, const OdGeVector3d& v,
                                      const OdString &sMsg, int &length, const OdIntArray &ulFlags, const OdGiTextStyle* pTextStyle)
{
  bool raw = true; OdString rawString = pGeom->textProcExEmbedRawData(sMsg, ulFlags, &length, &raw);
  pGeom->OdGiGeometrySimplifier::textProc(position, u, v, rawString, length, raw, pTextStyle);
}

static void drawTextProcExScore(HDC hdc, HGDIOBJ hThinPen, const OdGePoint3d& position, const OdIntArray &ulFlags,
                                const int *lpDx, const OdGiTextStyle* pTextStyle, XFORM *pXform = NULL)
{
  GDIObjStack _gdiPenAutoRestore;
  if (hThinPen)
    _gdiPenAutoRestore.pushObject(hdc, hThinPen);
  int nPrevPos = 0, nPos, nAccum = 0;
  int nFlags = (int)(ulFlags.size() >> 1), nPrevFlags = 0;
  const int *pFlags = ulFlags.getPtr();
  for (int nFlag = 0; nFlag < nFlags; nFlag++, pFlags += 2)
  {
    nPos = pFlags[0];
    if (nPos > nPrevPos)
    {
      int nLen = 0;
      for (int nPosx = nPrevPos; nPosx < nPos; nPosx++)
        nLen += lpDx[nPosx];
      if (nPrevFlags)
      {
        const int xFrom = ROUND(position.x) + nAccum; const int xTo = xFrom + nLen;
        TEXTMETRIC tm; ::GetTextMetrics(hdc, &tm);
        const int yBase = ROUND(position.y);
        if (pXform)
        { // Xform should be added here, elsewhere text metrics will differ from original
          ::SetGraphicsMode(hdc, GM_ADVANCED);
          ::SetWorldTransform(hdc, pXform);
        }
        if (GETBIT(nPrevFlags, 1))
        { // Underline
          const int y = yBase + tm.tmDescent;
          ::MoveToEx(hdc, xFrom, y, NULL);
          ::LineTo(hdc, xTo, y);
        }
        if (GETBIT(nPrevFlags, 2))
        { // Overline
          const int y = yBase - tm.tmAscent;
          ::MoveToEx(hdc, xFrom, y, NULL);
          ::LineTo(hdc, xTo, y);
        }
        if (GETBIT(nPrevFlags, 4))
        { // Strikeout
          const double fHeightRatio = double(tm.tmHeight) / pTextStyle->getFont()->getHeight();
          const double fAbove = pTextStyle->getFont()->getAbove() * fHeightRatio;
          const int y = ROUND(position.y - fAbove * 0.5);
          ::MoveToEx(hdc, xFrom, y, NULL);
          ::LineTo(hdc, xTo, y);
        }
        if (pXform)
        { // Xform should be removed here, elsewhere text metrics will differ from original
          ::ModifyWorldTransform(hdc, pXform, MWT_IDENTITY);
          ::SetGraphicsMode(hdc, GM_COMPATIBLE);
        }
      }
      nPrevPos = nPos;
      nAccum += nLen;
    }
    nPrevFlags = pFlags[1];
  }
}

void ExGiGDIGeometry::textProcEx(const OdGePoint3d& position, const OdGeVector3d& u, const OdGeVector3d& v,
                                 const OdString &sMsg, const OdIntArray &ulFlags,
                                 const OdGiTextStyle* pTextStyle, OdGeVector3d originalSize)
{
  int length = sMsg.getLength();
  if (!length) return;

  if ( (OdZero( u.x ) && OdZero( u.y ))
    || (OdZero( v.x ) && OdZero( v.y )) ) return;
  // project to XY:
  OdGeVector2d xDir( u.x, u.y ), yDir( v.x, v.y );
  originalSize.z = 0.;
  bool twisted = !yDir.isPerpendicularTo(xDir) || xDir.crossProduct(yDir) > 0.;
  double fontFactor = (yDir.length() / pTextStyle->getFont()->fontAbove());
  double coefWidth = xDir.length() / yDir.length();
// double extra = 0.;
// trackingPercent take into account in distributeExtra() function (CORE-12866)
//   if ( OdNonZero( pTextStyle->trackingPercent() - 1 ) )
//   { 
//     extra = pTextStyle->getFont()->getAverageWidth()
//       * fontFactor * coefWidth * ( pTextStyle->trackingPercent() - 1 );
//     originalSize /= pTextStyle->trackingPercent();  // #16359
//   }
  LOGFONT lFont;
  if ( ::getLogFont( xDir, yDir, *pTextStyle, twisted, &lFont, m_hTargetDC, &drawContext()->giContext() ) )
  {
    if ( HFONT hFont = ::CreateFontIndirect( &lFont ) )
    {
      HDC hdc = m_hTargetDC;
      HGDIOBJ hOldFont = ::SelectObject( hdc, hFont );

      // AlexR: cr5602
      /*const OdCmEntityColor& tc = drawContext()->effectiveTraits().trueColor();
      if ( tc.isByColor() )
      {
        ::SetTextColor( hdc, RGB( tc.red(), tc.green(), tc.blue() ) );
      }
      else
      {
        ::SetTextColor( hdc, m_paletteColors[ tc.colorIndex() ] );
      }*/
      // AMark: cr5603
      ::SetTextColor(hdc, m_penBrush.lbColor); // Set text color yet, because SetTextColor() not always called in draw_color()

      // TextCharacterExtraSaver _tces(hdc, ROUND(extra)); // It seems not work in case to print directly CORE-12866

      GCP_RESULTS res = {0};
      res.lStructSize = sizeof(GCP_RESULTS);
      int buffer1[100] = { 0 };
      WCHAR buffer2[100] = { 0 };
      OdIntArray dx;
      OdArray<WCHAR, OdMemoryAllocator<WCHAR> > glyphs;
      // #CORE-11816 : we should take into account trail zero, so invoke greater-equal operator here.
      if (length >= 100) // small string are rendered w/o memory allocation
      {
        dx.resize(length + 1, 0);
        glyphs.resize(length + 1, 0);
        res.lpDx = dx.asArrayPtr();
        res.nGlyphs = length + 1;
        res.lpGlyphs = glyphs.asArrayPtr();
      }
      else
      {
        res.lpDx = buffer1;
        res.nGlyphs = 100;
        res.lpGlyphs = buffer2;
      }
      // should there be used 
      DWORD flags = ::GetFontLanguageInfo(hdc);
      flags &= ~(GCP_LIGATE|GCP_USEKERNING); // Acad doesn't use kerning or ligatures
      DWORD size = ::GetCharacterPlacementW(hdc, sMsg, length, length, &res, flags);
      if ( ( size & 0xffff ) == 0  ) // #10937 
      {
        ::SelectObject( hdc, hOldFont );
        ::DeleteObject(hFont);
        ::drawTextThroughVectorizer(this, position, u, v, sMsg, length, ulFlags, pTextStyle);
        return;
      }
      // compensate jumping, caused by discreet font sizes
      ::distributeExtra(res.lpDx, res.nGlyphs, ROUND(originalSize.length())-(size & 0xffff));
      ::SetBkMode( hdc, TRANSPARENT );
      TextAlignSaver _tas(hdc, TA_BASELINE | TA_LEFT);

      DWORD outFlags = 0;
      LPCWSTR strOut = sMsg;
      DWORD sizeMsg = length;
      if ( (flags & GCP_GLYPHSHAPE) || (flags & FLI_GLYPHS) )
      {
        outFlags = ETO_GLYPH_INDEX;
        strOut = res.lpGlyphs;
        // #12625: with FLI_GLYPHS count of glyphs could be smaller
        sizeMsg = (DWORD)odStrLen(strOut);//res.nGlyphs;
        if (sizeMsg != DWORD(length) && !ulFlags.isEmpty())
        { // @@@TODO: we can't recompute character positions into glyph positions currently.
          ::drawTextThroughVectorizer(this, position, u, v, sMsg, length, ulFlags, pTextStyle);
          return;
        }
      }

      if (twisted)
      {
        bool reflected = xDir.crossProduct(yDir) > 0;
        double txtRotate = -xDir.angle();
        double txtOblique = OdaPI2 - fabs(xDir.angleTo(yDir));
        if (reflected)
          txtOblique = - txtOblique;
        XFORM xForm;
        xForm.eDx = (float) position.x;
        xForm.eDy = (float) position.y;
        xForm.eM11 = (float) cos(txtRotate);
        xForm.eM21 = (float) (sin(txtRotate) - tan(txtOblique)*cos(txtRotate));
        xForm.eM12 = (float) -sin(txtRotate);
        xForm.eM22 = (float) (cos(txtRotate) + tan(txtOblique)*sin(txtRotate));
        if (reflected)
        {
          xForm.eM21 = -xForm.eM21;
          xForm.eM22 = -xForm.eM22;
        }
        ::SetGraphicsMode( hdc, GM_ADVANCED );
        ::SetWorldTransform( hdc, &xForm );
        ::ExtTextOutW(hdc, 0, 0, outFlags, 0, strOut, sizeMsg, res.lpDx);
        ::ModifyWorldTransform( hdc, &xForm, MWT_IDENTITY );
        ::SetGraphicsMode( hdc, GM_COMPATIBLE );
        if (!ulFlags.isEmpty())
          ::drawTextProcExScore(hdc, (ttfProcessingPenOverride()) ? thinPen() : NULL, OdGePoint3d::kOrigin, ulFlags, res.lpDx, pTextStyle, &xForm);
      }
      else
      {
        try
        {
          ::ExtTextOutW(hdc, ROUND(position.x), ROUND(position.y), outFlags, 0, strOut, sizeMsg, res.lpDx);
          if (!ulFlags.isEmpty())
            ::drawTextProcExScore(hdc, (ttfProcessingPenOverride()) ? thinPen() : NULL, position, ulFlags, res.lpDx, pTextStyle);
        }
        catch (const OdError &)
        { // #15305 : resolve arithmetic overflow using vectorization
          ::drawTextThroughVectorizer(this, position, u, v, sMsg, length, ulFlags, pTextStyle);
        }
      }
      ::SelectObject( hdc, hOldFont );
      ::DeleteObject(hFont);
    }
    else
    {
      ::drawTextThroughVectorizer(this, position, u, v, sMsg, length, ulFlags, pTextStyle);
    }
  }
  else if( lFont.lfHeight < MIN_SIZE_TTF_TEXT )
  {
    if (lFont.lfHeight <= 0)
      lFont.lfHeight = 1;

    double width = pTextStyle->getFont()->getAverageWidth() * fontFactor * coefWidth;
    OdGeVector3d dY, dX;
    dX = u.normal()*((width)*length);
    dY = v.normal()*(lFont.lfHeight-1);

    OdGePoint3dArray points;
    points.resize(4);
    points[0] = position;
    OdGeVector3d p;
    p = position.asVector() + dY;
    points[1] = OdGePoint3d(p.x, p.y, 0.0);
    p = position.asVector() + dY + dX;
    points[2] = OdGePoint3d(p.x, p.y, 0.0);
    p = position.asVector() + dX;
    points[3] = OdGePoint3d(p.x, p.y, 0.0);
    polygonOut(4, points.getPtr());
  }
  else
  {
    // text shouldn't be fillstyled
    draw_fillStyle(kFsSolid);
    updatePenNBrush();
    ::drawTextThroughVectorizer(this, position, u, v, sMsg, length, ulFlags, pTextStyle);
  }
  // onTraitsModified must be called after m_bProcessingText was changed
}
#endif

#ifndef _WIN32_WCE
void ExGiGDIGeometry::pushViewportClip(const OdGsDCPoint* pPoints, const int *nrcCounts, int nCount)
{
  HDC hdc = m_hTargetDC;
  m_clipStack.push_back(::SaveDC(hdc));
  ::BeginPath(hdc);
  ::PolyPolygon(hdc, (CONST POINT*)pPoints, (CONST INT*)nrcCounts, nCount);
  ::EndPath(hdc);
  ::SelectClipPath(hdc, RGN_AND);
}
#else
void ExGiGDIGeometry::pushViewportClip(const OdGsDCPoint* pPoints, const int *nrcCntCounts, int nCount)
{
  HDC hdc = m_hTargetDC;
  m_clipStack.push_back(::SaveDC(hdc));
  // form rectangular region
  OdGsDCPoint min = *pPoints, max = *pPoints;
  while(nCount--)
  {
    int n = nrcCntCounts[nCount];
    while(--n)
    {
      pPoints++;
      if(min.x > pPoints->x)
        min.x = pPoints->x;
      if(min.y > pPoints->y)
        min.y = pPoints->y;
      if(max.x < pPoints->x)
        max.x = pPoints->x;
      if(max.y < pPoints->y)
        max.y = pPoints->y;
    }
  }
  pushViewportClip(min, max);
}
#endif

void ExGiGDIGeometry::pushViewportClip(const OdGsDCPoint& min, const OdGsDCPoint& max)
{
  m_clipStack.push_back(::SaveDC(m_hTargetDC));
  ::IntersectClipRect(m_hTargetDC, min.x, max.y, max.x, min.y);
}

void ExGiGDIGeometry::pushViewportClip(OdUInt32 nNumRects, const OdGsDCRect* rects)
{
#ifndef _WIN32_WCE
  OdGsDCPointArray points(nNumRects * 4);
  points.resize(nNumRects * 4);
  OdIntArray counts(nNumRects);
  counts.insert(counts.end(), nNumRects, 4);
  OdGsDCPoint* cp = points.asArrayPtr();
  while(nNumRects--)
  {
    cp[0] = rects->m_min;
    cp[1].x = rects->m_min.x;
    cp[1].y = rects->m_max.y;
    cp[2] = rects->m_max;
    cp[3].x = rects->m_max.x;
    cp[3].y = rects->m_min.y;
    cp += 4;
    ++rects;
  }
  HDC hdc = m_hTargetDC;
  m_clipStack.push_back(::SaveDC(hdc));
  int pfm =::SetPolyFillMode(hdc, WINDING);
  ::BeginPath(hdc);
  ::PolyPolygon(hdc, (CONST POINT*)points.getPtr(), (CONST INT*)counts.getPtr(), counts.size());
  ::EndPath(hdc);
  ::SelectClipPath(hdc, RGN_AND);
  ::SetPolyFillMode(hdc, pfm);
#else
  // This WCE implementation sets clip region to extents of union of clipping rects.
  CDC *pDc = CDC::FromHandle(m_hTargetDC);
  m_clipStack.push_back(pDc->SaveDC());
  if(nNumRects)
  {
    OdGsDCRect rect = *rects;
    ++rects;
    --nNumRects;
    while(nNumRects--)
    {
      rect |= *rects;
      ++rects;
    }
    pDc->IntersectClipRect(rect.m_min.x, rect.m_min.y, rect.m_max.x, rect.m_max.y);
  }
#endif
}

void ExGiGDIGeometry::popViewportClip()
{
  if(!m_clipStack.empty())
  {
    // GU, cr2513
    // to prevent leaks of hPen and hBrush
    HPEN hPen = (HPEN)::GetCurrentObject( m_hTargetDC, OBJ_PEN );
    HBRUSH hBrush = (HBRUSH)::GetCurrentObject( m_hTargetDC, OBJ_BRUSH );

    ::RestoreDC(m_hTargetDC, m_clipStack.last());

    ::SelectObject( m_hTargetDC, hPen );
    ::SelectObject( m_hTargetDC, hBrush );

    m_clipStack.removeLast();
    m_bBrushValid = m_bPenValid = m_bThinPenValid = false; // force recreate DC objects
  }
  else
  {
    ODA_FAIL();
  }
}

//*******************************************************************************/
//  rasterImage()
//*******************************************************************************/

inline void getImageColorAt(int x, int y,
                            OdUInt8& blue, OdUInt8& green, OdUInt8& red,
                            const OdGiRasterImage* pImg)
{
  int colorDepth(pImg->colorDepth());

  OdUInt8Array scLine;
  const OdUInt8* ptr = pImg->scanLines();
  if(ptr != NULL)
  {
    ptr += y * pImg->scanLineSize() + x*colorDepth/8;
  }
  else
  {
    scLine.resize(pImg->scanLineSize());
    pImg->scanLines(scLine.asArrayPtr(), y);
    ptr = scLine.asArrayPtr() + x*colorDepth/8;
  }

  if(colorDepth == 24)
  {
    red   = ptr[2];
    green = ptr[1];
    blue  = ptr[0];
  }
  else if(colorDepth == 16)
  {
    OdUInt32 val = *((OdUInt32*)ptr);
    blue  = (OdUInt8)(( val        & 0x1f)<<3);
    green = (OdUInt8)(((val >> 5)  & 0x1f)<<3);
    red   = (OdUInt8)(((val >> 10) & 0x1f)<<3);
  }
  else
  {
    OdUInt32 index =
      (
      ptr[0] >>
      (
      ((8 / colorDepth) - 1 - (x % (8 / colorDepth)))
      * colorDepth
      )
      )
      &
      ((1 << colorDepth) - 1);
    ODCOLORREF resCol = pImg->color(index);
    red   = ODGETRED(resCol);
    green = ODGETGREEN(resCol);
    blue  = ODGETBLUE(resCol);
  }
}

// gets the image's pixel in screen corrdinates x and y
// returns false if point is outside the image
inline bool getScreenPixel(int SCx, int SCy, const OdGeMatrix2d& screen2UV,
                           int imgWidth, int imgHeight,
                           OdUInt8& blue, OdUInt8& green, OdUInt8& red,
                           const OdGiRasterImage* pImg)
{
  OdGePoint2d ptUV = screen2UV * OdGePoint2d(SCx, SCy);
  int x = OdTruncateToLong(ptUV.x - 0.5);
  int y = OdTruncateToLong(ptUV.y - 0.5);
  if(x >= 0 && y >= 0 && x < imgWidth && y < imgHeight)
  {
    getImageColorAt(x, y, blue, green, red, pImg);
    return true;
  }
  else
  {
    return false;
  }
}

inline void PAINT_RECT(int left, int top, int right, int bottom, OdUInt8 blue,
                       OdUInt8 green, OdUInt8 red, HDC hdc)
{
  RECT rect;
  rect.left   = left;
  rect.top    = top;
  rect.right  = right;
  rect.bottom = bottom;
  LOGBRUSH brushPat;
  brushPat.lbColor = RGB(red, green, blue);
  brushPat.lbStyle = BS_SOLID;

  HBRUSH brush = ::CreateBrushIndirect(&brushPat);

  FillRect(hdc, &rect, brush);
  DeleteObject(brush);
}

#ifndef _WIN32_WCE
class StretchBltModeSaver
{
  HDC m_hDc;
  int m_nPrevMode;
  int m_nNewMode;
  public:
    StretchBltModeSaver(HDC hDc, int nMode)
      : m_hDc(hDc), m_nPrevMode(nMode), m_nNewMode(nMode)
    {
      m_nPrevMode = ::GetStretchBltMode(hDc);
      if (m_nPrevMode != m_nNewMode)
        ::SetStretchBltMode(hDc, nMode);
    }
    ~StretchBltModeSaver()
    {
      if (m_nNewMode != m_nPrevMode)
        ::SetStretchBltMode(m_hDc, m_nPrevMode);
    }

    static int monoImageMode(bool bIsTransparent, COLORREF bkgndColor)
    {
      const OdUInt32 brightness = 30 * GetRValue(bkgndColor) + 59 * GetGValue(bkgndColor) + 11 * GetBValue(bkgndColor);
      // If monochrome image is transparent we always works with mask which require preserving of white color,
      // elsewhere we can take background color into accout to preserve negative color.
      return (bIsTransparent || (brightness < 128)) ? WHITEONBLACK : BLACKONWHITE;
    }
};
#endif

void ExGiGDIGeometry::rasterImageProc(const OdGePoint3d& origin,
                                      const OdGeVector3d& u,
                                      const OdGeVector3d& v,
                                      const OdGiRasterImage* pImg, // image object
                                      const OdGePoint2d* uvBoundary, // may not be null
                                      OdUInt32 numBoundPts,
                                      bool transparency,
                                      double brightness,
                                      double contrast,
                                      double fade)
{
  if(isDragging())
    return ;

  const OdGeVector2d u2d(u.x, u.y), v2d(v.x, v.y);

  if(u2d.isZeroLength() || v2d.isZeroLength())
    return ; // image became line segment

  if(u2d.isParallelTo(v2d))
    return ; // image became degenerated

  HDC hdc = m_hTargetDC;

#ifndef _WIN32_WCE
  StretchBltModeSaver _sbms(hdc, (pImg->colorDepth() > 1) ? COLORONCOLOR : StretchBltModeSaver::monoImageMode(transparency, m_paletteColors[0]));
#endif

  OdGiRasterImagePtr pCopyImg;
  OdGiRasterImagePtr pBitonalImg;
  OdGiRasterImage::PixelFormatInfo fmt = pImg->pixelFormat();
  if( fmt.isRGB() || fmt.isRGBA() || (!(OdEqual(brightness, 50.) && OdEqual(contrast, 50.) && OdZero(fade)) &&
      (pImg->colorDepth() != 1))) // bitonal images can't be adjusted
  {
    OdSmartPtr<OdGiRasterImageDesc> pDesc = OdRxObjectImpl<OdGiRasterImageDesc>::createObject();
    pDesc->setColorDepth(pImg->colorDepth());
    switch (pImg->colorDepth())
    {
    case 24:
      pDesc->pixelFormat().setBGR();
      break;
    case 32:
      pDesc->pixelFormat().setBGRA();
      break;
    default:
      pDesc->pixelFormat() = pImg->pixelFormat();
    }
    pDesc->setPixelWidth(pImg->pixelWidth());
    pDesc->setPixelHeight(pImg->pixelHeight());
    pDesc->setScanLinesAlignment(pImg->scanLinesAlignment());
    pCopyImg = pImg->convert(false, brightness, contrast, fade, m_paletteColors[0], false, false, false, pDesc);
    pImg = pCopyImg.get();
  }
  else if(pImg->colorDepth() == 1)
  {
    OdSmartPtr<OdGiBitonalRasterTransformer> pBitRast = OdRxObjectImpl<OdGiBitonalRasterTransformer>::createObject();
    const OdGiSubEntityTraitsData &effTraits = drawContext()->effectiveTraits();
    if (transparency || !GETBIT(effTraits.drawFlags(), OdGiSubEntityTraits::kDrawContourFill))
    {
      pBitRast->setOriginal(pImg, m_penBrush.lbColor, m_paletteColors[0], transparency);
    }
    else
    {
      // Teigha(R) for .dwg files specific
      ODCOLORREF secColor;
      if(effTraits.secondaryTrueColor().isByACI() || effTraits.secondaryTrueColor().isByDgnIndex())
        secColor = m_paletteColors[effTraits.secondaryTrueColor().colorIndex()];
      else
        secColor = ODTOCOLORREF(effTraits.secondaryTrueColor());
      pBitRast->setOriginal(pImg, m_penBrush.lbColor, secColor, transparency);
    }
    pBitonalImg = pBitRast;
    pImg = pBitonalImg;
  }

  OdUInt32 imgWidth(pImg->pixelWidth()), imgHeight(pImg->pixelHeight());

  int i, j;
  OdGePoint2d temp[4];
  switch(numBoundPts)
  {
  case 0:
    temp[0].set(-0.5, -0.5);
    temp[2].set(imgWidth - 0.5, imgHeight - 0.5);
    temp[1].set(temp[0].x, temp[2].y);
    temp[3].set(temp[2].x, temp[0].y);
    uvBoundary = temp;
    numBoundPts = 4;
    break;

  case 2:
    temp[0] = uvBoundary[0];
    temp[2] = uvBoundary[1];
    temp[1].set(temp[0].x, temp[2].y);
    temp[3].set(temp[2].x, temp[0].y);
    uvBoundary = temp;
    numBoundPts = 4;
    break;
  }

  OdArray<OdGsDCPoint, OdMemoryAllocator<OdGsDCPoint> > screenBoundary(numBoundPts);
  OdGePoint3d pt;
  for(i = 0; i < (int)numBoundPts; i ++)
  {
    pt = origin + (uvBoundary[i].x + 0.5) * u + (imgHeight - uvBoundary[i].y - 0.5) * v;
    screenBoundary.push_back(OdGsDCPoint(OdTruncateToLong(pt.x), OdTruncateToLong(pt.y)));
  }

  if(numBoundPts)
  {
    int counts = numBoundPts;
    pushViewportClip(screenBoundary.getPtr(), &counts, 1);
  }

  OdGeMatrix2d screen2UV; // computing it...
  OdGeMatrix2d UV2Screen;
  UV2Screen.setCoordSystem(origin.convert2d(), u2d, v2d);
  screen2UV = UV2Screen.inverse();

  int dWidth = Od_abs(m_rcTarget.m_max.x - m_rcTarget.m_min.x);
  int dHeight = Od_abs(m_rcTarget.m_max.y - m_rcTarget.m_min.y);
  
  if( u.isCodirectionalTo(OdGeVector3d::kXAxis) && v.isCodirectionalTo(-OdGeVector3d::kYAxis) )
  {
    // particular

    // Fix for very scaled small images
    OdGePoint2d* corners = temp;
    corners[0].set(0.0, 0.0);
    corners[1].set(dWidth, 0.0);
    corners[2].set(0.0, dHeight);
    corners[3].set(dWidth, dHeight);
    for(i = 0; i < 4; i ++)
    {
      corners[i].transformBy(screen2UV);
      corners[i] -= OdGeVector2d(0.5, 0.5);
    }
    // need to take sign into account
    if((fabs(OdRound(corners[1].x) - OdRound(corners[0].x)) <= 1) &&
       (fabs(OdRound(corners[2].y) - OdRound(corners[0].y)) <= 1) )
    {
      int chColorLeft   = 0;
      int chColorTop    = 0;
      int chColorRight  = 0;
      int chColorBottom = 0;

      // left side
      OdUInt8 curBlue(0), curGreen(0), curRed(0);
      bool bWasImg = getScreenPixel(0, 0, screen2UV, imgWidth, imgHeight,
          curBlue, curGreen, curRed, pImg);
      for(i = 0; i < dHeight; i ++)
      {
        OdUInt8 blue(0), green(0), red(0);
        if(getScreenPixel(0, i, screen2UV, imgWidth, imgHeight, blue, green, red, pImg))
        {
          if(!bWasImg)
          {
            bWasImg  = true;
            curBlue  = blue;
            curGreen = green;
            curRed   = red;
          }
          else
          {
            if((curBlue != blue) || (curGreen != green) || (curRed != red))
            {
              curBlue  = blue;
              curGreen = green;
              curRed   = red;
              chColorLeft = i;
              break;
            }
          }
        }
      }
      // top side
      curBlue  = 0;
      curGreen = 0;
      curRed   = 0;
      bWasImg = getScreenPixel(0, 0, screen2UV, imgWidth, imgHeight,
          curBlue, curGreen, curRed, pImg);
      for(i = 0; i < dWidth; i ++)
      {
        OdUInt8 blue(0), green(0), red(0);
        if(getScreenPixel(i, 0, screen2UV, imgWidth, imgHeight, blue, green, red, pImg))
        {
          if(!bWasImg)
          {
            bWasImg  = true;
            curBlue  = blue;
            curGreen = green;
            curRed   = red;
            chColorTop = i;
            break;
          }
          else
          {
            if((curBlue != blue) || (curGreen != green) || (curRed != red))
            {
              curBlue  = blue;
              curGreen = green;
              curRed   = red;
              chColorTop = i;
              break;
            }
          }
        }
        else
        {
          // we have gone outside image
          chColorTop = i;
          break;
        }
      }
      // right side
      curBlue  = 0;
      curGreen = 0;
      curRed   = 0;
      bWasImg = getScreenPixel(dWidth, 0, screen2UV, imgWidth, imgHeight,
          curBlue, curGreen, curRed, pImg);
      for(i = 0; i < dHeight; i ++)
      {
        OdUInt8 blue(0), green(0), red(0);
        if(getScreenPixel(dWidth, i, screen2UV, imgWidth, imgHeight, blue, green, red, pImg))
        {
          if(!bWasImg)
          {
            bWasImg  = true;
            curBlue  = blue;
            curGreen = green;
            curRed   = red;
          }
          else
          {
            if((curBlue != blue) || (curGreen != green) || (curRed != red))
            {
              curBlue  = blue;
              curGreen = green;
              curRed   = red;
              chColorRight = i;
              break;
            }
          }
        }
      }
      // bottom side
      curBlue  = 0;
      curGreen = 0;
      curRed   = 0;
      bWasImg = getScreenPixel(0, dHeight, screen2UV, imgWidth, imgHeight,
          curBlue, curGreen, curRed, pImg);
      for(i = 0; i < dWidth; i ++)
      {
        OdUInt8 blue(0), green(0), red(0);
        if(getScreenPixel(i, dHeight, screen2UV, imgWidth, imgHeight, blue, green, red, pImg))
        {
          if(!bWasImg)
          {
            bWasImg  = true;
            curBlue  = blue;
            curGreen = green;
            curRed   = red;
            chColorBottom = i;
            break;
          }
          else
          {
            if((curBlue != blue) || (curGreen != green) || (curRed != red))
            {
              curBlue  = blue;
              curGreen = green;
              curRed   = red;
              chColorBottom = i;
              break;
            }
          }
        }
        else
        {
          chColorBottom = i;
          break;
        }
      }

      // now operating with colors
      if((chColorLeft != 0) && (chColorRight == 0))
      {
        chColorRight = chColorLeft;
      }
      if((chColorLeft == 0) && (chColorRight != 0))
      {
        chColorLeft = chColorRight;
      }
      if((chColorTop != 0) && (chColorBottom == 0))
      {
        chColorBottom = chColorTop;
      }
      if((chColorTop == 0) && (chColorBottom != 0))
      {
        chColorTop = chColorBottom;
      }

      // now painting 4 rects
      OdUInt8 blue(0), green(0), red(0);
      if(getScreenPixel(0, 0, screen2UV, imgWidth, imgHeight, blue, green, red, pImg))
      {
        PAINT_RECT(0, 0, chColorTop, chColorLeft, blue, green, red, hdc);
      }
      if(getScreenPixel(dWidth, 0, screen2UV, imgWidth, imgHeight, blue, green, red, pImg))
      {
        PAINT_RECT(chColorTop, 0, dWidth, chColorLeft, blue, green, red, hdc);
      }
      if(getScreenPixel(0, dHeight, screen2UV, imgWidth, imgHeight, blue, green, red, pImg))
      {
        PAINT_RECT(0, chColorLeft, chColorTop, dHeight, blue, green, red, hdc);
      }
      if(getScreenPixel(dWidth, dHeight, screen2UV, imgWidth, imgHeight, blue, green, red, pImg))
      {
        PAINT_RECT(chColorTop, chColorLeft, dWidth, dHeight, blue, green, red, hdc);
      }
    }
    else
    {
      BITMAPINFO* pHeader;
      OdUInt8Array headerMemCont;
      headerMemCont.resize(sizeof(BITMAPINFOHEADER) + pImg->paletteDataSize());
      pHeader = (BITMAPINFO*)headerMemCont.getPtr();
      pHeader->bmiHeader.biBitCount = (WORD)pImg->colorDepth();
      pHeader->bmiHeader.biClrImportant = 0;
      pHeader->bmiHeader.biClrUsed = 0;
      pHeader->bmiHeader.biCompression = BI_RGB;
      pHeader->bmiHeader.biHeight = imgHeight;
      pHeader->bmiHeader.biPlanes = 1;
      pHeader->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
      pHeader->bmiHeader.biSizeImage = 0;
      pHeader->bmiHeader.biWidth = imgWidth;
      pHeader->bmiHeader.biXPelsPerMeter = 0;
      pHeader->bmiHeader.biYPelsPerMeter = 0;
      pImg->paletteData((OdUInt8*)pHeader->bmiColors);

      if((pImg->colorDepth() == 1) && transparency)
      {
        // taking transparency into account

        // First, get the color that is transparent
        int transpColor(pImg->transparentColor());

        pHeader->bmiColors[transpColor].rgbBlue =
        pHeader->bmiColors[transpColor].rgbGreen =
        pHeader->bmiColors[transpColor].rgbRed = 255;

        OdUInt8 wasBlue  = pHeader->bmiColors[1-transpColor].rgbBlue;
        OdUInt8 wasGreen = pHeader->bmiColors[1-transpColor].rgbGreen;
        OdUInt8 wasRed   = pHeader->bmiColors[1-transpColor].rgbRed;

        pHeader->bmiColors[1-transpColor].rgbBlue =
        pHeader->bmiColors[1-transpColor].rgbGreen =
        pHeader->bmiColors[1-transpColor].rgbRed = 0;

        OdUInt8 *pPixels;
        HBITMAP hBmpMask = CreateDIBSection(NULL, pHeader, DIB_RGB_COLORS, (void**)&pPixels, NULL, 0);
        pImg->scanLines(pPixels, 0, imgHeight);
        HDC maskDC = CreateCompatibleDC(hdc);
        if (maskDC)
          SelectObject(maskDC, hBmpMask);
#ifndef _WIN32_WCE
        else
        {
          // Probably we're inside metafile (#7066)
          StretchDIBits(hdc,
            OdTruncateToLong(origin.x),
            OdTruncateToLong(origin.y - v.length() * imgHeight),
            OdTruncateToLong(u.length() * imgWidth),
            OdTruncateToLong(v.length() * imgHeight),
            0, 0, imgWidth, imgHeight, pPixels, pHeader, DIB_RGB_COLORS, SRCAND);
        }
#endif

        pHeader->bmiColors[transpColor].rgbBlue =
        pHeader->bmiColors[transpColor].rgbGreen =
        pHeader->bmiColors[transpColor].rgbRed = 0;

        pHeader->bmiColors[1-transpColor].rgbBlue  = wasBlue;
        pHeader->bmiColors[1-transpColor].rgbGreen = wasGreen;
        pHeader->bmiColors[1-transpColor].rgbRed   = wasRed;

        HBITMAP hBmpPatched = CreateDIBSection(NULL, pHeader, DIB_RGB_COLORS, (void**)&pPixels, NULL, 0);
        pImg->scanLines(pPixels, 0, imgHeight);
        HDC patchedDC = CreateCompatibleDC(hdc);
        if (patchedDC)
          SelectObject(patchedDC, hBmpPatched);
#ifndef _WIN32_WCE
        else
        {
          // Probably we're inside metafile (#7066)
          StretchDIBits(hdc,
            OdTruncateToLong(origin.x),
            OdTruncateToLong(origin.y - v.length() * imgHeight),
            OdTruncateToLong(u.length() * imgWidth),
            OdTruncateToLong(v.length() * imgHeight),
            0, 0, imgWidth, imgHeight, pPixels, pHeader, DIB_RGB_COLORS, SRCPAINT);
        }
#endif

        if (maskDC && patchedDC)
        {
          StretchBlt(hdc,
            OdTruncateToLong(origin.x),
            OdTruncateToLong(origin.y - v.length() * imgHeight),
            OdTruncateToLong(u.length() * imgWidth),
            OdTruncateToLong(v.length() * imgHeight),
            maskDC, 0, 0, imgWidth, imgHeight, SRCAND);
          StretchBlt(hdc,
            OdTruncateToLong(origin.x),
            OdTruncateToLong(origin.y - v.length() * imgHeight),
            OdTruncateToLong(u.length() * imgWidth),
            OdTruncateToLong(v.length() * imgHeight),
            patchedDC, 0, 0, imgWidth, imgHeight, SRCPAINT);

          DeleteDC(maskDC);
          DeleteDC(patchedDC);
        }
        DeleteObject(hBmpMask);
        DeleteObject(hBmpPatched);
      }
      else if((pImg->colorDepth() == 32) && transparency)
      {
        // taking transparency into account
        // This works, but it's not efficient way. Optimize later if it is possible.

        OdUInt8 *pPixels, *pPixels2;
        HBITMAP hBmpMask = CreateDIBSection(NULL, pHeader, DIB_RGB_COLORS, (void**)&pPixels, NULL, 0);
        pImg->scanLines(pPixels, 0, imgHeight);

        HDC maskDC = CreateCompatibleDC(hdc);
        if (maskDC)
          SelectObject(maskDC, hBmpMask);
#ifndef _WIN32_WCE
        else
        {
          // Probably we're inside metafile (#7066)
          StretchDIBits(hdc,
            OdTruncateToLong(origin.x),
            OdTruncateToLong(origin.y - v.length() * imgHeight),
            OdTruncateToLong(u.length() * imgWidth),
            OdTruncateToLong(v.length() * imgHeight),
            0, 0, imgWidth, imgHeight, pPixels, pHeader, DIB_RGB_COLORS, SRCAND);
        }
#endif

        HBITMAP hBmpPatched = CreateDIBSection(NULL, pHeader, DIB_RGB_COLORS, (void**)&pPixels2, NULL, 0);
        pImg->scanLines(pPixels2, 0, imgHeight);

        OdUInt32 *pPix = (OdUInt32*)pPixels, *pPix2 = (OdUInt32*)pPixels2, iy, ix;
        OdUInt8 *pPixH = (OdUInt8*)pPix, *pPixH2 = (OdUInt8*)pPix2;
        for (iy = 0; iy < imgHeight; iy++)
        {
          for (ix = 0; ix < imgWidth; ix++)
          {
            if (ODGETALPHA(pPix[ix]) == 0)
            {
              pPix[ix] = ODRGB(255, 255, 255);
              pPix2[ix] = ODRGB(0, 0, 0);
            }
            else
            {
              pPix[ix] = ODRGB(0, 0, 0);
            }
          }
          pPixH += pImg->scanLineSize();
          pPixH2 += pImg->scanLineSize();
          pPix = (OdUInt32*)pPixH;
          pPix2 = (OdUInt32*)pPixH2;
        }

        HDC patchedDC = CreateCompatibleDC(hdc);
        if (patchedDC)
          SelectObject(patchedDC, hBmpPatched);
#ifndef _WIN32_WCE
        else
        {
          // Probably we're inside metafile (#7066)
          StretchDIBits(hdc,
            OdTruncateToLong(origin.x),
            OdTruncateToLong(origin.y - v.length() * imgHeight),
            OdTruncateToLong(u.length() * imgWidth),
            OdTruncateToLong(v.length() * imgHeight),
            0, 0, imgWidth, imgHeight, pPixels2, pHeader, DIB_RGB_COLORS, SRCPAINT);
        }
#endif

        if (maskDC && patchedDC)
        {
          StretchBlt(hdc,
            OdTruncateToLong(origin.x),
            OdTruncateToLong(origin.y - v.length() * imgHeight),
            OdTruncateToLong(u.length() * imgWidth),
            OdTruncateToLong(v.length() * imgHeight),
            maskDC, 0, 0, imgWidth, imgHeight, SRCAND);
          StretchBlt(hdc,
            OdTruncateToLong(origin.x),
            OdTruncateToLong(origin.y - v.length() * imgHeight),
            OdTruncateToLong(u.length() * imgWidth),
            OdTruncateToLong(v.length() * imgHeight),
            patchedDC, 0, 0, imgWidth, imgHeight, SRCPAINT);

          DeleteDC(maskDC);
          DeleteDC(patchedDC);
        }
        DeleteObject(hBmpMask);
        DeleteObject(hBmpPatched);
      }
      else
      {
        OdUInt8* pBuf;
        HBITMAP hBmp = CreateDIBSection(NULL, pHeader, DIB_RGB_COLORS, (void**)&pBuf, NULL, 0);
        if (pImg->scanLineSize())
          pImg->scanLines(pBuf, 0, imgHeight);

        HDC sourceDC = CreateCompatibleDC(hdc);
        if (sourceDC)
        {
          // We're in raster device
          SelectObject(sourceDC, hBmp);
          StretchBlt(hdc,
            OdTruncateToLong(origin.x),
            OdTruncateToLong(origin.y - v.length() * imgHeight),
            OdTruncateToLong(u.length() * imgWidth),
            OdTruncateToLong(v.length() * imgHeight),
            sourceDC, 0, 0, imgWidth, imgHeight, SRCCOPY);
          DeleteDC(sourceDC);
        }
#ifndef _WIN32_WCE
        else
        {
          // Probably we're inside metafile (#7066)
          StretchDIBits(hdc,
            OdTruncateToLong(origin.x),
            OdTruncateToLong(origin.y - v.length() * imgHeight),
            OdTruncateToLong(u.length() * imgWidth),
            OdTruncateToLong(v.length() * imgHeight),
            0, 0, imgWidth, imgHeight, pBuf, pHeader, DIB_RGB_COLORS, SRCCOPY);
        }
#endif
        DeleteObject(hBmp);
      }
    }
  }
  else
  {
    // common case
    OdGePoint3d corners[4];
    corners[0] = origin;
    corners[1] = origin + u * imgWidth;
    corners[2] = origin + v * imgHeight;
    corners[3] = origin + v * imgHeight + u * imgWidth;

    int minX = dWidth;
    int minY = dHeight;
    int maxX = 0;
    int maxY = 0;

    for(i = 0; i < 4; i ++)
    {
      if(corners[i].x < minX)
      {
        minX = corners[i].x > 0 ? OdTruncateToLong(corners[i].x) : 0;
      }
      if(corners[i].y < minY)
      {
        minY = corners[i].y > 0 ? OdTruncateToLong(corners[i].y) : 0;
      }
      if(corners[i].x > maxX)
      {
        maxX = corners[i].x < dWidth ? OdTruncateToLong(corners[i].x) : dWidth;
      }
      if(corners[i].y > maxY)
      {
        maxY = corners[i].y < dHeight ? OdTruncateToLong(corners[i].y) : dHeight;
      }
    }

    int boundRectX = odmax(minX, 0);
    int boundRectY = odmax(minY, 0);
    int boundRectWidth  = odmin(maxX, dWidth)  - boundRectX;
    int boundRectHeight = odmin(maxY, dHeight) - boundRectY;
    // Painting pixels...

    OdUInt32 srcColDepth = pImg->colorDepth();
    /*OdUInt32 srcWidth    = */ pImg->pixelWidth();
    OdUInt32 srcHeight   = pImg->pixelHeight();
    OdUInt32 srcScLnSz   = pImg->scanLineSize();

    // it is very pitty, but here we haven't other way but to retrieve all
    // source image pixels
    OdUInt8Array srcPixels;
    const OdUInt8* pSrcPixels = pImg->scanLines();
    if(pSrcPixels == NULL)
    {
      srcPixels.resize(srcScLnSz * srcHeight);
      pImg->scanLines(srcPixels.asArrayPtr(), 0, srcHeight);
      pSrcPixels = srcPixels.asArrayPtr();
    }

    if((srcColDepth == 1) && transparency)
    {
      // First, get the color that is transparent
      int transpColor(pImg->transparentColor());

      OdUInt32 maskScLnSz = OdGiRasterImage::calcBMPScanLineSize(boundRectWidth, 1);
      OdUInt32 pixScLnSz  = OdGiRasterImage::calcBMPScanLineSize(boundRectWidth, 4);

      OdUInt8Array pixels, mask;
      pixels.resize(pixScLnSz * boundRectHeight);
      mask.resize(maskScLnSz * boundRectHeight);

      OdUInt32 maskBlackIndex(0), maskWhiteIndex(1); // let them be such
      OdUInt32 pixBlackIndex(2); // need to not forget to initialise

      for(i = boundRectY; i < boundRectY + boundRectHeight; i ++)
      {
        for(j = boundRectX; j < boundRectX + boundRectWidth; j ++)
        {
          OdGePoint2d ptUV = screen2UV * OdGePoint2d(j, i);
          OdUInt32 x = OdTruncateToLong(ptUV.x - 0.5);
          OdUInt32 y = OdTruncateToLong(ptUV.y - 0.5);
          if(x >= 0 && y >= 0 && x < imgWidth && y < imgHeight )
          {
            // getting index of (x,y) pixel
            const OdUInt8* ptr = pSrcPixels + y * srcScLnSz + x * srcColDepth / 8;
            OdUInt32 index =
              (ptr[0] >>
              (
              ((8 / srcColDepth) - 1 - (x % (8 / srcColDepth)))
              * srcColDepth
              )
              )
              &
              ((1 << srcColDepth) - 1);

            OdUInt32 dstX, dstY;
            dstX = j - boundRectX;
            dstY = boundRectHeight - i - 1 + boundRectY;
            OdUInt8* maskPtr = (OdUInt8*)mask.asArrayPtr() + dstY * maskScLnSz + dstX / 8;
            OdUInt8* pixPtr = (OdUInt8*)pixels.asArrayPtr() + dstY * pixScLnSz + dstX / 2;

            if(index == (OdUInt32)transpColor)
            {
              // transparent color - should be white in mask and black in pixels
              maskPtr[0] &= (0xFF - (1 << (7-dstX%8)));
              maskPtr[0] |= maskWhiteIndex << (7-dstX%8);

              pixPtr[0]  &= (0xFF - (15 << ((1-dstX%2)<<2) ));
              pixPtr[0]  |= pixBlackIndex << ((1-dstX%2)<<2);
            }
            else
            {
              // not transparent color - should be black in mask and as is in src in pixels
              maskPtr[0] &= (0xFF - (1 << (7-dstX%8)));
              maskPtr[0] |= maskBlackIndex << (7-dstX%8);

              pixPtr[0]  &= (0xFF - (15 << ((1-dstX%2)<<2) ));
              pixPtr[0]  |= index << ((1-dstX%2)<<2);
            }
          }
        }
      }

      // now creating bitmaps
      BITMAPINFO* pBmpH;
      OdUInt8Array headerMemCont;
      headerMemCont.resize(sizeof(BITMAPINFOHEADER) + 4*sizeof(ODCOLORREF));
      pBmpH = (BITMAPINFO*)headerMemCont.getPtr();
      pBmpH->bmiHeader.biBitCount = 1;
      pBmpH->bmiHeader.biClrImportant = 0;
      pBmpH->bmiHeader.biClrUsed = 0;
      pBmpH->bmiHeader.biCompression = BI_RGB;
      pBmpH->bmiHeader.biHeight = boundRectHeight;
      pBmpH->bmiHeader.biPlanes = 1;
      pBmpH->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
      pBmpH->bmiHeader.biSizeImage = 0;
      pBmpH->bmiHeader.biWidth = boundRectWidth;
      pBmpH->bmiHeader.biXPelsPerMeter = 0;
      pBmpH->bmiHeader.biYPelsPerMeter = 0;

      // Mask
      pBmpH->bmiColors[maskBlackIndex].rgbBlue  = 0;
      pBmpH->bmiColors[maskBlackIndex].rgbGreen = 0;
      pBmpH->bmiColors[maskBlackIndex].rgbRed   = 0;
      pBmpH->bmiColors[maskWhiteIndex].rgbBlue  = 255;
      pBmpH->bmiColors[maskWhiteIndex].rgbGreen = 255;
      pBmpH->bmiColors[maskWhiteIndex].rgbRed   = 255;
      OdUInt8* pBmpMask;
      HBITMAP hBmpMask = CreateDIBSection(NULL, pBmpH, DIB_RGB_COLORS, (void**)&pBmpMask, NULL, 0);
      memcpy(pBmpMask, mask.asArrayPtr(), maskScLnSz * boundRectHeight);

      // Patched pixels
      pBmpH->bmiHeader.biBitCount = 4;
      pImg->paletteData((OdUInt8*)(pBmpH->bmiColors));
      pBmpH->bmiColors[pixBlackIndex].rgbBlue  = 0;
      pBmpH->bmiColors[pixBlackIndex].rgbGreen = 0;
      pBmpH->bmiColors[pixBlackIndex].rgbRed   = 0;
      OdUInt8* pBmpPixels;
      HBITMAP hBmpPixels = CreateDIBSection(NULL, pBmpH, DIB_RGB_COLORS, (void**)&pBmpPixels, NULL, 0);
      memcpy(pBmpPixels, pixels.asArrayPtr(), pixScLnSz * boundRectHeight);

      HDC sourceDC = CreateCompatibleDC(hdc);
      if (sourceDC)
      {
        SelectObject(sourceDC, hBmpMask);
        BitBlt(hdc, boundRectX, boundRectY,
          boundRectWidth, boundRectHeight, sourceDC, 0, 0, SRCAND);
        SelectObject(sourceDC, hBmpPixels);
        BitBlt(hdc, boundRectX, boundRectY,
          boundRectWidth, boundRectHeight, sourceDC, 0, 0, SRCPAINT);
        DeleteDC(sourceDC);
      }
#ifndef _WIN32_WCE
      else
      {
        // Probably we're inside metafile (#7066)
        RGBQUAD bmiBefore1 = pBmpH->bmiColors[maskBlackIndex],
                bmiBefore2 = pBmpH->bmiColors[maskWhiteIndex];
        pBmpH->bmiColors[maskBlackIndex].rgbBlue  = 0;
        pBmpH->bmiColors[maskBlackIndex].rgbGreen = 0;
        pBmpH->bmiColors[maskBlackIndex].rgbRed   = 0;
        pBmpH->bmiColors[maskWhiteIndex].rgbBlue  = 255;
        pBmpH->bmiColors[maskWhiteIndex].rgbGreen = 255;
        pBmpH->bmiColors[maskWhiteIndex].rgbRed   = 255;
        pBmpH->bmiHeader.biBitCount = 1;
        StretchDIBits(hdc, boundRectX, boundRectY,
          boundRectWidth, boundRectHeight, 0, 0, boundRectWidth, boundRectHeight,
          pBmpMask, pBmpH, DIB_RGB_COLORS, SRCAND);
        pBmpH->bmiColors[maskBlackIndex] = bmiBefore1;
        pBmpH->bmiColors[maskWhiteIndex] = bmiBefore2;
        pBmpH->bmiHeader.biBitCount = 4;
        StretchDIBits(hdc, boundRectX, boundRectY,
          boundRectWidth, boundRectHeight, 0, 0, boundRectWidth, boundRectHeight,
          pBmpPixels, pBmpH, DIB_RGB_COLORS, SRCPAINT);
      }
#endif
      DeleteObject(hBmpMask);
      DeleteObject(hBmpPixels);
    }
    else if((srcColDepth == 32) && transparency)
    {
      // taking transparency into account
      // This works, but it's not efficient way. Optimize later if it is possible.

      OdUInt32 maskScLnSz = OdGiRasterImage::calcBMPScanLineSize(boundRectWidth, 1);
      OdUInt32 pixScLnSz  = OdGiRasterImage::calcBMPScanLineSize(boundRectWidth, 32);

      OdUInt8Array pixels, mask;
      pixels.resize(pixScLnSz * boundRectHeight);
      mask.resize(maskScLnSz * boundRectHeight);

      for(i = boundRectY; i < boundRectY + boundRectHeight; i ++)
      {
        for(j = boundRectX; j < boundRectX + boundRectWidth; j ++)
        {
          OdGePoint2d ptUV = screen2UV * OdGePoint2d(j, i);
          OdUInt32 x = OdTruncateToLong(ptUV.x - 0.5);
          OdUInt32 y = OdTruncateToLong(ptUV.y - 0.5);
          if (x >= 0 && y >= 0 && x < imgWidth && y < imgHeight)
          {
            // getting index of (x,y) pixel
            const OdUInt8* ptr = pSrcPixels + y * srcScLnSz + x * 4;
            OdUInt32 color = *(const OdUInt32*)ptr;

            OdUInt32 dstX, dstY;
            dstX = j - boundRectX;
            dstY = boundRectHeight - i - 1 + boundRectY;
            OdUInt8* maskPtr = (OdUInt8*)mask.asArrayPtr() + dstY * maskScLnSz + dstX / 8;
            OdUInt8* pixPtr = (OdUInt8*)pixels.asArrayPtr() + dstY * pixScLnSz + dstX * 4;

            if (ODGETALPHA(color) == 0)
            {
              maskPtr[0] &= (0xFF - (1 << (7 - dstX % 8)));
              maskPtr[0] |= 1 << (7 - dstX % 8);
              *(OdUInt32*)pixPtr = RGB(0, 0, 0);
            }
            else
            {
              maskPtr[0] &= (0xFF - (1 << (7 - dstX % 8)));
              //maskPtr[0] |= 0 << (7 - dstX % 8);
              *(OdUInt32*)pixPtr = color;
            }
          }
        }
      }

      // now creating bitmaps
      BITMAPINFO* pBmpH;
      OdUInt8Array headerMemCont;
      headerMemCont.resize(sizeof(BITMAPINFOHEADER) + 2 * sizeof(ODCOLORREF));
      pBmpH = (BITMAPINFO*)headerMemCont.getPtr();
      pBmpH->bmiHeader.biBitCount = 1;
      pBmpH->bmiHeader.biClrImportant = 0;
      pBmpH->bmiHeader.biClrUsed = 0;
      pBmpH->bmiHeader.biCompression = BI_RGB;
      pBmpH->bmiHeader.biHeight = boundRectHeight;
      pBmpH->bmiHeader.biPlanes = 1;
      pBmpH->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
      pBmpH->bmiHeader.biSizeImage = 0;
      pBmpH->bmiHeader.biWidth = boundRectWidth;
      pBmpH->bmiHeader.biXPelsPerMeter = 0;
      pBmpH->bmiHeader.biYPelsPerMeter = 0;

      // Mask
      pBmpH->bmiColors[0].rgbBlue  = 0;
      pBmpH->bmiColors[0].rgbGreen = 0;
      pBmpH->bmiColors[0].rgbRed   = 0;
      pBmpH->bmiColors[1].rgbBlue  = 255;
      pBmpH->bmiColors[1].rgbGreen = 255;
      pBmpH->bmiColors[1].rgbRed   = 255;
      OdUInt8* pBmpMask;
      HBITMAP hBmpMask = CreateDIBSection(NULL, pBmpH, DIB_RGB_COLORS, (void**)&pBmpMask, NULL, 0);
      memcpy(pBmpMask, mask.asArrayPtr(), maskScLnSz * boundRectHeight);

      // Patched pixels
      pBmpH->bmiHeader.biBitCount = 32;
      OdUInt8* pBmpPixels;
      HBITMAP hBmpPixels = CreateDIBSection(NULL, pBmpH, DIB_RGB_COLORS, (void**)&pBmpPixels, NULL, 0);
      memcpy(pBmpPixels, pixels.asArrayPtr(), pixScLnSz * boundRectHeight);

      HDC sourceDC = CreateCompatibleDC(hdc);
      if (sourceDC)
      {
        SelectObject(sourceDC, hBmpMask);
        BitBlt(hdc, boundRectX, boundRectY,
          boundRectWidth, boundRectHeight, sourceDC, 0, 0, SRCAND);
        SelectObject(sourceDC, hBmpPixels);
        BitBlt(hdc, boundRectX, boundRectY,
          boundRectWidth, boundRectHeight, sourceDC, 0, 0, SRCPAINT);
        DeleteDC(sourceDC);
      }
#ifndef _WIN32_WCE
      else
      {
        // Probably we're inside metafile (#7066)
        pBmpH->bmiHeader.biBitCount = 1;
        StretchDIBits(hdc, boundRectX, boundRectY,
          boundRectWidth, boundRectHeight, 0, 0, boundRectWidth, boundRectHeight,
          pBmpMask, pBmpH, DIB_RGB_COLORS, SRCAND);
        pBmpH->bmiHeader.biBitCount = 32;
        StretchDIBits(hdc, boundRectX, boundRectY,
          boundRectWidth, boundRectHeight, 0, 0, boundRectWidth, boundRectHeight,
          pBmpPixels, pBmpH, DIB_RGB_COLORS, SRCPAINT);
      }
#endif
      DeleteObject(hBmpMask);
      DeleteObject(hBmpPixels);
    }
    else
    {
      OdUInt8Array pixels;
      OdUInt32 dstScLnSz = OdGiRasterImage::calcBMPScanLineSize(boundRectWidth, srcColDepth);
      pixels.resize(dstScLnSz * boundRectHeight);

      // MKU 14.04.2003 [   - for supporting transparency (alpha)
      /* bool bGetAlpha = (device()->alphaDC() != NULL) ? true : false;
      OdBinaryData alphaPixels;
      OdUInt32 alphaBoundRectFullWidth = (boundRectWidth * 3 + 3) & ~3;
      OdUInt32 alphaSize = boundRectHeight * alphaBoundRectFullWidth;
      if (bGetAlpha)
      {
      alphaPixels.resize(alphaSize);
      memset(&alphaPixels[0], 0, alphaSize);
    } */

      for(i = boundRectY; i < boundRectY + boundRectHeight; i ++)
      {
        for(j = boundRectX; j < boundRectX + boundRectWidth; j ++)
        {
          OdGePoint2d ptUV = screen2UV * OdGePoint2d(j, i);
          OdUInt32 x = OdTruncateToLong(ptUV.x - 0.5);
          OdUInt32 y = OdTruncateToLong(ptUV.y - 0.5);
          if(x >= 0 && y >= 0 && x < imgWidth && y < imgHeight )
          {
            /* OdUInt8 blue, green, red;
            getImageColorAt(OdRoundToLong(x), OdRoundToLong(y), blue, green, red, pImg);
            pixels[boundRectFullWidth * (boundRectHeight - i - 1 + boundRectY) + (j - boundRectX) * 3]     = blue;
            pixels[boundRectFullWidth * (boundRectHeight - i - 1 + boundRectY) + (j - boundRectX) * 3 + 1] = green;
            pixels[boundRectFullWidth * (boundRectHeight - i - 1 + boundRectY) + (j - boundRectX) * 3 + 2] = red;

            if (bGetAlpha)
            {
            alphaPixels[alphaBoundRectFullWidth * (boundRectHeight - i - 1 + boundRectY) + (j - boundRectX) * 3] = 255;
            alphaPixels[alphaBoundRectFullWidth * (boundRectHeight - i - 1 + boundRectY) + (j - boundRectX) * 3 + 1] = 255;
            alphaPixels[alphaBoundRectFullWidth * (boundRectHeight - i - 1 + boundRectY) + (j - boundRectX) * 3 + 2] = 255;
            } */
            if(srcColDepth == 24)
              memcpy(pixels.asArrayPtr() +
                     dstScLnSz * (boundRectHeight - i - 1 + boundRectY) +
                     (j - boundRectX) * 3,
                     pSrcPixels +
                     srcScLnSz * y +
                     x * 3,
                     3);
            else if(srcColDepth == 16)
              memcpy(pixels.asArrayPtr() +
                     dstScLnSz * (boundRectHeight - i - 1 + boundRectY) +
                     (j - boundRectX) * 2,
                     pSrcPixels +
                     srcScLnSz * y +
                     x * 2,
                     2);
            else if(srcColDepth == 32)
              memcpy(pixels.asArrayPtr() +
                     dstScLnSz * (boundRectHeight - i - 1 + boundRectY) +
                     (j - boundRectX) * 4,
                     pSrcPixels +
                     srcScLnSz * y +
                     x * 4,
                     4);
            else // paletted
            {
              const OdUInt8* ptr = pSrcPixels + y * srcScLnSz + x * srcColDepth / 8;
              OdUInt32 index =
                (ptr[0] >>
                (
                ((8 / srcColDepth) - 1 - (x % (8 / srcColDepth)))
                * srcColDepth
                )
                )
                &
                ((1 << srcColDepth) - 1);
              OdUInt32 dstX, dstY;
              dstX = j - boundRectX;
              dstY = boundRectHeight - i - 1 + boundRectY;
              OdUInt8* dstPtr = (OdUInt8*)pixels.asArrayPtr() +
                dstY * dstScLnSz + dstX * srcColDepth / 8;

              // (0xFF - (1 << (7-dstX%8)));
              // (0xFF - (15 << ((1-dstX%2)<<2) ));

              // maskBlackIndex << (7-dstX%8);
              // index << ((1-dstX%2)<<2);
              OdUInt8 bitPerCol = (OdUInt8)(8/srcColDepth);
              dstPtr[0] &= (0xFF - (((1<<srcColDepth)-1) << ((bitPerCol-1-dstX%bitPerCol)*srcColDepth)));
              dstPtr[0] |= index << ((bitPerCol-1-dstX%bitPerCol)*srcColDepth);
            }
          }
        }
      }
      BITMAPINFO* pBoundRectH;
      OdUInt8Array headerCont;
      headerCont.resize(sizeof(BITMAPINFOHEADER) + pImg->paletteDataSize());
      pBoundRectH = (BITMAPINFO*)headerCont.asArrayPtr();
      pBoundRectH->bmiHeader.biBitCount = (WORD)srcColDepth;
      pBoundRectH->bmiHeader.biClrImportant = 0;
      pBoundRectH->bmiHeader.biClrUsed = 0;
      pBoundRectH->bmiHeader.biCompression = BI_RGB;
      pBoundRectH->bmiHeader.biHeight = boundRectHeight;
      pBoundRectH->bmiHeader.biPlanes = 1;
      pBoundRectH->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
      pBoundRectH->bmiHeader.biSizeImage = 0;
      pBoundRectH->bmiHeader.biWidth = boundRectWidth;
      pBoundRectH->bmiHeader.biXPelsPerMeter = 0;
      pBoundRectH->bmiHeader.biYPelsPerMeter = 0;
      pImg->paletteData((OdUInt8*)pBoundRectH->bmiColors);
      void* pBuf;
      HBITMAP hBmp = CreateDIBSection(hdc, pBoundRectH, DIB_RGB_COLORS, &pBuf, NULL, 0);
      if(pBuf)
        memcpy(pBuf, pixels.asArrayPtr(), boundRectHeight*dstScLnSz);
      HDC sourceDC = CreateCompatibleDC(hdc);
      if (sourceDC)
      {
        SelectObject(sourceDC, hBmp);
        BitBlt(hdc, boundRectX, boundRectY,
          boundRectWidth, boundRectHeight, sourceDC, 0, 0, SRCCOPY);
        DeleteDC(sourceDC);
      }
#ifndef _WIN32_WCE
      else
      {
        // Probably we're inside metafile (#7066)
        SetDIBitsToDevice(hdc, boundRectX, boundRectY, boundRectWidth, boundRectHeight,
          0, 0, 0, boundRectHeight, pBuf, pBoundRectH, DIB_RGB_COLORS);
      }
#endif
      DeleteObject(hBmp);
    }
  }

  if(numBoundPts)
    popViewportClip();
}

void ExGiGDIGeometry::metafileProc(const OdGePoint3d& origin,
                                   const OdGeVector3d& u,
                                   const OdGeVector3d& v,
                                   const OdGiMetafile* pMetafile,
                                   bool /*bDcAligned*/,
                                   bool /*bAllowClipping*/)
{
  if(isDragging())
    return ;

  if(renderMode() == OdGsView::k2DOptimized)
  {
    RECT rect;
//    if(bDcAligned)
    {
      OdGeExtents3d ext(origin, origin);
      ext.addPoint(origin + u);
      ext.addPoint(origin + v);
      ext.addPoint(origin + u + v);

      rect.left   = ROUND(ext.minPoint().x);
      rect.right  = ROUND(ext.maxPoint().x);
      rect.top    = ROUND(ext.minPoint().y);
      rect.bottom = ROUND(ext.maxPoint().y);
    }
//    else
    {
      //TODO: explode WMF here...
    }

#ifndef _WIN32_WCE
    OdUInt32 nSize = pMetafile->dataSize();
    const OdUInt8* pData = pMetafile->bitsData();
    OdUInt8Array data;
    if(!pData)
    {
      data.resize(nSize);
      pMetafile->bitsData(data.asArrayPtr());
      pData = data.getPtr();
    }

    HENHMETAFILE hEmf = ::SetEnhMetaFileBits(nSize, pData);
    if (!hEmf) // It was apparently not an enhanced metafile, so try 16bit windows metafile
    {
      METAFILEPICT mfp = { MM_ANISOTROPIC, Od_abs(rect.right - rect.left), Od_abs(rect.top - rect.bottom), 0 };
      mfp.xExt *= 100;
      mfp.yExt *= 100;
      hEmf = ::SetWinMetaFileBits(nSize, pData, NULL, &mfp);
    }

    if(hEmf)
    {
      pushViewportClip(OdGsDCPoint(rect.left, rect.bottom), OdGsDCPoint(rect.right, rect.top));
      ::FillRect(m_hTargetDC, &rect, ::GetSysColorBrush(COLOR_WINDOW));
      ::PlayEnhMetaFile(m_hTargetDC, hEmf, (LPRECT)&rect);
      popViewportClip();
      ::DeleteEnhMetaFile(hEmf);
    }
#endif
  }
}

inline bool isDcAligned(const OdGeVector3d& u, const OdGeVector3d& v, bool bRotated = true)
{
  if (bRotated)
    return (u.isParallelTo(OdGeVector3d::kXAxis) && v.isParallelTo(OdGeVector3d::kYAxis) ||
            u.isParallelTo(OdGeVector3d::kYAxis) && v.isParallelTo(OdGeVector3d::kXAxis));
  return u.isCodirectionalTo(OdGeVector3d::kXAxis) && v.isCodirectionalTo(-OdGeVector3d::kYAxis);
}

#ifndef _WIN32_WCE
static bool isMetafileDc(HDC hTargetDc)
{
  return ((::GetDeviceCaps(hTargetDc, TECHNOLOGY) == DT_METAFILE) || // WMF
          (::GetObjectType((HGDIOBJ)hTargetDc) == OBJ_ENHMETADC));   // EMF
}
static bool drawOwnerDrawDcThroughMemDc(const OdGiSelfGdiDrawable* pDrawable,
                                        const OdGiCommonDraw& context, const OdGsDCRect &outputRect,
                                        HDC hTargetDc, const OdGsDCRect &rect)
{
  RECT clipRect;
  ::SetRect(&clipRect, odmin(outputRect.m_min.x, outputRect.m_max.x), odmin(outputRect.m_min.y, outputRect.m_max.y),
            odmax(outputRect.m_min.x, outputRect.m_max.x), odmax(outputRect.m_min.y, outputRect.m_max.y));
  if (clipRect.left < rect.m_min.x) clipRect.left = rect.m_min.x;
  if (clipRect.right > rect.m_max.x) clipRect.right = rect.m_max.x;
  if (clipRect.top < rect.m_max.y) clipRect.top = rect.m_max.y;
  if (clipRect.bottom > rect.m_min.y) clipRect.bottom = rect.m_min.y;
  if (clipRect.left >= clipRect.right) return true;
  if (clipRect.top >= clipRect.bottom) return true;
  HDC hMemDc = ::CreateCompatibleDC(hTargetDc);
  if (!hMemDc) return false; // Actual for WMF
  LONG clipWidth = clipRect.right - clipRect.left,
       clipHeight = clipRect.bottom - clipRect.top;
  const LONG limit = 100000; // @@@TODO: Investigate limitation
  const bool bMaxLimit = isMetafileDc(hTargetDc);
  const LONG maxLimit = 2000; // Shouldn't be lower 'limit' since rescale only single image
  if (clipWidth > limit || clipHeight > limit)
  {
    // Try to render image by parts to make possible to view in EMF
    HBITMAP hMem = ::CreateCompatibleBitmap(hTargetDc, limit, limit);
    HGDIOBJ hPrevBmp = ::SelectObject(hMemDc, (HGDIOBJ)hMem);
    LONG clipStepX, clipStepY = clipRect.top;
    //int n = 0;
    while (clipStepY < clipHeight)
    {
      clipStepX = clipRect.left;
      while (clipStepX < clipWidth)
      {
        RECT actRect;
        ::SetRect(&actRect, clipStepX, clipStepY, odmin(clipStepX + limit, clipRect.right), odmin(clipStepY + limit, clipRect.bottom));
        ::SetViewportOrgEx(hMemDc, -actRect.left, -actRect.top, NULL);
        pDrawable->draw(context, (void*)hMemDc, rect);
        ::BitBlt(hTargetDc, actRect.left, actRect.top, actRect.right - actRect.left, actRect.bottom - actRect.top,
                 hMemDc, actRect.left, actRect.top, SRCCOPY);
        ::MoveToEx(hTargetDc, actRect.left, actRect.top, NULL);
        ::LineTo(hTargetDc, actRect.right, actRect.bottom);
        clipStepX += limit;
      }
      clipStepY += limit;
    }
    ::SelectObject(hMemDc, hPrevBmp);
    ::DeleteObject((HGDIOBJ)hMem);
  }
  else
  {
    if (!bMaxLimit || ((clipWidth <= maxLimit) && (clipHeight <= maxLimit)))
    {
      HBITMAP hMem = ::CreateCompatibleBitmap(hTargetDc, clipWidth, clipHeight);
      HGDIOBJ hPrevBmp = ::SelectObject(hMemDc, (HGDIOBJ)hMem);
      ::SetViewportOrgEx(hMemDc, -clipRect.left, -clipRect.top, NULL);
      pDrawable->draw(context, (void*)hMemDc, rect);
      ::BitBlt(hTargetDc, clipRect.left, clipRect.top, clipWidth, clipHeight,
               hMemDc, clipRect.left, clipRect.top, SRCCOPY);
      ::SelectObject(hMemDc, hPrevBmp);
      ::DeleteObject((HGDIOBJ)hMem);
    }
    else
    {
      const LONG clipMax = odmax(clipWidth, clipHeight);
      const LONG repWidth  = (LONG)::MulDiv((int)clipWidth , (int)maxLimit, (int)clipMax);
      const LONG repHeight = (LONG)::MulDiv((int)clipHeight, (int)maxLimit, (int)clipMax);
      if (repWidth && repHeight)
      {
        HBITMAP hMem = ::CreateCompatibleBitmap(hTargetDc, repWidth, repHeight);
        HGDIOBJ hPrevBmp = ::SelectObject(hMemDc, (HGDIOBJ)hMem);
        const LONG crpLeft = (LONG)::MulDiv((int)clipRect.left, (int)maxLimit, (int)clipMax);
        const LONG crpTop  = (LONG)::MulDiv((int)clipRect.top , (int)maxLimit, (int)clipMax);
        ::SetViewportOrgEx(hMemDc, -crpLeft, -crpTop, NULL);
        const OdGsDCRect repRect(::MulDiv((int)rect.m_min.x, (int)maxLimit, (int)clipMax),
                                 ::MulDiv((int)rect.m_max.x, (int)maxLimit, (int)clipMax),
                                 ::MulDiv((int)rect.m_min.y, (int)maxLimit, (int)clipMax),
                                 ::MulDiv((int)rect.m_max.y, (int)maxLimit, (int)clipMax));
        pDrawable->draw(context, (void*)hMemDc, repRect);
        ::StretchBlt(hTargetDc, clipRect.left, clipRect.top, clipWidth, clipHeight,
                     hMemDc, crpLeft, crpTop, repWidth, repHeight, SRCCOPY);
        ::SelectObject(hMemDc, hPrevBmp);
        ::DeleteObject((HGDIOBJ)hMem);
      }
    }
  }
  ::DeleteDC(hMemDc);
  return true;
}
#endif

void ExGiGDIGeometry::ownerDrawDc(const OdGiCommonDraw& context,
                                  const OdGePoint3d& origin,
                                  const OdGeVector3d& u,
                                  const OdGeVector3d& v,
                                  const OdGiSelfGdiDrawable* pDrawable,
                                  bool bDcAligned,
                                  bool /*bAllowClipping*/)
{
#ifndef _WIN32_WCE
  if (m_hTargetDC)
  {
    if (/*bDcAligned &&*/ !isDcAligned(u, v, bDcAligned))
      return;

    OdGsDCRect rect;

    OdGeExtents3d ext(origin, origin);
    ext.addPoint(origin + u);
    ext.addPoint(origin + v);
    ext.addPoint(origin + u + v);
    
    try
    {
      rect.m_min.x = ROUND(ext.minPoint().x);
      rect.m_max.x = ROUND(ext.maxPoint().x);
      rect.m_max.y = ROUND(ext.minPoint().y);
      rect.m_min.y = ROUND(ext.maxPoint().y);
    }
    catch (const OdError&)
    {
      // #10225 : Can't render if arithmetic overflow thrown
      return;
    }

    pushViewportClip(rect.m_min, rect.m_max);
    /* #11797 : unfortunately this is impossible to solve without intermediate context. In attached file
                OLE object contains metafile. Metafile playing cause clipping boundary corruption:
                internally it is call RestoreDC(dc, 1) which cause removing of all stored clipping and
                after that it is reset clipping. I think solution with intermediate context can be
                helpful for other cases: when exist context settings corrupt output of OLE object, and
                when OLE object corrupts some settings of exist context. This solution cause little
                performance loss, but it is not critical because it is provide more stable results.      */
    if (!drawOwnerDrawDcThroughMemDc(pDrawable, context, m_rcTarget, m_hTargetDC, rect))
      pDrawable->draw(context, (void*)m_hTargetDC, rect); 
    popViewportClip();
  }
#endif
}

void ExGiGDIGeometry::polypointProc(OdInt32 numPoints, const OdGePoint3d* vertexList,
                                    const OdCmEntityColor* pColors, const OdCmTransparency* pTransparency,
                                    const OdGeVector3d* pNormals, const OdGeVector3d* pExtrusions,
                                    const OdGsMarker* pSubEntMarkers, OdInt32 nPointSize)
{
  struct LwdOverrideSaver : public ExGiGDIGeometry::LwdOverrideSettings
  {
    ExGiGDIGeometry *m_pGeom;
    LwdOverrideSaver(ExGiGDIGeometry *pGeom) : ExGiGDIGeometry::LwdOverrideSettings(false, false, 0), m_pGeom(pGeom) {}
    void doOverride(OdInt32 nPointSize)
    { if (m_pGeom) m_pGeom->setLineweightOverride(ExGiGDIGeometry::LwdOverrideSettings(true, true, (int)nPointSize), this); }
    ~LwdOverrideSaver() { if (m_pGeom) m_pGeom->setLineweightOverride(*this); }
  } _los((nPointSize > 0) ? this : NULL);
  _los.doOverride(nPointSize);
  OdGiGeometrySimplifier::polypointProc(numPoints, vertexList, pColors, pTransparency, pNormals, pExtrusions, pSubEntMarkers, nPointSize);
}

//*******************************************************************************/
// draw_color
//*******************************************************************************/
void ExGiGDIGeometry::draw_color(const OdCmEntityColor& color)
{
  if(color.isByACI() || color.isByDgnIndex())
    draw_color_index(color.colorIndex());
  else
    draw_color(ODRGB(color.red(), color.green(), color.blue()));
}

//*******************************************************************************/
// draw_color
//*******************************************************************************/
void ExGiGDIGeometry::draw_color(COLORREF rgb)
{
  if (m_bGlobalAlpha)
  {
    rgb = ::OdMergeRGBAlpha(m_clrFadeColor, rgb, (OdUInt8)m_nGlobalAlpha);
  }
  if (m_pColorConverter)
  {
    if (m_pColorConverter->convertBackgroundColors() || !GETBIT(drawContext()->effectiveTraits().drawFlags(), OdGiSubEntityTraits::kDrawNoPlotstyle))
      rgb = m_pColorConverter->convert(rgb);
  }
  if(m_penBrush.lbColor != rgb)
  {
    m_bBrushValid = m_bPenValid = m_bThinPenValid = false;
    m_penBrush.lbColor = m_logBrush.lbColor = rgb;
    if(m_hTargetDC) // in some cases m_hTargetDC is not set yet when this function called
      SetTextColor(m_hTargetDC, rgb);
  }
}

//*******************************************************************************/
// draw_color_index
//*******************************************************************************/
void ExGiGDIGeometry::draw_color_index(int colorIndex)
{
  if(colorIndex < m_numPaletteColors)
  {
    draw_color(m_paletteColors[colorIndex]);
  }
}

void ExGiGDIGeometry::draw_lineweight(int nPixels)
{
  if(m_dwWidth != (DWORD)nPixels && !m_bLwdOverride)
  {
    m_bPenValid = false;
    m_dwWidth = nPixels;
  }
}

void ExGiGDIGeometry::draw_pslinetype(ExGiGDIGeometry::LineType linetype)
{
  if(linetype != m_oldLinetype)
  {
    m_oldLinetype = LineType(linetype);
    m_bPenValid = false;
    if ((linetype > kLtpSolid) && (linetype < kLtpNull))
    {
#ifndef _WIN32_WCE
      m_dwPenStyle |= PS_USERSTYLE;
      m_dwPenStyle |= PS_GEOMETRIC;
#endif
      m_dwStyleCount = (DWORD)m_psLinetypeDefs[linetype].m_numDashes;
      m_lpStyle = (DWORD*)m_psLinetypeDefs[linetype].m_dashLength;
    }
    else if(linetype == kLtpForSelection)
    {
      m_oldLinetype = kLtpForSelection;
#ifndef _WIN32_WCE
      m_dwPenStyle &= ~PS_USERSTYLE;
      m_dwPenStyle &= ~PS_GEOMETRIC;
      m_dwPenStyle |= PS_DOT | PS_COSMETIC;
#endif
      m_dwStyleCount = 0;
    }
    else
    {
#ifndef _WIN32_WCE
      m_dwPenStyle &= ~PS_USERSTYLE;
      m_dwPenStyle |= PS_GEOMETRIC;
#endif
      m_dwStyleCount = 0;
    }
  }
}

void ExGiGDIGeometry::draw_endStyle(OdPs::LineEndStyle endStyle)
{
#ifndef _WIN32_WCE
  if(endStyle != m_oldEndStyle)
  {
    m_dwPenStyle &= ~(PS_ENDCAP_FLAT | PS_ENDCAP_SQUARE | PS_ENDCAP_ROUND);
    m_bPenValid = false;
    m_oldEndStyle = endStyle;
    // taking end style into account
    switch(endStyle)
    {
    case OdPs::kLesButt:
      m_dwPenStyle |= PS_ENDCAP_FLAT;
      break;
    case OdPs::kLesSquare:
      m_dwPenStyle |= PS_ENDCAP_SQUARE;
      break;
    case OdPs::kLesRound:
    case OdPs::kLesDiamond: // NB: kDiamondLES is not supported
      m_dwPenStyle |= PS_ENDCAP_ROUND;
      break;
    }
  }
#endif
}

void ExGiGDIGeometry::draw_joinStyle(OdPs::LineJoinStyle joinStyle)
{
#ifndef _WIN32_WCE
  if(joinStyle != m_oldJoinStyle)
  {
    m_oldJoinStyle = joinStyle;
    m_bPenValid = false;
    m_dwPenStyle &= ~(PS_JOIN_MITER|PS_JOIN_BEVEL|PS_JOIN_ROUND);
    switch(joinStyle)
    {
    case OdPs::kLjsMiter:
      m_dwPenStyle |= PS_JOIN_MITER;
      break;
    case OdPs::kLjsBevel:
      m_dwPenStyle |= PS_JOIN_BEVEL;
      break;
    case OdPs::kLjsRound:
    case OdPs::kLjsDiamond: // NB: kDiamondLJS is not supported
      m_dwPenStyle |= PS_JOIN_ROUND;
      break;
    }
  }
#endif
}

static const OdUInt16 g_selPatScLns[8] =
{
  0x22, 0x55, 0x88, 0x55, 0x22, 0x55, 0x88, 0x55
};
static const OdInt16 g_htPsMapping[9] =
{
  // Use default WinGDI predefined styles
#ifndef _WIN32_WCE
  BS_HATCHED, -1, HS_CROSS, HS_DIAGCROSS, HS_HORIZONTAL, HS_BDIAGONAL, HS_FDIAGONAL, -1, HS_VERTICAL
#else
  -1, -1, -1, -1, -1, -1, -1, -1, -1
#endif
};

void ExGiGDIGeometry::draw_fillStyle(ExGiGDIGeometry::FillStyle fillStyle)
{
  // ttf text can have fillstyles "for selection", "Hollow"(?), or "Solid"
  // all other fillstyles are concidered "solid"
  if(m_bProcessingTTF && (fillStyle != kFsForSelection) && (fillStyle != kFsHollow))
    fillStyle = kFsSolid;

  if(m_oldFillStyle != fillStyle)
  {
    m_oldFillStyle = FillStyle(fillStyle);
    m_bBrushValid = false;

    if(m_logBrush.lbStyle == BS_PATTERN)
      ::DeleteObject((HBITMAP)m_logBrush.lbHatch); // delete old bitmap
    
    const OdUInt16 *pScanLines = NULL;
    switch (fillStyle)
    {
      case kFsHollow:
        m_logBrush.lbStyle = BS_HOLLOW;
      break;
      case kFsSolid:
        m_logBrush.lbStyle = BS_SOLID;
      break;
      case kFsForSelection:
        pScanLines = g_selPatScLns;
      break;
      default:
        if (fillStyle > kFsSolid && fillStyle < kFsHollow)
        {
          const OdUInt32 nFs = fillStyle - kFsSolid;
          if (g_htPsMapping[nFs] >= 0)
          {
            m_logBrush.lbHatch = (unsigned)g_htPsMapping[nFs];
            m_logBrush.lbStyle = (unsigned)g_htPsMapping[0];
          }
          else
            pScanLines = m_psFillstyleDefs[nFs];
        }
        else
        {
          ODA_FAIL();
        }
      break;
    }
    if (pScanLines)
    {
      HBITMAP hPatBmp = ::CreateBitmap(8, 8, 1, 1, pScanLines);
#ifdef __int3264
      m_logBrush.lbHatch = (ULONG_PTR)hPatBmp;
#else
      m_logBrush.lbHatch = (LONG)hPatBmp;
#endif
      m_logBrush.lbStyle = BS_PATTERN;
    }
  }
}

void ExGiGDIGeometry::draw_bkMode(int bkMode)
{
  if(m_oldBkMode != bkMode)
  {
    m_oldBkMode = bkMode;
    ::SetBkMode(m_hTargetDC, bkMode);
  }
}

inline bool ExGiGDIGeometry::isExtPen() const
{
#ifdef _WIN32_WCE
  return false;
#else
  return (m_dwPenStyle & PS_STYLE_MASK)==PS_USERSTYLE ||
    (m_dwPenStyle & (PS_ENDCAP_FLAT|PS_ENDCAP_SQUARE|PS_JOIN_MITER|PS_JOIN_BEVEL))!=0;
#endif
}

HPEN ExGiGDIGeometry::createCurrentPen()
{
#ifndef _WIN32_WCE
  if(isExtPen())
  {
    return ::ExtCreatePen(m_dwPenStyle, m_dwWidth, &m_penBrush, m_dwStyleCount, 
      m_dwPenStyle & PS_USERSTYLE ? m_lpStyle : NULL);
  }
  else
#endif
  {
    LOGPEN pen = {PS_SOLID, {(LONG)m_dwWidth,0}, m_penBrush.lbColor};
    return ::CreatePenIndirect(&pen);
  }
}

void ExGiGDIGeometry::updatePenNBrush(bool bForSelection)
{
  if( !m_bPenValid  )
  {
    if ( m_hTargetDC )
    {
#ifndef _WIN32_WCE
      if(bForSelection)
      {
        ::DeleteObject(::SelectObject(m_hTargetDC, ::CreatePen(m_dwPenStyle, m_dwWidth, 
          m_penBrush.lbColor)));
      }
      else
      {
        ::DeleteObject(::SelectObject(m_hTargetDC, createCurrentPen()));
      }
#else
      ::DeleteObject(::SelectObject(m_hTargetDC, 
        ::CreatePen(m_dwPenStyle, m_dwWidth, m_penBrush.lbColor)));
#endif
    }
    m_bPenValid = true;
  }
  if(!m_bBrushValid)
  {
#ifndef _WIN32_WCE
    if ( m_hTargetDC )
      ::DeleteObject(::SelectObject(m_hTargetDC, ::CreateBrushIndirect(&m_logBrush)));
#endif
    m_bBrushValid = true;
  }
}

HGDIOBJ ExGiGDIGeometry::thinPen()
{
  if (!m_bThinPenValid)
  {
    if (m_hTargetDC)
    {
      ::DeleteObject(m_hThinPen);
      m_hThinPen = ::CreatePen(PS_SOLID, 0, m_penBrush.lbColor);
    }
    m_bThinPenValid = true;
  }
  return (HGDIOBJ)m_hThinPen;
}

void ExGiGDIGeometry::renderGradientTriangles( HDC hdc )
{
#ifndef _WIN32_WCE
  // Draw gradients as bitmap
  // Compute size of gradient
  SIZE minLim; minLim.cx = m_triVertexes.first().x; minLim.cy = m_triVertexes.first().y;
  SIZE maxLim; maxLim = minLim;
  TRIVERTEX *pTriVerts = m_triVertexes.asArrayPtr();
  OdUInt32 nVerts = m_triVertexes.size(), i;
  for (i = 1; i < nVerts; i++) {
    if (pTriVerts[i].x < minLim.cx) minLim.cx = pTriVerts[i].x;
    else if (pTriVerts[i].x > maxLim.cx) maxLim.cx = pTriVerts[i].x;
    if (pTriVerts[i].y < minLim.cy) minLim.cy = pTriVerts[i].y;
    else if (pTriVerts[i].y > maxLim.cy) maxLim.cy = pTriVerts[i].y;
  }
  // Clip gradient by output rect
  if (minLim.cx < m_rcTarget.m_min.x)
    minLim.cx = m_rcTarget.m_min.x;
  if (maxLim.cx > m_rcTarget.m_max.x)
    maxLim.cx = m_rcTarget.m_max.x;
  if (minLim.cy < m_rcTarget.m_max.y)
    minLim.cy = m_rcTarget.m_max.y;
  if (maxLim.cy > m_rcTarget.m_min.y)
    maxLim.cy = m_rcTarget.m_min.y;
  if ((minLim.cx < maxLim.cx) && (minLim.cy < maxLim.cy))
  {
    SIZE bmpSize; bmpSize.cx = maxLim.cx - minLim.cx + 1; bmpSize.cy = maxLim.cy - minLim.cy + 1;

    RECT bmpRect; bmpRect.left = bmpRect.top = 0; bmpRect.right = bmpSize.cx; bmpRect.bottom = bmpSize.cy;

    HDC hMemDC = ::CreateCompatibleDC(NULL);
    BITMAPINFO biInfo;
    biInfo.bmiHeader.biBitCount = 32;
    biInfo.bmiHeader.biClrImportant = 0;
    biInfo.bmiHeader.biClrUsed = 0;
    biInfo.bmiHeader.biCompression = BI_RGB;
    biInfo.bmiHeader.biHeight = -bmpSize.cy;
    biInfo.bmiHeader.biPlanes = 1;
    biInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    biInfo.bmiHeader.biSizeImage = 0;
    biInfo.bmiHeader.biWidth = bmpSize.cx;
    biInfo.bmiHeader.biXPelsPerMeter = 0;
    biInfo.bmiHeader.biYPelsPerMeter = 0;
    OdUInt8 *pPixels;
    HBITMAP hBmp = ::CreateDIBSection(NULL, &biInfo, DIB_RGB_COLORS, (void**)&pPixels, NULL, 0);
    OdUInt32 nPixels = bmpSize.cx * bmpSize.cy;
    OdUInt8 *pPixelsBuf = pPixels;

    HBITMAP hPrevMemBitmap = (HBITMAP)::SelectObject(hMemDC, (HGDIOBJ)hBmp);
//    ::SetWindowOrgEx(hMemDC, minLim.cx, minLim.cy, NULL);

    ExGsGradientRender render;
    render.setupContext32BPP( bmpSize.cx, bmpSize.cy, pPixels );
    render.setOrigin( OdGePoint2d( minLim.cx, minLim.cy ) );
    render.setClipRect( OdGsDCRect( minLim.cx, maxLim.cx, minLim.cy, maxLim.cy ) );
    render.eraseBackground();
    render.renderTriangles( pTriVerts, nVerts,  m_triangles.asArrayPtr(), m_triangles.size() );

//    ::SetWindowOrgEx(hMemDC, 0, 0, NULL);

    BLENDFUNCTION bf;
    bf.AlphaFormat = AC_SRC_ALPHA;
    bf.BlendFlags = 0;
    bf.BlendOp = AC_SRC_OVER;
    bf.SourceConstantAlpha = 255;
    ((ALPHABLENDPROC)m_alphaBlendPrAddr)(hdc, minLim.cx, minLim.cy, bmpSize.cx, bmpSize.cy, hMemDC, 0, 0, bmpSize.cx, bmpSize.cy, bf);

    ::SelectObject(hMemDC, (HGDIOBJ)hPrevMemBitmap);
    ::DeleteObject((HGDIOBJ)hBmp);
    ::DeleteDC(hMemDC);
  }
#endif
}

//
