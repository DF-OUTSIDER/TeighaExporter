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

#ifndef OD_GECSPL3D_H
#define OD_GECSPL3D_H /*!DOM*/

#include "Ge/GeSplineEnt3d.h"

#include "TD_PackPush.h"

/** \details
    This class represents 3D interpolation cubic spline curves.
    Currently not implemented.

    Corresponding C++ library: TD_Ge

   <group OdGe_Classes> 
*/
class GE_TOOLKIT_EXPORT OdGeCubicSplineCurve3d : public OdGeSplineEnt3d
{
public:


  /** \param fitPnts [in]  An array of 3D fit points.
    \param tol [in]  Geometric tolerance.
    \param startDeriv [in]  Starting derivative of the cubic spline curve.
    \param endDeriv [in]  Ending derivative of this cubic spline curve.
    \param curve [in]  A curve to be approximated by this cubic spline curve.
    \param knots [in]  Knot vector.
    \param isPeriodic [in]  True if and only if the cubic spline curve is to be periodic (closed).
    \param firstDerivs [in]  Array of first derivatives at the fit points.
    \param source [in]  Object to be cloned.

    \remarks
    OdGeCubicCplineCurve3d(fitPnts, tol) constructs a periodic (closed)
    cubic spline curve. The last fit point must equal the first.

    \remarks
    Currently not implemented.
  */
  OdGeCubicSplineCurve3d();
  OdGeCubicSplineCurve3d(
    const OdGeCubicSplineCurve3d& source);
  OdGeCubicSplineCurve3d(
    const OdGePoint3dArray& fitPnts,
    const OdGeTol& tol = OdGeContext::gTol);
  OdGeCubicSplineCurve3d(
    const OdGePoint3dArray& fitPnts,
    const OdGeVector3d& startDeriv,
    const OdGeVector3d& endDeriv,
    const OdGeTol& tol = OdGeContext::gTol);
  OdGeCubicSplineCurve3d(
    const OdGeCurve3d& curve,
    double epsilon 
#ifndef SWIG
    = OdGeContext::gTol.equalPoint()
#endif
    );
  OdGeCubicSplineCurve3d(
    const OdGeKnotVector& knots,
    const OdGePoint3dArray& fitPnts,
    const OdGeVector3dArray& firstDerivs,
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
   OdGePoint3d fitPointAt(
    int fotPointIndex) const;

  /** \details
    Sets the fit point at the specified index, and returns a reference to this curve.

    \param fitPointIndex [in]  Fit point index.
    \param point [in]  Any 3D point.

    \remarks
    Currently not implemented.
  */
   OdGeCubicSplineCurve3d& setFitPointAt(
    int fitPointIndex, 
    const OdGePoint3d& point);

  /** \details
    Returns the first derivative at the specified index.

    \param fitPointIndex [in]  Fit point index.

    \remarks
    Currently not implemented.
  */
   OdGeVector3d firstDerivAt(
    int fitPointIndex) const;

  /** \details
    Sets the first derivative at the specified index, and returns a reference to this curve.

    \param fitPointIndex [in]  Fit point index.
    \param deriv [in]  The first derivative at the fit point.

    \remarks
    Currently not implemented.
  */
   OdGeCubicSplineCurve3d& setFirstDerivAt(
    int fitPointIndex, 
    const OdGeVector3d& deriv);

  OdGeCubicSplineCurve3d&  operator =(
    const OdGeCubicSplineCurve3d& spline);
};

#include "TD_PackPop.h"

#endif

