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


#include "HoopsExportCommon.h"
#include "DbBaseDatabase.h"
#include "HoopsExportDevice.h"
#include "HoopsExportView.h"

//////////////////////////////////////////////////////////////////////////////////
//                             HoopsExportDevice                                //
//////////////////////////////////////////////////////////////////////////////////

namespace TD_HOOPS_COMMON_EXPORT {

#define INVALIDEXTENTS 1.0e20  /* {Secret} */

OdHoopsExportDevice::OdHoopsExportDevice() : 
  m_pHoops( NULL ),
  m_bRecording( false )
{
  m_bRayExport   = false;
  m_bXLineExport = false;
  m_bExportGeometryOnly = false;
  m_bInitNormal = false;
  m_bSkipLines  = false;
  m_bSetCamera  = false;
  m_bInitCamera = false;

  m_pCurMapperItem = OdGiMapperRenderItem::createObject();
}

//==================================================================\\

OdHoopsExportDevice::~OdHoopsExportDevice()
{

}

//==================================================================\\

void OdHoopsExportDevice::setExportImpl(CHoopsExportImpl* pHoops)
{
  m_pHoops = pHoops;
  dc_init();
}

//==================================================================\\

CHoopsExportImpl* OdHoopsExportDevice::getExportImpl()
{
  return m_pHoops;
}

//==================================================================\\

OdGsDevicePtr OdHoopsExportDevice::createObject()
{
  OdGsDevicePtr pRes = OdRxObjectImpl<OdHoopsExportDevice, OdGsDevice>::createObject();
  return pRes;
}

//===================================================================\\

OdGsViewPtr OdHoopsExportDevice::createView(
  const OdGsClientViewInfo* pViewInfo, 
  bool bEnableLayVisPerView )
{
  OdGsViewPtr pView = OdHoopsExportView::createObject();
  OdHoopsExportView* pMyView = static_cast<OdHoopsExportView*>(pView.get());

  pMyView->init(this, pViewInfo, bEnableLayVisPerView);
  pMyView->setMode( OdGsView::kGouraudShaded );

  pMyView->output().setDestGeometry(*this);

  return pMyView;
}

//===================================================================\\

void OdHoopsExportDevice::onTraitsModified(const OdGiSubEntityTraitsData& traits)
{
  m_curTraits = traits;
}

//===================================================================\\

void OdHoopsExportDevice::setRenderDevice(OdGsDevice* pRenderDev) 
{ 
  m_pRenderDevice = pRenderDev; 
}

//===================================================================\\

void OdHoopsExportDevice::update(OdGsDCRect* pUpdatedRect)
{
  OdGsBaseVectorizeDevice::update(pUpdatedRect);
}

//===================================================================\\

void OdHoopsExportDevice::polylineOut(OdInt32 nPoints, const OdGePoint3d* pVertexList)
{
  if( getRayExport() )
  {
    if( nPoints > 1)
      dc_ray( pVertexList[0], pVertexList[1] );

    setRayExport( false );
  }
  else if( getXLineExport() )
  {
    if( nPoints > 1)
      dc_xline( pVertexList[0], pVertexList[1] );

    setXLineExport( false );
  }
  else if( nPoints )
    dc_polyline(nPoints, pVertexList);
}

//===================================================================\\

void OdHoopsExportDevice::xlineProc( const OdGePoint3d& p1, const OdGePoint3d& p2 )
{
  dc_xline( p1, p2 );
}

//===================================================================\\

void OdHoopsExportDevice::rayProc( const OdGePoint3d& p1, const OdGePoint3d& p2 )
{
  dc_ray( p1, p2 );
}

//===================================================================\\

void OdHoopsExportDevice::circleProc(const OdGePoint3d& center,
  double radius, 
  const OdGeVector3d& normal,
  const OdGeVector3d* )
{
  OdGeCircArc3d circArc(center, normal, radius);
  dc_circleArc( circArc );
}

//===================================================================\\

void OdHoopsExportDevice::circleProc(const OdGePoint3d& firstPoint,
  const OdGePoint3d& secondPoint, 
  const OdGePoint3d& thirdPoint,
  const OdGeVector3d* )
{
  OdGeCircArc3d circArc;
  OdGeError error;
  circArc.set(firstPoint, secondPoint, thirdPoint, error);

  if( error == OdGe::kOk )
    dc_circleArc( circArc );
}

//===================================================================\\

void OdHoopsExportDevice::circularArcProc(
  const OdGePoint3d& center,
  double radius,
  const OdGeVector3d& normal,
  const OdGeVector3d& startVector,
  double sweepAngle,
  OdGiArcType,
  const OdGeVector3d* )
{
  OdGeCircArc3d circArc(center, normal, startVector.normal(), radius, 0., sweepAngle);
  dc_circleArc( circArc );
}

//===================================================================\\

void OdHoopsExportDevice::circularArcProc(
  const OdGePoint3d& firstPoint,
  const OdGePoint3d& secondPoint,
  const OdGePoint3d& thirdPoint,
  OdGiArcType,
  const OdGeVector3d* )
{
  OdGeCircArc3d circArc;
  OdGeError error;
  circArc.set(firstPoint, secondPoint, thirdPoint, error);

  if( error == OdGe::kOk )
    dc_circleArc( circArc );
}

//===================================================================\\

void OdHoopsExportDevice::ellipArcProc(
  const OdGeEllipArc3d& ellipArc,
  const OdGePoint3d* /*endPointOverrides*/,
  OdGiArcType,
  const OdGeVector3d*)
{
  dc_ellipArc( ellipArc );
}

//===================================================================\\

void OdHoopsExportDevice::nurbsProc(const OdGeNurbCurve3d& nurbsCurve)
{
  dc_nurbs( nurbsCurve );
}

//===================================================================\\

void OdHoopsExportDevice::textProc(
  const OdGePoint3d& position,
  const OdGeVector3d& u, 
  const OdGeVector3d& v,
  const OdChar* msg, 
  OdInt32 length, 
  bool raw,
  const OdGiTextStyle* pTextStyle,
  const OdGeVector3d* pExtrusion )
{
  dc_text( position, u, v, msg, length, raw, pTextStyle, pExtrusion );
}

//===================================================================\\

void OdHoopsExportDevice::polygonOut(
  OdInt32 numPoints, 
  const OdGePoint3d* vertexList, 
  const OdGeVector3d* pNormal )
{
  dc_polygon( numPoints, vertexList, pNormal );
}

//===================================================================\\

void OdHoopsExportDevice::rasterImageProc(
  const OdGePoint3d& origin,
  const OdGeVector3d& u,
  const OdGeVector3d& v,
  const OdGiRasterImage* pImage, 
  const OdGePoint2d* uvBoundary, 
  OdUInt32           numBoundPts,
  bool transparency,
  double brightness,
  double contrast,
  double fade)
{
  dc_image( origin, u, v, pImage, uvBoundary, numBoundPts, transparency, brightness, contrast, fade );
}

//===================================================================

void OdHoopsExportDevice::polylineProc(OdInt32 numPoints, 
                          const OdGePoint3d* vertexList,
                          const OdGeVector3d* pNormal, 
                          const OdGeVector3d* pExtrusion,
                          OdGsMarker baseSubEntMarker )
{
  if( pNormal )
  {
    m_bInitNormal = true;
    m_vrNormal = *pNormal;
  }
  else
    m_bInitNormal = false;

  OdGiGeometrySimplifier::polylineProc( numPoints, vertexList,pNormal,pExtrusion, baseSubEntMarker);
}

//===================================================================\\

void OdHoopsExportDevice::shellProc(
  OdInt32 numVertices,
  const OdGePoint3d* vertexList,
  OdInt32 faceListSize,
  const OdInt32* faceList,
  const OdGiEdgeData* pEdgeData,
  const OdGiFaceData* pFaceData,
  const OdGiVertexData* pVertexData )
{
  m_pShellVertexData = pVertexData;
  dc_shell( numVertices, vertexList, faceListSize, faceList, pEdgeData, pFaceData, pVertexData );
}

//===================================================================\\

void OdHoopsExportDevice::meshProc(
  OdInt32 numRows,
  OdInt32 numColumns,
  const OdGePoint3d* vertexList,
  const OdGiEdgeData* pEdgeData,
  const OdGiFaceData* pFaceData,
  const OdGiVertexData* pVertexData )
{
  m_pShellVertexData = pVertexData;
  dc_mesh( numRows, numColumns, vertexList, pEdgeData, pFaceData, pVertexData );
}

//===================================================================\\

void OdHoopsExportDevice::plineProc(
  const OdGiPolyline& lwBuf,
  const OdGeMatrix3d* pXform,
  OdUInt32 fromIndex,
  OdUInt32 numSegs)
{
  OdGiGeometrySimplifier::plineProc( lwBuf, pXform, fromIndex, numSegs );
}

//===================================================================\\

void OdHoopsExportDevice::initTexture(const OdGePoint3d& origin, 
                                      const OdGeVector3d& u, 
                                      const OdGeVector3d& v,
                                      const OdGiRasterImage* pImage, 
                                      bool transparency, 
                                      double brightness,
                                      double contrast, 
                                      double fade )
{
  OdGiGeometrySimplifier::initTexture( origin, u, v, pImage, transparency, brightness, contrast, fade );
}

//====================================================================

bool OdHoopsExportDevice::getSavedExtents( const OdGiDrawable* pObject, OdGsView* pView, OdGeExtents3d& extents )
{
  bool bRet = false;

  for( OdUInt32 i = 0; i < m_extentsArr.size(); i++ )
  {
    if( m_extentsArr[i].pObject == pObject && pView == m_extentsArr[i].pView )
    {
      extents = m_extentsArr[i].extents;
      bRet = true;
      break;
    }
  }

  return bRet;
}

//====================================================================

void OdHoopsExportDevice::saveExtents( const OdGiDrawable* pObject, OdGsView* pView, OdGeExtents3d& extents )
{
  CHoopsExtentsKeeper tmp;
  tmp.pObject = pObject;
  tmp.extents = extents;
  tmp.pView   = pView;
  m_extentsArr.push_back(tmp);
}

#undef INVALIDEXTENTS

//====================================================================

void OdHoopsExportDevice::triangleOut( const OdInt32* p3Vertices, const OdGeVector3d* pNormal )
{
  m_iTriangleIndexes[0] = p3Vertices[0];
  m_iTriangleIndexes[1] = p3Vertices[1];
  m_iTriangleIndexes[2] = p3Vertices[2];

  m_bTriangleOut = true;
  OdGiGeometrySimplifier::triangleOut( p3Vertices, pNormal );
  m_bTriangleOut = false;
}
}
//====================================================================
