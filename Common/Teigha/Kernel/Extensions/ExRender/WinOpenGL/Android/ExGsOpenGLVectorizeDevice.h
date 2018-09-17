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

#ifndef ODEXGSOPENGLVECTORIZEDEVICE
#define ODEXGSOPENGLVECTORIZEDEVICE

#include "TD_PackPush.h"

#include "GsOpenGLStreamVectorizer.h"
#include "CommonOpenGLProps.h"

OD_OPENGL_DEFINE_COMMON_PROPS(ExGsOpenGLVectorizeDeviceBase, OdGsOpenGLStreamVectorizeDevice, OdCommonDeviceProps);

class ExGsOpenGLVectorizeDevice : public ExGsOpenGLVectorizeDeviceBase
{
  bool m_bContextCreated;

protected:
  ODRX_USING_HEAP_OPERATORS(ExGsOpenGLVectorizeDeviceBase);

  ExGsOpenGLVectorizeDevice();

  virtual ~ExGsOpenGLVectorizeDevice();

  void update(OdGsDCRect* pUpdatedRect);
  void updateScreen();

  /** \details
    Creates a context for this Device object.
  */
  virtual void createContext();
  virtual void deleteContext();
};

class ExGsOpenGLVectorizeView : public OdGsOpenGLStreamVectorizeView
{
  ExGsOpenGLVectorizeDevice* device()
  {
    return static_cast<ExGsOpenGLVectorizeDevice*>(OdGsOpenGLVectorizeView::device());
  }
  bool m_bSelecting;
public:
  ExGsOpenGLVectorizeView();

  void select(const OdGePoint2d* points, int numPoints, OdGsSelectionReactor* pReactor, OdGsView::SelectionMode mode);

  void polylineOut(OdInt32 numPoints, const OdGePoint3d* vertexList);

  void meshProc(OdInt32 numRows,
    OdInt32 numColumns,
    const OdGePoint3d* vertexList,
    const OdGiEdgeData* pEdgeData = 0,
    const OdGiFaceData* pFaceData = 0,
    const OdGiVertexData* pVertexData = 0);

  void shellProc(OdInt32 numVertices,
    const OdGePoint3d* vertexList,
    OdInt32 faceListSize,
    const OdInt32* faceList,
    const OdGiEdgeData* pEdgeData = 0,
    const OdGiFaceData* pFaceData = 0,
    const OdGiVertexData* pVertexData = 0);

  void rasterImageProc(const OdGePoint3d& origin,
    const OdGeVector3d& u,
    const OdGeVector3d& v,
    const OdGiRasterImage* pImage, 
    const OdGePoint2d* uvBoundary,
    OdUInt32 numBoundPts,
    bool transparency = false,
    double brightness = 50.0,
    double contrast = 50.0,
    double fade = 0.0);

  void metafileProc(const OdGePoint3d& origin,
    const OdGeVector3d& u,
    const OdGeVector3d& v,
    const OdGiMetafile* pMetafile,
    bool dcAligned = true,      
    bool allowClipping = false);

  //void ownerDrawDc(
  //  const OdGePoint3d& origin,
  //  const OdGeVector3d& u,
  //  const OdGeVector3d& v,
  //  const OdGiSelfGdiDrawable* pDrawable,
  //  bool dcAligned = true,      
  //  bool allowClipping = false);
};

#include "TD_PackPop.h"

#endif // ODEXGSOPENGLVECTORIZEDEVICE
