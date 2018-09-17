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

#ifndef OD_GEPONC3D_H
#define OD_GEPONC3D_H /*!DOM*/

#include "Ge/GePointEnt3d.h"
#include "Ge/GeCurve3d.h"

class OdGeCurve3d;

#include "TD_PackPush.h"

/** \details
    This class represents points along 3D curves.

    Corresponding C++ library: TD_Ge

    <group OdGe_Classes>

    \remarks
    The point representation includes its parameter value, its coordinates, and the
    the derivatives of the curve at that point.

    \sa
    <link ge_Evaluators.html, Working with Evaluator Classes>
*/
class GE_TOOLKIT_EXPORT OdGePointOnCurve3d : public OdGePointEnt3d
{
public:


  /** \param curve3d [in]  Any 3D curve.
    \param param [in]  Parameter to specify a point on curve3d.

    \remarks
    The default constructor constructs a point on the curve OdGeLine2d::kXAxis with a parameter value of 0
  */
  OdGePointOnCurve3d();
  OdGePointOnCurve3d(
    const OdGeCurve3d& curve3d);
  OdGePointOnCurve3d(
    const OdGeCurve3d& curve3d,
    double param);
  OdGePointOnCurve3d(
    const OdGePointOnCurve3d& src);

  // Assignment operator.
  OdGePointOnCurve3d& operator =(
    const OdGePointOnCurve3d& src);

  /** \details
    Returns the curve on which the point lies.
  */
   const OdGeCurve3d* curve() const;

  /** \details
    Returns the parameter value on the curve corresponding to the point.
  */
   double parameter() const;

  /** \details
    Returns the point on the curve.
  */
   OdGePoint3d point() const;

  /** \details
    Returns the point on the curve.

    \param param [in]  Sets the current parameter.
  */
   OdGePoint3d point(
    double param);

  /** \details
    Returns the point on the curve.

    \param param [in]  Sets the current parameter.
    \param curve3d [in]  Any 3D curve. Sets the current curve.
  */
  OdGePoint3d point(
    const OdGeCurve3d& curve3d,
    double param);

  /** \details
    Returns the derivative of the curve at the point on the curve.

    \param order [in]  The order of the derivative [1-2].

    \remarks
    Currently not implemented.
  */
   OdGeVector3d deriv(
    int order) const;

  /** \details
    Returns the derivative of the curve at the point on the curve.

    \param order [in]  the order of the derivative [1-2].
    \param param [in]  Sets the current parameter.

    \remarks
    Currently not implemented.
  */
    OdGeVector3d deriv(
    int order,
    double param);

  /** \details
    Returns the derivative of the curve at the point on the curve.

    \param order [in]  the order of the derivative [1-2].
    \param param [in]  Sets the current parameter.
    \param curve3d [in]  Any 3D curve. Sets the current curve.

    \remarks
    Currently not implemented.
  */
    OdGeVector3d deriv(
    int order,
    const OdGeCurve3d& curve3d,
    double param);

  /** \details
    Returns true if and only if the first derivative vector
    has a length of zero.

    \param tol [in]  Geometric tolerance.

    \remarks
    Currently not implemented.
  */
   bool isSingular(
    const OdGeTol& tol = OdGeContext::gTol) const;

  /** \details
    Returns true if and only if the first derivative vector has a non-zero length, and
    the curvature of the curve at the point on the curve.

    \param res [out]  Receives the curvature of the curve at the specified point.

    \remarks
    Currently not implemented.
  */
   bool curvature(
    double& res);

  /** \details
    Returns true if and only if the first derivative vector has a non-zero length, and
    the curvature of the curve at the point on the curve.

    \param param [in]  Parameter to specify a point on curve3d.
    \param res [out]  Receives the curvature of the curve at the specified point.

    \remarks
    Currently not implemented.
  */
    bool curvature(
    double param,
    double& res);

  /** \details
    Sets the current curve.

    \param curve3d [in]  Any 3D curve.
  */
   OdGePointOnCurve3d& setCurve(
    const OdGeCurve3d& curve3d);

  /** \details
    Sets the current parameter.

    \param param [in]  Sets the current parameter.
  */
   OdGePointOnCurve3d& setParameter(
    double param);

  //////////////////////////////////////////////////////////////////////////

};


#include "TD_PackPop.h"

#endif // OD_GEPONC3D_H
