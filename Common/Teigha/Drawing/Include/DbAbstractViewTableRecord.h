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




#ifndef _ODDBABSTRACTVIEWLTABLERECORD_INCLUDED
#define _ODDBABSTRACTVIEWLTABLERECORD_INCLUDED

#include "TD_PackPush.h"

#include "DbSymbolTableRecord.h"
#include "DbAbstractViewportData.h"
#include "ViewportDefs.h"
#include "Gi/GiViewportTraits.h"

class OdDbSun;

/** \details
    This class is the base class for OdDbViewTableRecord and OdDbViewportTableRecord.

    \sa
    TD_Db
    
    <group OdDb_Classes> 
*/
class TOOLKIT_EXPORT OdDbAbstractViewTableRecord : public OdDbSymbolTableRecord
{
public:

  ODDB_DECLARE_MEMBERS(OdDbAbstractViewTableRecord);

  OdDbAbstractViewTableRecord();

  /** \details
    Returns the DSC center point of this View. 
    \remarks
    *  DXF 10 for OdDbViewTableRecord.
    *  DXF 12 for OdDbViewportTableRecord.
  */
  OdGePoint2d centerPoint() const;

  /** \details
    Sets the DCS center point of this View.
    \param centerPoint [in]  Center point.
    \remarks
    *  DXF 10 for OdDbViewTableRecord.
    *  DXF 12 for OdDbViewportTableRecord.
  */
  void setCenterPoint(
    const OdGePoint2d& centerPoint);

  /** \details
    Returns the DCS height of this View (DXF 40).
  */
  double height() const;

  /** \details
    Sets the DCS height of this View (DXF 40).
    \param height [in]  Height.
  */
  void setHeight(double height);

  /** \details
    Returns the DCS width of this View (DXF 41).
    
    \remarks
    DXF 41 contains the width : height ratio for OdDbViewportTableRecord.
  */
  double width() const;

  /** \details
    Sets the DCS width of this View (DXF 41).

    \param width [in]  Width. 

    \remarks
    DXF 41 contains the width : height ratio for OdDbViewportTableRecord.
  */
  void setWidth(
    double width);

   /** \details
    Returns the WCS view target of this View.
    
    \remarks
    *  DXF 12 for OdDbViewTableRecord. 
    *  DXF 17 for OdDbViewportTableRecord.    
   */
  OdGePoint3d target() const;

  /** \details
    Sets the WCS view target of this View.
    \param target [in]  Target.
    \remarks
    *  DXF 12 for OdDbViewTableRecord.
    *  DXF 17 for OdDbViewportTableRecord.    
  */
  void setTarget(
    const OdGePoint3d& target);

  /** \details
    Returns the WCS view direction of this View.
    \param viewDirection [in] View direction.

    \remarks
    *  DXF 11 for OdDbViewTableRecord. 
    *  DXF 16 for OdDbViewportTableRecord.    
  */
  OdGeVector3d viewDirection() const;

  /** \details
    Sets the WCS view direction of this View.
    \param viewDirection [in]  View direction.

    \remarks
    *  DXF 12 for OdDbViewTableRecord. 
    *  DXF 17 for OdDbViewportTableRecord.    
  */
 void setViewDirection(
  const OdGeVector3d& viewDirection);

  /** \details
    Returns the DCS twist angle of this View.

    \remarks
    *  DXF 50 for OdDbViewTableRecord 
    *  DXF 51 for OdDbViewportTableRecord.    
    
    \remarks
    All angles are expressed in radians.
  */
  double viewTwist() const;

  /** \details
    Sets the DCS twist angle of this View.

    \remarks
    *  DXF 50 for OdDbViewTableRecord 
    *  DXF 51 for OdDbViewportTableRecord.    
    
    \param viewTwist [in]  View twist angle.
    \remarks
    All angles are expressed in radians.
  */
  void setViewTwist(
    double viewTwist);

  /** \details
    Returns the perspective mode lens length (in mm) of this View (DXF 42).
  */
  double lensLength() const;

  /** \details
    Sets the perspective mode lens length of this View (DXF 42).
    \param lensLength [in]  Lens length (mm).
  */
  void setLensLength(
    double lensLength);

  /** \details
      Returns the front clip distance of this View (DXF 43).
  */
  double frontClipDistance() const;

  /** \details
    Sets the front clip distance of this View (DXF 43).
    \param frontClipDistance [in]  Front clip distance.
  */
  void setFrontClipDistance(
    double frontClipDistance);

  /** \details
    Returns the back clip distance of this View (DXF 44).
  */
  double backClipDistance() const;

  /** \details
    Sets the back distance of this View (DXF 44).
    \param backClipDistance [in]  Back clip distance.
  */
  void setBackClipDistance(
    double backClipDistance);

  /** \details
     Returns true if and only if perspective is on for this View (DXF 70, bit 0x01).
  */
  bool perspectiveEnabled() const;

  /** \details
    Controls perspective mode for this view (DXF 71, bit 0x01).

    \param perspectiveEnabled [in]  Perspective Enabled.
  */
  void setPerspectiveEnabled(
    bool perspectiveEnabled);

  /** \details
    Returns true if and only if front clipping is enabled for this View (DXF 71, bit 0x02).
  */
  bool frontClipEnabled() const;

  /** \details
    Controls front clipping for this view (DXF 71, bit 0x02).

    \param frontClipEnabled [in]  Front clipping enabled.
  */
  void setFrontClipEnabled(
    bool frontClipEnabled);

  /** \details
    Returns true if and only if back clipping is enabled for this View (DXF 71, bit 0x04).
  */
  bool backClipEnabled() const;

  /** \details
    Controls back clipping for this view (DXF 71, bit 0x04).

    \param backClipEnabled [in]  Back clipping enabled.     
  */
  void setBackClipEnabled(
    bool backClipEnabled);

  /** \details
    Returns true if and only if the front clipping plane plane passes through the camera (DXF 71, bit 0x10).
    \remarks
    If true, the front clipping plane ignores the front clip distance.
  */
  bool frontClipAtEye() const;

  /** \details
    Controls the front clipping plane passing through the camera (DXF 71, bit 0x10).

    \param atEye [in]  True to pass through camera, false to use front clip distance.
  */
  void setFrontClipAtEye(
    bool atEye);

  /** \details
    Sets the render mode of this View (DXF 281).
    \param renderMode [in]  Render mode.

    \remarks
    renderMode must be one of the following:
    
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
  void setRenderMode(OdDb::RenderMode renderMode);

  /** \details
    Returns the render mode of this View (DXF 281).
    
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
  OdDb::RenderMode renderMode() const;

  /** \details
    Returns the origin, X-axis, and Y-Axis of the UCS associated with this Viewport object.

    \param origin [out]  Receives the UCS origin (DXF 110).
    \param xAxis [out]  Receives the UCS X-axis (DXF 111).
    \param yAxis [out]  Receives the UCS Y-axis (DXF 112).
  */
  void getUcs(OdGePoint3d& origin, 
    OdGeVector3d& xAxis, 
    OdGeVector3d& yAxis) const;

  /** \details
    Returns true if and only if the UCS associated with this View is
    orthographic with respect to UCSBASE (DXF 79).

    \param orthoUcs [out]  Receives the orthographic view type.
    \param pDb [in]  Working database. 
    \remarks
    orthoUcs must be one of the following:
    
    <table>
    Name                 Value   View type
    OdDb::kNonOrthoView   0       Non-orthographic with respect to the UCS 
    OdDb::kTopView        1       Top view with respect to the UCS 
    OdDb::kBottomView     2       Bottom view with respect to the UCS 
    OdDb::kFrontView      3       Front view with respect to the UCS 
    OdDb::kBackView       4       Back view with respect to the UCS 
    OdDb::kLeftView       5       Left view with respect to the UCS 
    OdDb::kRightView      6       Right view with respect to the UCS
    \remarks
    If if this View object is database resident, the pDb parameter is ignored.
    </table>
 
  */
  bool isUcsOrthographic(
    OdDb::OrthographicView& orthoUcs,
    const OdDbDatabase* pDb = 0) const;

  /** \details
    Returns the Object ID of the UCS associated with this View (DXF 345).
  */
  OdDbObjectId ucsName() const;

  /** \details
    Returns the elevation of the UCS plane of this View (DXF 146).
    
    \remarks
    The elevation is the distance from the WCS origin to the UCS plane of this View.
  */
  double elevation() const;

  /** \details
    Sets the UCS associated with this Viewport object.

    \param origin [in]  The UCS origin (DXF 110).
    \param xAxis [in]  The UCS X-axis (DXF 111).
    \param yAxis [in]  The UCS Y-axis (DXF 112).
  */
  void setUcs(
    const OdGePoint3d& origin, 
    const OdGeVector3d& xAxis, 
    const OdGeVector3d& yAxis);

  /** \details
    Sets the UCS associated with this Viewport object.

    \param orthoUcs [in]  Orthographic view type (DXF 79).
    \param pDb [in]  Working database. If object is a database resident this parameter is ignored.

    \remarks
    orthoUcs must be one of the following:
    
    <table>
    Name                 Value   View type
    OdDb::kNonOrthoView   0       Non-orthographic with respect to the UCS 
    OdDb::kTopView        1       Top view with respect to the UCS 
    OdDb::kBottomView     2       Bottom view with respect to the UCS 
    OdDb::kFrontView      3       Front view with respect to the UCS 
    OdDb::kBackView       4       Back view with respect to the UCS 
    OdDb::kLeftView       5       Left view with respect to the UCS 
    OdDb::kRightView      6       Right view with respect to the UCS 
    </table>
  */
  void setUcs(
    OdDb::OrthographicView orthoUcs,
    const OdDbDatabase* pDb = 0);
  
  /** \details
    Sets the UCS associated with this Viewport object.

    \param ucsId [in]  Object ID of UCS (DXF 345 or 346).
  */
  void setUcs(
    const OdDbObjectId& ucsId);

  /** \details
    Sets the UCS associated with this View to the WCS. 
  */
  void setUcsToWorld();

  /** \details
    Sets the elevation of the UCS plane of this View (DXF 146).
    \param elevation [in]  Elevation.
    \remarks
    The elevation is the distance from the WCS origin to the plane of this View.
  */
  void setElevation(
    double elevation);
  
  // New-2007 API

  OdDbObjectId background() const;
  void setBackground(const OdDbObjectId& background);

  OdDbObjectId visualStyle() const;
  void setVisualStyle(const OdDbObjectId& visualStyle);

  bool isDefaultLightingOn() const;
  void setDefaultLightingOn(bool isOn);

  OdGiViewportTraits::DefaultLightingType defaultLightingType() const;
  void setDefaultLightingType(OdGiViewportTraits::DefaultLightingType lightingType);

  double brightness() const;
  void setBrightness(double brightness);

  double contrast() const;
  void setContrast(double contrast);

  OdCmColor ambientLightColor() const;
  void setAmbientLightColor(const OdCmColor& color);

  OdDbObjectId sunId() const;
  OdDbObjectId setSun(OdDbSun* pSun);

  void toneOperatorParameters(OdGiToneOperatorParameters &params) const;
  void setToneOperatorParameters(const OdGiToneOperatorParameters &params);

  virtual OdResult dwgInFields(
    OdDbDwgFiler* pFiler);

  virtual void dwgOutFields(
    OdDbDwgFiler* pFiler) const;

  virtual void copyFrom (
    const OdRxObject* pSource);

  /*
      bool isViewOrthographic(OdDb::OrthographicView& view) const;
      void setViewDirection(OdDb::OrthographicView view);
  */

  virtual OdGiDrawable* drawable();

  virtual OdUInt32 subSetAttributes(
    OdGiDrawableTraits* pTraits) const;
};

/** \details
    This template class is a specialization of the OdSmartPtr class for OdDbViewportTableRecord object pointers.
*/
typedef OdSmartPtr<OdDbAbstractViewTableRecord> OdDbAbstractViewTableRecordPtr;
#include "TD_PackPop.h"

#endif // _ODDBABSTRACTVIEWLTABLERECORD_INCLUDED

