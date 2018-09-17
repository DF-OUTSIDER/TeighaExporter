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

#ifndef __ODGIIMAGE_H__
#define __ODGIIMAGE_H__

#include "GiExport.h"
#include "UInt32Array.h"
#include "RxObject.h"

#include "TD_PackPush.h"

// ODCOLORREF is RGBA32

/** \details
    This class defines pixel color parameters in RGB format, which stores an integer
	for red, green, and blue values, and also includes a value for alpha-shading.

    Corresponding C++ library: TD_Gi

    <group OdGi_Classes> 
*/
class OdGiPixelBGRA32
{
  public:
    OdGiPixelBGRA32 ()
    {
      m_bgra = ODRGBA(0, 0, 0, 255);
    }

    OdGiPixelBGRA32(OdUInt32 bgra)
      : m_bgra(bgra)
    {
    }

    OdGiPixelBGRA32(OdUInt8 blue, OdUInt8 green, OdUInt8 red, OdUInt8 alpha)
    {
      m_bgra = ODRGBA(blue, green, red, alpha);
    }

    OdUInt32 getBGRA() const;
    OdUInt32 getRGBA() const;
    OdUInt8 blue() const;
    OdUInt8 green() const;
    OdUInt8 red() const;
    OdUInt8 alpha() const;

    void setBGRA(OdUInt32 bgra);
    void setBGRA(OdUInt8 blue, 
                 OdUInt8 green, 
                 OdUInt8 red,
                 OdUInt8 alpha);
    void setRGBA(OdUInt32 rgba);
    void setRGBA(OdUInt8 red, 
                 OdUInt8 green, 
                 OdUInt8 blue,
                 OdUInt8 alpha);
    void setBlue(OdUInt8 blue);
    void setGreen(OdUInt8 green);
    void setRed(OdUInt8 red);
    void setAlpha(OdUInt8 alpha);
  private:
    OdUInt32 m_bgra;
};

/** \details
      Gets the color values in blue, green, red, alpha order.
*/
inline OdUInt32 OdGiPixelBGRA32::getBGRA() const
{
  return m_bgra;
}

/** \details
      Gets the color values in red, green, blue, alpha order.
*/
inline OdUInt32 OdGiPixelBGRA32::getRGBA() const
{
  return ODRGBA(ODGETBLUE(m_bgra), ODGETGREEN(m_bgra), ODGETRED(m_bgra), ODGETALPHA(m_bgra));
}

/** \details
      Gets the blue value of the color.
*/
inline OdUInt8 OdGiPixelBGRA32::blue() const
{
  return ODGETRED(m_bgra);
}

/** \details
      Gets the green value of the color.
*/
inline OdUInt8 OdGiPixelBGRA32::green() const
{
  return ODGETGREEN(m_bgra);
}

/** \details
      Gets the red value of the color.
*/
inline OdUInt8 OdGiPixelBGRA32::red() const
{
  return ODGETBLUE(m_bgra);
}

/** \details
      Gets the alpha shading value of the color.
*/
inline OdUInt8 OdGiPixelBGRA32::alpha() const
{
  return ODGETALPHA(m_bgra);
}

/** \details
  Sets the color values in blue, green, red, alpha order.
*/
inline void OdGiPixelBGRA32::setBGRA(OdUInt32 bgra)
{
  m_bgra = bgra;
}

/** \details
  Sets the color values in blue, green, red, alpha order.
*/
inline void OdGiPixelBGRA32::setBGRA(OdUInt8 blue, 
                                     OdUInt8 green, 
                                     OdUInt8 red,
                                     OdUInt8 alpha)
{
  m_bgra = ODRGBA(blue, green, red, alpha);
}

/** \details
      Sets the color values in red, green, blue, alpha order.
*/
inline void OdGiPixelBGRA32::setRGBA(OdUInt32 rgba)
{
  m_bgra = ODRGBA(ODGETBLUE(rgba), ODGETGREEN(rgba), ODGETRED(rgba), ODGETALPHA(rgba));
}

/** \details
      Sets the color values in red, green, blue, alpha order.
*/
inline void OdGiPixelBGRA32::setRGBA(OdUInt8 red, 
                                     OdUInt8 green, 
                                     OdUInt8 blue,
                                     OdUInt8 alpha)
{
  m_bgra = ODRGBA(blue, green, red, alpha);
}

/** \details
      Sets the blue value of a color.
*/
inline void OdGiPixelBGRA32::setBlue(OdUInt8 blue)
{
  m_bgra = ODRGBA(blue, ODGETGREEN(m_bgra), ODGETBLUE(m_bgra), ODGETALPHA(m_bgra));
}

/** \details
      Sets the green value of a color.
*/
inline void OdGiPixelBGRA32::setGreen(OdUInt8 green)
{
  m_bgra = ODRGBA(ODGETRED(m_bgra), green, ODGETBLUE(m_bgra), ODGETALPHA(m_bgra));
}

/** \details
      Sets the red value of a color.
*/
inline void OdGiPixelBGRA32::setRed(OdUInt8 red)
{
  m_bgra = ODRGBA(ODGETRED(m_bgra), ODGETGREEN(m_bgra), red, ODGETALPHA(m_bgra));
}

/** \details
      Sets the alpha-shading value of a color.
*/
inline void OdGiPixelBGRA32::setAlpha(OdUInt8 alpha)
{
  m_bgra = ODRGBA(ODGETRED(m_bgra), ODGETGREEN(m_bgra), ODGETBLUE(m_bgra), alpha);
}

/** \details
    This class defines image color parameters in RGB format, which stores an integer
	for red, green, and blue values, and includes a value for alpha-shading.

    Corresponding C++ library: TD_Gi

    <group OdGi_Classes> 
*/
class OdGiImageBGRA32
{
  public:
    OdGiImageBGRA32()
      : m_pImageData(NULL)
      , m_nImageWidth(0)
      , m_nImageHeight(0)
    {
    }

    OdGiImageBGRA32(OdUInt32 imageWidth, OdUInt32 imageHeight, OdGiPixelBGRA32* imageData)
      : m_pImageData(imageData)
      , m_nImageWidth(imageWidth)
      , m_nImageHeight(imageHeight)
    {
    }

    OdGiPixelBGRA32* image() const;
    OdUInt32 width() const;
    OdUInt32 height() const;

    void setImage(OdUInt32 imageWidth, 
                  OdUInt32 imageHeight, 
                  OdGiPixelBGRA32* imageData);
  private:
    OdGiPixelBGRA32* m_pImageData;
    OdUInt32 m_nImageWidth;
    OdUInt32 m_nImageHeight;
};

/** \details
      Gets the buffer data of an image.
*/
inline OdGiPixelBGRA32* OdGiImageBGRA32::image() const
{
  return m_pImageData;
}

/** \details
      Gets the width of an image.
*/
inline OdUInt32 OdGiImageBGRA32::width() const
{
  return m_nImageWidth;
}

/** \details
      Gets the height of an image.
*/
inline OdUInt32 OdGiImageBGRA32::height() const
{
  return m_nImageHeight;
}

/** \details
      Sets the buffer data, width, and height of an image.
*/
inline void OdGiImageBGRA32::setImage(OdUInt32 imageWidth, 
                                      OdUInt32 imageHeight, 
                                      OdGiPixelBGRA32* imageData)
{
  m_nImageWidth = imageWidth;
  m_nImageHeight = imageHeight;
  m_pImageData = imageData;
}

/** \details
    This template class is a specialization of the OdArray class for OdGiPixelBGRA32 objects.
*/
typedef OdArray<OdGiPixelBGRA32, OdMemoryAllocator<OdGiPixelBGRA32> > OdGiPixelBGRA32Array;

#include "TD_PackPop.h"

#endif // __ODGIIMAGE_H__
