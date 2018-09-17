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
#include "ExDgnElementDumperPE.h"

#include "DgLevelTableRecord.h"
#include "DgFontTableRecord.h"
#include "DgTextStyleTableRecord.h"
#include "DgDimStyleTableRecord.h"
#include "DgMultilineStyleTableRecord.h"
#include "DgLineStyleTableRecord.h"
#include "DgLineStyleDefTableRecord.h"
#include "DgDictionaryTableRecord.h"
#include "DgRegAppTableRecord.h"
#include "DgAttributeLinkage.h"
#include "DgApplicationData.h"
#include "DgLevelMask.h"
#include "DgXAttribute.h"
#include "DgProxyElement.h"
#include "DgMaterialTableRecord.h"
#include "DgLight.h"
#include "DgMesh.h"
#include "DgRasterFrame.h"
#include "DgOle2Frame.h"
#include "DgNamedGroup.h"
#include "DgACS.h"
#include "DgSectionClip.h"
#include "DgEnvironmentMap.h"

//----------------------------------------------------------
//
// OdDgRxObjectDumperPE
//
//----------------------------------------------------------
ODRX_NO_CONS_DEFINE_MEMBERS( OdDgRxObjectDumperPE, OdRxObject )

OdDgRxObjectDumperPE::OdDgRxObjectDumperPE()
{
  m_references = 0;
}

OdDgRxObjectDumperPE::~OdDgRxObjectDumperPE()
{
}

void OdDgRxObjectDumperPE::dump( OdRxObjectPtr /*pObj*/, OdExDgnDumper* /*pDumper*/ ) const
{
}

//----------------------------------------------------------
//
// OdDgElementDumperPE
//
//----------------------------------------------------------
ODRX_NO_CONS_DEFINE_MEMBERS( OdDgElementDumperPE, OdDgRxObjectDumperPE )

OdDgElementDumperPE::OdDgElementDumperPE()
{
}

OdDgElementDumperPE::~OdDgElementDumperPE()
{
}

OdDgElementIteratorPtr OdDgElementDumperPE::createIterator( OdDgElementPtr pElm, bool /*atBeginning = true*/, bool /*skipDeleted = true*/ ) const
{
  return OdDgElementIteratorPtr();
}

void OdDgElementDumperPE::dump( OdRxObjectPtr pObj, OdExDgnDumper* pDumper ) const
{
  dumpData(pObj, pDumper);
  dumpLinkages(pObj, pDumper);
  dumpXAttributes(pObj, pDumper);
  dumpReactors(pObj, pDumper);
}

void OdDgElementDumperPE::dumpData( OdRxObjectPtr pObj, OdExDgnDumper* pDumper ) const
{
  OdDgElementPtr pElm = pObj;
  pDumper->writeFieldValue( "ElementId", pElm->elementId().getHandle().ascii() );
  pDumper->writeFieldValue( "OwnerId", pElm->ownerId().getHandle().ascii() );
}

void OdDgElementDumperPE::dumpReactors( OdRxObjectPtr pObj, OdExDgnDumper* pDumper ) const
{
  OdDgElementPtr element = OdDgElement::cast( pObj );
  if( element.isNull() )
  {
    return;
  }  
  OdDgElementPtr pElm = pObj;

  OdDgElementIdArray arrPersReactors;
  OdDgElementReactorArray arrTransReactors;
  pElm->getPersistentReactors( arrPersReactors );
  pElm->getTransientReactors( arrTransReactors );

  pDumper->writeFieldValue("NumPersistentReactors", (OdUInt32)(arrPersReactors.size()) );
  pDumper->writeFieldValue("NumTransientReactors", (OdUInt32)(arrTransReactors.size()) );

  for( OdUInt32 i = 0; i < arrTransReactors.size(); i++ )
  {
    OdDgTagAssociationReactor* pTagReactor = dynamic_cast<OdDgTagAssociationReactor*>(arrTransReactors[i].get());

    if( pTagReactor )
    {
      pDumper->dumpFieldName( " - Tag Association Reactor ");
      pDumper->writeFieldValueHex("     Tag Id", (OdUInt64)(pTagReactor->getTagElementId().getHandle()) );
    }
  }
}

void OdDgElementDumperPE::dumpLinkages( OdRxObjectPtr pObj, OdExDgnDumper* pDumper ) const
{
  OdDgElementPtr pElm = pObj;

  OdRxObjectPtrArray linkages;
  pElm->getLinkages(linkages);
  pDumper->writeFieldValue( "Linkages Count", (OdUInt32)linkages.size() );
  if ( linkages.size() > 0 )
  {
    for (unsigned i = 0; i < linkages.size(); ++i)
    {
      OdDgAttributeLinkagePtr pLinkage = linkages[i];

      OdDgAttributeLinkageHeader header;
      OdBinaryData data;
      pLinkage->getData(data);

#if (TD_VERSION > 30999)
      header = pLinkage->getLinkageHeader(false);
#else
      OdDgAttributeLinkageExt extLinkage( pLinkage );
      header = extLinkage.getLinkageHeader(false);
#endif

      OdUInt16 primaryId = pLinkage->getPrimaryId();
      pDumper->writeFieldValueHex( "Linkage", primaryId );
      pDumper->writeFieldValue( "  Class name", pLinkage->isA()->name() );
      pDumper->writeFieldValue( "  Raw Data Size", (OdUInt32)data.size() );
      pDumper->writeFieldValue( "  Modified Linkage Flag", header.getModifiedFlag() );
      pDumper->writeFieldValue( "  Remote Linkage Flag", header.getRemoteFlag() );
      pDumper->writeFieldValue( "  User Linkage Flag", header.getUserDataFlag() );
      pDumper->writeFieldValue( "  Info Linkage Flag", header.getInfoLinkageFlag() );

      switch ( primaryId )
      {
        case OdDgAttributeLinkage::kLStyleMod:
        {
          OdDgLineStyleModificationLinkagePtr pLStyleModLinkage = pLinkage;

          if( pLStyleModLinkage->getUseLineStyleScaleFlag() )
          {
            pDumper->writeFieldValue( "  Line Style Scale", pLStyleModLinkage->getLineStyleScale() );
          }

          if( pLStyleModLinkage->getUseLineStyleDashScaleFlag() )
          {
            pDumper->writeFieldValue( "  Line Style Dash Scale", pLStyleModLinkage->getLineStyleDashScale() );
          }

          if( pLStyleModLinkage->getUseLineStyleGapScaleFlag() )
          {
            pDumper->writeFieldValue( "  Line Style Gap Scale", pLStyleModLinkage->getLineStyleGapScale() );
          }

          pDumper->writeFieldValue( "  Width Mode", pLStyleModLinkage->getWidthMode() );

          if( pLStyleModLinkage->getWidthMode() == OdDgLineStyleModificationLinkage::kLsModConstantWidth )
          {
            pDumper->writeFieldValue( "  Width", pLStyleModLinkage->getLineStyleWidth() );
            pDumper->writeFieldValue( "  True Width Flag", pLStyleModLinkage->getUseLineStyleTrueWidthFlag() );
          }
          else if( pLStyleModLinkage->getWidthMode() == OdDgLineStyleModificationLinkage::kLsModTaperedWidth )
          {
            pDumper->writeFieldValue( "  Start Width", pLStyleModLinkage->getLineStyleWidth() );
            pDumper->writeFieldValue( "  End Width", pLStyleModLinkage->getLineStyleEndWidth() );
            pDumper->writeFieldValue( "  True Width Flag", pLStyleModLinkage->getUseLineStyleTrueWidthFlag() );
          }

          pDumper->writeFieldValue( "  Shift Mode", pLStyleModLinkage->getShiftMode() );

          if( pLStyleModLinkage->getShiftMode() == OdDgLineStyleModificationLinkage::kLsModDistance )
          {
            pDumper->writeFieldValue( "  Shift Distance", pLStyleModLinkage->getLineStyleShift() );
          }
          else if( pLStyleModLinkage->getShiftMode() == OdDgLineStyleModificationLinkage::kLsModFraction )
          {
            pDumper->writeFieldValue( "  Fraction Phase", pLStyleModLinkage->getLineStyleFractionPhase() );
          }

          pDumper->writeFieldValue( "  Corner Mode", pLStyleModLinkage->getCornerMode() );

          if( pLStyleModLinkage->getUseLineStyleMultilineDataFlag() )
          {
            pDumper->writeFieldValue( "  Multiline Data", pLStyleModLinkage->getMultilineDataType() );

            if( pLStyleModLinkage->getMultilineDataType() == OdDgLineStyleModificationLinkage::kLsMultilineTypeLine )
            {
              OdUInt32Array indexArr;
              pLStyleModLinkage->getMultilineProfileIndexes( indexArr );

              if( indexArr.size() )
              {
                pDumper->writeFieldValue( "  Multiline Profile Index", indexArr[0] );
              }
            }
          }
        } break;

        case OdDgAttributeLinkage::kFillStyle:
        {
          OdDgDisplayStyleLinkagePtr pDisplayStyleLinkage = dynamic_cast<OdDgDisplayStyleLinkage*>(pLinkage.get());

          if( !pDisplayStyleLinkage.isNull() )
          {
            pDumper->writeFieldValue( "  Display Style Entry Id", pDisplayStyleLinkage->getDisplayStyleEntryId() );
            break;
          }

          OdDgFillColorLinkagePtr pFillColor = dynamic_cast<OdDgFillColorLinkage*>(pLinkage.get());

          if( !pFillColor.isNull() )
          {
            pDumper->writeFieldValue( "  Fill color", pFillColor->getColorIndex() );
            break;
          }

          OdDgTransparencyLinkagePtr pTransparency = dynamic_cast<OdDgTransparencyLinkage*>(pLinkage.get());

          if( !pTransparency.isNull() )
          {
            pDumper->writeFieldValue( "  Transparency", pTransparency->getTransparency() );
            break;
          }

          OdDgInternalMaterialLinkagePtr pMaterial = dynamic_cast<OdDgInternalMaterialLinkage*>(pLinkage.get());

          if( !pMaterial.isNull() )
          {
            pDumper->writeFieldValue( "  Linkage Type", pMaterial->getMaterialType() );
            pDumper->writeFieldValueHex( "  Material Id", pMaterial->getMaterialTableRecordId() );
            break;
          }

          OdDgGradientFillLinkagePtr pGradient = dynamic_cast<OdDgGradientFillLinkage*>(pLinkage.get());

          if( !pGradient.isNull() )
          {
            pDumper->writeFieldValue( "  Gradient Type", pGradient->getGradientType() );
            pDumper->writeFieldValue( "  Gradient Angle", pGradient->getAngle() );
            pDumper->writeFieldValue( "  White Intensity", pGradient->getWhiteIntensity() );
            pDumper->writeFieldValue( "  Invert Flag", pGradient->getInvertFlag() );
            pDumper->writeFieldValue( "  Key Count", pGradient->getKeyCount() );

            for( OdUInt32 k = 0; k < pGradient->getKeyCount(); k++ )
            {
              OdDgGradientFill::OdDgGradientKey key = pGradient->getKey(k);
              pDumper->writeFieldValue( "    Key Number", k );
              pDumper->writeFieldValue( "    Key Position", key.dKeyPosition );
              pDumper->writeFieldValueHex( "    Key Color", key.clrKeyColor );
            }

            break;
          }

        } break;
        case OdDgAttributeLinkage::kBREP:
        {
          OdDgBrepLinkagePtr pBrepLinkage = pLinkage;
          pDumper->writeFieldValue( "  Brep Flag1", (OdUInt16)pBrepLinkage->getFlag1() );
          pDumper->writeFieldValue( "  Brep Flag2", (OdUInt16)pBrepLinkage->getFlag2() );
          pDumper->writeFieldValue( "  Brep Scale", pBrepLinkage->getScale() );
        }
        break;
        case OdDgAttributeLinkage::kDependency:
        {
          OdDgDependencyLinkagePtr pDepLinkage = pLinkage;
          pDumper->writeFieldValue( "  Dependency AppId", (OdUInt16)pDepLinkage->getAppId() );
          pDumper->writeFieldValue( "  Dependency AppValue", (OdUInt16)pDepLinkage->getAppValue() );
          pDumper->writeFieldValue( "  Dependency RootDataType", pDepLinkage->getRootDataType() );

          switch( pDepLinkage->getRootDataType() )
          {
            case OdDgDependencyLinkage::kElementId:
            {
              OdDgDepLinkageElementIdPtr pDepLinkageElmId = pDepLinkage;
              unsigned idCount = pDepLinkageElmId->getCount();
              for (OdUInt32 i = 0; i < idCount; ++i)
              {
                pDumper->writeFieldValue( "  Root Number", i);
                pDumper->writeFieldValueHex( "    Dependency ElementId", (OdUInt64)pDepLinkageElmId->getAt(i) );
              }
            } break;

            case OdDgDependencyLinkage::kElementId_V:
            {
              OdDgDepLinkageElementIdVPtr pDepLinkageElmId = pDepLinkage;
              unsigned idCount = pDepLinkageElmId->getCount();

              for (OdUInt32 i = 0; i < idCount; ++i)
              {
                OdDgElementIdVData rootData = pDepLinkageElmId->getAt(i);

                pDumper->writeFieldValue(    "  Root Number", i);
                pDumper->writeFieldValueHex( "    Dependency ElementId", rootData.m_uId );
                pDumper->writeFieldValue(    "    Value", rootData.m_dValue);
              }
            } break;

            case OdDgDependencyLinkage::kAssocPoint:
            {
              OdDgDepLinkageAssocPointPtr pDepLinkageAssocPt = pDepLinkage;
              unsigned idCount = pDepLinkageAssocPt->getCount();

              for (OdUInt32 i = 0; i < idCount; ++i)
              {
                OdDgAssocPointRootPtr pRootData = pDepLinkageAssocPt->getAt(i);

                pDumper->writeFieldValue( "  Root Number", i);

                OdDgRxObjectDumperPEPtr pAssocRootDumper = OdDgRxObjectDumperPE::cast( pRootData );

                if ( pAssocRootDumper.get() )
                {
                  pAssocRootDumper->dump( pRootData.get(), pDumper );
                }
              }
            } break;

            case OdDgDependencyLinkage::kAssocPoint_I:
            {
              OdDgDepLinkageAssocPointIPtr pDepLinkageAssocPt = pDepLinkage;
              unsigned idCount = pDepLinkageAssocPt->getCount();

              for (OdUInt32 i = 0; i < idCount; ++i)
              {
                OdDgAssocPointIData rootData = pDepLinkageAssocPt->getAt(i);

                pDumper->writeFieldValue( "  Root Number", i);

                OdDgRxObjectDumperPEPtr pAssocRootDumper = OdDgRxObjectDumperPE::cast( rootData.m_assocPointData );

                if ( pAssocRootDumper.get() )
                {
                  pAssocRootDumper->dump( rootData.m_assocPointData.get(), pDumper );
                }

                pDumper->writeFieldValue("    Int 1", rootData.m_iParam1 );
                pDumper->writeFieldValue("    Int 2", rootData.m_iParam2 );
              }
            } break;

            case OdDgDependencyLinkage::kFarElementId:
            {
              OdDgDepLinkageFarElementIdPtr pDepLinkageElmId = pDepLinkage;
              unsigned idCount = pDepLinkageElmId->getCount();

              for (OdUInt32 i = 0; i < idCount; ++i)
              {
                OdDgFarElementIdData rootData = pDepLinkageElmId->getAt(i);

                pDumper->writeFieldValue(    "  Root Number", i);
                pDumper->writeFieldValueHex( "    Dependency ElementId", rootData.m_elementId );
                pDumper->writeFieldValueHex( "    XRef ElementId", rootData.m_referenceAttachId );
              }
            } break;

            case OdDgDependencyLinkage::kFarElementId_V:
            {
              OdDgDepLinkageFarElementIdVPtr pDepLinkageElmId = pDepLinkage;
              unsigned idCount = pDepLinkageElmId->getCount();

              for (OdUInt32 i = 0; i < idCount; ++i)
              {
                OdDgFarElementIdVData rootData = pDepLinkageElmId->getAt(i);

                pDumper->writeFieldValue(    "  Root Number", i);
                pDumper->writeFieldValueHex( "    Dependency ElementId", rootData.m_elementId );
                pDumper->writeFieldValueHex( "    XRef ElementId", rootData.m_referenceAttachId );
                pDumper->writeFieldValue(    "    Double Value", rootData.m_dParam );
              }
            } break;

            case OdDgDependencyLinkage::kPath_V:
            {
              OdDgDepLinkagePathPtr pDepLinkagePath = pDepLinkage;
              unsigned idCount = pDepLinkagePath->getCount();

              for (OdUInt32 i = 0; i < idCount; ++i)
              {
                OdDgDependencyPathData rootData = pDepLinkagePath->getAt(i);

                pDumper->writeFieldValue(    "  Root Number", i);
                pDumper->writeFieldValue(    "    Double Value", rootData.m_dParam );

                for( OdUInt32 l = 0; l < rootData.m_referenceAttachPath.size(); l++ )
                {
                  OdString strFieldName;
                  strFieldName.format(L"    Path item %d", l);
                  pDumper->writeFieldValueHex(    strFieldName, (OdUInt64) rootData.m_referenceAttachPath[l] );
                }
              }
            } break;
          }
        }
        break;
        case OdDgAttributeLinkage::kThickness:
        {
          OdDgThicknessLinkagePtr pThicknessLinkage = pLinkage;

          double dThickness = pThicknessLinkage->getThickness();

          OdDgDatabase* pDb = pElm->database();
          OdDgElementId idModel;

          if( pDb )
            idModel = pDb->getActiveModelId();

          if( !idModel.isNull() )
          {
            OdDgModelPtr pModel = OdDgModel::cast( idModel.openObject() );

            if( !pModel.isNull() )
            {
              dThickness = pModel->convertUORsToWorkingUnits( dThickness );
            }
          }
          else
          {
            dThickness /= DDLL(10000000000); // Storage units default factor
          }

          pDumper->writeFieldValue( "  Thickness", dThickness );
        }
        break;
        case OdDgAttributeLinkage::kInfiniteLine:
        {
          OdDgInfiniteLineLinkagePtr pFilterLinkage = pLinkage;
          pDumper->writeFieldValue( "  Infinite Line type", pFilterLinkage->getInfiniteLineType() );
        }
        break;
        case OdDgAttributeLinkage::kTextAnnotation:
        {
          OdDgTextAnnotationLinkagePtr pAnnotLinkage = pLinkage;
          pDumper->writeFieldValue( "  Annotation Scale", pAnnotLinkage->getAnnotationScale() );
        }
        break;
        case OdDgAttributeLinkage::kFilterMember:
        {
          OdDgFilterMemberLinkagePtr pFilterLinkage = pLinkage;
          pDumper->writeFieldValue( "  Member Id", pFilterLinkage->getMemberId() );
          pDumper->writeFieldValue( "  Member Type", pFilterLinkage->getMemberType() );
          pDumper->writeFieldValue( "  Name String", pFilterLinkage->getNameString() );
          pDumper->writeFieldValue( "  Expression String", pFilterLinkage->getExpressionString() );
        }
        break;
        case OdDgAttributeLinkage::kString:
        {
          OdDgStringLinkagePtr pStringLinkage = pLinkage;
          pDumper->writeFieldValue( "  String Id", pStringLinkage->getStringId() );
          pDumper->writeFieldValue( "  String Data", pStringLinkage->getString() );
        }
        break;
        case OdDgAttributeLinkage::kByteArray:
        {
          OdDgByteArrayLinkagePtr pByteArrayLinkage = pLinkage;
          pDumper->writeFieldValue( "  ByteArray Id", pByteArrayLinkage->getArrayId() );
          OdBinaryData data;
          pByteArrayLinkage->getArrayData(data);
          pDumper->writeFieldValue( "  ByteArray Data Size", (OdUInt32)data.size() );
        }
        break;
        case OdDgAttributeLinkage::kDoubleArray:
        {
          OdDgDoubleArrayLinkagePtr pDoubleArrayLinkage = pLinkage;
          pDumper->writeFieldValue( "  DoubleArray Id", pDoubleArrayLinkage->getArrayId() );

          for( OdUInt32 n = 0; n < pDoubleArrayLinkage->getItemCount(); n++ )
          {
            OdString strFieldName;
            strFieldName.format(L"  Double [%d]", n);
            pDumper->writeFieldValue( strFieldName,  pDoubleArrayLinkage->getItem(n));
          }
        }
        break;
        case OdDgAttributeLinkage::kHatch:
        {
          OdDgPatternLinkagePtr pPatternLinkage = pLinkage;
          pDumper->writeFieldValue( "  Pattern Type", (OdUInt32)pPatternLinkage->getType() );
          if( pPatternLinkage->getUseOffsetFlag() )
          {
            OdGePoint3d offset;
            pPatternLinkage->getOffset( offset );
            pDumper->writeFieldValue( "  offset", offset );
          }

          if( pPatternLinkage->getType() == OdDgPatternLinkage::kDWGPattern )
          {
            OdDgDWGPatternLinkagePtr pDWGPattern = pPatternLinkage;

            pDumper->dumpFieldName(L"  DWG hatch:");

            const OdDgDWGPatternLinkage::DWGHatch& hatch = pDWGPattern->getHatch();

            double dScaleFactor = 1.0;

            OdDgDatabase* pDb = pElm->database();
            OdDgElementId idModel;

            if( pDb )
              idModel = pDb->getActiveModelId();

            if( !idModel.isNull() )
            {
              OdDgModelPtr pModel = OdDgModel::cast( idModel.openObject() );

              if( !pModel.isNull() )
              {
                dScaleFactor = pModel->convertUORsToWorkingUnits( 1.0 );
              }
            }

            for( OdUInt32 k = 0; k < hatch.size(); k++ )
            {
              const OdDgDWGPatternLinkage::DWGHatchLine& hatchLine = hatch[k];

              OdGePoint2d ptOffset  = hatchLine.m_offset;
              OdGePoint2d ptThrough = hatchLine.m_throughPoint;

              pDumper->writeFieldValue(L"    Hatch line Number", k );
              pDumper->writeFieldValue(L"    Hatch line Angle", hatchLine.m_angle );
              pDumper->writeFieldValue(L"    Hatch line Offset", ptOffset * dScaleFactor );
              pDumper->writeFieldValue(L"    Hatch line Through pt", ptThrough * dScaleFactor );

              if(  hatchLine.m_dashes.size() )
              {
                pDumper->dumpFieldName(L"    Hatch line dashes:");
              }

              for( OdUInt32 n = 0 ; n < hatchLine.m_dashes.size(); n++ )
              {
                OdString strFieldName;
                strFieldName.format(L"      Dash %d", n );
                pDumper->writeFieldValue(strFieldName, hatchLine.m_dashes[n] * dScaleFactor );
              }
            }
          }
          else if( pPatternLinkage->getType() == OdDgPatternLinkage::kSymbolPattern )
          {
            OdDgSymbolPatternLinkagePtr pSymPattern = pPatternLinkage;

            pDumper->writeFieldValue( L"  Angle1", pSymPattern->getAngle1() );
            pDumper->writeFieldValue( L"  Angle2", pSymPattern->getAngle2() );
            pDumper->writeFieldValue( L"  Space1", pSymPattern->getSpace1() );
            pDumper->writeFieldValue( L"  Space2", pSymPattern->getSpace2() );
            pDumper->writeFieldValue( L"  Scale", pSymPattern->getScale() );
          }
        }
        break;
        case OdDgAttributeLinkage::kDimension:
        {
          OdDgDimensionLinkagePtr pDimLinkage = pLinkage;

          if( !pDimLinkage.isNull() && pDimLinkage->getType() == OdDgDimensionLinkage::kRefValue )
          {
            OdDgDimensionRefValueLinkagePtr pDimRefValueLinkage = pLinkage;
            pDumper->writeFieldValue( "  Referenced Value(In UORs)", pDimRefValueLinkage->getRefValue() );
          }
          else if( !pDimLinkage.isNull() && pDimLinkage->getType() == OdDgDimensionLinkage::kDimensionInfo )
          {
            OdDgDimensionInfoLinkagePtr pDimInfoLinkage = pLinkage;

            if( pDimInfoLinkage->getUseAnnotationScale() )
              pDumper->writeFieldValue( "  Annotation Scale", pDimInfoLinkage->getAnnotationScale() );

            if( pDimInfoLinkage->getUseDatumValue() )
            {
              double dDatumValue = pDimInfoLinkage->getDatumValue();

              OdDgDatabase* pDb = pElm->database();
              OdDgElementId idModel;

              if( pDb )
                idModel = pDb->getActiveModelId();

              if( !idModel.isNull() )
              {
                OdDgModelPtr pModel = OdDgModel::cast( idModel.openObject() );

                if( !pModel.isNull() )
                {
                  dDatumValue = pModel->convertUORsToWorkingUnits( dDatumValue );
                }
              }
              else
              {
                dDatumValue /= DDLL(10000000000); // Storage units default factor
              }

              pDumper->writeFieldValue( "  Datum Value", dDatumValue );
            }

            if( pDimInfoLinkage->getUseRetainFractionalAccuracy() )
            {
              pDumper->writeFieldValue( "  Detriment in reverse direction", pDimInfoLinkage->getUseDecrimentInReverceDirection() );
              pDumper->writeFieldValue( "  Primary retain fractional accuracy", pDimInfoLinkage->getPrimaryRetainFractionalAccuracy() );
              pDumper->writeFieldValue( "  Secondary retain fractional accuracy", pDimInfoLinkage->getSecondaryRetainFractionalAccuracy() );
              pDumper->writeFieldValue( "  Primary alt format retain fractional accuracy", pDimInfoLinkage->getPrimaryAltFormatRetainFractionalAccuracy() );
              pDumper->writeFieldValue( "  Secondary alt format retain fractional accuracy", pDimInfoLinkage->getSecondaryAltFormatRetainFractionalAccuracy() );
              pDumper->writeFieldValue( "  Primary tolerance retain fractional accuracy", pDimInfoLinkage->getPrimaryTolerRetainFractionalAccuracy() );
              pDumper->writeFieldValue( "  Secondary tolerance retain fractional accuracy", pDimInfoLinkage->getSecondaryTolerRetainFractionalAccuracy() );
              pDumper->writeFieldValue( "  Label line mode", pDimInfoLinkage->getLabelLineDimensionMode());
            }

            if( pDimInfoLinkage->getUseFitOptionsFlag() )
            {
              pDumper->writeFieldValue( "  Suppress unfit terminators", pDimInfoLinkage->getUseSuppressUnfitTerm() );
              pDumper->writeFieldValue( "  Use inline leader length", pDimInfoLinkage->getUseInlineLeaderLength() );
              pDumper->writeFieldValue( "  Text above optimal fit", pDimInfoLinkage->getUseTextAboveOptimalFit() );
              pDumper->writeFieldValue( "  Narrow font optimal fit", pDimInfoLinkage->getUseNarrowFontOptimalFit() );
              pDumper->writeFieldValue( "  Use Min Leader Terminator Length", pDimInfoLinkage->getUseMinLeaderTermLength() );
              pDumper->writeFieldValue( "  Use auto mode for dimension leader", pDimInfoLinkage->getUseAutoLeaderMode() );
              pDumper->writeFieldValue( "  Fit Options ", pDimInfoLinkage->getFitOptions() );
            }

            if( pDimInfoLinkage->getUseTextLocation() )
            {
              pDumper->writeFieldValue( "  Free location of text", pDimInfoLinkage->getUseFreeLocationText() ); 
              pDumper->writeFieldValue( "  Note spline fit", pDimInfoLinkage->getUseNoteSplineFit() );
              pDumper->writeFieldValue( "  Text location ", pDimInfoLinkage->getTextLocation() );
            }

            if( pDimInfoLinkage->getUseInlineLeaderLengthValue() )
            {
              pDumper->writeFieldValue( "  Inline leader length value", pDimInfoLinkage->getInlineLeaderLength() );
            }

            if( pDimInfoLinkage->getUseInlineTextLift() )
              pDumper->writeFieldValue( "  Inline text lift", pDimInfoLinkage->getInlineTextLift() );

            if( pDimInfoLinkage->getUseNoteFrameScale() )
              pDumper->writeFieldValue( "  Note frame scale", pDimInfoLinkage->getUseNoteFrameScale() );

            if( pDimInfoLinkage->getUseNoteLeaderLength() )
              pDumper->writeFieldValue( "  Note leader length", pDimInfoLinkage->getNoteLeaderLength() );

            if( pDimInfoLinkage->getUseNoteLeftMargin() )
              pDumper->writeFieldValue( "  Note left margin", pDimInfoLinkage->getUseNoteLeftMargin() );

            if( pDimInfoLinkage->getUseNoteLowerMargin() )
              pDumper->writeFieldValue( "  Note lower margin", pDimInfoLinkage->getUseNoteLowerMargin() );

            if( pDimInfoLinkage->getUsePrimaryToleranceAccuracy() )
              pDumper->writeFieldValue( "  Primary tolerance accuracy", pDimInfoLinkage->getPrimaryToleranceAccuracy() );

            if( pDimInfoLinkage->getUseSecondaryToleranceAccuracy() )
              pDumper->writeFieldValue( "  Secondary tolerance accuracy", pDimInfoLinkage->getSecondaryToleranceAccuracy() );

            if( pDimInfoLinkage->getUseStackedFractionScale() )
              pDumper->writeFieldValue( "  Stacked fraction scale", pDimInfoLinkage->getStackedFractionScale() );
          }
        } break;

        case OdDgAttributeLinkage::kReferenceAttachmentPath:
        {
          OdDgReferenceAttachPathLinkagePtr pPathLink = linkages[i];

          if( !pPathLink.isNull() )
          {
            for( OdUInt32 l = 0; l < pPathLink->getPathLength(); l++ )
            {
              OdString strFieldName;
              strFieldName.format(L"  Path item %d", l);
              pDumper->writeFieldValueHex( strFieldName, pPathLink->getPathItem(l) );
            }
          }

        } break;

        case OdDgAttributeLinkage::kOLE:
        {
          OdDgOleLinkagePtr pOleLinkage = linkages[i];

          pDumper->writeFieldValue("  Draw Aspect",pOleLinkage->getDrawAspect() );
          pDumper->writeFieldValue("  Ole Type",pOleLinkage->getOleType() );
          pDumper->writeFieldValue("  Size X ( in cm )",pOleLinkage->getXSize() / 1000.0 );
          pDumper->writeFieldValue("  Size Y ( in cm )",pOleLinkage->getYSize() / 1000.0 );
          pDumper->writeFieldValue("  Aspect Ratio Flag",pOleLinkage->getAspectRatioFlag() );
          pDumper->writeFieldValue("  Transparent Background Flag",pOleLinkage->getTransparentBackgroundFlag() );
          pDumper->writeFieldValue("  Rotate With View Flag",pOleLinkage->getRotateWithViewFlag() );
          pDumper->writeFieldValue("  View Projection Mode Flag",pOleLinkage->getViewProjectionModeFlag() );
          pDumper->writeFieldValue("  Can Be Picture Flag",pOleLinkage->getCanBePictureFlag() );
          pDumper->writeFieldValue("  Can Be Linked and Embedded Flag",pOleLinkage->getCanBeLinkedAndEmbeddedFlag() );

        } break;

        case OdDgAttributeLinkage::kSeedPoints:
        {
          OdDgSeedPointsLinkagePtr pSeedPoints = linkages[i];

          for( OdUInt32 iPt = 0; iPt < pSeedPoints->getPointsCount(); iPt++ )
          {
            OdString strName;
            strName.format( L"Point %d", iPt );
            pDumper->writeFieldValue( strName, pSeedPoints->getPointAt(iPt));
          }
        } break;

        case OdDgAttributeLinkage::kBitMaskLinkage:
        {
          OdDgTextWhiteSpaceLinkagePtr pTextBitMaskLinkage = dynamic_cast<OdDgTextWhiteSpaceLinkage*>(pLinkage.get());

          if( !pTextBitMaskLinkage.isNull() )
          {
            pDumper->writeFieldValue("  Symbol Count", pTextBitMaskLinkage->getSymbolCount() );

            for( OdUInt32 iSymbol = 0; iSymbol < pTextBitMaskLinkage->getSymbolCount(); iSymbol++ )
            {
              OdString strSymbolName;
              strSymbolName.format( L"    Symbol %d", iSymbol);
              pDumper->writeFieldValue(strSymbolName, pTextBitMaskLinkage->getSymbol( iSymbol ) );
            }

            pDumper->writeFieldValue("  Symbol String", pTextBitMaskLinkage->createString() );
          }

          OdDgBitMaskLinkagePtr pBitMaskLinkage = dynamic_cast<OdDgBitMaskLinkage*>(pLinkage.get());

          if( !pBitMaskLinkage.isNull() )
          {
            pDumper->writeFieldValue("  BitMask Id", pBitMaskLinkage->getBitMaskId() );
            pDumper->writeFieldValue("  Bit Count", pBitMaskLinkage->getBitCount() );

            OdBinaryData binData;

            binData = pBitMaskLinkage->getBitMaskData();

            OdUInt16* pBinData = (OdUInt16*)(binData.asArrayPtr());

            OdString strData;

            for( OdUInt32 iWord = 0; iWord < binData.size()/2; iWord++)
            {
              OdString strWord;

              strWord.format(L"0x%04x,", pBinData[iWord] );

              strData += strWord;
            }

            if( strData.getLength() > 0 )
            {
              strData = strData.left(strData.getLength() - 1);
            }

            pDumper->writeFieldValue("  Binary Data", strData );
          }
        } break;

        case OdDgAttributeLinkage::kTextIndentation:
        {
           OdDgTextIndentationLinkagePtr pTextLinkage = linkages[i];

           pDumper->writeFieldValue("  First Line Indentation", pTextLinkage->getFirstLineIndentation() );
           pDumper->writeFieldValue("  Paragraph Indentation", pTextLinkage->getParagraphIndentation() );
           pDumper->writeFieldValue("  Number of Tabs", pTextLinkage->getTabCount() );

           for( OdUInt32 iTab = 0; iTab < pTextLinkage->getTabCount(); iTab++ )
           {
             pDumper->writeFieldValue("    Tab Value", pTextLinkage->getTabValue( iTab ) );
           }
        } break;

        case OdDgAttributeLinkage::kDimTextStyle:
        {
          OdDgDimensionTextStyleDataLinkagePtr pTextStyleLinkage = linkages[i];

          OdDgTextStyleTableRecordPtr pTextStyle = pTextStyleLinkage->createTextStyle( pElm->database() );

          pDumper->writeFieldValue(L"---Text Style---", OdString(L"---------------------------"));

          OdSmartPtr<OdDgTextStyleTableRecordDumperPE> pTextStyleDumperPE = OdDgTextStyleTableRecordDumperPE::cast( pTextStyle );

          if( !pTextStyleDumperPE.isNull() )
          {
            pTextStyleDumperPE->dump(pTextStyle, pDumper);
          }

        } break;
      }

      OdDgProxyLinkagePtr pProxyLinkage = dynamic_cast<OdDgProxyLinkage*>(pLinkage.get());

      if( !pProxyLinkage.isNull() )
      {
        OdBinaryData binData;

        pProxyLinkage->getData( binData );

        dumpBinaryData( binData, pDumper );
      }
    }
  }
}

void OdDgElementDumperPE::dumpBinaryDataBytes( const OdBinaryData& binData, OdExDgnDumper* pDumper ) const 
{
  OdString strDataLine;
  OdString strData;
  OdUInt32 nData = 0;
  OdUInt32 uDataLine = 0;

  const OdUInt8* pByteData = binData.asArrayPtr();

  for( OdUInt32 l = 0; l < binData.length(); l++ )
  {
    OdString strByte;
    strByte.format(L"%02X, ", pByteData[l]);
    strData += strByte;
    nData++;

    if( nData == 16 )
    {
      strData = strData.left( strData.getLength() - 2 );
      strDataLine.format(L"  Line %03X", uDataLine*16 );
      uDataLine++;

      pDumper->writeFieldValue( strDataLine, strData );
      strData.empty();
      nData = 0;
    }
  }

  if( !strData.isEmpty() )
  {
    strData = strData.left( strData.getLength() - 2 );
    strDataLine.format(L"  Line %03X", uDataLine*16 );

    pDumper->writeFieldValue( strDataLine, strData );
  }
}

void OdDgElementDumperPE::dumpBinaryData( const OdBinaryData& binData, OdExDgnDumper* pDumper ) const 
{
  OdString strDataLine;
  OdString strData;
  OdUInt32 nData = 0;
  OdUInt32 uDataLine = 0;

  OdUInt16* pWordData = (OdUInt16*)(binData.asArrayPtr());

  for( OdUInt32 l = 0; l < binData.length()/2; l++ )
  {
    OdString strByte;

    if( pWordData[l] > 0x1f && pWordData[l] < 0x80 )
      strByte.format(L"\"%c\",", (char)(pWordData[l]) );
    else
      strByte.format(L"0x%04X, ", pWordData[l]);
    strData += strByte;
    nData++;

    if( nData == 4 )
    {
      strData = strData.left( strData.getLength() - 2 );
      strDataLine.format(L"  Line %d", uDataLine );
      uDataLine++;

      pDumper->writeFieldValue( strDataLine, strData );
      strData.empty();
      nData = 0;
    }
  }

  if( !strData.isEmpty() )
  {
    strData = strData.left( strData.getLength() - 2 );
    strDataLine.format(L"  Line %d", uDataLine );

    pDumper->writeFieldValue( strDataLine, strData );
  }
}

void OdDgElementDumperPE::dumpXAttributes( OdRxObjectPtr pObj, OdExDgnDumper* pDumper ) const
{
  OdDgElementPtr pElm = pObj;

  OdRxObjectPtrArray xattributes;
  pElm->getXAttributes(xattributes);
  pDumper->writeFieldValue( "XAttributes Count", (OdUInt32)xattributes.size() );
  if ( xattributes.size() > 0 )
  {
    for (unsigned i = 0; i < xattributes.size(); ++i)
    {
      OdDgXAttributePtr pXAttribute = xattributes[i];
      OdBinaryData data;
      pXAttribute->writeData(data);

      OdUInt32 handlerId = pXAttribute->getHandlerId();
      OdUInt32 attrId = pXAttribute->getXAttrId();
      pDumper->writeFieldValueHex( "XAttribute", handlerId );
      pDumper->writeFieldValue( "  Class name", pXAttribute->isA()->name() );
      pDumper->writeFieldValue( "  Attribute Id", attrId );

      OdDgDgnLinkNodeXAttributePtr pLink = OdDgDgnLinkNodeXAttribute::cast(pXAttribute);
      OdDgProcessedXmlXAttributePtr pExtXml = OdDgProcessedXmlXAttribute::cast(pXAttribute);
      OdDgXmlXAttributePtr pXml = OdDgXmlXAttribute::cast(pXAttribute);

      if( pLink.get() && !pLink->getLinkNode().isNull() )
      {
        pDumper->writeFieldValue( "  ", pLink->getLinkNode() );
        pDumper->writeFieldValue( "  ParentAttrId", pLink->getParentAttrId() );

        if( pLink->getChildCount() > 0 )
        {
          for( OdUInt32 uChild = 0; uChild < pLink->getChildCount(); uChild++ )
          {
            OdString strFildName;
            strFildName.format(L"     ChildAttrId %d", uChild );
            pDumper->writeFieldValue( strFildName, pLink->getChildAttrId(uChild) );
          }
        }
      }
      else if( !pExtXml.isNull() )
      {

        pDumper->writeFieldValue( L"  " + pExtXml->getRootNode().getName() + L":", pExtXml->getRootNode() );
      }
      else if( pXml.get() )
      {
        pDumper->writeFieldValue( "  Xml", pXml->getXmlString() );
      }

      OdDgGeometryCacheXAttributePtr pGeomCacheXAttr = OdDgGeometryCacheXAttribute::cast(pXAttribute);

      if( !pGeomCacheXAttr.isNull() )
      {
        pDumper->writeFieldValueHex( OD_T("  Type Value"), pGeomCacheXAttr->getTypeValue() );
        pDumper->dumpFieldName(OD_T("  Actions:"));

        for( OdUInt32 i = 0; i < pGeomCacheXAttr->getActionCount(); i++ )
        {
          pDumper->writeFieldValue(OD_T("    "), pGeomCacheXAttr->getAction(i));
        }
      }

      OdDgSetVariableAsPropertyXAttributePtr pVarSetXAttr = OdDgSetVariableAsPropertyXAttribute::cast(pXAttribute);

      if( !pVarSetXAttr.isNull() )
      {
        pDumper->writeFieldValueHex( OD_T("  Var Description Element Id"), pVarSetXAttr->getVariableDescriptionElementId() );
        pDumper->writeFieldValue( OD_T("  Identification String"), pVarSetXAttr->getIdentificationString() );
        pDumper->dumpFieldName(OD_T("  Property And Variable Pairs:") );

        OdUInt32 nPairs = pVarSetXAttr->getVariablePropertyPairCount();

        for( OdUInt32 l = 0; l < nPairs; l++ )
        {
          OdString strProp, strVar;
          pVarSetXAttr->getVariablePropertyPair(l, strVar, strProp );
          pDumper->writeFieldValue(OD_T("  - ")+strProp, strVar);
        }
      }

      OdDgConstraintsXAttributePtr pConsXAttr = OdDgConstraintsXAttribute::cast(pXAttribute);

      if( !pConsXAttr.isNull() )
      {
        OdDgConstraintsDataXAttrPtr pXAttrData = pConsXAttr->getXAttrData();

        if( !pXAttrData.isNull() )
        {
          pDumper->writeFieldValue(OD_T("  Constraints X-Attribute type:"), pXAttrData->getType() );

          switch( pXAttrData->getType() )
          {
            case OdDgConstraintsDataXAttr::k2dConstraints:
            {
              OdDg2dConstraintsDataXAttrPtr p2dConstraints = pXAttrData;

              if( p2dConstraints->isCorrupted() )
              {
                dumpBinaryDataBytes( p2dConstraints->getData(), pDumper );
              }
              else
              {
                for( OdUInt32 iConstraints = 0; iConstraints < p2dConstraints->getConstraintsCount(); iConstraints++ )
                {
                  OdDg2dConstraintRepresentationPtr p2dConstraint = p2dConstraints->getConstraint(iConstraints);
                  OdString strFieldName;
                  strFieldName.format(L"  Constraint %d", iConstraints );
                  pDumper->writeFieldValue( strFieldName, p2dConstraint->getType() );

                  for( OdUInt32 iPt = 0; iPt < p2dConstraint->getPointCount(); iPt++ )
                  {
                    strFieldName.format(L"   - Point %d", iPt );
                    pDumper->writeFieldValue( strFieldName, p2dConstraint->getPoint(iPt) );
                  }
                }
              }
            } break;
            case OdDgConstraintsDataXAttr::kDimConstraints:
            {
              OdDgDimConstraintsDataXAttrPtr pDimConstraints = pXAttrData;

              if( pDimConstraints->isCorrupted() )
              {
                dumpBinaryDataBytes( pDimConstraints->getData(), pDumper );
              }
              else
              {
                for( OdUInt32 iConstraints = 0; iConstraints < pDimConstraints->getConstraintsCount(); iConstraints++ )
                {
                  OdDgDimensionalConstraintRepresentationPtr pDimConstraint = pDimConstraints->getConstraint(iConstraints);
                  OdString strFieldName;
                  strFieldName.format(L"  Constraint %d", iConstraints );
                  pDumper->writeFieldValue( strFieldName, pDimConstraint->getType() );

                  for( OdUInt32 iPt = 0; iPt < pDimConstraint->getPointCount(); iPt++ )
                  {
                    strFieldName.format(L"   - Point %d", iPt );
                    pDumper->writeFieldValue( strFieldName, pDimConstraint->getPoint(iPt) );
                  }

                  pDumper->writeFieldValue( L"  - Value", pDimConstraint->getValue() );

                  if( !pDimConstraint->getVariableName().isEmpty() )
                    pDumper->writeFieldValue( L"  - Variable Name", pDimConstraint->getVariableName() );

                  if( !OdZero(pDimConstraint->getXOffsetValue()) )
                    pDumper->writeFieldValue( L"  - XOffset", pDimConstraint->getXOffsetValue() );

                  pDumper->writeFieldValue( L"  - OffsetY", pDimConstraint->getYOffsetValue() );

                  if( !OdZero(pDimConstraint->getParamValue()) )
                    pDumper->writeFieldValue( L"  - Param Value", pDimConstraint->getParamValue() );

                  pDumper->writeFieldValueHex( L"  - DirectionFlags", pDimConstraint->getDirectionFlags() );

                  if( pDimConstraint->getType() == OdDgConstraintRepresentation::kAngleConstraint )
                  {
                    pDumper->writeFieldValue( L"  - Angle Quadrant", pDimConstraint->getAngleQuadrant() );
                    pDumper->writeFieldValue( L"  - Clockwise Angle Flag", pDimConstraint->getClockwiseAngleFlag() );
                  }
                  else if( pDimConstraint->getType() == OdDgConstraintRepresentation::kDistanceConstraint )
                  {
                    pDumper->writeFieldValue( L"  - Segment 1 Status", pDimConstraint->getSegment1Status() );
                    pDumper->writeFieldValue( L"  - Segment 2 Status", pDimConstraint->getSegment2Status() );
                    pDumper->writeFieldValue( L"  - Alignment", pDimConstraint->getAlignment() );
                  }

                  if( pDimConstraint->getUnknownValue1() != 0 )
                    pDumper->writeFieldValue( L"  - Unknown1", pDimConstraint->getUnknownValue1() );

                  if( !OdZero(pDimConstraint->getUnknownValue2()) )
                    pDumper->writeFieldValue( L"  - Unknown2", pDimConstraint->getUnknownValue2() );
                }
              }
            } break;

            case OdDgConstraintsDataXAttr::kVersionConstraints:
            {
              OdDgVersionConstraintsDataXAttrPtr pVersion = pXAttrData;
              pDumper->writeFieldValue(OD_T("  Version:"), pVersion->getVersion() );
              pDumper->writeFieldValue(OD_T("  SubVersion:"), pVersion->getSubVersion() );
            } break;

            case OdDgConstraintsDataXAttr::kIdMapConstraints:
            {
              OdDgIdMapConstraintsDataXAttrPtr pIdMap = pXAttrData;

              for( OdUInt32 iItem = 0; iItem < pIdMap->getMapItemCount(); iItem++ )
              {
                OdUInt32 uItem = 0;
                OdUInt64 uId   = 0;
                pIdMap->getItem(iItem, uItem, uId);
                OdString strField;
                strField.format(OD_T("  %d"), uItem);
                pDumper->writeFieldValueHex( strField, uId );
              }
            } break;

            case OdDgConstraintsDataXAttr::kElmVisibilityConstraints:
            {
              OdDgElmVisibilityConstraintsDataXAttrPtr pFlags = pXAttrData;

              for( OdUInt32 iItem = 0; iItem < pFlags->getVisibilityCount(); iItem++ )
              {
                OdUInt32 uItem  = 0;
                bool     bFlags = 0;
                pFlags->getVisibility(iItem, uItem, bFlags);
                OdString strField;
                strField.format(OD_T("  %d"), uItem);

                if( bFlags )
                  pDumper->writeFieldValue( strField, OdString(L"is Visible") );
                else
                  pDumper->writeFieldValue( strField, OdString(L"is Invisible") );
              }
            } break;
          }
        }
      }

      OdDgSchemaDescriptionXAttributePtr pTableDescXAttr = OdDgSchemaDescriptionXAttribute::cast(pXAttribute);

      if( !pTableDescXAttr.isNull() )
      {
        pDumper->writeFieldValueHex(OD_T("  Schema Flags"), pTableDescXAttr->getDataFlags() );
        pDumper->writeFieldValue(OD_T("  Version"), pTableDescXAttr->getVersion() );
        pDumper->writeFieldValue(OD_T("  SubVersion"), pTableDescXAttr->getSubVersion() );

        OdDgSchemaDescriptionItemPtr pSchemaItem = pTableDescXAttr->getSchemaItem();

        if( pSchemaItem->getType() == OdDgSchemaDescriptionItem::kTableSchema )
        {
          OdDgTableSchemaDescriptionItem* pTableSchema = (OdDgTableSchemaDescriptionItem*)pSchemaItem.get();

          if( pTableSchema->getProxyDataFlag() )
          {
            OdBinaryData binData = pTableSchema->getProxyData();
            dumpBinaryData( binData, pDumper );
          }
          else if( pTableSchema->getSchemaType() == OdDgTableSchemaDescriptionItem::kSchemaFlags )
          {
            pDumper->writeFieldValue(OD_T("  Item Type"), OdString(OD_T("Schema Flags") ));
          }
          else
          {
            if( pTableSchema->getSchemaType() == OdDgTableSchemaDescriptionItem::kSchemaItem )
              pDumper->writeFieldValue(OD_T("  Item Type"), OdString(OD_T("Schema Item") ));
            else
              pDumper->writeFieldValue(OD_T("  Item Type"), OdString(OD_T("Item Group") ));

            pDumper->writeFieldValue(OD_T("  Item Name"), pTableSchema->getItemName() );
            pDumper->writeFieldValue(OD_T("  Item Data Type"), pTableSchema->getItemDataType() );
            pDumper->writeFieldValue(OD_T("  Item Data Offset"), pTableSchema->getItemDataOffset() );
            pDumper->writeFieldValueHex(OD_T("  Item Data Bitwise And"), (OdUInt64)(pTableSchema->getItemBitwiseAndData()) );
          }
        }
        else if( pSchemaItem->getType() == OdDgSchemaDescriptionItem::kCellParamSchema )
        {
          OdDgCellParamSchemaDescriptionItem* pParamSchema = (OdDgCellParamSchemaDescriptionItem*)pSchemaItem.get();
          pDumper->writeFieldValue(OD_T("  Schema Type"), OdString(L"kCellParam") );
          pDumper->writeFieldValue(OD_T("  Param Type"), pParamSchema->getTypeValue() );
          pDumper->writeFieldValue(OD_T("  Param Read Only"), pParamSchema->getReadOnlyFlag() );
          pDumper->writeFieldValue(OD_T("  Param Scope"), pParamSchema->getDefinitionScopeFlag() ? OdString(L"kDefinition") : OdString(L"kInstance") );
          pDumper->writeFieldValue(OD_T("  Param Visibility"), pParamSchema->getVisibleFlag() ? OdString(L"kVisible") : OdString(L"kHidden") );
          pDumper->writeFieldValue(OD_T("  Param Expression"), pParamSchema->getExpressionFlag() );
          pDumper->writeFieldValue(OD_T("  Param Name"), pParamSchema->getVariableName());
          pDumper->writeFieldValue(OD_T("  Param Value"), pParamSchema->getVariableValue());
          pDumper->writeFieldValue(OD_T("  Param Display Name"), pParamSchema->getVariableDisplayName());
          pDumper->writeFieldValue(OD_T("  Param Type Description"), pParamSchema->getVariableTypeDescription());
          pDumper->writeFieldValue(OD_T("  Param Units Description"), pParamSchema->getUnitsDescription());
        }
        else if( pSchemaItem->isKindOf(OdDgProxySchemaDescriptionItem::desc() ) )
        {
          OdDgProxySchemaDescriptionItem* pProxySchema = (OdDgProxySchemaDescriptionItem*)pSchemaItem.get();
          pDumper->writeFieldValue(OD_T("  Prototype Type"), pTableDescXAttr->getPrimaryIndex() );
          pDumper->writeFieldValue(OD_T("  Schema Index"), pTableDescXAttr->getSecondaryIndex() );

          OdBinaryData binData = pProxySchema->getProxyData();
          dumpBinaryDataBytes( binData, pDumper );
        }
      }

      OdDgTableElementSchemaXAttributePtr pTableXAttr = OdDgTableElementSchemaXAttribute::cast(pXAttribute);

      if( !pTableXAttr.isNull() )
      {
        pDumper->writeFieldValue(OD_T("  Table Type"), pTableXAttr->getTableType() );

        OdDgTableElementSchemaPtr pSchema = pTableXAttr->getSchema();

        if( !pSchema.isNull() )
        {
          pDumper->writeFieldValue(OD_T("  Version"), pSchema->getVersion() );
          pDumper->writeFieldValue(OD_T("  SubVersion"), pSchema->getSubVersion() );
        }

        if( pSchema->isKindOf( OdDgProxyTableElementSchema::desc() ) )
        {
          pDumper->writeFieldValue( OD_T("  Schema Type:"), pSchema->getType() );
          pDumper->dumpFieldName(OD_T("  Schema Data:"));
          OdBinaryData binData = pSchema->getData();
          dumpBinaryDataBytes( binData, pDumper );
        }
        else
        {
          switch( pSchema->getType() )
          {
            case OdDgTableElementSchema::kSnippedText:
            {
              OdDgTableSnippedTextSchemaPtr pSnippedText = pSchema;
              pDumper->writeFieldValue(OD_T("  Schema Type"), OdString(OD_T("kSnippedText")) );
              pDumper->writeFieldValue(OD_T("  Snipped Text Name"), pSnippedText->getName() );
              pDumper->writeFieldValue(OD_T("  Snipped Text Value"), pSnippedText->getText() );
              pDumper->writeFieldValueHex(OD_T("  Snipped Text Flags"), pSnippedText->getFlags() );
            } break;

            case OdDgTableElementSchema::kPCMCell:
            {
              OdDgPCMCellSchemaPtr pPCMCell = pSchema;
              pDumper->writeFieldValue(OD_T("  Schema Type"), OdString(OD_T("kPCMCell")) );
              pDumper->writeFieldValue(OD_T("  Rotation X"), pPCMCell->getXRotation() );
              pDumper->writeFieldValue(OD_T("  Rotation Y"), pPCMCell->getYRotation() );
              pDumper->writeFieldValue(OD_T("  Rotation Z"), pPCMCell->getZRotation() );
              pDumper->writeFieldValue(OD_T("  Origin"), pPCMCell->getOrigin() );
              pDumper->writeFieldValue(OD_T("  Scale"), pPCMCell->getScale() );
              pDumper->writeFieldValue(OD_T("  Variable Set Name"), pPCMCell->getVarSetName() );
            } break;

            case OdDgTableElementSchema::kPCMCellDef:
            {
              OdDgPCMCellDefSchemaPtr pPCMCellDef = pSchema;
              pDumper->writeFieldValue(OD_T("  Schema Type"), OdString(OD_T("kPCMCellDef")) );
              pDumper->writeFieldValue(OD_T("  Model Index"), pPCMCellDef->getModelIndex() );
              pDumper->writeFieldValue(OD_T("  Cell Name"), pPCMCellDef->getCellName() );
            } break;

            case OdDgTableElementSchema::kPrototypeDescription:
            {
              OdDgTablePrototypeDescriptionSchemaPtr pPrototypeDesc = pSchema;
              pDumper->writeFieldValue(OD_T("  Schema Type"), OdString(OD_T("kPrototypeDescription")) );
              pDumper->writeFieldValue(OD_T("  Prototype Id"), pPrototypeDesc->getPrototypeId() );
              pDumper->writeFieldValue(OD_T("  Prototype Name"), pPrototypeDesc->getPrototypeName() );
              pDumper->writeFieldValueHex(OD_T("  Prototype Flags"), pPrototypeDesc->getPrototypeFlags() );
            } break;

            case OdDgTableElementSchema::kSchemaPrototype:
            {
              OdDgTablePrototypeItemSchemaPtr pPrototypeItem = pSchema;
              pDumper->writeFieldValue(OD_T("  Schema Type"), OdString(OD_T("kPrototypeSchema")) );
              pDumper->writeFieldValueHex(OD_T("  Prototype Flags"), pPrototypeItem->getPrototypeFlags() );
              pDumper->writeFieldValue(OD_T("  Schema Id"), pPrototypeItem->getSchemaId() );
              pDumper->writeFieldValue(OD_T("  Schema Name"), pPrototypeItem->getSchemaName() );
              pDumper->writeFieldValueHex(OD_T("  Default Flags"), pPrototypeItem->getDefaultSchemaFlags() );

              OdString strData;

              for( OdUInt32 t = 0; t < pPrototypeItem->getSchemaItemCount(); t++ )
              {
                OdString strInt;
                strInt.format(L"%d, ", pPrototypeItem->getSchemaItemXAttributeIndex(t));
                strData += strInt;
              }

              if( !strData.isEmpty() )
                strData = strData.left(strData.getLength() - 2);

              pDumper->writeFieldValue( L"  XAttr Id Items:", strData );
            } break;

            case OdDgTableElementSchema::kHeaderPrototype:
            {
              OdDgTablePrototypeHeaderSchemaPtr pPrototypeHeader = pSchema;
              pDumper->writeFieldValue(OD_T("  Schema Type"), OdString(OD_T("kPrototypeHeader")) );
              pDumper->dumpFieldName(OD_T("  Header Data:"));
              OdBinaryData binData = pPrototypeHeader->getPrototypeHeader();
              dumpBinaryDataBytes( binData, pDumper );
            } break;

            case OdDgTableElementSchema::kMergeData:
            {
              OdDgMergeDataTableElementSchemaPtr pMerge = pSchema;
              pDumper->writeFieldValue(OD_T("  Schema Type"), OdString(OD_T("kMergeData")) );
              pDumper->writeFieldValue(OD_T("  First Row"), pMerge->getFirstRow() );
              pDumper->writeFieldValue(OD_T("  First Column"), pMerge->getFirstColumn() );
              pDumper->writeFieldValue(OD_T("  Rows Count"), pMerge->getRowsCount() );
              pDumper->writeFieldValue(OD_T("  Columns Count"), pMerge->getColumnsCount() );
            } break;

            case OdDgTableElementSchema::kFill:
            {
              OdDgFillTableElementSchemaPtr pFill = pSchema;
              pDumper->writeFieldValue(OD_T("  Schema Type"), OdString(OD_T("kFill")) );
              pDumper->writeFieldValue(OD_T("  Entry Id"), pFill->getEntryId() );
              pDumper->writeFieldColorValue(OD_T("  Fill Color Index"), pFill->getFillColorIndex() );
            } break;

            case OdDgTableElementSchema::kSymbology:
            {
              OdDgSymbologyTableElementSchemaPtr pSymbology = pSchema;
              pDumper->writeFieldValue(OD_T("  Schema Type"), OdString(OD_T("kSymbology")) );
              pDumper->writeFieldValue(OD_T("  Entry Id"), pSymbology->getEntryId() );
              pDumper->writeFieldColorValue(OD_T("  Color Index"), pSymbology->getColorIndex() );
              pDumper->writeFieldLineWeightValue(OD_T("  Line Weight"), pSymbology->getLineWeight() );
              pDumper->writeFieldLineStyleValue(OD_T("  Line Type"), pSymbology->getLineType() );
              pDumper->writeFieldValue(OD_T("  Visibility Flag"), pSymbology->getVisibilityFlag() );
              pDumper->writeFieldValue(OD_T("  Default Color Flag"), pSymbology->getDefaultColorFlag() );
              pDumper->writeFieldValue(OD_T("  Default Line Weight Flag"), pSymbology->getDefaultLineWeightFlag() );
              pDumper->writeFieldValue(OD_T("  Default Line Type Flag"), pSymbology->getDefaultLineTypeFlag() );
            } break;

            case OdDgTableElementSchema::kEdgeRun:
            {
              OdDgEdgeRunTableElementSchemaPtr pEdgeRun = pSchema;
              pDumper->writeFieldValue(OD_T("  Schema Type"), OdString(OD_T("kEdgeRun")) );
              pDumper->writeFieldValue(OD_T("  Cell Side"), pEdgeRun->getCellSide() );
              pDumper->writeFieldValue(OD_T("  Row"), pEdgeRun->getFirstRow() );
              pDumper->writeFieldValue(OD_T("  Column"), pEdgeRun->getFirstColumn() );
              pDumper->writeFieldValue(OD_T("  Length"), pEdgeRun->getLength() );
              pDumper->writeFieldValue(OD_T("  Symbology Index"), pEdgeRun->getSymbologyIndex() );
              pDumper->writeFieldValue(OD_T("  Default Symbology Flag"), pEdgeRun->getDefaultSymbologyFlag() );
              pDumper->writeFieldValue(OD_T("  First Row Or Column Flag"), pEdgeRun->getFirstRowOrColumnFlag() );
            } break;

            case OdDgTableElementSchema::kRowData:
            {
              OdDgRowTableElementSchemaPtr pRow = pSchema;
              pDumper->writeFieldValue(OD_T("  Schema Type"), OdString(OD_T("kRowData")) );
              pDumper->writeFieldValue(OD_T("  Row Index"), pRow->getRowIndex() );
              pDumper->writeFieldValue(OD_T("  Height Lock Flag"), pRow->getHeightLockFlag() );
              pDumper->writeFieldValue(OD_T("  Height"), pRow->getHeight() );
            } break;

            case OdDgTableElementSchema::kColumnData:
            {
              OdDgColumnTableElementSchemaPtr pColumn = pSchema;
              pDumper->writeFieldValue(OD_T("  Schema Type"), OdString(OD_T("kColumnData")) );
              pDumper->writeFieldValue(OD_T("  Column Index"), pColumn->getColumnIndex() );
              pDumper->writeFieldValue(OD_T("  Width Lock Flag"), pColumn->getWidthLockFlag() );
              pDumper->writeFieldValue(OD_T("  Width"), pColumn->getWidth() );
            } break;

            case OdDgTableElementSchema::kCellData:
            {
              OdDgCellTableElementSchemaPtr pCell = pSchema;
              pDumper->writeFieldValue(OD_T("  Schema Type"), OdString(OD_T("kCellData")) );
              pDumper->writeFieldValue(OD_T("  Row Index"), pCell->getRowIndex() );
              pDumper->writeFieldValue(OD_T("  Column Index"), pCell->getColumnIndex() );
              pDumper->writeFieldValue(OD_T("  Fill Flag"), pCell->getFillFlag() );
              pDumper->writeFieldValue(OD_T("  Fill Index"), pCell->getFillIndex() );
              pDumper->writeFieldValue(OD_T("  Text Alignment"), pCell->getTextAlignment() );
              pDumper->writeFieldValue(OD_T("  Text Orientation"), pCell->getTextOrientation() );

              if( pCell->getDefaultTopMarginFlag() )
                pDumper->writeFieldValue(OD_T("  Top Margin"),OdString(OD_T("Default")));
              else 
                pDumper->writeFieldValue(OD_T("  Top Margin"), pCell->getTopMargin() );
              
              if( pCell->getDefaultBottomMarginFlag() )
                pDumper->writeFieldValue(OD_T("  Bottom Margin"),OdString(OD_T("Default")));
              else 
                pDumper->writeFieldValue(OD_T("  Bottom Margin"), pCell->getBottomMargin() );

              if( pCell->getDefaultLeftMarginFlag() )
                pDumper->writeFieldValue(OD_T("  Left Margin"),OdString(OD_T("Default")));
              else 
                pDumper->writeFieldValue(OD_T("  Left Margin"), pCell->getLeftMargin() );

              if( pCell->getDefaultRightMarginFlag() )
                pDumper->writeFieldValue(OD_T("  Right Margin"),OdString(OD_T("Default")));
              else 
                pDumper->writeFieldValue(OD_T("  Right Margin"), pCell->getRightMargin() );

              if( pCell->hasTextData() )
              {
                pDumper->writeFieldValue(OD_T("  Text Data:"), pCell->getTextData() );
              }
              else
              {
                pDumper->writeFieldValue(OD_T("  Text Data:"), OdString( OD_T("<empty>")) );
              }
            } break;

            case OdDgTableElementSchema::kTableData:
            {
              OdDgTableDataTableElementSchemaPtr pTableData = pSchema;
              pDumper->writeFieldValue(OD_T("  Schema Type"), OdString(OD_T("kTableData")) );
              pDumper->writeFieldValue(OD_T("  Number of Rows "), pTableData->getRowCount() );
              pDumper->writeFieldValue(OD_T("  Number of Columns "), pTableData->getColumnCount() );
              pDumper->writeFieldValue(OD_T("  Number of Title Rows "), pTableData->getTitleRowCount() );
              pDumper->writeFieldValue(OD_T("  Number of Header Rows "), pTableData->getHeaderRowCount() );
              pDumper->writeFieldValue(OD_T("  Number of Footer Rows "), pTableData->getFooterRowCount() );
              pDumper->writeFieldValue(OD_T("  Number of First Columns "), pTableData->getFirstColumnCount() );
              pDumper->writeFieldValue(OD_T("  Number of Last Columns "), pTableData->getLastColumnCount() );
              pDumper->writeFieldValue(OD_T("  Body Text Style "), pTableData->getTableBodyTextStyle() );

              if( pTableData->getTitleRowsTextStyle() != OdUInt32(-1) )
                pDumper->writeFieldValue(OD_T("  Title Row Text Style "), pTableData->getTitleRowsTextStyle() );
              else
                pDumper->writeFieldValue(OD_T("  Title Row Text Style "), OdString(OD_T("From Body")) );

              if( pTableData->getHeaderRowsTextStyle() != OdUInt32(-1) )
                pDumper->writeFieldValue(OD_T("  Header Row Text Style "), pTableData->getHeaderRowsTextStyle() );
              else
                pDumper->writeFieldValue(OD_T("  Header Row Text Style "), OdString(OD_T("From Body")) );

              if( pTableData->getFooterRowsTextStyle() != OdUInt32(-1) )
                pDumper->writeFieldValue(OD_T("  Footer Row Text Style "), pTableData->getFooterRowsTextStyle() );
              else
                pDumper->writeFieldValue(OD_T("  Footer Row Text Style "), OdString(OD_T("From Body")) );

              if( pTableData->getFirstColumnsTextStyle() != OdUInt32(-1) )
                pDumper->writeFieldValue(OD_T("  First Column Text Style "), pTableData->getFirstColumnsTextStyle() );
              else
                pDumper->writeFieldValue(OD_T("  First Column Text Style "), OdString(OD_T("From Body")) );

              if( pTableData->getLastColumnsTextStyle() != OdUInt32(-1) )
                pDumper->writeFieldValue(OD_T("  Last Column Text Style "), pTableData->getLastColumnsTextStyle() );
              else
                pDumper->writeFieldValue(OD_T("  Last Column Text Style "), OdString(OD_T("From Body")) );

              if( pTableData->getOddFillStyleFlag() )
                pDumper->writeFieldValue(OD_T("  Odd Fill Style "), pTableData->getOddFillStyle() );
              else
                pDumper->writeFieldValue(OD_T("  Odd Fill Style "), OdString(OD_T("Off")) );

              if( pTableData->getEvenFillStyleFlag() )
                pDumper->writeFieldValue(OD_T("  Even Fill Style "), pTableData->getEvenFillStyle() );
              else
                pDumper->writeFieldValue(OD_T("  Even Fill Style "), OdString(OD_T("Off")) );

              if( pTableData->getDefaultTextSymbology() != OdUInt32(-1) )
                pDumper->writeFieldValue(OD_T("  Default Text Symbology "), pTableData->getDefaultTextSymbology() );
              else
                pDumper->writeFieldValue(OD_T("  Default Text Symbology  "), OdString(OD_T("From Table")) );

              if( pTableData->getAnnotationScaleFlag() )
                pDumper->writeFieldValue(OD_T("  Annotation Scale "), pTableData->getAnnotationScale() );
              else
                pDumper->writeFieldValue(OD_T("  Annotation Scale "), OdString(OD_T("Off")) );

              pDumper->writeFieldValue(OD_T("  Default Cell Width "), pTableData->getDefaultCellWidth() );
              pDumper->writeFieldValue(OD_T("  Default Cell Height "), pTableData->getDefaultCellHeight() );
              pDumper->writeFieldValue(OD_T("  Cell Top Margin "), pTableData->getCellTopMargin() );
              pDumper->writeFieldValue(OD_T("  Cell Bottom Margin "), pTableData->getCellBottomMargin() );
              pDumper->writeFieldValue(OD_T("  Cell Left Margin "), pTableData->getCellLeftMargin() );
              pDumper->writeFieldValue(OD_T("  Cell Right Margin "), pTableData->getCellRightMargin() );
              pDumper->writeFieldValue(OD_T("  Cell Alignment "), pTableData->getCellAlignment() );
              pDumper->writeFieldValue(OD_T("  Data Source Provider Id "), pTableData->getDataSourceProviderId() );
              pDumper->writeFieldValue(OD_T("  Default Text Size "), pTableData->getDefaultTextSize() );
              pDumper->writeFieldValue(OD_T("  Origin "), pTableData->getOrigin() );
              pDumper->writeFieldValue(OD_T("  Transformation "), pTableData->getTransformation() );
              pDumper->writeFieldValue(OD_T("  Break Type "), pTableData->getBreakType() );
              pDumper->writeFieldValue(OD_T("  Break Height "), pTableData->getBreakHeight() );
              pDumper->writeFieldValue(OD_T("  Sub Table Position "), pTableData->getSubTablePosition() );
              pDumper->writeFieldValue(OD_T("  Sub Table Spacing "), pTableData->getSubTableSpacing() );
              pDumper->writeFieldValue(OD_T("  Repeat Headers "), pTableData->getRepeatHeadersFlag() );
              pDumper->writeFieldValue(OD_T("  Repeat Footers "), pTableData->getRepeatFootersFlag() );

              if( !OdZero(pTableData->getBodyTextHeight()) )
                pDumper->writeFieldValue(OD_T("  Body Text Height "), pTableData->getBodyTextHeight() );
              else
                pDumper->writeFieldValue(OD_T("  Body Text Height "), OdString(OD_T("Default")) );

              if( !OdZero(pTableData->getTitleTextHeight()) )
                pDumper->writeFieldValue(OD_T("  Title Text Height "), pTableData->getTitleTextHeight() );
              else
                pDumper->writeFieldValue(OD_T("  Title Text Height "), OdString(OD_T("Default")) );

              if( !OdZero(pTableData->getHeaderTextHeight()) )
                pDumper->writeFieldValue(OD_T("  Header Text Height "), pTableData->getHeaderTextHeight() );
              else
                pDumper->writeFieldValue(OD_T("  Header Text Height "), OdString(OD_T("Default")) );

              if( !OdZero(pTableData->getFooterTextHeight()) )
                pDumper->writeFieldValue(OD_T("  Footer Text Height "), pTableData->getFooterTextHeight() );
              else
                pDumper->writeFieldValue(OD_T("  Footer Text Height "), OdString(OD_T("Default")) );

              if( !OdZero(pTableData->getFirstColumnTextHeight()) )
                pDumper->writeFieldValue(OD_T("  First Column Text Height "), pTableData->getFirstColumnTextHeight() );
              else
                pDumper->writeFieldValue(OD_T("  First Column Text Height "), OdString(OD_T("Default")) );

              if( !OdZero(pTableData->getLastColumnTextHeight()) )
                pDumper->writeFieldValue(OD_T("  Last Column Text Height "), pTableData->getLastColumnTextHeight() );
              else
                pDumper->writeFieldValue(OD_T("  Last Column Text Height "), OdString(OD_T("Default")) );

            } break;

            case OdDgTableElementSchema::kPrototypeBasedSchema:
            {
              OdDgPrototypeBasedTableElementSchemaPtr pPrototypeBasedData = pSchema;
              pDumper->writeFieldValue(L"  Schema Index", pPrototypeBasedData->getSchemaIndex() );
              pDumper->writeFieldValueHex(L"  Flags", pPrototypeBasedData->getPrototypeFlags() );

              for( OdUInt32 l = 0; l < pPrototypeBasedData->getSchemaItemCount(); l++ )
              {
                OdDgSchemaItemVariant curItemValue = pPrototypeBasedData->getSchemaItem(l);
                pDumper->writeFieldValue( L"  - " + curItemValue.getName(), curItemValue );
              }
            } break;

            default:
            {
              pDumper->dumpFieldName(OD_T("  Schema Data:"));
              OdBinaryData binData = pSchema->getData();
              dumpBinaryDataBytes( binData, pDumper );
            } break;
          }
        }
      }

      OdDgStringXAttributePtr pStringXAttr = OdDgStringXAttribute::cast(pXAttribute);

      if( !pStringXAttr.isNull() )
      {
        pDumper->writeFieldValue( OD_T("  String Value"), pStringXAttr->getString() );
      }

      OdDgDetailingSymbolXAttributePtr pDSXAttr = OdDgDetailingSymbolXAttribute::cast(pXAttribute);

      if( !pDSXAttr.isNull() )
      {
        if( pDSXAttr->getXAttributeType() == OdDgDetailingSymbolXAttribute::kSymbolSymbologyFlags )
        {
          OdString strName = OD_T("kSymbology");

          pDumper->writeFieldValue( OD_T("  XAttributeType"), strName );

          if( pDSXAttr->getBubbleSizeFlag() )
          {
            pDumper->writeFieldValue( OD_T("  BubbleSize"), pDSXAttr->getBubbleSize() );
          }
        }
        else
        {
          OdString strName = OD_T("kProperties");

          pDumper->writeFieldValue( OD_T("  XAttributeType"), strName );
          pDumper->writeFieldValue( OD_T("  AnnotationScaleFlag"), pDSXAttr->getAnnotationScaleFlag() );
          pDumper->writeFieldValue( OD_T("  AnnotationScale"), pDSXAttr->getAnnotationScale() );
          pDumper->writeFieldValue( OD_T("  SymbolSize"), pDSXAttr->getDetailingSymbolSize() );
          pDumper->writeFieldValue( OD_T("  DetailSymbolArea"), pDSXAttr->getDetailSymbolArea() );
        }
      }

      OdDgItemTypeXAttributePtr pItemTypeXAttr = OdDgItemTypeXAttribute::cast(pXAttribute);

      if( !pItemTypeXAttr.isNull() )
      {
        pDumper->writeFieldValue( "  Item Library Id", pItemTypeXAttr->getItemLibraryId() );
        pDumper->writeFieldValue( "  Item Library Name", pItemTypeXAttr->getItemLibraryName() );
        pDumper->writeFieldValue( "  Item Name", pItemTypeXAttr->getItemName() );
        pDumper->writeFieldValue( "  Properties:", pItemTypeXAttr->getRootNode() );
      }

      OdDgBinXMLXAttributePtr pBinXml = OdDgBinXMLXAttribute::cast(pXAttribute);

      if( !pBinXml.isNull() )
      {
        pDumper->writeFieldValue( "  TypeDescription", pBinXml->getTypeString() );
        pDumper->writeFieldValue( "  Fields:", pBinXml->getRootNode() );
      }

      OdDgTemplateHeaderXAttributePtr pTmpHdr = OdDgTemplateHeaderXAttribute::cast(pXAttribute);

      if( !pTmpHdr.isNull() )
      {
        pDumper->writeFieldValue(L"  Template Order", pTmpHdr->getTemplateOrder());

        if( pTmpHdr->getTemplateType() == OdDgTemplateHeaderXAttribute::kTemplateGroup )
        {
          pDumper->writeFieldValue(L"  Template Type",  OdString(L"kTemplateGroup"));
        }
        else
        {
          pDumper->writeFieldValue(L"  Template Type",  OdString(L"kElementTemplate"));
        }

        pDumper->writeFieldValue(L"  Template Name",  pTmpHdr->getTemplateName());
        pDumper->writeFieldValueHex(L"  Template ParentId",  pTmpHdr->getParentId());
      }

      OdDgTerrainStringXAttributePtr pTerrainStringXAttr = OdDgTerrainStringXAttribute::cast(pXAttribute);

      if( !pTerrainStringXAttr.isNull() )
      {
        pDumper->writeFieldValue(L"  String Value", pTerrainStringXAttr->getString() );
      }


      OdDgTerrainVertexCoordinatesXAttributePtr pTerrainVerticesXAttr = OdDgTerrainVertexCoordinatesXAttribute::cast(pXAttribute);

      if( !pTerrainVerticesXAttr.isNull() )
      {
        OdUInt32 nVertices = pTerrainVerticesXAttr->getVerticesCount();
        pDumper->writeFieldValue(L"  Vertices Count", nVertices );
        
        for( OdUInt32 i = 0; i < nVertices; i++ )
        {
          OdString strVertexName;
          strVertexName.format(L"    Vertex %d", i);
          pDumper->writeFieldValue(strVertexName, pTerrainVerticesXAttr->getVertex(i) );
        }
      }

      OdDgTerrainCorruptedFeatureXAttributePtr pTerrainBadFeatureXAttr = OdDgTerrainCorruptedFeatureXAttribute::cast(pXAttribute);

      if( !pTerrainBadFeatureXAttr.isNull() )
      {
        OdUInt32 nVertices = pTerrainBadFeatureXAttr->getVerticesCount();
        pDumper->writeFieldValue(L"  Vertices Count", nVertices );

        for( OdUInt32 i = 0; i < nVertices; i++ )
        {
          OdString strVertexName;
          strVertexName.format(L"    Vertex %d", i);
          pDumper->writeFieldValue(strVertexName, pTerrainBadFeatureXAttr->getVertex(i) );
        }
      }

      OdDgTextStyleDataXAttributePtr pTextStyleXAttr = OdDgTextStyleDataXAttribute::cast(pXAttribute);

      if( !pTextStyleXAttr.isNull() )
      {
        pDumper->writeFieldValue(L"  Text Style Index", pTextStyleXAttr->getTextStyleIndex() );
        pDumper->writeFieldValue(L"  Font Id", pTextStyleXAttr->getFontId() );
        pDumper->writeFieldValue(L"  Big Font Id", pTextStyleXAttr->getBigFontId() );
        pDumper->writeFieldValue(L"  CodePage", pTextStyleXAttr->getCodePage() );
        pDumper->writeFieldValue(L"  Text Width", pTextStyleXAttr->getTextWidth() );
        pDumper->writeFieldValue(L"  Text Height", pTextStyleXAttr->getTextHeight() );
        pDumper->writeFieldValue(L"  Slant Angle", pTextStyleXAttr->getSlantAngle() );
        pDumper->writeFieldValue(L"  Text Color Flag", pTextStyleXAttr->getTextColorFlag() );
        pDumper->writeFieldValue(L"  Text Color Index", pTextStyleXAttr->getTextColorIndex() );
        pDumper->writeFieldValue(L"  Text Justification", pTextStyleXAttr->getTextJustification() );
        pDumper->writeFieldValue(L"  Margin Justification Flag", pTextStyleXAttr->getMarginJustificationFlag() );
        pDumper->writeFieldValue(L"  Full Justification Flag", pTextStyleXAttr->getFullJustificationFlag() );
        pDumper->writeFieldValue(L"  Text Node Justification", pTextStyleXAttr->getTextNodeJustification() );
        pDumper->writeFieldValue(L"  Italic Flag", pTextStyleXAttr->getItalicFlag() );
        pDumper->writeFieldValue(L"  Bold Flag", pTextStyleXAttr->getBoldFlag() );
        pDumper->writeFieldValue(L"  SuperScript Flag", pTextStyleXAttr->getSuperScriptFlag() );
        pDumper->writeFieldValue(L"  SubScript Flag", pTextStyleXAttr->getSubScriptFlag() );
        pDumper->writeFieldValue(L"  Fraction Flag", pTextStyleXAttr->getFractionFlag() );
        pDumper->writeFieldValue(L"  Japanese Flag", pTextStyleXAttr->getJapaneseFlag() );
        pDumper->writeFieldValue(L"  Vertical Flag", pTextStyleXAttr->getVerticalFlag() );
        pDumper->writeFieldValue(L"  Right To Left Flag", pTextStyleXAttr->getRightToLeftFlag() );
        pDumper->writeFieldValue(L"  Backwards Flag", pTextStyleXAttr->getBackwardsFlag() );
        pDumper->writeFieldValue(L"  UpsideDown Flag", pTextStyleXAttr->getUpsideDownFlag() );
        pDumper->writeFieldValue(L"  Line Spacing Type", pTextStyleXAttr->getLineSpacingType() );
        pDumper->writeFieldValue(L"  Line Spacing", pTextStyleXAttr->getLineSpacing() );
        pDumper->writeFieldValue(L"  Interchar Spacing", pTextStyleXAttr->getIntercharSpacing() );
        pDumper->writeFieldValue(L"  Fixed Spacing Flag", pTextStyleXAttr->getFixedSpacingFlag() );
        pDumper->writeFieldValue(L"  ACad Interchar Spacing Flag", pTextStyleXAttr->getACadIntercharSpacingFlag() );
        pDumper->writeFieldValue(L"  Line Length", pTextStyleXAttr->getLineLength() );
        pDumper->writeFieldValue(L"  Line Offset", pTextStyleXAttr->getLineOffset() );
        pDumper->writeFieldValue(L"  Background Flag", pTextStyleXAttr->getBackgroundFlag() );
        pDumper->writeFieldValue(L"  Background Fill Color Index", pTextStyleXAttr->getBackgroundFillColorIndex() );
        pDumper->writeFieldValue(L"  Background Border Color Index", pTextStyleXAttr->getBackgroundColorIndex() );
        pDumper->writeFieldValue(L"  Background Border Line Style", pTextStyleXAttr->getBackgroundLineStyleEntryId() );
        pDumper->writeFieldValue(L"  Background Border Line Weight", pTextStyleXAttr->getBackgroundLineWeight() );
        pDumper->writeFieldValue(L"  Background Border Offset", pTextStyleXAttr->getBackgroundBorder() );
        pDumper->writeFieldValue(L"  Underline Flag", pTextStyleXAttr->getUnderlineFlag() );
        pDumper->writeFieldValue(L"  Underline Offset", pTextStyleXAttr->getUnderlineOffset() );
        pDumper->writeFieldValue(L"  Underline Style Flag", pTextStyleXAttr->getUnderlineStyleFlag() );
        pDumper->writeFieldValue(L"  Underline Color Index", pTextStyleXAttr->getUnderlineColorIndex() );
        pDumper->writeFieldValue(L"  Underline Line Style", pTextStyleXAttr->getUnderlineLineStyleEntryId() );
        pDumper->writeFieldValue(L"  Underline Line Weight", pTextStyleXAttr->getUnderlineLineWeight() );
        pDumper->writeFieldValue(L"  Overline Flag", pTextStyleXAttr->getOverlineFlag() );
        pDumper->writeFieldValue(L"  Overline Offset", pTextStyleXAttr->getOverlineOffset() );
        pDumper->writeFieldValue(L"  Overline Style Flag", pTextStyleXAttr->getOverlineStyleFlag() );
        pDumper->writeFieldValue(L"  Overline Color Index", pTextStyleXAttr->getOverlineColorIndex() );
        pDumper->writeFieldValue(L"  Overline Line Style", pTextStyleXAttr->getOverlineLineStyleEntryId() );
        pDumper->writeFieldValue(L"  Overline Line Weight", pTextStyleXAttr->getOverlineLineWeight() );
      }

      OdDgTerrainTransformationXAttributePtr pTerrainTransformXAttr = OdDgTerrainTransformationXAttribute::cast(pXAttribute);

      if( !pTerrainTransformXAttr.isNull() )
      {
        pDumper->writeFieldValue(L"  Transform Matrix", pTerrainTransformXAttr->getTransformation() );
      }

      OdDgTerrainDisplayStyleXAttributePtr pTerrainDisplayStyleXAttr = OdDgTerrainDisplayStyleXAttribute::cast(pXAttribute);

      if( !pTerrainDisplayStyleXAttr.isNull() )
      {
        pDumper->writeFieldValue(L"  Display Style Id", pTerrainDisplayStyleXAttr->getDisplayStyleEntryId() );
      }

      OdDgTerrainControlReferenceXAttributePtr pTerrainCtrlRefXAttr = OdDgTerrainControlReferenceXAttribute::cast(pXAttribute);

      if( !pTerrainCtrlRefXAttr.isNull() )
      {
        pDumper->writeFieldValueHex(L"  Terrain Control Element Id", pTerrainCtrlRefXAttr->getTerrainControlElementId() );
      }

      OdDgTerrainGraphicsSettingsXAttributePtr pTerrainGraphicsSettings = OdDgTerrainGraphicsSettingsXAttribute::cast(pXAttribute);

      if( !pTerrainGraphicsSettings.isNull() )
      {
        pDumper->writeFieldValue(L"  Terrain Settings Type", pTerrainGraphicsSettings->getSettingsType() );

        if( pTerrainGraphicsSettings->getSettingsType() == OdDgTerrainGraphicsSettingsXAttribute::kSourceFeatureSettings )
        {
          OdDgTerrainSourceFeatureSettings* pSourceFeatureSettings = (OdDgTerrainSourceFeatureSettings*)pTerrainGraphicsSettings->getGraphicsFeatureSettings().get();

          if( pSourceFeatureSettings )
          {
            pDumper->writeFieldValue(L"  Source Feature Type", pSourceFeatureSettings->getFeatureType() );
            pDumper->writeFieldValue(L"  Is On", pSourceFeatureSettings->getOnFlag() );
            pDumper->writeFieldValue(L"  Level Index", pSourceFeatureSettings->getLevelEntryId() );
            pDumper->writeFieldValue(L"  Color Index", (OdInt32)pSourceFeatureSettings->getColorIndex() );
            pDumper->writeFieldValue(L"  Line Style Entry Id", (OdInt32)pSourceFeatureSettings->getLineStyleEntryId() );
            pDumper->writeFieldValue(L"  Line Weight", (OdInt32)pSourceFeatureSettings->getLineWeight() );
            pDumper->writeFieldValue(L"  Transparency", pSourceFeatureSettings->getTransparency() );
          }
        }
        else if( pTerrainGraphicsSettings->getSettingsType() == OdDgTerrainGraphicsSettingsXAttribute::kVertexSettings )
        {
          OdDgTerrainVertexFeatureSettings* pSettings = (OdDgTerrainVertexFeatureSettings*)pTerrainGraphicsSettings->getGraphicsFeatureSettings().get();

          if( pSettings )
          {
            pDumper->writeFieldValue(L"  Vertex Feature Type", pSettings->getFeatureType() );
            pDumper->writeFieldValue(L"  Is On", pSettings->getOnFlag() );
            pDumper->writeFieldValue(L"  Level Index", pSettings->getLevelEntryId() );
            pDumper->writeFieldValue(L"  Color Index", (OdInt32)pSettings->getColorIndex() );
            pDumper->writeFieldValue(L"  Line Style Entry Id", (OdInt32)pSettings->getLineStyleEntryId() );
            pDumper->writeFieldValue(L"  Line Weight", (OdInt32)pSettings->getLineWeight() );
            pDumper->writeFieldValue(L"  Transparency", pSettings->getTransparency() );

            if( pSettings->getFeatureType() == OdDgTerrainVertexFeatureSettings::kLowPoints )
            {
              pDumper->writeFieldValue(L"  Min Value", pSettings->getMinValue() );
            }

            pDumper->writeFieldValue(L"  Cell X Scale", pSettings->getCellXScale() );
            pDumper->writeFieldValue(L"  Cell Y Scale", pSettings->getCellYScale() );
            pDumper->writeFieldValue(L"  Cell Z Scale", pSettings->getCellZScale() );

            if( pSettings->getFeatureType() != OdDgTerrainVertexFeatureSettings::kFlowArrows )
            {
              pDumper->writeFieldValue(L"  Display Text", pSettings->getDisplayTextFlag() );
              pDumper->writeFieldValue(L"  Prefix", pSettings->getPrefix() );
              pDumper->writeFieldValue(L"  Suffix", pSettings->getSuffix() );
              pDumper->writeFieldValue(L"  Text Style Id", pSettings->getTextStyleEntryId() );
            }

            pDumper->writeFieldValue(L"  Point Display Type", pSettings->getPointDisplayType() );

            if( pSettings->getPointDisplayType() == OdDgTerrainOptions::kCell )
            {
              pDumper->writeFieldValue(L"  Cell Name", pSettings->getCellName() );
            }
            else if( pSettings->getPointDisplayType() == OdDgTerrainOptions::kSymbol )
            {
              pDumper->writeFieldValue(L"  Char Symbol", pSettings->getCharSymbol() );
            }
          }
        }
        else if( pTerrainGraphicsSettings->getSettingsType() == OdDgTerrainGraphicsSettingsXAttribute::kContourSettings )
        {
          OdDgTerrainContourFeatureSettings* pContourFeatureSettings = (OdDgTerrainContourFeatureSettings*)pTerrainGraphicsSettings->getGraphicsFeatureSettings().get();

          if( pContourFeatureSettings )
          {
            pDumper->writeFieldValue(L"  Contour Feature Type", pContourFeatureSettings->getFeatureType() );
            pDumper->writeFieldValue(L"  Is On", pContourFeatureSettings->getOnFlag() );
            pDumper->writeFieldValue(L"  Level Index", pContourFeatureSettings->getLevelEntryId() );
            pDumper->writeFieldValue(L"  Color Index", (OdInt32)pContourFeatureSettings->getColorIndex() );
            pDumper->writeFieldValue(L"  Line Style Entry Id", (OdInt32)pContourFeatureSettings->getLineStyleEntryId() );
            pDumper->writeFieldValue(L"  Line Weight", (OdInt32)pContourFeatureSettings->getLineWeight() );
            pDumper->writeFieldValue(L"  Transparency", pContourFeatureSettings->getTransparency() );
            pDumper->writeFieldValue(L"  Interval", pContourFeatureSettings->getInterval() );
            pDumper->writeFieldValue(L"  Smoothing Type", pContourFeatureSettings->getSmoothingType() );
            pDumper->writeFieldValue(L"  Smoothing Factor", pContourFeatureSettings->getSmoothingFactor() );
            pDumper->writeFieldValue(L"  Display Text Flag", pContourFeatureSettings->getDisplayTextFlag() );
            pDumper->writeFieldValue(L"  Text Interval", pContourFeatureSettings->getTextInterval() );
            pDumper->writeFieldValue(L"  UORs Per Working Units Scale", pContourFeatureSettings->getUORPerWorkingUnitsScale() );
            pDumper->writeFieldValue(L"  Text Style Entry Id", pContourFeatureSettings->getTextStyleEntryId() );
            pDumper->writeFieldValue(L"  Ignore Max Slopes Flag", pContourFeatureSettings->getIgnoreMaxSlopesFlag() );
            pDumper->writeFieldValue(L"  Max Slope Value", pContourFeatureSettings->getMaxSlopeValue() );
            pDumper->writeFieldValue(L"  Depression Color Index", (OdInt32)pContourFeatureSettings->getDepressionColorIndex() );
            pDumper->writeFieldValue(L"  Depression Line Style Entry Id", (OdInt32)pContourFeatureSettings->getDepressionLineStyleEntryId() );
            pDumper->writeFieldValue(L"  Depression Line Weight", (OdInt32)pContourFeatureSettings->getDepressionLineWeight() );
            pDumper->writeFieldValue(L"  Contour Label Precision", pContourFeatureSettings->getContourLabelPrecision() );
          }
        }
        else if( pTerrainGraphicsSettings->getSettingsType() == OdDgTerrainGraphicsSettingsXAttribute::kTriangleSettings )
        {
          OdDgTerrainTrianglesSettings* pTrianglesSettings = (OdDgTerrainTrianglesSettings*)pTerrainGraphicsSettings->getGraphicsFeatureSettings().get();

          if( pTrianglesSettings )
          {
            pDumper->writeFieldValue(L"  Is On", pTrianglesSettings->getOnFlag() );
            pDumper->writeFieldValue(L"  Level Index", pTrianglesSettings->getLevelEntryId() );
            pDumper->writeFieldValue(L"  Color Index", (OdInt32)pTrianglesSettings->getColorIndex() );
            pDumper->writeFieldValue(L"  Line Style Entry Id", (OdInt32)pTrianglesSettings->getLineStyleEntryId() );
            pDumper->writeFieldValue(L"  Line Weight", (OdInt32)pTrianglesSettings->getLineWeight() );
            pDumper->writeFieldValue(L"  Transparency", pTrianglesSettings->getTransparency() );
            pDumper->writeFieldValue(L"  Material Id", pTrianglesSettings->getMaterialId() );
          }
        }
      }

      OdDgExtendedElementTypeXAttributePtr pExtElmTypeXAttr = OdDgExtendedElementTypeXAttribute::cast(pXAttribute);

      if( !pExtElmTypeXAttr.isNull() )
      {
        pDumper->writeFieldValue(L"  Extended Element Type", pExtElmTypeXAttr->getElementType() );
        pDumper->writeFieldValueHex(L"  Flags", pExtElmTypeXAttr->getFlags() );
      }

      OdDgTerrainInfoXAttributePtr pTerrainInfoXAttr = OdDgTerrainInfoXAttribute::cast(pXAttribute);

      if( !pTerrainInfoXAttr.isNull() )
      {
        pDumper->writeFieldValue(L"  Number Of Points", pTerrainInfoXAttr->getNumberOfPoints() );
        pDumper->writeFieldValue(L"  Number Of Edges", pTerrainInfoXAttr->getNumberOfEdges() );
        pDumper->writeFieldValue(L"  Number Of Triangles", pTerrainInfoXAttr->getNumberOfTriangles() );
        pDumper->writeFieldValue(L"  Number Of Features", pTerrainInfoXAttr->getNumberOfFeatures() );
        pDumper->writeFieldValue(L"  Number Of Edge Records", pTerrainInfoXAttr->getNumberOfEdgeRecords() );
        pDumper->writeFieldValue(L"  Number Of Feature Topo Records", pTerrainInfoXAttr->getNumberOfFeatureTopologyRecords() );
        pDumper->writeFieldValue(L"  Number Of Feature Linkages", pTerrainInfoXAttr->getNumberOfFeatureLinkages() );
        pDumper->writeFieldValue(L"  Number Of Vertex Topo Linkages", pTerrainInfoXAttr->getNumberOfVertexTopoLinkages() );
        pDumper->writeFieldValue(L"  Number Of Vertex Linkages", pTerrainInfoXAttr->getNumberOfVertexLinkages() );
        pDumper->writeFieldValue(L"  Number Of Edge Linkages", pTerrainInfoXAttr->getNumberOfEdgeLinkages() );
        pDumper->writeFieldValue(L"  Number Of Feature Topo Linkages", pTerrainInfoXAttr->getNumberOfFeatureTopoLinkages() );
        pDumper->writeFieldValue(L"  Number Of Unknown Linkages", pTerrainInfoXAttr->getNumberOfUnknownLinkages() );
        pDumper->writeFieldValue(L"  First Boundary Index", pTerrainInfoXAttr->getFirstBoundaryIndex() );
        pDumper->writeFieldValue(L"  Edge Method", pTerrainInfoXAttr->getEdgeMethod() );
        pDumper->writeFieldValue(L"  Number Of Native Triangles And Features", pTerrainInfoXAttr->getNumberOfNativeTrianglesAndFeatures() );
        pDumper->writeFieldValueHex(L"  Unknown Value 1", pTerrainInfoXAttr->getUnknownValue1() );
        pDumper->writeFieldValueHex(L"  Unknown Value 2", pTerrainInfoXAttr->getUnknownValue2() );
        pDumper->writeFieldValueHex(L"  Unknown Value 3", pTerrainInfoXAttr->getUnknownValue3() );
        pDumper->writeFieldValueHex(L"  Unknown Value 4", pTerrainInfoXAttr->getUnknownValue4() );
        pDumper->writeFieldValueHex(L"  Unknown Flags 1", pTerrainInfoXAttr->getUnknownFlags1() );
        pDumper->writeFieldValueHex(L"  Unknown Flags 2", pTerrainInfoXAttr->getUnknownFlags2() );
        pDumper->writeFieldValueHex(L"  Unknown Flags 3", pTerrainInfoXAttr->getUnknownFlags3() );
        pDumper->writeFieldValueHex(L"  Unknown Flags 4", pTerrainInfoXAttr->getUnknownFlags4() );
        pDumper->writeFieldValueHex(L"  Unknown Flags 5", pTerrainInfoXAttr->getUnknownFlags5() );
        pDumper->writeFieldValue(L"  Max Points Or 1000", pTerrainInfoXAttr->getMaxPointsOr1000() );
        pDumper->writeFieldValue(L"  Max Points Or 100", pTerrainInfoXAttr->getMaxPointsOr100() );
        pDumper->writeFieldValue(L"  Max Triangle Length", pTerrainInfoXAttr->getMaxTriangleLength() );
        pDumper->writeFieldValue(L"  Model Extents Min", pTerrainInfoXAttr->getModelExtents().minPoint() );
        pDumper->writeFieldValue(L"  Model Extents Max", pTerrainInfoXAttr->getModelExtents().maxPoint() );
      }

      OdDgTerrainVertexTopologyXAttributePtr pTerrainVertexTopologyXAttr = OdDgTerrainVertexTopologyXAttribute::cast(pXAttribute);

      if( !pTerrainVertexTopologyXAttr.isNull() )
      {
        OdUInt32 nVertices = pTerrainVertexTopologyXAttr->getVerticesCount();
        pDumper->writeFieldValue(L"  Vertices Count", nVertices );

        for( OdUInt32 i = 0; i < nVertices; i++ )
        {
          OdString strVertexName;
          strVertexName.format(L"    Vertex %d :", i);
          pDumper->dumpFieldName(strVertexName );

          OdDgTerrainVertexTopologyItem topologyItem = pTerrainVertexTopologyXAttr->getVertexTopology(i);

          if( topologyItem.m_iFlags != 0 )
          {
            pDumper->writeFieldValueHex(L"      Flags:", topologyItem.m_iFlags );
          }

          pDumper->writeFieldValue(L"      Next Boundary Index:", topologyItem.m_iNextBoundaryIndex );
          pDumper->writeFieldValue(L"      Triangle Start Index:", topologyItem.m_iTriangleStartIndex );

          if( topologyItem.m_iUnknownIndex2 != -1 )
          {
            pDumper->writeFieldValue(L"      Unknown Index 2:", topologyItem.m_iUnknownIndex2 );
          }

          if( topologyItem.m_iUnknownIndex3 != -1 )
          {
            pDumper->writeFieldValue(L"      Unknown Index 3:", topologyItem.m_iUnknownIndex3 );
          }

          if( topologyItem.m_iFeaturePtIndex != -1 )
          {
            pDumper->writeFieldValue(L"      Feature Point Index:", topologyItem.m_iFeaturePtIndex );
          }
        }
      }

      OdDgTerrainFeatureTopologyXAttributePtr pTerrainFeatureTopologyXAttr = OdDgTerrainFeatureTopologyXAttribute::cast(pXAttribute);

      if( !pTerrainFeatureTopologyXAttr.isNull() )
      {
        OdUInt32 nItems = pTerrainFeatureTopologyXAttr->getFeatureTopologyItemCount();
        pDumper->writeFieldValue(L"  Items Count", nItems );

        for( OdUInt32 i = 0; i < nItems; i++ )
        {
          OdDgTerrainFeatureTopologyItem curItem = pTerrainFeatureTopologyXAttr->getFeatureTopologyItem(i);

          OdString strFeatureName;
          strFeatureName.format(L"    Topology Item %d :", i);

          OdString strValueFeatureAndVertexIndex;
          strValueFeatureAndVertexIndex.format(L"%d: Feature %d, Vertex = %d, Item = %d, Index = %d", 
            curItem.m_uItemIndex, curItem.m_uFeatureNumber, curItem.m_uEndVertexIndex, curItem.m_uStartVertexItemIndex,
            curItem.m_uUnknownIndex);

          pDumper->writeFieldValue(strFeatureName, strValueFeatureAndVertexIndex );
        }
      }

      OdDgTerrainFeatureDescriptionXAttributePtr pTerrainFeatureDescXAttr = OdDgTerrainFeatureDescriptionXAttribute::cast(pXAttribute);

      if( !pTerrainFeatureDescXAttr.isNull() )
      {
        OdUInt32 nFeatures = pTerrainFeatureDescXAttr->getFeatureDescriptionCount();
        pDumper->writeFieldValue(L"  Feature Count", nFeatures );

        for( OdUInt32 i = 0; i < nFeatures; i++ )
        {
          OdString strFeatureFieldName;
          strFeatureFieldName.format(L"    Feature %d :", i);
          pDumper->dumpFieldName(strFeatureFieldName );

          OdDgTerrainFeatureDescriptionItem featureItem = pTerrainFeatureDescXAttr->getFeatureDescription(i);
          pDumper->writeFieldValue(L"      Feature Number:", featureItem.m_uFeatureNumber );
          pDumper->writeFieldValue(L"      Feature Type:", featureItem.m_uFeatureType );
          pDumper->writeFieldValue(L"      Feature Points:", featureItem.m_nPoints );
          pDumper->writeFieldValue(L"      Feature Vertex:", featureItem.m_uVertexIndex );

          if( featureItem.m_bCorruptedFeature )
          {
            pDumper->writeFieldValue(L"      Feature Status:", OdString(L"Corrupted") );
          }

          if( featureItem.m_uUnknownValue1 != -1 )
          {
            pDumper->writeFieldValueHex(L"      Unknown Value 1:", featureItem.m_uUnknownValue1 );
          }

          if( featureItem.m_uUnknownValue2 != 0 )
          {
            pDumper->writeFieldValueHex(L"      Unknown Value 2:", featureItem.m_uUnknownValue2 );
          }
        }
      }

      OdDgTerrainEdgesXAttributePtr pTerrainEdgesXAttr = OdDgTerrainEdgesXAttribute::cast(pXAttribute);

      if( !pTerrainEdgesXAttr.isNull() )
      {
        OdUInt32 nEdges = pTerrainEdgesXAttr->getEdgeCount();
        pDumper->writeFieldValue(L"  Edge Count", nEdges );

        for( OdUInt32 i = 0; i < nEdges; i++ )
        {
          OdDgTerrainEdgeItem edgeItem = pTerrainEdgesXAttr->getEdge(i);
          OdString strEdgeName;
          OdString strStartEndEdgeValue;
          OdString strEndStartEdgeValue;
          strStartEndEdgeValue.format(L"%d : Vertex Index - %d => %d", pTerrainEdgesXAttr->getXAttrId()*32768 + 
            edgeItem.m_startToEnd.m_uRecordIndex, edgeItem.m_startToEnd.m_uVertexIndex, edgeItem.m_startToEnd.m_iNextTriangleEdgeIndex );
          strEdgeName.format(L"    Edge %d", pTerrainEdgesXAttr->getXAttrId()*16384 + i);
          strEndStartEdgeValue.format(L"%d : Vertex Index - %d => %d", pTerrainEdgesXAttr->getXAttrId()*32768 + 
            edgeItem.m_endToStart.m_uRecordIndex, edgeItem.m_endToStart.m_uVertexIndex, edgeItem.m_endToStart.m_iNextTriangleEdgeIndex );
          pDumper->writeFieldValue(strEdgeName, strStartEndEdgeValue );
          pDumper->writeFieldValue(L"", strEndStartEdgeValue );
        }
      }

      OdDgSectionClipDataXAttributePtr pSecClipData = OdDgSectionClipDataXAttribute::cast(pXAttribute);

      if( !pSecClipData.isNull() )
      {
        pDumper->writeFieldValue(L"  Perspective Up Flag", pSecClipData->getPerspectiveUpFlag() );
        pDumper->writeFieldValue(L"  Left Crop Flag",   pSecClipData->getLeftCropFlag() );
        pDumper->writeFieldValue(L"  Right Crop Flag",  pSecClipData->getRightCropFlag() );
        pDumper->writeFieldValue(L"  Top Crop Flag",    pSecClipData->getTopCropFlag() );
        pDumper->writeFieldValue(L"  Bottom Crop Flag", pSecClipData->getBottomCropFlag() );
        pDumper->writeFieldValue(L"  Front Crop Flag",  pSecClipData->getFrontCropFlag() );
        pDumper->writeFieldValue(L"  Back Crop Flag",   pSecClipData->getBackCropFlag() );
        pDumper->writeFieldValue(L"  Front Depth",      pSecClipData->getFrontDepth() );
        pDumper->writeFieldValue(L"  Back  Depth",      pSecClipData->getBackDepth() );
        pDumper->writeFieldValue(L"  Top Height",       pSecClipData->getTopHeight() );
        pDumper->writeFieldValue(L"  Bottom Height",    pSecClipData->getBottomHeight() );
        pDumper->writeFieldValue(L"  Rotation",         pSecClipData->getRotation() );
      }

      OdDgSectionClipDataExtXAttributePtr pSecClipDataExt = OdDgSectionClipDataExtXAttribute::cast(pXAttribute);

      if( !pSecClipDataExt.isNull() )
      {
        pDumper->writeFieldValue(L"  Origin", pSecClipDataExt->getOrigin() );
        pDumper->writeFieldValue(L"  Direction Point", pSecClipDataExt->getDirection() );
      }

      OdDgTemplateIdXAttributePtr pTmpIdAttr = OdDgTemplateIdXAttribute::cast(pXAttribute);

      if( !pTmpIdAttr.isNull() )
      {
        pDumper->writeFieldValueHex(L"  Element Template Id", pTmpIdAttr->getTemplateId());
      }

      pDumper->writeFieldValue( "  Raw Data Size", (OdUInt32)data.size() );

      OdDgRasterClipXAttributePtr pRasterClipXAttr = OdDgRasterClipXAttribute::cast(pXAttribute);

      if( !pRasterClipXAttr.isNull() )
      {
        for( OdUInt32 n = 0; n < pRasterClipXAttr->getClipElementCount(); n++ )
        {
          OdDgRasterClipXAttribute::OdDgRasterClipElementData clipData = pRasterClipXAttr->getClipElementData(n);

          OdString strElementName;
          OdString strFieldName;

          strFieldName.format(L"  Clip element %d", n);

          if( !clipData.pContourElement.isNull() )
          {
            strElementName.format(L"<%ls>", clipData.pContourElement->isA()->name().c_str());
          }
          else
          {
            strElementName = L"<empty>";
          }

          pDumper->writeFieldValue( strFieldName, strElementName );
        }
      }

      OdDgAnnotationCellXAttributePtr pAnnotationCellAttr = OdDgAnnotationCellXAttribute::cast(pXAttribute);

      if( !pAnnotationCellAttr.isNull() )
      {
        pDumper->writeFieldValue(L"  Annotation Scale", pAnnotationCellAttr->getAnnotationScale());
      }

      OdDgDisplayStyleNumberXAttributePtr pDSNumAttr = OdDgDisplayStyleNumberXAttribute::cast(pXAttribute);

      if( !pDSNumAttr.isNull() )
      {
        pDumper->writeFieldValue(L"  Display Style Entry Id", pDSNumAttr->getDisplayStyleEntryId());
      }

      OdDgClipVolumeSettingsXAttributePtr pClipVolumeAttr = OdDgClipVolumeSettingsXAttribute::cast(pXAttribute);

      if( !pClipVolumeAttr.isNull() )
      {
        pDumper->writeFieldValue(L"  Forward Display Flag", pClipVolumeAttr->getForwardDisplayFlag());
        pDumper->writeFieldValue(L"  Forward Snap Flag", pClipVolumeAttr->getForwardSnapFlag());
        pDumper->writeFieldValue(L"  Forward Locate Flag", pClipVolumeAttr->getForwardLocateFlag());
        pDumper->writeFieldValueHex(L"  Forward Display Style Id", pClipVolumeAttr->getForwardDisplayStyleEntryId());
        pDumper->writeFieldValue(L"  Back Display Flag", pClipVolumeAttr->getBackDisplayFlag());
        pDumper->writeFieldValue(L"  Back Snap Flag", pClipVolumeAttr->getBackSnapFlag());
        pDumper->writeFieldValue(L"  Back Locate Flag", pClipVolumeAttr->getBackLocateFlag());
        pDumper->writeFieldValueHex(L"  Back Display Style Id", pClipVolumeAttr->getBackDisplayStyleEntryId());
        pDumper->writeFieldValue(L"  Cut Display Flag", pClipVolumeAttr->getCutDisplayFlag());
        pDumper->writeFieldValue(L"  Cut Snap Flag", pClipVolumeAttr->getCutSnapFlag());
        pDumper->writeFieldValue(L"  Cut Locate Flag", pClipVolumeAttr->getCutLocateFlag());
        pDumper->writeFieldValueHex(L"  Cut Display Style Id", pClipVolumeAttr->getCutDisplayStyleEntryId());
        pDumper->writeFieldValue(L"  Outside Display Flag", pClipVolumeAttr->getOutsideDisplayFlag());
        pDumper->writeFieldValue(L"  Outside Snap Flag", pClipVolumeAttr->getOutsideSnapFlag());
        pDumper->writeFieldValue(L"  Outside Locate Flag", pClipVolumeAttr->getOutsideLocateFlag());
        pDumper->writeFieldValueHex(L"  Outside Display Style Id", pClipVolumeAttr->getOutsideDisplayStyleEntryId());
      }

      OdDgDisplayStyleAzimuthAltitudeXAttributePtr pAzimuthAltitudeXAttr = OdDgDisplayStyleAzimuthAltitudeXAttribute::cast(pXAttribute);
      OdDgDisplayStyleLegendXAttributePtr          pDisplayStyleLegendXAttr = OdDgDisplayStyleLegendXAttribute::cast(pXAttribute);
      OdDgDisplayStyleTypeFilterXAttributePtr      pDisplayStyleTypeFilterXAttr = OdDgDisplayStyleTypeFilterXAttribute::cast(pXAttribute);
      OdDgZippedXAttributePtr pZippedXAttr = OdDgZippedXAttribute::cast(pXAttribute);

      if( !pAzimuthAltitudeXAttr.isNull() )
      {
        pDumper->writeFieldValue( L"  Azimuth",  pAzimuthAltitudeXAttr->getAzimuth() );
        pDumper->writeFieldValue( L"  Altitude", pAzimuthAltitudeXAttr->getAltitude() );
      }
      else if( !pDisplayStyleTypeFilterXAttr.isNull() )
      {
        pDumper->writeFieldValueHex( L"  Flags",  pDisplayStyleTypeFilterXAttr->getFlags() );
        pDumper->writeFieldValue( L"  ApplyTo", pDisplayStyleTypeFilterXAttr->getApplyToMode() );

        if( pDisplayStyleTypeFilterXAttr->getSelectedElementTypeCount() > 0 )
        {
          pDumper->dumpFieldName(L"  Selected element types:");

          for( OdUInt32 n = 0; n < pDisplayStyleTypeFilterXAttr->getSelectedElementTypeCount(); n++ )
          {
            OdString strFieldName;
            strFieldName.format(L"    Element Type %d", n);
            pDumper->writeFieldValue( strFieldName, pDisplayStyleTypeFilterXAttr->getSelectedElementType(n));
          }
        }
      }
      else if( !pDisplayStyleLegendXAttr.isNull() )
      {
        pDumper->writeFieldValue( L"  CoordinateAxis",  pDisplayStyleLegendXAttr->getCoordinateAxis() );
        pDumper->writeFieldValue( L"  SlopeDisplayMode",  pDisplayStyleLegendXAttr->getSlopeDisplayMode() );
        pDumper->writeFieldValue( L"  LegendColorScheme",  pDisplayStyleLegendXAttr->getLegendColorScheme() );
        pDumper->writeFieldValue( L"  UseFixedDisplayMinFlag",  pDisplayStyleLegendXAttr->getUseFixedDisplayMinFlag() );
        pDumper->writeFieldValue( L"  UseFixedDisplayMaxFlag",  pDisplayStyleLegendXAttr->getUseFixedDisplayMaxFlag() );
        pDumper->writeFieldValue( L"  EdgeOverride",  pDisplayStyleLegendXAttr->getEdgeOverride() );
        pDumper->writeFieldValue( L"  ValuesCalcMethod",  pDisplayStyleLegendXAttr->getValuesCalcMethod() );
        pDumper->writeFieldValue( L"  DisplayLegendFlag",  pDisplayStyleLegendXAttr->getDisplayLegendFlag() );
        pDumper->writeFieldValue( L"  SteppedDisplayMethod",  pDisplayStyleLegendXAttr->getSteppedDisplayMethod() );
        pDumper->writeFieldValue( L"  TransparentMarginsFlag",  pDisplayStyleLegendXAttr->getTransparentMarginsFlag() );
        pDumper->writeFieldValue( L"  DescendingLegendOrderFlag",  pDisplayStyleLegendXAttr->getDescendingLegendOrderFlag() );
        pDumper->writeFieldValue( L"  FixedDisplayMinValue",  pDisplayStyleLegendXAttr->getFixedDisplayMinValue() );
        pDumper->writeFieldValue( L"  FixedDisplayMaxValue",  pDisplayStyleLegendXAttr->getFixedDisplayMaxValue() );
        pDumper->writeFieldValue( L"  LegendStepValue",  pDisplayStyleLegendXAttr->getLegendStepValue() );
        pDumper->writeFieldValueHex( L"  MarginColor",  pDisplayStyleLegendXAttr->getMarginColor() );
        pDumper->writeFieldValue( L"  LegendTransparency",  pDisplayStyleLegendXAttr->getLegendTransparency() );

        if( pDisplayStyleLegendXAttr->getColorSchemeItemCount() )
        {
          pDumper->dumpFieldName("  Color Scheme:");

          for( OdUInt32 n = 0; n < pDisplayStyleLegendXAttr->getColorSchemeItemCount(); n++ )
          {
            OdString strFieldName;
            strFieldName.format(L"    Item %d", n);
            pDumper->writeFieldValue( strFieldName,  pDisplayStyleLegendXAttr->getColorSchemeItem(n) );
          }
        }

        if( pDisplayStyleLegendXAttr->getLegendItemCount() )
        {
          pDumper->dumpFieldName("  Legend:");

          for( OdUInt32 m = 0; m < pDisplayStyleLegendXAttr->getLegendItemCount(); m++ )
          {
            OdString strFieldName;
            strFieldName.format(L"    Item %d", m);
            pDumper->writeFieldValue( strFieldName,  pDisplayStyleLegendXAttr->getLegendItem(m) );
          }
        }
      }
      else if( !pZippedXAttr.isNull() )
      {
        OdBinaryData binData;

        pZippedXAttr->getUnzippedData( binData );

        dumpBinaryDataBytes( binData, pDumper );
      }

      OdDgProxyXAttributePtr pProxyXAttrAttr = OdDgProxyXAttribute::cast(pXAttribute);

      if( !pProxyXAttrAttr.isNull() )
      {
        OdBinaryData binData;

        pProxyXAttrAttr->getData( binData );

        dumpBinaryDataBytes( binData, pDumper );
      }
    }
  }
}

OdString OdDgElementDumperPE::getName( OdRxObjectPtr pObj ) const
{
  OdString s;
  s.format(L"<%ls>", pObj->isA()->name().c_str());
  return s;
}

bool OdDgElementDumperPE::hasSubElements( OdDgElementPtr pElm ) const
{
  OdDgElementIteratorPtr pIt = createIterator( pElm );
  return !pIt.isNull() && !pIt->done();
}

//----------------------------------------------------------
//
// OdDgDatabaseDumperPE
//
//----------------------------------------------------------
void OdDgDatabaseDumperPE::dumpData( OdRxObjectPtr pObj, OdExDgnDumper* pDumper ) const
{
  OdDgDatabasePtr pDb = pObj;

  OdDgElementId idDb = pDb->id();

  if( idDb.isNull() )
    pDumper->writeFieldValue( L"Database Element Id", OdString(L"NULL") );
  else
  {
    OdDgElementId elemId(pDb->id());
    OdRxObjectPtr spObj = elemId.openObject();

    pDumper->writeFieldValue( L"Database Element Id", OdString(L"Not NULL") );
  }

  //simple fields
  pDumper->writeFieldValue( "Filename", pDb->getFilename() );
  pDumper->writeFieldValue( "Major version", pDb->getMajorVersion() );
  pDumper->writeFieldValue( "Minor version", pDb->getMinorVersion() );
  pDumper->writeFieldValue( "Original file version", pDb->getOriginalFileVersion() );
  pDumper->writeFieldValue( "Control", pDb->getControlFlags() );
  pDumper->writeFieldValue( "Control 1", pDb->getControl1Flags() );
  pDumper->writeFieldValue( "Fbfdcn", pDb->getLockFlags() );
  pDumper->writeFieldValue( "Ext locks", pDb->getExtLockFlags() );
  pDumper->writeFieldValue( "Active angle", pDb->getActAngle() );
  pDumper->writeFieldValue( "Angle round", pDb->getAngleRnd() );
  pDumper->writeFieldValue( "X active scale", pDb->getXActScale() );
  pDumper->writeFieldValue( "Y active scale", pDb->getYActScale() );
  pDumper->writeFieldValue( "Z active scale", pDb->getZActScale() );
  pDumper->writeFieldValue( "Round scale", pDb->getRoundScale() );
  pDumper->writeFieldValue( "Azumuth", pDb->getAzimuth() );
  pDumper->writeFieldValue( "Low", pDb->getExtMin() );
  pDumper->writeFieldValue( "High", pDb->getExtMax() );
  pDumper->writeFieldValue( "Active level", pDb->getActiveLevelEntryId() );
  pDumper->writeFieldValue( "Active line style", pDb->getActiveLineStyleEntryId() );
  pDumper->writeFieldValue( "Active line weight", pDb->getActiveLineWeight() );
  pDumper->writeFieldValue( "Active color index", pDb->getActiveColorIndex() );
  pDumper->writeFieldValue( "Fill color", pDb->getFillColorIndex() );
  pDumper->writeFieldValue( "Active props", pDb->getActiveProps() );
  pDumper->writeFieldValue( "Line style", pDb->getLineStyle() );
  pDumper->writeFieldValue( "Line style scale", pDb->getLineStyleScale() );
  pDumper->writeFieldValueHex( "Multiline flags", pDb->getMultiLineFlags() );
  pDumper->writeFieldValue( "Text style ID", pDb->getActiveTextStyleEntryId() );
  pDumper->writeFieldValue( "Text scale lock", pDb->getTextScaleLock() );
  pDumper->writeFieldValue( "Active model ID", pDb->getActiveModelId().getHandle().ascii() );
  pDumper->writeFieldValue( "Angle readout format", pDb->getAngleFormat() );
  pDumper->writeFieldValue( "Angle readout precision", pDb->getAngleReadoutPrec() );
  pDumper->writeFieldValue( "Angle readout direction mode", pDb->getAngleReadoutDirectionMode() );
  pDumper->writeFieldValue( "Angle readout base angle", pDb->getAngleReadoutBaseAngle() );
  pDumper->writeFieldValue( "Angle readout clockwise", pDb->getAngleReadoutClockwiseFlag() );
  pDumper->writeFieldValue( "Tentative mode", pDb->getTentativeMode() );
  pDumper->writeFieldValue( "Tentative sub-mode", pDb->getTentativeSubMode() );
  pDumper->writeFieldValue( "Keypoint divident", pDb->getKeyPointDividend() );
  pDumper->writeFieldValue( "Default snap mode", pDb->getDefaultSnapMode() );
  pDumper->writeFieldValue( "System class", pDb->getSystemClass() );
  pDumper->writeFieldValueHex( "DMRSF flag", pDb->getDMRSFlag() );
  pDumper->writeFieldValue( "DMRS linkage generation mode", pDb->getDMRSLinkageGenerationMode() );
  pDumper->writeFieldValueHex( "Auto dimenstions flag", pDb->getAutoDimFlags() );
  pDumper->writeFieldValueHex( "Associative lock mask", pDb->getAssocLockMask() );
  pDumper->writeFieldValue( "Active cell", pDb->getActiveCell() );
  pDumper->writeFieldValue( "Active term cell", pDb->getActiveTermCell() );
  pDumper->writeFieldValue( "Active term scale", pDb->getActiveTermScale() );
  pDumper->writeFieldValue( "Active pattern cell", pDb->getActivePatternCell() );
  pDumper->writeFieldValue( "Active pattern scale", pDb->getActivePatternScale() );
  pDumper->writeFieldValue( "Active pattern angle", pDb->getActivePatternAngle() );
  pDumper->writeFieldValue( "Active pattern angle 2", pDb->getActivePatternAngle2() );
  pDumper->writeFieldValue( "Active pattern row spacing", pDb->getActivePatternRowSpacing() );
  pDumper->writeFieldValue( "Active pattern column spacing", pDb->getActivePatternColumnSpacing() );
  pDumper->writeFieldValue( "Pattern tolerance", pDb->getPatternTolerance() );
  pDumper->writeFieldValue( "Active point type", pDb->getActivePointType() );
  pDumper->writeFieldValue( "Active point symbol", pDb->getActivePointSymbol() );
  pDumper->writeFieldValue( "Active point cell", pDb->getActivePointCell() );
  pDumper->writeFieldValue( "Area pattern angle", pDb->getAreaPatternAngle() );
  pDumper->writeFieldValue( "Area pattern row spacing", pDb->getAreaPatternRowSpacing() );
  pDumper->writeFieldValue( "Area pattern column spacing", pDb->getAreaPatternColumnSpacing() );
  pDumper->writeFieldValue( "Reserved cell", pDb->getReservedCell() );
  pDumper->writeFieldValue( "Z range 2D low", pDb->getZRange2dLow() );
  pDumper->writeFieldValue( "Z range 2D high", pDb->getZRange2dHigh() );
  pDumper->writeFieldValue( "Stream delta", pDb->getStreamDelta() );
  pDumper->writeFieldValue( "Stream tolerance", pDb->getStreamTolerance() );
  pDumper->writeFieldValue( "Angle tolerance", pDb->getAngleTolerance() );
  pDumper->writeFieldValue( "Area tolerance", pDb->getAreaTolerance() );
  pDumper->writeFieldValue( "Highlight color", pDb->getHighlightColorIndex() );
  pDumper->writeFieldValue( "XOR color", pDb->getXorColorIndex() );
  pDumper->writeFieldValue( "Axis lock angle", pDb->getAxisLockAngle() );
  pDumper->writeFieldValue( "Axis lock origin", pDb->getAxisLockOrigin() );
  pDumper->writeFieldValue( "Chamfer distance 1", pDb->getChamferDist1() );
  pDumper->writeFieldValue( "Chamfer distance 2", pDb->getChamferDist2() );
  pDumper->writeFieldValue( "Autochain tolerance", pDb->getAutochainTolerance() );
  pDumper->writeFieldValue( "Consline distance", pDb->getConslineDistance() );
  pDumper->writeFieldValue( "Arc radius", pDb->getArcRadius() );
  pDumper->writeFieldValue( "Arc length", pDb->getArcLength() );
  pDumper->writeFieldValue( "Cone radius 1", pDb->getConeRadius1() );
  pDumper->writeFieldValue( "Cone radius 2", pDb->getConeRadius2() );
  pDumper->writeFieldValue( "Polygon radius", pDb->getPolygonRadius() );
  pDumper->writeFieldValue( "Surrev angle", pDb->getSurrevAngle() );
  pDumper->writeFieldValue( "Extend distance", pDb->getExtendDistance() );
  pDumper->writeFieldValue( "Fillet radius", pDb->getFilletRadius() );
  pDumper->writeFieldValue( "Coppar distance", pDb->getCopparDistance() );
  pDumper->writeFieldValue( "Array row distance", pDb->getArrayRowDistance() );
  pDumper->writeFieldValue( "Array column distance", pDb->getArrayColumnDistance() );
  pDumper->writeFieldValue( "Array fill angle", pDb->getArrayFillAngle() );
  pDumper->writeFieldValue( "Point distance", pDb->getPointDistance() );
  pDumper->writeFieldValue( "Polygon edges", pDb->getPolygonEdges() );
  pDumper->writeFieldValue( "Points between", pDb->getPointsBetween() );
  pDumper->writeFieldValue( "Array num of items", pDb->getArrayNumItems() );
  pDumper->writeFieldValue( "Array num of rows", pDb->getArrayNumRows() );
  pDumper->writeFieldValue( "Array num of columns", pDb->getArrayNumCols() );
  pDumper->writeFieldValue( "Array rotate", pDb->getArrayRotate() );
  pDumper->writeFieldValue( "B-spline order", pDb->getBSplineOrder() );
  pDumper->writeFieldValue( "Display attribute type", pDb->getDispAttrType() );
  //  pDumper->writeFieldValueHex( "Render flags", pDb->getRenderFlags() );
  pDumper->writeFieldValue( "Latitude", pDb->getLatitude() );
  pDumper->writeFieldValue( "Longitude", pDb->getLongitude() );
  pDumper->writeFieldValue( "Solar time", pDb->getSolarTime() );
  pDumper->writeFieldValue( "Solar year", pDb->getSolarYear() );
  pDumper->writeFieldValue( "GMT offset", pDb->getGMTOffset() );
  pDumper->writeFieldValue( "Solar direction", pDb->getSolarDirection() );
  pDumper->writeFieldValue( "Solar vector override", pDb->getSolarVectorOverride() );
  pDumper->writeFieldValue( "Solar intensity", pDb->getSolarIntensity() );
  pDumper->writeFieldValue( "Ambient intensity", pDb->getAmbientIntensity() );
  pDumper->writeFieldValue( "Flash intensity", pDb->getFlashIntensity() );
  pDumper->writeFieldValue( "Near depth density", pDb->getNearDepthDensity() );
  pDumper->writeFieldValue( "Far depth density", pDb->getFarDepthDensity() );
  pDumper->writeFieldValue( "Near depth distance", pDb->getNearDepthDistance() );
  pDumper->writeFieldValue( "Haze color", pDb->getHazeColor() );
  pDumper->writeFieldValue( "Shadow tolerance", pDb->getShadowTolerance() );
  pDumper->writeFieldValue( "Stroke tolerance", pDb->getStrokeTolerance() );
  pDumper->writeFieldValue( "Max polygon size", pDb->getMaxPolygonSize() );
  pDumper->writeFieldValue( "Arc minimum", pDb->getArcMinimum() );
  pDumper->writeFieldValue( "Exact Hline accuracy", pDb->getExactHLineAccuracy() );
  pDumper->writeFieldValue( "Exact Hline tolerance", pDb->getExactHLineTolerance() );
  pDumper->writeFieldValue( "Selection Highlight override", pDb->getSelectionHighlightOverride() );
  pDumper->writeFieldValue( "Selection Highlight color", pDb->getSelectionHighlightColor() );
  pDumper->writeFieldValue( "Cell filename", pDb->getCellFileName() );
  pDumper->writeFieldValue( "Background file", pDb->getBackgroundFile() );
  //pDumper->writeFieldValue( "Default model is 3D", pDb->getDefaultModelIs3D() );
  //pDumper->writeFieldValue( "Version", pDb->getVersion() );
  //pDumper->writeFieldValue( "Sub version", pDb->getSubVersion() );
  //pDumper->writeFieldValue( "Format", pDb->getFormat() );
  //pDumper->writeFieldValue( "Highest model ID", pDb->getHighestModelID() );
  pDumper->writeFieldValue( "Handseed", pDb->getHandseed() );
  pDumper->writeFieldValue( "Last saved time", pDb->getLastSaveTime() );
  pDumper->writeFieldValue( "Next graphics group", pDb->getNextGraphicGroup() );
  //pDumper->writeFieldValue( "Next text node", pDb->getNextTextNode() );
  //pDumper->writeFieldValue( "Original format", pDb->getOriginalFormat() );
  //pDumper->writeFieldValue( "Number of model specific digital signatures", pDb->getModelSpecificDigitalSignatures() );
  //pDumper->writeFieldValue( "Number of file-wide digital signatures", pDb->getFileWideDigitalSignatures() );
  //pDumper->writeFieldValue( "Primary application ID", pDb->getPrimaryApplicationID() );
  pDumper->writeFieldValue( "Is persistent", pDb->isPersistent() );

  OdDgElementId modelId = pDb->getDefaultModelId();
  if (!modelId.isNull())
  {
    OdDgModelPtr pModel = modelId.safeOpenObject();
    if (!pModel.isNull())
    {
      pDumper->writeFieldValue( "Default Model Name", pModel->getName() );
    }
  }

  OdDgDatabaseLockFlagsPEPtr pDatabaseLockFlags = pDb;

  pDumper->dumpFieldName("Lock Flags:");
  pDumper->writeFieldValue( " Solid Hole Flag", pDatabaseLockFlags->getSolidHoleFlag(pDb) );
  pDumper->writeFieldValue( " Scale Lock Flag", pDatabaseLockFlags->getScaleLockFlag(pDb) );
  pDumper->writeFieldValue( " Text Node Lock Flag", pDatabaseLockFlags->getTextNodeLockFlag(pDb) );
  pDumper->writeFieldValue( " Angle Lock Flag", pDatabaseLockFlags->getAngleLockFlag(pDb) );
  pDumper->writeFieldValue( " Snap Lock Flag", pDatabaseLockFlags->getSnapLockFlag(pDb) );
  pDumper->writeFieldValue( " Mirror Characters Flag", pDatabaseLockFlags->getMirrorCharactersFlag(pDb) );
  pDumper->writeFieldValue( " Fence Clip Flag", pDatabaseLockFlags->getFenceClipFlag(pDb) );
  pDumper->writeFieldValue( " Graphics Group Lock Flag", pDatabaseLockFlags->getGraphicsGroupLockFlag(pDb) );
  pDumper->writeFieldValue( " Level Lock Flag", pDatabaseLockFlags->getLevelLockFlag(pDb) );
  pDumper->writeFieldValue( " Fence Overlap Flag", pDatabaseLockFlags->getFenceOverlapFlag(pDb) );
  pDumper->writeFieldValue( " Axis Lock Flag", pDatabaseLockFlags->getAxisLockFlag(pDb) );
  pDumper->writeFieldValue( " Auxiliary Input Flag", pDatabaseLockFlags->getAuxiliaryInputFlag(pDb) );
  pDumper->writeFieldValue( " Auto Pan Flag", pDatabaseLockFlags->getAutoPanFlag(pDb) );
  pDumper->writeFieldValue( " Cell Stretch Flag", pDatabaseLockFlags->getCellStretchFlag(pDb) );
  pDumper->writeFieldValue( " Selection Set Flag", pDatabaseLockFlags->getSelectionSetFlag(pDb) );
  pDumper->writeFieldValue( " Auto Handles Flag", pDatabaseLockFlags->getAutoHandlesFlag(pDb) );
  pDumper->writeFieldValue( " Single Shot Mode Flag", pDatabaseLockFlags->getSingleShotModeFlag(pDb) );
  pDumper->writeFieldValue( " Dont Restart Flag", pDatabaseLockFlags->getDontRestartFlag(pDb) );
  pDumper->writeFieldValue( " View Single Shot Flag", pDatabaseLockFlags->getViewSingleShotFlag(pDb) );
  pDumper->writeFieldValue( " Snap Construction Plane Flag", pDatabaseLockFlags->getSnapConstructionPlaneFlag(pDb) );
  pDumper->writeFieldValue( " Construct Plane Perp Flag", pDatabaseLockFlags->getConstructPlanePerpFlag(pDb) );
  pDumper->writeFieldValue( " Intersect Snap Flag", pDatabaseLockFlags->getIntersectSnapFlag(pDb) );
  pDumper->writeFieldValue( " Association Flag", pDatabaseLockFlags->getAssociationFlag(pDb) );
  pDumper->writeFieldValue( " Shared Cells Flag", pDatabaseLockFlags->getSharedCellsFlag(pDb) );
  pDumper->writeFieldValue( " Fence Void Flag", pDatabaseLockFlags->getFenceVoidFlag(pDb) );
  pDumper->writeFieldValue( " Snappable Patterns Flag", pDatabaseLockFlags->getSnappablePatternsFlag(pDb) );
  pDumper->writeFieldValue( " Depth Lock Flag", pDatabaseLockFlags->getDepthLockFlag(pDb) );
  pDumper->writeFieldValue( " Associative Pattern Flag", pDatabaseLockFlags->getAssociativePatternFlag(pDb) );
  pDumper->writeFieldValue( " True Scale Cells Flag", pDatabaseLockFlags->getTrueScaleCellsFlag(pDb) );
  pDumper->writeFieldValue( " Ignore Ref Overrides Flag", pDatabaseLockFlags->getIgnoreRefOverridesFlag(pDb) );
  pDumper->writeFieldValue( " Scale Viewport LStyle Flag", pDatabaseLockFlags->getScaleViewportLStyleFlag(pDb) );
  pDumper->writeFieldValue( " Display Ref Boundaries Flag", pDatabaseLockFlags->getDisplayRefBoundariesFlag(pDb) );
  pDumper->writeFieldValue( " Continuous Line Styles Flag", pDatabaseLockFlags->getContinuousLineStylesFlag(pDb) );
  pDumper->writeFieldValue( " Fill Mode", pDatabaseLockFlags->getFillMode(pDb) );
}

void OdDgDatabaseDumperPE::dumpLinkages( OdRxObjectPtr /*pObj*/, OdExDgnDumper* /*pDumper*/) const
{
}

void OdDgDatabaseDumperPE::dumpXAttributes( OdRxObjectPtr /*pObj*/, OdExDgnDumper* /*pDumper*/) const
{
}

//----------------------------------------------------------
//
// OdDgModelDumperPE
//
//----------------------------------------------------------

static OdString toStr(OdDgModel::UnitSystem us)
{
  switch(us)
  {
    case OdDgModel::kCustom : return "kCustom";
    case OdDgModel::kEnglish: return "kEnglish";
    case OdDgModel::kMetric :return "kMetric";
  }
  return "ERROR";
}

void OdDgModelDumperPE::dumpData( OdRxObjectPtr pObj, OdExDgnDumper* pDumper ) const
{
  OdDgElementDumperPE::dumpData(pObj, pDumper);

  OdDgModelPtr element = pObj;

  pDumper->writeFieldValue(L"Model Type", element->getType() );
  pDumper->writeFieldValue(L"Model Cell Type", element->getCellType() );

  //storage unit
  {
    OdDgModel::StorageUnitDescription description;
    element->getStorageUnit( description );

    OdString unitName = L"Storage unit ";
    pDumper->writeFieldValue( unitName + "Base", OdString( description.m_base == OdDgModel::kMeter ? "kMeter" : "kNone" ) );
    pDumper->writeFieldValue( unitName + "System", toStr(description.m_system) );
    pDumper->writeFieldValue( unitName + "Nominator", description.m_numerator );
    pDumper->writeFieldValue( unitName + "Denominator", description.m_denominator );
    pDumper->writeFieldValue( unitName + "UORs per Storage", description.m_uorPerStorageUnit );
  }

  //master unit
  {
    OdDgModel::UnitDescription description;
    element->getMasterUnit( description );

    OdString unitName = L"Master unit ";
    pDumper->writeFieldValue( unitName + "Base", OdString( description.m_base == OdDgModel::kMeter ? "kMeter" : "kNone" ) );
    pDumper->writeFieldValue( unitName + "System", toStr(description.m_system) );
    pDumper->writeFieldValue( unitName + "Nominator", description.m_numerator );
    pDumper->writeFieldValue( unitName + "Denominator", description.m_denominator );
    pDumper->writeFieldValue( unitName + "Name", description.m_name );
  }

  //sub unit
  {
    OdDgModel::UnitDescription description;
    element->getSubUnit( description );

    OdString unitName = L"Sub unit ";
    pDumper->writeFieldValue( unitName + "Base", OdString( description.m_base == OdDgModel::kMeter ? "kMeter" : "kNone" ) );
    pDumper->writeFieldValue( unitName + "System", toStr(description.m_system) );
    pDumper->writeFieldValue( unitName + "Nominator", description.m_numerator );
    pDumper->writeFieldValue( unitName + "Denominator", description.m_denominator );
    pDumper->writeFieldValue( unitName + "Name", description.m_name );
  }

  pDumper->writeFieldValue( "Working unit", element->getWorkingUnit() );
  pDumper->writeFieldValue( "UORs to Working Scale", element->convertUORsToWorkingUnits(1.0));
  pDumper->writeFieldValue( "Global Origin", element->getGlobalOrigin() );

  double dSolidExtent = element->getSolidExtent();

  if( OdZero(dSolidExtent) )
    dSolidExtent = 4294967295.0;

  pDumper->writeFieldValue( "Solid Extent", dSolidExtent );

  {
    OdGeExtents3d extent;

    if( element->getGeomExtents( extent ) == eOk )
    {
      pDumper->writeFieldValue( "Extent", extent );
    }
    else
    {
      pDumper->writeFieldValue( "Extent", "invalid value" );
    }
  }

  // display grid

  pDumper->writeFieldValue( "Grid Master",    element->getGridMaster() );
  pDumper->writeFieldValue( "Grid Reference", element->getGridReference() );
  pDumper->writeFieldValue( "Grid Aspect",    element->getGridRatio() );
  pDumper->writeFieldValue( "Grid Angle",     element->getGridAngle() );
  pDumper->writeFieldValue( "Grid Origin Base", element->getGridBase() );
  pDumper->writeFieldValue( "Grid Isometric", element->getIsometricGridFlag() );
  pDumper->writeFieldValue( "Grid Orientation", element->getGridOrientation() );

  // ACS

  pDumper->writeFieldValue( "ACS Type",   element->getAcsType() );
  pDumper->writeFieldValue( "ACS Origin", element->getAcsOrigin() );
  pDumper->writeFieldValue( "ACS Rotation", element->getAcsRotation() );
  pDumper->writeFieldValueHex( "ACS ElementId", element->getAcsElementId().getHandle() );
}

//----------------------------------------------------------
//
// Tables
//
//----------------------------------------------------------
void OdDgTableRecordDumperPE::dumpData( OdRxObjectPtr pObj, OdExDgnDumper* pDumper ) const
{
  OdDgElementDumperPE::dumpData(pObj, pDumper);

  OdDgTableRecordPtr pRec = pObj;
  pDumper->writeFieldValue( "Name ", pRec->getName() );
}

void OdDgLevelTableRecordDumperPE::dumpData( OdRxObjectPtr pObj, OdExDgnDumper* pDumper ) const
{
  OdDgTableRecordDumperPE::dumpData(pObj, pDumper);

  OdDgLevelTableRecordPtr pLevel = pObj;

  pDumper->writeFieldValue( "Level Number", pLevel->getNumber() );
  pDumper->writeFieldValue( "Entry ID", pLevel->getEntryId() );
  pDumper->writeFieldValue( "Code", pLevel->getNumber() );

  pDumper->writeFieldValueHex( "Element Color (byLevel)", pLevel->getElementColorIndex() );
  pDumper->writeFieldValueHex( "Element LineStyle (byLevel)", pLevel->getElementLineStyleEntryId() );
  pDumper->writeFieldValue( "Element LineWeight (byLevel)", pLevel->getElementLineWeight() );

  pDumper->writeFieldValueHex( "Override Color", pLevel->getOverrideColorIndex() );
  pDumper->writeFieldValueHex( "Override LineStyle", pLevel->getOverrideLineStyleEntryId() );
  pDumper->writeFieldValue( "Override LineWeight", pLevel->getOverrideLineWeight() );
  pDumper->writeFieldValue( "Override Material", pLevel->getOverrideMaterial() );

  pDumper->writeFieldValue( "Use override color", pLevel->getUseOverrideColorFlag() );
  pDumper->writeFieldValue( "Use override line style", pLevel->getUseOverrideLineStyleFlag() );
  pDumper->writeFieldValue( "Use override line weight", pLevel->getUseOverrideLineWeightFlag() );

  pDumper->writeFieldValue( "Displayed", pLevel->getIsDisplayedFlag() );
  pDumper->writeFieldValue( "Can be Plotted", pLevel->getIsPlotFlag() );
  pDumper->writeFieldValue( "Derived from a library level ", pLevel->getIsExternalFlag() );
  pDumper->writeFieldValue( "Can be Snapped ", pLevel->getIsSnapFlag() );
  pDumper->writeFieldValue( "ReadOnly", pLevel->getIsReadOnlyFlag() );
  pDumper->writeFieldValue( "Hidden", pLevel->getIsHiddenFlag() );
  pDumper->writeFieldValue( "Frozen", pLevel->getIsFrozenFlag() );
  pDumper->writeFieldValue( "CustomStyleFromMaster", pLevel->getIsCustomStyleFromMasterFlag() );
  pDumper->writeFieldValue( "Displayed", pLevel->getIsDisplayedFlag() );

  OdUInt32 elementAccess = pLevel->getElementAccess();
  pDumper->writeFieldValueHex( "Element access", elementAccess );

  pDumper->writeFieldValue( "Description", pLevel->getDescription() );
  pDumper->writeFieldValue( "Priority", pLevel->getPriority() );
  pDumper->writeFieldValue( "Transparency", 1.0 -  pLevel->getTransparency().alphaPercent() );

  if( !pLevel->getOverrideMaterial().isNull() )
  {
    pDumper->writeFieldValueHex( "Override Material Id", pLevel->getOverrideMaterial().getHandle() );
    pDumper->writeFieldValue( "Override Material Palette", pLevel->getOverrideMaterialPalette() );
    pDumper->writeFieldValue( "Override Material Name", pLevel->getOverrideMaterialName() );
  }

  if( !pLevel->getByLevelMaterial().isNull() )
  {
    pDumper->writeFieldValueHex( "ByLevel Material Id", pLevel->getByLevelMaterial().getHandle() );
    pDumper->writeFieldValue( "ByLevel Material Palette", pLevel->getByLevelMaterialPalette() );
    pDumper->writeFieldValue( "ByLevel Material Name", pLevel->getByLevelMaterialName() );
  }

  if( pLevel->getAssignedMaterialsCount() > 0 )
  {
    pDumper->dumpFieldName(L"Assigned materials:");

    for( OdUInt32 k = 0; k < pLevel->getAssignedMaterialsCount(); k++ )
    {
      OdDgAssignedMaterial matPair;
      matPair = pLevel->getAssignedMaterial(k);
      pDumper->writeFieldValue( L"  " +  matPair.m_strMaterialName, matPair.m_uColorIndex );
    }
  }
}

void OdDgLevelFilterTableRecordDumperPE::dumpData( OdRxObjectPtr pObj, OdExDgnDumper* pDumper ) const
{
  OdDgTableRecordDumperPE::dumpData(pObj, pDumper);

  OdDgLevelFilterTableRecordPtr pFilter = pObj;

  pDumper->writeFieldValue( "Entry ID", pFilter->getEntryId() );
  pDumper->writeFieldValue( "Parent ID", (OdInt32)(pFilter->getParentId()) );
  pDumper->writeFieldValue( "Filter Type", pFilter->getFilterType() );
  pDumper->writeFieldValue( "Compose Or Flag", pFilter->getComposeOrFlag() );
  pDumper->dumpFieldName("Filter Expressions:");

  bool bAddData = false;

  for( OdUInt32 i = 0 ; i < pFilter->getFilterMemberCount(); i++ )
  {
    OdString strMemberName       = pFilter->getFilterMemberName(i);
    OdString strMemberExpression = pFilter->getFilterMemberExpression(i);

    if( !strMemberExpression.isEmpty() )
    {
      strMemberName = OD_T("  ") + strMemberName;

      pDumper->writeFieldValue( strMemberName, strMemberExpression );
      bAddData = true;
    }
  }

  if( !bAddData )
  {
    pDumper->dumpFieldValue("-------");
  }
}

struct OdDgMaterialTableDumperAssignIndexPair
{
  OdUInt32 m_uMin;
  OdUInt32 m_uMax;
};

void OdDgMaterialTableDumperPE::dumpData( OdRxObjectPtr pObj, OdExDgnDumper* pDumper ) const
{
  OdDgTableDumperPE::dumpData( pObj, pDumper );

  OdDgMaterialTablePtr pTable = pObj;
  OdDgMaterialAssignmentElementPEPtr pTablePE = pTable;

  if( pTablePE->getMaterialAssignmentCount(pTable) > 0 )
    pDumper->dumpFieldName(L"Material assignments:");

  for( OdUInt32 i = 0; i < pTablePE->getMaterialAssignmentCount(pTable); i++ )
  {
    OdDgMaterialAssignment matAssign = pTablePE->getMaterialAssignment(pTable, i);

    OdString strName  = L"- " + matAssign.getMaterialName();
    
    for( OdUInt32 j = 0; j < matAssign.getLevelAssignmentCount(); j++ )
    {
      OdString strValue;

      OdArray<OdDgMaterialTableDumperAssignIndexPair> arrPairs;
      OdDgMaterialTableDumperAssignIndexPair          pair;
      bool                                            bStartPair = false;
      OdUInt32 k;

      for( k = 0; k < 256; k++ )
      {
        if( !bStartPair && matAssign.getLevelAssignment(j).getColorAssignment(k) )
        {
          bStartPair = true;
          pair.m_uMin = k;
        }
        else if( !matAssign.getLevelAssignment(j).getColorAssignment(k) )
        {
          if( bStartPair )
          {
            pair.m_uMax = k - 1;
            arrPairs.push_back( pair );
          }
          bStartPair = false;
        }
      }

      if( bStartPair )
      {
        pair.m_uMax = 255;
        arrPairs.push_back( pair );
      }

      strValue = matAssign.getLevelAssignment(j).getLevelName() + L" : (";

      for( k = 0; k < arrPairs.size(); k++ )
      {
        OdString strData;

        if( arrPairs[k].m_uMin == arrPairs[k].m_uMax )
          strData.format(L"%d,",arrPairs[k].m_uMin);
        else
          strData.format(L"%d-%d,",arrPairs[k].m_uMin, arrPairs[k].m_uMax);

        strValue += strData;
      }

      strValue = strValue.left( strValue.getLength() - 1 );
      strValue += L")";

      pDumper->writeFieldValue( strName, strValue );
    }
  }
}

void OdDgLineStyleTableDumperPE::dumpData( OdRxObjectPtr pObj, OdExDgnDumper* pDumper ) const
{
  OdDgTableDumperPE::dumpData( pObj, pDumper );

  OdDgLineStyleTablePtr pTable = pObj;
  pDumper->dumpFieldName(L"Available rsc line styles:");

  for( OdUInt32 i = 0; i < pTable->getRscLineStyleCount(); i++ )
  {
    OdDgLineStyleTableRecordPtr pLineStyle = pTable->getRscLineStyle(i);

    if( !pLineStyle.isNull() )
    {
      OdString strName = pLineStyle->getName();

      strName = L" " + strName;

      pDumper->writeFieldValueHex(strName, pLineStyle->getEntryId());
    }
  }
}

void OdDgLevelFilterTableDumperPE::dumpData( OdRxObjectPtr pObj, OdExDgnDumper* pDumper ) const
{
  OdDgTableDumperPE::dumpData( pObj, pDumper );

  OdDgLevelFilterTablePtr pTable = pObj;
  pDumper->writeFieldValue( "Active Filter ID", (OdInt32)(pTable->getActiveFilterId()) );
  pDumper->dumpFieldName( L"Filter Members :" );

  for( OdUInt32 i = 0; i < pTable->getFilterMemberCount(); i++ )
  {
    OdString strMemberName;
    OdDgLevelFilterTable::OdDgFilterMemberType iMemberType;

    if( pTable->getFilterMember(i, strMemberName, iMemberType ) )
    {
      strMemberName = OD_T("  ") + strMemberName;
      pDumper->writeFieldValue( strMemberName, iMemberType );
    }
  }
}

void OdDgFontTableDumperPE::dumpData( OdRxObjectPtr pObj, OdExDgnDumper* pDumper ) const
{
  OdDgTableDumperPE::dumpData( pObj, pDumper );

  OdDgFontTablePtr pTable = pObj;

  pDumper->writeFieldValue( L"-----------------", OdString(L"------------------"));
  pDumper->writeFieldValue( L"Rsc Font Id", OdString(L"Rsc Font Name"));
  pDumper->writeFieldValue( L"-----------------", OdString(L"------------------"));

  for( OdUInt32 i = 0; i <= 255; i++ )
  {
    OdDgFontTableRecordPtr pRec = pTable->getFont(i);

    if( !pRec.isNull() )
    {
      OdString strNumber;
      strNumber.format(L"  %d", i);
      pDumper->writeFieldValue( strNumber, pRec->getName() );
    }
  }
}


void OdDgFontTableRecordDumperPE::dumpData( OdRxObjectPtr pObj, OdExDgnDumper* pDumper ) const
{
  OdDgTableRecordDumperPE::dumpData(pObj, pDumper);

  OdDgFontTableRecordPtr pFont = pObj;

  pDumper->writeFieldValue( "Font number", pFont->getNumber() );
  pDumper->writeFieldValue( "Entry ID", pFont->getNumber() );
  pDumper->writeFieldValue( "Alternate Font name", pFont->getAlternateName() );
}

void OdDgTextStyleTableRecordDumperPE::dumpData( OdRxObjectPtr pObj, OdExDgnDumper* pDumper ) const
{
  OdDgTableRecordDumperPE::dumpData(pObj, pDumper);

  OdDgTextStyleTableRecordPtr element = pObj;

  pDumper->writeFieldValue( "Entry ID", element->getEntryId() );

  pDumper->writeFieldValue( "Font number", element->getFontEntryId() );
  // Gets Font name
  OdDgFontTablePtr pFontTable = element->database()->getFontTable();
  OdDgFontTableRecordPtr pFont = pFontTable->getFont(element->getFontEntryId());
  if (!pFont.isNull())
  {
    pDumper->writeFieldValue( "Font Name", pFont->getName() );
  }

  pDumper->writeFieldValue( "Justification", element->getJustification() );
  pDumper->writeFieldValue( "Line length", element->getLineLength() );
  pDumper->writeFieldValue( "Line offset", element->getLineOffset() );
  pDumper->writeFieldValue( "Line spacing", element->getLineSpacing() );
  pDumper->writeFieldValue( "Font number", element->getFontEntryId() );
  pDumper->writeFieldValue( "Interchar spacing", element->getInterCharacter() );
  pDumper->writeFieldValue( "Parent text style ID", element->getParentTextStyleEntryId() );
  pDumper->writeFieldValue( "Slant", element->getSlant() );
  pDumper->writeFieldValue( "Direction", element->getTextDirection() );
  pDumper->writeFieldValue( "Height", element->getTextHeight() );
  pDumper->writeFieldValue( "Width", element->getTextWidth() );
  pDumper->writeFieldValue( "Node justification", element->getTextNodeJustification() );
  //writeFieldValue( "Is from table", element->getIsFromTableFlag() );

  pDumper->writeFieldValue( "Underline", element->getUnderlineFlag() );
  pDumper->writeFieldValue( "Overline", element->getOverlineFlag() );
  pDumper->writeFieldValue( "Italic", element->getItalicsFlag() );
  pDumper->writeFieldValue( "Bold", element->getBoldFlag() );
  pDumper->writeFieldValue( "Superscript", element->getSuperscriptFlag() );
  pDumper->writeFieldValue( "Subscript", element->getSubscriptFlag() );
  pDumper->writeFieldValue( "Background flag", element->getBackgroundFlag() );
  pDumper->writeFieldValue( "OverlineStyle", element->getOverlineStyleFlag() );
  pDumper->writeFieldValue( "UnderlineStyle", element->getUnderlineStyleFlag() );
  pDumper->writeFieldValue( "Fixed spacing", element->getFixedSpacingFlag() );
  pDumper->writeFieldValue( "Fractions", element->getFractionsFlag() );
  pDumper->writeFieldValue( "Color flag", element->getColorFlag() );
  pDumper->writeFieldValue( "AcadIntercharSpacingFlag", element->getAcadIntercharSpacingFlag() );
  pDumper->writeFieldValue( "FullJustificationFlag", element->getFullJustificationFlag() );
  pDumper->writeFieldValue( "AcadShapeFileFlag", element->getAcadShapeFileFlag() );

  pDumper->writeFieldValue( "Background border", element->getBackgroundBorder() );
  pDumper->writeFieldValue( "Background fill color index", element->getBackgroundFillColorIndex() );
  pDumper->writeFieldValue( "Background line color index", element->getBackgroundColorIndex() );
  pDumper->writeFieldValue( "Background line style entry id", element->getBackgroundLineStyleEntryId() );
  pDumper->writeFieldValue( "Background line weight", element->getBackgroundLineWeight() );
  pDumper->writeFieldValue( "Overline line color", element->getOverlineColorIndex() );
  pDumper->writeFieldValue( "Overline line style ID", element->getOverlineLineStyleEntryId() );
  pDumper->writeFieldValue( "Overline line weight", element->getOverlineLineWeight() );
  pDumper->writeFieldValue( "Overline offset", element->getOverlineOffset() );
  pDumper->writeFieldValue( "Underline line color", element->getUnderlineColorIndex() );
  pDumper->writeFieldValue( "Underline line style ID", element->getUnderlineLineStyleEntryId() );
  pDumper->writeFieldValue( "Underline line weight", element->getUnderlineLineWeight() );
  pDumper->writeFieldValue( "Underline offset", element->getUnderlineOffset() );
  pDumper->writeFieldValue( "LineSpacinType offset", element->getLineSpacingType() );
  pDumper->writeFieldValue( "Color index", element->getColorIndex() );

  pDumper->dumpFieldName("Override flags:");
  pDumper->writeFieldValue(" - FontNumber Override Flag", element->getFontNumberOverrideFlag());
  pDumper->writeFieldValue(" - ShxBigFont Override Flag", element->getShxBigFontOverrideFlag());
  pDumper->writeFieldValue(" - Width Override Flag", element->getWidthOverrideFlag());
  pDumper->writeFieldValue(" - Height Override Flag", element->getHeightOverrideFlag());
  pDumper->writeFieldValue(" - Slant Override Flag", element->getSlantOverrideFlag());
  pDumper->writeFieldValue(" - LineSpacing Override Flag", element->getLineSpacingOverrideFlag());
  pDumper->writeFieldValue(" - LineCharSpacing Override Flag", element->getLineCharSpacingOverrideFlag());
  pDumper->writeFieldValue(" - UnderlineOffset Override Flag", element->getUnderlineOffsetOverrideFlag());
  pDumper->writeFieldValue(" - OverlineOffset Override Flag", element->getOverlineOffsetOverrideFlag());
  pDumper->writeFieldValue(" - Justification Override Flag", element->getJustificationOverrideFlag());
  pDumper->writeFieldValue(" - NodeJustification Override Flag", element->getNodeJustificationOverrideFlag());
  pDumper->writeFieldValue(" - LineLength Override Flag", element->getLineLengthOverrideFlag());
  pDumper->writeFieldValue(" - Direction Override Flag", element->getDirectionOverrideFlag());
  pDumper->writeFieldValue(" - Underline Override Flag", element->getUnderlineOverrideFlag());
  pDumper->writeFieldValue(" - Overline Override Flag", element->getOverlineOverrideFlag());
  pDumper->writeFieldValue(" - Italics Override Flag", element->getItalicsOverrideFlag());
  pDumper->writeFieldValue(" - Bold Override Flag", element->getBoldOverrideFlag());
  pDumper->writeFieldValue(" - Superscript Override Flag", element->getSuperscriptOverrideFlag());
  pDumper->writeFieldValue(" - Subscript Override Flag", element->getSubscriptOverrideFlag());
  pDumper->writeFieldValue(" - FixedSpacing Override Flag", element->getFixedSpacingOverrideFlag());
  pDumper->writeFieldValue(" - Background Override Flag", element->getBackgroundOverrideFlag());
  pDumper->writeFieldValue(" - BackgroundStyle Override Flag", element->getBackgroundStyleOverrideFlag());
  pDumper->writeFieldValue(" - BackgroundWeight Override Flag", element->getBackgroundWeightOverrideFlag());
  pDumper->writeFieldValue(" - BackgroundColor Override Flag", element->getBackgroundColorOverrideFlag());
  pDumper->writeFieldValue(" - BackgroundFillColor Override Flag", element->getBackgroundFillColorOverrideFlag());
  pDumper->writeFieldValue(" - BackgroundBorder Override Flag", element->getBackgroundBorderOverrideFlag());
  pDumper->writeFieldValue(" - UnderlineStyle Override Flag", element->getUnderlineStyleOverrideFlag());
  pDumper->writeFieldValue(" - UnderlineWeight Override Flag", element->getUnderlineWeightOverrideFlag());
  pDumper->writeFieldValue(" - UnderlineColor Override Flag", element->getUnderlineColorOverrideFlag());
  pDumper->writeFieldValue(" - OverlineStyle Override Flag", element->getOverlineStyleOverrideFlag());
  pDumper->writeFieldValue(" - OverlineWeight Override Flag", element->getOverlineWeightOverrideFlag());
  pDumper->writeFieldValue(" - OverlineColor Override Flag", element->getOverlineColorOverrideFlag());
  pDumper->writeFieldValue(" - LineOffset Override Flag", element->getLineOffsetOverrideFlag());
  pDumper->writeFieldValue(" - Fractions Override Flag", element->getFractionsOverrideFlag());
  pDumper->writeFieldValue(" - OverlineStyleFlag Override Flag", element->getOverlineStyleFlagOverrideFlag());
  pDumper->writeFieldValue(" - UnderlineStyleFlag Override Flag", element->getUnderlineStyleFlagOverrideFlag());
  pDumper->writeFieldValue(" - Color Override Flag", element->getColorOverrideFlag());
  pDumper->writeFieldValue(" - WidthFactor Override Flag", element->getWidthFactorOverrideFlag());
  pDumper->writeFieldValue(" - ColorFlag Override Flag", element->getColorFlagOverrideFlag());
  pDumper->writeFieldValue(" - FullJustification Override Flag", element->getFullJustificationOverrideFlag());
  pDumper->writeFieldValue(" - LineSpacingType Override Flag", element->getLineSpacingTypeOverrideFlag());
  pDumper->writeFieldValue(" - Backwards Override Flag", element->getBackwardsOverrideFlag());
  pDumper->writeFieldValue(" - Upsidedown Override Flag", element->getUpsidedownOverrideFlag());
  pDumper->writeFieldValue(" - AcadInterCharSpacing Override Flag", element->getAcadInterCharSpacingOverrideFlag());
}

static void getToolTypeAndDescriptionByToolIndex( OdUInt32 iIndex, OdDgDimension::ToolType& iTool, OdString& strToolName )
{
  switch( iIndex )
  {
    case 0:
    {
      iTool = OdDgDimension::kToolTypeSizeArrow;
      strToolName = OD_T("  Tool type \"kToolTypeSizeArrow\"");
    } break;

    case 1:
    {
      iTool = OdDgDimension::kToolTypeSizeStroke;
      strToolName = OD_T("  Tool type \"kToolTypeSizeStroke\"");
    } break;

    case 2:
    {
      iTool = OdDgDimension::kToolTypeLocateSingle;
      strToolName = OD_T("  Tool type \"kToolTypeLocateSingle\"");
    } break;

    case 3:
    {
      iTool = OdDgDimension::kToolTypeLocateStacked;
      strToolName = OD_T("  Tool type \"kToolTypeLocateStacked\"");
    } break;

    case 4:
    {
      iTool = OdDgDimension::kToolTypeAngleSize;
      strToolName = OD_T("  Tool type \"kToolTypeAngleSize\"");
    } break;

    case 5:
    {
      iTool = OdDgDimension::kToolTypeArcSize;
      strToolName = OD_T("  Tool type \"kToolTypeArcSize\"");
    } break;

    case 6:
    {
      iTool = OdDgDimension::kToolTypeAngleLocation;
      strToolName = OD_T("  Tool type \"kToolTypeAngleLocation\"");
    } break;

    case 7:
    {
      iTool = OdDgDimension::kToolTypeArcLocation;
      strToolName = OD_T("  Tool type \"kToolTypeArcLocation\"");
    } break;

    case 8:
    {
      iTool = OdDgDimension::kToolTypeAngleLines;
      strToolName = OD_T("  Tool type \"kToolTypeAngleLines\"");
    } break;

    case 9:
    {
      iTool = OdDgDimension::kToolTypeAngleAxis;
      strToolName = OD_T("  Tool type \"kToolTypeAngleAxis\" ( X and Y )");
    } break;

    case 10:
    {
      iTool = OdDgDimension::kToolTypeRadius;
      strToolName = OD_T("  Tool type \"kToolTypeRadius\"");
    } break;

    case 11:
    {
      iTool = OdDgDimension::kToolTypeRadiusExtended;
      strToolName = OD_T("  Tool type \"kToolTypeRadiusExtended\"");
    } break;

    case 12:
    {
      iTool = OdDgDimension::kToolTypeDiameter;
      strToolName = OD_T("  Tool type \"kToolTypeDiameter\"");
    } break;

    case 13:
    {
      iTool = OdDgDimension::kToolTypeDiameterExtended;
      strToolName = OD_T("  Tool type \"kToolTypeDiameterExtended\"");
    } break;

    case 14:
    {
      iTool = OdDgDimension::kToolTypeDiameterPara;
      strToolName = OD_T("  Tool type \"kToolTypeDiameterPara\"");
    } break;

    case 15:
    {
      iTool = OdDgDimension::kToolTypeDiameterPerp;
      strToolName = OD_T("  Tool type \"kToolTypeDiameterPerp\"");
    } break;

    case 16:
    {
      iTool = OdDgDimension::kToolTypeCustomLinear;
      strToolName = OD_T("  Tool type \"kToolTypeCustomLinear\"");
    } break;

    case 17:
    {
      iTool = OdDgDimension::kToolTypeOrdinate;
      strToolName = OD_T("  Tool type \"kToolTypeOrdinate\"");
    } break;

    case 18:
    {
      iTool = OdDgDimension::kToolTypeCenter;
      strToolName = OD_T("  Tool type \"kToolTypeCenter\"");
    } break;

    case 19:
    {
      iTool = OdDgDimension::kToolTypeLabelLine;
      strToolName = OD_T("  Tool type \"kToolTypeLabelLine\"");
    } break;

    case 20:
    {
      iTool = OdDgDimension::kToolTypeNote;
      strToolName = OD_T("  Tool type \"kToolTypeNote\"");
    } break;

    default:
    {
      iTool = OdDgDimension::kToolTypeInvalid;
      strToolName = OD_T("  Invalid Tool type");
    }
  }
}

void OdDgDimStyleTableRecordDumperPE::dumpData( OdRxObjectPtr pObj, OdExDgnDumper* pDumper ) const
{
  OdDgTableRecordDumperPE::dumpData(pObj, pDumper);

  OdDgDimStyleTableRecordPtr element = pObj;

  pDumper->writeFieldValue( "Description", element->getDescription() );
  pDumper->writeFieldValue( "Entry ID", element->getEntryId() );
  pDumper->writeFieldValue( "Parent ID", element->getParentDimensionStyleEntryId() );
  pDumper->writeFieldValue( "Level ID", element->getLevel());
  pDumper->writeFieldValue( "Text Style ID", element->getTextStyleEntryId());
  pDumper->writeFieldValue( "Primary Accuracy", element->getPrimaryAccuracy());
  pDumper->writeFieldValue( "Secondary Accuracy", element->getSecondaryAccuracy());
  pDumper->dumpFieldName("Mode Flags:");
  pDumper->writeFieldValue( "  Placement alignment", element->getPlacementAlignment() );
  pDumper->writeFieldValue( "  Manual placement location", element->getManualLocationFlag() );
  pDumper->writeFieldValue( "  Extension lines present", element->getExtensionLinesPresentFlag() );
  pDumper->writeFieldValue( "  Text Justification", element->getTextJustification() );

  if( element->getShowAngleFormatFlag() )
    pDumper->writeFieldValue( "  Angle Format Units", OdString(OD_T("Angle")) );
  else
    pDumper->writeFieldValue( "  Angle Format Units", OdString(OD_T("Length")) );

  pDumper->dumpFieldName("Param Flags:");
  pDumper->writeFieldValue( "  Hide 0 su for primary units", element->getHideZeroPrimarySubUnitsFlag() );
  pDumper->writeFieldValue( "  Hide 0 su for secondary units", element->getHideZeroSecondarySubUnitsFlag() );
  pDumper->writeFieldValue( "  Show 0 mu for primary units", element->getShowZeroPrimaryMasterUnitsFlag() );
  pDumper->writeFieldValue( "  Show 0 mu for secondary units", element->getShowZeroSecondaryMasterUnitsFlag() );
  pDumper->writeFieldValue( "  Use working units as primary units", !element->getUseCustomUnitsFlag() );
  pDumper->writeFieldValue( "  Show secondary units", !element->getShowSecondaryUnitsFlag() );
  pDumper->writeFieldValue( "  Enable tolerance", element->getShowToleranceFlag() );

  if( element->getUseToleranceLimitModeFlag() )
    pDumper->writeFieldValue( "  Tolerance mode", OdString(OD_T("Limit")) );
  else
    pDumper->writeFieldValue( "  Tolerance mode", OdString(OD_T("Plus/Minus")) );

  pDumper->writeFieldValue( "  Embed text", element->getEmbedTextFlag() );
  pDumper->writeFieldValue( "  Text location", element->getTextLocation() );

  if( element->getHorizontalTextFlag() )
    pDumper->writeFieldValue( "  Text alignment", OdString(OD_T("Horizontal")) );
  else
    pDumper->writeFieldValue( "  Text alignment", OdString(OD_T("Aligned")) );

  pDumper->writeFieldValue( "  Primary label display mode", element->getPrimaryLabelDisplayMode() );
  pDumper->writeFieldValue( "  Secondary label display mode", element->getSecondaryLabelDisplayMode() );
  pDumper->writeFieldValue( "Arrow symbol font id", element->getArrowFont() );
  pDumper->writeFieldValue( "Arrow symbol code", element->getArrowChar() );
  pDumper->writeFieldValue( "Stroke symbol font id", element->getStrokeFont() );
  pDumper->writeFieldValue( "Stroke symbol code", element->getStrokeChar() );
  pDumper->writeFieldValue( "Origin symbol font id", element->getOriginFont() );
  pDumper->writeFieldValue( "Origin symbol code", element->getOriginChar() );
  pDumper->writeFieldValue( "Diameter symbol font id", element->getDiameterFont() );
  pDumper->writeFieldValue( "Diameter symbol code", element->getDiameterChar() );
  pDumper->writeFieldValue( "Text height", element->getTextHeight() );
  pDumper->writeFieldValue( "Lower tolerance value", element->getLowerToleranceValue() );
  pDumper->writeFieldValue( "Upper tolerance value", element->getUpperToleranceValue() );
  pDumper->writeFieldValue( "Tolerance text scale", element->getToleranceTextScale() );
  pDumper->writeFieldValue( "Witness line offset", element->getWitnessLineOffset() );
  pDumper->writeFieldValue( "Witness line extension", element->getWitnessLineExtension() );
  pDumper->writeFieldValue( "Dimension scale", element->getDimensionScale() );
  pDumper->writeFieldValue( "Geometry Margin", element->getGeometryMargin() );
  pDumper->dumpFieldName("Extended Flags:");
  pDumper->writeFieldValue( "  Joiner flag", element->getJoinerFlag() );
  pDumper->writeFieldValue( "  Box text flag", element->getBoxTextFlag() );
  pDumper->writeFieldValue( "  Semi-Auto placement location flag", element->getSemiAutoFlag() );
  pDumper->writeFieldValue( "  Primary leading zero flag", element->getPrimaryLeadingZeroFlag() );
  pDumper->writeFieldValue( "  Primary trailing zeros flag", element->getPrimaryTrailingZerosFlag() );
  pDumper->writeFieldValue( "  Decimal comma flag", element->getDecimalCommaFlag() );
  pDumper->writeFieldValue( "  Capsule text flag", element->getCapsuleTextFlag() );
  pDumper->writeFieldValue( "  Superscript LSD flag", element->getSuperscriptLSDFlag() );
  pDumper->writeFieldValue( "  Round LSD flag", element->getRoundLSDFlag() );
  pDumper->writeFieldValue( "  Omit leading delimiter flag", element->getOmitLeadingDelimiterFlag() );
  pDumper->writeFieldValue( "  Dim lines color override flag", element->getDimLinesColorOverrideFlag() );
  pDumper->writeFieldValue( "  Dim lines weight override flag", element->getDimLinesWeightOverrideFlag() );
  pDumper->writeFieldValue( "  Text color override flag", element->getTextColorOverrideFlag() );
  pDumper->writeFieldValue( "  Text weight override flag", element->getTextWeightOverrideFlag() );
  pDumper->writeFieldValue( "  Text font override flag", element->getTextFontOverrideFlag() );
  pDumper->writeFieldValue( "  Level override flag", element->getLevelOverrideFlag() );
  pDumper->writeFieldValue( "  Text height override flag", element->getTextHeightOverrideFlag() );
  pDumper->writeFieldValue( "  Drop dimension flag", element->getDropDimensionAfterCraetionFlag() );
  pDumper->writeFieldValue( "  Terminator Arrow head type", element->getTerminatorArrowHead() );
  pDumper->writeFieldValue( "  Use reference file scale flag", element->getUseReferenceFileScaleFlag() );
  pDumper->writeFieldValue( "  Relative dimension lines flag", element->getRelativeDimensionLineFlag() );
  pDumper->writeFieldValue( "  Underline text flag", element->getTextUnderlineFlag() );
  pDumper->writeFieldValue( "  Dim lines style override flag", element->getDimLinesStyleOverrideFlag() );
  pDumper->writeFieldValue( "  No auto text lift flag", element->getNoAutoTextLiftFlag() );
  pDumper->writeFieldValue( "  Terminator orientation", element->getTerminatorOrientation() );
  pDumper->writeFieldValue( "  Master file units flag", element->getMasterFileUnitsFlag() );
  pDumper->writeFieldValue( "  Override level symbology flag", element->getOverrideLevelSymbologyFlag() );
  pDumper->writeFieldValue( "  View rotation flag", element->getViewRotationFlag() );
  pDumper->writeFieldValue( "  Secondary leading zero flag", element->getSecondaryLeadingZeroFlag() );
  pDumper->writeFieldValue( "  Secondary trailing zeros flag", element->getSecondaryTrailingZerosFlag() );

  pDumper->dumpFieldName("Tool specific:");

  for( OdUInt32 i = 0; i < 19; i++ )
  {
    OdDgDimension::ToolType iTool;
    OdString strToolName;

    getToolTypeAndDescriptionByToolIndex(i, iTool, strToolName);

    pDumper->dumpFieldName(strToolName);
    pDumper->writeFieldValue( "    First terminator", element->getToolTermFirst(iTool) );
    pDumper->writeFieldValue( "    Left  terminator", element->getToolTermLeft(iTool) );
    pDumper->writeFieldValue( "    Right terminator", element->getToolTermRight(iTool) );
    pDumper->writeFieldValue( "    Joint terminator", element->getToolTermJoint(iTool) );
    pDumper->writeFieldValue( "    Prefix", element->getToolPrefix(iTool) );
    pDumper->writeFieldValue( "    Stack extension lines flag", element->getToolStackExtLinesFlag(iTool) );
    pDumper->writeFieldValue( "    Suffix", element->getToolSuffix(iTool) );
    pDumper->writeFieldValue( "    Show arc symbol flag", element->getToolExtLinesArcFlag(iTool) );
    pDumper->writeFieldValue( "    Show left extension line flag", element->getToolLeftExtLinesPresentFlag(iTool) );
    pDumper->writeFieldValue( "    Show right extension line flag", element->getToolRightExtLinesPresentFlag(iTool) );
    pDumper->writeFieldValue( "    Text type", element->getToolTextType(iTool) );
    pDumper->writeFieldValue( "    Center mark flag", element->getToolCenterMarkFlag(iTool) );
    pDumper->writeFieldValue( "    Center mark left extension flag", element->getToolCenterMarkLeftExtendFlag(iTool) );
    pDumper->writeFieldValue( "    Center mark right extension flag", element->getToolCenterMarkRightExtendFlag(iTool) );
    pDumper->writeFieldValue( "    Center mark top extension flag", element->getToolCenterMarkTopExtendFlag(iTool) );
    pDumper->writeFieldValue( "    Center mark bottom extension flag", element->getToolCenterMarkBottomExtendFlag(iTool) );
    pDumper->writeFieldValue( "    Chord align flag", element->getToolChordAlignFlag(iTool) );
  }
  pDumper->writeFieldValue( "Stack offset", element->getStackOffset() );
  pDumper->writeFieldValue( "Center mark size", element->getCenterMarkSize() );
  pDumper->writeFieldValue( "Current dimension cmd", element->getCurrentDimensionCmd() );
  pDumper->writeFieldValue( "Angle display mode", element->getAngleDisplayMode() );
  pDumper->writeFieldValue( "Angle accuracy", element->getAngleAccuracy() );
  pDumper->writeFieldValue( "Main prefix", element->getMainPrefix() );
  pDumper->writeFieldValue( "Main suffix", element->getMainSuffix() );
  pDumper->writeFieldValue( "Tolerance prefix", element->getTolerancePrefix() );
  pDumper->writeFieldValue( "Tolerance suffix", element->getToleranceSuffix() );
  pDumper->writeFieldValue( "Upper prefix", element->getUpperPrefix() );
  pDumper->writeFieldValue( "Upper suffix", element->getUpperSuffix() );
  pDumper->writeFieldValue( "Lower prefix", element->getLowerPrefix() );
  pDumper->writeFieldValue( "Lower suffix", element->getLowerSuffix() );
  pDumper->writeFieldValue( "Dimension color", element->getDimensionColor() );
  pDumper->writeFieldValue( "Dimension line weight", element->getDimensionLineWeight() );
  pDumper->writeFieldValue( "Text color", element->getTextColor() );
  pDumper->writeFieldValue( "Text weight", element->getTextWeight() );
  pDumper->writeFieldValue( "Text font entry id", element->getTextFontId() );
  pDumper->writeFieldValue( "Dot symbol font entry id", element->getDotSymbolFontId() );
  pDumper->writeFieldValue( "Dot symbol code", element->getDotSymbolCode() );
  pDumper->dumpFieldName("Extended style flags:");
  pDumper->writeFieldValue( "  Angle leading zero flag", element->getAngleLeadingZeroFlag() );
  pDumper->writeFieldValue( "  Angle trailing zeros flag", element->getAngleTrailingZerosFlag() );
  pDumper->writeFieldValue( "  Auto superscript char flag", element->getAutoSuperscriptCharFlag() );
  pDumper->writeFieldValue( "Dimension line style entry id", element->getDimensionLineStyleId() );
  pDumper->writeFieldValue( "Lower text margin", element->getTextLowerMargin() );
  pDumper->writeFieldValue( "Left text margin", element->getTextLeftMargin() );
  pDumper->writeFieldValue( "Left tolerance margin", element->getToleranceLeftMargin() );
  pDumper->writeFieldValue( "Separator tolerance margin", element->getToleranceSepMargin() );
  pDumper->writeFieldValue( "Terminator height", element->getTerminatorHeight() );
  pDumper->writeFieldValue( "Terminator width", element->getTerminatorWidth() );
  pDumper->writeFieldValue( "Text width", element->getTextWidth() );
  pDumper->dumpFieldName("Place flags:");
  pDumper->writeFieldValue( "  Text width override", element->getTextWidthOverrideFlag() );
  pDumper->writeFieldValue( "  Extension line style override", element->getExtensionLineStyleOverrideFlag() );
  pDumper->writeFieldValue( "  Extension line weight override", element->getExtensionLineWeightOverrideFlag() );
  pDumper->writeFieldValue( "  Extension line color override", element->getExtensionLineColorOverrideFlag() );
  pDumper->writeFieldValue( "  Terminator style override", element->getTerminatorLineStyleOverrideFlag() );
  pDumper->writeFieldValue( "  Terminator weight override", element->getTerminatorLineWeightOverrideFlag() );
  pDumper->writeFieldValue( "  Terminator color override", element->getTerminatorColorOverrideFlag() );
  pDumper->writeFieldValue( "  Note frame type", element->getNoteFrameType() );
  pDumper->writeFieldValue( "  Use note inline leader", element->getNoteInlineLeaderFlag() );
  pDumper->writeFieldValue( "  Note justification", element->getNoteJustification() );
  pDumper->writeFieldValue( "  Use thousand separator", element->getMetricSpaceFlag() );
  pDumper->writeFieldValue( "  Comma thousand separator", element->getThousandSeparatorFlag() );
  pDumper->writeFieldValue( "  Stacked fraction type", element->getStackedFractionType() );
  pDumper->writeFieldValue( "  Stacked fraction alignment", element->getStackedFractionAlign() );
  pDumper->writeFieldValue( "  Use Stacked fraction", element->getUseStackedFractionFlag() );
  pDumper->writeFieldValue( "  Uniform cell scale", element->getUniformCellScaleFlag() );
  pDumper->writeFieldValue( "Extension line style id", element->getExtensionLineStyleId() );
  pDumper->writeFieldValue( "Extension line weight", element->getExtensionLineWeight() );
  pDumper->writeFieldValue( "Extension line color", element->getExtensionLineColor() );
  pDumper->writeFieldValue( "Terminator line style id", element->getTerminatorLineStyleId() );
  pDumper->writeFieldValue( "Terminator line weight", element->getTerminatorLineWeight() );
  pDumper->writeFieldValue( "Terminator color", element->getTerminatorColor() );
  pDumper->dumpFieldName("Draw flags:");
  pDumper->writeFieldValue( "  Arrow symbol type", element->getArrowSymbolType() );
  pDumper->writeFieldValue( "  Stroke symbol type", element->getStrokeSymbolType() );
  pDumper->writeFieldValue( "  Origin symbol type", element->getOriginSymbolType() );
  pDumper->writeFieldValue( "  Dot symbol type", element->getDotSymbolType() );
  pDumper->writeFieldValue( "  Prefix symbol type", element->getPrefixSymbolType() );
  pDumper->writeFieldValue( "  Suffix symbol type", element->getSuffixSymbolType() );
  pDumper->writeFieldValue( "  Diameter symbol type", element->getDiameterSymbolType() );
  pDumper->writeFieldValue( "  Plus/minus symbol type", element->getPlusMinusSymbolType() );
  pDumper->writeFieldValue( "  No line through arrow terminator", element->getNoLineThroughArrowTerminatorFlag() );
  pDumper->writeFieldValue( "  No line through stroke terminator", element->getNoLineThroughStrokeTerminatorFlag() );
  pDumper->writeFieldValue( "  No line through origin terminator", element->getNoLineThroughOriginTerminatorFlag() );
  pDumper->writeFieldValue( "  No line through dot terminator", element->getNoLineThroughDotTerminatorFlag() );
  pDumper->writeFieldValue( "  Underline override", element->getUnderlineOverrideFlag() );
  pDumper->writeFieldValue( "Prefix font id", element->getPrefixFont() );
  pDumper->writeFieldValue( "Suffix font id", element->getSuffixFont() );
  pDumper->writeFieldValue( "Prefix symbol code", element->getPrefixChar() );
  pDumper->writeFieldValue( "Suffix symbol code", element->getSuffixChar() );
  pDumper->writeFieldValue( "Plus/Minus symbol code", element->getPlusMinusChar() );
  pDumper->dumpFieldName("Primary Alt format:");
  pDumper->writeFieldValue( "  Threshold", element->getPrimaryAltThreshold() );
  pDumper->writeFieldValue( "  Label display mode", element->getPrimaryAltLabelDisplayMode() );
  pDumper->writeFieldValue( "  Comparison operator", element->getPrimaryAltOperator() );
  pDumper->writeFieldValue( "  Subunit threshold", element->getPrimaryAltSubunitThresholdFlag() );
  pDumper->writeFieldValue( "  Show zero master units", element->getPrimaryAltShowZeroMasterUnitsFlag() );
  pDumper->writeFieldValue( "  Show zero sub units", !element->getPrimaryAltHideZeroSubUnitsFlag() );
  pDumper->writeFieldValue( "  Enable alt format", element->getPrimaryAltPresentFlag() );
  pDumper->writeFieldValue( "  Accuracy", element->getPrimaryAltAccuracy() );
  pDumper->dumpFieldName("Ball and chain Flags:");
  pDumper->writeFieldValue( "  Leader terminator", element->getDimLeaderTerminator());
  pDumper->writeFieldValue( "  Leader chain type", element->getDimLeaderChainType());
  pDumper->writeFieldValue( "  Leader text alignment", element->getDimLeaderAlignment());
  pDumper->writeFieldValue( "  Inline leader flag", element->getDimLeaderInlineLeaderFlag());
  pDumper->writeFieldValue( "  Leader enable flag", element->getDimLeaderEnableFlag());
  pDumper->writeFieldValue( "  Leader undock text flag", element->getDimLeaderUndockTextFlag());
  pDumper->dumpFieldName("Secondary Alt format:");
  pDumper->writeFieldValue( "  Threshold", element->getSecondaryAltThreshold() );
  pDumper->writeFieldValue( "  Label display mode", element->getSecondaryAltLabelDisplayMode() );
  pDumper->writeFieldValue( "  Comparison operator", element->getSecondaryAltOperator() );
  pDumper->writeFieldValue( "  Subunit threshold", element->getSecondaryAltSubunitThresholdFlag() );
  pDumper->writeFieldValue( "  Show zero master units", element->getSecondaryAltShowZeroMasterUnitsFlag() );
  pDumper->writeFieldValue( "  Show zero sub units", !element->getSecondaryAltHideZeroSubUnitsFlag() );
  pDumper->writeFieldValue( "  Enable alt format", element->getSecondaryAltPresentFlag() );
  pDumper->writeFieldValue( "  Accuracy", element->getSecondaryAltAccuracy() );
//  pDumper->writeFieldValue( "Primary master units", element->getPrimaryMasterUnits());
//  pDumper->writeFieldValue( "Primary sub units", element->getPrimarySubUnits());
//  pDumper->writeFieldValue( "Secondary master units", element->getSecondaryMasterUnits());
//  pDumper->writeFieldValue( "Secondary sub units", element->getSecondarySubUnits());
  pDumper->writeFieldValue( "Annotation scale", element->getAnnotationScale());
  pDumper->writeFieldValue( "Primary tolerance accuracy", element->getPrimaryToleranceAccuracy());
  pDumper->writeFieldValue( "Secondary tolerance accuracy", element->getSecondaryToleranceAccuracy());
  pDumper->writeFieldValue( "Stacked fraction scale", element->getStackedFractionScale());
  pDumper->writeFieldValue( "Datum value", element->getOrdinateDimensionDatumValue());
  pDumper->writeFieldValue( "Dimension leader inline leader length", element->getDimLeaderInlineLeaderLength());
  pDumper->writeFieldValue( "Decrement in reverse direction flag", element->getOrdinateDimDecrementInReverseDirectionFlag());
  pDumper->writeFieldValue( "Ordinate dimension free text location flag", element->getOrdinateDimFreeLocationTextFlag());
  pDumper->writeFieldValue( "Enable term. min leader length flag", element->getEnableTerminatorMinLeaderLengthFlag());
  pDumper->writeFieldValue( "Enable suppress unfit term. flag", element->getEnableSuppressUnfitTerminatorFlag());
  pDumper->writeFieldValue( "Enable dimension inline leader flag", element->getDimLeaderEnableInlineLeaderLengthFlag());
  pDumper->writeFieldValue( "Enable text above optimal fit flag", element->getEnableTextAboveOptimalFitFlag());
  pDumper->writeFieldValue( "Fit true dimension text is Narrow", element->getEnableNarrowFontOptimalFitFlag());
  pDumper->writeFieldValue( "Primary retain fractional accuracy flag", element->getPrimaryRetainFractionalAccuracyFlag());
  pDumper->writeFieldValue( "Secondary retain fractional accuracy flag", element->getSecondaryRetainFractionalAccuracyFlag());
  pDumper->writeFieldValue( "Primary tolerance retain fractional accuracy flag", element->getPrimaryToleranceRetainFractionalAccuracyFlag());
  pDumper->writeFieldValue( "Secondary tolerance retain fractional accuracy flag", element->getSecondaryToleranceRetainFractionalAccuracyFlag());
  pDumper->writeFieldValue( "Primary alt. format retain fractional accuracy flag", element->getPrimaryAltRetainFractionalAccuracyFlag());
  pDumper->writeFieldValue( "Secondary alt. format retain fractional accuracy flag", element->getSecondaryAltRetainFractionalAccuracyFlag());
  pDumper->writeFieldValue( "Fit options", element->getFitOptions());
  pDumper->writeFieldValue( "Label-line mode", element->getLabelLineDimensionMode());
  pDumper->writeFieldValue( "Note spline fit flag", element->getNoteSplineFitFlag());
}

void OdDgMultilineStyleTableRecordDumperPE::dumpData( OdRxObjectPtr pObj, OdExDgnDumper* pDumper ) const
{
  OdDgTableRecordDumperPE::dumpData(pObj, pDumper);

  OdDgMultilineStyleTableRecordPtr element = pObj;

  pDumper->writeFieldValue( "Uses fill color", element->getUseFillColorFlag() );
  pDumper->writeFieldValue( "Fill color", element->getFillColorIndex() );
  pDumper->writeFieldValue( "Origin cap angle", element->getOriginCapAngle() );
  pDumper->writeFieldValue( "End cap angle", element->getEndCapAngle() );
  {
    OdDgMultilineSymbology symbology;

    element->getOriginCap( symbology );
    pDumper->writeFieldValue( "Origin cap", symbology );
    element->getMiddleCap( symbology );
    pDumper->writeFieldValue( "End cap", symbology );
    element->getEndCap( symbology );
    pDumper->writeFieldValue( "Middle cap", symbology );
  }

  {
    OdString fieldName;
    OdDgMultilineProfile profile;
    int j = element->getProfilesCount();
    pDumper->writeFieldValue( "Number of profiles", j );
    for(int i = 0; i < j; i++ )
    {
      fieldName.format(L"Profile %d", i );
      element->getProfile( i, profile );
      pDumper->writeFieldValue( fieldName, profile );
    }
  }
}

void OdDgMaterialTableRecordDumperPE::dumpData( OdRxObjectPtr pObj, OdExDgnDumper* pDumper ) const
{
  OdDgTableRecordDumperPE::dumpData(pObj, pDumper);

  OdDgMaterialTableRecordPtr pMaterial = pObj;

  pDumper->writeFieldValue("Palette Name", pMaterial->getPaletteName() );

  OdGiMaterialColor diffColor;
  OdGiMaterialColor ambColor;
  OdGiMaterialColor specColor;
  OdDgMaterialMap   diffMap;
  OdGiMaterialMap   reflMap;
  double dGlossFactor;

  pMaterial->getAmbient( ambColor );
  pMaterial->getDiffuse( diffColor, diffMap, false );
  pMaterial->getSpecular( specColor, dGlossFactor );
  pMaterial->getReflection(reflMap);

  pDumper->writeFieldValue("Ambient Color", ambColor );
  pDumper->writeFieldValue("Diffuse Color", diffColor );
  pDumper->writeFieldValue("Specular Color", specColor );
  pDumper->writeFieldValue("Gloss Factor", dGlossFactor );
  pDumper->writeFieldValue("Reflection Factor", reflMap.blendFactor() );
  pDumper->writeFieldValue("Opacity", pMaterial->getOpacity() );
  pDumper->writeFieldValue("Refraction", pMaterial->getRefraction() );
  pDumper->writeFieldValue("Translucence", pMaterial->getTranslucence() );
  pDumper->writeFieldValue("Thickness", pMaterial->getThickness() );
  pDumper->writeFieldValue("Shininess", pMaterial->getShininess() );
  pDumper->writeFieldValue("Shadow Cast Flag", pMaterial->getShadowCastFlag() );
  pDumper->writeFieldValue("Version", pMaterial->getVersion() );
  pDumper->writeFieldValue("Global Illumination Flag", pMaterial->getGlobalIlluminationFlag() );
  pDumper->writeFieldValue("Version", pMaterial->getVersion() );

  if( pMaterial->getExtendedParamCount() )
  {
    pDumper->dumpFieldName("----------------");
    pDumper->dumpFieldName("Extended Params:");

    for( OdUInt32 i = 0; i < pMaterial->getExtendedParamCount(); i++ )
    {
      OdDgMaterialParam param = pMaterial->getExtendedParam(i);
      pDumper->writeFieldValue( L"  " + param.getParamName(), param );
    }

    pDumper->dumpFieldName("----------------");
  }

  if( pMaterial->getMaterialPatternCount() )
  {
    pDumper->dumpFieldName("=-=-=-=-=-=-=-=-=");

    for( OdUInt32 i = 0; i < pMaterial->getMaterialPatternCount(); i++ )
    {
      OdDgMaterialPatternPtr pPattern = pMaterial->getMaterialPattern(i);

      OdString strName;
      strName.format(L"Material Pattern %d :", i);
      pDumper->dumpFieldName( strName );

      pDumper->writeFieldValue("  Pattern Type",  pPattern->getType() );
      pDumper->writeFieldValueHex("  Flags",  pPattern->getFlags() );
      pDumper->writeFieldValue("  Mapping Type",  pPattern->getMappingType() );
      pDumper->writeFieldValue("  Image Texture Mapping Type",  pPattern->getImageTextureMappingType() );
      pDumper->writeFieldValue("  Scale Mode",  pPattern->getScaleMode() );
      pDumper->writeFieldValue("  Use Pattern Flag",  pPattern->getUsePattern() );
      pDumper->writeFieldValue("  Pattern Angle",  pPattern->getPatternAngle() );
      pDumper->writeFieldValue("  Pattern Scale",  pPattern->getPatternScale() );
      pDumper->writeFieldValue("  Pattern Offset",  pPattern->getPatternOffset() );
      pDumper->writeFieldValue("  Use Map Link Flag",  pPattern->getUseMapLink() );
      pDumper->writeFieldValueHex("  Pattern Flags",  pPattern->getPatternFlags() );
      pDumper->writeFieldValueHex("  Bump Flags",  pPattern->getBumpFlags() );
      pDumper->writeFieldValue("  Bump Scale",  pPattern->getBumpScale() );
      pDumper->writeFieldValue("  Pattern Weight",  pPattern->getPatternWeight() );
      pDumper->writeFieldValue("  Projection Angles",  pPattern->getProjectionAngles() );
      pDumper->writeFieldValue("  Projection Scale",  pPattern->getProjectionScale() );
      pDumper->writeFieldValue("  U Flip",  pPattern->getUFlip() );
      pDumper->writeFieldValue("  File Name",  pPattern->getFileName() );
      pDumper->writeFieldValue("  Flip X Flag",  pPattern->getFlipXFlag() );
      pDumper->writeFieldValue("  Flip Y Flag",  pPattern->getFlipYFlag() );
      pDumper->writeFieldValue("  Lock Scale Flag",  pPattern->getLockScaleFlag() );
      pDumper->writeFieldValue("  Capped Flag",  pPattern->getCappedFlag() );
      pDumper->writeFieldValue("  Transparent Bg Flag",  pPattern->getTransparentBackgroundFlag() );
      pDumper->writeFieldValue("  Invert Bump Flag",  pPattern->getInvertBumpFlag() );

      for( OdUInt32 k = 0; k < pPattern->getUserDataCount(); k++ )
      {
        pDumper->writeFieldValue("  <User Data>",  pPattern->getUserData( k ) );
      }

      if( pPattern->getExtendedParamCount() )
      {
        pDumper->dumpFieldName("  Extended Params:");

        for( OdUInt32 i = 0; i < pPattern->getExtendedParamCount(); i++ )
        {
          OdDgMaterialParam param = pPattern->getExtendedParam(i);
          pDumper->writeFieldValue( L"    " + param.getParamName(), param );
        }
      }

      if( pPattern->getPatternLayerCount() )
      {
        for( OdUInt32 n = 0; n < pPattern->getPatternLayerCount(); n++ )
        {
          OdDgMaterialLayerPatternPtr pMatLayer = pPattern->getPatternLayer(n);

          OdString strName;
          strName.format(L"  Layer %d :", n);
          pDumper->dumpFieldName( strName );
          pDumper->writeFieldValue("    Layer Type", pMatLayer->getLayerType());
          pDumper->writeFieldValue("    Layer Type String", pMatLayer->getLayerTypeStr());
          pDumper->writeFieldValue("    Mapping Type", pMatLayer->getMappingType());
          pDumper->writeFieldValue("    Scale Mode", pMatLayer->getScaleMode());
          pDumper->writeFieldValue("    Pattern Angle", pMatLayer->getPatternAngle());
          pDumper->writeFieldValue("    Pattern Scale", pMatLayer->getPatternScale());
          pDumper->writeFieldValue("    Pattern Offset", pMatLayer->getPatternOffset());
          pDumper->writeFieldValue("    Blend", pMatLayer->getBlend());
          pDumper->writeFieldValue("    Layer Flags", pMatLayer->getLayerFlags());
          pDumper->writeFieldValue("    Data Flags", pMatLayer->getDataFlags());
          pDumper->writeFieldValue("    Color Gamma", pMatLayer->getColorGamma());
          pDumper->writeFieldValue("    Brightness Color", pMatLayer->getBrightnessColor());
          pDumper->writeFieldValue("    Visible Layer Flag", pMatLayer->getVisibleLayerFlag());
          pDumper->writeFieldValue("    Transparent Layer Background Flag", pMatLayer->getTransparentLayerBackgroundFlag());
          pDumper->writeFieldValue("    Flip X Flag", pMatLayer->getFlipXFlag());
          pDumper->writeFieldValue("    Flip Y Flag", pMatLayer->getFlipYFlag());
          pDumper->writeFieldValue("    Lock Scale Flag", pMatLayer->getLockScaleFlag());
          pDumper->writeFieldValue("    No Repeat X Flag", pMatLayer->getNoRepeatXFlag());
          pDumper->writeFieldValue("    No Repeat Y Flag", pMatLayer->getNoRepeatYFlag());
          pDumper->writeFieldValue("    Mirror X Flag", pMatLayer->getMirrorXFlag());
          pDumper->writeFieldValue("    Mirror Y Flag", pMatLayer->getMirrorXFlag());

          for( OdUInt32 k = 0; k < pPattern->getUserDataCount(); k++ )
          {
            pDumper->writeFieldValue("    <User Data>",  pMatLayer->getUserData( k ) );
          }

          if( pMatLayer->getExtendedParamCount() )
          {
            pDumper->dumpFieldName("    Extended Params:");

            for( OdUInt32 i = 0; i < pMatLayer->getExtendedParamCount(); i++ )
            {
              OdDgMaterialParam param = pMatLayer->getExtendedParam(i);
              pDumper->writeFieldValue( L"      " + param.getParamName(), param );
            }
          }
        }
      }

      pDumper->dumpFieldName("=-=-=-=-=-=-=-=-=");
    }
  }
}

void OdDgVariablesListPE::dumpData( OdRxObjectPtr pObj, OdExDgnDumper* pDumper ) const
{
  OdDgElementDumperPE::dumpData(pObj, pDumper);

  OdDgVariablesListPtr pList = pObj;

  std::map<OdDgElementId, OdDgVariableValueVariant> mapVariables;

  OdDgElementIteratorPtr pLocalVarIter = pList->createLocalVariableIterator();

  for(; !pLocalVarIter->done(); pLocalVarIter->step() )
  {
    OdDgVariablePtr pVar = pLocalVarIter->item().openObject(OdDg::kForRead);
    mapVariables[pVar->elementId()] = pVar->getDefaultValue();
  }

  pLocalVarIter = pList->createItemTypeBasedVariableIterator();

  for(; !pLocalVarIter->done(); pLocalVarIter->step() )
  {
    OdDgItemTypeBasedVariablePtr pVar = pLocalVarIter->item().openObject(OdDg::kForRead);
    mapVariables[pVar->elementId()] = pVar->getDefaultValue();
  }

  // Dump local variables.

  pDumper->dumpFieldName(L"Local Variable List:");

  pLocalVarIter = pList->createLocalVariableIterator();

  for(; !pLocalVarIter->done(); pLocalVarIter->step() )
  {
    OdDgVariablePtr pLocalVar = pLocalVarIter->item().openObject(OdDg::kForRead);

    pDumper->dumpFieldName(L" + Local Variable " + pLocalVar->getDisplayName() + L":" );
    pDumper->writeFieldValue(L"   - Display Name", pLocalVar->getDisplayName());
    pDumper->writeFieldValue(L"   - Name", pLocalVar->getName());
    pDumper->writeFieldValue(L"   - Type", pLocalVar->getType());
    pDumper->writeFieldValue(L"   - Read Only", pLocalVar->getReadOnlyFlag());
    pDumper->writeFieldValue(L"   - Scope", pLocalVar->getScope());
    pDumper->writeFieldValue(L"   - Visible", pLocalVar->getVisibleFlag());

    if( !pLocalVar->getUnitsDescription().isEmpty() )
      pDumper->writeFieldValue(L"   - Units", pLocalVar->getUnitsDescription());

    OdDgExpressionVariablePtr pExpression = pLocalVar->getExpression();

    if( pExpression.isNull() )
      pDumper->writeFieldValue(L"   - Expression", OdString(L"<none>"));
    else
    {
      pDumper->writeFieldValue(L"   - Expression", pExpression->getExpressionString() );

      OdDgVariableValueVariant varResult;

      if( pExpression->calculate(mapVariables, varResult) )
      {
        OdString strResult;
        switch( varResult.getType() )
        {
          case OdDgVariable::kNumber:
            strResult.format(L"<Number> %f", varResult.getDoubleValue() ); break;
          case OdDgVariable::kDistance:
            strResult.format(L"<Distance> %f", varResult.getDoubleValue() ); break;
          case OdDgVariable::kAngle:
            strResult.format(L"<Angle> %f", varResult.getDoubleValue() ); break;
          case OdDgVariable::kArea:
            strResult.format(L"<Area> %f", varResult.getDoubleValue() ); break;
          case OdDgVariable::kInteger:
            strResult.format(L"<Integer> %d", varResult.getIntegerValue() ); break;
          case OdDgVariable::kBoolean:
            varResult.getBooleanValue() ? strResult = L"<Boolean> true" : strResult = L"<Boolean> false"; break;
        }

        pDumper->writeFieldValue(L"   - Calculated Result", strResult );
      }
      else
        pDumper->writeFieldValue(L"   - Calculated Result", OdString(L"<error>") );
    }

    pDumper->writeFieldValue(L"   - Default Value", pLocalVar->getDefaultValue());
  }

  // Dump item type based variables.

  if( !pList->getItemTypeLibraryId().isNull() )
  {
    pDumper->dumpFieldName(L"Item Type Based Variable List:");

    OdDgElementIteratorPtr pVarIter = pList->createItemTypeBasedVariableIterator();

    for(; !pVarIter->done(); pVarIter->step() )
    {
      OdDgItemTypeBasedVariablePtr pVar = pVarIter->item().openObject(OdDg::kForRead);

      pDumper->dumpFieldName(L" + Item Type Variable " + pVar->getName() + L":" );
      pDumper->writeFieldValue(L"   - Name", pVar->getName());
      pDumper->writeFieldValue(L"   - Type", pVar->getType());
      pDumper->writeFieldValue(L"   - Units", pVar->getUnits());
      pDumper->writeFieldValue(L"   - Scope", pVar->getScope());
      pDumper->writeFieldValue(L"   - Visible", pVar->getVisibleFlag());

      OdDgExpressionVariablePtr pExpression = pVar->getExpression();

      if( pExpression.isNull() )
        pDumper->writeFieldValue(L"   - Expression", OdString(L"<none>"));
      else
      {
        pDumper->writeFieldValue(L"   - Expression", pExpression->getExpressionString() );

        OdDgVariableValueVariant varResult;

        if( pExpression->calculate(mapVariables, varResult) )
        {
          varResult = OdDgItemTypeBasedVariable::convertValueToItemBasedUnits(varResult, pVar->getUnits(), pList->getMasterUnits() );

          OdString strResult;
          switch( varResult.getType() )
          {
            case OdDgVariable::kNumber:
              strResult.format(L"<Number> %f", varResult.getDoubleValue() ); break;
            case OdDgVariable::kDistance:
              strResult.format(L"<Distance> %f", varResult.getDoubleValue() ); break;
            case OdDgVariable::kAngle:
              strResult.format(L"<Angle> %f", varResult.getDoubleValue() ); break;
            case OdDgVariable::kArea:
              strResult.format(L"<Area> %f", varResult.getDoubleValue() ); break;
            case OdDgVariable::kInteger:
              strResult.format(L"<Integer> %d", varResult.getIntegerValue() ); break;
            case OdDgVariable::kBoolean:
              varResult.getBooleanValue() ? strResult = L"<Boolean> true" : strResult = L"<Boolean> false"; break;
          }

          pDumper->writeFieldValue(L"   - Calculated Result", strResult );
        }
        else
          pDumper->writeFieldValue(L"   - Calculated Result", OdString(L"<error>") );
      }

      pDumper->writeFieldValue(L"   - Default Value", OdDgItemTypeBasedVariable::convertValueToItemBasedUnits(pVar->getDefaultValue(), pVar->getUnits(), pList->getMasterUnits() ));
    }
  }

  // Dump item type defaults.

  if( !pList->getItemTypeLibraryId().isNull() )
  {
    pDumper->writeFieldValueHex(L"Item Type Library Id", (OdUInt64)(pList->getItemTypeLibraryId().getHandle()) );
    pDumper->writeFieldValue(L"Item Type Name", pList->getItemTypeName() );

    OdDgItemTypeInstance curItemType = pList->getItemTypeDefaults();

    if( curItemType.getPropertyCount() > 0 )
      pDumper->dumpFieldName(L"Item Type Defaults:");

    for( OdUInt32 j = 0; j < curItemType.getPropertyCount(); j++ )
    {
      OdDgItemTypePropertyInstance curProp = curItemType.getProperty(j);

      if( (curProp.getValue().getType() != OdDgItemTypePropertyInstanceValue::kComplexProperty) && 
        (curProp.getValue().getType() != OdDgItemTypePropertyInstanceValue::kComplexPropertyArray)
        )
      {
        OdString strPropName = L" - " + convertItemNameToDisplayLabel(curProp.getName());
        pDumper->writeFieldValue( strPropName, curProp.getValue() );
      }
      else
      {
        pDumper->dumpFieldName(L" + " + convertItemNameToDisplayLabel(curProp.getName()));
        OdString strPropName = L"   - " + convertItemNameToDisplayLabel(curProp.getName());
        pDumper->writeFieldValue( strPropName, curProp.getValue() );
      }
    }
  }
}

void OdDgVariableValuesSetPE::dumpData( OdRxObjectPtr pObj, OdExDgnDumper* pDumper ) const
{
  OdDgVariableValuesSetPtr pSet = pObj;

  OdUInt32 nVariables = pSet->getVariablesCount();

  OdArray<OdString> arrLocalVarNames;
  OdArray<OdDgVariableValueVariant> arrLocalValues;
  OdArray<OdString> arrItemTypeVarNames;
  OdArray<OdDgVariableValueVariant> arrItemTypeValues;

  OdUInt32 i;

  for( i = 0; i < nVariables; i++ )
  {
    OdDgElementId idVariable = pSet->getVariableId(i);
    OdDgVariableValueVariant curVal = pSet->getVariableValue( idVariable );

    if( idVariable.isNull() )
      continue;

    OdDgElementPtr pElm = idVariable.openObject(OdDg::kForRead);

    OdString strName;

    if( pElm->isKindOf(OdDgVariable::desc()) )
    {
      OdDgVariablePtr pVar = pElm;
      strName = pVar->getDisplayName();
      arrLocalVarNames.push_back(L" - " + strName);
      arrLocalValues.push_back(curVal);
    }
    else if( pElm->isKindOf(OdDgItemTypeBasedVariable::desc()) )
    {
      OdDgItemTypeBasedVariablePtr pVar = pElm;
      strName = pVar->getName();
      arrItemTypeVarNames.push_back(L" - " + strName);
      arrItemTypeValues.push_back(curVal);
    }
  }

  pDumper->dumpFieldName(L"Local Variables:");

  for( i = 0; i < arrLocalVarNames.size(); i++ )
    pDumper->writeFieldValue(arrLocalVarNames[i], arrLocalValues[i] );

  if( arrItemTypeValues.size() > 0 )
  {
    pDumper->dumpFieldName(L"Item Type Based Variables:");

    for( i = 0; i < arrItemTypeVarNames.size(); i++ )
      pDumper->writeFieldValue(arrItemTypeVarNames[i], arrItemTypeValues[i] );
  }

  OdDgItemTypeInstance curItemType = pSet->getItemTypeInstance();

  if( curItemType.getPropertyCount() > 0 )
    pDumper->dumpFieldName(L"Item Type Instance:");

  for( OdUInt32 j = 0; j < curItemType.getPropertyCount(); j++ )
  {
    OdDgItemTypePropertyInstance curProp = curItemType.getProperty(j);

    if( (curProp.getValue().getType() != OdDgItemTypePropertyInstanceValue::kComplexProperty) && 
      (curProp.getValue().getType() != OdDgItemTypePropertyInstanceValue::kComplexPropertyArray)
      )
    {
      OdString strPropName = L" - " + convertItemNameToDisplayLabel(curProp.getName());
      pDumper->writeFieldValue( strPropName, curProp.getValue() );
    }
    else
    {
      pDumper->dumpFieldName(L" + " + convertItemNameToDisplayLabel(curProp.getName()));
      OdString strPropName = L"   - " + convertItemNameToDisplayLabel(curProp.getName());
      pDumper->writeFieldValue( strPropName, curProp.getValue() );
    }
  }
}

void OdDgTablePrototypeElementPE::dumpData( OdRxObjectPtr pObj, OdExDgnDumper* pDumper ) const
{
  OdDgElementDumperPE::dumpData(pObj, pDumper);
  OdDgTablePrototypeElementPtr pTablePrototype = pObj;
  pDumper->writeFieldValue( "Prototype Index", pTablePrototype->getPrototypeIndex() );

  OdUInt32 nSchemas = pTablePrototype->getSchemaCount();

  for( OdUInt32 i = 0; i < nSchemas; i++ )
  {
    OdDgPrototypeSchema curSchema = pTablePrototype->getSchema(i);
    OdString strSchemaName;
    strSchemaName.format(L" = Schema %d ",curSchema.getSchemaIndex() );
    strSchemaName += L"(" + curSchema.getSchemaName() + ") :";
    pDumper->dumpFieldName(strSchemaName);

    for( OdUInt32 j = 0; j < curSchema.getSchemaItemCount(); j++ )
    {
      OdDgPrototypeSchemaItem curItem = curSchema.getSchemaItem(j);

      OdString strType = L"  - kProxyData";

      if( !curItem.getProxyDataFlag() )
      {
        switch( curItem.getItemDataType() )
        {
          case OdDgPrototypeSchemaItem::kGroup: strType = L"  - kGroup"; break;
          case OdDgPrototypeSchemaItem::kExpressionDesc: strType = L"  - kExpressionDesc"; break;
          case OdDgPrototypeSchemaItem::kStringArray: strType = L"  - kStringArray"; break;
          case OdDgPrototypeSchemaItem::kDoubleArray: strType = L"  - kDoubleArray"; break;
          case OdDgPrototypeSchemaItem::kInt32Array: strType  = L"  - kInt32Array"; break;
          case OdDgPrototypeSchemaItem::kBooleanArray: strType = L"  - kBooleanArray"; break;
          case OdDgPrototypeSchemaItem::kPoint3dArray: strType = L"  - kPoint3dArray"; break;
          case OdDgPrototypeSchemaItem::kDateTimeArray: strType = L"  - kDateTimeArray"; break;
          case OdDgPrototypeSchemaItem::kString: strType = L"  - kString"; break;
          case OdDgPrototypeSchemaItem::kDouble: strType = L"  - kDouble"; break;
          case OdDgPrototypeSchemaItem::kInt32: strType  = L"  - kInt32"; break;
          case OdDgPrototypeSchemaItem::kBoolean: strType = L"  - kBoolean"; break;
          case OdDgPrototypeSchemaItem::kPoint3d: strType = L"  - kPoint3d"; break;
          case OdDgPrototypeSchemaItem::kDateTime: strType = L"  - kDateTime"; break;
          case OdDgPrototypeSchemaItem::kFlags: 
          {
            if( j != 0)
              strType.format(L"  - kItemGroup (%d)",j);
            else
              strType = L"  - kFlags";

          } break;
        }

        if( curItem.getParentSchemaItemIndex() != 0 )
        {
          OdString strParent;
          strParent.format(L"(%d) ", curItem.getParentSchemaItemIndex() );
          pDumper->writeFieldValue( strType, strParent + curItem.getItemName() );
        }
        else
          pDumper->writeFieldValue( strType, curItem.getItemName() );
      }
      else
        pDumper->writeFieldValue( strType, OdString(L"<empty>") );
    }
  }
}

void OdDgEnvironmentMapTableDumperPE::dumpData( OdRxObjectPtr pObj, OdExDgnDumper* pDumper ) const
{
  OdDgElementDumperPE::dumpData(pObj, pDumper);

  OdDgEnvironmentMapTablePtr pEnvMap = pObj;

  OdString strFildName;

  for( OdUInt32 i = 0; i < pEnvMap->getMapCount(); i++ )
  {
    strFildName.format(L"  EnvironmentMap %d", i + 1);
    pDumper->dumpFieldName( strFildName );

    OdDgEnvironmentMap envMap = pEnvMap->getMapAt(i);

    // Main properties.

    pDumper->writeFieldValue( L"    Name", envMap.getMapName() );
    pDumper->writeFieldValue( L"    Untitled Flag", envMap.getUntitledFlag() );
    pDumper->writeFieldValue( L"    Enabled Flag", envMap.getEnableFlag() );
    pDumper->writeFieldValue( L"    Vis Camera Flag", envMap.getVisCameraFlag() );
    pDumper->writeFieldValue( L"    Vis Indirect Flag", envMap.getVisIndirectFlag() );
    pDumper->writeFieldValue( L"    Vis Reflect Flag", envMap.getReflectFlag() );
    pDumper->writeFieldValue( L"    Vis Refract Flag", envMap.getRefractFlag() );
    pDumper->writeFieldValue( L"    Override Existing Lights Flag", envMap.getUseIndirectLightsFlag() );
    pDumper->writeFieldValue( L"    Use Indirect Layer As Main Flag", envMap.getUseIndirectLayerAsMainFlag() );
    pDumper->writeFieldValue( L"    Fog Enabled Flag", envMap.getFogEnabledFlag() );
    pDumper->writeFieldValue( L"    Background Enabled Flag", envMap.getBackgroundEnabledFlag() );

    // Fog properties.

    OdDgEnvMapFog fog = envMap.getFog();

    pDumper->writeFieldValueHex( L"    Fog Color", fog.getColor() );
    pDumper->writeFieldValue( L"    Use Environment Color For Fog", fog.getUseEnvironmentColorForFogFlag() );
    pDumper->writeFieldValue( L"    Fog Mode", fog.getMode() );
    pDumper->writeFieldValue( L"    Fog Density", fog.getFogDensity() );
    pDumper->writeFieldValue( L"    Fog Start", fog.getFogStart() );
    pDumper->writeFieldValue( L"    Fog End", fog.getFogEnd() );
    pDumper->writeFieldValue( L"    Fog Brightness", fog.getBrightness() );

    // Background properties

    OdDgEnvMapBackground background = envMap.getBackground();

    pDumper->writeFieldValue( L"    Background Mode", background.getMode() );
    pDumper->writeFieldValueHex( L"    Background Color", background.getColor() );
    pDumper->writeFieldValue( L"    Background Image File", background.getImageFileName() );

    // Main layer

    OdDgEnvMapLayer mainLayer     = envMap.getMainLayer();

    dumpEnvironmentLayer( pDumper, mainLayer, true );

    // Indirect layer

    OdDgEnvMapLayer indirectLayer = envMap.getIndirectLayer();

    dumpEnvironmentLayer( pDumper, indirectLayer, false );
  }
}

void OdDgEnvironmentMapTableDumperPE::dumpEnvironmentLayer( OdExDgnDumper* pDumper, OdDgEnvMapLayer layer, bool bMainLayer ) const
{
  if( bMainLayer )
  {
    pDumper->dumpFieldName(L"     Main Layer");
  }
  else
  {
    pDumper->dumpFieldName(L"     Indirect Layer");
  }

  pDumper->writeFieldValue( L"      Layer Type", layer.getLayerType() );
  pDumper->writeFieldValue( L"      Intensity Exponent", layer.getIntensityExponent() );
  pDumper->writeFieldValue( L"      Lux Intensity", layer.getLuxIntensity() );
  pDumper->writeFieldValue( L"      Use Lux Intensity Flag", layer.getUseLuxIntensityFlag() );

  // Sky Light

  OdDgEnvMapSkyLight skyLight = layer.getSkyLight();

  pDumper->writeFieldValue( L"      Sky Light Solar Size", skyLight.getSolarSize() );

  // Light Probe

  OdDgEnvMapImage lightProbe = layer.getLightProbe();

  pDumper->writeFieldValue( L"      Light Probe Projection", lightProbe.getProjectionType() );
  pDumper->writeFieldValue( L"      Light Probe Mirror Image Flag", lightProbe.getMirrorFlag() );
  pDumper->writeFieldValue( L"      Light Probe Auto Correction Gamma Flag", lightProbe.getAutoCorrectionGammaFlag() );
  pDumper->writeFieldValue( L"      Light Probe Use Antialiasing Flag", lightProbe.getUseAntialiasingFlag() );
  pDumper->writeFieldValue( L"      Light Probe Texture Filter", lightProbe.getFilterType() );
  pDumper->writeFieldValue( L"      Light Probe Gamma", lightProbe.getGamma() );
  pDumper->writeFieldValue( L"      Light Probe Antialias Strength", lightProbe.getAntialiasingStrength() );
  pDumper->writeFieldValue( L"      Light Probe Image File", lightProbe.getImageFileName() );
  pDumper->writeFieldValue( L"      Light Probe Origin", lightProbe.getOrigin() );
  pDumper->writeFieldValue( L"      Light Probe Rotation", lightProbe.getRotation() );
  pDumper->writeFieldValue( L"      Light Probe Scale", lightProbe.getScale() );

  // Image

  OdDgEnvMapImage image = layer.getImage();

  pDumper->writeFieldValue( L"      Image Projection", image.getProjectionType() );
  pDumper->writeFieldValue( L"      Image Mirror Image Flag", image.getMirrorFlag() );
  pDumper->writeFieldValue( L"      Image Auto Correction Gamma Flag", image.getAutoCorrectionGammaFlag() );
  pDumper->writeFieldValue( L"      Image Use Antialiasing Flag", image.getUseAntialiasingFlag() );
  pDumper->writeFieldValue( L"      Image Texture Filter", image.getFilterType() );
  pDumper->writeFieldValue( L"      Image Gamma", image.getGamma() );
  pDumper->writeFieldValue( L"      Image Antialias Strength", image.getAntialiasingStrength() );
  pDumper->writeFieldValue( L"      Image File", image.getImageFileName() );
  pDumper->writeFieldValue( L"      Image Origin", image.getOrigin() );
  pDumper->writeFieldValue( L"      Image Rotation", image.getRotation() );
  pDumper->writeFieldValue( L"      Image Scale", image.getScale() );

  // Image Cube

  OdDgEnvMapImageCube imageCube = layer.getImageCube();

  pDumper->writeFieldValue( L"      Image Cube Rotation", imageCube.getRotation() );
  pDumper->writeFieldValue( L"      Image Cube Auto Correction Gamma Flag", imageCube.getAutoCorrectionGammaFlag() );
  pDumper->writeFieldValue( L"      Image Cube Use Antialiasing Flag", imageCube.getUseAntialiasingFlag() );
  pDumper->writeFieldValue( L"      Image Cube Texture Filter", imageCube.getFilterType() );
  pDumper->writeFieldValue( L"      Image Cube Gamma", imageCube.getGamma() );
  pDumper->writeFieldValue( L"      Image Cube Antialias Strength", imageCube.getAntialiasingStrength() );
  pDumper->writeFieldValue( L"      Image Cube Front File", imageCube.getFrontFileName() );
  pDumper->writeFieldValue( L"      Image Cube Back File", imageCube.getBackFileName() );
  pDumper->writeFieldValue( L"      Image Cube Left File", imageCube.getLeftFileName() );
  pDumper->writeFieldValue( L"      Image Cube Right File", imageCube.getRightFileName() );
  pDumper->writeFieldValue( L"      Image Cube Top File", imageCube.getTopFileName() );
  pDumper->writeFieldValue( L"      Image Cube Bottom File", imageCube.getBottomFileName() );

  // Gradient

  OdDgEnvMapGradient gradient = layer.getGradient();

  pDumper->writeFieldValue( L"      Gradient Mode", gradient.getGradientMode() );
  pDumper->writeFieldValue( L"      Gradient Ground Exponent", gradient.getGroundExponent() );
  pDumper->writeFieldValue( L"      Gradient Sky Exponent", gradient.getSkyExponent() );
  pDumper->writeFieldValueHex( L"      Gradient Zenith Color", gradient.getZenithColor() );
  pDumper->writeFieldValueHex( L"      Gradient Nadir Color", gradient.getNadirColor() );
  pDumper->writeFieldValueHex( L"      Gradient Sky Color", gradient.getSkyColor() );
  pDumper->writeFieldValueHex( L"      Gradient Ground Color", gradient.getGroundColor() );
}

void OdDgDisplayStyleTableDumperPE::dumpData( OdRxObjectPtr pObj, OdExDgnDumper* pDumper ) const
{
  OdDgElementDumperPE::dumpData(pObj, pDumper);
}

void OdDgPrintStyleTableRecordDumperPE::dumpData( OdRxObjectPtr pObj, OdExDgnDumper* pDumper ) const
{
  OdDgElementDumperPE::dumpData(pObj, pDumper);

  OdDgPrintStyleTableRecordPtr pPrintStyle = pObj;

  pDumper->writeFieldValue( L"Name", pPrintStyle->getName());
  pDumper->writeFieldValue( L"DefaultFlag", pPrintStyle->getDefaultStyleFlag());
  pDumper->dumpFieldName( OdString(L"------ [ Global Section ] ------"));
  pDumper->writeFieldValue( L"  Printer Configuration File Path", pPrintStyle->getPrinterConfigurationFilePath());
  pDumper->writeFieldValue( L"  Windows Printer Name", pPrintStyle->getWindowsPrinterName());
  pDumper->dumpFieldName( OdString(L"------ [ DWG/DGN Section ] ------"));
  pDumper->writeFieldValue( L"  Print Area", pPrintStyle->getPrintArea());
  pDumper->writeFieldValue( L"  Model Name", pPrintStyle->getPrintModelName());
  pDumper->writeFieldValue( L"  View Group Name", pPrintStyle->getViewGroupName());
  pDumper->writeFieldValue( L"  View Name", pPrintStyle->getViewName());
  pDumper->writeFieldValue( L"  View Number", pPrintStyle->getViewNumber());
  pDumper->writeFieldValue( L"  Rasterized Flag", pPrintStyle->getRasterizedFlag());
  pDumper->writeFieldValue( L"  Print To 3D Flag", pPrintStyle->getPrintTo3DFlag());
  pDumper->writeFieldValue( L"  Paper Size", pPrintStyle->getPaperSizeName());
  pDumper->writeFieldValue( L"  Paper Orientation", pPrintStyle->getPaperOrientation());
  pDumper->writeFieldValue( L"  Paper Source", pPrintStyle->getPaperSource());
  pDumper->writeFieldValue( L"  Full Sheet Flag", pPrintStyle->getFullSheetFlag());
  pDumper->writeFieldValue( L"  Layout Units", pPrintStyle->getLayoutUnits());
  pDumper->writeFieldValue( L"  Layout Size/Scale Mode", pPrintStyle->getLayoutSizeScaleMode() );

  switch( pPrintStyle->getLayoutSizeScaleMode() )
  {
    case OdDgPrintStyleTableRecord::kSizeX:
      pDumper->writeFieldValue( L"    - Size X ", pPrintStyle->getLayoutSizeX()); break;
    case OdDgPrintStyleTableRecord::kSizeY:
      pDumper->writeFieldValue( L"    - Size Y ", pPrintStyle->getLayoutSizeY()); break;
    case OdDgPrintStyleTableRecord::kScale:
      pDumper->writeFieldValue( L"    - Scale ", pPrintStyle->getLayoutScale()); break;
    case OdDgPrintStyleTableRecord::kRescaleFactor:
    {
      double dNum, dDen;
      pPrintStyle->getLayoutRescaleFactor(dNum, dDen);
      OdString strRescale;
      strRescale.format(L"%f, %f", dNum, dDen);
      pDumper->writeFieldValue( L"    - RescaleFactor ", strRescale);
    } break;
    case OdDgPrintStyleTableRecord::kRescaleSize:
      pDumper->writeFieldValue( L"    - RescaleSize ", pPrintStyle->getLayoutRescaleSizeValues()); break;
  }

  pDumper->writeFieldValue( L"  Layout Rotation", pPrintStyle->getLayoutRotation());
  pDumper->writeFieldValue( L"  Layout Origin Mode", pPrintStyle->getLayoutOriginMode());

  if( pPrintStyle->getLayoutOriginMode() == OdDgPrintStyleTableRecord::kUserDefinedOrigin )
    pDumper->writeFieldValue( L"    Origin", pPrintStyle->getLayoutUserDefinedOrigin());

  pDumper->writeFieldValue( L"  Layout Mirror Mode", pPrintStyle->getLayoutMirrorMode() );
  pDumper->writeFieldValue( L"  Layout Fence Align", pPrintStyle->getLayoutFenceAlign() );
  pDumper->writeFieldValue( L"  Pen Table File", pPrintStyle->getPenTableFilename() );
  pDumper->writeFieldValue( L"  Design Script File", pPrintStyle->getDesignScriptFilename() );
  pDumper->writeFieldValue( L"  Work Set", pPrintStyle->getProjectWorkspace() );
  pDumper->writeFieldValue( L"  Color Mode", pPrintStyle->getColorMode() );
  pDumper->writeFieldValue( L"  Cad Raster Quality", pPrintStyle->getCadRasterQualityFactor() );
  pDumper->writeFieldValue( L"  Cad Raster Brightness", pPrintStyle->getCadRasterBrightness() );
  pDumper->writeFieldValue( L"  Cad Raster Contrast", pPrintStyle->getCadRasterContrast() );
  pDumper->writeFieldValue( L"  Print Raster Flag", pPrintStyle->getPrintRasterFlag() );
  pDumper->writeFieldValue( L"  Print Binary Raster AsIs Flag", pPrintStyle->getPrintBinaryRasterAsIsFlag() );
  pDumper->writeFieldValue( L"  Print Raster As Grayscale Flag", pPrintStyle->getPrintRasterAsGrayscaleFlag() );
  pDumper->writeFieldValue( L"  Property Publish Method", pPrintStyle->getPropertyPublishingMethod() );
  pDumper->writeFieldValue( L"  Property Filter File", pPrintStyle->getPropertyFilterFileName() );
  pDumper->writeFieldValue( L"  Update Print Def Name Flag", pPrintStyle->getUpdatePrintDefNameFlag() );
  pDumper->writeFieldValue( L"  Update From Source Flag", pPrintStyle->getUpdateFromSourceFlag() );
  pDumper->writeFieldValue( L"  Print Def Creation Mode", pPrintStyle->getPrintDefCreationMode() );

  if( pPrintStyle->getPrintDefCreationMode() == OdDgPrintStyleTableRecord::kFromListedModels )
  {
    OdArray<OdString> arrModels = pPrintStyle->getPrintDefCreationModelList();

    for( OdUInt32 i = 0; i < arrModels.size(); i++ )
    {
      OdString strFieldName;
      strFieldName.format(L"    - Model List Item %d", i);
      pDumper->writeFieldValue( strFieldName, arrModels[i] );
    }
  }

  OdDgPrintStyleFenceCreationData fenceData = pPrintStyle->getFenceCreationData();

  pDumper->writeFieldValue( L"  Fence Creation Mode", fenceData.getMode() );

  if( fenceData.getMode() == OdDgPrintStyleFenceCreationData::kFromPoints )
  {
    OdArray<OdDgPrintStyleFencePoint> arrPts = fenceData.getFencePoints();

    for( OdUInt32 i = 0; i < arrPts.size(); i++ )
    {
      OdString strFieldName;
      strFieldName.format(L"    - Point %d", i);

      OdString strValue;
      strValue.format(L"( %f:%f:%f, %f:%f:%f, %f:%f:%f )", 
                      arrPts[i].m_X.m_dMU, arrPts[i].m_X.m_dSU, arrPts[i].m_X.m_dPU,
                      arrPts[i].m_Y.m_dMU, arrPts[i].m_Y.m_dSU, arrPts[i].m_Y.m_dPU,
                      arrPts[i].m_Z.m_dMU, arrPts[i].m_Z.m_dSU, arrPts[i].m_Z.m_dPU
                     );
      pDumper->writeFieldValue( strFieldName, strValue );
    }
  }
  else if( fenceData.getMode() != OdDgPrintStyleFenceCreationData::kNone )
  {
    pDumper->writeFieldValue( L"    - Search Master Model Flag", fenceData.getSearchMasterModel() );
    pDumper->writeFieldValue( L"    - Search References Flag", fenceData.getSearchReferences() );
    pDumper->writeFieldValue( L"    - Reference Names", fenceData.getReferenceNames(), true );
    pDumper->writeFieldValue( L"    - Level Names", fenceData.getLevelNames(), true );
    pDumper->writeFieldValue( L"    - Cell Names", fenceData.getCellNames(), true );
    pDumper->writeFieldValue( L"    - Color Indexes", fenceData.getColorIndexes(), true );
    pDumper->writeFieldValue( L"    - Line Styles", fenceData.getLineStyles(), true );
    pDumper->writeFieldValue( L"    - Line Weights", fenceData.getLineWeights(), true );

    OdArray<OdUInt32> arrTypes;

    for( OdUInt32 i = 0; i < fenceData.getElementTypes().size(); i++ )
      arrTypes.push_back( (OdUInt32)(fenceData.getElementTypes()[i]));

    pDumper->writeFieldValue( L"    - Element Types", arrTypes, true );
  }

  pDumper->writeFieldValue( L"  Border Comment", pPrintStyle->getBorderComment() );
  pDumper->writeFieldValue( L"  Apply Color Mode To Raster Flag", pPrintStyle->getApplyColorModeToRasterFlag() );
  pDumper->writeFieldValue( L"  Print Border Flag", pPrintStyle->getPrintBorderFlag() );
  pDumper->writeFieldValue( L"  Print Broken Association Symbology Flag", pPrintStyle->getPrintBrokenAssociationSymbologyFlag() );
  pDumper->writeFieldValue( L"  Print Clip Front Flag", pPrintStyle->getPrintClipFrontFlag() );
  pDumper->writeFieldValue( L"  Print Clip Back Flag", pPrintStyle->getPrintClipBackFlag() );
  pDumper->writeFieldValue( L"  Print Clip Volume Flag", pPrintStyle->getPrintClipVolumeFlag() );
  pDumper->writeFieldValue( L"  Print Constructions Flag", pPrintStyle->getPrintConstructionsFlag() );
  pDumper->writeFieldValue( L"  Print Custom Line Styles Flag", pPrintStyle->getPrintCustomLineStylesFlag() );
  pDumper->writeFieldValue( L"  Print Dimensions Flag", pPrintStyle->getPrintDimensionsFlag() );
  pDumper->writeFieldValue( L"  Print Data Fields Flag", pPrintStyle->getPrintDataFieldsFlag() );
  pDumper->writeFieldValue( L"  Print Fence Boundary Flag", pPrintStyle->getPrintFenceBoundaryFlag() );
  pDumper->writeFieldValue( L"  Print Fast Cells Flag", pPrintStyle->getPrintFastCellsFlag() );
  pDumper->writeFieldValue( L"  Print Fill Flag", pPrintStyle->getPrintFillFlag() );
  pDumper->writeFieldValue( L"  Print Level Overrides Flag", pPrintStyle->getPrintLevelOverridesFlag() );
  pDumper->writeFieldValue( L"  Print Line Weights Flag", pPrintStyle->getPrintLineWeightsFlag() );
  pDumper->writeFieldValue( L"  Print Patterns Flag", pPrintStyle->getPrintPatternsFlag() );
  pDumper->writeFieldValue( L"  Print Points Flag", pPrintStyle->getPrintPointsFlag() );
  pDumper->writeFieldValue( L"  Print Tags Flag", pPrintStyle->getPrintTagsFlag() );
  pDumper->writeFieldValue( L"  Print Texts Flag", pPrintStyle->getPrintTextsFlag() );
  pDumper->writeFieldValue( L"  Print Text Nodes Flag", pPrintStyle->getPrintTextNodesFlag() );
  pDumper->writeFieldValue( L"  Print Transparency Flag", pPrintStyle->getPrintTransparencyFlag() );
  pDumper->writeFieldValue( L"  Use View Background Color For Render Flag", pPrintStyle->getUseViewBackgroundColorForRenderFlag() );

  OdArray<OdDgPrintStyleLevelOverride> arrLevelOverrides = pPrintStyle->getLevelOverrides();

  if( !arrLevelOverrides.isEmpty() )
  {
    pDumper->dumpFieldName(L"  Level Overrides:");

    for( OdUInt32 i = 0; i < arrLevelOverrides.size(); i++ )
    {
      OdString strFieldName;
      OdString strFieldValue;

      strFieldName.format(L"    - Override %d", i);
      
      if( arrLevelOverrides[i].m_bOn )
        strFieldValue = arrLevelOverrides[i].m_strLevelNameRegex + L", " + arrLevelOverrides[i].m_strFileNameRegex + L", true";
      else
        strFieldValue = arrLevelOverrides[i].m_strLevelNameRegex + L", " + arrLevelOverrides[i].m_strFileNameRegex + L", false";

      pDumper->writeFieldValue(strFieldName, strFieldValue);
    }
  }

  OdArray<OdDgPrintStyleReferenceOverride> arrRefOverrides = pPrintStyle->getReferenceOverrides();

  if( !arrRefOverrides.isEmpty() )
  {
    pDumper->dumpFieldName(L"  Reference Overrides:");

    for( OdUInt32 i = 0; i < arrRefOverrides.size(); i++ )
    {
      OdString strFieldName;
      OdString strFieldValue;

      strFieldName.format(L"    - Override %d", i);

      if( arrRefOverrides[i].m_bOn )
        strFieldValue = arrRefOverrides[i].m_strFileNameRegex + L", true";
      else
        strFieldValue = arrRefOverrides[i].m_strFileNameRegex + L", false";

      pDumper->writeFieldValue(strFieldName, strFieldValue);
    }
  }

  pDumper->dumpFieldName( OdString(L"------ [ PDF Section ] ------"));
  pDumper->writeFieldValue( L"  Pdf Paper Size Mode", pPrintStyle->getPdfPaperSizeMode());

  if( pPrintStyle->getPdfPaperSizeMode() == OdDgPrintStyleTableRecord::kDefinedPaperSize )
    pDumper->writeFieldValue( L"  - Paper Size Name", pPrintStyle->getPdfPaperName());

  pDumper->writeFieldValue( L"  Pdf Paper Source", pPrintStyle->getPdfPaperSource());
  pDumper->writeFieldValue( L"  Pdf Full Sheet Flag", pPrintStyle->getPdfFullSheetFlag());
  pDumper->writeFieldValue( L"  Pdf Reduce To Paper Size Flag", pPrintStyle->getPdfReduceToPaperSizeFlag());
  pDumper->writeFieldValue( L"  Pdf Auto Center Flag", pPrintStyle->getPdfAutoCenterFlag());
  pDumper->writeFieldValue( L"  Pdf Print Comments Flag", pPrintStyle->getPdfPrintCommentsFlag());

  pDumper->dumpFieldName( OdString(L"------ [ Raster Section ] ------"));
  pDumper->writeFieldValue( L"  Raster Paper Size Name", pPrintStyle->getRasterPaperName());
  pDumper->writeFieldValue( L"  Raster Paper Orientation", pPrintStyle->getRasterPaperOrientation());
  pDumper->writeFieldValue( L"  Raster Paper Source", pPrintStyle->getRasterPaperSource());
  pDumper->writeFieldValue( L"  Raster Full Sheet Flag", pPrintStyle->getRasterFullSheetFlag());
  pDumper->writeFieldValue( L"  Raster Layout Units", pPrintStyle->getRasterLayoutUnits());
  pDumper->writeFieldValue( L"  Raster Layout Size Scale Mode", pPrintStyle->getRasterLayoutSizeScaleMode());

  switch( pPrintStyle->getRasterLayoutSizeScaleMode() )
  {
    case OdDgPrintStyleTableRecord::kSizeX:
      pDumper->writeFieldValue( L"    - Raster Size X ", pPrintStyle->getRasterLayoutSizeX()); break;
    case OdDgPrintStyleTableRecord::kSizeY:
      pDumper->writeFieldValue( L"    - Raster Size Y ", pPrintStyle->getRasterLayoutSizeY()); break;
    case OdDgPrintStyleTableRecord::kScale:
      pDumper->writeFieldValue( L"    - Raster Scale ", pPrintStyle->getRasterLayoutScale()); break;
  }

  pDumper->writeFieldValue( L"  Raster Rotation", pPrintStyle->getRasterRotation());
  pDumper->writeFieldValue( L"  Raster Layout Origin Mode", pPrintStyle->getRasterLayoutOriginMode());

  if( pPrintStyle->getRasterLayoutOriginMode() == OdDgPrintStyleTableRecord::kUserDefinedOrigin )
    pDumper->writeFieldValue( L"    - Raster Origin", pPrintStyle->getRasterLayoutUserDefinedOrigin());

  pDumper->writeFieldValue( L"  Raster Layout Mirror Mode", pPrintStyle->getRasterLayoutMirrorMode() );
  pDumper->writeFieldValue( L"  Raster Invert Binary Flag", pPrintStyle->getRasterInvertBinaryFlag());

  pDumper->dumpFieldName( OdString(L"--------------------------------"));
}

void OdDgDisplayStyleTableRecordDumperPE::dumpData( OdRxObjectPtr pObj, OdExDgnDumper* pDumper ) const
{
  OdDgElementDumperPE::dumpData(pObj, pDumper);

  OdDgDisplayStyleTableRecordPtr pDisplayStyle = pObj;
  pDumper->writeFieldValue( L"Entry Id", pDisplayStyle->getEntryId());
  pDumper->writeFieldValue( L"Name", pDisplayStyle->getName());
  pDumper->writeFieldValue( L"EnvironmentName", pDisplayStyle->getEnvironmentName());
  pDumper->writeFieldValue( L"EnvironmentTypeDisplayed", pDisplayStyle->getEnvironmentTypeDisplayed());
  pDumper->writeFieldValue( L"GroundPlaneColor", pDisplayStyle->getGroundPlaneColor());
  pDumper->writeFieldValue( L"GroundPlaneHeight", pDisplayStyle->getGroundPlaneHeight());
  pDumper->writeFieldValue( L"GroundPlaneTransparency", pDisplayStyle->getGroundPlaneTransparency());
  pDumper->writeFieldValue( L"ShowGroundFromBelowFlag", pDisplayStyle->getShowGroundFromBelowFlag());
  pDumper->writeFieldValue( L"UseFileOrderForDisplayFlag", pDisplayStyle->getUseFileOrderForDisplayFlag());
  pDumper->writeFieldValue( L"IgnoreGeometryMapsFlag", pDisplayStyle->getIgnoreGeometryMapsFlag());
  pDumper->writeFieldValue( L"IgnorePatternMapsFlag", pDisplayStyle->getIgnorePatternMapsFlag());
  pDumper->writeFieldValue( L"DisplayShadowsFlag", pDisplayStyle->getDisplayShadowsFlag());
  pDumper->writeFieldValue( L"SmoothIgnoreLightsFlag", pDisplayStyle->getSmoothIgnoreLightsFlag());
  pDumper->writeFieldValue( L"UseTransparencyThresholdFlag", pDisplayStyle->getUseTransparencyThresholdFlag());
  pDumper->writeFieldValue( L"UseColorFromMaterialFlag", pDisplayStyle->getUseColorFromMaterialFlag());
  pDumper->writeFieldValue( L"DisplayGroundPlaneFlag", pDisplayStyle->getDisplayGroundPlaneFlag());
  pDumper->writeFieldValue( L"UseElementColorOverrideFlag", pDisplayStyle->getUseElementColorOverrideFlag());
  pDumper->writeFieldValue( L"UseElementTransparencyOverrideFlag", pDisplayStyle->getUseElementTransparencyOverrideFlag());
  pDumper->writeFieldValue( L"UseElementLineStyleOverrideFlag", pDisplayStyle->getUseElementLineStyleOverrideFlag());
  pDumper->writeFieldValue( L"UseElementLineWeightOverrideFlag", pDisplayStyle->getUseElementLineWeightOverrideFlag());
  pDumper->writeFieldValue( L"UseMaterialOverrideFlag", pDisplayStyle->getUseMaterialOverrideFlag());
  pDumper->writeFieldValue( L"UseBackgroundColorOverrideFlag", pDisplayStyle->getUseBackgroundColorOverrideFlag());
  pDumper->writeFieldValue( L"InvisibleToCameraFlag", pDisplayStyle->getInvisibleToCameraFlag());
  pDumper->writeFieldValue( L"UseDisplayHandlerFlag", pDisplayStyle->getUseDisplayHandlerFlag());
  pDumper->writeFieldValue( L"DisplayVisibleEdgesFlag", pDisplayStyle->getDisplayVisibleEdgesFlag());
  pDumper->writeFieldValue( L"DisplayHiddenEdgesFlag", pDisplayStyle->getDisplayHiddenEdgesFlag());
  pDumper->writeFieldValue( L"UseVisibleEdgeColorFlag", pDisplayStyle->getUseVisibleEdgeColorFlag());
  pDumper->writeFieldValue( L"UseVisibleEdgeSolidLineFlag", pDisplayStyle->getUseVisibleEdgeSolidLineFlag());
  pDumper->writeFieldValue( L"UseVisibleEdgeWeightFlag", pDisplayStyle->getUseVisibleEdgeWeightFlag());
  pDumper->writeFieldValue( L"UseHiddenEdgeLineStyleFlag", pDisplayStyle->getUseHiddenEdgeLineStyleFlag());
  pDumper->writeFieldValue( L"UseHiddenEdgeZeroWeightFlag", pDisplayStyle->getUseHiddenEdgeZeroWeightFlag());
  pDumper->writeFieldValue( L"ApplyEdgeStyleToLineFlag", pDisplayStyle->getApplyEdgeStyleToLineFlag());
  pDumper->writeFieldValue( L"HideInPickerFlag", pDisplayStyle->getHideInPickerFlag());
  pDumper->writeFieldValue( L"UsableForViewsFlag", pDisplayStyle->getUsableForViewsFlag());
  pDumper->writeFieldValue( L"UsableForClipVolumesFlag", pDisplayStyle->getUsableForClipVolumesFlag());
  pDumper->writeFieldValue( L"DisplayMode", pDisplayStyle->getDisplayMode());
  pDumper->writeFieldValue( L"VisibleEdgeColor", pDisplayStyle->getVisibleEdgeColor());
  pDumper->writeFieldValue( L"VisibleEdgeWeight", pDisplayStyle->getVisibleEdgeWeight());
  pDumper->writeFieldValue( L"BackgroundColor", pDisplayStyle->getBackgroundColor());
  pDumper->writeFieldValue( L"ElementColor", pDisplayStyle->getElementColor());
  pDumper->writeFieldValue( L"ElementLineStyle", pDisplayStyle->getElementLineStyle());
  pDumper->writeFieldValue( L"ElementLineWeight", pDisplayStyle->getElementLineWeight());
  pDumper->writeFieldValue( L"ElementTransparency", pDisplayStyle->getElementTransparency());
  pDumper->writeFieldValue( L"TransparencyThreshold", pDisplayStyle->getTransparencyThreshold());
  pDumper->writeFieldValueHex( L"MaterialHandle", pDisplayStyle->getMaterialHandle());
  pDumper->writeFieldValue( L"DisplayHeader", pDisplayStyle->getDisplayHeader());
  
  if( pDisplayStyle->getExtendedPropertyCount() )
  {
    pDumper->dumpFieldName(L"Extended properties:");

    for( OdUInt32 i = 0; i < pDisplayStyle->getExtendedPropertyCount(); i++ )
    {
      OdDgXMLProperty curProp = pDisplayStyle->getExtendedProperty(i);

      if( curProp.isXMLNode() )
      {
        pDumper->writeFieldValue( L"  " + curProp.getName() + L":", curProp.asXMLNode());
      }
      else
      {
        pDumper->writeFieldValue( L"  " + curProp.getName(), curProp.asString());
      }
    }
  }
}

void OdDgLineStyleTableRecordDumperPE::dumpData( OdRxObjectPtr pObj, OdExDgnDumper* pDumper ) const
{
  OdDgTableRecordDumperPE::dumpData(pObj, pDumper);

  OdDgLineStyleTableRecordPtr pRec = pObj;

  pDumper->writeFieldValueHex( "EntryId", pRec->getEntryId() );
  pDumper->writeFieldValue   ( "RefersToElement Flag", pRec->getRefersToElementFlag() );
  pDumper->writeFieldValue   ( "Snappable Flag", pRec->getSnappableFlag() );
  pDumper->writeFieldValue   ( "Physical Flag", pRec->getPhysicalFlag() );
  pDumper->writeFieldValue   ( "Units Type", pRec->getUnitsType() );
  pDumper->writeFieldValue   ( "NoSnap Flag", pRec->getNoSnapFlag() );
  pDumper->writeFieldValue   ( "Continuous Flag", pRec->getContinuousFlag() );
  pDumper->writeFieldValue   ( "NoRange Flag", pRec->getNoRangeFlag() );
  pDumper->writeFieldValue   ( "SharedCellScaleIndependent Flag", pRec->getSharedCellScaleIndependentFlag() );
  pDumper->writeFieldValue   ( "NoWidth Flag", pRec->getNoWidthFlag() );
  pDumper->writeFieldValue   ( "RefersToId", pRec->getRefersToId().getHandle().ascii() );
  pDumper->writeFieldValue   ( "Type", (OdUInt16)pRec->getType() );
  pDumper->writeFieldValue   ( "RscFileName", pRec->getRscFileName() );
}

void OdDgLineStyleDefTableRecordDumperPE::dumpData( OdRxObjectPtr pObj, OdExDgnDumper* pDumper ) const
{
  OdDgTableRecordDumperPE::dumpData(pObj, pDumper);

  OdDgLineStyleDefTableRecordPtr pRec = pObj;

  OdDg::LineStyleType type = pRec->getType();
  pDumper->writeFieldValue( "Type", type );

  OdDgLineStyleResourcePtr pRes = pRec->getResource();
  OdDgRxObjectDumperPEPtr pResDumper = OdDgRxObjectDumperPE::cast( pRes );
  if ( pResDumper.get() )
  {
    pResDumper->dump( pRes.get(), pDumper );
  }
}

void OdDgDictionaryTableDumperPE::dumpData( OdRxObjectPtr pObj, OdExDgnDumper* pDumper ) const
{
  OdDgElementDumperPE::dumpData(pObj, pDumper);

  OdDgDictionaryTablePtr pTbl = pObj;
  pDumper->writeFieldValue( "DefaultId", pTbl->getDefaultId() );
  pDumper->writeFieldValue( "OwnerFlags", pTbl->getOwnerFlags() );
  pDumper->writeFieldValue( "CloneFlags", pTbl->getCloneFlags() );
}

void OdDgDictionaryTableRecordDumperPE::dumpData( OdRxObjectPtr pObj, OdExDgnDumper* pDumper ) const
{
  OdDgTableRecordDumperPE::dumpData(pObj, pDumper);

  OdDgDictionaryTableRecordPtr pRec = pObj;
  pDumper->writeFieldValue( "ItemId", pRec->getItemId() );
}

void OdDgDictionaryDumperPE::dumpData( OdRxObjectPtr pObj, OdExDgnDumper* pDumper ) const
{
  OdDgElementDumperPE::dumpData(pObj, pDumper);

  OdDgDictionaryPtr pRec = pObj;
  pDumper->writeFieldValue( "ItemId", pRec->getItemId() );
  pDumper->writeFieldValue( "DefaultId", pRec->getDefaultId() );
  pDumper->writeFieldValue( "OwnerFlags", pRec->getOwnerFlags() );
  pDumper->writeFieldValue( "CloneFlags", pRec->getCloneFlags() );
}

void OdDgRegAppTableRecordDumperPE::dumpData( OdRxObjectPtr pObj, OdExDgnDumper* pDumper ) const
{
  OdDgTableRecordDumperPE::dumpData(pObj, pDumper);

  //OdDgRegAppTableRecordPtr element = pObj;
}

void OdDgViewGroupDumperPE::dumpData( OdRxObjectPtr pObj, OdExDgnDumper* pDumper ) const
{
  OdDgElementDumperPE::dumpData(pObj, pDumper);

  OdDgViewGroupPtr element = pObj;
  pDumper->writeFieldValue( "Name", element->getName() );
  pDumper->writeFieldValue( "ModelId", element->getModelId() );
  pDumper->writeFieldValue( "Active Level", element->getActiveLevelEntryId());
  pDumper->writeFieldValue( "Active Color", element->getActiveColor());
  pDumper->writeFieldValue( "Active Line Style Id", element->getActiveLineStyleId());
  pDumper->writeFieldValue( "Active Line Weight", element->getActiveLineWeight());
  pDumper->writeFieldValue( "Dwg Display Order", element->getDwgDisplayOrder());
}

void OdDgReferenceAttachmentLevelMaskGroupPE::dumpData( OdRxObjectPtr pObj, OdExDgnDumper* pDumper ) const
{
  OdDgElementDumperPE::dumpData(pObj, pDumper);
  OdDgReferenceAttachmentLevelMaskGroupPtr element = pObj;
  pDumper->writeFieldValueHex( "Reference Id", element->getReferenceAttachmentId() );
}

void OdDgViewDumperPE::dumpData( OdRxObjectPtr pObj, OdExDgnDumper* pDumper ) const
{
  OdDgElementDumperPE::dumpData(pObj, pDumper);

  OdDgViewPtr element = pObj;

  pDumper->writeFieldValue( "Model ID", element->getModelId() );

  pDumper->writeFieldValue( "Is named", element->isNamed() );
  pDumper->writeFieldValue( "Name", element->getName() );

  pDumper->writeFieldValue( "Index", element->getIndex() );

  pDumper->writeFieldValue( "View rectangle", element->getViewRectangle() );

  pDumper->writeFieldValue( "Is visible", element->getVisibleFlag() );
  pDumper->writeFieldValue( "Fast curves", element->getFastCurveFlag() );
  pDumper->writeFieldValue( "Hide texts", element->getHideTextsFlag() );
  pDumper->writeFieldValue( "High quality fonts", element->getHighQualityFontsFlag() );
  pDumper->writeFieldValue( "Show line weights", element->getShowLineWeightsFlag() );
  pDumper->writeFieldValue( "Show patterns", element->getShowPatternsFlag() );
  pDumper->writeFieldValue( "Show text nodes", element->getShowTextNodesFlag() );
  pDumper->writeFieldValue( "Show data fields", element->getShowDataFieldsFlag() );
  pDumper->writeFieldValue( "Show grid", element->getShowGridFlag() );
  pDumper->writeFieldValue( "Show level symbology", element->getShowLevelSymbologyFlag() );
  pDumper->writeFieldValue( "Show points", element->getShowPointsFlag() );
  pDumper->writeFieldValue( "Show construction", element->getShowConstructionFlag() );
  pDumper->writeFieldValue( "Show dimensions", element->getShowDimensionsFlag() );
  pDumper->writeFieldValue( "Fast cells", element->getFastCellsFlag() );
  pDumper->writeFieldValue( "Is defined", element->getDefinedFlag() );
  pDumper->writeFieldValue( "Show fills", element->getShowFillsFlag() );
  pDumper->writeFieldValue( "Show raster text", element->getShowRasterTextFlag() );
  pDumper->writeFieldValue( "Show axis triad", element->getShowAxisTriadFlag() );
  pDumper->writeFieldValue( "Orientation display", element->getOrientationDisplayFlag() );
  pDumper->writeFieldValue( "View rendered", element->getViewRenderedFlag() );
  pDumper->writeFieldValue( "Show background", element->getShowBackgroundFlag() );
  pDumper->writeFieldValue( "Show boundary", element->getShowBoundaryFlag() );
  pDumper->writeFieldValue( "Fast boundary clip", element->getFastBoundaryClipFlag() );
  pDumper->writeFieldValue( "Use perspective camera", element->getUseCameraFlag() );
  pDumper->writeFieldValue( "Use depth cueing", element->getUseDepthCueFlag() );
  pDumper->writeFieldValue( "Inhibit dynamics", element->getInhibitDynamicsFlag() );
  pDumper->writeFieldValue( "Show shadows", element->getShowShadowsFlag() );
  pDumper->writeFieldValue( "Show texture maps", element->getShowTextureMapsFlag() );
  pDumper->writeFieldValue( "Show haze", element->getShowHazeFlag() );
  pDumper->writeFieldValue( "Use transparency", element->getUseTransparencyFlag() );
  pDumper->writeFieldValue( "Ignore line styles", element->getIgnoreLineStylesFlag() );
  pDumper->writeFieldValue( "Is accelerated", element->getAcceleratedFlag() );
  pDumper->writeFieldValue( "Is pattern dynamics", element->getPatternDynamicsFlag() );
  pDumper->writeFieldValue( "Hidden lines", element->getHiddenLineFlag() );
  pDumper->writeFieldValue( "Show tags", element->getShowTagsFlag() );
  pDumper->writeFieldValue( "Display edges", element->getDisplayEdgesFlag() );
  pDumper->writeFieldValue( "Display hidden edges", element->getDisplayHiddenEdgesFlag() );
  pDumper->writeFieldValue( "Is named", element->getNamedFlag() );
  pDumper->writeFieldValue( "Override background", element->getOverrideBackgroundFlag() );
  pDumper->writeFieldValue( "Show front clip", element->getShowClipFrontFlag() );
  pDumper->writeFieldValue( "Show back clip", element->getShowClipBackFlag() );
  pDumper->writeFieldValue( "Show volume clip", element->getShowClipVolumeFlag() );
  pDumper->writeFieldValue( "Use display set", element->getUseDisplaySetFlag() );

  pDumper->writeFieldValue( "Display Mode", element->getDisplayMode() );

  if( element->getUseCameraFlag() )
  {
    OdGeMatrix3d rotation;
    OdGePoint3d position;
    OdGeExtents2d rectangle;
    element->getCameraRotation( rotation );
    element->getCameraPosition( position );
    element->getCameraVisibleRectangle( rectangle );

    pDumper->writeFieldValue( "Position", position );
    pDumper->writeFieldValue( "Rotation", rotation );
    pDumper->writeFieldValue( "Focal length", element->getCameraFocalLength() );
    pDumper->writeFieldValue( "Visible rectangle", rectangle );
    pDumper->writeFieldValue( "Front clipping", element->getCameraFrontClippingDistance() );
    pDumper->writeFieldValue( "Back clipping", element->getCameraBackClippingDistance() );
  }
  else
  {
    OdGeMatrix3d rotation;
    OdGeExtents3d box;
    element->getOrthographyRotation( rotation );
    element->getOrthographyVisibleBox( box );

    pDumper->writeFieldValue( "Rotation", rotation );
    pDumper->writeFieldValue( "Visible box", box );
  }
}

void OdDgLevelMaskDumperPE::dumpData( OdRxObjectPtr pObj, OdExDgnDumper* pDumper ) const
{
  OdDgElementDumperPE::dumpData(pObj, pDumper);

  OdDgLevelMaskPtr pLevelMask = pObj;
  OdUInt32 maxLevelEntryId = pLevelMask->getMaxLevelEntryId();
  pDumper->writeFieldValue( "MaxLevelEntryId", maxLevelEntryId );
  pDumper->writeFieldValue( "View Number", pLevelMask->getViewIndex() );

  OdString                       strLevelName;
  OdDgLevelMask::OdDgLevelStatus uStatus;

  if( (maxLevelEntryId > 0) && pLevelMask->getLevelStatus(0, strLevelName, uStatus) )
  {
    pDumper->writeFieldValueHex( L"Reference Id", (OdUInt64)(pLevelMask->getReferenceAttachHandleId()) );

    for( OdUInt32 l = 0; l <= maxLevelEntryId; l++ )
    {
      if( pLevelMask->getLevelStatus(l, strLevelName, uStatus ) )
      {
        if( uStatus == OdDgLevelMask::kLevelVisible )
        {
          pDumper->writeFieldValue( strLevelName, true );
        }
        else if( uStatus == OdDgLevelMask::kLevelHidden)
        {
          pDumper->writeFieldValue( strLevelName, false );
        }
      }

      ODA_ASSERT_ONCE( pLevelMask->getLevelIsVisible(strLevelName) == uStatus );
    }
  }
  else
  {
    OdDgLevelTablePtr pTable;

    if( pLevelMask->getReferenceAttachId().isNull() )
    {
      pDumper->writeFieldValueHex( L"Reference Id", (OdUInt64)(0) );

      bool bXRefMask = false;

      OdDgElementId idOwner = pLevelMask->ownerId();

      if( !idOwner.isNull() )
      {
        OdDgElementPtr pOwner = idOwner.openObject( OdDg::kForRead );

        if( pOwner->getElementType() == OdDgElement::kTypeReferenceAttachmentHeader )
        {
          OdDgReferenceAttachmentHeaderPtr pXRef = pOwner;
          pTable = pXRef->getLevelTable( OdDg::kForRead );
          bXRefMask = true;
        }
      }

      if( !bXRefMask )
      {
        pTable = pLevelMask->database()->getLevelTable();
      }
    }
    else
    {
      pDumper->writeFieldValueHex( L"Reference Id", (OdUInt64)(pLevelMask->getReferenceAttachId().getHandle()) );

      if( !pLevelMask->getReferenceAttachId().isNull() )
      {
        OdDgReferenceAttachmentHeaderPtr pXRef = pLevelMask->getReferenceAttachId().openObject( OdDg::kForRead );
        pTable = pXRef->getLevelTable( OdDg::kForRead );
      }
    }

    if( !pTable.isNull() )
    {
      OdDgElementIteratorPtr pIt = pTable->createIterator();
      for( ; !pIt->done(); pIt->step() )
      {
        OdDgLevelTableRecordPtr pLevel = pIt->item().safeOpenObject();
        OdString levelName = pLevel->getName();
        OdUInt32 levelEntryId = pLevel->getEntryId();
        bool levelIsVisible = true;
        if ( levelEntryId <= maxLevelEntryId || levelEntryId == 64 )
        {
          levelIsVisible = pLevelMask->getLevelIsVisible(levelEntryId);
        }
        pDumper->writeFieldValue( levelName, levelIsVisible );
      }
    }
    else
    {
      pDumper->writeFieldValue(L"Can't load level table","!!!");
    }
  }
}

//----------------------------------------------------------
//
// Elements
//
//----------------------------------------------------------
void OdDgSharedCellDefinitionDumperPE::dumpData( OdRxObjectPtr pObj, OdExDgnDumper* pDumper ) const
{
  OdDgGraphicsElementDumperPE::dumpData(pObj, pDumper);

  OdDgSharedCellDefinitionPtr element = pObj;

  pDumper->writeFieldValue( "Name", element->getName() );
  pDumper->writeFieldValue( "Description", element->getDescription() );

  pDumper->writeFieldValue( "Origin", element->getOrigin() );
}

void OdDgACSDumperPE::dumpData( OdRxObjectPtr pObj, OdExDgnDumper* pDumper ) const
{
  OdDgElementDumperPE::dumpData(pObj, pDumper);

  OdDgACSPtr element = pObj;

  pDumper->writeFieldValue( "Name", element->getName() );
  pDumper->writeFieldValue( "Description", element->getDescription() );
  pDumper->writeFieldValue( "Type", element->getType() );
  pDumper->writeFieldValue( "Origin", element->getOrigin() );
  pDumper->writeFieldValue( "Rotation", element->getRotation() );
}

void OdDgSharedCellReferenceDumperPE::dumpData( OdRxObjectPtr pObj, OdExDgnDumper* pDumper ) const
{
  OdDgGraphicsElementDumperPE::dumpData(pObj, pDumper);

  OdDgSharedCellReferencePtr element = pObj;

  pDumper->writeFieldValue( "Name of definition", element->getDefinitionName() );

  pDumper->writeFieldValue( "Transformation", element->getTransformation() );
  pDumper->writeFieldValue( "Origin", element->getOrigin() );

  pDumper->writeFieldValueHex( "Class map", element->getClassMap() );

  pDumper->writeFieldValue( "Override level", element->getLevelOverrideFlag() );
  pDumper->writeFieldValue( "Override relative", element->getRelativeOverrideFlag() );
  pDumper->writeFieldValue( "Override class", element->getClassOverrideFlag() );
  pDumper->writeFieldValue( "Override color", element->getColorOverrideFlag() );
  pDumper->writeFieldValue( "Override weight", element->getWeightOverrideFlag() );
  pDumper->writeFieldValue( "Override style", element->getStyleOverrideFlag() );
  pDumper->writeFieldValue( "Override associative point", element->getAssociativePointOverrideFlag() );
//   pDumper->writeFieldValue( "Annotation Cell", element->getAnnotationCellFlag() );
//   pDumper->writeFieldValue( "Use Annotation Scale", element->getUseAnnotationScaleFlag() );
//   pDumper->writeFieldValue( "Annotation Scale", element->getAnnotationScale() );
}

void OdDgTagDefinitionSetDumperPE::dumpData( OdRxObjectPtr pObj, OdExDgnDumper* pDumper ) const
{
  OdDgElementDumperPE::dumpData(pObj, pDumper);

  OdDgTagDefinitionSetPtr pTagDefinitionsSet = pObj;

  pDumper->writeFieldValue( "Name", pTagDefinitionsSet->getName() );

  if( pTagDefinitionsSet->getProxyTagDefinitionSetFlag() )
  {
    OdBinaryData binProxyData;
    pTagDefinitionsSet->getBinaryData( binProxyData );

    pDumper->writeFieldValue( "Number of proxy bytes", (OdUInt32)(binProxyData.size()) );
  }

  OdUInt32 j = pTagDefinitionsSet->getCount();
  pDumper->writeFieldValue( "Number of definitions", j );

  for( OdUInt32 i = 0; i < j; i++ )
  {
    OdString strFieldName;
    strFieldName.format(L"  - Tag Definition %d", i);

    if( !pTagDefinitionsSet->getByEntryId(i).isNull() )
    {
      pDumper->writeFieldValue(strFieldName, pTagDefinitionsSet->getByEntryId(i)->getName());
    }
  }

//    OdRxObjectPtr                   pTagObj;
//    OdSmartPtr< OdDgTagDefinitionDumperPE > dumper;
//
//    for( OdUInt32 i = 0; i < j; i++ )
//    {
//      pTagObj = pTagDefinitionsSet->getByIndex( i );
//      dumper = pTagObj;
//      dumper->dumpData( pTagObj, pDumper );
//    }
}

void OdDgItemTypeLibraryDumperPE::dumpData( OdRxObjectPtr pObj, OdExDgnDumper* pDumper ) const
{
  OdDgElementDumperPE::dumpData(pObj, pDumper);

  OdDgItemTypeLibraryPtr pItemLib = pObj;

  pDumper->writeFieldValue( "Name", convertItemNameToDisplayLabel(pItemLib->getName()) );
  pDumper->writeFieldValue( "Description", pItemLib->getDescription() );
  pDumper->writeFieldValue( "DisplayLabel", pItemLib->getDisplayLabel() );

  if( pItemLib->getIsProxyFlag() )
  {
    pDumper->writeFieldValue( "Contain Proxy Data", OdString("true") );
  }
  else
  {
    if( pItemLib->getItemTypeCount() > 0 )
    {
      pDumper->dumpFieldName(L"Items:");
    }
    for( OdUInt32 i = 0; i < pItemLib->getItemTypeCount(); i++ )
    {
      OdDgItemTypePtr pItem = pItemLib->getItemType(i);
      OdString strName;
      strName.format(L"- Item %d Name", i);
      pDumper->writeFieldValue(strName, convertItemNameToDisplayLabel(pItem->getName()));
      pDumper->writeFieldValue("    Display Label", pItem->getDisplayLabel());
      pDumper->writeFieldValue("    Description", pItem->getDescription());
      pDumper->writeFieldValue("    Category", pItem->getCategory());
      pDumper->writeFieldValueHex("    Item Lib Id", pItem->getEntryId());
      pDumper->writeFieldValue("    Use Name For Element Flag", pItem->getUseNameForElementFlag());

      if( pItem->getPropertyCount() > 0 )
      {
        pDumper->dumpFieldName(L"    Properties:");

        for( OdUInt32 j = 0; j < pItem->getPropertyCount(); j++ )
        {
          OdDgItemTypePropertyPtr pProperty = pItem->getProperty(j);

          OdString strTypeName = convertItemNameToDisplayLabel(pProperty->getTypeName());
          
          if( pProperty->getArrayFlag() )
          {
            strTypeName = L"    ... " + strTypeName;
          }
          else if( pProperty->getReadOnlyFlag() )
          {
            strTypeName = L"    [-] " + strTypeName;
          }
          else if( pProperty->getUsePropertyTypeFlag() )
          {
            strTypeName = L"    [+] " + strTypeName;
          }
          else 
          {
            strTypeName = L"     =  " + strTypeName;
          }

          pDumper->writeFieldValue(strTypeName, convertItemNameToDisplayLabel(pProperty->getName()) );

          if( !pProperty->getDescription().isEmpty() )
          {
            pDumper->writeFieldValue(L"         description", pProperty->getDescription());
          }

          if( pProperty->hasDefaultValue() )
          {
            pDumper->writeFieldValue(L"         defaultValue", pProperty->getDefaultValue());
          }

          if( pProperty->getUnits().getUnitsType() != OdDgItemTypePropertyUnits::kUndefined )
          {
            pDumper->writeFieldValue(L"         units", pProperty->getUnits());
          }
        }
      }
    }
  }
}

void OdDgXMLCustomSchemaContainerDumperPE::dumpData( OdRxObjectPtr pObj, OdExDgnDumper* pDumper ) const
{
  OdDgElementDumperPE::dumpData(pObj, pDumper);

  OdDgXMLCustomSchemaContainerPtr pContainer = pObj;

  pDumper->writeFieldValue( "Name", convertItemNameToDisplayLabel(pContainer->getName()) );

  if( !pContainer->getDescription().isEmpty() )
    pDumper->writeFieldValue( "Description", pContainer->getDescription() );

  if( !pContainer->getDisplayName().isEmpty() )
    pDumper->writeFieldValue( "DisplayLabel", pContainer->getDisplayName() );

  if( pContainer->getCustomItemCount() > 0 )
      pDumper->dumpFieldName(L"Custom Items:");

  for( OdUInt32 i = 0; i < pContainer->getCustomItemCount(); i++ )
  {
    OdDgCustomItemTypePtr pItem = pContainer->getCustomItem(i);
    OdString strName;
    strName.format(L"- Item %d Name", i);
    pDumper->writeFieldValue(strName, convertItemNameToDisplayLabel(pItem->getName()));
    pDumper->writeFieldValue("    Display Label", pItem->getDisplayLabel());
    pDumper->writeFieldValue("    Description", pItem->getDescription());

    if( pItem->getPropertyCount() > 0 )
    {
      pDumper->dumpFieldName(L"    Properties:");

      for( OdUInt32 j = 0; j < pItem->getPropertyCount(); j++ )
      {
        OdDgCustomItemTypePropertyPtr pProperty = pItem->getProperty(j);

        OdString strTypeName = convertItemNameToDisplayLabel(pProperty->getTypeName());

        if( pProperty->getArrayFlag() )
          strTypeName = L"    ... " + strTypeName;
        else if( pProperty->getReadOnlyFlag() )
          strTypeName = L"    [-] " + strTypeName;
        else 
          strTypeName = L"     =  " + strTypeName;

        pDumper->writeFieldValue(strTypeName, convertItemNameToDisplayLabel(pProperty->getName()) );

        if( !pProperty->getDescription().isEmpty() )
          pDumper->writeFieldValue(L"         description", pProperty->getDescription());
      }
    }
  }
}

void OdDgTagDefinitionDumperPE::dumpData( OdRxObjectPtr pObj, OdExDgnDumper* pDumper ) const
{
  OdDgTagDefinitionPtr  element = pObj;

  pDumper->writeFieldValue( "Type", element->getType() );
  pDumper->writeFieldValue( "ID", element->getEntryId() );
  pDumper->writeFieldValue( "Name", element->getName() );
  pDumper->writeFieldValue( "Prompt", element->getPrompt() );
  pDumper->writeFieldValue( "Variable", element->isVariable() );
  switch( element->getType() )
  {
  case OdDgTagDefinition::kChar:
    pDumper->writeFieldValue( "Default char value", element->getString() );
    break;
  case OdDgTagDefinition::kInt16 :
    pDumper->writeFieldValue( "Default int16 value", element->getInt16() );
    break;
  case OdDgTagDefinition::kInt32 :
    pDumper->writeFieldValue( "Default int32 value", element->getInt32() );
    break;
  case OdDgTagDefinition::kDouble:
    pDumper->writeFieldValue( "Default double value", element->getDouble() );
    break;
  case OdDgTagDefinition::kBinary:
    pDumper->writeFieldValue( "Default binary data (size)", element->getBinarySize() );
    break;
  default:
    ODA_FAIL();
  }


}

void OdDgColorTableDumperPE::dumpData( OdRxObjectPtr pObj, OdExDgnDumper* pDumper ) const
{
  OdDgElementDumperPE::dumpData(pObj, pDumper);

  OdDgColorTablePtr element = pObj;
  int               i;
  char              name[ 20 ];



  for( i = 0; i < 256; i++ )
  {
    sprintf( name, "Color %d", i );
    pDumper->writeFieldValue( name, OdUInt32( i ) );
  }
}

void OdDgApplicationDataDumperPE::dumpData( OdRxObjectPtr pObj, OdExDgnDumper* pDumper ) const
{
  OdDgElementDumperPE::dumpData(pObj, pDumper);

  OdDgApplicationDataPtr pElm = pObj;

  OdBinaryData data;
  pElm->getData(data);
  pDumper->writeFieldValueHex( "Signature", pElm->getSignature() );
  pDumper->writeFieldValue( "Binary data size", (OdUInt32)data.size() );
}

void OdDgReferenceOverrideDumperPE::dumpData( OdRxObjectPtr pObj, OdExDgnDumper* pDumper ) const
{
  OdDgElementDumperPE::dumpData(pObj, pDumper);

  OdDgReferenceOverridePtr pRefOverride = pObj;

  pDumper->writeFieldValue( "Display Flag", pRefOverride->getDisplayFlag() );
  pDumper->writeFieldValue( "Display Override Flag", pRefOverride->getDisplayOverrideFlag() );
  pDumper->writeFieldValue( "Snap Flag", pRefOverride->getSnapFlag() );
  pDumper->writeFieldValue( "Snap Override Flag", pRefOverride->getSnapOverrideFlag() );
  pDumper->writeFieldValue( "Locate Flag", pRefOverride->getLocateFlag() );
  pDumper->writeFieldValue( "Locate Override Flag", pRefOverride->getLocateOverrideFlag() );

  OdUInt64Array path;
  pRefOverride->getPathOfXRef( path );

  OdString strPath;

  for( OdUInt32 i = 0; i < path.size(); i++ )
  {
    OdString strPathPart;

    if( i > 0 )
    {
      strPathPart.format(L"->0x%X", path[i] );
    }
    else
    {
      strPathPart.format(L"0x%X", path[i] );
    }

    strPath += strPathPart;
  }

  pDumper->writeFieldValue( "Path of X-Ref", strPath );
}

void OdDgReferenceOrderDumperPE::dumpData( OdRxObjectPtr pObj, OdExDgnDumper* pDumper ) const
{
  OdDgElementDumperPE::dumpData(pObj, pDumper);

  OdDgReferenceOrderPtr pElm = pObj;

  pDumper->writeFieldValue( "Number of references", pElm->getNumberOfReferences() );

  for( OdUInt32 i = 0; i < pElm->getNumberOfReferences(); i++ )
  {
    OdString strFieldName;
    strFieldName.format(L"  Reference Id %d", i);
    pDumper->writeFieldValueHex( strFieldName, pElm->getReferenceId(i) );
  }
}

void OdDgReferenceAttachmentHeaderDumperPE::dumpData( OdRxObjectPtr pObj, OdExDgnDumper* pDumper ) const
{
  OdDgElementDumperPE::dumpData(pObj, pDumper);

  OdDgReferenceAttachmentHeaderPtr  element = pObj;

  pDumper->writeFieldValue( "Level Entry Id", element->getLevelEntryId() );
  pDumper->writeFieldValue( "The full reference path", element->getFullFileName() );
  pDumper->writeFieldValue( "The base file name", element->getFileName() );
  pDumper->writeFieldValue( "ModelName", element->getModelName() );
  pDumper->writeFieldValue( "LogicalName", element->getLogicalName() );
  pDumper->writeFieldValue( "MasterOrigin", element->getMasterOrigin() );
  pDumper->writeFieldValue( "ReferenceOrigin", element->getReferenceOrigin() );
  pDumper->writeFieldValue( "Transformation", element->getTransformation() );

  pDumper->writeFieldValue( "Description", element->getDescription() );
  pDumper->writeFieldValue( "RevisionName", element->getRevisionName() );
  pDumper->writeFieldValue( "FileNumber", element->getFileNumber() );
  pDumper->writeFieldValue( "Priority", (OdInt32)(element->getPriority()) );
  pDumper->writeFieldValue( "BaseNestDepth", element->getBaseNestDepth() );
  pDumper->writeFieldValue( "NestDepth", element->getNestDepth() );
  pDumper->writeFieldValue( "Scale", element->getScale() );

  try
  {
    pDumper->writeFieldValue( "Entire Scale", element->getEntireScale() );
  }
  catch(...)
  {
  	pDumper->writeFieldValue( "Entire Scale", OdString(OD_T("Can't be calculated")) );
  }

  pDumper->writeFieldValue( "ZFront", element->getZFront() );
  pDumper->writeFieldValue( "ZBack", element->getZBack() );
  pDumper->writeFieldValue( "CameraPosition", element->getCameraPosition() );
  pDumper->writeFieldValue( "CameraFocalLength", element->getCameraFocalLength() );

  pDumper->writeFieldValue( "ClipPointsCount", element->getClipPointsCount() );
  char  name[ 20 ];
  for (OdUInt32 i = 0, nCount = element->getClipPointsCount(); i < nCount; i++)
  {

    sprintf( name, "ClipPoint %d", i );
    pDumper->writeFieldValue( name, element->getClipPoint( i ) );
  }

  pDumper->writeFieldValue( "CoincidentFlag", element->getCoincidentFlag() );
  pDumper->writeFieldValue( "SnapLockFlag", element->getSnapLockFlag() );
  pDumper->writeFieldValue( "LocateLockFlag", element->getLocateLockFlag() );
  pDumper->writeFieldValue( "CompletePathInV7Flag", element->getCompletePathInV7Flag() );
  pDumper->writeFieldValue( "AnonymousFlag", element->getAnonymousFlag() );
  pDumper->writeFieldValue( "InactiveFlag", element->getInactiveFlag() );
  pDumper->writeFieldValue( "MissingFileFlag", element->getMissingFileFlag() );
  pDumper->writeFieldValue( "LevelOverride", element->getLevelOverride() );
  pDumper->writeFieldValue( "DontDetachOnAllFlag", element->getDontDetachOnAllFlag() );
  pDumper->writeFieldValue( "MetadataOnlyFlag", element->getMetadataOnlyFlag() );
  pDumper->writeFieldValue( "DisplayFlag", element->getDisplayFlag() );
  pDumper->writeFieldValue( "LineStyleScaleFlag", element->getLineStyleScaleFlag() );
  pDumper->writeFieldValue( "HiddenLineFlag", element->getHiddenLineFlag() );
  pDumper->writeFieldValue( "DisplayHiddenLinesFlag", element->getDisplayHiddenLinesFlag() );
  pDumper->writeFieldValue( "RotateClippingFlag", element->getRotateClippingFlag() );
  pDumper->writeFieldValue( "ExtendedRefFlag", element->getExtendedRefFlag() );
  pDumper->writeFieldValue( "ClipBackFlag", element->getClipBackFlag() );
  pDumper->writeFieldValue( "ClipFrontFlag", element->getClipFrontFlag() );
  pDumper->writeFieldValue( "CameraOnFlag", element->getCameraOnFlag() );
  pDumper->writeFieldValue( "TrueScaleFlag", element->getTrueScaleFlag() );
  pDumper->writeFieldValue( "DisplayBoundaryFlag", element->getDisplayBoundaryFlag() );
  pDumper->writeFieldValue( "LibraryRefFlag", element->getLibraryRefFlag() );
  pDumper->writeFieldValue( "DisplayRasterRefsFlag", element->getDisplayRasterRefsFlag() );
  pDumper->writeFieldValue( "UseAlternateFileFlag", element->getUseAlternateFileFlag() );
  pDumper->writeFieldValue( "UseLightsFlag", element->getUseLightsFlag() );
  pDumper->writeFieldValue( "DoNotDisplayAsNestedFlag", element->getDoNotDisplayAsNestedFlag() );
  pDumper->writeFieldValue( "ColorTableUsage", element->getColorTableUsage() );
  pDumper->writeFieldValue( "ViewportFlag", element->getViewportFlag() );
  pDumper->writeFieldValue( "ScaleByStorageUnitsFlag", element->getScaleByStorageUnitsFlag() );
  pDumper->writeFieldValue( "PrintColorAdjustmentFlag", element->getPrintColorAdjustmentFlag() );
  pDumper->writeFieldValue( "UseViewFlags", element->getUseViewFlagsFlag() );

  OdDgReferenceAttachmentViewFlags flagView0 = element->getViewFlags(0);

  pDumper->dumpFieldName("View 0 Flags:");
  pDumper->writeFieldValue( " Fast curves", flagView0.getFastCurveFlag() );
  pDumper->writeFieldValue( " Hide texts", flagView0.getHideTextsFlag() );
  pDumper->writeFieldValue( " High quality fonts", flagView0.getHighQualityFontsFlag() );
  pDumper->writeFieldValue( " Show line weights", flagView0.getShowLineWeightsFlag() );
  pDumper->writeFieldValue( " Show patterns", flagView0.getShowPatternsFlag() );
  pDumper->writeFieldValue( " Show text nodes", flagView0.getShowTextNodesFlag() );
  pDumper->writeFieldValue( " Show data fields", flagView0.getShowDataFieldsFlag() );
  pDumper->writeFieldValue( " Show grid", flagView0.getShowGridFlag() );
  pDumper->writeFieldValue( " Show level symbology", flagView0.getShowLevelSymbologyFlag() );
  pDumper->writeFieldValue( " Show points", flagView0.getShowPointsFlag() );
  pDumper->writeFieldValue( " Show construction", flagView0.getShowConstructionFlag() );
  pDumper->writeFieldValue( " Show dimensions", flagView0.getShowDimensionsFlag() );
  pDumper->writeFieldValue( " Fast cells", flagView0.getFastCellsFlag() );
  pDumper->writeFieldValue( " Show fills", flagView0.getShowFillsFlag() );
  pDumper->writeFieldValue( " Show raster text", flagView0.getRasterTextFlag() );
  pDumper->writeFieldValue( " Show axis triad", flagView0.getShowAxisTriadFlag() );
  pDumper->writeFieldValue( " Orientation display", flagView0.getOrientationDisplayFlag() );
  pDumper->writeFieldValue( " View rendered", flagView0.getViewRenderedFlag() );
  pDumper->writeFieldValue( " Show background", flagView0.getShowBackgroundFlag() );
  pDumper->writeFieldValue( " Show boundary", flagView0.getShowBoundaryFlag() );
  pDumper->writeFieldValue( " Fast boundary clip", flagView0.getFastBoundaryClipFlag() );
  pDumper->writeFieldValue( " Use depth cueing", flagView0.getDepthCueFlag() );
  pDumper->writeFieldValue( " Inhibit dynamics", flagView0.getInhibitDynamicsFlag() );
  pDumper->writeFieldValue( " Show shadows", flagView0.getShowShadowsFlag() );
  pDumper->writeFieldValue( " Show texture maps", flagView0.getShowTextureMapsFlag() );
  pDumper->writeFieldValue( " Show haze", flagView0.getShowHazeFlag() );
  pDumper->writeFieldValue( " Use transparency", flagView0.getTransparencyFlag() );
  pDumper->writeFieldValue( " Ignore line styles", flagView0.getIgnoreLineStylesFlag() );
  pDumper->writeFieldValue( " Is accelerated", flagView0.getAcceleratedFlag() );
  pDumper->writeFieldValue( " Is pattern dynamics", flagView0.getPatternDynamicsFlag() );
  pDumper->writeFieldValue( " Hidden lines", flagView0.getHiddenLineFlag() );
  pDumper->writeFieldValue( " Show tags", flagView0.getShowTagsFlag() );
  pDumper->writeFieldValue( " Display edges", flagView0.getDisplayEdgesFlag() );
  pDumper->writeFieldValue( " Display hidden edges", flagView0.getDisplayHiddenEdgesFlag() );
  pDumper->writeFieldValue( " Override background", flagView0.getOverrideBackgroundFlag() );
  pDumper->writeFieldValue( " Show front clip", flagView0.getShowClipFrontFlag() );
  pDumper->writeFieldValue( " Show back clip", flagView0.getShowClipBackFlag() );
  pDumper->writeFieldValue( " Show volume clip", flagView0.getShowClipVolumeFlag() );
  pDumper->writeFieldValue( " Use display set", flagView0.getUseDisplaySetFlag() );

  pDumper->writeFieldValue( " Display Mode", flagView0.getDisplayMode() );
  
  OdGeExtents3d extData;
  element->getGeomExtents(extData);
  pDumper->writeFieldValue( " Geometry extents", extData );
}

void OdDgGraphicsElementDumperPE::dumpData( OdRxObjectPtr pObj, OdExDgnDumper* pDumper ) const
{
  OdDgElementDumperPE::dumpData(pObj, pDumper);

  OdDgGraphicsElementPtr pElm = pObj;

  // Modification time
  OdTimeStamp tmp;
  tmp.setPackedValue( (OdUInt64)(pElm->getLastModifiedTime())/1000);
  pDumper->writeFieldValue( "Last Modified Time", tmp );

  pDumper->writeFieldValue( "Level ID", pElm->getLevelEntryId() );
  pDumper->writeFieldColorValue( "Color index", pElm->getColorIndex() );
  pDumper->writeFieldValue( "Graphics group", pElm->getGraphicsGroupEntryId() );
  pDumper->writeFieldValue( "Class", pElm->getClass() );
  pDumper->writeFieldLineStyleValue( "Line style", (OdUInt32)pElm->getLineStyleEntryId() );
  pDumper->writeFieldLineWeightValue( "Line weight", pElm->getLineWeight() );
  pDumper->writeFieldValue( "Thickness", pElm->getThickness() );
  pDumper->writeFieldValue( "Priority", pElm->getPriority() );
  pDumper->writeFieldValue( "Is Invisible", pElm->getInvisibleFlag() );
  pDumper->writeFieldValue( "Is 3D Format Element", pElm->get3dFormatFlag() );
  pDumper->writeFieldValue( "View Independent", pElm->getViewIndependentFlag() );
  pDumper->writeFieldValue( "Non Planar", pElm->getNonPlanarFlag() );
  pDumper->writeFieldValue( "Not Snappable", pElm->getNotSnappableFlag() );
  pDumper->writeFieldValue( "Hbit", pElm->getHbitFlag() );

  // Extents
  {
    OdGeExtents3d extent;
    if ( pElm->getGeomExtents( extent ) == eOk )
    {
      pDumper->writeFieldValue( "Extent", extent );
    }
    else
    {
      pDumper->writeFieldValue( "Extent", OdString(OD_T("Invalid value")) );
    }
  }

  // Range
  {
    OdDgGraphicsElementPEPtr pRangeData = pElm;

    OdGeExtents3d extent;
    if ( pRangeData->getRange( pElm, extent ) == eOk )
    {
      pDumper->writeFieldValue( "Range", extent );
    }
    else
    {
      pDumper->writeFieldValue( "Range", OdString(OD_T("Invalid value")) );
    }
  }

  // Item types

  OdDgItemTypesGraphicsElementPEPtr pItemTypePE = pElm;

  OdUInt32 nItems = pItemTypePE->getItemTypeInstanceCount(pElm);

  if( !pItemTypePE.isNull() && (nItems > 0) )
  {
    pDumper->dumpFieldName( "Item Type Properties:" );

    for( OdUInt32 i = 0; i < nItems; i++ )
    {
      OdDgItemTypeInstance curItemType = pItemTypePE->getItemTypeInstance( pElm, i );

      for( OdUInt32 j = 0; j < curItemType.getPropertyCount(); j++ )
      {
        OdDgItemTypePropertyInstance curProp = curItemType.getProperty(j);

        if( (curProp.getValue().getType() != OdDgItemTypePropertyInstanceValue::kComplexProperty) && 
            (curProp.getValue().getType() != OdDgItemTypePropertyInstanceValue::kComplexPropertyArray)
          )
        {
          OdString strPropName = L" - " + convertItemNameToDisplayLabel(curProp.getName());
          pDumper->writeFieldValue( strPropName, curProp.getValue() );
        }
        else
        {
          pDumper->dumpFieldName(L" + " + convertItemNameToDisplayLabel(curProp.getName()));
          OdString strPropName = L"   - " + convertItemNameToDisplayLabel(curProp.getName());
          pDumper->writeFieldValue( strPropName, curProp.getValue() );
        }
      }
    }
  }
}

void OdDgTerrainGraphicsDumperPE::dumpData( OdRxObjectPtr pObj, OdExDgnDumper* pDumper ) const
{
  OdDgGraphicsElementDumperPE::dumpData(pObj, pDumper);

  OdDgTerrainPtr pTerrain = pObj;

  OdDgElementId idCtrlRef = pTerrain->getTerrainControlElementId();
  pDumper->writeFieldValueHex( L"Control Element Id", (OdUInt64)(idCtrlRef.getHandle()) );

  OdGeMatrix3d matTransform = pTerrain->getTransformation();
  pDumper->writeFieldValue( L"Transformation Matrix", matTransform );

  for( OdUInt32 i = OdDgTerrainOptions::kTriangles; i < OdDgTerrainOptions::kSourceContour + 1; i++ )
  {
    OdDgTerrainOptionsPtr pOptions = pTerrain->getOptions((OdDgTerrainOptions::OdDgTerrainOptionsType)(i));

    pDumper->writeFieldValue( L"Terrain Graphics Settings", pOptions->getType() );
    pDumper->writeFieldValue( L"  Level Entry Id", pOptions->getLevelEntryId() );
    pDumper->writeFieldColorValue( L"  Color Index", (OdInt32)pOptions->getColorIndex() );
    pDumper->writeFieldLineStyleValue( L"  Line Style Entry Id", (OdInt32)pOptions->getLineStyleEntryId() );
    pDumper->writeFieldLineWeightValue( L"  Line Weight", (OdInt32)pOptions->getLineWeight() );
    pDumper->writeFieldValue( L"  Transparency", pOptions->getTransparency() );
    pDumper->writeFieldValue( L"  Is Visible", pOptions->getOnFlag() );

    switch( i )
    {
      case OdDgTerrainOptions::kTriangles:
      {
        OdDgTerrainOptionsTrianglesPtr pTriangleOptions = pOptions;
        pDumper->writeFieldValueHex( L"  Material Id", pTriangleOptions->getMaterialId() );
        pDumper->writeFieldValue( L"  Display Style Index", pTriangleOptions->getDisplayStyleEntryId() );
      } break;

      case OdDgTerrainOptions::kTriangleVertices:
      {
        OdDgTerrainOptionsTriangleVerticesPtr pVerticesOptions = pOptions;
        pDumper->writeFieldValue( L"  Point Display Type", pVerticesOptions->getPointDisplayType() );

        if( pVerticesOptions->getPointDisplayType() == OdDgTerrainOptions::kCell )
          pDumper->writeFieldValue( L"    Cell Name", pVerticesOptions->getCellName() );
        else if( pVerticesOptions->getPointDisplayType() == OdDgTerrainOptions::kSymbol )
          pDumper->writeFieldValue( L"    Symbol Code", pVerticesOptions->getCharSymbol() );

        pDumper->writeFieldValue( L"  Display Text Flag", pVerticesOptions->getDisplayTextFlag() );
        pDumper->writeFieldValue( L"  Cell Scale", pVerticesOptions->getCellScale() );
        pDumper->writeFieldValue( L"  Text Style", pVerticesOptions->getTextStyleEntryId() );
        pDumper->writeFieldValue( L"  Prefix", pVerticesOptions->getPrefix() );
        pDumper->writeFieldValue( L"  Suffix", pVerticesOptions->getSuffix() );
      } break;

      case OdDgTerrainOptions::kSourceVertex:
      {
        OdDgTerrainOptionsSourceVerticesPtr pVerticesOptions = pOptions;
        pDumper->writeFieldValue( L"  Point Display Type", pVerticesOptions->getPointDisplayType() );

        if( pVerticesOptions->getPointDisplayType() == OdDgTerrainOptions::kCell )
          pDumper->writeFieldValue( L"    Cell Name", pVerticesOptions->getCellName() );
        else if( pVerticesOptions->getPointDisplayType() == OdDgTerrainOptions::kSymbol )
          pDumper->writeFieldValue( L"    Symbol Code", pVerticesOptions->getCharSymbol() );

        pDumper->writeFieldValue( L"  Display Text Flag", pVerticesOptions->getDisplayTextFlag() );
        pDumper->writeFieldValue( L"  Cell Scale", pVerticesOptions->getCellScale() );
        pDumper->writeFieldValue( L"  Text Style", pVerticesOptions->getTextStyleEntryId() );
        pDumper->writeFieldValue( L"  Prefix", pVerticesOptions->getPrefix() );
        pDumper->writeFieldValue( L"  Suffix", pVerticesOptions->getSuffix() );
      } break;

      case OdDgTerrainOptions::kLowPoints:
      {
        OdDgTerrainOptionsLowPointsPtr pVerticesOptions = pOptions;
        pDumper->writeFieldValue( L"  Point Display Type", pVerticesOptions->getPointDisplayType() );

        if( pVerticesOptions->getPointDisplayType() == OdDgTerrainOptions::kCell )
          pDumper->writeFieldValue( L"    Cell Name", pVerticesOptions->getCellName() );
        else if( pVerticesOptions->getPointDisplayType() == OdDgTerrainOptions::kSymbol )
          pDumper->writeFieldValue( L"    Symbol Code", pVerticesOptions->getCharSymbol() );

        pDumper->writeFieldValue( L"  Display Text Flag", pVerticesOptions->getDisplayTextFlag() );
        pDumper->writeFieldValue( L"  Cell Scale", pVerticesOptions->getCellScale() );
        pDumper->writeFieldValue( L"  Text Style", pVerticesOptions->getTextStyleEntryId() );
        pDumper->writeFieldValue( L"  Prefix", pVerticesOptions->getPrefix() );
        pDumper->writeFieldValue( L"  Suffix", pVerticesOptions->getSuffix() );
        pDumper->writeFieldValue( L"  Min Value", pVerticesOptions->getMinValue() );
      } break;

      case OdDgTerrainOptions::kHighPoints:
      {
        OdDgTerrainOptionsHighPointsPtr pVerticesOptions = pOptions;
        pDumper->writeFieldValue( L"  Point Display Type", pVerticesOptions->getPointDisplayType() );

        if( pVerticesOptions->getPointDisplayType() == OdDgTerrainOptions::kCell )
          pDumper->writeFieldValue( L"    Cell Name", pVerticesOptions->getCellName() );
        else if( pVerticesOptions->getPointDisplayType() == OdDgTerrainOptions::kSymbol )
          pDumper->writeFieldValue( L"    Symbol Code", pVerticesOptions->getCharSymbol() );

        pDumper->writeFieldValue( L"  Display Text Flag", pVerticesOptions->getDisplayTextFlag() );
        pDumper->writeFieldValue( L"  Cell Scale", pVerticesOptions->getCellScale() );
        pDumper->writeFieldValue( L"  Text Style", pVerticesOptions->getTextStyleEntryId() );
        pDumper->writeFieldValue( L"  Prefix", pVerticesOptions->getPrefix() );
        pDumper->writeFieldValue( L"  Suffix", pVerticesOptions->getSuffix() );
      } break;

      case OdDgTerrainOptions::kFlowArrows:
      {
        OdDgTerrainOptionsFlowArrowsPtr pVerticesOptions = pOptions;
        pDumper->writeFieldValue( L"  Point Display Type", pVerticesOptions->getPointDisplayType() );

        if( pVerticesOptions->getPointDisplayType() == OdDgTerrainOptions::kCell )
          pDumper->writeFieldValue( L"    Cell Name", pVerticesOptions->getCellName() );
        else if( pVerticesOptions->getPointDisplayType() == OdDgTerrainOptions::kSymbol )
          pDumper->writeFieldValue( L"    Symbol Code", pVerticesOptions->getCharSymbol() );

        pDumper->writeFieldValue( L"  Cell Scale", pVerticesOptions->getCellScale() );
      } break;

      case OdDgTerrainOptions::kMinorContour:
      {
        OdDgTerrainOptionsMinorContourPtr pContourOptions = pOptions;
        pDumper->writeFieldValue( L"  Ignore Max Slopes Flag", pContourOptions->getIgnoreMaxSlopesFlag() );
        pDumper->writeFieldValue( L"  Max Slope Value", pContourOptions->getMaxSlopeValue() );
        pDumper->writeFieldValue( L"  Contour Label Precision", pContourOptions->getContourLabelPrecision() );
        pDumper->writeFieldValue( L"  Smoothing Factor", pContourOptions->getSmoothingFactor() );
        pDumper->writeFieldValue( L"  Smoothing Type", pContourOptions->getSmoothingType() );
        pDumper->writeFieldValue( L"  Interval", pContourOptions->getInterval() );
        pDumper->writeFieldValue( L"  Display Text Flag", pContourOptions->getDisplayTextFlag() );
        pDumper->writeFieldValue( L"  Text Style", pContourOptions->getTextStyleEntryId() );
        pDumper->writeFieldValue( L"  Text Interval", pContourOptions->getTextInterval() );
        pDumper->writeFieldValue( L"  UORs Per Working Units Scale", pContourOptions->getUORPerWorkingUnitsScale() );
        pDumper->writeFieldColorValue( L"  Depression Color Index", pContourOptions->getDepressionColorIndex() );
        pDumper->writeFieldLineStyleValue( L"  Depression Line Style", pContourOptions->getDepressionLineStyleEntryId() );
        pDumper->writeFieldLineWeightValue( L"  Depression Line Weight", pContourOptions->getDepressionLineWeight() );
      } break;

      case OdDgTerrainOptions::kMajorContour:
      {
        OdDgTerrainOptionsMajorContourPtr pContourOptions = pOptions;
        pDumper->writeFieldValue( L"  Ignore Max Slopes Flag", pContourOptions->getIgnoreMaxSlopesFlag() );
        pDumper->writeFieldValue( L"  Max Slope Value", pContourOptions->getMaxSlopeValue() );
        pDumper->writeFieldValue( L"  Contour Label Precision", pContourOptions->getContourLabelPrecision() );
        pDumper->writeFieldValue( L"  Smoothing Factor", pContourOptions->getSmoothingFactor() );
        pDumper->writeFieldValue( L"  Smoothing Type", pContourOptions->getSmoothingType() );
        pDumper->writeFieldValue( L"  Interval", pContourOptions->getInterval() );
        pDumper->writeFieldValue( L"  Display Text Flag", pContourOptions->getDisplayTextFlag() );
        pDumper->writeFieldValue( L"  Text Style", pContourOptions->getTextStyleEntryId() );
        pDumper->writeFieldValue( L"  Text Interval", pContourOptions->getTextInterval() );
        pDumper->writeFieldValue( L"  UORs Per Working Units Scale", pContourOptions->getUORPerWorkingUnitsScale() );
        pDumper->writeFieldColorValue( L"  Depression Color Index", pContourOptions->getDepressionColorIndex() );
        pDumper->writeFieldLineStyleValue( L"  Depression Line Style", pContourOptions->getDepressionLineStyleEntryId() );
        pDumper->writeFieldLineWeightValue( L"  Depression Line Weight", pContourOptions->getDepressionLineWeight() );
      } break;
    }
  }
}

void OdDgLine2dDumperPE::dumpData( OdRxObjectPtr pObj, OdExDgnDumper* pDumper ) const
{
  OdDgGraphicsElementDumperPE::dumpData(pObj, pDumper);

  OdDgLine2dPtr pElm = pObj;
  pDumper->writeFieldValue( "Vertex 1", pElm->getStartPoint( ) );
  pDumper->writeFieldValue( "Vertex 2", pElm->getEndPoint( ) );
  pDumper->writeFieldValue( "Length", pElm->getEndPoint().distanceTo(pElm->getStartPoint()) );
}

void OdDgLine3dDumperPE::dumpData( OdRxObjectPtr pObj, OdExDgnDumper* pDumper ) const
{
  OdDgGraphicsElementDumperPE::dumpData(pObj, pDumper);

  OdDgLine3dPtr pElm = pObj;
  pDumper->writeFieldValue( "Vertex 1", pElm->getStartPoint( ) );
  pDumper->writeFieldValue( "Vertex 2", pElm->getEndPoint( ) );
  pDumper->writeFieldValue( "Length", pElm->getEndPoint().distanceTo(pElm->getStartPoint()) );
}


void OdDgLineString2dDumperPE::dumpData( OdRxObjectPtr pObj, OdExDgnDumper* pDumper ) const
{
  OdDgGraphicsElementDumperPE::dumpData(pObj, pDumper);

  OdDgLineString2dPtr element = pObj;
  OdGePoint2d         point;
  int                 j = element->getVerticesCount();

  pDumper->writeFieldValue( "Number of vertices", j );
  OdString fieldName;
  for(int i = 0; i < j; i++ )
  {
    fieldName.format( L"Vertex %d", i );
    point = element->getVertexAt( i );
    pDumper->writeFieldValue( fieldName, point );
  }
}



void OdDgLineString3dDumperPE::dumpData( OdRxObjectPtr pObj, OdExDgnDumper* pDumper ) const
{
  OdDgGraphicsElementDumperPE::dumpData(pObj, pDumper);

  OdDgLineString3dPtr element = pObj;
  OdGePoint3d         point;
  int                 j = element->getVerticesCount();

  pDumper->writeFieldValue( "Number of vertices", j );
  OdString fieldName;
  for(int i = 0; i < j; i++ )
  {
    fieldName.format( L"Vertex %d", i );
    point = element->getVertexAt( i );
    pDumper->writeFieldValue( fieldName, point );
  }
}

void OdDgRasterFrameDumperPE::dumpData( OdRxObjectPtr pObj, OdExDgnDumper* pDumper ) const
{
  OdDgGraphicsElementDumperPE::dumpData(pObj, pDumper);

  OdDgRasterFramePtr element = pObj;

  pDumper->writeFieldValue(L"Raster Reference Id", element->getRasterReferenceId() );
  pDumper->writeFieldValue(L"File Name", element->getFileName() );
  pDumper->writeFieldValue(L"Full File Name", element->getFullFileName() );
  pDumper->writeFieldValue(L"Logical Name", element->getLogicalName() );
  pDumper->writeFieldValue(L"Description", element->getDescription() );
  pDumper->writeFieldValue(L"Read Only Flag", element->getReadOnlyFlag() );
  pDumper->writeFieldValue(L"Geo Priority", element->getGeoPriority() );
  pDumper->writeFieldValue(L"Use Working Units GeoTiff Default Flag", element->getWUAsGeotiffDefaultUnitsFlag() );
  pDumper->writeFieldValue(L"Use Unit Definition GeoKey Flag", element->getUseUnitDefinitionGeokeyFlag() );
  pDumper->writeFieldValue(L"Ignore Rotation Flag", element->getIgnoreRotationFlag() );
  pDumper->writeFieldValue(L"Drawing Plane", element->getDrawingPlane() );
  pDumper->writeFieldValue(L"Transparent Flag", element->getTransparentFlag() );
  pDumper->writeFieldValue(L"Clipping Flag", element->getClippingFlag() );
  pDumper->writeFieldValue(L"Printable Flag", element->getPrintableFlag() );
  pDumper->writeFieldValue(L"Invert Flag", element->getInvertFlag() );
  pDumper->writeFieldValue(L"Transformation", element->getTransformation() );
  pDumper->writeFieldValue(L"Page Number", element->getPageNumder() );
  pDumper->writeFieldValue(L"Raster Color Mode", element->getRasterColorMode() );
  pDumper->writeFieldValue(L"Raster Dimensions", element->getRasterDimensions() );
  pDumper->writeFieldValue(L"Scanning Resolution", element->getScanningResolution() );
  pDumper->writeFieldValue(L"Layer", element->getLayer() );
  pDumper->writeFieldValue(L"View Gamma", element->getViewGamma() );
  pDumper->writeFieldValue(L"Plot Gamma", element->getPlotGamma() );

  for( OdUInt32 i = 0; i < 8; i++ )
  {
    OdString strViewIndex;
    strViewIndex.format(L"Display at view %d", i );
    pDumper->writeFieldValue(strViewIndex, element->getDisplayAtViewFlag(i) );
  }

  pDumper->writeFieldValue(L"Tint color", element->getTintColorIndex() );
  pDumper->writeFieldValue(L"Foreground color", element->getForegroundColorIndex() );
  pDumper->writeFieldValue(L"Background color", element->getBackgroundColorIndex() );
  pDumper->writeFieldValue(L"Highlight color", element->getHighlightColorIndex() );
  pDumper->writeFieldValue(L"Contrast", element->getContrast() );
  pDumper->writeFieldValue(L"Brightness", element->getBrightness() );

  switch( element->getRasterColorMode() )
  {
    case OdDgRasterFrame::k2ColorsMode:
    case OdDgRasterFrame::kMonochromeMode:
    {
      pDumper->writeFieldValue(L"FG Transparency", element->getTransparentPercent() );
      pDumper->writeFieldValue(L"BG Transparency", element->getBGTransparentPercent() );
    } break;

    case OdDgRasterFrame::k16ColorsMode:
    case OdDgRasterFrame::k256ColorsMode:
    case OdDgRasterFrame::kGrayscaleMode:
    {
      pDumper->writeFieldValue(L"Transparent color palette index", element->getTransparentColorPaletteIndex() );
      pDumper->writeFieldValue(L"Selected Color Transparency", element->getTransparentPercent() );
      pDumper->writeFieldValue(L"All Colors Transparency", element->getAllColorsTransparentPercent() );
    } break;

    case OdDgRasterFrame::kRGBMode:
    case OdDgRasterFrame::kRGBAMode:
    {
      pDumper->writeFieldValueHex(L"Transparent color", element->getTransparentColor() );
      pDumper->writeFieldValue(L"Selected Color Transparency", element->getTransparentPercent() );
      pDumper->writeFieldValue(L"All Colors Transparency", element->getAllColorsTransparentPercent() );
    } break;
  }

  pDumper->dumpFieldName(L"World File Default Units");
  pDumper->writeFieldValue(L"  Unit Base", element->getWorldFileDefaultUnit().m_base );
  pDumper->writeFieldValue(L"  Unit System", element->getWorldFileDefaultUnit().m_system );
  pDumper->writeFieldValue(L"  Unit Numerator", element->getWorldFileDefaultUnit().m_numerator );
  pDumper->writeFieldValue(L"  Unit Denominator", element->getWorldFileDefaultUnit().m_denominator );
  pDumper->writeFieldValue(L"  Unit Factor", element->getWorldFileDefaultUnitFactor() );

  pDumper->dumpFieldName(L"Geo Tiff Default Units");
  pDumper->writeFieldValue(L"  Unit Base", element->getGeoTiffDefaultUnit().m_base );
  pDumper->writeFieldValue(L"  Unit System", element->getGeoTiffDefaultUnit().m_system );
  pDumper->writeFieldValue(L"  Unit Numerator", element->getGeoTiffDefaultUnit().m_numerator );
  pDumper->writeFieldValue(L"  Unit Denominator", element->getGeoTiffDefaultUnit().m_denominator );
  pDumper->writeFieldValue(L"  Unit Factor", element->getGeoTiffDefaultUnitFactor() );
}



void OdDgText2dDumperPE::dumpData( OdRxObjectPtr pObj, OdExDgnDumper* pDumper ) const
{
  OdDgGraphicsElementDumperPE::dumpData(pObj, pDumper);

  OdDgText2dPtr element = pObj;

//#pragma MARKMESSAGE("TODO: Examples. OdDgText2d interface was modified.");

  OdDgDatabase* pDb = element->database();

  pDumper->writeFieldValue( "Text", element->getText() );
  pDumper->writeFieldValue( "Font ID", element->getFontEntryId() );
  // Gets Font name
  OdDgFontTablePtr pFontTable = element->database()->getFontTable();
  OdDgFontTableRecordPtr pFont = pFontTable->getFont(element->getFontEntryId());
  if (!pFont.isNull())
  {
    pDumper->writeFieldValue( "Font Name", pFont->getName() );
  }

  pDumper->writeFieldValue( "Justification", element->getJustification() );
  // MKU 14/05/09   getLength() & getHeight() are obsolete methods.
  //pDumper->writeFieldValue( "Length", element->getLength() );
  //pDumper->writeFieldValue( "Height", element->getHeight() );
  pDumper->writeFieldValue( "Length multiplier", element->getLengthMultiplier() );
  pDumper->writeFieldValue( "Height multiplier", element->getHeightMultiplier() );
  pDumper->writeFieldValue( "Rotation", element->getRotation() );
  pDumper->writeFieldValue( "Origin", element->getOrigin() );
  pDumper->writeFieldValue( "Position", element->getPosition() );
  pDumper->writeFieldValue( "Length", element->getLength() );

  pDumper->writeFieldValue( "TextStyle bit", element->getTextStyleFlag() );
  pDumper->writeFieldValue( "TextStyle ID", element->getTextStyleEntryId() );
  pDumper->writeFieldValue( "CodePage", element->getCodePage() );
  pDumper->writeFieldValue( "EndOfLine", element->getCrCount() > 0 ? true : false );
  pDumper->writeFieldValue( "StackedFractionAlign", element->getStackedFractionAlign() );
  pDumper->writeFieldValue( "StackedFractionType", element->getStackedFractionType() );
  pDumper->writeFieldValue( "StackedFractionSection", element->getStackedFractionSection() );

  OdDgTextStyleTablePtr pTextStyleTable = element->database()->getTextStyleTable();
  OdDgElementId id = pTextStyleTable->getAt(element->getTextStyleEntryId());
  if (!id.isNull())
  {
    OdDgTextStyleTableRecordPtr pTextStyle = id.safeOpenObject();
    pDumper->writeFieldValue( "TextStyle Name", pTextStyle->getName() );
  }

  OdInt16 nCount = element->getTextEditFieldCount(); 
  pDumper->writeFieldValue( "The number of enter data fields in the text element is ", nCount );

  for (OdInt16 i = 0; i < nCount; i++)
  {
    OdDgTextEditField textEditField = element->getTextEditFieldAt(i);

    pDumper->writeFieldValue( "StartPosition", element->getHeight() );
    pDumper->writeFieldValue( "Length", element->getRotation() );
    pDumper->writeFieldValue( "Justification", element->getOrigin() );
  }
}



void OdDgText3dDumperPE::dumpData( OdRxObjectPtr pObj, OdExDgnDumper* pDumper ) const
{
  OdDgGraphicsElementDumperPE::dumpData(pObj, pDumper);

  OdDgText3dPtr element = pObj;

  //#pragma MARKMESSAGE("TODO: Examples. OdDgText3d interface was modified.");


  pDumper->writeFieldValue( "Text", element->getText() );
  pDumper->writeFieldValue( "Font ID", element->getFontEntryId() );
  // Gets Font name
  OdDgFontTablePtr pFontTable = element->database()->getFontTable();
  OdDgFontTableRecordPtr pFont = pFontTable->getFont(element->getFontEntryId());
  if (!pFont.isNull())
  {
    pDumper->writeFieldValue( "Font Name", pFont->getName() );
  }


  pDumper->writeFieldValue( "Justification", element->getJustification() );
  // MKU 14/05/09   getLength() & getHeight() are obsolete methods.
  //pDumper->writeFieldValue( "Length", element->getLength() );
  //pDumper->writeFieldValue( "Height", element->getHeight() );
  pDumper->writeFieldValue( "Length multiplier", element->getLengthMultiplier() );
  pDumper->writeFieldValue( "Height multiplier", element->getHeightMultiplier() );
  pDumper->writeFieldValue( "Rotation", element->getRotation() );
  pDumper->writeFieldValue( "Origin", element->getOrigin() );
  pDumper->writeFieldValue( "Position", element->getPosition() );
  pDumper->writeFieldValue( "CodePage", element->getCodePage() );
  pDumper->writeFieldValue( "EndOfLine", element->getCrCount() > 0 ? true : false );
  pDumper->writeFieldValue( "StackedFractionAlign", element->getStackedFractionAlign() );
  pDumper->writeFieldValue( "StackedFractionType", element->getStackedFractionType() );
  pDumper->writeFieldValue( "StackedFractionSection", element->getStackedFractionSection() );
  pDumper->writeFieldValue( "Backwards Flag", element->getBackwardsFlag() );
  pDumper->writeFieldValue( "Unsidedown Flag", element->getUpsidedownFlag() );
  /*
  {
  TextAttributes attributes;

  element->getTextAttributes( attributes );
  pDumper->writeFieldValue( "Text attributes", attributes );
  }
  */

  /*
  //EDFields
  {
  int         i, j = element->EDField_getNumber();
  char        fieldName[ 20 ];
  TextEDField value;


  fprintf( DumpStream, "Number of EDFields: %d\n", j );

  for( i = 0; i < j; i++ )
  {
  sprintf( fieldName, "EDField %d", i );
  element->EDField_getByIndex( i, value );
  pDumper->writeFieldValue( fieldName, value );
  }
  }
  */




}


void OdDgTextNode2dDumperPE::dumpData( OdRxObjectPtr pObj, OdExDgnDumper* pDumper ) const
{
  OdDgGraphicsElementDumperPE::dumpData(pObj, pDumper);

  OdDgTextNode2dPtr element = pObj;

  //#pragma MARKMESSAGE("TODO: Examples. OdDgTextNode2d interface was modified.");


  pDumper->writeFieldValue( "Line spacing", element->getLineSpacing() );
  pDumper->writeFieldValue( "Font ID", element->getFontEntryId() );
  pDumper->writeFieldValue( "Max length", element->getMaxLength() );
  pDumper->writeFieldValue( "Justification", element->getJustification() );
  pDumper->writeFieldValue( "Length Multiplier", element->getLengthMultiplier() );
  pDumper->writeFieldValue( "Height Multiplier", element->getHeightMultiplier() );
  pDumper->writeFieldValue( "World Wrap Length", element->getTextnodeWordWrapLength() );
  //writeFieldValue( "Length", element->getLength() );
  //writeFieldValue( "Height", element->getHeight() );
  /*
  {
  TextAttributes attributes;

  element->getTextAttributes( attributes );
  pDumper->writeFieldValue( "Text attributes", attributes );
  }
  */
  pDumper->writeFieldValue( "Rotation", element->getRotation() );
  pDumper->writeFieldValue( "Origin", element->getOrigin() );
}



void OdDgTextNode3dDumperPE::dumpData( OdRxObjectPtr pObj, OdExDgnDumper* pDumper ) const
{
  OdDgGraphicsElementDumperPE::dumpData(pObj, pDumper);

  OdDgTextNode3dPtr element = pObj;

  //#pragma MARKMESSAGE("TODO: Examples. OdDgTextNode3d interface was modified.");


  pDumper->writeFieldValue( "Line spacing", element->getLineSpacing() );
  pDumper->writeFieldValue( "Font ID", element->getFontEntryId() );
  pDumper->writeFieldValue( "Max length", element->getMaxLength() );
  pDumper->writeFieldValue( "Justification", element->getJustification() );
  pDumper->writeFieldValue( "Length Multiplier", element->getLengthMultiplier() );
  pDumper->writeFieldValue( "Height Multiplier", element->getHeightMultiplier() );
  //writeFieldValue( "Char width", element->getCharWidth() );
  //writeFieldValue( "Char height", element->getCharHeight() );
  /*
  {
  TextAttributes attributes;

  element->getTextAttributes( attributes );
  pDumper->writeFieldValue( "Text attributes", attributes );
  }
  */
  pDumper->writeFieldValue( "Rotation", element->getRotation() );
  pDumper->writeFieldValue( "Origin", element->getOrigin() );
}

void OdDgTitleText2dDumperPE::dumpData( OdRxObjectPtr pObj, OdExDgnDumper* pDumper ) const
{
  OdDgGraphicsElementDumperPE::dumpData(pObj, pDumper);

  //OdDgTitleText2dPtr element = pObj;
}

void OdDgTitleText3dDumperPE::dumpData( OdRxObjectPtr pObj, OdExDgnDumper* pDumper ) const
{
  OdDgGraphicsElementDumperPE::dumpData(pObj, pDumper);

  //OdDgTitleText3dPtr element = pObj;
}

void OdDgSectionMarker2dDumperPE::dumpData( OdRxObjectPtr pObj, OdExDgnDumper* pDumper ) const
{
  OdDgGraphicsElementDumperPE::dumpData(pObj, pDumper);

  //OdDgSectionMarker2dPtr element = pObj;
}

void OdDgSectionMarker3dDumperPE::dumpData( OdRxObjectPtr pObj, OdExDgnDumper* pDumper ) const
{
  OdDgGraphicsElementDumperPE::dumpData(pObj, pDumper);

  //OdDgSectionMarker3dPtr element = pObj;
}

void OdDgArrowMarker2dDumperPE::dumpData( OdRxObjectPtr pObj, OdExDgnDumper* pDumper ) const
{
  OdDgGraphicsElementDumperPE::dumpData(pObj, pDumper);

  //OdDgArrowMarker2dPtr element = pObj;
}

void OdDgArrowMarker3dDumperPE::dumpData( OdRxObjectPtr pObj, OdExDgnDumper* pDumper ) const
{
  OdDgGraphicsElementDumperPE::dumpData(pObj, pDumper);

  //OdDgArrowMarker3dPtr element = pObj;
}

void OdDgDetailMarker2dDumperPE::dumpData( OdRxObjectPtr pObj, OdExDgnDumper* pDumper ) const
{
  OdDgGraphicsElementDumperPE::dumpData(pObj, pDumper);

  //OdDgDetailMarker2dPtr element = pObj;
}

void OdDgDetailMarker3dDumperPE::dumpData( OdRxObjectPtr pObj, OdExDgnDumper* pDumper ) const
{
  OdDgGraphicsElementDumperPE::dumpData(pObj, pDumper);

  //OdDgDetailMarker3dPtr element = pObj;
}

void OdDgSectionClipDumperPE::dumpData( OdRxObjectPtr pObj, OdExDgnDumper* pDumper ) const
{
  OdDgGraphicsElementDumperPE::dumpData(pObj, pDumper);

  OdDgSectionClipPtr pSectionClip = pObj;

  pDumper->writeFieldValue(L"Clip Name", pSectionClip->getName() );
  pDumper->writeFieldValue(L"Perspective Up Flag", pSectionClip->getPerspectiveUpFlag() );
  pDumper->writeFieldValue(L"Left Crop Flag", pSectionClip->getLeftCropFlag() );
  pDumper->writeFieldValue(L"Right Crop Flag", pSectionClip->getRightCropFlag() );
  pDumper->writeFieldValue(L"Top Crop Flag", pSectionClip->getTopCropFlag() );
  pDumper->writeFieldValue(L"Bottom Crop Flag", pSectionClip->getBottomCropFlag() );
  pDumper->writeFieldValue(L"Front Crop Flag", pSectionClip->getFrontCropFlag() );
  pDumper->writeFieldValue(L"Back Crop Flag", pSectionClip->getBackCropFlag() );
  pDumper->writeFieldValue(L"Front Depth", pSectionClip->getFrontDepth() );
  pDumper->writeFieldValue(L"Back Depth", pSectionClip->getBackDepth() );
  pDumper->writeFieldValue(L"Top Height", pSectionClip->getTopHeight() );
  pDumper->writeFieldValue(L"Bottom Height", pSectionClip->getBottomHeight() );
  pDumper->writeFieldValue(L"Clip Width", pSectionClip->getClipWidth() );
  pDumper->writeFieldValue(L"Origin", pSectionClip->getOrigin() );
  pDumper->writeFieldValue(L"Rotation", pSectionClip->getRotation() );
}


void OdDgColorBookDumperPE::dumpData( OdRxObjectPtr pObj, OdExDgnDumper* pDumper ) const
{
  OdDgElementDumperPE::dumpData(pObj, pDumper);

  OdDgColorBookPtr element = pObj;

  if( !element.isNull() )
  {
    for( OdUInt32 l = 0; l < element->getItemCount(); l++ )
    {
      OdString strName = L"  " + element->getColorBookItem(l).m_strColorName;

      pDumper->writeFieldValueHex( strName, element->getColorBookItem(l).m_clrColor);
    }
  }
}

void OdDgLinkSetDumperPE::dumpData( OdRxObjectPtr pObj, OdExDgnDumper* pDumper ) const
{
  OdDgElementDumperPE::dumpData(pObj, pDumper);

  OdDgLinkSetPtr pLinkSet = pObj;

  pDumper->dumpFieldName(L"Items:" );

  for( OdUInt32 i = 0; i < pLinkSet->getLinkNodeCount(); i++ )
  {
    pDumper->writeFieldValue(L"+ ", pLinkSet->getLinkNode(i) );
  }
}

void OdDgElementTemplateDumperPE::dumpData( OdRxObjectPtr pObj, OdExDgnDumper* pDumper ) const
{
  OdDgElementDumperPE::dumpData(pObj, pDumper);

  OdDgElementTemplatePtr element = pObj;

  pDumper->writeFieldValue( L"  Template Name", element->getTemplateName() );
  pDumper->writeFieldValue( L"  Order In Group", element->getTemplateOrder() );
  pDumper->writeFieldValueHex( L"  Parent Id", element->getParentId() );
  pDumper->writeFieldValue( L"  Template Group Flag", element->isTemplateGroup() );

  for( OdUInt32 i = 0; i < element->getItemCount(); i++ )
  {
    OdDgTemplateItem tmpItem = element->getTemplateItem(i);

    OdString strPathName;

    strPathName.format(L"  Template item %d :", i );

    pDumper->dumpFieldName( strPathName );

    strPathName.empty();

    for( OdUInt32 k = 0; k < tmpItem.getPathLength(); k++ )
    {
      strPathName += tmpItem.getPathItem(k);

      if( k != tmpItem.getPathLength() - 1 )
      {
        strPathName += L"->";
      }
    }

    pDumper->writeFieldValue( L"     Path:", strPathName );

    for( OdUInt32 n = 0; n < tmpItem.getValueCount(); n++ )
    {
      OdString strValueName;

      strValueName.format(L"     Value %d",n + 1);

      OdDgTemplateValueVariant value = tmpItem.getValue(n);

      pDumper->writeFieldValue(strValueName, value );
    }
  }
}

void OdDgShape2dDumperPE::dumpData( OdRxObjectPtr pObj, OdExDgnDumper* pDumper ) const
{
  OdDgGraphicsElementDumperPE::dumpData(pObj, pDumper);

  OdDgShape2dPtr element = pObj;
  OdGePoint2d    point;
  int            j = element->getVerticesCount();

  pDumper->writeFieldValue( "Number of vertices", j );
  OdString fieldName;
  for(int i = 0; i < j; i++ )
  {
    fieldName.format( L"Vertex %d", i );
    point = element->getVertexAt( i );
    pDumper->writeFieldValue( fieldName, point );
  }
}


void OdDgShape3dDumperPE::dumpData( OdRxObjectPtr pObj, OdExDgnDumper* pDumper ) const
{
  OdDgGraphicsElementDumperPE::dumpData(pObj, pDumper);

  OdDgShape3dPtr element = pObj;
  OdGePoint3d    point;
  int            j = element->getVerticesCount();

  pDumper->writeFieldValue( "Number of vertices", j );
  OdString fieldName;
  for(int i = 0; i < j; i++ )
  {
    fieldName.format( L"Vertex %d", i );
    point = element->getVertexAt( i );
    pDumper->writeFieldValue( fieldName, point );
  }
}


void OdDgCurve2dDumperPE::dumpData( OdRxObjectPtr pObj, OdExDgnDumper* pDumper ) const
{
  OdDgGraphicsElementDumperPE::dumpData(pObj, pDumper);

  OdDgCurve2dPtr element = pObj;
  OdGePoint2d    point;
  int            j = element->getVerticesCount();

  pDumper->writeFieldValue( "Number of vertices", j );
  OdString fieldName;
  for(int i = 0; i < j; i++ )
  {
    fieldName.format( L"Vertex %d", i );
    point = element->getVertexAt( i );
    pDumper->writeFieldValue( fieldName, point );
  }
}



void OdDgCurve3dDumperPE::dumpData( OdRxObjectPtr pObj, OdExDgnDumper* pDumper ) const
{
  OdDgGraphicsElementDumperPE::dumpData(pObj, pDumper);

  OdDgCurve3dPtr element = pObj;
  OdGePoint3d    point;
  int            j = element->getVerticesCount();

  pDumper->writeFieldValue( "Number of vertices", j );
  OdString fieldName;
  for(int i = 0; i < j; i++ )
  {
    fieldName.format( L"Vertex %d", i );
    point = element->getVertexAt( i );
    pDumper->writeFieldValue( fieldName, point );
  }
}


void OdDgEllipse2dDumperPE::dumpData( OdRxObjectPtr pObj, OdExDgnDumper* pDumper ) const
{
  OdDgGraphicsElementDumperPE::dumpData(pObj, pDumper);

  OdDgEllipse2dPtr element = pObj;



  pDumper->writeFieldValue( "Primary axis", element->getPrimaryAxis() );
  pDumper->writeFieldValue( "Secondary axis", element->getSecondaryAxis() );
  pDumper->writeFieldValue( "Rotation", element->getRotationAngle() );
  {
    OdGePoint2d point;

    point = element->getOrigin();
    pDumper->writeFieldValue( "Origin", point );
  }




}


void OdDgEllipse3dDumperPE::dumpData( OdRxObjectPtr pObj, OdExDgnDumper* pDumper ) const
{
  OdDgGraphicsElementDumperPE::dumpData(pObj, pDumper);

  OdDgEllipse3dPtr element = pObj;



  pDumper->writeFieldValue( "Primary axis", element->getPrimaryAxis() );
  pDumper->writeFieldValue( "Secondary axis", element->getSecondaryAxis() );
  pDumper->writeFieldValue( "Rotation", element->getRotation() );
  pDumper->writeFieldValue( "Origin", element->origin() );




}



void OdDgArc2dDumperPE::dumpData( OdRxObjectPtr pObj, OdExDgnDumper* pDumper ) const
{
  OdDgGraphicsElementDumperPE::dumpData(pObj, pDumper);

  OdDgArc2dPtr element = pObj;

  pDumper->writeFieldValue( "Primary axis", element->getPrimaryAxis() );
  pDumper->writeFieldValue( "Secondary axis", element->getSecondaryAxis() );
  pDumper->writeFieldValue( "Rotation", element->getRotationAngle() );
  {
    OdGePoint2d point;

    point = element->getOrigin();
    pDumper->writeFieldValue( "Origin", point );
  }
  pDumper->writeFieldValue( "Start angle", element->getStartAngle() );
  pDumper->writeFieldValue( "Sweep angle", element->getSweepAngle() );
  double dArea = 0;
  element->getArea(dArea);
  pDumper->writeFieldValue( "Area", dArea);
}



void OdDgArc3dDumperPE::dumpData( OdRxObjectPtr pObj, OdExDgnDumper* pDumper ) const
{
  OdDgGraphicsElementDumperPE::dumpData(pObj, pDumper);

  OdDgArc3dPtr element = pObj;

  pDumper->writeFieldValue( "Primary axis", element->getPrimaryAxis() );
  pDumper->writeFieldValue( "Secondary axis", element->getSecondaryAxis() );
  pDumper->writeFieldValue( "Rotation", element->getRotation() );
  pDumper->writeFieldValue( "Origin", element->getOrigin() );
  pDumper->writeFieldValue( "Start angle", element->getStartAngle() );
  pDumper->writeFieldValue( "Sweep angle", element->getSweepAngle() );
  double dArea = 0;
  element->getArea(dArea);
  pDumper->writeFieldValue( "Area", dArea);
}


void OdDgConeDumperPE::dumpData( OdRxObjectPtr pObj, OdExDgnDumper* pDumper ) const
{
  OdDgGraphicsElementDumperPE::dumpData(pObj, pDumper);

  OdDgConePtr element = pObj;
  {
    OdGePoint3d point = element->getCenter1();
    pDumper->writeFieldValue( "Center 1", point );
    point = element->getCenter2();
    pDumper->writeFieldValue( "Center 2", point );
  }
  pDumper->writeFieldValue( "Radius 1", element->getRadius1() );
  pDumper->writeFieldValue( "Radius 2", element->getRadius2() );
  pDumper->writeFieldValue( "Rotation", element->getRotation() );
  pDumper->writeFieldValue( "Is solid", element->isSolid() );
  pDumper->writeFieldValue( "Hole", element->getHbitFlag() );
}

void OdDgComplexStringDumperPE::dumpData( OdRxObjectPtr pObj, OdExDgnDumper* pDumper ) const
{
  OdDgGraphicsElementDumperPE::dumpData(pObj, pDumper);

  OdDgComplexStringPtr element = pObj;
  double dArea = 0;
  if( element->getArea(dArea) == eOk )
  {
    pDumper->writeFieldValue( "Area", dArea);
  }
  else
  {
    pDumper->writeFieldValue( "Area", OdString(L"Can't be calculated yet."));
  }
}


void OdDgComplexShapeDumperPE::dumpData( OdRxObjectPtr pObj, OdExDgnDumper* pDumper ) const
{
  OdDgGraphicsElementDumperPE::dumpData(pObj, pDumper);

  OdDgComplexShapePtr element = pObj;
  double dArea = 0;
  if( element->getArea(dArea) == eOk )
  {
    pDumper->writeFieldValue( "Area", dArea);
  }
  else
  {
    pDumper->writeFieldValue( "Area", OdString(L"Can't be calculated yet."));
  }
}



void OdDgPointString2dDumperPE::dumpData( OdRxObjectPtr pObj, OdExDgnDumper* pDumper ) const
{
  OdDgGraphicsElementDumperPE::dumpData(pObj, pDumper);

  OdDgPointString2dPtr element = pObj;
  OdGePoint2d         point;
  int                 j = element->getVerticesCount();

  pDumper->writeFieldValue( "Number of vertices", j );
  pDumper->writeFieldValue( "Continuous", element->getContinuousFlag() );
  OdString fieldName;
  for(int i = 0; i < j; i++ )
  {
    fieldName.format( L"Vertex %d", i );
    pDumper->writeFieldValue( fieldName, element->getVertexAt( i ) );

    fieldName.format( L"Rotation %d", i );
    pDumper->writeFieldValue( fieldName, element->getRotationAt( i ) );
  }
}


void OdDgPointString3dDumperPE::dumpData( OdRxObjectPtr pObj, OdExDgnDumper* pDumper ) const
{
  OdDgGraphicsElementDumperPE::dumpData(pObj, pDumper);

  OdDgPointString3dPtr element = pObj;
  OdGePoint3d         point;
  int                 j = element->getVerticesCount();

  pDumper->writeFieldValue( "Number of vertices", j );
  pDumper->writeFieldValue( "Continuous", element->getContinuousFlag() );
  OdString fieldName;
  for(int i = 0; i < j; i++ )
  {
    fieldName.format( L"Vertex %d", i );
    pDumper->writeFieldValue( fieldName, element->getVertexAt( i ) );

    fieldName.format( L"Rotation %d", i );
    pDumper->writeFieldValue( fieldName, element->getRotationAt( i ) );
  }
}

void OdDgNamedGroupDumperPE::dumpData( OdRxObjectPtr pObj, OdExDgnDumper* pDumper ) const
{
   OdDgElementDumperPE::dumpData(pObj, pDumper);

   OdDgNamedGroupHeaderPtr pGroup = pObj;

   pDumper->writeFieldValue("Group Name", pGroup->getName() );
   pDumper->writeFieldValue("Group Description", pGroup->getDescription() );
   pDumper->writeFieldValue("DefaultForwardPropagate", pGroup->getDefaultForwardPropagate() );
   pDumper->writeFieldValue("DefaultBackwardPropagate", pGroup->getDefaultBackwardPropagate() );
   pDumper->writeFieldValue("DefaultGroupPropagate", pGroup->getDefaultGroupPropagate() );
   pDumper->writeFieldValue("AllowDuplicatesFlag", pGroup->getAllowDuplicatesFlag() );
   pDumper->writeFieldValue("ExclusiveMembersFlag", pGroup->getExclusiveMembersFlag() );
   pDumper->writeFieldValue("AllowFarReferencesFlag", pGroup->getAllowFarReferencesFlag() );
   pDumper->writeFieldValue("ClosedGroupFlag", pGroup->getClosedGroupFlag() );
   pDumper->writeFieldValue("SelectMembersFlag", pGroup->getSelectMembersFlag() );
   pDumper->writeFieldValue("AnonimusFlag", pGroup->getAnonimusFlag() );
   pDumper->writeFieldValue("HasOtherGroupsFlag", pGroup->getHasOtherGroupFlag() );
   pDumper->writeFieldValue("HasAlwaysPropagateMembersFlag", pGroup->getHasAlwaysPropagateMembersFlag() );
   pDumper->dumpFieldName("Members:");

   for( OdUInt32 i = 0; i < pGroup->getNamedGroupItemCount(); i++ )
   {
     OdDgNamedGroupItem curItem = pGroup->getNamedGroupItem(i);

     OdString strItemName;
     strItemName.format(L"  Item %d", i);

     pDumper->dumpFieldName(strItemName);
     pDumper->writeFieldValue(L"   ForwardPropagate", curItem.getForwardPropagate() );
     pDumper->writeFieldValue(L"   BackwardPropagate", curItem.getBackwardPropagate() );
     pDumper->writeFieldValue(L"   GroupPropagate", curItem.getGroupPropagate() );

     OdArray<OdUInt64> arrPath;

     curItem.getPath(arrPath);

     OdString strPathValue;

     for( OdUInt32 j = 0; j < arrPath.size(); j++ )
     {
       OdString strPathItem;

       if( j == 0 )
       {
         strPathItem.format(L"%X",arrPath[ arrPath.size() - j - 1 ]);
       }
       else
       {
         strPathItem.format(L" -> %X",arrPath[ arrPath.size() - j - 1 ]);
       }

       strPathValue += strPathItem;
     }

     pDumper->writeFieldValue(L"    Path", strPathValue );
   }
}

void OdDgDimensionDumperPE::dumpData( OdRxObjectPtr pObj, OdExDgnDumper* pDumper ) const
{
  OdDgGraphicsElementDumperPE::dumpData(pObj, pDumper);

  OdDgDimensionPtr element = pObj;

  //simple fields
  pDumper->writeFieldValue( "Tool type", element->getDimensionType() );
  pDumper->writeFieldValue( "Scale", element->getScaleFactor() );
  pDumper->writeFieldValue( "AnnotationScale", element->getAnnotationScale() );
  pDumper->writeFieldValueHex( "Flags", element->getFlags() );

//#pragma MARKMESSAGE( "Can not obtain flag 'Is3d' for OdDgDimension pObj. /mushakov@softdev.spb.ru/" );
  //{
  //  OdGeMatrix2d    matrix;
  //  OdGeQuaternion  quaternion;

  //  element->getRotation( matrix );
  //  element->getRotation( quaternion );

  //  pDumper->writeFieldValue( "2D rotation", matrix );
  //  pDumper->writeFieldValue( "3D rotation", quaternion );
  //}

  {

    switch(element->getAlignment())
    {
    case OdDgDimension::kPaArbitrary:
      pDumper->writeFieldValue( "Placement Alignment", "kPaArbitrary\n" );
      break;
    case OdDgDimension::kPaDrawing:
      pDumper->writeFieldValue( "Placement Alignment", "kPaDrawing\n" );
      break;
    case OdDgDimension::kPaTrue:
      pDumper->writeFieldValue( "Placement Alignment", "kPaTrue\n" );
      break;
    case OdDgDimension::kPaView:
      pDumper->writeFieldValue( "Placement Alignment", "kPaView\n" );
  
      pDumper->writeFieldValue( "Alignment View", element->getAlignmentView() );
      break;
    };

    switch(element->getArrowHead())
    {
    case OdDgDimension::kOpened:
      pDumper->writeFieldValue( "Arrow Head Type", "kOpened\n" );
      break;
    case OdDgDimension::kClosed:
      pDumper->writeFieldValue( "Arrow Head Type", "kClosed\n" );
      break;
    case OdDgDimension::kFilled:
      pDumper->writeFieldValue( "Arrow Head Type", "kFilled\n" );
      break;
    };

    if(element->Is3D())
    {
      OdGeQuaternion q;
      element->getRotation(q);
      pDumper->writeFieldValue( "Rotation", q );
    }
    else
    {
      OdGeMatrix2d q;
      element->getRotation(q);
      pDumper->writeFieldValue( "Rotation", q );
    }


#undef DUMPDIMFLAG
#define DUMPDIMFLAG(flg) \
    pDumper->writeFieldValue( OdString(L"Flag ") + #flg, element->get##flg##Flag() );

    DUMPDIMFLAG(BoxText)
    DUMPDIMFLAG(CapsuleText)
    DUMPDIMFLAG(Centesimal)
    DUMPDIMFLAG(CrossCenter)
    DUMPDIMFLAG(Dual)
    DUMPDIMFLAG(Embed)
    DUMPDIMFLAG(Horizontal)
    DUMPDIMFLAG(Joiner)
    DUMPDIMFLAG(LeadingZero2)
    DUMPDIMFLAG(LeadingZeros)
    DUMPDIMFLAG(NoAutoTextLift)
    DUMPDIMFLAG(NoLevelSymb)
    DUMPDIMFLAG(RelDimLine)
    DUMPDIMFLAG(RelStat)
    DUMPDIMFLAG(StackFract)
    DUMPDIMFLAG(TextHeapPad)
    DUMPDIMFLAG(ThousandComma)
    DUMPDIMFLAG(Tolerance)
    DUMPDIMFLAG(Tolmode)
    DUMPDIMFLAG(TrailingZeros)
    DUMPDIMFLAG(TrailingZeros2)
    DUMPDIMFLAG(UnderlineText)
  }
  //
  //composite fields
  //

  //points
  {
    OdUInt32      j = element->getPointsCount();
    OdDgDimPoint  point;
    OdString      fieldName;

    pDumper->writeFieldValue( "Number of points", j );
    for(unsigned i = 0; i < j; i++ )
    {
      fieldName.format(L"Point %d", i );
      point = element->getPoint( i );
      pDumper->writeFieldValue( fieldName, point );
    }
  }

  //text info
  {
    OdDgDimTextInfo textInfo;

    element->getDimTextInfo( textInfo );
    pDumper->writeFieldValue( "Text info", textInfo );
  }

  //text format
  {
    OdDgDimTextFormat textFormat;

    element->getDimTextFormat( textFormat );
    pDumper->writeFieldValue( "Text format", textFormat );
  }

  //geometry
  {
    OdDgDimGeometry geometry;

    element->getDimGeometry( geometry );
    pDumper->writeFieldValue( "Geometry", geometry );
  }

  // Symbology
  {
    OdInt32 altLineStyle = element->getAltLineStyleEntryId();
    OdUInt32 altLineWeight = element->getAltLineWeight();
    OdUInt32 altColorIndex = element->getAltColorIndex();

    pDumper->writeFieldValue( "Alternative LineStyle", altLineStyle );
    pDumper->writeFieldValue( "Alternative LineWeight", altLineWeight );
    pDumper->writeFieldValue( "Alternative ColorIndex", altColorIndex );
  }

  // tools

  pDumper->writeFieldValue("Tools:", element );

  //options
  {
    OdUInt32 iOptionsCount = 0;

    #undef  INC_DIM_OPTIONS_COUNT
    #define INC_DIM_OPTIONS_COUNT(name)                      \
    if( !element->getOption(OdDgDimOption::k##name).isNull() ) \
         iOptionsCount++;

    INC_DIM_OPTIONS_COUNT(Tolerance)
    INC_DIM_OPTIONS_COUNT(Terminators)
    INC_DIM_OPTIONS_COUNT(PrefixSymbol)
    INC_DIM_OPTIONS_COUNT(SuffixSymbol)
    INC_DIM_OPTIONS_COUNT(DiameterSymbol)
    INC_DIM_OPTIONS_COUNT(PrefixSuffix)
    INC_DIM_OPTIONS_COUNT(PrimaryUnits)
    INC_DIM_OPTIONS_COUNT(SecondaryUnits)
    INC_DIM_OPTIONS_COUNT(TerminatorSymbology)
    INC_DIM_OPTIONS_COUNT(View)
    INC_DIM_OPTIONS_COUNT(AlternatePrimaryUnits)
    INC_DIM_OPTIONS_COUNT(Offset)
    INC_DIM_OPTIONS_COUNT(AlternateSecondaryUnits)
    INC_DIM_OPTIONS_COUNT(AlternatePrefixSymbol)
    INC_DIM_OPTIONS_COUNT(AlternateSuffixSymbol)
    INC_DIM_OPTIONS_COUNT(ProxyCell)

    pDumper->writeFieldValue( "Number of options", iOptionsCount );

    OdDgDimOptionPtr pDimOptions;
    iOptionsCount = 0;

#undef  DUMP_DIM_OPTIONS
#define DUMP_DIM_OPTIONS(name)                                 \
    pDimOptions = element->getOption(OdDgDimOption::k##name);  \
    if( !pDimOptions.isNull() )                                \
    {                                                          \
      OdString strOptionNum;                                   \
      strOptionNum.format(L"Dimension option %d", iOptionsCount ); \
      pDumper->writeFieldValue( strOptionNum, pDimOptions ); \
      iOptionsCount++; \
    }

    DUMP_DIM_OPTIONS(Tolerance)
    DUMP_DIM_OPTIONS(Terminators)
    DUMP_DIM_OPTIONS(PrefixSymbol)
    DUMP_DIM_OPTIONS(SuffixSymbol)
    DUMP_DIM_OPTIONS(DiameterSymbol)
    DUMP_DIM_OPTIONS(PrefixSuffix)
    DUMP_DIM_OPTIONS(PrimaryUnits)
    DUMP_DIM_OPTIONS(SecondaryUnits)
    DUMP_DIM_OPTIONS(TerminatorSymbology)
    DUMP_DIM_OPTIONS(View)
    DUMP_DIM_OPTIONS(AlternatePrimaryUnits)
    DUMP_DIM_OPTIONS(Offset)
    DUMP_DIM_OPTIONS(AlternateSecondaryUnits)
    DUMP_DIM_OPTIONS(AlternatePrefixSymbol)
    DUMP_DIM_OPTIONS(AlternateSuffixSymbol)
    DUMP_DIM_OPTIONS(ProxyCell)
  }
}

void OdDgMultilineDumperPE::dumpData( OdRxObjectPtr pObj, OdExDgnDumper* pDumper ) const
{
  OdDgGraphicsElementDumperPE::dumpData(pObj, pDumper);
  OdDgMultilinePtr element = pObj;

  //simple fields
  pDumper->writeFieldValue( "Origin cap angle", element->getOriginCapAngle() );
  pDumper->writeFieldValue( "End cap angle", element->getEndCapAngle() );
  {
    OdGeVector3d vector;

    vector = element->getZVector();
    pDumper->writeFieldValue( "Z vector", vector );
  }
  pDumper->writeFieldValue( "Is closed", element->getClosedFlag() );

  //symbologies
  {
    OdDgMultilineSymbology symbology;

    element->getOriginCap( symbology );
    pDumper->writeFieldValue( "Origin cap", symbology );
    element->getMiddleCap( symbology );
    pDumper->writeFieldValue( "Joint cap", symbology );
    element->getEndCap( symbology );
    pDumper->writeFieldValue( "End cap", symbology );
  }

  //points
  {
    OdUInt32 j = element->getPointsCount();
    OdDgMultilinePoint point;
    OdString      fieldName;

    pDumper->writeFieldValue( "Number of points", j );
    for(unsigned i = 0; i < j; i++ )
    {
      fieldName.format(L"Point %d", i );
      element->getPoint( i, point );
      pDumper->writeFieldValue( fieldName, point );
    }
  }

  //profiles
  {
    OdUInt32              j = element->getProfilesCount();
    OdDgMultilineProfile  profile;
    OdString      fieldName;

    pDumper->writeFieldValue( "Number of profiles", j );
    for(unsigned i = 0; i < j; i++ )
    {
      fieldName.format(L"Profile %d", i );
      element->getProfile( i, profile );
      pDumper->writeFieldValue( fieldName, profile );
    }
  }
}


void OdDgBSplineCurve2dDumperPE::dumpData( OdRxObjectPtr pObj, OdExDgnDumper* pDumper ) const
{
  OdDgGraphicsElementDumperPE::dumpData(pObj, pDumper);

  OdDgBSplineCurve2dPtr element = pObj;

  //simple fields
  //simple fields
  pDumper->writeFieldValue( "Order", element->getOrder() );
  pDumper->writeFieldValue( "Display curve", element->getCurveDisplayFlag() );
  pDumper->writeFieldValue( "Display polygon", element->getPolygonDisplayFlag() );
  pDumper->writeFieldValue( "Closed", element->getClosedFlag() );

  if( element->hasFitData() )
  {
    pDumper->writeFieldValue( "Construction type", OdString(L"kDefinedByFitPoints" ));
    pDumper->writeFieldValue( "Natural tangents", element->getNaturalTangentsFlag() );
    pDumper->writeFieldValue( "Chord length tangents", element->getChordLenTangentsFlag() );
    pDumper->writeFieldValue( "Collinear tangents", element->getColinearTangentsFlag() );

    OdGePoint2dArray fitPoints;
    OdUInt32 uOrder;
    bool bTangentExists;
    OdGeVector2d vrStartTangent;
    OdGeVector2d vrEndTangent;
    bool bUniformKnots;
    element->getFitData( fitPoints, uOrder, bTangentExists, vrStartTangent, vrEndTangent, bUniformKnots );

    OdGeKnotVector vrKnots = element->getKnots();

    pDumper->writeFieldValue(L"Num Fit Points", (OdUInt32)fitPoints.size() );

    for( OdUInt32 i = 0; i < fitPoints.size(); i++ )
    {
      OdString strFitPtsName;
      strFitPtsName.format(L"  Point %d", i);
      pDumper->writeFieldValue( strFitPtsName, fitPoints[i] );
    }

    pDumper->writeFieldValue(L"Start Tangent", vrStartTangent );
    pDumper->writeFieldValue(L"End Tangent", vrEndTangent );
    pDumper->writeFieldValue(L"Uniform Knots Flag", bUniformKnots );

    pDumper->writeFieldValue(L"Num Knots", (OdUInt32)vrKnots.length() );

    for( OdInt32 j = 0; j < vrKnots.length(); j++ )
    {
      OdString strKnotName;
      strKnotName.format(L"  Knot %d", j);
      pDumper->writeFieldValue( strKnotName, vrKnots[j] );
    }
  }
  else
  {
    pDumper->writeFieldValue( "Construction type", OdString(L"kDefinedByNurbsData" ));
    pDumper->writeFieldValue( "Rational", element->isRational() );

    OdGePoint2dArray arrCtrlPts;
    OdGeKnotVector   vrKnots;
    OdGeDoubleArray  arrWeights;
    OdUInt32 uOrder;
    bool bClosed;
    bool bRational;

    element->getNurbsData( uOrder, bRational, bClosed, arrCtrlPts, vrKnots, arrWeights );

    pDumper->writeFieldValue(L"Num Control Points", (OdUInt32)arrCtrlPts.size() );

    for( OdUInt32 i = 0; i < arrCtrlPts.size(); i++ )
    {
      OdString strCtrlPtsName;
      strCtrlPtsName.format(L"  Point %d", i);
      pDumper->writeFieldValue( strCtrlPtsName, arrCtrlPts[i] );
    }

    pDumper->writeFieldValue(L"Num Knots", (OdUInt32)vrKnots.length() );

    for( OdInt32 j = 0; j < vrKnots.length(); j++ )
    {
      OdString strKnotName;
      strKnotName.format(L"  Knot %d", j);
      pDumper->writeFieldValue( strKnotName, vrKnots[j] );
    }

    if( bRational )
    {
      pDumper->writeFieldValue(L"Num Weights", (OdUInt32)arrWeights.size() );

      for( OdUInt32 k = 0; k < arrWeights.length(); k++ )
      {
        OdString strWeightName;
        strWeightName.format(L"  Weight %d", k);
        pDumper->writeFieldValue( strWeightName, arrWeights[k] );
      }
    }
  }
}

void OdDgBSplineCurve3dDumperPE::dumpData( OdRxObjectPtr pObj, OdExDgnDumper* pDumper ) const
{
  OdDgGraphicsElementDumperPE::dumpData(pObj, pDumper);

  OdDgBSplineCurve3dPtr element = pObj;

  //simple fields
  pDumper->writeFieldValue( "Order", element->getOrder() );
  pDumper->writeFieldValue( "Display curve", element->getCurveDisplayFlag() );
  pDumper->writeFieldValue( "Display polygon", element->getPolygonDisplayFlag() );
  pDumper->writeFieldValue( "Closed", element->getClosedFlag() );

  if( element->hasFitData() )
  {
    pDumper->writeFieldValue( "Construction type", OdString(L"kDefinedByFitPoints" ));
    pDumper->writeFieldValue( "Natural tangents", element->getNaturalTangentsFlag() );
    pDumper->writeFieldValue( "Chord length tangents", element->getChordLenTangentsFlag() );
    pDumper->writeFieldValue( "Collinear tangents", element->getColinearTangentsFlag() );

    OdGePoint3dArray fitPoints;
    OdUInt32 uOrder;
    bool bTangentExists;
    OdGeVector3d vrStartTangent;
    OdGeVector3d vrEndTangent;
    bool bUniformKnots;
    element->getFitData( fitPoints, uOrder, bTangentExists, vrStartTangent, vrEndTangent, bUniformKnots );

    OdGeKnotVector vrKnots = element->getKnots();

    pDumper->writeFieldValue(L"Num Fit Points", (OdUInt32)fitPoints.size() );

    for( OdUInt32 i = 0; i < fitPoints.size(); i++ )
    {
      OdString strFitPtsName;
      strFitPtsName.format(L"  Point %d", i);
      pDumper->writeFieldValue( strFitPtsName, fitPoints[i] );
    }

    pDumper->writeFieldValue(L"Start Tangent", vrStartTangent );
    pDumper->writeFieldValue(L"End Tangent", vrEndTangent );
    pDumper->writeFieldValue(L"Uniform Knots Flag", bUniformKnots );

    pDumper->writeFieldValue(L"Num Knots", (OdUInt32)vrKnots.length() );

    for( OdInt32 j = 0; j < vrKnots.length(); j++ )
    {
      OdString strKnotName;
      strKnotName.format(L"  Knot %d", j);
      pDumper->writeFieldValue( strKnotName, vrKnots[j] );
    }
  }
  else
  {
    pDumper->writeFieldValue( "Construction type", OdString(L"kDefinedByNurbsData" ));
    pDumper->writeFieldValue( "Rational", element->isRational() );

    OdGePoint3dArray arrCtrlPts;
    OdGeKnotVector   vrKnots;
    OdGeDoubleArray  arrWeights;
    OdUInt32 uOrder;
    bool bClosed;
    bool bRational;

    element->getNurbsData( uOrder, bRational, bClosed, arrCtrlPts, vrKnots, arrWeights );

    pDumper->writeFieldValue(L"Num Control Points", (OdUInt32)arrCtrlPts.size() );

    for( OdUInt32 i = 0; i < arrCtrlPts.size(); i++ )
    {
      OdString strCtrlPtsName;
      strCtrlPtsName.format(L"  Point %d", i);
      pDumper->writeFieldValue( strCtrlPtsName, arrCtrlPts[i] );
    }

    pDumper->writeFieldValue(L"Num Knots", (OdUInt32)vrKnots.length() );

    for( OdInt32 j = 0; j < vrKnots.length(); j++ )
    {
      OdString strKnotName;
      strKnotName.format(L"  Knot %d", j);
      pDumper->writeFieldValue( strKnotName, vrKnots[j] );
    }

    if( bRational )
    {
      pDumper->writeFieldValue(L"Num Weights", (OdUInt32)arrWeights.size() );

      for( OdUInt32 k = 0; k < arrWeights.length(); k++ )
      {
        OdString strWeightName;
        strWeightName.format(L"  Weight %d", k);
        pDumper->writeFieldValue( strWeightName, arrWeights[k] );
      }
    }
  }
}

void OdDgSurfaceDumperPE::dumpData( OdRxObjectPtr pObj, OdExDgnDumper* pDumper ) const
{
  OdDgGraphicsElementDumperPE::dumpData(pObj, pDumper);
}

void OdDgSolidDumperPE::dumpData( OdRxObjectPtr pObj, OdExDgnDumper* pDumper ) const
{
  OdDgGraphicsElementDumperPE::dumpData(pObj, pDumper);
}

void OdDgMeshFaceLoopsDumperPE::dumpData( OdRxObjectPtr pObj, OdExDgnDumper* pDumper ) const
{
  OdDgGraphicsElementDumperPE::dumpData(pObj, pDumper);

//   OdDgMeshFaceLoopsPtr pMesh = pObj;
// 
//   OdUInt32 nFaces = pMesh->getFacesNumber();
// 
//   for( OdUInt32 i = 0; i < nFaces; i++ )
//   {
//     OdDgMeshFaceLoops::VerticesArray pointsData;
// 
//     pDumper->writeFieldValue(L"  Face number", i );
// 
//     pMesh->getFace( i, pointsData);
//     
//     for( OdUInt32 j = 0; j < pointsData.size(); j++ )
//     {
//       pDumper->writeFieldValue(L"    Point number", j );
//       pDumper->writeFieldValue(L"    Point value", pMesh->getPoint( pointsData[j].m_pointIndex ) );
// 
//       if( pMesh->getUseColorTableIndexesFlag() )
//       {
//         pDumper->writeFieldValue(L"    Color Index", pointsData[j].m_colorIndex );
//       }
// 
//       if( pMesh->getUseDoubleColorsFlag() )
//       {
//         OdString strColors;
//         strColors.format(L"%d %d %d", pointsData[j].m_dColorRGB[0], pointsData[j].m_dColorRGB[1], pointsData[j].m_dColorRGB[2] );
//         pDumper->writeFieldValue(L"    Double colors", strColors );
//       }
// 
//       if( pMesh->getUseNormalsFlag() )
//       {
//         pDumper->writeFieldValue(L"    Normal", pointsData[j].m_vrNormal );
//       }
// 
//       if( pMesh->getUseTextureCoordinatesFlag() )
//       {
//         pDumper->writeFieldValue(L"    Texture Coordinate", pointsData[j].m_texCoords );
//       }
//     }
//   }
}

void OdDgRasterAttachmentHeaderDumperPE::dumpData( OdRxObjectPtr pObj, OdExDgnDumper* pDumper ) const
{
  OdDgElementDumperPE::dumpData(pObj, pDumper);

  OdDgRasterAttachmentHeaderPtr element = pObj;

  pDumper->writeFieldValue( "File name", element->getFilename() );
  pDumper->writeFieldValue( "File path", element->getFilePath() );
  pDumper->writeFieldValue( "Logical name", element->getLogicalName() );
  pDumper->writeFieldValue( "Description", element->getDescription() );

  //simple fields
  {
    OdGePoint3d origin;
    OdGeVector3d u, v;    //these values are logged later with OdDgRasterAttachmentComponentGeo pObj

    element->getOrientation( origin, u, v );
    pDumper->writeFieldValue( "Origin", origin );
  }
  pDumper->writeFieldValue( "Extent", element->getExtent() );
  pDumper->writeFieldValue( "Display gamma", element->getDisplayGamma() );
  pDumper->writeFieldValue( "Plot gamma", element->getPlotGamma() );
  pDumper->writeFieldValue( "Apply rotation", element->getApplyRotationFlag() );
  pDumper->writeFieldValue( "Clipping", element->getClippingFlag() );
  pDumper->writeFieldValue( "Plot", element->getPlotFlag() );
  pDumper->writeFieldValue( "Invert", element->getInvertFlag() );
  {
    int   i;
    char  name[ 20 ];

    for( i = 1; i <= 8; i++ )
    {
      sprintf( name, "View %d", i );
      pDumper->writeFieldValue( name, element->getViewFlag( i ) );
    }
  }
  pDumper->writeFieldValue( "Raster plane", element->getRasterPlane() );
  pDumper->writeFieldValue( "Raster layer", element->getRasterLayer() );

  OdUInt16 j;
  OdUInt32 k;
  OdString strName;
  OdString strData;

  for( j = 0; j < element->getBoundaryClipNumber(); j++ )
  {
    OdString strType = L"Boundary";
    pDumper->writeFieldValue( L"  Clip Type",  strType );

    OdGePoint2dArray arrClipPoints;
    element->getBoundaryClipVertices( j, arrClipPoints );

    for( k = 0; k < arrClipPoints.size(); k++ )
    {
      strName.format( L"  Point %d",k);
      strType.format( L"X = %f, Y = %f", arrClipPoints[k].x, arrClipPoints[k].y);
      pDumper->writeFieldValue( strName, strType);
    }
  }

  for( j = 0; j < element->getMaskClipNumber(); j++ )
  {
    OdString strType = L"Mask";
    pDumper->writeFieldValue( L"  Clip Type",  strType );

    OdGePoint2dArray arrClipPoints;
    element->getMaskClipVertices( j, arrClipPoints );

    for( k = 0; k < arrClipPoints.size(); k++ )
    {
      strName.format( L"  Point %d",k);
      strType.format( L"X = %f, Y = %f", arrClipPoints[k].x, arrClipPoints[k].y);
      pDumper->writeFieldValue( strName, strType);
    }
  }

  OdDgRasterFrame::OdDgRasterColorMode clrMode = element->getColorMode();

  pDumper->writeFieldValue(L"Raster Color Mode", clrMode );

  if( clrMode == OdDgRasterFrame::k256ColorsMode || clrMode == OdDgRasterFrame::k16ColorsMode )
  {
    pDumper->writeFieldValue(L"Transparency All Colors", element->getAllColorTransparency() );

    if( element->getTransparencyIndexEntryCount() )
    {
      pDumper->dumpFieldName(L"Index Transparency:");

      for( k = 0; k < element->getTransparencyIndexEntryCount(); k++ )
      {
        OdDgRasterAttachmentHeader::OdDgIndexTransEntry trans = element->getTransparencyIndexEntry(k);

        ODCOLORREF clrTrans = element->getColorTableEntry( trans.m_uIndex );

        OdString strColor;
        strColor.format(L"   %08X", clrTrans );

        pDumper->writeFieldValue( strColor, trans.m_dTransparencyPercentage );
      }
    }
  }
  else if( clrMode == OdDgRasterFrame::kRGBMode || clrMode == OdDgRasterFrame::kRGBAMode )
  {
    pDumper->writeFieldValue(L"Transparency All Colors", element->getAllColorTransparency() );

    if( element->getTransparencyRgbEntryCount() )
    {
      pDumper->dumpFieldName(L"RGB Transparency:");

      for( k = 0; k < element->getTransparencyRgbEntryCount(); k++ )
      {
        OdDgRasterAttachmentHeader::OdDgRGBTransEntry trans = element->getTransparencyRgbEntry(k);

        OdString strColor;
        strColor.format(L"  %08X - %08X", trans.m_minColor, trans.m_maxColor );

        pDumper->writeFieldValue( strColor, trans.m_dTransparencyPercentage );
      }
    }
  }
  else if( clrMode != OdDgRasterFrame::kUnknownMode )
  {
    pDumper->writeFieldValue(L"Transparency Foreground", element->getForegroundTransparency() );
    pDumper->writeFieldValue(L"Transparency Background", element->getBackgroundTransparency() );
  }
}

void OdDgRasterHeader2dDumperPE::dumpData( OdRxObjectPtr pObj, OdExDgnDumper* pDumper ) const
{
  OdDgGraphicsElementDumperPE::dumpData(pObj, pDumper);

  OdDgRasterHeader2dPtr element = pObj;

  //simple fields
  pDumper->writeFieldValue( "Right justified", element->getRightJustifiedFlag() );
  pDumper->writeFieldValue( "Lower justified", element->getLowerJustifiedFlag() );
  pDumper->writeFieldValue( "Horizontal data", element->getHorizontalDataFlag() );
  pDumper->writeFieldValue( "Color", element->getColorFlag() );
  pDumper->writeFieldValue( "Transparent", element->getTransparentFlag() );
  pDumper->writeFieldValue( "Positive", element->getPositiveFlag() );
  pDumper->writeFieldValue( "Raster format", element->getFormat() );
  pDumper->writeFieldValue( "Foreground", element->getForeground() );
  pDumper->writeFieldValue( "Background", element->getBackground() );
  pDumper->writeFieldValue( "X extent", element->getXExtent() );
  pDumper->writeFieldValue( "Y extent", element->getYExtent() );
  pDumper->writeFieldValue( "Scale", element->getScale() );
  {
    OdGePoint3d origin = element->getOrigin();
    pDumper->writeFieldValue( "Origin", origin );
  }
}

void OdDgRasterHeader3dDumperPE::dumpData( OdRxObjectPtr pObj, OdExDgnDumper* pDumper ) const
{
  OdDgGraphicsElementDumperPE::dumpData(pObj, pDumper);

  OdDgRasterHeader3dPtr element = pObj;

  //simple fields
  pDumper->writeFieldValue( "Right justified", element->getRightJustifiedFlag() );
  pDumper->writeFieldValue( "Lower justified", element->getLowerJustifiedFlag() );
  pDumper->writeFieldValue( "Horizontal data", element->getHorizontalDataFlag() );
  pDumper->writeFieldValue( "Color", element->getColorFlag() );
  pDumper->writeFieldValue( "Transparent", element->getTransparentFlag() );
  pDumper->writeFieldValue( "Positive", element->getPositiveFlag() );
  pDumper->writeFieldValue( "Raster format", element->getFormat() );
  pDumper->writeFieldValue( "Foreground", element->getForeground() );
  pDumper->writeFieldValue( "Background", element->getBackground() );
  pDumper->writeFieldValue( "X extent", element->getXExtent() );
  pDumper->writeFieldValue( "Y extent", element->getYExtent() );
  pDumper->writeFieldValue( "Scale", element->getScale() );
  {
    OdGePoint3d origin = element->getOrigin();
    pDumper->writeFieldValue( "Origin", origin );
  }
}

void OdDgRasterComponentDumperPE::dumpData( OdRxObjectPtr pObj, OdExDgnDumper* pDumper ) const
{
  OdDgGraphicsElementDumperPE::dumpData(pObj, pDumper);

  OdDgRasterComponentPtr element = pObj;

  //simple fields
  pDumper->writeFieldValue( "Right justified", element->getRightJustifiedFlag() );
  pDumper->writeFieldValue( "Lower justified", element->getLowerJustifiedFlag() );
  pDumper->writeFieldValue( "Horizontal data", element->getHorizontalDataFlag() );
  pDumper->writeFieldValue( "Color", element->getColorFlag() );
  pDumper->writeFieldValue( "Transparent", element->getTransparentFlag() );
  pDumper->writeFieldValue( "Positive", element->getPositiveFlag() );
  pDumper->writeFieldValue( "Raster format", element->getFormat() );
  pDumper->writeFieldValue( "Foreground", element->getForeground() );
  pDumper->writeFieldValue( "Background", element->getBackground() );
  pDumper->writeFieldValue( "Offset X", element->getOffsetX() );
  pDumper->writeFieldValue( "Offset Y", element->getOffsetY() );
  pDumper->writeFieldValue( "Number of pixels", element->getNumberOfPixels() );
}

void OdDgTagElementDumperPE::dumpData( OdRxObjectPtr pObj, OdExDgnDumper* pDumper ) const
{
  OdDgGraphicsElementDumperPE::dumpData(pObj, pDumper);

  OdDgTagElementPtr element = pObj;

  //simple fields
  pDumper->writeFieldValue( "Origin", element->getOrigin() );
  pDumper->writeFieldValue( "Offset", element->getOffset() );
  pDumper->writeFieldValue( "Rotation (3d)", element->getRotation3d() );
  pDumper->writeFieldValue( "Associated", element->getAssociatedFlag() );
  pDumper->writeFieldValue( "Offset is used", element->getOffsetUsedFlag() );
  pDumper->writeFieldValue( "Justification is used", element->getJustificationUsedFlag() );
  pDumper->writeFieldValue( "Hide From Shared Cell", element->getHideForSharedCellFlag() );
  {
    OdDgElementId setId;
    OdUInt16      definitionId;

    element->getTagDefinitionId( setId, definitionId );
    pDumper->writeFieldValue( "Id of the set", setId );
    pDumper->writeFieldValue( "Entry Id of the definition", definitionId );

    if( !setId.isNull() )
    {
      OdDgTagDefinitionSetPtr pTagSet =  setId.safeOpenObject();
      if (!pTagSet.isNull())
      {
        pDumper->writeFieldValue( "TagDefinitionsSet Name", pTagSet->getName() );
        OdDgTagDefinitionPtr pTagDefinition = pTagSet->getByEntryId( definitionId );
        if (!pTagDefinition.isNull())
        {
          pDumper->writeFieldValue( "TagDefinition Name", pTagDefinition->getName());
          pDumper->writeFieldValue( "TagDefinition Type", pTagDefinition->getType());
        }
      }
    }
  }
  pDumper->writeFieldValue( "Id of the associated element", element->getAssociationId() );
  pDumper->writeFieldValue( "Freeze group", element->getFreezeGroup() );

  switch ( element->getDataType() )
  {
  case OdDgTagDefinition::kChar:
    pDumper->writeFieldValue( "Type", OdString( "char" ) );
    pDumper->writeFieldValue( "Value", element->getString() );
    break;
  case OdDgTagDefinition::kInt16:
    pDumper->writeFieldValue( "Type", OdString( "int16" ) );
    pDumper->writeFieldValue( "Value", element->getInt16() );
    break;
  case OdDgTagDefinition::kInt32:
    pDumper->writeFieldValue( "Type", OdString( "int32" ) );
    pDumper->writeFieldValue( "Value", element->getInt32() );
    break;
  case OdDgTagDefinition::kDouble:
    pDumper->writeFieldValue( "Type", OdString( "int16" ) );
    pDumper->writeFieldValue( "Value", element->getDouble() );
    break;
  case OdDgTagDefinition::kBinary:
    pDumper->writeFieldValue( "Type", OdString( "int16" ) );
    pDumper->writeFieldValue( "Size", element->getBinarySize() );
    break;
  default:
    ODA_FAIL();
    break;
  }

  pDumper->writeFieldValue( "Use interChar spacing", element->getUseInterCharSpacingFlag() );
  pDumper->writeFieldValue( "Fixed width spacing", element->getFixedWidthSpacingFlag() );
  pDumper->writeFieldValue( "Underlined", element->getUnderlineFlag() );
  pDumper->writeFieldValue( "Use slant", element->getUseSlantFlag() );
  pDumper->writeFieldValue( "Vertical", element->getVerticalFlag() );
  pDumper->writeFieldValue( "Right-to-left", element->getRightToLeftFlag() );
  pDumper->writeFieldValue( "Reverse MLine", element->getReverseMlineFlag() );
  pDumper->writeFieldValue( "Kerning", element->getKerningFlag() );
  pDumper->writeFieldValue( "Use codepage", element->getUseCodepageFlag() );
  pDumper->writeFieldValue( "Use SHX big font", element->getUseShxBigFontFlag() );
  pDumper->writeFieldValue( "Subscript", element->getSubscriptFlag() );
  pDumper->writeFieldValue( "Superscript", element->getSuperscriptFlag() );
  pDumper->writeFieldValue( "Use text style", element->getUseTextStyleFlag() );
  pDumper->writeFieldValue( "Overlined", element->getOverlineFlag() );
  pDumper->writeFieldValue( "Bold", element->getBoldFlag() );
  pDumper->writeFieldValue( "Full justification", element->getFullJustificationFlag() );
  pDumper->writeFieldValue( "ACAD interChar spacing", element->getAcadInterCharSpacingFlag() );
  pDumper->writeFieldValue( "Backwards", element->getBackwardsFlag() );
  pDumper->writeFieldValue( "Upside down", element->getUpsideDownFlag() );
  pDumper->writeFieldValue( "ACAD fitted text", element->getAcadFittedTextFlag() );

  pDumper->writeFieldValue( "Slant", element->getSlant() );
  pDumper->writeFieldValue( "Character spacing", element->getCharacterSpacing() );
  pDumper->writeFieldValue( "Underline spacing", element->getUnderlineSpacing() );
  pDumper->writeFieldValue( "Length multiplier", element->getLengthMultiplier() );
  pDumper->writeFieldValue( "Height multiplier", element->getHeightMultiplier() );
  pDumper->writeFieldValue( "Text style ID", element->getTextStyleEntryId() );
  pDumper->writeFieldValue( "SHX big font", element->getShxBigFont() );
  pDumper->writeFieldValue( "Font ID", element->getFontEntryId() );
  pDumper->writeFieldValue( "Justification", element->getJustification() );
  pDumper->writeFieldValue( "Codepage", element->getCodepage() );
}

void OdDgCellHeader2dDumperPE::dumpData( OdRxObjectPtr pObj, OdExDgnDumper* pDumper ) const
{
  OdDgGraphicsElementDumperPE::dumpData(pObj, pDumper);

  OdDgCellHeader2dPtr pCell = pObj;

  pDumper->writeFieldValue( "Name", pCell->getName() );
  pDumper->writeFieldValue( "Origin", pCell->getOrigin() );
  pDumper->writeFieldValue( "Transformation", pCell->getTransformation() );
//   pDumper->writeFieldValue( "Annotation Cell", pCell->getAnnotationCellFlag() );
//   pDumper->writeFieldValue( "Use Annotation Scale", pCell->getUseAnnotationScaleFlag() );
//   pDumper->writeFieldValue( "Annotation Scale", pCell->getAnnotationScale() );
}

void OdDgCellHeader3dDumperPE::dumpData( OdRxObjectPtr pObj, OdExDgnDumper* pDumper ) const
{
  OdDgGraphicsElementDumperPE::dumpData(pObj, pDumper);

  OdDgCellHeader3dPtr pCell = pObj;

  pDumper->writeFieldValue( "Name", pCell->getName() );
  pDumper->writeFieldValue( "Origin", pCell->getOrigin() );
  pDumper->writeFieldValue( "Transformation", pCell->getTransformation() );
//   pDumper->writeFieldValue( "Annotation Cell", pCell->getAnnotationCellFlag() );
//   pDumper->writeFieldValue( "Use Annotation Scale", pCell->getUseAnnotationScaleFlag() );
//   pDumper->writeFieldValue( "Annotation Scale", pCell->getAnnotationScale() );
}

void OdDgOle2Frame2dDumperPE::dumpData( OdRxObjectPtr pObj, OdExDgnDumper* pDumper ) const
{
  OdDgCellHeader2dDumperPE::dumpData(pObj, pDumper);

  OdDgOle2Frame2dPtr pOle = pObj;

  pDumper->writeFieldValue( "Ole Type", pOle->getOleType() );
  pDumper->writeFieldValue( "Draw Aspect", pOle->getDrawAspect() );
  pDumper->writeFieldValue( "Aspect Ratio Flag", pOle->getAspectRatioFlag() );
  pDumper->writeFieldValue( "View Rotation Mode", pOle->getViewRotationMode() );
  pDumper->writeFieldValue( "Transparent Background Flag", pOle->getTransparentBackgroundFlag() );
  pDumper->writeFieldValue( "Can be Linked and Embedded Flag", pOle->getCanBeLinkedAndEmbeddedFlag() );
  pDumper->writeFieldValue( "Can be Picture Flag", pOle->getCanBePictureFlag() );
  pDumper->writeFieldValue( "Object Size X", pOle->getObjectSizeX()*pOle->getXScale());
  pDumper->writeFieldValue( "Object Size Y", pOle->getObjectSizeY()*pOle->getYScale());
  pDumper->writeFieldValue( "Scale X", pOle->getXScale() );
  pDumper->writeFieldValue( "Scale Y", pOle->getYScale() );
  pDumper->writeFieldValue( "Position", pOle->getPosition() );
  pDumper->writeFieldValue( "Rotation Angle", pOle->getRotationAngle() );
  pDumper->writeFieldValue( "Compound Document Data Size", pOle->getCompoundDocumentDataSize() );
}

void OdDgOle2Frame3dDumperPE::dumpData( OdRxObjectPtr pObj, OdExDgnDumper* pDumper ) const
{
  OdDgCellHeader3dDumperPE::dumpData(pObj, pDumper);

  OdDgOle2Frame3dPtr pOle = pObj;

  pDumper->writeFieldValue( "Ole Type", pOle->getOleType() );
  pDumper->writeFieldValue( "Draw Aspect", pOle->getDrawAspect() );
  pDumper->writeFieldValue( "Aspect Ratio Flag", pOle->getAspectRatioFlag() );
  pDumper->writeFieldValue( "View Rotation Mode", pOle->getViewRotationMode() );
  pDumper->writeFieldValue( "Transparent Background Flag", pOle->getTransparentBackgroundFlag() );
  pDumper->writeFieldValue( "Can be Linked and Embedded Flag", pOle->getCanBeLinkedAndEmbeddedFlag() );
  pDumper->writeFieldValue( "Can be Picture Flag", pOle->getCanBePictureFlag() );
  pDumper->writeFieldValue( "Object Size X", pOle->getObjectSizeX()*pOle->getXScale());
  pDumper->writeFieldValue( "Object Size Y", pOle->getObjectSizeY()*pOle->getYScale());
  pDumper->writeFieldValue( "Scale X", pOle->getXScale() );
  pDumper->writeFieldValue( "Scale Y", pOle->getYScale() );
  pDumper->writeFieldValue( "Position", pOle->getPosition() );
  pDumper->writeFieldValue( "Rotation Angle", pOle->getRotation() );
  pDumper->writeFieldValue( "Compound Document Data Size", pOle->getCompoundDocumentDataSize() );
}


void OdDgDistantLightDumperPE::dumpData( OdRxObjectPtr pObj, OdExDgnDumper* pDumper ) const
{
  OdDgCellHeader3dDumperPE::dumpData(pObj, pDumper);

  OdDgLightDistantPtr pLight = pObj;

  pDumper->writeFieldValue( "Light name", pLight->getLightName() );
  pDumper->writeFieldValue( "On Flag"   , pLight->getOnFlag() );
  pDumper->writeFieldValue( "Intensity" , pLight->getIntensity() );
  pDumper->writeFieldValue( "Brightness" , pLight->getBrightness() );
  pDumper->writeFieldValue( "Shadows On Flag" , pLight->getShadowOnFlag() );
  pDumper->writeFieldValue( "Shadow Resolution" , pLight->getShadowResolution() );
  pDumper->writeFieldValue( "Color Red" , pLight->getColorRed() );
  pDumper->writeFieldValue( "Color Green" , pLight->getColorGreen() );
  pDumper->writeFieldValue( "Color Blue" , pLight->getColorBlue() );

  if( pLight->getExtendedParamCount() > 0 )
  {
    pDumper->dumpFieldName(L"Extended params:");

    for( OdUInt32 i = 0; i < pLight->getExtendedParamCount(); i++ )
    {
      OdDgLightParam tmp = pLight->getExtendedParam(i);
      pDumper->writeFieldValue(L"  " +  tmp.m_strParamName, tmp.m_strParamValue );
    }
  }
}

void OdDgPointLightDumperPE::dumpData( OdRxObjectPtr pObj, OdExDgnDumper* pDumper ) const
{
  OdDgCellHeader3dDumperPE::dumpData(pObj, pDumper);

  OdDgLightPointPtr pLight = pObj;

  pDumper->writeFieldValue( "Light name", pLight->getLightName() );
  pDumper->writeFieldValue( "On Flag"   , pLight->getOnFlag() );
  pDumper->writeFieldValue( "Intensity" , pLight->getIntensity() );
  pDumper->writeFieldValue( "Brightness" , pLight->getBrightness() );
  pDumper->writeFieldValue( "Shadows On Flag" , pLight->getShadowOnFlag() );
  pDumper->writeFieldValue( "Shadow Resolution" , pLight->getShadowResolution() );
  pDumper->writeFieldValue( "Color Red" , pLight->getColorRed() );
  pDumper->writeFieldValue( "Color Green" , pLight->getColorGreen() );
  pDumper->writeFieldValue( "Color Blue" , pLight->getColorBlue() );
  pDumper->writeFieldValue( "Attenuate On Flag" , pLight->getAttenuateOnFlag() );
  pDumper->writeFieldValue( "Attenuation Distance" , pLight->getAttenuationDistance() );
  pDumper->writeFieldValue( "IES Data On Flag" , pLight->getIESDataOnFlag() );
  pDumper->writeFieldValue( "IES Rotation" , pLight->getIESRotation() );
  pDumper->writeFieldValue( "IES Filename" , pLight->getIESFilename() );

  if( pLight->getExtendedParamCount() > 0 )
  {
    pDumper->dumpFieldName(L"Extended params:");

    for( OdUInt32 i = 0; i < pLight->getExtendedParamCount(); i++ )
    {
      OdDgLightParam tmp = pLight->getExtendedParam(i);
      pDumper->writeFieldValue(L"  " +  tmp.m_strParamName, tmp.m_strParamValue );
    }
  }
}

void OdDgSpotLightDumperPE::dumpData( OdRxObjectPtr pObj, OdExDgnDumper* pDumper ) const
{
  OdDgCellHeader3dDumperPE::dumpData(pObj, pDumper);

  OdDgLightSpotPtr pLight = pObj;

  pDumper->writeFieldValue( "Light name", pLight->getLightName() );
  pDumper->writeFieldValue( "On Flag"   , pLight->getOnFlag() );
  pDumper->writeFieldValue( "Intensity" , pLight->getIntensity() );
  pDumper->writeFieldValue( "Brightness" , pLight->getBrightness() );
  pDumper->writeFieldValue( "Shadows On Flag" , pLight->getShadowOnFlag() );
  pDumper->writeFieldValue( "Shadow Resolution" , pLight->getShadowResolution() );
  pDumper->writeFieldValue( "Color Red" , pLight->getColorRed() );
  pDumper->writeFieldValue( "Color Green" , pLight->getColorGreen() );
  pDumper->writeFieldValue( "Color Blue" , pLight->getColorBlue() );
  pDumper->writeFieldValue( "Attenuate On Flag" , pLight->getAttenuateOnFlag() );
  pDumper->writeFieldValue( "Attenuation Distance" , pLight->getAttenuationDistance() );
  pDumper->writeFieldValue( "IES Data On Flag" , pLight->getIESDataOnFlag() );
  pDumper->writeFieldValue( "IES Rotation" , pLight->getIESRotation() );
  pDumper->writeFieldValue( "IES Filename" , pLight->getIESFilename() );
  pDumper->writeFieldValue( "Cone Angle" , pLight->getConeAngle() );
  pDumper->writeFieldValue( "Delta Angle" , pLight->getDeltaAngle() );

  if( pLight->getExtendedParamCount() > 0 )
  {
    pDumper->dumpFieldName(L"Extended params:");

    for( OdUInt32 i = 0; i < pLight->getExtendedParamCount(); i++ )
    {
      OdDgLightParam tmp = pLight->getExtendedParam(i);
      pDumper->writeFieldValue(L"  " +  tmp.m_strParamName, tmp.m_strParamValue );
    }
  }
}

void OdDgAreaLightDumperPE::dumpData( OdRxObjectPtr pObj, OdExDgnDumper* pDumper ) const
{
  OdDgCellHeader3dDumperPE::dumpData(pObj, pDumper);

  OdDgLightAreaPtr pLight = pObj;

  pDumper->writeFieldValue( "Light name", pLight->getLightName() );
  pDumper->writeFieldValue( "On Flag"   , pLight->getOnFlag() );
  pDumper->writeFieldValue( "Intensity" , pLight->getIntensity() );
  pDumper->writeFieldValue( "Brightness" , pLight->getBrightness() );
  pDumper->writeFieldValue( "Shadows On Flag" , pLight->getShadowOnFlag() );
  pDumper->writeFieldValue( "Shadow Resolution" , pLight->getShadowResolution() );
  pDumper->writeFieldValue( "Color Red" , pLight->getColorRed() );
  pDumper->writeFieldValue( "Color Green" , pLight->getColorGreen() );
  pDumper->writeFieldValue( "Color Blue" , pLight->getColorBlue() );
  pDumper->writeFieldValue( "Attenuate On Flag" , pLight->getAttenuateOnFlag() );
  pDumper->writeFieldValue( "Attenuation Distance" , pLight->getAttenuationDistance() );
  pDumper->writeFieldValue( "IES Data On Flag" , pLight->getIESDataOnFlag() );
  pDumper->writeFieldValue( "IES Rotation" , pLight->getIESRotation() );
  pDumper->writeFieldValue( "IES Filename" , pLight->getIESFilename() );
  pDumper->writeFieldValue( "Sample Count" , pLight->getSampleCount() );

  if( pLight->getExtendedParamCount() > 0 )
  {
    pDumper->dumpFieldName(L"Extended params:");

    for( OdUInt32 i = 0; i < pLight->getExtendedParamCount(); i++ )
    {
      OdDgLightParam tmp = pLight->getExtendedParam(i);
      pDumper->writeFieldValue(L"  " +  tmp.m_strParamName, tmp.m_strParamValue );
    }
  }
}

void OdDgSkyOpeningLightDumperPE::dumpData( OdRxObjectPtr pObj, OdExDgnDumper* pDumper ) const
{
  OdDgCellHeader3dDumperPE::dumpData(pObj, pDumper);

  OdDgLightOpenSkyPtr pLight = pObj;

  pDumper->writeFieldValue( "Light name", pLight->getLightName() );
  pDumper->writeFieldValue( "On Flag"   , pLight->getOnFlag() );
  pDumper->writeFieldValue( "Min samples" , pLight->getMinSamples() );
  pDumper->writeFieldValue( "Max samples" , pLight->getMaxSamples() );

  if( pLight->getExtendedParamCount() > 0 )
  {
    pDumper->dumpFieldName(L"Extended params:");

    for( OdUInt32 i = 0; i < pLight->getExtendedParamCount(); i++ )
    {
      OdDgLightParam tmp = pLight->getExtendedParam(i);
      pDumper->writeFieldValue(L"  " +  tmp.m_strParamName, tmp.m_strParamValue );
    }
  }
}

void OdDgBSplineSurfaceDumperPE::dumpData( OdRxObjectPtr pObj, OdExDgnDumper* pDumper ) const
{
  OdDgGraphicsElementDumperPE::dumpData(pObj, pDumper);

  OdDgBSplineSurfacePtr element = pObj;

  OdUInt8 uOrderU, uOrderV;
  bool    bRational, bClosedInU, bClosedInV;
  int     nCtrlPtsInU, nCtrlPtsInV;
  OdGePoint3dArray arrCtrlPts;
  OdGeDoubleArray  arrWeights;
  OdGeKnotVector   arrKnotsU, arrKnotsV;
  OdUInt32 nRulesU, nRulesV;

  OdResult retVal = element->get( uOrderU, uOrderV, bRational, bClosedInU, bClosedInV, nCtrlPtsInU, nCtrlPtsInV, arrCtrlPts,
                                  arrWeights, arrKnotsU, arrKnotsV );

  if( retVal != eOk )
  {
    pDumper->dumpFieldName("Corrupted b-spline surface.");
    return;
  }

  element->getNumberOfSpansInU( nRulesU );
  element->getNumberOfSpansInV( nRulesV );

  //simple fields
  pDumper->writeFieldValue( "Order U", uOrderU );
  pDumper->writeFieldValue( "Order V", uOrderV );
  pDumper->writeFieldValue( "Closed U", bClosedInU );
  pDumper->writeFieldValue( "Closed V", bClosedInV );
  pDumper->writeFieldValue( "Display curve", element->getSurfaceDisplayFlag() );
  pDumper->writeFieldValue( "Display polygon", element->getControlNetDisplayFlag() );
  pDumper->writeFieldValue( "Rational", bRational );
  pDumper->writeFieldValue( "Hole", element->getHbitFlag() );
  pDumper->writeFieldValue( "Number of rules U", nRulesU );
  pDumper->writeFieldValue( "Number of rules V", nRulesV );
  pDumper->writeFieldValue( "Number of poles U", nCtrlPtsInU );
  pDumper->writeFieldValue( "Number of poles V", nCtrlPtsInV );
  pDumper->writeFieldValue( "Control points :", (OdUInt32)arrCtrlPts.size() );

  OdInt32 i,j;

  for( i = 0; i < nCtrlPtsInV; i++ )
  {
    for( j = 0; j < nCtrlPtsInU; j++ )
    {
      OdString strCtrlPoint;
      strCtrlPoint.format(L"  Point[%d,%d]", i, j);
      pDumper->writeFieldValue( strCtrlPoint, arrCtrlPts[i * nCtrlPtsInU + j] );
    }
  }

  if( bRational )
  {
    pDumper->writeFieldValue( "Weights :", (OdUInt32)arrWeights.size() );

    for( i = 0; i < nCtrlPtsInV; i++ )
    {
      for( j = 0; j < nCtrlPtsInU; j++ )
      {
        OdString strWeight;
        strWeight.format(L"  Weight[%d,%d]", i, j);
        pDumper->writeFieldValue( strWeight, arrWeights[i * nCtrlPtsInU + j] );
      }
    }
  }

  pDumper->writeFieldValue( "U Knots: ", (OdUInt32)arrKnotsU.length() );

  for( i = 0; i < arrKnotsU.length(); i++ )
  {
    OdString strKnot;
    strKnot.format(L"  KnotU %d", i);
    pDumper->writeFieldValue( strKnot, arrKnotsU[i] );
  }

  pDumper->writeFieldValue( "V Knots: ", (OdUInt32)arrKnotsV.length() );

  for( i = 0; i < arrKnotsV.length(); i++ )
  {
    OdString strKnot;
    strKnot.format(L"  KnotV %d", i);
    pDumper->writeFieldValue( strKnot, arrKnotsV[i] );
  }

  OdInt32 nBoundaries = (OdInt32)element->getBoundariesCount();

  pDumper->writeFieldValue( "Number of boundaries", nBoundaries );

  for( i = 0; i < nBoundaries; i++ )
  {
    OdGePoint2dArray arrBoundaryPts;
    element->getBoundary( (OdUInt32)i, arrBoundaryPts );

    OdString strBoundary;
    strBoundary.format(L"  Boundary %d :", i);
    pDumper->writeFieldValue( strBoundary, (OdUInt32)arrBoundaryPts.size() );

    for( OdUInt32 k = 0; k < arrBoundaryPts.size(); k++ )
    {
      OdString strBoundaryPt;
      strBoundaryPt.format(L"    Point %d :", k);
      pDumper->writeFieldValue( strBoundaryPt, arrBoundaryPts[k] );
    }
  }
}


void OdDgProxyElementDumperPE::dumpData( OdRxObjectPtr pObj, OdExDgnDumper* pDumper ) const
{
  OdDgElementDumperPE::dumpData(pObj, pDumper);
  OdDgElementPtr pElm = pObj;
  pDumper->writeFieldValue( "Element Type", pElm->getElementType() );
  pDumper->writeFieldValue( "Element SubType", pElm->getElementSubType() );

  OdDgProxyElementPtr pProxyElm = pObj;

  OdBinaryData binData;
  pProxyElm->getData( binData );

  pDumper->writeFieldValue( "Data Size", (OdUInt32)(binData.size()) );

  OdString strDataLine;
  OdString strData;
  OdUInt32 nData = 0;
  OdUInt32 uDataLine = 0;

  OdUInt16* pWordData = (OdUInt16*)(binData.asArrayPtr());

  for( OdUInt32 l = 0; l < binData.length()/2; l++ )
  {
    OdString strByte;
    strByte.format(L"0x%04X, ", pWordData[l]);
    strData += strByte;
    nData++;

    if( nData == 4 )
    {
      strData = strData.left( strData.getLength() - 2 );
      strDataLine.format(L"  Line %d", uDataLine );
      uDataLine++;

      pDumper->writeFieldValue( strDataLine, strData );
      strData.empty();
      nData = 0;
    }
  }

  if( !strData.isEmpty() )
  {
    strData = strData.left( strData.getLength() - 2 );
    strDataLine.format(L"  Line %d", uDataLine );

    pDumper->writeFieldValue( strDataLine, strData );
  }
}

OdDgElementIteratorPtr OdDgComplexProxyElementDumperPE::createIterator( OdDgElementPtr pElm, bool atBeginning, bool skipDeleted) const
{
  OdDgComplexProxyElementPtr pComplElm = pElm;
  return pComplElm->createIterator( atBeginning, skipDeleted );
}

void OdDgComplexProxyElementDumperPE::dumpData( OdRxObjectPtr pObj, OdExDgnDumper* pDumper ) const
{
  OdDgElementDumperPE::dumpData(pObj, pDumper);
  OdDgComplexProxyElementPtr pComplElm = pObj;

  pDumper->writeFieldValue( "Element Type", pComplElm->getElementType() );
  pDumper->writeFieldValue( "Element SubType", pComplElm->getElementSubType() );

  OdBinaryData binData;
  pComplElm->getData( binData );

  pDumper->writeFieldValue( "Data Size", (OdUInt32)(binData.size()) );

  OdString strDataLine;
  OdString strData;
  OdUInt32 nData = 0;
  OdUInt32 uDataLine = 0;

  OdUInt16* pWordData = (OdUInt16*)(binData.asArrayPtr());

  for( OdUInt32 l = 0; l < binData.length()/2; l++ )
  {
    OdString strByte;
    strByte.format(L"0x%04X, ", pWordData[l]);
    strData += strByte;
    nData++;

    if( nData == 4 )
    {
      strData = strData.left( strData.getLength() - 2 );
      strDataLine.format(L"  Line %d", uDataLine );
      uDataLine++;

      pDumper->writeFieldValue( strDataLine, strData );
      strData.empty();
      nData = 0;
    }
  }

  if( !strData.isEmpty() )
  {
    strData = strData.left( strData.getLength() - 2 );
    strDataLine.format(L"  Line %d", uDataLine );

    pDumper->writeFieldValue( strDataLine, strData );
  }
}

inline OdString rscIdAsString(OdUInt32 type)
{
  return OdString((char*)&type, 4).makeReverse();
}

void OdDgCompoundLineStyleResourceDumperPE::dump( OdRxObjectPtr pObj, OdExDgnDumper* pDumper ) const
{
  OdDgRxObjectDumperPE::dump(pObj, pDumper);

  OdDgCompoundLineStyleResourcePtr pRes = pObj;

  pDumper->writeFieldValue( L"Description", pRes->getDescription() );

  for ( unsigned i = 0; i < pRes->getComponentCount(); ++i )
  {
    OdDgCompoundLineStyleComponentInfo cmpInfo;
    pRes->getComponent( i, cmpInfo );

    pDumper->writeFieldValue( L"Component (index)", (OdUInt32)i );
    OdString str;
    str.format(L"  Component %d Type", i);
    pDumper->writeFieldValue( str, cmpInfo.getComponentType() );
    str.format(L"  Component %d Offset", i);
    pDumper->writeFieldValue( str, cmpInfo.getComponentOffset() );

    if( cmpInfo.getComponentHandleId() == 0 )
    {
      str.format(L"  Component %d Entry ID", i);
      pDumper->writeFieldValueHex( str, cmpInfo.getComponentEntryId() );
    }
    else
    {
      str.format(L"  Component %d Handle ID", i);
      pDumper->writeFieldValueHex( str, cmpInfo.getComponentHandleId() );
    }
  }
}

void OdDgLineCodeResourceDumperPE::dump( OdRxObjectPtr pObj, OdExDgnDumper* pDumper ) const
{
  OdDgRxObjectDumperPE::dump(pObj, pDumper);

  OdDgLineCodeResourcePtr pRes = pObj;

  pDumper->writeFieldValue( L"Description", pRes->getDescription() );
  pDumper->writeFieldValue( L"Phase", pRes->getPhase() );
  pDumper->writeFieldValue( L"Auto Phase Flag", pRes->getAutoPhaseFlag() );
  pDumper->writeFieldValue( L"Single Segment Mode Flag", pRes->getSingleSegmentModeFlag() );
  pDumper->writeFieldValue( L"Center Stretch Phase Mode Flag", pRes->getCenterStretchPhaseModeFlag() );
  pDumper->writeFieldValue( L"Use Iteration Limit Flag", pRes->getUseIterationLimitFlag() );
  pDumper->writeFieldValue( L"Max Iterations", pRes->getMaxIterations() );

  OdDgLineCodeResourceStrokeData strokeData;

  for ( unsigned i = 0; i < pRes->getStrokeCount(); ++i )
  {
    pRes->getStroke(i, strokeData);

    pDumper->writeFieldValue( L"Stroke (index)", (OdUInt32)i );
    pDumper->writeFieldValue( L"  Length", strokeData.getLength() );
    pDumper->writeFieldValue( L"  Width", strokeData.getStartWidth() );
    pDumper->writeFieldValue( L"  EndWidth", strokeData.getEndWidth() );
    pDumper->writeFieldValue( L"  Dash Flag", strokeData.getDashFlag() );
    pDumper->writeFieldValue( L"  ByPass Corner Flag", strokeData.getByPassCornerFlag() );
    pDumper->writeFieldValue( L"  Can Be Scaled Flag", strokeData.getCanBeScaledFlag() );
    pDumper->writeFieldValue( L"  Invert Stroke In First Code Flag", strokeData.getInvertStrokeInFirstCodeFlag() );
    pDumper->writeFieldValue( L"  Invert Stroke In Last Code", strokeData.getInvertStrokeInLastCodeFlag() );
    pDumper->writeFieldValue( L"  Width Mode", strokeData.getWidthMode() );
    pDumper->writeFieldValue( L"  Decreasing Taper Flag", strokeData.getDecreasingTaperFlag() );
    pDumper->writeFieldValue( L"  Increasing Taper Flag", strokeData.getIncreasingTaperFlag() );
    pDumper->writeFieldValue( L"  Caps Type", strokeData.getCapsType() );
  }
}

void OdDgLinePointResourceDumperPE::dump( OdRxObjectPtr pObj, OdExDgnDumper* pDumper ) const
{
  OdDgRxObjectDumperPE::dump(pObj, pDumper);

  OdDgLinePointResourcePtr pRes = pObj;

  pDumper->writeFieldValue( L"Description", pRes->getDescription() );

  pDumper->writeFieldValue( L"Base Pattern Type", pRes->getBasePatternType() );

  if( pRes->getBasePatternHandleId() == 0 )
  {
    pDumper->writeFieldValue( L"Base Pattern Entry Id", pRes->getBasePatternEntryId() );
  }
  else
  {
    pDumper->writeFieldValueHex( L"Base Pattern Handle Id", pRes->getBasePatternHandleId() );
  }

  for ( unsigned i = 0; i < pRes->getSymbolCount(); ++i )
  {
    pDumper->writeFieldValue( L"Symbol (index)", (OdUInt32)i );

    OdDgLinePointResourceSymInfo symInfo;
    pRes->getSymbol( i, symInfo );
    pDumper->writeFieldValue( L"  symType", symInfo.getSymbolType() );

    if( symInfo.getPointSymbolHandleId() == 0 )
    {
      pDumper->writeFieldValue( L"  symEntryID", symInfo.getPointSymbolEntryId() );
    }
    else
    {
      pDumper->writeFieldValueHex( L"  symID", symInfo.getPointSymbolHandleId() );
    }

    pDumper->writeFieldValue( L"  strokeNo", symInfo.getSymbolStrokeNo() );
    pDumper->writeFieldValue( L"  xOffset", symInfo.getOffset().x );
    pDumper->writeFieldValue( L"  yOffset", symInfo.getOffset().y );
    pDumper->writeFieldValue( L"  zAngle", symInfo.getRotationAngle() );
    pDumper->writeFieldValue( L"  Symbol At Element Origin Flag", symInfo.getSymbolAtElementOriginFlag() );
    pDumper->writeFieldValue( L"  Symbol At Element End Flag", symInfo.getSymbolAtElementEndFlag() );
    pDumper->writeFieldValue( L"  Symbol At Each Vertex Flag", symInfo.getSymbolAtEachVertexFlag() );
    pDumper->writeFieldValue( L"  Absolute Rotation Angle Flag", symInfo.getAbsoluteRotationAngleFlag() );
    pDumper->writeFieldValue( L"  Partial Origin Beyond End Flag", symInfo.getPartialOriginBeyondEndFlag() );
    pDumper->writeFieldValue( L"  No Partial Stroke Flag", symInfo.getNoPartialStrokesFlag() );
    pDumper->writeFieldValue( L"  Mirror Symbol Flag", symInfo.getMirrorSymbolForReversedLinesFlag() );
    pDumper->writeFieldValue( L"  Do Not Scale Element Flag", symInfo.getDoNotScaleElementFlag() );
    pDumper->writeFieldValue( L"  Do Not Clip Element Flag", symInfo.getDoNotClipElementFlag() );
    pDumper->writeFieldValue( L"  Symbol Position On Stroke", symInfo.getSymbolPosOnStroke() );
    pDumper->writeFieldValue( L"  Use Symbol Color", symInfo.getUseSymbolColorFlag() );
    pDumper->writeFieldValue( L"  Use Symbol Weight", symInfo.getUseSymbolWeightFlag() );
  }
}

void OdDgPointSymbolResourceDumperPE::dump( OdRxObjectPtr pObj, OdExDgnDumper* pDumper ) const
{
  OdDgRxObjectDumperPE::dump(pObj, pDumper);

  OdDgPointSymbolResourcePtr pRes = pObj;
  pDumper->writeFieldValue( L"Description", pRes->getDescription() );
  pDumper->writeFieldValueHex( L"Symbol Handle Id", pRes->getDependedCellHeaderHandle() );
  pDumper->writeFieldValue( L"rangeLow", pRes->getSymbolExtents().minPoint() );
  pDumper->writeFieldValue( L"rangeHigh", pRes->getSymbolExtents().maxPoint() );
  pDumper->writeFieldValue( L"offset", pRes->getSymbolOffset() );
  pDumper->writeFieldValue( L"scale", pRes->getSymbolScale() );
  pDumper->writeFieldValue( L"Symbol 3D Flag", pRes->getSymbol3DFlag() );
}

void OdDgLinearAssociationDumperPE::dump( OdRxObjectPtr pObj, OdExDgnDumper* pDumper ) const
{
  OdDgRxObjectDumperPE::dump(pObj, pDumper);

  OdDgLinearAssociationPtr pAssoc = pObj;

  pDumper->writeFieldValue(L"    Association Type", pAssoc->getType() );
  pDumper->writeFieldValue(L"    Vertex Number", pAssoc->getVertexNumber() );
  pDumper->writeFieldValue(L"    Numerator", pAssoc->getNumerator() );
  pDumper->writeFieldValue(L"    Divisor", pAssoc->getDivisor() );
  pDumper->writeFieldValueHex(L"    Element Id", (OdUInt64)(pAssoc->getElementId()) );
  pDumper->writeFieldValueHex(L"    XRef Id", (OdUInt64)(pAssoc->getRefAttachmentId()) );
  pDumper->writeFieldValue(L"    N Vertices ", pAssoc->getNVertices() );
}

void OdDgIntersectAssociationDumperPE::dump( OdRxObjectPtr pObj, OdExDgnDumper* pDumper ) const
{
  OdDgRxObjectDumperPE::dump(pObj, pDumper);

  OdDgIntersectAssociationPtr pAssoc = pObj;

  pDumper->writeFieldValue(L"    Association Type", pAssoc->getType() );
  pDumper->writeFieldValue(L"    Index", pAssoc->getIndex() );
  pDumper->writeFieldValueHex(L"    Element 1 Id", (OdUInt64)(pAssoc->getElementId()) );
  pDumper->writeFieldValueHex(L"    XRef 1 Id", (OdUInt64)(pAssoc->getRefAttachmentId()) );
  pDumper->writeFieldValueHex(L"    Element 2 Id", (OdUInt64)(pAssoc->getElement2Id()) );
  pDumper->writeFieldValueHex(L"    XRef 2 Id", (OdUInt64)(pAssoc->getRefAttachment2Id()) );
}

void OdDgArcAssociationDumperPE::dump( OdRxObjectPtr pObj, OdExDgnDumper* pDumper ) const
{
  OdDgRxObjectDumperPE::dump(pObj, pDumper);

  OdDgArcAssociationPtr pAssoc = pObj;

  pDumper->writeFieldValue(L"    Association Type", pAssoc->getType() );
  pDumper->writeFieldValue(L"    Key Point", pAssoc->getKeyPoint() );
  pDumper->writeFieldValueHex(L"    Element Id", (OdUInt64)(pAssoc->getElementId()) );
  pDumper->writeFieldValueHex(L"    XRef Id", (OdUInt64)(pAssoc->getRefAttachmentId()) );
  pDumper->writeFieldValue(L"    Angle", pAssoc->getAngle() );
}

void OdDgMultilineAssociationDumperPE::dump( OdRxObjectPtr pObj, OdExDgnDumper* pDumper ) const
{
  OdDgRxObjectDumperPE::dump(pObj, pDumper);

  OdDgMultilineAssociationPtr pAssoc = pObj;

  pDumper->writeFieldValue(L"    Association Type", pAssoc->getType() );
  pDumper->writeFieldValue(L"    Vertex Number", pAssoc->getVertexNumber() );
  pDumper->writeFieldValue(L"    Line Number", pAssoc->getLineNumber() );
  pDumper->writeFieldValue(L"    Joint Flag", pAssoc->getJointFlag() );
  pDumper->writeFieldValue(L"    Project Flag", pAssoc->getProjectFlag() );
  pDumper->writeFieldValueHex(L"    Element Id", (OdUInt64)(pAssoc->getElementId()) );
  pDumper->writeFieldValueHex(L"    XRef Id", (OdUInt64)(pAssoc->getRefAttachmentId()) );
  pDumper->writeFieldValue(L"    Offset", pAssoc->getOffset() );
  pDumper->writeFieldValue(L"    N Vertices ", pAssoc->getNVertices() );
}

void OdDgBSplineCurveAssociationDumperPE::dump( OdRxObjectPtr pObj, OdExDgnDumper* pDumper ) const
{
  OdDgRxObjectDumperPE::dump(pObj, pDumper);

  OdDgBSplineCurveAssociationPtr pAssoc = pObj;

  pDumper->writeFieldValue(L"    Association Type", pAssoc->getType() );
  pDumper->writeFieldValueHex(L"    Element Id", (OdUInt64)(pAssoc->getElementId()) );
  pDumper->writeFieldValueHex(L"    XRef Id", (OdUInt64)(pAssoc->getRefAttachmentId()) );
  pDumper->writeFieldValue(L"    U Param", pAssoc->getParam() );
}

void OdDgProjectionAssociationDumperPE::dump( OdRxObjectPtr pObj, OdExDgnDumper* pDumper ) const
{
  OdDgRxObjectDumperPE::dump(pObj, pDumper);

  OdDgProjectionAssociationPtr pAssoc = pObj;

  pDumper->writeFieldValue(L"    Association Type", pAssoc->getType() );
  pDumper->writeFieldValue(L"    Vertex Number", pAssoc->getVertexNumber() );
  pDumper->writeFieldValue(L"    N Vertices ", pAssoc->getNVertices() );
  pDumper->writeFieldValueHex(L"    Element Id", (OdUInt64)(pAssoc->getElementId()) );
  pDumper->writeFieldValueHex(L"    XRef Id", (OdUInt64)(pAssoc->getRefAttachmentId()) );
  pDumper->writeFieldValue(L"    Fraction", pAssoc->getFraction() );
}

void OdDgOriginAssociationDumperPE::dump( OdRxObjectPtr pObj, OdExDgnDumper* pDumper ) const
{
  OdDgRxObjectDumperPE::dump(pObj, pDumper);

  OdDgOriginAssociationPtr pAssoc = pObj;

  pDumper->writeFieldValue(L"    Association Type", pAssoc->getType() );
  pDumper->writeFieldValue(L"    Origin Option", pAssoc->getTextOriginOption() );
  pDumper->writeFieldValueHex(L"    Element Id", (OdUInt64)(pAssoc->getElementId()) );
  pDumper->writeFieldValueHex(L"    XRef Id", (OdUInt64)(pAssoc->getRefAttachmentId()) );
}

void OdDgIntersect2AssociationDumperPE::dump( OdRxObjectPtr pObj, OdExDgnDumper* pDumper ) const
{
  OdDgRxObjectDumperPE::dump(pObj, pDumper);

  OdDgIntersect2AssociationPtr pAssoc = pObj;

  pDumper->writeFieldValue(L"    Association Type", pAssoc->getType() );
  pDumper->writeFieldValue(L"    N Seg 1", pAssoc->getNSeg1() );
  pDumper->writeFieldValue(L"    Index", pAssoc->getIndex() );
  pDumper->writeFieldValue(L"    N Seg 2", pAssoc->getNSeg2() );
  pDumper->writeFieldValue(L"    Segment 1", pAssoc->getSegment1() );
  pDumper->writeFieldValue(L"    Segment 2", pAssoc->getSegment2() );
  pDumper->writeFieldValueHex(L"    Element 1 Id", (OdUInt64)(pAssoc->getElementId()) );
  pDumper->writeFieldValueHex(L"    XRef 1 Id", (OdUInt64)(pAssoc->getRefAttachmentId()) );
  pDumper->writeFieldValueHex(L"    Element 2 Id", (OdUInt64)(pAssoc->getElement2Id()) );
  pDumper->writeFieldValueHex(L"    XRef 2 Id", (OdUInt64)(pAssoc->getRefAttachment2Id()) );
}

void OdDgMeshVertexAssociationDumperPE::dump( OdRxObjectPtr pObj, OdExDgnDumper* pDumper ) const
{
  OdDgRxObjectDumperPE::dump(pObj, pDumper);

  OdDgMeshVertexAssociationPtr pAssoc = pObj;

  pDumper->writeFieldValue(L"    Association Type", pAssoc->getType() );
  pDumper->writeFieldValue(L"    Vertex Index", pAssoc->getVertexIndex() );
  pDumper->writeFieldValueHex(L"    Element Id", (OdUInt64)(pAssoc->getElementId()) );
  pDumper->writeFieldValueHex(L"    XRef Id", (OdUInt64)(pAssoc->getRefAttachmentId()) );
  pDumper->writeFieldValue(L"    N Vertices", pAssoc->getNVertices() );
}

void OdDgMeshEdgeAssociationDumperPE::dump( OdRxObjectPtr pObj, OdExDgnDumper* pDumper ) const
{
  OdDgRxObjectDumperPE::dump(pObj, pDumper);

  OdDgMeshEdgeAssociationPtr pAssoc = pObj;

  pDumper->writeFieldValue(L"    Association Type", pAssoc->getType() );
  pDumper->writeFieldValue(L"    Edge Index", pAssoc->getEdgeIndex() );
  pDumper->writeFieldValueHex(L"    Element Id", (OdUInt64)(pAssoc->getElementId()) );
  pDumper->writeFieldValueHex(L"    XRef Id", (OdUInt64)(pAssoc->getRefAttachmentId()) );
  pDumper->writeFieldValue(L"    U Param", pAssoc->getUParam() );
  pDumper->writeFieldValue(L"    N Edges", pAssoc->getNEdges() );
}

void OdDgBSplineSurfaceAssociationDumperPE::dump( OdRxObjectPtr pObj, OdExDgnDumper* pDumper ) const
{
  OdDgRxObjectDumperPE::dump(pObj, pDumper);

  OdDgBSplineSurfaceAssociationPtr pAssoc = pObj;

  pDumper->writeFieldValue(L"    Association Type", pAssoc->getType() );
  pDumper->writeFieldValueHex(L"    Element Id", (OdUInt64)(pAssoc->getElementId()) );
  pDumper->writeFieldValueHex(L"    XRef Id", (OdUInt64)(pAssoc->getRefAttachmentId()) );
  pDumper->writeFieldValue(L"    U Param", pAssoc->getUParam() );
  pDumper->writeFieldValue(L"    V Param", pAssoc->getVParam() );
}

void OdDgUnknownAssociationDumperPE::dump( OdRxObjectPtr pObj, OdExDgnDumper* pDumper ) const
{
  OdDgRxObjectDumperPE::dump(pObj, pDumper);

  OdDgUnknownAssociationPtr pAssoc = pObj;

  pDumper->writeFieldValue(L"    Association Type", pAssoc->getType() );
  pDumper->writeFieldValueHex(L"    Element Id", (OdUInt64)(pAssoc->getElementId()) );
  pDumper->writeFieldValueHex(L"    XRef Id", (OdUInt64)(pAssoc->getRefAttachmentId()) );
}


