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




#ifndef _SmSheet_h_Included_
#define _SmSheet_h_Included_

#include "Sm/SmComponent.h"
#include "Sm/SmNamedDbObjectReference.h"

/** \details
  
    <group TD_Namespaces>
*/
namespace dst
{
  struct OdSmSheetViews;

  /** \details
    Sheet component
  */
  struct SMDLL_EXPORT OdSmSheet : OdSmComponent
  {
    ODRX_DECLARE_MEMBERS(OdSmSheet);
    // Retrieves the number property of the sheet
    virtual OdString getNumber() = 0;
    // Sets/Modifies the number property of the sheet
    virtual void setNumber(const OdString&) = 0;
    // Retrieves the title property of the sheet
    virtual OdString getTitle() = 0;
    // Sets/Modifies the number property of the sheet
    virtual void setTitle (const OdString&) = 0;
    // Returns the OdSmDbLayoutReference which references the OdDbLayout that is associated with this sheet component.
    virtual OdSmDbLayoutReference* getLayout() = 0;
    // Sets the OdSmDbLayoutReference which references the OdDbLayout that is associated with this sheet component.
    virtual void setLayout( OdSmDbLayoutReference* pLayoutRef ) = 0;
    // Returns a boolean flag that signifies if this sheet should be skipped during plot/publish.
    virtual bool getDoNotPlot() = 0;
    // Sets a boolean flag that signifies if this sheet should be skipped during plot/publish.
    virtual void setDoNotPlot( bool doNotPlot ) = 0;
    virtual OdSmSheetViews* getSheetViews() = 0;
    
    // New properties added in 1.1
    
    // Retrieves the Revision Number this sheet is associated with.
    virtual OdString getRevisionNumber() const = 0;
    // Set the Revision Number this sheet is associated with.
    virtual void setRevisionNumber(const OdString& newVal) = 0;
    // Retrieves the Revision Date property.
    virtual OdString getRevisionDate() const = 0;
    // Set the Revision Date property.
    virtual void setRevisionDate(const OdString& newVal) = 0;
    // Retrieves the Issue Purpose property.
    virtual OdString getIssuePurpose() const = 0;
    // Set the Issue Purpose property.
    virtual void setIssuePurpose(const OdString& newVal) = 0;
    // Retrieves the Category property.
    virtual OdString getCategory() const = 0;
    // Set the Category property.
    virtual void setCategory(const OdString& newVal) = 0;
  };
  typedef OdSmartPtr<OdSmSheet> OdSmSheetPtr;
}

#endif //_SmSheet_h_Included_
