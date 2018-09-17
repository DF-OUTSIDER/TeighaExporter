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




#ifndef _PDFICONTENTCOMMANDS_INCLUDED_
#define _PDFICONTENTCOMMANDS_INCLUDED_ /*!DOM*// 

#include "PdfStream.h"
#include "PdfName.h"
#include "PdfLiteralString.h"
#include "Ge/GePoint2d.h"
#include "Ge/GeEllipArc2d.h"

namespace TD_PDF
{

class PDFEXPORT_TOOLKIT PDFIContentCommands
{
protected:

  virtual void F() = 0; // Equivalent to f; included only for compatibility. Although PDF consumer applications
                        // must be able to accept this operator, PDF producer applications should use f
                        // instead.
public:
  ///////////////////////////////////////////////////////////////////////////////
  // XObject operator
  //
  virtual void Do(PDFNamePtr pName) = 0; // Paint the specified XObject.

  ///////////////////////////////////////////////////////////////////////////////
  // Graphics State Operators
  //
  virtual void q() = 0; // Save the current graphics state on the graphics state stack
  virtual void Q() = 0; // Restore the graphics state
  virtual void gs(PDFNamePtr pGS) = 0;//set extended graphics state

                        // Modify the current transformation matrix
  virtual void cm(double a, double b, double c, double d, double e, double f) = 0;
                    

  virtual void w(double lw) = 0; // Line Weigth Operators
  virtual void J(PDFLineCap linecap) = 0;   // Set the line cap style in the graphics state
  virtual void j(PDFLineJoin linejoin) = 0; // Set the line join style in the graphics state

  ///////////////////////////////////////////////////////////////////////////////
  // Color Operators
  //
  virtual void cs(PDFNamePtr pColorSpace) = 0;
  virtual void CS(PDFNamePtr pColorSpace) = 0;
  virtual void sc(OdUInt8 indx) = 0;                 // for Indexed color space only
  virtual void SC(OdUInt8 indx) = 0;                 // for Indexed color space only
  virtual void RG(double r, double g, double b) = 0; // = /DeviceRGB CS + r g b SC
  virtual void rg(double r, double g, double b) = 0; // = /DeviceRGB CS + r g b SC
  virtual void g(double g) = 0;                      // = /DeviceGray
  virtual void G(double g) = 0;                      // = /DeviceGray

  // Path Construction Operators

  virtual void m(double x, double y, bool bToInt = true) = 0;
  virtual void l(double x, double y) = 0;
  virtual void c(double x1, double y1, double x2, double y2, double x3, double y3) = 0;
  virtual void v(double x2, double y2, double x3, double y3) = 0;
  virtual void y(double x2, double y2, double x3, double y3) = 0;

  virtual void m(const OdGePoint2d &xy, bool bToInt = true) { m(xy.x, xy.y, bToInt); }
  virtual void l(const OdGePoint2d &xy) { l(xy.x, xy.y); }
  virtual void c(const OdGePoint2d &xy1, const OdGePoint2d &xy2, const OdGePoint2d &xy3) { c(xy1.x, xy1.y, xy2.x, xy2.y, xy3.x, xy3.y); }
  virtual void v(const OdGePoint2d &xy2, const OdGePoint2d &xy3) { v(xy2.x, xy2.y, xy3.x, xy3.y); }
  virtual void y(const OdGePoint2d &xy2, const OdGePoint2d &xy3) { y(xy2.x, xy2.y, xy3.x, xy3.y); }

  virtual void re(double x, double y, double width, double height) = 0;
  
  virtual void h() = 0;     // Close the current subpath

  // Path-Painting Operators

  virtual void S() = 0;     // Stroke the path.
  virtual void s() = 0;     // Close and stroke the path. This operator has the same effect as the sequence h S.
  virtual void f() = 0;     // Fill the path, using the nonzero winding number rule to determine the region to fill
                            // Any subpaths that are open are implicitly closed before being filled.
  virtual void f_odd() = 0; // "f*" Fill the path, using the even-odd rule to determine the region to fill
  virtual void B() = 0;     // Fill and then stroke the path, using the nonzero winding number rule to determine
                            // the region to fill.
  virtual void B_odd() = 0; // "B*" Fill and then stroke the path, using the even-odd rule to determine the region to fill.
  virtual void b() = 0;     // Close, fill, and then stroke the path, using the nonzero winding number rule to determine
                            // the region to fill.
  virtual void b_odd() = 0; // "b*" Close, fill, and then stroke the path, using the even-odd rule to determine the region
                            // to fill.
  virtual void n() = 0;     // End the path object without filling or stroking it. This operator is a path-painting
                            // no-op, used primarily for the side effect of changing the current clipping path.

  // Clipping path operators
  virtual void W() = 0;     // Modify the current clipping path by intersecting it with the current path
  virtual void W_odd() = 0; // .... , using the even-odd rule

  // Text state operators

  virtual void Tc(double charSpace) = 0; // Set the character spacing, Tc, to charSpace
  virtual void Tw(double wordSpace) = 0; // Set the word spacing, Tw, to wordSpace
  virtual void Tz(double scale) = 0;     // Set the horizontal scaling, Th, to (scale . 100)
  virtual void TL(double leading) = 0;   // Set the text leading, Tl, to leading
  virtual void Tf(PDFNamePtr font, double size) = 0; // Set the text font, Tf, to font and the text font size
  virtual void Tr(OdUInt32 render) = 0;  // Set the text rendering mode
  virtual void Ts(double rise) = 0;      // Set the text rise

  // Text object operators
  virtual void BT() = 0; // Begin a text object
  virtual void ET() = 0; // End a text object

  // Text-positioning operators
  virtual void Td(double tx, double ty) = 0; // Move to the start of the next line
  virtual void TD(double tx, double ty) = 0; // Move to the start of the next line
  virtual void Tm(double a, double b, double c, double d, double e, double f) = 0; // Set the text matrix, Tm, and the text line matrix, Tlm:
  virtual void T_star() = 0; // T*           // Move to the start of the next line. This operator has the same effect as the code 0 Tl Td where Tl is the current leading parameter in the text state.

  virtual void Tj(PDFTextStringPtr pStr) = 0; // show text
  virtual void TJ(PDFArrayPtr pStr)      = 0; // show text array

  // Marked-content operators
  virtual void BDC(PDFNamePtr pTag, PDFObjectPtr pProperties) = 0; // Begin a marked-content sequence with an associated property list.
  virtual void BMC(PDFNamePtr pTag) = 0;                           // Begin a marked-content sequence
  virtual void EMC() = 0;                                          // End a marked-content sequence begun by a BMC or BDC operator.

  virtual void sh(PDFNamePtr pName) = 0;  // Paint the shape and color shading described by a shading dictionary.

  virtual ~PDFIContentCommands() {}

  // Auxiliary methods (call Path Construction Operators without // Path-Painting Operators)
  static void drawEllipse(PDFIContentCommands &pICommands, const OdGeEllipArc2d &ellipArc2d, bool bSkipFirstPoint = false, double deviation = 0.);
  static void drawPolyline(PDFIContentCommands &pICommands, OdUInt32 nPoints, const OdGePoint2d* pPoints);
  static void drawPolyline(PDFIContentCommands &pICommands, const OdGePoint2dArray &pnts2d);
  static void drawCurve(PDFIContentCommands &pICommands, const OdGeCurve2d &pCurve, double deviation = 0.);
  static void drawPoint(PDFIContentCommands &pICommands, const OdGePoint2d &pPoint2d);
};

};

#endif //_PDFICONTENTCOMMANDS_INCLUDED_

