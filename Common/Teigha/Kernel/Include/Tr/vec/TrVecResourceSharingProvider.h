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
// GLES2 device resource sharing provider

#ifndef ODTRVECRESOURCESHARINGPROVIDER
#define ODTRVECRESOURCESHARINGPROVIDER

#include "TD_PackPush.h"

#include "TrVecDefs.h"
#include "TrVisRendition.h"
#include "ExDrawOrderBase.h"

#include "StaticRxObject.h"

class OdTrVecDevice;
class OdTrVecTtfFontsCache;
class OdTrVecVisualStyleContainer;
class OdTrVecLayerContainer;

/** \details
    <group ExRender_Windows_Classes> 
*/
class OdTrVecResourceSharingProvider : public OdTrVisSharingProviderClient
                                     , public OdDrawOrderBaseDeviceMCCallback
{
  public:
    // Processing device bit sets
    struct DeviceBitSet
    {
      // Max bit for current OS
      static const OdUInt32 g_nMaxBit;
      static const OdUInt32 g_nBitSys;
      // Check does resource attached to device
      static bool isAttached(const OdDrawOrderMetafileBase &bitSet, const OdTrVecDevice *pDevice);
      // Attach resource to device
      static void attach(OdDrawOrderMetafileBase &bitSet, const OdTrVecDevice *pDevice);
      // Detach resource from device
      static void detach(OdDrawOrderMetafileBase &bitSet, const OdTrVecDevice *pDevice);
      // Eliminate device from bit set, compress bit set
      static void eliminate(OdDrawOrderMetafileBase &bitSet, const OdTrVecDevice *pDevice);
      static void compress(OdDrawOrderMetafileBase &bitSet, OdUInt32 nBit);
      // Free bit set
      static void clear(OdDrawOrderMetafileBase &bitSet);
    };

    typedef void (*broadcastProc)(OdDrawOrderBaseDevice *pDevice, void *pData);
  protected:
    struct DeviceEntry
    {
      enum Flags
      {
        kConnected = (1 << 0)
      };

      OdTrVecDevice *m_pDevice;
      OdUInt8 m_flags;

      DeviceEntry() : m_pDevice(NULL), m_flags(0) { }
      DeviceEntry(OdTrVecDevice *pDevice) : m_pDevice(pDevice), m_flags(0) { }
    };
  protected:
    OdArray<DeviceEntry> m_rmDevices;
    OdUInt32 m_nSharedDevices;
    OdTrVecTtfFontsCache *m_pTtfCache;
    OdTrVecVisualStyleContainer *m_pVSCache;
    OdTrVecLayerContainer *m_pLayerCache;
  public:
    ODRX_USING_HEAP_OPERATORS(OdTrVisSharingProviderClient);

    OdTrVecResourceSharingProvider();
    ~OdTrVecResourceSharingProvider();

    static OdSmartPtr<OdTrVecResourceSharingProvider> createObject();

    // Opens resource sharing for device
    virtual void openResourceSharing(OdTrVecDevice *pDevice);
    // Close resource sharing for device
    virtual void closeResourceSharing(OdTrVecDevice *pDevice);
    // Check does resource sharing is opened for device
    bool isResourceSharingOpened(OdTrVecDevice *pDevice) const;

    // Returns true in case if multiple devices (sharing) enabled
    bool hasSharedResources() const { return m_nSharedDevices > 1; }

    // OdDrawOrderBaseDeviceMCCallback override
    virtual void onDeleteRequest(onDeleteRequestProc pReqProc, void *pData);

    // Broadcast function for all registered devices
    void broadcastFunction(const OdDrawOrderMetafileBase &bitSet, broadcastProc pProc, void *pArg) const;

    // Broadcast function for all devices
    void broadcastFunctionForAll(broadcastProc pProc, void *pArg, const OdTrVecDevice *pExclusion = NULL) const;

    // Ttf fonts cache support
    OdTrVecTtfFontsCache *ttfFontsCache(OdTrVecDevice *pDevice);

    // Visual Styles cache support
    OdTrVecVisualStyleContainer *visualStylesCache(OdTrVecDevice *pDevice);

    // Layers cache support
    OdTrVecLayerContainer *layersCache(OdTrVecDevice *pDevice);

    // Find first usage device
    OdTrVecDevice *findUsage(const OdDrawOrderMetafileBase &bitSet, OdTrVecDevice *pStartFrom = NULL) const;

    // Share metafile with secondary device (base call, will be redirected to secondary override)
    virtual bool shareMetafile(OdTrVecDevice *pRequestedFor, OdDrawOrderMetafileBase &bitSet,
                               OdTrVisMetafileId metafileId, void *pInternalDataPtr, void *pOriginatorPtr);

    // Share texture with secondary device (base call, will be redirected to secondary override)
    virtual bool shareTexture(OdTrVecDevice *pRequestedFor, OdDrawOrderMetafileBase &bitSet,
                              OdTrVisTextureId textureId, void *pInternalDataPtr, void *pOriginatorPtr);

    // Share material with secondary device (base call, will be redirected to secondary override)
    virtual bool shareMaterial(OdTrVecDevice *pRequestedFor, OdDrawOrderMetafileBase &bitSet,
                               OdTrVisMaterialId materialId, void *pInternalDataPtr, void *pOriginatorPtr);
  protected:
    // Share metafile with secondary device (should be overrided by client, do nothing by default)
    virtual bool shareMetafile(OdTrVecDevice *pRequestedFor, OdTrVecDevice *pRequestedFrom,
                               OdTrVisMetafileId metafileId, void *pInternalDataPtr, void *pOriginatorPtr);
    // Share texture with secondary device (should be overrided by client, do nothing by default)
    virtual bool shareTexture(OdTrVecDevice *pRequestedFor, OdTrVecDevice *pRequestedFrom,
                              OdTrVisTextureId textureId, void *pInternalDataPtr, void *pOriginatorPtr);
    // Share material with secondary device (should be overrided by client, do nothing by default)
    virtual bool shareMaterial(OdTrVecDevice *pRequestedFor, OdTrVecDevice *pRequestedFrom,
                               OdTrVisMaterialId materialId, void *pInternalDataPtr, void *pOriginatorPtr);

    // Low-level accessors

    // Create compatible texture for device from another exist device
    bool internalTextureProc(OdTrVecDevice *pRequestedFor, OdTrVecDevice *pRequestedFrom,
                             OdTrVisTextureId textureId, void *pInternalDataPtr, void *pOriginatorPtr,
                             OdTrVisTextureDef &resTexture, bool &bCreated);

    // Initiate conversion of textures from material for device from another exist device
    bool materialTexturesProc(OdTrVecDevice *pRequestedFor, OdTrVecDevice *pRequestedFrom,
                              OdTrVisMaterialId materialId, void *pInternalDataPtr, void *pOriginatorPtr);

    // Share Id registration between devices
    void shareIdRegistration(OdTrVecDevice *pRequestedFor, OdTrVecDevice *pRequestedFrom,
                             OdTrVisId id, OdInt32 type);
  protected: // Helpers
    OdTrVisRendition *rendition(OdTrVecDevice *pRequestFor) const;
    const OdTrVisRendition::ClientSettings &clientSettings(const OdTrVecDevice *pRequestFor) const;
    OdTrVisRenderClient *renderClient(OdTrVecDevice *pRequestFor) const;
    OdGsDevice *gsDevice(OdTrVecDevice *pRequestFor) const;
  public:
    class ResourceDependency
    {
      public:
        enum TypeOfResource
        {
          kRasterImage = 0,
          kMaterial,
          kTexture,
          kVisualStyle
        };

        virtual TypeOfResource resourceType() const = 0;

        virtual OdTrVisId resourceId() const = 0;

        virtual OdDrawOrderMetafileBase &resourceBitSet() = 0;

        static OdRxClass *desc();
      private:
        typedef OdTrVisDescStub ResDepDesc;
        static OdStaticRxObject<ResDepDesc> g_ResDepDesc;
    };
};

typedef OdSmartPtr<OdTrVecResourceSharingProvider> OdTrVecResourceSharingProviderPtr;

#define OdTrVecResourceDependency_queryX(TypeOf, BaseClass) \
  OdRxObject *queryX(const OdRxClass* pClass) const \
  { \
    if (pClass == OdTrVecResourceSharingProvider::ResourceDependency::desc()) \
      return reinterpret_cast<OdRxObject*>((OdTrVecResourceSharingProvider::ResourceDependency*)this); \
    return BaseClass::queryX(pClass); \
  } \
  TypeOfResource resourceType() const { return TypeOf; }

#include "TD_PackPop.h"

#endif // ODTRVECRESOURCESHARINGPROVIDER
