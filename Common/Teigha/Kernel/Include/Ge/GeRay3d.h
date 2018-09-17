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

#ifndef OD_GERAY3D_H
#define OD_GERAY3D_H /*!DOM*/

#include "Ge/GeLinearEnt3d.h"

#include "TD_PackPush.h"

/** \details
    This class represents semi-infinite lines in 3D space.

    Corresponding C++ library: TD_Ge

    <group OdGe_Classes>

    \sa
    <link ge_OdGeRay.html, Working with Rays>
*/
class GE_TOOLKIT_EXPORT OdGeRay3d : public OdGeLinearEnt3d
{
public:


  /** \param line [in]  Any 3D line.
    \param point [in]  Any 3D point.
    \param vect [in]  Any 3D vector
    \param point1 [in]  Any 3D point.
    \param point2 [in]  Any 3D point.

    \remarks
    point and vect construct a semi-infinite line starting point with 
    a direction of vect. vect cannot have a zero length.

    point1 and point2 construct a semi-infinite line starting at point1, and passing through point2. The 
    points cannot be coincident.

    If called with no arguments, constructs a semi-infinite line starting at (0,0m0) and passing through (1,0,0).
  */
  OdGeRay3d();
  OdGeRay3d(
    const OdGeRay3d& line);
  OdGeRay3d(
    const OdGePoint3d& point, 
    const OdGeVector3d& vect);
  OdGeRay3d(const OdGePoint3d& point1, 
    const OdGePoint3d& point2);

  /** \details
    Sets the parameters for this ray according to the arguments.

    \param point [in]  Any 3D point.
    \param vect [in]  Any 3D vector.

    \returns
    Returns a reference to this ray.
    \remarks
    point and vect construct a semi-infinite line starting point with 
    a direction of vect. vect cannot have a zero length.
  */
   OdGeRay3d& set(
    const OdGePoint3d& point, 
    const OdGeVector3d& vect);

  /** \details
    Sets the parameters for this ray according to the arguments.

    \param point1 [in]  Any 3D point.
    \param point2 [in]  Any 3D point.

    \returns
    Returns a reference to this ray.
    \remarks
    point1 and point2 construct a semi-infinite line starting at point1, and passing through point2. The 
    points cannot be coincident.
  */
    OdGeRay3d& set(
    const OdGePoint3d& point1, 
    const OdGePoint3d& point2);

  // Assignment operator.
  OdGeRay3d& operator =(
    const OdGeRay3d& line);
};

#include "TD_PackPop.h"

#endif

