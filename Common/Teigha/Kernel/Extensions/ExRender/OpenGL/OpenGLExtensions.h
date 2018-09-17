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

// OpenGL extensions.
// This header should be included after TD_GL to avoid redefinitions in gl.h.

// This is compilation of extensions described here: http://www.opengl.org/registry/
// Includes only extensions used in ODA device.

#ifndef ODOGLEXT_INCLUDED
#define ODOGLEXT_INCLUDED

#ifndef APIENTRY
#define APIENTRY
#endif

#ifndef GL_EXT_bgra
#define GL_EXT_bgra 1

#define GL_BGR_EXT                        0x80E0
#define GL_BGRA_EXT                       0x80E1
#endif

#ifndef GL_ARB_texture_mirrored_repeat
#define GL_ARB_texture_mirrored_repeat 1

#define GL_MIRRORED_REPEAT_ARB            0x8370
#endif

#ifndef GL_ARB_transpose_matrix
#define GL_ARB_transpose_matrix 1

#define TRANSPOSE_MODELVIEW_MATRIX_ARB    0x84E3
#define TRANSPOSE_PROJECTION_MATRIX_ARB   0x84E4
#define TRANSPOSE_TEXTURE_MATRIX_ARB      0x84E5
#define TRANSPOSE_COLOR_MATRIX_ARB        0x84E6

typedef void (APIENTRY * PFNGLLOADTRANSPOSEMATRIXFARBPROC)(const GLfloat *m);
typedef void (APIENTRY * PFNGLLOADTRANSPOSEMATRIXDARBPROC)(const GLdouble *m);
typedef void (APIENTRY * PFNGLMULTTRANSPOSEMATRIXFARBPROC)(const GLfloat *m);
typedef void (APIENTRY * PFNGLMULTTRANSPOSEMATRIXDARBPROC)(const GLdouble *m);

#endif

// GL_ARB_transpose_matrix internal protocol
typedef void (APIENTRY * OdGLFn_LoadTransposeMatrixf_ARB)(const GLfloat *m);
typedef void (APIENTRY * OdGLFn_LoadTransposeMatrixd_ARB)(const GLdouble *m);
typedef void (APIENTRY * OdGLFn_MultTransposeMatrixf_ARB)(const GLfloat *m);
typedef void (APIENTRY * OdGLFn_MultTransposeMatrixd_ARB)(const GLdouble *m);
//

#ifndef GL_EXT_blend_color
#define GL_EXT_blend_color 1

#define CONSTANT_COLOR_EXT                0x8001
#define ONE_MINUS_CONSTANT_COLOR_EXT      0x8002
#define CONSTANT_ALPHA_EXT                0x8003
#define ONE_MINUS_CONSTANT_ALPHA_EXT      0x8004

#define BLEND_COLOR_EXT                   0x8005

typedef void (APIENTRY * PFNGLBLENDCOLOREXTPROC)(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha);

#endif

// GL_EXT_blend_color internal protocol
typedef void (APIENTRY * OdGLFn_BlendColor_EXT)(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha);
//

#ifndef GL_EXT_texture_compression_s3tc
#define GL_EXT_texture_compression_s3tc 1

#define GL_COMPRESSED_RGB_S3TC_DXT1_EXT   0x83F0
#define GL_COMPRESSED_RGBA_S3TC_DXT1_EXT  0x83F1
#define GL_COMPRESSED_RGBA_S3TC_DXT3_EXT  0x83F2
#define GL_COMPRESSED_RGBA_S3TC_DXT5_EXT  0x83F3

typedef void (APIENTRY * PFNGLCOMPRESSEDTEXIMAGE2DEXTPROC)(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const GLvoid *data);

#endif

// GL_EXT_texture_compression_s3tc internal protocol
typedef void (APIENTRY * OdGLFn_CompressedTexImage2D_EXT)(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const GLvoid *data);
//

#ifndef GL_EXT_texture_edge_clamp
#define GL_EXT_texture_edge_clamp 1

#define GL_CLAMP_TO_EDGE                  0x812F
#endif

// GL_POINT_SIZE_RANGE and GL_POINT_SIZE_GRANULARITY are deprecated in GL versions 1.2 and greater. Their functionality has been
// replaced by GL_SMOOTH_POINT_SIZE_RANGE and GL_SMOOTH_POINT_SIZE_GRANULARITY.
// In OpenGL 1.2, the tokens GL_LINE_WIDTH_RANGE and GL_LINE_WIDTH_GRANULARITY were replaced by GL_ALIASED_LINE_WIDTH_RANGE,
// GL_SMOOTH_LINE_WIDTH_RANGE, and GL_SMOOTH_LINE_WIDTH_GRANULARITY. The old names are retained for backward compatibility,
// but should not be used in new code.
#ifndef GL_SMOOTH_POINT_SIZE_RANGE
#define GL_SMOOTH_POINT_SIZE_RANGE GL_POINT_SIZE_RANGE
#endif
#ifndef GL_SMOOTH_LINE_WIDTH_RANGE
#define GL_SMOOTH_LINE_WIDTH_RANGE GL_LINE_WIDTH_RANGE
#endif

#ifndef GL_ARB_vertex_buffer_object
#define GL_ARB_vertex_buffer_object 1

#define GL_BUFFER_SIZE_ARB                0x8764
#define GL_BUFFER_USAGE_ARB               0x8765
#define GL_ARRAY_BUFFER_ARB               0x8892
#define GL_ELEMENT_ARRAY_BUFFER_ARB       0x8893
#define GL_ARRAY_BUFFER_BINDING_ARB       0x8894
#define GL_ELEMENT_ARRAY_BUFFER_BINDING_ARB 0x8895
#define GL_VERTEX_ARRAY_BUFFER_BINDING_ARB 0x8896
#define GL_NORMAL_ARRAY_BUFFER_BINDING_ARB 0x8897
#define GL_COLOR_ARRAY_BUFFER_BINDING_ARB 0x8898
#define GL_INDEX_ARRAY_BUFFER_BINDING_ARB 0x8899
#define GL_TEXTURE_COORD_ARRAY_BUFFER_BINDING_ARB 0x889A
#define GL_EDGE_FLAG_ARRAY_BUFFER_BINDING_ARB 0x889B
#define GL_SECONDARY_COLOR_ARRAY_BUFFER_BINDING_ARB 0x889C
#define GL_FOG_COORDINATE_ARRAY_BUFFER_BINDING_ARB 0x889D
#define GL_WEIGHT_ARRAY_BUFFER_BINDING_ARB 0x889E
#define GL_VERTEX_ATTRIB_ARRAY_BUFFER_BINDING_ARB 0x889F
#define GL_READ_ONLY_ARB                  0x88B8
#define GL_WRITE_ONLY_ARB                 0x88B9
#define GL_READ_WRITE_ARB                 0x88BA
#define GL_BUFFER_ACCESS_ARB              0x88BB
#define GL_BUFFER_MAPPED_ARB              0x88BC
#define GL_BUFFER_MAP_POINTER_ARB         0x88BD
#define GL_STREAM_DRAW_ARB                0x88E0
#define GL_STREAM_READ_ARB                0x88E1
#define GL_STREAM_COPY_ARB                0x88E2
#define GL_STATIC_DRAW_ARB                0x88E4
#define GL_STATIC_READ_ARB                0x88E5
#define GL_STATIC_COPY_ARB                0x88E6
#define GL_DYNAMIC_DRAW_ARB               0x88E8
#define GL_DYNAMIC_READ_ARB               0x88E9
#define GL_DYNAMIC_COPY_ARB               0x88EA

/* GL types for handling large vertex buffer objects */
typedef ptrdiff_t GLintptrARB;
typedef ptrdiff_t GLsizeiptrARB;

typedef void (APIENTRY * PFNGLBINDBUFFERARBPROC)(GLenum target, GLuint buffer);
typedef void (APIENTRY * PFNGLDELETEBUFFERSARBPROC)(GLsizei n, const GLuint *buffers);
typedef void (APIENTRY * PFNGLGENBUFFERSARBPROC)(GLsizei n, GLuint *buffers);
typedef GLboolean (APIENTRY * PFNGLISBUFFERARBPROC)(GLuint buffer);
typedef void (APIENTRY * PFNGLBUFFERDATAARBPROC)(GLenum target, GLsizeiptrARB size, const GLvoid *data, GLenum usage);
typedef void (APIENTRY * PFNGLBUFFERSUBDATAARBPROC)(GLenum target, GLintptrARB offset, GLsizeiptrARB size, const GLvoid *data);
typedef void (APIENTRY * PFNGLGETBUFFERSUBDATAARBPROC)(GLenum target, GLintptrARB offset, GLsizeiptrARB size, GLvoid *data);
typedef GLvoid* (APIENTRY * PFNGLMAPBUFFERARBPROC)(GLenum target, GLenum access);
typedef GLboolean (APIENTRY * PFNGLUNMAPBUFFERARBPROC)(GLenum target);
typedef void (APIENTRY * PFNGLGETBUFFERPARAMETERIVARBPROC)(GLenum target, GLenum pname, GLint *params);
typedef void (APIENTRY * PFNGLGETBUFFERPOINTERVARBPROC)(GLenum target, GLenum pname, GLvoid* *params);

#endif

// GL_ARB_vertex_buffer_object internal protocol
typedef void (APIENTRY * OdGLFn_BindBuffer_ARB)(GLenum target, GLuint buffer);
typedef void (APIENTRY * OdGLFn_DeleteBuffers_ARB)(GLsizei n, const GLuint *buffers);
typedef void (APIENTRY * OdGLFn_GenBuffers_ARB)(GLsizei n, GLuint *buffers);
typedef GLboolean (APIENTRY * OdGLFn_IsBuffer_ARB)(GLuint buffer);
typedef void (APIENTRY * OdGLFn_BufferData_ARB)(GLenum target, GLsizeiptrARB size, const GLvoid *data, GLenum usage);
typedef void (APIENTRY * OdGLFn_BufferSubData_ARB)(GLenum target, GLintptrARB offset, GLsizeiptrARB size, const GLvoid *data);
typedef void (APIENTRY * OdGLFn_GetBufferSubData_ARB)(GLenum target, GLintptrARB offset, GLsizeiptrARB size, GLvoid *data);
typedef GLvoid* (APIENTRY * OdGLFn_MapBuffer_ARB)(GLenum target, GLenum access);
typedef GLboolean (APIENTRY * OdGLFn_UnmapBuffer_ARB)(GLenum target);
typedef void (APIENTRY * OdGLFn_GetBufferParameteriv_ARB)(GLenum target, GLenum pname, GLint *params);
typedef void (APIENTRY * OdGLFn_GetBufferPointerv_ARB)(GLenum target, GLenum pname, GLvoid* *params);
//

#ifndef GL_ARB_pixel_buffer_object
#define GL_ARB_pixel_buffer_object 1

#define GL_PIXEL_PACK_BUFFER_ARB          0x88EB
#define GL_PIXEL_UNPACK_BUFFER_ARB        0x88EC
#define GL_PIXEL_PACK_BUFFER_BINDING_ARB  0x88ED
#define GL_PIXEL_UNPACK_BUFFER_BINDING_ARB 0x88EF

#endif

#ifndef GL_ARB_fragment_program
#define GL_ARB_fragment_program 1

#define GL_FRAGMENT_PROGRAM_ARB           0x8804
#define GL_PROGRAM_ERROR_POSITION_ARB     0x864B
#define GL_PROGRAM_ERROR_STRING_ARB       0x8874
#define GL_PROGRAM_FORMAT_ASCII_ARB       0x8875

typedef void (APIENTRY * PFNGLACTIVETEXTUREARBPROC)(GLenum texture); // Actually from another extension, but GL_ARB_fragment_program
                                                                     // haven't sense without it.
typedef void (APIENTRY * PFNGLPROGRAMSTRINGARBPROC)(GLenum target, GLenum format, GLsizei len, const GLvoid *string);
typedef void (APIENTRY * PFNGLBINDPROGRAMARBPROC)(GLenum target, GLuint program);
typedef void (APIENTRY * PFNGLDELETEPROGRAMSARBPROC)(GLsizei n, const GLuint *programs);
typedef void (APIENTRY * PFNGLGENPROGRAMSARBPROC)(GLsizei n, GLuint *programs);
typedef void (APIENTRY * PFNGLPROGRAMLOCALPARAMETER4FARBPROC)(GLenum target, GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
typedef void (APIENTRY * PFNGLPROGRAMLOCALPARAMETER4FVARBPROC)(GLenum target, GLuint index, const GLfloat *params);

#endif

// GL_ARB_fragment_program internal protocol
typedef void (APIENTRY * OdGLFn_ActiveTexture_ARB)(GLenum texture);
typedef void (APIENTRY * OdGLFn_ProgramString_ARB)(GLenum target, GLenum format, GLsizei len, const GLvoid *string);
typedef void (APIENTRY * OdGLFn_BindProgram_ARB)(GLenum target, GLuint program);
typedef void (APIENTRY * OdGLFn_DeletePrograms_ARB)(GLsizei n, const GLuint *programs);
typedef void (APIENTRY * OdGLFn_GenPrograms_ARB)(GLsizei n, GLuint *programs);
typedef void (APIENTRY * OdGLFn_ProgramLocalParameter4f_ARB)(GLenum target, GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
typedef void (APIENTRY * OdGLFn_ProgramLocalParameter4fv_ARB)(GLenum target, GLuint index, const GLfloat *params);
//

#ifndef GL_TEXTURE0
#define GL_TEXTURE0                       0x84C0
#endif

// WGL extensions
#if defined(WIN32) || defined(WIN64)

#ifndef WGL_ARB_pixel_format
#define WGL_ARB_pixel_format 1

#define WGL_NUMBER_PIXEL_FORMATS_ARB   0x2000
#define WGL_DRAW_TO_WINDOW_ARB         0x2001
#define WGL_DRAW_TO_BITMAP_ARB         0x2002
#define WGL_ACCELERATION_ARB           0x2003
#define WGL_NEED_PALETTE_ARB           0x2004
#define WGL_NEED_SYSTEM_PALETTE_ARB    0x2005
#define WGL_SWAP_LAYER_BUFFERS_ARB     0x2006
#define WGL_SWAP_METHOD_ARB            0x2007
#define WGL_NUMBER_OVERLAYS_ARB        0x2008
#define WGL_NUMBER_UNDERLAYS_ARB       0x2009
#define WGL_TRANSPARENT_ARB            0x200A
#define WGL_TRANSPARENT_RED_VALUE_ARB  0x2037
#define WGL_TRANSPARENT_GREEN_VALUE_ARB 0x2038
#define WGL_TRANSPARENT_BLUE_VALUE_ARB 0x2039
#define WGL_TRANSPARENT_ALPHA_VALUE_ARB 0x203A
#define WGL_TRANSPARENT_INDEX_VALUE_ARB 0x203B
#define WGL_SHARE_DEPTH_ARB            0x200C
#define WGL_SHARE_STENCIL_ARB          0x200D
#define WGL_SHARE_ACCUM_ARB            0x200E
#define WGL_SUPPORT_GDI_ARB            0x200F
#define WGL_SUPPORT_OPENGL_ARB         0x2010
#define WGL_DOUBLE_BUFFER_ARB          0x2011
#define WGL_STEREO_ARB                 0x2012
#define WGL_PIXEL_TYPE_ARB             0x2013
#define WGL_COLOR_BITS_ARB             0x2014
#define WGL_RED_BITS_ARB               0x2015
#define WGL_RED_SHIFT_ARB              0x2016
#define WGL_GREEN_BITS_ARB             0x2017
#define WGL_GREEN_SHIFT_ARB            0x2018
#define WGL_BLUE_BITS_ARB              0x2019
#define WGL_BLUE_SHIFT_ARB             0x201A
#define WGL_ALPHA_BITS_ARB             0x201B
#define WGL_ALPHA_SHIFT_ARB            0x201C
#define WGL_ACCUM_BITS_ARB             0x201D
#define WGL_ACCUM_RED_BITS_ARB         0x201E
#define WGL_ACCUM_GREEN_BITS_ARB       0x201F
#define WGL_ACCUM_BLUE_BITS_ARB        0x2020
#define WGL_ACCUM_ALPHA_BITS_ARB       0x2021
#define WGL_DEPTH_BITS_ARB             0x2022
#define WGL_STENCIL_BITS_ARB           0x2023
#define WGL_AUX_BUFFERS_ARB            0x2024
#define WGL_NO_ACCELERATION_ARB        0x2025
#define WGL_GENERIC_ACCELERATION_ARB   0x2026
#define WGL_FULL_ACCELERATION_ARB      0x2027
#define WGL_SWAP_EXCHANGE_ARB          0x2028
#define WGL_SWAP_COPY_ARB              0x2029
#define WGL_SWAP_UNDEFINED_ARB         0x202A
#define WGL_TYPE_RGBA_ARB              0x202B
#define WGL_TYPE_COLORINDEX_ARB        0x202C

typedef BOOL (APIENTRY * PFNWGLGETPIXELFORMATATTRIBIVARBPROC)(HDC hdc, int iPixelFormat, int iLayerPlane, UINT nAttributes, const int *piAttributes, int *piValues);
typedef BOOL (APIENTRY * PFNWGLGETPIXELFORMATATTRIBFVARBPROC)(HDC hdc, int iPixelFormat, int iLayerPlane, UINT nAttributes, const int *piAttributes, FLOAT *pfValues);
typedef BOOL (APIENTRY * PFNWGLCHOOSEPIXELFORMATARBPROC)(HDC hdc, const int *piAttribIList, const FLOAT *pfAttribFList, UINT nMaxFormats, int *piFormats, UINT *nNumFormats);

#endif

// WGL_ARB_pixel_format internal protocol
typedef BOOL (APIENTRY * OdGLFn_GetPixelFormatAttribiv_ARB)(HDC hdc, int iPixelFormat, int iLayerPlane, UINT nAttributes, const int *piAttributes, int *piValues);
typedef BOOL (APIENTRY * OdGLFn_GetPixelFormatAttribfv_ARB)(HDC hdc, int iPixelFormat, int iLayerPlane, UINT nAttributes, const int *piAttributes, FLOAT *pfValues);
typedef BOOL (APIENTRY * OdGLFn_ChoosePixelFormat_ARB)(HDC hdc, const int *piAttribIList, const FLOAT *pfAttribFList, UINT nMaxFormats, int *piFormats, UINT *nNumFormats);
//

#ifndef WGL_ARB_pbuffer
#define WGL_ARB_pbuffer 1

#define WGL_DRAW_TO_PBUFFER_ARB        0x202D
#define WGL_MAX_PBUFFER_PIXELS_ARB     0x202E
#define WGL_MAX_PBUFFER_WIDTH_ARB      0x202F
#define WGL_MAX_PBUFFER_HEIGHT_ARB     0x2030
#define WGL_PBUFFER_LARGEST_ARB        0x2033
#define WGL_PBUFFER_WIDTH_ARB          0x2034
#define WGL_PBUFFER_HEIGHT_ARB         0x2035
#define WGL_PBUFFER_LOST_ARB           0x2036

DECLARE_HANDLE(HPBUFFERARB);

typedef HPBUFFERARB (APIENTRY * PFNWGLCREATEPBUFFERARBPROC)(HDC hDC, int iPixelFormat, int iWidth, int iHeight, const int *piAttribList);
typedef HDC (APIENTRY * PFNWGLGETPBUFFERDCARBPROC)(HPBUFFERARB hPbuffer);
typedef int (APIENTRY * PFNWGLRELEASEPBUFFERDCARBPROC)(HPBUFFERARB hPbuffer, HDC hDC);
typedef BOOL (APIENTRY * PFNWGLDESTROYPBUFFERARBPROC)(HPBUFFERARB hPbuffer);
typedef BOOL (APIENTRY * PFNWGLQUERYPBUFFERARBPROC)(HPBUFFERARB hPbuffer, int iAttribute, int *piValue);

#endif

// WGL_ARB_pbuffer internal protocol
typedef HPBUFFERARB (APIENTRY * OdGLFn_CreatePBuffer_ARB)(HDC hDC, int iPixelFormat, int iWidth, int iHeight, const int *piAttribList);
typedef HDC (APIENTRY * OdGLFn_GetPBufferDC_ARB)(HPBUFFERARB hPbuffer);
typedef int (APIENTRY * OdGLFn_ReleasePBufferDC_ARB)(HPBUFFERARB hPbuffer, HDC hDC);
typedef BOOL (APIENTRY * OdGLFn_DestroyPBuffer_ARB)(HPBUFFERARB hPbuffer);
typedef BOOL (APIENTRY * OdGLFn_QueryPBuffer_ARB)(HPBUFFERARB hPbuffer, int iAttribute, int *piValue);
//

#ifndef WGL_ARB_extensions_string
#define WGL_ARB_extensions_string 1

typedef const char * (APIENTRY * PFNWGLGETEXTENSIONSSTRINGARBPROC)(HDC hdc);

#endif

// WGL_ARB_extensions_string internal protocol
typedef const char * (APIENTRY * OdGLFn_GetExtensionsString_ARB)(HDC hdc);
//

#endif // defined(WIN32) || defined(WIN64)

#endif // ODOGLEXT_INCLUDED
