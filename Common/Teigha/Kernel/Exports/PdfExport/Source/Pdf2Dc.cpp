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
// PdfWriter.cpp: implementation of the GsPdfFactory, GsPdfDevice, GsPdfView classes.
//
//////////////////////////////////////////////////////////////////////


#include "PdfExportCommon.h"
#include "OdBinaryData.h"
#include "EllipseArc2Bezier.h"
#include "Pdf2dExportDevice.h"
#include "PdfResourceDictionary.h"
#include "PdfPageNodeDictionary.h"
#include "PdfImage.h"
#include "PdfAux.h"
#include "PdfExportService.h"

#include "PdfOptionalContentGroupDictionary.h"
#include "PdfCatalogDictionary.h"
#include "PdfOptionalContentPropertiesDictionary.h"
#include "PdfOptionalContentConfigurationDictionary.h"
#include "PdfOCManager.h"
#include "PdfXObjectForm.h"

#include "Ge/GeEllipArc2d.h"
#include "Ge/GeEllipArc3d.h"
#include "Ge/GeNurbCurve2d.h"
#include "Ge/GeKnotVector.h"
#include "Ge/GeLineSeg2d.h"

#include "PdfType0Font.h"
#include "PdfCIDFont2.h"
#include "PdfCID2GIDStream.h"

#include "OdTrueTypeFontBase.h"

#include "PdfShadingPattern.h"
#include "PdfShadingT4Dictionary.h"
#include "MemoryStream.h"
#include "Pdf3dAnnotationDictionary.h"
#include "Pdf3dStream.h"
#include "Pdf3dActivationDictionary.h"
#include "Pdf3dViewDictionary.h"
#include "PdfProjectionDictionary.h"
#include "Pdf3dRenderModeDictionary.h"

#include "PdfOptionalContentPropertiesDictionary.h"
#include "PdfSquareAnnotationDictionary.h"
#include "PdfLinkAnnotationDictionary.h"
#include "PdfURIActionDictionary.h"
#include "PdfExtGState.h"

#include "Gi/GiRasterWrappers.h"
#include "RxRasterServices.h"
//====================================================================
//
namespace TD_PDF_2D_EXPORT {

void PDF2dExportDevice::saveGraphState(const PDFDevicePoint &LastPoint) 
{ 
  ODA_ASSERT(!m_bGraphStateFixed);
  m_bGraphStateFixed = true;
  m_num = 0;
  saveLastPoint(LastPoint);
}

void PDF2dExportDevice::saveLastPoint(const PDFDevicePoint &LastPoint)
{
  ODA_ASSERT(m_bGraphStateFixed);
  ODA_ASSERT(!(m_LastPoint.x == LastPoint.x && m_LastPoint.y == LastPoint.y));
  m_bOnePointPoly = false;

  PDFIContentCommands *pOut = cc();

  if (m_num >= 2)
  {
    OdGePoint2d start_point;
    OdGePoint2d middle_point;
    if (m_num < 6)
    {
      start_point.x = m_points[m_num - 2].x;
      start_point.y = m_points[m_num - 2].y;
      middle_point.x = m_points[m_num - 1].x;
      middle_point.y = m_points[m_num - 1].y;
    }
    else
    {
      start_point.x = m_points[m_num - 3].x;
      start_point.y = m_points[m_num - 3].y;
      middle_point.x = m_points[m_num - 2].x;
      middle_point.y = m_points[m_num - 2].y;
    }
    OdGePoint2d last_point(LastPoint.x, LastPoint.y);

    OdGeLineSeg2d res_line(start_point, last_point);
    OdGeLineSeg2d start_line(start_point, middle_point);
    OdGeLineSeg2d mid_line(middle_point, last_point);

    double distance = 1;

    if (res_line.length() &&
        res_line.length() > start_line.length() &&
        res_line.length() > mid_line.length()) //if res_line is lesser than start_line or mid_line the polyline changes direction
    {
      double paramOf = res_line.paramOf(middle_point);
      OdGePoint2d bb = res_line.evalPoint(paramOf);
      distance = bb.distanceTo(middle_point);
    }
    double dev = .5 * 72. / (double)m_pParamsHolder->getParams().getGeomDPI(); /*0.5 pixel scaled to correspond geom dpi*/
    if (OdZero(distance, dev))
      if (m_num < 6)
        m_points[m_num - 1] = LastPoint;
      else
        m_points[m_num - 2] = LastPoint;
    else
    {
      if (m_num < 6)
        m_points[m_num++] = LastPoint;
      if (m_num == 6)
      {
        pOut->m(m_points[0].x, m_points[0].y);
        for (OdUInt32 f = 1; f < m_num - 2; ++f)
          pOut->l(m_points[f].x, m_points[f].y);

        m_num++;
      }
      else if (m_num == 7)
      {
        pOut->l(m_points[m_num-3].x, m_points[m_num - 3].y);

        m_points[m_num - 3] = m_points[m_num - 2];
        m_points[m_num - 2] = LastPoint;
      }
    }
  }
  else
    m_points[m_num++] = LastPoint;

  m_LastPoint.x = LastPoint.x;
  m_LastPoint.y = LastPoint.y;
}

void PDF2dExportDevice::GraphStateChanged()
{
  if (m_bGraphStateFixed)
  {
    PDFIContentCommands *pOut = cc();
    
    double X0 = m_points[0].x;
    double Y0 = m_points[0].y;

    if (m_num == 5 && X0 == m_points[4].x &&
                      Y0 == m_points[4].y)
    {
      double X1 = m_points[1].x
           , Y1 = m_points[1].y
           , X2 = m_points[2].x
           , Y2 = m_points[2].y
           , X3 = m_points[3].x
           , Y3 = m_points[3].y;

      bool bRe = (X0 == X1  && X2 == X3  && Y0 == Y3  && Y2 == Y1) ||
                 (Y0 == Y1  && Y2 == Y3  && X0 == X3  && X2 == X1);

      if (bRe)
      {
        double w = fabs(X0 - X2);
        double h = fabs(Y0 - Y2);

        if (h != 0 && w != 0)
        {
          pOut->re(odmin(X0, X2), odmin(Y0, Y2), w, h);
          pOut->S();

          m_bOnePointPoly = true;
          m_bGraphStateFixed = false;
          m_LastPoint.x = m_LastPoint.y = -HUGE_VAL;
          m_num = 0;

          return;
        }
      }
    }
    
    if (m_bOnePointPoly)
    {
      PDFIContentCommands::drawPoint(*pOut, OdGePoint2d(m_LastPoint.x, m_LastPoint.y));
      pOut->S();
    } 
    else
    {
      ODA_ASSERT(m_num > 1);
      
      if (m_num < 6)
      {
        pOut->m(m_points[0].x, m_points[0].y);
        for (OdUInt32 f = 1; f < m_num - 1; ++f)
          pOut->l(m_points[f].x, m_points[f].y);
      }
      else
      {
        pOut->l(m_points[m_num-3].x, m_points[m_num - 3].y);
      }

      if (m_points[0].x == m_LastPoint.x && m_points[0].y == m_LastPoint.y)
      {
        pOut->s(); // h & S 
      }
      else
      {
        if (m_num < 6)
          pOut->l(m_points[m_num - 1].x, m_points[m_num - 1].y);
        else
          pOut->l(m_points[m_num - 2].x, m_points[m_num - 2].y);
        pOut->S();
      }
    }

    m_bOnePointPoly = true;
    m_bGraphStateFixed = false;
    m_LastPoint.x = m_LastPoint.y = -HUGE_VAL;
    m_num = 0;
  }
}

bool PDF2dExportDevice::needNewPolyline() const
{
  return !m_bGraphStateFixed;
}

bool PDF2dExportDevice::isDublicatedSegment(const PDFDevicePoint &LastPoint) const
{
  ODA_ASSERT(m_bGraphStateFixed);
  return m_LastPoint.x == LastPoint.x && m_LastPoint.y == LastPoint.y;
}

void PDF2dExportDevice::ContinuePolyline(const PDFDevicePoint &LastPoint)
{
  ODA_ASSERT(m_bGraphStateFixed);
  if (!isDublicatedSegment(LastPoint))
  {
    saveLastPoint(LastPoint);
  }
}

void PDF2dExportDevice::StartNewPolyline(const PDFDevicePoint &LastPoint)
{
  GraphStateChanged();
  saveGraphState(LastPoint);
  m_bOnePointPoly = true;
}

// call from beginViewVectorization()
void PDF2dExportDevice::StartOptimizator()
{
  GraphStateChanged();

  m_curRGB = 0xFFFFFFFF;
  m_curColorIndex = 0;
  m_bRGBActive = true; // don't change it to false

  m_lineWeight = -HUGE_VAL;

  m_curLayerName = "";
  m_LayerOpen = false;
  m_stubLayerCreated = false;
}

// call from endViewVectorization()
void PDF2dExportDevice::StopOptimizator()
{
  ClosePolyline();

  if (m_LayerOpen)
  {
    CloseLayerBracket();
    m_curLayerName = "";
  }
}

void PDF2dExportDevice::ClosePolyline()
{
  GraphStateChanged();
}

PDFNamePtr PDF2dExportDevice::CreateOC4Layer(const OdString &layer_name, bool bLocked)
{
  PDFDocument &PDFDoc = document();

  PDFCatalogDictionaryPtr pCatalog = PDFDoc.Root();

  PDFOCPropertiesDictionaryPtr pOCP = pCatalog->Find("OCProperties");
  if (pOCP.isNull())
  {
    pOCP = PDFOCPropertiesDictionary::createObject(PDFDoc, false);
    pCatalog->setOCProperties(pOCP);
  }

  PDFOCConfigurationDictionaryPtr pDefCfg = pOCP->getD(); 

  PDFNamePtr pOCG = PDFOCManager::createOCG(layer_name, pDefCfg, m_pParamsHolder->isLayerOriginallyVisible(layer_name), bLocked, getResourceDictionary());

  return pOCG;
}

void PDF2dExportDevice::OpenLayerBracket(const OdString &layer_name)
{
  ODA_ASSERT(isLayersSupported());

  if (m_curLayerName.isEmpty()) // strange situation
    return;

  PDFIContentCommands *pOut = cc();
  PDFDocument &PDFDoc = document();

  PDFNamePtr pOCG = PDFOCManager::getOCGbyName(layer_name, PDFDoc, getResourceDictionary());
  if (pOCG.isNull())
    pOCG = CreateOC4Layer(layer_name);

  pOut->BDC(PDF_OC_NAME, pOCG);
  m_LayerOpen = true;
}

void PDF2dExportDevice::CloseLayerBracket()
{
  ODA_ASSERT(isLayersSupported());

  PDFIContentCommands *pOut = cc();
  pOut->EMC(); 
  m_LayerOpen = false;
}

// frozen layers are always around BlockRefs and are not the same as general "layer"
void PDF2dExportDevice::open_Frozen_Layer(const OdString &layer_name)
{
  ODA_ASSERT(isLayersSupported());

  // close "general" layer before
  if (m_LayerOpen)
  {
    GraphStateChanged();
    CloseLayerBracket();
    m_curLayerName = "";
  }

  PDFIContentCommands *pOut = cc();
  PDFDocument &PDFDoc = document();

  PDFNamePtr pOCG = PDFOCManager::getOCGbyName(layer_name, PDFDoc, getResourceDictionary());
  if (pOCG.isNull())
    pOCG = CreateOC4Layer(layer_name);

  pOut->BDC(PDF_OC_NAME, pOCG);
}

void PDF2dExportDevice::close_Frozen_Layer()
{
  if (m_LayerOpen)
  {
    GraphStateChanged();
    CloseLayerBracket();
    m_curLayerName = "";
  }

  PDFIContentCommands *pOut = cc();
  pOut->EMC();
}

//====================================================================
//

void PDF2dExportDevice::dc_polyline(OdUInt32 nPts, const OdGePoint2d* pPoints)
{
  PDFIContentCommands *pOut = cc();

  if (isSimpleGeomOptimizationEnabled() && m_stateDrawingContour == 0)
  {
    PDFDevicePoint tmp;
    tmp.x = (int)(pPoints[0].x);
    tmp.y = (int)(pPoints[0].y);

    if (!needNewPolyline() && !isDublicatedSegment(tmp))
    {
      ClosePolyline();
    }

    if (needNewPolyline())
    {
      StartNewPolyline(tmp);
    }
    else
    {
      ContinuePolyline(tmp);
    }

    for(OdUInt32 f = 1; f < nPts; ++f)
    {
      PDFDevicePoint tmp;
      tmp.x = (int)(pPoints[f].x);
      tmp.y = (int)(pPoints[f].y);

      ContinuePolyline(tmp);
    }
  }
  else
  {
    if (nPts == 1 && m_stateDrawingContour == 0)
    {
      PDFIContentCommands::drawPoint(*pOut, pPoints[0]);
    }
    else
    {
      if(m_stateDrawingContour == 2)
      {
        pOut->m((pPoints[0].x), (pPoints[0].y));
        m_stateDrawingContour = 1;
      }
      if(m_stateDrawingContour == 0)
      {
        pOut->m((pPoints[0].x), (pPoints[0].y));
      }
      for(OdUInt32 f = 1; f < nPts; ++f)
      {
        pOut->l(pPoints[f].x, pPoints[f].y);
      }
    }

    if(m_stateDrawingContour == 0)
    {
      pOut->S();
    }
  }
}

//====================================================================
//
void PDF2dExportDevice::dc_polygon(OdUInt32 nPoints, const OdGePoint2d* pPoints)
{
  ClosePolyline();

  OdGePoint2d* pNewPoints = 0;
  OdUInt32 nNewPoints = 0;
  bool bOptimize = isSimpleGeomOptimizationEnabled();

  if (bOptimize)
  {
    pNewPoints = new OdGePoint2d[nPoints];
    for (OdUInt32 j = 0; j < nPoints; j++)
    {
      if (j == 0)
      {
        pNewPoints[j] = OdGePoint2d(pPoints[j].x, pPoints[j].y);
        nNewPoints++;
      }
      else if ((int)pPoints[j].x != (int)pNewPoints[nNewPoints - 1].x || (int)pPoints[j].y != (int)pNewPoints[nNewPoints - 1].y)
      {
        pNewPoints[nNewPoints] = OdGePoint2d(pPoints[j].x, pPoints[j].y);
        nNewPoints++;
      }
    }
  }
  else
  {
    nNewPoints = nPoints;
    //pNewPoints = pPoints;
  }

  PDFIContentCommands *pOut = cc();
  if (nNewPoints == 1)
  {
    set_LineWeight(0);
    pOut->m(pNewPoints[0].x, pNewPoints[0].y);
    pOut->l(pNewPoints[0].x, pNewPoints[0].y);
    pOut->s();
  }
  else if (nNewPoints == 2)
  {
    set_LineWeight(0);
    pOut->m(pNewPoints[0].x, pNewPoints[0].y);
    pOut->l(pNewPoints[1].x, pNewPoints[1].y);
    pOut->s();
  }
  else if (nNewPoints > 2)
  {
    if (GETBIT(m_effTraits.drawFlags(), OdGiSubEntityTraits::kDrawContourFill))
    { // .DGN specific mode
      ODCOLORREF rgb = m_curSecondaryColorRGB;

      if (exportParams().colorPolicy() == PDFExportParams::kGrayscale)
        convertColorToGrayscale(rgb);
      else if (exportParams().colorPolicy() == PDFExportParams::kMono && (ODGETRED(rgb) != 255 || ODGETGREEN(rgb) != 255 || ODGETBLUE(rgb) != 255))
        rgb = ODRGBA(0, 0, 0, ODGETALPHA(rgb));

      double r = double(ODGETRED(rgb)) / 255.;
      double g = double(ODGETGREEN(rgb)) / 255.;
      double b = double(ODGETBLUE(rgb)) / 255.;

      pOut->rg(r, g, b);

      if (!bOptimize)
        pOut->m((pPoints[0].x), (pPoints[0].y));
      else
        pOut->m((pNewPoints[0].x), (pNewPoints[0].y));

      for (OdUInt32 f = 1; f < nNewPoints; ++f)
      {
        if (!bOptimize)
          pOut->l((pPoints[f].x), (pPoints[f].y));
        else
          pOut->l((pNewPoints[f].x), (pNewPoints[f].y));
      }

      pOut->b();

      if (m_bRGBActive)
      {
        pOut->rg((double)ODGETRED(m_curRGB) / 255, (double)ODGETGREEN(m_curRGB) / 255, (double)ODGETBLUE(m_curRGB) / 255);
      }
      else
      {
        pOut->cs(DWG_PALETTE_NAME);
        m_bDwgPaletteNeeded = true;
        pOut->sc((OdUInt8)m_curColorIndex);
      }
    }
    else
    {
      if (m_fillRule == kFillStrokeCloseRule && m_lineWeight)
        set_LineWeight(0);
      if (!bOptimize)
        pOut->m((pPoints[0].x), (pPoints[0].y));
      else
        pOut->m((pNewPoints[0].x), (pNewPoints[0].y));
      for (OdUInt32 f = 1; f < nNewPoints; ++f)
      {
        if (!bOptimize)
          pOut->l((pPoints[f].x), (pPoints[f].y));
        else
          pOut->l((pNewPoints[f].x), (pNewPoints[f].y));
      }

      if (m_fillRule != kNoFill)
      {
        if (m_fillRule == kFillEvenOddRule)
          pOut->f_odd();
        else if (m_fillRule == kFillStrokeCloseRule)
          pOut->b();
        else
          pOut->f();
      }
    }
  }

  if (bOptimize)
    delete[] pNewPoints;
}

//====================================================================
//
void PDF2dExportDevice::dc_fill()
{
  PDFIContentCommands *pOut = cc();
  if (m_fillRule != kNoFill)
  {
    if (m_fillRule == kFillEvenOddRule)
      pOut->f_odd();
    else if (m_fillRule == kFillStrokeCloseRule)
      pOut->b_odd();
    else
      pOut->f();
  }
}

//====================================================================
//

void PDF2dExportDevice::dc_nurb(const OdGeNurbCurve2d& nurb )
{
  //ClosePolyline();

  int degree;
  bool rational, periodic;
  OdGeKnotVector aKt;
  OdGePoint2dArray aCP;
  OdGeDoubleArray aWt;
  nurb.getDefinitionData(degree, rational, periodic, aKt, aCP, aWt);
  ODA_ASSERT(degree == 3);
  ODA_ASSERT(!rational);
  ODA_ASSERT(aCP.size() == 4);

  PDFIContentCommands *pOut = cc();
  
  if (isSimpleGeomOptimizationEnabled())
  {
    OdGeVector2d vec = aCP[3] - aCP[0];

    if (vec.length() < 1.)
    {
      OdGePoint2d points2d[2];
      points2d[0] = aCP[0];
      points2d[1] = aCP[3];
      dc_polyline(2, points2d);
    } 
    else
    {
      OdGeExtents2d extents;
      vec.normalize();

      OdGeMatrix2d matTranslate, matRotate;
      matTranslate.setToTranslation(-aCP[0].asVector());
      OdGePoint2dArray aCP_Tmp = aCP;
      aCP_Tmp[0].transformBy(matTranslate);
      aCP_Tmp[1].transformBy(matTranslate);
      aCP_Tmp[2].transformBy(matTranslate);
      aCP_Tmp[3].transformBy(matTranslate);
      matRotate.setCoordSystem(OdGePoint2d::kOrigin, vec, -vec.perpVector());

      aCP_Tmp[0].transformBy(matRotate);
      aCP_Tmp[1].transformBy(matRotate);
      aCP_Tmp[2].transformBy(matRotate);
      aCP_Tmp[3].transformBy(matRotate);

      if (fabs(aCP_Tmp[1].y) < 1. && fabs(aCP_Tmp[2].y) < 1.) //it looks like a line in pdf
      {
        OdGePoint2d points2d[2];
        points2d[0] = aCP[0];
        points2d[1] = aCP[3];
        dc_polyline(2, points2d);
      }
      else
      {
        ClosePolyline();
        pOut->m(aCP[0], false);
        pOut->c(aCP[1], aCP[2], aCP[3]);
        pOut->S();
      }
    }
  }
  else
  {
    ClosePolyline();
    pOut->m(aCP[0], false);
    pOut->c(aCP[1], aCP[2], aCP[3]);
    pOut->S();
  }
}

void PDF2dExportDevice::dc_circle(const OdGeEllipArc3d& ellipse)
{
  dc_ellipse(ellipse);
}

void PDF2dExportDevice::dc_ellipse(const OdGeEllipArc3d& ellipArc3d)
{
  // got it? probably you are in extents calculation code. You shouldn't be here in extents mode.
  ODA_ASSERT(ellipArc3d.normal().isParallelTo(OdGeVector3d::kZAxis));
  
  OdGeEllipArc2d ellipArc2d(
      ellipArc3d.center().convert2d(),
      ellipArc3d.majorAxis().convert2d(),
      ellipArc3d.minorAxis().convert2d(),
      ellipArc3d.majorRadius(),
      ellipArc3d.minorRadius(),
      ellipArc3d.startAng(),
      ellipArc3d.endAng()
      );

  if (isSimpleGeomOptimizationEnabled()) //try to optimize curve in pdf to reduce file size
  {
    //check if the curve is point
    if (ellipArc2d.length() < 1.)
    {
      OdGePoint2d points2d[2];
      points2d[0] = points2d[1] = ellipArc2d.center();
      dc_polyline(2, points2d);
      return;
    }

    //check if the curve looks like line (or like point again)
    if (!ellipArc2d.startPoint().isEqualTo(ellipArc2d.endPoint())) //the curve is a part of an ellipse
    {
      OdGeExtents2d extents;
      OdGeVector2d x = ellipArc2d.endPoint() - ellipArc2d.startPoint();
      x.normalize();

      OdGeMatrix2d matTranslate, matRotate;
      matTranslate.setToTranslation(-ellipArc2d.startPoint().asVector());
      OdGeEllipArc2d tmpArc = ellipArc2d;
      tmpArc.transformBy(matTranslate);
      matRotate.setCoordSystem(OdGePoint2d::kOrigin, x, -x.perpVector());

      tmpArc.transformBy(matRotate);
      tmpArc.getGeomExtents(extents);

      if (fabs(extents.maxPoint().y - extents.minPoint().y) < 1.) //extents lesser than 1 pixel, so the curve looks like line anyway
      {
        OdGePoint2d points2d[2];
        points2d[0] = extents.minPoint();
        points2d[1] = extents.maxPoint();
        //well the points are two corners of rectangle, but the width is lesser than 1 pixel, so we don't care

        matRotate.invert();
        matTranslate.invert();
        points2d[0].transformBy(matRotate);
        points2d[0].transformBy(matTranslate);
        points2d[1].transformBy(matRotate);
        points2d[1].transformBy(matTranslate);

        dc_polyline(2, points2d);
        return;
      }
    }
    else //the curve is an ellipse or circle
    {
      double minR = ellipArc2d.minorRadius();
      double maxR = ellipArc2d.majorRadius();
      if (minR < .5 || maxR < .5)
      {
        if (minR < .5 && maxR < .5) //looks like point in pdf again
        {
          OdGePoint2d points2d[2];
          points2d[0] = points2d[1] = ellipArc2d.center();
          dc_polyline(2, points2d);
          return;
        }
        else //looks like line, let's find the coordinates
        {
          OdGePoint2d points2d[2];
          points2d[0] = ellipArc2d.center() + maxR*ellipArc2d.majorAxis();
          points2d[1] = ellipArc2d.center() - maxR*ellipArc2d.majorAxis();
          dc_polyline(2, points2d);
          return;
        }
      }
    }
  }

  ClosePolyline();

  PDFIContentCommands *pOut = cc();
  PDFIContentCommands::drawEllipse(*pOut, ellipArc2d, false, deviation(kOdGiMaxDevForCurve, OdGePoint3d(ellipArc2d.center().x, ellipArc2d.center().y, 0.)));
  pOut->S();
}

//====================================================================
//
void PDF2dExportDevice::dc_layer(const OdString &layer_name, bool bClose)
{
  GraphStateChanged();

  if (bClose)
    CloseLayerBracket();

  OpenLayerBracket(layer_name);
}

//====================================================================
//
void PDF2dExportDevice::dc_lineWeight (double lineWeight)
{
  GraphStateChanged();

  PDFIContentCommands *pOut = cc();
  pOut->w(lineWeight);
}

//====================================================================
//
void PDF2dExportDevice::dc_color(ODCOLORREF rgb)
{
  GraphStateChanged();

  if (!m_bRGBActive)
  {
    m_bRGBActive = true;
  }

  PDFIContentCommands *pOut = cc();

  double r = double(ODGETRED(rgb))   / 255.;
  double g = double(ODGETGREEN(rgb)) / 255.;
  double b = double(ODGETBLUE(rgb))  / 255.;
  pOut->RG(r, g, b);
  pOut->rg(r, g, b);
}

void PDF2dExportDevice::dc_color(OdUInt16 index)
{
  GraphStateChanged();

  PDFIContentCommands *pOut = cc();

  if (m_bRGBActive)
  {
    m_bRGBActive = false;
    pOut->cs(DWG_PALETTE_NAME);
    pOut->CS(DWG_PALETTE_NAME);
    m_bDwgPaletteNeeded = true; //DWG indexed palette is used for DGN export in most cases
  }
  pOut->sc(OdUInt8(index));
  pOut->SC(OdUInt8(index));
}

//====================================================================
//

void PDF2dExportDevice::dc_gouraud_polytriangle( const OdGePoint3d* pVertexList,
                                           OdInt32 faceListSize,
                                           const OdInt32* pFaceList,
                                           const OdCmEntityColor* pColors)
{
  ClosePolyline();

  PDFIContentCommands *pOut = cc();

  PDFDocument &PDFDoc = document();

  PDFShadingT4Ptr pType4Shading = PDFShadingT4::createObject( PDFDoc, true );
  
  // fill Shading stream with triangls
  pType4Shading->preFill();

  // calculate coordinates range

  double dMaxx = -HUGE_VAL;
  double dMinx =  HUGE_VAL;
  double dMaxy = -HUGE_VAL;
  double dMiny =  HUGE_VAL;

  OdInt32 f;
  for(f=0; f<faceListSize;)
  {
    OdInt32 num = pFaceList[f];
    if (num == 3)
    {
      OdInt32 a = pFaceList[f + 1];
      OdInt32 b = pFaceList[f + 2];
      OdInt32 c = pFaceList[f + 3];

      dMaxx = odmax(dMaxx, odmax(pVertexList[a].x, odmax(pVertexList[b].x, pVertexList[c].x)));
      dMinx = odmin(dMinx, odmin(pVertexList[a].x, odmin(pVertexList[b].x, pVertexList[c].x)));
      dMaxy = odmax(dMaxy, odmax(pVertexList[a].y, odmax(pVertexList[b].y, pVertexList[c].y)));
      dMiny = odmin(dMiny, odmin(pVertexList[a].y, odmin(pVertexList[b].y, pVertexList[c].y)));
    }
    f += num + 1;
  }

  // fill shading stream
  for(f=0; f<faceListSize;)
  {
    OdInt32 num = pFaceList[f];
    if (num == 3)
    {
      OdInt32 a = pFaceList[f + 1];
      OdInt32 b = pFaceList[f + 2];
      OdInt32 c = pFaceList[f + 3];

      OdUInt8 x1 = OdUInt8((pVertexList[a].x - dMinx) / (dMaxx - dMinx) * OdUInt8(-1));
      OdUInt8 y1 = OdUInt8((pVertexList[a].y - dMiny) / (dMaxy - dMiny) * OdUInt8(-1));
      OdUInt8 x2 = OdUInt8((pVertexList[b].x - dMinx) / (dMaxx - dMinx) * OdUInt8(-1));
      OdUInt8 y2 = OdUInt8((pVertexList[b].y - dMiny) / (dMaxy - dMiny) * OdUInt8(-1));
      OdUInt8 x3 = OdUInt8((pVertexList[c].x - dMinx) / (dMaxx - dMinx) * OdUInt8(-1));
      OdUInt8 y3 = OdUInt8((pVertexList[c].y - dMiny) / (dMaxy - dMiny) * OdUInt8(-1));


      ODCOLORREF rgb1 = ODTOCOLORREF(pColors[a]);
      ODCOLORREF rgb2 = ODTOCOLORREF(pColors[b]);
      ODCOLORREF rgb3 = ODTOCOLORREF(pColors[c]);
      if (exportParams().colorPolicy() == PDFExportParams::kGrayscale)
      {
        convertColorToGrayscale(rgb1);
        convertColorToGrayscale(rgb2);
        convertColorToGrayscale(rgb3);
      }
      else if (exportParams().colorPolicy() == PDFExportParams::kMono)
      {
        if (ODGETRED(rgb1) != 255 || ODGETGREEN(rgb1) != 255 || ODGETBLUE(rgb1) != 255)
          rgb1 = ODRGBA(0, 0, 0, ODGETALPHA(rgb1));
        if (ODGETRED(rgb2) != 255 || ODGETGREEN(rgb2) != 255 || ODGETBLUE(rgb2) != 255)
          rgb2 = ODRGBA(0, 0, 0, ODGETALPHA(rgb2));
        if (ODGETRED(rgb3) != 255 || ODGETGREEN(rgb3) != 255 || ODGETBLUE(rgb3) != 255)
          rgb3 = ODRGBA(0, 0, 0, ODGETALPHA(rgb3));
      }
      OdCmEntityColor color1 = ODTOCMCOLOR(rgb1);
      OdCmEntityColor color2 = ODTOCMCOLOR(rgb2);
      OdCmEntityColor color3 = ODTOCMCOLOR(rgb3);

      pType4Shading->addTriangle( x1, y1, x2, y2, x3, y3,
        color1.red(), color1.green(), color1.blue(),
        color2.red(), color2.green(), color2.blue(),
        color3.red(), color3.green(), color3.blue() );
    }
    f += num + 1;
  }
  
  pType4Shading->postFill(dMinx, dMaxx, dMiny, dMaxy);
  
  //PDFShadingPatternPtr pPattern = PDFShadingPattern::createObject( PDFDoc, true );
  //pPattern->setPatternType(2);
  //pPattern->setShading(pType4Shading);

  PDFResourceDictionaryPtr pResDict = getResourceDictionary();
  if (pResDict.isNull())
  {
    pResDict = PDFPageNodeDictionaryPtr(PDFDoc.Root()->getPages())->getResources();
  }
  PDFShadingSubDictionaryPtr pShadingSub = pResDict->getShading();
  PDFNamePtr pName = pShadingSub->AddUniqueProperty(0, pType4Shading);

  pOut->sh(pName);
}

OdGeMatrix2d PDF2dExportDevice::createImageClipTransform( const OdGiRasterImage* pImg 
    ,const OdGePoint3d& origin
    ,const OdGeVector3d& u
    ,const OdGeVector3d& v
    )
{
  OdUInt32 height(pImg->pixelHeight());

  OdGeVector2d u2( u.x, u.y );
  OdGeVector2d v2( -v.x, -v.y ); // image in svg(& pdf) is upside-down
  OdGePoint2d o2( origin.x, origin.y );
  OdGeMatrix2d m; m.setCoordSystem(  o2 - v2 * height, u2, v2 );
  
  return m;
}


OdGeMatrix2d PDF2dExportDevice::createImageTransform( const OdGiRasterImage* pImg 
    ,const OdGePoint3d& origin
    ,const OdGeVector3d& u
    ,const OdGeVector3d& v
    )
{
  OdUInt32 width(pImg->pixelWidth()), height(pImg->pixelHeight());
  OdGeVector2d u2( u.x * width, u.y * width );
  OdGeVector2d v2( -v.x * height, -v.y * height ); // image in svg(& pdf) is upside-down
  OdGePoint2d o2( origin.x, origin.y );
  OdGeMatrix2d m; m.setCoordSystem( o2 - v2, u2, v2 );
  
  return m;
}

void PDF2dExportDevice::dc_mx()
{
  PDFAUX::updateContentStreamWithCM(document(), m_currentPdfPage, exportParams());
//   OdGeMatrix2d m;
//   m.setToScaling(coeff);
// 
//   PDFIContentCommands *pOut = cc();
//   pOut->cm(m.entry[0][0], m.entry[1][0], m.entry[0][1], m.entry[1][1], m.entry[0][2], m.entry[1][2]);
// //  m_scaleCoeff = coeff;
}

OdGiRasterImagePtr PDF2dExportDevice::cropImage(const OdGiRasterImage* pImg, const OdGeVector2d& u1, const OdGeVector2d& v1, OdGeExtents2d& cropArea,
                                                OdGePoint2d& origin)
{
  OdSmartPtr<OdGiRasterImage> pImgClipped;
  OdGeExtents2d rasterExtents;
  OdGePoint2d newOrigin(origin);

  OdGeVector2d u = u1, v = v1;
  OdGeMatrix2d m;
  if (OdZero(u1.x)) //picture rotated
  {
    if (u1.y > 0)                   //rotation == 90
      m.setToRotation(3 * OdaPI2); //to complete full circle (360)
    else                            //rotation == 270
      m.setToRotation(OdaPI2);     //to complete full circle (360)

    cropArea.transformBy(m);
    u.transformBy(m);
    v.transformBy(m);
    origin.transformBy(m);
    newOrigin.transformBy(m);
    m.invert();
  }

  rasterExtents.addPoint(OdGePoint2d(origin.x, origin.y));
  rasterExtents.addPoint(OdGePoint2d(origin.x + u.x*pImg->pixelWidth(), origin.y));
  rasterExtents.addPoint(OdGePoint2d(origin.x, origin.y + v.y*pImg->pixelHeight()));
  rasterExtents.addPoint(OdGePoint2d(origin.x + u.x*pImg->pixelWidth(), origin.y + v.y*pImg->pixelHeight()));

  if (rasterExtents.minPoint().x < cropArea.minPoint().x || rasterExtents.minPoint().y < cropArea.minPoint().y ||
    rasterExtents.maxPoint().x > cropArea.maxPoint().x || rasterExtents.maxPoint().y > cropArea.maxPoint().y)
  {//crop needed
    double newWidth = fabs(u.x)*pImg->pixelWidth();
    double newHeight = fabs(v.y)*pImg->pixelHeight();
    double cropX = 0;
    double cropY = 0;
    if (rasterExtents.maxPoint().x > cropArea.maxPoint().x)
      newWidth -= rasterExtents.maxPoint().x - cropArea.maxPoint().x;
    if (rasterExtents.minPoint().x < cropArea.minPoint().x)
      newWidth -= cropArea.minPoint().x - rasterExtents.minPoint().x;
    if (rasterExtents.maxPoint().y > cropArea.maxPoint().y)
      newHeight -= rasterExtents.maxPoint().y - cropArea.maxPoint().y;
    if (rasterExtents.minPoint().y < cropArea.minPoint().y)
      newHeight -= cropArea.minPoint().y - rasterExtents.minPoint().y;

    if (newOrigin.x > cropArea.maxPoint().x)
    {
      newOrigin.x = cropArea.maxPoint().x;
      if (u.x < 0)
        cropX += rasterExtents.maxPoint().x - cropArea.maxPoint().x;
    }
    else if (newOrigin.x < cropArea.minPoint().x)
    {
      newOrigin.x = cropArea.minPoint().x;
      if (u.x > 0)
        cropX += cropArea.minPoint().x - rasterExtents.minPoint().x;
    }
    if (newOrigin.y > cropArea.maxPoint().y)
    {
      newOrigin.y = cropArea.maxPoint().y;
      if (v.y < 0)
        cropY += rasterExtents.maxPoint().y - cropArea.maxPoint().y;
    }
    else if (newOrigin.y < cropArea.minPoint().y)
    {
      newOrigin.y = cropArea.minPoint().y;
      if (v.y > 0)
        cropY += cropArea.minPoint().y - rasterExtents.minPoint().y;
    }
    if (newWidth <= 0 || newHeight <= 0)
    {
    //  ODA_ASSERT(0);
      if (OdZero(u1.x))
        origin.transformBy(m);
      return pImg;
    }
    OdUInt32 newPixelWidth = newWidth / fabs(u.x) + 1;
    OdUInt32 newPixelHeight = newHeight / fabs(v.y) + 1;
    cropX /= fabs(u.x);
    cropY /= fabs(v.y);

    if (!OdEqual(cropX, (OdUInt32)cropX)/*it is necessary to crop part of pixel that we can't do*/ && newOrigin.x != origin.x)
    {
      newOrigin.x -= u.x*(cropX - (OdUInt32)cropX);
      newPixelWidth++;
    }

    if (!OdEqual(cropY, (OdUInt32)cropY)/*it is necessary to crop part of pixel that we can't do*/ && newOrigin.y != origin.y)
    {
      newOrigin.y -= v.y*(cropY - (OdUInt32)cropY);
      newPixelHeight++;
    }

    pImgClipped = pImg->crop(cropX, cropY, newPixelWidth, newPixelHeight);
    origin = newOrigin;
  }
  else
    pImgClipped = pImg;

  if (OdZero(u1.x))
  {
    origin.transformBy(m);
    cropArea.transformBy(m);
  }

  return pImgClipped;
}

void PDF2dExportDevice::dc_raster_image(
  const OdGePoint3d& origin,
  const OdGeVector3d& u,
  const OdGeVector3d& v,
  const OdGiRasterImage* pImg, // image object
  const OdGePoint2d* uvBoundary, // may not be null
  OdUInt32 numBoundPts,
  bool transparency,
  double brightness,
  double contrast,
  double fade,
  ODCOLORREF entityColor)
{
  if (!pImg)
  {
    return;
  }

  OdUInt32 pixelWidth = pImg->pixelWidth();
  OdUInt32 pixelHeight = pImg->pixelHeight();

  if (pixelWidth == 0 || pixelHeight == 0)
  {
    return;
  }

  // #13599
  if (u.isZeroLength() || v.isZeroLength())
  {
    return;
  }

  ClosePolyline();

  //Crop Images/////////////////////////////////////////////////////////////
  OdSmartPtr<OdGiRasterImage> pImgClipped;
  OdGePoint2d newOrigin(origin.x, origin.y);

  if ((m_bRasterClipping || exportParams().imageCropping()) &&
    (u.isParallelTo(OdGeVector3d::kXAxis) || u.isParallelTo(OdGeVector3d::kYAxis))) //we cannot crop rotated picture because it wouldn't be the cropping but cutting picture corner
  {
    if (m_clipPoints.size() == 4 && !m_paperClip.isValidExtents())
    {
      int nrcCountsOut = 4;
      //find min & max points of clip rectangle
      OdUInt32 min_x = m_clipPoints[0].x, max_x = m_clipPoints[0].x, min_y = m_clipPoints[0].y, max_y = m_clipPoints[0].y;
      for (int f = 1; f < nrcCountsOut - 1; ++f)
      {
        if (m_clipPoints[f].x < min_x)
          min_x = m_clipPoints[f].x;
        else if (m_clipPoints[f].x > max_x)
          max_x = m_clipPoints[f].x;
        if (m_clipPoints[f].y < min_y)
          min_y = m_clipPoints[f].y;
        else if (m_clipPoints[f].y > max_y)
          max_y = m_clipPoints[f].y;
      }
      m_paperClip.set(OdGePoint2d((double)min_x, (double)min_y), OdGePoint2d((double)max_x, (double)max_y));
    }

    if (m_paperClip.isValidExtents())
    {
      pImgClipped = cropImage(pImg, OdGeVector2d(u.x, u.y), OdGeVector2d(v.x, v.y), m_paperClip, newOrigin);
    }
    else
      pImgClipped = pImg;
  }
  else
    pImgClipped = pImg;

  //OdRxRasterServicesPtr pRasSvcs = odrxDynamicLinker()->loadApp(RX_RASTER_SERVICES_APPNAME);
  //pRasSvcs->saveRasterImage(pImg, "F:\\01.bmp", OdRxRasterServices::kBMP);
  //pRasSvcs->saveRasterImage(pImgClipped, "F:\\02.bmp", OdRxRasterServices::kBMP);
  //////////////////////////////////////////////////////////////////////////

  PDFIContentCommands *pOut = cc();
  PDFDocument &PDFDoc = document();

  PDFResourceDictionaryPtr pResDict = getResourceDictionary();
  if (pResDict.isNull())
  {
    pResDict = PDFPageNodeDictionaryPtr(PDFDoc.Root()->getPages())->getResources();
  }

  PDFImagePtr pImage = TD_PDF_HELPER_FUNCS::addNewImage(pResDict);

  try
  {
    ODCOLORREF bgColor = getPaletteBackground();
    if (GETBIT(m_effTraits.drawFlags(), OdGiSubEntityTraits::kDrawContourFill))
    {
      if (m_effTraits.secondaryTrueColor().isByColor())
        bgColor = ODTOCOLORREF(m_effTraits.secondaryTrueColor());
      else
        bgColor = getColor(m_effTraits.secondaryTrueColor().colorIndex());
    }
    if (exportParams().colorPolicy() == PDFExportParams::kGrayscale)
    {
      convertColorToGrayscale(entityColor);
      convertColorToGrayscale(bgColor);
    }
    else if (exportParams().colorPolicy() == PDFExportParams::kMono)
    {
      double r0 = double(ODGETRED(entityColor));
      double g0 = double(ODGETGREEN(entityColor));
      double b0 = double(ODGETBLUE(entityColor));

      double r1 = double(ODGETRED(bgColor));
      double g1 = double(ODGETGREEN(bgColor));
      double b1 = double(ODGETBLUE(bgColor));
      if (30 * r0 + 59 * g0 + 11 * b0 > 30 * r1 + 59 * g1 + 11 * b1)
      {
        bgColor = ODRGBA(0, 0, 0, ODGETALPHA(bgColor));
        entityColor = ODRGBA(255, 255, 255, ODGETALPHA(entityColor));
      }
      else
      {
        entityColor = ODRGBA(0, 0, 0, ODGETALPHA(entityColor));
        bgColor = ODRGBA(255, 255, 255, ODGETALPHA(bgColor));
      }
    }

    OdSmartPtr<OdGiRasterImage> pConverted;
    if (exportParams().colorPolicy() == PDFExportParams::kGrayscale || exportParams().colorPolicy() == PDFExportParams::kMono)
    {
      if (exportParams().colorPolicy() == PDFExportParams::kGrayscale)
        pConverted = OdRxObjectImpl<OdGiGrayscaleRasterTransformer>::createObject();
      else if (exportParams().colorPolicy() == PDFExportParams::kMono)
      {
        pConverted = OdRxObjectImpl<OdGiMonochromaticRasterTransformer>::createObject();
        if (m_bHatchDrawingMode)
        {
          OdSmartPtr<OdGiMonochromaticRasterTransformer> pMon = OdGiMonochromaticRasterTransformer::cast(pConverted);
          if (!pMon.isNull())
            pMon->setThreshold(254);
        }
      }
      OdSmartPtr<OdGiRasterImageWrapper> pWrapper = OdGiRasterImageWrapper::cast(pConverted);
      if (!pWrapper.isNull())
        pWrapper->setOriginal(pImgClipped);
      else
        pConverted = pImgClipped;
    }
    else
      pConverted = pImgClipped;

    TD_PDF_HELPER_FUNCS::dcImageToPdfImage(pConverted, pImage, transparency, brightness, contrast, fade, entityColor, PDFDoc, bgColor, exportParams().dctQuality(), pResDict);

    pOut->q(); // push graph state to stack
    if (numBoundPts)
    {
      pOut->m(uvBoundary[0].x, uvBoundary[0].y);
      for (OdUInt32 f = 1; f < numBoundPts; ++f)
        pOut->l(uvBoundary[f].x, uvBoundary[f].y);
      pOut->W(); // set current clip path to uvBoundary
      pOut->n(); // apply it (draw without filling)
    }

    OdGeMatrix2d m = createImageTransform(pConverted, OdGePoint3d(newOrigin.x, newOrigin.y, 0.), u, v);
    pOut->cm(OdRoundToLong(m.entry[0][0]), OdRoundToLong(m.entry[1][0]), OdRoundToLong(m.entry[0][1]), OdRoundToLong(m.entry[1][1]), OdRoundToLong(m.entry[0][2]), OdRoundToLong(m.entry[1][2]));
    pOut->Do(pImage->dictionary()->getName());

    pOut->Q(); // restore prev graph state from stack
  }
  catch (const OdError&)
  {
    // skip unsupported images
  }
}

////////////////////
// bBitonalTransp - true if transparency is enabled for bitonal images
// in this case rendering may be optimized
////////////////////
void PDF2dExportDevice::dc_raster_image(OdGiRasterImagePtr pRaster, 
                                        const OdGeExtents2d& imgExts)
{
  ///SUP-716 Accuracy exporting DXF to PDF//////////////////////////////////
  ///We have to put cm matrix into content stream if view is exported as bitmap because of scale coefficient (dpi)
  dc_mx();
  //////////////////////////////////////////////////////////////////////////
  // used only for rendering of shaded viewports
  OdUInt32 pxWidth = pRaster->pixelWidth(), pxHeight = pRaster->pixelHeight();
  OdGePoint2d origin(imgExts.minPoint());
  dc_raster_image(OdGePoint3d(origin.x, origin.y, 0.0), OdGeVector3d::kXAxis * (imgExts.maxPoint().x-origin.x)/pxWidth, 
    OdGeVector3d::kYAxis * (imgExts.maxPoint().y-origin.y)/pxHeight, pRaster, 0, 0, false, 50.0, 50.0, 0.0, 0);
}

/*
OdAnsiString PDF2dExportDevice::dc_shx_font(const OdGiTextStyle& pTextStyle, PDFFontPtr *pOutFont)
{
  PDFDocument &PDFDoc = document();

  OdAnsiString fontName = PDFAUX::addType3Font(PDFDoc, pTextStyle, pOutFont);

  ODA_ASSERT(!fontName.isEmpty());
    
  return fontName;
}
*/

OdAnsiString PDF2dExportDevice::dc_truetype_font(const OdGiTextStyle& pTextStyle, PDFFontPtr *pOutFont)
{
  PDFDocument &PDFDoc = document();

  bool bEmbededFont = AllowEmbededFonts();
  bool bOptimizedEmbeddedFont = AllowEmbededOptimizedFonts();

  PDFResourceDictionaryPtr pResDict = getResourceDictionary();
  if (pResDict.isNull())
  {
    pResDict = PDFPageNodeDictionaryPtr(PDFDoc.Root()->getPages())->getResources();
  }

  bool bAllowedEdit = true;
  OdAnsiString fontName = TD_PDF_HELPER_FUNCS::addType0Font(PDFDoc, pTextStyle, bEmbededFont, bOptimizedEmbeddedFont, pOutFont, pResDict, bAllowedEdit);
  if (fontName.isEmpty())
  {
    fontName = TD_PDF_HELPER_FUNCS::addTrueTypeFont(PDFDoc, pTextStyle, bEmbededFont, bOptimizedEmbeddedFont, pOutFont, pResDict, bAllowedEdit);
  }

  ODA_ASSERT(!fontName.isEmpty());
    
  return fontName;
}

static void getTextAsDByte(OdBaseTextIteratorPtr iter, OdCharArray& retArray)
{
  OdChar ch = iter->nextChar();
  while (ch)
  {
    retArray.push_back(ch);
    ch = iter->nextChar();
  }
}

void PDF2dExportDevice::dc_drawunicodetext(const OdAnsiString &fontName, const OdGePoint2d& pos, const OdGeVector2d& u, const OdGeVector2d& v, OdArray<OdUInt16>* pUnicode, const OdGiTextStyle& pTextStyle, bool raw)
{
  if (!pUnicode->size())
    return;

  ClosePolyline();

  OdString unicodeStr;

  bool bProcessed = false;

  OdFont* pFont = pTextStyle.getFont();
  if ( !pFont->isShxFont() )
  {
      OdTrueTypeFontBase* pTT = static_cast<OdTrueTypeFontBase*>(pFont);
      if ( pTT->isSupportReorder() ) 
      {
          OdDbBaseDatabase* pDb = drawContext()->giContext().database();
          OdString wString(OdUS2W(pUnicode->asArrayPtr())); // not null-terminated
          OdBaseTextIteratorPtr iter = OdDbBaseDatabasePEPtr(pDb)->createTextIterator(pDb, wString.c_str(), pUnicode->size(), raw, &pTextStyle);

          OdCharArray srcArray, dstArray;
          getTextAsDByte(iter, srcArray);
          pTT->getGlyhpIndexes(srcArray, dstArray);

          PDFFontPtr pOutFont;
          dc_truetype_font(pTextStyle, &pOutFont);

          PDFType0FontPtr pNewT0TTF = pOutFont;
          PDFArrayPtr pAr = pNewT0TTF->getDescendantFonts();
    
          PDFCIDFont2Ptr pCIDFont = pAr->first();

          PDFCID2GIDStreamPtr pCID2GID = pCIDFont->getCIDToGIDMap();

          OdUInt16Array data;
          pCID2GID->getData(data);

          for(OdUInt32 k = 0; k < dstArray.size(); k++ )
          {
            OdUInt16 tmp = OdUInt16((((dstArray[k]) & 0xff00) >> 8) + (((dstArray[k]) & 0x00ff) << 8));

            bool bFound = false;
            for(OdUInt32 l = 0; l < data.size(); l++ )
            {
              if (tmp == data[l])
              {
                bFound = true;
                unicodeStr += OdUInt16(((l & 0xff00) >> 8) + ((l & 0x00ff) << 8));
                (*pUnicode)[k] = l;
                break;
              }
            }
            ODA_VERIFY(bFound);
          }
          bProcessed = true;
      }
  }
  
  if (!bProcessed)
  {
      for(OdUInt32 f=0; f<pUnicode->size(); ++f)
      {
          unicodeStr += OdUInt16((((*pUnicode)[f] & 0xff00) >> 8) + (((*pUnicode)[f] & 0x00ff) << 8));
      }
  }

  ODA_VERIFY(pTextStyle.xScale() == 1.); // is it always = 1 ?

  double dHeight = TD_PDF_HELPER_FUNCS::textHeight(pTextStyle);

  OdGeMatrix2d m;
  m.setCoordSystem(pos, u, v);

  PDFDocument &PDFDoc = document();
  PDFTextStringPtr pText = PDFTextString::createObject(PDFDoc);
  pText->set(unicodeStr);
  pText->setExportUnicodeMarker(false);
  pText->enableFixParenthesis(); // fix '(' ')' '\' -> '\(' '\)' '\\'

  PDFIContentCommands *pOut = cc();
  pOut->BT();
    pOut->Tm(m.entry[0][0], m.entry[1][0], m.entry[0][1], m.entry[1][1], m.entry[0][2], m.entry[1][2]);
    if (!OdEqual(pTextStyle.trackingPercent(), 1.))
      pOut->Tc((pTextStyle.trackingPercent() - 1.) * pTextStyle.getFont()->getAverageWidth() * .001);
    pOut->Tf(PDFName::createObject(PDFDoc, fontName.c_str()), dHeight);
    pOut->Tj(pText);
    if (!OdEqual(pTextStyle.trackingPercent(), 1.))
      pOut->Tc(0);
  pOut->ET();
}

void PDF2dExportDevice::dc_drawtext(const OdAnsiString &fontName, const OdGePoint2d& pos, const OdGeVector2d& u, const OdGeVector2d& v, const char& msg, OdInt32 length, const OdGiTextStyle& pTextStyle, const OdDoubleArray& charSpaces)
{
  ClosePolyline();

  ODA_VERIFY(pTextStyle.xScale() == 1.); // is it always = 1 ?
  PDFDocument &PDFDoc = document();

  double dHeight = TD_PDF_HELPER_FUNCS::textHeight(pTextStyle);

  double fontScale = 1.;
  PDFType3FontPtr pFont = TD_PDF_HELPER_FUNCS::getType3FontByName(PDFDoc, fontName);
  if (!pFont.isNull())
  {
    fontScale *= pFont->getScale();
  }

  OdGeMatrix2d m;
  m.setCoordSystem(pos, u*fontScale, v*fontScale);

  PDFTextStringPtr pText = PDFTextString::createObject(PDFDoc); // dont fix here '(' ')' '\' -> '\(' '\)' '\\'

  OdAnsiString str(&msg, length);
  pText->set(str);

  PDFIContentCommands *pOut = cc();
  pOut->BT();

  pOut->Tm(m.entry[0][0], m.entry[1][0], m.entry[0][1], m.entry[1][1], (int)m.entry[0][2], (int)m.entry[1][2]);
  pOut->Tf(PDFName::createObject(PDFDoc, fontName.c_str()), dHeight);

  if (!OdEqual(pTextStyle.trackingPercent(), 1.) && !charSpaces.empty() && length%4 == 0)
  {
    PDFArrayPtr pTextArray = PDFArray::createObject(document());
    const char* cPos = &msg;
    int iPos = 0;
    while (iPos < length)
    {
      PDFTextStringPtr pSymbol = PDFTextString::createObject(PDFDoc); // dont fix here '(' ')' '\' -> '\(' '\)' '\\'
      OdAnsiString str(cPos, 4);
      pSymbol->set(str);
      pTextArray->push_back(pSymbol);
      pTextArray->push_int(charSpaces.at(iPos / 4) / fontScale);

      iPos += 4;
      cPos += 4;
    }
    pOut->TJ(pTextArray);
  }
  else
    pOut->Tj(pText);
  pOut->ET();
}

void PDF2dExportDevice::dc_pushClip(int nrcContours, const int* nrcCounts, const OdGsDCPointArray &nrcPoints)
{
  PDFIContentCommands *pOut = cc();

  pOut->q(); // push graph state to stack

  int nCurPnt = 0;
  for(int f = 0; f < nrcContours; ++f)
  {
    pOut->m(nrcPoints[nCurPnt].x, nrcPoints[nCurPnt].y); ++nCurPnt;
    for(int j = 1; j < nrcCounts[f]; ++j, ++nCurPnt)
    {
      pOut->l(nrcPoints[nCurPnt].x, nrcPoints[nCurPnt].y);
    }
    pOut->h(); // close current subPath
  }
  pOut->W(); // enable clip mode
  pOut->n(); // apply it (draw without filling)
}

void PDF2dExportDevice::dc_popClip()
{
  PDFIContentCommands *pOut = cc();
  pOut->Q(); // restore prev graph state from stack

  // restart color cache system
  m_bRGBActive = true;
}

void PDF2dExportDevice::setCapJoin(PDFLineCap linecap, PDFLineJoin linejoin)
{
  PDFIContentCommands *pOut = cc();

  if (m_CapStyle != linecap)
  {
    m_CapStyle = linecap;
    pOut->J(linecap);
  }
  if (m_JoinStyle != linejoin)
  {
    m_JoinStyle = linejoin;
    pOut->j(linejoin);
  }

//  pOut->J(linecap);
//  pOut->j(linejoin);
}

// exports pDrawable into context and/or context into pStream (depending on params)
OdResult PDF2dExportDevice::dc_prc(OdStreamBuf *pStream, const OdGiDrawable* pDrawable, const PDF2PRCExportParams &params)
{
  // get service
  OdRxClassPtr pService = getPdfExportService();
  OdRxObjectPtr pObj = pService->create();
  PdfExportServiceInterface *pExportIFace = dynamic_cast<PdfExportServiceInterface*>(pObj.get());

  // call it passing context, pDrawable, pStream and params
  return pExportIFace->serialize(pStream, m_prc_context[params.m_viewportIdx].first, pDrawable, &params, exportParams().database());
}

// writes prc stream into pdf page
OdResult PDF2dExportDevice::dc_prc(OdStreamBuf *pStream, const OdGeExtents2d &extents2d, const OdGeVector3d &center, const OdGeVector3d &x, const OdGeVector3d &y, const OdGeVector3d &z, double distanceToTarget, double scale, OdGeMatrix3d *pmatr)
{
  ODA_ASSERT(pStream);
  if (!pStream)
  {
    return ::eInvalidInput;
  }

  if (pStream->length() > 0)
  {
    PDFPageDictionary *pPdfPage = currentPdfPage();
    ODA_ASSERT(pPdfPage);
    if (pPdfPage)
    {
      PDFArrayPtr annots = pPdfPage->getAnnots();
      PDF3dAnnotationDictionaryPtr pDict = PDF3dAnnotationDictionary::createObject(document(), true);

      PDFRectanglePtr rect = PDFRectangle::createObject(document());
      rect->set(extents2d.minPoint().x,extents2d.minPoint().y,extents2d.maxPoint().x,extents2d.maxPoint().y);
      pDict->setRect(rect);
      pDict->setP(pPdfPage);
      pDict->setF(PDFInteger::createObject(document(), OdInt32(68)));
      pDict->setContents(PDFTextString::createObject(document(), "3D Model"));

      PDF3dActivationDictionaryPtr p3DA = PDF3dActivationDictionary::createObject(document());
      p3DA->setTransparent(PDFBoolean::createObject(document(), true, false));
      p3DA->setA(PDFName::createObject(document(), "PO"));
      p3DA->setD(PDFName::createObject(document(), "XD"));
      pDict->set3DA(p3DA);
      PDF3dStreamPtr p3dStream = PDF3dStream::createObject(document(), true);

      PDF3dViewDictionaryPtr pView = PDF3dViewDictionary::createObject(document());
      pView->setXN(PDFTextString::createObject(document(), OdAnsiString("Default View")));
      pView->setMS(PDFName::createObject(document(), "M"));
      pView->setCO(PDFNumber::createObject(document(), distanceToTarget));
      PDFArrayPtr pMatrix = PDFArray::createObject(document());

      OdGeMatrix3d mx;
      mx.setCoordSystem(OdGePoint3d::kOrigin, x, y, z);
      if (pmatr)
      {
        mx = (*pmatr) * mx;
      }

      OdGeMatrix3d cameraTranslation;
      cameraTranslation.setToTranslation(center);
      mx = cameraTranslation * mx;

      ODA_ASSERT(mx[3][0] == 0.);
      ODA_ASSERT(mx[3][1] == 0.);
      ODA_ASSERT(mx[3][2] == 0.);
      ODA_ASSERT(mx[3][3] == 1.);

      pMatrix->push_number(mx[0][0]);
      pMatrix->push_number(mx[1][0]);
      pMatrix->push_number(mx[2][0]);
      pMatrix->push_number(mx[0][1]);
      pMatrix->push_number(mx[1][1]);
      pMatrix->push_number(mx[2][1]);
      pMatrix->push_number(mx[0][2]);
      pMatrix->push_number(mx[1][2]);
      pMatrix->push_number(mx[2][2]);
      pMatrix->push_number(mx[0][3]);
      pMatrix->push_number(mx[1][3]);
      pMatrix->push_number(mx[2][3]);
      pView->setC2W(pMatrix);
      PDFProjectionDictionaryPtr pProj = PDFProjectionDictionary::createObject(document());
      pProj->setSubtype(PDFName::createObject(document(), "O"));
      pProj->setOB(PDFName::createObject(document(), "Max"));
      pProj->setOS(PDFNumber::createObject(document(), scale));
      pView->setP(pProj);

      // set render mode
      PDF3D_ENUMS::PRCRenderingMode rendMode = exportParams().getPrcRenderingMode();
      if (rendMode != PDF3D_ENUMS::kDefaultMode)
      {
        PDF3dRenderModeDictionaryPtr pRM = PDF3dRenderModeDictionary::createObject(document());
        pRM->setSubtype((TD_PDF::RenderingMode)rendMode);
        pView->setRM(pRM);
      }

      pDict->set3DV(pView);

      PDFArrayPtr pAr = PDFArray::createObject(document());
      pAr->push_back(pView);
      p3dStream->getDictionary()->setVA(pAr);

      pDict->set3DD(p3dStream);
      annots->push_back(pDict);

      // TODO : prevent duplication
      {
        OdUInt8Array tmp;
        tmp.resize(pStream->length(), 0);
        pStream->rewind();
        pStream->getBytes(tmp.asArrayPtr(), pStream->length());
        p3dStream->putData((char *)tmp.asArrayPtr(), pStream->length());
      }
    }
  }

  return ::eOk;
}

static OdString encodeURL(const OdString& url)
{
  OdString in = url, tmpStr, out;
  OdInt32 urlLen = in.getLength();
  for (int i = 0; i < urlLen; ++i)
  {
    OdChar xCharUnicode = in.getAt(i);
    OdAnsiCharArray tmpBuf;
    OdCharMapper::unicodeToUtf8(&xCharUnicode, 1, tmpBuf);
    tmpStr = tmpBuf.asArrayPtr();
    OdUInt32 tmpStrLen = tmpStr.getLength();
    if (tmpStrLen == 1) //ASCII-symbol
      out += xCharUnicode;
    else if (tmpStrLen <= 4) //non ASCII-symbol
    {
      char charBuf[100];
      memset(charBuf, 0, 100);
      if (tmpStr.getLength() == 2)
        odSprintfA(charBuf, 100, "%%%X%%%X", (OdUInt8)tmpBuf.getAt(0), (OdUInt8)tmpBuf.getAt(1));
      else if (tmpStr.getLength() == 3)
        odSprintfA(charBuf, 100, "%%%X%%%X%%%X", (OdUInt8)tmpBuf.getAt(0), (OdUInt8)tmpBuf.getAt(1), (OdUInt8)tmpBuf.getAt(2));
      else if (tmpStr.getLength() == 4)
        odSprintfA(charBuf, 100, "%%%X%%%X%%%X%%%X", (OdUInt8)tmpBuf.getAt(0), (OdUInt8)tmpBuf.getAt(1), (OdUInt8)tmpBuf.getAt(2), (OdUInt8)tmpBuf.getAt(3));
      out += charBuf;
    }
    else
    {
      ODA_ASSERT(tmpStrLen <= 4);
      return OdString();
    }
  }
  return out;
}

void PDF2dExportDevice::dc_transparency(double alphaPercent, OdUInt8 alpha, bool mergeLines)
{
  PDFResourceDictionaryPtr pResDict(PDFPageNodeDictionaryPtr((this->document()).Root()->getPages())->getResources());

  PDFResourceDictionaryPtr pResDictPage = getResourceDictionary();

  OdAnsiString lwstr;
  odDToStr(lwstr.getBufferSetLength(512), alpha, 'f', 0);
  lwstr.releaseBuffer();
  OdAnsiString name("GST" + lwstr);
  if (alpha != 255 || m_bTransparencyGroup255 || mergeLines)
  {
    PDFExtGStateSubDictionaryPtr pExtGsSub = pResDict->getExtGState();
    if (!pExtGsSub->HasItem(name))
    {
      PdfExtGStatePtr pExtGState = PdfExtGState::createObject(document(), true);
      if (alpha == 255)
        pExtGState->setBM(PDFName::createObject(document(), mergeLines ? "Darken" : "Normal"));
      else
        pExtGState->setBM(PDFName::createObject(document(), "Normal"));
      pExtGState->setType(PDFName::createObject(document(), "ExtGState"));
      PDFNumberPtr pCa = PDFNumber::createObject(document(), alphaPercent, true);
      pExtGState->setCA(pCa);
      pExtGState->setca(pCa);
      pExtGsSub->AddItem(name, pExtGState);
      if (!pResDictPage.isNull())
        pResDictPage->setExtGState(pExtGsSub);
      m_bTransparencyGroup255 = true;
    }
  }
  if (m_currentTransparencyState.isEmpty())
  {
    m_currentTransparencyState = name;
    if (name != OdAnsiString("GST255")) //guess it can't be, but...
    {
      GraphStateChanged();
      PDFIContentCommands *pOut = cc();
      pOut->gs(PDFName::createObject(document(), name.c_str()));
    }
  }
  else if (m_currentTransparencyState != name)
  {
    m_currentTransparencyState = name;
    GraphStateChanged();
    PDFIContentCommands *pOut = cc();
    pOut->gs(PDFName::createObject(document(), name.c_str()));
  }
}

void PDF2dExportDevice::dc_mergeLines()
{
  PDFResourceDictionaryPtr pResDict(PDFPageNodeDictionaryPtr((this->document()).Root()->getPages())->getResources());

  PDFResourceDictionaryPtr pResDictPage = getResourceDictionary();

  const OdAnsiString name("GM");
  PDFExtGStateSubDictionaryPtr pExtGsSub = pResDict->getExtGState();
  if (!pExtGsSub->HasItem(name))
  {
    PdfExtGStatePtr pExtGState = PdfExtGState::createObject(document(), true);
    pExtGState->setType(PDFName::createObject(document(), "ExtGState"));
    pExtGState->setBM(PDFName::createObject(document(), "Darken"));
    pExtGsSub->AddItem(name, pExtGState);
    if (!pResDictPage.isNull())
      pResDictPage->setExtGState(pExtGsSub);
    PDFIContentCommands *pOut = cc();
    pOut->gs(PDFName::createObject(document(), name.c_str()));
  }
}

bool PDF2dExportDevice::applyLayerToAnnotation(const OdString& layer, PDFAnnotationDictionaryPtr pADict)
{
  if (layer.isEmpty() || pADict.isNull())
    return false;
  if (!m_stubLayerCreated)
  {
    PDFPageDictionary *pPdfPage = currentPdfPage();
    if (!pPdfPage)
      return false;
    PDFArrayPtr annots = pPdfPage->getAnnots();
    OdString uniqueName = odrxSystemServices()->createGuid();
    if (uniqueName.isEmpty())
      return false;
    PDFSquareAnnotationDictionaryPtr pDict = PDFSquareAnnotationDictionary::createObject(document(), true);
    if (!pADict->HasItem("Rect"))
      return false;
    pDict->setRect(pADict->getRect());

    PDFArrayPtr pBorder = PDFArray::createObject(document());
    pBorder->push_int(0);
    pBorder->push_int(0);
    pBorder->push_int(0);
    pDict->setBorder(pBorder);

    if (m_pParamsHolder->getParams().archived() != PDFExportParams::kPDFA_None)
    {
      PDFIntegerPtr pInt = PDFInteger::createObject(document(), 4, false);
      pDict->setF(pInt);

      PDFSubDictionaryPtr pAp = PDFSubDictionary::createObject(document());
      PDFXObjectFormPtr pN = PDFXObjectForm::createObject(document(), true);

      PDFRectanglePtr rect = PDFRectangle::createObject(document());
      rect->set(0, 0, 0, 0);
      pN->dictionary()->setBBox(rect);
      pAp->AddItem("N", pN);
      pDict->setAP(pAp);
    }

    PDFNamePtr pOCG = CreateOC4Layer(uniqueName, true);
    PDFPageNodeDictionaryPtr node = PDFOCManager::getOCGNodeByName(uniqueName, document());
    if (node.isNull())
      return false;
    pDict->setOC(node);

    annots->push_back(pDict);
    m_stubLayerCreated = true;
  }
  PDFPageNodeDictionaryPtr node = PDFOCManager::getOCGNodeByName(layer, document());
  if (node.isNull())
  {
    PDFNamePtr pOCG = CreateOC4Layer(layer);
    node = PDFOCManager::getOCGNodeByName(layer, document());
  }
  if (node.isNull())
    return false;
  pADict->setOC(node);

  return true;
}

void PDF2dExportDevice::dc_hyperlink(const OdGeExtents2d &extents2d, const OdString &link, const OdString &layer)
{
  if (!extents2d.maxPoint().isEqualTo(extents2d.minPoint()))
  {
    PDFPageDictionary *pPdfPage = currentPdfPage();
    ODA_ASSERT(pPdfPage);
    if (pPdfPage)
    {
      OdString url = encodeURL(link);
      if (url.isEmpty())
        return;

      PDFArrayPtr annots = pPdfPage->getAnnots();
      PDFLinkAnnotationDictionaryPtr pDict = PDFLinkAnnotationDictionary::createObject(document(), true);

      PDFRectanglePtr rect = PDFRectangle::createObject(document());
      rect->set(extents2d);
      pDict->setRect(rect);

      PDFArrayPtr pBorder = PDFArray::createObject(document());
      pBorder->push_int(0);
      pBorder->push_int(0);
      pBorder->push_int(0);
      pDict->setBorder(pBorder);

      pDict->setH(PDFName::createObject(document(), "I"));

      if (m_pParamsHolder->getParams().archived() != PDFExportParams::kPDFA_None)
      {
        PDFIntegerPtr pInt = PDFInteger::createObject(document(), 4, false);
        pDict->setF(pInt);
      }

      PDFURIActionDictionaryPtr pAction = PDFActionDictionary::createObject(document());
      pAction->setS(PDFName::createObject(document(), "URI"));
      PDFTextStringPtr sURI = PDFTextString::createObject(document(), OdAnsiString(url));
      sURI->enableFixParenthesis(true);
      pAction->setURI(sURI);

      pDict->setA(pAction);

      if (!layer.isEmpty())
      {
        if (!applyLayerToAnnotation(layer, pDict))
          return;
      }
      annots->push_back(pDict);
    }
  }
}

void PDF2dExportDevice::dc_textAnnot(const OdGeExtents2d &extents2d, const OdString &title, const OdString &content, const OdString &layer)
{
  if (!extents2d.maxPoint().isEqualTo(extents2d.minPoint()))
  {
    PDFPageDictionary *pPdfPage = currentPdfPage();
    ODA_ASSERT(pPdfPage);
    if (pPdfPage)
    {
      PDFArrayPtr annots = pPdfPage->getAnnots();
      PDFSquareAnnotationDictionaryPtr pDict = PDFSquareAnnotationDictionary::createObject(document(), true);

      PDFRectanglePtr rect = PDFRectangle::createObject(document());
      rect->set(extents2d);
      pDict->setRect(rect);

      PDFArrayPtr pBorder = PDFArray::createObject(document());
      pBorder->push_int(0);
      pBorder->push_int(0);
      pBorder->push_int(0);
      pDict->setBorder(pBorder);

      PDFTextStringPtr annotContent = PDFTextString::createObject(document(), false);
      TD_PDF_HELPER_FUNCS::getUnicodeTextString(content, annotContent);
      annotContent->enableFixParenthesis(true);
      pDict->setContents(annotContent);
      pDict->setT(PDFTextString::createObject(document(), OdAnsiString(title)));

      if (m_pParamsHolder->getParams().archived() != PDFExportParams::kPDFA_None)
      {
        PDFIntegerPtr pInt = PDFInteger::createObject(document(), 4, false);
        pDict->setF(pInt);

        PDFSubDictionaryPtr pAp = PDFSubDictionary::createObject(document());
        PDFXObjectFormPtr pN = PDFXObjectForm::createObject(document(), true);

        PDFRectanglePtr rect = PDFRectangle::createObject(document());
        rect->set(0, 0, (OdUInt32)extents2d.maxPoint().x - (OdUInt32)extents2d.minPoint().x, (OdUInt32)extents2d.maxPoint().y - (OdUInt32)extents2d.minPoint().y);
        pN->dictionary()->setBBox(rect);
        pAp->AddItem("N", pN);
        pDict->setAP(pAp);
      }

      if (!layer.isEmpty())
      {
        if (!applyLayerToAnnotation(layer, pDict))
          return;
      }
      annots->push_back(pDict);
    }
  }
}

}
