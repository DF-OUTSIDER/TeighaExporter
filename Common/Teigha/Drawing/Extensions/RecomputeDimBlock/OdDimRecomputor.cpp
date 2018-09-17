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


#include "OdaCommon.h"
#include "OdDimRecomputor.h"
#include "OdRound.h"

#include "Ge/GeMatrix3d.h"
#include "Ge/GeExtents3d.h"

#include "DbDimStyleTableRecord.h"
#include "DbLine.h"
#include "DbSolid.h"
#include "DbBlockReference.h"
#include "DbTextStyleTableRecord.h"
#include "DbBlockTableRecord.h"
#include "DbBlockTable.h"
#include "DbPoint.h"
#include "OdDToStr.h"
#include "OdRecomputorEngine.h"
#include "DbArc.h"
#include "Ge/GeCircArc2d.h"
#include "DbObjectContextData.h"
#include "DbXrecord.h"
#include "DbDictionary.h"
#include "DbBreakData.h"
#include "OdBreakDimEngine.h"
#include "DbField.h"

#define STL_USING_ALGORITHM
#include "OdaSTL.h"

extern bool oddbIsEnableDimensionBreak();

void OdDbDimRecomputePEImpl::recomputeDimBlock(OdDbDimension* pDim, OdDbDimensionObjectContextData* ctx)
{
  OdDbDimStyleTableRecord dimVars, *pDimVars = &dimVars;
  pDim->getDimstyleData(pDimVars);
  if (ctx)
  {
    if (ctx->hasOverride(OdDbDimensionObjectContextData::eDimtofl))
      dimVars.setDimtofl(ctx->dimtofl());
    if (ctx->hasOverride(OdDbDimensionObjectContextData::eDimsoxd))
      dimVars.setDimsoxd(ctx->dimsoxd());
    if (ctx->hasOverride(OdDbDimensionObjectContextData::eDimatfit))
      dimVars.setDimatfit(ctx->dimatfit());
    if (ctx->hasOverride(OdDbDimensionObjectContextData::eDimtix))
      dimVars.setDimtix(ctx->dimtix());
    if (ctx->hasOverride(OdDbDimensionObjectContextData::eDimtmove))
      dimVars.setDimtmove(ctx->dimtmove());
    double scale; ctx->getScale(scale);
    dimVars.setDimscale(1/scale);
  }
  OdArray<OdDbEntityPtr> ents;
  OdArray<OdDbEntityPtr>::iterator pSrcEnt;

  bool bUseDimLineIndex;
  try 
  {
    pDim->upgradeOpen();
    OdDimRecomputorPtr pDimRc = createRecomputor();
    bUseDimLineIndex = pDimRc->setUseDimLineIndex(pDim);
    if ( OdZero(pDimVars->dimscale()) )
      pDimVars->setDimscale(1.0); // Dimension subClose() method corrects it but force recompute may be wrong
    pDimRc->getDimParams(pDim, ctx);
    pDimRc->m_dMeasurement = -1.;
    pDimRc->preprocessDimPoints(pDimVars);
    pDimRc->updateMeasurement(pDimVars);
    pDimRc->buildDimension(ents, pDimVars);
    pDimRc->setDimParams(pDim, ctx);
    setMeasurementValue(pDim, pDimRc->m_dMeasurement);
    if ( pDimVars->dimtxtdirection() )
    {
      pSrcEnt = ents.begin();
      for(; pSrcEnt != ents.end(); ++ pSrcEnt)
      {
        if ( (*pSrcEnt)->isKindOf(OdDbMText::desc()) )
        {
          OdDbMTextPtr mTextPtr = (*pSrcEnt);
          OdGeVector3d v = mTextPtr->direction();
          v *= -1.0;
          mTextPtr->setDirection(v);
        }
      }
    }
  } 
  catch(const OdError&)
  {
    return;
  }

  // Support BREAK DATA
  if ( oddbIsEnableDimensionBreak() )
  {
    OdDbDictionaryPtr pExtDic = OdDbDictionary::cast(pDim->extensionDictionary().openObject(OdDb::kForRead)); 
    if ( pExtDic.get())
    {
      OdDbBreakDataPtr brData;
      brData = OdDbBreakData::cast(pExtDic->getAt(OD_T("ACAD_BREAKDATA"), OdDb::kForRead));
      if ( !brData.isNull() )
      {
        OdDbDimStyleTableRecordPtr dimStyle = pDim->dimensionStyle().openObject(OdDb::kForRead);
        double dBreakSize = dimStyle->getDIMBREAK()*pDimVars->dimscale();
        OdBreakDimEngine breakDimEngine(dBreakSize);
        breakDimEngine.setDimensionEnts(ents, pDim->dimBlockTransform());
    
        breakDimEngine.breakDimension(brData);
        ents = breakDimEngine.m_newDimEnts;

        if ( bUseDimLineIndex )
        {
          pSrcEnt = ents.begin();
          for(; pSrcEnt != ents.end(); ++ pSrcEnt)
          {
            if((*pSrcEnt)->isKindOf(OdDbPoint::desc())) // End of array
            {
              break;
            }
            removeDimLineIndex(*pSrcEnt);
          }
        }
      }
    }
  }

  OdDbBlockTableRecordPtr pBTR;
  if (ctx)
  {
    if (pDim->isDBRO())
    {
      if (ctx->blockIndex().isValid() && pDim->isSingleDimBlockReference())
      {
        pBTR = ctx->blockIndex().openObject(OdDb::kForWrite);
      }

      if (pBTR.isNull())
      {
        pBTR = pDim->dimBlock(OdDb::kForWrite);
        ctx->setBlockIndex(pBTR->objectId());
      }
    }
    else
    {
      pBTR = ctx->getNDBRDimBlock();
      if (pBTR.isNull())
      {
        pBTR = OdDbBlockTableRecord::createObject();
        pBTR->setName(L"*D");
        ctx->setNDBRDimBlock(pBTR);
      }
    }
  }
  else
    pBTR = pDim->dimBlock(OdDb::kForWrite);

  pSrcEnt = ents.begin();

  // need to set up layer
  // for all not point entities
  // if this dimension has non-default layer
  OdDbObjectId layId = pDim->layerId();
  if(!layId.isNull())
  {
    for(; pSrcEnt != ents.end(); ++ pSrcEnt)
    {
      if(!(*pSrcEnt)->isKindOf(OdDbPoint::desc()))
      {
        (*pSrcEnt)->setLayer(layId, true, true);
      }
    }
  }
  pSrcEnt = ents.begin();
  
  OdDbObjectIteratorPtr dstEntIter;
  bool bDstIterWasDone;
  bool bFld = pDim->hasFields();
  OdDbObjectId mTextId;
  if(pBTR->isDBRO())
  {
    dstEntIter = pBTR->newIterator();
    for(; (pSrcEnt != ents.end()) && (!dstEntIter->done()); ++ pSrcEnt, dstEntIter->step())
    {
      OdDbEntityPtr pDstEnt = dstEntIter->entity(OdDb::kForWrite);
      pDstEnt->handOverTo(pSrcEnt->get(), false, false);
      if ( bFld && mTextId.isNull() && (*pSrcEnt)->isKindOf(OdDbMText::desc()) )
      {
        mTextId = (*pSrcEnt)->objectId();
      }
    }
    bDstIterWasDone = dstEntIter->done(); // need to save this value, because
                                          // it can be changed due to appendOdDbEntity calls
  }
  else
  {
    bDstIterWasDone = true;
  }
  if((pSrcEnt != ents.end()) && bDstIterWasDone)
  {
    for(; (pSrcEnt != ents.end()); ++pSrcEnt)
    {
      pBTR->appendOdDbEntity(*pSrcEnt);
      if ( bFld && mTextId.isNull() && (*pSrcEnt)->isKindOf(OdDbMText::desc()) )
      {
        mTextId = (*pSrcEnt)->objectId();
      }
    }
  }

  if ( !mTextId.isNull() )
  {
    try 
    {
      OdDbMTextPtr mTextPtr = mTextId.safeOpenObject(OdDb::kForWrite);

      // pDim->fieldToMText(mTextPtr);
      OdDbFieldPtr field = pDim->getField(TEXT_FIELD_NAME, OdDb::kForWrite);
      if ( field.isNull() )
        throw OdError(eInvalidInput);

      mTextPtr->removeField(TEXT_FIELD_NAME);
      OdDbFieldPtr newTextField = field->clone();
      mTextPtr->setField(TEXT_FIELD_NAME, newTextField);

      // Don't add childs yet => getFieldCode returns FldIdx codes.
      OdString fieldCode = newTextField->getFieldCode(OdDbField::kRawCode);
      OdString formattedFieldCode;
      formatMeasurement(pDim, formattedFieldCode, pDim->measurement(), fieldCode);
      newTextField->setFieldCode(formattedFieldCode);

      OdInt32 chlCnt = field->childCount();
      for ( OdInt32 i = 0; i < chlCnt; i++ )
      {
        OdDbFieldPtr childFld =  field->getChild(i, OdDb::kForWrite);
        OdDbFieldPtr newChildFld = childFld->clone();
        newTextField->setField(OD_T(""), newChildFld);
      }

    }
    catch(const OdError&)
    {
    }
  }
  OdArray<OdDbEntityPtr> ents2Del;
  if((pSrcEnt == ents.end()) && !bDstIterWasDone)
  {
    for(; (!dstEntIter->done()); dstEntIter->step())
    {
      ents2Del.push_back(dstEntIter->entity(OdDb::kForWrite));
    }
  }
  for(OdArray<OdDbEntityPtr>::iterator ppEnt = ents2Del.begin(); ppEnt != ents2Del.end(); ++ ppEnt)
  {
    (*ppEnt)->erase();
  }
  resetDimBlockInsertParams(pDim);
}

void OdDbDimRecomputePEImpl::recomputeDimMeasurement(OdDbDimension* pDim)
{
  OdDbDimStyleTableRecord dimVars, *pDimVars = &dimVars;
  pDim->getDimstyleData(pDimVars);
  OdDimRecomputorPtr pDimRc = createRecomputor();
  pDimRc->getDimParams(pDim, 0);
  pDimRc->m_dMeasurement = -1.;
  pDimRc->preprocessDimPoints(pDimVars);
  pDimRc->updateMeasurement(pDimVars);
  setMeasurementValue(pDim, pDimRc->m_dMeasurement);
  pDimRc->setDimParams(pDim, 0);
}

void OdDbDimRecomputePEImpl::formatMeasurement(OdDbDimension* pDim, OdString& res, double measurement, const OdString& dimText)
{
  OdDbDimStyleTableRecord dimVars, *pDimVars = &dimVars;
  pDim->getDimstyleData(pDimVars);
  OdDimRecomputorPtr pDimRc = createRecomputor();
  pDimRc->getDimParams(pDim, 0);
  pDimRc->m_dMeasurement = measurement;
  pDimRc->m_strDimText = dimText;
  pDimRc->formatMeasurement(res, pDimVars);
}


OdDimRecomputor::OdDimRecomputor()
: m_dMeasurement(0.0)
, m_bUserDefinedTextPosition(false)
, m_dUcsXAngle(0.0)
, m_dTextRot(0.0)
, m_pDb(0)
, m_dimtaln(false)
, m_BgrndTxtFlag(0)
, m_ExtLineFixLenEnable(false)
, m_ExtLineFixLen(0.0)
, m_bArrowFirstIsFlipped(false)
, m_bArrowSecondIsFlipped(false)
, m_bAnnotative(false)
, m_bUseDimLineIndex(false)
, m_dTextWidth(0.0)
, m_dTextHeight(0.0)
{
}

OdDimRecomputor::~OdDimRecomputor()
{
}

void OdDimRecomputor::getDimParams(const OdDbDimension* pDim, OdDbDimensionObjectContextData* ctx)
{
  m_linetypeId = pDim->linetypeId();
  m_dLinetypeScale = pDim->linetypeScale();
  m_nLineweight = (OdInt16)pDim->lineWeight();
  m_transparency = pDim->transparency();

  m_pDb = pDim->database();

  OdDbBlockTableRecordPtr blTable = OdDbBlockTableRecord::cast(pDim->ownerId().openObject());
  m_dimlfac = pDim->dimlfac();
  if ( !blTable.isNull() && blTable->isLayout() )
  {
    if ( m_pDb->getModelSpaceId() == pDim->ownerId() )
    {
      if ( m_dimlfac < 0.0 )
      {
        m_dimlfac = 1.0;
      }
    } else
    {
      m_dimlfac = ::fabs(m_dimlfac);
    }
  } else
  {
    m_dimlfac = ::fabs(m_dimlfac);
  }

  m_dimtaln = pDim->getDIMTALN();
  m_vNormal = pDim->normal();
  m_bUserDefinedTextPosition = ctx ? !ctx->defTextLocation() : !pDim->isUsingDefaultTextPosition();
  m_strDimText = pDim->dimensionText();
  
  if (ctx)
  {
    m_ocsDimTextMidPt = OdGePoint3d(ctx->textLocation().x, ctx->textLocation().y, pDim->elevation());
    m_bAnnotative = true;
  }
  else
  {
    m_ocsDimTextMidPt = pDim->textPosition();
    m_ocsDimTextMidPt.transformBy(OdGeMatrix3d::worldToPlane(m_vNormal));
  }
  m_dUcsXAngle = pDim->horizontalRotation();
  m_dTextRot = ctx ? ctx->textRotation() : pDim->textRotation();
  if ( OdZero(m_dTextRot, 1.0E-5) )
	  m_dTextRot = 0.0;

  m_LinespacingStyle = (OdInt32)pDim->textLineSpacingStyle();
  m_dLinespacingFactor = pDim->textLineSpacingFactor();
  m_AttachPt = (OdInt32)pDim->textAttachment();

  // ACAD 2005 - 2006 support
  // m_BgrndTxtFlag = pDim->getBgrndTxtFlag();
  m_BgrndTxtFlag = pDim->getBgrndTxtColor(m_BgrndTxtColor);
  m_ExtLineFixLenEnable = pDim->getExtLineFixLenEnable();

  //m_ExtLineFixLen = pDim->getExtLineFixLen()*pDim->dimscale();
  m_ExtLineFixLen = pDim->getExtLineFixLen();
  double scale; 
  if(ctx && ctx->getScale(scale) == eOk && !OdZero(scale))
    m_ExtLineFixLen /= scale;
  else
    m_ExtLineFixLen *= pDim->dimscale();


  m_DimLinetypeId = pDim->getDimLinetype();
  if ( m_DimLinetypeId.isNull() )
    m_DimLinetypeId = m_linetypeId;

  m_DimExt1LinetypeId = pDim->getDimExt1Linetype();
  if ( m_DimExt1LinetypeId.isNull() )
    m_DimExt1LinetypeId = m_linetypeId; // m_DimLinetypeId; #6578

  m_DimExt2LinetypeId = pDim->getDimExt2Linetype();
  if ( m_DimExt2LinetypeId.isNull() )
    m_DimExt2LinetypeId = m_linetypeId; // m_DimLinetypeId; #6578

  if (ctx)
  {
    m_bArrowFirstIsFlipped = ctx->getArrowFirstIsFlipped();
    m_bArrowSecondIsFlipped = ctx->getArrowSecondIsFlipped();
  }
  else
  {
    m_bArrowFirstIsFlipped = pDim->getArrowFirstIsFlipped();
    m_bArrowSecondIsFlipped = pDim->getArrowSecondIsFlipped();
  }

  m_bInspection = pDim->inspection();
  m_nInspectionFrame = pDim->inspectionFrame();
  m_sInspectionLabel = pDim->inspectionLabel();
  m_sInspectionRate = pDim->inspectionRate();

  pDim->textDefinedSize(m_dTextWidth, m_dTextHeight);
 
}

void OdDimRecomputor::setDimParams(OdDbDimension* pDim, OdDbDimensionObjectContextData* ctx) const
{
  if (ctx)
  {
    ctx->setTextLocation(OdGePoint2d(m_ocsDimTextMidPt.x, m_ocsDimTextMidPt.y));
  }
  if (!ctx || ctx->isDefaultContextData())
  {
    OdGePoint3d worldTextPoint(m_ocsDimTextMidPt);
    worldTextPoint.transformBy(OdGeMatrix3d::planeToWorld(m_vNormal));
    pDim->setTextPosition(worldTextPoint);
  }
}

#define DIMVAR(name) pDimVars->dim##name()
#define SCALED(name) pDimVars->dimscale() * pDimVars->dim##name()

#define DIMVAR_41(name) SCALED(name)
#define DIMVAR_42(name) SCALED(name)
#define DIMVAR_44(name) SCALED(name)
#define DIMVAR_46(name) SCALED(name)
#define DIMVAR_49(name) SCALED(name)
#define DIMVAR_140(name) SCALED(name)
#define DIMVAR_141(name) SCALED(name)
#define DIMVAR_142(name) SCALED(name)
#define DIMVAR_147(name) SCALED(name)

#define DIMVAR_3(name)  DIMVAR(name)
#define DIMVAR_4(name)  DIMVAR(name)
#define DIMVAR_8(name)  DIMVAR(name)
#define DIMVAR_9(name)  DIMVAR(name)
#define DIMVAR_40(name)  DIMVAR(name)
#define DIMVAR_43(name)  DIMVAR(name)
#define DIMVAR_45(name)  DIMVAR(name)
#define DIMVAR_47(name)  DIMVAR(name)
#define DIMVAR_48(name)  DIMVAR(name)
#define DIMVAR_50(name)  DIMVAR(name)
#define DIMVAR_69(name)  DIMVAR(name)
#define DIMVAR_70(name)  DIMVAR(name)
#define DIMVAR_71(name)  DIMVAR(name)
#define DIMVAR_72(name)  DIMVAR(name)
#define DIMVAR_73(name)  DIMVAR(name)
#define DIMVAR_74(name)  DIMVAR(name)
#define DIMVAR_75(name)  DIMVAR(name)
#define DIMVAR_76(name)  DIMVAR(name)
#define DIMVAR_77(name)  DIMVAR(name)
#define DIMVAR_78(name)  DIMVAR(name)
#define DIMVAR_79(name)  DIMVAR(name)
#define DIMVAR_90(name)  DIMVAR(name)

#define DIMVAR_143(name)  DIMVAR(name)
#define DIMVAR_144(name)  DIMVAR(name)
#define DIMVAR_145(name)  DIMVAR(name)
#define DIMVAR_146(name)  DIMVAR(name)
#define DIMVAR_148(name)  DIMVAR(name)
#define DIMVAR_149(name)  DIMVAR(name)
#define DIMVAR_150(name)  DIMVAR(name)
#define DIMVAR_170(name)  DIMVAR(name)
#define DIMVAR_171(name)  DIMVAR(name)
#define DIMVAR_172(name)  DIMVAR(name)
#define DIMVAR_174(name)  DIMVAR(name)
#define DIMVAR_173(name)  DIMVAR(name)
#define DIMVAR_175(name)  DIMVAR(name)
#define DIMVAR_176(name)  DIMVAR(name)
#define DIMVAR_177(name)  DIMVAR(name)
#define DIMVAR_178(name)  DIMVAR(name)
#define DIMVAR_179(name)  DIMVAR(name)
#define DIMVAR_271(name)  DIMVAR(name)
#define DIMVAR_272(name)  DIMVAR(name)
#define DIMVAR_273(name)  DIMVAR(name)
#define DIMVAR_274(name)  DIMVAR(name)
#define DIMVAR_275(name)  DIMVAR(name)
#define DIMVAR_276(name)  DIMVAR(name)
#define DIMVAR_277(name)  DIMVAR(name)
#define DIMVAR_278(name)  DIMVAR(name)
#define DIMVAR_279(name)  DIMVAR(name)
#define DIMVAR_280(name)  DIMVAR(name)
#define DIMVAR_281(name)  DIMVAR(name)
#define DIMVAR_282(name)  DIMVAR(name)
#define DIMVAR_283(name)  DIMVAR(name)
#define DIMVAR_284(name)  DIMVAR(name)
#define DIMVAR_285(name)  DIMVAR(name)
#define DIMVAR_286(name)  DIMVAR(name)
#define DIMVAR_288(name)  DIMVAR(name)
#define DIMVAR_289(name)  DIMVAR(name)
#define DIMVAR_290(name)  DIMVAR(name)
#define DIMVAR_294(name)  DIMVAR(name)
#define DIMVAR_295(name)  DIMVAR(name)
#define DIMVAR_296(name)  DIMVAR(name)
#define DIMVAR_297(name)  DIMVAR(name)
#define DIMVAR_298(name)  DIMVAR(name)

#define DIMVAR_340(name)  DIMVAR(name)
#define DIMVAR_342(name)  DIMVAR(name)
#define DIMVAR_343(name)  DIMVAR(name)
#define DIMVAR_344(name)  DIMVAR(name)
#define DIMVAR_345(name)  DIMVAR(name)
#define DIMVAR_346(name)  DIMVAR(name)
#define DIMVAR_347(name)  DIMVAR(name)
#define DIMVAR_341(name)  DIMVAR(name)
#define DIMVAR_371(name)  DIMVAR(name)
#define DIMVAR_372(name)  DIMVAR(name)

#define VAR_DEF(type, name, dxf, def, metric_def, reserve1, reserve2) \
type OdDimRecomputor::getDim##name(OdDbDimStyleTableRecord* pDimVars) \
{ \
  return DIMVAR_##dxf(name);\
}

#include "DimVarDefs.h"
#undef  DIMVAR
#undef  SCALED
#undef  VAR_DEF

void OdDimRecomputor::buildExtLine(const OdGePoint3d& start,
                                   const OdGePoint3d& end,
                                   OdArray<OdDbEntityPtr>& ents,
                                   OdDbDimStyleTableRecord* pDimVars,
                                   double textLength,
                                   const OdGeVector3d dir, 
                                   double oblique)
{
  // Shifting start to end(offset)
  OdGeVector3d end2Start = end - start;
  OdGePoint3d newStart(start);
  OdGePoint3d newEnd(end);
  double len = end2Start.length();
  double exe = getDimexe(pDimVars);
  double exo = getDimexo(pDimVars);
  if ( OdNonZero(oblique) )
  {
    if (oblique > OdaPI )
      oblique = OdaPI - oblique;
    exe /= ::fabs(::sin(::fabs(oblique)));
    exo /= ::fabs(::sin(::fabs(oblique)));
  }
  if (!OdZero(len))
  {
    end2Start /= len;
    if ( m_ExtLineFixLenEnable && m_ExtLineFixLen + getDimexo(pDimVars) < len )
    {
      newStart = end - (end2Start * m_ExtLineFixLen);
      newEnd += (end2Start * exe);
    } else
    {
      newStart += (end2Start * exo); 
      // Moving endpoint beyond dimension line
      newEnd += (end2Start * exe); 
    }
    if(OdNonZero(textLength))
    {
      newEnd += end2Start * (textLength + 2.0 * getDimgap(pDimVars));
    }
  } else
  {
    newStart += dir*exo; 
    newEnd += dir*exe;
  }
  OdDbLinePtr pLine = OdDbLine::createObject();
  pLine->setColor(getDimclre(pDimVars));
  pLine->setLineWeight(OdDb::LineWeight(getDimlwe(pDimVars)));
  pLine->setLinetypeScale(m_dLinetypeScale);
  pLine->setTransparency(m_transparency);
  pLine->setStartPoint(newStart);
  pLine->setEndPoint(newEnd);
  ents.push_back(pLine);
}

bool OdDimRecomputor::buildExtLine1(const OdGePoint3d& start,
                                    const OdGePoint3d& end,
                                    OdArray<OdDbEntityPtr>& ents,
                                    OdDbDimStyleTableRecord* pDimVars,
                                    double textLength,
                                    const OdGeVector3d dir)
{
  if(getDimse1(pDimVars))
  {
    return false;
  }
  if((getDimjust(pDimVars) == 3) && (getDimtad(pDimVars) == 1))
  {
    buildExtLine(start, end, ents, pDimVars, textLength, dir);
  }
  else
  {
    buildExtLine(start, end, ents, pDimVars, 0.0, dir);
  }
  ents.last()->setLinetype(m_DimExt1LinetypeId);
  return true;
}

bool OdDimRecomputor::buildExtLine2(const OdGePoint3d& start,
                                    const OdGePoint3d& end,
                                    OdArray<OdDbEntityPtr>& ents,
                                    OdDbDimStyleTableRecord* pDimVars,
                                    double textLength,
                                    const OdGeVector3d dir)
{
  if(getDimse2(pDimVars))
  {
    return false;
  }
  if((getDimjust(pDimVars) == 4) && (getDimtad(pDimVars) == 1))
  {
    buildExtLine(start, end, ents, pDimVars, textLength, dir);
  }
  else
  {
    buildExtLine(start, end, ents, pDimVars, 0.0, dir);
  }
  ents.last()->setLinetype(m_DimExt2LinetypeId);
  return true;
}

void OdDimRecomputor::buildArrow1(const OdGePoint3d& origin,
                                  const OdGeVector3d& oppDir,
                                  OdArray<OdDbEntityPtr>& ents,
                                  OdDbDimStyleTableRecord* pDimVars)
{
  if (!pDimVars->dimsd1())
  {
    buildArrow(origin, oppDir, ents, pDimVars, pDimVars->arrowId(OdDb::kFirstArrow));
  }
}

void OdDimRecomputor::buildArrow2(const OdGePoint3d& origin,
                                  const OdGeVector3d& oppDir,
                                  OdArray<OdDbEntityPtr>& ents,
                                  OdDbDimStyleTableRecord* pDimVars)
{
  if (!pDimVars->dimsd2())
  {
    buildArrow(origin, oppDir, ents, pDimVars, pDimVars->arrowId(OdDb::kSecondArrow));
  }
}

void OdDimRecomputor::buildArrow(const OdGePoint3d& origin,
                                 const OdGeVector3d& oppDir,
                                 OdArray<OdDbEntityPtr>& ents,
                                 OdDbDimStyleTableRecord* pDimVars,
                                 OdDbObjectId arrowId)
{
  if ( OdZero(getDimasz(pDimVars)) )
    return;

  if(arrowId.isNull())
  {
    // creating solid(default)
    OdGePoint3d firstPoint, secondPoint, thirdPoint;

    OdGeVector3d ortho = OdGeVector3d::kZAxis.crossProduct(oppDir).normalize();
    double arrowLength = getDimasz(pDimVars);
    secondPoint = origin + oppDir * arrowLength + ortho * arrowLength / 6.0;
    thirdPoint  = origin + oppDir * arrowLength - ortho * arrowLength / 6.0;
    firstPoint  = origin;

    OdDbSolidPtr pSolid;
    pSolid = OdDbSolid::createObject();
    pSolid->setColor(getDimclrd(pDimVars));
    pSolid->setTransparency(m_transparency);
    pSolid->setPointAt(0, secondPoint);
    pSolid->setPointAt(1, thirdPoint);
    pSolid->setPointAt(2, firstPoint);
    pSolid->setPointAt(3, firstPoint);

    ents.push_back(pSolid);
  }
  else
  {
    OdString name = OdDmUtil::arrowName(arrowId);
    name.makeUpper();
    if ( name == OD_T("NONE") )   // #14958 ACAD doesn't create block reference in case arrow is NONE
      return;
    // Creating block reference
    OdDbBlockReferencePtr pBlkRef = OdDbBlockReference::createObject();
    pBlkRef->setLineWeight(OdDb::LineWeight(getDimlwd(pDimVars)));
    pBlkRef->setColor(getDimclrd(pDimVars));
    pBlkRef->setBlockTableRecord(arrowId);
    OdGeScale3d scale(getDimasz(pDimVars));
    if(scale.isValid())
    {
      pBlkRef->setScaleFactors(scale);
    }
    OdGePoint3d position = origin; // in WCS
    pBlkRef->setPosition(position);
    OdGeVector3d direction = -oppDir;
    ODA_ASSERT(fabs(direction.z) < 1.0e-4);
    double rotAngle = OD_ATAN2(direction.y, direction.x);
    if(rotAngle < 0.0)
    {
      rotAngle += Oda2PI;
    }
    pBlkRef->setRotation(rotAngle);

    ents.push_back(pBlkRef);
  }
}

bool OdDimRecomputor::toExtend(OdDbObjectId arrowId)
{
  return OdDmUtil::isZeroLengthArrow(OdDmUtil::arrowName(arrowId));
}

// Checks the intersection the line segment(startPoint, endPoint) and segment((x, minY), (x, maxY))
// Must be conditions:
// startPoint.x <= endPoint.x
// minY <= maxY
inline bool check_intersection(OdGePoint2d startPoint, OdGePoint2d endPoint,
                               double x, double minY, double maxY, OdGePoint2d& intersection)
{
  if(startPoint.x <= x && endPoint.x >= x && OdNonZero( endPoint.x - startPoint.x ) )
  {
    double y = startPoint.y + (x - startPoint.x) * (endPoint.y - startPoint.y) / (endPoint.x - startPoint.x);
    if(y >= minY && y <= maxY)
    {
      intersection = OdGePoint2d(x, y);
      return true;
    }
  }
  return false;
}

void OdDimRecomputor::intersectLineAndExts(const OdGePoint3d& dimLine1End, const OdGePoint3d& dimLine2End,
                                           const OdGeExtents3d& exts, OdGePoint2dArray& intersections)
{
  OdGePoint2d start, end;
  start.x = dimLine1End.x;
  start.y = dimLine1End.y;
  end.x   = dimLine2End.x;
  end.y   = dimLine2End.y;

  if(start.x > end.x)
  {
    std::swap(start, end);
  }

  // check first(vertical) boundary
  OdGePoint2d intersection;
  if(check_intersection(start, end, exts.minPoint().x, exts.minPoint().y, exts.maxPoint().y, intersection))
  {
    intersections.push_back(intersection);
  }

  // second(vertical)
  if(check_intersection(start, end, exts.maxPoint().x, exts.minPoint().y, exts.maxPoint().y, intersection))
  {
    intersections.push_back(intersection);
  }

  // to check intersections with horizontal lines we must swap coordinates
  std::swap(start.x, start.y);
  std::swap(end.x,   end.y);
  if(start.x > end.x)
  {
    std::swap(start, end);
  }

  // below is 2 coincident paragraphs but________ this word
  if(check_intersection(start, end, exts.minPoint().y, exts.minPoint().x, exts.maxPoint().x, intersection))
  {
    OdGePoint2d realPoint;
    realPoint.x = intersection.y;
    realPoint.y = intersection.x;
    bool exist = false;
    for(OdGePoint2dArray::iterator it = intersections.begin(); it != intersections.end(); ++ it)
    {
      if((*it).isEqualTo(realPoint))
      {
        exist = true;
        break;
      }
    }
    if(!exist)
    {
      intersections.push_back(realPoint);
    }
  }

  if(check_intersection(start, end, exts.maxPoint().y, exts.minPoint().x, exts.maxPoint().x, intersection))
  {
    OdGePoint2d realPoint;
    realPoint.x = intersection.y;
    realPoint.y = intersection.x;
    bool exist = false;
    for(OdGePoint2dArray::iterator it = intersections.begin(); it != intersections.end(); ++ it)
    {
      if((*it).isEqualTo(realPoint))
      {
        exist = true;
        break;
      }
    }
    if(!exist)
    {
      intersections.push_back(realPoint);
    }
  }
}

inline double dimRound(double value, double rnd, OdInt16 precision)
{
  // rounding value
  //   precision is DIMDEC
  //   rnd is DIMRND

  //if(fabs(rnd) > 1.0e-10)
  //{
  //  value = OdRound(value) / rnd) * rnd;
  //}
  // #5895 :
  if (rnd < 0.0) // DIMRND can not be negative
  {
    ODA_FAIL_ONCE();
    rnd = - rnd;
  }
  // after real experiments with ACAD (dimrnd_are_1e-2_and_1e-3.dwg etc)
  // It's look like 2 iteration of rounding :
  // 1. rounding with negative delta via DIMRND
  if (rnd > 1.0e-10)
  {
    double a = value/rnd;
    double aFloor = ::floor(a);
    if ( OdZero(rnd/2.0 - fmod(value, rnd)) )
    {
      if ( (OdInt32)(a) % 2 != 0 )
        aFloor = aFloor + 1.0;
    } else if ( a-aFloor > 0.5)
      aFloor = aFloor+1.0;

    value = aFloor * rnd;
  }
  // 2. rounding via current precision only 
  // if (precision > 0)  // #8840
  // {
    double rndScnd = pow(0.1, precision);
    if (rndScnd > fabs(rnd))
      value = OdRound((value + 1.0e-10) / rndScnd) * rndScnd;
  // }
  return  value;
}

// Formatting dimension string when units is decimal
void OdDimRecomputor::formatDimDecStr(OdString& dimStr, bool supressLeading, bool supressTrailing,
                                      OdInt16 precision, double value, double rnd, OdChar separator)
{
  value = dimRound(value, rnd, precision);
  //OdString format;
  //format.format(OD_T("%%.%ilf"), precision);
  //dimStr.format(format, value);
  dimStr = odDToStr(value, 'f', precision);
  if(supressLeading)
  {
    // supressing leading zeros(0.5 becomes .5)
    int numLeadZeros = 0;
    int length = dimStr.getLength();
    for(int i = 0; i < length; i ++)
    {
      if(dimStr[i] == '.')
      {
        break;
      }
      else if(dimStr[i] == '0')
      {
        numLeadZeros ++;
      }
      else
      {
        break;
      }
    }
    dimStr = dimStr.right(length - numLeadZeros);
  }
  if(supressTrailing && (precision > 0))
  {
    // supressing trailing zeros(12.50 becomes 12.5)
    int numTrailZeros = 0;
    int length = dimStr.getLength();
    for(int i = 0; i < length; i ++)
    {
      if(dimStr[length - i - 1] == '.')
      {
        numTrailZeros ++;
        break;
      }
      else if(dimStr[length - i - 1] == '0')
      {
        numTrailZeros ++;
      }
      else
      {
        break;
      }
    }
    dimStr = dimStr.left(length - numTrailZeros);
  }
  if ( dimStr.isEmpty() )
    dimStr = '0'; // value is zero
  if(separator) // GU, 02.08.2004, cr 2044
    dimStr.replace('.', separator);
}

inline void breakToDiv(double value, int& entier, int& numerator, int& denominator, int precision)
{
  bool neg = false;
  if(value < 0)
  {
    neg = true;
    value = -value;
  }
  denominator = 1 << precision;
  value = OdRound(value * denominator) / (double)denominator;
  entier = (int)floor(value);
  numerator = (int)OdRound((value - (double)entier) * (double)denominator);
  while((numerator % 2) == 0 && (numerator != 0))
  {
    numerator   /= 2;
    denominator /= 2;
  }
  if(neg)
  {
    entier = -entier;
  }
}

// Formatting dimension string when units is architectural
void OdDimRecomputor::formatDimArcStr(OdString& dimStr, OdInt16 precision, double value, double rnd,
                                      OdInt16 fracFormat, bool forLimits, bool forTolerances, bool bSupressFeet, bool bSupressInch, double dimTfac )
{
  int entier, numerator, denominator;
  // rounding value
  if(fabs(rnd) > 1.0e-10)
  {
    value = OdRound(value / rnd) * rnd;
  }
  breakToDiv(value, entier, numerator, denominator, precision);
  int bigUnits = entier / 12;
  entier %= 12;
  if(numerator != 0)
  {
    if(forLimits)
    {
      /* TODO: test
      if ( entier == 0 &&  bSupressInch )
        dimStr.format(OD_T("%i\\/%i\")", numerator, denominator);
      else */
        dimStr.format(OD_T("%i %i\\/%i\""), entier, numerator, denominator);
    }
    else if(forTolerances)
    {
      /* TODO: test
      if ( entier == 0 &&  bSupressInch )
        dimStr.format(OD_T("%i/%i\")", numerator, denominator);
      else */
        dimStr.format(OD_T("%i %i/%i\""), entier, numerator, denominator);
    }
    else
    {
      // dimStr.format(OD_T("%i{\\H1.000000x;\\S%i/%i;}\")", entier, numerator, denominator);

      // seems to be same as in frac (GU, 18.09.2003)
      OdString swHeight = odDToStr(dimTfac, 'f', 6);
      switch(fracFormat)
      {
      case 0: // Horizontal
        if ( !bSupressFeet || entier || bigUnits > 0 ) // Bricsys: zero entier is displayed if numerator != 0 and feet is displayed 
          dimStr.format(OD_T("%i{\\H%lsx;\\S%i/%i;}\""), entier, swHeight.c_str(), numerator, denominator);
          // dimStr.format(OD_T("%i{\\H%.6lfx;\\S%i/%i;}\""), entier, dimTfac, numerator, denominator);
        else
          dimStr.format(OD_T("{\\H%lsx;\\S%i/%i;}\""), swHeight.c_str(), numerator, denominator);
          // dimStr.format(OD_T("{\\H%.6lfx;\\S%i/%i;}\""), dimTfac, numerator, denominator);
        break;
      case 1: // Diagonal
        if ( !bSupressFeet || entier || bigUnits > 0 ) // Bricsys: zero entier is displayed if numerator != 0 and feet is displayed 
          dimStr.format(OD_T("%i{\\H%lsx;\\S%i#%i;}\""), entier, swHeight.c_str(), numerator, denominator);
          // dimStr.format(OD_T("%i{\\H%.6lfx;\\S%i#%i;}\""), entier, dimTfac, numerator, denominator);
        else
          dimStr.format(OD_T("{\\H%lsx;\\S%i#%i;}\""), swHeight.c_str(), numerator, denominator);
          // dimStr.format(OD_T("{\\H%.6lfx;\\S%i#%i;}\""), dimTfac, numerator, denominator);
        break;
      case 2: // Not stacked
        if ( !bSupressFeet || entier || bigUnits > 0 )  // Bricsys: zero entier is displayed if numerator != 0 and feet is displayed 
          dimStr.format(OD_T("%i %i/%i\""), entier, numerator, denominator);
        else
          dimStr.format(OD_T("%i/%i\""), numerator, denominator);
        break;
      default:
        ODA_FAIL();
      }
    }
  }
  else
  {
    if (entier || !bSupressInch)
      dimStr.format(OD_T("%i\""), entier);
  }
  if ( bigUnits || !bSupressFeet )
  {
    OdString BUstr;
    BUstr.format(OD_T("%i'"), bigUnits);
    if ( !dimStr.isEmpty() ) // || !bSupressFeet )
      dimStr = BUstr + "-" + dimStr;
    else
      dimStr = BUstr;
  }
  // if ( (forTolerances || forLimits ) && dimStr.isEmpty() )
  if (dimStr.isEmpty())
    dimStr = OD_T("0\"");
}

// Formatting dimension string when units is scientific
void OdDimRecomputor::formatDimScientStr(OdString& dimStr, OdInt16 precision, double value, double rnd)
{
  // rounding value
  if(fabs(rnd) > 1.0e-10)
  {
    value = OdRound(value / rnd) * rnd;
  }
  //OdString format;
  //format.format(OD_T("%%.%ile"), precision);
  //dimStr.format(format, value);
  //dimStr.replace('e', 'E');
  dimStr = odDToStr(value, 'E', precision);
  dimStr = dimStr.left(dimStr.getLength() - 3) + dimStr.right(2);
}

void OdDimRecomputor::formatDimFracStr(OdString& dimStr, OdInt16 precision, double value, double rnd,
                                       OdInt16 fracFormat, bool forLimits, bool forTolerances, double dimTfac)
{
  // limits case implementation
  int entier, numerator, denominator;
  // rounding value
  if(fabs(rnd) > 1.0e-10)
  {
    value = OdRound(value / rnd) * rnd;
  }
  breakToDiv(value, entier, numerator, denominator, precision);
  if(numerator != 0)
  {
    if(forLimits)
    {
      if(entier != 0)
        dimStr.format(OD_T("%i %i\\/%i"), entier, numerator, denominator);
      else
        dimStr.format(OD_T("%i\\/%i"), numerator, denominator);
    }
    else if(forTolerances)
    {
      if(entier != 0)
        dimStr.format(OD_T("%i %i/%i"), entier, numerator, denominator);
      else
        dimStr.format(OD_T("%i/%i"), numerator, denominator);
    }
    else
    {
      OdString swHeight = odDToStr(dimTfac, 'f', 6);
      switch(fracFormat)
      {
      case 0: // Horizontal
        if(entier != 0)
          dimStr.format(OD_T("%i{\\H%lsx;\\S%i/%i;}"), entier, swHeight.c_str(), numerator, denominator);
          // dimStr.format(OD_T("%i{\\H%.6lfx;\\S%i/%i;}"), entier, dimTfac, numerator, denominator);
        else
          dimStr.format(OD_T("{\\H%lsx;\\S%i/%i;}"), swHeight.c_str(), numerator, denominator);
          // dimStr.format(OD_T("{\\H%.6lfx;\\S%i/%i;}"), dimTfac, numerator, denominator);
        break;
      case 1: // Diagonal
        if(entier != 0)
          dimStr.format(OD_T("%i{\\H%lsx;\\S%i#%i;}"), entier, swHeight.c_str(), numerator, denominator);
          // dimStr.format(OD_T("%i{\\H%.6lfx;\\S%i#%i;}"), entier, dimTfac, numerator, denominator);
        else
          dimStr.format(OD_T("{\\H%lsx;\\S%i#%i;}"), swHeight.c_str(), numerator, denominator);
          // dimStr.format(OD_T("{\\H%.6lfx;\\S%i#%i;}"), dimTfac, numerator, denominator);
        break;
      case 2: // Not stacked
        if(entier != 0)
          dimStr.format(OD_T("%i %i/%i"), entier, numerator, denominator);
        else
          dimStr.format(OD_T("%i/%i"), numerator, denominator);
        break;
      default:
        ODA_FAIL();
      }
    }
  }
  else
  {
    dimStr.format(OD_T("%i"), entier);
  }
}

// common dimension text formatting function calling previous ones
// Parameters:
// dimStr    - string receiving result of formatting
// value     - source dimension value
// units     - unit format
// precision - precision :)
// roundOff  - value to round to dimension value
// tzin      - DIMTZIN value
// separator - decimal separator in text
// fracFormat- (for fractional dimensions) fraction format
// forLimits - generates dimension text for limits
void OdDimRecomputor::formatDimStr(OdString& dimStr, double value, OdInt16 units, OdInt16 precision,
                                   double roundOff, OdInt16 tzin, OdChar separator, OdInt16 fracFormat,
                                   bool forLimits, bool forTolerances, double dimTfac)
{
  switch(units)
  {
  case 1: // Scientific
    formatDimScientStr(dimStr,  precision, value, roundOff);
    break;
  case 6: // Windows desktop is just like decimal
          // I think it is needn't to implement one more function in hostApplicationservices
          // to duplicate dimsep variable(in fact)
          // (CR 2890) TODO: Windows desktop units is NOT like a decimal ones
#ifdef TD_USE_LOCALE
    {
      SET_LOCALE(LC_NUMERIC, OD_T(""))
      separator = (unsigned char)localeconv()->decimal_point[0];
      // separator = localeconv()->_W_decimal_point[0];
      OdChar thousands_sep = (unsigned char)localeconv()->thousands_sep[0];
      /// OdChar thousands_sep = localeconv()->_W_thousands_sep[0];

      formatDimDecStr(dimStr, (tzin & 4) != 0, (tzin & 8) != 0, precision, value, roundOff, separator);
      OdInt32 point = dimStr.find(separator);
      OdInt32 count = 0;
      if ( point == -1 )
        count = dimStr.getLength();
      else
        count = point;
      if ( count > 3 )
      {
        OdString newStr;
        OdInt32 p1 = count % 3;
        if ( p1 > 0 )
        {
          newStr += dimStr.getAt(0);
          if ( p1 > 1 )
            newStr += dimStr.getAt(1);
          newStr += thousands_sep;
        }
        for ( OdInt32 i = p1; i < count; i += 3)
        {
          newStr += dimStr.getAt(i);
          if ( count > i+1 )
            newStr += dimStr.getAt(i+1);
          if ( count > i+2 )
            newStr += dimStr.getAt(i+2);
          if ( count > i+3 )
            newStr += thousands_sep;
        }
        if ( point != -1 )
        {
           newStr += dimStr.mid(point);
        }
        dimStr = newStr;
      }
      break;
    }
#endif
  case 2: // Decimal
    formatDimDecStr(dimStr, (tzin & 4) != 0, (tzin & 8) != 0, precision, value, roundOff, separator);
    break;
  case 3: // Engineering
    {
      OdString inches;
      value = dimRound(value, roundOff, precision);

      int bigUnits = (int)floor(value / 12.0);
      value = fmod(value, 12.0);

      formatDimDecStr(inches, (tzin & 4) != 0, (tzin & 8) != 0, precision, value, roundOff, '.');

      int  acrchZin = tzin & 3;
      bool inchSupress = ( acrchZin == 2 || acrchZin == 0 );
      bool feetSupress = ( acrchZin == 3 || acrchZin == 0 );

      if( bigUnits || !feetSupress)
      {
        if ( OdNonZero(value) || !inchSupress)
          dimStr.format(OD_T("%i'-%ls\""), bigUnits, inches.c_str());
        else
          dimStr.format(OD_T("%i'"), bigUnits);
      }
      else
      {
        dimStr.format(OD_T("%ls\""), inches.c_str());
      }
      break;
    }
  case 4: // Architectural
    {
      int  acrchZin = tzin & 3;
      bool feetSupress = ( acrchZin == 3 || acrchZin == 0 );
      bool inchSupress = ( acrchZin == 2 || acrchZin == 0 );
      formatDimArcStr(dimStr, precision, value, roundOff, fracFormat, forLimits, forTolerances, feetSupress, inchSupress, dimTfac);
    }
    break;
  case 5: // Fractional
    formatDimFracStr(dimStr, precision, value, roundOff, fracFormat, forLimits, forTolerances, dimTfac);
    break;
  case 7: // Fractional
    formatDimFracStr(dimStr, precision, value, roundOff, 2, forLimits, forTolerances, dimTfac);
    break;
  }
}

// Formats dimension string taking tolerances and limits into account
// measurement  - formatted(result) string
// isLims       - wether limits present for this dimension
// isTols       - wether tolerances present for this dimension
// dMeasurement - double value of dimension(OD_T("average"))
// dimLunit     - dimension units
// dimTm        - low tolerance(difference between low limit and dMeasurement)
// dimTp        - high tolerance(difference between high limit and dMeasurement)
// dimTzin      - defining supressing zeros for tolerances and limits
// dimTfac      - tolerance factor
// dimTdec      - tolerance precision
// dimRnd       - roundOff precision
// dimDsep      - decimal separator(for decimal dimensions)
// dimFrac      - fraction format(for fractional dimensions)
// dimTxt       - text height
// dimZin       - defining supressing zeros for main dimension text
// dimDec       - main dimension value precision
// dimPost      - string defining prefix and suffix for measurement
void OdDimRecomputor::formatDimStrWithLimsAndTols(OdString& measurement, bool isLims, bool isTols,
                                                  double dMeasurement, OdInt16 dimLunit,
                                                  double dimTm, double dimTp, OdInt16 dimTzin, double dimTfac,
                                                  OdInt16 dimTdec, double dimRnd, OdChar dimDsep,
                                                  OdInt16 dimFrac, double dimTxt, OdInt16 dimZin, OdInt16 dimDec,
                                                  OdString dimPost, bool bPrimaryUnits)
{
  // Taking limits into account
  // adding prefix and suffix
  // GU 18.11.2004 a bit rewritten (cr 2381)
  OdString prefix, suffix;
  int numQuotesPost = dimPost.find(OD_T("<>"));  // dimpost - primary units 
  int numQuotesApost = dimPost.find(OD_T("[]")); // dimapost - alternate units

  if ( ( bPrimaryUnits && numQuotesPost != -1) || ( !bPrimaryUnits && numQuotesApost != -1 ) )
  {
    numQuotesPost = ( numQuotesApost != -1 ) ? numQuotesApost : numQuotesPost;
    prefix = dimPost.left(numQuotesPost);
    suffix = dimPost.mid(numQuotesPost+2);
  } else
  {
    suffix = dimPost; // prefix is empty
  }

  if(isLims)
  {
    OdString lowLim, highLim;

    formatDimStr(lowLim, dMeasurement - dimTm, dimLunit, dimTdec, dimRnd, dimTzin, dimDsep, dimFrac, true, false);
    formatDimStr(highLim, dMeasurement + dimTp, dimLunit, dimTdec, dimRnd, dimTzin, dimDsep, dimFrac, true, false);

    if ( m_dimtaln )
    {
      OdString formStr = OD_T("\\S%ls~") + dimDsep;
      formStr = formStr + OD_T("%ls;");
      measurement.format(formStr.c_str(), highLim.c_str(), lowLim.c_str());
      // measurement.format(OD_T("\\S%ls~.%ls;"), highLim.c_str(), lowLim.c_str());
    } else
    {
      measurement.format(OD_T("\\S%ls^%ls;"), highLim.c_str(), lowLim.c_str());
    }

    if(fabs(dimTfac - 1.0) > 1.0e-10) // size
    {
      OdString mesCopy = measurement;
      OdString swHeight = odDToStr(dimTfac * dimTxt, 'f', 6);
      measurement.format(OD_T("{\\H%ls;%ls}"), swHeight.c_str(), mesCopy.c_str());
      // measurement.format(OD_T("{\\H%.6lf;%ls}"), dimTfac * dimTxt, mesCopy.c_str());
    }
  }
  else
  {
    // There are no limits
    formatDimStr(measurement, dMeasurement, dimLunit, dimDec, dimRnd, dimZin, dimDsep, dimFrac, false, false, dimTfac);
  }
  // Taking tolerances into account
  OdString tolerance;
  if(isTols)
  {
    if(fabs(dimTm - dimTp) < 1.0e-14) // I think such condition is quite normal here //#8454 11 ->14
    {
      formatDimStr(tolerance, dimTm, dimLunit, dimTdec, 0.0, dimTzin, dimDsep, dimFrac, false, true);
      if ( numQuotesPost == -1)
        tolerance = OD_T("%%p") + tolerance + suffix;
      else
        tolerance = OD_T("%%p") + tolerance;
    }
    else
    {
      OdString tolUpper, tolLower;
      // 29.11.2004 GU, cr2400 (added fabs)
      formatDimStr(tolUpper, fabs(dimTp), dimLunit, dimTdec, 0.0, dimTzin, dimDsep, dimFrac, true, false);
      formatDimStr(tolLower, fabs(dimTm), dimLunit, dimTdec, 0.0, dimTzin, dimDsep, dimFrac, true, false);

      // tolerance = OD_T("\\S") + OdString((dimTp >= 0) ? OD_T("+") : OD_T("-")) + tolUpper + OD_T("^") + 
      //  ((dimTm >= 0) ? OD_T("-") : OD_T("+")) + tolLower + OD_T(";"); // GU, 03.08.2004, cr2102
      if ( !suffix.isEmpty() && numQuotesPost == -1 ) // prefix.isEmpty() )
      {
        if ( !tolUpper.isEmpty() )
          tolUpper += suffix;
        if ( !tolLower.isEmpty() )
          tolLower += suffix;
      }

      tolerance = OD_T("\\S");
      if ( OdNonZero(dimTp, 1.0e-11) || !m_pDb->getMEASUREMENT() )
        tolerance += ((dimTp >= 0) ? OD_T("+") : OD_T("-"));
      else
        tolerance += " ";

      if ( m_dimtaln )
        tolerance += tolUpper + OD_T("~") + dimDsep;
      else
        tolerance += tolUpper + OD_T("^");

      if ( OdNonZero(dimTm, 1.0e-11) || !m_pDb->getMEASUREMENT() )
        tolerance += ((dimTm >= 0) ? OD_T("-") : OD_T("+"));
      else
        tolerance += " ";

      tolerance += tolLower + OD_T(";");
    }
    // size
    if(fabs(dimTfac - 1.0) > 1.0e-10)
    {
      OdString tolCpy = tolerance;
      OdString swHeight = odDToStr(dimTfac * dimTxt, 'f', 6);
      tolerance.format(OD_T("{\\H%ls;%ls}"), swHeight.c_str(), tolCpy.c_str());
      // tolerance.format(OD_T("{\\H%.6lf;%ls}"), dimTfac * dimTxt, tolCpy.c_str());
    }
  }
  if ( !tolerance.isEmpty() )
  {
    if ( !suffix.isEmpty() && numQuotesPost == -1 ) // prefix.isEmpty() )
    {
      measurement += suffix;
      measurement += tolerance;
    } else  
    {
      measurement += tolerance;
      measurement = prefix + measurement + suffix;
    }
  } else
  {
     measurement = prefix + measurement + suffix;
  }
}

void OdDimRecomputor::formatDimStrWithAltUnits(OdString& measurement,
                                               OdDbDimStyleTableRecord* pDimVars)
{
  bool   isTol      = getDimtol(pDimVars);
  bool   isLims     = getDimlim(pDimVars);
  if ( isTol && isLims ) // if set both variable - isTol has priority #8434
    isLims = false;
  double dimTm      = getDimtm(pDimVars);
  double dimTp      = getDimtp(pDimVars);
  double textHeight = getDimtxt(pDimVars);
  OdChar dimDsep    = (OdChar)getDimdsep(pDimVars);
  OdInt16 dimFrac   = getDimfrac(pDimVars);
  double dimTfac    = getDimtfac(pDimVars);
  OdInt16 dimDec    = getDimdec(pDimVars);
  dimDec = ( dimDec < 0 ? 0 : dimDec );
  int quotesNum = m_strDimText.find(OD_T("<>"));
  bool bUseAltUnitStr = false;
  OdString altUnitStr;
  if(m_strDimText.isEmpty() || (quotesNum != -1))
  {
    OdString mzs = getDimmzs(pDimVars);
    double dimVal = m_dMeasurement;
    OdString dimPost = getDimpost(pDimVars);

    OdInt16 dimLunit = getDimlunit(pDimVars);
    // #23347
    if (!mzs.isEmpty() && (dimLunit == 2 || dimLunit == 6) && getDimzin(pDimVars))
    {
      // apply sub-unit suffix and sub-unit scale
      double mzf = getDimmzf(pDimVars);
      if ( mzf*dimVal < mzf )
      {
        dimVal = mzf*dimVal;
        OdString prefix;
        int numQuotesPost = dimPost.find(OD_T("<>"));  // dimpost - primary units 
        if ( numQuotesPost != -1 )
        {
          prefix = dimPost.left(numQuotesPost);
        }
        dimPost = prefix + OD_T("<>") + mzs;
      }
    }
    
    // text in primary units
    formatDimStrWithLimsAndTols(measurement, isLims, isTol, dimVal,
      getDimlunit(pDimVars), dimTm, dimTp, getDimtzin(pDimVars),
      dimTfac, getDimtdec(pDimVars),
      getDimrnd(pDimVars), dimDsep,
      dimFrac, textHeight, getDimzin(pDimVars),
      dimDec, dimPost, true);

    // alternative units
    bool bUseAltUnitVar = getDimalt(pDimVars);
    bUseAltUnitStr = (m_strDimText.find(OD_T("[]")) != -1);

    if ( bUseAltUnitVar || bUseAltUnitStr ) // getDimalt(pDimVars))
    {
      OdString alternative;
      double altFactor = getDimaltf(pDimVars);
      bool stacked = false;
      OdInt16 altU = getDimaltu(pDimVars);
      if(altU == 4 || altU == 5)// architectural and fractional(stacked)
      {
        //Dimfrac -  Sets the fraction format when DIMLUNIT is set to 4 (Architectural) or 5 (Fractional).
        // 0	Horizontal stacking
        // 1	Diagonal stacking
        // 2	Not stacked (for example, 1/2)
        stacked = true;
      } else if ( altU == 6 )
      {
        altU = 4;
      } else if ( altU == 7 )
      {
        altU = 5;
      } else if (altU == 8 )
      {
        if ( getDimlunit(pDimVars) == 2 )
          altU = 2;
        else
          altU = 6;
      } else if ( altU == 2 && getDimlunit(pDimVars) == 6 )
      {
        altU = 6;
      }
      formatDimStrWithLimsAndTols(alternative, isLims, isTol, m_dMeasurement * altFactor,
        altU, dimTm * altFactor, dimTp * altFactor,
        getDimalttz(pDimVars), dimTfac /*??*/,
        getDimalttd(pDimVars), getDimaltrnd(pDimVars), dimDsep,
        stacked ? dimFrac : 2, textHeight, getDimaltz(pDimVars),
        getDimaltd(pDimVars), getDimapost(pDimVars), false);
      OdString skobka1, skobka2;
      if(isLims || (isTol && fabs(dimTm - dimTp) > 1.0e-10))
      {
        double vel = textHeight * 2.4 * dimTfac;
        OdString swHeight = odDToStr(vel, 'f', 6);

        skobka1.format(OD_T("{\\o\\l\\H%ls;[}"), swHeight.c_str());
        skobka2.format(OD_T("{\\o\\l\\H%ls;]}"), swHeight.c_str());
        // skobka1.format(OD_T("{\\o\\l\\H%.6lf;[}"), vel);
        // skobka2.format(OD_T("{\\o\\l\\H%.6lf;]}"), vel);
      }
      else
      {
        skobka1 = OD_T("[");
        skobka2 = OD_T("]");
      }
      if ( bUseAltUnitStr )
        altUnitStr = OD_T(" ") + skobka1 + alternative + skobka2;
      else
        measurement += OD_T(" ") + skobka1 + alternative + skobka2;
    }
  }
  else
  {
    measurement = m_strDimText;
  }
    
 
  if(quotesNum != -1)
  {
    OdString oldMes = measurement;
    // #8532 - ACAD use the first pair "<>"
    measurement     =  m_strDimText.left(quotesNum);
    measurement     += oldMes;
    measurement     += m_strDimText.right(m_strDimText.getLength()-quotesNum-2);
    // measurement.replace(OD_T("<>"), oldMes.c_str());  
  }

  OdUInt8 tolj = getDimtolj(pDimVars);
  if(tolj != 0)
  {
    OdString toljAdd;
    toljAdd.format(OD_T("\\A%i;"), (int)tolj);
    measurement = toljAdd + measurement;
  }

  if ( bUseAltUnitStr )
  {
    // measurement.replace(OdString(OD_T("[]")), altUnitStr);
    const int altQuotesNum = measurement.find(OD_T("[]"));  // #14007
    ODA_ASSERT(altQuotesNum >= 0); // otherwise bUseAltUnitStr would not be set
    measurement = measurement.left(altQuotesNum) + altUnitStr +
          measurement.right(measurement.getLength() - altQuotesNum - 2);
  }
}

void OdDimRecomputor::formatMeasurement(OdString& measurement, OdDbDimStyleTableRecord* pDimVars)
{
  formatDimStrWithAltUnits(measurement, pDimVars);

  // working with OD_T("\X") symbol -- G. Udov, 29.07.2003
  int xPos = measurement.find(OD_T("\\X"));
  if(xPos != -1)
  {
    measurement.setAt(xPos+1,'P');
  }
}

void OdDimRecomputor::setTextProperty(OdDbMTextPtr& pText, OdDbDimStyleTableRecord* pDimVars)
{
  pText = OdDbMText::createObject();
  pText->setDatabaseDefaults(database());

  OdDbObjectId styleId = getDimtxsty(pDimVars);
  if (styleId.isNull())
  { 
    styleId = database()->getTextStyleStandardId();
  }

  OdDbTextStyleTableRecordPtr textStyle = OdDbTextStyleTableRecord::cast(styleId.safeOpenObject());
  if ( textStyle.isNull() )
  {
    styleId = database()->getTextStyleStandardId();
    textStyle = OdDbTextStyleTableRecord::cast(styleId.safeOpenObject());
  }
  pText->setTextStyle(styleId);
  // pText->setLocation(m_ocsDimTextMidPt - OdGeVector3d(0.0, 0.0, m_ocsDimTextMidPt.z));
  pText->setWidth(0.0);
  pText->setColor(getDimclrt(pDimVars));
  pText->setTransparency(m_transparency);

  double textHeight = textStyle->textSize();
  if ( OdZero(textHeight) )
  {
    textHeight = getDimtxt(pDimVars);
  } else if ( m_bAnnotative )
  {
    textHeight *= getDimscale(pDimVars); // CR5899
  }
  if ( textHeight > 0.0 )
    pText->setTextHeight(textHeight);
  pText->setAttachment(OdDbMText::kMiddleCenter);
  pText->setLineSpacingStyle(m_LinespacingStyle != OdDb::kExactly ? OdDb::kAtLeast : OdDb::kExactly);
  if ( m_dLinespacingFactor < 0.25 || m_dLinespacingFactor > 4.0 )
    pText->setLineSpacingFactor(1.0);
  else
    pText->setLineSpacingFactor(m_dLinespacingFactor);

  pText->setLineWeight((OdDb::LineWeight)m_nLineweight);

  if ( m_BgrndTxtFlag )
  {
    pText->setBackgroundFill(true);
    if ( m_BgrndTxtFlag & 1 )
    {
      pText->setUseBackgroundColor(true);
    }
    OdDbDatabase* pDb = database();
    if ( pDb && pDb->getMEASUREMENT() == OdDb::kMetric )
      pText->setBackgroundScaleFactor(1.1);
    else
      pText->setBackgroundScaleFactor(1.25);

    if ( m_BgrndTxtFlag & 2 )
      pText->setBackgroundFillColor(m_BgrndTxtColor);
  }
  pText->setWidth(m_dTextWidth);
  // pText->setHeight(m_dTextHeight);
}

static OdString convertToMText(OdString textStr)
{
  OdString mText;
  bool bUnderline = false;
  bool bOverline = false;

  if ( textStr.find(OD_T("%%")) == -1 ) 
    return textStr;

  int srcSize = textStr.getLength();
  for (int i = 0; i < textStr.getLength(); ++i)
  {
    if (textStr[i] == '%' && i+1 < srcSize && textStr[i + 1] == '%')
    {
      if ((i + 2 < srcSize))
      {
        if ( textStr[i + 2] == 'U' || textStr[i + 2] == 'u')
        {
          if ( !bUnderline )
          {
            mText += L"\\L";
          } else
          {
            mText += L"\\l";
          }
          bUnderline = !bUnderline;
          i += 2;
        } else if ( textStr[i+2] == 'O' || textStr[i+2] == 'o')
        {
          if ( !bOverline )
          {
            mText += L"\\O";
          } else
          {
            mText += L"\\o";
          }
          bOverline = !bOverline;
          i += 2;
        } else
        {
          mText += textStr[i];
        }
      }
      else
      {
        mText += textStr[i];
      }
    } else
    {
      mText += textStr[i];
    }
  }
  return mText;
}

void OdDimRecomputor::createTextEntityForDimGraphic(OdDbMTextPtr& pText,
                                                    OdDbDimStyleTableRecord* pDimVars)
{
  if(m_strDimText != OD_T(" "))
  {
    pText = OdDbMText::createObject();
    setTextProperty(pText, pDimVars);
    pText->setLocation(m_ocsDimTextMidPt - OdGeVector3d(0.0, 0.0, m_ocsDimTextMidPt.z));
    OdString measurement;
    formatMeasurement(measurement, pDimVars);
    
    if ( !m_strDimText.isEmpty() )   // #7198
    {
      measurement = convertToMText(measurement); 
    }
    pText->setContents(measurement);
  }
}

void OdDimRecomputor::createInspectionEntitisForDimGraphic(OdDbMTextPtr& pLabel, OdDbMTextPtr& pRate,
                                                    OdDbDimStyleTableRecord* pDimVars)
{
  if ( !m_bInspection )
    return;

  if ( m_nInspectionFrame & OdDbDimension::kShapeLabel )
  {
    pLabel = OdDbMText::createObject();
    setTextProperty(pLabel, pDimVars);
    pLabel->setContents(m_sInspectionLabel);
  }

  if ( m_nInspectionFrame & OdDbDimension::kShapeRate )
  {
    pRate = OdDbMText::createObject();
    setTextProperty(pRate, pDimVars);
    pRate->setContents(m_sInspectionRate);
  }
}

OdDbObjectId OdDimRecomputor::getLayerDefpointsId()
{
  return database()->getLayerDefpointsId(true);
}

void OdDimRecomputor::intersectLines(OdGePoint3d& intersectionPoint, const OdGePoint3d& firstLinePoint,
                                     const OdGePoint3d& secondLinePoint, const OdGeVector3d& firstLineVector,
                                     const OdGeVector3d& secondLineVector)
{
  intersectionPoint = firstLinePoint + firstLineVector *
    (secondLineVector.x*(secondLinePoint.y - firstLinePoint.y) -
    secondLineVector.y*(secondLinePoint.x - firstLinePoint.x)) /
    (secondLineVector.x * firstLineVector.y - secondLineVector.y * firstLineVector.x);
}

// Bad function. Need to redesign. Probably database functionality.
// Returns xAxis and yAxis of the current UCS(in OCS).
void OdDimRecomputor::getUCS(OdGeVector3d& xAxis, OdGeVector3d& yAxis)
{
  OdGeMatrix3d toUCS(OdGeMatrix3d::rotation(-m_dUcsXAngle, OdGeVector3d::kZAxis));
  xAxis = OdGeVector3d::kXAxis;
  yAxis = OdGeVector3d::kYAxis;
  xAxis.transformBy(toUCS);
  yAxis.transformBy(toUCS);
}

#define ADD_LINE(a, b) \
  pLine = OdDbLine::createObject(); \
  pLine->setColor(color); \
  pLine->setLineWeight(lineweight); \
  pLine->setStartPoint(a); \
  pLine->setEndPoint(b); \
  pLine->setLinetype(m_linetypeId); \
  pLine->setLinetypeScale(m_dLinetypeScale); \
  ents.push_back(pLine);

// Builds boundary box about the text, if it is nessesary
// Assuming text knows its database
void OdDimRecomputor::buildBoundaryBox(OdDbDimStyleTableRecord* pDimVars,
                                       OdDbMText* pText,
                                       OdArray<OdDbEntityPtr>& ents)
{
  ODA_ASSERT(pText->database());
  OdGeExtents3d exts;
  OdGeVector3d direction = pText->direction();
  pText->setDirection(OdGeVector3d::kXAxis);
  pText->getGeomExtents(exts);
  pText->setDirection(direction);
  OdGePoint3d minPoint, maxPoint;
  minPoint = exts.minPoint();
  maxPoint = exts.maxPoint();
  double gap = getDimgap(pDimVars);
  bool isRectAroud = (gap < 0);
  if(!isRectAroud)
  {
    return;
  }
  gap = fabs(gap);
  minPoint.x -= gap;
  minPoint.y -= gap;
  maxPoint.x += gap;
  maxPoint.y += gap;
  exts.set(minPoint, maxPoint);
  OdGeMatrix3d toDir(OdGeMatrix3d::rotation(OdGeVector3d::kXAxis.angleTo(direction, OdGeVector3d::kZAxis),
    OdGeVector3d::kZAxis, pText->location()));
  OdGePoint3d first (exts.minPoint());
  OdGePoint3d second(exts.maxPoint().x, exts.minPoint().y, 0.0);
  OdGePoint3d third (exts.maxPoint());
  OdGePoint3d fourth(exts.minPoint().x, exts.maxPoint().y, 0.0);
  first.transformBy(toDir);
  second.transformBy(toDir);
  third.transformBy(toDir);
  fourth.transformBy(toDir);

  OdDbLinePtr pLine;
  OdCmColor color = getDimclrd(pDimVars);
  OdDb::LineWeight lineweight = OdDb::LineWeight(getDimlwd(pDimVars));

  ADD_LINE(first, second);
  ADD_LINE(second, third);
  ADD_LINE(third, fourth);
  ADD_LINE(fourth, first);
}

#define ADD_ARC(sAngle, eAngle) \
  pDimArc = OdDbArc::createObject(); \
  pDimArc->setLineWeight(lineweight); \
  pDimArc->setColor(color); \
  pDimArc->setCenter(centerPoint); \
  pDimArc->setRadius(radius); \
  pDimArc->setStartAngle((sAngle > Oda2PI) ? sAngle - Oda2PI : sAngle ); \
  pDimArc->setEndAngle((eAngle > Oda2PI) ? eAngle - Oda2PI : eAngle ); \
  pDimArc->setLinetype(dimLinetype); \
  pDimArc->setLinetypeScale(linetypeScale); \
  ents.push_back(pDimArc);

void OdDimRecomputor::makeInspectionFrame(OdDbDimStyleTableRecord* pDimVars, OdRecomputorEngine* pEngine, OdArray<OdDbEntityPtr>& ents)
{
  if ( !m_bInspection )
    return;

  OdGePoint3d  pos = pEngine->m_textPosition;
  OdGeVector3d dir = pEngine->m_textDirection;
  double textWidth = pEngine->m_textExtents.maxPoint().x - pEngine->m_textExtents.minPoint().x;
  double labelWidth = pEngine->m_labelExtents.maxPoint().x - pEngine->m_labelExtents.minPoint().x;
  double rateWidth = pEngine->m_rateExtents.maxPoint().x - pEngine->m_rateExtents.minPoint().x;

  OdGeMatrix3d toDir(OdGeMatrix3d::rotation(OdGeVector3d::kXAxis.angleTo(dir, OdGeVector3d::kZAxis),
    OdGeVector3d::kZAxis, pos));

  double overallWidth = pEngine->overallWidth();
  double halfWidth = overallWidth/2.0;

  double gap = ::fabs(getDimgap(pDimVars));
  double dGap = gap*2.0;
  
  // calc positions
  OdGeVector3d perpDir = pEngine->m_perpTextDir;
  OdGePoint3d startPos = pos;
  startPos.x -= halfWidth;

  OdGePoint3d curPos = startPos;

  if ( m_nInspectionFrame & OdDbDimension::kShapeLabel )
  {
    pEngine->m_labelPosition = curPos;
    pEngine->m_labelPosition.x += labelWidth/2.0;
    pEngine->m_labelPosition.transformBy(toDir);
    curPos.x += labelWidth + dGap;
  }
  pEngine->m_textPosition = curPos;
  pEngine->m_textPosition.x += textWidth/2.0;
  pEngine->m_textPosition.transformBy(toDir);

  if ( m_nInspectionFrame & OdDbDimension::kShapeRate )
  {
    curPos.x += textWidth + dGap;
    pEngine->m_ratePosition = curPos;
    pEngine->m_ratePosition.x += rateWidth/2.0;
    pEngine->m_ratePosition.transformBy(toDir);
  }

  // make frame
  OdGePoint3d first;
  OdGePoint3d second;
  curPos = startPos;

  if ( ( m_nInspectionFrame & OdDbDimension::kShapeRound )|| 
    ( m_nInspectionFrame & OdDbDimension::kShapeAngular ) )
  {
    OdDbLinePtr pLine;
    OdCmColor color = getDimclrd(pDimVars);
    OdDb::LineWeight lineweight = OdDb::LineWeight(getDimlwd(pDimVars));

    // Make horizontal Lines
    curPos.y += pEngine->textHeight()/2.0;

    first = second = curPos;
    second.x += overallWidth;
    first.transformBy(toDir);
    second.transformBy(toDir);

    ADD_LINE(first, second);

    curPos.y -= pEngine->textHeight();
    first = second = curPos;
    second.x += overallWidth;
    first.transformBy(toDir);
    second.transformBy(toDir);

    ADD_LINE(first, second);

    // Make vertical Lines

    curPos = startPos;

    curPos.y += pEngine->textHeight()/2.0;

    if ( m_nInspectionFrame & OdDbDimension::kShapeLabel )
    {
      curPos.x += labelWidth + dGap/2.0;

      first = second = curPos;
      second.y -= pEngine->textHeight();
      first.transformBy(toDir);
      second.transformBy(toDir);

      ADD_LINE(first, second);
      curPos.x += dGap/2.0;
    } 
    if ( m_nInspectionFrame & OdDbDimension::kShapeRate )
    {
      curPos.x += textWidth + dGap/2.0;

      first = second = curPos;
      second.y -= pEngine->textHeight();
      first.transformBy(toDir);
      second.transformBy(toDir);

      ADD_LINE(first, second);
    }

    curPos = startPos;

    if ( m_nInspectionFrame & OdDbDimension::kShapeRound )
    {
      // Make Arcs
      OdGePoint3d centerPoint;
      double radius;
      OdDbArcPtr pDimArc;
      OdDbObjectId dimLinetype = m_DimLinetypeId;
      double linetypeScale = m_dLinetypeScale;

      centerPoint = curPos;
      centerPoint.transformBy(toDir);
      radius = pEngine->textHeight()/2.0;
      OdGeCircArc2d arc;
      arc.set(OdGePoint2d(centerPoint.x, centerPoint.y), 
        radius, OdaPI2, OdaPI + OdaPI2 );
      centerPoint = OdGePoint3d(arc.center().x, arc.center().y, 0.0);
      arc.rotateBy(pEngine->m_dTextRot, OdGePoint2d(centerPoint.x, centerPoint.y));

      ADD_ARC(arc.startAngFromXAxis(), arc.endAngFromXAxis() );

      centerPoint = curPos;
      centerPoint.x += overallWidth;
      centerPoint.transformBy(toDir);
      radius = pEngine->textHeight()/2.0;
      // OdGeCircArc2d arc;
      arc.set(OdGePoint2d(centerPoint.x, centerPoint.y), 
        radius, OdaPI + OdaPI2, Oda2PI + OdaPI2);
      arc.rotateBy(pEngine->m_dTextRot, OdGePoint2d(centerPoint.x, centerPoint.y));

      ADD_ARC(arc.startAngFromXAxis(), arc.endAngFromXAxis() );

    } else if ( m_nInspectionFrame & OdDbDimension::kShapeAngular  )
    {
      // Make Lines
      first = curPos;
      first.x = curPos.x - pEngine->textHeight()/2.0;

      second = curPos;
      second.y += pEngine->textHeight()/2.0;

      first.transformBy(toDir);
      second.transformBy(toDir);

      ADD_LINE(first, second);

      second = curPos;
      second.y -= pEngine->textHeight()/2.0;
      second.transformBy(toDir);

      ADD_LINE(first, second);

      curPos.x += overallWidth;
      first = curPos;
      first.x = curPos.x + pEngine->textHeight()/2.0;

      second = curPos;
      second.y += pEngine->textHeight()/2.0;

      first.transformBy(toDir);
      second.transformBy(toDir);

      ADD_LINE(first, second);

      second = curPos;
      second.y -= pEngine->textHeight()/2.0;
      second.transformBy(toDir);

      ADD_LINE(first, second);
    }
  } else
  {
    // simple frame
    if ( getDimgap(pDimVars) < 0.0 )
    {
      OdDbLinePtr pLine;
      OdCmColor color = getDimclrd(pDimVars);
      OdDb::LineWeight lineweight = OdDb::LineWeight(getDimlwd(pDimVars));

      curPos.x += getDimgap(pDimVars);
      // Make horizontal Lines
      curPos.y += pEngine->textHeight()/2.0;

      first = second = curPos;
      second.x += overallWidth + dGap;
      first.transformBy(toDir);
      second.transformBy(toDir);

      ADD_LINE(first, second);

      OdGePoint3d p1 = first;
      OdGePoint3d p2 = second;

      curPos.y -= pEngine->textHeight();
      first = second = curPos;
      second.x += overallWidth + dGap;
      first.transformBy(toDir);
      second.transformBy(toDir);

      ADD_LINE(first, second);

      // Make vertical Lines
      ADD_LINE( p1, first);
      ADD_LINE( p2, second);

    }
  }
}

bool OdDimRecomputor::setUseDimLineIndex(OdDbDimension* pDim)
{
  OdDbDictionaryPtr pExtDic = OdDbDictionary::cast(pDim->extensionDictionary().openObject(OdDb::kForRead));
  if (pExtDic.get())
  {
    OdDbBreakDataPtr brData;
    brData = OdDbBreakData::cast(pExtDic->getAt(OD_T("ACAD_BREAKDATA"), OdDb::kForRead));
    if ( !brData.isNull() )
    {
      OdDbBreakPointRefPtrArray pntRefAray;
      brData->getBreakPointRef(pntRefAray);
      OdInt32 amPoints = pntRefAray.size();
      for ( OdInt32 i = 0; i < amPoints; i++ )
      {
        OdDbBreakPointRefPtr pntRef = pntRefAray[i];
        OdDbBreakPointRef::BreakPointType pntType = pntRef->pointType();
        if ( pntType == OdDbBreakPointRef::kStatic2Point )
        {
          m_bUseDimLineIndex = true;
          return m_bUseDimLineIndex;
        }
      }
    }
  }
  m_bUseDimLineIndex = false;
  return m_bUseDimLineIndex;
}

void OdDimRecomputor::setDimLineIndex(OdDbEntityPtr& entity, OdInt32 index)
{
  if ( !m_bUseDimLineIndex )
    return;
  entity->database()->newRegApp(OD_T("Index"));
  OdResBufPtr xd = OdResBuf::newRb(OdResBuf::kDxfRegAppName);
  xd->setString(OD_T("Index"));
  OdResBufPtr r = xd->last();
  r = r->setNext(OdResBuf::newRb(1070, (OdInt16)index));
  entity->setXData(xd);
}

OdInt32 dimLineIndex(OdDbEntityPtr& entity)
{
  OdResBufPtr xd = entity->xData(OD_T("Index"));
  if ( xd.isNull() )
    return -1;
  return xd->next()->getInt16();
}

void removeDimLineIndex(OdDbEntityPtr& entity)
{
  OdResBufPtr xd = entity->xData(OD_T("Index"));
  if ( xd.isNull() )
    return;
  xd = OdResBuf::newRb(OdResBuf::kDxfRegAppName);
  xd->setString(OD_T("Index"));
  entity->setXData(xd);
}

