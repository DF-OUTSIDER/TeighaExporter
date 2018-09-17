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

#ifndef _OD_DB_GEOCOORDINATESYSTEM_PE_
#define _OD_DB_GEOCOORDINATESYSTEM_PE_

#include "OdDbGeoCoordinateSystem.h"

#include "TD_PackPush.h"
/** \details
  This abstract class represents a protocol extension for a category which holds string 
  identifiers referring to a group of coordinate reference systems.

  <group OdDb_Classes>
*/
class TOOLKIT_EXPORT OdDbGeoCoordinateSystemCategoryPE : public OdRxObject
{
public:
  ODRX_DECLARE_MEMBERS(OdDbGeoCoordinateSystemCategoryPE);
  
  /** \details
    Virtual destructor.
  */
  virtual ~OdDbGeoCoordinateSystemCategoryPE() {};
  
  /** \details
    Creates and returns all coordinate system categories.
    
    \param allCategories [in/out] Array of pointers to returned categories.
        
    \returns Returns eOK if successful, or an appropriate error code if not.
  */
  virtual OdResult  createAll(OdArray<OdDbGeoCoordinateSystemCategoryPtr>& allCategories) const = 0;
};
/** \details
  This template class is a specialization of the OdSmartPtr class for OdDbGeoCoordinateSystemCategoryPE object pointers.
*/
typedef OdSmartPtr<OdDbGeoCoordinateSystemCategoryPE> OdDbGeoCoordinateSystemCategoryPEPtr;

/** \details
  This abstract class represents a protocol extension for a coordinate reference system (CRS) definition.

  <group OdDb_Classes>
*/
class TOOLKIT_EXPORT OdDbGeoCoordinateSystemPE : public OdRxObject
{
public:
  ODRX_DECLARE_MEMBERS(OdDbGeoCoordinateSystemPE);
  
  /** \details
    Virtual destructor.
  */
  virtual ~OdDbGeoCoordinateSystemPE() {};
  
  /** \details
    Creates and returns a new OdDbGeoCoordinateSystem object from the current virtual catalog.
    
    \param coordSysIdOrFullDef [in] ID, WKT or XML representation of the CRS definition to load.
    \param pCoordSys [out] New CRS.
    
    \returns Returns eOK if successful, or an appropriate error code if not.
  */
  virtual OdResult create(const OdString& coordSysIdOrFullDef, OdDbGeoCoordinateSystemPtr& pCoordSys) const = 0;
  /** \details
    Creates and returns a new OdDbGeoCoordinateSystem object for every existing, 
    persistent CRS definition from the current virtual catalog.
    
    \param geoPt [in] Geodetic point.
    \param allCoordSys [out] Array of pointers to the new CRS objects.
    
    \returns Returns eOK if successful, or an appropriate error code if not.
  */
  virtual OdResult createAll(const OdGePoint3d& geoPt, OdArray<OdDbGeoCoordinateSystemPtr>& allCoordSys) const = 0;
  
  /** \details
    Creates and returns a new OdDbGeoCoordinateSystem object for every existing, 
    persistent CRS definition from the current virtual catalog.
    
    \param allCoordSys [out] Array of pointers to the new CRS objects.
    \param pCategory [in] Category.
    
    \returns Returns eOK if successful, or an appropriate error code if not.
    
    \remarks
    If pCategory is NULL, all existing and persistent CRS definitions are returned.
  */
  virtual OdResult createAll(OdArray<OdDbGeoCoordinateSystemPtr>& allCoordSys, const OdDbGeoCoordinateSystemCategory* pCategory = NULL) const = 0;
};
/** \details
  This template class is a specialization of the OdSmartPtr class for OdDbGeoCoordinateSystemPE object pointers.
*/
typedef OdSmartPtr<OdDbGeoCoordinateSystemPE> OdDbGeoCoordinateSystemPEPtr;

/** \details
  This abstract class represents a protocol extension for a transformer object 
  which is used to transform points from the source CRS to the target CRS.

  <group OdDb_Classes>
*/
class TOOLKIT_EXPORT OdDbGeoCoordinateSystemTransformerPE : public OdRxObject
{
public:
  ODRX_DECLARE_MEMBERS(OdDbGeoCoordinateSystemTransformerPE);
  
  /** \details
    Virtual destructor.
  */
  virtual ~OdDbGeoCoordinateSystemTransformerPE() {};
  /** \details
    Transforms the point from the source CRS to the target CRS.
    
    \param sourceCoordSysId [in] Source CRS's ID.
    \param targetCoordSysId [in] Target CRS's ID.
    \param pointIn [in] Point in the source CRS.
    \param pointOut [out] Point in the target CRS.
    
    \returns Returns eOK if successful, or an appropriate error code if not.
  */
  virtual OdResult transformPoint(const OdString& sourceCoordSysId,
    const OdString& targetCoordSysId,
    const OdGePoint3d& pointIn,
    OdGePoint3d& pointOut) const = 0;
  /** \details
    Transforms the array of points from the source CRS to the target CRS.
    
    \param sourceCoordSysId [in] Source CRS's ID.
    \param targetCoordSysId [in] Target CRS's ID.
    \param pointsIn [in] Point array in the source CRS.
    \param pointsOut [out] Point array in the target CRS.
    
    \returns Returns eOK if successful, or an appropriate error code if not.
  */
  virtual OdResult transformPoints(const OdString& sourceCoordSysId,
    const OdString& targetCoordSysId,
    const OdGePoint3dArray& pointsIn,
    OdGePoint3dArray& pointsOut) const = 0;
  
  /** \details
    Creates a new OdDbGeoCoordinateSystemTransformer object from the source and target CRSs.
    
    \param sourceCoordSysId [in] Source CRS's ID.
    \param targetCoordSysId [in] Target CRS's ID.
    \param pCoordSysTransformer [out] New OdDbGeoCoordinateSystemTransformer object.
    
    \returns Returns eOK if successful, or an appropriate error code if not.
  */
  virtual OdResult create(const OdString& sourceCoordSysId, 
    const OdString& targetCoordSysId,
    OdDbGeoCoordinateSystemTransformerPtr& pCoordSysTransformer) const = 0;
};
/** \details
  This template class is a specialization of the OdSmartPtr class for OdDbGeoCoordinateSystemTransformerPE object pointers.
*/
typedef OdSmartPtr<OdDbGeoCoordinateSystemTransformerPE> OdDbGeoCoordinateSystemTransformerPEPtr;

#include "TD_PackPop.h"

#endif //_OD_DB_GEOCOORDINATESYSTEM_PE_
