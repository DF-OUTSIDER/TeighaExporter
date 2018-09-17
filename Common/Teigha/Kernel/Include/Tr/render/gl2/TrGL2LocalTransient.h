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
// GLES2 device local metafile builder

#ifndef ODTRGL2LOCALMETAFILEBUILDER
#define ODTRGL2LOCALMETAFILEBUILDER

#include "TD_PackPush.h"

#include "TrRndLocalRenderer.h"
#include "TrVisMetafileBuilder.h"

/** \details
    <group ExRender_Windows_Classes>
*/

class OdTrRndLocalTransientManager;

/** \details
    <group ExRender_Windows_Classes>
*/
class OdTrGL2LocalTransient : public OdRxObject
{
  protected:
    enum Flags
    {
      kInvalid         = 1,
      kMViewCompatible = 2,
      kAutoRegen       = 4
    };
    OdUInt16 m_flags, m_counter;

    friend class OdTrGL2LocalTransientManagerImpl;
    void setInvalid() { SETBIT_1(m_flags, kInvalid); m_counter = 0; }
    void setValid() { SETBIT_0(m_flags, kInvalid); m_counter = 0; }
    bool isValid() const { return !GETBIT(m_flags, kInvalid); }

    void setMultiViewCompatible(bool bSet) { SETBIT(m_flags, kMViewCompatible, bSet); }
    bool isMultiViewCompatible() const { return GETBIT(m_flags, kMViewCompatible); }

    void setAutoRegen(bool bSet) { SETBIT(m_flags, kAutoRegen, bSet); }
    bool isAutoRegen() const { return GETBIT(m_flags, kAutoRegen); }

    typedef std::map<OdTrVisViewportId, OdRxObjectPtr> MetaDataMap;
    MetaDataMap m_metaData;
    OdTrRndLocalTransientManager *m_pOwner;
  public:
    OdTrGL2LocalTransient()
      : m_flags(kInvalid)
      , m_counter(0)
      , m_pOwner(NULL)
    { }
    virtual ~OdTrGL2LocalTransient() { }

    OdTrRndLocalTransientManager *owner() const { return m_pOwner; }

    void invalidate();
    void invalidate(OdTrVisViewportId vpId);

    bool isAttached(OdTrVisViewportId vpId) const;

    OdRxObject *viewportData(OdTrVisViewportId id);

    virtual void draw(OdTrVisMetafileBuilderPtr mfBuilder, const OdTrGL2LocalRenderer* pRenderer) = 0;

    typedef OdSmartPtr<OdTrGL2LocalTransient> Ptr;
};

/** \details
    <group ExRender_Windows_Classes>
*/
class OdTrRndLocalTransientManager
{
public:
  virtual ~OdTrRndLocalTransientManager() {}
  virtual void attachTransient(OdTrVisViewportId, OdTrGL2LocalTransient *) {}
  virtual void detachTransient(OdTrVisViewportId, OdTrGL2LocalTransient *) {}
  virtual void detachTransient(OdTrGL2LocalTransient *) {}

  virtual void detachAll(OdTrVisViewportId ) {}
  virtual void detachAll() {}

  virtual void invalidateTransient(OdTrGL2LocalTransient *) {}
  virtual void invalidateTransient(OdTrVisViewportId, OdTrGL2LocalTransient *) {}
  virtual void invalidate(OdTrVisViewportId vpId) {}
  virtual void invalidate() {}

  virtual bool update(OdTrGL2LocalRenderer *) { return false; }
  virtual void getExtents(OdTrVisViewportId, OdGeExtents3d &) {}

  virtual void draw(const OdTrGL2LocalRenderer *, OdTrVisViewportId) {}
  virtual bool isDrawing() const { return false; }
};

/** \details
    <group ExRender_Windows_Classes>
*/
class OdTrGL2LocalTransientManagerImpl : public OdTrRndLocalTransientManager
{
  protected:
    struct TransientSpec
    {
      OdTrGL2LocalTransient::Ptr m_pTransient;
      TransientSpec *m_pNextSpec;

      TransientSpec() : m_pNextSpec(NULL) {}
    };
    struct ViewportSpec
    {
      TransientSpec *m_pHead;
      TransientSpec *m_pTail;
      bool m_bValid;
      OdUInt32 m_nAutoRegen;
      OdGeExtents3d m_extents;

      ViewportSpec() : m_pHead(NULL), m_pTail(NULL), m_bValid(true), m_nAutoRegen(0) {}
    };
    typedef std::map<OdTrVisViewportId, ViewportSpec> ViewData;
    ViewData m_views;
    bool m_bDrawing, m_bViewsChanged;
  public:
    OdTrGL2LocalTransientManagerImpl();
    ~OdTrGL2LocalTransientManagerImpl();

    void attachTransient(OdTrVisViewportId vpId, OdTrGL2LocalTransient *pTransient);
    void detachTransient(OdTrVisViewportId vpId, OdTrGL2LocalTransient *pTransient);
    void detachTransient(OdTrGL2LocalTransient *pTransient);

    void detachAll(OdTrVisViewportId vpId);
    void detachAll();

    void invalidateTransient(OdTrGL2LocalTransient *pTransient);
    void invalidateTransient(OdTrVisViewportId vpId, OdTrGL2LocalTransient *pTransient);
    void invalidate(OdTrVisViewportId vpId);
    void invalidate();

    bool update(OdTrGL2LocalRenderer *pRenderer);
    void getExtents(OdTrVisViewportId vpId, OdGeExtents3d &extents);

    void draw(const OdTrGL2LocalRenderer *pRenderer, OdTrVisViewportId vpId);
    bool isDrawing() const { return m_bDrawing; }
};

#include "TD_PackPop.h"

#endif // ODTRGL2LOCALMETAFILEBUILDER
