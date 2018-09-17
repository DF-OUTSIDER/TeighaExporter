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
#include "DgnImportTableElement.h"
#include "DgnImportCommon.h"
#include "DgnImportContext.h"
#include <DgTableElement.h>
#include <DbTable.h>
#include <DbBlockTable.h>
#include <DbBlockTableRecord.h>
#include <DbMText.h>
#include <DbText.h>
#include "DgCmColor.h"
#include "DgDatabase.h"
#include "DbTextStyleTable.h"
#include "DbTextStyleTableRecord.h"

namespace TD_DGN_IMPORT {

//---------------------------------------------------------------------------------------------------
OdDb::TableBreakFlowDirection getFlowDirection( OdDgTableElement::OdDgTableElementSubTablePosition pos )
{
  OdDb::TableBreakFlowDirection retVal = OdDb::kTableBreakFlowDownOrUp;

  switch( pos )
  {
    case OdDgTableElement::kLeft:  retVal = OdDb::kTableBreakFlowLeft; break;
    case OdDgTableElement::kRight: retVal = OdDb::kTableBreakFlowRight; break;
  }

  return retVal;
}

//---------------------------------------------------------------------------------------------------

OdString createMTextContent( OdDbDatabase* pDb, OdDgTextNode3dPtr& pDgCellContent )
{
  OdString strRet = OdString::kEmpty;

  DgnImporter* pImporter = OdDgnImportContext::getDgnImporter();

  bool bRestoreTextNodePE = false;

  if( pImporter )
  {
    bRestoreTextNodePE = pImporter->setTextNodeToMTextImportPE();

    //

    OdDbBlockTableRecordPtr pMTextTmpBlock = OdDbBlockTableRecord::createObject();

    OdDbBlockTablePtr pBlockTable = pDb->getBlockTableId().openObject( OdDb::kForWrite );

    if( !pBlockTable.isNull() )
    {
      pMTextTmpBlock->setName( L"*D" );

      pBlockTable->add(pMTextTmpBlock);

      ((OdDgnImportPE*)pDgCellContent->queryX(OdDgnImportPE::desc()))->subImportElement(pDgCellContent, pMTextTmpBlock);
    }

    for( OdDbObjectIteratorPtr pIter = pMTextTmpBlock->newIterator() ; !pIter->done(); pIter->step() )
    {
      OdDbEntityPtr elm = pIter->objectId().safeOpenObject(OdDb::kForWrite);

      if( elm->isKindOf(OdDbMText::desc()) )
      {
        OdDbMTextPtr pMText = pIter->objectId().openObject(OdDb::kForRead);
        strRet = pMText->contents();
        break;
      }
      else if( elm->isKindOf( OdDbText::desc()) )
      {
        OdDbTextPtr pText = pIter->objectId().openObject(OdDb::kForRead);
        strRet = pText->textString();
        break;
      }
    }

    pMTextTmpBlock->erase(true);

    //

    if( bRestoreTextNodePE)
    {
      pImporter->setTextNodeToEntitySetImportPE();
    }
  }

  return strRet;
}

//---------------------------------------------------------------------------------------------------

OdDb::CellAlignment getDbCellAlignment( OdDgTableCellElement::OdDgTableCellElementTextAlignment uAlignment )
{
  OdDb::CellAlignment retVal = OdDb::kTopLeft;

  switch( uAlignment )
  {
    case OdDgTableCellElement::kLeftTop       : retVal = OdDb::kTopLeft; break;
    case OdDgTableCellElement::kLeftMiddle    : retVal = OdDb::kMiddleLeft; break;
    case OdDgTableCellElement::kLeftBottom    : retVal = OdDb::kBottomLeft; break;
    case OdDgTableCellElement::kCenterTop     : retVal = OdDb::kTopCenter; break;
    case OdDgTableCellElement::kCenterMiddle  : retVal = OdDb::kMiddleCenter; break;
    case OdDgTableCellElement::kCenterBottom  : retVal = OdDb::kBottomCenter; break;
    case OdDgTableCellElement::kRightTop      : retVal = OdDb::kTopRight; break;
    case OdDgTableCellElement::kRightMiddle   : retVal = OdDb::kMiddleRight; break;
    case OdDgTableCellElement::kRightBottom   : retVal = OdDb::kBottomRight; break;
  }

  return retVal;
}

//---------------------------------------------------------------------------------------------------

double getDbCellTextRotation( OdDgTableCellElement::OdDgTableCellElementTextOrientation uRot )
{
  double dRet = 0;

  switch( uRot )
  {
    case OdDgTableCellElement::kRotateLeft : dRet = OdaPI2; break;
    case OdDgTableCellElement::kRotateRight: dRet = 3*OdaPI2; break;
  }

  return dRet;
}

//---------------------------------------------------------------------------------------------------

void OdDgnTableElementImportPE::subImportElement(OdDgElement* e, OdDbBlockTableRecord* owner)
{
  OdDgTableElement* pDgTable = (OdDgTableElement*)(e);
  OdDbTablePtr      pDbTable = OdDbTable::createObject();
  pDbTable->setDatabaseDefaults( owner->database() );
  owner->appendOdDbEntity( pDbTable );
  copyEntityProperties(pDgTable, pDbTable);

  OdGeVector3d vrDir = OdGeVector3d::kXAxis;
  vrDir = vrDir.transformBy( pDgTable->getRotation() );

  // Import table main properties.

  pDbTable->setPosition( pDgTable->getOrigin() );
  pDbTable->setDirection( vrDir );
  pDbTable->setNumColumns( pDgTable->getColumnCount() );
  pDbTable->setNumRows( pDgTable->getRowCount() );

  // Init table text style

  OdDgElementId idDgTextStyle = pDgTable->database()->getTextStyleTable(OdDg::kForRead)->getAt(pDgTable->getBodyTextStyle());
  OdDbObjectId idDbTextStyle = OdDgnImportContext::getObjectId( idDgTextStyle );
  OdDbObjectId idDbTextStyleVertical;

  if( !idDbTextStyle.isNull() )
    pDbTable->setTextStyle( idDbTextStyle );

  // Init table defaults

  OdDbObjectId idTable = pDbTable->objectId();
  pDbTable = 0; // To call subClose(...) and init it.
  pDbTable = idTable.openObject(OdDb::kForWrite);
  pDbTable->unmergeCells(0,0,0, pDbTable->numColumns() - 1 );

  // Import table cells.

  OdInt32 i, j;

  for( i = 0; i < (OdInt32)pDgTable->getRowCount(); i++ )
    for( j = 0; j < (OdInt32)pDgTable->getColumnCount(); j++ )
    {
      // Import cell border settings.

      OdUInt32 uTableColor = pDgTable->getTableColorIndex();
      OdUInt32 uTableLS    = pDgTable->getTableLineStyle();
      OdUInt32 uTableLW    = pDgTable->getTableLineWeight();

      OdDgTableCellElementPtr pDgTableCell = pDgTable->getCell(i,j);
      OdDgTableCellSymbology  topBorder    = pDgTableCell->getTopBorder();
      OdDgTableCellSymbology  bottomBorder = pDgTableCell->getBottomBorder();
      OdDgTableCellSymbology  leftBorder   = pDgTableCell->getLeftBorder();
      OdDgTableCellSymbology  rightBorder  = pDgTableCell->getRightBorder();

      OdCmColor topColor, bottomColor, leftColor, rightColor;
      OdDb::LineWeight  uTopLW, uBottomLW, uLeftLW, uRightLW;
      OdDbObjectId idTopLineType, idBottomLineType, idLeftLineType, idRightLineType;

      if( topBorder.getUseTableColorFlag() )
        setDwgColorByDgnIndex( pDgTable->database(), topColor, uTableColor );
      else
        setDwgColorByDgnIndex( pDgTable->database(), topColor, topBorder.getColorIndex() );

      if( bottomBorder.getUseTableColorFlag() )
        setDwgColorByDgnIndex( pDgTable->database(), bottomColor, uTableColor );
      else
        setDwgColorByDgnIndex( pDgTable->database(), bottomColor, bottomBorder.getColorIndex() );

      if( leftBorder.getUseTableColorFlag() )
        setDwgColorByDgnIndex( pDgTable->database(), leftColor, uTableColor );
      else
        setDwgColorByDgnIndex( pDgTable->database(), leftColor, leftBorder.getColorIndex() );

      if( rightBorder.getUseTableColorFlag() )
        setDwgColorByDgnIndex( pDgTable->database(), rightColor, uTableColor );
      else
        setDwgColorByDgnIndex( pDgTable->database(), rightColor, rightBorder.getColorIndex() );

      if( topBorder.getUseTableLineWeightFlag() )
        uTopLW = odDgnImportLineweightMapping( uTableLW );
      else
        uTopLW = odDgnImportLineweightMapping( topBorder.getLineWeight() );

      if( bottomBorder.getUseTableLineWeightFlag() )
        uBottomLW = odDgnImportLineweightMapping( uTableLW );
      else
        uBottomLW = odDgnImportLineweightMapping( bottomBorder.getLineWeight() );

      if( leftBorder.getUseTableLineWeightFlag() )
        uLeftLW = odDgnImportLineweightMapping( uTableLW );
      else
        uLeftLW = odDgnImportLineweightMapping( leftBorder.getLineWeight() );

      if( rightBorder.getUseTableLineWeightFlag() )
        uRightLW = odDgnImportLineweightMapping( uTableLW );
      else
        uRightLW = odDgnImportLineweightMapping( rightBorder.getLineWeight() );

      if( topBorder.getUseTableLineStyleFlag() )
        setDwgLineTypeIdByDgnLineTypeIndex( owner->database(), e->database(), uTableLS, idTopLineType );
      else
        setDwgLineTypeIdByDgnLineTypeIndex( owner->database(), e->database(), topBorder.getLineStyle(), idTopLineType );

      if( bottomBorder.getUseTableLineStyleFlag() )
        setDwgLineTypeIdByDgnLineTypeIndex( owner->database(), e->database(), uTableLS, idBottomLineType );
      else
        setDwgLineTypeIdByDgnLineTypeIndex( owner->database(), e->database(), bottomBorder.getLineStyle(), idBottomLineType );

      if( leftBorder.getUseTableLineStyleFlag() )
        setDwgLineTypeIdByDgnLineTypeIndex( owner->database(), e->database(), uTableLS, idLeftLineType );
      else
        setDwgLineTypeIdByDgnLineTypeIndex( owner->database(), e->database(), leftBorder.getLineStyle(), idLeftLineType );

      if( rightBorder.getUseTableLineStyleFlag() )
        setDwgLineTypeIdByDgnLineTypeIndex( owner->database(), e->database(), uTableLS, idRightLineType );
      else
        setDwgLineTypeIdByDgnLineTypeIndex( owner->database(), e->database(), rightBorder.getLineStyle(), idRightLineType );

      pDbTable->setGridColor(      i, j, OdDb::kHorzTop, topColor );
      pDbTable->setGridLinetype(   i, j, OdDb::kHorzTop, idTopLineType );
      pDbTable->setGridLineWeight( i, j, OdDb::kHorzTop, uTopLW );
      pDbTable->setGridVisibility( i, j, OdDb::kHorzTop, topBorder.getLineVisibility() ? OdDb::kVisible : OdDb::kInvisible );

      pDbTable->setGridColor(      i, j, OdDb::kHorzBottom, bottomColor );
      pDbTable->setGridLinetype(   i, j, OdDb::kHorzBottom, idBottomLineType );
      pDbTable->setGridLineWeight( i, j, OdDb::kHorzBottom, uBottomLW );
      pDbTable->setGridVisibility( i, j, OdDb::kHorzBottom, bottomBorder.getLineVisibility() ? OdDb::kVisible : OdDb::kInvisible );

      pDbTable->setGridColor(      i, j, OdDb::kVertLeft, leftColor );
      pDbTable->setGridLinetype(   i, j, OdDb::kVertLeft, idLeftLineType );
      pDbTable->setGridLineWeight( i, j, OdDb::kVertLeft, uLeftLW );
      pDbTable->setGridVisibility( i, j, OdDb::kVertLeft, leftBorder.getLineVisibility() ? OdDb::kVisible : OdDb::kInvisible );

      pDbTable->setGridColor(      i, j, OdDb::kVertRight, rightColor );
      pDbTable->setGridLinetype(   i, j, OdDb::kVertRight, idRightLineType );
      pDbTable->setGridLineWeight( i, j, OdDb::kVertRight, uRightLW );
      pDbTable->setGridVisibility( i, j, OdDb::kVertRight, rightBorder.getLineVisibility() ? OdDb::kVisible : OdDb::kInvisible );

      // Import cell margins.

      double dTopMargin = 0, dBottomMargin = 0, dLeftMargin = 0, dRightMargin = 0;

      double dDefaultTopMargin    = pDgTable->getCellTopMargin();
      double dDefaultBottomMargin = pDgTable->getCellBottomMargin();
      double dDefaultLeftMargin   = pDgTable->getCellLeftMargin();
      double dDefaultRightMargin  = pDgTable->getCellRightMargin();

      switch( pDgTableCell->getCellMarginType() )
      {
        case OdDgTableCellElement::kNone:
        {
          dTopMargin    = 0;
          dBottomMargin = 0;
          dLeftMargin   = 0;
          dRightMargin  = 0;
        } break;

        case OdDgTableCellElement::kNarrow:
        {
          dTopMargin    = dDefaultTopMargin;
          dBottomMargin = dDefaultBottomMargin;
          dLeftMargin   = dDefaultTopMargin;
          dRightMargin  = dDefaultBottomMargin;
        } break;

        case OdDgTableCellElement::kNormal:
        {
          dTopMargin    = dDefaultTopMargin;
          dBottomMargin = dDefaultBottomMargin;
          dLeftMargin   = dDefaultLeftMargin;
          dRightMargin  = dDefaultRightMargin;
        } break;

        case OdDgTableCellElement::kWide:
        {
          dTopMargin    = dDefaultTopMargin + dDefaultLeftMargin;
          dBottomMargin = dDefaultTopMargin + dDefaultLeftMargin;
          dLeftMargin   = dDefaultTopMargin + dDefaultLeftMargin;
          dRightMargin  = dDefaultTopMargin + dDefaultLeftMargin;
        } break;
      }

      pDbTable->setMargin( i, j, OdDb::kCellMarginTop, dTopMargin );
      pDbTable->setMargin( i, j, OdDb::kCellMarginBottom, dBottomMargin );
      pDbTable->setMargin( i, j, OdDb::kCellMarginLeft, dLeftMargin );
      pDbTable->setMargin( i, j, OdDb::kCellMarginRight, dRightMargin );

      // Import cell fill.

      bool      bEnableFill = false;
      OdCmColor clrFillColor;

      if( pDgTableCell->getFillFlag() )
      {
         bEnableFill = true;
         setDwgColorByDgnIndex( pDgTable->database(), clrFillColor, pDgTableCell->getFillColor() );
      }
      else if( pDgTableCell->getTableCellType() == OdDgTableCellElement::kBody )
      {
        OdUInt32 uBodyRowNum = i - pDgTable->getTitleRowCount() - pDgTable->getHeaderRowCount();

        if( (uBodyRowNum%2 == 0) && pDgTable->getOddFillColorFlag() )
        {
          bEnableFill = true;
          setDwgColorByDgnIndex( pDgTable->database(), clrFillColor, pDgTable->getOddFillColor() );
        }
        else if( (uBodyRowNum%2 == 1) && pDgTable->getEvenFillColorFlag() )
        {
          bEnableFill = true;
          setDwgColorByDgnIndex( pDgTable->database(), clrFillColor, pDgTable->getEvenFillColor() );
        }
      }

      if( bEnableFill )
      {
        pDbTable->setBackgroundColorNone(i, j, false);
        pDbTable->setBackgroundColor(i, j, clrFillColor );
      }

      // Import cell content, alignment and rotation.

      OdDgTextNode3dPtr pDgCellContent = pDgTableCell->getCellText();

      if( pDgCellContent.isNull() )
      {
        pDbTable->setTextHeight( i, j, pDgTable->getDefaultTextSize() );
      }
      else
      {
        OdString strContent = createMTextContent( owner->database(), pDgCellContent );

        if( strContent.isEmpty() )
        {
          pDbTable->setTextHeight( i, j, pDgTable->getDefaultTextSize() );
        }
        else
        {
          pDbTable->setTextHeight( i, j, pDgCellContent->getHeightMultiplier() );
          pDbTable->setTextString( i, j, strContent );
          OdDb::CellAlignment cellAlignment = getDbCellAlignment( pDgTableCell->getTextAlignment() );
          pDbTable->setAlignment( i, j, cellAlignment );

          if( pDgTableCell->getTextOrientation() == OdDgTableCellElement::kVertical )
          {
            bool bSetVertical = true;

            if( (strContent.find(L"\\f") != -1) || (strContent.find(L"\\F") != -1) )
            {
              pDbTable->setRotation( i, j, 0, 3*OdaPI2 );
              bSetVertical = false;
            }
            else if( idDbTextStyleVertical.isNull() && !idDbTextStyle.isNull() )
            {
              OdDbTextStyleTablePtr pDbTextStyleTable = owner->database()->getTextStyleTableId().openObject(OdDb::kForWrite);
              OdDbTextStyleTableRecordPtr pDbTextStyle = idDbTextStyle.openObject(OdDb::kForRead);
              OdDbTextStyleTableRecordPtr pDbTextStyleVertical = pDbTextStyle->clone();
              pDbTextStyleVertical->setIsVertical(true);
              pDbTextStyleVertical->setFont( L"txt.shx", false, false, 0, 0);
              
              OdString strTextStyleName = pDbTextStyle->getName()+L"_V";

              OdUInt32 uCount = 1;

              while( !pDbTextStyleTable->getAt(strTextStyleName).isNull() )
              {
                strTextStyleName.format(L"_V_%d",uCount);
                strTextStyleName = pDbTextStyle->getName() + strTextStyleName;
                uCount++;
              }

              pDbTextStyleVertical->setName(strTextStyleName);
              pDbTextStyleTable->add( pDbTextStyleVertical );
              idDbTextStyleVertical = pDbTextStyleVertical->objectId();
            }

            if( bSetVertical && !idDbTextStyleVertical.isNull() )
              pDbTable->setTextStyle(i,j, 0, idDbTextStyleVertical);
          }
          else
            pDbTable->setRotation( i, j, 0, getDbCellTextRotation( pDgTableCell->getTextOrientation() ) );
        }
      }
    }

  // Import merges.

  for( i = 0; i < (OdInt32)pDgTable->getMergeCount(); i++ )
  {
    OdDgTableElementMerge curMerge = pDgTable->getMerge(i);

    pDbTable->mergeCells( curMerge.getBaseRowIndex(), curMerge.getBaseRowIndex() + curMerge.getRowCount() - 1,
                          curMerge.getBaseColumnIndex(), curMerge.getBaseColumnIndex() + curMerge.getColumnCount() - 1 );
  }

  // Import table column properties.

  for( i = 0; i < (OdInt32)pDgTable->getColumnCount(); i++ )
    pDbTable->setColumnWidth(i, pDgTable->getColumn(i)->getWidth() );

  // Import table row properties.

  for( i = 0; i < (OdInt32)pDgTable->getRowCount(); i++ )
    pDbTable->setRowHeight(i, pDgTable->getRow(i)->getHeight() );

  // Import table break properties.

  if( pDgTable->getBreakType() != OdDgTableElement::kNoBreak )
  {
    OdDb::TableBreakOption breakOptions = OdDb::kTableBreakEnableBreaking;

    if( pDgTable->getRepeatHeadersFlag() )
      breakOptions = (OdDb::TableBreakOption)((OdUInt16)(breakOptions) | OdDb::kTableBreakRepeatTopLabels);

    if( pDgTable->getRepeatFootersFlag() )
      breakOptions = (OdDb::TableBreakOption)((OdUInt16)(breakOptions) | OdDb::kTableBreakRepeatBottomLabels);

    pDbTable->setBreakOption( breakOptions );
    pDbTable->setBreakHeight( 0, pDgTable->getBreakHeight() );
    pDbTable->setBreakSpacing( pDgTable->getSubTableSpacing() );
    pDbTable->setBreakFlowDirection( getFlowDirection(pDgTable->getSubTablePosition()) );
  }

  // Save dwg table id to map

  OdDgnImportPathToDwgObject dwgPath;
  dwgPath.m_idPath.objectIds().push_back( pDbTable->objectId() );
  dwgPath.m_bExists = true;
  OdDgnImportContext::addObjectPath( pDgTable->elementId(), dwgPath );
}

//---------------------------------------------------------------------------------------------------

}
