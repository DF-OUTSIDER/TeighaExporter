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

#include "OdPlatform.h"

// iOS
#if defined(__APPLE__) && (defined(__IPHONE_OS_VERSION_MIN_REQUIRED) || TARGET_IPHONE_SIMULATOR)

#ifndef TD_OPENGL_ES
#define TD_OPENGL_ES
#endif

#endif

// Android
#if defined(ANDROID)

#ifndef TD_OPENGL_ES
#define TD_OPENGL_ES
#endif

#endif

//
#if !defined(TD_OPENGL_ES)

#if defined(__APPLE__) /////////////////////////////////// MAC

#include <OpenGL/gl.h>
#ifdef TD_USING_GLU
#include <OpenGL/glu.h>
#endif

#ifdef TD_USING_GLUT
#include <GLUT/glut.h>
#endif

#define TD_STD_CALL 

#if defined(TD_USE_QT_LIB)
#if !(defined(__IPHONE_OS_VERSION_MIN_REQUIRED))
#include <OpenGL/glext.h>
#endif
#endif

#elif defined sgi  ///////////////////////////////// SGI

#include <GL/gl.h>
#ifdef TD_USING_GLU
#include <GL/glu.h>
#endif

#ifdef TD_USING_GLUT
#include <GL/glut.h>
#endif

#define TD_STD_CALL 

#elif defined(__linux__) || defined(EMCC) ///////////////////////////////// Linux OR EMCC

#include <GL/gl.h>
#ifdef TD_USING_GLU
#include <GL/glu.h>
#endif

#ifdef TD_USING_GLUT
#include <GL/glut.h>
#endif

#define TD_STD_CALL 

#else                  ////////////////////////////////// Windows

#include <gl/gl.h>
#ifdef TD_USING_GLU
#include <gl/glu.h>
#endif

#ifdef TD_USING_GLUT
#include <GL/glut.h>
#endif

#define TD_STD_CALL __stdcall

#endif                 ////////////////////////////////// END

#else // OpenGL ES

#ifndef ANDROID
// for iOS
#include <OpenGLES/ES1/gl.h>
#include <OpenGLES/ES1/glext.h>
#else
// for Android
#include <GLES/gl.h>
#include <GLES/glext.h>
// debug log
// #include "../Extensions/ExRender/Android/gl_debug_driver.h"
#endif

#ifdef TD_USING_GLU
#error OpenGL ES platform does not support glu
#endif
#ifdef TD_USING_GLUT
#error OpenGL ES platform does not support glut
#endif

#define TD_STD_CALL 

#endif
