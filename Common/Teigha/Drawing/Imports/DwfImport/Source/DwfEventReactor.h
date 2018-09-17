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

#ifndef __DWFEVENTREACTOR_H
#define __DWFEVENTREACTOR_H

#include "EmbeddedImageDef.h"
#include "RxEvent.h"
#include "RxObjectImpl.h"

#define STL_USING_MAP
#include "OdaSTL.h"

/** \details
  <group OdImport_Classes> 
*/
namespace TD_DWF_IMPORT {
class DwfEventReactor;

typedef OdSmartPtr<DwfEventReactor> DwfEventReactorPtr;

/** \details
  This class implements the DWF event reactor.
*/
class DwfEventReactor : public OdRxEventReactor  
{
public:
  DwfEventReactor();
  static OdRxEventReactorPtr createObject()
  {
    return OdRxObjectImpl<DwfEventReactor>::createObject();
  }
  virtual void beginSave(
    OdDbDatabase* pDb, 
    const OdString& intendedName);
  virtual void databaseToBeDestroyed(OdDbDatabase*);
  void addImage(EmbeddedImageDefPtr);
private:
  static OdRxModule* m_sModuleLock;
  OdArray<EmbeddedImageDefPtr> _images;
  OdDbDatabase* _db;
  typedef std::map<OdDbDatabase*, DwfEventReactor*> ReactorMap;
  static ReactorMap _reactorMap;
public:
  static DwfEventReactor* getReactor(OdDbDatabase*);
  static void setModule( OdRxModule* module ){ m_sModuleLock = module; }
};
}
#endif // __DWFEVENTREACTOR_H
