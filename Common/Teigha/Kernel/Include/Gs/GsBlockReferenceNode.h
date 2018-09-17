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

#ifndef __OD_GS_BLOCKREFERENCE_NODE__
#define __OD_GS_BLOCKREFERENCE_NODE__

#include "TD_PackPush.h"

#include "GsEntityNode.h"
#include "OdVector.h"

class OdGsSpQueryContext;

/** \details
        
    Corresponding C++ library: TD_SpatialIndex

    <group OdSi_Classes> 
*/
class GS_TOOLKIT_EXPORT OdSiRecursiveVisitor : public OdSiVisitor
{
public:
  OdSiRecursiveVisitor(): m_pQuery(NULL), m_worldToNode(NULL), m_nodeToWorld(NULL),
      m_worldToModel(NULL), m_modelToWorld(NULL){}

  const OdSiShape* query() const { return m_pQuery; }
  void resetQuery(const OdSiShape* pQuery) { m_pQuery = pQuery; }

  const OdGeMatrix3d* worldToNode() const { return m_worldToNode; }
  void setWorldToNode(const OdGeMatrix3d* tf) { m_worldToNode = tf; }
  const OdGeMatrix3d* nodeToWorld() const { return m_nodeToWorld; }
  void setNodeToWorld(const OdGeMatrix3d* tf) { m_nodeToWorld = tf; }
  const OdGeMatrix3d* worldToModel() const { return m_worldToModel; }
  void setWorldToModel(const OdGeMatrix3d* tf) { m_worldToModel = tf; }
  const OdGeMatrix3d* modelToWorld() const { return m_modelToWorld; }
  void setModelToWorld(const OdGeMatrix3d* tf) { m_modelToWorld = tf; }

protected:
  const OdSiShape* m_pQuery;
  const OdGeMatrix3d* m_worldToNode;
  const OdGeMatrix3d* m_nodeToWorld;
  const OdGeMatrix3d* m_worldToModel;
  const OdGeMatrix3d* m_modelToWorld;
};

typedef OdSiRecursiveVisitor OdSiSelBaseVisitor;

/** \details
        
    Corresponding C++ library: TD_SpatialIndex

    <group OdSi_Classes> 
*/
class OdSiRecursiveVisitorTf
{
public:
  OdSiRecursiveVisitorTf(OdSiRecursiveVisitor& v, const OdGeMatrix3d& mat);
  ~OdSiRecursiveVisitorTf();
  operator OdSiRecursiveVisitor*() { return &m_visitor; }

protected:
  OdSiRecursiveVisitor& m_visitor;
private:
  const OdSiShape* m_prevQuery;
  std::auto_ptr<OdSiShape> m_transformedQuery;
};
typedef OdSiRecursiveVisitorTf OdSiSelectVisitorTf;

/** \details
        
  Corresponding C++ library: TD_SpatialIndex

  <group OdSi_Classes> 
*/
class OdGsSpQueryContextTf: public OdSiRecursiveVisitorTf
{
public:
  OdGsSpQueryContextTf(OdSiRecursiveVisitor& v, const OdGeMatrix3d& objectTf, const OdGeMatrix3d& objectTfInv,
      const OdGeScale3d& scale);
  ~OdGsSpQueryContextTf();

protected:
  OdGeMatrix3d m_nodeToWorld;
  OdGeMatrix3d m_worldToNode;
  const OdGeMatrix3d* m_prevNodeToWorld;
  const OdGeMatrix3d* m_prevWorldToNode;
  OdGeMatrix3d m_modelToWorld;
  OdGeMatrix3d m_worldToModel;
  const OdGeMatrix3d* m_prevModelToWorld;
  const OdGeMatrix3d* m_prevWorldToModel;
};

// forward declarations
class OdGsBlockReferenceNodeImpl;
class OdGsBlockNode;
class WorldDrawBlockRef;
class OdGsUpdateState;

/** \details
    This class is a service class for handling of viewport aware flags array.
    
    Corresponding C++ library: TD_Gs

    <group OdGs_Classes> 
*/
class GS_TOOLKIT_EXPORT OdGsAwareFlagsArray
{
public:
  OdGsAwareFlagsArray(){}
  enum
  {
    kChildrenNotUpToDate = 0x80000000
  };
  void setChildrenUpToDate(bool childrenUpToDate, const OdUInt32 nVpID);
  bool childrenUpToDate(OdUInt32 nVpID) const;
  OdUInt32 get(OdUInt32 nVpID) const;
  void set(OdUInt32 nVpID, OdUInt32 flags);
  void clear() { m_vpAwareFlags.clear(); }
  bool isEmpty() const { return m_vpAwareFlags.isEmpty(); }
  OdUInt32 numAwareFlags() const { return m_vpAwareFlags.size(); }
  bool areInvalid(OdUInt32 nVpID) const;

protected:
  bool findFlag(OdUInt32 nVpID) const
  { return nVpID < m_vpAwareFlags.size(); }

private:
  // viewport aware flags
  OdUInt32Vector m_vpAwareFlags;
};

/** \details
    This class is the base class for implementation of GS reference nodes.
    Derivatives of this class implement different model of storing referenced objects.

    Corresponding C++ library: Gs
    <group OdGs_Classes> 
*/
class GS_TOOLKIT_EXPORT OdGsBlockReferenceNodeImpl
{
public:
  OdGsBlockReferenceNodeImpl();
  void* operator new(size_t);
  void  operator delete(void*);

  void addRef()
  {
    ++m_nRefCounter;
  }
  void release()
  {
	ODA_ASSERT((m_nRefCounter > 0));
	if (!(--m_nRefCounter))
      delete this;
  }
  long numRefs() const { return m_nRefCounter; }

  // Main methods, used for OdGsNode interface implementation

  //pBlock can be NULL.
  //If is not NULL only when it is passed directly from the function where the drawable is open.
  //If it is NULL and it is required then use blockNode.underlyingDrawable() 
  virtual void updateSubitems(OdGsUpdateContext& ctx,
      OdGsBlockNode& blockNode, const OdGiDrawable* pBlock) = 0;
  virtual void display(OdGsDisplayContext& ctx, bool isHighlightedAll) = 0;

  //NS: following methods return true if operation succeded (invalidation
  //or destroying leave the object valid). If they return false, it means that
  //OdGsNode should release the instance and recreate m_pImpl.
  virtual bool invalidate(OdGsContainerNode* pParent, OdGsViewImpl* pView,
	  OdUInt32 mask) = 0;
  virtual bool destroy() = 0;
  virtual bool destroySubitems() = 0;

  virtual bool select(OdGsBaseVectorizer& view, OdSiSelBaseVisitor* pVisitor,
	  bool bHasExtents, OdGsView::SelectionMode mode) = 0;
  virtual void highlight(bool bDoIt, bool bWholeBranch) = 0;
  // Methods for traversing internal subitems
  virtual OdGsEntityNode* firstEntity() = 0;
  virtual void playAsGeometry(OdGsBaseVectorizer& view, EMetafilePlayMode eMode) = 0;
  virtual bool isSharedReference() const { return false; }
  virtual bool isSharedDefinition() const { return false; }
  virtual bool layersChanged(OdGsViewImpl& view) const;

  virtual void makeStock() = 0;
  virtual void releaseStock() = 0;
  virtual void propagateLayerChangesStock() = 0;
  virtual void doSpatialQuery(OdGsSpQueryContext& ctx) = 0;

  virtual bool saveNodeImplState(OdGsFiler *pFiler, OdGsBaseVectorizer *pVectorizer) const = 0;
  virtual bool loadNodeImplState(OdGsFiler *pFiler, OdGsBaseVectorizer *pVectorizer, OdGsBaseModel *pModel) = 0;
  virtual void postprocessNodeImplLoading(OdGsFiler * /*pFiler*/) { }
protected:
  virtual ~OdGsBlockReferenceNodeImpl();

protected:
  OdRefCounter m_nRefCounter;
};

/** \details
        
    Corresponding C++ library: TD_Gs

    <group OdGs_Classes> 
*/
class GS_TOOLKIT_EXPORT OdGsBlockReferenceNode: public OdGsEntityNode,
	protected OdGsAwareFlagsArray
{
public:
  ODRX_DECLARE_MEMBERS(OdGsBlockReferenceNode);
  
  OdGsBlockReferenceNode(OdGsBaseModel* pModel, const OdGiDrawable* pDrawable, bool bSetGsNode = false);
public:
  ~OdGsBlockReferenceNode();
  
  // additional flags for processing of inserts
  enum
  {
    kValid                = OdGsEntityNode::kLastFlag << 1,
    kBrExcludeFromViewExt = OdGsEntityNode::kLastFlag << 2,
    kBrClipped            = OdGsEntityNode::kLastFlag << 3,
    kLastFlag             = kBrClipped
  };
  
  bool isValid() const { return GETBIT(m_flags, kValid); }
  void setValid(bool bSet) { SETBIT(m_flags, kValid, bSet); }
  bool excludeFromViewExt() const { return GETBIT(m_flags, kBrExcludeFromViewExt); }
  void setExcludeFromViewExt(bool bSet) { SETBIT(m_flags, kBrExcludeFromViewExt, bSet); }

  virtual bool isReference() const { return true; }
  virtual bool layersChanged(OdGsViewImpl& view) const;
  // implementation of OdGsEntityNode interface:
  virtual void update(OdGsUpdateContext& ctx, OdGsContainerNode* pParent,
      OdSiSpatialIndex* pParentIndex);
  virtual void propagateLayerChanges(OdGsViewImpl& view);

  virtual void invalidate(OdGsContainerNode* pParent, OdGsViewImpl* pView, OdUInt32 mask);
  virtual void destroy();
  virtual void destroySubitems();

  virtual bool select(OdGsBaseVectorizer& view, OdSiSelBaseVisitor*,
    OdGsView::SelectionMode mode);

  /** \details
  Add/release counter of highlighted children of the node, and set kHLT/kHLT_All flags if necessary
  \param nSubnodes [in]  number of subnodes
  \param bHighlight [in]  if true then highlight else unhighlight
  \param bAll [in]  if true then consider kHLT_All flag
  Returns true if and only if highlighting of the child caused changing of kHLT flag on the container.
  */
  bool highlightSubnodes(OdUInt32 nSubnodes, bool bHighlight, bool bAll);
  
  /** \details
      Returns the Aware flags for the specified Viewport for object.
      \param viewportId [in]  Viewport ID. 
  */
   virtual OdUInt32 awareFlags(OdUInt32 viewportId) const;

  /** \details
  Check Aware flags for the specified Viewport and return true if flags are invalid,
  i.e. cannot be used for determining awareness of the node about view properties.
  \param viewportId [in]  Viewport ID. 
  */
   virtual bool awareFlagsAreInvalid(OdUInt32 viewportId) const;
  
  /** \details
      Returns true if the node has no graphical content.
  */
  virtual bool isEmpty();

  virtual OdGsEntityNode* firstEntity();
  virtual OdGsEntityNode* firstAttrib() { return m_pFirstAttrib; }
  void updateImpl(OdGsUpdateContext& ctx, const OdGiDrawable* pInsert,
      OdSiSpatialIndex* pParentIndex);
  
  OdGsBlockNode* blockNode() const { return m_pBlockNode; }
  void clearBlockNode();

  virtual void makeStock();
  virtual void releaseStock();
  virtual void propagateLayerChangesStock();
  void updateBlockNode(const OdGiDrawable* pBlockTableRecord);
  //For testing purposes
  void* sharedDefinition();
  virtual bool isMInsert() const { return false; }
  static OdGsBlockReferenceNode* create(OdGsBaseModel* pModel, const OdGiDrawable* pDrawable);
  virtual void doDisplay(OdGsDisplayContext& ctx);
  virtual bool doSelect(OdGsBaseVectorizer& vect, OdGiDrawable* pDrw,
      OdSiSelBaseVisitor* pVisitor, OdGsView::SelectionMode mode);

  virtual void spatialQuery(const OdGsView& view, OdSiRecursiveVisitor* pVisitor);

protected:
  virtual void setChildrenUpToDate(bool childrenUpToDate, const OdUInt32 nVpID);
  virtual bool childrenUpToDate(OdUInt32 nVpID) const;
  virtual bool childrenRegenDraw(OdUInt32 nVpID) const;
  void updateAttribute(OdGsUpdateContext& ctx, OdGsEntityNode* pNode,
      const OdGiDrawable* pInsert, OdGsMarker marker);
  // Tool methods
  void setAwareFlags(OdUInt32 viewportId, OdUInt32 nAwareFlags);
  virtual void playAsGeometry(OdGsBaseVectorizer& view, EMetafilePlayMode eMode);
  void initBlockNode(const OdGiDrawable* pBlock);
  void setMetafile(OdGsUpdateContext& ctx, OdGsEntityNode::Metafile* pMf,
      OdUInt32 awareFlags, const OdGeExtents3d& ext);
  bool needRegen(OdUInt32 nVpID) const
  {
    if(!childrenUpToDate(nVpID))
        return true;
    OdUInt32 nAF = awareFlags(nVpID);
    if( nAF == 0xFFFFFFFF || (baseModel()->viewChanges(nVpID) & nAF) != 0 )
    {
      const_cast<OdGsBlockReferenceNode*>(this)->setChildrenUpToDate(false, nVpID);
      return true;
    }
    return false;
  }
  virtual void doUpdateImpl(OdGsUpdateContext& ctx, const OdGiDrawable* pBlockTableRecord, unsigned iInstance);
  void drawBlock(OdGsUpdateContext& ctx, const OdGiDrawable* pBlockTableRecord,
      OdGsBlockReferenceNodeImpl* pContainer,
      bool bEnableSharedGraphics);
  void postUpdateStep(OdGsUpdateState& state);
  void addEntPropsFromState(const OdGsUpdateState& state);
  void checkCancelledSharedRef(OdGsUpdateState& state);
  static void actionPostUpdateStep(void* pObj, OdGsUpdateState& state);
  static void actionAddEntPropsFromState(void* pObj, OdGsUpdateState& state);
  static void actionCheckCancelledSharedRef(void* pObj, OdGsUpdateState& state);
  static void actionRestartLocked(void* pObj, OdGsUpdateContext& ctx);
  virtual void doSpatialQuery(OdGsSpQueryContext& ctx);

  bool saveClientNodeState(OdGsFiler *pFiler, OdGsBaseVectorizer *pVectorizer) const;
  bool loadClientNodeState(OdGsFiler *pFiler, OdGsBaseVectorizer *pVectorizer);
  bool postprocessNodeLoading(OdGsFiler *pFiler);
private:
  void createImpl(const OdGsUpdateContext& ctx, bool bTryShared);
  void drawBlockImpl(OdGsUpdateContext& ctx, const OdGiDrawable* pBlockTableRecord,
      OdGsBlockReferenceNodeImpl* pContainer,
      bool bEnableSharedGraphics);

  bool isAProxy(OdGiDrawable *pDrw, const OdGiContext *pCtx = NULL) const;
protected:
  // Pointer to implementation object
  TPtr<OdGsBlockReferenceNodeImpl> m_pImpl;
  // Pointer to the node, corresponding to the block table record  
  OdGsBlockNode* m_pBlockNode;
  // list of attributes
  OdGsEntityNode* m_pFirstAttrib;

  friend class WorldDrawBlockRef;
  friend class GsDrawableNested;
  friend class OdGsSpQueryContext;
};

/** \details
            
    Corresponding C++ library: TD_Gs

    <group OdGs_Classes> 
*/
class GS_TOOLKIT_EXPORT OdGsMInsertBlockNode: public OdGsBlockReferenceNode
{
public:
  ODRX_DECLARE_MEMBERS(OdGsMInsertBlockNode);
  OdGsMInsertBlockNode(OdGsBaseModel* pModel, const OdGiDrawable* pInsert, bool bSetGsNode = false);
public:
  ~OdGsMInsertBlockNode();

  virtual void invalidate(OdGsContainerNode* pParent, OdGsViewImpl* pView, OdUInt32 mask);
  virtual void destroy();
  virtual void destroySubitems();

  virtual void highlight(bool bDoIt, bool bWholeBranch);

  virtual OdGsEntityNode* firstEntity();
  virtual OdGsEntityNode* firstAttrib();
  virtual bool layersChanged(OdGsViewImpl& view) const;
  virtual void playAsGeometry(OdGsBaseVectorizer& view, EMetafilePlayMode eMode);

  virtual void makeStock();
  virtual void releaseStock();
  virtual void propagateLayerChangesStock();
  virtual bool isMInsert() const { return true; }
  virtual void doUpdateImpl(OdGsUpdateContext& ctx,
    const OdGiDrawable* pBlockTableRecord, unsigned iInstance);
  OdGsEntityNode*& firstAttrib(unsigned iInstance) { return item(iInstance)->m_pFirstAttrib; }
  void stretchExtents();
  void setParams(const OdGeMatrix3d& blockTf, int nCols, int nRows, double sx, double sy)
  {
    m_blockTf = blockTf;
    m_nCols = nCols;
    m_nRows = nRows;
    m_sx = sx;
    m_sy = sy;
  }
  static void actionStretchExtents(void* pObj, OdGsUpdateState& state);
  virtual void doDisplay(OdGsDisplayContext& ctx);
  virtual bool doSelect(OdGsBaseVectorizer& vect, OdGiDrawable* pDrw,
      OdSiSelBaseVisitor* pVisitor, OdGsView::SelectionMode mode);

protected:
  void destroyCollection();

  bool selectSubitems(OdGsBaseVectorizer& view,
	  OdSiSelBaseVisitor* pVisitor, OdGsView::SelectionMode mode);
  bool selectAttributes(OdGsBaseVectorizer& view);

protected:
  class GS_TOOLKIT_EXPORT CollectionItem
  {
  public:
    CollectionItem() : m_nodeImp(0), m_pFirstAttrib(0) { }
    CollectionItem(const CollectionItem& c):
    m_nodeImp(c.m_nodeImp), m_pFirstAttrib(c.m_pFirstAttrib){}
    void copyFrom(const CollectionItem& c);

    OdGsBlockReferenceNodeImpl* imp() { return m_nodeImp; }
    const OdGsBlockReferenceNodeImpl* imp() const { return m_nodeImp; }
    OdGsEntityNode* firstAttrib() { return m_pFirstAttrib; }
    const OdGsEntityNode* firstAttrib() const { return m_pFirstAttrib; }

  public:
    TPtr<OdGsBlockReferenceNodeImpl> m_nodeImp;
    OdGsEntityNode* m_pFirstAttrib;

    bool operator ==(const CollectionItem &it2) const
    {
      return (m_nodeImp == it2.m_nodeImp) && (m_pFirstAttrib == it2.m_pFirstAttrib);
    }
    bool operator !=(const CollectionItem &it2) const
    {
      return (m_nodeImp != it2.m_nodeImp) || (m_pFirstAttrib != it2.m_pFirstAttrib);
    }
  };
  class GS_TOOLKIT_EXPORT Collection: public OdArray<CollectionItem>
  {
  public:
    Collection(): OdArray<CollectionItem>(){}
    Collection(const Collection& c);
    ~Collection();
  };
protected:
  OdGsBlockReferenceNodeImpl* getImpl(unsigned iInstance) { return item(iInstance)->m_nodeImp; }
  CollectionItem* item(unsigned iInstance);
  void clearParams() {m_nCols = m_nRows = 0; m_sx = m_sy = 0.; m_blockTf.setToIdentity(); }

  bool saveClientNodeState(OdGsFiler *pFiler, OdGsBaseVectorizer *pVectorizer) const;
  bool loadClientNodeState(OdGsFiler *pFiler, OdGsBaseVectorizer *pVectorizer);
  bool postprocessNodeLoading(OdGsFiler *pFiler);
protected:
  // Collection of implementation objects, representing clipped MINSERT.
  // (If MINSERT is not clipped, it's represented by only implementation 
  // object OdGsBlockReferenceNode::m_pImpl)
  Collection* m_pCollectionImpl;
  OdGeMatrix3d m_xModelToWorld;
  OdGeMatrix3d m_blockTf;
  int m_nCols;
  int m_nRows;
  double m_sx;
  double m_sy;
};

inline void OdGsBlockReferenceNode::clearBlockNode()
{
  m_pBlockNode = 0;
}

#include "TD_PackPop.h"

#endif // __OD_GS_REFERENCE_NODE__
