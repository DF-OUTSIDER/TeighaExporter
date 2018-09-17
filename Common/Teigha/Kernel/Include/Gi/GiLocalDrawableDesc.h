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




// GiBaseVectorizer.h: interface for the OdGiBaseVectorizer class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _ODGILOCALDRAWABLEDESC_INCLUDED_
#define _ODGILOCALDRAWABLEDESC_INCLUDED_


#include "TD_PackPush.h"

#include "Gi/GiPathNode.h"

/** \details
    
    <group OdGi_Classes> 
*/
struct OdGiLocalDrawableDesc : OdGiDrawableDesc, OdGiPathNode
{
  OdGiDrawableDesc*& _pPlaceToRestore;
  OdGsMarker         _nSelectionMarker;
  inline OdGiLocalDrawableDesc(OdGiDrawableDesc*& pParentDesc)
    : _pPlaceToRestore(pParentDesc)
    , _nSelectionMarker(0) /* kNullSubentIndex */
  {
    pParent = pParentDesc;
    _pPlaceToRestore = this;
  }
  inline ~OdGiLocalDrawableDesc()
  {
    _pPlaceToRestore = pParent;
  }

  // OdGsPathNode interface
  const OdGiPathNode* parent() const
  {
    return static_cast<const OdGiLocalDrawableDesc*>(pParent);
  }

  OdDbStub* persistentDrawableId() const
  {
    return persistId;
  }

  const OdGiDrawable* transientDrawable() const
  {
    return pTransientDrawable;
  }

  OdGsMarker selectionMarker() const
  {
    return _nSelectionMarker;
  }
};


#include "TD_PackPop.h"

#endif // #ifndef _ODGILOCALDRAWABLEDESC_INCLUDED_
