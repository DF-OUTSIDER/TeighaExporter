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




#ifndef _PDFSHADINGT4DICTIONARY_INCLUDED_
#define _PDFSHADINGT4DICTIONARY_INCLUDED_ /*!DOM*// 

#include "PdfShadingDictionary.h"
#include "PdfStream.h"
#include "Ge/GePoint2d.h"

namespace TD_PDF
{

class PDFIStream;
class PDFVersion;

class PDFEXPORT_TOOLKIT PDFShadingT4StreamDictionary : public PDFShadingDictionary
{
  PDF_DECLARE_OBJECT(PDFShadingT4StreamDictionary, PDFShadingDictionary, kShadingT4StreamDictionary)

protected:
  virtual void InitObject();

public:
  void setDecode(double xmin, double xmax, double ymin, double ymax, bool addDefRGBColor = true);

#include "PdfDictKeysDefs.h"
#include "PdfShadingT4DictionaryKeys.h"
#include "PdfDictKeysUnDefs.h"

};

class PDFEXPORT_TOOLKIT PDFShadingT4 : public PDFStream
{
  PDF_DECLARE_OBJECT(PDFShadingT4, PDFStream, kShadingT4Stream)

protected:
  virtual void InitObject();

public:
  void preFill();
  void postFill(double dMinx, double dMaxx, double dMiny, double dMaxy);
  void addTriangle( OdUInt32 x1,  OdUInt32 y1,
                    OdUInt32 x2,  OdUInt32 y2,
                    OdUInt32 x3,  OdUInt32 y3,
                    OdUInt8 red1, OdUInt8 green1, OdUInt8 blue1, 
                    OdUInt8 red2, OdUInt8 green2, OdUInt8 blue2, // 0 - 255 range
                    OdUInt8 red3, OdUInt8 green3, OdUInt8 blue3);
};

typedef PDFSmartPtr<PDFShadingT4> PDFShadingT4Ptr;
typedef PDFSmartPtr<PDFShadingT4StreamDictionary> PDFShadingT4StreamDictionaryPtr;

};

#endif //_PDFSHADINGT4DICTIONARY_INCLUDED_

