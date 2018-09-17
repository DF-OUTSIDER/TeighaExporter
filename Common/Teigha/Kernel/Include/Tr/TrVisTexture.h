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
// GLES2 device textures

#ifndef ODTRVISTEXTURE
#define ODTRVISTEXTURE

#include "TD_PackPush.h"

#include "TrVisDefs.h"

/** \details
  This interface is a common texture interface
*/

class OdTrVisTexture : public OdRxObject
{
protected:
  OdUInt32 m_nWidth, m_nHeight;
  OdUInt32 m_nAlignment, m_nScanLength;
  void *m_pTextureData;

  OdTrVisTexture();

public:
  /** \details
    Supported image formats
  */
  enum Format
  {
    kRGB,
    kBGR,
    kRGBA,
    kBGRA,
    kFloatRGBA,
    kPalette,
    kHalfFloatRGBA,
    kRed,
    kAlpha,
    kColorDepth32bit
  };
  /** \details
    This union represents pixel color.

    \remarks
    If image is integer-based (or OdUInt8-based) integer part should be used, otherwise float part
  */
  union TextureColor
  {
    OdUInt32 intRGBA[ 4 ];
    float floatRGBA[ 4 ];
  };
  /** \details
  This structure is required for correct TextureColor transformation between different formats
  */
  struct TextureColorContext
  {
    enum Difference
    {
      kNone = 0,
      kDifferentDataTypes = 1,
      kDifferentScales = 2
    };
    OdUInt8 difference;

    void prepareContext( const OdTrVisTexture* pSrc, const OdTrVisTexture* pDst );
    
    bool isSrcInt;
    bool isDstInt;
    TextureColor srcMax;
    TextureColor dstMax;
    double scalesMult[ 4 ];
  };

  virtual ~OdTrVisTexture();

  // non-virtual part of OdTrVisTexture inline interface.
 
  // Typical data accessors
  OdUInt32 getTextureWidth() const { return m_nWidth; }
  OdUInt32 getTextureHeight() const { return m_nHeight; }
  OdUInt32 getScanlineLength() const { return m_nScanLength; }
  OdUInt32 getDataAlignment() const { return m_nAlignment; }

  const OdUInt8 *textureData() const { return (const OdUInt8*)m_pTextureData; }
  OdUInt8 *textureData() { return (OdUInt8*)m_pTextureData; }

  // Part of virtual OdTrVisTexture interface
 
  // Some types of texture formats can have limitation (for example S3TC - DXT formats require consists from 4x4 blocks, so texture size must be divideable by 4).
  virtual OdUInt32 subDivLimit() const = 0;

  // Typically textures store data as integers, but may as floats. So we require ability to access texture pixels similarly for all texture sub-types.
  virtual bool isFPImage() const = 0;

  virtual void getColor(OdUInt32 nX, OdUInt32 nY, TextureColor &color) const = 0;
  virtual void setColor( OdUInt32 nX, OdUInt32 nY, const TextureColor &color ) = 0;
 
  // This is also can be helpful for algorithms optimization to have accessors for entire scanlines
  virtual void getScanline(OdUInt32 startLine, OdUInt32 linesCount, TextureColor *pScanline) const
  {
    for( OdUInt32 i = 0; i < linesCount; ++i )
    {
      OdUInt32 offset = getTextureWidth() * i;
      for( OdUInt32 j = 0; j < getTextureWidth(); ++j )
      {
        getColor( j, startLine + i, pScanline[ j + offset ] );
      }
    }
  }

  virtual void setScanline(OdUInt32 startLine, OdUInt32 linesCount, const TextureColor *pScanline)
  {
    for( OdUInt32 i = 0; i < linesCount; ++i )
    {
      OdUInt32 offset = getTextureWidth() * i;
      for( OdUInt32 j = 0; j < getTextureWidth(); ++j )
      {
        setColor( j, startLine + i, pScanline[ j + offset ] );
      }
    }
  }
 
  // Probably we will require color setters or etc.
 
  // Textures in Lut format contain color indexes. Color palette can be stored simply as another 1D texture.
  virtual void setPalette(OdTrVisTexture *pTexture) = 0;
  virtual OdTrVisTexture *palette() = 0;
  virtual const OdTrVisTexture *palette() const = 0;
 
  // We can't rescale indexed textures with bilinear interpolation
  virtual bool isSmoothResampleSupported() const = 0;

  virtual Format format() const = 0;
  /** \details
  Since different format may have different color value intervals (0-255, 0.f - 1.f) we need this function to obtain maxs
  */
  virtual TextureColor getMaxValues() const = 0;
  /** \details
  This function converts TextureColors from one format to another (OdUInt8-based to float-based, etc) using TextureColorContext
  */
  static void prepareTextureColors( TextureColor* colors, OdUInt32 size, const TextureColorContext& context );
  /** \details
  Sometimes image have to be preprocessed befor we can call setColor or setScanline (Palette or compressed images, for example)
  */
  virtual void beginEditing() = 0;
  virtual void endEditing() = 0;
};

typedef OdSmartPtr< OdTrVisTexture > OdTrVisTexturePtr;

/** \details
  This class manage Texture operations
*/
class OdTrVisTextureManager
{
public:
  enum Resampler
  {
    kNearest,
    kBilinear
  };
public:
  /** \details
  Creates empty texture
  */
  static OdTrVisTexturePtr createTexture( OdTrVisTexture::Format fmt, OdUInt32 nWidth, OdUInt32 nHeight, OdUInt32 nAlign = 4 );
  /** \details
  Converts existing texture to the another format (or data alignment)
  */
  static OdTrVisTexturePtr convert( OdTrVisTexture::Format fmt, OdTrVisTexture* pTexture, OdUInt32 nNewAlign = 0, OdTrVisTexture* pPalette = NULL );
  /** \details
  Converts existing texture to the another texture format
  */
  static OdTrVisTexturePtr convert( OdTrVisTexture *pTextureTo, const OdTrVisTexture* pTextureFrom, Resampler method = kNearest );
  /** \details
  Resamples existing texture
  */
  static OdTrVisTexturePtr resample( Resampler method, OdUInt32 newWidth, OdUInt32 newHeight, OdTrVisTexture* pTexture );
  /** \details
  Converts and resamples existing texture
  */
  static OdTrVisTexturePtr convertAndResample( OdTrVisTexture::Format fmt, Resampler method, OdUInt32 newWidth, OdUInt32 newHeight, OdTrVisTexture* pTexture, OdUInt32 nNewAlign = 0, OdTrVisTexture* pPalette = NULL );
  /** \details
  Resample texture so its width and height becomes the power of 2 (if need). Width and height are limited by nMaxSize
  */
  static OdTrVisTexturePtr resamplePow2(Resampler method, bool bRequirePow2, OdTrVisTexture *pTexture, OdUInt32 nMaxSize);
  /** \details
  Creates textures from buffer
  */
  static OdTrVisTexturePtr createTextureFrom( OdTrVisTexture::Format fmt, OdUInt32 nWidth, OdUInt32 nHeight, OdUInt32 nAlign,
    OdTrVisTexture::Format bufferFmt, OdUInt32 bufferWidth, OdUInt32 bufferHeight, OdUInt32 bufferAlign, void* buffer, Resampler method, OdTrVisTexture* pPalette = NULL );

  /** \details
  Creates pow2 textures from buffer
  */
  static OdTrVisTexturePtr createPow2TextureFrom( OdTrVisTexture::Format fmt, bool bRequirePow2, OdUInt32 nMaxSize, OdUInt32 nAlign,
    OdTrVisTexture::Format bufferFmt, OdUInt32 bufferWidth, OdUInt32 bufferHeight, OdUInt32 bufferAlign, void* buffer, Resampler method, OdTrVisTexture* pPalette = NULL );

  /** \details
  Creates texture from buffer with the same sizes, format and data alignment
  */
  static OdTrVisTexturePtr createTextureFrom( OdTrVisTexture::Format fmt, OdUInt32 nWidth, OdUInt32 nHeight, OdUInt32 nAlign, void* buffer, OdTrVisTexture* pPalette = NULL,
                                              bool bOwnExistBuffer = false );
  /** \details
  Saves texture in file
  */
  static bool save(OdTrVisTexture *pTexture, const OdString &fileName);

};


#include "TD_PackPop.h"

#endif
