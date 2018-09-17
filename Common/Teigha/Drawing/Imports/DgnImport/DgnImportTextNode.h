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

#ifndef _DGN_IMPORT_TEXTNODE_INCLUDED_ 
#define _DGN_IMPORT_TEXTNODE_INCLUDED_

#include "DgnImportPE.h"
#include "DgnImportCommon.h"
#include <OdaCommon.h>
#include <DbText.h>
#include "DgText.h"
#include <DgFontTableRecord.h>
#include <DbTextStyleTable.h>
#include <DbTextStyleTableRecord.h>
#include <DgTextNode.h>
#include <Gi/GiTextStyle.h>
#include <DgTextStyleTableRecord.h>

namespace TD_DGN_IMPORT 
{

//---------------------------------------------------------------------------------------------------

template <class T> OdGePoint3d getMTextPosition(T*){return OdGePoint3d::kOrigin;}

//---------------------------------------------------------------------------------------------------

template <> OdGePoint3d getMTextPosition<OdDgTextNode2d>(OdDgTextNode2d* textNode2d)
{
  OdGePoint2d ptPos = textNode2d->getOrigin();

  return OdGePoint3d( ptPos.x, ptPos.y, 0 );
}

//---------------------------------------------------------------------------------------------------

template <> OdGePoint3d getMTextPosition<OdDgTextNode3d>(OdDgTextNode3d* textNode3d)
{
  return textNode3d->getOrigin();
}

//===================================================================================================

template <class T> OdGeVector3d getMTextNormal(T*){return OdGeVector3d::kZAxis;}

//---------------------------------------------------------------------------------------------------

template <> OdGeVector3d getMTextNormal<OdDgTextNode2d>(OdDgTextNode2d* textNode2d)
{
  return OdGeVector3d::kZAxis;
}

//---------------------------------------------------------------------------------------------------

template <> OdGeVector3d getMTextNormal<OdDgTextNode3d>(OdDgTextNode3d* textNode3d)
{
  OdGeMatrix3d matTransform = textNode3d->getRotation().getMatrix();

  OdGeVector3d vrZAxis = OdGeVector3d::kZAxis;

  vrZAxis = vrZAxis.transformBy(matTransform);

  if( !OdZero(vrZAxis.length()) )
  {
    vrZAxis.normalize();
  }
  else
  {
    vrZAxis = OdGeVector3d::kZAxis;
  }

  return vrZAxis;
}

//===================================================================================================

template <class T> double getMTextRotAngle(T*){return 0;}

//---------------------------------------------------------------------------------------------------

template <> double getMTextRotAngle<OdDgTextNode2d>(OdDgTextNode2d* textNode2d)
{
  return textNode2d->getRotation();
}

//---------------------------------------------------------------------------------------------------

template <> double getMTextRotAngle<OdDgTextNode3d>(OdDgTextNode3d* textNode3d)
{
  OdGeMatrix3d matTransform = textNode3d->getRotation().getMatrix();

  OdGeVector3d vrZAxis = OdGeVector3d::kZAxis;
  OdGeVector3d vrXAxis = OdGeVector3d::kXAxis;

  vrZAxis = vrZAxis.transformBy(matTransform);
  vrXAxis = vrXAxis.transformBy(matTransform);

  if( !OdZero(vrZAxis.length()) )
  {
    vrZAxis.normalize();
  }
  else
  {
    vrZAxis = OdGeVector3d::kZAxis;
  }

  if( !OdZero(vrXAxis.length()) )
  {
    vrXAxis.normalize();
  }
  else
  {
    vrXAxis = OdGeVector3d::kXAxis;
  }

  return OdGeMatrix3d::planeToWorld(vrZAxis).getCsXAxis().angleTo(vrXAxis, vrZAxis);
}

//============================================================================================

template <class T> OdString addTabSpaces( T* pTextNode )
{
  OdString strRet = OdString::kEmpty;

  OdDgTextIndentationLinkagePtr pTextLinkage;

  OdRxObjectPtrArray arrLinkages;

  pTextNode->getLinkages( OdDgAttributeLinkage::kTextIndentation, arrLinkages );

  if( arrLinkages.size() > 0 )
  {
    pTextLinkage = arrLinkages[0];
  }
  else
  {
    OdDgElementIteratorPtr pTextIter = pTextNode->createIterator();

    for(; !pTextIter->done(); pTextIter->step() )
    {
      OdDgElementPtr pElm = pTextIter->item().openObject(OdDg::kForRead);

      pElm->getLinkages( OdDgAttributeLinkage::kTextIndentation, arrLinkages );

      if( arrLinkages.size() > 0 )
      {
        pTextLinkage = arrLinkages[0];
        break;
      }
    }
  }

  if( !pTextLinkage.isNull() )
  {
    double dScaleFactor = 1.0;

    OdDgModelPtr pModel;

    OdDgElementId idOwner = pTextNode->ownerId();

    while( !idOwner.isNull() )
    {
      OdDgElementPtr pOwner = idOwner.openObject(OdDg::kForRead);

      if( pOwner.isNull() )
      {
        break;
      }

      if( pOwner->isKindOf(OdDgModel::desc()) )
      {
        pModel = pOwner;
        break;
      }

      idOwner = pOwner->ownerId();
    }

    if( !pModel.isNull() )
    {
      dScaleFactor = pModel->convertUORsToWorkingUnits( 1.0 );
    }

    OdArray<double> dTabStopArr;
    OdUInt32 i = 0;

    for( i = 0; i < pTextLinkage->getTabCount(); i++ )
    {
      dTabStopArr.push_back( pTextLinkage->getTabValue(i) * dScaleFactor );
    }

    for( i = 0; i < dTabStopArr.size(); i++ )
    {
      double dCurTabStop = dTabStopArr[i];

      for( OdUInt32 j = i+1; j < dTabStopArr.size(); j++ )
      {
        if( dTabStopArr[j] < dCurTabStop )
        {
          dCurTabStop = dTabStopArr[j];
          dTabStopArr[j] = dTabStopArr[i];
          dTabStopArr[i] = dCurTabStop;
        }
      }
    }

    if( dTabStopArr.size() > 0 )
    {
      strRet += L"\\pt ";

      for( i = 0; i < dTabStopArr.size(); i++ )
      {
        OdString strValue;
        strValue.format(L"%f", dTabStopArr[i]);
        strValue.replace(L',', L'.');

        if( strValue.find(L'.') != -1 )
        {
          while( strValue[strValue.getLength() - 1] == L'0')
          {
            strValue = strValue.left(strValue.getLength() - 1);
          }

          if( strValue[strValue.getLength() - 1] == L'.' )
          {
            strValue = strValue.left(strValue.getLength() - 1);
          }

          if( strValue.isEmpty() )
          {
            strValue = L"0";
          }
        }

        strRet += strValue + L",";
      }

      strRet = strRet.left(strRet.getLength() - 1);
      strRet += L";";
    }
  }

  return strRet;
}

//---------------------------------------------------------------------------------------------------

struct OdDgTextNodeImportData
{
  bool        m_bUnderlineText;
  bool        m_bOverlineText;
  bool        m_bUseRGBColor;
  bool        m_bUseBold;
  bool        m_bUseItalic;
  OdUInt32    m_uColorIndex;
  ODCOLORREF  m_uRGBColor;
  OdString    m_strFontName;
  double      m_dTextSize;
  double      m_dCharSpacing;
  double      m_dWidthFactor;
  double      m_dSlantAngle;
};

//---------------------------------------------------------------------------------------------------

OdString addTextNodeSpecialSymbols( const OdDgElement* pText )
{
  OdString strRet = OdString::kEmpty;

  if( !pText )
  {
    return strRet;
  }

  OdRxObjectPtrArray arrLinkages;

  pText->getLinkages( OdDgAttributeLinkage::kBitMaskLinkage, arrLinkages );

  for( OdUInt32 iLink = 0; iLink < arrLinkages.size(); iLink++ )
  {
    if( arrLinkages[iLink]->isKindOf( OdDgTextWhiteSpaceLinkage::desc()) )
    {
      OdDgTextWhiteSpaceLinkagePtr pWhiteSpaceLinkage = arrLinkages[iLink];
      OdString strSpaces = pWhiteSpaceLinkage->createString();
      strSpaces.replace(L"\\t",L"\t");
      strSpaces.replace(L"\\n",L"\\P");
      strSpaces.replace(L"\\p",L"\\P");
      strRet += strSpaces;
    }
  }

  return strRet;
}

//---------------------------------------------------------------------------------------------------

OdCmColor convertColor( OdDgGraphicsElement* pElm )
{
  OdCmColor retVal;

  OdUInt32 uColorIndex = pElm->getColorIndex();

  OdDgViewPtr pActiveView = OdDgnImportContext::getActiveView();

  if( !pActiveView.isNull() && pActiveView->getShowLevelSymbologyFlag() )
  {
    OdUInt32 uTmp1;
    OdInt32  uTmp2;
    applyLevelSymbologyOverrides( pElm->getLevelId(), uColorIndex, uTmp1, uTmp2 );
  }

  setDwgColorByDgnIndex( pElm->database(), retVal, uColorIndex );

  return retVal;
}

//---------------------------------------------------------------------------------------------------

template <class T>  OdString getDgnTextFontName( T* pText, bool& bTTF )
{
  OdString strRet;

  OdDgDatabase* pDgnDb = pText->database();

  OdDgFontTablePtr pFontTable = pDgnDb->getFontTable();

  OdDgFontTableRecordPtr pFont = pFontTable->getFont( pText->getFontEntryId() );

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

//---------------------------------------------------------------------------------------------------

template <class T> OdString addTextItem(T* pText, OdDgTextNodeImportData& textData )
{
  OdString strRet = OdString::kEmpty;

  // Prepare data

  double dSlantAngle   = pText->getSlant();
  double dTextSize     = fabs(pText->getHeightMultiplier());
  double dWidthFactor  = 1.0;
  
  if( !OdZero(dTextSize) )
  {
    dWidthFactor = fabs(pText->getLengthMultiplier()) / dTextSize;
  }

  double dCharSpacing = 0;
  OdDgTextExtendedProperties::Spacing uSpacing = OdDgTextExtendedProperties::sInterCharSpacing;

  pText->getSpacing(dCharSpacing,uSpacing);

  double dSpaceLength = fabs(pText->getLengthMultiplier()) * 0.7;
  double dTrackingPercent = (dCharSpacing+dSpaceLength)/dSpaceLength;

  bool bUnderline      = pText->getUnderlineFlag();
  bool bOverline       = pText->getOverlineFlag();
  bool bBold           = pText->getBoldFlag();
  bool bItalic         = pText->getSlantFlag() && OdZero(dSlantAngle);

  bool      bUseRgbColor    = true;
  OdUInt32  uColorIndex     = 0;
  OdUInt32  uRGBColor       = 0;

  OdCmColor textColor = convertColor( pText );

  if( textColor.colorMethod() == OdCmEntityColor::kByACI )
  {
    bUseRgbColor = false;
    uColorIndex  = textColor.colorIndex();
  }
  else if( textColor.colorMethod() == OdCmEntityColor::kByLayer )
  {
    bUseRgbColor = false;
    uColorIndex  = OdCmEntityColor::kACIbyLayer;
  }
  else
  {
    bUseRgbColor = true;
    uRGBColor    = ODRGB( textColor.red(), textColor.green(), textColor.blue() );
  }

  bool     bTTF = false;
  OdString strFontName = getDgnTextFontName( pText, bTTF );

  if( strFontName.isEmpty() )
  {
    strFontName = textData.m_strFontName;
  }

  // Set options

  bool bChangeFont = false;

  if( strFontName != textData.m_strFontName )
  {
    bChangeFont = true;
  }
  else if( bTTF )
  {
    if( (bBold != textData.m_bUseBold) || (bItalic != textData.m_bUseItalic) )
    {
      bChangeFont = true;
    }
  }

  if( bChangeFont )
  {
    if( bTTF )
    {
      strRet += L"\\f" + strFontName;

      if( bBold )
      {
        strRet += L"|b1";
      }
      else
      {
        strRet += L"|b0";
      }

      if( bItalic )
      {
        strRet += L"|i1";
      }
      else
      {
        strRet += L"|i0";
      }

      OdUInt32 uCharset = codePageToCharset(pText->getCodePage());

      OdString strCharset;
      strCharset.format(L"|c%03d", uCharset );

      strRet += strCharset;

      strRet += L"|p0;";
    }
    else if( OdDgnImportContext::getDgnImporter() )
    {
      strRet += L"\\F" + strFontName + L".shx;";

      OdDgnImportContext::getDgnImporter()->createShxFontFromRscFont( strFontName );
    }

    textData.m_strFontName = strFontName;
    textData.m_bUseBold    = bBold;
    textData.m_bUseItalic  = bItalic;
  }

  if( bUnderline != textData.m_bUnderlineText )
  {
    if( bUnderline )
    {
      strRet += L"\\L";
    }
    else
    {
      strRet += L"\\l";
    }

    textData.m_bUnderlineText = bUnderline;
  }

  if( bOverline != textData.m_bOverlineText )
  {
    if( bOverline )
    {
      strRet += L"\\O";
    }
    else
    {
      strRet += L"\\o";
    }

    textData.m_bOverlineText = bOverline;
  }

  bool bChangeColor = false;

  if( bUseRgbColor != textData.m_bUseRGBColor )
  {
    bChangeColor = true;
  }
  else if( bUseRgbColor )
  {
    if( (ODGETRED(uRGBColor) != ODGETRED(textData.m_uRGBColor)) ||
        (ODGETGREEN(uRGBColor) != ODGETGREEN(textData.m_uRGBColor)) ||
        (ODGETBLUE(uRGBColor) != ODGETBLUE(textData.m_uRGBColor))
      )
    {
      bChangeColor = true;
    }
  }
  else
  {
    if( uColorIndex != textData.m_uColorIndex )
    {
      bChangeColor = true;
    }
  }

  if( bChangeColor )
  {
    OdString strColor;

    if( bUseRgbColor )
    {
      strColor.format(L"\\c%d;", uRGBColor );
    }
    else
    {
      strColor.format(L"\\C%d;", uColorIndex );
    }

    strRet += strColor;

    textData.m_uColorIndex  = uColorIndex;
    textData.m_uRGBColor    = uRGBColor;
    textData.m_bUseRGBColor = bUseRgbColor;
  }

  if( !OdZero(textData.m_dTextSize - dTextSize, 1e-4 ) )
  {
    OdString strHeight;
    strHeight.format(L"\\H%fx;", dTextSize / textData.m_dTextSize );
    strHeight.replace(L',',L'.');
    strRet += strHeight;

    textData.m_dTextSize = dTextSize;
  }

  if( !OdZero(textData.m_dSlantAngle - dSlantAngle, 1e-4) )
  {
    OdString strAngle;
    strAngle.format(L"\\Q%f;", dSlantAngle * 180 / OdaPI );
    strAngle.replace(L',',L'.');
    strRet += strAngle;

    textData.m_dSlantAngle = dSlantAngle;
  }

  if( !OdZero( textData.m_dWidthFactor - dWidthFactor, 1e-4 ) )
  {
    OdString strWidthFactor;
    strWidthFactor.format(L"\\W%f;", dWidthFactor );
    strWidthFactor.replace(L',',L'.');
    strRet += strWidthFactor;

    textData.m_dWidthFactor = dWidthFactor;
  }

  if( !OdZero( textData.m_dCharSpacing - dTrackingPercent, 1e-4 ) )
  {
    OdString strSpacing;
    strSpacing.format(L"\\T%f;", dTrackingPercent );
    strSpacing.replace(L',',L'.');
    strRet += strSpacing;

    textData.m_dCharSpacing = dTrackingPercent;
  }

  // Set text

  OdString strCurTextData = pText->getText();

  if( !OdDgnImportContext::getConvertEmptyDataFieldsToSpacesFlag() )
  {
    if( pText->getTextEditFieldCount() > 0 )
    {
      for( OdInt32 i = 0; i < pText->getTextEditFieldCount(); i++ )
      {
        OdDgTextEditField txtField = pText->getTextEditFieldAt(i);
        strCurTextData = convertEmptyDataFieldsTo_( strCurTextData, txtField );
      }
    }
  }

  strCurTextData.replace("\\","\\\\");

  strRet += strCurTextData;

  // Set end of paragraph

  if( pText->getCrCount() != 0 )
  {
    strRet += L"\\P";
  }

  return strRet;
}

//===================================================================================================

template <class T> OdString getMTextContents(T*, OdDgTextNodeImportData& textData ){return OdString::kEmpty;}

//---------------------------------------------------------------------------------------------------

template <> OdString getMTextContents<OdDgTextNode2d>(OdDgTextNode2d* textNode2d, OdDgTextNodeImportData& textData)
{
  OdString strRet;

  strRet += addTabSpaces( textNode2d );
  strRet += addTextNodeSpecialSymbols( textNode2d );

  OdDgElementIteratorPtr pIter = textNode2d->createIterator();

  for(; !pIter->done(); pIter->step() )
  {
    OdDgText2dPtr pText2d = pIter->item().openObject(OdDg::kForWrite);

    strRet += addTextItem( pText2d.get(), textData );
    strRet += addTextNodeSpecialSymbols( pText2d );
  }

  return strRet;
}

//---------------------------------------------------------------------------------------------------

template <> OdString getMTextContents<OdDgTextNode3d>(OdDgTextNode3d* textNode3d, OdDgTextNodeImportData& textData)
{
  OdString strRet;

  strRet += addTabSpaces( textNode3d );
  strRet += addTextNodeSpecialSymbols( textNode3d );

  OdDgElementIteratorPtr pIter = textNode3d->createIterator();

  for(; !pIter->done(); pIter->step() )
  {
    OdDgText3dPtr pText3d = pIter->item().openObject(OdDg::kForWrite);

    strRet += addTextItem( pText3d.get(), textData );
    strRet += addTextNodeSpecialSymbols( pText3d );
  }

  return strRet;
}

//---------------------------------------------------------------------------------------------------

template <class T> OdDbObjectId getDbTextStyle(T* pTextNode, double dDefaultTextSize, OdDbDatabase* pDb, OdDgTextNodeImportData& textData )
{

  OdDgDatabase* pDgnDb = pTextNode->database();

  if( !pDb && !pDgnDb )
  {
    return OdDbObjectId::kNull;
  }

  OdUInt32 uTextStyleEntryId = pTextNode->getTextStyleEntryId();

  OdDgTextStyleTableRecordPtr pRecord;

  if( uTextStyleEntryId > 0 )
  {
    OdDgTextStyleTablePtr pTable = pDgnDb->getTextStyleTable();

    OdDgElementId idTextStyleTableRecord = pTable->getAt( uTextStyleEntryId );

    if( !idTextStyleTableRecord.isNull() )
    {
      pRecord = idTextStyleTableRecord.openObject();
    }
  }
  else
  {
    // set default
    pRecord = pDgnDb->getTextStyleTable()->getDefaultData();
  }

  if( pRecord.isNull() )
  {
    pRecord = pDgnDb->getTextStyleTable()->getDefaultData();
  }

  OdGiTextStyle style;

  if( !pRecord.isNull() )
  {
    pRecord->getGiTextStyle(style);
  }

  bool   bVerticalText = pTextNode->getVerticalFlag();
  bool   bBackwards    = pTextNode->getBackwardsFlag();
  bool   bUpsideDown   = pTextNode->getUpsidedownFlag();
  double dTextSize     = fabs(pTextNode->getHeightMultiplier());
  double dXScale       = 1.0;

  if( OdZero(dTextSize) )
  {
    dTextSize = dDefaultTextSize;
  }

  if( !OdZero(dTextSize) )
  {
    dXScale = fabs(pTextNode->getLengthMultiplier()) / dTextSize; 
  }

  if( OdZero(dXScale) )
  {
    dXScale = 1.0;
  }

  double dAngle = pTextNode->getSlant();

  OdString strFontName = OdString::kEmpty;

  style.setBackward(bBackwards);
  style.setUpsideDown(bUpsideDown);
  style.setTextSize( dTextSize );
  style.setXScale( dXScale );
  style.setVertical( bVerticalText );

  bool bBold    = pTextNode->getBoldFlag();
  bool bItalic  = pTextNode->getSlantFlag() && OdZero(dAngle);

  OdDgFontTablePtr pFontTable = pDgnDb->getFontTable();

  OdDgFontTableRecordPtr pFont = pFontTable->getFont( pTextNode->getFontEntryId() );

  if( pFont.isNull() )
  {
    pFont = pFontTable->getFont( pDgnDb->appServices()->getFAST_FONT() );
  }

  if( !pFont.isNull() )
  {
    if( pFont->getType() == kFontTypeTrueType )
    {
      style.setFont(pFont.get()->getName(), bBold, bItalic, 0, 0 );
    }
    else
    {
      style.setFont(pFont.get()->getName(), false, false, 0, 0 );
      style.setFileName( pFont.get()->getName() );
    }

    strFontName = pFont->getName();
  }

  if( pTextNode->getSlantFlag() && !OdZero(dAngle) && !bVerticalText )
  {
    style.setObliquingAngle( dAngle );
  }
  else
  {
    style.setObliquingAngle(0);
  }

  style.loadStyleRec( pDgnDb );

  textData.m_bOverlineText  = false;
  textData.m_bUnderlineText = false;
  textData.m_bUseBold       = bBold;
  textData.m_bUseItalic     = bItalic;
  textData.m_dWidthFactor   = dXScale;
  textData.m_dTextSize      = dTextSize;
  textData.m_strFontName    = strFontName;
  textData.m_dSlantAngle    = dAngle;

  double dCharSpaceing = 0;
  OdDgTextExtendedProperties::Spacing uSpacing = OdDgTextExtendedProperties::sInterCharSpacing;

  pTextNode->getSpacing(dCharSpaceing,uSpacing);

  double dSpaceLength = fabs(pTextNode->getLengthMultiplier()) * 0.7;
  double dTrackingPercent = 1.0;
  
  if( !OdZero(dSpaceLength) )
  {
    dTrackingPercent = (dCharSpaceing+dSpaceLength)/dSpaceLength;
  }

  textData.m_dCharSpacing = dTrackingPercent;

  OdDbTextStyleTablePtr pTable = pDb->getTextStyleTableId().safeOpenObject();

  for (OdDbSymbolTableIteratorPtr pIter = pTable->newIterator(); !pIter->done(); pIter->step())
  {
    OdDbTextStyleTableRecordPtr pRec = pIter->getRecord();
    OdGiTextStyle recStyle;
    giFromDbTextStyle(pRec, recStyle);

    if ( recStyle.getFont() == style.getFont()
      && recStyle.getBigFont() == style.getBigFont()
      && recStyle.isVertical() == style.isVertical()
      && recStyle.isBackward() == style.isBackward()
      && recStyle.isUpsideDown() == style.isUpsideDown()
        )
    {
      OdDgnImportContext::setResourceUsage(pIter->getRecordId());
      return pIter->getRecordId();
    }
  }

  return OdDgnImportContext::createStyleForDbText( pDb, pDgnDb, style );
}

//---------------------------------------------------------------------------------------------------

template <class T > struct OdDgnTextNodeImportPE : OdDgnComplexElementImportPE<T>
{
  static OdDbMText::AttachmentPoint convertTextNodeJustification( OdDg::TextJustification uJust )
  {
    OdDbMText::AttachmentPoint uRet = OdDbMText::kBottomLeft;

    switch( uJust )
    {
      case OdDg::kLeftTop :
      case OdDg::kLeftMarginTop :
      {
        uRet = OdDbMText::kTopLeft;
      } break;

      case OdDg::kLeftCenter :
      case OdDg::kLeftMarginCenter :
      {
        uRet = OdDbMText::kMiddleLeft;
      } break;

      case OdDg::kLeftBottom :
      case OdDg::kLeftMarginBottom :
      case OdDg::kLeftDescender:
      {
        uRet = OdDbMText::kBottomLeft;
      } break;

      case OdDg::kCenterTop :
      {
        uRet = OdDbMText::kTopCenter;
      } break;

      case OdDg::kCenterCenter :
      {
        uRet = OdDbMText::kMiddleCenter;
      } break;

      case OdDg::kCenterBottom :
      case OdDg::kCenterDescender:
      {
          uRet = OdDbMText::kBottomCenter;
      } break;

      case OdDg::kRightTop :
      case OdDg::kRightMarginTop :
      {
        uRet = OdDbMText::kTopRight;
      } break;

      case OdDg::kRightCenter :
      case OdDg::kRightMarginCenter :
      {
        uRet = OdDbMText::kMiddleRight;
      } break;

      case OdDg::kRightBottom :
      case OdDg::kRightMarginBottom :
      case OdDg::kRightDescender:
      {
        uRet = OdDbMText::kBottomRight;
      } break;
    }

    return uRet;
  }

//---------------------------------------------------------------------------------------------------

  void subImportElement(OdDgElement* e, OdDbBlockTableRecord* owner) ODRX_OVERRIDE
  {
    OdDbDatabase* pDb = owner->database();

    T* textNode = static_cast<T*>(e);

    //   Sometimes text nodes can contain non-text elements, may be it's not correct, but we decided to 
    // check text node items before of convert to MText.

    bool bHasWrongElements = false;

    OdDgElementIteratorPtr pTextIter = textNode->createIterator();

    for(; !pTextIter->done(); pTextIter->step() )
    {
      OdDgElementPtr pCheckElm = pTextIter->item().openObject(OdDg::kForRead);

      if( pCheckElm.isNull() || pCheckElm->getElementType() != OdDgElement::kTypeText )
      {
        bHasWrongElements = true;
        break;
      }
    }

    if( bHasWrongElements )
    {
      OdDgnComplexElementImportPE<T>::subImportElement(e, owner);
      return;
    }

    //

    double       dTextSize        = fabs(textNode->getHeightMultiplier());
    double       dSymbolWidth     = fabs(textNode->getLengthMultiplier());

    if( OdZero(dTextSize) )
    {
      OdDgElementIteratorPtr pIter = textNode->createIterator();

      for(; !pIter->done(); pIter->step() )
      {
        OdDgElementPtr pElm = pIter->item().openObject(OdDg::kForRead);

        if( pElm->isKindOf(OdDgText3d::desc()) )
        {
          OdDgText3dPtr pText3d = pElm;

          dTextSize        = fabs(pText3d->getHeightMultiplier());
          dSymbolWidth     = fabs(pText3d->getLengthMultiplier());

          break;
        }
        else if( pElm->isKindOf(OdDgText2d::desc()) )
        {
          OdDgText2dPtr pText2d = pElm;

          dTextSize        = fabs(pText2d->getHeightMultiplier());
          dSymbolWidth     = fabs(pText2d->getLengthMultiplier());

          break;
        }
      }
    }

    if( OdZero(dTextSize) )
    {
      OdDgnComplexElementImportPE<T>::subImportElement(e, owner);
      return;
    }

    OdDgTextNodeImportData curData;

    OdGePoint3d  ptPosition       = getMTextPosition(textNode);
    OdGeVector3d vrNormal         = getMTextNormal(textNode);
    double       dRotAngle        = getMTextRotAngle(textNode);
    OdDbObjectId idTextStyle      = getDbTextStyle(textNode, dTextSize, pDb, curData);

    OdDbMTextPtr pMText = OdDbMText::createObject();
    pMText->setDatabaseDefaults(pDb);

    pMText->setNormal(vrNormal);
    pMText->setAttachment(convertTextNodeJustification(textNode->getJustification()));
    pMText->setLocation(ptPosition);
    pMText->setRotation(dRotAngle);
    pMText->setTextHeight(dTextSize);
    pMText->setTextStyle( idTextStyle );

    OdUInt32 uMaxLength = textNode->getMaxLength();

    if( uMaxLength == 0 )
    {
      uMaxLength = 255;
    }

    if( !OdZero(dSymbolWidth) )
    {
      pMText->setWidth( uMaxLength * dSymbolWidth );
    }
    else
    {
      pMText->setWidth( uMaxLength * dTextSize );
    }

    if( textNode->getLineSpacingType() == 0 )
    {
      pMText->setLineSpacingStyle( OdDb::kExactly );

      double dLineSpacingFactor = (textNode->getLineSpacing() + dTextSize) / dTextSize * 3.0 / 5.0;

      if( dLineSpacingFactor < 0.25 )
      {
        dLineSpacingFactor = 0.25;
      }

      if( dLineSpacingFactor > 4.0 )
      {
        dLineSpacingFactor = 4.0;
      }

      pMText->setLineSpacingFactor( dLineSpacingFactor );
    }
    else
    {
      pMText->setLineSpacingStyle( OdDb::kAtLeast );

      double dLineSpacingFactor = textNode->getLineSpacing();

      if( dLineSpacingFactor < 0.25 )
      {
        dLineSpacingFactor = 0.25;
      }

      if( dLineSpacingFactor > 4.0 )
      {
        dLineSpacingFactor = 4.0;
      }

      pMText->setLineSpacingFactor( dLineSpacingFactor );
    }

    copyEntityProperties( textNode, pMText );

    OdCmColor curColor = pMText->color();

    if( curColor.colorMethod() == OdCmEntityColor::kByColor )
    {
      curData.m_bUseRGBColor = true;
      curData.m_uRGBColor    = curColor.color();
    }
    else if( (curColor.colorMethod() == OdCmEntityColor::kByACI) || (curColor.colorMethod() == OdCmEntityColor::kByDgnIndex) )
    {
      curData.m_bUseRGBColor = false;
      curData.m_uColorIndex  = curColor.colorIndex();
    }
    else if( curColor.colorMethod() == OdCmEntityColor::kByLayer )
    {
      curData.m_bUseRGBColor = false;
      curData.m_uColorIndex  = OdCmEntityColor::kACIbyLayer;
    }
    else
    {
      curData.m_bUseRGBColor = false;
      curData.m_uColorIndex  = OdCmEntityColor::kACIbyBlock;
    }

    OdString     strMTextContents = getMTextContents(textNode, curData);
    pMText->setContents(strMTextContents);

    owner->appendOdDbEntity(pMText);

    if( pMText->actualWidth() < pMText->width() * 0.9 )
    {
      pMText->setWidth(pMText->actualWidth()*1.1);
    }
  }
};

//---------------------------------------------------------------------------------------------------

}
#endif // _DGN_IMPORT_TEXTNODE_INCLUDED_
