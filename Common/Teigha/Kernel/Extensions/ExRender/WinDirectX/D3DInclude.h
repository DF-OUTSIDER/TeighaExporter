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

// Includes Direct3D for required version.

#ifndef __OD_EX_DIRECT_3D_INCLUDE__
#define __OD_EX_DIRECT_3D_INCLUDE__

#ifdef WIN64
#define OD_USE_DIRECT3D9 // Only D3D 9 useable for Win64
#endif

#define OD_USE_DIRECT3D9

#ifndef OD_USE_DIRECT3D9
// DXSDK path is in environment variable DXSDK_DIR
#include "include/d3d8.h" // Use old 8.1 API
#if defined(__BORLANDC__)
#include <math.h>
inline float sqrtf(float x)
{
  return (float)sqrt((double)x);
}
#endif
// DXSDK path is in environment variable DXSDK_DIR
#include "include/d3dx8math.h"
//
typedef IDirect3D8 IDirect3D;
typedef IDirect3DDevice8 IDirect3DDevice;
typedef IDirect3DSurface8 IDirect3DSurface;
typedef IDirect3DBaseTexture8 IDirect3DBaseTexture;
typedef IDirect3DTexture8 IDirect3DTexture;
typedef D3DMATERIAL8 D3DMATERIAL;
typedef D3DLIGHT8 D3DLIGHT;
typedef D3DCAPS8 D3DCAPS;
#else
#include "include/d3d9.h"
#include "include/d3dx9math.h"
//
typedef IDirect3D9 IDirect3D;
typedef IDirect3DDevice9 IDirect3DDevice;
typedef IDirect3DSurface9 IDirect3DSurface;
typedef IDirect3DBaseTexture9 IDirect3DBaseTexture;
typedef IDirect3DTexture9 IDirect3DTexture;
typedef D3DMATERIAL9 D3DMATERIAL;
typedef D3DLIGHT9 D3DLIGHT;
typedef D3DCAPS9 D3DCAPS;
//
#define GetBackBuffer(p1, p2, p3) GetBackBuffer(0, p1, p2, p3)
#define CreateTexture(p1, p2, p3, p4, p5, p6, p7) CreateTexture(p1, p2, p3, p4, p5, p6, p7, NULL)
#endif

#endif // __OD_EX_DIRECT_3D_INCLUDE__

//
