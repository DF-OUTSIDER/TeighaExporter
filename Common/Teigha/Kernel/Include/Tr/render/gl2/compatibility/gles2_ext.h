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

#ifndef ODGLES2EXT
#define ODGLES2EXT

#ifndef GL_GLEXT_LEGACY // Don't include GL extensions on Linux
#define GL_GLEXT_LEGACY
#endif
#include "TD_GL.h"

#ifndef APIENTRY
#define APIENTRY
#endif
#ifndef APIENTRYP
#define APIENTRYP APIENTRY *
#endif

#ifndef GL_VERSION_1_2
#define GL_VERSION_1_2 1
#define GL_VERSION_1_2_GLES2 1

#define GL_UNSIGNED_SHORT_4_4_4_4         0x8033
#define GL_UNSIGNED_SHORT_5_5_5_1         0x8034
#define GL_UNSIGNED_SHORT_5_6_5           0x8363
#define GL_CLAMP_TO_EDGE                  0x812F
#define GL_ALIASED_LINE_WIDTH_RANGE       0x846E
#define GL_ALIASED_POINT_SIZE_RANGE       0x846D

#endif /* GL_VERSION_1_2 */

#ifndef GL_VERSION_1_3
#define GL_VERSION_1_3 1
#define GL_VERSION_1_3_GLES2 1

#define GL_TEXTURE0                       0x84C0
#define GL_TEXTURE1                       0x84C1
#define GL_TEXTURE2                       0x84C2
#define GL_TEXTURE3                       0x84C3
#define GL_TEXTURE4                       0x84C4
#define GL_TEXTURE5                       0x84C5
#define GL_TEXTURE6                       0x84C6
#define GL_TEXTURE7                       0x84C7
#define GL_TEXTURE8                       0x84C8
#define GL_TEXTURE9                       0x84C9
#define GL_TEXTURE10                      0x84CA
#define GL_TEXTURE11                      0x84CB
#define GL_TEXTURE12                      0x84CC
#define GL_TEXTURE13                      0x84CD
#define GL_TEXTURE14                      0x84CE
#define GL_TEXTURE15                      0x84CF
#define GL_TEXTURE16                      0x84D0
#define GL_TEXTURE17                      0x84D1
#define GL_TEXTURE18                      0x84D2
#define GL_TEXTURE19                      0x84D3
#define GL_TEXTURE20                      0x84D4
#define GL_TEXTURE21                      0x84D5
#define GL_TEXTURE22                      0x84D6
#define GL_TEXTURE23                      0x84D7
#define GL_TEXTURE24                      0x84D8
#define GL_TEXTURE25                      0x84D9
#define GL_TEXTURE26                      0x84DA
#define GL_TEXTURE27                      0x84DB
#define GL_TEXTURE28                      0x84DC
#define GL_TEXTURE29                      0x84DD
#define GL_TEXTURE30                      0x84DE
#define GL_TEXTURE31                      0x84DF
#define GL_ACTIVE_TEXTURE                 0x84E0
#define GL_SAMPLE_ALPHA_TO_COVERAGE       0x809E
#define GL_SAMPLE_COVERAGE                0x80A0
#define GL_SAMPLE_BUFFERS                 0x80A8
#define GL_SAMPLES                        0x80A9
#define GL_SAMPLE_COVERAGE_VALUE          0x80AA
#define GL_SAMPLE_COVERAGE_INVERT         0x80AB
#define GL_TEXTURE_CUBE_MAP               0x8513
#define GL_TEXTURE_BINDING_CUBE_MAP       0x8514
#define GL_TEXTURE_CUBE_MAP_POSITIVE_X    0x8515
#define GL_TEXTURE_CUBE_MAP_NEGATIVE_X    0x8516
#define GL_TEXTURE_CUBE_MAP_POSITIVE_Y    0x8517
#define GL_TEXTURE_CUBE_MAP_NEGATIVE_Y    0x8518
#define GL_TEXTURE_CUBE_MAP_POSITIVE_Z    0x8519
#define GL_TEXTURE_CUBE_MAP_NEGATIVE_Z    0x851A
#define GL_MAX_CUBE_MAP_TEXTURE_SIZE      0x851C
#define GL_NUM_COMPRESSED_TEXTURE_FORMATS 0x86A2
#define GL_COMPRESSED_TEXTURE_FORMATS     0x86A3

typedef void (APIENTRYP PFNGLACTIVETEXTUREPROC) (GLenum texture);
typedef void (APIENTRYP PFNGLSAMPLECOVERAGEPROC) (GLfloat value, GLboolean invert);
typedef void (APIENTRYP PFNGLCOMPRESSEDTEXIMAGE2DPROC) (GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const void *data);
typedef void (APIENTRYP PFNGLCOMPRESSEDTEXSUBIMAGE2DPROC) (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, const void *data);

void glActiveTexture_oda_ (GLenum texture);
void glSampleCoverage_oda_ (GLfloat value, GLboolean invert);
void glCompressedTexImage2D_oda_ (GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const void *data);
void glCompressedTexSubImage2D_oda_ (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, const void *data);

#define glActiveTexture glActiveTexture_oda_
#define glSampleCoverage glSampleCoverage_oda_
#define glCompressedTexImage2D glCompressedTexImage2D_oda_
#define glCompressedTexSubImage2D glCompressedTexSubImage2D_oda_

#endif /* GL_VERSION_1_3 */

#ifndef GL_VERSION_1_4
#define GL_VERSION_1_4 1
#define GL_VERSION_1_4_GLES2 1

#define GL_BLEND_DST_RGB                  0x80C8
#define GL_BLEND_SRC_RGB                  0x80C9
#define GL_BLEND_DST_ALPHA                0x80CA
#define GL_BLEND_SRC_ALPHA                0x80CB
#define GL_DEPTH_COMPONENT16              0x81A5
#define GL_MIRRORED_REPEAT                0x8370
#define GL_INCR_WRAP                      0x8507
#define GL_DECR_WRAP                      0x8508
#define GL_GENERATE_MIPMAP_HINT           0x8192
#define GL_FUNC_ADD                       0x8006
#define GL_FUNC_SUBTRACT                  0x800A
#define GL_FUNC_REVERSE_SUBTRACT          0x800B
#define GL_CONSTANT_COLOR                 0x8001
#define GL_ONE_MINUS_CONSTANT_COLOR       0x8002
#define GL_CONSTANT_ALPHA                 0x8003
#define GL_ONE_MINUS_CONSTANT_ALPHA       0x8004

typedef void (APIENTRYP PFNGLBLENDFUNCSEPARATEPROC) (GLenum sfactorRGB, GLenum dfactorRGB, GLenum sfactorAlpha, GLenum dfactorAlpha);
typedef void (APIENTRYP PFNGLBLENDCOLORPROC) (GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
typedef void (APIENTRYP PFNGLBLENDEQUATIONPROC) (GLenum mode);

void glBlendFuncSeparate_oda_ (GLenum sfactorRGB, GLenum dfactorRGB, GLenum sfactorAlpha, GLenum dfactorAlpha);
void glBlendColor_oda_ (GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
void glBlendEquation_oda_ (GLenum mode);

#define glBlendFuncSeparate glBlendFuncSeparate_oda_
#define glBlendColor glBlendColor_oda_
#define glBlendEquation glBlendEquation_oda_

#endif /* GL_VERSION_1_4 */

#ifndef GL_VERSION_1_5
#define GL_VERSION_1_5 1
#define GL_VERSION_1_5_GLES2 1

#include <stddef.h>
typedef ptrdiff_t GLsizeiptr;
typedef ptrdiff_t GLintptr;
#define GL_BUFFER_SIZE                    0x8764
#define GL_BUFFER_USAGE                   0x8765
#define GL_ARRAY_BUFFER                   0x8892
#define GL_ELEMENT_ARRAY_BUFFER           0x8893
#define GL_ARRAY_BUFFER_BINDING           0x8894
#define GL_ELEMENT_ARRAY_BUFFER_BINDING   0x8895
#define GL_VERTEX_ATTRIB_ARRAY_BUFFER_BINDING 0x889F
#define GL_STREAM_DRAW                    0x88E0
#define GL_STATIC_DRAW                    0x88E4
#define GL_DYNAMIC_DRAW                   0x88E8

typedef void (APIENTRYP PFNGLBINDBUFFERPROC) (GLenum target, GLuint buffer);
typedef void (APIENTRYP PFNGLDELETEBUFFERSPROC) (GLsizei n, const GLuint *buffers);
typedef void (APIENTRYP PFNGLGENBUFFERSPROC) (GLsizei n, GLuint *buffers);
typedef GLboolean (APIENTRYP PFNGLISBUFFERPROC) (GLuint buffer);
typedef void (APIENTRYP PFNGLBUFFERDATAPROC) (GLenum target, GLsizeiptr size, const void *data, GLenum usage);
typedef void (APIENTRYP PFNGLBUFFERSUBDATAPROC) (GLenum target, GLintptr offset, GLsizeiptr size, const void *data);
typedef void (APIENTRYP PFNGLGETBUFFERPARAMETERIVPROC) (GLenum target, GLenum pname, GLint *params);

void glBindBuffer_oda_ (GLenum target, GLuint buffer);
void glDeleteBuffers_oda_ (GLsizei n, const GLuint *buffers);
void glGenBuffers_oda_ (GLsizei n, GLuint *buffers);
GLboolean glIsBuffer_oda_ (GLuint buffer);
void glBufferData_oda_ (GLenum target, GLsizeiptr size, const void *data, GLenum usage);
void glBufferSubData_oda_ (GLenum target, GLintptr offset, GLsizeiptr size, const void *data);
void glGetBufferParameteriv_oda_ (GLenum target, GLenum pname, GLint *params);

#define glBindBuffer glBindBuffer_oda_
#define glDeleteBuffers glDeleteBuffers_oda_
#define glGenBuffers glGenBuffers_oda_
#define glIsBuffer glIsBuffer_oda_
#define glBufferData glBufferData_oda_
#define glBufferSubData glBufferSubData_oda_
#define glGetBufferParameteriv glGetBufferParameteriv_oda_
#endif /* GL_VERSION_1_5 */

#ifndef GL_VERSION_2_0
#define GL_VERSION_2_0 1
#define GL_VERSION_2_0_GLES2 1

typedef char GLchar;
#define GL_BLEND_EQUATION_RGB             0x8009
#define GL_VERTEX_ATTRIB_ARRAY_ENABLED    0x8622
#define GL_VERTEX_ATTRIB_ARRAY_SIZE       0x8623
#define GL_VERTEX_ATTRIB_ARRAY_STRIDE     0x8624
#define GL_VERTEX_ATTRIB_ARRAY_TYPE       0x8625
#define GL_CURRENT_VERTEX_ATTRIB          0x8626
#define GL_VERTEX_ATTRIB_ARRAY_POINTER    0x8645
#define GL_STENCIL_BACK_FUNC              0x8800
#define GL_STENCIL_BACK_FAIL              0x8801
#define GL_STENCIL_BACK_PASS_DEPTH_FAIL   0x8802
#define GL_STENCIL_BACK_PASS_DEPTH_PASS   0x8803
#define GL_BLEND_EQUATION_ALPHA           0x883D
#define GL_MAX_VERTEX_ATTRIBS             0x8869
#define GL_VERTEX_ATTRIB_ARRAY_NORMALIZED 0x886A
#define GL_MAX_TEXTURE_IMAGE_UNITS        0x8872
#define GL_FRAGMENT_SHADER                0x8B30
#define GL_VERTEX_SHADER                  0x8B31
#define GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS 0x8B4C
#define GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS 0x8B4D
#define GL_SHADER_TYPE                    0x8B4F
#define GL_FLOAT_VEC2                     0x8B50
#define GL_FLOAT_VEC3                     0x8B51
#define GL_FLOAT_VEC4                     0x8B52
#define GL_INT_VEC2                       0x8B53
#define GL_INT_VEC3                       0x8B54
#define GL_INT_VEC4                       0x8B55
#define GL_BOOL                           0x8B56
#define GL_BOOL_VEC2                      0x8B57
#define GL_BOOL_VEC3                      0x8B58
#define GL_BOOL_VEC4                      0x8B59
#define GL_FLOAT_MAT2                     0x8B5A
#define GL_FLOAT_MAT3                     0x8B5B
#define GL_FLOAT_MAT4                     0x8B5C
#define GL_SAMPLER_2D                     0x8B5E
#define GL_SAMPLER_CUBE                   0x8B60
#define GL_DELETE_STATUS                  0x8B80
#define GL_COMPILE_STATUS                 0x8B81
#define GL_LINK_STATUS                    0x8B82
#define GL_VALIDATE_STATUS                0x8B83
#define GL_INFO_LOG_LENGTH                0x8B84
#define GL_ATTACHED_SHADERS               0x8B85
#define GL_ACTIVE_UNIFORMS                0x8B86
#define GL_ACTIVE_UNIFORM_MAX_LENGTH      0x8B87
#define GL_SHADER_SOURCE_LENGTH           0x8B88
#define GL_ACTIVE_ATTRIBUTES              0x8B89
#define GL_ACTIVE_ATTRIBUTE_MAX_LENGTH    0x8B8A
#define GL_SHADING_LANGUAGE_VERSION       0x8B8C
#define GL_CURRENT_PROGRAM                0x8B8D
#define GL_STENCIL_BACK_REF               0x8CA3
#define GL_STENCIL_BACK_VALUE_MASK        0x8CA4
#define GL_STENCIL_BACK_WRITEMASK         0x8CA5

typedef void (APIENTRYP PFNGLBLENDEQUATIONSEPARATEPROC) (GLenum modeRGB, GLenum modeAlpha);
typedef void (APIENTRYP PFNGLSTENCILOPSEPARATEPROC) (GLenum face, GLenum sfail, GLenum dpfail, GLenum dppass);
typedef void (APIENTRYP PFNGLSTENCILFUNCSEPARATEPROC) (GLenum face, GLenum func, GLint ref, GLuint mask);
typedef void (APIENTRYP PFNGLSTENCILMASKSEPARATEPROC) (GLenum face, GLuint mask);
typedef void (APIENTRYP PFNGLATTACHSHADERPROC) (GLuint program, GLuint shader);
typedef void (APIENTRYP PFNGLBINDATTRIBLOCATIONPROC) (GLuint program, GLuint index, const GLchar *name);
typedef void (APIENTRYP PFNGLCOMPILESHADERPROC) (GLuint shader);
typedef GLuint (APIENTRYP PFNGLCREATEPROGRAMPROC) (void);
typedef GLuint (APIENTRYP PFNGLCREATESHADERPROC) (GLenum type);
typedef void (APIENTRYP PFNGLDELETEPROGRAMPROC) (GLuint program);
typedef void (APIENTRYP PFNGLDELETESHADERPROC) (GLuint shader);
typedef void (APIENTRYP PFNGLDETACHSHADERPROC) (GLuint program, GLuint shader);
typedef void (APIENTRYP PFNGLDISABLEVERTEXATTRIBARRAYPROC) (GLuint index);
typedef void (APIENTRYP PFNGLENABLEVERTEXATTRIBARRAYPROC) (GLuint index);
typedef void (APIENTRYP PFNGLGETACTIVEATTRIBPROC) (GLuint program, GLuint index, GLsizei bufSize, GLsizei *length, GLint *size, GLenum *type, GLchar *name);
typedef void (APIENTRYP PFNGLGETACTIVEUNIFORMPROC) (GLuint program, GLuint index, GLsizei bufSize, GLsizei *length, GLint *size, GLenum *type, GLchar *name);
typedef void (APIENTRYP PFNGLGETATTACHEDSHADERSPROC) (GLuint program, GLsizei maxCount, GLsizei *count, GLuint *shaders);
typedef GLint (APIENTRYP PFNGLGETATTRIBLOCATIONPROC) (GLuint program, const GLchar *name);
typedef void (APIENTRYP PFNGLGETPROGRAMIVPROC) (GLuint program, GLenum pname, GLint *params);
typedef void (APIENTRYP PFNGLGETPROGRAMINFOLOGPROC) (GLuint program, GLsizei bufSize, GLsizei *length, GLchar *infoLog);
typedef void (APIENTRYP PFNGLGETSHADERIVPROC) (GLuint shader, GLenum pname, GLint *params);
typedef void (APIENTRYP PFNGLGETSHADERINFOLOGPROC) (GLuint shader, GLsizei bufSize, GLsizei *length, GLchar *infoLog);
typedef void (APIENTRYP PFNGLGETSHADERSOURCEPROC) (GLuint shader, GLsizei bufSize, GLsizei *length, GLchar *source);
typedef GLint (APIENTRYP PFNGLGETUNIFORMLOCATIONPROC) (GLuint program, const GLchar *name);
typedef void (APIENTRYP PFNGLGETUNIFORMFVPROC) (GLuint program, GLint location, GLfloat *params);
typedef void (APIENTRYP PFNGLGETUNIFORMIVPROC) (GLuint program, GLint location, GLint *params);
typedef void (APIENTRYP PFNGLGETVERTEXATTRIBFVPROC) (GLuint index, GLenum pname, GLfloat *params);
typedef void (APIENTRYP PFNGLGETVERTEXATTRIBIVPROC) (GLuint index, GLenum pname, GLint *params);
typedef void (APIENTRYP PFNGLGETVERTEXATTRIBPOINTERVPROC) (GLuint index, GLenum pname, void **pointer);
typedef GLboolean (APIENTRYP PFNGLISPROGRAMPROC) (GLuint program);
typedef GLboolean (APIENTRYP PFNGLISSHADERPROC) (GLuint shader);
typedef void (APIENTRYP PFNGLLINKPROGRAMPROC) (GLuint program);
typedef void (APIENTRYP PFNGLSHADERSOURCEPROC) (GLuint shader, GLsizei count, const GLchar *const*string, const GLint *length);
typedef void (APIENTRYP PFNGLUSEPROGRAMPROC) (GLuint program);
typedef void (APIENTRYP PFNGLUNIFORM1FPROC) (GLint location, GLfloat v0);
typedef void (APIENTRYP PFNGLUNIFORM2FPROC) (GLint location, GLfloat v0, GLfloat v1);
typedef void (APIENTRYP PFNGLUNIFORM3FPROC) (GLint location, GLfloat v0, GLfloat v1, GLfloat v2);
typedef void (APIENTRYP PFNGLUNIFORM4FPROC) (GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);
typedef void (APIENTRYP PFNGLUNIFORM1IPROC) (GLint location, GLint v0);
typedef void (APIENTRYP PFNGLUNIFORM2IPROC) (GLint location, GLint v0, GLint v1);
typedef void (APIENTRYP PFNGLUNIFORM3IPROC) (GLint location, GLint v0, GLint v1, GLint v2);
typedef void (APIENTRYP PFNGLUNIFORM4IPROC) (GLint location, GLint v0, GLint v1, GLint v2, GLint v3);
typedef void (APIENTRYP PFNGLUNIFORM1FVPROC) (GLint location, GLsizei count, const GLfloat *value);
typedef void (APIENTRYP PFNGLUNIFORM2FVPROC) (GLint location, GLsizei count, const GLfloat *value);
typedef void (APIENTRYP PFNGLUNIFORM3FVPROC) (GLint location, GLsizei count, const GLfloat *value);
typedef void (APIENTRYP PFNGLUNIFORM4FVPROC) (GLint location, GLsizei count, const GLfloat *value);
typedef void (APIENTRYP PFNGLUNIFORM1IVPROC) (GLint location, GLsizei count, const GLint *value);
typedef void (APIENTRYP PFNGLUNIFORM2IVPROC) (GLint location, GLsizei count, const GLint *value);
typedef void (APIENTRYP PFNGLUNIFORM3IVPROC) (GLint location, GLsizei count, const GLint *value);
typedef void (APIENTRYP PFNGLUNIFORM4IVPROC) (GLint location, GLsizei count, const GLint *value);
typedef void (APIENTRYP PFNGLUNIFORMMATRIX2FVPROC) (GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
typedef void (APIENTRYP PFNGLUNIFORMMATRIX3FVPROC) (GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
typedef void (APIENTRYP PFNGLUNIFORMMATRIX4FVPROC) (GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
typedef void (APIENTRYP PFNGLVALIDATEPROGRAMPROC) (GLuint program);
typedef void (APIENTRYP PFNGLVERTEXATTRIB1FPROC) (GLuint index, GLfloat x);
typedef void (APIENTRYP PFNGLVERTEXATTRIB1FVPROC) (GLuint index, const GLfloat *v);
typedef void (APIENTRYP PFNGLVERTEXATTRIB2FPROC) (GLuint index, GLfloat x, GLfloat y);
typedef void (APIENTRYP PFNGLVERTEXATTRIB2FVPROC) (GLuint index, const GLfloat *v);
typedef void (APIENTRYP PFNGLVERTEXATTRIB3FPROC) (GLuint index, GLfloat x, GLfloat y, GLfloat z);
typedef void (APIENTRYP PFNGLVERTEXATTRIB3FVPROC) (GLuint index, const GLfloat *v);
typedef void (APIENTRYP PFNGLVERTEXATTRIB4FPROC) (GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
typedef void (APIENTRYP PFNGLVERTEXATTRIB4FVPROC) (GLuint index, const GLfloat *v);
typedef void (APIENTRYP PFNGLVERTEXATTRIBPOINTERPROC) (GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void *pointer);

void glBlendEquationSeparate_oda_ (GLenum modeRGB, GLenum modeAlpha);
void glStencilOpSeparate_oda_ (GLenum face, GLenum sfail, GLenum dpfail, GLenum dppass);
void glStencilFuncSeparate_oda_ (GLenum face, GLenum func, GLint ref, GLuint mask);
void glStencilMaskSeparate_oda_ (GLenum face, GLuint mask);
void glAttachShader_oda_ (GLuint program, GLuint shader);
void glBindAttribLocation_oda_ (GLuint program, GLuint index, const GLchar *name);
void glCompileShader_oda_ (GLuint shader);
GLuint glCreateProgram_oda_ (void);
GLuint glCreateShader_oda_ (GLenum type);
void glDeleteProgram_oda_ (GLuint program);
void glDeleteShader_oda_ (GLuint shader);
void glDetachShader_oda_ (GLuint program, GLuint shader);
void glDisableVertexAttribArray_oda_ (GLuint index);
void glEnableVertexAttribArray_oda_ (GLuint index);
void glGetActiveAttrib_oda_ (GLuint program, GLuint index, GLsizei bufSize, GLsizei *length, GLint *size, GLenum *type, GLchar *name);
void glGetActiveUniform_oda_ (GLuint program, GLuint index, GLsizei bufSize, GLsizei *length, GLint *size, GLenum *type, GLchar *name);
void glGetAttachedShaders_oda_ (GLuint program, GLsizei maxCount, GLsizei *count, GLuint *shaders);
GLint glGetAttribLocation_oda_ (GLuint program, const GLchar *name);
void glGetProgramiv_oda_ (GLuint program, GLenum pname, GLint *params);
void glGetProgramInfoLog_oda_ (GLuint program, GLsizei bufSize, GLsizei *length, GLchar *infoLog);
void glGetShaderiv_oda_ (GLuint shader, GLenum pname, GLint *params);
void glGetShaderInfoLog_oda_ (GLuint shader, GLsizei bufSize, GLsizei *length, GLchar *infoLog);
void glGetShaderSource_oda_ (GLuint shader, GLsizei bufSize, GLsizei *length, GLchar *source);
GLint glGetUniformLocation_oda_ (GLuint program, const GLchar *name);
void glGetUniformfv_oda_ (GLuint program, GLint location, GLfloat *params);
void glGetUniformiv_oda_ (GLuint program, GLint location, GLint *params);
void glGetVertexAttribfv_oda_ (GLuint index, GLenum pname, GLfloat *params);
void glGetVertexAttribiv_oda_ (GLuint index, GLenum pname, GLint *params);
void glGetVertexAttribPointerv_oda_ (GLuint index, GLenum pname, void **pointer);
GLboolean glIsProgram_oda_ (GLuint program);
GLboolean glIsShader_oda_ (GLuint shader);
void glLinkProgram_oda_ (GLuint program);
void glShaderSource_oda_ (GLuint shader, GLsizei count, const GLchar *const*string, const GLint *length);
void glUseProgram_oda_ (GLuint program);
void glUniform1f_oda_ (GLint location, GLfloat v0);
void glUniform2f_oda_ (GLint location, GLfloat v0, GLfloat v1);
void glUniform3f_oda_ (GLint location, GLfloat v0, GLfloat v1, GLfloat v2);
void glUniform4f_oda_ (GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);
void glUniform1i_oda_ (GLint location, GLint v0);
void glUniform2i_oda_ (GLint location, GLint v0, GLint v1);
void glUniform3i_oda_ (GLint location, GLint v0, GLint v1, GLint v2);
void glUniform4i_oda_ (GLint location, GLint v0, GLint v1, GLint v2, GLint v3);
void glUniform1fv_oda_ (GLint location, GLsizei count, const GLfloat *value);
void glUniform2fv_oda_ (GLint location, GLsizei count, const GLfloat *value);
void glUniform3fv_oda_ (GLint location, GLsizei count, const GLfloat *value);
void glUniform4fv_oda_ (GLint location, GLsizei count, const GLfloat *value);
void glUniform1iv_oda_ (GLint location, GLsizei count, const GLint *value);
void glUniform2iv_oda_ (GLint location, GLsizei count, const GLint *value);
void glUniform3iv_oda_ (GLint location, GLsizei count, const GLint *value);
void glUniform4iv_oda_ (GLint location, GLsizei count, const GLint *value);
void glUniformMatrix2fv_oda_ (GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
void glUniformMatrix3fv_oda_ (GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
void glUniformMatrix4fv_oda_ (GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
void glValidateProgram_oda_ (GLuint program);
void glVertexAttrib1f_oda_ (GLuint index, GLfloat x);
void glVertexAttrib1fv_oda_ (GLuint index, const GLfloat *v);
void glVertexAttrib2f_oda_ (GLuint index, GLfloat x, GLfloat y);
void glVertexAttrib2fv_oda_ (GLuint index, const GLfloat *v);
void glVertexAttrib3f_oda_ (GLuint index, GLfloat x, GLfloat y, GLfloat z);
void glVertexAttrib3fv_oda_ (GLuint index, const GLfloat *v);
void glVertexAttrib4f_oda_ (GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
void glVertexAttrib4fv_oda_ (GLuint index, const GLfloat *v);
void glVertexAttribPointer_oda_ (GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void *pointer);

#define glBlendEquationSeparate glBlendEquationSeparate_oda_
#define glStencilOpSeparate glStencilOpSeparate_oda_
#define glStencilFuncSeparate glStencilFuncSeparate_oda_
#define glStencilMaskSeparate glStencilMaskSeparate_oda_
#define glAttachShader glAttachShader_oda_
#define glBindAttribLocation glBindAttribLocation_oda_
#define glCompileShader glCompileShader_oda_
#define glCreateProgram glCreateProgram_oda_
#define glCreateShader glCreateShader_oda_
#define glDeleteProgram glDeleteProgram_oda_
#define glDeleteShader glDeleteShader_oda_
#define glDetachShader glDetachShader_oda_
#define glDisableVertexAttribArray glDisableVertexAttribArray_oda_
#define glEnableVertexAttribArray glEnableVertexAttribArray_oda_
#define glGetActiveAttrib glGetActiveAttrib_oda_
#define glGetActiveUniform glGetActiveUniform_oda_
#define glGetAttachedShaders glGetAttachedShaders_oda_
#define glGetAttribLocation glGetAttribLocation_oda_
#define glGetProgramiv glGetProgramiv_oda_
#define glGetProgramInfoLog glGetProgramInfoLog_oda_
#define glGetShaderiv glGetShaderiv_oda_
#define glGetShaderInfoLog glGetShaderInfoLog_oda_
#define glGetShaderSource glGetShaderSource_oda_
#define glGetUniformLocation glGetUniformLocation_oda_
#define glGetUniformfv glGetUniformfv_oda_
#define glGetUniformiv glGetUniformiv_oda_
#define glGetVertexAttribfv glGetVertexAttribfv_oda_
#define glGetVertexAttribiv glGetVertexAttribiv_oda_
#define glGetVertexAttribPointerv glGetVertexAttribPointerv_oda_
#define glIsProgram glIsProgram_oda_
#define glIsShader glIsShader_oda_
#define glLinkProgram glLinkProgram_oda_
#define glShaderSource glShaderSource_oda_
#define glUseProgram glUseProgram_oda_
#define glUniform1f glUniform1f_oda_
#define glUniform2f glUniform2f_oda_
#define glUniform3f glUniform3f_oda_
#define glUniform4f glUniform4f_oda_
#define glUniform1i glUniform1i_oda_
#define glUniform2i glUniform2i_oda_
#define glUniform3i glUniform3i_oda_
#define glUniform4i glUniform4i_oda_
#define glUniform1fv glUniform1fv_oda_
#define glUniform2fv glUniform2fv_oda_
#define glUniform3fv glUniform3fv_oda_
#define glUniform4fv glUniform4fv_oda_
#define glUniform1iv glUniform1iv_oda_
#define glUniform2iv glUniform2iv_oda_
#define glUniform3iv glUniform3iv_oda_
#define glUniform4iv glUniform4iv_oda_
#define glUniformMatrix2fv glUniformMatrix2fv_oda_
#define glUniformMatrix3fv glUniformMatrix3fv_oda_
#define glUniformMatrix4fv glUniformMatrix4fv_oda_
#define glValidateProgram glValidateProgram_oda_
#define glVertexAttrib1f glVertexAttrib1f_oda_
#define glVertexAttrib1fv glVertexAttrib1fv_oda_
#define glVertexAttrib2f glVertexAttrib2f_oda_
#define glVertexAttrib2fv glVertexAttrib2fv_oda_
#define glVertexAttrib3f glVertexAttrib3f_oda_
#define glVertexAttrib3fv glVertexAttrib3fv_oda_
#define glVertexAttrib4f glVertexAttrib4f_oda_
#define glVertexAttrib4fv glVertexAttrib4fv_oda_
#define glVertexAttribPointer glVertexAttribPointer_oda_

#endif /* GL_VERSION_2_0 */

#ifdef __cplusplus
extern "C" {
#endif

#ifndef GL_VERSION_3_0
#define GL_VERSION_3_0 1
#define GL_VERSION_3_0_GLES2 1

#define GL_INVALID_FRAMEBUFFER_OPERATION  0x0506
#define GL_MAX_RENDERBUFFER_SIZE          0x84E8
#define GL_FRAMEBUFFER_BINDING            0x8CA6
#define GL_RENDERBUFFER_BINDING           0x8CA7
#define GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE 0x8CD0
#define GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME 0x8CD1
#define GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_LEVEL 0x8CD2
#define GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_CUBE_MAP_FACE 0x8CD3
#define GL_FRAMEBUFFER_COMPLETE           0x8CD5
#define GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT 0x8CD6
#define GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT 0x8CD7
#define GL_FRAMEBUFFER_UNSUPPORTED        0x8CDD
#define GL_COLOR_ATTACHMENT0              0x8CE0
#define GL_DEPTH_ATTACHMENT               0x8D00
#define GL_STENCIL_ATTACHMENT             0x8D20
#define GL_FRAMEBUFFER                    0x8D40
#define GL_RENDERBUFFER                   0x8D41
#define GL_RENDERBUFFER_WIDTH             0x8D42
#define GL_RENDERBUFFER_HEIGHT            0x8D43
#define GL_RENDERBUFFER_INTERNAL_FORMAT   0x8D44
#define GL_STENCIL_INDEX8                 0x8D48
#define GL_RENDERBUFFER_RED_SIZE          0x8D50
#define GL_RENDERBUFFER_GREEN_SIZE        0x8D51
#define GL_RENDERBUFFER_BLUE_SIZE         0x8D52
#define GL_RENDERBUFFER_ALPHA_SIZE        0x8D53
#define GL_RENDERBUFFER_DEPTH_SIZE        0x8D54
#define GL_RENDERBUFFER_STENCIL_SIZE      0x8D55

typedef GLboolean (APIENTRYP PFNGLISRENDERBUFFERPROC) (GLuint renderbuffer);
typedef void (APIENTRYP PFNGLBINDRENDERBUFFERPROC) (GLenum target, GLuint renderbuffer);
typedef void (APIENTRYP PFNGLDELETERENDERBUFFERSPROC) (GLsizei n, const GLuint *renderbuffers);
typedef void (APIENTRYP PFNGLGENRENDERBUFFERSPROC) (GLsizei n, GLuint *renderbuffers);
typedef void (APIENTRYP PFNGLRENDERBUFFERSTORAGEPROC) (GLenum target, GLenum internalformat, GLsizei width, GLsizei height);
typedef void (APIENTRYP PFNGLGETRENDERBUFFERPARAMETERIVPROC) (GLenum target, GLenum pname, GLint *params);
typedef GLboolean (APIENTRYP PFNGLISFRAMEBUFFERPROC) (GLuint framebuffer);
typedef void (APIENTRYP PFNGLBINDFRAMEBUFFERPROC) (GLenum target, GLuint framebuffer);
typedef void (APIENTRYP PFNGLDELETEFRAMEBUFFERSPROC) (GLsizei n, const GLuint *framebuffers);
typedef void (APIENTRYP PFNGLGENFRAMEBUFFERSPROC) (GLsizei n, GLuint *framebuffers);
typedef GLenum (APIENTRYP PFNGLCHECKFRAMEBUFFERSTATUSPROC) (GLenum target);
typedef void (APIENTRYP PFNGLFRAMEBUFFERTEXTURE2DPROC) (GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
typedef void (APIENTRYP PFNGLFRAMEBUFFERRENDERBUFFERPROC) (GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer);
typedef void (APIENTRYP PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIVPROC) (GLenum target, GLenum attachment, GLenum pname, GLint *params);
typedef void (APIENTRYP PFNGLGENERATEMIPMAPPROC) (GLenum target);

GLboolean glIsRenderbuffer_oda_ (GLuint renderbuffer);
void glBindRenderbuffer_oda_ (GLenum target, GLuint renderbuffer);
void glDeleteRenderbuffers_oda_ (GLsizei n, const GLuint *renderbuffers);
void glGenRenderbuffers_oda_ (GLsizei n, GLuint *renderbuffers);
void glRenderbufferStorage_oda_ (GLenum target, GLenum internalformat, GLsizei width, GLsizei height);
void glGetRenderbufferParameteriv_oda_ (GLenum target, GLenum pname, GLint *params);
GLboolean glIsFramebuffer_oda_ (GLuint framebuffer);
void glBindFramebuffer_oda_ (GLenum target, GLuint framebuffer);
void glDeleteFramebuffers_oda_ (GLsizei n, const GLuint *framebuffers);
void glGenFramebuffers_oda_ (GLsizei n, GLuint *framebuffers);
GLenum glCheckFramebufferStatus_oda_ (GLenum target);
void glFramebufferTexture2D_oda_ (GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
void glFramebufferRenderbuffer_oda_ (GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer);
void glGetFramebufferAttachmentParameteriv_oda_ (GLenum target, GLenum attachment, GLenum pname, GLint *params);
void glGenerateMipmap_oda_ (GLenum target);

#define glIsRenderbuffer glIsRenderbuffer_oda_
#define glBindRenderbuffer glBindRenderbuffer_oda_
#define glDeleteRenderbuffers glDeleteRenderbuffers_oda_
#define glGenRenderbuffers glGenRenderbuffers_oda_
#define glRenderbufferStorage glRenderbufferStorage_oda_
#define glGetRenderbufferParameteriv glGetRenderbufferParameteriv_oda_
#define glIsFramebuffer glIsFramebuffer_oda_
#define glBindFramebuffer glBindFramebuffer_oda_
#define glDeleteFramebuffers glDeleteFramebuffers_oda_
#define glGenFramebuffers glGenFramebuffers_oda_
#define glCheckFramebufferStatus glCheckFramebufferStatus_oda_
#define glFramebufferTexture2D glFramebufferTexture2D_oda_
#define glFramebufferRenderbuffer glFramebufferRenderbuffer_oda_
#define glGetFramebufferAttachmentParameteriv glGetFramebufferAttachmentParameteriv_oda_
#define glGenerateMipmap glGenerateMipmap_oda_

#endif /* GL_VERSION_3_0 */

#ifndef GL_VERSION_4_1
#define GL_VERSION_4_1 1
#define GL_VERSION_4_1_GLES2 1

#define GL_FIXED                          0x140C
#define GL_IMPLEMENTATION_COLOR_READ_TYPE 0x8B9A
#define GL_IMPLEMENTATION_COLOR_READ_FORMAT 0x8B9B
#define GL_LOW_FLOAT                      0x8DF0
#define GL_MEDIUM_FLOAT                   0x8DF1
#define GL_HIGH_FLOAT                     0x8DF2
#define GL_LOW_INT                        0x8DF3
#define GL_MEDIUM_INT                     0x8DF4
#define GL_HIGH_INT                       0x8DF5
#define GL_SHADER_COMPILER                0x8DFA
#define GL_SHADER_BINARY_FORMATS          0x8DF8
#define GL_NUM_SHADER_BINARY_FORMATS      0x8DF9
#define GL_MAX_VERTEX_UNIFORM_VECTORS     0x8DFB
#define GL_MAX_VARYING_VECTORS            0x8DFC
#define GL_MAX_FRAGMENT_UNIFORM_VECTORS   0x8DFD
#define GL_RGB565                         0x8D62

typedef void (APIENTRYP PFNGLRELEASESHADERCOMPILERPROC) (void);
typedef void (APIENTRYP PFNGLSHADERBINARYPROC) (GLsizei count, const GLuint *shaders, GLenum binaryformat, const void *binary, GLsizei length);
typedef void (APIENTRYP PFNGLGETSHADERPRECISIONFORMATPROC) (GLenum shadertype, GLenum precisiontype, GLint *range, GLint *precision);
typedef void (APIENTRYP PFNGLDEPTHRANGEFPROC) (GLfloat n, GLfloat f);
typedef void (APIENTRYP PFNGLCLEARDEPTHFPROC) (GLfloat d);

void glReleaseShaderCompiler_oda_ (void);
void glShaderBinary_oda_ (GLsizei count, const GLuint *shaders, GLenum binaryformat, const void *binary, GLsizei length);
void glGetShaderPrecisionFormat_oda_ (GLenum shadertype, GLenum precisiontype, GLint *range, GLint *precision);
void glDepthRangef_oda_ (GLfloat n, GLfloat f);
void glClearDepthf_oda_ (GLfloat d);

#define glReleaseShaderCompiler glReleaseShaderCompiler_oda_
#define glShaderBinary glShaderBinary_oda_
#define glGetShaderPrecisionFormat glGetShaderPrecisionFormat_oda_
#define glDepthRangef glDepthRangef_oda_
#define glClearDepthf glClearDepthf_oda_

#endif /* GL_VERSION_4_1 */
#ifdef __cplusplus
}
#endif
// GL_EXT_blend_color
#ifndef GL_BLEND_COLOR
#define GL_BLEND_COLOR                    0x8005
#endif

// ???
#ifndef GL_BLEND_EQUATION
#define GL_BLEND_EQUATION                 0x8009
#endif

// ???
#ifndef GL_INVERT
#define GL_INVERT                         0x150A
#endif

// GL_EXT_texture
#ifndef GL_RGBA4
#define GL_RGBA4                          0x8056
#endif
#ifndef GL_RGB5_A1
#define GL_RGB5_A1                        0x8057
#endif

// ???
#ifndef GL_STENCIL_INDEX
#define GL_STENCIL_INDEX                  0x1901
#endif

// ???
#ifndef GL_NONE
#define GL_NONE                           0
#endif

// GL_EXT_framebuffer_object
#ifndef GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS
#define GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS 0x8CD9
#endif

// Extensions registry:
// To be sure that ES2 compatible:
// GL_ARB_ES2_compatibility
// To be sure that fragment and vertex programs supported:
// GL_ARB_fragment_program, GL_ARB_vertex_program
// To be sure that shaders supported:
// GL_ARB_vertex_shader, GL_ARB_fragment_shader, GL_ARB_shader_objects
// To be sure that buffers supported:
// GL_ARB_vertex_buffer_object
// To be sure that framebuffers supported:
// GL_ARB_framebuffer_object (GL_EXT_framebuffer_object)
// To be sure that glSampleCoverage supported:
// GL_ARB_multisample
// To be sure that glActiveTexture supported:
// GL_ARB_multitexture
// To be sure that compressed textures supported:
// GL_ARB_texture_compression
// To be sure that cube maps supported:
// GL_ARB_texture_cube_map (GL_EXT_texture_cube_map)
// To be sure that texture environment add works:
// GL_ARB_texture_env_add (GL_EXT_texture_env_add)
// To be sure that texture mirror works:
// GL_ARB_texture_mirrored_repeat
// To be sure that edge clamp mode works:
// GL_EXT_texture_edge_clamp
// To be sure that glBlendColor and others present:
// GL_EXT_blend_color, GL_EXT_blend_equation_separate, GL_EXT_blend_func_separate, GL_EXT_blend_subtract
// Separate stencil buffer support:
// GL_ATI_separate_stencil

// Interface extensions

void *GLES2_GetProcAddress(const char *pFuncName);

bool GLES2_ParseExtension(const char *pExtName, const char *pExtensions);

int GLES2_GetMinorVersion();
int GLES2_GetMajorVersion();
int GLES2_IsVersionGEQ(int majorVersion, int minorVersion);

class OdTrVisRenderClient;
bool GLES2_Extensions_Initialize(OdTrVisRenderClient *pCtx, bool bThrowException = true);

#ifndef GL_ES_VERSION_2_0
#define GL_ES_VERSION_2_0 1
#endif



#endif // ODGLES2EXT
