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

#ifndef OD_GEPLANE_H
#define OD_GEPLANE_H /*!DOM*/

#include "Ge/GePlanarEnt.h"
#include "TD_PackPush.h"

class OdGeBoundedPlane;
class OdGeLine3d;
class OdGeLineSeg3d;

/** \details
    This class represents infinite planes in 3D space.

    Corresponding C++ library: TD_Ge

    <group OdGe_Classes> 

    \sa
    <link ge_OdGePlane.html, Working with Planes>
*/
class GE_TOOLKIT_EXPORT OdGePlane : public OdGePlanarEnt
{
public:


  GE_STATIC_EXPORT static const OdGePlane kXYPlane; // XY *plane*.
  GE_STATIC_EXPORT static const OdGePlane kYZPlane; // YZ *plane*.
  GE_STATIC_EXPORT static const OdGePlane kZXPlane; // ZY *plane*.

  /** \param origin [in]   Origin of plane.
    \param normal [in]   The normal to the plane.
    \param uPnt [in]  A point at the end of the U-axis.
    \param vPnt [in]  A point at the end of the V-axis.
    \param uAxis [in]  The U-axis.
    \param vAxis [in]  The V-axis.
    \param a [in]  Coefficient a.
    \param b [in]  Coefficient b.
    \param c [in]  Coefficient c.
    \param d [in]  Coefficient d.

    \remarks
    A parametric point on the plane with parameters u and v maps to the point S(u,v) as follows

            S(u,v) = originOfPlanarEntity + (u * uAxis) + (v * vAxis)

    uAxis and vAxis need not be either normalized or perpendicular, but they must
    not be collinear.

    The orthonormal canonical coordinate system associated with a plane defined follows

    @untitled table
    origin        Origin of plane.                            originOfPlanarEntiity                 
    axis1           A unit vector in the plane.                 uAxis.normal()                        
    axis2           A unit vector perpendicular to the plane.   uAxis.crossProduct(vAxis).normal()   

    The plane equation for this plane is as follows

            a * X + b * Y + c * Z + d = 0
  */
  OdGePlane();
  OdGePlane(
    const OdGePlane& plane);
  OdGePlane(
    const OdGePoint3d& origin, 
    const OdGeVector3d& normal);
  OdGePlane(
    const OdGePoint3d& uPnt, 
    const OdGePoint3d& origin, 
    const OdGePoint3d& vPnt);
  OdGePlane(
    const OdGePoint3d& origin, 
    const OdGeVector3d& uAxis, 
    const OdGeVector3d& vAxis);
  OdGePlane(
    double a, 
    double b, 
    double c, 
    double d);

  /** \details
    Returns true and the intersection point or line, if and only 
    if the specified line or plane intersects with this plane.

    \param line [in]  Any 3D linear entity.
    \param plane [in]  Any plane.
    \param intLine [out]  Receives the intersection line.
    \param point [out]  Receives the intersection point. 
    \param tol [in]  Geometric tolerance.
  */
  TD_USING(OdGePlanarEnt::intersectWith);
   bool intersectWith(
    const OdGePlane& plane, 
    OdGeLine3d& intLine,
    const OdGeTol& tol = OdGeContext::gTol) const;
   bool intersectWith(
    const OdGeBoundedPlane& plane, 
    OdGeLineSeg3d& intLine,
    const OdGeTol& tol = OdGeContext::gTol) const;

  /** \details
    Returns the signed distance to (elevation of) the specified point.

    \param point [in]  Any 3D point.
  */
   double signedDistanceTo(
    const OdGePoint3d& point) const;

   /** \details
    Sets the parameters for this plane according to the arguments. 

    \param normal [in]   The normal to the plane.
  
    \returns
    Returns a reference to this plane.
\remarks
    A parametric point on the plane with parameters u and v maps to the point S(u,v) as follows

            S(u,v) = originOfPlanarEntity + (u * uAxis) + (v * vAxis)

    uAxis and vAxis need not be either normalized or perpendicular, but they must
    not be collinear.

    The orthonormal canonical coordinate system associated with a plane defined follows

    @untitled table
    origin        Origin of plane.                            originOfPlanarEntiity                 
    axis1           A unit vector in the plane.                 uAxis.normal()                        
    axis2           A unit vector perpendicular to the plane.   uAxis.crossProduct(vAxis).normal()   

    The plane equation for this plane is as follows

            a * X + b * Y + c * Z + d = 0
   */
   OdGePlane& set(
    const OdGePoint3d& point, 
    const OdGeVector3d& normal);
 
   /** \details
    Sets the parameters for this plane according to the arguments. 

    \param origin [in]   Origin of plane.
    \param uPnt [in]  A point at the end of the U-axis.
    \param vPnt [in]  A point at the end of the V-axis.
    
    \returns
    Returns a reference to this plane.
\remarks
    A parametric point on the plane with parameters u and v maps to the point S(u,v) as follows

            S(u,v) = originOfPlanarEntity + (u * uAxis) + (v * vAxis)

    uAxis and vAxis need not be either normalized or perpendicular, but they must
    not be collinear.

    The orthonormal canonical coordinate system associated with a plane defined follows

    @untitled table
    origin        Origin of plane.                            originOfPlanarEntiity                 
    axis1           A unit vector in the plane.                 uAxis.normal()                        
    axis2           A unit vector perpendicular to the plane.   uAxis.crossProduct(vAxis).normal()   

    The plane equation for this plane is as follows

            a * X + b * Y + c * Z + d = 0
   */
  OdGePlane& set(
    const OdGePoint3d& uPnt, 
    const OdGePoint3d& origin, 
    const OdGePoint3d& vPnt);

   /** \details
    Sets the parameters for this plane according to the arguments. 

    \param a [in]  Coefficient a.
    \param b [in]  Coefficient b.
    \param c [in]  Coefficient c.
    \param d [in]  Coefficient d.

    \returns
    Returns a reference to this plane.
\remarks
    A parametric point on the plane with parameters u and v maps to the point S(u,v) as follows

            S(u,v) = originOfPlanarEntity + (u * uAxis) + (v * vAxis)

    uAxis and vAxis need not be either normalized or perpendicular, but they must
    not be collinear.

    The orthonormal canonical coordinate system associated with a plane defined follows

    @untitled table
    origin        Origin of plane.                            originOfPlanarEntiity                 
    axis1           A unit vector in the plane.                 uAxis.normal()                        
    axis2           A unit vector perpendicular to the plane.   uAxis.crossProduct(vAxis).normal()   

    The plane equation for this plane is as follows

            a * X + b * Y + c * Z + d = 0
   */
  OdGePlane& set(
    double a, 
    double b, 
    double c, 
    double d);

   /** \details
    Sets the parameters for this plane according to the arguments. 

    \param origin [in]   Origin of plane.
    \param uAxis [in]  The U-axis.
    \param vAxis [in]  The V-axis.

    \returns
    Returns a reference to this plane.
\remarks
    A parametric point on the plane with parameters u and v maps to the point S(u,v) as follows

            S(u,v) = originOfPlanarEntity + (u * uAxis) + (v * vAxis)

    uAxis and vAxis need not be either normalized or perpendicular, but they must
    not be collinear.

    The orthonormal canonical coordinate system associated with a plane defined follows

    @untitled table
    origin        Origin of plane.                            originOfPlanarEntiity                 
    axis1           A unit vector in the plane.                 uAxis.normal()                        
    axis2           A unit vector perpendicular to the plane.   uAxis.crossProduct(vAxis).normal()   

    The plane equation for this plane is as follows

            a * X + b * Y + c * Z + d = 0
   */
  OdGePlane& set(
    const OdGePoint3d& origin, 
    const OdGeVector3d& uAxis, 
    const OdGeVector3d& vAxis);

  OdGePlane& operator =(
    const OdGePlane& plane);

private:
  OdGePlane(OdGeEntity3dImpl*);
};

GE_TOOLKIT_EXPORT void setPlaneEnvelope( OdGePlane* pPlane, const OdGeInterval& intrvlU, const OdGeInterval& intrvlV );

#include "TD_PackPop.h"

#endif // OD_GEPLANE_H

