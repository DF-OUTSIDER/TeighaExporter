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

#ifndef _PDFUNDERLAYITEMIMPL_INCLUDED_
#define _PDFUNDERLAYITEMIMPL_INCLUDED_

#include "TD_PackPush.h"

#include "DbUnderlayHost.h"
#include "PdfDirectWrapper.h"

class OdGiViewportDraw;
class OdGiGeometry;

/** \details
  Library:  Source code provided.
  <group PdfServices_Classes>
*/
class PdfUnderlayItemImpl : public OdDbUnderlayItem
{
  OdGiRasterImagePtr       m_Thumbnail;
  OdDb::UnitsValue         m_Units;

  mutable bool                     m_bLayersCached;
  mutable OdArray<OdUnderlayLayer> m_LayerCache;
  const OdArray<OdUnderlayLayer> &accessToLayers() const;

  mutable PdfDirectWrapperPtr m_pWrapper;
  OdInt32                  m_nItem; // used for fast access to item info in wrapper

protected:
  PdfUnderlayItemImpl();
  virtual OdResult loadData();
  ODRX_DECLARE_MEMBERS(PdfUnderlayItemImpl);

public:
  ~PdfUnderlayItemImpl();

  static OdSmartPtr<PdfUnderlayItemImpl> createItem(PdfDirectWrapperPtr wrapper, int n_item, OdString fileName, OdString ActiveFileName, OdString password)
  {
    OdSmartPtr<PdfUnderlayItemImpl> pItem = OdRxObjectImpl<PdfUnderlayItemImpl>::createObject();

    pItem->m_pWrapper = wrapper;
    pItem->m_nItem = n_item;
    pItem->loadData();

    return pItem;
  }

  // OdDbUnderlayItem methods

  virtual OdString getName() const;

  virtual OdGiRasterImagePtr getThumbnail() const;

  virtual OdResult getMediaBox(OdGePoint2d& min, OdGePoint2d& max) const;

  virtual void getExtents(OdGePoint2d& min, OdGePoint2d& max) const;

  virtual OdDb::UnitsValue getUnits() const;

  virtual bool usingPartialContent() const;

  virtual bool worldDraw(OdGiWorldDraw* pVd, const OdDbUnderlayDrawContext& context) const;

  virtual void viewportDraw(OdGiViewportDraw* pVd, const OdDbUnderlayDrawContext& context) const;

  virtual OdGeMatrix3d modelTransform() const;

  virtual OdResult getOsnapPoints(const OdGeMatrix3d& modelToWorld,
    OdDb::OsnapMode     osnapMode,
    OdGsMarker    gsSelectionMark,
    const OdGePoint3d&  pickPoint,
    const OdGePoint3d&  lastPoint,
    const OdGeMatrix3d& viewXform,
    const OdGeMatrix3d& ucs,
    OdGePoint3dArray&   snapPoints,
    OdIntArray&         geomIds) const;

  OdResult getSnapGeometry(OdDbDatabase *pDb, OdGiGeometry *pGrabber, const OdArray<OdUnderlayLayer>& layers, const OdGePoint3d* pickPtMc) const;

  virtual OdResult getExplodeGeometry(OdGiGeometry *pGrabber, OdArray<OdUnderlayLayer> layers, bool bIgnoreImages) const;

  //virtual OdRxObjectPtr getSubEntityAtGsMarker(const OdGeMatrix3d& modelToWorld, OdGsMarker gsSelectionMark) const;

  //virtual OdResult intersectWith(const OdGeMatrix3d& modelToWorld,
  //                               const OdRxObject* pEnt,
  //                               OdDb::Intersect intType,
  //                               const OdGePlane* projPlane,
  //                               OdGePoint3dArray& points,
  //                               OdGsMarker thisGsMarker,
  //                               OdGsMarker otherGsMarker) const;

  virtual OdUInt32 underlayLayerCount() const;
  virtual OdResult getUnderlayLayer(int /*idx*/, OdUnderlayLayer& /*layer*/) const;

};
typedef OdSmartPtr<PdfUnderlayItemImpl> PdfUnderlayItemImplPtr;

#include "TD_PackPop.h"

#endif // _PDFUNDERLAYITEMIMPL_INCLUDED_
