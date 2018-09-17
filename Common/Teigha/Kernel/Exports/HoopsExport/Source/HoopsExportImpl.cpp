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
// HoopsExportImpl.cpp : implementation of the HoopsExportImpl class
//
///////////////////////////////////////////////////////////////////////////////

#ifdef	_TOOLKIT_IN_DLL_

#ifdef _DEBUG
#pragma comment(lib,"hoopsd.lib")
#pragma comment(lib,"hoops_streamd.lib")
#pragma comment(lib,"hoops_utilsstatd_md.lib")
#else
#pragma comment(lib,"hoops.lib")
#pragma comment(lib,"hoops_stream.lib")
#pragma comment(lib,"hoops_utilsstat_md.lib")
#endif

#endif // _TOOLKIT_IN_DLL_

#ifdef _MT // MT-MD configuration

#define HSTREAM_STATIC_LIB

#ifdef _DLL // MD only

#ifdef _DEBUG
#pragma comment(lib,"hoopstatd_md.lib")
#pragma comment(lib,"hoops_streamstatd_md.lib")
#pragma comment(lib,"hoops_utilsstatd_md.lib")
#else
#pragma comment(lib,"hoopstat_md.lib")
#pragma comment(lib,"hoops_streamstat_md.lib")
#pragma comment(lib,"hoops_utilsstat_md.lib")
#endif

#else //MT configuration only

#ifdef _DEBUG
#pragma comment(lib,"hoopstatd_mt.lib")
#pragma comment(lib,"hoops_streamstatd_mt.lib")
#pragma comment(lib,"hoops_utilsstatd_mt.lib")
#else
#pragma comment(lib,"hoopstat_mt.lib")
#pragma comment(lib,"hoops_streamstat_mt.lib")
#pragma comment(lib,"hoops_utilsstat_mt.lib")
#endif

#endif // MT only

#endif //MT-MD configuration






#include "HStream.h"
#include "HoopsExportImpl.h"
#include "HoopsExportDevice.h"
#include "HoopsExportView.h"

#include "DynamicLinker.h"

// define size for general buffers on stack

#ifdef _TOOLKIT_IN_DLL_
#ifdef ODA_WINDOWS
#ifdef _DEBUG
#define HOOPSSTREAM_DLL_NAME OD_T("hoops_stream1700_vc80d.dll")
#else
#define HOOPSSTREAM_DLL_NAME OD_T("hoops_stream1700_vc80.dll")
#endif
#else
#define HOOPSSTREAM_DLL_NAME L"hoops_stream"
#endif
#endif

namespace TD_HOOPS_COMMON_EXPORT {

#define MVO_BUFFER_SIZE						4096
#define L_HSF_EXT                 L".HSF"



//***************************************************************************
CHoopsExportImpl::CHoopsExportImpl()
{
  m_iShapeCount     = 0;
  m_iLineCount      = 0;
  m_iLTypeCount     = 0;
  m_iXLineCount     = 0;
  m_iRayCount       = 0;
  m_iCircleCount    = 0;
  m_iEllipseCount   = 0;
  m_iNurbsCount     = 0;
  m_iTextCount      = 0;
  m_iFontStyleCount = 0;
  m_iPolygonCount   = 0;
  m_iViewportCount  = 0;
  m_iImageCount     = 0;
  m_iImageShellCount = 0;
  m_iTextureCount   = 0;
  m_iMeshCount      = 0;
  m_iShellCount     = 0;
  m_iLSCount        = 0;
  m_iObjectCount    = 0;
}


CHoopsExportImpl::~CHoopsExportImpl()
{

}

HoopsResultEx CHoopsExportImpl::init(const HoopsExportParams& params )
{
  m_Params = params;

  if( m_Params.pDb == NULL )
    return exNullDatabase;

  return exOk;
}

static OdGsDevicePtr createRenderDevice()
{
  // creating the render device
  OdGsModulePtr pRenderGs = ::odrxDynamicLinker()->loadModule(OdWinOpenGLModuleName);
  OdGsDevicePtr pRenderDevice;
  if(pRenderGs.get())
  {
    pRenderDevice = pRenderGs->createBitmapDevice();
  }
  return pRenderDevice;
}

HoopsResultEx CHoopsExportImpl::run()
{
  OdString strFileExt = m_Params.strFileName;

  if( strFileExt.getLength() > 4 )
    strFileExt = strFileExt.right(4);

  strFileExt.makeUpper();

  if( strFileExt == L_HSF_EXT )
    m_Params.flags = m_Params.flags | SHX_TEXT_AS_GEOMETRY;

  OdDbBaseDatabasePEPtr pDbPE(m_Params.pDb);
  OdGiDefaultContextPtr pCtx = pDbPE->createGiContext(m_Params.pDb);
  pCtx->setPaletteBackground((ODCOLORREF)(m_Params.background));

  OdGsDevicePtr pRenderDevice = createRenderDevice();

  if( !pRenderDevice.isNull() )
  {
    pRenderDevice->setBackgroundColor(m_Params.background);

    if( m_Params.palette() )
    {
      pRenderDevice->setLogicalPalette( m_Params.palette(), 256 );
    }
  }

  OdRxObjectPtr pActiveLayout = pDbPE->currentLayout( m_Params.pDb );

  m_iRealStartKey = m_Params.iStartKey;

  OdGsDevicePtr pDevice = OdHoopsExportDevice::createObject();
  OdHoopsExportDevice* pDeviceHoops = (OdHoopsExportDevice*)pDevice.get();
  pDeviceHoops->setExportImpl(this);
  pDeviceHoops->setBackgroundColor(m_Params.background);

  if( m_Params.palette() )
  {
    pDeviceHoops->setLogicalPalette( m_Params.palette(), 256 );
  }

  if( !m_Params.bExportCurrentLayout )
  {
    OdRxIteratorPtr pLayIter = pDbPE->layouts(m_Params.pDb);

    if( pLayIter.isNull() )
      return ::exOdError;

    HC_KEY iStartKey = m_Params.iStartKey;

    int nLayoutsExported = 0;

    while( !pLayIter->done() )
    {
      if( !m_pDevice.isNull() )
        m_pDevice.release();

      pDeviceHoops->eraseAllViews();
      pDeviceHoops->m_bInitCamera = false;

      if( iStartKey == 0 )
      {
        iStartKey = m_Params.iStartKey;
        m_iRealStartKey = m_Params.iStartKey;
      }

      OdRxObjectPtr pCurLayout = pLayIter->object();
      OdDbBaseLayoutPEPtr pLayoutInfo( pCurLayout );

      if( pLayoutInfo.isNull() )
      {
         pLayIter->next();
         continue;
      }

      nLayoutsExported++;

      pDbPE->setCurrentLayout(m_Params.pDb, pLayoutInfo->name(pCurLayout));

      if( pLayoutInfo->isModelLayout(pCurLayout) && getParams().bSetCamera )
      {
        pDeviceHoops->m_bSetCamera = true;
        pCtx->enableGsModel(true);
      }
      else
      {
        pDeviceHoops->m_bSetCamera = false;
        pCtx->enableGsModel(false);
      }

      pDbPE->loadPlotstyleTableForActiveLayout(pCtx, m_Params.pDb);
      m_pDevice = pDbPE->setupActiveLayoutViews(pDeviceHoops, pCtx);

      if(pRenderDevice.get())
      {
        pRenderDevice = pDbPE->setupActiveLayoutViews(pRenderDevice, pCtx);
        pDeviceHoops->setRenderDevice(pRenderDevice);
      }

      if( !m_Params.bSetExportRenderModeFrpmLayout )
      {
        for( OdInt32  i = 0; i < pDeviceHoops->numViews(); i++ )
        {
          OdGsView* pView  = pDeviceHoops->viewAt(i);
          pView->setMode(OdGsView::kGouraudShaded);
        }
      }

      pDeviceHoops->dc_open_segment( iStartKey );

      if( pActiveLayout.get() == pCurLayout.get() )
        m_Params.iStartKey = pDeviceHoops->createActiveLayoutSegment(pLayoutInfo->name(pCurLayout));
      else
        m_Params.iStartKey = pDeviceHoops->createInvisibleLayoutSegment(pLayoutInfo->name(pCurLayout));

      pDeviceHoops->enableRecording();
      m_pDevice->update(0);
      pDeviceHoops->enableRecording(false);
      pDeviceHoops->dc_close_segment();

      pLayIter->next();
    }

    if( !nLayoutsExported )
      return ::exOdError;
  }
  else
  {
    pDeviceHoops->eraseAllViews();

    if( m_iRealStartKey == 0 )
      m_iRealStartKey = m_Params.iStartKey;

    OdRxObjectPtr pCurLayout = pDbPE->currentLayout( m_Params.pDb );

    OdDbBaseLayoutPEPtr pLayoutInfo( pCurLayout );

    if( !pLayoutInfo.isNull() )
    {
      if( pLayoutInfo->isModelLayout(pCurLayout) && getParams().bSetCamera  && getParams().iExportFileType != DGN_FILE_TYPE)
      {
        pDeviceHoops->m_bSetCamera = true;
        pCtx->enableGsModel(true);
      }

      pDbPE->loadPlotstyleTableForActiveLayout(pCtx, m_Params.pDb);
      m_pDevice = pDbPE->setupActiveLayoutViews(pDeviceHoops, pCtx);

      if(pRenderDevice.get())      
      {
        pDeviceHoops->setRenderDevice(pRenderDevice);
        pRenderDevice = pDbPE->setupActiveLayoutViews(pRenderDevice, pCtx);
      }

      if( !m_Params.bSetExportRenderModeFrpmLayout )
      {
        for( OdInt32  i = 0; i < pDeviceHoops->numViews(); i++ )
        {
          OdGsView* pView  = pDeviceHoops->viewAt(i);
          pView->setMode(OdGsView::kGouraudShaded);
        }
      }

      pDeviceHoops->enableRecording();
      m_pDevice->update(0);
      pDeviceHoops->enableRecording(false);
    }
    else
      return ::exOdError;
  }

  return ::exOk;
}

HoopsResultEx CHoopsExportImpl::exportFile()
{
  HoopsResultEx exRes = exFileSavingError;

  if( m_Params.strFileName != "" )
  {
    OdString strFileExt = m_Params.strFileName;

    if( strFileExt.getLength() > 4 )
      strFileExt = strFileExt.right(4);

    strFileExt.makeUpper();

    wchar_t file_name[MVO_BUFFER_SIZE];
    char    ascii_name[MVO_BUFFER_SIZE];

    Od_wcscpy( file_name, m_Params.strFileName.c_str() );

    Od_wcstombs( ascii_name, file_name, Od_wcslen(file_name) );
    ascii_name[Od_wcslen(file_name)] = 0;

    if( Od_wcscmp(file_name, L"") )
    {
      if( strFileExt == L_HSF_EXT )
      {
#if defined(_TOOLKIT_IN_DLL_) && defined(ODA_WINDOWS)
        HMODULE hmHoopsStream = ::LoadLibrary( HOOPSSTREAM_DLL_NAME );
        if( hmHoopsStream != NULL  )
#endif
        {
          int sflags = 0;
          sflags |= TK_Full_Resolution_Vertices;
          sflags |= TK_Full_Resolution_Normals;
          sflags |= TK_Full_Resolution_Parameters;
          sflags |= TK_Disable_Instancing;
          sflags |= TK_Disable_Tristrips;

          HStreamFileToolkit * tk = new HStreamFileToolkit();

          HC_Open_Segment_By_Key(m_Params.iStartKey);
          HTK_Write_Stream_File(ascii_name, sflags, tk);
          HC_Close_Segment();
          exRes = exOk;

          delete tk;
#if defined(_TOOLKIT_IN_DLL_) && defined(ODA_WINDOWS)
          ::FreeLibrary( hmHoopsStream );
#endif
        }
      }
      else
      {
        char segment_name[MVO_BUFFER_SIZE];
        HC_Open_Segment_By_Key(m_Params.iStartKey);
        HC_Show_Segment(m_Params.iStartKey,segment_name);
        HC_Write_Metafile(segment_name,ascii_name, "save state = (color names, fonts, textures)" );
        HC_Close_Segment();
        exRes = exOk;
      }
    }

    FreeHoopsModel();
  }
  else
  {
    if( m_Params.bResetSystemExport )
      FreeHoopsModel();

    exRes = exOk;
  }

  return exRes;
}

void CHoopsExportImpl::FreeHoopsModel()
{
  ResetHoops();
}
}
