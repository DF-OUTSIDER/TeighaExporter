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




#ifndef OD_DBSPFILT_H
#define OD_DBSPFILT_H

#include "TD_PackPush.h"

/** \details
   Defines the disabled back or front clipping plane distance.
*/
   #define ODDB_INFINITE_XCLIP_DEPTH (1.0e+300)

#include "DbFilter.h"
#include "Ge/GePoint3d.h"
#include "Ge/GePoint2dArray.h"

class OdGeExtents3d;
class OdDbBlockReference;

class OdDbFilteredBlockIterator;
/** \details
  This template class is a specialization of the OdSmartPtr class for OdDbFilteredBlockIterator object pointers.
*/
typedef OdSmartPtr<OdDbFilteredBlockIterator> OdDbFilteredBlockIteratorPtr;

struct OdGiClipBoundary;

/** \details
    This class implements Spatial Filter objects in an OdDbDatabase instance.

    \remarks
    Spatial Filter objects are extruded volumes based on a 2D boundary,
    an extrusion direction, and front and back clipping distances.
    
    They implement Xref clipping boundaries.
    
    \sa
    TD_Db

    <group OdDb_Classes>
*/
class TOOLKIT_EXPORT OdDbSpatialFilter : public OdDbFilter
{
public:
  ODDB_DECLARE_MEMBERS(OdDbSpatialFilter);

  OdDbSpatialFilter();

  OdRxClass* indexClass() const; 

  /** \details
    Returns clip boundary extents.

    \param ext [out]  Output extents.
  */
  void queryBounds(OdGeExtents3d& ext) const;

  /** \details
    Returns the clip boundary definition of this Spatial Filter object.
    
    \param fromPoint [out]  Receives the WCS 'from' point.
    \param toPoint [out]  Receives the  WCS 'to' point.
    \param upDir [out]  Receives the WCS normal vector.
    \param viewField [out]  Receives the WCS view field vector.
  */
  void getVolume(
      OdGePoint3d& fromPoint, 
      OdGePoint3d& toPoint, 
      OdGeVector3d& upDir,
      OdGeVector2d& viewField ) const;

  /** \details
    Sets the definition of this Spatial Filter object.

    \param points [in]  Boundary definition.
    \param normal [in]  WCS Positive extrusion vector.
    \param elevation [in]  Elevation.
    \param frontClip [in]  Front clip distance in the normal direction.
    \param backClip [in]  Back clip distance in the -normal direction.
    \param enabled [in]  Enables the clip volume. 
    
    \remarks
    The elevation is the distance from the WCS origin to the plane of the clip boundary.
 
    Together elevation and normal define the ECS plane of the clip boundary.
       
    If only two points are provided in points, these points define the diagonal of a rectangle. Otherwise,
    they define a non-self-intersecting polygon.
    
    The points are in the ECS defined by normal and elevation. 
    
    If the clip volume is disabled, this Spatial Filter object includes all of 3D space.
  */
  void setDefinition( 
    const OdGePoint2dArray& points, 
    const OdGeVector3d& normal = OdGeVector3d::kZAxis,
    double elevation = 0.0,
    double frontClip = ODDB_INFINITE_XCLIP_DEPTH,
    double backClip = ODDB_INFINITE_XCLIP_DEPTH,
    bool enabled = true); 

  /** \details
    Sets the definition of this Spatial Filter object.
    
    \param points [in]  Boundary definition.
    \param xToClipSpace [in]  The transformation matrix from WCS to ECS for the clip boundary.
    \param frontClip [in]  Front clip distance in the normal direction.
    \param backClip [in]  Back clip distance in the -normal direction.
    \param enabled [in]  Enables the clip volume. 
  */
  void setDefinition(
    const OdGePoint2dArray& points,
    const OdGeMatrix3d& xToClipSpace, 
    double frontClip = ODDB_INFINITE_XCLIP_DEPTH,
    double backClip = ODDB_INFINITE_XCLIP_DEPTH,
    bool enabled = true);

  /** \details
    Returns the definition of this Spatial Filter object.

    \param points [out]  Receives the boundary definition.
    \param normal [out]  Receives the WCS positive extrusion vector.
    \param elevation [out]  Receives the elevation.
    \param frontClip [out]  Receives the front clip distance in the normal direction.
    \param backClip [out]  Receives the back clip distance in the -normal direction.
    \param enabled [out]  Receives the status of the clip volume. 

    \remarks
    The elevation is the distance from the WCS origin to the plane of the clip boundary.
 
    Together elevation and normal define the ECS plane of the clip boundary.
       
    If only two points are provided in points, these points define the diagonal of a rectangle. Otherwise,
    they define a non-self-intersecting polygon.
    
    The points are in the ECS defined by normal and elevation. 
    
    If the clip volume is disabled, this Spatial Filter object includes all of 3D space.
  */
  void getDefinition( 
    OdGePoint2dArray& points, 
    OdGeVector3d& normal,
    double& elevation, 
    double& frontClip, 
    double& backClip,
    bool& enabled ) const; 

  /** \details
    Sets the definition of this Spatial Filter object.

    \param clipBoundary [in]  Clip boundary.
    \param enabled [in]  Enables the clip volume. 
  */
  void setDefinition(
    const OdGiClipBoundary& clipBoundary,
    bool enabled = true); 

  /** \details
    Returns the definition of this Spatial Filter object.

    \param clipBoundary [out]  Clip boundary.
    \param enabled [out]  Receives the status of the clip volume. 
    \param plotting [in]  Must be true if clip boundary will be plotted.
  */
  void getDefinition(
    OdGiClipBoundary& clipBoundary,
    bool& enabled,
    bool plotting = false ) const; 

  OdGeMatrix3d& getClipSpaceToWCSMatrix(OdGeMatrix3d& mat) const;
  OdGeMatrix3d& getOriginalInverseBlockXform(OdGeMatrix3d& mat) const;

  /** \details
    Returns the boundary defintion of this Spatial Filter object.

    \param points [out]  Receives the boundary definition.
  */
  void boundary(
    OdGePoint2dArray& points) const;
    
  /** \details
    Returns the WCS positive extrusion vector of this Spatial Filter object.
  */
  OdGeVector3d normal() const;
  
  /** \details
    Returns the WCS origin of this Spatial Filter object.
  */
  OdGePoint3d origin() const;

  /** \details
    Returns true if and only if front clipping is enabled for this Spatial Filter object.
  */
    bool frontClipEnabled() const;
  
  /** \details
    Returns the front clipping distance for this Spatial Filter object.
  */
    double frontClipDist() const;
  
  /** \details
    Returns true if and only if back clipping is enabled for this Spatial Filter object.
  */
    bool backClipDistEnabled() const;
  /** \details
    Returns the back clipping distance for this Spatial Filter object.
  */
    double backClipDist() const;
  /** \details
    Returns true if and only if the clip volume for this Spatial Filter object is enabled.
    \remarks
    If disabled, this Spatial Filter object includes all of 3D space.
  */
    bool isEnabled() const; 

  /** \details
    Sets the perspective camera position of this Spatial Filter object.

    \param fromPoint [in]  Perspective camera position.
  */
  void setPerspectiveCamera(
    const OdGePoint3d& fromPoint);
  
  /** \details
    Returns true if and only if the specified extents intersect the clip boundary of the Spatial Filter object.
  */
  bool clipVolumeIntersectsExtents(
    const OdGeExtents3d& extents) const;
  
  /** \details
    Returns true if and only if setPerspectiveCamera() has been called for this Spatial Filter object.
  */
  bool hasPerspectiveCamera() const;
  
  virtual OdResult dwgInFields(
    OdDbDwgFiler* pFiler);

  virtual void dwgOutFields(
    OdDbDwgFiler* pFiler) const;

  virtual OdResult dxfInFields(
    OdDbDxfFiler* pFiler);

  virtual void dxfOutFields(
    OdDbDxfFiler* pFiler) const;

  // TD Special :
  bool isFilterInverted() const;
  void setFilterInverted(bool bSet);
  OdResult generateClipBoundaryFromPline(OdDbObjectId id);
};

/** \details
  This template class is a specialization of the OdSmartPtr class for OdDbSpatialFilter object pointers.
*/
typedef OdSmartPtr<OdDbSpatialFilter> OdDbSpatialFilterPtr;

#include "TD_PackPop.h"

#endif // OD_DBSPFILT_H

