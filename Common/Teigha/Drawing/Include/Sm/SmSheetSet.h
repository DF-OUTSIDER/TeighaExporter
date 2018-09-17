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




#ifndef _SmSheetSet_h_Included_
#define _SmSheetSet_h_Included_

#include "Sm/SmSubset.h"
#include "Sm/SmProjectPointLocations.h"

/** \details
  
    <group TD_Namespaces>
*/
namespace dst
{
  struct OdSmSheetSelSets;
  struct OdSmResources;
  struct OdSmCalloutBlocks;
  struct OdSmViewCategories;
  struct OdSmPublishOptions;
  
  /** \details

       
  */
  struct SMDLL_EXPORT OdSmSheetSet : OdSmSubset
  {
    ODRX_DECLARE_MEMBERS(OdSmSheetSet);
    // Retrieves the template containing alternative page setups
    virtual const OdSmFileReference* getAltPageSetups() const = 0;
    //
    virtual void setAltPageSetups ( OdSmFileReference * pDwtRef ) = 0;
    // Retrieves the default page setup within the template containing alternative page setups
    virtual const OdSmNamedDbObjectReference* getDefAltPageSetup() const = 0;
    //
    virtual void setDefAltPageSetup ( OdSmNamedDbObjectReference * pAltPageSetup ) = 0;
    //
    virtual bool getPromptForDwgName() const = 0;
    virtual void setPromptForDwgName( bool askForName ) = 0;
    // Returns a collection of the sheet selection sets associated with the sheet set.
    virtual OdSmSheetSelSets* getSheetSelSets() = 0;
	// Returns a collection of the sheet selection sets associated with the sheet set.
    virtual const OdSmSheetSelSets* getSheetSelSets() const = 0;
    // Returns a collection of the resources associated with the sheet set
    virtual OdSmResources* getResources() = 0;
	// Returns a collection of the resources associated with the sheet set
    virtual const OdSmResources* getResources() const = 0;
    // Returns a collection of callout block references that are associated with the sheet 
    virtual OdSmCalloutBlocks* getCalloutBlocks() = 0;
	// Returns a collection of callout block references that are associated with the sheet 
    virtual const OdSmCalloutBlocks* getCalloutBlocks() const = 0;
    // reserved
    virtual OdSmViewCategories* getViewCategories() = 0;
	// reserved
    virtual const OdSmViewCategories* getViewCategories() const = 0;
    // Returns the label block definition for the sheet set. 
    virtual const OdSmDbBlockRecordReference* getDefLabelBlk() const = 0;
    // sets the label block definition for the sheet set. 
    virtual void setDefLabelBlk ( OdSmDbBlockRecordReference* pLabelBlkRef ) = 0;
    // Returns the collection of the publish options contained in this sheet set.
    virtual OdSmPublishOptions* getPublishOptions() = 0;
	// Returns the collection of the publish options contained in this sheet set.
    virtual const OdSmPublishOptions* getPublishOptions() const = 0;
    //
    virtual long subscribe( OdSmEvents * eventHandler ) = 0;
    virtual void unSubscribe( long cookie ) = 0;
    // internal
    virtual void updateSheetCustomProps() = 0;

    // New properties available in 1.1 version (AcSmComponents16)

    // Retrieves the Project Number this sheet set is associated with.
    virtual OdString getProjectNumber() const = 0;
    // "Set the Project Number this sheet set is associated with."
    virtual void setProjectNumber(const OdString& newVal) = 0;
    // "Retrieves the Project Name this sheet set is associated with."
    virtual OdString getProjectName() const = 0;
    // "Set the Project Name this sheet set is associated with."
    virtual void setProjectName(const OdString& newVal) = 0;
    // "Retrieves the Project Phase property."
    virtual OdString getProjectPhase() const = 0;
    // "Set the Project Phase property."
    virtual void setProjectPhase(const OdString& newVal) = 0;
    // "Retrieves the Project Milestone property."
    virtual OdString getProjectMilestone() const = 0;
    // "Set the Project Milestone property."
    virtual void setProjectMilestone(const OdString& newVal) = 0;
  };
  typedef OdSmartPtr<OdSmSheetSet> OdSmSheetSetPtr;
}

#endif
