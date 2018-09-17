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

#include "OdaCommon.h"
#include "ExGsOpenGLVectorizeDevice.h"
#include "TD_GL.h"
#include "SaveState.h"

OD_OPENGL_DECLARE_COMMON_PROPS(ExGsOpenGLVectorizeDeviceBase, OdGsOpenGLStreamVectorizeDevice, OdCommonDeviceProps);

ODRX_DECLARE_PROPERTY2(EAGLProto, ExGsOpenGLVectorizeDevice)

ODRX_BEGIN_DYNAMIC_PROPERTY_MAP(ExGsOpenGLVectorizeDevice);
  ODRX_INHERIT_PROPERTIES(ExGsOpenGLVectorizeDeviceBase)
  ODRX_GENERATE_PROPERTY2(EAGLProto, ExGsOpenGLVectorizeDevice)
ODRX_END_DYNAMIC_PROPERTY_MAP(ExGsOpenGLVectorizeDevice);

ODRX_DEFINE_PROPERTY_METHODS2(EAGLProto, ExGsOpenGLVectorizeDevice, ExGsOpenGLVectorizeDevice, getEAGLProto, setEAGLProto, getIntPtr);

#ifndef GL_OES_framebuffer_object
#error GL_OES_framebuffer_object extension unavailable
#endif

// glext.h header is incompleted for GL_OES_framebuffer_object extension
#ifndef GL_STENCIL_INDEX1_OES
#define GL_STENCIL_INDEX1_OES 0x8D46
#endif
#ifndef GL_STENCIL_INDEX4_OES
#define GL_STENCIL_INDEX4_OES 0x8D47
#endif

bool OpenGLParseExtension(const char *pExtName); // GsOpenGLVectorizer.cpp

//Implementation of class ExGsOpenGLVectorizeDevice
//*******************************************************************************/
// Constructor
//*******************************************************************************/
ExGsOpenGLVectorizeDevice::ExGsOpenGLVectorizeDevice()
  : m_bContextCreated(false)
  , m_pContext(NULL)
  , m_frameBuf(0)
  , m_colorBuf(0)
  , m_depthBuf(0)
  , m_stencilBuf(0)
  , m_backingWidth(0)
  , m_backingHeight(0)
{
}

//*******************************************************************************/
// Destructor
//*******************************************************************************/
ExGsOpenGLVectorizeDevice::~ExGsOpenGLVectorizeDevice()
{
  deleteContext();
}

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

void ExGsOpenGLVectorizeDevice::createFrameBuffers()
{
  // Create frame buffer
  ::glGenFramebuffersOES(1, &m_frameBuf);
  ::glBindFramebufferOES(GL_FRAMEBUFFER_OES, m_frameBuf);
  // Create render buffer
  ::glGenRenderbuffersOES(1, &m_colorBuf);
  ::glBindRenderbufferOES(GL_RENDERBUFFER_OES, m_colorBuf);
  GLint initWidth = width(), initHeight = height();
#if defined(GL_OES_rgb8_rgba8) || defined(GL_OES_packed_depth_stencil) || defined(GL_OES_depth24) || (!defined(TARGET_IPHONE_SIMULATOR) && defined(GL_OES_stencil8))
  bool bRetry = false;
#endif
  if (m_pContext)
  {
    m_pContext->eaglProtoFromDrawable();
    ::glGetRenderbufferParameterivOES(GL_RENDERBUFFER_OES, GL_RENDERBUFFER_WIDTH_OES, &initWidth);
    ::glGetRenderbufferParameterivOES(GL_RENDERBUFFER_OES, GL_RENDERBUFFER_HEIGHT_OES, &initHeight);
  }
  else
  {
    // @@@TODO: probably texture2d must be attached in this situation
#ifdef GL_OES_rgb8_rgba8
    bRetry = false;
    if (OpenGLParseExtension("GL_OES_rgb8_rgba8"))
    {
      ::glRenderbufferStorageOES(GL_RENDERBUFFER_OES, GL_RGBA8_OES, initWidth, initHeight);
      ::glFramebufferRenderbufferOES(GL_FRAMEBUFFER_OES, GL_COLOR_ATTACHMENT0_OES, GL_RENDERBUFFER_OES, m_colorBuf);
      bRetry = true;
    }
    if (!bRetry || (::glCheckFramebufferStatusOES(GL_FRAMEBUFFER_OES) != GL_FRAMEBUFFER_COMPLETE_OES))
    {
      if (bRetry)
        ::glFramebufferRenderbufferOES(GL_FRAMEBUFFER_OES, GL_COLOR_ATTACHMENT0_OES, GL_RENDERBUFFER_OES, 0);
#else
    {
#endif
      ::glRenderbufferStorageOES(GL_RENDERBUFFER_OES, GL_RGB565_OES, initWidth, initHeight);
    }
  }
  ::glFramebufferRenderbufferOES(GL_FRAMEBUFFER_OES, GL_COLOR_ATTACHMENT0_OES, GL_RENDERBUFFER_OES, m_colorBuf);
  // Create depth/stencil buffers
  ::glGenRenderbuffersOES(1, &m_depthBuf);
  ::glBindRenderbufferOES(GL_RENDERBUFFER_OES, m_depthBuf);
#ifdef GL_OES_packed_depth_stencil
  bRetry = false;
  if (OpenGLParseExtension("GL_OES_packed_depth_stencil") || OpenGLParseExtension("GL_EXT_packed_depth_stencil"))
  {
    ::glRenderbufferStorageOES(GL_RENDERBUFFER_OES, GL_DEPTH24_STENCIL8_OES, initWidth, initHeight);
    ::glFramebufferRenderbufferOES(GL_FRAMEBUFFER_OES, GL_DEPTH_ATTACHMENT_OES, GL_RENDERBUFFER_OES, m_depthBuf);
    if (stencilBits() > 0)
      ::glFramebufferRenderbufferOES(GL_FRAMEBUFFER_OES, GL_STENCIL_ATTACHMENT_OES, GL_RENDERBUFFER_OES, m_depthBuf);
    bRetry = true;
  }
  if (!bRetry || (::glCheckFramebufferStatusOES(GL_FRAMEBUFFER_OES) != GL_FRAMEBUFFER_COMPLETE_OES))
  {
    if (bRetry)
    {
      ::glFramebufferRenderbufferOES(GL_FRAMEBUFFER_OES, GL_DEPTH_ATTACHMENT_OES, GL_RENDERBUFFER_OES, 0);
      if (stencilBits() > 0)
        ::glFramebufferRenderbufferOES(GL_FRAMEBUFFER_OES, GL_STENCIL_ATTACHMENT_OES, GL_RENDERBUFFER_OES, 0);
    }
#else
  {
#endif
#ifdef GL_OES_depth24
    bRetry = false;
    if (OpenGLParseExtension("GL_OES_depth24") && depthBits() > 16)
    {
      ::glRenderbufferStorageOES(GL_RENDERBUFFER_OES, GL_DEPTH_COMPONENT24_OES, initWidth, initHeight);
      ::glFramebufferRenderbufferOES(GL_FRAMEBUFFER_OES, GL_DEPTH_ATTACHMENT_OES, GL_RENDERBUFFER_OES, m_depthBuf);
      bRetry = true;
    }
    if (!bRetry || (::glCheckFramebufferStatusOES(GL_FRAMEBUFFER_OES) != GL_FRAMEBUFFER_COMPLETE_OES))
    {
      if (bRetry)
        ::glFramebufferRenderbufferOES(GL_FRAMEBUFFER_OES, GL_DEPTH_ATTACHMENT_OES, GL_RENDERBUFFER_OES, 0);
#else
    {
#endif
      ::glRenderbufferStorageOES(GL_RENDERBUFFER_OES, GL_DEPTH_COMPONENT16_OES, initWidth, initHeight);
    }
    ::glFramebufferRenderbufferOES(GL_FRAMEBUFFER_OES, GL_DEPTH_ATTACHMENT_OES, GL_RENDERBUFFER_OES, m_depthBuf);
#ifndef TARGET_IPHONE_SIMULATOR // Simulator doesn't support stencil buffer
    if (stencilBits() > 0)
    {
      ::glGenRenderbuffersOES(1, &m_stencilBuf);
      ::glBindRenderbufferOES(GL_RENDERBUFFER_OES, m_stencilBuf);
#ifdef GL_OES_stencil8
      bRetry = false;
      if (OpenGLParseExtension("GL_OES_stencil8") && stencilBits() > 4)
      {
        ::glRenderbufferStorageOES(GL_RENDERBUFFER_OES, GL_STENCIL_INDEX8_OES, initWidth, initHeight);
        ::glFramebufferRenderbufferOES(GL_FRAMEBUFFER_OES, GL_STENCIL_ATTACHMENT_OES, GL_RENDERBUFFER_OES, m_stencilBuf);
        bRetry = true;
      }
      if (!bRetry || (::glCheckFramebufferStatusOES(GL_FRAMEBUFFER_OES) != GL_FRAMEBUFFER_COMPLETE_OES))
      {
        if (bRetry)
          ::glFramebufferRenderbufferOES(GL_FRAMEBUFFER_OES, GL_STENCIL_ATTACHMENT_OES, GL_RENDERBUFFER_OES, 0);
#else
      {
#endif
        ::glRenderbufferStorageOES(GL_RENDERBUFFER_OES, GL_STENCIL_INDEX4_OES, initWidth, initHeight);
        ::glFramebufferRenderbufferOES(GL_FRAMEBUFFER_OES, GL_STENCIL_ATTACHMENT_OES, GL_RENDERBUFFER_OES, m_stencilBuf);
      }
      if (::glCheckFramebufferStatusOES(GL_FRAMEBUFFER_OES) != GL_FRAMEBUFFER_COMPLETE_OES)
      {
        ::glFramebufferRenderbufferOES(GL_FRAMEBUFFER_OES, GL_STENCIL_ATTACHMENT_OES, GL_RENDERBUFFER_OES, 0);
        ::glDeleteRenderbuffersOES(1, &m_stencilBuf);
        m_stencilBuf = 0;
      }
    }
#endif
  }
  ODA_VERIFY(::glCheckFramebufferStatusOES(GL_FRAMEBUFFER_OES) == GL_FRAMEBUFFER_COMPLETE_OES);
  m_backingWidth = initWidth; m_backingHeight = initHeight;
}

void ExGsOpenGLVectorizeDevice::deleteFrameBuffers()
{
  if (m_stencilBuf)
  {
    ::glDeleteRenderbuffersOES(1, &m_stencilBuf);
    m_stencilBuf = 0;
  }
  if (m_depthBuf)
  {
    ::glDeleteRenderbuffersOES(1, &m_depthBuf);
    m_depthBuf = 0;
  }
  if (m_colorBuf)
  {
    ::glDeleteRenderbuffersOES(1, &m_colorBuf);
    m_colorBuf = 0;
  }
  if (m_frameBuf)
  {
    ::glDeleteFramebuffersOES(1, &m_frameBuf);
    m_frameBuf = 0;
  }
}

//*******************************************************************************/
//  createContext()
//*******************************************************************************/
void ExGsOpenGLVectorizeDevice::createContext()
{
  createFrameBuffers();
  m_bContextCreated = true;

  onContextCreate();
}

//*******************************************************************************/
// deleteContext()
//*******************************************************************************/
void ExGsOpenGLVectorizeDevice::deleteContext()
{
  onContextDelete();

  deleteFrameBuffers();
  m_bContextCreated = false;
}

OdIntPtr ExGsOpenGLVectorizeDevice::getEAGLProto() const
{
  return (OdIntPtr)m_pContext;
}

void ExGsOpenGLVectorizeDevice::setEAGLProto(OdIntPtr ctx)
{
  m_pContext = (OdEAGLProtocol*)ctx;
}

//*******************************************************************************/
//  update()
//*******************************************************************************/
void ExGsOpenGLVectorizeDevice::update(OdGsDCRect* pRect)
{
  if (!m_bContextCreated)
  {
    createContext();
  }
  else
  {
    if ((width() != m_backingWidth) || (height() != m_backingHeight))
    { // Recreate frame buffers in case if backing dimensions was changed
      deleteFrameBuffers();
      createFrameBuffers();
    }
    ::glBindFramebufferOES(GL_FRAMEBUFFER_OES, m_frameBuf);
  }
  ::glBindRenderbufferOES(GL_RENDERBUFFER_OES, m_colorBuf);

  m_vectPerfData.m_options = (supportMultithreading()) ? eEnableParallelVectorization : 0;
  if (forcePartialUpdateForMt()) m_vectPerfData.m_options |= eForcePartialUpdateForTest;
  mtDeviceSync().setEnabled(m_vectPerfData.enableParallelVectorization());
  if (m_vectPerfData.enableParallelVectorization() && !gsMultithreadEnabled())
    setMaxMtRegenThreads(getMtRegenThreadsCount(userGiContext()));

  ExGsOpenGLVectorizeDeviceBase::update(pRect);

  if (m_pContext)
    m_pContext->eaglProtoPresentRenderBuffer();

  ::glFinish();
}

void ExGsOpenGLVectorizeDevice::updateScreen()
{
  if (!isValid())
    OdGsOpenGLVectorizeDevice::updateScreen();
}

ExGsOpenGLVectorizeView::ExGsOpenGLVectorizeView()
  : OdGsOpenGLStreamVectorizeView()
  , m_bSelecting(false)
{
}

//*******************************************************************************/
//  polylineProc()
//*******************************************************************************/
void ExGsOpenGLVectorizeView::polylineOut(OdInt32 nPoints, const OdGePoint3d* pPoints)
{
  OdGsOpenGLStreamVectorizeView::polylineOut(nPoints, pPoints);
  if (!device()->doubleBufferEnabled() && !device()->forcePartialUpdateForMt())
    ::glFlush();
}

void ExGsOpenGLVectorizeView::meshProc(OdInt32 rows,
                                       OdInt32 columns,
                                       const OdGePoint3d* pVertexList,
                                       const OdGiEdgeData* pEdgeData,
                                       const OdGiFaceData* pFaceData,
                                       const OdGiVertexData* pVertexData)
{
  OdGsOpenGLStreamVectorizeView::meshProc(rows, columns, pVertexList, pEdgeData, pFaceData, pVertexData);
  if (!device()->doubleBufferEnabled() && !device()->forcePartialUpdateForMt())
    ::glFlush();
}

void ExGsOpenGLVectorizeView::shellProc(OdInt32 nbVertex,
                                        const OdGePoint3d* pVertexList,
                                        OdInt32 faceListSize,
                                        const OdInt32* pFaceList,
                                        const OdGiEdgeData* pEdgeData,
                                        const OdGiFaceData* pFaceData,
                                        const OdGiVertexData* pVertexData)
{
  OdGsOpenGLStreamVectorizeView::shellProc(nbVertex, pVertexList, faceListSize, pFaceList, pEdgeData, pFaceData, pVertexData);
  if (!device()->doubleBufferEnabled() && !device()->forcePartialUpdateForMt())
    ::glFlush();
}

void ExGsOpenGLVectorizeView::rasterImageProc(const OdGePoint3d& origin,
                                              const OdGeVector3d& u,
                                              const OdGeVector3d& v,
                                              const OdGiRasterImage* pImage, // image object
                                              const OdGePoint2d* uvBoundary, // may not be null
                                              OdUInt32 numBoundPts,
                                              bool transparency,
                                              double brightness,
                                              double contrast,
                                              double fade)
{
  OdGsOpenGLVectorizeView::rasterImageProc(origin, u, v, pImage, uvBoundary, 
                                           numBoundPts, transparency, brightness, 
                                           contrast, fade);

  if (!device()->doubleBufferEnabled() && !device()->forcePartialUpdateForMt())
    ::glFlush();
}

void ExGsOpenGLVectorizeView::metafileProc(const OdGePoint3d& /*origin*/,
                                           const OdGeVector3d& /*u*/,
                                           const OdGeVector3d& /*v*/,
                                           const OdGiMetafile* /*pMetafile*/,
                                           bool /*bDcAligned*/,
                                           bool /*bAllowClipping*/)
{
}

//void ExGsOpenGLVectorizeView::ownerDrawDc(const OdGePoint3d& _origin,
//                                          const OdGeVector3d& _u,
//                                          const OdGeVector3d& _v,
//                                          const OdGiSelfGdiDrawable* pDrawable,
//                                          bool bDcAligned,
//                                          bool bAllowClipping)
//{
//  OdGsOpenGLStreamVectorizeView::ownerDrawDc(_origin, _u, _v, pDrawable, bDcAligned, bAllowClipping);
//}

void ExGsOpenGLVectorizeView::select(const OdGePoint2d* pts, int nPoints, OdGsSelectionReactor* pReactor, OdGsView::SelectionMode mode)
{
  OdSaveState<bool> ss(m_bSelecting, true);
  OdGsOpenGLStreamVectorizeView::select(pts, nPoints, pReactor, mode);
}
