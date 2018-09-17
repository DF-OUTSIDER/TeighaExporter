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
#include "DgnImport3dObject.h"
#include "DgnImportCommon.h"
#include "DgnImportContext.h"
#include "DgDatabase.h"
#include "DgLevelTableRecord.h"
#include "Dg3DObject.h"
#include "DgBSplineSurface.h"
#include "DbBody.h"

#include "Ge/GeSurfacePtrArray.h"
#include "BrepBuilderInitialData.h"
#include "BrepBuilderFillerModule.h"
#include "Br/BrBrep.h"
#include "DgModelerGeometryCreator.h"
#include "DgBrepBuilderFilerInitPE.h"

namespace TD_DGN_IMPORT {

//---------------------------------------------------------------------------------------------------

bool OdDgnSolidImportPE::allowImportElement(OdDgElement* e)
{
  bool bRet = true;

  OdDgSolid* pSolid = (OdDgSolid*)e;

  if( !OdDgnImportContext::isImportInvisible() )
  {
    bRet = !pSolid->getInvisibleFlag();

    if( bRet )
    {
      OdUInt32 uLevelEntryId = pSolid->getLevelEntryId();

      try
      {
        OdDg3dObjectHelper helper( pSolid );
        OdDg3dObjectHelper::EntireObject structure;
        helper.extractInformation( structure );

        OdDgGraphicsElementPtr pGraph;

        for(OdUInt32 k = 0; k < structure.size(); k++ )
        {
          for( OdUInt32 j = 0; j < structure[k].m_boundary.size(); j++ )
          {
            if( !structure[k].m_boundary[j].isNull() )
            {
              pGraph = structure[k].m_boundary[j];
              break;
            }
          }

          if( !pGraph.isNull() )
          {
            break;
          }
        }

        if( !pGraph.isNull() )
        {
          uLevelEntryId = pGraph->getLevelEntryId();
        }
      }
      catch(...)
      {
      }

      OdDgDatabase* pDb = e->database();

      if( pDb )
      {
        OdDgLevelTablePtr pLevelTable = pDb->getLevelTable(OdDg::kForRead);

        if( !pLevelTable.isNull() )
        {
          OdDgElementId idLevel = pLevelTable->getAt( uLevelEntryId );

          if( !idLevel.isNull() )
          {
            OdDgLevelTableRecordPtr pLevel = idLevel.openObject(OdDg::kForRead);

            if( !pLevel.isNull() )
            {
              bRet = pLevel->getIsDisplayedFlag() && !pLevel->getIsFrozenFlag();
            }
          }
        }
      }

      if( bRet && OdDgnImportContext::isLevelMask() )
      {
        bRet = OdDgnImportContext::checkLevelMask( uLevelEntryId );
      }
    }
  }

  return bRet;
}

//---------------------------------------------------------------------------------------------------

bool convertSolidTo3dSurface(OdDgElement* pElm, OdDbBlockTableRecord* pOwner, bool bSolid )
{
  bool bRet = true;

  try
  {
    OdDg3dSolidBrepBuilderInitialDataPEPtr pSurfaceBrep = OdDg3dSolidBrepBuilderInitialDataPEPtr(OdRxObjectPtr(pElm));

    if( pSurfaceBrep.isNull() )
      return false;

    if( odrxServiceDictionary()->getAt(OD_T("OdModelerGeometryCreator")).isNull() )
      odrxDynamicLinker()->loadModule(OdModelerGeometryModuleName);

    OdRxClassPtr pServiceDWG(odrxServiceDictionary()->getAt(OD_T("OdModelerGeometryCreator"))); //dwg mode geom

    if( pServiceDWG.isNull() )
    {
      ODA_ASSERT_ONCE(!"OdModelerGeometryCreator has not been created.");
      return false;
    }

    OdBrepBuilder brepBuilder; // AcisBrepBuilder
    OdModelerGeometryCreatorPtr pCreatorDWG = (OdModelerGeometryCreator*)pServiceDWG->create().get(); //!!!! dwg

    if( pCreatorDWG.isNull() )
    {
      ODA_ASSERT_ONCE(!"OdModelerGeometryCreator has not been created.");
      return false; 
    }

    if( pCreatorDWG->initBrepBuilder(brepBuilder, kOpenShell) != eOk )
    {
      ODA_ASSERT_ONCE(!"Brep builder has not been initialized.");
      return false; 
    }

    OdDg3dSolidBrepBuilderInitialDataPE::OdDg3dSolidBaseType solidType = pSurfaceBrep->getDgnSolidBaseType(pElm);

    if( solidType == OdDg3dSolidBrepBuilderInitialDataPE::kExtrudeBased )
    {
      OdBrepBuilderFillerModulePtr pBuilderFiller = ::odrxDynamicLinker()->loadModule(OdBrepBuilderFillerModuleName, false);

      if( pBuilderFiller.isNull() )
      {
        ODA_ASSERT_ONCE(!"OdBrepBuilderFillerModule has not been loaded.");
        return false; 
      }

      BrepBuilderInitialData brepData;

      double dScale = 1.0;

      if( !pSurfaceBrep->getBrepBuilderInitialData( pElm, brepData, dScale ) )
      {
        ODA_ASSERT_ONCE(!"Can't get solid brep builder initial data.");
        return false; 
      }

      OdGeCurve3dPtrArray arrEdges;
      OdGeCurve2dPtrArray arrCoedges;
      OdGeSurfacePtrArray arrSurfaces;

      if( pBuilderFiller->initFrom(brepBuilder, brepData, arrEdges, arrCoedges, arrSurfaces) != eOk)
      {
        ODA_ASSERT_ONCE(!"Error on innitialization of edges, coedges and surfaces.");
        return false; 
      }

      OdModelerGeometryPtr pGeometry = brepBuilder.finish();

      if( pGeometry.isNull() )
      {
        ODA_ASSERT_ONCE(!"Modeler geometry is null.");
        return false; 
      }

      if( bSolid )
      {
        OdDb3dSolidPtr pSolid = OdDb3dSolid::createObject();
        pSolid->setBody(pGeometry);
        pOwner->appendOdDbEntity(pSolid);
        OdDgGraphicsElement* pGrElm = (OdDgGraphicsElement*)(pElm);
        copyEntityProperties(pGrElm, pSolid);
        pSolid->transformBy( OdGeMatrix3d::scaling(dScale) );
      }
      else
      {
        OdDbBodyPtr pBody = OdDbBody::createObject();
        pBody->setBody(pGeometry);
        pOwner->appendOdDbEntity(pBody);
        OdDgGraphicsElement* pGrElm = (OdDgGraphicsElement*)(pElm);
        copyEntityProperties(pGrElm, pBody);
        pBody->transformBy( OdGeMatrix3d::scaling(dScale) );
      }
    }
    else
    {
      // Not implemented yet.
      bRet = false;
    }
  }
  catch(...)
  {
    bRet = false;
  }

  return bRet;
}

//---------------------------------------------------------------------------------------------------

void OdDgnSolidImportPE::subImportElement(OdDgElement* pElm, OdDbBlockTableRecord* pOwner)
{
  bool bImportThroughDraw = true;

  if( OdDgnImportContext::getObject3dImportMode() == 1 )
    bImportThroughDraw = !convertSolidTo3dSurface(pElm, pOwner, true);

  if( bImportThroughDraw )
    OdDgnImportPE::subImportElement(pElm, pOwner);
}

//---------------------------------------------------------------------------------------------------

void OdDgnSurfaceImportPE::subImportElement(OdDgElement* pElm, OdDbBlockTableRecord* pOwner)
{
  bool bImportThroughDraw = true;

  if( OdDgnImportContext::getObject3dImportMode() == 1 )
    bImportThroughDraw = !convertSolidTo3dSurface(pElm, pOwner, false);

  if( bImportThroughDraw )
    OdDgnImportPE::subImportElement(pElm, pOwner);
}

//---------------------------------------------------------------------------------------------------

bool OdDgnSurfaceImportPE::allowImportElement(OdDgElement* e)
{
  bool bRet = true;

  OdDgSurface* pSurface = (OdDgSurface*)e;

  if( !OdDgnImportContext::isImportInvisible() )
  {
    bRet = !pSurface->getInvisibleFlag();

    if( bRet )
    {
      OdUInt32 uLevelEntryId = pSurface->getLevelEntryId();

      try
      {
        OdDg3dObjectHelper helper( pSurface );
        OdDg3dObjectHelper::EntireObject structure;
        helper.extractInformation( structure );

        OdDgGraphicsElementPtr pGraph;

        for(OdUInt32 k = 0; k < structure.size(); k++ )
        {
          for( OdUInt32 j = 0; j < structure[k].m_boundary.size(); j++ )
          {
            if( !structure[k].m_boundary[j].isNull() )
            {
              pGraph = structure[k].m_boundary[j];
              break;
            }
          }

          if( !pGraph.isNull() )
          {
            break;
          }
        }

        if( !pGraph.isNull() )
        {
          uLevelEntryId = pGraph->getLevelEntryId();
        }
      }
      catch(...)
      {
      }

      OdDgDatabase* pDb = e->database();

      if( pDb )
      {
        OdDgLevelTablePtr pLevelTable = pDb->getLevelTable(OdDg::kForRead);

        if( !pLevelTable.isNull() )
        {
          OdDgElementId idLevel = pLevelTable->getAt( uLevelEntryId );

          if( !idLevel.isNull() )
          {
            OdDgLevelTableRecordPtr pLevel = idLevel.openObject(OdDg::kForRead);

            if( !pLevel.isNull() )
            {
              bRet = pLevel->getIsDisplayedFlag() && !pLevel->getIsFrozenFlag();
            }
          }
        }
      }

      if( bRet && OdDgnImportContext::isLevelMask() )
      {
        bRet = OdDgnImportContext::checkLevelMask( uLevelEntryId );
      }
    }
  }

  return bRet;
}

//---------------------------------------------------------------------------------------------------

bool OdDgnBSplineSurfaceImportPE::convertBSplineSurfaceTo3dSurface(OdDgElement* pElm, OdDbBlockTableRecord* pOwner)
{
  bool bRet = true;

  try
  {
    OdDgBSplineSurface* pSurface = (OdDgBSplineSurface*)(pElm);

    OdDgBSplineSurfaceBrepBuilderInitialDataPEPtr pSurfaceBrep = OdDgBSplineSurfaceBrepBuilderInitialDataPEPtr(OdRxObjectPtr(pElm));

    if( pSurfaceBrep.isNull() )
      return false;

    if( odrxServiceDictionary()->getAt(OD_T("OdModelerGeometryCreator")).isNull() )
      odrxDynamicLinker()->loadModule(OdModelerGeometryModuleName);

    OdRxClassPtr pServiceDWG(odrxServiceDictionary()->getAt(OD_T("OdModelerGeometryCreator"))); //dwg mode geom

    if( pServiceDWG.isNull() )
    {
      ODA_ASSERT_ONCE(!"OdModelerGeometryCreator has not been created.");
      return false;
    }

    OdBrepBuilder brepBuilder; // AcisBrepBuilder
    OdModelerGeometryCreatorPtr pCreatorDWG = (OdModelerGeometryCreator*)pServiceDWG->create().get(); //!!!! dwg

    if( pCreatorDWG.isNull() )
    {
      ODA_ASSERT_ONCE(!"OdModelerGeometryCreator has not been created.");
      return false; 
    }

    if( pCreatorDWG->initBrepBuilder(brepBuilder, kOpenShell) != eOk )
    {
      ODA_ASSERT_ONCE(!"Brep builder has not been initialized.");
      return false; 
    }

    OdBrepBuilderFillerModulePtr pBuilderFiller = ::odrxDynamicLinker()->loadModule(OdBrepBuilderFillerModuleName, false);

    if( pBuilderFiller.isNull() )
    {
      ODA_ASSERT_ONCE(!"OdBrepBuilderFillerModule has not been loaded.");
      return false; 
    }

    BrepBuilderInitialData brepData;

    double dScale = 1.0;

    if( !pSurfaceBrep->getBrepBuilderInitialData( pSurface, brepData, dScale ) )
    {
      ODA_ASSERT_ONCE(!"Can't get b-spline surface brep builder initial data.");
      return false; 
    }

    OdGeCurve3dPtrArray arrEdges;
    OdGeCurve2dPtrArray arrCoedges;
    OdGeSurfacePtrArray arrSurfaces;

    if( pBuilderFiller->initFrom(brepBuilder, brepData, arrEdges, arrCoedges, arrSurfaces) != eOk)
    {
      ODA_ASSERT_ONCE(!"Error on innitialization of edges, coedges and surfaces.");
      return false; 
    }

    OdModelerGeometryPtr pGeometry = brepBuilder.finish();

    if( pGeometry.isNull() )
    {
      ODA_ASSERT_ONCE(!"Modeler geometry is null.");
      return false; 
    }

    OdDbSurfacePtr pDbSurface = OdDbSurface::createObject();
    pDbSurface->setBody(pGeometry);
    pOwner->appendOdDbEntity(pDbSurface);
    OdUInt32 nRulesU, nRulesV;
    pSurface->getNumberOfSpansInU(nRulesU);
    pSurface->getNumberOfSpansInV(nRulesV);
    pDbSurface->setUIsolineDensity( nRulesU );
    pDbSurface->setVIsolineDensity( nRulesV );
    OdDgGraphicsElement* pGrElm = (OdDgGraphicsElement*)(pElm);
    copyEntityProperties(pGrElm, pDbSurface);
    pDbSurface->transformBy( OdGeMatrix3d::scaling(dScale) );
  }
  catch(...)
  {
    bRet = false;
  }

  return bRet;
}

//---------------------------------------------------------------------------------------------------

void OdDgnBSplineSurfaceImportPE::subImportElement(OdDgElement* pElm, OdDbBlockTableRecord* pOwner)
{
  bool bImportThroughDraw = true;

  if( OdDgnImportContext::getObject3dImportMode() == 1 )
    bImportThroughDraw = !convertBSplineSurfaceTo3dSurface(pElm, pOwner);

  if( bImportThroughDraw )
    OdDgnImportPE::subImportElement(pElm, pOwner);
}

//---------------------------------------------------------------------------------------------------

bool OdDgnConeImportPE::convertConeTo3dSolid(OdDgElement* pElm, OdDbBlockTableRecord* pOwner)
{
  bool bRet = true;

  try
  {
    OdDgCone* pCone = (OdDgCone*)(pElm);

    OdDgConeBrepBuilderInitialDataPEPtr pSurfaceBrep = OdDgConeBrepBuilderInitialDataPEPtr(OdRxObjectPtr(pElm));

    if( pSurfaceBrep.isNull() )
      return false;

    if( odrxServiceDictionary()->getAt(OD_T("OdModelerGeometryCreator")).isNull() )
      odrxDynamicLinker()->loadModule(OdModelerGeometryModuleName);

    OdRxClassPtr pServiceDWG(odrxServiceDictionary()->getAt(OD_T("OdModelerGeometryCreator"))); //dwg mode geom

    if( pServiceDWG.isNull() )
    {
      ODA_ASSERT_ONCE(!"OdModelerGeometryCreator has not been created.");
      return false;
    }

    OdBrepBuilder brepBuilder; // AcisBrepBuilder
    OdModelerGeometryCreatorPtr pCreatorDWG = (OdModelerGeometryCreator*)pServiceDWG->create().get(); //!!!! dwg

    if( pCreatorDWG.isNull() )
    {
      ODA_ASSERT_ONCE(!"OdModelerGeometryCreator has not been created.");
      return false; 
    }

    if( pCreatorDWG->initBrepBuilder(brepBuilder, kOpenShell) != eOk )
    {
      ODA_ASSERT_ONCE(!"Brep builder has not been initialized.");
      return false; 
    }

    OdBrepBuilderFillerModulePtr pBuilderFiller = ::odrxDynamicLinker()->loadModule(OdBrepBuilderFillerModuleName, false);

    if( pBuilderFiller.isNull() )
    {
      ODA_ASSERT_ONCE(!"OdBrepBuilderFillerModule has not been loaded.");
      return false; 
    }

    BrepBuilderInitialData brepData;

    double dScale = 1.0;

    if( !pSurfaceBrep->getBrepBuilderInitialData( pCone, brepData, dScale ) )
    {
      ODA_ASSERT_ONCE(!"Can't get cone brep builder initial data.");
      return false; 
    }

    OdGeCurve3dPtrArray arrEdges;
    OdGeCurve2dPtrArray arrCoedges;
    OdGeSurfacePtrArray arrSurfaces;

    if( pBuilderFiller->initFrom(brepBuilder, brepData, arrEdges, arrCoedges, arrSurfaces) != eOk)
    {
      ODA_ASSERT_ONCE(!"Error on innitialization of edges, coedges and surfaces.");
      return false; 
    }

    OdModelerGeometryPtr pGeometry = brepBuilder.finish();

    if( pGeometry.isNull() )
    {
      ODA_ASSERT_ONCE(!"Modeler geometry is null.");
      return false; 
    }

    if( pCone->getHoleFlag() )
    {
      OdDbBodyPtr pBody = OdDbBody::createObject();
      pBody->setBody(pGeometry);
      pOwner->appendOdDbEntity(pBody);
      OdDgGraphicsElement* pGrElm = (OdDgGraphicsElement*)(pElm);
      copyEntityProperties(pGrElm, pBody);
      pBody->transformBy( OdGeMatrix3d::scaling(dScale) );
    }
    else
    {
      OdDb3dSolidPtr pSolid = OdDb3dSolid::createObject();
      pSolid->setBody(pGeometry);
      pOwner->appendOdDbEntity(pSolid);
      OdDgGraphicsElement* pGrElm = (OdDgGraphicsElement*)(pElm);
      copyEntityProperties(pGrElm, pSolid);
      pSolid->transformBy( OdGeMatrix3d::scaling(dScale) );
    }
  }
  catch(...)
  {
    bRet = false;
  }

  return bRet;
}

//---------------------------------------------------------------------------------------------------

void OdDgnConeImportPE::subImportElement(OdDgElement* pElm, OdDbBlockTableRecord* pOwner)
{
  bool bImportThroughDraw = true;

  if( OdDgnImportContext::getObject3dImportMode() == 1 )
    bImportThroughDraw = !convertConeTo3dSolid(pElm, pOwner);

  if( bImportThroughDraw )
    OdDgnImportPE::subImportElement(pElm, pOwner);
}

//---------------------------------------------------------------------------------------------------

}
