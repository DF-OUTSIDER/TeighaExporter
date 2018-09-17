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




#ifndef _SmProjectPointLocations_h_Included_
#define _SmProjectPointLocations_h_Included_

#include "Sm/SmProjectPointLocation.h"
#include "Sm/SmEnum.h"
#include "Sm/SmComponent.h"

/** \details
  
    <group TD_Namespaces>
*/
namespace dst
{
  typedef OdSmEnum<OdSmProjectPointLocation> OdSmEnumProjectPointLocation;
  typedef OdSmartPtr<OdSmEnumProjectPointLocation> OdSmEnumProjectPointLocationPtr;

  /** \details

       
  */
  struct SMDLL_EXPORT OdSmProjectPointLocations : OdSmComponent
  {
    ODRX_DECLARE_MEMBERS(OdSmProjectPointLocations);
    virtual const OdSmProjectPointLocation* getLocation(const OdString& locationName ) const = 0;
    virtual void removeLocation( OdSmProjectPointLocation* location ) = 0;
    virtual OdSmProjectPointLocation* addNewLocation(const OdString& name, const OdString& url, 
      const OdString& folder, const OdString& username, const OdString& password ) = 0;
    virtual OdSmEnumProjectPointLocationPtr getEnumerator() const = 0;
  };
  typedef OdSmartPtr<OdSmProjectPointLocations> OdSmProjectPointLocationsPtr;
}

#endif //_SmProjectPointLocations_h_Included_
