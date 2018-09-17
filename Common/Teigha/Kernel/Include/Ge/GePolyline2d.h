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

#ifndef OD_GE_POLYLINE_2D_H
#define OD_GE_POLYLINE_2D_H /*!DOM*/

#include "OdPlatform.h"
#include "Ge/GeSplineEnt2d.h"

#include "TD_PackPush.h"

/** \details
    This class represents piecewise linear splines in 2D space.

    Corresponding C++ library: TD_Ge

    <group OdGe_Classes> 
*/
class GE_TOOLKIT_EXPORT OdGePolyline2d : public OdGeSplineEnt2d
{
public:
  

  OdGePolyline2d();
  /** \param source [in]  Object to be cloned.
  */
  OdGePolyline2d(
    const OdGePolyline2d& source);
  /** \param points [in]  Array of fit points.
    \param knots [in]  Knot vector.
    \param crv [in]  A 2D curve to be approximated as a polyline.
    \param approxEps [in]  Approximate geometric tolerance. 
  */
  OdGePolyline2d(
    const OdGePoint2dArray& fitpoints);
  OdGePolyline2d(
    const OdGeKnotVector& knots,
    const OdGePoint2dArray& points);
  OdGePolyline2d(
    const OdGeCurve2d& crv, 
    double approxEps);

  /** \details
    Returns the number of fit points.
  */
  int numFitPoints() const;

  /** \details
    Returns the fit point at the specified index.

    \param fitPointIndex [in]  Fit point index.
  */
  OdGePoint2d fitPointAt(
    int fitPointIndex) const;

  /** \details
    Sets the fit point at the specified index, and returns a reference to this curve.

    \param fitPointIndex [in]  Fit point index.
    \param point [in]  Any 2D point.
  */
  OdGeSplineEnt2d& setFitPointAt(
    int fitPointIndex, 
    const OdGePoint2d& point);

  OdGePolyline2d& operator =(const OdGePolyline2d& pline);
};

#include "TD_PackPop.h"

#endif // OD_GE_POLYLINE_2D_H

