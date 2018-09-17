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

#ifndef ODDGNLS_DBLSCOMPONENT_H
#define ODDGNLS_DBLSCOMPONENT_H

#include "DbLSObject.h"

#include "TD_PackPush.h"

enum OdDbLSComponentType
{
  kLSSymbolComponent        = 1,
  kLSCompoundComponent      = 2,
  kLSStrokePatternComponent = 3,
  kLSPointComponent         = 4,
  kLSInternalComponent      = 6
};

/** \details
    Dgn Line Style Component Base Class.
    \sa
    TD_Db
    <group OdDbLS_Classes> 
*/
class ODDGNLS_EXPORT OdDbLSComponent : public OdDbLSObject
{
  public:
    ODDB_DECLARE_MEMBERS(OdDbLSComponent);

    /** \details
      Teigha applications typically will not use this constructor, insofar as 
      this class is a base class.
    */
    OdDbLSComponent();

    OdDbLSComponentType componentType() const;
    void setComponentType(OdDbLSComponentType type);

    const OdUInt8 *componentUID() const;

    OdString description() const;
    void setDescription(const OdString &str);
};

/** \details
    This template class is a specialization of the OdSmartPtr class for OdDbLSComponent object pointers.
*/
typedef OdSmartPtr<OdDbLSComponent> OdDbLSComponentPtr;

#include "TD_PackPop.h"

#endif // ODDGNLS_DBLSCOMPONENT_H
