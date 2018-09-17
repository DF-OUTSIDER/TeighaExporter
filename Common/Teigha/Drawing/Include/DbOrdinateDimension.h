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




#ifndef _OD_DB_ORDINATE_DIMENSION_
#define _OD_DB_ORDINATE_DIMENSION_

#include "TD_PackPush.h"

#include "DbDimension.h"
#include "DwgDeclareMembers.h"

/** \details
    This class represents Ordinate Dimension entities in an OdDbDatabase instance.

    \remarks
    An Ordinate Dimension entity dimensions the horizontal or vertical distance between 
    between the specified origin and the specified defining point.

    \sa
    TD_Db

    <group OdDb_Classes>
*/
class DBDIM_EXPORT OdDbOrdinateDimension : public OdDbDimension
{
public:
  DWGMAP_DECLARE_MEMBERS(OdDbOrdinateDimension);

  OdDbOrdinateDimension();
  
  /* OdDbOrdinateDimension(
       bool useXAxis,
       const OdGePoint3d& definingPoint,
       const OdGePoint3d& leaderEndPoint,
       const OdChar* dimText = 0,
       OdDbObjectId dimStyle = OdDbObjectId::kNull);
  */

  /** \details
    Returns true if and only if this Dimension entity measures the OCS X-axis distance
    between the origin and the defining point. 
    (DXF 70, bit 0x40 set).
  */
  bool isUsingXAxis() const;

  /** \details
    Returns true if and only if this Dimension entity measures the OCS Y-axis distance
    between the origin and the defining point. 
    (DXF 70, bit 0x40 cleared).
  */
  bool isUsingYAxis() const;

  /** \details
    Sets this Dimension entity to measure the OCS X-axis distance
    between the origin and the defining point. 
    (DXF 70, bit 0x40 set).
  */
  void useXAxis();

  /** \details
    Sets this Dimension entity to measure the OCS Y-axis distance
    between the origin and the defining point. 
    (DXF 70, bit 0x40 cleared).
  */
  void useYAxis();
  
  /** \details
    Returns the WCS origin and primary definition point of this Dimension entity (DXF 10).
    
    \remarks
    This Dimension entity measures the OCS X-axis or OCS Y-axis distance from the origin to the defining point.    
  */
  OdGePoint3d origin() const;

  /** \details
    Sets the WCS origin and primary definition point of this Dimension entity (DXF 10).

    \param origin [in]  Origin.
  
    \remarks
    This Dimension entity measures the OCS X-axis or OCS Y-axis distance from the origin to the defining point.    
  */
  void setOrigin(
    const OdGePoint3d& origin);
  
  /** \details
    Returns the WCS defining point for this Dimension entity (DXF 13).
    \remarks
    This Dimension entity measures the OCS X-axis or OCS Y-axis distance from the origin to the defining point.    
  */
  OdGePoint3d definingPoint() const;

  /** \details
    Sets the WCS defining point for this Dimension entity (DXF 13).
    
    \param definingPoint [in]  Defining point.
    
    \remarks
    This Dimension entity measures the OCS X-axis or OCS Y-axis distance from the origin to the defining point.    
  */
  void setDefiningPoint(
    const OdGePoint3d& definingPoint);
  
  /** \details
    Returns the WCS leader end point for this Dimension entity (DXF 14).
  */
  OdGePoint3d leaderEndPoint() const;

  /** \details
    Sets the WCS leader end point for this Dimension entity (DXF 14).
   
    \param leaderEndPoint [in]  Leader end point. 
  */
  void setLeaderEndPoint(
    const OdGePoint3d& leaderEndPoint);

  virtual OdResult dxfInFields_R12(
    OdDbDxfFiler* pFiler);

  virtual void dxfOutFields_R12(
    OdDbDxfFiler* pFiler) const;

  virtual OdResult dwgInFields(
    OdDbDwgFiler* pFiler);

  virtual void dwgOutFields(
    OdDbDwgFiler* pFiler) const;

  virtual OdResult dxfInFields(
    OdDbDxfFiler* pFiler);

  virtual void dxfOutFields(
    OdDbDxfFiler* pFiler) const;
  
  virtual OdResult subGetClassID(
    void* pClsid) const;
};

/** \details
  This template class is a specialization of the OdSmartPtr class for OdDbOrdinateDimension object pointers.
*/
typedef OdSmartPtr<OdDbOrdinateDimension> OdDbOrdinateDimensionPtr;

#include "TD_PackPop.h"

#endif
