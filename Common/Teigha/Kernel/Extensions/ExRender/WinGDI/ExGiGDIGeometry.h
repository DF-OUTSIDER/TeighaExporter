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

#ifndef _EXGSGDIGEOMETRY_H_INCLUDED_
#define _EXGSGDIGEOMETRY_H_INCLUDED_

#include "TD_PackPush.h"

#include "IntArray.h"
#include "Gi/GiGeometrySimplifier.h"
#include "Gi/GiPlotGenerator.h"
#include "Ps/PlotStyles.h"
#include "Gi/GiPlotStyleDefs.h"
#include "../ExColorConverterCallback.h"
#include "UInt8Array.h"
#include "CmColorBase.h"

class ExGDIVectorizeView;
class OdGiSelfGdiDrawable;

typedef OdArray<POINT, OdMemoryAllocator<POINT> >                         POINTArray;
#ifndef _WIN32_WCE
typedef OdArray<GRADIENT_TRIANGLE, OdMemoryAllocator<GRADIENT_TRIANGLE> > GRADIENT_TRIANGLEArray;
typedef OdArray<TRIVERTEX, OdMemoryAllocator<TRIVERTEX> >                 TRIVERTEXArray;
#endif //#ifndef _WIN32_WCE

/** \details
  This class implements Geometry Simplifier objects for GDI Vectorizer Device objects.
  <group ExRender_Windows_Classes> 
*/
class ExGiGDIGeometry : public OdGiGeometrySimplifier, public OdGiPlotGenerator::PolylineOut
{
public:
  enum LineType
  {
    kLtpSolid                         = OdPs::kLtpSolid,
    kLtpNull                          = OdPs::kLtpUseObject,
    kLtpForSelection                  = OdPs::kLtpLast + 1
  };
  /** \details
    Sets the draw linetype for this Vectorizer object.
    \param linetype [in]  Linetype.
  */
  void draw_pslinetype(LineType linetype);

  enum FillStyle
  {
    kFsSolid          = OdPs::kFsSolid,
    kFsCheckerboard   = OdPs::kFsCheckerboard,
    kFsCrosshatch     = OdPs::kFsCrosshatch,
    kFsDiamonds       = OdPs::kFsDiamonds,
    kFsHorizontalBars = OdPs::kFsHorizontalBars,
    kFsSlantLeft      = OdPs::kFsSlantLeft,
    kFsSlantRight     = OdPs::kFsSlantRight,
    kFsSquareDots     = OdPs::kFsSquareDots,
    kFsVerticalBars   = OdPs::kFsVerticalBars,
    kFsHollow         = OdPs::kFsUseObject,
    kFsForSelection   = OdPs::kFsLast + 1
  };
  /** \details
    Sets the draw fillstyle for this Vectorizer Device object.
    \param fillStyle [in]  Fill style.
  */
  void draw_fillStyle(FillStyle fillStyle);

private:
  bool isExtPen() const;
  HPEN createCurrentPen();
#ifndef _WIN32_WCE
  typedef void (ExGiGDIGeometry::*TriangleFn) (const OdInt32* pTriangleVertices, const OdGeVector3d* pNormal);

  HINSTANCE               m_hinstMsImgDll;
  FARPROC                 m_grFillPrAddr, m_alphaBlendPrAddr;
  FARPROC                 m_dcBrushClrPrAddr;
  GRADIENT_TRIANGLEArray  m_triangles;
  TRIVERTEXArray          m_triVertexes;
  TriangleFn              m_triangleFn;
#endif //#ifndef _WIN32_WCE

  const OdGiPsLinetypes::PsLinetypeGDI   *m_psLinetypeDefs;
  const OdGiPsFillstyles::PsFillstyleGDI *m_psFillstyleDefs;

  OdGePoint3d             m_trianglePts[3];
  OdIntArray              m_clipStack;
  HGDIOBJ                 m_hOldPen;
  HGDIOBJ                 m_hOldBrush;
  HPEN                    m_hNullPen;
  HPEN                    m_hThinPen;
  HPEN                    m_hExPen;
  HBRUSH                  m_hNullBrush;
  POINTArray              m_dcPoints;

  // LOGPEN                  m_logPen;

  LOGBRUSH                m_logBrush;

  DWORD                   m_dwPenStyle;
  DWORD                   m_dwWidth;
  DWORD                   *m_lpStyle;
  DWORD                   m_dwStyleCount;
  LOGBRUSH                m_penBrush;
  OdRxObjectPtr           m_pPVC;

  const ODCOLORREF*       m_paletteColors;
  OdUInt16                m_numPaletteColors;
  OdGsDCRect              m_rcTarget;
  HDC                     m_hTargetDC;

  // old device traits
  OdPs::LineEndStyle      m_oldEndStyle;
  OdPs::LineJoinStyle     m_oldJoinStyle;
  LineType                m_oldLinetype;
  FillStyle               m_oldFillStyle;
  int                     m_oldBkMode;

  // Global transparency
  bool                    m_bGlobalAlpha;
  int                     m_nGlobalAlpha;
  ODCOLORREF              m_clrFadeColor;

  // LineWeight settings
  bool                    m_bPointsLwd;
  OdPs::LineEndStyle      m_lwdCapStyle;
  OdPs::LineJoinStyle     m_lwdJoinStyle;
  bool                    m_bExtLwdStyle;

  bool                    m_bLwdOverride;
protected:
  // Gradients printing
  bool m_bGradientAsBitmap;
  bool m_bGradientAsPolys;
  OdUInt8 m_nGradientAsPolysSubdivideTreshold;
  // Color conversion callback
  OdColorConverterCallback *m_pColorConverter;
  // GradientFill replacement
  void renderGradientTriangles( HDC hdc );
public:
  /** \details
    Sets the draw color for this Vectorizer Device object.
    \param color [in]  Color.
  */
  void draw_color(const OdCmEntityColor& color);
  
  /** \details
    Sets the draw color for this Vectorizer Device object.
    \param color [in]  Color.
  */
  void draw_color(COLORREF color);
  /** \details
    Sets the draw color index for this Vectorizer Device object.
    \param colorIndex [in]  Color index.
  */
  void draw_color_index(int colorIndex);
  
  /** \details
    Sets the draw lineweight for this Vectorizer Device object.
    \param lineweight [in]  Lineweight in pixels.
  */
  void draw_lineweight(int lineweight);
  
  /** \details
    Sets the draw endstyle for this Vectorizer Device object.
    \param endStyle [in]  Endstyle.
  */
  void draw_endStyle(OdPs::LineEndStyle endStyle);
  /** \details
    Sets the draw joinstyle for this Vectorizer Device object.
    \param joinStyle [in]  Joinstyle.
  */
  void draw_joinStyle(OdPs::LineJoinStyle joinStyle);
  
  /** \details
    Sets the draw background mode for this Vectorizer Device object.
    \param bkMode [in]  Background mode.
    \remarks
    bkMode will be one of the following:
    
    <table>
    Name          Description
    OPAQUE        Entities are drawn opaque.
    TRANSPARENT   Entities are drawn transparent.
    </table>
  */
  void draw_bkMode(int bkMode);

  void draw_globalAlpha(bool bFlag) { m_bGlobalAlpha = bFlag; }
  void draw_globalAlpha(bool bFlag, int nAlpha, ODCOLORREF fadeColor)
    { m_bGlobalAlpha = bFlag; m_nGlobalAlpha = nAlpha; m_clrFadeColor = fadeColor; }

  void setLineWeightSettings(bool bPointsLwd, OdPs::LineEndStyle lwdCapStyle, OdPs::LineJoinStyle lwdJoinStyle)
    { m_bPointsLwd = bPointsLwd; m_lwdCapStyle = lwdCapStyle; m_lwdJoinStyle = lwdJoinStyle;
      m_bExtLwdStyle = (m_lwdCapStyle != OdPs::kLesRound) || (m_lwdJoinStyle != OdPs::kLjsRound); }
  bool isPointsLineWeightEnabled() const { return m_bPointsLwd; }
  OdPs::LineEndStyle getLineWeightCapStyle() const { return m_lwdCapStyle; }
  OdPs::LineJoinStyle getLineWeightJoinStyle() const { return m_lwdJoinStyle; }
  bool isLineWeightStyleExtended() const { return m_bExtLwdStyle; }

  struct LwdOverrideSettings { bool m_bOverride, m_bPointsLwd; int m_nLwd;
  LwdOverrideSettings(bool bOverride, bool bPointsLwd, int nLwd) : m_bOverride(bOverride), m_bPointsLwd(bPointsLwd), m_nLwd(nLwd) {} };
  void setLineweightOverride(const LwdOverrideSettings &pIn, LwdOverrideSettings *pOut = NULL)
  {
    if (pOut) { *pOut = LwdOverrideSettings(m_bLwdOverride, m_bPointsLwd, (int)m_dwWidth); }
    m_bLwdOverride = false; m_bPointsLwd = pIn.m_bPointsLwd;
    draw_lineweight(pIn.m_nLwd); m_bLwdOverride = pIn.m_bOverride;
  }

  void setGradientPrintProps(bool bPrintAsBitmap, bool bPrintAsPolys, OdUInt8 nThreshold)
    { m_bGradientAsBitmap = bPrintAsBitmap; m_bGradientAsPolys = bPrintAsPolys;
      m_nGradientAsPolysSubdivideTreshold = nThreshold; }

  void enableTTFProcessing(bool bSet) { m_bProcessingTTF = bSet; }
  void startThinPenOverride();
  void endThinPenOverride();
protected:
  bool plotGeneratorPolylineOut(const OdGePoint3d *pPoints, OdUInt32 nPoints,
    OdPs::LineEndStyle capStyle, OdPs::LineJoinStyle joinStyle, double fLwd);

  HGDIOBJ thinPen();
  HGDIOBJ nullPen() { return (HGDIOBJ)m_hNullPen; }
  bool ttfProcessingPenOverride() const { return m_bProcessingTTF && (m_dwWidth > 1) && !m_thinPenOverride; }

  DWORD             m_thinPenOverride;
  bool              m_bThinPenValid;

  bool              m_bPenValid;
  bool              m_bBrushValid;

  bool              m_bDrawSelection;
  bool              m_bDrawDragging;
  bool              m_bProcessingTTF;
public:
  ExGiGDIGeometry();
  ~ExGiGDIGeometry();

  /** \details
    Returns the pen style for this Vectorizer Device object.
  */
  OdInt32 penStyle() const
  {
    return m_dwPenStyle;
  }
  /** \details
    Returns the brush style for this Vectorizer Device object.
  */
  OdInt32 brushStyle() const
  {
    return m_logBrush.lbStyle;
  }

  /** \details
    Sets the pen style for this Vectorizer Device object.
    \param penStyle [in]  Pen style.
  */
  void setPenStyle(OdInt32 penStyle)
  {
    m_dwPenStyle = penStyle;
  }
  /** \details
    Sets the brush style for this Vectorizer Device object.
    \param brushStyle [in]  Brush style.
  */
  void setBrushStyle(OdInt32 brushStyle)
  {
    m_logBrush.lbStyle = brushStyle;
  }

  /** \details
    Returns true if and only if the DrawSelection flag is set for this Vectorizer Device object.
  */
  bool isSelection() { return m_bDrawSelection; }
  /** \details
    Returns true if and only if the DrawDragging flag is set for this Vectorizer Device object.
  */
  bool isDragging() { return m_bDrawDragging; }

  /** \details
    Controls the DrawSelection flag for this Vectorizer Device object.
    \param drawSelection [in]  DrawSelection flag.
  */
  void setSelection(bool drawSelection) { m_bDrawSelection = drawSelection; }
  /** \details
    Controls the DrawDragging flag for this Vectorizer Device object.
    \param drawDragging [in]  DrawDragging flag.
  */
  void setDragging(bool drawDragging) { m_bDrawDragging = drawDragging; }

  /** \details
    Initializes the paint process for this Vectorizer Device object.
    \param targetDC [in]  Handle to the target device context.
  */
  void beginPaint(HDC targetDC);
  /** \details
    Sets the logical palette to be used by this Vectorizer Device object.
    
    \param logicalPalette [in]  Logical palette.
    \param numColors [in]  Number of colors in palette.
  */
  void setLogicalPalette(const ODCOLORREF* logicalPalette, OdUInt16 numColors);
  /** \details
    Sets the output rectangle for this Vectorizer Device object.
    \param outputRect [in]  Output rectangle.
  */
  void setOutputRect(const OdGsDCRect& outputRect) { m_rcTarget = outputRect; }
  /** \details
    Returns a reference to the output rectangle for this Vectorizer Device object.
  */
  const OdGsDCRect& outputRect() const { return m_rcTarget; }

  /** \details
    Terminates the paint process for this Vectorize Device object.
  */
  void endPaint();

  /** \details
    Controls the pen and brush for this Vectorizer Device object.
    \param forSelection [in]  True if and only if the selection pen and brush are to be used.
  */
  void updatePenNBrush(bool bForSelection = false);

  /** \details
    Pushes the current device context onto the stack, and creates a new clip boundary.

    \param points [in]  Array of points defining the polygonal clip boundary.
    \param pointCounts [in]  Array of integers specifying the number of points in each polygon.
    \param numPolygons [in]   Number of polygons.
    \remarks
    If a two-vertex polygon is specified, the vertices specify the lower-left and upper-right
    corners of the clip boundary.
  */
  void pushViewportClip(const OdGsDCPoint* points, const int *pointCounts, int numPolygons);
  
  /** \details
    Pushes the current device context onto the stack, and creates a new clip boundary.
    \param min [in]  Lower-left corner of clip boundary.
    \param max [in]  Upper-right corner of clip boundary.
    \remarks
    If a two-vertex polygon is specified, the vertices specify the lower-left and upper-right
    corners of the clip boundary.
  */
  void pushViewportClip(const OdGsDCPoint& min, const OdGsDCPoint& max);
  
  /** \details
    Pushes the current device context onto the stack, and creates a new clip boundary.
    \param min [in]  Lower-left corner of clip boundary.
    \param max [in]  Upper-right corner of clip boundary.
    \param points [in]  Array of points defining the polygonal clip boundary.
    \param pointCounts [in]  Array of integers specifying the number of points in each polygon.
    \param numPolygons [in]   Number of polygons.
    \remarks
    If a two-vertex polygon is specified, the vertices specify the lower-left and upper-right
    corners of the clip boundary.
  */
  void pushViewportClip(OdUInt32 nNumRects, const OdGsDCRect* rects);

  /** \details
    Pops the clip boundary from the top of the stack.
  */
  void popViewportClip();

#ifndef _WIN32_WCE
  /** \details
    Pre-processes gradient-filled triangles with this Vectorizer Device object.
    
    \param numVertices [in]  Number of vertices.
    \param vertexList [in]  Array of vertices defining gradient-filled triangles.
    \param pVertexData [in]  Pointer to additional vertex data.
    \sa
    endGradientFill

    \note
    Configures triangleOut() to output gradient-filled triangles.
  */
  bool beginGradientFill(OdInt32 numVertices,
                         const OdGePoint3d* vertexList,
                         const OdGiVertexData* pVertexData);
  /** \details
    Output a gradient-filled triangle.

    \param vertices [in]  List of 3 numbers that define the vertices in the triangle.
    \param pNormal [in]  Normal vector for this triangle (ignored).
  */
  void gradientTriangleOut(const OdInt32* vertices, const OdGeVector3d* pNormal);

  /** \details
    Outputs filled triangles to the specified device context.
    \param hdc [in]  Handle of device context.
    \sa
    beginGradientFill
    \note
    Configures triangleOut() to output regular (non-filled) triangles.
  */
  void endGradientFill(HDC hdc);

  /** \details
    Output a non-filled triangle.
    \param vertices [in]  List of 3 numbers that define the vertices in the triangle.
    \param pNormal [in]  Normal vector for this triangle.
  */
  void regularTriangleOut(const OdInt32* vertices, const OdGeVector3d* pNormal);

#endif //#ifndef _WIN32_WCE
  /** \details
    Outputs both gradient and regular triangles.
    \note
    *  This function is initialized to output regular (non-filled) triangles.
    *  beginGradientFill configures this function to output gradient-filled triangles.
    *  endGradientFill configures this function to output regular (non-filled) triangles.
  */
  void triangleOut(const OdInt32* vertices, const OdGeVector3d* pNormal);

  void polylineOut(OdInt32 numPoints, const OdGePoint3d* vertexList);

  void polygonOut(OdInt32 numPoints, const OdGePoint3d* vertexList, const OdGeVector3d* pNormal = 0);

  void generateShellFaces(OdInt32 faceListSize, const OdInt32* pFaceList,
                          const OdGiEdgeData* pEdgeData, const OdGiFaceData* pFaceData);
  void polygonProc(OdInt32 nbPoints, const OdGePoint3d* pVertexList,
                   const OdGeVector3d* pNormal, const OdGeVector3d* pExtrusion);

  void shellFaceOut(OdInt32 faceListSize, const OdInt32* faceList, const OdGeVector3d* pNormal = 0);

  void ttfPolyDrawProc(OdInt32 nbVertex, const OdGePoint3d* pVertexList,
                       OdInt32 faceListSize, const OdInt32* pFaceList,
                       const OdUInt8* pBezierTypes, const OdGiFaceData* pFaceData = 0);

  void polylineProc(OdInt32 numPoints, const OdGePoint3d* vertexList, const OdGeVector3d* pNormal = 0,
    const OdGeVector3d* pExtrusion = 0, OdGsMarker baseSubEntMarker = -1);

  /*
  void polygonProc(OdInt32 nbPoints, const OdGePoint3d* pVertexList,
    const OdGeVector3d* pNormal = 0, const OdGeVector3d* pExtrusion = 0);

  void circleProc(const OdGePoint3d& center, double radius,
    const OdGeVector3d& normal,
    const OdGeVector3d* pExtrusion = 0);
  
  void circleProc(const OdGePoint3d&, const OdGePoint3d&, const OdGePoint3d&,
    const OdGeVector3d* pExtrusion = 0);
  
  void circularArcProc(const OdGePoint3d& center,
    double radius,
    const OdGeVector3d& normal,
    const OdGeVector3d& startVector,
    double sweepAngle,
    OdGiArcType arcType = kOdGiArcSimple,
    const OdGeVector3d* pExtrusion = 0);
  
  void circularArcProc(const OdGePoint3d& start,
    const OdGePoint3d& point,
    const OdGePoint3d& end,
    OdGiArcType arcType = kOdGiArcSimple,
    const OdGeVector3d* pExtrusion = 0);

  void ellipArcProc(OdGiConveyorContext* pDrawCtx,
    const OdGeEllipArc3d& arc,
    const OdGePoint3d* pEndPointsOverrides = 0,
    OdGiArcType arcType = kOdGiArcSimple, const OdGeVector3d* pExtrusion = 0);

  void nurbsProc(const OdGeNurbCurve3d& nurbs);

  void shapeProc(const OdGePoint3d& position,
    const OdGeVector3d& u, const OdGeVector3d& v,
    int shapeNo, const OdGiTextStyle* pStyle,
    const OdGeVector3d* pExtrusion = 0);
    */
  
  void meshProc(OdInt32 numRows,
    OdInt32 numColumns,
    const OdGePoint3d* vertexList,
    const OdGiEdgeData* pEdgeData = 0,
    const OdGiFaceData* pFaceData = 0,
    const OdGiVertexData* pVertexData = 0);

  void shellProc(OdInt32 numVertices,
    const OdGePoint3d* vertexList,
    OdInt32 faceListSize,
    const OdInt32* faceList,
    const OdGiEdgeData* pEdgeData = 0,
    const OdGiFaceData* pFaceData = 0,
    const OdGiVertexData* pVertexData = 0);

#if !defined(_WIN32_WCE)
  void textProcEx(const OdGePoint3d& position, const OdGeVector3d& u, const OdGeVector3d& v,
    const OdString &msg, const OdIntArray &ulFlags, const OdGiTextStyle* pTextStyle,
    OdGeVector3d originalSize);
  static OdString textProcExEmbedRawData(const OdString &msg, const OdIntArray &ulFlags, int *length = NULL, bool *raw = NULL);
#endif

  void rasterImageProc(
    const OdGePoint3d& origin,
    const OdGeVector3d& u,
    const OdGeVector3d& v,
    const OdGiRasterImage* pImage, 
    const OdGePoint2d* uvBoundary,
    OdUInt32 numBoundPts,
    bool transparency = false,
    double brightness = 50.0,
    double contrast = 50.0,
    double fade = 0.0);

  void metafileProc(
    const OdGePoint3d& origin,
    const OdGeVector3d& u,
    const OdGeVector3d& v,
    const OdGiMetafile* pMetafile,
    bool dcAligned = true,       
    bool allowClipping = false); 


  /** \details
    Passes to the rendering framework an object that can render itself.

    \param origin [in]  Object origin.
    \param u [in]  Object width vector.
    \param v [in]  Object height vector.
    \param dcAligned [in]  reserved.
    \param allowClipping [in]  reserved.
    \param pDrawable [in]  Pointer to the drawable.
    \remarks
    Teigha uses this function to render Ole2Frame entities. 
    
    \note
    ownerDrawDc is not conveyor primitive, so Teigha's rendering framework performs no processing 
    (transformation, linetyping, or clipping) for this object. 

    All coordinates are in Model coordinates.
    
    \sa
    OdGiSelfGdiDrawable.
  */
  void ownerDrawDc(
    const OdGiCommonDraw& context,
    const OdGePoint3d& origin,
    const OdGeVector3d& u,
    const OdGeVector3d& v,
    const OdGiSelfGdiDrawable* pDrawable,
    bool dcAligned = true,       
    bool allowClipping = false);

  void polypointProc(OdInt32 numPoints, const OdGePoint3d* vertexList,
                     const OdCmEntityColor* pColors, const OdCmTransparency* pTransparency = 0,
                     const OdGeVector3d* pNormals = 0, const OdGeVector3d* pExtrusions = 0,
                     const OdGsMarker* pSubEntMarkers = 0, OdInt32 nPointSize = 0);
};

#include "TD_PackPop.h"

#endif //#ifndef _EXGSGDIGEOMETRY_H_INCLUDED_
