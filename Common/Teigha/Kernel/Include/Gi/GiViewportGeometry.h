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

#ifndef _OD_GIVIEWPORT_GEOMETRY_H_
#define _OD_GIVIEWPORT_GEOMETRY_H_

#include "Gi/Gi.h"
#include "Gi/GiGeometry.h"

class OdGePoint3d;
class OdGeVector3d;
class OdGeMatrix2d;
class OdGiRasterImage;
class OdGiMetafile;
class OdGiCommonDraw;
class OdGsDCRect;

#include "TD_PackPush.h"

/** \details
  This class defines the interface for objects that draw themselves.
  <group OdGi_Classes> 
*/
class FIRSTDLL_EXPORT OdGiSelfGdiDrawable : public OdRxObject
{
public:
  ODRX_DECLARE_MEMBERS(OdGiSelfGdiDrawable);

  /** \details
    This function is called to have a GDI self-drawable to draw itself.
    Returns false if implementation is missing or in error case.

    \param drawObj [in]  Object to be drawn.
    \param hdc [in]  Handle to the device context.
    \param screenRect [in]  Screen rectangle in device coordinates (pixels).
  */
  virtual bool draw(const OdGiCommonDraw& drawObj,
                    void* hdc,
                    const OdGsDCRect& screenRect) const = 0;
};

/** \details
  This abstract class represents a protocol extension for the OdGiSelfGdiDrawable interface.
  <group OdGi_Classes> 
*/
class FIRSTDLL_EXPORT OdGiSelfGiDrawablePE : public OdRxObject
{
public:
  ODRX_DECLARE_MEMBERS(OdGiSelfGiDrawablePE);

  // return false to create HDC and draw via OdGiSelfGdiDrawable interface
  /** \details
    Returns false to create an HDC (handle to the device context) and the draw object via the OdGiSelfGdiDrawable interface.
    
    \param pRxObject [in] Pointer to a drawn object.
    \param pDestGeom [in] Pointer to the conveyor object. 
    \param origin [in] Drawn object's origin.
    \param u [in] Drawn object's width vector.
    \param v [in] Drawn object's height vector.
    \param screenRect [in] Screen rectangle in device coordinates (pixels).
    \param isExport [in] Boolean flag, which indicates that image transparency is on.
  */
  virtual bool draw(OdRxObject* pRxObject, 
                    class OdGiConveyorGeometry* pDestGeom,
                    const OdGePoint3d& origin,
                    const OdGeVector3d& u, const OdGeVector3d& v,
                    const OdGsDCRect& screenRect,
                    bool isExport = false) const = 0;
};

/** \details
    This class defines functions that allow entities to vectorize themselves.
    
    \remarks
    Vectorization with the methods of this class are viewport dependent. 
    For viewport specific vectorization, use subViewportDraw() instead of subWorldDraw().

    An OdGiViewportGeometry object passed to the subViewportDraw()
    method of an entity.

    Model coordinates are applied to all geometry in this class except for setExtents(). 
   
    You can obtain objects of the following classes fromOdGiViewportDraw:

    <table>
    Class                 Description
    OdGiSubEntityTraits   Control of drawing attributes and selection markers. 
    OdGiViewportGeometry  Drawing model coordinate geometry and transforms. 
    </table>

    \sa
    TD_Gi
    
    <group OdGi_Classes> 
*/
class FIRSTDLL_EXPORT OdGiViewportGeometry : public OdGiGeometry
{
public:
  ODRX_DECLARE_MEMBERS(OdGiViewportGeometry);

  /** \details
    Draws a polyline, with coordinates specified in Eye Coordinates.

    \param numVertices [in]  Number of vertices.
    \param vertexList [in]  Array of vertices.

    \remarks
    This polyline is drawn with a continuous linetype.

    \sa
    Coordinate Systems
  */
  virtual void polylineEye(
    OdUInt32 numVertices, 
    const OdGePoint3d* vertexList) = 0;

  /** \details
    Draws a filled polygon, with coordinates specified in Eye Coordinates.

    \param numVertices [in]  Number of vertices in the polygon.
    \param vertexList [in]  Vertices of the polygon.

    \sa
    Coordinate Systems
  */
  virtual void polygonEye(
    OdUInt32 numVertices, 
    const OdGePoint3d* vertexList) = 0;

  /** \details
    Draws a polyline, with coordinates specified in normalized device Coordinates.

    \param numVertices [in]  Number of vertices in the polyline.
    \param vertexList [in]  Vertices of the polyline.

    \remarks
    This polyline is drawn with a continuous linetype.

    \sa
    Coordinate Systems
  */
  virtual void polylineDc(
    OdUInt32 numVertices, 
    const OdGePoint3d* vertexList) = 0;

  /** \details
    Draws a filled polygon, with coordinates specified in normalized device Coordinates.

    \param numVertices [in]  Number of vertices in the polygon.
    \param vertexList [in]  Vertices of the polygon.

    \sa
    Coordinate Systems
  */
  virtual void polygonDc(
    OdUInt32 numVertices, 
    const OdGePoint3d* vertexList) = 0;

  /** \details
    Draws the specified raster image.

    \param origin [in]  Image origin.
    \param u [in]  Image width vector.
    \param v [in]  Image height vector.
    \param pImage [in]  Pointer to the image object.
    \param uvBoundary [in]  Array of image boundary points (may not be null).
    \param numBoundPts [in]  Number of boundary points.
    \param transparency [in]  True if and only if image transparency is on.
    \param brightness [in]  Image brightness [0 .. 100].
    \param contrast [in]  Image contrast [0 .. 100].
    \param fade [in]  Image fade value [0 .. 100].

    \remarks
    All coordinates are in Model coordinates.
  */
  virtual void rasterImageDc(
    const OdGePoint3d& origin,
    const OdGeVector3d& u,
    const OdGeVector3d& v,
    const OdGiRasterImage* pImage, 
    const OdGePoint2d* uvBoundary, 
    OdUInt32 numBoundPts,
    bool transparency = false,
    double brightness = 50.0,
    double contrast = 50.0,
    double fade = 0.0) = 0;

  /** \details
    Draws the specified Windows Metafile.

    \param origin [in]  Metafile origin.
    \param u [in]  Metafile width vector.
    \param v [in]  Metafile height vector.
    \param pMetafile [in]  Pointer to the Metafile object.
    \param dcAligned [in]  reserved.
    \param baAllowClipping [in]  reserved.

    \remarks
    All coordinates are in Model coordinates.
  */
  virtual void  metafileDc(
    const OdGePoint3d& origin,
    const OdGeVector3d& u,
    const OdGeVector3d& v,
    const OdGiMetafile* pMetafile,
    bool dcAligned = true,
    bool allowClipping = false) = 0;

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
    
    \remarks
    ownerDrawDc is not conveyor primitive, so Teigha's rendering framework performs no processing 
    (transformation, linetyping, or clipping) for this object. 

    All coordinates are in Model coordinates.

    \sa
    OdGiSelfGdiDrawable.
  */
  virtual void ownerDrawDc(
    const OdGePoint3d& origin,
    const OdGeVector3d& u,
    const OdGeVector3d& v,
    const OdGiSelfGdiDrawable* pDrawable,
    bool dcAligned = true,
    bool allowClipping = false);
};

#include "TD_PackPop.h"

#endif
