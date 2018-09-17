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
// PdfPublishCamera.h
//
///////////////////////////////////////////////////////////////////////////////

#ifndef _PDF_PUBLISH_CAMERA_INCLUDED_
#define _PDF_PUBLISH_CAMERA_INCLUDED_

#include "PdfPublishBaseObject.h"

//DOM-IGNORE-BEGIN
class OdGePoint3d;
class OdGeVector3d;
//DOM-IGNORE-END

/** \details 
Contains declarations for Teigha Publish.
*/
namespace OdPdfPublish{

/** \details
  This class implements a camera entity for .pdf documents created with Teigha Publish.
*/
class PDFPUBLISH_TOOLKIT OdCamera : public OdObject
{
  //DOM-IGNORE-BEGIN
protected:
  OdCamera();
  //DOM-IGNORE-END
public:
  //DOM-IGNORE-BEGIN
  ODRX_DECLARE_MEMBERS(OdCamera);
  ODRX_HEAP_OPERATORS();
  //DOM-IGNORE-END

  /** \details
    Destroys a camera object.
  */
  virtual ~OdCamera();
  
  /** \details 
    Changes the camera position and its target by moving it in a specified direction. 
    Moving direction is defined through move step sizes aling x, y and z coordinate acises.
    Move amounts are represented as world-space measurement units. 
   
    \param x_dir    [in] A movement step in the X-direction. The value is a result of crossing up and forward verctors.
    \param up       [in] A movement step in up (if positive) or down (if negative) direction.
    \param forward  [in] A movement step to (if positive) or from (if negative) to the target. 
  */
  void dolly(double x_dir, double up, double forward);
  
  /** \details 
    Orbits the camera about its target with a specified angle to the right and to the up.
    
    \param theta [in] An angle to orbit camera to the right (in degrees).
    \param phi   [in] An angle to orbit the camera to the up (in degrees).
    
    \remarks 
    The camera moves on a sphere with the center at the camera target and radius that is equal to the distance from the camera to target. 
    In other words, the distance between camera and its target remain unchanged.
    The camera is firstly rotated about its target on theta angle, then rotated on phi angle. 
  */
  void orbit(double theta, double phi);
  
  /** \details 
    Changes the camera position and its target by moving it in the XY-plane with a specified degrees. 
    The camera position and distance between camera and its target remain unchanged.
    
    \param theta [in] The number of degrees to move the the camera target to the right. 
    \param phi   [in] The number of degrees to move the the camera target up. 
  */
  void pan(double theta, double phi);
  
  /** \details 
    Rotates the camera around its target with a specified angle (in degrees). 
    The camera position, target and fild remain unchanged. 
    If the rotation angle is positive, camera rotation is clock-wise, otherwise camera rotation is counterclock-wise.
    
    \param theta    [in] 	A rotation angle (in degrees). 
  */
  void roll(double theta);
  
  /** \details 
    Zooms the camera view field with a specified scale factor.
    
    \param zoom [in] A zoom scale value.
    
    \remarks
    Zoom operation means that the sizes of the related view field will be recalculated using the proportion specified in zoom parameter.
    If zoom parameter value is greater than 1.0, objects on screen becomes larger. 
    If zoom parameter is less than 1.0 and greater than 0.0, objects on screen view becomes smaller.
  */
  void zoom(double zoom);

  /** \details 
    Sets the camera field. 

    \param width     [in] 	A width value of the camera field (in object measuring units). 
    \param height    [in] 	A height value of the camera field (in object measuring units). 
  */
  void setField(double width, double height);

  /** \details 
    Sets the camera near limit. 

    \param limit [in] A new near limit value. Should be between 0.0 and 1.0.
    
    \remarks 
    The near limit determines how close to the target the camera is. Its value should be between 0 and 1. 
    The values 0 and 1 mean that near limit is as close to the camera position (if 0) or to the camera target (if 1)
    as it possible according the precision of z-buffer.
  */
  void setNearLimit(double const limit);
  
  /** \details 
    Sets the camera position. 

    \param position [in] A new camera position represented with a three-dimensional point.
    
    \sa 
    <exref target="https://docsdev.opendesign.com/td/OdGePoint3d.html">OdGePoint3d class</exref>.
  */
  void setPosition(const OdGePoint3d& position);

  /** \details 
    Sets the camera projection.
    Camera projection is defined with projection type and oblique skews in horizontal and vertical directions.

    \param type [in] A new camera projection type.
    \param oblique_y_skew [in] A new vertical oblique skew value.
    \param oblique_x_skew [in] A new horizontal skew.
    
    \sa
    <link OdPdfPublish::Camera::Projection, Camera::Projection enumeration>.
  */
  void setProjection(Camera::Projection type, double oblique_y_skew = 0., double oblique_x_skew = 0.);
  
  /** \details 
    Sets the camera target. 

    \param target [in] A new camera target represented with a three-dimensional point.
    
    \sa 
    <exref target="https://docsdev.opendesign.com/td/OdGePoint3d.html">OdGePoint3d class</exref>.
  */
  void setTarget(const OdGePoint3d& target);
  
  /** \details 
    Sets the camera up-vector.

    \param vector [in] A new camera up-vector.
    
    \sa 
    <exref target="https://docsdev.opendesign.com/td/OdGeVector3d.html">OdGeVector3d class</exref>.
  */
  void setUpVector(const OdGeVector3d& vector);

  /** \details 
    Returns current camera field information.
    The method fills passed double values with width and height of the camera field and returns them to a calling subroutine.

    \param width     [out] 	A placeholder for the width value of the camera field. 
    \param height    [out] 	A placeholder for the height value of the camera field.
    
    \remarks 
    Camera field width and height are measured in in object units.
  */
  void getField(double& width, double& height) const;
  
  /** \details 
    Returns current camera near limit. 
    The method fills a passed double value with current near limit and returns it to a calling subroutine.

    \param limit [out] A placeholder for the near limit value.
    
    \remarks 
    The near limit determines how close to the target the camera is. Its value should be between 0 and 1. 
    The values 0 and 1 mean that near limit is as close to the camera position (if 0) or to the camera target (if 1)
    as it possible according the precision of z-buffer.
  */
  void getNearLimit(double& limit) const;
  
  /** \details 
    Returns current camera position. 
    The method fills a passed three-dimensional point object and returns it to a calling subroutine.

    \param position [out] A placeholder for the camera position point.
    
    \sa 
    <exref target="https://docsdev.opendesign.com/td/OdGePoint3d.html">OdGePoint3d class</exref>.
  */
  void getPosition(OdGePoint3d& position) const;
  
  /** \details 
    Returns current camera projection.
    Camera projection is defined with projection type and oblique skews in horizontal and vertical directions.
    The method fills passed values with current type of projection and obligue skew values and returns them to a calling subroutine.

    \param type           [out] A placeholder for the camera projection type.
    \param oblique_y_skew [out] A placeholder for the vertical oblique skew value.
    \param oblique_x_skew [out] A placeholder for the horizontal skew.
    
    \sa
    <link OdPdfPublish::Camera::Projection, Camera::Projection enumeration>.
  */
  void getProjection(Camera::Projection& type, double& oblique_y_skew, double& oblique_x_skew) const;
  
  
  /** \details 
    Returns current camera target. 
    The method fills a passed three-dimensional point object and returns it to a calling subroutine.

    \param target [out] A placeholder for the camera target point.
    
    \sa 
    <exref target="https://docsdev.opendesign.com/td/OdGePoint3d.html">OdGePoint3d class</exref>.
  */
  void getTarget(OdGePoint3d& target) const;
  
  /** \details 
    Returns current camera up-vector.
    The method fills a passed three-dimensional vector object and returns it to a calling subroutine.

    \param vector [out] A placeholder for the camera up-vector.
    
    \sa 
    <exref target="https://docsdev.opendesign.com/td/OdGeVector3d.html">OdGeVector3d class</exref>.
  */
  void getUpVector(OdGeVector3d& vector) const;

};

/** \details
  A data type that represents a smart pointer to an <link OdPdfPublish::OdCamera, OdCamera> object.
*/
SMARTPTR(OdCamera);

}
#endif // _PDF_PUBLISH_CAMERA_INCLUDED_
