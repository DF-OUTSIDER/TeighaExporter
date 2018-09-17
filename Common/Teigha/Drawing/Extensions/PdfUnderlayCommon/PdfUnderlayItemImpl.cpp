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


#include "OdaCommon.h"
#include "PdfUnderlayItemImpl.h"
#include "Ge/GePolyline3d.h"
#include "Gi/GiWorldDraw.h"
#include "Gi/GiRasterWrappers.h"
#include "Gi/GiBaseVectorizer.h"
#include "StaticRxObject.h"

#include "PdfDirectWrapper.h"

ODRX_NO_CONS_DEFINE_MEMBERS(PdfUnderlayItemImpl, OdDbUnderlayItem);

PdfUnderlayItemImpl::PdfUnderlayItemImpl()
  : m_Units(OdDb::kUnitsUndefined)
  , m_nItem(0)
  , m_bLayersCached(false)
{
  desc()->module()->addRef();
}

PdfUnderlayItemImpl::~PdfUnderlayItemImpl()
{
  desc()->module()->release();
}

const OdArray<OdUnderlayLayer> &PdfUnderlayItemImpl::accessToLayers() const
{
  if (false == m_bLayersCached)
  {
    m_bLayersCached = true;
    
    // fill the cache at first request
    int n_layers = m_pWrapper->getLevelsCount(m_nItem);
    m_LayerCache.reserve(n_layers);
    OdUnderlayLayer layer;
    for (int i = 0; i < n_layers; i++)
    {
      layer.setName(m_pWrapper->getLevelName(m_nItem, i));
      layer.setState((m_pWrapper->isLevelEnabled(m_nItem, i) == 0) ? OdUnderlayLayer::kOff : OdUnderlayLayer::kOn);
      m_LayerCache.append(layer);
    }
  }

  return m_LayerCache;
}

OdResult PdfUnderlayItemImpl::loadData()
{
  return eOk;
}

OdString PdfUnderlayItemImpl::getName() const
{
  return m_pWrapper.isNull() ? OdString::kEmpty : m_pWrapper->getItemName(m_nItem);
}

OdGiRasterImagePtr PdfUnderlayItemImpl::getThumbnail() const
{
  return m_pWrapper->getThumbnail(m_nItem);
}

OdResult PdfUnderlayItemImpl::getMediaBox(OdGePoint2d& min, OdGePoint2d& max) const
{
  return m_pWrapper->getMediaBox(m_nItem, min, max);
}

void PdfUnderlayItemImpl::getExtents(OdGePoint2d& min, OdGePoint2d& max) const
{
  m_pWrapper->getItemExtents(m_nItem, min, max);
}

OdDb::UnitsValue PdfUnderlayItemImpl::getUnits() const
{
  /* OdDb::kUnitsUndefined always */
  return m_Units;
}

bool PdfUnderlayItemImpl::usingPartialContent() const
{
  return false;
}

bool PdfUnderlayItemImpl::worldDraw(OdGiWorldDraw* pVd, const OdDbUnderlayDrawContext& context) const
{
  return m_pWrapper->worldDraw(m_nItem, pVd, context);
}

void PdfUnderlayItemImpl::viewportDraw(OdGiViewportDraw* pVd, const OdDbUnderlayDrawContext& context) const
{
  m_pWrapper->viewportDraw(m_nItem, pVd, context);

  //////////////////////////////////////////////////////////////////////////
  //CORE-11805 PdfUnderlay cannot be selected by click on internal geometry.
  if (!pVd->context()->isPlotGeneration())
  {
    m_pWrapper->drawGeometryForSelection(m_nItem, pVd, context);
  }
  //////////////////////////////////////////////////////////////////////////
}

OdGeMatrix3d PdfUnderlayItemImpl::modelTransform() const
{
  return OdGeMatrix3d(); // Not callable
  // Here may be added transformations for units
}

OdResult PdfUnderlayItemImpl::getOsnapPoints(const OdGeMatrix3d& modelToWorld,
                                             OdDb::OsnapMode     osnapMode,
                                             OdGsMarker    gsSelectionMark,
                                             const OdGePoint3d&  pickPoint,
                                             const OdGePoint3d&  lastPoint,
                                             const OdGeMatrix3d& viewXform,
                                             const OdGeMatrix3d& /*ucs*/,
                                             OdGePoint3dArray&   snapPoints,
                                             OdIntArray&         /*geomIds*/) const
{
  return m_pWrapper->getOsnapPoints( m_nItem,
    modelToWorld, osnapMode, gsSelectionMark, pickPoint,
    lastPoint, viewXform, snapPoints);
}

OdResult PdfUnderlayItemImpl::getSnapGeometry(OdDbDatabase* pDb, OdGiGeometry *pGrabber, const OdArray<OdUnderlayLayer>& layers, const OdGePoint3d *pickPtMc) const
{
  return m_pWrapper->getSnapGeometry(pDb, m_nItem, pGrabber, layers, pickPtMc);
}

OdResult PdfUnderlayItemImpl::getExplodeGeometry(OdGiGeometry *pGrabber, OdArray<OdUnderlayLayer> layers, bool bIgnoreImages) const
{
  return m_pWrapper->getExplodeGeometry(m_nItem, pGrabber, layers, bIgnoreImages);
}

//OdRxObjectPtr PdfUnderlayItemImpl::getSubEntityAtGsMarker(const OdGeMatrix3d& , OdGsMarker ) const
//{
//  return OdRxObjectPtr(); 
//}

//OdResult PdfUnderlayItemImpl::intersectWith(const OdGeMatrix3d& ,
//                                            const OdRxObject* ,
//                                            OdDb::Intersect ,
//                                            const OdGePlane* ,
//                                            OdGePoint3dArray& ,
//                                            OdGsMarker ,
//                                            OdGsMarker ) const
//{
//  return eOk; 
//}

OdUInt32 PdfUnderlayItemImpl::underlayLayerCount() const 
{
  return accessToLayers().size();
}

OdResult PdfUnderlayItemImpl::getUnderlayLayer(int idx, OdUnderlayLayer& layer) const 
{
  int numLayers = (int)underlayLayerCount();
  if (idx < 0 || idx >= numLayers)
    return eInvalidInput;

  layer = accessToLayers()[idx];

  return eOk; 
}

