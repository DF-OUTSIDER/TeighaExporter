/////////////////////////////////////////////////////////////////////////////// 
// Copyright (C) 2002-2017, Open Design Alliance (the "Alliance"). 
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
//   Teigha(R) Copyright (C) 2002-2017 by Open Design Alliance. 
//   All rights reserved.
//
// By use of this software, its documentation or related materials, you 
// acknowledge and accept the above terms.
///////////////////////////////////////////////////////////////////////////////
// GLES2 device json server rendition

#include "OdaCommon.h"
#include "Tr/TrVisRenderClient.h"
#include "ExGsGLES2JsonSharingProvider.h"
#include "ExGsGLES2JsonRendition.h"
#include "JsonMetafileConverter.h"

#define OD_GLES2JSON_DROPHLTASCLIENTONLY

#define STL_USING_LIMITS
#include "OdaSTL.h"

#ifdef OD_GLES2JSON_DROPSCD_TEST
#include "ExGsGLES2ShareableClientData.h"

struct DropSCDTest : public OdGLES2JsonServerStub
{
  OdGLES2JsonServer **m_ppWhere;
  OdGLES2JsonServer  *m_pWho;
  static OdTrVecDevice *m_pDevice;

  DropSCDTest(OdGLES2JsonServer **ppWhere)
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

OdGLES2JsonRendition::OdGLES2JsonRendition()
  : m_pJson()
  , m_pJsonObj()
  , m_mode(kIntermediate)
  , m_pRedir(NULL)
  , m_outputState(OdGLES2JsonServer::kIntermediateState)
  , m_pProcDevice(NULL)
{
  m_overallViewport.setDefault();
  m_pJsonObj = new JsonObjectFormat(m_pJson);
}

OdGLES2JsonRendition::~OdGLES2JsonRendition()
{
  finalizeJsonServerUsage();
}

OdIntPtr OdGLES2JsonRendition::getClientSettings() const
{
  return (OdIntPtr)&m_clientSettings;
}

void OdGLES2JsonRendition::setClientSettings(OdIntPtr pClSet)
{
  m_clientSettings = *(ClientSettings*)pClSet;
}

OdIntPtr OdGLES2JsonRendition::getJsonServer() const
{
  return (OdIntPtr)m_pJson.get();
}

void OdGLES2JsonRendition::setJsonServer(OdIntPtr pJsonServer)
{
  if (m_pJson.get() != (OdGLES2JsonServer*)pJsonServer)
    finalizeJsonServerUsage();
  m_pJson = (OdGLES2JsonServer*)pJsonServer;
  m_pJsonObj->SetJsonServer(m_pJson);
}

OdGLES2JsonServer *OdGLES2JsonRendition::json()
{
  if (m_pJson.isNull()) throw OdError(eNullPtr);
  return m_pJson;
}

const OdGLES2JsonServer *OdGLES2JsonRendition::json() const
{
  if (m_pJson.isNull()) throw OdError(eNullPtr);
  return m_pJson;
}

JsonObjectFormat *OdGLES2JsonRendition::json_obj()
{
  if (m_pJsonObj.isNull()) throw OdError(eNullPtr);
  return m_pJsonObj;
}

const JsonObjectFormat *OdGLES2JsonRendition::json_obj() const
{
  if (m_pJsonObj.isNull()) throw OdError(eNullPtr);
  return m_pJsonObj;
}

void OdGLES2JsonRendition::setIntermediateMode()
{
  m_mode = kIntermediate;
  m_pRedir->setForRecord();
}

void OdGLES2JsonRendition::setGsUpdateMode()
{
  m_mode = kGsUpdate;
  m_pRedir->setForPlay();
}

void OdGLES2JsonRendition::beginDeviceProcessing(OdTrVisRenderClient *pDevice)
{
  ODA_ASSERT(!m_pProcDevice);
  m_pProcDevice = pDevice;
}

void OdGLES2JsonRendition::endDeviceProcessing(OdTrVisRenderClient *
#ifdef ODA_DIAGNOSTICS
  pDevice
#endif
  )
{
  ODA_ASSERT(m_pProcDevice == pDevice);
  m_pProcDevice = NULL;
}

OdTrVisRenderClient *OdGLES2JsonRendition::processingDevice() const
{
  ODA_ASSERT(m_pProcDevice);
  return m_pProcDevice;
}

inline OdGLES2JsonServer::IdType renditionDataType2ServerIdType(OdTrVisRendition::DataType type)
{
  OdGLES2JsonServer::IdType                   idType = OdGLES2JsonServer::kUnknownId;
  switch (type)
  {
    case OdTrVisRendition::kViewportData:    idType = OdGLES2JsonServer::kViewportId;    break;
    case OdTrVisRendition::kMetafileData:    idType = OdGLES2JsonServer::kMetafileId;    break;
    case OdTrVisRendition::kHlBranchData:    idType = OdGLES2JsonServer::kHlBranchId;    break;
    case OdTrVisRendition::kTextureData:     idType = OdGLES2JsonServer::kTextureId;     break;
    case OdTrVisRendition::kMaterialData:    idType = OdGLES2JsonServer::kMaterialId;    break;
    case OdTrVisRendition::kProgramData:     idType = OdGLES2JsonServer::kProgramId;     break;
    case OdTrVisRendition::kVisualStyleData: idType = OdGLES2JsonServer::kVisualStyleId; break;
    case OdTrVisRendition::kLayerData:       idType = OdGLES2JsonServer::kLayerId;       break;
    case OdTrVisRendition::kOverlayData:     idType = OdGLES2JsonServer::kOverlayId;     break;
    default:                                                                            break;
  }
  return idType;
}

bool OdGLES2JsonRendition::idRegTraverseCallback(OdTrVisRendition::DataType type, OdTrVisId id, OdTrVisUniqueID *pRegId)
{
  OdGLES2JsonServer::IdType idType = renditionDataType2ServerIdType(type);
  if (idType == OdGLES2JsonServer::kUnknownId) return true;
  json()->UnregisterId(id, pRegId, idType);    return true;
}

void OdGLES2JsonRendition::finalizeJsonServerUsage()
{
  if (m_pRedir && m_pJson.get())
  {
    m_pRedir->dropRecords(); // ???
    m_idReg.traverse(this);
    m_idReg.killAll();
  }
}

void OdGLES2JsonRendition::onDeviceUpdateBegin(OdTrVisRenderClient *pDevice)
{
#ifdef OD_GLES2JSON_DROPSCD_TEST
  DropSCDTest::m_pDevice = static_cast<OdTrVecDevice*>(pDevice);
#endif
  OutputState(OdGLES2JsonServer::kIntermediateState, true);

  json_obj()->SetMeataData((float)4.3, "Object", "TeighaObjectExporter");
  setGsUpdateMode();

  beginDeviceProcessing(pDevice);
  if (!updateClientDependentSectionOnly())
    m_pRedir->dropRecords(); // Output all collected between GsUpdate's data
}

void OdGLES2JsonRendition::onDeviceUpdateEnd(OdTrVisRenderClient *pDevice)
{
  setIntermediateMode();
  OutputState(OdGLES2JsonServer::kIntermediateState);  
  json_obj()->DropObject();

  endDeviceProcessing(pDevice);
}

void OdGLES2JsonRendition::obtainClientSettings(ClientSettings &clientSets)
{
  clientSets = m_clientSettings;
}

OdTrVisSharingProviderClientPtr OdGLES2JsonRendition::createSharingProvider()
{
  return OdRxObjectImpl<OdGLES2JsonSharingProvider, OdTrVisSharingProviderClient>::createObject();
}

OdTrVisRendition::MtHandlerType OdGLES2JsonRendition::mtDataTypeProcessing(DataType &type) const
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
    return kMtGlobal; // Secure by global mutex since all passed through one JsonServer
    default:
      ODA_FAIL();
      return kMtRedirect;
  };
}

// Returns set of bit flags which indicate rendition capabilities/settings
OdUInt32 OdGLES2JsonRendition::queryRenditionCaps(OdUInt32 requestFor) const
{
  OdUInt32 retCaps = kSupportGsStateSaving | kSupportCompositeMetafiles | kSupportVisualStyles | kSupportOverlayBuffers;
  if (GETBIT(requestFor, kUpdateClientSectionOnly))
    SETBIT(retCaps, kUpdateClientSectionOnly,    json()->UpdateClientDependentSectionOnly());
  if (GETBIT(requestFor, kUpdateShareableSectionOnly))
    SETBIT(retCaps, kUpdateShareableSectionOnly, json()->UpdateShareableSectionOnly());
  return retCaps;
}

bool OdGLES2JsonRendition::isSupportCompositeMetafiles() const
{
  ODA_ASSERT(processingDevice());
  return processingDevice()->useCompositeMetafiles();
}
bool OdGLES2JsonRendition::isSupportVisualStyles() const
{
  ODA_ASSERT(processingDevice());
  return processingDevice()->useVisualStyles();
}
bool OdGLES2JsonRendition::isSupportOverlayBuffers() const
{
  ODA_ASSERT(processingDevice());
  return processingDevice()->useOverlays();
}

// Returns Id registrator, or NULL if no registration supported
OdTrVisRendition::IdRegistrator *OdGLES2JsonRendition::idRegistrator() const
{
  return &m_idReg;
}

bool OdGLES2JsonRendition::saveRenditionState(OdGsFiler *pFiler) const
{
  pFiler->wrChar('X');
  // Save client settings
  //if (!::odTrVisSaveClientSettings(pFiler, m_clientSettings))
  //  return false;
  // Save Id mapping
  if (!m_idReg.save(pFiler))
    return false;

  json()->saveState(pFiler);

  // Save overall viewport
  if (!::odTrVisSaveViewportDef(pFiler, m_overallViewport))
    return false;
  return true;
}

bool OdGLES2JsonRendition::loadRenditionState(OdGsFiler *pFiler, OdTrVisIdMap *pIdMap)
{
  if (pFiler->rdChar() != 'X')
    return false;
  // Load client settings
  //if (!::odTrVisLoadClientSettings(pFiler, m_clientSettings))
  //  return false;
  // Load Id mapping
  if (!m_idReg.load(pFiler, pIdMap))
    return false;

  json()->loadState(pFiler, pIdMap);

  // Load overall viewport
  if (!::odTrVisLoadViewportDef(pFiler, m_overallViewport, pIdMap))
    return false;
  return true;
}

void OdGLES2JsonRendition::onSurfaceChanged(const OdTrVisSurfaceDef &pDef)
{
  json_obj()->SetBackColor(pDef.m_backgroundColor);
}

struct json_RegistrationBase
{
  OdGLES2JsonServer *m_pJson;
  OdTrVisUniqueID *m_pRegId;
  OdTrVisRendition::DataType m_dataType;
  OdGLES2JsonServer::IdType m_idType;
  OdTrVisId m_resId;

  json_RegistrationBase(OdGLES2JsonServer *pJson, OdGLES2RegistratorImpl &idReg, OdTrVisRendition::DataType dataType, OdTrVisId resId, bool idDel = false)
    : m_pJson(pJson)
    , m_pRegId(idReg.get(dataType, resId, idDel))
    , m_dataType(dataType)
    , m_idType(renditionDataType2ServerIdType(dataType))
    , m_resId(resId)
  {
  }
};
struct json_RegisterId : public json_RegistrationBase
{
  json_RegisterId(OdGLES2JsonServer *pJson, OdGLES2RegistratorImpl &idReg, OdTrVisRendition::DataType dataType, OdTrVisId resId)
    : json_RegistrationBase(pJson, idReg, dataType, resId)
  {
    m_pJson->RegisterId(m_resId, m_pRegId, m_idType);
  }
};
struct json_UnregisterId : public json_RegistrationBase
{
  json_UnregisterId(OdGLES2JsonServer *pJson, OdGLES2RegistratorImpl &idReg, OdTrVisRendition::DataType dataType, OdTrVisId resId)
    : json_RegistrationBase(pJson, idReg, dataType, resId) { }
  ~json_UnregisterId()
  {
    m_pJson->UnregisterId(m_resId, m_pRegId, m_idType);
  }
};
#define JSON_REGISTERED(dataType, resId) \
  json_RegistrationBase _jsonReg(json(), m_idReg, dataType, resId)
#define JSON_REGISTER(dataType, resId) \
  json_RegisterId _jsonReg(json(), m_idReg, dataType, resId)
#define JSON_UNREGISTER(dataType, resId) \
  json_UnregisterId _jsonReg(json(), m_idReg, dataType, resId)
#define JSON_DROP_REG_ID(pTagName) \
  json()->DropId(pTagName, _jsonReg.m_resId, _jsonReg.m_pRegId, _jsonReg.m_idType)

void OdGLES2JsonRendition::onOverlayAdded(OdTrVisOverlayId overlayId, const OdTrVisOverlayDef &pDef)
{
}

void OdGLES2JsonRendition::onOverlayDeleted(OdTrVisOverlayId overlayId)
{
}

void OdGLES2JsonRendition::onOverlayVisibilityChanged(OdTrVisOverlayId overlayId, bool bVisibility)
{
}

void OdGLES2JsonRendition::onOverlayRenderOrderChanged(OdTrVisOverlayId overlayId, OdInt16 nRenderOrder)
{
}

void OdGLES2JsonRendition::onViewportAdded(OdTrVisViewportId viewportId/*, const OdTrVisViewportDef &pDef*/)
{
  JSON_REGISTER(kViewportData, viewportId);
  json_obj()->SetCameraMetafileMode();
}

void OdGLES2JsonRendition::onViewportDeleted(OdTrVisViewportId viewportId)
{
}

void OdGLES2JsonRendition::onViewportInserted(OdTrVisViewportId viewportId, int nIndex)
{
}

void OdGLES2JsonRendition::DropViewParams(JsonObjectFormat *pJsonObj, const OdTrVisViewParamsDef &pDef, const OdTrVisId &view_id)
{
  pJsonObj->SetCameraMetafileMatrix(pDef.m_metafileMatrix);

  OdGeVector3d zAxis = (pDef.m_viewPosition - pDef.m_viewTarget).normalize();
  OdGeVector3d xAxis = zAxis.crossProduct(OdGeVector3d::kYAxis);
  OdGeVector3d yAxis = zAxis.crossProduct(xAxis);
  const OdGeMatrix3d &matrix = OdGeMatrix3d().setCoordSystem(pDef.m_viewPosition, xAxis, yAxis, zAxis);

  if (pDef.m_bPerspectiveProjection == true)
    pJsonObj->AddPerspectiveCamera(view_id, pDef.m_fieldWidth / pDef.m_fieldHeight, matrix, pDef.m_frontClip.m_bClipped ? pDef.m_frontClip.m_clipDist : (std::numeric_limits<float>::max)(), pDef.m_backClip.m_bClipped ? pDef.m_backClip.m_clipDist : (std::numeric_limits<float>::min)());
  else
    pJsonObj->AddOrthographicCamera(view_id, pDef.m_fieldWidth, pDef.m_fieldHeight, matrix, (std::numeric_limits<float>::max)(), (std::numeric_limits<float>::min)());
}

void OdGLES2JsonRendition::onViewportModified(OdTrVisViewportId viewportId, const OdTrVisViewportDef &pDef, OdUInt32 kindOfMod)
{
  JSON_REGISTERED(kViewportData, viewportId);//
 
  if (GETBIT(kindOfMod, kViewportModOrientation))
  {
    DropViewParams(json_obj(), pDef.m_viewParams, _jsonReg.m_resId);
  }
  if (GETBIT(kindOfMod, kViewportModLineweights))
  {
    json_obj()->RecalcLensWeight(pDef.m_lineweightsCoef);
    if (!pDef.m_lineweightsEnum.isEmpty())
      json_obj()->SetLensWeightByIndx(pDef.m_lineweightsEnum);
  }
}

void OdGLES2JsonRendition::onMetafileOrderChanged(OdTrVisViewportId viewportId, OdTrVisOverlayId overlayId, OdUInt32 nInvalidFrom, OdUInt32 nInvalidLen,
                                                 const OdTrVisDisplayId *pValidFrom, OdUInt32 nValidLen)
{
  onMetafileOrderChangedImpl(viewportId, overlayId, nInvalidFrom, nInvalidLen, pValidFrom, nValidLen);
}

bool OdGLES2JsonRendition::DropDisplayList(const OdTrVisDisplayId *pDispList, OdUInt32 nListLen, const OdTrVisId &ViewPortId, DDL2ndPassInfo *p2ndPass)
{ 
  for (OdUInt32 n = 0; n < nListLen; n++)
  {
    ODA_ASSERT(pDispList[n] >= kDisplayCodeRange);
    OdTrVisUniqueID *pRegEntry = m_idReg.get(kMetafileData, pDispList[n]);
    if (!p2ndPass || pRegEntry)
    {
      JSON_REGISTERED(kMetafileData, pDispList[n]);
      json_obj()->SetCameraMetafileMatrixToObject(_jsonReg.m_resId);
    }
    else
    {
      if (!p2ndPass->m_bScan)
      {
        p2ndPass->m_bScan = true;
        p2ndPass->m_nTransLast = p2ndPass->m_nTransFirst = n;
      }
      else
        p2ndPass->m_nTransLast = n;
    }
  }
  if (p2ndPass)
    return p2ndPass->m_bScan;
  return false;
}

void OdGLES2JsonRendition::DropDisplayList2ndPass(OdTrVisViewportId viewportId, OdTrVisOverlayId overlayId, OdUInt32 nInvalidFrom,
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

void OdGLES2JsonRendition::onMetafileOrderChangedImpl(OdTrVisViewportId viewportId, OdTrVisOverlayId overlayId, OdUInt32 nInvalidFrom, OdUInt32 nInvalidLen,
                                                     const OdTrVisDisplayId *pValidFrom, OdUInt32 nValidLen, bool bFirstPass)
{
  JSON_REGISTERED(kViewportData, viewportId);

  DDL2ndPassInfo s2ndPass;
  DropDisplayList(pValidFrom, nValidLen, _jsonReg.m_resId, (bFirstPass) ? &s2ndPass : NULL); 
}

void OdGLES2JsonRendition::onLightsListChanged(OdTrVisViewportId viewportId, float ambientLight[4], bool bDefaultLightsOn,
                                              const OdTrVisLightDef *pList, OdUInt32 nEntries)
{
  json_obj()->AddAmbientLight(viewportId, ODRGBA(ambientLight[0], ambientLight[1], ambientLight[2], ambientLight[3]));

  for (OdUInt32 n = 0; n < nEntries; n++)
  {
    switch ((OdInt32)pList[n].m_type)
    {
      case OdTrVisLightDef::kPoint:
        json_obj()->AddPointLight(viewportId, ODRGB(pList[n].m_color[0], pList[n].m_color[1], pList[n].m_color[2]), pList[n].m_position);
        break;
     case OdTrVisLightDef::kDistant:
       json_obj()->AddDirectionLight(viewportId, ODRGB(pList[n].m_color[0], pList[n].m_color[1], pList[n].m_color[2]), pList[n].m_direction);
       break;
     case OdTrVisLightDef::kSpot:
       json_obj()->AddSpotLight(viewportId, ODRGB(pList[n].m_color[0], pList[n].m_color[1], pList[n].m_color[2]), pList[n].m_position, pList[n].m_direction);
       break;
     default:
       break;
    }
  }
}

void OdGLES2JsonRendition::onBackgroundChanged(OdTrVisViewportId viewportId, OdTrVisFlatMetafileContainerPtr pStream, OdTrVisProgramId baseProgramId)
{
}

void OdGLES2JsonRendition::onExtentsChanged(OdTrVisViewportId viewportId, OdTrVisOverlayId overlayId, const OdTrVisExtentsDef &pDef)
{
}

void OdGLES2JsonRendition::onOverlayViewParamsOverride(OdTrVisViewportId viewportId, OdTrVisOverlayId overlayId, bool bOverride, const OdTrVisViewParamsDef &pDef)
{
}

OdTrVisDisplayId OdGLES2JsonRendition::onMetafileAdded(OdTrVisMetafileId metafileId, const OdTrVisMetafileDef &pDef)
{
  JSON_REGISTER(kMetafileData, metafileId);
  OutputState((_jsonReg.m_pRegId) ? OdGLES2JsonServer::kShareableState : OdGLES2JsonServer::kClientOnlyState);
  {
    if (!json()->DropMetafileAdded(*this, metafileId, pDef))
    {
      DropMetadata(pDef.m_pMetafile.get(), _jsonReg.m_resId, pDef.m_pMetafile->m_pLayer);
    }

#ifdef OD_GLES2JSON_DROPHLTASCLIENTONLY
    if (_jsonReg.m_pRegId && pDef.m_pMetafile->isMetafileHighlighted())
      onMetafileHighlightingChanged(metafileId, true);
#endif
  }
  return (OdTrVisDisplayId)metafileId;
}

void OdGLES2JsonRendition::onMetafileDeleted(OdTrVisMetafileId metafileId)
{
}

void OdGLES2JsonRendition::onMetafileVisibilityChanged(OdTrVisMetafileId metafileId, bool bVisibility)
{
}

void OdGLES2JsonRendition::onMetafileHighlightingChanged(OdTrVisMetafileId metafileId, bool bHighlight)
{
}

void OdGLES2JsonRendition::onMetafileFadingChanged(OdTrVisMetafileId metafileId, bool bFade)
{
}

void OdGLES2JsonRendition::onMetafileHlBranchAttached(OdTrVisMetafileId metafileId, OdTrVisHlBranchId hlBranchId)
{
}

void OdGLES2JsonRendition::onMetafileHlBranchDetached(OdTrVisMetafileId metafileId)
{
}

void OdGLES2JsonRendition::onHlBranchAdded(OdTrVisHlBranchId hlBranchId, const OdTrVisHlBranchDef &pDef)
{
}

void OdGLES2JsonRendition::onHlBranchModified(OdTrVisHlBranchId hlBranchId, const OdTrVisHlBranchDef &pDef,
                                             bool bChildsModified, bool bMarkersModified)
{
}

void OdGLES2JsonRendition::onHlBranchDeleted(OdTrVisHlBranchId hlBranchId)
{
}


void OdGLES2JsonRendition::onTextureAdded(OdTrVisTextureId textureId, const OdTrVisTextureDef &pDef)
{
}

void OdGLES2JsonRendition::onTextureDeleted(OdTrVisTextureId textureId)
{
}

void OdGLES2JsonRendition::onMaterialAdded(OdTrVisMaterialId materialId, const OdTrVisMaterialDef &pDef)
{
  JSON_REGISTER(kMaterialData, materialId);
  OutputState((_jsonReg.m_pRegId) ? OdGLES2JsonServer::kShareableState : OdGLES2JsonServer::kClientOnlyState);

  json_obj()->AddMaterial(_jsonReg.m_resId, JsonMaterials::default_mat_type, pDef.m_opacityLevel);
  json_obj()->AddMaterialColor(_jsonReg.m_resId, JsonMaterialColor::ambient_color, ODRGB(pDef.m_ambientColor[0], pDef.m_ambientColor[1], pDef.m_ambientColor[2]));
  json_obj()->AddMaterialColor(_jsonReg.m_resId, JsonMaterialColor::emessive_color, ODRGB(pDef.m_emissionColor[0], pDef.m_emissionColor[1], pDef.m_emissionColor[2]));
  json_obj()->AddMaterialColor(_jsonReg.m_resId, JsonMaterialColor::specular_color, ODRGB(pDef.m_specularColor[0], pDef.m_specularColor[1], pDef.m_specularColor[2]));
}

void OdGLES2JsonRendition::onMaterialDeleted(OdTrVisMaterialId materialId)
{
}

void OdGLES2JsonRendition::onVisualStyleAdded(OdTrVisVisualStyleId visualStyleId, const OdTrVisVisualStyle &pDef)
{
}

void OdGLES2JsonRendition::onVisualStyleModified(OdTrVisVisualStyleId visualStyleId, const OdTrVisVisualStyle &pDef)
{
}

void OdGLES2JsonRendition::onVisualStyleDeleted(OdTrVisVisualStyleId visualStyleId)
{
}

void OdGLES2JsonRendition::onLayerAdded(OdTrVisLayerId layerId, const OdTrVisLayerDef &pDef)
{
  JSON_REGISTER(kLayerData, layerId);
  json_obj()->AddLayer(_jsonReg.m_resId, pDef.m_name);
}

void OdGLES2JsonRendition::onLayerModified(OdTrVisLayerId layerId, const OdTrVisLayerDef &pDef)
{
}

void OdGLES2JsonRendition::onLayerDeleted(OdTrVisLayerId layerId)
{
}

void OdGLES2JsonRendition::onLayerViewportPropsOverride(OdTrVisViewportId viewportId, OdTrVisLayerId layerId, bool bOverride, const OdTrVisLayerProps &pProps )
{
}


void OdGLES2JsonRendition::DropHlBranch(const OdTrVisHlBranchDef &pHlBranch, bool bDropChilds, bool bDropMarkers)
{
}

void OdGLES2JsonRendition::DropVisualStyleData(const OdTrVisVisualStyle &pVStyle, bool bDropNonModified)
{
}

void OdGLES2JsonRendition::DropLayerData(const OdTrVisLayerDef &pLayer, bool bDropOwnerId)
{ 
}

void OdGLES2JsonRendition::DropLayerProps(const OdTrVisLayerProps &pLayerProps)
{
}

void OdGLES2JsonRendition::DropMetadata(const OdTrVisFlatMetafileContainer *pMetafile, const OdUInt64 &metafile_reg_id, const OdUInt64 &layer_id)
{
  OdUInt32 uSize = pMetafile->size();
  if (uSize == 0) return;

  OdUInt32 currArrayNum = 0;
  OdUInt8 currArrayType = 0;

  const OdUInt8 *pMemPtr = pMetafile->memoryPtr();
  const OdUInt8 *pMemPtrReadFor = pMemPtr + uSize; 

  JsonMetafileConverter metafile_converter;

  while (pMemPtr < pMemPtrReadFor)
  {
    OdTrVisMetaRecType recType = OD_OGL_RDR_READVALRETYPE(OdTrVisMetaRecType, OdUInt8, pMemPtr);
    OD_OGL_RDR_SEEKINC(pMemPtr);
    switch (recType)
    {
      case OdTrVisMetaRecType_EnableOpt:
      {
        OD_OGL_RDR_SEEKINC(pMemPtr);
      }
      break;
      case OdTrVisMetaRecType_DisableOpt:
      {
        OD_OGL_RDR_SEEKINC(pMemPtr);
      }
      break;
      case OdTrVisMetaRecType_Color:
      {
        metafile_converter.SetRGB(ODRGBA(pMemPtr[0], pMemPtr[1], pMemPtr[2], pMemPtr[3]));
        OD_OGL_RDR_SEEKFWD(pMemPtr, 4);
      }
      break;
      case OdTrVisMetaRecType_EnableArray:
      {
        const OdUInt8 &type = OD_OGL_RDR_READVAL(OdUInt8, pMemPtr);
        if (type == JsonMetafileConverter::colors_arr)
          metafile_converter.OrToFacesForm(JsonMetafileConverter::vertex_color);
        else if (type == JsonMetafileConverter::normals_arr)
          metafile_converter.OrToFacesForm(JsonMetafileConverter::vertex_normal);

        OD_OGL_RDR_SEEKINC(pMemPtr);

        if (type != JsonMetafileConverter::secondary_normal_arr)
          metafile_converter.AddGeometryArrayInd(OD_OGL_RDR_READVAL(OdUInt32, pMemPtr), type);

        OD_OGL_RDR_SEEKFWD(pMemPtr, sizeof(OdUInt32));
      }
      break;
      case OdTrVisMetaRecType_DisableArray:
      {
        const OdUInt8 &type = OD_OGL_RDR_READVAL(OdUInt8, pMemPtr);
        if (type == JsonMetafileConverter::colors_arr)
          metafile_converter.AndToFacesForm(~(JsonMetafileConverter::vertex_color));
        else if (type == JsonMetafileConverter::normals_arr)
          metafile_converter.AndToFacesForm(~(JsonMetafileConverter::vertex_normal));

        if (type != JsonMetafileConverter::secondary_normal_arr)
          metafile_converter.DeleteGeometryArrayByType(type);

        OD_OGL_RDR_SEEKINC(pMemPtr);
      }
      break;
      case OdTrVisMetaRecType_DrawArrays:
      {
        const OdUInt8 &type = OD_OGL_RDR_READVAL(OdUInt8, pMemPtr);
        const OdInt32 &first = OD_OGL_RDR_READVALOFFSET(OdInt32, pMemPtr, 1);
        const OdInt32 &count = OD_OGL_RDR_READVALOFFSET(OdInt32, pMemPtr, 1 + sizeof(OdInt32));
        if (type == 0)//render points (1 vertex per point).
          metafile_converter.AddPoint(first, count);
        else if (type == 1)//render lines 
          metafile_converter.AddLine(first, count);
        else if (type == 3)//render lines strip
          metafile_converter.AddLineStrip(first, count);
        else if (type == 4 && json()->isEnableFaces())//render filled triangles(3 vertexes per triangle)
          metafile_converter.AddMesh(first, count);
        //not implemented for type
        //1 - render lines loop
        //5 - render filled triangles strip
        //6 - render filled triangles fan

        OD_OGL_RDR_SEEKFWD(pMemPtr, (sizeof(OdInt32) << 1) + 1);
      }
      break;
      case OdTrVisMetaRecType_DrawElements:
      {
        const OdUInt8 &type = OD_OGL_RDR_READVAL(OdUInt8, pMemPtr);
        const OdInt32 &count = OD_OGL_RDR_READVALOFFSET(OdInt32, pMemPtr, 1);
        const OdUInt32 &ind = OD_OGL_RDR_READVALOFFSET(OdUInt32, pMemPtr, 1 + sizeof(OdInt32));
        if (type == 0)//render points (1 vertex per point). 
          metafile_converter.AddPoint(0, count, JsonMetafileConverter::buffer_geometry, ind);
        else if (type == 1)//render lines 
          metafile_converter.AddLine(0, count, JsonMetafileConverter::buffer_geometry, ind);
        else if (type == 3)//render lines strip
          metafile_converter.AddLineStrip(0, count, JsonMetafileConverter::buffer_geometry, ind);
        else if (type == 4 && json()->isEnableFaces())//render filled triangles(3 vertexes per triangle)
          metafile_converter.AddMesh(0, count, JsonMetafileConverter::buffer_geometry, ind);
        //not implemented for type:
        //1 - render lines loop
        //5 - render filled triangles strip
        //6 - render filled triangles fan

        OD_OGL_RDR_SEEKFWD(pMemPtr, sizeof(OdInt32) + 1 + sizeof(OdUInt32));
      }
      break;
      case OdTrVisMetaRecType_CullFace:
      {
        OD_OGL_RDR_SEEKINC(pMemPtr);
      }
      break;
      case OdTrVisMetaRecType_LStipple:
      {
        OD_OGL_RDR_SEEKINC(pMemPtr);
      }
      break;
      case OdTrVisMetaRecType_PStipple:
      {
        OD_OGL_RDR_SEEKINC(pMemPtr);
      }
      break;
      case OdTrVisMetaRecType_VPoint:
      {
        const float *pFloats = (const float*)pMemPtr;
        metafile_converter.AddPoint(pFloats);
      }
      OD_OGL_RDR_SEEKFWD(pMemPtr, (sizeof(float) << 1) + sizeof(float));
      break;
      case OdTrVisMetaRecType_VLine:
      {
        const float *pFloats = (const float*)pMemPtr;
        metafile_converter.AddLine(pFloats);
      }
      OD_OGL_RDR_SEEKFWD(pMemPtr, (sizeof(float) << 2) + (sizeof(float) << 1));
      break;
      case OdTrVisMetaRecType_IPoint:
      {
        const OdInt32 &first = OD_OGL_RDR_READVAL(OdInt32, pMemPtr);
        const OdInt32 &count = 1;
        metafile_converter.AddPoint(first, count);
      }
      OD_OGL_RDR_SEEKFWD(pMemPtr, sizeof(OdInt32));
      break;
      case OdTrVisMetaRecType_ILine:
      {
        const OdInt32 &first = OD_OGL_RDR_READVAL(OdInt32, pMemPtr);
        const OdInt32 &count = OD_OGL_RDR_READVALOFFSET(OdInt32, pMemPtr, sizeof(OdInt32)) - first + 1;
        metafile_converter.AddLine(first, count);
      }
      OD_OGL_RDR_SEEKFWD(pMemPtr, sizeof(OdInt32) * 2);
      break;
      case OdTrVisMetaRecType_HLRStencil:
      {
        OD_OGL_RDR_SEEKINC(pMemPtr);
      }
      break;
      case OdTrVisMetaRecType_EnableShading:
      {
        OD_OGL_RDR_SEEKINC(pMemPtr);
      }
      break;
      case OdTrVisMetaRecType_DisableShading:
      {
        OD_OGL_RDR_SEEKINC(pMemPtr);
      }
      break;
      case OdTrVisMetaRecType_Material:
        metafile_converter.SetMaterial(OD_OGL_RDR_READVAL(OdUInt64, pMemPtr));
        OD_OGL_RDR_SEEKFWD(pMemPtr, 8);
      break;
      case OdTrVisMetaRecType_UserEntry:
        OD_OGL_RDR_SEEKFWD(pMemPtr, sizeof(OdUInt32) + OD_OGL_RDR_READVAL(OdUInt32, pMemPtr));
      break;
      case OdTrVisMetaRecType_InitTexture:
        OD_OGL_RDR_SEEKFWD(pMemPtr, 8 + 1);
      break;
      case OdTrVisMetaRecType_UninitTexture: 
      break;
      case OdTrVisMetaRecType_SelectionMarker:
        OD_OGL_RDR_SEEKFWD(pMemPtr, sizeof(OdTrVisGsMarker));
      break;
      case OdTrVisMetaRecType_EnableMarkerArray:
      {
        OD_OGL_RDR_SEEKINC(pMemPtr);
        OD_OGL_RDR_SEEKFWD(pMemPtr, sizeof(OdUInt32));
      }
      break;
      case OdTrVisMetaRecType_DisableMarkerArray:
      {
        OD_OGL_RDR_SEEKINC(pMemPtr);
      }
      break;
      case OdTrVisMetaRecType_SelectionFlags:
      {
        OD_OGL_RDR_SEEKINC(pMemPtr);
      }
      break;
      case OdTrVisMetaRecType_Lineweight:
      {
        const OdUInt8 &lwdType = OD_OGL_RDR_READVAL(OdUInt8, pMemPtr);
        if (OdTrVisLwdSetting::isPs((OdTrVisLwdSetting::LwdType)lwdType))
        {
          metafile_converter.SetLineWeight<double>(lwdType, OD_OGL_RDR_READVALOFFSET(double, pMemPtr, sizeof(OdUInt8)));
          OD_OGL_RDR_SEEKFWD(pMemPtr, sizeof(OdUInt8) + sizeof(double));
        }
        else
        {
          metafile_converter.SetLineWeight<OdInt16>(lwdType, OD_OGL_RDR_READVALOFFSET(OdInt16, pMemPtr, sizeof(OdUInt8)));
          OD_OGL_RDR_SEEKFWD(pMemPtr, sizeof(OdUInt8) + sizeof(OdInt16));
        }
      }
      break;
      case OdTrVisMetaRecType_Linestyle:
      {
       const bool defStyle = (OD_OGL_RDR_READVAL(OdUInt8, pMemPtr) != 0);
       if (!defStyle)
       {
         OD_OGL_RDR_SEEKFWD(pMemPtr, sizeof(OdUInt8) * 3);
       }
       else
         OD_OGL_RDR_SEEKINC(pMemPtr);
      }
      break;
      case OdTrVisMetaRecType_Program:
        OD_OGL_RDR_SEEKFWD(pMemPtr, 8);
      break;
      case OdTrVisMetaRecType_TtfText:
      {
        const OdTrVisTtfFontsCacheClient *pCache = processingDevice()->ttfFontsCache();
        if (pCache)
        { // In case if real vectorization device is connected
          OdUInt32 nText = OD_OGL_RDR_READVALOFFSET(OdUInt32, pMemPtr, sizeof(OdUInt64) * 2 + sizeof(OdGeMatrix3d) + sizeof(OdGeVector3d));
          OD_OGL_RDR_SEEKFWD(pMemPtr, sizeof(OdUInt64) * 2 + sizeof(OdGeMatrix3d) + sizeof(OdGeVector3d) + sizeof(OdUInt32) * (nText + 1));
        }
        else
        { // In case if there is no connection to real vectorization device
          OD_OGL_RDR_SEEKFWD(pMemPtr, sizeof(OdGeMatrix3d) + sizeof(OdGeVector3d) + sizeof(OdUInt32));
        }
      }
      break;
      case OdTrVisMetaRecType_PushMatrix:
      {
        bool bIdentity = OD_OGL_RDR_READVALRETYPE(OdTrVisPushMatrixType, OdUInt8, pMemPtr) == OdTrVisPushMatrixType_Identity;
        OD_OGL_RDR_SEEKINC(pMemPtr);
        if (!bIdentity)
        {
          OD_OGL_RDR_SEEKFWD(pMemPtr, sizeof(OdGeMatrix3d));
        }
      }
      break;
      case OdTrVisMetaRecType_PopMatrix:
      break;
      case OdTrVisMetaRecType_Metafile:
      {
        OD_OGL_RDR_SEEKFWD(pMemPtr, sizeof(OdTrVisMetafileId));
      }
      break;
      case OdTrVisMetaRecType_GeomMarker:
      {
        metafile_converter.SetGeomMark(OD_OGL_RDR_READVAL(OdUInt8, pMemPtr));
        OD_OGL_RDR_SEEKINC(pMemPtr);
      }
      break;
      case OdTrVisMetaRecType_VisualStyle:
      {
        OD_OGL_RDR_SEEKFWD(pMemPtr, sizeof(OdTrVisVisualStyleId));
      }
      break;
      case OdTrVisMetaRecType_MetafileMarker:
      {
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

  metafile_converter.AddObjectsByRootId(json_obj(), pMetafile, metafile_reg_id, layer_id);
}

void OdGLES2JsonRendition::OutputState(OdGLES2JsonServer::OutputState newState, bool bForce)
{
  if ((newState != m_outputState) || bForce)
  {
    m_outputState = newState;
    json()->OnStateChanged(newState);
  }
}
