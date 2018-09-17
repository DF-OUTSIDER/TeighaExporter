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

#ifndef __OD_GS_NODE__
#define __OD_GS_NODE__

#include "TD_PackPush.h"

#include "Gs/GsBaseModel.h"
#include "ChunkAllocator.h"

class OdGsContainerNode;
class OdGsLayerNode;
	
class OdGsBaseModel;
class OdGsUpdateContext;
class OdGsDisplayContext;
class OdSiSpatialIndex;
class OdGsUpdateManager;

/** \details
    This class is the base class for custom GsNode objects which implement custom caching support 
    for Teigha vectorization applications.

    \remarks
    Cached rendering information may be stored in a custom GsNode descendant
    for each drawable in an OdDbDatabase object.
    
    This cached information can be fed into the Teigha vectorization
    framework during a redraw instead of making the usual calls to subWorldDraw() and 
    subViewportDraw(), thereby greatly improving the speed of redraws.

    \remarks
    Clients should derive their custom node classes from this class.

    Corresponding C++ library: TD_Gs
    <group OdGs_Classes> 
*/
class GS_TOOLKIT_EXPORT ODRX_ABSTRACT OdGsNode : public OdGsCache
{
  friend class OdGsBaseModel;
  friend class OdGsViewImpl;
protected:
  mutable OdGsNode* m_pPrev;
  mutable OdGsNode* m_pNext;

  GS_STATIC_EXPORT static AllocatorArray s_aAlloc;

  OdGsBaseModel*    m_pModel;
  void*             m_underlyingDrawable;

  enum
  {
    kPersistent   = 0x00000001,
    kContainer    = 0x00000002,
    kHLT          = 0x00000004, // Highlight
    kHLT_All      = 0x00000008, // Highlight whole branch
    kSyncDrawable = 0x00000010,
    kHidden       = 0x00000020,
    kSelectHidden = 0x00000040,
    kLastFlag     = kSelectHidden
  };

  mutable OdUInt32  m_flags;

  enum { kInvalidateVp = 0x80000000 };
public:
  inline bool invalidVp() const { return GETBIT(m_flags, kInvalidateVp); }
protected:
  inline void setInvalidVp(bool val) { SETBIT(m_flags, kInvalidateVp, val); }

  /** \details
    Removes the associations between the underlying Drawable object and this GsNode object.
  */
  void clearDrawable();

public:
  void setToDrawable(const OdGiDrawable* pUnderlyingDrawable);

public:
  ODRX_DECLARE_MEMBERS(OdGsNode);

  void* operator new(size_t);
  void operator delete(void *);

  /** \remarks
      OdGsNode objects perform no reference counting. This function does nothing but return.
  */
  void addRef();

  /** \remarks
      OdGsNode objects perform no reference counting. This function does nothing but return.
  */
  void release();

  /** \param pModel [in]  Pointer to the BaseModel object.
    \param pUnderlyingDrawable [in]  Pointer to the underlying Drawable object.
  */
  OdGsNode(OdGsBaseModel* pModel, const OdGiDrawable* pUnderlyingDrawable);
protected:
  OdGsNode(const OdGsNode& c);
public:
  virtual ~OdGsNode();

  /** \details
    Pointer to the BaseModel object associated with this Node object.
  */
  OdGsBaseModel* baseModel() const;

  OdGsModel* model() const;

  /** \details
    Returns true if and only if this Node object serves as a container for other Node objects.
  */
  bool isContainer() const;

  virtual ENodeType nodeType() const = 0;

  bool isSyncDrawable() const;

  /** \details
    Invalidates the cached data within this object.
    
    \param pParent [in]  Pointer to the ContainerNode object for this Node object.
    \param pView [in]  Pointer to the VectorizeView object for which data are to be invalidated.
    \param mask [in]  Mask.
    \remarks
    The cached data will be regenerated the next time this Node object is accessed.
  */
  virtual void invalidate(OdGsContainerNode* pParent, OdGsViewImpl* pView, OdUInt32 mask) = 0;

  /** \details
    Returns the underlying Drawable object associated with this Node object.
  */
  OdGiDrawablePtr underlyingDrawable() const;

  /** \details
    Returns ObjectID of the underlying Drawable object associated with this Node object.
  */
  OdDbStub* underlyingDrawableId() const;

  /** \details
    Updates the cached data within this Node object for the specified VectorizeView and ContainerNode objects. 
    \param pParent [in]  Pointer to the ContainerNode object for this Node object.
    \param view [in]  VectorizeView object for which data are to be updated.
  */
  virtual void update(OdGsUpdateContext& ctx, OdGsContainerNode* pParent,
      OdSiSpatialIndex* pParentIndex) = 0;

  /** \details
    Updates the cached data if Node object is visible in specified view
    \param pViewImpl [in] pointer to the View Implementation
  */
  virtual void updateVisible( OdGsViewImpl* pViewImpl ) {};

  /** \details
    Paints the specified view with the cached data associated with this Node object.
    \param view [in]  View to be painted.
  */
  virtual void display(OdGsDisplayContext& ctx) = 0;

  /** \details
    Invalidates parts of cache depended on layers modified since last regeneration.
    
    \remarks
    For internal use only.
    Returns this if and only if the this node is one of modified layers.
  */
  virtual void propagateLayerChanges(OdGsViewImpl& view) = 0;

  virtual void highlight(bool bDoIt, bool bWholeBranch);

  bool isHighlighted() const;
  bool isHighlightedAll() const;

  virtual void destroy() = 0;

  void hide(bool bDoIt);
  bool isHidden() const;

  void setSelectableIfHidden(bool bSelectable);
  bool isSelectableIfHidden() const;

  virtual bool saveNodeState(OdGsFiler *pFiler, OdGsBaseVectorizer *pVectorizer = NULL) const;
  virtual bool loadNodeState(OdGsFiler *pFiler, OdGsBaseVectorizer *pVectorizer = NULL);
  virtual bool saveClientNodeState(OdGsFiler *pFiler, OdGsBaseVectorizer *pVectorizer) const;
  virtual bool loadClientNodeState(OdGsFiler *pFiler, OdGsBaseVectorizer *pVectorizer);
  virtual bool postprocessNodeLoading(OdGsFiler *pFiler);

  TD_USING(OdGsCache::extents);

  virtual bool extents(const OdGsView* pView, OdGeExtents3d& ext) const;

  virtual OdDb::LineWeight getMaxLineweightUsed() const;

  virtual void setDrawableNull();

  struct UpdateManagerContext
  {
    enum ContextType
    {
      kForMemoryLimit = 0,
      kForDrawOrder
    };
    ContextType contextType;
    OdGsViewImpl* pView;
  };

  /** \details
    Adds content of node to the update manager
    \param viewportId [in]  Id of viewport
    \param pManager [in] Pointer to the Update Manager instance
    \param context [in] UpdateManagerContext that specify what values should be initialized
  */
  virtual void addContentToUpdateManager( OdUInt32 viewportId, OdGsUpdateManager* pManager, const UpdateManagerContext& context ) {}

  /** * \details
    Returns true if and only if metafiles is compatible for specified VectorizeViews
    \param pView1 [in] Pointer to the VectorizeView
    \param pView2 [in] Pointer to the VectorizeView
  */
  virtual bool isMetafilesCompatible( const OdGsViewImpl* pView1, const OdGsViewImpl* pView2, OdGsUpdateContext* ctx ) { return true; }
};

inline OdGiDrawablePtr OdGsNode::underlyingDrawable() const
{
  if(GETBIT(m_flags, kPersistent))
    return m_pModel->open(reinterpret_cast<OdDbStub*>(m_underlyingDrawable));
  return reinterpret_cast<OdGiDrawable*>(m_underlyingDrawable);
}

inline OdDbStub* OdGsNode::underlyingDrawableId() const
{
  if(GETBIT(m_flags, kPersistent))
    return reinterpret_cast<OdDbStub*>(m_underlyingDrawable);
  return 0;
}

inline void OdGsNode::highlight(bool bDoIt, bool bWholeBranch)
{
  SETBIT(m_flags, kHLT, bDoIt);
  SETBIT(m_flags, kHLT_All, bDoIt ? bWholeBranch : false);
}

inline bool OdGsNode::isHighlighted() const
{
  return GETBIT(m_flags, kHLT);
}

inline bool OdGsNode::isHighlightedAll() const
{
  return GETBIT(m_flags, kHLT_All);
}

inline OdGsBaseModel* OdGsNode::baseModel() const
{
  return const_cast<OdGsBaseModel*>(m_pModel);
}

inline bool OdGsNode::isContainer() const
{
  return GETBIT(m_flags, kContainer);
}

inline bool OdGsNode::isSyncDrawable() const
{
  return GETBIT(m_flags, kSyncDrawable);
}

inline void OdGsNode::hide(bool bDoIt)
{
  SETBIT(m_flags, kHidden, bDoIt);
}

inline bool OdGsNode::isHidden() const
{
  return GETBIT(m_flags, kHidden);
}

inline void OdGsNode::setSelectableIfHidden(bool bSelectable)
{
  SETBIT(m_flags, kSelectHidden, bSelectable);
}

inline bool OdGsNode::isSelectableIfHidden() const
{
  return GETBIT(m_flags, kSelectHidden);
}

#include "TD_PackPop.h"

#endif // __OD_GS_NODE__
