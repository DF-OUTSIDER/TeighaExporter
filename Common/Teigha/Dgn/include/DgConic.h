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
#ifndef __DG_CONIC_H__
#define __DG_CONIC_H__

#include "TD_PackPush.h"

#include "DgMultiVertex.h"


/** \details
   
    Corresponding C++ library: TG_Db

    <group OdDg_Classes>
*/
class TG_EXPORT OdDgConic2d : public OdDgMultiVertex2d
{
  ODDG_DECLARE_MEMBERS_INTERNAL(OdDgConic2d);
public:
  //no additions to the MultiVertex object
};

typedef OdSmartPtr<OdDgConic2d> OdDgConic2dPtr;


/** \details
   
    Corresponding C++ library: TG_Db

    <group OdDg_Classes>
*/
class TG_EXPORT OdDgConic3d : public OdDgMultiVertex3d
{
  ODDG_DECLARE_MEMBERS_INTERNAL(OdDgConic3d);
public:
  //no additions to the MultiVertex object
};

typedef OdSmartPtr<OdDgConic3d> OdDgConic3dPtr;


#include "TD_PackPop.h"

#endif // __DG_CONIC_H__
