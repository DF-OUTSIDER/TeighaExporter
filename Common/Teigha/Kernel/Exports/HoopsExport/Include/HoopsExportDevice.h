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



//////////////////////////////////////////////////////////////////////
//
// PdfDrawObject.h: definition of the GsPdfFactory, PDF2dExportDevice, OdPdfView classes.
//
//////////////////////////////////////////////////////////////////////

#ifndef _HOOPS_EXPORT_DEVICE_INCLUDED_
#define _HOOPS_EXPORT_DEVICE_INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Gs/GsBaseInclude.h"
#include "Gi/GiGeometrySimplifier.h"
#include "Gi/GiRasterImage.h"
#include "Gi/GiSpatialFilter.h"
#include "Gi/GiPerspectivePreprocessor.h"
#include "Gi/GiHLRemover.h"
#include "BoolArray.h"
#include "HoopsExportImpl.h"
#include "Gi/GiLinetype.h"
#include "HoopsCommon.h"
#include "Ge/GeCircArc3d.h"
#include "Ge/GeExtents3d.h"
#include "hc.h"

//////////////////////////////////////////////////////////////////////

/** \details
  This namespace contains elements related to functionality for exporting to a hoops file.
  
  <group OdExport_Classes> 
*/
namespace TD_HOOPS_COMMON_EXPORT {

/** \details
  This structure implements the hoops extents keeper.
*/
struct CHoopsExtentsKeeper
{
  const OdGiDrawable* pObject;  //!< A pointer to a drawable entity.
  OdGsView*     pView;          //!< A pointer to a view entity.
  OdGeExtents3d extents;        //!< An extents object instance.
};

/** \details
  This class implements the hoops export device.
  A class instance gets 3D data from Teigha Rendering Framework.
*/
class OdHoopsExportDevice : public OdGsBaseVectorizeDevice, 
                            public OdGiGeometrySimplifier  
{
protected:
  //DOM-IGNORE-BEGIN
  ODRX_USING_HEAP_OPERATORS(OdGsBaseVectorizeDevice);
  //DOM-IGNORE-END

public:

  bool                        m_bSetCamera;   //!<< A flag determining whether the camera is set.
  bool                        m_bInitCamera;  //!<< A flag determining whether the camera should be initialized.

  /** \details 
    Creates a new instance of a hoops export device.
  */
  OdHoopsExportDevice();
  
  /** \details 
    Destroys an instance of the hoops export device.
  */
  virtual ~OdHoopsExportDevice();

  /** \details 
    Creates an instance of a hoops export device and returns a typified smart pointer to it.
    
    \remarks 
    This pseudo constructor sets the target data stream and the type.
  */
  static OdGsDevicePtr createObject();

  /** \details 
    Creates an instance of the desired OdGsView subclass. 
    
    \param pInfo                          [in] An object containing client view information to be used for creation.
    \param bEnableLayerVisibilityPerView  [in] A flag that determines whether the layer is visible for the created view.
    
    \remarks 
    This method usually is called by a rendering framework. 
  */
  OdGsViewPtr createView(const OdGsClientViewInfo* pInfo = 0, bool bEnableLayerVisibilityPerView = false);

  /** \details 
    Sets a new rendering device for the export device.
    
    \param pRenderDev [in] A pointer to the new rendering device to be used. 
  */
  void setRenderDevice(OdGsDevice* pRenderDev);
  
  /** \details 
    Enables or disables recording mode for the device.
    
    \param bEnable [in] A flag that determines whether the rendering mode is available (if true) or not (if false). 
  */
  void enableRecording( bool bEnable = true ) { m_bRecording = bEnable; }
  
  
  bool getSavedExtents( const OdGiDrawable* pObject, OdGsView* pView, OdGeExtents3d& extents );
  
  /** \details 
    Sets a new rendering device for the export device.
    
    \param pRenderDev [in] A pointer to a new render device to be used. 
  */
  void saveExtents( const OdGiDrawable* pObject, OdGsView* pView, OdGeExtents3d& extents );

  /** \details 
    Sets a new special hoops export device context.
    
    \param pHoops [in] A pointer to a new device content to be set.
  */
  void setExportImpl(CHoopsExportImpl* pHoops);
  
  /** \details 
    Returns the current special device context.
  */
  CHoopsExportImpl* getExportImpl();

  /** \details 
    Sets a flag value that determines whether rays are exported to hoops.
    
    \param bSet [in] A flag value to be set.
  */    
  void setRayExport( bool bSet ) { m_bRayExport = bSet; };
  
  /** \details 
    Sets a flag value that determines whether infinite lines are exported to hoops.
    
    \param bSet [in] A flag value to be set.
  */    
  void setXLineExport( bool bSet ) { m_bXLineExport = bSet; };
  
  /** \details 
    Returns the current flag value that determines whether rays are exported to hoops.
    
    \returns Returns true if rays are exported to hoops, otherwise returns false.
  */    
  bool getRayExport() { return m_bRayExport; };
  
  /** \details 
    Returns the current flag value that determines whether infinite lines are exported to hoops.
    
    \returns Returns true if infinite lines are exported to hoops, otherwise returns false.
  */    
  bool getXLineExport() { return m_bXLineExport; };

  /** \details 
    Updates the specified rectangle.
    
    \param pUpdatedRect [in] A pointer to the rectangle to be updated. 
    
    \remarks 
    This method is called by Teigha Rendering Framework before rendering the view associated with the device.
    The override must call the parent version of this method, OdGsBaseVectorizeDevice::update().     
  */
  void update(OdGsDCRect* pUpdatedRect);

  /** \details 
    Initializes the hoops export device.
    
    \remarks
    This method resets the hoops engine, gets the hoops segment key, makes the hoops geometry visible and sets the background color. 
  */
  void dc_init();
  
  /** \details 
    Adds a polyline to hoops geometry.
    \param nPts [in] A quantity of polyline vertices.
    \param nPts [in] A pointer to a three-dimensional points array, which represents the vertices of the polyline.
  */  
  void dc_polyline(OdUInt32 nPts, const OdGePoint3d* pPoints);
  
  /** \details 
    Adds an infinite line to hoops geometry.
    \param p1 [in] The first three-dimensional point that determines the line.
    \param p2 [in] The second three-dimensional point that determines the line.
  */  
  void dc_xline( const OdGePoint3d& p1, const OdGePoint3d& p2 );
  
  /** \details 
    Adds a ray to hoops geometry.
    \param p1 [in] The three-dimensional point that represents the starting point of the ray.
    \param p2 [in] The second three-dimensional point that determines the ray.
  */    
  void dc_ray( const OdGePoint3d& p1, const OdGePoint3d& p2 );
  
  /** \details 
    Adds a circle arc to hoops geometry.
    \param circArc [in] A circle arc object to be added.
  */
  void dc_circleArc( const OdGeCircArc3d& circArc );
  
  /** \details 
    Adds an elliptical arc to hoops geometry.
    \param ellipArc [in] An elliptical arc object to be added.
  */
  void dc_ellipArc( const OdGeEllipArc3d& ellipArc );
  
  /** \details 
    Adds a NURB curve to hoops geometry.
    \param nurbCurve [in] A NURB curve object to be added.
  */  
  void dc_nurbs( const OdGeNurbCurve3d& nurbCurve );

  /** \details 
    Adds text to hoops geometry.
    
    \param position   [in] A point to define the text start position.
    \param normal     [in] A U vector of the text plane.
    \param normal     [in] A V vector of the text plane.
    \param msg        [in] Text to be vectorized represented by a pointer to an OdChar object.
    \param length     [in] Text length.
    \param raw        [in] A flag value that determines whether escape symbols should be interpreted.
    \param pTextStyle [in] A style to be used for text vectorizing.  
    \param pExtrusion [in] A vector that determines the text extrusion direction.
  */
  void dc_text( const OdGePoint3d& position, const OdGeVector3d& u, const OdGeVector3d& v,
                const OdChar* msg, OdInt32 length, bool raw, const OdGiTextStyle* pTextStyle,
                const OdGeVector3d* pExtrusion = 0);
                
  /** \details 
    Adds a polygon to hoops geometry.
    
    \param numPoints  [in] A quantity of polygon vertices.
    \param vertexList [in] A pointer to a polygon vertices array.
    \param pNormal    [in] A vector that determines the normal to the polygon plane.
  */
  void dc_polygon( OdInt32 numPoints, const OdGePoint3d* vertexList, const OdGeVector3d* pNormal );

  /** \details 
    Adds a raster image to hoops geometry.
    
    \param origin       [in] A three-dimensional point that determines the position where the image should be inserted.
    \param u            [in] A U vector of the image plane.
    \param v            [in] A V vector of the image plane.
    \param pImage       [in] A raster image to be inserted.
    \param uvBoundary   [in] A two-dimensional point that determines the boundary.
    \param numBoundPts  [in] A quantity of boundary points.
    \param transparency [in] A flag value that determines whether the image has transparency (if true) or not (if false).
    \param brightness   [in] An image brightness value.
    \param contrast     [in] An image contast value.
    \param fade         [in] An image fade value.
  */
  void dc_image( const OdGePoint3d& origin, const OdGeVector3d& u, const OdGeVector3d& v,
                 const OdGiRasterImage* pImage, const OdGePoint2d* uvBoundary, OdUInt32 numBoundPts,
                 bool transparency, double brightness, double contrast, double fade
                 );
                 
  /** \details 
    Adds a shell to hoops geometry.
    
    \param numVertices  [in] A quantity of the shell vertices.
    \param vertexList   [in] A pointer to an array of the shell vertices.
    \param faceListSize [in] A quantity of the shell faces.
    \param faceList     [in] A pointer to an array of the shell faces.
    \param pEdgeData    [in] A pointer to the information about shell edges.
    \param pFaceData    [in] A pointer to the information about shell faces.
    \param pVertexData  [in] A pointer to the information about shell vertices.
  */
  void dc_shell( OdInt32 numVertices, const OdGePoint3d* vertexList, OdInt32 faceListSize,
                 const OdInt32* faceList, const OdGiEdgeData* pEdgeData, const OdGiFaceData* pFaceData,
                 const OdGiVertexData* pVertexData );
                 
  /** \details 
    Adds a mesh to hoops geometry.
    
    \param numRows     [in] A quantity of the mesh rows.
    \param numColumns  [in] A quantity of the mesh columns.
    \param vertexList  [in] A pointer to an array of the mesh vertices.
    \param pEdgeData   [in] A pointer to the information about mesh edges.
    \param pFaceData   [in] A pointer to the information about mesh faces.
    \param pVertexData [in] A pointer to the information about mesh vertices.
  */
  void dc_mesh( OdInt32 numRows, OdInt32 numColumns, const OdGePoint3d* vertexList, 
                const OdGiEdgeData* pEdgeData, const OdGiFaceData* pFaceData, 
                const OdGiVertexData* pVertexData );
                
  /** \details 
    Opens a segment in a hoops file with a specified hoops key value.
    
    \param iSegmentKey [in] A segment key value.
  */
  void dc_open_segment( HC_KEY iSegmentKey );
  
  /** \details 
    Opens a segment in a hoops file with a specified name.
    
    \param strSegmentName [in] A segment name.
    
    \returns Returns the hoops key value of the opened segment.
  */
  HC_KEY dc_open_segment( OdString strSegmentName );
  
  /** \details 
    Closes the previously opened segment in a hoops file.
  */
  void dc_close_segment();

  /** \details 
    Creates a new layout segment in a hoops file with a specified name and makes it active.
    
    \param strName [in] A layout segment name.
    
    \returns Returns the hoops key value of the created layout segment.
  */
  HC_KEY createActiveLayoutSegment( OdString strName );
  
  /** \details 
    Creates a new invisible layout segment in a hoops file with a specified name.
    
    \param strName [in] A layout segment name.
    
    \returns Returns the hoops key value of the created layout segment.
  */
  HC_KEY createInvisibleLayoutSegment( OdString strName );

  //DOM-IGNORE-BEGIN
  // ----- The Overrides of OdGiGeometrySimplifier (OdGiConveyorGeometry) ----- \\
  //DOM-IGNORE-END

  virtual void polylineOut(OdInt32 numPoints, const OdGePoint3d* pVertexList);
  virtual void xlineProc( const OdGePoint3d& p1, const OdGePoint3d& p2 );
  virtual void rayProc( const OdGePoint3d& p1, const OdGePoint3d& p2 );

  virtual void circleProc(const OdGePoint3d& center,
    double radius, 
    const OdGeVector3d& normal,
    const OdGeVector3d* pExtrusion = 0);

  virtual void circleProc(const OdGePoint3d& firstPoint,
    const OdGePoint3d& secondPoint, 
    const OdGePoint3d& thirdPoint,
    const OdGeVector3d* pExtrusion = 0);

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

  virtual void ellipArcProc(
    const OdGeEllipArc3d& ellipArc,
    const OdGePoint3d* endPointOverrides = 0,
    OdGiArcType arcType = kOdGiArcSimple,
    const OdGeVector3d* pExtrusion = 0);

  virtual void textProc(const OdGePoint3d& position,
    const OdGeVector3d& u, 
    const OdGeVector3d& v,
    const OdChar* msg, 
    OdInt32 length, 
    bool raw,
    const OdGiTextStyle* pTextStyle,
    const OdGeVector3d* pExtrusion = 0);

  virtual void polygonOut(OdInt32 numPoints, 
    const OdGePoint3d* vertexList, 
    const OdGeVector3d* pNormal = 0);

  virtual void nurbsProc(const OdGeNurbCurve3d& nurbsCurve);

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

  virtual void polylineProc(OdInt32 numPoints, 
    const OdGePoint3d* vertexList,
    const OdGeVector3d* pNormal = 0, 
    const OdGeVector3d* pExtrusion = 0,
    OdGsMarker baseSubEntMarker = -1);

  virtual void shellProc(OdInt32 numVertices,
    const OdGePoint3d* vertexList,
    OdInt32 faceListSize,
    const OdInt32* faceList,
    const OdGiEdgeData* pEdgeData = 0,
    const OdGiFaceData* pFaceData = 0,
    const OdGiVertexData* pVertexData = 0);

  virtual void meshProc(OdInt32 numRows,
    OdInt32 numColumns,
    const OdGePoint3d* vertexList,
    const OdGiEdgeData* pEdgeData = 0,
    const OdGiFaceData* pFaceData = 0,
    const OdGiVertexData* pVertexData = 0);

  virtual void plineProc(
    const OdGiPolyline& lwBuf,
    const OdGeMatrix3d* pXform,
    OdUInt32 fromIndex,
    OdUInt32 numSegs);

  virtual void initTexture(
    const OdGePoint3d& origin, 
    const OdGeVector3d& u, 
    const OdGeVector3d& v,
    const OdGiRasterImage* pImage, 
    bool transparency, 
    double brightness,
    double contrast, 
    double fade );

  virtual void triangleOut( const OdInt32* p3Vertices, const OdGeVector3d* pNormal );

  /** \details 
    Implements a reaction to changing any attributes of hoops graphics elements, such as color, linetypes, layer and so on.
    
    \param traits [in] A traits information object.
  */
  virtual void onTraitsModified(const OdGiSubEntityTraitsData& traits);

//DOM-IGNORE-BEGIN
private:

  enum hoopsObjectType 
  {
    kHoopsLine   = 1,
    kHoopsXLine  = 2,
    kHoopsRay    = 3,
    kHoopsShape  = 4,
    kHoopsLType  = 5,
    kHoopsCircle = 6,
    kHoopsEllipse = 7,
    kHoopsNurbs  = 8,
    kHoopsText   = 9,
    kHoopsFontStyle = 10,
    kHoopsPolygon = 11,
    kHoopsImageDef = 12,
    kHoopsImage    = 13,
    kHoopsTexture  = 14,
    kHoopsMesh     = 15,
    kHoopsShell    = 16,
    kHoolsLight    = 17,
    kHoopsViewport = 18
  };

  CHoopsExportImpl*           m_pHoops;
  OdGiSubEntityTraitsData     m_curTraits;
  OdGsDevicePtr               m_pRenderDevice;
  OdGeVector3d                m_vrNormal;
  bool                        m_bInitNormal;
  bool                        m_bRecording;

  OdArray<CHoopsExtentsKeeper> m_extentsArr;

  //
  HoopsMaterialInfo           m_MaterialInfo;
  OdRxObjectPtr               m_pCurMapperItem;
  bool                        m_bSkipLines;
  bool                        m_bTriangleOut;
  OdInt32                     m_iTriangleIndexes[3];
  const OdGiVertexData*       m_pShellVertexData;
  //

  bool                        m_bRayExport;
  bool                        m_bXLineExport;
  bool                        m_bExportGeometryOnly;

  OdString defineHoopsObjectName( hoopsObjectType iObjType, bool bSkipCheck = true );

  OdString getHoopsLineType( OdDbStub* pLineType, double dLTypeScale, OdDbStub* pLayer, bool& bShapes );
  void     AddGlyph( OdGiLinetypeDash curDash );
  bool     GetHoopsGlyph( OdGiLinetypeDash curDash, HoopsGlyphsPair& glyphInfo );
  bool     DefineHoopsGlyph( OdGiTextStyle curGlyphInfo, OdGiLinetypeDash curDash, 
                             OdString& strGlyphName, double& dLength );
  OdString DefineHoopsLineType( OdGiLinetype& curLS, double dLTypeScale, bool& bShapesInLT );
  OdString GetHoopsFontStyle( const OdGiTextStyle* pTextStyle, bool bRaw, double& dScaleFactor );
  OdString DefineHoopsFontStyle( const OdGiTextStyle* pTextStyle, bool bRaw, double& dScaleFactor );

  void     setHoopsTextExtents( OdInt64 TextKey, const OdGiTextStyle* pTextStyle, double dBaseSize, 
                                const OdChar* msg, OdInt32 length, OdGePoint3d ptInsert, OdGeVector3d u, 
                                OdGeVector3d v, double dScaleFactor, double dAngle = 0 );

  bool     getHoopsImage( HoopsImagePair& imageInfo );
  bool     DefineHoopsImage( HoopsImagePair& newImagePair );
  void     setHoopsMaterial(const OdGiSubEntityTraitsData& Traits, bool b3dObjectMaterial = true );
  OdString CreateHoopsTexture();
  OdString getHoopsTextureImage( OdString strFileName, double dFade );
  bool     CreateHoopsTextureImage( OdString strFileName, OdString strTextureName, double dFade );
  bool     IsLayerVisible( OdDbStub* layerId );

  void     setHoopsPolygonColor( const OdCmEntityColor& color );
  void     setHoopsLineColor( const OdCmEntityColor& color );
  void     setHoopsTextColor( const OdCmEntityColor& color );
  void     setHoopsVertexColor( const OdCmEntityColor& color );
  void     setHoopsEdgeColor( const OdCmEntityColor& color );
  void     setHoopsFaceColor( const OdCmEntityColor& color );

  void addHoopsPointLight( const OdString& strSegName, OdGePoint3d ptPos, 
    OdCmEntityColor color, double dIntensity, bool bOn );
  void addHoopsSpotLight(  const OdString& strSegName, OdGePoint3d ptPos, OdGePoint3d ptTarget, 
    double dHotSpot, double dFallOff, OdCmEntityColor color, 
    double dIntensity, bool bOn );
  void addHoopsDistantLight( const OdString& strSegName, OdGeVector3d vrDir,
    OdCmEntityColor color, double dIntensity, bool bOn );

  OdUInt32 getPaletteSize() const;

  friend class OdHoopsExportView;
//DOM-IGNORE-END
};
}
//////////////////////////////////////////////////////////////////////

#endif // #ifndef _HOOPS_EXPORT_DEVICE_INCLUDED_
