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




#ifndef __IMGENT_H
#define __IMGENT_H /*!DOM*/

#include "TD_PackPush.h"

#include "Ge/GePoint2dArray.h"

class OdRasterImageImpl;

#include "DbImage.h"
#include "DbRasterImageDef.h"

/** \details
    This class represents RasterImage entities in an OdDbDatabase instance.
    Corresponding C++ library: ISM

    \sa
    TD_Db
    
    <group OdDb_Classes>
*/
class ISM_EXPORT OdDbRasterImage : public OdDbImage
{
public:
  ODDB_DECLARE_MEMBERS(OdDbRasterImage);

  OdDbRasterImage();

  enum ClipBoundaryType
  {
    kInvalid, // Invalid
    kRect,    // Rectangular
    kPoly     // Polygonal
  };

  enum ImageDisplayOpt
  {
    kShow          = 1,   // Show
    kShowUnAligned = 2,   // Show unaligned
    kClip          = 4,   // Clip
    kTransparent   = 8    // Transparent
  };

  /** \details
    Sets the Object ID of the OdDbRasterImageDef object associated with this raster image entity (DXF 340).
    \param imageDefId [in]  Object ID of image to be displayed.
  */
  virtual void setImageDefId(
    OdDbObjectId imageDefId);

  /** \details
    Returns the Object ID of the OdDbRasterImageDef object associated with this raster image entity (DXF 340).
  */
  virtual OdDbObjectId imageDefId() const;

  /** \details
    Sets the OdDbRasterImageDefReactor object associated with this raster image entity (DXF 360).
    \param reactorId [in]  Object ID of reactor for this image.
    
    \remarks
    Each OdDbRasterImage object must have an OdDbRasterImageDefReactor associated with the OdDbRasterImageDef
    object it is using. This call makes this OdDbRasterImage object the owner of the reactor object.
  */
  virtual void setReactorId(
    OdDbObjectId reactorId);

  /** \details
    Returns the Object ID of the OdDbRasterImageDefReactor object associated with this raster image entity (DXF 360).

    \remarks
    Each OdDbRasterImage object must have an OdDbRasterImageDefReactor associated with the OdDbRasterImageDef
    object it is using.
  */
  virtual OdDbObjectId reactorId() const;

  /** \details
    Sets the clip boundary for this raster image entity (DXF 71, 14, 24).

    \param clipPoints [in]  Array of 2D points in pixel coordinates.
  */
  virtual void setClipBoundary(
    const OdGePoint2dArray& clipPoints);

  /** \details
    Returns the clip boundary for this raster image entity (DXF 14, 24).
    \remarks
    The clip boundary is specified in pixel coordinates.
  */
  virtual const OdGePoint2dArray& clipBoundary() const;

  /** \details
    Returns the clipping state for this raster image entity (DXF 280).
  */
  bool isClipped() const;

  /** \details
    Adds the frame vertices of this raster image entity to the specified array.
    
    \param vertices [in/out] Receives the vertices of the image frame.

    \remarks
    If isClipped(), these are the vertices of the clip boundary. 
    
    If !isClipped(), these are the corners of the image.
  */
  virtual OdResult getVertices(
    OdGePoint3dArray& vertices) const;

  /** \details
    Sets the clip boundary to coincide with the image corners of this raster image entity. 
    
    \remarks
    Any existing boundary is deleted.
  */
  virtual void setClipBoundaryToWholeImage();

  /** \details
    Returns the clip boundary type of this this raster image entity (DXF 71).
  */
  ClipBoundaryType clipBoundaryType() const;

  /** \details
    Returns the pixel-to-model coordinate transformation matrix for this raster image entity.
  */
  virtual OdGeMatrix3d getPixelToModelTransform() const;

  /** \details
    Sets the brightness value for this raster image entity (DXF 281).
    \param brightness [in]  Image brightness [0.0 .. 100.0].
  */
  virtual OdResult setBrightness( 
    OdInt8 brightness );

  /** \details
    Returns the brightness value for this raster image entity (DXF 281).
    \returns
    Returns a value in the range [0.0 .. 100.0].
  */
  virtual OdInt8 brightness() const;

  /** \details
    Sets the contrast value for this raster image entity (DXF 282).
    \param contrast [in]  Image contrast. [0.0 .. 100.0]
  */
  virtual OdResult setContrast( 
    OdInt8 contrast );

  /** \details
    Returns the contrast value for this raster image entity (DXF 282).
    \returns
    Returns a value in the range [0.0 .. 100.0].  
  */
  virtual OdInt8 contrast() const;

  /** \details
    Sets the fade value for this raster image entity (DXF 283).

    \param fade [in]  Image fade. [0.0 .. 100.0].
  */
  virtual OdResult setFade( 
    OdInt8 fade );

  /** \details
    Returns the fade value for this raster image entity (DXF 283).
    \returns
    Returns a value in the range [0.0 .. 100.0].  
  */
  virtual OdInt8 fade() const;

  /** \details
    Sets the specified image display option for this raster image entity (DXF 70).

    \param optionIndex [in]  Option index.
    \param value [in]  Value for option.
    
    \remarks
    option is one of the following:
    
    <table>
    Name              Value   Description
    kShow             1       Show
    kShowUnAligned    2       Show unaligned
    kClip             4       Clip
    kTransparent      8       Transparent
    </table>
  */
  virtual void setDisplayOpt(
    ImageDisplayOpt optionIndex, 
    bool value);

  /** \details
    Returns the specified image display option for this raster image entity (DXF 70).

    \param optionIndex [in]  Option to return.

  */
  virtual bool isSetDisplayOpt(
    ImageDisplayOpt option) const;

  /** \details
    Returns the size in pixels of this raster image entity (DXF 13, 23).
      
    \param getCachedValue [in]  True to always return cached value. False to return the value from the OdDbRasterImageDef
                        object of available.  
  */
  virtual OdGeVector2d imageSize(
    bool getCachedValue = false) const;

  /** \details
    Returns the effective scale factor of this raster image entity.
    
    \remarks
    The effective scale factor is the image size in drawing units divided by the original image size.
    
    If the original image size is unavalable, or the user has not defined a drawing unit, the
    image width is assumed to be one drawing unit. 
  */
  virtual OdGeVector2d scale() const;

  /** \details
    Sets the orientation and origin of this raster image entity.

    \param origin [in]  Lower-left corner. 
    \param u [in]  Image width vector.
    \param v [in]  Image height vector.
    
    \returns
    Returns true if and only if successful.
    
    \remarks
    The actual height of the image is determined by the width and aspect ratio fo the original image. 
    
    These vectors define the outer edges of the raster image entity. 
  */
  virtual bool setOrientation(
    const OdGePoint3d& origin, 
    const OdGeVector3d& u,
    const OdGeVector3d& v);

  /** \details
    Returns the orientation and origin of this raster image entity.

    \param origin [out]  Receives the lower-left corner. 
    \param u [out]  Receives the vector defining the image direction and width. 
    \param v [out]  Receives the vector defining the direction of the height of the image.
    
    \remarks
    These vectors define the outer edges of the raster image entity.
  */
  virtual void getOrientation(
    OdGePoint3d& origin, 
    OdGeVector3d& u, 
    OdGeVector3d& v) const;

  /** \details
    Sets the ClipInverted flag value for this raster image entity.

    \param val [in]  The ClipInverted flag value.
  */
  virtual void setClipInverted(bool val);

  /** \details
    Returns the ClipInverted flag value for this raster image entity.
  */
  virtual bool isClipInverted() const;


  /** \details
      TBC.
  OdGiSentScanLines* getScanLines(const OdGiRequestScanLines& req) const;
  bool freeScanLines(OdGiSentScanLines* pSSL) const;
  */

  /** \details
       coment it for a while, because we do not know what it do
     static Oda::ClassVersion classVersion();
  */

  virtual OdResult dwgInFields(
    OdDbDwgFiler* pFiler) ODRX_OVERRIDE;

  virtual void dwgOutFields(
    OdDbDwgFiler* pFiler) const ODRX_OVERRIDE;

  virtual OdResult dxfInFields(
    OdDbDxfFiler* pFiler) ODRX_OVERRIDE;

  virtual void dxfOutFields(
    OdDbDxfFiler* pFiler) const ODRX_OVERRIDE;

  virtual void subClose() ODRX_OVERRIDE;

  /*
     virtual void subSwapIdWith(OdDbObjectId otherId, bool swapXdata = false, bool swapExtDict = false);
  */

  virtual bool subWorldDraw(
    OdGiWorldDraw* pWd) const ODRX_OVERRIDE;

  virtual void subViewportDraw(
    OdGiViewportDraw* pVd) const ODRX_OVERRIDE;

  virtual OdResult subGetGeomExtents(
    OdGeExtents3d& extents) const ODRX_OVERRIDE;

  virtual OdResult subGetClassID(
    void* pClsid) const ODRX_OVERRIDE;

  virtual OdResult subTransformBy(
    const OdGeMatrix3d& xfm) ODRX_OVERRIDE;

  virtual OdResult subExplode(
    OdRxObjectPtrArray& entitySet) const ODRX_OVERRIDE;

  virtual OdDbObjectPtr subDeepClone(
    OdDbIdMapping& ownerIdMap, OdDbObject*, bool bPrimary) const ODRX_OVERRIDE;

  /*
  virtual void getOsnapPoints( OdDb::OsnapMode osnapMode,
    OdGsMarker gsSelectionMark, const OdGePoint3d& pickPoint,
    const OdGePoint3d& lastPoint, const OdGeMatrix3d& viewXform,
    OdGePoint3dArray& snapPoints, OdDbIntArray& geomIds) const;

  virtual void getGripPoints(OdGePoint3dArray& gripPoints,
    OdDbIntArray& osnapModes, OdDbIntArray& geomIds) const;

  virtual void moveGripPointsAt(const OdDbIntArray& indices, const OdGeVector3d& offset);

  virtual void getStretchPoints(OdGePoint3dArray& stretchPoints) const;

  virtual void moveStretchPointsAt(const OdDbIntArray& indices, const OdGeVector3d& offset);

  virtual void transformBy(const OdGeMatrix3d& xform);
  virtual void getTransformedCopy(const OdGeMatrix3d& xform, OdDbEntity** ent) const;
  virtual OdResult explode(OdRxObjectPtrArray& entitySet) const; //Replace OdRxObjectPtrArray
  virtual bool subGetGeomExtents(OdGeExtents3d& extents) const;

  virtual void getSubentPathsAtGsMarker(OdDb::SubentType type,
    OdGsMarker gsMark, const OdGePoint3d& pickPoint,
    const OdGeMatrix3d& viewXform, int& numPaths,
    OdDbFullSubentPath*& subentPaths, int numInserts = 0,
    OdDbObjectId* entAndInsertStack = 0) const;

  virtual void getGsMarkersAtSubentPath(
    const OdDbFullSubentPath& subPath,
    OdGsMarkerArray& gsMarkers) const;

  virtual OdDbEntityPtr subentPtr(const OdDbFullSubentPath& id) const;
  virtual void saveAs(OdGiWorldDraw* mode, OdDb::EntSaveAsType st);
  virtual void intersectWith(const OdDbEntity* ent,
    OdDb::Intersect intType, OdGePoint3dArray& points,
    OdGsMarker thisGsMarker = 0, OdGsMarker otherGsMarker = 0) const;

  virtual void intersectWith(const OdDbEntity* ent,
    OdDb::Intersect intType, const OdGePlane& projPlane,
    OdGePoint3dArray& points, OdGsMarker thisGsMarker = 0,
    OdGsMarker otherGsMarker = 0) const;
  */
};

/** \details
  Translates the specified point from pixel coordinates to model coordinates.

  \param xf, [in] Pixel->Model transformation matrix.
  \param pixelPoint [in] Pixel point.
  \param modelPoint [out] Receives the Model point.
*/
inline void pixelToModel(
  const OdGeMatrix3d& xfm,
  const OdGePoint2d& pixelPoint,
  OdGePoint3d& modelPoint)
{
  modelPoint.set(pixelPoint.x, pixelPoint.y, 0);
  modelPoint.transformBy(xfm);
}

/** \details
  Translates the specified point from model coordinates to pixel coordinates.
  \param xfm [in]  Model->Pixel transformation matrix.
  \param pixelPoint [out]  Receives the pixel point.
  \param modelPoint [in]  Model point.
  \param plane [in]  Plane on which modelPoint is to be projected prior to transformation.
  \param viewDir [in]  Vector defining the projection direction.
*/
inline void modelToPixel(
  const OdGeMatrix3d& xfm,
  const OdGePoint3d& modelPoint,
  OdGePoint2d& pixelPoint)
{
  OdGePoint3d modelPt = modelPoint;
  modelPt.transformBy(xfm);
  pixelPoint.set(modelPt.x, modelPt.y);
}

inline void modelToPixel(
  const OdGeVector3d& viewDir,
  const OdGeMatrix3d& xfm,
  const OdGePlane& plane,
  const OdGePoint3d& modelPoint,
  OdGePoint2d& pixelPoint)
{
  OdGePoint3d ptOnPlane = modelPoint.project(plane, viewDir);
  ptOnPlane.transformBy(xfm);
  pixelPoint.set(ptOnPlane.x, ptOnPlane.y);
}

inline OdGeMatrix3d pixelToModelTransform(const OdGePoint3d& origin, 
                                          const OdGeVector3d& u, 
                                          const OdGeVector3d& v, 
                                          double ySize)
{
  OdGeVector3d vOffset(0.5 * u + (ySize - 0.5) * v);
  OdGeMatrix3d x;
  x.setCoordSystem( origin + vOffset, u, -v, u.crossProduct(v));
  return x;
}

/** \details
  This template class is a specialization of the OdSmartPtr class for OdDbRasterImage object pointers.
*/
typedef OdSmartPtr<OdDbRasterImage> OdDbRasterImagePtr;

#include "TD_PackPop.h"

#endif // __IMGENT_H

