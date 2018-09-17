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



#ifndef OD_AVEMAPPING_H
#define OD_AVEMAPPING_H

#include "TD_PackPush.h"

#include "Ve/AveDefs.h"
#include "Ge/GePoint3d.h"
#include "Ge/GeMatrix3d.h"

/** \details
    This class represents AVE mapping settings.
    Corresponding C++ library: TD_Ave
    <group OdAve_Classes>
*/
class AVE_EXPORT OdAveMapping
{
public:
  OdAveMapping();
  /** \details
    Populates this Mapping object with the mapping settings from the specified object.
    \param objectId [in]  Object ID of the source object.
    \returns
    Returns true if and only if there were mapping settings for the specified object.
  */
  bool getMapping(const OdDbObjectId& objectId);
  /** \details
    Writes the mapping settings from this Mapping object 
    to the specified object. 
    \param objectId [in]  Object ID of the destination object.
  */
  void setMapping(const OdDbObjectId& objectId);

  /** \details
    Removes the mapping settings from the specified object. 
    \param objectId [in]  Object ID of object to be cleared.
  */
  static void clearMapping(const OdDbObjectId& objectId);

  OdAve::SetuvType  m_MappingType;  // Description: kSetuvPlanar=1, kSetuvCylindrical=2, kSetuvSpherical=3, kSetuvSolid=6
  OdGePoint3d       m_Pt1;          // Description: Lower-left corner of mapping plane.
  OdGePoint3d       m_Pt2;          // Description: Lower-right corner of mapping plane.
  OdGePoint3d       m_Pt3;          // Description: Upper left corner of mapping plane.
  OdGePoint3d       m_Pt4;          // Description: Upper-right corner of mapping plane.
  OdAve::TileMethod m_Tile;         // Description: kTmDefault=0, kTmTile=1, kTmClamp=2
  OdGePoint3d       m_Scale;        // Description: Mapping scale.
  OdGePoint3d       m_Offset;       // Description: Mapping offset.
  OdGeMatrix3d      m_Transform;    // Description: Transformation matrix from UCS to mapping plane.
};

#include "TD_PackPop.h"

#endif // OD_AVEMAPPING_H
