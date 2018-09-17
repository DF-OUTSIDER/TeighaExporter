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




#ifndef _OD_DB_2LINEANGULAR_DIMENSION_
#define _OD_DB_2LINEANGULAR_DIMENSION_

#include "TD_PackPush.h"

#include "DbDimension.h"
#include "DwgDeclareMembers.h"

/** \details
    This class represents 2-Line Angular Dimension entities in an OdDbDatabase instance.
    
    \remarks
    A 2-Line Angular Dimension entity dimensions the angle defined by two lines.
    
    \sa
    OdDb3PointAngularDimension
    
    \sa
    TD_Db

    <group OdDb_Classes> 
*/
class DBDIM_EXPORT OdDb2LineAngularDimension : public OdDbDimension
{
public:

  DWGMAP_DECLARE_MEMBERS(OdDb2LineAngularDimension);
  OdDb2LineAngularDimension();
  
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
  */
  OdGePoint3d xLine1Start() const;

  /** \details
    Sets the WCS start point of the first extension line of this Dimension entity (DXF 13).
    \param xLine1Start [in]  Start point.
  */
  void setXLine1Start(
    const OdGePoint3d& xLine1Start);
  
  /** \details
    Returns the WCS end point of the first extension line of this Dimension entity (DXF 14).
  */
  OdGePoint3d xLine1End() const;

  /** \details
    Sets the WCS end point of the first extension line of this Dimension entity (DXF 14).
    \param xLine1End [in]  End point.
  */
  void setXLine1End(
    const OdGePoint3d& xLine1End);
  
  /** \details
    Returns the WCS start point of the second extension line of this Dimension entity (DXF 15).
  */
  OdGePoint3d xLine2Start() const;

  /** \details
    Sets the WCS start point of the second extension line of this Dimension entity (DXF 15).
    \param xLine2Start [in]  Start point.
  */
  void setXLine2Start(
    const OdGePoint3d& xLine2Start);
  
  /** \details
    Returns the WCS end point of the second extension line of this Dimension entity (DXF 10).
  */
  OdGePoint3d xLine2End() const;

  /** \details
    Sets the WCS end point of the second extension line of this Dimension entity (DXF 10).
    \param xLine2End [in]  End point.
  */
  void setXLine2End(
    const OdGePoint3d& xLine2End);

  /** \details
    Returns the extension arc status of this Dimension entity.

    \returns Returns true if extension arc is on, otherwise returns  false.
  */
  bool extArcOn() const;
  
  /** \details
    Sets the extension arc status for this Dimension entity.

    \param value [in] Input extension arc status.
  */
  void setExtArcOn(bool value);

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
  This template class is a specialization of the OdSmartPtr class for OdDb2LineAngularDimension object pointers.
*/
typedef OdSmartPtr<OdDb2LineAngularDimension> OdDb2LineAngularDimensionPtr;

#include "TD_PackPop.h"

#endif
