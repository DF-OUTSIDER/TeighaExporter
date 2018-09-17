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

// OpenGL ES helpers utility.
// This header should be included after TD_GL to avoid redefinitions in gl.h.

// File defines methods and definitions which is excluded in OpenGL ES: http://www.khronos.org/opengles/spec/

#ifndef ODOGLES_INCLUDED
#define ODOGLES_INCLUDED

#ifdef TD_OPENGL_ES // Main ES definition

// OpenGL ES doesn't support line/polygon stippling, so enable alternative highlighting method
#ifndef OD_OGL_ALTERNATIVEHLT
#define OD_OGL_ALTERNATIVEHLT
#endif

// TD_OPENGL_ES must be defined before, but OD_OGL_ES corresponds to code here
#ifndef OD_OGL_ES
#define OD_OGL_ES
#endif

// Enable emulation mode to simplify porting on Windows
#ifdef WIN32
#define OD_OGL_EMULATE_ES
#else
// Enable GL_VERSION_1_1 specification
#ifndef GL_VERSION_1_1
#define GL_VERSION_1_1
#endif
#endif

// Forward declaration for extensions parser
bool OpenGLParseExtension(const char *pExtName);

#ifdef OD_OGL_EMULATE_ES
#define OD_OGLES_EMUL_DECL(x) x##_
#else
#define OD_OGLES_EMUL_DECL(x) x
#endif
#define OD_OGLES_EMUL_SET(x) OD_OGLES_EMUL_DECL(x)

// GLdouble doesn't defined in OpenGL ES, make it compatible with GLfloat
#ifndef OD_OGL_EMULATE_ES
typedef GLfloat GLdouble;
#endif

// There is no line/polygon stipple supported on OpenGL ES, so define this modes as zero.
// glEnable/glDisable/glIsEnabled will skip this parameter, but shouldn't crash (elsewhere we could redirect to filter call)
#ifdef OD_OGL_EMULATE_ES
#undef GL_LINE_STIPPLE
#undef GL_POLYGON_STIPPLE
#endif
#ifndef GL_LINE_STIPPLE
#define GL_LINE_STIPPLE    0
#endif
#ifndef GL_POLYGON_STIPPLE
#define GL_POLYGON_STIPPLE 0
#endif

// There is no texture generation supported on OpenGL ES, so define this modes as zero.
// glEnable/glDisable/glIsEnabled will skip this parameter, but shouldn't crash (elsewhere we could redirect to filter call)
#ifdef OD_OGL_EMULATE_ES
#undef GL_TEXTURE_GEN_S
#undef GL_TEXTURE_GEN_T
#endif
#ifndef GL_TEXTURE_GEN_S
#define GL_TEXTURE_GEN_S   0
#endif
#ifndef GL_TEXTURE_GEN_T
#define GL_TEXTURE_GEN_T   0
#endif

// Android-specifics
#ifdef ANDROID

// Wraps functionality unavailable in emulator (and 2.2/2.3 devices).
// This functionality works under Android 3.2 devices, but we can still wrap it because it doesn't make things worse.
#ifndef ANDROID_EMULATOR
#define ANDROID_EMULATOR
#endif

#ifdef ANDROID_EMULATOR

// glColor4ub
inline void glColor4ub_Android(GLubyte red, GLubyte green, GLubyte blue, GLubyte alpha)
{
  ::glColor4x(((GLfixed)red) << 8, ((GLfixed)green) << 8, ((GLfixed)blue) << 8, ((GLfixed)alpha) << 8);
}
#ifdef glColor4ub
#undef glColor4ub
#endif
#define glColor4ub glColor4ub_Android

#if 0 // Unavailable emulator API, but real device should call it
// glTexEnvi
inline void glTexEnvi_Android(GLenum /*target*/, GLenum /*pname*/, GLint /*param*/)
{
  // cause problems
  //::glTexEnvx(target, pname, (GLfixed)param);
}
#ifdef glTexEnvi
#undef glTexEnvi
#endif
#define glTexEnvi glTexEnvi_Android
#endif // 0

// glGetBooleanv
inline void glGetBooleanv_Android(GLenum pname, GLboolean *params)
{
  GLint tmpParams = 0;
  ::glGetIntegerv(pname, &tmpParams);
  *params = (GLboolean)tmpParams;
}
#ifdef glGetBooleanv
#undef glGetBooleanv
#endif
#define glGetBooleanv glGetBooleanv_Android

// glGetFloatv
inline void glGetFloatv_Android(GLenum pname, GLfloat *params)
{
  static bool bOESExtParsed = false;
  static bool bOESExtSupported = false;
  switch (pname)
  {
    case GL_MODELVIEW_MATRIX:
#ifdef GL_OES_matrix_get
      if (!bOESExtParsed)
      {
        bOESExtParsed = true;
        bOESExtSupported = ::OpenGLParseExtension("GL_OES_matrix_get");
      }
      if (bOESExtSupported)
        ::glGetIntegerv(GL_MODELVIEW_MATRIX_FLOAT_AS_INT_BITS_OES, (GLint*)params);
      else
        ::glGetFloatv(GL_MODELVIEW_MATRIX, params);
#else
      ::glGetFloatv(GL_MODELVIEW_MATRIX, params);
#endif
    break;
    case GL_PROJECTION_MATRIX:
#ifdef GL_OES_matrix_get
      if (!bOESExtParsed)
      {
        bOESExtParsed = true;
        bOESExtSupported = ::OpenGLParseExtension("GL_OES_matrix_get");
      }
      if (bOESExtSupported)
        ::glGetIntegerv(GL_PROJECTION_MATRIX_FLOAT_AS_INT_BITS_OES, (GLint*)params);
      else
        ::glGetFloatv(GL_PROJECTION_MATRIX, params);
#else
      ::glGetFloatv(GL_PROJECTION_MATRIX, params);
#endif
    break;
    case GL_SMOOTH_LINE_WIDTH_RANGE:
    case GL_SMOOTH_POINT_SIZE_RANGE:
      {
        GLint tmpVal[2] = { 1, 1 };
        ::glGetIntegerv(pname, tmpVal);
        params[0] = (GLfloat)tmpVal[0];
        params[1] = (GLfloat)tmpVal[1];
      }
    break;
    case GL_POINT_SIZE:
    case GL_LINE_WIDTH:
      {
        GLint tmpVal = 1;
        ::glGetIntegerv(pname, &tmpVal);
        *params = (GLfloat)tmpVal;
      }
    break;
  }
}
#ifdef glGetFloatv
#undef glGetFloatv
#endif
#define glGetFloatv glGetFloatv_Android

/** \details
  Support for glIsEnabled, require changes for glEnable/glDisable
  <group ExRender_Classes>
*/
struct glIsEnabled_Android_Helpers
{
  enum Attribs
  {
    Attr_StencilTest = 0,
    Attr_Lighting,
    Attr_Fog,
    Attr_CullFace,
    Attr_Texture,
    Attr_DepthTest,
    Attr_Blend,
    Attr_LineSmoothing,
    Attr_nAttrs
  };
  static GLboolean *glDataArrayForAttribs_Android()
  {
    static GLboolean attrs[Attr_nAttrs] = { GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE };
    return attrs;
  }
  static GLboolean *glenumToAttrib(GLenum cap)
  {
    switch (cap)
    {
      case GL_STENCIL_TEST:
      return glDataArrayForAttribs_Android() + Attr_StencilTest;
      case GL_LIGHTING:
      return glDataArrayForAttribs_Android() + Attr_Lighting;
      case GL_FOG:
      return glDataArrayForAttribs_Android() + Attr_Fog;
      case GL_CULL_FACE:
      return glDataArrayForAttribs_Android() + Attr_CullFace;
      case GL_TEXTURE_2D:
      return glDataArrayForAttribs_Android() + Attr_Texture;
      case GL_DEPTH_TEST:
      return glDataArrayForAttribs_Android() + Attr_DepthTest;
      case GL_BLEND:
      return glDataArrayForAttribs_Android() + Attr_Blend;
      case GL_LINE_SMOOTH:
      return glDataArrayForAttribs_Android() + Attr_LineSmoothing;
    }
    return NULL;
  }
  static void glEnableHelper(GLenum cap)
  {
    if (cap == 0) return;
    GLboolean *pVal = glenumToAttrib(cap);
    if (pVal != NULL) *pVal = GL_TRUE;
    ::glEnable(cap);
  }
  static void glDisableHelper(GLenum cap)
  {
    if (cap == 0) return;
    GLboolean *pVal = glenumToAttrib(cap);
    if (pVal != NULL) *pVal = GL_FALSE;
    ::glDisable(cap);
  }
  static GLboolean glIsEnabledHelper(GLenum cap)
  {
    if (cap == 0) return GL_FALSE;
    GLboolean *pVal = glenumToAttrib(cap);
    if (pVal != NULL) return *pVal;
    return GL_FALSE;
  }
};

// glEnable
inline void glEnable_Android(GLenum cap)
{
  glIsEnabled_Android_Helpers::glEnableHelper(cap);
}
// glDisable
inline void glDisable_Android(GLenum cap)
{
  glIsEnabled_Android_Helpers::glDisableHelper(cap);
}
// glIsEnabled
inline GLboolean glIsEnabled_Android(GLenum cap)
{
  return glIsEnabled_Android_Helpers::glIsEnabledHelper(cap);
}
#ifdef glEnable
#undef glEnable
#endif
#define glEnable glEnable_Android
#ifdef glDisable
#undef glDisable
#endif
#define glDisable glDisable_Android
#ifdef glIsEnabled
#undef glIsEnabled
#endif
#define glIsEnabled glIsEnabled_Android

/* Known emulator issues:
   1) getInteger:GL_CULL_FACE_MODE doesn't work. Could be emulated through redefines as for glIsEnabled. (for 2.3 emulator only, 3.2 emulator supports)
   2) Stencil buffer doesn't work. It could be solved only on Java layer for real device. (hardware drivers supports it)
   3) Material color doesn't work (check for real device. Could be emulated through vertex color). (no problem in 3.2 emulator, available only in 2.3 and early)
   4) PolygonOffset works incorrectly (check for real device). Couldn't be solved w/o driver support. (no problem on real devices)
*/

#endif // ANDROID_EMULATOR

#endif // @eof Android-specific

// For both iOS and Android (generic OpenGL ES porting based on OpenGL specification)

// glColor4ubv
inline void OD_OGLES_EMUL_DECL(glColor4ubv)(const GLubyte *pData)
{
  ::glColor4ub(pData[0], pData[1], pData[2], pData[3]);
}
#define glColor4ubv OD_OGLES_EMUL_SET(glColor4ubv)

// glColor3ub
inline void OD_OGLES_EMUL_DECL(glColor3ub)(GLubyte red, GLubyte green, GLubyte blue)
{
  ::glColor4ub(red, green, blue, 255);
}
#define glColor3ub OD_OGLES_EMUL_SET(glColor3ub)

// glLineStipple
inline void OD_OGLES_EMUL_DECL(glLineStipple)(GLint /*factor*/, GLushort /*pattern*/)
{
  // Does nothing
}
#define glLineStipple OD_OGLES_EMUL_SET(glLineStipple)

// glPolygonStipple
inline void OD_OGLES_EMUL_DECL(glPolygonStipple)(const GLubyte * /*mask*/)
{
  // Does nothing
}
#define glPolygonStipple OD_OGLES_EMUL_SET(glPolygonStipple)

// Polygon could be rendered as triangle fan in our cases
#ifdef OD_OGL_EMULATE_ES
#undef GL_POLYGON
#endif
#ifndef GL_POLYGON
#define GL_POLYGON GL_TRIANGLE_FAN
#endif

// Call lists doesn't supported

// glDeleteLists
inline void OD_OGLES_EMUL_DECL(glDeleteLists)(GLuint /*list*/, GLsizei /*range*/)
{
  // Does nothing
}
#define glDeleteLists OD_OGLES_EMUL_SET(glDeleteLists)

// glNewList
inline void OD_OGLES_EMUL_DECL(glNewList)(GLuint /*list*/, GLenum /*mode*/)
{
  // Does nothing
}
#define glNewList OD_OGLES_EMUL_SET(glNewList)

// glGenLists
inline GLuint OD_OGLES_EMUL_DECL(glGenLists)(GLsizei /*range*/)
{
  // Does nothing
  return 0;
}
#define glGenLists OD_OGLES_EMUL_SET(glGenLists)

// glEndList
inline void OD_OGLES_EMUL_DECL(glEndList)()
{
  // Does nothing
}
#define glEndList OD_OGLES_EMUL_SET(glEndList)

// glCallList
inline void OD_OGLES_EMUL_DECL(glCallList)(GLuint /*list*/)
{
  // Does nothing
}
#define glCallList OD_OGLES_EMUL_SET(glCallList)

#ifdef OD_OGL_EMULATE_ES
#undef GL_COMPILE
#endif
#ifndef GL_COMPILE
#define GL_COMPILE 0
#endif

//

// glLightModeli
inline void OD_OGLES_EMUL_DECL(glLightModeli)(GLenum pname, GLint param)
{
#ifndef OD_OGL_EMULATE_ES
  ::glLightModelx(pname, param);
#else
  ::glLightModeli(pname, param);
#endif
}
#define glLightModeli OD_OGLES_EMUL_SET(glLightModeli)

// glColorMaterial
inline void OD_OGLES_EMUL_DECL(glColorMaterial)(GLenum /*face*/, GLenum /*mode*/)
{
  // Does nothing
}
#define glColorMaterial OD_OGLES_EMUL_SET(glColorMaterial)

// glBegin
inline void OD_OGLES_EMUL_DECL(glBegin)(GLenum /*mode*/)
{
  // Does nothing
}
#define glBegin OD_OGLES_EMUL_SET(glBegin)

// glEnd
inline void OD_OGLES_EMUL_DECL(glEnd)()
{
  // Does nothing
}
#define glEnd OD_OGLES_EMUL_SET(glEnd)

// Texture wrapping
#ifndef OD_OGL_EMULATE_ES
#ifndef GL_CLAMP
#define GL_CLAMP GL_CLAMP_TO_EDGE
#endif
#endif

// glFogi
inline void OD_OGLES_EMUL_DECL(glFogi)(GLenum pname, GLint param)
{
#ifndef OD_OGL_EMULATE_ES
  ::glFogx(pname, param);
#else
  ::glFogi(pname, param);
#endif
}
#define glFogi OD_OGLES_EMUL_SET(glFogi)

// glClearDepth
inline void OD_OGLES_EMUL_DECL(glClearDepth)(double depth)
{
#ifndef OD_OGL_EMULATE_ES
  ::glClearDepthf((GLfloat)depth);
#else
  ::glClearDepth(depth);
#endif
}
#define glClearDepth OD_OGLES_EMUL_SET(glClearDepth)

// glGetDoublev
inline void OD_OGLES_EMUL_DECL(glGetDoublev)(GLenum pname, GLdouble *params)
{
#ifndef OD_OGL_EMULATE_ES
  ::glGetFloatv(pname, params);
#else
  ::glGetDoublev(pname, params);
#endif
}
#define glGetDoublev OD_OGLES_EMUL_SET(glGetDoublev)

// glLoadMatrixd
inline void OD_OGLES_EMUL_DECL(glLoadMatrixd)(const GLdouble *m)
{
#ifndef OD_OGL_EMULATE_ES
  ::glLoadMatrixf(m);
#else
  ::glLoadMatrixd(m);
#endif
}
#define glLoadMatrixd OD_OGLES_EMUL_SET(glLoadMatrixd)

#endif // TD_OPENGL_ES

#endif // ODOGLES_INCLUDED
