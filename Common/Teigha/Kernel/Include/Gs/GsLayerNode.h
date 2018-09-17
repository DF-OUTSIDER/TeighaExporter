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

#ifndef __OD_GS_LAYER_NODE__
#define __OD_GS_LAYER_NODE__

#include "TD_PackPush.h"

#include "Gs/GsNode.h"
#include "Gs/GsViewImpl.h"
#include "Gi/GiLayerTraitsData.h"
#include "ThreadsCounter.h"
#include "GsEmptyMetafileCache.h"
#include "OdVector.h"

typedef OdVector<OdGiLayerTraitsData, OdMemoryAllocator<OdGiLayerTraitsData> > OdGiLayerTraitsDataArray;

/** <group OdGs_Classes> 
*/
class GS_TOOLKIT_EXPORT OdGsLayerNode : public OdGsNode
{
  enum
  {
    kDirty     = OdGsNode::kLastFlag << 1,
    kVpDep     = OdGsNode::kLastFlag << 2,
    kVpFrozen  = OdGsNode::kLastFlag << 3,

    kLastFlag = kVpFrozen
  };
protected:
  /** \details
      No operation.
  */
  void update(OdGsUpdateContext& ctx, OdGsContainerNode* pParent,
      OdSiSpatialIndex* pParentIndex);

  /** \details
      No operation.
  */
  virtual void display(OdGsDisplayContext& ctx);

  /** \details
      No operation.
  */
  bool extents(OdGeExtents3d& ) const;

  void propagateLayerChanges(OdGsViewImpl& view);

  void updateVpFrozen(OdGsViewImpl *pView);

  OdGiLayerTraitsData m_layerTraits;
  mutable OdGiLayerTraitsDataArray m_vpLayerTraits;

  friend class OdGsBaseModel;
  OdGsLayerNode* m_pNextChanged;
public:
  ODRX_DECLARE_MEMBERS(OdGsLayerNode);

  /** \param pModel [in]  Pointer to the BaseModel object.
    \param pUnderlyingDrawable [in]  Pointer to the underlying Drawable object.
  */
  OdGsLayerNode(OdGsBaseModel* pModel, const OdGiDrawable* pUnderlyingDrawable, bool bSetGsNode = true);
  ~OdGsLayerNode();

  void destroy() {}

  OdGiLayerTraitsData& layerTraits(OdUInt32 nVpId);

  const OdGiLayerTraitsData& layerTraits(OdUInt32 nVpId) const;

  void reserveLayerTraits(OdUInt32 nVpId);

  bool isDirty() const;
  void setDirty(bool bDirty);
  bool isUpToDate(OdUInt32 nVpId) const;
  bool isValidCache(OdUInt32 nVpId) const;
  void setCacheUpToDate(OdUInt32 nVpId);
  bool isInvalidated() const;
  bool isVpDep() const;
  virtual ENodeType nodeType() const { return kLayerNode; }
  inline bool isVpFrozen() const;
  inline bool isFrozen() const;

  /** \details
      No operation.
  */
  void invalidate(OdGsContainerNode* pParent, OdGsViewImpl* pView, OdUInt32 mask);

  /** \details
  */
  virtual void update(OdGsBaseVectorizer& view);

  bool isTraitsCompatible(OdUInt32 nVpId1, OdUInt32 nVpId2) const;

  bool saveNodeState(OdGsFiler *pFiler, OdGsBaseVectorizer *pVectorizer = NULL) const;
  bool loadNodeState(OdGsFiler *pFiler, OdGsBaseVectorizer *pVectorizer = NULL);
private:
  friend class OdGsEmptyMetafileCache;
  OdGsEmptyMetafileCache m_emfCache;
  OdMutexPtr m_mt;
};

inline bool
OdGsLayerNode::isDirty() const
{
  return GETBIT(m_flags, kDirty);
}

inline void
OdGsLayerNode::setDirty(bool bDirty)
{
  SETBIT(m_flags, kDirty, bDirty);
}

inline bool OdGsLayerNode::isUpToDate(OdUInt32 nVpId) const
{
  if(isInvalidated())
    return false;
  if(isVpDep())
  {
    if(nVpId > 0 && m_vpLayerTraits.size() < nVpId)
      return false;
    return !GETBIT(layerTraits(nVpId).flags(), 0x80000000);
  }
  return !GETBIT(layerTraits(0).flags(), 0x80000000);
}

inline bool OdGsLayerNode::isInvalidated() const
{
  return (m_layerTraits.m_flags == 0xFFFFFFFF) && !m_vpLayerTraits.size();
}

inline bool OdGsLayerNode::isVpDep() const
{
  return GETBIT(m_flags, kVpDep);
}

inline OdGiLayerTraitsData& OdGsLayerNode::layerTraits(OdUInt32 nVpId)
{
  return const_cast<OdGiLayerTraitsData&>(static_cast<const OdGsLayerNode*>(this)->layerTraits(nVpId));
}

inline const OdGiLayerTraitsData& OdGsLayerNode::layerTraits(OdUInt32 nVpId) const
{
  if(isVpDep() && nVpId > 0)
  {
    ODA_ASSERT(m_vpLayerTraits.size() >= nVpId);
    return m_vpLayerTraits[--nVpId];
  }
  return m_layerTraits;
}

inline bool OdGsLayerNode::isVpFrozen() const
{
  return GETBIT(m_flags, kVpFrozen);
}

inline bool OdGsLayerNode::isFrozen() const
{
  return m_layerTraits.isFrozen();
}

/** \details
    This is a helper class to substitute OdGiLayerTraitsTaker in
    OdGsBaseModel::propagateLayerChanges() where vectorizers are not available
    to be set by setRefView(), and not actually required.
    <group OdGs_Classes>
*/

class OdGiLayerTraitsTakerView : public OdGiLayerTraitsTaker, public OdGiViewport
{
  ODRX_USING_HEAP_OPERATORS(OdGiLayerTraitsTaker);
  public:
    OdGiLayerTraitsTakerView(OdGsViewImpl& view) : m_view(view){ }

    /** OdGiViewportDraw methods **/
    OdGiViewport& viewport() const { return *const_cast<OdGiLayerTraitsTakerView*>(this); }
    OdUInt32 sequenceNumber() const { return 0; }
    bool isValidId(const OdUInt32 acgiId) const { return m_view.isValidViewportId(acgiId); }
    OdDbStub* viewportObjectId() const { return m_view.getViewportObjectId(); }

    /** OdGiViewport methods **/
    virtual OdGeMatrix3d getModelToEyeTransform() const { return getWorldToEyeTransform(); }
    virtual OdGeMatrix3d getEyeToModelTransform() const { return getEyeToWorldTransform(); }
    virtual OdGeMatrix3d getWorldToEyeTransform() const { return m_view.worldToEyeMatrix(); }
    virtual OdGeMatrix3d getEyeToWorldTransform() const { return m_view.eyeToWorldMatrix(); }
    virtual bool isPerspective() const { return m_view.isPerspective(); }
    virtual bool doPerspective(OdGePoint3d& pt) const { return m_view.doPerspectivePt(pt); }
    virtual bool doInversePerspective(OdGePoint3d& pt) const { return m_view.doInversePerspectivePt(pt); }
    virtual void getNumPixelsInUnitSquare(const OdGePoint3d& pt, OdGePoint2d& pixelDensity, bool bUsePerspective = true) const
    { m_view.getNumPixelsInUnitSquare(pt, pixelDensity, bUsePerspective); }
    virtual OdGePoint3d getCameraLocation() const { return m_view.position(); }
    virtual OdGePoint3d getCameraTarget() const { return m_view.target(); }
    virtual OdGeVector3d getCameraUpVector() const { return m_view.upVector(); }
    virtual OdGeVector3d viewDir() const { return m_view.eyeVector(); }
    virtual OdUInt32 viewportId() const { return m_view.viewportId(); }
    virtual OdInt16 acadWindowId() const
    { OdGsClientViewInfo vi;
      m_view.clientViewInfo(vi);
      return OdInt16(vi.acadWindowId);
    }
    virtual void getViewportDcCorners(OdGePoint2d& lowerLeft, OdGePoint2d& upperRight) const
    { m_view.viewportDcCorners(lowerLeft, upperRight); }
    virtual bool getFrontAndBackClipValues(bool& clipFront, bool& clipBack, double& front, double& back) const
    { clipFront = m_view.isFrontClipped();
      clipBack = m_view.isBackClipped();
      front = m_view.frontClip();
      back = m_view.backClip();
      return (clipFront || clipBack);
    }
    virtual double linetypeScaleMultiplier() const { return m_view.getLinetypeScaleMultiplier(); }
    virtual double linetypeGenerationCriteria() const { return m_view.getLinetypeGenerationCriteria(); }
    virtual bool layerVisible(OdDbStub* layerId) const { return m_view.isLayerVisible(layerId); }

    /** OdRxObject methods **/
    virtual void addRef() {}
    virtual void release() {}

    /** OdGiCommonDraw methods **/
    virtual double deviation(const OdGiDeviationType type, const OdGePoint3d& pt) const
    { return m_view.getDeviation(type, pt, true); }
    virtual OdUInt32 numberOfIsolines() const { return m_view.userGiContext()->numberOfIsolines(); }
    virtual OdGiRegenType regenType() const { return m_view.getRegenType(); }
    virtual bool regenAbort() const { return m_view.userGiContext()->regenAbort(); }
    virtual OdGiContext* context() const { return m_view.userGiContext(); }
  protected:
    OdGsViewImpl& m_view;
};

// Include inline OdGsEmptyMetafileCache methods
#ifndef __OD_GS_EMPTY_METAFILE_CACHE_INLINES_INCLUDE__
#define __OD_GS_EMPTY_METAFILE_CACHE_INLINES_INCLUDE__
#include "GsEmptyMetafileCache.h"
#endif
//

#include "TD_PackPop.h"

#endif // __OD_GS_LAYER_NODE__
