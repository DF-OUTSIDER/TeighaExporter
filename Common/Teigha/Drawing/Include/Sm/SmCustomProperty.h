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




#ifndef _SmCustomProperty_h_Included_
#define _SmCustomProperty_h_Included_

#include "Sm/SmPersist.h"
#define STL_USING_STRING
#include "OdaSTL.h"
#include <RxObject.h>
#include <RxVariant.h>
#include "Sm/SmExport.h"

/** \details
  
    <group TD_Namespaces>
*/
namespace dst
{
  enum PropertyFlags
  {
    EMPTY = 0,
    CUSTOM_SHEETSET_PROP = 1,
    CUSTOM_SHEET_PROP = 2,
    IS_CHILD = 4
  };
  
  /** \details

       
  */
  struct SMDLL_EXPORT OdSmCustomPropertyValue : public OdSmPersist
  {
    ODRX_DECLARE_MEMBERS( OdSmCustomPropertyValue );
    virtual OdVariant getValue() const = 0;
    virtual void setValue ( const OdVariant& value ) = 0;
    virtual PropertyFlags getFlags() const = 0;
    virtual void setFlags( PropertyFlags value ) = 0;
  };
  typedef OdSmartPtr<OdSmCustomPropertyValue> OdSmCustomPropertyValuePtr;
  
  /** \details

       
  */
  struct OdSmEnumCustomPropertyValue : public OdRxObject
  {
    virtual bool next( OdString& propname, const OdSmCustomPropertyValue*& ppValue ) = 0;
    virtual void reset() = 0;
  };
  typedef OdSmartPtr<OdSmEnumCustomPropertyValue> OdSmEnumCustomPropertyValuePtr;
  
  /** \details

       
  */
  struct SMDLL_EXPORT OdSmCustomPropertyBag : OdSmPersist
  {
    ODRX_DECLARE_MEMBERS(OdSmCustomPropertyBag);
    virtual const OdSmCustomPropertyValue* getProperty( const OdString& name ) const = 0;
    virtual void setProperty(const OdString& name, OdSmCustomPropertyValue* value ) = 0;
    virtual OdSmEnumCustomPropertyValuePtr getPropertyEnumerator() const = 0;
    virtual bool removeProperty(const OdString& name) = 0;
  };
  typedef OdSmartPtr<OdSmCustomPropertyBag> OdSmCustomPropertyBagPtr;
}

#endif
