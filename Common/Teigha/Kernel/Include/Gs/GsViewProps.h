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

#ifndef __OD_GS_VIEW_PROPS__
#define __OD_GS_VIEW_PROPS__

#include "GsViewPropsDef.h"
#include "Ge/GeMatrix3d.h"
#include "DbStubPtrArray.h"
#include "GsViewLocalId.h"

#include "TD_PackPush.h"

class OdGsViewImpl;

/** \details
    
    Corresponding C++ library: TD_Gs

    <group !!RECORDS_TD_APIRef> 
*/
struct GS_TOOLKIT_EXPORT ViewProps
{
public:
    ViewProps();
    OdUInt32              m_vpId;
    OdDbStub*             m_vpObjectId;
    OdGiRegenType         m_regenType;
    OdGsView::RenderMode  m_renderMode;
    OdGeMatrix3d          m_worldToEye;
    OdGePoint3d           m_cameraLocation;
    OdGePoint3d           m_cameraTarget;
    OdGeVector3d          m_cameraUpVector;
    OdGeVector3d          m_viewDir;
    OdGePoint2d           m_vpLowerLeft;
    OdGePoint2d           m_vpUpperRight;
    double                m_deviation[5];
    double                m_frontClip;
    double                m_backClip;
    OdDbStubPtrArray      m_frozenLayers;
    double                m_linetypeScaleMultiplier;
    const void*           m_renderModule;
    OdDbStub*             m_annoScale;
    OdIntPtr              m_filterFunction;
    OdDbStub*             m_visualStyle;
    OdUInt32              m_nViewChanges;

    void set(const OdGsViewImpl& view);
    OdUInt32 difference(const ViewProps& props) const;
    bool isCompatibleWith(const ViewProps& props, OdUInt32 nFlags,
        bool bCompareFrozenLayers = true) const;
    bool isInvalid() const { return GETBIT(m_nViewChanges, 0x80000000); }
    void setInvalid(bool bValue) { SETBIT(m_nViewChanges, 0x80000000, bValue); }
};
typedef OdArray<ViewProps> ViewPropsArray;

/** \details
    
    Corresponding C++ library: TD_Gs

    <group OdGs_Classes> 
*/
class GS_TOOLKIT_EXPORT ViewRefs
{
public:
  ViewRefs() : m_nViews(0), m_nMaxVpId(-1) {}
  OdUInt32 numViews() const { return m_nViews; }
  void add(OdUInt32 viewportId)
  {
    if(viewportId < m_data.size())
    {
      if(!m_data[viewportId])
        ++m_nViews;
    }
    else
    {
      m_data.insert(m_data.end(), viewportId + 1 - m_data.size(), 0);
      ++m_nViews;
    }
    ++m_data[viewportId];
    m_nMaxVpId = -1;
  }
  void remove(OdUInt32 viewportId)
  {
    ODA_ASSERT(m_nViews);
    ODA_ASSERT(m_data.size() > viewportId);
    ODA_ASSERT(m_data[viewportId] > 0);
    if((--m_data[viewportId]) == 0)
    {
      --m_nViews;
      if(!m_nViews)
        m_data.clear();
    }
    m_nMaxVpId = -1;
  }
  int maxViewportId() const
  {
    if (m_nMaxVpId < 0)
    {
      for(unsigned i = m_data.size(); i; )
        if(m_data[--i])
          return m_nMaxVpId = (int)i;
    }
    return m_nMaxVpId;
  }
  unsigned numDifferentViewports() const
  {
    unsigned count = 0;
    for(unsigned i = 0; i < m_data.size(); ++i)
      if(m_data[i])
        ++count;
    return count;
  }
  void clear() { m_data.clear(); m_nViews = 0; m_nMaxVpId = -1; }
#ifndef ODGS_LOCAL_IDS
  void set(OdGsDevice& device, OdUInt32 nViews);
#else
  void set(const OdGsBaseModel *pModel);
#endif
  bool contains(OdUInt32 viewportId) const
  {
    return (viewportId < m_data.size()) && (m_data[viewportId] != 0);
  }

  class const_iterator
  {
  public:
    const_iterator(const ViewRefs& refs): m_refs(refs), m_pos(-1){}
    const_iterator(const const_iterator& c): m_refs(c.m_refs), m_pos(c.m_pos){}
    void start() { m_pos = -1; findNext(); }
    bool done() const { return m_pos < 0 || m_pos >= (int)(m_refs.m_data.size()); }
    OdUInt32 viewportId() const { ODA_ASSERT(!done()); return m_pos; }
    void step() { findNext(); }

  protected:
    void findNext()
    {
      ODA_ASSERT(m_pos > -2);
      for(++m_pos; (m_pos < (int)m_refs.m_data.size()) && !m_refs.m_data[m_pos]; ++m_pos);
    }
  protected:
    const ViewRefs& m_refs;
    int m_pos;
  };

  friend class const_iterator;
protected:
  typedef OdIntArray _marray;
  _marray m_data;
  OdUInt32 m_nViews;
  mutable int m_nMaxVpId; // cache maxViewportId
};

/** \details
        
    Corresponding C++ library: TD_Gs

    <group OdGs_Classes> 
*/
class StockProps
{
public:
  StockProps(): m_deviceClass(0){}
  StockProps(const StockProps& c): m_viewRefs(c.m_viewRefs),
      m_viewProps(c.m_viewProps), m_deviceClass(c.m_deviceClass){}

  const ViewProps& viewProps(unsigned i) const { return m_viewProps.getAt(i); }
  unsigned numViewProps() const { return m_viewProps.size(); }
  const ViewRefs& viewRefs() const { return m_viewRefs; }
  const OdRxClass* deviceClass() const { return m_deviceClass; }

  bool set(const ViewRefs& refs, const ViewPropsArray& props,
      const OdRxClass* deviceClass)
  {
      if(!deviceClass)
          return false;//no device to identity cached data
      unsigned i = 0;
      for(; (i < props.size()) && props.getAt(i).isInvalid(); ++i);
      if(i == props.size())
          return false; //all props are invalid, won't be used anyway
      m_viewRefs = refs;
      m_viewProps = props;
      m_deviceClass = deviceClass;
      return true;
  }
  void clear() { m_viewRefs.clear(); m_viewProps.clear(); }

protected:
  ViewRefs m_viewRefs;
  ViewPropsArray m_viewProps;
  const OdRxClass* m_deviceClass;
};

inline bool sameSortedArrays(const OdDbStubPtrArray& ar1, const OdDbStubPtrArray& ar2)
{
  if (ar1.size() == ar2.size())
  {
    if (ar1.getPtr() != ar2.getPtr())
      return ::memcmp(ar1.getPtr(), ar2.getPtr(), sizeof(OdDbStub*) * ar2.size()) == 0;
    else
      return true;
  }
  return false;
}

inline bool ViewProps::isCompatibleWith(
    const ViewProps& props, OdUInt32 nMask
    ,bool bCompareFrozenLayers
    ) const
{
  //NS: optimization
  if((!nMask) && !bCompareFrozenLayers)
    return true;
  if(GETBIT(nMask, kVpID))
  {
    if((m_vpId!=props.m_vpId) || (m_vpObjectId!=props.m_vpObjectId))
      return false;
  }
  if(GETBIT(nMask, kVpRegenType))
  {
    if(m_regenType!=props.m_regenType)
      return false;
  }
  if(GETBIT(nMask, kVpRenderMode))
  {
    if(m_renderMode!=props.m_renderMode)
      return false;
  }
  if(bCompareFrozenLayers && (GETBIT(nMask, kVpFrozenLayers) ||
     !sameSortedArrays(m_frozenLayers, props.m_frozenLayers)))
  {
    return false;
  }
  if(GETBIT(nMask, kVpFrontBack))
  {
    if(m_frontClip!=props.m_frontClip || m_backClip!=props.m_backClip)
      return false;
  }
  if(GETBIT(nMask, kVpCamLocation))
  {
    if(m_cameraLocation!=props.m_cameraLocation)
      return false;
  }
  if(GETBIT(nMask, kVpCamTarget))
  {
    if(m_cameraTarget!=props.m_cameraTarget)
      return false;
  }
  if(GETBIT(nMask, kVpMaxDevForCircle))
  {
    if(OdNegative(m_deviation[kOdGiMaxDevForCircle] - props.m_deviation[kOdGiMaxDevForCircle]))
      return false;
  }
  if(GETBIT(nMask, kVpCamUpVector))
  {
    if(m_cameraUpVector!=props.m_cameraUpVector)
      return false;
  }
  if(GETBIT(nMask, kVpCamViewDir))
  {
    if(m_viewDir!=props.m_viewDir)
      return false;
  }
  if(GETBIT(nMask, kVpViewport))
  {
    if(m_vpLowerLeft!=props.m_vpLowerLeft || m_vpUpperRight!=props.m_vpUpperRight)
      return false;
  }
  if(GETBIT(nMask, kVpWorldToEye))
  {
    if(m_worldToEye!=props.m_worldToEye)
      return false;
  }
  if(GETBIT(nMask, kVpLtypeScaleMult))
  {
    if(!OdEqual(m_linetypeScaleMultiplier, props.m_linetypeScaleMultiplier))
      return false;
  }
  if(GETBIT(nMask, kVpRenderModule))
  {
    if(m_renderModule!=props.m_renderModule)
      return false;
  }
  if(GETBIT(nMask, kVpAnnoScale))
  {
    if(m_annoScale!=props.m_annoScale)
      return false;
  }
  if(GETBIT(nMask, kVpFilterFunction))
  {
    if(m_filterFunction!=props.m_filterFunction)
      return false;
  }
  if(GETBIT(nMask, kVpVisualStyle))
  {
    if(m_visualStyle!=props.m_visualStyle)
      return false;
  }
  return true;
}

#include "TD_PackPop.h"

#endif // __OD_GS_VIEW_PROPS__
