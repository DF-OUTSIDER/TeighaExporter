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

#include <iostream>
#include "OdPlatformStreamer.h"

namespace LODAlgorithm {

namespace Clustering {

void compress(const OdUInt32& nPoints, 
              const OdUInt8& level, 
              const OdGeExtents3d& ext, 
              OdStreamBuf& origBuf, 
              OdStreamBuf& targetBuf, 
              const bool isLog = false)
{
  OdUInt8 mask = 0x80;
  unsigned int byte = 0;
  OdUInt32 bytesCnt = 0;
  int comp = 0;

  double min[3] = {ext.minPoint().x, ext.minPoint().y, ext.minPoint().z};
  double max[3] = {ext.maxPoint().x, ext.maxPoint().y, ext.maxPoint().z};

  if (isLog) {
    printf("LOD Start compression\n");
    printf("EXT MIN: %f %f %f\nEXT MAX: %f %f %f\n", min[0], min[1], min[2], max[0], max[1], max[2]);
    printf("N points: %d Level: %d\n", nPoints, level);
  }

  OdUInt32 valCnt = nPoints * 3;
  while(valCnt--) {
    double value = OdPlatformStreamer::rdDouble(origBuf);

    OdUInt32 clstrValue = floor(((pow(2.0, level) - 1) / (max[comp] - min[comp])) * (value - min[comp]) + 0.5);

    //calc component axis
    comp = (comp >= 2) ? 0 : comp + 1;

    if (isLog)
      printf("v:%f n:%06d  ", value, clstrValue);

    for (int i = level - 1; i >= 0; --i) {
      unsigned int bit = clstrValue >> i & 1;
      byte = OdUInt8(bit != 0 ? (byte|mask) : (byte & (~mask)));

      //seek
      mask >>= 1;
      if(!mask)
      {
        mask = 0x80;
        targetBuf.putByte(byte);
        bytesCnt++;
        byte = 0;
      }
    }
  }

  if(byte != 0)
    targetBuf.putByte(byte);

  if (isLog)
    printf("End compression\nBytes written: %d\n", ++bytesCnt);
}

void decompress(const OdUInt32& nPoints, 
                const OdUInt8& level, 
                const OdGeExtents3d& ext, 
                OdStreamBuf& origBuf, 
                OdStreamBuf& targetBuf, 
                const bool isLog = false) 
{
  double min[3] = {ext.minPoint().x, ext.minPoint().y, ext.minPoint().z};
  double max[3] = {ext.maxPoint().x, ext.maxPoint().y, ext.maxPoint().z};

  if (isLog) {
    printf("LOD Start decompression\n");
    printf("EXT MIN: %f %f %f\nEXT MAX: %f %f %f\n", min[0], min[1], min[2], max[0], max[1], max[2]);
    printf("N points: %d Level: %d\n", nPoints, level);
  }

  unsigned int byte = origBuf.getByte();
  OdUInt8 mask = 0x80;
  OdUInt32 value = 0;
  OdUInt8 comp = 0;
  OdUInt8 bitShift = 7;
  OdUInt32 bytesCnt = 1;

  for (OdUInt32 i = 0; i < nPoints * 3; ++i) {

    for (int j = level - 1; j >= 0; --j) {
      unsigned int val = byte >> bitShift & 1;
      value += val << j;
      mask >>=1;
      bitShift--;

      if(!mask) {
        mask = 0x80;
        bitShift = 7;

        if (i == nPoints * 3 - 1 && j == 0)
          continue;

        byte = origBuf.getByte();
        bytesCnt++;
      }
    }

    float newVal = ((max[comp] - min[comp]) / pow(2.0, level)) * value + min[comp];
    if (isLog)
      printf("v:%f n:%d \t", newVal, value);

    value = 0;
    comp = (comp >= 2) ? 0 : comp + 1;
    OdPlatformStreamer::wrFloat(targetBuf, newVal);
  }

  if (isLog)
    printf("End compression\nBytes readed: %d\n", bytesCnt);
}

}
}
