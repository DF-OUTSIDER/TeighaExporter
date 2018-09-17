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

#include "OdaCommon.h"
#include "Gs/GsFiler.h"
#include "TrVisRenderClient.h"
#include "TrVisTtfFontsCacheClient.h"
#include "TrVisIdMap.h"
#include "ExGsGLES2XmlSharingProvider.h"
#include "ExGsGLES2XmlRendition.h"

#define OD_GLES2XML_DROPHLTASCLIENTONLY

// #define OD_GLES2XML_DROPSCD_TEST

#ifdef OD_GLES2XML_DROPSCD_TEST
#include "ExGsGLES2ShareableClientData.h"

struct DropSCDTest : public OdGLES2XmlServerStub
{
  OdGLES2XmlServer **m_ppWhere;
  OdGLES2XmlServer  *m_pWho;
  static OdTrVecDevice *m_pDevice;

  DropSCDTest(OdGLES2XmlServer **ppWhere)
    : m_ppWhere(ppWhere)
    , m_pWho(*ppWhere)
  {
    *ppWhere = this;
  }
  ~DropSCDTest()
  {
    if (*m_ppWhere != m_pWho)
      *m_ppWhere = m_pWho;
  }

  virtual bool UpdateClientDependentSectionOnly() const { return m_pWho->UpdateClientDependentSectionOnly(); }

  virtual bool DropShareableClientData(IdType /*idType*/, IdSubType /*idSubType*/, OdUInt64 resId, OdTrVisUniqueID* pRegId,
                                       OdUInt32 nBinaryFields, const void ** const pBinaryFields, const OdUInt32* nBinaryFieldsData,
                                       OdUInt32 /*nIdMappings*/ = 0, const IdMapping* pIdMappings = NULL)
  {
    OdUInt8Array pkgr;
    for (OdUInt32 n = 0; n < nBinaryFields; n++)
    {
      if (pBinaryFields[n])
        pkgr.insert(pkgr.end(), (const OdUInt8*)pBinaryFields[n], ((const OdUInt8*)pBinaryFields[n] + nBinaryFieldsData[n]));
    }
    *m_ppWhere = m_pWho;
    if (pkgr.size())
      OdGLES2ShareableClientData::process(m_pWho, resId, pRegId, pkgr.getPtr(), pIdMappings, m_pDevice->clientSettings(), &m_pDevice->outputRect());
    return true;
  }
};

OdTrVecDevice *DropSCDTest::m_pDevice = NULL;
#endif

OdGLES2XmlRendition::OdGLES2XmlRendition()
  : m_pXml()
  , m_mode(kIntermediate)
  , m_pRedir(NULL)
  , m_outputState(OdGLES2XmlServer::kIntermediateState)
  , m_pProcDevice(NULL)
{
  m_overallViewport.setDefault();
}

OdGLES2XmlRendition::~OdGLES2XmlRendition()
{
  finalizeXmlServerUsage();
}

OdIntPtr OdGLES2XmlRendition::getClientSettings() const
{
  return (OdIntPtr)&m_clientSettings;
}

void OdGLES2XmlRendition::setClientSettings(OdIntPtr pClSet)
{
  m_clientSettings = *(ClientSettings*)pClSet;
}

OdIntPtr OdGLES2XmlRendition::getXmlServer() const
{
  return (OdIntPtr)m_pXml.get();
}

void OdGLES2XmlRendition::setXmlServer(OdIntPtr pXmlServer)
{
  if (m_pXml.get() != (OdGLES2XmlServer*)pXmlServer)
    finalizeXmlServerUsage();
  m_pXml = (OdGLES2XmlServer*)pXmlServer;
}

OdGLES2XmlServer *OdGLES2XmlRendition::xml()
{
  if (m_pXml.isNull()) throw OdError(eNullPtr);
  return m_pXml;
}

const OdGLES2XmlServer *OdGLES2XmlRendition::xml() const
{
  if (m_pXml.isNull()) throw OdError(eNullPtr);
  return m_pXml;
}

void OdGLES2XmlRendition::setIntermediateMode()
{
  m_mode = kIntermediate;
  m_pRedir->setForRecord();
}

void OdGLES2XmlRendition::setGsUpdateMode()
{
  m_mode = kGsUpdate;
  m_pRedir->setForPlay();
}

class OdGLES2XmlNestingLevel
{
  OdGLES2XmlServer *m_pXml;
  const char *m_pText;
  public:
    OdGLES2XmlNestingLevel(OdGLES2XmlServer *pXml, const char *pText)
      : m_pXml(pXml)
      , m_pText(pText)
    {
      pXml->IncreaseNesting(pText);
    }
    ~OdGLES2XmlNestingLevel()
    {
      m_pXml->DecreaseNesting(m_pText);
    }
};

#define XML_LEVEL_STATIC(pXml, nLevel, pText) \
  OdGLES2XmlNestingLevel _xmlNsL##nLevel(pXml, pText)
#define XML_LEVEL0_STATIC(pXml, pText) \
  XML_LEVEL_STATIC(pXml, 0, pText)

#define XML_LEVEL(nLevel, pText) \
  OdGLES2XmlNestingLevel _xmlNsL##nLevel(xml(), pText)
#define XML_LEVEL0(pText) \
  XML_LEVEL(0, pText)

void OdGLES2XmlRendition::beginDeviceProcessing(OdTrVisRenderClient *pDevice)
{
  ODA_ASSERT(!m_pProcDevice);
  m_pProcDevice = pDevice;
}

void OdGLES2XmlRendition::endDeviceProcessing(OdTrVisRenderClient *
#ifdef ODA_DIAGNOSTICS
  pDevice
#endif
  )
{
  ODA_ASSERT(m_pProcDevice == pDevice);
  m_pProcDevice = NULL;
}

OdTrVisRenderClient *OdGLES2XmlRendition::processingDevice() const
{
  ODA_ASSERT(m_pProcDevice);
  return m_pProcDevice;
}

inline OdGLES2XmlServer::IdType renditionDataType2ServerIdType(OdTrVisRendition::DataType type)
{
  OdGLES2XmlServer::IdType                   idType = OdGLES2XmlServer::kUnknownId;
  switch (type)
  {
    case OdTrVisRendition::kViewportData:    idType = OdGLES2XmlServer::kViewportId;    break;
    case OdTrVisRendition::kMetafileData:    idType = OdGLES2XmlServer::kMetafileId;    break;
    case OdTrVisRendition::kHlBranchData:    idType = OdGLES2XmlServer::kHlBranchId;    break;
    case OdTrVisRendition::kTextureData:     idType = OdGLES2XmlServer::kTextureId;     break;
    case OdTrVisRendition::kMaterialData:    idType = OdGLES2XmlServer::kMaterialId;    break;
    case OdTrVisRendition::kProgramData:     idType = OdGLES2XmlServer::kProgramId;     break;
    case OdTrVisRendition::kVisualStyleData: idType = OdGLES2XmlServer::kVisualStyleId; break;
    case OdTrVisRendition::kLayerData:       idType = OdGLES2XmlServer::kLayerId;       break;
    case OdTrVisRendition::kOverlayData:     idType = OdGLES2XmlServer::kOverlayId;     break;
    default:                                                                            break;
  }
  return idType;
}

bool OdGLES2XmlRendition::idRegTraverseCallback(OdTrVisRendition::DataType type, OdTrVisId id, OdTrVisUniqueID *pRegId)
{
  OdGLES2XmlServer::IdType idType = renditionDataType2ServerIdType(type);
  if (idType == OdGLES2XmlServer::kUnknownId) return true;
  xml()->UnregisterId(id, pRegId, idType);    return true;
}

void OdGLES2XmlRendition::finalizeXmlServerUsage()
{
  if (m_pRedir && m_pXml.get())
  {
    m_pRedir->dropRecords(); // ???
    m_idReg.traverse(this);
    m_idReg.killAll();
  }
}

void OdGLES2XmlRendition::onDeviceUpdateBegin(OdTrVisRenderClient *pDevice)
{
#ifdef OD_GLES2XML_DROPSCD_TEST
  DropSCDTest::m_pDevice = static_cast<OdTrVecDevice*>(pDevice);
#endif
  OutputState(OdGLES2XmlServer::kIntermediateState, true);

  xml()->IncreaseNesting("GsUpdate");
  setGsUpdateMode();

  beginDeviceProcessing(pDevice);
  if (!updateClientDependentSectionOnly())
    m_pRedir->dropRecords(); // Output all collected between GsUpdate's data
}

void OdGLES2XmlRendition::onDeviceUpdateEnd(OdTrVisRenderClient *pDevice)
{
  setIntermediateMode();
  OutputState(OdGLES2XmlServer::kIntermediateState);
  xml()->DecreaseNesting("GsUpdate");

  endDeviceProcessing(pDevice);
}

void OdGLES2XmlRendition::obtainClientSettings(ClientSettings &clientSets)
{
  clientSets = m_clientSettings;
}

OdTrVisSharingProviderClientPtr OdGLES2XmlRendition::createSharingProvider()
{
  return OdRxObjectImpl<OdGLES2XmlSharingProvider, OdTrVisSharingProviderClient>::createObject();
}

OdTrVisRendition::MtHandlerType OdGLES2XmlRendition::mtDataTypeProcessing(DataType &type) const
{
  switch (type)
  {
    case kSurfaceData:
    case kViewportData: case kViewportPropsData: case kOrderData: case kLightsData: case kBackgroundData: case kExtentsData:
    case kProgramData:
    case kOverlayData: case kOverlayPropsData:
    return kMtRedirect; // Redirect without Mt handler since not called from Mt
    case kMetafileData: case kMetafilePropsData:
    case kHlBranchData:
    case kTextureData:
    case kMaterialData:
    case kVisualStyleData:
    case kLayerData:
    return kMtGlobal; // Secure by global mutex since all passed through one XmlServer
    default:
      ODA_FAIL();
      return kMtRedirect;
  };
}

// Returns set of bit flags which indicate rendition capabilities/settings
OdUInt32 OdGLES2XmlRendition::queryRenditionCaps(OdUInt32 requestFor) const
{
  OdUInt32 retCaps = kSupportGsStateSaving | kSupportCompositeMetafiles | kSupportVisualStyles | kSupportOverlayBuffers;
  if (GETBIT(requestFor, kUpdateClientSectionOnly))
    SETBIT(retCaps, kUpdateClientSectionOnly,    xml()->UpdateClientDependentSectionOnly());
  if (GETBIT(requestFor, kUpdateShareableSectionOnly))
    SETBIT(retCaps, kUpdateShareableSectionOnly, xml()->UpdateShareableSectionOnly());
  return retCaps;
}

bool OdGLES2XmlRendition::isSupportCompositeMetafiles() const
{
  ODA_ASSERT(processingDevice());
  return processingDevice()->useCompositeMetafiles();
}
bool OdGLES2XmlRendition::isSupportVisualStyles() const
{
  ODA_ASSERT(processingDevice());
  return processingDevice()->useVisualStyles();
}
bool OdGLES2XmlRendition::isSupportOverlayBuffers() const
{
  ODA_ASSERT(processingDevice());
  return processingDevice()->useOverlays();
}

// Returns Id registrator, or NULL if no registration supported
OdTrVisRendition::IdRegistrator *OdGLES2XmlRendition::idRegistrator() const
{
  return &m_idReg;
}

bool OdGLES2XmlRendition::saveRenditionState(OdGsFiler *pFiler) const
{
  pFiler->wrChar('X');
  // Save client settings
  //if (!::odTrVisSaveClientSettings(pFiler, m_clientSettings))
  //  return false;
  // Save Id mapping
  if (!m_idReg.save(pFiler))
    return false;

  xml()->saveState(pFiler);

  // Save overall viewport
  if (!::odTrVisSaveViewportDef(pFiler, m_overallViewport))
    return false;
  return true;
}

bool OdGLES2XmlRendition::loadRenditionState(OdGsFiler *pFiler, OdTrVisIdMap *pIdMap)
{
  if (pFiler->rdChar() != 'X')
    return false;
  // Load client settings
  //if (!::odTrVisLoadClientSettings(pFiler, m_clientSettings))
  //  return false;
  // Load Id mapping
  if (!m_idReg.load(pFiler, pIdMap))
    return false;

  xml()->loadState(pFiler, pIdMap);

  // Load overall viewport
  if (!::odTrVisLoadViewportDef(pFiler, m_overallViewport, pIdMap))
    return false;
  return true;
}

void OdGLES2XmlRendition::onSurfaceChanged(const OdTrVisSurfaceDef &pDef)
{
  OutputState(OdGLES2XmlServer::kClientDependentState);
  XML_LEVEL0("SurfaceChanged");
    xml()->DropInt32("SurfaceSizeXMin", pDef.m_surfaceSize.m_min.x);
    xml()->DropInt32("SurfaceSizeYMin", pDef.m_surfaceSize.m_min.y);
    xml()->DropInt32("SurfaceSizeXMax", pDef.m_surfaceSize.m_max.x);
    xml()->DropInt32("SurfaceSizeYMax", pDef.m_surfaceSize.m_max.y);
    xml()->DropUInt8RGB("BackgroundColor", ODGETRED(pDef.m_backgroundColor), ODGETGREEN(pDef.m_backgroundColor), ODGETBLUE(pDef.m_backgroundColor), 255);
    xml()->DropBooleanOpt("TransparentBackground", pDef.isTransparentBackground());
}

struct xml_RegistrationBase
{
  OdGLES2XmlServer *m_pXml;
  OdTrVisUniqueID *m_pRegId;
  OdTrVisRendition::DataType m_dataType;
  OdGLES2XmlServer::IdType m_idType;
  OdTrVisId m_resId;

  xml_RegistrationBase(OdGLES2XmlServer *pXml, OdGLES2RegistratorImpl &idReg, OdTrVisRendition::DataType dataType, OdTrVisId resId, bool idDel = false)
    : m_pXml(pXml)
    , m_pRegId(idReg.get(dataType, resId, idDel))
    , m_dataType(dataType)
    , m_idType(renditionDataType2ServerIdType(dataType))
    , m_resId(resId)
  {
  }
};
struct xml_RegisterId : public xml_RegistrationBase
{
  xml_RegisterId(OdGLES2XmlServer *pXml, OdGLES2RegistratorImpl &idReg, OdTrVisRendition::DataType dataType, OdTrVisId resId)
    : xml_RegistrationBase(pXml, idReg, dataType, resId)
  {
    m_pXml->RegisterId(m_resId, m_pRegId, m_idType);
  }
};
struct xml_UnregisterId : public xml_RegistrationBase
{
  xml_UnregisterId(OdGLES2XmlServer *pXml, OdGLES2RegistratorImpl &idReg, OdTrVisRendition::DataType dataType, OdTrVisId resId)
    : xml_RegistrationBase(pXml, idReg, dataType, resId) { }
  ~xml_UnregisterId()
  {
    m_pXml->UnregisterId(m_resId, m_pRegId, m_idType);
  }
};
#define XML_REGISTERED(dataType, resId) \
  xml_RegistrationBase _xmlReg(xml(), m_idReg, dataType, resId)
#define XML_REGISTER(dataType, resId) \
  xml_RegisterId _xmlReg(xml(), m_idReg, dataType, resId)
#define XML_UNREGISTER(dataType, resId) \
  xml_UnregisterId _xmlReg(xml(), m_idReg, dataType, resId)
#define XML_DROP_REG_ID(pTagName) \
  xml()->DropId(pTagName, _xmlReg.m_resId, _xmlReg.m_pRegId, _xmlReg.m_idType)

void OdGLES2XmlRendition::onOverlayAdded(OdTrVisOverlayId overlayId, const OdTrVisOverlayDef &pDef)
{
  XML_REGISTER(kOverlayData, overlayId);
  OutputState(OdGLES2XmlServer::kClientDependentState);
  XML_LEVEL0("OverlayAdded");
    XML_DROP_REG_ID("OverlayID");
    xml()->DropUInt16("Flags", pDef.m_overlayFlags);
    xml()->DropInt16("RenderOrder", pDef.m_renderOrder);
}

void OdGLES2XmlRendition::onOverlayDeleted(OdTrVisOverlayId overlayId)
{
  XML_UNREGISTER(kOverlayData, overlayId);
  OutputState(OdGLES2XmlServer::kClientDependentState);
  XML_LEVEL0("OverlayDeleted");
    XML_DROP_REG_ID("OverlayID");
}

void OdGLES2XmlRendition::onOverlayVisibilityChanged(OdTrVisOverlayId overlayId, bool bVisibility)
{
  XML_REGISTERED(kOverlayData, overlayId);
  OutputState(OdGLES2XmlServer::kClientDependentState);
  XML_LEVEL0("OverlayVisibilityChanged");
    XML_DROP_REG_ID("OverlayID");
    xml()->DropBoolean("Visibility", bVisibility);
}

void OdGLES2XmlRendition::onOverlayRenderOrderChanged(OdTrVisOverlayId overlayId, OdInt16 nRenderOrder)
{
  XML_REGISTERED(kOverlayData, overlayId);
  OutputState(OdGLES2XmlServer::kClientDependentState);
  XML_LEVEL0("OverlayRenderOrderChanged");
    XML_DROP_REG_ID("OverlayID");
    xml()->DropInt16("RenderOrder", nRenderOrder);
}

void OdGLES2XmlRendition::onViewportAdded(OdTrVisViewportId viewportId/*, const OdTrVisViewportDef &pDef*/)
{
  XML_REGISTER(kViewportData, viewportId);
  OutputState(OdGLES2XmlServer::kClientDependentState);
  XML_LEVEL0("ViewportAdded");
    XML_DROP_REG_ID("ViewportID");
}

void OdGLES2XmlRendition::onViewportDeleted(OdTrVisViewportId viewportId)
{
  XML_UNREGISTER(kViewportData, viewportId);
  OutputState(OdGLES2XmlServer::kClientDependentState);
  XML_LEVEL0("ViewportDeleted");
    XML_DROP_REG_ID("ViewportID");
}

void OdGLES2XmlRendition::onViewportInserted(OdTrVisViewportId viewportId, int nIndex)
{
  XML_REGISTER(kViewportData, viewportId);
  OutputState(OdGLES2XmlServer::kClientDependentState);
  XML_LEVEL0("ViewportInserted");
    XML_DROP_REG_ID("ViewportID");
    xml()->DropInt32("Index", nIndex);
}

void OdGLES2XmlRendition::DropViewParams(OdGLES2XmlServer *pXml, const OdTrVisViewParamsDef &pDef)
{
  // @@@TODO: remove this section when client will be completed
  pXml->DropMatrix("ScreenMatrix", pDef.m_screenMatrix);
  pXml->DropMatrix("ViewingMatrix", pDef.m_viewingMatrix);
  pXml->DropMatrix("ProjectionMatrix", pDef.m_projectionMatrix);
  pXml->DropMatrix("CorrectionMatrix", pDef.m_correctionMatrix);
  pXml->DropMatrix("OutputMatrix", pDef.m_outputMatrix);
  // @@@eof tmp section
  pXml->DropMatrix("MetafileMatrix", pDef.m_metafileMatrix);
  pXml->DropDouble3("ViewPosition", pDef.m_viewPosition.x, pDef.m_viewPosition.y, pDef.m_viewPosition.z);
  pXml->DropDouble3("ViewBasisX", pDef.m_viewBasis[0].x, pDef.m_viewBasis[0].y, pDef.m_viewBasis[0].z);
  pXml->DropDouble3("ViewBasisY", pDef.m_viewBasis[1].x, pDef.m_viewBasis[1].y, pDef.m_viewBasis[1].z);
  pXml->DropDouble3("ViewBasisZ", pDef.m_viewBasis[2].x, pDef.m_viewBasis[2].y, pDef.m_viewBasis[2].z);
  pXml->DropBoolean("PerspectiveProjection", pDef.m_bPerspectiveProjection);
  pXml->DropDouble("FieldWidth", pDef.m_fieldWidth);
  pXml->DropDouble("FieldHeight", pDef.m_fieldHeight);
  pXml->DropDouble("LensLength", pDef.m_lensLength);
  pXml->DropDouble3("ViewTarget", pDef.m_viewTarget.x, pDef.m_viewTarget.y, pDef.m_viewTarget.z);
  pXml->DropBoolean("FrontClip", pDef.m_frontClip.m_bClipped);
  if (pDef.m_frontClip.m_bClipped)
    pXml->DropDouble("FrontClipDist", pDef.m_frontClip.m_clipDist);
  pXml->DropBoolean("BackClip", pDef.m_backClip.m_bClipped);
  if (pDef.m_backClip.m_bClipped)
    pXml->DropDouble("BackClipDist", pDef.m_backClip.m_clipDist);
}

void OdGLES2XmlRendition::onViewportModified(OdTrVisViewportId viewportId, const OdTrVisViewportDef &pDef, OdUInt32 kindOfMod)
{
  OutputState(OdGLES2XmlServer::kClientDependentState);
  XML_REGISTERED(kViewportData, viewportId);
  if (pDef.isPSOverallViewport())
  {
    if (GETBIT(kindOfMod, OdTrVisRendition::kViewportModOrientation))
    {
      m_overallViewport.m_viewParams = pDef.m_viewParams;
      m_overallViewport.m_viewParams.m_correctionMatrix = pDef.m_viewParams.m_screenMatrix * pDef.m_viewParams.m_projectionMatrix
                                                                                           * pDef.m_viewParams.m_viewingMatrix; // hack
    }
    if (GETBIT(kindOfMod, OdTrVisRendition::kViewportModRect))
    {
      m_overallViewport.m_vptFlags = pDef.m_vptFlags;
      m_overallViewport.m_overallViewportId = pDef.m_overallViewportId;
      m_overallViewport.m_lineStyleConfig = pDef.m_lineStyleConfig;
    }
    if (GETBIT(kindOfMod, OdTrVisRendition::kViewportModRect))
    {
      // Recomputor waits rect in LL-UR specification
      m_overallViewport.m_screenRect.m_min.x = pDef.m_screenRect.m_min.x;
      m_overallViewport.m_screenRect.m_min.y = pDef.m_screenRect.m_max.y;
      m_overallViewport.m_screenRect.m_max.x = pDef.m_screenRect.m_max.x;
      m_overallViewport.m_screenRect.m_max.y = pDef.m_screenRect.m_min.y;
    }
  }

  XML_LEVEL0_STATIC(xml(), "ViewportModified");
  xml()->DropId("ViewportID", viewportId, _xmlReg.m_pRegId, OdGLES2XmlServer::kViewportId);
  xml()->DropUInt32("KindOfMod", kindOfMod);
  if (GETBIT(kindOfMod, kViewportModVisibility))
  {
    xml()->DropBoolean("Visibility", pDef.m_bVisible);
  }
  if (GETBIT(kindOfMod, kViewportModOrientation))
  {
    XML_LEVEL_STATIC(xml(), 1, "ViewParams");
      DropViewParams(xml(), pDef.m_viewParams);
  }
  if (GETBIT(kindOfMod, kViewportModPersistent))
  {
    xml()->DropBooleanOpt("PSModelViewport", pDef.isPSModelViewport());
    if (pDef.m_overallViewportId && _xmlReg.m_pRegId )
      xml()->DropId("OverallVpId", pDef.m_overallViewportId,
        m_idReg.get(kViewportData, pDef.m_overallViewportId) );
    xml()->DropBooleanOpt("PSOverallViewport", pDef.isPSOverallViewport());
    xml()->DropBooleanOpt("PSHelperViewport", pDef.isPSHelperViewport());
  }
  if (GETBIT(kindOfMod, kViewportModContextual))
  {
    xml()->DropUInt8RGB("LineStyleConfig", ODGETRED(pDef.m_lineStyleConfig), ODGETGREEN(pDef.m_lineStyleConfig), ODGETBLUE(pDef.m_lineStyleConfig), ODGETALPHA(pDef.m_lineStyleConfig));
    xml()->DropUInt8RGB("FadingConfig",    ODGETRED(pDef.m_fadingConfig),    ODGETGREEN(pDef.m_fadingConfig),    ODGETBLUE(pDef.m_fadingConfig), ODGETALPHA(pDef.m_fadingConfig));
  }
  if (GETBIT(kindOfMod, kViewportModRect))
  {
    xml()->DropDouble4("ScreenRect", pDef.m_screenRect.m_min.x, pDef.m_screenRect.m_min.y,
                                    pDef.m_screenRect.m_max.x, pDef.m_screenRect.m_max.y);
  }
  if (GETBIT(kindOfMod, kViewportModNrcClip))
  {
    xml()->DropUInt32("NNrcCounts", pDef.m_nrcCounts.size());
    xml()->DropUInt32("NNrcPoints", pDef.m_nrcPoints.size() / 2);
    if (!pDef.m_nrcCounts.isEmpty())
    {
      XML_LEVEL_STATIC(xml(), 1, "NrcCounts");
        for (OdUInt32 n = 0; n < pDef.m_nrcCounts.size(); n++)
          xml()->DropInt32("Count", pDef.m_nrcCounts.getPtr()[n]);
    }
    if (!pDef.m_nrcPoints.isEmpty())
    {
      XML_LEVEL_STATIC(xml(), 1, "NrcPoints");
        for (OdUInt32 n = 0; n < pDef.m_nrcPoints.size(); n += 2)
          xml()->DropFloat2("Point", pDef.m_nrcPoints.getPtr()[n], pDef.m_nrcPoints.getPtr()[n + 1]);
    }
  }
  if (GETBIT(kindOfMod, kViewportModLineweights))
  {
    xml()->DropUInt32("NLineweightEnum", pDef.m_lineweightsEnum.size());
    xml()->DropDouble("LineweightCoef", pDef.m_lineweightsCoef);
    if (!pDef.m_lineweightsEnum.isEmpty())
    {
      XML_LEVEL_STATIC(xml(), 1, "LineweightEnum");
        for (OdUInt32 n = 0; n < pDef.m_lineweightsEnum.size(); n++)
          xml()->DropUInt8("NPixels", pDef.m_lineweightsEnum.getPtr()[n]);
    }
  }
  if (GETBIT(kindOfMod, kViewportModShader))
  {
    if (pDef.m_rmShader != kTrVisNegativeId) // Necessary for clients which support queryProgramId only
      xml()->DropId("ShaderId", pDef.m_rmShader, NULL, OdGLES2XmlServer::kProgramId);
    if (pDef.m_visualStyle != kTrVisNegativeId) {
      xml()->DropId("VisualStyleId", pDef.m_visualStyle, m_idReg.get(kVisualStyleData, pDef.m_visualStyle) );
    } else {
      xml()->DropId("VisualStyleId", kTrVisNegativeId);
    }
    ODA_ASSERT_ONCE(pDef.m_renderMode.controlVersion() == 5);
    xml()->DropBoolean("DepthBuffer", pDef.m_renderMode.depthBufferEnabled());
    xml()->DropBoolean("StencilBuffer", pDef.m_renderMode.stencilBufferEnabled());
    xml()->DropBoolean("PolygonOffset", pDef.m_renderMode.polygonOffsetEnabled());
    xml()->DropBoolean("Lighting", pDef.m_renderMode.lightingEnabled());
    xml()->DropBoolean("FaceNormals", pDef.m_renderMode.faceNormalsEnabled());
    xml()->DropUInt8RGB("FaceFillColor", ODGETRED(pDef.m_faceFillColor), ODGETGREEN(pDef.m_faceFillColor), ODGETBLUE(pDef.m_faceFillColor), 255);
  }
  if (GETBIT(kindOfMod, kViewportModBorder))
  {
    xml()->DropBoolean("Border", pDef.m_bBorder);
    xml()->DropUInt8RGB("BorderColor", ODGETRED(pDef.m_borderColor), ODGETGREEN(pDef.m_borderColor), ODGETBLUE(pDef.m_borderColor), 255);
    xml()->DropInt32("BorderWidth", pDef.m_borderWeight);
    xml()->DropUInt32("NBorderCounts", pDef.m_borderCounts.size());
    xml()->DropUInt32("NBorderPoints", pDef.m_borderPoints.size() / 2);
    if (!pDef.m_borderCounts.isEmpty())
    {
      XML_LEVEL_STATIC(xml(), 1, "BorderCounts");
        for (OdUInt32 n = 0; n < pDef.m_borderCounts.size(); n++)
          xml()->DropInt32("Count", pDef.m_borderCounts.getPtr()[n]);
    }
    if (!pDef.m_borderPoints.isEmpty())
    {
      XML_LEVEL_STATIC(xml(), 1, "BorderPoints");
        for (OdUInt32 n = 0; n < pDef.m_borderPoints.size(); n += 2)
          xml()->DropFloat2("Point", pDef.m_borderPoints.getPtr()[n], pDef.m_borderPoints.getPtr()[n + 1]);
    }
  }
}

void OdGLES2XmlRendition::onMetafileOrderChanged(OdTrVisViewportId viewportId, OdTrVisOverlayId overlayId, OdUInt32 nInvalidFrom, OdUInt32 nInvalidLen,
                                                 const OdTrVisDisplayId *pValidFrom, OdUInt32 nValidLen)
{
  onMetafileOrderChangedImpl(viewportId, overlayId, nInvalidFrom, nInvalidLen, pValidFrom, nValidLen);
}

void OdGLES2XmlRendition::onMetafileOrderInheritance( OdTrVisViewportId viewportId1, OdTrVisOverlayId overlayId1, OdTrVisViewportId viewportId2 )
{
  onMetafileOrderInheritanceImpl( viewportId1, overlayId1, viewportId2 );
}

bool OdGLES2XmlRendition::DropDisplayList(const OdTrVisDisplayId *pDispList, OdUInt32 nListLen, DDL2ndPassInfo *p2ndPass)
{
  for (OdUInt32 n = 0; n < nListLen; n++)
  {
    ODA_ASSERT(pDispList[n] >= kDisplayCodeRange);
    OdTrVisUniqueID *pRegEntry = m_idReg.get(kMetafileData, pDispList[n]);
    if (!p2ndPass || pRegEntry)
      xml()->DropId("MetafileID", pDispList[n], pRegEntry, OdGLES2XmlServer::kDisplayId);
    else
    {
      if (!p2ndPass->m_bScan)
      {
        p2ndPass->m_bScan = true;
        p2ndPass->m_nTransLast = p2ndPass->m_nTransFirst = n;
      }
      else
        p2ndPass->m_nTransLast = n;
      xml()->DropUInt32("MetafileID", 0);
    }
  }
  if (p2ndPass)
    return p2ndPass->m_bScan;
  return false;
}

void OdGLES2XmlRendition::DropDisplayList2ndPass(OdTrVisViewportId viewportId, OdTrVisOverlayId overlayId, OdUInt32 nInvalidFrom,
                                                 const OdTrVisDisplayId *pDispList, OdUInt32 nListLen, DDL2ndPassInfo &p2ndPass)
{
  OdUInt32 nScan = p2ndPass.m_nTransFirst;
  while (nScan <= p2ndPass.m_nTransLast)
  {
    OdUInt32 nScanLast = nScan + 1;
    while (nScanLast <= p2ndPass.m_nTransLast)
    {
      if (m_idReg.get(kMetafileData, pDispList[nScanLast]))
        break;
      nScanLast++;
    }
    onMetafileOrderChangedImpl(viewportId, overlayId, nInvalidFrom + nScan, nScanLast - nScan, pDispList + nScan, nScanLast - nScan, false);
    nScan = nScanLast + 1;
    while (nScan <= p2ndPass.m_nTransLast)
    {
      if (!m_idReg.get(kMetafileData, pDispList[nScan]))
        break;
      nScan++;
    }
  }
}

void OdGLES2XmlRendition::onMetafileOrderChangedImpl(OdTrVisViewportId viewportId, OdTrVisOverlayId overlayId, OdUInt32 nInvalidFrom, OdUInt32 nInvalidLen,
                                                     const OdTrVisDisplayId *pValidFrom, OdUInt32 nValidLen, bool bFirstPass)
{
  OutputState((bFirstPass) ? OdGLES2XmlServer::kShareableClientState : OdGLES2XmlServer::kClientOnlyState);
  DDL2ndPassInfo s2ndPass;
  {
    XML_LEVEL0("MetafileOrderChanged");
      xml()->DropId("ViewportID", viewportId, m_idReg.get(kViewportData, viewportId), OdGLES2XmlServer::kViewportId);
      xml()->DropId("OverlayID", overlayId, m_idReg.get(kOverlayData, overlayId), OdGLES2XmlServer::kOverlayId);
      xml()->DropUInt32("InvalidFrom", nInvalidFrom);
      xml()->DropUInt32("InvalidLen", nInvalidLen);
      xml()->DropUInt32("ValidLen", nValidLen);
      if (nValidLen)
      {
        XML_LEVEL(1, "Ordering");
          DropDisplayList(pValidFrom, nValidLen, (bFirstPass) ? &s2ndPass : NULL);
      }
  }
  if (s2ndPass.m_bScan)
    DropDisplayList2ndPass(viewportId, overlayId, nInvalidFrom, pValidFrom, nValidLen, s2ndPass);
}

void OdGLES2XmlRendition::onMetafileOrderInheritanceImpl( OdTrVisViewportId viewportId1, OdTrVisOverlayId overlayId1, OdTrVisViewportId viewportId2 )
{
  OutputState(OdGLES2XmlServer::kShareableClientState);
  XML_LEVEL0("MetafileOrderInheritance");
    xml()->DropId("ViewportID1", viewportId1, m_idReg.get(kViewportData, viewportId1), OdGLES2XmlServer::kViewportId);
    xml()->DropId("OverlayID", overlayId1, m_idReg.get(kOverlayData, overlayId1), OdGLES2XmlServer::kOverlayId);
    xml()->DropId("ViewportID2", viewportId2, m_idReg.get(kViewportData, viewportId2), OdGLES2XmlServer::kViewportId);
}

void OdGLES2XmlRendition::onLightsListChanged(OdTrVisViewportId viewportId, float ambientLight[4], bool bDefaultLightsOn,
                                              const OdTrVisLightDef *pList, OdUInt32 nEntries)
{
  OutputState(OdGLES2XmlServer::kShareableClientState);
  XML_LEVEL0("LightsListChanged");
    xml()->DropId("ViewportID", viewportId, m_idReg.get(kViewportData, viewportId), OdGLES2XmlServer::kViewportId);
    xml()->DropFloat4("AmbientLight", ambientLight[0], ambientLight[1], ambientLight[2], ambientLight[3]);
    xml()->DropBoolean("DefaultLightsOn", bDefaultLightsOn);
    xml()->DropUInt32("NLights", nEntries);
    if (nEntries)
    {
      XML_LEVEL(1, "LightsList");
        for (OdUInt32 n = 0; n < nEntries; n++)
        {
          XML_LEVEL(2, "Light");
            xml()->DropInt32("Type", (OdInt32)pList[n].m_type);
            xml()->DropBoolean("DefLight", pList[n].m_bDefLight);
            xml()->DropDouble3("Position", pList[n].m_position.x, pList[n].m_position.y, pList[n].m_position.z);
            xml()->DropDouble3("Direction", pList[n].m_direction.x, pList[n].m_direction.y, pList[n].m_direction.z);
            xml()->DropFloat3("Color", pList[n].m_color[0], pList[n].m_color[1], pList[n].m_color[2]);
            xml()->DropFloat("ConstantAttenuation", pList[n].m_constantAttenuation);
            xml()->DropFloat("LinearAttenuation", pList[n].m_linearAttenuation);
            xml()->DropFloat("QuadraticAttenuation", pList[n].m_quadraticAttenuation);
            xml()->DropFloat("SpotCutoff", pList[n].m_spotCutoff);
            xml()->DropFloat("SpotExponent", pList[n].m_spotExponent);
            xml()->DropFloat("SpecularFactor", pList[n].m_specularFactor);
        }
    }
}

void OdGLES2XmlRendition::onBackgroundChanged(OdTrVisViewportId viewportId, OdTrVisFlatMetafileContainerPtr pStream, OdTrVisProgramId baseProgramId)
{
  OutputState(OdGLES2XmlServer::kShareableClientState);
  XML_LEVEL0("BackgroundChanged");
    xml()->DropId("ViewportID", viewportId, m_idReg.get(kViewportData, viewportId), OdGLES2XmlServer::kViewportId);
    if (baseProgramId != kTrVisNegativeId) // Neccessary for clients which support queryProgramId only
      xml()->DropId("BaseProgramID", baseProgramId, NULL, OdGLES2XmlServer::kProgramId);
    xml()->DropBoolean("Visible", !pStream.isNull());
    if (!pStream.isNull())
      DropMetadata(pStream);
}

void OdGLES2XmlRendition::onExtentsChanged(OdTrVisViewportId viewportId, OdTrVisOverlayId overlayId, const OdTrVisExtentsDef &pDef)
{
  OutputState(OdGLES2XmlServer::kShareableClientState);
  XML_REGISTERED(kViewportData, viewportId);

  XML_LEVEL0_STATIC(xml(), "ExtentsChanged");
    xml()->DropId("ViewportID", viewportId, _xmlReg.m_pRegId, OdGLES2XmlServer::kViewportId);
    xml()->DropId("OverlayID", overlayId, m_idReg.get(kOverlayData, overlayId), OdGLES2XmlServer::kOverlayId);
    xml()->DropDouble3("SceneMin", pDef.m_sceneExtents.minPoint().x, pDef.m_sceneExtents.minPoint().y, pDef.m_sceneExtents.minPoint().z);
    xml()->DropDouble3("SceneMax", pDef.m_sceneExtents.maxPoint().x, pDef.m_sceneExtents.maxPoint().y, pDef.m_sceneExtents.maxPoint().z);
    xml()->DropDouble3("RealMin", pDef.m_realExtents.minPoint().x, pDef.m_realExtents.minPoint().y, pDef.m_realExtents.minPoint().z);
    xml()->DropDouble3("RealMax", pDef.m_realExtents.maxPoint().x, pDef.m_realExtents.maxPoint().y, pDef.m_realExtents.maxPoint().z);
}

void OdGLES2XmlRendition::onOverlayViewParamsOverride(OdTrVisViewportId viewportId, OdTrVisOverlayId overlayId, bool bOverride, const OdTrVisViewParamsDef &pDef)
{
  OutputState(OdGLES2XmlServer::kClientDependentState);
  XML_LEVEL0("OverlayViewParamsOverride");
    xml()->DropId("ViewportID", viewportId, m_idReg.get(kViewportData, viewportId), OdGLES2XmlServer::kViewportId);
    xml()->DropId("OverlayID", overlayId, m_idReg.get(kOverlayData, overlayId), OdGLES2XmlServer::kOverlayId);
    xml()->DropBoolean("Override", bOverride);
    if (bOverride)
    {
      XML_LEVEL(1, "ViewParams");
      DropViewParams(xml(), pDef);
    }
}

OdTrVisDisplayId OdGLES2XmlRendition::onMetafileAdded(OdTrVisMetafileId metafileId, const OdTrVisMetafileDef &pDef)
{
  XML_REGISTER(kMetafileData, metafileId);
  // OdAnsiString str = _xmlReg.m_pRegId->m_uId.transform();
  // printf("uId : %s\n", str.c_str());
  OutputState((_xmlReg.m_pRegId) ? OdGLES2XmlServer::kShareableState : OdGLES2XmlServer::kClientOnlyState);
  {
    XML_LEVEL0("MetafileAdded");
    XML_DROP_REG_ID("MetafileID");
    xml()->DropDbStub("OwnerID", pDef.m_pMetafile->m_pOwnerId);
    xml()->DropId("LayerID", pDef.m_pMetafile->m_pLayer, m_idReg.get(kLayerData, pDef.m_pMetafile->m_pLayer), OdGLES2XmlServer::kLayerId);
    xml()->DropId("GeneratorID", pDef.m_generatorId, m_idReg.get(kViewportData, pDef.m_generatorId), OdGLES2XmlServer::kViewportId);
    xml()->DropBooleanOpt("Visibility", pDef.m_pMetafile->isMetafileVisible());
#ifndef OD_GLES2XML_DROPHLTASCLIENTONLY
    xml()->DropBooleanOpt("Highlighted", pDef.m_pMetafile->isMetafileHighlighted());
#else
    xml()->DropBooleanOpt("Highlighted", pDef.m_pMetafile->isMetafileHighlighted() && !_xmlReg.m_pRegId);
#endif
    xml()->DropBooleanOpt("Temporary", pDef.m_pMetafile->isTemporaryMetafile());
    xml()->DropBooleanOpt("Nested", pDef.m_pMetafile->isNestedMetafile());
    xml()->DropBooleanOpt("BlockRef", pDef.m_pMetafile->isBlockRefMetafile());
    xml()->DropBooleanOpt("Utilitary", pDef.m_pMetafile->isUtilitaryMetafile());
    xml()->DropBooleanOpt("Sectionable", pDef.m_pMetafile->isSectionableMetafile());
    xml()->DropBooleanOpt("Composite", pDef.m_pMetafile->isCompositeMetafile());
    xml()->DropBooleanOpt("XRef", pDef.m_pMetafile->isXRefMetafile());
    xml()->DropBooleanOpt("Faded", pDef.m_pMetafile->isMetafileFaded());
    xml()->DropDouble3("ExtMin", pDef.m_extents.minPoint().x, pDef.m_extents.minPoint().y, pDef.m_extents.minPoint().z);
    xml()->DropDouble3("ExtMax", pDef.m_extents.maxPoint().x, pDef.m_extents.maxPoint().y, pDef.m_extents.maxPoint().z);

    if (!xml()->DropMetafileAdded(*this, metafileId, pDef))
      DropMetadata(pDef.m_pMetafile.get());

#ifdef OD_GLES2XML_DROPHLTASCLIENTONLY
    if (_xmlReg.m_pRegId && pDef.m_pMetafile->isMetafileHighlighted())
      onMetafileHighlightingChanged(metafileId, true);
#endif
  }
  return (OdTrVisDisplayId)metafileId;
}

void OdGLES2XmlRendition::onMetafileDeleted(OdTrVisMetafileId metafileId)
{
  XML_UNREGISTER(kMetafileData, metafileId);
  OutputState((_xmlReg.m_pRegId) ? OdGLES2XmlServer::kShareableState : OdGLES2XmlServer::kClientOnlyState);
  XML_LEVEL0("MetafileDeleted");
    XML_DROP_REG_ID("MetafileID");
}

void OdGLES2XmlRendition::onMetafileVisibilityChanged(OdTrVisMetafileId metafileId, bool bVisibility)
{
  XML_REGISTERED(kMetafileData, metafileId);
  OutputState((_xmlReg.m_pRegId) ? OdGLES2XmlServer::kShareableState : OdGLES2XmlServer::kClientOnlyState);
  XML_LEVEL0("MetafileVisibilityChanged");
    XML_DROP_REG_ID("MetafileID");
    xml()->DropBoolean("Visibility", bVisibility);
}

void OdGLES2XmlRendition::onMetafileHighlightingChanged(OdTrVisMetafileId metafileId, bool bHighlight)
{
  XML_REGISTERED(kMetafileData, metafileId);
#ifndef OD_GLES2XML_DROPHLTASCLIENTONLY
  OutputState((_xmlReg.m_pRegId) ? OdGLES2XmlServer::kShareableState : OdGLES2XmlServer::kClientOnlyState);
#else
  OutputState(OdGLES2XmlServer::kClientOnlyState);
#endif
  XML_LEVEL0("MetafileHighlightingChanged");
    XML_DROP_REG_ID("MetafileID");
    xml()->DropBoolean("Highlight", bHighlight);
}

void OdGLES2XmlRendition::onMetafileFadingChanged(OdTrVisMetafileId metafileId, bool bFade)
{
  XML_REGISTERED(kMetafileData, metafileId);
#ifndef OD_GLES2XML_DROPHLTASCLIENTONLY
  OutputState((_xmlReg.m_pRegId) ? OdGLES2XmlServer::kShareableState : OdGLES2XmlServer::kClientOnlyState);
#else
  OutputState(OdGLES2XmlServer::kClientOnlyState);
#endif
  XML_LEVEL0("MetafileFadingChanged");
    XML_DROP_REG_ID("MetafileID");
    xml()->DropBoolean("Fade", bFade);
}

void OdGLES2XmlRendition::onMetafileHlBranchAttached(OdTrVisMetafileId metafileId, OdTrVisHlBranchId hlBranchId)
{
  XML_REGISTERED(kMetafileData, metafileId);
#ifndef OD_GLES2XML_DROPHLTASCLIENTONLY
  OutputState((_xmlReg.m_pRegId) ? OdGLES2XmlServer::kShareableState : OdGLES2XmlServer::kClientOnlyState);
#else
  OutputState(OdGLES2XmlServer::kClientOnlyState);
#endif
  XML_LEVEL0("MetafileHlBranchAttached");
    XML_DROP_REG_ID("MetafileID");
    xml()->DropId("HlBranchID", hlBranchId, m_idReg.get(kHlBranchData, hlBranchId), OdGLES2XmlServer::kHlBranchId);
}

void OdGLES2XmlRendition::onMetafileHlBranchDetached(OdTrVisMetafileId metafileId)
{
  XML_REGISTERED(kMetafileData, metafileId);
#ifndef OD_GLES2XML_DROPHLTASCLIENTONLY
  OutputState((_xmlReg.m_pRegId) ? OdGLES2XmlServer::kShareableState : OdGLES2XmlServer::kClientOnlyState);
#else
  OutputState(OdGLES2XmlServer::kClientOnlyState);
#endif
  XML_LEVEL0("MetafileHlBranchDetached");
    XML_DROP_REG_ID("MetafileID");
}

void OdGLES2XmlRendition::onHlBranchAdded(OdTrVisHlBranchId hlBranchId, const OdTrVisHlBranchDef &pDef)
{
  XML_REGISTER(kHlBranchData, hlBranchId);
#ifndef OD_GLES2XML_DROPHLTASCLIENTONLY
  OutputState((_xmlReg.m_pRegId) ? OdGLES2XmlServer::kShareableState : OdGLES2XmlServer::kClientOnlyState);
#else
  OutputState(OdGLES2XmlServer::kClientOnlyState);
#endif
  XML_LEVEL0("HlBranchAdded");
    XML_DROP_REG_ID("HlBranchID");
    xml()->DropDbStub("OwnerID", pDef.m_pOwner);
    DropHlBranch(pDef, true, true);
}

void OdGLES2XmlRendition::onHlBranchModified(OdTrVisHlBranchId hlBranchId, const OdTrVisHlBranchDef &pDef,
                                             bool bChildsModified, bool bMarkersModified)
{
  XML_REGISTERED(kHlBranchData, hlBranchId);
#ifndef OD_GLES2XML_DROPHLTASCLIENTONLY
  OutputState((_xmlReg.m_pRegId) ? OdGLES2XmlServer::kShareableState : OdGLES2XmlServer::kClientOnlyState);
#else
  OutputState(OdGLES2XmlServer::kClientOnlyState);
#endif
  XML_LEVEL0("HlBranchModified");
    XML_DROP_REG_ID("HlBranchID");
    DropHlBranch(pDef, bChildsModified, bMarkersModified);
}

void OdGLES2XmlRendition::onHlBranchDeleted(OdTrVisHlBranchId hlBranchId)
{
  XML_UNREGISTER(kHlBranchData, hlBranchId);
#ifndef OD_GLES2XML_DROPHLTASCLIENTONLY
  OutputState((_xmlReg.m_pRegId) ? OdGLES2XmlServer::kShareableState : OdGLES2XmlServer::kClientOnlyState);
#else
  OutputState(OdGLES2XmlServer::kClientOnlyState);
#endif
  XML_LEVEL0("HlBranchDeleted");
    XML_DROP_REG_ID("HlBranchID");
}


void OdGLES2XmlRendition::onTextureAdded(OdTrVisTextureId textureId, const OdTrVisTextureDef &pDef)
{
  XML_REGISTER(kTextureData, textureId);
  OutputState((_xmlReg.m_pRegId) ? OdGLES2XmlServer::kShareableClientState : OdGLES2XmlServer::kClientOnlyState);

  XML_LEVEL0_STATIC(xml(), "TextureAdded");
    xml()->DropId("TextureID", textureId, _xmlReg.m_pRegId, OdGLES2XmlServer::kTextureId);
    xml()->DropUInt32("Type", (OdUInt32)pDef.m_pTexture->format());
    xml()->DropBooleanOpt("Smooth", pDef.smoothFilterEnabled());
    xml()->DropBooleanOpt("Modulate", pDef.colorModulationEnabled());
    xml()->DropUInt32("Width", pDef.m_pTexture->getTextureWidth());
    xml()->DropUInt32("Height", pDef.m_pTexture->getTextureHeight());
    xml()->DropUInt32("ScanlineLength", pDef.m_pTexture->getScanlineLength());
    xml()->DropUInt32("DataAlignment", pDef.m_pTexture->getDataAlignment());
    xml()->DropBinaryStream("TextureData", pDef.m_pTexture->textureData(), pDef.m_pTexture->getScanlineLength() * pDef.m_pTexture->getTextureHeight() );
    const OdTrVisTexture* pal = pDef.m_pTexture->palette();
    if( pal )
    {
      xml()->DropUInt32("PaletteType", pal->format() );
      xml()->DropUInt32("PaletteWidth", pal->getTextureWidth());
      xml()->DropUInt32("PaletteScanlineLength", pal->getScanlineLength());
      xml()->DropUInt32("PaletteDataAlignment", pal->getDataAlignment());
      xml()->DropBinaryStream("PaletteData", pal->textureData(), pal->getScanlineLength() * pal->getTextureHeight() );
    }
}

void OdGLES2XmlRendition::onTextureDeleted(OdTrVisTextureId textureId)
{
  XML_UNREGISTER(kTextureData, textureId);
  OutputState((_xmlReg.m_pRegId) ? OdGLES2XmlServer::kShareableClientState : OdGLES2XmlServer::kClientOnlyState);
  XML_LEVEL0("TextureDeleted");
    XML_DROP_REG_ID("TextureID");
}

void OdGLES2XmlRendition::onMaterialAdded(OdTrVisMaterialId materialId, const OdTrVisMaterialDef &pDef)
{
  XML_REGISTER(kMaterialData, materialId);
  OutputState((_xmlReg.m_pRegId) ? OdGLES2XmlServer::kShareableState : OdGLES2XmlServer::kClientOnlyState);
  XML_LEVEL0("MaterialAdded");
    XML_DROP_REG_ID("MaterialID");
    xml()->DropString("MaterialName", pDef.m_name);
    xml()->DropBoolean("IsDefault", pDef.m_bDefault);
    xml()->DropFloat3("AmbientColor", pDef.m_ambientColor[0], pDef.m_ambientColor[1], pDef.m_ambientColor[2]);
    xml()->DropFloat3("ShadingAmbientColor",  pDef.m_shadingAmbientColor[0],  pDef.m_shadingAmbientColor[1],  pDef.m_shadingAmbientColor[2]);
    xml()->DropFloat3("DiffuseColor", pDef.m_diffuseColor[0], pDef.m_diffuseColor[1], pDef.m_diffuseColor[2]);
    xml()->DropFloat3("ShadingDiffuseColor",  pDef.m_shadingDiffuseColor[0],  pDef.m_shadingDiffuseColor[1],  pDef.m_shadingDiffuseColor[2]);
    xml()->DropFloat3("SpecularColor", pDef.m_specularColor[0], pDef.m_specularColor[1], pDef.m_specularColor[2]);
    xml()->DropFloat3("ShadingSpecularColor", pDef.m_shadingSpecularColor[0], pDef.m_shadingSpecularColor[1], pDef.m_shadingSpecularColor[2]);
    xml()->DropFloat3("EmissionColor", pDef.m_emissionColor[0], pDef.m_emissionColor[1], pDef.m_emissionColor[2]);
    xml()->DropFloat3("ShadingEmissionColor", pDef.m_shadingEmissionColor[0], pDef.m_shadingEmissionColor[1], pDef.m_shadingEmissionColor[2]);
    xml()->DropFloat("BlendFactor", pDef.m_blendFactor);
    xml()->DropFloat("ShadingBlendFactor", pDef.m_shadingBlendFactor);
    xml()->DropFloat("OpacityLevel", pDef.m_opacityLevel);
    xml()->DropFloat("ShadingOpacityLevel", pDef.m_shadingOpacityLevel);
    xml()->DropFloat("SpecularPower", pDef.m_specularPower);
    xml()->DropFloat("TextureBlend", pDef.m_textureBlend);
    xml()->DropId("TextureID", pDef.m_textureId, m_idReg.get(kTextureData, pDef.m_textureId), OdGLES2XmlServer::kTextureId);
    xml()->DropUInt32("UWrapType", (OdUInt32)pDef.m_uWrap);
    xml()->DropUInt32("VWrapType", (OdUInt32)pDef.m_vWrap);
}

void OdGLES2XmlRendition::onMaterialDeleted(OdTrVisMaterialId materialId)
{
  XML_UNREGISTER(kMaterialData, materialId);
  OutputState((_xmlReg.m_pRegId) ? OdGLES2XmlServer::kShareableState : OdGLES2XmlServer::kClientOnlyState);
  XML_LEVEL0("MaterialDeleted");
    XML_DROP_REG_ID("MaterialID");
}

void OdGLES2XmlRendition::onVisualStyleAdded(OdTrVisVisualStyleId visualStyleId, const OdTrVisVisualStyle &pDef)
{
  XML_REGISTER(kVisualStyleData, visualStyleId);
  OutputState((_xmlReg.m_pRegId) ? OdGLES2XmlServer::kShareableState : OdGLES2XmlServer::kClientOnlyState);
  XML_LEVEL0("VisualStyleAdded");
    XML_DROP_REG_ID("VisualStyleID");
    DropVisualStyleData(pDef);
}

void OdGLES2XmlRendition::onVisualStyleModified(OdTrVisVisualStyleId visualStyleId, const OdTrVisVisualStyle &pDef)
{
  XML_REGISTERED(kVisualStyleData, visualStyleId);
  OutputState((_xmlReg.m_pRegId) ? OdGLES2XmlServer::kShareableState : OdGLES2XmlServer::kClientOnlyState);
  XML_LEVEL0("VisualStyleModified");
    XML_DROP_REG_ID("VisualStyleID");
    DropVisualStyleData(pDef, false);
}

void OdGLES2XmlRendition::onVisualStyleDeleted(OdTrVisVisualStyleId visualStyleId)
{
  XML_UNREGISTER(kVisualStyleData, visualStyleId);
  OutputState((_xmlReg.m_pRegId) ? OdGLES2XmlServer::kShareableState : OdGLES2XmlServer::kClientOnlyState);
  XML_LEVEL0("VisualStyleDeleted");
    XML_DROP_REG_ID("VisualStyleID");
}

void OdGLES2XmlRendition::onLayerAdded(OdTrVisLayerId layerId, const OdTrVisLayerDef &pDef)
{
  XML_REGISTER(kLayerData, layerId);
  OutputState((_xmlReg.m_pRegId) ? OdGLES2XmlServer::kShareableState : OdGLES2XmlServer::kClientOnlyState);
  XML_LEVEL0("LayerAdded");
    XML_DROP_REG_ID("LayerID");
    DropLayerData(pDef);
}

void OdGLES2XmlRendition::onLayerModified(OdTrVisLayerId layerId, const OdTrVisLayerDef &pDef)
{
  XML_REGISTERED(kLayerData, layerId);
  OutputState((_xmlReg.m_pRegId) ? OdGLES2XmlServer::kShareableState : OdGLES2XmlServer::kClientOnlyState);
  XML_LEVEL0("LayerModified");
    XML_DROP_REG_ID("LayerID");
    DropLayerData(pDef, false);
}

void OdGLES2XmlRendition::onLayerDeleted(OdTrVisLayerId layerId)
{
  XML_UNREGISTER(kLayerData, layerId);
  OutputState((_xmlReg.m_pRegId) ? OdGLES2XmlServer::kShareableState : OdGLES2XmlServer::kClientOnlyState);
  XML_LEVEL0("LayerDeleted");
    XML_DROP_REG_ID("LayerID");
}

void OdGLES2XmlRendition::onLayerViewportPropsOverride(OdTrVisViewportId viewportId, OdTrVisLayerId layerId, bool bOverride, const OdTrVisLayerProps &pProps )
{
  XML_REGISTERED(kLayerData, layerId);
  OutputState((_xmlReg.m_pRegId) ? OdGLES2XmlServer::kShareableState : OdGLES2XmlServer::kClientOnlyState);
  XML_LEVEL0("LayerViewportPropsOverride");
    xml()->DropId( "ViewportID", viewportId, m_idReg.get(kViewportData, viewportId), OdGLES2XmlServer::kViewportId );
    xml()->DropId( "LayerID", layerId, m_idReg.get(kLayerData, layerId), OdGLES2XmlServer::kLayerId );
    xml()->DropBoolean("Override", bOverride);
    if( bOverride )
    {
      XML_LEVEL(1, "VpDepSpec");
      DropLayerProps( pProps );
    }
}


void OdGLES2XmlRendition::DropHlBranch(const OdTrVisHlBranchDef &pHlBranch, bool bDropChilds, bool bDropMarkers)
{
  if (bDropChilds)
  {
    OdUInt32 nChilds = pHlBranch.m_childs.size();
    const OdTrVisHlBranchId *pChilds = pHlBranch.m_childs.getPtr();
    xml()->DropUInt32("NChilds", nChilds);
    if (bDropMarkers)
      xml()->DropUInt32("NMarkers", pHlBranch.m_markers.size());
    if (nChilds)
    { XML_LEVEL(1, "Childs");
      for (OdUInt32 n = 0; n < nChilds; n++)
      {
        OdTrVisHlBranchId hlBranchId = pChilds[n];
        xml()->DropId("HlBranchID", hlBranchId, m_idReg.get(kHlBranchData, hlBranchId), OdGLES2XmlServer::kHlBranchId);
      }
    }
  }
  if (bDropMarkers)
  {
    OdUInt32 nMarkers = pHlBranch.m_markers.size();
    const OdTrVisGsMarker *pMarkers = pHlBranch.m_markers.getPtr();
    if (!bDropChilds)
      xml()->DropUInt32("NMarkers", nMarkers);
    if (nMarkers)
    {
      XML_LEVEL(1, "Markers");
      for (OdUInt32 n = 0; n < nMarkers; n++)
        xml()->DropUInt64("Marker", (OdUInt64)pMarkers[n]);
    }
  }
}

void OdGLES2XmlRendition::DropVisualStyleData(const OdTrVisVisualStyle &pVStyle, bool bDropNonModified)
{
  xml()->DropUInt32("Type", (OdUInt32)pVStyle.type());
  xml()->DropString("Name", pVStyle.name());
  xml()->DropBoolean("Internal", pVStyle.isInternal());
  xml()->DropBoolean("Temporary", pVStyle.isTemporary());
  const OdUInt32 nProps = pVStyle.numProperties();
  xml()->DropUInt32("NProps", nProps);
  if (bDropNonModified) xml()->DropUInt32("NPropsModified", nProps);
  else xml()->DropUInt32("NPropsModified", pVStyle.countModified());
  for (OdUInt32 nProp = 0; nProp < nProps; nProp++)
  {
    const OdTrVisVisualStyleProperty &prop = pVStyle.property((OdGiVisualStyleProperties::Property)nProp);
    if (bDropNonModified || prop.isModified())
    {
      XML_LEVEL(1, "Prop");
        xml()->DropUInt32("NProp", prop.propertyNumber());
        xml()->DropUInt32("PropType", (OdUInt32)prop.type());
        xml()->DropBoolean("Set", prop.operation());
        switch (prop.type())
        {
          case OdGiVariant::kBoolean: xml()->DropBoolean("BVal", prop.getBool()); break;
          case OdGiVariant::kInt: xml()->DropInt32("IVal", prop.getInt()); break;
          case OdGiVariant::kDouble: xml()->DropDouble("DVal", prop.getDouble()); break;
          case OdGiVariant::kColor: xml()->DropUInt32("CVal", prop.getUInt()); break;
          case OdGiVariant::kString: xml()->DropString("SVal", OdString(prop.getString())); break;
          default: ODA_FAIL();
        }
    }
  }
}

void OdGLES2XmlRendition::DropLayerData(const OdTrVisLayerDef &pLayer, bool bDropOwnerId)
{
  { // Name section
    xml()->DropString("Name", pLayer.m_name);
    if (bDropOwnerId)
      xml()->DropDbStub("OwnerID", pLayer.m_pOwner);
  }
  { // Props section
    DropLayerProps(pLayer.m_props);
  }
}

void OdGLES2XmlRendition::DropLayerProps(const OdTrVisLayerProps &pLayerProps)
{
  xml()->DropUInt16("Props", pLayerProps.m_flags);
}

void OdGLES2XmlRendition::DropMetadata(const OdTrVisFlatMetafileContainer *pMetafile)
{
  XML_LEVEL(1, "MetafileData");
    xml()->DropUInt32("NArrays", pMetafile->m_ArrayElements.size());
    for (OdUInt32 n = 0; n < pMetafile->m_ArrayElements.size(); n++)
    {
      const OdTrVisArrayWrapper &array = pMetafile->m_ArrayElements.getPtr()[n];
      XML_LEVEL(2, "Array");
        xml()->DropUInt32("Type", (OdUInt32)array.m_type);
        switch (array.m_type)
        {
          case OdTrVisArrayWrapper::Type_Vertex:
          case OdTrVisArrayWrapper::Type_Normal:
            {
              const float *pData = (const float*)array.m_pData;
              OdUInt32 nData = array.m_uSize / sizeof(float);
              xml()->DropUInt32("NData", nData / 3);
              xml()->DropFloats("ArrayData", nData, pData);
            }
          break;
          case OdTrVisArrayWrapper::Type_Color:
            {
              const float *pData = (const float*)array.m_pData;
              OdUInt32 nData = array.m_uSize / sizeof(float);
              xml()->DropUInt32("NData", nData / 4);
              xml()->DropFloats("ArrayData", nData, pData);
            }
          break;
          case OdTrVisArrayWrapper::Type_TexCoord:
            {
              const float *pData = (const float*)array.m_pData;
              OdUInt32 nData = array.m_uSize / sizeof(float);
              xml()->DropUInt32("NData", nData / 2);
              xml()->DropFloats("ArrayData", nData, pData);
            }
          break;
          case OdTrVisArrayWrapper::Type_Depth:
            {
              const float *pData = (const float*)array.m_pData;
              OdUInt32 nData = array.m_uSize / sizeof(float);
              xml()->DropUInt32("NData", nData);
              xml()->DropFloats("ArrayData", nData, pData);
            }
          break;
          case OdTrVisArrayWrapper::Type_Index:
            {
              const OdUInt16 *pData = (const OdUInt16*)array.m_pData;
              OdUInt32 nData = array.m_uSize / sizeof(OdUInt16);
              xml()->DropUInt32("NData", nData);
              xml()->DropInts("ArrayData", nData, pData);
            }
          break;
          case OdTrVisArrayWrapper::Type_Marker:
            {
              const OdTrVisDefProcMark *pData = (const OdTrVisDefProcMark*)array.m_pData;
              OdUInt32 nData = array.m_uSize / sizeof(OdTrVisDefProcMark);
              xml()->DropUInt32("NData", nData);
              { XML_LEVEL(3, "Data");
                for (OdUInt32 n = 0; n < nData; n++)
                {
                  XML_LEVEL(4, "Entry");
                    xml()->DropUInt64("Marker", pData[n].m_nMark);
                    xml()->DropUInt32("From",   pData[n].m_uFrom);
                }
              }
            }
          break;
          default: ODA_FAIL();
        }
    }
    ParseMetafileStream(pMetafile);
}

void OdGLES2XmlRendition::ParseMetafileStream(const OdTrVisFlatMetafileContainer *pMetafile)
{
  XML_LEVEL(2, "MetafileStream");
  OdUInt32 uSize = pMetafile->size();
  if (uSize == 0) return;

  OdUInt32 currArrayNum = 0;
  OdUInt8 currArrayType = 0;

  const OdUInt8 *pMemPtr = pMetafile->memoryPtr();
  const OdUInt8 *pMemPtrReadFor = pMemPtr + uSize;
  while (pMemPtr < pMemPtrReadFor)
  {
    OdTrVisMetaRecType recType = OD_OGL_RDR_READVALRETYPE(OdTrVisMetaRecType, OdUInt8, pMemPtr);
    OD_OGL_RDR_SEEKINC(pMemPtr);
    switch (recType)
    {
      case OdTrVisMetaRecType_EnableOpt:
        {
          XML_LEVEL(3, "EnableOpt");
            xml()->DropUInt8("Mode", OD_OGL_RDR_READVAL(OdUInt8, pMemPtr));
            OD_OGL_RDR_SEEKINC(pMemPtr);
        }
      break;
      case OdTrVisMetaRecType_DisableOpt:
        {
          XML_LEVEL(3, "DisableOpt");
            xml()->DropUInt8("Mode", OD_OGL_RDR_READVAL(OdUInt8, pMemPtr));
            OD_OGL_RDR_SEEKINC(pMemPtr);
        }
      break;
      case OdTrVisMetaRecType_Color:
        {
          XML_LEVEL(3, "Color");
            xml()->DropUInt8RGB("Color", pMemPtr[0], pMemPtr[1], pMemPtr[2], pMemPtr[3]);
            OD_OGL_RDR_SEEKFWD(pMemPtr, 4);
        }
      break;
      case OdTrVisMetaRecType_EnableArray:
        {
          XML_LEVEL(3, "EnableArray");
            xml()->DropUInt8("Type", OD_OGL_RDR_READVAL(OdUInt8, pMemPtr));
            OD_OGL_RDR_SEEKINC(pMemPtr);
            xml()->DropUInt32("NArray", OD_OGL_RDR_READVAL(OdUInt32, pMemPtr));
            OD_OGL_RDR_SEEKFWD(pMemPtr, sizeof(OdUInt32));
        }
      break;
      case OdTrVisMetaRecType_DisableArray:
        {
          XML_LEVEL(3, "DisableArray");
            xml()->DropUInt8("Type", OD_OGL_RDR_READVAL(OdUInt8, pMemPtr));
            OD_OGL_RDR_SEEKINC(pMemPtr);
        }
      break;
      case OdTrVisMetaRecType_DrawArrays:
        {
          XML_LEVEL(3, "DrawArrays");
            xml()->DropUInt8("Mode", OD_OGL_RDR_READVAL(OdUInt8, pMemPtr));
            xml()->DropInt32("First", OD_OGL_RDR_READVALOFFSET(OdInt32, pMemPtr, 1));
            xml()->DropInt32("Count", OD_OGL_RDR_READVALOFFSET(OdInt32, pMemPtr, 1 + sizeof(OdInt32)));
            OD_OGL_RDR_SEEKFWD(pMemPtr, (sizeof(OdInt32) << 1) + 1);
        }
      break;
      case OdTrVisMetaRecType_DrawElements:
        {
          XML_LEVEL(3, "DrawElements");
            xml()->DropUInt8("Mode", OD_OGL_RDR_READVAL(OdUInt8, pMemPtr));
            xml()->DropInt32("Count", OD_OGL_RDR_READVALOFFSET(OdInt32, pMemPtr, 1));
            xml()->DropUInt32("NArray", OD_OGL_RDR_READVALOFFSET(OdUInt32, pMemPtr, 1 + sizeof(OdInt32)));
            OD_OGL_RDR_SEEKFWD(pMemPtr, sizeof(OdInt32) + 1 + sizeof(OdUInt32));
        }
      break;
      case OdTrVisMetaRecType_CullFace:
        {
          XML_LEVEL(3, "CullFace");
            xml()->DropUInt8("Mode", OD_OGL_RDR_READVAL(OdUInt8, pMemPtr));
            OD_OGL_RDR_SEEKINC(pMemPtr);
        }
      break;
      case OdTrVisMetaRecType_LStipple:
        {
          XML_LEVEL(3, "LStipple");
            xml()->DropUInt8("Pattern", OD_OGL_RDR_READVAL(OdUInt8, pMemPtr));
            OD_OGL_RDR_SEEKINC(pMemPtr);
        }
      break;
      case OdTrVisMetaRecType_PStipple:
        {
          XML_LEVEL(3, "PStipple");
            xml()->DropUInt8("Pattern", OD_OGL_RDR_READVAL(OdUInt8, pMemPtr));
            OD_OGL_RDR_SEEKINC(pMemPtr);
        }
      break;
      case OdTrVisMetaRecType_VPoint:
        {
          XML_LEVEL(3, "VPoint");
            const float *pFloats = (const float*)pMemPtr;
            xml()->DropFloat3("Point", pFloats[0], pFloats[1], pFloats[2]);
        }
        OD_OGL_RDR_SEEKFWD(pMemPtr, (sizeof(float) << 1) + sizeof(float));
      break;
      case OdTrVisMetaRecType_VLine:
        {
          XML_LEVEL(3, "VLine");
            const float *pFloats = (const float*)pMemPtr;
            xml()->DropFloat3("Point1", pFloats[0], pFloats[1], pFloats[2]);
            xml()->DropFloat3("Point2", pFloats[3], pFloats[4], pFloats[5]);
        }
        OD_OGL_RDR_SEEKFWD(pMemPtr, (sizeof(float) << 2) + (sizeof(float) << 1));
      break;
      case OdTrVisMetaRecType_IPoint:
        {
          XML_LEVEL(3, "IPoint");
            xml()->DropInt32("Index", OD_OGL_RDR_READVAL(OdInt32, pMemPtr));
        }
        OD_OGL_RDR_SEEKFWD(pMemPtr, sizeof(OdInt32));
      break;
      case OdTrVisMetaRecType_ILine:
        {
          XML_LEVEL(3, "ILine");
            xml()->DropInt32("Index1", OD_OGL_RDR_READVAL(OdInt32, pMemPtr));
            xml()->DropInt32("Index2", OD_OGL_RDR_READVALOFFSET(OdInt32, pMemPtr, sizeof(OdInt32)));
        }
        OD_OGL_RDR_SEEKFWD(pMemPtr, sizeof(OdInt32) * 2);
      break;
      case OdTrVisMetaRecType_HLRStencil:
        {
          XML_LEVEL(3, "HLRStencil");
            xml()->DropUInt8("State", OD_OGL_RDR_READVAL(OdUInt8, pMemPtr));
            OD_OGL_RDR_SEEKINC(pMemPtr);
        }
      break;
      case OdTrVisMetaRecType_EnableShading:
        {
          XML_LEVEL(3, "EnableShading");
            xml()->DropUInt8("Mode", OD_OGL_RDR_READVAL(OdUInt8, pMemPtr));
            OD_OGL_RDR_SEEKINC(pMemPtr);
        }
      break;
      case OdTrVisMetaRecType_DisableShading:
        {
          XML_LEVEL(3, "DisableShading");
            xml()->DropUInt8("Mode", OD_OGL_RDR_READVAL(OdUInt8, pMemPtr));
            OD_OGL_RDR_SEEKINC(pMemPtr);
        }
      break;
      case OdTrVisMetaRecType_Material:
        {
          XML_LEVEL(3, "Material");
            xml()->DropId("MaterialID", OD_OGL_RDR_READVAL(OdUInt64, pMemPtr),
              m_idReg.get(kMaterialData, OD_OGL_RDR_READVALRETYPE(OdTrVisId, OdUInt64, pMemPtr)), OdGLES2XmlServer::kMaterialId);
        }
        OD_OGL_RDR_SEEKFWD(pMemPtr, 8);
      break;
      case OdTrVisMetaRecType_UserEntry:
        {
          XML_LEVEL(3, "UserEntry");
            xml()->DropUInt32("NumData", OD_OGL_RDR_READVAL(OdUInt32, pMemPtr));
            xml()->DropBinaryStream("Data", OD_OGL_RDR_GETPTRNATIVEOFFSET(const OdUInt8, pMemPtr, sizeof(OdUInt32)),
                                            OD_OGL_RDR_READVAL(OdUInt32, pMemPtr));
        }
        OD_OGL_RDR_SEEKFWD(pMemPtr, sizeof(OdUInt32) + OD_OGL_RDR_READVAL(OdUInt32, pMemPtr));
      break;
      case OdTrVisMetaRecType_InitTexture:
        {
          XML_LEVEL(3, "InitTexture");
            xml()->DropId("TextureID", OD_OGL_RDR_READVAL(OdUInt64, pMemPtr),
              m_idReg.get(kTextureData, OD_OGL_RDR_READVALRETYPE(OdTrVisId, OdUInt64, pMemPtr)), OdGLES2XmlServer::kTextureId);
            xml()->DropUInt8("Repeat", OD_OGL_RDR_READVALOFFSET(OdUInt8, pMemPtr, 8));
        }
        OD_OGL_RDR_SEEKFWD(pMemPtr, 8 + 1);
      break;
      case OdTrVisMetaRecType_UninitTexture: // Haven't data
        {
          XML_LEVEL(3, "UninitTexture");
          // No data in this entry
        }
      break;
      case OdTrVisMetaRecType_SelectionMarker:
        {
          XML_LEVEL(3, "SelectionMarker");
            xml()->DropUInt64("Marker", OD_OGL_RDR_READVALRETYPE(OdUInt64, OdTrVisGsMarker, pMemPtr));
        }
        OD_OGL_RDR_SEEKFWD(pMemPtr, sizeof(OdTrVisGsMarker));
      break;
      case OdTrVisMetaRecType_EnableMarkerArray:
        {
          XML_LEVEL(3, "EnableMarkerArray");
            xml()->DropUInt8("Type", OD_OGL_RDR_READVAL(OdUInt8, pMemPtr));
            OD_OGL_RDR_SEEKINC(pMemPtr);
            xml()->DropUInt32("NArray", OD_OGL_RDR_READVAL(OdUInt32, pMemPtr));
            OD_OGL_RDR_SEEKFWD(pMemPtr, sizeof(OdUInt32));
        }
      break;
      case OdTrVisMetaRecType_DisableMarkerArray:
        {
          XML_LEVEL(3, "DisableMarkerArray");
            xml()->DropUInt8("Type", OD_OGL_RDR_READVAL(OdUInt8, pMemPtr));
            OD_OGL_RDR_SEEKINC(pMemPtr);
        }
      break;
      case OdTrVisMetaRecType_SelectionFlags:
        {
          XML_LEVEL(3, "SelectionFlags");
            xml()->DropUInt8("Flags", OD_OGL_RDR_READVAL(OdUInt8, pMemPtr));
            OD_OGL_RDR_SEEKINC(pMemPtr);
        }
      break;
      case OdTrVisMetaRecType_Lineweight:
        {
          XML_LEVEL(3, "Lineweight");
            const OdUInt8 lwdType = OD_OGL_RDR_READVAL(OdUInt8, pMemPtr);
            xml()->DropUInt8("Type", lwdType);
            if (OdTrVisLwdSetting::isPs((OdTrVisLwdSetting::LwdType)lwdType))
            {
              xml()->DropDouble("Ps", OD_OGL_RDR_READVALOFFSET(double, pMemPtr, sizeof(OdUInt8)));
              OD_OGL_RDR_SEEKFWD(pMemPtr, sizeof(OdUInt8) + sizeof(double));
            }
            else
            {
              xml()->DropInt16("Lw", OD_OGL_RDR_READVALOFFSET(OdInt16, pMemPtr, sizeof(OdUInt8)));
              OD_OGL_RDR_SEEKFWD(pMemPtr, sizeof(OdUInt8) + sizeof(OdInt16));
            }
        }
      break;
      case OdTrVisMetaRecType_Linestyle:
        {
          XML_LEVEL(3, "Linestyle");
            const bool defStyle = (OD_OGL_RDR_READVAL(OdUInt8, pMemPtr) != 0);
            xml()->DropBoolean("Default", defStyle);
            if (!defStyle)
            {
              xml()->DropUInt8("CapsType", OD_OGL_RDR_READVALOFFSET(OdUInt8, pMemPtr, sizeof(OdUInt8)));
              xml()->DropUInt8("JoinType", OD_OGL_RDR_READVALOFFSET(OdUInt8, pMemPtr, sizeof(OdUInt8) * 2));
              OD_OGL_RDR_SEEKFWD(pMemPtr, sizeof(OdUInt8) * 3);
            }
            else
              OD_OGL_RDR_SEEKINC(pMemPtr);
        }
      break;
      case OdTrVisMetaRecType_Program:
        {
          XML_LEVEL(3, "Program");
            xml()->DropId("ProgramID", OD_OGL_RDR_READVAL(OdUInt64, pMemPtr), NULL, OdGLES2XmlServer::kProgramId);
        }
        OD_OGL_RDR_SEEKFWD(pMemPtr, 8);
      break;
      case OdTrVisMetaRecType_TtfText:
        {
          XML_LEVEL(3, "TtfText");
            const OdTrVisTtfFontsCacheClient *pCache = processingDevice()->ttfFontsCache();
            if (pCache)
            { // In case if real vectorization device is connected
              OdUInt64 fontKeyA = OD_OGL_RDR_READVAL(OdUInt64, pMemPtr);
              OdUInt64 fontKeyB = OD_OGL_RDR_READVALOFFSET(OdUInt64, pMemPtr, sizeof(OdUInt64));
              const OdGeMatrix3d *pMtx = (const OdGeMatrix3d*)(pMemPtr + sizeof(OdUInt64) * 2);
              const OdGeVector3d *pVec = (const OdGeVector3d*)(pMemPtr + sizeof(OdUInt64) * 2 + sizeof(OdGeMatrix3d));
              OdUInt32 nText = OD_OGL_RDR_READVALOFFSET(OdUInt32, pMemPtr, sizeof(OdUInt64) * 2 + sizeof(OdGeMatrix3d) + sizeof(OdGeVector3d));
              const OdUInt32 *pText = (const OdUInt32*)(pMemPtr + sizeof(OdUInt64) * 2 + sizeof(OdGeMatrix3d) + sizeof(OdGeVector3d) + sizeof(OdUInt32));
              OdTrVisTtfFontsCacheClient::TtfFont pFont = pCache->getFontCache(fontKeyA, fontKeyB);
              if (pFont && nText)
              {
                xml()->DropMatrix("Transform", *pMtx);
                xml()->DropDouble3("Direction", pVec->x, pVec->y, pVec->z);
                xml()->DropUInt32("NText", nText);
                { XML_LEVEL(4, "Text");
                  for (OdUInt32 n = 0; n < nText; n++)
                  {
                    double sideMult;
                    OdTrVisDisplayId mfId = pCache->getCharMetafile(pFont, pText[n], &sideMult);
                    XML_LEVEL(5, "Char");
                      xml()->DropId("MetafileID", mfId, m_idReg.get(kMetafileData, mfId), OdGLES2XmlServer::kDisplayId);
                      xml()->DropDouble("Multiplier", sideMult);
                  }
                }
              }
              else
                xml()->DropUInt32("NText", 0);
              OD_OGL_RDR_SEEKFWD(pMemPtr, sizeof(OdUInt64) * 2 + sizeof(OdGeMatrix3d) + sizeof(OdGeVector3d) + sizeof(OdUInt32) * (nText + 1));
            }
            else
            { // In case if there is no connection to real vectorization device
              const OdGeMatrix3d *pMtx = (const OdGeMatrix3d*)pMemPtr;
              const OdGeVector3d *pVec = (const OdGeVector3d*)(pMemPtr + sizeof(OdGeMatrix3d));
              OdUInt32 nText = OD_OGL_RDR_READVALOFFSET(OdUInt32, pMemPtr, sizeof(OdGeMatrix3d) + sizeof(OdGeVector3d));
              OD_OGL_RDR_SEEKFWD(pMemPtr, sizeof(OdGeMatrix3d) + sizeof(OdGeVector3d) + sizeof(OdUInt32));
              if (nText)
              {
                xml()->DropUInt32("NText", nText);
                xml()->DropMatrix("Transform", *pMtx);
                xml()->DropDouble3("Direction", pVec->x, pVec->y, pVec->z);
                OdTrVisMetafileId mfId; double sideMult;
                while (nText)
                {
                  mfId = OD_OGL_RDR_READVAL(OdTrVisMetafileId, pMemPtr);
                  sideMult = OD_OGL_RDR_READVALOFFSET(double, pMemPtr, sizeof(OdTrVisMetafileId));
                  XML_LEVEL(5, "Char");
                    xml()->DropId("MetafileID", mfId, m_idReg.get(kMetafileData, mfId), OdGLES2XmlServer::kDisplayId);
                    xml()->DropDouble("Multiplier", sideMult);
                  nText--;
                  OD_OGL_RDR_SEEKFWD(pMemPtr, sizeof(OdTrVisMetafileId) + sizeof(double));
                }
              }
              else
                xml()->DropUInt32("NText", 0);
            }
        }
      break;
      case OdTrVisMetaRecType_PushMatrix:
        {
          XML_LEVEL(3, "PushMatrix");
          bool bIdentity = OD_OGL_RDR_READVALRETYPE(OdTrVisPushMatrixType, OdUInt8, pMemPtr) == OdTrVisPushMatrixType_Identity;
          xml()->DropBooleanOpt("Identity", bIdentity);
          OD_OGL_RDR_SEEKINC(pMemPtr);
          if (!bIdentity)
          {
            xml()->DropMatrix("Matrix", *OD_OGL_RDR_GETPTRNATIVE(const OdGeMatrix3d, pMemPtr));
            OD_OGL_RDR_SEEKFWD(pMemPtr, sizeof(OdGeMatrix3d));
          }
        }
      break;
      case OdTrVisMetaRecType_PopMatrix:
        {
          XML_LEVEL(3, "PopMatrix");
          // No data in this entry
        }
      break;
      case OdTrVisMetaRecType_Metafile:
        {
          XML_LEVEL(3, "Metafile");
            xml()->DropId("MetafileID", OD_OGL_RDR_READVAL(OdUInt64, pMemPtr),
              m_idReg.get(kMetafileData, OD_OGL_RDR_READVALRETYPE(OdTrVisId, OdUInt64, pMemPtr)), OdGLES2XmlServer::kDisplayId);
          OD_OGL_RDR_SEEKFWD(pMemPtr, sizeof(OdTrVisMetafileId));
        }
      break;
      case OdTrVisMetaRecType_GeomMarker:
        {
          XML_LEVEL(3, "GeomMarker");
            xml()->DropUInt8("Type", OD_OGL_RDR_READVAL(OdUInt8, pMemPtr));
          OD_OGL_RDR_SEEKINC(pMemPtr);
        }
      break;
      case OdTrVisMetaRecType_VisualStyle:
        {
          XML_LEVEL(3, "VisualStyle");
            xml()->DropId("VisualStyleID", OD_OGL_RDR_READVAL(OdUInt64, pMemPtr),
              m_idReg.get(kVisualStyleData, OD_OGL_RDR_READVALRETYPE(OdTrVisId, OdUInt64, pMemPtr)), OdGLES2XmlServer::kVisualStyleId);
          OD_OGL_RDR_SEEKFWD(pMemPtr, sizeof(OdTrVisVisualStyleId));
        }
      break;
      case OdTrVisMetaRecType_MetafileMarker:
        {
          XML_LEVEL(3, "MetafileMarker");
            xml()->DropId("Marker", OD_OGL_RDR_READVAL(OdUInt64, pMemPtr),
              m_idReg.get(kMetafileData, OD_OGL_RDR_READVALRETYPE(OdTrVisId, OdUInt64, pMemPtr)), OdGLES2XmlServer::kDisplayId);
            OD_OGL_RDR_SEEKFWD(pMemPtr, sizeof(OdTrVisMetafileId));
        }
      break;
      case OdTrVisMetaRecType_Empty:
        // Skip
      break;
      default:
        ODA_FAIL();
    }
  }
}

void OdGLES2XmlRendition::OutputState(OdGLES2XmlServer::OutputState newState, bool bForce)
{
  if ((newState != m_outputState) || bForce)
  {
    m_outputState = newState;
    xml()->OnStateChanged(newState);
  }
}

//
