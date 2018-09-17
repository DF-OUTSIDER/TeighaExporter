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


#if defined(_MSC_VER)
// warning C4290: C++ Exception Specification ignored
#pragma warning ( disable : 4290 )
#if _MSC_VER <= 1200
// compiler stack overflows if these warning is enabled
#pragma warning ( disable : 4786)
#endif
#endif

#include "OdaCommon.h"
#include "RxDispatchImpl.h"
#include "RxVariantValue.h"
#include "DbDictionary.h"
#include "RxDynamicModule.h"
#include "Ed/EdCommandStack.h"
#include "Ed/EdCommandContext.h"
#include "Ed/EdUserIO.h"
#include "DbCommandContext.h"
#include "StaticRxObject.h"
#include "PdfProperties.h"
#include "PdfImportImpl.h"
#include "DbDatabase.h"
#include "DbHostAppServices.h"
#include "DbBlockTableRecord.h"
#include "DbUnderlayReference.h"
#include "DbUnderlayDefinition.h"
#include "Ge/GeScale3d.h"
#include "OdPath.h"

#include "DbDeviceDriver.h"
#include "PdfImportFunctions.h"

#include "core/fpdfapi/cpdf_modulemgr.h"
#include "core/fpdfapi/cpdf_pagerendercontext.h"
#include "core/fpdfapi/page/cpdf_image.h"
#include "core/fpdfapi/page/cpdf_imageobject.h"
#include "core/fpdfapi/page/cpdf_page.h"
#include "core/fpdfapi/page/cpdf_pageobject.h"
#include "core/fpdfapi/page/cpdf_pathobject.h"
#include "core/fpdfapi/parser/cpdf_array.h"
#include "core/fpdfapi/parser/cpdf_dictionary.h"
#include "core/fpdfapi/parser/cpdf_document.h"
#include "core/fpdfapi/parser/fpdf_parser_decode.h"
#include "core/fpdfapi/parser/cpdf_array.h"
#include "core/fpdfapi/parser/cpdf_reference.h"
#include "core/fpdfapi/render/cpdf_progressiverenderer.h"
#include "core/fpdfapi/render/cpdf_renderoptions.h"
#include "core/fpdfdoc/cpdf_annotlist.h"
#include "core/fpdfdoc/cpdf_nametree.h"
#include "core/fpdfdoc/cpdf_occontext.h"
#include "core/fpdfdoc/cpdf_viewerpreferences.h"
#include "core/fxcrt/fx_memory.h"
#include "core/fxcrt/fx_safe_types.h"
#include "core/fxcrt/fx_stream.h"
#include "core/fxge/cfx_defaultrenderdevice.h"
#include "core/fxge/cfx_gemodule.h"
#include "fpdfsdk/cpdfsdk_formfillenvironment.h"
#include "fpdfsdk/cpdfsdk_pageview.h"
#include "fpdfsdk/fsdk_define.h"
#include "fpdfsdk/fsdk_pauseadapter.h"
#include "fxjs/ijs_runtime.h"
#include "public/fpdf_edit.h"
#include "public/fpdf_ext.h"
#include "public/fpdf_formfill.h"
#include "public/fpdf_progressive.h"
#include "third_party/base/allocator/partition_allocator/partition_alloc.h"
#include "third_party/base/numerics/safe_conversions_impl.h"
#include "third_party/base/ptr_util.h"


OdRxDictionaryPtr PdfProperties::createObject()
{
  return OdRxObjectImpl<PdfProperties, OdRxDictionary>::createObject();
}


ODRX_DECLARE_PROPERTY(PdfPath)
ODRX_DECLARE_PROPERTY(Password)
ODRX_DECLARE_PROPERTY(Database)
ODRX_DECLARE_PROPERTY(PageNumber)
ODRX_DECLARE_PROPERTY(LayersUseType)
ODRX_DECLARE_PROPERTY(ImportVectorGeometry)
ODRX_DECLARE_PROPERTY(ImportSolidFills)
ODRX_DECLARE_PROPERTY(ImportTrueTypeText)
ODRX_DECLARE_PROPERTY(ImportRasterImages)
ODRX_DECLARE_PROPERTY(ImportAsBlock)
ODRX_DECLARE_PROPERTY(JoinLineAndArcSegments)
ODRX_DECLARE_PROPERTY(ConvertSolidsToHatches)
ODRX_DECLARE_PROPERTY(ApplyLineweight)
ODRX_DECLARE_PROPERTY(InsertionPointX)
ODRX_DECLARE_PROPERTY(InsertionPointY)
ODRX_DECLARE_PROPERTY(Scaling)
ODRX_DECLARE_PROPERTY(Rotation)
ODRX_DECLARE_PROPERTY(ImportTrueTypeTextAsGeometry)
ODRX_DECLARE_PROPERTY(ImportGradientFills)


ODRX_DEFINE_PROPERTY(PdfPath, PdfProperties, getString)
ODRX_DEFINE_PROPERTY(Password, PdfProperties, getString)
ODRX_DEFINE_PROPERTY(PageNumber, PdfProperties, getInt32)
ODRX_DEFINE_PROPERTY_OBJECT(Database, PdfProperties,  get_Database, put_Database, OdDbDatabase)
ODRX_DEFINE_PROPERTY(LayersUseType, PdfProperties, getUInt8)
ODRX_DEFINE_PROPERTY(ImportVectorGeometry, PdfProperties, getBool)
ODRX_DEFINE_PROPERTY(ImportSolidFills, PdfProperties, getBool)
ODRX_DEFINE_PROPERTY(ImportTrueTypeText, PdfProperties, getBool)
ODRX_DEFINE_PROPERTY(ImportRasterImages, PdfProperties, getBool)
ODRX_DEFINE_PROPERTY(ImportAsBlock, PdfProperties, getBool)
ODRX_DEFINE_PROPERTY(JoinLineAndArcSegments, PdfProperties, getBool)
ODRX_DEFINE_PROPERTY(ConvertSolidsToHatches, PdfProperties, getBool)
ODRX_DEFINE_PROPERTY(ApplyLineweight, PdfProperties, getBool)
ODRX_DEFINE_PROPERTY(InsertionPointX, PdfProperties, getDouble)
ODRX_DEFINE_PROPERTY(InsertionPointY, PdfProperties, getDouble)
ODRX_DEFINE_PROPERTY(Scaling, PdfProperties, getDouble)
ODRX_DEFINE_PROPERTY(Rotation, PdfProperties, getDouble)
ODRX_DEFINE_PROPERTY(ImportTrueTypeTextAsGeometry, PdfProperties, getBool)
ODRX_DEFINE_PROPERTY(ImportGradientFills, PdfProperties, getBool)

ODRX_BEGIN_DYNAMIC_PROPERTY_MAP( PdfProperties );
  ODRX_GENERATE_PROPERTY( PdfPath )
  ODRX_GENERATE_PROPERTY( Password )
  ODRX_GENERATE_PROPERTY( Database )
  ODRX_GENERATE_PROPERTY(PageNumber)
  ODRX_GENERATE_PROPERTY(LayersUseType)
  ODRX_GENERATE_PROPERTY(ImportVectorGeometry)
  ODRX_GENERATE_PROPERTY(ImportSolidFills)
  ODRX_GENERATE_PROPERTY(ImportTrueTypeText)
  ODRX_GENERATE_PROPERTY(ImportRasterImages)
  ODRX_GENERATE_PROPERTY(ImportAsBlock)
  ODRX_GENERATE_PROPERTY(JoinLineAndArcSegments)
  ODRX_GENERATE_PROPERTY(ConvertSolidsToHatches)
  ODRX_GENERATE_PROPERTY(ApplyLineweight)
  ODRX_GENERATE_PROPERTY(InsertionPointX)
  ODRX_GENERATE_PROPERTY(InsertionPointY)
  ODRX_GENERATE_PROPERTY(Scaling)
  ODRX_GENERATE_PROPERTY(Rotation)
  ODRX_GENERATE_PROPERTY(ImportTrueTypeTextAsGeometry)
  ODRX_GENERATE_PROPERTY(ImportGradientFills)
ODRX_END_DYNAMIC_PROPERTY_MAP( PdfProperties );

#if defined(_MSC_VER) && (_MSC_VER >= 1300)
#pragma warning(disable:4355)
#endif

PdfImporter::PdfImporter() 
  : m_pProperties( PdfProperties::createObject() )
  , m_pDocument(NULL)
  , m_Page(NULL)
  , m_PageNum(-1)
  , m_isClipBoundaryInverted(false)
{
}

PdfImporter::~PdfImporter()
{
  if (isLoaded())
  {
    FPDF_ClosePage(m_Page);
    FPDF_CloseDocument(m_pDocument);
  }
}

OdRxDictionaryPtr PdfImporter::properties() { return m_pProperties; }

#define TOCHAR(a) ((char *)OdAnsiString(OdString(a)).c_str())

double PdfImporter::getPageWidth() const
{
  return FPDF_GetPageWidth(m_Page);
}

void PdfImporter::setClipBoundary(const OdGePoint2dArray& clipBoundary, const bool is_clip_boundary_inverted)
{
  m_ClipBoundary = clipBoundary;
  m_isClipBoundaryInverted = is_clip_boundary_inverted;
}

void PdfImporter::setLayersInfo(std::map<ByteString, bool>& layers_info)
{
  m_LayersInfo = layers_info;
}

void PdfImporter::ClearUnderlayInfo()
{
  m_ClipBoundary.clear();
  m_LayersInfo.clear();
  m_isClipBoundaryInverted = false;
}

double PdfImporter::getPageHeight() const
{
  return FPDF_GetPageHeight(m_Page);
}

double PdfImporter::getMeasureDictInfo()
{
  double xScale = 0;

  CPDF_Document* pDoc = CPDFDocumentFromFPDFDocument(m_pDocument);
  if (pDoc)
  {
    CPDF_Dictionary* pPageDict = pDoc->GetPage(m_PageNum);
    if (pPageDict)
    {
      CPDF_Array* pVPArr = pPageDict->GetArrayFor("VP");
      if (pVPArr && pVPArr->GetCount()) 
      {
        CPDF_Dictionary* pVP = pVPArr->GetDictAt(0);
        if (pVP)
        {
          CPDF_Dictionary* pMeasureDict = pVP->GetDictFor("Measure");
          if (pMeasureDict)
          {
            CPDF_Array* pXArr = pMeasureDict->GetArrayFor("X");
            if (pXArr && pXArr->GetCount() == 1) //see INT-763 for more info about pXArr->GetCount()
            {
              CPDF_Dictionary* pX = pXArr->GetDictAt(0);
              if (pX)
                xScale = pX->GetFloatFor("C");
            }
          }
        }
      }
    }
  }

  return (xScale == 0.) ? 1. : (xScale)*72.0;
}

OdPdfImport::ImportResult PdfImporter::fillLayersInfo(std::map<ByteString, bool>& layers_info)
{
  CPDF_Document* pDoc = CPDFDocumentFromFPDFDocument(m_pDocument);
  if (!pDoc)
    return OdPdfImport::fail;
  CPDF_Dictionary* pRoot = pDoc->GetRoot(); //required
  if (!pRoot)
    return OdPdfImport::fail;
  if (pRoot->KeyExist("OCProperties"))
  {
    CPDF_Array* pLayers = pRoot->GetDictFor("OCProperties")->GetArrayFor("OCGs"); //required
    if (!pLayers)
      return OdPdfImport::fail;
    CPDF_Dictionary* pD = pRoot->GetDictFor("OCProperties")->GetDictFor("D"); //required
    if (!pD)
      return OdPdfImport::fail;

    CPDF_Array* pONLayers = pD->GetArrayFor("ON");
    CPDF_Array* pOFFLayers = pD->GetArrayFor("OFF");

    if (!pOFFLayers || !pONLayers)
    {
      if (!pONLayers)
        pONLayers = pD->SetNewFor<CPDF_Array>("ON");
      if (!pOFFLayers)
        pOFFLayers = pD->SetNewFor<CPDF_Array>("OFF");
      ByteString baseSt = pD->GetStringFor("BaseState");
      if (baseSt.GetStringLength() == 0 || baseSt == "ON")
      {
        pONLayers->Clear();
        for (OdUInt32 i = 0; i < pLayers->GetCount(); i++)
        {
          if (pOFFLayers->end() == std::find_if(pOFFLayers->begin(), pOFFLayers->end(),
            [&](const std::unique_ptr<CPDF_Object>& candidate)
            {
              return candidate->GetDirect() == pLayers->GetObjectAt(i)->GetDirect();
            }))
          {
            pONLayers->Add(pLayers->GetObjectAt(i)->Clone());
          }
        }
      }
      else if (baseSt == "OFF")
      {
        pOFFLayers->Clear();
        for (OdUInt32 i = 0; i < pLayers->GetCount(); i++)
        {
          if (pONLayers->end() == std::find_if(pONLayers->begin(), pONLayers->end(),
            [&](const std::unique_ptr<CPDF_Object>& candidate)
            {
              return candidate->GetDirect() == pLayers->GetObjectAt(i)->GetDirect();
            }))
          {
            pOFFLayers->Add(pLayers->GetObjectAt(i)->Clone());
          }
        }
      }
    }

    if (0 != layers_info.size())
    {
      pONLayers->Clear();
      pOFFLayers->Clear();
      for (auto& elem : layers_info)
      {
        for (OdUInt32 i = 0; i < pLayers->GetCount(); i++)
        {
          CPDF_Dictionary* pOCG = pLayers->GetObjectAt(i)->AsReference()->GetDirect()->AsDictionary();
          if (pOCG)
          {
            if (!OdString(pOCG->GetStringFor("Name").c_str()).compare(elem.first.c_str()))
            {
              if (elem.second)
                pONLayers->Add(pLayers->GetObjectAt(i)->Clone());
              else
                pOFFLayers->Add(pLayers->GetObjectAt(i)->Clone());
            }
          }
        }
      }
      return OdPdfImport::success;
    }

    for (OdUInt32 i = 0; i < pONLayers->GetCount(); i++)
    {
      CPDF_Dictionary* pOCG_ON = pONLayers->GetObjectAt(i)->AsReference()->GetDirect()->AsDictionary();
      if (!pOCG_ON)
        return OdPdfImport::fail;
      layers_info.emplace(ByteString::FromUnicode(pOCG_ON->GetUnicodeTextFor("Name").c_str()), true);
    }
    for (OdUInt32 i = 0; i < pOFFLayers->GetCount(); i++)
    {
      CPDF_Dictionary* pOCG_OFF = pOFFLayers->GetObjectAt(i)->AsReference()->GetDirect()->AsDictionary();
      if (!pOCG_OFF)
        return OdPdfImport::fail;
      layers_info.emplace(ByteString::FromUnicode(pOCG_OFF->GetUnicodeTextFor("Name").c_str()), false);
    }
  }
  return OdPdfImport::success;
}

OdPdfImport::ImportResult PdfImporter::import()
{
  OdPdfImport::ImportResult result(OdPdfImport::success);
  try
  {
    result = load();
    if (OdPdfImport::success != result)
      return result;

    CPDF_Page* pPage = CPDFPageFromFPDFPage(m_Page);
    if (!pPage)
    {
      return OdPdfImport::fail;
    }

    PdfPropertiesPtr properties = PdfProperties::createObject();
    properties = m_pProperties;

    OdDbDatabasePtr pDb = OdDbDatabasePtr(properties->get_Database());

    OdString image_path =  pDb->appServices()->getPDFIMPORTIMAGEPATH();
    if(image_path.isEmpty())
      image_path = pDb->appServices()->getTempPath();
    if (0 != image_path.getLength() && image_path[image_path.getLength() - 1] != pathChar)
      image_path += pathChar;

    const double measure = getMeasureDictInfo();
    properties->put_Scaling(m_pProperties->get_Scaling() * measure);

    double page_width = getPageWidth();
    double page_height = getPageHeight();

    CPDF_PageRenderContext* pContext = new CPDF_PageRenderContext;

    pPage->SetRenderContext(pdfium::WrapUnique(pContext));

    CFX_RenderDevice* pDevice = new CFX_RenderDevice;

    size_t error_conter(0);

    ByteString layer_name;
    bool is_object_visible;

    std::map<ByteString, bool> layers_info(m_LayersInfo);
    result = fillLayersInfo(layers_info);
    if (OdPdfImport::success != result)
      return result;

    OdGePoint2dArray clip_boundary(m_ClipBoundary);
    for(auto& elem : clip_boundary)
      elem /= measure;

    pDevice->SetDeviceDriver(pdfium::MakeUnique<OdDbDeviceDriver>(page_width, -page_height, properties,
      error_conter, layer_name, layers_info, is_object_visible, clip_boundary, m_isClipBoundaryInverted,
      image_path, result));
    pContext->m_pDevice.reset(pDevice);

    int flags = FPDF_ANNOT;
    if (properties->get_ImportTrueTypeTextAsGeometry())
      flags = flags | RENDER_NO_NATIVETEXT | RENDER_NOTEXTSMOOTH;

    RenderPage_Retail(pContext, m_Page , 0, 0, page_width, -page_height, 0, flags, true, nullptr, layer_name, is_object_visible);

    pPage->SetRenderContext(nullptr);
  }
  catch (...)
  {
    return OdPdfImport::fail;
  }

  return result;
}

OdPdfImport::ImportResult PdfImporter::load()
{
  if(isLoaded())
    return OdPdfImport::success;

  OdString path(m_pProperties->get_PdfPath());

  OdString password = m_pProperties->get_Password();

  m_PageNum = m_pProperties->get_PageNumber() - 1;

  m_pDocument = FPDF_LoadDocument(TOCHAR(path.c_str()), TOCHAR(password.c_str()));

  if (NULL == m_pDocument)
  {
    unsigned long error = FPDF_GetLastError();
    if (FPDF_ERR_PASSWORD == error)
      return OdPdfImport::bad_password;
    return OdPdfImport::fail;
  }

  int page_count = FPDF_GetPageCount(m_pDocument);

  if (m_PageNum >= page_count)
  {
    FPDF_CloseDocument(m_pDocument);
    return OdPdfImport::invalid_page_number;
  }

  m_Page = FPDF_LoadPage(m_pDocument, m_PageNum);

  return OdPdfImport::success;
}

bool PdfImporter::isLoaded() const
{
  return (m_Page != NULL && m_pDocument != NULL);
}

class PdfImportPointTracker : public OdStaticRxObject<OdEdPointTracker>
{
  OdDbDatabase*  m_pDb;
  OdDb3dSolidPtr m_ent;
  const double   m_Width;
  const double   m_Height;
  const double   m_Angle;

public:
  PdfImportPointTracker(OdDbDatabase* pDb, OdDb3dSolidPtr ent, const double width, const double height, const double angle)
    : m_pDb(pDb)
    , m_ent(ent)
    , m_Width(width)
    , m_Height(height)
    , m_Angle(angle)
  {
  }

  static OdGeMatrix3d getUcsToWcsMatrix(OdDbDatabase* pDb)
  {
    OdGeMatrix3d ucsToWcsMatrix;
    ucsToWcsMatrix.setCoordSystem(pDb->getUCSORG(), pDb->getUCSXDIR(), pDb->getUCSYDIR(),
      pDb->getUCSXDIR().crossProduct(pDb->getUCSYDIR()));
    return ucsToWcsMatrix;
  }

  virtual int addDrawables(OdGsView* pView)
  {
    pView->add(m_ent, 0);
    return 1;
  }

  virtual void removeDrawables(OdGsView* pView)
  {
    pView->erase(m_ent);
  }

  virtual void setValue(const OdGePoint3d& value)
  {
    m_ent->createBox(m_Width, m_Height, 1);
    OdGePoint3d center = value + OdGeVector3d(m_Width / 2.0, m_Height / 2.0, 0);
    m_ent->transformBy(OdGeMatrix3d::translation(center.asVector()));
    m_ent->transformBy(OdGeMatrix3d::rotation(m_Angle, OdGeVector3d::kZAxis, value));
    m_ent->transformBy(getUcsToWcsMatrix(m_pDb));
  }

};

class PdfImportSourceTracker : public OdStaticRxObject<OdEdSSetTracker>
{

public:
  PdfImportSourceTracker()
  {}
  virtual int addDrawables(OdGsView*) { return 0; }
  virtual void removeDrawables(OdGsView*) {}

  virtual bool append(const OdDbObjectId& entId, const OdDbSelectionMethod*)
  {
    OdDbObjectPtr obj = entId.openObject();
    return !obj.isNull() && obj->isKindOf(OdDbPdfReference::desc());
  }
  virtual bool remove(const OdDbObjectId&, const OdDbSelectionMethod*)
  {
    return true;
  }
};

class PdfImportCommand : public OdEdCommand
{
public:
  const OdString groupName() const { return OD_T("IMPORTS"); }
  const OdString globalName() const { return OD_T("pdfIn"); };
  void execute(OdEdCommandContext* pCmdCtx)
  {
    std::function<void(OdPdfImport::ImportResult res)> error_func = [](OdPdfImport::ImportResult res)
    {
      switch (res)
      {
      case OdPdfImport::fail:
        throw OdError(eTxError);
      case OdPdfImport::bad_password:
        throw OdError(eInvalidPassword);
      case OdPdfImport::bad_file:
        throw OdError(eFileNotFound);
      case OdPdfImport::bad_database:
        throw OdError(eWrongDatabase);
      case OdPdfImport::encrypted_file:
        throw OdError(eDecryptionError);
      case OdPdfImport::invalid_page_number:
        throw OdError(eInvalidIndex);
      case OdPdfImport::image_file_error:
        throw OdError(eFilerError);
      }
    };

    PdfImporterPtr importer = createImporter();
    OdRxDictionaryPtr pProps = importer->properties();

    OdDbUserIOPtr pIO = OdDbCommandContextPtr(pCmdCtx)->dbUserIO();

    try
    {

      PdfImportSourceTracker trecker;

      OdDbSelectionSetPtr pSSet;
      OdDbPdfReferencePtr pPdfRef;
      OdDbPdfDefinitionPtr pPdfDef;

      bool is_file_import(false);
      do 
      {
        is_file_import = pIO->getKeyword(L"Enter source for import [File/Underlay]", L"File Underlay", 0) == 0;
        if (!is_file_import)
        {
          pSSet = pIO->select(OD_T("Select PDF underlay:"), OdEd::kSelSingleEntity| OdEd::kSelSinglePass, NULL, OdString::kEmpty, &trecker);
          if(pSSet->numEntities())
            break;
        }
      } 
      while (!is_file_import);

      // Target database
      pProps->putAt(OD_T("Database"), OdDbCommandContextPtr(pCmdCtx)->database());

      OdString path;
      OdString password;
      OdUInt32 page_num(0);
      double scale(1.0), rotation(0.0);
      if (is_file_import)
      {
        path = pIO->getFilePath(OD_T("Enter file name:"), OdEd::kGfpForOpen, OD_T("Pdf file to load"),
          OD_T("pdf"), OdString::kEmpty, OD_T("Design Web Format (*.pdf)|*.pdf|PDF file (*.pdf)||"));
        password = pIO->getString(OD_T("password <>:"));
        page_num = (OdInt32)pIO->getInt(OD_T("Select page number to import <1>:"), OdEd::kInpNonZero | OdEd::kInpNonNeg, 1);
        rotation = pIO->getReal(OD_T("Select rotation <0.>:"), OdEd::kInpDefault, 0.0);
        scale = pIO->getReal(OD_T("Select scale <1.>:"), OdEd::kInpNonNeg, 1.0);

        importer->ClearUnderlayInfo();
      }
      else
      {
        pPdfRef = OdDbPdfReference::cast(pSSet->objectIdArray().first().openObject(OdDb::kForWrite));
        if (pPdfRef.isNull())
          return;
        pPdfDef = OdDbPdfDefinition::cast(pPdfRef->definitionId().openObject(OdDb::kForWrite));
        if (pPdfDef.isNull())
          return;

        path = pPdfDef->getActiveFileName();
        page_num = odStrToUInt(pPdfDef->getItemName());
        scale = odmax(pPdfRef->scaleFactors().sx, pPdfRef->scaleFactors().sy);
        rotation = pPdfRef->rotation();
        OdGePoint3d insertion_point = pPdfRef->position();
        pProps->putAt(OD_T("InsertionPointX"), OdRxVariantValue(insertion_point.x));
        pProps->putAt(OD_T("InsertionPointY"), OdRxVariantValue(insertion_point.y));

        importer->setClipBoundary(pPdfRef->clipBoundary(), pPdfRef->isClipInverted());

        std::map<ByteString, bool> layers_info;
        for (OdUInt32 i = 0; i < pPdfRef->underlayLayerCount(); ++i)
        {
          OdUnderlayLayer layer;
          pPdfRef->getUnderlayLayer(i, layer);
          ByteString layer_name = OdAnsiString(layer.name()).c_str();
          layers_info.emplace(std::make_pair(layer_name.c_str(), OdUnderlayLayer::kOn == layer.state()));
        }
        importer->setLayersInfo(layers_info);

      }
      if (path.isEmpty())
        return;


      // Source PDF file
      pProps->putAt(OD_T("PdfPath"), OdRxVariantValue(path));
      // PDF may be password protected
      pProps->putAt(OD_T("Password"), OdRxVariantValue(password));

      // PDF may contain many sheets. You may select which one to import
      pProps->putAt(OD_T("PageNumber"), OdRxVariantValue(page_num));
      pProps->putAt(OD_T("Rotation"), OdRxVariantValue(rotation));
      pProps->putAt(OD_T("Scaling"), OdRxVariantValue(scale));

      OdPdfImport::ImportResult load_res = importer->load();
      if (OdPdfImport::success != load_res)
      {
        if (OdPdfImport::bad_password != load_res)
          error_func(load_res);
        //check password cache for the valid password.
        OdPwdCachePtr pCache = OdDbCommandContextPtr(pCmdCtx)->database()->appServices()->getPasswordCache();
        OdPassword cache_password;
        if (!pCache.isNull())
        {
          OdPwdIteratorPtr pIter = pCache->newIterator();
          if (!pIter.isNull())
          {
            for (; !pIter->done() && load_res != OdPdfImport::success; pIter->next())
            {
              pIter->get(cache_password);
              pProps->putAt(OD_T("Password"), OdRxVariantValue(cache_password));
              load_res = importer->load();
            }
          }
        }
        if (OdPdfImport::bad_password == load_res)
        {
          // ask user about valid password
          cache_password = pIO->getString(OD_T("password <>:"));

          // try to load again with new password
          pProps->putAt(OD_T("Password"), OdRxVariantValue(cache_password));
          load_res = importer->load();
          if (OdPdfImport::bad_password == load_res)
          {
            throw OdError(eInvalidPassword);
          }
          else
          {
            pCache->add(cache_password);
          }
        }
      }

      bool change_settings = pIO->getKeyword(L"Change default settings [Yes/No]", L"Yes No", 1) == 0;

      if (change_settings)
      {
        pProps->putAt(OD_T("LayersUseType"), OdRxVariantValue(
          (OdUInt8)pIO->getKeyword(L"Enter method to apply for assigning imported objects to layers[UsePDFLayers/ObjectLayers/CurrentLayer]",
            L"UsePDFLayers ObjectLayers CurrentLayer", 1)));

        pProps->putAt(OD_T("ImportVectorGeometry"), OdRxVariantValue((pIO->getKeyword(OD_T("Enable import of vector geometry [Yes/No]"), OD_T("Yes No"), 0) == 0)));
        pProps->putAt(OD_T("ImportSolidFills"), OdRxVariantValue((pIO->getKeyword(OD_T("Enable import of solid fills [Yes/No]"), OD_T("Yes No"), 0) == 0)));
        pProps->putAt(OD_T("ImportTrueTypeText"), OdRxVariantValue((pIO->getKeyword(OD_T("Enable import of TrueType text [Yes/No]"), OD_T("Yes No"), 0) == 0)));
        pProps->putAt(OD_T("ImportRasterImages"), OdRxVariantValue((pIO->getKeyword(OD_T("Enable import of raster images [Yes/No]:"), OD_T("Yes No"), 0) == 0)));
        pProps->putAt(OD_T("ImportGradientFills"), OdRxVariantValue((pIO->getKeyword(OD_T("Enable shading objects [Yes/No]"), OD_T("Yes No"), 0) == 0)));
        pProps->putAt(OD_T("ImportAsBlock"), OdRxVariantValue((pIO->getKeyword(OD_T("Enable import as block [Yes/No]"), OD_T("Yes No"), 1) == 0)));
        pProps->putAt(OD_T("JoinLineAndArcSegments"), OdRxVariantValue((pIO->getKeyword(OD_T("Enable joins contiguous segments into a polyline where possible [Yes/No]"), OD_T("Yes No"), 1) == 0)));
        pProps->putAt(OD_T("ConvertSolidsToHatches"), OdRxVariantValue((pIO->getKeyword(OD_T("Enable converts 2D solid objects into solid-filled hatches [Yes/No]"), OD_T("Yes No"), 0) == 0)));
        pProps->putAt(OD_T("ApplyLineweight"), OdRxVariantValue((pIO->getKeyword(OD_T("Apply lineweight properties [Yes/No]"), OD_T("Yes No"), 0) == 0)));
        pProps->putAt(OD_T("ImportTrueTypeTextAsGeometry"), OdRxVariantValue((pIO->getKeyword(OD_T("Import TrueType text as geometry [Yes/No]"), OD_T("Yes No"), 1) == 0)));
      }

      if (is_file_import)
      {
        error_func(importer->load());

        const double measure = importer->getMeasureDictInfo();
        OdDb3dSolidPtr pSolid = OdDb3dSolid::createObject();
        pSolid->setDatabaseDefaults(OdDbCommandContextPtr(pCmdCtx)->database());
        PdfImportPointTracker tracker(OdDbCommandContextPtr(pCmdCtx)->database(), pSolid, importer->getPageWidth()*measure / 72.,
          importer->getPageHeight()*measure / 72., OdRxVariantValue(pProps->getAt(OD_T("Rotation"))));
        OdGePoint3d insertion_point;
        insertion_point = pIO->getPoint(OD_T("Specify insertion point: "), OdEd::kGptDefault, &insertion_point, OD_T("Insertion point"), &tracker);
        pProps->putAt(OD_T("InsertionPointX"), OdRxVariantValue(insertion_point.x));
        pProps->putAt(OD_T("InsertionPointY"), OdRxVariantValue(insertion_point.y));
      }
      else
      {
        OdUInt8 post_action = pIO->getKeyword(L"Keep, Detach or Unload PDF underlay? [Keep/Detach/Unload]", L"Keep Detach Unload", 2);
        switch (post_action)
        {
        case 1:
          {
            OdDbBlockTableRecordPtr pBlock = OdDbCommandContextPtr(pCmdCtx)->database()->getActiveLayoutBTRId().safeOpenObject(OdDb::kForWrite);
            OdDbObjectIteratorPtr pIter = pBlock->newIterator();
            for (pIter->start(); !pIter->done(); pIter->step())
            {
              OdDbEntityPtr pEntity = pIter->entity();
              OdDbPdfReferencePtr pRef = OdDbPdfReference::cast(pEntity);
              if (!pRef.isNull() && pRef->definitionId() == pPdfDef->objectId())
              {
                pRef->upgradeOpen();
                pRef->erase();
              }
            }
            pPdfDef->erase();
          }
          break;
        case 2:
          pPdfDef->unload();
          break;
        }
      }


    }
    catch (const OdEdKeyword&)
    {
    }

    OdPdfImport::ImportResult res = importer->import();
    pIO->putString(OD_T("PdfImport complete"));
    error_func(res);
  }
};

class PdfImportModule : public OdPdfImportModule
{
  OdStaticRxObject<PdfImportCommand> m_Command;
public:
  virtual void initApp()
  {
    FPDF_InitLibrary();
    odedRegCmds()->addCommand( &m_Command );
  }
  virtual void uninitApp()
  {
    FPDF_DestroyLibrary();
    odedRegCmds()->removeCmd( m_Command.groupName(), m_Command.globalName() );
  }
  
  virtual OdPdfImportPtr create ()
  {
      return OdRxObjectImpl<PdfImporter>::createObject();
  }

};

ODRX_DEFINE_DYNAMIC_MODULE(PdfImportModule)
