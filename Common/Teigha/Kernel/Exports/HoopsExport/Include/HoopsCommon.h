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


///////////////////////////////////////////////////////////////////////////////
//
// HoopCommon.h - Open Design Hoops Export definitions
//
///////////////////////////////////////////////////////////////////////////////

#ifndef _HOOPS_COMMON_INCLUDED_
#define _HOOPS_COMMON_INCLUDED_

#include "HoopsExportCommon.h"
#include "Gs/GsBaseVectorizer.h"

#include "BoolArray.h"
#include "HoopsExportImpl.h"
#include "HoopsStringsDef.h"
#include "Gi/GiLinetype.h"
#include "Gi/GiGeometrySimplifier.h"
#include "Gi/GiRasterImage.h"

#include "Ge/GeLine3d.h"
#include "Ge/GeCircArc3d.h"
#include "Ge/GeEllipArc3d.h"
#include "Ge/GeNurbCurve3d.h"

//////////////////////////////////////////////////////////////////////

/** \details
  This namespace represents structures, classes and functions, that implement functionality for exporting to a hoops file.
  
  <group OdExport_Classes> 
*/
namespace TD_HOOPS_COMMON_EXPORT {

/** \details
  This structure implements the hoops three-dimensional point.
*/
struct HoopsPoint
{
  float x; //!< Coordinate X of the three-dimensional point.
  float y; //!< Coordinate Y of the three-dimensional point. 
  float z; //!< Coordinate Z of the three-dimensional point.
};

/** \details
  This structure implements the hoops two-dimensional point.
*/
struct Hoops2dPoint
{
  float x; //!< Coordinate X of the two-dimensional point.
  float y; //!< Coordinate Y of the two-dimensional point.
};

/** \details
  This structure implements the hoops two-dimensional point.
*/
struct HoopsD2dPoint
{
  double x; //!< Coordinate X of the two-dimensional point.
  double y; //!< Coordinate Y of the two-dimensional point.
};

/** \details
  This structure implements the three-dimensional glyph geometry information.
*/
struct CGlyphGeometryInfo
{
  OdGePoint3dArray pVertices; //!< Array of vertices.
  bool             bFillMode; //!< Fill flag of the geometry.
};

/** \details
  This structure implements the two-dimensional glyph geometry information.
*/
struct CGlyph2dGeometryInfo
{
  OdGePoint2dArray pVertices; //!< Array of vertices.
  bool             bFillMode; //!< Fill flag of the geometry.
};

/** \details
  This class implements the two-dimensional glyph geometry.
  
  <group OdExport_Classes> 
*/
class CGlyph2dGeometry
{
public:
  /** \details 
    Creates an instance of two-dimensional glyph geometry.
  */
  CGlyph2dGeometry();
  
  /** \details 
    Destroys an instance of two-dimensional glyph geometry.
  */
  virtual ~CGlyph2dGeometry();
  
  /** \details 
    Sets a new maximum size value.
    
    \param iSize [in] A new maximum size value to be set.
  */
  void setMaxSize( OdInt32 iSize );
  
  /** \details 
    Returns the current maximum size value.
  */
  OdInt32 maxSize();

  /** \details 
    Returns the current region length.
  */
  double getRegionLength();
  
  /** \details 
    Returns the current region height.
  */
  double getRegionHeight();
  
  /** \details 
    Returns the current maximum Y coordinate of the region.
  */
  double getMaxY();
  
  /** \details 
    Returns the current minimum Y coordinate of the region.
  */
  double getMinY();
  
  /** \details 
    Sets a new length value through minimum and maximum coordinates at the X axis. 
    
    \param dXMin [in] A minimum X coordinate value. 
    \param dXMax [in] A maximum X coordinate value.
  */
  void setLength( double dXMin, double dXMax );
  
  /** \details 
    Sets a new height value through minimum and maximum coordinates at the Y axis. 
    
    \param dYMin [in] A minimum Y coordinate value. 
    \param dYMax [in] A maximum Y coordinate value.
  */
  void setHeight( double dYMin, double dYMax );
  
  /** \details 
    Adds a new three-dimensional geometry item. 
    
    \param pInfo    [in] A new geometry item to be added. 
    \param bKeepXY  [in] A flag that determines whether coordinates are kept.
  */
  void Add3dGeometryItem( CGlyphGeometryInfo pInfo, bool bKeepXY = false);
  
  /** \details 
    Returns minimum and maximum points of the region. 
    
    \param ptMin [out] A reference to an OdGePoint2d object used to return the minimum point.
    \param ptMax [out] A reference to an OdGePoint2d object used to return the maximum point.
  */
  void getMinMax( OdGePoint2d& ptMin, OdGePoint2d& ptMax );
  
  /** \details 
    Rotates the geometry object using a specified angle. 
    
    \param dAngle         [in] A rotation angle.
    \param bUpdateExtents [in] A flag that determines whether extents should be updated after the rotation (if true, extents are updated after rotation).
  */
  void rotateByAngle( double dAngle, bool bUpdateExtents = true );
  
  /** \details 
    Fixes the center of the glyph geometry object. 
  */
  void fixGlyphCenter();
  
  /** \details 
    Returns the current glyph geometry object center.
  */
  OdGePoint2d getGlyphCenter();
  
  /** \details 
    Clips the glyph geometry object to a region specified with its size.
    
    \param dSize              [in] A clip region size.
    \param bMoveCenterToZero  [in] A flag that determines whether the center is moved to the origin.
    \param bUpdateExtents     [in] A flag that determines whether extents should be updated before clipping.
    
    \returns Returns the scale value to be implemented for clipped geometry.
    
    \remarks 
    The values of the dSize parameter determines the scale. If bMoveCenterToZero is true, the clipped rectangle is centered around the origin of the coordinate system.
    Otherwise the mininum point of the region is moved to the coordinate system origin.
    The returned scale value also depends on the bMoveCenterToZero flag value. If the flag is false, the scale value is determined as the result of dividing dSize by the maximum length of the region's side.
    If the flag is true, the scale value is equal to the result of dSize multiplicated by 2 divided by the maximum length of the region's side.
  */
  double ClipToRegion( double dSize, bool bMoveCenterToZero = true, bool bUpdateExtents = true );
  
  /** \details 
    Returns the character's quantity used for glyph definition.
  */
  OdUInt32 getnCharsForGlyphDefine();
  
  /** \details 
    Returns the current item's quantity in the glyph geometry object.
  */
  OdInt32 getnItems();

  /** \details 
    Returns the specified item from the glyph geometry object.
    
    \param iNum    [in]  A number (index) of an item to retrieve the information about.
    \param p2dInfo [out] A reference to a CGlyph2dGeometryInfo structure instance to store and return the specified item information.
    
    \returns Returns true if the information has been successfully returned, otherwise returns false.
  */
  bool getItem( OdUInt32 iNum, CGlyph2dGeometryInfo& p2dInfo );

//DOM-IGNORE-BEGIN
private:

  double  m_dXMin;
  double  m_dXMax;
  double  m_dYMin;
  double  m_dYMax;
  OdInt32 m_iMaxSize;
  OdGePoint2d m_ptCenter;
  bool    m_bFixedCenter;
  OdArray<CGlyph2dGeometryInfo> m_arrVerticesData;

  void     Convert3dPtArrayForGlyphDefine( OdGePoint3dArray arr3dPts, OdGePoint2dArray& arr2dPts, bool bKeepXY = false );
  void     reducePolylinePoints( OdGePoint2dArray& arr2dPts );
//DOM-IGNORE-END
};

/** \details
  This class implements the hoops glyph writer.
  
  <group OdExport_Classes> 
*/
class CHoopsGlyphWriter : public OdGiGeometrySimplifier
{
public:
  /** \details 
    Creates an instance of a hoops glyph writer.
  */
  CHoopsGlyphWriter() { dScale = 1; bTTF = false; dXOffset = dYOffset = 0; };
  
  /** \details 
    Destroys the hoops glyph writer object.
  */
  virtual ~CHoopsGlyphWriter(){};

  OdArray<CGlyphGeometryInfo> arrGlyphGeometry; //!< An array of glyph geometry information.
  double dScale;                                //!< A scale value used for geometry.
  double dXOffset;                              //!< An offset from the origin at the X-axis.
  double dYOffset;                              //!< An offset from the origin at the Y-axis.
  bool   bTTF;                                  //!< A flag that determines whether glyph geometry uses true type fonts (TTF). If TTFs are used, the value is equal to true, otherwise equal to false.

  ///////////////////////////////////////////////////////////////////

  /** \details 
    Resets the glyph writer parameters: sets the scale value to 1, sets the offset to zero, and clears the array of geometry information.
  */
  void reset()
  {
    dXOffset = 0;
    dYOffset = 0;
    dScale   = 1;

    for( OdUInt32 i = 0; i < arrGlyphGeometry.size(); i++)
      arrGlyphGeometry[i].pVertices.clear();

    arrGlyphGeometry.clear();
  }

  ///////////////////////////////////////////////////////////////////

  /** \details 
    Adds a polyline to the glyph geometry.
    
    \param nbPoints     [in] A number of polyline vertices.
    \param pVertexList  [in] An array of three-dimensional points, which are the polyline vertices.
  */
  virtual void polylineOut( OdInt32 nbPoints, const OdGePoint3d* pVertexList )
  {   
    bool bFillMode = drawContext()->effectiveTraits().fillType()==kOdGiFillAlways;

    CGlyphGeometryInfo tmpRecord;

    if( bTTF && giCtx().fillTtf())
      tmpRecord.bFillMode = true;
    else
      tmpRecord.bFillMode = bFillMode;

    tmpRecord.bFillMode = false;

    for( OdInt32 i = 0; i<nbPoints; i++ )
    {
      OdGePoint3d tmp = pVertexList[i];
      tmp.x *= dScale;
      tmp.y *= dScale;
      tmp.z *= dScale;
      tmp.x += dXOffset;
      tmp.y += dYOffset;
      tmpRecord.pVertices.push_back(tmp);
    }

    arrGlyphGeometry.push_back(tmpRecord);
  }

  /////////////////////////////////////////////////////////////////////
  
  /** \details 
    Adds a polygon to the glyph geometry.
    
    \param numPoints    [in] A number of polygon vertices.
    \param vertexList   [in] An array of three-dimensional points, which are the polygon vertices.
  */
  virtual void polygonOut( OdInt32 numPoints, const OdGePoint3d* vertexList, const OdGeVector3d* /*pNormal*/ )
  {   
    if( !OdZero( vertexList[0].distanceTo(vertexList[numPoints])) )
    {
      OdGePoint3d* pNewPts = new OdGePoint3d[ numPoints + 1 ];
      memcpy( pNewPts, vertexList, numPoints * sizeof(OdGePoint3d) );
      pNewPts[numPoints] = pNewPts[0];
      polylineOut( numPoints+1, pNewPts );
      delete[] pNewPts;
    }
    else
      polylineOut( numPoints, vertexList );
  }

  TD_USING(OdGiGeometrySimplifier::polylineOut);
};

//////////////////////////////////////////////////////////////////////////////////

/** \details 
  Converts a Teigha string (OdString object) to a pointer on char.
  
  \param numPoints    [in] A string to be converted.
*/
char* OdStringToPChar( OdString strData );

/** \details 
  Update shell list of faces.
  
  \param faceList     [in] A pointer to a face list.
  \param faceListSize [in] A face list size (count of elements of face list).
  \param vertexList   [in] An array of three-dimensional points that form faces.
  \param numVertices  [in] A quantity of vertices in the vertexList array.
*/
void  UpdateShellFaceList( OdInt32* faceList, OdInt32 faceListSize, 
                           const OdGePoint3d* vertexList, OdInt32 numVertices );

/** \details 
  Sets a normal vector for the hoops engine.
  
  \param normal [in] A three-dimensional vector to be set as a normal.
*/                           
void  setHoopsVertexNormal( OdGeVector3d normal );

/** \details 
  Sets whether edges are visible in the hoops engine.
  
  \param bShow [in] A flag value that determines the visibility of edges (true if edges are visible, otherwise false).
*/ 
void  setHoopsEdgeVisibility( bool bShow );

/** \details 
  Sets a normal vector for faces in the hoops engine.
  
  \param normal [in] A three-dimensional vector to be set as a normal.
*/
void  setHoopsFaceNormal( OdGeVector3d normal );

/** \details 
  Sets whether faces are visible in the hoops engine.
  
  \param bShow [in] A flag value that determines the visibility of faces (true if faces are visible, otherwise false).
*/ 
void  setHoopsFaceVisibility( bool bShow );

/** \details 
  Sets a lineweight value for the hoops engine.
  
  \param dLineWeight [in] A lineweight value to be set.
*/ 
void  setHoopsLineWeight( double dLineWeight );

/** \details 
  Sets a linetype value for the hoops engine.
  
  \param strLineType [in] A linetype string value.
*/ 
void  setHoopsLineType( OdString strLineType );

/** \details 
  Sets a text size for the hoops engine.
  
  \param dTextSize [in] A text size value to be set.
*/ 
void  setHoopsTextSize( double dTextSize );

/** \details 
  Sets a font name to be used by the hoops engine.
  
  \param strFontName  [in] A font name.
  \param bTTF         [in] A flag value that determines whether the specified font in strFontName is a true type font (if true) or not (if false).
*/
void  setHoopsFontName( OdString strFontName, bool bTTF );

/** \details 
  Sets the three dimensional mode for the hoops engine.
  
  \param bText3d [in] A flag value that determines whether the three-dimensional mode is on (if true) or off (if false).
*/
void  setText3dMode( bool bText3d = true );

/** \details 
  Sets the default alignment for the hoops engine.
*/
void  setHoopsDefaultAlignment();

/** \details 
  Sets an obliquing angle for the hoops engine.
  
  \param dAngle [in] An angle value to be set.
*/
void  setHoopsObliquingAngle( double dAngle );

/** \details 
  Sets a scaling factor for the hoops engine fonts.
  
  \param dScale [in] A scale factor value to be used.
*/
void  setHoopsFontWidthFactor( double dScale );

/** \details 
  Sets the bold mode for the hoops engine fonts.
  
  \param bBold [in] A flag value that determines whether the font is bold (if true) or not (if false).
*/
void  setHoopsFontBold( bool bBold );

/** \details 
  Sets the italic mode for the hoops engine fonts.
  
  \param bItalic [in] A flag value that determines whether the font is italic (if true) or not (if false).
*/
void  setHoopsFontItalic( bool bItalic );

/** \details 
  Sets the underlined mode for the hoops engine fonts.
  
  \param bUnderlined [in] A flag value that determines whether the font is underlined (if true) or not (if false).
*/
void  setHoopsFontUnderlined( bool bUnderlined );

/** \details 
  Sets the overlined mode for the hoops engine fonts.
  
  \param bOverlined [in] A flag value that determines whether the font is overlined (if true) or not (if false).
*/
void  setHoopsFontOverlined( bool bOverlined );

/** \details 
  Sets a new line orientation for the hoops engine.
  
  \param iKey   [in] A hoops object unique key of a line.
  \param ptsArr [in] An array of three-dimensional points that defines the line orientation.
*/
void  setHoopsLineOrientation( HC_KEY iKey, OdGePoint3dArray ptsArr );

/** \details 
  Sets a new line orientation for the hoops engine using a normal vector.
  
  \param iKey   [in] A hoops object unique key of a line.
  \param ptsArr [in] A three-dimensional normal vector used for line orientation.
*/
void  setHoopsOrientationByNormal( HC_KEY iKey, OdGeVector3d vrNorm );

/** \details 
  Opens a segment of a hoops file with a specified name or path.
  
  \param strSegment [in] A hoops object segment name (path).
*/
void  OpenHoopsSegment( OdString strSegment );

/** \details 
  Resets the hoops engine.
*/
void  ResetHoops();

/** \details 
  Searches a hoops glyph object for a specified name in a specified segment.
  
  \param iKey         [in] A key of a segment where to search.
  \param strGlyphName [in] A name of the desired glyph object.
  
  \returns Returns true if a specified glyph object has been found in a specified segment. Otherwise returns false.
*/
bool  isHoopsGlyphExists( HC_KEY iKey, OdString strGlyphName );

/** \details 
  Searches a linetype for a specified name in a specified segment.
  
  \param iKey         [in] A key of a segment where to search.
  \param strLTypeName [in] A name of the desired linetype.
  
  \returns Returns true if a specified linetype has been found in a specified segment. Otherwise returns false.
*/
bool  isHoopsLTypeExists( HC_KEY iKey, OdString strLTypeName );

/** \details 
  Searches a font for a specified name in a specified segment.
  
  \param iKey         [in] A key of a segment where to search.
  \param strFontName  [in] A name of the desired font.
  
  \returns Returns true if a specified font has been found in a specified segment. Otherwise returns false.
*/
bool  isHoopsFontExists( HC_KEY iKey, OdString strFontName );

/** \details 
  Searches a texture for a specified name in a specified segment.
  
  \param iKey           [in] A key of a segment where to search.
  \param strTextureName [in] A name of the desired texture.
  
  \returns Returns true if a specified texture has been found in a specified segment. Otherwise returns false.
*/
bool  isHoopsTextureExists( HC_KEY iKey, OdString strTextureName );

/** \details 
  Searches a segment for a specified name in a specified parent segment.
  
  \param iKey           [in] A key of a parent segment where to search.
  \param strSegmentName [in] A name of the desired segment.
  
  \returns Returns true if a specified segment has been found in a specified parent segment. Otherwise returns false.
*/
bool  isHoopsSegmentExists( HC_KEY iKey, OdString strSegmentName );

/** \details 
  Returns the scale value used for a specified true type font.
  
  \param iKey           [in] A pointer to a font object (an instance of the OdFont class).
*/
double getTtfScale( const OdFont* pFont );

/** \details 
  Returns the planar entity for a specified array of three-dimensional points.
  
  \param GlyphLinePts [in] A pointer to a font object (an instance of the OdFont class).
  \param PlaneEnt     [in] A reference to an OdGePlane object to fill and return information about the planar entity.
  \param bKeepXY      [in] A flag that determines whether X and Y coordinates are kept.
  
  \return Returns true if a planar entity has been successfully created, otherwise returns false.
*/
bool   getPlanarEntity( OdGePoint3dArray GlyphLinePts, OdGePlane& PlaneEnt, bool bKeepXY = false );

/** \details 
  Sets a new clip region with an array of three-dimensional points.
  
  \param ptsClipBoundary [in] An array of three-dimensional points that determines the clipping boundaries.
*/
void HoopsSetClipRegion( OdGePoint3dArray ptsClipBoundary);

/** \details 
  Sets parameters of a camera for the hoops engine.
  
  \param ptCameraPos    [in] A three-dimensional point that determines the camera position.
  \param ptCameraTarget [in] A three-dimensional point that determines the camera target.
  \param ptCameraTarget [in] A three-dimensional vector that determines moving of the camera's vertical.
  \param dCameraWidth   [in] Width of the camera.
  \param dCameraHeight  [in] Height of the camera.
  \param bPerspective   [in] A flag value that determines whether the camera is in perspective (if true) or in parallel (if false) mode.
  \param bStereoView    [in] A flag value that determines whether the camera is in stereo mode. If this flag is equal to true, the camera is in stereo mode.
  \param dStereoDist    [in] A distance value for stereo mode. Currently this parameter is not used.
  \param dStereoAngle   [in] An angle value for stereo mode. Currently this parameter is not used.
*/
void HoopsSetCamera( OdGePoint3d ptCameraPos, OdGePoint3d ptCameraTarget, 
                     OdGeVector3d vrCameraUp, double dCameraWidth, double dCameraHeight, 
                     bool bPerspective, bool bStereoView, double dStereoDist, double dStereoAngle);

/** \details 
  Exports a specified three-dimensional arc to hoops format.
  
  \param circArc              [in] An arc object to be exported.
  \param bExportGeometryOnly  [in] A flag value that determines whether only geometry is exported (if true).
  \param bSkipTransform       [in] A flag value that determines whether arc transformations shoud be skipped (if true) or not (if false).
*/
void HoopsExportCircleArc( OdGeCircArc3d circArc, bool bExportGeometryOnly = false, bool bSkipTransform = false );

/** \details 
  Exports a specified three-dimensional ellipse to hoops format.
  
  \param ellipArc             [in] An ellipse object to be exported.
  \param bExportGeometryOnly  [in] A flag value that determines whether only geometry is exported (if true).
  \param bSkipTransform       [in] A flag value that determines whether arc transformations shoud be skipped (if true) or not (if false).
*/
void HoopsExportEllipArc( OdGeEllipArc3d ellipArc, bool bExportGeometryOnly = false, bool bSkipTransform = false );

/** \details 
  Exports a specified NURBS curve to hoops format.
  
  \param nurbsCurve [in] A NURBS curve to be exported.
*/
void HoopsExportNurbs( OdGeNurbCurve3d nurbsCurve );

/** \details 
  Exports a specified polygon to hoops format.
  
  \param numPoints [in] A quantity of vertices in a polygon.
  \param pPointArr [in] An array of the polygon's vertices.
  
  \returns Returns the hoops object key of the exported polygon.
*/
HC_KEY HoopsExportPolygon( OdInt32 numPoints, HoopsPoint* pPointArr );

/** \details 
  Exports a specified shell to hoops format.
  
  \param numPoints    [in] A quantity of vertices in a polygon.
  \param pPointArr    [in] An array of the polygon's vertices.
  \param faceListSize [in] Face array size.
  \param faceList     [in] Face list.
  
  \returns Returns the hoops object key of the exported shell.
*/
HC_KEY HoopsExportShell( OdInt32 numPoints, HoopsPoint* pPointArr,  OdInt32 faceListSize, const OdInt32* faceList );

/** \details 
  Exports a specified mesh to hoops format.
  
  \param numRows    [in] A quantity of rows in a mesh.
  \param numColumns [in] A quantity of columns in a mesh.
  \param pPointArr  [in] An array of mesh points.
  
  \returns Returns the hoops object key of the exported mesh.
*/
HC_KEY HoopsExportMesh( OdInt32 numRows, OdInt32 numColumns, const HoopsPoint* pPointArr );

/** \details 
  Creates a raster image in a hoops file.
  
  \param strShellName   [in] A name of the shell segment where the image should be created.
  \param strImageName   [in] An image name.
  \param strTextureName [in] A texture name.
  \param dWidth         [in] Resulting image width.
  \param dHeight        [in] Resulting image height.
  \param ptsShellArr    [in] An array of three-dimensional points that determine the shell.
  \param ptsTextureArr  [in] An array of two-dimensional points that determine the texture.
*/
void CreateRasterImage(OdString strShellName, OdString strImageName, OdString strTextureName, double dWidth,
                       double dHeight, OdGePoint3dArray ptsShellArr, OdGePoint2dArray ptsTextureArr );

/** \details 
  Sets an ambient color value for the hoops engine.
  
  \param clrAmbient [in] A new ambient color value to be set.
*/
void setHoopsAmbientColor( ODCOLORREF clrAmbient );

/** \details 
  Sets a diffuse color value for the hoops engine. 
  
  \param clrDiffuse [in] A new diffuse color value to be set.
*/
void setHoopsDiffuseColor( ODCOLORREF clrDiffuse );

/** \details 
  Sets a specular color value for the hoops engine.
  
  \param clrSpecular [in] A new specular color value to be set.
*/
void setHoopsSpecularColor( ODCOLORREF clrSpecular );

/** \details 
  Sets a gloss level for the hoops engine. 
  
  \param dGloss [in] A new gloss level value to be set.
*/
void setHoopsGloss( double dGloss );

/** \details 
  Sets a transparency level for the hoops engine. 
  
  \param dTransFactor [in] A new transparency level value to be set.
*/
void setHoopsTransparency( double dTransFactor );

/** \details 
  Sets a background color value for the hoops engine.
  
  \param clrBackground [in] A new background color value to be set.
*/
void setHoopsBgColor( ODCOLORREF clrBackground );

/** \details 
  Inserts a raster image in a hoops image segment.
  
  \param pImage       [in] A smart pointer to an OdGiRasterImage object to be inserted.
  \param strImageName [in] An inserted image name in a hoops file.
  \param strImageName [in] An image name in a hoops file.
  \param bSetRealSize [in] A flag value that determines whether the image keeps its size in the hoops file (if true) or not (if false).
  \param bForseBGR    [in] A flag value that determines whether the BGR pixel format for the image is kept after adding the image to a hoops file.
  
  \returns Returns true if an image has been successfully inserted, otherwise returns false.
  
  \remarks 
  The value of the bForseBGR parameter is ignored if the pImage object already has the BGR or BGRA pixel format.
*/
bool InsertHoopsImage( OdGiRasterImagePtr pImage, OdString strImageName, bool bSetRealSize = false, 
                       bool bForseBGR = false );
}
#endif // _HOOPS_COMMON_INCLUDED_

