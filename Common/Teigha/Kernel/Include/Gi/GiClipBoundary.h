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

#ifndef _GI_CLIP_BOUNDARY_H_INCLUDED_
#define _GI_CLIP_BOUNDARY_H_INCLUDED_

#include "TD_PackPush.h"

#include "Gi/GiExport.h"
#include "RxObject.h"
#include "Ge/GePoint2dArray.h"
#include "Ge/GeVector3d.h"
#include "Ge/GeMatrix3d.h"

/** \details
    This struct is used to pass clip boundary specifications from OdGiDrawable objects to the underlying OdGi system.
    Corresponding C++ library: TD_Gi
    <group !!RECORDS_TD_APIRef> 
*/
struct OdGiClipBoundary
{
  OdGeVector3d          m_vNormal;                // Normal vector that defines the plane in which the clip boundary lies.               
  OdGePoint3d           m_ptPoint;                // Origin of the clip boundary.
  OdGePoint2dArray      m_Points;                 // Array of points that defines the clip boundary.
  OdGeMatrix3d          m_xToClipSpace;           // Transformation matrix from ModelSpace to ClipSpace.
  OdGeMatrix3d          m_xInverseBlockRefXForm;  // Transformation matrix from BlockSpace to WorldSpace.
  double                m_dFrontClipZ;            // Distance from target point to FrontClip plane.
  double                m_dBackClipZ;             // Distance from target point to BackClip plane.
  bool                  m_bClippingFront;         // Enables front clipping.
  bool                  m_bDrawBoundary;          // Enables clipping boundary to be drawn.
  bool                  m_bClippingBack;          // Enables back clipping.
};

/** \details
    Checks clip boundary is valid for clipping.
*/
ODGI_EXPORT bool odgiIsValidClipBoundary(const OdGePoint2dArray& points);

/** \details
    Make clip boundary empty.
*/
ODGI_EXPORT void odgiEmptyClipBoundary(OdGiClipBoundary &clipBoundary);

#include "TD_PackPop.h"

#endif
