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

#ifndef __OD_GS_BLOCK_NODE__
#define __OD_GS_BLOCK_NODE__

#include "TD_PackPush.h"

#include "GsBlockReferenceNode.h"
#include "Ge/GeScale3d.h"
#include "Gi/GiSubEntityTraitsData.h"
#include "Gi/GiAnnoScaleSet.h"
#define STL_USING_SET
#define STL_USING_MAP
#include "OdaSTL.h"
#include "ThreadsCounter.h"

class OdGsSharedRefDefinition;
class OdGsReferenceImpl;

/** \details

    Corresponding C++ library: Gs
    <group OdGs_Classes> 
*/
class GS_TOOLKIT_EXPORT OdGsBlockRefNodeDescMem
{
public:
  OdGsBlockRefNodeDescMem(const OdDbStub* layoutBlockId, const OdGiSubEntityTraitsData& tr,
    const OdGeScale3d& scale, bool bUsePlotStyles):
      m_layoutBlockId(layoutBlockId),
      m_layerId(tr.layer())
    , m_lineTypeId(tr.lineType())
    , m_cmColor(tr.trueColor().color())
    , m_lineWeight(tr.lineWeight())
    , m_lineTypeScale(tr.lineTypeScale())
    , m_plotStyleNameType(bUsePlotStyles ? tr.plotStyleNameType() : OdDb::kPlotStyleNameIsDictDefault)
    , m_plotStyleNameId(bUsePlotStyles ? tr.plotStyleNameId() : NULL)
    , m_materialId(tr.material()), m_cmTransparency(tr.transparency().serializeOut())
    , m_mapper(tr.mapper() ? *tr.mapper() : OdGiMapper())
    , m_scale(scale)
  {
  }
  OdGsBlockRefNodeDescMem(const OdGsBlockRefNodeDescMem& c)
  {
    ::memcpy(this, &c, sizeof(OdGsBlockRefNodeDescMem));
  }
  OdGsBlockRefNodeDescMem(){ ::memset(this, 0, sizeof(OdGsBlockRefNodeDescMem)); }

protected:
  bool lessOrEqual(const OdGsBlockRefNodeDescMem& v, bool& bEqual) const;

public:
  const OdDbStub* m_layoutBlockId;
  OdDbStub* m_layerId;
  OdDbStub* m_lineTypeId;
  OdUInt32 m_cmColor;
  OdDb::LineWeight m_lineWeight;
  double m_lineTypeScale;

  OdDb::PlotStyleNameType m_plotStyleNameType;
  OdDbStub* m_plotStyleNameId;

  OdDbStub* m_materialId;
  OdGiMapper m_mapper;
  OdUInt32 m_cmTransparency;

  OdGeScale3d m_scale;
};

/** \details

    Corresponding C++ library: Gs
    <group OdGs_Classes> 
*/
class GS_TOOLKIT_EXPORT OdGsBlockRefNodeDesc: public OdGsBlockRefNodeDescMem
{
public:
  OdGsBlockRefNodeDesc(const OdDbStub* layoutBlockId, const OdGiSubEntityTraitsData& tr,
    const OdGeScale3d& scale, bool bUsePlotStyles, const OdGiAnnoScaleSet& annoScales):
    OdGsBlockRefNodeDescMem(layoutBlockId, tr, scale, bUsePlotStyles), m_annoScales(annoScales)
  {
  }
  OdGsBlockRefNodeDesc(const OdGsBlockRefNodeDesc& c): OdGsBlockRefNodeDescMem(c), m_annoScales(c.m_annoScales)
  {
  }
  OdGsBlockRefNodeDesc(){}

  bool operator < (const OdGsBlockRefNodeDesc& v) const
  {
    bool bEqual = false;
    if(lessOrEqual(v, bEqual) && !bEqual)
      return true;
    return bEqual ? m_annoScales < v.m_annoScales : false;
  }
public:
  OdGiAnnoScaleSet m_annoScales;
};

/** \details

    Corresponding C++ library: Gs
    <group OdGs_Classes> 
*/
class GS_TOOLKIT_EXPORT OdGsBlockNode: public OdGsNode
{
protected:
  enum
  {
    kUnloaded   = OdGsNode::kLastFlag << 1,
    kLastFlag   = kUnloaded
  };
public:
  enum
  {
    kInvalidateRefExtentsMask = (kVpLastPropBit << 1)
	//NS: use this mask in invalidate() to invalidate extents of corresponding
	//references when block is modified
  };

  ODRX_DECLARE_MEMBERS(OdGsBlockNode);

  OdGsBlockNode(OdGsBaseModel* pModel, const OdGiDrawable* pUnderlyingDrawable,
    bool bSetGsNode): OdGsNode(pModel, pUnderlyingDrawable), m_bModelTfDependent(0)
  {
    if(bSetGsNode)
      setToDrawable(pUnderlyingDrawable);
  }
  ~OdGsBlockNode()
  {
    if (isUnloaded())
    {
      // this block-node corresponds to block in xreffed db;
      // here db can be unloaded, so prevent access to the associated block table record.
      SETBIT(m_flags, kSyncDrawable, false);
    }
    clearInserts();
  }

  void setUnloaded(bool isUnloaded);
  bool isUnloaded() const;
  void setModelTfDependent(bool bOn);
  bool isModelTfDependent() const;

  virtual ENodeType nodeType() const { return kBlockNode; }

  void invalidate(OdGsContainerNode* pParent, OdGsViewImpl* view, OdUInt32 mask);

  void addRef(OdGsBlockReferenceNode* pEntity)
  {
    if(odThreadsCounter())
    {
      TD_AUTOLOCK_P(m_insertsMt.get());
      m_inserts.insert(pEntity);
    }
    else
    {
      m_inserts.insert(pEntity);
    }
  }

  void release(OdGsBlockReferenceNode* pEntity)
  {
    TD_AUTOLOCK_P_DEF(m_insertsMt);
    m_inserts.erase(pEntity);
    if(!isUnloaded() || !m_inserts.empty())
      return;
    // this block-node corresponds to block in xreffed db;
    // here db can be unloaded, so prevent access to the associated block table record.
    SETBIT(m_flags, kSyncDrawable, false);
    baseModel()->detach(this);
  }

  /** \details
    Updates the cached data within this Node object for the specified VectorizeView and ContainerNode objects. 
    \param pParent [in]  Pointer to the ContainerNode object for this Node object.
    \param view [in]  VectorizeView object for which data are to be updated.
  */
  void update(OdGsUpdateContext&, OdGsContainerNode*, OdSiSpatialIndex*)
  {
    ODA_ASSERT(false);
  }

  /** \details
    Paints the specified view with the cached data associated with this Node object.
    \param view [in]  View to be painted.
  */
  void display(OdGsDisplayContext&)
  {
    ODA_ASSERT(false);
  }

  /** \details
    Returns the extents of the underlying GiDrawable object(s) associated with this Node object.
    
    \param extents [out]  Receives the extents.
    
    \returns
    Returns true if and only if the GiDrawable object(s) have extents.
  */
  bool extents(OdGeExtents3d&) const
  {
    ODA_ASSERT(false);
    return false;
  }

  /** \details
    Invalidates parts of cache depended on layers modified since last regeneration.
    
    \remarks
    For internal use only.
    Returns this if and only if the this node is one of modified layers.
  */
  virtual void propagateLayerChanges(OdGsViewImpl& /*view*/);

  void destroy();

  void invalidateShared()
  {
    m_sharedImp.destroy();
  }

  OdGsSharedRefDefinition* createDef(const OdGsBlockRefNodeDesc& d);
  OdGsSharedRefDefinition* findCreateDef(const OdGsBlockRefNodeDesc& d)
  {
    if(odThreadsCounter())
    {
      TD_AUTOLOCK_P(m_sharedMt.get());
      OdGsSharedRefDefinition* pRes = m_sharedImp.findAt(d);
      return pRes ? pRes : createDef(d);
    }
    OdGsSharedRefDefinition* pRes = m_sharedImp.findAt(d);
    return pRes ? pRes : createDef(d);
  }

  void invalidateSharedSubents(OdUInt32 vpID, OdUInt32 nViewChanges);

  void invalidateSharedRegenDraw(OdUInt32 vpID, const OdDbStub* layoutId);
  void invalidateSharedSectionable();
  void invalidateSharedAwareFlags(OdGsViewImpl* pView, OdUInt32 nViewChanges, const OdDbStub* layoutId);

  void removeDef(OdGsSharedRefDefinition* pDef)
  {
    if(odThreadsCounter())
    {
      TD_AUTOLOCK_P(m_sharedMt.get());
      m_sharedImp.removeDef(pDef);
    }
    else
    {
      m_sharedImp.removeDef(pDef);
    }
  }
  bool findDef(OdGsSharedRefDefinition* pDef, OdGsBlockRefNodeDesc& d)
  {
    if(odThreadsCounter())
    {
      TD_AUTOLOCK_P(m_sharedMt.get());
      return m_sharedImp.findDef(pDef, d);
    }
    return m_sharedImp.findDef(pDef, d);
  }

  bool saveNodeState(OdGsFiler *pFiler, OdGsBaseVectorizer *pVectorizer = NULL) const;
  bool loadNodeState(OdGsFiler *pFiler, OdGsBaseVectorizer *pVectorizer = NULL);
  bool postprocessNodeLoading(OdGsFiler *pFiler);

protected:
  void clearInserts();

protected:
  class GS_TOOLKIT_EXPORT ImpMap
  {
  public:
    ~ImpMap() { clear(); }
    void clear();
    void destroy();
    OdGsSharedRefDefinition* findAt(const OdGsBlockRefNodeDesc& d)
    {
      _map::iterator it = m_map.find(d);
      return (it == m_map.end() ? NULL : it->second);
    }
    OdGsSharedRefDefinition* insertAt(const OdGsBlockRefNodeDesc& d,
        OdGsReferenceImpl* pImp);
    bool layersChanged(OdGsViewImpl& view, OdGsBaseModel* pModel);
    void invalidateSubents(OdUInt32 vpID, OdUInt32 nViewChanges);
    void invalidateRegenDraw(OdUInt32 vpID, const OdDbStub* layoutId);
    void removeDef(OdGsSharedRefDefinition* pDef);
    bool findDef(OdGsSharedRefDefinition* pDef, OdGsBlockRefNodeDesc& d);
    void invalidateSectionable();
#ifndef ODGS_LOCAL_IDS
    void invalidateAwareFlags(OdGsViewImpl* pView, OdUInt32 nViewChanges, const OdDbStub* layoutId);
#else
    void invalidateAwareFlags(OdGsViewImpl* pView, OdUInt32 nViewChanges, const OdDbStub* layoutId, OdGsBaseModel* pModel);
#endif
    void propagateLayerChangesStock();

  public:
    typedef std::map<OdGsBlockRefNodeDesc, OdGsSharedRefDefinition*,
        std::less<OdGsBlockRefNodeDesc> > _map;
    _map m_map;
  };

protected:
  typedef std::set<OdGsBlockReferenceNode*> Inserts;
  typedef Inserts::iterator InsertIterator;
  Inserts m_inserts;
  ImpMap m_sharedImp;
  OdMutexPtr m_sharedMt;
  OdMutexPtr m_insertsMt;
  volatile int m_bModelTfDependent;
};

inline void OdGsBlockNode::setUnloaded(bool isUnloaded)
{
  SETBIT(m_flags, kUnloaded, isUnloaded);
}

inline bool OdGsBlockNode::isUnloaded() const
{
  return GETBIT(m_flags, kUnloaded);
}

inline void OdGsBlockNode::setModelTfDependent(bool bOn)
{
  if(isModelTfDependent() == bOn)
      return;
#ifndef TD_SINGLE_THREAD
  OdInterlockedExchangeAdd(&m_bModelTfDependent, bOn ? 1 : -1);
#else
  m_bModelTfDependent = bOn;
#endif
  if(bOn)
  {
    //don't keep content of m_sharedImp anymore. These definitions remain 
    //on inserts, if any, and will be released in the course of their update()
    if(odThreadsCounter())
    {
      TD_AUTOLOCK_P(m_sharedMt.get());
      m_sharedImp.clear();
    }
    else
    {
      m_sharedImp.clear();
    }
  }
}

inline bool OdGsBlockNode::isModelTfDependent() const
{
  //atomic function is required, called in MT mode (update)
#ifndef TD_SINGLE_THREAD
  return OdInterlockedExchangeAdd(&const_cast<OdGsBlockNode*>(this)->m_bModelTfDependent, 0) != 0;
#else
  return m_bModelTfDependent;
#endif
}

#include "TD_PackPop.h"

#endif // __OD_GS_BLOCK_NODE__
