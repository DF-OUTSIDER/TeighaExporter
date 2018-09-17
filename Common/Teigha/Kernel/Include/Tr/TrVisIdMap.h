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
// GLES2 Id mapping

#ifndef ODTRVISREGENTRY
#define ODTRVISREGENTRY

#include "TD_PackPush.h"

#include "TrVisRendition.h"

class OdGsFiler;

/** \details
    <group ExRender_Windows_Classes>
*/
class OdTrVisIdMap : public OdRxObject
{
  public:
    static const OdChar* getClassName();
    static void attachToFiler(OdGsFiler *pFiler);
    static OdTrVisIdMap* getFromFiler(OdGsFiler *pFiler);
    static void detachFromFiler(OdGsFiler *pFiler);

    // Id Maps accessors

    virtual void add(OdTrVisRendition::DataType type, OdTrVisId nPrev, OdTrVisId nNew) = 0;
    virtual void kill(OdTrVisRendition::DataType type, OdTrVisId nPrev) = 0;

    virtual void clear(OdTrVisRendition::DataType type) = 0;
    virtual void clear() = 0;

    virtual OdTrVisId get(OdTrVisRendition::DataType type, OdTrVisId nPrev, bool *bRes = NULL) const = 0;
    virtual bool get(OdTrVisRendition::DataType type, OdTrVisId nPrev, OdTrVisId &nNew) const = 0;

    virtual OdTrVisId longSearch(OdTrVisRendition::DataType type, OdTrVisId nNew, bool *bRes = NULL) const = 0;
    virtual bool longSearch(OdTrVisRendition::DataType type, OdTrVisId nNew, OdTrVisId &nPrev) const = 0;

    virtual OdUInt32 numEntries(OdTrVisRendition::DataType type) const = 0;
    virtual const void *getEntry(OdTrVisRendition::DataType type, const void *pIndex, OdTrVisId &nPrev, OdTrVisId &nNew) const = 0;

    // Resource data accessors

    class ResourceDataAccessor : public OdRxObject
    {
      public:
        virtual bool getResourceData(OdTrVisId resourceId, void *pDataStorage) = 0;
    };

    virtual void addDataAccessor(OdTrVisRendition::DataType type, OdTrVisId resourceId, ResourceDataAccessor *pAccessor) = 0;
    virtual void killDataAccessor(OdTrVisRendition::DataType type, OdTrVisId resourceId) = 0;

    virtual void clearDataAccessors(OdTrVisRendition::DataType type) = 0;
    virtual void clearDataAccessors() = 0;

    virtual void clearAll(OdTrVisRendition::DataType type) = 0;
    virtual void clearAll() = 0;

    virtual ResourceDataAccessor *getDataAccessor(OdTrVisRendition::DataType type, OdTrVisId resourceId) const = 0;
    virtual bool dataAccess(OdTrVisRendition::DataType type, OdTrVisId resourceId, void *pDataStorage) const = 0;

    // Texture data accessor

    class TextureDataAccessor : public ResourceDataAccessor
    {
      public:
        bool getTextureData(OdTrVisTextureId textureId, OdTrVisTextureDef &pDef)
        {
          return getResourceData(textureId, &pDef);
        }
    };

    TextureDataAccessor *getTextureDataAccessor(OdTrVisTextureId textureId) const
    {
      return static_cast<TextureDataAccessor*>(getDataAccessor(OdTrVisRendition::kTextureData, textureId));
    }
    bool textureDataAccess(OdTrVisTextureId textureId, OdTrVisTextureDef &pDef)
    {
      return dataAccess(OdTrVisRendition::kTextureData, textureId, &pDef);
    }
};

// OdGsFiler helpers
// for OdTrVisRendition::ClientSettings
bool odTrVisSaveClientSettings(OdGsFiler *pFiler, const OdTrVisRendition::ClientSettings &cliSets);
bool odTrVisLoadClientSettings(OdGsFiler *pFiler, OdTrVisRendition::ClientSettings &cliSets);
// for OdTrVisSurfaceDef
bool odTrVisSaveSurfaceDef(OdGsFiler *pFiler, const OdTrVisSurfaceDef &pDef);
bool odTrVisLoadSurfaceDef(OdGsFiler *pFiler, OdTrVisSurfaceDef &pDef, OdTrVisIdMap *pIdMap = NULL);
void odTrVisLoadPostprocessSurfaceDef(OdTrVisSurfaceDef &pDef, OdTrVisIdMap *pIdMap);
// for OdTrVisViewParamsDef
bool odTrVisSaveViewParamsDef(OdGsFiler *pFiler, const OdTrVisViewParamsDef &pDef);
bool odTrVisLoadViewParamsDef(OdGsFiler *pFiler, OdTrVisViewParamsDef &pDef, OdTrVisIdMap *pIdMap = NULL);
void odTrVisLoadPostprocessViewParamsDef(OdTrVisViewParamsDef &pDef, OdTrVisIdMap *pIdMap);
// for OdTrVisViewportDef
bool odTrVisSaveViewportDef(OdGsFiler *pFiler, const OdTrVisViewportDef &pDef);
bool odTrVisLoadViewportDef(OdGsFiler *pFiler, OdTrVisViewportDef &pDef, OdTrVisIdMap *pIdMap = NULL);
void odTrVisLoadPostprocessViewportDef(OdTrVisViewportDef &pDef, OdTrVisIdMap *pIdMap);
// for OdTrVisLightDef
bool odTrVisSaveLightDef(OdGsFiler *pFiler, const OdTrVisLightDef &pDef);
bool odTrVisLoadLightDef(OdGsFiler *pFiler, OdTrVisLightDef &pDef, OdTrVisIdMap *pIdMap = NULL);
void odTrVisLoadPostprocessLightDef(OdTrVisLightDef &pDef, OdTrVisIdMap *pIdMap);
// for OdTrVisFlatMetafileContainer
bool odTrVisSaveRawMetafileData(OdGsFiler *pFiler, const OdTrVisFlatMetafileContainer *pMf);
bool odTrVisLoadRawMetafileData(OdGsFiler *pFiler, OdTrVisFlatMetafileContainerPtr &pMf, OdTrVisIdMap *pIdMap = NULL, bool bDisplayId = false);
void odTrVisLoadPostprocessRawMetafileData(OdTrVisFlatMetafileContainer *pMf, OdTrVisIdMap *pIdMap, bool bDisplayId = false);
// for OdTrVisMetafileContainer
bool odTrVisSaveMetafileData(OdGsFiler *pFiler, const OdTrVisMetafileContainer *pMf);
bool odTrVisLoadMetafileData(OdGsFiler *pFiler, OdTrVisMetafileContainerPtr &pMf, OdTrVisIdMap *pIdMap = NULL, bool bDisplayId = false);
void odTrVisLoadPostprocessMetafileData(OdTrVisMetafileContainer *pMf, OdTrVisIdMap *pIdMap, bool bDisplayId = false);
// for OdTrVisMetafileDef
bool odTrVisSaveMetafileDef(OdGsFiler *pFiler, const OdTrVisMetafileDef &pDef);
bool odTrVisLoadMetafileDef(OdGsFiler *pFiler, OdTrVisMetafileDef &pDef, OdTrVisIdMap *pIdMap = NULL, bool bDisplayId = false);
void odTrVisLoadPostprocessMetafileDef(OdTrVisMetafileDef &pDef, OdTrVisIdMap *pIdMap, bool bDisplayId = false);
// for OdTrVisHlBranchDef
bool odTrVisSaveHlBranchDef(OdGsFiler *pFiler, const OdTrVisHlBranchDef &pDef);
bool odTrVisLoadHlBranchDef(OdGsFiler *pFiler, OdTrVisHlBranchDef &pDef, OdTrVisIdMap *pIdMap = NULL);
void odTrVisLoadPostprocessHlBranchDef(OdTrVisHlBranchDef &pDef, OdTrVisIdMap *pIdMap);
// for OdTrVisTextureDef
bool odTrVisSaveTextureDef(OdGsFiler *pFiler, const OdTrVisTextureDef &pDef);
bool odTrVisLoadTextureDef(OdGsFiler *pFiler, OdTrVisTextureDef &pDef, OdTrVisIdMap *pIdMap = NULL);
void odTrVisLoadPostprocessTextureDef(OdTrVisTextureDef &pDef, OdTrVisIdMap *pIdMap);
bool odTrVisSaveTexturePtr( OdGsFiler *pFiler, const OdTrVisTexture* tex );
OdTrVisTexturePtr odTrVisLoadTexturePtr( OdGsFiler *pFiler );
// for OdTrVisMaterialDef
bool odTrVisSaveMaterialDef(OdGsFiler *pFiler, const OdTrVisMaterialDef &pDef);
bool odTrVisLoadMaterialDef(OdGsFiler *pFiler, OdTrVisMaterialDef &pDef, OdTrVisIdMap *pIdMap = NULL);
void odTrVisLoadPostprocessMaterialDef(OdTrVisMaterialDef &pDef, OdTrVisIdMap *pIdMap);
// for OdTrVisVisualStyle
bool odTrVisSaveVisualStyle(OdGsFiler *pFiler, const OdTrVisVisualStyle &pVS);
bool odTrVisLoadVisualStyle(OdGsFiler *pFiler, OdTrVisVisualStyle &pVS);
// for OdTrVisLayerProps
void odTrVisSaveLayerDefProps(OdGsFiler *pFiler, const OdTrVisLayerProps &pProps);
bool odTrVisLoadLayerDefProps(OdGsFiler *pFiler, OdTrVisLayerProps &pProps);
// for OdTrVisLayerDef
bool odTrVisSaveLayerDef(OdGsFiler *pFiler, const OdTrVisLayerDef &pDef);
bool odTrVisLoadLayerDef(OdGsFiler *pFiler, OdTrVisLayerDef &pDef, OdTrVisIdMap *pIdMap = NULL);
void odTrVisLoadPostprocessLayerDef(OdTrVisLayerDef &pDef, OdTrVisIdMap *pIdMap);
// for OdTrVisExtentsDef
bool odTrVisSaveExtentsDef(OdGsFiler *pFiler, const OdTrVisExtentsDef &pDef);
bool odTrVisLoadExtentsDef(OdGsFiler *pFiler, OdTrVisExtentsDef &pDef);

#include "TD_PackPop.h"

#endif // ODTRVISREGENTRY
