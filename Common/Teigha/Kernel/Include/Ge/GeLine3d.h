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

#ifndef OD_GELINE3D_H
#define OD_GELINE3D_H /*!DOM*/

class OdGeLine2d;
#include "Ge/GeLinearEnt3d.h"

#include "TD_PackPush.h"

/** \details
    This class represents infinite lines in 3D space.

    Corresponding C++ library: TD_Ge

    <group OdGe_Classes>

    \sa
    <link ge_OdGeLine.html, Working with Lines>
*/
class GE_TOOLKIT_EXPORT OdGeLine3d : public OdGeLinearEnt3d
{
public:


  /** \param line [in]  Any 3D line.
    \param point [in]  Any 3D point.
    \param point1 [in]  Any 3D point.
    \param point2 [in]  Any 3D point.
    \param source [in]  Object to be cloned.
    \param vect [in]  Any 3D vector.
    
    \remarks
    point and vect construct an infinite line passing through point with 
    a direction of vect. vect cannot have a zero length.

    point1 and point2 construct an infinite line passing through both points. The 
    points cannnot be coincident.

    If called with no arguments, constructs in infinite line coincident with the X-axis.
  */
  OdGeLine3d();
  OdGeLine3d(
    const OdGeLine3d& source);
  OdGeLine3d(
    const OdGePoint3d& point, 
    const OdGeVector3d& vect);
  OdGeLine3d(
    const OdGePoint3d& point1, 
    const OdGePoint3d& point2);

  GE_STATIC_EXPORT static const OdGeLine3d kXAxis; // X-axis *line*.
  GE_STATIC_EXPORT static const OdGeLine3d kYAxis; // Y-axis *line*.
  GE_STATIC_EXPORT static const OdGeLine3d kZAxis; // Z-axis *line*.

  /** \details
    Sets the parameters for this line according to the arguments, and returns a reference to this line.

    \param point [in]  Any 3D point.
    \param vect [in]  Any 3D vector.

    \remarks
    point and vect construct an infinite line passing through point with 
    a direction of vect. vect cannot have a zero length.
  */
   OdGeLine3d& set(
    const OdGePoint3d& point, 
    const OdGeVector3d& vect);

  /** \details
    Sets the parameters for this line according to the arguments, and returns a reference to this line.

    \param point1 [in]  Any 3D point.
    \param point2 [in]  Any 3D point.

    \remarks
    point1 and point2 construct an infinite line passing through both points. The 
    points should no be coincident.
  */
  OdGeLine3d& set(
    const OdGePoint3d& point1, 
    const OdGePoint3d& point2);

  OdGeLine3d& operator =(
    const OdGeLine3d& line);

private:
  OdGeLine3d(OdGeEntity3dImpl*);
};

#include "TD_PackPop.h"

#endif // OD_GELINE3D_H
