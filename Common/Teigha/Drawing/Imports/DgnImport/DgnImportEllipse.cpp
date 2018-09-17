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
#include "RxObject.h"
#include "DgnImportContext.h"
#include "DgnImportImpl.h"
#include "DgnImportEllipse.h"
#include "DgnImportCommon.h"
#include "DgnImportPatterns.h"
#include <DbEllipse.h>
#include <DgEllipse.h>
#include <DbRegion.h>

namespace TD_DGN_IMPORT {

//---------------------------------------------------------------------------------------------------

void OdDgnEllipse2dImportPE::subImportElement(OdDgElement* e, OdDbBlockTableRecord* owner)
{
  OdUInt8 uEllipse2dImportMode = OdDgnImportContext::getEllipse2dImportMode();

  bool bImportAsPolyline = OdDgnImportContext::getImportCurvesOnlyFlag();

  OdDbEntityPtr pEntToCreateFill;

  OdDgEllipse2d* ell = (OdDgEllipse2d*)e;
  OdGePoint2d origin = ell->getOrigin();
  double xlen = ell->getPrimaryAxis();
  double ylen = ell->getSecondaryAxis();
  if (OdZero(xlen, 1.E-6) || OdZero(ylen, 1.E-6) || (xlen/ylen) > 1.E6 || (xlen/ylen) < 1.E-6)
    return;
  OdDbEllipsePtr dbEll = OdDbEllipse::createObject();
  dbEll->setDatabaseDefaults(owner->database());
  owner->database()->addOdDbObject(dbEll, owner->objectId());
  copyEntityProperties(ell, dbEll);
  owner->appendOdDbEntity(dbEll);
  if (xlen >= ylen)
   {
    OdGeVector3d axis = OdGeVector3d::kXAxis * xlen;
    axis.rotateBy(ell->getRotationAngle(), OdGeVector3d::kZAxis);
    dbEll->set(OdGePoint3d(origin.x, origin.y, 0.0), OdGeVector3d::kZAxis, axis, ylen / xlen);
  }
  else
  {
    OdGeVector3d axis = OdGeVector3d::kYAxis * ylen;
    axis.rotateBy(ell->getRotationAngle(), OdGeVector3d::kZAxis);
    dbEll->set(OdGePoint3d(origin.x, origin.y, 0.0), OdGeVector3d::kZAxis, axis, xlen / ylen);
  }

  pEntToCreateFill = dbEll;

  if( uEllipse2dImportMode == 1  && !bImportAsPolyline ) // Create region
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
      arrCurves.push_back( (OdRxObjectPtr)(dbEll.get()) );

      if( (OdDbRegion::createFromCurves(arrCurves,arrRegions) == eOk) && arrRegions.size() )
      {
        OdDbRegionPtr pRegion = arrRegions[0];

        owner->appendOdDbEntity( pRegion );
        dbEll->erase(true);

        copyEntityProperties( ell , pRegion );

        pEntToCreateFill = pRegion;
      }
    }
  }

  if( !bImportAsPolyline )
  {
    importClosedElementFill( e, dbEll, owner );
  }

  OdDgnImportPathToDwgObject dwgPath;
  dwgPath.m_idPath.objectIds().push_back( dbEll->objectId() );
  dwgPath.m_bExists = true;
  OdDgnImportContext::addObjectPath( e->elementId(), dwgPath );
}

//---------------------------------------------------------------------------------------------------

void OdDgnEllipse3dImportPE::subImportElement(OdDgElement* e, OdDbBlockTableRecord* owner)
{
  OdUInt8 uEllipse3dImportMode = OdDgnImportContext::getEllipse3dImportMode();

  bool bImportAsPolyline = OdDgnImportContext::getImportCurvesOnlyFlag();

  OdDbEntityPtr pEntToCreateFill;

  OdDgEllipse3d* ell = (OdDgEllipse3d*)e;
  double xlen = ell->getPrimaryAxis();
  double ylen = ell->getSecondaryAxis();
  if (OdZero(xlen, 1.E-6) || OdZero(ylen, 1.E-6) || (xlen/ylen) > 1.E6 || (xlen/ylen) < 1.E-6)
    return;
  OdDbEllipsePtr dbEll = OdDbEllipse::createObject();
  dbEll->setDatabaseDefaults(owner->database());
  owner->database()->addOdDbObject(dbEll, owner->objectId());
  copyEntityProperties(ell, dbEll);
  owner->appendOdDbEntity(dbEll);
  if (xlen >= ylen)
  {
    OdGeVector3d axis = OdGeVector3d::kXAxis * xlen;
    ell->getRotation().rotate(axis);
    OdGeVector3d normal = OdGeVector3d::kZAxis;
    ell->getRotation().rotate(normal);
    dbEll->set(ell->origin(), normal, axis, ylen / xlen);
  }
  else
  {
    OdGeVector3d axis = OdGeVector3d::kYAxis * ylen;
    ell->getRotation().rotate(axis);
    OdGeVector3d normal = OdGeVector3d::kZAxis;
    ell->getRotation().rotate(normal);
    dbEll->set(ell->origin(), normal, axis, xlen / ylen);
  }

  pEntToCreateFill = dbEll;

  if( (uEllipse3dImportMode == 1) && !bImportAsPolyline ) // Create region
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
      arrCurves.push_back( (OdRxObjectPtr)(dbEll.get()) );

      if( (OdDbRegion::createFromCurves(arrCurves,arrRegions) == eOk) && arrRegions.size() )
      {
        OdDbRegionPtr pRegion = arrRegions[0];

        owner->appendOdDbEntity( pRegion );
        dbEll->erase(true);

        copyEntityProperties( ell , pRegion );

        pEntToCreateFill = pRegion;
      }
    }
  }
  else
  {
    OdDgnImportPathToDwgObject dwgPath;
    dwgPath.m_idPath.objectIds().push_back( dbEll->objectId() );
    dwgPath.m_bExists = true;
    OdDgnImportContext::addObjectPath( e->elementId(), dwgPath );
  }

  if( !bImportAsPolyline )
  {
    importClosedElementFill( e, pEntToCreateFill, owner );
  }
}

//---------------------------------------------------------------------------------------------------

}
