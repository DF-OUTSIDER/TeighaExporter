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




#ifndef _PDFFIELDFLAGS_INCLUDED_
#define _PDFFIELDFLAGS_INCLUDED_ /*!DOM*// 

#include "PdfInteger.h"

namespace TD_PDF
{

class PDFIStream;
class PDFVersion;

class PDFEXPORT_TOOLKIT PDFFieldFlags : public PDFInteger
{
  PDF_DECLARE_OBJECT(PDFFieldFlags, PDFInteger, kFieldFlags)
  
public:
  enum PDFFieldFlagsEnum
  {
    kReadOnly = 1,
    kRequired = 2,
    kNoExport = 3,
    kMultiline = 13,
    kPassword = 14,
    kNoToggleToOff = 15,
    kRadio = 16,
    kPushbutton = 17,
    kCombo = 18,
    kEdit = 19,
    kSort  = 20,
    kFileSelect = 21,
    kMultiSelect = 22,
    kDoNotSpellCheck  = 23,
    kDoNotScroll = 24,
    kComb = 25,
    kRichText = 26,
    kRadiosInUnison = 26,
    kCommitOnSelChange = 27
  };

  virtual void setBit(PDFFieldFlagsEnum bitNum, bool bVal);
  virtual bool getBit(PDFFieldFlagsEnum bitNum) const;
};

typedef PDFSmartPtr<PDFFieldFlags> PDFFieldFlagsPtr;

};

#endif //_PDFFIELDFLAGS_INCLUDED_

