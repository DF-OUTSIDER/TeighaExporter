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
#include "OpenGLExtensions.h"

OD_OPENGL_DECLARE_COMMON_PROPS(ExGsOpenGLVectorizeDeviceBase, _undefined_, OdCommonDeviceProps);

ODRX_DECLARE_PROPERTY2(XDisplay, ExGsOpenGLVectorizeDevice)
ODRX_DECLARE_PROPERTY2(XWindow, ExGsOpenGLVectorizeDevice)
ODRX_DECLARE_PROPERTY2(XChooseVisual, ExGsOpenGLVectorizeDevice)
ODRX_DECLARE_PROPERTY2(GLXContext, ExGsOpenGLVectorizeDevice)

ODRX_BEGIN_DYNAMIC_PROPERTY_MAP(ExGsOpenGLVectorizeDevice);
  ODRX_INHERIT_PROPERTIES(ExGsOpenGLVectorizeDeviceBase)
  ODRX_GENERATE_PROPERTY2(XDisplay, ExGsOpenGLVectorizeDevice)
  ODRX_GENERATE_PROPERTY2(XWindow, ExGsOpenGLVectorizeDevice)
  ODRX_GENERATE_PROPERTY2(XChooseVisual, ExGsOpenGLVectorizeDevice)
  ODRX_GENERATE_PROPERTY2(GLXContext, ExGsOpenGLVectorizeDevice)
ODRX_END_DYNAMIC_PROPERTY_MAP(ExGsOpenGLVectorizeDevice);

ODRX_DEFINE_PROPERTY_METHODS2(XDisplay, ExGsOpenGLVectorizeDevice, ExGsOpenGLVectorizeDevice, getXDisplay, setXDisplay, getIntPtr);
ODRX_DEFINE_PROPERTY_METHODS2(XWindow, ExGsOpenGLVectorizeDevice, ExGsOpenGLVectorizeDevice, getXWindow, setXWindow, getIntPtr);
ODRX_DEFINE_PROPERTY_METHODS2(XChooseVisual, ExGsOpenGLVectorizeDevice, ExGsOpenGLVectorizeDevice, chooseVisual, setChooseVisual, getIntPtr);
ODRX_DEFINE_PROPERTY_METHODS2(GLXContext, ExGsOpenGLVectorizeDevice, ExGsOpenGLVectorizeDevice, getGLXContext, setGLXContext, getIntPtr);

// Implementation of class ExGsOpenGLVectorizeDevice

//*******************************************************************************/
// Constructor
//*******************************************************************************/
ExGsOpenGLVectorizeDevice::ExGsOpenGLVectorizeDevice()
  : m_bContextCreated(false)
{
  m_xConnection.display = NULL;
  m_xConnection.window = 0;
  m_xConnection.vinfo = NULL;
  m_context = 0;
}

//*******************************************************************************/
// Destructor
//*******************************************************************************/
ExGsOpenGLVectorizeDevice::~ExGsOpenGLVectorizeDevice()
{
  deleteContext();
}

OdIntPtr ExGsOpenGLVectorizeDevice::chooseVisual() const
{
  int attributes[] =
  {
    GLX_USE_GL,
    GLX_RGBA,
    GLX_RED_SIZE, 8,
    GLX_GREEN_SIZE, 8,
    GLX_BLUE_SIZE, 8,
    GLX_DEPTH_SIZE, 24,
    GLX_STENCIL_SIZE, 2,
    (m_bDoubleBufferEnabled) ? GLX_DOUBLEBUFFER : 0,
    0
  };
  ODA_ASSERT(m_xConnection.display != NULL);
  XVisualInfo *vInfo = m_xConnection.vinfo = glXChooseVisual(m_xConnection.display, DefaultScreen(m_xConnection.display), attributes);
  // ODA_ASSERT(vInfo != NULL);
  return (OdIntPtr)vInfo;
}

//*******************************************************************************/
//  createContext()
//*******************************************************************************/
void ExGsOpenGLVectorizeDevice::createContext()
{
  if (!m_context)
  {
    ODA_ASSERT(m_xConnection.display != NULL && m_xConnection.vinfo != NULL);
    m_context = glXCreateContext(m_xConnection.display, m_xConnection.vinfo, 0, true);
    ODA_ASSERT(m_context != 0);
    m_bContextCreated = true;
  }
  glXMakeCurrent(m_xConnection.display, m_xConnection.window, m_context);

  onContextCreate();
}

//*******************************************************************************/
// deleteContext()
//*******************************************************************************/
void ExGsOpenGLVectorizeDevice::deleteContext()
{
  if (m_context)
  {
    if (m_context != glXGetCurrentContext())
      glXMakeCurrent(m_xConnection.display, m_xConnection.window, m_context);
    onContextDelete();

    // make the rendering context not current
    // glXMakeCurrent(m_xConnection.display, m_xConnection.window, 0); // crash

    // delete the rendering context
    if (m_bContextCreated)
      glXDestroyContext(m_xConnection.display, m_context);
    m_context = 0;
  }
}

void ExGsOpenGLVectorizeDevice::preReleaseResource()
{
  if (m_context)
    glXMakeCurrent(m_xConnection.display, m_xConnection.window, m_context);
}
void ExGsOpenGLVectorizeDevice::postReleaseResource() { }

OdIntPtr ExGsOpenGLVectorizeDevice::getXDisplay() const
{
  return (OdIntPtr)m_xConnection.display;
}

void ExGsOpenGLVectorizeDevice::setXDisplay(OdIntPtr dp)
{
  m_xConnection.display = (Display*)dp;
}

OdIntPtr ExGsOpenGLVectorizeDevice::getXWindow() const
{
  return (OdIntPtr)m_xConnection.window;
}

void ExGsOpenGLVectorizeDevice::setXWindow(OdIntPtr win)
{
  m_xConnection.window = (Window)win;
}

void ExGsOpenGLVectorizeDevice::setChooseVisual(OdIntPtr vinfo)
{
  m_xConnection.vinfo = (XVisualInfo*)vinfo;
}

OdIntPtr ExGsOpenGLVectorizeDevice::getGLXContext() const
{
  return (OdIntPtr)m_context;
}

void ExGsOpenGLVectorizeDevice::setGLXContext(OdIntPtr ctx)
{
  m_context = (GLXContext)ctx;
}

bool ExGsOpenGLVectorizeDevice::checkExtensionFuncSupport(OpenGLExtension /*extIdx*/) const
{
#ifdef GLX_ARB_get_proc_address
  // All extension functions provided
  return true;
#else
  return false;
#endif
}

void *ExGsOpenGLVectorizeDevice::getExtensionFuncPtrFromListInt(const char *pFuncName) const
{
#ifdef GLX_ARB_get_proc_address
  // We've always have similar internal and external prototypes on Windows
  return (void*)glXGetProcAddressARB((const GLubyte*)pFuncName);
#else
  return NULL;
#endif
}

//*******************************************************************************/
//  update()
//*******************************************************************************/
void ExGsOpenGLVectorizeDevice::update(OdGsDCRect* pRect)
{
  if (!m_context)
  {
    createContext();
  }
  else
  {
    glXMakeCurrent(m_xConnection.display, m_xConnection.window, m_context);
  }

  m_vectPerfData.m_options = (supportMultithreading()) ? eEnableParallelVectorization : 0;
  if (forcePartialUpdateForMt()) m_vectPerfData.m_options |= eForcePartialUpdateForTest;
  mtDeviceSync().setEnabled(m_vectPerfData.enableParallelVectorization());
  if (m_vectPerfData.enableParallelVectorization() && !gsMultithreadEnabled())
    setMaxMtRegenThreads(getMtRegenThreadsCount(userGiContext()));

  ExGsOpenGLVectorizeDeviceBase::update(pRect);

  if (isClearScreenEnabled())
  {
    glXSwapBuffers(m_xConnection.display, m_xConnection.window);
  }

  ::glFinish();
}

void ExGsOpenGLVectorizeDevice::updateScreen()
{
  if (!isValid())
    OdGsOpenGLVectorizeDevice::updateScreen();
}

ExGsOpenGLVectorizeView::ExGsOpenGLVectorizeView()
  : ExGsOpenGLVectorizeViewBase()
  , m_bSelecting(false)
{
}

//*******************************************************************************/
//  polylineProc()
//*******************************************************************************/
void ExGsOpenGLVectorizeView::polylineOut(OdInt32 nPoints, const OdGePoint3d* pPoints)
{
  ExGsOpenGLVectorizeViewBase::polylineOut(nPoints, pPoints);
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
  ExGsOpenGLVectorizeViewBase::meshProc(rows, columns, pVertexList, pEdgeData, pFaceData, pVertexData);
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
  ExGsOpenGLVectorizeViewBase::shellProc(nbVertex, pVertexList, faceListSize, pFaceList, pEdgeData, pFaceData, pVertexData);
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
//  ExGsOpenGLVectorizeViewBase::ownerDrawDc(_origin, _u, _v, pDrawable, bDcAligned, bAllowClipping);
//}

void ExGsOpenGLVectorizeView::select(const OdGePoint2d* pts, int nPoints, OdGsSelectionReactor* pReactor, OdGsView::SelectionMode mode)
{
  if (device()->m_context != 0)
    glXMakeCurrent(device()->m_xConnection.display, device()->m_xConnection.window, device()->m_context);
  OdSaveState<bool> ss(m_bSelecting, true);
  ExGsOpenGLVectorizeViewBase::select(pts, nPoints, pReactor, mode);
}

#include "SnapshotSupport.h"

OdGiRasterImagePtr ExGsOpenGLVectorizeDevice::snapshotRegion(const OdGsDCRect &area, bool bCrop) const
{
  if (!m_context)
    return OdGiRasterImagePtr();
  glXMakeCurrent(m_xConnection.display, m_xConnection.window, m_context);
  return OdGsOpenGLVectorizeDevice::snapshotRegion(area, bCrop);
}
