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
#ifndef __DG_PROXYELEMENT_H__
#define __DG_PROXYELEMENT_H__


#include "TD_PackPush.h"

#include "DgElement.h"

/** \details
   
    Corresponding C++ library: TG_Db 

    <group OdDg_Classes>
*/
class TG_EXPORT OdDgProxyElement : public OdDgElement
{
  ODDG_DECLARE_MEMBERS_INTERNAL(OdDgProxyElement);

public:
  virtual void     getData(OdBinaryData& data) const;
  virtual bool     subWorldDraw(OdGiWorldDraw* pWd) const;
  virtual OdUInt32 subSetAttributes(OdGiDrawableTraits* pTraits) const;
};

typedef OdSmartPtr<OdDgProxyElement> OdDgProxyElementPtr;

/** \details
   
    Corresponding C++ library: TG_Db 

    <group OdDg_Classes>
*/
class TG_EXPORT OdDgComplexProxyElement : public OdDgElement
{
  ODDG_DECLARE_MEMBERS_INTERNAL(OdDgComplexProxyElement);
public:

  /** \details
    Returns an Iterator object that can be used to traverse this collection object.

    \param atBeginning [in]  True to start at the beginning, false to start at the end. 
    \param skipDeleted [in]  If and only if true, deleted objects are skipped.
  */
  OdDgElementIteratorPtr createIterator( bool atBeginning = true, bool skipDeleted = true ) const;

  virtual void     getData(OdBinaryData& data) const;
  virtual bool     subWorldDraw(OdGiWorldDraw* pWd) const;
  virtual OdUInt32 subSetAttributes(OdGiDrawableTraits* pTraits) const;
};

typedef OdSmartPtr<OdDgComplexProxyElement> OdDgComplexProxyElementPtr;


#include "TD_PackPop.h"

#endif // __DG_PROXYELEMENT_H__
