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
#include "DgnImportXRef.h"
#include "DgnImportCommon.h"
#include "DgnImportContext.h"
#include <RxDispatchImpl.h>
#include <RxVariantValue.h>
#include "DgnImportImpl.h"
#include <DgColorTable.h>
#include <DgAttributeLinkage.h>
#include <DgDatabase.h>
#include <DgReferenceAttach.h>
#include <DbSpatialFilter.h>
#include <Gi/GiClipBoundary.h>
#include <XRefMan.h>
#include <DbBlockReference.h>
#include <DbBlockTable.h>
#include <OdUtilAds.h>
#include "DbSymUtl.h"
#include <ExHostAppServices.h>
#include "DgLevelTableRecord.h"
#include "DbUnderlayDefinition.h"
#include "DbUnderlayReference.h"
#include "DbViewport.h"
#include "DbIdMapping.h"
#include "DbDimAssoc.h"
#include "DgProxyElement.h"

#include "Gi/GiBaseVectorizer.h"
#include "Gi/GiGeometrySimplifier.h"

namespace TD_DGN_IMPORT {

//---------------------------------------------------------------------------------------------------

bool getXRefPalette( OdDgReferenceAttachmentHeader* pXRef, OdDgDatabase* pDb, ODCOLORREF* pPalette )
{
  bool bRet = false;

  if( pDb && pXRef && pPalette )
  {
    OdDgColorTablePtr pRefColorTable = pDb->getColorTable(OdDg::kForRead);

    if( !pRefColorTable.isNull() )
    {
      memcpy( pPalette, pRefColorTable->palette(), 256*sizeof(ODCOLORREF));
    }
    else
    {
      memcpy( pPalette, OdDgColorTable::defaultPalette(), 256*sizeof(ODCOLORREF));
    }

    OdDgColorTable::correctPaletteForWhiteBackground(pPalette);

    // Check color correction for reference attachment palette

    bool bUseColorCorrection = false;
    double dInvertLighting   = 0;
    double dInvertSaturation = 0;

    OdRxObjectPtrArray arrLinkages;
    pXRef->getLinkages( OdDgAttributeLinkage::kDoubleArray, arrLinkages );

    for( OdUInt32 iLink = 0; iLink < arrLinkages.size(); iLink++ )
    {
      if( arrLinkages[iLink]->isKindOf(OdDgDoubleArrayLinkage::desc()) )
      {
        OdDgDoubleArrayLinkagePtr pDblArrLinkage = arrLinkages[iLink];

        if( !pDblArrLinkage.isNull() && pDblArrLinkage->getArrayId() == 0x09 && pDblArrLinkage->getItemCount() == 2 )
        {
          dInvertLighting   = pDblArrLinkage->getItem(0);
          dInvertSaturation = pDblArrLinkage->getItem(1);

          if( (dInvertLighting >= 0) && (dInvertLighting < 101) && 
            (dInvertSaturation >= 0) && (dInvertSaturation <101)  
            )
          {
            bUseColorCorrection = true;
            dInvertLighting   = (OdUInt32)(dInvertLighting);
            dInvertSaturation = (OdUInt32)(dInvertSaturation);
          }

          break;
        }
      }
    }

    if( bUseColorCorrection )
    {
      for( OdUInt32 iColor = 0; iColor < 256; iColor++ )
      {
        ODCOLORREF curColor = pPalette[iColor];

        // Saturation correction

        if( !OdZero(dInvertSaturation) )
        {
          OdUInt8 uRed   = ODGETRED( curColor );
          OdUInt8 uGreen = ODGETGREEN( curColor );
          OdUInt8 uBlue  = ODGETBLUE( curColor );

          OdUInt8 uMax = (uRed > uGreen) ? uRed : uGreen;

          if( uBlue > uMax )
          {
            uMax = uBlue;
          }

          uRed   = uRed   + (OdUInt8)(( uMax - uRed   )*dInvertSaturation / 100.0);
          uGreen = uGreen + (OdUInt8)(( uMax - uGreen )*dInvertSaturation / 100.0);
          uBlue  = uBlue  + (OdUInt8)(( uMax - uBlue  )*dInvertSaturation / 100.0);

          curColor = ODRGB( uRed, uGreen, uBlue );
        }

        // Lighting correction

        if( !OdZero(dInvertLighting) )
        {
          OdUInt8 uRed   = ODGETRED( curColor );
          OdUInt8 uGreen = ODGETGREEN( curColor );
          OdUInt8 uBlue  = ODGETBLUE( curColor );

          uRed     = (OdUInt8)(uRed  *(100.0 - dInvertLighting) / 100.0);
          uGreen   = (OdUInt8)(uGreen*(100.0 - dInvertLighting) / 100.0);
          uBlue    = (OdUInt8)(uBlue *(100.0 - dInvertLighting) / 100.0);

          curColor = ODRGB( uRed, uGreen, uBlue );
        }

        pPalette[iColor] = curColor;
      }
    }

    bRet = true;
  }

  return bRet;
}

//---------------------------------------------------------------------------------------------------

bool getActualXRefLevelMask( const OdDgReferenceAttachmentHeader* pXRef, OdUInt32 uViewNumber, OdDgLevelMaskPtr& pLevelMask )
{
  bool bRet = false;

  OdDgViewPtr pCurView = OdDgnImportContext::getActiveView();

  if( !pCurView.isNull() )
  {
    OdDgElementIteratorPtr pViewIter = pCurView->createIterator();

    bool bPathCorrect = true;

    if( OdDgnImportContext::getXRefNestedDepth() > 0 )
    {
      OdArray<OdUInt64> arrXRefPath;

      for( OdUInt32 k = 0; k < OdDgnImportContext::getXRefNestedDepth(); k++ )
      {
        bool bLevelGroupIsPresent = false;

        OdDgElementId idPathXRef = OdDgnImportContext::getXRefId(k);

        if( !idPathXRef.isNull() )
        {
          for(; !pViewIter->done(); pViewIter->step() )
          {
            OdDgElementPtr pElm = pViewIter->item().openObject(OdDg::kForRead);

            if( pElm->isKindOf(OdDgReferenceAttachmentLevelMaskGroup::desc()) )
            {
              OdDgReferenceAttachmentLevelMaskGroupPtr pLevelMaskGroup = pElm;

              if( pLevelMaskGroup->getReferenceAttachmentId() == (OdUInt64)(idPathXRef.getHandle()) )
              {
                bLevelGroupIsPresent = true;
                pViewIter = pLevelMaskGroup->createIterator();
                break;
              }
            }
            else if( pElm->isKindOf(OdDgComplexProxyElement::desc()) )
            {
              OdRxObjectPtrArray arrLinkages;
              pElm->getLinkages( OdDgAttributeLinkage::kReferenceAttachmentPath, arrLinkages );

              if( arrLinkages.size() > 0 )
              {
                OdDgReferenceAttachPathLinkagePtr pPath = arrLinkages[0];

                if( pPath->getPathItem(pPath->getPathLength() - 1) == (OdUInt64)(idPathXRef.getHandle()) )
                {
                  OdDgComplexProxyElementPtr pProxy = pElm;
                  bLevelGroupIsPresent = true;
                  pViewIter = pProxy->createIterator();
                  break;
                }
              }
            }
          }

          if( !bLevelGroupIsPresent )
          {
            bPathCorrect = false;
            break;
          }
        }
        else
        {
          bPathCorrect = false;
          break;
        }
      }
    }

    if( !pViewIter.isNull() )
    {
      for(; !pViewIter->done(); pViewIter->step() )
      {
        OdDgElementPtr pElm = pViewIter->item().openObject(OdDg::kForRead);

        if( pElm->isKindOf(OdDgLevelMask::desc()) )
        {
          OdDgLevelMaskPtr pCurLevelMask = pElm;

          if( pCurLevelMask->getReferenceAttachHandleId() == (OdUInt64)(pXRef->elementId().getHandle()) )
          {
            pLevelMask = pCurLevelMask;
            bRet = true;
            break;
          }
        }
      }
    }
  }

  if( bRet )
    return bRet;

  OdDgReferenceAttachmentHeaderPtr pBaseXRef = pXRef;

  OdDgElementIteratorPtr pLevelMaskIter;

  if( OdDgnImportContext::getXRefNestedDepth() > 0 )
  {
    OdArray<OdUInt64> arrXRefPath;

    for( OdUInt32 k = 1; k < OdDgnImportContext::getXRefNestedDepth(); k++ )
    {
      OdDgElementId idPathXRef = OdDgnImportContext::getXRefId(k);

      if( !idPathXRef.isNull() )
      {
        arrXRefPath.push_back( (OdUInt64)(idPathXRef.getHandle()));
      }
      else
      {
        arrXRefPath.push_back( (OdUInt64)(0));
      }
    }

    arrXRefPath.push_back( (OdUInt64)(pXRef->elementId().getHandle()));

    OdDgElementId idBaseXRef = OdDgnImportContext::getXRefId(0);
    pBaseXRef = idBaseXRef.openObject(OdDg::kForRead);

    OdDgElementIteratorPtr pXRefIter = pBaseXRef->createIterator();

    for(; !pXRefIter->done(); pXRefIter->step() )
    {
      OdDgElementPtr pChild = pXRefIter->item().openObject(OdDg::kForRead);

      if( !pChild.isNull() && pChild->isKindOf( OdDgReferenceOverride::desc() ) )
      {
        OdDgReferenceOverridePtr pXRefOverride = pChild;

        OdUInt64Array arrPath;
        pXRefOverride->getPathOfXRef(arrPath);

        if( arrPath.size() == arrXRefPath.size() )
        {
          bool bCorrectOverride = true;

          for( OdUInt32 n = 0; n < arrXRefPath.size(); n++ )
          {
            if( arrPath[n] != arrXRefPath[n] )
            {
              bCorrectOverride = false;
              break;
            }
          }

          if( bCorrectOverride )
          {
            pLevelMaskIter = pXRefOverride->createIterator();
            break;
          }
        }
      }
    }
  }
  else
  {
    pLevelMaskIter = pBaseXRef->createIterator();
  }

  if( !pLevelMaskIter.isNull() )
  {
    for(; !pLevelMaskIter->done(); pLevelMaskIter->step() )
    {
      OdDgElementPtr pLevelMaskElm = pLevelMaskIter->item().openObject(OdDg::kForRead);

      if( !pLevelMaskElm.isNull() && pLevelMaskElm->isKindOf(OdDgLevelMask::desc()) )
      {
        OdDgLevelMaskPtr pCurLevelMask = pLevelMaskElm;

        if( pCurLevelMask->getViewIndex() == uViewNumber )
        {
          bRet = true;
          pLevelMask = pCurLevelMask;
          break;
        }
      }
    }
  }

  return bRet;
}

//---------------------------------------------------------------------------------------------------

class OdClippingBoundaryCreate : public OdStaticRxObject<OdGiBaseVectorizer>
{
  class Loops : public OdGiGeometrySimplifier
  {
    public:

      void polylineProc(OdInt32 nbPoints, 
                          const OdGePoint3d* pVertexList,
                            const OdGeVector3d* = 0, 
                              const OdGeVector3d* = 0, 
                                OdGsMarker = 0
                        )
      {
        polylineOut(nbPoints, pVertexList);
      }

      void polylineOut(OdInt32 nPoints, const OdGePoint3d* pPoints)
      {
        if(nPoints > 1)
        {
          if(m_nCount && pPoints[0]==m_points.last() && pPoints[0]!=pPoints[nPoints-1])
          {
            ++pPoints;
            --nPoints;
            m_counts.last() += nPoints;
          }
          else
          {
            ++m_nCount;
            m_counts.append(nPoints);
          }

          m_points.insert(m_points.end(), pPoints, pPoints+nPoints);
        }
      }

      void meshProc(OdInt32 rows, OdInt32 columns,
                      const OdGePoint3d* pVertexList,
                        const OdGiEdgeData* /*pEdgeData*/,
                          const OdGiFaceData* /*pFaceData*/,
                            const OdGiVertexData* /*pVertexData*/
                    )
      {

        ODA_ASSERT(rows == 2 && columns == 2);
        OdGePoint3dArray points(5, 5);
        points.resize(5);
        OdGePoint3d *pArray = points.asArrayPtr();
        pArray[0] = pVertexList[0]; pArray[1] = pVertexList[1];
        pArray[2] = pVertexList[3]; pArray[3] = pVertexList[2];
        pArray[4] = pVertexList[0];
        polylineOut(5, pArray);
      }

      void shellProc( OdInt32 nbVertex, 
                        const OdGePoint3d* pVertexList, 
                          OdInt32 faceListSize,
                            const OdInt32* pFaceList, 
                              const OdGiEdgeData* /*pEdgeData*/, 
                                const OdGiFaceData* pFaceData,
                                  const OdGiVertexData* pVertexData)
      {
        setVertexData(nbVertex, pVertexList, pVertexData);
        generateShellWires(faceListSize, pFaceList, NULL, pFaceData);
      }

      int               m_nCount;
      OdIntArray        m_counts;
      OdGePoint3dArray  m_points;

      TD_USING(OdGiGeometrySimplifier::polylineOut);
  }

  m_loopsGen;

  OdSmartPtr<OdGiCommonDraw> m_pDeviation;

public:

  OdClippingBoundaryCreate()
  {
    m_loopsGen.setDrawContext(drawContext());
    output().setDestGeometry(m_loopsGen);
    SETBIT(m_flags, kDrawLayerOff | kDrawLayerFrozen, true);
  }

  bool buildBoundary(OdDbObjectId id)
  {
    m_loopsGen.m_nCount = 0;
    m_loopsGen.m_counts.clear();
    m_loopsGen.m_points.clear();
    OdDbObjectPtr pObj = id.openObject();
    OdDbEntityPtr pEnt = pObj;
    if(pEnt.get())
    {
      OdGeExtents3d extEntity;

      pEnt->getGeomExtents( extEntity );

      if( extEntity.isValidExtents() )
      {
        m_dDeviation = extEntity.maxPoint().distanceTo( extEntity.minPoint() ) / 1000.0;
      }
      else
      {
        m_dDeviation = 1e-6;
      }

      draw(pObj);
    }

    return (m_loopsGen.m_nCount!=0);
  }

  inline int loops() const
  {
    return m_loopsGen.m_nCount;
  }

  inline const int* counts() const
  {
    return m_loopsGen.m_counts.getPtr();
  }

  inline const OdGePoint3d* points() const
  {
    return m_loopsGen.m_points.getPtr();
  }

  double deviation(const OdGiDeviationType deviationType, const OdGePoint3d& pointOnCurve) const
  {
    return m_dDeviation;
  }

private:
  double m_dDeviation;
};

//---------------------------------------------------------------------------------------------------

void setViewportRefClipBoundary( const OdDbViewport* pViewport, 
                                   OdDbBlockReferencePtr& pViewportReference, 
                                     const OdGeMatrix3d& matMSToPS
                               )
{
  bool bClipViewport = false;

  OdGiClipBoundary viewportClip;

  viewportClip.m_bClippingBack  = false;
  viewportClip.m_bClippingFront = false;
  viewportClip.m_bDrawBoundary  = true;
  viewportClip.m_dBackClipZ     = 0;
  viewportClip.m_dFrontClipZ    = 0;
  viewportClip.m_ptPoint        = pViewport->centerPoint();
  viewportClip.m_vNormal        = OdGeVector3d::kZAxis;
  viewportClip.m_xInverseBlockRefXForm = OdGeMatrix3d::kIdentity;
  viewportClip.m_xToClipSpace   = matMSToPS;

  bool bCreateRectangleClip = true;

  if( pViewport->isNonRectClipOn() )
  {
    OdClippingBoundaryCreate cbClipBoundaryCreate;

    if( cbClipBoundaryCreate.buildBoundary(pViewport->nonRectClipEntityId()) )
    {
      OdGePoint3dArray arrClip3dPts;

      OdInt32 nPoints = cbClipBoundaryCreate.counts()[0];

      for( OdInt32 i = 0; i < nPoints; i++ )
      {
        OdGePoint3d ptClip = cbClipBoundaryCreate.points()[i];
        viewportClip.m_Points.push_back( ptClip.convert2d() );
      }

      bCreateRectangleClip = false;
      bClipViewport = true;
    }
  }

  if( bCreateRectangleClip )
  {
    OdGePoint3d ptCenter(pViewport->viewCenter().x, pViewport->viewCenter().y, 0.0);
    double dFieldHeight = pViewport->viewHeight() / 2.0;
    double dFieldWidth  = dFieldHeight * pViewport->width() / pViewport->height();

    OdGePoint3d ptClip = ptCenter - OdGeVector3d::kXAxis*dFieldWidth - OdGeVector3d::kYAxis*dFieldHeight;
    ptClip.transformBy(matMSToPS);
    viewportClip.m_Points.push_back( ptClip.convert2d());

    ptClip = ptCenter - OdGeVector3d::kXAxis*dFieldWidth + OdGeVector3d::kYAxis*dFieldHeight;
    ptClip.transformBy(matMSToPS);
    viewportClip.m_Points.push_back( ptClip.convert2d());

    ptClip = ptCenter + OdGeVector3d::kXAxis*dFieldWidth + OdGeVector3d::kYAxis*dFieldHeight;
    ptClip.transformBy(matMSToPS);
    viewportClip.m_Points.push_back( ptClip.convert2d());

    ptClip = ptCenter + OdGeVector3d::kXAxis*dFieldWidth - OdGeVector3d::kYAxis*dFieldHeight;
    ptClip.transformBy(matMSToPS);
    viewportClip.m_Points.push_back( ptClip.convert2d());

    bClipViewport = true;
  }

  if( bClipViewport )
  {
    if( pViewportReference->extensionDictionary().isNull() )
    {
      pViewportReference->createExtensionDictionary();
    }

    OdDbDictionaryPtr pDict = OdDbDictionary::cast(pViewportReference->extensionDictionary().openObject(OdDb::kForWrite));

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
          pSp->setDefinition( viewportClip );
        }
      }
    }
  }
}

//---------------------------------------------------------------------------------------------------

bool importDwgLayoutXRef( OdDbBlockTableRecord* owner, 
                            OdDgReferenceAttachmentHeader* pXRef,
                              OdDbDatabase* pXRefDb,
                                const OdString& xRefFileName,
                                  const OdString& strLayoutName, 
                                    OdGeMatrix3d& matTransform, 
                                      OdDbBlockTableRecordPtr& pXRefDef 
                         )
{
  bool bRet = false;

  OdDbDatabase* pDWGDb = owner->database();

  if( !pDWGDb || !pXRefDb )
  {
    return bRet;
  }

  pXRefDb->setCurrentLayout( strLayoutName );

  if( pXRefDb->getTILEMODE() )
  {
    return bRet;
  }

  OdDbObjectId idLayout = pXRefDb->currentLayoutId();

  if( idLayout.isNull() )
  {
    return bRet;
  }

  OdDbLayoutPtr pLayout = idLayout.openObject(OdDb::kForRead);

  if( pLayout.isNull() )
  {
    return bRet;
  }

  OdDbObjectId idLayoutBlock = pLayout->getBlockTableRecordId();

  if( idLayoutBlock.isNull() )
  {
    return bRet;
  }

  OdDbBlockTableRecordPtr pLayoutBlock = idLayoutBlock.openObject( OdDb::kForRead );

  if( pLayoutBlock.isNull() )
  {
    return bRet;
  }

  // Create block

  OdDbObjectId idBlockTable = pDWGDb->getBlockTableId();

  if( idBlockTable.isNull() )
  {
    return bRet;
  }

  OdDbBlockTablePtr pBlockTable = idBlockTable.openObject(OdDb::kForWrite);

  if( pBlockTable.isNull() )
  {
    return bRet;
  }

  OdString strDefNameBase = xRefFileName;
  strDefNameBase.replace(L'\\',L'/');

  if( strDefNameBase.reverseFind(L'/') != -1 )
  {
    strDefNameBase = strDefNameBase.right( strDefNameBase.getLength() - strDefNameBase.reverseFind(L'/') - 1 );
  }

  strDefNameBase = strDefNameBase.trimLeft();
  strDefNameBase = strDefNameBase.trimRight();

  if( strDefNameBase.getLength() > 4 && strDefNameBase[ strDefNameBase.getLength() - 4] == L'.' )
  {
    strDefNameBase = strDefNameBase.left( strDefNameBase.getLength() - 4 );
  }

  OdString strDefName = strDefNameBase;

  OdUInt32 uCount = 1;

  OdDgnImportContext::getCellBlockNextIndex(strDefName, uCount);

  while( strDefName.isEmpty() || !pBlockTable->getAt( strDefName).isNull() )
  {
    if( strDefNameBase.isEmpty() )
    {
      strDefName.format(L"AnonymusLayout%d", uCount );
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

  if( OdDbSymUtil::repairSymbolName(repairedName, strDefName, pDWGDb ) == eOk && !repairedName.isEmpty() )
  {
    strDefNameBase = repairedName;
    strDefName = strDefNameBase;
  }

  pXRefDef = OdDbBlockTableRecord::createObject();
  pXRefDef->setName( strDefName );

  pBlockTable->add( pXRefDef );

  // wBlockClone all objects of layout except viewports

  OdDbObjectIteratorPtr pIter = pLayoutBlock->newIterator();

  OdDbObjectIdArray arrItemsToClone;
  OdDbObjectIdArray arrViewportIds;

  for(; !pIter->done(); pIter->step() )
  {
    OdDbEntityPtr pItem = pIter->entity();

    if( pItem->isKindOf( OdDbViewport::desc()) )
    {
      arrViewportIds.push_back( pItem->objectId() );
      continue;
    }

    arrItemsToClone.push_back( pItem->objectId() );
  }

  OdDbIdMappingPtr pMap = OdDbIdMapping::createObject();
  pMap->setDestDb( pDWGDb );

  pXRefDb->wblockCloneObjects( arrItemsToClone, pXRefDef->objectId(), *pMap, OdDb::kDrcIgnore, false );

  // Convert viewports to XRefs

  for( OdUInt32 i = 0; i < arrViewportIds.size(); i++ )
  {
    if( arrViewportIds[i] == pLayout->overallVportId() )
    {
      continue;
    }

    OdDbViewportPtr pViewport =  arrViewportIds[i].openObject( OdDb::kForRead );

    OdGeMatrix3d matTransform = OdDbPointRef::mswcsToPswcs( pViewport );

    // Generate name for viewport

    OdString strViewportBlockName;
    strViewportBlockName.format(L"_viewport_%d", i);
    strViewportBlockName = strDefName + strViewportBlockName;

    if( OdDbSymUtil::repairSymbolName(repairedName, strViewportBlockName, pDWGDb ) == eOk && !repairedName.isEmpty() )
    {
      strViewportBlockName = repairedName;
    }

    OdDbBlockTableRecordPtr pViewportDef = OdDbXRefManExt::addNewXRefDefBlock(pDWGDb, xRefFileName, strViewportBlockName, false);

    OdDbBlockReferencePtr pViewportReference = OdDbBlockReference::createObject();
    pViewportReference->setDatabaseDefaults(pDWGDb);
    pXRefDef->appendOdDbEntity(pViewportReference);

    pViewportReference->setBlockTableRecord(pViewportDef->objectId());
    pViewportReference->setBlockTransform( matTransform );
    pViewportReference->setVisibility( pViewport->isOn() ? OdDb::kVisible : OdDb::kInvisible );

    // Init viewport clip

    setViewportRefClipBoundary( pViewport, pViewportReference, matTransform );
  }

  bRet = true;

  return bRet;
}
//---------------------------------------------------------------------------------------------------

void importDwgXRef( OdDbBlockTableRecord* owner, 
                      OdDgReferenceAttachmentHeader* pXRef, 
                        OdGeMatrix3d& matTransform, 
                          OdDbBlockReferencePtr& pBlockReference
                   )
{
  OdString databaseShortName = pXRef->getFileName();
  OdString databaseFullName  = pXRef->getFullFileName();

  OdDgDatabase* pDgnDb = pXRef->database();

  OdString xRefFileName;

  if( !databaseFullName.isEmpty() )
  {
    xRefFileName = pDgnDb->appServices()->findFile( databaseFullName, pDgnDb, OdDgHostAppServices::kXRefDrawing );
  }

  if( xRefFileName.isEmpty() )
  {
    xRefFileName = pDgnDb->appServices()->findFile( databaseShortName, pDgnDb, OdDgHostAppServices::kXRefDrawing );
  }

  OdString strFileExt = OdString::kEmpty;

  if( xRefFileName.getLength() > 4 )
  {
    strFileExt = xRefFileName.right(4);
    strFileExt.makeUpper();
  }

  bool bCorrectFile = !strFileExt.isEmpty() && ( (strFileExt == L".DWG") || (strFileExt == L".DXF"));

  if( bCorrectFile )
  {
    bool bCreateXRefToModelSpace = false;

    OdString strModelName = pXRef->getModelName();

    OdDbBlockTableRecordPtr pXRefDef;
    OdDbDatabase* pDwgDb = owner->database();

    OdDbDatabasePtr pXRefDb;

    if( strModelName.isEmpty() )
    {
      bCreateXRefToModelSpace = true;
    }
    else
    {
      pXRefDb = pDwgDb->appServices()->readFile(xRefFileName);
      bCreateXRefToModelSpace = !importDwgLayoutXRef( owner, pXRef, pXRefDb, xRefFileName, strModelName, matTransform, pXRefDef );
    }

    if( bCreateXRefToModelSpace )
    {
      OdDbObjectId idBlockDefTable = owner->database()->getBlockTableId();

      if( !idBlockDefTable.isNull() )
      {
        OdDbBlockTablePtr pBlockDefTable = idBlockDefTable.openObject(OdDb::kForWrite);

        OdDbSymbolTableIteratorPtr pBlockDefIter = pBlockDefTable->newIterator();

        for(; !pBlockDefIter->done(); pBlockDefIter->step() )
        {
          OdDbSymbolTableRecordPtr pRec = pBlockDefIter->getRecord(OdDb::kForRead);

          if( !pRec->isKindOf(OdDbBlockTableRecord::desc()) )
          {
            continue;
          }

          OdDbBlockTableRecordPtr pBlockDef = pRec;

          if( !pBlockDef.isNull() && pBlockDef->isFromExternalReference() && (pBlockDef->pathName() == xRefFileName) )
          {
            pXRefDef = pBlockDef;
            break;
          }
        }

        if( pXRefDef.isNull() )
        {
          OdString strXRefBlockNameBase = xRefFileName;

          OdUInt32 iPosition = strXRefBlockNameBase.find(L':');
          OdUInt32 iPositionSlash = strXRefBlockNameBase.reverseFind(L'\\');
          OdUInt32 iPositionBSlash = strXRefBlockNameBase.reverseFind(L'/');

          if( iPosition != -1 )
          {
            iPosition = strXRefBlockNameBase.reverseFind( L'\\' );

            if( iPosition != -1 )
              strXRefBlockNameBase.deleteChars( 0, iPosition + 1 );
            else
            {
              iPosition = strXRefBlockNameBase.reverseFind( L'/' );

              if( iPosition != -1 )
                strXRefBlockNameBase.deleteChars( 0, iPosition + 1 );
            }
          }
          else if( iPositionSlash != -1 )
            strXRefBlockNameBase.deleteChars( 0, iPositionSlash + 1 );
          else if( iPositionBSlash != -1 )
            strXRefBlockNameBase.deleteChars( 0, iPositionBSlash + 1 );

          if( strXRefBlockNameBase.getLength() > 4 && (strXRefBlockNameBase[strXRefBlockNameBase.getLength() - 4] == OdChar('.')) )
          {
            strXRefBlockNameBase = strXRefBlockNameBase.left( strXRefBlockNameBase.getLength() - 4 );
          }

          OdString strXRefBlockName = strXRefBlockNameBase;
          OdUInt32 uCount           = 1;

          while( !pBlockDefTable->getAt( strXRefBlockName ).isNull() )
          {
            strXRefBlockName.format(L"_%d", uCount );
            strXRefBlockName = strXRefBlockNameBase + strXRefBlockName;
          }

          OdString repairedName;

          if( OdDbSymUtil::repairSymbolName(repairedName, strXRefBlockName, pDwgDb ) == eOk && !repairedName.isEmpty() )
            strXRefBlockName = repairedName;

          pXRefDef = OdDbXRefManExt::addNewXRefDefBlock(pDwgDb, xRefFileName, strXRefBlockName, false);
        }
      }
    }

    if( !pXRefDef.isNull() )
    {
      pBlockReference = OdDbBlockReference::createObject();
      pBlockReference->setDatabaseDefaults(pDwgDb);
      owner->appendOdDbEntity(pBlockReference);

      pBlockReference->setBlockTableRecord(pXRefDef->objectId());

      // Calculate scale

      OdDgReferenceAttachmentHeader::OdDgReferenceForeignUnits iDefaultUnits = pXRef->getForeignUnits();
      double dDefaultUnitScale = OdDgReferenceAttachmentHeader::getForeignUnitsScale( iDefaultUnits );

      double dScale = dDefaultUnitScale;

      if( iDefaultUnits == OdDgReferenceAttachmentHeader::kDesignCenterUnits )
      { 
        if( pXRefDb.isNull() )
        {
          pXRefDb = pDwgDb->appServices()->readFile(xRefFileName, false, true);
        }

        OdDb::UnitsValue iBaseUnit = OdDb::kUnitsUndefined;

        if( !pXRefDb.isNull() )
        {
          iBaseUnit = pXRefDb->getINSUNITS();
        }

        if( iBaseUnit <= OdDb::kUnitsUndefined || iBaseUnit > OdDb::kUnitsMax )
        {
          dScale = dDefaultUnitScale;
        }
        else
        {
          dScale = oddbGetUnitsConversion( iBaseUnit, OdDb::kUnitsMeters );
        }
      }

      OdDgModelPtr pXRefOwnerModel;

      OdDgElementId idXRefOwner = pXRef->ownerId();

      if( !idXRefOwner.isNull() )
      {
        OdDgElementPtr pOwner = idXRefOwner.openObject(OdDg::kForRead);

        if( pOwner->isKindOf(OdDgModel::desc() ) )
        {
          pXRefOwnerModel = pOwner;
        }
      }

      if( !pXRefOwnerModel.isNull() )
      {
        OdDgModel::WorkingUnit coincidedUnit;
        coincidedUnit = OdDgModel::kWuWorldUnit;

        dScale *= pXRefOwnerModel->getMeasuresConversion( coincidedUnit, pXRefOwnerModel->getWorkingUnit() );
        dScale *= pXRef->getScale();
      }

      //

      matTransform = OdGeMatrix3d::translation( -pXRef->getReferenceOrigin().asVector() );
      matTransform = OdGeMatrix3d::scaling( dScale ) * matTransform;
      matTransform = pXRef->getTransformation() * matTransform;
      matTransform = OdGeMatrix3d::translation( pXRef->getMasterOrigin().asVector() ) * matTransform;
    }
  }
}

//---------------------------------------------------------------------------------------------------

void importDgnUnderlay( OdDbBlockTableRecord* owner, 
                          OdDgReferenceAttachmentHeader* pXRef, 
                            OdGeMatrix3d& matTransform, 
                              OdDbBlockReferencePtr& pBlockReference
                      )
{
  OdString databaseShortName = pXRef->getFileName();
  OdString databaseFullName  = pXRef->getFullFileName();
  OdString modelName         = pXRef->getModelName();

  OdDgModelPtr pModel = pXRef->getReferencedModel();

  if( !pModel.isNull() )
  {
    modelName        = pModel->getName();
    databaseFullName = pModel->database()->getFilename();
  }

  OdDgDatabase* pDgnDb = pXRef->database();

  OdString xRefFileName;

  if( !databaseFullName.isEmpty() )
  {
    xRefFileName = pDgnDb->appServices()->findFile( databaseFullName, pDgnDb, OdDgHostAppServices::kXRefDrawing );
  }

  if( xRefFileName.isEmpty() )
  {
    xRefFileName = pDgnDb->appServices()->findFile( databaseShortName, pDgnDb, OdDgHostAppServices::kXRefDrawing );
  }

  OdDbObjectId idDgnDef;

  OdDbDatabase* pDwgDb = owner->database();

  OdDbDictionaryPtr pDict = pDwgDb->getNamedObjectsDictionaryId().safeOpenObject();
  OdDbObjectId idDefDict  = pDict->getAt(OdDbDgnDefinition::dictionaryKey( OdDbDgnDefinition::desc()));

  if(!idDefDict.isNull())
  {
    OdDbDictionaryPtr pDgnUnderlayDict = idDefDict.openObject(OdDb::kForRead);

    if( !pDgnUnderlayDict.isNull() )
    {
      OdDbDictionaryIteratorPtr pIter = pDgnUnderlayDict->newIterator();

      for(; !pIter->done(); pIter->next() )
      {
        OdDbDgnDefinitionPtr pCurDgnUnderlay = pIter->object();

        if( (pCurDgnUnderlay->getSourceFileName() == xRefFileName) && (pCurDgnUnderlay->getItemName() == modelName) )
        {
          idDgnDef = pCurDgnUnderlay->objectId();
          break;
        }
      }
    }
  }

  if( idDgnDef.isNull() )
  {
    OdDbDgnDefinitionPtr pDgnDef = OdDbDgnDefinition::createObject();
    pDgnDef->setSourceFileName(xRefFileName);
    pDgnDef->setItemName(modelName);

    // Post to database

    OdUInt32 uIndex = 0;

    for (;;)
    {
      try
      {
        OdString sName;
        sName.format(L"dgnUnderlay_%d", uIndex++);

        OdDbObjectId idDef = pDgnDef->postDefinitionToDb(owner->database(), sName);

        if( !idDef.isNull() )
        {
          idDgnDef = idDef;
        }

        break;
      }
      catch (const OdError )
      {
      }
    }
  }

  if( !idDgnDef.isNull() )
  {
    double dModelMeasuresConversion = 1.0;
    double dEntireScale = 1.0;

    if( !pModel.isNull() )
    {
      dModelMeasuresConversion = pModel->getMeasuresConversion( OdDgModel::kWuUnitOfResolution, pModel->getWorkingUnit() );
      dEntireScale = pXRef->getEntireScale();
    }

    matTransform = OdGeMatrix3d::translation( -pXRef->getReferenceOrigin().asVector() * dModelMeasuresConversion );
    matTransform = OdGeMatrix3d::scaling( dEntireScale ) * matTransform;
    matTransform = pXRef->getTransformation() * matTransform;
    matTransform = OdGeMatrix3d::translation( pXRef->getMasterOrigin().asVector() ) * matTransform;

    OdDbDgnReferencePtr pDgnRef = OdDbDgnReference::createObject();
    pDgnRef->setDatabaseDefaults(owner->database());
    owner->appendOdDbEntity(pDgnRef);
    pDgnRef->setDefinitionId(idDgnDef);
    pDgnRef->setTransform( matTransform );
  }
}

//---------------------------------------------------------------------------------------------------

void importDgnXRef( OdDbBlockTableRecord* owner, 
                      OdDgReferenceAttachmentHeader* pXRef, 
                        OdGeMatrix3d& matTransform, 
                          OdDbBlockReferencePtr& pBlockReference,
                            OdUInt8 uXRefImportMode
                   )
{
  OdDgModelPtr pModel  = pXRef->getReferencedModel();
  OdString strFilename = pModel->database()->getFilename();

  if( uXRefImportMode == 3 ) // Create dgn underlay
  {
    importDgnUnderlay( owner, pXRef, matTransform, pBlockReference );

    return;
  }

  // Try to find model of x-ref into map

  OdString strGroupName = pXRef->getNamedGroupName();

  if( !strGroupName.isEmpty() )
  {
    strFilename += strGroupName;
  }

  DgnImporter* pImport = OdDgnImportContext::getDgnImporter();

  OdRxVariantValue tmpVar = (OdRxVariantValue)pImport->properties()->getAt("ImportViewIndex");
  OdInt8 uImportViewNumber = tmpVar->getInt8();

  OdString strLevelMaskExt = OdString::kEmpty;

  if( uXRefImportMode == 2 && OdDgnImportContext::isLevelMask() )
  {
    // Set Level Mask
    OdDgLevelMaskPtr pLevelMask;

    if( getActualXRefLevelMask( pXRef, uImportViewNumber, pLevelMask ) )
    {
      uImportViewNumber = -1;
    }

    OdDgDatabasePtr pXRefDb = pXRef->getReferencedDatabase();

    if( !pXRefDb.isNull() )
    {
      OdDgnImportContext::pushLevelMask( pLevelMask, pXRefDb->getLevelTable( OdDg::kForRead) );
    }
    else
    {
      OdDgnImportContext::pushLevelMask( pLevelMask, pXRef->getLevelTable( OdDg::kForRead) );
    }

    strLevelMaskExt = OdDgnImportContext::getLevelMaskString();
  }

  OdDbObjectId idBlockDefinition = OdDgnImportContext::getDwgModelBlockId(strFilename, pModel->elementId(), strLevelMaskExt );

  if( idBlockDefinition.isNull() ) // Import model of x-ref as block definition
  {
    OdDbObjectId idBlockDefTable = owner->database()->getBlockTableId();

    if( !idBlockDefTable.isNull() )
    {
      OdDbBlockTablePtr pBlockDefTable = idBlockDefTable.openObject(OdDb::kForWrite);

      OdString strBlockDefNameBase = pModel->getName();

      strBlockDefNameBase = strBlockDefNameBase.trimLeft();
      strBlockDefNameBase = strBlockDefNameBase.trimRight();

      OdString strBlockDefName = strBlockDefNameBase;

      OdUInt32 uCount = 1;

      while( !pBlockDefTable->getAt( strBlockDefName ).isNull() )
      {
        if( strBlockDefNameBase.isEmpty() )
        {
          strBlockDefName.format(L"DgnModel_%d",uCount );
        }
        else
        {
          strBlockDefName.format(L"_%d",uCount );
          strBlockDefName = strBlockDefNameBase + strBlockDefName;
        }

        uCount++;
      }

      OdString repairedName;

      if( OdDbSymUtil::repairSymbolName(repairedName, strBlockDefName, owner->database() ) == eOk && !repairedName.isEmpty() )
      {
        strBlockDefNameBase = repairedName;
        strBlockDefName = strBlockDefNameBase;
      }

      OdDgnImportContext::pushXRef(strFilename, pModel->elementId(), pXRef->elementId(), pXRef->getNestDepth(), strLevelMaskExt );

      bool bRestorePalette = false;

      OdDgDatabase* pDb = pModel->database();

      // Correct color table

      if( pDb && (pDb->getOriginalFileVersion() > 7 || pDb->getColorTable(OdDg::kForRead).isNull()) )
      {
        ODCOLORREF pXRefPalette[256];

        if( getXRefPalette( pXRef, pDb, pXRefPalette ) )
        {
          OdDgnImportContext::pushPalette();
          bRestorePalette = true;

          OdDgnImportContext::setPalette( pXRefPalette );
        }
      }

      // Set global line scale

      OdDgModelPtr pOwnerModel;

      OdDgElementId idOwner = pXRef->ownerId();

      while( !idOwner.isNull() )
      {
        OdDgElementPtr pOwner = idOwner.openObject(OdDg::kForRead);

        if( pOwner->isKindOf(OdDgModel::desc()))
        {
          pOwnerModel = pOwner;
          break;
        }

        idOwner = pOwner->ownerId();
      }

      double dLineStyleScale = 1.0;

      bool bUpdateGlobalLineScaleFlag = false;

      if( !pModel.isNull() && !pOwnerModel.isNull() )
      {
        double dCurLineStyleScale = 1.0;

        if( !OdDgnImportContext::getGlobalLineScale( dCurLineStyleScale ) )
        {
          dCurLineStyleScale = pOwnerModel->getLinestyleScale();
        }

        double dAssocLineStyleScale = pModel->getLinestyleScale();

        if( pXRef->getLineStyleScaleFlag() )
        {
          if( OdZero(dAssocLineStyleScale) )
          {
            dAssocLineStyleScale = pModel->database()->getLineStyleScale();
          }

          if( OdZero(dAssocLineStyleScale) )
          {
            dAssocLineStyleScale = 1.0;
          }

          dLineStyleScale *= dAssocLineStyleScale / pXRef->getScale();
        }
        else
        {
          switch( pXRef->getLineStyleScaleMode() )
          {
            case OdDgReferenceAttachmentHeader::kLSMaster:
            {
              dLineStyleScale *= dCurLineStyleScale;
            }; break;

            case OdDgReferenceAttachmentHeader::kLSReference:
            {
              dLineStyleScale *= dAssocLineStyleScale;
            }; break;

            case OdDgReferenceAttachmentHeader::kLSMasterReference:
            {
              dLineStyleScale *= dCurLineStyleScale * dAssocLineStyleScale;
            }; break;
          }
        }

        OdDgnImportContext::pushGlobalLineScale( dLineStyleScale );
        bUpdateGlobalLineScaleFlag = true;
      }

      if( (uXRefImportMode == 1) && (pModel->database() != pOwnerModel->database() ) && strGroupName.isEmpty() ) // Retain X-Ref
      {
        if( pImport )
        {
          OdString        strNewFileName;

          if( pDb )
          {
            strNewFileName = pDb->getFilename();

            if( strNewFileName.getLength() > 3 )
            {
              strNewFileName = strNewFileName.left( strNewFileName.getLength() - 4);
            }

            strNewFileName += L".dwg";
          }

          OdRxVariantValue tmpVar = (OdRxVariantValue)pImport->properties()->getAt("Services");

          ExHostAppServices* pHostAppServices = static_cast<ExHostAppServices*>(((OdRxObjectPtr)(tmpVar)).get());

          bool bXRefPresent = false;

          tmpVar = (OdRxVariantValue)pImport->properties()->getAt("ExplodeTextNodes");

          bool bExplodeTextNodes = tmpVar;

          tmpVar = (OdRxVariantValue)pImport->properties()->getAt("DontImportInvisibleElements");

          bool bDontImportInvisible = tmpVar;

          tmpVar = (OdRxVariantValue)pImport->properties()->getAt("ImportViewIndex");

          OdInt8 iImportViewIndex = tmpVar->getInt8();

          if( pHostAppServices )
          {
            OdString strXRefName = pHostAppServices->findFile(strNewFileName);

            if( !strXRefName.isEmpty() )
            {
              bXRefPresent   = true;
              strNewFileName = strXRefName;
            }
          }
          
          OdDgnImportLineWeightsMapPtr pLWMap = pImport->properties()->getAt("LineWeightsMap");

          bool bSkipXRef = false;

          if( !bXRefPresent )
          {
            OdDgnImportModulePtr pModule = ::odrxDynamicLinker()->loadApp(OdDgnImportModuleName, false);
            OdDgnImportPtr importer = pModule->create();

            importer->properties()->putAt( L"Services", pHostAppServices );
            importer->properties()->putAt( L"DgnPath", OdRxVariantValue(pDb->getFilename()) );
            importer->properties()->putAt( L"XRefImportMode", OdRxVariantValue(OdUInt8(uXRefImportMode)) );
            importer->properties()->putAt( L"ExplodeTextNodes", OdRxVariantValue(bExplodeTextNodes) );
            importer->properties()->putAt( L"ImportViewIndex", OdRxVariantValue(iImportViewIndex) );
            importer->properties()->putAt( L"DontImportInvisibleElements", OdRxVariantValue(bDontImportInvisible) );
            importer->properties()->putAt( L"3dEllipseImportMode", OdRxVariantValue( OdDgnImportContext::getEllipse3dImportMode()) );
            importer->properties()->putAt( L"2dEllipseImportMode", OdRxVariantValue( OdDgnImportContext::getEllipse2dImportMode()) );
            importer->properties()->putAt( L"3dShapeImportMode", OdRxVariantValue( OdDgnImportContext::getShape3dImportMode()) );
            importer->properties()->putAt( L"2dShapeImportMode", OdRxVariantValue( OdDgnImportContext::getShape2dImportMode()) );
            importer->properties()->putAt( L"3dObjectImportMode", OdRxVariantValue( OdDgnImportContext::getObject3dImportMode()) );
            importer->properties()->putAt( L"3dClosedBSplineCurveImportMode", OdRxVariantValue(OdDgnImportContext::getClosedBSplineCurve3dImportMode()) );
            importer->properties()->putAt( L"2dClosedBSplineCurveImportMode", OdRxVariantValue(OdDgnImportContext::getClosedBSplineCurve2dImportMode()) );
            importer->properties()->putAt( L"ConvertDgnColorIndicesToTrueColors", OdRxVariantValue(OdDgnImportContext::getConvertColorIndexToTrueColorFlag()) );
            importer->properties()->putAt( L"ConvertEmptyDataFieldsToSpaces", OdRxVariantValue(OdDgnImportContext::getConvertEmptyDataFieldsToSpacesFlag()) );

            OdDgnImportLineWeightsMapPtr pLineWeightsMap = importer->properties()->getAt(L"LineWeightsMap");
            pLineWeightsMap->copyLineWeightsMap(pLWMap);

            OdDgnImportContext::setInternalImportFlag( true );

            OdDgnImport::ImportResult res = importer->import();

            OdDgnImportContext::setInternalImportFlag( false );

            pLineWeightsMap = 0;

            OdDbDatabasePtr pXRefDb;

            if( res == OdDgnImport::success )
            {
              pXRefDb = importer->properties()->getAt(L"Database");
            }

            if( !pXRefDb.isNull() )
            {
              pXRefDb->writeFile(strNewFileName, OdDb::kDwg, OdDb::kDHL_CURRENT);
            }
            else
            {
              bSkipXRef = true;
            }
          }

          pLWMap = 0;

          OdDbObjectId idBlockDefTable = owner->database()->getBlockTableId();

          if( !idBlockDefTable.isNull() && !bSkipXRef )
          {
            OdDbBlockTablePtr pBlockDefTable = idBlockDefTable.openObject(OdDb::kForWrite);
            OdDbDatabase* pDwgDb = owner->database();

            OdDbBlockTableRecordPtr pXRefDef;

            OdDbSymbolTableIteratorPtr pBlockDefIter = pBlockDefTable->newIterator();

            for(; !pBlockDefIter->done(); pBlockDefIter->step() )
            {
              OdDbBlockTableRecordPtr pBlockDef = pBlockDefIter->getRecord(OdDb::kForRead);

              if( !pBlockDef.isNull() && pBlockDef->isFromExternalReference() && (pBlockDef->pathName() == strNewFileName) )
              {
                pXRefDef = pBlockDef;
                break;
              }
            }

            if( pXRefDef.isNull() )
            {
              OdString strXRefBlockNameBase = strNewFileName;

              OdUInt32 iPosition = strXRefBlockNameBase.find(L':');
              OdUInt32 iPositionSlash = strXRefBlockNameBase.reverseFind(L'\\');
              OdUInt32 iPositionBSlash = strXRefBlockNameBase.reverseFind(L'/');

              if( iPosition != -1 )
              {
                iPosition = strXRefBlockNameBase.reverseFind( L'\\' );

                if( iPosition != -1 )
                  strXRefBlockNameBase.deleteChars( 0, iPosition + 1 );
                else
                {
                  iPosition = strXRefBlockNameBase.reverseFind( L'/' );

                  if( iPosition != -1 )
                    strXRefBlockNameBase.deleteChars( 0, iPosition + 1 );
                }
              }
              else if( iPositionSlash != -1 )
                strXRefBlockNameBase.deleteChars( 0, iPositionSlash + 1 );
              else if( iPositionBSlash != -1 )
                strXRefBlockNameBase.deleteChars( 0, iPositionBSlash + 1 );

              if( strXRefBlockNameBase.getLength() > 4 && (strXRefBlockNameBase[strXRefBlockNameBase.getLength() - 4] == L'.') )
              {
                strXRefBlockNameBase = strXRefBlockNameBase.left( strXRefBlockNameBase.getLength() - 4 );
              }

              OdString strXRefBlockName = strXRefBlockNameBase;
              OdUInt32 uCount           = 1;

              while( !pBlockDefTable->getAt( strXRefBlockName ).isNull() )
              {
                strXRefBlockName.format(L"_%d", uCount );
                strXRefBlockName = strXRefBlockNameBase + strXRefBlockName;
              }

              if( OdDbSymUtil::repairSymbolName(repairedName, strXRefBlockName, pDwgDb ) == eOk && !repairedName.isEmpty() )
                strXRefBlockName = repairedName;

              pXRefDef = OdDbXRefManExt::addNewXRefDefBlock(pDwgDb, strNewFileName, strXRefBlockName, false);
            }

            idBlockDefinition = pXRefDef->objectId();
          }
        }
      }
      else
      {
        // Prepare level table and copy x-ref blocks

        if( pImport )
        {
          OdDgDatabase* pDgnDb = pModel->database();
          OdDbDatabase* pDwgDb = owner->database();

          // Prepare importer and context to XRef import
          pImport->pushIdMap();
          OdDgnImportContext::prepareToImport(pImport, true);

          pImport->copyTextStyles(pDgnDb, pDwgDb );
          pImport->copyLineStyles(pDgnDb, pDwgDb );
          pImport->copyLayers( pDgnDb, pDwgDb, true);
          pImport->copyBlocks(pDgnDb, pDwgDb );
        }

        // Import model to block definition

        OdDbBlockTableRecordPtr pBlockDef = OdDbBlockTableRecord::createObject();
        pBlockDef->setName( strBlockDefName );
        pBlockDefTable->add( pBlockDef );

        if( strGroupName.isEmpty() )
        {
          DgnImporter::copySpaceBlock( pModel, pBlockDef, true, uImportViewNumber );
        }
        else
        {
          DgnImporter::copyNamedGroup( pModel, strGroupName, pBlockDef, uImportViewNumber );
        }

        if( pImport )
        {
          // Update dimension association and  restore importer and context.
          OdRxVariantValue tmpVar = (OdRxVariantValue)pImport->properties()->getAt("BreakDimensionAssociation");

          if( !tmpVar->getBool() )
          {
            pImport->setDimensionAssociation( pModel->database(), owner->database());
          }
          else
          {
            OdDgnImportContext::clearDimAssocDescriptors();
          }

          OdDgnImportContext::restoreAfterImport();
          pImport->popIdMap();
        }

        idBlockDefinition = pBlockDef->objectId();
      }

      // Restore data

      OdDgnImportContext::popXRef();

      if( bUpdateGlobalLineScaleFlag )
      {
        OdDgnImportContext::popGlobalLineScale();
      }

      if( bRestorePalette )
      {
        OdDgnImportContext::popPalette();
      }

      OdDgnImportContext::addDwgModelBlockId(strFilename, pModel->elementId(), idBlockDefinition);
    }
  }

  if( uXRefImportMode == 2 && OdDgnImportContext::isLevelMask() )
  {
    // Restore Level Mask
    OdDgnImportContext::popLevelMask();
  }

  if( !idBlockDefinition.isNull() )
  {
    matTransform = OdGeMatrix3d::translation( -pXRef->getReferenceOrigin().asVector()
      * pModel->getMeasuresConversion( OdDgModel::kWuUnitOfResolution, pModel->getWorkingUnit() ) );
    matTransform = OdGeMatrix3d::scaling( pXRef->getEntireScale() ) * matTransform;
    matTransform = pXRef->getTransformation() * matTransform;
    matTransform = OdGeMatrix3d::translation( pXRef->getMasterOrigin().asVector() ) * matTransform;

    pBlockReference = OdDbBlockReference::createObject();
    pBlockReference->setBlockTableRecord( idBlockDefinition );
    owner->appendOdDbEntity( pBlockReference );
  }
}

//---------------------------------------------------------------------------------------------------

bool OdDgReferenceAttachmentImportPE::allowImportElement(OdDgElement* e)
{
  OdDgReferenceAttachmentHeader* pXRef = (OdDgReferenceAttachmentHeader*)(e);

  bool bRet = true;

  if( !OdDgnImportContext::isImportInvisible() && OdDgnImportContext::isLevelMask() )
  {
    OdUInt32 uLevelId = pXRef->getLevelEntryId();

    if( (uLevelId != 0) && !pXRef->getViewportFlag() )
    {
      OdDgElementId levelId = pXRef->getLevelId();
      levelId.convertToRedirectedId();

      OdDgLevelTableRecordPtr pLevel = levelId.openObject(OdDg::kForRead);

      if( !pLevel.isNull() )
      {
        if( !pLevel->getIsDisplayedFlag() || pLevel->getIsFrozenFlag() )
        {
          bRet = false;
        }
        else
        {
          bRet = OdDgnImportContext::checkLevelMask( pLevel->getEntryId() );
        }
      }
    }
  }

  return bRet;
}

//---------------------------------------------------------------------------------------------------

void OdDgReferenceAttachmentImportPE::subImportElement(OdDgElement* e, OdDbBlockTableRecord* owner )
{
  OdDgReferenceAttachmentHeader* pXRef = (OdDgReferenceAttachmentHeader*)(e);

  OdUInt8 uXRefMode = 2;

  DgnImporter* pImport = OdDgnImportContext::getDgnImporter();

  if( pImport )
  {
    OdRxVariantValue tmpVar = (OdRxVariantValue)pImport->properties()->getAt("XRefImportMode");
    uXRefMode = tmpVar->getUInt8();
  }

  if( !pXRef || !owner )
  {
    return;
  }

  if( pXRef->getDoNotDisplayAsNestedFlag() &&  (OdDgnImportContext::getXRefNestedDepth() > 0) )
  {
    return;
  }

  OdDgModelPtr pModel = pXRef->getReferencedModel();
  OdDbBlockReferencePtr pBlockReference;
  OdGeMatrix3d matTransform = OdGeMatrix3d::kIdentity;

  if( !pModel.isNull() ) // dgn model
  {
    OdString strFilename = pModel->database()->getFilename();

    // Check visibility of x-ref

    if( OdDgnImportContext::isXRefInStack(strFilename, pModel->elementId()) )
    {
      return;
    }

    if( !OdDgnImportContext::checkNestedDepth() )
    {
      return;
    }

    importDgnXRef( owner, pXRef, matTransform, pBlockReference, uXRefMode );

  }
  else // may be dwg model
  {
    importDwgXRef( owner, pXRef, matTransform, pBlockReference );
  }

  if( !pBlockReference.isNull() )
  {
    // layer
    if( pXRef->getLevelEntryId() == 0 )
    {
      pBlockReference->setLayer(owner->database()->getLayerZeroId());
    }
    else
    {
      OdDbObjectId layerId = OdDgnImportContext::getObjectId(pXRef->getLevelId());

      if( layerId.isNull() || OdDbLayerTableRecord::cast(layerId.openObject()).isNull() ) // is used in audit checking also
      {
        layerId = owner->database()->getLayerZeroId();
      }

      pBlockReference->setLayer(layerId);
    }

    // transform
    pBlockReference->setBlockTransform( matTransform );

    // visibility
    pBlockReference->setVisibility( pXRef->getDisplayFlag() ? OdDb::kVisible : OdDb::kInvisible );

    // transparency
    pBlockReference->setTransparency( pXRef->getTransparency() );

    // clip
    OdGiClipBoundary clipBoundary;
    bool useClipBoundary = false;
    bool bInverceClip = false;

    OdDgReferenceAttachClipBoundaryPEPtr pBoundaryPE = OdDgReferenceAttachClipBoundaryPEPtr(OdRxObjectPtr(pXRef));

    if( !pBoundaryPE.isNull() )
    {
      useClipBoundary = pBoundaryPE->extractBoundary( pXRef, clipBoundary, 1e-6, false, NULL, bInverceClip, true, matTransform, true );
    }

    if( useClipBoundary )
    {
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

            if( bInverceClip)
              pSp->setFilterInverted(bInverceClip);
          }
        }
      }
    }
  }
}

//---------------------------------------------------------------------------------------------------

}
