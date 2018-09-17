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
// OpenGL local GL definitions (to provide compatible isolated server part)

#ifndef ODGLLOCALGLDEFINITIONS
#define ODGLLOCALGLDEFINITIONS

#include "TD_PackPush.h"

/* Primitive */
#ifndef GL_POINTS
#define GL_POINTS                         0x0000
#endif
#ifndef GL_LINES
#define GL_LINES                          0x0001
#endif
#ifndef GL_LINE_LOOP
#define GL_LINE_LOOP                      0x0002
#endif
#ifndef GL_LINE_STRIP
#define GL_LINE_STRIP                     0x0003
#endif
#ifndef GL_TRIANGLES
#define GL_TRIANGLES                      0x0004
#endif
#ifndef GL_TRIANGLE_STRIP
#define GL_TRIANGLE_STRIP                 0x0005
#endif
#ifndef GL_TRIANGLE_FAN
#define GL_TRIANGLE_FAN                   0x0006
#endif

/* CullFaceMode */
#ifndef GL_FRONT
#define GL_FRONT                          0x0404
#endif
#ifndef GL_BACK
#define GL_BACK                           0x0405
#endif
#ifndef GL_FRONT_AND_BACK
#define GL_FRONT_AND_BACK                 0x0408
#endif

typedef void     OdGLvoid;
//typedef char     OdGLchar; // wasn't defined in Android SDK
typedef OdUInt32 OdGLenum;
typedef OdUInt8  OdGLboolean;
typedef OdInt8   OdGLbyte;
typedef OdInt16  OdGLshort;
typedef OdInt32  OdGLint;
typedef OdInt32  OdGLsizei;
typedef OdUInt8  OdGLubyte;
typedef OdUInt16 OdGLushort;
typedef OdUInt32 OdGLuint;
typedef float    OdGLfloat;
typedef float    OdGLclampf;
typedef double   OdGLdouble;

#include "TD_PackPop.h"

#endif // ODGLLOCALGLDEFINITIONS

// Assume that our GL headers have compatible data types

#ifdef ODGLLOCALGLDEFINITIONS_ASSUME_GL

// I
//ODA_ASSUME(sizeof(GLchar) == sizeof(OdGLchar)); // wasn't defined in Android SDK
// II
ODA_ASSUME(sizeof(GLenum) == sizeof(OdGLenum));
// III
ODA_ASSUME(sizeof(GLboolean) == sizeof(OdGLboolean));
// IV
ODA_ASSUME(sizeof(GLbyte) == sizeof(OdGLbyte));
// V
ODA_ASSUME(sizeof(GLshort) == sizeof(OdGLshort));
// VI
ODA_ASSUME(sizeof(GLint) == sizeof(OdGLint));
// VII
ODA_ASSUME(sizeof(GLsizei) == sizeof(OdGLsizei));
// VIII
ODA_ASSUME(sizeof(GLubyte) == sizeof(OdGLubyte));
// IX
ODA_ASSUME(sizeof(GLushort) == sizeof(OdGLushort));
// X
ODA_ASSUME(sizeof(GLuint) == sizeof(OdGLuint));
// XI
ODA_ASSUME(sizeof(GLfloat) == sizeof(OdGLfloat));
// XII
ODA_ASSUME(sizeof(GLclampf) == sizeof(OdGLclampf));
// XIII
//ODA_ASSUME(sizeof(GLdouble) == sizeof(OdGLdouble)); // wasn't defined in ES2

#endif // ODGLLOCALGLDEFINITIONS_ASSUME_GL
