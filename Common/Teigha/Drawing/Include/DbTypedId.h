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

#ifndef _ODDBTYPEDID_INCLUDED_
#define _ODDBTYPEDID_INCLUDED_

#include "TD_PackPush.h"

#include "DbObjectId.h"


/** \details
    This class adds reference types to memory-resident Object ID objects for OdDbDatabase objects.  

    <group OdDb_Classes>
*/
class OdDbTypedId : public OdDbObjectId
{
  OdDb::ReferenceType m_rt;

public:
  /** \param source [in]  Source object.
    \param refType [in]  Reference type.
  */
  OdDbTypedId( OdDbObjectId source = 0, OdDb::ReferenceType refType = OdDb::kSoftPointerRef ) 
      : OdDbObjectId(source), m_rt(refType) { }

  /** \details
    Returns the refernece type for this TypedId object.
  */
	OdDb::ReferenceType getRefType() const { return m_rt; }
  /** \details
    Sets the refernece type for this TypedId object.

    \param refType [in]  Reference type.
  */
	void setRefType(OdDb::ReferenceType refType) { m_rt = refType; }
	OdDbTypedId& operator=(const OdDbObjectId& rhs)	{	*((OdDbObjectId*)this) = rhs; return *this; }
};

#include "TD_PackPop.h"

#endif //_ODDBTYPEDID_INCLUDED_
