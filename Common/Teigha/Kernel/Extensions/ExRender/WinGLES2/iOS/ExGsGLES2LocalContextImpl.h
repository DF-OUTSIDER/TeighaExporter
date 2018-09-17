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

#ifndef EXGSGLES2LOCALCONTEXT
#define EXGSGLES2LOCALCONTEXT

void *odGLES2AttachLocalContext(void *pLayer);
void *odGLES2AttachLocalContext(long nWidth, long nHeight);
void odGLES2DetachLocalContext(void *pObj);
void odGLES2UpdateLocalContext(void *pObj, void *pLayer, long nWidth, long nHeight);
bool odGLES2IsOffscreenLocalContext(void *pObj);
bool odGLES2CheckDimensionsOfLocalContext(void *pObj, long nWidth, long nHeight);
void odGLES2MakeCurrentLocalContext(void *pObj);
void odGLES2PresentLocalContext(void *pObj);
void odGLES2MakeCurrentExternalContext(void *pContext);
void odGLES2PresentExternalContext(void *pContext);
void *odGLES2ExtractPointerForLocalContext(void *pObj);

#endif // EXGSGLES2LOCALCONTEXT
