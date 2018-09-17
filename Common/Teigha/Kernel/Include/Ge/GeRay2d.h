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

#ifndef OD_GE_RAY_2D_H
#define OD_GE_RAY_2D_H /*!DOM*/

#include "Ge/GeLinearEnt2d.h"

#include "TD_PackPush.h"

/** \details
    This class represents semi-infinite lines in 2D space.

    Corresponding C++ library: TD_Ge

    <group OdGe_Classes>

    \sa
    <link ge_OdGeRay.html, Working with Rays>
*/
class GE_TOOLKIT_EXPORT OdGeRay2d : public OdGeLinearEnt2d
{
public:
  

  /** \param line [in]  Any 2D line.
    \param point [in]  Any 2D point.
    \param vect [in]  Any 2D vector
    \param point1 [in]  Any 2D point.
    \param point2 [in]  Any 2D point.

    \remarks
    point and vect construct a semi-infinite line starting point with 
    a direction of vect. vect cannot have a zero length.

    point1 and point2 construct a semi-infinite line starting at point1, and passing through point2. The 
    points cannnot be coincident.

    If called with no arguments, constructs a semi-infinite line starting at (0,0) and passing through (1,0).
  */
  OdGeRay2d();
  OdGeRay2d(const OdGeRay2d& ray);
  OdGeRay2d(
    const OdGePoint2d& point, 
    const OdGeVector2d& vect);
  OdGeRay2d(
    const OdGePoint2d& point1, 
    const OdGePoint2d& point2);
  
  /** \details
    Sets the parameters for this ray according to the arguments.

    \param point [in]  Any 2D point.
    \param vect [in]  Any 2D vector.

    \returns
    Returns a reference to this ray.
    \remarks
    point and vect construct a semi-infinite line starting point with 
    a direction of vect. vect cannot have a zero length.
  */
  OdGeRay2d& set(
    const OdGePoint2d& point, 
    const OdGeVector2d& vect);
  
  /** \details
    Sets the parameters for this ray according to the arguments.

    \param point1 [in]  Any 2D point.
    \param point2 [in]  Any 2D point.

    \returns
    Returns a reference to this ray.
    \remarks
    point1 and point2 construct a semi-infinite line starting at point1, and passing through point2. The 
    points cannnot be coincident.
  */
  OdGeRay2d& set(
    const OdGePoint2d& point1, 
    const OdGePoint2d& point2);

  OdGeRay2d& operator =(const OdGeRay2d& ray);

};

#include "TD_PackPop.h"

#endif // OD_GE_RAY_2D_H
