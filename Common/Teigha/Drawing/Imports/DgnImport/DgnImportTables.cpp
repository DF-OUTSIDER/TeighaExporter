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
#include <RxDispatchImpl.h>
#include <RxVariantValue.h>
#include <DbDictionary.h>
#include <RxDynamicModule.h>
#include "DgnImportImpl.h"
#include <DgDatabase.h>
#include <DgTable.h>
#include <DgLevelTableRecord.h>
#include <DbLayerTableRecord.h>
#include <DgLineStyleTableRecord.h>
#include <DbLayerTable.h>
#include <DgLineStyleDefTableRecord.h>
#include <DbLinetypeTableRecord.h>
#include <DgRasterAttach.h>
#include <DgReferenceAttach.h>
#include <DbBlockTableRecord.h>
#include <DbBlockTable.h>
#include <DgRegAppTableRecord.h>
#include <DbRegAppTable.h>
#include <DbRegAppTableRecord.h>
#include <Gi/GiTextStyle.h>
#include <DgTextStyleTableRecord.h>
#include <DbTextStyleTableRecord.h>
#include <DbTextStyleTable.h>
#include <DbViewTableRecord.h>
#include <DbViewTable.h>
#include <ColorMapping.h>
#include <DgFontTableRecord.h>
#include "DgnImportLS.h"
#include "DbSymUtl.h"
#include <DbViewportTableRecord.h>
#include <DbViewportTable.h>
#include <Ge/GePlane.h>
#include <DbEntity.h>
#include <DgNamedGroup.h>
#include "DgnImportCommon.h"
#include "DgnImportContext.h"
#include "DgnImportPE.h"
#include <DbDimension.h>
#include "DgMultilineStyleTableRecord.h"
#include <DbMlineStyle.h>
#include "Gi/GiMaterial.h"
#include "DgDisplayStyle.h"
#include <DbVisualStyle.h>
#include <DbDimStyleTable.h>

namespace TD_DGN_IMPORT {

// From DgnImportElements.cpp
OdDb::LineWeight odDgnImportLineweightMapping(OdUInt32 mappingVal);

void DgnImporter::copyLayerProperties(OdDgLevelTableRecord* l, OdDbLayerTableRecord* l_d)
{
  l_d->setLineWeight(odDgnImportLineweightMapping(l->getElementLineWeight()));
  {
    OdUInt32 entryId = l->getElementLineStyleEntryId();
    if (entryId < 8 && entryId >= 0)
    {
      // internal
      if (entryId == 0)
        l_d->setLinetypeObjectId(l_d->database()->getLinetypeContinuousId());
      else
        l_d->setLinetypeObjectId(OdDbLinetypeTable::cast(l_d->database()->getLinetypeTableId().openObject())->getAt(OdString().format(OD_T("DGN%u"), entryId)));
    }
    else if (entryId == OdDg::kLineStyleByLevel)
      l_d->setLinetypeObjectId(l_d->database()->getLinetypeByLayerId());
    else if (entryId == OdDg::kLineStyleByCell)
      l_d->setLinetypeObjectId(l_d->database()->getLinetypeByBlockId());
    else
    {
      OdDgnImportPathToDwgObject dwgPath = _idMap[l->database()->getLineStyleTable()->getAt(l->getElementLineStyleEntryId())];

      if( !dwgPath.m_idPath.objectIds().isEmpty() )
      {
        setResourceUsage(dwgPath.m_idPath.objectIds()[0]);
        l_d->setLinetypeObjectId( dwgPath.m_idPath.objectIds()[0] );
      }
      else
        l_d->setLinetypeObjectId( 0 );
    }
  }
  l_d->setIsFrozen(l->getIsFrozenFlag());
  l_d->setIsOff(!l->getIsDisplayedFlag());
  l_d->setIsLocked( l->getElementAccess() == OdDgLevelTableRecord::kAccessLocked );
  l_d->setIsPlottable(l->getIsPlotFlag());
  OdUInt32 color = l->getElementColorIndex();
  ODCOLORREF c = OdDgColorTable::lookupRGB(l->database(), color);

  bool bSetTrueColor = true;

  if( color < 256 && !OdDgnImportContext::getConvertColorIndexToTrueColorFlag() )
  {
    OdDgnImportColorIndexDescription dwgColor;

    if( OdDgnImportContext::getDwgColor( color, dwgColor ) )
    {
      if( !dwgColor.m_bUseTrueColor )
      {
        bSetTrueColor = false;
        l_d->setColorIndex(dwgColor.m_uDwgColorIndex);
      }
    }
  }

  if( bSetTrueColor )
  {
    OdCmColor cc;
    cc.setRGB(ODGETRED(c), ODGETGREEN(c), ODGETBLUE(c));
    l_d->setColor(cc);
  }

  OdCmTransparency transparency = l->getTransparency();

  if( transparency.alphaPercent() != 0 )
  {
    l_d->setTransparency( transparency );
  }

  l_d->setDescription(l->getDescription());
  
  if( !l->getByLevelMaterial().isNull() )
    l_d->setMaterialId( OdDgnImportContext::getObjectId(l->getByLevelMaterial()) );
}

void convertDgnTextureMappingToDwgTextureMapping( OdGiMaterialMap& matMapping )
{
  if( matMapping.source() == OdGiMaterialMap::kFile )
  {
    OdGiMapper& texMapper = matMapping.mapper();

    if( texMapper.projection() == OdGiMapper::kDgnParametric || texMapper.projection() == OdGiMapper::kDgnPlanar )
    {
      texMapper.setProjection( OdGiMapper::kPlanar );
      matMapping.setMapper( texMapper );
    }
    else if( texMapper.projection() == OdGiMapper::kDgnSphere )
    {
      texMapper.setProjection( OdGiMapper::kSphere );
      matMapping.setMapper( texMapper );
    }
    else if( texMapper.projection() == OdGiMapper::kDgnCylinder || texMapper.projection() == OdGiMapper::kDgnCylinderCapped )
    {
      texMapper.setProjection( OdGiMapper::kCylinder );
      matMapping.setMapper( texMapper );
    }
  }
}

bool DgnImporter::copyMaterialProperties( const OdDgMaterialTableRecordPtr& pDgMaterial, 
                                            OdDbMaterialPtr& pDbMaterial,
                                              const OdDbDictionaryPtr& pMatDictionary
                                        )
{
  if( pDgMaterial.isNull() || pDbMaterial.isNull() || pMatDictionary.isNull() )
  {
    return false;
  }

  // Material name

  OdString strMaterialName = pDgMaterial->getName();

  if( strMaterialName.isEmpty() )
  {
    OdUInt32 uIndexMat = 1;

    do
    {
      strMaterialName.format(L"DgnMaterial_%d",uIndexMat);
      uIndexMat++;
    }
    while( pMatDictionary->has(strMaterialName) );
  }
  else if( pMatDictionary->has( strMaterialName ) )
  {
    OdUInt32 uIndexMat = 1;
    OdString strMaterialNameBase = strMaterialName;
    do
    {
      strMaterialName.format(L"_%d",uIndexMat);
      strMaterialName = strMaterialNameBase + strMaterialName;
      uIndexMat++;
    }
    while( pMatDictionary->has(strMaterialName) );
  }

  OdString repairedName;
  if( (OdDbSymUtil::repairSymbolName(repairedName, strMaterialName, pMatDictionary->database()) == eOk)
      && !repairedName.isEmpty()
    )
  {
    strMaterialName = repairedName;
  }

  pDbMaterial->setName( strMaterialName );

  // Information about palette

  if( !pDgMaterial->getPaletteName().isEmpty() )
  {
    pDbMaterial->setDescription(L"Material Palette:" + pDgMaterial->getPaletteName() );
  }

  // Ambient color

  OdGiMaterialColor clrAmbient;
  pDgMaterial->getAmbient( clrAmbient );
  pDbMaterial->setAmbient( clrAmbient );

  // Diffuse color

  OdGiMaterialColor clrDiffuse;
  OdDgMaterialMap   mapDiffuse;
  OdGiMaterialMap   mapGiDiffuse;
  pDgMaterial->getDiffuse( clrDiffuse, mapDiffuse );
  mapDiffuse.getGiMaterialMap( mapGiDiffuse );

  if( mapGiDiffuse.source() == OdGiMaterialMap::kFile && mapGiDiffuse.sourceFileName().isEmpty() )
  {
    mapGiDiffuse.setSource( OdGiMaterialMap::kScene );
  }

  convertDgnTextureMappingToDwgTextureMapping(mapGiDiffuse);
  pDbMaterial->setDiffuse( clrDiffuse, mapGiDiffuse);

  // Specular color

  OdGiMaterialColor clrSpecular;
  double            dGlossFactor;
  OdGiMaterialMap   mapSpecular;
  mapSpecular.setSource( OdGiMaterialMap::kScene);
  pDgMaterial->getSpecular( clrSpecular, dGlossFactor );
  pDbMaterial->setSpecular( clrSpecular, mapSpecular, dGlossFactor );

  // Reflection

  OdGiMaterialMap   mapReflection;
  pDgMaterial->getReflection( mapReflection );
  pDbMaterial->setReflection( mapReflection );
  pDbMaterial->setReflectanceScale( 1.0 );

  // Opacity

  OdGiMaterialMap   mapOpacity;
  pDbMaterial->setOpacity( pDgMaterial->getOpacity(), mapOpacity );

  // Refraction

  OdGiMaterialMap   mapRefraction;
  pDbMaterial->setRefraction( pDgMaterial->getRefraction(), mapRefraction );

  // Translucence

  pDbMaterial->setTranslucence( pDgMaterial->getTranslucence() );

  // Luminance

  pDbMaterial->setShininess( pDgMaterial->getShininess() );

  // Bump

  OdDgMaterialMap   mapBump;
  OdGiMaterialMap   mapGiBump;
  pDgMaterial->getBump( mapBump );
  mapBump.getGiMaterialMap( mapGiBump );
  convertDgnTextureMappingToDwgTextureMapping(mapGiBump);

  if( mapGiBump.source() == OdGiMaterialMap::kFile && mapGiBump.sourceFileName().isEmpty() )
  {
    mapGiBump.setSource( OdGiMaterialMap::kScene );
  }

  pDbMaterial->setBump(mapGiBump);

  // Global illumination.

  pDbMaterial->setGlobalIllumination( pDgMaterial->getGlobalIlluminationFlag() ? OdGiMaterialTraits::kGlobalIlluminationCastAndReceive : 
                                                                                 OdGiMaterialTraits::kGlobalIlluminationNone);

  // Two sided.

  pDbMaterial->setTwoSided( true );

  return true;
}

void DgnImporter::copyMaterials(OdDgDatabase* pDg, OdDbDatabase* pDb )
{
  OdDgMaterialTablePtr pDgnMaterialTable = pDg->getMaterialTable(OdDg::kForRead);
  OdDbObjectId idMatDictionary = pDb->getMaterialDictionaryId();

  if( idMatDictionary.isNull() || pDgnMaterialTable.isNull() )
  {
    ODA_FAIL_ONCE();
    return;
  }

  OdDbDictionaryPtr pDbMatDictionary = idMatDictionary.openObject(OdDb::kForWrite);

  OdDgElementIteratorPtr pMatTableIter = pDgnMaterialTable->createIterator();

  for(; !pMatTableIter->done(); pMatTableIter->step() )
  {
    OdDgMaterialTableRecordPtr pDgMaterial = pMatTableIter->item().openObject(OdDg::kForRead);

    OdDbMaterialPtr pDbMaterial = OdDbMaterial::createObject();

    if( copyMaterialProperties( pDgMaterial, pDbMaterial, pDbMatDictionary ) )
    {
      pDbMatDictionary->setAt( pDbMaterial->name(), pDbMaterial );

      OdDgnImportPathToDwgObject dwgPath;
      dwgPath.m_bExists = true;
      dwgPath.m_idPath.objectIds().push_back(pDbMaterial->objectId());

      _idMap[pDgMaterial->elementId()] = dwgPath;
      addResourceId(pDbMaterial->objectId());
    }
  }
}

void DgnImporter::copyLayers(OdDgDatabase* src, OdDbDatabase* dst, bool bRenameLayers)
{
  OdDgLevelTablePtr lt = src->getLevelTable(OdDg::kForRead);
  for (OdDgElementIteratorPtr i = lt->createIterator(); !i->done(); i->step())
  {
    OdDgLevelTableRecordPtr l = i->item().safeOpenObject();
    OdString name = l->getName();
    if (name.isEmpty())
      continue;
    if (name.iCompare(L"default") == 0)
      name = L"0";
    OdString repairedName;
    if (OdDbSymUtil::repairSymbolName(repairedName, name, dst) == eOk
        && !repairedName.isEmpty())
      name = repairedName;
    OdDbLayerTablePtr lt_d = dst->getLayerTableId().safeOpenObject(OdDb::kForWrite);
    OdDbObjectId l_d_id = lt_d->getAt(name);

    if( bRenameLayers )
    {
      OdString strNameBase = name;
      OdUInt32 uCount = 1;

      while( !l_d_id.isNull() )
      {
        name.format(L"_%d", uCount );
        name = strNameBase + name;

        l_d_id = lt_d->getAt(name);
        uCount++;
      }
    }

    if (l_d_id.isNull())
    {
      OdDbLayerTableRecordPtr l_d = OdDbLayerTableRecord::createObject();
      l_d->setName(name);
      dst->addOdDbObject(l_d, lt_d->objectId());
      l_d_id = lt_d->add(l_d);
    }

    OdDgnImportPathToDwgObject dwgPath;
    dwgPath.m_bExists = true;
    dwgPath.m_idPath.objectIds().push_back(l_d_id);

    _idMap[l->elementId()] = dwgPath;
    addResourceId(l_d_id);

    OdDbLayerTableRecordPtr l_d = l_d_id.safeOpenObject(OdDb::kForWrite);
    copyLayerProperties(l, l_d);
  }
}

OdDbObjectId DgnImporter::getObjectId( const OdDgElementId& idDgnElement ) const
{
  OdDbObjectId retVal;

  IdMap::const_iterator pIter = _idMap.find( idDgnElement );

  if( pIter != _idMap.end() )
  {

    if( pIter->second.m_bExists )
    {
      OdDbObjectIdArray arrPath = pIter->second.m_idPath.objectIds();

      retVal = arrPath[arrPath.size() - 1];
    }
  }

  return retVal;
}

bool DgnImporter::getObjectPath( const OdDgElementId& idDgnElement, OdDgnImportPathToDwgObject& dwgPath )
{
  bool bRet = false;

  IdMap::const_iterator pIter = _idMap.find( idDgnElement );

  if( pIter != _idMap.end() )
  {
    dwgPath = pIter->second;
    bRet = true;
  }

  return bRet;
}

void DgnImporter::addObjectPath( const OdDgElementId& idDgnElement, const OdDgnImportPathToDwgObject& dwgPath )
{
  _idMap[idDgnElement] = dwgPath;
}

void DgnImporter::pushIdMap()
{
  m_arrIdMapStack.push_back( _idMap );
  _idMap.clear();
}

void DgnImporter::popIdMap()
{
  if( m_arrIdMapStack.size() > 0 )
  {
    _idMap = m_arrIdMapStack.last();
    m_arrIdMapStack.removeLast();
  }
}

void DgnImporter::addResourceId( const OdDbObjectId& idResource )
{
  if( _properties->get_EraseUnusedResources() && !idResource.isNull() )
  {
    IdResourceUsage::iterator pIter = _dwgResourceUsage.find(idResource);

    if( pIter == _dwgResourceUsage.end() )
      _dwgResourceUsage[idResource] = false;
  }
}

void DgnImporter::setResourceUsage( const OdDbObjectId& idResource )
{
  if( _properties->get_EraseUnusedResources() && !idResource.isNull() )
  {
    IdResourceUsage::iterator pIter = _dwgResourceUsage.find(idResource);

    if( pIter != _dwgResourceUsage.end() )
      _dwgResourceUsage[idResource] = true;
  }
}

void DgnImporter::removeUnusedResources()
{
  if( _properties->get_EraseUnusedResources() )
  {
    IdResourceUsage::iterator pIter = _dwgResourceUsage.begin();

    for(; pIter != _dwgResourceUsage.end(); pIter++ )
    {
      if( !pIter->second )
      {
        OdDbObjectPtr pObj = pIter->first.openObject(OdDb::kForWrite);
        pObj->erase(true);
      }
    }
  }
}

void DgnImporter::copyLineStyles(OdDgDatabase* src, OdDbDatabase* dst)
{
  DgnLSImporter lsImporter(src, dst);
  OdDgLineStyleTablePtr lst = src->getLineStyleTable();
  { // Import defaults
    for (OdUInt32 nDef = 0; nDef < 8; nDef++)
    {
      OdDgLineStyleTableRecordPtr ls = lst->getAt(nDef).safeOpenObject();
      lsImporter.copyLinetype(ls);
      //OdDbLinetypeTableRecordPtr lt_d = lsImporter.copyLinetype(ls);
      // DGNImportDuplicates_9896_VK
      // Internal linestyles haven't id's. Them must be searched by entryId.
      //if (lt_d.isNull())
      //  _idMap[ls->id()] = dst->getLinetypeContinuousId();
      //else
      //  _idMap[ls->id()] = lt_d->id();
    }
  } //
  for (OdDgElementIteratorPtr i = lst->createIterator(); !i->done();  i->step())
  {
    OdDgLineStyleTableRecordPtr ls = i->item().safeOpenObject();
    OdDbLinetypeTableRecordPtr lt_d = lsImporter.copyLinetype(ls);
    if (lt_d.isNull())
    {
      OdDgnImportPathToDwgObject dwgPath;
      dwgPath.m_bExists = true;
      dwgPath.m_idPath.objectIds().push_back(dst->getLinetypeContinuousId());

      _idMap[i->item()] = dwgPath;
    }
    else
    {
      OdDgnImportPathToDwgObject dwgPath;
      dwgPath.m_bExists = true;
      dwgPath.m_idPath.objectIds().push_back(lt_d->id());

      _idMap[i->item()] = dwgPath;
      addResourceId(lt_d->id());
    }
  }
}

void DgnImporter::copyNamedGroup(OdDgModel* m, const OdString& strGroupName, OdDbBlockTableRecord* ms, OdInt8 iViewIndex )
{
  if( !m || !ms )
  {
    return;
  }

  setModelLevelMask( m, iViewIndex );

  OdUInt8 uXRefMode = 2;

  DgnImporter* pImporter = OdDgnImportContext::getDgnImporter();

  if( pImporter )
  {
    OdRxVariantValue tmpVar = (OdRxVariantValue)pImporter->properties()->getAt("XRefImportMode");
    uXRefMode = tmpVar->getUInt8();
  }

  OdDgNamedGroupHeaderPtr pNamedGroup;

  OdDgElementIteratorPtr pCtrlIter = m->createControlElementsIterator();

  for(; !pCtrlIter->done(); pCtrlIter->step() )
  {
    OdDgElementPtr pItem = pCtrlIter->item().openObject(OdDg::kForRead);

    if( pItem->isKindOf( OdDgNamedGroupHeader::desc()) )
    {
      pNamedGroup = pItem;

      if( pNamedGroup->getName() == strGroupName )
      {
        break;
      }
      else
      {
        pNamedGroup = 0;
      }
    }
  }

  if( pNamedGroup.isNull() )
  {
    copySpaceBlock( m, ms, false, iViewIndex );
  }
  else
  {
    OdArray<OdDgElementId> idElements;

    OdDgDatabase* pDb = m->database();

    if( pDb )
    {
      for( OdUInt32 i = 0; i < pNamedGroup->getNamedGroupItemCount(); i++ )
      {
        OdDgNamedGroupItem curItem = pNamedGroup->getNamedGroupItem( i );

        if( curItem.getPathLength() != 1 )
        {
          continue;
        }

        OdArray<OdUInt64> arrPath;
        curItem.getPath(arrPath);

        idElements.push_back(pDb->getElementId( OdDbHandle(arrPath[0])));
      }
    }

    for( OdUInt32 j = 0; j < idElements.size(); j++ )
    {
      if( idElements[j].isNull() )
      {
        continue;
      }

      OdDgElementPtr pDgElement = idElements[j].openObject();

      if( !uXRefMode && (pDgElement->getElementType() == OdDgElement::kTypeReferenceAttachmentHeader) )
      {
        continue;
      }

      if (!pDgElement.isNull())
      {
        ((OdDgnImportPE*)pDgElement->queryX(OdDgnImportPE::desc()))->importElement(pDgElement, ms);
      }
    }
  }

  removeModelLevelMask();
}

bool DgnImporter::setActiveModelView( OdDgModel* m, OdInt8 iViewIndex )
{
  bool bRet = false;

  if( OdDgnImportContext::getActiveView().isNull() && (iViewIndex >=0) && (iViewIndex < 8) )
  {
    OdDgViewGroupTablePtr pViewGroupTable = m->database()->getViewGroupTable(OdDg::kForRead);

    OdDgElementIteratorPtr pViewGroupIter = pViewGroupTable->createIterator();

    for(; !pViewGroupIter->done(); pViewGroupIter->step() )
    {
      OdDgViewGroupPtr pVG = pViewGroupIter->item().openObject(OdDg::kForRead);

      if( !pVG.isNull() && pVG->getModelId() == m->elementId() )
      {
        OdDgElementIteratorPtr pViewIter = pVG->createIterator();

        for(; !pViewIter->done(); pViewIter->step() )
        {
          OdDgViewPtr pView = pViewIter->item().openObject(OdDg::kForWrite);

          if( !pView.isNull() && (pView->getIndex() == (OdUInt8)(iViewIndex)) )
          {
            OdDgnImportContext::setActiveView( pView );
            bRet = true;
            break;
          }
        }
        break;
      }
    }
  }

  return bRet;
}

void DgnImporter::copySpaceBlock(OdDgModel* m, 
                                   OdDbBlockTableRecord* ms, 
                                     bool bIgnoreTransformToOffset, 
                                       OdInt8 iViewIndex, 
                                         bool bModelSpace
                                )
{
  bool bResetView = false;

  if( setActiveModelView(  m, iViewIndex ) )
  {
    bResetView = true;

    if( bModelSpace )
    {
      ms->database()->setLWDISPLAY( OdDgnImportContext::getActiveView()->getShowLineWeightsFlag() );
    }
  }

  setModelLevelMask( m, iViewIndex );

  OdUInt8 uXRefMode = 2;

  DgnImporter* pImporter = OdDgnImportContext::getDgnImporter();

  if( pImporter )
  {
    OdRxVariantValue tmpVar = (OdRxVariantValue)pImporter->properties()->getAt("XRefImportMode");
    uXRefMode = tmpVar->getUInt8();
  }

  OdDgnImportContext::setBackgroundColor( m->getBackground() );

  OdDgElementIdArray arrRasterRefsBefore;
  OdDgElementIdArray arrRasterRefsAfter;

  OdDgElementIdArray        arrXRefBefore;
  OdDgElementIdArray        arrXRefAfter;
  OdDgElementIdArray        arrXRef;
  OdDgReferenceOrderPtr     pXRefOrder;

  for (OdDgElementIteratorPtr pIter = m->createControlElementsIterator(); !pIter->done(); pIter->step())
  {
    OdRxObjectPtr unknownElement = pIter->item().openObject( OdDg::kForRead );
    if (unknownElement->isKindOf(OdDgRasterAttachmentHeader::desc()))
    {
      OdDgRasterAttachmentHeaderPtr pRasterHeader = unknownElement;

      if( !pRasterHeader.isNull() )
      {
        if( pRasterHeader->getRasterPlane() == OdDgRasterAttachmentHeader::kBackground )
        {
          arrRasterRefsBefore.push_back(pIter->item());
        }
        else
        {
          arrRasterRefsAfter.push_back(pIter->item());
        }
      }
    }
    else if (unknownElement->isKindOf(OdDgReferenceAttachmentHeader::desc()) && uXRefMode )
    {
      arrXRef.push_back(pIter->item());
    }
    else if( unknownElement->isKindOf( OdDgReferenceOrder::desc() ) && uXRefMode )
    {
      pXRefOrder = unknownElement;
    }
  }

  if( !pXRefOrder.isNull() )
  {
    OdUInt32 uRef = 0;

    bool bDrawBeforeOfModel = true;

    for( uRef = 0; uRef < pXRefOrder->getNumberOfReferences(); uRef++ )
    {
      OdUInt64 uRefId = pXRefOrder->getReferenceId(uRef);

      if( uRefId == 0 )
      {
        bDrawBeforeOfModel = false;
      }
      else
      {
        for( OdUInt32 uCurRef = 0; uCurRef < arrXRef.size(); uCurRef++ )
        {
          if( arrXRef[uCurRef].getHandle() == uRefId )
          {
            if( bDrawBeforeOfModel )
            {
              arrXRefBefore.push_back( arrXRef[uCurRef] );
            }
            else
            {
              arrXRefAfter.push_back( arrXRef[uCurRef] );
            }

            arrXRef.removeAt( uCurRef );
            break;
          }
        }
      }
    }

    for( uRef = 0; uRef < arrXRef.size(); uRef++ )
    {
      arrXRefAfter.push_back( arrXRef[uRef] );
    }
  }
  else
  {
    for( OdUInt32 uRef = 0; uRef < arrXRef.size(); uRef++ )
    {
      arrXRefAfter.push_back( arrXRef[uRef] );
    }
  }

  OdUInt32 i = 0;

  for( i = 0; i < arrRasterRefsBefore.size(); i++ )
  {
    OdDgElementPtr pRasterAttachment = arrRasterRefsBefore[i].openObject(OdDg::kForRead);

    if( !pRasterAttachment.isNull() )
    {
      ((OdDgnImportPE*)pRasterAttachment->queryX(OdDgnImportPE::desc()))->importElement(pRasterAttachment, ms);
    }
  }

  for( i = 0; i < arrXRefBefore.size(); i++ )
  {
    OdDgElementPtr pXRef = arrXRefBefore[i].openObject(OdDg::kForRead);

    if( !pXRef.isNull() )
    {
      ((OdDgnImportPE*)pXRef->queryX(OdDgnImportPE::desc()))->importElement(pXRef, ms);
    }
  }

  for (OdDgElementIteratorPtr pIter = m->createGraphicsElementsIterator() ; !pIter->done(); pIter->step())
  {
    OdDgElementPtr pDgElement = pIter->item().openObject();
    if (!pDgElement.isNull())
    {
      ((OdDgnImportPE*)pDgElement->queryX(OdDgnImportPE::desc()))->importElement(pDgElement, ms);
    }
  }

  for( i = 0; i < arrXRefAfter.size(); i++ )
  {
    OdDgElementPtr pXRef = arrXRefAfter[i].openObject(OdDg::kForRead);

    if( !pXRef.isNull() )
    {
      ((OdDgnImportPE*)pXRef->queryX(OdDgnImportPE::desc()))->importElement(pXRef, ms);
    }
  }

  for( i = 0; i < arrRasterRefsAfter.size(); i++ )
  {
    OdDgElementPtr pRasterAttachment = arrRasterRefsAfter[i].openObject(OdDg::kForRead);

    if( !pRasterAttachment.isNull() )
    {
      ((OdDgnImportPE*)pRasterAttachment->queryX(OdDgnImportPE::desc()))->importElement(pRasterAttachment, ms);
    }
  }

  if( !bIgnoreTransformToOffset )
  {
    OdGeVector3d globalOffset = m->getGlobalOrigin().asVector();

    if (globalOffset != OdGeVector3d::kIdentity)
    {
      OdGeMatrix3d tr = OdGeMatrix3d::translation(-globalOffset);

      for (OdDbObjectIteratorPtr it = ms->newIterator(); !it->done(); it->step())
      {
        OdDbEntityPtr e = it->objectId().safeOpenObject(OdDb::kForWrite);

        if( e->isKindOf(OdDbDimension::desc() ) )
        {
          OdDbDimensionPtr pDim = e;
          OdDbObjectId idBlock = pDim->dimBlockId();

          if( !idBlock.isNull() )
          {
            OdDbBlockTableRecordPtr pDimBlock = idBlock.openObject(OdDb::kForWrite);

            OdGeVector3d vrDimNormal = pDim->normal();
            OdGeMatrix3d matWorldToPlaneDim = OdGeMatrix3d::worldToPlane(vrDimNormal);
            OdGeMatrix3d matPlaneToWorldDim = OdGeMatrix3d::planeToWorld(vrDimNormal);

            if( !pDimBlock.isNull() )
            {
              for (OdDbObjectIteratorPtr it = pDimBlock->newIterator(); !it->done(); it->step())
              {
                OdDbEntityPtr e = it->objectId().safeOpenObject(OdDb::kForWrite);
                e->transformBy(matPlaneToWorldDim);
                e->transformBy(tr);
                e->transformBy(matWorldToPlaneDim);
              }
            }
          }
        }

        e->transformBy(tr);
      }
    }
  }

  if( bResetView )
  {
    OdDgnImportContext::setActiveView( OdDgViewPtr() );
  }

  if( iViewIndex >= 0 && iViewIndex < 8 )
  {
    removeModelLevelMask();
  }
}

void DgnImporter::copyModelSpace(OdDgModel* m, OdDbDatabase* pDb)
{
  OdDbBlockTableRecordPtr ms = pDb->getModelSpaceId().safeOpenObject(OdDb::kForWrite);
  copySpaceBlock(m, ms, false, _properties->get_ImportViewIndex(), true );
}

void DgnImporter::copyLayout(OdDgModel* m, OdDbDatabase* pDb)
{
  ODA_ASSERT(m->isKindOf(OdDgSheetModel::desc()));
  OdDgSheetModel* sm = (OdDgSheetModel*)m;
  OdDbObjectId btrId;
  OdString name = m->getName(),
           repairedName;
  if (   OdDbSymUtil::repairSymbolName(repairedName, name, pDb) == eOk
      && !repairedName.isEmpty())
    name = repairedName;
  OdDbObjectId layoutId = pDb->appServices()->layoutManager()->createLayout(pDb, name, &btrId);
  OdDbLayoutPtr dbLayout = layoutId.safeOpenObject(OdDb::kForWrite);
  OdDbPlotSettingsValidator* pv = pDb->appServices()->plotSettingsValidator();
  // TODO: units
  pv->setClosestMediaName(dbLayout, sm->getSheetWidth(), sm->getSheetHeight(), OdDbPlotSettings::kMillimeters, false);
  OdDbBlockTableRecordPtr btr = btrId.safeOpenObject(OdDb::kForWrite);
  copySpaceBlock(m, btr, false, _properties->get_ImportViewIndex() );
}

void DgnImporter::copyBlocks(OdDgDatabase* pDg, OdDbDatabase* pDb)
{
  OdDbBlockTablePtr bt = pDb->getBlockTableId().safeOpenObject(OdDb::kForWrite);
  OdDgElementIteratorPtr i = pDg->getSharedCellDefinitionTable()->createIterator();
  for (; !i->done(); i->step())
  {
    OdDgSharedCellDefinitionPtr scd = i->item().safeOpenObject();
    OdDbBlockTableRecordPtr block = OdDbBlockTableRecord::createObject();
    OdString name = scd->getName();
    if (name.isEmpty()) // empty name => user anonymous block
      name = L"*U";
    else if (name[0] == '*') // anonymous blocks will be reindexed
    {
      if (name.getLength() < 2)
        name = L"*U";
      else if (name.getLength() > 2)
        name = name.left(2);
    }
    else
    {
      OdString repairedName;
      if (OdDbSymUtil::repairSymbolName(repairedName, name, pDb) == eOk && !repairedName.isEmpty())
        name = repairedName;
    }
    block->setComments(scd->getDescription());
    block->setOrigin(scd->getOrigin());
    pDb->addOdDbObject(block, bt->objectId());

    if(!bt->has(name) )
      block->setName(name);
    else
    	block->setName(L"*U");

    bt->add(block);

    OdDgnImportPathToDwgObject dwgPath;
    dwgPath.m_bExists = true;
    dwgPath.m_idPath.objectIds().push_back(block->objectId());

    _idMap[i->item()] = dwgPath;
    addResourceId(block->objectId());
  }
  // all the block definitions are added first, because of possible block references
  for (i->start(); !i->done(); i->step())
  {
    OdDgSharedCellDefinitionPtr scd = i->item().safeOpenObject();
    OdDbObjectId idBlock = getObjectId( i->item() );

    if( !idBlock.isNull() )
    {
      OdDbBlockTableRecordPtr block = idBlock.safeOpenObject(OdDb::kForWrite);
      for (OdDgElementIteratorPtr  pIter = scd->createIterator() ; !pIter->done(); pIter->step())
      {
        OdDgElementPtr pDgElement = pIter->item().openObject();
        if (!pDgElement.isNull())
        {
          ((OdDgnImportPE*)pDgElement->queryX(OdDgnImportPE::desc()))->importElement(pDgElement, block);
        }
      }
    }
  }
}

void DgnImporter::copyRegApps(OdDgDatabase* pDg, OdDbDatabase* pDb)
{
  OdDbRegAppTablePtr pRegAppTable = pDb->getRegAppTableId().openObject(OdDb::kForWrite);
  for (OdDgElementIteratorPtr i = pDg->getRegAppTable()->createIterator(); !i->done(); i->step())
  {
    OdDgRegAppTableRecordPtr r = i->item().safeOpenObject();
    OdString regAppName = r->getName();

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
    if (   OdDbSymUtil::repairSymbolName(repairedName, regAppName, pDb) == eOk
        && !repairedName.isEmpty())
      regAppName = repairedName;
    if (!pRegAppTable->has(regAppName))
    {
      OdDbRegAppTableRecordPtr pRegApp = OdDbRegAppTableRecord::createObject();
      pRegApp->setName(regAppName);

      OdDbObjectId idItem = pDb->getOdDbObjectId( OdDbHandle((OdUInt64)(i->item().getHandle())));

      if( idItem.isNull() )
      {
        pDb->addOdDbObject(pRegApp, pRegAppTable->objectId(), i->item().getHandle());
      }
      else
      {
        pDb->addOdDbObject(pRegApp, pRegAppTable->objectId() );
      }

      pRegAppTable->add(pRegApp);
    }
  }
}

void DgnImporter::copyTextStyles(OdDgDatabase* pDg, OdDbDatabase* pDb)
{
  OdDbTextStyleTablePtr tst = pDb->getTextStyleTableId().safeOpenObject(OdDb::kForWrite);
  for (OdDgElementIteratorPtr i = pDg->getTextStyleTable()->createIterator(); !i->done(); i->step())
  {
    OdDgTextStyleTableRecordPtr ts = i->item().safeOpenObject();
    OdGiTextStyle giStyle;
    ts->getGiTextStyle(giStyle);
    OdDbTextStyleTableRecordPtr dbTs;
    OdString tsname = ts->getName();
    if (tsname.isEmpty())
    {
      OdDgFontTableRecordPtr font = pDg->getFontTable()->getFont(ts->getFontEntryId());
      if (font.isNull())
        continue;
      tsname = "Style-" + font->getName();
    }
    OdString repairedName;
    if (   OdDbSymUtil::repairSymbolName(repairedName, tsname, pDb) == eOk
        && !repairedName.isEmpty())
      tsname = repairedName;
    OdDbObjectId id = tst->getAt(tsname);
    if (id.isNull())
    {
      dbTs = OdDbTextStyleTableRecord::createObject();
      dbTs->setName(tsname);

      OdDbObjectId idItem = pDb->getOdDbObjectId( OdDbHandle((OdUInt64)(i->item().getHandle())));

      if( idItem.isNull() )
      {
        pDb->addOdDbObject(dbTs, tst->objectId(), i->item().getHandle());
      }
      else
      {
        pDb->addOdDbObject(dbTs, tst->objectId() );
      }

      tst->add(dbTs);
    }
    else
    {
      dbTs = id.safeOpenObject(OdDb::kForWrite);
    }
    dbTs->setIsVertical(giStyle.isVertical());
    dbTs->setTextSize(giStyle.textSize());
    dbTs->setXScale(giStyle.xScale());
    dbTs->setObliquingAngle(giStyle.obliquingAngle());
    dbTs->setIsBackwards(giStyle.isBackward());
    dbTs->setIsUpsideDown(giStyle.isUpsideDown());
    // TODO: other font types
    if (giStyle.isShxFont())
    {
      dbTs->setFileName(giStyle.getFontFilePath(pDb)); // ??
      dbTs->setBigFontFileName(giStyle.bigFontFileName());
    }
    else
    {
      OdString Typeface;
      bool bold, italic;
      int nCharset, nPitchAndFamily;
      giStyle.font(Typeface, bold, italic, nCharset, nPitchAndFamily);
      dbTs->setFont(Typeface, bold, italic, nCharset, nPitchAndFamily);
    }

    OdDgnImportPathToDwgObject dwgPath;
    dwgPath.m_bExists = true;
    dwgPath.m_idPath.objectIds().push_back(dbTs->objectId());

    _idMap[i->item()] = dwgPath;

    addResourceId(dbTs->objectId());
  }
}

OdDbObjectId createTextStyleFromDimStyle( const OdDgDimStyleTableRecordPtr& pDgDimStyle, 
                                            OdDbDatabase* pDb,
                                              double dTextHeight,
                                                double dTextWidth )
{
  OdDbObjectId idRet;

  OdGiTextStyle dimTextStyle;

  OdDgFontTableRecordPtr pFontRec;

  OdDgTextStyleTableRecordPtr pRec;

  OdDgTextStyleTableRecordPtr pTextStyle;

  if( pDgDimStyle->getTextStyleEntryId() > 0 )
  {
    OdDgTextStyleTablePtr pTextStyleTable = pDgDimStyle->database()->getTextStyleTable(OdDg::kForRead);

    if( !pTextStyleTable.isNull() )
    {
      OdDgElementId idTextStyle = pTextStyleTable->getAt( pDgDimStyle->getTextStyleEntryId() );

      if( !idTextStyle.isNull() )
      {
        pTextStyle = idTextStyle.openObject(OdDg::kForRead);
      }
    }
  }

  if( pTextStyle.isNull() )
    pRec = pDgDimStyle->database()->getTextStyleTable()->getDefaultData();
  else
    pRec = pTextStyle;

  OdDgFontTablePtr pFontTable =  pDgDimStyle->database()->getFontTable();

  OdUInt32 uFontId = pRec->getFontEntryId();

  if( pDgDimStyle->getTextFontOverrideFlag() )
    uFontId = pDgDimStyle->getTextFontId();

  if( !pFontTable.isNull() )
    pFontRec = pFontTable->getFont( uFontId );

  if( pFontRec.isNull() )
  {
    pFontRec = pFontTable->getFont( pDgDimStyle->database()->appServices()->getFAST_FONT() );
  }

  pRec->getGiTextStyle(dimTextStyle);
  dimTextStyle.setTextSize( dTextHeight );
  dimTextStyle.setXScale( dTextWidth/dTextHeight );
  dimTextStyle.setUnderlined( pDgDimStyle->getTextUnderlineFlag() );

  if( !pFontRec.isNull() )
  {
    if( pFontRec->getType() == kFontTypeTrueType )
    {
      dimTextStyle.setFont(pFontRec.get()->getName(), pRec->getBoldFlag(), pRec->getItalicsFlag(), 0, 0 );
    }
    else
    {
      dimTextStyle.setFont(pFontRec.get()->getName(), false, false, 0, 0 );
      dimTextStyle.setFileName( pFontRec.get()->getName() );
    }

    dimTextStyle.loadStyleRec( pDgDimStyle->database() );
  }

  dimTextStyle.setVertical(false);

  idRet = OdDgnImportContext::getStyleForDbText(pDb, dimTextStyle );

  if( idRet.isNull() )
  {
    idRet = OdDgnImportContext::createStyleForDbText(pDb, pDgDimStyle->database(), dimTextStyle );
  }

  return idRet;
}

void DgnImporter::copyDimStyleProperties( const OdDgDimStyleTableRecordPtr& pDgDimStyle, 
                                            OdDbDimStyleTableRecordPtr& pDbDimStyle,
                                              OdDbDatabase* pDb )
{
  OdString strName = pDgDimStyle->getName();

  if( strName.isEmpty() )
    strName = L"Default";

  OdString repairedName;
  if( (OdDbSymUtil::repairSymbolName(repairedName, strName, pDb) == eOk) && !repairedName.isEmpty() )
    strName = repairedName;

  pDbDimStyle->setName( strName );

  pDbDimStyle->setDimcen( pDgDimStyle->getCenterMarkSize() );

  // Angular data

  pDbDimStyle->setDimarcsym(0);

  OdUInt32 uAccur = 4;

  if( pDgDimStyle->getAngleAccuracy() >= 0 && pDgDimStyle->getAngleAccuracy() < 8 )
  {
    uAccur = pDgDimStyle->getAngleAccuracy();
  }

  pDbDimStyle->setDimadec( uAccur );

  if( pDgDimStyle->getAngleDisplayMode() == OdDgDimTextFormat::kAngle_Radian )
  {
    pDbDimStyle->setDimaunit(3);
  }
  else if( pDgDimStyle->getAngleDisplayMode() == OdDgDimTextFormat::kAngle_C )
  {
    pDbDimStyle->setDimaunit(2);
  }
  else if( pDgDimStyle->getShowAngleFormatFlag() )
  {
    pDbDimStyle->setDimaunit(1);
  }
  else
  {
    pDbDimStyle->setDimaunit(0);
  }

  OdUInt32 uDimAZinValue = 0;

  if( !pDgDimStyle->getAngleLeadingZeroFlag() )
  {
    uDimAZinValue += 1;
  }

  if( !pDgDimStyle->getAngleTrailingZerosFlag() )
  {
    uDimAZinValue += 2;
  }

  pDbDimStyle->setDimazin( uDimAZinValue );

  // Extension lines

  pDbDimStyle->setDimse1( !pDgDimStyle->getExtensionLinesPresentFlag() );
  pDbDimStyle->setDimse2( !pDgDimStyle->getExtensionLinesPresentFlag() );

  // Dimension units

  bool bUseSecondaryUnits = false;

  if( pDgDimStyle->getShowSecondaryUnitsFlag() )
  {
    bUseSecondaryUnits = true;
  }

  // Dimension Primary units

  // 1. leading and trailing zeros

  OdUInt32 uDimZinValue = 0;

  if( !pDgDimStyle->getPrimaryLeadingZeroFlag() )
  {
    uDimZinValue += 4;
  }

  if( !pDgDimStyle->getPrimaryTrailingZerosFlag() )
  {
    uDimZinValue += 8;
  }

  // 2. Scale factor of units and subunits

  OdString strPrimaryUnitsName;
  OdString strPrimarySubUnitsName;

  double dUORtoMasterScaleFactor = 1.0;

  OdDgModel::UnitDescription baseUnits;
  OdDgModel::UnitDescription masterUnits;
  OdDgModel::UnitDescription subUnits;

  baseUnits.m_numerator = 1.0;
  baseUnits.m_denominator = 1.0;

  OdDgModelPtr pDimensionModel;

  OdDgElementId idModel = pDgDimStyle->database()->getActiveModelId();

  if( !idModel.isNull() )
  {
    pDimensionModel = idModel.openObject(OdDg::kForRead);
  }

  if( !pDimensionModel.isNull() )
  {
    pDimensionModel->getMasterUnit(baseUnits);
    pDimensionModel->getSubUnit(subUnits);
    masterUnits = baseUnits;
    dUORtoMasterScaleFactor = pDimensionModel->convertUORsToWorkingUnits(1.0);
  }

  if( !pDgDimStyle->getMasterFileUnitsFlag() )
  {
    masterUnits = pDgDimStyle->getPrimaryMasterUnits();
    subUnits = pDgDimStyle->getPrimarySubUnits();
  }

  double dPrimaryUnitsScaleFactor = baseUnits.m_denominator / 
    baseUnits.m_numerator / masterUnits.m_denominator * masterUnits.m_numerator;

  double dSubUnitsScaleFactor = masterUnits.m_denominator / masterUnits.m_numerator / 
    subUnits.m_denominator * subUnits.m_numerator;

  // 3. Prefix and suffix for units and subunits

  OdString strPrefixSuffixMaster = OdString::kEmpty;
  OdString strPrefixSuffixSub    = OdString::kEmpty;

  OdString strUnitPrefix;
  OdString strMasterLabel = masterUnits.m_name;
  OdString strSubLabel = subUnits.m_name;
  OdString strUnitSuffix;

  if( pDgDimStyle->getPrimaryLabelDisplayMode() == OdDgDimTextFormat::kMu || 
      pDgDimStyle->getPrimaryLabelDisplayMode() == OdDgDimTextFormat::kMu_Su ||
      pDgDimStyle->getPrimaryLabelDisplayMode() == OdDgDimTextFormat::kSu
    )
  {
    strMasterLabel = OdString::kEmpty;
    strSubLabel    = OdString::kEmpty;
  }

  if( bUseSecondaryUnits )
  {
    strUnitPrefix = pDgDimStyle->getUpperPrefix();
    strUnitSuffix = pDgDimStyle->getUpperSuffix();
  }
  else
  {
    strUnitPrefix = pDgDimStyle->getMainPrefix();
    strUnitSuffix = pDgDimStyle->getMainSuffix();
  }

  if( strUnitPrefix[0] == L'\0' )
    strUnitPrefix.empty();

  if( strUnitSuffix[0] == L'\0' )
    strUnitSuffix.empty();

  strPrefixSuffixMaster = strUnitPrefix + L"<>" + strMasterLabel + strUnitSuffix;
  strPrefixSuffixSub    = strSubLabel + strUnitSuffix;

  if( bUseSecondaryUnits )
  {
    strPrefixSuffixMaster += L"\\X";
    strPrefixSuffixSub += L"\\X";

    if( pDgDimStyle->getTextUnderlineFlag() )
    {
      strPrefixSuffixMaster += L"\\L";
      strPrefixSuffixSub += L"\\L";
    }
  }

  // 4. Accuracy

  OdUInt8 uDimUnitFormat = 2;
  OdUInt8 uDimPresission = 0;

  OdDgnImportContext::convertAccuracyToDwgFormat(pDgDimStyle->getPrimaryAccuracy(), uDimUnitFormat, uDimPresission );

  pDbDimStyle->setDimlfac(dPrimaryUnitsScaleFactor);
  pDbDimStyle->setDimmzf( dSubUnitsScaleFactor );
  pDbDimStyle->setDimrnd(0.0);
  pDbDimStyle->setDimdec( uDimPresission );
  pDbDimStyle->setDimlunit( uDimUnitFormat );
  pDbDimStyle->setDimdsep( pDgDimStyle->getDecimalCommaFlag() ? L',' : L'.');
  pDbDimStyle->setDimzin( uDimZinValue );
  pDbDimStyle->setDimpost( strPrefixSuffixMaster );
  pDbDimStyle->setDimmzs( strPrefixSuffixSub );

  // Dimension alternate units

  pDbDimStyle->setDimalt( bUseSecondaryUnits );

  if( bUseSecondaryUnits )
  {
    // 1. leading and trailing zeros

    uDimZinValue = 0;

    if( !pDgDimStyle->getSecondaryLeadingZeroFlag() )
    {
      uDimZinValue += 4;
    }

    if( !pDgDimStyle->getSecondaryTrailingZerosFlag() )
    {
      uDimZinValue += 8;
    }

    // 2. Scale factor of units and subunits

    OdString strSecondaryUnitsName;
    OdString strSecondarySubUnitsName;

    masterUnits = pDgDimStyle->getSecondaryMasterUnits();
    subUnits = pDgDimStyle->getSecondarySubUnits();

    double dSecondaryUnitsScaleFactor = baseUnits.m_denominator / 
      baseUnits.m_numerator / masterUnits.m_denominator * masterUnits.m_numerator;

    double dSecondarySubUnitsScaleFactor = masterUnits.m_denominator / masterUnits.m_numerator / 
      subUnits.m_denominator * subUnits.m_numerator;

    // 3. Prefix and suffix for units and subunits

    strPrefixSuffixMaster = OdString::kEmpty;
    strPrefixSuffixSub    = OdString::kEmpty;

    strUnitPrefix = OdString::kEmpty;
    strMasterLabel = masterUnits.m_name;
    strSubLabel = subUnits.m_name;
    strUnitSuffix = OdString::kEmpty;

    if( pDgDimStyle->getSecondaryLabelDisplayMode() == OdDgDimTextFormat::kMu || 
        pDgDimStyle->getSecondaryLabelDisplayMode() == OdDgDimTextFormat::kMu_Su ||
        pDgDimStyle->getSecondaryLabelDisplayMode() == OdDgDimTextFormat::kSu
      )
    {
      strMasterLabel = OdString::kEmpty;
      strSubLabel    = OdString::kEmpty;
    }

    strUnitPrefix = pDgDimStyle->getLowerPrefix();
    strUnitSuffix = pDgDimStyle->getLowerSuffix();

    if( strUnitPrefix[0] == L'\0' )
      strUnitPrefix.empty();

    if( strUnitSuffix[0] == L'\0' )
      strUnitSuffix.empty();

    strPrefixSuffixMaster = strUnitPrefix + L"[]" + strMasterLabel + strUnitSuffix;
    strPrefixSuffixSub    = strSubLabel + strUnitSuffix;

    // 4. Accuracy

    uDimUnitFormat = 2;
    uDimPresission = 0;

    OdDgnImportContext::convertAccuracyToDwgFormat(pDgDimStyle->getSecondaryAccuracy(), uDimUnitFormat, uDimPresission );

    if( uDimUnitFormat == 5 ) // Fractional (stacked)
    {
      uDimUnitFormat = 7; // Fractional
    }

    pDbDimStyle->setDimaltf(dSecondaryUnitsScaleFactor);
    pDbDimStyle->setDimaltmzf( dSecondarySubUnitsScaleFactor );
    pDbDimStyle->setDimaltrnd(0.0);
    pDbDimStyle->setDimaltd( uDimPresission );
    pDbDimStyle->setDimaltu( uDimUnitFormat );
    pDbDimStyle->setDimaltz( uDimZinValue );
    pDbDimStyle->setDimapost( strPrefixSuffixMaster );
    pDbDimStyle->setDimaltmzs( strPrefixSuffixSub );
  }

  // Dimension Stacked fraction settings

  if( pDgDimStyle->getUseStackedFractionFlag() )
  {
    pDbDimStyle->setDimfrac( 2 - (OdUInt8)(pDgDimStyle->getStackedFractionType()) );
  }
  else
  {
    pDbDimStyle->setDimfrac(2);
  }

  // Calculate text height

  double dTextHeight = 1.0;
  double dTextWidth  = 1.0;

  OdDgTextStyleTablePtr pTextStyle = pDgDimStyle->database()->getTextStyleTable(OdDg::kForRead);

  if( !pTextStyle->getAt(pDgDimStyle->getTextStyleEntryId()).isNull() )
  {
    OdDgTextStyleTableRecordPtr pDimTextStyle = pTextStyle->getAt(pDgDimStyle->getTextStyleEntryId()).openObject(OdDg::kForRead);
    dTextHeight = pDimTextStyle->getTextHeight();
    dTextWidth  = pDimTextStyle->getTextWidth();
  }

  if( pDgDimStyle->getTextHeightOverrideFlag() )
    dTextHeight = pDgDimStyle->getTextHeight();

  if( pDgDimStyle->getTextWidthOverrideFlag() )
    dTextWidth = pDgDimStyle->getTextWidth();

  // Dimension Tolerances

  bool bDisplayTolerance = false;

  if( pDgDimStyle->getShowToleranceFlag() && 
    !OdZero(pDgDimStyle->getToleranceTextScale()*dTextHeight) &&  
    !OdZero(pDgDimStyle->getToleranceTextScale()*dTextWidth) )
  {
    bDisplayTolerance = true;
  }

  pDbDimStyle->setDimtol(bDisplayTolerance);

  if( bDisplayTolerance )
  {
    pDbDimStyle->setDimlim(pDgDimStyle->getUseToleranceLimitModeFlag());

    double dLowTol  = pDgDimStyle->getLowerToleranceValue();
    double dHighTol = pDgDimStyle->getUpperToleranceValue();

    if( OdZero(dLowTol - dHighTol) && pDgDimStyle->getToleranceStackIfEqualFlag() )
    {
      dLowTol -= 1e-9;
    }

    pDbDimStyle->setDimtm( dLowTol );
    pDbDimStyle->setDimtp( dHighTol );
    pDbDimStyle->setDimtolj( 1 );
    pDbDimStyle->setDimtfac( pDgDimStyle->getToleranceTextScale() );

    uDimUnitFormat = 2;
    uDimPresission = 0;

    OdDgDimTextFormat::Accuracy dimAccuracy = pDgDimStyle->getPrimaryToleranceAccuracy();
    OdDgnImportContext::convertAccuracyToDwgFormat( dimAccuracy , uDimUnitFormat, uDimPresission );

    pDbDimStyle->setDimtdec( uDimPresission );

    uDimUnitFormat = 2;
    uDimPresission = 0;

    dimAccuracy =  pDgDimStyle->getSecondaryToleranceAccuracy(); 
    OdDgnImportContext::convertAccuracyToDwgFormat(dimAccuracy, uDimUnitFormat, uDimPresission );

    pDbDimStyle->setDimalttd( uDimPresission );
    pDbDimStyle->setDimtzin(8);
  }

  // Dimension fit options.

  pDbDimStyle->setDimtofl( pDgDimStyle->getJoinerFlag() );
  pDbDimStyle->setDimsoxd(true);

  OdDgDimTextInfo::FitOptions          iFitMode  = pDgDimStyle->getFitOptions();

  pDbDimStyle->setDimatfit(3);

  switch( iFitMode )
  {
    case OdDgDimTextInfo::kTermMovesFirst:
    {
      pDbDimStyle->setDimatfit(1);
    } break;

    case OdDgDimTextInfo::kTextInside:
    {
      pDbDimStyle->setDimtix(true);
    } break;

    case OdDgDimTextInfo::kTextMovesFirst:
    {
      pDbDimStyle->setDimatfit(2);
    } break;

    case OdDgDimTextInfo::kBothMoves:
    {
      pDbDimStyle->setDimatfit(0);
    } break;

    case OdDgDimTextInfo::kSmallestMoves:
    {
      pDbDimStyle->setDimatfit(3);
    } break;
  }

  if( pDgDimStyle->getDimLeaderEnableFlag() )
  {
    OdDgDimOptionOffset::ChainType iLeaderType = pDgDimStyle->getDimLeaderChainType();

    if( iLeaderType == OdDgDimOptionOffset::kNone )
    {
      pDbDimStyle->setDimtmove(2);
    }
    else
    {
      pDbDimStyle->setDimtmove(1);
    }
  }
  else
  {
    pDbDimStyle->setDimtmove(0);
  }


  // Dimension text

  // 1.Text height

  pDbDimStyle->setDimtfill(0);

  if( !OdZero(dTextHeight) )
    pDbDimStyle->setDimtxt( fabs(dTextHeight) );

  pDbDimStyle->setDimtxtdirection(false);

  // 2.Text color

  OdCmColor textColor;

  OdUInt32 uTextColorIndex = pDgDimStyle->getDimensionColor();

  if( pDgDimStyle->getTextColorOverrideFlag() )
  {
    uTextColorIndex = pDgDimStyle->getTextColor();
  }

  setDwgColorByDgnIndex( pDgDimStyle->database(), textColor, uTextColorIndex );

  pDbDimStyle->setDimclrt( textColor );

  // 3.Text direction

  if( pDgDimStyle->getHorizontalTextFlag() )
  {
    pDbDimStyle->setDimtih(true);
    pDbDimStyle->setDimtoh(true);
  }
  else
  {
    pDbDimStyle->setDimtih(false);
    pDbDimStyle->setDimtoh(false);
  }

  // 4.Text offset and lift

  double dTextOffsetX = pDgDimStyle->getGeometryMargin();

  if( pDgDimStyle->getCapsuleTextFlag() || pDgDimStyle->getBoxTextFlag() )
  {
    dTextOffsetX *= -0.5;
  }

  pDbDimStyle->setDimgap( dTextOffsetX );
  pDbDimStyle->setDimtvp( pDgDimStyle->getInlineTextLift() );

  // 5. Text alignments

  OdDgDimTextInfo::TextLocation txtLocation = pDgDimStyle->getTextLocation() ;

  pDbDimStyle->setDimtad( (OdUInt8)(txtLocation) );

  OdDgDimTextInfo::TextAlignment textAlm = pDgDimStyle->getTextJustification();

  pDbDimStyle->setDimjust(0);

  if( textAlm == OdDgDimTextInfo::kLeftText )
  {
    pDbDimStyle->setDimjust(1);
  }
  else if( textAlm == OdDgDimTextInfo::kRightText )
  {
    pDbDimStyle->setDimjust(2);
  }

  // 6. Text style

  OdDbObjectId idDimTextStyle = createTextStyleFromDimStyle(pDgDimStyle, pDb, dTextHeight, dTextWidth);
  pDbDimStyle->setDimtxsty( idDimTextStyle );

  // Arrows and lines

  pDbDimStyle->setDimexe( fabs(pDgDimStyle->getWitnessLineExtension()) );
  pDbDimStyle->setDimexo( fabs(pDgDimStyle->getWitnessLineOffset()) );
  pDbDimStyle->setDimsd1( false );
  pDbDimStyle->setDimsd2( false );

  OdUInt32 iDimColor  = pDgDimStyle->getDimensionColor();
  OdUInt32 iDimWeight = pDgDimStyle->getDimensionLineWeight();
  OdUInt32 iDimStyle  = pDgDimStyle->getDimensionLineStyleId();

  OdUInt32 iExtColor  = iDimColor;
  OdUInt32 iExtWeight = iDimWeight;
  OdUInt32 iExtStyle  = iDimStyle;

  if( pDgDimStyle->getExtensionLineColorOverrideFlag() )
    iExtColor  = pDgDimStyle->getExtensionLineColor();

  if( pDgDimStyle->getExtensionLineWeightOverrideFlag())
    iExtWeight = pDgDimStyle->getExtensionLineWeight();

  if( pDgDimStyle->getExtensionLineStyleOverrideFlag())
    iExtStyle  = pDgDimStyle->getExtensionLineStyleId();

  OdCmColor extColor;
  setDwgColorByDgnIndex( pDgDimStyle->database(), extColor, iExtColor );
  OdDb::LineWeight extWeight = odDgnImportLineweightMapping( iExtWeight );
  OdDbObjectId lineTypeId;
  setDwgLineTypeIdByDgnLineTypeIndex( pDb, pDgDimStyle->database(), iExtStyle, lineTypeId );

  pDbDimStyle->setDimclre( extColor);
  pDbDimStyle->setDimlwe( extWeight );
  pDbDimStyle->setDimltex1( lineTypeId );
  pDbDimStyle->setDimltex2( lineTypeId );

  setDwgColorByDgnIndex( pDgDimStyle->database(), extColor, iDimColor );
  extWeight = odDgnImportLineweightMapping( iDimWeight );
  setDwgLineTypeIdByDgnLineTypeIndex( pDb, pDgDimStyle->database(), iDimStyle, lineTypeId );

  pDbDimStyle->setDimclrd( extColor);
  pDbDimStyle->setDimlwd( extWeight );
  pDbDimStyle->setDimltype( lineTypeId );

  // Dimension arrow heads

  OdDgDimension::TerminatorArrowHeadType atype = pDgDimStyle->getTerminatorArrowHead();

  if( atype > 2 )
    atype = (OdDgDimension::TerminatorArrowHeadType)(atype & OdDgDimension::kClosed); 

  double dArrowSize  = pDgDimStyle->getTerminatorHeight() > pDgDimStyle->getTerminatorWidth() ? pDgDimStyle->getTerminatorHeight() : pDgDimStyle->getTerminatorWidth();
  double dArrowWidth = 0.0;

  OdDbObjectId idArrow;

  switch( atype )
  {
    case OdDgDimension::kClosed:
    {
      idArrow = OdDmUtil::getArrowId(L"_CLOSEDBLANK", pDb);
    }
    break;

    case OdDgDimension::kFilled:
    {
      idArrow = OdDmUtil::getArrowId(L"", pDb);
    }
    break;

    default:
    {
      idArrow = OdDmUtil::getArrowId(L"_OPEN", pDb);
    }
    break;
  }

  pDbDimStyle->setDimsah(true);
  pDbDimStyle->setDimblk1( idArrow );
  pDbDimStyle->setDimblk2( idArrow );
  pDbDimStyle->setDimasz( dArrowSize );
  pDbDimStyle->setDimtsz( dArrowWidth );
}

void DgnImporter::copyDimStyles(OdDgDatabase* pDg, OdDbDatabase* pDb)
{
  if( _properties->get_EraseUnusedResources() )
    return;

  OdDgDimStyleTablePtr pDgDimStyleTable = pDg->getDimStyleTable(OdDg::kForRead);
  OdDbDimStyleTablePtr pDbDimStyleTable = pDb->getDimStyleTableId().openObject(OdDb::kForWrite);

  OdDgElementIteratorPtr pIter = pDgDimStyleTable->createIterator();

  for(; !pIter->done(); pIter->step() )
  {
    OdDgDimStyleTableRecordPtr pDgDimStyle = pIter->item().openObject(OdDg::kForRead);

    OdString strName = pDgDimStyle->getName();

    if( strName.isEmpty() )
      strName = L"Default";

    if( pDbDimStyleTable->getAt(strName).isNull() )
    {
      OdDbDimStyleTableRecordPtr pDbDimStyle = OdDbDimStyleTableRecord::createObject();
      copyDimStyleProperties( pDgDimStyle, pDbDimStyle, pDb );
      pDbDimStyleTable->add( pDbDimStyle );

      OdDgnImportPathToDwgObject dwgPath;
      dwgPath.m_bExists = true;
      dwgPath.m_idPath.objectIds().push_back(pDbDimStyle->objectId());

      _idMap[pIter->item()] = dwgPath;
    }
  }
}

void DgnImporter::copyDisplayStyles(OdDgDatabase* pDg, OdDbDatabase* pDb)
{
  OdDgDisplayStyleTablePtr pDisplayStyleTable = pDg->getDisplayStyleTable(OdDg::kForRead);
  OdDbDictionaryPtr pDic;

  if( !pDisplayStyleTable.isNull() && pDb )
  {
    pDic = pDb->getVisualStyleDictionaryId(true).openObject( OdDb::kForWrite );

    OdDgElementIteratorPtr pDisplayStyleIter = pDisplayStyleTable->createIterator();

    for(; !pDisplayStyleIter->done(); pDisplayStyleIter->step())
    {
      OdDgDisplayStyleTableRecordPtr pDisplayStyle = pDisplayStyleIter->item().openObject(OdDg::kForRead);
      OdDbVisualStylePtr             pVisualStyle  = OdDbVisualStyle::createObject();

      if( pDic->getAt(pDisplayStyle->getName()).isNull() )
      {
        pVisualStyle->setDescription( OdString::kEmpty );

        pVisualStyle->setType( OdGiVisualStyle::kCustom);

        OdGiDisplayStylePtr giDisplayStyle = pVisualStyle->createDisplayStyle();
        OdGiFaceStylePtr    giFaceStyle    = pVisualStyle->createFaceStyle();
        OdGiEdgeStylePtr    giEdgeStyle    = pVisualStyle->createEdgeStyle();

        giDisplayStyle->setBrightness(0.0);
        giDisplayStyle->setShadowType( pDisplayStyle->getDisplayShadowsFlag()? OdGiDisplayStyle::kShadowsFull : OdGiDisplayStyle::kShadowsNone );
        giDisplayStyle->setDisplaySettingsFlag( OdGiDisplayStyle::kBackgrounds, pDisplayStyle->getUseBackgroundColorOverrideFlag() ? true : false);
        giDisplayStyle->setDisplaySettingsFlag( OdGiDisplayStyle::kMaterials, pDisplayStyle->getDisplayMode() == OdDgDisplayStyleTableRecord::kShaded ? true : false );
        giDisplayStyle->setDisplaySettingsFlag( OdGiDisplayStyle::kLights, pDisplayStyle->getSmoothIgnoreLightsFlag() ? false : true );
        giDisplayStyle->setDisplaySettingsFlag( OdGiDisplayStyle::kTextures, pDisplayStyle->getIgnorePatternMapsFlag() ? false : true );

        switch( pDisplayStyle->getDisplayMode() )
        {
          case OdDgDisplayStyleTableRecord::kWireframe:
          {
            giFaceStyle->setLightingModel( OdGiFaceStyle::kInvisible );
            giFaceStyle->setFaceColorMode( OdGiFaceStyle::kNoColorMode );
            giFaceStyle->setLightingQuality( OdGiFaceStyle::kNoLighting);
            giEdgeStyle->setEdgeModel( OdGiEdgeStyle::kIsolines );
          } break;

          case OdDgDisplayStyleTableRecord::kFilledVisibleEdges:
          {
            giFaceStyle->setLightingModel( OdGiFaceStyle::kConstant );
            giFaceStyle->setLightingQuality( OdGiFaceStyle::kNoLighting);

            if( pDisplayStyle->getUseElementColorOverrideFlag() )
            {
              giFaceStyle->setFaceColorMode( OdGiFaceStyle::kMono );
              ODCOLORREF clrElement = OdDgColorTable::lookupRGB( pDg, pDisplayStyle->getElementColor() );
              OdDgCmColor faceMonoColor;
              faceMonoColor.setRGB( ODGETRED(clrElement), ODGETGREEN(clrElement), ODGETBLUE(clrElement));
              giFaceStyle->setMonoColor( faceMonoColor, true );
            }
            else
            {
              giFaceStyle->setFaceColorMode( OdGiFaceStyle::kObjectColor );
            }

            giEdgeStyle->setEdgeModel( OdGiEdgeStyle::kIsolines );
          } break;

          case OdDgDisplayStyleTableRecord::kVisibleEdges:
          {
            giFaceStyle->setLightingModel( OdGiFaceStyle::kConstant );
            giFaceStyle->setFaceColorMode( OdGiFaceStyle::kBackgroundColor );
            giFaceStyle->setLightingQuality( OdGiFaceStyle::kNoLighting);
            giEdgeStyle->setEdgeModel( OdGiEdgeStyle::kIsolines );
          } break;

          case OdDgDisplayStyleTableRecord::kShaded:
          {
            giFaceStyle->setLightingModel( OdGiFaceStyle::kPhong );

            if( pDisplayStyle->getUseElementColorOverrideFlag() )
            {
              giFaceStyle->setFaceColorMode( OdGiFaceStyle::kMono );
              ODCOLORREF clrElement = OdDgColorTable::lookupRGB( pDg, pDisplayStyle->getElementColor() );
              OdDgCmColor faceMonoColor;
              faceMonoColor.setRGB( ODGETRED(clrElement), ODGETGREEN(clrElement), ODGETBLUE(clrElement));
              giFaceStyle->setMonoColor( faceMonoColor, true );

            }
            else
            {
              giFaceStyle->setFaceColorMode( OdGiFaceStyle::kNoColorMode );
            }

            if( pDisplayStyle->getSmoothIgnoreLightsFlag() )
            {
              giFaceStyle->setLightingModel( OdGiFaceStyle::kConstant );
              giFaceStyle->setLightingQuality( OdGiFaceStyle::kNoLighting );
            }
            else
            {
              giFaceStyle->setLightingQuality( OdGiFaceStyle::kPerVertexLighting );
            }

            if( pDisplayStyle->getDisplayVisibleEdgesFlag() )
            {
              giEdgeStyle->setEdgeModel( OdGiEdgeStyle::kIsolines );
            }
            else
            {
              giEdgeStyle->setEdgeModel( OdGiEdgeStyle::kNoEdges );
            }
          } break;
        }

        if( pDisplayStyle->getUseElementTransparencyOverrideFlag() )
        {
          giFaceStyle->setFaceModifierFlag( OdGiFaceStyle::kOpacity, true );
          giFaceStyle->setOpacityLevel( 1.0 - pDisplayStyle->getElementTransparency(), true );
        }

        if( pDisplayStyle->getUseVisibleEdgeColorFlag() )
        {
          ODCOLORREF clrElement = OdDgColorTable::lookupRGB( pDg, pDisplayStyle->getVisibleEdgeColor() );
          OdDgCmColor elmColor;
          elmColor.setRGB( ODGETRED(clrElement), ODGETGREEN(clrElement), ODGETBLUE(clrElement));
          giEdgeStyle->setEdgeColor( elmColor, true );
        }
        else if( pDisplayStyle->getUseElementColorOverrideFlag() )
        {
          ODCOLORREF clrElement = OdDgColorTable::lookupRGB( pDg, pDisplayStyle->getElementColor() );
          OdDgCmColor elmColor;
          elmColor.setRGB( ODGETRED(clrElement), ODGETGREEN(clrElement), ODGETBLUE(clrElement));
          giEdgeStyle->setEdgeColor( elmColor, true );

        }
        else
        {
          giEdgeStyle->setEdgeModifierFlag(OdGiEdgeStyle::kColor, false);
        }

        if( pDisplayStyle->getUseVisibleEdgeWeightFlag() )
        {
          giEdgeStyle->setEdgeWidth(pDisplayStyle->getVisibleEdgeWeight(), true );
        }
        else if( pDisplayStyle->getUseElementLineWeightOverrideFlag() )
        {
          giEdgeStyle->setEdgeWidth(pDisplayStyle->getElementLineWeight(), true );
        }
        else
        {
          giEdgeStyle->setEdgeModifierFlag(OdGiEdgeStyle::kWidth, false);
        }

        if( pDisplayStyle->getUseHiddenEdgeLineStyleFlag() )
        {
          switch( pDisplayStyle->getHiddenEdgeLineStyle() )
          {
            case OdDgDisplayStyleTableRecord::kDotted:
              giEdgeStyle->setObscuredLinetype( OdGiEdgeStyle::kDotted ); break;
            case OdDgDisplayStyleTableRecord::kDashed:
              giEdgeStyle->setObscuredLinetype( OdGiEdgeStyle::kDashed ); break;
            case OdDgDisplayStyleTableRecord::kLongDash:
              giEdgeStyle->setObscuredLinetype( OdGiEdgeStyle::kLongDash); break;
            case OdDgDisplayStyleTableRecord::kDashDot:
            case OdDgDisplayStyleTableRecord::kShortLongDash:
            case OdDgDisplayStyleTableRecord::kDashDotDot:
              giEdgeStyle->setObscuredLinetype( OdGiEdgeStyle::kMediumLongDash); break;
            case OdDgDisplayStyleTableRecord::kShortDash:
              giEdgeStyle->setObscuredLinetype( OdGiEdgeStyle::kShortDash); break;
            default:
              giEdgeStyle->setObscuredLinetype( OdGiEdgeStyle::kDotted ); break;
          };
        }

        giEdgeStyle->setEdgeStyleFlag( OdGiEdgeStyle::kObscured, pDisplayStyle->getDisplayHiddenEdgesFlag() ? true : false );

        pVisualStyle->setDisplayStyle( *giDisplayStyle );
        pVisualStyle->setEdgeStyle( *giEdgeStyle );
        pVisualStyle->setFaceStyle( *giFaceStyle );
        pVisualStyle->setInternalUseOnly( pDisplayStyle->getHideInPickerFlag() );
        pDic->setAt( pDisplayStyle->getName(), pVisualStyle);

        OdDbObjectId idStyle = pVisualStyle->objectId();

        OdDgnImportPathToDwgObject dwgPath;
        dwgPath.m_bExists = true;
        dwgPath.m_idPath.objectIds().push_back(idStyle);

        _idMap[pDisplayStyle->elementId()] = dwgPath;
      }
    }
  }
}

void DgnImporter::copyMLineStyles(OdDgDatabase* pDg, OdDbDatabase* pDb, const OdDgModelPtr& pBaseModel )
{
  OdDgMultilineStyleTablePtr pMLineStyleTable = pDg->getMultilineStyleTable(OdDg::kForRead);

  if( pMLineStyleTable.isNull() )
  {
    return;
  }

  double dScale = 1.0;

  if( !pBaseModel.isNull() )
  {
    dScale = pBaseModel->convertUORsToWorkingUnits(1.0);
  }

  OdDgElementIteratorPtr pIter = pMLineStyleTable->createIterator();

  for(; !pIter->done(); pIter->step() )
  {
    OdDgMultilineStyleTableRecordPtr pMLineStyle = pIter->item().openObject(OdDg::kForRead);

    OdDgnImportMultilineDescriptor styleDesc;

    styleDesc.m_bTableMLineStyle = true;
    styleDesc.m_uMLineStyle      = (OdUInt64)(pMLineStyle->elementId().getHandle());
    styleDesc.m_uJustification   = 0;
    styleDesc.m_strStyleName     = pMLineStyle->getName();

    if( styleDesc.m_strStyleName.isEmpty() )
    {
      styleDesc.m_strStyleName = OD_T("Default");
    }

    OdDgMultilineSymbology startCap;
    OdDgMultilineSymbology endCap;
    OdDgMultilineSymbology middleCap;

    pMLineStyle->getOriginCap(startCap);
    pMLineStyle->getEndCap(endCap);
    pMLineStyle->getMiddleCap( middleCap );

    bool      bFillMLine = pMLineStyle->getUseFillColorFlag();
    OdUInt32  uFillColor = pMLineStyle->getFillColorIndex();

    styleDesc.m_bShowJoints          = middleCap.getCapLineFlag();
    styleDesc.m_bShowStartCapLine    = startCap.getCapLineFlag();
    styleDesc.m_bShowStartCapInArc   = startCap.getCapInArcFlag();
    styleDesc.m_bShowStartCapOutArc  = startCap.getCapOutArcFlag();
    styleDesc.m_bShowEndCapLine      = endCap.getCapLineFlag();
    styleDesc.m_bShowEndCapInArc     = endCap.getCapInArcFlag();
    styleDesc.m_bShowEndCapOutArc    = endCap.getCapOutArcFlag();
    styleDesc.m_dStartCapAngle       = pMLineStyle->getOriginCapAngle() / 180.0 * OdaPI;
    styleDesc.m_dEndCapAngle         = pMLineStyle->getEndCapAngle() / 180.0 * OdaPI;
    styleDesc.m_bShowFill            = bFillMLine;
    styleDesc.m_uFillColor           = uFillColor;

    for( OdUInt32 i = 0; i < pMLineStyle->getProfilesCount(); i++ )
    {
      OdDgMultilineProfile curProfile;
      pMLineStyle->getProfile( i, curProfile );
      styleDesc.m_arrOffsets.push_back( curProfile.getDistance()*dScale );
      styleDesc.m_arrColorIndexes.push_back( curProfile.symbology().getColorIndex() );
      styleDesc.m_arrLineStyles.push_back( curProfile.symbology().getLineStyleEntryId() );
    }

    OdDbObjectId idStyle = OdDgnImportContext::createMultilineStyle( styleDesc, pDb, pDg );

    OdDgnImportPathToDwgObject dwgPath;
    dwgPath.m_bExists = true;
    dwgPath.m_idPath.objectIds().push_back(idStyle);

    _idMap[pIter->item()] = dwgPath;
    addResourceId(idStyle);
  }
}

void normalizeMinMax( double &newMin, double &newMax )
{
  if (newMin < 0)
    newMin = 0;
  if (newMax > 1)
    newMax = 1;
}

static void copyViewport(OdDgView* pDgView, OdAbstractViewPE* pView, OdRxObject* dbView)
{
  // conversion algorithm is copypasted from OdDgGsManager::_setupDesignModelView
  double dFrontClip, dBackClip;
  // model
  OdGePoint3d ptGlobalOriginOffset = OdGePoint3d::kOrigin;

  OdDgElementId idModel = pDgView->getModelId();

  if( !idModel.isNull() )
  {
    OdDgModelPtr pModel = idModel.openObject(OdDg::kForRead);

    if( !pModel.isNull() )
      ptGlobalOriginOffset = pModel->getGlobalOrigin();
  }

  if (pDgView->getUseCameraFlag())
  {
    OdGeMatrix3d rotation;
    OdGeVector3d upVector = OdGeVector3d::kYAxis, zVector = OdGeVector3d::kZAxis;
    OdGePoint3d cameraPosition;
    pDgView->getCameraPosition( cameraPosition );
    pDgView->getCameraRotation( rotation );
    rotation.transposeIt();

    cameraPosition -= ptGlobalOriginOffset.asVector();

    OdGeVector3d xVector = OdGeVector3d::kXAxis;

    upVector.transformBy( rotation );
    zVector.transformBy( rotation );

    pView->setView(dbView, cameraPosition, zVector, upVector, 1., 1., true );

    //TD manages it as a camera with fixed size, so we should juggle with a kind of crop-factor
    {
      //take a native rectangle
      OdGeExtents2d visibleRectangle;
      pDgView->getCameraVisibleRectangle( visibleRectangle );
      OdGePoint2d min = visibleRectangle.minPoint(), max = visibleRectangle.maxPoint();

      //emulate new lens
      double oldFocus = pDgView->getCameraFocalLength();
      double newFocus;
      {
        double a = fabs( min.x ), b = fabs( min.y ), c = fabs( max.x ), d = fabs( max.y );
        double maxX = a > c ? a : c, maxY = b > d ? b : d;

        double focusX = .018 * oldFocus / maxX, focusY = .012 * oldFocus / maxY; //TD uses 36*24mm camera
        newFocus = focusX < focusY ? focusX : focusY;
      }

      double newMin, newMax;

      //right edge sticks to the right edge of the view (left one goes outside)
      if( fabs( min.x ) < fabs( max.x ) )
      {
        newMax = 1.;
        newMin = 1. - 2. * max.x / ( max.x - min.x );
      }
      //left edge stick to the left edge of the view (right one goes outside right)
      else
      {
        newMin = 0.;
        newMax = -2. * min.x / ( max.x - min.x );
      }
      normalizeMinMax(newMin, newMax);
      min.x = newMin;
      max.x = newMax;

      //the same for bottom/top edges
      if( fabs( min.y ) < fabs( max.y ) )
      {
        newMax = 1.;
        newMin = 1. - 2. * max.y / ( max.y - min.y );
      }
      else
      {
        newMin = 0.;
        newMax = -2. * min.y / ( max.y - min.y );
      }
      normalizeMinMax(newMin, newMax);
      min.y = newMin;
      max.y = newMax;

      //try to set it
      pView->setLensLength(dbView, newFocus * 1000. ); //it was in meters and is needed as millimeters
      pView->setViewport(dbView, min, max );

      dFrontClip = pDgView->getCameraFrontClippingDistance();
      dBackClip  = pDgView->getCameraBackClippingDistance();
    }
  }
  else
  {
    OdGeMatrix3d rotation;
    pDgView->getOrthographyRotation( rotation );
    rotation.transposeIt(); //it is an inversion for correct matrices of rotation

    //apply that rotation
    OdGeVector3d upVector = OdGeVector3d::kYAxis, rightVector = OdGeVector3d::kXAxis, directionVector = OdGeVector3d::kZAxis;
    upVector.transformBy( rotation );
    rightVector.transformBy( rotation );
    directionVector.transformBy( rotation );

    //get the central point (target) and the extent
    double width, height;
    OdGePoint3d target;
    {
      OdGeExtents3d visibleRectangle;
      pDgView->getOrthographyVisibleBox( visibleRectangle );

      const OdGePoint3d &min = visibleRectangle.minPoint(), &max = visibleRectangle.maxPoint();

      target = ( min + max.asVector() ) / 2.;
      target.transformBy( rotation );

      width = fabs(max.x - min.x);
      height = fabs(max.y - min.y);
    }

    OdGePoint3d ptPosition = pDgView->getOrthographyPosition();
    OdGeVector3d vrPosDirection = directionVector;
    OdGePlane targetPlane( target, directionVector );
    OdGePoint3d ptPositionProject = ptPosition.orthoProject(targetPlane);
    double dPosDistance = ptPosition.distanceTo(ptPositionProject);

    if( OdZero(dPosDistance) )
    {
      dPosDistance = 1.0;
    }
    else
    {
      vrPosDirection = ptPosition - ptPositionProject;
      vrPosDirection.normalize();
    }

    OdGePoint3d ptPositionNew = target + vrPosDirection*dPosDistance - ptGlobalOriginOffset.asVector();

    pView->setView(dbView, ptPositionNew - directionVector, directionVector, upVector, width, height, false );

    dFrontClip = pDgView->getOrthographyFrontClippingDistance();
    dBackClip  = pDgView->getOrthographyBackClippingDistance();
  }
  if( pDgView->getShowClipFrontFlag() )
  {
    pView->setFrontClipOn(dbView, true);
    pView->setFrontClipDistance(dbView, 1 - dFrontClip );
    pView->setFrontClipAtEyeOn( dbView, false);
  }

  if( pDgView->getShowClipBackFlag() )
  {
    pView->setBackClipOn(dbView, true);
    pView->setBackClipDistance(dbView, 1 - dBackClip );
  }

  OdDgDisplayStyleViewPEPtr pDisplayStyleViewPE = pDgView;

  if( !pDisplayStyleViewPE.isNull() && !pDisplayStyleViewPE->getDisplayStyle(pDgView).isNull() )
  {
    OdDbObjectId idVisualStyle = OdDgnImportContext::getObjectId(pDisplayStyleViewPE->getDisplayStyle(pDgView));

    if( !idVisualStyle.isNull() )
    {
      OdDbVisualStylePtr pVisualStyle = idVisualStyle.openObject(OdDb::kForRead);
      const OdGiFaceStyle& faceStyle = pVisualStyle->faceStyle();
      OdDb::RenderMode rm = OdDb::kWireframe;

      switch( faceStyle.lightingModel() )
      {
        case OdGiFaceStyle::kInvisible: rm = OdDb::kWireframe; break;
        case OdGiFaceStyle::kConstant:  rm = OdDb::kHiddenLine; break;
        case OdGiFaceStyle::kPhong:     rm = OdDb::kGouraudShaded; break;
      }
      pView->setRenderMode( dbView, rm );
      pView->setVisualStyle(dbView, idVisualStyle);
    }
  }
}
void DgnImporter::copyViewports(OdDgDatabase* pDg, OdDbDatabase* pDb, const OdDgModelPtr& pBaseModel )
{
  OdInt32 uViewIndex = _properties->get_ImportViewIndex();

  OdDgViewGroupPtr viewGroup;

  OdDgViewGroupTablePtr pViewGroupTable = pDg->getViewGroupTable(OdDg::kForRead);

  if( !pViewGroupTable.isNull() && !pBaseModel.isNull() )
  {
    OdDgElementIteratorPtr pVgIter = pViewGroupTable->createIterator();

    for(; !pVgIter->done(); pVgIter->step() )
    {
      OdDgViewGroupPtr pVg = pVgIter->item().openObject(OdDg::kForRead);

      if( !pVg.isNull() && pVg->getModelId() == pBaseModel->elementId() )
      {
        viewGroup = pVg;
        break;
      }
    }
  }
  else
  {
    viewGroup = OdDgViewGroup::cast(pDg->getActiveViewGroupId().openObject());
  }

  if (!viewGroup.isNull())
  {
    OdDgElementIteratorPtr iterator = viewGroup->createIterator();

    OdDgViewPtr pDgView0;

    bool bCreateActiveViewport = false;

    while( !iterator->done() )
    {
      OdDgViewPtr pDgView = OdDgView::cast(iterator->item().openObject());

      if( pDgView0.isNull() )
      {
        pDgView0 = pDgView;
      }

      if( uViewIndex >= 0  && uViewIndex < 8 )
      {
        if( pDgView->getIndex() != uViewIndex )
        {
          iterator->step();
          continue;
        }
      }
      else
      {
        if( !pDgView->getVisibleFlag() )
        {
          iterator->step();
          continue;
        }
        else
        {
          _properties->put_ImportViewIndex( pDgView->getIndex() );
        }
      }

      bCreateActiveViewport = true;

      OdDbViewportTablePtr vt = pDb->getViewportTableId().safeOpenObject();
      OdDbViewportTableRecordPtr dbView = vt->getActiveViewportId().safeOpenObject(OdDb::kForWrite);
      OdAbstractViewPEPtr pView = OdAbstractViewPE::cast(dbView);
      copyViewport(pDgView, pView, dbView);
      break;
    }

    if( !bCreateActiveViewport )
    {
      OdDbViewportTablePtr vt = pDb->getViewportTableId().safeOpenObject();
      OdDbViewportTableRecordPtr dbView = vt->getActiveViewportId().safeOpenObject(OdDb::kForWrite);
      OdAbstractViewPEPtr pView = OdAbstractViewPE::cast(dbView);
      copyViewport(pDgView0, pView, dbView);
    }
  }
  // TODO: other viewports
}

void DgnImporter::copyNamedViews(OdDgDatabase* pDg, OdDbDatabase* pDb)
{
  OdDbViewTablePtr vt = pDb->getViewTableId().safeOpenObject(OdDb::kForWrite);
  for (OdDgElementIteratorPtr i = pDg->getNamedViewTable()->createIterator(); !i->done(); i->step())
  {
    OdDgViewPtr v = i->item().safeOpenObject();
    OdString name = v->getName(),
             repairedName;
    if (   OdDbSymUtil::repairSymbolName(repairedName, name, pDb) == eOk
        && !repairedName.isEmpty())
      name = repairedName;
    if (vt->has(name))
      continue;
    OdDbViewTableRecordPtr dbView = OdDbViewTableRecord::createObject();
    dbView->setName(name);

    OdDbObjectId idItem = pDb->getOdDbObjectId( OdDbHandle((OdUInt64)(i->item().getHandle())));

    if( idItem.isNull() )
    {
      pDb->addOdDbObject(dbView, vt->objectId(), i->item().getHandle());
    }
    else
    {
      pDb->addOdDbObject(dbView, vt->objectId() );
    }

    vt->add(dbView);
    OdDgModelPtr m = v->getModelId().openObject();
    if (!m.isNull() && m->getType() == OdDgModel::kSheetModel)
    {
      dbView->setIsPaperspaceView(true);
      name = m->getName();
      if (   OdDbSymUtil::repairSymbolName(repairedName, name, pDb) == eOk
          && !repairedName.isEmpty())
        name = repairedName;
      dbView->setLayout(pDb->findLayoutNamed(name));
    }
    if (v->getUseCameraFlag())
    {
      dbView->setPerspectiveEnabled(true);
      OdGePoint3d position; v->getCameraPosition(position);
      OdGeMatrix3d rotation; v->getCameraRotation(rotation);
      dbView->setLensLength(v->getCameraFocalLength());
      //dbView->set
      OdGeExtents2d extent;
      v->getCameraVisibleRectangle(extent);
      dbView->setFrontClipDistance(v->getCameraFrontClippingDistance());
      dbView->setBackClipDistance(v->getCameraBackClippingDistance());
      //double v->getCameraLensAngle() const;
      //v->
    }
    else
    {
      dbView->setPerspectiveEnabled(false);
      OdGeExtents3d box;
      v->getOrthographyVisibleBox(box);
      OdGeMatrix3d rotation;
      v->getOrthographyRotation(rotation);
      dbView->setFrontClipDistance(v->getOrthographyFrontClippingDistance());
      dbView->setBackClipDistance(v->getOrthographyBackClippingDistance());
      dbView->setTarget(v->getOrthographyPosition());
    }
  }
}

//--------------------------------------------------------------------------------------------------------------

void DgnImporter::setModelLevelMask( OdDgModel* m, OdInt8 iViewIndex )
{
  if( m && (iViewIndex >= 0) && (iViewIndex < 8) )
  {
    bool bLevelMaskAdded = false;

    OdDgViewGroupTablePtr pViewGroupTable = m->database()->getViewGroupTable(OdDg::kForRead);

    OdDgElementIteratorPtr pViewGroupIter = pViewGroupTable->createIterator();

    for(; !pViewGroupIter->done(); pViewGroupIter->step() )
    {
      OdDgViewGroupPtr pVG = pViewGroupIter->item().openObject(OdDg::kForRead);

      if( !pVG.isNull() && pVG->getModelId() == m->elementId() )
      {
        OdDgElementIteratorPtr pViewIter = pVG->createIterator();

        for(; !pViewIter->done(); pViewIter->step() )
        {
          OdDgViewPtr pView = pViewIter->item().openObject(OdDg::kForRead);

          if( !pView.isNull() && (pView->getIndex() == (OdUInt8)(iViewIndex)) )
          {
            OdDgElementIteratorPtr pLMIter = pView->createIterator();

            if( !pLMIter->done() )
            {
              OdDgLevelMaskPtr pLevelMask = pLMIter->item().openObject(OdDg::kForRead);

              if( !pLevelMask.isNull() )
              {
                OdDgnImportContext::pushLevelMask(pLevelMask, m->database()->getLevelTable(OdDg::kForRead) );
                bLevelMaskAdded = true;
              }
            }

            break;
          }
        }

        break;
      }
    }

    if( !bLevelMaskAdded )
    {
      OdDgLevelMaskPtr pLevelMask;
      OdDgnImportContext::pushLevelMask(pLevelMask, m->database()->getLevelTable(OdDg::kForRead) );
    }
  }
}

//--------------------------------------------------------------------------------------------------------------

void DgnImporter::removeModelLevelMask()
{
  OdDgnImportContext::popLevelMask();
}


}
