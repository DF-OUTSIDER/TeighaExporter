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

#ifndef _DGN_IMPORT_CELLHEADER_INCLUDED_ 
#define _DGN_IMPORT_CELLHEADER_INCLUDED_

#include "DgnImportPE.h"
#include "DgnImportCommon.h"
#include "DgnImportContext.h"
#include <OdaCommon.h>
#include "DbBlockReference.h"
#include "DgCellHeader.h"
#include <DgBRepEntityPE.h>
#include <DbBlockTable.h>
#include "DbSymUtl.h"
#include <Ge/GeScale3d.h>
#include "DgLevelTableRecord.h"
#include "DgDetailingSymbol.h"
#include <DbSpatialFilter.h>
#include <DbBody.h>

#include "Ge/GeSurfacePtrArray.h"
#include "BrepBuilderInitialData.h"
#include "BrepBuilderFillerModule.h"
#include "Br/BrBrep.h"
#include "DgModelerGeometryCreator.h"

namespace TD_DGN_IMPORT 
{

//---------------------------------------------------------------------------------------------------

template <class T> OdGePoint3d getCellHeaderOrigin(T*){return OdGePoint3d::kOrigin;}

//---------------------------------------------------------------------------------------------------

template <> OdGePoint3d getCellHeaderOrigin<OdDgCellHeader2d>(OdDgCellHeader2d* cell2d)
{
  OdGePoint2d ptPos = cell2d->getOrigin();

  return OdGePoint3d( ptPos.x, ptPos.y, 0 );
}

//---------------------------------------------------------------------------------------------------

template <> OdGePoint3d getCellHeaderOrigin<OdDgCellHeader3d>(OdDgCellHeader3d* cell3d)
{
  return cell3d->getOrigin();
}

//---------------------------------------------------------------------------------------------------

template <class T> OdGeMatrix3d getCellHeaderRotation(T*){return OdGeMatrix3d::kIdentity;}

//---------------------------------------------------------------------------------------------------

template <> OdGeMatrix3d getCellHeaderRotation<OdDgCellHeader2d>(OdDgCellHeader2d* cell2d)
{
  OdGeMatrix2d matRotation2d = cell2d->getTransformation();

  OdGeMatrix3d matRotation;
  matRotation[0][0] = matRotation2d[0][0];
  matRotation[0][1] = matRotation2d[0][1];
  matRotation[0][3] = matRotation2d[0][2];
  matRotation[1][0] = matRotation2d[1][0];
  matRotation[1][1] = matRotation2d[1][1];
  matRotation[1][3] = matRotation2d[1][2];
  matRotation[3][0] = matRotation2d[2][0];
  matRotation[3][1] = matRotation2d[2][1];
  matRotation[3][3] = matRotation2d[2][2];
  
  return matRotation;
}

//---------------------------------------------------------------------------------------------------

template <> OdGeMatrix3d getCellHeaderRotation<OdDgCellHeader3d>(OdDgCellHeader3d* cell3d)
{
  return cell3d->getTransformation();
}

//---------------------------------------------------------------------------------------------------

template <class T> bool isImportThroughDraw(T*){return false;}

//---------------------------------------------------------------------------------------------------

template<> bool isImportThroughDraw<OdDgCellHeader2d>( OdDgCellHeader2d* cell2d )
{
  bool bRet = cell2d->isKindOf( OdDgDetailingSymbolCellHeader2d::desc() );

  if( !bRet )
  {   
    bRet = cell2d->isPolygonWithHoles();
  }

  return bRet;
}

//---------------------------------------------------------------------------------------------------

template<> bool isImportThroughDraw<OdDgCellHeader3d>( OdDgCellHeader3d* cell3d )
{
  bool bRet = cell3d->isKindOf( OdDgDetailingSymbolCellHeader3d::desc() );
 
  if( !bRet )
  {   
    bRet = cell3d->isPolygonWithHoles();;
  }

  OdDgBRepEntityPE::BRepEntityType brepType = OdDgBRepEntityPEPtr(cell3d)->type(cell3d);

  if( brepType != OdDgBRepEntityPE::eNotBRepEntity )
  {
    bRet = true;
  }

  return bRet;
}

//---------------------------------------------------------------------------------------------------

template <class T> struct OdDgnCellHeaderImportPE : OdDgnImportPE
{
//---------------------------------------------------------------------------------------------------

  bool allowImportElement(OdDgElement* e)
  {
    bool bRet = true;

    if( e->isKindOf(OdDgCellHeader3d::desc()) )
    {
      OdDgCellHeader3dPtr pCell3d = (OdDgCellHeader3d*)(e);

      OdDgBRepEntityPE::BRepEntityType brepType = OdDgBRepEntityPEPtr(pCell3d)->type(pCell3d);

      if( (brepType != OdDgBRepEntityPE::eNotBRepEntity) || pCell3d->isPolygonWithHoles() )
      {
        if( !OdDgnImportContext::isImportInvisible() )
        {
          bRet = !pCell3d->getInvisibleFlag();

          if( bRet )
          {
            OdUInt32 uLevelEntryId = pCell3d->getLevelEntryId();

            OdDgGraphicsElementPtr pInternalGraphics;
            OdDgElementIteratorPtr localPtr = pCell3d->createIterator();
            getInternalGraphicsElement( localPtr, pInternalGraphics );

            if( !pInternalGraphics.isNull() )
            {
              uLevelEntryId = pInternalGraphics->getLevelEntryId();
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
      }
    }

    return bRet;
  }

//---------------------------------------------------------------------------------------------------

  void getHatchesAndFills( OdDgElement* pElm,
                             OdUInt32& uFillColor,
                               OdRxObjectPtrArray& arrGradientLinkages,
                                  OdRxObjectPtrArray& arrPatternLinkages,
                                    bool& bUseFillColor,
                                      bool bCheckFillColor,
                                        bool bCheckGradients,
                                          bool bCheckPatterns
                         )
  {
    OdRxObjectPtrArray      arrFillLinkages;

    OdUInt32 i = 0;

    if( bCheckFillColor )
    {
      bUseFillColor = false;
    }

    if( bCheckFillColor || bCheckGradients )
    {
      pElm->getLinkages( OdDgAttributeLinkage::kFillStyle, arrFillLinkages );

      for( i = 0; i < arrFillLinkages.size(); i++ )
      {
        if( arrFillLinkages[i]->isKindOf(OdDgFillColorLinkage::desc() ) && !bUseFillColor && bCheckFillColor )
        {
          OdDgFillColorLinkagePtr pFillLinkage = arrFillLinkages[i];

          if( !pFillLinkage.isNull() )
          {
            bUseFillColor = true;
            uFillColor = pFillLinkage->getColorIndex();
          }
        }

        if( arrFillLinkages[i]->isKindOf(OdDgGradientFillLinkage::desc() ) && bCheckGradients )
        {
          arrGradientLinkages.push_back( arrFillLinkages[i] );
        }
      }
    }

    arrFillLinkages.clear();

    if( bCheckPatterns )
    {
      pElm->getLinkages( OdDgAttributeLinkage::kHatch, arrFillLinkages );

      for( i = 0; i < arrFillLinkages.size(); i++ )
      {
        if( arrFillLinkages[i]->isKindOf( OdDgLinearPatternLinkage::desc() ) ||
          arrFillLinkages[i]->isKindOf( OdDgCrossPatternLinkage::desc() ) ||
          arrFillLinkages[i]->isKindOf( OdDgSymbolPatternLinkage::desc() ) ||
          arrFillLinkages[i]->isKindOf( OdDgDWGPatternLinkage::desc() )
          )
        {
          arrPatternLinkages.push_back( arrFillLinkages[i] );
        }
      }
    }
  }

//---------------------------------------------------------------------------------------------------

  void createHatchBoundaries( OdDgElement* e, OdDbBlockTableRecord* owner, OdDbObjectIdArray& arrIds )
  {
    T* cell = static_cast<T*>(e);

    OdDgElementIteratorPtr pIter = cell->createIterator();

    OdUInt32 nObjectsBefore = 0;

    OdDbObjectIteratorPtr pDbIter = owner->newIterator();

    while( !pDbIter->done() )
    {
      pDbIter->step();
      nObjectsBefore++;
    }

    OdDgnImportContext::setImportCurvesOnlyFlag( true );

    for(; !pIter->done(); pIter->step() )
    {
      OdDgElementPtr pElm = pIter->item().openObject(OdDg::kForRead);
      ((OdDgnImportPE*)pElm->queryX(OdDgnImportPE::desc()))->subImportElement(pElm, owner);
    }

    OdDgnImportContext::setImportCurvesOnlyFlag( false );

    OdUInt32 uCount = 0;

    pDbIter = owner->newIterator();

    for(; !pDbIter->done(); pDbIter->step() )
    {
      uCount++;

      if( uCount <= nObjectsBefore )
      {
        continue;
      }

      OdDbObjectPtr pObj = pDbIter->objectId().openObject(OdDb::kForWrite);

      if( !pObj.isNull() )
      {
        if( !pObj->isKindOf(OdDbHatch::desc()) && pObj->isKindOf(OdDbCurve::desc()))
        {
          arrIds.push_back( pObj->id() );
        }
        else
        {
          pObj->erase(true);
        }
      }
    }
  }

//---------------------------------------------------------------------------------------------------

  void importPolygonWithHoles( OdDgElement* e, OdDbBlockTableRecord* owner, bool bImportHatchOnly )
  {
    T* cell = static_cast<T*>(e);

    bool bUseFillPattern  = false;
    bool bUseGradientFill = false;
    bool bUseHatchPattern = false;

    OdUInt32 uFillColor                     = 0;
    OdRxObjectPtrArray      arrGradientLinkages;
    OdRxObjectPtrArray      arrPatternLinkages;

    getHatchesAndFills( e, uFillColor, arrGradientLinkages, arrPatternLinkages, bUseFillPattern, true, true, true );

    bUseGradientFill = arrGradientLinkages.size() > 0;
    bUseHatchPattern = arrPatternLinkages.size() > 0;

    OdDgGraphicsElement* pBaseElm = cell;

    OdDgElementId idOwher = cell->ownerId();

    if( !idOwher.isNull() )
    {
      OdDgElementPtr pOwner = idOwher.openObject(OdDg::kForRead);

      bool bUseHatchPatternOld = bUseHatchPattern;

      if( pOwner->getElementType() == OdDgElement::kTypeCellHeader )
      {
        getHatchesAndFills( pOwner, uFillColor, arrGradientLinkages, arrPatternLinkages, bUseFillPattern, !bUseFillPattern, !bUseGradientFill, !bUseHatchPattern );

        bUseGradientFill = arrGradientLinkages.size() > 0;
        bUseHatchPattern = arrPatternLinkages.size() > 0;
      }

      if( !bUseHatchPatternOld && bUseHatchPattern )
      {
        pBaseElm = (OdDgGraphicsElement*)(pOwner.get());
      }
    }

    OdDgElementIteratorPtr pIter = cell->createIterator();

    for(; !pIter->done(); pIter->step() )
    {
      OdDgElementPtr pElm = pIter->item().openObject(OdDg::kForRead);

      if( pElm->isKindOf(OdDgGraphicsElement::desc()) )
      {
        OdDgGraphicsElementPtr pGraph = pElm;

        if( !pGraph->getHbitFlag() && !pGraph->getInvisibleFlag() )
        {
          getHatchesAndFills( pGraph, uFillColor, arrGradientLinkages, arrPatternLinkages, bUseFillPattern, !bUseFillPattern, !bUseGradientFill, !bUseHatchPattern );

          bUseGradientFill = arrGradientLinkages.size() > 0;
          bUseHatchPattern = arrPatternLinkages.size() > 0;
        }
      }
    }

    OdDbObjectIdArray arrIds;

    if( bUseFillPattern || bUseGradientFill || bUseHatchPattern )
    {
      createHatchBoundaries( e, owner, arrIds );
    }

    if( (bUseFillPattern || bUseGradientFill) && (arrIds.size() > 0) )
    {
      OdDbHatchPtr pHatch = createHatchEntity(arrIds, true);
      owner->appendOdDbEntity( pHatch );

      if( bUseGradientFill )
      {
        OdDgGradientFillLinkagePtr pGradLinkage = (OdDgGradientFillLinkagePtr)(arrGradientLinkages[0].get());

        importGradient( pHatch, pGradLinkage );

        for( OdUInt32 k = 0; k < arrIds.size(); k++ )
        {
          OdDbEntityPtr pEnt = arrIds[k].openObject(OdDb::kForWrite);
          pEnt->setVisibility(OdDb::kInvisible);
        }
      }
      else
      {
        pHatch->setPattern( OdDbHatch::kPreDefined, L"SOLID" );

        OdDbObjectId idTmp = pHatch->id();

        pHatch->swapIdWith( arrIds[0] );

        arrIds[0] = idTmp;
      }

      copyEntityProperties( cell, pHatch );

      if( bUseFillPattern )
      {
        setDwgColorByDgnIndex( cell->database(), pHatch, uFillColor );
      }
    }
    else if( !bImportHatchOnly )
    {
      OdDgnImportContext::setSkipPatternDrawFlag( true );

      OdDgnImportPE::subImportElement( e, owner );

      OdDgnImportContext::setSkipPatternDrawFlag( false );
    }

    if( bUseHatchPattern )
    {
      if( !bUseGradientFill && !bUseFillPattern )
      {
        for( OdUInt32 k = 0; k < arrIds.size(); k++ )
        {
          OdDbEntityPtr pEnt = arrIds[k].openObject(OdDb::kForWrite);
          pEnt->setVisibility(OdDb::kInvisible);
        }
      }

      OdDgPatternLinkagePtr pPatternLinkage = arrPatternLinkages[0];

      OdDbHatchPtr pHatch = createHatchEntity(arrIds, true);
      owner->appendOdDbEntity(pHatch);

      copyEntityProperties( pBaseElm, pHatch );

      double dUORsToModelScale = 1.0;

      OdDgElementId idDgOwner = cell->ownerId();

      while( !idDgOwner.isNull() )
      {
        OdDgElementPtr pDgElm = idDgOwner.openObject(OdDg::kForRead);

        if( pDgElm->isKindOf(OdDgModel::desc()) )
        {
          OdDgModelPtr pModel = pDgElm;

          if( !pModel.isNull() )
          {
            dUORsToModelScale = pModel->convertUORsToWorkingUnits(1.0);
            break;
          }
        }

        idDgOwner = pDgElm->ownerId();
      }

      if( !pPatternLinkage.isNull() && pPatternLinkage->isKindOf( OdDgLinearPatternLinkage::desc()) )
      {
        importLinearPattern( pHatch, cell, pPatternLinkage, dUORsToModelScale );
      }
      else if( !pPatternLinkage.isNull() && pPatternLinkage->isKindOf( OdDgCrossPatternLinkage::desc()) )
      {
        importCrossPattern( pHatch, cell, pPatternLinkage, dUORsToModelScale );
      }
      else if( !pPatternLinkage.isNull() && pPatternLinkage->isKindOf( OdDgDWGPatternLinkage::desc()) )
      {
        importDwgBasedPattern( pHatch, cell, pPatternLinkage, dUORsToModelScale );
      }
      else if( !pPatternLinkage.isNull() && pPatternLinkage->isKindOf( OdDgSymbolPatternLinkage::desc()) )
      {
        importSymbolPattern( pHatch, pBaseElm, pPatternLinkage, dUORsToModelScale, owner );
        //importSymbolPattern( pBaseElm, owner );
        //pHatch->erase(true);
      }
      else if( !pHatch.isNull() )
      {
        pHatch->erase(true);
      }
    }
  }

//---------------------------------------------------------------------------------------------------

  bool isAssociativeRgn( OdDgElement* pCell )
  {
    bool bRet = false;

    OdRxObjectPtrArray patterns;
    pCell->getLinkages( OdDgAttributeLinkage::kHatch, patterns );

    OdRxObjectPtrArray assocRgn;
    pCell->getLinkages( OdDgAttributeLinkage::kAssocRegion, assocRgn );

    if( patterns.size() && assocRgn.size() )
    {
      bRet = true;
    }
    else if( patterns.size() )
    {
      OdDgPatternLinkagePtr ptrnLinkage = patterns[ 0 ];

      OdString strCellName;

      if( pCell->isKindOf(OdDgCellHeader2d::desc()) )
      {
        OdDgCellHeader2dPtr pCell2d = pCell;
        strCellName = pCell2d->getName();
      }
      else if( pCell->isKindOf(OdDgCellHeader3d::desc()) )
      {
        OdDgCellHeader3dPtr pCell3d = pCell;
        strCellName = pCell3d->getName();
      }

      if( (strCellName.makeUpper() == sAssocRgn) || 
        (ptrnLinkage->getType() == OdDgPatternLinkage::kDWGPattern)
        )
      {
        bRet = true;
      }
    }

    return bRet;
  }

//---------------------------------------------------------------------------------------------------

  bool isMyNoteDimensionAfterMe( OdDgElement* e )
  {
    bool bRet = false;

    OdRxObjectPtrArray arrLinkages;
    e->getLinkages( OdDgAttributeLinkage::kDependency, arrLinkages );

    for( OdUInt32 i = 0; i < arrLinkages.size(); i++ )
    {
      OdDgDepLinkageElementIdPtr pDepLinkage = OdDgDepLinkageElementId::cast( arrLinkages[i] );

      if( !pDepLinkage.isNull() )
      {
        for( OdUInt32 j = 0; j < pDepLinkage->getCount(); j++ )
        {
          OdUInt64 uId = pDepLinkage->getAt(j);

          OdDgElementId idElm = e->database()->getElementId( OdDbHandle(uId) );

          if( !idElm.isNull() )
          {
            OdDgElementPtr pElm = idElm.openObject(OdDg::kForRead);

            if( pElm->isKindOf( OdDgDimNote::desc()) )
            {
              OdDbObjectId idObj = OdDgnImportContext::getObjectId( idElm );

              if( idObj.isNull() )
              {
                bRet = true;
                break;
              }
            }
          }
        }
      }

      if( bRet )
      {
        break;
      }
    }

    return bRet;
  }

//---------------------------------------------------------------------------------------------------

  bool createDwg3dObjectFromParasolidStream( OdStreamBufPtr& pStreamBuf, 
                                               OdDgCellHeader3d* pCell3d, 
                                                 OdDbBlockTableRecord* owner, 
                                                   const OdDgBRepEntityPE::BRepEntityType brepType )
  {
    bool bRet = true;

    try
    {
      OdRxClassPtr pService(odrxServiceDictionary()->getAt(OD_T("OdDgModelerGeometryCreator"))); //dgn mode geom

      if( pService.isNull() )
      {
        ODA_ASSERT_ONCE(!"OdDgModelerGeometryCreator is not loaded.");
        return false;
      }

      OdDgModelerGeometryCreatorPtr pCreator = pService->create();

      if( pCreator.isNull() )
      { 
        ODA_ASSERT_ONCE(!"OdDgModelerGeometryCreator has not been created."); 
        return false;
      }

      OdArray<OdDgModelerGeometryPtr> models;

      if( pCreator->createModeler(models, pStreamBuf) != eOk || !models.size() )
      {
        ODA_ASSERT_ONCE(!"Models has not been created.");
        return false;
      }

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

      OdBrBrep brbrep;
      models[0]->brep(brbrep); //dgn

      OdBrepBuilderFillerModulePtr pBuilderFiller = ::odrxDynamicLinker()->loadModule(OdBrepBuilderFillerModuleName, false);

      if( pBuilderFiller.isNull() )
      {
        ODA_ASSERT_ONCE(!"OdBrepBuilderFillerModule has not been loaded.");
        return false; 
      }

      //OdMaterialHelper materialHelper;
      OdBrepBuilderFillerParams params;
      BrepBuilderInitialData data;

      if( pBuilderFiller->getDataFrom(brbrep, NULL, params, data) != eOk)
      {
        ODA_ASSERT_ONCE(!"No brep builder filer data.");
        return false; 
      }

      OdGeCurve3dPtrArray arrEdges;
      OdGeCurve2dPtrArray arrCoedges;
      OdGeSurfacePtrArray arrSurfaces;

      if( pBuilderFiller->initFrom(brepBuilder, data, arrEdges, arrCoedges, arrSurfaces) != eOk)
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

      OdDgGraphicsElementPtr pInternalGraphics;
      OdDgElementIteratorPtr localPtr = pCell3d->createIterator();
      getInternalGraphicsElement( localPtr, pInternalGraphics );

      if( pInternalGraphics.isNull() )
        pInternalGraphics = pCell3d;

      OdDbEntityPtr pSolidEnt;

      double dInternalScale = OdDgBRepEntityPEPtr(pCell3d)->getInternalScale(*pCell3d, 0.0);
      OdGeMatrix3d mx( OdGeMatrix3d::translation(pCell3d->getOrigin().asVector())*pCell3d->getTransformation() * OdGeMatrix3d::scaling(dInternalScale) );

      OdDbBlockTableRecord* pBlockToAdd = owner;

      OdDbBlockReferencePtr   pBlockReference;
      OdDbBlockTableRecordPtr pDwgCellDefinition;

      if( !mx.isUniScaledOrtho() )
      {
        OdDbObjectId idBlockTable = owner->database()->getBlockTableId();

        if( !idBlockTable.isNull() )
        {
          OdDbBlockTablePtr pBlockTable = idBlockTable.openObject(OdDb::kForWrite);
          OdString strDefNameBase = OD_T("TransformedSolid");

          OdString strDefName = strDefNameBase;

          OdUInt32 uCount = 1;

          OdDgnImportContext::getCellBlockNextIndex(strDefName, uCount);

          while( strDefName.isEmpty() || !pBlockTable->getAt( strDefName).isNull() )
          {
            if( strDefNameBase.isEmpty() )
            {
              strDefName.format(L"TransformedSolid%d", uCount );
            }
            else
            {
              strDefName.format(L"_%d", uCount );
              strDefName = strDefNameBase + strDefName;
            }

            uCount++;
          }

          OdDgnImportContext::setCellBlockNextIndex(strDefNameBase, uCount + 1);

          OdString repairedName;

          if( OdDbSymUtil::repairSymbolName(repairedName, strDefName, owner->database() ) == eOk && !repairedName.isEmpty() )
          {
            strDefNameBase = repairedName;
            strDefName = strDefNameBase;
          }

          pDwgCellDefinition = OdDbBlockTableRecord::createObject();
          pDwgCellDefinition->setName( strDefName );

          pBlockTable->add( pDwgCellDefinition );

          pBlockToAdd = pDwgCellDefinition.get();

          pBlockReference = OdDbBlockReference::createObject();
          pBlockReference->setBlockTableRecord( pDwgCellDefinition->objectId() );
          pBlockReference->setBlockTransform(mx);
          owner->appendOdDbEntity( pBlockReference );
        }
      }

      if( brepType == OdDgBRepEntityPE::eSmartSurface )
      {
        OdDbBodyPtr pBody = OdDbBody::createObject();
        pBody->setBody(pGeometry);
        pBlockToAdd->appendOdDbEntity(pBody);
        copyEntityProperties(pInternalGraphics, pBody);
        pSolidEnt = pBody;
      }
      else
      {
        OdDb3dSolidPtr p3dSolid = OdDb3dSolid::createObject();
        p3dSolid->setBody(pGeometry);
        pBlockToAdd->appendOdDbEntity(p3dSolid);
        copyEntityProperties(pInternalGraphics, p3dSolid);
        pSolidEnt = p3dSolid;
      }

      if( mx.isUniScaledOrtho() )
        pSolidEnt->transformBy(mx);
    }
    catch (...)
    {
      ODA_ASSERT_ONCE(!"Parasolid to ACIS conversion error.");
      return false;
    }

    return bRet;
  }

  //---------------------------------------------------------------------------------------------------

  bool createDwg3dObjectFromAcisStream( OdStreamBufPtr& pStreamBuf, 
                                          OdDgCellHeader3d* pCell3d, 
                                            OdDbBlockTableRecord* owner, 
                                              const OdDgBRepEntityPE::BRepEntityType brepType )
  {
    bool bRet = true;

    try
    {
      OdDgGraphicsElementPtr pInternalGraphics;
      OdDgElementIteratorPtr localPtr = pCell3d->createIterator();
      getInternalGraphicsElement( localPtr, pInternalGraphics );

      if( pInternalGraphics.isNull() )
        pInternalGraphics = pCell3d;

      OdDbEntityPtr pSolidEnt;

      double dInternalScale = OdDgBRepEntityPEPtr(pCell3d)->getInternalScale(*pCell3d, 0.0);
      OdGeMatrix3d mx( OdGeMatrix3d::translation(pCell3d->getOrigin().asVector())*pCell3d->getTransformation() * OdGeMatrix3d::scaling(dInternalScale) );

      OdDbBlockTableRecord* pBlockToAdd = owner;

      OdDbBlockReferencePtr   pBlockReference;
      OdDbBlockTableRecordPtr pDwgCellDefinition;

      if( !mx.isUniScaledOrtho() )
      {
        OdDbObjectId idBlockTable = owner->database()->getBlockTableId();

        if( !idBlockTable.isNull() )
        {
          OdDbBlockTablePtr pBlockTable = idBlockTable.openObject(OdDb::kForWrite);
          OdString strDefNameBase = OD_T("TransformedSolid");

          OdString strDefName = strDefNameBase;

          OdUInt32 uCount = 1;

          OdDgnImportContext::getCellBlockNextIndex(strDefName, uCount);

          while( strDefName.isEmpty() || !pBlockTable->getAt( strDefName).isNull() )
          {
            if( strDefNameBase.isEmpty() )
            {
              strDefName.format(L"TransformedSolid%d", uCount );
            }
            else
            {
              strDefName.format(L"_%d", uCount );
              strDefName = strDefNameBase + strDefName;
            }

            uCount++;
          }

          OdDgnImportContext::setCellBlockNextIndex(strDefNameBase, uCount + 1);

          OdString repairedName;

          if( OdDbSymUtil::repairSymbolName(repairedName, strDefName, owner->database() ) == eOk && !repairedName.isEmpty() )
          {
            strDefNameBase = repairedName;
            strDefName = strDefNameBase;
          }

          pDwgCellDefinition = OdDbBlockTableRecord::createObject();
          pDwgCellDefinition->setName( strDefName );

          pBlockTable->add( pDwgCellDefinition );

          pBlockToAdd = pDwgCellDefinition.get();

          pBlockReference = OdDbBlockReference::createObject();
          pBlockReference->setBlockTableRecord( pDwgCellDefinition->objectId() );
          pBlockReference->setBlockTransform(mx);
          owner->appendOdDbEntity( pBlockReference );
        }
      }

      if( brepType == OdDgBRepEntityPE::eSmartSurface )
      {
        OdDbBodyPtr pBody = OdDbBody::createObject();
        pBody->acisIn(pStreamBuf);
        owner->appendOdDbEntity(pBody);
        copyEntityProperties(pInternalGraphics, pBody);
        bRet = true;
      }
      else
      {
        OdDb3dSolidPtr p3dSolid = OdDb3dSolid::createObject();
        p3dSolid->acisIn(pStreamBuf);
        owner->appendOdDbEntity(p3dSolid);
        copyEntityProperties(pInternalGraphics, p3dSolid);
        bRet = true;
      }

      if( mx.isUniScaledOrtho() )
        pSolidEnt->transformBy(mx);
    }
    catch (...)
    {
      ODA_ASSERT_ONCE(!"Parasolid to ACIS conversion error.");
      return false;
    }

    return bRet;
  }

  //---------------------------------------------------------------------------------------------------

  bool importBrepElement( OdDgCellHeader3d* pCell3d, OdDbBlockTableRecord*owner, const OdDgBRepEntityPE::BRepEntityType brepType )
  {
    bool bRet = false;
    // Got stream with 3dObject data.
    try
    {
      OdStreamBufPtr pStreamBuf = OdMemoryStream::createNew();
      OdUInt32 ver = OdDgBRepEntityPEPtr(pCell3d)->brepVersion(pCell3d);

      if( (ver & OdDgModelerGeometry::kVersionMask) > OdDgModelerGeometry::kAS_7_0)
      {
        ver &= ~OdDgModelerGeometry::kVersionMask;
        ver |= OdDgModelerGeometry::kAS_7_0;
      }

      bool b = OdDgBRepEntityPEPtr(pCell3d)->brepOut(pCell3d, ver, *pStreamBuf) == eOk;
      pStreamBuf->rewind();

      if (b && pStreamBuf->length() != 0)
      {
        OdDgBRepEntityPE::OdDgBRepEntityDataType dataType = OdDgBRepEntityPEPtr(pCell3d)->getDataType(pCell3d);

        if( dataType == OdDgBRepEntityPE::kAcisData )
        {
          bRet = createDwg3dObjectFromAcisStream( pStreamBuf, pCell3d, owner, brepType );
        }
        else if( (dataType == OdDgBRepEntityPE::kParasolidData) )
        {
          bRet = createDwg3dObjectFromParasolidStream( pStreamBuf, pCell3d, owner, brepType );
        }
      }
    }
    catch(...)
    {
      bRet = false;
    }

    return bRet;
  }

//---------------------------------------------------------------------------------------------------

  void subImportElement(OdDgElement* e, OdDbBlockTableRecord* owner) ODRX_OVERRIDE
  {
    OdDbDatabase* pDb = owner->database();

    T* cell = static_cast<T*>(e);

    if( !pDb || !cell )
    {
      return;
    }

    if( OdDgnImportContext::getImportCurvesOnlyFlag() )
    {
      OdDgElementIteratorPtr pIter = cell->createIterator();

      if( !pIter.isNull() )
      {
        for(; !pIter->done(); pIter->step())
        {
          OdDgElementPtr pDgElement = pIter->item().openObject();

          if (!pDgElement.isNull())
          {
            ((OdDgnImportPE*)pDgElement->queryX(OdDgnImportPE::desc()))->importElement(pDgElement, owner);
          }
        }
      }

      return;
    }

    if( e->isKindOf(OdDgCellHeader3d::desc()) && (OdDgnImportContext::getObject3dImportMode() == 1) )
    {
      OdDgCellHeader3d* pCell3d = static_cast<OdDgCellHeader3d*>(e);
      OdDgBRepEntityPE::BRepEntityType brepType = OdDgBRepEntityPEPtr(pCell3d)->type(pCell3d);

      if (brepType != OdDgBRepEntityPE::eNotBRepEntity)
      {
        if( importBrepElement( pCell3d, owner, brepType ) )
        {
          return;
        }
      }
    }

    if( OdDgnImportContext::isNoteDimensionCell( e->elementId() ) )
    {
      OdDgnImportContext::removeNoteDimensionCell(e->elementId());
      return; 
    }
    else if( isMyNoteDimensionAfterMe( e ) )
    {
      return;
    }

    OdDbObjectId idBlockTable = pDb->getBlockTableId();

    if( idBlockTable.isNull() )
    {
      return;
    }

    OdDbBlockTablePtr pBlockTable = idBlockTable.openObject(OdDb::kForWrite);

    if( pBlockTable.isNull() )
    {
      return;
    }

    OdString strDefNameBase = cell->getName();

    strDefNameBase = strDefNameBase.trimLeft();
    strDefNameBase = strDefNameBase.trimRight();

    OdString strDefName = strDefNameBase;

    OdUInt32 uCount = 1;

    OdDgnImportContext::getCellBlockNextIndex(strDefName, uCount);

    while( strDefName.isEmpty() || !pBlockTable->getAt( strDefName).isNull() )
    {
      if( strDefNameBase.isEmpty() )
      {
        strDefName.format(L"AnonymusCell%d", uCount );
      }
      else
      {
        strDefName.format(L"_%d", uCount );
        strDefName = strDefNameBase + strDefName;
      }

      uCount++;
    }

    OdDgnImportContext::setCellBlockNextIndex(strDefNameBase, uCount + 1);

    OdString repairedName;

    if( OdDbSymUtil::repairSymbolName(repairedName, strDefName, pDb ) == eOk && !repairedName.isEmpty() )
    {
      strDefNameBase = repairedName;
      strDefName = strDefNameBase;
    }

    OdDbBlockTableRecordPtr pDwgCellDefinition = OdDbBlockTableRecord::createObject();
    pDwgCellDefinition->setName( strDefName );

    pBlockTable->add( pDwgCellDefinition );

    OdDbBlockReferencePtr pBlockReference = OdDbBlockReference::createObject();
    pBlockReference->setBlockTableRecord( pDwgCellDefinition->objectId() );
    owner->appendOdDbEntity( pBlockReference );

    OdDgnImportPathToDwgObject dwgPath;
    dwgPath.m_idPath.objectIds().push_back( pBlockReference->objectId() );
    dwgPath.m_bExists = true;
    OdDgnImportContext::addObjectPath( e->elementId(), dwgPath );

    bool bImportInvisible = OdDgnImportContext::isImportInvisible();

    bool bAssocRgn = isAssociativeRgn( cell );

    if( !bImportInvisible && bAssocRgn )
    {
      OdDgnImportContext::setImportInvisible(true);
    }
    double ts_scale = 1.0;
    bool finished = false;
    bool bImportThroughDraw = isImportThroughDraw( cell );
    if( bImportThroughDraw )
    {      
      if (cell->isPolygonWithHoles()) // fixme: unknown brep type
      {
        importPolygonWithHoles(e, pDwgCellDefinition, false);
      }
      else
      {
        OdDgnImportPE::subImportElement(e, pDwgCellDefinition);
      }
    }
    else
    {
      OdDgnImportContext::addCellHeaderIdToPath( pBlockReference->objectId() );

      OdDgElementIteratorPtr pIter = cell->createIterator();

      if( !pIter.isNull() )
      {
        for(; !pIter->done(); pIter->step())
        {
          OdDgElementPtr pDgElement = pIter->item().openObject();

          if (!pDgElement.isNull())
          {
             ((OdDgnImportPE*)pDgElement->queryX(OdDgnImportPE::desc()))->importElement(pDgElement, pDwgCellDefinition);
          }
        }
      }

      if( bAssocRgn )
      {
        importPolygonWithHoles( e, pDwgCellDefinition, true );
      }

      OdDgnImportContext::removeLastCellHeaderIdFromPath();
    }

    OdDgnImportContext::setImportInvisible(bImportInvisible);

    OdGeVector3d vrOffset = getCellHeaderOrigin( cell ).asVector();
    OdGeMatrix3d matRotation = getCellHeaderRotation( cell );

    if( OdZero(matRotation.getCsXAxis().length()) || OdZero(matRotation.getCsYAxis().length()) || OdZero(matRotation.getCsZAxis().length()) )
    {
      matRotation = OdGeMatrix3d::kIdentity;
    }

    OdGeScale3d  curScale;
    curScale.removeScale( matRotation );
    double dScale = fabs( curScale.sx ) * ts_scale;
    matRotation = OdGeMatrix3d::scaling(dScale) * matRotation;

    bool bCorrectMatrix = !matRotation.isSingular();

    if( bCorrectMatrix )
    {
      matRotation = matRotation.invert();
    }

    OdGeMatrix3d matTransform = OdGeMatrix3d::kIdentity;
    
    if( bCorrectMatrix )
    {
      matTransform = matRotation * OdGeMatrix3d::translation(-vrOffset);
    }

    bCorrectMatrix = prepareMatrixToBlockTransform( matTransform, true );

    if( !bCorrectMatrix )
      matTransform = OdGeMatrix3d::kIdentity;

    if( matTransform != OdGeMatrix3d::kIdentity)
    {
      for (OdDbObjectIteratorPtr it = pDwgCellDefinition->newIterator(); !it->done(); it->step())
      {
        OdDbEntityPtr e = it->objectId().safeOpenObject(OdDb::kForWrite);
        e->transformBy(matTransform);
      }
    }

    matTransform = matTransform.invert();

    OdDgGraphicsElement* pBaseElm = cell;

    OdDgElementId idOwner = cell->ownerId();

    if( !idOwner.isNull() )
    {
      OdDgElementPtr pDgnOwner = idOwner.openObject(OdDg::kForRead);

      if( !pDgnOwner.isNull() && (pDgnOwner->getElementType() == OdDgElement::kTypeCellHeader) )
      {
        if( isAssociativeRgn( pDgnOwner ) )
        {
          pBaseElm = (OdDgGraphicsElement*)(pDgnOwner.get());
        }
      }
    }

    pBlockReference->setBlockTransform( matTransform );

    copyEntityProperties( pBaseElm, pBlockReference );

    OdDgCellBoundaryClipPEPtr pCellBoundaryClip = OdDgCellBoundaryClipPEPtr(OdRxObjectPtr(e));

    if( !pCellBoundaryClip.isNull() )
    {
      OdGiClipBoundary clipBoundary;

      if( pCellBoundaryClip->getBoundaryClip(e, clipBoundary) )
      {
        clipBoundary.m_xInverseBlockRefXForm = pBlockReference->blockTransform().invert();

        if( pBlockReference->extensionDictionary().isNull() )
        {
          pBlockReference->createExtensionDictionary();
        }

        OdDbDictionaryPtr pDict = OdDbDictionary::cast(pBlockReference->extensionDictionary().openObject(OdDb::kForWrite));

        if( !pDict.isNull() )
        {
          OdDbDictionaryPtr pFDict = OdDbDictionary::cast(pDict->getAt(OD_T("ACAD_FILTER"), OdDb::kForWrite));

          if( pFDict.isNull() )
          {
            pFDict = OdDbDictionary::createObject();
            pDict->setAt( OD_T("ACAD_FILTER"), pFDict );
          }

          if( !pFDict.isNull() )
          {
            OdDbSpatialFilterPtr pSp = OdDbSpatialFilter::cast(pFDict->getAt(OD_T("SPATIAL"), OdDb::kForWrite));

            if( pSp.isNull() )
            {
              pSp = OdDbSpatialFilter::createObject();
              pFDict->setAt( OD_T("SPATIAL"), pSp );
            }

            if( !pSp.isNull() )
            {
              double dFrontClip = ODDB_INFINITE_XCLIP_DEPTH;
              double dBackClip = ODDB_INFINITE_XCLIP_DEPTH;

              if( clipBoundary.m_bClippingFront )
              {
                dFrontClip = clipBoundary.m_dFrontClipZ;
              }

              if( clipBoundary.m_bClippingBack )
              {
                dBackClip = clipBoundary.m_dBackClipZ;
              }
              pSp->setDefinition( clipBoundary );
            }
          }
        }
      }
    }
  }
};

//---------------------------------------------------------------------------------------------------

}
#endif // _DGN_IMPORT_CELLHEADER_INCLUDED_
