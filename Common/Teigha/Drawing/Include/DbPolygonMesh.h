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




#ifndef _OD_DB_POLYGON_MESH_
#define _OD_DB_POLYGON_MESH_

#include "TD_PackPush.h"

#include "DbEntity.h"
#include "DbObjectIterator.h"
#include "DbPolygonMeshVertex.h"
#include "DwgDeclareMembers.h"

class OdDbSequenceEnd;
typedef OdSmartPtr<OdDbPolygonMeshVertex> OdDbPolygonMeshVertexPtr;
typedef OdSmartPtr<OdDbSequenceEnd> OdDbSequenceEndPtr;

/** \details
  
    <group TD_Namespaces>
*/
namespace OdDb
{
  enum PolyMeshType
  {
    kSimpleMesh        = 0, // Simple mesh
    kQuadSurfaceMesh   = 5, // Quadratic B-spline fit
    kCubicSurfaceMesh  = 6, // Cubic B-spline fit
    kBezierSurfaceMesh = 8  // Bezier Surface fit
  };
}

/** \details
    This class represents PolygonMesh entities in an OdDbDatabase instance.

    \sa
    TD_Db

    \remarks
    A PolygonMesh entity consists of a list of PolygonMeshVertex (coordinate) vertices
    describing a M x N array of vertices, defining a set of 3D faces.   
    
    M is the number vertices in a row, N is the number of vertices in a column.
    The first N vertices define the first column, the second N, the second column, etc.

    The mesh may be closed in the M and/or N directions. A closed mesh is connected from the last row
    or column to the first.

    \remarks
    Never derive from this class.

    <group OdDb_Classes>
*/
class DBENT_EXPORT OdDbPolygonMesh : public OdDbEntity
{
public:
  DWGMAP_DECLARE_MEMBERS(OdDbPolygonMesh);

  OdDbPolygonMesh();
  
  /** \details
    Returns the mesh type of this PolygonMesh entity (DXF 75).
    
    \remarks
    polyMeshType() returns one of the following:
      
    <table>
    Name                       Value    Description
    OdDb::kSimpleMesh          0        Simple Mesh
    OdDb::kQuadSurfaceMesh     5        Quadratic B-spline fitting
    OdDb::kCubicSurfaceMesh    6        Cubic B-spline fitting
    OdDb::kBezierSurfaceMesh   8        Bezier Surface fitting
    </table>
  */
  OdDb::PolyMeshType polyMeshType() const;

  /** \details
    Sets the mesh type of this PolygonMesh entity (DXF 75).

    \param polyMeshType [in]  PolygonMesh type.
    
    \remarks
    polyMeshType must be one of the following:
      
    <table>
    Name                      Value    Description
    OdDb::kSimpleMesh          0        Simple Mesh
    OdDb::kQuadSurfaceMesh     5        Quadratic B-spline fitting
    OdDb::kCubicSurfaceMesh    6        Cubic B-spline fitting
    OdDb::kBezierSurfaceMesh   8        Bezier Surface fitting
    </table>
  */
  void setPolyMeshType(
    OdDb::PolyMeshType polyMeshType);

  /** \details
    Uses surfaceFit() to convert mesh type of this PolygonMesh entity (DXF 75).

    \param polyMeshType [in]  PolygonMesh type.
    
    \remarks
    polyMeshType must be one of the following:
      
    <table>
    Name                      Value    Description
    OdDb::kSimpleMesh          0        Simple Mesh
    OdDb::kQuadSurfaceMesh     5        Quadratic B-spline fitting
    OdDb::kCubicSurfaceMesh    6        Cubic B-spline fitting
    OdDb::kBezierSurfaceMesh   8        Bezier Surface fitting
    </table>
  */
  void convertToPolyMeshType(
    OdDb::PolyMeshType polyMeshType);
  
  /** \details
    Returns the number of vertices in the M direction for this PolygonMesh entity (DXF 71).
  */
  OdInt16 mSize() const;

  /** \details
    Sets the number of vertices in the M direction for this PolygonMesh entity (DXF 71).
    \param mSize [in]  Number of vertices in M direction.  
  */
  void setMSize(
    OdInt16 mSize);
  
  /** \details
    Returns the number of vertices in the N direction for this PolygonMesh entity (DXF 72).
  */
  OdInt16 nSize() const;

  /** \details
    Sets the number of vertices in the N direction for this PolygonMesh entity (DXF 72).
    \param nSize [in]  Number of vertices in N direction.  
  */
  void setNSize(OdInt16 nSize);
  
  /** \details
    Returns true if and only if this PolygonMesh entity is closed in the M direction (DXF 70, bit 0x01).
  */
  bool isMClosed() const;

  /** \details
    Sets this PolygonMesh entity closed in the M direction (DXF 70, bit 0x01).
  */
  void makeMClosed();

  /** \details
    Sets this PolygonMesh entity opened in the M direction (DXF 70, bit 0x01).
  */
  void makeMOpen();
  
  /** \details
    Returns true if and only if this PolygonMesh entity is closed in the N direction (DXF 70, bit 0x20).
  */
  bool isNClosed() const;

  /** \details
    Sets this PolygonMesh entity closed in the N direction (DXF 70, bit 0x20).
  */
  void makeNClosed();

  /** \details
    Sets this PolygonMesh entity opened in the N direction (DXF 70, bit 0x20).
  */
  void makeNOpen();
  
  /** \details
    Returns the M surface density for this PolygonMesh entity (DXF 73).
    
    \remarks
    This is the number of vertices in the M direction after a surfaceFit.
     
    Used instead of M if polyMeshType() != OdDb::kSimpleMesh.
  */
  OdInt16 mSurfaceDensity() const;

  /** \details
    Sets the M surface density for this PolygonMesh entity (DXF 73).
    \param mSurfaceDensity [in]  M surface density. 
      
    \remarks
    This is the number of vertices in the M direction after a surfaceFit.
    
    Used instead of M if polyMeshType() != OdDb::kSimpleMesh.
  */
  void setMSurfaceDensity(
    OdInt16 mSurfaceDensity);
  
  /** \details
    Returns the N surface density for this PolygonMesh entity (DXF 74).
    
    \remarks
    This is the number of vertices in the N direction after a surfaceFit. 
    
    Used instead of N if polyMeshType() != OdDb::kSimpleMesh.
  */
  OdInt16 nSurfaceDensity() const;

  /** \details
    Sets the M surface density for this PolygonMesh entity (DXF 74).
    \param nSurfaceDensity [in]  N surface density. 
      
    \remarks
    This is the number of vertices in the N direction after a surfaceFit.
    
    Used instead of N if polyMeshType() != OdDb::kSimpleMesh.
  */
  void setNSurfaceDensity(
    OdInt16 nSurfaceDensity);
  
  /** \details
    Removes all the surface fit vertices for this PolygonMesh entity.
  */
  void straighten();

  /** \details
    Surface fits a smooth surface to this PolygonMesh entity.
  */
  void surfaceFit();

  /** \details
    Surface fits a smooth surface to this PolygonMesh entity.
    \param surfType [in]  Surface type (overrides SURFTYPE system variable).
    \param surfU [in]  M Surface density (overrides SURFU system variable).
    \param surfV [in]  N Surface density (overrides SURFV system variable.
    \remarks
    surfU and surfV will be in the range [2..200].

    \remarks
    surfType must be one of the following:
      
    <table>
    Name                       Value    Description
    OdDb::kQuadSurfaceMesh     5        Quadratic B-spline fitting
    OdDb::kCubicSurfaceMesh    6        Cubic B-spline fitting
    OdDb::kBezierSurfaceMesh   8        Bezier Surface fitting
    </table>
  */
  void surfaceFit(
    OdDb::PolyMeshType surfType, 
    OdInt16 surfU, 
    OdInt16 surfV);
  
   /** \details
    Appends the specified vertex onto this PolygonMesh entity.

    \param pVertex [in]  Pointer to the vertex to append.
    \param vType [in]  Vertex type.
    
    \returns
    Returns the Object ID of the appended vertex.
    
    vType must be one of the following:
    
    <table>
    Name                     Value   DXF 70   Description
    OdDb::k3dSimpleVertex    0       0x40     Standard vertex.
    OdDb::k3dControlVertex   1       0x10     Spline-fit or curve-fit control point.
    OdDb::k3dFitVertex       2       0x08     Spline-fit or curve-fit generated vertex.
    </table>
   */
  OdDbObjectId appendVertex(
    OdDbPolygonMeshVertex* pVertex, 
    OdDb::Vertex3dType vType = OdDb::k3dSimpleVertex);
  
  /** \details
    Opens a vertex owned by this PolygonMesh entity.

    \param vertId [in]  Object ID of vertex to be opened.
    \param mode [in]  Mode in which to open the vertex.
    \param openErasedOne [in]  If and only if true, erased objects will be opened.

    \returns
    Returns a SmartPointer to the opened object if successful, otherwise a null SmartPointer.
  */
  OdDbPolygonMeshVertexPtr openVertex(
    OdDbObjectId vertId, 
    OdDb::OpenMode mode, 
    bool openErasedOne = false);
  
  /** \details
    Opens the OdDbSequenceEnd entity for this PolygonMesh entity.

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
    by this PolygonMesh entity.
  */
  OdDbObjectIteratorPtr vertexIterator() const;
  
  virtual OdResult dwgInFields(
    OdDbDwgFiler* pFiler);

  virtual void dwgOutFields(
    OdDbDwgFiler* pFiler) const;

  virtual OdResult dxfInFields(
    OdDbDxfFiler* pFiler);

  virtual void dxfOutFields(
    OdDbDxfFiler* pFiler) const;

  virtual void dxfOut(
    OdDbDxfFiler* pFiler) const;

  virtual OdResult dxfIn(
    OdDbDxfFiler* pFiler);

  virtual OdResult dxfInFields_R12(
    OdDbDxfFiler* pFiler);

  virtual void dxfOutFields_R12(
    OdDbDxfFiler* pFiler) const;

  bool subWorldDraw(
    OdGiWorldDraw* pWd) const;

  void subClose();

  OdResult subGetClassID(
    void* pClsid) const;

  virtual bool isPlanar() const;
  
  virtual OdResult getPlane(
    OdGePlane& plane, 
    OdDb::Planarity& planarity) const;

  virtual OdResult subTransformBy(
    const OdGeMatrix3d& xfm);

  /** \remarks
    Creates and returns a set of OdDbFace entities.
  */
  virtual OdResult subExplode(
    OdRxObjectPtrArray& entitySet) const;

  /** \remarks
    Creates and returns a set of OdDbFace entities.
  */
  virtual OdResult explodeGeometry(OdRxObjectPtrArray& entitySet) const;

  OdResult subGetSubentPathsAtGsMarker( OdDb::SubentType type,
                                        OdGsMarker gsMark, 
                                        const OdGePoint3d& ,
                                        const OdGeMatrix3d& , 
                                        OdDbFullSubentPathArray& subentPaths, 
                                        const OdDbObjectIdArray* pEntAndInsertStack ) const;

  OdResult subGetGsMarkersAtSubentPath( const OdDbFullSubentPath& subPath, 
                                        OdGsMarkerArray& gsMarkers) const;
};

/** \details
    This template class is a specialization of the OdSmartPtr class for OdDbPolygonMesh object pointers.
*/
typedef OdSmartPtr<OdDbPolygonMesh> OdDbPolygonMeshPtr;

#include "TD_PackPop.h"

#endif
