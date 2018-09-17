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

#ifndef __GI_VIEWPORT__
#define __GI_VIEWPORT__

#include "RxObject.h"

class OdGePoint2d;
class OdGePoint3d;
class OdGeVector3d;
class OdDbStub;
class OdGeMatrix3d;
class OdGiContextualColors;

#include "TD_PackPush.h"

/** \details
    This class represents the view characteristics of current Viewport within the Teigha vectorization framework.  

    \remarks
    Client code can query this information to generate the correct 
    viewport -dependent geometric representation for an object.

    \sa
    TD_Gi:

    <group OdGi_Classes> 
*/
class FIRSTDLL_EXPORT OdGiViewport : public OdRxObject
{ 
public:
  ODRX_DECLARE_MEMBERS(OdGiViewport);

  /** \details
    Returns the transformation matrix from ModelSpace to EyeSpace for this Viewport object.

    \sa
    Coordinate Systems
  */
  virtual OdGeMatrix3d getModelToEyeTransform() const = 0;

  /** \details
    Returns the transformation matrix from EyeSpace to ModelSpace for this Viewport object.

    \sa
    Coordinate Systems
  */
  virtual OdGeMatrix3d getEyeToModelTransform() const = 0;

  /** \details
    Returns the transformation matrix from WCS to EyeSpace for this Viewport object.

    \sa
    Coordinate Systems
  */
  virtual OdGeMatrix3d getWorldToEyeTransform() const = 0;

  /** \details
    Returns the transformation matrix from EyeSpace to WCS for this Viewport object.

    \sa
    Coordinate Systems
  */
  virtual OdGeMatrix3d getEyeToWorldTransform() const = 0;

  /** \details
    Returns true if and only if perspective mode is on for this Viewport.
  */
  virtual bool isPerspective() const = 0;

  /** \details
    Applies the current perspective transformation to the specified point.

    \param point [in/out] Any 3D point.

    \remarks
    The point is transformed from EyeSpace coordinates to normalized device coordinates.

    Returns true if and only if the point was transformed; i.e., a perspective transform
    is active, and the point was neither too close or behind the camera.

    \sa
    Coordinate Systems
  */
  virtual bool doPerspective(
    OdGePoint3d& point) const = 0;

  /** \details
    Applies the inverse of the current perspective transformation to the specified point.

    \param point [in/out] Any 3D point.

    \remarks
    The point is transformed from normalized device coordinates to EyeSpace coordinates

    Returns true if and only if the point was transformed; i.e., a perspective transform
    is active, and the point was neither too close or behind the camera.

    \sa
    Coordinate Systems
  */
  virtual bool doInversePerspective(
    OdGePoint3d& point) const = 0;

  /** \details
    Returns the display pixel density at the specified point for this Viewport object.

    \param point [in]  WCS center of the unit square.
    \param pixelDensity [out]  Receives the pixel density.
    \param bUsePerspective [in]  Enable perspective inclusion into pixel density calculation.

    \remarks
    Pixel density is measured in pixels per WCS unit.

    This function can be used to determine if the geometry generated for an object will 
    be smaller than the size of a pixel.
  */
  virtual void getNumPixelsInUnitSquare(
    const OdGePoint3d& point, 
    OdGePoint2d& pixelDensity,
    bool bUsePerspective = true) const = 0;

  /** \details
    Returns the WCS camera (eye) location for this Viewport object.
  */
  virtual OdGePoint3d getCameraLocation() const = 0;

  /** \details
    Returns the WCS camera target for this Viewport object.
  */
  virtual OdGePoint3d getCameraTarget() const = 0;

  /** \details
    Returns the WCS camera "up" vector for this Viewport object.
  */
  virtual OdGeVector3d getCameraUpVector() const = 0;

  /** \details
    Returns the vector from the camera target to the camera location.
  */
  virtual OdGeVector3d viewDir() const = 0;

  /** \details
    Returns the OdGi Viewport ID for this Viewport object.

    \remarks
    The value returned has no relationshp to the CVPORT system variable.
  */
  virtual OdUInt32 viewportId() const = 0;

  /** \details
    Returns the ID for this Viewport object..

    \remarks
    The value returned corresponds to the CVPORT system variable.
  */
  virtual OdInt16 acadWindowId() const = 0;

  /** \details
    Returns lower-left and upper-right corners of this Viewport object in 
    normalized device Coordinates.

    \param lowerLeft [out]  Receives the lower-left corner.
    \param upperRight [out]  Receives the upper-right.

    \remarks
    This information lets position items that are fixed in size and/or position
    with respect to a Viewport.

    \sa
    Coordinate Systems      
  */
  virtual void getViewportDcCorners(
    OdGePoint2d& lowerLeft, 
    OdGePoint2d& upperRight) const = 0;

  /** \details
    Returns the front and back clipping parameters for this Viewport object.

    \param clipFront [out]  Receives true if and only if front clipping is enabled.
    \param clipBack [out]   Receives true if and only if back clipping is enabled.
    \param front [out]  Receives the front clipping distance.
    \param back [out]  Receives the back clipping distance.

    \remarks
    Returns clipFront || clipBack.

    front and back are the eye coordinate Z values. 

    Clipping planes are perpendicular to the view direction.

    If the front clipping plane is enabled, geometry in front of it is not displayed.

    If the back clipping plane is enabled, geometry in back of it is not displayed.
  */
  virtual bool getFrontAndBackClipValues(
    bool& clipFront, 
    bool& clipBack, 
    double& front, 
    double& back) const = 0;

  /** \details
    Returns a multiplier that is used to scale all linetypes in this Viewport object.
  */
  virtual double linetypeScaleMultiplier() const = 0;

  /** \details
    Returns the WCS value below which linetype generation is not used.

    \remarks
    If the WCS length an entire linetype pattern is less than the returned value, then
    the linetype will be rendered as continuous.
  */
  virtual double linetypeGenerationCriteria() const = 0;

  /** \details
    Returns true if and only if the specified layer is not frozen either globally or in this Viewport object.

    \param layerId [in]  Object ID of OdLayerTableRecord.
  */
  virtual bool layerVisible(
    OdDbStub* layerId) const = 0;

  /** \details
    Returns pointer to context-dependent colors storage.

    \remarks
    This pointer could be null if contextual colors doesn't specified by context.
  */
  virtual const OdGiContextualColors *contextualColors() const { return NULL; }
};

#include "TD_PackPop.h"

#endif // __GI_VIEWPORT__
