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
// PdfExportImpl.cpp : implementation of the CPdfExportImpl class
//
///////////////////////////////////////////////////////////////////////////////

#include "OdaCommon.h"
//#ifdef _WIN32
#define STL_USING_IOSTREAM
#include "OdaSTL.h"
//#endif

#include "PdfExportCommon.h"

#include "Ge/GeExtents3d.h"
#include "OdTimeStamp.h"
#include "ColorMapping.h"
#include "2dSupport.h"
#include "Pdf2dExportDevice.h"
#include "Pdf2dExportView.h"
#include "PdfExportImpl.h"
#include "PdfPageNodeDictionary.h"
#include "PdfOCManager.h"
#include "Gs/GsPageParams.h"
#include "PdfAux.h"
#include "PdfLinearizer.h"

#define STL_USING_MAP
#define STL_USING_VECTOR
#include "OdaSTL.h"

#include "PdfExportService.h"
#include "RxObject.h"
#include "AbstractViewPE.h"
#include "MemoryStream.h"

namespace TD_PDF_2D_EXPORT {

static PDFVersion ver2ver(PDFExportParams::PDFExportVersions ver)
{
  switch(ver)
  {
  case PDFExportParams::kPDFv1_0:
    return PDFVersion::kPDFv1_0;
  case PDFExportParams::kPDFv1_1:
    return PDFVersion::kPDFv1_1;
  case PDFExportParams::kPDFv1_2:
    return PDFVersion::kPDFv1_2;
  case PDFExportParams::kPDFv1_3:
    return PDFVersion::kPDFv1_3;
  case PDFExportParams::kPDFv1_4:
    return PDFVersion::kPDFv1_4;
  case PDFExportParams::kPDFv1_5:
    return PDFVersion::kPDFv1_5;
  case PDFExportParams::kPDFv1_6:
    return PDFVersion::kPDFv1_6;
  case PDFExportParams::kPDFv1_7:
    return PDFVersion::kPDFv1_7;
  };

  return PDFVersion::kPDFUnknown;
}

//***************************************************************************
// 
//***************************************************************************
CPdfExportImpl::CPdfExportImpl()
: m_ParamsHolder(m_Document, m_FontOptimizer, m_Type3Optimizer)
{
}

CPdfExportImpl::~CPdfExportImpl()
{
  // must be called to clear all dictionaries to prevent cross links by smart ptrs.
  m_Document.clearDictionaries();
}

#if 0 //SRC-895 PDF export creates imprecise dimensions of predefined standard paper sizes
// AMark : #5917 : Some paper sizes of ISO standart has not exact mm metrics. But if paper sizes specified by rounded mm values,
//   this function fixes Adobe point width and height for this paper sizes.
static bool odPdfFixPaperSize(const OdGsPageParams &params, double &fFixWidth, double &fFixHeight)
{
  struct CheckPS {
    double mmWidth, mmHeight;
    double ptWidth, ptHeight;
  };
  static const CheckPS cps[] =
  {
    // AutoGenerator based on following algorithm: http://us3.php.net/manual/en/function.pdf-begin-page.php
    // Fixed by following mm metrics: http://www.prepressure.com/library/paper-sizes
    // ISO 216 Standart: http://en.wikipedia.org/wiki/ISO_216
    // DL: http://en.wikipedia.org/wiki/ISO_269
    // RA, SRA: http://en.wikipedia.org/wiki/ISO_217
    // Paper sizes: http://en.wikipedia.org/wiki/Paper_size

    // "a0": 840.8964152537mm x 1189.2071150027mm == 2383.6433818216pt x 3370.9807984329pt
    // AutoRounded: 840mm x 1189mm (Fixed {1})
    { 841.0, 1189.0, 2383.6433818216, 3370.9807984329 },
    // "a0 (Landscape)": 1189.2071150027mm x 840.8964152537mm == 3370.9807984329pt x 2383.6433818216px
    // AutoRounded: 1189mm x 840mm (Fixed {1})
    { 1189.0, 841.0, 3370.9807984329, 2383.6433818216 },
    // "a1": 594.6035575014mm x 840.8964152537mm == 1685.4903992165px x 2383.6433818216px
    // AutoRounded: 594mm x 840mm (Fixed {1})
    { 594.0, 841.0, 1685.4903992165, 2383.6433818216 },
    // "a1 (Landscape)": 840.8964152537mm x 594.6035575014mm == 2383.6433818216px x 1685.4903992165px
    // AutoRounded: 840mm x 594mm (Fixed {1})
    { 841.0, 594.0, 2383.6433818216, 1685.4903992165 },
    // "a2": 420.4482076269mm x 594.6035575014mm == 1191.8216909108px x 1685.4903992165px
    // AutoRounded: 420mm x 594mm (Fixed {0})
    { 420.0, 594.0, 1191.8216909108, 1685.4903992165 },
    // "a2 (Landscape)": 594.6035575014mm x 420.4482076269mm == 1685.4903992165pt x 1191.8216909108pt
    // AutoRounded: 594mm x 420mm (Fixed {0})
    { 594.0, 420.0, 1685.4903992165, 1191.8216909108 },
    // "a3": 297.3017787507mm x 420.4482076269mm == 842.7451996082pt x 1191.8216909108pt
    // AutoRounded: 297mm x 420mm (Fixed {0})
    { 297.0, 420.0, 842.7451996082, 1191.8216909108 },
    // "a3 (Landscape)": 420.4482076269mm x 297.3017787507mm == 1191.8216909108pt x 842.7451996082pt
    // AutoRounded: 420mm x 297mm (Fixed {0})
    { 420.0, 297.0, 1191.8216909108, 842.7451996082 },
    // "a4": 210.2241038134mm x 297.3017787507mm == 595.9108454554pt x 842.7451996082pt
    // AutoRounded: 210mm x 297mm (Fixed {0})
    { 210.0, 297.0, 595.9108454554, 842.7451996082 },
    // "a4 (Landscape)": 297.3017787507mm x 210.2241038134mm == 842.7451996082pt x 595.9108454554pt
    // AutoRounded: 297mm x 210mm (Fixed {0})
    { 297.0, 210.0, 842.7451996082, 595.9108454554 },
    // "a5": 148.6508893753mm x 210.2241038134mm == 421.3725998041pt x 595.9108454554pt
    // AutoRounded: 148mm x 210mm (Fixed {0})
    { 148.0, 210.0, 421.3725998041, 595.9108454554 },
    // "a5 (Landscape)": 210.2241038134mm x 148.6508893753mm == 595.9108454554pt x 421.3725998041pt
    // AutoRounded: 210mm x 148mm (Fixed {0})
    { 210.0, 148.0, 595.9108454554, 421.3725998041 },
    // "a6": 105.1120519067mm x 148.6508893753mm == 297.9554227277pt x 421.3725998041pt
    // AutoRounded: 105mm x 148mm (Fixed {0})
    { 105.0, 148.0, 297.9554227277, 421.3725998041 },
    // "a6 (Landscape)": 148.6508893753mm x 105.1120519067mm == 421.3725998041pt x 297.9554227277pt
    // AutoRounded: 148mm x 105mm (Fixed {0})
    { 148.0, 105.0, 421.3725998041, 297.9554227277 },
    // "a7": 74.3254446877mm x 105.1120519067mm == 210.6862999021pt x 297.9554227277pt
    // AutoRounded: 74mm x 105mm (Fixed {0})
    { 74.0, 105.0, 210.6862999021, 297.9554227277 },
    // "a7 (Landscape)": 105.1120519067mm x 74.3254446877mm == 297.9554227277pt x 210.6862999021pt
    // AutoRounded: 105mm x 74mm (Fixed {0})
    { 105.0, 74.0, 297.9554227277, 210.6862999021 },
    // "a8": 52.5560259534mm x 74.3254446877mm == 148.9777113638pt x 210.6862999021pt
    // AutoRounded: 52mm x 74mm (Fixed {0})
    { 52.0, 74.0, 148.9777113638, 210.6862999021 },
    // "a8 (Landscape)": 74.3254446877mm x 52.5560259534mm == 210.6862999021pt x 148.9777113638pt
    // AutoRounded: 74mm x 52mm (Fixed {0})
    { 74.0, 52.0, 210.6862999021, 148.9777113638 },
    // "a9": 37.1627223438mm x 52.5560259534mm == 105.3431499510pt x 148.9777113638pt
    // AutoRounded: 37mm x 52mm (Fixed {0})
    { 37.0, 52.0, 105.3431499510, 148.9777113638 },
    // "a9 (Landscape)": 52.5560259534mm x 37.1627223438mm == 148.9777113638pt x 105.3431499510pt
    // AutoRounded: 52mm x 37mm (Fixed {0})
    { 52.0, 37.0, 148.9777113638, 105.3431499510 },
    // "a10": 26.2780129767mm x 37.1627223438mm == 74.4888556819pt x 105.3431499510pt
    // AutoRounded: 26mm x 37mm (Fixed {0})
    { 26.0, 37.0, 74.4888556819, 105.3431499510 },
    // "a10 (Landscape)": 37.1627223438mm x 26.2780129767mm == 105.3431499510pt x 74.4888556819pt
    // AutoRounded: 37mm x 26mm (Fixed {0})
    { 37.0, 26.0, 105.3431499510, 74.4888556819 },
    // "b0": 1000.0000000000mm x 1414.2135623731mm == 2834.6456692913pt x 4008.7943500340pt
    // AutoRounded: 1000mm x 1414mm (Fixed {0})
    { 1000.0, 1414.0, 2834.6456692913, 4008.7943500340 },
    // "b0 (Landscape)": 1414.2135623731mm x 1000.0000000000mm == 4008.7943500340pt x 2834.6456692913pt
    // AutoRounded: 1414mm x 1000mm (Fixed {0})
    { 1414.0, 1000.0, 4008.7943500340, 2834.6456692913 },
    // "b1": 707.1067811865mm x 1000.0000000000mm == 2004.3971750170pt x 2834.6456692913pt
    // AutoRounded: 707mm x 1000mm (Fixed {0})
    { 707.0, 1000.0, 2004.3971750170, 2834.6456692913 },
    // "b1 (Landscape)": 1000.0000000000mm x 707.1067811865mm == 2834.6456692913pt x 2004.3971750170pt
    // AutoRounded: 1000mm x 707mm (Fixed {0})
    { 1000.0, 707.0, 2834.6456692913, 2004.3971750170 },
    // "b2": 500.0000000000mm x 707.1067811865mm == 1417.3228346457pt x 2004.3971750170pt
    // AutoRounded: 500mm x 707mm (Fixed {0})
    { 500.0, 707.0, 1417.3228346457, 2004.3971750170 },
    // "b2 (Landscape)": 707.1067811865mm x 500.0000000000mm == 2004.3971750170pt x 1417.3228346457pt
    // AutoRounded: 707mm x 500mm (Fixed {0})
    { 707.0, 500.0, 2004.3971750170, 1417.3228346457 },
    // "b3": 353.5533905933mm x 500.0000000000mm == 1002.1985875085pt x 1417.3228346457pt
    // AutoRounded: 353mm x 500mm (Fixed {0})
    { 353.0, 500.0, 1002.1985875085, 1417.3228346457 },
    // "b3 (Landscape)": 500.0000000000mm x 353.5533905933mm == 1417.3228346457pt x 1002.1985875085pt
    // AutoRounded: 500mm x 353mm (Fixed {0})
    { 500.0, 353.0, 1417.3228346457, 1002.1985875085 },
    // "b4": 250.0000000000mm x 353.5533905933mm == 708.6614173228pt x 1002.1985875085pt
    // AutoRounded: 250mm x 353mm (Fixed {0})
    { 250.0, 353.0, 708.6614173228, 1002.1985875085 },
    // "b4 (Landscape)": 353.5533905933mm x 250.0000000000mm == 1002.1985875085pt x 708.6614173228pt
    // AutoRounded: 353mm x 250mm (Fixed {0})
    { 353.0, 250.0, 1002.1985875085, 708.6614173228 },
    // "b5": 176.7766952966mm x 250.0000000000mm == 501.0992937542pt x 708.6614173228pt
    // AutoRounded: 176mm x 250mm (Fixed {0})
    { 176.0, 250.0, 501.0992937542, 708.6614173228 },
    // "b5 (Landscape)": 250.0000000000mm x 176.7766952966mm == 708.6614173228pt x 501.0992937542pt
    // AutoRounded: 250mm x 176mm (Fixed {0})
    { 250.0, 176.0, 708.6614173228, 501.0992937542 },
    // "b6": 125.0000000000mm x 176.7766952966mm == 354.3307086614pt x 501.0992937542pt
    // AutoRounded: 125mm x 176mm (Fixed {0})
    { 125.0, 176.0, 354.3307086614, 501.0992937542 },
    // "b6 (Landscape)": 176.7766952966mm x 125.0000000000mm == 501.0992937542pt x 354.3307086614pt
    // AutoRounded: 176mm x 125mm (Fixed {0})
    { 176.0, 125.0, 501.0992937542, 354.3307086614 },
    // "b7": 88.3883476483mm x 125.0000000000mm == 250.5496468771pt x 354.3307086614pt
    // AutoRounded: 88mm x 125mm (Fixed {0})
    { 88.0, 125.0, 250.5496468771, 354.3307086614 },
    // "b7 (Landscape)": 125.0000000000mm x 88.3883476483mm == 354.3307086614pt x 250.5496468771pt
    // AutoRounded: 125mm x 88mm (Fixed {0})
    { 125.0, 88.0, 354.3307086614, 250.5496468771 },
    // "b8": 62.5000000000mm x 88.3883476483mm == 177.1653543307pt x 250.5496468771pt
    // AutoRounded: 62mm x 88mm (Fixed {0})
    { 62.0, 88.0, 177.1653543307, 250.5496468771 },
    // "b8 (Landscape)": 88.3883476483mm x 62.5000000000mm == 250.5496468771pt x 177.1653543307pt
    // AutoRounded: 88mm x 62mm (Fixed {0})
    { 88.0, 62.0, 250.5496468771, 177.1653543307 },
    // "b9": 44.1941738242mm x 62.5000000000mm == 125.2748234386pt x 177.1653543307pt
    // AutoRounded: 44mm x 62mm (Fixed {0})
    { 44.0, 62.0, 125.2748234386, 177.1653543307 },
    // "b9 (Landscape)": 62.5000000000mm x 44.1941738242mm == 177.1653543307pt x 125.2748234386pt
    // AutoRounded: 62mm x 44mm (Fixed {0})
    { 62.0, 44.0, 177.1653543307, 125.2748234386 },
    // "b10": 31.2500000000mm x 44.1941738242mm == 88.5826771654pt x 125.2748234386pt
    // AutoRounded: 31mm x 44mm (Fixed {0})
    { 31.0, 44.0, 88.5826771654, 125.2748234386 },
    // "b10 (Landscape)": 44.1941738242mm x 31.2500000000mm == 125.2748234386pt x 88.5826771654pt
    // AutoRounded: 44mm x 31mm (Fixed {0})
    { 44.0, 31.0, 125.2748234386, 88.5826771654 },
    // "c0": 917.0040432047mm x 1296.8395546510mm == 2599.3815397928pt x 3676.0806273572pt
    // AutoRounded: 917mm x 1296mm (Fixed {1})
    { 917.0, 1297.0, 2599.3815397928, 3676.0806273572 },
    // "c0 (Landscape)": 1296.8395546510mm x 917.0040432047mm == 3676.0806273572pt x 2599.3815397928pt
    // AutoRounded: 1296mm x 917mm (Fixed {1})
    { 1297.0, 917.0, 3676.0806273572, 2599.3815397928 },
    // "c1": 648.4197773255mm x 917.0040432047mm == 1838.0403136786pt x 2599.3815397928pt
    // AutoRounded: 648mm x 917mm (Fixed {0})
    { 648.0, 917.0, 1838.0403136786, 2599.3815397928 },
    // "c1 (Landscape)": 917.0040432047mm x 648.4197773255mm == 2599.3815397928pt x 1838.0403136786pt
    // AutoRounded: 917mm x 648mm (Fixed {0})
    { 917.0, 648.0, 2599.3815397928, 1838.0403136786 },
    // "c2": 458.5020216023mm x 648.4197773255mm == 1299.6907698964pt x 1838.0403136786pt
    // AutoRounded: 458mm x 648mm (Fixed {0})
    { 458.0, 648.0, 1299.6907698964, 1838.0403136786 },
    // "c2 (Landscape)": 648.4197773255mm x 458.5020216023mm == 1838.0403136786pt x 1299.6907698964pt
    // AutoRounded: 648mm x 458mm (Fixed {0})
    { 648.0, 458.0, 1838.0403136786, 1299.6907698964 },
    // "c3": 324.2098886628mm x 458.5020216023mm == 919.0201568393pt x 1299.6907698964pt
    // AutoRounded: 324mm x 458mm (Fixed {0})
    { 324.0, 458.0, 919.0201568393, 1299.6907698964 },
    // "c3 (Landscape)": 458.5020216023mm x 324.2098886628mm == 1299.6907698964pt x 919.0201568393pt
    // AutoRounded: 458mm x 324mm (Fixed {0})
    { 458.0, 324.0, 1299.6907698964, 919.0201568393 },
    // "c4": 229.2510108012mm x 324.2098886628mm == 649.8453849482pt x 919.0201568393pt
    // AutoRounded: 229mm x 324mm (Fixed {0})
    { 229.0, 324.0, 649.8453849482, 919.0201568393 },
    // "c4 (Landscape)": 324.2098886628mm x 229.2510108012mm == 919.0201568393pt x 649.8453849482pt
    // AutoRounded: 324mm x 229mm (Fixed {0})
    { 324.0, 229.0, 919.0201568393, 649.8453849482 },
    // "c5": 162.1049443314mm x 229.2510108012mm == 459.5100784196pt x 649.8453849482pt
    // AutoRounded: 162mm x 229mm (Fixed {0})
    { 162.0, 229.0, 459.5100784196, 649.8453849482 },
    // "c5 (Landscape)": 229.2510108012mm x 162.1049443314mm == 649.8453849482pt x 459.5100784196pt
    // AutoRounded: 229mm x 162mm (Fixed {0})
    { 229.0, 162.0, 649.8453849482, 459.5100784196 },
    // "c6": 114.6255054006mm x 162.1049443314mm == 324.9226924741pt x 459.5100784196pt
    // AutoRounded: 114mm x 162mm (Fixed {0})
    { 114.0, 162.0, 324.9226924741, 459.5100784196 },
    // "c6 (Landscape)": 162.1049443314mm x 114.6255054006mm == 459.5100784196pt x 324.9226924741pt
    // AutoRounded: 162mm x 114mm (Fixed {0})
    { 162.0, 114.0, 459.5100784196, 324.9226924741 },
    // "c7": 81.0524721657mm x 114.6255054006mm == 229.7550392098pt x 324.9226924741pt
    // AutoRounded: 81mm x 114mm (Fixed {0})
    { 81.0, 114.0, 229.7550392098, 324.9226924741 },
    // "c7 (Landscape)": 114.6255054006mm x 81.0524721657mm == 324.9226924741pt x 229.7550392098pt
    // AutoRounded: 114mm x 81mm (Fixed {0})
    { 114.0, 81.0, 324.9226924741, 229.7550392098 },
    // "c8": 57.3127527003mm x 81.0524721657mm == 162.4613462370pt x 229.7550392098pt
    // AutoRounded: 57mm x 81mm (Fixed {0})
    { 57.0, 81.0, 162.4613462370, 229.7550392098 },
    // "c8 (Landscape)": 81.0524721657mm x 57.3127527003mm == 229.7550392098pt x 162.4613462370pt
    // AutoRounded: 81mm x 57mm (Fixed {0})
    { 81.0, 57.0, 229.7550392098, 162.4613462370 },
    // "c9": 40.5262360828mm x 57.3127527003mm == 114.8775196049pt x 162.4613462370pt
    // AutoRounded: 40mm x 57mm (Fixed {0})
    { 40.0, 57.0, 114.8775196049, 162.4613462370 },
    // "c9 (Landscape)": 57.3127527003mm x 40.5262360828mm == 162.4613462370pt x 114.8775196049pt
    // AutoRounded: 57mm x 40mm (Fixed {0})
    { 57.0, 40.0, 162.4613462370, 114.8775196049 },
    // "c10": 28.6563763501mm x 40.5262360828mm == 81.2306731185pt x 114.8775196049pt
    // AutoRounded: 28mm x 40mm (Fixed {0})
    { 28.0, 40.0, 81.2306731185, 114.8775196049 },
    // "c10 (Landscape)": 40.5262360828mm x 28.6563763501mm == 114.8775196049pt x 81.2306731185pt
    // AutoRounded: 40mm x 28mm (Fixed {0})
    { 40.0, 28.0, 114.8775196049, 81.2306731185 },
    // "e0": 1090.5077326653mm x 1542.2108254079mm == 3091.2030217283pt x 4371.6212373768pt
    // AutoRounded: 1090mm x 1542mm (Fixed {0})
    { 1090.0, 1542.0, 3091.2030217283, 4371.6212373768 },
    // "e0 (Landscape)": 1542.2108254079mm x 1090.5077326653mm == 4371.6212373768pt x 3091.2030217283pt
    // AutoRounded: 1542mm x 1090mm (Fixed {0})
    { 1542.0, 1090.0, 4371.6212373768, 3091.2030217283 },
    // "e1": 771.1054127040mm x 1090.5077326653mm == 2185.8106186884pt x 3091.2030217283pt
    // AutoRounded: 771mm x 1090mm (Fixed {0})
    { 771.0, 1090.0, 2185.8106186884, 3091.2030217283 }, // ISO D0
    // "e1 (Landscape)": 1090.5077326653mm x 771.1054127040mm == 3091.2030217283pt x 2185.8106186884pt
    // AutoRounded: 1090mm x 771mm (Fixed {0})
    { 1090.0, 771.0, 3091.2030217283, 2185.8106186884 },
    // "e2": 545.2538663326mm x 771.1054127040mm == 1545.6015108641pt x 2185.8106186884pt
    // AutoRounded: 545mm x 771mm (Fixed {0})
    { 545.0, 771.0, 1545.6015108641, 2185.8106186884 },
    // "e2 (Landscape)": 771.1054127040mm x 545.2538663326mm == 2185.8106186884pt x 1545.6015108641pt
    // AutoRounded: 771mm x 545mm (Fixed {0})
    { 771.0, 545.0, 2185.8106186884, 1545.6015108641 },
    // "e3": 385.5527063520mm x 545.2538663326mm == 1092.9053093442pt x 1545.6015108641pt
    // AutoRounded: 385mm x 545mm (Fixed {0})
    { 385.0, 545.0, 1092.9053093442, 1545.6015108641 },
    // "e3 (Landscape)": 545.2538663326mm x 385.5527063520mm == 1545.6015108641pt x 1092.9053093442pt
    // AutoRounded: 545mm x 385mm (Fixed {0})
    { 545.0, 385.0, 1545.6015108641, 1092.9053093442 },
    // "e4": 272.6269331663mm x 385.5527063520mm == 772.8007554321pt x 1092.9053093442pt
    // AutoRounded: 272mm x 385mm (Fixed {0})
    { 272.0, 385.0, 772.8007554321, 1092.9053093442 },
    // "e4 (Landscape)": 385.5527063520mm x 272.6269331663mm == 1092.9053093442pt x 772.8007554321pt
    // AutoRounded: 385mm x 272mm (Fixed {0})
    { 385.0, 272.0, 1092.9053093442, 772.8007554321 },
    // "e5": 192.7763531760mm x 272.6269331663mm == 546.4526546721pt x 772.8007554321pt
    // AutoRounded: 192mm x 272mm (Fixed {0})
    { 192.0, 272.0, 546.4526546721, 772.8007554321 },
    // "e5 (Landscape)": 272.6269331663mm x 192.7763531760mm == 772.8007554321pt x 546.4526546721pt
    // AutoRounded: 272mm x 192mm (Fixed {0})
    { 272.0, 192.0, 772.8007554321, 546.4526546721 },
    // "e6": 136.3134665832mm x 192.7763531760mm == 386.4003777160pt x 546.4526546721pt
    // AutoRounded: 136mm x 192mm (Fixed {0})
    { 136.0, 192.0, 386.4003777160, 546.4526546721 },
    // "e6 (Landscape)": 192.7763531760mm x 136.3134665832mm == 546.4526546721pt x 386.4003777160pt
    // AutoRounded: 192mm x 136mm (Fixed {0})
    { 192.0, 136.0, 546.4526546721, 386.4003777160 },
    // "e7": 96.3881765880mm x 136.3134665832mm == 273.2263273361pt x 386.4003777160pt
    // AutoRounded: 96mm x 136mm (Fixed {0})
    { 96.0, 136.0, 273.2263273361, 386.4003777160 },
    // "e7 (Landscape)": 136.3134665832mm x 96.3881765880mm == 386.4003777160pt x 273.2263273361pt
    // AutoRounded: 136mm x 96mm (Fixed {0})
    { 136.0, 96.0, 386.4003777160, 273.2263273361 },
    // "e8": 68.1567332916mm x 96.3881765880mm == 193.2001888580pt x 273.2263273361pt
    // AutoRounded: 68mm x 96mm (Fixed {0})
    { 68.0, 96.0, 193.2001888580, 273.2263273361 },
    // "e8 (Landscape)": 96.3881765880mm x 68.1567332916mm == 273.2263273361pt x 193.2001888580pt
    // AutoRounded: 96mm x 68mm (Fixed {0})
    { 96.0, 68.0, 273.2263273361, 193.2001888580 },
    // "e9": 48.1940882940mm x 68.1567332916mm == 136.6131636680pt x 193.2001888580pt
    // AutoRounded: 48mm x 68mm (Fixed {0})
    { 48.0, 68.0, 136.6131636680, 193.2001888580 },
    // "e9 (Landscape)": 68.1567332916mm x 48.1940882940mm == 193.2001888580pt x 136.6131636680pt
    // AutoRounded: 68mm x 48mm (Fixed {0})
    { 68.0, 48.0, 193.2001888580, 136.6131636680 },
    // "e10": 34.0783666458mm x 48.1940882940mm == 96.6000944290pt x 136.6131636680pt
    // AutoRounded: 34mm x 48mm (Fixed {0})
    { 34.0, 48.0, 96.6000944290, 136.6131636680 },
    // "e10 (Landscape)": 48.1940882940mm x 34.0783666458mm == 136.6131636680pt x 96.6000944290pt
    // AutoRounded: 48mm x 34mm (Fixed {0})
    { 48.0, 34.0, 136.6131636680, 96.6000944290 },
    // "dl": 220.0000000000mm x 110.0000000000mm == 623.6220472441pt x 311.8110236220pt
    // AutoRound disabled
    { 220.000000, 110.000000, 623.6220472441, 311.8110236220 },
    // "dl (Landscape)": 110.0000000000mm x 220.0000000000mm == 311.8110236220pt x 623.6220472441pt
    // AutoRound disabled
    { 110.000000, 220.000000, 311.8110236220, 623.6220472441 },
    // "m65": 162.1049443314mm x 162.1049443314mm == 459.5100784196pt x 459.5100784196pt
    // AutoRounded: 162mm x 162mm (Fixed {0})
    { 162.0, 162.0, 459.5100784196, 459.5100784196 },
    // "m65 (Landscape)": 162.1049443314mm x 162.1049443314mm == 459.5100784196pt x 459.5100784196pt
    // AutoRounded: 162mm x 162mm (Fixed {0})
    { 162.0, 162.0, 459.5100784196, 459.5100784196 },
    // "ledger": 355.6000000000mm x 431.8000000000mm == 1008.0000000000pt x 1224.0000000000pt
    // AutoRound disabled
    //{ 355.600000, 431.800000, 1008.0000000000, 1224.0000000000 },
    { 279.400000, 431.800000, 792.0000000000, 1224.0000000000 },
    // "ledger (Landscape)": 431.8000000000mm x 355.6000000000mm == 1224.0000000000pt x 1008.0000000000pt
    // AutoRound disabled
    //{ 431.800000, 355.600000, 1224.0000000000, 1008.0000000000 },
    { 431.800000, 279.400000, 1224.0000000000, 792.0000000000 },
    // "tabloid": 355.6000000000mm x 431.8000000000mm == 1008.0000000000pt x 1224.0000000000pt
    // AutoRound disabled
    //{ 355.600000, 431.800000, 1008.0000000000, 1224.0000000000 },
    { 279.400000, 431.800000, 792.0000000000, 1224.0000000000 },
    // "tabloid (Landscape)": 431.8000000000mm x 355.6000000000mm == 1224.0000000000pt x 1008.0000000000pt
    // AutoRound disabled
    //{ 431.800000, 355.600000, 1224.0000000000, 1008.0000000000 },
    { 431.800000, 279.400000, 1224.0000000000, 792.0000000000 },
    // "legal": 215.9000000000mm x 305.3287081164mm == 612.0000000000pt x 865.4987001723pt
    // AutoRound disabled
    //{ 215.900000, 305.328708, 612.0000000000, 865.4987001723 },
    { 215.900000, 355.600000, 612.0000000000, 1008.0000000000 },
    // "legal (Landscape)": 305.3287081164mm x 215.9000000000mm == 865.4987001723pt x 612.0000000000pt
    // AutoRound disabled
    //{ 305.328708, 215.900000, 865.4987001723, 612.0000000000 },
    { 355.600000, 215.900000, 1008.0000000000, 612.0000000000 },
    // "letter": 215.9000000000mm x 279.4000000000mm == 612.0000000000pt x 792.0000000000pt
    // AutoRound disabled
    { 215.900000, 279.400000, 612.0000000000, 792.0000000000 },
    // "letter (Landscape)": 279.4000000000mm x 215.9000000000mm == 792.0000000000pt x 612.0000000000pt
    // AutoRound disabled
    { 279.400000, 215.900000, 792.0000000000, 612.0000000000 }
  }; // <\cps>
  OdUInt32 maxTests = sizeof(cps) / sizeof(CheckPS);
  double pw = params.getPaperWidth(), ph = params.getPaperHeight();
  for (OdUInt32 i = 0; i < maxTests; i++)
  {
    if (cps[i].mmWidth == pw && cps[i].mmHeight == ph) // Maybe use tol?
    {
      fFixWidth = cps[i].ptWidth;
      fFixHeight = cps[i].ptHeight;
      return true;
    }
  }
  return false;
}

static void odPdfFixPaperSizeApply(bool bApply, OdGsPageParams &params, double fFixWidth, double fFixHeight)
{
  if (bApply)
  {
    params.set(fFixWidth, fFixHeight, params.getLeftMargin(), params.getRightMargin(), params.getTopMargin(), params.getBottomMargin());
  }
}
#endif //SRC-895 PDF export creates imprecise dimensions of predefined standard paper sizes

PDFResultEx CPdfExportImpl::init(const PDFExportParams &pParams)
{
  PDFResultEx res = m_ParamsHolder.init(pParams);

  m_ParamsHolder.document().enableEncoding(GETBIT(m_ParamsHolder.getParams().exportFlags(), PDFExportParams::kFlateCompression));
  m_ParamsHolder.document().enableEncodingASCIIHEX(GETBIT(m_ParamsHolder.getParams().exportFlags(), PDFExportParams::kASCIIHexEncoding));
  m_ParamsHolder.document().enableEncodingDCT(m_ParamsHolder.getParams().dctCompression());
  m_ParamsHolder.document().setPdfA(m_ParamsHolder.getParams().archived() == PDFExportParams::kPDFA_None ? false : true);
  m_ParamsHolder.document().setTmpStream(m_ParamsHolder.getParams().output()); // Can be used for encoding as temp storage.
  m_ParamsHolder.document().setLinearized(GETBIT(m_ParamsHolder.getParams().exportFlags(), PDFExportParams::kLinearized));

  return res;
}

PDFResultEx CPdfExportImpl::run()
{
  PDFAUX::createBasePDF4DWG(m_ParamsHolder.document(), m_ParamsHolder.getParams().pageParams(), m_ParamsHolder.getParams());
  // fill pdf document information
  PDFAUX::createDocumentInformation(m_ParamsHolder.document(), m_ParamsHolder.getParams());

  if (m_ParamsHolder.document().isPdfA())
  {
    PDFAUX::CreateOutputIntent(m_ParamsHolder.document());
  }

  // fill and set to document resource dictionary
  PDFDictionaryPtr pResDict(PDFAUX::createResDictionary4DWG(m_ParamsHolder.document()));

  PDFPageNodeDictionaryPtr pPageTree(m_ParamsHolder.document().Root()->getPages());
  pPageTree->setResources(pResDict);

  m_FontOptimizer.clear(GETBIT(m_ParamsHolder.getParams().exportFlags(), PDFExportParams::kEmbededOptimizedTTF), GETBIT(m_ParamsHolder.getParams().exportFlags(), PDFExportParams::kEmbededTTF));
  m_Type3Optimizer.clear();

  PDFResultEx result = base_run(m_ParamsHolder, false);
  if (::exOk != result)
    return result;

  m_FontOptimizer.Optimize();
  m_Type3Optimizer.Optimize();

  return ::exOk;

}

PDFResultEx CPdfExportImpl::exportFile()
{
  // export PDF object tree to output stream
  PDFIStreamPtr outputStream = PDFIStream::createObject();
  OdStreamBufPtr linearizedOutput;
  if (GETBIT(m_ParamsHolder.getParams().exportFlags(), PDFExportParams::kLinearized) == 0)
    outputStream->setStreamBuf(m_ParamsHolder.getParams().output());
  else
  {
    linearizedOutput = OdMemoryStream::createNew();
    outputStream->setStreamBuf(linearizedOutput);
  }
  m_ParamsHolder.document().Export(outputStream, ver2ver(m_ParamsHolder.getParams().version()) );

  if (GETBIT(m_ParamsHolder.getParams().exportFlags(), PDFExportParams::kLinearized))
  {
    OdStreamBufPtr output = m_ParamsHolder.getParams().output();
    PDFLinearizer::linearize(linearizedOutput, output);
  }

  return ::exOk;
}
}
