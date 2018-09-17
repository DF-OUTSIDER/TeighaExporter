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

#ifndef __OD_DB_UNDERLAY_ITEM__
#define __OD_DB_UNDERLAY_ITEM__

#include "TD_PackPush.h"

#include "OdPlatformSettings.h"
#include "DbRootExport.h"
#include "Gi/GiRasterImage.h"
#include "Ge/GePoint3dArray.h"
#include "IntArray.h"
#include "OdaDefs.h"

class OdGePoint2d;
class OdGeMatrix3d;
class OdGePlane;
class OdGiWorldDraw;
class OdGiViewportDraw;
//const int kItemVersionNumber = 1;

/** <group Other_Classes>
*/
class DBROOT_EXPORT OdUnderlayLayer
{
public:
    OdUnderlayLayer();
    ~OdUnderlayLayer();

    enum State
    {
      kOff = 0,
      kOn = 1
    };

    OdString name() const;
    State state() const;
    void setName(const OdString& name);
    void setState(State state);

    OdUnderlayLayer& operator =(const OdUnderlayLayer& source);
    OdUnderlayLayer(const OdUnderlayLayer & source);

private:
    class OdUnderlayLayerImpl* m_pImpl;
};

struct OdDbUnderlayDrawContext
{
   OdUInt8 contrast; 
   OdUInt8 fade;
   bool monochrome;
   bool adjustColorForBackground;
   OdArray<OdUnderlayLayer> layers;
};

/** \details

    <group Other_Classes>
*/
class DBROOT_EXPORT OdDbUnderlayItem : public OdRxObject
{
public:
  ODRX_DECLARE_MEMBERS(OdDbUnderlayItem);

  virtual OdString getName() const = 0;

  virtual OdGiRasterImagePtr getThumbnail() const = 0;

  virtual void getExtents(OdGePoint2d& min, OdGePoint2d& max) const = 0;

  virtual OdDb::UnitsValue getUnits() const = 0;

  virtual bool usingPartialContent() const;

  virtual bool worldDraw(OdGiWorldDraw* pWd, const OdDbUnderlayDrawContext& context) const = 0;

  virtual void viewportDraw(OdGiViewportDraw* pVd, const OdDbUnderlayDrawContext& context) const = 0;

  virtual OdGeMatrix3d modelTransform() const;

  virtual OdResult getMediaBox(OdGePoint2d &min, OdGePoint2d &max) const; //to use with PdfUnderlays because getExtents can return extents in mm or in inches randomly, this method always return media box in 1/72 inch  

  virtual OdResult getOsnapPoints(const OdGeMatrix3d& modelToWorld,
                                  OdDb::OsnapMode     osnapMode,
                                  OdGsMarker          gsSelectionMark,
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

  int version() const;
};

typedef OdSmartPtr<OdDbUnderlayItem> OdDbUnderlayItemPtr;

#include "TD_PackPop.h"

#endif // __OD_DB_UNDERLAY_ITEM__
