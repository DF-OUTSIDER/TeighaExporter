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




#ifndef OD_DBPL_H
#define OD_DBPL_H

#include "TD_PackPush.h"

#include "DbCurve.h"
#include "Db2dPolyline.h"
#include "DwgDeclareMembers.h"

class OdDb2dPolyline;
class OdGeLineSeg2d;
class OdGeLineSeg3d;
class OdGeCircArc2d;
class OdGeCircArc3d;

/** \details
    This class represents Lightweight Polyline entities in an OdDbDatabase instance.

    \sa
    TD_Db

    OdDbPolyline entities differ from OdDb2dPolyline entities as follows:
    * OdDbPolyline entities are stored as single objects, thereby improving
       performance and reducing overhead compared to OdDb2DPolyline objects.
    * Curve fitting and Spline fitting of OdDbPolyline entities are not supported.  
    
    \remarks
    The number of vertices in an OdDbPolyline must be at least two. 
    Polylines with less than two vertices should not left in or added to
    the database.
    
    Since the nth Segment of a Polyline is the segment following the nth Vertex, 
    the segment index and vertex index may be used interchangeably. 
    
    <group OdDb_Classes> 
*/
class DBENT_EXPORT OdDbPolyline : public OdDbCurve
{
public:
  DWGMAP_DECLARE_MEMBERS(OdDbPolyline);

  OdDbPolyline();

  /** \details
    Fills this OdDbPolyline entity with data from the specified OdDb2dPolyline entity.  
    
    \param pSource [in]  Pointer to the source OdDb2dPolyline entity.
    \param transferId [in]  True to do a handOverTo() between the source OdDb2dPolyline 
                   entity and this OdDbPolyline entity.  

    \remarks
    If and only if transferId is true, 
      * This OdDbPolyline entity will be made database resident.
      * It will assume the objectId, handle, extended entity data, extension
         dictionary, and reactors of the source OdDb2dPolyline entity
      * The source OdDb2dPolyline entity will be deleted.
            
    \remarks 
    The source OdDb2dPolyline entity
      * Must be non- database resident.
      * Must by of type k2dSimplePoly or k2dFitCurvePoly.
   
    \returns
    Returns one of the following:
    
    <table>
    Name                   Description
    eOk                    Success
    AlreadyInDb            This entity is database resident
    eIllegalEntityType     *pSource is not an OdDb2dPolyline
    eNotApplicable         *pSource is not k2dSimplePoly or there is extended
                           entity data attached to a vertex.
    </table>
  */
  OdResult convertFrom(
    OdDbEntity* pSource, 
    bool transferId = true);

  /** \details
    Fills the specified OdDb2dPolyline with data from this OdDbPolyline entity.

    \param pDest [in]  Pointer to the destination OdDb2dPolyline entity.
    \param transferId [in]  True to do a handOverTo() between this OdDbPolyline entity 
    and the destination OdDb2dPolyline entity.  

    \remarks
    If and only if transferId is true,
      * The destination OdDb2dPolyline entity will be made database resident.
      * It will assume the objectId, handle, extended entity data, extension
         dictionary, and reactors of this OdDbPolyline entity. 
      * This OdDbPolyline entity will made non- database resident, and may be deleted with the C++ delete operator.

    \returns
    Returns one of the following:
    
    <table>
    Name                   Description
    eOk                    Success
    eIllegalReplacement    This entity is non- database resident.
    eObjectToBeDeleted     This entity is now non- database resident, and should be deleted.
    </table>
  */
  OdResult convertTo(
    OdDb2dPolyline* pDest, 
    bool transferId = true);

  /** \details
    Returns the OCS point of the specified vertex of this Polyline entity (DXF 10).

    \param vertexIndex [in]  Vertex index.
    \param point2d [out]  Receives the OCS point.
  */
  void getPointAt(
    unsigned int vertexIndex, 
    OdGePoint2d& point2d) const;
    
  /** \details
    Returns the WCS point of the specified vertex of this Polyline entity (DXF 10).

    \param vertexIndex [in]  Vertex index.
    \param point3d [out]  Receives the WCS point.
  */
  void getPointAt(
    unsigned int vertexIndex, 
    OdGePoint3d& point3d) const;


  enum SegType
  {
    kLine,        // Straight segment with length > 0.
    kArc,         // Arc segment with length > 0.
    kCoincident,  // Segment with length == 0.
    kPoint,       // Polyline with 1 vertex.
    kEmpty        // Polyline with 0 vertices.
  };

  /** \details
    Returns the type of the specified segment of this Polyline entity.

    \param segmentIndex [in]  Segment index.
    \returns
    Returns one of the following:
    
    <table>
    Name           Description
    kLine          Straight segment with length > 0.0
    kArc           Arc segment with length > 0.0
    kCoincident    Segment with length == 0.0
    kPoint         Polyline with 1 vertex.
    kEmpty         Polyline with 0 vertices.
    </table>
  */
  SegType segType(
    unsigned int segmentIndex) const;

  /** \details
    Returns the specified OCS line segment of this Polyline entity.
    \param segmentIndex [in]  Segment index.
    \param line2d [out]  Receives the OCS line segment.
  */
  void getLineSegAt(
    unsigned int segmentIndex, 
    OdGeLineSeg2d& line2d) const;

  /** \details
    Returns the specified WCS line segment of this Polyline entity.
    \param segmentIndex [in]  Segment index.
    \param line3d [out]  Receives the WCS line segment.
  */
  void getLineSegAt(
    unsigned int segmentIndex, 
    OdGeLineSeg3d& line3d) const;

  /** \details
    Returns the specified OCS arc segment of this Polyline entity.
    \param segmentIndex [in]  Segment index.
    \param arc2d [out]  Receives the OCS arc segment.
  */
  void getArcSegAt(
    unsigned int segmentIndex, 
    OdGeCircArc2d& arc2d) const;

  /** \details
    Returns the specified WCS arc segment of this Polyline entity.
    \param segmentIndex [in]  Segment index.
    \param arc3d [out]  Receives the WCS arc segment.
  */
  void getArcSegAt(
    unsigned int segmentIndex, 
    OdGeCircArc3d& arc3d) const;

  /** \details
    Returns true if and only if the specified OCS point is on the specified segment
    of this Polyline entity,
    and returns the parameter of that point on the segment.
    
    \param segmentIndex [in]  Segment index.
    \param point2d [in]  The OCS point to query.
    \param param [out]  The parameter at that point.
    
    \remarks
    The returned parameter will be in the parametric form of the segment (linear or arc).
  */
  virtual bool onSegAt(
    unsigned int segmentIndex, 
    const OdGePoint2d& point2d, 
    double& param) const;

  /** \details
    Controls the closed state for this Polyline entity (DXF 70, bit 0x01=1).
    
    \param closed [in]  Controls closed.
  */
  void setClosed(
    bool closed);

  /** \details
    Controls the linetype generation for this Polyline entity (DXF 70, bit 0x80).
      
    \remarks
    Linetype generation on indicates that the linetype pattern of this Polyline entity
    is continuously generated around all vertices.
    
    Linetype generation off indicates that the linetype pattern of this Polyline entity is
    restarted at each vertex.
    
    \param plinegen [in]  Controls linetype generation.
  */
  void setPlinegen(
    bool plinegen);

  /** \details
    Sets the elevation of this entity in the OCS (DXF 38).

    \param elevation [in]  Elevation.    

    \remarks
    The elevation is the distance from the WCS origin to the plane of this entity.
  */
  void setElevation(
    double elevation);

  /** \details
    Sets the thickness of this entity (DXF 39).
    \param thickness [in]  Thickness.
    \remarks
    Thickness is the extrusion length along the normal.
  */
  void setThickness(
    double thickness);

  /** \details
    Sets this Polyline entity to a constant width (DXF 43).
    \param constantWidth [in]  Constant width.
  */
  void setConstantWidth(
    double constantWidth);

  /** \details
    Sets the WCS normal to the plane of this entity (DXF 210).
    \param normal [in]  Normal.
  */
  void setNormal(
    const OdGeVector3d& normal);

  /** \details
    Returns true if and only if this Polyline entity consists solely of line segments.
  */
  bool isOnlyLines() const;

  /** \details
    Returns true if and only if linetype generation is on for this Polyline entity (DXF 70, bit 0x80).
      
    \remarks
    Linetype generation on indicates that the linetype pattern of this Polyline entity
    is continuously generated around all vertices.
    
    Linetype generation off indicates that the linetype pattern of this Polyline entity is
    restarted at each vertex.
  */
  bool hasPlinegen() const;

  /** \details
    Returns the elevation of this entity in the OCS (DXF 30).
    
    \remarks
    The elevation is the distance from the WCS origin to the plane of this entity.
  */
  double elevation() const;

  /** \details
    Returns the thickness of this entity (DXF 39).
    
    \remarks
    Thickness is the extrusion length along the normal.
  */
  double thickness() const;

  /** \details
    Returns the constant width for this Polyline entity (DXF 43).
  */
  double getConstantWidth() const;

  /** \details
    Returns the WCS normal to the plane of this entity (DXF 210).
  */
  OdGeVector3d normal() const;

  /** \details
    Inserts a vertex into this Polyline entity at the specified index.

    \param vertexIndex [in]  Vertex index.
    \param point2d [in]  OCS point of the vertex.
    \param bulge [in]  Bulge value for the segment following the vertex.
    \param startWidth [in]  Start width for the segment following the vertex.
    \param endWidth [in]  End width for the segment following the vertex.
    \param vertexIdentifier [in]  
    
    \remarks
    The vertex is inserted before the specified vertex.
    If vertexIndex == numVerts(), the vertex is appended to the Polyline.
    
    Bulge is the tangent of 1/4 the included angle of the arc segment, measured counterclockwise.
  */
  void addVertexAt(
    unsigned int vertexIndex,
    const OdGePoint2d& point2d,
    double bulge = 0.,
    double startWidth = -1.,
    double endWidth = -1.,
    OdInt32 vertexIdentifier = 0);

  /** \details
    Removes the specified vertex from this Polyline entity.
    \param vertexIndex [in]  Vertex index.
  */
  void removeVertexAt(
    unsigned int vertexIndex);

  /** \details
     Returns the number of vertices in this Polyline entity (DXF 90).
  */
  unsigned int numVerts() const;


  /** \details
    Returns the bulge of the specified segment of this Polyline entity.

    \param index [in]  Segment index.
    
    \remarks
    Bulge is the tangent of 1/4 the included angle of the arc segment, measured counterclockwise.
  */
  double getBulgeAt(
    unsigned int index) const;

  /** \details
    Returns the vertex identifier of the specified segment of this Polyline entity.

    \param index [in]  Segment index.
  */
  OdInt32 getVertexIdentifierAt(
    unsigned int index) const;

  /** \details
    Returns the start and end widths for the specified segment of this Polyline entity.

    \param segmentIndex [in]  Segment index.
    \param startWidth [out]  Receives the start width for the vertex.
    \param endWidth [out]  Receives the end width for the vertex.
  */
  void getWidthsAt(
    unsigned int segmentIndex, 
    double& startWidth,  
    double& endWidth) const;

  /** \details
    Sets the OCS point for the specified vertex of this Polyline entity.
    \param vertexIndex [in]  Vertex index.
    \param point2d [in]  OCS point of vertex.
  */
  void setPointAt(
    unsigned int vertexIndex, 
    const OdGePoint2d& point2d);

  /** \details
    Sets the bulge of the specified segment of this Polyline entity.

    \param index [in]  Segment index.
    \param bulge [in]  Bulge.
    
    \remarks
    Bulge is the tangent of 1/4 the included angle of the arc segment, measured counterclockwise.

  */
  void setBulgeAt(
    unsigned int index, 
    double bulge);

  /** \details
    Sets the *vertex identifier* of the specified segment of this Polyline entity.

    \param index [in]  Segment index.
    \param suggestedId [in]  Vertex identifier.
  */
  void setVertexIdentifierAt(
    unsigned int index,
    OdInt32 suggestedId);

  /** \details
    Sets the start and end widths for the specified segment of this Polyline entity.

    \param index [in]  Segment index.
    \param startWidth [in]  Start width for the vertex.
    \param endWidth [in]  End width for the vertex.
  */
  void setWidthsAt(
    unsigned int index, 
    double startWidth, 
    double endWidth);

  /** \details
    Compresses this Polyline entity.
    
    \remarks
    Takes processing time, and should not be used until all edits are complete.
  */
  void minimizeMemory();

  /** \details
    Decompresses this Polyline entity to expedite modifications.
  */
  void maximizeMemory();

  /** \details
    Resets the vertex data for this Polyline entity.

    \param reuse [in]  True to retain vertices.
    \param numVerts [in]  Number of vertices to retain.
      
    \remarks
    If reuse is true, the vertex list will be expanded or truncated 
    such that exactly numVerts vertices exist.
    
    If reuse is false, all vertices will be deleted.
  */
  void reset(
    bool reuse, 
    unsigned int numVerts);

  /** \details
    Returns true if and only if any of the segments in this Polyline entity have non-zero bulges.
  */
  bool hasBulges() const;

  /** \details
    Returns true if and only if any of the segments in this Polyline entity have non-zero vertex identifiers.
  */
  bool hasVertexIdentifiers() const;

  /** \details
    Returns true if and only if any of the segments in this Polyline entity have start and end widths.
  */
  bool hasWidth() const;

  virtual OdResult subGetTransformedCopy(
    const OdGeMatrix3d& mat, OdDbEntityPtr& pCopy) const ODRX_OVERRIDE;

  virtual OdResult subTransformBy(
    const OdGeMatrix3d& xfm) ODRX_OVERRIDE;

  virtual bool subWorldDraw(
    OdGiWorldDraw* pWd) const ODRX_OVERRIDE;

  virtual void subViewportDraw(
    OdGiViewportDraw* pVd) const ODRX_OVERRIDE;

  virtual OdResult dwgInFields(
    OdDbDwgFiler* pFiler) ODRX_OVERRIDE;

  virtual void dwgOutFields(
    OdDbDwgFiler* pFiler) const ODRX_OVERRIDE;

  virtual OdResult dxfInFields(
    OdDbDxfFiler* pFiler) ODRX_OVERRIDE;

  virtual OdResult dxfInFields_R12(
    OdDbDxfFiler* pFiler) ODRX_OVERRIDE;

  virtual void dxfOutFields(
    OdDbDxfFiler* pFiler) const ODRX_OVERRIDE;

  virtual OdResult subGetClassID(
    void* pClsid) const ODRX_OVERRIDE;

  virtual bool isPlanar() const ODRX_OVERRIDE;

  virtual OdResult getPlane(
    OdGePlane& plane, 
    OdDb::Planarity& planarity) const;



  /** \details
      OdDbCurveMethods
  */

  virtual bool isClosed() const;

  virtual bool isPeriodic() const;

  virtual OdResult getStartParam(
    double& startParam) const;

  virtual OdResult getEndParam (
    double& endParam) const;

  virtual OdResult getStartPoint(
    OdGePoint3d& startPoint) const;

  virtual OdResult getEndPoint(
    OdGePoint3d& endPoint) const;

  virtual OdResult getPointAtParam(
    double param, 
    OdGePoint3d& pointOnCurve) const;

  virtual OdResult getParamAtPoint(
    const OdGePoint3d& pointOnCurve, 
    double& param) const;

  virtual OdResult getFirstDeriv (
    double param,
    OdGeVector3d& firstDeriv) const;

  virtual OdResult getSecondDeriv (
    double param,
    OdGeVector3d& secondDeriv) const;

  virtual OdResult getArea(
    double& area) const;

  TD_USING(OdDbCurve::getFirstDeriv);
  TD_USING(OdDbCurve::getSecondDeriv);

  virtual OdResult getDistAtParam( double param, double& dist ) const;

  virtual OdResult getParamAtDist( double dist, double& param ) const;

  virtual OdResult subExplode(
    OdRxObjectPtrArray& entitySet) const;

  virtual OdResult explodeGeometry(
    OdRxObjectPtrArray& entitySet) const;

  virtual OdResult subGetSubentPathsAtGsMarker(OdDb::SubentType type,
                                            OdGsMarker gsMark, 
                                            const OdGePoint3d& pickPoint,
                                            const OdGeMatrix3d& viewXform, 
                                            OdDbFullSubentPathArray& subentPaths, 
                                            const OdDbObjectIdArray* pEntAndInsertStack) const;

  virtual OdResult subGetGsMarkersAtSubentPath( const OdDbFullSubentPath& subPath, 
                                                OdGsMarkerArray& gsMarkers) const;

  virtual OdDbEntityPtr subSubentPtr(const OdDbFullSubentPath& path) const;

  virtual OdResult reverseCurve();

  /** \details
  Returns the entity coordinate system matrix.
  */
  OdGeMatrix3d getEcs( ) const;

  virtual OdResult getOdGeCurve(OdGeCurve3d*& pGeCurve, const OdGeTol& tol = OdGeContext::gTol) const;
  virtual OdResult setFromOdGeCurve(const OdGeCurve3d& geCurve, OdGeVector3d *normal = NULL, const OdGeTol& tol = OdGeContext::gTol);
};

/** \details
    This template class is a specialization of the OdSmartPtr class for OdDbPolyline object pointers.
*/
typedef OdSmartPtr<OdDbPolyline> OdDbPolylinePtr;

#include "TD_PackPop.h"

#endif //OD_DBPL_H

