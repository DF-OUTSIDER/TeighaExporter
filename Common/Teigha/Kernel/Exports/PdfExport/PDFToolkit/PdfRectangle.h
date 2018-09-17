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




#ifndef _PDFRECTANGLE_INCLUDED_
#define _PDFRECTANGLE_INCLUDED_ /*!DOM*// 

#include "PdfObject.h"
#include "Ge/GeExtents2d.h"

namespace TD_PDF
{

class PDFIStream;
class PDFVersion;

class PDFEXPORT_TOOLKIT PDFRectangle : public PDFObject
{
  OdInt32 m_lx, m_ux, m_ly, m_uy;
  PDF_DECLARE_OBJECT(PDFRectangle, PDFObject, kRectangle)
public:
  void set(OdInt32 lx, OdInt32 ly, OdInt32 ux, OdInt32 uy);
  void set(const OdGeExtents2d &extents2d);

  void get(OdInt32& lx, OdInt32& ly, OdInt32& ux, OdInt32& uy) const;

  virtual bool Export(PDFIStream *pStream, const PDFVersion &ver);
};

typedef PDFSmartPtr<PDFRectangle> PDFRectanglePtr;

};

#endif //_PDFRECTANGLE_INCLUDED_

