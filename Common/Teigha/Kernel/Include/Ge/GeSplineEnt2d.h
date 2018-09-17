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

#ifndef OD_GE_SPLINE_ENT_2D_H
#define OD_GE_SPLINE_ENT_2D_H /*!DOM*/

class OdGeKnotVector;

#include "Ge/GeCurve2d.h"

#include "TD_PackPush.h"

/** \details
    This class represents various spline objects in 2D space. Currently not implemented.

    Corresponding C++ library: TD_Ge

    <group OdGe_Classes> 
*/
class GE_TOOLKIT_EXPORT OdGeSplineEnt2d : public OdGeCurve2d
{
public:

  /** \details
    Returns true if and only if the spline is rational or a polynomial.

    \remarks
    This function is meaningful only for OdGeNurbCurve2d objects derived from OdGeSplineEnt2d.

    \remarks
    Currently not implemented.
  */
  bool isRational() const;

  /** \details
    Returns the degree of the spline.

    \remarks
    Currently not implemented.
  */
  int degree() const;

  /** \details
    Returns the order of the spline.

    \remarks
    Currently not implemented.
  */
  int order() const;

  /** \details
    Returns the number of knots in the knot vector.

    \remarks
    Currently not implemented.
  */
  int numKnots() const;

  /** \details
    Returns the knot vector.

    \remarks
    Currently not implemented.
  */
  const OdGeKnotVector& knots() const;

  /** \details
    Returns the number of points in the control point array.

    \remarks
    Currently not implemented.
  */
  int numControlPoints() const;

  /** \details
    Returns the degree of the highest derivative that is defined at a specified knot.

    \param knotIndex [in]  Knot index.
    \param tol [in]  Geometric tolerance.

    \remarks
    Currently not implemented.
  */
  int continuityAtKnot(
    int knotIndex, 
    const OdGeTol& tol = OdGeContext::gTol) const;

  /** \details
    Returns the parameter corresponding to the start point of this spline.

    \remarks
    Currently not implemented.
  */
  double startParam() const;

  /** \details
    Returns the parameter corresponding to the end point of this spline.

    \remarks
    Currently not implemented.
  */
  double endParam() const;

  /** \details
    Returns the start point of this spline.

    \remarks
    Currently not implemented.
  */
  OdGePoint2d startPoint() const;

  /** \details
    Returns the end point of this spline.

    \remarks
    Currently not implemented.
  */
  OdGePoint2d endPoint() const;

  /** \details
    Returns true if and only if the spline is constructed using fit points.

    \remarks
    Currently not implemented.
  */
  bool hasFitData() const;

  /** \details
    Returns a knot value for the specified knot.

    \param knotIndex [in]  Knot index.

    \remarks
    Currently not implemented.
  */
  double knotAt(
    int knotIndex) const;

  /** \details
    Sets the knot value for the specified knot.

    \param knotIndex [in]  Knot index.
    \param val [in]  The new value for the knot.

    \remarks
    Currently not implemented.
  */
  OdGeSplineEnt2d& setKnotAt(
    int knotIndex, 
    double val);

  /** \details
    Returns the specified control point in the control point array.

    \param controlPointIndex [in]  Control point index.

    \remarks
    Currently not implemented.
  */
  OdGePoint2d controlPointAt(
    int controlPointIndex) const;

  /** \details
    Sets the specified control point in the control point array.

    \param controlPointIndex [in]  Control point index.

    \remarks
    Currently not implemented.
  */
  OdGeSplineEnt2d& setControlPointAt(
    int controlPointIndex, 
    const OdGePoint2d& point);

  OdGeSplineEnt2d& operator =(const OdGeSplineEnt2d& spline);

  //////////////////////////////////////////////////////////////////////////

protected:
  OdGeSplineEnt2d();
  OdGeSplineEnt2d(
    const OdGeSplineEnt2d& spline);
};

#include "TD_PackPop.h"

#endif // OD_GE_SPLINE_ENT_2D_H
