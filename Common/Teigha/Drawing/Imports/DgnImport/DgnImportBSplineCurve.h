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

#ifndef _DGN_IMPORT_BSPLINE_CURVE_INCLUDED_ 
#define _DGN_IMPORT_BSPLINE_CURVE_INCLUDED_

#include "DgnImportPE.h"
#include "DgnImportPatterns.h"
#include <DgBSplineCurve.h>
#include <DbSpline.h>
#include <Ge/GeKnotVector.h>
#include <DbRegion.h>
#include "DgCurve.h"

namespace TD_DGN_IMPORT 
{

//------------------------------------------------------------------------------

template <class T> struct OdDgBSplineCurveImportPE : OdDgnImportPE
{
  void subImportElement(OdDgElement* e, OdDbBlockTableRecord* owner) ODRX_OVERRIDE
  {
    T* bs = static_cast<T*>(e);
    OdDbSplinePtr s = OdDbSpline::createObject();

    OdUInt8 uCurveImportMode = OdDgnImportContext::getClosedBSplineCurve2dImportMode();

    bool bImportAsPolyline = OdDgnImportContext::getImportCurvesOnlyFlag();

    if( bs->get3dFormatFlag() )
    {
      uCurveImportMode = OdDgnImportContext::getClosedBSplineCurve3dImportMode();
    }

    try
    {
      OdDbEntityPtr pEntToCreateFill;

      OdGeNurbCurve3d nurbs;
      bs->generateGeCurve(nurbs);
      s->setDatabaseDefaults(owner->database());
      owner->database()->addOdDbObject(s, owner->objectId());
      owner->appendOdDbEntity(s);
      OdGePoint3dArray fitPoints;
      OdGeTol fitTolerance;
      bool tangentsExist;
      OdGeVector3d startTangent, endTangent;
      if (nurbs.getFitData(fitPoints, fitTolerance, tangentsExist, startTangent, endTangent))
      {
        s->setFitData(fitPoints, nurbs.degree(), fitTolerance.equalPoint(), startTangent, endTangent);
      }
      else
      {
        int degree;
        bool rational, periodic;
        OdGeKnotVector knots;
        OdGePoint3dArray controlPoints;
        OdGeDoubleArray weights;
        nurbs.getDefinitionData(degree, rational, periodic, knots, controlPoints, weights);
        s->setNurbsData(degree, rational, nurbs.isClosed(), periodic, controlPoints, knots, weights, 0);
      }

      copyEntityProperties(bs, s);

      pEntToCreateFill = s;

      if( (uCurveImportMode == 1) && bs->getClosedFlag() && !bImportAsPolyline )
      {
        bool bUseFillColor = false;

        OdRxObjectPtrArray arrFillLinkages;

        e->getLinkages( OdDgAttributeLinkage::kFillStyle, arrFillLinkages );

        for( OdUInt32 i = 0; i < arrFillLinkages.size(); i++ )
        {
          if( arrFillLinkages[i]->isKindOf(OdDgFillColorLinkage::desc() ) )
          {
            bUseFillColor = true;
            break;
          }

          if( arrFillLinkages[i]->isKindOf( OdDgGradientFillLinkage::desc() ) )
          {
            bUseFillColor = true;
            break;
          }
        }

        if( !bUseFillColor )
        {
          OdRxObjectPtrArray arrRegions;
          OdRxObjectPtrArray arrCurves;
          arrCurves.push_back( (OdRxObjectPtr)(s.get()) );

          if( (OdDbRegion::createFromCurves(arrCurves,arrRegions) == eOk) && arrRegions.size() )
          {
            OdDbRegionPtr pRegion = arrRegions[0];

            owner->appendOdDbEntity( pRegion );
            s->erase(true);

            copyEntityProperties( bs , pRegion );

            pEntToCreateFill = pRegion;
          }
        }
      }
      else
      {
        OdDgnImportPathToDwgObject dwgPath;
        dwgPath.m_idPath.objectIds().push_back( s->objectId() );
        dwgPath.m_bExists = true;
        OdDgnImportContext::addObjectPath( e->elementId(), dwgPath );
      }

      if( !bImportAsPolyline )
      {
        importClosedElementFill( e, pEntToCreateFill, owner );
      }
    }
    catch(const OdError&)
    {
    }
  }
};

//------------------------------------------------------------------------------

template <class T> void generateGeCurve(T*, OdGeNurbCurve3d& nurbs ){};

//---------------------------------------------------------------------------------------------------

template <> void generateGeCurve<OdDgCurve2d>(OdDgCurve2d* pCurve2d, OdGeNurbCurve3d& nurbs )
{
  if( !pCurve2d->getGeNurbCurve( nurbs) )
  {
    throw( OdError(eDegenerateGeometry) );
  }
}

//---------------------------------------------------------------------------------------------------

template <> void generateGeCurve<OdDgCurve3d>(OdDgCurve3d* pCurve3d, OdGeNurbCurve3d& nurbs )
{
  if( !pCurve3d->getGeNurbCurve( nurbs) )
  {
    throw( OdError(eDegenerateGeometry) );
  }
}

//---------------------------------------------------------------------------------------------------

template <class T> struct OdDgCurveImportPE : OdDgnImportPE
{
  void subImportElement(OdDgElement* e, OdDbBlockTableRecord* owner) ODRX_OVERRIDE
  {
    T* pCurve = static_cast<T*>(e);

    OdDbSplinePtr s = OdDbSpline::createObject();

    try
    {
      OdDbEntityPtr pEntToCreateFill;

      OdGeNurbCurve3d nurbs;
      generateGeCurve(pCurve, nurbs);
      s->setDatabaseDefaults(owner->database());
      owner->database()->addOdDbObject(s, owner->objectId());
      owner->appendOdDbEntity(s);
      s->setFromOdGeCurve( nurbs );
      copyEntityProperties(pCurve,s);

      OdDgnImportPathToDwgObject dwgPath;
      dwgPath.m_idPath.objectIds().push_back( s->objectId() );
      dwgPath.m_bExists = true;
      OdDgnImportContext::addObjectPath( e->elementId(), dwgPath );
    }
    catch(const OdError&)
    {    
      OdDgnImportPE::subImportElement( e, owner );
    }
  }
};

//------------------------------------------------------------------------------

}
#endif // _DGN_IMPORT_BSPLINE_CURVE_INCLUDED_
