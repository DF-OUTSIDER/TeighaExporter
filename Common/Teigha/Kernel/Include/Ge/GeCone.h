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

#ifndef OD_GECONE_H
#define OD_GECONE_H /*!DOM*/

#include "OdPlatformSettings.h"
#include "Ge/GeSurface.h"
#include "Ge/GeInterval.h"

class OdGePoint3d;
class OdGeVector3d;
class OdGeCircArc3d;
class OdGeLinearEnt3d;

#include "TD_PackPush.h"

/** \details
    This class represents cones with a circular base.

    \remarks
    A circular cone is defined by:

    * axis of symmetry
    * height
    * half angle (the angle between the generating line and the axis of symmetry)
    * origin (a point on the axis of symmetry)
    * radius at the origin 

    The cross section containing the origin is refered to as the base. 
    The radius at the base cannot be zero.  
    This cone may extend on either side of the base.

    The half angle is constrained to the interval (-OdaPI/2, OdaPI/2),
    and is measured from the symmetric axis of the cone.
    A negative angle results in an apex on the opposite direction of the
    symmetric axis.

    Parameter V is the angle of revolution measured from the refAxis
    (an axis perpendicular to the axis of symmetry). For a closed cone,
    it defaults to [-OdaPI, OdaPI). The positive V direction is defined 
    by applying the right hand rule along the axis of symmetry.  
    The surface is periodic in V with a period of Oda2PI.

    Parameter U varies along the axis of symmetry. U is dimensionless
    and increases in the direction of the axis of symmetry. 
    U = 0 corresponds to center of the cone base, 
    and U = 1 corresponds to the cone apex. 

    [umin, umax] x [vmin, vmax] defines a four sided conical patch
    bounded by two straight lines (at angles vmin and vmax), and two
    circular arcs (at umin and umax). The following must be observed
    when defining a cone:

    * umin < umax
    * |vmax - vmin| <= Oda2PI.
    * baseRadius > 0.0

    The height interval of this cone is specified relative to its origin
    (with the height increasing in the direction of the symmetric
    axis).
    
    Corresponding C++ library: TD_Ge

    <group OdGe_Classes> 

    \sa
    <link ge_OdGeCone.html, Working with Cones>
*/
class GE_TOOLKIT_EXPORT OdGeCone : public OdGeSurface
{
public:


  /** \param cosineAngle [in]  The cosine of the angle between the generating line and the axis of symmetry.
    \param sineAngle [in]  The sine of the angle between the generating line and the axis of symmetry.
    \param baseOrigin [in]  Center of the base.
    \param baseRadius [in]  Radius of the base. 
    \param axisOfSymmetry [in]  Axis of symmetry (rotation).
    \param refAxis [in]  defines thegle 0 of circular base.
    \param height [in]  Height interval of the cone.
    \param startAng [in]  Start angle.
    \param endAng [in]  End angle.

    \remarks
    The default constructor uses a half angle of 45°,
    a reference axis of (1,0,0), a baseOrigin of (0,0,0), and a base radius of 2.0.
    \remarks
    All angles are expressed in radians.

  */
  OdGeCone();
  OdGeCone(
    double cosineAngle, 
    double sineAngle,
    const  OdGePoint3d& baseOrigin, 
    double baseRadius,
    const  OdGeVector3d& axisOfSymmetry);
  OdGeCone(
    double cosineAngle, 
    double sineAngle,
    const  OdGePoint3d& baseOrigin, 
    double baseRadius,
    const  OdGeVector3d& axisOfSymmetry,
    const  OdGeVector3d& refAxis, 
    const  OdGeInterval& height,
    double startAng, 
    double endAng);
  OdGeCone(const OdGeCone& cone);

  /** \details
  Returns the base radius of this cone.
  */
  double baseRadius() const;

  /** \details
  Returns the center of the base.
  */
  OdGePoint3d baseCenter() const;

  /** \details
  Returns the starting and ending angles of this cone.

  \param startAng [out]  Receives the start angle.
  \param endAng [out]  Receives the end angle.
  \remarks
  All angles are expressed in radians.

  */
  void getAngles(
    double& startAng, 
    double& endAng) const;

  /** \details
    Returns the angle between the generating line and the axis of symmetry.
  */
  double halfAngle() const;

  /** \details
    Returns the cosine and the sine of the angle between the generating line and the axis of symmetry.

    \param cosineAngle [out]  Receives the cosine of the angle.
    \param sineAngle [out]  Receives the sine of the angle.
  */
  void getHalfAngle(
    double& cosineAngle, 
    double& sineAngle) const;

  /** \details
    Returns the interval of the axis of symmetry.

    \param height [out]  Receives the interval.
  */
  void getHeight(
    OdGeInterval& height) const;

  /** \details
    Returns the height of this cone corresponding to
    the specified position on the U-axis.

    \remarks
    Parameter U varies along the axis of symmetry.  U is dimensionless,
    and increases in the direction of the axis of symmetry. 

    \param u [in]  Position on the U-axis of this cone.

  */
  double heightAt(
    double u) const;

  /** \details
    Returns the axis of symmetry of this cone.
  */
  OdGeVector3d axisOfSymmetry() const;

  /** \details
    Returns the reference axis of this cone.
  */
  OdGeVector3d refAxis () const;

  /** \details
    Returns the apex of this cone.
  */
  OdGePoint3d apex() const;

  /** \details
    Returns true if and only if the base of this cone
    is a full circle within the specified tolerance.

    \param tol [in]  Geometric tolerance.
  */
  bool isClosed(
    const OdGeTol& tol = OdGeContext::gTol) const;

  /** \details
  Returns true if and only if the normal to this surface
  is pointing outward.
  */
  bool isOuterNormal() const;

  /** \details
    Sets the base radius of this cone.

    \param baseRadius [in]  Radius of the base. 
  */
  OdGeCone& setBaseRadius(
    double baseRadius);

  /** \details
    Sets the starting and ending angles of this cone.

    \param startAng [in]  Start angle.
    \param endAng [in]  End angle.
    \remarks
    All angles are expressed in radians.

  */
  OdGeCone& setAngles(
    double startAng, 
    double endAng);

  /** \details
    Sets the height interval of this cone.

    \param height [in]  Height interval of this cone.
  */
  OdGeCone& setHeight(
    const OdGeInterval& height);

  /** \details
    Sets the parameters for this cone according to the arguments, 
    and returns a reference to this cone.

    \param cosineAngle [in]  The cosine of the angle between the generating line and the axis of symmetry.
    \param sineAngle [in]  The sine of the angle between the generating line and the axis of symmetry.
    \param baseCenter [in]  Center of the base.
    \param baseRadius [in]  Radius of the base. 
    \param axisOfSymmetry [in]  Axis of symmetry (rotation).
  
    \remarks
    All angles are expressed in radians.
  */
  OdGeCone& set(
    double cosineAngle, 
    double sineAngle,
    const OdGePoint3d& baseCenter,
    double baseRadius,
    const OdGeVector3d& axisOfSymmetry);
 
  /** \details
    Sets the parameters for this cone according to the arguments, 
    and returns a reference to this cone.

    \param cosineAngle [in]  The cosine of the angle between the generating line and the axis of symmetry.
    \param sineAngle [in]  The sine of the angle between the generating line and the axis of symmetry.
    \param baseCenter [in]  Center of the base.
    \param baseRadius [in]  Radius of the base. 
    \param axisOfSymmetry [in]  Axis of symmetry (rotation).
    \param refAxis [in]  defines thegle 0 of circular base.
    \param height [in]  Height interval of this cone.
    \param startAng [in]  Start angle.
    \param endAng [in]  End angle.
    
    \remarks
    All angles are expressed in radians.
  */
 OdGeCone& set(
    double cosineAngle, 
    double sineAngle,
    const OdGePoint3d& baseCenter,
    double baseRadius,
    const OdGeVector3d& axisOfSymmetry,
    const OdGeVector3d& refAxis,
    const OdGeInterval& height,
    double startAng, 
    double endAng);
  
  OdGeCone& operator =(const OdGeCone& cone);

  /** \details
    Computes the intersection of this object with a linear entity.
  */
  bool intersectWith(
    const OdGeLinearEnt3d& linEnt, 
    int& numInt,
    OdGePoint3d& p1,
    OdGePoint3d& p2,
    const OdGeTol& tol = OdGeContext::gTol) const;
};

#include "TD_PackPop.h"

#endif // OD_GECONE_H

