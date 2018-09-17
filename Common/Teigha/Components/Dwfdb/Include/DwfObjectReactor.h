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
#ifndef __DWF_OBJECTREACTOR_H__
#define __DWF_OBJECTREACTOR_H__

#include "RxObject.h"
#include "DwfdbExport.h"

class OdDwfObject;

class DWFDBEXPORT OdDwfObjectReactor : public OdRxObject
{
public:
  ODRX_DECLARE_MEMBERS(OdDwfObjectReactor);

  virtual void copied(const OdDwfObject* pElement,
                      const OdDwfObject* pNewElement) {}
  virtual void erased(const OdDwfObject* pElement, bool erasing = true) {}

  virtual void goodbye(const OdDwfObject* pElement) {}

  virtual void openedForModify(const OdDwfObject* pElement) {}

  virtual void modified(const OdDwfObject* pElement) {}

  virtual void modifiedGraphics(const OdDwfObject* pElement) {}

  virtual void modifiedLinkages(const OdDwfObject* pElement) {}

  virtual void subElementModified(const OdDwfObject* pElement,
                                  const OdDwfObject* pSubElm) {}

  virtual void modifyUndone(const OdDwfObject* pElement) {}

  virtual void unappended(const OdDwfObject* pElement) {}

  virtual void reappended(const OdDwfObject* pElement) {}

  virtual void closed(class OdDbStub* elementId) {}
};
typedef OdSmartPtr<OdDwfObjectReactor> OdDwfObjectReactorPtr;

typedef OdDwfObjectReactor OdDwfDatabaseReactor;
typedef OdSmartPtr<OdDwfDatabaseReactor> OdDwfDatabaseReactorPtr;

#endif // __DWF_OBJECTREACTOR_H__
