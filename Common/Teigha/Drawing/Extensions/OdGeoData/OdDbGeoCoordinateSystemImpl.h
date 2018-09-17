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

#ifndef _OD_DB_GEOCOORDINATESYSTEM_IMPL_
#define _OD_DB_GEOCOORDINATESYSTEM_IMPL_

#include "TD_PackPush.h"

#include "DbDimension.h"
#include "Ge/GeExtents2d.h"

#include "OdDbGeoCoordinateSystem.h"

class OdDbGeoCoordinateSystemCategoryImpl : public OdDbGeoCoordinateSystemCategory
{
public:
  ODRX_DECLARE_MEMBERS(OdDbGeoCoordinateSystemCategoryImpl);
  OdDbGeoCoordinateSystemCategoryImpl();
  virtual ~OdDbGeoCoordinateSystemCategoryImpl();

  virtual OdResult getId(OdString& categoryId) const;
  virtual OdResult getNumOfCoordinateSystem(int& num) const;
  virtual OdResult getCoordinateSystemAt(int index, OdDbGeoCoordinateSystemPtr& pCoordSys) const;

  static  OdResult createAll(OdArray<OdDbGeoCoordinateSystemCategoryPtr>& allCategories);
private:
  OdString m_sCategoryId;
};

typedef OdSmartPtr<OdDbGeoCoordinateSystemCategoryImpl> OdDbGeoCoordinateSystemCategoryImplPtr;

class OdDbGeoCoordinateSystemImpl : public OdDbGeoCoordinateSystem
{
public:
  ODRX_DECLARE_MEMBERS(OdDbGeoCoordinateSystemImpl);
  OdDbGeoCoordinateSystemImpl();
  virtual ~OdDbGeoCoordinateSystemImpl();
  virtual OdResult getId(OdString& coordSysId) const;
  virtual OdResult getEpsgCode(int& epsgCode) const;
  virtual OdResult getType(OdDbGeoCoordinateSystem::Type& type) const;
  virtual OdResult getDescription(OdString& coordSysDesc) const;
  virtual OdResult getUnit(OdDb::UnitsValue& unit) const;
  virtual OdResult getUnit(OdDbGeoCoordinateSystem::Unit& unit) const;
  virtual OdResult getUnitScale(double& unitScale) const;
  virtual OdResult getProjectionCode(OdDbGeoCoordinateSystem::ProjectionCode& prjCode) const;
  virtual OdResult getProjectionParameters(OdArray<OdDbGeoProjectionParameter>& prjParams,
    bool includeSpecialParams) const;
  virtual OdResult getDatum(OdDbGeoDatum& datum) const;
  virtual OdResult getEllipsoid(OdDbGeoEllipsoid& ellipsoid) const;
  virtual OdResult getOffset(OdGeVector2d& offsetVec) const;
  virtual OdResult getCartesianExtents(OdGeExtents2d& exts) const;
  virtual OdResult getGeodeticExtents(OdGeExtents2d& exts) const;
  virtual OdResult getXmlRepresentation(OdString& strXml) const;
  virtual OdResult getWktRepresentation(OdString& strWkt) const;

  static OdResult create(const OdString& coordSysIdOrFullDef, 
    OdDbGeoCoordinateSystemPtr& pCoordSys);
  static OdResult createAll(const OdGePoint3d& geoPt, OdArray<OdDbGeoCoordinateSystemPtr>& allCoordSys);
  static OdResult createAll(OdArray<OdDbGeoCoordinateSystemPtr>& allCoordSys,
    const OdDbGeoCoordinateSystemCategory* pCategory = NULL);

private:
  OdGeExtents2d m_catresianExtents;
  OdDbGeoDatum m_datum;
  OdString m_sDescription;
  OdDbGeoEllipsoid m_ellipsoid;
  int m_dEpgsCode;
  OdGeExtents2d m_geodeticExtents;
  OdDbGeoCoordinateSystem::Unit m_eGeoUnit;
  OdString m_sId;
  OdGeVector2d m_vOffset;
  OdDbGeoCoordinateSystem::ProjectionCode m_eProjectionCode;
  OdDbGeoCoordinateSystem::Type m_eType;
  OdDb::UnitsValue m_eUnit;
  double m_dUnitScale;
  OdString m_sWktRepresentation;
  OdString m_sXmlRepresentation;
};

typedef OdSmartPtr<OdDbGeoCoordinateSystemImpl> OdDbGeoCoordinateSystemImplPtr;

class OdDbGeoCoordinateSystemTransformerImpl : public OdDbGeoCoordinateSystemTransformer
{
public:
  ODRX_DECLARE_MEMBERS(OdDbGeoCoordinateSystemTransformerImpl);
  OdDbGeoCoordinateSystemTransformerImpl();
  virtual ~OdDbGeoCoordinateSystemTransformerImpl();
  virtual OdResult getSourceCoordinateSystemId(OdString& sourceCoordSysId) const;
  virtual OdResult getTargetCoordinateSystemId(OdString& targetCoordSysId) const;
  virtual OdResult transformPoint(const OdGePoint3d& pointIn, OdGePoint3d& pointOut) const;
  virtual OdResult transformPoints(const OdGePoint3dArray& pointsIn, OdGePoint3dArray& pointsOut) const;

  static OdResult transformPoint(const OdString& sourceCoordSysId,
    const OdString& targetCoordSysId,
    const OdGePoint3d& pointIn,
    OdGePoint3d& pointOut);

  static OdResult transformPoints(const OdString& sourceCoordSysId,
    const OdString& targetCoordSysId,
    const OdGePoint3dArray& pointsIn,
    OdGePoint3dArray& pointsOut);

  static OdResult create(const OdString& sourceCoordSysId, 
    const OdString& targetCoordSysId,
    OdDbGeoCoordinateSystemTransformerPtr& pCoordSysTransformer);
private:
  OdString m_sSourceCoordSysId;
  OdString m_sTargetCoordSysId;
};

typedef OdSmartPtr<OdDbGeoCoordinateSystemTransformerImpl> OdDbGeoCoordinateSystemTransformerImplPtr;

#include "TD_PackPop.h"

#endif //_OD_DB_GEOCOORDINATESYSTEM_IMPL_
