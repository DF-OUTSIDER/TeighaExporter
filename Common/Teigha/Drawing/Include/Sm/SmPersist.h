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




#ifndef _SmPersist_h_Included_
#define _SmPersist_h_Included_

#include <RxObject.h>
#define STL_USING_STRING
#define STL_USING_VECTOR
#include <OdaSTL.h>
#include <OdString.h>
#include "Sm/SmExport.h"

/** \details
  
    <group TD_Namespaces>
*/
namespace dst
{
  struct OdSmFiler;
  struct OdSmDatabase;
  struct OdSmObjectId;

  /** \details

       
  */
  struct SMDLL_EXPORT OdSmPersist : public OdRxObject
  {
    ODRX_DECLARE_MEMBERS(OdSmPersist);
    virtual OdString clsid() const = 0;
    virtual bool isDirty() const = 0;
    virtual bool load ( OdSmFiler * pFiler ) = 0;
    virtual void save ( OdSmFiler * pFiler ) const = 0;
    virtual OdString typeName() const = 0;
    virtual void initNew( const OdSmPersist* pOwner, const OdString& handle = OdString::kEmpty ) = 0;
    virtual const OdSmPersist* owner() const = 0;
    virtual void setOwner( const OdSmPersist * pOwner ) = 0;
    virtual const OdSmDatabase* database() const = 0;
    virtual const OdSmObjectId* objectId() const = 0;
    virtual void clear() = 0;
    virtual void directlyOwnedObjects( std::vector<OdSmPersist*>& ) const = 0;
  };
  
  typedef OdSmartPtr<OdSmPersist> OdSmPersistPtr;
}

#endif
