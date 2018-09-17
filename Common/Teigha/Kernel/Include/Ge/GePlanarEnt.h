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

#ifndef OD_GEPLANAR_H
#define OD_GEPLANAR_H /*!DOM*/

#include "Ge/GeSurface.h"
#include "Ge/GeInterval.h"
#include "OdPlatformSettings.h"

class OdGeLinearEnt3d;

#include "TD_PackPush.h"

/** \details
    This class is the base class for all OdGe planes in 3D space.

    \remarks
    A parametric point on the plane with parameters u and v maps to the point S(u,v) as follows

              S(u,v) = originOfPlanarEntity + (u * uAxis) + (v * vAxis)

    uAxis and vAxis need not be either normalized or perpendicular, but they must
    not be colinear.

    <table>
    Parameter       Description                                   Computed as
    origin        Origin of plane.                            origin              
    axis1           A unit vector in the plane.                 uAxis.normal()                        
    axis2           A unit vector perpendicular to the plane.   uAxis.crossProduct(vAxis).normal()   
    </table>

    The plane equation for a plane is as follows

              a * X + b * Y + c * Z + d = 0

    Corresponding C++ library: TD_Ge

    <group OdGe_Classes> 
*/
class GE_TOOLKIT_EXPORT OdGePlanarEnt : public OdGeSurface
{
public:


  /** \details
    Returns true and the intersection with the specified linear entity,
    if and only if the specified linear entity intersects with this plane.

    \param line [in]  Any 3D linear entity.
    \param point [out]  Receives the point of intersection. 
    \param tol [in]  Geometric tolerance.
  */
   bool intersectWith(
    const OdGeLinearEnt3d& line, 
    OdGePoint3d& point,
    const OdGeTol& tol = OdGeContext::gTol) const;

  /** \details
    Returns the point on this plane that is closest to the specified linear entity, 
	and the point on the linear entity that is closest to this plane.

    \param line [in]  Any 3D linear entity.
    \param pointOnLine [out]  Receives the closest point on the linear entity.
    \param tol [in]  Geometric tolerance.
  */
   OdGePoint3d closestPointToLinearEnt(
    const OdGeLinearEnt3d& line,
    OdGePoint3d& pointOnLine, 
    const OdGeTol& tol = OdGeContext::gTol) const;

  /** \details
    Returns the point on this plane that is closest to the specified plane, and the point
    on the specified plane that is closest to this plane.

    \param plane [in]  Any plane.
    \param pointOnOtherPlane [out]  Receives the closest point on the plane.
    \param tol [in]  Geometric tolerance.
  */
   OdGePoint3d closestPointToPlanarEnt(
    const OdGePlanarEnt& plane,
    OdGePoint3d& pointOnOtherPlane, 
    const OdGeTol& tol = OdGeContext::gTol) const;

  /** \details
    Returns true if and only if the specified entity is parallel to this one.

    \param line [in]  Any 3D linear entity.
    \param tol [in]  Geometric tolerance.
  */
   bool isParallelTo(
    const OdGeLinearEnt3d& line, 
    const OdGeTol& tol = OdGeContext::gTol) const;

  /** \details
    Returns true if and only if the specified entity is parallel to this one.

    \param plane [in]  Any plane.
    \param tol [in]  Geometric tolerance.
  */
   bool isParallelTo(
    const OdGePlanarEnt& plane,
    const OdGeTol& tol = OdGeContext::gTol) const;

  /** \details
    Returns true if and only if the specified entity is perpendicular to this one.

    \param line [in]  Any 3D linear entity.
    \param tol [in]  Geometric tolerance.
  */
   bool isPerpendicularTo(
    const OdGeLinearEnt3d& line,
    const OdGeTol& tol = OdGeContext::gTol) const;

  /** \details
    Returns true if and only if the specified entity is perpendicular to this one.

    \param plane [in]  Any plane.
    \param tol [in]  Geometric tolerance.
  */
  bool isPerpendicularTo(
    const OdGePlanarEnt& plane,
    const OdGeTol& tol = OdGeContext::gTol) const;

  /** \details
    Returns true if and only 
    the specified plane is colinear with this one.

    \param plane [in]  Any plane.
    \param tol [in]  Geometric tolerance.
  */
   bool isCoplanarTo(
    const OdGePlanarEnt& plane,
    const OdGeTol& tol = OdGeContext::gTol) const;

  /** \details
    Returns the parameters of this plane.

    \param origin [in]  The origin of this plane.
    \param uAxis [in]  The U-axis.
    \param vAxis [in]  The V-axis.

    \remarks
    The U-axis and V-axis cannot be colinear, and are defined as follows

            uAxis=uPnt-origin
            vAxis=vPnt-origin
  */
   void get(
    OdGePoint3d& origin, 
    OdGeVector3d& uAxis, 
    OdGeVector3d& vAxis) const;
  
  /** \details
    Returns the parameters of this plane.

    \param origin [in]  The origin of this plane.
    \param uPnt [in]  A point at the end of the U-axis.
    \param vPnt [in]  A point at the end of the V-axis.

    \remarks
    The U-axis and V-axis cannot be colinear, and are defined as follows

            uAxis=uPnt-origin
            vAxis=vPnt-origin
  */
    void get(
    OdGePoint3d& uPnt, 
    OdGePoint3d& origin, 
    OdGePoint3d& vPnt) const;

  /** \details
    Returns an arbitrary point on the plane.
  */
   OdGePoint3d pointOnPlane() const;

  /** \details
    Returns the normal to the plane as a unit vector. 
  */
   OdGeVector3d normal() const;

  /** \details
    Returns the coefficients of the plane equation for this plane.

    \param a [out]  Receives the coefficient a.
    \param b [out]  Receives the coefficient b.
    \param c [out]  Receives the coefficient c.
    \param d [out]  Receives the coefficient d.

    \remarks
    The plane equation for this plane is as follows

            a * x + b * y + c * z + d = 0
  */
   void getCoefficients(
    double& a, 
    double& b, 
    double& c, 
    double& d) const;

  /** \details
    Returns the orthonormal canonical coordinate system of this plane.

    \param origin [out]  Receives the origin of this plane
    \param axis1 [out]  Receives a unit vector in the plane.
    \param axis2 [out]  Receives a unit vector perpendicular to the plane.

    \remarks
    The orthonormal canonical coordinate system associated with a plane defined follows

    <table>
    Parameter       Description                                           Computed as
    origin        Origin of plane.                                    origin              
    axis1           A unit vector in the plane.                         uAxis.normal()                        
    axis2           A unit vector in the plane perpendicular to axis1.  normal.crossProduct(axis1)   
    </table>
  */
   void getCoordSystem(
    OdGePoint3d& origin, 
    OdGeVector3d& axis1, 
    OdGeVector3d& axis2) const;

  OdGePlanarEnt& operator =(
    const OdGePlanarEnt& plane);

  //////////////////////////////////////////////////////////////////////////
  // TD Special :

  /** \details
    Returns projP and true,
    if and only if there is a point on this surface, projP, where
    the this surface normal or unitDir (if specified) passes through the point p.

    \param p [in]  Any 3D point.
    \param projP [out]  Receives the point on the plane.
    \param unitDir [in]  Unit vector specifying the projection direction.
    \param tol [in]  Geometric tolerance.
  */
  TD_USING(OdGeSurface::project);
   bool project(
    const OdGePoint3d& p, 
    const OdGeVector3d& unitDir, 
    OdGePoint3d& projP, 
    const OdGeTol& tol = OdGeContext::gTol) const;

  //////////////////////////////////////////////////////////////////////////

protected:
  OdGePlanarEnt();
  OdGePlanarEnt(const OdGePlanarEnt& plane);
};

#include "TD_PackPop.h"

#endif // OD_GEPLANAR_H

