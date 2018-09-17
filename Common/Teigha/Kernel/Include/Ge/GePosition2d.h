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


#ifndef OD_GE_POSITION_2D_H
#define OD_GE_POSITION_2D_H /*!DOM*/

#include "Ge/GePointEnt2d.h"

#include "TD_PackPush.h"

/** \details
    This class represents points (positions) in 2D space.

    Corresponding C++ library: TD_Ge

    <group OdGe_Classes> 

    \sa
    <link ge_OdGePosition.html, Working with Points>
*/
class GE_TOOLKIT_EXPORT OdGePosition2d : public OdGePointEnt2d
{
public:
  

  /** \param point [in]  Any 2D point.
    \param x [in]  The X-coordinate of the point.
    \param y [in]  The Y-coordinate of the point.
    \param source [in]  Object to be cloned.
  */
  OdGePosition2d();
  OdGePosition2d(
    const OdGePoint2d& point);
  OdGePosition2d(
    double x, 
    double y);
  OdGePosition2d(
    const OdGePosition2d& source);

  /** \details
    Sets the coordinates of, and returns a reference
    to, this point.

    \param point [in]  Any 2D point.
  */
  OdGePosition2d& set(
    const OdGePoint2d& point);
  
  /** \details
    Sets the coordinates of, and returns a reference
    to, this point.

    \param x [in]  The X-coordinate of the position.
    \param y [in]  The Y-coordinate of the position.
  */
  OdGePosition2d& set(
    double x, 
    double y);

  OdGePosition2d& operator = (
    const OdGePosition2d& pos);

};

#include "TD_PackPop.h"

#endif // OD_GE_POSITION_2D_H

