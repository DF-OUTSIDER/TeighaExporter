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

#ifndef OD_GE_ELLIP_ARC_2D_H
#define OD_GE_ELLIP_ARC_2D_H /*!DOM*/

class OdGeCircArc2d;
class OdGePlanarEnt;
class OdGeEllipArc2d;
class OdGeLinearEnt2d;
class OdGeExtents2d;
#include "OdPlatform.h"
#include "Ge/GeCurve2d.h"

#include "TD_PackPush.h"

/** \details
    This class represents elliptical arcs and full ellipses in 2D space.

    \remarks
    The angle of a point on an ellipse is measured by projecting
    the point along a vector perpendicular to the major axis onto a
    circle whose center is the center of this ellipse and whose
    radius is the major radius of this ellipse.

    The angle between the major axis of this ellipse, and a vector from
    the center of this ellipse to the intersection point with the circle, 
    measured counterclockwise, is the angle of this point on the ellipse.

    Corresponding C++ library: TD_Ge

    <group OdGe_Classes> 
	
	  \sa
    <link ge_OdGeEllipArc3d.html, Working with Elliptical Arcs>
*/
class GE_TOOLKIT_EXPORT OdGeEllipArc2d : public OdGeCurve2d
{
public:
  

  /** \param center [in]  The center of this elliptical arc.
    \param majorAxis [in]  The major axis of this elliptical arc.
    \param minorAxis [in]  The minor axis of this elliptical arc.
    \param majorRadius [in]  The major radius of this elliptical arc.
    \param minorRadius [in]  The minor radius of this elliptical arc.
    \param startAng [in]  Starting angle of this elliptical arc.
    \param endAng [in]  Ending angle of this elliptical arc.
    \param source [in]  Object to be cloned.

    \remarks
    All angles are expressed in radians.
  */
  OdGeEllipArc2d();
  OdGeEllipArc2d(
    const OdGeEllipArc2d& ell);
  OdGeEllipArc2d(
    const OdGeCircArc2d& arc);
  OdGeEllipArc2d(
    const OdGePoint2d& center, 
    const OdGeVector2d& majorAxis,
    const OdGeVector2d& minorAxis, 
    double majorRadius, 
    double minorRadius);
  OdGeEllipArc2d(
    const OdGePoint2d& center,
    const OdGeVector2d& majorAxis,
    const OdGeVector2d& minorAxis, 
    double majorRadius, 
    double minorRadius, 
    double startAng, 
    double endAng);

  /** \details
    Determines if the input line intersects the ellipse.

    \param line [in]  Input any 2D linear entity  
    \param intn [out]  Output number of intersection points  
    \param p1 [out] Output first intersection point  
    \param p2 [out] Output second intersection point  
    \param tol [in] Output tolerance for tangent intersections  

  */
  bool intersectWith(
    const OdGeLinearEnt2d& line, 
    int& numInt,
    OdGePoint2d& p1, 
    OdGePoint2d& p2,
    const OdGeTol& tol = OdGeContext::gTol) const;

  /** \details
    Determines if an ellipse is a circular arc.

    \param tol [in]  Input tolerance  

  */
  bool isCircular(
    const OdGeTol& tol = OdGeContext::gTol) const;

  /** \details
    Returns true if and only if the specified point lies within
    the full ellipse.

    \param point [in]  Any 2D point.
    \param tol [in]  Geometric tolerance.
  */
  bool isInside(
    const OdGePoint2d& point, 
    const OdGeTol& tol = OdGeContext::gTol) const;

  /** \details
    Returns the center of this elliptical arc.
  */
  OdGePoint2d center() const;

  /** \details
    Returns the minor radius of this elliptical arc.
  */
  double minorRadius() const;

  /** \details
    Returns the major radius of this elliptical arc.
  */
  double majorRadius() const;

  /** \details
    Returns the minor axis of this elliptical arc as a unit vector.
  */
  OdGeVector2d minorAxis() const;

  /** \details
    Returns the major axis of this elliptical arc as a unit vector.
  */
  OdGeVector2d majorAxis() const;

  /** \details
    Returns the start angle measured from major axis of this elliptical arc.
    \remarks
    All angles are expressed in radians.

  */
  double startAng() const;

  /** \details
    Returns the end angle measured from major axis of this elliptical arc.
    \remarks
    All angles are expressed in radians.

  */
  double endAng() const;

  /** \details
    Returns the start point of this elliptical arc.
  */
  OdGePoint2d startPoint() const;

  /** \details
    Returns the end point of this elliptical arc.
  */
  OdGePoint2d endPoint() const;

  /** \details
    Returns true if and only if this elliptical arc is drawn clockwise from start point to end point.
  */
  bool isClockWise() const;

  /** \details
    Sets the center of this elliptical arc.

    \param center [in]  The center of this elliptical arc.
  */
  OdGeEllipArc2d& setCenter(
  const OdGePoint2d& center);

  /** \details
    Sets the minor radius of this elliptical arc.

    \param minorRadius [in]  The minor radius of this elliptical arc.
  */
  OdGeEllipArc2d& setMinorRadius(
    double rad);

  /** \details
    Sets the major radius of this elliptical arc.

    \param majorRadius [in]  The major radius of this elliptical arc.
  */
  OdGeEllipArc2d& setMajorRadius(
    double rad);

  /** \details
    Sets the major and minor axes of this elliptical arc.

    \param majorAxis [in]  The major axis of this elliptical arc.
    \param minorAxis [in]  The minor axis of this elliptical arc.
  */
  OdGeEllipArc2d& setAxes(
    const OdGeVector2d& majorAxis,
    const OdGeVector2d& minorAxis);

  /** \details
    Sets the starting and ending angles of this elliptical arc.

    \param startAng [in]  Starting angle of this elliptical arc.
    \param endAng [in]  Ending angle of this elliptical arc.
    \remarks
    All angles are expressed in radians.

  */
  OdGeEllipArc2d& setAngles(
    double startAng, 
    double endAng);

  /** \details
    Sets the parameters for this elliptical arc according to the arguments, and returns a reference to this elliptical arc.

    \param center [in]  The center of this elliptical arc.
    \param majorAxis [in]  The major axis of this elliptical arc.
    \param minorAxis [in]  The minor axis of this elliptical arc.
    \param majorRadius [in]  The major radius of this elliptical arc.
    \param minorRadius [in]  The minor radius of this elliptical arc.
  */
  OdGeEllipArc2d& set(
    const OdGePoint2d& center,
    const OdGeVector2d& majorAxis,
    const OdGeVector2d& minorAxis,
    double majorRadius, 
    double minorRadius);

  /** \details
    Sets the parameters for this elliptical arc according to the arguments, and returns a reference to this elliptical arc.

    \param center [in]  The center of this elliptical arc.
    \param majorAxis [in]  The major axis of this elliptical arc.
    \param minorAxis [in]  The minor axis of this elliptical arc.
    \param majorRadius [in]  The major radius of this elliptical arc.
    \param minorRadius [in]  The minor radius of this elliptical arc.
    \param startAng [in]  Starting angle of this elliptical arc.
    \param endAng [in]  Ending angle of this elliptical arc.

    \remarks
    All angles are expressed in radians.
  */
    OdGeEllipArc2d& set(
    const OdGePoint2d& center,
    const OdGeVector2d& majorAxis,
    const OdGeVector2d& minorAxis,
    double majorRadius, 
    double minorRadius,
    double startAng, 
    double endAng);
 
  /** \details
    Sets the parameters for this elliptical arc according to the arguments, and returns a reference to this elliptical arc.

    \param arc [in]  Any 2D circular arc.
  */
  OdGeEllipArc2d& set(
    const OdGeCircArc2d& arc);

  OdGeEllipArc2d& operator =(
    const OdGeEllipArc2d& ell);

  //////////////////////////////////////////////////////////////////////////
  // Overrides :

  //////////////////////////////////////////////////////////////////////////
  // TD Special :

  /** \details
  Returns the geometric extents of this elliptical arc.

  \param extents [out]  Receives the geometric extents.
  */
  void getGeomExtents(
    OdGeExtents2d& extents) const;

  // returns the parameter of elliptical arc points (if any) which have tangent 
  // co-directional to "tan" vector. Parameters are returned in params array.
  // Note that this function doesn't clear array - only appends items.

  /** \details
    Returns the parameters of the points (if any) on the elliptical arc,
    whose tangents are parallel to the specified tangent vector.

    \param tan [in] A vector defining the tangent direction.
    \param params [out] Receives the array of tangent point parameters.

    \remarks
    The parameters are appended to the specified array. You may wish to 
    initialize the params array with clear().
  */
  void inverseTangent(
    OdGeVector2d tan, 
    OdGeDoubleArray& params);
};

#include "TD_PackPop.h"

#endif // OD_GE_ELLIP_ARC_2D_H
