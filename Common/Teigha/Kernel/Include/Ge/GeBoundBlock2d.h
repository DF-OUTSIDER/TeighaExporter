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

#ifndef OD_GE_BOUND_BLOCK2D_H
#define OD_GE_BOUND_BLOCK2D_H /*!DOM*/

#include "Ge/GeEntity2d.h"
#include "Ge/GeExtents2d.h"

#include "TD_PackPush.h"

/** \details
    This class implements a 2D bounding area.

    Corresponding C++ library: TD_Ge

    <group OdGe_Classes>

	  \sa
    <link ge_OdGeBoundBlock3D.html, Working with Bounding Blocks>
*/
class GE_TOOLKIT_EXPORT OdGeBoundBlock2d : public OdGeEntity2d
{
public:
  

  /** \param point1 [in]  First point of a coordinate-aligned box.
    \param point2 [in]  Second point of a coordinate-aligned box.
    \param base [in]  Base of parallelogram bounding block.
    \param dir1 [in]  First side of parallelogram bounding block.
    \param dir2 [in]  Second side of parallelogram bounding block.
    \param source [in]  Object to be cloned.
    
    \remarks
    The default constructor constructs a parallelogram reduced to the coordinate origin.
  */
  OdGeBoundBlock2d();
  OdGeBoundBlock2d(
    const OdGePoint2d& point1, 
    const OdGePoint2d& point2);

  OdGeBoundBlock2d(
    const OdGePoint2d& base, 
    const OdGeVector2d& dir1, 
    const OdGeVector2d& dir2);
  OdGeBoundBlock2d(
    const OdGeBoundBlock2d& source);

  /** \details
    Returns the extents of the bounding block.

    \param p1 [out]  Receives the first corner of the extents.
    \param p2 [out]  Receives the second corner of the extents.
  */
  void getMinMaxPoints(
    OdGePoint2d& p1, 
    OdGePoint2d& p2) const;

  /** \details
    Returns base and sides of bounding block.

    \param base [out]  Receives the base of bounding block.
    \param side1 [out]  Receives the first side of bounding block.
    \param side2 [out]  Receives the second side of bounding block.
  */
  void get(
    OdGePoint2d& base, 
    OdGeVector2d& side1, 
    OdGeVector2d& side2) const;

  /** \details
    Sets the bounding block to a coordinate-aligned box.

    \param p1 [in]  First point of a coordinate-aligned box.
    \param p2 [in]  Second point of a coordinate-aligned box.
  */
  OdGeBoundBlock2d& set(
    const OdGePoint2d& p1, 
    const OdGePoint2d& p2);

  /** \details
    Sets the bounding block to a parallelogram bounding block.

    \param base [in]  Base of parallelogram bounding block.
    \param side1 [in]  First side of parallelogram bounding block.
    \param side2 [in]  Second side of parallelogram bounding block.
  */
  OdGeBoundBlock2d& set(
    const OdGePoint2d& base, 
    const OdGeVector2d& side1, 
    const OdGeVector2d& side2);

  /** \details
    Extends the bounding block to contain
    the specified point.

    \param point [in]  Any 2D point.
  */
  OdGeBoundBlock2d& extend(
    const OdGePoint2d& point);

  /** \details
    Moves the walls of the bounding block the specified distance.

    \param distance [in]  Distance.
  */
  OdGeBoundBlock2d& swell(
    double distance);

  /** \details
    Returns true if and only if the point is
    contained in the bounding block.

    \param point [in]  Any 2D point.
  */
  bool contains(
    const OdGePoint2d& point) const;

  /** \details
    Returns true if and only if specified bounding block
    does not intersect this bounding block.

    \param block [in]  Any 2D bounding block.
  */
  bool isDisjoint(
    const OdGeBoundBlock2d& block) const;

  OdGeBoundBlock2d& operator =(
    const OdGeBoundBlock2d& block);

  /** \details
    Returns true if and only if this bounding block is a
    coordinate-aligned box.
  */
  bool isBox() const;

  /** \details
  Sets this bounding block to a coordinate-aligned box, or a
    parallelogram bounding block. Returns a reference to this 
    bounding block.

    \param toBox [in]  If true, sets this bounding block to a 
      coordinate-aligned box; otherwise, sets it to a 
      parallelogram bounding block.
  */
  OdGeBoundBlock2d& setToBox(
    bool toBox);
};

#include "TD_PackPop.h"

#endif // OD_GE_BOUND_BLOCK2D_H

