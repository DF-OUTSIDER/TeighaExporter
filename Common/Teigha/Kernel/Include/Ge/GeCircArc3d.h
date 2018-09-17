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

#ifndef OD_GECIRCARC3D_H
#define OD_GECIRCARC3D_H /*!DOM*/

#include "Ge/GeCurve3d.h"
#include "Ge/GePlane.h"

#include "TD_PackPush.h"

class OdGeLine3d;
class OdGeCircArc2d;
class OdGePlanarEnt;
class OdGeExtents3d;

/** \details
    A mathematical entity used to represent a circular arc in 3D space.

    Corresponding C++ library: TD_Ge

    <group OdGe_Classes>

    \sa
    <link ge_OdGeCircArc3d.html, Working with Circular Arcs>
*/
class GE_TOOLKIT_EXPORT OdGeCircArc3d : public OdGeCurve3d
{
public:


  /** \param center [in]  Center of arc.
    \param normal [in]  A vector normal to the plane of the arc
    \param radius [in]  Radius of arc.
    \param startAng [in]  Starting angle of arc.
    \param endAng [in]  Ending angle of arc.
    \param refVec [in]  The reference vector defining arc angle 0.
    \param isClockWise [in]  If true, the arc is drawn clockwise, otherwise, counterclockwise.
    \param startPoint [in]  Startpoint of arc.
    \param secondPoint [in]  Second point on a 3-point arc.
    \param endPoint [in]  Endpoint of arc.
    \param source [in]  Object to be cloned.

    \remarks
    To construct a circle, set endAng = startAng + Oda2PI

    \remarks
    All angles are expressed in radians.

    startAng must be less than endAng.
  */
  OdGeCircArc3d();
  OdGeCircArc3d(
    const OdGeCircArc3d& source);
  OdGeCircArc3d(
    const OdGePoint3d& center,
    const OdGeVector3d& normal,
    double radius);
  OdGeCircArc3d(
    const OdGePoint3d& center,
    const OdGeVector3d& normal,
    const OdGeVector3d& refVec,
    double radius, double startAng = 0,
    double endAng = Oda2PI);
  OdGeCircArc3d(
    const OdGePoint3d& startPoint,
    const OdGePoint3d& secondPoint,
    const OdGePoint3d& endPoint);

  /** \details
    Returns the point on this circle closest
    to the specified plane, and the point
    on the plane closest to this circle.

    \param plane [in]  Any plane.
    \param pointOnPlane [out]  Receives the closest point on plane.
    \param tol [in]  Geometric tolerance.
  */
  OdGePoint3d closestPointToPlane(
    const OdGePlanarEnt& plane,
    OdGePoint3d& pointOnPlane,
    const OdGeTol& tol = OdGeContext::gTol) const;

  /** \details
    Returns the intersections with other objects.

    \param arc [in]  Any 3D arc.
    \param line [in]  Any 3D linear entity.
    \param plane [in]  Any plane.
    \param numInt [out]  Receives the number of intersections.
    \param p1 [out]  Receives the first intersection point.
    \param p2 [out]  Receives the second intersection point.
    \param tol [in]  Geometric tolerance.

  */
   bool intersectWith(
    const OdGeLinearEnt3d& line,
    int& numInt,
    OdGePoint3d& p1,
    OdGePoint3d& p2,
    const OdGeTol& tol = OdGeContext::gTol) const;

  /** \details
    Returns True if the specified arc intersects the arc entity.

    \param arc [in]  Any 3D arc entity.
    \param intn [out]  Receives the number of intersections.
    \param p1 [out]  Receives the first intersection point on the arc.
    \param p2 [out]  Receives the second intersection point on the arc.
    \param tol [in]  Geometric tolerance.

    \remarks
    * p1 has meaning only if intn > 0.
    * p2 has meaning only if intn > 1.
  */
   bool intersectWith(
    const OdGeCircArc3d& arc,
    int& numInt,
    OdGePoint3d& p1,
    OdGePoint3d& p2,
    const OdGeTol& tol = OdGeContext::gTol) const;

  /** \details
    Returns True if the specifed plane, line, or arc entity intersects this
	arc entity, and returns the number of intersections and points of intersection.

    \param plane [in]  Any plane entity.
    \param numInt [out]  Receives the number of intersections.
    \param p1 [out]  Receives the first intersection point on the arc.
    \param p2 [out]  Receives the second intersection point on the arc.
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
    const OdGeTol& tol = OdGeContext::gTol) const;

  /** \details
    Returns True if the projected points of the arc intersect with the
	specified linear entity, and returns the number of intersections
	and points of intersection.

    \param line [in]  Any 3D linear entity.
    \param projDir [in]  Projection direction.
    \param numInt [out]  Receives the number of intersections.
    \param pntOnArc1 [out]  Receives the first intersection point on the arc.
    \param pntOnArc2 [out]  Receives the second intersection point on the arc.
    \param pntOnLine1 [out]  Receives the first intersection point on the line.
    \param pntOnLine2 [out]  Receives the second intersection point on the line.
    \param tol [in]  Geometric tolerance.
  */
   bool projIntersectWith(
    const OdGeLinearEnt3d& line,
    const OdGeVector3d& projDir,
    int& numInt,
    OdGePoint3d& pntOnArc1,
    OdGePoint3d& pntOnArc2,
    OdGePoint3d& pntOnLine1,
    OdGePoint3d& pntOnLine2,
    const OdGeTol& tol = OdGeContext::gTol) const;

  /** \details
    Returns true if and only if the specified point is
    on the full circle of this arc, the tangent
    at that point.

    \param point [in]  The point on the full circle.
    \param line [out]  Receives the tangent line at that point.
    \param tol [in]  Geometric tolerance.
  */
   bool tangent(
    const OdGePoint3d& point,
    OdGeLine3d& line,
    const OdGeTol& tol = OdGeContext::gTol) const;
   
  /** \details
    Returns true if and only if the specified point is
    on the full circle of this arc, the tangent
    at that point, and the status of the query.

    \param point [in]  The point on the full circle.
    \param line [out]  Receives the tangent line at that point.
    \param tol [in]  Geometric tolerance.
    \param status [out]  Receives the status of the query.

    \remarks
    Possible values for status are as follows:

    @untitled table
    kArg1TooBig
    kArg1InsideThis
    kArg1OnThis
  */
   bool tangent(
    const OdGePoint3d& point,
    OdGeLine3d& line,
    const OdGeTol& tol,
    OdGeError& status) const;

  /** \details
    Returns the plane of the arc.

    \param plane [out]  Receives the plane of the arc.
  */
   void getPlane(
    OdGePlane& plane) const;

  /** \details
    Returns true if and only if the specified point lies inside the full circle of this arc, and is
    on the same plane as this arc.

    \param point [in]  Any 3D point.
    \param tol [in]  Geometric tolerance.
  */
   bool isInside(
    const OdGePoint3d& point,
    const OdGeTol& tol = OdGeContext::gTol) const;

  /** \details
    Returns the center of this arc.
  */
   OdGePoint3d center() const;

  /** \details
    Returns the vector normal to the plane of this arc.
  */
   OdGeVector3d normal() const;

  /** \details
    Returns the reference vector as a unit vector.
  */
   OdGeVector3d refVec() const;

  /** \details
    Returns the radius of this arc.
  */
   double radius() const;

  /** \details
    Returns the starting angle measured from the reference vector in the arc direction.
    \remarks
    All angles are expressed in radians.
  */
   double startAng() const;

  /** \details
    Returns the ending angle measured from the reference vector in the arc direction.
    \remarks
    All angles are expressed in radians.
  */
   double endAng() const;

  /** \details
    Returns the start point of this arc.
  */
   OdGePoint3d startPoint() const;

  /** \details
    Returns the end point of this arc.
  */
   OdGePoint3d endPoint() const;

  /** \details
    Sets the center of this arc, and returns a reference to this arc.

    \param center [in]  Center of arc.
  */
   OdGeCircArc3d& setCenter(
    const OdGePoint3d& center);

  /** \details
    Sets the normal and reference vectors for this arc. Returns a reference
    to this arc.

    \param normal [in]  A vector normal to the plane of the arc.
    \param refVec [in]  The reference vector defining arc angle 0.
  */
   OdGeCircArc3d& setAxes(
    const OdGeVector3d& normal,
    const OdGeVector3d& refVec);

  /** \details
    Sets the radius of this arc, and returns a reference to this arc.

    \param radius [in]  Radius of arc.
  */
   OdGeCircArc3d& setRadius(
    double radius);

  /** \details
    Sets the starting and ending angles of this arc, and returns a reference to this arc.

    \param startAng [in]  Starting angle of arc.
    \param endAng [in]  Ending angle of arc.
    \remarks
    All angles are expressed in radians.
  */
   OdGeCircArc3d& setAngles(
    double startAng,
    double endAng);

  /** \details
    Sets the parameters for this arc according to the arguments, and returns a reference to this arc.

    \param center [in]  Center of arc.
    \param normal [in]  A vector normal to the plane of the arc
    \param radius [in]  Radius of arc.
  */
   OdGeCircArc3d& set(
    const OdGePoint3d& center,
    const OdGeVector3d& normal,
    double radius);

  /** \details
    Sets the parameters for this arc according to the arguments, and returns a reference to this arc.

    \param center [in]  Center of arc.
    \param normal [in]  A vector normal to the plane of the arc
    \param radius [in]  Radius of arc.
    \param startAng [in]  Starting angle of arc.
    \param endAng [in]  Ending angle of arc.
    \param refVec [in]  The reference vector defining arc angle 0.
    
    \remarks
    To construct a circle, set endAng = startAng + Oda2PI

    \remarks
    All angles are expressed in radians.

    startAng must be less than endAng.
  */
   OdGeCircArc3d& set(
    const OdGePoint3d& center,
    const OdGeVector3d& normal,
    const OdGeVector3d& refVec,
    double radius,
    double startAng,
    double endAng);
  
  /** \details
    Sets the parameters for this arc according to the arguments, and returns a reference to this arc.

    \param startPoint [in]  Startpoint of arc.
    \param secondPoint [in]  Second point on a 3-point arc.
    \param endPoint [in]  Endpoint of arc.
  */
   OdGeCircArc3d& set(
    const OdGePoint3d& startPoint,
    const OdGePoint3d& secondPoint,
    const OdGePoint3d& endPoint);
  
  /** \details
    Sets the parameters for this arc according to the arguments, and returns a reference to this arc.

    \param startPoint [in]  Startpoint of arc.
    \param secondPoint [in]  Second point on a 3-point arc.
    \param endPoint [in]  Endpoint of arc.
    \param status [out]  Receives status of set().
  */
   OdGeCircArc3d& set(
    const OdGePoint3d& startPoint,
    const OdGePoint3d& secondPoint,
    const OdGePoint3d& endPoint,
    OdGeError& status);
  
  /** \details
    Sets the parameters for this arc according to the arguments, and returns a reference to this arc.

    \param radius [in]  Radius of arc.
    \param curve1 [in]  First curve to define a tangent arc.
    \param curve2 [in]  Second curve to define a tangent arc.
    \param param1 [in]  Parameter corresponding tangency point on curve1.
    \param param2 [in]  Parameter corresponding tangency point on curve2.
    \param success [out]  Receives true if and only if the tan-tan-radius or
    tan-tan-tan curve could be constructed. If false,
    this arc is unmodified.
  */
   OdGeCircArc3d& set(
    const OdGeCurve3d& curve1,
    const OdGeCurve3d& curve2,
    double radius,
    double& param1,
    double& param2,
    bool& success);
  
  /** \details
    Sets the parameters for this arc according to the arguments, and returns a reference to this arc.

    \param curve1 [in]  First curve to define a tangent arc.
    \param curve2 [in]  Second curve to define a tangent arc.
    \param curve3 [in]  Third curve to define a tangent arc.
    \param param1 [in]  Parameter corresponding tangency point on curve1.
    \param param2 [in]  Parameter corresponding tangency point on curve2.
    \param param3 [in]  Parameter corresponding tangency point on curve3.
    \param success [out]  Receives true if and only if the tan-tan-radius or
    tan-tan-tan curve could be constructed. If false,
    this arc is unmodified.
  */
   OdGeCircArc3d& set(
    const OdGeCurve3d& curve1,
    const OdGeCurve3d& curve2,
    const OdGeCurve3d& curve3,
    double& param1,
    double& param2,
    double& param3,
    bool& success);

  OdGeCircArc3d& operator =(
    const OdGeCircArc3d& arc);

  //////////////////////////////////////////////////////////////////////////

  /** \details
  Returns the geometric extents of this arc.

  \param extents [out]  Receives the geometric extents.
  */
   void getGeomExtents(
    OdGeExtents3d& extents) const;
};

#include "TD_PackPop.h"

#endif

