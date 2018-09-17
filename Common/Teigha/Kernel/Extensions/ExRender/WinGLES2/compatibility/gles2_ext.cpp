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

#include <stdlib.h>
#include <string.h>
#include <stddef.h>

#include "OdaCommon.h"

#include "../../TrVisInfoString.h"
#include "../../TrVisRenderClient.h"

#include "gles2_ext.h"

// Function pointer extraction

#if defined(__APPLE__)
#include <mach-o/dyld.h>

static void* AppleGLGetProcAddress (const GLubyte *name)
{
  static const struct mach_header* image = NULL;
  NSSymbol symbol;
  char* symbolName;
  if (NULL == image)
  {
    image = NSAddImage("/System/Library/Frameworks/OpenGL.framework/Versions/Current/OpenGL", NSADDIMAGE_OPTION_RETURN_ON_ERROR);
  }
  /* prepend a '_' for the Unix C symbol mangling convention */
  symbolName = (char*)malloc(strlen((const char*)name) + 2);
  strcpy(symbolName+1, (const char*)name);
  symbolName[0] = '_';
  symbol = NULL;
  /* if (NSIsSymbolNameDefined(symbolName))
   symbol = NSLookupAndBindSymbol(symbolName); */
  symbol = image ? NSLookupSymbolInImage(image, symbolName, NSLOOKUPSYMBOLINIMAGE_OPTION_BIND | NSLOOKUPSYMBOLINIMAGE_OPTION_RETURN_ON_ERROR) : NULL;
  free(symbolName);
  return symbol ? NSAddressOfSymbol(symbol) : NULL;
}
#endif /* __APPLE__ */

#if defined(__sgi) || defined (__sun)
#include <dlfcn.h>
#include <stdio.h>

static void* SunGetProcAddress (const GLubyte* name)
{
  static void* h = NULL;
  static void* gpa;

  if (h == NULL)
  {
    if ((h = dlopen(NULL, RTLD_LAZY | RTLD_LOCAL)) == NULL) return NULL;
    gpa = dlsym(h, "glXGetProcAddress");
  }

  if (gpa != NULL)
    return ((void*(*)(const GLubyte*))gpa)(name);
  else
    return dlsym(h, (const char*)name);
}
#endif /* __sgi || __sun */

#if defined(_WIN32)

#ifdef _MSC_VER
#pragma warning(disable: 4055)
#pragma warning(disable: 4054)
#endif

static int TestPointer(const PROC pTest)
{
  ptrdiff_t iTest;
  if(!pTest) return 0;
  iTest = (ptrdiff_t)pTest;
  
  if(iTest == 1 || iTest == 2 || iTest == 3 || iTest == -1) return 0;
  
  return 1;
}

static PROC WinGetProcAddress(const char *name)
{
  HMODULE glMod = NULL;
  PROC pFunc = wglGetProcAddress((LPCSTR)name);
  if(TestPointer(pFunc))
  {
    return pFunc;
  }
  glMod = GetModuleHandleA("OpenGL32.dll");
  return (PROC)GetProcAddress(glMod, (LPCSTR)name);
}
  
#define IntGetProcAddress(name) WinGetProcAddress(name)
#else
  #if defined(__APPLE__)
    #define IntGetProcAddress(name) AppleGLGetProcAddress((const GLubyte*)name)
  #else
    #if defined(__sgi) || defined(__sun)
      #define IntGetProcAddress(name) SunGetProcAddress((const GLubyte*)name)
    #else /* GLX */
      #if !defined(GLX_GLXEXT_LEGACY)
        #define GLX_GLXEXT_LEGACY
      #endif
      #include <GL/glx.h>
      #ifndef GLX_ARB_get_proc_address
        #error glXGetProcAddressARB is not defined
      #endif

      #define IntGetProcAddress(name) (*glXGetProcAddressARB)((const GLubyte*)name)
    #endif
  #endif
#endif

// Extension functions implementation

enum IsCriticalLevel
{
  kCritical = 0,
  kDesirable,
  kMinor
};

typedef bool (PFN_GLES2_FuncInitializerErrorHandlerProc) (const char *pFuncName, void *pArg, IsCriticalLevel isCriticalLevel);

typedef const char * const GLES2_RegExt;
inline bool GLES2_RegExt_Has(GLES2_RegExt *pExts, const char *pExtName)
{ while (*pExts) { if (strcmp(*pExts, pExtName) == 0) return true; pExts++; } return false; }

#define GLES2_INIT_FUNCPTR_IMPL(ptrName, funcName, isCritical) \
  { \
    const char *pFuncName = funcName; \
    _ptr_##ptrName = (ptrName)IntGetProcAddress(pFuncName); \
    if (!_ptr_##ptrName) \
    { \
      if (!errorHandler(pFuncName, pArg, isCritical)) \
        return false; \
    } \
  }
#define GLES2_INIT_FUNCPTR(ptrName, funcName, isCritical) \
  GLES2_INIT_FUNCPTR_IMPL(ptrName, #funcName, isCritical)
#define GLES2_INIT_EMULPTR(ptrName, funcName, isCritical) \
  GLES2_INIT_FUNCPTR_IMPL(ptrName, #funcName, isCritical) \
  if (!_ptr_##ptrName) _ptr_##ptrName = funcName##_oda_emul;
#define GLES2_INIT_EXTRPTR(ptrName, funcName, isCritical, extName) \
  if (GLES2_RegExt_Has(pExts, extName)) \
    GLES2_INIT_FUNCPTR_IMPL(ptrName, #funcName, isCritical) \
  if (!_ptr_##ptrName) _ptr_##ptrName = funcName##_oda_emul;

#ifdef GL_VERSION_1_3_GLES2

static PFNGLACTIVETEXTUREPROC _ptr_PFNGLACTIVETEXTUREPROC = NULL;
void glActiveTexture_oda_ (GLenum texture)
{
  _ptr_PFNGLACTIVETEXTUREPROC(texture);
}

static PFNGLSAMPLECOVERAGEPROC _ptr_PFNGLSAMPLECOVERAGEPROC = NULL;
void glSampleCoverage_oda_ (GLfloat value, GLboolean invert)
{
  _ptr_PFNGLSAMPLECOVERAGEPROC(value, invert);
}

static PFNGLCOMPRESSEDTEXIMAGE2DPROC _ptr_PFNGLCOMPRESSEDTEXIMAGE2DPROC = NULL;
void glCompressedTexImage2D_oda_ (GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const void *data)
{
  _ptr_PFNGLCOMPRESSEDTEXIMAGE2DPROC(target, level, internalformat, width, height, border, imageSize, data);
}

static PFNGLCOMPRESSEDTEXSUBIMAGE2DPROC _ptr_PFNGLCOMPRESSEDTEXSUBIMAGE2DPROC = NULL;
void glCompressedTexSubImage2D_oda_ (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, const void *data)
{
  _ptr_PFNGLCOMPRESSEDTEXSUBIMAGE2DPROC(target, level, xoffset, yoffset, width, height, format, imageSize, data);
}

static bool GL_VERSION_1_3_GLES2_init(PFN_GLES2_FuncInitializerErrorHandlerProc *errorHandler, void *pArg, GLES2_RegExt* /*pExts*/)
{
  GLES2_INIT_FUNCPTR(PFNGLACTIVETEXTUREPROC          , glActiveTexture          , kCritical)
  GLES2_INIT_FUNCPTR(PFNGLSAMPLECOVERAGEPROC         , glSampleCoverage         , kMinor)
  GLES2_INIT_FUNCPTR(PFNGLCOMPRESSEDTEXIMAGE2DPROC   , glCompressedTexImage2D   , kMinor)
  GLES2_INIT_FUNCPTR(PFNGLCOMPRESSEDTEXSUBIMAGE2DPROC, glCompressedTexSubImage2D, kMinor)
  return true;
}

#endif // GL_VERSION_1_3_GLES2

#ifdef GL_VERSION_1_4_GLES2

static PFNGLBLENDFUNCSEPARATEPROC _ptr_PFNGLBLENDFUNCSEPARATEPROC = NULL;
void glBlendFuncSeparate_oda_ (GLenum sfactorRGB, GLenum dfactorRGB, GLenum sfactorAlpha, GLenum dfactorAlpha)
{
  _ptr_PFNGLBLENDFUNCSEPARATEPROC(sfactorRGB, dfactorRGB, sfactorAlpha, dfactorAlpha);
}
static void APIENTRY glBlendFuncSeparate_oda_emul (GLenum sfactorRGB, GLenum dfactorRGB, GLenum /*sfactorAlpha*/, GLenum /*dfactorAlpha*/)
{
  ODA_ASSERT_ONCE(false);
  ::glBlendFunc(sfactorRGB, dfactorRGB);
}

static PFNGLBLENDCOLORPROC _ptr_PFNGLBLENDCOLORPROC = NULL;
void glBlendColor_oda_ (GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha)
{
  _ptr_PFNGLBLENDCOLORPROC(red, green, blue, alpha);
}
static void APIENTRY glBlendColor_oda_emul (GLfloat /*red*/, GLfloat /*green*/, GLfloat /*blue*/, GLfloat /*alpha*/)
{
  ODA_ASSERT_ONCE(false);
}

static PFNGLBLENDEQUATIONPROC _ptr_PFNGLBLENDEQUATIONPROC = NULL;
void glBlendEquation_oda_ (GLenum mode)
{
  _ptr_PFNGLBLENDEQUATIONPROC(mode);
}
static void APIENTRY glBlendEquation_oda_emul (GLenum /*mode*/)
{
  ODA_ASSERT_ONCE(false);
}

static bool GL_VERSION_1_4_GLES2_init(PFN_GLES2_FuncInitializerErrorHandlerProc *errorHandler, void *pArg, GLES2_RegExt* /*pExts*/)
{
  GLES2_INIT_EMULPTR(PFNGLBLENDFUNCSEPARATEPROC, glBlendFuncSeparate, kDesirable)
  GLES2_INIT_EMULPTR(PFNGLBLENDCOLORPROC       , glBlendColor       , kDesirable)
  GLES2_INIT_EMULPTR(PFNGLBLENDEQUATIONPROC    , glBlendEquation    , kDesirable)
  return true;
}

#endif // GL_VERSION_1_4_GLES2

#ifdef GL_VERSION_1_5_GLES2

static PFNGLBINDBUFFERPROC _ptr_PFNGLBINDBUFFERPROC = NULL;
void glBindBuffer_oda_ (GLenum target, GLuint buffer)
{
  _ptr_PFNGLBINDBUFFERPROC(target, buffer);
}

static PFNGLDELETEBUFFERSPROC _ptr_PFNGLDELETEBUFFERSPROC = NULL;
void glDeleteBuffers_oda_ (GLsizei n, const GLuint *buffers)
{
  _ptr_PFNGLDELETEBUFFERSPROC(n, buffers);
}

static PFNGLGENBUFFERSPROC _ptr_PFNGLGENBUFFERSPROC = NULL;
void glGenBuffers_oda_ (GLsizei n, GLuint *buffers)
{
  _ptr_PFNGLGENBUFFERSPROC(n, buffers);
}

static PFNGLISBUFFERPROC _ptr_PFNGLISBUFFERPROC = NULL;
GLboolean glIsBuffer_oda_ (GLuint buffer)
{
  return _ptr_PFNGLISBUFFERPROC(buffer);
}

static PFNGLBUFFERDATAPROC _ptr_PFNGLBUFFERDATAPROC = NULL;
void glBufferData_oda_ (GLenum target, GLsizeiptr size, const void *data, GLenum usage)
{
  _ptr_PFNGLBUFFERDATAPROC(target, size, data, usage);
}

static PFNGLBUFFERSUBDATAPROC _ptr_PFNGLBUFFERSUBDATAPROC = NULL;
void glBufferSubData_oda_ (GLenum target, GLintptr offset, GLsizeiptr size, const void *data)
{
  _ptr_PFNGLBUFFERSUBDATAPROC(target, offset, size, data);
}

static PFNGLGETBUFFERPARAMETERIVPROC _ptr_PFNGLGETBUFFERPARAMETERIVPROC = NULL;
void glGetBufferParameteriv_oda_ (GLenum target, GLenum pname, GLint *params)
{
  _ptr_PFNGLGETBUFFERPARAMETERIVPROC(target, pname, params);
}

static bool GL_VERSION_1_5_GLES2_init(PFN_GLES2_FuncInitializerErrorHandlerProc *errorHandler, void *pArg, GLES2_RegExt* /*pExts*/)
{
  GLES2_INIT_FUNCPTR(PFNGLBINDBUFFERPROC          , glBindBuffer          , kCritical)
  GLES2_INIT_FUNCPTR(PFNGLDELETEBUFFERSPROC       , glDeleteBuffers       , kCritical)
  GLES2_INIT_FUNCPTR(PFNGLGENBUFFERSPROC          , glGenBuffers          , kCritical)
  GLES2_INIT_FUNCPTR(PFNGLISBUFFERPROC            , glIsBuffer            , kDesirable)
  GLES2_INIT_FUNCPTR(PFNGLBUFFERDATAPROC          , glBufferData          , kCritical)
  GLES2_INIT_FUNCPTR(PFNGLBUFFERSUBDATAPROC       , glBufferSubData       , kCritical)
  GLES2_INIT_FUNCPTR(PFNGLGETBUFFERPARAMETERIVPROC, glGetBufferParameteriv, kDesirable)
  return true;
}

#endif // GL_VERSION_1_5_GLES2

#ifdef GL_VERSION_2_0_GLES2

static PFNGLBLENDEQUATIONSEPARATEPROC _ptr_PFNGLBLENDEQUATIONSEPARATEPROC = NULL;
void glBlendEquationSeparate_oda_ (GLenum modeRGB, GLenum modeAlpha)
{
  _ptr_PFNGLBLENDEQUATIONSEPARATEPROC(modeRGB, modeAlpha);
}
static void APIENTRY glBlendEquationSeparate_oda_emul (GLenum modeRGB, GLenum /*modeAlpha*/)
{
  ODA_ASSERT_ONCE(false);
  ::glBlendEquation(modeRGB);
}

static PFNGLSTENCILOPSEPARATEPROC _ptr_PFNGLSTENCILOPSEPARATEPROC = NULL;
void glStencilOpSeparate_oda_ (GLenum face, GLenum sfail, GLenum dpfail, GLenum dppass)
{
  _ptr_PFNGLSTENCILOPSEPARATEPROC(face, sfail, dpfail, dppass);
}

static PFNGLSTENCILFUNCSEPARATEPROC _ptr_PFNGLSTENCILFUNCSEPARATEPROC = NULL;
void glStencilFuncSeparate_oda_ (GLenum face, GLenum func, GLint ref, GLuint mask)
{
  _ptr_PFNGLSTENCILFUNCSEPARATEPROC(face, func, ref, mask);
}

static PFNGLSTENCILMASKSEPARATEPROC _ptr_PFNGLSTENCILMASKSEPARATEPROC = NULL;
void glStencilMaskSeparate_oda_ (GLenum face, GLuint mask)
{
  _ptr_PFNGLSTENCILMASKSEPARATEPROC(face, mask);
}

static PFNGLATTACHSHADERPROC _ptr_PFNGLATTACHSHADERPROC = NULL;
void glAttachShader_oda_ (GLuint program, GLuint shader)
{
  _ptr_PFNGLATTACHSHADERPROC(program, shader);
}

static PFNGLBINDATTRIBLOCATIONPROC _ptr_PFNGLBINDATTRIBLOCATIONPROC = NULL;
void glBindAttribLocation_oda_ (GLuint program, GLuint index, const GLchar *name)
{
  _ptr_PFNGLBINDATTRIBLOCATIONPROC(program, index, name);
}

static PFNGLCOMPILESHADERPROC _ptr_PFNGLCOMPILESHADERPROC = NULL;
void glCompileShader_oda_ (GLuint shader)
{
  _ptr_PFNGLCOMPILESHADERPROC(shader);
}

static PFNGLCREATEPROGRAMPROC _ptr_PFNGLCREATEPROGRAMPROC = NULL;
GLuint glCreateProgram_oda_ (void)
{
  return _ptr_PFNGLCREATEPROGRAMPROC();
}

static PFNGLCREATESHADERPROC _ptr_PFNGLCREATESHADERPROC = NULL;
GLuint glCreateShader_oda_ (GLenum type)
{
  return _ptr_PFNGLCREATESHADERPROC(type);
}

static PFNGLDELETEPROGRAMPROC _ptr_PFNGLDELETEPROGRAMPROC = NULL;
void glDeleteProgram_oda_ (GLuint program)
{
  _ptr_PFNGLDELETEPROGRAMPROC(program);
}

static PFNGLDELETESHADERPROC _ptr_PFNGLDELETESHADERPROC = NULL;
void glDeleteShader_oda_ (GLuint shader)
{
  _ptr_PFNGLDELETESHADERPROC(shader);
}

static PFNGLDETACHSHADERPROC _ptr_PFNGLDETACHSHADERPROC = NULL;
void glDetachShader_oda_ (GLuint program, GLuint shader)
{
  _ptr_PFNGLDETACHSHADERPROC(program, shader);
}

static PFNGLDISABLEVERTEXATTRIBARRAYPROC _ptr_PFNGLDISABLEVERTEXATTRIBARRAYPROC = NULL;
void glDisableVertexAttribArray_oda_ (GLuint index)
{
  _ptr_PFNGLDISABLEVERTEXATTRIBARRAYPROC(index);
}

static PFNGLENABLEVERTEXATTRIBARRAYPROC _ptr_PFNGLENABLEVERTEXATTRIBARRAYPROC = NULL;
void glEnableVertexAttribArray_oda_ (GLuint index)
{
  _ptr_PFNGLENABLEVERTEXATTRIBARRAYPROC(index);
}

static PFNGLGETACTIVEATTRIBPROC _ptr_PFNGLGETACTIVEATTRIBPROC = NULL;
void glGetActiveAttrib_oda_ (GLuint program, GLuint index, GLsizei bufSize, GLsizei *length, GLint *size, GLenum *type, GLchar *name)
{
  _ptr_PFNGLGETACTIVEATTRIBPROC(program, index, bufSize, length, size, type, name);
}

static PFNGLGETACTIVEUNIFORMPROC _ptr_PFNGLGETACTIVEUNIFORMPROC = NULL;
void glGetActiveUniform_oda_ (GLuint program, GLuint index, GLsizei bufSize, GLsizei *length, GLint *size, GLenum *type, GLchar *name)
{
  _ptr_PFNGLGETACTIVEUNIFORMPROC(program, index, bufSize, length, size, type, name);
}

static PFNGLGETATTACHEDSHADERSPROC _ptr_PFNGLGETATTACHEDSHADERSPROC = NULL;
void glGetAttachedShaders_oda_ (GLuint program, GLsizei maxCount, GLsizei *count, GLuint *shaders)
{
  _ptr_PFNGLGETATTACHEDSHADERSPROC(program, maxCount, count, shaders);
}

static PFNGLGETATTRIBLOCATIONPROC _ptr_PFNGLGETATTRIBLOCATIONPROC = NULL;
GLint glGetAttribLocation_oda_ (GLuint program, const GLchar *name)
{
  return _ptr_PFNGLGETATTRIBLOCATIONPROC(program, name);
}

static PFNGLGETPROGRAMIVPROC _ptr_PFNGLGETPROGRAMIVPROC = NULL;
void glGetProgramiv_oda_ (GLuint program, GLenum pname, GLint *params)
{
  _ptr_PFNGLGETPROGRAMIVPROC(program, pname, params);
}

static PFNGLGETPROGRAMINFOLOGPROC _ptr_PFNGLGETPROGRAMINFOLOGPROC = NULL;
void glGetProgramInfoLog_oda_ (GLuint program, GLsizei bufSize, GLsizei *length, GLchar *infoLog)
{
  _ptr_PFNGLGETPROGRAMINFOLOGPROC(program, bufSize, length, infoLog);
}

static PFNGLGETSHADERIVPROC _ptr_PFNGLGETSHADERIVPROC = NULL;
void glGetShaderiv_oda_ (GLuint shader, GLenum pname, GLint *params)
{
  _ptr_PFNGLGETSHADERIVPROC(shader, pname, params);
}

static PFNGLGETSHADERINFOLOGPROC _ptr_PFNGLGETSHADERINFOLOGPROC = NULL;
void glGetShaderInfoLog_oda_ (GLuint shader, GLsizei bufSize, GLsizei *length, GLchar *infoLog)
{
  _ptr_PFNGLGETSHADERINFOLOGPROC(shader, bufSize, length, infoLog);
}

static PFNGLGETSHADERSOURCEPROC _ptr_PFNGLGETSHADERSOURCEPROC = NULL;
void glGetShaderSource_oda_ (GLuint shader, GLsizei bufSize, GLsizei *length, GLchar *source)
{
  _ptr_PFNGLGETSHADERSOURCEPROC(shader, bufSize, length, source);
}

static PFNGLGETUNIFORMLOCATIONPROC _ptr_PFNGLGETUNIFORMLOCATIONPROC = NULL;
GLint glGetUniformLocation_oda_ (GLuint program, const GLchar *name)
{
  return _ptr_PFNGLGETUNIFORMLOCATIONPROC(program, name);
}

static PFNGLGETUNIFORMFVPROC _ptr_PFNGLGETUNIFORMFVPROC = NULL;
void glGetUniformfv_oda_ (GLuint program, GLint location, GLfloat *params)
{
  _ptr_PFNGLGETUNIFORMFVPROC(program, location, params);
}

static PFNGLGETUNIFORMIVPROC _ptr_PFNGLGETUNIFORMIVPROC = NULL;
void glGetUniformiv_oda_ (GLuint program, GLint location, GLint *params)
{
  _ptr_PFNGLGETUNIFORMIVPROC(program, location, params);
}

static PFNGLGETVERTEXATTRIBFVPROC _ptr_PFNGLGETVERTEXATTRIBFVPROC = NULL;
void glGetVertexAttribfv_oda_ (GLuint index, GLenum pname, GLfloat *params)
{
  _ptr_PFNGLGETVERTEXATTRIBFVPROC(index, pname, params);
}

static PFNGLGETVERTEXATTRIBIVPROC _ptr_PFNGLGETVERTEXATTRIBIVPROC = NULL;
void glGetVertexAttribiv_oda_ (GLuint index, GLenum pname, GLint *params)
{
  _ptr_PFNGLGETVERTEXATTRIBIVPROC(index, pname, params);
}

static PFNGLGETVERTEXATTRIBPOINTERVPROC _ptr_PFNGLGETVERTEXATTRIBPOINTERVPROC = NULL;
void glGetVertexAttribPointerv_oda_ (GLuint index, GLenum pname, void **pointer)
{
  _ptr_PFNGLGETVERTEXATTRIBPOINTERVPROC(index, pname, pointer);
}

static PFNGLISPROGRAMPROC _ptr_PFNGLISPROGRAMPROC = NULL;
GLboolean glIsProgram_oda_ (GLuint program)
{
  return _ptr_PFNGLISPROGRAMPROC(program);
}

static PFNGLISSHADERPROC _ptr_PFNGLISSHADERPROC = NULL;
GLboolean glIsShader_oda_ (GLuint shader)
{
  return _ptr_PFNGLISSHADERPROC(shader);
}

static PFNGLLINKPROGRAMPROC _ptr_PFNGLLINKPROGRAMPROC = NULL;
void glLinkProgram_oda_ (GLuint program)
{
  _ptr_PFNGLLINKPROGRAMPROC(program);
}

static PFNGLSHADERSOURCEPROC _ptr_PFNGLSHADERSOURCEPROC = NULL;
void glShaderSource_oda_ (GLuint shader, GLsizei count, const GLchar *const*string, const GLint *length)
{
  _ptr_PFNGLSHADERSOURCEPROC(shader, count, string, length);
}

static PFNGLUSEPROGRAMPROC _ptr_PFNGLUSEPROGRAMPROC = NULL;
void glUseProgram_oda_ (GLuint program)
{
  _ptr_PFNGLUSEPROGRAMPROC(program);
}

static PFNGLUNIFORM1FPROC _ptr_PFNGLUNIFORM1FPROC = NULL;
void glUniform1f_oda_ (GLint location, GLfloat v0)
{
  _ptr_PFNGLUNIFORM1FPROC(location, v0);
}

static PFNGLUNIFORM2FPROC _ptr_PFNGLUNIFORM2FPROC = NULL;
void glUniform2f_oda_ (GLint location, GLfloat v0, GLfloat v1)
{
  _ptr_PFNGLUNIFORM2FPROC(location, v0, v1);
}

static PFNGLUNIFORM3FPROC _ptr_PFNGLUNIFORM3FPROC = NULL;
void glUniform3f_oda_ (GLint location, GLfloat v0, GLfloat v1, GLfloat v2)
{
  _ptr_PFNGLUNIFORM3FPROC(location, v0, v1, v2);
}

static PFNGLUNIFORM4FPROC _ptr_PFNGLUNIFORM4FPROC = NULL;
void glUniform4f_oda_ (GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3)
{
  _ptr_PFNGLUNIFORM4FPROC(location, v0, v1, v2, v3);
}

static PFNGLUNIFORM1IPROC _ptr_PFNGLUNIFORM1IPROC = NULL;
void glUniform1i_oda_ (GLint location, GLint v0)
{
  _ptr_PFNGLUNIFORM1IPROC(location, v0);
}

static PFNGLUNIFORM2IPROC _ptr_PFNGLUNIFORM2IPROC = NULL;
void glUniform2i_oda_ (GLint location, GLint v0, GLint v1)
{
  _ptr_PFNGLUNIFORM2IPROC(location, v0, v1);
}

static PFNGLUNIFORM3IPROC _ptr_PFNGLUNIFORM3IPROC = NULL;
void glUniform3i_oda_ (GLint location, GLint v0, GLint v1, GLint v2)
{
  _ptr_PFNGLUNIFORM3IPROC(location, v0, v1, v2);
}

static PFNGLUNIFORM4IPROC _ptr_PFNGLUNIFORM4IPROC = NULL;
void glUniform4i_oda_ (GLint location, GLint v0, GLint v1, GLint v2, GLint v3)
{
  _ptr_PFNGLUNIFORM4IPROC(location, v0, v1, v2, v3);
}

static PFNGLUNIFORM1FVPROC _ptr_PFNGLUNIFORM1FVPROC = NULL;
void glUniform1fv_oda_ (GLint location, GLsizei count, const GLfloat *value)
{
  _ptr_PFNGLUNIFORM1FVPROC(location, count, value);
}

static PFNGLUNIFORM2FVPROC _ptr_PFNGLUNIFORM2FVPROC = NULL;
void glUniform2fv_oda_ (GLint location, GLsizei count, const GLfloat *value)
{
  _ptr_PFNGLUNIFORM2FVPROC(location, count, value);
}

static PFNGLUNIFORM3FVPROC _ptr_PFNGLUNIFORM3FVPROC = NULL;
void glUniform3fv_oda_ (GLint location, GLsizei count, const GLfloat *value)
{
  _ptr_PFNGLUNIFORM3FVPROC(location, count, value);
}

static PFNGLUNIFORM4FVPROC _ptr_PFNGLUNIFORM4FVPROC = NULL;
void glUniform4fv_oda_ (GLint location, GLsizei count, const GLfloat *value)
{
  _ptr_PFNGLUNIFORM4FVPROC(location, count, value);
}

static PFNGLUNIFORM1IVPROC _ptr_PFNGLUNIFORM1IVPROC = NULL;
void glUniform1iv_oda_ (GLint location, GLsizei count, const GLint *value)
{
  _ptr_PFNGLUNIFORM1IVPROC(location, count, value);
}

static PFNGLUNIFORM2IVPROC _ptr_PFNGLUNIFORM2IVPROC = NULL;
void glUniform2iv_oda_ (GLint location, GLsizei count, const GLint *value)
{
  _ptr_PFNGLUNIFORM2IVPROC(location, count, value);
}

static PFNGLUNIFORM3IVPROC _ptr_PFNGLUNIFORM3IVPROC = NULL;
void glUniform3iv_oda_ (GLint location, GLsizei count, const GLint *value)
{
  _ptr_PFNGLUNIFORM3IVPROC(location, count, value);
}

static PFNGLUNIFORM4IVPROC _ptr_PFNGLUNIFORM4IVPROC = NULL;
void glUniform4iv_oda_ (GLint location, GLsizei count, const GLint *value)
{
  _ptr_PFNGLUNIFORM4IVPROC(location, count, value);
}

static PFNGLUNIFORMMATRIX2FVPROC _ptr_PFNGLUNIFORMMATRIX2FVPROC = NULL;
void glUniformMatrix2fv_oda_ (GLint location, GLsizei count, GLboolean transpose, const GLfloat *value)
{
  _ptr_PFNGLUNIFORMMATRIX2FVPROC(location, count, transpose, value);
}

static PFNGLUNIFORMMATRIX3FVPROC _ptr_PFNGLUNIFORMMATRIX3FVPROC = NULL;
void glUniformMatrix3fv_oda_ (GLint location, GLsizei count, GLboolean transpose, const GLfloat *value)
{
  _ptr_PFNGLUNIFORMMATRIX3FVPROC(location, count, transpose, value);
}

static PFNGLUNIFORMMATRIX4FVPROC _ptr_PFNGLUNIFORMMATRIX4FVPROC = NULL;
void glUniformMatrix4fv_oda_ (GLint location, GLsizei count, GLboolean transpose, const GLfloat *value)
{
  _ptr_PFNGLUNIFORMMATRIX4FVPROC(location, count, transpose, value);
}

static PFNGLVALIDATEPROGRAMPROC _ptr_PFNGLVALIDATEPROGRAMPROC = NULL;
void glValidateProgram_oda_ (GLuint program)
{
  _ptr_PFNGLVALIDATEPROGRAMPROC(program);
}

static PFNGLVERTEXATTRIB1FPROC _ptr_PFNGLVERTEXATTRIB1FPROC = NULL;
void glVertexAttrib1f_oda_ (GLuint index, GLfloat x)
{
  _ptr_PFNGLVERTEXATTRIB1FPROC(index, x);
}

static PFNGLVERTEXATTRIB1FVPROC _ptr_PFNGLVERTEXATTRIB1FVPROC = NULL;
void glVertexAttrib1fv_oda_ (GLuint index, const GLfloat *v)
{
  _ptr_PFNGLVERTEXATTRIB1FVPROC(index, v);
}

static PFNGLVERTEXATTRIB2FPROC _ptr_PFNGLVERTEXATTRIB2FPROC = NULL;
void glVertexAttrib2f_oda_ (GLuint index, GLfloat x, GLfloat y)
{
  _ptr_PFNGLVERTEXATTRIB2FPROC(index, x, y);
}

static PFNGLVERTEXATTRIB2FVPROC _ptr_PFNGLVERTEXATTRIB2FVPROC = NULL;
void glVertexAttrib2fv_oda_ (GLuint index, const GLfloat *v)
{
  _ptr_PFNGLVERTEXATTRIB2FVPROC(index, v);
}

static PFNGLVERTEXATTRIB3FPROC _ptr_PFNGLVERTEXATTRIB3FPROC = NULL;
void glVertexAttrib3f_oda_ (GLuint index, GLfloat x, GLfloat y, GLfloat z)
{
  _ptr_PFNGLVERTEXATTRIB3FPROC(index, x, y, z);
}

static PFNGLVERTEXATTRIB3FVPROC _ptr_PFNGLVERTEXATTRIB3FVPROC = NULL;
void glVertexAttrib3fv_oda_ (GLuint index, const GLfloat *v)
{
  _ptr_PFNGLVERTEXATTRIB3FVPROC(index, v);
}

static PFNGLVERTEXATTRIB4FPROC _ptr_PFNGLVERTEXATTRIB4FPROC = NULL;
void glVertexAttrib4f_oda_ (GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w)
{
  _ptr_PFNGLVERTEXATTRIB4FPROC(index, x, y, z, w);
}

static PFNGLVERTEXATTRIB4FVPROC _ptr_PFNGLVERTEXATTRIB4FVPROC = NULL;
void glVertexAttrib4fv_oda_ (GLuint index, const GLfloat *v)
{
  _ptr_PFNGLVERTEXATTRIB4FVPROC(index, v);
}

static PFNGLVERTEXATTRIBPOINTERPROC _ptr_PFNGLVERTEXATTRIBPOINTERPROC = NULL;
void glVertexAttribPointer_oda_ (GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void *pointer)
{
  _ptr_PFNGLVERTEXATTRIBPOINTERPROC(index, size, type, normalized, stride, pointer);
}

static bool GL_VERSION_2_0_GLES2_init(PFN_GLES2_FuncInitializerErrorHandlerProc *errorHandler, void *pArg, GLES2_RegExt* /*pExts*/)
{
  GLES2_INIT_EMULPTR(PFNGLBLENDEQUATIONSEPARATEPROC   , glBlendEquationSeparate   , kDesirable)
  GLES2_INIT_FUNCPTR(PFNGLSTENCILOPSEPARATEPROC       , glStencilOpSeparate       , kMinor)
  GLES2_INIT_FUNCPTR(PFNGLSTENCILFUNCSEPARATEPROC     , glStencilFuncSeparate     , kMinor)
  GLES2_INIT_FUNCPTR(PFNGLSTENCILMASKSEPARATEPROC     , glStencilMaskSeparate     , kMinor)
  GLES2_INIT_FUNCPTR(PFNGLATTACHSHADERPROC            , glAttachShader            , kCritical)
  GLES2_INIT_FUNCPTR(PFNGLBINDATTRIBLOCATIONPROC      , glBindAttribLocation      , kCritical)
  GLES2_INIT_FUNCPTR(PFNGLCOMPILESHADERPROC           , glCompileShader           , kCritical)
  GLES2_INIT_FUNCPTR(PFNGLCREATEPROGRAMPROC           , glCreateProgram           , kCritical)
  GLES2_INIT_FUNCPTR(PFNGLCREATESHADERPROC            , glCreateShader            , kCritical)
  GLES2_INIT_FUNCPTR(PFNGLDELETEPROGRAMPROC           , glDeleteProgram           , kCritical)
  GLES2_INIT_FUNCPTR(PFNGLDELETESHADERPROC            , glDeleteShader            , kCritical)
  GLES2_INIT_FUNCPTR(PFNGLDETACHSHADERPROC            , glDetachShader            , kDesirable)
  GLES2_INIT_FUNCPTR(PFNGLDISABLEVERTEXATTRIBARRAYPROC, glDisableVertexAttribArray, kCritical)
  GLES2_INIT_FUNCPTR(PFNGLENABLEVERTEXATTRIBARRAYPROC , glEnableVertexAttribArray , kCritical)
  GLES2_INIT_FUNCPTR(PFNGLGETACTIVEATTRIBPROC         , glGetActiveAttrib         , kDesirable)
  GLES2_INIT_FUNCPTR(PFNGLGETACTIVEUNIFORMPROC        , glGetActiveUniform        , kDesirable)
  GLES2_INIT_FUNCPTR(PFNGLGETATTACHEDSHADERSPROC      , glGetAttachedShaders      , kDesirable)
  GLES2_INIT_FUNCPTR(PFNGLGETATTRIBLOCATIONPROC       , glGetAttribLocation       , kCritical)
  GLES2_INIT_FUNCPTR(PFNGLGETPROGRAMIVPROC            , glGetProgramiv            , kDesirable)
  GLES2_INIT_FUNCPTR(PFNGLGETPROGRAMINFOLOGPROC       , glGetProgramInfoLog       , kCritical)
  GLES2_INIT_FUNCPTR(PFNGLGETSHADERIVPROC             , glGetShaderiv             , kDesirable)
  GLES2_INIT_FUNCPTR(PFNGLGETSHADERINFOLOGPROC        , glGetShaderInfoLog        , kCritical)
  GLES2_INIT_FUNCPTR(PFNGLGETSHADERSOURCEPROC         , glGetShaderSource         , kDesirable)
  GLES2_INIT_FUNCPTR(PFNGLGETUNIFORMLOCATIONPROC      , glGetUniformLocation      , kCritical)
  GLES2_INIT_FUNCPTR(PFNGLGETUNIFORMFVPROC            , glGetUniformfv            , kDesirable)
  GLES2_INIT_FUNCPTR(PFNGLGETUNIFORMIVPROC            , glGetUniformiv            , kDesirable)
  GLES2_INIT_FUNCPTR(PFNGLGETVERTEXATTRIBFVPROC       , glGetVertexAttribfv       , kDesirable)
  GLES2_INIT_FUNCPTR(PFNGLGETVERTEXATTRIBIVPROC       , glGetVertexAttribiv       , kDesirable)
  GLES2_INIT_FUNCPTR(PFNGLGETVERTEXATTRIBPOINTERVPROC , glGetVertexAttribPointerv , kCritical)
  GLES2_INIT_FUNCPTR(PFNGLISPROGRAMPROC               , glIsProgram               , kDesirable)
  GLES2_INIT_FUNCPTR(PFNGLISSHADERPROC                , glIsShader                , kDesirable)
  GLES2_INIT_FUNCPTR(PFNGLLINKPROGRAMPROC             , glLinkProgram             , kCritical)
  GLES2_INIT_FUNCPTR(PFNGLSHADERSOURCEPROC            , glShaderSource            , kCritical)
  GLES2_INIT_FUNCPTR(PFNGLUSEPROGRAMPROC              , glUseProgram              , kCritical)
  GLES2_INIT_FUNCPTR(PFNGLUNIFORM1FPROC               , glUniform1f               , kCritical)
  GLES2_INIT_FUNCPTR(PFNGLUNIFORM2FPROC               , glUniform2f               , kCritical)
  GLES2_INIT_FUNCPTR(PFNGLUNIFORM3FPROC               , glUniform3f               , kCritical)
  GLES2_INIT_FUNCPTR(PFNGLUNIFORM4FPROC               , glUniform4f               , kCritical)
  GLES2_INIT_FUNCPTR(PFNGLUNIFORM1IPROC               , glUniform1i               , kCritical)
  GLES2_INIT_FUNCPTR(PFNGLUNIFORM2IPROC               , glUniform2i               , kCritical)
  GLES2_INIT_FUNCPTR(PFNGLUNIFORM3IPROC               , glUniform3i               , kCritical)
  GLES2_INIT_FUNCPTR(PFNGLUNIFORM4IPROC               , glUniform4i               , kCritical)
  GLES2_INIT_FUNCPTR(PFNGLUNIFORM1FVPROC              , glUniform1fv              , kCritical)
  GLES2_INIT_FUNCPTR(PFNGLUNIFORM2FVPROC              , glUniform2fv              , kCritical)
  GLES2_INIT_FUNCPTR(PFNGLUNIFORM3FVPROC              , glUniform3fv              , kCritical)
  GLES2_INIT_FUNCPTR(PFNGLUNIFORM4FVPROC              , glUniform4fv              , kCritical)
  GLES2_INIT_FUNCPTR(PFNGLUNIFORM1IVPROC              , glUniform1iv              , kCritical)
  GLES2_INIT_FUNCPTR(PFNGLUNIFORM2IVPROC              , glUniform2iv              , kCritical)
  GLES2_INIT_FUNCPTR(PFNGLUNIFORM3IVPROC              , glUniform3iv              , kCritical)
  GLES2_INIT_FUNCPTR(PFNGLUNIFORM4IVPROC              , glUniform4iv              , kCritical)
  GLES2_INIT_FUNCPTR(PFNGLUNIFORMMATRIX2FVPROC        , glUniformMatrix2fv        , kCritical)
  GLES2_INIT_FUNCPTR(PFNGLUNIFORMMATRIX3FVPROC        , glUniformMatrix3fv        , kCritical)
  GLES2_INIT_FUNCPTR(PFNGLUNIFORMMATRIX4FVPROC        , glUniformMatrix4fv        , kCritical)
  GLES2_INIT_FUNCPTR(PFNGLVALIDATEPROGRAMPROC         , glValidateProgram         , kDesirable)
  GLES2_INIT_FUNCPTR(PFNGLVERTEXATTRIB1FPROC          , glVertexAttrib1f          , kCritical)
  GLES2_INIT_FUNCPTR(PFNGLVERTEXATTRIB1FVPROC         , glVertexAttrib1fv         , kCritical)
  GLES2_INIT_FUNCPTR(PFNGLVERTEXATTRIB2FPROC          , glVertexAttrib2f          , kCritical)
  GLES2_INIT_FUNCPTR(PFNGLVERTEXATTRIB2FVPROC         , glVertexAttrib2fv         , kCritical)
  GLES2_INIT_FUNCPTR(PFNGLVERTEXATTRIB3FPROC          , glVertexAttrib3f          , kCritical)
  GLES2_INIT_FUNCPTR(PFNGLVERTEXATTRIB3FVPROC         , glVertexAttrib3fv         , kCritical)
  GLES2_INIT_FUNCPTR(PFNGLVERTEXATTRIB4FPROC          , glVertexAttrib4f          , kCritical)
  GLES2_INIT_FUNCPTR(PFNGLVERTEXATTRIB4FVPROC         , glVertexAttrib4fv         , kCritical)
  GLES2_INIT_FUNCPTR(PFNGLVERTEXATTRIBPOINTERPROC     , glVertexAttribPointer     , kCritical)
  return true;
}

#endif // GL_VERSION_2_0_GLES2

#ifdef GL_VERSION_3_0_GLES2

static PFNGLISRENDERBUFFERPROC _ptr_PFNGLISRENDERBUFFERPROC = NULL;
GLboolean glIsRenderbuffer_oda_ (GLuint renderbuffer)
{
  return _ptr_PFNGLISRENDERBUFFERPROC(renderbuffer);
}

static PFNGLBINDRENDERBUFFERPROC _ptr_PFNGLBINDRENDERBUFFERPROC = NULL;
void glBindRenderbuffer_oda_ (GLenum target, GLuint renderbuffer)
{
  _ptr_PFNGLBINDRENDERBUFFERPROC(target, renderbuffer);
}

static PFNGLDELETERENDERBUFFERSPROC _ptr_PFNGLDELETERENDERBUFFERSPROC = NULL;
void glDeleteRenderbuffers_oda_ (GLsizei n, const GLuint *renderbuffers)
{
  _ptr_PFNGLDELETERENDERBUFFERSPROC(n, renderbuffers);
}

static PFNGLGENRENDERBUFFERSPROC _ptr_PFNGLGENRENDERBUFFERSPROC = NULL;
void glGenRenderbuffers_oda_ (GLsizei n, GLuint *renderbuffers)
{
  _ptr_PFNGLGENRENDERBUFFERSPROC(n, renderbuffers);
}

static PFNGLRENDERBUFFERSTORAGEPROC _ptr_PFNGLRENDERBUFFERSTORAGEPROC = NULL;
void glRenderbufferStorage_oda_ (GLenum target, GLenum internalformat, GLsizei width, GLsizei height)
{
  _ptr_PFNGLRENDERBUFFERSTORAGEPROC(target, internalformat, width, height);
}

static PFNGLGETRENDERBUFFERPARAMETERIVPROC _ptr_PFNGLGETRENDERBUFFERPARAMETERIVPROC = NULL;
void glGetRenderbufferParameteriv_oda_ (GLenum target, GLenum pname, GLint *params)
{
  _ptr_PFNGLGETRENDERBUFFERPARAMETERIVPROC(target, pname, params);
}

static PFNGLISFRAMEBUFFERPROC _ptr_PFNGLISFRAMEBUFFERPROC = NULL;
GLboolean glIsFramebuffer_oda_ (GLuint framebuffer)
{
  return _ptr_PFNGLISFRAMEBUFFERPROC(framebuffer);
}

static PFNGLBINDFRAMEBUFFERPROC _ptr_PFNGLBINDFRAMEBUFFERPROC = NULL;
void glBindFramebuffer_oda_ (GLenum target, GLuint framebuffer)
{
  _ptr_PFNGLBINDFRAMEBUFFERPROC(target, framebuffer);
}

static PFNGLDELETEFRAMEBUFFERSPROC _ptr_PFNGLDELETEFRAMEBUFFERSPROC = NULL;
void glDeleteFramebuffers_oda_ (GLsizei n, const GLuint *framebuffers)
{
  _ptr_PFNGLDELETEFRAMEBUFFERSPROC(n, framebuffers);
}

static PFNGLGENFRAMEBUFFERSPROC _ptr_PFNGLGENFRAMEBUFFERSPROC = NULL;
void glGenFramebuffers_oda_ (GLsizei n, GLuint *framebuffers)
{
  _ptr_PFNGLGENFRAMEBUFFERSPROC(n, framebuffers);
}

static PFNGLCHECKFRAMEBUFFERSTATUSPROC _ptr_PFNGLCHECKFRAMEBUFFERSTATUSPROC = NULL;
GLenum glCheckFramebufferStatus_oda_ (GLenum target)
{
  return _ptr_PFNGLCHECKFRAMEBUFFERSTATUSPROC(target);
}

static PFNGLFRAMEBUFFERTEXTURE2DPROC _ptr_PFNGLFRAMEBUFFERTEXTURE2DPROC = NULL;
void glFramebufferTexture2D_oda_ (GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level)
{
  _ptr_PFNGLFRAMEBUFFERTEXTURE2DPROC(target, attachment, textarget, texture, level);
}

static PFNGLFRAMEBUFFERRENDERBUFFERPROC _ptr_PFNGLFRAMEBUFFERRENDERBUFFERPROC = NULL;
void glFramebufferRenderbuffer_oda_ (GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer)
{
  _ptr_PFNGLFRAMEBUFFERRENDERBUFFERPROC(target, attachment, renderbuffertarget, renderbuffer);
}

static PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIVPROC _ptr_PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIVPROC = NULL;
void glGetFramebufferAttachmentParameteriv_oda_ (GLenum target, GLenum attachment, GLenum pname, GLint *params)
{
  _ptr_PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIVPROC(target, attachment, pname, params);
}

static PFNGLGENERATEMIPMAPPROC _ptr_PFNGLGENERATEMIPMAPPROC = NULL;
void glGenerateMipmap_oda_ (GLenum target)
{
  _ptr_PFNGLGENERATEMIPMAPPROC(target);
}

static bool GL_VERSION_3_0_GLES2_init(PFN_GLES2_FuncInitializerErrorHandlerProc *errorHandler, void *pArg, GLES2_RegExt* /*pExts*/)
{
  GLES2_INIT_FUNCPTR(PFNGLISRENDERBUFFERPROC                     , glIsRenderbuffer                     , kDesirable)
  GLES2_INIT_FUNCPTR(PFNGLBINDRENDERBUFFERPROC                   , glBindRenderbuffer                   , kDesirable)
  GLES2_INIT_FUNCPTR(PFNGLDELETERENDERBUFFERSPROC                , glDeleteRenderbuffers                , kDesirable)
  GLES2_INIT_FUNCPTR(PFNGLGENRENDERBUFFERSPROC                   , glGenRenderbuffers                   , kDesirable)
  GLES2_INIT_FUNCPTR(PFNGLRENDERBUFFERSTORAGEPROC                , glRenderbufferStorage                , kDesirable)
  GLES2_INIT_FUNCPTR(PFNGLGETRENDERBUFFERPARAMETERIVPROC         , glGetRenderbufferParameteriv         , kDesirable)
  GLES2_INIT_FUNCPTR(PFNGLISFRAMEBUFFERPROC                      , glIsFramebuffer                      , kDesirable)
  GLES2_INIT_FUNCPTR(PFNGLBINDFRAMEBUFFERPROC                    , glBindFramebuffer                    , kDesirable)
  GLES2_INIT_FUNCPTR(PFNGLDELETEFRAMEBUFFERSPROC                 , glDeleteFramebuffers                 , kDesirable)
  GLES2_INIT_FUNCPTR(PFNGLGENFRAMEBUFFERSPROC                    , glGenFramebuffers                    , kDesirable)
  GLES2_INIT_FUNCPTR(PFNGLCHECKFRAMEBUFFERSTATUSPROC             , glCheckFramebufferStatus             , kDesirable)
  GLES2_INIT_FUNCPTR(PFNGLFRAMEBUFFERTEXTURE2DPROC               , glFramebufferTexture2D               , kDesirable)
  GLES2_INIT_FUNCPTR(PFNGLFRAMEBUFFERRENDERBUFFERPROC            , glFramebufferRenderbuffer            , kDesirable)
  GLES2_INIT_FUNCPTR(PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIVPROC, glGetFramebufferAttachmentParameteriv, kDesirable)
  GLES2_INIT_FUNCPTR(PFNGLGENERATEMIPMAPPROC                     , glGenerateMipmap                     , kMinor)
  return true;
}

#endif // GL_VERSION_3_0_GLES2

#ifdef GL_VERSION_4_1_GLES2

static PFNGLRELEASESHADERCOMPILERPROC _ptr_PFNGLRELEASESHADERCOMPILERPROC = NULL;
void glReleaseShaderCompiler_oda_ (void)
{
  _ptr_PFNGLRELEASESHADERCOMPILERPROC();
}

static PFNGLSHADERBINARYPROC _ptr_PFNGLSHADERBINARYPROC = NULL;
void glShaderBinary_oda_ (GLsizei count, const GLuint *shaders, GLenum binaryformat, const void *binary, GLsizei length)
{
  _ptr_PFNGLSHADERBINARYPROC(count, shaders, binaryformat, binary, length);
}

static PFNGLGETSHADERPRECISIONFORMATPROC _ptr_PFNGLGETSHADERPRECISIONFORMATPROC = NULL;
void glGetShaderPrecisionFormat_oda_ (GLenum shadertype, GLenum precisiontype, GLint *range, GLint *precision)
{
  _ptr_PFNGLGETSHADERPRECISIONFORMATPROC(shadertype, precisiontype, range, precision);
}

static PFNGLDEPTHRANGEFPROC _ptr_PFNGLDEPTHRANGEFPROC = NULL;
void glDepthRangef_oda_ (GLfloat n, GLfloat f)
{
  _ptr_PFNGLDEPTHRANGEFPROC(n, f);
}
static void APIENTRY glDepthRangef_oda_emul (GLfloat n, GLfloat f)
{
  ::glDepthRange((GLclampd)n, (GLclampd)f);
}

static PFNGLCLEARDEPTHFPROC _ptr_PFNGLCLEARDEPTHFPROC = NULL;
void glClearDepthf_oda_ (GLfloat d)
{
  _ptr_PFNGLCLEARDEPTHFPROC(d);
}
static void APIENTRY glClearDepthf_oda_emul (GLfloat d)
{
  ::glClearDepth((GLclampd)d);
}

static bool GL_VERSION_4_1_GLES2_init(PFN_GLES2_FuncInitializerErrorHandlerProc *errorHandler, void *pArg, GLES2_RegExt *pExts)
{
  GLES2_INIT_FUNCPTR(PFNGLRELEASESHADERCOMPILERPROC       , glReleaseShaderCompiler   , kMinor)
  GLES2_INIT_FUNCPTR(PFNGLSHADERBINARYPROC                , glShaderBinary            , kMinor)
  GLES2_INIT_FUNCPTR(PFNGLGETSHADERPRECISIONFORMATPROC    , glGetShaderPrecisionFormat, kMinor)
  GLES2_INIT_EXTRPTR(PFNGLDEPTHRANGEFPROC                 , glDepthRangef             , kMinor, "GL_ARB_ES2_compatibility")
  GLES2_INIT_EXTRPTR(PFNGLCLEARDEPTHFPROC                 , glClearDepthf             , kMinor, "GL_ARB_ES2_compatibility")
  return true;
}

#endif // GL_VERSION_4_1_GLES2

// Extensions parser

void *GLES2_GetProcAddress(const char *pFuncName)
{
  return (void*)IntGetProcAddress(pFuncName);
}

bool GLES2_ParseExtension(const char *pExtName, const char *pExtensions)
{
  if (pExtName == NULL)
    return false;
  if (!pExtensions)
  {
    pExtensions = (const char*)::glGetString(GL_EXTENSIONS);
    if (!pExtensions)
      return false;
  }
  return OdTrVisInfoString::checkExtensionExternal(pExtensions, pExtName);
}

// Version parser

static int g_major_version = 0;
static int g_minor_version = 0;

static void ParseVersionFromString(int *pOutMajor, int *pOutMinor, const char *strVersion)
{
  const char *strDotPos = NULL;
  int iLength = 0;
  char strWorkBuff[10];
  *pOutMinor = 0;
  *pOutMajor = 0;

  strDotPos = strchr(strVersion, '.');
  if (!strDotPos)
    return;

  iLength = (int)((ptrdiff_t)strDotPos - (ptrdiff_t)strVersion);
  strncpy(strWorkBuff, strVersion, iLength);
  strWorkBuff[iLength] = '\0';

  *pOutMajor = atoi(strWorkBuff);
  strDotPos = strchr(strVersion + iLength + 1, ' ');
  if(!strDotPos)
  {
    /*No extra data. Take the whole rest of the string.*/
    strcpy(strWorkBuff, strVersion + iLength + 1);
  }
  else
  {
    /*Copy only up until the space.*/
    int iLengthMinor = (int)((ptrdiff_t)strDotPos - (ptrdiff_t)strVersion);
    iLengthMinor = iLengthMinor - (iLength + 1);
    strncpy(strWorkBuff, strVersion + iLength + 1, iLengthMinor);
    strWorkBuff[iLengthMinor] = '\0';
  }

  *pOutMinor = atoi(strWorkBuff);
}

static void GetGLVersion()
{
  ParseVersionFromString(&g_major_version, &g_minor_version, (const char*)glGetString(GL_VERSION));
}

int GLES2_GetMajorVersion()
{
  if (g_major_version == 0)
    GetGLVersion();
  return g_major_version;
}

int GLES2_GetMinorVersion()
{
  if (g_major_version == 0) //Yes, check the major version to get the minor one.
    GetGLVersion();
  return g_minor_version;
}

int GLES2_IsVersionGEQ(int majorVersion, int minorVersion)
{
  if (g_major_version == 0)
    GetGLVersion();
    
  if (majorVersion > g_major_version) return 1;
  if (majorVersion < g_major_version) return 0;
  if (minorVersion >= g_minor_version) return 1;
  return 0;
}

// GLES2 extensions

typedef struct GLES2_ExtMap_s
{
  const char *extensionName;
  const char *aliasExtensionName;
  IsCriticalLevel isCriticalLevel; // kCritical - throws OdError exception since we can't continue without it.
                                   // kDesirable - warning; this extension can be useful, but we still can work without it.
                                   // kMinor - we can silently continue work without this extension.
} GLES2_ExtMap;

static const GLES2_ExtMap g_ExtensionMap[] = {
  { "GL_ARB_ES2_compatibility"      , NULL                            , kMinor }, // Minor since all required functions can be emulated
  { "GL_ARB_vertex_shader"          , NULL                            , kCritical },
  { "GL_ARB_fragment_shader"        , NULL                            , kCritical },
  { "GL_ARB_shader_objects"         , NULL                            , kCritical },
  { "GL_ARB_vertex_buffer_object"   , NULL                            , kCritical },
  { "GL_ARB_framebuffer_object"     , "GL_EXT_framebuffer_object"     , kDesirable },
  { "GL_ARB_multisample"            , NULL                            , kDesirable },
  { "GL_ARB_multitexture"           , NULL                            , kCritical },
  { "GL_ARB_texture_compression"    , NULL                            , kDesirable },
  { "GL_ARB_texture_cube_map"       , "GL_EXT_texture_cube_map"       , kDesirable },
  { "GL_ARB_texture_env_add"        , "GL_EXT_texture_env_add"        , kMinor },
  { "GL_ARB_texture_mirrored_repeat", "GL_IBM_texture_mirrored_repeat", kDesirable },
  { "GL_EXT_texture_edge_clamp"     , NULL                            , kDesirable },
  { "GL_EXT_blend_color"            , NULL                            , kMinor },
  { "GL_EXT_blend_equation_separate", NULL                            , kMinor },
  { "GL_EXT_blend_func_separate"    , NULL                            , kDesirable },
  { "GL_EXT_blend_subtract"         , NULL                            , kMinor },
  { "GL_ATI_separate_stencil"       , NULL                            , kMinor },
                                                             
  { NULL                            , NULL                            , kMinor }
};

// Error handler

class GLES2ExtErrorHandler
{
  protected:
    OdAnsiString m_warns;
    OdAnsiString m_errs;
  public:
    GLES2ExtErrorHandler() { }

    static bool FuncInitializerErrorHandlerProc(const char *pFuncName, void *pArg, IsCriticalLevel isCriticalLevel)
    {
      GLES2ExtErrorHandler *pHandler = reinterpret_cast<GLES2ExtErrorHandler*>(pArg);
      if (isCriticalLevel == kMinor)
        return true;
      OdAnsiString *pOut = (isCriticalLevel == kCritical) ? &pHandler->m_errs : &pHandler->m_warns;
      if (!pOut->isEmpty())
        *pOut += ";\n";
      *pOut += pFuncName;
      *pOut += " function not found";
      return true;
    }

    void ExtensionErrorHandlerProc(const char *pFuncName, IsCriticalLevel nCriticalLevel)
    {
      if (nCriticalLevel == kMinor)
        return; // Silently skip this error level
      OdAnsiString *pOut = (nCriticalLevel == kCritical) ? &m_errs : &m_warns;
      if (!pOut->isEmpty())
        *pOut += ";\n";
      *pOut += pFuncName;
      *pOut += " extension not found";
    }

    bool emitError(OdTrVisRenderClient *pCtx, bool bThrowException)
    {
      if (m_errs.isEmpty())
        return false;
      if (bThrowException)
        pCtx->emitError(m_errs.c_str());
      else
        pCtx->emitWarning(m_errs.c_str());
      return true;
    }
    bool emitWarn(OdTrVisRenderClient *pCtx, bool bThrowWarning)
    {
      if (m_warns.isEmpty())
        return false;
      if (bThrowWarning)
        pCtx->emitWarning(m_warns.c_str());
      return true;
    }
};

class GLES2ExtMapDtr
{
  protected:
    OdVector<const char *, OdMemoryAllocator<const char*> > m_regExts;
  public:
    GLES2ExtMapDtr() : m_regExts(50, 10) {}
    void regExt(const char *pExt) { m_regExts.push_back(pExt); }
    void finalize() { m_regExts.push_back(NULL); }
    operator GLES2_RegExt*() const { return m_regExts.getPtr(); }
};

// Main extensions initializer

bool GLES2_Extensions_Initialize(OdTrVisRenderClient *pCtx, bool bThrowException)
{
  GLES2ExtErrorHandler _handler;
  GLES2ExtMapDtr _extRegs;
  { // Run for extensions
    const char *pExtensions = (const char*)::glGetString(GL_EXTENSIONS);
    const GLES2_ExtMap *pProc = g_ExtensionMap;
    while (pProc->extensionName)
    {
      if (!GLES2_ParseExtension(pProc->extensionName, pExtensions) &&
          (!pProc->aliasExtensionName ||
           !GLES2_ParseExtension(pProc->aliasExtensionName, pExtensions)))
        _handler.ExtensionErrorHandlerProc(pProc->extensionName, pProc->isCriticalLevel);
      else _extRegs.regExt(pProc->extensionName);
      pProc++;
    }
    _extRegs.finalize();
  }
  if (_handler.emitError(pCtx, bThrowException))
    return false;
  { // Run for functions
#ifdef GL_VERSION_1_3_GLES2
    GL_VERSION_1_3_GLES2_init(GLES2ExtErrorHandler::FuncInitializerErrorHandlerProc, &_handler, _extRegs);
#endif
#ifdef GL_VERSION_1_4_GLES2
    GL_VERSION_1_4_GLES2_init(GLES2ExtErrorHandler::FuncInitializerErrorHandlerProc, &_handler, _extRegs);
#endif
#ifdef GL_VERSION_1_5_GLES2
    GL_VERSION_1_5_GLES2_init(GLES2ExtErrorHandler::FuncInitializerErrorHandlerProc, &_handler, _extRegs);
#endif
#ifdef GL_VERSION_2_0_GLES2
    GL_VERSION_2_0_GLES2_init(GLES2ExtErrorHandler::FuncInitializerErrorHandlerProc, &_handler, _extRegs);
#endif
#ifdef GL_VERSION_3_0_GLES2
    GL_VERSION_3_0_GLES2_init(GLES2ExtErrorHandler::FuncInitializerErrorHandlerProc, &_handler, _extRegs);
#endif
#ifdef GL_VERSION_4_1_GLES2
    GL_VERSION_4_1_GLES2_init(GLES2ExtErrorHandler::FuncInitializerErrorHandlerProc, &_handler, _extRegs);
#endif
  }
  _handler.emitWarn(pCtx, bThrowException);
  return !_handler.emitError(pCtx, bThrowException);
}

//
