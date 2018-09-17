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




#ifndef _SmComponent_h_Included_
#define _SmComponent_h_Included_

#include "Sm/SmPersist.h"
#include "Sm/SmCustomProperty.h"
#define STL_USING_STRING
#include "OdaSTL.h"
#include "Sm/SmEnum.h"

/** \details

    <group TD_Namespaces>
*/
namespace dst
{
  /** \details

       
  */
  struct SMDLL_EXPORT OdSmComponent : OdSmPersist
  {
    ODRX_DECLARE_MEMBERS(OdSmComponent);
    virtual OdString getName() const = 0;
    virtual void setName( const OdString& name ) = 0;
    virtual OdString getDesc() const = 0;
    virtual void setDesc( const OdString& desc ) = 0;
    virtual OdSmCustomPropertyBag* customProperties()= 0;
    virtual const OdSmCustomPropertyBag* customProperties() const = 0;
  };
  typedef OdSmartPtr<OdSmComponent> OdSmComponentPtr;
  typedef OdSmEnum<OdSmComponent> OdSmEnumComponent;
  typedef OdSmartPtr<OdSmEnumComponent> OdSmEnumComponentPtr;
}

#endif // _SmComponent_h_Included_
