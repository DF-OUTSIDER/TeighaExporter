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




#ifndef _SmViewCategories_h_Included_
#define _SmViewCategories_h_Included_

#include "Sm/SmComponent.h"
#include "Sm/SmViewCategory.h"
#include "Sm/SmEnum.h"

/** \details
  
    <group TD_Namespaces>
*/
namespace dst
{
  typedef OdSmEnum<OdSmViewCategory>OdSmEnumViewCategory;
  typedef OdSmartPtr<OdSmEnumViewCategory> OdSmEnumViewCategoryPtr;
  
  /** \details

       
  */
  struct SMDLL_EXPORT OdSmViewCategories : OdSmComponent
  {
    ODRX_DECLARE_MEMBERS(OdSmViewCategories);
    virtual OdSmEnumViewCategoryPtr getEnumerator() const = 0;
    virtual OdSmViewCategory* createViewCategory( const OdString& name, const OdString& desc, const OdString& id ) = 0;
    virtual void insertViewCategory(OdSmViewCategory* newCat, OdSmViewCategory* beforeCat = 0) = 0;
    virtual void insertViewCategoryAfter(OdSmViewCategory* newCat, OdSmViewCategory* afterCat = 0) = 0;
    virtual void removeViewCategory( OdSmViewCategory * viewCat ) = 0;
    virtual OdSmViewCategory* getDefaultViewCategory() = 0;
  };
  typedef OdSmartPtr<OdSmViewCategories> OdSmViewCategoriesPtr;
}

#endif //_SmViewCategories_h_Included_
