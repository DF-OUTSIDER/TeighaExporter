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

#ifndef OD_GEELL3D_H
#define OD_GEELL3D_H /*!DOM*/

class OdGeEllipArc2d;
class OdGeCircArc3d;
class OdGeLineEnt3d;
class OdGePlanarEnt;
class OdGeExtents3d;

#include "Ge/GeCurve3d.h"
#include "OdPlatformSettings.h"

#include "TD_PackPush.h"

/** \details
    This class represents 3D elliptical arcs and full ellipses in 3D space.

    \remarks
    The angle of a point on an ellipse is measured by projecting
    the point along a vector perpendicular to the major axis onto a
    circle whose center is the center of this ellipse and whose
    radius is the major radius of this ellipse.

    The angle between the major axis of the ellipse, and a vector from
    the center of the ellipse to the intersection point with the circle, 
    measured counterclockwise about the crossproduct of the major and minor axes,
    is the angle of the point on the ellipse.

    Corresponding C++ library: TD_Ge

    <group OdGe_Classes> 

    \sa
    <link ge_OdGeEllipArc3d.html, Working with Elliptical Arcs>
*/
class GE_TOOLKIT_EXPORT OdGeEllipArc3d : public OdGeCurve3d
{
public:


  /** \param center [in]  The center of the elliptical arc.
    \param majorAxis [in]  The major axis of the elliptical arc.
    \param minorAxis [in]  The minor axis of the elliptical arc.
    \param majorRadius [in]  The major radius of the elliptical arc.
    \param minorRadius [in]  The minor radius of the elliptical arc.
    \param startAng [in]  Starting angle of the elliptical arc.
    \param endAng [in]  Ending angle of the elliptical arc.
    \param source [in]  Object to be cloned.
    \remarks
    All angles are expressed in radians.

  */
  OdGeEllipArc3d();
  OdGeEllipArc3d(
    const OdGeEllipArc3d& ell);
  OdGeEllipArc3d(
    const OdGeCircArc3d& source);
  OdGeEllipArc3d(
    const OdGePoint3d& center, 
    const OdGeVector3d& majorAxis,
    const OdGeVector3d& minorAxis, 
    double majorRadius,
    double minorRadius);
  OdGeEllipArc3d(
    const OdGePoint3d& center, 
    const OdGeVector3d& majorAxis,
    const OdGeVector3d& minorAxis, 
    double majorRadius,
    double minorRadius, 
    double startAng, 
    double endAng);

  /** \details
    Returns the point on the elliptical arc that is closest to the
    specified plane and the point on the specified plane that is closest
    to the elliptical arc.

    \param plane [in]  Any plane.
    \param pointOnPlane [out]  Receives the closest point on the plane.
    \param tol [in]  Geometric tolerance.
  */
   OdGePoint3d closestPointToPlane(
    const OdGePlanarEnt& plane,
    OdGePoint3d& pointOnPlane,
    const OdGeTol& tol = OdGeContext::gTol) const ;

  /** \details
    Returns True if the specifed linear entity intersects the arc entity,
	  and returns the number of intersections and points of intersection.

    \param line [in]  Any 3D linear entity.
    \param numInt [out]  Receives the number of intersections.
    \param p1 [out]  Receives the first intersection point of the elliptical arc.
    \param p2 [out]  Receives the second intersection point of the elliptical arc.
    \param tol [in]  Geometric tolerance.

    \remarks
    * p1 is valid only if numInt > 0.
    * p2 is valid only if numInt > 1.
  */
   bool intersectWith(
    const OdGeLinearEnt3d& line, 
    int& numInt,
    OdGePoint3d& p1, 
    OdGePoint3d& p2,
    const OdGeTol& tol = OdGeContext::gTol) const ;

  /** \details
    Returns True if the specifed plane or linear entity intersects the elliptical 
	arc entity, and returns the number of intersections and points of intersection.

    \param plane [in]  Any plane entity.
    \param numInt [out]  Receives the number of intersections.
    \param p1 [out]  Receives the first intersection point on the elliptical arc.
    \param p2 [out]  Receives the second intersection point on the elliptical arc. 
    \param tol [in]  Geometric tolerance.   
	
    \remarks
    * p1 has meaning only if numInt > 0. 
    * p2 has meaning only if numInt > 1.   
  */
   bool intersectWith(
    const OdGePlanarEnt& plane, 
    int& numInt,
    OdGePoint3d& p1, 
    OdGePoint3d& p2,
    const OdGeTol& tol = OdGeContext::gTol) const ;

  /** \details
    Returns True if the projected points of the elliptical arc intersect with the 
	specified linear entity, and returns the number of intersections and points of 
	intersection.

    \param line [in]  Any 3D linear entity.
    \param projDir [in]  Projection direction.
    \param numInt [out]  Receives the number of intersections.
    \param pntOnEllipse1 [out]  Receives the first intersection point on the elliptical arc. 
    \param pntOnEllipse2 [out]  Receives the second intersection point on the elliptical arc.
    \param pntOnLine1 [out]  Receives the first intersection point on the line.   
    \param pntOnLine2 [out]  Receives the second intersection point on the line.
    \param tol [in]  Geometric tolerance.
  */
   bool projIntersectWith(
    const OdGeLinearEnt3d& line,
    const OdGeVector3d& projDir, 
    int& numInt,
    OdGePoint3d& pntOnEllipse1,
    OdGePoint3d& pntOnEllipse2,
    OdGePoint3d& pntOnLine1,
    OdGePoint3d& pntOnLine2,
    const OdGeTol& tol = OdGeContext::gTol) const ;

  /** \details
    Returns the plane of the ellipse.
  */
   void getPlane(
    OdGePlane& plane) const ;

  /** \details
    Returns true if and only if the major and minor radii of the ellipse
    are the same within the specified tolerance.

    \param tol [in]  Geometric tolerance.
  */
   bool isCircular(
    const OdGeTol& tol = OdGeContext::gTol) const ;

  /** \details
    Returns true if and only if the specified point lies within
    the full ellipse, and on the plane of the ellipse.

    \param point [in]  Any 3D point.
    \param tol [in]  Geometric tolerance.
  */
   bool isInside(
    const OdGePoint3d& point, 
    const OdGeTol& tol = OdGeContext::gTol) const ;

  /** \details
    Returns the center of this elliptical arc.
  */
   OdGePoint3d center() const ; 

  /** \details
    Returns the minor radius of this elliptical arc.
  */
   double minorRadius() const ;
  
  /** \details
    Returns the major radius of this elliptical arc.
  */
   double majorRadius() const ;

  /** \details
    Returns the minor axis of this elliptical arc as a unit vector.
  */
   OdGeVector3d minorAxis() const ;

  /** \details
    Returns the major axis of this elliptical arc as a unit vector.
  */
   OdGeVector3d majorAxis() const ;

  /** \details
    Returns the normal to this elliptical arc as a unit vector. 

    \remarks
    Positive angles are always drawn counterclockwise about this vector.
  */
   OdGeVector3d normal() const ;

  /** \details
    Returns the start angle measured from the major axis of this elliptical arc in the 
    plane of the arc.

    \remarks
    All angles are expressed in radians.
  */
   double startAng() const ;

  /** \details
    Returns the end angle measured from the major axis of this elliptical arc in the 
    plane of the arc.

    \remarks
    All angles are expressed in radians.
  */
   double endAng() const ;

  /** \details
    Returns the start point of this elliptical arc.
  */
   OdGePoint3d startPoint() const ;

  /** \details
    Returns the end point of this elliptical arc.
  */
   OdGePoint3d endPoint() const ;

  /** \details
    Sets the center of the elliptical arc.

    \param center [in]  The center of the elliptical arc.
  */
   OdGeEllipArc3d& setCenter(
    const OdGePoint3d& center) ;

  /** \details
    Sets the minor radius of the elliptical arc.

    \param minorRadius [in]  The minor radius of the elliptical arc.
  */
   OdGeEllipArc3d& setMinorRadius(
    double rad) ;

  /** \details
    Sets the major radius of the elliptical arc.

    \param majorRadius [in]  The major radius of the elliptical arc.
  */
   OdGeEllipArc3d& setMajorRadius(
    double rad) ;

  /** \details
    Sets the major and minor axes of the elliptical arc.

    \param majorAxis [in]  The major axis of the elliptical arc.
    \param minorAxis [in]  The minor axis of the elliptical arc.
  */
   OdGeEllipArc3d& setAxes(
    const OdGeVector3d& majorAxis,
    const OdGeVector3d& minorAxis) ;

  /** \details
    Sets the starting and ending angles of the elliptical arc.

    \param startAng [in]  Starting angle of the elliptical arc.
    \param endAng [in]  Ending angle of the elliptical arc.
    \remarks
    All angles are expressed in radians.
  */
   OdGeEllipArc3d& setAngles(
    double startAng, 
    double endAng) ;

  /** \details
    Sets the parameters for this elliptical arc according to the arguments, 
    and returns a reference to this elliptical arc.

    \param center [in]  The center of the elliptical arc.
    \param majorAxis [in]  The major axis of the elliptical arc.
    \param minorAxis [in]  The minor axis of the elliptical arc.
    \param majorRadius [in]  The major radius of the elliptical arc.
    \param minorRadius [in]  The minor radius of the elliptical arc.
  */
   OdGeEllipArc3d& set(
    const OdGePoint3d& center,
    const OdGeVector3d& majorAxis,
    const OdGeVector3d& minorAxis,
    double majorRadius, 
    double minorRadius) ;

  /** \details
    Sets the parameters for this elliptical arc according to the arguments, 
    and returns a reference to this elliptical arc.

    \param center [in]  The center of the elliptical arc.
    \param majorAxis [in]  The major axis of the elliptical arc.
    \param minorAxis [in]  The minor axis of the elliptical arc.
    \param majorRadius [in]  The major radius of the elliptical arc.
    \param minorRadius [in]  The minor radius of the elliptical arc.
    \param startAng [in]  Starting angle of the elliptical arc.
    \param endAng [in]  Ending angle of the elliptical arc.
    \remarks
    All angles are expressed in radians.
  */
    OdGeEllipArc3d& set(
    const OdGePoint3d& center,
    const OdGeVector3d& majorAxis,
    const OdGeVector3d& minorAxis,
    double majorRadius, 
    double minorRadius,
    double startAng, 
    double endAng) ;

  /** \details
    Sets the parameters for this elliptical arc according to the arguments, 
    and returns a reference to this elliptical arc.

    \param arc [in]  Any 3D circular arc.
  */
  OdGeEllipArc3d& set(
    const OdGeCircArc3d& arc) ;

  OdGeEllipArc3d& operator =(
    const OdGeEllipArc3d& ell);

  //////////////////////////////////////////////////////////////////////////
  // TD Special :

  // functions making ellipse's axis orthogonal
  // this one requires error flag

  /** \details
    Makes the minor axis orthogonal to the major axis.

    \param tol [in]  Geometric tolerance.
    \param flag [out]  Receives the status of computation.

    \remarks
    Possible values for flag are as follows:

    @untitled table
    kOk
    kDegenerateGeometry
    
    If flag is not specified, the following exception may be thrown:

    Throws:
    <table>
    Exception           
    eDegenerateGeometry 
    </table>
  */
   void orthogonolizeAxes(
    const OdGeTol& tol, 
    OdGe::ErrorCondition& flag) ;
  
  /** \details
    Makes the minor axis orthogonal to the major axis.

    \param tol [in]  Geometric tolerance.

    \remarks
    The following exception may be thrown:

    Throws:
    <table>
    Exception           
    eDegenerateGeometry 
    </table>
  */
  void orthogonolizeAxes(
    const OdGeTol& tol = OdGeContext::gTol) ;

  /** \details
    Returns the tangent vector at the point specified by param.

    \param param [in]  Parameter corresponding to the point at which to compute the tangent.
  */
   OdGeVector3d tangentAt(
    double param) const ;

  /** \details
    Returns the geometric extents of this elliptical arc.

    \param extents [out]  Receives the geometric extents.
  */
  void getGeomExtents(
    OdGeExtents3d& extents) const ;

  /** \details
    Returns the parameters of the points (if any) on the elliptical arc,
    whose tangents are parallel to the specified tangent vector.

    \param tan [in] A vector defining the tangent direction.
    \param params [out] Receives the array of tangent point parameters.

    \remarks
    The parameters are appended to the specified array. You may wish to 
    initialize the params array with clear().
  */
   OdResult inverseTangent(
    OdGeVector3d tan, 
    OdGeDoubleArray& params) const ;

  /** \details
    Returns the parameters of the points (if any) on the elliptical arc,
    whose tangents are parallel to the specified reference plane.

    \param refPlane [in] A reference plane.
    \param params [out] Receives the array of tangent point parameters.

    \remarks
    The parameters are appended to the specified array. You may wish to 
    initialize the params array with clear().
  */
  OdResult inverseTangentPlane(
    const OdGePlane& refPlane, 
    OdGeDoubleArray& params) const ;

};

#include "TD_PackPop.h"

#endif // OD_GEELL3D_H

