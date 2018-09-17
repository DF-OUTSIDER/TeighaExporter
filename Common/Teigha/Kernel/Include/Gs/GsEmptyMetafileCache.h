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

#ifndef __OD_GS_EMPTY_METAFILE_CACHE__
#define __OD_GS_EMPTY_METAFILE_CACHE__

#include "TD_PackPush.h"

#include "Gs/GsEntityNode.h"
#define STL_USING_MAP
#include "OdaSTL.h"

class OdGsLayerNode;

/** <group OdGs_Classes>
*/
class OdGsEmptyMetafileCache
{
  private:
    typedef std::map<OdUInt32, OdGsEntityNode::MetafilePtr> OdMmap;
    // Global map
    friend class OdGsModuleObject;
    GS_STATIC_EXPORT static OdMmap * s_mfMap;
    GS_STATIC_EXPORT static OdMutex* s_mtMfMap;
    static void rxInit();
    static void rxUninit();
    // Local map
    OdMmap m_mfMap;
  public:
    OdGsEmptyMetafileCache();

    static void get(OdGsEntityNode::MetafilePtr &pPtr, OdUInt32 flags, OdGsLayerNode *pLayerNode = NULL);
  protected:
    void getLocal(OdGsEntityNode::MetafilePtr &pPtr, OdUInt32 flags, OdGsLayerNode &pLayerNode);
    static void getGlobal(OdGsEntityNode::MetafilePtr &pPtr, OdUInt32 flags, OdGsLayerNode *pLayerNode);
    static void getImpl(OdGsEntityNode::MetafilePtr &pPtr, OdMmap &mfMap, OdUInt32 flags, OdGsLayerNode *pLayerNode);
};

#define ODGS_EMFCACHE_DEFINE_MEMBERS() \
  GS_STATIC_EXPORT OdGsEmptyMetafileCache::OdMmap* OdGsEmptyMetafileCache::s_mfMap = NULL; \
  GS_STATIC_EXPORT                        OdMutex* OdGsEmptyMetafileCache::s_mtMfMap = NULL; \
  void OdGsEmptyMetafileCache::rxInit() \
  { \
    s_mfMap   = new OdMmap(); \
    s_mtMfMap = new OdMutex(); \
  } \
  void OdGsEmptyMetafileCache::rxUninit() \
  { \
    delete s_mfMap;   s_mfMap = NULL; \
    delete s_mtMfMap; s_mtMfMap = NULL; \
  }

#include "TD_PackPop.h"

#endif // __OD_GS_EMPTY_METAFILE_CACHE__

// Inlines

#if defined(__OD_GS_EMPTY_METAFILE_CACHE__) && (defined(__OD_GS_EMPTY_METAFILE_CACHE_INLINES_INCLUDE__) || !defined(__OD_GS_LAYER_NODE__))

#ifndef __OD_GS_EMPTY_METAFILE_CACHE_INLINES__
#define __OD_GS_EMPTY_METAFILE_CACHE_INLINES__

#include "Gs/GsLayerNode.h"

inline
OdGsEmptyMetafileCache::OdGsEmptyMetafileCache()
{
}

inline
void OdGsEmptyMetafileCache::get(OdGsEntityNode::MetafilePtr &pPtr, OdUInt32 flags, OdGsLayerNode *pLayerNode)
{
  if (pLayerNode)
    pLayerNode->m_emfCache.getLocal(pPtr, flags, *pLayerNode);
  else
    getGlobal(pPtr, flags, pLayerNode);
}

inline
void OdGsEmptyMetafileCache::getLocal(OdGsEntityNode::MetafilePtr &pPtr, OdUInt32 flags, OdGsLayerNode &pLayerNode)
{
  TD_AUTOLOCK_P_DEF(pLayerNode.m_mt);
  getImpl(pPtr, m_mfMap, flags, &pLayerNode);
}

inline
void OdGsEmptyMetafileCache::getGlobal(OdGsEntityNode::MetafilePtr &pPtr, OdUInt32 flags, OdGsLayerNode *pLayerNode)
{
  OdMutexPtrAutoLock autoLock(odThreadsCounter() ? s_mtMfMap : NULL);
  getImpl(pPtr, *s_mfMap, flags, pLayerNode);
}

inline
void OdGsEmptyMetafileCache::getImpl(OdGsEntityNode::MetafilePtr &pPtr, OdMmap &mfMap, OdUInt32 flags, OdGsLayerNode *pLayerNode)
{
  OdMmap::iterator it = mfMap.find(flags);
  if (it != mfMap.end())
  {
    pPtr = it->second;
    return;
  }
  pPtr = OdGiRxObjectReImpl<OdGsEntityNode::Metafile>::createObject();
  pPtr->m_nAwareFlags = flags;
  pPtr->m_first.m_pLayer = pLayerNode;
  mfMap[flags] = pPtr;
}

#endif // __OD_GS_EMPTY_METAFILE_CACHE_INLINES__

#endif // defined(__OD_GS_EMPTY_METAFILE_CACHE__) && (defined(__OD_GS_EMPTY_METAFILE_CACHE_INLINES_INCLUDE__) || !defined(__OD_GS_LAYER_NODE__))
