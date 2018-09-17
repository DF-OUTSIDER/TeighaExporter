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
// GLES2 render client

#ifndef ODTRVISRENDERCLIENT
#define ODTRVISRENDERCLIENT

#include "TD_PackPush.h"

#include "TrVisDefs.h"
#include "TrVisRendition.h"
#include "RxVariantValue.h"

class OdTrVisTtfFontsCacheClient;

/** \details
    <group ExRender_Windows_Classes>
*/
class OdTrVisRenderClient
{
  public:
    enum AntiAliasingMode
    {
      kAntiAliasDisabled = 0,    // Anti-aliasing disabled.
      kAntiAliasLines = (1 << 0) // Lines anti-aliasing.
    };
    enum BlendingMode
    {
      kBlendingAlphaDefault = 0, // Alpha blending without sorting (default transparency behavior).
      kBlendingAlphaSorting,     // Alpha blending with sorting from back to front using second rendering pass.
      kBlendingMultiPassOIT,     // Weighted Blended Order-Independed Transparency using multiple rendering passes.
      kBlendingSinglePassOIT     // Weighted Blended Order-Independed Transparency using multiple draw buffers.
    };
    enum RenderBufferFormat
    {
      kRBufFmtRGBA = 0,          // RGBA direct render buffer
      kRBufFmtRGB,               // RGB direct render buffer
      kRBufFmtBGRA,              // BGRA direct render buffer
      kRBufFmtBGR                // BGR direct render buffer
    };
  public:
    // Properties accessibility
    virtual bool hasProperty(const OdChar *pPropName) const = 0;
    virtual OdRxVariantValue getProperty(const OdChar *pPropName) const = 0;
    virtual void setProperty(const OdChar *pPropName, OdRxVariantValue pValue) = 0;

    // Elementary properties
    virtual bool usePartialUpdate() const
    { // This property isn't currently configurable through properties dictionary.
      // It is comes from device specifics (on-screen devices prefer partial update availability,
      // off-screen devices typically prefer work w/o partial updates).
      //if (hasProperty(OD_T("UsePartialUpdate")))
      //  return getProperty(OD_T("UsePartialUpdate"))->getBool();
      //if (hasProperty(OD_T("ForcePartialUpdate")))
      //  return getProperty(OD_T("ForcePartialUpdate"))->getBool();
      return false;
    }
    virtual bool useCompositeMetafiles() const
    {
      if (hasProperty(OD_T("UseCompositeMetafiles")))
        return getProperty(OD_T("UseCompositeMetafiles"))->getBool();
      return false;
    }
    virtual bool useVisualStyles() const
    {
      if (hasProperty(OD_T("UseVisualStyles")))
        return getProperty(OD_T("UseVisualStyles"))->getBool();
      return false;
    }
    virtual bool useOverlays() const
    {
      if (hasProperty(OD_T("UseOverlays")))
        return getProperty(OD_T("UseOverlays"))->getBool();
      return false;
    }
    virtual bool useSceneGraph() const
    {
      if (hasProperty(OD_T("UseSceneGraph")))
        return getProperty(OD_T("UseSceneGraph"))->getBool();
      return false;
    }
    virtual OdUInt32 antiAliasingMode() const
    { // Configurable through GiContext.
      return kAntiAliasDisabled;
    }
    virtual OdUInt32 blendingMode() const
    {
      if (hasProperty(OD_T("BlendingMode")))
        return getProperty(OD_T("BlendingMode"))->getUInt32();
      return kBlendingAlphaDefault;
    }

    // Output window size
    virtual int outputWindowWidth() const = 0;
    virtual int outputWindowHeight() const = 0;

    // Direct render buffer accessibility
    virtual bool hasDirectRenderBuffer() const { return false; }
    virtual OdUInt8 *getDirectRenderBuffer(OdUInt32* /*pWidth*/, OdUInt32* /*pHeight*/, OdUInt32* /*pFormat*/ = NULL, OdUInt32* /*pAlignment*/ = NULL) { return NULL; }

    // TrueType fonts cache
    virtual const OdTrVisTtfFontsCacheClient *ttfFontsCache() const { return NULL; }
    virtual const OdTrVisSharingProviderClient *sharingProvider() const { return NULL; }

    // Rendering process handlers
    virtual void emitError(const char *pError) = 0;
    virtual void emitWarning(const char *pWarn) = 0;

    virtual ~OdTrVisRenderClient() {}
};

/** \details
    <group ExRender_Windows_Classes>
*/
class OdTrVisRenderClientPropsDic
{
  protected:
    struct DicPair
    {
      OdString m_propName;
      OdRxVariantValue m_pValue;

      DicPair() : m_pValue(false) { }
      DicPair(const OdChar *pPropName, OdRxVariantValue pValue)
        : m_propName(pPropName), m_pValue(pValue) { }
    };
    typedef OdVector<DicPair> DicPairsArray;
    DicPairsArray m_dic;
  public:
    OdTrVisRenderClientPropsDic() {}

    OdUInt32 findProp(const OdChar *pPropName) const
    {
      OdUInt32 nSearchBase = 0, nSearchCur;
      OdUInt32 nSearchLimit = m_dic.size();
      const DicPair *pDic = m_dic.getPtr();
      for ( ; nSearchLimit != 0; nSearchLimit >>= 1)
      {
        nSearchCur = nSearchBase + (nSearchLimit >> 1);
        int nCmp = pDic[nSearchCur].m_propName.compare(pPropName);
        if (!nCmp) return nSearchCur;
        if (nCmp > 0) { nSearchBase = nSearchCur + 1; nSearchLimit--; }
      }
      return 0xFFFFFFFF;
    }
    OdUInt32 numProps() const { return m_dic.size(); }
    OdRxVariantValue getPropAt(OdUInt32 nProp) const { return m_dic[nProp].m_pValue; }
    void setPropAt(OdUInt32 nProp, OdRxVariantValue pValue) { m_dic[nProp].m_pValue = pValue; }
    void killPropAt(OdUInt32 nProp) { m_dic.removeAt(nProp); }
    bool isEmpty() { return m_dic.isEmpty(); }
    void clear() { m_dic.clear(); }

    bool hasProp(const OdChar *pPropName) const { return findProp(pPropName) != 0xFFFFFFFF; }
    void setProp(const OdChar *pPropName, OdRxVariantValue pValue)
    {
      if (m_dic.isEmpty())
        m_dic.push_back(DicPair(pPropName, pValue));
      else
      {
        OdUInt32 nSearchBase = 0, nSearchCur = 0;
        OdUInt32 nSearchLimit = m_dic.size();
        const DicPair *pDic = m_dic.getPtr();
        for ( ; nSearchLimit != 0; nSearchLimit >>= 1)
        {
          nSearchCur = nSearchBase + (nSearchLimit >> 1);
          int nCmp = pDic[nSearchCur].m_propName.compare(pPropName);
          if (!nCmp) { m_dic[nSearchCur].m_pValue = pValue; return; }
          if (nCmp > 0) { nSearchBase = nSearchCur + 1; nSearchLimit--; }
        }
        if (!(pDic[nSearchCur].m_propName.compare(pPropName) < 0))
          nSearchCur++;
        m_dic.insertAt(nSearchCur, DicPair(pPropName, pValue));
      }
    }
    OdRxVariantValue getProp(const OdChar *pPropName) const
    {
      OdUInt32 nProp = findProp(pPropName);
      if (nProp == 0xFFFFFFFF) return OdRxVariantValue(false);
      return m_dic.getPtr()[nProp].m_pValue;
    }
    void killProp(const OdChar *pPropName)
    {
      OdUInt32 nProp = findProp(pPropName);
      if (nProp != 0xFFFFFFFF) killPropAt(nProp);
    }
};

/** \details
    <group ExRender_Windows_Classes>
*/
class OdTrVisRenderClientPropImpl : public OdTrVisRenderClient
{
  protected:
    OdTrVisRenderClientPropsDic m_props;
  public:
    // Properties accessibility
    virtual bool hasProperty(const OdChar *pPropName) const
    {
      return m_props.hasProp(pPropName);
    }
    virtual OdRxVariantValue getProperty(const OdChar *pPropName) const
    {
      return m_props.getProp(pPropName);
    }
    virtual void setProperty(const OdChar *pPropName, OdRxVariantValue pValue)
    {
      m_props.setProp(pPropName, pValue);
    }
};

/** \details
    <group ExRender_Windows_Classes>
*/
template <typename RCInterface = OdTrVisRenderClient>
class OdTrVisRenderClientWrapper : public RCInterface
{
  protected:
    OdTrVisRenderClient *m_pRedirection;
  public:
    OdTrVisRenderClientWrapper(OdTrVisRenderClient *pRedirection = NULL) : m_pRedirection(pRedirection) { }

    bool hasRedirection() const { return m_pRedirection != NULL; }
    void setRedirection(OdTrVisRenderClient *pRedirection) { m_pRedirection = pRedirection; }
    void resetRedirection() { setRedirection(NULL); }
    OdTrVisRenderClient *redirection() const { return m_pRedirection; }

    // Properties accessibility
    virtual bool hasProperty(const OdChar *pPropName) const
    {
      if (hasRedirection())
        return redirection()->hasProperty(pPropName);
      return false;
    }
    virtual OdRxVariantValue getProperty(const OdChar *pPropName) const
    {
      if (hasRedirection())
        return redirection()->getProperty(pPropName);
      return OdRxVariantValue(false);
    }
    virtual void setProperty(const OdChar *pPropName, OdRxVariantValue pValue)
    {
      if (hasRedirection())
        redirection()->setProperty(pPropName, pValue);
    }

    // Elementary properties
    virtual bool usePartialUpdate() const
    {
      if (hasRedirection())
        return redirection()->usePartialUpdate();
      return false;
    }
    virtual bool useCompositeMetafiles() const
    {
      if (hasRedirection())
        return redirection()->useCompositeMetafiles();
      return false;
    }
    virtual bool useVisualStyles() const
    {
      if (hasRedirection())
        return redirection()->useVisualStyles();
      return false;
    }
    virtual bool useOverlays() const
    {
      if (hasRedirection())
        return redirection()->useOverlays();
      return false;
    }
    virtual bool useSceneGraph() const
    {
      if (hasRedirection())
        return redirection()->useSceneGraph();
      return false;
    }
    virtual OdUInt32 antiAliasingMode() const
    {
      if (hasRedirection())
        return redirection()->antiAliasingMode();
      return OdTrVisRenderClient::kAntiAliasDisabled;
    }
    virtual OdUInt32 blendingMode() const
    {
      if (hasRedirection())
        return redirection()->blendingMode();
      return OdTrVisRenderClient::kBlendingAlphaDefault;
    }

    // Output window size
    virtual int outputWindowWidth() const
    {
      if (hasRedirection())
        return redirection()->outputWindowWidth();
      return 0;
    }
    virtual int outputWindowHeight() const
    {
      if (hasRedirection())
        return redirection()->outputWindowHeight();
      return 0;
    }

    // Direct render buffer accessibility
    virtual bool hasDirectRenderBuffer() const
    {
      if (hasRedirection())
        return redirection()->hasDirectRenderBuffer();
      return false;
    }
    virtual OdUInt8 *getDirectRenderBuffer(OdUInt32* pWidth, OdUInt32* pHeight, OdUInt32* pFormat = NULL, OdUInt32* pAlignment = NULL)
    {
      if (hasRedirection())
        return redirection()->getDirectRenderBuffer(pWidth, pHeight, pFormat, pAlignment);
      return NULL;
    }

    // TrueType fonts cache
    virtual const OdTrVisTtfFontsCacheClient *ttfFontsCache() const
    {
      if (hasRedirection())
        return redirection()->ttfFontsCache();
      return NULL;
    }
    virtual const OdTrVisSharingProviderClient *sharingProvider() const
    {
      if (hasRedirection())
        return redirection()->sharingProvider();
      return NULL;
    }

    // Rendering process handlers
    virtual void emitError(const char *pError)
    {
      if (hasRedirection())
        redirection()->emitError(pError);
    }
    virtual void emitWarning(const char *pWarn)
    {
      if (hasRedirection())
        redirection()->emitWarning(pWarn);
    }
};


#include "TD_PackPop.h"

#endif // ODTRVISRENDERCLIENT
