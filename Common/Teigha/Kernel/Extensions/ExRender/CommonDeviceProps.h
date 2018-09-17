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

#ifndef __COMMON_DEVICE_PROPERTIES__
#define __COMMON_DEVICE_PROPERTIES__

#include "TD_PackPush.h"

#include "RxDispatchImpl.h"
#include "RxVariantValue.h"
#include "ExColorConverterCallback.h"

class OdGiContext;

// platform-independent properties that common for all devices

/** \details
  This class contains the platform-independent methods and properties
  common for all Vectorizer devices.
  Library: Source code provided.
  <group ExRender_Classes> 
*/
class OdCommonDeviceProps : public OdRxDispatchImpl<>
{
public:
  OdCommonDeviceProps();
  virtual ~OdCommonDeviceProps();

  ODRX_DECLARE_DYNAMIC_PROPERTY_MAP(OdCommonDeviceProps);
  
  /** \details
    Controls the DoubleBuffer flag for this Vectorizer device.
    \param doubleBuffer [in]  Controls double buffering.
  */
  void setDoubleBufferEnabled(bool doubleBuffer) { m_bDoubleBufferEnabled = doubleBuffer; }
  /** \details
    Returns the DoubleBuffer flag for this Vectorizer device.
  */
  bool doubleBufferEnabled() const { return m_bDoubleBufferEnabled; }

  /** \details
    Returns the DrawSelection flag for this Vectorizer device.
  */
  bool isDrawSelection() const { return m_bDrawSelection; }
  /** \details
    Controls the DrawSelection flag for this Vectorizer device.
    \param drawSelection [in]  Controls draw selection.
  */
  void setDrawSelection(bool drawSelection) { m_bDrawSelection = drawSelection; }
  
  /** \details
    Returns the DrawDragging flag for this Vectorizer device.
  */
  bool isDrawDragging() const { return m_bDrawDragging; }
  /** \details
    Controls the DrawDragging flag for this Vectorizer device.
    \param drawDragging [in]  Controls draw selection.
  */
  void setDrawDragging(bool drawDragging) { m_bDrawDragging = drawDragging; }

  /** \details
    Controls the BlocksCache flag for this Vectorizer device.
    \param blocksCache [in]  Controls blocks caching.
  */
  virtual void setBlocksCacheEnabled(bool blocksCache) { m_bBlocksCacheEnabled = blocksCache; }
  /** \details
    Returns the BlocksCache flag for this Vectorizer device.
  */
  virtual bool blocksCacheEnabled() const { return m_bBlocksCacheEnabled; }

  /** \details
    Controls the DynamicSubEntSel flag for this Vectorizer device.
    \param dynamicSubEntSel [in]  Controls dynamic subentities highlighting cache mode.
  */
  virtual void setDynamicSubEntHltEnabled(bool dynamicSubEntHlt) { m_bDynamicSubEntHltEnabled = dynamicSubEntHlt; }
  /** \details
    Returns the DynamicSubEntSel flag for this Vectorizer device.
  */
  virtual bool dynamicSubEntHltEnabled() const { return m_bDynamicSubEntHltEnabled; }

  /** \details
    Controls the EnableMultithread flag for this Vectorizer device.
    \param bMultithread [in]  Controls multithread regeneration.
  */
  virtual void setGsMultithreadEnabled(bool bMultithread) { SETBIT(m_nGsMultithreadProps, 1, bMultithread); }
  /** \details
    Returns the EnableMultithread flag for this Vectorizer device.
  */
  virtual bool gsMultithreadEnabled() const { return GETBIT(m_nGsMultithreadProps, 1); }

  /** \details
    Controls the ForcePartialUpdate flag for this Vectorizer device.
    \param bForcePartial [in]  Controls forcing partial update for multithread regeneration test.
  */
  virtual void setForcePartialUpdateForMt(bool bForcePartial) { SETBIT(m_nGsMultithreadProps, 2, bForcePartial); }
  /** \details
    Returns the ForcePartialUpdate flag for this Vectorizer device.
  */
  virtual bool forcePartialUpdateForMt() const { return GETBIT(m_nGsMultithreadProps, 2); }

  /** \details
    Controls the MaxRegenThreads flag for this Vectorizer device.
    \param nMaxThreads [in]  Controls maximum limit of threads for multithread regeneration.
  */
  virtual void setMaxMtRegenThreads(OdUInt16 nMaxThreads) { m_nMaxMtRegenThreads = nMaxThreads; }
  /** \details
    Returns the MaxRegenThreads flag for this Vectorizer device.
  */
  virtual OdUInt16 maxMtRegenThreads() const { return m_nMaxMtRegenThreads; }

  /** \details
    Check does MtRegen is actually enabled.
    \param pGiContext [in]  Gi context of current vectorization.
  */
  virtual bool checkMtRegenEnabled(OdGiContext *pGiContext) const;
  /** \details
    Returns count of threads for MtRegen.
    \param pGiContext [in]  Gi context of current vectorization.
  */
  virtual OdUInt16 getMtRegenThreadsCount(OdGiContext *pGiContext) const;

  /** \details
    Controls the ForcePartialUpdate flag for this Vectorizer device.
    \param bForcePartial [in]  Controls forcing partial update for multithread regeneration test.
  */
  virtual void setColorConverter(const OdColorConverterCallback *pCC) { m_pColorConverter = pCC; }
  /** \details
    Returns the ForcePartialUpdate flag for this Vectorizer device.
  */
  virtual OdColorConverterCallback *getColorConverter() const { return m_pColorConverter.get(); }

protected:
  bool                                m_bDoubleBufferEnabled;
  bool                                m_bDrawSelection;
  bool                                m_bDrawDragging;
  bool                                m_bBlocksCacheEnabled;
  bool                                m_bDynamicSubEntHltEnabled;
  OdUInt8                             m_nGsMultithreadProps;
  OdUInt16                            m_nMaxMtRegenThreads;
  mutable OdColorConverterCallbackPtr m_pColorConverter;
};

#define OD_COMMONDEVPROP_BlocksCacheEnabled_DevRedirect() \
  void setBlocksCacheEnabled(bool blocksCache) { OdCommonDeviceProps::setBlocksCacheEnabled(blocksCache); \
                                                 SETBIT(m_flags, kSupportBlocks, blocksCache); } \
  bool blocksCacheEnabled() const { return GETBIT(m_flags, kSupportBlocks); }
#define OD_COMMONDEVPROP_DynamicSubEntHltEnabled_DevRedirect() \
  void setDynamicSubEntHltEnabled(bool dynamicSubEntHlt) { OdCommonDeviceProps::setDynamicSubEntHltEnabled(dynamicSubEntHlt); \
                                                           SETBIT(m_flags, kDynamicSubhighlight, dynamicSubEntHlt); } \
  bool dynamicSubEntHltEnabled() const { return GETBIT(m_flags, kDynamicSubhighlight); }

#include "TD_PackPop.h"

#endif // __COMMON_DEVICE_PROPERTIES__
