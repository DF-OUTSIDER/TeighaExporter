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

#import <QuartzCore/QuartzCore.h>
#import <QuartzCore/CAEAGLLayer.h>

#import <OpenGLES/EAGL.h>
#import <OpenGLES/EAGLDrawable.h>

#import <OpenGLES/ES2/gl.h>
#import <OpenGLES/ES2/glext.h>

#ifndef GL_RGBA8
#ifdef GL_OES_rgb8_rgba8
#define GL_RGBA8 GL_RGBA8_OES
#else
#define GL_RGBA8 0x8058
#endif
#endif

@interface OdGLES2LocalContext : NSObject
{
  EAGLContext *m_pContext;
  GLboolean m_bOffscreen;
  GLint m_nBackingWidth;
  GLint m_nBackingHeight;
  GLuint m_frameBuf;
  GLuint m_colorBuf;
  GLuint m_depthBuf;
  GLuint m_stencilBuf;
}

@property (retain) EAGLContext* m_pContext;
-(id)initWithLayer:(CAEAGLLayer*)layer;
-(id)initWithDimensions:(long)width inHeight:(long)height;
-(void)uninit;
-(id)initializeLocalContext:(GLboolean)isOffscreen inLayer:(CAEAGLLayer*)layer inWidth:(long)width inHeight:(long)height;
-(void)createFrameBuffers:(CAEAGLLayer*)layer inWidth:(long)width inHeight:(long)height;
-(void)deleteFrameBuffers;
-(bool)isOffscreen;
-(bool)checkDimensions:(long)width inHeight:(long)height;
-(id)getContext;
@end

@implementation OdGLES2LocalContext : NSObject
#if __IPHONE_OS_VERSION_MIN_REQUIRED < 70000
{
  EAGLContext *m_pContext;
  GLboolean m_bOffscreen;
  GLint m_nBackingWidth;
  GLint m_nBackingHeight;
  GLuint m_frameBuf;
  GLuint m_colorBuf;
  GLuint m_depthBuf;
  GLuint m_stencilBuf;
}
#endif

@synthesize m_pContext;

static GLint stencilBits()
{
  static GLint bits;
  static bool inited = false;
  if (!inited)
  {
    glGetIntegerv(GL_STENCIL_BITS, &bits);
    inited = true;
  }
  return bits;
}

static GLint depthBits()
{
  static GLint bits;
  static bool inited = false;
  if (!inited)
  {
    //glGetIntegerv(GL_DEPTH_BITS, &bits);
    bits = (stencilBits() > 0) ? 24 : 16;
    inited = true;
  }
  return bits;
}

static int extLen(const char *pExtName)
{
  int nLen = 0;
  while (pExtName[nLen])
    nLen++;
  return nLen;
}

static bool parseExtension(const char *pExtName)
{
  if (!pExtName)
    return false;
  const char *pExtensions = (const char*)::glGetString(GL_EXTENSIONS);
  if (!pExtensions)
    return false;
  int start = 0, cur, end;
  for (;;)
  {
    {
      const char *a = pExtensions + start, *b = pExtName;
      const char *pStr = a, *pRv = NULL;
      for (; *pStr != 0 && !pRv; pStr++)
      {
        if (*pStr != *b)
          continue;
        a = pStr;
        for (;;)
        {
          if (*b == 0) {
            pRv = pStr;
            break;
          }
          if (*a++ != *b++)
            break;
        }
        b = pExtName;
      }
      if (pRv)
        cur = (int)(pRv - pExtensions);
      else
        break;
    }
    end = extLen(pExtName) + cur;
    if (cur == 0 || pExtensions[cur - 1] == ' ')
    {
      if (end == extLen(pExtensions) || pExtensions[end] == ' ')
        return true;
    }
    start = end;
  }
  return false;
}

+ (Class)layerClass
{
  return [CAEAGLLayer class];
}

-(void)createFrameBuffers:(CAEAGLLayer*)layer inWidth:(long)width inHeight:(long)height
{
  // Create frame buffer
  ::glGenFramebuffers(1, &m_frameBuf);
  ::glBindFramebuffer(GL_FRAMEBUFFER, m_frameBuf);
  // Create render buffer
  ::glGenRenderbuffers(1, &m_colorBuf);
  ::glBindRenderbuffer(GL_RENDERBUFFER, m_colorBuf);
#if defined(GL_OES_packed_depth_stencil) || defined(GL_OES_depth24)
  bool bRetry = false;
#endif
  if (!m_bOffscreen)
  {
    [m_pContext renderbufferStorage:GL_RENDERBUFFER fromDrawable:layer];
    ::glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_WIDTH, &m_nBackingWidth);
    ::glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_HEIGHT, &m_nBackingHeight);
  }
  else
  {
    m_nBackingWidth = (GLint)width; m_nBackingHeight = (GLint)height;
    ::glRenderbufferStorage(GL_RENDERBUFFER, GL_RGBA8, m_nBackingWidth, m_nBackingHeight);
  }
  ::glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, m_colorBuf);
  // Create depth/stencil buffers
  ::glGenRenderbuffers(1, &m_depthBuf);
  ::glBindRenderbuffer(GL_RENDERBUFFER, m_depthBuf);
#ifdef GL_OES_packed_depth_stencil
  bRetry = false;
  if (::parseExtension("GL_OES_packed_depth_stencil") || ::parseExtension("GL_EXT_packed_depth_stencil"))
  {
    ::glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8_OES, m_nBackingWidth, m_nBackingHeight);
    ::glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_depthBuf);
    if (stencilBits() > 0)
      ::glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_depthBuf);
    bRetry = true;
  }
  if (!bRetry || (::glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE))
  {
    if (bRetry)
    {
      ::glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, 0);
      if (stencilBits() > 0)
        ::glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, 0);
    }
#else
  {
#endif
#ifdef GL_OES_depth24
    bRetry = false;
    if (::parseExtension("GL_OES_depth24") && ::depthBits() > 16)
    {
      ::glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24_OES, m_nBackingWidth, m_nBackingHeight);
      ::glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_depthBuf);
      bRetry = true;
    }
    if (!bRetry || (::glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE))
    {
      if (bRetry)
        ::glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, 0);
#else
    {
#endif
      ::glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, m_nBackingWidth, m_nBackingHeight);
    }
    ::glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_depthBuf);
//#ifndef TARGET_IPHONE_SIMULATOR // Simulator doesn't support stencil buffer
    if (::stencilBits() > 0)
    {
      ::glGenRenderbuffers(1, &m_stencilBuf);
      ::glBindRenderbuffer(GL_RENDERBUFFER, m_stencilBuf);
      bRetry = false;
      if (::stencilBits() > 4)
      {
        ::glRenderbufferStorage(GL_RENDERBUFFER, GL_STENCIL_INDEX8, m_nBackingWidth, m_nBackingHeight);
        ::glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_stencilBuf);
        bRetry = true;
      }
      if (!bRetry || (::glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE))
      {
        if (bRetry)
          ::glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, 0);
        if (::glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        {
          ::glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, 0);
          ::glDeleteRenderbuffers(1, &m_stencilBuf);
          m_stencilBuf = 0;
        }
      }
//#endif
    }
  }
  ::glBindRenderbuffer(GL_RENDERBUFFER, m_colorBuf);
  //ODA_VERIFY(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
}

-(void)deleteFrameBuffers
{
  if (m_stencilBuf)
  {
    ::glDeleteRenderbuffers(1, &m_stencilBuf);
    m_stencilBuf = 0;
  }
  if (m_depthBuf)
  {
    ::glDeleteRenderbuffers(1, &m_depthBuf);
    m_depthBuf = 0;
  }
  if (m_colorBuf)
  {
    ::glDeleteRenderbuffers(1, &m_colorBuf);
    m_colorBuf = 0;
  }
  if (m_frameBuf)
  {
    ::glDeleteFramebuffers(1, &m_frameBuf);
    m_frameBuf = 0;
  }
}

-(id)initializeLocalContext:(GLboolean)isOffscreen inLayer:(CAEAGLLayer*)layer inWidth:(long)width inHeight:(long)height
{
  m_frameBuf = m_colorBuf = m_depthBuf = m_stencilBuf = 0;
  m_bOffscreen = isOffscreen;
  // Create GL context
  m_pContext = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2];
  if (!m_pContext || ![EAGLContext setCurrentContext:m_pContext])
  {
    [self release];
    return nil;
  }
  [self createFrameBuffers:layer inWidth:width inHeight:height];
  return self;
}

-(id)initWithLayer:(CAEAGLLayer*)layer
{
  return [self initializeLocalContext:GL_FALSE inLayer:layer inWidth:0 inHeight:0];
}

-(id)initWithDimensions:(long)width inHeight:(long)height
{
  return [self initializeLocalContext:GL_TRUE inLayer:nil inWidth:width inHeight:height];
}
    
-(void)uninit
{
  [self deleteFrameBuffers];
}

-(bool)isOffscreen
{
  return m_bOffscreen;
}

-(bool)checkDimensions:(long)width inHeight:(long)height
{
  return (m_nBackingWidth == (GLint)width) && (m_nBackingHeight == (GLint)height);
}

-(id)getContext
{
  return m_pContext;
}
@end

// C++ wrappers

void *odGLES2AttachLocalContext(void *pLayer)
{
  id objId = (id)pLayer;
  CAEAGLLayer *pLayer_ = objId;
  return (void*)[[OdGLES2LocalContext alloc] initWithLayer:pLayer_];
}

void *odGLES2AttachLocalContext(long nWidth, long nHeight)
{
  return (void*)[[OdGLES2LocalContext alloc] initWithDimensions:nWidth inHeight:nHeight];
}

void odGLES2DetachLocalContext(void *pObj)
{
  id objId = (id)pObj;
  OdGLES2LocalContext *ctx = objId;
  [ctx uninit];
  [ctx release];
}

void odGLES2UpdateLocalContext(void *pObj, void *pLayer, long nWidth, long nHeight)
{
  id objId = (id)pObj;
  OdGLES2LocalContext *ctx = objId;
  [ctx deleteFrameBuffers];
  if (![ctx isOffscreen])
  {
    id layerId = (id)pLayer;
    CAEAGLLayer *pLayer_ = layerId;
    [ctx createFrameBuffers:pLayer_ inWidth:nWidth inHeight:nHeight];
  }
  else
    [ctx createFrameBuffers:nil inWidth:nWidth inHeight:nHeight];
}

bool odGLES2IsOffscreenLocalContext(void *pObj)
{
  id objId = (id)pObj;
  OdGLES2LocalContext *ctx = objId;
  return [ctx isOffscreen];
}

bool odGLES2CheckDimensionsOfLocalContext(void *pObj, long nWidth, long nHeight)
{
  id objId = (id)pObj;
  OdGLES2LocalContext *ctx = objId;
  return [ctx checkDimensions:nWidth inHeight:nHeight];
}

void odGLES2MakeCurrentLocalContext(void *pObj)
{
  id objId = (id)pObj;
  OdGLES2LocalContext *ctx = objId;
  [EAGLContext setCurrentContext:ctx.m_pContext];
}
    
void odGLES2PresentLocalContext(void *pObj)
{
  id objId = (id)pObj;
  OdGLES2LocalContext *ctx = objId;
  [ctx.m_pContext presentRenderbuffer:GL_RENDERBUFFER];
}

void odGLES2MakeCurrentExternalContext(void *pContext)
{
  id objId = (id)pContext;
  EAGLContext *ctx = objId;
  [EAGLContext setCurrentContext:ctx];
}

void odGLES2PresentExternalContext(void *pContext)
{
  id objId = (id)pContext;
  EAGLContext *ctx = objId;
  [ctx presentRenderbuffer:GL_RENDERBUFFER];
}

void *odGLES2ExtractPointerForLocalContext(void *pObj)
{
  id objId = (id)pObj;
  OdGLES2LocalContext *ctx = objId;
  return (void*)[ctx getContext];
}

//
