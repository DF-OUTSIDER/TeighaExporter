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
// GLES2 device xml server rendition

#ifndef ODGLES2XMLRENDITION
#define ODGLES2XMLRENDITION

#include "TD_PackPush.h"

#include "TrVisRendition.h"
#include "ExGsGLES2IdRegistratorImpl.h"
#include "XmlServer.h"
#include "ExGsGLES2XmlExports.h"

#define OD_GLES2XML_ENABLEPROGRAMDECISION

class OdGLES2XmlRenditionRedir;

/** \details
    <group ExRender_Windows_Classes> 
*/
class ODGLES2XML_EXPORT OdGLES2XmlRendition : public OdTrVisRenditionDef, protected OdGLES2RegistratorImpl::Traverser
{
  protected:
    // Settings
    ClientSettings m_clientSettings;
    // Id registrator
    mutable OdGLES2RegistratorImpl m_idReg;
    // Output
    OdSmartPtr<OdGLES2XmlServer> m_pXml;
    // Current mode
    enum ProcMode
    {
      kIntermediate = 0,
      kGsUpdate
    } m_mode;
    // Redirection controller
    OdGLES2XmlRenditionRedir *m_pRedir;
    // Current output state
    OdGLES2XmlServer::OutputState m_outputState;
    // Current processing device
    OdTrVisRenderClient *m_pProcDevice;
    // Overall viewport state
    OdTrVisViewportDef m_overallViewport;
  public:
    OdGLES2XmlRendition();
    ~OdGLES2XmlRendition();

    // XmlRendition methods

    OdIntPtr getClientSettings() const;
    void setClientSettings(OdIntPtr pClSet);

    OdIntPtr getXmlServer() const;
    void setXmlServer(OdIntPtr pXmlServer);

    OdGLES2XmlServer *xml();
    const OdGLES2XmlServer *xml() const;

    void setRedirectionController(OdGLES2XmlRenditionRedir *pRedir) { m_pRedir = pRedir; }

    void setIntermediateMode();
    void setGsUpdateMode();
    bool isIntermediateMode() const { return (m_mode == kIntermediate); }
    bool isGsUpdateMode() const { return (m_mode == kGsUpdate); }
    void DropMetadata(const OdTrVisFlatMetafileContainer *pMetafile);

    // High-level callbacks

    // Called on starting vectorizer update.
    virtual void onDeviceUpdateBegin(OdTrVisRenderClient *pDevice);
    // Called on ending vectorizer update.
    virtual void onDeviceUpdateEnd(OdTrVisRenderClient *pDevice);

    // Returns set of bit flags which indicate rendition capabilities/settings
    virtual OdUInt32 queryRenditionCaps(OdUInt32 requestFor = 0xFFFFFFFF) const;

    // Allocate new resource sharing provider compatible with rendition
    virtual OdTrVisSharingProviderClientPtr createSharingProvider();

    // Multithread processing flags
    virtual MtHandlerType mtDataTypeProcessing(DataType &type) const;

    // Returns Id registrator, or NULL if no registration supported
    virtual IdRegistrator *idRegistrator() const;

    // GsFiler support
    virtual bool saveRenditionState(OdGsFiler *pFiler) const;
    virtual bool loadRenditionState(OdGsFiler *pFiler, OdTrVisIdMap *pIdMap);

    // Mid-level callbacks

    // Client information

    virtual void obtainClientSettings(ClientSettings &clientSets);

    // Surface

    virtual void onSurfaceChanged(const OdTrVisSurfaceDef &pDef);

    // Overlays

    // Called when new overlay added into device.
    virtual void onOverlayAdded(OdTrVisOverlayId overlayId, const OdTrVisOverlayDef &pDef);
    // Called when exist overlay removed from device.
    virtual void onOverlayDeleted(OdTrVisOverlayId overlayId);
    // Called when overlay visibility changed.
    virtual void onOverlayVisibilityChanged(OdTrVisOverlayId overlayId, bool bVisibility);
    // Called when overlay rendering order changed.
    virtual void onOverlayRenderOrderChanged(OdTrVisOverlayId overlayId, OdInt16 nRenderOrder);

    // Viewports

    // Called when new viewport added into device.
    virtual void onViewportAdded(OdTrVisViewportId viewportId/*, const OdTrVisViewportDef &pDef*/);
    // Called when viewport removed from device.
    virtual void onViewportDeleted(OdTrVisViewportId viewportId);
    // Called when viewport inserted in list with some index.
    virtual void onViewportInserted(OdTrVisViewportId viewportId, int nIndex);
    // Called when viewport already exist in device modified.
    virtual void onViewportModified(OdTrVisViewportId viewportId, const OdTrVisViewportDef &pDef, OdUInt32 kindOfMod = kViewportModAll);

    // Viewports ordering lists

    // Called when order of metafiles in viewport is changed (for obtain modified part of list by renderer).
    virtual void onMetafileOrderChanged(OdTrVisViewportId viewportId, OdTrVisOverlayId overlayId, OdUInt32 nInvalidFrom, OdUInt32 nInvalidLen,
                                        const OdTrVisDisplayId *pValidFrom, OdUInt32 nValidLen);
    virtual void onMetafileOrderInheritance( OdTrVisViewportId viewportId1, OdTrVisOverlayId overlayId1, OdTrVisViewportId viewportId2 );

    // Viewport light lists

    // Called when lights list in viewport is changed.
    virtual void onLightsListChanged(OdTrVisViewportId viewportId, float ambientLight[4], bool bDefaultLightsOn,
                                     const OdTrVisLightDef *pList, OdUInt32 nEntries);

    // Viewport background

    // Called when background stream in viewport is changed.
    virtual void onBackgroundChanged(OdTrVisViewportId viewportId, OdTrVisFlatMetafileContainerPtr pStream,
                                     OdTrVisProgramId baseProgramId = kTrVisNegativeId);

    // Viewport extents

    // Called when viewport extents changed
    virtual void onExtentsChanged(OdTrVisViewportId viewportId, OdTrVisOverlayId overlayId, const OdTrVisExtentsDef &pDef);

    // Overlay viewport parameters override

    // Called when viewport needs override view parameters for specific overlay
    virtual void onOverlayViewParamsOverride(OdTrVisViewportId viewportId, OdTrVisOverlayId overlayId, bool bOverride,
                                             const OdTrVisViewParamsDef &pDef = OdTrVisViewParamsDef());

    // Metafiles

    // Called when new metafile added (returns visualId, equal to metafileId by default).
    virtual OdTrVisDisplayId onMetafileAdded(OdTrVisMetafileId metafileId, const OdTrVisMetafileDef &pDef);
    // Called when exist metafile deleted.
    virtual void onMetafileDeleted(OdTrVisMetafileId metafileId);
    // Called when exist metafile changed.
    //virtual void onMetafileModified(OdTrVisMetafileId metafileId, const OdTrVisMetafileDef &pDef);
    // Called when exist metafile visibility changed.
    virtual void onMetafileVisibilityChanged(OdTrVisMetafileId metafileId, bool bVisibility);
    // Called when exist metafile highlighting changed.
    virtual void onMetafileHighlightingChanged(OdTrVisMetafileId metafileId, bool bHighlight);
    // Called when exist metafile fading changed.
    virtual void onMetafileFadingChanged(OdTrVisMetafileId metafileId, bool bFade);
    // Called to attach highlighting branch to metafile.
    virtual void onMetafileHlBranchAttached(OdTrVisMetafileId metafileId, OdTrVisHlBranchId hlBranchId);
    // Called to detach highlighting branch from metafile.
    virtual void onMetafileHlBranchDetached(OdTrVisMetafileId metafileId);

    // Highlighting

    // Called when new highlighting branch added.
    virtual void onHlBranchAdded(OdTrVisHlBranchId hlBranchId, const OdTrVisHlBranchDef &pDef);
    // Called when exist highlighting branch modified.
    virtual void onHlBranchModified(OdTrVisHlBranchId hlBranchId, const OdTrVisHlBranchDef &pDef,
                                    bool bChildsModified, bool bMarkersModified);
    // Called when exist highlighting branch deleted.
    virtual void onHlBranchDeleted(OdTrVisHlBranchId hlBranchId);

    // Textures

    // Called when new texture added.
    virtual void onTextureAdded(OdTrVisTextureId textureId, const OdTrVisTextureDef &pDef);
    // Called when exist texture deleted.
    virtual void onTextureDeleted(OdTrVisTextureId textureId);

    // Materials

    // Called when new material added.
    virtual void onMaterialAdded(OdTrVisMaterialId materialId, const OdTrVisMaterialDef &pDef);
    // Called when exist material deleted.
    virtual void onMaterialDeleted(OdTrVisMaterialId materialId);

    // Visual Styles

    // Called when new visual style added.
    virtual void onVisualStyleAdded(OdTrVisVisualStyleId visualStyleId, const OdTrVisVisualStyle &pDef);
    // Called when exist visual style modified.
    virtual void onVisualStyleModified(OdTrVisVisualStyleId visualStyleId, const OdTrVisVisualStyle &pDef);
    // Called when exist visual style deleted.
    virtual void onVisualStyleDeleted(OdTrVisVisualStyleId visualStyleId);

    // Layers

    // Called when new layer added.
    virtual void onLayerAdded(OdTrVisLayerId layerId, const OdTrVisLayerDef &pDef);
    // Called when exist layer modified.
    virtual void onLayerModified(OdTrVisLayerId layerId, const OdTrVisLayerDef &pDef);
    // Called when exist layer deleted.
    virtual void onLayerDeleted(OdTrVisLayerId layerId);
    //Called when layer viewport-dependent properties changed
    virtual void onLayerViewportPropsOverride(OdTrVisViewportId viewportId, OdTrVisLayerId layerId, bool bOverride, const OdTrVisLayerProps &pProps = OdTrVisLayerProps() );

  protected:
    void beginDeviceProcessing(OdTrVisRenderClient *pDevice);
    void endDeviceProcessing(OdTrVisRenderClient *pDevice);
    OdTrVisRenderClient *processingDevice() const;

    bool isSupportCompositeMetafiles() const;
    bool isSupportVisualStyles() const;
    bool isSupportOverlayBuffers() const;

    virtual bool idRegTraverseCallback(OdTrVisRendition::DataType type, OdTrVisId id, OdTrVisUniqueID *pRegId);
    void finalizeXmlServerUsage();

    struct DDL2ndPassInfo
    {
      bool m_bScan;
      OdUInt32 m_nTransFirst, m_nTransLast;
      DDL2ndPassInfo() : m_bScan(false), m_nTransFirst(0xFFFFFFFF), m_nTransLast(0xFFFFFFFF) { }
    };
    bool DropDisplayList(const OdTrVisDisplayId *pDispList, OdUInt32 nListLen, DDL2ndPassInfo *p2ndPass = NULL);
    void DropDisplayList2ndPass(OdTrVisViewportId viewportId, OdTrVisOverlayId overlayId, OdUInt32 nInvalidFrom,
                                const OdTrVisDisplayId *pDispList, OdUInt32 nListLen, DDL2ndPassInfo &p2ndPass);
    void onMetafileOrderChangedImpl(OdTrVisViewportId viewportId, OdTrVisOverlayId overlayId, OdUInt32 nInvalidFrom, OdUInt32 nInvalidLen,
                                    const OdTrVisDisplayId *pValidFrom, OdUInt32 nValidLen, bool bFirstPass = true);
    void onMetafileOrderInheritanceImpl( OdTrVisViewportId viewportId1, OdTrVisOverlayId overlayId1, OdTrVisViewportId viewportId2 );

    void DropHlBranch(const OdTrVisHlBranchDef &pHlBranch, bool bDropChilds, bool bDropMarkers);
    void DropVisualStyleData(const OdTrVisVisualStyle &pVStyle, bool bDropNonModified = true);
    void DropLayerData(const OdTrVisLayerDef &pLayer, bool bDropOwnerId = true);
    void DropLayerProps(const OdTrVisLayerProps &pLayerProps);
    void ParseMetafileStream(const OdTrVisFlatMetafileContainer *pMetafile);

    void OutputState(OdGLES2XmlServer::OutputState newState, bool bForce = false);

    static void DropViewParams(OdGLES2XmlServer *pXml, const OdTrVisViewParamsDef &pDef);
  public:
    static void ParseMetafileStream(OdGLES2XmlServer *pXml, const OdTrVisFlatMetafileContainer *pMetafile)
    {
      OdSmartPtr<OdGLES2XmlRendition> res = OdRxObjectImpl<OdGLES2XmlRendition>::createObject();
      res->setXmlServer((OdIntPtr)pXml);
      res->ParseMetafileStream(pMetafile);
      res->DropMetadata(pMetafile);
    }
};

/** \details
    <group ExRender_Windows_Classes> 
*/
class OdGLES2XmlRenditionRedir : public OdTrVisRenditionRedir, protected OdTrVisRenditionRedir::IdRegistratorRedir
{
  protected:
    OdStaticRxObject<OdGLES2XmlRendition> m_xmlRendition;
    OdStaticRxObject<OdTrVisRenditionRecorder> m_recorder;
  public:
    OdGLES2XmlRenditionRedir()
      : OdTrVisRenditionRedir()
    {
      setRedirection(&m_recorder);
      setInformationalRedirection(&m_xmlRendition);
      setIdRegRedirection(m_recorder.idRegistrator());
      m_xmlRendition.setRedirectionController(this);
    }

    void setForRecord() { setRedirection(&m_recorder); }
    bool isSetForRecord() const { return (getRedirection() == &m_recorder); }
    void setForPlay() { setRedirection(&m_xmlRendition); }
    bool isSetForPlay() const { return (getRedirection() == &m_xmlRendition); }

    void dropRecords() { m_recorder.playRecords(&m_xmlRendition, true); }

    OdGLES2XmlRendition &xmlRendition() { return m_xmlRendition; }
    const OdGLES2XmlRendition &xmlRendition() const { return m_xmlRendition; }

    // Returns Id registrator, or NULL if no registration supported
    virtual OdTrVisRendition::IdRegistrator *idRegistrator() const
    {
      if (isSetForRecord() && getInformationalRedirection()->idRegistrator())
        return (OdTrVisRendition::IdRegistrator*)this;
      return getInformationalRedirection()->idRegistrator();
    }
  protected:
    // IdRegistrator override
    virtual OdTrVisUniqueID *get(DataType type, OdTrVisId id, bool bKill = false) const
    {
      OdTrVisRenditionRedir::IdRegistratorRedir::get(type, id, bKill);
      return getInformationalRedirection()->idRegistrator()->get(type, id);
    }
};

#include "TD_PackPop.h"

#endif // ODGLES2XMLRENDITION
