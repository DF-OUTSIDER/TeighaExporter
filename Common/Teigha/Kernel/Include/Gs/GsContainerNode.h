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

#ifndef __OD_GS_CONTAINER_NODE__
#define __OD_GS_CONTAINER_NODE__

#include "TD_PackPush.h"

#include "Gs/GsNode.h"
#include "Gs/GsDrawableHolder.h"
#include "Si/SiSpatialIndex.h"
#include "OdVector.h"
#include "ThreadsCounter.h"

class OdGsEntityNode;
class OdGsLightNode;
class OdGsBlockNode;

/** \details
    This class implements OdGsNode descendent objects that
    can serve as containers for OdGsEntityNode objects.

    Corresponding C++ library: TD_Gs
    <group OdGs_Classes> 
*/
class GS_TOOLKIT_EXPORT OdGsContainerNode : public OdGsNode
{
protected:
  enum
  {
    kEntityListValid  = OdGsNode::kLastFlag << 1,
    kCheckWorkset     = OdGsNode::kLastFlag << 2,
    kHasStockData     = OdGsNode::kLastFlag << 3,
    kVpDepCache       = OdGsNode::kLastFlag << 4,
    kLastFlag         = kVpDepCache
  };

  enum
  {
    kChildrenNotUpToDate = 0x80000000 //special flag to use in setChildrenUpToDate/childrenUpToDate
      //per each viewport, flags are stored in m_vpAwareFlags
  };

  typedef OdList<OdGsLightNode*> LightsList;

  struct GS_TOOLKIT_EXPORT VpData : public OdRxObject
  {
    OdGsEntityNode*         m_pFirstEntity;
    OdGsEntityNode*         m_pLastEntity;
    LightsList              m_lightPtrs; // some of the lights are entities (ones that lies in model space)
    OdSiSpatialIndexPtr     m_pSpIndex;
    OdGsEntityNode*         m_pClearSpatialQueryStateFirst;
    int                     m_nChild;
    OdUInt32                m_nChildErased, m_nChildErasedPermanently;
    OdUInt32                m_nChildSingleThreaded;
    OdUInt32                m_nChildHighlighted;
    OdUInt32                m_flags;
    OdGsUpdateExtents       m_realExtents; //Keeps real extents (for EXTMIN/MAX support)

    enum
    {
      kEntityListValid  = OdGsContainerNode::kEntityListValid,
      kVpDepCache       = OdGsContainerNode::kVpDepCache
    };

    VpData();
    static OdSmartPtr<VpData> createObject(bool bShareable = false);
  };
  typedef OdSmartPtr<VpData> VpDataPtr;
  typedef OdVector<VpDataPtr> VpDataArray;

  inline VpData *getVpData(OdUInt32 nVpId, bool bCreateIfNotFound = true) const;
  VpData *makeVpData(OdUInt32 nVpId) const;
  inline OdUInt32 vpDataId(const VpData *pVpData) const;
public:
  inline bool hasVpData(OdUInt32 nVpId) const;
  inline OdUInt32 numVpData() const;
  OdUInt32 isAttached(OdGsEntityNode *pNode) const;
  void setEntityListsInvalid();
  OdUInt32 viewportId(const OdGsViewImpl &pView, bool bForceVpId = true) const;
  inline bool isVpDepCache() const;
public:
  /** \details
    Controls the EntityListValid flag for this ContainterNode object.
    \param nVpId [in]  Viewport ID.
    \param entityListValid [in]  Controls flag.
    
    \remarks
    This flag is typically set by update() and cleared by invalidate().     
  */
  void setEntityListValid(OdUInt32 nVpId, bool entityListValid);
  
  /** \details
    Returns true if and only if the ChildrenUpToDate flag is set for this ContainterNode object.
    \param nVpId [in]  Viewport ID.
    \remarks
    This flag is typically set by update() and cleared by addChild() and invalidate().
  */
  bool childrenUpToDate(OdUInt32 nVpId) const;

  /** \details
    Returns true if and only if a regen is required for the specified Viewport object for this ContainterNode object.
    \param nVpId [in]  Viewport ID. 
  */
  bool needRegen(OdUInt32 nVpId) const;

  void addChildNode(OdUInt32 nVpId, OdGsEntityNode* pEnt);

  void turnOnLights(OdGsBaseVectorizer& view);
  virtual ENodeType nodeType() const { return kContainerNode; }

  /** \details
    Vectorizes the entities in all of the child nodes of this ContainerNode object.
    \param ctx [in]  Display context.
  */
  virtual void displayEntityList(OdGsDisplayContext& ctx);

  /** \details
    Returns pointer to the first entity node inside container.
    \param nVpId [in]  Viewport ID.
  */
  OdGsEntityNode* firstEntityNode(OdUInt32 nVpId);
  /** \details
    Returns pointer to the first entity node inside container.
    \param nVpId [in]  Viewport ID.
  */
  const OdGsEntityNode* firstEntityNode(OdUInt32 nVpId) const;
  /** \details
    Returns pointer to the last entity node inside container.
    \param nVpId [in]  Viewport ID.
  */
  OdGsEntityNode* lastEntityNode(OdUInt32 nVpId);
  /** \details
    Returns pointer to the last entity node inside container.
    \param nVpId [in]  Viewport ID.
  */
  const OdGsEntityNode* lastEntityNode(OdUInt32 nVpId) const;
  /** \details
    Returns light nodes list.
    \param nVpId [in]  Viewport ID.
  */
  OdList<OdGsLightNode*> &lightNodesList(OdUInt32 nVpId);
  /** \details
    Returns light nodes list.
    \param nVpId [in]  Viewport ID.
  */
  const OdList<OdGsLightNode*> &lightNodesList(OdUInt32 nVpId) const;

  /** \details
    Returns the Aware flags for the specified Viewport for this ContainerNode object.
    
    \param viewportId [in]  Viewport ID. 
    
    \remarks
    awareFlags() returns a combination of zero or more of the following:
    
    <table>
    Name                            Value
    kVpID                           0x00000001
    kVpRegenType                    0x00000002
    kVpRenderMode                   0x00000004
    kVpWorldToEye                   0x00000008
    kVpPerspective                  0x00000010
    kVpResolution                   0x00000020
    kVpMaxDevForCircle              0x00000040
    kVpMaxDevForCurve               0x00000080
    kVpMaxDevForBoundary            0x00000100
    kVpMaxDevForIsoline             0x00000200
    kVpMaxDevForFacet               0x00000400
    kVpCamLocation                  0x00000800
    kVpCamTarget                    0x00001000
    kVpCamUpVector                  0x00002000
    kVpCamViewDir                   0x00004000
    kVpViewport                     0x00008000
    kVpFrontBack                    0x00010000
    kVpFrozenLayers                 0x00020000
    kVpLtypeScaleMult               0x00040000
    kEntityRegenDraw                0x00080000
    kVpDisableLayoutCache           0x00100000
    kVpModelTf                      0x00200000
    kVpExcludeFromExtMinMax         0x00400000
    kSectionable                    0x00800000
    kVpRenderModule                 0x01000000
    kVpAnnoScale                    0x02000000
    kVpFilterFunction               0x04000000
    kVpVisualStyle                  0x08000000
    kVpHighlightingGeom             0x10000000
    </table>
  */
   OdUInt32 awareFlags(OdUInt32 viewportId) const;

  /** \details
    Sets the Aware flags for the specified Viewport for this ContainerNode object.
    
    \param viewportId [in]  Viewport ID. 
    \param flags [in]  Aware flags.
    
    \remarks
    awareFlags() returns a combination of zero or more of the following:
    
    <table>
    Name                            Value
    kVpID                           0x00000001
    kVpRegenType                    0x00000002
    kVpRenderMode                   0x00000004
    kVpWorldToEye                   0x00000008
    kVpPerspective                  0x00000010
    kVpResolution                   0x00000020
    kVpMaxDevForCircle              0x00000040
    kVpMaxDevForCurve               0x00000080
    kVpMaxDevForBoundary            0x00000100
    kVpMaxDevForIsoline             0x00000200
    kVpMaxDevForFacet               0x00000400
    kVpCamLocation                  0x00000800
    kVpCamTarget                    0x00001000
    kVpCamUpVector                  0x00002000
    kVpCamViewDir                   0x00004000
    kVpViewport                     0x00008000
    kVpFrontBack                    0x00010000
    kVpFrozenLayers                 0x00020000
    kVpLtypeScaleMult               0x00040000
    kEntityRegenDraw                0x00080000
    kVpDisableLayoutCache           0x00100000
    kVpModelTf                      0x00200000
    kVpExcludeFromExtMinMax         0x00400000
    kSectionable                    0x00800000
    kVpRenderModule                 0x01000000
    kVpAnnoScale                    0x02000000
    kVpFilterFunction               0x04000000
    kVpVisualStyle                  0x08000000
    kVpHighlightingGeom             0x10000000
    </table>
  */
   void setAwareFlags(OdUInt32 viewportId, OdUInt32 flags);

protected:
  virtual void propagateLayerChanges(OdGsViewImpl& view);
  bool saveVpData(OdGsFiler *pFiler, const VpData *pVpData) const;
  bool loadVpData(OdGsFiler *pFiler, VpData *pVpData);

public:
  ODRX_DECLARE_MEMBERS(OdGsContainerNode);

  /** \param pModel [in]  Pointer to the BaseModel object.
    \param pUnderlyingDrawable [in]  Pointer to the underlying Drawable object.
    \param bTopLevel [in]  true if container represents layout's block
  */
  OdGsContainerNode(OdGsBaseModel* pModel, const OdGiDrawable* pUnderlyingDrawable, bool bSetGsNode = true);
  ~OdGsContainerNode();

  /** \details
    Returns true if and only if the EntityListValid flag is set for this ContainterNode object.
    \param nVpId [in]  Viewport ID.
    \remarks
    This flag is typically set by update() and cleared by invalidate().     
  */
  bool entityListValid(OdUInt32 nVpId) const;
  bool allEntityListsValid() const;
  bool isEmpty() const;

  /** \details
    Controls the ChildrenUpToDate flag for this ContainterNode object.
    \param childrenUpToDate [in]  Controls flag.
    \param nVpID [in]  Viewport ID.
    \remarks
    This flag is typically set by update() and cleared by addChild() and invalidate().
  */
  void setChildrenUpToDate(bool childrenUpToDate, const OdUInt32* nVpID = NULL);
  bool childrenRegenDraw(OdUInt32 nVpID) const;
  void setChildrenRegenDraw(bool bVal, OdUInt32 nVpID);

  /** \details
    Adds a child node to this ContainerNode object.
    \param pDrawable [in]  Pointer to the Drawable object.
    \param unerased [in]  Added child is unerased.
  */
  virtual void addChild(const OdGiDrawable* pDrawable, OdGsViewImpl *pView, bool unerased = false);

  /* \details
    Adds information about the light if the light doesn't have gsNode yet.
    \param ctx [in]  Current update context.
    \param pDrawable [in]  Pointer to the Drawable object.
  */
  virtual void addLight(OdGsUpdateContext& ctx, const OdGiDrawable* pDrawable);
  /* \details
    Removes information about the light added through addLight method.
    \param pOwner [in]  gsNode of the owner entity.
    \param nVpId [in]  Optional Viewport ID.
  */
  virtual void removeLights(OdGsNode *pOwner, OdUInt32 nVpId = 0xFFFFFFFF);

  /** \details
    Removes the specified child node from this ContainerNode object.
    \param pNode [in]  Pointer to the child object. 
  */
  virtual void removeChild(OdGsNode* pNode);

  void update(OdGsUpdateContext& ctx, OdGsContainerNode* pParent, OdSiSpatialIndex* pParentIndex);

  void updateVisible( OdGsViewImpl* pViewImpl );

  void display(OdGsDisplayContext& ctx);

  /** \details
    Returns a reference to the spatial index for this ContainerNode object.
    \param nVpId [in]  Viewport ID.
  */
  OdSiSpatialIndex& spatialIndex(OdUInt32 nVpId);
  /** \details
    Returns a reference to the spatial index for this ContainerNode object.
    \param nVpId [in]  Viewport ID.
  */
  const OdSiSpatialIndex& spatialIndex(OdUInt32 nVpId) const;

  /** \remarks
    The default implementation of this function ignores the pParent argument.
  */
  void invalidate(OdGsContainerNode* pParent, OdGsViewImpl* pView, OdUInt32 mask);

  /** \details
    Returns summ of all aware flags for entire viewports list.
  */
  OdUInt32 currViewChanges() const;

  /** \details
    Returns extents of this container node object.
    \param extents [out]  Reference to the OdGeExtents3d object which will receive current container node extents.
  */
  bool extents(OdGeExtents3d& extents) const;

  /** \details
    Returns extents of this container node object.
    \param ext [out]  Reference to the OdGeExtents3d object which will receive current container node extents.
  */
  virtual bool extents(const OdGsView* pView, OdGeExtents3d& ext) const;

  /** \details
    Updates invalidVp flag for entire entities list.
  */
  void propagateInvalidVpFlag();

  bool checkWorkset() const;
  void setCheckWorkset(bool bVal);

  virtual bool updateEntityList(OdGsUpdateContext& ctx);

  virtual void updateEntities(OdGsUpdateContext& ctx);

  bool findCompatibleCache(OdGsViewImpl& keyView);

  /** \details
    Add/release counter of highlighted children of the node, and set kHLT/kHLT_All flags if necessary
    \param nSubnodes [in]  number of subnodes
    \param bHighlight [in]  if true then highlight else unhighlight
    \param bAll [in]  if true then consider kHLT_All flag
    Returns true if and only if highlighting of the child caused changing of kHLT flag on the container.
  */
  bool highlightSubnodes(OdUInt32 nSubnodes, bool bHighlight, bool bAll);

  void destroy();

  void destroySubitems();

  /** \details
    Returns real extents of the node
    \param nVpId [in]  Viewport ID.
  */
  inline const OdGsUpdateExtents& realExtents(OdUInt32 nVpId) const;

  /** \details
    Sets real extents of the node and its maximal lineweight
    \param nVpId [in]  Viewport ID.
    \param ext [in]  New real extents for the node.
    \param nMaxLw [in]  New maximal lineweight for the node.
  */
  inline void setRealExtents(OdUInt32 nVpId, const OdGsUpdateExtents& ext);

  inline void addViewRef(OdUInt32 nVpId);
  inline void removeViewRef(OdUInt32 nVpId);
  inline const ViewRefs& viewRefs() const;

  inline const StockProps& stock() const;
  void makeStock();
  void releaseStock();
  void propagateLayerChangesStock();

  bool saveNodeState(OdGsFiler *pFiler, OdGsBaseVectorizer *pVectorizer = NULL) const;
  bool loadNodeState(OdGsFiler *pFiler, OdGsBaseVectorizer *pVectorizer = NULL);
  bool postprocessNodeLoading(OdGsFiler *pFiler);

  bool safeEntityUpdate(OdGsEntityNode* pEntity, OdGsUpdateContext& ctx, OdSiSpatialIndex* pSpatialIndex);
  void addEntProps(const OdGsUpdateContext& ctx);
  bool doMTUpdate(OdGsUpdateContext& ctx);
  void doSTUpdate(OdGsUpdateContext& ctx);

  /** \details
    Removes entities marked as erased.
  */
  void removeErased();

  /** \details
    Returns number of child entities
    \param nVpId [in]  Viewport ID.
  */
  inline int numberOfChildren(OdUInt32 nVpId) const;
  inline OdUInt32 numberOfChildrenST(OdUInt32 nVpId) const;
  inline OdUInt32 numberOfChildrenErased(OdUInt32 nVpId) const;
protected:
  mutable VpDataArray m_vpData;
  mutable VpDataPtr m_shareableData;
  OdUInt32Vector m_vpAwareFlags; //this memory is never shared, no need in OdArray with refcounter on buffer
  ViewRefs m_viewRefs;
  StockProps m_stock;
  OdMutexPtr m_lightsMutex;
public:
  void addContentToUpdateManager( OdUInt32 viewportId, OdGsUpdateManager* pManager, const UpdateManagerContext& context );
  bool isMetafilesCompatible( const OdGsViewImpl* pView1, const OdGsViewImpl* pView2, OdGsUpdateContext* ctx );
};

inline OdGsContainerNode::VpData *OdGsContainerNode::getVpData(OdUInt32 nVpId, bool bCreateIfNotFound) const
{
  if (!GETBIT(m_flags, kVpDepCache))
    return m_shareableData;
  VpData *pVpData = (m_vpData.size() > nVpId) ? m_vpData[nVpId].get() : NULL;
  if (!pVpData && bCreateIfNotFound)
    return makeVpData(nVpId);
  return pVpData;
}

inline OdUInt32 OdGsContainerNode::vpDataId(const VpData *pVpData) const
{
  if (GETBIT(m_flags, kVpDepCache))
  {
    const OdUInt32 nVpData = m_vpData.size();
    const VpDataPtr *pVpDataPtr = m_vpData.getPtr();
    for (OdUInt32 nVpId = 0; nVpId < nVpData; nVpId++)
    {
      if (pVpDataPtr[nVpId].get() == pVpData)
        return nVpId;
    }
  }
  return 0;
}

inline bool OdGsContainerNode::hasVpData(OdUInt32 nVpId) const
{
  if (!GETBIT(m_flags, kVpDepCache))
    return true;
  return ((m_vpData.size() > nVpId) && !m_vpData.getAt(nVpId).isNull());
}

inline OdUInt32 OdGsContainerNode::numVpData() const
{
  if (!GETBIT(m_flags, kVpDepCache))
    return 1;
  return m_vpData.size();
}

inline bool OdGsContainerNode::isVpDepCache() const
{
  return GETBIT(m_flags, kVpDepCache);
}

inline bool OdGsContainerNode::entityListValid(OdUInt32 nVpId) const
{
  if (!isVpDepCache())
    return GETBIT(m_flags, kEntityListValid);
  const VpData *pVpData = getVpData(nVpId, false);
  if (pVpData)
    return GETBIT(pVpData->m_flags, kEntityListValid);
  return false;
}

inline bool OdGsContainerNode::allEntityListsValid() const
{
  return GETBIT(m_flags, kEntityListValid);
}

inline bool OdGsContainerNode::isEmpty() const
{
  const OdUInt32 nVpData = numVpData();
  for (OdUInt32 nView = 0; nView < nVpData; nView++)
  {
    const VpData *pVpData = getVpData(nView, false);
    if (pVpData && pVpData->m_pFirstEntity)
      return false;
  }
  return true;
}

inline void OdGsContainerNode::setEntityListsInvalid()
{
  for (OdUInt32 nView = 0; nView < numVpData(); nView++)
  {
    if (hasVpData(nView))
      SETBIT_0(getVpData(nView, false)->m_flags, VpData::kEntityListValid);
  }
  SETBIT_0(m_flags, kEntityListValid);
}

inline void OdGsContainerNode::setEntityListValid(OdUInt32 nVpId, bool bValid)
{
  SETBIT(getVpData(nVpId)->m_flags, kEntityListValid, bValid);
  if (!bValid || !isVpDepCache())
    SETBIT(m_flags, kEntityListValid, bValid);
  else
  { // Check does all entity lists valid
    const OdUInt32 nViews = numVpData();
    bool bAllListsValid = true;
    for (OdUInt32 nView = 0; nView < nViews; nView++)
    {
      const VpData *pVpData = getVpData(nView, false);
      if (pVpData && !GETBIT(pVpData->m_flags, VpData::kEntityListValid))
      {
        bAllListsValid = false;
        break;
      }
    }
    SETBIT(m_flags, kEntityListValid, bAllListsValid);
  }
}

inline OdSiSpatialIndex& OdGsContainerNode::spatialIndex(OdUInt32 nVpId)
{
  return *getVpData(nVpId)->m_pSpIndex.get();
}

inline const OdSiSpatialIndex& OdGsContainerNode::spatialIndex(OdUInt32 nVpId) const
{
  return *getVpData(nVpId)->m_pSpIndex.get();
}

inline OdGsEntityNode* OdGsContainerNode::firstEntityNode(OdUInt32 nVpId)
{
  VpData *pVpData = getVpData(nVpId, false);
  if (!pVpData)
    return NULL;
  return pVpData->m_pFirstEntity;
}

inline const OdGsEntityNode* OdGsContainerNode::firstEntityNode(OdUInt32 nVpId) const
{
  VpData *pVpData = getVpData(nVpId, false);
  if (!pVpData)
    return NULL;
  return pVpData->m_pFirstEntity;
}

inline OdGsEntityNode* OdGsContainerNode::lastEntityNode(OdUInt32 nVpId)
{
  VpData *pVpData = getVpData(nVpId, false);
  if (!pVpData)
    return NULL;
  return pVpData->m_pLastEntity;
}

inline const OdGsEntityNode* OdGsContainerNode::lastEntityNode(OdUInt32 nVpId) const
{
  VpData *pVpData = getVpData(nVpId, false);
  if (!pVpData)
    return NULL;
  return pVpData->m_pLastEntity;
}

inline OdList<OdGsLightNode*> &OdGsContainerNode::lightNodesList(OdUInt32 nVpId)
{
  return getVpData(nVpId)->m_lightPtrs;
}

inline const OdList<OdGsLightNode*> &OdGsContainerNode::lightNodesList(OdUInt32 nVpId) const
{
  return getVpData(nVpId)->m_lightPtrs;
}

inline bool OdGsContainerNode::childrenUpToDate(OdUInt32 nVpID) const
{
  if(m_vpAwareFlags.size()>nVpID)
    return !GETBIT(m_vpAwareFlags[nVpID], kChildrenNotUpToDate);
  return false;
}

inline void OdGsContainerNode::setChildrenUpToDate(bool bValid, const OdUInt32* nVpID)
{
  if(nVpID)
  {
    OdUInt32Array::size_type n = m_vpAwareFlags.size();
    if(n <= *nVpID)
      m_vpAwareFlags.insert(m_vpAwareFlags.end(), (*nVpID)+1-n, 0xFFFFFFFF);
    SETBIT(m_vpAwareFlags[*nVpID], kChildrenNotUpToDate, !bValid);
  }
  else
  {
    ODA_ASSERT(!bValid);
    for(OdUInt32 i = 0; i < m_vpAwareFlags.size(); ++i)
      SETBIT(m_vpAwareFlags[i], kChildrenNotUpToDate, true);
  }
}

inline OdUInt32 OdGsContainerNode::awareFlags(OdUInt32 nVpID) const
{
  if(m_vpAwareFlags.size()>nVpID)
    return m_vpAwareFlags[nVpID] & ~kChildrenNotUpToDate;
  return 0xFFFFFFFF;
}

inline void OdGsContainerNode::setAwareFlags(OdUInt32 nVpID, OdUInt32 flags)
{
  flags &= ~kChildrenNotUpToDate;
  OdUInt32Array::size_type n = m_vpAwareFlags.size();
  if(n <= nVpID)
  {
    m_vpAwareFlags.insert(m_vpAwareFlags.end(), nVpID+1-n, 0xFFFFFFFF);
    m_vpAwareFlags[nVpID] = flags;
  }
  else
  {
    m_vpAwareFlags[nVpID] = ((m_vpAwareFlags[nVpID] & kChildrenNotUpToDate) | flags);
  }
}

inline bool OdGsContainerNode::needRegen(OdUInt32 nVpID) const
{
  if(!childrenUpToDate(nVpID))
    return true;

  OdUInt32 nAF = awareFlags(nVpID);
  return ( nAF == 0xFFFFFFFF || ( baseModel()->viewChanges(nVpID) & nAF) != 0 );
}

inline bool OdGsContainerNode::checkWorkset() const
{
  return GETBIT(m_flags, kCheckWorkset);
}

inline void OdGsContainerNode::setCheckWorkset(bool bVal)
{
  SETBIT(m_flags, kCheckWorkset, bVal);
}

inline bool OdGsContainerNode::childrenRegenDraw(OdUInt32 nVpID) const
{
  if(m_vpAwareFlags.size()>nVpID)
    return GETBIT(m_vpAwareFlags[nVpID], kEntityRegenDraw);
  return false;
}

inline void OdGsContainerNode::setChildrenRegenDraw(bool bVal, OdUInt32 nVpID)
{
  OdUInt32Array::size_type n = m_vpAwareFlags.size();
  if(n <= nVpID)
    m_vpAwareFlags.insert(m_vpAwareFlags.end(), nVpID+1-n, 0xFFFFFFFF);
  SETBIT(m_vpAwareFlags[nVpID], kEntityRegenDraw, bVal);
}

inline const OdGsUpdateExtents& OdGsContainerNode::realExtents(OdUInt32 nVpId) const
{
  VpData *pVpData = getVpData(nVpId, false);
  if (!pVpData)
    return OdGsUpdateExtents::kInvalid;
  return getVpData(nVpId)->m_realExtents;
}

inline void OdGsContainerNode::setRealExtents(OdUInt32 nVpId, const OdGsUpdateExtents& ext)
{
  VpData *pVpData = getVpData(nVpId);
  pVpData->m_realExtents = ext;
}

inline void OdGsContainerNode::addViewRef(OdUInt32 nVpId)
{
  m_viewRefs.add(nVpId);
}

inline void OdGsContainerNode::removeViewRef(OdUInt32 nVpId)
{
  m_viewRefs.remove(nVpId);
}

inline const ViewRefs& OdGsContainerNode::viewRefs() const
{
  return m_viewRefs;
}

inline const StockProps& OdGsContainerNode::stock() const
{
  return m_stock;
}

inline int OdGsContainerNode::numberOfChildren(OdUInt32 nVpId) const
{
  return getVpData(nVpId)->m_nChild;
}

inline OdUInt32 OdGsContainerNode::numberOfChildrenST(OdUInt32 nVpId) const
{
  return getVpData(nVpId)->m_nChildSingleThreaded;
}

inline OdUInt32 OdGsContainerNode::numberOfChildrenErased(OdUInt32 nVpId) const
{ VpData *pVpData = getVpData(nVpId);
  return pVpData->m_nChildErased + pVpData->m_nChildErasedPermanently;
}

#include "TD_PackPop.h"

#endif // __OD_GS_CONTAINER_NODE__
