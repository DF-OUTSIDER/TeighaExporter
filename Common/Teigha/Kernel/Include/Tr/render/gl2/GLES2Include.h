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
// GLES2 include

#ifndef ODTRGL2INCLUDE
#define ODTRGL2INCLUDE

#include "OdPlatformSettings.h"

// Includes for OpenGL ES 2.0
#ifdef EMCC
#include<GLES2/gl2.h>
#elif defined(__IPHONE_OS_VERSION_MIN_REQUIRED) || defined(TARGET_IPHONE_SIMULATOR)
// iOS
#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>
#define OD_TRGL2_IOS
#elif defined(ANDROID) || defined(TRGL2EMUL_ENABLED) || defined(EMCC)
#include <GLES2/gl2.h>
// Includes for EGL
#include <EGL/egl.h>
// EGL is available only since 9 Android platform, define this for platform 8 and earlier
//#define OD_TRGL2_NOEGL
#else // #elif IOS/Android
// Windows/Linux/Mac/Sun and etc. PC OS
#include "compatibility/gles2_ext.h"
#endif

#if defined(ANDROID) || defined(OD_TRGL2_IOS) || defined(EMCC)
#define OD_TRGL2_NATIVEES2
#endif

// Extensions
#include "TrGL2Extensions.h"

#if defined(ANDROID)
#define MAX_COUNTER_BEFORE_FLASH 300
void glesSafeDrawArrays(GLenum mode, GLint first, GLsizei count);
#else //ANDROID
#define glesSafeDrawArrays ::glDrawArrays
#endif //ANDROID

#endif // ODTRGL2INCLUDE
