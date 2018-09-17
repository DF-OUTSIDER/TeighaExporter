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
#include <DbDatabase.h>
#include <DbBlockTableRecord.h>
#include "DgnImportImpl.h"
#include "DgnImportDimensions.h"
#include "DgDatabase.h"
#include "DgDimension.h"
#include "DbRotatedDimension.h"
#include "DgDimDefs.h"
#include "DgnImportCommon.h"
#include "DgnImportContext.h"
#include "Gi/GiTextStyle.h"
#include "DgTextStyleTableRecord.h"
#include "DgTable.h"
#include "DgFontTableRecord.h"
#include "DgAttributeLinkage.h"
#include <DbBlockTable.h>
#include <DbText.h>
#include "DbDimStyleTableRecord.h"
#include "Db3PointAngularDimension.h"
#include "DbArcDimension.h"
#include "DbRadialDimension.h"
#include "DbDiametricDimension.h"
#include "DbOrdinateDimension.h"
#include "DbBlockReference.h"
#include "DbMLeader.h"
#include "DgTextNode.h"
#include "DgText.h"
#include "DgLine.h"
#include <DbHatch.h>
#include <DbLine.h>
#include <DbArc.h>
#include <DbMPolygon.h>
#include <DbCircle.h>
#include "DbDictionary.h"

using namespace TD_DGN_IMPORT;

namespace TD_DGN_IMPORT 
{
//-----------------------------------------------------------------------------------------------

  extern void importDimensionSegment(OdDgElement* e, OdUInt32 uSegmentNum, const OdGeVector3d& vrNormal, OdDbBlockTableRecord* owner);

//-----------------------------------------------------------------------------------------------

  typedef std::map<OdUInt32, OdDgAssocPointRootPtr> OdDgnImportDgDimDependencyMap;

//-----------------------------------------------------------------------------------------------

  bool createDgnDimDependencyMap( OdDgDimension* pDim, OdDgnImportDgDimDependencyMap& dependencyMap )
  {
    bool bRet = false;

    OdRxObjectPtrArray arrLinkages;

    pDim->getLinkages( OdDgAttributeLinkage::kDependency, arrLinkages );

    for( OdUInt32 i = 0; i < arrLinkages.size(); i++ )
    {
      if( arrLinkages[i]->isKindOf( OdDgDepLinkageAssocPointI::desc()) )
      {
        OdDgDepLinkageAssocPointIPtr pAssocLinkage = arrLinkages[i];

        if( !pAssocLinkage.isNull() )
        {
          OdUInt32 nRoots = pAssocLinkage->getCount();

          for( OdUInt32 j = 0; j < nRoots; j++ )
          {
            OdDgAssocPointIData rootData = pAssocLinkage->getAt(j);
            dependencyMap[rootData.m_iParam1] = rootData.m_assocPointData;
            bRet = true;
          }
        }
      }
    }

    return bRet;
  }

//-----------------------------------------------------------------------------------------------

  void getDimensionViewOptionData(OdDgDimension* pDim, OdDgDimRotation& quatView)
  {
    OdDgDimOptionView* pViewOption;
    OdDgDimOptionPtr pOption = pDim->getOption( OdDgDimOption::kView );
    pViewOption = dynamic_cast< OdDgDimOptionView*>(pOption.get());

    if( pViewOption != NULL )
    {
      OdGeQuaternion quat = pViewOption->getQuaternion();

      if( pDim->Is3D() )
        quat.w *= -1;

      quatView.create( quat, pDim->Is3D());
    }
  }

//

  void convertAccuracyToDwgFormat( OdDgDimTextFormat::Accuracy dgnAccur, OdUInt8& uDwgUnitsType, OdUInt8& uPresission )
  {
    OdDgnImportContext::convertAccuracyToDwgFormat( dgnAccur, uDwgUnitsType, uPresission );
  }

//------------------------------------------------------------------------------------------------

  void OdDgnImportContext::convertAccuracyToDwgFormat( OdDgDimTextFormat::Accuracy dgnAccur, OdUInt8& uDwgUnitsType, OdUInt8& uPresission )
  {
    switch( dgnAccur )
    {
      case OdDgDimTextFormat::kDecimal1:
      {
        uDwgUnitsType = 2;
        uPresission   = 1;
      } break;

      case OdDgDimTextFormat::kDecimal2:
      {
        uDwgUnitsType = 2;
        uPresission   = 2;
      } break;

      case OdDgDimTextFormat::kDecimal3:
      {
        uDwgUnitsType = 2;
        uPresission   = 3;
      } break;

      case OdDgDimTextFormat::kDecimal4:
      {
        uDwgUnitsType = 2;
        uPresission   = 4;
      } break;

      case OdDgDimTextFormat::kDecimal5:
      {
        uDwgUnitsType = 2;
        uPresission   = 5;
      } break;

      case OdDgDimTextFormat::kDecimal6:
      {
        uDwgUnitsType = 2;
        uPresission   = 6;
      } break;

      case OdDgDimTextFormat::kDecimal7:
      {
        uDwgUnitsType = 2;
        uPresission   = 7;
      } break;

      case OdDgDimTextFormat::kDecimal8:
      {
        uDwgUnitsType = 2;
        uPresission   = 8;
      } break;

      case OdDgDimTextFormat::kExponential1:
      {
        uDwgUnitsType = 1;
        uPresission   = 1;
      } break;

      case OdDgDimTextFormat::kExponential2:
      {
        uDwgUnitsType = 1;
        uPresission   = 2;
      } break;

      case OdDgDimTextFormat::kExponential3:
      {
        uDwgUnitsType = 1;
        uPresission   = 3;
      } break;

      case OdDgDimTextFormat::kExponential4:
      {
        uDwgUnitsType = 1;
        uPresission   = 4;
      } break;

      case OdDgDimTextFormat::kExponential5:
      {
        uDwgUnitsType = 1;
        uPresission   = 5;
      } break;

      case OdDgDimTextFormat::kExponential6:
      {
        uDwgUnitsType = 1;
        uPresission   = 6;
      } break;

      case OdDgDimTextFormat::kExponential7:
      {
        uDwgUnitsType = 1;
        uPresission   = 7;
      } break;

      case OdDgDimTextFormat::kExponential8:
      {
        uDwgUnitsType = 1;
        uPresission   = 8;
      } break;

      case OdDgDimTextFormat::kFractional2:
      {
        uDwgUnitsType = 5;
        uPresission   = 1;
      } break;

      case OdDgDimTextFormat::kFractional4:
      {
        uDwgUnitsType = 5;
        uPresission   = 2;
      } break;

      case OdDgDimTextFormat::kFractional8:
      {
        uDwgUnitsType = 5;
        uPresission   = 3;
      } break;

      case OdDgDimTextFormat::kFractional16:
      {
        uDwgUnitsType = 5;
        uPresission   = 4;
      } break;

      case OdDgDimTextFormat::kFractional32:
      {
        uDwgUnitsType = 5;
        uPresission   = 5;
      } break;

      case OdDgDimTextFormat::kFractional64:
      {
        uDwgUnitsType = 5;
        uPresission   = 6;
      } break;

      default:
      {
        uDwgUnitsType = 2;
        uPresission   = 0;
      } break;
    }
  }

//------------------------------------------------------------------------------------------------

  OdString getToolSymbolString( OdDgDimTool::CustomSymbol toolSymbol )
  {
    OdString strRet = OdString::kEmpty;

    switch( toolSymbol )
    {
      case OdDgDimTool::kCsDiameter:
      {
        strRet = L"%%c";
      } break;

      case OdDgDimTool::kCsRadius:
      {
        strRet = L"R";
      } break;

      case OdDgDimTool::kCsSDiameter:
      {
        strRet = L"S%%c";
      } break;

      case OdDgDimTool::kCsSR:
      {
        strRet = L"SR";
      } break;
    }

    return strRet;
  }

//------------------------------------------------------------------------------------------------

OdDbObjectId createTextStyleFromDimension(OdDgDimension* pDim, OdDbDatabase* pDb )
{
  OdDbObjectId idRet;

  OdGiTextStyle dimTextStyle;

  OdDgFontTableRecordPtr pFontRec;

  OdDgTextStyleTableRecordPtr pRec;

  OdDgTextStyleTableRecordPtr pTextStyle;

  if( pDim->getTextStyleEntryId() > 0 )
  {
    OdDgTextStyleTablePtr pTextStyleTable = pDim->database()->getTextStyleTable(OdDg::kForRead);

    if( !pTextStyleTable.isNull() )
    {
      OdDgElementId idTextStyle = pTextStyleTable->getAt( pDim->getTextStyleEntryId() );

      if( !idTextStyle.isNull() )
      {
        pTextStyle = idTextStyle.openObject(OdDg::kForRead);
      }
    }
  }

  if( pTextStyle.isNull() )
    pRec = pDim->database()->getTextStyleTable()->getDefaultData();
  else
    pRec = pTextStyle;

  OdDgFontTablePtr pFontTable =  pDim->database()->getFontTable();

  OdDgDimTextInfo txtInfo;
  pDim->getDimTextInfo(txtInfo);

  if( !pFontTable.isNull() )
    pFontRec = pFontTable->getFont( txtInfo.getFontEntryId() );

  if( pFontRec.isNull() )
  {
    pFontRec = pFontTable->getFont( pDim->database()->appServices()->getFAST_FONT() );
  }

  pRec->getGiTextStyle(dimTextStyle);
  dimTextStyle.setTextSize( txtInfo.getHeight() );
  dimTextStyle.setXScale( txtInfo.getWidth()/txtInfo.getHeight() );
  dimTextStyle.setUnderlined( pDim->getUnderlineTextFlag() );

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

    dimTextStyle.loadStyleRec( pDim->database() );
  }

  OdDgDimensionExportPEPtr pDgDimPE =  OdDgDimensionExportPEPtr(OdRxObjectPtr(pDim));

  if( (pFontRec->getType() != kFontTypeTrueType) && !pDgDimPE.isNull() && (pDgDimPE->getTextType( pDim ) == OdDgDimTool::kVertical) )
  {
    dimTextStyle.setVertical(true);
  }
  else
  {
    dimTextStyle.setVertical(false);
  }

  idRet = OdDgnImportContext::getStyleForDbText(pDb, dimTextStyle );

  if( idRet.isNull() )
  {
    idRet = OdDgnImportContext::createStyleForDbText(pDb, pDim->database(), dimTextStyle );
  }

  return idRet;
}

//------------------------------------------------------------------------------------------------

OdString getDgnFontName( OdDgDatabase* pDgnDb, OdUInt32 uFontEntryId, bool& bTTF )
{
  OdString strRet;
  OdDgFontTablePtr pFontTable = pDgnDb->getFontTable();

  OdDgFontTableRecordPtr pFont = pFontTable->getFont( uFontEntryId );

  if( pFont.isNull() )
  {
    pFont = pFontTable->getFont( pDgnDb->appServices()->getFAST_FONT() );
  }

  if( !pFont.isNull() )
  {
    if( pFont->getType() == kFontTypeTrueType )
    {
      bTTF = true;
    }
    else
    {
      bTTF = false;
    }

    strRet = pFont.get()->getName();
  }

  return strRet;
}

//------------------------------------------------------------------------------------------------

struct OdDgnImportDimensionMTextData
{
  double    m_dHeight;
  OdString  m_strFontName;
  OdCmColor m_clrColor;
  bool      m_bBold;
  bool      m_bItalic;
};

OdString createDimensionMTextItem(const OdDgDimensionTextItemLinkagePtr& pDimText,
                                    const OdCmColor& clrDefaultTextColor,
                                      double dDefaultTextHeight,
                                        OdUInt32 uDefaultFontEntryId,
                                          double dUORToMasterScaleFactor,
                                            OdDgDatabase* pDgnDb,
                                              OdDgnImportDimensionMTextData& curMTextData,
                                                bool bInitMTextData
                                  )
{
  OdString strRet;

  if( bInitMTextData )
  {
    bool bTTF                   = false;
    curMTextData.m_bBold        = false;
    curMTextData.m_bItalic      = false;
    curMTextData.m_clrColor     = clrDefaultTextColor;
    curMTextData.m_dHeight      = dDefaultTextHeight;
    curMTextData.m_strFontName  = getDgnFontName( pDgnDb, uDefaultFontEntryId, bTTF);
  }

  if( pDimText->getColorFlag() )
  {
    OdCmColor clrNewColor;
    setDwgColorByDgnIndex( pDgnDb, clrNewColor, pDimText->getColorIndex() );

    if( clrNewColor != curMTextData.m_clrColor )
    {
      OdString strColor;

      if( clrNewColor.colorMethod() == OdCmEntityColor::kByColor )
      {
        strColor.format(L"\\c%d;", ODRGB(clrNewColor.red(), clrNewColor.green(), clrNewColor.blue() ) );
      }
      else
      {
        strColor.format(L"\\C%d;", clrNewColor.colorIndex() );
      }

      strRet += strColor;

      curMTextData.m_clrColor = clrNewColor;
    }
  }

  OdString strDgnFontName = curMTextData.m_strFontName;
  bool bTTF = false;
  
  if( pDimText->getFontEntryId() != 0 )
  {
    strDgnFontName = getDgnFontName( pDgnDb, uDefaultFontEntryId, bTTF);
  }

  OdString strFont;

  if( strDgnFontName != curMTextData.m_strFontName )
  {
    if( bTTF )
    {
      strFont += L"\\f" + strDgnFontName;

      if( pDimText->getBoldFlag() )
      {
        strFont += L"|b1";
      }
      else
      {
        strFont += L"|b0";
      }

      if( pDimText->getSlantFlag() )
      {
        strFont += L"|i1";
      }
      else
      {
        strFont += L"|i0";
      }

      OdUInt32 uCharset = codePageToCharset(pDimText->getCodePage());

      OdString strCharset;
      strCharset.format(L"|c%03d", uCharset );

      strFont += strCharset;
      strFont += L"|p0;";
    }
    else
    {
      strFont += L"\\F" + strDgnFontName + L".shx;";
    }
  }
  else if( bTTF )
  {
    bool bAddFontName = false;

    if( curMTextData.m_bBold != pDimText->getBoldFlag() )
    {
      bAddFontName = true;

      strFont += L"\\f" + strDgnFontName;

      if( pDimText->getBoldFlag() )
      {
        strFont += L"|b1";
      }
      else
      {
        strFont += L"|b0";
      }
    }

    if( curMTextData.m_bItalic != pDimText->getSlantFlag() )
    {
      if( !bAddFontName )
      {
        strFont += L"\\f" + strDgnFontName;
      }

      if( pDimText->getSlantFlag() )
      {
        strFont += L"|i1";
      }
      else
      {
        strFont += L"|i0";
      }
    }
  }

  if( !strFont.isEmpty() )
  {
    strFont += L";";
  }

  curMTextData.m_strFontName = strDgnFontName;

  if( bTTF )
  {
    curMTextData.m_bBold       = pDimText->getBoldFlag();
    curMTextData.m_bItalic     = pDimText->getSlantFlag();
  }

  strRet += strFont;

  double dCurHeight = pDimText->getHeightFactor() * dUORToMasterScaleFactor;

  if( !OdZero( dCurHeight - curMTextData.m_dHeight ) )
  {
    OdString strHeight;
    strHeight.format(L"\\H%fx;", dDefaultTextHeight / dCurHeight );
    strHeight.replace(L',',L'.');
    strRet += strHeight;

    curMTextData.m_dHeight = dCurHeight;
  }

  strRet += pDimText->getTextString();

  if( pDimText->getCrCount() != 0 )
  {
    strRet += L"\\P";
  }

  return strRet;
}

//------------------------------------------------------------------------------------------------

OdString createTextOverride( const OdDgDimPoint& ptDimStart, 
                               OdDgDimension* pDim, 
                                  OdUInt32 iPoint,
                                    const OdCmColor& clrDefaultTextColor,
                                      double dDefaultTextHeight,
                                        OdUInt32 uDefaultFontEntryId,
                                          double dUORToMasterScaleFactor
                            )
{
  OdString strRet = OdString::kEmpty;

  if( ptDimStart.getPrimaryTextFlag() )
  {
    strRet += ptDimStart.getPrimaryText();
    strRet.replace(L"*", L"<>");
  }

  if( ptDimStart.getSecondaryTextFlag() )
  {
    strRet += L"\\X" + ptDimStart.getSecondaryText();

    if( strRet.find(L"<>") == -1 )
    {
      strRet.replace(L"*", L"<>");
    }
  }

  OdString strDimensionMText = OdString::kEmpty;

  OdRxObjectPtrArray arrDimTextLinkages;

  pDim->getLinkages( OdDgAttributeLinkage::kDimension, arrDimTextLinkages );

  OdDgnImportDimensionMTextData curMTextData;
  bool                          bInitMTextData = true;

  for( OdUInt32 i = 0; i < arrDimTextLinkages.size(); i++ )
  {
    if( !arrDimTextLinkages[i]->isKindOf( OdDgDimensionTextItemLinkage::desc() ) )
    {
      continue;
    }

    OdDgDimensionTextItemLinkagePtr pDimText = arrDimTextLinkages[i];

    if( !pDimText.isNull() )
    {
      if( pDimText->getDimensionSegmentNumber() != iPoint )
      {
        continue;
      }

      strDimensionMText += createDimensionMTextItem(pDimText, clrDefaultTextColor ,dDefaultTextHeight, uDefaultFontEntryId, dUORToMasterScaleFactor, 
                                                    pDim->database(), curMTextData, bInitMTextData);
      bInitMTextData = false;
    }
  }

  if( !strDimensionMText.isEmpty() )
  {
    strDimensionMText.replace(OdString(L"*"), strRet );

    strRet = strDimensionMText;

    if( pDim->getUnderlineTextFlag() )
    {
      strRet = L"\\L" + strRet;
    }
  }
  else
  {
    if( pDim->getUnderlineTextFlag() )
    {
      strRet = L"\\L<>";
    }
  }

  return strRet;
}

//------------------------------------------------------------------------------------------------

OdDbObjectId getTerminatorCellId( OdUInt64 uDgnCellId, OdDgDatabase* pDgnDb, OdDbDatabase* pDwgDb )
{
  OdDbObjectId idRet;

  OdDgElementId idDgnCell = pDgnDb->getElementId( OdDbHandle(uDgnCellId) );

  if( !idDgnCell.isNull() )
  {
    OdDbObjectId idDwgCell = OdDgnImportContext::getObjectId( idDgnCell );

    if( !idDwgCell.isNull() )
    {
      OdDbObjectPtr pObj = idDwgCell.safeOpenObject(OdDb::kForRead);

      if( !pObj.isNull() && pObj->isKindOf( OdDbBlockTableRecord::desc() ) )
      {
        idRet = idDwgCell;
      }
    }
  }

  return idRet;
}

//------------------------------------------------------------------------------------------------

OdDbObjectId getTerminatorSymbolCellId( OdUInt32 uFontId, OdChar chSymbol, OdDgDatabase* pDgnDb, OdDbDatabase* pDwgDb )
{
  OdDbObjectId idRet;

  bool bTTF = false;

  OdString strFontName = getDgnFontName( pDgnDb, uFontId, bTTF );

  OdString strBlockName;
  strBlockName.format(L"%d", chSymbol );
  strBlockName = strFontName + strBlockName;

  OdDbBlockTablePtr pBlockTable = pDwgDb->getBlockTableId().openObject(OdDb::kForWrite);

  if( !pBlockTable.isNull() && pBlockTable->getAt( strBlockName ).isNull() )
  {
    // Add symbol to block
    OdDbBlockTableRecordPtr pBlock = OdDbBlockTableRecord::createObject();
    pBlock->setName( strBlockName );
    pBlockTable->add( pBlock );

    OdDbTextPtr pText = OdDbText::createObject();
    pBlock->appendOdDbEntity( pText );

    OdString strText;
    strText += chSymbol;

    pText->setTextString(strText);

    OdGiTextStyle textStyle;

    if( bTTF )
    {
      textStyle.setFont(strFontName, false, false, 0, 0 );
    }
    else
    {
      textStyle.setFont(strFontName, false, false, 0, 0 );
      textStyle.setFileName( strFontName );
    }

    textStyle.loadStyleRec( pDwgDb );

    OdDbObjectId idTextStyle = OdDgnImportContext::getStyleForDbText( pDwgDb, textStyle );

    if( idTextStyle.isNull() )
    {
      idTextStyle = OdDgnImportContext::createStyleForDbText( pDwgDb, pDgnDb, textStyle );
    }

    pText->setTextStyle( idTextStyle );
    pText->setPosition(OdGePoint3d(0,0,0));
    pText->setHorizontalMode(OdDb::kTextRight);
    pText->setVerticalMode(OdDb::kTextVertMid);

    idRet = pBlock->id();
  }
  else if( !pBlockTable.isNull() )
  {
    idRet = pBlockTable->getAt( strBlockName );
  }

  return idRet;
}

//------------------------------------------------------------------------------------------------

double getTerminatorCellScale( const OdDgElementId& idCell, double dTermSize )
{
  double dRet = 1.0;

  if( !idCell.isNull() )
  {
    OdDgSharedCellDefinitionPtr pCellDef = idCell.openObject( OdDg::kForRead );

    if( !pCellDef.isNull() )
    {
      OdGeExtents3d cellExt;
      pCellDef->getGeomExtents( cellExt );

      double dCellWidth  = cellExt.maxPoint().x - cellExt.minPoint().x;
      double dCellHeight = cellExt.maxPoint().y - cellExt.minPoint().y;

      double dCellSize = dCellWidth > dCellHeight ? dCellWidth : dCellHeight;

      if( !OdZero( dCellSize ) )
      {
        dRet = dTermSize / dCellSize;
      }
    }
  }

  return dRet;
}

//------------------------------------------------------------------------------------------------

OdDbObjectId getDimensionArrowBlock( OdDgDimTool::TerminatorType uTermType, 
                                       OdDgDimension::TerminatorArrowHeadType uArrowType, 
                                         OdDgDimension* pDim, 
                                           OdDbDatabase* pDb,
                                             double dUORtoMasterScaleFactor,
                                              double& dSizeScaleFactor
                                    )
{
  OdDbObjectId idRet = OdDmUtil::getArrowId(L"_OPEN", pDb);

  OdDgDimOptionTerminatorsPtr pTermOpt = pDim->getOption( OdDgDimOption::kTerminators);
  
  switch( uTermType )
  {
     case OdDgDimTool::kTtNone:
     {
       idRet = OdDmUtil::getArrowId(L"_NONE", pDb);
     } break;

     case OdDgDimTool::kTtArrow:
     {
       if( pTermOpt.isNull() || pTermOpt->getArrowTermStyle() == OdDgDimOptionTerminators::kTermDefault )
       {
          switch(uArrowType)
          {
            case OdDgDimension::kOpened:
            {
              idRet = OdDmUtil::getArrowId(L"_OPEN", pDb);
            }
            break;
   
            case OdDgDimension::kClosed:
            {
              idRet = OdDmUtil::getArrowId(L"_CLOSEDBLANK", pDb);
            }
            break;
   
            case OdDgDimension::kFilled:
            {
              idRet = OdDmUtil::getArrowId(L"", pDb);
            }
            break;
         };
       }
       else if( pTermOpt->getArrowTermStyle() == OdDgDimOptionTerminators::kTermCell ||
                pTermOpt->getArrowTermStyle() == OdDgDimOptionTerminators::kTermScaledCell )
       {
         idRet = getTerminatorCellId( pTermOpt->getArrowCellID(), pDim->database(), pDb );

         if( idRet.isNull() )
         {
           idRet = OdDmUtil::getArrowId(L"_OPEN", pDb);
         }
         else
         {
           if( pTermOpt->getArrowTermStyle() == OdDgDimOptionTerminators::kTermScaledCell )
           {
             dSizeScaleFactor = dUORtoMasterScaleFactor * pTermOpt->getSharedCellScale();
           }
           else
           {
             dSizeScaleFactor = getTerminatorCellScale( pDim->database()->getElementId(OdDbHandle(pTermOpt->getArrowCellID())), dSizeScaleFactor );
           }
         }
       }
       else
       {
         idRet = getTerminatorSymbolCellId(pTermOpt->getArrowFontID(), pTermOpt->getArrowSymbol(), pDim->database(), pDb );

         if( idRet.isNull() )
         {
           idRet = OdDmUtil::getArrowId(L"_OPEN", pDb);
         }
       }
     }
     break;
   
     case OdDgDimTool::kTtCircle:
     {
       if( pTermOpt.isNull() || pTermOpt->getOriginTermStyle() == OdDgDimOptionTerminators::kTermDefault )
       {
         idRet = OdDmUtil::getArrowId(L"_ORIGIN", pDb);
         dSizeScaleFactor *= 0.7;
       }
       else if( pTermOpt->getOriginTermStyle() == OdDgDimOptionTerminators::kTermCell ||
                pTermOpt->getOriginTermStyle() == OdDgDimOptionTerminators::kTermScaledCell
              )
       {
         idRet = getTerminatorCellId( pTermOpt->getOriginCellID(), pDim->database(), pDb );

         if( idRet.isNull() )
         {
           idRet = OdDmUtil::getArrowId(L"_ORIGIN", pDb);
           dSizeScaleFactor *= 0.7;
         }
         else
         {
           if( pTermOpt->getOriginTermStyle() == OdDgDimOptionTerminators::kTermScaledCell )
           {
             dSizeScaleFactor = dUORtoMasterScaleFactor * pTermOpt->getSharedCellScale();
           }
           else
           {
             dSizeScaleFactor = getTerminatorCellScale( pDim->database()->getElementId(OdDbHandle(pTermOpt->getOriginCellID())), dSizeScaleFactor );
           }
         }
       }
       else
       {
         idRet = getTerminatorSymbolCellId( pTermOpt->getOriginFontID(), pTermOpt->getOriginSymbol(), pDim->database(), pDb );

         if( idRet.isNull() )
         {
           idRet = OdDmUtil::getArrowId(L"_ORIGIN", pDb);
           dSizeScaleFactor *= 0.7;
         }
       }
     }
     break;

     case OdDgDimTool::kTtStroke:
     {
       if( pTermOpt.isNull() || pTermOpt->getStrokeTermStyle() == OdDgDimOptionTerminators::kTermDefault )
       {
         idRet = OdDmUtil::getArrowId(L"_OBLIQUE", pDb);
       }
       else if( pTermOpt->getStrokeTermStyle() == OdDgDimOptionTerminators::kTermCell ||
                pTermOpt->getStrokeTermStyle() == OdDgDimOptionTerminators::kTermScaledCell
              )
       {
         idRet = getTerminatorCellId( pTermOpt->getStrokeCellID(), pDim->database(), pDb );

         if( idRet.isNull() )
         {
           idRet = OdDmUtil::getArrowId(L"_OBLIQUE", pDb);
         }
         else
         {
           if( pTermOpt->getStrokeTermStyle() == OdDgDimOptionTerminators::kTermScaledCell )
           {
             dSizeScaleFactor = dUORtoMasterScaleFactor * pTermOpt->getSharedCellScale();
           }
           else
           {
             dSizeScaleFactor = getTerminatorCellScale( pDim->database()->getElementId(OdDbHandle(pTermOpt->getStrokeCellID())), dSizeScaleFactor );
           }
         }
       }
       else
       {
         idRet = getTerminatorSymbolCellId( pTermOpt->getStrokeFontID(), pTermOpt->getStrokeSymbol(), pDim->database(), pDb );

         if( idRet.isNull() )
         {
           idRet = OdDmUtil::getArrowId(L"_OBLIQUE", pDb);
         }
       }
     }
     break;

     case OdDgDimTool::kTtFilledCircle:
     {
       if( pTermOpt.isNull() || pTermOpt->getDotTermStyle() == OdDgDimOptionTerminators::kTermDefault )
       {
         dSizeScaleFactor *= 0.2;

         if( uArrowType == OdDgDimension::kFilled )
         {
           idRet = OdDmUtil::getArrowId(L"_DOT", pDb);
         }
         else
         {
           idRet = OdDmUtil::getArrowId(L"_ORIGIN", pDb);
         }
       }
       else if( pTermOpt->getDotTermStyle() == OdDgDimOptionTerminators::kTermCell ||
                pTermOpt->getDotTermStyle() == OdDgDimOptionTerminators::kTermScaledCell
              )
       {
         idRet = getTerminatorCellId( pTermOpt->getDotCellID(), pDim->database(), pDb );

         if( idRet.isNull() )
         {
           dSizeScaleFactor *= 0.2;

           if( uArrowType == OdDgDimension::kFilled )
           {
             idRet = OdDmUtil::getArrowId(L"_DOT", pDb);
           }
           else
           {
             idRet = OdDmUtil::getArrowId(L"_ORIGIN", pDb);
           }
         }
         else
         {
           if( pTermOpt->getDotTermStyle() == OdDgDimOptionTerminators::kTermScaledCell )
           {
             dSizeScaleFactor = dUORtoMasterScaleFactor * pTermOpt->getSharedCellScale();
           }
           else
           {
             dSizeScaleFactor = getTerminatorCellScale( pDim->database()->getElementId(OdDbHandle(pTermOpt->getDotCellID())), dSizeScaleFactor );
           }
         }
       }
       else
       {
         idRet = getTerminatorSymbolCellId( pTermOpt->getDotFontID(), pTermOpt->getDotSymbol(), pDim->database(), pDb );

         if( idRet.isNull() )
         {
           dSizeScaleFactor *= 0.2;

           if( uArrowType == OdDgDimension::kFilled )
           {
             idRet = OdDmUtil::getArrowId(L"_DOT", pDb);
           }
           else
           {
             idRet = OdDmUtil::getArrowId(L"_ORIGIN", pDb);
           }
         }
       }
     }
     break;
  };

  return idRet;
}

//------------------------------------------------------------------------------------------------

void createDimensionGeometryBlock( OdDgDimension* pDgDim, OdUInt32 uPointNum, const OdGeVector3d& vrNormal, 
                                   OdDbDimension* pDbDim, OdDbDatabase* pDb )
{
  if( !pDb || !pDbDim || !pDgDim )
  {
    return;
  }

  DgnImporter* pImporter = OdDgnImportContext::getDgnImporter();

  if( pImporter )
  {
    OdRxVariantValue tmpVar = (OdRxVariantValue)pImporter->properties()->getAt("RecomputeDimensionsAfterImport");

    if( tmpVar->getBool() )
    {
      return;
    }
  }

  OdDbBlockTableRecordPtr pDimGeometryBlock = OdDbBlockTableRecord::createObject();

  OdDbBlockTablePtr pBlockTable = pDb->getBlockTableId().openObject( OdDb::kForWrite );

  if( !pBlockTable.isNull() )
  {
    pDimGeometryBlock->setName( L"*D" );

    pBlockTable->add(pDimGeometryBlock);

    importDimensionSegment( pDgDim, uPointNum, vrNormal, pDimGeometryBlock );

    pDbDim->setDimBlockId( pDimGeometryBlock->id() );
  }
}

//------------------------------------------------------------------------------------------------

void getTerminatorTypes( const OdDgDimension* pDim, 
                           bool bUseFirstTerm,
                             bool bUseJointTerm,
                               OdDgDimTool::TerminatorType& leftTerm, 
                                 OdDgDimTool::TerminatorType& rightTerm )
{
  OdDgDimensionExportPEPtr pDgDimPE =  OdDgDimensionExportPEPtr(OdRxObjectPtr(pDim));

  if( pDgDimPE.isNull() )
  {
    return ;
  }

  leftTerm    = pDgDimPE->getLeftTerminator( pDim );
  rightTerm   = pDgDimPE->getRightTerminator( pDim );

  if( bUseFirstTerm )
  {
    if( pDgDimPE->getFirstTerminator( pDim ) != OdDgDimTool::kTtNone )
      leftTerm = pDgDimPE->getFirstTerminator( pDim );
  }
  else
  {
    if( pDgDimPE->getJointTerminator( pDim ) != OdDgDimTool::kTtNone )
      leftTerm = pDgDimPE->getJointTerminator( pDim );
  }

  if( bUseJointTerm )
  {
    if( pDgDimPE->getJointTerminator( pDim ) != OdDgDimTool::kTtNone )
      rightTerm = OdDgDimTool::kTtNone;
  }
}

//------------------------------------------------------------------------------------------------

void fillDimensionProperties( OdDgDimension* pDim, 
                                const OdGeVector3d& vrNormal,
                                  OdUInt32 iPoint,
                                    const OdDgDimPoint& ptDimStart,
                                      const OdDgDimPoint& ptDimEnd,
                                        const OdGePoint3d& ptTermStart,
                                          const OdGePoint3d& ptTermEnd,
                                            const OdGePoint3d& ptTextControl,
                                              const OdGeVector3d& vrDimDir,
                                                bool bFirstTerm,
                                                  bool bLastTerm,
                                                    bool bLinearDimension,
                                                      bool bIgnorePrefixSuffix,
                                                        OdDbDimension* pDbDim,
                                                          OdDbDatabase* pDb, 
                                                            OdUInt32 uDimLeaderMode = 0,
                                                              bool bOrdinateDimension = false,
                                                                bool bIgnoreExtLineSymbology = false
                                                      )
{
  OdDgDimensionExportPEPtr pDgDimPE =  OdDgDimensionExportPEPtr(OdRxObjectPtr(pDim));

  if( pDgDimPE.isNull() )
  {
    return;
  }

  OdDgDimGeometry geom;
  pDim->getDimGeometry(geom);

  OdDgDimTextFormat format;
  pDim->getDimTextFormat(format);

  OdDgDimTextInfo txtInfo;
  pDim->getDimTextInfo(txtInfo);

  // Center mark

  pDbDim->setDimcen( pDgDimPE->getCenterMarkFlag(pDim) ? geom.getCenterSize() : 0 );

  // Angular data

  pDbDim->setDimarcsym( pDgDimPE->getArcSymbolFlag( pDim ) ? 0 : 2 );

  if( !bLinearDimension )
  {
    OdUInt32 uAccur = 4;

    if( format.getPrimaryAccuracy() >= 0  && format.getPrimaryAccuracy() < 8 )
    {
      uAccur = format.getPrimaryAccuracy();
    }

    pDbDim->setDimadec( uAccur );

    if( format.getRadiansFlag() )
    {
      pDbDim->setDimaunit(3);
    }
    else if( pDim->getCentesimalFlag() )
    {
      pDbDim->setDimaunit(2);
    }
    else if( format.getAngleFormatFlag() )
    {
      pDbDim->setDimaunit(1);
    }
    else
    {
      pDbDim->setDimaunit(0);
    }

    OdUInt32 uDimAZinValue = 0;

    if( !pDim->getLeadingZerosFlag() )
    {
      uDimAZinValue += 1;
    }

    if( !pDim->getTrailingZerosFlag() )
    {
      uDimAZinValue += 2;
    }

    pDbDim->setDimazin( uDimAZinValue );
  }

  // Extension lines

  pDbDim->setDimse1( ptDimStart.getNoWitnessLineFlag() );
  pDbDim->setDimse2( ptDimEnd.getNoWitnessLineFlag() );

  // Dimension units

  bool bUseSecondaryUnits = false;

  OdDgDimOptionPtr pOptionSecUnit  = pDim->getOption( OdDgDimOption::kSecondaryUnits );
  OdDgDimOptionPtr pOptionPrimUnit = pDim->getOption( OdDgDimOption::kPrimaryUnits );

  if( !pOptionSecUnit.isNull() && pDim->getDualFlag() )
  {
    bUseSecondaryUnits = true;
  }

  // Dimension Primary units

  // 1. leading and trailing zeros

  OdUInt32 uDimZinValue = 0;

  if( !pDim->getLeadingZerosFlag() )
  {
    uDimZinValue += 4;
  }

  if( !pDim->getTrailingZerosFlag() )
  {
    uDimZinValue += 8;
  }

  // 2. Scale factor of units and subunits

  OdString strPrimaryUnitsName;
  OdString strPrimarySubUnitsName;

  OdDgModelPtr pDimensionModel;

  OdDgElementId idOwner = pDim->ownerId();

  while( !idOwner.isNull() )
  {
    OdDgElementPtr pOwner = idOwner.openObject( OdDg::kForRead );

    if( pOwner->isKindOf( OdDgModel::desc() ) )
    {
      pDimensionModel = pOwner;
      break;
    }

    idOwner = pOwner->ownerId();
  }

  double dUORtoMasterScaleFactor = 1.0;

  if( pDimensionModel.isNull() )
  {
    OdDgElementId idModel = pDim->database()->getActiveModelId();

    if( !idModel.isNull() )
    {
      pDimensionModel = idModel.openObject(OdDg::kForRead);
    }
  }

  OdDgModel::UnitDescription baseUnits;
  OdDgModel::UnitDescription masterUnits;
  OdDgModel::UnitDescription subUnits;

  baseUnits.m_numerator = 1.0;
  baseUnits.m_denominator = 1.0;

  if( !pDimensionModel.isNull() )
  {
    pDimensionModel->getMasterUnit(baseUnits);
    pDimensionModel->getSubUnit(subUnits);
    masterUnits = baseUnits;
    dUORtoMasterScaleFactor = pDimensionModel->convertUORsToWorkingUnits(1.0);
  }

  if( !pOptionPrimUnit.isNull() )
  {
    OdDgDimOptionPrimaryUnitsPtr pPrimaryUnits = pOptionPrimUnit;
    pPrimaryUnits->getMasterUnit( masterUnits );
    pPrimaryUnits->getSubUnit( subUnits );
  }

  double dPrimaryUnitsScaleFactor = pDim->getScaleFactor() * baseUnits.m_denominator / 
    baseUnits.m_numerator / masterUnits.m_denominator * masterUnits.m_numerator;

  double dSubUnitsScaleFactor = masterUnits.m_denominator / masterUnits.m_numerator / 
    subUnits.m_denominator * subUnits.m_numerator;

  // 3. Prefix and suffix for units and subunits

  OdString strPrefixSuffixMaster = OdString::kEmpty;
  OdString strPrefixSuffixSub    = OdString::kEmpty;

  OdString strToolPrefix = getToolSymbolString( pDgDimPE->getPrefix(pDim) );
  OdString strUnitPrefix;
  OdString strMasterLabel = masterUnits.m_name;
  OdString strSubLabel = subUnits.m_name;
  OdString strUnitSuffix;
  OdString strToolSuffix = getToolSymbolString( pDgDimPE->getSuffix(pDim) );

  if( !format.getPrimaryLabelsFlag() )
  {
    strMasterLabel.empty();
    strSubLabel.empty();
  }

  OdDgDimOptionPtr pOptionPrefixSuffix = pDim->getOption( OdDgDimOption::kPrefixSuffix );

  if( !pOptionPrefixSuffix.isNull() )
  {
    OdDimOptionPrefixSuffixPtr pPrimaryPrefixSuffix = pOptionPrefixSuffix;

    if( bUseSecondaryUnits )
    {
      strUnitPrefix = pPrimaryPrefixSuffix->getUpperPrefix();
      strUnitSuffix = pPrimaryPrefixSuffix->getUpperSuffix();
    }
    else
    {
      strUnitPrefix = pPrimaryPrefixSuffix->getMainPrefix();
      strUnitSuffix = pPrimaryPrefixSuffix->getMainSuffix();
    }

    if( strUnitPrefix[0] == L'\0' )
    {
      strUnitPrefix.empty();
    }

    if( strUnitSuffix[0] == L'\0' )
    {
      strUnitSuffix.empty();
    }
  }

  strPrefixSuffixMaster = strToolPrefix + strUnitPrefix + L"<>" + strMasterLabel + strUnitSuffix + strToolSuffix;
  strPrefixSuffixSub    = strSubLabel + strUnitSuffix + strToolSuffix;

  if( bUseSecondaryUnits )
  {
    strPrefixSuffixMaster += L"\\X";
    strPrefixSuffixSub += L"\\X";

    if( pDim->getUnderlineTextFlag() )
    {
      strPrefixSuffixMaster += L"\\L";
      strPrefixSuffixSub += L"\\L";
    }
  }

  if( bIgnorePrefixSuffix )
  {
    strPrefixSuffixMaster.empty();
    strPrefixSuffixSub.empty();
  }

  // 4. Accuracy

  OdUInt8 uDimUnitFormat = 2;
  OdUInt8 uDimPresission = 0;

  convertAccuracyToDwgFormat(format.getPrimaryAccuracy(), uDimUnitFormat, uDimPresission );

  pDbDim->setDimlfac(dPrimaryUnitsScaleFactor);
  pDbDim->setDimmzf( dSubUnitsScaleFactor );
  pDbDim->setDimrnd(0.0);
  pDbDim->setDimdec( uDimPresission );
  pDbDim->setDimlunit( uDimUnitFormat );
  pDbDim->setDimdsep( format.getDecimalCommaFlag() ? L',' : L'.');
  pDbDim->setDimzin( uDimZinValue );
  pDbDim->setDimpost( strPrefixSuffixMaster );
  pDbDim->setDimmzs( strPrefixSuffixSub );

  // Dimension alternate units

  pDbDim->setDimalt( bUseSecondaryUnits );

  if( bUseSecondaryUnits )
  {
    // 1. leading and trailing zeros

    uDimZinValue = 0;

    if( !pDim->getLeadingZero2Flag() )
    {
      uDimZinValue += 4;
    }

    if( !pDim->getTrailingZeros2Flag() )
    {
      uDimZinValue += 8;
    }

    // 2. Scale factor of units and subunits

    OdString strSecondaryUnitsName;
    OdString strSecondarySubUnitsName;

    if( !pOptionSecUnit.isNull() )
    {
      OdDgDimOptionSecondaryUnitsPtr pSecondaryUnits = pOptionSecUnit;
      pSecondaryUnits->getMasterUnit( masterUnits );
      pSecondaryUnits->getSubUnit( subUnits );
    }

    double dSecondaryUnitsScaleFactor = pDim->getScaleFactor() * baseUnits.m_denominator / 
      baseUnits.m_numerator / masterUnits.m_denominator * masterUnits.m_numerator;

    double dSecondarySubUnitsScaleFactor = masterUnits.m_denominator / masterUnits.m_numerator / 
      subUnits.m_denominator * subUnits.m_numerator;

    // 3. Prefix and suffix for units and subunits

    strPrefixSuffixMaster = OdString::kEmpty;
    strPrefixSuffixSub    = OdString::kEmpty;

    strMasterLabel = masterUnits.m_name;
    strSubLabel = subUnits.m_name;

    if( !format.getSecondaryLabelsFlag() )
    {
      strMasterLabel.empty();
      strSubLabel.empty();
    }

    if( !pOptionPrefixSuffix.isNull() )
    {
      OdDimOptionPrefixSuffixPtr pPrimaryPrefixSuffix = pOptionPrefixSuffix;

      strUnitPrefix = pPrimaryPrefixSuffix->getLowerPrefix();
      strUnitSuffix = pPrimaryPrefixSuffix->getLowerSuffix();

      if( strUnitPrefix[0] == L'\0' )
      {
        strUnitPrefix.empty();
      }

      if( strUnitSuffix[0] == L'\0' )
      {
        strUnitSuffix.empty();
      }
    }

    strPrefixSuffixMaster = strUnitPrefix + L"[]" + strMasterLabel + strUnitSuffix;
    strPrefixSuffixSub    = strSubLabel + strUnitSuffix;

    // 4. Accuracy

    uDimUnitFormat = 2;
    uDimPresission = 0;

    convertAccuracyToDwgFormat(format.getSecondaryAccuracy(), uDimUnitFormat, uDimPresission );

    if( uDimUnitFormat == 5 ) // Fractional (stacked)
    {
      uDimUnitFormat = 7; // Fractional
    }

    pDbDim->setDimaltf(dSecondaryUnitsScaleFactor);
    pDbDim->setDimaltmzf( dSecondarySubUnitsScaleFactor );
    pDbDim->setDimaltrnd(0.0);
    pDbDim->setDimaltd( uDimPresission );
    pDbDim->setDimaltu( uDimUnitFormat );
    pDbDim->setDimaltz( uDimZinValue );
    pDbDim->setDimapost( strPrefixSuffixMaster );
    pDbDim->setDimaltmzs( strPrefixSuffixSub );
  }

  // Dimension Stacked fraction settings

  if( pDim->getStackFractFlag() )
  {
    pDbDim->setDimfrac( 2 - (OdUInt8)(txtInfo.getStackedFractionType()) );
  }
  else
  {
    pDbDim->setDimfrac(2);
  }

  // Dimension Tolerances

  bool bDisplayTolerance = false;

  OdDgDimOptionTolerance* pTolerance;
  OdDgDimOptionPtr pOptionTol = pDim->getOption( OdDgDimOption::kTolerance );
  pTolerance = dynamic_cast< OdDgDimOptionTolerance*>(pOptionTol.get());

  if( pTolerance && pDim->getToleranceFlag() && !OdZero(pTolerance->getToleranceTextHeight()) &&  
    !OdZero(pTolerance->getToleranceTextWidth()) )
  {
    bDisplayTolerance = true;
  }

  pDbDim->setDimtol(bDisplayTolerance);

  if( bDisplayTolerance )
  {
    pDbDim->setDimlim(pDim->getTolmodeFlag());

    double dLowTol  = pTolerance->getToleranceLower();
    double dHighTol = pTolerance->getToleranceUpper();

    if( OdZero(dLowTol - dHighTol) && pTolerance->getStackEqualFlag() )
    {
      dLowTol -= 1e-9;
    }

    pDbDim->setDimtm( dLowTol );
    pDbDim->setDimtp( dHighTol );

    if( pTolerance->getStackAlign() != 0 )
    {
      pDbDim->setDimtolj( pTolerance->getStackAlign() );
    }
    else
    {
      pDbDim->setDimtolj( 1 );
    }

    if( !OdZero(pTolerance->getToleranceTextHeight()) && !OdZero(txtInfo.getHeight()) )
    {
      pDbDim->setDimtfac( pTolerance->getToleranceTextHeight() / txtInfo.getHeight() );
    }
    else
    {
      pDbDim->setDimtfac( 1.0 );
    }

    uDimUnitFormat = 2;
    uDimPresission = 0;

    OdDgDimTextFormat::Accuracy dimAccuracy = format.getPrimaryAccuracy();

    pDim->getPrimaryToleranceAccuracy( dimAccuracy ); 

    convertAccuracyToDwgFormat( dimAccuracy , uDimUnitFormat, uDimPresission );

    pDbDim->setDimtdec( uDimPresission );

    uDimUnitFormat = 2;
    uDimPresission = 0;

    pDim->getSecondaryToleranceAccuracy( dimAccuracy ); 

    convertAccuracyToDwgFormat(dimAccuracy, uDimUnitFormat, uDimPresission );

    pDbDim->setDimalttd( uDimPresission );
    pDbDim->setDimtzin(8);
  }

  // Dimension fit options.

  OdGeVector3d vrTextDir = pDgDimPE->getDimensionTextDirection(pDim, iPoint);
  OdGeVector3d vrTextYDir = vrTextDir;
  vrTextYDir.rotateBy(OdaPI2, vrNormal);

  pDbDim->setDimtofl( pDim->getJoinerFlag() );
  pDbDim->setDimsoxd(true);

  OdDgDimension::TerminatorOrientation iTermMode = pDim->getTermMode();
  OdDgDimTextInfo::FitOptions          iFitMode  = pDim->getFitOptions();

  if( iTermMode == OdDgDimension::kReversed || iTermMode == OdDgDimension::kOutside )
  {
    pDbDim->setArrowFirstIsFlipped(true);
    pDbDim->setArrowSecondIsFlipped(true);
  }
  else
  {
    pDbDim->setArrowFirstIsFlipped(false);
    pDbDim->setArrowSecondIsFlipped(false);
  }

  pDbDim->setDimatfit(3);

  switch( iFitMode )
  {
    case OdDgDimTextInfo::kTermMovesFirst:
    {
      pDbDim->setDimatfit(1);
    } break;

    case OdDgDimTextInfo::kTermReversed:
    case OdDgDimTextInfo::kTermOutside:
    {
      pDbDim->setArrowFirstIsFlipped(true);
      pDbDim->setArrowSecondIsFlipped(true);
    } break;

    case OdDgDimTextInfo::kTextInside:
    {
      pDbDim->setDimtix(true);
    } break;

    case OdDgDimTextInfo::kTextMovesFirst:
    {
      pDbDim->setDimatfit(2);
    } break;

    case OdDgDimTextInfo::kBothMoves:
    {
      pDbDim->setDimatfit(0);
    } break;

    case OdDgDimTextInfo::kSmallestMoves:
    {
      pDbDim->setDimatfit(3);
    } break;
  }

  OdDgDimOptionOffsetPtr pDimLeaderOption = pDim->getOption(OdDgDimOption::kOffset);

  if( !pDimLeaderOption.isNull() )
  {
    OdDgDimOptionOffset::ChainType iLeaderType = pDimLeaderOption->getChainType();

    if( iLeaderType == OdDgDimOptionOffset::kNone )
    {
      pDbDim->setDimtmove(2);
    }
    else
    {
      pDbDim->setDimtmove(1);
    }
  }
  else
  {
    pDbDim->setDimtmove(uDimLeaderMode);
  }


  // Dimension text

  // 1.Text height

  pDbDim->setDimtfill(0);
  pDbDim->setDimtxt( fabs(txtInfo.getHeight()) );
  pDbDim->setDimtxtdirection(false);

  // 2.Text color

  OdDgViewPtr pActiveView = OdDgnImportContext::getActiveView();

  OdCmColor textColor;

  OdUInt32 uTextColorIndex = pDim->getColorIndex();

  if( txtInfo.getUseColorFlag() )
  {
    uTextColorIndex = txtInfo.getColorIndex();
  }

  if( !pActiveView.isNull() && pActiveView->getShowLevelSymbologyFlag() )
  {
    OdUInt32 uTmp1;
    OdInt32  uTmp2;
    applyLevelSymbologyOverrides( pDim->getLevelId(), uTextColorIndex, uTmp1, uTmp2 );
  }

  setDwgColorByDgnIndex( pDim->database(), textColor, uTextColorIndex );

  pDbDim->setDimclrt( textColor );

  // 3.Text direction

  if( pDim->getHorizontalFlag() && !bOrdinateDimension )
  {
    pDbDim->setDimtih(true);
    pDbDim->setDimtoh(true);
  }
  else
  {
    pDbDim->setDimtih(false);
    pDbDim->setDimtoh(false);
  }

  // 4.Text offset and lift

  double dTextOffsetX = geom.getTextMargin();

  if( pDim->getCapsuleTextFlag() || pDim->getBoxTextFlag() )
  {
    dTextOffsetX *= -0.5;
  }

  pDbDim->setDimgap( dTextOffsetX );
  pDbDim->setDimtvp( geom.getTextLift() );

  // 5. Text alignments

  OdDgDimTextInfo::TextLocation txtLocation = pDim->getTextLocation() ;

  pDbDim->setDimtad( (OdUInt8)(txtLocation) );

  if( !bLinearDimension && pDim->getHorizontalFlag() )
  {
    txtLocation = OdDgDimTextInfo::kTextInline;
  }

  OdDgDimTextInfo::TextAlignment textAlm = ptDimEnd.getJustification();

  pDbDim->setDimjust(0);

  OdGePoint3d ptTextPosition = pDgDimPE->getDimensionTextPosition( pDim, iPoint );

  if( !bLinearDimension || (pDbDim->dimtmove() != 1) )
  {
    ptTextPosition += vrTextDir * pDgDimPE->getDimensionTextWidth( pDim, iPoint ) / 2.0;
  }

  if( (txtLocation != OdDgDimTextInfo::kTextInline) && ( pDbDim->dimtmove() != 2 ) && !bOrdinateDimension )
  {
    OdGeLine3d lineTextDir( ptTextPosition, vrTextYDir );
    OdGeLine3d lineDimDir( ptTextControl, vrDimDir );

    OdGePoint3d ptIsect;

    if( lineTextDir.intersectWith( lineDimDir, ptIsect ) )
    {
      OdGeVector3d vrTextGapOffset = ptIsect - ptTextPosition;

      if( !OdZero(vrTextGapOffset.length()) )
      {
        vrTextGapOffset.normalize();

        if( vrTextGapOffset.angleTo(vrTextYDir) < OdaPI2 )
        {
          ptTextPosition += vrTextGapOffset * (geom.getTextLift() + pDgDimPE->getDimensionTextHeight(pDim, iPoint));
        }
        else
        {
          ptTextPosition += vrTextGapOffset * geom.getTextLift();
        }
      }

      if( pDbDim->dimtmove() == 1 )
      {
        OdGeVector3d vrCheck;

        if( ptIsect.distanceTo( ptTermStart) <= ptIsect.distanceTo(ptTermEnd) )
        {
          vrCheck = ptTermStart - ptTermEnd;
        }
        else
        {
          vrCheck = ptTermEnd - ptTermStart;
        }

        if( !OdZero(vrCheck.length()) )
        {
          vrCheck.normalize();

          if( vrTextDir.angleTo( vrCheck ) > OdaPI2 )
          {
            ptTextPosition += vrTextDir * pDgDimPE->getDimensionTextWidth(pDim, iPoint);
          }
        }
      }
    }
  }

  if( (pDbDim->dimtmove() == 2) || bOrdinateDimension )
  {
    ptTextPosition += vrTextYDir * pDgDimPE->getDimensionTextHeight(pDim, iPoint) / 2.0;
  }

  if( (pDbDim->dimtmove() != 0) && !bLinearDimension && !pDim->getHorizontalFlag() )
  {
    if( !bUseSecondaryUnits )
    {
      ptTextPosition += vrTextYDir * pDgDimPE->getDimensionTextHeight( pDim, iPoint ) / 2.0;
    }

//     double dTextAngle = vrTextDir.angleTo( OdGeVector3d::kXAxis );
//     pDbDim->setTextRotation( dTextAngle );
  }

  bool bManualText = (textAlm == OdDgDimTextInfo::kManualText);

  if( bManualText )
  {
    pDbDim->useSetTextPosition();
  }
  else
  {
    pDbDim->useDefaultTextPosition();
  }

  pDbDim->setTextPosition( ptTextPosition );

  if( textAlm == OdDgDimTextInfo::kLeftText )
  {
    pDbDim->setDimjust(1);
  }
  else if( textAlm == OdDgDimTextInfo::kRightText )
  {
    pDbDim->setDimjust(2);
  }
  else if( textAlm != OdDgDimTextInfo::kManualText )
  {
    if( pDgDimPE->isTextInsideOfDimension( pDim, iPoint) )
    {
      pDbDim->setDimjust(0);
    }
    else
    { 
      bool bChangeDir = false;

      if( !bLinearDimension )
      {
        bChangeDir = true;
      }

      if( textAlm == OdDgDimTextInfo::kCenterLeftText )
      {
        pDbDim->setDimjust( bChangeDir ? 0 : 1 );
      }
      else 
      {
        pDbDim->setDimjust( bChangeDir ? 1 : 0 );
      }
    }
  }

  // 6. Text override and style

  OdDbObjectId idDimTextStyle = createTextStyleFromDimension(pDim, pDb);
  pDbDim->setDimtxsty( idDimTextStyle );

  OdString strTextOverride = createTextOverride( ptDimEnd, pDim, iPoint - 1, textColor, 
    fabs(txtInfo.getHeight()), txtInfo.getFontEntryId(), 
    dUORtoMasterScaleFactor );

  pDbDim->setDimensionText( strTextOverride );

  // Arrows and lines

  pDbDim->setDimexe( fabs(geom.getWitnessLineExtend()) );
  pDbDim->setDimexo( fabs(geom.getWitnessLineOffset()) );
  pDbDim->setDimsd1( false );
  pDbDim->setDimsd2( false );

  OdUInt32 iDimColor  = pDim->getColorIndex();
  OdUInt32 iDimWeight = pDim->getLineWeight();
  OdUInt32 iDimStyle  = pDim->getLineStyleEntryId();

  OdUInt32 iExtColor  = iDimColor;
  OdUInt32 iExtWeight = iDimWeight;
  OdUInt32 iExtStyle  = iDimStyle;

  if( ptDimEnd.getUseAltSymbologyFlag() && !bIgnoreExtLineSymbology )
  {
    iExtColor  = pDim->getAltColorIndex();
    iExtWeight = pDim->getAltLineWeight();
    iExtStyle  = pDim->getAltLineStyleEntryId();
  }

  if( !pActiveView.isNull() && pActiveView->getShowLevelSymbologyFlag() )
  {
    OdInt32 iStyle = iExtStyle;
    applyLevelSymbologyOverrides( pDim->getLevelId(), iExtColor, iExtWeight, iStyle );
    iExtStyle = iStyle;
    iStyle = iDimStyle;
    applyLevelSymbologyOverrides( pDim->getLevelId(), iDimColor, iDimWeight, iStyle );
    iDimStyle = iStyle;
  }

  OdCmColor extColor;
  setDwgColorByDgnIndex( pDim->database(), extColor, iExtColor );
  OdDb::LineWeight extWeight = odDgnImportLineweightMapping( iExtWeight );
  OdDbObjectId lineTypeId;
  setDwgLineTypeIdByDgnLineTypeIndex( pDb, pDim->database(), iExtStyle, lineTypeId );

  pDbDim->setDimclre( extColor);
  pDbDim->setDimlwe( extWeight );
  pDbDim->setDimltex1( lineTypeId );
  pDbDim->setDimltex2( lineTypeId );

  setDwgColorByDgnIndex( pDim->database(), extColor, iDimColor );
  extWeight = odDgnImportLineweightMapping( iDimWeight );
  setDwgLineTypeIdByDgnLineTypeIndex( pDb, pDim->database(), iDimStyle, lineTypeId );

  pDbDim->setDimclrd( extColor);
  pDbDim->setDimlwd( extWeight );
  pDbDim->setDimltype( lineTypeId );

  // Dimension arrow heads

  OdDgDimTool::TerminatorType leftTerm;
  OdDgDimTool::TerminatorType rightTerm;

  getTerminatorTypes( pDim, bFirstTerm, !bLastTerm, leftTerm, rightTerm );

  OdDgDimension::TerminatorArrowHeadType atype = pDim->getArrowHead();

  if( atype > 2 )
  {
    atype = (OdDgDimension::TerminatorArrowHeadType)(atype & OdDgDimension::kClosed); 
  }

  double dArrowSize  = geom.getTerminatorHeight() > geom.getTerminatorWidth() ? geom.getTerminatorHeight() : geom.getTerminatorWidth();
  double dArrowWidth = 0.0;

  double dDotLeftArrowScale = dArrowSize;
  double dDotRightArrowScale = dArrowSize;

  OdDbObjectId idLeftArrow  = getDimensionArrowBlock( leftTerm, atype, pDim, pDb, dUORtoMasterScaleFactor, dDotLeftArrowScale );
  OdDbObjectId idRightArrow = getDimensionArrowBlock( rightTerm, atype, pDim, pDb, dUORtoMasterScaleFactor, dDotRightArrowScale );

  if( dDotLeftArrowScale > dDotRightArrowScale )
  {
    dArrowSize = dDotLeftArrowScale;
  }
  else
  {
    dArrowSize = dDotRightArrowScale;
  }

  pDbDim->setDimsah(true);
  pDbDim->setDimblk1( idLeftArrow );
  pDbDim->setDimblk2( idRightArrow );
  pDbDim->setDimasz( dArrowSize );
  pDbDim->setDimtsz( dArrowWidth );
}

//------------------------------------------------------------------------------------------------

OdDbObjectId importSimpleLinearDimension( OdDgDimension* pDim, 
                                            OdDgDimPoint ptDimBase, 
                                              OdDgDimPoint ptDimStart, 
                                                OdDgDimPoint ptDimEnd,
                                                  OdDbBlockTableRecord* owner,
                                                    double dDimLineOffset,
                                                      bool bFirstItem,
                                                        bool bLastItem, 
                                                          OdUInt32 iPoint
                                 )
{
  OdDbObjectId idRet;

  OdDgDimRotation quatDim(pDim);

  OdDgDimensionExportPEPtr pDgDimPE =  OdDgDimensionExportPEPtr(OdRxObjectPtr(pDim));

  if( pDgDimPE.isNull() )
  {
    return idRet;
  }

  OdDgDimRotation quatView;

  getDimensionViewOptionData( pDim, quatView );

  OdDgDimGeometry geom;
  pDim->getDimGeometry(geom);

  double  dMinLeader    = geom.getMargin();
  double  dTermWidth    = geom.getTerminatorWidth();

  if( !geom.getUseMargin() )
    dMinLeader = dTermWidth * 2.0;

  // Prepare data to draw dimension

  OdGePoint3d ptBase  = ptDimBase.getPoint();
  OdGePoint3d ptStart = ptDimStart.getPoint();
  OdGePoint3d ptEnd   = ptDimEnd.getPoint();
  OdGeVector3d vrWitDirection = OdGeVector3d::kYAxis;
  OdGeVector3d vrDimDirection = OdGeVector3d::kXAxis;
  vrWitDirection = quatDim.rotate( vrWitDirection );
  vrDimDirection = quatDim.rotate( vrDimDirection );
  OdGeVector3d vrDimNormal = OdGeVector3d::kZAxis;
  vrDimNormal = quatDim.rotate( vrDimNormal );

  bool bPointIntoSamePlane = true;

  OdGePoint3d ptOldEnd = ptEnd;

  OdGePlane dimPlane(ptDimBase.getPoint(),vrDimDirection, vrWitDirection);
  dimPlane.project(ptStart, vrDimNormal, ptStart);
  dimPlane.project(ptEnd, vrDimNormal, ptEnd);

  if( ptEnd.distanceTo(ptOldEnd) > 1e-6 )
  {
    bPointIntoSamePlane = false;
  }

  vrWitDirection = vrWitDirection.rotateBy(pDgDimPE->getWitnessLineSlantAngle( pDim )-OdaPI2, vrDimNormal );

  // Calculate dimension line base points

  OdGePoint3d ptTermStart = ptStart + vrWitDirection * dDimLineOffset;
  OdGePoint3d ptTermEnd;

  OdGeLine3d  lineDimDir( ptTermStart, vrDimDirection );
  OdGeLine3d  lineWitDir( ptEnd, vrWitDirection );

  if( !lineDimDir.intersectWith(lineWitDir, ptTermEnd) )
  {
    ODA_ASSERT(0);
    return idRet;
  }

  // prepare text and axis data

  OdGeVector3d vrNormal = OdGeVector3d::kZAxis;

  vrNormal = quatDim.rotate( vrNormal );

  OdGeVector3d vrViewNormal = OdGeVector3d::kZAxis;

  vrViewNormal = quatView.rotate( vrViewNormal );

  if( OdZero(vrViewNormal.angleTo( vrNormal ) - OdaPI, 1e-4) )
  {
    vrNormal *= -1;
  }

  // Create dwg dimension

  OdDbRotatedDimensionPtr pDbDim = OdDbRotatedDimension::createObject();
  pDbDim->setDatabaseDefaults( owner->database() );

  copyEntityProperties(pDim,pDbDim);

  // Dimension points and rotation
  double dDimRotation = -vrDimDirection.angleTo(OdGeVector3d::kXAxis, vrNormal);

  pDbDim->setXLine1Point(ptStart);
  pDbDim->setXLine2Point(ptEnd);
  pDbDim->setDimLinePoint( ptTermEnd );
  pDbDim->setNormal( vrNormal );
  pDbDim->setRotation( dDimRotation );
  pDbDim->setOblique( vrDimDirection.angleTo(vrWitDirection, vrNormal) - OdaPI );

  fillDimensionProperties( pDim, vrNormal, iPoint, ptDimStart, ptDimEnd, ptTermStart, ptTermEnd, ptTermStart, vrDimDirection, bFirstItem, bLastItem, true, false, pDbDim, owner->database() );

  owner->appendOdDbEntity( pDbDim );

  createDimensionGeometryBlock( pDim, iPoint - 1, vrNormal, pDbDim, owner->database() );

  idRet = pDbDim->objectId();

  return idRet;
}

//-----------------------------------------------------------------------------------------------

  void importSizeArrowDimension( OdDgElement* e, OdDbBlockTableRecord* owner )
  {
    OdDgDimension* pDgDim = (OdDgDimension*)(e);

    OdDgDimensionExportPEPtr pDgDimPE =  OdDgDimensionExportPEPtr(OdRxObjectPtr(pDgDim));

    if( pDgDimPE.isNull() )
    {
      return;
    }

    OdDgnImportDgDimDependencyMap dependencyMap;

    bool bSaveAssocDimData = createDgnDimDependencyMap( pDgDim, dependencyMap );

    OdUInt32 numPts = pDgDim->getPointsCount();

    OdDgDimPoint ptDimBase, ptDimCur, ptDimEnd;

    if( numPts < 2)
    {
      return;
    }

    ptDimBase = pDgDim->getPoint( 0 );
    ptDimCur  = ptDimBase;

    double       dDimLineOffset       = 0;
    double       dDimLineStackOffset  = 0;

    bool bSkipOffset = true;

    OdDgDimTextInfo::FitOptions iFitOpt = pDgDim->getFitOptions();

    if( pDgDim->getTermMode() == OdDgDimension::kAutomatic )
    {
      if( iFitOpt != OdDgDimTextInfo::kTextInside &&
          iFitOpt != OdDgDimTextInfo::kTermInside &&
          iFitOpt != OdDgDimTextInfo::kTermOutside &&
          iFitOpt != OdDgDimTextInfo::kTermReversed
        )
      {
        bSkipOffset = false;
      }
    }

    OdDgDimGeometry geom;
    pDgDim->getDimGeometry(geom);

    OdDgDimRotation quatDim(pDgDim);

    OdDgDimTextInfo txtInfo;
    pDgDim->getDimTextInfo(txtInfo);

    OdDgDimTextFormat format;
    pDgDim->getDimTextFormat(format);

    if( OdZero(geom.getStackOffset()) )
    {
      bool bUseSecondaryUnits = false;

      OdDgDimOptionPtr pSecondaryUnits = pDgDim->getOption(OdDgDimOption::kSecondaryUnits );

      if( !pSecondaryUnits.isNull() )
      {
        bUseSecondaryUnits = true;
      }

      bool bInlineText = (pDgDim->getTextLocation() == OdDgDimTextInfo::kTextInline);

      if( pDgDim->getHorizontalFlag() )
      {
        dDimLineStackOffset = fabs(geom.getTextMargin())+ 3*txtInfo.getHeight() / 2.0;
      }
      else
      {
        dDimLineStackOffset = fabs(geom.getTextMargin())+ txtInfo.getHeight();
      }

      if( !bInlineText )
      {
        dDimLineStackOffset += txtInfo.getHeight();
      }

      if( bUseSecondaryUnits )
      {
        dDimLineStackOffset += 2*txtInfo.getHeight();
      }
    }
    else
      dDimLineStackOffset = geom.getStackOffset();

    if( pDgDim->getTermMode() == OdDgDimension::kInside || pDgDim->getTermMode() == OdDgDimension::kReversed )
      dDimLineStackOffset = 0;

    OdGeVector3d vrWitDirection = OdGeVector3d::kYAxis;
    OdGeVector3d vrDimDirection = OdGeVector3d::kXAxis;
    vrWitDirection = quatDim.rotate( vrWitDirection );
    vrDimDirection = quatDim.rotate( vrDimDirection );

    OdGePlane dimPlane(ptDimBase.getPoint(),vrDimDirection, vrWitDirection);

    OdGeVector3d vrDimNormal = OdGeVector3d::kZAxis;
    vrDimNormal = quatDim.rotate( vrDimNormal );
    vrWitDirection = vrWitDirection.rotateBy(pDgDimPE->getWitnessLineSlantAngle( pDgDim ) - OdaPI2, vrDimNormal );

    OdGeLine3d lineDimBase( ptDimBase.getPoint() + vrWitDirection * ptDimBase.getOffsetToDimLine(), vrDimDirection );
    OdGePoint3d ptBase = ptDimBase.getPoint() + vrWitDirection * ptDimBase.getOffsetToDimLine();

    bool bDimensionLeaderPresent = false;

    OdGeLine3d  lineCurWit;        
    OdGeLine3d  lineEndWit;

    double dAngle = 0;

    OdUInt32 uPoint1 = 0;
    OdUInt32 uPoint2 = 1;

    for( OdUInt32 i = 1; i < numPts; i++ )
    {
      ptDimEnd = pDgDim->getPoint(i);
      uPoint2  = i;

      OdDgDimOptionOffsetPtr pDimLeaderOption     = pDgDim->getOption(OdDgDimOption::kOffset);

      if( !pDimLeaderOption.isNull() )
      {
        bool bDimLeaderAutoMode   = pDgDimPE->getUseDimLeaderAutomaticMode(pDgDim);

        bDimensionLeaderPresent = true;

        if( !bDimLeaderAutoMode )
        {
          if( OdZero(ptDimEnd.getOffsetY()) || ptDimEnd.getJustification() != OdDgDimTextInfo::kManualText )
            bDimensionLeaderPresent = false;
        }
      }

      if( bDimensionLeaderPresent )
        bSkipOffset = true;

      OdGePoint3d ptCur = ptDimCur.getPoint();
      OdGePoint3d ptEnd = ptDimEnd.getPoint();

      dimPlane.project(ptCur, vrDimNormal, ptCur);
      dimPlane.project(ptEnd, vrDimNormal, ptEnd);

      lineCurWit.set( ptCur, vrWitDirection );    // MKU 19/03/10 - optimization after Ge changes
      lineEndWit.set( ptEnd, vrWitDirection );
      OdGePoint3d ptCurIsect;
      OdGePoint3d ptEndIsect;

      if( !lineDimBase.intersectWith(lineCurWit,ptCurIsect) )
        continue;

      if( !lineDimBase.intersectWith(lineEndWit,ptEndIsect) )
        continue;

      dDimLineOffset = ptCurIsect.distanceTo( ptCur );

      if( !OdZero(dDimLineOffset) )
      {
        OdGeVector3d vrOffsetDir = ptCurIsect - ptCur;
        vrOffsetDir.normalize();

        if( vrOffsetDir.angleTo(vrWitDirection) > OdaPI2 )
          dDimLineOffset *= -1;
      }

      if( i == 1 )
      {
        OdDgDimRotation quatView;

        getDimensionViewOptionData( pDgDim, quatView );

        OdGeVector3d vrViewXAxis = OdGeVector3d::kXAxis;
        vrViewXAxis = quatView.rotate(vrViewXAxis);

        OdGeVector3d vrDimDir = ptEndIsect - ptCurIsect;

        dAngle = vrDimDir.angleTo(vrViewXAxis);

        if( dAngle > OdaPI2)
          dAngle = OdaPI - dAngle;
      }

      OdDbObjectId idNewDim;

      if( pDgDim->isKindOf(OdDgDimSingleLocation::desc()) )
      {
        double dDimWidth = ptCurIsect.distanceTo(ptEndIsect);

        double tWidth  = pDgDimPE->getDimensionTextWidth(pDgDim, i);
        double tHeight = pDgDimPE->getDimensionTextHeight(pDgDim, i);

        double dFullTextWidth = tWidth;

        dFullTextWidth += geom.getTextMargin()*2.0;

        bool bUseStackOffset = false;

        if( pDgDim->getHorizontalFlag() && !bSkipOffset )
        {
          double tHorWidth = dFullTextWidth;

          if( !OdZero(dAngle) )
          {
            double dTextBoxWidth  = dFullTextWidth;
            double dTextBoxHeight = tHeight + geom.getTextMargin() + txtInfo.getWidth() / 2.0;
            double dCheckAngle = atan( dTextBoxHeight / dTextBoxWidth );

            if( dCheckAngle > OdaPI2)
            {
              dCheckAngle = OdaPI - dAngle;
            }

            if( dAngle > dCheckAngle )
            {
              tHorWidth = dTextBoxHeight / sin( dAngle );
            }
            else
            {
              tHorWidth = dTextBoxWidth / cos( dAngle );
            }
          }

          bUseStackOffset = ((tHorWidth > dDimWidth) && (i > 1));
        }
        else if( !bSkipOffset )
        {
          if( geom.getUseMargin() )
          {
            dFullTextWidth += 2*geom.getMargin();
          }
          else
          {
            dFullTextWidth += geom.getTerminatorWidth()*4.0;
          }

          bUseStackOffset = ((dFullTextWidth > dDimWidth) && (i > 1));
        }

        if( bUseStackOffset )
        {
          if( dDimLineStackOffset > 0 && pDgDim->getHorizontalFlag() )
          {
            double dWidthOffset = 0;

            dWidthOffset =(tWidth + geom.getTextMargin()*2)*sin(dAngle);

            if( dWidthOffset > dDimLineStackOffset )
              dDimLineStackOffset = dWidthOffset;
          }

          if( dDimLineStackOffset > 0 && tHeight > dDimLineStackOffset )
          {
            dDimLineStackOffset = tHeight + txtInfo.getHeight();
          }

          if( dDimLineOffset > 0 )
            dDimLineOffset += dDimLineStackOffset;
          else
            dDimLineOffset -= dDimLineStackOffset;
        }

        idNewDim = importSimpleLinearDimension( pDgDim, ptDimBase, ptDimCur, ptDimEnd, owner, dDimLineOffset, i == 1, i == numPts - 1, i );
      }
      else
      {
        double dDimValue = ptCurIsect.distanceTo(ptEndIsect);

        double tWidth  = pDgDimPE->getDimensionTextWidth(pDgDim, i);
        double tHeight = pDgDimPE->getDimensionTextHeight(pDgDim, i);

        double dFullTextWidth = tWidth;

        dFullTextWidth += geom.getTextMargin()*2.0;

        bool bUseStackOffset = false;

        if( pDgDim->getHorizontalFlag() && !bSkipOffset )
        {
          double tHorWidth = dFullTextWidth;

          if( !OdZero(dAngle) )
          {
            double dTextBoxWidth  = dFullTextWidth;
            double dTextBoxHeight = tHeight + geom.getTextMargin() + txtInfo.getWidth() / 2.0;
            double dCheckAngle = atan( dTextBoxHeight / dTextBoxWidth );

            if( dCheckAngle > OdaPI2)
            {
              dCheckAngle = OdaPI - dAngle;
            }

            if( dAngle > dCheckAngle )
            {
              tHorWidth = dTextBoxHeight / sin( dAngle );
            }
            else
            {
              tHorWidth = dTextBoxWidth / cos( dAngle );
            }
          }

          bUseStackOffset = ((tHorWidth > dDimValue) && (i > 1));
        }
        else if( !bSkipOffset )
        {
          if( geom.getUseMargin() )
          {
            dFullTextWidth += 2*geom.getMargin();
          }
          else
          {
            dFullTextWidth += geom.getTerminatorWidth()*4.0;
          }

          bUseStackOffset = ((dFullTextWidth > dDimValue) && (i > 1));
        }

        if( bUseStackOffset )
        {
          if( dDimLineStackOffset > 0 && pDgDim->getHorizontalFlag() )
          {
            double dWidthOffset = 0;

            dWidthOffset =(tWidth + geom.getTextMargin()*2)*sin(dAngle);

            if( dWidthOffset > dDimLineStackOffset )
              dDimLineStackOffset = dWidthOffset;
          }

          if( tHeight > dDimLineStackOffset )
          {
            dDimLineStackOffset = tHeight + txtInfo.getHeight();
          }

          if( dDimLineOffset >= 0 )
            dDimLineOffset += dDimLineStackOffset;
          else
            dDimLineOffset -= dDimLineStackOffset;
        }

        idNewDim = importSimpleLinearDimension( pDgDim, ptDimCur, ptDimCur, ptDimEnd, owner, dDimLineOffset, i == 1, i == numPts - 1, i );
      }

      if( bSaveAssocDimData && !idNewDim.isNull() )
      {
        OdDgnImportDgDimDependencyMap::iterator pPoint1Iter = dependencyMap.find( uPoint1 );
        OdDgnImportDgDimDependencyMap::iterator pPoint2Iter = dependencyMap.find( uPoint2 );

        OdDgnImportDimAssocDescriptor curDesc;

        bool bAddAssocDesc = false;

        if( pPoint1Iter != dependencyMap.end() )
        {
          curDesc.point1Desc = pPoint1Iter->second;
          bAddAssocDesc = true;
        }

        if( pPoint2Iter != dependencyMap.end() )
        {
          curDesc.point2Desc = pPoint2Iter->second;
          bAddAssocDesc = true;
        }

        if( bAddAssocDesc )
        {
          OdDgnImportContext::addDimAssocDescriptor( idNewDim, curDesc );
        }
      }

      ptDimCur = ptDimEnd;
      uPoint1 = i;
    }
  }

//-----------------------------------------------------------------------------------------------

  void importStackedLinearDimension( OdDgElement* e, OdDbBlockTableRecord* owner )
  {
    OdDgDimension* pDgDim = (OdDgDimension*)(e);

    OdDgDimensionExportPEPtr pDgDimPE =  OdDgDimensionExportPEPtr(OdRxObjectPtr(pDgDim));

    if( pDgDimPE.isNull() )
    {
      return;
    }

    OdUInt32 numPts = pDgDim->getPointsCount();

    if( numPts < 2)
      return;

    OdDgnImportDgDimDependencyMap dependencyMap;

    bool bSaveAssocDimData = createDgnDimDependencyMap( pDgDim, dependencyMap );

    OdDgDimPoint ptDimBase, ptDimEnd;
    ptDimBase = pDgDim->getPoint( 0 );
    ptDimEnd  = pDgDim->getPoint( 1 );

    double       dDimBaseLineOffset = ptDimBase.getOffsetToDimLine();
    double       dDimLineOffset     = 0;
    double       dDimLineOffsetIncrement = 0;

    OdDgDimGeometry geom;
    pDgDim->getDimGeometry(geom);

    OdDgDimTextInfo txtInfo;
    pDgDim->getDimTextInfo(txtInfo);

    if( OdZero(geom.getStackOffset()) )
      dDimLineOffsetIncrement = fabs(geom.getTextMargin())+ 2*txtInfo.getHeight();
    else
      dDimLineOffsetIncrement = geom.getStackOffset();

    double dOffset = 0;

    OdUInt32 uPoint1 = 0;
    OdUInt32 uPoint2 = 1;

    OdDbObjectId idNewDim;

    for( OdUInt32 i = 1; i < numPts; i++ )
    {
      uPoint2 = i;

      ptDimEnd = pDgDim->getPoint(i);

      dDimLineOffset = dDimBaseLineOffset + dOffset;

      idNewDim = importSimpleLinearDimension( pDgDim, ptDimBase, ptDimBase, ptDimEnd, owner, dDimLineOffset, i == 1, i == numPts - 1, i );

      double dAltIncriment = 0;

      if( pDgDim->getHorizontalFlag() )
      {
        OdDgDimRotation quatView;

        getDimensionViewOptionData( pDgDim, quatView );

        OdGeVector3d vrViewXAxis = OdGeVector3d::kXAxis;
        vrViewXAxis = quatView.rotate(vrViewXAxis);

        OdGeVector3d vrDimDir = ptDimEnd.getPoint() - ptDimBase.getPoint();

        double dAngle = vrDimDir.angleTo(vrViewXAxis);

        if( dAngle > OdaPI2)
          dAngle = OdaPI - dAngle;

        double dTextWidth = pDgDimPE->getDimensionTextWidth(pDgDim, i, true);
        dAltIncriment =(dTextWidth + geom.getTextMargin()*2)*sin(dAngle);
      }

      bool bUseSecondaryUnits = false;

      OdDgDimOptionPtr pOptionSecUnit = pDgDim->getOption( OdDgDimOption::kSecondaryUnits );

      if( !pOptionSecUnit.isNull() && pDgDim->getDualFlag() )
      {
        bUseSecondaryUnits = true;
      }

      if( OdZero(geom.getStackOffset()) && bUseSecondaryUnits )
      {
        double dTextHeight = pDgDimPE->getDimensionTextHeight(pDgDim, i);
       
        if( pDgDim->getCapsuleTextFlag() || pDgDim->getBoxTextFlag() )
          dTextHeight += txtInfo.getWidth();

        double dHeightOffset = dTextHeight + fabs(geom.getTextMargin()* 2);

        if( dAltIncriment < dHeightOffset )
          dAltIncriment = dHeightOffset;
      }

      if( OdZero(geom.getStackOffset()) && (dAltIncriment > dDimLineOffsetIncrement) )
      {
        dOffset += dAltIncriment;
      }
      else
      {
        dOffset += dDimLineOffsetIncrement;
      }

      if( bSaveAssocDimData && !idNewDim.isNull() )
      {
        OdDgnImportDgDimDependencyMap::iterator pPoint1Iter = dependencyMap.find( uPoint1 );
        OdDgnImportDgDimDependencyMap::iterator pPoint2Iter = dependencyMap.find( uPoint2 );

        OdDgnImportDimAssocDescriptor curDesc;

        bool bAddAssocDesc = false;

        if( pPoint1Iter != dependencyMap.end() )
        {
          curDesc.point1Desc = pPoint1Iter->second;
          bAddAssocDesc = true;
        }

        if( pPoint2Iter != dependencyMap.end() )
        {
          curDesc.point2Desc = pPoint2Iter->second;
          bAddAssocDesc = true;
        }

        if( bAddAssocDesc )
        {
          OdDgnImportContext::addDimAssocDescriptor( idNewDim, curDesc );
        }
      }
    }
  }

//-----------------------------------------------------------------------------------------------

  OdGePoint3d getArcCtrlPointAndDirection( const OdGePoint3d& ptArcStart, 
                                             const OdGePoint3d& ptArcEnd, 
                                               const OdGePoint3d& ptArcCenter,
                                                 const OdGeVector3d& vrNormal,
                                                   OdUInt32 iPoint,
                                                     OdDgDimension* pDim,
                                                       OdGeVector3d& vrDimDirection
                              )
  {
    OdGePoint3d ptRet = ptArcEnd;

    OdGeVector3d vrStart = ptArcStart - ptArcCenter;
    OdGeVector3d vrEnd   = ptArcEnd - ptArcCenter;

    if( !OdZero(vrStart.length()) )
    {
      vrStart.normalize();
      vrEnd.normalize();

      double angle = vrStart.angleTo(vrEnd, vrNormal); 

      if( OdZero( angle ) )
      {
        angle = Oda2PI;
      }

      double dDirAngle = angle / 2.0;

      angle /= 3;

      OdGePoint3d p1 = ptArcStart;
      OdGePoint3d p2 = ptArcEnd;
      OdGePoint3d p3 = ptArcStart;
      p1.rotateBy(angle, vrNormal, ptArcCenter);
      p2.rotateBy(-angle, vrNormal, ptArcCenter);
      p3.rotateBy(dDirAngle, vrNormal, ptArcCenter);

      vrDimDirection = p3 - ptArcCenter;

      if( !OdZero(vrDimDirection.length()))
      {
        vrDimDirection.normalize();
      }
      else
      {
        vrDimDirection = OdGeVector3d::kXAxis;
      }

      double d1 = 0;
      double d2 = 0;

      OdGePoint3d ptTextPosition;
      OdGeVector3d vrTextDir;
      OdGeVector3d vrTextYDir;

      OdDgDimensionExportPEPtr pDgDimPE =  OdDgDimensionExportPEPtr(OdRxObjectPtr(pDim));

      if( !pDgDimPE.isNull() )
      {
        ptTextPosition = pDgDimPE->getDimensionTextPosition( pDim, iPoint );
        vrTextDir      = pDgDimPE->getDimensionTextDirection( pDim, iPoint );;
        vrTextYDir     = vrTextDir;
        vrTextYDir     = vrTextYDir.rotateBy(OdaPI2, vrNormal);

        ptTextPosition += vrTextDir * pDgDimPE->getDimensionTextWidth( pDim, iPoint );
        ptTextPosition += vrTextYDir * pDgDimPE->getDimensionTextHeight( pDim, iPoint );

        d1 = ptTextPosition.distanceTo(p1);
        d2 = ptTextPosition.distanceTo(p2);
      }

      if ( d1 > d2 || OdZero(d1-d2) )
        ptRet = p1;
      else
        ptRet = p2;
    }

    return ptRet;
  }

//-----------------------------------------------------------------------------------------------

  OdDbObjectId importSimpleAngularDimension( OdDgDimension* pDim,
                                               OdUInt32 uSegmentNum, 
                                                 const OdGePoint3d& ptCenter, 
                                                   const OdDgDimPoint& ptDimStartAngle,
                                                     const OdDgDimPoint& ptDimEndAngle,
                                                       const OdGePoint3d& ptArcStart, 
                                                         const OdGePoint3d& ptArcEnd, 
                                                           const OdGeVector3d& vrDimNormal, 
                                                             const OdGePoint3d& ptStartExtLine, 
                                                               const OdGePoint3d& ptEndExtLine, 
                                                                 bool bFirstSegment,
                                                                   OdDbBlockTableRecord* owner 
                                            )
  {
    OdDbObjectId idRet;

    OdDgDimRotation quatDim(pDim);

    // prepare text and axis data

    OdGeVector3d vrNormal = OdGeVector3d::kZAxis;

    vrNormal = quatDim.rotate( vrNormal );

    // Create dwg dimension

    OdDgDimTextFormat format;
    pDim->getDimTextFormat(format);

    OdDbDimensionPtr pAngDim;

    if( format.getAngleMeasureFlag() )
    {
      OdDb3PointAngularDimensionPtr pDbDim = OdDb3PointAngularDimension::createObject();
      pDbDim->setDatabaseDefaults( owner->database() );

      copyEntityProperties(pDim,pDbDim);

      pDbDim->setXLine1Point( ptStartExtLine );
      pDbDim->setXLine2Point( ptEndExtLine );
      pDbDim->setCenterPoint( ptCenter );

      OdGeVector3d vrDimDirection;

      OdGePoint3d ptArc = getArcCtrlPointAndDirection( ptArcStart, ptArcEnd, ptCenter, vrNormal, uSegmentNum, pDim, vrDimDirection );
      pDbDim->setArcPoint( ptArc );

      pAngDim = pDbDim;

      fillDimensionProperties( pDim, vrNormal, uSegmentNum, ptDimStartAngle, ptDimEndAngle, ptArcStart, ptArcEnd, ptArcStart, vrDimDirection, bFirstSegment, false, false, true, pDbDim, owner->database() );
    }
    else
    {
      OdDbArcDimensionPtr pDbDim = OdDbArcDimension::createObject();
      pDbDim->setDatabaseDefaults( owner->database() );

      copyEntityProperties(pDim,pDbDim);

      pDbDim->setXLine1Point( ptStartExtLine );
      pDbDim->setXLine2Point( ptEndExtLine );
      pDbDim->setCenterPoint( ptCenter );

      OdGeVector3d vrDimDirection;

      OdGePoint3d ptArc = getArcCtrlPointAndDirection( ptArcStart, ptArcEnd, ptCenter, vrNormal, uSegmentNum, pDim, vrDimDirection );
      pDbDim->setArcPoint( ptArc );

      pAngDim = pDbDim;

      fillDimensionProperties( pDim, vrNormal, uSegmentNum, ptDimStartAngle, ptDimEndAngle, ptArcStart, ptArcEnd, ptArcStart, vrDimDirection, bFirstSegment, false, false, false, pDbDim, owner->database() );
    }

    owner->appendOdDbEntity( pAngDim );

    createDimensionGeometryBlock( pDim, uSegmentNum, vrNormal, pAngDim, owner->database() );

    idRet = pAngDim->objectId();

    return idRet;
  }

//-----------------------------------------------------------------------------------------------

  void importAngularDimension( OdDgElement* e, OdDbBlockTableRecord* owner )
  {
    OdDgDimension* pDgDim = (OdDgDimension*)(e);

    OdDgDimensionExportPEPtr pDgDimPE =  OdDgDimensionExportPEPtr(OdRxObjectPtr(pDgDim));

    OdUInt32      ptCnt = pDgDim->getPointsCount();

    if( (ptCnt < 3) || pDgDimPE.isNull() )
    {
      return;
    }

    OdDgnImportDgDimDependencyMap dependencyMap;
    bool bSaveAssocDimData = createDgnDimDependencyMap( pDgDim, dependencyMap );

    OdDgDimPoint  ptDimCenter, ptDimStartAngle, ptDimEndAngle;
    OdGePoint3d   vertices[4];
    ptDimCenter      = pDgDim->getPoint(0);
    ptDimStartAngle  = pDgDim->getPoint(1);

    OdDgDimRotation quatDim(pDgDim);

    OdDgDimGeometry geom;
    pDgDim->getDimGeometry(geom);

    OdDgDimTextFormat format;
    pDgDim->getDimTextFormat(format);

    OdDgDimTextInfo txtInfo;
    pDgDim->getDimTextInfo(txtInfo);

    OdDgDimRotation quatView;

    getDimensionViewOptionData( pDgDim, quatView );

    OdUInt32     ptNum = 2;
    double       dDimArcRadius = 0;
    double       dDimActualRadius = 0;
    double       dDimOffset = ptDimStartAngle.getOffsetToDimLine();
    OdGeVector3d vrNormal = OdGeVector3d::kZAxis;
    OdGeVector3d vrViewXAxis  = OdGeVector3d::kXAxis;
    OdGeVector3d vrViewYAxis  = OdGeVector3d::kYAxis;
    vrNormal     = quatDim.rotate( vrNormal );
    vrViewXAxis  = quatView.rotate( vrViewXAxis  );
    vrViewYAxis  = quatView.rotate( vrViewYAxis  );
    OdGeVector3d vrArcBegin,vrArcEnd;
    OdGePoint3d  ptArcBegin,ptArcEnd;
    OdGePoint3d  ptCenter = ptDimCenter.getPoint();
    OdGePoint3d  ptStartAngle = ptDimStartAngle.getPoint();
    OdGePoint3d  ptEndAngle;

    OdGeVector3d vrWitDirection = OdGeVector3d::kYAxis;
    OdGeVector3d vrDimDirection = OdGeVector3d::kXAxis;
    vrWitDirection = quatDim.rotate( vrWitDirection );
    vrDimDirection = quatDim.rotate( vrDimDirection );

    double       dRadiusIncriment = 0;
    bool         bStackedDim = false;

    if( pDgDimPE->getStackExtLinesFlag( pDgDim ) )
    {
      bStackedDim = true;

      if( OdZero(geom.getStackOffset()) )
      {
        if( pDgDimPE->getArcSymbolFlag( pDgDim ) )
        {
          dRadiusIncriment = fabs(4*txtInfo.getHeight());
        }
        else
        {
          dRadiusIncriment = fabs(2*txtInfo.getHeight());
        }
      }
      else
      {
        dRadiusIncriment = geom.getStackOffset();
      }
    }

    double       dSweepAngle = 0.0;
    bool         bClockwiseAngle = pDgDim->getDimExtClockwiseAngleFlag();

    OdUInt32 uCenterPt = 0;
    OdUInt32 uStartPt  = 1;
    OdUInt32 uEndPt    = 2;

    while( ptNum < ptCnt )
    {
      uEndPt = ptNum;

      ptDimEndAngle = pDgDim->getPoint(ptNum);
      ptEndAngle = ptDimEndAngle.getPoint();

      OdGeVector3d vrArcBegin(ptStartAngle - ptCenter);

      if( OdZero(vrArcBegin.length()) )
      {
        vrArcBegin = OdGeVector3d::kXAxis;
        vrArcBegin = quatDim.rotate(vrArcBegin);
      }
      else
        vrArcBegin.normalize();

      OdGeVector3d vrArcEnd(ptEndAngle - ptCenter);

      if( OdZero(vrArcEnd.length()) )
      {
        vrArcEnd = OdGeVector3d::kXAxis;
        vrArcEnd = quatDim.rotate(vrArcEnd);
      }
      else
        vrArcEnd.normalize();

      dSweepAngle = vrArcBegin.angleTo(vrArcEnd, vrNormal);

      if( ptNum == 2 ) // quaternion doesn't keep information about dimension rotation
      {
        // set the normal through the center, for all text and as axis for rotation
        vrNormal = vrArcBegin.crossProduct((dSweepAngle > OdaPI) ? -vrArcEnd : vrArcEnd);

        if( OdZero(vrNormal.length()) )
        {
          vrNormal = OdGeVector3d::kZAxis;
          vrNormal = quatDim.rotate(vrNormal);
        }
        else
          vrNormal.normalize();

        vrViewXAxis = vrViewXAxis.orthoProject(vrNormal);
        vrViewYAxis = vrViewYAxis.orthoProject(vrNormal);

        if( !OdZero(vrViewXAxis.length()) )
        {
          vrViewXAxis.normalize();

          vrViewYAxis = vrViewXAxis;
          vrViewYAxis.rotateBy(OdaPI2, vrNormal);
        }
        else
        {
          vrViewYAxis.normalize();

          vrViewXAxis = vrViewYAxis;
          vrViewXAxis.rotateBy(-OdaPI2, vrNormal);
        }

        dDimArcRadius = fabs(ptCenter.distanceTo(ptStartAngle) + dDimOffset);
      }
      else
        dDimArcRadius += dRadiusIncriment;

      OdGeVector3d vrDimNormal = vrNormal;

      if( !vrArcBegin.isParallelTo(vrArcEnd) )
      {
        vrDimNormal = vrArcBegin.crossProduct(vrArcEnd);
      }

      if( vrDimNormal.angleTo(vrNormal) > OdaPI2 )
      {
        vrDimNormal *= -1;
      }

      dDimActualRadius = ptCenter.distanceTo(ptStartAngle);

      ptArcBegin = ptCenter + vrArcBegin * dDimArcRadius;
      ptArcEnd   = ptCenter + vrArcEnd * dDimArcRadius;

      if( dDimArcRadius > ptCenter.distanceTo( ptStartAngle ) )
        vrArcBegin = -vrArcBegin;

      if( dDimArcRadius > ptCenter.distanceTo( ptEndAngle ) )
        vrArcEnd   = -vrArcEnd;

      OdGePoint3d ptStartExtLine = ptStartAngle;
      OdGePoint3d ptEndExtLine   = ptEndAngle;

      if( bClockwiseAngle )
      {
        OdGeVector3d vrTmp = vrArcBegin;
        vrArcBegin = vrArcEnd;
        vrArcEnd   = vrTmp;

        OdGePoint3d ptTmp = ptArcBegin;
        ptArcBegin = ptArcEnd;
        ptArcEnd   = ptTmp;

        ptTmp = ptEndExtLine;
        ptEndExtLine = ptStartExtLine;
        ptStartExtLine = ptTmp;

        dSweepAngle = Oda2PI - dSweepAngle;
      }

      if( pDgDim->getDimensionType() == OdDgDimension::kToolTypeArcSize && (ptCnt == 3) &&
        !format.getAngleMeasureFlag() && dSweepAngle < OdaPI && pDgDimPE->getChordAlignFlag( pDgDim ) 
        )
      {
        OdGeVector3d vrExtLinesDir = vrArcBegin;
        vrExtLinesDir = vrExtLinesDir.rotateBy( dSweepAngle/2.0, vrNormal );

        int iSign = 1;

        if( dDimOffset < 0 )
        {
          iSign = -1;
        }

        ptArcBegin = ptCenter - iSign*(vrArcBegin*dDimActualRadius + vrExtLinesDir*dDimOffset);
        ptArcEnd   = ptCenter - iSign*(vrArcEnd*dDimActualRadius + vrExtLinesDir*dDimOffset);
        ptCenter   = ptCenter - iSign*vrExtLinesDir*dDimOffset;
      }

      OdDbObjectId idNewDim = importSimpleAngularDimension( pDgDim, ptNum - 2, ptCenter, ptDimStartAngle, ptDimEndAngle, 
                                                            ptArcBegin, ptArcEnd, vrDimNormal, ptStartExtLine, 
                                                            ptEndExtLine, ptNum == 2, owner );

      ptNum++;

      if( bSaveAssocDimData && !idNewDim.isNull() )
      {
        OdDgnImportDgDimDependencyMap::iterator pPoint1Iter = dependencyMap.find( uStartPt );
        OdDgnImportDgDimDependencyMap::iterator pPoint2Iter = dependencyMap.find( uEndPt );
        OdDgnImportDgDimDependencyMap::iterator pPoint3Iter = dependencyMap.find( uCenterPt );

        OdDgnImportDimAssocDescriptor curDesc;

        bool bAddAssocDesc = false;

        if( pPoint1Iter != dependencyMap.end() )
        {
          curDesc.point1Desc = pPoint1Iter->second;
          bAddAssocDesc = true;
        }

        if( pPoint2Iter != dependencyMap.end() )
        {
          curDesc.point2Desc = pPoint2Iter->second;
          bAddAssocDesc = true;
        }

        if( pPoint3Iter != dependencyMap.end() )
        {
          curDesc.point3Desc = pPoint3Iter->second;
          bAddAssocDesc = true;
        }

        if( bAddAssocDesc )
        {
          OdDgnImportContext::addDimAssocDescriptor( idNewDim, curDesc );
        }
      }

      if( !bStackedDim )
      {
        uStartPt = uEndPt;
        ptDimStartAngle = ptDimEndAngle;
        ptStartAngle = ptEndAngle;
      }
    }
  }

//-----------------------------------------------------------------------------------------------

  void OdDgnLinearDimensionImportPE::subImportElement(OdDgElement* e, OdDbBlockTableRecord* owner)
  {
    OdDgDimension* pDgDim = (OdDgDimension*)(e);

    OdDgDimensionExportPEPtr pDgDimPE =  OdDgDimensionExportPEPtr(OdRxObjectPtr(pDgDim));

    if( !pDgDimPE.isNull() && pDgDimPE->getStackExtLinesFlag( pDgDim ) )
    {
      importStackedLinearDimension( e, owner );
    }
    else
    {
      importSizeArrowDimension( e, owner );
    }
  }

//-----------------------------------------------------------------------------------------------

  void importDimensionDiameterSpecial( OdDgDimension* pDim, 
                                         const OdGePoint3d& ptStart, 
                                           const OdGePoint3d& ptEnd, 
                                             const OdGePoint3d& ptTermStart, 
                                               const OdGePoint3d& ptTermEnd, 
                                                 const OdGeVector3d& vrNormal, 
                                                   const OdDgDimPoint& ptDimCenter, 
                                                     OdDbBlockTableRecord* owner )
  {
    // Create dwg dimension

    OdDgnImportDgDimDependencyMap dependencyMap;
    bool bSaveAssocDimData = createDgnDimDependencyMap( pDim, dependencyMap );

    OdDbRotatedDimensionPtr pDbDim = OdDbRotatedDimension::createObject();
    pDbDim->setDatabaseDefaults( owner->database() );

    copyEntityProperties(pDim, pDbDim);

    OdGeVector3d vrDimDirection = ptTermEnd - ptTermStart;

    // Dimension points and rotation
    double dDimRotation = -vrDimDirection.angleTo(OdGeVector3d::kXAxis, vrNormal);

    pDbDim->setXLine1Point(ptStart);
    pDbDim->setXLine2Point(ptEnd);
    pDbDim->setDimLinePoint( ptTermEnd );
    pDbDim->setNormal( vrNormal );
    pDbDim->setRotation( dDimRotation );

    fillDimensionProperties( pDim, vrNormal, 0, ptDimCenter, ptDimCenter, ptTermStart, ptTermEnd, ptTermStart, vrDimDirection, false, false, true, false, pDbDim, owner->database(), 0, false, true );

    owner->appendOdDbEntity( pDbDim );

    if( bSaveAssocDimData )
    {
      OdDgnImportDgDimDependencyMap::iterator pPoint1Iter = dependencyMap.find( 0 );

      OdDgnImportDimAssocDescriptor curDesc;

      bool bAddAssocDesc = false;

      if( pPoint1Iter != dependencyMap.end() )
      {
        curDesc.point1Desc = pPoint1Iter->second;
        curDesc.bRadialDiamDimension = true;
        bAddAssocDesc = true;
      }

      if( bAddAssocDesc )
      {
        OdDgnImportContext::addDimAssocDescriptor( pDbDim->objectId(), curDesc );
      }
    }

    createDimensionGeometryBlock( pDim, 0, vrNormal, pDbDim, owner->database() );
  }

//-----------------------------------------------------------------------------------------------

  void importDiameterPerpDimension( OdDgElement* e, OdDbBlockTableRecord* owner )
  {
    OdDgDimension* pDim = (OdDgDimension*)(e);
    OdDgDimensionExportPEPtr pDgDimPE =  OdDgDimensionExportPEPtr(OdRxObjectPtr(pDim));
    // Get dimension data
    OdUInt32 ptCnt                    = pDim->getPointsCount();

    if( (ptCnt < 3) || pDgDimPE.isNull() )
    {
      return;
    }

    OdDgDimPoint ptDimCenter      = pDim->getPoint(0);
    OdDgDimPoint ptDimRadius      = pDim->getPoint(1);
    OdDgDimPoint ptDimDirection   = pDim->getPoint(2);

    OdDgDimRotation quatDim(pDim);

    OdDgDimGeometry geom;
    pDim->getDimGeometry(geom);

    OdDgDimTextFormat format;
    pDim->getDimTextFormat(format);

    OdDgDimTextInfo txtInfo;
    pDim->getDimTextInfo(txtInfo);

    OdDgDimRotation quatView;

    getDimensionViewOptionData( pDim, quatView );

    OdGePoint3d  ptCenter     = ptDimCenter.getPoint();
    OdGePoint3d  ptRadius     = ptDimRadius.getPoint();
    OdGePoint3d  ptDirection  = ptDimDirection.getPoint();

    double       dRadius        = ptCenter.distanceTo(ptRadius);

    OdGeVector3d vrBaseArcNormal = OdGeVector3d::kZAxis;

    vrBaseArcNormal = quatDim.rotate(vrBaseArcNormal);

    OdGeVector3d vrDimDirection = ptDirection - ptCenter;
    vrDimDirection = vrDimDirection.orthoProject(vrBaseArcNormal);

    if( OdZero(vrDimDirection.length()) )
    {
      ODA_ASSERT(false);
      vrDimDirection = vrBaseArcNormal;
    }
    else
      vrDimDirection.normalize();

    OdGePoint3d ptStart =  ptCenter + vrDimDirection * dRadius;
    OdGePoint3d ptEnd   =  ptCenter - vrDimDirection * dRadius;
    OdGeVector3d vrCenterDirection = ptDirection - ptCenter;
    double dBaseDirLength = vrCenterDirection.length();

    if( OdZero(dBaseDirLength) )
    {
      ODA_ASSERT(false);
      vrCenterDirection = OdGeVector3d::kXAxis;
    }
    else
      vrCenterDirection.normalize();

    double dYAxisDirAngle = vrCenterDirection.angleTo( vrBaseArcNormal );
    double dDimExtLength  = dBaseDirLength * cos(dYAxisDirAngle);

    OdGeVector3d vrXAxis = OdGeVector3d::kXAxis;
    OdGeVector3d vrYAxis = OdGeVector3d::kYAxis;

    vrXAxis = quatView.rotate( vrXAxis );
    vrYAxis = quatView.rotate( vrYAxis );

    vrXAxis = vrXAxis.orthoProject( vrBaseArcNormal );
    vrYAxis = vrYAxis.orthoProject( vrBaseArcNormal );

    if( !OdZero(vrXAxis.length()) )
    {
      vrXAxis.normalize();
      vrYAxis = vrXAxis;
      vrYAxis.rotateBy(OdaPI2, vrBaseArcNormal );
    }
    else
    {
      vrYAxis.normalize();
      vrXAxis = vrYAxis;
      vrXAxis.rotateBy(-OdaPI2, vrBaseArcNormal );
    }

    OdDgDimTextInfo::TextAlignment iAlign = ptDimCenter.getJustification();

    if( iAlign == OdDgDimTextInfo::kManualText )
      iAlign = OdDgDimTextInfo::kCenterLeftText;

    OdGePoint3d ptTermStart = ptStart + vrBaseArcNormal*dDimExtLength;
    OdGePoint3d ptTermEnd   = ptEnd + vrBaseArcNormal*dDimExtLength;

    if( ptDimCenter.getJustification() == OdDgDimTextInfo::kManualText )
    {
      ptDimCenter.setJustification( OdDgDimTextInfo::kCenterLeftText );
    }

    importDimensionDiameterSpecial( pDim, ptStart, ptEnd, ptTermStart, ptTermEnd, vrBaseArcNormal, ptDimCenter, owner );
  }

//-----------------------------------------------------------------------------------------------

  void importDiameterParallelDimension( OdDgElement* e, OdDbBlockTableRecord* owner )
  {
    OdDgDimension* pDim = (OdDgDimension*)(e);
    OdDgDimensionExportPEPtr pDgDimPE =  OdDgDimensionExportPEPtr(OdRxObjectPtr(pDim));

    // Get dimension data
    OdUInt32 ptCnt                = pDim->getPointsCount(); // 3 point for this dimension

    if( ptCnt < 3)
    {
      return;
    }

    OdDgDimPoint ptDimCenter      = pDim->getPoint(0);
    OdDgDimPoint ptDimRadius      = pDim->getPoint(1);
    OdDgDimPoint ptDimDirection   = pDim->getPoint(2);

    OdDgDimRotation quatDim(pDim);

    OdDgDimGeometry geom;
    pDim->getDimGeometry(geom);

    OdDgDimTextFormat format;
    pDim->getDimTextFormat(format);

    OdDgDimTextInfo txtInfo;
    pDim->getDimTextInfo(txtInfo);

    OdDgDimRotation quatView;

    getDimensionViewOptionData( pDim, quatView);

    double                                 dMinLeader    = geom.getMargin();
    double                                 dTermWidth    = geom.getTerminatorWidth();

    if( !geom.getUseMargin() )
      dMinLeader = dTermWidth * 2.0;

    // Prepare data to draw dimension

    OdGePoint3d  ptCenter     = ptDimCenter.getPoint();
    OdGePoint3d  ptRadius     = ptDimRadius.getPoint();
    OdGePoint3d  ptDirection  = ptDimDirection.getPoint();

    double       dRadius        = ptCenter.distanceTo(ptRadius);
    double       dExtLineOffset = -1;
    OdGeVector3d vrDimensionDir = ptDirection - ptCenter;

    if( OdZero(vrDimensionDir.length()) )
    {
      ODA_ASSERT(false);
      vrDimensionDir = OdGeVector3d::kXAxis;
      vrDimensionDir = quatDim.rotate(vrDimensionDir);
    }
    else
      vrDimensionDir.normalize();

    OdDgDimension::PlacementAlignment iAlignment = pDim->getAlignment();

    OdGeVector3d vrXAxis = OdGeVector3d::kXAxis;
    OdGeVector3d vrYAxis = OdGeVector3d::kYAxis;
    OdGeVector3d vrNormal = OdGeVector3d::kZAxis;

    vrNormal = quatDim.rotate( vrNormal );

    OdGePoint3d  ptBaseStart, ptBaseEnd;
    OdGeVector3d vrExtLineDir;

    switch( iAlignment )
    {
      case OdDgDimension::kPaView:
      case OdDgDimension::kPaDrawing:
      {
        // Calculate axis direction for dimension

        OdGeVector3d vrOrdX = OdGeVector3d::kXAxis;
        OdGeVector3d vrOrdY = OdGeVector3d::kYAxis;
        OdGeVector3d vrOrdZ = OdGeVector3d::kZAxis;
        OdGeVector3d vrXBasedXAxis = OdGeVector3d::kXAxis;
        OdGeVector3d vrXBasedYAxis = OdGeVector3d::kYAxis;
        OdGeVector3d vrYBasedXAxis = OdGeVector3d::kXAxis;
        OdGeVector3d vrYBasedYAxis = OdGeVector3d::kYAxis;

        if( ptDimRadius.getRelative() )
          dRadius = ptRadius.asVector().length();

        if( ptDimDirection.getRelative() )
        {
          vrDimensionDir = ptDirection.asVector();

          if( OdZero(vrDimensionDir.length()) )
          {
            ODA_ASSERT(false);
            vrDimensionDir = OdGeVector3d::kXAxis;
            vrDimensionDir = quatDim.rotate( vrDimensionDir );
          }
          else
            vrDimensionDir.normalize();

          ptDirection = ptCenter + ptDirection.asVector();

          vrXBasedXAxis = OdGeVector3d::kXAxis;
          vrXBasedYAxis = OdGeVector3d::kYAxis;
          vrYBasedXAxis = OdGeVector3d::kXAxis;
          vrYBasedYAxis = OdGeVector3d::kYAxis;
        }
        else
        {
          if( iAlignment == OdDgDimension::kPaView )
          {
            vrOrdX = quatView.rotate( vrOrdX );
            vrOrdY = quatView.rotate( vrOrdY );
          }

          vrOrdZ = quatDim.rotate( vrOrdZ );
          vrOrdZ.normalize();

          vrOrdX = vrOrdX.orthoProject( vrOrdZ );
          vrOrdY = vrOrdY.orthoProject( vrOrdZ );

          if( OdZero(vrOrdX.length()) || OdZero(vrOrdY.length()) )
          {
            vrOrdX.set(1.0,0.0,0.0);
            vrOrdY.set(0.0,1.0,0.0);
            vrOrdX = quatDim.rotate( vrOrdX );
            vrOrdY = quatDim.rotate( vrOrdY );
          }

          vrOrdX.normalize();
          vrOrdY.normalize();

          vrXBasedXAxis = vrOrdX;
          vrXBasedYAxis = vrOrdX;
          vrXBasedYAxis.rotateBy( OdaPI2, vrOrdZ );
          vrYBasedYAxis = vrOrdY;
          vrYBasedXAxis = vrOrdY;
          vrYBasedXAxis.rotateBy( -OdaPI2, vrOrdZ );
        }

        // Calculate dimension base points

        OdGeVector3d vrXBisector = vrXBasedXAxis + vrXBasedYAxis;

        if( OdZero(vrXBisector.length()) )
        {
          ODA_ASSERT(false);
          vrXBisector = OdGeVector3d::kXAxis;
          vrXBisector = vrXBisector.rotateBy( OdaPI4, vrNormal );
        }
        else
          vrXBisector.normalize();

        OdGeVector3d vrYBisector = vrYBasedXAxis + vrYBasedYAxis;

        if( OdZero(vrYBisector.length()) )
          vrYBisector = vrXBisector;
        else
          vrYBisector.normalize();

        double dAngleX = vrDimensionDir.angleTo( vrXBasedXAxis );
        double dAngleY = vrDimensionDir.angleTo( vrYBasedYAxis );
        double dAngleXBis = vrDimensionDir.angleTo( vrXBisector );
        double dAngleYBis = vrDimensionDir.angleTo( vrYBisector );

        if( dAngleX < 0 )
          dAngleX = fabs( dAngleX );

        if( dAngleY < 0 )
          dAngleY = fabs( dAngleY );

        if( dAngleXBis < 0 )
          dAngleXBis = fabs( dAngleXBis );

        if( dAngleYBis < 0 )
          dAngleYBis = fabs( dAngleYBis );

        if( dAngleX > OdaPI2 )
          dAngleX = OdaPI - dAngleX;

        if( dAngleY > OdaPI2 )
          dAngleY = OdaPI - dAngleY;

        if( dAngleXBis > OdaPI2 )
          dAngleXBis = OdaPI - dAngleXBis;

        if( dAngleYBis > OdaPI2 )
          dAngleYBis = OdaPI - dAngleYBis;

        if( dAngleX < dAngleY )
        {
          vrXAxis = vrXBasedXAxis;
          vrYAxis = vrXBasedYAxis;

          ptBaseStart = ptCenter - vrYAxis * dRadius;
          ptBaseEnd   = ptCenter + vrYAxis * dRadius;

          if( vrDimensionDir.angleTo(vrXAxis) > vrDimensionDir.angleTo(-vrXAxis) )
            vrExtLineDir = -vrXAxis;
          else
            vrExtLineDir = vrXAxis;
        }
        else
        {
          if( dAngleXBis < dAngleYBis )
          {
            vrXAxis = vrXBasedXAxis;
            vrYAxis = vrXBasedYAxis;
          }
          else
          {
            vrXAxis = vrYBasedXAxis;
            vrYAxis = vrYBasedYAxis;
          }

          ptBaseStart = ptCenter - vrXAxis * dRadius;
          ptBaseEnd   = ptCenter + vrXAxis * dRadius;

          if( vrDimensionDir.angleTo(vrYAxis) > vrDimensionDir.angleTo(-vrYAxis) )
            vrExtLineDir = -vrYAxis;
          else
            vrExtLineDir = vrYAxis;
        }

        // Calculate extension line distance

        double dToPtDir = ptBaseStart.distanceTo( ptDirection );
        OdGeVector3d vrDirBase  = ptDirection - ptBaseStart;
        OdGeVector3d vrStartEnd = ptBaseEnd - ptBaseStart;

        if( OdZero(vrDirBase.length()) )
        {
          ODA_ASSERT(false);
          vrDirBase = OdGeVector3d::kYAxis;
        }
        else
          vrDirBase.normalize();

        if( OdZero(vrStartEnd.length()) )
        {
          ODA_ASSERT(false);
          vrStartEnd = OdGeVector3d::kXAxis;
        }
        else
          vrStartEnd.normalize();

        dExtLineOffset = dToPtDir * sin(vrStartEnd.angleTo(vrDirBase));
      } break;

      case OdDgDimension::kPaTrue:
      case OdDgDimension::kPaArbitrary:
      {
        if( ptDimRadius.getRelative() )
          dRadius = ptRadius.asVector().length();

        if( ptDimDirection.getRelative() )
        {
          vrDimensionDir = ptDirection.asVector();

          if( OdZero(vrDimensionDir.length()) )
          {
            ODA_ASSERT(false);
            vrDimensionDir = OdGeVector3d::kXAxis;
            vrDimensionDir = quatDim.rotate( vrDimensionDir );
          }
          else
            vrDimensionDir.normalize();

          ptDirection = ptCenter + ptDirection.asVector();
        }

        vrExtLineDir = ptDirection - ptCenter;
        dExtLineOffset = vrExtLineDir.length();
        vrExtLineDir.normalize();
        OdGeVector3d vrOrdZ = OdGeVector3d::kZAxis;
        OdGeVector3d vrOrdX = OdGeVector3d::kXAxis;
        OdGeVector3d vrOrdY = OdGeVector3d::kYAxis;
        vrOrdZ = quatDim.rotate( vrOrdZ );
        vrOrdX = quatView.rotate(vrOrdX );
        vrOrdY = quatView.rotate(vrOrdY );

        vrOrdX.orthoProject(vrOrdZ);
        vrOrdY.orthoProject(vrOrdZ);

        if( !OdZero(vrOrdX.length()) )
        {
          vrXAxis = vrOrdX;
          vrXAxis.normalize();

          vrYAxis = vrXAxis;
          vrYAxis.rotateBy(OdaPI2, vrOrdZ);
        }
        else
        {
          vrYAxis = vrOrdY;
          vrYAxis.normalize();

          vrXAxis = vrYAxis;
          vrXAxis.rotateBy(-OdaPI2, vrOrdZ);
        }

        OdGeVector3d vrBase = vrExtLineDir;
        vrBase = vrBase.rotateBy( OdaPI2, vrOrdZ );
        vrBase.normalize();

        if( fabs(vrXAxis.angleTo(vrBase)) < OdaPI2 )
        {
          ptBaseStart = ptCenter - vrBase * dRadius;
          ptBaseEnd   = ptCenter + vrBase * dRadius;
        }
        else
        {
          ptBaseStart = ptCenter + vrBase * dRadius;
          ptBaseEnd   = ptCenter - vrBase * dRadius;
        }
      } break;
    }

    // Draw leaders and terminators

    OdGePoint3d ptTermLineStart = ptBaseStart + vrExtLineDir * dExtLineOffset;
    OdGePoint3d ptTermLineEnd   = ptBaseEnd + vrExtLineDir * dExtLineOffset;

    OdGeVector3d vrDimCheckDir = ptTermLineEnd - ptTermLineStart;

    if( !OdZero(vrDimCheckDir.length()) )
    {
      vrDimCheckDir.normalize();
    }
    else
      vrDimCheckDir = OdGeVector3d::kXAxis;

    vrDimCheckDir.rotateBy(OdaPI2,vrNormal);

    if( vrDimCheckDir.angleTo(vrExtLineDir) > OdaPI2 )
    {
      OdGePoint3d ptTmp = ptTermLineStart;
      ptTermLineStart = ptTermLineEnd;
      ptTermLineEnd = ptTmp;
    }

    if( ptDimCenter.getJustification() == OdDgDimTextInfo::kManualText )
    {
      ptDimCenter.setJustification( OdDgDimTextInfo::kCenterLeftText );
    }

    importDimensionDiameterSpecial( pDim, ptBaseStart, ptBaseEnd, ptTermLineStart, ptTermLineEnd, vrNormal, ptDimCenter, owner );
  }

//-----------------------------------------------------------------------------------------------

  void importDiameterRadiusDimension( OdDgElement* e, OdDbBlockTableRecord* owner, bool bDiameterDimension )
  {
    OdDgDimension* pDim = (OdDgDimension*)(e);
    OdDgDimensionExportPEPtr pDgDimPE =  OdDgDimensionExportPEPtr(OdRxObjectPtr(pDim));

    // Get dimension data
    OdUInt32 ptCnt                = pDim->getPointsCount();

    if( (ptCnt < 3) || pDgDimPE.isNull() )
    {
      return;
    }

    OdDgnImportDgDimDependencyMap dependencyMap;
    bool bSaveAssocDimData = createDgnDimDependencyMap( pDim, dependencyMap );

    OdDgDimPoint ptDimCenter      = pDim->getPoint(0);
    OdDgDimPoint ptDimRadius      = pDim->getPoint(1);
    OdDgDimPoint ptDimDirection   = pDim->getPoint(2);

    OdDgDimRotation quatDim(pDim);

    OdGeVector3d vrNormal = OdGeVector3d::kZAxis;

    vrNormal = quatDim.rotate( vrNormal );

    // Prepare data to draw dimension

    OdGePoint3d  ptCenter     = ptDimCenter.getPoint();
    OdGePoint3d  ptRadius     = ptDimRadius.getPoint();
    OdGePoint3d  ptDirection  = ptDimDirection.getPoint();

    double       dRadius        = ptCenter.distanceTo(ptRadius);
    OdGeVector3d vrDimensionDir = ptDirection - ptCenter;

    if( ptDimRadius.getRelative() )
    {
      dRadius = ptRadius.asVector().length();

      ptRadius = ptCenter + ptRadius.asVector();
    }

    if( ptDimDirection.getRelative() )
    {
      vrDimensionDir = ptDirection.asVector();

      if( OdZero(vrDimensionDir.length()) )
      {
        vrDimensionDir = OdGeVector3d::kXAxis;
        vrDimensionDir = quatDim.rotate( vrDimensionDir );
      }
      else
        vrDimensionDir.normalize();

      ptDirection = ptCenter + ptDirection.asVector();
    }
    else if( OdZero(vrDimensionDir.length()) )
    {
      vrDimensionDir = OdGeVector3d::kXAxis;
      vrDimensionDir = quatDim.rotate( vrDimensionDir );
    }
    else
      vrDimensionDir.normalize();

    OdDgDimTool::Leader         iDimType = pDgDimPE->getLeader( pDim );

    OdGePoint3d ptDimLeader = ptDirection;

    for( OdUInt32 i = 3; i < ptCnt; i++ )
    {
      OdDgDimPoint ptCurDimLeader  = pDim->getPoint(i);

      OdGePoint3d ptLeader = ptCurDimLeader.getPoint();

      if( ptCurDimLeader.getRelative() )
      {
        ptLeader += ptCenter.asVector();
      }

      ptDimLeader = ptLeader;
    }

    double       dExtLine       = ptCenter.distanceTo(ptDimLeader);

    bool bOffForceLine      = false;
    OdUInt32 uDimLeaderMode = 0;
    bool bTextOutline = true;

    if( dRadius > dExtLine )
    {
      uDimLeaderMode = 2;
      bOffForceLine  = false;
      bTextOutline   = false;
    }

    if( iDimType == OdDgDimTool::kRadius )
    {
      uDimLeaderMode = 0;
      bOffForceLine  = true;
    }

    OdDbDimensionPtr pDbDimBase;

    OdGeVector3d vrTextDir = pDgDimPE->getDimensionTextDirection( pDim, 0 );
    ptDimCenter.setJustification( OdDgDimTextInfo::kManualText );

    if( bDiameterDimension )
    {
      OdDbDiametricDimensionPtr pDbDim = OdDbDiametricDimension::createObject();
      pDbDim->setDatabaseDefaults( owner->database() );
      copyEntityProperties(pDim,pDbDim);

      ptRadius = ptCenter + vrDimensionDir * dRadius;

      pDbDim->setChordPoint( ptRadius );

      OdGePoint3d ptFarChord = ptCenter - vrDimensionDir * dRadius ;

      OdGeVector3d vrRadiusCenter = ptCenter - ptRadius;

      if( !OdZero(vrRadiusCenter.length()) )
      {
        vrRadiusCenter.normalize();

        ptFarChord = ptCenter + vrRadiusCenter * dRadius ;
      }

      if( !OdZero(ptRadius.distanceTo( ptFarChord) - dRadius*2, 1e-4) )
      {
        ptFarChord = ptCenter + vrDimensionDir * dRadius ;
      }

      pDbDim->setFarChordPoint( ptFarChord );
      pDbDim->setLeaderLength( 0 );
      pDbDim->setNormal( vrNormal );

      fillDimensionProperties( pDim, vrNormal, 0, ptDimCenter, ptDimCenter, ptRadius, ptCenter - vrDimensionDir * dRadius, ptDimLeader, vrTextDir, false, false, true, false, pDbDim, owner->database(), uDimLeaderMode );

      pDbDimBase = pDbDim;
    }
    else
    {
      OdDbRadialDimensionPtr pDbDim = OdDbRadialDimension::createObject();
      pDbDim->setDatabaseDefaults( owner->database() );
      copyEntityProperties(pDim,pDbDim);

      ptRadius = ptCenter + vrDimensionDir * dRadius;

      pDbDim->setCenter( ptCenter );
      pDbDim->setChordPoint( ptRadius );
      pDbDim->setLeaderLength( 0 );
      pDbDim->setNormal( vrNormal );

      fillDimensionProperties( pDim, vrNormal, 0, ptDimCenter, ptDimCenter, ptDimLeader, ptCenter, ptDimLeader, vrTextDir, false, false, true, false, pDbDim, owner->database(), uDimLeaderMode );

      pDbDimBase = pDbDim;
    }

    pDbDimBase->setDimtofl( !bOffForceLine );
    pDbDimBase->setDimtix( !bTextOutline );

    if( !bTextOutline )
    {
      pDbDimBase->setDimatfit(1);
    }

    owner->appendOdDbEntity( pDbDimBase );

    if( bSaveAssocDimData )
    {
      OdDgnImportDgDimDependencyMap::iterator pPoint1Iter = dependencyMap.find( 0 );

      OdDgnImportDimAssocDescriptor curDesc;

      bool bAddAssocDesc = false;

      if( pPoint1Iter != dependencyMap.end() )
      {
        curDesc.point1Desc = pPoint1Iter->second;
        bAddAssocDesc = true;
      }

      if( bAddAssocDesc )
      {
        OdDgnImportContext::addDimAssocDescriptor( pDbDimBase->objectId(), curDesc );
      }
    }

    createDimensionGeometryBlock( pDim, 0, vrNormal,pDbDimBase, owner->database() );
  }

//-----------------------------------------------------------------------------------------------

  void importOrdinateDimension( OdDgElement* e, OdDbBlockTableRecord* owner )
  {
    OdDgDimOrdinate* pDim = (OdDgDimOrdinate*)(e);
    OdDgDimensionExportPEPtr pDgDimPE =  OdDgDimensionExportPEPtr(OdRxObjectPtr(pDim));

    OdUInt32     nPts            = pDim->getPointsCount();

    if( nPts < 1 || pDgDimPE.isNull() )
    {
      return;
    }

    OdDgnImportDgDimDependencyMap dependencyMap;

    bool bSaveAssocDimData = createDgnDimDependencyMap( pDim, dependencyMap );

    OdGePoint3d ptOrigin;
    OdGePoint3d ptDefinition;
    OdGePoint3d ptLeaderEnd;
    bool        bXDirextion = true;

    bool   bStackedDimension  = false;
    bool   bFreeTextDimension = false;
    double dStartValue = 0;
    bool   bDecrimentInReverceDirection = false;

    OdDgDimPoint ptDimStart       = pDim->getPoint(0);

    OdDgDimRotation quatDim(pDim);

    OdDgDimGeometry geom;
    pDim->getDimGeometry(geom);

    OdDgDimTextFormat format;
    pDim->getDimTextFormat(format);

    OdDgDimTextInfo txtInfo;
    pDim->getDimTextInfo(txtInfo);

    OdDgDimRotation quatView;

    getDimensionViewOptionData( pDim, quatView );

    bStackedDimension            = pDgDimPE->getStackExtLinesFlag( pDim );
    dStartValue                  = pDgDimPE->getDatumValue( pDim );
    bFreeTextDimension           = pDgDimPE->getFreeLocationOfTxtFlag( pDim );
    bDecrimentInReverceDirection = pDgDimPE->getDecrementInReverseDirectionFlag( pDim );

    double dMinLeader    = geom.getMargin();

    if( !geom.getUseMargin() )
      dMinLeader = geom.getTerminatorWidth() * 2.0;

    OdGeVector3d vrDimDirection = OdGeVector3d::kXAxis;
    OdGeVector3d vrNormal       = OdGeVector3d::kZAxis;
    vrDimDirection = quatDim.rotate(vrDimDirection);
    vrNormal       = quatDim.rotate(vrNormal);

    OdGeVector3d vrViewNormal = OdGeVector3d::kZAxis;
    vrViewNormal = quatView.rotate(vrViewNormal);

    OdGeVector3d vrDimYDirection = vrDimDirection;
    vrDimYDirection.rotateBy(OdaPI2, vrNormal);

    if( vrNormal.angleTo(vrViewNormal) > OdaPI2 )
      vrNormal *= -1;

    OdGeVector3d vrXAxis = OdGeVector3d::kXAxis;
    OdGeVector3d vrYAxis = OdGeVector3d::kYAxis;
    vrXAxis = quatView.rotate( vrXAxis );
    vrYAxis = quatView.rotate( vrYAxis );

    vrXAxis.orthoProject(vrNormal);
    vrYAxis.orthoProject(vrNormal);

    if( !OdZero(vrXAxis.length()) )
    {
      vrXAxis.normalize();

      vrYAxis = vrXAxis;
      vrYAxis.rotateBy(OdaPI2, vrNormal);
    }
    else
    {
      vrYAxis.normalize();

      vrXAxis = vrYAxis;
      vrXAxis.rotateBy(-OdaPI2, vrNormal);
    }

    OdGeVector3d vrDimYInc = vrDimYDirection;

//     if( vrDimYDirection.angleTo(vrYAxis) > OdaPI2 )
//       vrDimYInc *= -1;

    OdArray<OdDgDimPoint> m_dimPtsArr;

    OdGeLine3d lineCurPt;

    bool bNegativeY = false;

    OdGeVector3d vrDimYDirectionCheck = OdGeVector3d::kYAxis;
    vrDimYDirectionCheck = quatDim.rotate(vrDimYDirectionCheck);

    double dCheckYAngle = vrDimYDirectionCheck.angleTo(vrDimYDirection);

    if( OdZero(dCheckYAngle - Oda2PI, 1e-4) )
    {
      dCheckYAngle = 0;
    }

    if( dCheckYAngle > OdaPI2 )
    {
      bNegativeY = true;
    }

    if( !bStackedDimension )
    {
      vrDimYDirection = vrDimYDirectionCheck;
    }

    if( bStackedDimension )
    {
      OdDoubleArray dDimValueArr;
      m_dimPtsArr.push_back(pDim->getPoint(0));
      dDimValueArr.push_back(0);

      OdGeLine3d lineCheck( ptDimStart.getPoint(), vrDimDirection);

      for( OdUInt32 k = 1; k < nPts; k++ )
      {
        double dDistanceToBase = 0;
        OdGePoint3d ptCur = pDim->getPoint(k).getPoint();

        lineCurPt.set( ptCur, vrDimYDirection );            

        OdGePoint3d  ptProjectOnBaseLine = ptCur;
        OdGeVector3d vrProjectCur = vrDimYDirection;
        lineCheck.intersectWith(lineCurPt, ptProjectOnBaseLine );
        vrProjectCur = ptCur - ptProjectOnBaseLine;

        dDistanceToBase = ptProjectOnBaseLine.distanceTo(ptCur);

        if( !OdZero(vrProjectCur.length()) )
        {
          vrProjectCur.normalize();

          if( !OdZero(vrProjectCur.angleTo(vrDimYInc), 1e-4) )
            dDistanceToBase *= -1;
        }

        bool bAddToEnd = true;

        for( OdUInt32 l = 0; l < dDimValueArr.size(); l++ )
        {
          bool bExchangeElements = false;

          if( bNegativeY )
          {
            bExchangeElements = dDistanceToBase > dDimValueArr[l];
          }
          else
          {
            bExchangeElements = dDistanceToBase < dDimValueArr[l];
          }

          if( bExchangeElements )
          {
            dDimValueArr.insert(dDimValueArr.begin()+l, dDistanceToBase);
            m_dimPtsArr.insert(m_dimPtsArr.begin()+l, pDim->getPoint(k));
            bAddToEnd = false;
            break;
          }
        }

        if( bAddToEnd )
        {
          dDimValueArr.push_back(dDistanceToBase);
          m_dimPtsArr.push_back(pDim->getPoint(k));
        }
      }
    }
    else
    {
      for( OdUInt32 k = 0; k < nPts; k++ )
        m_dimPtsArr.push_back(pDim->getPoint(k));
    }

    ptDimStart = m_dimPtsArr[0];

    ptOrigin = ptDimStart.getPoint();

    double dCheckAngle = vrDimDirection.angleTo( vrXAxis );

    if( dCheckAngle > OdaPI2 )
    {
      dCheckAngle = OdaPI - dCheckAngle;
    }

    bXDirextion = fabs( dCheckAngle ) > OdaPI4;

    OdGeLine3d lineBase( ptDimStart.getPoint(), vrDimDirection);
    OdGePoint3d ptBaseLineStart = ptDimStart.getPoint();
    OdGePoint3d ptBaseLineEnd   = ptBaseLineStart + vrDimDirection;

    double dStackOffset = fabs(geom.getTextMargin()) + txtInfo.getHeight();

    double dTopStackOffset = 0;
    double dBottomStackOffset = 0;
    bool   bInitStacks = false;

    for( OdUInt32 i = 0; i < nPts; i++ )
    {
      ptDimStart      = m_dimPtsArr[i];
      double dXOffset = ptDimStart.getOffsetToDimLine();
      double dYOffset = ptDimStart.getOffsetY();

      double dDistanceToBase = 0;

      OdGePoint3d ptCur = ptDimStart.getPoint();

      lineCurPt.set( ptCur, vrDimYDirection);               // MKU 19/03/10 - optimization after Ge changes

      OdGePoint3d  ptProjectOnBaseLine = ptCur;
      OdGeVector3d vrProjectCur = vrDimYDirection;
      lineBase.intersectWith(lineCurPt, ptProjectOnBaseLine );
      vrProjectCur = ptCur - ptProjectOnBaseLine;

      dDistanceToBase = ptProjectOnBaseLine.distanceTo(ptCur);

      if( !OdZero(vrProjectCur.length()) )
      {
        vrProjectCur.normalize();

        if( !OdZero(vrProjectCur.angleTo(vrDimYInc),1e-4) )
          dDistanceToBase *= -1;
      }

      if( bStackedDimension && !bFreeTextDimension )
      {
        if( dDistanceToBase < 0 )
        {
          if( dDistanceToBase > dBottomStackOffset )
            dYOffset = dBottomStackOffset - dDistanceToBase;

          dBottomStackOffset = dDistanceToBase + dYOffset - dStackOffset;
        }
        else if( dDistanceToBase > 0 )
        {
          if( dDistanceToBase < dTopStackOffset )
            dYOffset = dTopStackOffset - dDistanceToBase;

          dTopStackOffset = dDistanceToBase + dYOffset + dStackOffset;
        }
        else
        {
          if( !bInitStacks )
          {
            dBottomStackOffset = - dStackOffset;
            dTopStackOffset = dStackOffset;
            bInitStacks = true;
          }
        }
      }

      if( ptDimStart.getNoWitnessLineFlag() )
      {
        continue;
      }

      OdGePoint3d vertices[4];

      ptDefinition = ptCur;
      ptLeaderEnd  = ptCur;

      if(!OdZero(dYOffset))
      {
        double dDirAngle = vrDimDirection.angleTo(vrXAxis);

        if( OdZero( dDirAngle - OdaPI2) )
        {
          double dDirYAngle = vrDimDirection.angleTo(vrYAxis);

          if( dDirYAngle < OdaPI2 )
            dDirAngle -= OdaPI4;
          else
            dDirAngle += OdaPI4;
        }

        if( dDirAngle < OdaPI2 )
        {
          if( dXOffset > 0 )
          {
            ptLeaderEnd  = ptCur + vrDimYDirection*dYOffset + vrDimDirection * ( fabs(dXOffset) - geom.getTextMargin() );
          }
          else
          {
            ptLeaderEnd  = ptCur + vrDimYDirection*dYOffset - vrDimDirection * ( fabs(dXOffset) );
          }
        }
        else
        {
          if( dXOffset > 0 )
          {
            ptLeaderEnd = ptCur + vrDimYDirection*dYOffset + vrDimDirection * ( fabs(dXOffset) );
          }
          else
          {
            ptLeaderEnd = ptCur + vrDimYDirection*dYOffset - vrDimDirection * ( fabs(dXOffset) - geom.getTextMargin() );
          }
        }
      }
      else if( OdZero(dYOffset) && !OdZero(dXOffset) )
      {
        double dDirAngle = vrDimDirection.angleTo(vrXAxis);

        if( OdZero( dDirAngle - OdaPI2) )
        {
          double dDirYAngle = vrDimDirection.angleTo(vrYAxis);

          if( dDirYAngle < OdaPI2 )
            dDirAngle -= OdaPI4;
          else
            dDirAngle += OdaPI4;
        }

        if( dDirAngle < OdaPI2)
        {
          if( dXOffset > 0 )
          {
            if( fabs(dXOffset) > (fabs(geom.getWitnessLineOffset()) + fabs(geom.getTextMargin())))
            {
              ptLeaderEnd = ptCur + vrDimDirection *( fabs(dXOffset) - geom.getTextMargin());
            }
          }
          else
          {
            if( fabs(dXOffset) > fabs(geom.getWitnessLineOffset()))
            {
              ptLeaderEnd = ptCur - vrDimDirection *fabs(dXOffset);
            }
          }
        }
        else
        {
          if( dXOffset > 0 )
          {
            if( fabs(dXOffset) > fabs(geom.getWitnessLineOffset()))
            {
              ptLeaderEnd = ptCur + vrDimDirection *fabs(dXOffset);
            }
          }
          else
          {
            if( fabs(dXOffset) > (fabs(geom.getWitnessLineOffset()) + fabs(geom.getTextMargin())))
            {
              ptLeaderEnd = ptCur - vrDimDirection *( fabs(dXOffset) - geom.getTextMargin());
            }
          }
        }
      }

      // Create dimension

      OdDbOrdinateDimensionPtr pDbDim = OdDbOrdinateDimension::createObject();

      pDbDim->setDatabaseDefaults( owner->database() );

      copyEntityProperties(pDim,pDbDim);

      pDbDim->setOrigin( ptOrigin + vrDimYDirection * dStartValue );
      pDbDim->setDefiningPoint( ptDefinition );
      pDbDim->setLeaderEndPoint( ptLeaderEnd );

      if( bXDirextion )
      {
        pDbDim->useXAxis();
      }
      else
      {
        pDbDim->useYAxis();
      }

      double dDimAngle = 0;

      if( !bXDirextion )
      {
        dDimAngle = vrDimDirection.angleTo( vrXAxis, vrNormal );
      }
      else
      {
        dDimAngle = vrDimDirection.angleTo( vrYAxis, vrNormal );
      }

      if( dDimAngle > OdaPI2 )
      {
        dDimAngle = dDimAngle - OdaPI;
      }

      pDbDim->setHorizontalRotation( dDimAngle );

      fillDimensionProperties( pDim, vrNormal, i, ptDimStart, ptDimStart, ptDefinition, ptLeaderEnd, ptLeaderEnd, vrDimDirection, false, false, true, false, pDbDim, owner->database(), 0, true );

      owner->appendOdDbEntity( pDbDim );

      if( bSaveAssocDimData )
      {
        OdDgnImportDgDimDependencyMap::iterator pPoint1Iter = dependencyMap.end();

        if( OdZero(dStartValue) )
        {
          pPoint1Iter = dependencyMap.find( 0 );
        }

        OdDgnImportDgDimDependencyMap::iterator pPoint2Iter = dependencyMap.find( i );

        OdDgnImportDimAssocDescriptor curDesc;

        bool bAddAssocDesc = false;

        if( pPoint1Iter != dependencyMap.end() )
        {
          curDesc.point1Desc = pPoint1Iter->second;
          bAddAssocDesc = true;
        }

        if( pPoint2Iter != dependencyMap.end() )
        {
          curDesc.point2Desc = pPoint2Iter->second;
          bAddAssocDesc = true;
        }

        if( bAddAssocDesc )
        {
          OdDgnImportContext::addDimAssocDescriptor( pDbDim->objectId(), curDesc );
        }
      }

      createDimensionGeometryBlock( pDim, i, vrNormal, pDbDim, owner->database() );
    }
  }

//-----------------------------------------------------------------------------------------------

  void OdDgnAngularDimensionImportPE::subImportElement(OdDgElement* e, OdDbBlockTableRecord* owner)
  {
    importAngularDimension( e, owner );
  }

//-----------------------------------------------------------------------------------------------

  void OdDgnDiameterPerpDimensionImportPE::subImportElement(OdDgElement* e, OdDbBlockTableRecord* owner)
  {
    importDiameterPerpDimension( e, owner );
  }

//-----------------------------------------------------------------------------------------------

  void OdDgnDiameterParallelDimensionImportPE::subImportElement(OdDgElement* e, OdDbBlockTableRecord* owner)
  {
    importDiameterParallelDimension( e, owner );
  }

//-----------------------------------------------------------------------------------------------

  void OdDgnDiameterDimensionImportPE::subImportElement(OdDgElement* e, OdDbBlockTableRecord* owner)
  {
    importDiameterRadiusDimension( e, owner, true );
  }

//-----------------------------------------------------------------------------------------------

  void OdDgnRadiusDimensionImportPE::subImportElement(OdDgElement* e, OdDbBlockTableRecord* owner)
  {
    importDiameterRadiusDimension( e, owner, false );
  }

//-----------------------------------------------------------------------------------------------

  void OdDgnOrdinateDimensionImportPE::subImportElement(OdDgElement* e, OdDbBlockTableRecord* owner)
  {
    importOrdinateDimension( e, owner );
  }

//-----------------------------------------------------------------------------------------------

  void OdDgnCenterMarkDimensionImportPE::subImportElement(OdDgElement* e, OdDbBlockTableRecord* owner)
  {
    OdDgDimCenter* pDim = (OdDgDimCenter*)(e);

    OdDbBlockTableRecordPtr pDimGeometryBlock = OdDbBlockTableRecord::createObject();

    OdDbBlockTablePtr pBlockTable = owner->database()->getBlockTableId().openObject( OdDb::kForWrite );

    OdDgDimRotation quatDim(pDim);

    OdGeVector3d vrNormal = OdGeVector3d::kZAxis;

    vrNormal = quatDim.rotate( vrNormal );

    if( !pBlockTable.isNull() )
    {
      pDimGeometryBlock->setName( L"*D" );

      pBlockTable->add(pDimGeometryBlock);

      importDimensionSegment( pDim, 0, vrNormal, pDimGeometryBlock );

      OdDgDimRotation quatDim(pDim);

      OdGeMatrix3d matTransform = quatDim.getMatrix() * OdGeMatrix3d::translation( pDim->getPoint(0).getPoint().asVector() );
      OdGeMatrix3d matDeTransform = matTransform;
      matDeTransform = matDeTransform.invert();

      for (OdDbObjectIteratorPtr pIter = pDimGeometryBlock->newIterator() ; !pIter->done(); pIter->step())
      {
        OdDbEntityPtr elm = pIter->objectId().safeOpenObject(OdDb::kForWrite);
        elm->transformBy(matDeTransform);
      }

      OdDbBlockReferencePtr pBlockReference = OdDbBlockReference::createObject();
      pBlockReference->setBlockTableRecord( pDimGeometryBlock->objectId() );
      owner->appendOdDbEntity( pBlockReference );
      pBlockReference->setBlockTransform( matTransform );
      copyEntityProperties( pDim, pBlockReference );
    }
  }

//-----------------------------------------------------------------------------------------------

  void OdDgnLabelLineDimensionImportPE::subImportElement(OdDgElement* e, OdDbBlockTableRecord* owner)
  {
    OdDgDimLabelLine* pDim = (OdDgDimLabelLine*)(e);

    OdDbBlockTableRecordPtr pDimGeometryBlock = OdDbBlockTableRecord::createObject();

    OdDbBlockTablePtr pBlockTable = owner->database()->getBlockTableId().openObject( OdDb::kForWrite );

    OdDgDimRotation quatDim(pDim);

    OdGeVector3d vrNormal = OdGeVector3d::kZAxis;

    vrNormal = quatDim.rotate( vrNormal );

    if( !pBlockTable.isNull() )
    {
      pDimGeometryBlock->setName( L"*D" );

      pBlockTable->add(pDimGeometryBlock);

      importDimensionSegment( pDim, 0, vrNormal, pDimGeometryBlock );

      OdDbMTextPtr pMText;

      for (OdDbObjectIteratorPtr pIter = pDimGeometryBlock->newIterator() ; !pIter->done(); pIter->step())
      {
        OdDbEntityPtr elm = pIter->objectId().safeOpenObject(OdDb::kForWrite);

        if( !elm->isKindOf( OdDbText::desc() ) && !elm->isKindOf( OdDbMText::desc() ) )
        {
          continue;
        }

        OdDbMTextPtr pCurMText;

        if( elm->isKindOf( OdDbText::desc() ) )
        {
          OdDbTextPtr pText = elm;

          pCurMText = OdDbMText::createObject();
          pCurMText->setPropertiesFrom( pText );
          pCurMText->setTextHeight( pText->height() );
          pCurMText->setTextStyle( pText->textStyle() );
          pCurMText->setContents( pText->textString() );
          pCurMText->setLocation( pText->position() );
          pCurMText->setNormal( pText->normal() );
          pCurMText->setRotation( pText->rotation() );
        }
        else
        {
          pCurMText = elm;
        }

        if( pMText.isNull() )
        {
          pMText = pCurMText->clone();
        }
        else
        {
          OdString strContents = pMText->contents();

          if( (pDim->getLabelLineMode() == OdDgDimLabelLine::kLengthAngle) || (pDim->getLabelLineMode() == OdDgDimLabelLine::kAngleLength) )
          {
            strContents += L"\\P" + pCurMText->contents();
          }
          else
          {
            strContents += L"  " + pCurMText->contents();
          }

          pMText->setContents( strContents );
        }
      }

      if( !pMText.isNull() )
      {
        OdDgDimGeometry geom;
        pDim->getDimGeometry(geom);

        OdDgDimTextInfo txtInfo;
        pDim->getDimTextInfo(txtInfo);

        pMText->setLineSpacingFactor( geom.getTextLift() / txtInfo.getHeight() + 1.0 );
        pMText->setLineSpacingStyle( OdDb::kExactly );
        pMText->setAttachment(OdDbMText::kMiddleCenter);

        OdGeExtents3d extDim;
        pDim->getGeomExtents( extDim );

        if( (pDim->getLabelLineMode() == OdDgDimLabelLine::kLengthAngle) || (pDim->getLabelLineMode() == OdDgDimLabelLine::kAngleLength) )
        {

          OdGePoint3d ptStart = pDim->getPoint( 0 ).getPoint();
          OdGePoint3d ptEnd   = pDim->getPoint( 1 ).getPoint();

          OdDgDimRotation quatDim(pDim);

          OdGeVector3d vrNormal = OdGeVector3d::kZAxis;
         
          vrNormal = quatDim.rotate( vrNormal );
         
          OdGeVector3d vrDimensionDir = ptEnd - ptStart;
          vrDimensionDir.normalize();
          OdGeVector3d vrDimensionYDir = vrDimensionDir;
          vrDimensionYDir = vrDimensionYDir.rotateBy(OdaPI2,vrNormal);

          OdGeLine3d lineDim( ptStart, vrDimensionDir );
          OdGeLine3d lineTextPerp( extDim.center(), vrDimensionYDir );

          OdGePoint3d ptIsect;

          if( lineDim.intersectWith( lineTextPerp, ptIsect ) )
          {
            pMText->setLocation( ptIsect );
          }
          else
          {
            pMText->setLocation( extDim.center() );
          }
        }
        else
        {
          if( extDim.isValidExtents() )
          {
            pMText->setLocation( extDim.center() );
          }
        }

        owner->appendOdDbEntity( pMText );
      }

      pDimGeometryBlock->erase(true);
    }
  }

//-----------------------------------------------------------------------------------------------

  OdDgElementId getDimNoteExternalContext( OdDgDimNote* pDim )
  {
    OdDgElementId idRet;

    OdDgElementPtr pCellElement;

    OdRxObjectPtrArray arrDepLinkages;

    pDim->getLinkages( OdDgAttributeLinkage::kDependency, arrDepLinkages );

    for( OdUInt32 i = 0; i < arrDepLinkages.size(); i++ )
    {
      OdDgElementId idElmToCheck;

      if( arrDepLinkages[i]->isKindOf(OdDgDepLinkageElementId::desc() ) )
      {
        OdDgDepLinkageElementIdPtr pDepLinkage = arrDepLinkages[i];

        if( !pDepLinkage.isNull() && (pDepLinkage->getCount() > 0) )
        {
          idElmToCheck = pDim->database()->getElementId( OdDbHandle(pDepLinkage->getAt(0)));
        }
      }
      else if( arrDepLinkages[i]->isKindOf( OdDgDepLinkageAssocPoint::desc() ) )
      {
        OdDgDepLinkageAssocPointPtr pAssocPointLinkage = arrDepLinkages[i];

        if( !pAssocPointLinkage.isNull() && (pAssocPointLinkage->getCount() > 0) )
        {
          OdDgAssocPointRootPtr pRoot = pAssocPointLinkage->getAt(0);
          idElmToCheck = pDim->database()->getElementId( OdDbHandle(pRoot->getElementId()));
        }
      }
      else if( arrDepLinkages[i]->isKindOf( OdDgDepLinkageAssocPointI::desc() ) )
      {
        OdDgDepLinkageAssocPointIPtr pAssocPointILinkage = arrDepLinkages[i];

        if( !pAssocPointILinkage.isNull() && (pAssocPointILinkage->getCount() > 0) )
        {
          OdDgAssocPointIData pData = pAssocPointILinkage->getAt(0);
          idElmToCheck = pDim->database()->getElementId( OdDbHandle(pData.m_assocPointData->getElementId()));
        }
      }

      if( !idElmToCheck.isNull() )
      {
        OdDgElementPtr pElm = idElmToCheck.openObject(OdDg::kForRead);

        if( pElm->getElementType() == OdDgElement::kTypeCellHeader )
        {
          pCellElement = pElm;
        }
      }
    }

//     if( pCellElement.isNull() )
//     {
//       OdDgElementId idOwner = pDim->ownerId();
// 
//       if( !idOwner.isNull() )
//       {
//         OdDgElementPtr pOwner = idOwner.openObject(OdDg::kForRead);
// 
//         OdDgElementIteratorPtr pIter;
// 
//         if( pOwner->isKindOf( OdDgModel::desc() ) )
//         {
//           pIter = ((OdDgModelPtr)(pOwner))->createGraphicsElementsIterator();
//         }
//         else if( pOwner->isKindOf( OdDgCellHeader2d::desc() ) )
//         {
//           pIter = ((OdDgCellHeader2dPtr)(pOwner))->createIterator();
//         }
//         else if( pOwner->isKindOf( OdDgCellHeader3d::desc() ) )
//         {
//           pIter = ((OdDgCellHeader3dPtr)(pOwner))->createIterator();
//         }
//         else if( pOwner->isKindOf( OdDgSharedCellDefinition::desc() ) )
//         {
//           pIter = ((OdDgSharedCellDefinitionPtr)(pOwner))->createIterator();
//         }
// 
//         if( !pIter.isNull() )
//         {
//           for(; !pIter->done(); pIter->step() )
//           {
//             if( pIter->item() == pDim->elementId() )
//             {
//               break;
//             }
//           }
// 
//           if( !pIter->done() )
//           {
//             pIter->step();
//           }
// 
//           if( !pIter->done() )
//           {
//             pCellElement = pIter->item().openObject(OdDg::kForRead);
//           }
//         }
//       }
//     }

    if( !pCellElement.isNull() ) 
    {
      OdRxObjectPtrArray arrLinkages;
      pCellElement->getLinkages(OdDgAttributeLinkage::kNoteLinkage, arrLinkages);

      if( !arrLinkages.isEmpty() )
      {
        if( pCellElement->isKindOf( OdDgCellHeader2d::desc() ) || pCellElement->isKindOf( OdDgCellHeader3d::desc()))
        {
          idRet = pCellElement->elementId();
        }
      }
    }

    return idRet;
  }

//-----------------------------------------------------------------------------------------------

  OdDbObjectId createMText( const OdDbTextPtr pText, OdDbBlockTableRecord* owner )
  {
    OdDbObjectId idRet;

    OdDbMTextPtr pMText = OdDbMText::createObject();
    pMText->setPropertiesFrom( pText );
    pMText->setTextHeight( pText->height() );
    pMText->setTextStyle( pText->textStyle() );
    pMText->setContents( pText->textString() );
    pMText->setLocation( pText->position() );
    pMText->setNormal( pText->normal() );
    pMText->setRotation( pText->rotation() );

    owner->appendOdDbEntity( pMText );

    idRet = pMText->objectId();

    return idRet;
  }

//-----------------------------------------------------------------------------------------------

  bool getDimNoteExternalMText( const OdDgElementId& idExternalCell, 
                                  OdDbBlockTableRecord* owner,
                                    bool bCreateMTextOnly,
                                      OdDbObjectId& idMText,
                                        OdDbObjectId& idBlock )
  {
    idMText = 0;
    idBlock = 0;

    DgnImporter* pImporter = OdDgnImportContext::getDgnImporter();

    bool bRestoreTextNodePE = false;

    if( pImporter && !idExternalCell.isNull() )
    {
      bRestoreTextNodePE = pImporter->setTextNodeToMTextImportPE();

      //

      OdDbBlockTableRecordPtr pDimGeometryTmpBlock = OdDbBlockTableRecord::createObject();

      OdDbBlockTablePtr pBlockTable = owner->database()->getBlockTableId().openObject( OdDb::kForWrite );

      if( !pBlockTable.isNull() )
      {
        pDimGeometryTmpBlock->setName( L"*D" );

        pBlockTable->add(pDimGeometryTmpBlock);

        OdDgElementPtr pCellObj = idExternalCell.openObject(OdDg::kForRead);

        OdDgElementIteratorPtr pCellIter;

        OdGePoint3d ptOrigin;

        if( pCellObj->isKindOf( OdDgCellHeader2d::desc() ) )
        {
          OdDgCellHeader2dPtr pCell2d = pCellObj;

          pCellIter = pCell2d->createIterator();

          OdGePoint2d ptOrigin2d = pCell2d->getOrigin();

          ptOrigin.set( ptOrigin2d.x, ptOrigin2d.y, 0 );
        }
        else if( pCellObj->isKindOf( OdDgCellHeader3d::desc() ) )
        {
          OdDgCellHeader3dPtr pCell3d = pCellObj;

          pCellIter = pCell3d->createIterator();

          ptOrigin = pCell3d->getOrigin();
        }

        bool bCreateTextNodeOnly = true;

        if( !pCellIter.isNull() )
        {
          for(; !pCellIter->done(); pCellIter->step() )
          {
            OdDgElementPtr pCellItem = pCellIter->item().openObject(OdDg::kForRead);

            if( pCellItem->getElementType() == OdDgElement::kTypeDimension )
            {
              continue;
            }

            if( pCellItem->getElementType() == OdDgElement::kTypeLine )
            {
              OdGePoint3d ptLineStart, ptLineEnd;

              if( pCellItem->isKindOf( OdDgLine2d::desc() ) )
              {
                OdDgLine2dPtr pLine2d = pCellItem;

                OdGePoint2d ptStart2d = pLine2d->getStartPoint();
                OdGePoint2d ptEnd2d   = pLine2d->getEndPoint();

                ptLineStart.set( ptStart2d.x, ptStart2d.y, 0.0 );
                ptLineEnd.set( ptEnd2d.x, ptEnd2d.y, 0 );
              } 
              else
              {
                OdDgLine3dPtr pLine3d = pCellItem;

                ptLineStart = pLine3d->getStartPoint();
                ptLineEnd   = pLine3d->getEndPoint();
              }

              if( OdZero(ptOrigin.distanceTo( ptLineStart )) || OdZero(ptOrigin.distanceTo( ptLineEnd )) )
              {
                continue;
              }
            }

            if( !pCellItem->isKindOf(OdDgTextNode2d::desc()) && !pCellItem->isKindOf(OdDgTextNode3d::desc()) )
            {
              if( bCreateMTextOnly )
              {
                continue;
              }

              bCreateTextNodeOnly = false;
            }

            ((OdDgnImportPE*)pCellItem->queryX(OdDgnImportPE::desc()))->subImportElement(pCellItem, pDimGeometryTmpBlock);
          }
        }

        if( bCreateTextNodeOnly )
        {
          for (OdDbObjectIteratorPtr pIter = pDimGeometryTmpBlock->newIterator() ; !pIter->done(); pIter->step())
          {
            OdDbEntityPtr elm = pIter->objectId().safeOpenObject(OdDb::kForWrite);

            if( elm->isKindOf(OdDbMText::desc()) )
            {
              idMText = pIter->objectId();
              break;
            }
            else if( elm->isKindOf( OdDbText::desc()) )
            {
              idMText = createMText( elm, pDimGeometryTmpBlock );
            }
          }

          pDimGeometryTmpBlock->erase(true);
        }
        else
        {
          idBlock = pDimGeometryTmpBlock->objectId();
        }
      }

      //

      if( bRestoreTextNodePE)
      {
        pImporter->setTextNodeToEntitySetImportPE();
      }
    }

    return !idMText.isNull() || !idBlock.isNull();
  }

//-----------------------------------------------------------------------------------------------

  OdDbObjectId getDimNoteInternalMText( OdDgDimNote* pDim, OdDbBlockTableRecord* owner )
  {
    OdDbObjectId idMText;

    DgnImporter* pImporter = OdDgnImportContext::getDgnImporter();

    bool bRestoreTextNodePE = false;

    if( pImporter && pDim )
    {
      bRestoreTextNodePE = pImporter->setTextNodeToMTextImportPE();

      //

      OdRxObjectPtrArray arrDimExplode;

      pDim->explodeGeometry( arrDimExplode );

      OdDgElementPtr pTextNode;

      for( OdUInt32 k = 0; k < arrDimExplode.size(); k++ )
      {
        if( arrDimExplode[k]->isKindOf(OdDgTextNode3d::desc()) || arrDimExplode[k]->isKindOf(OdDgTextNode2d::desc()) ||
            arrDimExplode[k]->isKindOf(OdDgText3d::desc())     || arrDimExplode[k]->isKindOf(OdDgTextNode2d::desc())
          )
        {
          pTextNode = arrDimExplode[k];
          break;
        }
      }

      if( !pTextNode.isNull() )
      {
        OdDgGraphicsElementPtr pGrText = pTextNode;

        OdUInt32 uOldColor  = pGrText->getColorIndex();
        pTextNode->setPropertiesFrom( pDim );
        pGrText->setColorIndex( uOldColor );

        OdDbBlockTableRecordPtr pDimGeometryTmpBlock = OdDbBlockTableRecord::createObject();

        OdDbBlockTablePtr pBlockTable = owner->database()->getBlockTableId().openObject( OdDb::kForWrite );

        if( !pBlockTable.isNull() )
        {
          pDimGeometryTmpBlock->setName( L"*D" );

          pBlockTable->add(pDimGeometryTmpBlock);

          ((OdDgnImportPE*)pTextNode->queryX(OdDgnImportPE::desc()))->subImportElement(pTextNode, pDimGeometryTmpBlock);

          for (OdDbObjectIteratorPtr pIter = pDimGeometryTmpBlock->newIterator() ; !pIter->done(); pIter->step())
          {
            OdDbEntityPtr elm = pIter->objectId().safeOpenObject(OdDb::kForWrite);

            if( elm->isKindOf(OdDbMText::desc()) )
            {
              idMText = pIter->objectId();
              break;
            }
            else if( elm->isKindOf( OdDbText::desc()) )
            {
              idMText = createMText( elm, pDimGeometryTmpBlock );
            }
          }
        }

        pDimGeometryTmpBlock->erase(true);
      }

      //

      if( bRestoreTextNodePE)
      {
        pImporter->setTextNodeToEntitySetImportPE();
      }
    }

    return idMText;
  }

//-----------------------------------------------------------------------------------------------

OdDbMLeaderStyle::TextAttachmentType convertTextAttachment( OdDgDimNote::NoteAttachmentAlignment iAlignment,
                                                            bool bMultiRowMText
                                                          )
{
  OdDbMLeaderStyle::TextAttachmentType retVal = OdDbMLeaderStyle::kAttachmentMiddle;

  if( bMultiRowMText )
  {
    switch( iAlignment )
    {
      case OdDgDimNote::kAlignTop:
      {
        retVal = OdDbMLeaderStyle::kAttachmentTopOfTop;
      } break;

      case OdDgDimNote::kAlignBottom:
      {
        retVal = OdDbMLeaderStyle::kAttachmentBottomLine;
      } break;

      case OdDgDimNote::kAlignFirstLine:
      {
        retVal = OdDbMLeaderStyle::kAttachmentMiddleOfTop;
      } break;

      case OdDgDimNote::kAlignMiddle:
      {
        retVal = OdDbMLeaderStyle::kAttachmentMiddle;
      } break;

      case OdDgDimNote::kAlignLastLine:
      {
        retVal = OdDbMLeaderStyle::kAttachmentMiddleOfBottom;
      } break;

      case OdDgDimNote::kAlignDynamicLine:
      {
        retVal = OdDbMLeaderStyle::kAttachmentMiddle;
      } break;

      case OdDgDimNote::kAlignDynamicCorner:
      {
        retVal = OdDbMLeaderStyle::kAttachmentMiddle;
      } break;

      case OdDgDimNote::kAlignUnderline:
      {
        retVal = OdDbMLeaderStyle::kAttachmentBottomOfBottom;
      } break;
    }
  }
  else
  {
    switch( iAlignment )
    {
      case OdDgDimNote::kAlignTop:
      {
        retVal = OdDbMLeaderStyle::kAttachmentTopOfTop;
      } break;

      case OdDgDimNote::kAlignBottom:
      {
        retVal = OdDbMLeaderStyle::kAttachmentBottomLine;
      } break;
    }
  }

  return retVal;
}

OdDbMLeaderStyle::LeaderDirectionType convertLeaderDirection( OdDgDimNote::NoteAttachmentDirection iDirection )
{
  OdDbMLeaderStyle::LeaderDirectionType retVal = OdDbMLeaderStyle::kUnknownLeader;

  switch( iDirection )
  {
    case OdDgDimNote::kDirectionAuto:
    {
      retVal = OdDbMLeaderStyle::kLeftLeader;
    } break;

    case OdDgDimNote::kDirectionLeft:
    {
      retVal = OdDbMLeaderStyle::kLeftLeader;
    } break;

    case OdDgDimNote::kDirectionRight:
    {
      retVal = OdDbMLeaderStyle::kRightLeader;
    } break;
  }

  return retVal;
}

//-----------------------------------------------------------------------------------------------

  void OdDgnNoteDimensionImportPE::subImportElement(OdDgElement* e, OdDbBlockTableRecord* owner)
  {
    OdDgDimNote* pDim = (OdDgDimNote*)(e);

    OdUInt32 numPts = pDim->getPointsCount();
    OdDgDimensionExportPEPtr pDgDimPE =  OdDgDimensionExportPEPtr(OdRxObjectPtr(pDim));

    if( numPts < 2 || pDgDimPE.isNull())
    {
      return;
    }

    bool bExternalContext = false;

    OdDgElementId idExternalCell = getDimNoteExternalContext( pDim );

    if( !idExternalCell.isNull() )
    {
      bExternalContext = true;
    }

    OdDgDimPoint ptDim;
    OdDgDimRotation quatView;

    getDimensionViewOptionData( pDim, quatView );

    OdDgDimRotation quatDim(pDim);

    OdDgDimTextInfo txtInfo;
    pDim->getDimTextInfo(txtInfo);

    OdDgDimTool::TerminatorType iTermType = OdDgDimTool::kTtArrow;
    OdDgDimTextInfo::TextRotation iTextRot = OdDgDimTextInfo::kInline;

    OdDgDimGeometry geom;
    pDim->getDimGeometry(geom);

    double  dLeftMargin   = geom.getTextMargin();
    double  dLeaderLength = 0;

    bool bSplineFit = false;

    iTermType  = pDim->getTerminator();
    iTextRot   = pDim->getTextRotation();
    bSplineFit = pDim->getSplineFit();

    if( pDim->getHorizontalFlag() )
    {
      iTextRot = OdDgDimTextInfo::kHorizontal;
    }
    else if( iTextRot != OdDgDimTextInfo::kVertical )
    {
      iTextRot = OdDgDimTextInfo::kInline;
    }

    pDim->getLeftMargin(dLeftMargin);

    if( iTextRot != OdDgDimTextInfo::kVertical )
    {
      dLeaderLength = txtInfo.getWidth()*2.0;
    }

    double dDimLeaderLength = 0;

    if( pDim->getLeaderLength(dDimLeaderLength) )
    {
      if( OdZero( dDimLeaderLength ) )
      {
        dLeaderLength = 0;
      }
    }

    if( !pDim->getDimNoteInlineLeaderFlag() )
    {
      dLeaderLength = 0;
    }

    OdGeVector3d vrNormal = OdGeVector3d::kZAxis;
    OdGeVector3d vrXAxis  = OdGeVector3d::kXAxis;
    OdGeVector3d vrYAxis  = OdGeVector3d::kYAxis;

    vrNormal = quatDim.rotate( vrNormal );
    vrXAxis  = quatView.rotate( vrXAxis );
    vrYAxis  = quatView.rotate( vrYAxis );

    vrXAxis.orthoProject(vrNormal);
    vrYAxis.orthoProject(vrNormal);

    if( !OdZero(vrXAxis.length()) )
    {
      vrXAxis.normalize();

      vrYAxis = vrXAxis;
      vrYAxis.rotateBy(OdaPI2, vrNormal);
    }
    else
    {
      vrYAxis.normalize();

      vrXAxis = vrYAxis;
      vrXAxis.rotateBy(-OdaPI2, vrNormal);
    }

    OdGePoint3dArray arrDimPts;
    ptDim = pDim->getPoint( 0 );

    arrDimPts.push_back( ptDim.getPoint() );

    for( OdUInt32 n = 1; n < numPts; n++ )
    {
      ptDim = pDim->getPoint( n );
      arrDimPts.push_back( ptDim.getPoint() );
    }

    OdGeVector3d vrDimDirection = OdGeVector3d::kXAxis;

    if( arrDimPts.size() > 1)
    {
      vrDimDirection = arrDimPts[ arrDimPts.size() - 1 ] - arrDimPts[ arrDimPts.size()- 2];

      if( OdZero( vrDimDirection.length()) )
      {
        vrDimDirection = OdGeVector3d::kXAxis;
      }
      else
      {
        vrDimDirection.normalize();
      }
    }

    OdDbObjectId idMText;
    OdDbObjectId idBlock;

    if( bExternalContext )
    {
      bool bCreateMTextOnly = true; //false;

      if( (pDim->getTextRotation() == OdDgDimTextInfo::kVertical) ||
          (pDim->getNoteFrameType() == OdDgDimNote::kFrameBox)
        )
      {
        bCreateMTextOnly = true;
      }

      getDimNoteExternalMText( idExternalCell, owner, bCreateMTextOnly, 
                               idMText, idBlock );
      OdDgnImportContext::addNoteDimensionCell(idExternalCell);
    }
    else
    {
      idMText = getDimNoteInternalMText( pDim, owner );
    }

    // Create MLeader

    OdDbMLeaderPtr pMLeader = OdDbMLeader::createObject();
    pMLeader->setDatabaseDefaults( owner->database() );
    copyEntityProperties(pDim, pMLeader);

    OdGePlane curPlane( OdGePoint3d(0,0,0), vrNormal ); 
    pMLeader->setPlane( curPlane );

    pMLeader->setContentType(OdDbMLeaderStyle::kMTextContent );

    int uLeaderLineIndex = 0;

    bool bSetZeroDglLength = false;
    bool bMultiRowMText = false;

    OdDgDimTextInfo textInfo;
    pDim->getDimTextInfo( textInfo );

    double dLandingGap = 0.0;
    pDim->getLeftMargin(dLandingGap);
    pMLeader->setLandingGap(dLandingGap * textInfo.getHeight());

    if( !idMText.isNull() )
    {
      OdDbMTextPtr pMText = idMText.safeOpenObject(OdDb::kForWrite);
      pMText->setWidth(0);
      pMText->setAttachmentMovingLocation( OdDbMText::kTopLeft );

      OdString strMTextContents = pMText->contents();
      strMTextContents.makeUpper();

      if( strMTextContents.find(L"\\P") != -1 )
      {
        strMTextContents = strMTextContents.left( strMTextContents.getLength() - 2 );

        if( strMTextContents.find(L"\\P") != -1 )
        {
          bMultiRowMText = true;
        }
      }

      pMText->downgradeOpen();

      pMLeader->setMText( pMText.get() );
      pMLeader->setTextHeight( pMText->textHeight() );
    }
    else if( !idBlock.isNull() )
    {
      pMLeader->setContentType(OdDbMLeaderStyle::kBlockContent );
      pMLeader->setBlockContentId( idBlock );
    }
    else
    {
      pMLeader->setLandingGap(0.0);
      OdDbMTextPtr pMText = OdDbMText::createObject();
      pMText->setDatabaseDefaults( owner->database() );
      pMText->setLocation( arrDimPts[ arrDimPts.size()-1 ] );
      pMText->setWidth(0);
      pMText->setAttachmentMovingLocation( OdDbMText::kTopLeft );
      pMText->downgradeOpen();
      pMLeader->setMText( pMText.get() );
      pMLeader->setTextHeight( pMText->textHeight() );
      bSetZeroDglLength = true;
    }

    // Create or find MLeader style 

    OdDbDatabase* pDwgDb = owner->database();
    OdDbDictionaryPtr pMLeaderStyleDic = pDwgDb->getMLeaderStyleDictionaryId().safeOpenObject(OdDb::kForWrite);

    double dTextHeight   = pMLeader->textHeight();

    OdDbDictionaryIteratorPtr pMStyleIter = pMLeaderStyleDic->newIterator();

    OdDbObjectId idMLStyle;

    OdUInt32 uMLStyleIndexToCreate = 0;

    for(; !pMStyleIter->done(); pMStyleIter->next() )
    {
      OdDbMLeaderStylePtr pCurMLeaderStyle = pMStyleIter->getObject(OdDb::kForRead);

      if( !pCurMLeaderStyle.isNull() )
      {
        if( OdZero(pCurMLeaderStyle->textHeight() - dTextHeight) && OdZero(pCurMLeaderStyle->doglegLength() - dLeaderLength) )
        {
          idMLStyle = pCurMLeaderStyle->objectId();
          break;
        }

        OdString strName = pCurMLeaderStyle->getName();

        if( strName.left(18) == OD_T("CustomMLeaderStyle") )
        {
          strName = strName.right(strName.getLength() - 18 );
          OdUInt32 uCurStyleIndex = odStrToUInt(strName.c_str()) + 1;

          if( uCurStyleIndex > uMLStyleIndexToCreate )
          {
            uMLStyleIndexToCreate = uCurStyleIndex;
          }
        }
      }
    }

    if( idMLStyle.isNull() )
    {
      OdDbMLeaderStylePtr pMLeaderStyle = OdDbMLeaderStyle::createObject();
      pMLeaderStyle->setDatabaseDefaults(pDwgDb);
      pMLeaderStyle->setTextHeight( dTextHeight );
      pMLeaderStyle->setDoglegLength( dLeaderLength );
      OdString strNewStyleName;
      strNewStyleName.format(OD_T("CustomMLeaderStyle%d"), uMLStyleIndexToCreate );
      pMLeaderStyleDic->setAt(strNewStyleName, pMLeaderStyle );
      idMLStyle = pMLeaderStyle->objectId();
    }

    pMLeader->setMLeaderStyle( idMLStyle );

    //

    if( numPts > 0 )
    {
      pMLeader->addLeaderLine( arrDimPts[numPts-1], uLeaderLineIndex);

      if( bSetZeroDglLength )
      {
        pMLeader->setDoglegLength( uLeaderLineIndex, 0 );
      }

      OdUInt32 uVertexCount = 0;

      for( int k = numPts-2; k >= 0; k-- )
      {
        if( uVertexCount == 0 )
        {
          pMLeader->setFirstVertex( uLeaderLineIndex, arrDimPts[k] );
        }
        else
        {
          pMLeader->addFirstVertex( uLeaderLineIndex, arrDimPts[k] );
        }

        uVertexCount++;
      }

      if (!bSetZeroDglLength)
      {
        pMLeader->setDoglegLength( uLeaderLineIndex, dLeaderLength );
        pMLeader->setEnableDogleg(true);
      }
      else
        pMLeader->setEnableDogleg(false);
    }
    else
      pMLeader->setEnableDogleg(false);

    OdDgDimNote::NoteFrameType uFrameType = pDim->getNoteFrameType();

    bool bTextFrame = (uFrameType != OdDgDimNote::kFrameNone) && (uFrameType != OdDgDimNote::kFrameLine);

    pMLeader->setEnableFrameText( bTextFrame );

    if( iTextRot == OdDgDimTextInfo::kVertical )
    {
      pMLeader->setTextAttachmentDirection(OdDbMLeaderStyle::kAttachmentVertical);
    }
    else
    {
      pMLeader->setTextAttachmentDirection(OdDbMLeaderStyle::kAttachmentHorizontal);
    }

    OdDgDimension::TerminatorArrowHeadType atype = pDim->getArrowHead();

    if( atype > 2 )
    {
      atype = (OdDgDimension::TerminatorArrowHeadType)(atype & OdDgDimension::kClosed); 
    }

    OdDgModelPtr pDimensionModel;

    OdDgElementId idOwner = pDim->ownerId();

    while( !idOwner.isNull() )
    {
      OdDgElementPtr pOwner = idOwner.openObject( OdDg::kForRead );

      if( pOwner->isKindOf( OdDgModel::desc() ) )
      {
        pDimensionModel = pOwner;
        break;
      }

      idOwner = pOwner->ownerId();
    }

    double dUORtoMasterScaleFactor = 1.0;

    if( pDimensionModel.isNull() )
    {
      OdDgElementId idModel = pDim->database()->getActiveModelId();

      if( !idModel.isNull() )
      {
        pDimensionModel = idModel.openObject(OdDg::kForRead);
      }
    }

    if( !pDimensionModel.isNull() )
    {
      dUORtoMasterScaleFactor = pDimensionModel->convertUORsToWorkingUnits(1.0);
    }

    double dArrowSize  = geom.getTerminatorHeight() > geom.getTerminatorWidth() ? geom.getTerminatorHeight() : geom.getTerminatorWidth();

    OdDbObjectId idArrow = getDimensionArrowBlock( iTermType, atype, pDim, owner->database(), dUORtoMasterScaleFactor, dArrowSize );

    pMLeader->setArrowSymbolId( idArrow );
    pMLeader->setArrowSize( dArrowSize );

    OdGePoint3d ptOldTextPos;

    if( pMLeader->contentType() != OdDbMLeaderStyle::kBlockContent )
    {
      pMLeader->getTextLocation(ptOldTextPos);
    }

    pMLeader->setTextAttachmentType( convertTextAttachment( pDim->getNoteLeftAttachmentAlignment(), bMultiRowMText ), OdDbMLeaderStyle::kLeftLeader );
    pMLeader->setTextAttachmentType( convertTextAttachment( pDim->getNoteRightAttachmentAlignment(), bMultiRowMText ), OdDbMLeaderStyle::kRightLeader );
    pMLeader->setTextAttachmentType( convertTextAttachment( pDim->getNoteLeftAttachmentAlignment(), bMultiRowMText ), OdDbMLeaderStyle::kTopLeader );
    pMLeader->setTextAttachmentType( convertTextAttachment( pDim->getNoteRightAttachmentAlignment(), bMultiRowMText ), OdDbMLeaderStyle::kBottomLeader );

    if( pMLeader->contentType() != OdDbMLeaderStyle::kBlockContent )
    {
      pMLeader->setTextLocation( ptOldTextPos );
    }

    pMLeader->setLeaderLineType( uLeaderLineIndex, pDim->getSplineFit() ? OdDbMLeaderStyle::kSplineLeader : OdDbMLeaderStyle::kStraightLeader );

    owner->appendOdDbEntity( pMLeader );

    OdDgnImportPathToDwgObject dwgPath;
    dwgPath.m_idPath.objectIds().push_back( pMLeader->objectId() );
    dwgPath.m_bExists = true;
    OdDgnImportContext::addObjectPath( e->elementId(), dwgPath );
  }

//------------------------------------------------------------------------------

};
