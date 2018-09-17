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




#ifndef _SmNamedDbObjectReference_h_Included_
#define _SmNamedDbObjectReference_h_Included_

#include "Sm/SmDbObjectReference.h"
#include <OdString.h>

/** \details
  
    <group TD_Namespaces>
*/
namespace dst
{
  /** \details

       
  */
  struct SMDLL_EXPORT OdSmNamedDbObjectReference : OdSmDbObjectReference
  {
    ODRX_DECLARE_MEMBERS(OdSmNamedDbObjectReference);
    virtual void setName(const OdString& name ) = 0;
    virtual OdString getName() const = 0;
    // owner currently is not saved to file, because these methods are not 
    // implemented in "acsmcomponents16.dll" library, and correct tag name for OwnerDbHandle is unknown
    virtual void setOwnerDbHandle(const OdString& handle ) = 0;
    virtual OdString getOwnerDbHandle() const = 0;
  };
  
  /** \details

       
  */
  struct SMDLL_EXPORT OdSmDbBlockRecordReference : OdSmNamedDbObjectReference 
  {
    ODRX_DECLARE_MEMBERS(OdSmDbBlockRecordReference);
  };

  /** \details

       
  */
  struct SMDLL_EXPORT OdSmDbViewReference : OdSmNamedDbObjectReference 
  {
    ODRX_DECLARE_MEMBERS(OdSmDbViewReference);
  };

  /** \details

       
  */
  struct SMDLL_EXPORT OdSmDbLayoutReference : OdSmNamedDbObjectReference 
  {
    ODRX_DECLARE_MEMBERS(OdSmDbLayoutReference);
  };
  
  typedef OdSmartPtr<OdSmNamedDbObjectReference> OdSmNamedDbObjectReferencePtr;
  typedef OdSmartPtr<OdSmDbBlockRecordReference>OdSmDbBlockRecordReferencePtr;
  typedef OdSmartPtr<OdSmDbViewReference>OdSmDbViewReferencePtr;
  typedef OdSmartPtr<OdSmDbLayoutReference>OdSmDbLayoutReferencePtr;
}

#endif //_SmNamedDbObjectReference_h_Included_
