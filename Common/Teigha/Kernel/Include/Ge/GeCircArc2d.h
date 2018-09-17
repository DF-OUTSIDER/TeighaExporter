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

#ifndef OD_GE_ARC2D_H
#define OD_GE_ARC2D_H /*!DOM*/

class OdGeLine2d;
class OdGeLinearEnt2d;
class OdGeExtents2d;


#include "Ge/GeCurve2d.h"

#include "TD_PackPush.h"

/** \details
    This class represents arcs and full circles in 2D space.

    Corresponding C++ library: TD_Ge

    <group OdGe_Classes> 
    
    \sa
    <link ge_OdGeCircArc3d.html, Working with Circular Arcs>
*/
class GE_TOOLKIT_EXPORT OdGeCircArc2d : public OdGeCurve2d
{
public:
  

  /** \param center [in]  Center of arc.
    \param radius [in]  Radius of arc.
    \param startAng [in]  Starting angle of arc.
    \param endAng [in]  Ending angle of arc.
    \param refVec [in]  The reference vector defining arc angle 0.
    \param startPoint [in]  Startpoint of arc.
    \param secondPoint [in]  Second point on a 3-point arc.
    \param endPoint [in]  Endpoint of arc.
    \param bulge [in]  Specifies the bulge of the arc.
    \param bulgeFlag [in]  Specifies how bulge is to be interpreted.
    \param source [in]  Object to be cloned.

    \remarks
    The default constructor creates a circle with a center of (0,0) and a radius of 1.
    
    To construct a circle, set endAng = startAng + Oda2PI

    If bulgeFlag == true, then bulge is the maximum distance from the arc perpendicular to a
    chord between the start and endpoints.

    If bulgeFlag == false, the bulge is the tangent of 1/4 the included 
    angle of the arc, measured counterclockwise.

    \remarks
    All angles are expressed in radians.
        
    startAng must be less than endAng. 

  */
  OdGeCircArc2d();
  OdGeCircArc2d(
    const OdGeCircArc2d& source);
  OdGeCircArc2d(
    const OdGePoint2d& center, 
    double radius);
  OdGeCircArc2d(
    const OdGePoint2d& center, 
    double radius, 
    double startAng,
    double endAng, 
    const OdGeVector2d& refVec = OdGeVector2d::kXAxis,
    bool isClockWise = false);
  OdGeCircArc2d(
    const OdGePoint2d& startPoint,
    const OdGePoint2d& secondPoint,
    const OdGePoint2d& endPoint);
  OdGeCircArc2d(
    const OdGePoint2d& startPoint, 
    const OdGePoint2d& endPoint, 
    double bulge, 
    bool bulgeFlag = true);

  /** \details
    Returns True if the specified linear entity intersects the arc entity,
	and returns the number of intersections and points of intersection.

    \param line [in]  Any 2D linear entity.
    \param numInt [out]  Receives the number of intersections with this curve.
    \param p1 [out]  Receives the first intersection point.
    \param p2 [out]  Receives the second intersection point.
    \param tol [in]  Geometric tolerance.

    \remarks
    * p1 has meaning only if numInt > 0. 
    * p2 has meaning only if numInt > 1.   
  */
  bool intersectWith(
    const OdGeLinearEnt2d& line, 
    int& numInt,
    OdGePoint2d& p1, 
    OdGePoint2d& p2, 
    const OdGeTol& tol = OdGeContext::gTol) const;
 
  /** \details
    Returns True if the specified arc entity intersects the arc entity,
	and returns the number of intersections and points of intersection.

    \param circarc [in]  Any 2D arc entity.
    \param numInt [out]  Receives the number of intersections with this curve.
    \param p1 [out]  Receives the first intersection point.
    \param p2 [out]  Receives the second intersection point.
    \param tol [in]  Geometric tolerance.

    \remarks
    * p1 has meaning only if numInt > 0. 
    * p2 has meaning only if numInt > 1.   
  */
  bool intersectWith(
    const OdGeCircArc2d& circarc, 
    int& numInt,
    OdGePoint2d& p1, 
    OdGePoint2d& p2, 
    const OdGeTol& tol = OdGeContext::gTol) const;

  /** \details
    Returns true if and only if the specified point is
    on the full circle of this arc, the tangent
    at that point.

    \param point [in]  The point on the full circle.
    \param line [out]  Receives the tangent at that point.
    \param tol [in]  Geometric tolerance.
  */
  bool tangent(
    const OdGePoint2d& point, 
    OdGeLine2d& line,
    const OdGeTol& tol = OdGeContext::gTol) const;
  
  /** \details
    Returns true if and only if the specified point is
    on the full circle of this arc, the tangent
    at that point, and the status of the query.

    \param point [in]  The point on the full circle.
    \param line [out]  Receives the tangent at that point.
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
    const OdGePoint2d& point, 
    OdGeLine2d& line,
    const OdGeTol& tol, 
    OdGeError& status) const;

  /** \details
    Returns true if and only if the input point lies within
    the full circle of this arc.

    \param point [in]  Any 2D point.
    \param tol [in]  Geometric tolerance.
  */
  bool isInside(
    const OdGePoint2d& point, 
    const OdGeTol& tol = OdGeContext::gTol) const;

  /** \details
    Returns the center of this arc.
  */
  OdGePoint2d center() const;

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
    Returns true if and only if this arc is drawn clockwise from start point to end point.
  */
  bool isClockWise() const;

  /** \details
    Returns the reference vector as a unit vector.
  */
  OdGeVector2d refVec() const;

  /** \details
    Returns the start point of this arc.
  */
  OdGePoint2d startPoint() const;

  /** \details
    Returns the end point of this arc.
  */
  OdGePoint2d endPoint() const;

  /** \details
    Sets the center of this arc, and returns a reference to this arc.

    \param center [in]  Center of arc.
  */
  OdGeCircArc2d& setCenter(
    const OdGePoint2d& center);

  /** \details
    Sets the radius of this arc, and returns a reference to this arc.

    \param radius [in]  Radius of arc.
  */
  OdGeCircArc2d& setRadius(
    double radius);

  /** \details
    Sets the starting and ending angles of this arc, and returns a reference to this arc.

    \param startAng [in]  Starting angle of arc.
    \param endAng [in]  Ending angle of arc.
    \remarks
    All angles are expressed in radians.

  */
  OdGeCircArc2d& setAngles(
    double startAng, 
    double endAng);

  /** \details
    Reverses the direction of this arc while maintaining its endpoints, and returns a reference to this arc.
  */
  OdGeCircArc2d& setToComplement();

  /** \details
    Sets the reference vector of this arc, and returns a reference to this arc.

    \param refVec [in]  The reference vector defining arc angle 0.
  */
  OdGeCircArc2d& setRefVec(
    const OdGeVector2d& vect);

  /** \details
    Sets the parameters for this arc according to the arguments, and returns a reference to this arc.

    \param center [in]  Center of arc.
    \param radius [in]  Radius of arc.
  */
  OdGeCircArc2d& set(
    const OdGePoint2d& center, 
    double radius);
  
  /** \details
    Sets the parameters for this arc according to the arguments, and returns a reference to this arc.

    \param center [in]  Center of arc.
    \param radius [in]  Radius of arc.
    \param startAng [in]  Starting angle of arc.
    \param endAng [in]  Ending angle of arc.
    \param refVec [in]  The reference vector defining arc angle 0.
    \param isClockWise [in]  If true, the arc is drawn clockwise, otherwise, counterclockwise.
    
    \remarks
    To construct a circle, set endAng = startAng + Oda2PI

    \remarks
    startAng must be less than endAng. 
    All angles are expressed in radians.
  */
  OdGeCircArc2d& set(
    const OdGePoint2d& center, 
    double radius, 
    double startAng, 
    double endAng,
    const OdGeVector2d& refVec = OdGeVector2d::kXAxis, 
    bool isClockWise = false);
  
  /** \details
    Sets the parameters for this arc according to the arguments, and returns a reference to this arc.

    \param startPoint [in]  Startpoint of arc.
    \param secondPoint [in]  Second point on a 3-point arc.
    \param endPoint [in]  Endpoint of arc.
  */
  OdGeCircArc2d& set(
    const OdGePoint2d& startPoint, 
    const OdGePoint2d& secondPoint,
    const OdGePoint2d& endPoint);
  
  /** \details
    Sets the parameters for this arc according to the arguments, and returns a reference to this arc.

    \param status [out]  Receives the status for this method.
    \param startPoint [in]  Startpoint of arc.
    \param secondPoint [in]  Second point on a 3-point arc.
    \param endPoint [in]  Endpoint of arc.
    
    \remarks
    Possible values for status are as follows

    @untitled table
    kEqualArg1Arg2
    kEqualArg1Arg3
    kEqualArg2Arg3
    kLinearlyDependentArg1Arg2Arg3.
  */
  OdGeCircArc2d& set(
    const OdGePoint2d& startPoint,
    const OdGePoint2d& secondPoint,
    const OdGePoint2d& endPoint, 
    OdGeError& status);
  
  /** \details
    Sets the parameters for this arc according to the arguments, and returns a reference to this arc.

    \param startPoint [in]  Startpoint of arc.
    \param endPoint [in]  Endpoint of arc.
    \param bulge [in]  Specifies the bulge of the arc.
    \param bulgeFlag [in]  Specifies how bulge is to be interpreted.
  
    \remarks
    If bulgeFlag == true, then bulge is the maximum distance from the arc to a
    chord between the start and endpoints.

    If bulgeFlag == false, the bulge is the tangent of 1/4 the included angle of the arc.
  */
  OdGeCircArc2d& set(
    const OdGePoint2d& startPoint, 
    const OdGePoint2d& endPoint, 
    double bulge, 
    bool bulgeFlag = true);
  
  /** \details
    Sets the parameters for this arc according to the arguments, and returns a reference to this arc.

    \param radius [in]  Radius of arc.
    \param curve1 [in]  First curve to define a tangent arc.
    \param curve2 [in]  Second curve to define a tangent arc.
    \param param1 [out]  Receives the parameter corresponding tangency point on curve1.
    \param param2 [out]  Receives the parameter corresponding tangency point on curve2.
    \param success [out]  Receives true if and only if the tan-tan-radius or
    tan-tan-tan curve could be constructed. If false,
    this arc is unmodified.
  */
  OdGeCircArc2d& set(
    const OdGeCurve2d& curve1, 
    const OdGeCurve2d& curve2,
    double radius, 
    double& param1, 
    double& param2, 
    bool& success);
  
  /** \details
    Sets the parameters for this arc according to the arguments, and returns a reference to this arc.

    \param curve1 [in]  First curve to define a tangent arc.
    \param curve2 [in]  Second curve to define a tangent arc.
    \param curve3 [in]  Third curve to define a tangent arc.
    \param param1 [out]  Receives the parameter corresponding tangency point on curve1.
    \param param2 [out]  Receives the parameter corresponding tangency point on curve2.
    \param param3 [out]  Receives the parameter corresponding tangency point on curve3.
    \param success [out]  Receives true if and only if the tan-tan-radius or
    tan-tan-tan curve could be constructed. If false,
    this arc is unmodified.
  */
  OdGeCircArc2d& set(
    const OdGeCurve2d& curve1,
    const OdGeCurve2d& curve2, 
    const OdGeCurve2d& curve3,
    double& param1, 
    double& param2, 
    double& param3, 
    bool& success);

  OdGeCircArc2d& operator =(const OdGeCircArc2d& arc);

  /** \details
    Returns the geometric extents of this arc.

    \param extents [out]  Receives the geometric extents.
  */
  void getGeomExtents(
    OdGeExtents2d& extents) const;

  /** \details
    Returns the starting angle measured from the X-axis in the arc direction.
	    
    \remarks
    All angles are expressed in radians.
  */
  double startAngFromXAxis() const;

  /** \details
    Returns the ending angle measured from the X-axis in the arc direction.
	
    \remarks
    All angles are expressed in radians.
  */
  double endAngFromXAxis() const;
};

#include "TD_PackPop.h"

#endif // OD_GE_ARC2D_H

