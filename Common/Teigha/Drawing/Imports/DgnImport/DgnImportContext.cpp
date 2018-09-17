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
#include <DgDatabase.h>
#include <DgLevelTableRecord.h>
#include "DbSymUtl.h"
#include "DgnImportContext.h"
#include "DgnImportCommon.h"
#include <Gi/GiTextStyle.h>
#include <DbTextStyleTable.h>
#include <DbMlineStyle.h>
#include <DbDictionary.h>
#include "DgMultiline.h"
#include "DgFontTableRecord.h"

namespace TD_DGN_IMPORT {

struct OdDgImportXRefDescriptor
{
  OdString m_strName;
  OdUInt32 m_uNestedDepth;
  OdDgElementId m_idXRef;
};

typedef std::map<OdUInt32,bool>           OdDgnImportLevelMaskDescriptor;
typedef std::map<OdUInt32,bool>::iterator OdDgnImportLevelMaskDescriptorIterator;

class OdDgnImportContextData
{
public:
  OdDgnImportContextData():m_bInternalImport(false), m_bImportCurvesOnly(false), 
                           m_bSkipPatternDraw(false), 
                           m_bImportInvisible(false), m_bInitPolygonImportModes(false),
                           m_3dEllipseImportMode(0), m_2dEllipseImportMode(0),
                           m_3dShapeImportMode(1), m_2dShapeImportMode(0),
                           m_3dSplineImportMode(1), m_2dSplineImportMode(0),
                           m_3dObjectImportMode(0),
                           m_bConvertColorIndexToTrueColor(false),
                           m_bConvertEmptyDataFieldsToSpaces(true)
  {};

public:
  OdArray<std::map<OdUInt32, OdDgnImportColorIndexDescription> > m_paletteStack;
  std::map<OdUInt32, OdDgnImportColorIndexDescription> m_dgnToDwgColorMap;
  std::map<OdString, OdDbObjectId>                     m_dgnModelToBlockDefMap;
  OdArray<double>                                      m_dGlobalLineScaleStack;
  DgnImporter*                                         m_pDgnImporter;
  bool                                                 m_bInternalImport;
  bool                                                 m_bImportCurvesOnly;
  bool                                                 m_bSkipPatternDraw;
  bool                                                 m_bImportInvisible;
  bool                                                 m_bConvertColorIndexToTrueColor;
  bool                                                 m_bConvertEmptyDataFieldsToSpaces;
  OdArray<OdDgnImportLevelMaskDescriptor>              m_pLevelMaskStack;
  OdDgViewPtr                                          m_pActiveView;
  bool                                                 m_bInitPolygonImportModes;
  OdUInt8                                              m_3dEllipseImportMode;
  OdUInt8                                              m_2dEllipseImportMode;
  OdUInt8                                              m_3dShapeImportMode;
  OdUInt8                                              m_3dObjectImportMode;
  OdUInt8                                              m_2dShapeImportMode;
  OdUInt8                                              m_3dSplineImportMode;
  OdUInt8                                              m_2dSplineImportMode;
  OdArray<OdString>                                      m_arrRscFontNames;
  std::map<OdDbObjectId, OdDgnImportMultilineDescriptor> m_mapMultilineStyles;
  std::map<OdUInt64,OdDbObjectId>                        m_mapTableMLStyleIds;
  std::map<OdDbObjectId, OdDgnImportDimAssocDescriptor>  m_mapDimAssocDescripotors;
  std::map<OdDgElementId, OdDgnImportSymbolHatchDescriptor> m_mapSymbolHatch;
  std::map<OdString, OdUInt32>                           m_mapCellNextBlockIndex;
  OdDbObjectIdArray                                      m_arrCellHeaderIds;
  OdArray<OdDgElementId>                               m_arrNoteCellHeaders;
};

OdArray<OdDgnImportContextData>   g_importContextDataArr;
OdArray<OdDgImportXRefDescriptor> g_strXRefNameStack;

static OdDgnImportContextData* getCurrentContext()
{
  if( g_importContextDataArr.size() > 0 )
  {
    return g_importContextDataArr.asArrayPtr() + g_importContextDataArr.size() - 1;
  }
  else
  {
    return NULL;
  }
}

void OdDgnImportContext::pushPalette()
{
  OdDgnImportContextData* pContextData = getCurrentContext();

  if( pContextData )
  {
    pContextData->m_paletteStack.push_back(pContextData->m_dgnToDwgColorMap);
  }
}

void OdDgnImportContext::popPalette()
{
  OdDgnImportContextData* pContextData = getCurrentContext();

  if( pContextData && pContextData->m_paletteStack.size() > 0 )
  {
    pContextData->m_dgnToDwgColorMap = pContextData->m_paletteStack[pContextData->m_paletteStack.size() - 1];
    pContextData->m_paletteStack.removeLast();
  }
}

void OdDgnImportContext::setPalette( OdDgDatabase* pDb )
{
  const ODCOLORREF* pPalette = OdDgColorTable::currentPalette( pDb );

  setPalette( pPalette );
}

void OdDgnImportContext::setPalette( const ODCOLORREF* pPalette )
{
  OdDgnImportContextData* pContextData = getCurrentContext();

  if( pPalette && pContextData )
  {
    for( OdUInt32 i = 0; i < 256; i++ )
    {
      ODCOLORREF curDgnColor = pPalette[i];

      OdDgnImportColorIndexDescription curDwgColor;

      if( (i == 0) && ( ((ODGETRED(curDgnColor) == 255) && (ODGETGREEN(curDgnColor) == 255) && (ODGETBLUE(curDgnColor) == 255)) || 
                        ((ODGETRED(curDgnColor) == 0) && (ODGETGREEN(curDgnColor) == 0) && (ODGETBLUE(curDgnColor) == 0))
                      )
        )
      {
        curDwgColor.m_uDwgColorIndex  = 7;
        curDwgColor.m_bUseTrueColor   = false;
      }
      else
      {
        curDwgColor.m_uDwgColorIndex = OdCmEntityColor::lookUpACI( ODGETRED(curDgnColor), ODGETGREEN(curDgnColor), ODGETBLUE(curDgnColor) );
        curDwgColor.m_clrDwgTrueColor = curDgnColor;

        OdUInt32 curDwgTrueColor   = OdCmEntityColor::lookUpRGB( curDwgColor.m_uDwgColorIndex );

        curDwgTrueColor |= 0xC2000000;

        OdUInt8 uDwgTrueColorRed   = OdCmEntityColor::red( &curDwgTrueColor );
        OdUInt8 uDwgTrueColorGreen =  OdCmEntityColor::green( &curDwgTrueColor );
        OdUInt8 uDwgTrueColorBlue  = OdCmEntityColor::blue( &curDwgTrueColor );

        if( (uDwgTrueColorRed != ODGETRED(curDgnColor)) || (uDwgTrueColorGreen != ODGETGREEN(curDgnColor)) || (uDwgTrueColorBlue != ODGETBLUE(curDgnColor)) )
        {
          curDwgColor.m_bUseTrueColor = true;
        }
        else
        {
          curDwgColor.m_bUseTrueColor = false;
        }
      }

      pContextData->m_dgnToDwgColorMap[ i ] = curDwgColor;
    }
  }
}

void OdDgnImportContext::setBackgroundColor( ODCOLORREF bgColor )
{
  OdDgnImportContextData* pContextData = getCurrentContext();

  if( pContextData )
  {
    std::map<OdUInt32, OdDgnImportColorIndexDescription>::iterator pIter = pContextData->m_dgnToDwgColorMap.find(255);

    if( pIter != pContextData->m_dgnToDwgColorMap.end() )
    {
      OdDgnImportColorIndexDescription curDwgColor;

      curDwgColor.m_uDwgColorIndex = OdCmEntityColor::lookUpACI( ODGETRED(bgColor), ODGETGREEN(bgColor), ODGETBLUE(bgColor) );
      curDwgColor.m_clrDwgTrueColor = bgColor;

      OdUInt32 curDwgTrueColor   = OdCmEntityColor::lookUpRGB( curDwgColor.m_uDwgColorIndex );

      curDwgTrueColor |= 0xC2000000;

      OdUInt8 uDwgTrueColorRed   = OdCmEntityColor::red( &curDwgTrueColor );
      OdUInt8 uDwgTrueColorGreen =  OdCmEntityColor::green( &curDwgTrueColor );
      OdUInt8 uDwgTrueColorBlue  = OdCmEntityColor::blue( &curDwgTrueColor );

      if( (uDwgTrueColorRed != ODGETRED(bgColor)) || (uDwgTrueColorGreen != ODGETGREEN(bgColor)) || (uDwgTrueColorBlue != ODGETBLUE(bgColor)) )
      {
        curDwgColor.m_bUseTrueColor = true;
      }
      else
      {
        curDwgColor.m_bUseTrueColor = false;
      }

      pContextData->m_dgnToDwgColorMap[ 255 ] = curDwgColor;
    }

  }
}

bool OdDgnImportContext::getDwgColor( OdUInt32 uDgnColorIndex, OdDgnImportColorIndexDescription& retColor )
{
  bool bRet = false;

  OdDgnImportContextData* pContextData = getCurrentContext();

  if( pContextData )
  {
    std::map<OdUInt32, OdDgnImportColorIndexDescription>::iterator pIter = pContextData->m_dgnToDwgColorMap.find(uDgnColorIndex);

    if( pIter != pContextData->m_dgnToDwgColorMap.end() )
    {
      bRet = true;

      retColor = pIter->second;
    }
  }

  return bRet;
}

bool OdDgnImportContext::getConvertColorIndexToTrueColorFlag()
{
  bool bRet = false;

  OdDgnImportContextData* pContextData = getCurrentContext();

  if( pContextData )
  {
    bRet = pContextData->m_bConvertColorIndexToTrueColor;
  }

  return bRet;
}

void OdDgnImportContext::setConvertColorIndexToTrueColorFlag( bool bSet )
{
  OdDgnImportContextData* pContextData = getCurrentContext();

  if( pContextData )
  {
    pContextData->m_bConvertColorIndexToTrueColor = bSet;
  }
}

// Reference attachment map
void OdDgnImportContext::addDwgModelBlockId( const OdString& strFileName, 
                                               const OdDgElementId& idModel, 
                                                 const OdDbObjectId& idBlockDefinition )
{
  OdDgnImportContextData* pContextData = getCurrentContext();

  if( pContextData )
  {
    OdString strModelId;
    strModelId.format(L"%08X", (OdUInt64)(idModel.getHandle()) );

    OdString strMapName = strFileName + strModelId;

    pContextData->m_dgnModelToBlockDefMap[strMapName] = idBlockDefinition;
  }
}

void OdDgnImportContext::restoreAfterImport()
{
  if( g_importContextDataArr.size() > 0 )
  {
    g_importContextDataArr.removeLast();
  }
}

void         OdDgnImportContext::prepareToImport( DgnImporter* dgnImporter, bool bCopyLevelMasks )
{
  OdDgnImportContextData newData;
  newData.m_pDgnImporter = dgnImporter;
  newData.m_pActiveView  = getActiveView();

  if( bCopyLevelMasks && g_importContextDataArr.size() > 0 )
  {
    for( OdUInt32 i = 0; i < g_importContextDataArr[g_importContextDataArr.size()-1].m_pLevelMaskStack.size(); i++ )
      newData.m_pLevelMaskStack.push_back( g_importContextDataArr[g_importContextDataArr.size()-1].m_pLevelMaskStack[i] );
  }

  g_importContextDataArr.push_back( newData );
}

DgnImporter* OdDgnImportContext::getDgnImporter()
{
  DgnImporter*        pImporter = NULL;
  OdDgnImportContextData* pContextData = getCurrentContext();

  if( pContextData )
  {
    pImporter = pContextData->m_pDgnImporter;
  }

  return pImporter;
}

bool OdDgnImportContext::getInternalImportFlag()
{
  bool bRet = false;

  OdDgnImportContextData* pContextData = getCurrentContext();

  if( pContextData )
  {
    bRet = pContextData->m_bInternalImport;
  }

  return bRet;
}

void OdDgnImportContext::setInternalImportFlag( bool bSet )
{
  OdDgnImportContextData* pContextData = getCurrentContext();

  if( pContextData )
  {
    pContextData->m_bInternalImport = bSet;
  }
}

OdDbObjectId OdDgnImportContext::getDwgModelBlockId( const OdString& strFileName, 
                                                       const OdDgElementId& idModel,
                                                         const OdString& strLevelMaskExt
                                                    )
{
    OdDbObjectId retVal;

    OdDgnImportContextData* pContextData = getCurrentContext();

    if( pContextData )
    {
      OdString strModelId;
      strModelId.format(L"%08X", (OdUInt64)(idModel.getHandle()) );

      OdString strMapName = strFileName + strModelId;

      if( !strLevelMaskExt.isEmpty() )
      {
        strMapName += strLevelMaskExt;
      }

      std::map<OdString, OdDbObjectId>::iterator pIter = pContextData->m_dgnModelToBlockDefMap.find( strMapName );

      if( pIter != pContextData->m_dgnModelToBlockDefMap.end() )
      {
        retVal = pIter->second;
      }
    }

    return retVal;
}

OdUInt32 OdDgnImportContext::getXRefNestedDepth()
{
  return g_strXRefNameStack.size();
}

void     OdDgnImportContext::pushXRef( const OdString& strFileName, 
                                         const OdDgElementId& idModel, 
                                            const OdDgElementId& idXRef,
                                              OdUInt32 uNestedDepth,
                                                const OdString& strLevelMaskExt
                                      )
{
  OdString strModelId;
  strModelId.format(L"%08X", (OdUInt64)(idModel.getHandle()) );

  OdString strMapName = strFileName + strModelId;

  if( !strLevelMaskExt.isEmpty() )
  {
    strMapName += strLevelMaskExt;
  }

  OdDgImportXRefDescriptor tmp;
  tmp.m_strName = strMapName;
  tmp.m_uNestedDepth = uNestedDepth;
  tmp.m_idXRef = idXRef;

  g_strXRefNameStack.push_back( tmp );
}

void     OdDgnImportContext::popXRef()
{
  if( g_strXRefNameStack.size() > 0 )
  {
    g_strXRefNameStack.removeLast();
  }
}

bool     OdDgnImportContext::checkNestedDepth()
{
  bool bRet = true;

  OdUInt32 uStackLenght = g_strXRefNameStack.size();

  for( OdUInt32 i = 0; i < uStackLenght; i++ )
  {
     if( g_strXRefNameStack[i].m_uNestedDepth < (uStackLenght - i) )
     {
       bRet = false;
       break;
     }
  }

  return bRet;
}

bool     OdDgnImportContext::isXRefInStack( const OdString& strFileName, const OdDgElementId& idModel )
{
  OdDbObjectId retVal;

  OdString strModelId;
  strModelId.format(L"%08X", (OdUInt64)(idModel.getHandle()) );

  OdString strMapName = strFileName + strModelId;

  bool bRet = false;

  for( OdUInt32 i = 0; i < g_strXRefNameStack.size(); i++ )
  {
    if( g_strXRefNameStack[i].m_strName == strMapName )
    {
      bRet = true;
      break;
    }
  }

  return bRet;
}

//---------------------------------------------------------------------------

OdDgElementId OdDgnImportContext::getXRefId( OdUInt32 uIndex )
{
  OdDgElementId idRet;

  if( g_strXRefNameStack.size() > uIndex )
  {
    idRet = g_strXRefNameStack[uIndex].m_idXRef;
  }

  return idRet;
}

//---------------------------------------------------------------------------

bool OdDgnImportContext::getGlobalLineScale( double& dLineScale )
{
  bool bRet = false;

  OdDgnImportContextData* pContextData = getCurrentContext();

  if( pContextData && (pContextData->m_dGlobalLineScaleStack.size() > 0) )
  {

    dLineScale = pContextData->m_dGlobalLineScaleStack[ pContextData->m_dGlobalLineScaleStack.size() - 1];
    bRet = true;
  }

  return bRet;
}

//---------------------------------------------------------------------------

void OdDgnImportContext::pushGlobalLineScale( double dLineScale )
{
  OdDgnImportContextData* pContextData = getCurrentContext();

  if( pContextData )
  {
    pContextData->m_dGlobalLineScaleStack.push_back( dLineScale );
  }
}

//---------------------------------------------------------------------------

void OdDgnImportContext::popGlobalLineScale()
{
  OdDgnImportContextData* pContextData = getCurrentContext();

  if( pContextData && pContextData->m_dGlobalLineScaleStack.size() > 0 )
  {
    pContextData->m_dGlobalLineScaleStack.removeLast();
  }
}

//---------------------------------------------------------------------------

bool OdDgnImportContext::isFontsEqual(const OdFont* pFont1, const OdFont* pFont2)
{
  bool bRet = false;

  if( !pFont1 && !pFont2 )
  {
    bRet = true;
  }
  else if( pFont1 && pFont2 && (pFont1->getFileName() == pFont2->getFileName()) &&
         (pFont1->getFlags() == pFont2->getFlags())
    )
  {
     bRet = true;
  }

  return bRet;
}

//---------------------------------------------------------------------------

bool OdDgnImportContext::getConvertEmptyDataFieldsToSpacesFlag()
{
  bool bRet = true;

  OdDgnImportContextData* pContextData = getCurrentContext();

  if( pContextData )
  {
    bRet = pContextData->m_bConvertEmptyDataFieldsToSpaces;
  }

  return bRet;
}

//---------------------------------------------------------------------------

void OdDgnImportContext::setConvertEmptyDataFieldsToSpacesFlag( bool bSet )
{
  OdDgnImportContextData* pContextData = getCurrentContext();

  if( pContextData )
  {
    pContextData->m_bConvertEmptyDataFieldsToSpaces = bSet;
  }
}

//---------------------------------------------------------------------------

 OdDbObjectId OdDgnImportContext::getStyleForDbText(OdDbDatabase* pDb, const OdGiTextStyle& textStyle)
 {
   if (!pDb)
   {
     return OdDbObjectId::kNull;
   }

   // Try to find existing text style with suitable properties
   // - get a style by name,
   // - find an acceptable style by properties.
   //
   OdDbTextStyleTablePtr pTable = pDb->getTextStyleTableId().safeOpenObject();
   OdDbObjectId styleId;
   if (!textStyle.styleName().isEmpty())
   {
     OdDbTextStyleTableRecordPtr pRec = pTable->getAt(textStyle.styleName(), OdDb::kForRead);
     if (pRec.get())
     {
       OdGiTextStyle recStyle;
       giFromDbTextStyle(pRec, recStyle);
       if ( recStyle.getFont() == textStyle.getFont()
         && recStyle.getBigFont() == textStyle.getBigFont()
         && recStyle.textSize() == textStyle.textSize()
         && recStyle.isVertical() == textStyle.isVertical()
         && recStyle.isBackward() == textStyle.isBackward()
         && recStyle.isUpsideDown() == textStyle.isUpsideDown())
       {
         OdDgnImportContext::setResourceUsage(pRec->objectId());
         return pRec->objectId();
       }
     }
   }

   for (OdDbSymbolTableIteratorPtr pIter = pTable->newIterator(); !pIter->done(); pIter->step())
   {
     OdDbTextStyleTableRecordPtr pRec = pIter->getRecord();
     OdGiTextStyle recStyle;
     giFromDbTextStyle(pRec, recStyle);
     if ( isFontsEqual(recStyle.getFont(), textStyle.getFont())
       && isFontsEqual(recStyle.getBigFont(), textStyle.getBigFont())
       && recStyle.textSize() == textStyle.textSize()
       && recStyle.isVertical() == textStyle.isVertical()
       && recStyle.isBackward() == textStyle.isBackward()
       && recStyle.isUpsideDown() == textStyle.isUpsideDown())
     {
       OdDgnImportContext::setResourceUsage(pIter->getRecordId());
       return pIter->getRecordId();
     }
   }

   return OdDbObjectId::kNull;
 }

//---------------------------------------------------------------------------

OdDbObjectId OdDgnImportContext::createStyleForDbText( OdDbDatabase* pDb, 
                                                         OdDgDatabase* pDgDb, 
                                                           const OdGiTextStyle& textStyle)
{
  OdDbObjectId recId;

  if( !pDb )
  {
    return recId;
  }

  OdDbTextStyleTableRecordPtr pRec = OdDbTextStyleTableRecord::createObject();
  pDb->addOdDbObject(pRec);

  OdString sTypeFace;
  bool bBold = false, bItalic = false;
  int nCharSet = 0, nPitchAndFamily = 0;
  OdGiTextStyle srcStyle(textStyle);
  srcStyle.font(sTypeFace, bBold, bItalic, nCharSet, nPitchAndFamily);

  pRec->setFont(sTypeFace, bBold, bItalic, nCharSet, nPitchAndFamily);

  if(srcStyle.getFont() && srcStyle.getFont()->isShxFont() )
  {
    OdString fileName = srcStyle.ttfdescriptor().fileName();

    if(fileName.isEmpty() )
    {
      pRec->setFileName(srcStyle.ttfdescriptor().typeface());
    }
    else if( OdDgnImportContext::getDgnImporter() )
    {
      OdString strFileExt = fileName;

      if( strFileExt.getLength() > 4 )
      {
        strFileExt = strFileExt.right(4);
        strFileExt.makeUpper();

        if( strFileExt != L".SHX" )
        {
          if( OdDgnImportContext::getDgnImporter() )
            OdDgnImportContext::getDgnImporter()->createShxFontFromRscFont( fileName );
          fileName += L".shx";
        }
      }

      pRec->setFileName(fileName);
    }
  }

  pRec->setBigFontFileName(srcStyle.bigFontFileName());
  pRec->setIsVertical(textStyle.isVertical());
  pRec->setIsUpsideDown(textStyle.isUpsideDown());
  pRec->setIsBackwards(textStyle.isBackward());
  pRec->setTextSize( textStyle.textSize() );

  OdDbTextStyleTablePtr pTable = pDb->getTextStyleTableId().openObject(OdDb::kForWrite);

  OdDbSymbolTableIteratorPtr pDbTextStyleIter = pTable->newIterator();

  unsigned index = 0;

  for(; !pDbTextStyleIter->done(); pDbTextStyleIter->step() )
  {
    OdDbTextStyleTableRecordPtr pDbTextStyle = pDbTextStyleIter->getRecord( OdDb::kForRead );

    OdString sCurTypeFace, sRecTypeFace;
    bool bCurBold = false, bCurItalic = false, bRecBold = false, bRecItalic = false;
    int nCurCharSet = 0, nCurPitchAndFamily = 0, nRecCharSet = 0, nRecPitchAndFamily = 0;

    pDbTextStyle->font(sCurTypeFace, bCurBold, bCurItalic, nCurCharSet, nCurPitchAndFamily);
    pRec->font(sRecTypeFace, bRecBold, bRecItalic, nRecCharSet, nRecPitchAndFamily);

    if( (sCurTypeFace == sRecTypeFace) && (bCurBold == bRecBold) && (bCurItalic == bRecItalic) && 
        (nCurCharSet == nRecCharSet) && (nCurPitchAndFamily == nRecPitchAndFamily) &&
        (pDbTextStyle->bigFontFileName() == pRec->bigFontFileName() ) &&
        (pDbTextStyle->isVertical() == pRec->isVertical() ) &&
        (pDbTextStyle->isUpsideDown() == pRec->isUpsideDown() ) &&
        (pDbTextStyle->isBackwards() == pRec->isBackwards() ) &&
        OdZero(pDbTextStyle->textSize() - pRec->textSize())
      )
    {
      OdDgnImportContext::setResourceUsage(pDbTextStyle->objectId());
      recId = pDbTextStyle->objectId();
      break;
    }
  }

  if( recId.isNull() )
  {
    for (;;)
    {
      try
      {
        OdString sName;
        sName.format(L"Style_%d", index++);

        if( pTable->getAt(sName) )
          continue;
        
        pRec->setName(sName);
        recId = pTable->add(pRec);
        break;
      }
      catch (const OdError_DuplicateRecordName&)
      {
      }
    }
  }

  return recId;
}

//---------------------------------------------------------------------------

OdDbObjectId OdDgnImportContext::getObjectId( const OdDgElementId& idDgnElement )
{
  OdDbObjectId retVal;

  OdDgnImportContextData* pContextData = getCurrentContext();

  if( pContextData && pContextData->m_pDgnImporter )
  {
    retVal = pContextData->m_pDgnImporter->getObjectId( idDgnElement );
    pContextData->m_pDgnImporter->setResourceUsage(retVal);
  }

  return retVal;
}

//---------------------------------------------------------------------------

bool OdDgnImportContext::getObjectPath( const OdDgElementId& idDgnElement,
                                          OdDgnImportPathToDwgObject& dwgPath
                                      )
{
  bool bRet = false;

  OdDgnImportContextData* pContextData = getCurrentContext();

  if( pContextData && pContextData->m_pDgnImporter )
  {
    bRet = pContextData->m_pDgnImporter->getObjectPath( idDgnElement, dwgPath );
  }

  return bRet;
}

//---------------------------------------------------------------------------

void OdDgnImportContext::addObjectPath( const OdDgElementId& idDgnElement, OdDgnImportPathToDwgObject& dwgPath )
{
  OdDgnImportContextData* pContextData = getCurrentContext();

  if( pContextData && pContextData->m_pDgnImporter )
  {
    if( pContextData->m_arrCellHeaderIds.size() > 0 )
    {
      dwgPath.m_idPath.objectIds().insert( dwgPath.m_idPath.objectIds().begin(), pContextData->m_arrCellHeaderIds.begin(), pContextData->m_arrCellHeaderIds.end() );
    }

    pContextData->m_pDgnImporter->addObjectPath( idDgnElement, dwgPath );
  }
}

//---------------------------------------------------------------------------

void OdDgnImportContext::addResourceId( const OdDbObjectId& idResource )
{
  OdDgnImportContextData* pContextData = getCurrentContext();

  if( pContextData && pContextData->m_pDgnImporter )
    pContextData->m_pDgnImporter->addResourceId( idResource );
}

//---------------------------------------------------------------------------

void OdDgnImportContext::setResourceUsage( const OdDbObjectId& idResource )
{
  OdDgnImportContextData* pContextData = getCurrentContext();

  if( pContextData && pContextData->m_pDgnImporter )
    pContextData->m_pDgnImporter->setResourceUsage( idResource );
}

//---------------------------------------------------------------------------

void OdDgnImportContext::addCellHeaderIdToPath( const OdDbObjectId& idCellHeader )
{
  OdDgnImportContextData* pContextData = getCurrentContext();

  if( pContextData )
  {
    pContextData->m_arrCellHeaderIds.push_back( idCellHeader );
  }
}

//---------------------------------------------------------------------------

void OdDgnImportContext::removeLastCellHeaderIdFromPath()
{
  OdDgnImportContextData* pContextData = getCurrentContext();

  if( pContextData && pContextData->m_arrCellHeaderIds.size() )
  {
    pContextData->m_arrCellHeaderIds.removeLast();
  }
}

//---------------------------------------------------------------------------

bool OdDgnImportContext::getImportCurvesOnlyFlag()
{
  OdDgnImportContextData* pContextData = getCurrentContext();

  if( pContextData )
  {
    return pContextData->m_bImportCurvesOnly;
  }

  return false;
}

//---------------------------------------------------------------------------

void OdDgnImportContext::setImportCurvesOnlyFlag( bool bSet )
{
  OdDgnImportContextData* pContextData = getCurrentContext();

  if( pContextData )
  {
    pContextData->m_bImportCurvesOnly = bSet;
  }
}

//---------------------------------------------------------------------------

bool OdDgnImportContext::getSkipPatternDrawFlag()
{
  OdDgnImportContextData* pContextData = getCurrentContext();

  if( pContextData )
  {
    return pContextData->m_bSkipPatternDraw;
  }

  return false;
}

//---------------------------------------------------------------------------

void OdDgnImportContext::setSkipPatternDrawFlag( bool bSet )
{
  OdDgnImportContextData* pContextData = getCurrentContext();

  if( pContextData )
  {
    pContextData->m_bSkipPatternDraw = bSet;
  }
}

//----------------------------------------------------------------------------

bool OdDgnImportContext::isNoteDimensionCell( const OdDgElementId& idCellHeader )
{
  bool bRet = false;

  OdDgnImportContextData* pContextData = getCurrentContext();

  if( pContextData )
  {
    for( OdUInt32 i = 0; i < pContextData->m_arrNoteCellHeaders.size(); i++ )
    {
      if( pContextData->m_arrNoteCellHeaders[i] == idCellHeader )
      {
        bRet = true;
        break;
      }
    }
  }

  return bRet;
}

//----------------------------------------------------------------------------

void OdDgnImportContext::addNoteDimensionCell( const OdDgElementId& idCellHeader )
{
  OdDgnImportContextData* pContextData = getCurrentContext();

  if( pContextData )
  {
    pContextData->m_arrNoteCellHeaders.push_back( idCellHeader );
  }
}

//----------------------------------------------------------------------------

void OdDgnImportContext::removeNoteDimensionCell( const OdDgElementId& idCellHeader )
{
  OdDgnImportContextData* pContextData = getCurrentContext();

  if( pContextData )
  {
    for( OdUInt32 i = 0; i < pContextData->m_arrNoteCellHeaders.size(); i++ )
    {
      if( pContextData->m_arrNoteCellHeaders[i] == idCellHeader )
      {
        pContextData->m_arrNoteCellHeaders.removeAt(i);
        break;
      }
    }
  }
}

//---------------------------------------------------------------------------

void OdDgnImportContext::pushLevelMask( const OdDgLevelMaskPtr& pLevelMask, const OdDgLevelTablePtr& pLevelTable )
{
  OdDgnImportContextData* pContextData = getCurrentContext();

  if( pContextData )
  {
    OdDgnImportLevelMaskDescriptor tmp;

    if( !pLevelTable.isNull() && !pLevelMask.isNull() )
    {
      std::map<OdString, bool> levelMaskData;

      for( OdUInt32 i = 0; i <= pLevelMask->getMaxLevelEntryId(); i++ )
      {
        OdDgLevelMask::OdDgLevelStatus curStatus;
        OdString strLevelName;
        pLevelMask->getLevelStatus(i, strLevelName, curStatus );

        if( strLevelName.isEmpty() )
        {
          OdDgElementId idLevel = pLevelTable->getAt( i );

          if( !idLevel.isNull() )
          {
            OdDgLevelTableRecordPtr pLevel = idLevel.openObject(OdDg::kForRead);
            strLevelName = pLevel->getName();
          }
        }

        if( curStatus == OdDgLevelMask::kLevelVisible )
        {
          levelMaskData[ strLevelName ] = true;
        }
        else if( curStatus == OdDgLevelMask::kLevelHidden )
        {
          levelMaskData[ strLevelName ] = false;
        }
      }

      OdDgElementIteratorPtr pLevelIter = pLevelTable->createIterator();

      for(; !pLevelIter->done(); pLevelIter->step() )
      {
        OdDgLevelTableRecordPtr pLevel = pLevelIter->item().openObject(OdDg::kForRead);

        if( !pLevel.isNull() )
        {
          bool bLevelIsVisible = true;

          std::map<OdString, bool>::iterator pMaskIter = levelMaskData.find( pLevel->getName() );

          if( pMaskIter != levelMaskData.end() )
          {
            bLevelIsVisible = pMaskIter->second;
          }

          tmp[pLevel->getEntryId()] = bLevelIsVisible;
        }
      }
    }

    pContextData->m_pLevelMaskStack.push_back( tmp );
  }
}

//---------------------------------------------------------------------------

void OdDgnImportContext::popLevelMask()
{
  OdDgnImportContextData* pContextData = getCurrentContext();

  if( pContextData && pContextData->m_pLevelMaskStack.size() > 0 )
  {
    pContextData->m_pLevelMaskStack.removeLast();
  }
}

//---------------------------------------------------------------------------

bool OdDgnImportContext::isLevelMask()
{
  bool bRet = false;

  OdDgnImportContextData* pContextData = getCurrentContext();

  if( pContextData && pContextData->m_pLevelMaskStack.size() > 0 )
  {
    bRet = true;
  }

  return bRet;
}

//----------------------------------------------------------------------------

bool OdDgnImportContext::checkLevelMask( OdUInt32 uLevelEntryId )
{
  bool bRet = true;

  OdDgnImportContextData* pContextData = getCurrentContext();

  if( pContextData && pContextData->m_pLevelMaskStack.size() > 0 )
  {
    OdDgnImportLevelMaskDescriptor& curMask = pContextData->m_pLevelMaskStack[pContextData->m_pLevelMaskStack.size() - 1];

    OdDgnImportLevelMaskDescriptorIterator curIter = curMask.find( uLevelEntryId );

    if( curIter != curMask.end() )
    {
      bRet = curIter->second;
    }
  }

  return bRet;
}

//----------------------------------------------------------------------------

OdString OdDgnImportContext::getLevelMaskString()
{
  OdString strRet = OdString::kEmpty;

  OdDgnImportContextData* pContextData = getCurrentContext();

  if( pContextData && pContextData->m_pLevelMaskStack.size() > 0 )
  {
    OdDgnImportLevelMaskDescriptor& curMask = pContextData->m_pLevelMaskStack[pContextData->m_pLevelMaskStack.size() - 1];

    OdDgnImportLevelMaskDescriptorIterator curIter = curMask.begin();

    while( curIter != curMask.end() )
    {
      if( curIter->second )
      {
        strRet += L"1";
      }
      else
      {
        strRet += L"0";
      }

      curIter++;
    }
  }

  return strRet;
}

//----------------------------------------------------------------------------

bool OdDgnImportContext::isImportInvisible()
{
  bool bRet = false;

  OdDgnImportContextData* pContextData = getCurrentContext();

  if( pContextData  )
  {
    bRet = pContextData->m_bImportInvisible;
  }

  return bRet;
}

//----------------------------------------------------------------------------

void OdDgnImportContext::setImportInvisible( bool bSet )
{
  OdDgnImportContextData* pContextData = getCurrentContext();

  if( pContextData  )
  {
    pContextData->m_bImportInvisible = bSet;
  }
}

//----------------------------------------------------------------------------

OdDgViewPtr OdDgnImportContext::getActiveView()
{
  OdDgnImportContextData* pContextData = getCurrentContext();

  if( pContextData  )
  {
    return pContextData->m_pActiveView;
  }

  return OdDgViewPtr();
}

//----------------------------------------------------------------------------

void        OdDgnImportContext::setActiveView( const OdDgViewPtr& pView )
{
  OdDgnImportContextData* pContextData = getCurrentContext();

  if( pContextData  )
  {
    pContextData->m_pActiveView = pView;
  }
}

//----------------------------------------------------------------------------

void initPolygonImportModes()
{
  OdDgnImportContextData* pContextData = getCurrentContext();

  if( pContextData && pContextData->m_pDgnImporter )
  {
    OdRxVariantValue tmpVar = (OdRxVariantValue)pContextData->m_pDgnImporter->properties()->getAt("3dShapeImportMode");
    pContextData->m_3dShapeImportMode = tmpVar->getInt8();
    tmpVar = (OdRxVariantValue)pContextData->m_pDgnImporter->properties()->getAt("2dShapeImportMode");
    pContextData->m_2dShapeImportMode = tmpVar->getInt8();
    tmpVar = (OdRxVariantValue)pContextData->m_pDgnImporter->properties()->getAt("3dEllipseImportMode");
    pContextData->m_3dEllipseImportMode = tmpVar->getInt8();
    tmpVar = (OdRxVariantValue)pContextData->m_pDgnImporter->properties()->getAt("2dEllipseImportMode");
    pContextData->m_2dEllipseImportMode = tmpVar->getInt8();
    tmpVar = (OdRxVariantValue)pContextData->m_pDgnImporter->properties()->getAt("3dClosedBSplineCurveImportMode");
    pContextData->m_3dSplineImportMode = tmpVar->getInt8();
    tmpVar = (OdRxVariantValue)pContextData->m_pDgnImporter->properties()->getAt("2dClosedBSplineCurveImportMode");
    pContextData->m_2dSplineImportMode = tmpVar->getInt8();
    tmpVar = (OdRxVariantValue)pContextData->m_pDgnImporter->properties()->getAt("3dObjectImportMode");
    pContextData->m_3dObjectImportMode = tmpVar->getInt8();

    pContextData->m_bInitPolygonImportModes = true;
  }
}

//----------------------------------------------------------------------------

OdUInt8 OdDgnImportContext::getShape3dImportMode()
{
  OdUInt8 uRet = 1;

  OdDgnImportContextData* pContextData = getCurrentContext();

  if( pContextData  )
  {
    if( !pContextData->m_bInitPolygonImportModes )
    {
      initPolygonImportModes();
    }

    uRet = pContextData->m_3dShapeImportMode;
  }

  return uRet;
}

//----------------------------------------------------------------------------

OdUInt8 OdDgnImportContext::getObject3dImportMode()
{
  OdUInt8 uRet = 1;

  OdDgnImportContextData* pContextData = getCurrentContext();

  if( pContextData  )
  {
    if( !pContextData->m_bInitPolygonImportModes )
    {
      initPolygonImportModes();
    }

    uRet = pContextData->m_3dObjectImportMode;
  }

  return uRet;
}


//----------------------------------------------------------------------------

OdUInt8 OdDgnImportContext::getShape2dImportMode()
{
  OdUInt8 uRet = 0;

  OdDgnImportContextData* pContextData = getCurrentContext();

  if( pContextData  )
  {
    if( !pContextData->m_bInitPolygonImportModes )
    {
      initPolygonImportModes();
    }

    uRet = pContextData->m_2dShapeImportMode;
  }

  return uRet;
}

//----------------------------------------------------------------------------

OdUInt8 OdDgnImportContext::getEllipse3dImportMode()
{
  OdUInt8 uRet = 0;

  OdDgnImportContextData* pContextData = getCurrentContext();

  if( pContextData  )
  {
    if( !pContextData->m_bInitPolygonImportModes )
    {
      initPolygonImportModes();
    }

    uRet = pContextData->m_3dEllipseImportMode;
  }

  return uRet;
}

//----------------------------------------------------------------------------

OdUInt8 OdDgnImportContext::getEllipse2dImportMode()
{
  OdUInt8 uRet = 0;

  OdDgnImportContextData* pContextData = getCurrentContext();

  if( pContextData  )
  {
    if( !pContextData->m_bInitPolygonImportModes )
    {
      initPolygonImportModes();
    }

    uRet = pContextData->m_2dEllipseImportMode;
  }

  return uRet;
}

//----------------------------------------------------------------------------

OdUInt8 OdDgnImportContext::getClosedBSplineCurve3dImportMode()
{
  OdUInt8 uRet = 1;

  OdDgnImportContextData* pContextData = getCurrentContext();

  if( pContextData  )
  {
    if( !pContextData->m_bInitPolygonImportModes )
    {
      initPolygonImportModes();
    }

    uRet = pContextData->m_3dSplineImportMode;
  }

  return uRet;
}

//----------------------------------------------------------------------------

OdUInt8 OdDgnImportContext::getClosedBSplineCurve2dImportMode()
{
  OdUInt8 uRet = 0;

  OdDgnImportContextData* pContextData = getCurrentContext();

  if( pContextData  )
  {
    if( !pContextData->m_bInitPolygonImportModes )
    {
      initPolygonImportModes();
    }

    uRet = pContextData->m_2dSplineImportMode;
  }

  return uRet;
}

//----------------------------------------------------------------------------

void    OdDgnImportContext::setCurvesOnlyImportMode()
{
  OdDgnImportContextData* pContextData = getCurrentContext();

  if( pContextData  )
  {
    pContextData->m_2dSplineImportMode  = 0;
    pContextData->m_3dSplineImportMode  = 0;
    pContextData->m_2dEllipseImportMode = 0;
    pContextData->m_3dEllipseImportMode = 0;
    pContextData->m_2dShapeImportMode   = 0;
    pContextData->m_3dShapeImportMode   = 0;
    pContextData->m_bInitPolygonImportModes = true;
  }
}

//----------------------------------------------------------------------------

void    OdDgnImportContext::setUpdateCurvesImportMode()
{
  OdDgnImportContextData* pContextData = getCurrentContext();

  if( pContextData  )
  {
    pContextData->m_bInitPolygonImportModes = false;
  }
}

//----------------------------------------------------------------------------

OdUInt32 OdDgnImportContext::getRscFontCount()
{
  OdDgnImportContextData* pContextData = getCurrentContext();

  if( pContextData  )
  {
    return pContextData->m_arrRscFontNames.size();
  }

  return 0;
}

//----------------------------------------------------------------------------

OdString OdDgnImportContext::getRscFontName( OdUInt32 uIndex )
{
  OdString strRet;

  OdDgnImportContextData* pContextData = getCurrentContext();

  if( pContextData && pContextData->m_arrRscFontNames.size() > uIndex )
  {
    strRet = pContextData->m_arrRscFontNames[uIndex];
  }

  return strRet;
}

//----------------------------------------------------------------------------

void     OdDgnImportContext::addRscFontName( const OdString& strName )
{
  OdDgnImportContextData* pContextData = getCurrentContext();

  if( pContextData && !strName.isEmpty() )
  {
    bool bFontNamePresent = false;

    for( OdUInt32 i = 0; i < pContextData->m_arrRscFontNames.size(); i++ )
    {
      if( pContextData->m_arrRscFontNames[i] == strName )
      {
        bFontNamePresent = true;
        break;
      }
    }

    if( !bFontNamePresent )
    {
      pContextData->m_arrRscFontNames.push_back(strName);
    }
  }
}

//----------------------------------------------------------------------------

void     OdDgnImportContext::createShxFontFromRscFont( const OdString& strFontName, 
                                                         OdDgDatabase* pDgDb,
                                                           OdDbHostAppServices* pHostAppServices,
                                                             const OdString& strShxPathData )
{
  if( !pDgDb )
    return;

  OdDgFontTablePtr   pFontTable   = pDgDb->getFontTable(OdDg::kForRead);
  OdDgFontTablePEPtr pFontTablePE = OdDgFontTablePEPtr(OdRxObjectPtr(pFontTable));

  if( pFontTable.isNull() || pFontTablePE.isNull() )
  {
    return;
  }

  bool bFontWasCreated = false;

  for( OdUInt32 i = 0; i < OdDgnImportContext::getRscFontCount(); i++ )
  {
    if( OdDgnImportContext::getRscFontName( i ) == strFontName )
    {
      bFontWasCreated = true;
      break;
    }
  }

  if( !bFontWasCreated)
  {
    OdDgnImportContext::addRscFontName( strFontName );

    OdString strCurFontName = strFontName;

    OdString strFontExt = strCurFontName;

    if( strFontExt.getLength() > 4 )
    {
      strFontExt = strFontExt.right(4);
    }

    strFontExt.makeUpper();

    if( strFontExt == L".SHX" )
    {
      return;
    }

    OdDgFontTableRecordPtr pFontRec = pFontTable->getFont( strCurFontName );

    if( !pFontRec.isNull() && (pFontRec->getType() == kFontTypeRsc) )
    {
      OdString strFileName = pHostAppServices->findFile( strCurFontName + L".shx", 0, OdDbBaseHostAppServices::kFontFile );

      if( !strFileName.isEmpty() )
      {
        return;
      }

      OdString strShxPath = strShxPathData;

      if( !strShxPath.isEmpty() )
      {
        strShxPath.replace(L'\\', L'/');

        if( strShxPath[strShxPath.getLength()-1] != L'/' )
        {
          strShxPath += L"/";
        }

        OdString strFileName = pHostAppServices->findFile( strShxPath + strCurFontName + L".shx", 0, OdDbBaseHostAppServices::kFontFile );

        if( !strFileName.isEmpty() )
        {
          return;
        }
      }
      else
      {
        strShxPath.replace(L'\\', L'/');

        if( strShxPath.reverseFind(L'/') != -1 )
        {
          strShxPath = strShxPath.left(strShxPath.reverseFind(L'/') + 1);
        }
        else
        {
          strShxPath.empty();
        }
      }

      OdString strShxFileName = strShxPath + strCurFontName + L".shx";

      strFileName = pHostAppServices->findFile( strShxFileName, 0, OdDbBaseHostAppServices::kFontFile );

      if( strFileName.isEmpty() )
      {
        pFontTablePE->convertRscFontToShxFont( pFontTable, strCurFontName, strShxFileName );
      }
    }
  }
}

//----------------------------------------------------------------------------

void     OdDgnImportContext::clearRscFontArray()
{
  OdDgnImportContextData* pContextData = getCurrentContext();

  if( pContextData )
  {
    pContextData->m_arrRscFontNames.clear();
  }
}

//----------------------------------------------------------------------------

OdDbObjectId OdDgnImportContext::createMultilineStyle( const OdDgnImportMultilineDescriptor& mLineDesc, OdDbDatabase* pDb, OdDgDatabase* pDg )
{
  OdDgnImportMultilineDescriptor desc = mLineDesc;

  OdDgnImportContextData* pContextData = getCurrentContext();

  OdDbDictionaryPtr pMLDic = pDb->getMLStyleDictionaryId().safeOpenObject(OdDb::kForWrite);

  OdDbMlineStylePtr pMLStyle = OdDbMlineStyle::createObject();

  OdString strStyleName = L"MLStyle_0";

  if( !desc.m_strStyleName.isEmpty() )
  {
    strStyleName = desc.m_strStyleName;
    strStyleName.remove( L' ' );

    if( strStyleName.getLength() > 32 )
    {
      strStyleName = strStyleName.left(32);
    }
  }

  OdString repairedName;

  if( OdDbSymUtil::repairSymbolName(repairedName, strStyleName, pDb ) == eOk && !repairedName.isEmpty() )
  {
    strStyleName = repairedName;
  }

  OdUInt32 iCount = 0;

  while( pMLDic->has(strStyleName) )
  {
    iCount++;

    strStyleName.format(L"MLStyle_%d", iCount);
  }

  OdCmColor clrFillColor;

  setDwgColorByDgnIndex( pDg, clrFillColor, desc.m_uFillColor );

  // Create multiline style

  pMLStyle->initMlineStyle();
  pMLStyle->setName( strStyleName );
  pMLStyle->setShowMiters( desc.m_bShowJoints );
  pMLStyle->setStartSquareCap( desc.m_bShowStartCapLine );
  pMLStyle->setStartInnerArcs( desc.m_bShowStartCapInArc );
  pMLStyle->setStartRoundCap( desc.m_bShowStartCapOutArc );
  pMLStyle->setStartAngle( desc.m_dStartCapAngle );
  pMLStyle->setEndSquareCap( desc.m_bShowEndCapLine );
  pMLStyle->setEndInnerArcs( desc.m_bShowEndCapInArc );
  pMLStyle->setEndRoundCap( desc.m_bShowEndCapOutArc );
  pMLStyle->setEndAngle( desc.m_dEndCapAngle );
  pMLStyle->setFilled( desc.m_bShowFill );
  pMLStyle->setFillColor( clrFillColor );

  OdArray<double>       arrOffsets;
  OdArray<OdCmColor>    arrColors;
  OdArray<OdDbObjectId> arrLineTypes;
  double                dMaxOffset = -1e20;
  double                dMinOffset = 1e20;

  OdUInt32 i;

  for( i = 0; i < desc.m_arrOffsets.size(); i++ )
  {
    double dCurOffset = desc.m_arrOffsets[i];

    arrOffsets.push_back( dCurOffset );

    if( dCurOffset < dMinOffset )
    {
      dMinOffset = dCurOffset;
    }

    if( dCurOffset > dMaxOffset )
    {
      dMaxOffset = dCurOffset;
    }

    OdCmColor curColor;
    setDwgColorByDgnIndex( pDg, curColor, desc.m_arrColorIndexes[i] );

    arrColors.push_back( curColor );

    OdDbObjectId idCurLineStyle;

    setDwgLineTypeIdByDgnLineTypeIndex(pDb, pDg, desc.m_arrLineStyles[i], idCurLineStyle );

    arrLineTypes.push_back( idCurLineStyle );
  }
  
  double dCenterOffset = (dMinOffset + dMaxOffset) / 2.0;

  desc.m_bCenterIsZero = OdZero( dCenterOffset );

  if( desc.m_uJustification == OdDgMultiline::kByCenter )
  {
    for( i = 0; i < arrOffsets.size(); i++ )
    {
      arrOffsets[i] -= dCenterOffset;
    }
  }

  for( i = 0; i < arrOffsets.size(); i++ )
  {
    pMLStyle->addElement( arrOffsets[i], arrColors[i], arrLineTypes[i] );
  }

  pMLDic->setAt(strStyleName, pMLStyle);

  OdDbObjectId idStyle = pMLStyle->objectId();

  if( pContextData )
  {
    pContextData->m_mapMultilineStyles[idStyle] = desc;

    if( desc.m_bTableMLineStyle )
    {
      pContextData->m_mapTableMLStyleIds[ desc.m_uMLineStyle ] = idStyle;
    }
  }

  return idStyle;
}

//----------------------------------------------------------------------------

bool isEqualMLineStyle( const OdDgnImportMultilineDescriptor& desc1, 
                          const OdDgnImportMultilineDescriptor& desc2 
                      )
{
  bool bRet = true;

  if( bRet && (desc1.m_arrOffsets.size() != desc2.m_arrOffsets.size()) )
  {
    bRet = false;
  }

  if( desc1.m_bShowJoints != desc2.m_bShowJoints )
  {
    bRet = false;
  }

  if( bRet && (desc1.m_bShowStartCapLine != desc2.m_bShowStartCapLine) )
  {
    bRet = false;
  }

  if( bRet && (desc1.m_bShowStartCapInArc != desc2.m_bShowStartCapInArc) )
  {
    bRet = false;
  }

  if( bRet && (desc1.m_bShowStartCapOutArc != desc2.m_bShowStartCapOutArc) )
  {
    bRet = false;
  }

  if( bRet && (desc1.m_bShowEndCapLine != desc2.m_bShowEndCapLine) )
  {
    bRet = false;
  }

  if( bRet && (desc1.m_bShowEndCapInArc != desc2.m_bShowEndCapInArc) )
  {
    bRet = false;
  }

  if( bRet && (desc1.m_bShowEndCapOutArc != desc2.m_bShowEndCapOutArc) )
  {
    bRet = false;
  }

  if( bRet && (desc1.m_bShowFill != desc2.m_bShowFill) )
  {
    bRet = false;
  }


  if( bRet && !OdZero( desc1.m_dStartCapAngle - desc2.m_dStartCapAngle) )
  {
    bRet = false;
  }

  if( bRet && !OdZero( desc1.m_dEndCapAngle - desc2.m_dEndCapAngle) )
  {
    bRet = false;
  }

  if( bRet && desc1.m_bShowFill )
  {
    if( desc1.m_uFillColor != desc2.m_uFillColor )
    {
      bRet = false;
    }
  }

  if( bRet )
  {
    if( desc2.m_uJustification == OdDgMultiline::kByCenter )
    {
      if( !desc1.m_bCenterIsZero && (desc1.m_uJustification != OdDgMultiline::kByCenter) )
      {
        bRet = false;
      }
    }
  }

  bool bCheckByDifs = false;

  if( (desc2.m_uJustification == OdDgMultiline::kByMaximum) ||(desc2.m_uJustification == OdDgMultiline::kByMinimum) )
  {
    bCheckByDifs = true;
  }

  if( bRet )
  {
    bool bEqualElementData = true;

    for( OdUInt32 i = 0; i < desc1.m_arrOffsets.size(); i++ )
    {
      if( desc1.m_arrColorIndexes[i] != desc2.m_arrColorIndexes[i] )
      {
        bEqualElementData = false;
        break;
      }

      if( desc1.m_arrLineStyles[i] != desc2.m_arrLineStyles[i] )
      {
        bEqualElementData = false;
        break;
      }

      if( !bCheckByDifs && !OdZero(desc1.m_arrOffsets[i] - desc2.m_arrOffsets[i] ) )
      {
        bEqualElementData = false;
        break;
      }
      else if( bCheckByDifs && (i < (desc1.m_arrOffsets.size() - 1)) )
      {
        double dDif1 = desc1.m_arrOffsets[i] - desc1.m_arrOffsets[i+1];
        double dDif2 = desc2.m_arrOffsets[i] - desc2.m_arrOffsets[i+1];

        if( !OdZero(dDif2 - dDif1) )
        {
          bEqualElementData = false;
          break;
        }
      }
    }

    bRet = bEqualElementData;
  }

  return bRet;
}

//----------------------------------------------------------------------------

OdDbObjectId OdDgnImportContext::getMultilineStyle( const OdDgnImportMultilineDescriptor& desc )
{
  OdDbObjectId idRet;

  OdDgnImportContextData* pContextData = getCurrentContext();

  if( pContextData )
  {
    OdDbObjectId                   mlStyleId;

    if( !desc.m_bTableMLineStyle )
    {
      std::map<OdUInt64, OdDbObjectId>::iterator pMlStyleIter = pContextData->m_mapTableMLStyleIds.find(desc.m_uMLineStyle);

      if( pMlStyleIter != pContextData->m_mapTableMLStyleIds.end() )
      {
        mlStyleId = pMlStyleIter->second;

        std::map<OdDbObjectId, OdDgnImportMultilineDescriptor>::iterator pCurIter = pContextData->m_mapMultilineStyles.find(mlStyleId);

        if( pCurIter != pContextData->m_mapMultilineStyles.end() )
        {
          OdDgnImportMultilineDescriptor curDesc = pCurIter->second;

          if( isEqualMLineStyle( curDesc, desc ) )
          {
            idRet = mlStyleId;
          }
        }
      }
    }

    if( idRet.isNull() )
    {
      std::map<OdDbObjectId, OdDgnImportMultilineDescriptor>::iterator pIter = pContextData->m_mapMultilineStyles.begin();

      while( pIter != pContextData->m_mapMultilineStyles.end() )
      {
        OdDgnImportMultilineDescriptor curDesc = pIter->second;
        OdDbObjectId                   curId   = pIter->first;

        pIter++;

        if( curDesc.m_bTableMLineStyle )
        {
          continue;
        }

        if( !isEqualMLineStyle( curDesc, desc ) )
        {
          continue;
        }

        idRet = curId;
        break;
      }
    }
  }

  return idRet;
}

//----------------------------------------------------------------------------

void         OdDgnImportContext::clearMultilineStyles()
{
  OdDgnImportContextData* pContextData = getCurrentContext();

  if( pContextData )
  {
    pContextData->m_mapMultilineStyles.clear();
  }
}

//----------------------------------------------------------------------------

OdUInt32 OdDgnImportContext::getDimAssocDescriptorCount()
{
  OdDgnImportContextData* pContextData = getCurrentContext();

  if( pContextData )
  {
    return (OdUInt32)(pContextData->m_mapDimAssocDescripotors.size());
  }

  return 0;
}

//----------------------------------------------------------------------------

void     OdDgnImportContext::addDimAssocDescriptor( const OdDbObjectId& idDwgDim, 
                                                      const OdDgnImportDimAssocDescriptor& dimAssocDesc )
{
  OdDgnImportContextData* pContextData = getCurrentContext();

  if( pContextData )
  {
    pContextData->m_mapDimAssocDescripotors[idDwgDim] = dimAssocDesc;
  }
}

//----------------------------------------------------------------------------

void     OdDgnImportContext::getDimAssocDescriptor( OdUInt32 uIndex, 
                                                      OdDbObjectId& idDwgDim, 
                                                        OdDgnImportDimAssocDescriptor& dimAssocDesc )
{
  OdDgnImportContextData* pContextData = getCurrentContext();

  if( pContextData )
  {
    if( uIndex >= pContextData->m_mapDimAssocDescripotors.size() )
    {
      return;
    }

    std::map<OdDbObjectId, OdDgnImportDimAssocDescriptor>::iterator pIter = pContextData->m_mapDimAssocDescripotors.begin();

    for( OdUInt32 i = 0; i < uIndex; i++ )
    {
      pIter++;
    }

    idDwgDim     = pIter->first;
    dimAssocDesc = pIter->second;
  }
}

//----------------------------------------------------------------------------

void     OdDgnImportContext::clearDimAssocDescriptors()
{
  OdDgnImportContextData* pContextData = getCurrentContext();

  if( pContextData )
  {
    pContextData->m_mapDimAssocDescripotors.clear();
  }
}

//----------------------------------------------------------------------------

void     OdDgnImportContext::addSymbolHatchDesc( const OdDgElementId& idSymbol, const OdDgnImportSymbolHatchDescriptor& hatchDesc )
{
  OdDgnImportContextData* pContextData = getCurrentContext();

  if( pContextData )
  {
    pContextData->m_mapSymbolHatch[idSymbol] = hatchDesc;
  }
}

//----------------------------------------------------------------------------

bool     OdDgnImportContext::getSymbolHatchDesc( const OdDgElementId& idSymbol, 
                                                   double dRowSpacing, 
                                                     double dColSpacing, 
                                                       OdDgnImportSymbolHatchDescriptor& hatchDesc )
{
  bool bRet = false;

  OdDgnImportContextData* pContextData = getCurrentContext();

  if( pContextData )
  {
    std::map<OdDgElementId, OdDgnImportSymbolHatchDescriptor>::iterator pIter = pContextData->m_mapSymbolHatch.find( idSymbol );

    if( pIter != pContextData->m_mapSymbolHatch.end() )
    {
      hatchDesc = pIter->second;

      if( OdZero(hatchDesc.m_dPatternRowSpacing - dRowSpacing, 1e-6) && OdZero(hatchDesc.m_dPatternColSpacing - dColSpacing, 1e-6) )
      {
        bRet = true;
      }
    }
  }

  return bRet;
}

//----------------------------------------------------------------------------

void     OdDgnImportContext::clearSymbolHatchDesc()
{
  OdDgnImportContextData* pContextData = getCurrentContext();

  if( pContextData )
  {
    pContextData->m_mapSymbolHatch.clear();
  }
}

//----------------------------------------------------------------------------

void     OdDgnImportContext::setCellBlockNextIndex( const OdString& strCellName, OdUInt32 uIndex )
{
  OdDgnImportContextData* pContextData = getCurrentContext();

  if( pContextData )
  {
    pContextData->m_mapCellNextBlockIndex[ strCellName ] = uIndex;
  }
}

//----------------------------------------------------------------------------

bool     OdDgnImportContext::getCellBlockNextIndex( const OdString& strCellName, OdUInt32& uIndex )
{
  bool bRet = false;

  OdDgnImportContextData* pContextData = getCurrentContext();

  if( pContextData )
  {
    std::map<OdString, OdUInt32>::iterator pIter = pContextData->m_mapCellNextBlockIndex.find( strCellName );

    if( pIter != pContextData->m_mapCellNextBlockIndex.end() )
    {
      bRet = true;
      uIndex = pIter->second;
    }
  }

  return bRet;
}

//----------------------------------------------------------------------------

}
