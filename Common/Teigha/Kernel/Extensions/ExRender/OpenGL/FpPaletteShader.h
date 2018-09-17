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
static const char *g_fp10_PaletteShader_main =
  "!!ARBfp1.0" "\n"
  "# cgc version 3.1.0013, build date Apr 18 2012" "\n"
  "# command line args: -profile arbfp1" "\n"
  "# source file: PaletteShader.cg" "\n"
  "#vendor NVIDIA Corporation" "\n"
  "#version 3.1.0.13" "\n"
  "#profile arbfp1" "\n"
  "#program main" "\n"
  "#semantic main.image : TEXUNIT0" "\n"
  "#semantic main.palette : TEXUNIT1" "\n"
  "#semantic main.fogColor : state.fog.color" "\n"
  "#semantic main.params" "\n"
  "#var float4 faceColor : $vin.COLOR0 : COL0 : 0 : 1" "\n"
  "#var float2 texCoords : $vin.TEXCOORD0 : TEX0 : 1 : 1" "\n"
  "#var sampler2D image : TEXUNIT0 : texunit 0 : 2 : 1" "\n"
  "#var sampler2D palette : TEXUNIT1 : texunit 1 : 3 : 1" "\n"
  "#var float4 fogColor : state.fog.color : c[0] : 4 : 1" "\n"
  "#var float4 params :  : c[1] : 5 : 1" "\n"
  "#var float4 main : $vout.COLOR : COL : -1 : 1" "\n"
  "#const c[2] = -1 0.5 255 1" "\n"
  "PARAM c[3] = { state.fog.color," "\n"
  "               program.local[1]," "\n"
  "            { -1, 0.5, 255, 1 } };" "\n"
  "TEMP R0;" "\n"
  "TEMP R1;" "\n"
  "MOV R0.y, c[2].x;" "\n"
  "ADD R0.z, R0.y, c[1].x;" "\n"
  "RCP R0.y, c[1].x;" "\n"
  "RCP R0.w, R0.z;" "\n"
  "ADD R0.z, -R0.y, c[2].w;" "\n"
  "MUL R0.z, R0, R0.w;" "\n"
  "TEX R0.x, fragment.texcoord[0], texture[0], 2D;" "\n"
  "MUL R0.x, R0.z, R0;" "\n"
  "MUL R0.z, R0.y, c[2].y;" "\n"
  "MOV R0.y, c[2];" "\n"
  "MAD R0.x, R0, c[2].z, R0.z;" "\n"
  "TEX R0, R0, texture[1], 2D;" "\n"
  "ADD R1.xyz, -R0, c[0];" "\n"
  "MAD result.color.xyz, R1, c[1].z, R0;" "\n"
  "MUL result.color.w, R0, fragment.color.primary;" "\n"
  "END" "\n"
  "# 15 instructions, 2 R-regs" "\n";
