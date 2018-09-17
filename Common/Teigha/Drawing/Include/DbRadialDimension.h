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




#ifndef _OD_DB_RADIAL_DIMENSION_
#define _OD_DB_RADIAL_DIMENSION_

#include "TD_PackPush.h"

#include "DbDimension.h"
#include "DwgDeclareMembers.h"

/** \details
    This class represents Radial Dimension entities in an OdDbDatabase instance.

    \sa
    TD_Db

    \remarks
    Radial Dimension entities require a point the curve being dimensioned and
    a center point. In additional, leaderLength specifies
    how far the dimension line extends beyond the curve before
    the dogleg to the annotation text. 
    <group OdDb_Classes>
*/
class DBDIM_EXPORT OdDbRadialDimension : public OdDbDimension
{
public:
  DWGMAP_DECLARE_MEMBERS(OdDbRadialDimension);

  OdDbRadialDimension();
  
  /* OdDbRadialDimension(  
       const OdGePoint3d& center,
       const OdGePoint3d& chordPoint,
       double leaderLength,
       const OdChar* dimText = 0,
       OdDbObjectId dimStyle = OdDbObjectId::kNull);
  */

  /** \details
    Sets the length from the chordPoint to the dogleg for this Dimension entity
    (DXF 40).
    
    \param leaderLength [in]  Leader length.
  */
  void setLeaderLength(
    double leaderLength);

  /** \details
    Returns the length from the chordPoint to the dogleg for this Dimension entity
    (DXF 40).
  */
  double leaderLength() const; 
  
  /** \details
    Returns the WCS center of the arc being dimensioned by this Dimension entity (DXF 10).
  */
  OdGePoint3d center() const;

  /** \details
    Sets the WCS center of the arc being dimensioned by this Dimension entity (DXF 10).
    \param center [in]  Center.
  */
  void setCenter(
    const OdGePoint3d& center);
  
  /** \details
    Returns the WCS chord point for this Dimension entity (DXF 15).
    
    \remarks
    The chord point is the point at which the dimension line for this Dimension entity
    intersects the curve being dimensioned.
  */
  OdGePoint3d chordPoint() const;

  /** \details
    Sets the WCS chord point for this Dimension entity (DXF 15).
    
    \param chordPoint [in]  Chord point.
    
    \remarks
    The chord point is the point at which the dimension line for this Dimension entity
    intersects the curve being dimensioned.
  */
  void setChordPoint(
    const OdGePoint3d& chordPoint);

  /** \details
    Returns the extension arc status of this Dimension entity.

    \returns Returns true if extension arc is on, otherwise returns false.
  */
  bool extArcOn() const;
  
  /** \details
    Returns the extension arc start angle of this dimension entity.

    \returns Returns extension arc start angle as a double value.
  */
  double extArcStartAngle() const;
  
  /** \details
    Sets the extension arc start angle for this dimension entity.

    \param newAngle [in] Input arc start angle. 
  */
  void setExtArcStartAngle(double newAngle);
  
  /** \details
    Returns the extension arc end angle of this dimension entity.

    \returns Returns extension arc end angle as a double value.
  */
  double extArcEndAngle() const;
  
  /** \details
    Sets the extension arc end angle for this dimension entity.

    \param newAngle [in] Input arc end angle. 
  */
  void setExtArcEndAngle(double newAngle);

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
  This template class is a specialization of the OdSmartPtr class for OdDbRadialDimension object pointers.
*/
typedef OdSmartPtr<OdDbRadialDimension> OdDbRadialDimensionPtr;

#include "TD_PackPop.h"

#endif
