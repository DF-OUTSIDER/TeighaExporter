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

#ifndef __OD_GS_ENTITY_NODE__
#define __OD_GS_ENTITY_NODE__

#include "Gs/GsNode.h"
#include "Si/SiSpatialIndex.h"
#include "OdArrayMemAlloc.h"
#include "Gs/GsHighlightData.h"
#include "Gs/GsUpdateManager.h"

class OdGsBaseVectorizer;
class OdGsBaseVectorizeDevice;

#include "TD_PackPush.h"

/** <group !!RECORDS_TD_APIRef>
*/
struct OdGsGeomPortion
{
  OdGsLayerNode*    m_pLayer;
  OdRxObjectPtr     m_pGsMetafile;
  OdGsGeomPortion*  m_pNext;

  OdGsGeomPortion()
    : m_pLayer(0)
    , m_pGsMetafile(0)
    , m_pNext(0)
  {
  }

  void copyFrom(OdGsGeomPortion& c)
  {
    m_pLayer = c.m_pLayer;
    m_pGsMetafile = c.m_pGsMetafile;
    m_pNext = c.m_pNext;
  }

  void clear()
  {
    m_pLayer = NULL;
    m_pGsMetafile = NULL;
    m_pNext = NULL;
  }

  void freeNestedPortions()
  {
    OdGsGeomPortion* pPortion = m_pNext;
    while (pPortion)
    {
      OdGsGeomPortion* pNextPortion = pPortion->m_pNext;
      delete pPortion;
      pPortion = pNextPortion;
    }
  }

  bool isEmptyPortion() const { return (m_pLayer == NULL) && m_pGsMetafile.isNull(); }
  void filterEmptyNestedPortions()
  {
    while (m_pNext && m_pNext->isEmptyPortion())
    {
      OdGsGeomPortion* pNextPortion = m_pNext->m_pNext;
      delete m_pNext;
      m_pNext = pNextPortion;
    }
    if (m_pNext)
      m_pNext->filterEmptyNestedPortions();
  }
};

class OdGsBaseVectorizeDevice;
class OdGsBaseContext;

/** \details
    This class implements OdGsNode descendant objects that
    can serve as containers for other OdGsNode objects.

    Corresponding C++ library: TD_Gs
    <group OdGs_Classes> 
*/
class GS_TOOLKIT_EXPORT OdGsEntityNode : public OdGsNode, public OdSiEntity
{
  OdGsEntityNode*             m_pNextEntity;

  struct NextEntityEntry
  {
    OdGsEntityNode*           m_pNextEntity;
    OdUInt32                  m_entFlags;
  };
  void allocNextEntityArray(OdUInt32 nMaxVpId);
  inline NextEntityEntry *firstEntEntry() { return reinterpret_cast<NextEntityEntry*>(m_pNextEntity); }
  inline const NextEntityEntry *firstEntEntry() const { return reinterpret_cast<const NextEntityEntry*>(m_pNextEntity); }
  inline void checkEntEntry(OdUInt32 nVpId) const { if ((OdUInt32)OdPtrToInt32(firstEntEntry()->m_pNextEntity) <= nVpId) throw OdError(eInvalidIndex); }
  inline NextEntityEntry *nextEntEntry(OdUInt32 nVpId) { checkEntEntry(nVpId); return reinterpret_cast<NextEntityEntry*>(m_pNextEntity) + (nVpId + 1); }
  inline const NextEntityEntry *nextEntEntry(OdUInt32 nVpId) const { checkEntEntry(nVpId); return reinterpret_cast<const NextEntityEntry*>(m_pNextEntity) + (nVpId + 1); }
protected:
  mutable OdGeExtents3d       m_extents;

public:
  class GS_TOOLKIT_EXPORT Metafile : public OdRxObject
  {
  public:
    void* operator new(size_t);
    void operator delete(void *);

    Metafile();
    ~Metafile();

    bool play(OdGsBaseVectorizer& view, EMetafilePlayMode eMode, OdGsEntityNode& node, OdGsBaseContext* ctx = NULL);
    bool playNested(const OdGsView& view, OdGsEntityNode& node, OdGsBaseContext& ctx);
    bool hasDataToPlay(OdGsBaseVectorizer& view, EMetafilePlayMode eMode = kMfDisplay);
    bool layersChanged(OdUInt32 nVpId) const;
    bool containsVpFrozenLayers() const;
    bool containsFrozenLayers() const;
    bool containsAnyOfLayers( OdDbStub*const* layers, OdUInt32 count ) const;

    bool isEmpty() const;
    bool isAllLayersOff( OdUInt32 viewportId ) const;

    OdUInt32            m_nAwareFlags;
    OdGeExtents3d       m_extents;
    OdGsGeomPortion     m_first;
  };
  friend class Metafile;

  /** \details
    This template class is a specialization of the OdSmartPtr class for Metafile object pointers.
  */
  typedef OdSmartPtr<Metafile> MetafilePtr;

  class GS_TOOLKIT_EXPORT MetafileEx : public Metafile, public OdGsUpdateManager::OdGsUpdateManagerElement
  {
  protected:
    OdRxObjectPtr m_pDeviceRef;
    OdUInt64 m_nLength;
  public:
    class GS_TOOLKIT_EXPORT OdGsDeviceSelfReference : public OdRxObject
    {
      OdGsBaseVectorizeDevice* m_pDevice;
    public:
      OdGsDeviceSelfReference()
      {
        m_pDevice = NULL;
      };
      void setDevice( OdGsBaseVectorizeDevice* device ) 
      {
        m_pDevice = device;
      }
      OdGsBaseVectorizeDevice* device() { return m_pDevice; }
    };

    MetafileEx() : Metafile(), OdGsUpdateManager::OdGsUpdateManagerElement()
    {
      m_pDeviceRef = NULL;
    }
    ~MetafileEx();
    void setDevice( OdRxObjectPtr dev ) { m_pDeviceRef = dev; }
    OdRxObjectPtr device() { return m_pDeviceRef; }

    void setMetafileLength( OdUInt64 l ) { m_nLength = l; }
    OdUInt64 metafileLength() const { return m_nLength; }
  };

  friend class OdGsModuleObject;
  friend class OdGsBlockReferenceNode;
public:
  static void* Alloc(size_t nBytes) { return OdGsNode::s_aAlloc.getAt(1)->alloc((int)nBytes); }
  static void Free(void* pMemBlock) { OdGsNode::s_aAlloc.getAt(1)->release(pMemBlock); }
  static void* Realloc(void* pMemBlock, size_t newSize, size_t)
  {
    if(pMemBlock)
      OdGsNode::s_aAlloc.getAt(1)->release(pMemBlock);
    return OdGsNode::s_aAlloc.getAt(1)->alloc((int)newSize);
  }
  /** \details
      This template class is a specialization of the OdArray class for MetafilePtr objects.
  */
  class MetafilePtrArray: public OdArrayMemAlloc<MetafilePtr, OdObjectsAllocator<MetafilePtr>, OdGsEntityNode>
  {
  public:
    ~MetafilePtrArray(){}
  };

  class GS_TOOLKIT_EXPORT MetafileHolder
  {
  public:
    enum 
    {
      kArray = 1,
      kVpDependent = 2,
      kAwareFlagsRegenType = 4,

      kRegenTypeStandard = 8,
      kRegenTypeHideOrShade = 16,
      kRegenTypeRenderCommand = 32,
      kRegenTypeMask = kRegenTypeStandard | kRegenTypeHideOrShade | kRegenTypeRenderCommand,

      kDependentGeometry = 64, // Marks PaperSpace entities
      kLayerDependent = 128 // Marks compound entities w/o blocks cache (#15036)
    };
    MetafileHolder(): m_flags(0)
    {
      *reinterpret_cast<Metafile**>(rawPtr()) = 0;
    }
    MetafileHolder(const MetafileHolder& c): m_flags(0)
    {
      *reinterpret_cast<Metafile**>(rawPtr()) = 0;
      if(!c.isValid())
        return;
      if(c.isArray())
      {
        allocateArray();
        getArray() = const_cast<MetafileHolder&>(c).getArray();
      }
      else
      {
        *reinterpret_cast<Metafile**>(rawPtr()) = const_cast<MetafileHolder&>(c).get();
        get()->addRef();
      }
      m_flags = c.m_flags;
    }
    ~MetafileHolder()
    {
      destroy();
    }
    
    bool isValid() const { return *((const Metafile**)rawPtr()) != NULL; }
    bool isArray() const { return GETBIT(flags(), kArray); }
    
    bool isVpDependent() const { return GETBIT(flags(), kVpDependent); }
    void setVpDependent(bool bOn)
    {
      SETBIT(m_flags, kVpDependent, bOn);
      if(bOn)
      {
        SETBIT(m_flags, kRegenTypeMask, false);
        setAwareFlagsRegenType(false);
      }
    }

    bool isAwareFlagsRegenType() const { return GETBIT(flags(), kAwareFlagsRegenType); }
    bool isRegenTypeDependent() const { return GETBIT(flags(), kRegenTypeMask); }

    void setDependentGeometry(bool bSet) { SETBIT(m_flags, kDependentGeometry, bSet); }
    bool isDependentGeometry() const { return GETBIT(flags(), kDependentGeometry); }

    void setLayerDependent(bool bSet) { SETBIT(m_flags, kLayerDependent, bSet); }
    bool isLayerDependent() const { return GETBIT(m_flags, kLayerDependent); }

    Metafile* get()
    {
        ODA_ASSERT(!isArray());
        return *reinterpret_cast<Metafile**>(rawPtr());
    }
    const Metafile* get() const
    {
        ODA_ASSERT(!isArray());
        return *reinterpret_cast<Metafile**>(const_cast<MetafileHolder*>(this)->rawPtr());
    }
    MetafilePtrArray& getArray()
    {
        ODA_ASSERT(isArray());
        return *reinterpret_cast<MetafilePtrArray*>(rawPtr());
    }
    const MetafilePtrArray& getArray() const
    {
        ODA_ASSERT(isArray());
        return *reinterpret_cast<const MetafilePtrArray*>(rawPtr());
    }
    void set(Metafile* pMf)
    {
      destroy();
      *reinterpret_cast<Metafile**>(rawPtr()) = pMf;
      if(pMf)
        pMf->addRef();
    }

    void setAtRegenType(Metafile* pMf, OdGiRegenType eRegenType)
    {
      int index = eRegenType - kOdGiStandardDisplay;
      if(isArray())
      {
        ODA_ASSERT(isRegenTypeDependent());
        setAt(index, pMf);
      }
      else if(isRegenTypeDependent())
      {
        if(GETBIT(m_flags , kRegenTypeStandard << index))
        {
          set(pMf);
          if(!pMf)
            setAwareFlagsRegenType(false);
          return;
        }
        allocateArray();
        setAt(index, pMf);
      }
      else
      {
        set(pMf);
      }
      if(pMf)
      {
        m_flags |= kRegenTypeStandard << index;
        if(pMf->m_nAwareFlags & kVpRegenType)
          setAwareFlagsRegenType(true);
      }
    }
    
    Metafile* getAtRegenType(OdGiRegenType& eRegenType)
    {
      ODA_ASSERT(isRegenTypeDependent());
      ODA_ASSERT(!isArray());
      if(GETBIT(flags(), kRegenTypeStandard))
        eRegenType = kOdGiStandardDisplay;
      else if(GETBIT(flags(), kRegenTypeHideOrShade))
        eRegenType = kOdGiHideOrShadeCommand;
      else
        eRegenType = kOdGiRenderCommand;
      return get();
    }

    void allocateArray();

    void destroy();
    int checkValid();

    void moveTo(MetafileHolder& c, int& n);
    OdUInt32 awareFlags(OdUInt32 nVpID) const
    {
      if(isArray())
      {
        if(isVpDependent())
            return getArray().at(nVpID)->m_nAwareFlags;
        return kVpRenderMode | (isAwareFlagsRegenType() ? kVpRegenType : 0);
      }
      return isValid() ? get()->m_nAwareFlags : 0;
    }

    void setAt(int n, Metafile* pMf)
    {
      ODA_ASSERT(isArray());
      if(getArray().size() <= OdUInt32(n))
        getArray().resize(n + 1);
      getArray()[n] = pMf;
    }

  protected:
    int flags() const { return (int)m_flags; }
    void* rawPtr() { return &m_data; }
    const void* rawPtr() const { return &m_data; }
    bool isValidMf(const Metafile& mf) const;
    void setAwareFlagsRegenType(bool bOn) { SETBIT(m_flags, kAwareFlagsRegenType, bOn); }

  protected:
    OdUInt8 m_flags;
#ifdef TD_STRICT_ALIGNMENT
    OdIntPtr m_data[odmax(sizeof(MetafilePtrArray),sizeof(OdRxObject*)) / sizeof(OdIntPtr)];
#else
    OdUInt8 m_data[odmax(sizeof(MetafilePtrArray),sizeof(OdRxObject*))];
#endif
  };

protected:
  OdGsHlBranchPtr m_hlBranch;

  virtual void propagateLayerChanges(OdGsViewImpl& view);
protected:
  /** \details
    Returns a reference to the metafile array of this EntityNode object as an array of MetafilePtr.
  */
  MetafilePtrArray& metafileArray();

  mutable MetafileHolder m_metafile;
  /** \details
    Returns a reference to the metafile array of this EntityNode object as an array of MetafilePtr.
  */
  const MetafilePtrArray& metafileArray() const;

  /** \details
    Returns the array the objects in the metafile array of this EntityNode object.
  */
  OdGsEntityNode::MetafilePtr metafile() const;

  void setExtentsFromMetafiles();
public:
  static OdGsEntityNode::MetafilePtr getEmptyMetafile(OdUInt32 flags, OdGsLayerNode *pLayerNode = NULL);

  /** \details
    Returns the specified MetafilePtr from the metafile array of this EntityNode object.
    \param metafileIndex [in]  Metafile index.
  */
  OdGsEntityNode::MetafilePtr metafileAt(int metafileIndex) const;
  
  /** \details
    Sets the metafile array for this EntityNode object from the specified array of objects.
    \param metafiles [in] Array of metafile objects.
  */
  void setMetafile(OdGsEntityNode::Metafile* metafiles);
  
  /** \details
    Sets the specified entry in the metafile array of this EntityNode object.
    \param metafileIndex [in]  Metafile index.
    \param pMetafile [in]  Pointer to the Metafile object
  */
  void setMetafileAt(int metafileIndex, OdGsEntityNode::Metafile* pMetafile);

  /** \details
    Convert a RegenType dependent cache to Viewport dependent cache.
    \param ctx [in]  Current update context.
  */
  void convertToViewportDependent(const OdGsUpdateContext& ctx);

  /** \details
    This enumeration used with OdGsEntityNode::metafile method.
  */
  enum MetafileCompatibilityLevel
  {
    kSkipCheckCompatible = 0, // Avoid check of viewport compatibility.
    kCheckViewChanges    = 1, // Check for view changes.
    kCheckCompatibleView = 2, // Avoid check for viewp changes, but find compatible metafile for other viewport.
    kFindCompatible      = 3  // Check for view changes and try to find compatible for other viewports.
  };

  /** \details
    Returns metafile pointer compatible with requrested view configuration.
    \param view [in]  View object for which metafile is to be returned.
    \param ctx [in]  Current update context.
    \param bFindCompatible [in]  Check for viewport changes if and only if true.
  */
  OdGsEntityNode::Metafile* metafile(const OdGsViewImpl& view, OdGsUpdateContext* ctx,
                                     MetafileCompatibilityLevel findCompatibleLevel = kFindCompatible);
  /** \details
    Sets metafile for current view configuration.
    \param ctx [in]  Current update context.
    \param metafiles [in]  Pointer onto new metafile object.
  */
  void setMetafile(OdGsUpdateContext& ctx, OdGsEntityNode::Metafile* metafiles);
  OdRxObject* findCompatibleCache(const OdGsViewImpl& keyView, OdGsUpdateContext* ctx, OdUInt32 awareMask = 0);
protected:
  // m_flags (don't add new flags here, we already utilized ~30 bits)
  enum
  {
    kHasExtents         = OdGsNode::kLastFlag << 1,
    kSpatiallyIndexed   = OdGsNode::kLastFlag << 2,
    kInWorkset          = OdGsNode::kLastFlag << 3,
    kSingleThreaded     = OdGsNode::kLastFlag << 4,
    kOwned              = OdGsNode::kLastFlag << 5,
    kDrawableRegenDraw  = OdGsNode::kLastFlag << 6,
    kEntityUnerased     = OdGsNode::kLastFlag << 7,
    kInvisible          = OdGsNode::kLastFlag << 8,
    kExtentsOutOfMS     = OdGsNode::kLastFlag << 9,
    kNonSelectable      = OdGsNode::kLastFlag << 10,
    kSkipSelection      = OdGsNode::kLastFlag << 11,
    kErased             = OdGsNode::kLastFlag << 12,
    kDrawableIsXref     = OdGsNode::kLastFlag << 13,
    kMultilistNextPtr   = OdGsNode::kLastFlag << 14,
    kLastFlag           = kMultilistNextPtr
  };
  // m_entFlags (add all new flags related to EntityNode here, keep some free space inside m_flags for inherited classes)
  enum
  {
    kLineweightBit1     = (1 << 0), // reserve 8 bits for maximal lineweight
    kLineweightBit2     = (1 << 1), kLineweightOffset = 0,
    kLineweightBit3     = (1 << 2),
    kLineweightBit4     = (1 << 3),
    kLineweightBit5     = (1 << 4),
    kLineweightBit6     = (1 << 5),
    kLineweightBit7     = (1 << 6),
    kLineweightBit8     = (1 << 7),
    kLineweightBitMask  = kLineweightBit1 | kLineweightBit2 | kLineweightBit3 | kLineweightBit4 | kLineweightBit5 | kLineweightBit6 | kLineweightBit7 | kLineweightBit8,
    kLightSourceOwner   = (1 << 8), // owning at least one light source
    kRegenOnHighlight   = (1 << 9), // cache require regeneration during highlighting state change
    kMarkedByUpdateManager = (1 << 10),
    kLastEntFlag        = kMarkedByUpdateManager
  };
  // m_markToSkipFlags
  enum
  {
    kMarkedToSkipFirst  = 0x00000001,
    kMarkedToSkipLast   = kMarkedToSkipFirst << 30,
    kMarkedToSkipAll    = 0x7FFFFFFF
  };

public:
  ODRX_DECLARE_MEMBERS(OdGsEntityNode);

  /** \param pModel [in]  Pointer to the BaseModel object.
    \param pUnderlyingDrawable [in]  Pointer to the underlying Drawable object.
  */
  OdGsEntityNode(OdGsBaseModel* pModel, const OdGiDrawable* pUnderlyingDrawable, bool bSetGsNode = false);
public:
  ~OdGsEntityNode();

  /** \details
    Sets the next entity for this EntityNode object to the specified EntityNode object.
    \param pNextEntity [in]  Pointer to the next EntityNode.
  */
  void setNextEntity(OdGsEntityNode* pNextEntity);
  
  /** \details
    Sets the next entity for this EntityNode object to the specified EntityNode object.
    \param pNextEntity [in]  Pointer to the next EntityNode.
    \param nVpId [in]  Viewport ID.
  */
  void setNextEntity(OdUInt32 nVpId, OdGsEntityNode* pNextEntity);
  /** \details
    Returns the next entity for this EntityNode object.
    \param nVpId [in]  Viewport ID.
  */
  OdGsEntityNode* nextEntity();
  OdGsEntityNode* nextEntity(OdUInt32 nVpId);
  /** \details
    Returns the next entity for this EntityNode object.
    \param nVpId [in]  Viewport ID.
  */
  const OdGsEntityNode* nextEntity() const;
  const OdGsEntityNode* nextEntity(OdUInt32 nVpId) const;
  /** \details
    Returns number of next entities.
  */
  OdUInt32 numNextEntity() const;
  /** \details
    Enable multiple entity links.
  */
  void enableMultipleNextEntities(OdUInt32 nMaxVpId);

  void update(OdGsUpdateContext& ctx, OdGsContainerNode* pParent,
      OdSiSpatialIndex* pParentIndex);

  void updateVisible( OdGsViewImpl* pViewImpl );
  
  virtual OdGsEntityNode::MetafilePtr regenMetafile(OdGsUpdateContext &ctx, OdUInt32 vpDepFlags);

  /** \details
    Returns the Aware flags for the specified Viewport for this ContainerNode object.
    
    \param viewportId [in]  Viewport ID. 
    
    \remarks
    awareFlags() returns a combination of zero or more of the following:
    
    <table>
    Name                            Value
    kSuppressViewportDraw           0x01
    kDrawInvisibleEnts              0x02
    kDrawLayerOff                   0x04
    kDrawLayerFrozen                0x08
    kIgnoreFillPlane                0x10
    kFillPlaneSet                   0x20
    </table>
  */
  virtual OdUInt32 awareFlags(OdUInt32 viewportId) const;

  virtual ENodeType nodeType() const { return kEntityNode; }
  
  virtual bool isReference() const { return false; }
  virtual bool isLight() const { return false; }

  void display(OdGsDisplayContext& ctx);

  void select(OdGsBaseVectorizer& view);

  void invalidate(OdGsContainerNode* pParent, OdGsViewImpl* pView, OdUInt32 mask);

  bool extents(OdGeExtents3d& extents) const;

  /** \details
    Returns true if and only if this EntityNode object is spatially indexed.
    \param nVpId [in]  Viewport ID.
  */
  bool spatiallyIndexed() const;
  
  /** \details
    Returns true if and only if this EntityNode object is spatially indexed.
    \param nVpId [in]  Viewport ID.
  */
  bool spatiallyIndexed(OdUInt32 nVpId) const;
  
  /** \details
    Controls the spatial indexing of this EntityNode object.

    \param spatiallyIndexed [in]  Controls spatially indexed.
  */
  void setSpatiallyIndexed(bool spatiallyIndexed);
  
  /** \details
    Controls the spatial indexing of this EntityNode object.
    \param nVpId [in]  Viewport ID.
    \param spatiallyIndexed [in]  Controls spatially indexed.
  */
  void setSpatiallyIndexed(OdUInt32 nVpId, bool spatiallyIndexed);

  /** \details
    Returns true if and only if this EntityNode object is the child of a ContainerNode object.
  */
  bool owned() const;
  
  /** \details
    Returns true if and only if this EntityNode object is the child of a ContainerNode object.
    \param nVpId [in]  Viewport ID.
  */
  bool owned(OdUInt32 nVpId) const;
  /** \details
    Controls the owned status of this EntityNode object.

    \param owned [in]  Controls owned status.
  */
  void setOwned(bool owned);
  
  /** \details
    Controls the owned status of this EntityNode object.
    \param nVpId [in]  Viewport ID.
    \param owned [in]  Controls owned status.
  */
  void setOwned(OdUInt32 nVpId, bool owned);

  /** \details
    Returns true if and only if this EntityNode object is marked by Update Manager.
  */
  bool markedByUpdateManager() const;

  /** \details
    Controls the marking by Update Manager status of this EntityNode object.
    \param marked [in]  Controls marked status.
  */
  void setMarkedByUpdateManager( bool marked );

  /** \details
    Returns true if and only if the underlying GiDrawable object(s) have extents.
  */
  bool hasExtents() const;

  const OdGeExtents3d& extents() const;

  /** \details
    Returns true if the node has no graphical content.
  */
  virtual bool isEmpty();

  /** \details
    Returns true if the node has no graphical content.
  */
  virtual bool hasFrozenLayers();

  /** \details
    Returns true if and only if this EntityNode object is marked to be skipped
    with the given mask (thread).
  */
  inline bool markedToSkip(unsigned mask);

  /** \details
    Controls the MarkToSkip status of this EntityNode object.
    \param markToSkip [in]  Controls MarkToSkip.
    \param mask [in]  the mask(thread).
  */
  inline void markToSkip(unsigned mask, bool markToSkip);

  inline void markToSkipAll();

  static unsigned markToSkipMask(OdUInt32 threadIndex)
  {
    ODA_ASSERT(threadIndex >= 0 && threadIndex < 31);
    return kMarkedToSkipFirst << threadIndex;
  }

  void destroy(){}

  virtual bool layersChanged(OdGsViewImpl& view) const;

  bool markedAsNonSelectable() const;
  void markAsNonSelectable(bool val);
  bool isMarkedSkipSelection() const;
  void markSkipSelection(bool val);
  bool isMarkedErased() const;
  void markErased(bool val);

  bool isInWorkset() const;
  void setInWorkset(bool val);

  bool isRegenOnDraw() const;
  void setRegenOnDraw(bool val);

  bool isSingleThreaded() const;
  void setSingleThreaded(bool val);

  virtual void highlight(bool bDoIt, bool bWholeBranch);
  void setHlBranch(OdGsHlBranch* pBr) { m_hlBranch = pBr; }
  OdGsHlBranch* hlBranch() { return m_hlBranch; }
  const OdGsHlBranch* hlBranch() const { return m_hlBranch; }

  virtual void playAsGeometry(OdGsBaseVectorizer& view, EMetafilePlayMode eMode);

  bool extents(const OdGsView* pView, OdGeExtents3d& ext) const;
  OdDb::LineWeight getMaxLineweightUsed() const;
  void addMaxLineweightUsed(OdDb::LineWeight lwd);
  void setMaxLineweightUsed(OdDb::LineWeight lwd);

  bool entityUnerased() const;
  void setEntityUnerased(bool flag);

  bool isInvisible() const;

  bool isExtentsOutOfModelSpace() const;
  void setExtentsOutOfModelSpace(bool flag);

  void resetInvalidVpFlag();

  void setAsLightSourceOwner(bool bSet);
  bool isLightSourceOwner() const;

  bool isRequireRegenOnHighlightChange() const;

public:
  virtual void makeStock();
  virtual void releaseStock();
  virtual void propagateLayerChangesStock();

  bool saveNodeState(OdGsFiler *pFiler, OdGsBaseVectorizer *pVectorizer = NULL) const;
  bool loadNodeState(OdGsFiler *pFiler, OdGsBaseVectorizer *pVectorizer = NULL);
protected:
  bool findMetafileInStock(OdGsUpdateContext& ctx);
  void insertToSpatialIndex(OdUInt32 nVpId, OdSiSpatialIndex& parentIndex,
                            const OdGeExtents3d& prevExtents);
  inline void setExtentsFlags();

private:
  MetafileHolder m_mfStock;
  volatile int m_markToSkipFlags;
protected:
  // This flags willn't increase memory usage (at least on Win32) since utilize free space in 8 bytes alignment (__alignof(entity_node) == 8).
  OdUInt32 m_entFlags;

  OdGsDCRect extentsToPixels( const OdGeExtents3d& ext, OdGsViewImpl* pViewImpl );

public:
  void addContentToUpdateManager( OdUInt32 viewportId, OdGsUpdateManager* pManager, const UpdateManagerContext& context );
  bool isMetafilesCompatible( const OdGsViewImpl* pView1, const OdGsViewImpl* pView2, OdGsUpdateContext* ctx );
};

/** \details
    This template class is a specialization of the OdSmartPtr class for OdGsEntityNode object pointers. 
*/
typedef OdSmartPtr<OdGsEntityNode> OdGsEntityNodePtr;


inline void OdGsEntityNode::setNextEntity(OdGsEntityNode* pNextEntity)
{
  if (GETBIT(m_flags, kMultilistNextPtr))
    throw OdError(eInvalidIndex);
  m_pNextEntity = pNextEntity;
}

inline void OdGsEntityNode::setNextEntity(OdUInt32 nVpId, OdGsEntityNode* pNextEntity)
{
  if (GETBIT(m_flags, kMultilistNextPtr))
    nextEntEntry(nVpId)->m_pNextEntity = pNextEntity;
  else
    m_pNextEntity = pNextEntity;
}

inline void OdGsEntityNode::enableMultipleNextEntities(OdUInt32 nMaxVpId)
{
  if (nMaxVpId == 0xFFFFFFFF) nMaxVpId = 0;
  if ((numNextEntity() <= nMaxVpId) || ((nMaxVpId == 0) && GETBIT(m_flags, kMultilistNextPtr)))
    allocNextEntityArray(nMaxVpId);
}

inline OdGsEntityNode* OdGsEntityNode::nextEntity()
{
  if (GETBIT(m_flags, kMultilistNextPtr))
    throw OdError(eInvalidIndex);
  return m_pNextEntity;
}

inline OdGsEntityNode* OdGsEntityNode::nextEntity(OdUInt32 nVpId)
{
  if (GETBIT(m_flags, kMultilistNextPtr))
    return nextEntEntry(nVpId)->m_pNextEntity;
  //if (nVpId > 0) // Normal case, since we can request from non-VpDep container
  //  throw OdError(eInvalidIndex);
  return m_pNextEntity;
}

inline const OdGsEntityNode* OdGsEntityNode::nextEntity() const
{
  if (GETBIT(m_flags, kMultilistNextPtr))
    throw OdError(eInvalidIndex);
  return m_pNextEntity;
}

inline const OdGsEntityNode* OdGsEntityNode::nextEntity(OdUInt32 nVpId) const
{
  if (GETBIT(m_flags, kMultilistNextPtr))
    return nextEntEntry(nVpId)->m_pNextEntity;
  //if (nVpId > 0) // Normal case, since we can request from non-VpDep container
  //  throw OdError(eInvalidIndex);
  return m_pNextEntity;
}

inline OdUInt32 OdGsEntityNode::numNextEntity() const
{
  if (GETBIT(m_flags, kMultilistNextPtr))
    return (OdUInt32)OdPtrToInt32(firstEntEntry()->m_pNextEntity);
  return 1;
}

inline bool OdGsEntityNode::hasExtents() const
{
  return GETBIT(m_flags, kHasExtents);
}

inline const OdGeExtents3d& OdGsEntityNode::extents() const
{
  return m_extents;
}

inline bool OdGsEntityNode::markedToSkip(unsigned mask)
{
  ODA_ASSERT((mask >= kMarkedToSkipFirst) && (mask <= kMarkedToSkipLast));
  //atomic function is required, called in MT mode (display)
#ifndef TD_SINGLE_THREAD
  return (mask & OdInterlockedExchangeAdd(&m_markToSkipFlags, 0)) != 0;
#else
  return GETBIT(m_markToSkipFlags, mask);
#endif
}

inline void OdGsEntityNode::markToSkip(unsigned mask, bool bSkip)
{
  ODA_ASSERT((mask >= kMarkedToSkipFirst) && (mask <= kMarkedToSkipLast));
  if(markedToSkip(mask) != bSkip)
  {
    //atomic function is required, called in MT mode (display)
#ifndef TD_SINGLE_THREAD
    if(bSkip)
      OdInterlockedExchangeAdd(&m_markToSkipFlags, mask);
    else
      OdInterlockedExchangeAdd(&m_markToSkipFlags, -(int)mask);
#else
    SETBIT(m_markToSkipFlags, mask, bSkip);
#endif
  }
}

inline void OdGsEntityNode::markToSkipAll()
{
  //atomic function is NOT required, is NOT called in MT mode
  m_markToSkipFlags = kMarkedToSkipAll;
}

inline bool OdGsEntityNode::spatiallyIndexed() const
{
  if (GETBIT(m_flags, kMultilistNextPtr))
    throw OdError(eInvalidIndex);
  return GETBIT(m_flags, kSpatiallyIndexed);
}

inline bool OdGsEntityNode::spatiallyIndexed(OdUInt32 nVpId) const
{
  if (GETBIT(m_flags, kMultilistNextPtr))
    return GETBIT(nextEntEntry(nVpId)->m_entFlags, kSpatiallyIndexed);
  //if (nVpId > 0) // Normal case, since we can request from non-VpDep container
  //  throw OdError(eInvalidIndex);
  return GETBIT(m_flags, kSpatiallyIndexed);
}

inline void OdGsEntityNode::setSpatiallyIndexed(bool bIndexed)
{
  if (GETBIT(m_flags, kMultilistNextPtr))
    throw OdError(eInvalidIndex);
  SETBIT(m_flags, kSpatiallyIndexed, bIndexed);
}

inline void OdGsEntityNode::setSpatiallyIndexed(OdUInt32 nVpId, bool bIndexed)
{
  if (GETBIT(m_flags, kMultilistNextPtr))
    SETBIT(nextEntEntry(nVpId)->m_entFlags, kSpatiallyIndexed, bIndexed);
  else
    SETBIT(m_flags, kSpatiallyIndexed, bIndexed);
}

inline bool OdGsEntityNode::owned() const
{
  if (GETBIT(m_flags, kMultilistNextPtr))
    throw OdError(eInvalidIndex);
  return GETBIT(m_flags, kOwned);
}

inline bool OdGsEntityNode::owned(OdUInt32 nVpId) const
{
  if (GETBIT(m_flags, kMultilistNextPtr))
    return GETBIT(nextEntEntry(nVpId)->m_entFlags, kOwned);
  //if (nVpId > 0) // Normal case, since we can request from non-VpDep container
  //  throw OdError(eInvalidIndex);
  return GETBIT(m_flags, kOwned);
}

inline void OdGsEntityNode::setOwned(bool bOwned)
{
  if (GETBIT(m_flags, kMultilistNextPtr))
    throw OdError(eInvalidIndex);
  SETBIT(m_flags, kOwned, bOwned);
}

inline void OdGsEntityNode::setOwned(OdUInt32 nVpId, bool bOwned)
{
  if (GETBIT(m_flags, kMultilistNextPtr))
    SETBIT(nextEntEntry(nVpId)->m_entFlags, kOwned, bOwned);
  else
    SETBIT(m_flags, kOwned, bOwned);
}

inline OdGsEntityNode::MetafilePtrArray& OdGsEntityNode::metafileArray()
{
  return m_metafile.getArray();
}

inline const OdGsEntityNode::MetafilePtrArray& OdGsEntityNode::metafileArray() const
{
  return m_metafile.getArray();
}

inline OdGsEntityNode::MetafilePtr OdGsEntityNode::metafile() const
{
  return m_metafile.get();
}

inline OdGsEntityNode::MetafilePtr OdGsEntityNode::metafileAt(int metafileIndex) const
{
  return metafileArray()[metafileIndex];
}

#define VPMETAFILEHOLDERARRAY_DESTRUCTOR ~MetafilePtrArray

inline void OdGsEntityNode::setMetafile(OdGsEntityNode::Metafile* pMetafile)
{
  m_metafile.set(pMetafile);
}

inline bool OdGsEntityNode::entityUnerased() const
{
  return GETBIT(m_flags, kEntityUnerased);
}

inline void OdGsEntityNode::setEntityUnerased(bool flag)
{
  SETBIT(m_flags, kEntityUnerased, flag);
}

inline bool OdGsEntityNode::isInvisible() const
{
  return GETBIT(m_flags, kInvisible);
}

inline void OdGsEntityNode::resetInvalidVpFlag()
{
  if (invalidVp())
  {
    setInvalidVp(false);
  }
}

inline bool OdGsEntityNode::markedAsNonSelectable() const
{
  return GETBIT(m_flags, kNonSelectable);
}

inline void OdGsEntityNode::markAsNonSelectable(bool val)
{
  SETBIT(m_flags, kNonSelectable, val);
}

inline bool OdGsEntityNode::isMarkedSkipSelection() const
{
  return GETBIT(m_flags, kSkipSelection);
}

inline void OdGsEntityNode::markSkipSelection(bool val)
{
  SETBIT(m_flags, kSkipSelection, val);
}

inline bool OdGsEntityNode::isMarkedErased() const
{
  return GETBIT(m_flags, kErased);
}

inline void OdGsEntityNode::markErased(bool val)
{
  SETBIT(m_flags, kErased, val);
}

inline bool OdGsEntityNode::Metafile::isEmpty() const
{
  for(const OdGsGeomPortion* ptr = &m_first; ptr; ptr = ptr->m_pNext)
  {
    if(ptr->m_pGsMetafile.get())
      return false;
  }
  return true;
}

inline bool OdGsEntityNode::isExtentsOutOfModelSpace() const
{
  return GETBIT(m_flags, kExtentsOutOfMS);
}

inline void OdGsEntityNode::setExtentsOutOfModelSpace(bool val)
{
  SETBIT(m_flags, kExtentsOutOfMS, val);
}

inline void OdGsEntityNode::setAsLightSourceOwner(bool bSet)
{
  SETBIT(m_entFlags, kLightSourceOwner, bSet);
}

inline bool OdGsEntityNode::isLightSourceOwner() const
{
  return GETBIT(m_entFlags, kLightSourceOwner);
}

inline bool OdGsEntityNode::isRequireRegenOnHighlightChange() const
{
  return GETBIT(m_entFlags, kRegenOnHighlight);
}

inline bool OdGsEntityNode::isInWorkset() const
{
  return GETBIT(m_flags, kInWorkset);
}

inline void OdGsEntityNode::setInWorkset(bool val)
{
  SETBIT(m_flags, kInWorkset, val);
}

inline bool OdGsEntityNode::isSingleThreaded() const
{
  return GETBIT(m_flags, kSingleThreaded);
}

inline void OdGsEntityNode::setSingleThreaded(bool val)
{
  SETBIT(m_flags, kSingleThreaded, val);
}

inline bool OdGsEntityNode::isRegenOnDraw() const
{
  return GETBIT(m_flags, kDrawableRegenDraw);
}

inline void OdGsEntityNode::setRegenOnDraw(bool val)
{
  SETBIT(m_flags, kDrawableRegenDraw, val);
}

inline void OdGsEntityNode::highlight(bool bDoIt, bool bWholeBranch)
{
  OdGsNode::highlight(bDoIt, bWholeBranch);
  if(hlBranch() && (!bDoIt || bWholeBranch))
    setHlBranch(NULL);
}

inline void OdGsEntityNode::setExtentsFlags()
{
  SETBIT(m_flags, kHasExtents, m_extents.isValidExtents());
  SETBIT(m_flags, kExtentsOutOfMS, !OdSi::properExtents(m_extents));
}

inline OdDb::LineWeight OdGsEntityNode::getMaxLineweightUsed() const
{
  return (OdDb::LineWeight)((m_entFlags & kLineweightBitMask) >> kLineweightOffset);
}

inline void OdGsEntityNode::addMaxLineweightUsed(OdDb::LineWeight lwd)
{
  if (lwd)
  {
    OdDb::LineWeight maxLwd = getMaxLineweightUsed();
    if (lwd > maxLwd)
      setMaxLineweightUsed(lwd);
  }
}

inline void OdGsEntityNode::setMaxLineweightUsed(OdDb::LineWeight lwd)
{
  m_entFlags = ((OdUInt32(lwd) << kLineweightOffset) & kLineweightBitMask) | (m_entFlags & ~kLineweightBitMask);
}

inline bool OdGsEntityNode::markedByUpdateManager() const
{
  return GETBIT(m_entFlags, kMarkedByUpdateManager);
}

inline void OdGsEntityNode::setMarkedByUpdateManager(bool bMarked)
{
  SETBIT(m_entFlags, kMarkedByUpdateManager, bMarked);
}

#include "TD_PackPop.h"

#endif // __OD_GS_ENTITY_NODE__
