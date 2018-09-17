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
#include "DbSystemServices.h"
#include "DbHostAppServices.h"

#include "OdGeoDataPEImpl.h"
#include "OdDbGeoCoordinateSystem.h"
#include "Ge/GeMatrix3d.h"
#include <tinyxml.h>

ODRX_CONS_DEFINE_MEMBERS(OdDbGeoDataPEImpl, OdDbGeoDataPE, RXIMPL_CONSTR);

OdDbGeoDataPEImpl::~OdDbGeoDataPEImpl()
{
}

OdResult OdDbGeoDataPEImpl::transformFromLonLatAlt(const OdDbGeoData* pGeoData, const double & dblLongitude, const double & dblLatitude, const double & dblAltitude,
                                                   double & dblDwgX, double & dblDwgY, double & dblDwgZ)
{
  if(pGeoData == NULL)
  {
    return eNullPtr;
  }

  OdGePoint3d lla(dblLongitude,dblLatitude,dblAltitude);
  OdGePoint3d res;
  OdResult st = OdDbGeoCoordinateSystemTransformer::transformPoint("LL", parseIdFromXml(pGeoData->coordinateSystem()), lla, res);
  if (st != eOk)
  {
    return st;
  }

  if(pGeoData->coordinateType() == OdDbGeoData::kCoordTypLocal)
  {
    OdGePoint3d m_designPoint = pGeoData->designPoint();
    OdGePoint3d m_referencePoint = pGeoData->referencePoint();
    if (m_referencePoint.x != 0. || m_referencePoint.y != 0.)
    {
      return eNotImplementedYet;
    }

    res.transformBy(OdGeMatrix3d::rotation(-pGeoData->northDirection(), pGeoData->upDirection()));
    dblDwgX = res.x + m_designPoint.x - m_referencePoint.x;
    dblDwgY = res.y + m_designPoint.y - m_referencePoint.y;
    dblDwgZ = res.z + m_designPoint.z - m_referencePoint.z;
  }
  else
  {
    dblDwgX = res.x;
    dblDwgY = res.y;
    dblDwgZ = res.z;
  }

  return eOk;
}

OdResult OdDbGeoDataPEImpl::transformToLonLatAlt(const OdDbGeoData* pGeoData, const double & dblDwgX, const double & dblDwgY, const double & dblDwgZ,
                                                 double & dblLongitude, double & dblLatitude, double & dblAltitude)
{
  if(pGeoData == NULL)
  {
    return eNullPtr;
  }

  OdGePoint3d ptOld;

  if(pGeoData->coordinateType() == OdDbGeoData::kCoordTypLocal)
  {
    OdGePoint3d m_designPoint = pGeoData->designPoint();
    OdGePoint3d m_referencePoint = pGeoData->referencePoint();
    if (m_referencePoint.x != 0. || m_referencePoint.y != 0.)
    {
      return eNotImplementedYet;
    }

    ptOld.x = dblDwgX - m_designPoint.x + m_referencePoint.x;
    ptOld.y = dblDwgY - m_designPoint.y + m_referencePoint.y;
    ptOld.z = dblDwgZ - m_designPoint.z + m_referencePoint.z;
    ptOld.transformBy(OdGeMatrix3d::rotation(pGeoData->northDirection(), pGeoData->upDirection()));
  }
  else 
  {
    ptOld.x = dblDwgX;
    ptOld.y = dblDwgY;
    ptOld.z = dblDwgZ;
  }

  OdGePoint3d res;
  OdResult st = OdDbGeoCoordinateSystemTransformer::transformPoint(parseIdFromXml(pGeoData->coordinateSystem()), "LL", ptOld, res);
  if (st != eOk)
  {
    return st;
  }

  dblLongitude = res.x;
  dblLatitude = res.y;
  dblAltitude = res.z;

  return eOk;
}

bool OdDbGeoDataPEImpl::validateCs(const OdString & coordinateSystem)
{
  //validation presented in OdDbGeoCoordinateSystem.h classes
  //this function is for user validation stuff
  return true;
}

OdResult OdDbGeoDataPEImpl::parseCsName(const OdDbGeoData* pGeoData, const OdString & sInputName, OdString & sResultName)
{
  if(pGeoData == NULL)
  {
    return eNullPtr;
  }

  OdDbGeoCoordinateSystemPtr pCs;
  OdResult status = OdDbGeoCoordinateSystem::create(sInputName, pCs);
  if (status != eOk)
  {
    return status;
  }

  return pCs->getXmlRepresentation(sResultName);
}

OdResult OdDbGeoDataPEImpl::geoCoordinateSystemWillChange(OdDbGeoData* pGeoData, const OdString & newName)
{
  if(pGeoData == NULL)
  {
    return eNullPtr;
  }
  return eOk;
}

OdResult OdDbGeoDataPEImpl::geoCoordinateSystemChanged(OdDbGeoData* pGeoData, const OdString & oldName)
{
  if(pGeoData == NULL)
  {
    return eNullPtr;
  }
  return eOk;
}

OdString OdDbGeoDataPEImpl::parseIdFromXml(const OdString & xml)
{
  TiXmlDocument doc;
  doc.Parse(xml, 0, TIXML_ENCODING_UTF8);
  TiXmlHandle docHandle(&doc);
  
  TiXmlElement * el = docHandle.FirstChild("Dictionary").FirstChild().ToElement();
  if(el == NULL)
  {
    return OdString();
  }
  return el->Attribute("id");
}
