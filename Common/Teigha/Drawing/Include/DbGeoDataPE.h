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
#ifndef _OD_DB_GEODATA_PE_
#define _OD_DB_GEODATA_PE_

#include "RxObject.h"
#include "SmartPtr.h"
#include "DbExport.h"

#include "DbGeoData.h"

#include "TD_PackPush.h"
/** \details
An abstract class that represents a protocol extension for an OdDbGeoData object.

<group OdDb_Classes> 
*/
class TOOLKIT_EXPORT OdDbGeoDataPE : public OdRxObject
{
public:
  ODRX_DECLARE_MEMBERS(OdDbGeoDataPE);
  
  /** \details
    Virtual destructor.
  */
  virtual ~OdDbGeoDataPE() {};
  /** \details
    Transforms the geographic point into an equivalent design point.
    
    \param pGeoData [in] Pointer to an OdDbGeoData object.
    \param dblLongitude [in] Longitude ordinate of the point to be transformed. 
    \param dblLatitude [in] Latitude ordinate of the point to be transformed. 
    \param dblAltitude [in] Altitude ordinate of the point to be transformed. 
    \param dblDwgX [out] X ordinate of the transformed point.
    \param dblDwgY [out] Y ordinate of the transformed point.
    \param dblDwgZ [out] Z ordinate of the transformed point.
    
    \returns Returns eOK if successful, or an appropriate error code if not.
  */
  virtual OdResult transformFromLonLatAlt(const OdDbGeoData* pGeoData, const double& dblLongitude, const double& dblLatitude, const double& dblAltitude,
                                                             double& dblDwgX, double& dblDwgY, double& dblDwgZ) = 0;
  /** \details
    Transforms the design point into an equivalent geographic point.
    
    \param pGeoData [in] Pointer to an OdDbGeoData object.
    \param dblDwgX [in] X ordinate of the design point.
    \param dblDwgY [in] Y ordinate of the design point.
    \param dblDwgZ [in] Z ordinate of the design point.
    \param dblLongitude [out] Longitude ordinate of the transformed point. 
    \param dblLatitude [out] Latitude ordinate of the transformed point. 
    \param dblAltitude [out] Altitude (elevation) ordinate of the transformed point. 
    
    \returns Returns eOK if successful, or an appropriate error code if not.
  */
  virtual OdResult transformToLonLatAlt(const OdDbGeoData* pGeoData, const double& dblDwgX, const double& dblDwgY, const double& dblDwgZ,
                                                             double& dblLongitude, double& dblLatitude, double& dblAltitude) = 0;
  /** \details
    This method is used for user validation.
    
    \param coordinateSystem [in] Coordinate system name.
     
    \returns Returns true if validation passed successfully, or false if not.
  */
  virtual bool validateCs(const OdString & coordinateSystem) = 0;
  
  /** \details
    Returns xml representation of the coordinate system.
    
    \param pGeoData [in] Pointer to an OdDbGeoData object.
    \param inputName [in] Coordinate system name.
    \param resultXml [out] Xml representation of the coordinate system.
     
    \returns Returns eOK if successful, or an appropriate error code if not.
  */
  virtual OdResult parseCsName(const OdDbGeoData* pGeoData, const OdString & inputName, OdString & resultXml) = 0;
  
  /** \details
    This method is called when the coordinate system will be changed.
    
    \param pGeoData [in/out] Pointer to an OdDbGeoData object.
    \param oldName [in] Coordinate system name.
     
    \returns Returns eOK if successful, or an appropriate error code if not.
  */
  virtual OdResult geoCoordinateSystemWillChange(OdDbGeoData* pGeoData, const OdString& newName) = 0;
  
  /** \details
    This method is called when the coordinate system is changed.
    
    \param pGeoData [in/out] Pointer to an OdDbGeoData object.
    \param oldName [in] Coordinate system name.
     
    \returns Returns eOK if successful, or an appropriate error code if not.
  */
  virtual OdResult geoCoordinateSystemChanged(OdDbGeoData* pGeoData, const OdString& oldName) = 0;
  
  /** \details
    Parses and returns the coordinate system ID.
    
    \param xml [in] String with xml representation of coordinate system.
  */
  virtual OdString parseIdFromXml(const OdString& xml) = 0;
};

typedef OdSmartPtr<OdDbGeoDataPE> OdDbGeoDataPEPtr;

#include "TD_PackPop.h"

#endif //_OD_DB_GEODATA_PE_
