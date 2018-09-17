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
#include "OdaDefs.h"
#include "DbFiler.h"
#include "DbHandle.h"
#include "Ge/GeEllipArc3d.h"
#include "ResBuf.h"
#include "Gi/GiWorldDraw.h"

//using namespace OdDb;

#include "IdComplexEntity.h"

ODRX_DEFINE_MEMBERS_EX(OdIdComplexEntity,                                                                            // ClassName
                       OdDbEntity,                                                                                   // ParentClass
                       DBOBJECT_CONSTR,                                                                              // DOCREATE
                       OdDb::vAC21,                                                                                  // DwgVer
                       OdDb::kMRelease1,                                                                             // MaintVer
                       1,                                                                                            // NProxyFlags
                       L"AcIdComplexEntity",                                                                         // DWG class name
                       L"ACIDCOMPLEXENTITY",                                                                         // DxfName
                       L"AcIdViewObj|Product: Inventor Drawing Enabler|Company: Autodesk|Website: www.autodesk.com", // AppName
                       OdRx::kMTLoading|OdRx::kMTRender|OdRx::kMTRenderInBlock | OdRx::kHistoryAware)                                      // CustomFlags

OdResult OdIdComplexEntity::dwgInFields(OdDbDwgFiler* pFiler)
{
  assertWriteEnabled();
  OdDbEntity::dwgInFields(pFiler);
  OdUInt32 count;

  CLineData::iterator   tmpLineData = NULL;
  CCircleData::iterator tmpCircleData = NULL;
  CArcData::iterator    tmpArcData = NULL;

  m_LogLineData = 0;
  m_LineData.resize( 0 );

  m_LogCircleData = 0;
  m_CircleData.resize( 0 );

  m_LogArcData = 0;
  m_ArcData.resize( 0 );

  for( int zd = 0; zd < 6; zd++ )
  {
    OdUInt32  k;
    count = pFiler->rdInt32();
    switch( zd ) 
    {
    case 0:
      m_LogLineData += count;
    case 3:
      for( k = 0; k < count; k++ )
      {
        tmpLineData = m_LineData.append();

        tmpLineData->pnt1 = pFiler->rdPoint3d();
        tmpLineData->pnt2 = pFiler->rdPoint3d();
        tmpLineData->unknownId = pFiler->rdSoftPointerId();
      }
      break;
    case 1:
      m_LogCircleData += count;
    case 4:
      for( k = 0; k < count; k++ )
      {
        tmpCircleData = m_CircleData.append();

        tmpCircleData->pnt = pFiler->rdPoint3d();
        tmpCircleData->vec = pFiler->rdVector3d();
        tmpCircleData->dbl = pFiler->rdDouble();
      }
      break;
    case 2:
      m_LogArcData += count;
    case 5:
      for( k = 0; k < count; k++ )
      {
        tmpArcData = m_ArcData.append();

        tmpArcData->pnt = pFiler->rdPoint3d();
        tmpArcData->vec1 = pFiler->rdVector3d();
        tmpArcData->dbl1 = pFiler->rdDouble();
        tmpArcData->vec2 = pFiler->rdVector3d();
        tmpArcData->dbl2 = pFiler->rdDouble();
      }
      break;
    }
  }

  return eOk;
}

void OdIdComplexEntity::dwgOutFields(OdDbDwgFiler* pFiler) const
{
  assertReadEnabled();
  OdDbEntity::dwgOutFields(pFiler);

  OdUInt32  count = 0;
  OdUInt32  iLineDataIndex = 0;
  OdUInt32  iCircleDataIndex = 0;
  OdUInt32  iArcDataIndex = 0;

  for( int zd = 0; zd < 6; zd++ )
  {
    switch( zd )
    {
    case 0:
      count = m_LogLineData;
      break;
    case 1:
      count = m_LogCircleData;
      break;
    case 2:
      count = m_LogArcData;
      break;
    case 3:
      count = m_LineData.length() - m_LogLineData;
      break;
    case 4:
      count = m_CircleData.length() - m_LogCircleData;
      break;
    case 5:
      count = m_ArcData.length() - m_LogArcData;
      break;
    }

    pFiler->wrInt32( count );

    OdUInt32 k,tmpIndex;
    switch( zd ) 
    {
    case 0:
    case 3:
      for( k = 0, tmpIndex = iLineDataIndex; k < count; k++, tmpIndex++ )
      {
        pFiler->wrPoint3d( m_LineData[tmpIndex].pnt1 );
        pFiler->wrPoint3d( m_LineData[tmpIndex].pnt2 );
        pFiler->wrSoftPointerId( m_LineData[tmpIndex].unknownId );
      }
      iLineDataIndex += count;
      break;
    case 1:
    case 4:
      for( k = 0, tmpIndex = iCircleDataIndex; k < count; k++, tmpIndex++ )
      {
        pFiler->wrPoint3d(m_CircleData[tmpIndex].pnt);
        pFiler->wrVector3d(m_CircleData[tmpIndex].vec);
        pFiler->wrDouble(m_CircleData[tmpIndex].dbl);
      }
      iCircleDataIndex += count;
      break;
    case 2:
    case 5:
      for( k = 0, tmpIndex = iArcDataIndex; k < count; k++, tmpIndex++ )
      {
        pFiler->wrPoint3d(m_ArcData[tmpIndex].pnt);
        pFiler->wrVector3d(m_ArcData[tmpIndex].vec1);
        pFiler->wrDouble(m_ArcData[tmpIndex].dbl1);
        pFiler->wrVector3d(m_ArcData[tmpIndex].vec2);
        pFiler->wrDouble(m_ArcData[tmpIndex].dbl2);
      }
      iArcDataIndex += count;
      break;
    }
  }
}

bool OdIdComplexEntity::subWorldDraw(OdGiWorldDraw* pWd) const
{
  assertReadEnabled();
  OdGeVector3d  vNormal = OdGeVector3d::kZAxis;
  OdGiGeometry& geom = pWd->geometry();

  OdGePoint3dArray    vertexList;
  OdUInt32            counter;

  for( counter = m_LogLineData; counter < m_LineData.length(); counter++ )
  {
    vertexList.append( m_LineData[counter].pnt1 );
    vertexList.append( m_LineData[counter].pnt2 );

    geom.polyline( vertexList.length(), vertexList.asArrayPtr(), &vNormal );
  }

  for( counter = m_LogArcData; counter < m_ArcData.length(); counter++ )
  {
    geom.circularArc( m_ArcData[counter].pnt,
                      m_ArcData[counter].dbl1,
                      m_ArcData[counter].vec1,
                      m_ArcData[counter].vec2,
                      m_ArcData[counter].dbl2 );
  }

  for( counter = m_LogCircleData; counter < m_CircleData.length(); counter++ )
  {
    geom.circle( m_CircleData[counter].pnt, m_CircleData[counter].dbl, m_CircleData[counter].vec );
  }
  return true;
}

OdResult OdIdComplexEntity::subExplode(OdRxObjectPtrArray& /*entitySet*/) const
{
  assertReadEnabled();
  return eOk;
}


void  OdIdComplexEntity::addGraphicalArc(
    OdGePoint3d const &point,
    OdGeVector3d const &v1,
    double const &dbl1,
    OdGeVector3d const &v2,
    double const &dbl2 )
{
  assertWriteEnabled();
  CArcData::iterator    tmpArcData = NULL;

  tmpArcData = m_ArcData.append();

  tmpArcData->pnt   = point;
  tmpArcData->vec1  = v1;
  tmpArcData->dbl1  = dbl1;
  tmpArcData->vec2  = v2;
  tmpArcData->dbl2  = dbl2;
}

void  OdIdComplexEntity::addGraphicalCircle(
    OdGePoint3d const &point,
    OdGeVector3d const &v,
    double const &dbl )
{
  assertWriteEnabled();
  CCircleData::iterator tmpCircleData = NULL;

  tmpCircleData = m_CircleData.append();

  tmpCircleData->pnt  = point;
  tmpCircleData->vec  = v;
  tmpCircleData->dbl  = dbl;
}

void  OdIdComplexEntity::addGraphicalLine(
    OdGePoint3d const *ppoint )
{
  assertWriteEnabled();
  CLineData::iterator   tmpLineData = NULL; 

  tmpLineData = m_LineData.append();

  tmpLineData->pnt1 = ppoint[0];
  tmpLineData->pnt2 = ppoint[1];
  tmpLineData->unknownId = OdDbObjectId::kNull;
}


void  OdIdComplexEntity::addLogicalArc(
    OdGePoint3d const &point,
    OdGeVector3d const &v1,
    double const &dbl1,
    OdGeVector3d const &v2,
    double const &dbl2 )
{
  assertWriteEnabled();
//  CArcData::iterator    tmpArcData = NULL;

  struct _ArcData    tmpArcData;

  tmpArcData.pnt = point;
  tmpArcData.vec1 = v1;
  tmpArcData.dbl1 = dbl1;
  tmpArcData.vec2 = v2;
  tmpArcData.dbl2 = dbl2;

  m_ArcData.insertAt( m_LogArcData, tmpArcData );
  m_LogArcData++;
}

void  OdIdComplexEntity::addLogicalCircle(
    OdGePoint3d const &point,
    OdGeVector3d const &v,
    double const &dbl )
{
  assertWriteEnabled();
  struct _CircleData  tmpCircleData;

  tmpCircleData.pnt = point;
  tmpCircleData.vec = v;
  tmpCircleData.dbl = dbl;

  m_CircleData.insertAt(m_LogCircleData, tmpCircleData);
  m_LogCircleData++;
}

void  OdIdComplexEntity::addLogicalLine(
    OdGePoint3d const *ppoint )
{
  assertWriteEnabled();
  struct _LineData tmpLineData;

  tmpLineData.pnt1 = ppoint[0];
  tmpLineData.pnt2 = ppoint[1];
  tmpLineData.unknownId = OdDbObjectId::kNull;

  m_LineData.insertAt( m_LogLineData, tmpLineData );
  m_LogLineData++;
}
