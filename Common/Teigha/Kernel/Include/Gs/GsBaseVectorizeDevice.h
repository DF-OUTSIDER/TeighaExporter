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

#ifndef ODGSBASEVECTORIZEDEVICE_INC
#define ODGSBASEVECTORIZEDEVICE_INC

#include "TD_PackPush.h"
#include "Gs/GsBaseModule.h"
#include "Gs/GsDCRectArray.h"
#include "Gs/GsVectPerformance.h"
#include "Gs/GsModel.h"
#include "GsOverlayDefs.h"
#include "SlotManager.h"
#include "ThreadsCounter.h"

#include "Gs/GsUpdateManager.h"

/** \details
    This template class is a specialization of the OdArray class for ODCOLORREF objects.
*/
typedef OdArray<ODCOLORREF, OdMemoryAllocator<ODCOLORREF> > ODGSPALETTE;

class OdGiPathNode;
class OdGiTransientManager;
class OdGiSectionGeometryManager;

class OdGsBaseVectorizeView;
class OdGsViewImpl;

/** \details
  This template class is a specialization of the OdSmartPtr class for OdGiTransientManager object pointers. 
*/
typedef OdSmartPtr<OdGiTransientManager> OdGiTransientManagerPtr;

/** \details
    This class implements bitmapped GUI display windows.

    \remarks
    OdGsBaseVectorizeDevice objects own, update, and refresh one or more OdGsView objects.
    
    In addition, they responds to operating system notifications,
    such as window resize, forwarded by the graphics system client.

    Corresponding C++ library: Gs

    <group OdGs_Classes> 
*/
class GS_TOOLKIT_EXPORT OdGsBaseVectorizeDevice : public OdGsDevice
{
  friend class OdGsViewImpl;
  friend class OdGsBaseVectorizeView;

  friend class OdGsBaseModule;
  OdGsBaseModulePtr       m_pModule;
  OdSlotManager           m_slotManager;
protected:
  OdArray<OdGsViewPtr>    m_views;
  ODGSPALETTE             m_logPalette;
  ODCOLORREF              m_Background;
  OdGsDCRect              m_outputRect;
  OdSmartPtr<OdGiContext> m_pUserContext;
  OdRxObjectPtr           m_sectionGeometryMap;
  OdRxObjectPtr           m_sectionManager;
  OdGiTransientManagerPtr m_transientManager;
  OdGsModel::RenderType   m_transientRenderType;

  OdGsUpdateManagerPtr    m_pUpdateManager;

  OdUInt32                m_flags;
  enum
  {
    kFirstFlag             = 1,

    kInvalid               = kFirstFlag << 0,
    kSortRenderTypes       = kFirstFlag << 1,
    kSpatialIndexDisabled  = kFirstFlag << 2,
    kDynamicHighlight      = kFirstFlag << 3,
    kDynamicSubhighlight   = kFirstFlag << 4,
    kSupportBlocks         = kFirstFlag << 5,
    kSupportOverlays       = kFirstFlag << 6,
    kSupportComposition    = kFirstFlag << 7,
    kEnableComposition     = kFirstFlag << 8,
    kMtDisplay             = kFirstFlag << 9,
    kUseSectionGeometryMap = kFirstFlag << 10,
    kUseVpLtypeScaleMult   = kFirstFlag << 11,
    kUseVpFilterFunction   = kFirstFlag << 12,
    kCullingVolumeEnabled  = kFirstFlag << 13,
    kDisplayOffLayers      = kFirstFlag << 14,
    kSupportInteractiveView= kFirstFlag << 15,

    kLastFlag              = kSupportInteractiveView
  };

  struct GsDeviceOverlayData
  {
    OdGsDCRectArray m_invalidRects;
    GsDeviceOverlayData() {}
  };
  class GS_TOOLKIT_EXPORT GsDeviceOverlayDataContainer : public OdGsOverlayDataContainer<GsDeviceOverlayData>
  {
    public:
      void clearInvalidRects(OdUInt32 nOverlays = kGsAllOverlays);
      bool hasInvalidRects(OdUInt32 nOverlays = kGsAllOverlays) const;
      bool mergeInvalidRects(OdGsDCRect* pRect, OdUInt32 nOverlays = kGsAllOverlays) const;
      bool containsInvalidRects(const OdGsViewImpl &view, bool bIfHas = false, OdUInt32 nOverlays = kGsAllOverlays) const;
  };
  GsDeviceOverlayDataContainer m_overlayData;

  void registerOverlay(OdGsViewImpl *pView, const OdGsModel *pModel);
  void unregisterOverlay(OdGsViewImpl *pView, const OdGsModel *pModel);

  virtual void onOverlayActivated(OdGsOverlayId nOverlay);
  virtual void onOverlayDeactivated(OdGsOverlayId nOverlay);
public:
  /** \details
    Returns true in case if device in completely invalid state.
  */
  bool invalid() const { return GETBIT(m_flags, kInvalid); }
  /** \details
    Reset device completely invalid state flag.
    \param bSet [in]  New device invalid state.
  */
  void setInvalid(bool bSet) { SETBIT(m_flags, kInvalid, bSet); }
  /** \details
    Reset all invalidation device state parameters.
    \param bFlag [in]  New device validity state.
  */
  void setValid(bool bFlag);

  /** \details
    Must be overridden to return true in child class to inform base implementation that
    child class supports partial update.
  */
public:
  virtual bool supportPartialUpdate() const;

  void onViewAdded(OdGsView* pView);
public:
  ODRX_DECLARE_MEMBERS(OdGsBaseVectorizeDevice);

  ~OdGsBaseVectorizeDevice();

  OdGiContext* userGiContext() const;
  void setUserGiContext(OdGiContext* pUserGiContext);

  OdRxDictionaryPtr properties();
  OdGsBaseVectorizeDevice();

  /** \details
    Returns the height of this Device object in pixels.
  */
  int height() const;

  /** \details
    Returns the width of this Device object in pixels.
  */
  int width() const;

  void invalidate();
  void invalidate(const OdGsDCRect &screenRect);

  virtual void invalidate(OdUInt32 nOverlays);
  virtual void invalidate(OdUInt32 nOverlays, const OdGsDCRect &screenRect);
  void invalidate(const OdGsViewImpl *pView, const OdGsDCRect &screenRect);
  void invalidate(const OdGsModel *pModel, const OdGsDCRect &screenRect);

  void invalidateRegion(const OdGiPathNode& path);

  const OdGsDCRectArray& invalidRects() const;
  const OdGsDCRectArray* invalidRects(OdGsOverlayId overlayId) const;

  bool isValid() const;

  virtual void update(OdGsDCRect* pUpdatedRect);
  virtual void updateGeometry();
  virtual void updateScreen();

  void onSize(const OdGsDCRect& outputRect);

  const OdGsDCRect& outputRect() const;
  void getSize(OdGsDCRect& outputRect) const;

  void onRealizeForegroundPalette();

  void onRealizeBackgroundPalette();

  void onDisplayChange(
    int bitsPerPixel, 
    int xPixels, 
    int yPixels);

  OdGsViewPtr createView(
    const OdGsClientViewInfo* pViewInfo = 0, 
    bool enableLayerVisibilityPerView = false);

  void addView(OdGsView* pView);

  void insertView(int viewIndex, OdGsView* pView);

  bool eraseView(OdGsView* pView);

  int numViews() const;

  OdGsView* viewAt(int viewIndex);
  const OdGsView* viewAt(int viewIndex) const;
  OdGsViewImpl* viewImplAt(int viewIndex);
  const OdGsViewImpl* viewImplAt(int viewIndex) const;

  bool eraseView(int viewIndex);

  void eraseAllViews();

  bool setBackgroundColor(ODCOLORREF backgroundColor);

  ODCOLORREF getBackgroundColor();

  void setLogicalPalette(const ODCOLORREF* logicalPalette, int numColors);

  /** \details
    Currently not implemented.
  */
  void getSnapShot(OdGiRasterImagePtr &pImage, const OdGsDCRect &region);

  /** \details
    Returns the specified color in the logical palette of this Device object.
    \param colorIndex [in]  Color index.
  */
  ODCOLORREF getColor(OdUInt16 colorIndex) const;

  /** \details
    Returns background color in the logical palette of this Device object.
  */
  ODCOLORREF getPaletteBackground() const;

  /** \details
    Returns the logical palette of this Device object.
  */
  const ODCOLORREF* getPalette() const;

  /** \details
    Returns the logical palette of this Device object.
    \param numColors [out]  Returns number of colors inside logical palette.
  */
  const ODCOLORREF* getLogicalPalette(int &numColors) const;

  OdGsModelPtr createModel();
  bool isModelCompatible(OdGsModel* pModel) const;

  const OdGsView* rootView() const;

  // renderTypes support
  bool sortRenderTypes() const;
  void setSortRenderTypes(bool flag);
  virtual OdUInt32 renderTypeWeight(OdGsModel::RenderType renderType) const;
  OdGsModel::RenderType transientRenderType() const;
  void setTransientRenderType(OdGsModel::RenderType renderType);
  // overlays support
  bool supportOverlays() const;
  void setSupportOverlays(bool bFlag);
  virtual OdGsOverlayId renderTypeOverlay(OdGsModel::RenderType renderType) const;
  OdGsOverlayId gsModelOverlay(const OdGsModel *pModel) const;
  int isOverlayRequireUpdate(OdGsOverlayId overlayId) const;

  bool isSpatialIndexDisabled() const;
  void disableSpatialIndex(bool bSet);

  bool isCullingVolumeEnabled() const;
  void enableCullingVolume(bool bSet);

  bool supportDynamicHighlight() const;
  bool supportDynamicSubhighlight() const;

  bool supportBlocks() const;

  bool supportComposition() const;

  bool compositionEnabled() const;
  void enableComposition(bool bSet);

  bool isDisplayOffLayersEnabled() const;
  void enableDisplayOffLayers(bool bSet);

  bool isValidViewportId(const OdUInt32 acgiId) const
  {
    return m_slotManager.contains(acgiId);
  }

  bool useVpLtypeScaleMult() const { return GETBIT(m_flags, kUseVpLtypeScaleMult); }
  bool useVpFilterFunction() const { return GETBIT(m_flags, kUseVpFilterFunction); }

  OdGiSectionGeometryManager *getSectionGeometryManager();
  OdRxObjectPtr getSectionGeometryMap();
  void clearSectionGeometryMap();
  OdRxObject* mtServices() { return m_mtServices; }

  OdGiTransientManager *transientManager();
  void setTransientManager(OdGiTransientManager *pManager);

  virtual bool isSupportDeviceStateSaving() const;
  bool saveDeviceState(OdGsFiler *pFiler) const;
  bool loadDeviceState(OdGsFiler *pFiler);
  virtual bool saveClientDeviceState(OdGsFiler *pFiler) const;
  virtual bool loadClientDeviceState(OdGsFiler *pFiler);
  virtual bool saveLinkedDeviceState(OdGsFiler *pFiler) const;
  virtual bool loadLinkedDeviceState(OdGsFiler *pFiler);

  bool supportInteractiveViewMode() const { return GETBIT( m_flags, kSupportInteractiveView ); }
  void setSupportInteractiveViewMode( bool bSet ) { SETBIT( m_flags, kSupportInteractiveView, bSet ); }

  OdGsUpdateManagerPtr getUpdateManager( bool createIfNotExist = false );
  void setUpdateManager( OdGsUpdateManagerPtr pManager );
  OdGsUpdateManager* updateManager() { return m_pUpdateManager.get(); }

protected:
  OdRxObjectPtr m_pDeviceRef;
public:
  OdRxObjectPtr deviceSelfReference() { return m_pDeviceRef; }

protected:
  friend class DisplayScheduler;
  virtual bool supportParallelDisplay() const { return false; }
  void initMtServices();
  void viewUpdateScreen(OdGsBaseVectorizeView* pView, bool bUpdate);

public:
  GsVectPerformanceData m_vectPerfData;
private:
  OdRxObjectPtr m_mtServices;
  OdRefCounter m_vectThreadIndex;
  OdMutexPtr m_mtInvalidate;
  OdMutexPtr m_mtSectionGeometryMap;
};

inline bool OdGsBaseVectorizeDevice::sortRenderTypes() const
{
  return GETBIT(m_flags, kSortRenderTypes);
}

inline void OdGsBaseVectorizeDevice::setSortRenderTypes(bool bFlag)
{
  SETBIT(m_flags, kSortRenderTypes, bFlag);
}

inline bool OdGsBaseVectorizeDevice::supportOverlays() const
{
  return GETBIT(m_flags, kSupportOverlays);
}

inline void OdGsBaseVectorizeDevice::setSupportOverlays(bool bFlag)
{
  SETBIT(m_flags, kSupportOverlays, bFlag);
}

inline bool OdGsBaseVectorizeDevice::isSpatialIndexDisabled() const
{
  return GETBIT(m_flags, kSpatialIndexDisabled);
}

inline void OdGsBaseVectorizeDevice::disableSpatialIndex(bool bSet)
{
  SETBIT(m_flags, kSpatialIndexDisabled, bSet);
}

inline bool OdGsBaseVectorizeDevice::isCullingVolumeEnabled() const
{
  return GETBIT(m_flags, kCullingVolumeEnabled);
}

inline void OdGsBaseVectorizeDevice::enableCullingVolume(bool bSet)
{
  SETBIT(m_flags, kCullingVolumeEnabled, bSet);
}

inline bool OdGsBaseVectorizeDevice::supportDynamicHighlight() const
{
  return GETBIT(m_flags, kDynamicHighlight);
}

inline bool OdGsBaseVectorizeDevice::supportDynamicSubhighlight() const
{
  return GETBIT(m_flags, kDynamicSubhighlight);
}

inline bool OdGsBaseVectorizeDevice::supportBlocks() const
{
  return GETBIT(m_flags, kSupportBlocks);
}

inline bool OdGsBaseVectorizeDevice::supportComposition() const
{
  return GETBIT(m_flags, kSupportComposition);
}

inline bool OdGsBaseVectorizeDevice::compositionEnabled() const
{
  return GETBIT(m_flags, kEnableComposition);
}

inline void OdGsBaseVectorizeDevice::enableComposition(bool bSet)
{
  SETBIT(m_flags, kEnableComposition, bSet);
}

inline bool OdGsBaseVectorizeDevice::isDisplayOffLayersEnabled() const
{
  return GETBIT(m_flags, kDisplayOffLayers);
}

inline void OdGsBaseVectorizeDevice::enableDisplayOffLayers(bool bSet)
{
  SETBIT(m_flags, kDisplayOffLayers, bSet);
}

inline OdGsModel::RenderType OdGsBaseVectorizeDevice::transientRenderType() const
{
  return m_transientRenderType;
}

inline void OdGsBaseVectorizeDevice::setTransientRenderType(OdGsModel::RenderType renderType)
{
  m_transientRenderType = renderType;
}

inline const
OdGsDCRect& OdGsBaseVectorizeDevice::outputRect() const
{
  return m_outputRect;
}

inline
void OdGsBaseVectorizeDevice::getSize(OdGsDCRect& outputRect) const
{
  outputRect = this->outputRect();
}

inline
ODCOLORREF OdGsBaseVectorizeDevice::getColor(OdUInt16 colorIndex) const
{
  return m_logPalette[colorIndex];
}

inline
const ODCOLORREF* OdGsBaseVectorizeDevice::getPalette() const
{
  return m_logPalette.getPtr();
}

inline
const ODCOLORREF* OdGsBaseVectorizeDevice::getLogicalPalette(int &numColors) const
{
  numColors = (int)m_logPalette.size();
  return m_logPalette.getPtr();
}

inline const OdGsDCRectArray&
OdGsBaseVectorizeDevice::invalidRects() const
{ // Main overlay is always available
  return m_overlayData.getOverlayData(kGsMainOverlay)->m_invalidRects;
}

inline const OdGsDCRectArray*
OdGsBaseVectorizeDevice::invalidRects(OdGsOverlayId overlayId) const
{ const GsDeviceOverlayData *pOverlayData = m_overlayData.getOverlayData(overlayId);
  return (pOverlayData) ? &(pOverlayData->m_invalidRects) : NULL;
}

#include "TD_PackPop.h"

#endif // ODGSBASEVECTORIZEDEVICE_INC
