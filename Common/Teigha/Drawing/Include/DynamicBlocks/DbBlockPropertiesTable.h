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

#ifndef _DbBlockPropertiesTable_h_Included_
#define _DbBlockPropertiesTable_h_Included_

#include "DbBlock1PtParameter.h"
#include "DbBlockGrips.h"

/** \details
    Corresponding C++ library:  TD_Db
    <group OdDb_Classes> 
*/
class DYNBLOCKS_EXPORT OdDbBlockUserParameter : public OdDbBlock1PtParameter
{
public:
  ODDB_EVAL_DECLARE_MEMBERS(OdDbBlockUserParameter);
  OdDbBlockUserParameter();
};
typedef OdSmartPtr<OdDbBlockUserParameter> OdDbBlockUserParameterPtr;

/** \details
    Corresponding C++ library:  TD_Db
    <group OdDb_Classes> 
*/
class DYNBLOCKS_EXPORT OdDbBlockPropertiesTable : public OdDbBlock1PtParameter
{
public:
  ODDB_EVAL_DECLARE_MEMBERS(OdDbBlockPropertiesTable);
  OdDbBlockPropertiesTable();
};
typedef OdSmartPtr<OdDbBlockPropertiesTable> OdDbBlockPropertiesTablePtr;

/** \details
    Corresponding C++ library:  TD_Db
    <group OdDb_Classes> 
*/
class DYNBLOCKS_EXPORT OdDbBlockPropertiesTableGrip : public OdDbBlockGrip
{
public:
  ODDB_DECLARE_MEMBERS(OdDbBlockPropertiesTableGrip);
  OdDbBlockPropertiesTableGrip();
};
typedef OdSmartPtr<OdDbBlockPropertiesTableGrip> OdDbBlockPropertiesTableGripPtr;

#endif //_DbBlockPropertiesTable_h_Included_
