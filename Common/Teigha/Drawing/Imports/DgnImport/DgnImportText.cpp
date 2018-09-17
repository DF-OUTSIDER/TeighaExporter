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
#include "DgnImportText.h"
#include "DgnImportCommon.h"
#include <DgDatabase.h>
#include <DgText.h>
#include <DgTextStyleTableRecord.h>
#include <DbText.h>
#include <DgFontTableRecord.h>
#include <DbTextStyleTable.h>
#include <DbTextStyleTableRecord.h>
#include <DgTagElement.h>
#include <DbBlockReference.h>
#include <DbAttribute.h>
#include "DgnImportContext.h"
#include "DgnImportImpl.h"

namespace TD_DGN_IMPORT {

//---------------------------------------------------------------------------------------------------

static void setTextJustification(OdDbText* dbTxt, OdDg::TextJustification tj)
{
  switch (tj)
  {
  case OdDg::kLeftTop:
  case OdDg::kLeftMarginTop:
    dbTxt->setHorizontalMode(OdDb::kTextLeft);
    dbTxt->setVerticalMode(OdDb::kTextTop);
    break;
  case OdDg::kLeftCenter:
  case OdDg::kLeftMarginCenter:
    dbTxt->setHorizontalMode(OdDb::kTextLeft);
    dbTxt->setVerticalMode(OdDb::kTextVertMid);
    break;
  case OdDg::kLeftBottom:
  case OdDg::kLeftMarginBottom:
  case OdDg::kLeftDescender:
    dbTxt->setHorizontalMode(OdDb::kTextLeft);
    dbTxt->setVerticalMode(OdDb::kTextBase);
    break;
  case OdDg::kCenterTop:
    dbTxt->setHorizontalMode(OdDb::kTextCenter);
    dbTxt->setVerticalMode(OdDb::kTextTop);
    break;
  case OdDg::kCenterCenter:
    dbTxt->setHorizontalMode(OdDb::kTextCenter);
    dbTxt->setVerticalMode(OdDb::kTextVertMid);
    break;
  case OdDg::kCenterBottom:
  case OdDg::kCenterDescender:
    dbTxt->setHorizontalMode(OdDb::kTextCenter);
    dbTxt->setVerticalMode(OdDb::kTextBase);
    break;
  case OdDg::kRightTop:
  case OdDg::kRightMarginTop:
    dbTxt->setHorizontalMode(OdDb::kTextRight);
    dbTxt->setVerticalMode(OdDb::kTextTop);
    break;
  case OdDg::kRightCenter:
  case OdDg::kRightMarginCenter:
    dbTxt->setHorizontalMode(OdDb::kTextRight);
    dbTxt->setVerticalMode(OdDb::kTextVertMid);
    break;
  case OdDg::kRightBottom:
  case OdDg::kRightMarginBottom:
  case OdDg::kRightDescender:
    dbTxt->setHorizontalMode(OdDb::kTextRight);
    dbTxt->setVerticalMode(OdDb::kTextBase);
    break;
  }
}

//---------------------------------------------------------------------------------------------------

template<class T> static void setTextStyle(T* txt, OdDbText* dbTxt)
{
  OdDgElementId dgStyleId = txt->database()->getTextStyleTable()->getAt(txt->getTextStyleEntryId());

  OdDgTextStyleTableRecordPtr pDgTextStyle;

  if (dgStyleId.isValid())
    pDgTextStyle = dgStyleId.openObject(OdDg::kForRead);

  bool bEqualFonts = true;

  if( !pDgTextStyle.isNull() )
  {
    OdUInt32 uFontData = txt->getFontEntryId();
    OdUInt32 uFontStyleData = pDgTextStyle->getFontEntryId();

    bEqualFonts = uFontStyleData == uFontData;
  }

  if (dgStyleId.isValid() && bEqualFonts )
  {
    OdDbObjectId idDbTextStyle = OdDgnImportContext::getObjectId(dgStyleId);
    dbTxt->setTextStyle(idDbTextStyle);
  }
  else
  {
    OdDgFontTableRecordPtr pFontRec = txt->database()->getFontTable( OdDg::kForRead )->getFont(txt->getFontEntryId());

    if( pFontRec.isNull() )
    {
      pFontRec =  txt->database()->getFontTable( OdDg::kForRead )->getFont( txt->database()->appServices()->getFAST_FONT() );
    }

    if( pFontRec.isNull() )
    {
      return;
    }

    OdString styleName = L"Style-" + pFontRec->getName();

    if( txt->getVerticalFlag() )
    {
       styleName = L"Style-V-" + pFontRec->getName();
    }

    OdDbTextStyleTablePtr tst = dbTxt->database()->getTextStyleTableId().safeOpenObject(OdDb::kForWrite);
    OdDbObjectId styleId = tst->getAt(styleName);
    if (styleId.isValid())
    {
      OdDgnImportContext::setResourceUsage(styleId);
      dbTxt->setTextStyle(styleId);
    }
    else
    {
      OdDbTextStyleTableRecordPtr ts = OdDbTextStyleTableRecord::createObject();
      ts->setName(styleName);
      OdDbObjectId idStyle = tst->add(ts);

      if (pFontRec->getType() == kFontTypeShx)
      {
        ts->setFileName(pFontRec->getName());
      }
      else if( pFontRec->getType() == kFontTypeRsc && OdDgnImportContext::getDgnImporter() )
      {
        OdDgnImportContext::getDgnImporter()->createShxFontFromRscFont( pFontRec->getName() );
        ts->setFileName(pFontRec->getName() + L".shx");
      }
      else if (pFontRec->getType() == kFontTypeTrueType)
      {
        ts->setFont(pFontRec->getName(), false, false, 0, 0);
      }
      else
        return;

      ts->setIsVertical( txt->getVerticalFlag() );

      dbTxt->setTextStyle(idStyle);
    }
  }
}

//---------------------------------------------------------------------------------------------------

template<class T> static bool isUnderlineText(T* txt )
{
  bool bRet = false;

  if( txt->getUnderlineOverrideFlag() )
  {
    bRet = txt->getUnderlineFlag();
  }
  else
  {
    OdDgElementId dgStyleId = txt->database()->getTextStyleTable()->getAt(txt->getTextStyleEntryId());

    if( dgStyleId.isValid() )
    {
      OdDgTextStyleTableRecordPtr pTextStyle = dgStyleId.openObject( OdDg::kForRead );

      if( !pTextStyle.isNull() )
      {
        bRet = pTextStyle->getUnderlineFlag();
      }
    }
  }

  return bRet;
}

//---------------------------------------------------------------------------------------------------

template<class T> static bool isOverlineText(T* txt )
{
  bool bRet = false;

  if( txt->getOverlineOverrideFlag() )
  {
    bRet = txt->getOverlineFlag();
  }
  else
  {
    OdDgElementId dgStyleId = txt->database()->getTextStyleTable()->getAt(txt->getTextStyleEntryId());

    if( dgStyleId.isValid() )
    {
      OdDgTextStyleTableRecordPtr pTextStyle = dgStyleId.openObject( OdDg::kForRead );

      if( !pTextStyle.isNull() )
      {
        bRet = pTextStyle->getOverlineFlag();
      }
    }
  }

  return bRet;
}

//---------------------------------------------------------------------------------------------------

void OdDgText2dImportPE::subImportElement(OdDgElement* e, OdDbBlockTableRecord* owner)
{
  OdDgText2d* txt = (OdDgText2d*)e;

  if( OdZero(txt->getHeightMultiplier()) )
    return;

  OdDbTextPtr dbTxt = OdDbText::createObject();
  dbTxt->setDatabaseDefaults(owner->database());
  owner->database()->addOdDbObject(dbTxt, owner->objectId());
  copyEntityProperties(txt, dbTxt);
  owner->appendOdDbEntity(dbTxt);
  setTextStyle(txt, dbTxt);

  OdString strText = txt->getText();

  if( !OdDgnImportContext::getConvertEmptyDataFieldsToSpacesFlag() )
  {
    if( txt->getTextEditFieldCount() > 0 )
    {
      for( OdInt32 i = 0; i < txt->getTextEditFieldCount(); i++ )
      {
        OdDgTextEditField txtField = txt->getTextEditFieldAt(i);
        strText = convertEmptyDataFieldsTo_( strText, txtField );
      }
    }
  }

  strText.replace("\\","\\\\");

  if( isUnderlineText(txt) )
  {
    strText = L"%%u" + strText;
  }

  if( isOverlineText(txt) )
  {
    strText = L"%%o" + strText;
  }

  dbTxt->setTextString(strText);

  dbTxt->setHeight(fabs(txt->getHeightMultiplier()));

  if( txt->getHeightMultiplier() < 0 )
    dbTxt->mirrorInY(true);

  if( txt->getLengthMultiplier() < 0 )
    dbTxt->mirrorInX(true);

  setTextJustification(dbTxt, txt->getJustification());
  dbTxt->setRotation(txt->getRotation());
  OdGePoint2d p2(txt->getPosition());
  OdGePoint3d p(p2.x, p2.y, 0);
  dbTxt->setOblique(txt->getSlant());
  if (!OdZero(txt->getHeightMultiplier()) && !OdZero(txt->getLengthMultiplier()))
    dbTxt->setWidthFactor(fabs(txt->getLengthMultiplier())/fabs(txt->getHeightMultiplier()));
  if (dbTxt->verticalMode() == OdDb::kTextBase &&  dbTxt->horizontalMode() == OdDb::kTextLeft)
    dbTxt->setPosition(p);
  else
    dbTxt->setAlignmentPoint(p);
  dbTxt->adjustAlignment(owner->database());

  if( txt->getOffsetFlag() )
  {
    OdGeVector3d vrOffset( txt->getLineOffset().x, txt->getLineOffset().y, 0 );
    vrOffset.rotateBy( txt->getRotation(), OdGeVector3d::kZAxis );
    OdGeMatrix3d matTransform = OdGeMatrix3d::translation( vrOffset );
    dbTxt->transformBy(matTransform);
  }

  OdDgnImportPathToDwgObject dwgPath;
  dwgPath.m_idPath.objectIds().push_back( dbTxt->objectId() );
  dwgPath.m_bExists = true;
  OdDgnImportContext::addObjectPath( e->elementId(), dwgPath );
}

//---------------------------------------------------------------------------------------------------

void OdDgText3dImportPE::subImportElement(OdDgElement* e, OdDbBlockTableRecord* owner)
{
  OdDgText3d* txt = (OdDgText3d*)e;

  if( OdZero(txt->getHeightMultiplier()) )
    return;

  OdDbTextPtr dbTxt = OdDbText::createObject();
  dbTxt->setDatabaseDefaults(owner->database());
  owner->database()->addOdDbObject(dbTxt, owner->objectId());
  copyEntityProperties(txt, dbTxt);
  owner->appendOdDbEntity(dbTxt);
  setTextStyle(txt, dbTxt);

  OdString strText = txt->getText();

  if( !OdDgnImportContext::getConvertEmptyDataFieldsToSpacesFlag() )
  {
    if( txt->getTextEditFieldCount() > 0 )
    {
      for( OdInt32 i = 0; i < txt->getTextEditFieldCount(); i++ )
      {
        OdDgTextEditField txtField = txt->getTextEditFieldAt(i);
        strText = convertEmptyDataFieldsTo_( strText, txtField );
      }
    }
  }

  strText.replace("\\","\\\\");

  if( isUnderlineText(txt) )
  {
    strText = L"%%u" + strText;
  }

  if( isOverlineText(txt) )
  {
    strText = L"%%o" + strText;
  }

  dbTxt->setTextString(strText);

  dbTxt->setHeight(fabs(txt->getHeightMultiplier()));

  if( txt->getHeightMultiplier() < 0 )
    dbTxt->mirrorInY(true);

  if( txt->getLengthMultiplier() < 0 )
    dbTxt->mirrorInX(true);

  dbTxt->transformBy(txt->getRotation().getMatrix());
  setTextJustification(dbTxt, txt->getJustification());
  { //There are dgn files with invalid oblique angle
    double oblique = txt->getSlant();
    oblique -= floor(oblique/Oda2PI)*Oda2PI;
    if (oblique >= OdaPI)
      oblique -= Oda2PI;
    if (fabs(oblique) <= OdaToRadian(85.))
      dbTxt->setOblique(oblique);
  }
  if (!OdZero(txt->getHeightMultiplier()) && !OdZero(txt->getLengthMultiplier()))
    dbTxt->setWidthFactor(fabs(txt->getLengthMultiplier())/fabs(txt->getHeightMultiplier()));
  if (dbTxt->verticalMode() == OdDb::kTextBase &&  dbTxt->horizontalMode() == OdDb::kTextLeft)
    dbTxt->setPosition(txt->getPosition());
  else
    dbTxt->setAlignmentPoint(txt->getPosition());
  dbTxt->adjustAlignment(owner->database());

  if( txt->getOffsetFlag() )
  {
    OdGeVector3d vrOffset( txt->getLineOffset().x, txt->getLineOffset().y, 0 );
    vrOffset.transformBy( txt->getRotation().getMatrix() );
    OdGeMatrix3d matTransform = OdGeMatrix3d::translation( vrOffset );
    dbTxt->transformBy(matTransform);
  }

  OdDgnImportPathToDwgObject dwgPath;
  dwgPath.m_idPath.objectIds().push_back( dbTxt->objectId() );
  dwgPath.m_bExists = true;
  OdDgnImportContext::addObjectPath( e->elementId(), dwgPath );
}

//---------------------------------------------------------------------------------------------------

void OdDgTagImportPE::subImportElement(OdDgElement* e, OdDbBlockTableRecord* owner)
{
  OdDgTagElement* tag = (OdDgTagElement*)e;
  if (tag->getDataType() != OdDgTagDefinition::kChar)
    return;
  OdDbObjectId ownerId = owner->database()->getOdDbObjectId(tag->getAssociationId().getHandle());

  if( ownerId.isErased() || ownerId.isNull() )
  {
    // Try to export tag as text.
    OdDbTextPtr dbTxt = OdDbText::createObject();
    dbTxt->setDatabaseDefaults(owner->database());
    owner->database()->addOdDbObject(dbTxt, owner->objectId());
    copyEntityProperties(tag, dbTxt);
    owner->appendOdDbEntity(dbTxt);
    setTextStyle(tag, dbTxt);

    OdString strText = tag->getString();
    strText.replace("\\","\\\\");
    dbTxt->setTextString(strText);

    dbTxt->setHeight(fabs(tag->getHeightMultiplier()));

    if( tag->getHeightMultiplier() < 0 )
      dbTxt->mirrorInY(true);

    if( tag->getLengthMultiplier() < 0 )
      dbTxt->mirrorInX(true);

    OdGeMatrix3d matTransform;

    if( tag->get3dFormatFlag() )
    {
      matTransform = tag->getRotation3d().getMatrix();
    }
    else
    {
      matTransform = OdGeMatrix3d::rotation( tag->getRotation2d(), OdGeVector3d::kZAxis );
    }

    dbTxt->transformBy(matTransform);
    setTextJustification(dbTxt, tag->getJustification());
    { //There are dgn files with invalid oblique angle
      double oblique = tag->getSlant();
      oblique -= floor(oblique/Oda2PI)*Oda2PI;
      if (oblique >= OdaPI)
        oblique -= Oda2PI;
      if (fabs(oblique) <= OdaToRadian(85.))
        dbTxt->setOblique(oblique);
    }
    if (!OdZero(tag->getHeightMultiplier()) && !OdZero(tag->getLengthMultiplier()))
      dbTxt->setWidthFactor(fabs(tag->getLengthMultiplier())/fabs(tag->getHeightMultiplier()));
    if (dbTxt->verticalMode() == OdDb::kTextBase &&  dbTxt->horizontalMode() == OdDb::kTextLeft)
      dbTxt->setPosition(tag->getOrigin() );
    else
      dbTxt->setAlignmentPoint( tag->getOrigin() );

    if( tag->getOffsetUsedFlag() )
    {
      matTransform = OdGeMatrix3d::translation( tag->getOffset() );
      dbTxt->transformBy(matTransform);
    }

    dbTxt->adjustAlignment(owner->database());

    return;
  }
  OdDbBlockReferencePtr ownerBlockRef = OdDbBlockReference::cast(ownerId.openObject(OdDb::kForWrite));
  if (ownerBlockRef.isNull())
    return;
  OdDbAttributePtr dbAttr = OdDbAttribute::createObject();
  dbAttr->setDatabaseDefaults(owner->database());
  owner->database()->addOdDbObject(dbAttr, ownerBlockRef->objectId());
  copyEntityProperties(tag, dbAttr);
  ownerBlockRef->appendAttribute(dbAttr);
  setTextStyle(tag, dbAttr);
  dbAttr->setTextString(tag->getString());
  dbAttr->setHeight(fabs(tag->getHeightMultiplier()));
  setTextJustification(dbAttr, tag->getJustification());
  OdGeMatrix3d m = tag->getRotation3d().getMatrix();
  if (!m.isSingular())
  {
    OdGePoint3d o;
    OdGeVector3d vx, vy, vz;
    m.getCoordSystem(o, vx, vy, vz);
    double elevation;
    OdGeMatrix2d rotation = m.convertToLocal(vz, elevation);
    OdGeVector2d test(OdGeVector2d::kXAxis);
    test.transformBy(rotation);
    dbAttr->setRotation(test.angle());
    dbAttr->setNormal(vz);
  }
  try { //Where are dgn files with invalid oblique angle
    dbAttr->setOblique(tag->getSlant());
  }
  catch (OdError&)
  {}
  if (!OdZero(tag->getHeightMultiplier()) && !OdZero(tag->getLengthMultiplier()))
    dbAttr->setWidthFactor(fabs(tag->getLengthMultiplier())/fabs(tag->getHeightMultiplier()));
  dbAttr->setAlignmentPoint(tag->getOrigin() + tag->getOffset());
}

}
