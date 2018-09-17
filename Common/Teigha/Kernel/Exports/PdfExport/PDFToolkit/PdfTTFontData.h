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
// PdfTTFontData.h
//
///////////////////////////////////////////////////////////////////////////////

#ifndef _PDF_TTFONTDATA_INCLUDED_
#define _PDF_TTFONTDATA_INCLUDED_

#include "Int32Array.h"
#include "UInt16Array.h"
#include "Int16Array.h"
#include "Gi/GiTextStyle.h"

class OdFont;

/** \details
  <group OdExport_Classes> 
*/
namespace TD_PDF {

struct pdf_tt_cmap;
struct pdf_tt_cmap_sub;
struct pdf_tt_hmtx_elem;
struct pdf_tt_hhea;
struct pdf_tt_head;
struct pdf_tt_os2;
struct pdf_tt_post;
struct pdf_tt_pclt;

/** \details
  This class implements the font data for PDF export.
*/
class PDFTTFontData
{
  const OdFont *m_pFont;
  const OdGiTextStyle* m_pTextStyle;

  OdInt32       m_dBBox[4];
  double        m_dDescent;
  double        m_dAscent;
  OdInt32       m_nFontDescriptorFlags;
  double        m_dStemV;
  double        m_dUnitsPerEM;
  
  mutable OdInt8     *m_pCMap;
  mutable OdUInt16  m_TableType;

  static OdUInt32 nHEAD;// = *((OdUInt32*)&"head");
  static OdUInt32 nGLYF;// = *((OdUInt32*)&"glyf");
  static OdUInt32 nHHEA;// = *((OdUInt32*)&"hhea");
  static OdUInt32 nHMTX;// = *((OdUInt32*)&"hmtx");
  static OdUInt32 nCMAP;// = *((OdUInt32*)&"cmap");
  static OdUInt32 nNAME;// = *((OdUInt32*)&"name");
  static OdUInt32 nOS2 ;// = *((OdUInt32*)&"OS/2");
  static OdUInt32 nPOST;// = *((OdUInt32*)&"post");
  static OdUInt32 nPCLT;// = *((OdUInt32*)&"PCLT");

public:

  enum TTFCharSet
  {
    kANSI_CHARSET        = 0x00,
    kDEFAULT_CHARSET     = 0x01,
    kSYMBOL_CHARSET      = 0x02,
    kMAC_CHARSET         = 0x4D,
    kSHIFTJIS_CHARSET    = 0x80,
    kHANGEUL_CHARSET     = 0x81,
    kGB2312_CHARSET      = 0x86,
    kCHINESEBIG5_CHARSET = 0x88,
    kGREEK_CHARSET       = 0xA1,
    kTURKISH_CHARSET     = 0xA2,
    kVIETNAMESE_CHARSET  = 0xA3,
    kHEBREW_CHARSET      = 0xB1,
    kARABIC_CHARSET      = 0xB2,
    kBALTIC_CHARSET      = 0xBA,
    kRUSSIAN_CHARSET     = 0xCC,
    kTHAI_CHARSET        = 0xDE,
    kEASTEUROPE_CHARSET  = 0xEE,
    kOEM_CHARSET         = 0xFF
  };

  enum TTFEncodingIDs
  {
    kSymbol   = 0,
    kUNICODE  = 1,
    kShiftJIS = 2,
    kBig5     = 3,
    kPRC      = 4,
    kWansung  = 5,
    kJohab    = 6
  };

  enum TTFPlatformIDs
  {
    kAppleUnicode = 0,
    kMacintosh    = 1,
    kISO          = 2,
    kWindows      = 3
  };

  enum TTFFontStretch
  {
    kUltraCondensed = 1, 
    kExtraCondensed = 2,
    kCondensed      = 3,
    kSemiCondensed  = 4,
    kNormal         = 5,
    kSemiExpanded   = 6,
    kExpanded       = 7,
    kExtraExpanded  = 8,
    kUltraExpanded  = 9
  };

  enum TTFCopyright
  {
    kInstallable = 0 ,             // Fonts with this setting indicate that they may be embedded and permanently 
                                   // installed on the remote system by an application. The user of the remote 
                                   // system acquires the identical rights, obligations and licenses for that font 
                                   // as the original purchaser of the font, and is subject to the same end-user 
                                   // license agreement, copyright, design patent, and/or trademark as was the original purchaser.
    kRestrictedLicense = 2,        // Fonts that have this bit set must not be modified, embedded or exchanged 
                                   // in any manner without first obtaining permission of the legal owner
                                   // Note: For Restricted License embedding to take effect, it must be the only level of embedding selected.
    kPreviewPrint      = 4,        // Fonts with this bit set indicate that they may be embedded within documents 
                                   // but must only be installed temporarily on the remote system. Any document 
                                   // which includes a Preview & Print embedded font must be opened "read-only;" 
                                   // the application must not allow the user to edit the document; it can only 
                                   // be viewed and/or printed.
    kEditable          = 8,        // Fonts with this bit set indicate that they may be embedded in documents, 
                                   // but must only be installed temporarily on the remote system. In contrast 
                                   // to Preview & Print fonts, documents containing Editable fonts may be opened 
                                   // "read-write;" editing is permitted, and changes may be saved.
    kNoSubsetting = 0x0100,        // When this bit is set, the font may not be subsetted prior to
                                   // embedding.Other embedding restrictions specified in bits 0 - 3 and 9 also apply.
    kBitmapEmbeddingOnly = 0x0200, // When this bit is set, only bitmaps contained in the font may be embedded.
                                   // No outline data may be embedded. If there are no bitmaps available in the font, then the font
                                   // is considered unembeddable and the embedding services will fail. Other embedding restrictions
                                   // specified in bits 0 - 3 and 8 also apply.
    kUndefined = 0xFF              // TTF hasn't OS/2 table
  };

  enum TFFWidthClass
  {
    kwThin       = 100,
    kwExtraLight = 200,
    kwLight      = 300,
    kwNormal     = 400,
    kwMedium     = 500,
    kwSemiDold   = 600,
    kwBold       = 700,
    kwExtraBold  = 800,
    kwBlack      = 900
  };
protected:

  static bool getEncodingByCharSet(TTFCharSet cs, TTFEncodingIDs &id);

  bool getFontName(OdAnsiString &fontName, TTFEncodingIDs id) const;
  bool getFontStyleName(OdAnsiString &fontStyle, TTFEncodingIDs id) const;

  bool getPOST(pdf_tt_post &post) const;
  bool getHEAD(pdf_tt_head &head) const;
  bool getHHEA(pdf_tt_hhea &hhea) const;
  bool getHMTX(pdf_tt_hmtx_elem* &pHMTX, OdUInt16 numberOfHMetrics) const;
  bool getCMAP(pdf_tt_cmap &cmap, pdf_tt_cmap_sub* &cmapSub) const;
  bool getOS2(pdf_tt_os2 &os2) const;
  bool getPCLT(pdf_tt_pclt &pclt) const;

public:
 
  OdResult getSubTableByEncodingID(TTFEncodingIDs id) const;

public:
  PDFTTFontData();
  ~PDFTTFontData();

  OdResult set(const OdFont *pFont, const OdGiTextStyle* pTextStyle);

  bool getWidthsByCharSet(OdInt32Array &nWidths, OdUInt8 nStartChar = 0, OdUInt8 nEndChar = 255);

  const OdInt32 *BBox() const { return m_dBBox; }
  double  Descent() const { return m_dDescent; }
  double  Ascent() const { return m_dDescent; }
  double  Ascender() const { return m_dAscent; }
  bool FontDescriptorFlags(OdInt32 &nFlags) const;
  double  StemV() const { return m_dStemV; }

  bool getFontName(OdAnsiString &fontName) const;
  // temporary fix for CORE-11783 
  bool getFontStyleName(OdAnsiString &fontName) const;

  bool getCID2GIDMap(OdUInt16 maxChar, OdUInt16Array &map, bool bSwap) const;
  bool getCID2W(OdUInt16 maxChar, OdInt16Array &nWidths) const;

  TTFCopyright getCopyright() const;
  bool fontCanBeEmbedded(bool& bAllowedEdit) const;

  OdAnsiString getFontStretch() const;
  bool getWidthClass(TFFWidthClass &widthClass) const;
  bool getItalicAngle(double &dItalicAngle) const;
  bool getCapHeight(double &dCapHeight) const;
  bool getXHeight(double &dXHeight) const;
};
}
#endif // _PDF_TTFONTDATA_INCLUDED_

