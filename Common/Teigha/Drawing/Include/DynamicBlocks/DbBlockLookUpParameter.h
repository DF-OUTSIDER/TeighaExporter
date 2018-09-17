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

#ifndef _DbBlockLookUpParameter_h_Included_
#define _DbBlockLookUpParameter_h_Included_

#include "DbBlock1PtParameter.h"

class OdDbBlockLookupGrip;
typedef OdSmartPtr<OdDbBlockLookupGrip> OdDbBlockLookupGripPtr;

/** \details

    \sa
    TD_Db

    <group OdDb_Classes> 
*/
class DYNBLOCKS_EXPORT OdDbBlockLookUpParameter : public OdDbBlock1PtParameter
{
public:
  ODDB_EVAL_DECLARE_MEMBERS(OdDbBlockLookUpParameter);
  OdDbBlockLookUpParameter();
  OdDbBlockLookupGripPtr getAssociatedLookupGrip( OdDb::OpenMode );
  virtual bool historyRequired() const;
  OdString lookupActionName() const;
  OdString lookupDescription() const;
  OdString lookupName() const;
  void setLookupActionName(const OdString&);
  void setLookupDescription(const OdString&);
  void setLookupName(const OdString&);
};

typedef OdSmartPtr<OdDbBlockLookUpParameter> OdDbBlockLookUpParameterPtr;

#endif // _DbBlockLookUpParameter_h_Included_
