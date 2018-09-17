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




#ifndef __ODDRAWABLEIMPL_H__
#define __ODDRAWABLEIMPL_H__

class OdGsCache;
class OdGiViewportDraw;
class OdDbStub;

#include "TD_PackPush.h"

#include "Gs/GsModel.h"

/** \details

    <group OdGi_Classes> 
*/
template<class T = OdGiDrawable>
class OdGiDrawableImpl : public T
{
  OdGsCache* m_pGsNode;
public:
  OdGiDrawableImpl() : m_pGsNode(0) {}
  
  ~OdGiDrawableImpl() { if(m_pGsNode) m_pGsNode->setDrawableNull(); }

  void setGsNode(OdGsCache* pGsNode) { m_pGsNode = pGsNode; }

	OdGsCache* gsNode() const { return m_pGsNode; }

  void subViewportDraw(OdGiViewportDraw* ) const {}
  
  bool isPersistent() const { return false; }
  OdDbStub* id() const { return 0; }
};

#include "TD_PackPop.h"

#endif // __ODDRAWABLEIMPL_H__

