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
#ifndef __DG_ACS_H__
#define __DG_ACS_H__

#include "TD_PackPush.h"

#include "DgModel.h"


/** \details
   
    Corresponding C++ library: TG_Db 
    <group OdDg_Classes>
*/
class TG_EXPORT OdDgACS : public OdDgElement
{
  ODDG_DECLARE_MEMBERS_INTERNAL(OdDgACS);

public:

  virtual OdString getName() const;  
  virtual void     setName( const OdString& strName );

  virtual OdString getDescription() const;  
  virtual void     setDescription( const OdString& strDescription );

  virtual OdGePoint3d getOrigin() const;
  virtual void setOrigin( const OdGePoint3d& ptOrigin );

  virtual OdGeMatrix3d getRotation() const;
  virtual void setRotation( const OdGeMatrix3d& matRotation );

  virtual OdDgModel::AcsType getType() const;
  virtual void setType( OdDgModel::AcsType uType );
};

typedef OdSmartPtr<OdDgACS> OdDgACSPtr;

#include "TD_PackPop.h"

#endif // __DG_CONE_H__
