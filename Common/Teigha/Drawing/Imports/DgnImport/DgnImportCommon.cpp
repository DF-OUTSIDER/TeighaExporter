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
#include "DgnImportImpl.h"
#include <DbLayerTableRecord.h>
#include <DgMaterialTableRecord.h>
#include <Gi/GiUtils.h>
#include "Gi/GiBaseVectorizer.h"
#include "Gi/GiGeometrySimplifier.h"
#include "DgGiContext.h"
#include "DgnImportCommon.h"
#include "DgnImportContext.h"
#include <DbLinetypeTable.h>
#include "Dg3DObject.h"
#include <DgBRepEntityPE.h>
#include <DgLine.h>
#include <DgArc.h>
#include <DbPolyline.h>
#include <DgLineString.h>
#include <DgEllipse.h>
#include <DgBSplineCurve.h>
#include <DgCurve.h>
#include "DgShape.h"
#include "DgPointString.h"
#include <DbBlockTableRecord.h>
#include <DbDictionary.h>
#include "Gs/LineWtIndex.h"
#include "DbSymUtl.h"
#include <DbRegAppTable.h>
#include <DbRegAppTableRecord.h>
#include <Ge/GeCircArc2d.h>
#include <Ge/GeLine3d.h>

#include <DgnLS/DbLSXData.h>

using namespace TD_DGN_IMPORT;

namespace TD_DGN_IMPORT {

//------------------------------------------------------------------------------------------------------------
//                        Implementation of class OdDgnImportLineWeightMap
//------------------------------------------------------------------------------------------------------------
OdDb::LineWeight OdDgnImportLineWeightsMapImpl::m_arrDefaultLineWeights[] = 
{
  OdDb::kLnWt000, // 0
  OdDb::kLnWt013, // 1
  OdDb::kLnWt030, // 2
  OdDb::kLnWt040, // 3
  OdDb::kLnWt053, // 4
  OdDb::kLnWt070, // 5
  OdDb::kLnWt080, // 6
  OdDb::kLnWt100, // 7
  OdDb::kLnWt106, // 8
  OdDb::kLnWt120, // 9
  OdDb::kLnWt140, // 10
  OdDb::kLnWt158, // 11
  OdDb::kLnWt158, // 12
  OdDb::kLnWt158, // 13
  OdDb::kLnWt200, // 14
  OdDb::kLnWt211, // 15
  OdDb::kLnWt211, // 16
  OdDb::kLnWt211, // 17
  OdDb::kLnWt211, // 18
  OdDb::kLnWt211, // 19
  OdDb::kLnWt211, // 20
  OdDb::kLnWt211, // 21
  OdDb::kLnWt211, // 22
  OdDb::kLnWt211, // 23
  OdDb::kLnWt211, // 24
  OdDb::kLnWt211, // 25
  OdDb::kLnWt211, // 26
  OdDb::kLnWt211, // 27
  OdDb::kLnWt211, // 28
  OdDb::kLnWt211, // 29
  OdDb::kLnWt211, // 30
  OdDb::kLnWt211, // 31
  OdDb::kLnWtByLayer, // ByLevel
  OdDb::kLnWtByBlock  // ByCell
};
//------------------------------------------------------------------------------------------------------------

OdDgnImportLineWeightsMapImpl::OdDgnImportLineWeightsMapImpl()
{
  resetToDefaults();
}

//------------------------------------------------------------------------------------------------------------

void OdDgnImportLineWeightsMapImpl::resetToDefaults()
{
  for( OdUInt32 i = 0; i < 34; i++ )
  {
    m_arrLineWeights[i] = m_arrDefaultLineWeights[i];
  }
}

//------------------------------------------------------------------------------------------------------------

OdResult OdDgnImportLineWeightsMapImpl::setLineWeightForDgnIndex( OdUInt32 uIndex,  OdDb::LineWeight lineWeight )
{
  if( uIndex < 32 )
  {
    int iIndex = lineWeightIndex(lineWeight);

    if( iIndex == 0 && (lineWeight != OdDb::kLnWt000) )
      return eInvalidIndex;

    m_arrLineWeights[uIndex] = lineWeight;
  }
  else
  {
    return eInvalidIndex;
  }

  return eOk;
}

//------------------------------------------------------------------------------------------------------------

OdDb::LineWeight  OdDgnImportLineWeightsMapImpl::getLineWeightByDgnIndex( OdUInt32 uIndex ) const
{
  OdDb::LineWeight retVal = OdDb::kLnWt000;

  if( uIndex < 32 )
  {
    retVal = m_arrLineWeights[uIndex];
  }
  else if( uIndex == OdDg::kLineWeightByLevel )
  {
    retVal = m_arrLineWeights[32];
  }
  else if( uIndex == OdDg::kLineWeightByCell )
  {
    retVal = m_arrLineWeights[33];
  }

  return retVal;
}

//------------------------------------------------------------------------------------------------------------

OdDb::LineWeight OdDgnImportLineWeightsMapImpl::getDefaultLineWeightByDgnIndex( OdUInt32 uIndex )
{
  OdDb::LineWeight retVal = OdDb::kLnWt000;

  if( uIndex < 32 )
  {
    retVal = m_arrDefaultLineWeights[uIndex];
  }
  else if( uIndex == OdDg::kLineWeightByLevel )
  {
    retVal = m_arrDefaultLineWeights[32];
  }
  else if( uIndex == OdDg::kLineWeightByCell )
  {
    retVal = m_arrDefaultLineWeights[33];
  }

  return retVal;
}

//------------------------------------------------------------------------------------------------------------

void OdDgnImportLineWeightsMapImpl::copyLineWeightsMap( const OdDgnImportLineWeightsMapPtr& pMap )
{
  if( !pMap.isNull() )
  {
    for( OdUInt32 i = 0; i < 32; i++ )
    {
      setLineWeightForDgnIndex( i, pMap->getLineWeightByDgnIndex(i));
    }
  }
}

//------------------------------------------------------------------------------------------------------------
//                                      Useful functions
//------------------------------------------------------------------------------------------------------------

OdDgElementId odDgnImportGetMaterialId(OdDgGraphicsElement* src, OdRxObjectPtrArray& pLinkages)
{
  switch( src->getElementType() )
  {
    case OdDgElement::kTypeShape:
    case OdDgElement::kTypeComplexShape:
    case OdDgElement::kTypeCone:
    case OdDgElement::kTypeConicSection:
    case OdDgElement::kTypeSolid:
    case OdDgElement::kTypeMesh:
    case OdDgElement::kTypeBSplineSurface:
    case OdDgElement::kTypeEllipse:
    case OdDgElement::kTypeSurface:
    case OdDgElement::kTypeText:
    case OdDgElement::kTypeCellHeader:
    {
      OdDgElementId idMaterial;
      OdDgDatabase* pDb = src->database();
      for(unsigned iLinkage = 0 ; iLinkage < pLinkages.size(); iLinkage++ )
      {
        OdDgInternalMaterialLinkagePtr pMaterialLinkage = OdDgInternalMaterialLinkage::cast( pLinkages[iLinkage] );
        if (!pMaterialLinkage.isNull())
        {
          OdDbHandle hndMat = pMaterialLinkage->getMaterialTableRecordId();
          if (pDb)
            idMaterial = pDb->getElementId(hndMat);
          if( !idMaterial.isNull() )
          {
            OdDgMaterialTableRecordPtr pMatTest = dynamic_cast<OdDgMaterialTableRecord*>(idMaterial.openObject(OdDg::kForRead).get());
            if( pMatTest.isNull() )
              idMaterial = 0;
          }
          break;
        }
      }
      if (idMaterial.isNull())
      {
        src->getLinkages(OdDgAttributeLinkage::kExternalMaterial, pLinkages);
        for (unsigned iLinkage = 0 ; iLinkage < pLinkages.size(); iLinkage++ )
        {
          OdDgMaterialLinkagePtr pExternalMaterialLinkage = OdDgMaterialLinkage::cast( pLinkages[iLinkage] );
          if (!pExternalMaterialLinkage.isNull())
          {
            OdString strMatName = pExternalMaterialLinkage->getMaterialName();
            if (!strMatName.isEmpty())
            {
              OdDgMaterialTablePtr pMaterialTable = pDb->getMaterialTable( OdDg::kForRead );
              if (!pMaterialTable.isNull())
              {
                idMaterial = pMaterialTable->getAt( strMatName );
                if (!idMaterial.isNull())
                  break;
              }
            }
          }
        }
      }
      return idMaterial;
    } 
    break;
  default:
    break;
  }
  return OdDgElementId::kNull;
}

//-----------------------------------------------------------------------------------------------------------

// Converts OdDgLineStyleModificationLinkage in the form convinient for rendering
bool odDgnImportLineStyleModifiers(const OdDgLineStyleModificationLinkage *pLStyleModLinkage, OdDgElementId ownerId,
                                   OdGiDgLinetypeModifiers &lineTypeModifiers, double &modScale)
{
  if (!pLStyleModLinkage)
    throw OdError(eNullPtr);

  double lsScale = modScale = 1.0;

  if (pLStyleModLinkage->getUseLineStyleScaleFlag())
    modScale = pLStyleModLinkage->getLineStyleScale();

  bool bApplyModifiers = !OdEqual(modScale, 1.0);

  { // UORs
    OdDgModelPtr pModel;
    while (!ownerId.isNull())
    {
      OdDgElementPtr pOwner = ownerId.openObject(OdDg::kForRead);
      if (pOwner.isNull())
        break;
      if (pOwner->isKindOf(OdDgModel::desc()))
      {
        pModel = pOwner;
        break;
      }
      ownerId = pOwner->ownerId();
    }
    if (!pModel.isNull())
      lsScale = pModel->convertUORsToWorkingUnits(1.0);
  }

  if (pLStyleModLinkage->getUseLineStyleDashScaleFlag())
  {
    lineTypeModifiers.setDashScaleFlag(true);
    lineTypeModifiers.setDashScale(pLStyleModLinkage->getLineStyleDashScale());
    bApplyModifiers = true;
  }
  if (pLStyleModLinkage->getUseLineStyleGapScaleFlag())
  {
    lineTypeModifiers.setGapScaleFlag(true);
    lineTypeModifiers.setGapScale(pLStyleModLinkage->getLineStyleGapScale());
    bApplyModifiers = true;
  }

  const OdDgLineStyleModificationLinkage::OdDgLsModWidthMode modWidthMode = pLStyleModLinkage->getWidthMode();
  if (modWidthMode != OdDgLineStyleModificationLinkage::kLsModNoWidth)
  {
    double dWidthScale = 1.0f;

    if (pLStyleModLinkage->getUseLineStyleTrueWidthFlag())
      dWidthScale = lsScale;

    lineTypeModifiers.setWidthMode((OdGiDgLinetypeModifiers::WidthMode)modWidthMode);
    lineTypeModifiers.setWidth(pLStyleModLinkage->getLineStyleWidth() * dWidthScale);
    if (modWidthMode == OdDgLineStyleModificationLinkage::kLsModTaperedWidth)
      lineTypeModifiers.setEndWidth(pLStyleModLinkage->getLineStyleEndWidth() * dWidthScale);

    lineTypeModifiers.setTrueWidthFlag(pLStyleModLinkage->getUseLineStyleTrueWidthFlag());
    bApplyModifiers = true;
  }

  switch (pLStyleModLinkage->getShiftMode())
  {
    case OdDgLineStyleModificationLinkage::kLsModDistance:
      lineTypeModifiers.setShiftMode(OdGiDgLinetypeModifiers::kLsShiftDistance);
      lineTypeModifiers.setPhase(pLStyleModLinkage->getLineStyleShift() * lsScale);
      bApplyModifiers = true;
    break;
    case OdDgLineStyleModificationLinkage::kLsModFraction:
      lineTypeModifiers.setShiftMode(OdGiDgLinetypeModifiers::kLsShiftFraction);
      lineTypeModifiers.setPhase(pLStyleModLinkage->getLineStyleFractionPhase());
      bApplyModifiers = true;
    break;
    case OdDgLineStyleModificationLinkage::kLsModCentered:
      lineTypeModifiers.setShiftMode(OdGiDgLinetypeModifiers::kLsShiftCentered);
      bApplyModifiers = true;
    break;
    default: break;
  }

  const OdDgLineStyleModificationLinkage::OdDgLsModCornerMode modCornerMode = pLStyleModLinkage->getCornerMode();
  if (modCornerMode != OdDgLineStyleModificationLinkage::kLsModFromLineStyle)
  {
    lineTypeModifiers.setCornersMode((OdGiDgLinetypeModifiers::CornersMode)modCornerMode);
    bApplyModifiers = true;
  }
  return bApplyModifiers;
}

//-------------------------------------------------------------------------------------------------

void odDgnImportSetLSModXData(OdDbEntity *pEntity, const OdGiDgLinetypeModifiers &lsMod, double lsScale)
{
  if (!pEntity)
    throw OdError(eNullPtr);
  ::oddbDgnLSWriteEntityXData(pEntity, lsMod, lsScale);
}

//-------------------------------------------------------------------------------------------------

OdDb::LineWeight odDgnImportLineweightMapping(OdUInt32 mappingVal)
{
  DgnImporter* pImporter = OdDgnImportContext::getDgnImporter();
  
  if( pImporter )
  {
    return pImporter->getLineWeightByDgnIndex(mappingVal);
  }
  else
  {
    return OdDgnImportLineWeightsMapImpl::getDefaultLineWeightByDgnIndex( mappingVal );
  }
}

//------------------------------------------------------------------------

static double globalLinetypeScale(OdDgGraphicsElement* src)
{
  OdDgElementPtr pElem = src;

  double dScaleMultiplier = 1.0;

  if( OdDgnImportContext::getGlobalLineScale( dScaleMultiplier ) )
  {
    if( OdZero(dScaleMultiplier) )
    {
      dScaleMultiplier = src->database()->getLineStyleScale();
    }
  }
  else
  {
    do 
    {
      pElem = OdDgElement::cast(pElem->ownerId().openObject());

      if( !pElem.isNull() )
      {
        OdDgModelPtr pModel = OdDgModel::cast(pElem);

        if( !pModel.isNull() )
        {
          dScaleMultiplier = pModel->getLinestyleScale();

          if( OdZero(dScaleMultiplier) )
          {
            dScaleMultiplier = pModel->database()->getLineStyleScale();
            break;
          }
        }
      }
    } while (!pElem.isNull());
  }

  if( OdZero(dScaleMultiplier) )
  {
    dScaleMultiplier = 1.0;
  }

  return dScaleMultiplier;
}

//----------------------------------------------------------------------------------------------------

void setDwgLineTypeIdByDgnLineTypeIndex(OdDbDatabase* pDb, OdDgDatabase* pDgnDb, OdUInt32 uDgnLineStyle, OdDbObjectId& idDwgLineType )
{
  // linetype
  if( uDgnLineStyle < 8 && uDgnLineStyle >= 0)
  {
    // internal
    if( uDgnLineStyle == 0 )
      idDwgLineType = pDb->getLinetypeContinuousId();
    else
    {
      OdDbLinetypeTablePtr pTable = pDb->getLinetypeTableId().openObject();
      idDwgLineType = pTable->getAt(OdString().format(OD_T("DGN%d"), uDgnLineStyle) );
    }
  }
  else if( uDgnLineStyle == OdDg::kLineStyleByLevel)
  {
    idDwgLineType = pDb->getLinetypeByLayerId();
  }
  else if ( uDgnLineStyle == OdDg::kLineStyleByCell)
  {
    idDwgLineType = pDb->getLinetypeByBlockId();
  }
  else
  {
    OdDgElementId idDgnLineStyle;

    if( pDgnDb )
    {
      idDgnLineStyle = pDgnDb->getLineStyleTable()->getAt( uDgnLineStyle );
    }

    if( !idDgnLineStyle.isNull() )
    {
      idDwgLineType = OdDgnImportContext::getObjectId( idDgnLineStyle );
    }
  }

  if( idDwgLineType.isNull())
  {
    idDwgLineType = pDb->getLinetypeContinuousId();
  }
}

//----------------------------------------------------------------------------------------------------

void setDwgColorByDgnIndex( OdDgDatabase* pDb, OdCmColor& dstClr, OdUInt32 uColorIndex )
{
  {
    OdUInt32 color = uColorIndex;
    ODCOLORREF c = OdDgColorTable::lookupRGB(pDb, color);

    if (color == OdDgCmEntityColor::kByLevel)
    {
      dstClr.setColorIndex(OdCmEntityColor::kACIbyLayer);
    }
    else if (color == OdDgCmEntityColor::kByCell)
    {
      dstClr.setColorIndex(OdCmEntityColor::kACIbyBlock);
    }
    else if( color < 256 && !OdDgnImportContext::getConvertColorIndexToTrueColorFlag() )
    {
      OdDgnImportColorIndexDescription dwgColor;

      bool bSetTrueColor = true;

      if( OdDgnImportContext::getDwgColor( color, dwgColor ) )
      {
        if( !dwgColor.m_bUseTrueColor )
        {
          bSetTrueColor = false;
          dstClr.setColorIndex(dwgColor.m_uDwgColorIndex);
        }
      }

      if( bSetTrueColor )
      {
        dstClr.setRGB(ODGETRED(c), ODGETGREEN(c), ODGETBLUE(c));
      }
    }
    else
    {
      dstClr.setRGB(ODGETRED(c), ODGETGREEN(c), ODGETBLUE(c));
    }
  }
}

//----------------------------------------------------------------------------------------------------

void setDwgColorByDgnIndex( OdDgDatabase* pDb, OdDbEntity* dst, OdUInt32 uColorIndex )
{
  OdCmColor dwgColor;

  setDwgColorByDgnIndex( pDb, dwgColor, uColorIndex );

  dst->setColor(dwgColor);
}

//----------------------------------------------------------------------------------------------------

OdDgGraphicsElementPtr getFirstGraphicsElement( OdDgElementIteratorPtr pIter )
{
  OdDgGraphicsElementPtr pRet;

  if( pIter.isNull() )
    return pRet;

  for(; !pIter->done(); pIter->step() )
  {
    OdDgElementPtr pElm = pIter->item().openObject(OdDg::kForRead);
    
    if( !pElm.isNull() && pElm->isKindOf(OdDgGraphicsElement::desc()) )
    {
      pRet = pElm;
      break;
    }
  }

  return pRet;
}

//----------------------------------------------------------------------------------------------------

void setVisibility(OdDgGraphicsElement* src, OdDbEntity* dst)
{
  bool bInvisible = src->getInvisibleFlag();

  OdDgGraphicsElement::Class curClass = src->getClass();

  dst->setVisibility(src->getInvisibleFlag() ? OdDb::kInvisible : OdDb::kVisible);

  if( !bInvisible )
  {
    if( OdDgnImportContext::isLevelMask() )
    {
      bool bSkipLevelCheck = false;
      OdUInt32 uLevelEntryId = src->getLevelEntryId();

      if( (src->getElementType() == OdDgElement::kTypeCellHeader) ||
          (src->getElementType() == OdDgElement::kTypeSolid) ||
          (src->getElementType() == OdDgElement::kTypeSurface)
        )
      {
        if( src->isKindOf(OdDgCellHeader2d::desc()) )
        {
          OdDgCellHeader2dPtr pCell2d = src;

          if( pCell2d->isPolygonWithHoles() )
          {
            OdDgGraphicsElementPtr pIntGraph;
            OdDgElementIteratorPtr localPtr = pCell2d->createIterator();
            getInternalGraphicsElement( localPtr, pIntGraph );

            if( !pIntGraph.isNull() )
            {
              uLevelEntryId = pIntGraph->getLevelEntryId();
            }
            else
            {
              bSkipLevelCheck = true;
            }
          }
          else
          {
            bSkipLevelCheck = true;
          }

          OdDgElementIteratorPtr pCellIter = pCell2d->createIterator();
          OdDgGraphicsElementPtr pFirstChild = getFirstGraphicsElement( pCellIter );

          if( !pFirstChild.isNull() )
            curClass = pFirstChild->getClass();
        }
        else if( src->isKindOf(OdDgCellHeader3d::desc()) )
        {
          OdDgCellHeader3dPtr pCell3d = src;

          OdDgBRepEntityPE::BRepEntityType brepType = OdDgBRepEntityPEPtr(pCell3d)->type(pCell3d);

          if( (brepType != OdDgBRepEntityPE::eNotBRepEntity) || pCell3d->isPolygonWithHoles() )
          {
            OdDgGraphicsElementPtr pIntGraph;
            OdDgElementIteratorPtr lPtr = pCell3d->createIterator();
            getInternalGraphicsElement( lPtr, pIntGraph );

            if( !pIntGraph.isNull() )
            {
              uLevelEntryId = pIntGraph->getLevelEntryId();
            }
            else
            {
              bSkipLevelCheck = true;
            }
          }
          else
          {
            bSkipLevelCheck = true;
          }

          OdDgElementIteratorPtr pCellIter = pCell3d->createIterator();
          OdDgGraphicsElementPtr pFirstChild = getFirstGraphicsElement( pCellIter );

          if( !pFirstChild.isNull() )
            curClass = pFirstChild->getClass();
        }
        else if( src->isKindOf(OdDgSolid::desc()) )
        {
          OdDgSolidPtr pSolid = src;

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
            else
            {
              bSkipLevelCheck = true;
            }
          }
          catch(...)
          {
          	bSkipLevelCheck = true;
          }
        }
        else if( src->isKindOf(OdDgSurface::desc()) )
        {
          OdDgSurfacePtr pSurface = src;

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
            else
            {
              bSkipLevelCheck = true;
            }
          }
          catch(...)
          {
            bSkipLevelCheck = true;
          }
        }
      }

      if( !bSkipLevelCheck )
      {
        bInvisible = !OdDgnImportContext::checkLevelMask( uLevelEntryId );
        dst->setVisibility( bInvisible ? OdDb::kInvisible : OdDb::kVisible );
      }
    }

    OdDgViewPtr pActiveView = OdDgnImportContext::getActiveView();

    if( !bInvisible && !pActiveView.isNull() )
    {
      if( !pActiveView->getShowConstructionFlag() && (curClass == OdDgGraphicsElement::kClassConstruction) )
      {
        bInvisible = true;
      }

      if( !pActiveView->getShowDimensionsFlag() && (src->getElementType() == OdDgElement::kTypeDimension) )
      {
        bInvisible = true;
      }

      if( !pActiveView->getShowTagsFlag() && (src->getElementType() == OdDgElement::kTypeTagElement) )
      {
        bInvisible = true;
      }

      if( pActiveView->getHideTextsFlag() && 
        ((src->getElementType() == OdDgElement::kTypeText) || (src->getElementType() == OdDgElement::kTypeTextNode))
        )
      {
        bInvisible = true;
      }

      dst->setVisibility( bInvisible ? OdDb::kInvisible : OdDb::kVisible );
    }
  }
}
//----------------------------------------------------------------------------------------------------

void applyLevelSymbologyOverrides( const OdDgElementId& idLevel, 
                                     OdUInt32& uColorIndex, 
                                       OdUInt32& uLineWeight, 
                                          OdInt32& iLineStyleId 
                                  )
{
  if( idLevel.isNull() )
  {
    return;
  }

  OdDgLevelTableRecordPtr pLevel = idLevel.openObject(OdDg::kForRead);

  if( pLevel.isNull() )
  {
    return;
  }

  if( pLevel->getUseOverrideColorFlag() )
  {
    uColorIndex = pLevel->getOverrideColorIndex();
  }

  if( pLevel->getUseOverrideLineWeightFlag() )
  {
    uLineWeight = pLevel->getOverrideLineWeight();
  }

  if( pLevel->getUseOverrideLineStyleFlag() )
  {
    iLineStyleId = (OdInt32)pLevel->getOverrideLineStyleEntryId();
  }
}

//----------------------------------------------------------------------------------------------------

OdDbObjectId getDwgMaterialByDgnMaterial( const OdDgElementId& idDgnMaterial, 
                                            OdDbDatabase* pDwgDb 
                                        )
{
  OdDbObjectId retId = OdDgnImportContext::getObjectId(idDgnMaterial);

  if( retId.isNull() && pDwgDb && !idDgnMaterial.isNull() )
  {
    OdDgElementPtr pElmMat = idDgnMaterial.openObject(OdDg::kForRead);

    if( !pElmMat->isKindOf( OdDgMaterialTableRecord::desc() ) )
    {
      return retId;
    }

    OdDgMaterialTableRecordPtr pDgnMaterial = pElmMat;

    OdDbObjectId idMatDictionary = pDwgDb->getMaterialDictionaryId();

    if( idMatDictionary.isNull() )
    {
      return retId;
    }

    OdDbDictionaryPtr pDbMatDictionary = idMatDictionary.openObject(OdDb::kForWrite);

    OdDbMaterialPtr pDbMaterial = OdDbMaterial::createObject();

    if( !pDgnMaterial.isNull() )
    {
      if( DgnImporter::copyMaterialProperties( pDgnMaterial, pDbMaterial, pDbMatDictionary ) )
      {
        pDbMatDictionary->setAt( pDbMaterial->name(), pDbMaterial );

        OdDgnImportPathToDwgObject dwgPath;
        dwgPath.m_bExists = true;
        dwgPath.m_idPath.objectIds().push_back(pDbMaterial->objectId());

        DgnImporter* pImporter = OdDgnImportContext::getDgnImporter();

        if( pImporter )
        {
          pImporter->addObjectPath( pDgnMaterial->elementId(), dwgPath );
        }

        retId = pDbMaterial->objectId();
      }
    }
  }

  return retId;
}

bool isCorrectXData( const OdResBufPtr& pResBuf, const OdDbObjectPtr& pObj )
{
  bool bRet = true;

  OdResBufPtr curBuf = pResBuf;

  while( !curBuf.isNull() )
  {
    if(curBuf->restype() != 1001 )
      bRet = false;
    else
    {
      OdDbDatabase* pDb = pObj->database();

      OdString strAppName = curBuf->getString();

      if( pDb )
      {
        OdDbObjectId appId = OdDbSymUtil::getRegAppId(strAppName, pDb);

        if( appId.isNull() )
        {
          OdDbRegAppTablePtr pRegAppTable = pDb->getRegAppTableId().openObject(OdDb::kForWrite);

          OdString regAppName = strAppName;

          if( regAppName.isEmpty() )
          {
            regAppName = OD_T("RegApp0");

            OdUInt32 iCount = 1;

            while( pRegAppTable->has(regAppName) )
            {
              regAppName.format(OD_T("RegApp%d"), iCount );
              iCount++;
            }
          }

          OdString repairedName;
          if( OdDbSymUtil::repairSymbolName(repairedName, regAppName, pDb) == eOk && !repairedName.isEmpty() )
            regAppName = repairedName;

          if( !pRegAppTable->has(regAppName) )
          {
            OdDbRegAppTableRecordPtr pRegApp = OdDbRegAppTableRecord::createObject();
            pRegApp->setName(regAppName);
            pRegAppTable->add(pRegApp);
          }
          else
            bRet = false;
        }
      }
      else
        bRet = false;
    }

    if( !bRet )
      break;

    curBuf = curBuf->next();

    while( !curBuf.isNull() && (curBuf->restype() != 1001) )
      curBuf = curBuf->next();
  }

  return bRet;
}

//----------------------------------------------------------------------------------------------------

void copyEntityProperties(OdDgGraphicsElement* src, OdDbEntity* dst)
{
  // color
  OdUInt32 color        = src->getColorIndex();
  OdUInt32 uLineWeight  = src->getLineWeight();
  OdInt32  iLineStyleId = src->getLineStyleEntryId();

  OdDgViewPtr pActiveView = OdDgnImportContext::getActiveView();

  if( !pActiveView.isNull() && pActiveView->getShowLevelSymbologyFlag() )
  {
    applyLevelSymbologyOverrides( src->getLevelId(), color, uLineWeight, iLineStyleId );
  }

  setDwgColorByDgnIndex( src->database(), dst, color );
  
  // layer
  OdDbObjectId layerId = OdDgnImportContext::getObjectId(src->getLevelId());

  if( layerId.isNull() || OdDbLayerTableRecord::cast(layerId.openObject()).isNull() ) // is used in audit checking also
  {
    layerId = dst->database()->getLayerZeroId();
  }

  dst->setLayer(layerId);

  // transparency
  OdCmTransparency trans((OdUInt8)255);
  OdRxObjectPtrArray pLinkages;
  src->getLinkages(OdDgAttributeLinkage::kFillStyle, pLinkages);
  for(unsigned int iLinkage = 0 ; iLinkage < pLinkages.size(); iLinkage++ )
  {
    OdDgTransparencyLinkagePtr pTransparencyLinkage = OdDgTransparencyLinkage::cast( pLinkages[iLinkage] );
    if( !pTransparencyLinkage.isNull() )
    {
      trans.setAlphaPercent(1.0 - pTransparencyLinkage->getTransparency());
      break;
    }
  }

  if( OdZero(trans.alphaPercent() - 1.0) )
  {
    trans.setMethod(OdCmTransparency::kByLayer);
    dst->setTransparency(trans);
  }
  else
  {
    if( !layerId.isNull() )
    {
      OdDbLayerTableRecordPtr pLayer = layerId.openObject(OdDb::kForRead);

      if( !pLayer.isNull() )
      {
        OdCmTransparency layerTrans = pLayer->transparency();

        trans.setAlphaPercent( trans.alphaPercent() * layerTrans.alphaPercent());
      }
    }
    dst->setTransparency(trans);
  }

  setVisibility( src, dst );

  // material
  OdDgElementId idMaterial = odDgnImportGetMaterialId(src, pLinkages);

  if (!idMaterial.isNull())
  {
    OdDbObjectId materialId = getDwgMaterialByDgnMaterial( idMaterial, dst->database() );

    if( !materialId.isNull() )
    {
      dst->setMaterial(materialId);
    }
  }

  // linetype scale
  src->getLinkages(OdDgAttributeLinkage::kLStyleMod, pLinkages);
  if( !pLinkages.isEmpty() )
  {
    OdDgLineStyleModificationLinkagePtr pLStyleModLinkage = OdDgLineStyleModificationLinkage::cast( pLinkages[0] );
    if( !pLStyleModLinkage.isNull() )
    {
      if( pLStyleModLinkage->getUseLineStyleScaleFlag() )
      {
        dst->setLinetypeScale(pLStyleModLinkage->getLineStyleScale());
      }
      // linestyle modifiers
      OdGiDgLinetypeModifiers lsMods; double lsScale = 1.0;
      if (odDgnImportLineStyleModifiers(pLStyleModLinkage, src->ownerId(), lsMods, lsScale))
        odDgnImportSetLSModXData(dst, lsMods, lsScale);
    }
  }
  double globalLtpScale = globalLinetypeScale(src);
  if (OdNonZero(globalLtpScale))
    dst->setLinetypeScale(dst->linetypeScale() * globalLtpScale);

  // lineweight
  dst->setLineWeight(odDgnImportLineweightMapping(uLineWeight));

  // linetype
  if( iLineStyleId < 8 && iLineStyleId >= 0)
  {
    // internal
    if( iLineStyleId == 0 )
      dst->setLinetype(dst->database()->getLinetypeContinuousId());
    else
      dst->setLinetype(OdString().format(OD_T("DGN%d"), iLineStyleId));
  }
  else if( iLineStyleId == OdDg::kLineStyleByLevel )
    dst->setLinetype(dst->database()->getLinetypeByLayerId());
  else if( iLineStyleId == OdDg::kLineStyleByCell )
    dst->setLinetype(dst->database()->getLinetypeByBlockId());
  else
  {
    OdDbHandle hLineStyleHandle = 0;

    OdDbObjectId ltpId;

    if( src->database() )
    {
      OdDgElementId idDgLineStyle = src->database()->getLineStyleTable( OdDg::kForRead )->getAt( iLineStyleId );

      if( !idDgLineStyle.isNull() )
      {
        ltpId = OdDgnImportContext::getObjectId( idDgLineStyle );
      }
    }

    if (ltpId.isNull())
      ltpId = dst->database()->getLinetypeContinuousId();
    dst->setLinetype(ltpId);
  }

  // x-data convert.
  OdResBufPtr pResBuf = src->getXDataLinkage();

  if( !pResBuf.isNull() && isCorrectXData( pResBuf, dst) )
  {
    dst->setXData( pResBuf.get() );
  }
}

//--------------------------------------------------------------------------------------------

OdUInt32 codePageToCharset( OdUInt32 uCodePage )
{
  switch( uCodePage )
  {
    case CP_ANSI_1252: /* SYMBOL_CHARSET      */ return 0x02;
    case CP_ANSI_932:  /* SHIFTJIS_CHARSET    */ return 0x80;
    case CP_ANSI_949:  /* HANGEUL_CHARSET     */ return 0x81;
    case CP_ANSI_936:  /* GB2312_CHARSET      */ return 0x86;
    case CP_ANSI_950:  /* CHINESEBIG5_CHARSET */ return 0x88;
    case CP_ANSI_1253: /* GREEK_CHARSET       */ return 0xA1;
    case CP_ANSI_1254: /* TURKISH_CHARSET     */ return 0xA2;
    case CP_ANSI_1255: /* HEBREW_CHARSET      */ return 0xB1;
    case CP_ANSI_1256: /* ARABIC_CHARSET      */ return 0xB2;
    case CP_ANSI_1257: /* BALTIC_CHARSET      */ return 0xBA;
    case CP_ANSI_1251: /* RUSSIAN_CHARSET     */ return 0xCC;
    case CP_ANSI_874:  /* THAI_CHARSET        */ return 0xDE;
    case CP_ANSI_1250: /* EASTEUROPE_CHARSET  */ return 0xEE;
  }

  return 0xEE;
}

//-----------------------------------------------------------------------------------------------------

bool createVertexPolyline( OdGePoint3dArray arrVertices, OdDbPolylinePtr& pPolyline, double dThickness )
{
  bool bRet = false;

  OdGeVector3d vNormal;

  if( geCalculateNormal( arrVertices, &vNormal) == OdGe::kOk )
  {
    pPolyline = OdDbPolyline::createObject();

    OdGeMatrix3d matrix;
    if (vNormal.isParallelTo(OdGeVector3d::kZAxis))
    {
      vNormal = OdGeVector3d::kZAxis;
    }
    else
    {
      matrix.setToWorldToPlane(vNormal);
    }

    pPolyline->setThickness( dThickness );
    pPolyline->setNormal(vNormal);

    OdGePoint3d pnt = matrix*arrVertices[0];
    pPolyline->setElevation(pnt.z);

    matrix.setTranslation(OdGeVector3d(0., 0., -pnt.z));

    for( OdUInt32 f = 0; f < arrVertices.size(); ++f)
    {
      OdGePoint3d ptPos(matrix * arrVertices[f]);
      ODA_ASSERT_ONCE(OdZero(ptPos.z / 2., 1E-9));
      pPolyline->addVertexAt(f, ptPos.convert2d());
    }

    pPolyline->setClosed(true);

    bRet = true;
  }

  return bRet;
}

//------------------------------------------------------------------------------------------------------

OdUInt32 getLineStyleEntryId( const OdDgGraphicsElementPtr& pElm )
{
  OdUInt32 uRet = pElm->getLineStyleEntryId();

  if( uRet == OdDg::kLineStyleByLevel )
  {
    OdDgElementId idLevel = pElm->getLevelId();

    if( !idLevel.isNull() )
    {
      OdDgLevelTableRecordPtr pLevel = idLevel.openObject(OdDg::kForRead);

      if( !pLevel.isNull() )
      {
        uRet = pLevel->getElementLineStyleEntryId();
      }
    }
  }

  return uRet;
}

//---------------------------------------------------------------------------------------------------

void getInternalGraphicsElement( OdDgElementIteratorPtr& pIter, OdDgGraphicsElementPtr& pInternalGraphics )
{
  if( pIter.isNull() )
  {
    return;
  }

  for(; !pIter->done(); pIter->step() )
  {
    OdDgElementPtr pNewCell = pIter->item().openObject( OdDg::kForRead );

    if( pNewCell->isKindOf(OdDgCellHeader3d::desc()) )
    {
      OdDgCellHeader3dPtr pCell3d = pNewCell;
      OdDgElementIteratorPtr pNewIter = pCell3d->createIterator();

      getInternalGraphicsElement( pNewIter, pInternalGraphics );

      if( !pInternalGraphics.isNull() )
      {
        break;
      }
    }
    else if( pNewCell->isKindOf(OdDgCellHeader2d::desc()) )
    {
      OdDgCellHeader2dPtr pCell2d = pNewCell;
      OdDgElementIteratorPtr pNewIter = pCell2d->createIterator();

      getInternalGraphicsElement( pNewIter, pInternalGraphics );

      if( !pInternalGraphics.isNull() )
      {
        break;
      }
    }
    else if( pNewCell->isKindOf(OdDgGraphicsElement::desc()) )
    {
      pInternalGraphics = pNewCell;
      break;
    }
  }
}

//---------------------------------------------------------------------------------------------------

struct OdDgComplexStringItemDesc
{
  bool             m_bLineString;
  OdDgElementId    m_idDgnElm;
  OdGeVector3d     m_vrBulgeZ;
  double           m_dBulge;
  OdGePoint3d      m_ptArc;
  OdGePoint3dArray m_arrPts;
};

//---------------------------------------------------------------------------------------------------

bool getComplexStringItemDesc( const OdDgGraphicsElementPtr& pElm, OdDgComplexStringItemDesc& curItemDesc, double dDeviation )
{
  bool bRet = false;

  curItemDesc.m_bLineString = false;

  switch( pElm->getElementType() )
  {
    case OdDgElement::kTypeLine:
    {
      OdDgLine2dPtr pLine2d = OdDgLine2d::cast( pElm );
      OdDgLine3dPtr pLine3d = OdDgLine3d::cast( pElm );

      if( !pLine2d.isNull() )
      {
        OdGePoint2d ptStart = pLine2d->getStartPoint();
        OdGePoint2d ptEnd   = pLine2d->getEndPoint();
        curItemDesc.m_arrPts.push_back( OdGePoint3d(ptStart.x, ptStart.y, 0) );
        curItemDesc.m_arrPts.push_back( OdGePoint3d(ptEnd.x, ptEnd.y, 0) );
        curItemDesc.m_dBulge = 0;
        curItemDesc.m_idDgnElm = pLine2d->elementId();

        bRet = true;
      }
      else if( !pLine3d.isNull() )
      {
        curItemDesc.m_arrPts.push_back( pLine3d->getStartPoint() );
        curItemDesc.m_arrPts.push_back( pLine3d->getEndPoint() );
        curItemDesc.m_dBulge = 0;
        curItemDesc.m_idDgnElm = pLine3d->elementId();

        bRet = true;
      }
    } break;

    case OdDgElement::kTypeLineString:
    {
      OdDgLineString2dPtr pLineString2d = OdDgLineString2d::cast( pElm );
      OdDgLineString3dPtr pLineString3d = OdDgLineString3d::cast( pElm );

      if( !pLineString2d.isNull() )
      {
        for( OdUInt32 k = 0; k < pLineString2d->getVerticesCount(); k++ )
        {
          OdGePoint2d ptCur = pLineString2d->getVertexAt(k);

          curItemDesc.m_arrPts.push_back( OdGePoint3d(ptCur.x, ptCur.y,0) );
        }

        curItemDesc.m_dBulge = 0;
        curItemDesc.m_idDgnElm = pLineString2d->elementId();
      }
      else if( !pLineString3d.isNull() )
      {
        for( OdUInt32 k = 0; k < pLineString3d->getVerticesCount(); k++ )
        {
          curItemDesc.m_arrPts.push_back( pLineString3d->getVertexAt(k) );
        }

        curItemDesc.m_dBulge = 0;
        curItemDesc.m_idDgnElm = pLineString3d->elementId();
      }

      curItemDesc.m_bLineString = true;
    } break;

    case OdDgElement::kTypeShape:
    {
      OdDgShape2dPtr pShape2d = OdDgShape2d::cast( pElm );
      OdDgShape3dPtr pShape3d = OdDgShape3d::cast( pElm );

      if( !pShape2d.isNull() )
      {
        for( OdUInt32 k = 0; k < pShape2d->getVerticesCount(); k++ )
        {
          OdGePoint2d ptCur = pShape2d->getVertexAt(k);

          curItemDesc.m_arrPts.push_back( OdGePoint3d(ptCur.x, ptCur.y,0) );
        }

        curItemDesc.m_dBulge = 0;
        curItemDesc.m_idDgnElm = pShape2d->elementId();
      }
      else if( !pShape3d.isNull() )
      {
        for( OdUInt32 k = 0; k < pShape3d->getVerticesCount(); k++ )
        {
          curItemDesc.m_arrPts.push_back( pShape3d->getVertexAt(k) );
        }

        curItemDesc.m_dBulge = 0;
        curItemDesc.m_idDgnElm = pShape3d->elementId();
      }
    } break;

    case OdDgElement::kTypePointString:
    {
      OdDgPointString2dPtr pString2d = OdDgPointString2d::cast( pElm );
      OdDgPointString3dPtr pString3d = OdDgPointString3d::cast( pElm );

      if( !pString2d.isNull() )
      {
        for( OdUInt32 k = 0; k < pString2d->getVerticesCount(); k++ )
        {
          OdGePoint2d ptCur = pString2d->getVertexAt(k);

          curItemDesc.m_arrPts.push_back( OdGePoint3d(ptCur.x, ptCur.y,0) );
        }

        curItemDesc.m_dBulge = 0;
        curItemDesc.m_idDgnElm = pString2d->elementId();
      }
      else if( !pString3d.isNull() )
      {
        for( OdUInt32 k = 0; k < pString3d->getVerticesCount(); k++ )
        {
          curItemDesc.m_arrPts.push_back( pString3d->getVertexAt(k) );
        }

        curItemDesc.m_dBulge = 0;
        curItemDesc.m_idDgnElm = pString3d->elementId();
      }
    } break;

    case OdDgElement::kTypeEllipse:
    {
      OdDgEllipse2dPtr pEllipse2d = OdDgEllipse2d::cast( pElm );
      OdDgEllipse3dPtr pEllipse3d = OdDgEllipse3d::cast( pElm );

      if( !pEllipse2d.isNull() )
      {
        OdGeEllipArc2d ellipArc = pEllipse2d->getEllipArc();

        OdGeInterval paramInterval;

        ellipArc.getInterval(paramInterval);

        OdGePoint2dArray arrPts;

        ellipArc.getSamplePoints(paramInterval.lowerBound(),paramInterval.upperBound(), dDeviation, arrPts );

        for( OdUInt32 n = 0; n < arrPts.size(); n++ )
        {
          curItemDesc.m_arrPts.push_back(OdGePoint3d( arrPts[n].x, arrPts[n].y, 0));
        }

        curItemDesc.m_dBulge = 0;
        curItemDesc.m_idDgnElm = pEllipse2d->elementId();
      }
      else if( !pEllipse3d.isNull() )
      {
        OdGeEllipArc3d ellipArc = pEllipse3d->getEllipArc();

        OdGeInterval paramInterval;

        ellipArc.getInterval(paramInterval);
        ellipArc.getSamplePoints(paramInterval.lowerBound(),paramInterval.upperBound(), dDeviation, curItemDesc.m_arrPts );

        curItemDesc.m_dBulge = 0;
        curItemDesc.m_idDgnElm = pEllipse3d->elementId();
      }
    } break; 

    case OdDgElement::kTypeBSplineCurve:
    {
      OdDgBSplineCurve2dPtr pCurve2d = OdDgBSplineCurve2d::cast( pElm );
      OdDgBSplineCurve3dPtr pCurve3d = OdDgBSplineCurve3d::cast( pElm );

      OdGeNurbCurve3d curve;

      if( !pCurve2d.isNull() )
      {
        pCurve2d->generateGeCurve( curve );
      }
      else if( !pCurve3d.isNull() )
      {
        pCurve3d->generateGeCurve( curve );
      }

      OdGeInterval paramInterval;

      curve.getInterval(paramInterval);
      curve.getSamplePoints(paramInterval.lowerBound(),paramInterval.upperBound(), dDeviation, curItemDesc.m_arrPts );

      curItemDesc.m_dBulge = 0;
      curItemDesc.m_idDgnElm = pElm->elementId();
    } break;

    case OdDgElement::kTypeCurve:
    {
      OdDgCurve2dPtr pCurve2d = OdDgCurve2d::cast( pElm );
      OdDgCurve3dPtr pCurve3d = OdDgCurve3d::cast( pElm );

      OdGeNurbCurve3d curve;
      bool bCurveOk = false;

      if( !pCurve2d.isNull() )
      {
        bCurveOk = pCurve2d->getGeNurbCurve( curve );
      }
      else if( !pCurve3d.isNull() )
      {
        bCurveOk = pCurve3d->getGeNurbCurve( curve );
      }

      if( bCurveOk )
      {
        OdGeInterval paramInterval;

        curve.getInterval(paramInterval);
        curve.getSamplePoints(paramInterval.lowerBound(),paramInterval.upperBound(), dDeviation, curItemDesc.m_arrPts );

        curItemDesc.m_dBulge = 0;
        curItemDesc.m_idDgnElm = pElm->elementId();
      }
      else
      {
        OdRxObjectPtrArray arrPolyline;
        pElm->explodeGeometry(arrPolyline);

        if( arrPolyline.size() > 0 )
        {
          OdDgElementPtr pExplodeElm = arrPolyline[0];

          if( pExplodeElm->getElementType() != OdDgElement::kTypeCurve )
          {
            bRet = getComplexStringItemDesc( pExplodeElm, curItemDesc, dDeviation );
          }

          curItemDesc.m_idDgnElm = pElm->elementId();
        }
      }
    } break;

    case OdDgElement::kTypeArc:
    {
      OdDgArc2dPtr pArc2d = OdDgArc2d::cast( pElm );
      OdDgArc3dPtr pArc3d = OdDgArc3d::cast( pElm );

      if( !pArc2d.isNull() )
      {
        OdGeEllipArc2d arc2d = pArc2d->getEllipArc();

        OdGeInterval paramInterval;

        arc2d.getInterval(paramInterval);

        if( OdZero(pArc2d->getPrimaryAxis() - pArc2d->getSecondaryAxis()) &&  // Circular arc
          !OdZero((fabs(pArc2d->getSweepAngle()) - Oda2PI)) &&
          !OdZero(pArc2d->getSweepAngle())
          )
        {
          OdGePoint2d ptStart = arc2d.startPoint();
          OdGePoint2d ptEnd = arc2d.endPoint();
          OdGePoint2d ptArc = arc2d.evalPoint( (paramInterval.upperBound() + paramInterval.lowerBound())/2.0);

          curItemDesc.m_arrPts.push_back(OdGePoint3d( ptStart.x, ptStart.y, 0));
          curItemDesc.m_arrPts.push_back(OdGePoint3d( ptEnd.x, ptEnd.y, 0));
          curItemDesc.m_ptArc = OdGePoint3d( ptArc.x, ptArc.y, 0);

          curItemDesc.m_dBulge = tan( pArc2d->getSweepAngle() / 4.0 );
          curItemDesc.m_idDgnElm = pArc2d->elementId();
        }
        else
        {
          OdGePoint2dArray arrPts;

          arc2d.getSamplePoints(paramInterval.lowerBound(),paramInterval.upperBound(), dDeviation, arrPts );

          for( OdUInt32 n = 0; n < arrPts.size(); n++ )
          {
            curItemDesc.m_arrPts.push_back(OdGePoint3d( arrPts[n].x, arrPts[n].y, 0));
          }

          curItemDesc.m_dBulge = 0;
          curItemDesc.m_idDgnElm = pArc2d->elementId();
        }
      }
      else if( !pArc3d.isNull() )
      {
        OdGeEllipArc3d arc3d = pArc3d->getEllipArc();

        OdGeInterval paramInterval;

        arc3d.getInterval(paramInterval);

        if( OdZero(pArc3d->getPrimaryAxis() - pArc3d->getSecondaryAxis()) && // Circular arc
          !OdZero((pArc3d->getSweepAngle() - Oda2PI)) &&
          !OdZero(pArc3d->getSweepAngle())
          )
        {
          OdGePoint3d ptStart = arc3d.startPoint();
          OdGePoint3d ptEnd = arc3d.endPoint();

          curItemDesc.m_arrPts.push_back(ptStart);
          curItemDesc.m_arrPts.push_back(ptEnd);
          curItemDesc.m_ptArc = arc3d.evalPoint( (paramInterval.upperBound() + paramInterval.lowerBound())/2.0);

          curItemDesc.m_dBulge   = tan( (arc3d.endAng() - arc3d.startAng()) / 4.0 );
          curItemDesc.m_vrBulgeZ = arc3d.normal();
          curItemDesc.m_idDgnElm = pArc3d->elementId();
        }
        else
        {
          arc3d.getSamplePoints(paramInterval.lowerBound(),paramInterval.upperBound(), dDeviation, curItemDesc.m_arrPts );

          curItemDesc.m_dBulge = 0;
          curItemDesc.m_idDgnElm = pArc3d->elementId();
        }
      }
    } break;
  }

  if( curItemDesc.m_arrPts.size() > 1 )
  {
    bRet = true;
  }

  return bRet;
}

//---------------------------------------------------------------------------------------------------

void addElmPoints( OdDgComplexStringItemDesc& itemDesc, 
                  OdDbPolylinePtr& pPolyline, 
                  OdGePoint3d& ptLast,
                  const OdGeVector3d& vrNormal
                  )
{
  if( pPolyline->numVerts() )
  {
    if( OdZero(itemDesc.m_arrPts[0].distanceTo(ptLast)) )
    {
      pPolyline->removeVertexAt( pPolyline->numVerts() - 1 );
    }
  }

  OdUInt32 uPtStart = pPolyline->numVerts();

  ptLast = itemDesc.m_arrPts[itemDesc.m_arrPts.size() - 1];

  OdGeMatrix3d matrix;

  matrix.setToWorldToPlane(vrNormal);

  OdGePoint3d pnt = matrix*itemDesc.m_arrPts[0];
  matrix.setTranslation(OdGeVector3d(0., 0., -pnt.z));

  for( OdUInt32 i = 0; i < itemDesc.m_arrPts.size(); i++ )
  {
    OdGePoint3d ptPos(matrix * itemDesc.m_arrPts[i]);
    ODA_ASSERT_ONCE(OdZero(ptPos.z / 2., 1E-9));

    double dBulge = itemDesc.m_dBulge;

    if( !OdZero(dBulge) && (vrNormal.angleTo(itemDesc.m_vrBulgeZ) > OdaPI2) )
      dBulge = - 1.0* dBulge;

    if( i == itemDesc.m_arrPts.size() - 1 )
      dBulge = 0;

    pPolyline->addVertexAt(pPolyline->numVerts(), ptPos.convert2d(), dBulge );
  }

  OdUInt32 uPtEnd = pPolyline->numVerts();

  OdDgnImportPathToDwgObject dwgPath;
  dwgPath.m_idPath.objectIds().push_back( pPolyline->objectId() );
  dwgPath.m_bExists = false;
  dwgPath.m_uStartPt = uPtStart;
  dwgPath.m_uEndPt   = uPtEnd - 1;
  OdDgnImportContext::addObjectPath( itemDesc.m_idDgnElm, dwgPath );
}

//---------------------------------------------------------------------------------------------------

bool calculateNormal( const OdArray<OdDgComplexStringItemDesc>& arrCplxStrItems, OdGeVector3d& vrNormal )
{
  bool bRet = false;

  OdGePoint3dArray arrPts;

  for( OdUInt32 i = 0; i < arrCplxStrItems.size(); i++ )
  {
    if( OdZero(arrCplxStrItems[i].m_dBulge) )
    {
      arrPts.append( arrCplxStrItems[i].m_arrPts );
    }
    else
    {
      arrPts.append( arrCplxStrItems[i].m_arrPts[0] );
      arrPts.append( arrCplxStrItems[i].m_ptArc );
      arrPts.append( arrCplxStrItems[i].m_arrPts[1] );
    }
  }

  if( geCalculateNormal(arrPts, &vrNormal) == OdGe::kOk )
  {
    if( vrNormal.isParallelTo(OdGeVector3d::kZAxis) )
    {
      vrNormal = OdGeVector3d::kZAxis;
    }

    bRet = true;
  }

  return bRet;
}

//---------------------------------------------------------------------------------------------------

bool createPolylineFromComplexElement( OdDgGraphicsElement* e, 
                                         OdDgElementIteratorPtr& pIter, 
                                           OdDbBlockTableRecord* owner,
                                             OdDbPolylinePtr& pPline,
                                               bool bForceCloseElement )
{
  OdDbPolylinePtr pPolyline = OdDbPolyline::createObject();

  OdArray<OdDgComplexStringItemDesc> arrCplxStrItems;

  double dDeviation = 1e-6;

  OdGeExtents3d extComplexString;
  e->getGeomExtents(extComplexString);

  if( extComplexString.isValidExtents() )
  {
    dDeviation = extComplexString.maxPoint().distanceTo( extComplexString.minPoint() ) / 500.0;
  }

  for(; !pIter->done(); pIter->step() )
  {
    OdDgElementPtr pElm = pIter->item().openObject(OdDg::kForRead);

    if( pElm->isKindOf(OdDgGraphicsElement::desc()) )
    {
      OdDgComplexStringItemDesc curItemDesc;

      if( getComplexStringItemDesc( pElm, curItemDesc, dDeviation ) )
      {
        arrCplxStrItems.push_back( curItemDesc );
      }
    }
  }

  // Check if line string breaks present

  bool bSplitPolyline = false;

  for( OdUInt32 k = 0; k < arrCplxStrItems.size(); k++ )
  {
    if( arrCplxStrItems[k].m_bLineString )
    {
      for( OdUInt32 n = 0; n < arrCplxStrItems[k].m_arrPts.size(); n++ )
      {
        if( arrCplxStrItems[k].m_arrPts[n].x > 1e100 || 
            arrCplxStrItems[k].m_arrPts[n].y > 1e100 ||
            arrCplxStrItems[k].m_arrPts[n].z > 1e100
          )
        {
          if( bForceCloseElement )
          {
            arrCplxStrItems[k].m_arrPts.removeAt(n);
            n--;
          }
          else
          {
            bSplitPolyline = true;
            break;
          }
        }
      }

      if( bSplitPolyline )
      {
        break;
      }
    }
  }

  if( bSplitPolyline )
  {
    return false; // import complex string through draw.
  }

  OdGePoint3d ptLast;

  OdGeVector3d vrNormal = OdGeVector3d::kZAxis;

  if( !calculateNormal(arrCplxStrItems, vrNormal ) )
  {
    return false;
  }

  pPolyline->setNormal( vrNormal );
  pPolyline->setThickness( e->getThickness() );

  OdGeMatrix3d matrix;
  matrix.setToWorldToPlane(vrNormal);

  OdGePoint3d pnt = matrix*arrCplxStrItems[0].m_arrPts[0];
  pPolyline->setElevation( pnt.z );

  owner->appendOdDbEntity( pPolyline );

  for( OdUInt32 i = 0; i < arrCplxStrItems.size(); i++ )
  {
    addElmPoints( arrCplxStrItems[i], pPolyline, ptLast, vrNormal );
  }

  OdGePoint3d ptPlineFirst = arrCplxStrItems[0].m_arrPts[0];
  OdGePoint3d ptPlineLast  = arrCplxStrItems[arrCplxStrItems.size()-1].m_arrPts[arrCplxStrItems[arrCplxStrItems.size()-1].m_arrPts.size() - 1];

  if( OdZero(ptPlineFirst.distanceTo(ptPlineLast)) )
  {
    pPolyline->removeVertexAt( pPolyline->numVerts() - 1 );
    pPolyline->setClosed( true );
  }

  if( bForceCloseElement )
  {
    pPolyline->setClosed( true );
  }

  copyEntityProperties( e , pPolyline );

  pPline = pPolyline;

  return true;
}

//-------------------------------------------------------------------------------------------------------

bool prepareMatrixToBlockTransform( OdGeMatrix3d& matTransform, bool bUniScaleTransform )
{
  if( bUniScaleTransform && !matTransform.isUniScaledOrtho() )
  {
    OdGeVector3d vrXAxis, vrYAxis, vrZAxis;
    OdGePoint3d  ptOrigin;

    matTransform.getCoordSystem( ptOrigin, vrXAxis, vrYAxis, vrZAxis );

    double dScaleX = vrXAxis.length();

    if( vrXAxis.isZeroLength() )
      return false;

    if( vrYAxis.isZeroLength() )
      return false;

    vrXAxis.normalize();
    vrYAxis.normalize();
    vrZAxis = vrXAxis.crossProduct(vrYAxis);

    if( vrZAxis.isZeroLength() )
      return false;

    vrZAxis.normalize();
    vrYAxis = vrZAxis.crossProduct(vrXAxis);

    vrXAxis = vrXAxis * dScaleX;
    vrYAxis = vrYAxis * dScaleX;
    vrZAxis = vrZAxis * dScaleX;

    matTransform.setCoordSystem( ptOrigin, vrXAxis, vrYAxis, vrZAxis );
  }

  if( !matTransform.isScaledOrtho() )
  {
    OdGeVector3d vrXAxis, vrYAxis, vrZAxis;
    OdGePoint3d  ptOrigin;

    matTransform.getCoordSystem( ptOrigin, vrXAxis, vrYAxis, vrZAxis );

    double dScaleX = vrXAxis.length();
    double dScaleY = vrYAxis.length();
    double dScaleZ = vrZAxis.length();

    if( OdZero(dScaleX) )
      return false;

    if( OdZero(dScaleY) )
      return false;

    vrXAxis.normalize();
    vrYAxis.normalize();
    vrZAxis = vrXAxis.crossProduct(vrYAxis);
    vrZAxis.normalize();

    if( vrZAxis.isZeroLength() )
      return false;

    vrYAxis = vrZAxis.crossProduct(vrXAxis);

    vrXAxis = vrXAxis * dScaleX;
    vrYAxis = vrYAxis * dScaleY;
    vrZAxis = vrZAxis * dScaleZ;

    matTransform.setCoordSystem( ptOrigin, vrXAxis, vrYAxis, vrZAxis );
  }

  return true;
}

//-------------------------------------------------------------------------------------------------------

OdString convertEmptyDataFieldsTo_( const OdString& strTextData, const OdDgTextEditField& textField )
{
  OdString strRetVal = strTextData;

  OdString strFieldValue = strTextData.mid(textField.m_uStartPosition - 1, textField.m_uLength);

  bool bUpdateField = false;

  OdString strNewFieldValue;

  for( OdInt32 j = 0; j < strFieldValue.getLength(); j++ )
  {
    if( strFieldValue.getAt(j) == L' ' )
    {
      bUpdateField = true;
      strNewFieldValue += L"_";
    }
    else
    {
      strNewFieldValue += strFieldValue.getAt(j);
    }
  }

  if( bUpdateField )
  {
    OdString strLeft;
    OdString strRight;

    if( textField.m_uStartPosition > 0 )
    {
      strLeft = strTextData.left(textField.m_uStartPosition - 1);
    }

    if( textField.m_uStartPosition + textField.m_uLength <= strTextData.getLength() )
    {
      strRight = strTextData.right(strTextData.getLength() - textField.m_uStartPosition - textField.m_uLength + 1);
    }

    strRetVal = strLeft + strNewFieldValue + strRight;                  
  }

  return strRetVal;
}

}
