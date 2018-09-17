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
// Interfaces for extension rendering modules

// Composite view

#ifndef EXWINGSCOMPOSITEMODULEINTERFACE
#define EXWINGSCOMPOSITEMODULEINTERFACE

#include "TD_PackPush.h"

#include "Gs/GsBaseMaterialView.h"

struct ExWinCompositeBitBltOp;

// Main interface for composite client device. Contains abstract methods acessible by composite device.

/** \details
    <group ExRender_Windows_Classes> 
*/
class ExGsCompositeClientDeviceIface
{
  public:
    ExGsCompositeClientDeviceIface() { }
    virtual ~ExGsCompositeClientDeviceIface() { }

    // Moved to GsBaseVectorizeDevice to improve effectivity and avoid rtti usage.
    //virtual bool compositionEnabled() const = 0;
    //virtual void setCompositionEnabled() = 0;

    // Partial update state
    virtual bool isCompositionUpdateEnabled() const = 0;
    virtual void enableCompositionUpdate(bool bSet) = 0;

    // Redirection mode state
    virtual bool isRedirectionModeEnabled() const = 0;
    virtual void enableRedirectionMode(bool bSet) = 0;

    // Requirements
    // Hidden window necessary (draw context can't be accessed directly)
    virtual bool requireWindow() const { return false; }
    // Resturns true if device is mt-aware in composition mode (only for double buffered)
    virtual bool mtCompositeAware() const { return false; }

    // Win32 BitBlt
    virtual void makeBitBltOp(const ExWinCompositeBitBltOp & /*bitBltOp*/) { }
};

// Composition-aware device must inherit this interface

/** \details
    <group ExRender_Windows_Classes> 
*/
template <class baseVectDev>
class ExGsCompositeClientDevice : public baseVectDev, public ExGsCompositeClientDeviceIface
{
  protected:
    enum ClientCompositionFlags
    {
      kCompositionUpdate    = 1 << 0,
      kRedirectionMode      = 1 << 1
    };

    OdUInt32 m_clientCompFlags;
  public:
    ExGsCompositeClientDevice()
      : baseVectDev()
      , ExGsCompositeClientDeviceIface()
      , m_clientCompFlags(0)
    {
      SETBIT(baseVectDev::m_flags, baseVectDev::kSupportComposition, true);
    }

    bool isCompositionUpdateEnabled() const
    {
      return GETBIT(m_clientCompFlags, kCompositionUpdate);
    }
    void enableCompositionUpdate(bool bSet)
    {
      SETBIT(m_clientCompFlags, kCompositionUpdate, bSet);
    }

    bool isRedirectionModeEnabled() const
    {
      return GETBIT(m_clientCompFlags, kRedirectionMode);
    }
    void enableRedirectionMode(bool bSet)
    {
      SETBIT(m_clientCompFlags, kRedirectionMode, bSet);
    }

    bool compositionOrRedirectionEnabled() const
    {
      return baseVectDev::compositionEnabled() || isRedirectionModeEnabled();
    }

    // this overrides required for GCC template build rules
    bool supportPartialUpdate() const
    {
      return baseVectDev::supportPartialUpdate();
    }
    void updateGeometry()
    {
      if (baseVectDev::compositionEnabled() && !isCompositionUpdateEnabled())
        return;
      baseVectDev::updateGeometry();
    }
    void updateScreen()
    {
      //if (baseVectDev::compositionEnabled() && isCompositionUpdateEnabled())
      //  return;
      baseVectDev::updateScreen();
    }

    // This is our main change for support composition. We need separate partial update onto two steps, because
    // invalidation data after updateExtents must be propagated into all linked devices. After that updateScreen
    // could be called for all dependent devices in same context.
    virtual void update(OdGsDCRect* pUpdatedRect)
    {
      if (baseVectDev::compositionEnabled() && supportPartialUpdate())
      {
        if (isCompositionUpdateEnabled())
        {
          //OdSaveState<OdGsDCRectArray> _ss1(baseVectDev::m_invalidRects);
          //OdSaveStateFlag<OdUInt32, baseVectDev::kInvalid> _ss2(baseVectDev::m_flags);
          //OdSaveState<OdUInt32> _ss3(baseVectDev::m_invalidOverlays);
          //baseVectDev::update(NULL);
          updateGeometry();
        }
        if (!isCompositionUpdateEnabled())
          //updateScreen();
          baseVectDev::update(pUpdatedRect);
      }
      else
      {
        baseVectDev::update(pUpdatedRect);
      }
    }
};

#include "TD_PackPop.h"

#endif // EXWINGSCOMPOSITEMODULEINTERFACE
