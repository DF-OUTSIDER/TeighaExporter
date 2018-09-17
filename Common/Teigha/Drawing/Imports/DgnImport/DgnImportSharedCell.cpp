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
#include "DgnImportSharedCell.h"
#include "DgnImportCommon.h"
#include "DgnImportContext.h"
#include "DbSymUtl.h"

#include <DbObjectId.h>
#include <DbBlockReference.h>
#include <DgSharedCellReference.h>
#include <DbBlockTable.h>
#include <DbSpatialFilter.h>

namespace TD_DGN_IMPORT {

//---------------------------------------------------------------------------------------------------

void OdDgSharedCellReferenceImportPE::subImportElement(OdDgElement* e, OdDbBlockTableRecord* owner)
{
  OdDgSharedCellReference* r = (OdDgSharedCellReference*)e;
  OdDbBlockTablePtr bt = owner->database()->getBlockTableId().safeOpenObject();
  OdDgSharedCellDefinitionPtr pDef = r->findDefinition();

  OdDgElementId idCellDef;

  if( !pDef.isNull() )
  {
    idCellDef = pDef->elementId();
  }

  OdDbObjectId blockId = OdDgnImportContext::getObjectId( idCellDef );

  if (blockId.isNull())
  {
    ODA_FAIL_ONCE();
    return;
  }

  bool bSkipTransform = false;

  if( r->getColorOverrideFlag() /*|| r->getLevelOverrideFlag()*/ )
  {
    OdDbBlockTableRecordPtr block = OdDbBlockTableRecord::createObject();

    OdString name = pDef->getName();

    if( name.isEmpty() ) // empty name => user anonymous block
    {
      name = L"*U";
    }
    else if( name[0] == '*' ) // anonymous blocks will be reindexed
    {
      if( name.getLength() < 2 )
      {
        name = L"*U";
      }
      else if( name.getLength() > 2 )
      {
        name = name.left(2);
      }
    }
    else
    {
      OdString repairedName;

      if( OdDbSymUtil::repairSymbolName(repairedName, name, owner->database()) == eOk && !repairedName.isEmpty() )
      {
        name = repairedName;
      }
    }

    block->setComments(pDef->getDescription());
    block->setOrigin(OdGePoint3d(0.0,0.0,0.0));

    owner->database()->addOdDbObject(block, bt->objectId());

    try
    {
      block->setName(name);
      bt->add(block);
    }
    catch (const OdError&)
    {
      block->setName(L"*U");
      bt->add(block);
    }

    blockId = block->objectId();

    OdDgnImportPE::subImportElement( e, block );

    bSkipTransform = true;
  }

  OdDbBlockReferencePtr dbRef = OdDbBlockReference::createObject();
  dbRef->setDatabaseDefaults(owner->database());
  owner->database()->addOdDbObject(dbRef, owner->objectId());
  copyEntityProperties(r, dbRef);
  owner->appendOdDbEntity(dbRef);
  dbRef->setBlockTableRecord(blockId);

  if( bSkipTransform )
  {
    dbRef->setBlockTransform(OdGeMatrix3d::kIdentity);
  }
  else
  {
    OdGeMatrix3d matTransform = OdGeMatrix3d::translation(r->getOrigin().asVector()) * r->getTransformation();

    prepareMatrixToBlockTransform( matTransform );

    dbRef->setBlockTransform(matTransform);
  }

  OdDgCellBoundaryClipPEPtr pCellBoundaryClip = OdDgCellBoundaryClipPEPtr(OdRxObjectPtr(e));

  if( !pCellBoundaryClip.isNull() )
  {
    OdGiClipBoundary clipBoundary;

    if( pCellBoundaryClip->getBoundaryClip(e, clipBoundary) )
    {
      clipBoundary.m_xInverseBlockRefXForm = dbRef->blockTransform().invert();

      if( dbRef->extensionDictionary().isNull() )
      {
        dbRef->createExtensionDictionary();
      }

      OdDbDictionaryPtr pDict = OdDbDictionary::cast(dbRef->extensionDictionary().openObject(OdDb::kForWrite));

      if( !pDict.isNull() )
      {
        OdDbDictionaryPtr pFDict = OdDbDictionary::cast(pDict->getAt(OD_T("ACAD_FILTER"), OdDb::kForWrite));

        if( pFDict.isNull() )
        {
          pFDict = OdDbDictionary::createObject();
          pDict->setAt( OD_T("ACAD_FILTER"), pFDict );
        }

        if( !pFDict.isNull() )
        {
          OdDbSpatialFilterPtr pSp = OdDbSpatialFilter::cast(pFDict->getAt(OD_T("SPATIAL"), OdDb::kForWrite));

          if( pSp.isNull() )
          {
            pSp = OdDbSpatialFilter::createObject();
            pFDict->setAt( OD_T("SPATIAL"), pSp );
          }

          if( !pSp.isNull() )
          {
            double dFrontClip = ODDB_INFINITE_XCLIP_DEPTH;
            double dBackClip = ODDB_INFINITE_XCLIP_DEPTH;

            if( clipBoundary.m_bClippingFront )
            {
              dFrontClip = clipBoundary.m_dFrontClipZ;
            }

            if( clipBoundary.m_bClippingBack )
            {
              dBackClip = clipBoundary.m_dBackClipZ;
            }
            pSp->setDefinition( clipBoundary );
          }
        }
      }
    }
  }


  OdDgnImportPathToDwgObject dwgPath;
  dwgPath.m_idPath.objectIds().push_back( dbRef->objectId() );
  dwgPath.m_bExists = true;
  OdDgnImportContext::addObjectPath( e->elementId(), dwgPath );
}

//---------------------------------------------------------------------------------------------------

}
