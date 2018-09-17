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
// TrVec module

#ifndef ODBASETRVECMODULE
#define ODBASETRVECMODULE

#include "TD_PackPush.h"

#include "Gs/GsBaseModule.h"
#include "../TrVisRenderClient.h"

// Forward declaration
class OdTrVectorizerModuleHost;

/** \details
    <group ExRender_Windows_Classes> 
*/
struct OdTrVectorizerPropertyResolver : public OdRxObject
{
  virtual OdTrVectorizerModuleHost *hostByPropsDict(OdRxDictionary *pInfo) const = 0;
};

/** \details
    <group ExRender_Windows_Classes> 
*/
typedef OdSmartPtr<OdTrVectorizerPropertyResolver> OdTrVectorizerPropertyResolverPtr;

/** \details
    <group ExRender_Windows_Classes> 
*/
struct OdTrVectorizerUpdateHost
{
  virtual void hostUpdate(OdGsDCRect* pRect) = 0;
};

/** \details
    Interface for communication with vectorization module side from vectorizer side.
    <group ExRender_Windows_Classes> 
*/
class OdTrVectorizerModuleHost : public OdRxObject
{
  protected:
    OdTrVisRenderClient              *m_pRenderClient;
    OdTrVectorizerPropertyResolverPtr m_pPropResolver;
  public:
    enum DeviceSetupType
    {
      kDtDefault = 0,
      kDtOnScreen,
      kDtOffScreen
    };
  public:
    OdTrVectorizerModuleHost() : m_pRenderClient(NULL) {}

    void setRenderClient(OdTrVisRenderClient *pRenderClient, OdTrVectorizerPropertyResolver *pPropsResolver)
    { m_pRenderClient = pRenderClient; m_pPropResolver = pPropsResolver; }
    OdTrVisRenderClient *renderClient() const { return m_pRenderClient; }

    virtual OdTrVisRenditionPtr createRendition() { return OdTrVisRenditionPtr(); }

    virtual void generateProperties(OdRxDictionary* /*pInfo*/) const {}

    virtual void configureForPlatform(OdTrVisRenderClient * /*pRenderClient*/, DeviceSetupType /*devType*/) {}

    virtual DeviceSetupType deviceSetupOverride() const { return kDtDefault; }

    virtual bool onRenderingBegin(OdGsDCRect* /*pUpdatedRect*/ = NULL, OdTrVectorizerUpdateHost* /*pUpdateHost*/ = NULL) { return true; }
    virtual void onRenderingEnd(OdGsDCRect* /*pUpdatedRect*/ = NULL, OdTrVectorizerUpdateHost* /*pUpdateHost*/ = NULL) {}

    virtual bool hasDirectRenderBuffer() const { return false; }
    virtual OdUInt8 *getDirectRenderBuffer(OdUInt32* /*pWidth*/, OdUInt32* /*pHeight*/, OdUInt32* /*pFormat*/ = NULL, OdUInt32* /*pAlignment*/ = NULL) { return NULL; }
};

/** \details
    <group ExRender_Windows_Classes> 
*/
typedef OdSmartPtr<OdTrVectorizerModuleHost> OdTrVectorizerModuleHostPtr;

#define ODRX_DECLARE_PROPERTY_TRV(PropName) \
  struct _##PropName##_PropDesc : public OdRxObjectImpl<OdRxPropDesc> \
  { \
    OdTrVectorizerPropertyResolverPtr m_pResolver; \
    static OdRxObjectPtr createObject(const OdTrVectorizerPropertyResolver *pResolver); \
    static inline  OdString _name(); \
    OdString name() const; \
    OdRxObjectPtr prop_get(const void* pThis) const; \
    void prop_put(void* pThis, OdRxObject* pVal); \
  };

#define ODRX_GENERATE_PROPERTY_TRV(PropName) \
  pInfo->putAt(_##PropName##_PropDesc::_name(), _##PropName##_PropDesc::createObject(m_pPropResolver));

#define ODRX_INHERIT_PROPERTIES_TRV(BASE_CLASS) \
  BASE_CLASS::generateProperties(pInfo);

#define ODRX_DEFINE_PROPERTY_METHODS_PREFIX_TRV(ClassPrefix, PropName, PropHolder, getMethodName, putMethodName, accessFn) \
  OdRxObjectPtr ClassPrefix _##PropName##_PropDesc::createObject(const OdTrVectorizerPropertyResolver *pResolver) \
  { \
    OdRxObjectPtr pThis = OdRxObjectPtr(new _##PropName##_PropDesc, kOdRxObjAttach); \
    static_cast<_##PropName##_PropDesc*>(pThis.get())->m_pResolver = pResolver; \
    return pThis; \
  } \
  inline OdString ClassPrefix _##PropName##_PropDesc::_name() \
  { \
    static OdString sName(OD_T(#PropName)); \
    return sName; \
  } \
  OdString ClassPrefix _##PropName##_PropDesc::name() const \
  { \
    return _name(); \
  } \
  OdRxObjectPtr ClassPrefix _##PropName##_PropDesc::prop_get(const void* pThis) const \
  { \
    return (OdRxObject*)OdRxVariantValue((static_cast<PropHolder*>(m_pResolver->hostByPropsDict((OdRxDictionary*)pThis)))->getMethodName()); \
  } \
  void ClassPrefix _##PropName##_PropDesc::prop_put(void* pThis, OdRxObject* pVal) \
  { \
    (static_cast<PropHolder*>(m_pResolver->hostByPropsDict((OdRxDictionary*)pThis)))->putMethodName(OdRxVariantValue(pVal)->accessFn()); \
  }

#define ODRX_DEFINE_PROPERTY_PREFIX_TRV(ClassPrefix, PropName, PropHolder, accessFn) \
  ODRX_DEFINE_PROPERTY_METHODS_PREFIX_TRV(ClassPrefix, PropName, PropHolder, get_##PropName, put_##PropName, accessFn)

/** \details
    <group ExRender_Windows_Classes> 
*/
class BaseGLES2Module : public OdGsBaseModule
{
    OdTrVectorizerPropertyResolverPtr m_pPropResolver;
  protected:
    void initApp();
    void uninitApp();

    virtual OdSmartPtr<OdGsBaseVectorizeDevice> createVectorizeDevice(OdTrVectorizerModuleHost *pModuleHost, OdTrVectorizerModuleHost::DeviceSetupType creationHint);
    virtual OdSmartPtr<OdGsViewImpl> createVectorizeView();

    virtual OdTrVectorizerPropertyResolverPtr createPropertyResolver();
    virtual void releasePropertyResolver();
};

#include "TD_PackPop.h"

#endif // ODBASETRVECMODULE
