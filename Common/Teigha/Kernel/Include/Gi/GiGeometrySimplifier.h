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

#ifndef __ODGIGEOMETRYSIMPLIFIER_H__
#define __ODGIGEOMETRYSIMPLIFIER_H__

#include "Gi/GiFill.h"
#include "Gi/GiMapperItem.h"
#include "Gi/GiExport.h"
#include "Gi/GiConveyorGeometry.h"
#include "Ge/GeDoubleArray.h"
#include "Ge/GePoint2dArray.h"
#include "Ge/GePoint3dArray.h"
#include "Ge/GeVector3dArray.h"
#include "Gs/Gs.h"

class OdGiDeviation;

class OdGeLineSeg2d;
class OdGeLine2d;
class OdGeCircArc2d;
class OdGeCircArc3d;
class OdGeEllipArc3d;

#include "TD_PackPush.h"

/** \details
    This class provides tessellation functionality for the Teigha vectorization framework.
    
    \remarks
    Tessellation consists of breaking complex entities into sets of simpler entities.  
    
    An instance of this class is used to simplify the geometry produced by the Teigha vectorization
    framework.

    <group OdGi_Classes> 
*/
class ODGI_EXPORT OdGiGeometrySimplifier : public OdGiConveyorGeometry
{
  // Old version do not override !!!
  // Overide shellFaceOut() or triangleOut() instead
  virtual int polygonWithHolesOut(OdUInt32, const OdGePoint3d*, OdUInt32 , OdUInt32*);
protected:
  /** \details
    Draws an arc of the specified type.
    \param arcType [in]  Arc type.
    \param center [in]  Center of arc.
    \param points [out]  Receives an array of points comprising the arc.
    \param pNormal [in]  Pointer to the normal to the arc.
    \param pExtrusion [in]  Pointer to the extrusion direction of the arc.
  */
  void drawTypedArc(OdGiArcType arcType, const OdGePoint3d& center,
    OdGePoint3dArray& points, const OdGeVector3d* pNormal, const OdGeVector3d* pExtrusion);
protected:
  OdGiConveyorContext*  m_pDrawCtx;
  OdGiSubEntityTraits*  m_pTraits;
  OdGsMarker            m_baseSubEntMarker;

  enum SimplifierFlags
  {
    kSimplProcessingText   = (1 << 0), // True if simplifier is inside TrueType text processing.
    kSimplFillModeDisabled = (1 << 1), // True if shell/mesh primitives must be filled during vectorization.
    kSimplEyeSpaceSilhs    = (1 << 2), // Enables silhouettes processing in ECS (Eye Space).
    kSimplLastFlag         = kSimplEyeSpaceSilhs
  };
  OdUInt32              m_simplFlags;
private:
  OdInt32               m_nVertexCount;
  const OdGePoint3d*    m_pVertexList;
  const OdGiVertexData* m_pVertexData;
  OdGePoint2dArray      m_loopsPnts;
  OdGePoint3dArray      m_points3d1; // for shellProc/plineProc level
  OdGePoint3dArray      m_points3d2; // for triangleOut/polylineOut level
protected:
  const OdGiDeviation*  m_pDeviation;
  OdGeDoubleArray       m_deviations;
  OdGsView::RenderMode  m_renderMode;

  /** \details
    Returns the recommended maximum deviation of the current vectorization 
    for the specified point on the curve or surface being tesselated.
    \param deviationType [in]  Deviation type.
    \param pointOnCurve [in]  Point on the curve.
  */
  double deviation(const OdGiDeviationType deviationType, const OdGePoint3d& pointOnCurve) const;

  /** \details
    Returns the FillMode and DrawEdges for this Vectorizer object.
    \param drawEdges [out]  Receives the DrawEdges value.
    \remarks
    As implemented, this function returns false if called without arguments
  */
  bool fillMode();
  bool fillMode(bool& drawEdges);
  bool fillMode(bool& drawEdges, bool& secColor);
  bool circleArcFillMode();
  bool shmFillMode(bool& drawEdges);
  bool shmFillMode(bool& drawEdges, bool& secColor);

  /** \details
    Returns true if and only if this Vectorizer object is processing TrueType text.
  */
  bool processingText() const { return GETBIT(m_simplFlags, kSimplProcessingText); }

  /** \details
    Returns true if silhouettes processing in ECS (Eye Coordinates Space) enabled.
  */
  bool silhouettesProcessingInEyeSpace() const { return GETBIT(m_simplFlags, kSimplEyeSpaceSilhs); }
  /** \details
    Enables processing silhouettes in ECS (Eye Coordinates Space).
  */
  void enableSilhouettesProcessingInEyeSpace(bool bEnable) { SETBIT(m_simplFlags, kSimplEyeSpaceSilhs, bEnable); }

  /** \details
    Returns the OdGiContext for this Vectorizer object.
  */
  OdGiContext& giCtx() const { return const_cast<OdGiContext&>(m_pDrawCtx->giContext()); }

  /** \details
    Controls plineProc arc segment output.
    \param drawContextFlags [in]  Input conveyor context draw flags.
    \returns
    Returns true if plineProc arc segments as arc output enabled.
    Returns false to output plineProc arc segments as polyline segments chain.
  */
  virtual bool plineArcSegmentsAsArcProc(OdUInt32 drawContextFlags) const;
public:
  OdGiGeometrySimplifier();
  ~OdGiGeometrySimplifier();

  /** \details
      Specifies the maximum deviation allowed during the 
      tessellation process.  
      \remarks
      This value sets the limit for the maximum difference 
      between the actual curve or surface, and the tessellated curve or surface.

      \param deviations [in]  Array of deviation values.
      \remarks
      The deviation values (in isotropic world space) are  
      the deviation types defined by the OdGiDeviationType enum.

      Each OdGiDeviationType value is used as an index into this array.
  */
  void setDeviation(const OdGeDoubleArray& deviations);

  /** \details
    Specifies the maximum deviation allowed during the 
    tessellation process. 

    \param pDeviation [in]  Deviation to be used for anisotropic space (perspective view).
  */
  void setDeviation(const OdGiDeviation* pDeviation);

  /** \details
    Returns the render mode for this object.
       
    \remarks
    renderMode() returns one of the following:
    
    <table>
    Name                            Value      Description
    kBoundingBox                    -1         Bounding box. For internal use only.
    k2DOptimized                    0          Standard display. Optimized for 2D.
    kWireframe                      1          Standard display. Uses 3D pipeline.
    kHiddenLine                     2          Wireframe display. Hidden lines removed.
    kFlatShaded                     3          Faceted display. One color per face.
    kGouraudShaded                  4          Smooth shaded display. Colors interpolated between vertices.
    kFlatShadedWithWireframe        5          Faceted display with wireframe overlay.
    kGouraudShadedWithWireframe     6          Smooth shaded display with wireframe overlay.
    </table>
  */
  OdGsView::RenderMode renderMode() { return m_renderMode; }

  /** \details
     Sets the OdGiConveyorContext for this object.  
     \param pDrawCtx [in]  Pointer to the draw context.
     \remarks
     The draw context must be set before 
     calling any of the tessellation functions.
  */
  void setDrawContext(OdGiConveyorContext* pDrawCtx);

  /** \details
    Returns the OdGiConveyorContext associated with this Vectorizer object.
  */
  OdGiConveyorContext* drawContext( ) { return m_pDrawCtx; }
  const OdGiConveyorContext* drawContext( ) const { return m_pDrawCtx; }

  /** \details
    Passes polyline data to this Vectorizer object.
    \remarks
    This function is called by the OdGiGeometrySimplifier class to 
    pass polyline data generated during tessellation, to a client application.

    Client applications have the option of overriding this function to process 
    the data themselves.  If the function is not overridden, 
    the default implementation of this function calls polylineOut with the vertex list.

    \param numPoints [in]  Number of points in the polyline.
    \param vertexList [in]  Array of vertices that make up the polyline.
  */
  virtual void polylineOut(OdInt32 numPoints, const OdGePoint3d* vertexList);

  /** \details
      Passes polygon data to this Vectorizer object.
      \remarks
      This function is called by the OdGiGeometrySimplifier class to 
      pass polygon data generated during tessellation, to a client application.
      Clients should override this function.

      \param numPoints [in]  Number of points in the polygon.
      \param vertexList [in]  Array of vertices that make up the polygon.
      \param pNormal [in]  Normal vector for the polygon.
  */
  virtual void polygonOut(OdInt32 numPoints, 
    const OdGePoint3d* vertexList, 
    const OdGeVector3d* pNormal = 0);

  /** \details
      Sets vertex data for tessilation of meshes and shells.
      \remarks
      Called by the default implementations of meshProc and shellProc, to 
      set vertex data that will be used in the tessellation of these objects.

      \param numVertices [in]  Number of vertices in the mesh or shell.
      \param vertexList [in]  Array of vertices that make up the mesh or shell.
      \param pVertexData [in]  Pointer to additional vertex data. 
          
      \sa
      OdGiVertexData, Vectorization Primatives
  */
  void setVertexData(OdInt32 numVertices,
    const OdGePoint3d* vertexList,
    const OdGiVertexData* pVertexData = 0);

  /** \details
      Returns the vertex count set in the most recent call to setVertexData.
  */
  OdInt32 vertexDataCount() const
  {
    return m_nVertexCount;
  }

  /** \details
      Returns the vertex list set in the most recent call to setVertexData.
  */
  const OdGePoint3d* vertexDataList() const
  {
    return m_pVertexList;
  }

  /** \details
      Returns the vertex attribute data set in the most recent call to setVertexData.
  */
  const OdGiVertexData* vertexData() const
  {
    return m_pVertexData;
  }

  /** \details
      Returns base subentity selection marker for polylineOut().
  */
  OdGsMarker baseSubEntMarker() const
  {
    return m_baseSubEntMarker;
  }

  /** \remarks
      Client applications have the option of overriding this function to process the polyline
      data themselves.  If the function is not overridden, the default implementation of 
      this function processes the polyline, taking into account the following:

        * The pXfm transformation (if any) that must be applied to the polyline.
        * Extrusion of the polyline (if it has a non-zero thickness).
        * Creating the closing segment if the polyline is closed.
        * Start and end widths for each segment, or constant width, if applicable.
        * Arc segments (if bulge values are present).

      In the default implementation of this function, line segments with a non-zero width 
      will be converted into calls to shellProc, and segments with no width will generate
      calls to polylineProc.  Arc segments with non-zero width will be converted into 
      calls to polygonProc, and arc segments with no width will generate calls to 
      polylineProc.      

      OdGiGeometrySimplifier::plineProc is only called for polylines with a continuous
      linetype. 
  */
  virtual void plineProc(const OdGiPolyline& polyline,
    const OdGeMatrix3d* pXfm = 0,
    OdUInt32 fromIndex = 0,
    OdUInt32 numSegs = 0);

  /** \remarks
      Client applications have the option of overriding this function to process the polyline
      data themselves.  If the function is not overridden, the default implementation
      of this function processes the polyline, calling polylineOut if the data is not 
      extruded, or meshProc if an extrusion value is present.

      OdGiGeometrySimplifier::polylineProc is only called for polylines with a continuous
      linetype. 
  */
  virtual void polylineProc(OdInt32 numPoints, 
    const OdGePoint3d* vertexList,
    const OdGeVector3d* pNormal = 0, 
    const OdGeVector3d* pExtrusion = 0,
    OdGsMarker baseSubEntMarker = -1);

  /** \remarks
      Client applications have the option of overriding this function to process the polygon
      data themselves.  If the function is not overridden, the default implementation
      of this function processes the polygon, calling polygonOut if the data is not 
      extruded, or shellProc if an extrusion value is present.
  */
  virtual void polygonProc(OdInt32 numPoints, 
    const OdGePoint3d* vertexList,
    const OdGeVector3d* pNormal = 0, 
    const OdGeVector3d* pExtrusion = 0);

  /** \remarks
     This function should not be overridden by clients. 
  */
  virtual void xlineProc(const OdGePoint3d& firstPoint, const OdGePoint3d& secondPoint);

  /** \remarks
      This function should not be overridden by clients. 
  */
  virtual void rayProc(const OdGePoint3d& basePoint, const OdGePoint3d& throughPoint);

  /** \remarks
      Client applications have the option of overriding this function to process the mesh
      data themselves.  If the function is not overridden, the default implementation
      of this function processes the mesh into a set of facets.  More specifically,
      the default meshProc function calls setVertexData, and the calls either
      generateMeshFaces for filled facets, or generateMeshWires for a wireframe.
  */
  virtual void meshProc(OdInt32 numRows,
    OdInt32 numColumns,
    const OdGePoint3d* vertexList,
    const OdGiEdgeData* pEdgeData = 0,
    const OdGiFaceData* pFaceData = 0,
    const OdGiVertexData* pVertexData = 0);

  /** \details
    Tessellates a wireframe mesh.
    \remarks
    This function is called from the default implementation of meshProc
    to tessellate a wireframe mesh. Vertex data needed for this function
    can be obtained by calling vertexDataList and vertexData.

    Client applications have the option of overriding this function to process the mesh
    data themselves.  If the function is not overridden, the default implementation
    of this function processes the mesh into a set of facets.  More specifically,
    the default generateMeshWires function processes mesh attributes, and calls
    polylineOut with the tessellated mesh data.

    \param numRows [in]  Number of rows in the mesh.
    \param numColumns [in]  Number of columns in the mesh.
    \param pEdgeData [in]  Pointer to additional edge data.
    \param pFaceData [in]  Pointer to additional face data.

    \sa
    OdGiEdgeData, OdGiFaceData
  */
  virtual void generateMeshWires(OdInt32 numRows, 
    OdInt32 numColumns,
    const OdGiEdgeData* pEdgeData,
    const OdGiFaceData* pFaceData);

  /** \details
    Tessellates a filled mesh.
    \remarks
    This function is called from the default implementation of meshProc
    to tessellate a filled mesh. Vertex data needed for this function
    can be obtained by calling vertexDataList and vertexData.

    Client applications have the option of overriding this function to process the mesh
    data themselves.  If the function is not overridden, the default implementation
    of this function processes the mesh into a set of facets.  More specifically,
    the default generateMeshFaces function processes mesh attributes, and calls
    triangleOut function with the tessellated mesh data.

      \param numRows [in]  Number of rows in the mesh.
      \param numColumns [in]  Number of columns in the mesh.
      \param pFaceData [in]  Pointer to additional face data.

    \sa
      OdGiEdgeData, OdGiFaceData
  */
  virtual void generateMeshFaces(OdInt32 numRows, OdInt32 numColumns,
                                 const OdGiFaceData* pFaceData);

  /** \details
    Outputs a face of a tessilated mesh.
    \param faceList [in]  Array of numbers that define the face.
    \param pNormal [in]  Pointer to the normal to the face.
    \sa
    Vectorization Primitives
  */
  virtual void meshFaceOut(const OdInt32* faceList, const OdGeVector3d* pNormal);


    /** \details
      BIM-specific version of shellProc which also generates fill pattern data
      for shell faces.

      \param vertexList [in]  OdGePoint3d Array which represents vertices.
      \param faceListSize [in]  Number of entries in faceList.
      \param faceList [in]  List of numbers that define the faces in the shell.
      \param pEdgeData [in]  Pointer to additional edge data.
      \param pFaceData [in]  Pointer to additional face data.
  */
  virtual void filledShellProc(
                 const OdGePoint3d* vertexList,
                 OdInt32 faceListSize,
                 const OdInt32* faceList,
                 const OdGiEdgeData* pEdgeData = 0,
                 const OdGiFaceData* pFaceData = 0);


  /** \remarks
      Client applications have the option of overriding this function to process the shell
      data themselves.  If the function is not overridden, the default implementation
      of this function processes the shell into a set of facets.  More specifically,
      the default shellProc function calls setVertexData, and the calls either
      generateShellFaces for filled facets, or generateShellWires for a wireframe.
  */
  virtual void shellProc(OdInt32 numVertices,
                 const OdGePoint3d* vertexList,
                 OdInt32 faceListSize,
                 const OdInt32* faceList,
                 const OdGiEdgeData* pEdgeData = 0,
                 const OdGiFaceData* pFaceData = 0,
                 const OdGiVertexData* pVertexData = 0);

  /** \details
    Tessellates a wireframe shell
    \remarks
    This function is called from the default implementation of shellProc
    to tessellate a wireframe shell. Vertex data needed for this function
    can be obtained by calling vertexDataList and vertexData.

    Client applications have the option of overriding this function to process the shell
    data themselves.  If the function is not overridden, the default implementation
    of this function processes the shell into a set of facets.  More specifically,
    the default generateShellWires function processes shell attributes, and calls
    polylineOut with the tessellated shell data.

    \param faceListSize [in]  Number of entries in faceList.
    \param faceList [in]  List of numbers that define the faces in the shell.
    \param pEdgeData [in]  Pointer to additional edge data.
    \param pFaceData [in]  Pointer to additional face data.

    \sa
    OdGiEdgeData, OdGiFaceData, Vectorization Primitives
  */
   virtual void generateShellWires(OdInt32 faceListSize,
                                   const OdInt32* faceList,
                                   const OdGiEdgeData* pEdgeData = 0,
                                   const OdGiFaceData* pFaceData = 0);

  /** \details
    Tessellates a filled wireframe shell.
    \remarks
    This function is called from the default implementation of shellProc
    to tessellate a filled wireframe shell. Vertex data needed for this function
    can be obtained by calling vertexDataList and vertexData.

    Client applications have the option of overriding this function to process the shell
    data themselves.  If the function is not overridden, the default implementation
    of this function processes the shell into a set of facets.  More specifically,
    the default generateShellFaces function processes shell attributes, and calls
    shellFaceOut with the tessellated shell data.

    \param faceListSize [in]  Number of entries in faceList.
    \param faceList [in]  List of numbers that define the faces in the shell.
    \param pEdgeData [in]  Pointer to additional edge data.
    \param pFaceData [in]  Pointer to additional face data.

    \sa
    OdGiEdgeData, OdGiFaceData, Vectorization Primitives
  */
  virtual void generateShellFaces(OdInt32 faceListSize,
                                  const OdInt32* faceList,
                                  const OdGiEdgeData* pEdgeData = 0,
                                  const OdGiFaceData* pFaceData = 0);

  /** \details
    Outputs the face of a tessilated shell.

    \remarks
    This function is called from the default implementation of generateShellFaces,
    to process a single face in a shell along with the holes in that face.
    Vertex data needed for this function can be obtained by calling 
    vertexDataList and vertexData.

    Client applications have the option of overriding this function to process the face
    data themselves.  If the function is not overridden, the default implementation
    of this function processes the face information calls
    triangleOut with the resulting triangle data.

    \param faceListSize [in]  Number of entries in faceList.
    \param faceList [in]  List of numbers that define the face.
    \param pNormal [in]  Normal vector for this face.    
    \sa
    Vectorization Primitives
  */
    virtual void shellFaceOut(OdInt32 faceListSize,
                        const OdInt32* faceList,
                        const OdGeVector3d* pNormal);

  /** \details
    Output a triangle.
    \remarks
    This function is called from the default implementations of shellFaceOut
    and generateMeshFaces.  Vertex data needed for this function
    can be obtained by calling vertexDataList and vertexData.
    
    Client applications have the option of overriding this function to process 
    the triangle data themselves.  If the function is not overridden, 
    the default implementation of this function processes the color 
    attributes for the triangle, and calls polygonOut.

    \param vertices [in]  List of 3 numbers that define the vertices in the triangle.
    \param pNormal [in]  Normal vector for this triangle.

  */
  virtual void triangleOut(const OdInt32* vertices,
                           const OdGeVector3d* pNormal);

  /** \details
    Tessilates a shell.
    \remarks
    This auxiliary function that can be used to tessilate a shell up into a set of faces, 
    each with a maximum number of edges.
    
    Vertex data needed for this function is obtained by calling 
    vertexDataList and vertexData (so the caller of this function is responsible for 
    setting this data).  Face data is passed to the facetOut function, which 
    can be overridden by client applications to capture the tessellated data.

    \param faceListSize [in]  Number of entries in faceList.
    \param faceList [in]  List of numbers that define the faces in the shell.
    \param pFaceData [in]  Pointer to additional face data.
    \param maxFacetSize [in]  Maximum number of edges in the tessellated faces produced
      by this function.

    \sa
    OdGiFaceData, Vectorization Primitives
  */
  virtual void generateShellFacets(OdInt32 faceListSize,
    const OdInt32* faceList,
    const OdGiFaceData* pFaceData = 0,
    OdInt32 maxFacetSize = 3);

  /** \details
    Outputs a facet.
    \remarks
    This function is called from the default implementations of generateShellFacets.  
    Vertex data needed for this function is obtained by calling 
    vertexDataList and vertexData.

    Client applications have the option of overriding this function to process 
    the face data themselves.  If the function is not overridden, 
    the default implementation of this function calls polygonOut with the face data.

    \param faceList [in]  List of numbers that define the vertices in the facet.  
    \param edgeIndices [in]  Currently not used.
    \param pNormal [in]  Pointer to the normal vector for this face.
    \sa
    Vectorization Primitives
  */
  virtual void facetOut(const OdInt32* faceList,
        const OdInt32* edgeIndices,
        const OdGeVector3d* pNormal);

  /** \remarks
    Client applications have the option of overriding these function to process the circle
    data themselves.  If the function is not overridden, the default implementation
    tessellates the passed in circle using the current kOdGiMaxDevForCircle deviation,
    and calls polylineProc with the resulting data.
  */
  virtual void circleProc(const OdGePoint3d& center,
    double radius, 
    const OdGeVector3d& normal,
    const OdGeVector3d* pExtrusion = 0);

  virtual void circleProc(const OdGePoint3d& firstPoint,
    const OdGePoint3d& secondPoint, 
    const OdGePoint3d& thirdPoint,
    const OdGeVector3d* pExtrusion = 0);

  /** \remarks
    Client applications have the option of overriding this function to process the circular arc
    data themselves.  If the function is not overridden, the default implementation
    tessellates the passed in circular arc using the current kOdGiMaxDevForCircle deviation,
    and calls polylineProc with the resulting data (for kOdGiArcSimple type arcs).  For 
    kOdGiArcSector and kOdGiArcChord arc types, polygonOut is called.
  */
  virtual void circularArcProc(const OdGePoint3d& center,
    double radius,
    const OdGeVector3d& normal,
    const OdGeVector3d& startVector,
    double sweepAngle,
    OdGiArcType arcType = kOdGiArcSimple,
    const OdGeVector3d* pExtrusion = 0);

  virtual void circularArcProc(const OdGePoint3d& firstPoint,
    const OdGePoint3d& secondPoint,
    const OdGePoint3d& thirdPoint,
    OdGiArcType arcType = kOdGiArcSimple,
    const OdGeVector3d* pExtrusion = 0);

  /** \remarks
    Client applications have the option of overriding this function to process the text
    data themselves.  If the function is not overridden, the default implementation
    tessellates the passed in text string, by calling the textProc function
    on the associated OdGiContext object.  TrueType text will result in calls to 
    shellProc, and SHX text will get sent to polylineProc and polygonProc.
  */
  virtual void textProc(const OdGePoint3d& position,
    const OdGeVector3d& u, 
    const OdGeVector3d& v,
    const OdChar* msg, 
    OdInt32 length, 
    bool raw,
    const OdGiTextStyle* pTextStyle,
    const OdGeVector3d* pExtrusion = 0);
  virtual void textProc2(const OdGePoint3d& position,
    const OdGeVector3d& u, const OdGeVector3d& v,
    const OdChar* msg, OdInt32 length, bool raw, const OdGiTextStyle* pTextStyle,
    const OdGeVector3d* pExtrusion = 0, const OdGeExtents3d* extentsBox = 0);

  /** \remarks
    Client applications have the option of overriding this function to process the shape
    data themselves.  If the function is not overridden, the default implementation
    tessellates the passed in shape, by calling the shapeProc function
    on the associated OdGiContext object.  The resulting geometry 
    will get sent to polylineProc and polygonProc.
  */
  virtual void shapeProc(const OdGePoint3d& position,
    const OdGeVector3d& direction, 
    const OdGeVector3d& upVector,
    int shapeNumber, 
    const OdGiTextStyle* pTextStyle,
    const OdGeVector3d* pExtrusion = 0);

  /** \remarks
    Client applications have the option of overriding this function to process the NURBS
    data themselves.  If the function is not overridden, the default implementation
    tessellates the passed in NURBS curve using the current kOdGiMaxDevForCurve deviation,
    and calls polylineProc with the resulting data.
  */
  virtual void nurbsProc(const OdGeNurbCurve3d& nurbsCurve);

  /** \remarks
    Client applications have the option of overriding this function to process the elliptical arc
    data themselves. If the function is not overridden, the default implementation
    tessellates the passed in elliptical arc using the current kOdGiMaxDevForCurve deviation,
    and calls polylineProc with the resulting data.
  */
  virtual void ellipArcProc(
    const OdGeEllipArc3d& ellipArc,
    const OdGePoint3d* endPointOverrides = 0,
    OdGiArcType arcType = kOdGiArcSimple,
    const OdGeVector3d* pExtrusion = 0);

  /** \remarks
    Client applications have the option of overriding this function to process the elliptical arc
    data themselves. If the function is not overridden, the default implementation
    tessellates the passed in elliptical arc using the current kOdGiMaxDevForCurve deviation,
    and calls polylineProc with the resulting data.
  */
  virtual bool ellipArcProc(const OdGeEllipArc3d& ellipArc, double width);

  /** \remarks
      The default implementation of this function does nothing but return.
  */
  virtual void rasterImageProc(const OdGePoint3d& origin,
    const OdGeVector3d& u,
    const OdGeVector3d& v,
    const OdGiRasterImage* pImage, 
    const OdGePoint2d* uvBoundary, 
    OdUInt32 numBoundPts,
    bool transparency = false,
    double brightness = 50.0,
    double contrast = 50.0,
    double fade = 0.0);

  /** \details
    Initializes a texture map for this Vectorizer object.

    \param origin [in]  Lower-left corner. 
    \param u [in]  Image width vector.
    \param v [in]  Image height vector.
    \param pImage [in]  Pointer to the RasterImage object.
    \param transparency [in]  True if and only if image transparency is on.
    \param brightness [in]  Image brightness [0.0 .. 100.0].
    \param contrast [in]  Image contrast [0.0 .. 100.0].
    \param fade [in]  Image fade value [0.0 .. 100.0].
  */
  virtual void initTexture(const OdGePoint3d& origin,
    const OdGeVector3d& u,
    const OdGeVector3d& v,
    const OdGiRasterImage* pImage,
    bool transparency,
    double brightness,
    double contrast,
    double fade);

  /** \details
    Releases a texture map for this Vectorizer object.
  */
  virtual void uninitTexture();

  /** \remarks
      The default implementation of this function does nothing but return.
  */
  virtual void metafileProc(const OdGePoint3d& origin,
    const OdGeVector3d& u,
    const OdGeVector3d& v,
    const OdGiMetafile* pMetafile,
    bool dcAligned = true,        
    bool allowClipping = false); 

  /** \remarks
      Client applications have the option of overriding this function to process the polypoint
      data themselves. If the function is not overridden, the default implementation
      of this function processes the polypoint, calling polylineProc for each point.
  */
  virtual void polypointProc(OdInt32 numPoints,
    const OdGePoint3d* vertexList,
    const OdCmEntityColor* pColors,
    const OdCmTransparency* pTransparency = 0,
    const OdGeVector3d* pNormals = 0,
    const OdGeVector3d* pExtrusions = 0,
    const OdGsMarker* pSubEntMarkers = 0,
    OdInt32 nPointSize = 0);

  /** \remarks
      Client applications have the option of overriding this function to process the rowOfDots
      data themselves. If the function is not overridden, the default implementation
      of this function processes the rowOfDots, calling polylineProc for each point.
  */
  virtual void rowOfDotsProc(OdInt32 numPoints, const OdGePoint3d& startPoint, const OdGeVector3d& dirToNextPoint);

  /** \remarks
      Client applications have the option of overriding this function to process the edge
      data themselves. If the function is not overridden, the default implementation
      of this function processes the edge, calling polylineProc for each hatch line or
      shellProc for solid and gradient filling.
  */
  virtual void edgeProc(const OdGiEdge2dArray& edges, const OdGeMatrix3d* pXform = 0);

  /** \details
    Passes polyline data to this Vectorizer object.

    \remarks
    This function is called by the OdGiGeometrySimplifier class to 
    pass polyline data generated during tessellation, to a client application.

    Client applications have the option of overriding this function to process 
    the data themselves.  If the function is not overridden, 
    the default implementation of this function calls polylineOut with the vertex list.

    \param vertexIndexList [in]  Array of vertex indices in vertexDataList().
  */
  virtual void polylineOut(OdInt32 numPoints, const OdInt32* vertexIndexList);

  /** \remarks
      Utility function to be called from client implementation of meshProc()
      to unify mesh primitives processing. 
  */
  void convertMeshToShell(
    OdInt32 rows, OdInt32 columns,
    const OdGePoint3d* pVertexList,
    const OdGiEdgeData* pEdgeData,
    const OdGiFaceData* pFaceData,
    const OdGiVertexData* pVertexData);

  /** \remarks
    Client applications have the option of overriding this function to process the PolyDraw
    data themselves. If the function is not overridden, the default implementation
    tesselates the passed in PolyDraw contours using the current kOdGiMaxDevForCurve deviation,
    and calls shellProc with the resulting data.
  */
  virtual void ttfPolyDrawProc(
    OdInt32 numVertices, const OdGePoint3d* vertexList,
    OdInt32 faceListSize, const OdInt32* faceList,
    const OdUInt8* pBezierTypes, const OdGiFaceData* pFaceData = 0);

  /** \remarks
      Utility function to be called from client implementation of shellProc()
      to subdivide shell onto set of smaller shell primitives by vertexes number limit.
  */
  void subdivideShellByVertexLimit(OdInt32 numVertices, const OdGePoint3d* vertexList,
    OdInt32 faceListSize, const OdInt32* faceList,
    const OdGiEdgeData* pEdgeData, const OdGiFaceData* pFaceData,
    const OdGiVertexData* pVertexData, OdUInt32 nLimit);


  /** \details
    Applies given fill pattern to the shell.
    Returns true if pattern was applied succesfully and false 
    if pattern is too dense and solid fill is required. 
  */
  virtual bool generateShellFill( OdGiHatchPatternPtr pHatch,
                                  OdDouble& fillDensity,
                                  const OdGePoint3d* pVertexList,
                                  OdInt32 faceListSize,
                                  const OdInt32* pFaceList,
                                  const OdGiFaceData* pFaceData = 0,
                                  OdGiMapperItemEntry* pMapper = 0
                                  );

protected:
  // Pline primitive generation helpers
  bool jointLineWithArc(const OdGiPolyline& lwBuf, OdInt32 LineSegNo, OdInt32  ArcSegNo, OdGePoint3dArray& points);
  bool jointLineWithLine(const OdGiPolyline& lwBuf, OdInt32 LineSegNo1, OdInt32 LineSegNo2, OdGePoint3dArray& points);
  bool jointArcWithLine(const OdGiPolyline& lwBuf, OdGeCircArc2d& arc, OdInt32 arcSegNo, OdInt32 lineSegNo,
                        OdGePoint3d& point1, OdGePoint3d& point2);
  // Mesh primitive generation helpers
  bool isMeshRequireVertexNormals(const OdGiVertexData *pVertexData);
  void generateMeshVertexNormals(OdInt32 numRows, OdInt32 numCols, const OdGePoint3d *pVertexList, const OdGiVertexData *pVertexData,
                                 OdGiVertexData &vertexData, OdGeVector3dArray &normals);
  // Block of internal optimizations
  OdGeLineSeg2d *m_pLineSeg2d; OdGeLineSeg2d &tmpLineSeg2d();
  OdGeLineSeg2d *m_pLineSeg2dEx[2]; OdGeLineSeg2d &tmpLineSeg2dEx(int n);
  OdGeLine2d *m_pLine2dEx[4]; OdGeLine2d &tmpLine2dEx(int n);
  OdGeCircArc2d *m_pCircArc2d; OdGeCircArc2d &tmpCircArc2d();
  OdGeCircArc3d *m_pCircArc3d; OdGeCircArc3d &tmpCircArc3d();
  OdGeEllipArc3d *m_pEllipArc3d; OdGeEllipArc3d &tmpEllipArc3d();
  OdGePoint3dArray &tmpPoints3d(int nLevel) { return (!nLevel) ? m_points3d1 : m_points3d2; }
};

/** \details

    \sa
    TD_Gi 
    <group OdGi_Classes> 
*/
struct ODGI_EXPORT OdGiFillData
{
  OdGiFillData(OdGiConveyorContext* pDrawCtx, OdGiSubEntityTraits* pTraits,
    OdDb::LineWeight lweight = OdDb::kLnWt000,
    OdGiFillType fillType = kOdGiFillNever);
  void set(OdDb::LineWeight lweight, OdGiFillType fillType, const OdGeVector3d* fillNormal);
  ~OdGiFillData();

  OdDb::LineWeight m_lweight;
  OdGiFillType m_fillType;
  OdGeVector3d m_fillNormal;
  OdGeVector3d* m_pFillNormal;
  OdGiSubEntityTraits* m_pTraits;
  OdGiConveyorContext* m_pDrawCtx;
};

#include "TD_PackPop.h"

#endif //#ifndef __ODGIGEOMETRYSIMPLIFIER_H__
