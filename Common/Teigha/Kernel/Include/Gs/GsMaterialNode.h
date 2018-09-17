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

#ifndef __OD_GS_MATERIAL_NODE__
#define __OD_GS_MATERIAL_NODE__

#include "TD_PackPush.h"

#include "Gs/GsNode.h"
#include "Gi/GiMaterial.h"
#include "Gi/GiMaterialTraitsData.h"
#include "ThreadsCounter.h"
#define STL_USING_MAP
#include "OdaSTL.h"

// Materials cached in GsModel, but in separate cache, instead all geometry.

// GsModel caches hierarchy:
// OdGsBaseModel
//   OdGsContainerNode <-- OdGsNode - geometry cache
//     OdGsEntityNode <-- OdGsNode
//       OdGsLightNode - has separate array, but stored into geometry cache too
//   OdGsLayerNode <-- OdGsNode - layers cache
// OdGsBaseMode::OdGsMaterialCache
//   OdGsMaterialNode <-- OdGsNode - materials cache

/** <group OdGs_Classes> 
*/
class GS_TOOLKIT_EXPORT OdGsMaterialNode : public OdGsNode
{
protected:
  /** \details
      No operation.
  */
  void update(OdGsUpdateContext& ctx, OdGsContainerNode* pParent,
      OdSiSpatialIndex* pParentIndex);

  /** \details
      No operation.
  */
  void display(OdGsDisplayContext& ctx);

  /** \details
      No operation.
  */
  bool extents(OdGeExtents3d& ) const;

  /** \details
      No operation.
  */
  virtual void propagateLayerChanges(OdGsViewImpl& view);

  OdGiMaterialTraitsData m_materialTraits;
  bool m_dataInitialized;
  bool m_dataModified;
  struct DataEntry
  {
    bool m_cachedDataModified;
    OdRxObjectPtr m_pCachedData;

    DataEntry() : m_cachedDataModified(true) { }
  };
  typedef std::map<const void *, DataEntry> CacheMap;
  CacheMap m_cacheMap;
  OdMutexPtr m_pCacheMutex;

  mutable OdGsMaterialNode* m_pMatPrev;
  mutable OdGsMaterialNode* m_pMatNext;
public:
  ODRX_DECLARE_MEMBERS(OdGsMaterialNode);

  /** \param pModel [in]  Pointer to the BaseModel object.
    \param pUnderlyingDrawable [in]  Pointer to the underlying Drawable object.
  */
  OdGsMaterialNode(OdGsBaseModel* pModel, const OdGiDrawable* pUnderlyingDrawable, bool bSetGsNode = true);
  ~OdGsMaterialNode();

  void destroy() {}

  OdGiMaterialTraitsData& materialTraits();
  const OdGiMaterialTraitsData& materialTraits() const;

  // Called from OdGsNode if material data changed at update.
  // May be used by inherited nodes to update device-dependant information.
  virtual void onModified(OdGsBaseVectorizer& view);

  /** \details
      No operation.
  */
  void invalidate(OdGsContainerNode* pParent, OdGsViewImpl* pView, OdUInt32 mask);

  /** \details
  */
  virtual void update(OdGsBaseVectorizer& view);

  OdGsMaterialNode *nextNode() const;
  OdGsMaterialNode *prevNode() const;
  OdGsMaterialNode *setNextNode(OdGsMaterialNode *newNode);
  OdGsMaterialNode *setPrevNode(OdGsMaterialNode *newNode);

  bool isDataModified() const;

  bool isCachedDataModified(OdGsBaseVectorizer& view) const;
  void setCachedDataUpdated(OdGsBaseVectorizer& view);

  const OdRxObject *cachedData(OdGsBaseVectorizer& view) const;
  void setCachedData(OdGsBaseVectorizer& view, OdRxObject *newCachedData);

  OdMutexPtr &cacheMutex() { return m_pCacheMutex; }

  virtual ENodeType nodeType() const { return kMaterialNode; }

  bool saveNodeState(OdGsFiler *pFiler, OdGsBaseVectorizer *pVectorizer = NULL) const;
  bool loadNodeState(OdGsFiler *pFiler, OdGsBaseVectorizer *pVectorizer = NULL);
};

// Cache for material nodes
#include "GsMaterialCache.h"

#include "TD_PackPop.h"

#endif // __OD_GS_MATERIAL_NODE__
