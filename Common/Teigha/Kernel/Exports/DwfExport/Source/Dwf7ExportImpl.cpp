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
// DwfExportImpl.cpp : implementation of the CDwfExportImpl class
//
///////////////////////////////////////////////////////////////////////////////

#include "OdaCommon.h"

#define STL_USING_MAP
#define STL_USING_VECTOR
#include "OdaSTL.h"

#include "DbBaseDatabase.h"
#include "Ge/GeExtents3d.h"
#include "OdTimeStamp.h"
#include "DbBaseHostAppServices.h"
#include "ColorMapping.h"
#include "OdString.h"

#ifdef _MSC_VER
#pragma warning(push)
#include <yvals.h>              // warning numbers get enabled in yvals.h 
#pragma warning(disable: 4290)
#pragma warning(disable: 4244)
#endif // _MSC_VER


#include "2dSupport.h"
#include "DwfDrawObject.h"
#include "DwfExportImpl.h"
#include "DwfSupport.h"

extern const WT_RGBA32 WD_Old_Default_Palette[256];
extern const WT_RGBA32 WD_New_Default_Palette[256];

#ifdef OD_HAVE_DIRECT_FILE
#include <direct.h>
#endif

#include <sys/stat.h>
#include <errno.h>


// Use this instead of %c, since %c isn't consistent across platforms
#define DWF_DATE_FORMAT_STR OD_T("%m/%d/%Y %I:%M:%S %p")

#include "dwfcore/File.h"
#include "dwfcore/MIME.h"
#include "dwf/Version.h"
#include "dwf/package/Constants.h"
#include "dwf/package/EPlotSection.h"
#include "dwf/package/writer/DWF6PackageWriter.h"
#include "dwf/whiptk/w2d_class_factory.h"
#include "dwf/package/writer/DWFXPackageWriter.h"
#include "dwf/XAML/XamlClassFactory.h"
#include "dwf/XAML/XamlFile.h"
#include "dwf/dwfx/Constants.h"
#include "dwf/package/ContentManager.h"
#if defined(_MSC_VER)
#include "dwf/publisher/win32/EmbeddedFontImpl.h"
#endif

#ifdef _MSC_VER
#pragma warning(pop)
#endif // _MSC_VER

using namespace DWFCore;
using namespace DWFToolkit;

#define OBFUSCATE

namespace TD_DWF_EXPORT {

  void DataForDWFPlotting::SetClip(double paperClip[])
  {
    for (int i = 0; i < 4; i++)
    {
      m_pPaperClip[i] = paperClip[i];
    }
  }
  void DataForDWFPlotting::SetExtents(double extents[])
  {
    for (int i = 0; i < 4; i++)
    {
      m_pClip[i] = extents[i];
    }
  }
  void DataForDWFPlotting::SetTransform(double dwfToPaper[])
  {
    for (int i = 0; i < 4; i++)
    {
      for (int j = 0; j < 4; j++)
      {
        m_pTransform[i * 4 + j] = dwfToPaper[i * 4 + j];
      }
    }
  }

  void DataForDWFPlotting::SetColor( ODCOLORREF color)
  {
    // AMark : 26.08.2008 : Background color conversion
    // ABGR >> ARGB [WIN32 tested, test other platforms]
    m_nColorARGB = ODRGBA(ODGETBLUE(color), ODGETGREEN(color), ODGETRED(color), ODGETALPHA(color));
  }

//---------------------------------------------------------------------------
// SPecifyImageMimeType()
//
//  Specifies the file resource's MIME-type
//  Returns true for jpg or png files.
//  In other cases there is no appropriate file resource's MIME-type
//
bool SpecifyImageMimeType(DWFString& zMimeType, const OdChar* pImage)
{
  bool bRet(false);
#ifdef TD_UNICODE
  std::wstring sImage = pImage;
#else
  std::string sImage = pImage;
#endif
  if (sImage.rfind(OD_T(".jpg")) != std::string::npos)
  {
    zMimeType = DWFString( DWFMIME::kzMIMEType_JPG );
    bRet = true;
  }
  else if (sImage.rfind(OD_T(".png")) != std::string::npos)
  {
    zMimeType = DWFString( DWFMIME::kzMIMEType_PNG );
    bRet = true;
  }
  else
  {
    bRet = false;     // unsupported
  }

  return bRet;
}

//static bool isAnythingForExport(OdDbDatabase* database(), OdDbObjectId layoutId) // before 22.12,2011
static bool isAnythingForExport(/*OdDbBaseDatabase* pDb,*/ OdRxIteratorPtr layoutIter) // dgn dwf export
{
  bool bRet(true);

  //OdDbLayoutPtr pLayout = layoutId.safeOpenObject();
  //OdDbBaseLayoutPEPtr pLayout(layoutIter);
  OdDbBaseLayoutPEPtr pLayout(layoutIter->object());

  
  //if (pLayout->getBlockTableRecordId() != database()->getModelSpaceId() // before 22.12.2011
  //if (pLayout->getBlockTableRecordId() != ((OdDbBaseDatabasePE*)database())->getFirstLayout() // dgn dwf export
  //      && 
  //    (pLayout->overallVportId()).isErased()) // no viewport in paper space
  //if (!pLayout->isModelLayout(pLayout.get()) && pLayout->isOverallVPortErased(pLayout.get()))
  if (!pLayout->isModelLayout(layoutIter->object()) && pLayout->isOverallVPortErased(layoutIter->object()))
  {
    bRet = false;
  }
  return bRet;
}

//***************************************************************************
//
// CDwfExportMeter
//  Progress Meter processing
//
//***************************************************************************
CDwfExportMeter::CDwfExportMeter()
  : m_pProgressMeter(NULL)
{
}

void CDwfExportMeter::pmInit()
{
  if (!m_pProgressMeter)
  {
    OdDbBaseDatabasePEPtr pDb(getDB());
    OdDbBaseHostAppServices *pServ = pDb->appServices(getDB());
    m_pProgressMeter = pServ->newProgressMeter();
    //m_pProgressMeter = OdDbBaseDatabasePEPtr(getDB())->appServices(getDB())->newProgressMeter();
  }
}

void CDwfExportMeter::pmRelease()
{
  if (m_pProgressMeter)
  {
    OdDbBaseDatabasePEPtr pDb(getDB());
    OdDbBaseHostAppServices *pServ = pDb->appServices(getDB());
    pServ->releaseProgressMeter(m_pProgressMeter);
    //OdDbBaseDatabasePEPtr(getDB())->appServices(getDB())->releaseProgressMeter(m_pProgressMeter);
  }
}

void CDwfExportMeter::pmStart(const OdChar* displayString)
{
  if (m_pProgressMeter)
  {
    m_pProgressMeter->start(displayString);
  }
}

void CDwfExportMeter::pmStop()
{
  if (m_pProgressMeter)
  {
    m_pProgressMeter->stop();
  }
}

void CDwfExportMeter::pmMeterProgress()
{
  if(m_pProgressMeter)
  {
    m_pProgressMeter->meterProgress();
  }
}

void CDwfExportMeter::pmSetLimit(int max)
{
  if (m_pProgressMeter)
  {
    m_pProgressMeter->setLimit(max);
  }
}

//***************************************************************************
//
// CDwfExportImpl
//
//***************************************************************************
CDwfExportImpl::CDwfExportImpl(DwExportParams& Params)
    : m_Params(Params)
    , m_wtFile(0)
    , m_bWtDefaulPalette(false)
{
}

CDwfExportImpl::~CDwfExportImpl()
{
  if (m_wtFile)
    delete m_wtFile;
}


//============================================================================
//
// isDwfVersion6()
//
// Checks if the DWF version is 06.00 and greater.
//  Returns true in this case.
//  Otherwise - false.
//
bool CDwfExportImpl::isDwfVersion6()
{
  if (m_Params.version() > nDwf_v6_unknown)
  {
    ODA_FAIL(); // unknown version
    throw OdError(eIncorrectInitFileVersion);
  }
  return m_Params.version() >= nDwf_v60;
}

bool CDwfExportImpl::runExport()
{
  try
  {
    // To set the palette to be used. 
    //  If parameter is NULL - one of two default palettes will be used depending on background color.
    m_pPalette = (m_Params.palette() != NULL) ? m_Params.palette() : odcmAcadPalette(m_Params.background());

    if (isDwfVersion6())
    {
      if (m_Params.format() == DW_COMPRESSED_BINARY) // DWF6 does not support w2d compression
        return false;

      OdGsDevicePtr pDevice = OdDwfDevice::createObject();
      m_pDeviceDwf = (OdDwfDevice*)pDevice.get();
      if (m_Params.format() == DW_XPS)
        m_pDeviceDwf->setClassFactory(new WT_XAML_Class_Factory());
      else
        m_pDeviceDwf->setClassFactory(new WT_W2D_Class_Factory());

      OdArray<DwfLayoutInfo> g_arrayLayoutInfo;
      initializeLayoutList(g_arrayLayoutInfo);
      if (g_arrayLayoutInfo.isEmpty())
      {
        (OdDbBaseDatabasePEPtr(m_Params.database()))->appServices(m_Params.database())->warning(L"\nexportDwf: at least one layout must be initialized, exiting");
        return false;
      }

      pmInit();
      pmStart(OD_T("Publishing Dwf6 file..."));
      pmSetLimit(g_arrayLayoutInfo.size());

      DWFCore::DWFFile oDWF( m_Params.dwfFileName().c_str() );
      std::auto_ptr<DWFToolkit::DWFPackageWriter> oWriter( (m_Params.format() == DW_XPS) ? 
        static_cast<DWFToolkit::DWFPackageWriter*>(new DWFToolkit::DWFXPackageWriter(oDWF))
        : static_cast<DWFToolkit::DWFPackageWriter*>(new DWFToolkit::DWF6PackageWriter(oDWF)) );
      int validLayouts = 0;
      for (unsigned int i = 0; i < g_arrayLayoutInfo.size(); ++i)
      {
        DwfLayoutInfo& layoutInfo = g_arrayLayoutInfo[i];
        initPlotting(layoutInfo);
        initColors();

        OdDbBaseDatabasePEPtr(m_Params.database())->setCurrentLayout(m_Params.database(), layoutInfo.m_sLayoutName);

        m_pDeviceDwf->eraseAllViews();

        // To create & activate own Dwf View        
        OdGiDefaultContextPtr pCtx = OdDbBaseDatabasePEPtr(m_Params.database())->createGiContext(m_Params.database());
        pCtx->setPlotGeneration(true);
        pCtx->setPaletteBackground(m_Params.background());
        OdDbBaseDatabasePEPtr(m_Params.database())->loadPlotstyleTableForActiveLayout(pCtx, m_Params.database());

        m_pDevice = OdDbBaseDatabasePEPtr(m_Params.database())->setupLayoutView(m_pDeviceDwf, pCtx, layoutInfo.m_idLayout);// dgn dwf export
        m_pDeviceDwf->setBackgroundColor(m_Params.background());
        m_pDeviceDwf->setLogicalPalette(m_pPalette, 256);

        if (!layoutInfo.m_PlotInfo.m_bUninitializedLayout)
          setPlotSettingsToLayout();

        m_pDeviceDwf->setRenderDevice();
        for (int nDevice = 0; nDevice < m_pDeviceDwf->numRenderDevices(); nDevice++)
        {
          OdGsDevice* pRenderDevice = m_pDeviceDwf->renderDeviceAt(nDevice);
          pRenderDevice->setBackgroundColor(m_Params.background());
          pRenderDevice->setLogicalPalette(m_pPalette, 256);
          OdGsDevicePtr pLayoutHelper = (OdDbBaseDatabasePEPtr(m_Params.database()))->setupLayoutView(pRenderDevice, pCtx, g_arrayLayoutInfo[i].m_idLayout);// dgn dwf export
          m_pDeviceDwf->setRenderLayoutHelperAt(nDevice, pLayoutHelper.get());
        }
        OdGsDCRect outputRect( OdGsDCPoint( m_rectOutput.left,  m_rectOutput.bottom),
          OdGsDCPoint( m_rectOutput.right, m_rectOutput.top) );

        setDeviceScale(layoutInfo);

        m_pDeviceDwf->onSize(outputRect);

        DWFToolkit::DWFEPlotSection* section = createPlotSection(layoutInfo);
        if (!section)
        {
          (OdDbBaseDatabasePEPtr(m_Params.database()))->appServices(m_Params.database())->warning(L"\nexportDwf: skipping invalid layout");
          continue;
        }
        ++validLayouts;
        oWriter->addSection(section);
        openWtFile(layoutInfo);
        m_pDeviceDwf->setContext(this);
        try
        {
          draw(layoutInfo);
        }
        catch (const OdError&)
        {
          try { m_wtFile->close(); }
          catch (...) {}
          delete m_wtFile;
          m_wtFile = 0;
          throw;
        }

        m_wtFile->close();
        delete m_wtFile;
        m_wtFile = 0;
        m_pDevice->eraseAllViews();

        // add embedded font resources for DWF6
        if (m_Params.format() != DW_XPS)
        {
          addFonts(layoutInfo.m_iParamIndex < (int)m_Params.pageData().size() ? &m_Params.pageData()[ layoutInfo.m_iParamIndex ] : 0, section);
        }

        pmMeterProgress();
      }

      pmStop();
      if (validLayouts)
      {
        pmStart(OD_T("Writing Dwf6 file..."));
        oWriter->write(L"Open Design Alliance Inc.", L"Open Design DWF Export",         
          TD_PRODUCT_VER_STR_W, L"Autodesk", _DWFTK_VERSION_STRING, DWFZipFileDescriptor::eZipSmallest);
        pmStop();
      }
      pmRelease();
    }
    else // previous DWF versions
    {
      begin();
      OdDbBaseDatabasePEPtr pDbPE(m_Params.database());
      OdRxObjectPtr pL = pDbPE->currentLayout(m_Params.database());
      OdDbBaseLayoutPEPtr pLayout(pL);
      DwfLayoutInfo layoutInfo;
      layoutInfo.m_iParamIndex = 0;
      layoutInfo.m_idLayout = pDbPE->getId(pL.get());
      layoutInfo.m_sLayoutName = pLayout->name(pL.get());
      setDeviceScale(layoutInfo);
      draw(layoutInfo);
      end();
    }
  }
  catch (DWFException& ex)
  {
    OdString msg = OdString(L"Dwf6 exception is thrown during exportDwf: ") +
      ex.message() + OdString(ex.function()) + ex.file();
    (OdDbBaseDatabasePEPtr(m_Params.database()))->appServices(m_Params.database())->warning(msg);
    pmStop();
    pmRelease();
    return false;
  }
  catch (WT_Result::Enum result)
  {
    OdString msg;
    msg.format(L"WhipTk exception is thrown during exportDwf: %ld", result);
    (OdDbBaseDatabasePEPtr(m_Params.database()))->appServices(m_Params.database())->warning(msg);
    pmStop();
    pmRelease();
    return false;
  }
  catch (OdError& err)
  {
    OdString msg = L"OdError is thrown during exportDwf: " + err.description();
    (OdDbBaseDatabasePEPtr(m_Params.database()))->appServices(m_Params.database())->warning(msg);
    pmStop();
    pmRelease();
    return false;
  }
  catch (...)
  {
    (OdDbBaseDatabasePEPtr(m_Params.database()))->appServices(m_Params.database())->warning(L"Unexpected error is happened during exportDwf.");
    pmStop();
    pmRelease();
    return false;
  }
  return true;
}

void CDwfExportImpl::showAllLayers()
{
  for (OdRxIteratorPtr it = OdDbBaseDatabasePEPtr(m_Params.database())->layers(m_Params.database()); !it->done(); it->next())
  {
    OdRxObjectPtr pLayer = it->object();
    OdDbBaseLayerPEPtr lpe(pLayer);
    if (lpe->isOff(pLayer) || lpe->isFrozen(pLayer))
    {
      lpe->setIsOff(pLayer, false);
      lpe->setIsFrozen(pLayer, false);
    }
  }
}

bool CDwfExportImpl::run()
{
  (OdDbBaseDatabasePEPtr(m_Params.database()))->startTransaction(m_Params.database());

  bool bRet = true;
  try
  {
    if (m_Params.exportInvisibleLayers())
      showAllLayers();

    bRet = runExport();
  }
  catch (...)
  {
  	bRet = false;
  }

  (OdDbBaseDatabasePEPtr(m_Params.database()))->abortTransaction(m_Params.database());

  return bRet;
}

//***************************************************************************
//
// begin()
//
//***************************************************************************
void CDwfExportImpl::begin()
{
  // for previous DWF versions only (earlier then v06.00)
  ODA_ASSERT(!isDwfVersion6() && !m_wtFile);
  if (!m_Params.database())
  {
    ODA_FAIL();
    throw OdError(eNoDatabase);
  }
  initContext();

  OdGsDevicePtr pDevice = OdDwfDevice::createObject();
  m_pDeviceDwf = (OdDwfDevice*)pDevice.get();
  m_pDeviceDwf->setClassFactory(new WT_W2D_Class_Factory());
  m_wtFile = m_pDeviceDwf->classFactory()->Create_File();
  m_wtFile->set_file_mode(WT_File::File_Write);
  m_wtFile->set_filename(OdW2US(m_Params.dwfFileName().c_str()));
  m_wtFile->open();
  setW2dCompression();
  createDevice();
}

static int comparePalette(const WT_RGBA32* defailt_palette, const ODCOLORREF* pPalette)
{
  int numberNotOfMatchings = 0;
  for (int i = 0; i < 256; i++)
  {
    ODCOLORREF colorFromUser = pPalette[i];
    int j = 0;
    for (; j < 256; j++)
    {
      WT_RGBA32 wt_color = defailt_palette[j];
      ODCOLORREF colorFromDefault(ODRGB(wt_color.m_rgb.r, wt_color.m_rgb.g, wt_color.m_rgb.b));
      if (colorFromUser == colorFromDefault)
        break;
    }
    if (j >= 256)
      ++numberNotOfMatchings;
  }
  return numberNotOfMatchings;
}
//
// initContext()
//
//  To initialize environment
//
void CDwfExportImpl::initContext()
{
  OdInt32 xSize = (m_Params.xSize() > lMaxDwfResolution) ? lMaxDwfResolution : m_Params.xSize();
  OdInt32 ySize = (m_Params.ySize() > lMaxDwfResolution) ? lMaxDwfResolution : m_Params.ySize();

  // DWF coordinates range is 0 - (2**31 - 1)
  // Xmin must not be 0 because if drawing has some extra space at the left
  // (panned to right for example)  Autodesk's viewer does not treat this situation
  // correctly - left margin of visible area has negative X coordinate.
  // There is no such problem with Y coord.
  // AutoCAD's DWFOUT does the same thing
  m_rectViewport.left = 0;           // Xmin
  m_rectViewport.right = m_rectViewport.left + xSize - 1; // Xmax
  m_rectViewport.bottom = 0;                                 // Ymin
  m_rectViewport.top = m_rectViewport.bottom + ySize - 1;  // Ymax
  m_rectOutput = m_rectViewport;

  ODA_ASSERT(m_pPalette);
  if (!m_pPalette)    
  {
    ODA_FAIL();
    throw OdError(eNullObjectPointer);
  }

  if (!m_Params.colorMapOptimize())
  {
    return;
  }
  // To fill from the selected palette (Default or Set by user) by RGB and corresponding index
  if (comparePalette(WD_New_Default_Palette, m_pPalette))
  {
    m_bWtDefaulPalette = false;
    m_pColors.resize(256);
  
    for (int k = 0; k < 256; k++)
    {
      ODCOLORREF color = m_pPalette[k];
      m_colorIndexMap[color] = k;
      m_pColors[k] = WT_RGBA32(ODGETRED(color), ODGETGREEN(color), ODGETBLUE(color));
    } 
  }
  else
  {
    for (int m = 0; m < 256; m++)
    {
      WT_RGBA32 wt_color = WD_New_Default_Palette[m];
      ODCOLORREF rgb(ODRGB(wt_color.m_rgb.r, wt_color.m_rgb.g, wt_color.m_rgb.b));
      m_colorIndexMap[rgb] = m;
    }

    m_bWtDefaulPalette = true;
  }
}


void CDwfExportImpl::initPlotting(DwfLayoutInfo& layoutInfo)
{
  DwfLayoutPlotInfo& plotInfo = layoutInfo.m_PlotInfo;
  OdDbBaseDatabasePEPtr db(m_Params.database());
  plotInfo.m_Measurement = (OdDb::MeasurementValue)db->getMeasurement(m_Params.database());

  OdRxObjectPtr pL = db->getLayout(m_Params.database(), layoutInfo.m_sLayoutName);
  OdDbBaseLayoutPEPtr pLayout = OdDbBaseLayoutPEPtr(pL);
  
  pLayout->getPlotPaperSize(pL.get(), plotInfo.m_dPaperWidth, plotInfo.m_dPaperHeight );

  if (OdZero(plotInfo.m_dPaperWidth) || OdZero(plotInfo.m_dPaperHeight))
  {
    // This is an uninitialized layout. To set default Plot Info
    //
    plotInfo.m_bUninitializedLayout = true;

    plotInfo.m_dPaperWidth =  210;
    plotInfo.m_dPaperHeight = 297;

    double dTopMargin    =  17.793762207031;
    double dRightMargin  =   5.7937469482422;
    double dBottomMargin =  17.793750762939;
    double dLeftMargin   =   5.7937498092651;

    plotInfo.m_dPrintableXmin = dLeftMargin;
    plotInfo.m_dPrintableYmin = dBottomMargin;
    plotInfo.m_dPrintableXmax = plotInfo.m_dPaperWidth - dRightMargin;
    plotInfo.m_dPrintableYmax = plotInfo.m_dPaperHeight - dTopMargin;

    plotInfo.m_PlotPaperUnit = DwfLayoutPlotInfo::kMillimeters;
  }
  else
  {
    // MKU 06.07.2004   #1763
    // take into account Plot Rotation
    OdDbBaseLayoutPE::PlotRotation rotation = pLayout->plotRotation(pL.get());
    double dTmp;
    switch (rotation)
    {
      case OdDbBaseLayoutPE::k90degrees:
      case OdDbBaseLayoutPE::k270degrees:
        dTmp = plotInfo.m_dPaperWidth;
        plotInfo.m_dPaperWidth = plotInfo.m_dPaperHeight;
        plotInfo.m_dPaperHeight = dTmp;

        plotInfo.m_dPrintableYmin = pLayout->getLeftMargin(pL.get());
        plotInfo.m_dPrintableXmin = pLayout->getBottomMargin(pL.get());
        plotInfo.m_dPrintableYmax = pLayout->getRightMargin(pL.get());
        plotInfo.m_dPrintableXmax = pLayout->getTopMargin(pL.get());
        break;

      default:
        plotInfo.m_dPrintableXmin = pLayout->getLeftMargin(pL.get());
        plotInfo.m_dPrintableYmin = pLayout->getBottomMargin(pL.get());
        plotInfo.m_dPrintableXmax = pLayout->getRightMargin(pL.get());
        plotInfo.m_dPrintableYmax = pLayout->getTopMargin(pL.get());
        break;
    }
    plotInfo.m_PlotPaperUnit = (DwfLayoutPlotInfo::OdDwfExportPlotPaperUnits)pLayout->plotPaperUnits(pL.get());
    if (plotInfo.m_PlotPaperUnit == DwfLayoutPlotInfo::kInches)
    {
      plotInfo.m_dPrintableXmax /= kMmPerInch;
      plotInfo.m_dPrintableYmax /= kMmPerInch;
      plotInfo.m_dPrintableXmin /= kMmPerInch;
      plotInfo.m_dPrintableYmin /= kMmPerInch;
    }

    // getPlotPaperMargins returns margins
    plotInfo.m_dPrintableXmax = plotInfo.m_dPaperWidth - plotInfo.m_dPrintableXmax;
    plotInfo.m_dPrintableYmax = plotInfo.m_dPaperHeight - plotInfo.m_dPrintableYmax;

  }

  OdInt32 xSize = (m_Params.xSize() > lMaxDwfResolution) ? lMaxDwfResolution : m_Params.xSize();
  OdInt32 ySize = (m_Params.ySize() > lMaxDwfResolution) ? lMaxDwfResolution : m_Params.ySize();

  // DWF coordinates range is 0 - (2**31 - 1)
  // Xmin must not be 0 because if drawing has some extra space at the left
  // (panned to right for example)  Autodesk's viewer does not treat this situation
  // correctly - left margin of visible area has negative X coordinate.
  // There is no such problem with Y coord.
  // AutoCAD's DWFOUT does the same thing
  m_rectViewport.left = 0;           // Xmin
  m_rectViewport.right = m_rectViewport.left + xSize - 1; // Xmax
  m_rectViewport.bottom = 0;                                 // Ymin
  m_rectViewport.top = m_rectViewport.bottom + ySize - 1;  // Ymax
  m_rectOutput = m_rectViewport;

  // Set output rectangle in which rendering will be applied
  if (pLayout->plotType(pL.get()) == 4 ) // OdDbPlotSettings::kWindow
  {
    double xMin, xMax, yMin, yMax;
    pLayout->getPlotWindowArea(pL.get(), xMin, yMin, xMax, yMax);
    // Model space will be plotted to extents in center of drawing.
    // Paper space will be plotted to center of drawing.
    // This is not seems like Acad behaviour, but some other cases (paper margins, zoom to extents and etc) are not supported also on this moment.
    double dxVal = fabs(xMax - xMin);
    double dyVal = fabs(yMax - yMin);

    if (OdZero(dyVal))
      throw OdError(eInvalidWindowArea);

    double outAspect = dxVal / dyVal;
    double inAspect = (double)(m_rectOutput.right - m_rectOutput.left) / (double)(m_rectOutput.top - m_rectOutput.bottom);
    if (inAspect != outAspect)
    {
      if (outAspect > inAspect)
      {
        long correction = (m_rectOutput.top - m_rectOutput.bottom) - OdRoundToLong((double)(m_rectOutput.right - m_rectOutput.left) / outAspect);
        m_rectOutput.bottom += correction >> 1;
        m_rectOutput.top -= correction >> 1;
      }
      else
      {
        long correction = (m_rectOutput.right - m_rectOutput.left) - OdRoundToLong((double)(m_rectOutput.top - m_rectOutput.bottom) * outAspect);
        m_rectOutput.left += correction >> 1;
        m_rectOutput.right -= correction >> 1;
      }
    }
  }
}


void CDwfExportImpl::initColors()
{
  ODA_ASSERT(m_pPalette);
  if (!m_pPalette)    
  {
    ODA_FAIL();
    throw OdError(eNullObjectPointer);
  }

  // To fill from the selected palette (Default or Set by user) by RGB and corresponding index
  if (comparePalette(WD_New_Default_Palette, m_pPalette))
  {
    m_bWtDefaulPalette = false;
    m_pColors.resize(256);

    for (int k = 0; k < 256; k++)
    {
      ODCOLORREF color = m_pPalette[k];
      m_colorIndexMap[color] = k;
      m_pColors[k] = WT_RGBA32(ODGETRED(color), ODGETGREEN(color), ODGETBLUE(color));
    }
  }
  else
  {
    for (int m = 0; m < 256; m++)
    {
      WT_RGBA32 wt_color = WD_New_Default_Palette[m];
      ODCOLORREF rgb(ODRGB(wt_color.m_rgb.r, wt_color.m_rgb.g, wt_color.m_rgb.b));
      m_colorIndexMap[rgb] = m;
    }

    m_bWtDefaulPalette = true;
  }
}

class XamlResourceInputStream;

class OpcHelper : public WT_OpcResourceSerializer, public WT_OpcResourceMaterializer
{
public:
  OpcHelper();
  virtual ~OpcHelper();
  void setCurrentSection( DWFSection* pSection);

  WT_Result getPartInputStream(
    const DWFCore::DWFString& rzPath, 
    DWFCore::DWFInputStream** ppStream);
  WT_Result getPartOutputStream(
    const DWFCore::DWFString& rzMimeType,
    DWFCore::DWFString& rzPath,
    DWFCore::DWFOutputStream** ppStream);
  WT_Result getSubsetFontName(
    OdString& zFontName,
    WT_Integer32 nHeight,
    const OdString&zStr,
    bool  bForEmbeddedFontObject = false);

private:
  int         _nPartCount;
  DWFSection* _pSection;
  DWFUUID     _oUuidGen;
  DWFString   _zNextOutputStreamPath;
  DWFBufferOutputStream* _pStreamCopy;

  typedef std::map<DWFString, XamlResourceInputStream*>  _tPathToStreamMap;
  _tPathToStreamMap _oMap;

  void _addGfxRelationship(const DWFString& zRel, DWFResource* pTarget );
}
gHelper;


static WT_Result WtStreamOpenAction(WT_File & ){ return WT_Result::Success;}
static WT_Result WtStreamCloseAction(WT_File & file)
{
  delete (DWFOutputStream*)file.stream_user_data();
  return WT_Result::Success;
}
static WT_Result WtStreamSeekAction(WT_File&, int , int&)
{
  ODA_FAIL_ONCE();
  return WT_Result::Success;
}
static WT_Result WTStreamReadAction(WT_File & file,int bytes_desired,int & bytes_read,void * buffer)
{
  ODA_FAIL_ONCE();
  return WT_Result::Success;
}

static WT_Result WTStreamTellAction(WT_File & , unsigned long*)
{
  ODA_FAIL_ONCE();
  return WT_Result::Success;
}

static WT_Result WTStreamWriteAction(WT_File& file, int count,void const* buffer)
{
  DWFOutputStream* stream = (DWFOutputStream*)file.stream_user_data();
  stream->write(buffer, count);
  return WT_Result::Success;
}
static WT_Result WtStreamEndSeekAction(WT_File &)
{
  ODA_FAIL_ONCE();
  return WT_Result::Success;
}

void CDwfExportImpl::openWtFile(const DwfLayoutInfo& layoutInfo)
{
  ODA_ASSERT(!m_wtFile);
  m_wtFile = m_pDeviceDwf->classFactory()->Create_File();
  m_wtFile->set_file_mode(WT_File::File_Write);
  if (m_Params.format() == DW_XPS)
  {
    WT_XAML_File *pxFile = static_cast<WT_XAML_File *>(m_wtFile);
    pxFile->xamlStreamOut() = layoutInfo.pOutStream;
    pxFile->w2xStreamOut() = layoutInfo.pOutStreamExt;
    pxFile->opcResourceSerializer() = &gHelper;
    pxFile->opcResourceMaterializer() = &gHelper;
    pxFile->obfuscate_embedded_fonts() = false;

    // page layout should be set before the first element is written
    WT_Matrix2D page_transform;
    page_transform(2,0) = layoutInfo.m_PlottingData.xps_page_offset.x;
    page_transform(2,1) = layoutInfo.m_PlottingData.xps_page_offset.y;
    pxFile->definePageLayout(page_transform, m_rectOutput.right - m_rectOutput.left,
      fabs((double)m_rectOutput.top - m_rectOutput.bottom), layoutInfo.m_PlottingData.xps_scale);
  }
  else
  {
    m_wtFile->set_stream_user_data(layoutInfo.pOutStream);
    m_wtFile->set_stream_open_action(WtStreamOpenAction);
    m_wtFile->set_stream_close_action(WtStreamCloseAction);
    m_wtFile->set_stream_seek_action(WtStreamSeekAction);
    m_wtFile->set_stream_end_seek_action(WtStreamEndSeekAction);
    m_wtFile->set_stream_read_action(WTStreamReadAction);
    m_wtFile->set_stream_tell_action(WTStreamTellAction);
    m_wtFile->set_stream_write_action(WTStreamWriteAction);
    m_wtFile->heuristics().set_target_version(m_Params.version());
    setW2dCompression();
  }
  m_wtFile->open();
}



//
// createDevice()
//
//  Create & set own Dwf Device Create; create & activate own Dwf View.
//
void CDwfExportImpl::createDevice()
{
  // for previous DWF versions only (earlier then v06.00)

  if (isDwfVersion6())
  {
    ODA_ASSERT(false);
  }

  m_pDeviceDwf->setContext(this);  //  set special context for dwf device
  // Note: to call Own Device methods before ::odgsSetupActiveLayoutViews() not after!
  //       Only Base Interface Methords could be called after ::odgsSetupActiveLayoutViews().

  //  SK  05.02.03
  OdGiDefaultContextPtr pCtx = (OdDbBaseDatabasePEPtr(m_Params.database()))->createGiContext(m_Params.database());//OdGiDefaultContext::createObject(); // dgn dwf export
  pCtx->setPlotGeneration(true);
  OdDbBaseDatabasePEPtr(m_Params.database())->loadPlotstyleTableForActiveLayout(pCtx, m_Params.database());

  // if the layout was set by user to setup this one
  //  otherwise to setup the active layout
  OdString sLayout;
  if (m_Params.pageData().size())
  {
    sLayout = m_Params.pageData()[0].sLayout;
  }
  m_pDevice = (!sLayout.isEmpty() && !(OdDbBaseDatabasePEPtr(m_Params.database()))->findLayoutNamed(m_Params.database(), sLayout) == 0)
      ? (OdDbBaseDatabasePEPtr(m_Params.database()))->setupLayoutView(m_pDeviceDwf, pCtx, OdDbBaseDatabasePEPtr(m_Params.database())->findLayoutNamed(m_Params.database(), sLayout))
      : (OdDbBaseDatabasePEPtr(m_Params.database()))->setupActiveLayoutViews(m_pDeviceDwf, pCtx);

  // NB: duplicated code, needs to be rafactored
  m_pDeviceDwf->setRenderDevice();
  for (int nDevice = 0; nDevice < m_pDeviceDwf->numRenderDevices(); nDevice++)
  {
    OdGsDevice* pRenderDevice = m_pDeviceDwf->renderDeviceAt(nDevice);
    pRenderDevice->setBackgroundColor(m_Params.background());
    pRenderDevice->setLogicalPalette(m_pPalette, 256);
    OdGsDevicePtr pLayoutHelper = (!sLayout.isEmpty() && !(OdDbBaseDatabasePEPtr(m_Params.database()))->findLayoutNamed(m_Params.database(), sLayout) == 0)
      ? (OdDbBaseDatabasePEPtr(m_Params.database()))->setupLayoutView(pRenderDevice, pCtx, OdDbBaseDatabasePEPtr(m_Params.database())->findLayoutNamed(m_Params.database(), sLayout))
      : (OdDbBaseDatabasePEPtr(m_Params.database()))->setupActiveLayoutViews(pRenderDevice, pCtx);
    m_pDeviceDwf->setRenderLayoutHelperAt(nDevice, pLayoutHelper.get());
  }

  m_pDeviceDwf->setBackgroundColor(m_Params.background());
  m_pDeviceDwf->setLogicalPalette(m_pPalette, 256);

  OdGsDCRect outputRect(
      OdGsDCPoint(m_rectOutput.left, m_rectOutput.bottom),
      OdGsDCPoint(m_rectOutput.right, m_rectOutput.top)
    );

  m_pDeviceDwf->onSize(outputRect);
}


//***************************************************************************
//
void CDwfExportImpl::end()
{
  // for previous DWF versions only (earlier then v06.00)

  if (isDwfVersion6())
  {
    ODA_ASSERT(false);
  }

  m_wtFile->close();
}


//***************************************************************************
//
// draw()
//
//***************************************************************************
void CDwfExportImpl::draw(const DwfLayoutInfo& li)
{
  nonprocessRun(li);

  putInkedArea(li);

  if (m_Params.format() != DW_XPS)
    putDrawingInfo();           // Puts the Drawing Info into the file 

  putDrawingUnits();          // Defines Drawing Units 
  putEmbed();                 // Embed Source File 
  putViewInfo(li);
  putPlotInfo(li);  // todo new // Define Plot Info
  putColorMap();              // Puts the list of colors (a map)
  putBackground();            // Defines Drawing Background
  m_pDeviceDwf->update(0, OdDwfDevice::Main_run);
}


///////////////////////////////////////////////////////////////////////////////
// Auxiliary methods
///////////////////////////////////////////////////////////////////////////////
void CDwfExportImpl::collectSymbols(const OdString typeface, const OdChar* msg, int length)
{
  FontSubsetContainer::iterator pi = m_xpsFontMap.find(typeface);
  if (pi == m_xpsFontMap.end())
  {
    pi = m_xpsFontMap.insert(FontSubsetContainer::value_type(typeface, std::set<OdChar>())).first;
  }
  std::set<OdChar>& s = pi->second;
  for (int i = 0; i < length; ++i)
    s.insert(msg[i]);
}
//***************************************************************************
//
// nonprocessRun()
//
//  To optimize a presetting by nonprocess run (draw)
//***************************************************************************
void CDwfExportImpl::nonprocessRun(const DwfLayoutInfo& li)
{
  m_xpsFontMap.clear();

  if (m_bWtDefaulPalette)
    return;

  if (m_Params.colorMapOptimize() || m_Params.format() == DW_XPS || m_Params.embedAllFonts() || (li.m_iParamIndex < (int)m_Params.pageData().size() && !m_Params.pageData()[li.m_iParamIndex].arFonts.empty()))
  {
    m_pColors.resize(1);
    m_pColors[0] = WT_RGBA32(ODGETRED(m_Params.background()), 
                             ODGETGREEN(m_Params.background()), 
                             ODGETBLUE(m_Params.background()));

    m_pDeviceDwf->update(0, OdDwfDevice::Nonprocess_run);
    m_pDeviceDwf->invalidate();
    if (m_Params.format() == DW_XPS)
    {
      for (FontSubsetContainer::iterator pfs = m_xpsFontMap.begin(); pfs != m_xpsFontMap.end(); ++pfs)
      {
        std::vector<OdChar> msg(pfs->second.size()+1);
        std::copy(pfs->second.begin(), pfs->second.end(), msg.begin());
        msg[msg.size() - 1] = 0;
        registerFontForXPS(&msg[0], pfs->first);
      }
    }
    m_colorIndexMap.clear();
    for (unsigned int i = 0; i < m_pColors.size(); i++)
    {
      WT_RGBA32 wt_color = m_pColors[i];
      ODCOLORREF rgb(ODRGB(wt_color.m_rgb.r, wt_color.m_rgb.g, wt_color.m_rgb.b));
      m_colorIndexMap[rgb] = i;
    }
  }
  else
  {
    // To set off the color map optimization and to export whole color map always
    m_colorIndexMap.clear();

    ODA_ASSERT(m_pPalette);
    if (!m_pPalette)    
    {
      ODA_FAIL();
      throw OdError(eNullObjectPointer);
    }

    m_pColors.resize(256);
    for (int i = 0; i < 256; i++)
    {
      ODCOLORREF colorFromUser = m_pPalette[i];
      WT_RGBA32 wt_color(ODGETRED(colorFromUser), ODGETGREEN(colorFromUser), ODGETBLUE(colorFromUser));
      m_pColors[i] = wt_color;
      m_colorIndexMap[colorFromUser] = i;
    }
  }
}

void CDwfExportImpl::colorPresetting(ODCOLORREF color)
{
  if (m_pColors.size() >= 255)
    return;
  WT_RGBA32 wt_color(ODGETRED(color), ODGETGREEN(color), ODGETBLUE(color));
  if (!m_pColors.contains(wt_color))
    m_pColors.append(wt_color);
}


//***************************************************************************
//
// getColorIndex()
//
//  To get color indext at active palette
//
//***************************************************************************
bool CDwfExportImpl::getWtColorIndex(const ODCOLORREF rgb, int& color_index) const
{
  bool bRet(true);
  
  colorIndexMap::const_iterator it;
  it = m_colorIndexMap.find(rgb);
  if (it != m_colorIndexMap.end())
  {
    color_index = (*it).second;
  }
  else
  {
    color_index = getWtDefaultColorIndex();
    bRet = false;
  }
  
  return bRet;
}

/////////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------

//
// putInkedArea()
//
//  The Inked Area opcode specifies a rectangle, in logical coordinates, 
//  in which all of the .dwf file’s geometric primitives fit tightly inside.
//
void CDwfExportImpl::putInkedArea(const DwfLayoutInfo& li)
{
  // To get extents of active view & transform them
  OdGsView* view = m_pDeviceDwf->viewAt(0);
  OdGeExtents3d extents;
  OdRxObjectPtr pL = (OdDbBaseDatabasePEPtr(m_Params.database()))->currentLayout(m_Params.database());
  OdDbBaseLayoutPEPtr pLayout(pL);
  if (pLayout->getGeomExtents(pL.get(), extents) != eOk)
  {
    return;
  }

  extents.transformBy(view->objectToDeviceMatrix());

  OdGePoint3d ptMin, ptMax;
  ptMin = extents.minPoint(); 
  ptMax = extents.maxPoint(); 

  if (m_Params.inkedArea())    // MKU 05/31/2005 - don't miss m_bLandscape
  {
    WT_Logical_Point bounds[4];
    boxAnticlockwise(bounds, ptMin, ptMax);   // To set a direction of box detour clockwise
  
    WT_Inked_Area inkedArea;
    inkedArea.set_inked_area(bounds);
    inkedArea.serialize(*m_wtFile);
  }
}

//
// putDrawingInfo()
//
//  Put into the file Drawing Info
//
void CDwfExportImpl::putDrawingInfo()
{
  // Set Drawing Info
  //

  bool bPageDataExists = (m_Params.pageData().size() == 0) ? false : true;
  DwfPageData pageData;
  if (bPageDataExists)
  {
    pageData = m_Params.pageData()[ 0 ];
  }

  if (bPageDataExists && !pageData.sPageDescription.isEmpty())
  {
    m_wtFile->desired_rendition().drawing_info().description().set(OdW2US(pageData.sPageDescription.c_str()));
  }

  if (bPageDataExists && !pageData.sPageAuthor.isEmpty())
  {
    m_wtFile->desired_rendition().drawing_info().author().set(OdW2US(pageData.sPageAuthor.c_str()));
  }

  if (bPageDataExists && !pageData.sPageComments.isEmpty())
  {
    m_wtFile->desired_rendition().drawing_info().comments().set(OdW2US(pageData.sPageComments.c_str()));
  }

  if (bPageDataExists && !pageData.sPageCopyright.isEmpty())
  {
    m_wtFile->desired_rendition().drawing_info().copyright().set(OdW2US(pageData.sPageCopyright.c_str()));
  }
/*
  if (!pageData.sCreator.isEmpty())
  {
    m_wtFile->desired_rendition().drawing_info().creator().set(OdW2US(m_Params.sCreator.c_str()));
  }
  else
  {
*/
    m_wtFile->desired_rendition().drawing_info().creator().set(
    #ifdef WIN32
        OdW2US(OD_T("Open Design DWF Export - WIN32"))
    #elif defined(ANDROID)
      OdW2US(OD_T("Open Design DWF Export - Android"))
    #elif defined(__linux__)
        OdW2US(OD_T("Open Design DWF Export - Linux"))
    #elif defined(sparc)
        OdW2US(OD_T("Open Design DWF Export - Sparc"))
    #elif defined(__sun)
        OdW2US(OD_T("Open Design DWF Export - Solaris"))
    #elif defined(sgi)
        OdW2US(OD_T("Open Design DWF Export - SGI"))
    #elif defined(__hpux)
        OdW2US(OD_T("Open Design DWF Export - HP"))
    #elif defined(_AIX)
        OdW2US(OD_T("Open Design DWF Export - AIX"))
    #elif defined(__MWERKS__) || (defined(__APPLE__) && defined(__GNUC__))
        OdW2US(OD_T("Open Design DWF Export - Mac"))
    #elif defined(EMCC)
        OdW2US(OD_T("Open Design DWF Export - Emscripten"))
    #endif
    );
//  }

  if (bPageDataExists && !pageData.sPageKeywords.isEmpty())
  {
    m_wtFile->desired_rendition().drawing_info().keywords().set(OdW2US(pageData.sPageKeywords.c_str()));
  }

  if (bPageDataExists && !pageData.sPageTitle.isEmpty())
  {
    m_wtFile->desired_rendition().drawing_info().title().set(OdW2US(pageData.sPageTitle.c_str()));
  }

  if (bPageDataExists && !pageData.sPageSubject.isEmpty())
  {
    m_wtFile->desired_rendition().drawing_info().subject().set(OdW2US(pageData.sPageSubject.c_str()));
  }
/*
  if (!m_Params.sSourceFileName.isEmpty())
  {
    m_wtFile->desired_rendition().drawing_info().source_filename().set(OdW2US(m_Params.sSourceFileName.c_str()));
  }
*/
  m_wtFile->desired_rendition().drawing_info().source_filename().set(OdW2US((OdDbBaseDatabasePEPtr(m_Params.database()))->getFilename(m_Params.database()).c_str()));

  // Creation and Modification time
  OdString locTimeStr;

  OdTimeStamp locTime(OdTimeStamp::kInitLocalTime);
  locTime.strftime(DWF_DATE_FORMAT_STR, locTimeStr);
  WT_Integer32 ltime = (WT_Integer32)locTime.packedValue();

  m_wtFile->desired_rendition().drawing_info().creation_time().set(ltime, OdW2US(locTimeStr.c_str()));
  m_wtFile->desired_rendition().drawing_info().modification_time().set(ltime, OdW2US(locTimeStr.c_str()));

  // Drawing Creation time
  locTime = (OdDbBaseDatabasePEPtr(m_Params.database()))->getCreationTime(m_Params.database()); // dgn dwf export
  ltime = (WT_Integer32)locTime.packedValue();
  locTime.strftime(DWF_DATE_FORMAT_STR, locTimeStr);

  m_wtFile->desired_rendition().drawing_info().source_creation_time().set(ltime, OdW2US(locTimeStr.c_str()));
  m_wtFile->desired_rendition().drawing_info().source_creation_time().
                                                    set_guid(WT_String(OdW2US((OdDbBaseDatabasePEPtr(m_Params.database()))->getFingerPrintGuid(m_Params.database()).c_str()))); // dgn dwf export

  // Drawing Modification time
  locTime = (OdDbBaseDatabasePEPtr(m_Params.database()))->getUpdateTime(m_Params.database()); // dgn dwf export
  locTime.universalToLocal(); 
  ltime = (WT_Integer32)locTime.packedValue();
  locTime.strftime(DWF_DATE_FORMAT_STR, locTimeStr);

  m_wtFile->desired_rendition().drawing_info().source_modification_time().set(ltime, OdW2US(locTimeStr.c_str()));
  m_wtFile->desired_rendition().drawing_info().source_modification_time().
                                                    //set_guid(WT_String(OdW2US(m_Params.database()->getVERSIONGUID().c_str()))); // before 04.01.2012
                                                    set_guid(WT_String(OdW2US((OdDbBaseDatabasePEPtr(m_Params.database()))->getVersionGuid(m_Params.database()).c_str()))); // dgn dwf export

  // Put Drawing Info
  m_wtFile->desired_rendition().drawing_info().sync(*m_wtFile);
}


//
// putDrawingUnits()
//
//  The Define Drawing Units function describes the real-world scale of the DWF’s logical coordinates.
//
void CDwfExportImpl::putDrawingUnits()
{
  double xform[16];

  ((OdDwfView*)m_pDeviceDwf->viewAt(0))->convDeviceMatrixToWt(xform);

  std::auto_ptr<WT_Units> units(m_pDeviceDwf->classFactory()->Create_Units());
  units->set_application_to_dwf_transform( WT_Matrix(xform) );
  units->set_units( getUnitNameString( (OdDbBaseDatabasePEPtr(m_Params.database()))->getUnits(m_Params.database()) ) ); // dgn dwf export

  units->serialize(*m_wtFile);
}

//
// putEmbed()
//
//  The Embed Source File function allows the source file from which the DWF was created 
//  to be embedded (or referenced) inside the DWF, thus allowing smart drag-and-drop operations.
//
void CDwfExportImpl::putEmbed()
{
  std::auto_ptr<WT_Embed> Embed(m_pDeviceDwf->classFactory()->Create_Embed());
  Embed->set_whole_MIME("image/vnd.dwg", *m_wtFile);
  if (m_wtFile->desired_rendition().drawing_info().description().string().length())
  {
    Embed->set_description(m_wtFile->desired_rendition().drawing_info().description().string(), *m_wtFile);
  }
  else
  {
    Embed->set_description(" ", *m_wtFile);  // AutoDesk's viewer do not like it empty
  }
  Embed->set_filename(OdW2US((OdDbBaseDatabasePEPtr(m_Params.database()))->getFilename(m_Params.database()).c_str()), *m_wtFile);
  Embed->serialize(*m_wtFile);
}

//
// putViewInfo()
//
void CDwfExportImpl::putViewInfo(const DwfLayoutInfo& layoutInfo)
{
  OdGsView* view = m_pDeviceDwf->viewAt(0);

  putView(view);

  if (!m_Params.forceInitialViewToExtents())
  {
    putView(view, OD_T("INITIAL"));   //  to get active view as initial
  }
  else
  {
    OdDbBaseDatabasePEPtr pDbPE(m_Params.database());
    OdRxObjectPtr pL = pDbPE->getFirstLayout(m_Params.database());
    OdDbBaseLayoutPEPtr pLayout(pL);
    OdGeExtents3d extents;
    if (layoutInfo.m_idLayout == pDbPE->getId(pL.get()) && eOk == pLayout->getGeomExtents(pL.get(), extents))
    {
      extents.transformBy(view->objectToDeviceMatrix());
      putView(OD_T("INITIAL"), extents.minPoint(), extents.maxPoint());
    }
  }

  // Named View
  OdStringArray names;
  OdGePoint3dArray points;
  (OdDbBaseDatabasePEPtr(m_Params.database()))->putNamedViewInfo(m_Params.database(), layoutInfo.m_idLayout, names, points);
  int nCount = names.size();
  for (int i = 0; i < nCount; i++)
  {
      points[2*i].transformBy(view->objectToDeviceMatrix());
      points[2*i+1].transformBy(view->objectToDeviceMatrix());

      putView( names[i].c_str(), points[2*i], points[2*i+1] );
  }
}

//
// putView() 
//
void CDwfExportImpl::putView(OdGsView* gsView, const OdChar* Name)
{
  WT_Logical_Point wtMin;
  WT_Logical_Point wtMax;

  ((OdDwfView*)gsView)->convViewportToWt(/*gsView, */wtMin, wtMax);

  WT_Logical_Box wtBox( wtMin, wtMax );
  if (Name != NULL)
  {
    std::auto_ptr<WT_Named_View> View(m_pDeviceDwf->classFactory()->Create_Named_View(wtBox, OdW2US(Name)));
    View->serialize(*m_wtFile);
  } 
  else
  {
    std::auto_ptr<WT_View> View(m_pDeviceDwf->classFactory()->Create_View(wtBox));
    View->serialize(*m_wtFile);
  }
}


void CDwfExportImpl::putView(const OdChar* Name, const OdGePoint3d& min, const OdGePoint3d& max)
{
  WT_Logical_Point wtMin;
  WT_Logical_Point wtMax;

  RoundPoint(wtMin, OdGePoint2d(min.x, min.y));
  RoundPoint(wtMax, OdGePoint2d(max.x, max.y));

  WT_Logical_Box wtBox( wtMin, wtMax );

  std::auto_ptr<WT_Named_View> View(m_pDeviceDwf->classFactory()->Create_Named_View(wtBox, OdW2US(Name)));
  View->serialize(*m_wtFile);
}

//
// putPlotInfo()
//
//  The Define Plot Info function describes the real-world plotting of the DWF’s logical coordinates.
//
void CDwfExportImpl::putPlotInfo(const DwfLayoutInfo& li)
{
  if (m_Params.format() == DW_XPS
    || m_Params.version() < nDwf_v60) return;
  WT_Plot_Info plotInfo;
  plotInfo.for_display() = li.m_sLayoutName != L"Model";
  plotInfo.lower_left() = WT_Point2D(li.m_PlotInfo.m_dPrintableXmin, li.m_PlotInfo.m_dPrintableYmin);
  plotInfo.upper_right() = WT_Point2D(li.m_PlotInfo.m_dPrintableXmax, li.m_PlotInfo.m_dPrintableYmax);
  plotInfo.paper_height() = li.m_PlotInfo.m_dPaperHeight;
  plotInfo.paper_width() = li.m_PlotInfo.m_dPaperWidth;
  plotInfo.paper_units() = li.m_PlotInfo.m_PlotPaperUnit == DwfLayoutPlotInfo::kMillimeters ? WT_Plot_Info::Millimeters : WT_Plot_Info::Inches;
  WT_Matrix2D toPaper;
  for (int i = 0; i < 2; ++i)
  {
    for (int j = 0; j < 2; ++j)
    {
      toPaper(i, j) = li.m_PlottingData.m_pTransform[i*4 +j];
    }
  }
  toPaper(2, 0) = li.m_PlottingData.m_pTransform[3*4];
  toPaper(2, 1) = li.m_PlottingData.m_pTransform[3*4+1];
  plotInfo.to_paper() = toPaper;
  plotInfo.serialize(*m_wtFile);
}

//
// putBackground()
//
//  The Define Drawing Background function specifies how the DWF reading application should render 
//  the background behind the geometric primitives to be drawn.
//
void CDwfExportImpl::putBackground()
{
  // Background color
  if (m_Params.version() >= nDwf_v60)
  {
    // since version 6.0 Background opcode was deprecated
    return;
  }

  if (m_Params.version() == nDwf_v42)
  {
    if ( ODGETRED(m_Params.background()) == 0 
      && ODGETGREEN(m_Params.background()) == 0 
      && ODGETBLUE(m_Params.background()) == 0
      )
    { 
      // For some reason zero background is not accepted by AutoDesk's viewer
      m_wtFile->desired_rendition().rendering_options().background().set(WT_Color(1, 0, 0));
    } 
    else 
    {
      m_wtFile->desired_rendition().rendering_options().background().set(WT_Color(
              ODGETRED(m_Params.background()),
              ODGETGREEN(m_Params.background()),
              ODGETBLUE(m_Params.background())));
    }  
    m_wtFile->desired_rendition().rendering_options().sync(*m_wtFile);
  }
  else if (m_Params.version() == nDwf_v55)
  {
    int color_index;
    WT_Background background;
    if (getWtColorIndex(m_Params.background(), color_index))
    {
      background.set(WT_Color (color_index, m_wtFile->desired_rendition().color_map()));
    }
    else
    {
      background.set(WT_Color (ODGETRED(m_Params.background()), 
                                          ODGETGREEN(m_Params.background()), 
                                          ODGETBLUE(m_Params.background())));
    }
    background.serialize(*m_wtFile);
  }
  else
  {
    ODA_ASSERT(false);
    throw OdError(eIncorrectInitFileVersion);
  }
}


//
// putColorMap()
//
//  A list of colors (a map) to be used when displaying images
//
void CDwfExportImpl::putColorMap()
{
  if (m_bWtDefaulPalette || m_pColors.isEmpty())   
    return;
  m_wtFile->desired_rendition().color_map().set(m_pColors.size(), m_pColors.asArrayPtr(), *m_wtFile);
}




//***************************************************************************
//
// writingDwf6Files()
//
//  A few steps to creating a basic DWF 6 file: 
//    - Create an DWFEPlot::EPlotSectionManager;
//    - Create a DWFWriter::DwfWriter object;
//    - Create a DWFEPlot::EPlotPageDescriptor which will be used to define a page in the DWF 6 file;
//    - Add resources to the page;
//    - Destroy the page descriptor, dwf writer, and section manager, which causes the file to be written 
//      with the manifest and descriptor files in it;
//    - Optimize the file.
//
//***************************************************************************


void CDwfExportImpl::addThumbnail( DwfPageData &pageData, const DWFString& dwf_zSectionTitle, 
                                  DWFToolkit::DWFEPlotSection* pPage, DWFToolkit::DWFGraphicResource* p2Dgfx)
{
  if (!pageData.thumbnail.sFileName.isEmpty())
  {
    // now let's add the thumbnail
    //
    // To specify the image file resource's MIME-type
    DWFString zMimeType;
    if (SpecifyImageMimeType(zMimeType, pageData.thumbnail.sFileName))
    {
      DWFToolkit::DWFImageResource* pThumbnail = 
        DWFCORE_ALLOC_OBJECT( DWFImageResource( dwf_zSectionTitle,    // The friendly/display name of the resource
        DWFXML::kzRole_Thumbnail,
        zMimeType) );

      if (pThumbnail == NULL)
      {
        _DWFCORE_THROW( DWFMemoryException, L"Failed to allocate resource" );
      }

      //
      // configure the resource
      //
      double anTransform2[4][4] = { {1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1} };

      double anExtents2[4];           // left bottom right top
      anExtents2[0] = 0;
      anExtents2[1] = 0;
      anExtents2[2] = pageData.thumbnail.Width;
      anExtents2[3] = pageData.thumbnail.Height;

      pThumbnail->configureGraphic( (const double*)anTransform2,
        (const double*)anExtents2 );

      pThumbnail->configureImage( (unsigned char)pageData.thumbnail.ColorDepth );   // the number of colors used in the image

      DWFCore::DWFFile oThumbnailFilename( pageData.thumbnail.sFileName.c_str() );
      DWFCore::DWFStreamFileDescriptor* pThumbnailFile = DWFCORE_ALLOC_OBJECT( DWFStreamFileDescriptor(oThumbnailFilename, L"rb") );

      if (pThumbnailFile == NULL)
      {
        DWFCORE_FREE_OBJECT( pThumbnail );

        _DWFCORE_THROW( DWFMemoryException, L"Failed to allocate file descriptor" );
      }

      DWFCore::DWFFileInputStream* pThumbnailFilestream = DWFCORE_ALLOC_OBJECT( DWFFileInputStream );

      if (pThumbnailFilestream == NULL)
      {
        DWFCORE_FREE_OBJECT( pThumbnail );
        DWFCORE_FREE_OBJECT( pThumbnailFile );

        _DWFCORE_THROW( DWFMemoryException, L"Failed to allocate file stream" );
      }

      pThumbnailFile->open();
      pThumbnailFilestream->attach( pThumbnailFile, true );

      pThumbnail->setInputStream( pThumbnailFilestream );

      // drop the resource into the page
      // and be sure to mention that it "belongs" to the W2D 
      // (and replace and delete)
      //
      pThumbnail->enableCustomZipMode(true, DWFZipFileDescriptor::eZipSmallest);
      pPage->addResource( pThumbnail, true, true, true, p2Dgfx );
    }
  }
}
class OpcHelper;

class XamlResourceInputStream : public DWFCore::DWFInputStream
{
  friend class OpcHelper;

public:
  XamlResourceInputStream()
    : _pInputStream( NULL )
    , _pBuffer( NULL )
    , _nBufferBytes( 0 )
  {
    ;
  }

  XamlResourceInputStream( XamlResourceInputStream& rSource)
  {
    _nBufferBytes = rSource.available();
    _pBuffer = DWFCORE_ALLOC_MEMORY( unsigned char, _nBufferBytes );
    rSource.read( _pBuffer, _nBufferBytes );
    rSource.seek( SEEK_SET, 0 );
    _pInputStream = DWFCORE_ALLOC_OBJECT(DWFBufferInputStream( _pBuffer, _nBufferBytes ));
  }
  virtual ~XamlResourceInputStream()throw()
  {
    DWFCORE_FREE_MEMORY( _pBuffer );
    DWFCORE_FREE_OBJECT( _pInputStream );
  }

  void provideBytes( const void* pBuffer, size_t nBufferBytes)
  {
    if (!pBuffer)
      return;
    if (_pBuffer)
    {
      _nBufferBytes = 0;
      DWFCORE_FREE_MEMORY( _pBuffer );
    }

    _nBufferBytes = nBufferBytes;
    _pBuffer = DWFCORE_ALLOC_MEMORY( unsigned char, _nBufferBytes );
    DWFCORE_COPY_MEMORY( _pBuffer, pBuffer, _nBufferBytes );
    if (_pInputStream)
    {
      DWFCORE_FREE_OBJECT( _pInputStream );
    }
    _pInputStream = DWFCORE_ALLOC_OBJECT(DWFBufferInputStream( _pBuffer, _nBufferBytes ));
  }

  virtual size_t available() const
    throw( DWFException )
  {
    if (_pInputStream == NULL)
    {
      return 0;
    }
    else
    {
      return _pInputStream->available();
    }
  }

  virtual size_t read( void*  pBuffer,
    size_t nBytesToRead )
    throw( DWFException )
  {
    if (_pInputStream == NULL)
    {
      return 0;
    }
    else
    {
      return _pInputStream->read( pBuffer, nBytesToRead );
    }
  }

  virtual off_t seek( int    eOrigin,
    off_t  nOffset )
    throw( DWFException )
  {
    if (_pInputStream == NULL)
    {
      return 0;
    }
    else
    {
      return _pInputStream->seek( eOrigin, nOffset );
    }
  }

private:
  DWFBufferInputStream* _pInputStream;
  unsigned char*        _pBuffer;
  size_t                _nBufferBytes;
};

class XamlResourceOutputStream : public DWFCore::DWFBufferOutputStream
{
public:
  XamlResourceOutputStream( XamlResourceInputStream* pInputStream)
    : DWFBufferOutputStream( 1024 )
    , _pInputStream( pInputStream )
  {
    ;
  }

  virtual ~XamlResourceOutputStream()throw()
  {
    _pInputStream->provideBytes( buffer(), bytes() );
  }

private:
  XamlResourceInputStream* _pInputStream;
};

DWFToolkit::DWFEPlotSection* CDwfExportImpl::createPlotSection(DwfLayoutInfo& layoutInfo)
{
  bool        bPageDataExists(false);
  DwfPageData pageData;

  // If the page data exists to get them
  if (layoutInfo.m_iParamIndex < (int)m_Params.pageData().size())
  {
    bPageDataExists = true;
    pageData = m_Params.pageData()[ layoutInfo.m_iParamIndex ];
  }
  else
  {
    bPageDataExists = false;
  }

  // It should provide the actual source Filename from which all of the resources 
  //  on this page are generated or in which they are somehow contained.
  DWFString zSourceFilename((OdDbBaseDatabasePEPtr(m_Params.database()))->getFilename(m_Params.database()).c_str());
  DWFToolkit::DWFSource oSource( zSourceFilename, L"Open Design DWF Export", L"" );

  DWFString dwf_zSectionTitle;                      // Section title
  if (bPageDataExists && !pageData.sPageTitle.isEmpty())
  {
    dwf_zSectionTitle = pageData.sPageTitle.c_str();
  }
  else if (bPageDataExists && !pageData.sLayout.isEmpty())
  {
    dwf_zSectionTitle = pageData.sLayout.c_str();
  }
  else
  {
    dwf_zSectionTitle = layoutInfo.m_sLayoutName.c_str();
  }

  if (!setPlottingSpace(layoutInfo))
    return 0;

  DWFToolkit::DWFPaper oPaper( layoutInfo.m_PlottingData.m_dWidth,
    layoutInfo.m_PlottingData.m_dHeight,
    layoutInfo.m_PlottingData.m_eUnits /*DWFPaper::eInches*/,
    layoutInfo.m_PlottingData.m_nColorARGB,
    (const double*)layoutInfo.m_PlottingData.m_pPaperClip,
    layoutInfo.m_PlottingData.m_bShow );

  DWFToolkit::DWFEPlotSection* pPage = new DWFToolkit::DWFEPlotSection(dwf_zSectionTitle,
    dwf_zSectionTitle,
    // Uniquely identifies the section
    // (for that the internal W2D stream name is used)
    layoutInfo.m_iParamIndex + 1,  // Indicates the initial position in the package 
    //  to which the section was/will be published.
    //  As default to use publishing number.
    oSource,    // Describes the original source of the data 
    //  in the section (a drawing file)
    0x00ff00ff, // Deprecated. Not used
    &oPaper ); // Defines the virtual paper onto which 
  //  the 2D section graphics will be placed.
  gHelper.setCurrentSection(pPage);
  addMetadata(bPageDataExists, pageData, pPage);
  
  DWFToolkit::DWFGraphicResource* p2Dgfx = new DWFGraphicResource(pPage->title(),
    DWFXML::kzRole_Graphics2d,           // role
    m_Params.format() == DW_XPS ? DWFMIME::kzMIMEType_FIXEDPAGEXML : DWFMIME::kzMIMEType_W2D,             // MIME type
    L"",                                 // author
    L"",                                 // description
    L"",                                 // creation time
    L"");                              // modification time // TODO:

  p2Dgfx->configureGraphic( layoutInfo.m_PlottingData.m_pTransform, NULL, layoutInfo.m_PlottingData.m_pClip );

  XamlResourceInputStream* pXamlResourceInputStream = DWFCORE_ALLOC_OBJECT( XamlResourceInputStream() );
  layoutInfo.pOutStream = new XamlResourceOutputStream(pXamlResourceInputStream);
  p2Dgfx->setInputStream( pXamlResourceInputStream );
  if (m_Params.format() != DW_XPS)
    p2Dgfx->enableCustomZipMode(true, DWFZipFileDescriptor::eZipSmallest);
  pPage->addResource( p2Dgfx, true );
  if (m_Params.format() == DW_XPS)
  {
    XamlResourceInputStream* pW2XInputStream = new XamlResourceInputStream();
    layoutInfo.pOutStreamExt = new XamlResourceOutputStream(pW2XInputStream);
    DWFResource* pW2XExtensionResource = new DWFResource(L"W2X Resource",
      DWFXML::kzRole_Graphics2dExtension, DWFMIME::kzMIMEType_XML);
    pW2XExtensionResource->setInputStream( pW2XInputStream );
    pPage->addResource( pW2XExtensionResource, true );
    p2Dgfx->addRelationship(pW2XExtensionResource, DWFXXML::kzRelationship_Graphics2dExtensionResource);
  }
  else if (bPageDataExists)
  {
    addThumbnail(pageData, pPage->title(), pPage, p2Dgfx);
    addPreview(pageData, pPage->title(), pPage, p2Dgfx);
  }
  return pPage;
}

//-----------------------------------------------------------------

//
// getPlotPaperSize()
//
//  get paper size for current view
void CDwfExportImpl::getPlotPaperSize(const DwfLayoutInfo& li, double& plotWidth, double& plotHeight )
{
  if (isDwfVersion6())
  {
    plotWidth = li.m_PlotInfo.m_dPaperWidth;
    plotHeight = li.m_PlotInfo.m_dPaperHeight;
  }
  else
  {
    plotWidth  = 215.89999389648;
    plotHeight = 279.39999389648;
  }
}

//-----------------------------------------------------------------

//
// setPlottingSpace()
//
//  Function describes the real-world plotting. 
//  Sets information relating the WHIP! drawing coordinate space to plotting (paper) space.
//  Sets the transform of the resource.
//

bool CDwfExportImpl::setPlottingSpace(DwfLayoutInfo& layoutInfo)
{
  DwfLayoutPlotInfo& plotInfo = layoutInfo.m_PlotInfo;
  DataForDWFPlotting* plotting = &layoutInfo.m_PlottingData;
  OdRxObjectPtr pL = (OdDbBaseDatabasePEPtr(m_Params.database()))->getLayout(m_Params.database(), layoutInfo.m_sLayoutName);
  OdDbBaseLayoutPEPtr pLayout = OdDbBaseLayoutPEPtr(pL);
  bool bModelSpace = pLayout->isModelLayout(pL.get());
  OdGeExtents3d extents;
  if (bModelSpace && plotInfo.m_bUninitializedLayout && pLayout->getGeomExtents(pL.get(), extents) == eOk)
  {
    extents.transformBy(m_pDeviceDwf->viewAt(0)->objectToDeviceMatrix());
    OdGePoint3d ptMin = extents.minPoint(); 
    OdGePoint3d ptMax = extents.maxPoint(); 
    bool bLandscape = fabs(ptMax.x - ptMin.x) > fabs(ptMax.y - ptMin.y);
    // MKU 06/02/2005 - m_bLandscape takes into account in this case only
    if ((bLandscape && plotInfo.m_dPaperHeight > plotInfo.m_dPaperWidth)
      || (!bLandscape && plotInfo.m_dPaperHeight < plotInfo.m_dPaperWidth))
    {
      std::swap(plotInfo.m_dPaperHeight, plotInfo.m_dPaperWidth);
      std::swap(plotInfo.m_dPrintableXmax, plotInfo.m_dPrintableYmax);
      std::swap(plotInfo.m_dPrintableXmin, plotInfo.m_dPrintableYmin);
    }
  }

  plotting->SetColor( m_Params.background() );

  if (plotInfo.m_PlotPaperUnit == DwfLayoutPlotInfo::kMillimeters)
  {
    plotting->SetUnits( DWFPaper::eMillimeters );
    plotting->SetHeight( plotInfo.m_dPaperHeight );
    plotting->SetWidth( plotInfo.m_dPaperWidth );
  }
  else
  {
    plotting->SetUnits( DWFPaper::eInches );
    plotting->SetHeight( plotInfo.m_dPaperHeight / kMmPerInch);
    plotting->SetWidth( plotInfo.m_dPaperWidth / kMmPerInch);
  }

  double paperClip[4] = 
  {
    0,
    0,
    plotting->GetWidth(),
    plotting->GetHeight()
  };
  plotting->SetClip(paperClip);

  // To specify a clipping rect for any graphic resource, if applicable.  
  //  This rectangle is in paper units and is relative to the paper "origin", 
  //  which is the bottom left corner of the paper.

  // To set the transform of the resource.  This is a transform mapping
  //  the drawing vector-space (W2D) to the paper.

  double centerDrawingX =  ( m_rectViewport.right + m_rectViewport.left ) / 2;
  double centerDrawingY =  ( m_rectViewport.top + m_rectViewport.bottom ) / 2;

  double centerPaperX =  (paperClip[2] + paperClip[0] ) / 2;
  double centerPaperY =  (paperClip[3] + paperClip[1] ) / 2;

  double scale;
  if ( !bModelSpace )
  {
    scale = 1 / m_pDeviceDwf->viewAt(0)->worldToDeviceMatrix().scale();
    // Get scale factor
    double factor;
    if (pLayout->useStandardScale(pL.get()))
    {
      pLayout->getStdScale(pL.get(), factor);
    }
    else
    {
      double numerator, denominator;
      pLayout->getCustomPrintScale(pL.get(), numerator, denominator);
      factor = numerator / denominator;
    }
    scale *= factor;
  }
  else
  {
    int trueWidth  = Od_abs( (int)(m_rectViewport.right - m_rectViewport.left) );
    int trueHeight = Od_abs( (int)(m_rectViewport.top - m_rectViewport.bottom) );
    double scaleX = plotting->GetWidth() / (trueWidth ? (double)trueWidth : plotting->GetWidth());
    double scaleY = plotting->GetHeight() / (trueHeight ? (double)trueHeight : plotting->GetHeight());
    scale = odmin(scaleX, scaleY);
  }
  if (OdZero(scale))
    return false;
  double transX = centerPaperX - scale * centerDrawingX;
  double transY = centerPaperY - scale * centerDrawingY;

  // To set the DWFGraphicResource's (W2D) transform (identity).

  double dwfToPaper[16] = 
  {
    scale, 0, 0, 0,
    0, scale, 0, 0,
    0, 0, 1, 0, 
    transX, transY, 0, 1
  };
  plotting->SetTransform(dwfToPaper);

  // Set the DWFGraphicResource's (W2D) extents
  double extents1[4] =
  {
    m_rectViewport.left   * scale + transX,  // left
    m_rectViewport.bottom * scale + transY,  // bottom
    m_rectViewport.right  * scale + transX,  // right
    m_rectViewport.top    * scale + transY   // top
  };
  plotting->SetExtents(extents1);
  plotting->xps_page_offset.x = centerPaperX / scale - centerDrawingX;
  plotting->xps_page_offset.y = centerDrawingY - centerPaperY / scale;
  plotting->xps_scale = scale;
  return true;
}


void CDwfExportImpl::setPlotSettingsToLayout()
{
  OdGsDCRect clipBox;
  (OdDbBaseDatabasePEPtr(m_Params.database()))->applyLayoutSettings(clipBox, m_pDevice, m_Params.database());

  /*OdDbBaseLayoutPEPtr pLayout = OdDbBaseLayoutPEPtr(((OdDbBaseDatabasePE*)m_Params.database())->currentLayout(m_Params.database()));
  bool bModel = m_pDevice->isKindOf(OdGsModelLayoutHelper::desc());
  OdDbPlotSettings::PlotType plotType = pLayout->plotType(pLayout.get());
  OdGsViewPtr pOverallView = bModel ? OdGsModelLayoutHelperPtr(m_pDevice)->activeView() 
    : OdGsPaperLayoutHelperPtr(m_pDevice)->overallView();

  OdGePoint3d viewTarget;
  OdRxObjectPtr pVObject;
  if (plotType == OdDbPlotSettings::kView)
  {
    OdString sPlotViewName = pLayout->getPlotViewName(pLayout.get());
//    OdDbViewTableRecordPtr pVtr = // before 09.01.2012
//      ((OdDbViewTablePtr)(m_Params.database()->getViewTableId().safeOpenObject()))->getAt(sPlotViewName).safeOpenObject();

    OdDbBaseViewTablePEPtr Tab = ((OdDbBaseDatabasePE*)m_Params.database())->getViewTable(m_Params.database());
    OdDbBaseViewTableRecordPEPtr pVtr = Tab->getRecordByName(Tab.get(), sPlotViewName);
    viewTarget = pVtr->getTarget(pVtr.get());     // in plotPaperUnits
    pVObject = pVtr;
  }
  else if (bModel)
  {
    OdDbViewportTablePtr pVPT = m_Params.database()->getViewportTableId().safeOpenObject();
    OdDbViewportTableRecordPtr pActiveVP = pVPT->getActiveViewportId().safeOpenObject();

    viewTarget = pActiveVP->target();     // in plotPaperUnits
    pVObject = pActiveVP;
  }
  else
  {
    OdDbObjectId overallVpId = pLayout->overallVportId();
    OdDbViewportPtr pActiveVP = overallVpId.safeOpenObject();

    viewTarget = pActiveVP->viewTarget();   // in plotPaperUnits
    pVObject = pActiveVP;
  }

  OdAbstractViewPEPtr pAbstractViewPE = pVObject;
  OdGePoint3d viewportCenter =          pAbstractViewPE->target(pVObject);       // in plotPaperUnits
  bool isPerspective  =                 pAbstractViewPE->isPerspective(pVObject);
  double viewportH =                    pAbstractViewPE->fieldHeight(pVObject);  // in plotPaperUnits
  double viewportW =                    pAbstractViewPE->fieldWidth(pVObject);   // in plotPaperUnits
  OdGeVector3d viewDir =                pAbstractViewPE->direction(pVObject);    // in plotPaperUnits
  OdGeVector3d upV =                    pAbstractViewPE->upVector(pVObject);     // in plotPaperUnits
  OdGeMatrix3d eyeToWorld =             pAbstractViewPE->eyeToWorld(pVObject);
  OdGeMatrix3d WorldToeye =             pAbstractViewPE->worldToEye(pVObject);

  bool isPlanView = /*viewTarget.isEqualTo(OdGePoint3d(0,0,0)) &&*/ /*viewDir.normal().isEqualTo(OdGeVector3d::kZAxis);

  OdGePoint3d oldTarget = viewTarget;

  double fieldWidth(viewportW), fieldHeight(viewportH);
  if (plotType == OdDbPlotSettings::kWindow || (plotType == OdDbPlotSettings::kLimits && isPlanView))
  {
    double xmin, ymin, xmax, ymax;
    if (plotType == OdDbPlotSettings::kWindow)
    {
      pLayout->getPlotWindowArea(pLayout.get(), xmin, ymin, xmax, ymax); // in plotPaperUnits
    }
    else
    {
      xmin = m_Params.database()->getLIMMIN().x; ymin = m_Params.database()->getLIMMIN().y;
      xmax = m_Params.database()->getLIMMAX().x; ymax = m_Params.database()->getLIMMAX().y;
    }

    fieldWidth  = xmax - xmin;
    fieldHeight = ymax - ymin;

    OdGeVector3d tmp = viewportCenter - viewTarget;
    viewTarget.set((xmin + xmax) / 2., (ymin + ymax) / 2., 0);
//  if (!bModel) // incorrect
    viewTarget.transformBy(eyeToWorld);
    viewTarget -= tmp;
  } 
  else if (plotType == OdDbPlotSettings::kDisplay)
  {
    viewTarget = viewportCenter;
    fieldWidth  = viewportW;
    fieldHeight = viewportH;
  }
  else if (plotType == OdDbPlotSettings::kExtents || (plotType == OdDbPlotSettings::kLimits && !isPlanView))
  {
    OdGeExtents3d extents;
    if (pLayout->getBlockTableRecordId().safeOpenObject()->getGeomExtents(extents) == ::eOk)
    {
      viewTarget = (extents.minPoint() + extents.maxPoint().asVector()) / 2.;

      extents.transformBy(WorldToeye);

      fieldWidth  = fabs(extents.maxPoint().x - extents.minPoint().x);
      fieldHeight = fabs(extents.maxPoint().y - extents.minPoint().y);
    }
    if (OdZero(fieldWidth) || OdZero(fieldHeight))
      return; // don't change the default view in that (invalid) case
  }
  else if (plotType == OdDbPlotSettings::kView)
  {
    viewTarget = viewportCenter;

    fieldWidth  = viewportW;
    fieldHeight = viewportH;
  }
  else if (plotType == OdDbPlotSettings::kLayout)
  {
    double offX, offY, scaleX, scaleY;
    pLayout->getPlotOrigin(pLayout.get(), offX, offY);
    pLayout->getPlotPaperSize(pLayout.get(), scaleX, scaleY);
    if (scaleX < 1.e-10 || scaleY < 1.e-10) return;
    double dLeftMargin   = pLayout->getLeftMargin(pLayout.get());
    //double dRightMargin  = pLayout->getRightMargin();
    double dTopMargin    = pLayout->getTopMargin(pLayout.get());
    double dBottomMargin = pLayout->getBottomMargin(pLayout.get());
    // Get scale factor
    double factor;
    if (pLayout->useStandardScale(pLayout.get()))
    {
      pLayout->getStdScale(pLayout.get(), factor);
    }
    else
    {
      double numerator, denominator;
      pLayout->getCustomPrintScale(pLayout.get(), numerator, denominator);
      factor = numerator / denominator;
    }
    OdDbPlotSettings::PlotRotation plotRot = (OdDbPlotSettings::PlotRotation)pLayout->plotRotation(pLayout.get());
    // Apply paper rotation to paper parameters
    switch (plotRot)
    {
      case OdDbPlotSettings::k270degrees:
      case OdDbPlotSettings::k90degrees:
        dBottomMargin = dLeftMargin;
        dLeftMargin = dTopMargin;
        std::swap(offX, offY);
        std::swap(scaleX, scaleY);
        break;
      default:
        break;
    }
    // Units conversion 
    if (pLayout->plotPaperUnits(pLayout.get()) != OdDbPlotSettings::kMillimeters)
    {
      offX /= kMmPerInch; offY /= kMmPerInch;
      scaleX /= kMmPerInch; scaleY /= kMmPerInch;
      dLeftMargin /= kMmPerInch; dBottomMargin /= kMmPerInch;
    }
    if (factor != 1.0)
    {
      offX /= factor; offY /= factor;
      scaleX /= factor; scaleY /= factor;
      dLeftMargin /= factor; dBottomMargin /= factor;
    }
    // View parameters calculate
    viewTarget.x = scaleX / 2 - (offX + dLeftMargin);
    viewTarget.y = scaleY / 2 - (offY + dBottomMargin);
    fieldWidth = scaleX;
    fieldHeight = scaleY;
  }

  if (plotType != OdDbPlotSettings::kView) // AlexR : 3952
    viewTarget = viewTarget.orthoProject(OdGePlane(oldTarget, viewDir));

  // in plotpaper units
  pOverallView->setView(viewTarget + viewDir, viewTarget, upV, fieldWidth, fieldHeight, isPerspective ? OdGsView::kPerspective : OdGsView::kParallel);*/
}

void CDwfExportImpl::addMetadata(bool bPageDataExists, DwfPageData &pageData, DWFToolkit::DWFEPlotSection* pPage)
{
  OdString strLocalTime;
  OdTimeStamp createTime((OdDbBaseDatabasePEPtr(m_Params.database()))->getCreationTime(m_Params.database()));
  OdTimeStamp modificationTime((OdDbBaseDatabasePEPtr(m_Params.database()))->getUpdateTime(m_Params.database()));

  // createTime.universalToLocal(); TDCREATE returns local time
  createTime.strftime(DWF_DATE_FORMAT_STR, strLocalTime);
  DWFString zCreationTime( strLocalTime.c_str() );

  // modificationTime.universalToLocal(); TDUPDATE returns local time
  modificationTime.strftime(DWF_DATE_FORMAT_STR, strLocalTime);
  DWFString zModificationTime( strLocalTime.c_str() );

  DWFString strValue = (bPageDataExists && !pageData.sLayout.isEmpty()) 
    ? DWFString(pageData.sLayout.c_str())
    : DWFString(L"");
  pPage->addProperty( DWFCORE_ALLOC_OBJECT(DWFToolkit::DWFProperty( L"Layout Name", 
    strValue,
    L"", L"", L"")), true );

  strValue = (bPageDataExists && !pageData.sPageAuthor.isEmpty()) 
    ? DWFString(pageData.sPageAuthor.c_str())
    : DWFString(L"");
  pPage->addProperty( DWFCORE_ALLOC_OBJECT(DWFToolkit::DWFProperty( L"author", 
    strValue,
    L"", L"", L"")), true );

  strValue = (bPageDataExists && !pageData.sPageSubject.isEmpty()) 
    ? DWFString(pageData.sPageSubject.c_str())
    : DWFString(L"");
  pPage->addProperty( DWFCORE_ALLOC_OBJECT(DWFToolkit::DWFProperty( L"subject", 
    strValue,
    L"", L"", L"")), true );

  strValue = (bPageDataExists && !pageData.sPageCompany.isEmpty()) 
    ? DWFString(pageData.sPageCompany.c_str())
    : DWFString(L"");
  pPage->addProperty( DWFCORE_ALLOC_OBJECT(DWFToolkit::DWFProperty( L"company",
    strValue,
    L"", L"", L"")), true );

  strValue = (bPageDataExists && !pageData.sPageComments.isEmpty()) 
    ? DWFString(pageData.sPageComments.c_str())
    : DWFString(L"");
  pPage->addProperty( DWFCORE_ALLOC_OBJECT(DWFToolkit::DWFProperty( L"comments",
    strValue,
    L"", L"", L"")), true );

  strValue = (bPageDataExists && !pageData.sPageReviewers.isEmpty()) 
    ? DWFString(pageData.sPageReviewers.c_str())
    : DWFString(L"");
  pPage->addProperty( DWFCORE_ALLOC_OBJECT(DWFToolkit::DWFProperty( L"reviewers",
    strValue,
    L"", L"", L"")), true );

  strValue = (bPageDataExists && !pageData.sPageKeywords.isEmpty()) 
    ? DWFString(pageData.sPageKeywords.c_str())
    : DWFString(L"");
  pPage->addProperty( DWFCORE_ALLOC_OBJECT(DWFToolkit::DWFProperty( L"keywords",
    strValue,
    L"", L"", L"")), true );

  strValue = (bPageDataExists && !pageData.sPageDescription.isEmpty()) 
    ? DWFString(pageData.sPageDescription.c_str())
    : DWFString(L"");
  pPage->addProperty( DWFCORE_ALLOC_OBJECT(DWFToolkit::DWFProperty( L"description",
    strValue,
    L"", L"", L"")), true );

  strValue = (bPageDataExists && !pageData.sPageCopyright.isEmpty()) 
    ? DWFString(pageData.sPageCopyright.c_str())
    : DWFString(L"");
  pPage->addProperty( DWFCORE_ALLOC_OBJECT(DWFToolkit::DWFProperty( L"copyright",
    strValue,
    L"", L"", L"")), true );

  // Creation and Modification time
  // MKU 22.12.2004 - #2466
  OdString locTimeStr;

  OdTimeStamp locTime(OdTimeStamp::kInitLocalTime);
  locTime.strftime(DWF_DATE_FORMAT_STR, locTimeStr);

  DWFString zPageTime(locTimeStr.c_str());

  // TODO: The form of Manifest schema-valid time strings. For instance, CCYY-MM-DDThh:mm:ss.sss

  pPage->addProperty( DWFCORE_ALLOC_OBJECT(DWFToolkit::DWFProperty( DWFXML::kzAttribute_CreationTime, 
    zPageTime, L"", L"", L"")), true );
  pPage->addProperty( DWFCORE_ALLOC_OBJECT(DWFToolkit::DWFProperty( DWFXML::kzAttribute_ModificationTime, 
    zPageTime, L"", L"", L"")), true );

  if (!strLocalTime.isEmpty())
  {
    pPage->addProperty( DWFCORE_ALLOC_OBJECT(DWFToolkit::DWFProperty( DWFXML::kzAttribute_CreationTime, 
      zCreationTime, L"", L"", L"")), true );
  }
  if (!strLocalTime.isEmpty())
  {
    pPage->addProperty( DWFCORE_ALLOC_OBJECT(DWFToolkit::DWFProperty( DWFXML::kzAttribute_ModificationTime, 
      zModificationTime, L"", L"", L"")), true );
  }
}

void CDwfExportImpl::addPreview( DwfPageData &pageData, const DWFString& dwf_zSectionTitle, DWFToolkit::DWFEPlotSection* pPage, DWFToolkit::DWFGraphicResource* p2Dgfx )
{
  if (!pageData.preview.sFileName.isEmpty())
  {
    // now let's add the preview
    //
    // To specify the image file resource's MIME-type
    DWFString zMimeType;
    if (SpecifyImageMimeType(zMimeType, pageData.preview.sFileName))
    {
      DWFToolkit::DWFImageResource* pPreview = 
        DWFCORE_ALLOC_OBJECT( DWFImageResource( dwf_zSectionTitle,    // The friendly/display name of the resource
        DWFXML::kzRole_Preview,
        zMimeType) );

      if (pPreview == NULL)
      {
        _DWFCORE_THROW( DWFMemoryException, L"Failed to allocate resource" );
      }

      //
      // configure the resource
      //
      double anTransform2[4][4] = { {1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1} };

      double anExtents2[4];           // left bottom right top
      anExtents2[0] = 0;
      anExtents2[1] = 0;
      anExtents2[2] = pageData.preview.Width;
      anExtents2[3] = pageData.preview.Height;

      pPreview->configureGraphic( (const double*)anTransform2,
        (const double*)anExtents2 );

      pPreview->configureImage( (unsigned char)pageData.preview.ColorDepth );   // the number of colors used in the image

      DWFCore::DWFFile oPreviewFilename( pageData.preview.sFileName.c_str() );
      DWFCore::DWFStreamFileDescriptor* pPreviewFile = DWFCORE_ALLOC_OBJECT( DWFStreamFileDescriptor(oPreviewFilename, L"rb") );

      if (pPreviewFile == NULL)
      {
        DWFCORE_FREE_OBJECT( pPreview );

        _DWFCORE_THROW( DWFMemoryException, L"Failed to allocate file descriptor" );
      }

      DWFCore::DWFFileInputStream* pPreviewFilestream = DWFCORE_ALLOC_OBJECT( DWFFileInputStream );

      if (pPreviewFilestream == NULL)
      {
        DWFCORE_FREE_OBJECT( pPreview );
        DWFCORE_FREE_OBJECT( pPreviewFile );

        _DWFCORE_THROW( DWFMemoryException, L"Failed to allocate file stream" );
      }

      pPreviewFile->open();
      pPreviewFilestream->attach( pPreviewFile, true );

      pPreview->setInputStream( pPreviewFilestream );

      // drop the resource into the page
      // and be sure to mention that it "belongs" to the W2D 
      // (and replace and delete)
      //
      pPreview->enableCustomZipMode(true, DWFZipFileDescriptor::eZipSmallest);
      pPage->addResource( pPreview, true, true, true, p2Dgfx );
    }
  }
}

void CDwfExportImpl::addFonts( const DwfPageData* pageData, DWFEPlotSection* pPage)
{
  #if defined(_MSC_VER)  // font embedding works only for Windows
  if (!pageData && !getParams().embedAllFonts())
    return;
  for (FontSubsetContainer::const_iterator fi = m_xpsFontMap.begin(); fi != m_xpsFontMap.end(); ++fi)
  {
    if (!getParams().embedAllFonts() && !pageData->arFonts.contains(fi->first))
      continue;
    LOGFONT lf;
    ZeroMemory(&lf, sizeof(LOGFONT));
    lstrcpy(lf.lfFaceName, fi->first);
    lf.lfHeight = 100;
    HFONT hf = ::CreateFontIndirect(&lf);
    DWFEmbeddedFont* pFont = DWFCORE_ALLOC_OBJECT(DWFEmbeddedFont(new DWFEmbeddedFontImpl_Win32(hf)) );
    DWFString chars;
    for (std::set<OdChar>::const_iterator it = fi->second.begin(); it != fi->second.end(); ++it)
    {
      wchar_t c = *it;
      chars.append(&c, 1);
    }
    
    pFont->addCharacters(chars);
    pFont->embed();

    DWFFontResource* pFontRes = 
      DWFCORE_ALLOC_OBJECT( DWFFontResource(pFont->getRequest(),pFont->getPrivilege(),pFont->getCharacterCode(),pFont->getFaceName(),pFont->getLogfontName()));

    pFontRes->setInputStream( pFont->getInputStream() );
    pPage->addResource( pFontRes, true );
  }
#endif
}

OpcHelper::OpcHelper() : _nPartCount(0), _pSection(0), _pStreamCopy(0)
{
}

OpcHelper::~OpcHelper()
{
}

void OpcHelper::setCurrentSection( DWFSection* pSection)
{
  _pSection = pSection;
}

WT_Result OpcHelper::getPartInputStream(const DWFCore::DWFString& rzPath, DWFCore::DWFInputStream** ppStream)
{
  if (ppStream == NULL)
    return WT_Result::Toolkit_Usage_Error;

  XamlResourceInputStream* pStream = _oMap.find( rzPath )->second;
  XamlResourceInputStream* pStreamCopy = DWFCORE_ALLOC_OBJECT( XamlResourceInputStream( *pStream ) );
  *ppStream = pStreamCopy;
  return WT_Result::Success;
}

WT_Result OpcHelper::getPartOutputStream(		
  const DWFCore::DWFString& rzMimeType,
  DWFCore::DWFString& rzPath,
  DWFCore::DWFOutputStream** ppStream)
{
  if (ppStream == NULL)
    return WT_Result::Toolkit_Usage_Error;

  if (_pSection == NULL)
  {
    return WT_Result::Toolkit_Usage_Error;
  }

  if (_zNextOutputStreamPath.chars())
  {
    rzPath = _zNextOutputStreamPath;
    *ppStream = _pStreamCopy;
    _pStreamCopy = NULL;
    _zNextOutputStreamPath.assign( L"" );
    return WT_Result::Success;
  }

  XamlResourceInputStream* pXamlResourceInputStream = DWFCORE_ALLOC_OBJECT( XamlResourceInputStream() );
  XamlResourceOutputStream* pXamlResourceOutputStream = DWFCORE_ALLOC_OBJECT( XamlResourceOutputStream(pXamlResourceInputStream) );

  DWFString zPartName;
  zPartName = _oUuidGen.next(false);
  zPartName.append( L"." );
  zPartName.append(DWFMIME::GetExtension( rzMimeType ));

  rzPath = L"";
  rzPath.append( zPartName );

  DWFResource* pResource = NULL;
  if (rzMimeType == L"application/vnd.ms-package.obfuscated-opentype")
  {
    pResource = DWFCORE_ALLOC_OBJECT( DWFResource( L"Obfuscated Font", DWFXML::kzRole_Font, DWFMIME::kzMIMEType_OBFUSCATEDOPENTYPE, rzPath ));
  }
  else if (rzMimeType == L"application/vnd.ms-opentype")
  {
    pResource = DWFCORE_ALLOC_OBJECT( DWFResource( L"Font", DWFXML::kzRole_Font, DWFMIME::kzMIMEType_TTF, rzPath ));
  }
  else if (rzMimeType == L"application/vnd.ms-package.xps-resourcedictionary+xml")
  {
    pResource = DWFCORE_ALLOC_OBJECT( DWFResource( L"Remote Resource Dictionary", DWFXML::kzRole_Graphics2dDictionary, DWFMIME::kzMIMEType_REMOTERESOURCEDICTIONARY, rzPath ));
  }
  else if (rzMimeType == L"image/jpeg")
  {
    pResource = DWFCORE_ALLOC_OBJECT( DWFImageResource( L"Image", DWFXML::kzRole_RasterReference, DWFMIME::kzMIMEType_JPG, rzPath ));
  }
  else if (rzMimeType == L"image/png")
  {
    pResource = DWFCORE_ALLOC_OBJECT( DWFImageResource( L"Image", DWFXML::kzRole_RasterReference, DWFMIME::kzMIMEType_PNG, rzPath ));
  }
  else if (rzMimeType == L"image/tiff")
  {
    pResource = DWFCORE_ALLOC_OBJECT( DWFImageResource( L"Image", DWFXML::kzRole_RasterReference, DWFMIME::kzMIMEType_TIFF, rzPath ));
  }
  else
  {
    ODA_FAIL(); //unknown mime type!
  }

  if ( pResource )
  {
    _addGfxRelationship( DWFXXML::GetRelationship( pResource->role() ), pResource );   
  }

  _oMap.insert( _tPathToStreamMap::value_type(rzPath, pXamlResourceInputStream) );

  pResource->setRequestedName( zPartName );
  pResource->setInputStream( pXamlResourceInputStream );
  *ppStream = pXamlResourceOutputStream;
  _pSection->addResource( pResource, true );
  return WT_Result::Success;
}

void OpcHelper::_addGfxRelationship(const DWFString& zRel, DWFResource* pTarget )
{
  if ( _pSection )
  {
    DWFPointer<DWFResourceContainer::ResourceIterator> pIt(
      _pSection->findResourcesByRole( DWFXML::kzRole_Graphics2d ), false );
    if ( !pIt.isNull() )
    {
      pIt->get()->addRelationship( pTarget, zRel );
    }
  }
}
#ifndef _DWFCORE_WIN32_SYSTEM
WT_Result 
OpcHelper::getSubsetFontName(
                             OdString& /*zFontName*/,
                             WT_Integer32 /*nHeight*/,
                             const OdString& /*zStr*/,
                             bool /*bForEmbeddedFontObject*/)
{
  return WT_Result::Success;
}
#endif

#if defined(_DWFCORE_WIN32_SYSTEM)

}
_DWFTK_API int subsetFont(void* pFontBuffer,               /*in*/
             DWORD nFontBufferLen,                       /*in*/
             const unsigned short * keepList,            /*in*/
             unsigned short count,                       /*in*/
             bool isTTC,
             unsigned char*& puchFontPackageBuffer,      /*out*/
             unsigned long&   ulFontPackageBufferSize,   /*out*/
             unsigned long&   ulBytesWritten);           /*out*/

namespace TD_DWF_EXPORT {


void parseKey( wchar_t* keyOut, const wchar_t* guidIn)
{
	/*
		MUST be of the form "B03B02B01B00-B11B10-B21B20-B30B31-B32B33B34B35B36B37" or 
		"B03B02B01B00-B11B10-B21B20-B30B31-B32B33B34B35B36B37.ext" where each Bx represents a 
		placeholder for one byte of the GUID, represented as two hex digits [M2.54].		
	*/

	/*
		Key: B37B36B35B34B33B32B31B30B20B21B10B11B00B01B02B03
	*/

	int i, idx=15;
	bool high = true;
	size_t len = Od_wcslen( guidIn );

	memset(keyOut, 0, 16*sizeof(wchar_t));

	for (i=0; i<(int)len && idx>=0; i++)
	{
		wchar_t val = 0xFFFF;
		if ( guidIn[i] >= '0' && guidIn[i] <= '9' )
		{
			val = guidIn[i] - '0';
		}
		else if ( guidIn[i] >= 'A' && guidIn[i] <= 'F' )
		{
			val = 10 + (guidIn[i] - 'A');
		}
		else if ( guidIn[i] >= 'a' && guidIn[i] <= 'f' )
		{
			val = 10 + (guidIn[i] - 'a');
		}

		if (val != 0xFFFF)
		{
			if (high)
			{
				keyOut[idx] = (wchar_t) (val*16);
				high = false;
			}
			else
			{
				keyOut[idx] += (val & 0xFFFF);
				high = true;
				idx--;
			}
		}
	}
}

//It does both obfuscation and deobfuscation
int fuscator(DWFInputStream* in, DWFOutputStream* out, const wchar_t* key)
{
	if (in == NULL || out == NULL)
	{
		return -1;
	}

	char buf[32];
	bool done = false;
	bool xorComplete = false;
	while( !done )
	{
        size_t bytes = in->read(buf, 32);
        if (bytes != 0)
        {
            //This only works when we're actually embedding in XPS
            //documents.  For testing purposes (XamlPad, IE7, etc.)
            //we leave the font de-obfuscated.


            #ifndef OBFUSCATE
            xorComplete; key; //reference these for compiler happiness
            #else
                if ( !xorComplete )
                {
                    int i,j;
                    for( j=0; j<32; j+=16 )
                    {
                        for( i=0; i<16; i++ )
                        {
                            buf[i+j] ^= key[i];
                        }
                    }
                    xorComplete = true;
                }
            #endif
            size_t written = out->write(buf, bytes);
            WD_Assert( written == bytes);
        }

		if ( bytes == 0 && in->available() == 0)
		{
			done = true;
		}
	}

    out->flush();

	return 0;
}

WT_Result 
OpcHelper::getSubsetFontName(
                             OdString& zFontName,
                             WT_Integer32 nHeight,
                             const OdString& zStr,
                             bool bForEmbeddedFontObject)
{
  //Load the font into a file
  HDC hdc = ::GetDC( GetDesktopWindow() );
  LOGFONTW logFont;
  ZeroMemory( &logFont, sizeof(logFont) );
  logFont.lfHeight = nHeight;
  wcscpy(logFont.lfFaceName, zFontName);
  HFONT hfont = ::CreateFontIndirect(&logFont);
  if (hfont == NULL || hfont == INVALID_HANDLE_VALUE)
  {
    return WT_Result::Internal_Error;
  }
  HGDIOBJ holdFont = ::SelectObject(hdc, hfont);
  DWORD nBytesRequired = ::GetFontData( hdc, NULL, 0, NULL, 0);
  void* pFontBuffer = malloc( nBytesRequired );
  bool isTTC = false;
  DWORD nBytesRead = ::GetFontData(hdc, 0x66637474, 0, pFontBuffer, nBytesRequired);
  if( nBytesRequired != nBytesRead )
    nBytesRead = ::GetFontData(hdc, 0, 0, pFontBuffer, nBytesRequired);
  else
    isTTC = true;
  WD_Assert(nBytesRequired == nBytesRead);


  //Do the font subsetting
  unsigned char* puchFontPackageBuffer = NULL; //holds the subset
  unsigned long  ulFontPackageBufferSize = 0; //holds the buffer size
  unsigned long  ulBytesWritten = 0; //holds the number of bytes written (should be == buffer size)
  int retval = subsetFont( 
    pFontBuffer, 
    nBytesRead, 
    (const unsigned short*) zStr.c_str(), 
    (unsigned short) zStr.getLength(), 
    isTTC,
    puchFontPackageBuffer, 
    ulFontPackageBufferSize, 
    ulBytesWritten );
  ::SelectObject(hdc, holdFont);
  ::DeleteObject(hfont);
  ::ReleaseDC( GetDesktopWindow(), hdc );
  if (retval != 0)
  {
    return WT_Result::Internal_Error;
  }

  //Now, create a GUID and filename
  DWFString zObfuscatedFontFilename;
  DWFString zGuid = _oUuidGen.next(false);
  DWFString zPartName = zGuid;

#ifdef OBFUSCATE
  zPartName.append( L".odttf" );
#else
  zPartName.append( L".ttf" );
#endif
  zObfuscatedFontFilename.append( zPartName );

  zFontName = (const wchar_t*)zObfuscatedFontFilename;

  XamlResourceInputStream* pXamlResourceInputStream = DWFCORE_ALLOC_OBJECT( XamlResourceInputStream() );

  XamlResourceOutputStream* pXamlResourceOutputStream = DWFCORE_ALLOC_OBJECT( XamlResourceOutputStream(pXamlResourceInputStream) );

  DWFBufferInputStream oReadFrom( puchFontPackageBuffer, ulBytesWritten );

  //Create a key
  wchar_t key[64];
  memset(key, 0, sizeof(key));
  parseKey( key, zGuid );

  //obfuscate the data as we write the data to a file
  fuscator( &oReadFrom, pXamlResourceOutputStream, key );

  if (bForEmbeddedFontObject)
  {
    _pStreamCopy = DWFCORE_ALLOC_OBJECT( DWFBufferOutputStream( pXamlResourceOutputStream->bytes() ) );
    _pStreamCopy->write( pXamlResourceOutputStream->buffer(), pXamlResourceOutputStream->bytes() );
  }

  DWFCORE_FREE_OBJECT( pXamlResourceOutputStream );

  DWFToolkit::DWFFontResource* pFontResource = NULL;
  //DWFToolkit::DWFResource* pResource = NULL;
  //Add DWFFontResource
  //
  int												iRequest(0);
  DWFToolkit::DWFFontResource::tePrivilege		ePrivilege(DWFToolkit::DWFFontResource::eEditable);
  DWFToolkit::DWFFontResource::teCharacterCode	eCharacterCode(DWFToolkit::DWFFontResource::eUnicode);
  DWFCore::DWFString								strCanonicalName(logFont.lfFaceName);
  DWFCore::DWFString								strLogFontName(logFont.lfFaceName);
#ifdef OBFUSCATE
  //pFontResource = DWFCORE_ALLOC_OBJECT( DWFToolkit::DWFResource( (wchar_t*)zFontName.unicode(), DWFXML::kzRole_Font, DWFMIME::kzMIMEType_OBFUSCATEDOPENTYPE, zObfuscatedFontFilename ));

  pFontResource = DWFCORE_ALLOC_OBJECT( DWFToolkit::DWFFontResource(	iRequest,
    ePrivilege,
    eCharacterCode,
    strCanonicalName,
    strLogFontName));
  pFontResource->setMIME(DWFMIME::kzMIMEType_OBFUSCATEDOPENTYPE);;
#else
  pFontResource = DWFCORE_ALLOC_OBJECT( DWFToolkit::DWFFontResource( (wchar_t*)zFontName.unicode(), DWFXML::kzRole_Font, DWFMIME::kzMIMEType_TTF, zObfuscatedFontFilename ));
  pFontResource->setMIME(DWFMIME::kzMIMEType_TTF);
#endif

  _oMap.insert( _tPathToStreamMap::value_type(zObfuscatedFontFilename, pXamlResourceInputStream) );
  pFontResource->setRequestedName( zPartName );
  pFontResource->setInputStream( pXamlResourceInputStream );
  _pSection->addResource( pFontResource, true );

  if (puchFontPackageBuffer != NULL)
  {
    free( puchFontPackageBuffer );
  }
  if (pFontBuffer != NULL)
  {
    free( pFontBuffer );
  }

  if (bForEmbeddedFontObject)
  {
    _zNextOutputStreamPath = (const wchar_t*)zFontName;
    //
    // copy this to a local file so that W2dTest can find it
    //
    XamlResourceInputStream oStreamCopy( *pXamlResourceInputStream );
    DWFFile oFile( _zNextOutputStreamPath );
    DWFStreamFileDescriptor oFD( oFile, L"w" );

    oFD.open();
    oFD.write( oStreamCopy._pBuffer, oStreamCopy._nBufferBytes );
  }
  return WT_Result::Success;
}
#endif

WT_Result CDwfExportImpl::registerFontForXPS(const OdChar* msg, const OdString& fontName)
{
  WT_XAML_File* f = static_cast<WT_XAML_File*>(getWtFile());
  if (!f->resolveFontUri(fontName))
  {
    OdString zFontName(fontName);
    OdString zFontPath(zFontName );
    if (WT_Result::Internal_Error == gHelper.getSubsetFontName(zFontPath, 20, msg))
      return WT_Result::Internal_Error;
    f->registerFontUri(zFontName, zFontPath);
  }
  return WT_Result::Success;
}

void CDwfExportImpl::initializeLayoutList(OdArray<DwfLayoutInfo>& g_arrayLayoutInfo)
{
  g_arrayLayoutInfo.resize(0);

  if ( m_Params.pageData().size() > 0 )
  {
    for (unsigned int k = 0; k < m_Params.pageData().size(); k++)
    {
      OdString sLayout = m_Params.pageData()[k].sLayout;

      if (!sLayout.isEmpty() && !((OdDbBaseDatabasePEPtr(m_Params.database()))->findLayoutNamed(m_Params.database(), sLayout) == 0)) // dgn dwf export
      {
        OdDbStub* idLayout = (OdDbBaseDatabasePEPtr(m_Params.database()))->findLayoutNamed(m_Params.database(), sLayout); // dgn dwf export

        int nItem = g_arrayLayoutInfo.size();
        g_arrayLayoutInfo.resize(nItem + 1);

        g_arrayLayoutInfo[nItem].m_sLayoutName = sLayout;
        g_arrayLayoutInfo[nItem].m_idLayout = idLayout;
        g_arrayLayoutInfo[nItem].m_iParamIndex = k;
      }
      else
      {
        ODA_ASSERT(false);
      }
    }
  }
  else
  {
    // To export all initialized layouts
    //
    // Model Space is the 1st

    g_arrayLayoutInfo.resize(1);

    OdRxObjectPtr idMsLayout = (OdDbBaseDatabasePEPtr(m_Params.database()))->getFirstLayout(m_Params.database());// dgn dwf export
    OdDbBaseLayoutPEPtr pMsLayout(idMsLayout);

    // To get the paper layouts in this database.

    OdDbBaseDatabasePEPtr pDbPE(m_Params.database());
    OdRxIteratorPtr pLayoutIter = pDbPE->layouts(m_Params.database());

    for (; !pLayoutIter->done(); pLayoutIter->next())
    {
      OdDbBaseLayoutPEPtr pLayout(pLayoutIter->object());
      OdDbStub* idCur = pDbPE->getId(pLayoutIter->object());//(pLayoutIter.get());
      OdString nameCur = pLayout->name(pLayoutIter->object());//(pLayoutIter.get());
      if (idCur == pDbPE->getId(idMsLayout.get()))//(pMsLayout.get()))
      {
        g_arrayLayoutInfo[0].m_idLayout = idCur; // dgn dwf export
        g_arrayLayoutInfo[0].m_sLayoutName = nameCur; // dgn dwf export
        g_arrayLayoutInfo[0].m_iParamIndex = 0;
        continue;
      }
      if (nameCur.isEmpty())
        continue;
      if (isAnythingForExport(/*m_Params.database(),*/ pLayoutIter))
      {
        int nItem = g_arrayLayoutInfo.size();
        g_arrayLayoutInfo.resize(nItem + 1);
        g_arrayLayoutInfo[nItem].m_idLayout = idCur;
        g_arrayLayoutInfo[nItem].m_sLayoutName = nameCur; // dgn dwf export
        g_arrayLayoutInfo[nItem].m_iParamIndex = nItem;
      }
    }
  }
}
//====================================================================
//
void CDwfExportImpl::setDeviceScale(const DwfLayoutInfo& li)
{
  OdRxObjectPtr pLayout = OdDbBaseDatabasePEPtr(m_Params.database())->currentLayout(m_Params.database());
  OdDbBaseLayoutPEPtr pLayoutPE(pLayout);
  if (pLayoutPE->printLineweights(pLayout) || pLayoutPE->showPlotStyles(pLayout))
  {
    double deviceWidth = (getParams().xSize() > lMaxDwfResolution) ? lMaxDwfResolution : getParams().xSize();
    double deviceHeight = (getParams().ySize() > lMaxDwfResolution) ? lMaxDwfResolution : getParams().ySize();

    double plotHeight, plotWidth;
    getPlotPaperSize( li, plotWidth, plotHeight );

    double scaleX = deviceWidth / plotWidth;
    double scaleY = deviceHeight / plotHeight;

    OdGsView* pTo = m_pDevice->viewAt(0);
    pTo->setLineweightToDcScale(odmin(scaleX, scaleY) * 72. / 25.4 / 100); // logpixels/mm2in
  }
}

void CDwfExportImpl::setW2dCompression()
{
  switch(m_Params.format())
  {
  case DW_UNCOMPRESSED_BINARY:
    m_wtFile->heuristics().set_allow_binary_data(WD_True);
    m_wtFile->set_data_decompression(WD_False);
    break;

  case DW_COMPRESSED_BINARY:
    m_wtFile->heuristics().set_allow_binary_data(WD_True);
    m_wtFile->heuristics().set_allow_data_compression(WD_True);
    break;

  case DW_ASCII:
    m_wtFile->heuristics().set_allow_binary_data(WD_False);
    m_wtFile->set_data_decompression(WD_False);
    break;
  default:
    ODA_FAIL();
    break;
  }
}

}
