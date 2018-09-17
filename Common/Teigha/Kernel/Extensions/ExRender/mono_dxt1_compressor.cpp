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

#include "OdaCommon.h"
#include "OdAlloc.h"
#include "mono_dxt1_compressor.h"

// @@@TODO: convert to plain C will increase code optimization and performance.

inline OdUInt16 dxt1RGBTo565(const OdUInt8 *pColor)
{
  return ((pColor[0] >> 3) << 11) |
         ((pColor[1] >> 2) << 5)  |
          (pColor[2] >> 3);
}

static void dxt1EmitRGBBlock(const OdUInt16 *pColors, const OdUInt8 *pSort, OdUInt16 monoBlockData, OdUInt8 *&pOutput)
{
  if (monoBlockData == 0)
  { // Block uses 1st color only
    OdUInt16 *pOutColors = (OdUInt16*)pOutput;
    pOutColors[0] = pColors[0];
    pOutColors[1] = pColors[0];
    *(OdUInt32*)(pOutput + 4) = 0;
  }
  else if (monoBlockData == 0xFFFF)
  { // Block uses 2nd color only
    OdUInt16 *pOutColors = (OdUInt16*)pOutput;
    pOutColors[0] = pColors[1];
    pOutColors[1] = pColors[1];
    *(OdUInt32*)(pOutput + 4) = 0;
  }
  else
  { // Block uses both colors
    OdUInt16 *pOutColors = (OdUInt16*)pOutput;
    OdUInt32 *pOutData = (OdUInt32*)(pOutput + 4);
    pOutColors[0] = pColors[pSort[0]];
    pOutColors[1] = pColors[pSort[1]];
    *pOutData = 0;
    for (OdUInt32 nBit = 0; nBit < 16; nBit++)
    {
      OdUInt32 nData = pSort[(monoBlockData >> (15 - nBit)) & 1];
      *pOutData |= nData << (nBit << 1);
    }
  }
  pOutput += 8;
}

static void dxt1EmitRGBABlock(const OdUInt16 *pColors, const OdUInt8 *pSort, OdUInt16 monoBlockData, OdUInt8 *&pOutput)
{
  OdUInt16 *pOutColors = (OdUInt16*)pOutput;
  pOutColors[0] = *pColors;
  pOutColors[1] = *pColors;
  if (monoBlockData == 0)
  { // Block uses 1st color only
    if (pSort[0] == 0)
      *(OdUInt32*)(pOutput + 4) = 0;
    else
      *(OdUInt32*)(pOutput + 4) = 0xFFFFFFFF;
  }
  else if (monoBlockData == 0xFFFF)
  { // Block uses 2nd color only
    if (pSort[1] == 0)
      *(OdUInt32*)(pOutput + 4) = 0;
    else
      *(OdUInt32*)(pOutput + 4) = 0xFFFFFFFF;
  }
  else
  { // Block uses both colors
    OdUInt32 *pOutData = (OdUInt32*)(pOutput + 4);
    *pOutData = 0;
    for (OdUInt32 nBit = 0; nBit < 16; nBit++)
    {
      OdUInt32 nData = pSort[(monoBlockData >> (15 - nBit)) & 1];
      *pOutData |= nData << (nBit << 1);
    }
  }
  pOutput += 8;
}

inline OdUInt32 dxt1MulDiv(OdUInt32 nVal, OdUInt32 nMult, OdUInt32 nDiv)
{
  // Compilers know how to optimize this sentence, so it will be faster than WinAPI MulDiv function.
  return OdUInt32(OdUInt64(nVal) * nMult / nDiv);
}

static void dxt1ComputeColorsRGB(const OdUInt8 *pColor1, const OdUInt8 *pColor2,
                                 OdUInt16 *inputColors, OdUInt8 *sortIndex)
{
  // Convert input colors
  inputColors[0] = dxt1RGBTo565(pColor1);
  inputColors[1] = dxt1RGBTo565(pColor2);
  // Sort colors
  if (inputColors[0] >= inputColors[1])
  {
    sortIndex[0] = 0;
    sortIndex[1] = 1;
  }
  else
  {
    sortIndex[0] = 1;
    sortIndex[1] = 0;
  }
}

static void dxt1ComputeColorsRGBA(const OdUInt8 *pColor1, const OdUInt8 *pColor2,
                                  OdUInt16 *inputColors, OdUInt8 *sortIndex)
{
  // Convert input colors and sort
  if (pColor1[3] > 128)
  { // First is opaque
    sortIndex[0] = 0;
    sortIndex[1] = 3;
    *inputColors = dxt1RGBTo565(pColor1);
  }
  else
  { // First is transparent
    sortIndex[0] = 3;
    sortIndex[1] = 0;
    *inputColors = dxt1RGBTo565(pColor2);
  }
}

static bool dxt1MonoCompressor(const OdUInt8 *pInput, OdUInt32 nInputWidth, OdUInt32 nInputHeight,
                               OdUInt32 nInputScanlineLength, const OdUInt8 *pColor1, const OdUInt8 *pColor2,
                               OdUInt8 *pOutput, OdUInt32 nOutputWidth, OdUInt32 nOutputHeight,
                               void (*_dxt1ComputeColors)(const OdUInt8 *pColor1, const OdUInt8 *pColor2, OdUInt16 *inputColors, OdUInt8 *sortIndex),
                               void (*_dxt1EmitBlock)(const OdUInt16 *pColors, const OdUInt8 *pSort, OdUInt16 monoBlockData, OdUInt8 *&pOutput))
{
  // Check input data for compatibility
  if (!pInput || !nInputWidth || !nInputHeight || !nInputScanlineLength || !pColor1 || !pColor2 ||
      !pOutput || !nOutputWidth || !nOutputHeight)
    return false;
  // Output width/height must be dividible on 4
  if ((nOutputWidth % 4 > 0) || (nOutputHeight % 4 > 0))
    return false;
  // Check does resampling required
  bool bResample = (nInputWidth != nOutputWidth) || (nInputHeight != nOutputHeight);
  // Convert input colors and compute sort indexes
  OdUInt16 inputColors[2];
  OdUInt8 sortIndex[2];
  _dxt1ComputeColors(pColor1, pColor2, inputColors, sortIndex);
  // Run loop for block-by-block compression
  OdUInt16 monoBlockData;
  OdUInt32 nXBlocks = nOutputWidth >> 2, nYBlocks = nOutputHeight >> 2;
  if (!bResample)
  { // Optimized loop if no resampling required
    const OdUInt8 *pScanlineBase = pInput;
    for (OdUInt32 nY = 0; nY < nYBlocks; nY++)
    {
      const OdUInt8 *pScanline1 = pScanlineBase;
      const OdUInt8 *pScanline2 = pScanline1 + nInputScanlineLength;
      const OdUInt8 *pScanline3 = pScanline2 + nInputScanlineLength;
      const OdUInt8 *pScanline4 = pScanline3 + nInputScanlineLength;
      for (OdUInt32 nX = 0; nX < nXBlocks; nX++)
      {
        if ((nX & 1) == 0)
        {
          monoBlockData = ((*pScanline1 & 0xF0) << 8) |
                          ((*pScanline2 & 0xF0) << 4) |
                          (*pScanline3 & 0xF0) |
                          ((*pScanline4 & 0xF0) >> 4);
        }
        else
        {
          monoBlockData = ((*pScanline1 & 0x0F) << 12) |
                          ((*pScanline2 & 0x0F) << 8) |
                          ((*pScanline3 & 0x0F) << 4) |
                          (*pScanline4 & 0x0F);
          pScanline1++; pScanline2++; pScanline3++; pScanline4++;
        }
        _dxt1EmitBlock(inputColors, sortIndex, monoBlockData, pOutput);
      }
      pScanlineBase += nInputScanlineLength << 2;
    }
  }
  else
  { // Resampled loop
    // Precompute x/y offsets
    OdUInt32 *pOffsetsX = (OdUInt32*)::odrxAlloc(sizeof(OdUInt32) * (nOutputWidth + nOutputHeight));
    OdUInt32 *pOffsetsY = pOffsetsX + nOutputWidth;
    if (nInputWidth == nOutputWidth)
    {
      for (OdUInt32 nX = 0; nX < nOutputWidth; nX++)
        pOffsetsX[nX] = nX;
    }
    else
    {
      for (OdUInt32 nX = 0; nX < nOutputWidth; nX++)
        pOffsetsX[nX] = dxt1MulDiv(nX, nInputWidth, nOutputWidth);
    }
    if (nInputHeight == nOutputHeight)
    {
      for (OdUInt32 nY = 0; nY < nOutputHeight; nY++)
        pOffsetsY[nY] = nY;
    }
    else
    {
      for (OdUInt32 nY = 0; nY < nOutputHeight; nY++)
        pOffsetsY[nY] = dxt1MulDiv(nY, nInputHeight, nOutputHeight);
    }
    // Run resampling/compression loop
    for (OdUInt32 nY = 0; nY < nYBlocks; nY++)
    {
      const OdUInt32 baseScanY = nY << 2;
      const OdUInt8 *pScanline1 = pInput + nInputScanlineLength * pOffsetsY[baseScanY];
      const OdUInt8 *pScanline2 = pInput + nInputScanlineLength * pOffsetsY[baseScanY + 1];
      const OdUInt8 *pScanline3 = pInput + nInputScanlineLength * pOffsetsY[baseScanY + 2];
      const OdUInt8 *pScanline4 = pInput + nInputScanlineLength * pOffsetsY[baseScanY + 3];
      for (OdUInt32 nX = 0; nX < nXBlocks; nX++)
      {
        const OdUInt32 baseScanX = nX << 2;
        monoBlockData = (((pScanline1[pOffsetsX[baseScanX] / 8] >> (0x07 - (pOffsetsX[baseScanX] % 8))) & 0x01) << 15) |
                        (((pScanline1[pOffsetsX[baseScanX + 1] / 8] >> (0x07 - (pOffsetsX[baseScanX + 1] % 8))) & 0x01) << 14) |
                        (((pScanline1[pOffsetsX[baseScanX + 2] / 8] >> (0x07 - (pOffsetsX[baseScanX + 2] % 8))) & 0x01) << 13) |
                        (((pScanline1[pOffsetsX[baseScanX + 3] / 8] >> (0x07 - (pOffsetsX[baseScanX + 3] % 8))) & 0x01) << 12) |
                        (((pScanline2[pOffsetsX[baseScanX] / 8] >> (0x07 - (pOffsetsX[baseScanX] % 8))) & 0x01) << 11) |
                        (((pScanline2[pOffsetsX[baseScanX + 1] / 8] >> (0x07 - (pOffsetsX[baseScanX + 1] % 8))) & 0x01) << 10) |
                        (((pScanline2[pOffsetsX[baseScanX + 2] / 8] >> (0x07 - (pOffsetsX[baseScanX + 2] % 8))) & 0x01) << 9) |
                        (((pScanline2[pOffsetsX[baseScanX + 3] / 8] >> (0x07 - (pOffsetsX[baseScanX + 3] % 8))) & 0x01) << 8) |
                        (((pScanline3[pOffsetsX[baseScanX] / 8] >> (0x07 - (pOffsetsX[baseScanX] % 8))) & 0x01) << 7) |
                        (((pScanline3[pOffsetsX[baseScanX + 1] / 8] >> (0x07 - (pOffsetsX[baseScanX + 1] % 8))) & 0x01) << 6) |
                        (((pScanline3[pOffsetsX[baseScanX + 2] / 8] >> (0x07 - (pOffsetsX[baseScanX + 2] % 8))) & 0x01) << 5) |
                        (((pScanline3[pOffsetsX[baseScanX + 3] / 8] >> (0x07 - (pOffsetsX[baseScanX + 3] % 8))) & 0x01) << 4) |
                        (((pScanline4[pOffsetsX[baseScanX] / 8] >> (0x07 - (pOffsetsX[baseScanX] % 8))) & 0x01) << 3) |
                        (((pScanline4[pOffsetsX[baseScanX + 1] / 8] >> (0x07 - (pOffsetsX[baseScanX + 1] % 8))) & 0x01) << 2) |
                        (((pScanline4[pOffsetsX[baseScanX + 2] / 8] >> (0x07 - (pOffsetsX[baseScanX + 2] % 8))) & 0x01) << 1) |
                         ((pScanline4[pOffsetsX[baseScanX + 3] / 8] >> (0x07 - (pOffsetsX[baseScanX + 3] % 8))) & 0x01);
        _dxt1EmitBlock(inputColors, sortIndex, monoBlockData, pOutput);
      }
    }
    // Free intermediate allocations
    ::odrxFree(pOffsetsX);
  }
  return true;
}

void odNormalizeDimensionsForDxt1Compression(OdUInt32 &nOutputWidth, OdUInt32 &nOutputHeight)
{
  if (nOutputWidth % 4 > 0)
  {
    if (nOutputWidth < 4) nOutputWidth = 4;
    else nOutputWidth -= nOutputWidth % 4;
  }
  if (nOutputHeight % 4 > 0)
  {
    if (nOutputHeight < 4) nOutputHeight = 4;
    else nOutputHeight -= nOutputHeight % 4;
  }
}

OdUInt32 odComputeDxt1CompressedBufferSize(OdUInt32 nOutputWidth, OdUInt32 nOutputHeight)
{
  return ((nOutputWidth >> 2) * (nOutputHeight >> 2)) << 3;
}

bool odMonoDxt1RGBCompress(const OdUInt8 *pInput, OdUInt32 nInputWidth, OdUInt32 nInputHeight,
                           OdUInt32 nInputScanlineLength, const OdUInt8 *pColor1, const OdUInt8 *pColor2,
                           OdUInt8 *pOutput, OdUInt32 nOutputWidth, OdUInt32 nOutputHeight)
{
  return dxt1MonoCompressor(pInput, nInputWidth, nInputHeight, nInputScanlineLength, pColor1, pColor2,
                            pOutput, nOutputWidth, nOutputHeight, dxt1ComputeColorsRGB, dxt1EmitRGBBlock);
}

bool odMonoDxt1RGBACompress(const OdUInt8 *pInput, OdUInt32 nInputWidth, OdUInt32 nInputHeight,
                            OdUInt32 nInputScanlineLength, const OdUInt8 *pColor1, const OdUInt8 *pColor2,
                            OdUInt8 *pOutput, OdUInt32 nOutputWidth, OdUInt32 nOutputHeight)
{
  return dxt1MonoCompressor(pInput, nInputWidth, nInputHeight, nInputScanlineLength, pColor1, pColor2,
                            pOutput, nOutputWidth, nOutputHeight, dxt1ComputeColorsRGBA, dxt1EmitRGBABlock);
}

//
