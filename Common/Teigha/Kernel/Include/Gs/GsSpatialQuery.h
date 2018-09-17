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
#ifndef __OD_GS_SPATIAL_QUERY_H_
#define __OD_GS_SPATIAL_QUERY_H_

#include "TD_PackPush.h"
#include "Si/BBox.h"
#include "Gs/SiVolume.h"
#include "Gs/GsExport.h"
#include "Ge/GeExtents3d.h"

class OdGsViewImpl;
class OdGsDCPoint;
class OdGePoint2d;

/** \details
            
    Corresponding C++ library: TD_Gs

    <group OdGs_Classes> 
*/
class GS_TOOLKIT_EXPORT OdGsSpatialQuery
{
public:
  OdGsSpatialQuery(){}

  /** \details
  Builds a spatial query for the specified polygon in this view object.
  \param sceneExtents [in]  Whole scene extents.
  \param points [in]  Array of points in eye coords.
  \param numPoints [in]  Number of points.
  */
  const OdSiShape& build(const OdGsViewImpl& view,
      const OdGeExtents3d& sceneExtents,
      int numPoints, const OdGePoint2d* points);

  /** \details
    Builds a spatial query for the specified polygon in this view object.
    \param sceneExtents [in]  Whole scene extents.
    \param points [in]  Array of points in screen coords.
    \param numPoints [in]  Number of points. 
  */
  const OdSiShape& build(const OdGsViewImpl& view,
      const OdGeExtents3d& sceneExtents,
      int numPoints = 0, const OdGsDCPoint* points = 0);

  /** \details
  Returns the current query's bounding box; assumed to be called after 'build()'.
  */
  const OdSi::BBox& getBBox() const { return m_spqbox; }

protected:
  OdSi::BBox m_spqbox;
  OdSi::Volume m_spqvol;
};

#include "TD_PackPop.h"

#endif // __OD_GS_SPATIAL_QUERY_H_
