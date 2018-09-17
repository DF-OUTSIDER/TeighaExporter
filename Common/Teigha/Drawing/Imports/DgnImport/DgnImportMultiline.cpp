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
#include "DgnImportMultiline.h"
#include "DgnImportCommon.h"
#include "DgMultiline.h"
#include <DbMline.h>
#include "DgView.h"
#include "DgnImportContext.h"

namespace TD_DGN_IMPORT {

//---------------------------------------------------------------------------------------------------

Mline::MlineJustification convertMLineOffset( OdDgMultiline::OffsetMode uMode )
{
  Mline::MlineJustification uRet = Mline::kZero;

  switch( uMode )
  {
    case OdDgMultiline::kByWorkLine:
    case OdDgMultiline::kByCenter:
    {
      uRet = Mline::kZero;
    } break;

    case OdDgMultiline::kByMaximum:
    {
      uRet = Mline::kTop;
    } break;

    case OdDgMultiline::kByMinimum:
    {
      uRet = Mline::kBottom;
    } break;
  }

  return uRet;
}

//---------------------------------------------------------------------------------------------------

OdDbObjectId getMLineStyle( const OdDgMultiline* pMLine, OdDbDatabase* pDb )
{
  OdDbObjectId idRet;

  // Create descriptor

  OdDgMultilineSymbology startCap;
  OdDgMultilineSymbology endCap;
  OdDgMultilineSymbology middleCap;

  pMLine->getOriginCap(startCap);
  pMLine->getEndCap(endCap);
  pMLine->getMiddleCap( middleCap );

  bool      bFillMLine = false;
  OdUInt32  uFillColor = 0;

  OdDgViewPtr pActiveView = OdDgnImportContext::getActiveView();

  if( pActiveView.isNull() || pActiveView->getShowFillsFlag() )
  {
    OdRxObjectPtrArray arrLinkages;

    pMLine->getLinkages( OdDgAttributeLinkage::kFillStyle, arrLinkages );

    for( OdUInt32 j = 0; j < arrLinkages.size(); j++ )
    {
      if( arrLinkages[j]->isKindOf( OdDgFillColorLinkage::desc()) )
      {
        OdDgFillColorLinkagePtr pFillLinkage = arrLinkages[j];

        uFillColor = pFillLinkage->getColorIndex();
        bFillMLine = true;
        break;
      }
    }
  }

  OdDgnImportMultilineDescriptor mlineDesc;
  mlineDesc.m_bShowJoints          = middleCap.getCapLineFlag();
  mlineDesc.m_bShowStartCapLine    = startCap.getCapLineFlag();
  mlineDesc.m_bShowStartCapInArc   = startCap.getCapInArcFlag();
  mlineDesc.m_bShowStartCapOutArc  = startCap.getCapOutArcFlag();
  mlineDesc.m_bShowEndCapLine      = endCap.getCapLineFlag();
  mlineDesc.m_bShowEndCapInArc     = endCap.getCapInArcFlag();
  mlineDesc.m_bShowEndCapOutArc    = endCap.getCapOutArcFlag();
  mlineDesc.m_dStartCapAngle       = pMLine->getOriginCapAngle();
  mlineDesc.m_dEndCapAngle         = pMLine->getEndCapAngle();
  mlineDesc.m_bShowFill            = bFillMLine;
  mlineDesc.m_uJustification       = pMLine->getOffsetMode();
  mlineDesc.m_uFillColor           = uFillColor;
  mlineDesc.m_uMLineStyle          = pMLine->getMultilineStyleId();
  mlineDesc.m_bTableMLineStyle     = false;

  for( OdUInt32 i = 0; i < pMLine->getProfilesCount(); i++ )
  {
    OdDgMultilineProfile curProfile;
    pMLine->getProfile( i, curProfile );
    mlineDesc.m_arrOffsets.push_back( curProfile.getDistance() );
    mlineDesc.m_arrColorIndexes.push_back( curProfile.symbology().getColorIndex() );
    mlineDesc.m_arrLineStyles.push_back( curProfile.symbology().getLineStyleEntryId() );
  }

  idRet = OdDgnImportContext::getMultilineStyle( mlineDesc );

  if( idRet.isNull() )
  {
    idRet = OdDgnImportContext::createMultilineStyle( mlineDesc, pDb, pMLine->database() );
  }

  OdDgnImportContext::setResourceUsage(idRet);

  return idRet;
}

//---------------------------------------------------------------------------------------------------

void OdDgnMultilineImportPE::subImportElement(OdDgElement* e, OdDbBlockTableRecord* owner)
{
  OdDgMultiline* pDgnMLine = (OdDgMultiline*)(e);

  OdDbMlinePtr pDbMLine = OdDbMline::createObject();
  owner->appendOdDbEntity( pDbMLine );

  pDbMLine->setStyle( getMLineStyle( pDgnMLine, owner->database() ) );

  for( OdUInt32 i = 0; i < pDgnMLine->getPointsCount(); i++ )
  {
    OdDgMultilinePoint ptMLine;
    pDgnMLine->getPoint(i, ptMLine);
    OdGePoint3d pt3dMLine;
    ptMLine.getPoint(pt3dMLine);

    pDbMLine->appendSeg( pt3dMLine );
  }

  OdDgMultilineSymbology startCap;
  OdDgMultilineSymbology endCap;
  pDgnMLine->getOriginCap(startCap);
  pDgnMLine->getEndCap(endCap);

  pDbMLine->setJustification( convertMLineOffset(pDgnMLine->getOffsetMode()) );
  pDbMLine->setNormal( pDgnMLine->getZVector() );
  pDbMLine->setClosedMline( pDgnMLine->getClosedFlag() );
  pDbMLine->setSupressStartCaps( !startCap.getCapOutArcFlag() && !startCap.getCapInArcFlag() && !startCap.getCapLineFlag() );
  pDbMLine->setSupressEndCaps( !endCap.getCapOutArcFlag() && !endCap.getCapInArcFlag() && !endCap.getCapLineFlag() );

  copyEntityProperties( pDgnMLine, pDbMLine );

  OdDgnImportPathToDwgObject dwgPath;
  dwgPath.m_idPath.objectIds().push_back( pDbMLine->objectId() );
  dwgPath.m_bExists = true;
  OdDgnImportContext::addObjectPath( e->elementId(), dwgPath );
}

//---------------------------------------------------------------------------------------------------

}
