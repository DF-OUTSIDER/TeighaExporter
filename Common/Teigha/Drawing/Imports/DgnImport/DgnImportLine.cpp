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
#include "DgnImportLine.h"
#include "DgnImportCommon.h"
#include "DgnImportContext.h"
#include <DgLine.h>
#include <DbLine.h>

namespace TD_DGN_IMPORT {

//---------------------------------------------------------------------------------------------------

void OdDgnLine2dImportPE::subImportElement(OdDgElement* e, OdDbBlockTableRecord* owner)
{
  OdDgLine2d* l = (OdDgLine2d*)e;
  OdGePoint2d p1, p2;
  p1 = l->getStartPoint();
  p2 = l->getEndPoint();
  OdDbLinePtr dbLine = OdDbLine::createObject();
  // TODO: normal?
  dbLine->setStartPoint(OdGePoint3d(p1.x, p1.y, 0));
  dbLine->setEndPoint(OdGePoint3d(p2.x, p2.y, 0));
  dbLine->setDatabaseDefaults(owner->database());
  dbLine->setThickness(l->getThickness());
  owner->database()->addOdDbObject(dbLine, owner->objectId());
  copyEntityProperties(l, dbLine);
  owner->appendOdDbEntity(dbLine);

  OdDgnImportPathToDwgObject dwgPath;
  dwgPath.m_idPath.objectIds().push_back( dbLine->objectId() );
  dwgPath.m_bExists = true;
  OdDgnImportContext::addObjectPath( l->elementId(), dwgPath );
}

//---------------------------------------------------------------------------------------------------

void OdDgnLine3dImportPE::subImportElement(OdDgElement* e, OdDbBlockTableRecord* owner)
{
  OdDgLine3d* l = (OdDgLine3d*)e;
  OdGePoint3d p1, p2;
  p1 = l->getStartPoint();
  p2 = l->getEndPoint();
  OdDbLinePtr dbLine = OdDbLine::createObject();
  dbLine->setStartPoint(p1);
  dbLine->setEndPoint(p2);
  dbLine->setDatabaseDefaults(owner->database());
  dbLine->setThickness(l->getThickness());
  owner->database()->addOdDbObject(dbLine, owner->objectId());
  copyEntityProperties(l, dbLine);
  owner->appendOdDbEntity(dbLine);

  OdDgnImportPathToDwgObject dwgPath;
  dwgPath.m_idPath.objectIds().push_back( dbLine->objectId() );
  dwgPath.m_bExists = true;
  OdDgnImportContext::addObjectPath( l->elementId(), dwgPath );
}

//---------------------------------------------------------------------------------------------------

}
