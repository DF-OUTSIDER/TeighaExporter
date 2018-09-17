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

#ifndef OD_ABSTRACTVIEWPE_H
#define OD_ABSTRACTVIEWPE_H

#include "DbRootExport.h"
#include "Ge/GePoint3d.h"
#include "Ge/GeVector3d.h"
#include "ViewportDefs.h"
#include "DbStubPtrArray.h"
#include "Gi/GiViewportTraits.h"

class OdGeBoundBlock3d;

typedef OdRxObject OdDbBaseDatabase;

#include "TD_PackPush.h"

/** \details
    This class defines the interface for Protocol Extension classes for OdGsView, OdDbViewTableRecord, OdDbViewport and OdDbViewportTableRecord objects.
    
    \note
    Only the following objects are supported:
    
    *  OdGsView
    *  OdDbViewTableRecord
    *  OdDbViewport
    *  OdDbViewportTableRecord
    *  Derivatives of one of the above.

    Corresponding C++ library: TD_Db
    
    <group Other_Classes> 
*/
class DBROOT_EXPORT OdAbstractViewPE : public OdRxObject
{
public:
  ODRX_DECLARE_MEMBERS(OdAbstractViewPE);

  // Viewport methods:

  /** \details
    Returns the lower-left corner of the specified Viewport object.
    \param pViewport [in]  Pointer to the Viewport object.
    \remarks
    *  point == (0.0, 0.0) corresponds to the lower-left corner of the graphics area.
    *  point == (1.0, 1.0) corresponds to the upper-right corner of the graphics area.
    \sa
    Coordinate Systems
  */
  virtual OdGePoint2d lowerLeftCorner(const OdRxObject* pViewport) const;
  /** \details
    Returns the upper-right corner of the specified Viewport object.
    \param pViewport [in]  Pointer to the Viewport object.
    \remarks
    *  point == (0.0, 0.0) corresponds to the lower-left corner of the graphics area.
    *  point == (1.0, 1.0) corresponds to the upper-right corner of the graphics area.
    \sa
    Coordinate Systems
  */
  virtual OdGePoint2d upperRightCorner(const OdRxObject* pViewport) const;
  /** \details
    Sets the corners of the specified Viewport object.
    \param pViewport [in]  Pointer to the Viewport object.
    \param lowerLeft [in]  Lower-left corner.
    \param upperRight [in]  Upper-right corner.
    \remarks
    *  point == (0.0, 0.0) corresponds to the lower-left corner of the graphics area.
    *  point == (1.0, 1.0) corresponds to the upper-right corner of the graphics area.
    \sa
    Coordinate Systems
  */
  virtual void setViewport(OdRxObject* pViewport, const OdGePoint2d& lowerLeft, const OdGePoint2d& upperRight) const;
  /** \details
    Returns true if and only if there is a viewport data associated with the specified Viewport object.
    \param pViewport [in]  Pointer to the Viewport object.
  */
  virtual bool hasViewport(const OdRxObject* pViewport) const;

  // View methods:

   /** \details
    Returns the WCS view target of the specified Viewport object.
    \param pViewport [in]  Pointer to the Viewport object.
   */
  virtual OdGePoint3d target(const OdRxObject* pViewport) const = 0;
  
  /** \details
    Returns the WCS view direction of the specified Viewport object.
    \param pViewport [in]  Pointer to the Viewport object.
  */
  virtual OdGeVector3d direction(const OdRxObject* pViewport) const = 0;

  /** \details
    Returns the WCS Up vector of the specified Viewport object.
    \param pViewport [in]  Pointer to the Viewport object.
  */
  virtual OdGeVector3d upVector (const OdRxObject* pViewport) const = 0;
  /** \details
    Returns the DCS field width of the specified Viewport object.
    \param pViewport [in]  Pointer to the Viewport object.
    \sa
    Coordinate Systems
  */
  virtual double fieldWidth(const OdRxObject* pViewport) const = 0;
  /** \details
    Returns the DCS field height of the specified Viewport object.
    \param pViewport [in]  Pointer to the Viewport object.
    \sa
    Coordinate Systems
  */
  virtual double fieldHeight(const OdRxObject* pViewport) const = 0;

  /** \details
    Returns true if and only if the projection type for the specified Viewport object is perspective. 
    \param pViewport [in]  Pointer to the Viewport object.
  */
  virtual bool isPerspective(const OdRxObject* pViewport) const = 0;
  
  /** \details
    Returns the view offset vector for the specified Viewport object.
    \param pViewport [in]  Pointer to the Viewport object.
  */
  virtual OdGeVector2d viewOffset(const OdRxObject* pViewport) const;
  /** \details
    Returns true if and only if there is a view offset associated with the specified Viewport object.
    \param pViewport [in]  Pointer to the Viewport object.
  */
  virtual bool hasViewOffset(const OdRxObject* pViewport) const;

  /** \details
    Returns the view DCS twist angle for the specified Viewport object.
    \param pViewport [in]  Pointer to the Viewport object.
  */
  virtual double viewTwist(const OdRxObject* pViewport) const = 0;

  /** \details
    Sets the parameters for the specified Viewport object according to the arguments.
    \param pViewport [in]  Pointer to the Viewport object.
    \param target [in]  WCS view target.
    \param direction [in]  WCS view direction.
    \param upVector [in]  WCS Up vector.
    \param fieldWidth [in]  DCS field width.
    \param fieldHeight [in]  DCS field height.
    \param isPerspective [in]  Controls perspective.
    \param viewOffset [in]  View offset vector.

    \sa
    Coordinate Systems
  */
  virtual void setView(
      OdRxObject* pViewport,
      const OdGePoint3d& target,
      const OdGeVector3d& direction,
      const OdGeVector3d& upVector,
      double fieldWidth,
      double fieldHeight,
      bool isPerspective,
      const OdGeVector2d& viewOffset = OdGeVector2d::kIdentity) const = 0;

  /** \details
    Sets the perspective lens length for the specified Viewport object.
    \param pViewport [in]  Pointer to the Viewport object.
    \param lensLength [in]  Perspective lens length.
  */
  virtual void setLensLength(OdRxObject* pViewport, double lensLength) const = 0;
  /** \details
    Returns the perspective lens length for the specified Viewport object.
    \param pViewport [in]  Pointer to the Viewport object.
  */
  virtual double lensLength(const OdRxObject* pViewport) const = 0;

  /** \details
    Returns true if and only if front clipping is on for the specified Viewport object.
    \param pViewport [in]  Pointer to the Viewport object.
  */
  virtual bool isFrontClipOn(const OdRxObject* pViewport) const = 0;

  /** \details
    Controls front clipping for the specified Viewport object.
    \param pViewport [in]  Pointer to the Viewport object.
    \param frontClip [in]  Controls front clip.
  */
  virtual void setFrontClipOn(OdRxObject* pViewport, bool frontClip) const = 0;

  /** \details
    Returns true if and only if back clipping is on for the specified Viewport object.
    \param pViewport [in]  Pointer to the Viewport object.
  */
  virtual bool isBackClipOn(const OdRxObject* pViewport) const = 0;
  
  /** \details
    Controls back clipping for the specified Viewport object.
    \param pViewport [in]  Pointer to the Viewport object.
    \param backClip [in]  Controls back clip.
  */
  virtual void setBackClipOn(OdRxObject* pViewport, bool backClip) const = 0;

  /** \details
    Returns true if and only if the front clipping plane passes through the camera for the specified Viewport object.
    \remarks
    If true, the front clipping plane ignores the front clip distance.
    \param pViewport [in]  Pointer to the Viewport object.
  */
  virtual bool isFrontClipAtEyeOn(const OdRxObject* pViewport) const = 0;
  
  /** \details
    Controls the front clipping plane's passing through the camera for the specified Viewport object.
    \remarks
    If and only if frontClipAtEye is true, the front clipping plane ignores the front clip distance.

    \param pViewport [in]  Pointer to the Viewport object.
    \param frontClipAtEye [in]  Controls FrontClipAtEye.
  */
  virtual void setFrontClipAtEyeOn(OdRxObject* pViewport, bool frontClipAtEye) const = 0;

  /** \details
    Returns the front clip distance for the specified Viewport object.
    \param pViewport [in]  Pointer to the Viewport object.
  */
  virtual double frontClipDistance(const OdRxObject* pViewport) const = 0;
  
  /** \details
    Sets the front clip distance for the specified Viewport object.
    \param pViewport [in]  Pointer to the Viewport object.
    \param frontClipDistance [in]  Front clip distance.
  */
  virtual void setFrontClipDistance(OdRxObject* pViewport, double frontClipDistance) const = 0;

  /** \details
    Returns the back clip distance for the specified Viewport object.
    \param pViewport [in]  Pointer to the Viewport object.
  */
  virtual double backClipDistance(const OdRxObject* pViewport) const = 0;
  /** \details
    Sets the front clip distance for the specified Viewport object.
    \param pViewport [in]  Pointer to the Viewport object.
    \param backClipDistance [in]  Back clip distance.
  */
  virtual void setBackClipDistance(OdRxObject* pViewport, double backClipDistance) const = 0;

  /** \details
    Sets the render mode for the specified Viewport object.
    \param renderMode [in]  Render mode.
    \param pViewport [in]  Pointer to the Viewport object.
    
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
  virtual void setRenderMode(OdRxObject* pViewport, OdDb::RenderMode renderMode) const = 0;

  /** \details
    Returns the render mode for the specified Viewport object.
    \param pViewport [in]  Pointer to the Viewport object.

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
  virtual OdDb::RenderMode renderMode(const OdRxObject* pViewport) const = 0;

  /** \details
    Sets visual style for the specified Viewport object.
    \param pViewport [in]  Pointer to the Viewport object.
    \param visualStyleId [in]  Visual style Object ID.
  */
  virtual void setVisualStyle(OdRxObject* pViewport, OdDbStub* visualStyleId) const = 0;
  /** \details
    Returns visual style for the specified Viewport object.
    \param pViewport [in]  Pointer to the Viewport object.
  */
  virtual OdDbStub* visualStyle(const OdRxObject* pViewport) const = 0;

  /** \details
    Sets background for the specified Viewport object.
    \param pViewport [in]  Pointer to the Viewport object.
    \param backgroundId [in]  Background Object ID.
  */
  virtual void setBackground(OdRxObject* pViewport, OdDbStub* backgroundId) const = 0;
  /** \details
    Returns background for the specified Viewport object.
    \param pViewport [in]  Pointer to the Viewport object.
  */
  virtual OdDbStub* background(const OdRxObject* pViewport) const = 0;

  /** \details
    Returns default lighting state.
    \param pViewport [in]  Pointer to the Viewport object.
  */
  virtual bool isDefaultLightingOn(const OdRxObject* pViewport) const;
  /** \details
    Sets default lighting state.
    \param pViewport [in]  Pointer to the Viewport object.
    \param isOn [in]  New default lighting state.
  */
  virtual void setDefaultLightingOn(OdRxObject* pViewport, bool isOn) const;

  /** \details
    Returns default lighting type.
    \param pViewport [in]  Pointer to the Viewport object.
  */
  virtual OdGiViewportTraits::DefaultLightingType defaultLightingType(const OdRxObject* pViewport) const;
  /** \details
    Sets default lighting type.
    \param pViewport [in]  Pointer to the Viewport object.
    \param lightingType [in]  New default lighting type.
  */
  virtual void setDefaultLightingType(OdRxObject* pViewport, OdGiViewportTraits::DefaultLightingType lightingType) const;

  /** \details
    Returns the frozen layers for the specified Viewport object.
    
    \param pViewport [in]  Pointer to the Viewport object.
    \param frozenLayers [out]  Receives an array of frozen layer Object IDs.
  */
  virtual void frozenLayers(const OdRxObject* pViewport, OdDbStubPtrArray& frozenLayers) const;

  /** \details
    Sets the frozen layers for the specified Viewport object.
    
    \param pViewport [in]  Pointer to the Viewport object.
    \param frozenLayers [in]  Array of frozen layer Object IDs.
  */
  virtual void setFrozenLayers(OdRxObject* pViewport, const OdDbStubPtrArray& frozenLayers) const;

  /** \details
    Sets the parameters for the specified Viewport object according to the arguments.

    \param pDestinationView [in]  Pointer to the destination Viewport object
	\param pSourceView [in]  Pointer to the source Viewport object.
    
	\sa
    Coordinate Systems
  */
  virtual void setView(OdRxObject* pDestinationView, const OdRxObject* pSourceView) const;

  // UCS methods:

  /** \details
    Returns true if and only if there is a UCS associated with the specified Viewport object.
    
    \param pViewport [in]  Pointer to the Viewport object.
  */
  virtual bool hasUcs(const OdRxObject* pViewport) const;

  /** \details
    Returns orthographic view type of the UCS associated with the specified Viewport object.

    \param pViewport [in]  Pointer to the Viewport object.
    \param pDb [in]  Working database. If object is a database resident this parameter is ignored.

    \remarks
    orthoUcs() returns one of the following:
    
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
  virtual OdDb::OrthographicView orthoUcs(const OdRxObject* pViewport, const OdDbBaseDatabase* pDb = 0) const;

  /** \details
    Sets the UCS associated with the specified Viewport object.
    \param pViewport [in]  Pointer to the Viewport object.
    \param orthoUcs [in] Orthographic view type.
    \param pDb [in] Working database. If object is a database resident this parameter is ignored.

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
  virtual bool setUcs(OdRxObject* pViewport, OdDb::OrthographicView orthoUcs, const OdDbBaseDatabase* pDb = 0) const;
  
  /** \details
    Returns the Object ID of the UCS name associated with the specified Viewport object. 
    \param pViewport [in]  Pointer to the Viewport object.
  */
  virtual OdDbStub* ucsName(const OdRxObject* pViewport) const;

  /** 
  */
  /** \details
    Sets the UCS associated with the specified Viewport object.
	
    \param pViewport [in]  Pointer to the Viewport object.
    \param ucsId [in]  Object ID of UCS.
  */
  virtual bool setUcs(OdRxObject* pViewport, OdDbStub* ucsId) const;

  /** \details
    Returns the UCS associated with the specified Viewport object. 
    \param origin [out]  Receives the UCS origin.
    \param xAxis [out]  Receives the UCS X-axis.
    \param yAxis [out]  Receives the UCS Y-axis.
  */
  virtual void getUcs(const OdRxObject* pViewport, OdGePoint3d& origin, OdGeVector3d& xAxis, OdGeVector3d& yAxis) const;

  /** \details
    Sets the UCS associated with the specified Viewport object.
	
    \param pViewport [in]  Pointer to the Viewport object.
    \param origin [in]  The UCS origin.
    \param xAxis [in]  The UCS X-axis.
    \param yAxis [in]  The UCS Y-axis.
  */
  virtual void setUcs(OdRxObject* pViewport, const OdGePoint3d& origin, const OdGeVector3d& xAxis, const OdGeVector3d& yAxis) const;

  /** \details
    Returns the elevation for the specified Viewport object.
    \param pViewport [in]  Pointer to the Viewport object.
  */
  virtual double elevation(const OdRxObject* pViewport) const;

  /** \details
    Sets the elevation for the specified Viewport object.
    \param pViewport [in]  Pointer to the Viewport object.
    \param elevation [in]  Elevation.
  */
  virtual void setElevation(OdRxObject* pViewport, double elevation) const;
  /** 
  */
  /** \details
    Sets the UCS associated with the specified Viewport object.
	
	\param pDestinationView [in]  Pointer to the destination Viewport object
    \param pSourceView [in]  Pointer to the source Viewport object.
  */
  virtual void setUcs(OdRxObject* pDestinationView, const OdRxObject* pSourceView) const;

  // Util methods:

  /** \details
    Returns the eye coordinate system extents of the specified Viewport object.
    
    \param pViewport [in]  Pointer to the Viewport object.
    \param extents [out]  Receives the extents.

    \returns
    Returns true if and only if the extents are defined.

    \note
    The components of extents are clipped to the range [-10e100..10e100].
    \sa
    Coordinate Systems
  */
  virtual bool viewExtents(
    const OdRxObject* pViewport,
    OdGeBoundBlock3d& extents) const = 0;

  /** \details
    Returns the eye coordinate system plotting extents of the specified Viewport object.
    
    \param pViewport [in]  Pointer to the Viewport object.
    \param extents [out]  Receives the extents.
    \param bExtendOnly [in]  Only extend exist extents by overall viewport extents.
    \param bExtentsValid [in]  Input extents is valid for extending using overall viewport extents.
    \param pWorldToEye [in]  Optional WCS to ECS transformation for plot extents extending.

    \returns
    Returns true if and only if the extents are defined.

    \note
    The components of extents are clipped to the range [-10e100..10e100].
    \sa
    Coordinate Systems
  */
  virtual bool plotExtents(
    const OdRxObject* pViewport,
    OdGeBoundBlock3d& extents,
    bool bExtendOnly = false,
    bool bExtentsValid = true,
    const OdGeMatrix3d *pWorldToEye = NULL) const;

  /** \details
    Modifies the specified Viewport object to fit the specified eye coordinate system extents.
    
    \param pViewport [in]  Pointer to the Viewport object.
    \param pExtents [in]  Pointer to the extents.
    \param extCoef [in]  Optional extents multiplier.

    \returns
    Returns true if and only if the extents are defined.

    \remarks
    If pExtents is null, this function calls viewExtents() to determine the extents.
  */
  virtual bool zoomExtents(
    OdRxObject* pViewport,
    const OdGeBoundBlock3d* pExtents = 0,
    double extCoef = 1.02) const;

  /** \details
    Returns the transformation matrix from WCS to EyeSpace for the specified Viewport object.
    \param pViewport [in]  Pointer to the Viewport object.

    \sa
    Coordinate Systems
  */
  virtual OdGeMatrix3d worldToEye(const OdRxObject* pViewport) const;
  /** \details
    Returns the transformation matrix from EyeSpace to WCS for the specified Viewport object.
    \param pViewport [in]  Pointer to the Viewport object.

    \sa
    Coordinate Systems
  */
  virtual OdGeMatrix3d eyeToWorld(const OdRxObject* pViewport) const;

  /** \details
    Returns true if and only if viewport is used during plotting.
    \param pViewport [in]  Pointer to the Viewport object.
  */
  virtual bool isPlotting(const OdRxObject* pViewport) const;
  /** \details
    Returns the plot settings object pointer associated with this Viewport object.
    \param pViewport [in]  Pointer to the Viewport object.
    \param bOpenForWrite [in]  Mode in which to open the object.
    \remarks
    Return OdDbPlotSettings object for OdDbViewportTableRecord Viewport object and
    OdDbViewport object for OdDbViewport object or OdDbPlotSettings object for
    overall OdDbViewport. Return Null object pointer for all other cases.
    \sa
    OdDbAbstractPlotData
  */
  virtual OdRxObjectPtr plotDataObject(const OdRxObject* pViewport, bool bOpenForWrite = false) const;
  /** \details
    Applies plot settings to view.
    \param pSourceView [in]  Pointer to the source Viewport object.
    \param pDestinationView [in]  Pointer to the destination Viewport object
    \returns
    Returns true if plot settings was applied successfully.
  */
  virtual bool applyPlotSettings(OdRxObject* pDestinationView, const OdRxObject* pSourceView) const;

  /** \details
    Returns annotation scale object ID associated with this Viewport object.
    \param pViewport [in]  Pointer to the Viewport object.
  */
  virtual OdDbStub* annotationScale(const OdRxObject* pViewport) const;

  /** \details
    Returns compatible visual style or background object for specified viewport.
    \param pViewport [in]  Pointer to the Viewport object.
    \param pCopyObject [in]  Input object ID.
    \returns Returns comptaible objectID or null in case if there is no any compatible objects found.
  */
  virtual OdDbStub* compatibleCopyObject(const OdRxObject* pViewport, OdDbStub* pCopyObject) const;
};

/** \details
  This template class is a specialization of the OdSmartPtr class for OdAbstractViewPE object pointers.
*/
typedef OdSmartPtr<OdAbstractViewPE> OdAbstractViewPEPtr;

#include "TD_PackPop.h"

#endif // OD_ABSTRACTVIEWPE_H
