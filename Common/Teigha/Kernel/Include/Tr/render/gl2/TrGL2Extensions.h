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
// GLES2 extensions

#ifndef ODTRGL2EXTENSIONS
#define ODTRGL2EXTENSIONS

#include "RxObject.h"

// BGR/BGRA textures (kExtensionBGR/kExtensionBGRA)
#ifndef GL_BGR_EXT
#define GL_BGR_EXT                        0x80E0
#endif // GL_BGR_EXT
#ifndef GL_BGRA_EXT
#define GL_BGRA_EXT                       0x80E1
#endif // GL_BGRA_EXT

// ClampToEdge wrap mode (kExtensionTextureEdgeClamp)
#ifndef GL_CLAMP
#define GL_CLAMP                          0x2900
#endif // GL_CLAMP

// ClampToBorder wrap mode (kExtensionTextureBorderClamp)
#ifndef GL_CLAMP_TO_BORDER
#define GL_CLAMP_TO_BORDER                0x812D
#endif // GL_CLAMP_TO_BORDER
#ifndef GL_TEXTURE_BORDER_COLOR
#define GL_TEXTURE_BORDER_COLOR           0x1004
#endif // GL_TEXTURE_BORDER_COLOR

// Depth textures (kExtensionDepthTexture)
#ifndef GL_DEPTH_COMPONENT
#define GL_DEPTH_COMPONENT                0x1902
#endif // GL_DEPTH_COMPONENT

// 24-bit depth buffer format (kExtensionDepth24)
#ifndef GL_DEPTH_COMPONENT24
#define GL_DEPTH_COMPONENT24              0x81A6
#endif // GL_DEPTH_COMPONENT24

// Packed depth-stencil format (kExtensionPackedDepthStencil)
#ifndef GL_DEPTH_STENCIL
#define GL_DEPTH_STENCIL                  0x84F9
#endif // GL_DEPTH_STENCIL
#ifndef GL_UNSIGNED_INT_24_8
#define GL_UNSIGNED_INT_24_8              0x84FA
#endif // GL_UNSIGNED_INT_24_8
#ifndef GL_DEPTH24_STENCIL8
#define GL_DEPTH24_STENCIL8               0x88F0
#endif // GL_DEPTH24_STENCIL8
// WebGL case
#ifndef GL_DEPTH_STENCIL_ATTACHMENT
#define GL_DEPTH_STENCIL_ATTACHMENT       0x821A
#endif // GL_DEPTH_STENCIL_ATTACHMENT

// GL_STENCIL_INDEX8 render buffer storage (kExtensionStencil8)
#ifndef GL_STENCIL_INDEX
#define GL_STENCIL_INDEX                  0x1901
#endif // GL_STENCIL_INDEX
#ifndef GL_STENCIL_INDEX8
#define GL_STENCIL_INDEX8                 0x8D48
#endif // GL_STENCIL_INDEX8

// Half-float textures support (kExtensionTextureHalfFloat)
#ifndef GL_RGBA16F
#define GL_RGBA16F                        0x881A
#endif // GL_RGBA16F
//#ifndef GL_ALPHA16F
//#define GL_ALPHA16F                       0x881C
//#endif // GL_ALPHA16F
// This definitions is only for desktop OpenGL. Native OpenGL ES should use GL_RGBA instead and GL_HALF_FLOAT will be accepted as texture type.
#ifndef GL_HALF_FLOAT
#define GL_HALF_FLOAT                     0x8D61
#endif // GL_HALF_FLOAT

// Floating point textures support (kExtensionTextureFloat)
#ifndef GL_RGBA32F
#define GL_RGBA32F                        0x8814
#endif // GL_RGBA32F
//#ifndef GL_ALPHA32F
//#define GL_ALPHA32F                       0x8816
//#endif // GL_ALPHA32F
// This definitions is only for desktop OpenGL. Native OpenGL ES should use GL_RGBA instead and GL_FLOAT will be accepted as texture type.

// Multiple Render Targets support (kExtensionDrawBuffers)
#ifndef GL_DRAW_BUFFER0
#define GL_DRAW_BUFFER0                   0x8825
#endif // GL_DRAW_BUFFER0

// Lines anti-aliasing
#ifndef GL_LINE_SMOOTH
#define GL_LINE_SMOOTH                    0x0B20
#endif // GL_LINE_SMOOTH
#ifndef GL_LINE_SMOOTH_HINT
#define GL_LINE_SMOOTH_HINT               0x0C52
#endif // GL_LINE_SMOOTH_HINT
#ifndef GL_SMOOTH_LINE_WIDTH_RANGE
#define GL_SMOOTH_LINE_WIDTH_RANGE        0x0B22
#endif // GL_SMOOTH_LINE_WIDTH_RANGE

// Shading language version
#ifndef GL_SHADING_LANGUAGE_VERSION
#define GL_SHADING_LANGUAGE_VERSION       0x8B8C
#endif // GL_SHADING_LANGUAGE_VERSION

// Required for some desktop GPU's which invoke compatible OpenGL profiles (not required since OpenGL 3.2)
#ifndef GL_POINT_SPRITE
#define GL_POINT_SPRITE                   0x8861
#endif // GL_POINT_SPRITE
// OpenGL ES 2 GLSL point size (not required by native, but necessary for PC and WebGL)
#ifndef GL_VERTEX_PROGRAM_POINT_SIZE
#define GL_VERTEX_PROGRAM_POINT_SIZE      0x8642
#endif // GL_VERTEX_PROGRAM_POINT_SIZE

// RGBA framebuffers
// Note: don't think that we must check availability for this format. It is defined with GL_EXT_texture extension for desktop OpenGL, but
//       this format will be available anyway if frame buffers supported. It is defined with GL_OES_rgb8_rgba8 extension for OpemGL ES, but
//       it is in any case is one of the native OpenGL formats, so don't think that it will be unavailable anywhere.
#ifndef GL_RGBA8
#define GL_RGBA8                          0x8058
#endif // GL_RGBA8
//

// Geometry shader extension
#ifndef GL_GEOMETRY_SHADER
#define GL_GEOMETRY_SHADER                0x8DD9
#endif // GL_GEOMETRY_SHADER
#ifndef GL_GEOMETRY_VERTICES_OUT
#define GL_GEOMETRY_VERTICES_OUT          0x8DDA
#endif // GL_GEOMETRY_VERTICES_OUT
#ifndef GL_GEOMETRY_INPUT_TYPE
#define GL_GEOMETRY_INPUT_TYPE            0x8DDB
#endif // GL_GEOMETRY_INPUT_TYPE
#ifndef GL_GEOMETRY_OUTPUT_TYPE
#define GL_GEOMETRY_OUTPUT_TYPE           0x8DDC
#endif // GL_GEOMETRY_OUTPUT_TYPE
#ifndef GL_LINES_ADJACENCY
#define GL_LINES_ADJACENCY                0xA
#endif // GL_LINES_ADJACENCY
#ifndef GL_LINE_STRIP_ADJACENCY
#define GL_LINE_STRIP_ADJACENCY           0xB
#endif // GL_LINE_STRIP_ADJACENCY
#ifndef GL_TRIANGLES_ADJACENCY
#define GL_TRIANGLES_ADJACENCY            0xC
#endif // GL_TRIANGLES_ADJACENCY
#ifndef GL_TRIANGLE_STRIP_ADJACENCY
#define GL_TRIANGLE_STRIP_ADJACENCY       0xD
#endif // GL_TRIANGLE_STRIP_ADJACENCY

class OdTrGL2LocalContext;

class OdTrGL2ExtensionsRegistry : public OdRxObject
{
  public:
    enum ExtensionName
    {
      kExtensionNonPow2 = 0,
      kExtensionNonPow2ES,
      kExtensionBGRA,
      kExtensionBGR,
      kExtensionTextureMirroredRepeat,
      kExtensionTextureEdgeClamp,
      kExtensionTextureBorderClamp,
      kExtensionDepthTexture,
      kExtensionDepth24,
      kExtensionPackedDepthStencil,
      kExtensionStencil8,
      kExtensionTextureHalfFloat,
      kExtensionTextureFloat,
      kExtensionDrawBuffers,
      kExtensionElementIndexUInt,
      kExtensionGeometryShader,

      kNumExtensions
    };
    enum FunctionName
    {
      kPfnDrawBuffers = 0,
      kPfnProgramParameteri,

      kNumFunctions
    };
  protected:
    OdUInt32 m_extensionChecked[kNumExtensions / 32 + 1];
    OdUInt32 m_extensionAvailable[kNumExtensions / 32 + 1];
    OdTrGL2LocalContext *m_pLocalContext;
    void *m_pFunctionPtr[kNumFunctions];
    friend class OdTrGL2LocalContext;
  public:
    OdTrGL2ExtensionsRegistry();

    bool isExtensionSupported(ExtensionName nExt);

    bool isFunctionAvailable(FunctionName nFunc);

    // Extension function callers
    // kExtensionDrawBuffers
    void glDrawBuffers(GLsizei n, const GLenum *bufs);
    // kExtensionGeometryShader
    void glProgramParameteri(GLuint program, GLenum pname, GLint value);
};

#endif // ODTRGL2EXTENSIONS
