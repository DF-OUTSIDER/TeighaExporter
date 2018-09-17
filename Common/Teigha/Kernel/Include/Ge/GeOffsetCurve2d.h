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

#ifndef OD_GE_OFFSET_CURVE_2D_H
#define OD_GE_OFFSET_CURVE_2D_H /*!DOM*/

#include "Ge/GeCurve2d.h"

#include "TD_PackPush.h"

/** \details
    This class represents 2D curves that are exact offsets of other curves.

    \remarks
    Every GeOffsetCurve2d contains a pointer to the curve from which it is offest.
    Modifying the base curve modifies the offset curve. Modifying the offset curve
    does not modify the base curve. setInterval() for an offset curve result create a curve that
    is an offset of the specified interval in the base curve.

    This curve may be self-intersecting, even if the base curve is not.

    Positive offset distances at any point on the base curve are defined as 90° counterclockwise
    from the tangent of the base curve at that point.

    An offset curve with a 0.0 offset distance is exact replica of the base curve. An offseet curve with
    a non-zero 0.0 offset distance has a continuity of one less than that of the base curve. 
    To insure that the offset curve is a valid curve, the base curve must have a continuity
    of at least 1.

    Corresponding C++ library: TD_Ge

    <group OdGe_Classes>

    \sa
    <link ge_OdGeOffsetCurve3d.html, Working with Offset Curves>
*/
class GE_TOOLKIT_EXPORT OdGeOffsetCurve2d : public OdGeCurve2d
{
public:
  

  /** \param baseCurve [in]  Any 2D curve.
    \param offsetDistance [in]  Offset distance.
    \param makeCopy [in]  Makes a copy of baseCurve.
    \param source [in]  Object to be cloned.

    \remarks
    Without arguments, the constructor sets the base curve pointer to NULL, and the offset distance to 0.0.
  */

  OdGeOffsetCurve2d();
  OdGeOffsetCurve2d(
    const OdGeCurve2d& baseCurve, 
    double offsetDistance,
    bool makeCopy =  false);
  OdGeOffsetCurve2d(
    const OdGeOffsetCurve2d& source);
  /** \details
    Returns the base curve. 
  */
  const OdGeCurve2d* curve() const;

  /** \details
    Returns the offset distance of this curve. 
  */
  double offsetDistance() const;

  /** \details
    Returns true if and only if this curve has the same parameter direction as the base curve.

    \remarks
    Currently not implemented.
  */
  bool paramDirection() const;

  /** \details
    Returns the concatenation of the transformation matrices applied to this curve with transformBy(). 

    \remarks
    If no transformation matrices have been applied to this curve, returns the identity matrix.

    \remarks
    Currently not implemented.
  */
  OdGeMatrix2d transformation() const;

  /** \details
    Sets the base curve for this curve.

    \param baseCurve [in]  Any 2D curve.
    \param makeCopy [in]  Makes a copy of baseCurve.
  */
  OdGeOffsetCurve2d& setCurve(
    const OdGeCurve2d& baseCurve,
    bool makeCopy =  false);

  /** \details
    Sets the offset distance for this curve.

    \param offsetDistance [in]  Offset distance.
  */
  OdGeOffsetCurve2d& setOffsetDistance(
    double distance);

  OdGeOffsetCurve2d& operator = (
    const OdGeOffsetCurve2d& offsetCurve);

 
  //////////////////////////////////////////////////////////////////////////
  // TD Special overrides :
};

#include "TD_PackPop.h"

#endif // OD_GE_OFFSET_CURVE_2D_H
