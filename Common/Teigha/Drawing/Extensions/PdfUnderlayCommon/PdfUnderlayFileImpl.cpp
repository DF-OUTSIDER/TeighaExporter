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


#include "OdaCommon.h"
#include "RxObject.h"
#include "RxObjectImpl.h"
#include "OdString.h"

#include "Gs/Gs.h"
#include "Gs/GsBaseVectorizer.h"

#include "Gi/GiRasterImage.h"

#include "PdfDirectWrapper.h"
#include "PdfUnderlayFileImpl.h"
#include "PdfUnderlayItemImpl.h"

ODRX_NO_CONS_DEFINE_MEMBERS(PdfUnderlayFileImpl, OdDbUnderlayFile);

PdfUnderlayFileImpl::PdfUnderlayFileImpl()
{
}

OdResult PdfUnderlayFileImpl::loadFile(OdString path, OdString password)
{
  // Read PDF.
  PdfDirectWrapperPtr wrapper = PdfDirectWrapper::createObjectImpl(); // Stored inside item
  
  OdResult res = wrapper->loadFile(path, password);
  if (res != eOk) 
  {
    return res;
  }
  
  int n_items = wrapper->getItemsCount();

  m_UnderlayItems.clear();
  m_UnderlayItems.reserve(n_items);

  for (int i = 0; i < n_items; i++)
  {
    PdfUnderlayItemImplPtr item = OdRxObjectImpl<PdfUnderlayItemImpl>::createItem(wrapper, i, path, path, password);
    m_UnderlayItems.append(item);
  }

  return eOk;
}

int PdfUnderlayFileImpl::getItemCount() const 
{
  return m_UnderlayItems.size();
}

OdDbUnderlayItemPtr PdfUnderlayFileImpl::getItem(int i) const 
{
  return m_UnderlayItems[i]; 
}

OdDbUnderlayItemPtr PdfUnderlayFileImpl::getItem(OdString name) const 
{
  OdArray<OdDbUnderlayItemPtr>::const_iterator pIt  = m_UnderlayItems.begin();
  OdArray<OdDbUnderlayItemPtr>::const_iterator pEnd = m_UnderlayItems.end();
  
  while(pIt != pEnd)
  {
    if ((*pIt)->getName() == name)
      return *pIt;

    ++pIt;
  }
  
  return OdDbUnderlayItemPtr(); 
}

//
