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

#import <AppKit/AppKit.h>
#include <Cocoa/Cocoa.h>

#include "OdaCommon.h"
#include <OpenGL/CGLTypes.h>
#include <OpenGL/OpenGL.h>
#include <OpenGL/CGLRenderers.h>

@interface GLES2OnscreenRenderer : NSObject
{
  NSAutoreleasePool* m_pool;
@public
  NSView* m_view;
  NSOpenGLContext* m_ctx;
  bool m_bReleaseCtx;
  unsigned m_width, m_height;
}
@property (retain) NSOpenGLContext* m_ctx;
-(id)initWithParams:(NSView*)pView withContext:(NSOpenGLContext*)pCtx withDoubleBuffer:(bool)bDoubleBuffer withWidth:(unsigned)width withHeight:(unsigned)height;
-(void)uninit;
-(void)setDims:(unsigned)width withHeight:(unsigned)height;
@end

@implementation GLES2OnscreenRenderer : NSObject
{
#ifndef __clang__
  NSAutoreleasePool* m_pool;
@public
  NSView* m_view;
  NSOpenGLContext* m_ctx;
  bool m_bReleaseCtx;
  unsigned m_width, m_height;
#endif
}
@synthesize m_ctx;
-(id)initWithParams:(NSView*)pView withContext:(NSOpenGLContext*)pCtx withDoubleBuffer:(bool)bDoubleBuffer withWidth:(unsigned)width withHeight:(unsigned)height
{
  if (pCtx == nil)
  {
    NSOpenGLPixelFormatAttribute attributes[] =
    {
      NSOpenGLPFAWindow,
      NSOpenGLPFADepthSize, (NSOpenGLPixelFormatAttribute)24,
      NSOpenGLPFAStencilSize, (NSOpenGLPixelFormatAttribute)8,
      (bDoubleBuffer) ? NSOpenGLPFADoubleBuffer : 0,
      0
    };
    m_pool = [[NSAutoreleasePool alloc] init];
    NSOpenGLPixelFormat* format = [[[NSOpenGLPixelFormat alloc] initWithAttributes:attributes] autorelease];
    m_ctx = [[NSOpenGLContext alloc] initWithFormat:format shareContext:nil];
    ODA_ASSERT(m_ctx != nil);
    [m_ctx setView:pView];
    [m_ctx update];
    m_view = pView;
    m_bReleaseCtx = true;
  }
  else
  {
    m_view = pView;
    m_ctx = pCtx;
    m_bReleaseCtx = false;
  }
  [m_ctx makeCurrentContext];
  [self setDims:width withHeight:height];
  m_width = width; m_height = height;
  return self;
}
-(void)uninit
{
  if (m_bReleaseCtx)
  {
    [m_ctx clearDrawable];
    [m_ctx release];
    [m_pool release];
  }
}
-(void)setDims:(unsigned)width withHeight:(unsigned)height
{
  m_width = width;
  m_height = height;
}
@end

// C++ wrappers
void *odGLES2AttachCocoaNS(void *pNSView, void *pGLContext, bool bDoubleBuffer, unsigned width, unsigned height)
{
  id viewId = (id)pNSView, ctxId = (id)pGLContext;
  return (void*)[[GLES2OnscreenRenderer alloc] initWithParams:(NSView*)viewId withContext:(NSOpenGLContext*)ctxId withDoubleBuffer:bDoubleBuffer withWidth:width withHeight:height];
}

void odGLES2DetachCocoaNS(void *pObj)
{
  id objId = (id)pObj;
  GLES2OnscreenRenderer *m_ctx = objId;
  [m_ctx uninit];
  [m_ctx release];
}

void odGLES2MakeCurrentCocoaNS(void *pObj)
{
  id objId = (id)pObj;
  GLES2OnscreenRenderer *m_ctx = objId;
  [m_ctx.m_ctx makeCurrentContext];
}

void *odGLES2ExtractContextCocoaNS(void *pObj)
{
  id objId = (id)pObj;
  GLES2OnscreenRenderer *m_ctx = objId;
  return (void*)m_ctx->m_ctx;
}

void odGLES2PresentCocoaNS(void *pObj)
{
  id objId = (id)pObj;
  GLES2OnscreenRenderer *m_ctx = objId;
  //if (m_ctx->m_bReleaseCtx) // Logically incorrect
  {
    //if ([m_ctx->m_view isLiveResize])
    //  ::glFlush();
    //else
      [m_ctx->m_ctx flushBuffer];
  }
}

bool odGLES2CheckDimsCocoaNS(void *pObj, unsigned width, unsigned height)
{
  id objId = (id)pObj;
  GLES2OnscreenRenderer *m_ctx = objId;
  if (m_ctx->m_bReleaseCtx)
  {
    if ((m_ctx->m_width != width) || (m_ctx->m_height != height))
    {
      [m_ctx setDims:width withHeight:height];
      [m_ctx->m_ctx update];
    }
  }
}
//
