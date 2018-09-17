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
// XmlGlesLoader.cpp

#include "OdaCommon.h"
#include "XmlGlesLoader.h"
#include "TrVisUniqueId.h"
#include "CloudTools.h"
#include "OdFakeDbStub.h"

#ifdef OD_TGS_ENABLED
#undef ODA_ASSERT_ONCE
#define ODA_ASSERT_ONCE(a)
#endif

//SEA 4/18/2017
//OdAnsiString ::Fromat has ASSERT on PRIX64 in non-x86 and non-Alpha case
//Fix for Win64
#ifdef WIN64
#define PTRX64FORMAT "%llX"
#else
#define PTRX64FORMAT PRIX64
#endif

///////////////////////////////////////////////////////////////////////////////

//static OdSharedPtr<std::map<OdAnsiString, OdSharedPtr<OdAnsiString> > > s_pMapFakeDbStub;
// redesigned to way of CspGsModel (to be compatible with current Core/Extensions/ExRender/GLES2/remote/JS-API.cpp)
// see also  Core/Extensions/ExRender/GLES2/remote/CspGsModel.h
static OdSharedPtr<std::map<OdAnsiString, OdSharedPtr<OdFakeDbStub> > > s_pMapFakeDbStub;

OdXmlGlesLoader::OdXmlGlesLoader( OdTrVisRendition* pRendition, OdXmlGlesLoaderIdMapper* pIdMapper )
  : m_pIdMapper( pIdMapper )
  , m_pRendition(pRendition)
  , m_typeElement(OdTrVisArrayWrapper::Type_Invalid) // undef
  , m_indexElement(0)
  , m_numDataElement(0)
  , m_uaMarkerIndex(0)
  , m_kindOfModViewportModified(0)
  , m_pVisualStyleProperty(NULL)
  , m_typeVisualStyle(OdGiVariant::kUndefined)
  , m_metafileChangedFlag(0xFF)
{
  ODA_ASSERT_ONCE(m_pRendition);
  m_bDestroyIdMapped = false;
  if( !m_pIdMapper )
  {
    m_pIdMapper = new OdXmlGesIdMapper;
    m_bDestroyIdMapped = true;
  }
  m_pMapFakeDbStub = m_pIdMapper->getMapFakeDbStub();
  s_pMapFakeDbStub = m_pMapFakeDbStub; // to hold this map after destroying of OdXmlGlesLoader

  m_mfUserEntryNumData = 0;
}

OdXmlGlesLoader::~OdXmlGlesLoader()
{
  if( m_bDestroyIdMapped )
  {
    delete m_pIdMapper;
  }
}

OdTrVisMetafileContainer* OdXmlGlesLoader::metafile()
{
  if (m_pMetafileDef.get())
  {
    if (m_pMetafileDef->m_pMetafile.isNull())
    {
      m_pMetafileDef->m_pMetafile = new OdTrVisMetafileContainer;
      m_pMetafileDef->m_pMetafile->setDefault();
    }
    return m_pMetafileDef->m_pMetafile.get();
  }
  if (m_pBackgroundDef.get())
  {
    if (m_pBackgroundDef->m_pStream.isNull())
    {
      m_pBackgroundDef->m_pStream = new OdTrVisMetafileContainer;
      static_cast<OdTrVisMetafileContainer*>(m_pBackgroundDef->m_pStream.get())->setDefault();
    }
    return (OdTrVisMetafileContainer*) m_pBackgroundDef->m_pStream.get();
  }
  return NULL;
}

#define PK_CASE(enKey) case pk_##enKey
#define PK_CASE_EX(enKey) case pk_##MetafileAdded_MetafileData##enKey: case pk_##BackgroundChanged_MetafileData##enKey

bool OdXmlGlesLoader::DropBegin(OdEnPathKey pk)
{
  switch (pk)
  {
  PK_CASE(MaterialAdded):
    ODA_ASSERT_ONCE(m_pMaterialDef.isNull()); 
    m_pMaterialDef = new OdTrVisMaterialDef();
    m_pMaterialDef->setDefault();
    break;
  PK_CASE(MaterialDeleted):
    m_id = OdTrVisId(0);
    break;
  PK_CASE(TextureAdded):
    ODA_ASSERT_ONCE(m_pTextureDef.isNull());
    m_pTextureDef = new OdTrVisTextureDef();
    m_pTextureDef->setDefault();
    m_curTexture.isValid = true;
    break;
  PK_CASE(TextureDeleted):
    m_id = OdTrVisId(0);
    break;
  PK_CASE(OverlayAdded):
  PK_CASE(OverlayVisibilityChanged):
  PK_CASE(OverlayRenderOrderChanged):
    ODA_ASSERT_ONCE(m_pOverlayDef.isNull());
    m_pOverlayDef = new OdTrVisOverlayDef();
    m_pOverlayDef->setDefault();
    break;
  PK_CASE(OverlayDeleted):
    m_id = OdTrVisId(0);
    break;
  PK_CASE(OverlayViewParamsOverride):
    ODA_ASSERT_ONCE(m_pOverlayViewParamsOverrideDef.isNull());
    m_pOverlayViewParamsOverrideDef = new OdGLES2OverlayViewParamsOverrideDef();
    m_pOverlayViewParamsOverrideDef->setDefault();
    break;
  PK_CASE(ViewportAdded): PK_CASE(ViewportModified):
    ODA_ASSERT_ONCE(m_pViewportDef.isNull());
    m_pViewportDef = new OdTrVisViewportDef();
    m_pViewportDef->setDefault(),
    m_kindOfModViewportModified = 0;
    break;
  PK_CASE(ViewportDeleted):
    m_id = 0;
    break;
  PK_CASE(ViewportInserted):
    m_id = 0;
    m_nViewportInsertedIndex = 0;
    break;
  PK_CASE(ViewportModified_ViewParams):
  PK_CASE(OverlayViewParamsOverride_ViewParams):
    ODA_ASSERT_ONCE(m_pViewParamsDef.isNull());
    m_pViewParamsDef = new OdTrVisViewParamsDef();
    m_pViewParamsDef->setDefault();
    break;
  PK_CASE(VisualStyleAdded): PK_CASE(VisualStyleModified):
    ODA_ASSERT_ONCE(m_pVisualStyleDef.isNull() && !m_pVisualStyleProperty && m_typeVisualStyle == OdGiVariant::kUndefined);
    m_pVisualStyleDef = new OdTrVisVisualStyle();
    break;
  PK_CASE(VisualStyleAdded_Prop): PK_CASE(VisualStyleModified_Prop):
    ODA_ASSERT_ONCE(m_pVisualStyleDef.get() && !m_pVisualStyleProperty && m_typeVisualStyle == OdGiVariant::kUndefined);
    break;
  PK_CASE(VisualStyleDeleted):
    m_id = OdTrVisId(0);
    break;
  PK_CASE(LayerAdded): PK_CASE(LayerModified):
    ODA_ASSERT_ONCE(m_pLayerDef.isNull());
    m_pLayerDef = new OdTrVisLayerDef();
    m_pLayerDef->setDefault();
    break;
  PK_CASE(LayerDeleted):
    ODA_ASSERT_ONCE(m_pLayerDef.isNull());
    m_id = OdTrVisId(0);
    break;
  //PK_CASE(LayerAdded_VpDepSpecs_VpDepSpec): PK_CASE(LayerModified_VpDepSpecs_VpDepSpec):
  //  break;
  PK_CASE(MetafileAdded):
    ODA_ASSERT_ONCE(m_pMetafileDef.isNull()); 
    m_pMetafileDef = new OdTrVisMetafileDef(),
    m_pMetafileDef->setDefault();
    break;
  PK_CASE_EX(_MetafileStream):
    if (metafile())
    {
      ODA_ASSERT_ONCE(m_pWriter.isNull());
      m_pWriter = static_cast<OdTrVisFlatMetafileContainerWriter*>(metafile()->createWriter());
      m_pWriter->setContainer(metafile());
      m_streamWriter.setContainerIO(m_pWriter);
    }
    break;
  PK_CASE_EX(_Array_Data):
    if (metafile())
    {
      ODA_ASSERT_ONCE(   m_typeElement == OdTrVisArrayWrapper::Type_Marker
                      && m_indexElement < metafile()->m_ArrayElements.size()
                      && !m_uaMarkerIndex && !m_aMarkers.size()
                      && m_numDataElement);
      m_uaMarkerIndex = 0;
      m_aMarkers.resize(m_numDataElement);
    }
    break;
  PK_CASE_EX(_MetafileStream_UninitTexture):
    ODA_ASSERT_ONCE(m_pWriter.get());
    m_streamWriter.writeUInt8((OdUInt8) OdTrVisMetaRecType_UninitTexture);
    break;
  PK_CASE_EX(_MetafileStream_PopMatrix):
    ODA_ASSERT_ONCE(m_pWriter.get());
    m_streamWriter.writeUInt8((OdUInt8) OdTrVisMetaRecType_PopMatrix);
    break;
  PK_CASE(SurfaceChanged):
    ODA_ASSERT_ONCE(m_pSurfaceDef.isNull());
    m_pSurfaceDef = new OdTrVisSurfaceDef(),
    m_pSurfaceDef->setDefault();
    break;
  PK_CASE(ExtentsChanged):
    ODA_ASSERT_ONCE(m_pExtentsDef.isNull());
    m_pExtentsDef = new OdGLES2ExtentsChangedDef(),
    m_pExtentsDef->setDefault();
    break;
  PK_CASE(MetafileOrderChanged):
    ODA_ASSERT_ONCE(m_pMetafileOrderDef.isNull());
    m_pMetafileOrderDef = new OdGLES2MetafileOrderDef(),
    m_pMetafileOrderDef->setDefault();
    break;
  PK_CASE(MetafileOrderInheritance):
    ODA_ASSERT_ONCE(m_pMetafileOrderInheritance.isNull());
    m_pMetafileOrderInheritance = new OdGLES2MetafileOrderInheritanceDef(),
    m_pMetafileOrderInheritance->setDefault();
    break;
  PK_CASE(LightsListChanged):
    ODA_ASSERT_ONCE(m_pLightsListDef.isNull());
    m_pLightsListDef = new OdGLES2LightsListDef(),
    m_pLightsListDef->setDefault();
    break;
  PK_CASE(BackgroundChanged):
    ODA_ASSERT_ONCE(m_pBackgroundDef.isNull());
    m_pBackgroundDef = new OdGLES2BackgroundDef(),
    m_pBackgroundDef->setDefault();
    break;
  PK_CASE(MetafileDeleted):
    m_id = OdTrVisId(0);
    break;
  PK_CASE(MetafileVisibilityChanged):
  PK_CASE(MetafileHighlightingChanged):
  PK_CASE(MetafileFadingChanged):
    m_metafileChangedFlag = 0xFF;
    break;
  PK_CASE(HlBranchAdded):
  PK_CASE(HlBranchModified):
    ODA_ASSERT_ONCE(m_pHlBranchDef.isNull()); 
    m_pHlBranchDef = new OdTrVisHlBranchDef();
    break;
  PK_CASE(HlBranchAdded_Childs):
  PK_CASE(HlBranchModified_Childs):
    ODA_ASSERT_ONCE( m_pHlBranchDef.get() );
    break;
  PK_CASE(HlBranchAdded_Markers):
  PK_CASE(HlBranchModified_Markers):
    ODA_ASSERT_ONCE( m_pHlBranchDef.get() );
    break;
  PK_CASE(MetafileHlBranchAttached):
    m_id = 0;
    m_HlBranchId = 0;
    break;
  PK_CASE(MetafileHlBranchDetached):
    m_id = 0;
    break;
  PK_CASE(HlBranchDeleted):
    m_id = 0;
    break;
  PK_CASE( LayerViewportPropsOverride ):
    m_id = 0;
    m_overrideLayerId = 0;
    m_bOverrideLayer = false;
    m_overrideLayerProps = OdTrVisLayerProps();
    break;
  }
  return true;
}

bool OdXmlGlesLoader::DropEnd(OdEnPathKey pk)
{
  switch (pk)
  {
  //if (m_pProgramDef.get())
  //{
  //  m_pProgramDef = NULL;
  //  ODA_FAIL_ONCE(); // TODO
  //  return false;
  //}
  PK_CASE(MaterialAdded):
    ODA_ASSERT_ONCE(m_pMaterialDef.get() && m_id > OdTrVisId(100));
    m_pRendition->onMaterialAdded((OdTrVisMetafileId) m_id, *m_pMaterialDef.get());
    m_id = OdTrVisId(0);
    m_pMaterialDef = NULL;
    break;
  PK_CASE(MaterialDeleted):
    ODA_ASSERT_ONCE( m_id > OdTrVisId(0) );
    m_pRendition->onMaterialDeleted( m_id );
    m_id = OdTrVisId(0);
    break;
  PK_CASE(TextureAdded):
    ODA_ASSERT_ONCE(m_pTextureDef.get() && m_id > OdTrVisId(100));
    m_pRendition->onTextureAdded((OdTrVisTextureId) m_id, *m_pTextureDef.get());
    m_id = OdTrVisId(0);
    m_pTextureDef = NULL;
    break;
  PK_CASE(TextureDeleted):
    ODA_ASSERT_ONCE( m_id > OdTrVisId(0) );
    m_pRendition->onTextureDeleted( m_id );
    m_id = OdTrVisId(0);
    break;
  PK_CASE(OverlayAdded):
    ODA_ASSERT_ONCE(m_pOverlayDef.get() && m_id > OdTrVisId(100));
    m_pRendition->onOverlayAdded((OdTrVisOverlayId) m_id, *m_pOverlayDef.get());
    m_id = OdTrVisId(0);
    m_pOverlayDef = NULL;
    break;
  PK_CASE(OverlayDeleted):
    ODA_ASSERT_ONCE(m_pOverlayDef.isNull() && m_id > OdTrVisId(100));
    m_pRendition->onOverlayDeleted((OdTrVisOverlayId) m_id);
    m_id = OdTrVisId(0);
    break;
  PK_CASE(OverlayVisibilityChanged):
    ODA_ASSERT_ONCE(m_pOverlayDef.get() && m_id > OdTrVisId(100));
    m_pRendition->onOverlayVisibilityChanged((OdTrVisOverlayId) m_id, m_pOverlayDef->isOverlayEnabled());
    m_id = OdTrVisId(0);
    m_pOverlayDef = NULL;
    break;
  PK_CASE(OverlayRenderOrderChanged):
    ODA_ASSERT_ONCE(m_pOverlayDef.get() && m_id > OdTrVisId(100));
    m_pRendition->onOverlayRenderOrderChanged((OdTrVisOverlayId) m_id, m_pOverlayDef->m_renderOrder);
    m_id = OdTrVisId(0);
    m_pOverlayDef = NULL;
    break;
  PK_CASE(OverlayViewParamsOverride):
    ODA_ASSERT_ONCE(m_pOverlayViewParamsOverrideDef.get());
    m_pRendition->onOverlayViewParamsOverride(m_pOverlayViewParamsOverrideDef->m_viewportId, m_pOverlayViewParamsOverrideDef->m_overlayId,
                                              m_pOverlayViewParamsOverrideDef->m_bOverride, m_pOverlayViewParamsOverrideDef->m_viewParams);
    m_pOverlayViewParamsOverrideDef = NULL;
    break;
  PK_CASE(ViewportAdded):
    ODA_ASSERT_ONCE(m_pViewportDef.get() && m_id > OdTrVisId(100));
    m_pRendition->onViewportAdded((OdTrVisViewportId) m_id);
    m_id = OdTrVisId(0);
    m_pViewportDef = NULL;
    m_kindOfModViewportModified = 0;
    break;
  PK_CASE(ViewportDeleted):
    ODA_ASSERT_ONCE( m_id > OdTrVisId(100) );
    m_pRendition->onViewportDeleted( m_id );
    m_id = OdTrVisId(0);
    break;
  PK_CASE(ViewportInserted):
    ODA_ASSERT_ONCE( m_id > OdTrVisId(100) );
    m_pRendition->onViewportInserted( m_id, m_nViewportInsertedIndex );
    m_id = OdTrVisId(0);
    break;
  PK_CASE(ViewportModified):
    ODA_ASSERT_ONCE(m_pViewportDef.get() && m_id > OdTrVisId(100));
    m_pRendition->onViewportModified((OdTrVisViewportId) m_id, *m_pViewportDef.get(), m_kindOfModViewportModified);
    m_id = OdTrVisId(0);
    m_pViewportDef = NULL;
    m_kindOfModViewportModified = 0;
    break;
  PK_CASE(ViewportModified_ViewParams):
    ODA_ASSERT_ONCE(m_pViewportDef.get() && m_pViewParamsDef.get());
    m_pViewportDef->m_viewParams = *m_pViewParamsDef;
    m_pViewParamsDef = NULL;
    break;
  PK_CASE(OverlayViewParamsOverride_ViewParams):
    ODA_ASSERT_ONCE(m_pOverlayViewParamsOverrideDef.get() && m_pViewParamsDef.get());
    m_pOverlayViewParamsOverrideDef->m_viewParams = *m_pViewParamsDef;
    m_pViewParamsDef = NULL;
    break;
  PK_CASE(VisualStyleAdded):
    ODA_ASSERT_ONCE(   m_pVisualStyleDef.get() && m_id > OdTrVisId(100) 
                    && !m_pVisualStyleProperty && m_typeVisualStyle == OdGiVariant::kUndefined);
    m_pRendition->onVisualStyleAdded((OdTrVisVisualStyleId) m_id, *m_pVisualStyleDef.get());
    m_id = OdTrVisId(0);
    m_pVisualStyleDef = NULL; 
    break;
  PK_CASE(VisualStyleModified):
    ODA_ASSERT_ONCE(   m_pVisualStyleDef.get() && m_id > OdTrVisId(100) 
                    && !m_pVisualStyleProperty && m_typeVisualStyle == OdGiVariant::kUndefined);
    m_pRendition->onVisualStyleModified((OdTrVisVisualStyleId) m_id, *m_pVisualStyleDef.get());
    m_id = OdTrVisId(0);
    m_pVisualStyleDef = NULL; 
    break;
  PK_CASE(VisualStyleAdded_Prop): PK_CASE(VisualStyleModified_Prop):
    ODA_ASSERT_ONCE(m_pVisualStyleDef.get() && m_pVisualStyleProperty && m_typeVisualStyle != OdGiVariant::kUndefined);
    m_pVisualStyleProperty->setModified(true);
    m_pVisualStyleProperty = NULL;
    m_typeVisualStyle = OdGiVariant::kUndefined;
    break;
  PK_CASE(VisualStyleDeleted):
    ODA_ASSERT_ONCE(m_pVisualStyleDef.isNull() && m_id > OdTrVisId(100));
    m_pRendition->onVisualStyleDeleted((OdTrVisVisualStyleId) m_id);
    m_id = OdTrVisId(0);
    break;
  PK_CASE(LayerAdded):
    ODA_ASSERT_ONCE(m_pLayerDef.get() && m_id > OdTrVisId(100));
    m_pRendition->onLayerAdded((OdTrVisLayerId) m_id, *m_pLayerDef.get());
    m_id = OdTrVisId(0);
    m_pLayerDef = NULL; 
    break;
  PK_CASE(LayerModified):
    ODA_ASSERT_ONCE(m_pLayerDef.get() && m_id > OdTrVisId(100));
    m_pRendition->onLayerModified((OdTrVisLayerId) m_id, *m_pLayerDef.get());
    m_id = OdTrVisId(0);
    m_pLayerDef = NULL; 
    break;
  PK_CASE(LayerDeleted):
    ODA_ASSERT_ONCE(m_pLayerDef.isNull() && m_id > OdTrVisId(100));
    m_pRendition->onLayerDeleted((OdTrVisLayerId) m_id);
    m_id = OdTrVisId(0);
    break;
  PK_CASE(MetafileAdded):
    ODA_ASSERT_ONCE(m_pMetafileDef.get() && m_id > OdTrVisId(100));
    m_pRendition->onMetafileAdded((OdTrVisMetafileId) m_id, *m_pMetafileDef.get());
    m_id = OdTrVisId(0);
    m_pMetafileDef = NULL;
    break;
  PK_CASE_EX(_Array_Data):
    if (metafile())
    {
      ODA_ASSERT_ONCE(   m_typeElement == OdTrVisArrayWrapper::Type_Marker
                      && m_indexElement < metafile()->m_ArrayElements.size()
                      && m_aMarkers.size() == m_numDataElement
                      && m_uaMarkerIndex == m_numDataElement); 
      OdTrVisArrayWrapper& array = metafile()->m_ArrayElements[m_indexElement++];
      array.setArray(m_typeElement, m_aMarkers.getPtr(), m_aMarkers.size() * sizeof(OdTrVisDefProcMark));
      m_uaMarkerIndex = 0;
      m_aMarkers.resize(0);
      m_typeElement = OdTrVisArrayWrapper::Type_Invalid;
      m_numDataElement = 0;
    }
    break;
  PK_CASE_EX(_MetafileStream):
    ODA_ASSERT_ONCE(m_pWriter.get());
    m_streamWriter.setContainerIO(NULL);
    m_pWriter->completeWriting();
    m_pWriter = NULL;
    break;
  PK_CASE(SurfaceChanged):
    ODA_ASSERT_ONCE(m_pSurfaceDef.get());
    m_pRendition->onSurfaceChanged(*m_pSurfaceDef.get());
    m_id = OdTrVisId(0);
    m_pSurfaceDef = NULL;
    break;
  PK_CASE(ExtentsChanged):
    ODA_ASSERT_ONCE(m_pExtentsDef.get());
    m_pRendition->onExtentsChanged(m_pExtentsDef->m_viewportId, m_pExtentsDef->m_overlayId, *m_pExtentsDef);
    m_pExtentsDef = NULL;
    break;
  PK_CASE(MetafileOrderChanged):
    ODA_ASSERT_ONCE(   m_pMetafileOrderDef.get() 
                    && !m_pMetafileOrderDef->m_nInvalidFrom && !m_pMetafileOrderDef->m_nInvalidLen); // TODO
    m_pRendition->onMetafileOrderChanged(m_pMetafileOrderDef->m_viewportId,
                                         m_pMetafileOrderDef->m_overlayId,
                                         m_pMetafileOrderDef->m_nInvalidFrom,
                                         m_pMetafileOrderDef->m_nInvalidLen,
                                         m_pMetafileOrderDef->m_metafileOrder.getPtr(),
                                         m_pMetafileOrderDef->m_metafileOrder.size());
    m_pMetafileOrderDef = NULL;
    break;
  PK_CASE(MetafileOrderInheritance):
    ODA_ASSERT_ONCE(m_pMetafileOrderInheritance.get());
    m_pRendition->onMetafileOrderInheritance( m_pMetafileOrderInheritance->m_viewportId1,
                                              m_pMetafileOrderInheritance->m_overlayId,
                                              m_pMetafileOrderInheritance->m_viewportId2 );
    break;
  PK_CASE(LightsListChanged):
    ODA_ASSERT_ONCE(m_pLightsListDef.get());
    m_pRendition->onLightsListChanged(m_pLightsListDef->m_viewportId, m_pLightsListDef->m_ambientLight, m_pLightsListDef->m_bDefaultLightsOn,
                                      m_pLightsListDef->m_Defs.getPtr(), m_pLightsListDef->m_Defs.size());
    m_pLightsListDef = NULL;
    break;
  PK_CASE(BackgroundChanged):
    ODA_ASSERT_ONCE(m_pBackgroundDef.get());
    m_pRendition->onBackgroundChanged(m_pBackgroundDef->m_viewportId, m_pBackgroundDef->m_pStream, m_pBackgroundDef->m_baseProgramId);
    m_pBackgroundDef = NULL;
    break;
  PK_CASE(MetafileDeleted):
    ODA_ASSERT_ONCE(!m_pMetafileDef.get() && m_id > OdTrVisId(100));
    m_pRendition->onMetafileDeleted((OdTrVisMetafileId) m_id);
    m_id = OdTrVisId(0);
    break;
  PK_CASE(MetafileVisibilityChanged):
    ODA_ASSERT_ONCE(m_id > OdTrVisId(100) && m_metafileChangedFlag <= 1);
    m_pRendition->onMetafileVisibilityChanged((OdTrVisMetafileId)m_id, !!m_metafileChangedFlag);
    break;
  PK_CASE(MetafileHighlightingChanged):
    ODA_ASSERT_ONCE(m_id > OdTrVisId(100) && m_metafileChangedFlag <= 1);
    m_pRendition->onMetafileHighlightingChanged((OdTrVisMetafileId)m_id, !!m_metafileChangedFlag);
    break;
  PK_CASE(MetafileFadingChanged):
    ODA_ASSERT_ONCE(m_id > OdTrVisId(100) && m_metafileChangedFlag <= 1);
    m_pRendition->onMetafileFadingChanged((OdTrVisMetafileId)m_id, !!m_metafileChangedFlag);
    break;
  PK_CASE(HlBranchAdded):
    ODA_ASSERT_ONCE( m_pHlBranchDef.get() && m_id > OdTrVisId(100) ); 
    m_pRendition->onHlBranchAdded( m_id, *m_pHlBranchDef.get() );
    m_pHlBranchDef = NULL;
    break;
  PK_CASE(HlBranchAdded_Childs):
  PK_CASE(HlBranchAdded_Markers):
  PK_CASE(HlBranchModified_Childs):
  PK_CASE(HlBranchModified_Markers):
    ODA_ASSERT_ONCE( m_pHlBranchDef.get() && m_id > OdTrVisId(100) );
    break;
  PK_CASE(MetafileHlBranchAttached):
    ODA_ASSERT_ONCE( m_id > OdTrVisId(100) && m_HlBranchId > OdTrVisId(100) ); // TODO
    m_pRendition->onMetafileHlBranchAttached( m_id, m_HlBranchId );
    break;
  PK_CASE(HlBranchModified):
    ODA_ASSERT_ONCE( m_pHlBranchDef.get() && m_id > OdTrVisId(100) ); 
    m_pRendition->onHlBranchModified( m_id, *m_pHlBranchDef.get(), !m_pHlBranchDef->m_childs.empty(), !m_pHlBranchDef->m_markers.empty() );
    m_pHlBranchDef = NULL;
  PK_CASE( HlBranchDeleted ):
    ODA_ASSERT_ONCE( m_id > OdTrVisId(100) ); 
    m_pRendition->onHlBranchDeleted( m_id );
    break;
  PK_CASE(MetafileHlBranchDetached):
    ODA_ASSERT_ONCE( m_id > OdTrVisId(100) ); 
    m_pRendition->onMetafileHlBranchDetached( m_id );
    break;
  PK_CASE( LayerViewportPropsOverride ):
    ODA_ASSERT_ONCE( m_id > OdTrVisId(100) && m_overrideLayerId > OdTrVisId(100) );
    m_pRendition->onLayerViewportPropsOverride( m_id, m_overrideLayerId, m_bOverrideLayer, m_overrideLayerProps );
    break;
  }

  return true;
}

bool OdXmlGlesLoader::DropId(OdEnPathKey pk, const OdAnsiString& asValue)
{
  ODA_ASSERT_ONCE(!asValue.isEmpty());

  // unfortunately asValue canbe same for kTextureData & kMaterialData //OdAnsiString asKey = asValue;
  OdAnsiString asKey;
  OdTrVisRendition::DataType typeToRegId = (enum  OdTrVisRendition::DataType) -1; // undef
  switch (pk)
  {
  PK_CASE(TextureAdded_TextureID):
  PK_CASE(MaterialAdded_TextureID):
  PK_CASE(TextureDeleted_TextureID):
  PK_CASE_EX(_MetafileStream_InitTexture_TextureID):
    asKey = "textureid"; 
    typeToRegId = OdTrVisRendition::kTextureData;
    break;
  PK_CASE(MaterialAdded_MaterialID):
  PK_CASE(MaterialDeleted_MaterialID):
  PK_CASE_EX(_MetafileStream_Material_MaterialID):
    asKey = "materialid"; 
    typeToRegId = OdTrVisRendition::kMaterialData;
    break;
  PK_CASE(MetafileAdded_MetafileID):
  PK_CASE_EX(_MetafileStream_TtfText_Text_Char_MetafileID):
  PK_CASE_EX(_MetafileStream_Metafile_MetafileID):
  PK_CASE_EX(_MetafileStream_MetafileMarker_Marker):
  PK_CASE(MetafileOrderChanged_Ordering_MetafileID):
  PK_CASE(MetafileHighlightingChanged_MetafileID):
  PK_CASE(MetafileVisibilityChanged_MetafileID):
  PK_CASE(MetafileFadingChanged_MetafileID):
  PK_CASE(MetafileDeleted_MetafileID):
  PK_CASE(MetafileHlBranchAttached_MetafileID):
  PK_CASE(MetafileHlBranchDetached_MetafileID):
  PK_CASE_EX(_MetafileStream_Program_ProgramID):
    asKey = "metafileid"; 
    typeToRegId = OdTrVisRendition::kMetafileData;
    break;
  PK_CASE(MetafileAdded_GeneratorID):
  PK_CASE(ViewportAdded_ViewportID): PK_CASE(ViewportModified_ViewportID):
  PK_CASE(ViewportDeleted_ViewportID):
  PK_CASE(ViewportInserted_ViewportID):
  PK_CASE(ViewportModified_OverallVpId):
  PK_CASE(MetafileOrderChanged_ViewportID):
  PK_CASE(MetafileOrderInheritance_ViewportID1):
  PK_CASE(MetafileOrderInheritance_ViewportID2):
  PK_CASE(ExtentsChanged_ViewportID):
  PK_CASE(LightsListChanged_ViewportID):
  PK_CASE(BackgroundChanged_ViewportID):
  PK_CASE(OverlayViewParamsOverride_ViewportID):
  PK_CASE(ViewportModified_ShaderId):
  PK_CASE(LayerViewportPropsOverride_ViewportID):
    asKey = "viewportid";
    typeToRegId = OdTrVisRendition::kViewportData;
    break;
  PK_CASE(ViewportModified_VisualStyleId):
  PK_CASE(VisualStyleAdded_VisualStyleID): PK_CASE(VisualStyleModified_VisualStyleID): 
  PK_CASE(VisualStyleDeleted_VisualStyleID):
  PK_CASE_EX( _MetafileStream_VisualStyle_VisualStyleID ):
    asKey = "visualstyleid";
    typeToRegId = OdTrVisRendition::kVisualStyleData;
    break;
  PK_CASE(MetafileAdded_LayerID):
  PK_CASE(LayerAdded_LayerID): PK_CASE(LayerModified_LayerID): PK_CASE(LayerDeleted_LayerID):
  PK_CASE(LayerViewportPropsOverride_LayerID):
    asKey = "layerid";
    typeToRegId = OdTrVisRendition::kLayerData;
    break;
  PK_CASE(ExtentsChanged_OverlayID):
  PK_CASE(MetafileOrderChanged_OverlayID):
  PK_CASE(OverlayAdded_OverlayID):
  PK_CASE(OverlayDeleted_OverlayID):
  PK_CASE(OverlayVisibilityChanged_OverlayID):
  PK_CASE(OverlayRenderOrderChanged_OverlayID):
  PK_CASE(OverlayViewParamsOverride_OverlayID):
  PK_CASE(MetafileOrderInheritance_OverlayID):
    asKey = "overlayid";
    typeToRegId = OdTrVisRendition::kOverlayData;
    break;
  PK_CASE(HlBranchAdded_HlBranchID):
  PK_CASE(HlBranchAdded_Childs_HlBranchID):
  PK_CASE(MetafileHlBranchAttached_HlBranchID):
  PK_CASE(HlBranchModified_HlBranchID):
  PK_CASE(HlBranchModified_Childs_HlBranchID):
  PK_CASE(HlBranchDeleted_HlBranchID):
    asKey = "hlbranchid";
    typeToRegId = OdTrVisRendition::kHlBranchData;
    break;
  PK_CASE(BackgroundChanged_BaseProgramID):
    asKey = "backgroundid";
    typeToRegId = OdTrVisRendition::kBackgroundData;
    break;
  default:
    ODA_FAIL_ONCE();
    return false;
  }
  asKey += "-";
  asKey += asValue;

  OdTrVisId id = OdTrVisId(0);
  bool isNewId = !m_pIdMapper->hasId( asValue, typeToRegId);
  if (isNewId && (asValue == "0")) // special (TextureID ...)
  {
    isNewId = false;
    id = OdTrVisId(0);
  }
  else
  {
    id = m_pIdMapper->getId( asValue, typeToRegId );
  }
  ODA_ASSERT_ONCE(id > OdTrVisId(100) || id == OdTrVisId(0));

  //SEA
  //Let's check ID: if it new but we expect that object should already exist
  //or if it not new but we expect that object should be new
  //let's throw exception
  if( isNewId )
  {
    switch( pk )
    {
    PK_CASE(MetafileHlBranchAttached_MetafileID):
    PK_CASE(MetafileHlBranchDetached_MetafileID):
    PK_CASE(MetafileFadingChanged_MetafileID):
    PK_CASE(MetafileVisibilityChanged_MetafileID):
    PK_CASE(MetafileHighlightingChanged_MetafileID):
    PK_CASE(MetafileOrderChanged_Ordering_MetafileID):
      throw OdError( eInvalidInput );
    }
  }
  else
  {
    switch( pk )
    {
    PK_CASE(MetafileAdded_MetafileID):
      throw OdError( eInvalidInput );
    }
  }

  switch (pk)
  {
  PK_CASE(TextureAdded_TextureID):
    ODA_ASSERT_ONCE(m_pTextureDef.get());
    m_id = id;
    break;
  PK_CASE(TextureDeleted_TextureID):
    m_id = id;
    break;
  PK_CASE(MaterialAdded_MaterialID):
    ODA_ASSERT_ONCE(m_pMaterialDef.get());
    m_id = id;
    break;
  PK_CASE(MaterialDeleted_MaterialID):
    m_id = id;
    break;
  PK_CASE(MaterialAdded_TextureID):
    ODA_ASSERT_ONCE(m_pMaterialDef.get());
    m_pMaterialDef->m_textureId = id;
    break;
  PK_CASE(VisualStyleAdded_VisualStyleID): PK_CASE(VisualStyleModified_VisualStyleID):
  PK_CASE(VisualStyleDeleted_VisualStyleID):
    ODA_ASSERT_ONCE(m_pVisualStyleDef.get());
    m_id = id;
    break;
  PK_CASE(LayerAdded_LayerID): PK_CASE(LayerModified_LayerID): 
    ODA_ASSERT_ONCE(m_pLayerDef.get());
  PK_CASE(LayerDeleted_LayerID):
    m_id = id;
    break;
  PK_CASE( LayerViewportPropsOverride_ViewportID ):
    m_id = id;
    break;
  PK_CASE( LayerViewportPropsOverride_LayerID ):
    m_overrideLayerId = id;
    break;
  PK_CASE(MetafileAdded_MetafileID):
    ODA_ASSERT_ONCE(m_pMetafileDef.get());
    m_id = id;
    break;
  PK_CASE(MetafileDeleted_MetafileID):
  PK_CASE(MetafileVisibilityChanged_MetafileID):
  PK_CASE(MetafileHighlightingChanged_MetafileID):
  PK_CASE(MetafileFadingChanged_MetafileID):
    ODA_ASSERT_ONCE(!m_pMetafileDef.get());
    m_id = id;
    break;
  PK_CASE(MetafileAdded_LayerID):
    ODA_ASSERT_ONCE(m_pMetafileDef.get());
    metafile()->m_pLayer = id;
    break;
  PK_CASE(MetafileAdded_GeneratorID):
    ODA_ASSERT_ONCE(m_pMetafileDef.get());
    m_pMetafileDef->m_generatorId = id;
    break;
  PK_CASE_EX(_MetafileStream_Material_MaterialID):
    ODA_ASSERT_ONCE(m_pWriter.get());
    m_streamWriter.writeUInt8((OdUInt8) OdTrVisMetaRecType_Material);
    m_streamWriter.writeUInt64((OdUInt64) id);
    break;
  PK_CASE_EX(_MetafileStream_InitTexture_TextureID):
    ODA_ASSERT_ONCE(m_pWriter.get());
    m_streamWriter.writeUInt8((OdUInt8) OdTrVisMetaRecType_InitTexture);
    m_streamWriter.writeUInt64((OdUInt64) id);
    break;
  //PK_CASE_EX(_MetafileStream_TtfText_Text_Char_MetafileID):
  //  ODA_ASSERT_ONCE(m_pWriter.get());
  //  ...
  //  break;
  PK_CASE_EX(_MetafileStream_Metafile_MetafileID):
    ODA_ASSERT_ONCE(m_pWriter.get());
    m_streamWriter.writeUInt8((OdUInt8) OdTrVisMetaRecType_Metafile);
    m_streamWriter.writeUInt64((OdUInt64) id);
    break;
  PK_CASE_EX(_MetafileStream_MetafileMarker_Marker):
    ODA_ASSERT_ONCE(m_pWriter.get());
    m_streamWriter.writeUInt8((OdUInt8) OdTrVisMetaRecType_MetafileMarker),
    m_streamWriter.writeInt64((OdUInt64) id);
    break;
  PK_CASE(ViewportAdded_ViewportID): PK_CASE(ViewportModified_ViewportID):
    ODA_ASSERT_ONCE(m_pViewportDef.get());
    m_id = id;
    break;
  PK_CASE(ViewportDeleted_ViewportID):
  PK_CASE(ViewportInserted_ViewportID):
    m_id = id;
    break;
  PK_CASE(ViewportModified_OverallVpId):
    ODA_ASSERT_ONCE(m_pViewportDef.get());
    m_pViewportDef->m_overallViewportId = id;
    break;
  PK_CASE(ViewportModified_VisualStyleId):
    ODA_ASSERT_ONCE(m_pViewportDef.get());
    m_pViewportDef->m_visualStyle = id;
    break;
  PK_CASE(MetafileOrderChanged_ViewportID):
    ODA_ASSERT_ONCE(m_pMetafileOrderDef.get());
    m_pMetafileOrderDef->m_viewportId = id;
    break;
  PK_CASE(MetafileOrderInheritance_ViewportID1):
    ODA_ASSERT_ONCE(m_pMetafileOrderInheritance.get());
    m_pMetafileOrderInheritance->m_viewportId1 = id;
    break;
  PK_CASE(MetafileOrderInheritance_ViewportID2):
    ODA_ASSERT_ONCE(m_pMetafileOrderInheritance.get());
    m_pMetafileOrderInheritance->m_viewportId2 = id;
    break;
  PK_CASE(MetafileOrderInheritance_OverlayID):
    ODA_ASSERT_ONCE(m_pMetafileOrderInheritance.get());
    m_pMetafileOrderInheritance->m_overlayId = id;
    break;
  PK_CASE(MetafileOrderChanged_Ordering_MetafileID):
    ODA_ASSERT_ONCE(m_pMetafileOrderDef.get());
    m_pMetafileOrderDef->m_metafileOrder.push_back(id);
    break;
  PK_CASE(ExtentsChanged_ViewportID):
    ODA_ASSERT_ONCE(m_pExtentsDef.get());
    m_pExtentsDef->m_viewportId = id;
    break;
  PK_CASE(LightsListChanged_ViewportID):
    ODA_ASSERT_ONCE(m_pLightsListDef.get());
    m_pLightsListDef->m_viewportId = id;
    break;
  PK_CASE(BackgroundChanged_ViewportID):
    ODA_ASSERT_ONCE(m_pBackgroundDef.get());
    m_pBackgroundDef->m_viewportId = id;
    break;
  PK_CASE(OverlayViewParamsOverride_ViewportID):
    ODA_ASSERT_ONCE(m_pOverlayViewParamsOverrideDef.get());
    m_pOverlayViewParamsOverrideDef->m_viewportId = id;
    break;
  PK_CASE(OverlayAdded_OverlayID):
  PK_CASE(OverlayVisibilityChanged_OverlayID):
  PK_CASE(OverlayRenderOrderChanged_OverlayID):
    ODA_ASSERT_ONCE(m_pOverlayDef.get());
    m_id = id;
    break;
  PK_CASE(OverlayDeleted_OverlayID):
    ODA_ASSERT_ONCE(!m_pOverlayDef.get());
    m_id = id;
    break;
  PK_CASE(MetafileOrderChanged_OverlayID):
    ODA_ASSERT_ONCE(m_pMetafileOrderDef.get());
    m_pMetafileOrderDef->m_overlayId = id;
    break;
  PK_CASE(ExtentsChanged_OverlayID):
    ODA_ASSERT_ONCE(m_pExtentsDef.get());
    m_pExtentsDef->m_overlayId = id;
    break;
  PK_CASE(OverlayViewParamsOverride_OverlayID):
    ODA_ASSERT_ONCE(m_pOverlayViewParamsOverrideDef.get());
    m_pOverlayViewParamsOverrideDef->m_overlayId = id;
    break;
  PK_CASE(HlBranchAdded_HlBranchID):
  PK_CASE(HlBranchModified_HlBranchID):
    ODA_ASSERT_ONCE( m_pHlBranchDef.get() );
    m_id = id;
    break;
  PK_CASE(HlBranchAdded_Childs_HlBranchID):
  PK_CASE(HlBranchModified_Childs_HlBranchID):
    ODA_ASSERT_ONCE( m_pHlBranchDef.get() );
    m_pHlBranchDef->m_childs.push_back( id );
    break;
  PK_CASE(MetafileHlBranchAttached_MetafileID):
  PK_CASE(MetafileHlBranchDetached_MetafileID):
    m_id = id;
    break;
  PK_CASE(MetafileHlBranchAttached_HlBranchID):
    m_HlBranchId = id;
    break;
  PK_CASE(HlBranchDeleted_HlBranchID):
    m_id = id;
    break;
  PK_CASE_EX(_MetafileStream_Program_ProgramID):
    ODA_ASSERT_ONCE(m_pWriter.get());
    m_streamWriter.writeUInt8((OdUInt8) OdTrVisMetaRecType_Program),
    m_streamWriter.writeUInt64((OdUInt64) id);
    break;
  PK_CASE(ViewportModified_ShaderId):
    ODA_ASSERT_ONCE(m_pViewportDef.get());
    m_pViewportDef->m_rmShader = (OdTrVisProgramId) id,
    m_kindOfModViewportModified |= OdTrVisRendition::kViewportModShader;
    break;
  PK_CASE(BackgroundChanged_BaseProgramID):
    ODA_ASSERT_ONCE(m_pBackgroundDef.get());
    m_pBackgroundDef->m_baseProgramId = (OdTrVisProgramId) id;
    break;
  PK_CASE_EX(_MetafileStream_VisualStyle_VisualStyleID):
    ODA_ASSERT_ONCE( m_pWriter.get() );
    m_streamWriter.writeUInt8((OdUInt8) OdTrVisMetaRecType_VisualStyle);
    m_streamWriter.writeUInt64((OdUInt64) id);
    break;
  //default:
  //  ODA_FAIL_ONCE();
  //  return false;
  }
  ODA_ASSERT_ONCE(typeToRegId >= 0);
  if (typeToRegId < 0)
    return false;

  if (isNewId && (id == OdTrVisId(0))) // special
  {
    m_pIdMapper->setSpecialId( asValue, id, typeToRegId );
  }
  else if (isNewId)
  {

    OdTrVisRendition::IdRegistrator* pIdRegistrator = m_pRendition->idRegistrator();
    if (isNewId && pIdRegistrator)
    {
      OdTrVisUniqueID uId(asValue); 
      ODA_ASSERT_ONCE(typeToRegId >= 0 && asValue == uId.transform());

      OdAnsiString asDbStub = asValue;
      int pos = asDbStub.find('-');
      ODA_ASSERT_ONCE(pos);
      if (pos > 0)
        asDbStub = asDbStub.left(pos);

        // do not touch content - it is used in other places via (OdDbStub*)(*m_pMapFakeDbStub)[asDbStub].get()
      if (m_pMapFakeDbStub->find(asDbStub) == m_pMapFakeDbStub->end())
        (*m_pMapFakeDbStub)[asDbStub] = new OdFakeDbStub(odStrToUInt64(asDbStub)); // = new OdAnsiString(asDbStub);
      //OdDbStub* pFakeDbStub = (OdDbStub*) (*m_pMapFakeDbStub)[asDbStub]->c_str();
      //ODA_ASSERT_ONCE(asDbStub == (const char*) pFakeDbStub);
      ODA_ASSERT_ONCE(asDbStub == OdAnsiString().format(/*PRIX64*/PTRX64FORMAT, (*m_pMapFakeDbStub)[asDbStub]->handle));
      ////pIdRegistrator->reg(typeToRegId, id, pFakeDbStub, uId);

      pIdRegistrator->reg(typeToRegId, id, uId);
    }
  }

  return true;
}

bool OdXmlGlesLoader::DropDbStub(OdEnPathKey pk, const OdAnsiString& asValue)
{
  ODA_ASSERT_ONCE(!asValue.isEmpty() && asValue != "0");

  // do not touch content - it is used in other places via (OdDbStub*)(*m_pMapFakeDbStub)[asValue].get()
  if (m_pMapFakeDbStub->find(asValue) == m_pMapFakeDbStub->end())
    (*m_pMapFakeDbStub)[asValue] = new OdFakeDbStub(odStrToUInt64(asValue)); // = new OdAnsiString(asValue);
  OdFakeDbStub* pFakeDbStub = (*m_pMapFakeDbStub)[asValue].get();
  //ODA_ASSERT_ONCE(asValue == (const char*) pFakeDbStub);
  ODA_ASSERT_ONCE(asValue == OdAnsiString().format(/*PRIX64*/PTRX64FORMAT, pFakeDbStub->handle));

  switch (pk)
  {
  PK_CASE(MetafileAdded_OwnerID):
    ODA_ASSERT_ONCE(m_pMetafileDef.get());
    metafile()->m_pOwnerId = (OdDbStub*) pFakeDbStub;
    break;
  PK_CASE(LayerAdded_OwnerID):
    ODA_ASSERT_ONCE(m_pLayerDef.get());
    m_pLayerDef->m_pOwner = (OdDbStub*) pFakeDbStub;
    break;
  PK_CASE(HlBranchAdded_OwnerID):
    ODA_ASSERT_ONCE(m_pHlBranchDef.get());
    m_pHlBranchDef->m_pOwner = (OdDbStub*) pFakeDbStub;
    break;
  default:
    ODA_FAIL_ONCE(); // TODO
    return false;
  }
  return true;
}

bool OdXmlGlesLoader::DropChars(OdEnPathKey pk, const OdAnsiString& asValue)
{
  switch (pk)
  {
  PK_CASE(VisualStyleAdded_Prop_SVal): PK_CASE(VisualStyleModified_Prop_SVal):
    ODA_ASSERT_ONCE(   m_pVisualStyleDef.get() && m_pVisualStyleProperty 
                    && m_typeVisualStyle == OdGiVariant::kString);
    m_pVisualStyleProperty->set(OdString_toUnicode(asValue), m_pVisualStyleProperty->operation());
    ODA_ASSERT_ONCE(m_typeVisualStyle == m_pVisualStyleProperty->type());
    break;
  PK_CASE(VisualStyleAdded_Name): PK_CASE(VisualStyleModified_Name):
    ODA_ASSERT_ONCE(m_pVisualStyleDef.get());
    m_pVisualStyleDef->setName(OdString_toUnicode(asValue));
    ODA_ASSERT_ONCE(OdString_toUtf8(m_pVisualStyleDef->name()) == asValue);
    break;
  PK_CASE(MaterialAdded_MaterialName):
    ODA_ASSERT_ONCE(m_pMaterialDef.get());
    m_pMaterialDef->m_name = OdString_toUnicode(asValue);
    ODA_ASSERT_ONCE(OdString_toUtf8(m_pMaterialDef->m_name) == asValue);
    break;
  PK_CASE(LayerAdded_Name): PK_CASE(LayerModified_Name):
    ODA_ASSERT_ONCE(m_pLayerDef.get());
    m_pLayerDef->m_name = OdString_toUnicode(asValue);
    break;
  default:
    ODA_FAIL_ONCE(); // TODO
    return false;
  }
  return true;
}

bool OdXmlGlesLoader::DropBoolean(OdEnPathKey pk, bool data)
{
  switch (pk)
  {
  PK_CASE(TextureAdded_Smooth):
    ODA_ASSERT_ONCE(m_pTextureDef.get());
    m_pTextureDef->setSmoothFilter(data);
    break;
  PK_CASE(TextureAdded_Modulate):
    ODA_ASSERT_ONCE(m_pTextureDef.get());
    m_pTextureDef->setColorModulation(data);
    break;
  PK_CASE(MetafileAdded_Visibility):
    ODA_ASSERT_ONCE(m_pMetafileDef.get());
    metafile()->setMetafileVisible(data);
    break;
  PK_CASE(MetafileAdded_Highlighted):
    ODA_ASSERT_ONCE(m_pMetafileDef.get());
    metafile()->setMetafileHighlighted(data);
    break;
  PK_CASE(MetafileAdded_Temporary):
    ODA_ASSERT_ONCE(m_pMetafileDef.get());
    metafile()->setTemporaryMetafile(data);
    break;
  PK_CASE(MetafileAdded_Nested):
    ODA_ASSERT_ONCE(m_pMetafileDef.get());
    metafile()->setNestedMetafile(data);
    break;
  PK_CASE(MetafileAdded_BlockRef):
    ODA_ASSERT_ONCE(m_pMetafileDef.get());
    metafile()->setBlockRefMetafile(data);
    break;
  PK_CASE(MetafileAdded_Utilitary):
    ODA_ASSERT_ONCE(m_pMetafileDef.get());
    metafile()->setUtilitaryMetafile(data);
    break;
  PK_CASE(MetafileAdded_Sectionable):
    ODA_ASSERT_ONCE(m_pMetafileDef.get());
    metafile()->setSectionableMetafile(data);
    break;
  PK_CASE(MetafileAdded_Composite):
    ODA_ASSERT_ONCE(m_pMetafileDef.get());
    metafile()->setCompositeMetafile(data);
    break;
  PK_CASE(MetafileAdded_XRef):
    ODA_ASSERT_ONCE(m_pMetafileDef.get());
    metafile()->setXRefMetafile(data);
    break;
  PK_CASE(MetafileAdded_Faded):
    ODA_ASSERT_ONCE(m_pMetafileDef.get());
    metafile()->setMetafileFaded(data);
    break;
  PK_CASE(MetafileHighlightingChanged_Highlight):
  PK_CASE(MetafileVisibilityChanged_Visibility):
  PK_CASE(MetafileFadingChanged_Fade):
    ODA_ASSERT_ONCE(m_metafileChangedFlag == 0xFF);
    m_metafileChangedFlag = (data) ? 1 : 0;
    break;
  PK_CASE(SurfaceChanged_TransparentBackground):
    ODA_ASSERT_ONCE(m_pSurfaceDef.get());
    m_pSurfaceDef->setTransparentBackground(data);
    break;
  PK_CASE(ViewportModified_Visibility):
    ODA_ASSERT_ONCE(m_pViewportDef.get());
    m_pViewportDef->m_bVisible = data,
    m_kindOfModViewportModified |= OdTrVisRendition::kViewportModVisibility;
    break;
  PK_CASE(ViewportModified_ViewParams_PerspectiveProjection):
  PK_CASE(OverlayViewParamsOverride_ViewParams_PerspectiveProjection):
    ODA_ASSERT_ONCE(m_pViewParamsDef.get());
    m_pViewParamsDef->m_bPerspectiveProjection = data;
    break;
  PK_CASE(ViewportModified_ViewParams_FrontClip):
  PK_CASE(OverlayViewParamsOverride_ViewParams_FrontClip):
    ODA_ASSERT_ONCE(m_pViewParamsDef.get());
    m_pViewParamsDef->m_frontClip.m_bClipped = data;
    break;
  PK_CASE(ViewportModified_ViewParams_BackClip):
  PK_CASE(OverlayViewParamsOverride_ViewParams_BackClip):
    ODA_ASSERT_ONCE(m_pViewParamsDef.get());
    m_pViewParamsDef->m_backClip.m_bClipped = data;
    break;
  PK_CASE(ViewportModified_PSOverallViewport):
    ODA_ASSERT_ONCE(m_pViewportDef.get());
    m_pViewportDef->setPSOverallViewport(data);
    m_kindOfModViewportModified |= OdTrVisRendition::kViewportModPersistent;
    break;
  PK_CASE(ViewportModified_PSModelViewport):
    ODA_ASSERT_ONCE(m_pViewportDef.get());
    m_pViewportDef->setPSModelViewport(data);
    m_kindOfModViewportModified |= OdTrVisRendition::kViewportModPersistent;
    break;
  PK_CASE(ViewportModified_PSHelperViewport):
    ODA_ASSERT_ONCE(m_pViewportDef.get());
    m_pViewportDef->setPSHelperViewport(data);
    m_kindOfModViewportModified |= OdTrVisRendition::kViewportModPersistent;
    break;
  PK_CASE(ViewportModified_DepthBuffer):
    ODA_ASSERT_ONCE(m_pViewportDef.get());
    ODA_ASSERT_ONCE(m_pViewportDef->m_renderMode.controlVersion() == 5);
    m_pViewportDef->m_renderMode.setDepthBufferEnabled(data);
    break;
  PK_CASE(ViewportModified_StencilBuffer):
    ODA_ASSERT_ONCE(m_pViewportDef.get());
    ODA_ASSERT_ONCE(m_pViewportDef->m_renderMode.controlVersion() == 5);
    m_pViewportDef->m_renderMode.setStencilBufferEnabled(data);
    break;
  PK_CASE(ViewportModified_PolygonOffset):
    ODA_ASSERT_ONCE(m_pViewportDef.get());
    ODA_ASSERT_ONCE(m_pViewportDef->m_renderMode.controlVersion() == 5);
    m_pViewportDef->m_renderMode.setPolygonOffsetEnabled(data);
    break;
  PK_CASE(ViewportModified_Lighting):
    ODA_ASSERT_ONCE(m_pViewportDef.get());
    ODA_ASSERT_ONCE(m_pViewportDef->m_renderMode.controlVersion() == 5);
    m_pViewportDef->m_renderMode.setLightingEnabled(data);
    break;
  PK_CASE(ViewportModified_FaceNormals):
    ODA_ASSERT_ONCE(m_pViewportDef.get());
    ODA_ASSERT_ONCE(m_pViewportDef->m_renderMode.controlVersion() == 5);
    m_pViewportDef->m_renderMode.setFaceNormalsEnabled(data);
    break;
  PK_CASE(ViewportModified_Border):
    ODA_ASSERT_ONCE(m_pViewportDef.get());
    m_pViewportDef->m_bBorder = data;
    m_kindOfModViewportModified |= OdTrVisRendition::kViewportModBorder;
    break;
  PK_CASE(LightsListChanged_DefaultLightsOn):
    ODA_ASSERT_ONCE(m_pLightsListDef.get());
    m_pLightsListDef->m_bDefaultLightsOn = data;
    break;
  PK_CASE(LightsListChanged_LightsList_Light_DefLight):
    ODA_ASSERT_ONCE(m_pLightsListDef.get());
    m_pLightsListDef->m_Defs[m_pLightsListDef->m_Defs.size() - 1].m_bDefLight = data;
    break;
  PK_CASE(BackgroundChanged_Visible):
    ODA_ASSERT_ONCE(m_pBackgroundDef.get() && m_pBackgroundDef->m_pStream.isNull() && data);
    metafile(); // create m_pBackgroundDef->m_pStream
    break;
  PK_CASE(VisualStyleAdded_Prop_Set): PK_CASE(VisualStyleModified_Prop_Set):
    ODA_ASSERT_ONCE(m_pVisualStyleDef.get() && m_pVisualStyleProperty);
    m_pVisualStyleProperty->setOperation(data);
    ODA_ASSERT_ONCE(m_pVisualStyleProperty->operation() == data);
    break;
  PK_CASE(VisualStyleAdded_Prop_BVal): PK_CASE(VisualStyleModified_Prop_BVal):
    ODA_ASSERT_ONCE(   m_pVisualStyleDef.get() && m_pVisualStyleProperty 
                    && m_typeVisualStyle == OdGiVariant::kBoolean);
    m_pVisualStyleProperty->set(data, m_pVisualStyleProperty->operation());
    ODA_ASSERT_ONCE(m_typeVisualStyle == m_pVisualStyleProperty->type());
    break;
  PK_CASE(VisualStyleAdded_Internal): PK_CASE(VisualStyleModified_Internal):
    ODA_ASSERT_ONCE(m_pVisualStyleDef.get());
    m_pVisualStyleDef->setInternal(data);
    break;
  PK_CASE(VisualStyleAdded_Temporary): PK_CASE(VisualStyleModified_Temporary):
    ODA_ASSERT_ONCE(m_pVisualStyleDef.get());
    m_pVisualStyleDef->setTemporary(data);
    break;
  PK_CASE(OverlayVisibilityChanged_Visibility):
    ODA_ASSERT_ONCE(m_pOverlayDef.get());
    m_pOverlayDef->setOverlayEnabled(data);
    break;
  PK_CASE(OverlayViewParamsOverride_Override):
    ODA_ASSERT_ONCE(m_pOverlayViewParamsOverrideDef.get());
    m_pOverlayViewParamsOverrideDef->m_bOverride = data;
    break;
  PK_CASE( LayerViewportPropsOverride_Override ):
    m_bOverrideLayer = data;
    break;
  PK_CASE(MaterialAdded_IsDefault):
    ODA_ASSERT_ONCE(m_pMaterialDef.get());
    m_pMaterialDef->m_bDefault = data;
    break;
  PK_CASE_EX( _MetafileStream_Linestyle_Default ):
    ODA_ASSERT_ONCE( m_pWriter.get() );
    m_streamWriter.writeUInt8( (OdUInt8) OdTrVisMetaRecType_Linestyle );
    m_streamWriter.writeBool( data );
    break;
  default:
    ODA_FAIL_ONCE(); // TODO
    return false;
  }
  return true;
}

bool OdXmlGlesLoader::DropUInt8RGB(OdEnPathKey pk, const OdBinaryData& data)
{
  ODA_ASSERT_ONCE(data.size() == 4);
  if (data.size() != 4)
    return false;
  ODCOLORREF color = ODRGBA(data[0], data[1], data[2], data[3]);

  switch (pk)
  {
  PK_CASE_EX(_MetafileStream_Color_Color):
    ODA_ASSERT_ONCE(m_pWriter.get());
    m_streamWriter.writeUInt8((OdUInt8) OdTrVisMetaRecType_Color),
    m_streamWriter.writeUInt8(data[0]), m_streamWriter.writeUInt8(data[1]),
    m_streamWriter.writeUInt8(data[2]), m_streamWriter.writeUInt8(data[3]);
    break;
  PK_CASE(SurfaceChanged_BackgroundColor):
    ODA_ASSERT_ONCE(m_pSurfaceDef.get());
    m_pSurfaceDef->m_backgroundColor = color;
    break;
  PK_CASE(ViewportModified_LineStyleConfig):
    ODA_ASSERT_ONCE(m_pViewportDef.get());
    m_pViewportDef->m_lineStyleConfig = color;
    m_kindOfModViewportModified |= OdTrVisRendition::kViewportModContextual;
    break;
  PK_CASE(ViewportModified_FadingConfig):
    ODA_ASSERT_ONCE(m_pViewportDef.get());
    m_pViewportDef->m_fadingConfig = color;
    break;
  PK_CASE(ViewportModified_FaceFillColor):
    ODA_ASSERT_ONCE(m_pViewportDef.get());
    m_pViewportDef->m_faceFillColor = color;
    break;
  PK_CASE(ViewportModified_BorderColor):
    ODA_ASSERT_ONCE(m_pViewportDef.get());
    m_pViewportDef->m_borderColor = color;
    break;
  default:
    ODA_FAIL_ONCE(); // TODO
    return false;
  }
  return true;
}

bool OdXmlGlesLoader::DropUInt8(OdEnPathKey pk, OdUInt8 data)
{
  switch (pk)
  {
  PK_CASE_EX(_MetafileStream_EnableOpt_Mode):
    ODA_ASSERT_ONCE(m_pWriter.get());
    m_streamWriter.writeUInt8((OdUInt8) OdTrVisMetaRecType_EnableOpt);
    m_streamWriter.writeUInt8(data);
    break;
  PK_CASE_EX(_MetafileStream_DisableOpt_Mode):
    ODA_ASSERT_ONCE(m_pWriter.get());
    m_streamWriter.writeUInt8((OdUInt8) OdTrVisMetaRecType_DisableOpt);
    m_streamWriter.writeUInt8(data);
    break;
  PK_CASE_EX(_MetafileStream_EnableArray_Type):
    ODA_ASSERT_ONCE(m_pWriter.get());
    m_streamWriter.writeUInt8((OdUInt8) OdTrVisMetaRecType_EnableArray);
    m_streamWriter.writeUInt8(data);
    break;
  PK_CASE_EX(_MetafileStream_DisableArray_Type):
    ODA_ASSERT_ONCE(m_pWriter.get());
    m_streamWriter.writeUInt8((OdUInt8) OdTrVisMetaRecType_DisableArray);
    m_streamWriter.writeUInt8(data);
    break;
  PK_CASE_EX(_MetafileStream_DrawArrays_Mode):
    ODA_ASSERT_ONCE(m_pWriter.get());
    m_streamWriter.writeUInt8((OdUInt8) OdTrVisMetaRecType_DrawArrays);
    m_streamWriter.writeUInt8(data);
    break;
  PK_CASE_EX(_MetafileStream_DrawElements_Mode):
    ODA_ASSERT_ONCE(m_pWriter.get());
    m_streamWriter.writeUInt8((OdUInt8) OdTrVisMetaRecType_DrawElements);
    m_streamWriter.writeUInt8(data);
    break;
  PK_CASE_EX(_MetafileStream_CullFace_Mode):
    ODA_ASSERT_ONCE(m_pWriter.get());
    m_streamWriter.writeUInt8((OdUInt8)OdTrVisMetaRecType_CullFace),
    m_streamWriter.writeUInt8(data);
    break;
  PK_CASE_EX(_MetafileStream_LStipple_Pattern):
    ODA_ASSERT_ONCE(m_pWriter.get());
    m_streamWriter.writeUInt8((OdUInt8) OdTrVisMetaRecType_LStipple);
    m_streamWriter.writeUInt8(data);
    break;
  PK_CASE_EX(_MetafileStream_PStipple_Pattern):
    ODA_ASSERT_ONCE(m_pWriter.get());
    m_streamWriter.writeUInt8((OdUInt8) OdTrVisMetaRecType_PStipple);
    m_streamWriter.writeUInt8(data);
    break;
  PK_CASE_EX(_MetafileStream_HLRStencil_State):
    ODA_ASSERT_ONCE(m_pWriter.get());
    m_streamWriter.writeUInt8((OdUInt8) OdTrVisMetaRecType_HLRStencil);
    m_streamWriter.writeUInt8(data);
    break;
  PK_CASE_EX(_MetafileStream_EnableShading_Mode):
    ODA_ASSERT_ONCE(m_pWriter.get());
    m_streamWriter.writeUInt8((OdUInt8) OdTrVisMetaRecType_EnableShading);
    m_streamWriter.writeUInt8(data);
    break;
  PK_CASE_EX(_MetafileStream_DisableShading_Mode):
    ODA_ASSERT_ONCE(m_pWriter.get());
    m_streamWriter.writeUInt8((OdUInt8) OdTrVisMetaRecType_DisableShading);
    m_streamWriter.writeUInt8(data);
    break;
  PK_CASE_EX(_MetafileStream_InitTexture_Repeat):
    ODA_ASSERT_ONCE(m_pWriter.get());
    //m_streamWriter.writeUInt8((OdUInt8) OdTrVisMetaRecType_InitTexture); // via "MetafileAdded/MetafileData/MetafileStream/InitTexture/TextureID"
    m_streamWriter.writeUInt8(data);
    break;
  PK_CASE_EX(_MetafileStream_EnableMarkerArray_Type):
    ODA_ASSERT_ONCE(m_pWriter.get());
    m_streamWriter.writeUInt8((OdUInt8) OdTrVisMetaRecType_EnableMarkerArray);
    m_streamWriter.writeUInt8(data);
    break;
  PK_CASE_EX(_MetafileStream_DisableMarkerArray_Type):
    ODA_ASSERT_ONCE(m_pWriter.get());
    m_streamWriter.writeUInt8((OdUInt8) OdTrVisMetaRecType_DisableMarkerArray);
    m_streamWriter.writeUInt8(data);
    break;
  PK_CASE_EX(_MetafileStream_SelectionFlags_Flags):
    ODA_ASSERT_ONCE(m_pWriter.get());
    m_streamWriter.writeUInt8((OdUInt8) OdTrVisMetaRecType_SelectionFlags);
    m_streamWriter.writeUInt8(data);
    break;
  PK_CASE_EX(_MetafileStream_Lineweight_Type):
    ODA_ASSERT_ONCE(m_pWriter.get());
    m_streamWriter.writeUInt8((OdUInt8) OdTrVisMetaRecType_Lineweight);
    m_streamWriter.writeUInt8(data);
    break;
  PK_CASE_EX(_MetafileStream_GeomMarker_Type):
    ODA_ASSERT_ONCE(m_pWriter.get());
    m_streamWriter.writeUInt8((OdUInt8) OdTrVisMetaRecType_GeomMarker);
    m_streamWriter.writeUInt8(data);
    break;
  PK_CASE(ViewportModified_LineweightEnum_NPixels):
    ODA_ASSERT_ONCE(m_pViewportDef.get());
    m_pViewportDef->m_lineweightsEnum.push_back(data);
    break;
  PK_CASE_EX(_MetafileStream_Linestyle_CapsType):
    ODA_ASSERT_ONCE(m_pWriter.get());
    m_streamWriter.writeUInt8(data);
    break;
  PK_CASE_EX(_MetafileStream_Linestyle_JoinType):
    ODA_ASSERT_ONCE(m_pWriter.get());
    m_streamWriter.writeUInt8(data);
    break;
  default:
    ODA_FAIL_ONCE(); // TODO
    return false;
  }
  return true;
}

bool OdXmlGlesLoader::DropUInt16(OdEnPathKey pk, OdUInt16 data)
{
  switch (pk)
  {
  PK_CASE(LayerAdded_Props): PK_CASE(LayerModified_Props):
    ODA_ASSERT_ONCE(m_pLayerDef.get());
    m_pLayerDef->m_props.m_flags = data; // DropLayerProps 
    break;
  PK_CASE( LayerViewportPropsOverride_VpDepSpec_Props ):
    m_overrideLayerProps.m_flags = data;
    break;
  PK_CASE(OverlayAdded_Flags):
    ODA_ASSERT_ONCE(m_pOverlayDef.get());
    m_pOverlayDef->resetOverlayFlags(data);
    break;
  default:
    ODA_FAIL_ONCE(); // TODO
    return false;
  }
  return true;
}

bool OdXmlGlesLoader::DropUInt32(OdEnPathKey pk, OdUInt32 data)
{
  switch (pk)
  {
  PK_CASE(TextureAdded_Type):
    ODA_ASSERT_ONCE(m_pTextureDef.get() && m_curTexture.isValid);
    m_curTexture.fmt = (enum OdTrVisTexture::Format) data;
    break;
  PK_CASE(TextureAdded_PaletteType):
    ODA_ASSERT_ONCE(m_pTextureDef.get());
    m_curTexture.pal_fmt = (enum OdTrVisTexture::Format) data;
    break;
  PK_CASE(TextureAdded_Width):
    ODA_ASSERT_ONCE(m_pTextureDef.get());
    m_curTexture.nWidth = data;
    break;
  PK_CASE(TextureAdded_Height):
    ODA_ASSERT_ONCE(m_pTextureDef.get());
    m_curTexture.nHeight = data;
    break;
  PK_CASE(TextureAdded_ScanlineLength):
    ODA_ASSERT_ONCE(m_pTextureDef.get());
    m_curTexture.nScanLineLength = data;
    break;
  PK_CASE(TextureAdded_PaletteWidth):
    ODA_ASSERT_ONCE(m_pTextureDef.get());
    m_curTexture.pal_nWidth = data;
    break;
  PK_CASE(TextureAdded_PaletteScanlineLength):
    ODA_ASSERT_ONCE(m_pTextureDef.get());
    m_curTexture.pal_nScanLineLength = data;
    break;
  PK_CASE(TextureAdded_DataAlignment):
    ODA_ASSERT_ONCE(m_pTextureDef.get());
    m_curTexture.nAlign = data;
    break;
  PK_CASE(TextureAdded_PaletteDataAlignment):
    ODA_ASSERT_ONCE(m_pTextureDef.get());
    m_curTexture.pal_nAlign = data;
    break;
  PK_CASE(MaterialAdded_UWrapType):
    ODA_ASSERT_ONCE(m_pMaterialDef.get());
    m_pMaterialDef->m_uWrap = (enum OdTrVisMaterialDef::WrapType) data;
    break;
  PK_CASE(MaterialAdded_VWrapType):
    ODA_ASSERT_ONCE(m_pMaterialDef.get());
    m_pMaterialDef->m_vWrap = (enum OdTrVisMaterialDef::WrapType) data;
    break;
  PK_CASE(VisualStyleAdded_Type): PK_CASE(VisualStyleModified_Type):
    ODA_ASSERT_ONCE(m_pVisualStyleDef.get());
    m_pVisualStyleDef->setType((OdGiVisualStyle::Type) data);
    break;
  PK_CASE(VisualStyleAdded_NProps): PK_CASE(VisualStyleModified_NProps):
    ODA_ASSERT_ONCE(m_pVisualStyleDef.get() && data > 0);
    m_pVisualStyleDef->setPropertiesNumber(data);
    break;
  PK_CASE(VisualStyleAdded_NPropsModified): PK_CASE(VisualStyleModified_NPropsModified): // usele for xml
    ODA_ASSERT_ONCE(m_pVisualStyleDef.get());
    break;
  PK_CASE(VisualStyleAdded_Prop_NProp): PK_CASE(VisualStyleModified_Prop_NProp):
    ODA_ASSERT_ONCE(   m_pVisualStyleDef.get() && data < m_pVisualStyleDef->numProperties() 
                    && !m_pVisualStyleProperty && m_typeVisualStyle == OdGiVariant::kUndefined);
    m_pVisualStyleProperty = &m_pVisualStyleDef->property((OdGiVisualStyleProperties::Property) data);
    m_pVisualStyleProperty->setPropertyNumber(data);
    //m_pVisualStyleProperty->setModified(true);
    break;
  PK_CASE(VisualStyleAdded_Prop_PropType): PK_CASE(VisualStyleModified_Prop_PropType):
    ODA_ASSERT_ONCE(   m_pVisualStyleDef.get() && m_pVisualStyleProperty 
                    && data >= OdGiVariant::kBoolean && data <= OdGiVariant::kString);
    m_typeVisualStyle = (OdGiVariant::VariantType) data;
    break;
  PK_CASE_EX(_NArrays):
    if (metafile())
    {
      m_typeElement = OdTrVisArrayWrapper::Type_Invalid,
      m_indexElement = 0,
      m_numDataElement = 0,
      metafile()->m_ArrayElements.resize(data);
    }
    break;
  PK_CASE_EX(_Array_Type):
    if (metafile())
      m_typeElement = (enum OdTrVisArrayWrapper::Type) data;
    break;
  PK_CASE_EX(_Array_NData):
    if (metafile())
      m_numDataElement = data;
    break;
  PK_CASE_EX(_Array_Data_Entry_From):
    if (metafile())
    {
      ODA_ASSERT_ONCE(   m_typeElement == OdTrVisArrayWrapper::Type_Marker
                      && m_indexElement < metafile()->m_ArrayElements.size()
                      && m_uaMarkerIndex < m_aMarkers.size());
      OdTrVisDefProcMark& nMark = m_aMarkers[m_uaMarkerIndex++];
      nMark.m_uFrom = data;
    }
    break;
  PK_CASE_EX(_MetafileStream_EnableArray_NArray): PK_CASE_EX(_MetafileStream_DrawElements_NArray):
  PK_CASE_EX(_MetafileStream_EnableMarkerArray_NArray):
    ODA_ASSERT_ONCE(m_pWriter.get());
    m_streamWriter.writeUInt32(data);
    break;
  PK_CASE_EX( _MetafileStream_UserEntry_NumData ):
    ODA_ASSERT_ONCE(m_pWriter.get());
    m_mfUserEntryNumData = data;
    break;
  //PK_CASE_EX(_MetafileStream_TtfText_NText):
  //  ODA_ASSERT_ONCE(m_pWriter.get());
  //  break;
  PK_CASE(MetafileOrderChanged_InvalidFrom):
    ODA_ASSERT_ONCE(m_pMetafileOrderDef.get() && m_pMetafileOrderDef->m_nInvalidFrom == data && !data); // test
    m_pMetafileOrderDef->m_nInvalidFrom = data;
    break;
  PK_CASE(MetafileOrderChanged_InvalidLen):
    ODA_ASSERT_ONCE(m_pMetafileOrderDef.get() && m_pMetafileOrderDef->m_nInvalidLen == data && !data); // test
    m_pMetafileOrderDef->m_nInvalidLen = data;
    break;
  PK_CASE(MetafileOrderChanged_ValidLen):
    ODA_ASSERT_ONCE(m_pMetafileOrderDef.get());
    break;
  PK_CASE(ViewportModified_KindOfMod):
    ODA_ASSERT_ONCE(m_pViewportDef.get());
    m_kindOfModViewportModified = data; // New in 4.1.0 release
    break;
  PK_CASE(ViewportModified_NNrcCounts):
    ODA_ASSERT_ONCE(m_pViewportDef.get());
    m_pViewportDef->m_nrcCounts.resize(0),// (data), idx = 0;
    m_kindOfModViewportModified |= OdTrVisRendition::kViewportModNrcClip;
    break;
  PK_CASE(ViewportModified_NNrcPoints):
    ODA_ASSERT_ONCE(m_pViewportDef.get());
    m_pViewportDef->m_nrcPoints.resize(0); // (data * 2), idx = 0;
    break;
  PK_CASE(ViewportModified_NBorderCounts):
    ODA_ASSERT_ONCE(m_pViewportDef.get());
    m_pViewportDef->m_borderCounts.resize(0); // (data), idx = 0;
    break;
  PK_CASE(ViewportModified_NBorderPoints):
    ODA_ASSERT_ONCE(m_pViewportDef.get());
    m_pViewportDef->m_borderPoints.resize(0); // (data * 2), idx = 0;
    break;
  PK_CASE(ViewportModified_NLineweightEnum):
    ODA_ASSERT_ONCE(m_pViewportDef.get());
    m_pViewportDef->m_lineweightsEnum.resize(0), // (data), idx = 0;
    m_kindOfModViewportModified |= OdTrVisRendition::kViewportModLineweights;
    break;
  PK_CASE(LightsListChanged_NLights):
    ODA_ASSERT_ONCE(m_pLightsListDef.get());
    break;
  PK_CASE(VisualStyleAdded_Prop_CVal): PK_CASE(VisualStyleModified_Prop_IVal):
    ODA_ASSERT_ONCE(   m_pVisualStyleDef.get() && m_pVisualStyleProperty 
                    && m_typeVisualStyle == OdGiVariant::kColor);
    m_pVisualStyleProperty->set(data, m_pVisualStyleProperty->operation(), true);
    ODA_ASSERT_ONCE(m_typeVisualStyle == m_pVisualStyleProperty->type());
    break;
  default:
    ODA_FAIL_ONCE(); // TODO
    return false;
  }
  return true;
}

bool OdXmlGlesLoader::DropUInt64(OdEnPathKey pk, OdUInt64 data)
{
  switch (pk)
  {
  PK_CASE_EX(_MetafileStream_SelectionMarker_Marker):
    ODA_ASSERT_ONCE(m_pWriter.get());
    m_streamWriter.writeUInt8((OdUInt8) OdTrVisMetaRecType_SelectionMarker),
    m_streamWriter.writeInt64(data);
    break;
  PK_CASE_EX(_Array_Data_Entry_Marker):
    if (metafile())
    {
      ODA_ASSERT_ONCE(   m_typeElement == OdTrVisArrayWrapper::Type_Marker
                      && m_indexElement < metafile()->m_ArrayElements.size()
                      && m_uaMarkerIndex < m_aMarkers.size());
      OdTrVisDefProcMark& nMark = m_aMarkers[m_uaMarkerIndex];
      nMark.m_nMark = data;
    }
    break;
  PK_CASE(HlBranchAdded_NChilds):
  PK_CASE(HlBranchModified_NChilds):
    ODA_ASSERT_ONCE( m_pHlBranchDef.get() );
    m_pHlBranchDef->m_childs.reserve( data );
    break;
  PK_CASE(HlBranchAdded_NMarkers):
  PK_CASE(HlBranchModified_NMarkers):
    ODA_ASSERT_ONCE( m_pHlBranchDef.get() );
    m_pHlBranchDef->m_markers.reserve( data );
    break;
  PK_CASE( HlBranchAdded_Markers_Marker ):
  PK_CASE(HlBranchModified_Markers_Marker):
    ODA_ASSERT_ONCE( m_pHlBranchDef.get() );
    m_pHlBranchDef->m_markers.push_back( data );
  default:
    ODA_FAIL_ONCE(); // TODO
    return false;
  }
  return true;
}

bool OdXmlGlesLoader::DropInt16(OdEnPathKey pk, OdInt16 data)
{
  switch (pk)
  {
  PK_CASE_EX(_MetafileStream_Lineweight_Lw):
    ODA_ASSERT_ONCE(m_pWriter.get());
    m_streamWriter.writeInt16(data);
    break;
  PK_CASE(OverlayAdded_RenderOrder):
  PK_CASE(OverlayRenderOrderChanged_RenderOrder):
    ODA_ASSERT_ONCE(m_pOverlayDef.get());
    m_pOverlayDef->m_renderOrder = data;
    break;
  default:
    ODA_FAIL_ONCE(); // TODO
    return false;
  }
  return true;
}

bool OdXmlGlesLoader::DropInt32(OdEnPathKey pk, OdInt32 data)
{
  switch (pk)
  {
  PK_CASE_EX(_MetafileStream_DrawArrays_First):
    ODA_ASSERT_ONCE(m_pWriter.get());
    m_streamWriter.writeInt32(data);
    break;
  PK_CASE_EX(_MetafileStream_DrawArrays_Count): PK_CASE_EX(_MetafileStream_DrawElements_Count):
    ODA_ASSERT_ONCE(m_pWriter.get());
    m_streamWriter.writeInt32(data);
    break;
  PK_CASE_EX(_MetafileStream_IPoint_Index):
    ODA_ASSERT_ONCE(m_pWriter.get());
    m_streamWriter.writeUInt8((OdUInt8) OdTrVisMetaRecType_IPoint),
    m_streamWriter.writeInt32(data);
    break;
  PK_CASE_EX(_MetafileStream_ILine_Index1):
    ODA_ASSERT_ONCE(m_pWriter.get());
    m_streamWriter.writeUInt8((OdUInt8) OdTrVisMetaRecType_ILine),
    m_streamWriter.writeInt32(data);
    break;
  PK_CASE_EX(_MetafileStream_ILine_Index2):
    ODA_ASSERT_ONCE(m_pWriter.get());
    m_streamWriter.writeInt32(data);
    break;
  PK_CASE(SurfaceChanged_SurfaceSizeXMin):
    ODA_ASSERT_ONCE(m_pSurfaceDef.get());
    m_pSurfaceDef->m_surfaceSize.m_min.x = data;
    break;
  PK_CASE(SurfaceChanged_SurfaceSizeYMin):
    ODA_ASSERT_ONCE(m_pSurfaceDef.get());
    m_pSurfaceDef->m_surfaceSize.m_min.y = data;
    break;
  PK_CASE(SurfaceChanged_SurfaceSizeXMax):
    ODA_ASSERT_ONCE(m_pSurfaceDef.get());
    m_pSurfaceDef->m_surfaceSize.m_max.x = data;
    break;
  PK_CASE(SurfaceChanged_SurfaceSizeYMax):
    ODA_ASSERT_ONCE(m_pSurfaceDef.get());
    m_pSurfaceDef->m_surfaceSize.m_max.y = data;
    break;
  PK_CASE(ViewportModified_NrcCounts_Count):
    ODA_ASSERT_ONCE(m_pViewportDef.get());
    m_pViewportDef->m_nrcCounts.push_back(data);
    break;
  PK_CASE(ViewportModified_BorderWidth):
    ODA_ASSERT_ONCE(m_pViewportDef.get());
    m_pViewportDef->m_borderWeight = data;
    break;
  PK_CASE(ViewportModified_BorderCounts_Count):
    ODA_ASSERT_ONCE(m_pViewportDef.get());
    m_pViewportDef->m_borderCounts.push_back(data);
    break;
  PK_CASE(LightsListChanged_LightsList_Light_Type):
    ODA_ASSERT_ONCE(m_pLightsListDef.get());
    m_pLightsListDef->m_Defs.push_back(OdTrVisLightDef()),
    m_pLightsListDef->m_Defs[m_pLightsListDef->m_Defs.size() - 1].m_type = (enum OdTrVisLightDef::LightType) data;
    break;
  PK_CASE(VisualStyleAdded_Prop_IVal): PK_CASE(VisualStyleModified_Prop_IVal):
    ODA_ASSERT_ONCE(   m_pVisualStyleDef.get() && m_pVisualStyleProperty 
                    && m_typeVisualStyle == OdGiVariant::kInt);
    m_pVisualStyleProperty->set(data, m_pVisualStyleProperty->operation());
    ODA_ASSERT_ONCE(m_typeVisualStyle == m_pVisualStyleProperty->type());
    break;
  PK_CASE( ViewportInserted_Index ):
    m_nViewportInsertedIndex = data;
    break;
  default:
    ODA_FAIL_ONCE(); // TODO
    return false;
  }
  return true;
}

bool OdXmlGlesLoader::DropBinaryStream(OdEnPathKey pk, const OdBinaryData& data)
{
  switch (pk)
  {
  PK_CASE(TextureAdded_TextureData):
    {
      ODA_ASSERT_ONCE(m_pTextureDef.get());
      OdTrVisTexturePtr tex = OdTrVisTextureManager::createTexture( m_curTexture.fmt, m_curTexture.nWidth, m_curTexture.nHeight, m_curTexture.nAlign );
      memcpy( tex->textureData(), data.asArrayPtr(), m_curTexture.nScanLineLength * m_curTexture.nHeight );
      m_pTextureDef->m_pTexture = tex;
    }
    break;
  PK_CASE(TextureAdded_PaletteData):
    {
      ODA_ASSERT_ONCE(m_pTextureDef.get());
      OdTrVisTexturePtr tex = OdTrVisTextureManager::createTexture( m_curTexture.pal_fmt, m_curTexture.pal_nWidth, 1, m_curTexture.pal_nAlign );
      memcpy( tex->textureData(), data.asArrayPtr(), m_curTexture.pal_nScanLineLength );
      m_pTextureDef->m_pTexture->setPalette( tex );
    }
    break;
  PK_CASE_EX( _MetafileStream_UserEntry_Data ):
    {
      ODA_ASSERT_ONCE(m_pWriter.get());
      m_streamWriter.writeUInt8((OdUInt8) OdTrVisMetaRecType_UserEntry);
      m_streamWriter.writeUInt32( m_mfUserEntryNumData );
      m_streamWriter.write( data.asArrayPtr(), m_mfUserEntryNumData );
    }
    break;
  default:
    ODA_FAIL_ONCE(); // TODO
    return false;
  }
  return true;
}

bool OdXmlGlesLoader::DropInts(OdEnPathKey pk, const OdIntArray& data)
{
  ODA_FAIL_ONCE(); // TODO
  return false;
}

bool OdXmlGlesLoader::DropFloat(OdEnPathKey pk, float data)
{
  switch (pk)
  {
  PK_CASE(MaterialAdded_BlendFactor):
    ODA_ASSERT_ONCE(m_pMaterialDef.get());
    m_pMaterialDef->m_blendFactor = data;
    break;
  PK_CASE(MaterialAdded_ShadingBlendFactor):
    ODA_ASSERT_ONCE(m_pMaterialDef.get());
    m_pMaterialDef->m_shadingBlendFactor = data;
    break;
  PK_CASE(MaterialAdded_OpacityLevel):
    ODA_ASSERT_ONCE(m_pMaterialDef.get());
    m_pMaterialDef->m_opacityLevel = data;
    break;
  PK_CASE(MaterialAdded_ShadingOpacityLevel):
    ODA_ASSERT_ONCE(m_pMaterialDef.get());
    m_pMaterialDef->m_shadingOpacityLevel = data;
    break;
  PK_CASE(MaterialAdded_SpecularPower):
    ODA_ASSERT_ONCE(m_pMaterialDef.get());
    m_pMaterialDef->m_specularPower = data;
    break;
  PK_CASE(MaterialAdded_TextureBlend):
    ODA_ASSERT_ONCE(m_pMaterialDef.get());
    m_pMaterialDef->m_textureBlend = data;
    break;
  PK_CASE(LightsListChanged_LightsList_Light_ConstantAttenuation):
    ODA_ASSERT_ONCE(m_pLightsListDef.get());
    m_pLightsListDef->m_Defs[m_pLightsListDef->m_Defs.size() - 1].m_constantAttenuation = data;
    break;
  PK_CASE(LightsListChanged_LightsList_Light_LinearAttenuation):
    ODA_ASSERT_ONCE(m_pLightsListDef.get());
    m_pLightsListDef->m_Defs[m_pLightsListDef->m_Defs.size() - 1].m_linearAttenuation = data;
    break;
  PK_CASE(LightsListChanged_LightsList_Light_QuadraticAttenuation):
    ODA_ASSERT_ONCE(m_pLightsListDef.get());
    m_pLightsListDef->m_Defs[m_pLightsListDef->m_Defs.size() - 1].m_quadraticAttenuation = data;
    break;
  PK_CASE(LightsListChanged_LightsList_Light_SpotCutoff):
    ODA_ASSERT_ONCE(m_pLightsListDef.get());
    m_pLightsListDef->m_Defs[m_pLightsListDef->m_Defs.size() - 1].m_spotCutoff = data;
    break;
  PK_CASE(LightsListChanged_LightsList_Light_SpotExponent):
    ODA_ASSERT_ONCE(m_pLightsListDef.get());
    m_pLightsListDef->m_Defs[m_pLightsListDef->m_Defs.size() - 1].m_spotExponent = data;
    break;
  PK_CASE(LightsListChanged_LightsList_Light_SpecularFactor):
    ODA_ASSERT_ONCE(m_pLightsListDef.get());
    m_pLightsListDef->m_Defs[m_pLightsListDef->m_Defs.size() - 1].m_specularFactor = data;
    break;
  default:
    ODA_FAIL_ONCE(); // TODO
    return false;
  }
  return true;
}

bool OdXmlGlesLoader::DropDouble(OdEnPathKey pk, double data)
{
  switch (pk)
  {
  PK_CASE(ViewportModified_ViewParams_FieldWidth):
  PK_CASE(OverlayViewParamsOverride_ViewParams_FieldWidth):
    ODA_ASSERT_ONCE(m_pViewParamsDef.get());
    m_pViewParamsDef->m_fieldWidth = data;
    break;
  PK_CASE(ViewportModified_ViewParams_FieldHeight):
  PK_CASE(OverlayViewParamsOverride_ViewParams_FieldHeight):
    ODA_ASSERT_ONCE(m_pViewParamsDef.get());
    m_pViewParamsDef->m_fieldHeight = data;
    break;
  PK_CASE(ViewportModified_ViewParams_LensLength):
  PK_CASE(OverlayViewParamsOverride_ViewParams_LensLength):
    ODA_ASSERT_ONCE(m_pViewParamsDef.get());
    m_pViewParamsDef->m_lensLength = data;
    break;
  PK_CASE(ViewportModified_ViewParams_FrontClipDist):
  PK_CASE(OverlayViewParamsOverride_ViewParams_FrontClipDist):
    ODA_ASSERT_ONCE(m_pViewParamsDef.get());
    m_pViewParamsDef->m_frontClip.m_clipDist = data;
    break;
  PK_CASE(ViewportModified_ViewParams_BackClipDist):
  PK_CASE(OverlayViewParamsOverride_ViewParams_BackClipDist):
    ODA_ASSERT_ONCE(m_pViewParamsDef.get());
    m_pViewParamsDef->m_backClip.m_clipDist = data;
    break;
  PK_CASE(ViewportModified_LineweightCoef):
    ODA_ASSERT_ONCE(m_pViewportDef.get());
    m_pViewportDef->m_lineweightsCoef = data,
    m_kindOfModViewportModified |= OdTrVisRendition::kViewportModLineweights;
    break;
  PK_CASE(VisualStyleAdded_Prop_DVal): PK_CASE(VisualStyleModified_Prop_DVal):
    ODA_ASSERT_ONCE(   m_pVisualStyleDef.get() && m_pVisualStyleProperty 
                    && m_typeVisualStyle == OdGiVariant::kDouble);
    m_pVisualStyleProperty->set(data, m_pVisualStyleProperty->operation());
    ODA_ASSERT_ONCE(m_typeVisualStyle == m_pVisualStyleProperty->type());
    break;
  PK_CASE_EX(_MetafileStream_Lineweight_Ps):
    ODA_ASSERT_ONCE(m_pWriter.get());
    m_streamWriter.writeDouble(data);
    break;
//PK_CASE_EX(_MetafileStream_TtfText_Text_Char_Multiplier):
//  ODA_ASSERT_ONCE(m_pWriter.get());
//  break;
  default:
    ODA_FAIL_ONCE(); // TODO
    //return false;
    return DropFloat(pk, (float) data);
  }
  return true;
}

bool OdXmlGlesLoader::DropUInts16(OdEnPathKey pk, const OdArray<OdUInt16, OdMemoryAllocator<OdUInt16> >& data)
{
  switch (pk)
  {
  PK_CASE_EX(_Array_ArrayData):
    if (metafile())
    {
      ODA_ASSERT_ONCE(   m_typeElement != OdTrVisArrayWrapper::Type_Invalid
                      && m_indexElement < metafile()->m_ArrayElements.size());
      OdUInt32 sz = data.size();
      ODA_ASSERT_ONCE(sz == m_numDataElement && m_typeElement == OdTrVisArrayWrapper::Type_Index);
      OdTrVisArrayWrapper& array = metafile()->m_ArrayElements[m_indexElement++];
      array.setArray(m_typeElement, data.getPtr(), sz * sizeof(OdUInt16));
      m_typeElement = OdTrVisArrayWrapper::Type_Invalid;
      m_numDataElement = 0;
    }
    break;
  default:
    ODA_FAIL_ONCE(); // TODO
    return false;
  }
  return true;
}

bool OdXmlGlesLoader::DropFloats(OdEnPathKey pk, const OdArray<float, OdMemoryAllocator<float> >& data)
{
  OdUInt32 sz = data.size();
  switch (pk)
  {
  PK_CASE(MaterialAdded_AmbientColor):
    ODA_ASSERT_ONCE(m_pMaterialDef.get() && sz == 3);
    m_pMaterialDef->m_ambientColor[0] = data[0], 
    m_pMaterialDef->m_ambientColor[1] = data[1], 
    m_pMaterialDef->m_ambientColor[2] = data[2];
    break;
  PK_CASE(MaterialAdded_ShadingAmbientColor):
    ODA_ASSERT_ONCE(m_pMaterialDef.get() && sz == 3);
    m_pMaterialDef->m_shadingAmbientColor[0] = data[0],
    m_pMaterialDef->m_shadingAmbientColor[1] = data[1],
    m_pMaterialDef->m_shadingAmbientColor[2] = data[2];
    break;
  PK_CASE(MaterialAdded_DiffuseColor):
    ODA_ASSERT_ONCE(m_pMaterialDef.get() && sz == 3);
    m_pMaterialDef->m_diffuseColor[0] = data[0],
    m_pMaterialDef->m_diffuseColor[1] = data[1],
    m_pMaterialDef->m_diffuseColor[2] = data[2];
    break;
  PK_CASE(MaterialAdded_ShadingDiffuseColor):
    ODA_ASSERT_ONCE(m_pMaterialDef.get() && sz == 3);
    m_pMaterialDef->m_shadingDiffuseColor[0] = data[0],
    m_pMaterialDef->m_shadingDiffuseColor[1] = data[1],
    m_pMaterialDef->m_shadingDiffuseColor[2] = data[2];
    break;
  PK_CASE(MaterialAdded_SpecularColor):
    ODA_ASSERT_ONCE(m_pMaterialDef.get() && sz == 3);
    m_pMaterialDef->m_specularColor[0] = data[0],
    m_pMaterialDef->m_specularColor[1] = data[1],
    m_pMaterialDef->m_specularColor[2] = data[2];
    break;
  PK_CASE(MaterialAdded_ShadingSpecularColor):
    ODA_ASSERT_ONCE(m_pMaterialDef.get() && sz == 3);
    m_pMaterialDef->m_shadingSpecularColor[0] = data[0],
    m_pMaterialDef->m_shadingSpecularColor[1] = data[1],
    m_pMaterialDef->m_shadingSpecularColor[2] = data[2];
    break;
  PK_CASE(MaterialAdded_EmissionColor):
    ODA_ASSERT_ONCE(m_pMaterialDef.get() && sz == 3);
    m_pMaterialDef->m_emissionColor[0] = data[0],
    m_pMaterialDef->m_emissionColor[1] = data[1],
    m_pMaterialDef->m_emissionColor[2] = data[2];
    break;
  PK_CASE(MaterialAdded_ShadingEmissionColor):
    ODA_ASSERT_ONCE(m_pMaterialDef.get() && sz == 3);
    m_pMaterialDef->m_shadingEmissionColor[0] = data[0],
    m_pMaterialDef->m_shadingEmissionColor[1] = data[1],
    m_pMaterialDef->m_shadingEmissionColor[2] = data[2];
    break;
  PK_CASE_EX(_Array_ArrayOffset):
    if (metafile())
    {
      ODA_ASSERT_ONCE(sz == 3);
      m_ArrayOffset.resize(3), 
      m_ArrayOffset[0] = data[0], m_ArrayOffset[1] = data[1], m_ArrayOffset[2] = data[2];
    }
    break;
  PK_CASE_EX(_Array_ArrayData):
    if (metafile())
    {
      ODA_ASSERT_ONCE(   m_typeElement != OdTrVisArrayWrapper::Type_Invalid
                      && m_indexElement < metafile()->m_ArrayElements.size());
      ODA_ASSERT_ONCE(  (   sz == (m_numDataElement * 3) 
                         && (   m_typeElement == OdTrVisArrayWrapper::Type_Vertex 
                             || m_typeElement == OdTrVisArrayWrapper::Type_Normal))
                      || (   sz == (m_numDataElement * 4) 
                          && m_typeElement == OdTrVisArrayWrapper::Type_Color)
                      || (   sz == (m_numDataElement * 2) 
                          && m_typeElement == OdTrVisArrayWrapper::Type_TexCoord)
                      || (  (sz ==  m_numDataElement)
                          && m_typeElement == OdTrVisArrayWrapper::Type_Depth));
      OdTrVisArrayWrapper& array = metafile()->m_ArrayElements[m_indexElement++];
      if (m_ArrayOffset.size() == 3)
      {
       float* pData = const_cast<float*>(data.getPtr());
       for (OdUInt32 idx = 0; idx < data.size(); idx++)
         pData[idx] += m_ArrayOffset[idx % 3];
      }
      m_ArrayOffset.clear();
      array.setArray(m_typeElement, data.getPtr(), sz * sizeof(float));
      m_typeElement = OdTrVisArrayWrapper::Type_Invalid;
      m_numDataElement = 0;
    }
    break;

  PK_CASE_EX(_MetafileStream_VPoint_Point):
    ODA_ASSERT_ONCE(m_pWriter.get() && sz == 3);
    m_streamWriter.writeUInt8((OdUInt8) OdTrVisMetaRecType_VPoint),
    m_streamWriter.writeFloat(data[0]), m_streamWriter.writeFloat(data[1]), m_streamWriter.writeFloat(data[2]);
    break;
  PK_CASE_EX(_MetafileStream_VLine_Point1):
    ODA_ASSERT_ONCE(m_pWriter.get() && sz == 3);
    m_streamWriter.writeUInt8((OdUInt8) OdTrVisMetaRecType_VLine),
    m_streamWriter.writeFloat(data[0]), m_streamWriter.writeFloat(data[1]), m_streamWriter.writeFloat(data[2]);
    break;
  PK_CASE_EX(_MetafileStream_VLine_Point2):
    ODA_ASSERT_ONCE(m_pWriter.get() && sz == 3);
    m_streamWriter.writeFloat(data[0]), m_streamWriter.writeFloat(data[1]), m_streamWriter.writeFloat(data[2]);
    break;

  PK_CASE(ViewportModified_NrcPoints_Point):
    ODA_ASSERT_ONCE(m_pViewportDef.get() && sz == 2);
    m_pViewportDef->m_nrcPoints.push_back(data[0]),
    m_pViewportDef->m_nrcPoints.push_back(data[1]);
    break;
  PK_CASE(ViewportModified_BorderPoints_Point):
    ODA_ASSERT_ONCE(m_pViewportDef.get() && sz == 2);
    m_pViewportDef->m_borderPoints.push_back(data[0]),
    m_pViewportDef->m_borderPoints.push_back(data[1]);
    break;

  PK_CASE(LightsListChanged_AmbientLight):
    ODA_ASSERT_ONCE(m_pLightsListDef.get() && sz == 4);
    m_pLightsListDef->m_ambientLight[0] = data[0],
    m_pLightsListDef->m_ambientLight[1] = data[1],
    m_pLightsListDef->m_ambientLight[2] = data[2],
    m_pLightsListDef->m_ambientLight[3] = data[3];
    break;
  PK_CASE(LightsListChanged_LightsList_Light_Color):
    ODA_ASSERT_ONCE(m_pLightsListDef.get() && sz == 3);
    m_pLightsListDef->m_Defs[m_pLightsListDef->m_Defs.size() - 1].m_color[0] = data[0],
    m_pLightsListDef->m_Defs[m_pLightsListDef->m_Defs.size() - 1].m_color[1] = data[1],
    m_pLightsListDef->m_Defs[m_pLightsListDef->m_Defs.size() - 1].m_color[2] = data[2];
    break;
  default:
    ODA_FAIL_ONCE(); // TODO
    return false;
  }
  return true;
}

bool OdXmlGlesLoader::DropDoubles(OdEnPathKey pk, const OdArray<double, OdMemoryAllocator<double> >& data)
{
  OdUInt32 sz = data.size();
  switch (pk)
  {
  PK_CASE(ViewportModified_ViewParams_ViewPosition):
  PK_CASE(OverlayViewParamsOverride_ViewParams_ViewPosition):
    ODA_ASSERT_ONCE(m_pViewParamsDef.get() && sz == 3);
    m_pViewParamsDef->m_viewPosition.x = data[0],
    m_pViewParamsDef->m_viewPosition.y = data[1],
    m_pViewParamsDef->m_viewPosition.z = data[2],
    m_kindOfModViewportModified |= OdTrVisRendition::kViewportModOrientation;
    break;
  PK_CASE(ViewportModified_ViewParams_ViewBasisX):
  PK_CASE(OverlayViewParamsOverride_ViewParams_ViewBasisX):
    ODA_ASSERT_ONCE(m_pViewParamsDef.get() && sz == 3);
    m_pViewParamsDef->m_viewBasis[0].x = data[0],
    m_pViewParamsDef->m_viewBasis[0].y = data[1],
    m_pViewParamsDef->m_viewBasis[0].z = data[2];
    break;
  PK_CASE(ViewportModified_ViewParams_ViewBasisY):
  PK_CASE(OverlayViewParamsOverride_ViewParams_ViewBasisY):
    ODA_ASSERT_ONCE(m_pViewParamsDef.get() && sz == 3);
    m_pViewParamsDef->m_viewBasis[1].x = data[0],
    m_pViewParamsDef->m_viewBasis[1].y = data[1],
    m_pViewParamsDef->m_viewBasis[1].z = data[2];
    break;
  PK_CASE(ViewportModified_ViewParams_ViewBasisZ):
  PK_CASE(OverlayViewParamsOverride_ViewParams_ViewBasisZ):
    ODA_ASSERT_ONCE(m_pViewParamsDef.get() && sz == 3);
    m_pViewParamsDef->m_viewBasis[2].x = data[0],
    m_pViewParamsDef->m_viewBasis[2].y = data[1],
    m_pViewParamsDef->m_viewBasis[2].z = data[2];
    break;
  PK_CASE(ViewportModified_ViewParams_ViewTarget):
  PK_CASE(OverlayViewParamsOverride_ViewParams_ViewTarget):
    ODA_ASSERT_ONCE(m_pViewParamsDef.get() && sz == 3);
    m_pViewParamsDef->m_viewTarget.x = data[0],
    m_pViewParamsDef->m_viewTarget.y = data[1],
    m_pViewParamsDef->m_viewTarget.z = data[2];
    break;
  PK_CASE(ViewportModified_ScreenRect):
    ODA_ASSERT_ONCE(m_pViewportDef.get() && sz == 4);
    m_pViewportDef->m_screenRect.m_min.x = data[0],
    m_pViewportDef->m_screenRect.m_min.y = data[1],
    m_pViewportDef->m_screenRect.m_max.x = data[2],
    m_pViewportDef->m_screenRect.m_max.y = data[3],
    m_kindOfModViewportModified |= OdTrVisRendition::kViewportModRect;
    break;
  PK_CASE(LightsListChanged_LightsList_Light_Position):
    ODA_ASSERT_ONCE(m_pLightsListDef.get() && sz == 3);
    m_pLightsListDef->m_Defs[m_pLightsListDef->m_Defs.size() - 1].m_position.set(data[0], data[1], data[2]);
    break;
  PK_CASE(LightsListChanged_LightsList_Light_Direction):
    ODA_ASSERT_ONCE(m_pLightsListDef.get() && sz == 3);
    m_pLightsListDef->m_Defs[m_pLightsListDef->m_Defs.size() - 1].m_direction.set(data[0], data[1], data[2]);
    break;
//PK_CASE_EX(_MetafileStream_TtfText_Direction):
//  ODA_ASSERT_ONCE(m_pWriter.get() && sz == 3);
//  break;
  PK_CASE(ExtentsChanged_SceneMin):
    ODA_ASSERT_ONCE(m_pExtentsDef.get() && sz == 3);
    m_pExtentsDef->m_sceneExtents.set(OdGePoint3d(data[0], data[1], data[2]), m_pExtentsDef->m_sceneExtents.maxPoint());
    break;
  PK_CASE(ExtentsChanged_SceneMax):
    ODA_ASSERT_ONCE(m_pExtentsDef.get() && sz == 3);
    m_pExtentsDef->m_sceneExtents.set(m_pExtentsDef->m_sceneExtents.minPoint(), OdGePoint3d(data[0], data[1], data[2]));
    break;
  PK_CASE(ExtentsChanged_RealMin):
    ODA_ASSERT_ONCE(m_pExtentsDef.get() && sz == 3);
    m_pExtentsDef->m_realExtents.set(OdGePoint3d(data[0], data[1], data[2]), m_pExtentsDef->m_realExtents.maxPoint());
    break;
  PK_CASE(ExtentsChanged_RealMax):
    ODA_ASSERT_ONCE(m_pExtentsDef.get() && sz == 3);
    m_pExtentsDef->m_realExtents.set(m_pExtentsDef->m_realExtents.minPoint(), OdGePoint3d(data[0], data[1], data[2]));
    break;
  PK_CASE(MetafileAdded_ExtMin):
    ODA_ASSERT_ONCE(m_pMetafileDef.get() && sz == 3);
    m_pMetafileDef->m_extents = OdGeExtents3d(OdGePoint3d(data[0], data[1], data[2]),
                                              m_pMetafileDef->m_extents.maxPoint());
    break;
  PK_CASE(MetafileAdded_ExtMax):
    ODA_ASSERT_ONCE(m_pMetafileDef.get() && sz == 3);
    m_pMetafileDef->m_extents = OdGeExtents3d(m_pMetafileDef->m_extents.minPoint(),
                                              OdGePoint3d(data[0], data[1], data[2]));
    break;
  default:
    {
      ODA_FAIL_ONCE(); // TODO
      //return false;
      OdArray<float, OdMemoryAllocator<float> > flts;
      flts.resize(data.size());
      for (OdUInt32 idx = 0; idx < data.size(); idx++)
        flts[idx] = (float) data[idx];
      return DropFloats(pk, flts);
    }
  }
  return true;
}

bool OdXmlGlesLoader::DropMatrix(OdEnPathKey pk, 
                                 const OdGeMatrix3d& data,
                                 const OdArray<float, OdMemoryAllocator<float> >* pDataFloats) // = NULL
{
  switch (pk)
  {
  PK_CASE_EX(_MetafileStream_PushMatrix_Matrix):
    ODA_ASSERT_ONCE(m_pWriter.get());
    m_streamWriter.writeUInt8((OdUInt8) OdTrVisMetaRecType_PushMatrix);
    //if (data == OdGeMatrix3d::kIdentity)
    //  m_streamWriter.writeUInt8((OdUInt8) OdTrVisPushMatrixType_Identity);
    //else
    {
      m_streamWriter.writeUInt8((OdUInt8) OdTrVisPushMatrixType_Full);
      m_streamWriter.writeMatrix3d(data); //m_streamWriter.writeMatrix3dArray(&data, sizeof(OdGeMatrix3d));
    }
    break;
//PK_CASE_EX(_MetafileStream_TtfText_Transform):
//  ODA_ASSERT_ONCE(m_pWriter.get() && pDataFloats);
//  m_streamWriter.writeFloatArray(pDataFloats->getPtr(), 16); // TODO OdUInt64 fontKeyA & OdUInt64 fontKeyB - are missing in xml
//  break;
  PK_CASE(ViewportModified_ViewParams_ScreenMatrix):
  PK_CASE(OverlayViewParamsOverride_ViewParams_ScreenMatrix):
    ODA_ASSERT_ONCE(m_pViewParamsDef.get());
    m_pViewParamsDef->m_screenMatrix = data;
    break;
  PK_CASE(ViewportModified_ViewParams_ViewingMatrix):
  PK_CASE(OverlayViewParamsOverride_ViewParams_ViewingMatrix):
    ODA_ASSERT_ONCE(m_pViewParamsDef.get());
    m_pViewParamsDef->m_viewingMatrix = data;
    break;
  PK_CASE(ViewportModified_ViewParams_ProjectionMatrix):
  PK_CASE(OverlayViewParamsOverride_ViewParams_ProjectionMatrix):
    ODA_ASSERT_ONCE(m_pViewParamsDef.get());
    m_pViewParamsDef->m_projectionMatrix = data;
    break;
  PK_CASE(ViewportModified_ViewParams_CorrectionMatrix):
  PK_CASE(OverlayViewParamsOverride_ViewParams_CorrectionMatrix):
    ODA_ASSERT_ONCE(m_pViewParamsDef.get());
    m_pViewParamsDef->m_correctionMatrix = data;
    break;
  PK_CASE(ViewportModified_ViewParams_OutputMatrix):
  PK_CASE(OverlayViewParamsOverride_ViewParams_OutputMatrix):
    ODA_ASSERT_ONCE(m_pViewParamsDef.get());
    m_pViewParamsDef->m_outputMatrix = data;
    break;
  PK_CASE(ViewportModified_ViewParams_MetafileMatrix):
  PK_CASE(OverlayViewParamsOverride_ViewParams_MetafileMatrix):
    ODA_ASSERT_ONCE(m_pViewParamsDef.get());
    m_pViewParamsDef->m_metafileMatrix = data;
    break;
  default:
    ODA_FAIL_ONCE(); // TODO
    return false;
  }
  return true;
}
