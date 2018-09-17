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
// For extensionFuncPtr
#include <mach-o/dyld.h>

OD_OPENGL_DECLARE_COMMON_PROPS(ExGsOpenGLVectorizeDeviceBase, _undefined_, OdCommonDeviceProps);

ODRX_DECLARE_PROPERTY2(ContextCreated, ExGsOpenGLVectorizeDevice)

ODRX_BEGIN_DYNAMIC_PROPERTY_MAP(ExGsOpenGLVectorizeDevice);
  ODRX_INHERIT_PROPERTIES(ExGsOpenGLVectorizeDeviceBase)
  ODRX_GENERATE_PROPERTY2(ContextCreated, ExGsOpenGLVectorizeDevice)
ODRX_END_DYNAMIC_PROPERTY_MAP(ExGsOpenGLVectorizeDevice);

ODRX_DEFINE_PROPERTY_METHODS2(ContextCreated, ExGsOpenGLVectorizeDevice, ExGsOpenGLVectorizeDevice, isContextCreated, setContextCreated, getBool);

// Implementation of class ExGsOpenGLVectorizeDevice

//*******************************************************************************/
// Constructor
//*******************************************************************************/
ExGsOpenGLVectorizeDevice::ExGsOpenGLVectorizeDevice()
  : m_bContextCreated(false)
{
}

//*******************************************************************************/
// Destructor
//*******************************************************************************/
ExGsOpenGLVectorizeDevice::~ExGsOpenGLVectorizeDevice()
{
  deleteContext();
}

//*******************************************************************************/
//  createContext()
//*******************************************************************************/
void ExGsOpenGLVectorizeDevice::createContext()
{
  m_bContextCreated = true;

  onContextCreate();
}

//*******************************************************************************/
// deleteContext()
//*******************************************************************************/
void ExGsOpenGLVectorizeDevice::deleteContext()
{
  onContextDelete();

  m_bContextCreated = false;
}

static void* AppleGLGetProcAddress(const GLubyte *name)
{
  static const struct mach_header* image = NULL;
  NSSymbol symbol;
  char* symbolName;
  if (NULL == image)
  {
    image = NSAddImage("/System/Library/Frameworks/OpenGL.framework/Versions/Current/OpenGL", NSADDIMAGE_OPTION_RETURN_ON_ERROR);
  }
  /* prepend a '_' for the Unix C symbol mangling convention */
  symbolName = (char*)malloc(strlen((const char*)name) + 2);
  strcpy(symbolName + 1, (const char*)name);
  symbolName[0] = '_';
  symbol = NULL;
  /* if (NSIsSymbolNameDefined(symbolName))
   symbol = NSLookupAndBindSymbol(symbolName); */
  symbol = image ? NSLookupSymbolInImage(image, symbolName, NSLOOKUPSYMBOLINIMAGE_OPTION_BIND | NSLOOKUPSYMBOLINIMAGE_OPTION_RETURN_ON_ERROR) : NULL;
  free(symbolName);
  return symbol ? NSAddressOfSymbol(symbol) : NULL;
}

bool ExGsOpenGLVectorizeDevice::checkExtensionFuncSupport(OpenGLExtension /*extIdx*/) const
{
  return true;
}

void *ExGsOpenGLVectorizeDevice::getExtensionFuncPtrFromListInt(const char *pFuncName) const
{
  return AppleGLGetProcAddress((const GLubyte*)pFuncName);
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

  m_vectPerfData.m_options = (supportMultithreading()) ? eEnableParallelVectorization : 0;
  if (forcePartialUpdateForMt()) m_vectPerfData.m_options |= eForcePartialUpdateForTest;
  mtDeviceSync().setEnabled(m_vectPerfData.enableParallelVectorization());
  if (m_vectPerfData.enableParallelVectorization() && !gsMultithreadEnabled())
    setMaxMtRegenThreads(getMtRegenThreadsCount(userGiContext()));

  ExGsOpenGLVectorizeDeviceBase::update(pRect);

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
  OdSaveState<bool> ss(m_bSelecting, true);
  ExGsOpenGLVectorizeViewBase::select(pts, nPoints, pReactor, mode);
}
