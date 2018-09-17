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
// 3dDwfExportImpl.cpp : implementation of the C3dDwfExportImpl class
//
///////////////////////////////////////////////////////////////////////////////


#include "OdaCommon.h"

// workaround for bug in certain versions of HP's aCC compiler
#ifdef OD_REDEFINE_STD
namespace std {} using namespace std;
#endif

#define STL_USING_IOSTREAM
#define STL_USING_MAP
#define STL_USING_VECTOR
#include "OdaSTL.h"

#include "Ge/GeExtents3d.h"
#include "OdTimeStamp.h"
#include "ColorMapping.h"
#include "DbBaseHostAppServices.h"

#include "3dDwfExportImpl.h"
#include "3dDwfDrawObject.h"

#if defined(_TOOLKIT_IN_DLL_) && defined(WIN32)
#include "whiptk/palette.h"
#else
extern const WT_RGBA32 WD_Old_Default_Palette[256];
extern const WT_RGBA32 WD_New_Default_Palette[256];
#endif

#ifdef OD_HAVE_DIRECT_FILE
#include <direct.h>
#endif
#include <sys/stat.h>
#include <errno.h>

#include "dwfcore/File.h"
#include "dwfcore/MIME.h"
#include "dwf/Version.h"
#include "dwf/package/Constants.h"
#include "dwf/package/EPlotSection.h"
#include "dwf/package/writer/PackageWriter.h"

#include "dwf/package/ContentManager.h"
#include "dwf/publisher/model/Model.h"
#include "dwf/publisher/impl/DWF6PackagePublisher.h"
#include "dwf/publisher/impl/DWFXPackagePublisher.h"
#ifdef  _DWFCORE_WIN32_SYSTEM
#include "dwf/publisher/win32/EmbeddedFontImpl.h"
#endif


using namespace DWFCore;
using namespace DWFToolkit;
namespace  TD_DWF_EXPORT{
///////////////////////////////////////////////////////////////////////////////
// Auxiliary methods
///////////////////////////////////////////////////////////////////////////////

DWFUnits::teType getUnitName(OdUInt32 idUnit);

//***************************************************************************
// 
//***************************************************************************
}

namespace  TD_DWF_EXPORT{
//***************************************************************************
//  C3dDwfExportImpl
//***************************************************************************

C3dDwfExportImpl::C3dDwfExportImpl(Dwf3dExportParams& Params)
    : m_Params(Params)
{
}

C3dDwfExportImpl::~C3dDwfExportImpl()
{
}

bool C3dDwfExportImpl::run()
{
  try
  {
    m_pPalette = (m_Params.palette() != NULL) ? m_Params.palette()
                                             : odcmAcadPalette(m_Params.background());

    // To create DWF device

    OdGsDevicePtr wr3dDwfDevice = Od3dDwfDevice::createObject();

    // To get Model Space

    OdDbBaseDatabasePEPtr pDbPe(m_Params.database());

    OdRxIteratorPtr pLayoutIter = pDbPe->layouts(m_Params.database());

    OdString  sLayoutNames = OdString::kEmpty;
    OdDbStub* g_idMsLayout = NULL;

    for (; !pLayoutIter->done(); pLayoutIter->next())
    {
      OdRxObjectPtr pLayout = pLayoutIter->object();
      OdDbBaseLayoutPEPtr pLayoutPE( pLayout );

      if( pLayoutPE->isModelLayout(pLayout) )
      {
        sLayoutNames = pLayoutPE->name(pLayout);
        g_idMsLayout = pDbPe->findLayoutNamed(m_Params.database(), sLayoutNames); // dgn dwf export;
        break;
      }
    }

    if (sLayoutNames.isEmpty())
    {
      ODA_ASSERT(false);
    }

    ((Od3dDwfDevice*)wr3dDwfDevice.get())->setContext(this);       // set special context for 3d dwf device

    // To create & activate own Dwf View        
    OdDbBaseDatabasePEPtr pDbPE(m_Params.database());
    OdGiDefaultContextPtr pContex = pDbPE->createGiContext(m_Params.database());
    pContex->setPlotGeneration(false);

    wr3dDwfDevice->setBackgroundColor(m_Params.background());
    wr3dDwfDevice->setLogicalPalette(m_pPalette, 256);
    OdGsDevicePtr pLayoutHelper = pDbPE->setupLayoutView( wr3dDwfDevice, pContex, g_idMsLayout);  // dgn dwf export
    
    wr3dDwfDevice->onSize( OdGsDCRect( 0, m_Params.ySize(), m_Params.xSize(), 0 ) /*outputRect*/ );

    // To create Publisher
    //
    DWFCore::DWFFile oDWF( m_Params.dwfFileName().c_str() );
    OdString ext = m_Params.dwfFileName().mid(m_Params.dwfFileName().reverseFind(L'.') + 1);
    
    std::auto_ptr<DWFPackagePublisher> oPublisher(ext.iCompare(L"dwfx") == 0 ? 
      static_cast<DWFPackagePublisher*>(new DWFXPackagePublisher(oDWF)) : static_cast<DWFPackagePublisher*>(new DWF6PackagePublisher(oDWF)));

    // Starts with a new model
    //
    DWFModel oModel( (m_Params.title().isEmpty())              // Corresponds to the EModel section title
                        ? m_Params.dwfFileName().c_str() 
                        : m_Params.title().c_str() );     
    
        DWFContentManager* pContentManager = DWFCORE_ALLOC_OBJECT( DWFContentManager );
        DWFContent* pContent = pContentManager->getContent();

    OdUInt32 unitId = pDbPE->getUnits(m_Params.database());
    oModel.open( pContent,
                 DWFModel::eHandednessLeft,
                 getUnitName( unitId ),                       // DWFUnits::eCentimeters,
                 NULL,                                        // transform
                 true,                                        // default lights
                 false,                                       // published edges
                 true );                                      // silhouettes
    oModel.setViewCubeCompassState(DWFModel::eViewCubeCompassOn);


    OdGsView* view = wr3dDwfDevice->viewAt(0);
    double dHeight = view->fieldHeight();
    double dWidth = view->fieldWidth();
    OdGeVector3d vtUpVector = view->upVector();
    OdGePoint3d ptTarget = view->target();
    OdGePoint3d ptPosition = view->position();
    
    W3DCamera oDefault;
    OdGeMatrix3d w3DTransform; // 3D DWF is [Front] -> [Top] transformed (cf. #9714)
    w3DTransform.setCoordSystem(OdGePoint3d::kOrigin, OdGeVector3d::kXAxis, -OdGeVector3d::kZAxis, OdGeVector3d::kYAxis);
    OdGeVector3d dir = (ptTarget - ptPosition);
    vtUpVector.transformBy(w3DTransform);
    dir.transformBy(w3DTransform);
    ptPosition = ptTarget - dir;
    oDefault.setProjection( view->isPerspective() ? W3DCamera::ePerspective : W3DCamera::eOrthographic );
    oDefault.setPosition( ptPosition.x, ptPosition.y, ptPosition.z );
    oDefault.setTarget( ptTarget.x, ptTarget.y, ptTarget.z );
    oDefault.setUpVector( vtUpVector.x, vtUpVector.y, vtUpVector.z );
    oDefault.setField( dWidth, dHeight );

    oModel.createView( "default", oDefault ); // This should be done before any scene graphics are added

    m_pModel = &oModel;

    // The first thing to do is create an include segment
    // that defines a collection of graphics that will be
    // reused throughout the model
    //
    DWFIncludeSegment include = oModel.openIncludeSegment();

    // Creates a simple scene
    //
    DWFSegment oRootSegment = oModel.openSegment();
    m_pCurrentSegment = &oRootSegment;

    oRootSegment.open( sLayoutNames.c_str() );
    {

      TK_Color_RGB& rColor = oRootSegment.getColorRGBHandler();
      m_pTKColorRGB = &rColor;

      //todo
      //oRootSegment.addPropertyContainer( pPartProperties ); // Model layout properties

      // Initiate rendering.
      //  Doing rendering to fill Model and to create the graphics, etc.
      wr3dDwfDevice->update( 0 );
    }

    oRootSegment.close();

    // ADD A THUMBNAIL
    //
    if ( !m_Params.thumbnail().sFileName.isEmpty() )
    {
      // open as a streaming binary file
      //
      DWFStreamFileDescriptor* pThumbnailFile = new DWFStreamFileDescriptor( DWFString(m_Params.thumbnail().sFileName.c_str()), OD_T("rb") );
      pThumbnailFile->open();

      // create a stream and attach the file descriptor
      //
      DWFFileInputStream* pThumbnailStream = new DWFFileInputStream;
      pThumbnailStream->attach( pThumbnailFile, false );

      // To specify the image file resource's MIME-type
      DWFString zMimeType;
      if (SpecifyImageMimeType(zMimeType, m_Params.thumbnail().sFileName))
      {

        // create a thumbnail image and attach the file stream
        //
        DWFImage* pThumbnail = DWFCORE_ALLOC_OBJECT( DWFImage( zMimeType, 
                                                               DWFImage::eThumbnail, 
                                                               m_Params.thumbnail().ColorDepth,
                                                               m_Params.thumbnail().Width, 
                                                               m_Params.thumbnail().Height) );

        if (pThumbnail)
        {
          pThumbnail->attach( pThumbnailStream, true );

          // add to the model
          //
          oModel.addResource( pThumbnail );
        }
      }
    }

    // add some properties that apply to the model as a whole
    //
/*  
    oModel.addProperty( DWFCORE_ALLOC_OBJECT(DWFProperty(L"Cutting Plane", L"Hide", L"View Modifier")), true );
    oModel.addProperty( DWFCORE_ALLOC_OBJECT(DWFProperty(L"Origin", L"DWFToolkit Sample Applications", L"General")), true );
    oModel.addProperty( DWFCORE_ALLOC_OBJECT(DWFProperty(L"Project", L"Bianchi", L"General")), true );
*/ 

    // close the model
    //
    oModel.close();

    oModel.publish( *oPublisher );

    // create the DWF
    //
    oPublisher->publish();
  }
  catch (DWFException& ex)
  {
    OdString msg(OD_T("Dwf exception is thrown during export3dDwf: "));
    OdString wsMsg;
    wsMsg = ex.message();
    msg += wsMsg;
    msg += OdString(ex.function());
    wsMsg = ex.file();
    //wsMsg += ex.line();
    msg += wsMsg;

    pmStop();
    pmRelease();

    return false;
  }
	catch (OdError& err)
	{
    OdString msg(OD_T("OdError is thrown during export3dDwf: "));
    msg += err.description();
    (OdDbBaseDatabasePEPtr(m_Params.database()))->appServices(m_Params.database())->warning(msg);

    pmStop();
    pmRelease();

    return false;

  }
  catch (...)
  {
    OdString msg(OD_T("Unexpected error is happened during exportDwf."));
    (OdDbBaseDatabasePEPtr(m_Params.database()))->appServices(m_Params.database())->warning(msg);
    pmStop();
    pmRelease();
    return false;
  }
  return true;
}

///////////////////////////////////////////////////////////////////////////////
// Implementation of auxiliary methods 
///////////////////////////////////////////////////////////////////////////////

DWFUnits::teType getUnitName(OdUInt32 idUnit)
{
  static DWFUnits::teType const Units[] = 
  {
    DWFUnits::eMillimeters ,  //"", /* 0	Unspecified (No units) */
    DWFUnits::eInches ,  // "Inches",
    DWFUnits::eFeet ,  // "Feet",
    DWFUnits::eFeet ,  // "Miles",
    DWFUnits::eMillimeters ,  // "Millimeters",
    DWFUnits::eCentimeters ,  // "Centimeters",
    DWFUnits::eMeters ,  // "Meters",
    DWFUnits::eMeters ,  // "Kilometers",
    DWFUnits::eMillimeters ,  // "Microinches",
    DWFUnits::eFeet ,  // "Mils",
    DWFUnits::eFeet ,  // "Yards",
    DWFUnits::eMillimeters ,  // "Angstroms",
    DWFUnits::eMillimeters ,  // "Nanometers",
    DWFUnits::eMillimeters ,  // "Microns",
    DWFUnits::eMillimeters ,  // "Decimeters",
    DWFUnits::eMeters ,  // "Decameters",
    DWFUnits::eMeters ,  // "Hectometers",
    DWFUnits::eMeters ,  // "Gigameters",
    DWFUnits::eMeters ,  // "Astronomical Units",
    DWFUnits::eMeters ,  // "Light Years",
    DWFUnits::eMeters ,  // "Parsecs"
  };

  if (idUnit > 20)
  {
    idUnit = 0;
  }

  return Units[idUnit];
}
}
