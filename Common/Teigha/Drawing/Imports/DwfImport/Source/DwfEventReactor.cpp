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

#include "StdAfx.h"
#include "OdaCommon.h"
#include "DbObject.h"
#include "OdStreamBuf.h"
#include "RxSysRegistry.h"
#include "DwfEventReactor.h"
#include "EmbeddedImageDef.h"
#include "RxModule.h"

namespace TD_DWF_IMPORT {
//ODRX_CONS_DEFINE_MEMBERS(DwfEventReactor, OdRxEventReactor, RXIMPL_CONSTR)

DwfEventReactor::DwfEventReactor() : _db(0)
{
}

void DwfEventReactor::beginSave(OdDbDatabase* db, const OdString& pIntendedName)
{
  if (_db != db) return;
  for (unsigned int i = 0; i < _images.size(); i++)
  {
    _images[i]->beginSave(pIntendedName);
  }
}
void DwfEventReactor::addImage(EmbeddedImageDefPtr im)
{
  _images.push_back(im);
}

OdRxModule* DwfEventReactor::m_sModuleLock = 0;

void DwfEventReactor::databaseToBeDestroyed(OdDbDatabase* db)
{
  if (db != _db) return;
  if ( m_sModuleLock ) m_sModuleLock->release();
  odrxEvent()->removeReactor(this);
  ReactorMap::iterator i = _reactorMap.find(db);
  ODA_ASSERT(i != _reactorMap.end());
  _reactorMap.erase(i);
}

DwfEventReactor::ReactorMap DwfEventReactor::_reactorMap;

DwfEventReactor* DwfEventReactor::getReactor(OdDbDatabase* db)
{
  ReactorMap::iterator i = _reactorMap.find(db);
  if ( i != _reactorMap.end() ) return i->second;
  OdRxEventReactorPtr r = DwfEventReactor::createObject();
  odrxEvent()->addReactor(r);
  if ( m_sModuleLock ) m_sModuleLock->addRef();
  DwfEventReactor* der = static_cast<DwfEventReactor*>( r.get() );
  der->_db = db;
  _reactorMap[db] = der;
  return der;
}
}
