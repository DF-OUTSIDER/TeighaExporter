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

#include "OdDbGeoCoordinateSystemPEImpl.h"
#include "OdDbGeoCoordinateSystemImpl.h"

ODRX_CONS_DEFINE_MEMBERS(OdDbGeoCoordinateSystemCategoryPEImpl, OdDbGeoCoordinateSystemCategoryPE, RXIMPL_CONSTR);
ODRX_CONS_DEFINE_MEMBERS(OdDbGeoCoordinateSystemPEImpl, OdDbGeoCoordinateSystemPE, RXIMPL_CONSTR);
ODRX_CONS_DEFINE_MEMBERS(OdDbGeoCoordinateSystemTransformerPEImpl, OdDbGeoCoordinateSystemTransformerPE, RXIMPL_CONSTR);

//OdDbGeoCoordinateSystemCategoryPEImpl
OdDbGeoCoordinateSystemCategoryPEImpl::OdDbGeoCoordinateSystemCategoryPEImpl()
{
}

OdDbGeoCoordinateSystemCategoryPEImpl::~OdDbGeoCoordinateSystemCategoryPEImpl()
{
}

OdResult OdDbGeoCoordinateSystemCategoryPEImpl::createAll(OdArray<OdDbGeoCoordinateSystemCategoryPtr>& allCategories) const
{
  return OdDbGeoCoordinateSystemCategoryImpl::createAll(allCategories);
}


//OdDbGeoCoordinateSystemPEImpl
OdDbGeoCoordinateSystemPEImpl::OdDbGeoCoordinateSystemPEImpl()
{
}

OdDbGeoCoordinateSystemPEImpl::~OdDbGeoCoordinateSystemPEImpl()
{
}

OdResult OdDbGeoCoordinateSystemPEImpl::create(const OdString& coordSysIdOrFullDef, OdDbGeoCoordinateSystemPtr& pCoordSys) const
{
  return OdDbGeoCoordinateSystemImpl::create(coordSysIdOrFullDef, pCoordSys);
}

OdResult OdDbGeoCoordinateSystemPEImpl::createAll(const OdGePoint3d& geoPt, OdArray<OdDbGeoCoordinateSystemPtr>& allCoordSys) const
{
  return OdDbGeoCoordinateSystemImpl::createAll(geoPt, allCoordSys);
}

OdResult OdDbGeoCoordinateSystemPEImpl::createAll(OdArray<OdDbGeoCoordinateSystemPtr>& allCoordSys, const OdDbGeoCoordinateSystemCategory* pCategory) const
{
  return OdDbGeoCoordinateSystemImpl::createAll(allCoordSys, pCategory);
}


//OdDbGeoCoordinateSystemTransformerPEImpl
OdDbGeoCoordinateSystemTransformerPEImpl::OdDbGeoCoordinateSystemTransformerPEImpl()
{
}

OdDbGeoCoordinateSystemTransformerPEImpl::~OdDbGeoCoordinateSystemTransformerPEImpl()
{
}

OdResult OdDbGeoCoordinateSystemTransformerPEImpl::transformPoint(const OdString& sourceCoordSysId,
                                                                const OdString& targetCoordSysId,
                                                                const OdGePoint3d& pointIn,
                                                                OdGePoint3d& pointOut) const
{
  return OdDbGeoCoordinateSystemTransformerImpl::transformPoint(sourceCoordSysId,targetCoordSysId,pointIn,pointOut);
}

OdResult OdDbGeoCoordinateSystemTransformerPEImpl::transformPoints(const OdString& sourceCoordSysId,
                                                                 const OdString& targetCoordSysId,
                                                                 const OdGePoint3dArray& pointsIn,
                                                                 OdGePoint3dArray& pointsOut) const
{
  return OdDbGeoCoordinateSystemTransformerImpl::transformPoints(sourceCoordSysId, targetCoordSysId, pointsIn, pointsOut);
}

OdResult OdDbGeoCoordinateSystemTransformerPEImpl::create(const OdString& sourceCoordSysId, 
                                                        const OdString& targetCoordSysId,
                                                        OdDbGeoCoordinateSystemTransformerPtr& pCoordSysTransformer) const
{
  return OdDbGeoCoordinateSystemTransformerImpl::create(sourceCoordSysId, targetCoordSysId, pCoordSysTransformer);
}
