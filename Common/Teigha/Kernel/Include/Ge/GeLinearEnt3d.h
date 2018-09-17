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

#ifndef OD_GELENT3D_H
#define OD_GELENT3D_H /*!DOM*/

class OdGeLine3d;
class OdGeCircArc3d;
class OdGePlanarEnt;

#include "OdPlatform.h"
#include "Ge/GeCurve3d.h"

#include "TD_PackPush.h"

/** \details
    This class is the base class for OdGe 3D linear entities.

    Corresponding C++ library: TD_Ge

    <group OdGe_Classes> 
*/
class GE_TOOLKIT_EXPORT OdGeLinearEnt3d : public OdGeCurve3d
{
public:

  ///////////////////////////////////////////////////////////////////////////

  /** \details
    Returns true and the intersection point, if and only 
    if the specified line intersects with this line.

    \param line [in]  Any 3D linear entity.
    \param intPt [out]  Receives the intesection point.
    \param tol [in]  Geometric tolerance.
  */
   bool intersectWith(
    const OdGeLinearEnt3d& line,
    OdGePoint3d& intPt,
    const OdGeTol& tol = OdGeContext::gTol) const;
  
  /** \details
    Returns true and the intersection point, if and only 
    if the specified plane intersects with this line.

    \param plane [in]  Any planar entity.
    \param intPnt [out]  Receives the intesection point.
    \param tol [in]  Geometric tolerance.
  */
  bool intersectWith(
    const OdGePlanarEnt& plane, 
    OdGePoint3d& intPnt,
    const OdGeTol& tol = OdGeContext::gTol) const;

  /** \details
    Returns the projected intersection of the specified line with this line.

    \param line [in]  Any 3D line.
    \param projDir [in]  Projection direction.
    \param pntOnThisLine [out]  Receives the intersection point on this line.   
    \param pntOnOtherLine [out]  Receives the intersection point on the other line. 
    \param tol [in]  Geometric tolerance.

    \remarks
    By default this function throws exception "not Implemented".
  */
   bool projIntersectWith(
    const OdGeLinearEnt3d& line,
    const OdGeVector3d& projDir,
    OdGePoint3d& pntOnThisLine,
    OdGePoint3d& pntOnOtherLine,
    const OdGeTol& tol = OdGeContext::gTol) const;

   /** \details
    Determines if two lines overlap and if so returns 
    the line that coincides with their region of overlap. 

    \param line [in]  Any 3D linear entity.
    \param overlap [out]  Output linear entity that is coincident with region of overlap  
    \param tol [in]  Input tolerance.

    \remarks
    Currently not implemented.
   */
   bool overlap(
    const OdGeLinearEnt3d& line,
    OdGeLinearEnt3d*& overlap,
    const OdGeTol& tol = OdGeContext::gTol) const;

  TD_USING(OdGeCurve3d::isOn);
   bool isOn(
    const OdGePlane& plane,
    const OdGeTol& tol = OdGeContext::gTol) const;

  /** \details
    Returns true if and only 
    if the specified entity is parallel to this line.

    \param line [in]  Any 3D linear entity.
    \param tol [in]  Geometric tolerance.
  */
   bool isParallelTo(
    const OdGeLinearEnt3d& line,
    const OdGeTol& tol = OdGeContext::gTol) const;

  /** \details
    Returns true if and only 
    if the specified entity is parallel to this line.

    \param plane [in]  Any plane.
    \param tol [in]  Geometric tolerance.
  */
    bool isParallelTo(
    const OdGePlanarEnt& plane,
    const OdGeTol& tol = OdGeContext::gTol) const;

  /** \details
    Returns true if and only 
    if the specified entity is perpendicular to this line.

    \param line [in]  Any 3D linear entity.
    \param tol [in]  Geometric tolerance.
  */
   bool isPerpendicularTo(
    const OdGeLinearEnt3d& line,
    const OdGeTol& tol = OdGeContext::gTol) const;

  /** \details
    Returns true if and only 
    if the specified entity is perpendicular to this line.

    \param plane [in]  Any plane.
    \param tol [in]  Geometric tolerance.
  */
    bool isPerpendicularTo(
    const OdGePlanarEnt& plane,
    const OdGeTol& tol = OdGeContext::gTol) const;

  /** \details
    Returns true if and only if the specified line is colinear with this one. 

    \param line [in]  Any 3D linear entity.
    \param tol [in]  Geometric tolerance.
  */
   bool isColinearTo(
    const OdGeLinearEnt3d& line,
    const OdGeTol& tol = OdGeContext::gTol) const;

  /** \details
    Returns a plane, containing the specified point, perpendicular to this line.

    \param point [in]  Any 3D point.
    \param plane [out]  Receives the perpendicular plane.

    \remarks
    The returned plane is created with the new method. It is up to the caller to delete it.
  */
   void getPerpPlane(
      const OdGePoint3d& point, 
      OdGePlane& plane) const;

  /** \details
    Returns an arbitrary point on this line.
  */
   OdGePoint3d pointOnLine() const;

  /** \details
    Returns a unit vector parallel to this line, 
    and pointing in the direction of increasing parametric value.
  */
   OdGeVector3d direction() const;

  /** \details
    Returns a reference to an infinite line colinear with this one.

    \param line [out]  Receives the infinite line.
  */
   void getLine(OdGeLine3d& line) const;

  OdGeLinearEnt3d& operator =(
      const OdGeLinearEnt3d& line);
protected:
  OdGeLinearEnt3d(); 
  OdGeLinearEnt3d(
    const OdGeLinearEnt3d& source);
};

#include "TD_PackPop.h"

#endif // OD_GELENT3D_H
