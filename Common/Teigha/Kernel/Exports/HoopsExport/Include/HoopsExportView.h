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
//
//////////////////////////////////////////////////////////////////////

#ifndef _HOOPS_EXPORT_VIEW_INCLUDED_
#define _HOOPS_EXPORT_VIEW_INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "HoopsExportDevice.h"
#include "Gi/GiMapperItem.h"

/** \details
  This namespace represents structures, classes and functions, that implement functionality for exporting to a hoops file.
  
  <group OdExport_Classes> 
*/
namespace TD_HOOPS_COMMON_EXPORT {
#define DIFFUSE_TEXTURE     1
#define BUMP_TEXTURE        2
#define REFLECTION_TEXTURE  3
#define SPECULAR_TEXTURE    4

//////////////////////////////////////////////////////////////////////

/** \details
  This class implements the hoops export view.
  
  <group OdExport_Classes> 
*/
class OdHoopsExportView :   public OdGsBaseVectorizeViewDef
{
public:
  OdGiOrthoClipperPtr             m_pViewportClip;  //!< Geometry clipper object for a viewport.
  OdGiXformPtr                    m_pDcToWorld;     //!< A transformation object for converting from the device coordinates to the world coordinate system (WCS).
  OdGiXformPtr                    m_pWorldToDc;     //!< A transformation object for converting from the world coordinate system (WCS) to the device coordinates.
  
//DOM-IGNORE-BEGIN
private:
  OdGeMatrix3d                    m_DcToWorldMatrix;
  OdGeMatrix3d                    m_WorldToDcMatrix;
  bool                            m_bMaterialCommited;
  bool                            m_bProcessingTTF;
  bool                            m_bExportToViewport;
  const OdGiDrawable*             m_pCurDrawable;
  OdUInt32                        m_iDoDrawFlag;
//DOM-IGNORE-END
  
public:

  /** \details 
    Creates an instance of the hoops view class.
  */
  OdHoopsExportView();
  
  /** \details 
    Destroys the hoops view object.
  */
  virtual ~OdHoopsExportView();

  /** \details 
    Creates an instance of the hoops view object. It is the pseudo-constructor that creates an instance of this class and returns the typified smart pointer to it.
  */
  static OdGsViewPtr   createObject();
  
  /** \details 
    Returns a pointer to an export device related to the view object.
  */
  OdHoopsExportDevice* device();

  /** \details 
    Stops the current rendering operation.
    
    \returns Returns a flag that shows whether the current rendering operation has been aborted successfully (if true) or not (if false).
    
    \remarks 
    This method can be called by Teigha Rendering Framework to give a client application a chance to terminate the current rendering process. 
  */
  bool regenAbort() const
  {
    return false;
  }
  
  /** \details 
    Returns the deviation value of a specified type for a specified three-dimensional point.
    
    \remarks 
    Current implementation returns a zero value (0.0) to make Teigha Rendering Framework use a reasonable value.
  */
  double deviation(const OdGiDeviationType, const OdGePoint3d&) const
  {
    // to force Teigha Rendering Framework to use a reasonable value.
    return 0.0; 
  }

  /** \details 
    Implements a reaction to changing any attributes of hoops graphics elements, such as color, linetypes, layer and so on.
  */
  void onTraitsModified();
  
  /** \details 
    Starts the vectorization process for the view.
  */
  virtual void beginViewVectorization();
  
  /** \details 
    Ends the vectorization process for the view.
  */
  virtual void endViewVectorization();
  
  /** \details 
    Loads a viewport for this view.
  */
  virtual void loadViewport();

  /** \details 
    Draws a specified drawable object using the export device associated with this view.
    
    \param drawableFlags  [in] Flag that determines drawing parameters.
    \param pDrawable      [in] A drawable entity to draw.
  */
  bool doDraw(OdUInt32 drawableFlags, const OdGiDrawable* pDrawable);

  /** \details 
    Sets the view parameters used for transformation from the world coordinate system (WCS) to device coordinates. 
    
    \param position     [in] A point that determines the view position.
    \param target       [in] A point that determines the view target.
    \param upVector     [in] A vector that determines upward moving of the view.
    \param fieldWidth   [in] A projection width of the view.
    \param fieldHeight  [in] A projection height of the view.
    \param projection   [in] A view projection type (perspective or parallel).
  */
  virtual void setView(const OdGePoint3d & position, const OdGePoint3d& target,
                       const OdGeVector3d& upVector, double fieldWidth,
                       double fieldHeight, Projection projection = kParallel);

  /**\details 
    Vectorizes an infinite line, defined with two three-dimensional points.
    
    \param p1 [in] A first point to define an infinite line.
    \param p2 [in] A second point to define an infinite line.
  */
  virtual void xline( const OdGePoint3d& p1, const OdGePoint3d& p2 );
  
  /**\details 
    Vectorizes an infinite ray, defined with two three-dimensional points.
    
    \param start  [in] A ray starting point.
    \param pt2    [in] A second point to define a ray.
  */
  virtual void ray( const OdGePoint3d& start, const OdGePoint3d&  pt2 );

  /**\details 
    Vectorizes specified text at the specified position and style.
    
    \param position  [in] A point to define the text position.
    \param normal    [in] A vector to define a normal for the text plane.
    \param direction [in] A vector to define the text direction.
    \param msg       [in] A text to be vectorized represented by a pointer to an OdChar object.
    \param length    [in] A text length.
    \param raw       [in] A flag value that determines whether escape symbols should be interpreted.
    \param pTextStyle[in] A style to be used for text vectorizing.
  */
  virtual void text(const OdGePoint3d& position, const OdGeVector3d& normal, const OdGeVector3d& direction,
                    const OdChar* msg, OdInt32 length, bool raw, const OdGiTextStyle* pTextStyle );

  /**\details 
    Adds an all-directional light for the view.
    
    \param LightData [in] A set of light parameters (position of light source, color, intensity).
  */
  virtual void addPointLight(const OdGiPointLightTraitsData&);
  
  /**\details 
    Adds a source of a spotlight for the view.
    
    \param LightData [in] A set of spotlight parameters (direction, color, intensity).
  */
  virtual void addSpotLight(const OdGiSpotLightTraitsData&);
  
  /**\details 
    Adds a source of distant light for the view.
    
    \param LightData [in] A set of distant light parameters (direction, color, intensity).
  */
  virtual void addDistantLight(const OdGiDistantLightTraitsData&);
  
//DOM-IGNORE-BEGIN
protected:

  void processMaterialNode(OdDbStub *pMaterialId, OdGsMaterialNode *pNode);
  void processMaterialNode(OdDbStub *pMaterialId, OdGsMaterialNode *pNode, const OdGiMaterialTraitsData &materialTraits, const OdGiMapper *pMapper);

  bool GetCurObjectExtents( OdGeExtents3d& extents );

  friend class OdHoopsExportDevice;
//DOM-IGNORE-END
}; // end OdHoopsExportView 
}
#endif // #ifndef _HOOPS_EXPORT_VIEW_INCLUDED_
