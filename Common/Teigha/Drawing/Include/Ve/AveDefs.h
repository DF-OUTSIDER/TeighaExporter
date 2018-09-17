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



#ifndef OD_AVEDEFS_H
#define OD_AVEDEFS_H

#include "Ve/AveExport.h"
#include "OdString.h"
#include "RxIterator.h"

/** \details

    <group TD_Namespaces>
*/
namespace OdAve
{
  enum MaterialType
  {
    kStandard     = 0,
    kGranite      = 1,
    kMarble       = 2,
    kWood         = 3
  };

  enum MaterialAttrib
  {
    kAmbient        = 0x1,  
    kDiffuse        = 0x2,  
    kSpecular       = 0x4,  
    kTransparency   = 0x8,  
    kRoughness      = 0x20, 
    kBumpMap        = 0x40, 
    kReflectionProp = 0x100,
    kRefraction     = 0x200
  };

  enum RenderType
  {
    kRndSimple      = 30,
    kRndPhotoReal   = 50,
    kRndRaytrace    = 60
  };

  enum RenderDestination
  {
    kDestViewport   = 20,
    kDestWindow     = 30,
    kDestFile       = 40
  };

  enum RenderQuality
  {
    kGouraud        = 0,
    kPhong          = 1
  };

  enum AntiAliasing
  {
    kAntiAliasNone      = 1,
    kAntiAliasMinimal   = 2,
    kAntiAliasLow       = 3,
    kAntiAliasMedium    = 4,
    kAntiAliasHigh      = 5
  };

  enum TextureMapSampling
  {
    kTMapSampPoint    = 1,
    kTMapSampLinear   = 2,
    kTMapSampMipMap   = 3
  };

  enum BackgroundType
  {
    kBackgroundSolid      = 0,
    kBackgroundGradient   = 1,
    kBackgroundImage      = 2,
    kBackgroundMerge      = 3
  };

  enum OptionType
  {
    kRndPref              = 0,
    kRndFullOpt           = 1,
    kRndQuickOpt          = 2,
    kRndScanlOpt          = 3,
    kRndRaytrOpt          = 4, 
    kRndRFileOpt          = 5,
    kRndFogOpt            = 6,
    kRndBGOpt             = 7,  
    kRndOptAmount         = 8
  };

  enum ProjectionMethod
  {
    kPlanar               = 1,
    kBox                  = 2,
    kCylinder             = 3,
    kSphere               = 4
  };

  enum TileMethod
  {
    kTmDefault    = 0, // Tiling as set for the material.
    kTmTile       = 1, // Tiled.
    kTmCrop       = 2, // Cropped.
    kTmClamp      = 3  // Clamped.
  };

  enum SetuvType
  {
    kSetuvPlanar      = 1,
    kSetuvCylindrical = 2,
    kSetuvSpherical   = 3,
    kSetuvSolid       = 6
  };
}

/** \details
    This class is the base class for AVE Bitmap objects.
    
    Corresponding C++ library: TD_Ave
    <group OdAve_Classes>
*/
class AVE_EXPORT OdAveBitmap
{
public:
  virtual ~OdAveBitmap() {}

  /** \details
    Returns the blend factor for this Bitmap object.

    \remarks
    Returns a value in the range [0.0 .. 1.0]

    A blend factor of 0.0 makes the Bitmap invisible. A blend factor of 1.0 makes it opaque.
    In between, the Bitmap is transparent.
  */
  virtual double blendFactor() = 0;
  
  /** \details
    Sets the source filename of this Bitmap object.

    \param filename [out] Receives the filename.
  */
  virtual void source(OdString& filename) = 0;
  
  /** \details
    Returns the scale of this Bitmap object in the U-direction.
    
    \remarks
    This is the number of repetitions of this Bitmap object in the specified direction.
  */
  virtual double scaleU() = 0;
  /** \details
    Returns the scale of this Bitmap object in the V-direction.
    
    \remarks
    This is the number of repetitions of this Bitmap object in the specified direction.
  */
  virtual double scaleV() = 0;
  /** \details
    Returns the offset of this Bitmap object in the U-direction.
    \remarks
    Returns a value in the range [-1.0 .. 1.0].
  */
  virtual double offsetU() = 0;
  /** \details
    Returns the offset of this Bitmap object in the V-direction.
    \remarks
    Returns a value in the range [-1.0 .. 1.0].
  */
  virtual double offsetV() = 0;
  /** \details
    Returns the tile method of this Bitmap object.
    \remarks
    Returns one of the following:
    
    <table>
    Name          Value   Description
    kTmDefault    0       Tiling as set for the material.
    kTmTile       1       Tiled.
    kTmCrop       2       Cropped.
    kTmClamp      3       Clamped.
    </table>
  */
  virtual OdAve::TileMethod tileMethod() = 0;
  /** \details
    Returns true if and only if this Bitmap object uses Auto Axis.
    \remarks
    AutoAxis automatically maps materials to surfaces 
    whose normals are the X, Y, and Z axes.
    
    If AutoAxis is not used, only surfaces whose normal is the Z axis are mapped.
  */
  virtual bool useAutoAxis() = 0;
  /** \details
    Returns true if and only if this Bitmap object is automatically scaled
    to the object to which this bitmap is attached.
  */
  virtual bool fitToObject() = 0;

  /** \details
    Sets the blend factor for this Bitmap object.
    \param blendFactor [in]  Blend factor. [0.0 .. 1.0]
    \remarks
    A blend factor of 0.0 makes the Bitmap invisible. A blend factor of 1.0 makes it opaque.
    In between, the Bitmap is transparent.
  */
  virtual void setBlendFactor(double blendFactor) = 0;
  /** \details
    Returns the source filename of this Bitmap object.

    \remarks
    \param filename [in] Filename.
  */
  virtual void setSource(const OdString& filename) = 0;
  /** \details
    Sets the scale of this Bitmap object in the U-direction.
    \param scaleU [in]  Scale in U-direction.
    \remarks
    This is the number of repetitions of this Bitmap object in the specified direction.
  */
  virtual void setScaleU(double scaleU) = 0;
  /** \details
    Sets the scale of this Bitmap object in the V-direction.
    \param scaleV [in]  Scale in V-direction.
    \remarks
    This is the number of repetitions of this Bitmap object in the specified direction.
  */
  virtual void setScaleV(double scaleV) = 0;
  /** \details
    Sets the offset of this Bitmap object in the U-direction.
    \param offsetU [in]  Offset in U-direction. [-1.0 .. 1.0].
  */
  virtual void setOffsetU(double offsetU) = 0;
  /** \details
    Sets the offset of this Bitmap object in the V-direction.
    \param offsetV [in]  Offset in V-direction. [-1.0 .. 1.0].
  */
  virtual void setOffsetV(double offsetV) = 0;
  /** \details
    Sets the tile method of this Bitmap object.
    
    \param tileMethod [in]  Tile method.
    
    \remarks
    tileMethod must be one of the following:
    
    <table>
    Name          Value   Description
    kTmDefault    0       Tiling as set for the material.
    kTmTile       1       Tiled.
    kTmCrop       2       Cropped.
    kTmClamp      3       Clamped.
    </table>
  */
  virtual void setTileMethod(OdAve::TileMethod tileMethod) = 0;
  /** \details
    Controls the use of Auto Axis by this Bitmap object.
    \param useAutoAxis [in]  Controls use of Auto Axis.
    \remarks
    AutoAxis automatically maps materials to surfaces 
    whose normals are the X, Y, and Z axes.
    
    If AutoAxis is not used, only surfaces whose normal is the Z axis are mapped.
  */
  virtual void setUseAutoAxis(bool useAutoAxis) = 0;
 
  /** \details
    Controls the automatically scaling of this Bitmap object to the object to which
    it is attached.
    \param fitToObject [in]  Controls automatic scaling.
  */
  virtual void setFitToObject(bool fitToObject) = 0;
};

#endif // OD_AVEDEFS_H
