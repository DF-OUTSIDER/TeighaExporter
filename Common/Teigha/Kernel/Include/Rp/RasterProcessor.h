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


#ifndef _OD_ODRASTERPROCESSOR_H_
#define _OD_ODRASTERPROCESSOR_H_

#include "RxObject.h"
#include "RxModule.h"
#include "Gi/GiRasterImage.h"

#include "TD_PackPush.h"

/** \details
    This class is the Raster Converter Protocol Extension class.
    Corresponding C++ library: RasterProcessor
    <group Other_Classes>
*/
class FIRSTDLL_EXPORT OdRasterConvertPE : public OdRxObject
{
public:
  ODRX_DECLARE_MEMBERS(OdRasterConvertPE);

  /** \details
    This function applies the specified raster conversions to the specified RasterImage object.

    \remarks
    These conversions include filtering, flips, resampling, resizing, rotations, and trimming.
    
    \param pOrig [in]  Pointer to the original image.
    \param convertPaletteToRGB [in]  Convert palette to RGB.
    \param brightness [in]  Brightness. [0.0 .. 100.0].
    \param contrast [in]  Contrast. [0.0 .. 100.0].
    \param fade [in]  Fade. [0.0 .. 100.0].
    \param backgroundColor [in]  Background color.
    \param flipX [in]  Flip along the X-axis.
    \param flipY [in]  Flip along the Y-axis.
    \param rotate90 [in]  Rotate 90° CCW.
    \param pDestDesc [in]  Pointer to the destination image description..
    \param transparency [in]  Make transparent.
    
  */
  virtual OdGiRasterImagePtr convertImage(
    const OdGiRasterImage* pOrig,
    bool convertPaletteToRGB,
    double brightness = 50.0, double contrast = 50.0, double fade = 0.0, ODCOLORREF backgroundColor = 0,
    bool flipX = false, bool flipY = false, bool rotate90 = false,
    const OdGiRasterImage* pDestDesc = 0,
    bool transparency = false) = 0;

  /** \details
    Brightness, contrast, fade adjustments behaviour.

    \remarks
    kDwgBehaviour is default state for adjustments behaviour.
  */
  enum AdjustmentsBehaviour
  {
    kDwgBehaviour = 0,
    kDgnBehaviour = 1
  };

  /** \details
    Setup adjustments behaviour for next convertImage calls.

    \param newBehaviour [in]  new adjustments behaviour state.
  */
  virtual void setAdjustmentsBehaviour(AdjustmentsBehaviour newBehaviour) = 0;

  /** \details
    Returns current adjustments behaviour state.
  */
  virtual AdjustmentsBehaviour adjustmentsBehaviour() const = 0;
};

/** \details
    This template class is a specialization of the OdSmartPtr class for OdRasterConvertPE object pointers.
*/
typedef OdSmartPtr<OdRasterConvertPE> OdRasterConvertPEPtr;

/** \details
    This class is the base class for classes that provide platform-dependent raster processing services for Teigha.

    Corresponding C++ library: RasterProcessor
    <group Other_Classes>
*/
class FIRSTDLL_EXPORT OdRasterProcessingServices : public OdRxModule
{
public:
  ODRX_DECLARE_MEMBERS(OdRasterProcessingServices);
};

/** \details
    This template class is a specialization of the OdSmartPtr class for OdRasterProcessingServices object pointers.
*/
typedef OdSmartPtr<OdRasterProcessingServices> OdRasterProcessingServicesPtr;

#include "TD_PackPop.h"

#endif //#ifndef _OD_ODRASTERPROCESSOR_H_
