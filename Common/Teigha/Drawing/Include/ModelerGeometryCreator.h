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


#ifndef _OD_MODELERGEOMETRYCREATOR_INCLUDED_
#define _OD_MODELERGEOMETRYCREATOR_INCLUDED_

#include "RxObject.h"
#include "ModelerGeometry.h"
#include "OdArray.h"
#include "Ge/GeNurbSurface.h"
#include "BrepBuilder/BrepBuilder.h"

class OdStreamBuf;

#include "TD_PackPush.h"

/** \details
    This utility class loads and saves SAT/SAB files, and performs other
    miscellaneous other ACIS-related operations.
    
    \remarks
    The functions in this class can do the following: 
    * Create OdModelerGeometry instances from a specified input stream.
    * Save SAT/SAB data from an OdModelerGeometry instance.
    * Create region entities from sets of curves.

    Corresponding C++ library: ModelerGeometry
    <group Other_Classes>
*/
class DBENT_EXPORT OdModelerGeometryCreator : public OdRxObject
{
protected:
  OdModelerGeometryCreator() {}
public:  
  ODRX_DECLARE_MEMBERS(OdModelerGeometryCreator);

  /** \details
      Creates one or more instances of OdModelerGeometry from the specified SAT/SAB file.
      
      \param models [out]  Receives an array of OdModelerGeometry SmartPointers.
      \param pStreamBuf [in]  Pointer to the StreamBuf object from which the data are to be read.   
      \param standardSaveFlag [in]  Controls the saving and restoring of use count data in the save file.

      \remarks
      Since OdModelerGeometry does not support multi-body SAT files, such files 
      are divided into a set of single body OdModelerGeometry objects

      Empty streamBuf generates single element array with empty OdModelerGeometry.

      Returns eOk if successful, or an appropriate error code if not.
  */
  virtual OdResult createModeler(OdArray<OdModelerGeometryPtr> &models, 
    OdStreamBuf* pStreamBuf, 
    OdDbAuditInfo* pAuditInfo = NULL,
    bool standardSaveFlag = true, 
    bool bEnableACISAudit = false) = 0;

  /** \details
    Creates a single SAT/SAB file from the specified array of objects.

    \param models [in]  Array of SmartPointers to the OdDbModelerGeometry objects to be written.
    \param pStreamBuf [in]  Pointer to the StreamBuf object to which the data are to be written.   
    \param typeVer [in] Type and version of ACIS data to write.
    \param standardSaveFlag [in] Controls the saving and restoring of use count data in the save file.
  */
  virtual OdResult createSat(const OdArray<OdModelerGeometryPtr> &models, 
    OdStreamBuf* pStreamBuf, 
    AfTypeVer typeVer, 
    bool standardSaveFlag = true) = 0;

  /** \details
    Creates a single SAT/SAB file from the specified array of objects.

    \param entities [in]  Array of SmartPointers to the OdDbEntity objects to be written.
    \param pStreamBuf [in]  Pointer to the StreamBuf object to which the data are to be written.   
    \param typeVer [in] Type and version of ACIS data to write.
    \param standardSaveFlag [in] Controls the saving and restoring of use count data in the save file.
	
	\returns
    Returns eOk if successful, or an appropriate error code if not.
	
	\remarks
    Color attributes are also added to the SAT/SAB file from the specified OdDbEntity objects.
    Only the following OdDbEntity types are valid: OdDb3dSolid, OdDbBody, and OdDbRegion/
  */
  virtual OdResult createSat(const OdDbEntityPtrArray &entities, 
    OdStreamBuf* pStreamBuf, 
    AfTypeVer typeVer, 
    bool standardSaveFlag = true) = 0;

  /** \details
    Creates OdDbRegion entities from the closed loops defined by the specified curve segments.

    \param curveSegments [in]  Array of curve segments.
    \param pRegions [out]  Receives an array of pointers to the regions.

    \remarks
    Each curve segment must be one of the following

    @untitled table
    OdDb3dPolyline
    OdDbArc
    OdDbCircle
    OdDbEllipse
    OdDbLine
    OdDbPolyline
    OdDbSpline

    The newly created regions are non- database -resident. It is up to the caller to either add them
    to an OdDbDatabase or to delete them.

    \returns 
    Returns eOk if successful, or an appropriate error code if not. 
    
    \remarks
    * All curves must be in the same plane.
    * The endpoint of each curve must be equal to the start point of the next curve
    * The endpoint of the last curve must be equal to the start point of the first curve.
    
  */
  virtual OdResult createRegionFromCurves(const OdRxObjectPtrArray& curveSegments, 
    OdArray<OdModelerGeometryPtr> &pRegions) = 0;

  virtual OdResult initBrepBuilder(OdBrepBuilder& brepBuilder, BrepType brepType) = 0;
};

/** \details
    This template class is a specialization of the OdSmartPtr class for OdModelerGeometryCreator object pointers.
*/
typedef OdSmartPtr<OdModelerGeometryCreator> OdModelerGeometryCreatorPtr;

#include "TD_PackPop.h"

#endif // _OD_MODELERGEOMETRYCREATOR_INCLUDED_
