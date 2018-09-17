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

#ifndef OD_GECLIPBOUNDARY2D_H
#define OD_GECLIPBOUNDARY2D_H /*!DOM*/

#include "Ge/GeEntity2d.h"
#include "Ge/GePoint2dArray.h"
#include "Ge/GeIntArray.h"

#include "TD_PackPush.h"

/** \details
    This class defines a planar clipping object for clipping arbitrary
    2D polygons or polylines to 2D convex open or closed polyline curves
    or rectangles.

    Corresponding C++ library: TD_Ge

   <group OdGe_Classes> 
*/
class GE_TOOLKIT_EXPORT OdGeClipBoundary2d : public OdGeEntity2d
{
public:
  

  /** \details
    Constructor for ortho-aligned rectangular clip boundary.
    Constructor for convex polyline / polygon clip boundary.
    Copy constructor.

    \remarks
    Currently not implemented.
  */
  OdGeClipBoundary2d();
  OdGeClipBoundary2d(
    const OdGePoint2d& cornerA, 
    const OdGePoint2d& cornerB);
  OdGeClipBoundary2d(
    const OdGePoint2dArray& clipBoundary);
  OdGeClipBoundary2d(
    const OdGeClipBoundary2d& src);

  /** \details
    Initialize ortho-aligned rectangular clip boundary.

    \remarks
    Currently not implemented.
  */
  OdGe::ClipError	set(
    const OdGePoint2d& cornerA, 
    const OdGePoint2d& cornerB);
    
  /** \details
    Initialize convex polyline / polygon clip boundary.

    \remarks
    Currently not implemented.
  */
  OdGe::ClipError	set(
    const OdGePoint2dArray& clipBoundary);

  /** \details
    Clip a closed polygon, creating a second closed polygon.

    \remarks
    Currently not implemented.
  */
  OdGe::ClipError	clipPolygon(
    const OdGePoint2dArray& rawVertices, 
    OdGePoint2dArray& clippedVertices,
    OdGe::ClipCondition& clipCondition,
    OdGeIntArray* pClippedSegmentSourceLabel = NULL) const;

  /** \details
    Clip a polyline, creating a second polyline.

    \remarks
    Currently not implemented.
  */
  OdGe::ClipError	clipPolyline(
    const OdGePoint2dArray& rawVertices, 
    OdGePoint2dArray& clippedVertices,
    OdGe::ClipCondition& clipCondition,
    OdGeIntArray* pClippedSegmentSourceLabel = NULL) const;

  /// Assignment operator.
  OdGeClipBoundary2d& operator =(const OdGeClipBoundary2d& src);

};

#include "TD_PackPop.h"

#endif
