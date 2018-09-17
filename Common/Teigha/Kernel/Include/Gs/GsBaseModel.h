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

#ifndef __ODGSMODELIMPL_H__
#define __ODGSMODELIMPL_H__

#define OD_MATERIAL_CACHE_SUPPORTED
class OdGsNode;
class OdGsViewImpl;

#include "TD_PackPush.h"

#include "OdList.h"
#include "UInt8Array.h"
#include "UInt32Array.h"
#include "DbStubPtrArray.h"
#include "StaticRxObject.h"
#include "OdVector.h"

#include "Gi/GiDrawable.h"
#include "Gi/GiCommonDraw.h"
#include "Gi/GiPathNode.h"

#include "Gs/GsExport.h"
#include "Gs/GsModel.h"
#include "Gs/GsViewProps.h"
#include "Gs/GsExtAccum.h"
#include "Gs/GsMaterialCache.h"
#include "Gs/GsViewLocalId.h"
#include "Gs/GsHighlightData.h"

#define CACHE_STATISTICS

class OdGsLayerNode;
class OdGsBaseVectorizeDevice;
class OdGsBaseVectorizer;
class OdGsBaseModule;

class OdGsBaseModelImpl;
class OdGsEntityNode;
class OdGsContainerNode;
class OdGsLightNode;

enum ENodeType
{
  kBlockNode,
  kContainerNode,
  kLayerNode,
  kEntityNode,
  kMaterialNode,
  kLastNodeType, //number of types enumerated above
  kLightNode,
  kBlockReferenceNode,
  kMInsertBlockNode,
};

/** \details
    GsBaseModel reactor.

    Corresponding C++ library: TD_Gs
    <group OdGs_Classes> 
*/
class OdGsBaseModelReactor : public OdGsModelReactor
{
  public:
    virtual bool onHighlight(OdGsModel *pModel, const OdGiPathNode& path, bool bDoIt, const OdGsView* pView) { return true; }

    virtual bool onInvalidate(OdGsModel *pModel, OdGsModel::InvalidationHint hint) { return true; }
    virtual bool onInvalidate(OdGsModel *pModel, OdGsView* pView) { return true; }

    enum ModelProperty
    {
      kModelTransform = 0,
      kModelRenderType,
      kModelBackground,
      kModelVisualStyle,
      kModelSectioning,

      kNumModelProps
    };
    virtual bool onPropertyModified(OdGsModel *pModel, ModelProperty nProp) { return true; }
};

/** \details
    This class is the base class for custom GsBaseModel objects which coordinate custom caching support 
    for Teigha vectorization applications.

    \remarks
    Clients should derive their custom model classes from this class.

    Corresponding C++ library: TD_Gs
    <group OdGs_Classes> 
*/
class GS_TOOLKIT_EXPORT OdGsBaseModel : public OdGsModel
{
protected:
  OdGiOpenDrawableFn m_openDrawableFn;
  OdGsNode*          m_aNodes[kLastNodeType];

  friend class OdGsNode;
  friend class OdGsViewImpl;
  friend class OdGsBaseVectorizer;
  friend class OdGsBaseVectorizeDevice;

  /** \details
    Adds the specified Node object to this BaseModel object.
    \param pNode [in]  Pointer to the Node to be added.  
  */
  void addNode(OdGsNode* pNode);

public:
  virtual void invalidateEntRect(OdGiDrawable* pDrawable, OdGiDrawable* pParent, bool bForceIfNoExtents = false);
  virtual void invalidateEntRect(OdGsEntityNode* pNode, OdGsContainerNode* pParent, bool bForceIfNoExtents);

#if defined(CACHE_STATISTICS)
  unsigned m_nMfCached;
  unsigned m_nMfReused;
#endif//CACHE_STATISTICS
protected:
  enum GsModelFlags
  {
    kCheckFaded      = (1 << 0),
    kEnableLinetypes = (1 << 1),
    kModelSelectable = (1 << 2),
    kLightsInBlocks  = (1 << 3),
    kSkipViewClip    = (1 << 4),
    kIdentityXform   = (1 << 5),
    kVectorizing     = (1 << 6)
  };
  OdUInt32 m_gsModelFlags;
public:
  bool checkFaded() const { return GETBIT(m_gsModelFlags, kCheckFaded); }
  void setCheckFaded(bool bOn) { SETBIT(m_gsModelFlags, kCheckFaded, bOn); }
  void setVectorizing() { SETBIT_1(m_gsModelFlags, kVectorizing); }
  void resetVectorizing() { SETBIT_0(m_gsModelFlags, kVectorizing); }
#ifdef ODA_DIAGNOSTICS
  OdRefCounter m_nNodes; //number of OdGsNodes allocated within the model (not necessary in 
                //m_pNodes list _directly_, can be owned by insert/block nodes).
#endif

  typedef std::pair<OdGsViewImpl*, OdUInt32> ViewRef;
  OdArray<ViewRef, OdMemoryAllocator<ViewRef> > m_views;
  typedef std::pair<const OdGsBaseModule*, OdUInt32> ModuleRef;
  OdArray<ModuleRef, OdMemoryAllocator<ModuleRef> > m_modules;
  const ViewPropsArray& getViewProps() const { return m_viewProps; }
  OdGsViewImpl *refView() const;
  OdGsBaseVectorizeDevice *refDevice() const;
  OdUInt32 refModulesCount() const { return m_modules.size(); }
#ifdef ODGS_LOCAL_IDS
  virtual void attachLocalViewportId(OdGsViewImpl* pView, OdGsViewImpl* pFrom = NULL);
  virtual OdUInt32 getLocalViewportId(OdGsViewImpl* pView);
  virtual OdUInt32 getMaxLocalViewportId() const;
#endif

  /** \details
    For internal needs. Do not use!
  */
  bool invalidVp(unsigned int viewportId) const
  {
    if(m_viewProps.size()>viewportId)
      return m_viewProps[viewportId].isInvalid();
    return true;
  }

  OdGsBaseModelImpl* impl() { return m_impl; }
  void invalidateRegenDrawBlocks(OdGsViewImpl& view, const OdDbStub* layoutId);
  void invalidateSectionableBlocks();

  struct SectioningSettings
  {
    bool             m_bEnabled;
    bool             m_bTopSet;
    bool             m_bBottomSet;
    bool             m_bVisualStyle;
    OdGePoint3dArray m_points;
    OdGeVector3d     m_upVector;
    double           m_dTop;
    double           m_dBottom;
    OdDbStub        *m_visualStyle;
    SectioningSettings()
      : m_bEnabled(false), m_bTopSet(false), m_bBottomSet(false), m_bVisualStyle(false)
      , m_dTop(0.0), m_dBottom(0.0), m_visualStyle(NULL) { }
    bool isEnabled() const { return m_bEnabled && !m_points.isEmpty(); }
  };

protected:
  void addViewRef(OdGsViewImpl* pView);
  void removeViewRef(OdGsViewImpl* pView);

  /** \details
    For internal needs. Do not use!
  */
  void setInvalidVp(unsigned int viewportId, bool val)
  {
    if(m_viewProps.size()>viewportId)
      m_viewProps[viewportId].setInvalid(val);
  }
  virtual void clearChangedLayersList();

protected:

  OdArray<ViewProps>           m_viewProps;
  OdUInt32                     m_nLayers;
  OdGsLayerNode*               m_pFirstChangedLayer;

  OdGsView::RenderMode         m_renderModeOverride;
  OdGeMatrix3d                 m_xForm;
  OdDbStub*                    m_modelBackground;
  OdDbStub*                    m_modelVisualStyle;
  OdGiDrawablePtr              m_pTransVisualStyle;

  RenderType                   m_renderType;
  // Reserved pointer for internal use (binary incompatible changes), do not delete.
  OdRxObjectPtr                m_pGsReserved;

  // Material cache support
  OdGsMaterialCachePtr         m_pMaterialCache;

  OdGsBaseModelImpl*           m_impl;

  SectioningSettings           m_sectioning;

#ifdef ODGS_LOCAL_IDS
  OdGsBaseModelLocalIdsPtr     m_pViewIds;
#endif

  OdVector<OdGsModelReactor*, OdMemoryAllocator<OdGsModelReactor*> > m_modelReactors;
  OdVector<OdGsBaseModelReactor*, OdMemoryAllocator<OdGsBaseModelReactor*> > m_baseModelReactors;

protected:

  OdGsBaseModel();
  ~OdGsBaseModel();

public:
  /** \details
    Returns the OdGsNode object associated with the specified Drawable.
    
    \param pDrawable [in]  Pointer to the Drawable object.
    \remarks
    If no Node object is associated with the Drawable object, one is created.
  */
  virtual OdGsNode* gsNode(OdGiDrawable* pDrawable);

  /** \details
    Removes the associations between the specified Node object and this BaseModel object.
    \param pNode [in]  Pointer to the Node object. 
  */
  virtual void detach(OdGsNode* pNode);

  /** \details
    Removes all Node objects from this BaseModel object.
  */
  virtual void detachAll();

  void detachAllFromDb(const OdDbBaseDatabase* pDb);

  /** \details
    Opens the specified object.
    \param objectID [in]  Object ID of the Drawable object to be opened.
    
    \returns
    Returns a SmartPointer to the opened object.
  */
  OdGiDrawablePtr open(OdDbStub* objectId);

  void setOpenDrawableFn(OdGiOpenDrawableFn openDrawableFn);
  OdGiOpenDrawableFn openDrawableFn() const;

  enum AdditionMode
  {
    kAddDrawable = 0, // New drawable added
    kUneraseDrawable  // Exist drawable unerased
  };

  void onAdded(OdGiDrawable* pAdded, OdGiDrawable* pParent);
  void onAdded(OdGiDrawable* pAdded, OdDbStub* parentID);
  virtual void onAddedImpl(OdGiDrawable* pAdded, OdGiDrawable* pParent, AdditionMode additionMode = kAddDrawable);

  void onModified(OdGiDrawable* pModified, OdGiDrawable* pParent);
  void onModified(OdGiDrawable* pModified, OdDbStub* parentID);
  virtual void onModifiedImpl(OdGiDrawable* pModified, OdGiDrawable* pParent);

  void onErased(OdGiDrawable* pErased, OdGiDrawable* pParent);
  void onErased(OdGiDrawable* pErased, OdDbStub* parentID);
  virtual void onErasedImpl(OdGiDrawable* pErased, OdGiDrawable* pParent);

  void onUnerased(OdGiDrawable* pUnerased, OdGiDrawable* pParent);
  void onUnerased(OdGiDrawable* pUnerased, OdDbStub* parentID);

  void invalidate(InvalidationHint hint);
  void invalidate(OdGsView* pView);
  void invalidateVisible( OdGsDevice* pDevice );
  /** \param view [in]  BaseVectorizeView object for which data are to be invalidated.
    \param mask [in]  Mask.
  */
  void invalidate(OdGsViewImpl& view, OdUInt32 mask);

  /** \details
    Returns the view properties that have changed for the specified Viewport
    
    \param viewportId [in]  Viewport ID. 
  */
  OdUInt32 viewChanges(OdUInt32 viewportId) const;
  /** \details
    Returns the size of the ViewProperties array for this BaseModel object.
  */
  OdUInt32 numViewProps() const;
  
  /** \details
    Returns a reference to the the ViewProp structure for the specified Viewport.
    
    \param viewportId [in]  Viewport ID. 
  */
  const ViewProps& viewProps(OdUInt32 viewportId) const;
  
  /** \details
    Updates the ViewProps structure for the specified Viewport.
    \param viewport [in]  Viewport.
  */
  virtual void updateViewProps(const OdGsViewImpl& viewport);

  virtual bool isViewPropsValid(OdUInt32 viewportId) const;

  virtual OdGsLayerNode* gsLayerNode(OdDbStub* layerId, const OdGsBaseVectorizer* pView);

  virtual void propagateLayerChanges(OdGsBaseVectorizeDevice& device);

  void setTransform(const OdGeMatrix3d& xForm);

  OdGeMatrix3d transform() const;

  bool isIdentityTransform() const;

  OdGsView::RenderMode renderModeOverride() const;

  void setRenderModeOverride(OdGsView::RenderMode mode = OdGsView::kNone);

  void highlight(const OdGiPathNode& path, bool bDoIt = true, const OdGsView* pView = NULL);
  virtual void highlightImpl(const OdGiPathNode& path, bool bDoIt = true, const OdGsView* pView = NULL);

  // Material cache support
  OdGsMaterialCache* materialCache() { return m_pMaterialCache.get(); }

  void setAdditionMode(AdditionMode mode);
  AdditionMode additionMode() const;

  void setRenderType(RenderType renderType);
  RenderType renderType() const;

  void setBackground(OdDbStub *backgroundId);
  OdDbStub *background() const;

  void setVisualStyle(OdDbStub *visualStyleId);
  OdDbStub *visualStyle() const;
  void setVisualStyle(const OdGiVisualStyle &visualStyle);
  bool visualStyle(OdGiVisualStyle &visualStyle) const;
  const OdGiDrawable *visualStyleDrawable() const;

  virtual bool makeStock(OdDbStub* layoutId);
  virtual void releaseStock(OdDbStub* layoutId);

  bool hasChangedLayers() const { return m_pFirstChangedLayer != NULL; }

  void addModelReactor(OdGsModelReactor *pReactor);
  void removeModelReactor(OdGsModelReactor *pReactor);

  void setEnableSectioning(bool bEnable);
  bool isSectioningEnabled() const;
  bool setSectioning(const OdGePoint3dArray &points, const OdGeVector3d &upVector);
  bool setSectioning(const OdGePoint3dArray &points, const OdGeVector3d &upVector,
                     double dTop, double dBottom);
  void setSectioningVisualStyle(OdDbStub *visualStyleId);
  const SectioningSettings &getSectioning() const { return m_sectioning; }

  void setViewClippingOverride(bool bEnable);
  bool isViewClippingOverride() const;

  void setEnableLinetypes(bool bEnable);
  bool isLinetypesEnabled() const;

  void setSelectable(bool bEnable);
  bool isSelectable() const;

  void setEnableLightsInBlocks(bool bEnable);
  bool isLightsInBlocksEnabled() const;

  virtual bool saveModelState(OdGsFiler *pFiler, OdGsBaseVectorizer *pVectorizer = NULL) const;
  virtual bool loadModelState(OdGsFiler *pFiler, OdGsBaseVectorizer *pVectorizer = NULL);
  virtual bool saveClientModelState(OdGsFiler *pFiler) const;
  virtual bool loadClientModelState(OdGsFiler *pFiler);
  bool postprocessModelLoading(OdGsFiler *pFiler);
protected:
  void onModifiedNode(OdGsNode* pCache, OdGiDrawable* pDrawable, OdGiDrawable* pParent);
  void onModifiedDrawable(OdGiDrawable* pDrawable, OdGiDrawable* pParent);
  void onPropertyModifiedImpl(OdGsBaseModelReactor::ModelProperty nProp, bool &bReturnValue);
  virtual bool onPropertyModified(OdGsBaseModelReactor::ModelProperty nProp);
public:
  virtual OdGsNode* newNode(ENodeType ntp, const OdGiDrawable* drawable, bool bSetGsNode);
};

inline OdGsView::RenderMode OdGsBaseModel::renderModeOverride() const
{
  return m_renderModeOverride;
}

inline bool OdGsBaseModel::isViewClippingOverride() const
{
  return GETBIT(m_gsModelFlags, kSkipViewClip);
}

inline OdGiDrawablePtr OdGsBaseModel::open(OdDbStub* objectId) 
{
  if(m_openDrawableFn)
    return m_openDrawableFn(objectId);
  //ODA_ASSERT_ONCE(objectId==0);
  return OdGiDrawablePtr();
}

inline OdUInt32 OdGsBaseModel::numViewProps() const
{
  return m_viewProps.size();
}

inline const ViewProps& OdGsBaseModel::viewProps(OdUInt32 viewportId) const
{
  ODA_ASSERT(m_viewProps.size() > viewportId);
  return m_viewProps[viewportId];
}

inline OdUInt32 OdGsBaseModel::viewChanges(OdUInt32 viewportId) const
{
  return viewProps(viewportId).m_nViewChanges;
}

inline OdGsViewImpl *OdGsBaseModel::refView() const
{
  if (m_views.empty())
    return NULL;
  return m_views.first().first;
}

inline OdGsModel::RenderType OdGsBaseModel::renderType() const
{
  return m_renderType;
}

inline bool OdGsBaseModel::isIdentityTransform() const
{
  return GETBIT(m_gsModelFlags, kIdentityXform);
}

inline const OdGiDrawable *OdGsBaseModel::visualStyleDrawable() const
{
  return m_pTransVisualStyle.get();
}

#include "TD_PackPop.h"

#endif // __ODGSMODELIMPL_H__
