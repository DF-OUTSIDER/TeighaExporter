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

#ifndef _OD_DGMODELERGEOMETRYCREATOR_INCLUDED_
#define _OD_DGMODELERGEOMETRYCREATOR_INCLUDED_

#include "DgModelerGeometry.h"
#include "OdArray.h"
#include "BrepBuilder/BrepBuilder.h"

class OdStreamBuf;

#include "TD_PackPush.h"

/** \details
    This utility class loads and saves SAT/SAB files, and performs other
    miscellaneous other ACIS-related operations.
    
    \remarks
    The functions in this class can do the following: 
    
    * Create OdDgModelerGeometry instances from a specified input stream.
    * Save SAT/SAB data from an OdDgModelerGeometry instance.
    * Create region entities from sets of curves.

    Corresponding C++ library: TG_Db
    <group OdDg_Classes>
*/
class TG_EXPORT OdDgModelerGeometryCreator : public OdRxObject
{
protected:
  OdDgModelerGeometryCreator() {}
public:  
  ODRX_DECLARE_MEMBERS(OdDgModelerGeometryCreator);

  /** \details
      Creates one or more instances of OdDgModelerGeometry from the specified stream.
      
      \param models [out]  Receives an array of OdDgModelerGeometry SmartPointers.
      \param pStreamBuf [in]  Pointer to the StreamBuf object from which the data are to be read.   

      Empty streamBuf generates single element array with empty OdDgModelerGeometry.

      Returns eOk if successful, or an appropriate error code if not.
  */
  virtual OdResult createModeler(OdArray<OdDgModelerGeometryPtr> &models, 
    OdStreamBuf* pStreamBuf) = 0;

  /** \details
    Creates a single stream from the specified array of objects.

    \param models [in]  Array of SmartPointers to the OdDbModelerGeometry objects to be written.
    \param pStreamBuf [in]  Pointer to the StreamBuf object to which the data are to be written.   
    \param typeVer [in] Type and version of PARASOLID data to write.
    \param standardSaveFlag [in] Controls the saving and restoring of use count data in the save file (used by Autodesk).
  */
  virtual OdResult createStream(const OdArray<OdDgModelerGeometryPtr> &models, 
    OdStreamBuf* pStreamBuf, 
    const OdDgModelerGeometry::OdDgModelerVersion& typeVer) = 0;

  virtual OdResult initBrepBuilder(OdBrepBuilder& brepBuilder, BrepType brepType) = 0;
};

/** \details
    This template class is a specialization of the OdSmartPtr class for OdDgModelerGeometryCreator object pointers.
*/
typedef OdSmartPtr<OdDgModelerGeometryCreator> OdDgModelerGeometryCreatorPtr;

#include "TD_PackPop.h"

#endif // _OD_DGMODELERGEOMETRYCREATOR_INCLUDED_
