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
// GLES2 device local resource sharing provider

#include "OdaCommon.h"
//#include "../vec/TrVecDevice.h"
//#include "TrGL2LocalRendition.h"
#include "TrGL2LocalRenditionHost.h"
#include "ExGsGLES2LocalSharingProvider.h"

OdTrGL2LocalSharingProvider::OdTrGL2LocalSharingProvider()
  : OdTrVecResourceSharingProvider()
{
}

OdTrGL2LocalSharingProvider::~OdTrGL2LocalSharingProvider()
{
}

OdTrGL2LocalRenditionHost *OdTrGL2LocalSharingProvider::renditionHost(OdTrVecDevice *pRequestFor)
{
  return OdTrGL2LocalRenditionHostPtr(OdTrVecResourceSharingProvider::rendition(pRequestFor));
}

// Share metafile with secondary device (should be overrided by client, do nothing by default)
bool OdTrGL2LocalSharingProvider::shareMetafile(OdTrVecDevice *pRequestedFor, OdTrVecDevice *pRequestedFrom,
                                                OdTrVisMetafileId metafileId, void * /*pInternalDataPtr*/, void * /*pOriginatorPtr*/)
{
  OdTrVisMetafileDef metafileDef;
  renditionHost(pRequestedFrom)->getMetafileDef(metafileId, metafileDef);
  OdTrVecResourceSharingProvider::rendition(pRequestedFor)->onMetafileAdded(metafileId, metafileDef);
  return true;
}

// Share texture with secondary device (should be overrided by client, do nothing by default)
bool OdTrGL2LocalSharingProvider::shareTexture(OdTrVecDevice *pRequestedFor, OdTrVecDevice *pRequestedFrom,
                                               OdTrVisTextureId textureId, void *pInternalDataPtr, void *pOriginatorPtr)
{
  OdTrVisTextureDef textureDef; bool bNeedSend = false;
  bool bRes = internalTextureProc(pRequestedFor, pRequestedFrom, textureId, pInternalDataPtr, pOriginatorPtr, textureDef, bNeedSend);
  if (bNeedSend)
    OdTrVecResourceSharingProvider::rendition(pRequestedFor)->onTextureAdded(textureId, textureDef);
  return bRes;
}

// Share material with secondary device (should be overrided by client, do nothing by default)
bool OdTrGL2LocalSharingProvider::shareMaterial(OdTrVecDevice *pRequestedFor, OdTrVecDevice *pRequestedFrom,
                                                OdTrVisMaterialId materialId, void *pInternalDataPtr, void *pOriginatorPtr)
{
  OdTrVisMaterialDef pDef;
  // Access exist material
  renditionHost(pRequestedFrom)->getMaterialDef(materialId, pDef);
  // Send material to new device
  OdTrVecResourceSharingProvider::rendition(pRequestedFor)->onMaterialAdded(materialId, pDef);
  // Send textures if exist
  if (pDef.m_textureBlend > 0.0f) // Actually check isn't needed here, placed for optimization only
    materialTexturesProc(pRequestedFor, pRequestedFrom, materialId, pInternalDataPtr, pOriginatorPtr);
  return true;
}

//
