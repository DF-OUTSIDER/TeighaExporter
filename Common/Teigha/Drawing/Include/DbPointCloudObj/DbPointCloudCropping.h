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

#ifndef _OD_DB_POINTCLOUDCROPPING_
#define _OD_DB_POINTCLOUDCROPPING_

#include "TD_PackPush.h"
#include "DbPointCloudObjExports.h"
#include "DbObject.h"
#include "Ge/GePoint3d.h"
#include "Ge/GePoint2dArray.h"
#include "Ge/GePlane.h"


class OdGiGeometry;

/** \details
  This class represents cropping boundaries for point cloud entities.

  Corresponding C++ library: AcDbPointCloudObj

  <group OdDbPointCloud_Classes>
*/
class DBPOINTCLOUDOBJ_EXPORT OdDbPointCloudCropping
{
public:
  /** \details
  Defines cropping boundary types.
  */
  enum CropType
  {
    CT_INVALID = 0,       //Box
    CT_RECTANGLE,         //Rectangular boundary
    CT_POLYLINE,          //Polyline boundary
    CT_CIRCULAR           //Circular bounfary 
  };

  /** \details
    Default constructor. Creates an OdDbPointCloudCropping instance.
  */
  OdDbPointCloudCropping();

  bool operator == (const OdDbPointCloudCropping&) const;

  /** \details
    Check whether the boundary is valid. Returns true only if a number of vertices matches the boundary type: 
    box or rectangular boundary must have two vertices, polygonal - at least 3 vertices.
  */
  bool isValid() const;

  /** \details
    Returns the number of vertices of this cropping boundary.
  */
  OdInt16 length() const; 

  /** \details
    Returns the type of this cropping boundary.
  */
  CropType type() const;

  /** \details
    Set type of this cropping boundary.
  */
  void setCropType( CropType type );

  /** \details
    Sets the boundary to inverted.
  */
  void setInverted(const bool);

  /** \details
    Returns true if only the boundary is inverted.
  */
  bool isInverted() const;

  /** \details
    Returns closed boundary
  */
  const OdGePoint3dArray& get() const;

  /** \details
    Returns closed boundary
  */
  void set( const OdGePoint3dArray& points) ;

  /** \details

  */
  bool getCropPlane( OdGePlane& plane ) const;

  /** \details

  */
  void setCropPlane( const OdGePlane& plane ) ;

  /** \details

  */
  void setInside(const bool);

  /** \details

  */
  bool isInside() const;

  /** \details

  */
  void clear();

  /** \details

  */
  virtual void draw( OdGiGeometry &geom );

  OdResult dwgInFields( OdDbDwgFiler *pFiler );
  void dwgOutFields( OdDbDwgFiler *pFiler ) const;

  OdResult dxfInFields( OdDbDxfFiler *pFiler );
  void dxfOutFields( OdDbDxfFiler *pFiler, OdInt16 &dvfInt8, OdInt16 &dxfInt32, OdInt16 &dxfXCoord, OdInt16 &dxfNormalX ) const;


private:
  
  OdInt8 m_type;
  bool m_isInside;
  bool m_inverted;
  OdGePlane m_plane;
  OdGePoint3dArray m_points;
};

#include "TD_PackPop.h"

#endif //_OD_DB_POINTCLOUDCROPPING_
