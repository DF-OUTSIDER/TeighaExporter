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

#ifndef __OD_DB_UNDERLAY_REFERENCE__
#define __OD_DB_UNDERLAY_REFERENCE__

#include "TD_PackPush.h"
#include "DbEntity.h"
#include "Ge/GePoint2dArray.h"

/** \details
    This class represents underlays, where OdDbUnderlayReference controls the placement of the
	underlay in the drawing and OdDbUnderlayDefinition links to the underlay content.
	
    <group Other_Classes>
*/
class OdUnderlayLayer;

/** \details

    <group OdDb_Classes>
*/
class UNDERLAY_EXPORT OdDbUnderlayReference : public OdDbEntity
{
public:
  ODDB_DECLARE_MEMBERS(OdDbUnderlayReference);

  virtual OdResult dwgInFields(
    OdDbDwgFiler* pFiler);

  virtual void dwgOutFields(
    OdDbDwgFiler* pFiler) const;

  virtual OdResult dxfInFields(
    OdDbDxfFiler* pFiler);

  virtual void dxfOutFields(
    OdDbDxfFiler* pFiler) const;

  virtual OdUInt32 subSetAttributes(
    OdGiDrawableTraits* traits) const;

  virtual bool subWorldDraw(
    OdGiWorldDraw* pWd) const;

  virtual void subViewportDraw(
    OdGiViewportDraw* pVd) const;

  // method with wrong input parameters - must be removed in next release
  virtual OdResult subGetOsnapPoints(
    OdDb::OsnapMode osnapMode, 
    OdGsMarker        gsSelectionMark, 
    const OdGePoint3d& pickPoint,
    const OdGePoint3d& lastPoint, 
    const OdGeMatrix3d& viewXform, 
    const OdGeMatrix3d& ucs, 
    OdGePoint3dArray& snapPoints ) const { return eNotApplicable; }

  virtual OdResult subGetOsnapPoints(
    OdDb::OsnapMode osnapMode,
    OdGsMarker gsSelectionMark,
    const OdGePoint3d& pickPoint,
    const OdGePoint3d& lastPoint,
    const OdGeMatrix3d& xWorldToEye,
    OdGePoint3dArray& snapPoints ) const;

  virtual OdResult subTransformBy(
    const OdGeMatrix3d& xfm);

  OdGePoint3d position() const;
  void setPosition(const OdGePoint3d& position);

  OdGeScale3d scaleFactors() const;
  void setScaleFactors(const OdGeScale3d& scale);

  double rotation() const;
  void setRotation(double rotation);

  OdGeVector3d normal() const;
  void setNormal(const OdGeVector3d& normal);

  OdGeMatrix3d transform() const;
  OdResult setTransform(const OdGeMatrix3d& transform);

  OdDbObjectId definitionId() const;
  void setDefinitionId(OdDbObjectId id);

  const OdGePoint2dArray& clipBoundary() const;
  void setClipBoundary(const OdGePoint2d* clipBoundary, OdUInt32 nbPts);

  bool isClipped() const;
  void setIsClipped(bool value);

  OdUInt8 contrast() const;
  void setContrast(OdUInt8 value);

  OdUInt8 fade() const;
  void setFade(OdUInt8 value);

  bool isOn() const;
  void setIsOn(bool value);

  bool isMonochrome() const;
  void setIsMonochrome(bool value);

  bool isAdjustedForBackground() const;
  void setIsAdjustedForBackground(bool value);

  // must be overriden

  virtual bool isFrameVisible() const = 0;
  virtual bool isFramePlottable() const = 0;

  OdUInt32 underlayLayerCount() const;
  OdResult getUnderlayLayer(int index, OdUnderlayLayer& layer) const;
  OdResult setUnderlayLayer(int index, const OdUnderlayLayer& layer);

  static OdUInt8 contrastLowerLimit();
  static OdUInt8 contrastUpperLimit();
  static OdUInt8 contrastDefault();
  static OdUInt8 fadeLowerLimit();
  static OdUInt8 fadeUpperLimit();
  static OdUInt8 fadeDefault();

  bool isClipInverted() const;
  void setClipInverted(bool value);
  OdResult generateClipBoundaryFromPline(OdDbObjectId plId);

  OdGeMatrix3d getEcs( ) const;
};

typedef OdSmartPtr<OdDbUnderlayReference> OdDbUnderlayReferencePtr;


/** \details

    <group OdDb_Classes>
*/
class DBENT_EXPORT OdDbDwfReference : public OdDbUnderlayReference
{
public:
  ODDB_DECLARE_MEMBERS(OdDbDwfReference);

  OdDbDwfReference();

  virtual bool isFrameVisible() const;
  virtual bool isFramePlottable() const;

  virtual bool subIsContentSnappable() const;
  virtual OdResult subGetClassID(void* pClsid) const;
};

typedef OdSmartPtr<OdDbDwfReference> OdDbDwfReferencePtr;


/** \details

    <group OdDb_Classes>
*/
class DBENT_EXPORT OdDbDgnReference : public OdDbUnderlayReference
{
public:
  ODDB_DECLARE_MEMBERS(OdDbDgnReference);

  OdDbDgnReference();

  virtual bool isFrameVisible() const;
  virtual bool isFramePlottable() const;

  virtual bool subIsContentSnappable() const;
  virtual OdResult subGetClassID(void* pClsid) const;

  void setXrefDepth(OdInt32 depth);
};

typedef OdSmartPtr<OdDbDgnReference> OdDbDgnReferencePtr;

/** \details

<group OdDb_Classes>
*/
class DBENT_EXPORT OdDbPdfReference : public OdDbUnderlayReference
{
public:
  ODDB_DECLARE_MEMBERS(OdDbPdfReference);

  OdDbPdfReference();

  virtual bool isFrameVisible() const;
  virtual bool isFramePlottable() const;

  virtual bool subIsContentSnappable() const;
  virtual OdResult subGetClassID(void* pClsid) const;

  virtual OdUInt32 subSetAttributes(OdGiDrawableTraits *traits) const;

  static OdUInt8 contrastDefault();
  static OdUInt8 fadeDefault();
};

typedef OdSmartPtr<OdDbPdfReference> OdDbPdfReferencePtr;

/** \details

    <group OdDb_Classes>
*/
class DBENT_EXPORT OdDbNavisworksReference : public OdDbEntity
{
public:
  ODDB_DECLARE_MEMBERS( OdDbNavisworksReference );

  OdDbNavisworksReference();
  
  OdGePoint3d getInsertionPoint() const;
  void setInsertionPoint( const OdGePoint3d& position );

  double getScale() const;
  void setScale( double scale );

  double getRotation() const;
  void setRotation( double rotation );

  OdGeMatrix3d getTransformation() const;
  OdResult setTransformation( const OdGeMatrix3d& transform );

  OdDbObjectId getDefinitionId() const;
  void setDefinitionId( OdDbObjectId id );

  OdResult getHostDwgVisibility( bool &bVisibility )const;
  OdResult setHostDwgVisibility( bool bVisibility );

  OdResult getName( OdString &name )const;
  OdResult getSavedPath( OdString &path )const;

  double getInsertionUnitScaleFactor()const;
  void setInsertionUnit( OdDb::UnitsValue unitsVal );

  virtual OdResult subTransformBy(
    const OdGeMatrix3d& xfm );

  virtual OdUInt32 subSetAttributes(
    OdGiDrawableTraits* traits ) const;

  virtual bool subWorldDraw(
    OdGiWorldDraw* pWd ) const;

  virtual OdResult dwgInFields(
    OdDbDwgFiler* pFiler );

  virtual void dwgOutFields(
    OdDbDwgFiler* pFiler ) const;

  virtual OdResult dxfInFields(
    OdDbDxfFiler* pFiler );

  virtual void dxfOutFields(
    OdDbDxfFiler* pFiler ) const;
};

typedef OdSmartPtr<OdDbNavisworksReference> OdDbNavisworksReferencePtr;

#include "TD_PackPop.h"

#endif // __OD_DB_UNDERLAY_REFERENCE__
