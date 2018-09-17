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

//Implementation of class ExGsOpenGLVectorizeDevice
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
  /* For Android platforms context could be created only from Java layer, so here is only stub.
     Recommended buffer settings: RGBA/depth/stencil enabled. Look for examples.                */
  m_bContextCreated = true;

  onContextCreate();
}

//*******************************************************************************/
// deleteContext()
//*******************************************************************************/
void ExGsOpenGLVectorizeDevice::deleteContext()
{
  m_bContextCreated = false;

  onContextDelete();
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
  /* Note: context should be active here. It is activated automatically from Java layer. */

  m_vectPerfData.m_options = (supportMultithreading()) ? eEnableParallelVectorization : 0;
  if (forcePartialUpdateForMt()) m_vectPerfData.m_options |= eForcePartialUpdateForTest;
  mtDeviceSync().setEnabled(m_vectPerfData.enableParallelVectorization());
  if (m_vectPerfData.enableParallelVectorization() && !gsMultithreadEnabled())
    setMaxMtRegenThreads(getMtRegenThreadsCount(userGiContext()));

  ExGsOpenGLVectorizeDeviceBase::update(pRect);

  /* Note: the presentation will be done by Java layer */

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
