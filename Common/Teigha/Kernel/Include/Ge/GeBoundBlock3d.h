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

#ifndef _OD_GEBOUNDBLOCK3D_H
#define _OD_GEBOUNDBLOCK3D_H /*!DOM*/

#include "Ge/GeEntity3d.h"
#include "Ge/GeExtents3d.h"

#include "TD_PackPush.h"

/** \details
    This class implements 3D bounding volumes.

    Corresponding C++ library: TD_Ge

    <group OdGe_Classes>

    \sa
    <link ge_OdGeBoundBlock3D.html, Working with Bounding Blocks>
*/
class GE_TOOLKIT_EXPORT OdGeBoundBlock3d : public OdGeEntity3d
{
public: 


  /** \param point1 [in]  First point of a coordinate-aligned block.
    \param point2 [in]  Second point of a coordinate-aligned block.
    \param base [in]  Base of parallelepiped bounding block.
    \param side1 [in]  First side of parallelepiped bounding block.
    \param side2 [in]  Second side of parallelepiped bounding block.
    \param side3 [in]  Third side of parallelepiped bounding block.
    \param source [in]  Object to be cloned.

    \remarks
    The default constructor constructs a parallelepiped reduced to the coordinate origin.
    
    A parallelepiped is a parallelogram extruded in an arbitrary direction. 
  */
  OdGeBoundBlock3d();
  OdGeBoundBlock3d(
    const OdGePoint3d& base, 
    const OdGeVector3d& side1,
    const OdGeVector3d& side2, 
    const OdGeVector3d& side3);
  // TD Special
  OdGeBoundBlock3d(
    const OdGePoint3d& point1, 
    const OdGePoint3d& point2);
  //
  OdGeBoundBlock3d(
    const OdGeBoundBlock3d& source);

  /** \details
    Returns the extents of the bounding block.

    \param p1 [out]  Receives the first corner of the extents.
    \param p2 [out]  Receives the second corner of the extents.
  */
   void getMinMaxPoints(
    OdGePoint3d& p1, 
    OdGePoint3d& p2) const;

  /** \details
    Returns base and sides of bounding block.

    \param base [out]  Receives the base of the bounding box.
    \param side1 [out]  Receives the first side.
    \param side2 [out]  Receives the second side.
    \param side3 [out]  Receives the third side.
  */
   void get(
    OdGePoint3d& base,
    OdGeVector3d& side1,
    OdGeVector3d& side2,
    OdGeVector3d& side3) const;

  /** \details
    Sets the bounding block to a coordinate-aligned box.

    \param p1 [in]  First point of a coordinate-aligned box.
    \param p2 [in]  Second point of a coordinate-aligned box.
  */
   OdGeBoundBlock3d& set(
    const OdGePoint3d& p1,
    const OdGePoint3d& p2);
    
  /** \details
    Sets the bounding block to a parallelepiped bounding block.

    \param base [in]  Base of parallelepiped bounding block.
    \param side1 [in]  First side of parallelepiped bounding block.
    \param side2 [in]  Second side of parallelepiped bounding block.
    \param side3 [in]  Third side of parallelepiped bounding block.
  */
   OdGeBoundBlock3d& set(
    const OdGePoint3d& base,
    const OdGeVector3d& side1,
    const OdGeVector3d& side2,
    const OdGeVector3d& side3);

  /** \details
    Extends the bounding block to contain
    the specified point.

    \param point [in]  Any 3D point.
  */
   OdGeBoundBlock3d& extend(
    const OdGePoint3d& point);

  /** \details
    Moves the walls of the bounding block the specified distance.

    \param distance [in]  Distance.
  */
   OdGeBoundBlock3d& swell(
    double distance);

  /** \details
    Returns true if and only if this point is
    contained in the bounding block.

    \param point [in]  Any 3D point.
  */
   bool contains(
    const OdGePoint3d& point, 
    const OdGeTol& tol = OdGeContext::gTol) const;

  /** \details
    Returns true if and only if specified bounding block
    does not intersect this bounding block.

    \param block [in]  Any 3D bounding block.
  */
   bool isDisjoint(
    const OdGeBoundBlock3d& block,
    const OdGeTol& tol = OdGeContext::gTol) const;

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
    coordinate-aligned otherwise, sets it to a 
    parallelepiped bounding block.
  */
   OdGeBoundBlock3d&  setToBox(
    bool toBox);

  OdGeBoundBlock3d& operator =(const OdGeBoundBlock3d& block);

  //////////////////////////////////////////////////////////////////////////
  // TD Special :

   OdGePoint3d minPoint() const;
   OdGePoint3d maxPoint() const;

  /** \details
    Returns the center of the bounding block.
  */
   OdGePoint3d center() const;

  /** \details
    Call setToBox(true) if axes is parallel to world coordinate space axes.
  */
   void setToBoxOrtho(const OdGeVector3d &dir1, const OdGeVector3d &dir2, const OdGeVector3d &dir3);
  
  /** \details
    Call setToBox(true) if axes is parallel to world coordinate space axes.
  */
   void setToBoxOrtho();
};

inline void OdGeBoundBlock3d::setToBoxOrtho(const OdGeVector3d &dir1, const OdGeVector3d &dir2, const OdGeVector3d &dir3)
{
  OdGeError status;
  if((dir1.isParallelTo(OdGeVector3d::kXAxis)
      || dir1.isParallelTo(OdGeVector3d::kYAxis)
      || dir1.isParallelTo(OdGeVector3d::kZAxis, OdGeContext::gTol, status)
      || status == OdGe::k0This)
      && (dir2.isParallelTo(OdGeVector3d::kXAxis)
      || dir2.isParallelTo(OdGeVector3d::kYAxis)
      || dir2.isParallelTo(OdGeVector3d::kZAxis, OdGeContext::gTol, status)
      || status == OdGe::k0This)
      && (dir3.isParallelTo(OdGeVector3d::kXAxis)
      || dir3.isParallelTo(OdGeVector3d::kYAxis)
      || dir3.isParallelTo(OdGeVector3d::kZAxis, OdGeContext::gTol, status)
      || status == OdGe::k0This))
    setToBox(true);
}

inline void OdGeBoundBlock3d::setToBoxOrtho()
{
  OdGePoint3d basePt;
  OdGeVector3d dir1, dir2, dir3;
  get(basePt, dir1, dir2, dir3);
  setToBoxOrtho(dir1, dir2, dir3);
}

#include "TD_PackPop.h"

#endif
