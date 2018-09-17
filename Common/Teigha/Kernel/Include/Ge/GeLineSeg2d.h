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

#ifndef OD_GE_LINE_SEG_2D_H
#define OD_GE_LINE_SEG_2D_H /*!DOM*/

#include "Ge/GeLinearEnt2d.h"

#include "TD_PackPush.h"

/** \details
    This class represents line segments in 2D space.

    Corresponding C++ library: TD_Ge

    <group OdGe_Classes> 

    \sa
    <link ge_OdGeLineSeg.html, Working with Line Segments>
*/
class GE_TOOLKIT_EXPORT OdGeLineSeg2d : public OdGeLinearEnt2d
{
public:
  

  /** \param line [in]  Any 2D line.
    \param point [in]  Any 2D point.
    \param point1 [in]  Any 2D point.
    \param point2 [in]  Any 2D point.
    \param vect [in]  Any 2D vector
    \param source [in]  Object to be cloned.

    \remarks
    point and vect construct a line segment between points point and point + vect. vect cannot have a zero length.

    point1 and point2 construct a line segment between points point1 and point2. The 
    points cannot be coincident.

    If called with no arguments, constructs a line segment between the points (0,0) and (1,0).
  */
  OdGeLineSeg2d();
  OdGeLineSeg2d(
    const OdGeLineSeg2d& source);
  OdGeLineSeg2d(
    const OdGePoint2d& point1, 
    const OdGePoint2d& point2);
  OdGeLineSeg2d(
    const OdGePoint2d& point, 
    const OdGeVector2d& vect);

  /** \details
    Sets the parameters for this line according to the arguments, and returns a reference to this line.

    \param point [in]  Any 2D point.
    \param vect [in]  Any 2D vector

    \remarks
    point and vect construct a line segment between points point and point + vect. vect cannot have a zero length.
  */
  OdGeLineSeg2d& set(
    const OdGePoint2d& point, 
    const OdGeVector2d& vect);

  /** \details
    Sets the parameters for this line according to the arguments, and returns a reference to this line.

    \param point1 [in]  Any 2D point.
    \param point2 [in]  Any 2D point.
    
    \remarks
    point1 and point2 construct a line segment between points point1 and point2. The 
    points cannot be coincident.
  */
  OdGeLineSeg2d& set(
    const OdGePoint2d& point1, 
    const OdGePoint2d& point2);

  /** \details
    Sets the parameters for this line according to the arguments, and returns a reference to this line.

    \param curve1 [in]  Any 2D curve 
    \param curve2 [in]  Any 2D curve 
    \param param1 [in]  Point on curve1 where this line segment is tangent to curve1 
    \param param2 [in]  Point on curve2 where this line segment is tangent to curve2 
    \param success [out]  Receives true if and only if the tangent was constructed. If false, this line segment is unchanged.

    \remarks
    curve1 and curve2 construct a tangent line segment between curve1 and curve2. param1 and param2 are  
    the approximate tangent points on curve1 and curve2, respectively.
  */
  OdGeLineSeg2d& set(
    const OdGeCurve2d& curve1, 
    const OdGeCurve2d& curve2,
    double& param1, 
    double& param2, 
    bool& success);

  /** \details
    Sets the parameters for this line according to the arguments, and returns a reference to this line.

    \param point [in]  Any 2D point.
    \param curve [in]  Any 2D curve.
    \param param [in]  Point on curve1 where this line segment is tangent to curve. 
    \param success [out]  Receives true if and only if the tangent was constructed. If false, this line segment is unchanged.

    \remarks
    curve and point construct a line segment starting at point, and tangent to curve. 
    param is the approximate tangent point on curve.
  */
  OdGeLineSeg2d& set(
    const OdGeCurve2d& curve, 
    const OdGePoint2d& point, 
    double& param, 
    bool& success);

  /** \details
    Gets the infinite perpendicular bisector of this line segment.
    
    \param line [out]  Receives the perpendicular bisector.
  */
  void getBisector(
    OdGeLine2d& line) const;
    
  /** \details
    Returns the weighted average of the start point and end point of this line segment.
    
    \param blendCoeff [in]  Blend coefficient.
    
    \remarks
    <table>
    blendCoeff      Returns
    0               start point
    1               end point
    0 to 1          point on this line segment
    < 0 or > 1      point not on this line segment, but colinear with it.
    </table>
  */
  OdGePoint2d baryComb(
    double blendCoeff) const;


  /** \details
    Returns the start point of this line segment.
  */
  OdGePoint2d startPoint() const;
  
  /** \details
    Returns the midpoint of this line segment.
  */
  OdGePoint2d midPoint() const;
  
  /** \details
    Returns the end point of this line segment.
  */
  OdGePoint2d endPoint() const; 

  OdGeLineSeg2d& operator =(const OdGeLineSeg2d& line);
};

#include "TD_PackPop.h"

#endif // OD_GE_LINE_SEG_2D_H

