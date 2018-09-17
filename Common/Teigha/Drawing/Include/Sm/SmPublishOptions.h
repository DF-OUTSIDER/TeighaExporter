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




#ifndef _SmPublishOptions_h_Included_
#define _SmPublishOptions_h_Included_

#include "Sm/SmPersist.h"
#include "Sm/SmFileReference.h"
#include "Sm/SmCustomProperty.h"

/** \details
  
    <group TD_Namespaces>
*/
namespace dst
{
  /** \details

       
  */
  struct SMDLL_EXPORT OdSmPublishOptions : OdSmPersist
  {
    ODRX_DECLARE_MEMBERS(OdSmPublishOptions);
    virtual const OdSmFileReference* getDefaultOutputdir() const = 0;
    virtual void setDefaultOutputdir( OdSmFileReference* pValue ) = 0;
    virtual bool getDwfType() const = 0;
    virtual void setDwfType( bool value ) = 0;
    virtual bool getPromptForName() const = 0;
    virtual void setPromptForName( bool value ) = 0;
    virtual bool getUsePassword() const = 0;
    virtual void setUsePassword( bool value ) = 0;
    virtual bool getPromptForPassword() const = 0;
    virtual void setPromptForPassword( bool value ) = 0;
    virtual bool getLayerInfo() const = 0;
    virtual void setLayerInfo( bool value ) = 0;
    virtual const OdSmCustomPropertyBag* getUnrecognizedData() const = 0;
    virtual void setUnrecognizedData( OdSmCustomPropertyBag * bag ) = 0;
    virtual const OdSmCustomPropertyBag* getUnrecognizedSections() const = 0;
    virtual void setUnrecognizedSections( OdSmCustomPropertyBag* sectionArray ) = 0;
    // Retrieves the boolean flag that indicates if the application should include SheetSet Data.
    virtual bool getIncludeSheetSetData() const = 0;
    //Sets/Modifies the boolean flag that indicates if the application should include SheetSet Data property.
    virtual void setIncludeSheetSetData(bool value) = 0;
    // Retrieves the boolean flag that indicates if the application should include Sheet Data.
    virtual bool getIncludeSheetData() const = 0;
    //Sets/Modifies the boolean flag that indicates if the application should include Sheet Data.
    virtual void setIncludeSheetData(bool value) = 0;
  };
  typedef OdSmartPtr<OdSmPublishOptions> OdSmPublishOptionsPtr;
}
#endif //_SmPublishOptions_h_Included_
