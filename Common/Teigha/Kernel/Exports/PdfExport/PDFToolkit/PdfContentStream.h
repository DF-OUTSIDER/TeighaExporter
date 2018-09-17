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




#ifndef _PDFCONTENTSTREAM_INCLUDED_
#define _PDFCONTENTSTREAM_INCLUDED_ /*!DOM*// 

#include "PdfStream.h"
#include "PdfIContentCommands.h"

namespace TD_PDF
{

class PDFIStream;
class PDFVersion;

#define FORMAT_BUF_LEN 100

class PDFEXPORT_TOOLKIT PDFContentStream : public PDFStream, public PDFIContentCommands
{
  PDF_DECLARE_OBJECT(PDFContentStream, PDFStream, kContentStream)

  char m_format_buf[FORMAT_BUF_LEN];
protected:
  static OdAnsiString format(OdUInt32 n);

  static OdAnsiString format(double d, int cropzeros = CROP_ZEROES_FOR_ALL);
  static OdAnsiString format(double d0, double d1, int cropzeros = CROP_ZEROES_FOR_ALL);
  static OdAnsiString format(double d0, double d1, double d2, int cropzeros = CROP_ZEROES_FOR_ALL);
  static OdAnsiString format(double d0, double d1, double d2, double d3, int cropzeros = CROP_ZEROES_FOR_ALL);
  static OdAnsiString format(double d0, double d1, double d2, double d3, double d4, int cropzeros = CROP_ZEROES_FOR_ALL);
  static OdAnsiString format(double d0, double d1, double d2, double d3, double d4, double d5, int cropzeros = CROP_ZEROES_FOR_ALL);

  void putString(const char* val);

  virtual void InitObject();

  virtual void F(); // Equivalent to f; included only for compatibility. Although PDF consumer applications
                    // must be able to accept this operator, PDF producer applications should use f
                    // instead.
public:
  ///////////////////////////////////////////////////////////////////////////////
  // XObject operator
  //
  virtual void Do(PDFNamePtr pName); // Paint the specified XObject.

  ///////////////////////////////////////////////////////////////////////////////
  // Graphics State Operators
  //
  virtual void q(); // Save the current graphics state on the graphics state stack
  virtual void Q(); // Restore the graphics state
  virtual void gs(PDFNamePtr pGS);//set extended graphics state

                    // Modify the current transformation matrix
  virtual void cm(double a, double b, double c, double d, double e, double f);
                    

  virtual void w(double lw); // Line Weigth Operators
  virtual void J(PDFLineCap linecap);   // Set the line cap style in the graphics state
  virtual void j(PDFLineJoin linejoin); // Set the line join style in the graphics state

  virtual void d(PDFArrayPtr dashArray, OdUInt32 phase); //line dash pattern

  ///////////////////////////////////////////////////////////////////////////////
  // Color Operators
  //
  virtual void cs(PDFNamePtr pColorSpace);
  virtual void CS(PDFNamePtr pColorSpace);
  virtual void sc(OdUInt8 indx);                 // for Indexed color space only
  virtual void SC(OdUInt8 indx);                 // for Indexed color space only
  virtual void RG(double r, double g, double b); // = /DeviceRGB CS + r g b SC
  virtual void rg(double r, double g, double b); // = /DeviceRGB CS + r g b SC
  virtual void g(double g);                      // = /DeviceGray
  virtual void G(double g);                      // = /DeviceGray

  // Path Construction Operators

  virtual void m(double x, double y, bool bToInt = true);
  virtual void l(double x, double y);
  virtual void c(double x1, double y1, double x2, double y2, double x3, double y3);
  virtual void v(double x2, double y2, double x3, double y3);
  virtual void y(double x2, double y2, double x3, double y3);

  virtual void re(double x, double y, double width, double height);
  
  virtual void h();     // Close the current subpath

  // Path-Painting Operators

  virtual void S();     // Stroke the path.
  virtual void s();     // Close and stroke the path. This operator has the same effect as the sequence h S.
  virtual void f();     // Fill the path, using the nonzero winding number rule to determine the region to fill
                        // Any subpaths that are open are implicitly closed before being filled.
  virtual void f_odd(); // "f*" Fill the path, using the even-odd rule to determine the region to fill
  virtual void B();     // Fill and then stroke the path, using the nonzero winding number rule to determine
                        // the region to fill.
  virtual void B_odd(); // "B*" Fill and then stroke the path, using the even-odd rule to determine the region to fill.
  virtual void b();     // Close, fill, and then stroke the path, using the nonzero winding number rule to determine
                        // the region to fill.
  virtual void b_odd(); // "b*" Close, fill, and then stroke the path, using the even-odd rule to determine the region
                        // to fill.
  virtual void n();     // End the path object without filling or stroking it. This operator is a path-painting
                        // no-op, used primarily for the side effect of changing the current clipping path.

  // Clipping path operators
  virtual void W();     // Modify the current clipping path by intersecting it with the current path
  virtual void W_odd(); // .... , using the even-odd rule

  // Text state operators

  virtual void Tc(double charSpace); // Set the character spacing, Tc, to charSpace
  virtual void Tw(double wordSpace); // Set the word spacing, Tw, to wordSpace
  virtual void Tz(double scale);     // Set the horizontal scaling, Th, to (scale . 100)
  virtual void TL(double leading);   // Set the text leading, Tl, to leading
  virtual void Tf(PDFNamePtr font, double size); // Set the text font, Tf, to font and the text font size
  virtual void Tr(OdUInt32 render);  // Set the text rendering mode
  virtual void Ts(double rise);      // Set the text rise

  // Text object operators
  virtual void BT(); // Begin a text object
  virtual void ET(); // End a text object

  // Text-positioning operators
  virtual void Td(double tx, double ty); // Move to the start of the next line
  virtual void TD(double tx, double ty); // Move to the start of the next line
  virtual void Tm(double a, double b, double c, double d, double e, double f); // Set the text matrix, Tm, and the text line matrix, Tlm:
  virtual void T_star(); // T*           // Move to the start of the next line. This operator has the same effect as the code 0 Tl Td where Tl is the current leading parameter in the text state.

  virtual void Tj(PDFTextStringPtr pStr); // show text
  virtual void TJ(PDFArrayPtr pStr); // show text array

  // Marked-content operators
  virtual void BDC(PDFNamePtr pTag, PDFObjectPtr pProperties); // Begin a marked-content sequence with an associated property list.
  virtual void BMC(PDFNamePtr pTag);                           // Begin a marked-content sequence
  virtual void EMC();                                          // End a marked-content sequence begun by a BMC or BDC operator.

  virtual void sh(PDFNamePtr pName);
};

class PDFEXPORT_TOOLKIT PDFDummyContentStream : public PDFIContentCommands
{
  virtual void F() { }
public:
  virtual void Do(PDFNamePtr ) { } 
  virtual void q() { } 
  virtual void Q() { } 
  virtual void cm(double , double , double , double , double , double ) { }
  virtual void w(double ) { } 
  virtual void J(PDFLineCap ) { } 
  virtual void d(PDFArrayPtr dashArray, OdUInt32 phase) { }
  virtual void j(PDFLineJoin ) { } 
  virtual void cs(PDFNamePtr ) { }
  virtual void gs(PDFNamePtr) { }
  virtual void CS(PDFNamePtr ) { }
  virtual void sc(OdUInt8 ) { }                 
  virtual void SC(OdUInt8 ) { }                 
  virtual void RG(double , double , double ) { } 
  virtual void rg(double , double , double ) { } 
  virtual void g(double g) { }
  virtual void G(double g) { }
  virtual void m(double , double , bool = true) { }
  virtual void l(double , double ) { }
  virtual void c(double , double , double , double , double , double ) { }
  virtual void v(double , double , double , double ) { }
  virtual void y(double , double , double , double ) { }
  virtual void re(double , double , double , double ) { }
  virtual void h() { }     
  virtual void S() { }     
  virtual void s() { }     
  virtual void f() { }     
  virtual void f_odd() { } 
  virtual void B() { }     
  virtual void B_odd() { } 
  virtual void b() { }     
  virtual void b_odd() { } 
  virtual void n() { }     
  virtual void W() { }     
  virtual void W_odd() { } 
  virtual void Tc(double ) { } 
  virtual void Tw(double ) { } 
  virtual void Tz(double ) { }     
  virtual void TL(double ) { }   
  virtual void Tf(PDFNamePtr , double ) { } 
  virtual void Tr(OdUInt32 ) { }  
  virtual void Ts(double ) { }      
  virtual void BT() { } 
  virtual void ET() { } 
  virtual void Td(double , double ) { } 
  virtual void TD(double , double ) { } 
  virtual void Tm(double , double , double , double , double , double ) { } 
  virtual void T_star() { } 
  virtual void Tj(PDFTextStringPtr ) { } 
  virtual void TJ(PDFArrayPtr) {}
  virtual void BDC(PDFNamePtr , PDFObjectPtr ) { }
  virtual void BMC(PDFNamePtr) {}
  virtual void EMC() { }
  virtual void sh(PDFNamePtr ) { }
};

typedef PDFSmartPtr<PDFContentStream> PDFContentStreamPtr;

};

#endif //_PDFCONTENTSTREAM_INCLUDED_

