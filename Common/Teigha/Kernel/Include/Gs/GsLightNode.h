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

#ifndef __OD_GS_LIGHT_NODE__
#define __OD_GS_LIGHT_NODE__

#include "TD_PackPush.h"

#include "Gs/GsEntityNode.h"
#include "Gi/GiLightTraitsData.h"

#define STL_USING_MAP
#include "OdaSTL.h"

// Lights are entities that have some special meaning

/** <group OdGs_Classes> 
*/
class GS_TOOLKIT_EXPORT OdGsLightNode : public OdGsEntityNode
{
protected:
  OdGeMatrix3d m_model2World; // In case light isn't in model space
  OdGiLightTraitsData* m_pLightTraits;
  OdGsNode *m_pLightParent;
  typedef OdVector<OdIntPtr, OdMemoryAllocator<OdIntPtr> > LightInsertPath;
  LightInsertPath m_lightInsertParents;
  std::map< OdUInt32, OdGiLightTraitsData* > m_mapVpDepTraitsData;

  enum
  {
    kViewportDependentLight      = (OdGsEntityNode::kLastFlag << 1),
    kViewportDependentNeedUpdate = (OdGsEntityNode::kLastFlag << 2),
    kLightDrawableIsInvisible    = (OdGsEntityNode::kLastFlag << 3),
    kLastFlag                    = kLightDrawableIsInvisible
  };
protected:
  void update();

  void contextualUpdate(OdGsUpdateContext& ctx);

  void adjustViewportDependentLightTraitsData( OdGiLightTraitsVpDepData* vdData, OdGiLightTraitsData* data );

  void clearVpDepTraitsMap();

public:
  ODRX_DECLARE_MEMBERS(OdGsLightNode);

  ~OdGsLightNode();
  OdGsLightNode(OdGsBaseModel* pModel, const OdGiDrawable* pUnderlyingDrawable, bool bSetGsNode = true);
public:

  void setModelTransform(const OdGeMatrix3d& xform) { m_model2World = xform; }
  OdGeMatrix3d modelTransform() const { return m_model2World; }

  void setLightOwner(OdGsUpdateContext& ctx, OdGsContainerNode* pParent);
  bool isOwnedBy(OdGsUpdateContext& ctx, OdGsContainerNode* pParent) const;
  bool isOwnedBy(const OdGsNode *pOwner) const { return m_pLightParent == pOwner; }
  OdGsNode *getLightOwner() const { return m_pLightParent; }

  OdGiLightTraitsData::LightType lightType();
  virtual bool isLight() const;

  OdGiPointLightTraitsData* pointLightTraitsData( OdUInt32 viewportID = ULONG_MAX );
  OdGiSpotLightTraitsData* spotLightTraitsData( OdUInt32 viewportID = ULONG_MAX );
  OdGiDistantLightTraitsData* distantLightTraitsData( OdUInt32 viewportID = ULONG_MAX );
  OdGiWebLightTraitsData* webLightTraitsData( OdUInt32 viewportID = ULONG_MAX );
  OdGiLightTraitsData* lightTraitsData( OdUInt32 viewportID = ULONG_MAX );

  // OdGsNode virtual overrides
  
  virtual void invalidate(OdGsContainerNode* pParent, OdGsViewImpl* pView, OdUInt32 mask);
  virtual void update(OdGsUpdateContext& ctx, OdGsContainerNode* pParent,
      OdSiSpatialIndex*);

  virtual bool saveClientNodeState(OdGsFiler *pFiler, OdGsBaseVectorizer *pVectorizer) const;
  virtual bool loadClientNodeState(OdGsFiler *pFiler, OdGsBaseVectorizer *pVectorizer);

  /** \details
    Updates viewport-dependent properties
    \param pView [in] View implementation
  */
  void updateViewportDependent( OdGsViewImpl* pView );

  /** \details
    Returns true if and only if specified drawable is a light entity.
    \param pDrawable [in]  Pointer onto Drawable object.
  */
  static bool drawableIsLight(const OdGiDrawable *pDrawable);
};

typedef OdSmartPtr<OdGsLightNode> OdGsLightNodePtr;

inline OdGiLightTraitsData::LightType OdGsLightNode::lightType()
{
  return OdGiLightTraitsData::drawableLightType(underlyingDrawable());
}

inline bool OdGsLightNode::isLight() const
{
  return true;
}

inline OdGiPointLightTraitsData* OdGsLightNode::pointLightTraitsData( OdUInt32 viewportID )
{
  if (!m_pLightTraits || ((m_pLightTraits->type() != OdGiLightTraitsData::kPointLight) && (m_pLightTraits->type() != OdGiLightTraitsData::kWebLight)))
    return NULL;
  if(GETBIT(m_flags, kViewportDependentLight))
  {
    std::map< OdUInt32, OdGiLightTraitsData* >::iterator it = m_mapVpDepTraitsData.find( viewportID );
    if( it != m_mapVpDepTraitsData.end() )
    {
      return (OdGiPointLightTraitsData*)(it->second);
    }
  }
  return (OdGiPointLightTraitsData*)m_pLightTraits;
}
inline OdGiSpotLightTraitsData* OdGsLightNode::spotLightTraitsData( OdUInt32 viewportID )
{
  if (!m_pLightTraits || (m_pLightTraits->type() != OdGiLightTraitsData::kSpotLight))
    return NULL;
  if(GETBIT(m_flags, kViewportDependentLight))
  {
    std::map< OdUInt32, OdGiLightTraitsData* >::iterator it = m_mapVpDepTraitsData.find( viewportID );
    if( it != m_mapVpDepTraitsData.end() )
    {
      return (OdGiSpotLightTraitsData*)(it->second);
    }
  }
  return (OdGiSpotLightTraitsData*)m_pLightTraits;
}
inline OdGiDistantLightTraitsData* OdGsLightNode::distantLightTraitsData( OdUInt32 viewportID )
{
  if (!m_pLightTraits || (m_pLightTraits->type() != OdGiLightTraitsData::kDistantLight))
    return NULL;
  if(GETBIT(m_flags, kViewportDependentLight))
  {
    std::map< OdUInt32, OdGiLightTraitsData* >::iterator it = m_mapVpDepTraitsData.find( viewportID );
    if( it != m_mapVpDepTraitsData.end() )
    {
      return (OdGiDistantLightTraitsData*)(it->second);
    }
  }
  return (OdGiDistantLightTraitsData*)m_pLightTraits;
}
inline OdGiWebLightTraitsData* OdGsLightNode::webLightTraitsData( OdUInt32 viewportID )
{
  if (!m_pLightTraits || (m_pLightTraits->type() != OdGiLightTraitsData::kWebLight))
    return NULL;
  if(GETBIT(m_flags, kViewportDependentLight))
  {
    std::map< OdUInt32, OdGiLightTraitsData* >::iterator it = m_mapVpDepTraitsData.find( viewportID );
    if( it != m_mapVpDepTraitsData.end() )
    {
      return (OdGiWebLightTraitsData*)(it->second);
    }
  }
  return (OdGiWebLightTraitsData*)m_pLightTraits;
}
inline OdGiLightTraitsData* OdGsLightNode::lightTraitsData( OdUInt32 viewportID )
{
  if(GETBIT(m_flags, kViewportDependentLight))
  {
    std::map< OdUInt32, OdGiLightTraitsData* >::iterator it = m_mapVpDepTraitsData.find( viewportID );
    if( it != m_mapVpDepTraitsData.end() )
    {
      return it->second;
    }
  }
  return m_pLightTraits;
}

inline void OdGsLightNode::adjustViewportDependentLightTraitsData( OdGiLightTraitsVpDepData* vdData, OdGiLightTraitsData* data )
{
  if( !GETBIT(m_flags, kViewportDependentLight) ) return;
  if( !data || !vdData ) return;
  data->setOn( vdData->vpDepOn() && !GETBIT(m_flags, kLightDrawableIsInvisible) );
  data->setIntensity( data->intensity() * ( 1.0 - vdData->vpDepDimming() ) );
}

inline bool OdGsLightNode::drawableIsLight(const OdGiDrawable *pDrawable)
{
  return (OdGiLightTraitsData::drawableLightType(pDrawable) != OdGiLightTraitsData::kInvalidLight);
}

inline void OdGsLightNode::clearVpDepTraitsMap()
{
  for( std::map< OdUInt32, OdGiLightTraitsData* >::iterator it = m_mapVpDepTraitsData.begin(); it != m_mapVpDepTraitsData.end(); ++it )
  {
    OdGiLightTraitsData::deleteLightTraitsData(it->second);
  }
  m_mapVpDepTraitsData.clear();
}

#include "TD_PackPop.h"

#endif // __OD_GS_LIGHT_NODE__
