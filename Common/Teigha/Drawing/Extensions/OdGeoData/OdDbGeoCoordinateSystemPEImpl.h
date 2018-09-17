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

#ifndef _OD_DB_GEOCOORDINATESYSTEM_PE_IMPL_
#define _OD_DB_GEOCOORDINATESYSTEM_PE_IMPL_

#include "TD_PackPush.h"

#include "OdDbGeoCoordinateSystemPE.h"

class OdDbGeoCoordinateSystemCategoryPEImpl : public OdDbGeoCoordinateSystemCategoryPE
{
public:
  ODRX_DECLARE_MEMBERS(OdDbGeoCoordinateSystemCategoryPEImpl);
  OdDbGeoCoordinateSystemCategoryPEImpl();
  virtual ~OdDbGeoCoordinateSystemCategoryPEImpl();

  virtual OdResult createAll(OdArray<OdDbGeoCoordinateSystemCategoryPtr>& allCategories) const;
};

typedef OdSmartPtr<OdDbGeoCoordinateSystemCategoryPEImpl> OdDbGeoCoordinateSystemCategoryPEImplPtr;

class OdDbGeoCoordinateSystemPEImpl : public OdDbGeoCoordinateSystemPE
{
public:
  ODRX_DECLARE_MEMBERS(OdDbGeoCoordinateSystemPEImpl);
  OdDbGeoCoordinateSystemPEImpl();
  virtual ~OdDbGeoCoordinateSystemPEImpl();

  virtual OdResult create(const OdString& coordSysIdOrFullDef, OdDbGeoCoordinateSystemPtr& pCoordSys) const;
  virtual OdResult createAll(const OdGePoint3d& geoPt, OdArray<OdDbGeoCoordinateSystemPtr>& allCoordSys) const;
  virtual OdResult createAll(OdArray<OdDbGeoCoordinateSystemPtr>& allCoordSys, const OdDbGeoCoordinateSystemCategory* pCategory = NULL) const;
};

typedef OdSmartPtr<OdDbGeoCoordinateSystemPEImpl> OdDbGeoCoordinateSystemPEImplPtr;

class OdDbGeoCoordinateSystemTransformerPEImpl : public OdDbGeoCoordinateSystemTransformerPE
{
public:
  ODRX_DECLARE_MEMBERS(OdDbGeoCoordinateSystemTransformerPEImpl);
  OdDbGeoCoordinateSystemTransformerPEImpl();
  virtual ~OdDbGeoCoordinateSystemTransformerPEImpl();

  virtual OdResult transformPoint(const OdString& sourceCoordSysId,
                                                      const OdString& targetCoordSysId,
                                                      const OdGePoint3d& pointIn,
                                                      OdGePoint3d& pointOut) const;

  virtual OdResult transformPoints(const OdString& sourceCoordSysId,
                                                       const OdString& targetCoordSysId,
                                                       const OdGePoint3dArray& pointsIn,
                                                       OdGePoint3dArray& pointsOut) const;
  virtual OdResult create(const OdString& sourceCoordSysId, 
                                              const OdString& targetCoordSysId,
                                              OdDbGeoCoordinateSystemTransformerPtr& pCoordSysTransformer) const;
};

typedef OdSmartPtr<OdDbGeoCoordinateSystemTransformerPEImpl> OdDbGeoCoordinateSystemTransformerPEImplPtr;

#include "TD_PackPop.h"

#endif //_OD_DB_GEOCOORDINATESYSTEM_PE_IMPL_
