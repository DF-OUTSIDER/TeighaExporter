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

#ifndef OD_GE_CUBIC_SPLINE_CURVE_2D_H
#define OD_GE_CUBIC_SPLINE_CURVE_2D_H /*!DOM*/

//
/** \details
  
   This file contains the class OdGeCubicSplineCurve2d - A mathematic
   entity used to represent a 2d interpolation cubic-spline curve.
*/

#include "Ge/GeSplineEnt2d.h"
#include "Ge/GePoint2dArray.h"
#include "Ge/GeVector2dArray.h"

#include "TD_PackPush.h"

/** \details
    This class represents 2D interpolation cubic spline curves.
    Currently not implemented.

    Corresponding C++ library: TD_Ge

   <group OdGe_Classes> 
*/
class GE_TOOLKIT_EXPORT OdGeCubicSplineCurve2d : public OdGeSplineEnt2d
{
public:
  /** \param fitPnts [in]  An array of 2D fit points.
    \param tol [in]  Geometric tolerance.
    \param startDeriv [in]  Starting derivative of the cubic spline curve.
    \param endDeriv [in]  Ending derivative of this cubic spline curve.
    \param curve [in]  A curve to be approximated by this cubic spline curve.
    \param knots [in]  Knot vector.
    \param isPeriodic [in]  True if and only if the cubic spline curve is to be periodic (closed).
    \param firstDerivs [in]  Array of first derivatives at the fit points.
    \param source [in]  Object to be cloned.

    \remarks
    OdGeCubicCplineCurve2d(fitPnts, tol) constructs a periodic (closed)
    cubic spline curve. The last fit point must equal the first.

    \remarks
    Currently not implemented.
  */
  OdGeCubicSplineCurve2d();
  OdGeCubicSplineCurve2d(
    const OdGeCubicSplineCurve2d& source);
  OdGeCubicSplineCurve2d(
    const OdGePoint2dArray& fitPnts,
    const OdGeTol& tol = OdGeContext::gTol);
  OdGeCubicSplineCurve2d(
    const OdGePoint2dArray& fitPnts,
    const OdGeVector2d& startDeriv,
    const OdGeVector2d& endDeriv,
    const OdGeTol& tol = OdGeContext::gTol);
  OdGeCubicSplineCurve2d(
    const OdGeCurve2d& curve,
    double tol 
#ifndef SWIG
    = OdGeContext::gTol.equalPoint()
#endif
    );
  OdGeCubicSplineCurve2d(
    const OdGeKnotVector& knots,
    const OdGePoint2dArray& fitPnts,
    const OdGeVector2dArray& firstDerivs,
    bool isPeriodic = false);

  /** \details
    Returns the number of fit points.

    \remarks
    Currently not implemented.
  */
  int numFitPoints() const;

  /** \details
    Returns the fit point at the specified index.

    \param fitPointIndex [in]  Fit point index.

    \remarks
    Currently not implemented.
  */
  OdGePoint2d fitPointAt(
    int fitPointIndex) const;

  /** \details
    Sets the fit point at the specified index, and returns a reference to this curve.

    \param fitPointIndex [in]  Fit point index.
    \param point [in]  Any 2D point.

    \remarks
    Currently not implemented.
  */
  OdGeCubicSplineCurve2d& setFitPointAt(
    int fitPointIndex, 
    const OdGePoint2d& point);

  /** \details
    Returns the first derivative at the specified index.

    \param fitPointIndex [in]  Fit point index.

    \remarks
    Currently not implemented.
  */
  OdGeVector2d firstDerivAt(
    int fitPointIndex) const;

  /** \details
    Sets the first derivative at the specified index, and returns a reference to this curve.

    \param fitPointIndex [in]  Fit point index.
    \param deriv [in]  The first derivative at the fit point.

    \remarks
    Currently not implemented.
  */
  OdGeCubicSplineCurve2d& setFirstDerivAt(
    int fitPointIndex, 
    const OdGeVector2d& deriv);

  /** \remarks
    Currently not implemented.
  */
    OdGeCubicSplineCurve2d&  operator =(
    const OdGeCubicSplineCurve2d& spline);
};

#include "TD_PackPop.h"

#endif // OD_GE_CUBIC_SPLINE_CURVE_2D_H
