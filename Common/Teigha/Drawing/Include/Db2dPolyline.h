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




#ifndef _OD_DB_2DPOLYLINE_
#define _OD_DB_2DPOLYLINE_

#include "TD_PackPush.h"

#include "DbCurve.h"
#include "DbObjectIterator.h"
#include "Db2dVertex.h"
#include "DwgDeclareMembers.h"

class OdGePoint3d;
class OdDbSequenceEnd;
template <class T> class OdSmartPtr;

/** \details
    This template class is a specialization of the OdSmartPtr class for OdDbSequenceEnd object pointers.
*/
typedef OdSmartPtr<OdDbSequenceEnd> OdDbSequenceEndPtr;

/** \details
  
    <group TD_Namespaces>
*/
namespace OdDb
{
  enum Poly2dType
  {
    k2dSimplePoly      = 0, // Simple polyline.
    k2dFitCurvePoly    = 1, // Curve fit (DXF 70, bit 0x04).
    k2dQuadSplinePoly  = 2, // Quadratic B-spline fit (DXF 80, bit 0x08; DXF 75 == 5).
    k2dCubicSplinePoly = 3  // Cubic B-spline-fit (DXF 80, bit 0x08; DXF 75 == 6).
  };
}
/** \details
    This class represents 2D Polyline entities in an OdDbDatabase instance.

    \sa
    TD_Db

    <group OdDb_Classes> 
*/
class DBENT_EXPORT OdDb2dPolyline: public OdDbCurve
{
public:

  DWGMAP_DECLARE_MEMBERS(OdDb2dPolyline);

  OdDb2dPolyline();

  /** \details
    Returns the type of this Polyline entity. 
    
    \remarks
    polyType() returns one of the following:
    
    <table>
    Name                        Value   Description
    OdDb::k2dSimplePoly         0       Simple polyline.
    OdDb::k2dFitCurvePoly       1       Curve fit (DXF 70, bit 0x04).
    OdDb::k2dQuadSplinePoly     2       Quadratic B-spline fit (DXF 80, bit 0x08; DXF 75 == 5).
    OdDb::k2dCubicSplinePoly    3       Cubic B-spline-fit (DXF 80, bit 0x08; DXF 75 == 6).
    </table>
  */
  OdDb::Poly2dType polyType() const;

  /** \details
    Sets the type of this Polyline entity. 
      
    \param polyType [in]  Polyline type.
    
    \remarks
    polyType must be one of the following:
    
    <table>
    Name                        Value   Description
    OdDb::k2dSimplePoly         0       Simple polyline.
    OdDb::k2dFitCurvePoly       1       Curve fit (DXF 70, bit 0x04).
    OdDb::k2dQuadSplinePoly     2       Quadratic B-spline fit (DXF 80, bit 0x08; DXF 75 == 5).
    OdDb::k2dCubicSplinePoly    3       Cubic B-spline-fit (DXF 80, bit 0x08; DXF 75 == 6).
    </table>
  */
  void setPolyType(
    OdDb::Poly2dType polyType);

  /** \details
    Sets this Polyline entity closed (DXF 70, bit 0x01=1).
  */
  void makeClosed();

  /** \details
    Sets this Polyline entity open (DXF 70, bit 0x01=0).
  */
  void makeOpen();

  /** \details
    Returns the default segment start width for this Polyline entity (DXF 40).
  */
  virtual double defaultStartWidth() const;

  /** \details
    Sets the default segment start width for this Polyline entity (DXF 40).
    \param defaultStartWidth [in]  Default start width.
  */
  void setDefaultStartWidth(
    double defaultStartWidth);

  /** \details
    Returns the default segment end width for this Polyline entity (DXF 41).
  */
  virtual double defaultEndWidth() const;

  /** \details
    Sets the default segment end width for this Polyline entity (DXF 41).
    \param defaultEndWidth [in]  Default end width.
  */
  void setDefaultEndWidth(
    double defaultEndWidth);

  /** \details
    Returns the thickness of this entity (DXF 39).
    
    \remarks
    Thickness is the extrusion length along the normal.
  */
  double thickness() const;
  
  /** \details
    Sets the thickness of this entity (DXF 39).
    \param thickness [in]  Thickness.
    \remarks
    Thickness is the extrusion length along the normal.
  */
  void setThickness(
    double thickness);

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
    Returns the elevation of this entity in the OCS (DXF 30).
    
    \remarks
    The elevation is the distance from the WCS origin to the plane of this entity.
  */
  double elevation() const;

  /** \details
    Sets the elevation of this entity in the OCS (DXF 30).

    \param elevation [in]  Elevation.    

    \remarks
    The elevation is the distance from the WCS origin to the plane of this entity.
  */
  void setElevation(
    double elevation);

  /** \details
    Returns true if and only if linetype generation is on for this Polyline entity (DXF 70, bit 0x80).
      
    \remarks
    Linetype generation on indicates that the linetype pattern of this Polyline entity
    is continuously generated around all vertices, rather than being restarted at each vertex.
  */
  bool isLinetypeGenerationOn() const;

  /** \details
    Sets the linetype generation on for this Polyline entity (DXF 70, bit 0x80).
      
    \remarks
    Linetype generation on indicates that the linetype pattern of this Polyline entity
    is continuously generated around all vertices, rather than being restarted at each vertex.
  */
  void setLinetypeGenerationOn();

  /** \details
    Sets the linetype generation off for this Polyline entity (DXF 70, bit 0x80).
      
    \remarks
    Linetype generation off indicates that the linetype pattern of this Polyline entity is
    restarted at each vertex, rather than being continuously generated around all vertices.
  */
  void setLinetypeGenerationOff();

  /** \details
    Removes all curve and spline fitting from this Polyline entity.
  
    \remarks
    Removes all but the simple vertices.
  */
  void straighten();

  /** \details
    Appends the specified Vertex entity to this Polyline entity, and makes this Polyline entity its owner.
    
    \returns
    Returns the Object ID of the appended vertex.
    
    If this Polyline entity is database resident, the Vertex entity will be made database resident. 
    
    If this Polyline entity is not database resident, the Vertex entity will be made database resident
    when this Polyline entity is made database resident.
    
    \remarks
    If this Polyline is database resident, the Vertex entity must explicitly be closed when
    appendVertex() returns.
    
    \param pVertex [in]  Pointer to the Vertex entity to be appended.
  */
  OdDbObjectId appendVertex(
    OdDb2dVertex* pVertex);

  /** \details
    Inserts the specified Vertex entity into this Polyline entity
    after the specified vertex, and makes this Polyline its owner.
    
    \returns
    Returns the Object ID of the newly inserted vertex.
    
    The Vertex will be made database resident. 
    
    If this Polyline entity is not database resident, the vertex will be made database resident
    when the polyline is made database resident.
    
    To insert the specified Vertex at the start of this Polyline, use a null indexVertexId.
    
    \remarks
    The Vertex entity must explicitly be closed when insertertex() returns.

    \param indexVertId [in]  Object ID of vertex after which to insert the specified vertex.
    \param pVertex [in]  Pointer to the Vertex entity to be inserted.
  */
  OdDbObjectId insertVertexAt(
    const OdDbObjectId& indexVertId, 
    OdDb2dVertex* pVertex);

  /** \details
    Inserts the specified Vertex entity into this Polyline entity
    after the specified vertex, and makes this Polyline its owner.
    
    \returns
    Returns the Object ID of the newly inserted vertex.
    
    The Vertex will be made database resident. 
    
    If this Polyline entity is not database resident, the vertex will be made database resident
    when the polyline is made database resident.
    
    To insert the specified Vertex at the start of this Polyline, use a null indexVertexId.
    
    \remarks
    The Vertex entity must explicitly be closed when insertertex() returns.
    
    \param pIndexVert [in]  Pointer to vertex after which to insert the specified vertex.
    \param pVertex [in]  Pointer to the Vertex entity to be inserted.
  */
  OdDbObjectId insertVertexAt(
    const OdDb2dVertex* pIndexVert, 
    OdDb2dVertex* pVertex);

  /** \details
    Opens a vertex owned by this Polyline entity.

    \param vertId [in]  Object ID of vertex to be opened.
    \param mode [in]  Mode in which to open the vertex.
    \param openErasedOne [in]  If and only if true, erased objects will be opened.

    \returns
    Returns a SmartPointer to the opened object if successful, otherwise a null SmartPointer.
  */
  OdDb2dVertexPtr openVertex(
    OdDbObjectId vertId, 
    OdDb::OpenMode mode, 
    bool openErasedOne = false);

  /** \details
    Opens the OdDbSequenceEnd entity for this Polyline entity.

    \param mode [in]  Mode in which to open the OdDbSequenceEnd entity.

    \returns
    Returns a SmartPointer to the newly opened OdDbSequenceEnd, or a null SmartPointer.

    \remarks
    This method is provided solely for applications that store XData on
    OdDbSequenceEnd entities; this is not recommended. 
  */
  OdDbSequenceEndPtr openSequenceEnd(
    OdDb::OpenMode mode);

  /** \details
    Returns an Iterator that can be used to traverse the vertices owned 
    by this Polyline entity.
  */
  OdDbObjectIteratorPtr vertexIterator() const;

  /** \details
    Returns the WCS position of the specified Vertex entity.
    
    \param vertex [in]  Vertex.
  */
  OdGePoint3d vertexPosition(
    const OdDb2dVertex& vertex) const;

  virtual bool subWorldDraw(
    OdGiWorldDraw* pWd) const;

  virtual OdResult dwgInFields(
    OdDbDwgFiler* pFiler);

  virtual void dwgOutFields(
    OdDbDwgFiler* pFiler) const;

  virtual OdResult dxfInFields(
    OdDbDxfFiler* pFiler);

  virtual void dxfOutFields(
    OdDbDxfFiler* pFiler) const;

  virtual OdResult dxfInFields_R12(
    OdDbDxfFiler* pFiler);

  virtual void dxfOutFields_R12(
    OdDbDxfFiler* pFiler) const;

  virtual void dxfOut(
    OdDbDxfFiler* pFiler) const;

  virtual OdResult dxfIn(
    OdDbDxfFiler* pFiler);

  virtual void subClose();

  virtual OdResult subExplode(
    OdRxObjectPtrArray& entitySet) const ODRX_OVERRIDE;

  virtual OdResult explodeGeometry(
    OdRxObjectPtrArray& entitySet) const ODRX_OVERRIDE;

  virtual OdResult subTransformBy(
    const OdGeMatrix3d& xfm) ODRX_OVERRIDE;

  virtual OdResult subGetClassID(
    void* pClsid) const ODRX_OVERRIDE;

  virtual bool isPlanar() const;

  virtual OdResult getArea(
    double& area) const;

  virtual OdResult getPlane(
    OdGePlane& plane, 
    OdDb::Planarity& planarity) const;


  // OdDbCurveMethods

  /** \details
  Returns true if 2D-Polyline is closed or false otherwise.
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

  virtual OdResult getDistAtParam( double param, double& dist ) const;

  virtual OdResult getParamAtDist( double dist, double& param ) const;

  virtual OdResult getFirstDeriv (
    double param,
    OdGeVector3d& firstDeriv) const;

  virtual OdResult getSecondDeriv (
    double param,
    OdGeVector3d& secondDeriv) const;

  TD_USING(OdDbCurve::getFirstDeriv);
  TD_USING(OdDbCurve::getSecondDeriv);

  /** \details
  Convert 2D-Polyline to the specified type.

  \param newVal [in]  Polyline type.
    
  \remarks
  newVal must be one of the following:

  <table>
  Name                        Value   Description
  OdDb::k2dSimplePoly         0       Simple polyline.
  OdDb::k2dFitCurvePoly       1       Curve fit (DXF 70, bit 0x04).
  OdDb::k2dQuadSplinePoly     2       Quadratic B-spline fit (DXF 80, bit 0x08; DXF 75 == 5).
  OdDb::k2dCubicSplinePoly    3       Cubic B-spline-fit (DXF 80, bit 0x08; DXF 75 == 6).
  </table>
  */
  OdResult convertToPolyType(OdDb::Poly2dType newVal);
  
  /** \details
  Convert polyline to a smooth curve consisting of pairs of arcs in each pair of vertices.
    
  \returns eOk if successful, or an appropriate error code if not.
  */
  OdResult curveFit();
  
  /** \details
  Transforms all simple vertices to control vertices and creates a new spline-fit vertices based
  on a control vertices, through which the polyline will be drawn.
    
  \returns eOk if successful, or an appropriate error code if not.
  */
  OdResult splineFit();
  
  /** \details
  Transforms all simple vertices to control vertices and creates a new spline-fit vertices based
  on a control vertices, through which the polyline will be drawn.

  \param splineType [in]  Spline-Fit Polyline type.

  \param splineSegs [in]  Number of Spline-Fit segments.
  
  \returns eOk if successful, or an appropriate error code if not.
  */
  OdResult splineFit(OdDb::Poly2dType splineType, OdInt16 splineSegs);

  OdResult subGetSubentPathsAtGsMarker( OdDb::SubentType type,
                                        OdGsMarker gsMark, 
                                        const OdGePoint3d& ,
                                        const OdGeMatrix3d& , 
                                        OdDbFullSubentPathArray& subentPaths, 
                                        const OdDbObjectIdArray* pEntAndInsertStack ) const;
  OdResult subGetGsMarkersAtSubentPath( const OdDbFullSubentPath& subPath, 
                                        OdGsMarkerArray& gsMarkers) const;

  OdDbEntityPtr subSubentPtr(const OdDbFullSubentPath& path) const;

  /** \details
  Returns the entity coordinate system matrix.
  */
  OdGeMatrix3d getEcs( ) const;

  virtual OdResult getOdGeCurve(OdGeCurve3d*& pGeCurve, const OdGeTol& tol = OdGeContext::gTol) const;
  virtual OdResult setFromOdGeCurve(const OdGeCurve3d& geCurve, OdGeVector3d *normal = NULL, const OdGeTol& tol = OdGeContext::gTol);
  
  /** \details
  Reverses the Polyline so that the first vertex become the last vertex of a Polyline 
  and the last vertex become the first vertex.
    
  \returns eOk if successful, or an appropriate error code if not.
  */
  virtual OdResult reverseCurve();

  virtual OdInt32 findVertexIndex(const OdDbVertex* vert) const;
};
/** \details
    This template class is a specialization of the OdSmartPtr class for OdDb2dPolyline object pointers.
*/
typedef OdSmartPtr<OdDb2dPolyline> OdDb2dPolylinePtr;


/*

inline void OdDb2dPolyline::extend(double)
{
  return OdeNotApplicable;
}
*/
#include "TD_PackPop.h"

#endif

