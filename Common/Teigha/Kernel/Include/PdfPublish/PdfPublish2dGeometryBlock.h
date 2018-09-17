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


///////////////////////////////////////////////////////////////////////////////
//
// PdfPublish2dGeometryBlock.h
//
///////////////////////////////////////////////////////////////////////////////

#ifndef _PDF_PUBLISH_2DGEOM_BLOCK_INCLUDED_
#define _PDF_PUBLISH_2DGEOM_BLOCK_INCLUDED_

#include "PdfPublishCommon.h"
#include "PdfPublishBaseObject.h"
#include "PdfPublishImage.h"
#include "PdfPublishText.h"

#include "Ge/GeEllipArc2d.h"

/** \details 
Contains declarations for Teigha Publish.
*/
namespace OdPdfPublish{

/** \details
  This class implements an 2d geometry in .pdf documents created with Teigha Publish.
*/
class PDFPUBLISH_TOOLKIT Od2dGeometryBlock : public OdObject
{
  //DOM-IGNORE-BEGIN
protected:
  Od2dGeometryBlock();
  //DOM-IGNORE-END
public:
  //DOM-IGNORE-BEGIN
  ODRX_DECLARE_MEMBERS(Od2dGeometryBlock);
  ODRX_HEAP_OPERATORS();
  //DOM-IGNORE-END

  /** \details
    Destroys an object.
  */
  virtual ~Od2dGeometryBlock();

  void setOrigin(const OdGePoint2d& origin);

  void addLine(const OdGePoint2dArray& points);
  void addLine(const OdGePoint2d& start, const OdGePoint2d& end);
  void addLine(OdUInt32 nPoints, const OdGePoint2d* pPoints);
  void addCircle(const OdGeCircArc2d& circle);
  void addEllipse(const OdGeEllipArc2d& ellipse);
  void addCurve(const OdGeNurbCurve2d& nurb);

  void putColor(ODCOLORREF color);
  void putLineWeight(double lw);
  void putLineCap(Geometry::PDFLineCap lineCap);
  void putLineJoin(Geometry::PDFLineJoin lineJoin);
  void putTransform(const OdGeMatrix2d& transform);
  
  //start drawing contour
  void startContour();
  //finish contour using rule
  void finishContour(Geometry::PDFFinishRule rule);

  void addText(const OdTextPtr& text, const OdRect& location);
  void addImage(const OdImagePtr& image, const OdRect& location);
};

/** \details
  A data type that represents a smart pointer to an <link OdPdfPublish::Od2dGeometryBlock, Od2dGeometryBlock> object.
*/
SMARTPTR(Od2dGeometryBlock);

}
#endif // _PDF_PUBLISH_2DGEOM_BLOCK_INCLUDED_

