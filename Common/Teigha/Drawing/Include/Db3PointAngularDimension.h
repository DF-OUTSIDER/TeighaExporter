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




#ifndef _OD_DB_3POINTANGULAR_DIMENSION_
#define _OD_DB_3POINTANGULAR_DIMENSION_

#include "TD_PackPush.h"

#include "DbDimension.h"
#include "DwgDeclareMembers.h"

/** \details
    This class represents 3-Point Angular Dimension entities in an OdDbDatabase instance.

    \remarks
    A 3-Point Angular Dimension entity dimensions the angle defined by three points.
    
    \sa
    OdDb2LineAngularDimension
    
    \sa
    TD_Db

    <group OdDb_Classes> 
*/
class DBDIM_EXPORT OdDb3PointAngularDimension : public OdDbDimension
{
public:

  DWGMAP_DECLARE_MEMBERS(OdDb3PointAngularDimension);

  OdDb3PointAngularDimension();
  
  /** \details
    Returns the point defining the location of dimension arc for this Dimension entity (DXF 16 as WCS).
  */
  OdGePoint3d arcPoint() const;

  /** \details
    Sets the point defining the location of dimension arc for this Dimension entity (DXF 16 as WCS).
    \param arcPoint [in]  Arc point.
  */
  void setArcPoint(
    const OdGePoint3d& arcPoint);
  
  /** \details
    Returns the WCS start point of the first extension line of this Dimension entity (DXF 13).
      
    \remarks
    This point and the centerPoint define one side of 
    the angle being dimensioned.
  */
  OdGePoint3d xLine1Point() const;

  /** \details
    Sets the WCS start point of the first extension line of this Dimension entity (DXF 13).
  
    \param xLine1Point [in]  Start point.
        
    \remarks
    This point and the centerPoint define one side of 
    the angle being dimensioned.
  */
  void setXLine1Point(
    const OdGePoint3d& xLine1Point);

  /** \details
    Returns the WCS start point of the second extension line of this Dimension entity (DXF 14).
      
    \remarks
    This point and the centerPoint define one side of 
    the angle being dimensioned.
  */
  OdGePoint3d xLine2Point() const;

  /** \details
    Sets the WCS start point of the second extension line of this Dimension entity (DXF 14).
  
    \param xLine2Point [in]  Start point.
        
    \remarks
    This point and the centerPoint define one side of 
    the angle being dimensioned.
  */
  void setXLine2Point(
    const OdGePoint3d& xLine2Point);

  
  /** \details
    Returns the WCS vertex of the angle being dimensioned by this Dimension entity (DXF 15).
    \remarks
    The vertex of the angle being dimensioned is the center of the dimension arc.
  */
  OdGePoint3d centerPoint() const;

  /** \details
    Returns the extension arc status of this Dimension entity.

    \returns Returns true if extension arc is on, otherwise returns false.
  */
  bool extArcOn() const;

  /** \details
    Sets the extension arc status for this Dimension entity.

    \param value [in] Input extension arc status.
  */
  void setExtArcOn(bool value);

  /** \details
    Sets the WCS vertex of the angle being dimensioned by this Dimension entity (DXF 15).
    \param centerPoint [in]  Center point.
    \remarks
    The vertex of the angle being dimensioned is the center of the dimension arc.
  */
  void setCenterPoint(
    const OdGePoint3d& centerPoint);

  virtual OdResult dwgInFields(
    OdDbDwgFiler* pFiler);

  virtual void dwgOutFields(
    OdDbDwgFiler* pFiler) const;

  virtual OdResult dxfInFields(
    OdDbDxfFiler* pFiler);

  virtual void dxfOutFields(
    OdDbDxfFiler* pFiler) const;

  virtual OdResult dxfInFields_R12(
    OdDbDxfFiler* pFiler);

  virtual void dxfOutFields_R12(
    OdDbDxfFiler* pFiler) const;

  virtual OdResult subGetClassID(
    void* pClsid) const;
};

/** \details
  This template class is a specialization of the OdSmartPtr class for OdDb3PointAngularDimension object pointers.
*/
typedef OdSmartPtr<OdDb3PointAngularDimension> OdDb3PointAngularDimensionPtr;

#include "TD_PackPop.h"

#endif
