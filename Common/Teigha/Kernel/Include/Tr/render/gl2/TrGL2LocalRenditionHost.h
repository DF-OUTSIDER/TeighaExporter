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
// TrGL2 renderer local host

#ifndef ODTRGL2LOCALRENDITIONHOST
#define ODTRGL2LOCALRENDITIONHOST

#include "TD_PackPush.h"

#include "TPtr.h"
#include "TrVisRendition.h"
#include "TrGL2LocalContext.h"

// Forward declaration
class OdTrRndRenderSettingsManager;

/** \details
    <group ExRender_Windows_Classes> 
*/
class OdTrGL2LocalRenditionHost
{
  public:
    // Returns managed rendition
    virtual OdTrVisRendition *rendition() = 0;
    virtual const OdTrVisRendition *rendition() const = 0;

    // Returns render settings manager
    virtual OdTrRndRenderSettingsManager* renderSettingsManager() = 0;
    virtual const OdTrRndRenderSettingsManager* renderSettingsManager() const = 0;

    // Context data storage management
    virtual bool hasContextData() const = 0;
    virtual void processContextData(bool bStore) = 0;

    // Resource accessors
    virtual bool getMetafileDef(OdTrVisMetafileId metafileId, OdTrVisMetafileDef &mfDef) const = 0;
    virtual bool getMaterialDef(OdTrVisMaterialId materialId, OdTrVisMaterialDef &mtDef) const = 0;
};

/** \details
    <group ExRender_Windows_Classes> 
*/
struct OdTrGL2HlBranchClient : public OdRxObject
{
  virtual const void *hlBranchGetPtr(OdTrVisHlBranchId hlBranchId) const = 0;
  virtual bool hlBranchIsEmpty(const void *pHlBranch) const = 0;
  virtual bool hlBranchHasMarkers(const void *pHlBranch) const = 0;
  virtual const void *hlBranchGetChild(const void *pHlBranch, OdDbStub *pChildId) const = 0;
  virtual bool hlBranchHasMarker(const void *pHlBranch, OdTrVisGsMarker gsMarker) const = 0;
};

/** \details
    <group ExRender_Windows_Classes>
*/
typedef OdSmartPtr<OdTrGL2HlBranchClient> OdTrGL2HlBranchClientPtr;

/** \details
    <group ExRender_Windows_Classes> 
*/
class OdTrGL2LocalRenditionClient : public OdRxObject
{
  public:
    // Returns local context
    virtual OdTrGL2LocalContextPtr createLocalContext(OdTrVisRenderClient *pDevice) = 0;

    // Returns optional highlighting branches interface (if true, rendition will be initialized as Gs-based)
    virtual OdTrGL2HlBranchClientPtr hlBranchClient() { return OdTrGL2HlBranchClientPtr(); }

    // Returns optional resource sharing provider (for multiple devices session sharing)
    virtual OdTrVisSharingProviderClientPtr createSharingProvider() { return OdTrVisSharingProviderClientPtr(); }

    // Predefined textures generation
    virtual bool generateLinetypeTexture(OdPs::LineType /*ltpId*/, OdUInt8Array& /*outPixels*/, OdUInt8 /*forePixVal*/) const { return false; }
    virtual bool generateFilltypeTexture(OdPs::FillStyle /*fsId*/, OdUInt8Array& /*outPixels*/, OdUInt32 /*dimX*/, OdUInt32 /*dimY*/, OdUInt8 /*forePixVal*/) const { return false; }
};

/** \details
    <group ExRender_Windows_Classes>
*/
typedef OdSmartPtr<OdTrGL2LocalRenditionClient> OdTrGL2LocalRenditionClientPtr;

/** \details
    <group ExRender_Windows_Classes>
*/
class OdTrGL2LocalRenditionHostTPtrAdaptor
{
  public:
    static void addRef(OdTrGL2LocalRenditionHost* pObj)  { if (pObj) pObj->rendition()->addRef(); }
    static void release(OdTrGL2LocalRenditionHost* pObj) { if (pObj) pObj->rendition()->release(); }
};

/** \details
    <group ExRender_Windows_Classes>
*/
class OdTrGL2LocalRenditionHostPtr : public TPtr<OdTrGL2LocalRenditionHost, OdTrGL2LocalRenditionHostTPtrAdaptor>
{
  public:
    OdTrGL2LocalRenditionHostPtr()
      : TPtr<OdTrGL2LocalRenditionHost, OdTrGL2LocalRenditionHostTPtrAdaptor>() {}
    OdTrGL2LocalRenditionHostPtr(const OdTrGL2LocalRenditionHostPtr& ref)
      : TPtr<OdTrGL2LocalRenditionHost, OdTrGL2LocalRenditionHostTPtrAdaptor>(ref) {}
    OdTrGL2LocalRenditionHostPtr(const OdTrGL2LocalRenditionHost* pObj, bool bAttach = false)
      : TPtr<OdTrGL2LocalRenditionHost, OdTrGL2LocalRenditionHostTPtrAdaptor>(pObj, bAttach) {}
    OdTrGL2LocalRenditionHostPtr(const OdTrVisRendition *pRendition);
    OdTrGL2LocalRenditionHostPtr &operator =(OdTrGL2LocalRenditionHost *pObj)
    { return static_cast<OdTrGL2LocalRenditionHostPtr&>(TPtr<OdTrGL2LocalRenditionHost, OdTrGL2LocalRenditionHostTPtrAdaptor>::operator =(pObj)); }
    OdTrGL2LocalRenditionHostPtr &operator =(const OdTrGL2LocalRenditionHostPtr& ref)
    { return static_cast<OdTrGL2LocalRenditionHostPtr&>(TPtr<OdTrGL2LocalRenditionHost, OdTrGL2LocalRenditionHostTPtrAdaptor>::operator =(ref)); }
    OdTrGL2LocalRenditionHostPtr &operator =(OdTrVisRendition *pObj);
    void release() { *this = TPtr<OdTrGL2LocalRenditionHost, OdTrGL2LocalRenditionHostTPtrAdaptor>::operator =(NULL); }
    OdTrGL2LocalRenditionHostPtr &create(OdTrGL2LocalRenditionClient *pClient);
    OdUInt32 checkRendererVersion(OdTrVisRenderClient *pRenderClient, OdUInt32 nCheckVersion) const;
};

#include "TD_PackPop.h"

#endif // ODTRGL2LOCALRENDITIONHOST
