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
// Wrap OpenGL ES calls to log output data

#ifndef _h_androidwrap
#define _h_androidwrap

#include <android/log.h>

#define OGL_LOG_TAG "TeighaOpenGL"
#define OGL_LOG(...) __android_log_print(ANDROID_LOG_INFO, OGL_LOG_TAG, __VA_ARGS__)

#define OGL_WRAP_GLMTH_0(funcName) \
  inline void funcName##_logwrap() \
  { \
    OGL_LOG("--> %s();", #funcName); \
    ::funcName(); \
    OGL_LOG("<-- %s() = %u", #funcName, ::glGetError()); \
  }
#define OGL_WRAP_GLMTH_1(funcName, p1Type, p1Name, p1Mod) \
  inline void funcName##_logwrap(p1Type p1Name) \
  { \
    OGL_LOG("--> %s(" p1Mod ");", #funcName, p1Name); \
    ::funcName(p1Name); \
    OGL_LOG("<-- %s() = %u", #funcName, ::glGetError()); \
  }
#define OGL_WRAP_GLMTH_1_RVAL(funcName, p1Type, p1Name, p1Mod, rTyp) \
  inline rTyp funcName##_logwrap(p1Type p1Name) \
  { \
    OGL_LOG("--> %s(" p1Mod ");", #funcName, p1Name); \
    rTyp tmpRval = ::funcName(p1Name); \
    OGL_LOG("<-- %s(%u) = %u", #funcName, (GLuint)tmpRval, ::glGetError()); \
    return tmpRval; \
  }
#define OGL_WRAP_GLMTH_2(funcName, p1Type, p1Name, p1Mod, p2Type, p2Name, p2Mod) \
  inline void funcName##_logwrap(p1Type p1Name, p2Type p2Name) \
  { \
    OGL_LOG("--> %s(" p1Mod ", " p2Mod ");", #funcName, p1Name, p2Name); \
    ::funcName(p1Name, p2Name); \
    OGL_LOG("<-- %s() = %u", #funcName, ::glGetError()); \
  }
#define OGL_WRAP_GLMTH_3(funcName, p1Type, p1Name, p1Mod, p2Type, p2Name, p2Mod, p3Type, p3Name, p3Mod) \
  inline void funcName##_logwrap(p1Type p1Name, p2Type p2Name, p3Type p3Name) \
  { \
    OGL_LOG("--> %s(" p1Mod ", " p2Mod ", " p3Mod ");", #funcName, p1Name, p2Name, p3Name); \
    ::funcName(p1Name, p2Name, p3Name); \
    OGL_LOG("<-- %s() = %u", #funcName, ::glGetError()); \
  }
#define OGL_WRAP_GLMTH_4(funcName, p1Type, p1Name, p1Mod, p2Type, p2Name, p2Mod, p3Type, p3Name, p3Mod, p4Type, p4Name, p4Mod) \
  inline void funcName##_logwrap(p1Type p1Name, p2Type p2Name, p3Type p3Name, p4Type p4Name) \
  { \
    OGL_LOG("--> %s(" p1Mod ", " p2Mod ", " p3Mod ", " p4Mod ");", #funcName, p1Name, p2Name, p3Name, p4Name); \
    ::funcName(p1Name, p2Name, p3Name, p4Name); \
    OGL_LOG("<-- %s() = %u", #funcName, ::glGetError()); \
  }
#define OGL_WRAP_GLMTH_4_UB(funcName, p1Type, p1Name, p1Mod, p2Type, p2Name, p2Mod, p3Type, p3Name, p3Mod, p4Type, p4Name, p4Mod) \
  inline void funcName##_logwrap(p1Type p1Name, p2Type p2Name, p3Type p3Name, p4Type p4Name) \
  { \
    OGL_LOG("--> %s(" p1Mod ", " p2Mod ", " p3Mod ", " p4Mod ");", #funcName, (GLuint)p1Name, (GLuint)p2Name, (GLuint)p3Name, (GLuint)p4Name); \
    ::funcName(p1Name, p2Name, p3Name, p4Name); \
    OGL_LOG("<-- %s() = %u", #funcName, ::glGetError()); \
  }
#define OGL_WRAP_GLMTH_9(funcName, p1Type, p1Name, p1Mod, p2Type, p2Name, p2Mod, p3Type, p3Name, p3Mod, p4Type, p4Name, p4Mod, p5Type, p5Name, p5Mod, p6Type, p6Name, p6Mod, p7Type, p7Name, p7Mod, p8Type, p8Name, p8Mod, p9Type, p9Name, p9Mod) \
  inline void funcName##_logwrap(p1Type p1Name, p2Type p2Name, p3Type p3Name, p4Type p4Name, p5Type p5Name, p6Type p6Name, p7Type p7Name, p8Type p8Name, p9Type p9Name) \
  { \
    OGL_LOG("--> %s(" p1Mod ", " p2Mod ", " p3Mod ", " p4Mod ", " p5Mod ", " p6Mod ", " p7Mod ", " p8Mod ", " p9Mod ");", #funcName, p1Name, p2Name, p3Name, p4Name, p5Name, p6Name, p7Name, p8Name, p9Name); \
    ::funcName(p1Name, p2Name, p3Name, p4Name, p5Name, p6Name, p7Name, p8Name, p9Name); \
    OGL_LOG("<-- %s() = %u", #funcName, ::glGetError()); \
  }

// GL_API void GL_APIENTRY glAlphaFunc (GLenum func, GLclampf ref);
OGL_WRAP_GLMTH_2(glAlphaFunc, GLenum, func, "%u", GLclampf, ref, "%f");
#define glAlphaFunc glAlphaFunc_logwrap
// GL_API void GL_APIENTRY glClearColor (GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha);
OGL_WRAP_GLMTH_4(glClearColor, GLclampf, red, "%f", GLclampf, green, "%f", GLclampf, blue, "%f", GLclampf, alpha, "%f");
#define glClearColor glClearColor_logwrap
// GL_API void GL_APIENTRY glClearDepthf (GLclampf depth);
OGL_WRAP_GLMTH_1(glClearDepthf, GLclampf, depth, "%f");
#define glClearDepthf glClearDepthf_logwrap
// GL_API void GL_APIENTRY glClipPlanef (GLenum plane, const GLfloat *equation);
// @@@unused
// GL_API void GL_APIENTRY glColor4f (GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
OGL_WRAP_GLMTH_4(glColor4f, GLclampf, red, "%f", GLclampf, green, "%f", GLclampf, blue, "%f", GLclampf, alpha, "%f");
#define glColor4f glColor4f_logwrap
// GL_API void GL_APIENTRY glDepthRangef (GLclampf zNear, GLclampf zFar);
OGL_WRAP_GLMTH_2(glDepthRangef, GLclampf, zNear, "%f", GLclampf, zFar, "%f");
#define glDepthRangef glDepthRangef_logwrap
// GL_API void GL_APIENTRY glFogf (GLenum pname, GLfloat param);
OGL_WRAP_GLMTH_2(glFogf, GLenum, pname, "%u", GLfloat, param, "%f");
#define glFogf glFogf_logwrap
// GL_API void GL_APIENTRY glFogfv (GLenum pname, const GLfloat *params);
OGL_WRAP_GLMTH_2(glFogfv, GLenum, pname, "%u", const GLfloat *, params, "%p");
#define glFogfv glFogfv_logwrap
// GL_API void GL_APIENTRY glFrustumf (GLfloat left, GLfloat right, GLfloat bottom, GLfloat top, GLfloat zNear, GLfloat zFar);
// @@@unused
// GL_API void GL_APIENTRY glGetClipPlanef (GLenum pname, GLfloat eqn[4]);
// @@@unused
// GL_API void GL_APIENTRY glGetFloatv (GLenum pname, GLfloat *params);
OGL_WRAP_GLMTH_2(glGetFloatv, GLenum, pname, "%u", GLfloat *, params, "%p");
#define glGetFloatv glGetFloatv_logwrap
// GL_API void GL_APIENTRY glGetLightfv (GLenum light, GLenum pname, GLfloat *params);
// @@@unused
// GL_API void GL_APIENTRY glGetMaterialfv (GLenum face, GLenum pname, GLfloat *params);
// @@@unused
// GL_API void GL_APIENTRY glGetTexEnvfv (GLenum env, GLenum pname, GLfloat *params);
// @@@unused
// GL_API void GL_APIENTRY glGetTexParameterfv (GLenum target, GLenum pname, GLfloat *params);
// @@@unused
// GL_API void GL_APIENTRY glLightModelf (GLenum pname, GLfloat param);
// @@@unused
// GL_API void GL_APIENTRY glLightModelfv (GLenum pname, const GLfloat *params);
// @@@unused
// GL_API void GL_APIENTRY glLightf (GLenum light, GLenum pname, GLfloat param);
OGL_WRAP_GLMTH_3(glLightf, GLenum, light, "%u", GLenum, pname, "%u", GLfloat, param, "%f");
#define glLightf glLightf_logwrap
// GL_API void GL_APIENTRY glLightfv (GLenum light, GLenum pname, const GLfloat *params);
OGL_WRAP_GLMTH_3(glLightfv, GLenum, light, "%u", GLenum, pname, "%u", const GLfloat *, params, "%p");
#define glLightfv glLightfv_logwrap
// GL_API void GL_APIENTRY glLineWidth (GLfloat width);
OGL_WRAP_GLMTH_1(glLineWidth, GLfloat, width, "%f");
#define glLineWidth glLineWidth_logwrap
// GL_API void GL_APIENTRY glLoadMatrixf (const GLfloat *m);
OGL_WRAP_GLMTH_1(glLoadMatrixf, const GLfloat *, m, "%p");
#define glLoadMatrixf glLoadMatrixf_logwrap
// GL_API void GL_APIENTRY glMaterialf (GLenum face, GLenum pname, GLfloat param);
OGL_WRAP_GLMTH_3(glMaterialf, GLenum, face, "%u", GLenum, pname, "%u", GLfloat, param, "%f");
#define glMaterialf glMaterialf_logwrap
// GL_API void GL_APIENTRY glMaterialfv (GLenum face, GLenum pname, const GLfloat *params);
OGL_WRAP_GLMTH_3(glMaterialfv, GLenum, face, "%u", GLenum, pname, "%u", const GLfloat *, params, "%p");
#define glMaterialfv glMaterialfv_logwrap
// GL_API void GL_APIENTRY glMultMatrixf (const GLfloat *m);
OGL_WRAP_GLMTH_1(glMultMatrixf, const GLfloat *, m, "%p");
#define glMultMatrixf glMultMatrixf_logwrap
// GL_API void GL_APIENTRY glMultiTexCoord4f (GLenum target, GLfloat s, GLfloat t, GLfloat r, GLfloat q);
// @@@unused
// GL_API void GL_APIENTRY glNormal3f (GLfloat nx, GLfloat ny, GLfloat nz);
OGL_WRAP_GLMTH_3(glNormal3f, GLfloat, nx, "%f", GLfloat, ny, "%f", GLfloat, nz, "%f");
#define glNormal3f glNormal3f_logwrap
// GL_API void GL_APIENTRY glOrthof (GLfloat left, GLfloat right, GLfloat bottom, GLfloat top, GLfloat zNear, GLfloat zFar);
// @@@unused
// GL_API void GL_APIENTRY glPointParameterf (GLenum pname, GLfloat param);
// @@@unused
// GL_API void GL_APIENTRY glPointParameterfv (GLenum pname, const GLfloat *params);
// @@@unused
// GL_API void GL_APIENTRY glPointSize (GLfloat size);
// @@@unused
// GL_API void GL_APIENTRY glPolygonOffset (GLfloat factor, GLfloat units);
OGL_WRAP_GLMTH_2(glPolygonOffset, GLfloat, factor, "%f", GLfloat, units, "%f");
#define glPolygonOffset glPolygonOffset_logwrap
// GL_API void GL_APIENTRY glRotatef (GLfloat angle, GLfloat x, GLfloat y, GLfloat z);
// @@@unused
// GL_API void GL_APIENTRY glScalef (GLfloat x, GLfloat y, GLfloat z);
// @@@unused
// GL_API void GL_APIENTRY glTexEnvf (GLenum target, GLenum pname, GLfloat param);
OGL_WRAP_GLMTH_3(glTexEnvf, GLenum, target, "%u", GLenum, pname, "%u", GLfloat, param, "%f");
#define glTexEnvf glTexEnvf_logwrap
// GL_API void GL_APIENTRY glTexEnvfv (GLenum target, GLenum pname, const GLfloat *params);
OGL_WRAP_GLMTH_3(glTexEnvfv, GLenum, target, "%u", GLenum, pname, "%u", const GLfloat *, params, "%p");
#define glTexEnvfv glTexEnvfv_logwrap
// GL_API void GL_APIENTRY glTexParameterf (GLenum target, GLenum pname, GLfloat param);
OGL_WRAP_GLMTH_3(glTexParameterf, GLenum, target, "%u", GLenum, pname, "%u", GLfloat, param, "%f");
#define glTexParameterf glTexParameterf_logwrap
// GL_API void GL_APIENTRY glTexParameterfv (GLenum target, GLenum pname, const GLfloat *params);
OGL_WRAP_GLMTH_3(glTexParameterfv, GLenum, target, "%u", GLenum, pname, "%u", const GLfloat *, params, "%p");
#define glTexParameterfv glTexParameterfv_logwrap
// GL_API void GL_APIENTRY glTranslatef (GLfloat x, GLfloat y, GLfloat z);
// @@@unused
//
// GL_API void GL_APIENTRY glActiveTexture (GLenum texture);
// @@@unused
// GL_API void GL_APIENTRY glAlphaFuncx (GLenum func, GLclampx ref);
// @@@unused
// GL_API void GL_APIENTRY glBindBuffer (GLenum target, GLuint buffer);
// @@@unused
// GL_API void GL_APIENTRY glBindTexture (GLenum target, GLuint texture);
OGL_WRAP_GLMTH_2(glBindTexture, GLenum, target, "%u", GLuint, texture, "%u");
#define glBindTexture glBindTexture_logwrap
// GL_API void GL_APIENTRY glBlendFunc (GLenum sfactor, GLenum dfactor);
OGL_WRAP_GLMTH_2(glBlendFunc, GLenum, sfactor, "%u", GLenum, dfactor, "%u");
#define glBlendFunc glBlendFunc_logwrap
// GL_API void GL_APIENTRY glBufferData (GLenum target, GLsizeiptr size, const GLvoid *data, GLenum usage);
// @@@unused
// GL_API void GL_APIENTRY glBufferSubData (GLenum target, GLintptr offset, GLsizeiptr size, const GLvoid *data);
// @@@unused
// GL_API void GL_APIENTRY glClear (GLbitfield mask);
OGL_WRAP_GLMTH_1(glClear, GLbitfield, mask, "%u");
#define glClear glClear_logwrap
// GL_API void GL_APIENTRY glClearColorx (GLclampx red, GLclampx green, GLclampx blue, GLclampx alpha);
OGL_WRAP_GLMTH_4(glClearColorx, GLclampx, red, "%d", GLclampx, green, "%d", GLclampx, blue, "%d", GLclampx, alpha, "%d");
#define glClearColorx glClearColorx_logwrap
// GL_API void GL_APIENTRY glClearDepthx (GLclampx depth);
OGL_WRAP_GLMTH_1(glClearDepthx, GLclampx, depth, "%d");
#define glClearDepthx glClearDepthx_logwrap
// GL_API void GL_APIENTRY glClearStencil (GLint s);
OGL_WRAP_GLMTH_1(glClearStencil, GLint, s, "%d");
#define glClearStencil glClearStencil_logwrap
// GL_API void GL_APIENTRY glClientActiveTexture (GLenum texture);
// @@@unused
// GL_API void GL_APIENTRY glClipPlanex (GLenum plane, const GLfixed *equation);
// @@@unused
// GL_API void GL_APIENTRY glColor4ub (GLubyte red, GLubyte green, GLubyte blue, GLubyte alpha);
OGL_WRAP_GLMTH_4_UB(glColor4ub, GLubyte, red, "%u", GLubyte, green, "%u", GLubyte, blue, "%u", GLubyte, alpha, "%u");
#define glColor4ub glColor4ub_logwrap
//GL_API void GL_APIENTRY glColor4x (GLfixed red, GLfixed green, GLfixed blue, GLfixed alpha);
OGL_WRAP_GLMTH_4(glColor4x, GLfixed, red, "%d", GLfixed, green, "%d", GLfixed, blue, "%d", GLfixed, alpha, "%d");
#define glColor4x glColor4x_logwrap
// GL_API void GL_APIENTRY glColorMask (GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha);
OGL_WRAP_GLMTH_4_UB(glColorMask, GLboolean, red, "%u", GLboolean, green, "%u", GLboolean, blue, "%u", GLboolean, alpha, "%u");
#define glColorMask glColorMask_logwrap
// GL_API void GL_APIENTRY glColorPointer (GLint size, GLenum type, GLsizei stride, const GLvoid *pointer);
OGL_WRAP_GLMTH_4(glColorPointer, GLint, size, "%d", GLenum, type, "%u", GLsizei, stride, "%u", const GLvoid *, pointer, "%p");
#define glColorPointer glColorPointer_logwrap
// GL_API void GL_APIENTRY glCompressedTexImage2D (GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const GLvoid *data);
// @@@unused
// GL_API void GL_APIENTRY glCompressedTexSubImage2D (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, const GLvoid *data);
// @@@unused
// GL_API void GL_APIENTRY glCopyTexImage2D (GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLsizei height, GLint border);
// @@@unused
// GL_API void GL_APIENTRY glCopyTexSubImage2D (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height);
// @@@unused
// GL_API void GL_APIENTRY glCullFace (GLenum mode);
OGL_WRAP_GLMTH_1(glCullFace, GLenum, mode, "%u");
#define glCullFace glCullFace_logwrap
// GL_API void GL_APIENTRY glDeleteBuffers (GLsizei n, const GLuint *buffers);
// @@@unused
// GL_API void GL_APIENTRY glDeleteTextures (GLsizei n, const GLuint *textures);
OGL_WRAP_GLMTH_2(glDeleteTextures, GLsizei, n, "%u", const GLuint *, textures, "%p");
#define glDeleteTextures glDeleteTextures_logwrap
// GL_API void GL_APIENTRY glDepthFunc (GLenum func);
OGL_WRAP_GLMTH_1(glDepthFunc, GLenum, func, "%u");
#define glDepthFunc glDepthFunc_logwrap
// GL_API void GL_APIENTRY glDepthMask (GLboolean flag);
// @@@unused
// GL_API void GL_APIENTRY glDepthRangex (GLclampx zNear, GLclampx zFar);
// @@@unused
// GL_API void GL_APIENTRY glDisable (GLenum cap);
OGL_WRAP_GLMTH_1(glDisable, GLenum, cap, "%u");
#define glDisable glDisable_logwrap
// GL_API void GL_APIENTRY glDisableClientState (GLenum array);
OGL_WRAP_GLMTH_1(glDisableClientState, GLenum, array, "%u");
#define glDisableClientState glDisableClientState_logwrap
// GL_API void GL_APIENTRY glDrawArrays (GLenum mode, GLint first, GLsizei count);
OGL_WRAP_GLMTH_3(glDrawArrays, GLenum, mode, "%u", GLint, first, "%d", GLsizei, count, "%u");
#define glDrawArrays glDrawArrays_logwrap
// GL_API void GL_APIENTRY glDrawElements (GLenum mode, GLsizei count, GLenum type, const GLvoid *indices);
OGL_WRAP_GLMTH_4(glDrawElements, GLenum, mode, "%u", GLsizei, count, "%u", GLenum, type, "%u", const GLvoid *, indices, "%p");
#define glDrawElements glDrawElements_logwrap
// GL_API void GL_APIENTRY glEnable (GLenum cap);
OGL_WRAP_GLMTH_1(glEnable, GLenum, cap, "%u");
#define glEnable glEnable_logwrap
// GL_API void GL_APIENTRY glEnableClientState (GLenum array);
OGL_WRAP_GLMTH_1(glEnableClientState, GLenum, array, "%u");
#define glEnableClientState glEnableClientState_logwrap
// GL_API void GL_APIENTRY glFinish (void);
OGL_WRAP_GLMTH_0(glFinish);
#define glFinish glFinish_logwrap
// GL_API void GL_APIENTRY glFlush (void);
OGL_WRAP_GLMTH_0(glFlush);
#define glFlush glFlush_logwrap
// GL_API void GL_APIENTRY glFogx (GLenum pname, GLfixed param);
OGL_WRAP_GLMTH_2(glFogx, GLenum, pname, "%u", GLfixed, param, "%d");
#define glFogx glFogx_logwrap
// GL_API void GL_APIENTRY glFogxv (GLenum pname, const GLfixed *params);
OGL_WRAP_GLMTH_2(glFogxv, GLenum, pname, "%u", const GLfixed *, params, "%p");
#define glFogxv glFogxv_logwrap
// GL_API void GL_APIENTRY glFrontFace (GLenum mode);
OGL_WRAP_GLMTH_1(glFrontFace, GLenum, mode, "%u");
#define glFrontFace glFrontFace_logwrap
// GL_API void GL_APIENTRY glFrustumx (GLfixed left, GLfixed right, GLfixed bottom, GLfixed top, GLfixed zNear, GLfixed zFar);
// @@@unused
// GL_API void GL_APIENTRY glGetBooleanv (GLenum pname, GLboolean *params);
OGL_WRAP_GLMTH_2(glGetBooleanv, GLenum, pname, "%u", GLboolean *, params, "%p");
#define glGetBooleanv glGetBooleanv_logwrap
// GL_API void GL_APIENTRY glGetBufferParameteriv (GLenum target, GLenum pname, GLint *params);
// @@@unused
// GL_API void GL_APIENTRY glGetClipPlanex (GLenum pname, GLfixed eqn[4]);
// @@@unused
// GL_API void GL_APIENTRY glGenBuffers (GLsizei n, GLuint *buffers);
// @@@unused
// GL_API void GL_APIENTRY glGenTextures (GLsizei n, GLuint *textures);
OGL_WRAP_GLMTH_2(glGenTextures, GLsizei, n, "%u", GLuint *, textures, "%p");
#define glGenTextures glGenTextures_logwrap
// GL_API GLenum GL_APIENTRY glGetError (void);
// @@@dontwrap
// GL_API void GL_APIENTRY glGetFixedv (GLenum pname, GLfixed *params);
OGL_WRAP_GLMTH_2(glGetFixedv, GLenum, pname, "%u", GLfixed *, params, "%p");
#define glGetFixedv glGetFixedv_logwrap
// GL_API void GL_APIENTRY glGetIntegerv (GLenum pname, GLint *params);
OGL_WRAP_GLMTH_2(glGetIntegerv, GLenum, pname, "%u", GLint *, params, "%p");
#define glGetIntegerv glGetIntegerv_logwrap
// GL_API void GL_APIENTRY glGetLightxv (GLenum light, GLenum pname, GLfixed *params);
// @@@unused
// GL_API void GL_APIENTRY glGetMaterialxv (GLenum face, GLenum pname, GLfixed *params);
// @@@unused
// GL_API void GL_APIENTRY glGetPointerv (GLenum pname, void **params);
OGL_WRAP_GLMTH_2(glGetPointerv, GLenum, pname, "%u", void **, params, "%p");
#define glGetPointerv glGetPointerv_logwrap
// GL_API const GLubyte * GL_APIENTRY glGetString (GLenum name);
// @@@dontwrap
// GL_API void GL_APIENTRY glGetTexEnviv (GLenum env, GLenum pname, GLint *params);
// @@@unused
// GL_API void GL_APIENTRY glGetTexEnvxv (GLenum env, GLenum pname, GLfixed *params);
// @@@unused
// GL_API void GL_APIENTRY glGetTexParameteriv (GLenum target, GLenum pname, GLint *params);
// @@@unused
// GL_API void GL_APIENTRY glGetTexParameterxv (GLenum target, GLenum pname, GLfixed *params);
// @@@unused
// GL_API void GL_APIENTRY glHint (GLenum target, GLenum mode);
OGL_WRAP_GLMTH_2(glHint, GLenum, target, "%u", GLenum, mode, "%u");
#define glHint glHint_logwrap
// GL_API GLboolean GL_APIENTRY glIsBuffer (GLuint buffer);
// @@@unused
// GL_API GLboolean GL_APIENTRY glIsEnabled (GLenum cap);
OGL_WRAP_GLMTH_1_RVAL(glIsEnabled, GLenum, cap, "%u", GLboolean);
#define glIsEnabled glIsEnabled_logwrap
// GL_API GLboolean GL_APIENTRY glIsTexture (GLuint texture);
// @@@unused
// GL_API void GL_APIENTRY glLightModelx (GLenum pname, GLfixed param);
OGL_WRAP_GLMTH_2(glLightModelx, GLenum, pname, "%u", GLfixed, param, "%d");
#define glLightModelx glLightModelx_logwrap
// GL_API void GL_APIENTRY glLightModelxv (GLenum pname, const GLfixed *params);
OGL_WRAP_GLMTH_2(glLightModelxv, GLenum, pname, "%u", const GLfixed *, params, "%p");
#define glLightModelxv glLightModelxv_logwrap
// GL_API void GL_APIENTRY glLightx (GLenum light, GLenum pname, GLfixed param);
OGL_WRAP_GLMTH_3(glLightx, GLenum, light, "%u", GLenum, pname, "%u", GLfixed, param, "%d");
#define glLightx glLightx_logwrap
// GL_API void GL_APIENTRY glLightxv (GLenum light, GLenum pname, const GLfixed *params);
OGL_WRAP_GLMTH_3(glLightxv, GLenum, light, "%u", GLenum, pname, "%u", const GLfixed *, params, "%p");
#define glLightxv glLightxv_logwrap
// GL_API void GL_APIENTRY glLineWidthx (GLfixed width);
OGL_WRAP_GLMTH_1(glLineWidthx, GLfixed, width, "%d");
#define glLineWidthx glLineWidthx_logwrap
// GL_API void GL_APIENTRY glLoadIdentity (void);
OGL_WRAP_GLMTH_0(glLoadIdentity);
#define glLoadIdentity glLoadIdentity_logwrap
// GL_API void GL_APIENTRY glLoadMatrixx (const GLfixed *m);
OGL_WRAP_GLMTH_1(glLoadMatrixx, const GLfixed *, m, "%p");
#define glLoadMatrixx glLoadMatrixx_logwrap
// GL_API void GL_APIENTRY glLogicOp (GLenum opcode);
OGL_WRAP_GLMTH_1(glLogicOp, GLenum, opcode, "%u");
#define glLogicOp glLogicOp_logwrap
// GL_API void GL_APIENTRY glMaterialx (GLenum face, GLenum pname, GLfixed param);
OGL_WRAP_GLMTH_3(glMaterialx, GLenum, face, "%u", GLenum, pname, "%u", GLfixed, param, "%d");
#define glMaterialx glMaterialx_logwrap
// GL_API void GL_APIENTRY glMaterialxv (GLenum face, GLenum pname, const GLfixed *params);
OGL_WRAP_GLMTH_3(glMaterialxv, GLenum, face, "%u", GLenum, pname, "%u", const GLfixed *, params, "%p");
#define glMaterialxv glMaterialxv_logwrap
// GL_API void GL_APIENTRY glMatrixMode (GLenum mode);
OGL_WRAP_GLMTH_1(glMatrixMode, GLenum, mode, "%u");
#define glMatrixMode glMatrixMode_logwrap
// GL_API void GL_APIENTRY glMultMatrixx (const GLfixed *m);
// @@@unused
// GL_API void GL_APIENTRY glMultiTexCoord4x (GLenum target, GLfixed s, GLfixed t, GLfixed r, GLfixed q);
// @@@unused
// GL_API void GL_APIENTRY glNormal3x (GLfixed nx, GLfixed ny, GLfixed nz);
OGL_WRAP_GLMTH_3(glNormal3x, GLfixed, nx, "%d", GLfixed, ny, "%d", GLfixed, nz, "%d");
#define glNormal3x glNormal3x_logwrap
// GL_API void GL_APIENTRY glNormalPointer (GLenum type, GLsizei stride, const GLvoid *pointer);
OGL_WRAP_GLMTH_3(glNormalPointer, GLenum, type, "%u", GLsizei, stride, "%u", const GLvoid *, pointer, "%p");
#define glNormalPointer glNormalPointer_logwrap
// GL_API void GL_APIENTRY glOrthox (GLfixed left, GLfixed right, GLfixed bottom, GLfixed top, GLfixed zNear, GLfixed zFar);
// @@@unused
// GL_API void GL_APIENTRY glPixelStorei (GLenum pname, GLint param);
OGL_WRAP_GLMTH_2(glPixelStorei, GLenum, pname, "%u", GLint, param, "%d");
#define glPixelStorei glPixelStorei_logwrap
// GL_API void GL_APIENTRY glPointParameterx (GLenum pname, GLfixed param);
// @@@unused
// GL_API void GL_APIENTRY glPointParameterxv (GLenum pname, const GLfixed *params);
// @@@unused
// GL_API void GL_APIENTRY glPointSizex (GLfixed size);
// @@@unused
// GL_API void GL_APIENTRY glPolygonOffsetx (GLfixed factor, GLfixed units);
OGL_WRAP_GLMTH_2(glPolygonOffsetx, GLfixed, factor, "%d", GLfixed, units, "%d");
#define glPolygonOffsetx glPolygonOffsetx_logwrap
// GL_API void GL_APIENTRY glPopMatrix (void);
OGL_WRAP_GLMTH_0(glPopMatrix);
#define glPopMatrix glPopMatrix_logwrap
// GL_API void GL_APIENTRY glPushMatrix (void);
OGL_WRAP_GLMTH_0(glPushMatrix);
#define glPushMatrix glPushMatrix_logwrap
// GL_API void GL_APIENTRY glReadPixels (GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, GLvoid *pixels);
// @@@unused
// GL_API void GL_APIENTRY glRotatex (GLfixed angle, GLfixed x, GLfixed y, GLfixed z);
// @@@unused
// GL_API void GL_APIENTRY glSampleCoverage (GLclampf value, GLboolean invert);
// @@@unused
// GL_API void GL_APIENTRY glSampleCoveragex (GLclampx value, GLboolean invert);
// @@@unused
// GL_API void GL_APIENTRY glScalex (GLfixed x, GLfixed y, GLfixed z);
// @@@unused
// GL_API void GL_APIENTRY glScissor (GLint x, GLint y, GLsizei width, GLsizei height);
// @@@unused
// GL_API void GL_APIENTRY glShadeModel (GLenum mode);
OGL_WRAP_GLMTH_1(glShadeModel, GLenum, mode, "%u");
#define glShadeModel glShadeModel_logwrap
// GL_API void GL_APIENTRY glStencilFunc (GLenum func, GLint ref, GLuint mask);
OGL_WRAP_GLMTH_3(glStencilFunc, GLenum, func, "%u", GLint, ref, "%d", GLuint, mask, "%u");
#define glStencilFunc glStencilFunc_logwrap
// GL_API void GL_APIENTRY glStencilMask (GLuint mask);
OGL_WRAP_GLMTH_1(glStencilMask, GLuint, mask, "%u");
#define glStencilMask glStencilMask_logwrap
// GL_API void GL_APIENTRY glStencilOp (GLenum fail, GLenum zfail, GLenum zpass);
OGL_WRAP_GLMTH_3(glStencilOp, GLenum, fail, "%u", GLenum, zfail, "%u", GLenum, zpass, "%u");
#define glStencilOp glStencilOp_logwrap
// GL_API void GL_APIENTRY glTexCoordPointer (GLint size, GLenum type, GLsizei stride, const GLvoid *pointer);
OGL_WRAP_GLMTH_4(glTexCoordPointer, GLint, size, "%d", GLenum, type, "%u", GLsizei, stride, "%u", const GLvoid *, pointer, "%p");
#define glTexCoordPointer glTexCoordPointer_logwrap
// GL_API void GL_APIENTRY glTexEnvi (GLenum target, GLenum pname, GLint param);
OGL_WRAP_GLMTH_3(glTexEnvi, GLenum, target, "%u", GLenum, pname, "%u", GLint, param, "%d");
#define glTexEnvi glTexEnvi_logwrap
// GL_API void GL_APIENTRY glTexEnvx (GLenum target, GLenum pname, GLfixed param);
OGL_WRAP_GLMTH_3(glTexEnvx, GLenum, target, "%u", GLenum, pname, "%u", GLfixed, param, "%d");
#define glTexEnvx glTexEnvx_logwrap
// GL_API void GL_APIENTRY glTexEnviv (GLenum target, GLenum pname, const GLint *params);
OGL_WRAP_GLMTH_3(glTexEnviv, GLenum, target, "%u", GLenum, pname, "%u", const GLint *, params, "%p");
#define glTexEnviv glTexEnviv_logwrap
// GL_API void GL_APIENTRY glTexEnvxv (GLenum target, GLenum pname, const GLfixed *params);
OGL_WRAP_GLMTH_3(glTexEnvxv, GLenum, target, "%u", GLenum, pname, "%u", const GLfixed *, params, "%p");
#define glTexEnvxv glTexEnvxv_logwrap
// GL_API void GL_APIENTRY glTexImage2D (GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid *pixels);
OGL_WRAP_GLMTH_9(glTexImage2D, GLenum, target, "%u", GLint, level, "%d", GLint, internalformat, "%d", GLsizei, width, "%u", GLsizei, height, "%u", GLint, border, "%d", GLenum, format, "%u", GLenum, type, "%u", const GLvoid *, pixels, "%p");
#define glTexImage2D glTexImage2D_logwrap
// GL_API void GL_APIENTRY glTexParameteri (GLenum target, GLenum pname, GLint param);
OGL_WRAP_GLMTH_3(glTexParameteri, GLenum, target, "%u", GLenum, pname, "%u", GLint, param, "%d");
#define glTexParameteri glTexParameteri_logwrap
// GL_API void GL_APIENTRY glTexParameterx (GLenum target, GLenum pname, GLfixed param);
OGL_WRAP_GLMTH_3(glTexParameterx, GLenum, target, "%u", GLenum, pname, "%u", GLfixed, param, "%d");
#define glTexParameterx glTexParameterx_logwrap
// GL_API void GL_APIENTRY glTexParameteriv (GLenum target, GLenum pname, const GLint *params);
OGL_WRAP_GLMTH_3(glTexParameteriv, GLenum, target, "%u", GLenum, pname, "%u", const GLint *, params, "%p");
#define glTexParameteriv glTexParameteriv_logwrap
// GL_API void GL_APIENTRY glTexParameterxv (GLenum target, GLenum pname, const GLfixed *params);
OGL_WRAP_GLMTH_3(glTexParameterxv, GLenum, target, "%u", GLenum, pname, "%u", const GLfixed *, params, "%p");
#define glTexParameterxv glTexParameterxv_logwrap
// GL_API void GL_APIENTRY glTexSubImage2D (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid *pixels);
// @@@unused
// GL_API void GL_APIENTRY glTranslatex (GLfixed x, GLfixed y, GLfixed z);
// @@@unused
// GL_API void GL_APIENTRY glVertexPointer (GLint size, GLenum type, GLsizei stride, const GLvoid *pointer);
OGL_WRAP_GLMTH_4(glVertexPointer, GLint, size, "%d", GLenum, type, "%u", GLsizei, stride, "%u", const GLvoid *, pointer, "%p");
#define glVertexPointer glVertexPointer_logwrap
// GL_API void GL_APIENTRY glViewport (GLint x, GLint y, GLsizei width, GLsizei height);
OGL_WRAP_GLMTH_4(glViewport, GLint, x, "%d", GLint, y, "%d", GLsizei, width, "%u", GLsizei, height, "%u");
#define glViewport glViewport_logwrap

#endif // _h_androidwrap
