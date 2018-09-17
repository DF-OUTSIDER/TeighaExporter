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

#ifndef _DWFDBITEMIMPL_H_
#define _DWFDBITEMIMPL_H_

#include "Ge/GeExtents3d.h"
#include "Gi/GiRasterImage.h"
#include "DbUnderlayItem.h"
#include "DwfObjectImpl.h"

namespace DWFCore
{
  class DWFInputStream;
};

class DwfDbItemImpl : public OdRxObjectImpl<OdDbUnderlayItem>
{
  OdDwfObjectId m_idLayoutView;
  mutable OdDbBaseDatabasePtr m_pDbHolderForUnderlay;
  OdString m_Name;
  OdString m_FileName;
  OdString m_ActiveFileName;
  OdString m_Password;

  mutable OdStreamBufPtr m_pStream;
  mutable OdGiRasterImagePtr m_Thumbnail;
  mutable bool m_bThumbnailLoaded;
  mutable OdGeExtents3d m_Extents;
  mutable OdDb::UnitsValue m_Units;
  mutable bool m_bUnitsLoaded;
  mutable OdArray<OdUnderlayLayer> m_Layers; friend class OdDwfLayoutView;

public:
  DwfDbItemImpl();
  ~DwfDbItemImpl();
  ODRX_DECLARE_MEMBERS(DwfDbItemImpl);

  virtual void set(OdDwfObjectId idLayoutView, OdString Name, OdString fileName, OdString ActiveFileName, OdString password);

  // OdDbUnderlayItem methods :

  virtual OdString getName() const;

  virtual OdGiRasterImagePtr getThumbnail() const;

  virtual void getExtents(OdGePoint2d& min, OdGePoint2d& max) const;

  virtual OdDb::UnitsValue getUnits() const;

  virtual bool usingPartialContent() const;

  virtual bool worldDraw(OdGiWorldDraw* pVd, const OdDbUnderlayDrawContext& context) const;

  virtual void viewportDraw(OdGiViewportDraw* pVd, const OdDbUnderlayDrawContext& context) const;

  virtual OdGeMatrix3d modelTransform() const;

  virtual OdResult getOsnapPoints(const OdGeMatrix3d& modelToWorld,
    OdDb::OsnapMode     osnapMode,
    OdGsMarker   gsSelectionMark,
    const OdGePoint3d&  pickPoint,
    const OdGePoint3d&  lastPoint,
    const OdGeMatrix3d& viewXform,
    const OdGeMatrix3d& ucs,
    OdGePoint3dArray&   snapPoints,
    OdIntArray&         geomIds) const;

  virtual OdRxObjectPtr getSubEntityAtGsMarker(const OdGeMatrix3d& modelToWorld, OdGsMarker gsSelectionMark) const;

  virtual OdResult intersectWith(const OdGeMatrix3d& modelToWorld,
                                 const OdRxObject* pEnt,
                                 OdDb::Intersect intType,
                                 const OdGePlane* projPlane,
                                 OdGePoint3dArray& points,
                                 OdGsMarker thisGsMarker,
                                 OdGsMarker otherGsMarker) const;

  virtual OdUInt32 underlayLayerCount() const;
  virtual OdResult getUnderlayLayer(int idx, OdUnderlayLayer& layer) const;

  static OdSmartPtr<DwfDbItemImpl> create()
  {
    return OdSmartPtr<DwfDbItemImpl>(static_cast<DwfDbItemImpl*>(new DwfDbItemImpl()), kOdRxObjAttach);
  }
  virtual void addRef();
  virtual void release();
};

typedef OdSmartPtr<DwfDbItemImpl> DwfDbItemImplPtr;

#endif // _DWFDBITEMIMPL_H_
