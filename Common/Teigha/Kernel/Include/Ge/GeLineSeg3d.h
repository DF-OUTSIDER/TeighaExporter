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

#ifndef OD_GELNSG3D_H
#define OD_GELNSG3D_H /*!DOM*/

class OdGeLineSeg2d;
#include "Ge/GeLinearEnt3d.h"
#include "Ge/GeInterval.h"

#include "TD_PackPush.h"

/** \details
    This class represents line segments in 3D space.

    Corresponding C++ library: TD_Ge

    <group OdGe_Classes>

    \sa
    <link ge_OdGeLineSeg.html, Working with Line Segments>
*/
class GE_TOOLKIT_EXPORT OdGeLineSeg3d : public OdGeLinearEnt3d
{
public:
  /** \details
    Constructs a line segment with start and end points at coordinates origin (0,0,0).
  */
  OdGeLineSeg3d();
  
  /** \details
    Constructs a line segment.

    \param source [in]  Object to be cloned.
  */  
  OdGeLineSeg3d(
    const OdGeLineSeg3d& source);

  /** \details
    Constructs a line segment between the points: point and point + vect.

    \param point [in]  Any 3D point.
    \param vect [in]  Any 3D vector.

    \remarks
    vect can have a zero length.
  */
    OdGeLineSeg3d(
    const OdGePoint3d& point, 
    const OdGeVector3d& vect);
  
  /** \details
    Constructs a line segment between the specified points.

    \param point1 [in]  Any 3D point.
    \param point2 [in]  Any 3D point.

    \remarks
    Points point1 and point2 can be coincident.
  */
  OdGeLineSeg3d(
    const OdGePoint3d& point1, 
    const OdGePoint3d& point2);

  /** \details
    Gets the unbounded perpendicular bisecting plane of this line segment.

    \param line [out]  Receives the bisecting plane.
  */
  void getBisector(OdGePlane& plane) const;

  /** \details
    Returns the weighted average of the start point and end point of this line segment.

    \param blendCoeff [in]  Blend coefficient.

    \remarks
    <table>
    blendCoeff      Returns
    0               start point
    1               end point
    0 to 1          point on this line segment
    < 0 or > 1      point not on this line segment, but colinear with it.
    </table>
  */
  OdGePoint3d baryComb(double blendCoeff) const;

  /** \details
    Returns the start point of this line segment.
  */
   OdGePoint3d startPoint() const;

  /** \details
    Returns the midpoint of this line segment.
  */
   OdGePoint3d midPoint() const;

  /** \details
    Returns the end point of this line segment.
  */
   OdGePoint3d endPoint() const;

  /** \details
    Sets the parameters for this line according to the arguments, and returns a reference to this line.

    \param point [in]  Any 3D point.
    \param vect [in]  Any 3D vector.

    \remarks
    point and vect construct a line segment between the points: point and point + vect. vect can have a zero length.
  */
  OdGeLineSeg3d& set(
    const OdGePoint3d& point, 
    const OdGeVector3d& vect);

  /** \details
    Sets the parameters for this line according to the arguments, and returns a reference to this line.

    \param point1 [in]  Any 3D point.
    \param point2 [in]  Any 3D point.

    \remarks
    point1 and point2 construct a line segment between points point1 and point2. The 
    points can be coincident.
  */   
  OdGeLineSeg3d& set(
    const OdGePoint3d& point1, 
    const OdGePoint3d& point2);

  /** \details
    Sets the parameters for this line segment according to the arguments, and returns a reference to this line segment.

    \param curve1 [in]  Any 3D curve.
    \param curve2 [in]  Any 3D curve.
    \param param1 [in]  A point on curve1 where line segment is tangent to curve.
    \param param2 [in]  A point on curve2 where line segment is tangent to curve.
    \param success [out]  Indicates that tangent line was computed successfully.

    \remarks
    If success returns false, the line segment does not change. 
  */
  OdGeLineSeg3d& set(
    const OdGeCurve3d& curve1,
    const OdGeCurve3d& curve2,
    double& param1, double& param2,
    bool& success);

  /** \details
    Sets the parameters for this line segment according to the arguments, and returns a reference to this line segment.

    \param curve [in]  Any 3D curve.
    \param point [in]  Any 3D point.
    \param param [in]  A point on curve where line segment is tangent to curve.
    \param success [out]  Indicates that tangent line was computed successfully.

    \remarks
    The point must lie on the plane of the curve and the curve must be planar.
    If success returns false, the line segment does not change.
  */
  OdGeLineSeg3d& set(
    const OdGeCurve3d& curve,
    const OdGePoint3d& point,
    double& param,
    bool& success);

  OdGeLineSeg3d& operator =(
    const OdGeLineSeg3d& line);

  /** \details
    Returns the distance to vector.

    \param point [in]  Any 3D point.
    \param tol [in]  Tolerance.
  */
  double getDistanceToVector(const OdGePoint3d& point, const OdGeTol& tol = OdGeContext::gTol) const;
};

#include "TD_PackPop.h"

#endif

