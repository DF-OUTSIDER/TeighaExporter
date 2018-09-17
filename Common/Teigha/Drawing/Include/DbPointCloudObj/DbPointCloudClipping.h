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

#ifndef _OD_DB_POINTCLOUDCLIPPING_
#define _OD_DB_POINTCLOUDCLIPPING_

#include "TD_PackPush.h"
#include "DbPointCloudObjExports.h"
#include "DbObject.h"
#include "Ge/GePoint3d.h"
#include "Ge/GePoint2dArray.h"


/** \details
  This class represents clipping boundaries for point cloud entities.

  Corresponding C++ library: AcDbPointCloudObj

  <group OdDbPointCloud_Classes>
*/
class DBPOINTCLOUDOBJ_EXPORT OdDbPointCloudClipping
{
public:
  /** \details
  Defines clipping boundary types.
  */
  enum ClipType
  {
    CT_BOX = 1,       //Box
    CT_RECTANGLE = 2, //Rectangular boundary
    CT_POLYLINE = 3   //Polyline boundary
  };

  /** \details
    Default constructor. Creates an OdDbPointCloudClipping instance.
  */
  OdDbPointCloudClipping();

  bool operator == (const OdDbPointCloudClipping&) const;

  /** \details
    Check whether the boundary is valid. Returns true only if a number of vertices matches the boundary type: 
    box or rectangular boundary must have two vertices, polygonal - at least 3 vertices.
  */
  bool isValid() const;

  /** \details
    Returns the number of vertices of this clipping boundary.
  */
  OdInt16 length() const;


  /** \details
    Sets this clipping boundary to a parallelepiped clipping boundary with the specified diagonal vertices.

    \param p1 [in]  - First corner.
    \param p2 [in]  - Second corner.
  */
	void setBox(OdGePoint3d const &, OdGePoint3d const &);

  /** \details
    Sets this clipping boundary to a rectangle clipping boundary with the specified diagonal vertices.

    \param p1 [in]  - First vertex.
    \param p2 [in]  - Second vertex.
  */
	void setRectangle(OdGePoint2d const &, OdGePoint2d const &);

  /** \details
    Sets this clipping boundary to a polygonal clipping boundary with the specified vertices.

    \param points [in]  - Array of vertices.
  */
  void setBoundary(const OdGePoint2dArray &);

  OdResult dwgInFields(OdDbDwgFiler *pFiler);
  void dwgOutFields(OdDbDwgFiler *pFiler) const;

  OdResult dxfInFields(OdDbDxfFiler *pFiler);
  void dxfOutFields(OdDbDxfFiler *pFiler, int &ind1, int &ind2, int &ind3) const;

  /** \details
    Returns the type of this clipping boundary.
  */
  ClipType type() const;

  /** \details
    Sets the maximum value of Z-coordinate.
  */
  void setZMax(const double);

  /** \details
    Sets the minimum value of Z-coordinate.
  */
  void setZMin(const double);

/** \details
    Returns the minimum value of Z-coordinate.
*/
  double zMax() const;

  /** \details
    Returns the minimum value of Z-coordinate.
  */
  double zMin() const;


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
  const OdGePoint2dArray& vertices() const;


private:
  
  OdInt16 m_type;
  bool m_inverted;
  double m_zMax;
  double m_zMin;

  OdGePoint2dArray m_points;
};

#include "TD_PackPop.h"

#endif
