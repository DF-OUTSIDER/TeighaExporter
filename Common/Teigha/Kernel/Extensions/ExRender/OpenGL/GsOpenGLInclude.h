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

#ifndef ODOPENGLVECTORIZER_GLINCLUDE_INCLUDED
#define ODOPENGLVECTORIZER_GLINCLUDE_INCLUDED

#include "TD_PackPush.h"

#include "GsOpenGLDefs.h"

#ifndef TD_OPENGL_ES2
#include "TD_GL.h"
#include "OpenGL_ES.h"
#endif

// Enables alternative highlighting method
#ifndef OD_OGL_ES
//#define OD_OGL_ALTERNATIVEHLT
#endif

#if defined(ANDROID)
#define MAX_COUNTER_BEFORE_FLASH 60
void glSafeDrawArrays(GLenum mode, GLint first, GLsizei count);
#else //ANDROID
#define glSafeDrawArrays ::glDrawArrays
#endif //ANDROID

#include "TD_PackPop.h"

#endif // ODOPENGLVECTORIZER_GLINCLUDE_INCLUDED
