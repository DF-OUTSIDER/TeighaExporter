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

/////////////////////////////////////////////////////////////////////
//
// PdfAux.cpp
//
//////////////////////////////////////////////////////////////////////

#include "PdfExportCommon.h"
#include "PdfAux.h"
#include "OdPlatform.h"
#include "Pdf2dExportDevice.h"

#include "PdfDocument.h"
#include "PdfCatalogDictionary.h"
#include "PdfPageNodeDictionary.h"
#include "PdfPageDictionary.h"
#include "PdfOutputIntentsDictionary.h"
#include "PdfContentStream.h"
#include "PdfRGBStream.h"
#include "PdfExportParams.h"
#include "PdfDocumentInformation.h"
#include "PdfExportVersion.h"
#include "PdfToolkitVersion.h"
#include "PdfTempFileStream.h"
#include "PdfMetadataStream.h"
#include "PdfICCBasedStream.h"
#include "PdfViewportDictionary.h"
#include "PdfMeasureDictionary.h"
#include "PdfNumberFormatDictionary.h"

#include "MemoryStream.h"
#include "OdCharMapper.h"
#include "Gi/GiRasterWrappers.h"
#include "RxObjectImpl.h"

namespace TD_PDF_2D_EXPORT {

void PDFAUX::createBasePDF4DWG(PDFDocument &PDFDoc, const OdArray<OdGsPageParams> &pageParams, const PDFExportParams &pParams)
{
  PDFPageNodeDictionaryPtr pRootPageNode = PDFPageNodeDictionary::createObject(PDFDoc, true);
  
  for(OdUInt32 f=0; f<pageParams.size(); ++f)
  {
    PDFPageDictionaryPtr pCurPage( PDFPageDictionary::createObject(PDFDoc, true) );
  
    PDFRectanglePtr pRect( PDFRectangle::createObject(PDFDoc) );
    pRect->set(0, 0, OdRound(pageParams[f].getPaperWidth()), OdRound(pageParams[f].getPaperHeight()));
    pCurPage->setMediaBox(pRect);

    PDFContentStreamPtr pCStream = PDFContentStream::createObject(PDFDoc, true);
    PDFArrayPtr pContentArray = PDFArray::createObject(PDFDoc);
    pContentArray->append(pCStream);

    pCurPage->setContents(pContentArray);

    if (pParams.archived() == PDFExportParams::kPDFA_2b)
    {
      pCStream->CS(PDFName::createObject(PDFDoc, ("DeviceRGB")));
      pCStream->cs(PDFName::createObject(PDFDoc, ("DeviceRGB")));

      PDFResourceDictionaryPtr pRes = createResDictionary4DWG(PDFDoc);
      pCurPage->setResources(pRes);
    }

    pRootPageNode->AddKids(pCurPage);
  }

  PDFCatalogDictionaryPtr pCatalog = PDFCatalogDictionary::createObject(PDFDoc, true); PDFDoc.setRoot(pCatalog);
  pCatalog->setPages( pRootPageNode );
}

void PDFAUX::createMeasuringViewPort(const OdGsDCRect& bbox, PDFDocument &PDFDoc, PDFPageDictionary* pPage, double iMeasureScale)
{
  PDFArrayPtr pVPArray = PDFArray::createObject(PDFDoc);
  PDFViewportDictionaryPtr pVP(PDFViewportDictionary::createObject(PDFDoc));

  PDFRectanglePtr rect = PDFRectangle::createObject(PDFDoc);
  rect->set(bbox.m_min.x, bbox.m_min.y, bbox.m_max.x, bbox.m_max.y);
  pVP->setBBox(rect);

  PDFMeasureDictionaryPtr pMeasure = PDFMeasureDictionary::createObject(PDFDoc);
  pMeasure->setSubtype(PDFName::createObject(PDFDoc, "RL"));
  PDFTextStringPtr pEmptyStr = PDFTextString::createObject(PDFDoc, L"", false);
  pMeasure->setR(pEmptyStr);

  PDFArrayPtr pADArray = PDFArray::createObject(PDFDoc);
  PDFArrayPtr pXArray = PDFArray::createObject(PDFDoc);
  PDFNumberFormatDictionaryPtr pNumF1 = PDFNumberFormatDictionary::createObject(PDFDoc);
  PDFNumberFormatDictionaryPtr pNumF2 = PDFNumberFormatDictionary::createObject(PDFDoc);
  pNumF1->setU(pEmptyStr);
  pNumF1->setC(PDFNumber::createObject(PDFDoc, 1, false));
  pNumF2->setU(pEmptyStr);
  pNumF2->setC(PDFNumber::createObject(PDFDoc, iMeasureScale, false));
  pADArray->push_back(pNumF1);
  pXArray->push_back(pNumF2);

  pMeasure->setA(pADArray);
  pMeasure->setD(pADArray);
  pMeasure->setX(pXArray);
  pVP->setMeasure(pMeasure);
  pVPArray->append(pVP);
  pPage->setVP(pVPArray);
}

void PDFAUX::updateContentStreamWithCM(PDFDocument &PDFDoc, PDFPageDictionary* pPage, const PDFExportParams &pParams)
{
  if (!pPage || pParams.export2XObject()) //recording disabled
  {
    return;
  }

  PDFArrayPtr pContent = pPage->getContents();
  if (pContent->size() == 1) //now we can have only 1 CS for page
  {
    PDFContentStreamPtr pCStream = PDFContentStream::createObject(PDFDoc, true);

    OdGeMatrix2d m;
    OdUInt16 geomDPI = pParams.getGeomDPI();
    m.setToScaling(72. / (double)geomDPI);

    pCStream->cm(m.entry[0][0], m.entry[1][0], m.entry[0][1], m.entry[1][1], m.entry[0][2], m.entry[1][2]);
    pContent->insert(pContent->begin(), pCStream);
  }
}

void PDFAUX::createDocumentInformation(PDFDocument &PDFDoc, const PDFExportParams &pParams)
{
  PDFDocumentInformationPtr pDI = PDFDocumentInformation::createObject(PDFDoc, true);
  PDFMetadataStreamPtr pMD;
  if (PDFDoc.isPdfA())
    pMD = PDFMetadataStream::createObject(PDFDoc, true);

  if (!pParams.title().isEmpty())
  {
    PDFTextStringPtr pTitle = PDFTextString::createObject(PDFDoc, false);
    TD_PDF_HELPER_FUNCS::getUnicodeTextString(pParams.title(), pTitle);
    pTitle->enableFixParenthesis(); // fix '(' ')' '\' -> '\(' '\)' '\\'
    pDI->setTitle(pTitle);
    if (!pMD.isNull())
      pMD->setTitle(pParams.title());
  }

  if (!pParams.author().isEmpty())
  {
    PDFTextStringPtr pAuthor = PDFTextString::createObject(PDFDoc, false);
    TD_PDF_HELPER_FUNCS::getUnicodeTextString(pParams.author(), pAuthor);
    pAuthor->enableFixParenthesis(); // fix '(' ')' '\' -> '\(' '\)' '\\'
    pDI->setAuthor(pAuthor);
    if (!pMD.isNull())
      pMD->setAuthor(pParams.author());
  }

  if (!pParams.subject().isEmpty())
  {
    PDFTextStringPtr pSubject = PDFTextString::createObject(PDFDoc, false);
    TD_PDF_HELPER_FUNCS::getUnicodeTextString(pParams.subject(), pSubject);
    pSubject->enableFixParenthesis(); // fix '(' ')' '\' -> '\(' '\)' '\\'
    pDI->setSubject(pSubject);
    if (!pMD.isNull())
      pMD->setSubject(pParams.subject());
  }

  if (!pParams.keywords().isEmpty())
  {
    PDFTextStringPtr pKeywords = PDFTextString::createObject(PDFDoc, false);
    TD_PDF_HELPER_FUNCS::getUnicodeTextString(pParams.keywords(), pKeywords);
    pKeywords->enableFixParenthesis(); // fix '(' ')' '\' -> '\(' '\)' '\\'
    pDI->setKeywords(pKeywords);
    if (!pMD.isNull())
      pMD->setKeywords(pParams.keywords());
  }

  if (!pParams.creator().isEmpty())
  {
    PDFTextStringPtr pCreator = PDFTextString::createObject(PDFDoc, false);
    TD_PDF_HELPER_FUNCS::getUnicodeTextString(pParams.creator(), pCreator);
    pCreator->enableFixParenthesis(); // fix '(' ')' '\' -> '\(' '\)' '\\'
    pDI->setCreator(pCreator);
    if (!pMD.isNull())
      pMD->setCreator(pParams.creator());
  }

  if (!pParams.producer().isEmpty())
  {
    PDFTextStringPtr pProducer = PDFTextString::createObject(PDFDoc, false);
    TD_PDF_HELPER_FUNCS::getUnicodeTextString(pParams.producer(), pProducer);
    pProducer->enableFixParenthesis(); // fix '(' ')' '\' -> '\(' '\)' '\\'
    pDI->setProducer(pProducer);
    if (!pMD.isNull())
      pMD->setProducer(pParams.producer());
  }
  else
  {
    OdAnsiString str;
    str.format(("ODA PDF Export v%d.%d.%d.%d (v%d.%d.%d.%d)"), PDF_EXPORT_MAJOR_VERSION, PDF_EXPORT_MINOR_VERSION, PDF_EXPORT_MAJOR_BUILD_VERSION, PDF_EXPORT_MINOR_BUILD_VERSION, PDF_TOOLKIT_MAJOR_VERSION, PDF_TOOLKIT_MINOR_VERSION, PDF_TOOLKIT_MAJOR_BUILD_VERSION, PDF_TOOLKIT_MINOR_BUILD_VERSION);
    pDI->setProducer(PDFTextString::createObject(PDFDoc, str, false));
    if (!pMD.isNull())
      pMD->setProducer(str);
  }

  PDFDatePtr pCreationDate = PDFDate::createObject(PDFDoc);
  OdTimeStamp curDate(OdTimeStamp::kInitUniversalTime);
  pCreationDate->set(curDate, 0, 0);

  pDI->setCreationDate(pCreationDate);
  if (!pMD.isNull())
  {
    pMD->setCreationDate(curDate);

    pMD->setPdfALevelConf((OdUInt16)pParams.archived(), 2);//conformance B is hardcoded
    PDFCatalogDictionaryPtr pCatalog = PDFDoc.Root();
    pCatalog->setMetadata(pMD);
  }

  PDFDoc.setDocumentInformation(pDI);
}

void PDFAUX::CreateOutputIntent(PDFDocument &PDFDoc)
{
  PDFCatalogDictionaryPtr pCatalog = PDFDoc.Root();

  PDFArrayPtr pIntents = pCatalog->Find("OutputIntents");
  if (pIntents.isNull())
  {
    PDFOutputIntentsDictionaryPtr pIntent = PDFOutputIntentsDictionary::createObject(PDFDoc);
    PDFICCBasedStreamPtr pDestOutProfile = PDFICCBasedStream::createObject(PDFDoc, true);
    pIntent->setDestOutputProfile(pDestOutProfile);

    pIntents = PDFArray::createObject(PDFDoc);
    pIntents->append(pIntent);
    pCatalog->setOutputIntents(pIntents);
  }
}

PDFIndexedRGBColorSpacePtr PDFAUX::CreateIndexedRGBColorSpace(const ODCOLORREF *pRGB, OdUInt32 num, PDFDocument &PDFDoc)
{
  PDFIndexedRGBColorSpacePtr pIndexed = PDFIndexedRGBColorSpace::createObject(PDFDoc, true);

  PDFRGBStreamPtr pRGBStream( pIndexed->getLookup() );

  for(OdUInt32 f=0; f<num; ++f, ++pRGB)
  {
    pRGBStream->addRGB(ODGETRED(*pRGB), ODGETGREEN(*pRGB), ODGETBLUE(*pRGB));
  }

  return pIndexed;
}

PDFIndexedRGBColorSpacePtr PDFAUX::CreateIndexedRGBColorSpace(const OdUInt8 *pRGB, OdUInt32 num, PDFDocument &PDFDoc)
{
  PDFIndexedRGBColorSpacePtr pIndexed = PDFIndexedRGBColorSpace::createObject(PDFDoc, true);

  PDFRGBStreamPtr(pIndexed->getLookup())->addRGB(pRGB, num);

  return pIndexed;
}

PDFResourceDictionaryPtr PDFAUX::createResDictionary4DWG(PDFDocument &PDFDoc)
{
  PDFResourceDictionaryPtr pResDic = PDFResourceDictionary::createObject(PDFDoc, true);
  return pResDic;
}

void PDFAUX::createIndexedDWGPalette(PDFResourceDictionaryPtr pResDic, const ODCOLORREF *pRGB, OdUInt32 num, PDFDocument &PDFDoc)
{
  PDFSubDictionaryPtr pSubColorDict = pResDic->getColorSpace();
  PDFIndexedRGBColorSpacePtr pIndexed = CreateIndexedRGBColorSpace(pRGB, num, PDFDoc);
  pSubColorDict->AddItem(("DWGPalette"), pIndexed );
}

class OdGiRasterImageMaskWrapper : public OdGiRasterImageWrapper
{
public:
  virtual OdGiRasterImage::PixelFormatInfo pixelFormat() const
  {
    OdGiRasterImage::PixelFormatInfo pf = OdGiRasterImageWrapper::pixelFormat();
    return pf;
  }

  virtual OdUInt32 scanLinesAlignment() const 
  { 
    return 1; 
  }
};

}
