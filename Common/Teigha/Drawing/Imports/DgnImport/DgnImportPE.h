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

#ifndef _DGN_IMPORT_PE_INCLUDED_ 
#define _DGN_IMPORT_PE_INCLUDED_

#include "OdaCommon.h"
#include "DbBlockTableRecord.h"
#include "DgElement.h"
#include "DgnImportCommon.h"

namespace TD_DGN_IMPORT 
{

//------------------------------------------------------------------------------

class OdDgnImportPE : public OdRxObject
{
public:
  ODRX_DECLARE_MEMBERS(OdDgnImportPE);
  virtual void importElement(OdDgElement*, OdDbBlockTableRecord* owner);
  virtual bool allowImportElement(OdDgElement*);
  virtual void subImportElement(OdDgElement*, OdDbBlockTableRecord* owner);
};

//------------------------------------------------------------------------------

struct IgnoredElementImportPE : OdDgnImportPE
{
public:
  void subImportElement(OdDgElement*, OdDbBlockTableRecord*) ODRX_OVERRIDE;
};

//------------------------------------------------------------------------------

template<class T> struct OdDgnComplexElementImportPE : OdDgnImportPE
{
public:
  void subImportElement(OdDgElement* e, OdDbBlockTableRecord* owner) ODRX_OVERRIDE
  {
    T* node = static_cast<T*>(e);

    for (OdDgElementIteratorPtr iterator = node->createIterator(); !iterator->done(); iterator->step() )
    {
      OdDgElementPtr subElement = iterator->item().openObject();

      if (!subElement.isNull())
        ((OdDgnImportPE*)subElement->queryX(OdDgnImportPE::desc()))->importElement(subElement, owner);
    }
  }
};

//------------------------------------------------------------------------------


}
#endif // _DGN_IMPORT_PE_INCLUDED_
