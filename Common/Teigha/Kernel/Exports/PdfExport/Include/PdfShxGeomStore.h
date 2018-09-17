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

#ifndef _PDF_SHXGEOMSTORE_
#define _PDF_SHXGEOMSTORE_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Gi/GiEmptyGeometry.h"
#include "Ge/GePoint3dArray.h"
#include "Ge/GeCircArc3d.h"

#include "PdfIContentCommands4Type3.h"
#include "PdfIContentCommands.h"

using namespace TD_PDF;

namespace TD_PDF_2D_EXPORT{

/** \details
  This class implements the PDF box store.
*/
class PDFBBoxStore
{
  bool bInit;
  double dllx, dlly, durx, dury;

public:
  
  PDFBBoxStore()
  { clear(); }

  bool isInit() const { return bInit; }

  void add(PDFBBoxStore &bbox)
  {
    add(bbox.dllx, bbox.dlly, bbox.durx, bbox.dury);
  }
    
  void add(double x, double y)
  {
    add(x, y, x, y);
  }

  void add(double llx, double lly, double urx, double ury)
  {
    if (!bInit)
    {
      set(llx, lly, urx, ury);
    }
    else
    {
      dllx = odmin(llx, dllx);
      dlly = odmin(lly, dlly);
      durx = odmax(urx, durx);
      dury = odmax(ury, dury);
    }
  }

  void set(double llx, double lly, double urx, double ury)
  {
    dllx = llx;
    dlly = lly;
    durx = urx;
    dury = ury;

    bInit = true;
  }

  void get(double &llx, double &lly, double &urx, double &ury)
  {
    llx = dllx;
    lly = dlly;
    urx = durx;
    ury = dury;
  }

  void scaleBy(double dScale)
  {
    if (bInit)
    {
      dllx *= dScale;
      dlly *= dScale;
      durx *= dScale;
      dury *= dScale;
    }
  }

  void clear()
  {
    bInit = false;
    dllx = dlly = durx = dury = 0;
  }
};


//////////////////////////////////////////////////////////////////////

/** \details
  This class implements the PDF SHX geometry store for PDF export.
*/
class PDFShxGeomStore : public OdGiEmptyGeometry
{
  PDFIContentCommands       *m_pCommands;
  PDFIContentCommands4Type3 *m_pCommands4T3;

  OdArray<OdGePoint3dArray>  m_Lines;
  OdArray<OdGeCircArc3d>     m_Circles;
  OdGePoint2d                m_adv;
  PDFBBoxStore               m_BBox;

  double                     m_dGeomScale;

  OdArray<OdGePoint2dArray> m_OuterContours;
  OdArray<OdGePoint2dArray> m_InnerContours;

protected:
  void CalculateBBox(PDFBBoxStore &pBBox);
public:
  PDFShxGeomStore(PDFIContentCommands4Type3 *pCommands4T3, PDFIContentCommands *pCommands);

  virtual bool getBBox(PDFBBoxStore &pBBox) const;
  virtual void setAdvance(OdGePoint2d &adv);

  virtual double getCharWidth() const;

  virtual void setScale(double dGeomScale);
  virtual double getScale() const;

  virtual void polylineProc(
    OdInt32 numPoints, const OdGePoint3d* vertexList,
    const OdGeVector3d* pNormal = 0,
    const OdGeVector3d* pExtrusion = 0,
    OdGsMarker baseSubEntMarker = -1);

  virtual void circleProc(
    const OdGePoint3d& center, double radius,
    const OdGeVector3d& normal,
    const OdGeVector3d* pExtrusion = 0);

  virtual void circularArcProc(
    const OdGePoint3d& center,
    double radius,
    const OdGeVector3d& normal,
    const OdGeVector3d& startVector,
    double sweepAngle,
    OdGiArcType arcType = kOdGiArcSimple, const OdGeVector3d* pExtrusion = 0);
  
  virtual void circularArcProc(
    const OdGePoint3d& firstPoint,
    const OdGePoint3d& secondPoint,
    const OdGePoint3d& thirdPoint,
    OdGiArcType arcType = kOdGiArcSimple, const OdGeVector3d* pExtrusion = 0);

  virtual void shellProc(OdInt32 nbVertex,
    const OdGePoint3d* pVertexList,
    OdInt32 faceListSize,
    const OdInt32* pFaceList,
    const OdGiEdgeData* pEdgeData = 0,
    const OdGiFaceData* pFaceData = 0,
    const OdGiVertexData* pVertexData = 0);

  virtual void fillContent(double lw);
};

}
#endif // #ifndef _PDF_SHXGEOMSTORE_
