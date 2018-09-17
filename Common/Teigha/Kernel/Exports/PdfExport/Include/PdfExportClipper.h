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

#ifndef _PDF_EXPORT_CLIPPER_H_INCLUDED_
#define _PDF_EXPORT_CLIPPER_H_INCLUDED_

#include "TD_PackPush.h"

#include "Gi/GiOrthoClipperEx.h"
#include "Gs/GsDefs.h"

/** \details
  <group OdExport_Classes>
  This class implements Viewport Clipping manager for PDF Device objects.
*/
class PdfExportClipper : public OdGiConveyorNode
{
  OdGiOrthoClipperExPtr m_pClipper;
protected:
  PdfExportClipper();
public:
  static OdSmartPtr<PdfExportClipper> createObject();

  bool isClipping() const { return !m_pClipper->isEmpty(); }

  bool push(const OdGsDCPoint* points, const int *pointCounts, int numPolygons);
  bool push(const OdGePoint2d& min, const OdGePoint2d& max);
  bool pop();

  void setDrawContext(OdGiConveyorContext* pDrawContext);
  void setDeviation(const OdGeDoubleArray& deviations);
  void setDeviation(const OdGiDeviation* pDeviation);

  void enable(bool bEnable)
  {
    bEnable ? m_pClipper->enable() : m_pClipper->disable();
  }
  bool enabled()
  {
    return m_pClipper->enabled();
  }

  // OdGiConveyorNode methods

  virtual OdGiConveyorInput& input();
  virtual OdGiConveyorOutput& output();

  OdGiOrthoClipperEx::VisibilityStatus checkVisibility(const OdGeExtents3d &extents) const
  {
    return m_pClipper->checkExtentsVisibility(extents);
  }
};

/** \details
    This template class is a specialization of the OdSmartPtr class for PdfExportClipper object pointers.
*/
typedef OdSmartPtr<PdfExportClipper> PdfExportClipperPtr;

#include "TD_PackPop.h"

#endif // _PDF_EXPORT_CLIPPER_H_INCLUDED_
