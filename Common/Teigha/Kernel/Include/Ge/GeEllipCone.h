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


#ifndef OD_GEELLIPCONE_H
#define OD_GEELLIPCONE_H /*!DOM*/

#include "Ge/GeSurface.h"
#include "Ge/GeInterval.h"
#include "OdPlatformSettings.h"

class OdGePoint3d;
class OdGeVector3d;
class OdGeCircArc3d;
class OdGeLinearEnt3d;

#include "TD_PackPush.h"

/** \details
    This class represents right elliptical cones.

    \remarks
    A elliptical cone is defined by its 
    * major and minor radii
    * origin (a point on the axis of symmetry)
    * axis of symmetry
    * major axis
    * height

    The "base" is the cross section which contains the origin.

    The major and minor radii must be greater than 0.  

    The cone may extend on either side of the base.

    The half angle is constrained to the interval (0, OdaPI/2),
    and is measured from the symmetric axis of this elliptical cone to a point on the major axis.

    A negative angle results in an apex which is opposite the direction of the
    symmetric axis.

    Parameter V is the revolution angle measured from the refAxis (which 
    is perpendicular to the axis of symmetry).  For a closed cone,
    it defaults to [-OdaPI, OdaPI).  The positive direction of V
    is defined by applying the right hand rule applied along the
    axis of symmetry.  The surface is periodic in V with a period of Oda2PI.

    The angle of point on an ellipse is measured by projecting
    the point on the ellipse perpendicular to major axis onto a
    circle whose center is the center of the ellipse and whose
    radius is the major radius of the ellipse.

    The angle between the major axis of the ellipse, and a vector from
    the center of the ellipse to the intersection point with the circle, 
    measured counterclockwise, is the angle of the point on the ellipse.

    Parameter U varies along the axis of symmetry.  U is dimensionless,
    and increases in the direction of the axis of symmetry. 
    U = 0 corresponds to center of the base of this cone, 
    and U = 1 corresponds to the apex of this cone. 

    [umin, umax] by [vmin, vmax] defines a conical patch with four sides
    bounded by two straight lines (at angles vmin and vmax), and two
    circular arcs (at umin and umax).  The following must be observed
    when defining a cone:

    * umin < umax
    * |vmax - vmin| <= Oda2PI
    * majorRadius > 0.0
    * minorRadius > 0.0

    The height interval of this elliptical cone is specified relative to its origin
    (note that the height increases in the direction of the symmetric
    axis).

    Corresponding C++ library: TD_Ge

    <group OdGe_Classes>

    \sa
    <link ge_OdGeCone.html, Working with Cones>
*/
class GE_TOOLKIT_EXPORT OdGeEllipCone : public OdGeSurface
{
public:


  /** \param cosineAngle [in]  The cosine of the angle between the generating line passing
    through the major axis, and the axis of symmetry.
    \param sineAngle [in] The sine of the angle between the generating line passing
    through the major axis, and the axis of symmetry.
    \param majorRadius [in] The major radius of this elliptical cone.
    \param minorRadius [in] The minor radius of this elliptical cone.
    \param baseOrigin [in] The origin of this elliptical cone.
    \param axisOfSymmetry [in] Axis of symmetry (rotation).
    \param majorAxis [in] The major axis of this elliptical cone.
    \param height [in] Height interval of this elliptical cone.
    \param startAng [in] Starting angle of this elliptical cone.
    \param endAng [in] Ending angle of this elliptical cone.
    \remarks
    All angles are expressed in radians.
  */
  OdGeEllipCone ();
  OdGeEllipCone(const OdGeEllipCone&);
  OdGeEllipCone (
    double cosineAngle, 
    double sineAngle,
    const  OdGePoint3d& origin, 
    double minorRadius,
    double majorRadius, 
    const OdGeVector3d& axisOfSymmetry);
  OdGeEllipCone (
    double cosineAngle, 
    double sineAngle,
    const OdGePoint3d& baseOrigin, 
    double minorRadius,
    double majorRadius,
    const OdGeVector3d& axisOfSymmetry,
    const OdGeVector3d& majorAxis, 
    const OdGeInterval& height,
    double startAng, 
    double endAng);

  /** \details
    Returns the ratio of the minor to the major radius of this elliptical cone.
  */
  double radiusRatio () const ;
  
  /** \details
    Returns the minor radius of this elliptical cone.
  */
  double minorRadius () const ;

  /** \details
    Returns the major radius of this elliptical cone.
  */
  double majorRadius () const  ;

  /** \details
    Returns the center of the base.
  */
  OdGePoint3d baseCenter () const  ;

  /** \details
    Returns the starting and ending angles of this elliptical cone.

    \param startAng [out]  Receives the start angle.
    \param endAng [out]  Receives the end angle.
    \remarks
    All angles are expressed in radians.

  */
  void getAngles (
    double& startAng, 
    double& endAng) const ;

  /** \details
    Returns the angle between the generating line passing through the major axis, and the axis of symmetry.
  */
  double halfAngle () const ;

  /** \details
    Returns the angle between the generating line passing through the major axis, and the axis of symmetry.
  */
  void getHalfAngle (
    double& cosineAngle, 
    double& sineAngle) const ;


  /** \details
    Returns the interval of the axis of symmetry.

    \param height [out]  Receives the interval of the axis of symmetry.
  */
  void getHeight(
    OdGeInterval& height) const ;

  /** \details
    Returns the height of this elliptical cone corresponding to
    the specified position on the U-axis.

    \remarks
    Parameter U varies along the axis of symmetry.  U is dimensionless,
    and increases in the direction of the axis of symmetry. 

    \param u [in]  Position on the U-axis of this elliptical cone.
  */
  double heightAt(
    double u) const  ;

  /** \details
    Returns the axis of symmetry of this elliptical cone.
  */
  OdGeVector3d axisOfSymmetry() const  ;

  /** \details
    Returns the major axis of this elliptical cone.
  */
  OdGeVector3d majorAxis() const  ;

  /** \details
    Returns the minor axis of this elliptical cone.
  */
  OdGeVector3d minorAxis() const ;

  /** \details
    Returns the apex of this elliptical cone.
  */
  OdGePoint3d apex() const ;

  /** \details
    Returns true if and only if the base of this elliptical cone
    is a full ellipse within the specified tolerance.

    \param tol [in]  Geometric tolerance.
  */
  bool isClosed(
    const OdGeTol& tol = OdGeContext::gTol) const ;

  /** \details
    Returns true if and only if the normal to this surface
    is pointing outward.
  */
  bool isOuterNormal() const ;

  /** \details
    Sets minor radius of this elliptical cone.

    \param minorRadius [in]  The minor radius of this elliptical cone.
  */
  OdGeEllipCone& setMinorRadius(
    double minorRadius) ;

  /** \details
    Sets major radius of this elliptical cone.

    \param majorRadius [in]  The major radius of this elliptical cone.
  */
  OdGeEllipCone& setMajorRadius(
    double majorRadius) ;


  /** \details
    Sets the starting and ending angles of this elliptical cone.

    \param startAng [in]  Start angle in radians.
    \param endAng [in]  End angle in radians.
  */
  OdGeEllipCone& setAngles(
    double startAng, 
    double endAng) ;

  /** \details
    Sets the height interval of this elliptical cone.

    \param height [in]  Height interval of this elliptical cone.
  */
  OdGeEllipCone& setHeight(
    const OdGeInterval& height) ;

  /** \details
    Sets the parameters for this elliptical cone according to the arguments, 
    and returns a reference to this elliptical cone.

    \param cosineAngle [in]  The cosine of the angle between the generating line passing
    through the major axis, and the axis of symmetry.
    \param sineAngle [in] The sine of the angle between the generating line passing
    through the major axis, and the axis of symmetry.
    \param majorRadius [in] The major radius of this elliptical cone.
    \param minorRadius [in] The minor radius of this elliptical cone.
    \param center [in] The origin of this elliptical cone.
    \param axisOfSymmetry [in] Axis of symmetry (rotation).
        
    \remarks
    All angles are expressed in radians.
  */
  OdGeEllipCone& set(
    double cosineAngle, 
    double sineAngle,
    const  OdGePoint3d& center,
    double minorRadius, 
    double majorRadius,
    const  OdGeVector3d& axisOfSymmetry) ;
  
  /** \details
    Sets the parameters for this elliptical cone according to the arguments, 
    and returns a reference to this elliptical cone.

    \param cosineAngle [in]  The cosine of the angle between the generating line passing
    through the major axis, and the axis of symmetry.
    \param sineAngle [in] The sine of the angle between the generating line passing
    through the major axis, and the axis of symmetry.
    \param majorRadius [in] The major radius of this elliptical cone.
    \param minorRadius [in] The minor radius of this elliptical cone.
    \param center [in] The origin of this elliptical cone.
    \param axisOfSymmetry [in] Axis of symmetry (rotation).
    \param majorAxis [in] The major axis of this elliptical cone.
    \param height [in] Height interval of this elliptical cone.
    \param startAng [in] Starting angle of this elliptical cone.
    \param endAng [in] Ending angle of this elliptical cone in.
    
    \remarks
    All angles are expressed in radians.
  */
  OdGeEllipCone& set(
    double cosineAngle, 
    double sineAngle,
    const  OdGePoint3d& center,
    double minorRadius, 
    double majorRadius,
    const  OdGeVector3d& axisOfSymmetry,
    const  OdGeVector3d& majorAxis,
    const  OdGeInterval& height,
    double startAng, 
    double endAng) ;

  OdGeEllipCone& operator =(const OdGeEllipCone& );

};

#include "TD_PackPop.h"

#endif // OD_GEELLIPCONE_H
