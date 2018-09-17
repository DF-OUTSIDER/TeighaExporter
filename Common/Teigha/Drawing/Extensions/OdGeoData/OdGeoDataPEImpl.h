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

#ifndef _OD_DB_GEODATA_PEIMPL_
#define _OD_DB_GEODATA_PEIMPL_

#include "TD_PackPush.h"

#include "DbGeoDataPE.h"

class OdDbGeoDataPEImpl : public OdDbGeoDataPE
{
public:
  ODRX_DECLARE_MEMBERS(OdDbGeoDataPEImpl);
  virtual ~OdDbGeoDataPEImpl();
  virtual OdResult transformFromLonLatAlt(const OdDbGeoData* pGeoData, const double & dblLongitude, const double & dblLatitude, const double & dblAltitude,
    double & dblDwgX, double & dblDwgY, double & dblDwgZ);
  virtual OdResult transformToLonLatAlt(const OdDbGeoData* pGeoData, const double & dblDwgX, const double & dblDwgY, const double & dblDwgZ,
    double & dblLongitude, double & dblLatitude, double & dblAltitude);
  virtual bool validateCs(const OdString & coordinateSystem);
  virtual OdResult parseCsName(const OdDbGeoData* pGeoData, const OdString & inputName, OdString & resultName);
  virtual OdResult geoCoordinateSystemWillChange(OdDbGeoData* pGeoData, const OdString & newName);
  virtual OdResult geoCoordinateSystemChanged(OdDbGeoData* pGeoData, const OdString & oldName);
  virtual OdString parseIdFromXml(const OdString & xml);
};

typedef OdSmartPtr<OdDbGeoDataPEImpl> OdDbGeoDataPEImplPtr;

#include "TD_PackPop.h"
#endif //_OD_DB_GEODATA_PEIMPL_
