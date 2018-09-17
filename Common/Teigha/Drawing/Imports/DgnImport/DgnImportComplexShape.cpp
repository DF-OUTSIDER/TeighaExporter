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

#include <OdaCommon.h>
#include "DgnImportComplexShape.h"
#include "DgnImportCommon.h"
#include "DgnImportContext.h"
#include "DgnImportPatterns.h"
#include "DbRegion.h"
#include <DgComplexShape.h>
#include <DbPolyline.h>
#include <DbArc.h>
#include <DbLine.h>

namespace TD_DGN_IMPORT {

//----------------------------------------------------------------------------------------------

void reverseCurve( OdDbCurvePtr& pCurve )
{
  if( pCurve->isKindOf( OdDbArc::desc() ) )
  {
    OdDbArcPtr pArc = pCurve;
    pArc->setNormal( -pArc->normal() );
    double dSatrtAngle = pArc->startAngle();
    pArc->setStartAngle( OdaPI - pArc->endAngle() );
    pArc->setEndAngle( OdaPI - dSatrtAngle );
  }
  else if( pCurve->isKindOf( OdDbLine::desc() ) )
  {
    OdDbLinePtr pLine = pCurve;
    OdGePoint3d ptTmp = pLine->startPoint();
    pLine->setStartPoint( pLine->endPoint() );
    pLine->setEndPoint( ptTmp );
  }
  else
  {
    pCurve->reverseCurve();
  }
}

//----------------------------------------------------------------------------------------------

void prepareCurvesToRegionCreation( OdRxObjectPtrArray& arrCurves, OdDbBlockTableRecord* owner )
{
  OdGePoint3d LoopStartPoint;
  OdGePoint3d PrevEndPoint;
  OdGePoint3d SegEndPoint;
  OdGePoint3d SegStartPoint;

  bool bStart = true;

  for( OdUInt32 i = 0; i < arrCurves.size(); i++ )
  {
    OdDbCurvePtr pCurve = OdDbCurve::cast(arrCurves[i]);

    pCurve->getStartPoint(SegStartPoint);
    pCurve->getEndPoint(SegEndPoint);

    if( bStart )
    {
      LoopStartPoint = SegStartPoint;
      bStart = false;

      if( SegEndPoint.isEqualTo(LoopStartPoint) )
      {
        bStart = true;
      }
    }
    else
    {
      if( PrevEndPoint.isEqualTo( SegEndPoint ) )
      {
        reverseCurve( pCurve );

        OdGePoint3d ptTmp = SegStartPoint;
        SegStartPoint = SegEndPoint;
        SegEndPoint   = ptTmp;
      }
      else if( !PrevEndPoint.isEqualTo( SegStartPoint ) )
      {
        bool bCreateConnectionLine = true;

        if( i == 1 )
        {
          if( LoopStartPoint.isEqualTo( SegStartPoint) )
          {
            OdDbCurvePtr pStartCurve = OdDbCurve::cast(arrCurves[0]);
            reverseCurve( pStartCurve );

            OdGePoint3d ptTmp = LoopStartPoint;
            LoopStartPoint = PrevEndPoint;
            PrevEndPoint   = ptTmp;

            bCreateConnectionLine = false;
          }
          else if( LoopStartPoint.isEqualTo( SegEndPoint) )
          {
            OdDbCurvePtr pStartCurve = OdDbCurve::cast(arrCurves[0]);
            reverseCurve( pStartCurve );

            reverseCurve( pCurve );

            OdGePoint3d ptTmp = SegStartPoint;
            SegStartPoint = SegEndPoint;
            SegEndPoint   = ptTmp;

            ptTmp = LoopStartPoint;
            LoopStartPoint = PrevEndPoint;
            PrevEndPoint   = ptTmp;

            bCreateConnectionLine = false;
          }
        }

        if( bCreateConnectionLine )
        {
          OdDbLinePtr pLine = OdDbLine::createObject();

          if( PrevEndPoint.distanceTo( SegEndPoint ) < PrevEndPoint.distanceTo( SegStartPoint ) )
          {
            pLine->setStartPoint( PrevEndPoint );
            pLine->setEndPoint( SegEndPoint );

            owner->appendOdDbEntity( pLine );
            pLine->erase(true);

            OdDbCurvePtr pCurCurve = OdDbCurve::cast(arrCurves[i]);
            reverseCurve( pCurCurve );

            OdGePoint3d ptTmp = SegStartPoint;
            SegStartPoint = SegEndPoint;
            SegEndPoint   = ptTmp;
          }
          else
          {
            pLine->setStartPoint( PrevEndPoint );
            pLine->setEndPoint( SegStartPoint );

            owner->appendOdDbEntity( pLine );
            pLine->erase(true);
          }

          arrCurves.insert( arrCurves.begin() + i , (OdRxObjectPtr)(pLine) );
          i++;
        }
      }

      if( SegEndPoint.isEqualTo(LoopStartPoint) )
      {
        bStart = true;
      }
    }

    PrevEndPoint = SegEndPoint;
  }

  if( !bStart )
  {
    OdDbLinePtr pLine = OdDbLine::createObject();

    pLine->setStartPoint( PrevEndPoint );
    pLine->setEndPoint( LoopStartPoint );

    owner->appendOdDbEntity( pLine );
    pLine->erase(true);

    arrCurves.push_back( (OdRxObjectPtr)(pLine) );
  }
}

//----------------------------------------------------------------------------------------------

bool createComplexPolyline( OdRxObjectPtrArray& arrCurves, OdDbPolylinePtr& pPolyline, double dDeviation = 1e-6, double dThickness = 0 )
{
  bool bRet = false;

  OdGePoint3dArray arrPts;

  for( OdUInt32 i = 0; i < arrCurves.size(); i++ )
  {
    if( arrCurves[i]->isKindOf(OdDbCurve::desc() ) )
    {
      OdDbCurvePtr pDbCurve = arrCurves[i];

      OdGeCurve3d* pCurve = NULL;

      try
      {
        pDbCurve->getOdGeCurve( pCurve );

        if( pCurve )
        {
          OdGeInterval curveInterval;
          pCurve->getInterval( curveInterval );

          OdGePoint3dArray arrCurPts;
          pCurve->getSamplePoints( &curveInterval, dDeviation, arrCurPts );

          arrPts.append( arrCurPts );
        }
      }
      catch(...)
      {
        if( pCurve )
        {
          delete pCurve;
        }

        continue;
      }

      if( pCurve )
      {
        delete pCurve;
      }
    }
  }

  if( arrPts.size() > 1 )
  {
    bRet = createVertexPolyline( arrPts, pPolyline, dThickness );
  }

  return bRet;
}

//----------------------------------------------------------------------------------------------

void OdDgComplexShapeImportPE::subImportElement(OdDgElement* e, OdDbBlockTableRecord* owner)
{
  OdDgComplexShape* pComplexShape = (OdDgComplexShape*)e;

  OdUInt8 uShapeImportMode = OdDgnImportContext::getShape2dImportMode();

  if( pComplexShape->get3dFormatFlag() )
  {
    uShapeImportMode = OdDgnImportContext::getShape3dImportMode();
  }

  bool     bUseFillColor   = false;

  OdRxObjectPtrArray arrFillLinkages;

  pComplexShape->getLinkages( OdDgAttributeLinkage::kFillStyle, arrFillLinkages );

  for( OdUInt32 i = 0; i < arrFillLinkages.size(); i++ )
  {
    if( arrFillLinkages[i]->isKindOf(OdDgFillColorLinkage::desc() ) )
    {
      bUseFillColor = true;
      break;
    }
  }

  bool bImportAsPolyline = OdDgnImportContext::getImportCurvesOnlyFlag();

  if( (uShapeImportMode == 2) && !bImportAsPolyline )
  {
    OdDgnImportPE::subImportElement( e, owner );
    return;
  }

  OdDgElementIteratorPtr pIter = pComplexShape->createIterator();

  OdDbPolylinePtr pPolylineLine;

  if( !createPolylineFromComplexElement( pComplexShape, pIter, owner, pPolylineLine, true ) )
  {
    OdDgnImportPE::subImportElement( e, owner );
    return;
  }

  OdDbEntityPtr pRegion = pPolylineLine;

  if( (uShapeImportMode == 1) && !bUseFillColor && !bImportAsPolyline )
  {
    OdRxObjectPtrArray arrCurves;
    OdRxObjectPtrArray arrRegions;
    arrCurves.push_back( pPolylineLine.get() );

    if( (OdDbRegion::createFromCurves(arrCurves,arrRegions) == eOk) && arrRegions.size() )
    {
      pRegion = arrRegions[0];

      owner->appendOdDbEntity( pRegion );

      copyEntityProperties( pComplexShape , pRegion );

      pPolylineLine->erase(true);
    }
    else
    {
      pRegion = pPolylineLine;
    }
  }

  if( pRegion.isNull() )
  {
    OdDgnImportPE::subImportElement(pComplexShape, owner);
    return;
  }

  if( !pRegion.isNull() && !bImportAsPolyline )
  {
    importClosedElementFill( e, pRegion, owner );
  }
}

//----------------------------------------------------------------------------------------------

}
