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

#ifndef ODMONODXT1COMPRESSOR_INCLUDED
#define ODMONODXT1COMPRESSOR_INCLUDED

#include "TD_PackPush.h"

/** \details
  Normalizes image dimensions for prepare to Dxt1 compression.
  \param nOutputWidth [in,out]  Image width for normalization.
  \param nOutputHeight [in,out]  Image height for normalization.
*/
void odNormalizeDimensionsForDxt1Compression(OdUInt32 &nOutputWidth, OdUInt32 &nOutputHeight);

/** \details
  Compute buffer size for Dxt1 compressed output.
  \param nOutputWidth [in]  Image width.
  \param nOutputHeight [in]  Image height.
*/
OdUInt32 odComputeDxt1CompressedBufferSize(OdUInt32 nOutputWidth, OdUInt32 nOutputHeight);

/** \details
  Effectively compress input monochrome (1bpp) image into Dxt1 texture format.
  \remarks
  Output width and height must be dividible on 4x4 blocks.
  \param pInput [in]  Input binary data (as 1bpp BMP image).
  \param nInputWidth [in]  Width of input image.
  \param nInputHeight [in]  Height of input image.
  \param nInputScanlineLength [in]  Scanline length of input image including alignment bytes.
  \param pColor1 [in]  First color for monochrome image (index 0).
  \param pColor2 [in]  Second color for monochrome image (index 1).
  \param pOutput [in]  Preallocated output buffer.
  \param nOutputWidth [in]  Width of output image.
  \param nOutputHeight [in]  Height of output image.
*/
bool odMonoDxt1RGBCompress(const OdUInt8 *pInput, OdUInt32 nInputWidth, OdUInt32 nInputHeight,
                           OdUInt32 nInputScanlineLength, const OdUInt8 *pColor1, const OdUInt8 *pColor2,
                           OdUInt8 *pOutput, OdUInt32 nOutputWidth, OdUInt32 nOutputHeight);

/** \details
  Effectively compress input monochrome (1bpp) image into Dxt1 texture format include premultiplied alpha transparency.
  \remarks
  Output width and height must be dividible on 4x4 blocks.
  \param pInput [in]  Input binary data (as 1bpp BMP image).
  \param nInputWidth [in]  Width of input image.
  \param nInputHeight [in]  Height of input image.
  \param nInputScanlineLength [in]  Scanline length of input image including alignment bytes.
  \param pColor1 [in]  First color for monochrome image (index 0).
  \param pColor2 [in]  Second color for monochrome image (index 1).
  \param pOutput [in]  Preallocated output buffer.
  \param nOutputWidth [in]  Width of output image.
  \param nOutputHeight [in]  Height of output image.
*/
bool odMonoDxt1RGBACompress(const OdUInt8 *pInput, OdUInt32 nInputWidth, OdUInt32 nInputHeight,
                            OdUInt32 nInputScanlineLength, const OdUInt8 *pColor1, const OdUInt8 *pColor2,
                            OdUInt8 *pOutput, OdUInt32 nOutputWidth, OdUInt32 nOutputHeight);

#include "TD_PackPop.h"

#endif // ODMONODXT1COMPRESSOR_INCLUDED
