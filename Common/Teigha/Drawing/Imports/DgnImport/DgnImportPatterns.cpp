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
#include <DbBlockReference.h>
#include <DbBlockTable.h>
#include <DgEllipse.h>
#include <DgBSplineCurve.h>
#include <XRefMan.h>
#include <ExHostAppServices.h>
#include "DbSymUtl.h"
#include "DgShape.h"
#include <Gi/GiUtils.h>
#include "DgGiContext.h"
#include "DgnImportCommon.h"
#include "DgnImportContext.h"
#include "DgnImportPatterns.h"
#include <DbLinetypeTable.h>
#include <DgnLS/DbLSXData.h>

#include <DbCircle.h>
#include <DbRegion.h>
#include <DbEllipse.h>
#include <DbArc.h>

#include "Ge/GeCircArc3d.h"
#include "Ge/GeNurbCurve3d.h"
#include "Ge/GeEllipArc3d.h"
#include "OdRound.h"

using namespace TD_DGN_IMPORT;

namespace TD_DGN_IMPORT {

//-----------------------------------------------------------------------------------------
//                                Import hatch and gradient
//-----------------------------------------------------------------------------------------

extern void importSymbolPatternOnly(OdDgElement* e, OdDbBlockTableRecord* owner);

void importGradient( OdDbHatchPtr& pHatch,
                       OdDgGradientFillLinkagePtr& pGradientLinkage
                   )
{
  pHatch->setHatchObjectType( OdDbHatch::kGradientObject );
  pHatch->setGradientAngle( pGradientLinkage->getAngle() );
  pHatch->setGradientOneColorMode( pGradientLinkage->getKeyCount() < 2 );

  switch( pGradientLinkage->getGradientType() )
  {
    case OdDgGradientFill::kLinear:
    {
      if( pGradientLinkage->getInvertFlag() )
      {
        pHatch->setGradient( OdDbHatch::kPreDefinedGradient, L"INVLINEAR" );
      }
      else
      {
        pHatch->setGradient( OdDbHatch::kPreDefinedGradient, L"LINEAR" );
      }
    } break;

    case OdDgGradientFill::kCurved:
    {
      if( pGradientLinkage->getInvertFlag() )
      {
        pHatch->setGradient( OdDbHatch::kPreDefinedGradient, L"INVCURVED" );
      }
      else
      {
        pHatch->setGradient( OdDbHatch::kPreDefinedGradient, L"CURVED" );
      }
    } break;

    case OdDgGradientFill::kCylindrical:
    {
      if( pGradientLinkage->getInvertFlag() )
      {
        pHatch->setGradient( OdDbHatch::kPreDefinedGradient, L"INVCYLINDER" );
      }
      else
      {
        pHatch->setGradient( OdDbHatch::kPreDefinedGradient, L"CYLINDER" );
      }
    } break;

    case OdDgGradientFill::kSpherical:
    {
      if( pGradientLinkage->getInvertFlag() )
      {
        pHatch->setGradient( OdDbHatch::kPreDefinedGradient, L"INVSPHERICAL" );
      }
      else
      {
        pHatch->setGradient( OdDbHatch::kPreDefinedGradient, L"SPHERICAL" );
      }
    } break;

    case OdDgGradientFill::kHemispherical:
    {
      if( pGradientLinkage->getInvertFlag() )
      {
        pHatch->setGradient( OdDbHatch::kPreDefinedGradient, L"INVHEMISPHERICAL" );
      }
      else
      {
        pHatch->setGradient( OdDbHatch::kPreDefinedGradient, L"HEMISPHERICAL" );
      }
    } break;
  }

  OdArray<double> arrValues;
  OdArray<OdCmColor> arrColors;

//   for( OdUInt32 i = 0; i < pGradientLinkage->getKeyCount(); i++ )
//   {
//     OdDgGradientFill::OdDgGradientKey curKey = pGradientLinkage->getKey(i);
// 
//     arrValues.push_back( curKey.dKeyPosition );
// 
//     OdCmColor curColor;
//     curColor.setRGB( ODGETRED(curKey.clrKeyColor), ODGETGREEN(curKey.clrKeyColor), ODGETBLUE(curKey.clrKeyColor) );
//     arrColors.push_back( curColor );
//   }

  arrValues.push_back(0.0);
  arrValues.push_back(1.0);

  if( pGradientLinkage->getKeyCount() == 0 )
  {
    OdCmColor curColor = pHatch->color();
    arrColors.push_back( curColor );
    curColor.setRGB( (OdUInt8)(255 * pGradientLinkage->getWhiteIntensity()),
      (OdUInt8)(255 * pGradientLinkage->getWhiteIntensity()),
      (OdUInt8)(255 * pGradientLinkage->getWhiteIntensity())
      );
    arrColors.push_back( curColor );
  }
  else if( pGradientLinkage->getKeyCount() == 1 )
  {
    OdDgGradientFill::OdDgGradientKey gradKey = pGradientLinkage->getKey(0);

    OdCmColor curColor;
    curColor.setRGB( ODGETRED(gradKey.clrKeyColor), ODGETGREEN(gradKey.clrKeyColor), ODGETBLUE(gradKey.clrKeyColor) );
    arrColors.push_back( curColor );
    curColor.setRGB( (OdUInt8)(255 * pGradientLinkage->getWhiteIntensity()),
                     (OdUInt8)(255 * pGradientLinkage->getWhiteIntensity()),
                     (OdUInt8)(255 * pGradientLinkage->getWhiteIntensity())
                   );
    arrColors.push_back( curColor );
  }
  else
  {
    OdDgGradientFill::OdDgGradientKey firstKey = pGradientLinkage->getKey(0);
    OdDgGradientFill::OdDgGradientKey lastKey = pGradientLinkage->getKey(pGradientLinkage->getKeyCount() - 1);

    OdCmColor curColor;
    curColor.setRGB( ODGETRED(firstKey.clrKeyColor), ODGETGREEN(firstKey.clrKeyColor), ODGETBLUE(firstKey.clrKeyColor) );
    arrColors.push_back( curColor );
    curColor.setRGB( ODGETRED(lastKey.clrKeyColor), ODGETGREEN(lastKey.clrKeyColor), ODGETBLUE(lastKey.clrKeyColor) );
    arrColors.push_back( curColor );
  }

  pHatch->setGradientColors(arrValues.size(), arrColors.asArrayPtr(), arrValues.asArrayPtr() );
}

//-----------------------------------------------------------------------------------------

template <class T> void applyDgnPatternSymbologyToHatch( OdDbHatchPtr& pHatch,
                                                           OdDgDatabase* pDb, 
                                                             const T* pPaternLinkage
                                     )
{
  if( pPaternLinkage->getUseLineColorFlag() )
  {
    setDwgColorByDgnIndex( pDb, pHatch, pPaternLinkage->getLineColorIndex() );
  }

  applyDgnPatternLineTypeAndWeightToHatch( pHatch, pDb, pPaternLinkage );
}

template <class T> void applyDgnPatternLineTypeAndWeightToHatch( OdDbHatchPtr& pHatch,
                                                                   OdDgDatabase* pDb, 
                                                                     const T* pPaternLinkage
                                                                )
{
  if( pPaternLinkage->getUseLineWeightFlag() )
  {
    pHatch->setLineWeight(odDgnImportLineweightMapping(pPaternLinkage->getLineWeight()));
  }

  if( pPaternLinkage->getUseLineStyleFlag() )
  {
    if( pPaternLinkage->getLineStyleEntryId() < 8 && pPaternLinkage->getLineStyleEntryId() >= 0)
    {

      if( pPaternLinkage->getLineStyleEntryId() == 0 )
      {
        pHatch->setLinetype(pHatch->database()->getLinetypeContinuousId());
      }
      else
      {
        pHatch->setLinetype(OdString().format(OD_T("DGN%d"), pPaternLinkage->getLineStyleEntryId()));
      }
    }
    else if( pPaternLinkage->getLineStyleEntryId() == OdDg::kLineStyleByLevel )
    {
      pHatch->setLinetype(pHatch->database()->getLinetypeByLayerId());
    }
    else if( pPaternLinkage->getLineStyleEntryId() == OdDg::kLineStyleByCell)
    {
      pHatch->setLinetype(pHatch->database()->getLinetypeByBlockId());
    }
    else
    {
      OdDgElementId idLineStyle;

      if( pDb )
      {
        OdDgLineStyleTablePtr pLSTable = pDb->getLineStyleTable( OdDg::kForRead );

        if( !pLSTable.isNull() )
        {
          idLineStyle = pLSTable->getAt( pPaternLinkage->getLineStyleEntryId() );
        }
      }

      OdDbObjectId ltpId = pHatch->database()->getOdDbObjectId(idLineStyle.getHandle());

      if( ltpId.isNull() )
      {
        ltpId = pHatch->database()->getLinetypeContinuousId();
      }

      pHatch->setLinetype(ltpId);
    }
  }
}
//-----------------------------------------------------------------------------------------

void applyDgnSymbolPatternSymbologyToHatch( OdDbHatchPtr& pHatch,
                                              OdDgDatabase* pDb, 
                                                const OdDgSymbolPatternLinkage* pPaternLinkage
                                     )
{
  if( pPaternLinkage->getUseColorFlag() )
  {
    setDwgColorByDgnIndex( pDb, pHatch, pPaternLinkage->getColorIndex() );
  }

  applyDgnPatternLineTypeAndWeightToHatch( pHatch, pDb, pPaternLinkage );
}

//-----------------------------------------------------------------------------------------

OdGePoint3d getHatchSeedPointBase( OdDgElement* pElm )
{
  switch( pElm->getElementType() )
  {
    case OdDgElement::kTypeEllipse :
    {
      if( pElm->isKindOf( OdDgEllipse2d::desc() ) )
      {
        OdDgEllipse2dPtr ellipse = pElm;
        OdGePoint2d origin = ellipse->getOrigin();
        return OdGePoint3d( origin.x, origin.y, 0. );
      }
      else
      {
        OdDgEllipse3dPtr ellipse = pElm;
        OdGePoint3d origin;
        ellipse->getOrigin( origin );
        return origin;
      }
    }
    break;

    case OdDgElement::kTypeShape :
    {
      if( pElm->isKindOf( OdDgShape2d::desc() ) )
      {
        OdDgShape2dPtr shape = pElm;
        OdGePoint2d origin = shape->getVertexAt( 0 );
        return OdGePoint3d( origin.x, origin.y, 0. );
      }
      else
      {
        OdDgShape3dPtr shape = pElm;
        return shape->getVertexAt( 0 );
      }
    }
    break;

    case OdDgElement::kTypeBSplineCurve :
    {
      OdGeExtents3d extents;

      if( pElm->isKindOf(OdDgBSplineCurve2d::desc()) )
      {
        OdDgBSplineCurve2dPtr pCurve2d = pElm;

        OdGePoint3dArray arrCtrlPts;
        OdUInt32 i;

        if( pCurve2d->hasFitData() )
        {
          for( i = 0; i < pCurve2d->numFitPoints(); i++ )
          {
            OdGePoint2d ptFit;

            if( pCurve2d->getFitPointAt(i, ptFit) == eOk )
            {
              arrCtrlPts.push_back( OdGePoint3d(ptFit.x, ptFit.y, 0.0) );
            }
          }
        }
        else
        {
          for( i = 0; i < pCurve2d->numControlPoints(); i++ )
          {
            OdGePoint2d ptCtrl;

            if( pCurve2d->getControlPointAt(i, ptCtrl) == eOk )
            {
              arrCtrlPts.push_back( OdGePoint3d(ptCtrl.x, ptCtrl.y, 0.0) );
            }
          }
        }

        for( i = 0; i < arrCtrlPts.size(); i++ )
        {
          extents.addPoint( arrCtrlPts[i] );
        }
      }
      else
      {
        OdDgBSplineCurve3dPtr pCurve3d = pElm;

        OdUInt32 i;

        if( pCurve3d->hasFitData() )
        {
          for( i = 0; i < pCurve3d->numFitPoints(); i++ )
          {
            OdGePoint3d ptFit;

            if( pCurve3d->getFitPointAt(i, ptFit) == eOk )
            {
              extents.addPoint( ptFit );
            }
          }
        }
        else
        {
          for( i = 0; i < pCurve3d->numControlPoints(); i++ )
          {
            OdGePoint3d ptCtrl;

            if( pCurve3d->getControlPointAt(i, ptCtrl) == eOk )
            {
              extents.addPoint( ptCtrl );
            }
          }
        }
      }

      if( extents.isValidExtents() )
      {
        return extents.minPoint();
      }
    } break;

    case OdDgElement::kTypeComplexShape:
    case OdDgElement::kTypeCellHeader:
    {
      OdGeExtents3d extents;
      pElm->getGeomExtents(extents);

      if( extents.isValidExtents() )
      {
        return extents.minPoint();
      }
      else
      {
        OdArray<bool>          arrInvisibleStatus;
        OdArray<OdDgElementId> arrElementId;

        pElm->getGeomExtents(extents);

        if( extents.isValidExtents() )
        {
          return extents.minPoint();
        }
      }
    } break;
  }

  return OdGePoint3d();
}

//-----------------------------------------------------------------------------------------

OdGePoint2d getHatchSeedPoint( OdDgElement* pElm, 
                                 OdDgPatternLinkagePtr pPatternLinkage, 
                                   OdDbHatchPtr& pHatch,
                                     double dUORsToModelScale
                              )
{
  OdGePoint3d ptSeedBase(0,0,0);

  if( !pElm || pPatternLinkage.isNull() || pHatch.isNull() || pHatch->isErased() )
  {
    return ptSeedBase.convert2d();
  }

  if( pPatternLinkage->getUseOffsetFlag() )
  {
    pPatternLinkage->getOffset( ptSeedBase );
    ptSeedBase *= dUORsToModelScale;
  }

  OdGePoint3d ptElementOrigin(0,0,0);

  if( pPatternLinkage->getType() == OdDgPatternLinkage::kDWGPattern )
  {
    ptSeedBase.set(0,0,0);
  }
  else
  {
    ptElementOrigin = getHatchSeedPointBase( pElm );
  }

  if( ptElementOrigin.distanceTo( ptSeedBase ) > 1e12 )
  {
    ptSeedBase = ptElementOrigin;
  }
  else
  {
    ptSeedBase += ptElementOrigin.asVector();
  }

  OdGeMatrix3d matrix;
  matrix.setToWorldToPlane(pHatch->normal());

  ptSeedBase = ptSeedBase.transformBy(matrix);

  return ptSeedBase.convert2d();
}

//-----------------------------------------------------------------------------------------

void importLinearPattern( OdDbHatchPtr& pHatch,
                            OdDgElement* pElm, 
                              const OdDgLinearPatternLinkagePtr& pPaternLinkage,
                                double dUORsToModelScale
                        )
{
  OdDgDatabase* pDb = pElm->database();

  pHatch->setHatchStyle( OdDbHatch::kNormal );
  pHatch->setHatchObjectType( OdDbHatch::kHatchObject );

  OdHatchPattern linearPattern;

  OdHatchPatternLine patternLine;
  patternLine.m_basePoint.set(0.0,0.5);
  patternLine.m_dLineAngle = 0;
  patternLine.m_patternOffset.set(0.0,pPaternLinkage->getSpace()*dUORsToModelScale);
  patternLine.m_dashes.push_back(pPaternLinkage->getSpace()*dUORsToModelScale);

  linearPattern.push_back( patternLine );

  OdGePoint2d ptBase = getHatchSeedPoint( pElm, pPaternLinkage, pHatch, dUORsToModelScale );

  pHatch->setPattern(OdDbHatch::kUserDefined, L"_USER", pPaternLinkage->getAngle(), 1.0, linearPattern, ptBase );

  applyDgnPatternSymbologyToHatch( pHatch, pDb, pPaternLinkage.get() );
}

//-----------------------------------------------------------------------------------------

void importCrossPattern( OdDbHatchPtr& pHatch,
                           OdDgElement* pElm, 
                             const OdDgCrossPatternLinkagePtr& pPaternLinkage,
                               double dUORsToModelScale
                         )
{
  OdDgDatabase* pDb = pElm->database();

  pHatch->setHatchStyle( OdDbHatch::kNormal );
  pHatch->setHatchObjectType( OdDbHatch::kHatchObject );

  OdHatchPattern crossPattern;

  OdHatchPatternLine patternLine;
  patternLine.m_basePoint.set(0.0,0.0);
  patternLine.m_dLineAngle = pPaternLinkage->getAngle1();
  patternLine.m_patternOffset.set(0.0,pPaternLinkage->getSpace1()*dUORsToModelScale);
  patternLine.m_dashes.push_back(pPaternLinkage->getSpace1()*dUORsToModelScale);

  crossPattern.push_back( patternLine );

  patternLine.m_basePoint.set(0.0,0.0);
  patternLine.m_dLineAngle = pPaternLinkage->getAngle2();
  patternLine.m_patternOffset.set(0.0,pPaternLinkage->getSpace2()*dUORsToModelScale);
  patternLine.m_dashes.clear();
  patternLine.m_dashes.push_back(pPaternLinkage->getSpace2()*dUORsToModelScale);
  crossPattern.push_back( patternLine );

  OdGePoint2d ptBase = getHatchSeedPoint( pElm, pPaternLinkage, pHatch, dUORsToModelScale );

  pHatch->setPattern(OdDbHatch::kUserDefined, L"_USER", 0.0, 1.0, crossPattern, ptBase );

  applyDgnPatternSymbologyToHatch( pHatch, pDb, pPaternLinkage.get() );
}

//-----------------------------------------------------------------------------------------

void importSymbolPattern( OdDgElement* pElm, OdDbBlockTableRecord* owner )
{
  OdDbObjectId idBlockTable = owner->database()->getBlockTableId();

  if( idBlockTable.isNull() )
  {
    return;
  }

  OdDbBlockTablePtr pBlockTable = idBlockTable.openObject(OdDb::kForWrite);

  if( !pBlockTable.isNull() )
  {
    OdString strDefNameBase = L"SymbolPattern";

    OdString strDefName = strDefNameBase + L"_1";

    OdUInt32 uCount = 2;

    while( !pBlockTable->getAt( strDefName).isNull() )
    {
      strDefName.format(L"_%d", uCount );
      strDefName = strDefNameBase + strDefName;

      uCount++;
    }

    OdString repairedName;

    if( OdDbSymUtil::repairSymbolName(repairedName, strDefName, owner->database() ) == eOk && !repairedName.isEmpty() )
    {
      strDefNameBase = repairedName;
      strDefName = strDefNameBase;
    }

    OdDbBlockTableRecordPtr pDwgSymbolPattern = OdDbBlockTableRecord::createObject();
    pDwgSymbolPattern->setName( strDefName );

    pBlockTable->add( pDwgSymbolPattern );

    OdGePoint3d ptPatternOrigin = OdGePoint3d::kOrigin;

    OdGeExtents3d extShape;
    pElm->getGeomExtents( extShape );

    if( extShape.isValidExtents() )
    {
      ptPatternOrigin = extShape.center();
    }

    importSymbolPatternOnly( pElm, pDwgSymbolPattern );

    OdGeMatrix3d matTransform = OdGeMatrix3d::translation(-ptPatternOrigin.asVector());

    if( matTransform != OdGeMatrix3d::kIdentity)
    {
      for (OdDbObjectIteratorPtr it = pDwgSymbolPattern->newIterator(); !it->done(); it->step())
      {
        OdDbEntityPtr e = it->objectId().safeOpenObject(OdDb::kForWrite);
        e->transformBy(matTransform);
      }
    }

    OdDbBlockReferencePtr pBlockReference = OdDbBlockReference::createObject();
    pBlockReference->setBlockTableRecord( pDwgSymbolPattern->objectId() );
    owner->appendOdDbEntity( pBlockReference );
    pBlockReference->setBlockTransform( OdGeMatrix3d::kIdentity );
    pBlockReference->setPosition( ptPatternOrigin );

    if( pElm->isKindOf( OdDgGraphicsElement::desc()) )
    {
      OdDgGraphicsElement* pGrElm = dynamic_cast<OdDgGraphicsElement*>(pElm);

      copyEntityProperties( pGrElm, pBlockReference );
    }
  }
}

//=========================================================================================

OdDgnImportSymbolToHatchConverter::OdDgnImportSymbolToHatchConverter()
{
  OdGiBaseVectorizer::m_flags |= (kDrawInvisibleEnts|kDrawLayerOff|kDrawLayerFrozen);
  setContext(this); 
  OdGiGeometrySimplifier::setDrawContext(OdGiBaseVectorizer::drawContext());
  m_pModelToEyeProc->setDrawContext(OdGiBaseVectorizer::drawContext());
  output().setDestGeometry(*this);
}

//-----------------------------------------------------------------------------------------

bool OdDgnImportSymbolToHatchConverter::convertSymbolToHatch( const OdDgSharedCellDefinition* pCell, 
                                                                double dTolerance, 
                                                                  OdUInt32 nMaxLineNumber, 
                                                                    OdUInt32 nMinLineNumber,
                                                                      OdUInt32 nArcSegNum,
                                                                        double dRowSpacing,
                                                                          double dColSpacing,
                                                                            OdHatchPattern& dwgPattern,
                                                                              double& dScaleFactor,
                                                                                OdUInt32& uHatchColorIndex
                                                            )
{
  bool bRet = false;

  OdGeExtents3d extSymbol;
  pCell->getGeomExtents( extSymbol );

  if( extSymbol.isValidExtents() )
  {
    m_dTolerance = dTolerance;
    m_maxLineNumber = nMaxLineNumber;
    m_minLineNumber = nMinLineNumber;
    m_uSircleSegNum = nArcSegNum;

    m_dSymbolWidth  = extSymbol.maxPoint().x - extSymbol.minPoint().x + dRowSpacing;
    m_dSymbolHeight = extSymbol.maxPoint().y - extSymbol.minPoint().y + dColSpacing;

    if( !OdZero( m_dSymbolWidth, 1e-8 ) && !OdZero( m_dSymbolHeight, 1e-8 ) )
    {
      m_dBorderAngle = atan( m_dSymbolHeight / m_dSymbolWidth );

      double dMaxValue = (m_dSymbolWidth > m_dSymbolHeight) ? m_dSymbolWidth : m_dSymbolHeight;

      m_ptOffset = extSymbol.minPoint();

      m_dScaleFactor = dMaxValue;

      m_dSymbolWidth  /= dMaxValue;
      m_dSymbolHeight /= dMaxValue;
      m_ptOffset.x    /= dMaxValue;
      m_ptOffset.y    /= dMaxValue;

      std::map<OdUInt32,OdUInt32> colorMap;

      OdDgElementIteratorPtr pIter = pCell->createIterator();

      for(; !pIter->done(); pIter->step() )
      {
        OdDgElementPtr pItem = pIter->item().openObject(OdDg::kForRead);

        if( pItem->isKindOf( OdDgGraphicsElement::desc()) )
        {
          OdUInt32 uColorIndex = ((OdDgGraphicsElementPtr)(pItem))->getColorIndex();

          std::map<OdUInt32,OdUInt32>::iterator pColorIter = colorMap.find( uColorIndex );

          if( pColorIter != colorMap.end() )
          {
            colorMap[uColorIndex]++;
          }
          else
          {
            colorMap[uColorIndex] = 1;
          }

          draw( pItem );
        }
      }

      if( colorMap.size() > 2 )
      {
        uHatchColorIndex = (OdUInt32)(-2);
      }
      else
      {
        std::map<OdUInt32,OdUInt32>::iterator pColorIter = colorMap.begin();

        uHatchColorIndex = pColorIter->first;
        OdUInt32 uMaxColors = pColorIter->second;

        for(; pColorIter != colorMap.end(); pColorIter++ )
        {
          if( pColorIter->second > uMaxColors )
          {
            uMaxColors = pColorIter->second;
            uHatchColorIndex = pColorIter->first;
          }
        }
      }

      dwgPattern   = m_hatchPattern;
      dScaleFactor = m_dScaleFactor;

      bRet = true;
    }
  }

  return bRet;
}

//-----------------------------------------------------------------------------------------

OdGiRegenType OdDgnImportSymbolToHatchConverter::regenType() const
{
  return kOdGiStandardDisplay;
}

//-----------------------------------------------------------------------------------------

void OdDgnImportSymbolToHatchConverter::createYTopLine( double dCurAngle, OdGePoint2d& ptStart, OdGePoint2d& ptEnd )
{
  OdUInt32 n = 0;
  OdUInt32 m = 0;
  OdUInt32 bestN = 0;

  double   dMErrorMin = 1.0;

  bool bIdexesFound = false;

  double dScaleMN = m_dSymbolWidth * tan(dCurAngle) / m_dSymbolHeight;

  double dNMax = m_maxLineNumber * 1.0 / dScaleMN; 

  OdUInt32 nMax = (OdUInt32)OdRound(dNMax);

  if( nMax < m_minLineNumber )
  {
    nMax = m_minLineNumber;
  }

  for( n = 1; n < nMax; n++ )
  {
    double dMFull = n * dScaleMN;

    double dMError = dMFull - OdRound(dMFull);

    if( fabs(dMError) < m_dTolerance )
    {
      m = (OdUInt32)(OdRound(dMFull));
      bIdexesFound = true;
      break;
    }
    else if( fabs(dMError) < dMErrorMin )
    {
      dMErrorMin = fabs(dMError);
      bestN = n;
    }
  }

  if( !bIdexesFound )
  {
    n = bestN;
    m = (OdUInt32)(OdRound(n * dScaleMN));
  }

  OdGeVector2d vrDirection( n * m_dSymbolWidth, m*m_dSymbolHeight );
  vrDirection.normalize();

  dCurAngle = vrDirection.angle();
  dScaleMN = m_dSymbolWidth * tan(dCurAngle) / m_dSymbolHeight;

  double dDashLength = ptStart.distanceTo(ptEnd);

  OdGePoint2d ptNewEnd = ptStart + vrDirection * dDashLength;

  if( !OdZero( ptNewEnd.distanceTo(ptEnd), 1e-3) )
  {
    m_arrBasePoints.push_back( ptEnd );
    m_arrCorrectedPoints.push_back( ptNewEnd );
  }

  OdHatchPatternLine newLine;
  newLine.m_basePoint = ptStart;
  newLine.m_dLineAngle = dCurAngle;

  OdUInt32 nNext = 1;

  dMErrorMin = 1.0;

  for( OdUInt32 t = 1; t < n; t++ )
  {
    double dMFull = t * dScaleMN;

    double dMError = dMFull - OdRound(dMFull);

    if( (dMError > 0) && (dMError < dMErrorMin) )
    {
      dMErrorMin = dMError;
      nNext = t;
    }
  }

  double dYOffset = m_dSymbolWidth / m * sin( dCurAngle);
  double dXOffset = ( m_dSymbolWidth*nNext - m_dSymbolWidth / m )/cos(dCurAngle) + dYOffset / tan( dCurAngle );

  newLine.m_patternOffset.set( -dXOffset, dYOffset );

  newLine.m_dashes.push_back( dDashLength );

  double dSectorGapLength = m_dSymbolHeight / sin( dCurAngle ) - dDashLength;

  if( !OdZero(dSectorGapLength) && (dSectorGapLength > 0) )
  {
    newLine.m_dashes.push_back( -dSectorGapLength );
  }

  double dSubSectorGap = m_dSymbolHeight * ( m - 1 ) / sin( dCurAngle );

  if( !OdZero(dSubSectorGap) )
  {
    newLine.m_dashes.push_back( -dSubSectorGap );
  }

  m_hatchPattern.push_back( newLine );
}

//-----------------------------------------------------------------------------------------

void OdDgnImportSymbolToHatchConverter::createXTopLine( double dCurAngle, OdGePoint2d& ptStart, OdGePoint2d& ptEnd )
{
  OdUInt32 n = 0;
  OdUInt32 m = 0;
  OdUInt32 bestM = 0;

  double   dNErrorMin = 1.0;

  bool bIdexesFound = false;

  double dScaleMN = m_dSymbolHeight / ( tan(dCurAngle) * m_dSymbolWidth );

  double dMMax = m_maxLineNumber * 1.0 / dScaleMN; 

  OdUInt32 mMax = (OdUInt32)OdRound(dMMax);

  if( mMax < m_minLineNumber )
  {
    mMax = m_minLineNumber;
  }

  for( m = 1; m < mMax; m++ )
  {
    double dNFull = m * dScaleMN;

    double dNError = dNFull - OdRound(dNFull);

    if( fabs(dNError) < m_dTolerance )
    {
      n = (OdUInt32)(OdRound(dNFull));
      bIdexesFound = true;
      break;
    }
    else if( fabs(dNError) < dNErrorMin )
    {
      dNErrorMin = fabs(dNError);
      bestM = m;
    }
  }

  if( !bIdexesFound )
  {
    m = bestM;
    n = (OdUInt32)(OdRound(m * dScaleMN));
  }

  OdGeVector2d vrDirection( n * m_dSymbolWidth, m*m_dSymbolHeight );
  vrDirection.normalize();

  dCurAngle = vrDirection.angle();
  dScaleMN = m_dSymbolHeight / ( tan(dCurAngle) * m_dSymbolWidth );

  double dDashLength = ptStart.distanceTo(ptEnd);

  OdGePoint2d ptNewEnd = ptStart + vrDirection * dDashLength;

  if( !OdZero( ptNewEnd.distanceTo(ptEnd), 1e-3) )
  {
    m_arrBasePoints.push_back( ptEnd );
    m_arrCorrectedPoints.push_back( ptNewEnd );
  }

  OdHatchPatternLine newLine;
  newLine.m_basePoint = ptStart;
  newLine.m_dLineAngle = dCurAngle;

  OdUInt32 mNext = 1;

  dNErrorMin = 1.0;

  for( OdUInt32 t = 1; t < m; t++ )
  {
    double dNFull = t * dScaleMN;

    double dNError = dNFull - OdRound(dNFull);

    if( (dNError > 0) && (dNError < dNErrorMin) )
    {
      dNErrorMin = dNError;
      mNext = t;
    }
  }

  double dYOffset = m_dSymbolHeight / n * cos( dCurAngle);
  double dXOffset = ( m_dSymbolHeight*mNext - m_dSymbolHeight / n )/sin(dCurAngle) + dYOffset * tan( dCurAngle );

  newLine.m_patternOffset.set( dXOffset, dYOffset );

  newLine.m_dashes.push_back( dDashLength );

  double dSectorGapLength = m_dSymbolWidth / cos( dCurAngle ) - dDashLength;

  if( !OdZero(dSectorGapLength) && (dSectorGapLength > 0) )
  {
    newLine.m_dashes.push_back( -dSectorGapLength );
  }

  double dSubSectorGap = m_dSymbolWidth * ( n - 1 ) / cos( dCurAngle );

  if( !OdZero(dSubSectorGap) )
  {
    newLine.m_dashes.push_back( -dSubSectorGap );
  }

  m_hatchPattern.push_back( newLine );
}

//-----------------------------------------------------------------------------------------

void OdDgnImportSymbolToHatchConverter::createYBottomLine( double dCurAngle, OdGePoint2d& ptStart, OdGePoint2d& ptEnd )
{
  OdUInt32 n = 0;
  OdUInt32 m = 0;
  OdUInt32 bestN = 0;

  double   dMErrorMin = 1.0;

  bool bIdexesFound = false;

  double dScaleMN = m_dSymbolWidth * fabs(tan(dCurAngle)) / m_dSymbolHeight;

  double dNMax = m_maxLineNumber * 1.0 / dScaleMN; 

  OdUInt32 nMax = (OdUInt32)OdRound(dNMax);

  if( nMax < m_minLineNumber )
  {
    nMax = m_minLineNumber;
  }

  for( n = 1; n < nMax; n++ )
  {
    double dMFull = n * dScaleMN;

    double dMError = dMFull - OdRound(dMFull);

    if( fabs(dMError) < m_dTolerance )
    {
      m = (OdUInt32)(OdRound(dMFull));
      bIdexesFound = true;
      break;
    }
    else if( fabs(dMError) < dMErrorMin )
    {
      dMErrorMin = fabs(dMError);
      bestN = n;
    }
  }

  if( !bIdexesFound )
  {
    n = bestN;
    m = (OdUInt32)(OdRound(n * dScaleMN));
  }

  OdGeVector2d vrDirection( -1.0 * n * m_dSymbolWidth, m*m_dSymbolHeight );
  vrDirection.normalize();

  dCurAngle = vrDirection.angle();
  dScaleMN = m_dSymbolWidth * fabs(tan(dCurAngle)) / m_dSymbolHeight;

  double dDashLength = ptStart.distanceTo(ptEnd);

  OdGePoint2d ptNewEnd = ptStart + vrDirection * dDashLength;

  if( !OdZero( ptNewEnd.distanceTo(ptEnd), 1e-3) )
  {
    m_arrBasePoints.push_back( ptEnd );
    m_arrCorrectedPoints.push_back( ptNewEnd );
  }

  OdHatchPatternLine newLine;
  newLine.m_basePoint = ptStart;
  newLine.m_dLineAngle = dCurAngle;

  OdUInt32 nNext = 1;

  dMErrorMin = 1.0;

  for( OdUInt32 t = 1; t < n; t++ )
  {
    double dMFull = t * dScaleMN;

    double dMError = dMFull - OdRound(dMFull);

    if( (dMError > 0) && (dMError < dMErrorMin) )
    {
      dMErrorMin = dMError;
      nNext = t;
    }
  }

  double dYOffset = m_dSymbolWidth / m * fabs(sin( dCurAngle));
  double dXOffset = ( m_dSymbolWidth*nNext - m_dSymbolWidth / m )/cos(dCurAngle) + dYOffset / fabs(tan( dCurAngle ));

  newLine.m_patternOffset.set( -dXOffset, dYOffset );

  newLine.m_dashes.push_back( dDashLength );

  double dSectorGapLength = m_dSymbolHeight / fabs(sin( dCurAngle )) - dDashLength;

  if( !OdZero(dSectorGapLength) && (dSectorGapLength > 0) )
  {
    newLine.m_dashes.push_back( -dSectorGapLength );
  }

  double dSubSectorGap = m_dSymbolHeight * ( m - 1 ) / fabs(sin( dCurAngle ));

  if( !OdZero(dSubSectorGap) )
  {
    newLine.m_dashes.push_back( -dSubSectorGap );
  }

  m_hatchPattern.push_back( newLine );
}

//-----------------------------------------------------------------------------------------

void OdDgnImportSymbolToHatchConverter::createXBottomLine( double dCurAngle, OdGePoint2d& ptStart, OdGePoint2d& ptEnd )
{
  OdUInt32 n = 0;
  OdUInt32 m = 0;
  OdUInt32 bestM = 0;

  double   dNErrorMin = 1.0;

  bool bIdexesFound = false;

  double dScaleMN = m_dSymbolHeight / (fabs(tan(dCurAngle)) * m_dSymbolWidth);

  double dMMax = m_maxLineNumber * 1.0 / dScaleMN; 

  OdUInt32 mMax = (OdUInt32)OdRound(dMMax);

  if( mMax < m_minLineNumber )
  {
    mMax = m_minLineNumber;
  }

  for( m = 1; m < mMax; m++ )
  {
    double dNFull = m * dScaleMN;

    double dNError = dNFull - OdRound(dNFull);

    if( fabs(dNError) < m_dTolerance )
    {
      n = (OdUInt32)(OdRound(dNFull));
      bIdexesFound = true;
      break;
    }
    else if( fabs(dNError) < dNErrorMin )
    {
      dNErrorMin = fabs(dNError);
      bestM = m;
    }
  }

  if( !bIdexesFound )
  {
    m = bestM;
    n = (OdUInt32)(OdRound(m * dScaleMN));
  }

  OdGeVector2d vrDirection( n * m_dSymbolWidth, -1.0 * m*m_dSymbolHeight );
  vrDirection.normalize();

  dCurAngle = vrDirection.angle();
  dScaleMN = m_dSymbolHeight / (fabs(tan(dCurAngle)) * m_dSymbolWidth);

  double dDashLength = ptStart.distanceTo(ptEnd);

  OdGePoint2d ptNewEnd = ptStart + vrDirection * dDashLength;

  if( !OdZero( ptNewEnd.distanceTo(ptEnd), 1e-3) )
  {
    m_arrBasePoints.push_back( ptEnd );
    m_arrCorrectedPoints.push_back( ptNewEnd );
  }

  OdHatchPatternLine newLine;
  newLine.m_basePoint = ptStart;
  newLine.m_dLineAngle = dCurAngle;

  OdUInt32 mNext = 1;

  dNErrorMin = 1.0;

  for( OdUInt32 t = 1; t < m; t++ )
  {
    double dNFull = t * dScaleMN;

    double dNError = dNFull - OdRound(dNFull);

    if( (dNError > 0) && (dNError < dNErrorMin) )
    {
      dNErrorMin = dNError;
      mNext = t;
    }
  }

  double dYOffset = m_dSymbolHeight / n * cos( dCurAngle);
  double dXOffset = ( m_dSymbolHeight*mNext - m_dSymbolHeight / n )/fabs(sin(dCurAngle)) + dYOffset * fabs(tan( dCurAngle ));

  newLine.m_patternOffset.set( -dXOffset, dYOffset );

  newLine.m_dashes.push_back( dDashLength );

  double dSectorGapLength = m_dSymbolWidth / cos( dCurAngle ) - dDashLength;

  if( !OdZero(dSectorGapLength) && (dSectorGapLength > 0) )
  {
    newLine.m_dashes.push_back( -dSectorGapLength );
  }

  double dSubSectorGap = m_dSymbolWidth * ( n - 1 ) / cos( dCurAngle );

  if( !OdZero(dSubSectorGap) )
  {
    newLine.m_dashes.push_back( -dSubSectorGap );
  }

  m_hatchPattern.push_back( newLine );
}

//-----------------------------------------------------------------------------------------

void OdDgnImportSymbolToHatchConverter::addHatchLine( OdGePoint2d ptStart, OdGePoint2d ptEnd )
{
  if( OdZero(ptStart.distanceTo( ptEnd )) )
  {
    OdHatchPatternLine newLine;
    newLine.m_basePoint = ptStart;
    newLine.m_dLineAngle = 0;
    newLine.m_patternOffset.set( 0 , m_dSymbolHeight );
    newLine.m_dashes.push_back(0);
    newLine.m_dashes.push_back(-m_dSymbolWidth);

    m_hatchPattern.push_back( newLine );
  }
  else
  {
    OdGeVector2d vrLine = ptEnd - ptStart;
    vrLine.normalize();

    double dCurAngle = vrLine.angle();

    if( OdZero( dCurAngle, 1e-2) || OdZero( dCurAngle - OdaPI, 1e-2) || OdZero( dCurAngle - Oda2PI, 1e-2) )
    {
      OdHatchPatternLine newLine;
      newLine.m_basePoint.set(0, ptStart.y);
      newLine.m_dLineAngle = 0;
      newLine.m_patternOffset.set( 0 , m_dSymbolHeight );

      double dStartGap = 0;
      double dDash     = 0;
      double dEndGap   = 0;

      if( ptStart.x < ptEnd.x )
      {
        dStartGap = ptStart.x;
        dDash     = ptEnd.x - ptStart.x;
        dEndGap   = m_dSymbolWidth - ptEnd.x;
      }
      else
      {
        dStartGap = ptEnd.x;
        dDash     = ptStart.x - ptEnd.x;
        dEndGap   = m_dSymbolWidth - ptStart.x;
      }

      if( !OdZero( dStartGap, 1e-8) )
      {
        newLine.m_dashes.push_back(-dStartGap);
      }

      newLine.m_dashes.push_back(dDash);

      if( !OdZero( dEndGap, 1e-8) )
      {
        newLine.m_dashes.push_back(-dEndGap);
      }

      m_hatchPattern.push_back( newLine );

      OdGePoint2d ptNewEnd( ptEnd.x, ptStart.y);

      if( !OdZero( ptNewEnd.distanceTo(ptEnd), 1e-3) )
      {
        m_arrBasePoints.push_back( ptEnd );
        m_arrCorrectedPoints.push_back( ptNewEnd );
      }
    }
    else if( OdZero( dCurAngle - OdaPI2, 1e-2) || OdZero( dCurAngle - 3*OdaPI2, 1e-2) )
    {
      OdHatchPatternLine newLine;
      newLine.m_basePoint.set(ptStart.x, 0);
      newLine.m_dLineAngle = OdaPI2;
      newLine.m_patternOffset.set( 0 , m_dSymbolWidth );

      double dStartGap = 0;
      double dDash     = 0;
      double dEndGap   = 0;

      if( ptStart.y < ptEnd.y )
      {
        dStartGap = ptStart.y;
        dDash     = ptEnd.y - ptStart.y;
        dEndGap   = m_dSymbolHeight - ptEnd.y;
      }
      else
      {
        dStartGap = ptEnd.y;
        dDash     = ptStart.y - ptEnd.y;
        dEndGap   = m_dSymbolHeight - ptStart.y;
      }

      if( !OdZero( dStartGap, 1e-8) )
      {
        newLine.m_dashes.push_back(-dStartGap);
      }

      newLine.m_dashes.push_back(dDash);

      if( !OdZero( dEndGap, 1e-8) )
      {
        newLine.m_dashes.push_back(-dEndGap);
      }

      m_hatchPattern.push_back( newLine );

      OdGePoint2d ptNewEnd( ptStart.x, ptEnd.y);

      if( !OdZero( ptNewEnd.distanceTo(ptEnd), 1e-3) )
      {
        m_arrBasePoints.push_back( ptEnd );
        m_arrCorrectedPoints.push_back( ptNewEnd );
      }
    }
    else
    {
      if( (dCurAngle > OdaPI2) && (dCurAngle < 3*OdaPI2) )
      {
        dCurAngle -= OdaPI;

        if( dCurAngle < 0  )
        {
          dCurAngle = Oda2PI - dCurAngle;
        }

        OdGePoint2d ptTmp = ptStart;
        ptStart = ptEnd;
        ptEnd   = ptTmp;
      }

      if( (dCurAngle < OdaPI2) && (dCurAngle > m_dBorderAngle) )
      {
        createYTopLine( dCurAngle, ptStart, ptEnd );
      }
      else if( m_dBorderAngle >= dCurAngle )
      {
        createXTopLine( dCurAngle, ptStart, ptEnd );
      }
      else if( dCurAngle > OdaPI )
      {
        createXBottomLine( dCurAngle, ptStart, ptEnd );
      }
      else
      {
        createYBottomLine( dCurAngle, ptStart, ptEnd );
      }
    }
  }
}

//-----------------------------------------------------------------------------------------

void OdDgnImportSymbolToHatchConverter::addHatchPolyline(const OdGePoint3dArray& arrPts)
{
  addHatchPolyline( arrPts.asArrayPtr(), arrPts.size() );
}

//-----------------------------------------------------------------------------------------

void OdDgnImportSymbolToHatchConverter::addHatchPolyline(const OdGePoint3d* arrPts, OdUInt32 uNumPts )
{
  m_arrBasePoints.clear();
  m_arrCorrectedPoints.clear();

  for( OdUInt32 i = 0; i < uNumPts - 1; i++ )
  {
    OdGePoint2d ptStart = arrPts[i].convert2d();
    OdGePoint2d ptEnd   = arrPts[i + 1].convert2d();
    ptStart /= m_dScaleFactor;
    ptEnd   /= m_dScaleFactor;

    ptStart -= m_ptOffset.convert2d().asVector();
    ptEnd   -= m_ptOffset.convert2d().asVector();

    for( OdUInt32 j = 0; j < m_arrBasePoints.size(); j++ )
    {
      if( OdZero( ptStart.distanceTo(m_arrBasePoints[j]), 1e-4 ) )
      {
        ptStart = m_arrCorrectedPoints[j];
        break;
      }
    }

    addHatchLine( ptStart, ptEnd );
  }
}

//-----------------------------------------------------------------------------------------

void OdDgnImportSymbolToHatchConverter::circleProc(const OdGePoint3d& center, 
                                                     double radius, 
                                                       const OdGeVector3d& normal, 
                                                         const OdGeVector3d* pExtrusion )
{
  OdGeCircArc3d geArc( center, normal, normal.perpVector(), radius, 0, Oda2PI );

  OdGePoint3dArray arrPts;

  geArc.getSamplePoints( m_uSircleSegNum, arrPts );

  if( geArc.isClosed() )
  {
    if( !OdZero(arrPts[0].distanceTo(arrPts[arrPts.size() - 1])) )
    {
      arrPts.push_back(arrPts[0]);
    }
  }

  addHatchPolyline( arrPts );
}

//-----------------------------------------------------------------------------------------

void OdDgnImportSymbolToHatchConverter::circleProc(const OdGePoint3d& pt1, 
                                                     const OdGePoint3d& pt2, 
                                                       const OdGePoint3d& pt3,
                                                         const OdGeVector3d* pExtrusion )
{
  OdGeCircArc3d geArc1( pt1, pt2, pt3 );

  OdGeCircArc3d geArc( geArc1.center(), geArc1.normal(), geArc1.normal().perpVector(), geArc1.radius(), 0, Oda2PI );

  OdGePoint3dArray arrPts;

  geArc.getSamplePoints( m_uSircleSegNum, arrPts );

  if( geArc.isClosed() )
  {
    if( !OdZero(arrPts[0].distanceTo(arrPts[arrPts.size() - 1])) )
    {
      arrPts.push_back(arrPts[0]);
    }
  }

 addHatchPolyline( arrPts );
}

//-----------------------------------------------------------------------------------------

void OdDgnImportSymbolToHatchConverter::circularArcProc(const OdGePoint3d& center, 
                                                          double radius, 
                                                            const OdGeVector3d& normal, 
                                                              const OdGeVector3d& startVector, 
                                                                double sweepAngle, 
                                                                  OdGiArcType arcType, 
                                                                    const OdGeVector3d* pExtrusion )
{
  OdGeCircArc3d geArc( center, normal, startVector, radius, 0, sweepAngle );

  OdGePoint3dArray arrPts;

  geArc.getSamplePoints( m_uSircleSegNum, arrPts );

  if( geArc.isClosed() )
  {
    if( !OdZero(arrPts[0].distanceTo(arrPts[arrPts.size() - 1])) )
    {
      arrPts.push_back(arrPts[0]);
    }
  }

  addHatchPolyline( arrPts );
}

//-----------------------------------------------------------------------------------------

void OdDgnImportSymbolToHatchConverter::circularArcProc(const OdGePoint3d& start, 
                                                          const OdGePoint3d& point, 
                                                            const OdGePoint3d& end, 
                                                              OdGiArcType arcType, 
                                                                const OdGeVector3d* pExtrusion )
{
  OdGeCircArc3d geArc( start, point, end );

  OdGePoint3dArray arrPts;

  geArc.getSamplePoints( m_uSircleSegNum, arrPts );

  if( geArc.isClosed() )
  {
    if( !OdZero(arrPts[0].distanceTo(arrPts[arrPts.size() - 1])) )
    {
      arrPts.push_back(arrPts[0]);
    }
  }

  addHatchPolyline( arrPts );
}

//-----------------------------------------------------------------------------------------

void OdDgnImportSymbolToHatchConverter::polylineOut(OdInt32 nbPoints, const OdGePoint3d* pVertexList)
{
  addHatchPolyline( pVertexList, nbPoints );
}

//-----------------------------------------------------------------------------------------

void OdDgnImportSymbolToHatchConverter::polygonOut(OdInt32 numPoints, 
                                                     const OdGePoint3d* vertexList, 
                                                       const OdGeVector3d* pNormal )
{
  addHatchPolyline( vertexList, numPoints );
}

//-----------------------------------------------------------------------------------------

void OdDgnImportSymbolToHatchConverter::nurbsProc( const OdGeNurbCurve3d& nurbsCurve )
{
  OdGePoint3dArray arrPts;

  nurbsCurve.getSamplePoints( m_uSircleSegNum, arrPts );

  if( nurbsCurve.isClosed() )
  {
    if( !OdZero(arrPts[0].distanceTo(arrPts[arrPts.size() - 1])) )
    {
      arrPts.push_back(arrPts[0]);
    }
  }

  addHatchPolyline( arrPts );
}

//-----------------------------------------------------------------------------------------

void OdDgnImportSymbolToHatchConverter::ellipArcProc( const OdGeEllipArc3d& ellipArc, 
                                                        const OdGePoint3d* endPointOverrides, 
                                                          OdGiArcType arcType,
                                                            const OdGeVector3d* pExtrusion )
{
  OdGePoint3dArray arrPts;

  ellipArc.getSamplePoints( m_uSircleSegNum, arrPts );

  if( ellipArc.isClosed() )
  {
    if( !OdZero(arrPts[0].distanceTo(arrPts[arrPts.size() - 1])) )
    {
      arrPts.push_back(arrPts[0]);
    }
  }

  addHatchPolyline( arrPts );
}

//=========================================================================================

void importSymbolPattern( OdDbHatchPtr& pHatch, 
                            OdDgElement* pElm, 
                              const OdDgSymbolPatternLinkagePtr& pPaternLinkage, 
                                double dUORsToModelScale,
                                  OdDbBlockTableRecord* pOwner
                        )
{
  pHatch->setHatchStyle( OdDbHatch::kNormal );
  pHatch->setHatchObjectType( OdDbHatch::kHatchObject );

  OdDgSharedCellDefinitionPtr pSymbol;

  OdRxObjectPtrArray dependencyLinkages;
  OdDgDepLinkageElementIdPtr dependencyLinkage;

  pElm->getLinkages( OdDgAttributeLinkage::kDependency, dependencyLinkages );

  int i, j = dependencyLinkages.size();

  for( i = 0; i < j; i++ )
  {
    if( !dependencyLinkages[ i ]->isKindOf( OdDgDepLinkageElementId::desc() ) )
    {
      continue;
    }

    dependencyLinkage = dependencyLinkages[ i ];
    if( dependencyLinkage->getAppId() == 0x2714 )
    {
      OdDgElementId elementId = pElm->database()->getElementId( OdDbHandle( dependencyLinkage->getAt( 0 ) ) );
      OdDgElementPtr element = elementId.openObject( OdDg::kForRead );

      if( element->isKindOf( OdDgSharedCellDefinition::desc() ) )
      {
        pSymbol = element;
        break;
      }
    }
  }

  bool bCorrectHatch = false;

  if( !pSymbol.isNull() )
  {
    OdDgnImportSymbolHatchDescriptor hatchDesc;

    OdUInt32 uHatchColorIndex = 0;

    if( OdDgnImportContext::getSymbolHatchDesc( pSymbol->elementId(), pPaternLinkage->getSpace2() / pPaternLinkage->getScale(), 
                                                pPaternLinkage->getSpace1() / pPaternLinkage->getScale(), hatchDesc) )
    {
      OdGePoint2d ptBase = getHatchSeedPoint( pElm, pPaternLinkage, pHatch, dUORsToModelScale );

      pHatch->setPattern(OdDbHatch::kUserDefined, L"_USER", pPaternLinkage->getAngle1(), pPaternLinkage->getScale() * dUORsToModelScale * hatchDesc.m_dHatchScale, hatchDesc.m_hatchPattern, ptBase );

      uHatchColorIndex = hatchDesc.m_uColorIndex;
      bCorrectHatch = true;
    }
    else
    {
      OdHatchPattern dwgHatch;

      OdStaticRxObject<OdDgnImportSymbolToHatchConverter> symbolToHatch;

      double dSymbolScale = 1.0;

      if( symbolToHatch.convertSymbolToHatch( pSymbol, 1e-2, 300, 5, 12, pPaternLinkage->getSpace2() / pPaternLinkage->getScale(), 
                                              pPaternLinkage->getSpace1() / pPaternLinkage->getScale(), 
                                              dwgHatch, dSymbolScale, uHatchColorIndex ) )
      {
        hatchDesc.m_dHatchScale  = dSymbolScale;
        hatchDesc.m_hatchPattern = dwgHatch;
        hatchDesc.m_uColorIndex  = uHatchColorIndex;
        hatchDesc.m_dPatternRowSpacing = pPaternLinkage->getSpace2() / pPaternLinkage->getScale();
        hatchDesc.m_dPatternColSpacing = pPaternLinkage->getSpace1() / pPaternLinkage->getScale();

        OdDgnImportContext::addSymbolHatchDesc( pSymbol->elementId(), hatchDesc );

        OdGePoint2d ptBase = getHatchSeedPoint( pElm, pPaternLinkage, pHatch, dUORsToModelScale );

        pHatch->setPattern(OdDbHatch::kUserDefined, L"_USER", pPaternLinkage->getAngle1(), pPaternLinkage->getScale() * dUORsToModelScale * dSymbolScale, dwgHatch, ptBase );
        bCorrectHatch = true;
      }
    }

    if( !bCorrectHatch )
    {
      importSymbolPattern( pElm, pOwner );
    }
    else if( !pSymbol->getViewIndependentFlag() )
    {
      pHatch->setLinetype(OdDbObjectId(0));

      if( uHatchColorIndex != OdDg::kColorByCell )
      {
        setDwgColorByDgnIndex( pSymbol->database(), pHatch, uHatchColorIndex );
      }
    }
  }
  else
  {
    pHatch->erase(true);
  }
}

//-----------------------------------------------------------------------------------------

void importDwgBasedPattern( OdDbHatchPtr& pHatch,
                              OdDgElement* pElm,
                                const OdDgDWGPatternLinkagePtr& pPaternLinkage,
                                  double dUORsToModelScale
                         )
{
  pHatch->setHatchStyle( OdDbHatch::kNormal );
  pHatch->setHatchObjectType( OdDbHatch::kHatchObject );

  OdDgDWGPatternLinkage::DWGHatch dwgHatch = pPaternLinkage->getHatch();

  OdHatchPattern dwgPattern;

  for( OdUInt32 i = 0; i < dwgHatch.size(); i++ )
  {
    OdHatchPatternLine patternLine;

    patternLine.m_basePoint = dwgHatch[i].m_throughPoint * dUORsToModelScale;
    patternLine.m_dLineAngle = dwgHatch[i].m_angle;
    patternLine.m_patternOffset = dwgHatch[i].m_offset.asVector() * dUORsToModelScale;

    for( OdUInt32 j = 0; j < dwgHatch[i].m_dashes.size(); j++ )
    {
      patternLine.m_dashes.push_back(dwgHatch[i].m_dashes[j]*dUORsToModelScale);
    }

    patternLine.m_patternOffset.rotateBy( -patternLine.m_dLineAngle );

    dwgPattern.push_back( patternLine );
  }

  OdGePoint2d ptBase = getHatchSeedPoint( pElm, pPaternLinkage, pHatch, dUORsToModelScale );

  pHatch->setPattern(OdDbHatch::kUserDefined, L"_USER", 0.0, 1.0, dwgPattern, ptBase );
}

//-----------------------------------------------------------------------------------------

OdDbEntityPtr createHatchEntity( const OdGePoint3dArray& arrPts )
{
  if( arrPts.size() < 3 )
    return OdDbEntityPtr();

  OdGeVector3d vNormal;

  if (geCalculateNormal(arrPts, &vNormal) == OdGe::kOk )
  {
    vNormal = odgiFaceNormal(arrPts.size(), arrPts.asArrayPtr());
    OdGeMatrix3d matrix;

    if (vNormal.isParallelTo(OdGeVector3d::kZAxis))
    {
      vNormal = OdGeVector3d::kZAxis;
    }
    else
    {
      matrix.setToWorldToPlane(vNormal);
    }

    OdDbHatchPtr pHatch = OdDbHatch::createObject();

    pHatch->setNormal(vNormal);
    OdGePoint3d pnt = matrix * arrPts[0];
    pHatch->setElevation(pnt.z);
    matrix.setTranslation(OdGeVector3d(0., 0., -pnt.z));

    OdGePoint2dArray verts;
    OdGeDoubleArray bulges;

    for( OdUInt32 f = 0; f < arrPts.size(); ++f)
    {
      OdGePoint3d ptPos(matrix * arrPts[f]);
      ODA_ASSERT(OdZero(ptPos.z / 2.));
      verts.append(ptPos.convert2d());
      bulges.append(0.0);
    }

    pHatch->appendLoop(OdDbHatch::kOutermost|OdDbHatch::kPolyline, verts, bulges);

    return pHatch;
  }

  return OdDbEntityPtr();
}

//-----------------------------------------------------------------------------------------

bool getLoopNormal( const OdDbObjectIdArray& arrElements, OdGeVector3d& vrNormal )
{
  bool bRet = false;

  for( OdUInt32 i = 0; i < arrElements.size(); i++ )
  {
    OdDbObjectPtr pObject = arrElements[i].openObject( OdDb::kForRead );

    if( !pObject.isNull() )
    {
      if( pObject->isKindOf( OdDbCircle::desc() ) )
      {
        OdDbCirclePtr pCircle = pObject;

        vrNormal = pCircle->normal();

        bRet = true;
      }
      else if( pObject->isKindOf( OdDbEllipse::desc() ) )
      {
        OdDbEllipsePtr pEllipse = pObject;

        vrNormal = pEllipse->normal();

        bRet = true;
      }
      else if( pObject->isKindOf( OdDbArc::desc() ) )
      {
        OdDbArcPtr pArc = pObject;

        vrNormal = pArc->normal();

        bRet = true;
      }
      else if( pObject->isKindOf( OdDbPolyline::desc() ) )
      {
        OdDbPolylinePtr pPLine = pObject;

        vrNormal = pPLine->normal();

        bRet = true;
      }
      else if( pObject->isKindOf( OdDbHatch::desc() ) )
      {
        OdDbHatchPtr pHatch = pObject;

        vrNormal = pHatch->normal();

        bRet = true;
      }
      else if( pObject->isKindOf( OdDbRegion::desc() ) )
      {
        OdDbRegionPtr pRegion = pObject;

        if( pRegion->getNormal( vrNormal ) == eOk )
        {
          bRet = true;
        }
      }

      if( bRet )
      {
        break;
      }
    }
  }

  return bRet;
}

//-----------------------------------------------------------------------------------------

OdDbEntityPtr createHatchEntity( const OdDbObjectIdArray& arrElements, bool bSplitContours )
{
  if( arrElements.isEmpty() )
    return OdDbEntityPtr();

  OdDbHatchPtr pHatch = OdDbHatch::createObject();

  OdGeVector3d vrNormal;

  if( getLoopNormal( arrElements, vrNormal) )
  {
    pHatch->setNormal( vrNormal );
  }

  if( bSplitContours )
  {
    for( OdUInt32 i = 0; i < arrElements.size(); i++ )
    {
      OdDbObjectIdArray arrTmp;
      arrTmp.push_back( arrElements[i] );
      pHatch->appendLoop(OdDbHatch::kExternal, arrTmp);
    }
  }
  else
  {
    pHatch->appendLoop(OdDbHatch::kExternal, arrElements);
    pHatch->setAssociative( true );
    pHatch->setAssocObjIdsAt( pHatch->numLoops() - 1, arrElements );
  }

  return pHatch;
}

//-----------------------------------------------------------------------------------------

void importPatterns( OdDgGraphicsElementPtr pShape, 
                       const OdGePoint3dArray& arrVertices,
                         const OdDbObjectIdArray& arrElementIds,
                           bool bUsePoints,
                             OdDbBlockTableRecord* owner,
                               bool bAssoc,
                                 OdDbObjectId idAssoc
                    )
{
  OdDgViewPtr pActiveView = OdDgnImportContext::getActiveView();

  if( !pActiveView.isNull() && !pActiveView->getShowPatternsFlag() )
  {
    return;
  }

  OdRxObjectPtrArray arrFillLinkages;
  OdRxObjectPtrArray arrPatternLinkages;

  OdDgGradientFillLinkagePtr pGradientLinkage;
  OdDgPatternLinkagePtr      pPaternLinkage;

  OdDgGraphicsElementPtr pBaseElement = pShape;

  OdDgElementId idOwner = pShape->ownerId();

  if( !idOwner.isNull() )
  {
    OdDgElementPtr pOwner = idOwner.openObject(OdDg::kForRead);

    if( pOwner->getElementType() == OdDgElement::kTypeCellHeader )
    {
      pOwner->getLinkages( OdDgAttributeLinkage::kHatch, arrPatternLinkages );
    }

    if( arrPatternLinkages.size() > 0 )
    {
      pBaseElement = pOwner;
    }
  }


  pShape->getLinkages( OdDgAttributeLinkage::kFillStyle, arrFillLinkages );

  if( arrPatternLinkages.size() == 0 )
  {
    pShape->getLinkages( OdDgAttributeLinkage::kHatch, arrPatternLinkages );
  }

  for( OdUInt32 i = 0; i < arrFillLinkages.size(); i++ )
  {
    if( arrFillLinkages[i]->isKindOf( OdDgGradientFillLinkage::desc() ) )
    {
      pGradientLinkage = arrFillLinkages[i];
      break;
    }
  }

  for( OdUInt32 j = 0; j < arrPatternLinkages.size(); j++ )
  {
    if( arrPatternLinkages[j]->isKindOf( OdDgLinearPatternLinkage::desc() ) || 
        arrPatternLinkages[j]->isKindOf( OdDgCrossPatternLinkage::desc() )  ||
        arrPatternLinkages[j]->isKindOf( OdDgSymbolPatternLinkage::desc() ) ||
        arrPatternLinkages[j]->isKindOf( OdDgDWGPatternLinkage::desc() )
      )
    {
      pPaternLinkage = arrPatternLinkages[j];
      break;
    }
  }

  if( !pGradientLinkage.isNull() || !pPaternLinkage.isNull() )
  {
    double dUORsToModelScale = 1.0;

    OdDgElementId idDgOwner = pShape->ownerId();

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

    OdDbHatchPtr pHatch;
    
    if( bUsePoints )
    {
      pHatch = createHatchEntity( arrVertices );

      if( !arrElementIds.isEmpty() )
      {
        pHatch->setAssociative(true);
        pHatch->setAssocObjIdsAt( 0, arrElementIds );
      }
    }
    else
    {
      pHatch = createHatchEntity( arrElementIds, false );
    }

    if( !pHatch.isNull() )
    {
      owner->appendOdDbEntity(pHatch);
    }
    else
    {
      return;
    }

    if( bAssoc && !idAssoc.isNull() )
    {
      pHatch->setAssociative( true );
      OdDbObjectIdArray arrIds;
      arrIds.push_back( idAssoc );
      pHatch->setAssocObjIdsAt( 0, arrIds );
    }

    if( !pGradientLinkage.isNull() )
    {
      copyEntityProperties( pShape.get(), pHatch );

      importGradient( pHatch, pGradientLinkage );

//       if( bAssoc && !idAssoc.isNull() )
//       {
//         OdDbEntityPtr pEnt = idAssoc.openObject(OdDb::kForWrite);
// 
//         if( !pEnt.isNull() )
//         {
//           pEnt->setVisibility( OdDb::kInvisible );
//         }
//       }

      pHatch = 0;
    }
    else 
    {
      copyEntityProperties( pBaseElement.get(), pHatch );
    }

    if( !pPaternLinkage.isNull() && pHatch.isNull() )
    {
      if( bUsePoints )
      {
        pHatch = createHatchEntity( arrVertices );
      }
      else
      {
        pHatch = createHatchEntity( arrElementIds, false );
      }

      owner->appendOdDbEntity(pHatch);

      if( bAssoc && !idAssoc.isNull() )
      {
        pHatch->setAssociative( true );
        OdDbObjectIdArray arrIds;
        arrIds.push_back( idAssoc );
        pHatch->setAssocObjIdsAt( 0, arrIds );
      }

      copyEntityProperties( pBaseElement.get(), pHatch );
    }

    if( !pPaternLinkage.isNull() && pPaternLinkage->isKindOf( OdDgLinearPatternLinkage::desc()) )
    {
      importLinearPattern( pHatch, pShape, pPaternLinkage, dUORsToModelScale );
    }
    else if( !pPaternLinkage.isNull() && pPaternLinkage->isKindOf( OdDgCrossPatternLinkage::desc()) )
    {
      importCrossPattern( pHatch, pShape, pPaternLinkage, dUORsToModelScale );
    }
    else if( !pPaternLinkage.isNull() && pPaternLinkage->isKindOf( OdDgDWGPatternLinkage::desc()) )
    {
      importDwgBasedPattern( pHatch, pShape, pPaternLinkage, dUORsToModelScale );
    }
    else if( !pPaternLinkage.isNull() && pPaternLinkage->isKindOf( OdDgSymbolPatternLinkage::desc()) )
    {
      importSymbolPattern( pHatch, pShape, pPaternLinkage, dUORsToModelScale, owner );
      //importSymbolPattern( pBaseElement, owner );
      //pHatch->erase(true);
    }
    else if( !pHatch.isNull() )
    {
      pHatch->erase(true);
    }
  }
}

//--------------------------------------------------------------------------------------------------------------

void importClosedElementFill( OdDgElement* e, OdDbEntity* dbEll, OdDbBlockTableRecord* owner )
{
  bool     bUseFillColor   = false;
  OdUInt32 uFillColorIndex = 0;

  OdRxObjectPtrArray arrFillLinkages;

  e->getLinkages( OdDgAttributeLinkage::kFillStyle, arrFillLinkages );

  for( OdUInt32 i = 0; i < arrFillLinkages.size(); i++ )
  {
    if( arrFillLinkages[i]->isKindOf(OdDgFillColorLinkage::desc() ) )
    {
      bUseFillColor = true;
      OdDgFillColorLinkagePtr pFillColor = arrFillLinkages[i];
      uFillColorIndex = pFillColor->getColorIndex();
      break;
    }
  }

  OdDgViewPtr pActiveView = OdDgnImportContext::getActiveView();

  if( !pActiveView.isNull() && !pActiveView->getShowFillsFlag() )
  {
    bUseFillColor = false;
  }

  OdDbObjectIdArray arrHatchItems;
  arrHatchItems.push_back( dbEll->id() );

  if( bUseFillColor ) // Create solid hatch
  {
    OdDbHatchPtr pHatch = OdDbHatch::createObject();
    owner->appendOdDbEntity( pHatch );

    OdGeVector3d vrNormal = OdGeVector3d::kZAxis;

    if( getLoopNormal( arrHatchItems, vrNormal) )
    {
      pHatch->setNormal( vrNormal );
    }

    pHatch->appendLoop( OdDbHatch::kExternal, arrHatchItems );
    pHatch->setPattern( OdDbHatch::kPreDefined, L"SOLID" );

    if( !pHatch.isNull() )
    {
      if( e->isKindOf(OdDgGraphicsElement::desc()) )
      {
        OdDgGraphicsElement* pGrElement = dynamic_cast<OdDgGraphicsElement*>(e);

        if( pGrElement )
        {
          copyEntityProperties( pGrElement, pHatch);
        }
      }

      setDwgColorByDgnIndex( e->database(), pHatch, uFillColorIndex );
    }

    pHatch->swapIdWith( dbEll->id(), true, true );
    arrHatchItems.clear();
    arrHatchItems.push_back( dbEll->id() );

    pHatch->setAssociative(true);
    pHatch->setAssocObjIdsAt( 0, arrHatchItems );
  }

  OdGePoint3dArray arrPoints;

  importPatterns( e, arrPoints, arrHatchItems, false, owner, true, dbEll->id() );
}

}
