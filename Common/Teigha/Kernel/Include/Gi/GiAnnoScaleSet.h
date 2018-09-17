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

#ifndef __ODGIANNOSCALESET_H__
#define __ODGIANNOSCALESET_H__

#include "TD_PackPush.h"

#include "Gi/GiExport.h"
#include "DbStubPtrArray.h"

/** \details
    Annotation scales array.

    \sa
    TD_Gi
    <group OdGi_Classes>
*/
class ODGI_EXPORT OdGiAnnoScaleSet
{
public:
  OdGiAnnoScaleSet() : m_defaultId(NULL){}
  OdGiAnnoScaleSet(OdDbStubPtrArray& aSortedId, OdDbStub* defaultId) : m_aId(aSortedId), m_defaultId(defaultId){}
  OdGiAnnoScaleSet(const OdGiAnnoScaleSet& v) : m_aId(v.m_aId), m_defaultId(v.m_defaultId){}
  bool operator < (const OdGiAnnoScaleSet& v) const;
  void set(OdDbStubPtrArray& aSortedId, OdDbStub* defaultId)
  {
    m_aId = aSortedId; m_defaultId = defaultId;
  }
  void clear() { m_aId.clear(); m_defaultId = NULL; }
  const OdDbStubPtrArray &annoScaleIds() const { return m_aId; }
  OdDbStub* defaultId() const { return m_defaultId; }

protected:
  OdDbStubPtrArray m_aId;//sorted
  OdDbStub* m_defaultId;
};

#include "TD_PackPop.h"

#endif // __ODGIANNOSCALESET_H__
