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

#ifndef _DbBlockVisibilityParameter_h_Included_
#define _DbBlockVisibilityParameter_h_Included_

#include "DbBlock1PtParameter.h"

/** \details

    \sa
    TD_Db

    <group OdDb_Classes> 
*/
class DYNBLOCKS_EXPORT OdDbBlockVisibilityParameter : public OdDbBlock1PtParameter
{
public:
  ODDB_EVAL_DECLARE_MEMBERS(OdDbBlockVisibilityParameter);
  OdDbBlockVisibilityParameter();
  bool isInitialized() const;
  void setInitiaized(bool);
  int currentVisibilityState()const;
  void setCurrentVisibilityState(int);
  void clearListOfVisibilityStates();
  OdString visibilityName() const;
  void setVisibilityName(const OdString&);
  OdString visibilityDescription() const;
  void setVisibilityDescription(const OdString&);
  void blockEntityList( OdDbObjectIdArray& ) const;
  void setBlockEntityList( OdDbObjectIdArray& );
  int numberOfVisibilityStates() const;
  void visibilityState(int index, OdString& name, OdDbObjectIdArray& objects, OdDbObjectIdArray& elements);
  void setVisibilityState(int index,const OdString& name, const OdDbObjectIdArray& objects, const OdDbObjectIdArray& elements);
  virtual void getPropertyDescription(OdDbBlkParamPropertyDescriptorArray& ) const ODRX_OVERRIDE;
  void listVisibleEntities( OdDbObjectIdArray& ) const;
  virtual void setMemberOfCurrentVisibilitySet(bool) ODRX_OVERRIDE {}
};
typedef OdSmartPtr<OdDbBlockVisibilityParameter> OdDbBlockVisibilityParameterPtr;

#endif // _DbBlockVisibilityParameter_h_Included_
