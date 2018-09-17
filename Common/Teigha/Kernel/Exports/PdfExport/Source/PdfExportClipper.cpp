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
#include "RxObjectImpl.h"
#include "PdfExportClipper.h"

// PdfExportClipper

PdfExportClipper::PdfExportClipper()
{
  m_pClipper = OdGiOrthoClipperEx::createObject();
  m_pClipper->disable();
}

OdSmartPtr<PdfExportClipper> PdfExportClipper::createObject()
{
  return OdRxObjectImpl<PdfExportClipper>::createObject();
}

bool PdfExportClipper::push(const OdGsDCPoint* points, const int *pointCounts, int numPolygons)
{
  OdGePoint2dArray pts;
  int numPoints = 0;
  for (int nPoly = 0; nPoly < numPolygons; nPoly++)
    numPoints += pointCounts[nPoly];
  pts.resize((OdUInt32)numPoints);
  for (int i = 0; i < numPoints; i++)
    pts[i].set((double)points[i].x, (double)points[i].y);
  m_pClipper->pushClipStage((OdUInt32)numPolygons, pointCounts, (OdUInt32)numPoints, pts.getPtr(),
                            OdGiOrthoClipperEx::kCSNoFlags, OdGiOrthoClipperEx::kCCClassifiedByInclusion);
  m_pClipper->enable();
  return true;
}

bool PdfExportClipper::push(const OdGePoint2d& min, const OdGePoint2d& max)
{
  const OdGePoint2d pts[4] = { min,
                               OdGePoint2d((double)max.x, (double)min.y),
                               max,
                               OdGePoint2d((double)min.x, (double)max.y) };
  const int ptsCounts = 4;
  m_pClipper->pushClipStage(1, &ptsCounts, 4, pts);
  m_pClipper->enable();
  return true;
}

bool PdfExportClipper::pop()
{
  return m_pClipper->popClipStage();
}

void PdfExportClipper::setDrawContext(OdGiConveyorContext* pDrawContext)
{
  m_pClipper->setDrawContext(pDrawContext);
}

void PdfExportClipper::setDeviation(const OdGeDoubleArray& deviations)
{
  m_pClipper->setDeviation(deviations);
}

void PdfExportClipper::setDeviation(const OdGiDeviation* pDeviation)
{
  m_pClipper->setDeviation(pDeviation);
}

OdGiConveyorInput& PdfExportClipper::input()
{
  return m_pClipper->input();
}

OdGiConveyorOutput& PdfExportClipper::output()
{
  return m_pClipper->output();
}
