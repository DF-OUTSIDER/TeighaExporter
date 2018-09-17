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

#ifndef OD_GEBNDPLN_H
#define OD_GEBNDPLN_H  /*!DOM*/

#include "Ge/GePlanarEnt.h"
#include "Ge/GePlane.h"
#include "Ge/GeVector3d.h"

class OdGePlane;
class OdGeVector3d;
class OdGePoint3d;
class OdGePoint2d;
class OdGeLineSeg3d;

#include "TD_PackPush.h"

/** \details
    This class represents bounded planes in 3D space.

    Corresponding C++ library: TD_Ge

    <group OdGe_Classes> 

    \sa
    <link ge_OdGeBoundedPlane.html, Working with Bounded Planes>
*/
class GE_TOOLKIT_EXPORT OdGeBoundedPlane : public OdGePlanarEnt
{
public:


  /** \param origin [in]  Origin of the bounded plane. 
    \param uAxis [in]  u axis.
    \param vAxis [in]  v axis.
    \param uPnt [in]  A point at the end of the U-axis.
    \param vPnt [in]  A point at the end of the V-axis.

    \remarks
    Bounded planes can be specified with an origin and two vectors, or 
    with an origin and two points.

    With no arguments, constructs an infinite plane parallel to the XY plane.
  */
  OdGeBoundedPlane();
  OdGeBoundedPlane(const OdGeBoundedPlane& plane);
  OdGeBoundedPlane(
    const OdGePoint3d& origin,
    const OdGeVector3d& uAxis,
    const OdGeVector3d& vAxis);
  OdGeBoundedPlane(
    const OdGePoint3d& uPnt,
    const OdGePoint3d& origin,
    const OdGePoint3d& vPnt);

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
    Sets parameters for this plane according to the arguments, and returns a reference to this plane.

    \param origin [in]  The origin of the bounded plane.
    \param uAxis [in]  The U-axis of the plane.
    \param vAxis [in]  The V-axis of the plane.
   
    \returns
    Returns a reference to this plane.
  */
   OdGeBoundedPlane& set(
    const OdGePoint3d& origin, 
    const OdGeVector3d& uAxis, 
    const OdGeVector3d& vAxis);
    
  /** \details
    Sets parameters for this plane according to the arguments, and returns a reference to this plane.

    \param origin [in]  The origin of the bounded plane.
    \param uPnt [in]  A point at the end of the U-axis.
    \param vPnt [in]  A point at the end of the V-axis.
   
    \returns
    Returns a reference to this plane.
  */
   OdGeBoundedPlane& set(
    const OdGePoint3d& uPnt,
    const OdGePoint3d& origin,
    const OdGePoint3d& vPnt);

  OdGeBoundedPlane& operator =(const OdGeBoundedPlane& plane);

  //////////////////////////////////////////////////////////////////////////
  TD_USING(OdGePlanarEnt::isOn);

};

#include "TD_PackPop.h"

#endif // OD_GEBNDPLN_H

