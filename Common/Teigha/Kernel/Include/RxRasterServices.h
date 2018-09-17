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

// RxRasterServices.h - interface of module, performing different operations on raster images

#ifndef __OD_RASTER_MODULE__
#define __OD_RASTER_MODULE__

#include "RxModule.h"
#include "DbRootExport.h"
#include "UInt32Array.h"
#include "TDVersion.h"
class OdGiRasterImage;
typedef OdSmartPtr<OdGiRasterImage> OdGiRasterImagePtr;

#include "TD_PackPush.h"

//////////////////
#ifndef lit_
#define lit_  0x20 // ' '
#define lit_0 0x30 // '0'
#define lit_1 0x31 // '1'
#define lit_2 0x32 // '2'
#define lit_3 0x33 // '3'
#define lit_4 0x34 // '4'
#define lit_5 0x35 // '5'
#define lit_6 0x36 // '6'
#define lit_7 0x37 // '7'
#define lit_8 0x38 // '8'
#define lit_9 0x39 // '9'
#define lit_A 0x41 // 'A'
#define lit_B 0x42 // 'B'
#define lit_C 0x43 // 'C'
#define lit_D 0x44 // 'D'
#define lit_E 0x45 // 'E'
#define lit_F 0x46 // 'F'
#define lit_G 0x47 // 'G'
#define lit_H 0x48 // 'H'
#define lit_I 0x49 // 'I'
#define lit_J 0x4A // 'J'
#define lit_K 0x4B // 'K'
#define lit_L 0x4C // 'L'
#define lit_M 0x4D // 'M'
#define lit_N 0x4E // 'N'
#define lit_O 0x4F // 'O'
#define lit_P 0x50 // 'P'
#define lit_Q 0x51 // 'Q'
#define lit_R 0x52 // 'R'
#define lit_S 0x53 // 'S'
#define lit_T 0x54 // 'T'
#define lit_U 0x55 // 'U'
#define lit_V 0x56 // 'V'
#define lit_W 0x57 // 'W'
#define lit_X 0x58 // 'X'
#define lit_Y 0x59 // 'Y'
#define lit_Z 0X5A // 'Z'
#define lit_a 0x61 // 'a'
#define lit_b 0x62 // 'b'
#define lit_c 0x63 // 'c'
#define lit_d 0x64 // 'd'
#define lit_e 0x65 // 'e'
#define lit_f 0x66 // 'f'
#define lit_g 0x67 // 'g'
#define lit_h 0x68 // 'h'
#define lit_i 0x69 // 'i'
#define lit_j 0x6A // 'j'
#define lit_k 0x6B // 'k'
#define lit_l 0x6C // 'l'
#define lit_m 0x6D // 'm'
#define lit_n 0x6E // 'n'
#define lit_o 0x6F // 'o'
#define lit_p 0x70 // 'p'
#define lit_q 0x71 // 'q'
#define lit_r 0x72 // 'r'
#define lit_s 0x73 // 's'
#define lit_t 0x74 // 't'
#define lit_u 0x75 // 'u'
#define lit_v 0x76 // 'v'
#define lit_w 0x77 // 'w'
#define lit_x 0x78 // 'x'
#define lit_y 0x79 // 'y'
#define lit_z 0x7A // 'z'
#endif // lit_
//////////////////

#ifndef OD_FOURCC
#ifndef ODA_BIGENDIAN
#if defined(OD_SWIGCSHARP) || defined(OD_SWIGJAVA)
#define OD_FOURCC(a, b, c, d) (((long)(d) << 24) | ((long)(c) << 16) | ((long)(b) << 8) | ((long)(a)))
#else
 #define OD_FOURCC(a, b, c, d) (((OdUInt32)(d) << 24) | ((OdUInt32)(c) << 16) | ((OdUInt32)(b) << 8) | ((OdUInt32)(a)))
#endif
#else
 #define OD_FOURCC(a, b, c, d) (((OdUInt32)(a) << 24) | ((OdUInt32)(b) << 16) | ((OdUInt32)(c) << 8) | ((OdUInt32)(d)))
#endif
#endif // OD_FOURCC

/** \details
    This class is the base class for classes that provide platform-dependent loading and saving
    of Raster Image files for Teigha.
    Library: TD_Db
    <group OdRx_Classes> 
*/
class ODRX_ABSTRACT FIRSTDLL_EXPORT OdRxRasterServices : public OdRxModule
{
public:
  // Predefined image types
  enum ImageType
  {
    kUnknown  = -1,
    kBMP      = OD_FOURCC(lit_B, lit_M, lit_P, lit_ ),
    kICO      = OD_FOURCC(lit_I, lit_C, lit_O, lit_ ),
    kJPEG     = OD_FOURCC(lit_J, lit_P, lit_E, lit_G),
    kJNG      = OD_FOURCC(lit_J, lit_N, lit_G, lit_ ),
    kKOALA    = OD_FOURCC(lit_K, lit_O, lit_A, lit_L),
    kLBM      = OD_FOURCC(lit_L, lit_B, lit_M, lit_ ),
    kIFF      = kLBM,
    kMNG      = OD_FOURCC(lit_M, lit_N, lit_G, lit_ ),
    kPBM      = OD_FOURCC(lit_P, lit_B, lit_M, lit_ ),
    kPBMRAW   = OD_FOURCC(lit_P, lit_B, lit_M, lit_R),
    kPCD      = OD_FOURCC(lit_P, lit_C, lit_D, lit_ ),
    kPCX      = OD_FOURCC(lit_P, lit_C, lit_X, lit_ ),
    kPGM      = OD_FOURCC(lit_P, lit_G, lit_M, lit_ ),
    kPGMRAW   = OD_FOURCC(lit_P, lit_G, lit_M, lit_R),
    kPNG      = OD_FOURCC(lit_P, lit_N, lit_G, lit_ ),
    kPPM      = OD_FOURCC(lit_P, lit_P, lit_M, lit_ ),
    kPPMRAW   = OD_FOURCC(lit_P, lit_P, lit_M, lit_R),
    kRAS      = OD_FOURCC(lit_R, lit_A, lit_S, lit_ ),
    kTARGA    = OD_FOURCC(lit_T, lit_G, lit_A, lit_ ),
    kTIFF     = OD_FOURCC(lit_T, lit_I, lit_F, lit_F),
    kWBMP     = OD_FOURCC(lit_W, lit_B, lit_M, lit_P),
    kPSD      = OD_FOURCC(lit_P, lit_S, lit_D, lit_ ),
    kCUT      = OD_FOURCC(lit_C, lit_U, lit_T, lit_ ),
    kXBM      = OD_FOURCC(lit_X, lit_B, lit_M, lit_ ),
    kXPM      = OD_FOURCC(lit_X, lit_P, lit_M, lit_ ),
    kDDS      = OD_FOURCC(lit_D, lit_D, lit_S, lit_ ),
    kGIF      = OD_FOURCC(lit_G, lit_I, lit_F, lit_ ),
    kHDR      = OD_FOURCC(lit_H, lit_D, lit_R, lit_ ),
    kFAXG3    = OD_FOURCC(lit_F, lit_X, lit_G, lit_3),
    kSGI      = OD_FOURCC(lit_S, lit_G, lit_I, lit_ ),
    kEXR      = OD_FOURCC(lit_E, lit_X, lit_R, lit_ ),
    kJ2K      = OD_FOURCC(lit_J, lit_2, lit_K, lit_ ),
    kJP2      = OD_FOURCC(lit_J, lit_P, lit_2, lit_ ),
    kPFM      = OD_FOURCC(lit_P, lit_F, lit_M, lit_ ),
    kPICT     = OD_FOURCC(lit_P, lit_I, lit_C, lit_T),
    kRAW      = OD_FOURCC(lit_R, lit_A, lit_W, lit_ )
  }; 

  // Loading flags
  enum LoadFlags
  {
    // Specify loading format explicitly
    kLoadFmt        = OD_FOURCC(lit_F, lit_M, lit_T, lit_ ),
    // Avoids post-reorientation of TIFF format images
    kNoTIFFRotation = OD_FOURCC(lit_N, lit_T, lit_F, lit_R)
  };

  // Saving flags
  enum SaveFlags
  {
    // Specify palette index of transparent color (-1 - by default)
    kTransparentColor = OD_FOURCC(lit_T, lit_C, lit_L, lit_R),

    // Jpeg compression quality (default depends from implementation)
    kJpegQuality      = OD_FOURCC(lit_J, lit_Q, lit_T, lit_Y),

    // Tiff compression format (no compression by default)
    kTiffCompression          = OD_FOURCC(lit_T, lit_C, lit_M, lit_P),
    kTiffCompressionDeflate   = OD_FOURCC(lit_Z, lit_I, lit_P, lit_ ),
    kTiffCompressionLzw       = OD_FOURCC(lit_L, lit_Z, lit_W, lit_ ),
    kTiffCompressionJpeg      = OD_FOURCC(lit_J, lit_P, lit_E, lit_G),
    kTiffCompressionCCITTFax3 = OD_FOURCC(lit_F, lit_A, lit_X, lit_3),
    kTiffCompressionCCITTFax4 = OD_FOURCC(lit_F, lit_A, lit_X, lit_4),
    kTiffCompressionEmbedded  = OD_FOURCC(lit_E, lit_M, lit_B, lit_D), // ODA extension for IbEnabler, actually it is CCITFax4 compression

    //dithering flag
    kDithering             = OD_FOURCC(lit_D, lit_I, lit_T, lit_H),
    kDitheringFS           = OD_FOURCC(lit_D, lit_T, lit_F, lit_S), // Floyd & Steinberg error diffusion
    kDitheringBayer4x4     = OD_FOURCC(lit_B, lit_Y, lit_R, lit_4), // Bayer ordered dispersed dot dithering (order 2 dithering matrix)
    kDitheringBayer8x8     = OD_FOURCC(lit_B, lit_Y, lit_R, lit_8), // Bayer ordered dispersed dot dithering (order 3 dithering matrix)
    kDitheringBayer16x16   = OD_FOURCC(lit_B, lit_R, lit_1, lit_6), // Bayer ordered dispersed dot dithering (order 4 dithering matrix)
    kDitheringCluster6x6   = OD_FOURCC(lit_C, lit_L, lit_R, lit_6), // Ordered clustered dot dithering (order 3 - 6x6 matrix)
    kDitheringCluster8x8   = OD_FOURCC(lit_C, lit_L, lit_R, lit_8), // Ordered clustered dot dithering (order 4 - 8x8 matrix)
    kDitheringCluster16x16 = OD_FOURCC(lit_C, lit_L, lit_1, lit_6), // Ordered clustered dot dithering (order 8 - 16x16 matrix)

    //rescale flags
    kRescale           = OD_FOURCC(lit_R, lit_S, lit_C, lit_L),
    kRescaleBox        = OD_FOURCC(lit_B, lit_O, lit_X, lit_ ),     // Box, pulse, Fourier window, 1st order (constant) b-spline
    kRescaleBicubic    = OD_FOURCC(lit_B, lit_C, lit_B, lit_C),     // Mitchell & Netravali's two-param cubic filter
    kRescaleBilinear   = OD_FOURCC(lit_B, lit_L, lit_N, lit_R),     // Bilinear filter
    kRescaleBspline    = OD_FOURCC(lit_B, lit_S, lit_P, lit_L),     // 4th order (cubic) b-spline
    kRescaleCatmullrom = OD_FOURCC(lit_C, lit_T, lit_M, lit_L),     // Catmull-Rom spline, Overhauser spline
    kRescaleLanczos3   = OD_FOURCC(lit_L, lit_N, lit_C, lit_Z),     // Lanczos3 filter
    kRescaleWidth      = OD_FOURCC(lit_W, lit_D, lit_T, lit_H),
    kRescaleHeight     = OD_FOURCC(lit_H, lit_G, lit_H, lit_T),
    //Note: rescaling converts bitional images to 8-bit images, so to keep the image bitional after rescale, the dithering flags must be in flag chain.

    // color quantization algorithm flag (is usable for FIF_GIF with colorDepth() > 8)
    kQuantizeNone = OD_FOURCC(lit_C, lit_T, lit_O, lit_8),
    kQuantizeWU   = OD_FOURCC(lit_W, lit_U, lit_C, lit_Q), // Xiaolin Wu color quantization algorithm (is default)
    kQuantizeNN   = OD_FOURCC(lit_N, lit_N, lit_Q, lit_ ), // NeuQuant neural-net quantization algorithm by Anthony Dekker
    kQuantizeLFP  = OD_FOURCC(lit_L, lit_F, lit_P, lit_Q)  // Lossless Fast Pseudo-Quantization Algorithm by Carsten Klein
  };

  ODRX_DECLARE_MEMBERS(OdRxRasterServices);

  /** \details
      Loads the specified Raster Image file.
      \param filename [in]  Filename of the Raster Image file to be read.
      \param pFlagsChain [in]  Optional zero-terminated loading flag pairs array.

      \remarks
      The returned pointer is expected to be passed to OdGiViewportGeometry::rasterImageDc().
  */
  virtual OdGiRasterImagePtr loadRasterImage(const OdString &filename, const OdUInt32 *pFlagsChain = NULL) = 0;

  /** \details
      Loads the specified Raster Image file.

      \param pStreamBuf [in]  Pointer to the StreamBuf object from which the data are to be read.
      \param pFlagsChain [in]  Optional zero-terminated loading flag pairs array.

      \remarks
      The returned pointer is expected to be passed to OdGiViewportGeometry::rasterImageDc().
  */
  virtual OdGiRasterImagePtr loadRasterImage(OdStreamBuf *pStreamBuf, const OdUInt32 *pFlagsChain = NULL) = 0;

  /** \details
      Create raster image using user-defined image implementation.
      \param pImp [in]  User-defined raster image implementation.

      \remarks
      For default implementation always return null.
  */
  virtual OdGiRasterImagePtr createRasterImage(void *pImp);

  /** \details
      Saves specified Raster Image to the specified file.
      \param rasterImage [in]  Raster image to be saved.
      \param filename [in]  Filename of the Raster Image file to be written.
      \param pFlagsChain [in]  Optional zero-terminated saving flag pairs array.
  */
  virtual bool saveRasterImage(const OdGiRasterImage* rasterImage, const OdString& filename,
                               const OdUInt32 *pFlagsChain = NULL) = 0;
                               
  /** \details
      Saves specified Raster Image to the specified file.
      \param rasterImage [in]  Raster image to be saved.
      \param filename [in]  Filename of the Raster Image file to be written.
      \param type [in]  Image format type to be written.
      \param pFlagsChain [in]  Optional zero-terminated saving flag pairs array.
  */
  virtual bool saveRasterImage(const OdGiRasterImage* rasterImage, const OdString& filename,
                               OdUInt32 type, const OdUInt32 *pFlagsChain = NULL) = 0;

  /** \details
      Try to convert raster image (RGB) to JPEG or other type.
      \param pRaster [in]  Raster image to be converted.
      \param type [in]  Image format type to be converted.
      \param pStreamBuf [in]  Pointer to the StreamBuf object to which the data are to be stored.
      \param pFlagsChain [in]  Optional zero-terminated saving flag pairs array.
  */
  virtual bool convertRasterImage(const OdGiRasterImage* pRaster, OdUInt32 type,
                                  OdStreamBuf* pStreamBuf, const OdUInt32 *pFlagsChain = NULL) = 0;

  /** \details
      Try to convert raster image to other type.
      \param pSrcStream [in]  Pointer to the StreamBuf object from which the data are to be converted.
      \param pDstStream [in]  Pointer to the StreamBuf object to which the data are to be converted.
      \param type [in]  Image format type to be converted.
      \param pFlagsChainSrc [in]  Optional zero-terminated loading flag pairs array.
      \param pFlagsChainDst [in]  Optional zero-terminated saving flag pairs array.
  */
  virtual bool convertRasterImage(OdStreamBuf* pSrcStream, OdStreamBuf* pDstStream, OdUInt32 type,
                                  const OdUInt32 *pFlagsChainSrc = NULL, const OdUInt32 *pFlagsChainDst = NULL) = 0;

  /** \details
      Returns array of supported image format types.
  */
  virtual OdUInt32Array getRasterImageTypes() const = 0;

  /** \details
      Checks does image format type is supported.
      \param type [in]  Image format type to be checked.
  */
  virtual bool isRasterImageTypeSupported(OdUInt32 type) const;

  /** \details
      Get file extension and filter name by type.
      \param type [in]  Image format type to be formatted.
      \param psFilterName [out]  Output filter name (can be Null).
  */
  virtual OdString mapTypeToExtension(OdUInt32 type, OdString* psFilterName) const = 0;

  /** \details
      Get image format type by file extension.
      \param extension [in]  File extension.
  */
  virtual OdUInt32 mapExtensionToType(const OdString& extension) const = 0;

  /** \details
    Try to detect image format type from input stream.
    \param filename [in]  Filename of the Raster Image file to be checked.
  */
  virtual OdUInt32 getImageFormat(const OdString &filename) const = 0;
  
  /** \details
    Try to detect image format type from input stream.
    \param pStreamBuf [in]  Pointer to the StreamBuf object from which the data are to be checked.
  */
  virtual OdUInt32 getImageFormat(OdStreamBuf* pStreamBuf) const = 0;
};

/** \details
  This template class is a specialization of the OdSmartPtr class for OdRxRasterServices object pointers.
*/
typedef OdSmartPtr<OdRxRasterServices> OdRxRasterServicesPtr;

//////////////////
#ifdef lit_
#undef lit_  // ' '
#undef lit_0 // '0'
#undef lit_1 // '1'
#undef lit_2 // '2'
#undef lit_3 // '3'
#undef lit_4 // '4'
#undef lit_5 // '5'
#undef lit_6 // '6'
#undef lit_7 // '7'
#undef lit_8 // '8'
#undef lit_9 // '9'
#undef lit_A // 'A'
#undef lit_B // 'B'
#undef lit_C // 'C'
#undef lit_D // 'D'
#undef lit_E // 'E'
#undef lit_F // 'F'
#undef lit_G // 'G'
#undef lit_H // 'H'
#undef lit_I // 'I'
#undef lit_J // 'J'
#undef lit_K // 'K'
#undef lit_L // 'L'
#undef lit_M // 'M'
#undef lit_N // 'N'
#undef lit_O // 'O'
#undef lit_P // 'P'
#undef lit_Q // 'Q'
#undef lit_R // 'R'
#undef lit_S // 'S'
#undef lit_T // 'T'
#undef lit_U // 'U'
#undef lit_V // 'V'
#undef lit_W // 'W'
#undef lit_X // 'X'
#undef lit_Y // 'Y'
#undef lit_Z // 'Z'
#undef lit_a // 'a'
#undef lit_b // 'b'
#undef lit_c // 'c'
#undef lit_d // 'd'
#undef lit_e // 'e'
#undef lit_f // 'f'
#undef lit_g // 'g'
#undef lit_h // 'h'
#undef lit_i // 'i'
#undef lit_j // 'j'
#undef lit_k // 'k'
#undef lit_l // 'l'
#undef lit_m // 'm'
#undef lit_n // 'n'
#undef lit_o // 'o'
#undef lit_p // 'p'
#undef lit_q // 'q'
#undef lit_r // 'r'
#undef lit_s // 's'
#undef lit_t // 't'
#undef lit_u // 'u'
#undef lit_v // 'v'
#undef lit_w // 'w'
#undef lit_x // 'x'
#undef lit_y // 'y'
#undef lit_z // 'z'
#endif // lit_
//////////////////

#include "TD_PackPop.h"

#endif // __OD_RASTER_MODULE__
