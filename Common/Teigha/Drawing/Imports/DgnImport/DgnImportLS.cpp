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

#include "DgnImportLS.h"
#include "DgnImportPE.h"
#include "DgnImportImpl.h"

#include <float.h> // for DBL_MAX

#include <StaticRxObject.h>
#include <Gi/GiLinetype.h>
#include <Gi/GiNonEntityTraits.h>

#include <DgDatabase.h>
#include <DgLineStyleTableRecord.h>
#include <DgLineStyleDefTableRecord.h>
#include <DgTable.h>

#include <DbDatabase.h>
#include <DbBlockTable.h>
#include <DbBlockTableRecord.h>
#include <DbEntity.h>

#include <DgnLS/DbLSMisc.h>
#include <DgnLS/DbLSDefinition.h>
#include <DgnLS/DbLSInternalComponent.h>
#include <DgnLS/DbLSPointComponent.h>
#include <DgnLS/DbLSStrokePatternComponent.h>
#include <DgnLS/DbLSSymbolComponent.h>
#include <DgnLS/DbLSCompoundComponent.h>
#include "DbSymUtl.h"
#include "DbIdMapping.h"

DgnLSImporter::DgnLSImporter(OdDgDatabase *pSrcDb, OdDbDatabase *pDstDb)
  : m_pSrcDb(pSrcDb)
  , m_pDstDb(pDstDb)
{
  ::oddbDgnLSInitializeImportUID(m_curUID);
  for (int nCtr = 0; nCtr < kLSInternalComponent; nCtr++)
    m_compCounters[nCtr] = 0;
  m_pLTTable = m_pDstDb->getLinetypeTableId().safeOpenObject(OdDb::kForWrite);
}

OdDbLinetypeTableRecordPtr DgnLSImporter::copyLinetype(OdDgLineStyleTableRecord *pLS)
{
  if (!pLS)
    return OdDbLinetypeTableRecordPtr();
  m_pLS = pLS;
  m_fUORPerStorage = 1.0;
  m_fUORPerMaster = 1.0;
  bool bIsElement;
  bool bIsInternal = false;
  OdDgLineStyleDefTableRecordPtr pLsDef;
  if (pLS->getRefersToElementFlag())
  {
    if (m_pSrcDb)
    {
      OdDgElementId lsDefId = pLS->getRefersToId();
      pLsDef = OdDgLineStyleDefTableRecord::cast(lsDefId.openObject());
      
      OdDgModelPtr pDefModel = m_pSrcDb->getDefaultModelId().openObject();
      if (pDefModel.get())
      {
        OdDgModel::StorageUnitDescription unitDesc;
        pDefModel->getStorageUnit(unitDesc);
        m_fUORPerStorage = unitDesc.m_uorPerStorageUnit;
        OdDgModel::UnitDescription masterUnits;
        pDefModel->getMasterUnit(masterUnits);
        //m_fUORPerMaster = unitDesc.m_uorPerStorageUnit / unitDesc.m_numerator *
        //                  unitDesc.m_denominator / masterUnits.m_numerator *
        //                  masterUnits.m_denominator;
        m_fUORPerMaster = unitDesc.m_uorPerStorageUnit * ((unitDesc.m_numerator /
                          unitDesc.m_denominator) / (masterUnits.m_numerator /
                          masterUnits.m_denominator));
      }
    }
    bIsElement = true;
  }
  else // from RSC
  {
    OdDgLineStyleDefTablePtr pLineStyleDef = m_pSrcDb->getLineStyleDefTable();
    pLsDef = pLineStyleDef->getRscLineStyleDef(pLS->getName());
    bIsElement = false;
    if (pLsDef.isNull() && pLS->getEntryId() < 8 && pLS->getEntryId() > 0)
      bIsInternal = true;
  }
  if ((pLsDef.isNull() || pLsDef->getResource().isNull()) && !bIsInternal)
    return OdDbLinetypeTableRecordPtr();
  OdDbLinetypeTableRecordPtr pRec = OdDbLinetypeTableRecord::createObject();
  if (!bIsInternal)
  {
    OdString name = pLsDef->getName(),
             repairedName;
    if (OdDbSymUtil::repairSymbolName(repairedName, name, m_pDstDb, false, L' ', false) == eOk
        && !repairedName.isEmpty())
      name = repairedName;
    if (name.isEmpty()) // Microstation skips such linestyles while exporting DWG
      return OdDbLinetypeTableRecordPtr();
    // Avoid addition of linetype with similar name (could be from secondary database)
    if (m_pLTTable->has(name)) // DGNImportEmptyName_10194_VK
      return OdDbLinetypeTableRecord::cast(m_pLTTable->getAt(name, OdDb::kForRead));
    pRec->setName(name);
    pRec->setComments(pLsDef->getName());
    m_pDstDb->addOdDbObject(pRec, m_pDstDb->getLinetypeTableId());
  }
  else
  {
    OdString name;
    name.format(OD_T("DGN%u"), pLS->getEntryId());
    // Avoid addition of linetype with similar name (could be from secondary database)
    if (m_pLTTable->has(name)) // DGNImportEmptyName_10194_VK
      return OdDbLinetypeTableRecord::cast(m_pLTTable->getAt(name, OdDb::kForRead));
    pRec->setName(name);
    pRec->setComments(OD_T("internal stroke component"));
  }
  m_pLTTable->add(pRec);
  processLineStyle(pRec, pLsDef, bIsElement, bIsInternal);
  return pRec;
}

OdDgLineStyleDefTableRecordPtr findLineStyleDefIntoDatabase( const OdDgDatabase* pDb, 
                                                               OdUInt32 uEntryId,
                                                                 OdDgLineStyleResource::OdLsResourceType uResType
                                                            )
{
  OdDgLineStyleDefTableRecordPtr pRet;

  if( !pDb )
  {
    return pRet;
  }

  OdDg::LineStyleType uActualType = OdDg::kLsTypeInternal;

  if( (uResType == OdDgLineStyleResource::kLsPointSymbolRes) || 
    (uResType == OdDgLineStyleResource::kLsPointSymbolResV7)
    )
  {
    uActualType = OdDg::kLsTypePointSymbol;
  }
  else if( uResType == OdDgLineStyleResource::kLsCompoundRes )
  {
    uActualType = OdDg::kLsTypeCompound;
  }
  else if( uResType == OdDgLineStyleResource::kLsLinePointRes )
  {
    uActualType = OdDg::kLsTypeLinePoint;
  }
  else if( uResType == OdDgLineStyleResource::kLsLineCodeRes )
  {
    uActualType = OdDg::kLsTypeLineCode;
  }

  if( uActualType == OdDg::kLsTypeInternal )
  {
    return pRet;
  }

  OdDgLineStyleDefTablePtr pLineStyleDefTable = pDb->getLineStyleDefTable();

  OdUInt32 uTypeCount = 0;

  if( !pLineStyleDefTable.isNull() )
  {
    OdDgElementIteratorPtr pElmIter = pLineStyleDefTable->createIterator();

    for(; !pElmIter->done(); pElmIter->step() )
    {
      OdDgElementPtr pElm = pElmIter->item().openObject(OdDg::kForRead);

      if( pElm->isKindOf(OdDgLineStyleDefTableRecord::desc() ) )
      {
        OdDgLineStyleDefTableRecordPtr pLsDef = pElm;

        if( pLsDef->getType() == uActualType )
        {
          if( uTypeCount == uEntryId )
          {
            pRet = pLsDef;
            break;
          }
          uTypeCount++;
        }
      }
    }
  }

  return pRet;
}

bool DgnLSImporter::processLineStyle(OdDbLinetypeTableRecord *pRec, OdDgLineStyleDefTableRecord *pDef, bool bIsElement, bool bIsInternal)
{
  // 1) Process for primitive representation (if DgnLS module doesn't supported by client system)
  processPrimitiveRepresentation(pRec, pDef, bIsInternal);
  // 2) Process for DgnLS records
  return processDgnLSRepresentation(pRec, pDef, bIsElement, bIsInternal);;
}

bool DgnLSImporter::processPrimitiveRepresentation(OdDbLinetypeTableRecord *pRec, OdDgLineStyleDefTableRecord *pDef, bool bIsInternal)
{
  if (bIsInternal)
    return processPrimitive_Internal(pRec);
  switch (pDef->getType())
  {
    case OdDg::kLsTypeLinePoint:
      {
        OdDgLinePointResourcePtr pRes = OdDgLinePointResource::cast(pDef->getResource());
        if (!pRes.isNull())
        {
          OdDgLineStyleDefTableRecordPtr pLsBasePattern;
          if (pRes->getBasePatternHandleId() != 0)
          {
            OdDgElementId depId = m_pSrcDb->getElementId(OdDbHandle(pRes->getBasePatternHandleId()));
            pLsBasePattern = OdDgLineStyleDefTableRecord::cast(depId.openObject(OdDg::kForRead));
          }
          else
          {
            OdDgLineStyleDefTablePtr pLineStyleDef = m_pSrcDb->getLineStyleDefTable();
            pLsBasePattern = pLineStyleDef->getRscLineStyleDef(pRes->getBasePatternEntryId(), (OdDgLineStyleResource::OdLsResourceType)(pRes->getBasePatternType()));
          }
          if (!pLsBasePattern.isNull())
          {
            if (!pLsBasePattern.isNull())
            {
              OdDgLineCodeResourcePtr pStroke = OdDgLineCodeResource::cast(pLsBasePattern->getResource());
              if (!pStroke.isNull())
              {
                return processPrimitive_Stroke(pRec, pStroke);
              }
            }
          }
        }
      }
    break;
    case OdDg::kLsTypeLineCode:
      {
        OdDgLineCodeResourcePtr pRes = OdDgLineCodeResource::cast(pDef->getResource());
        if (!pRes.isNull())
        {
          return processPrimitive_Stroke(pRec, pRes);
        }
      }
    break;
  }
  return true;
}

bool DgnLSImporter::processPrimitive_Internal(OdDbLinetypeTableRecord *pRec)
{
#if 0 // The values is too strange in TG. I need some coef (129.45976396322523903698080112251) to convert into Px
  struct TraitsGetter : public OdStaticRxObject<OdGiLinetypeTraits>
  {
    double m_fPatternLength;
    OdArray<OdGiLinetypeDash> m_dashes;
    double m_fScale;

    TraitsGetter()
      : m_fPatternLength(0.0)
      , m_fScale(0.0)
    {
    }

    double patternLength() const { return m_fPatternLength; }
    void dashes(OdArray<OdGiLinetypeDash>& dashes) { dashes = m_dashes; }
    double scale() const { return m_fScale; }
    void setDashes(const OdArray<OdGiLinetypeDash>& dashes) { m_dashes = dashes; }
    void setScale(double scale) { m_fScale = scale; }
    void setPatternLength(double patternLength) { m_fPatternLength = patternLength; }
  } traitsGetter;
  m_pLS->setAttributes(&traitsGetter);
  ODA_ASSERT(!traitsGetter.m_dashes.isEmpty());
#endif
  // Do it manually
  double fPatternLength = 0.0;
  OdVector<OdGiLinetypeDash> dashes(6, 2);
  OdGiLinetypeDash *pCurDash;
  switch (m_pLS->getEntryId())
  {
    case 0: // Continuous
      //dashes.resize(1);
      //pCurDash = dashes.asArrayPtr();
      //pCurDash->length = 1.0;
      //fPatternLength = 1.0;
      fPatternLength = 0.0;
    break;
    case 1: // DGN1
      dashes.resize(2);
      pCurDash = dashes.asArrayPtr();
      (++pCurDash)->length = -6.0;
      fPatternLength = 6.0;
    break;
    case 2: // DGN2
      dashes.resize(2);
      pCurDash = dashes.asArrayPtr();
      pCurDash->length = 4.0;
      (++pCurDash)->length = -4.0;
      fPatternLength = 8.0;
    break;
    case 3: // DGN3
      dashes.resize(2);
      pCurDash = dashes.asArrayPtr();
      pCurDash->length = 8.0;
      (++pCurDash)->length = -4.0;
      fPatternLength = 12.0;
    break;
    case 4: // DGN4
      dashes.resize(4);
      pCurDash = dashes.asArrayPtr();
      pCurDash->length = 8.0;
      (++pCurDash)->length = -4.0;
      pCurDash++;
      (++pCurDash)->length = -4.0;
      fPatternLength = 16.0;
    break;
    case 5: // DGN5
      dashes.resize(2);
      pCurDash = dashes.asArrayPtr();
      pCurDash->length = 2.0;
      (++pCurDash)->length = -4.0;
      fPatternLength = 6.0;
    break;
    case 6: // DGN6
      dashes.resize(6);
      pCurDash = dashes.asArrayPtr();
      (++pCurDash)->length = -3.0;
      (++pCurDash)->length = 6.0;
      (++pCurDash)->length = -3.0;
      pCurDash++;
      (++pCurDash)->length = -3.0;
      fPatternLength = 15.0;
    break;
    case 7: // DGN7
      dashes.resize(4);
      pCurDash = dashes.asArrayPtr();
      pCurDash->length = 8.0;
      (++pCurDash)->length = -3.0;
      (++pCurDash)->length = 2.0;
      (++pCurDash)->length = -3.0;
      fPatternLength = 16.0;
    break;
    default:
      ODA_FAIL();
  }
  double scaleCoef = 1.0;
  { // Compute scale coefficient
    OdDgViewGroupPtr viewGroup = OdDgViewGroup::cast(m_pSrcDb->getActiveViewGroupId().openObject());
    if (!viewGroup.isNull())
    {
      OdDgElementIteratorPtr iterator = viewGroup->createIterator();
      if (!iterator->done())
      {
        OdDgViewPtr pView = OdDgView::cast(iterator->item().openObject());
        OdGsDCRect screenRect = pView->getViewRectangle();
        double dWidth;
        if (pView->getUseCameraFlag())
        {
          OdGeExtents2d visRect;
          pView->getCameraVisibleRectangle(visRect);
          dWidth = visRect.maxPoint().x - visRect.minPoint().x;
        }
        else
        {
          OdGeExtents3d visRect;
          pView->getOrthographyVisibleBox(visRect);
          dWidth = visRect.maxPoint().x - visRect.minPoint().x;
        }
        long int dWidth2 = screenRect.m_max.x - screenRect.m_min.x;
        if (dWidth2 == 0) dWidth2 = 1;
        if (OdZero(dWidth)) dWidth = 1.0;
        scaleCoef = dWidth / Od_abs(dWidth2);
#if 0
        // Unfortunately this is unknown from where was get magic numbers in Dg, but this coef makes linetype representation correct
        scaleCoef *= 129.45976396322523903698080112251; // recompute from Dg hardcoded values to Px
#endif
      }
    }
  }
#if 0
  pRec->setPatternLength(traitsGetter.m_fPatternLength * scaleCoef);
  if (!traitsGetter.m_dashes.empty())
  {
    OdUInt32 nDashes = traitsGetter.m_dashes.size();
    pRec->setNumDashes((int)nDashes);
    const OdGiLinetypeDash *pDashes = traitsGetter.m_dashes.getPtr();
    for (OdUInt32 nDash = 0; nDash < nDashes; nDash++)
    {
      pRec->setDashLengthAt((int)nDash, pDashes[nDash].length * scaleCoef);
    }
  }
#else
  pRec->setPatternLength(fPatternLength * scaleCoef);
  if (!dashes.empty())
  {
    OdUInt32 nDashes = dashes.size();
    pRec->setNumDashes((int)nDashes);
    const OdGiLinetypeDash *pDashes = dashes.getPtr();
    for (OdUInt32 nDash = 0; nDash < nDashes; nDash++)
    {
      pRec->setDashLengthAt((int)nDash, pDashes[nDash].length * scaleCoef);
    }
  }
#endif
  return true;
}

bool DgnLSImporter::processPrimitive_Stroke(OdDbLinetypeTableRecord *pRec, OdDgLineCodeResource *pRes)
{
  OdUInt32 nStrokes = pRes->getStrokeCount();
  if (nStrokes > 1)
  {
    pRec->setNumDashes((int)nStrokes);
    OdDgLineCodeResourceStrokeData data;
    double dLength = 0.0;
    for (OdUInt32 nStroke = 0; nStroke < nStrokes; nStroke++)
    {
      pRes->getStroke(nStroke, data);
      pRec->setDashLengthAt((int)nStroke, ((data.getDashFlag()) ? data.getLength() : -data.getLength()) / m_fUORPerStorage);
      dLength += fabs(pRec->dashLengthAt((int)nStroke));
    }
    pRec->setPatternLength(dLength);
  }
  return true;
}

bool DgnLSImporter::processDgnLSRepresentation(OdDbLinetypeTableRecord *pRec, OdDgLineStyleDefTableRecord *pDef, bool bIsElement, bool bIsInternal)
{
  OdDbLSDefinitionPtr pLSDef = OdDbLSDefinition::createObject();
  if (bIsInternal)
  {
    pLSDef->setIsContinuous(true);
  }
  else
  {
    pLSDef->setIsContinuous(m_pLS->getContinuousFlag());
    pLSDef->setIsElement(bIsElement);
    pLSDef->setIsPhysical(m_pLS->getPhysicalFlag());
    if (bIsElement)
    {
      //m_fUORPerMaster = m_fUORPerStorage / m_fUORPerMaster; // ??
      pLSDef->setUnitsType(kMSLSUORS); // #12094 : scale by master units
      pLSDef->setUnitScale(1.0 / /*m_fUORPerStorage*/m_fUORPerMaster);
      m_fUORPerStorage = 1.0;
    }
    pLSDef->setIsSCScaleIndependent(m_pLS->getSharedCellScaleIndependentFlag());
    pLSDef->setIsSnappable(m_pLS->getSnappableFlag());
    OdString name = m_pLS->getName(),
             repairedName;
    if (OdDbSymUtil::repairSymbolName(repairedName, name, m_pDstDb, false, L' ', false) == eOk
        && !repairedName.isEmpty())
      name = repairedName;
    pLSDef->setName(name);
  }
  OdDbLSComponentPtr pComponent;
  if (bIsInternal)
    pComponent = processDgnLS_Internal(m_pLS->getEntryId(), true);
  else
    pComponent = processDgnLS_Component(pDef->database(), pDef->getResource(), pDef->elementId().getHandle(), !bIsElement );
  if (pComponent.isNull()) // @@@ temporary
    return false;
  ODA_ASSERT(!pComponent.isNull());
  pLSDef->setComponent(pComponent->id());
  pLSDef->setComponentUID(pComponent->componentUID());
  pRec->createExtensionDictionary();
  OdDbDictionaryPtr pDict = OdDbDictionary::cast(pRec->extensionDictionary().openObject(OdDb::kForWrite));
  pDict->setAt(::oddbDgnLSGetDefinitionKeyName(), pLSDef);
  return true;
}

OdDbLSComponentPtr DgnLSImporter::processDgnLS_Component(const OdDgDatabase* pDgnDb, 
                                                           OdDgLineStyleResource *pRes, 
                                                             OdUInt64 handleId,
                                                               bool bFromRscFile)
{
  if (!pRes)
    return OdDbLSComponentPtr();
  switch (pRes->getType())
  {
    //case OdDg::kLsTypeInternal:
    //case OdDg::kLsTypePointSymbol:
    case OdDg::kLsTypeCompound:
    return processDgnLS_Compound(pDgnDb, pRes, handleId, bFromRscFile);
    case OdDg::kLsTypeLineCode:
    return processDgnLS_Stroke(pRes, handleId);
    case OdDg::kLsTypeLinePoint:
    return processDgnLS_Point(pDgnDb, pRes, handleId, bFromRscFile);
    default: return OdDbLSComponentPtr();
  }
}

OdDbLSComponentPtr DgnLSImporter::processDgnLS_Internal(OdUInt32 lineCode, bool bFromBase)
{
  OdDbDictionaryPtr pDict = ::oddbDgnLSGetComponentsDictionary(m_pDstDb, OdDb::kForWrite, true);
  OdDbLSComponentPtr pSearch = OdDbLSComponent::cast(pDict->getAt(OdString().format(OD_T("DGN%u"), lineCode), OdDb::kForRead));
  if (!pSearch.isNull())
    return pSearch;
  OdDbLSInternalComponentPtr pComponent = OdDbLSInternalComponent::createObject();
  // Component
  pComponent->setComponentType(kLSInternalComponent);
  if (bFromBase)
    pComponent->setDescription(OdString().format(OD_T("DGN%u"), lineCode));
  pComponent->setComponentUID(m_curUID); getUIDIncrement();
  // Internal
  pComponent->setHardwareStyle(lineCode);
  pComponent->setIsHardwareStyle(!bFromBase && lineCode != 0);
  if (pComponent->isHardwareStyle())
    pComponent->setLineCode(0x80000000 + lineCode);
  else
    pComponent->setLineCode(lineCode);
  switch (lineCode)
  {
    case 0: // Continuous
      pComponent->insertStroke(DBL_MAX)->setDash();
    break;
    case 1: // DGN1
      pComponent->insertStroke(0.0)->setDash();
      pComponent->insertStroke(0.006)->setGap();
    break;
    case 2: // DGN2
      pComponent->insertStroke(0.004)->setDash();
      pComponent->insertStroke(0.004)->setGap();
    break;
    case 3: // DGN3
      pComponent->insertStroke(0.008)->setDash();
      pComponent->insertStroke(0.004)->setGap();
    break;
    case 4: // DGN4
      pComponent->insertStroke(0.008)->setDash();
      pComponent->insertStroke(0.004)->setGap();
      pComponent->insertStroke(0.0)->setDash();
      pComponent->insertStroke(0.004)->setGap();
    break;
    case 5: // DGN5
      pComponent->insertStroke(0.002)->setDash();
      pComponent->insertStroke(0.004)->setGap();
    break;
    case 6: // DGN6
      pComponent->insertStroke(0.0)->setDash();
      pComponent->insertStroke(0.003)->setGap();
      pComponent->insertStroke(0.006)->setDash();
      pComponent->insertStroke(0.003)->setGap();
      pComponent->insertStroke(0.0)->setDash();
      pComponent->insertStroke(0.003)->setGap();
    break;
    case 7: // DGN7
      pComponent->insertStroke(0.008)->setDash();
      pComponent->insertStroke(0.003)->setGap();
      pComponent->insertStroke(0.002)->setDash();
      pComponent->insertStroke(0.003)->setGap();
    break;
    default:
      ODA_FAIL();
  }
  pDict->setAt(OdString().format(OD_T("DGN%u"), lineCode), pComponent);
  return pComponent;
}

//OdDbObjectId DgnLSImporter::processDgnLS_Internal(OdDgLineStyleResource *pRec)
//{
//}

OdDbLSComponentPtr DgnLSImporter::processDgnLS_Stroke(OdDgLineStyleResource *pRes, OdUInt64 handleId)
{
  if (!pRes)
    return OdDbLSComponentPtr();
  CompMap::iterator it = m_componentsMap.find(pRes);
  if (it != m_componentsMap.end())
    return OdDbLSComponent::cast(it->second.openObject());
  OdDbLSStrokePatternComponentPtr pComponent = OdDbLSStrokePatternComponent::createObject();
  OdDgLineCodeResource *pFrom = static_cast<OdDgLineCodeResource*>(pRes);
  // Component
  pComponent->setComponentType(kLSStrokePatternComponent);
  pComponent->setDescription(pFrom->getDescription());
  pComponent->setComponentUID(m_curUID); getUIDIncrement();
  // Stroke
  if (pFrom->getAutoPhaseFlag())
  {
    /* #12125 : Acad clamps fraction to [0-1] range, other values become zeroes. But I don't see reason for this,
                because in MS it is could be > 1 and < 0 and such line styles drawn differently. Moreover Acad
                draws fraction exctremely imprecise with any values and <0 and >1 values doesn't produce any
                errors except strange visibility, so keep fraction value without corrections for us.              */
    pComponent->setAutoPhase(pFrom->getPhase());
    pComponent->setPhaseMode(OdDbLSStrokePatternComponent::kLSPhaseFraction);
  }
  else if (pFrom->getCenterStretchPhaseModeFlag())
  {
    pComponent->setPhaseMode(OdDbLSStrokePatternComponent::kLSPhaseCentered);
  }
  else
  {
    pComponent->setPhase(pFrom->getPhase());
    pComponent->setPhaseMode(OdDbLSStrokePatternComponent::kLSPhaseDistance);
  }
  pComponent->setHasIterationLimit(pFrom->getUseIterationLimitFlag());
  pComponent->setIterationLimit((OdInt32)pFrom->getMaxIterations());
  pComponent->setIsSingleSegment(pFrom->getSingleSegmentModeFlag());
  OdUInt32 nStrokes = pFrom->getStrokeCount();
  OdDgLineCodeResourceStrokeData strokeIn;
  for (OdUInt32 nStroke = 0; nStroke < nStrokes; nStroke++)
  {
    pFrom->getStroke(nStroke, strokeIn);
    OdDbLSStroke strokeOut;
    strokeOut.setIsDash(strokeIn.getDashFlag());
    strokeOut.setBypassCorner(strokeIn.getByPassCornerFlag());
    strokeOut.setCanBeScaled(strokeIn.getCanBeScaledFlag());
    if (strokeOut.isDash())
    { // always false for gaps
      strokeOut.setInvertAtOrigin(strokeIn.getInvertStrokeInFirstCodeFlag());
      strokeOut.setInvertAtEnd(strokeIn.getInvertStrokeInLastCodeFlag());
    }
    strokeOut.setLength(strokeIn.getLength() * m_fUORPerStorage);
    strokeOut.setStartWidth(strokeIn.getStartWidth() * m_fUORPerStorage);
    if (((strokeIn.getStartWidth() > strokeIn.getEndWidth()) && !strokeIn.getDecreasingTaperFlag()) ||
        ((strokeIn.getStartWidth() < strokeIn.getEndWidth()) && !strokeIn.getIncreasingTaperFlag()))
      strokeOut.setEndWidth(strokeOut.startWidth());
    else
      strokeOut.setEndWidth(strokeIn.getEndWidth() * m_fUORPerStorage);
    strokeOut.setWidthMode((OdDbLSStroke::WidthMode)strokeIn.getWidthMode());
    strokeOut.setCapMode((OdDbLSStroke::CapMode)strokeIn.getCapsType());
    pComponent->insertStroke(&strokeOut);
  }
  m_componentsMap[pRes] = ::oddbDgnLSGetComponentsDictionary(m_pDstDb, OdDb::kForWrite, true)->setAt(
    generateDictKey(pRes, handleId), pComponent);
  return pComponent;
}

OdDbLSComponentPtr DgnLSImporter::processDgnLS_Compound(const OdDgDatabase* pDgnDb, 
                                                          OdDgLineStyleResource *pRes, 
                                                            OdUInt64 handleId, 
                                                              bool bFromRscFile)
{
  if (!pRes)
    return OdDbLSComponentPtr();
  CompMap::iterator it = m_componentsMap.find(pRes);
  if (it != m_componentsMap.end())
    return OdDbLSComponent::cast(it->second.openObject());
  OdDbLSCompoundComponentPtr pComponent = OdDbLSCompoundComponent::createObject();
  // Should be pre-registered to avoid self-reference problems
  m_componentsMap[pRes] = ::oddbDgnLSGetComponentsDictionary(m_pDstDb, OdDb::kForWrite, true)->setAt(
    generateDictKey(pRes, handleId), pComponent);
  OdDgCompoundLineStyleResource *pFrom = static_cast<OdDgCompoundLineStyleResource*>(pRes);
  // Component
  pComponent->setComponentType(kLSCompoundComponent);
  pComponent->setDescription(pFrom->getDescription());
  pComponent->setComponentUID(m_curUID); getUIDIncrement();
  // Compound
  OdUInt32 nComps = pFrom->getComponentCount();
  OdDgCompoundLineStyleComponentInfo compIn;
  for (OdUInt32 nComp = 0; nComp < nComps; nComp++)
  {
    pFrom->getComponent(nComp, compIn);
    OdDgLineStyleDefTableRecordPtr pLSDef;
    if (compIn.getComponentHandleId() != 0)
    {
      OdDgElementId lsDefId = m_pSrcDb->getElementId(OdDbHandle(compIn.getComponentHandleId()));
      pLSDef = OdDgLineStyleDefTableRecord::cast(lsDefId.openObject(OdDg::kForRead));
    }
    else if( bFromRscFile )
    {
      OdDgLineStyleDefTablePtr pLineStyleDef = m_pSrcDb->getLineStyleDefTable();
      pLSDef = pLineStyleDef->getRscLineStyleDef(compIn.getComponentEntryId(), compIn.getComponentType());
    }
    else
    {
      pLSDef = findLineStyleDefIntoDatabase( pDgnDb, compIn.getComponentEntryId(), compIn.getComponentType() );
    }

    OdDbLSComponentPtr pComp;
    if (compIn.getComponentType() == OdDgLineStyleResource::kLsInternalRes)
    {
      OdUInt32 nEntry = compIn.getComponentEntryId();
      //ODA_ASSERT((nEntry >= 0x80000000) && (nEntry <= 0x80000007));
      if (nEntry < 0x80000000 || nEntry > 0x80000007)
        nEntry = 0x80000000;
      pComp = processDgnLS_Internal(nEntry - 0x80000000, false);
    }
    else if( !pLSDef.isNull() )
    {
      pComp = processDgnLS_Component( pDgnDb, pLSDef->getResource(), pLSDef->elementId().getHandle(), bFromRscFile);
    }

    if (pComp.isNull())
      continue;
    pComponent->appendComponent(pComp->id(), compIn.getComponentOffset() * m_fUORPerStorage);
  }
  return pComponent;
}

OdDbLSComponentPtr DgnLSImporter::processDgnLS_Point( const OdDgDatabase* pDgnDb,
                                                        OdDgLineStyleResource *pRes, 
                                                          OdUInt64 handleId,
                                                            bool bFromRscFile
                                                     )
{
  if (!pRes)
    return OdDbLSComponentPtr();
  CompMap::iterator it = m_componentsMap.find(pRes);
  if (it != m_componentsMap.end())
    return OdDbLSComponent::cast(it->second.openObject());
  OdDbLSPointComponentPtr pComponent = OdDbLSPointComponent::createObject();
  OdDgLinePointResource *pFrom = static_cast<OdDgLinePointResource*>(pRes);
  // Component
  pComponent->setComponentType(kLSPointComponent);
  pComponent->setDescription(pFrom->getDescription());
  pComponent->setComponentUID(m_curUID); getUIDIncrement();
  // Point
  { // base pattern
    OdDgLineStyleDefTableRecordPtr pLSDef;
    if (pFrom->getBasePatternHandleId() != 0)
    {
      OdDgElementId lsDefId = m_pSrcDb->getElementId(OdDbHandle(pFrom->getBasePatternHandleId()));
      pLSDef = OdDgLineStyleDefTableRecord::cast(lsDefId.openObject(OdDg::kForRead));
    }
    else if( bFromRscFile )
    {
      OdDgLineStyleDefTablePtr pLineStyleDef = m_pSrcDb->getLineStyleDefTable();
      pLSDef = pLineStyleDef->getRscLineStyleDef(pFrom->getBasePatternEntryId(), (OdDgLineStyleResource::OdLsResourceType)pFrom->getBasePatternType());
    }
    else
    {
      pLSDef = findLineStyleDefIntoDatabase( pDgnDb, pFrom->getBasePatternEntryId(), (OdDgLineStyleResource::OdLsResourceType)pFrom->getBasePatternType() );
    }

    OdDbLSComponentPtr pComp;
    if (pLSDef.isNull() || pLSDef->getType() != OdDg::kLsTypeLineCode)
      pComp = processDgnLS_Internal(0, false);
    else
      pComp = processDgnLS_Stroke(pLSDef->getResource(), pLSDef->elementId().getHandle());
    ODA_ASSERT(!pComp.isNull());
    if (pComp.isNull())
      return pComp;
    pComponent->setStrokeComponent(pComp->id());
  }
  { // symbols
    OdUInt32 nSyms = pFrom->getSymbolCount();
    OdDgLinePointResourceSymInfo symIn;
    for (OdUInt32 nSym = 0; nSym < nSyms; nSym++)
    {
      pFrom->getSymbol(nSym, symIn);
      // Check symbol first, elsewhere we couldn't process at all
      OdDgLineStyleDefTableRecordPtr pLSDef;
      if (symIn.getPointSymbolHandleId() != 0)
      {
        OdDgElementId lsDefId = m_pSrcDb->getElementId(OdDbHandle(symIn.getPointSymbolHandleId()));
        pLSDef = OdDgLineStyleDefTableRecord::cast(lsDefId.openObject(OdDg::kForRead));
      }
      else if( bFromRscFile )
      {
        OdDgLineStyleDefTablePtr pLineStyleDef = m_pSrcDb->getLineStyleDefTable();
        pLSDef = pLineStyleDef->getRscLineStyleDef(symIn.getPointSymbolEntryId(), symIn.getSymbolType());
      }
      else
      {
        pLSDef = findLineStyleDefIntoDatabase( pDgnDb, symIn.getPointSymbolEntryId(), symIn.getSymbolType() );
      }

      if (pLSDef.isNull() || pLSDef->getType() != OdDg::kLsTypePointSymbol)
        continue;
      OdDbLSComponentPtr pComp = processDgnLS_Symbol(pLSDef->getResource(), pLSDef->elementId().getHandle(), symIn);
      if (pComp.isNull())
        continue;
      // Scale factor
      double fScaleFactor = m_fUORPerStorage; // #12104
      //
      OdDbLSSymbolReference::VertexMask vMask = OdDbLSSymbolReference::kLSAtStroke;
      if (symIn.getSymbolAtElementOriginFlag())
        vMask = OdDbLSSymbolReference::kLSAtOrigin;
      else if (symIn.getSymbolAtEachVertexFlag())
        vMask = OdDbLSSymbolReference::kLSAtVertex;
      else if (symIn.getSymbolAtElementEndFlag())
        vMask = OdDbLSSymbolReference::kLSAtEnd;
      OdDbLSSymbolReference *pRef;
      if (vMask == OdDbLSSymbolReference::kLSAtStroke)
        pRef = pComponent->insertSymbolForStroke(pComp->id(), (OdInt32)symIn.getSymbolStrokeNo());
      else
        pRef = pComponent->insertSymbolForVertex(pComp->id(), vMask);
      pRef->setPartialStrokes(!symIn.getNoPartialStrokesFlag());
      pRef->setClipPartial(!symIn.getDoNotClipElementFlag());
      pRef->setAllowStretch(!symIn.getDoNotScaleElementFlag());
      pRef->setPartialProjected(symIn.getPartialOriginBeyondEndFlag());
      pRef->setUseSymbolColor(symIn.getUseSymbolColorFlag());
      pRef->setUseSymbolLineweight(symIn.getUseSymbolWeightFlag());
      pRef->setJustify((OdDbLSSymbolReference::Justify)symIn.getSymbolPosOnStroke());
      if (symIn.getAbsoluteRotationAngleFlag())
        pRef->setRotationType(OdDbLSSymbolReference::kLSAbsolute);
      else if (symIn.getMirrorSymbolForReversedLinesFlag())
        pRef->setRotationType(OdDbLSSymbolReference::kLSAdjusted);
      else
        pRef->setRotationType(OdDbLSSymbolReference::kLSRelative);
      pRef->setXOffset(symIn.getOffset().x * fScaleFactor);
      pRef->setYOffset(symIn.getOffset().y * fScaleFactor);
      pRef->setAngle(symIn.getRotationAngle());
    }
  }
  m_componentsMap[pRes] = ::oddbDgnLSGetComponentsDictionary(m_pDstDb, OdDb::kForWrite, true)->setAt(
    generateDictKey(pRes, handleId), pComponent);
  return pComponent;
}

OdDbLSComponentPtr DgnLSImporter::processDgnLS_Symbol(OdDgLineStyleResource *pRes, OdUInt64 handleId, OdDgLinePointResourceSymInfo &symInfo,
                                                      double scaleX, double scaleY,
                                                      double offsetX, double offsetY)
{
  if (!pRes)
    return OdDbLSComponentPtr();
  CompMap::iterator it = m_componentsMap.find(pRes);
  if (it != m_componentsMap.end())
    return OdDbLSComponent::cast(it->second.openObject());
  OdDbLSSymbolComponentPtr pComponent = OdDbLSSymbolComponent::createObject();
  OdDgPointSymbolResource *pFrom = static_cast<OdDgPointSymbolResource*>(pRes);
  // Xform
  bool bXform = false;
  OdGeMatrix3d mtxXform;
  scaleX /= pFrom->getSymbolScale();
  scaleY /= pFrom->getSymbolScale();
  if (!OdEqual(scaleX, 1.0) || !OdEqual(scaleY, 1.0) || OdNonZero(offsetX) || OdNonZero(offsetY))
  {
    bXform = true;
    ODA_ASSERT(OdEqual(scaleX, scaleY));
    mtxXform.setToScaling(scaleY).preMultBy(OdGeMatrix3d::translation(OdGeVector3d(offsetX, offsetY, 0.0)));
  }
  // Component
  pComponent->setComponentType(kLSSymbolComponent);
  pComponent->setDescription(pFrom->getDescription());
  pComponent->setComponentUID(m_curUID); getUIDIncrement();
  // Symbol
  pComponent->setIs3d(pFrom->getSymbol3DFlag());
  // Typically 1000 in Acad. Reason is unknown.
  pComponent->setStoredUnitScale(pFrom->getSymbolScale());
  pComponent->setUnitScale(pFrom->getSymbolScale());
  { // fill block table record
    OdDbBlockTableRecordPtr pBlock = OdDbBlockTableRecord::createObject();
    pBlock->setName(/*OdString().format(OD_T("*A%u"), ++m_compCounters[kLSSymbolComponent])*/OD_T("*A"));
    //block->setOrigin(scd->getOrigin());
    OdDbObjectId blockId = OdDbBlockTable::cast(m_pDstDb->getBlockTableId().openObject(OdDb::kForWrite))->add(pBlock);
    OdDgElementId cellId = pFrom->getCell(m_pSrcDb, symInfo.getPointSymbolEntryId(), symInfo.getSymbolType() == OdDgLineCodeResource::kLsPointSymbolResV7);
    OdDgGraphicsElementPtr pCell = OdDgGraphicsElement::cast(cellId.openObject(OdDg::kForRead));
    ODA_ASSERT(!pCell.isNull());
    if (pCell->isKindOf(OdDgCellHeader3d::desc()))
    {
      OdDgCellHeader3dPtr pCellHdr = OdDgCellHeader3d::cast(pCell);
      OdDgElementIteratorPtr pIt = pCellHdr->createIterator();
      while (!pIt->done())
      {
        OdDgElementPtr pElem = pIt->item().safeOpenObject();
        ((TD_DGN_IMPORT::OdDgnImportPE*)pElem->queryX(TD_DGN_IMPORT::OdDgnImportPE::desc()))->importElement(pElem, pBlock);
        pIt->step();
      }
    }
    else if (pCell->isKindOf(OdDgCellHeader2d::desc()))
    {
      OdDgCellHeader2dPtr pCellHdr = OdDgCellHeader2d::cast(pCell);
      OdDgElementIteratorPtr pIt = pCellHdr->createIterator();
      while (!pIt->done())
      {
        OdDgElementPtr pElem = pIt->item().safeOpenObject();
        ((TD_DGN_IMPORT::OdDgnImportPE*)pElem->queryX(TD_DGN_IMPORT::OdDgnImportPE::desc()))->importElement(pElem, pBlock);
        pIt->step();
      }
    }
    else
    {
      ODA_FAIL();
    }
    pComponent->setBlockTableRecord(blockId);
    if (bXform)
      transformBy(pBlock, mtxXform);
    // Unfortunately Acad doesn't support colored symbols correctly. Result of lintyping is unpredictable in this case.
    // 2013 importer resets all symbol elements color to ByBlock, so make it similarly.
    clearColor(pBlock);
  }
  m_componentsMap[pRes] = ::oddbDgnLSGetComponentsDictionary(m_pDstDb, OdDb::kForWrite, true)->setAt(
    generateDictKey(pRes, handleId, &symInfo), pComponent);
  return pComponent;
}

const OdUInt8 *DgnLSImporter::getUIDIncrement()
{
  ::oddbDgnLSIncrementUID(m_curUID);
  return m_curUID;
}

void DgnLSImporter::transformBy(OdDbBlockTableRecord *pBTR, const OdGeMatrix3d &xfm) const
{
  OdDbObjectIteratorPtr pIt = pBTR->newIterator();
  while (!pIt->done())
  {
    OdDbEntityPtr pEntity = pIt->entity(OdDb::kForWrite);
    pEntity->transformBy(xfm);
    pIt->step();
  }
}

void DgnLSImporter::clearColor(OdDbBlockTableRecord *pBTR) const
{
  OdDbObjectIteratorPtr pIt = pBTR->newIterator();
  while (!pIt->done())
  {
    OdDbEntityPtr pEntity = pIt->entity(OdDb::kForWrite);
    pEntity->setColorIndex(OdCmEntityColor::kACIbyBlock);
    pIt->step();
  }
}

static OdString extractFileName(const OdString &fullPath)
{
  const int nSlh[2] = { fullPath.reverseFind('\\'), fullPath.reverseFind('/') };
  int nDivider = odmax(nSlh[0], nSlh[1]);
  if (nDivider >= 0)
    return fullPath.right(fullPath.getLength() - (nDivider + 1));
  return fullPath;
}

OdString DgnLSImporter::generateDictKey(OdDgLineStyleResource *pRes, OdUInt64 handleId, OdDgLinePointResourceSymInfo *symInfo)
{
  OdDbLSComponentType compType;
  if (symInfo)
    compType = kLSSymbolComponent;
  else
  {
    switch (pRes->getType())
    {
      case OdDg::kLsTypeCompound: compType = kLSCompoundComponent; break;
      case OdDg::kLsTypeLineCode: compType = kLSStrokePatternComponent; break;
      case OdDg::kLsTypeLinePoint: compType = kLSPointComponent; break;
      default: return OdString::kEmpty; // to prevent warning only
    }
  }
  const bool bIsElement = m_pLS->getRefersToElementFlag();
  // Naming has following structure:
  // <fname>-<type>-<num>
  // Where:
  // <fname> : name of rsc file if element loaded from rsc; name of dgn file if element embedded in dgn.
  // <type> : symbolic name of resource type.
  // <num> : internal counter if element loaded from rsc; id if element embedded in dgn.
  OdString fnameComponent; // <fname> component
  const OdChar *pTypeComponent = NULL; // <type> component
  OdUInt32 numComponent = 0; // <num> component
  if (bIsElement)
  {
    fnameComponent = extractFileName(m_pSrcDb->getFilename());
    numComponent = (OdUInt32)handleId;
  }
  else
  {
    fnameComponent = extractFileName(m_pLS->getRscFileName());
    numComponent = ++m_compCounters[compType];
  }
  switch (compType)
  {
    case kLSSymbolComponent:
      if (symInfo->getSymbolType() == OdDgLineCodeResource::kLsPointSymbolResV7)
        pTypeComponent = OD_T("SymbolV7");
      else
        pTypeComponent = OD_T("Symbol");
    break;
    case kLSCompoundComponent: pTypeComponent = OD_T("Compound"); break;
    case kLSStrokePatternComponent: pTypeComponent = OD_T("StrokePattern"); break;
    case kLSPointComponent: pTypeComponent = OD_T("Point"); break;
    default: return OdString::kEmpty; // to prevent warning only
  }
  OdString valName, repairedName;
  if (fnameComponent.isEmpty())
    valName = OdString().format(OD_T("%ls-%u"), pTypeComponent, (unsigned)numComponent);
  else
    valName = OdString().format(OD_T("%ls-%ls-%u"), fnameComponent.c_str(), pTypeComponent, (unsigned)numComponent);
  if (OdDbSymUtil::repairSymbolName(repairedName, valName, m_pDstDb, false, L' ', false) == eOk
      && !repairedName.isEmpty())
    valName = repairedName;
  return valName;
}

void DgnLSImporter::prepareLS(OdDgDatabase* pDb)
{
  OdDgLineStyleTablePtr pLST = pDb->getLineStyleTable();

  for (OdDgElementIteratorPtr i = pLST->createIterator(); !i->done();  i->step())
  {
    OdDgLineStyleTableRecordPtr pLS = i->item().safeOpenObject();
    OdDgLineStyleDefTableRecordPtr pLsDef;
    if (pLS->getRefersToElementFlag())
    {
      OdDgElementId lsDefId = pLS->getRefersToId();
      pLsDef = OdDgLineStyleDefTableRecord::cast(lsDefId.openObject());
    }
    else
    {
      OdDgLineStyleDefTablePtr pLineStyleDef = pDb->getLineStyleDefTable();
      pLsDef = pLineStyleDef->getRscLineStyleDef(pLS->getName());
      if (pLsDef.isNull() && pLS->getEntryId() < 8 && pLS->getEntryId() >= 0)
        continue;
    }

    if (pLsDef.isNull() || pLsDef->getResource().isNull())
      continue;
    switch (pLsDef->getResource()->getType())
    {
      case OdDg::kLsTypeCompound:
        {
          OdVector<OdDgLineStyleResource*> nesting;
          prepareLS_Compound(pDb, pLsDef->getResource(), nesting, !pLS->getRefersToElementFlag() );
        }
      break;
      case OdDg::kLsTypeLinePoint:
        prepareLS_Point(pDb, pLsDef->getResource(), !pLS->getRefersToElementFlag());
      break;
      default: break;
    }
  }
}

void DgnLSImporter::prepareLS_Compound( OdDgDatabase* pDb, 
                                          OdDgLineStyleResource *pRes, 
                                            OdVector<OdDgLineStyleResource*> &pNesting,
                                              bool bStyleFromRscFile
                                      )
{
  if (pNesting.contains(pRes) || !pRes || !pRes->isKindOf(OdDgCompoundLineStyleResource::desc()) )
    return;
  OdDgCompoundLineStyleResource *pFrom = static_cast<OdDgCompoundLineStyleResource*>(pRes);
  OdUInt32 nComps = pFrom->getComponentCount();
  OdDgCompoundLineStyleComponentInfo compIn;

  for (OdUInt32 nComp = 0; nComp < nComps; nComp++)
  {
    pFrom->getComponent(nComp, compIn);
    OdDgLineStyleDefTableRecordPtr pLSDef;
    if (compIn.getComponentHandleId() != 0)
    {
      OdDgElementId lsDefId = pDb->getElementId(OdDbHandle(compIn.getComponentHandleId()));
      pLSDef = OdDgLineStyleDefTableRecord::cast(lsDefId.openObject(OdDg::kForRead));
    }
    else if( bStyleFromRscFile )
    {
      OdDgLineStyleDefTablePtr pLineStyleDef = pDb->getLineStyleDefTable();
      pLSDef = pLineStyleDef->getRscLineStyleDef(compIn.getComponentEntryId(), compIn.getComponentType());
    }
    else
    {
      pLSDef = findLineStyleDefIntoDatabase( pDb, compIn.getComponentEntryId(), compIn.getComponentType() );
    }

    if (pLSDef.isNull())
      continue;

    switch (compIn.getComponentType())
    {
      case OdDgLineStyleResource::kLsCompoundRes:
        {
          pNesting.push_back(pRes);
          prepareLS_Compound(pDb, pLSDef->getResource(), pNesting, bStyleFromRscFile);
        }
      break;
      case OdDgLineStyleResource::kLsLinePointRes:
        prepareLS_Point(pDb, pLSDef->getResource(), bStyleFromRscFile);
      break;
      default: break;
    }
  }
}

void DgnLSImporter::prepareLS_Point(OdDgDatabase* pDb, OdDgLineStyleResource *pRes, bool bStyleFromRscFile)
{
  if (!pRes || !pRes->isKindOf(OdDgLinePointResource::desc()))
    return;

  OdDgLinePointResource *pFrom = static_cast<OdDgLinePointResource*>(pRes);
  OdUInt32 nSyms = pFrom->getSymbolCount();
  OdDgLinePointResourceSymInfo symIn;
  for (OdUInt32 nSym = 0; nSym < nSyms; nSym++)
  {
    pFrom->getSymbol(nSym, symIn);
    OdDgLineStyleDefTableRecordPtr pLSDef;
    if (symIn.getPointSymbolHandleId() != 0)
    {
      OdDgElementId lsDefId = pDb->getElementId(OdDbHandle(symIn.getPointSymbolHandleId()));
      pLSDef = OdDgLineStyleDefTableRecord::cast(lsDefId.openObject(OdDg::kForRead));
    }
    else if( bStyleFromRscFile )
    {
      OdDgLineStyleDefTablePtr pLineStyleDef = pDb->getLineStyleDefTable();
      pLSDef = pLineStyleDef->getRscLineStyleDef(symIn.getPointSymbolEntryId(), symIn.getSymbolType());
    }
    else
    {
      pLSDef = findLineStyleDefIntoDatabase( pDb, symIn.getPointSymbolEntryId(), symIn.getSymbolType() );
    }

    if (pLSDef.isNull() || pLSDef->getType() != OdDg::kLsTypePointSymbol)
      continue;

    prepareLS_Symbol(pDb, pLSDef->getResource(), symIn);
  }
}

void DgnLSImporter::prepareLS_Symbol(OdDgDatabase* pDb, OdDgLineStyleResource *pRes, OdDgLinePointResourceSymInfo &symInfo)
{
  if (!pRes)
    return;
  OdDgPointSymbolResource *pFrom = static_cast<OdDgPointSymbolResource*>(pRes);
  // This is only what we really wants to do:
  pFrom->getCell(pDb, symInfo.getPointSymbolEntryId(), symInfo.getSymbolType() == OdDgLineCodeResource::kLsPointSymbolResV7);
}

//==============================================================================================================================
//          Implementation of OdDgnImportLineStyleImpl
//==============================================================================================================================

namespace TD_DGN_IMPORT 
{

OdDbObjectId OdDgnImportLineStyleImpl::importLineStyle( OdDgDatabase* pDgnDb, OdDbDatabase* pDwgDb, const OdString& strLineStyleName )
{
  OdDbObjectId retVal;

  if( !pDgnDb || !pDwgDb )
  {
    return retVal;
  }

  OdDbHostAppServices* pDbHostApp = pDwgDb->appServices();

  if( !pDbHostApp )
  {
    return retVal;
  }

  OdDgLineStyleTableRecordPtr pDgLineStyleTableRecord;

  OdDgLineStyleTablePtr pDgnLineStyleTable = pDgnDb->getLineStyleTable(OdDg::kForRead);

  if( !pDgnLineStyleTable.isNull() )
  {
    OdDgElementId idLineStyle;

    if( (strLineStyleName.getLength() == 4) && (strLineStyleName.left(3) == L"DGN") )
    {
      if( strLineStyleName[3] == 0 )
      {
        retVal = pDwgDb->getLinetypeContinuousId();
        return retVal;
      }
      else
      {
        OdUInt32 uInternalIndex = strLineStyleName[3] - L'1' + 1;
        idLineStyle = pDgnLineStyleTable->getAt( uInternalIndex );
      }
    }
    else
    {
      idLineStyle = pDgnLineStyleTable->getAt( strLineStyleName );
    }

    if( !idLineStyle.isNull() )
    {
      pDgLineStyleTableRecord = idLineStyle.openObject(OdDg::kForRead);
    }
  }

  bool bRemoveImportPE = false;

  if( OdDgElement::desc()->getX(OdDgnImportPE::desc()).isNull() )
  {
    bRemoveImportPE = true;
    DgnImporter::setImportPEToElements( false );
  }

  OdDbDatabasePtr pEmptyDb = pDbHostApp->createDatabase();

  DgnLSImporter lineStyleImporter( pDgnDb, pEmptyDb );

  OdDbLinetypeTableRecordPtr pDbLineType;

  try
  {
    pDbLineType = lineStyleImporter.copyLinetype(pDgLineStyleTableRecord.get());
  }
  catch(...)
  {	
    pDbLineType = 0;
  }

  if( !pDbLineType.isNull() )
  {
    OdDbObjectIdArray arrLineTypeIds;
    arrLineTypeIds.push_back( pDbLineType->objectId() );

    OdDbIdMappingPtr cloneMap = OdDbIdMapping::createObject(OdDb::kDcWblock);
    cloneMap->setDestDb( pDwgDb );
    pEmptyDb->wblockCloneObjects(arrLineTypeIds, pDwgDb->getLinetypeTableId(), *cloneMap, OdDb::kDrcIgnore );

    OdDbIdPair lineTypePair(pDbLineType->objectId());
    cloneMap->compute(lineTypePair);

    if( lineTypePair.isCloned() )
    {
      retVal = lineTypePair.value();
    }
  }

  if( bRemoveImportPE )
  {
    DgnImporter::removeImportPEFromElements();
  }

  return retVal;
}

};

//
