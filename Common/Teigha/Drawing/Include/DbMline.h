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




#ifndef OD_DBMLINE_H
#define OD_DBMLINE_H

#include "TD_PackPush.h"

#include "DbEntity.h"
#include "DwgDeclareMembers.h"
//#include "Ge/GeVoidPointerArray.h"

/** \details
  This struct contains enumerated types and data used by OdDbMline.
  
  <group !!RECORDS_tdrawings_apiref>
*/
struct Mline
{
  typedef OdInt8 MlineJustification;
  enum
  {
    kTop = 0,
    kZero,
    kBottom
  };
  enum
  {
    kOpen = 0,
    kClosed,
    kMerged
  };
};

class OdGePlane;

/** \details
  Corresponding C++ library: TD_Db
  <group !!RECORDS_tdrawings_apiref>
*/
      struct OdMLSegment
{
  OdGeDoubleArray m_AreaFillParams;
  OdGeDoubleArray m_SegParams;
};

typedef OdArray<OdMLSegment> OdMLSegmentArray;

/** \details
    <group OdDb_Classes>

    This class represents multi-line entities in an OdDbDatabase instance.
    
    Corresponding C++ library: TD_Db

    \sa
    <link db_mline.html, Working with Multi-Lines>

    OdDbXline, OdDbLine classes
*/
class DBENT_EXPORT OdDbMline : public OdDbEntity
{
public:
  DWGMAP_DECLARE_MEMBERS(OdDbMline);

  OdDbMline();

  /** \details
    Sets the Object ID of the MLine Style of this MLine entity (DXF 340).
    \param styleId [in]  Object ID of the MLine Style
  */
  void setStyle(
    const OdDbObjectId &styleId);

  /** \details
    Returns the Object ID of the MLine Style of this MLine entity (DXF 340).
  */
  OdDbObjectId style() const;

  /** \details
    Sets the justification of this MLine entity (DXF 70).
    \param justification [in]  Justification.
  */
  void setJustification(Mline::MlineJustification justification);

  /** \details
    Returns the justification of this MLine entity (DXF 70).
  */
  Mline::MlineJustification justification() const;

  /** \details
    Sets the scale of this entity (DXF 40).
  */
  void setScale(
    double scale);

  /** \details
    Returns the scale of this entity (DXF 40).
  */
  double scale() const;

  /** \details
    Returns the WCS normal to the plane of this entity (DXF 210).
  */
  OdGeVector3d normal() const;
  
  /** \details
    Sets the WCS normal to the plane of this entity (DXF 210).
    \param normal [in]  Normal.
  */
  void setNormal(
    const OdGeVector3d& normal);

  /** \details
    Appends a vertex onto the end of this MLine entity.

    \param newVertex [in]  Vertex to append.
    
    \remarks
    newVertex is projected onto the plane of this MLine entity, and this projected vertex
    appended to it.     
  */
  void appendSeg(
    const OdGePoint3d& newVertex);

  /** \details
    Returns and removes the last vertex from this MLine entity.

    \param lastVertex [out]  Receives the last vertex.
  */
  void removeLastSeg
    (OdGePoint3d& lastVertex);

  /** \details
    Moves the specified vertex of this MLine entity.

    \param vertexIndex [in]  Vertex index.
    \param newPosition [in]  New WCS position for specified vertex.

    \remarks
    newPosition is projected onto the plane of this MLine entity, and this projected vertex
    replaces the specified vertex.     
  */
  void moveVertexAt(
    int vertexIndex, 
    const OdGePoint3d& newPosition);

  /** \details
    Controls the closed status of this MLine entity (DXF 71, bit 0x02).
    
    \param closedMline [in]  Sets this MLine closed if true, open if false.
  */
  void setClosedMline(
    bool closedMline);

  /** \details
    Returns the closed status of this MLine entity(DXF 71, bit 0x02).
    \returns
    Returns true if and only if this MLine entity is closed.
  */
  bool closedMline() const;

  /** \details
    Sets the "Suppress Start Caps" status of this MLine entity (DXF 71, bit 0x04).

    \param suppressIt [in]  Suppresses start caps if true, enables start caps if false.
  */
  void setSupressStartCaps(
    bool supressIt);

  /** \details
    Returns the "Suppress Start Caps" status of this MLine entity (DXF 71, bit 0x04).

    \returns
    Returns true if and only if start caps are suppressed.
  */
  bool supressStartCaps() const;

  /** \details
    Sets the "Suppress End Caps" status of this MLine entity (DXF 71, bit 0x04).

    \param suppressIt [in]  Suppresses end caps if true, enables end caps if false.
  */
  void setSupressEndCaps(
    bool supressIt);

  /** \details
    Returns the "Suppress End Caps" status of this MLine entity (DXF 71, bit 0x04).

    \returns
    Returns true if and only if end caps are suppressed.
  */
  bool supressEndCaps() const;

  /** \details
    Returns the number of vertices in this MLine entity (DXF 72).
  */
  int numVertices() const;

  /** \details
    Returns the specified vertex of this MLine entity (DXF 10 or 11).
    \param vertexIndex [in]  Vertex index.
  */
  OdGePoint3d vertexAt(
    int vertexIndex) const;

  /** \details
    Returns the direction vector of the segment starting at the specified vertex (DXF 12).
    \param vertexIndex [in]  Vertex index.
  */
  OdGeVector3d axisAt(
    int vertexIndex) const;

  /** \details
    Returns the direction vector of the miter starting at the specified vertex (DXF 13).
    \param vertexIndex [in]  Vertex index.
  */
  OdGeVector3d miterAt(
    int vertexIndex) const;

  /** \details
    Returns the element and area fill parameters at the specified vertex (DXF 41 and 42).
    \param vertexIndex [in]  Vertex index.
    \param params [out]  Receives the element and area fill parameters.
  */
  void getParametersAt(int index, OdMLSegmentArray& params) const;

  /** \details
    Sets the element and area fill parameters at the specified vertex (DXF 41 and 42).
    \param vertexIndex [in]  Vertex index.
    \param params [in]  Element and area fill parameters.
  */
  void setParametersAt(
    int vertexIndex, 
    const OdMLSegmentArray& params);

  /** \details
    Attempts to find which element of the Mline object contains the point.
    \param pt [in]  Point to find intersection with Mline elements.
    \remarks
    If an element is not found that directly contains the point and the Mline object is not closed,
    then the ends of the MLine object are virtually extended to see if point lies in the path of any
    element. 
    If successful, the index number of the element that contains point is returned.
    If not successful, then -1 is returned.
  */
  int element(const OdGePoint3d &pt) const;

  /** \details
    Finds closest point to this Mline object.
    \param pt [in]  Input point.
    \param ptOut [out]  Output point.
    \param bExtend [in]  Virtually extends Mline geometry.
    \param bExcludeCaps [in]  Excludes Mline start/end caps and joints.
  */
  OdResult getClosestPointTo(const OdGePoint3d &pt, OdGePoint3d& ptOut, bool bExtend, bool bExcludeCaps = false) const;

  /** \details
    Finds closest point to this Mline object projected on specified plane.
    \param pt [in]  Input point.
    \param norm [in]  Plane normal.
    \param ptOut [out]  Output point.
    \param bExtend [in]  Virtually extends Mline geometry.
    \param bExcludeCaps [in]  Excludes Mline start/end caps and joints.
  */
  OdResult getClosestPointTo(const OdGePoint3d &pt, const OdGeVector3d &norm, OdGePoint3d& ptOut, bool bExtend, bool bExcludeCaps = false) const;

  void getPlane(OdGePlane &plane) const;

  virtual OdResult dwgInFields(
    OdDbDwgFiler* pFiler);

  virtual void dwgOutFields(
    OdDbDwgFiler* pFiler) const;

  virtual OdResult dxfInFields(
    OdDbDxfFiler* pFiler);

  virtual void dxfOutFields(
    OdDbDxfFiler* pFiler) const;

  virtual OdResult subTransformBy(
    const OdGeMatrix3d& xfm);

  virtual bool subWorldDraw(
    OdGiWorldDraw* pWd) const;

  virtual OdResult subExplode(
    OdRxObjectPtrArray& entitySet) const; /* Replace OdRxObjectPtrArray */

  virtual OdResult explodeGeometry(OdRxObjectPtrArray& entitySet) const;

  virtual void subClose();

  virtual OdResult subGetClassID(
    void* pClsid) const;

  virtual OdResult subGetTransformedCopy(
    const OdGeMatrix3d& mat, OdDbEntityPtr& pCopy) const;

  virtual void subSetDatabaseDefaults(OdDbDatabase * pDb, bool doSubents);

  virtual OdResult subGetSubentPathsAtGsMarker(OdDb::SubentType type,
                                               OdGsMarker gsMark,
                                               const OdGePoint3d& ,
                                               const OdGeMatrix3d& ,
                                               OdDbFullSubentPathArray& subentPaths,
                                               const OdDbObjectIdArray* pEntAndInsertStack) const;

  virtual OdResult subGetGsMarkersAtSubentPath(const OdDbFullSubentPath& subPath,
                                               OdGsMarkerArray& gsMarkers) const;
};

/** \details
  This template class is a specialization of the OdSmartPtr class for OdDbMline object pointers.
*/
typedef OdSmartPtr<OdDbMline> OdDbMlinePtr;

#include "TD_PackPop.h"

#endif

