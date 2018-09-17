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
// GLES2 device local resource sharing provider

#include "OdaCommon.h"
#include "Tr/TrVisRenderClient.h"
#include "ExGsGLES2JsonRendition.h"
#include "ExGsGLES2JsonSharingProvider.h"

OdGLES2JsonSharingProvider::OdGLES2JsonSharingProvider()
  : OdTrVecResourceSharingProvider()
{
}

OdGLES2JsonSharingProvider::~OdGLES2JsonSharingProvider()
{
}

OdGLES2JsonRenditionRedir *OdGLES2JsonSharingProvider::rendition(OdTrVecDevice *pRequestFor)
{
  return static_cast<OdGLES2JsonRenditionRedir*>(OdTrVecResourceSharingProvider::rendition(pRequestFor));
}

// Share metafile with secondary device (should be overrided by client, do nothing by default)
bool OdGLES2JsonSharingProvider::shareMetafile(OdTrVecDevice *pRequestedFor, OdTrVecDevice *pRequestedFrom,
                                              OdTrVisMetafileId metafileId, void * /*pInternalDataPtr*/, void * /*pOriginatorPtr*/)
{
  shareIdRegistration(pRequestedFor, pRequestedFrom, metafileId, OdTrVisRendition::kMetafileData);
  OdTrVisUniqueID *pRegId = rendition(pRequestedFor)->idRegistrator()->get(OdTrVisRendition::kMetafileData, metafileId);
  if (pRegId) rendition(pRequestedFor)->jsonRendition().json()->RegisterId(metafileId, pRegId, OdGLES2JsonServer::kMetafileId);
  // Returns true since metafiles already available on client side
  return true;
}

// Share texture with secondary device (should be overrided by client, do nothing by default)
bool OdGLES2JsonSharingProvider::shareTexture(OdTrVecDevice *pRequestedFor, OdTrVecDevice *pRequestedFrom,
                                             OdTrVisTextureId textureId, void *pInternalDataPtr, void *pOriginatorPtr)
{
  shareIdRegistration(pRequestedFor, pRequestedFrom, textureId, OdTrVisRendition::kTextureData);
  if ((clientSettings(pRequestedFor).isSupportNonPow2Textures() != clientSettings(pRequestedFrom).isSupportNonPow2Textures()) ||
      (clientSettings(pRequestedFor).isSupportNonPow2TexturesWrapping() != clientSettings(pRequestedFrom).isSupportNonPow2TexturesWrapping()) ||
      (clientSettings(pRequestedFor).m_nMaxTextureSize != clientSettings(pRequestedFrom).m_nMaxTextureSize) ||
      (clientSettings(pRequestedFor).m_nTextureAlignment != clientSettings(pRequestedFrom).m_nTextureAlignment) ||
      (clientSettings(pRequestedFor).isRequireBGR() != clientSettings(pRequestedFrom).isRequireBGR()) ||
      (clientSettings(pRequestedFor).isSupportBGR() != clientSettings(pRequestedFrom).isSupportBGR()) ||
      (clientSettings(pRequestedFor).isSupportBGRA() != clientSettings(pRequestedFrom).isSupportBGRA()) ||
      (clientSettings(pRequestedFor).isLutOverride() != clientSettings(pRequestedFrom).isLutOverride()) ||
      (clientSettings(pRequestedFor).isLutMonochrome() != clientSettings(pRequestedFrom).isLutMonochrome()) ||
      (clientSettings(pRequestedFor).isLutPalette() != clientSettings(pRequestedFrom).isLutPalette()) ||
      (renderClient(pRequestedFor)->getProperty(OD_T("UseLutPalette"))->getUInt32() != renderClient(pRequestedFrom)->getProperty(OD_T("UseLutPalette"))->getUInt32()))
  { // Texture settings differ, so we need regenerate textures with newer settings
    OdTrVisTextureDef textureDef; bool bNeedSend = false;
    bool bRes = internalTextureProc(pRequestedFor, pRequestedFrom, textureId, pInternalDataPtr, pOriginatorPtr, textureDef, bNeedSend);
    if (bNeedSend)
      rendition(pRequestedFor)->onTextureAdded(textureId, textureDef);
    return bRes;
  }
  OdTrVisUniqueID *pRegId = rendition(pRequestedFor)->idRegistrator()->get(OdTrVisRendition::kTextureData, textureId);
  if (pRegId) rendition(pRequestedFor)->jsonRendition().json()->RegisterId(textureId, pRegId, OdGLES2JsonServer::kTextureId);
  // Returns true since textures with compatible settings already available on client side
  return true;
}

// Share material with secondary device (should be overrided by client, do nothing by default)
bool OdGLES2JsonSharingProvider::shareMaterial(OdTrVecDevice *pRequestedFor, OdTrVecDevice *pRequestedFrom,
                                              OdTrVisMaterialId materialId, void *pInternalDataPtr, void *pOriginatorPtr)
{
  shareIdRegistration(pRequestedFor, pRequestedFrom, materialId, OdTrVisRendition::kMaterialData);
  OdTrVisUniqueID *pRegId = rendition(pRequestedFor)->idRegistrator()->get(OdTrVisRendition::kMaterialData, materialId);
  if (pRegId) rendition(pRequestedFor)->jsonRendition().json()->RegisterId(materialId, pRegId, OdGLES2JsonServer::kMaterialId);
  // This call process nested material textures
  materialTexturesProc(pRequestedFor, pRequestedFrom, materialId, pInternalDataPtr, pOriginatorPtr);
  // Returns true since materials already available on client side
  return true;
}

//
