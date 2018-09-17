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
#ifndef __DG_REGAPPTABLERECORD_H__
#define __DG_REGAPPTABLERECORD_H__


#include "TD_PackPush.h"

#include "DgTableRecord.h"

/** \details
    This class represents RegApp records in the OdDgRegAppTable in an OdDgDatabase instance.

    Corresponding C++ library: TG_Db
    
    <group OdDg_Classes>
*/
class TG_EXPORT OdDgRegAppTableRecord : public OdDgTableRecord
{
  ODDG_DECLARE_MEMBERS_INTERNAL(OdDgRegAppTableRecord);
};

/** \details
    This template class is a specialization of the OdSmartPtr class for OdDgRegAppTableRecord object pointers.
*/
typedef OdSmartPtr<OdDgRegAppTableRecord> OdDgRegAppTableRecordPtr;


#include "TD_PackPop.h"

#endif // __DG_REGAPPTABLERECORD_H__
