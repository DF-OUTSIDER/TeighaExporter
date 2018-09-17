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
//
//
//////////////////////////////////////////////////////////////////////


#include "PdfExportCommon.h"
#include "PdfShxGeomStore.h"
#include "PdfIContentCommands4Type3.h"
#include "PdfIContentCommands.h"
#include "Ge/GeEllipArc3d.h"
#include "Ge/GeEllipArc2d.h"
#include "Ge/GeExtents3d.h"

namespace TD_PDF_2D_EXPORT {

PDFShxGeomStore::PDFShxGeomStore(PDFIContentCommands4Type3 *pCommands4T3, PDFIContentCommands *pCommands)
  : m_pCommands(pCommands)
  , m_pCommands4T3(pCommands4T3)
  , m_dGeomScale(1.)
{
}

void PDFShxGeomStore::setScale(double dGeomScale)
{
  m_dGeomScale = dGeomScale;
}

double PDFShxGeomStore::getScale() const
{
  return m_dGeomScale;
}

double PDFShxGeomStore::getCharWidth() const
{
  return m_adv.x * m_dGeomScale;
}

void PDFShxGeomStore::polylineProc(
    OdInt32 numPoints, const OdGePoint3d* vertexList,
    const OdGeVector3d* /*pNormal*/,
    const OdGeVector3d* /*pExtrusion*/,
    OdGsMarker /*baseSubEntMarker*/)
{
  if (numPoints)
  {
    OdGePoint3dArray pArr; pArr.resize(numPoints);

    for(OdInt32 f=0; f<numPoints; ++f)
    {
      pArr[f] = vertexList[f];
    }

    m_Lines.append(pArr);
  }
}

void PDFShxGeomStore::circleProc(const OdGePoint3d& center, double radius, const OdGeVector3d& normal, const OdGeVector3d* /*pExtrusion*/)
{
  OdGeCircArc3d arc(center, normal, radius);
  m_Circles.append(arc);

  //OdGePoint3dArray pnt3d;
  //arc.getSamplePoints(arc.startAng(), arc.endAng(), 0., pnt3d);ODA_ASSERT(pnt3d.size());
  //polylineProc(pnt3d.size(), pnt3d.asArrayPtr());
}

void PDFShxGeomStore::circularArcProc(
    const OdGePoint3d& center,
    double radius,
    const OdGeVector3d& normal,
    const OdGeVector3d& startVector,
    double sweepAngle,
    OdGiArcType /*arcType*/, const OdGeVector3d* /*pExtrusion*/)
{
  OdGeCircArc3d arc(center, normal, startVector, radius, 0., sweepAngle);
  m_Circles.append(arc);

  //OdGePoint3dArray pnt3d;
  //arc.getSamplePoints(arc.startAng(), arc.endAng(), 0., pnt3d); ODA_ASSERT(pnt3d.size());
  //polylineProc(pnt3d.size(), pnt3d.asArrayPtr());
}

void PDFShxGeomStore::circularArcProc(
    const OdGePoint3d& firstPoint,
    const OdGePoint3d& secondPoint,
    const OdGePoint3d& thirdPoint,
    OdGiArcType /*arcType*/, const OdGeVector3d* /*pExtrusion*/)  
{
  OdGeCircArc3d arc(firstPoint, secondPoint, thirdPoint);
  m_Circles.append(arc);

  //OdGePoint3dArray pnt3d;
  //arc.getSamplePoints(arc.startAng(), arc.endAng(), 0., pnt3d); ODA_ASSERT(pnt3d.size());
  //polylineProc(pnt3d.size(), pnt3d.asArrayPtr());
}

void PDFShxGeomStore::setAdvance(OdGePoint2d &adv)
{
  m_adv = adv;
}

void PDFShxGeomStore::CalculateBBox(PDFBBoxStore &pBBox)
{
  // space hasn't geometry
  //ODA_ASSERT(!m_Lines.isEmpty());
  //ODA_ASSERT(!m_Lines.first().isEmpty());

  OdArray<OdGePoint3dArray>::iterator pIt = m_Lines.begin();
  OdArray<OdGePoint3dArray>::iterator pE  = m_Lines.end();

  while(pIt != pE)
  {
    OdGePoint3dArray::iterator pPnt = pIt->begin();
    OdGePoint3dArray::iterator pEPnt  = pIt->end();
    while(pPnt != pEPnt)
    {
      pBBox.add(pPnt->x, pPnt->y);
      ++pPnt;
    }
    ++pIt;
  }
  
  OdArray<OdGeCircArc3d>::iterator pItc = m_Circles.begin();
  OdArray<OdGeCircArc3d>::iterator pEc  = m_Circles.end();

  while(pItc != pEc)
  {
    OdGeExtents3d extents;
    pItc->getGeomExtents(extents);

    pBBox.add(extents.minPoint().x, extents.minPoint().y);
    pBBox.add(extents.maxPoint().x, extents.maxPoint().y);

    ++pItc;
  }
}

void PDFShxGeomStore::shellProc(OdInt32 numVertices,
  const OdGePoint3d* vertexList,
  OdInt32 faceListSize,
  const OdInt32* faceList,
  const OdGiEdgeData* pEdgeData,
  const OdGiFaceData* pFaceData,
  const OdGiVertexData* pVertexData)
{
  //return;
  if (faceListSize == 0 || numVertices == 0)
    return;

  OdInt32 holes = 0, contours = 0;
  OdInt32 pointsCount = faceList[0];
  OdInt32 counter = 0;
  while (counter < faceListSize)
  {
    if (pointsCount < 0)
    {
      holes++;
      pointsCount = -pointsCount;
    }
    else
      contours++;
    counter += pointsCount + 1;
    if (counter < faceListSize)
      pointsCount = faceList[counter];
  }

  m_OuterContours.resize(contours);
  m_InnerContours.resize(holes);
  
  pointsCount = faceList[0];
  bool hole = false;
  if (pointsCount < 0)
  {
    pointsCount = -pointsCount;
    hole = true;
  }
  OdGePoint2dArray points;
  points.resize(pointsCount);
  for (OdInt32 i = 1; i < faceListSize; i++)
  {
    if (pointsCount)
    {
      points[pointsCount-1] = vertexList[faceList[i]].convert2d();
      pointsCount--;
    } 
    else
    {
      if (hole)
        m_InnerContours[--holes] = points;
      else
        m_OuterContours[--contours] = points;

      points.clear();
      pointsCount = faceList[i];
      
      hole = false;
      if (pointsCount < 0)
      {
        hole = true;
        pointsCount = -pointsCount;
      }
      points.resize(pointsCount);
    }
  }

  if (hole)
    m_InnerContours[--holes] = points;
  else
    m_OuterContours[--contours] = points;
}

#if 0
void PDFShxGeomStore::fillContent()
{
  CalculateBBox(m_BBox);

  if (m_BBox.isInit())
  {
    double llx, lly, urx, ury;
    m_BBox.get(llx, lly, urx, ury);
    m_pCommands4T3->d1(m_adv.x * m_dGeomScale, m_adv.y * m_dGeomScale, llx * m_dGeomScale, lly * m_dGeomScale, urx * m_dGeomScale, ury * m_dGeomScale);
  }
  else
  {
    m_pCommands4T3->d1(m_adv.x * m_dGeomScale, m_adv.y * m_dGeomScale, 0, 0, 0, 0);
  }

  OdArray<OdGePoint3dArray>::iterator pIt = m_Lines.begin();
  OdArray<OdGePoint3dArray>::iterator pE  = m_Lines.end();

  while(pIt != pE)
  {
    OdGePoint3dArray::iterator pPnt = pIt->begin();
    OdGePoint3dArray::iterator pEPnt  = pIt->end();

    if (pPnt != pEPnt)
    {
      m_pCommands->m(pPnt->x * m_dGeomScale, pPnt->y * m_dGeomScale); ++pPnt;

      while(pPnt != pEPnt)
      {
        m_pCommands->l(pPnt->x * m_dGeomScale, pPnt->y * m_dGeomScale);

        ++pPnt;
      }
      m_pCommands->S();
    }
    ++pIt;
  }

  OdArray<OdGeCircArc3d>::iterator pItc = m_Circles.begin();
  OdArray<OdGeCircArc3d>::iterator pEc  = m_Circles.end();

  while(pItc != pEc)
  {
    OdGeEllipArc3d ellipArc3d(*pItc);
    OdGeEllipArc2d ellipArc2d(
      ellipArc3d.center().convert2d(),
      ellipArc3d.majorAxis().convert2d(),
      ellipArc3d.minorAxis().convert2d(),
      ellipArc3d.majorRadius(),
      ellipArc3d.minorRadius(),
      ellipArc3d.startAng(),
      ellipArc3d.endAng()
      );

    PDFIContentCommands::drawEllipse(*m_pCommands, ellipArc2d);
    m_pCommands->S();

    ++pItc;
  }
}
#else
#define USE_D0
void PDFShxGeomStore::fillContent(double fontLW)
{
#ifdef USE_D0
  // #10496
  m_pCommands4T3->d0(m_adv.x * m_dGeomScale, m_adv.y * m_dGeomScale);
#else
  CalculateBBox(m_BBox);

  if (m_BBox.isInit())
  {
    double llx, lly, urx, ury;
    m_BBox.get(llx, lly, urx, ury);
    m_pCommands4T3->d1(m_adv.x * m_dGeomScale, m_adv.y * m_dGeomScale, llx * m_dGeomScale, lly * m_dGeomScale, urx * m_dGeomScale, ury * m_dGeomScale);
  }
  else
  {
    m_pCommands4T3->d1(m_adv.x * m_dGeomScale, m_adv.y * m_dGeomScale, 0, 0, 0, 0);
  }
#endif
  m_pCommands->w(fontLW);

  //Set round line join and line cap style
  m_pCommands->j(kRoundJoin);
  m_pCommands->J(kRoundCap);

  OdArray<OdGePoint3dArray>::iterator pIt = m_Lines.begin();
  OdArray<OdGePoint3dArray>::iterator pE  = m_Lines.end();

  OdGePoint2d lastPoint, firstPointOfLoop;
  bool bLastPointInit = false;

  while(pIt != pE)
  {
    OdGePoint3dArray::iterator pPnt = pIt->begin();
    OdGePoint3dArray::iterator pEPnt  = pIt->end();

    if (pPnt != pEPnt)
    {
      if (!bLastPointInit)
      {
        m_pCommands->m(pPnt->x * m_dGeomScale, pPnt->y * m_dGeomScale); ++pPnt;
        firstPointOfLoop.set(pPnt->x, pPnt->y);
      }
      else
      {
        if ( !((OdGePoint2d*)pPnt)->isEqualTo(lastPoint) )
        {
          if (lastPoint.isEqualTo(firstPointOfLoop))
          {
            m_pCommands->h();
          }
          m_pCommands->S();
          
          firstPointOfLoop.set(pPnt->x, pPnt->y);
          m_pCommands->m(pPnt->x * m_dGeomScale, pPnt->y * m_dGeomScale); ++pPnt;
        }
      }

      while(pPnt != pEPnt)
      {
        m_pCommands->l(pPnt->x * m_dGeomScale, pPnt->y * m_dGeomScale);
      
        ++pPnt;
      }

      bLastPointInit = true;
      lastPoint.set(pIt->last().x, pIt->last().y);
    }
    ++pIt;
  }

  OdArray<OdGeCircArc3d>::iterator pItc = m_Circles.begin();
  OdArray<OdGeCircArc3d>::iterator pEc  = m_Circles.end();

  while(pItc != pEc)
  {
    OdGeEllipArc3d ellipArc3d(*pItc);
    OdGeEllipArc2d ellipArc2d(
      ellipArc3d.center().convert2d() * m_dGeomScale,
      ellipArc3d.majorAxis().convert2d(),
      ellipArc3d.minorAxis().convert2d(),
      ellipArc3d.majorRadius() * m_dGeomScale,
      ellipArc3d.minorRadius() * m_dGeomScale,
      ellipArc3d.startAng(),
      ellipArc3d.endAng()
      );


    if (!bLastPointInit)
    {
      PDFIContentCommands::drawEllipse(*m_pCommands, ellipArc2d, false);
    }
    else
    {
      OdGePoint2d tmp(pItc->startPoint().x, pItc->startPoint().y);
      if (lastPoint.isEqualTo(tmp))
      {
        PDFIContentCommands::drawEllipse(*m_pCommands, ellipArc2d, true);  
      }
      else
      {
        if (lastPoint.isEqualTo(firstPointOfLoop))
        {
          m_pCommands->h();
        }
        m_pCommands->S();
        PDFIContentCommands::drawEllipse(*m_pCommands, ellipArc2d, false);

        firstPointOfLoop.set(pItc->startPoint().x, pItc->startPoint().y);
      }
    }
 
    bLastPointInit = true;
    lastPoint.set(pItc->endPoint().x, pItc->endPoint().y);

    ++pItc;
  }

  if (bLastPointInit)
  {
    if (lastPoint.isEqualTo(firstPointOfLoop))
    {
      m_pCommands->h();
    }
    m_pCommands->S();
  }

  for (OdUInt32 i = 0; i < m_InnerContours.size(); i++)
  {
    OdGePoint2dArray pnts2d = m_InnerContours[i];

    m_pCommands->m(pnts2d[0].x, pnts2d[0].y);
    for (OdUInt32 f = 1; f < pnts2d.size(); ++f)
      m_pCommands->l(pnts2d[f].x, pnts2d[f].y);
  }
  for (OdUInt32 i = 0; i < m_OuterContours.size(); i++)
  {
    OdGePoint2dArray pnts2d = m_OuterContours[i];

    m_pCommands->m(pnts2d[0].x, pnts2d[0].y);
    for (OdUInt32 f = 1; f < pnts2d.size(); ++f)
      m_pCommands->l(pnts2d[f].x, pnts2d[f].y);
    m_pCommands->f_odd(); //fill
  }
}
#endif
bool PDFShxGeomStore::getBBox(PDFBBoxStore &pBBox) const
{
  pBBox = m_BBox;
  pBBox.scaleBy(m_dGeomScale);
  return pBBox.isInit();
}
}
